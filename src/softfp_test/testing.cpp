#include <boost/test/unit_test.hpp>

#include <unistd.h>   // for isatty
#include <csignal>    // for signal
//#include <mpfr.h>     // for mpfr_fma

#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "controller.hpp"
#include "hardfloat.hpp"
#include "sequencer.hpp"

namespace postrisc {
namespace fpu {

/******************************************************************************
* per-test fixture
******************************************************************************/
TestSequencer::TestSequencer(void)
{
    BOOST_TEST_MESSAGE("TestSequencer ctor");
}

TestSequencer::~TestSequencer(void)
{
    BOOST_TEST_MESSAGE("TestSequencer dtor");
}

void TestSequencer::setup(void)
{
    BOOST_TEST_MESSAGE("TestSequencer setup");
}

void TestSequencer::teardown(void)
{
    BOOST_TEST_MESSAGE("TestSequencer teardown");
}

/******************************************************************************
* write input/result value, hex + usual format
******************************************************************************/
static void writeValue(std::ostream& out, const bool v) { out << v; }
using postrisc::operator<<;
static void writeValue(std::ostream& out, const  i32 v) { out << fmt::hex(v) << ' ' << v; }
static void writeValue(std::ostream& out, const  u32 v) { out << fmt::hex(v) << ' ' << v; }
static void writeValue(std::ostream& out, const  i64 v) { out << fmt::hex(v) << ' ' << v; }
static void writeValue(std::ostream& out, const  u64 v) { out << fmt::hex(v) << ' ' << v; }
static void writeValue(std::ostream& out, const i128 v) { out << fmt::hex(v) << ' ' << v; }
static void writeValue(std::ostream& out, const u128 v) { out << fmt::hex(v) << ' ' << v; }

template<typename T>
static void writeClassify(std::ostream& out, T f)
{
    if (is_inf(f)) out << " inf";
    if (is_nan(f)) out << " nan";
    if (is_signan(f)) out << " signan";
}

static void writeValue(std::ostream& out, const f16 f)
{
    out << fmt::hex<u16, 2>(f.val() >> f.MANT_SIZE)
        << '.'
        << fmt::hex<u16, 3>(f.get_fraction())
        << ' '
        << std::setprecision(std::numeric_limits<f16::F_HARD>::max_digits10)
        << (float)f.hard();

    writeClassify(out, f);
}

static void writeValue(std::ostream& out, const f32 f)
{
    out << fmt::hex<u32, 3>(f.val() >> f.MANT_SIZE)
        << '.'
        << fmt::hex<u32, 6>(f.get_fraction())
        << ' '
        << std::setprecision(std::numeric_limits<f32::F_HARD>::max_digits10)
        << f.hard();

    writeClassify(out, f);
}

static void writeValue(std::ostream& out, const f64 f)
{
    out << fmt::hex<u64, 3>(f.val() >> f.MANT_SIZE)
        << '.'
        << fmt::hex<u64, 13>(f.get_fraction())
        << ' '
        << std::setprecision(std::numeric_limits<f64::F_HARD>::max_digits10)
        << f.hard();

    writeClassify(out, f);
}

static void writeValue(std::ostream& out, const f128 f)
{
    using postrisc::operator<<;

    out << fmt::hex<u32, 4>(static_cast<u32>(f.val() >> f.MANT_SIZE))
        << '.'
        << fmt::hex<u64, 12>(static_cast<u64>(f.get_fraction() >> 64))
        << fmt::hex<u64, 16>(static_cast<u64>(f.get_fraction()))
        << ' '
        << std::setprecision(std::numeric_limits<f128::F_HARD>::max_digits10)
        << f.hard();

    writeClassify(out, f);
}

/**************************************************************************************
* template op helpers
**************************************************************************************/
class add    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.add(a, b); }};
class sub    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.sub(a, b); }};
class mul    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.mul(a, b); }};
class div    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.div(a, b); }};

class compare_ordered      { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered     (a, b); }};
class compare_ordered_eq   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_eq  (a, b); }};
class compare_ordered_ne   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_ne  (a, b); }};
class compare_ordered_lt   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_lt  (a, b); }};
class compare_ordered_ge   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_ge  (a, b); }};
class compare_unordered    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered   (a, b); }};
class compare_unordered_eq { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_eq(a, b); }};
class compare_unordered_ne { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_ne(a, b); }};
class compare_unordered_lt { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_lt(a, b); }};
class compare_unordered_ge { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_ge(a, b); }};

class muladd { public: template<typename DST, typename SRC, class FPU>
    static DST func(FPU& fpu, SRC a, SRC b, SRC c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_NONE); }};

class exp     { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.exp(a); }};
class sqrt    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.sqrt(a); }};
class log2    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.log2(a); }};
class extend  { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.template extend<DST, SRC>(a); }};
class cvt_f_f { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.template cvt_f_f<DST, SRC>(a, fpu.get_rounding_mode()); }};
class cvt_i_f { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.template cvt_i_f<DST, SRC>(a, fpu.get_rounding_mode()); }};
class cvt_u_f { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.template cvt_i_f<DST, SRC>(a, fpu.get_rounding_mode()); }};
class cvt_f_i { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.template cvt_f_i<DST, SRC>(a, fpu.get_rounding_mode()); }};
class cvt_f_u { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a) { return fpu.template cvt_f_i<DST, SRC>(a, fpu.get_rounding_mode()); }};

/**************************************************************************************
* error logging helpers
**************************************************************************************/
template<typename T> class FlagsAndResult;
template<typename T> std::ostream& operator<<(std::ostream& out, const FlagsAndResult<T>& r);

template<typename T> class FlagsAndResult {
public:
    FlagsAndResult(T _v, fp_flags _flags) : v(_v), flags(_flags) {}
    friend std::ostream& operator<< <> (std::ostream& out, const FlagsAndResult& r);
private:
    T        v;
    fp_flags flags;
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const FlagsAndResult<T>& r)
{
    out << r.flags << ' ';
    writeValue(out, r.v);
    out << fmt::endl;
    return out;
}

template<typename T> class InputData_1 {
public:
    InputData_1(T _a) : a(_a) {}
    friend std::ostream& operator<<(std::ostream& out, const InputData_1& r) {
        out << "1: ";  writeValue(out, r.a);  out << fmt::endl;
        return out;
    }
private:
    T a;
};

template<typename T> class InputData_2 {
public:
    InputData_2(T _a, T _b) : a(_a), b(_b) {}
    friend std::ostream& operator<<(std::ostream& out, const InputData_2& r) {
        out << "1: ";  writeValue(out, r.a);  out << fmt::endl;
        out << "2: ";  writeValue(out, r.b);  out << fmt::endl;
        return out;
    }
private:
    T a,b;
};

template<typename T> class InputData_3 {
public:
    InputData_3(T _a, T _b, T _c) : a(_a), b(_b), c(_c) {}
    friend std::ostream& operator<<(std::ostream& out, const InputData_3& r) {
        out << "1: ";  writeValue(out, r.a);  out << fmt::endl;
        out << "2: ";  writeValue(out, r.b);  out << fmt::endl;
        out << "3: ";  writeValue(out, r.c);  out << fmt::endl;
        return out;
    }
private:
    T a,b,c;
};

/**************************************************************************************
* test templates helpers
**************************************************************************************/
template<typename T> bool is_int_nan(T v)
{
    static_assert(std::numeric_limits<T>::is_integer);
    return v == std::numeric_limits<T>::max() ||
           v == std::numeric_limits<T>::min();
}

template<typename T>
bool
TestSequencer::is_ok(T trueZ, fp_flags trueFlags, T testZ, fp_flags testFlags)
{
    bool const is_flags_ok = !TestController::checkFlags || trueFlags == testFlags;

    if constexpr (fp_traits<T>::is_fp()) {
        if (trueZ.val() == testZ.val())
            return is_flags_ok;
        if (!TestController::checkNaNs) {
            if (is_nan(trueZ) && is_nan(testZ))
                return is_flags_ok;
        }
        return false;
    } else { // integer, bool
        if (trueZ == testZ)
            return is_flags_ok;
        if constexpr (std::is_same<T, bool>::value)
            return false;
        if (!TestController::checkNaNs) {
            // integer NaNs for host may be wrong/different, invalid flags check may be enough
            if ((testFlags & trueFlags & FFLAG_INVALID) == FFLAG_INVALID)
                return is_flags_ok;
            //if (is_int_nan(trueZ) && is_int_nan(testZ)) return is_flags_ok;
        }
        return false;
    }
    return false;
}

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
void
TestSequencer::test_1(TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name)
{
    u64 count = 0;
    T a{};

    total = 100 * 1000;
    TestController::writeTestsTotal(total);

    while ((count < total) || TestController::forever) {
        next(a);

        true_fpu.clear_flags();
        const RESULT trueZ = OP::template func<RESULT,T,TRUE_FPU>(true_fpu, a);
        const fp_flags trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        const RESULT testZ = OP::template func<RESULT,T,TEST_FPU>(test_fpu, a);
        const fp_flags testFlags = test_fpu.get_flags();

        bool const ok = is_ok(trueZ, trueFlags, testZ, testFlags);
        if (!ok) {
            std::cout << "error in " << name << fmt::endl
                      << InputData_1(a)
                      << FlagsAndResult(trueZ, trueFlags)
                      << FlagsAndResult(testZ, testFlags)
                      << fmt::endl;
            ++errorCount;
            if (errorCount == TestController::maxErrorCount) break;
        }
        TestController::checkEarlyExit(count, errorCount);
    }

    TestController::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
void
TestSequencer::test_1a(TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name)
{
    u64 count = 0;
    T a{};

    total = 100 * 1000;
    TestController::writeTestsTotal(total);

    while ((count < total) || TestController::forever) {
        next(a);
        a = test_fpu.abs(a);

        true_fpu.clear_flags();
        const RESULT trueZ = OP::template func<RESULT,T,TRUE_FPU>(true_fpu, a);
        const fp_flags trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        const RESULT testZ = OP::template func<RESULT,T,TEST_FPU>(test_fpu, a);
        const fp_flags testFlags = test_fpu.get_flags();

        bool const ok = is_ok(trueZ, trueFlags, testZ, testFlags);
        if (!ok) {
            std::cout << "error in " << name << fmt::endl
                      << InputData_1(a)
                      << FlagsAndResult(trueZ, trueFlags)
                      << FlagsAndResult(testZ, testFlags)
                      << fmt::endl;
            ++errorCount;
            if (errorCount == TestController::maxErrorCount) break;
        }
        TestController::checkEarlyExit(count, errorCount);
    }

    TestController::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
void
TestSequencer::test_2(TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name)
{
    u64 count = 0;
    T a{}, b{};

    total = 1000 * 1000;
    TestController::writeTestsTotal(total);

    while ((count < total) || TestController::forever) {
        next(a, b);

        true_fpu.clear_flags();
        const RESULT trueZ = OP::template func<RESULT,T,TRUE_FPU>(true_fpu, a, b);
        const fp_flags trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        const RESULT testZ = OP::template func<RESULT,T,TEST_FPU>(test_fpu, a, b);
        const fp_flags testFlags = test_fpu.get_flags();

        bool const ok = is_ok(trueZ, trueFlags, testZ, testFlags);
        if (!ok) {
            std::cout << "error in " << name << fmt::endl
                      << InputData_2(a, b)
                      << FlagsAndResult(trueZ, trueFlags)
                      << FlagsAndResult(testZ, testFlags)
                      << fmt::endl;
            ++errorCount;
            if (errorCount == TestController::maxErrorCount) break;
        }
        TestController::checkEarlyExit(count, errorCount);
    }

    TestController::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
void
TestSequencer::test_3(TRUE_FPU& true_fpu, TEST_FPU& test_fpu, char const *name)
{
    u64 count = 0;
    T a{}, b{}, c{};

    total = 1000 * 1000;
    TestController::writeTestsTotal(total);

    while ((count < total) || TestController::forever) {
        next(a, b, c);

        true_fpu.clear_flags();
        const RESULT trueZ = OP::template func<RESULT,T,TRUE_FPU>(true_fpu, a, b, c);
        const fp_flags trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        const RESULT testZ = OP::template func<RESULT,T,TEST_FPU>(test_fpu, a, b, c);
        const fp_flags testFlags = test_fpu.get_flags();

        bool const ok = is_ok(trueZ, trueFlags, testZ, testFlags);
        if (!ok) {
            std::cout << "error in " << name << fmt::endl
                      << InputData_3(a, b, c)
                      << FlagsAndResult(trueZ, trueFlags)
                      << FlagsAndResult(testZ, testFlags)
                      << fmt::endl;
            ++errorCount;
            if (errorCount == TestController::maxErrorCount) break;
        }
        TestController::checkEarlyExit(count, errorCount);
    }

    TestController::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

static const char *get_rm_name(RoundingModeEnum rm)
{
    switch (rm) {
        case RM_RNE: return "tonearest";
        case RM_RTZ: return "towardzero";
        case RM_RDN: return "downward";
        case RM_RUP: return "upward";
        case RM_RMM: return "tomaxmag";
    }
    return "unknown";
}

BOOST_FIXTURE_TEST_CASE(test_sample, TestSequencer)
{
    SoftFPU fpu;
    {
    const i64 v = I64C(0x00007fa85f6753d6);
    const f128 res = fpu.cvt_i_f<f128, i64>(v, fpu::RM_RNE);
    std::cout << "sample: ";
    writeValue(std::cout, res);
    std::cout << std::endl;
    }
    {
    const i32 v = I32C(0x00007ffd);
    const f128 res = fpu.cvt_i_f<f128, i32>(v, fpu::RM_RNE);
    std::cout << "sample: ";
    writeValue(std::cout, res);
    std::cout << std::endl;
    }
}

/**************************************************************************************
* tests
**************************************************************************************/
#define FPU_RM_TESTING_TEST_CASE(TRUE_FPU_TYPE, TEST_FPU_TYPE, Templt, DST, SRC, OP)               \
    static void test_ ## DST ## _  ## SRC ## _ ## OP ## _handler(                                  \
        TestSequencer& ctrl, RoundingModeEnum rm, char const *name)                                \
    {                                                                                              \
        std::cout << "Testing " #DST " " #SRC " " #OP " " << get_rm_name(rm) << ' ';               \
        TRUE_FPU_TYPE true_fpu;                                                                    \
        TEST_FPU_TYPE test_fpu;                                                                    \
        true_fpu.set_rounding_mode(rm);                                                            \
        test_fpu.set_rounding_mode(rm);                                                            \
        ctrl.test_ ## Templt<DST, SRC, OP, TRUE_FPU_TYPE, TEST_FPU_TYPE>                           \
            (true_fpu, test_fpu, name);                                                            \
    }                                                                                              \
                                                                                                   \
    BOOST_FIXTURE_TEST_CASE(test_ ## DST ## _ ## SRC ## _ ## OP ## _tonearest, TestSequencer) {    \
        BOOST_CHECK_NO_THROW(test_ ## DST ## _  ## SRC ## _ ## OP ## _handler                      \
            (*this, RM_RNE, #DST "_" #SRC "_" #OP "_tonearest"));                                  \
    }                                                                                              \
    BOOST_FIXTURE_TEST_CASE(test_ ## DST ## _ ## SRC ## _ ## OP ## _towardzero, TestSequencer) {   \
        BOOST_CHECK_NO_THROW(test_ ## DST ## _  ## SRC ## _ ## OP ## _handler                      \
            (*this, RM_RTZ, #DST "_" #SRC "_" #OP "_towardzero"));                                 \
    }                                                                                              \
    BOOST_FIXTURE_TEST_CASE(test_ ## DST ## _ ## SRC ## _ ## OP ## _upward, TestSequencer) {       \
        BOOST_CHECK_NO_THROW(test_ ## DST ## _  ## SRC ## _ ## OP ## _handler                      \
            (*this, RM_RUP, #DST "_" #SRC "_" #OP "_upward"));                                     \
    }                                                                                              \
    BOOST_FIXTURE_TEST_CASE(test_ ## DST ## _ ## SRC ## _ ## OP ## _downward, TestSequencer) {     \
        BOOST_CHECK_NO_THROW(test_ ## DST ## _  ## SRC ## _ ## OP ## _handler                      \
            (*this, RM_RDN, #DST "_" #SRC "_" #OP "_downward"));                                   \
    }                                                                                              \

#define DEFINE_TEST_RM(Templt, DST, SRC, OP) \
    FPU_RM_TESTING_TEST_CASE(HardFPU, SoftFPU, Templt, DST, SRC, OP)

DEFINE_TEST_RM(2,  f16,   f16,   add)
DEFINE_TEST_RM(2,  f16,   f16,   sub)
DEFINE_TEST_RM(2,  f16,   f16,   mul)
DEFINE_TEST_RM(2,  f16,   f16,   div)
DEFINE_TEST_RM(3,  f16,   f16,   muladd)
DEFINE_TEST_RM(1a, f16,   f16,   sqrt)
DEFINE_TEST_RM(1,  i32,   f16,   cvt_f_i)
DEFINE_TEST_RM(1,  i64,   f16,   cvt_f_i)
DEFINE_TEST_RM(1,  i128,  f16,   cvt_f_i)
DEFINE_TEST_RM(1a, u32,   f16,   cvt_f_u)
DEFINE_TEST_RM(1a, u64,   f16,   cvt_f_u)
DEFINE_TEST_RM(1a, u128,  f16,   cvt_f_u)
DEFINE_TEST_RM(1,  f16,   i32,   cvt_i_f)
DEFINE_TEST_RM(1,  f16,   i64,   cvt_i_f)
DEFINE_TEST_RM(1,  f16,   i128,  cvt_i_f)
DEFINE_TEST_RM(1,  f16,   u32,   cvt_u_f)
DEFINE_TEST_RM(1,  f16,   u64,   cvt_u_f)
DEFINE_TEST_RM(1,  f16,   u128,  cvt_u_f)

DEFINE_TEST_RM(2,  f32,   f32,   add)
DEFINE_TEST_RM(2,  f32,   f32,   sub)
DEFINE_TEST_RM(2,  f32,   f32,   mul)
DEFINE_TEST_RM(2,  f32,   f32,   div)
DEFINE_TEST_RM(3,  f32,   f32,   muladd)
DEFINE_TEST_RM(1a, f32,   f32,   sqrt)
DEFINE_TEST_RM(1,  i32,   f32,   cvt_f_i)
DEFINE_TEST_RM(1,  i64,   f32,   cvt_f_i)
DEFINE_TEST_RM(1,  i128,  f32,   cvt_f_i)
DEFINE_TEST_RM(1a, u32,   f32,   cvt_f_u)
DEFINE_TEST_RM(1a, u64,   f32,   cvt_f_u)
DEFINE_TEST_RM(1a, u128,  f32,   cvt_f_u)
DEFINE_TEST_RM(1,  f32,   i32,   cvt_i_f)
DEFINE_TEST_RM(1,  f32,   i64,   cvt_i_f)
DEFINE_TEST_RM(1,  f32,   i128,  cvt_i_f)
DEFINE_TEST_RM(1,  f32,   u32,   cvt_u_f)
DEFINE_TEST_RM(1,  f32,   u64,   cvt_u_f)
DEFINE_TEST_RM(1,  f32,   u128,  cvt_u_f)

DEFINE_TEST_RM(2,  f64,   f64,   add)
DEFINE_TEST_RM(2,  f64,   f64,   sub)
DEFINE_TEST_RM(2,  f64,   f64,   mul)
DEFINE_TEST_RM(2,  f64,   f64,   div)
DEFINE_TEST_RM(3,  f64,   f64,   muladd)
DEFINE_TEST_RM(1a, f64,   f64,   sqrt)
DEFINE_TEST_RM(1,  i32,   f64,   cvt_f_i)
DEFINE_TEST_RM(1,  i64,   f64,   cvt_f_i)
DEFINE_TEST_RM(1,  i128,  f64,   cvt_f_i)
DEFINE_TEST_RM(1a, u32,   f64,   cvt_f_u)
DEFINE_TEST_RM(1a, u64,   f64,   cvt_f_u)
DEFINE_TEST_RM(1a, u128,  f64,   cvt_f_u)
DEFINE_TEST_RM(1,  f64,   i32,   cvt_i_f)
DEFINE_TEST_RM(1,  f64,   i64,   cvt_i_f)
DEFINE_TEST_RM(1,  f64,   i128,  cvt_i_f)
DEFINE_TEST_RM(1,  f64,   u32,   cvt_u_f)
DEFINE_TEST_RM(1,  f64,   u64,   cvt_u_f)
DEFINE_TEST_RM(1,  f64,   u128,  cvt_u_f)

DEFINE_TEST_RM(2,  f128,  f128,  add)
DEFINE_TEST_RM(2,  f128,  f128,  sub)
DEFINE_TEST_RM(2,  f128,  f128,  mul)
DEFINE_TEST_RM(2,  f128,  f128,  div)
DEFINE_TEST_RM(3,  f128,  f128,  muladd)
DEFINE_TEST_RM(1a, f128,  f128,  sqrt)
DEFINE_TEST_RM(1,  i32,   f128,  cvt_f_i)
DEFINE_TEST_RM(1,  i64,   f128,  cvt_f_i)
DEFINE_TEST_RM(1,  i128,  f128,  cvt_f_i)
DEFINE_TEST_RM(1a, u32,   f128,  cvt_f_u)
DEFINE_TEST_RM(1a, u64,   f128,  cvt_f_u)
DEFINE_TEST_RM(1a, u128,  f128,  cvt_f_u)
DEFINE_TEST_RM(1,  f128,  i32,   cvt_i_f)
DEFINE_TEST_RM(1,  f128,  i64,   cvt_i_f)
DEFINE_TEST_RM(1,  f128,  i128,  cvt_i_f)
DEFINE_TEST_RM(1,  f128,  u32,   cvt_u_f)
DEFINE_TEST_RM(1,  f128,  u64,   cvt_u_f)
DEFINE_TEST_RM(1,  f128,  u128,  cvt_u_f)

DEFINE_TEST_RM(1,  f16,   f32,   cvt_f_f)
DEFINE_TEST_RM(1,  f16,   f64,   cvt_f_f)
DEFINE_TEST_RM(1,  f16,   f128,  cvt_f_f)

DEFINE_TEST_RM(1,  f32,   f64,   cvt_f_f)
DEFINE_TEST_RM(1,  f32,   f128,  cvt_f_f)
DEFINE_TEST_RM(1,  f64,   f128,  cvt_f_f)

DEFINE_TEST_RM(1,  f32,   f16,   extend)
DEFINE_TEST_RM(1,  f64,   f16,   extend)
DEFINE_TEST_RM(1,  f128,  f16,   extend)
DEFINE_TEST_RM(1,  f64,   f32,   extend)
DEFINE_TEST_RM(1,  f128,  f32,   extend)
DEFINE_TEST_RM(1,  f128,  f64,   extend)

#if 0
DEFINE_TEST_RM(1,  f32,   f32,   exp)
DEFINE_TEST_RM(1a, f32,   f32,   log2)
DEFINE_TEST_RM(1a, f64,   f64,   log2)
#endif


#define FPU_TESTING_TEST_CASE(TRUE_FPU_TYPE, TEST_FPU_TYPE, Templt, DST, SRC, OP)                  \
    static void test_ ## DST ## _  ## SRC ## _ ## OP ## _handler(                                  \
        TestSequencer& ctrl, char const *name)                                                     \
    {                                                                                              \
        std::cout << "Testing " #DST " " #SRC " " #OP " ";                                         \
        TRUE_FPU_TYPE true_fpu;                                                                    \
        TEST_FPU_TYPE test_fpu;                                                                    \
        ctrl.test_ ## Templt<DST, SRC, OP, TRUE_FPU_TYPE, TEST_FPU_TYPE>                           \
            (true_fpu, test_fpu, name);                                                            \
    }                                                                                              \
                                                                                                   \
    BOOST_FIXTURE_TEST_CASE(test_ ## DST ## _ ## SRC ## _ ## OP, TestSequencer) {                  \
        BOOST_CHECK_NO_THROW(test_ ## DST ## _  ## SRC ## _ ## OP ## _handler                      \
            (*this, #DST "_" #SRC "_" #OP));                                                       \
    }


#define DEFINE_TEST(Templt, DST, SRC, OP) \
    FPU_TESTING_TEST_CASE(HardFPU, SoftFPU, Templt, DST, SRC, OP)


DEFINE_TEST(2,  bool,  f16,   compare_ordered)
DEFINE_TEST(2,  bool,  f16,   compare_ordered_eq)
DEFINE_TEST(2,  bool,  f16,   compare_ordered_ne)
DEFINE_TEST(2,  bool,  f16,   compare_ordered_lt)
DEFINE_TEST(2,  bool,  f16,   compare_ordered_ge)
DEFINE_TEST(2,  bool,  f16,   compare_unordered)
DEFINE_TEST(2,  bool,  f16,   compare_unordered_eq)
DEFINE_TEST(2,  bool,  f16,   compare_unordered_ne)
DEFINE_TEST(2,  bool,  f16,   compare_unordered_lt)
DEFINE_TEST(2,  bool,  f16,   compare_unordered_ge)

DEFINE_TEST(2,  bool,  f32,   compare_ordered)
DEFINE_TEST(2,  bool,  f32,   compare_ordered_eq)
DEFINE_TEST(2,  bool,  f32,   compare_ordered_ne)
DEFINE_TEST(2,  bool,  f32,   compare_ordered_lt)
DEFINE_TEST(2,  bool,  f32,   compare_ordered_ge)
DEFINE_TEST(2,  bool,  f32,   compare_unordered)
DEFINE_TEST(2,  bool,  f32,   compare_unordered_eq)
DEFINE_TEST(2,  bool,  f32,   compare_unordered_ne)
DEFINE_TEST(2,  bool,  f32,   compare_unordered_lt)
DEFINE_TEST(2,  bool,  f32,   compare_unordered_ge)

DEFINE_TEST(2,  bool,  f64,   compare_ordered)
DEFINE_TEST(2,  bool,  f64,   compare_ordered_eq)
DEFINE_TEST(2,  bool,  f64,   compare_ordered_ne)
DEFINE_TEST(2,  bool,  f64,   compare_ordered_lt)
DEFINE_TEST(2,  bool,  f64,   compare_ordered_ge)
DEFINE_TEST(2,  bool,  f64,   compare_unordered)
DEFINE_TEST(2,  bool,  f64,   compare_unordered_eq)
DEFINE_TEST(2,  bool,  f64,   compare_unordered_ne)
DEFINE_TEST(2,  bool,  f64,   compare_unordered_lt)
DEFINE_TEST(2,  bool,  f64,   compare_unordered_ge)

DEFINE_TEST(2,  bool,  f128,  compare_ordered)
DEFINE_TEST(2,  bool,  f128,  compare_ordered_eq)
DEFINE_TEST(2,  bool,  f128,  compare_ordered_ne)
DEFINE_TEST(2,  bool,  f128,  compare_ordered_lt)
DEFINE_TEST(2,  bool,  f128,  compare_ordered_ge)
DEFINE_TEST(2,  bool,  f128,  compare_unordered)
DEFINE_TEST(2,  bool,  f128,  compare_unordered_eq)
DEFINE_TEST(2,  bool,  f128,  compare_unordered_ne)
DEFINE_TEST(2,  bool,  f128,  compare_unordered_lt)
DEFINE_TEST(2,  bool,  f128,  compare_unordered_ge)

} // namespace fpu
} // namespace postrisc
