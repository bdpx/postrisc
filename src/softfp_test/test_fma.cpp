#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "HardFPU.hpp"
#include "Sequencer.hpp"

namespace postrisc {
namespace fpu {

class muladd { public: template<typename DST, typename SRC, class FPU>
    static DST func(FPU& fpu, SRC a, SRC b, SRC c, RoundingModeEnum rm) {
        return fpu.muladd(a, b, c, rm, fpu::FLOAT_MULADD_NEGATE_NONE);
    }
};

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
static u32
test_fma(u64 count, TRUE_FPU& true_fpu, TEST_FPU& test_fpu, T a, T b, T c, RoundingModeEnum rm, const char *name)
{
    true_fpu.clear_flags();
    RESULT trueZ = OP::template func<RESULT,T,TRUE_FPU>(true_fpu, a, b, c, rm);
    fp_flags trueFlags = true_fpu.get_flags();

    test_fpu.clear_flags();
    RESULT testZ = OP::template func<RESULT,T,TEST_FPU>(test_fpu, a, b, c, rm);
    fp_flags testFlags = test_fpu.get_flags();

    // ZERO * INF + NAN
    // 1) may or may not raise INVALID flag (implementation-defined): unify to raise
    // 2) may emit cNAN, or emit new NAN: unify
    if constexpr (std::is_same<f128, T>::value || std::is_same<f16, T>::value) {
        if (Controller::s_checkFlags && is_nan(c) && ((is_inf(a) && is_zero(b)) || (is_inf(b) && is_zero(a))) && is_nan(trueZ) && is_nan(testZ)) {
            trueFlags = trueFlags | FFLAG_INVALID;
            testFlags = testFlags | FFLAG_INVALID;
            testZ = trueZ;
        }
    }

    // libquadmath fmaq is ugly about NAN propagation rules
    if constexpr (std::is_same<f128, T>::value) {
        if (Controller::s_checkNaNs && is_nan(trueZ) && is_nan(testZ)) {
            testZ = trueZ;
        }
    }

    if (!is_ok(trueZ, trueFlags, testZ, testFlags)) {
        true_fpu.set_rounding_mode(RM_RNE);
        std::cout << name << " error n=" << count << fmt::endl
                  << "1: " << ValuePrinter(a) << fmt::endl
                  << "2: " << ValuePrinter(b) << fmt::endl
                  << "3: " << ValuePrinter(c) << fmt::endl
                  << "true: " << trueFlags << ' ' << ValuePrinter(trueZ) << fmt::endl
                  << "test: " <<  testFlags << ' ' << ValuePrinter(testZ) << fmt::endl
                  << fmt::endl;
        true_fpu.set_rounding_mode(rm);
        return 1;
    }
    return 0;
}

template<typename RESULT, typename T,  typename OP>
static void test_handler(RoundingModeEnum rm, char const *name)
{
    Sequencer seq;
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
        for (const T& b : specials) {
            for (const T& c : specials) {
                errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, b, c, rm, name);
                count++;
            }
        }
    }

    // check determined sequencer
    for ( ; count < Controller::s_maxCount; count++) {
        T a{}, b{}, c{};
        seq.next(a, b, c);

        errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, b, c, rm, name);

        if (Controller::isEnoughErrors(errorCount))
            break;
        Controller::checkEarlyExit(count, errorCount);
    }

    // continue with random floats
    for ( ; count < Controller::s_maxCount; count++) {
        const T a = rrandom_sf<T>();
        const T b = rrandom_sf<T>();
        const T c = rrandom_sf<T>();

        errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, b, c, rm, name);
        errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, c, b, rm, name);
        errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, c, a, b, rm, name);
        errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, c, b, a, rm, name);
        errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, b, a, c, rm, name);
        errorCount += test_fma<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, b, c, a, rm, name);

        if (Controller::isEnoughErrors(errorCount))
            break;
        Controller::checkEarlyExit(count, errorCount);
    }


    Controller::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

#define DEFINE_TEST_FMA(T, OP) \
    BOOST_AUTO_TEST_CASE(T ## _fma_rne) { test_handler<T,T,OP>(RM_RNE, #T "_fma_rne"); } \
    BOOST_AUTO_TEST_CASE(T ## _fma_rtz) { test_handler<T,T,OP>(RM_RTZ, #T "_fma_rtz"); } \
    BOOST_AUTO_TEST_CASE(T ## _fma_rup) { test_handler<T,T,OP>(RM_RUP, #T "_fma_rup"); } \
    BOOST_AUTO_TEST_CASE(T ## _fma_rdn) { test_handler<T,T,OP>(RM_RDN, #T "_fma_rdn"); } \
    BOOST_AUTO_TEST_CASE(T ## _fma_rmm) { test_handler<T,T,OP>(RM_RMM, #T "_fma_rmm"); } \
    BOOST_AUTO_TEST_CASE(T ## _fma_rno) { test_handler<T,T,OP>(RM_RNO, #T "_fma_rno"); } \
    BOOST_AUTO_TEST_CASE(T ## _fma_rto) { test_handler<T,T,OP>(RM_RTO, #T "_fma_rto"); } \

  DEFINE_TEST_FMA(f16,  muladd)
  DEFINE_TEST_FMA(f32,  muladd)
  DEFINE_TEST_FMA(f64,  muladd)
  DEFINE_TEST_FMA(f128, muladd)

} // namespace fpu
} // namespace postrisc
