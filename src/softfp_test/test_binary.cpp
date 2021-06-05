#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "HardFPU.hpp"
#include "Sequencer.hpp"

namespace postrisc {
namespace fpu {

class add { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b, RoundingModeEnum rm) { return fpu.add(a, b, rm); }};
class sub { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b, RoundingModeEnum rm) { return fpu.sub(a, b, rm); }};
class mul { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b, RoundingModeEnum rm) { return fpu.mul(a, b, rm); }};
class div { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b, RoundingModeEnum rm) { return fpu.div(a, b, rm); }};

template<typename RESULT, typename T, typename OP, typename TRUE_FPU, typename TEST_FPU>
u32 test_binary(u64 count, TRUE_FPU& true_fpu, TEST_FPU& test_fpu, T a, T b, RoundingModeEnum rm, const char *name)
{
    true_fpu.clear_flags();
    RESULT trueZ = OP::template func<RESULT,T,TRUE_FPU>(true_fpu, a, b, rm);
    fp_flags trueFlags = true_fpu.get_flags();

    test_fpu.clear_flags();
    RESULT testZ = OP::template func<RESULT,T,TEST_FPU>(test_fpu, a, b, rm);
    fp_flags testFlags = test_fpu.get_flags();

    // gcc runtime (add|sub|mul|div) is ugly about NAN propagation rules
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
                  << "true: " << trueFlags << ' ' << ValuePrinter(trueZ) << fmt::endl
                  << "test: " <<  testFlags << ' ' << ValuePrinter(testZ) << fmt::endl
                  << fmt::endl;
        true_fpu.set_rounding_mode(rm);
        return 1;
    }
    return 0;
}

template<typename T, class OP>
void test_handler(RoundingModeEnum rm, const char *name)
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
        for (const T& b : specials) {
            errorCount += test_binary<T, T, OP,  HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, b, rm, name);
            count++;
        }
    }

    // check determined sequencer
    for ( ; count < Controller::s_maxCount; count++) {
        T a{}, b{};
        ctrl.next(a, b);

        errorCount += test_binary<T, T, OP, HardFPU, SoftFPU>(count, true_fpu, test_fpu, a, b, rm, name);

        if (Controller::isEnoughErrors(errorCount))
            break;
        Controller::checkEarlyExit(count, errorCount);
    }

    Controller::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

#define TEST_BINARY(T, OP) \
    BOOST_AUTO_TEST_CASE(T ## _ ## OP ## _rne) { test_handler<T,OP>(RM_RNE, #T "_" #OP "_rne"); } \
    BOOST_AUTO_TEST_CASE(T ## _ ## OP ## _rtz) { test_handler<T,OP>(RM_RTZ, #T "_" #OP "_rtz"); } \
    BOOST_AUTO_TEST_CASE(T ## _ ## OP ## _rup) { test_handler<T,OP>(RM_RUP, #T "_" #OP "_rup"); } \
    BOOST_AUTO_TEST_CASE(T ## _ ## OP ## _rdn) { test_handler<T,OP>(RM_RDN, #T "_" #OP "_rdn"); } \
    BOOST_AUTO_TEST_CASE(T ## _ ## OP ## _rmm) { test_handler<T,OP>(RM_RMM, #T "_" #OP "_rmm"); } \
    BOOST_AUTO_TEST_CASE(T ## _ ## OP ## _rno) { test_handler<T,OP>(RM_RNO, #T "_" #OP "_rno"); } \
    BOOST_AUTO_TEST_CASE(T ## _ ## OP ## _rto) { test_handler<T,OP>(RM_RTO, #T "_" #OP "_rto"); } \

TEST_BINARY(f16, add)
TEST_BINARY(f16, sub)
TEST_BINARY(f16, mul)
TEST_BINARY(f16, div)

TEST_BINARY(f32, add)
TEST_BINARY(f32, sub)
TEST_BINARY(f32, mul)
TEST_BINARY(f32, div)

TEST_BINARY(f64, add)
TEST_BINARY(f64, sub)
TEST_BINARY(f64, mul)
TEST_BINARY(f64, div)

TEST_BINARY(f128, add)
TEST_BINARY(f128, sub)
TEST_BINARY(f128, mul)
TEST_BINARY(f128, div)

BOOST_AUTO_TEST_CASE(f64_add_example)
{
    static const RoundingModeEnum hrms[] = {
        RM_RNE, RM_RTZ, RM_RDN, RM_RUP
    };

    static const RoundingModeEnum rms[] = {
        RM_RNE, RM_RTZ, RM_RDN, RM_RUP, RM_RMM, RM_RNO, RM_RTO
    };

    // add 1.0..2.0 values and 1.5ulp
    static const struct {
       f64 a;
       f64 b;
    } tests[] = {
        { f64{0x3fffffffffffffff}, f64{0x3cb8000000000000} },
        { f64{0x3fffffffffffffff}, f64{0x3cb8000000000000} },
        { f64{0x3ff0000fffffffff}, f64{0x3cb8000000000000} },
        { f64{0x3ff0000000000fff}, f64{0x3cb8000000000000} },
        { f64{0x3ff0000000000000}, f64{0x3cb8000000000000} },
    };

    for (const auto& test : tests) {

        std::cout << "f64_add_example: " << fmt::endl
                  << "1: " << ValuePrinter(test.a) << fmt::endl
                  << "2: " << ValuePrinter(test.b) << fmt::endl;

        for (const RoundingModeEnum rm : hrms) {
            HardFPU true_fpu;
            true_fpu.set_rounding_mode(rm);

            true_fpu.clear_flags();
            const f64 trueZ = true_fpu.add(test.a, test.b, rm);
            const fp_flags trueFlags = true_fpu.get_flags();

            true_fpu.set_rounding_mode(RM_RNE);
            std::cout << rm << "(h): " << trueFlags << ' ' << ValuePrinter(trueZ) << fmt::endl;
        }

        for (const RoundingModeEnum rm : rms) {
            SoftFPU test_fpu;
            test_fpu.set_rounding_mode(rm);

            test_fpu.clear_flags();
            const f64 testZ = test_fpu.add(test.a, test.b, rm);
            const fp_flags testFlags = test_fpu.get_flags();

            std::cout << rm << "(s): " << testFlags << ' ' << ValuePrinter(testZ) << fmt::endl;
        }
    }

}

} // namespace fpu
} // namespace postrisc
