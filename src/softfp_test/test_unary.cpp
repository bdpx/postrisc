#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "HardFPU.hpp"
#include "Sequencer.hpp"

namespace postrisc {
namespace fpu {

class round { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.round(a, rm, true /*inexact*/); }};

class exp    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.exp (a, rm); }};
class sqrt   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.sqrt(a, rm); }};
class log2   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.log2(a, rm); }};
class shrink { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.template cvt_f_f<DST, SRC>(a, rm); }};
class extend { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum UNUSED(rm)) { return fpu.template extend<DST, SRC>(a); }};


template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
u32
test_unary(u64 count, TRUE_FPU& true_fpu, TEST_FPU& test_fpu, T a, RoundingModeEnum rm, const char *name)
{
    true_fpu.clear_flags();
    const RESULT trueZ = OP::template func<RESULT,T,HardFPU>(true_fpu, a, rm);
    const fp_flags trueFlags = true_fpu.get_flags();

    test_fpu.clear_flags();
    const RESULT testZ = OP::template func<RESULT,T,SoftFPU>(test_fpu, a, rm);
    const fp_flags testFlags = test_fpu.get_flags();

    bool ok = is_ok(trueZ, trueFlags, testZ, testFlags);

    if constexpr (std::is_same<sqrt, OP>::value && std::is_same<f128, T>::value) {
        // check if quadmath sqrtq doesn't set inexact flag
        if (!ok && trueFlags == FFLAG_NOERROR && testFlags == FFLAG_INEXACT && trueZ.val() == testZ.val()) {
            ok = true;
        }
        // check if quadmath sqrtq(SNaN) returns SNaN without invalid flag
        if (!ok && is_signan(a) && trueFlags == FFLAG_NOERROR && is_signan(trueZ)
                                && testFlags == FFLAG_INVALID && !is_signan(testZ) && is_nan(testZ)) {
            ok = true;
        }
    }

    if (!ok) {
        true_fpu.set_rounding_mode(RM_RNE);
        std::cout << name << " error n=" << count << fmt::endl
                  << "1: " << ValuePrinter(a) << fmt::endl
                  << "true: " << trueFlags << ' ' << ValuePrinter(trueZ) << fmt::endl
                  << "test: " << testFlags << ' ' << ValuePrinter(testZ) << fmt::endl
                  << fmt::endl;
        true_fpu.set_rounding_mode(rm);
        return 1;
    }
    return 0;
}

template<typename RESULT, typename T, class OP>
static void test_handler(RoundingModeEnum rm, char const *name)
{
    Sequencer ctrl;
    Controller::writeTestsTotal(name);

    HardFPU true_fpu;
    SoftFPU test_fpu;
    true_fpu.set_rounding_mode(rm);
    test_fpu.set_rounding_mode(rm);

    u32 errorCount = 0;
    u64 count = 0;

    // check special values
    const auto specials = special_sfs<T>();

    for (const T& a : specials) {
        errorCount += test_unary<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, rm, name);
        count++;
    }

    // check determined sequencer
    for ( ; count < Controller::s_maxCount; count++) {
        T a{};
        ctrl.next(a);
        if constexpr (std::is_same<sqrt, OP>::value || std::is_same<log2, OP>::value) {
            a = test_fpu.abs(a);
        }

        errorCount += test_unary<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, rm, name);
        if (Controller::isEnoughErrors(errorCount))
            break;
        Controller::checkEarlyExit(count, errorCount);
    }

    Controller::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

#define DEFINE_TEST_RM(DST, SRC, OP) \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _ ## OP ## _rne) { test_handler<DST,SRC,OP>(RM_RNE, #SRC "_to_" #DST "_" #OP "_rne"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _ ## OP ## _rtz) { test_handler<DST,SRC,OP>(RM_RTZ, #SRC "_to_" #DST "_" #OP "_rtz"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _ ## OP ## _rup) { test_handler<DST,SRC,OP>(RM_RUP, #SRC "_to_" #DST "_" #OP "_rup"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _ ## OP ## _rdn) { test_handler<DST,SRC,OP>(RM_RDN, #SRC "_to_" #DST "_" #OP "_rdn"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _ ## OP ## _rmm) { test_handler<DST,SRC,OP>(RM_RMM, #SRC "_to_" #DST "_" #OP "_rmm"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _ ## OP ## _rno) { test_handler<DST,SRC,OP>(RM_RNO, #SRC "_to_" #DST "_" #OP "_rno"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _ ## OP ## _rto) { test_handler<DST,SRC,OP>(RM_RTO, #SRC "_to_" #DST "_" #OP "_rto"); } \

DEFINE_TEST_RM(f16,  f16,   round)
DEFINE_TEST_RM(f32,  f32,   round)
DEFINE_TEST_RM(f64,  f64,   round)
DEFINE_TEST_RM(f128, f128,  round)

DEFINE_TEST_RM(f16,  f16,   sqrt)
DEFINE_TEST_RM(f32,  f32,   sqrt)
DEFINE_TEST_RM(f64,  f64,   sqrt)
DEFINE_TEST_RM(f128, f128,  sqrt)
//DEFINE_TEST_RM(f16,  f16,   log2)
//DEFINE_TEST_RM(f32,  f32,   log2)
//DEFINE_TEST_RM(f64,  f64,   log2)
//DEFINE_TEST_RM(f128, f128,  log2)
//DEFINE_TEST_RM(f32,  f32,   exp)

DEFINE_TEST_RM(f16, f32,  shrink)
DEFINE_TEST_RM(f16, f64,  shrink)
DEFINE_TEST_RM(f16, f128, shrink)
DEFINE_TEST_RM(f32, f64,  shrink)
DEFINE_TEST_RM(f32, f128, shrink)
DEFINE_TEST_RM(f64, f128, shrink)

DEFINE_TEST_RM(f32,  f16, extend)
DEFINE_TEST_RM(f64,  f16, extend)
DEFINE_TEST_RM(f128, f16, extend)
DEFINE_TEST_RM(f64,  f32, extend)
DEFINE_TEST_RM(f128, f32, extend)
DEFINE_TEST_RM(f128, f64, extend)

static void test(double value, float expected, RoundingModeEnum rm)
{
    SoftFPU test_fpu;
    test_fpu.set_rounding_mode(RM_RNO);
    const f64 t64 = f64::soft(value);
    const f32 t32_expected = f32::soft(expected);
    const f32 t32 = test_fpu.cvt_f_f<f32, f64>(t64, rm);

    std::cout << rm << " rounded: " << ValuePrinter(t64) << " result: " << ValuePrinter(t32) << " expected: " << ValuePrinter(t32_expected) << fmt::endl;

    BOOST_TEST(t32_expected.val() == t32.val());
}

BOOST_AUTO_TEST_CASE(test_f64_to_f32)
{
    //const native::f32  half_ulp16 = 0x1.0p-11f16;
    const native::f64  half_ulp32 = 0x1.0p-24f;
    //const native::f128 half_ulp64 = 0x1.0p-53;

    //const native::f16 ulp16 = 0x1.0p-10f16;
    const native::f32 ulp32 = 0x1.0p-23f;
    //const native::f64 ulp64 = 0x1.0p-52;

    for (i32 i = 0; i < 10; i++) { test(-1.5 + i * ulp32 + half_ulp32, -1.5f + ((i+1) & ~1) * ulp32, RM_RNE); } // nearest even up
    for (i32 i = 0; i < 10; i++) { test( 1.5 + i * ulp32 + half_ulp32,  1.5f + ((i+1) & ~1) * ulp32, RM_RNE); } // nearest even up

    for (i32 i = 0; i < 10; i++) { test(-1.5 + i * ulp32 + half_ulp32, -1.5f + (i         ) * ulp32, RM_RDN); } // down
    for (i32 i = 0; i < 10; i++) { test( 1.5 + i * ulp32 + half_ulp32,  1.5f + (i         ) * ulp32, RM_RDN); } // down

    for (i32 i = 0; i < 10; i++) { test(-1.5 + i * ulp32 + half_ulp32, -1.5f + (i + 1     ) * ulp32, RM_RUP); } // up
    for (i32 i = 0; i < 10; i++) { test( 1.5 + i * ulp32 + half_ulp32,  1.5f + (i + 1     ) * ulp32, RM_RUP); } // up

    for (i32 i = 0; i < 10; i++) { test(-1.5 + i * ulp32 + half_ulp32, -1.5f + (i + 1     ) * ulp32, RM_RTZ); } // up for negative
    for (i32 i = 0; i < 10; i++) { test( 1.5 + i * ulp32 + half_ulp32,  1.5f + (i         ) * ulp32, RM_RTZ); } // down for positive

    for (i32 i = 0; i < 10; i++) { test(-1.5 + i * ulp32 + half_ulp32, -1.5f + (i         ) * ulp32, RM_RMM); } // down for negative
    for (i32 i = 0; i < 10; i++) { test( 1.5 + i * ulp32 + half_ulp32,  1.5f + (i + 1     ) * ulp32, RM_RMM); } // up for positive

    for (i32 i = 0; i < 10; i++) { test( 1.5 + i * ulp32 + half_ulp32,  1.5f + (i | 1     ) * ulp32, RM_RNO); } // nearest odd up
    for (i32 i = 0; i < 10; i++) { test(-1.5 + i * ulp32 + half_ulp32, -1.5f + (i | 1     ) * ulp32, RM_RNO); } // nearest odd up

    for (i32 i = 0; i < 10; i++) { test( 1.5 + i * ulp32 + half_ulp32,  1.5f + (i | 1     ) * ulp32, RM_RTO); } // nearest odd up
    for (i32 i = 0; i < 10; i++) { test(-1.5 + i * ulp32 + half_ulp32, -1.5f + (i | 1     ) * ulp32, RM_RTO); } // nearest odd up

    static const struct {
        double d;
        float f;
        RoundingModeEnum rm;
    } tests[] = {
        { 1.5         + half_ulp32, 1.5f + 1*ulp32, RM_RTO }, // 0.5ulp => 1ulp (ok, odd)
        { 1.5         + half_ulp32, 1.5f + 0*ulp32, RM_RNE }, // 0.5ulp => 0ulp (ok, even)

        { 1.5 + ulp32 + half_ulp32, 1.5f + 1*ulp32, RM_RTO }, // 1.5ulp => 1ulp (ok, odd)
        { 1.5 + ulp32 + half_ulp32, 1.5f + 2*ulp32, RM_RNE }, // 1.5ulp => 2ulp (ok, even)
    };

    // FIXME
    for (const auto& test : tests) {
        SoftFPU test_fpu;
        test_fpu.set_rounding_mode(test.rm);

        const f64 value = f64::soft(test.d);
        const f32 result = f32::soft(test.f);
        const f32 t = test_fpu.cvt_f_f<f32, f64>(value, test.rm);

        std::cout << test.rm << ": " << ValuePrinter(value) << ' ' << ValuePrinter(result) << ' ' << ValuePrinter(t) << fmt::endl;

        BOOST_TEST(result.val() == t.val());
    }
}

} // namespace fpu
} // namespace postrisc
