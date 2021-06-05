#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "HardFPU.hpp"
#include "Sequencer.hpp"

namespace postrisc {
namespace fpu {

class cvt_i_f { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.template cvt_i_f<DST, SRC>(a, rm); }};
class cvt_u_f { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, RoundingModeEnum rm) { return fpu.template cvt_i_f<DST, SRC>(a, rm); }};

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
static u32 test_unary(u64 count, TRUE_FPU& true_fpu, TEST_FPU& test_fpu, T a, RoundingModeEnum rm, const char *name)
{
    true_fpu.clear_flags();
    const RESULT trueZ = OP::template func<RESULT,T,TRUE_FPU>(true_fpu, a, rm);
    const fp_flags trueFlags = true_fpu.get_flags();

    test_fpu.clear_flags();
    const RESULT testZ = OP::template func<RESULT,T,TEST_FPU>(test_fpu, a, rm);
    const fp_flags testFlags = test_fpu.get_flags();

    bool ok = is_ok(trueZ, trueFlags, testZ, testFlags);

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
    const auto specials = special_ints<T>();

    for (const T& a : specials) {
        //std::cout << "n=" << count << fmt::endl;
        errorCount += test_unary<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, rm, name);
        count++;
    }

    for ( ; count < Controller::s_maxCount; count++) {
        T a{};
        ctrl.next(a);

        //std::cout << "n=" << count << fmt::endl;
        errorCount += test_unary<RESULT, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, rm, name);
        if (Controller::isEnoughErrors(errorCount))
            break;
        Controller::checkEarlyExit(count, errorCount);
    }

    Controller::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

#define DEFINE_CVT_I_F_RM(DST, SRC, OP) \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rne) { test_handler<DST, SRC, OP>(RM_RNE, #SRC "_to_" #DST "_rne"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rtz) { test_handler<DST, SRC, OP>(RM_RTZ, #SRC "_to_" #DST "_rtz"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rup) { test_handler<DST, SRC, OP>(RM_RUP, #SRC "_to_" #DST "_rup"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rdn) { test_handler<DST, SRC, OP>(RM_RDN, #SRC "_to_" #DST "_rdn"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rmm) { test_handler<DST, SRC, OP>(RM_RMM, #SRC "_to_" #DST "_rmm"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rno) { test_handler<DST, SRC, OP>(RM_RNO, #SRC "_to_" #DST "_rno"); } \
    BOOST_AUTO_TEST_CASE(SRC ## _to_ ## DST ## _rto) { test_handler<DST, SRC, OP>(RM_RTO, #SRC "_to_" #DST "_rto"); } \

DEFINE_CVT_I_F_RM(f16,   i32,   cvt_i_f)
DEFINE_CVT_I_F_RM(f16,   i64,   cvt_i_f)
DEFINE_CVT_I_F_RM(f16,   i128,  cvt_i_f)
DEFINE_CVT_I_F_RM(f16,   u32,   cvt_u_f)
DEFINE_CVT_I_F_RM(f16,   u64,   cvt_u_f)
DEFINE_CVT_I_F_RM(f16,   u128,  cvt_u_f)
DEFINE_CVT_I_F_RM(f32,   i32,   cvt_i_f)
DEFINE_CVT_I_F_RM(f32,   i64,   cvt_i_f)
DEFINE_CVT_I_F_RM(f32,   i128,  cvt_i_f)
DEFINE_CVT_I_F_RM(f32,   u32,   cvt_u_f)
DEFINE_CVT_I_F_RM(f32,   u64,   cvt_u_f)
DEFINE_CVT_I_F_RM(f32,   u128,  cvt_u_f)
DEFINE_CVT_I_F_RM(f64,   i32,   cvt_i_f)
DEFINE_CVT_I_F_RM(f64,   i64,   cvt_i_f)
DEFINE_CVT_I_F_RM(f64,   i128,  cvt_i_f)
DEFINE_CVT_I_F_RM(f64,   u32,   cvt_u_f)
DEFINE_CVT_I_F_RM(f64,   u64,   cvt_u_f)
DEFINE_CVT_I_F_RM(f64,   u128,  cvt_u_f)
DEFINE_CVT_I_F_RM(f128,  i32,   cvt_i_f)
DEFINE_CVT_I_F_RM(f128,  i64,   cvt_i_f)
DEFINE_CVT_I_F_RM(f128,  i128,  cvt_i_f)
DEFINE_CVT_I_F_RM(f128,  u32,   cvt_u_f)
DEFINE_CVT_I_F_RM(f128,  u64,   cvt_u_f)
DEFINE_CVT_I_F_RM(f128,  u128,  cvt_u_f)

} // namespace fpu
} // namespace postrisc
