#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "Controller.hpp"
#include "HardFPU.hpp"
#include "Sequencer.hpp"

namespace postrisc {
namespace fpu {

// same implementation. but cvt_f_u is checked in test_1 to use non-negative floats
class cvt_f_i { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.template cvt_f_i<DST, SRC>(a, rm); }};
class cvt_f_u { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.template cvt_f_i<DST, SRC>(a, rm); }};

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
static u32 test_unary(u64 count, TRUE_FPU& true_fpu, TEST_FPU& test_fpu, T a, RoundingModeEnum rm, const char *name)
{
    true_fpu.clear_flags();
    RESULT trueZ = OP::template func<RESULT,T,HardFPU>(true_fpu, a, rm);
    fp_flags trueFlags = true_fpu.get_flags();

    test_fpu.clear_flags();
    RESULT testZ = OP::template func<RESULT,T,SoftFPU>(test_fpu, a, rm);
    fp_flags testFlags = test_fpu.get_flags();

    // both true and test emit INVALID: real INVALID case
    if ((trueFlags & testFlags & FFLAG_INVALID) == FFLAG_INVALID) {
        // FIXME: lib soft floating point implementation emit different int NANs that hardware
        // hardware: 0x80000000 is int NAN for signed int, 0xFFFFFFFF is NAN for unsigned int (no matter what float value is)
        // lib: 0x00000000 for negative float and unsigned int, 0x7FFFFFFF for overflow float and signed int
        if (Controller::s_checkIntNaNs) {
            if  ( std::is_same<f128, T>::value ||                                           // from f128
                  (std::is_same<f16, T>::value && !HardFPU::avx_fp16_supports()) ||         // from f16 without avx512-fp16 hardware
                  (std::is_same<i128, RESULT>::value || std::is_same<u128, RESULT>::value)  // to i128/u128
                ) {
                // convert to signed int
                if constexpr (std::is_same<cvt_f_i, OP>::value) {
                    const RESULT testNAN = static_cast<RESULT>(static_cast<RESULT>(1) << (8 * sizeof(RESULT) - 1)); // like 0x80000000 for i32
                    const RESULT trueNAN = static_cast<RESULT>(~testNAN);                                           // like 0x7FFFFFFF for i32
                    if ( trueZ == trueNAN && testZ == testNAN) {
                        trueZ = testZ;
                    }
                }
                // convert to unsigned int
                if constexpr (std::is_same<cvt_f_u, OP>::value) {
                    const RESULT testNAN = static_cast<RESULT>(~static_cast<RESULT>(0)); // like 0xFFFFFFFF for u32
                    const RESULT trueNAN = static_cast<RESULT>(0);                       // like 0x00000000 for u32
                    if (trueZ == trueNAN && testZ == testNAN) {
                        trueZ = testZ;
                    }
                }

                // FIXME: builtin softlib f128|f16 set INVALID, but also set INEXACT(?)
                if ((trueFlags & FFLAG_INEXACT) == FFLAG_INEXACT &&
                    (testFlags & FFLAG_INEXACT) != FFLAG_INEXACT)
                {
                    // remove INEXACT, ignore difference
                    trueFlags = trueFlags & ~FFLAG_INEXACT;
                }
            }
        } else { // don't check int NANs
            if (trueZ != testZ) {
                testZ = trueZ;
            }
        }
    }

    if (!is_ok(trueZ, trueFlags, testZ, testFlags)) {
        true_fpu.set_rounding_mode(RM_RNE);
        std::cout << name << " error n=" << count << fmt::endl
                  << "1: " << ValuePrinter<T>(a) << fmt::endl
                  << "true: " << trueFlags << ' ' << ValuePrinter<RESULT>(trueZ) << fmt::endl
                  << "test: " << testFlags << ' ' << ValuePrinter<RESULT>(testZ) << fmt::endl
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
        if constexpr (std::is_same<cvt_f_u, OP>::value) {
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

#define   DEFINE_CVT_F_I_RM(DST, SRC, OP) \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rne) { test_handler<DST,SRC,OP>(RM_RNE, #SRC "_to_" #DST "_rne"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rtz) { test_handler<DST,SRC,OP>(RM_RTZ, #SRC "_to_" #DST "_rtz"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rup) { test_handler<DST,SRC,OP>(RM_RUP, #SRC "_to_" #DST "_rup"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rdn) { test_handler<DST,SRC,OP>(RM_RDN, #SRC "_to_" #DST "_rdn"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rmm) { test_handler<DST,SRC,OP>(RM_RMM, #SRC "_to_" #DST "_rmm"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rno) { test_handler<DST,SRC,OP>(RM_RNO, #SRC "_to_" #DST "_rno"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rto) { test_handler<DST,SRC,OP>(RM_RTO, #SRC "_to_" #DST "_rto"); } \

DEFINE_CVT_F_I_RM(i16,   f16,   cvt_f_i)
DEFINE_CVT_F_I_RM(u16,   f16,   cvt_f_u)
DEFINE_CVT_F_I_RM(i32,   f16,   cvt_f_i)
DEFINE_CVT_F_I_RM(i64,   f16,   cvt_f_i)
DEFINE_CVT_F_I_RM(i128,  f16,   cvt_f_i)
DEFINE_CVT_F_I_RM(u32,   f16,   cvt_f_u)
DEFINE_CVT_F_I_RM(u64,   f16,   cvt_f_u)
DEFINE_CVT_F_I_RM(u128,  f16,   cvt_f_u)
DEFINE_CVT_F_I_RM(i32,   f32,   cvt_f_i)
DEFINE_CVT_F_I_RM(i64,   f32,   cvt_f_i)
DEFINE_CVT_F_I_RM(i128,  f32,   cvt_f_i)
DEFINE_CVT_F_I_RM(u32,   f32,   cvt_f_u)
DEFINE_CVT_F_I_RM(u64,   f32,   cvt_f_u)
DEFINE_CVT_F_I_RM(u128,  f32,   cvt_f_u)
DEFINE_CVT_F_I_RM(i32,   f64,   cvt_f_i)
DEFINE_CVT_F_I_RM(i64,   f64,   cvt_f_i)
DEFINE_CVT_F_I_RM(i128,  f64,   cvt_f_i)
DEFINE_CVT_F_I_RM(u32,   f64,   cvt_f_u)
DEFINE_CVT_F_I_RM(u64,   f64,   cvt_f_u)
DEFINE_CVT_F_I_RM(u128,  f64,   cvt_f_u)
DEFINE_CVT_F_I_RM(i32,   f128,  cvt_f_i)
DEFINE_CVT_F_I_RM(i64,   f128,  cvt_f_i)
DEFINE_CVT_F_I_RM(i128,  f128,  cvt_f_i)
DEFINE_CVT_F_I_RM(u32,   f128,  cvt_f_u)
DEFINE_CVT_F_I_RM(u64,   f128,  cvt_f_u)
DEFINE_CVT_F_I_RM(u128,  f128,  cvt_f_u)

BOOST_AUTO_TEST_CASE(test_f_to_i)
{
    SoftFPU test_fpu;
    test_fpu.set_rounding_mode(RM_RNO);

    const int n_rm = 7;

    static const RoundingModeEnum rms[n_rm] = {
        RM_RNE, RM_RTZ, RM_RDN, RM_RUP, RM_RMM, RM_RNO, RM_RTO
    };

    static const struct {
        double f;
        i32 result[n_rm];
    } tests[] = {
    //        RNE RTZ RDN RUP RMM RNO RTO
    { -3.2, { -3, -3, -4, -3, -3, -3, -3 } },
    { -3.5, { -4, -3, -4, -3, -4, -3, -3 } },
    { -3.6, { -4, -3, -4, -3, -4, -4, -3 } },

    { -2.4, { -2, -2, -3, -2, -2, -2, -3 } },
    { -2.5, { -2, -2, -3, -2, -3, -3, -3 } },
    { -2.8, { -3, -2, -3, -2, -3, -3, -3 } },

    { -1.3, { -1, -1, -2, -1, -1, -1, -1 } },
    { -1.5, { -2, -1, -2, -1, -2, -1, -1 } },
    { -1.7, { -2, -1, -2, -1, -2, -2, -1 } },

    {  0.3, {  0,  0,  0,  1,  0,  0,  1 } },
    {  0.5, {  0,  0,  0,  1,  1,  1,  1 } },
    {  0.7, {  1,  0,  0,  1,  1,  1,  1 } },

    {  1.3, {  1,  1,  1,  2,  1,  1,  1 } },
    {  1.5, {  2,  1,  1,  2,  2,  1,  1 } },
    {  1.7, {  2,  1,  1,  2,  2,  2,  1 } },

    {  2.4, {  2,  2,  2,  3,  2,  2,  3 } },
    {  2.5, {  2,  2,  2,  3,  3,  3,  3 } },
    {  2.8, {  3,  2,  2,  3,  3,  3,  3 } },

    {  3.2, {  3,  3,  3,  4,  3,  3,  3 } },
    {  3.5, {  4,  3,  3,  4,  4,  3,  3 } },
    {  3.6, {  4,  3,  3,  4,  4,  4,  3 } },
    };

    for (size_t i = 0; i < n_rm; i++) {
        for (const auto& test : tests) {
            const f32 value = f32::soft(static_cast<native::f32>(test.f));
            const i32 result = test_fpu.cvt_f_i<i32, f32>(value, rms[i]);
            std::cout << rms[i] << " result: " << ValuePrinter(result) << " expected: " << ValuePrinter(test.result[i]) << " fp: " << ValuePrinter(value) << fmt::endl;
            BOOST_TEST(result == test.result[i]);
        }

        for (const auto& test : tests) {
            const f64 value = f64::soft(test.f);
            const i32 result = test_fpu.cvt_f_i<i32, f64>(value, rms[i]);
            std::cout << rms[i] << " result: " << ValuePrinter(result) << " expected: " << ValuePrinter(test.result[i]) << " fp: " << ValuePrinter(value) << fmt::endl;
            BOOST_TEST(result == test.result[i]);
        }

        for (const auto& test : tests) {
            const f128 value = f128::soft(static_cast<native::f128>(test.f));
            const i32 result = test_fpu.cvt_f_i<i32, f128>(value, rms[i]);
            std::cout << rms[i] << " result: " << ValuePrinter(result) << " expected: " << ValuePrinter(test.result[i]) << " fp: " << ValuePrinter(value) << fmt::endl;
            BOOST_TEST(result == test.result[i]);
        }
    }
}

} // namespace fpu
} // namespace postrisc
