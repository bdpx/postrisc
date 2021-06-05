#include "util/common.hpp"
#include "util/quadruple.hpp"
#include "HardFPU.hpp"
#include "Sequencer.hpp"

namespace postrisc {
namespace fpu {

class ordered      { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered     (a, b); }};
class ordered_eq   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_eq  (a, b); }};
class ordered_ne   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_ne  (a, b); }};
class ordered_lt   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_lt  (a, b); }};
class ordered_ge   { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_ordered_ge  (a, b); }};
class unordered    { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered   (a, b); }};
class unordered_eq { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_eq(a, b); }};
class unordered_ne { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_ne(a, b); }};
class unordered_lt { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_lt(a, b); }};
class unordered_ge { public: template<typename DST, typename SRC, class FPU> static DST func(FPU& fpu, SRC a, SRC b) { return fpu.compare_unordered_ge(a, b); }};

template<typename T, class OP>
static void test_compare(char const *name)
{
    Sequencer ctrl;
    Controller::writeTestsTotal(name);

    HardFPU true_fpu;
    SoftFPU test_fpu;

    u32 errorCount = 0;
    u64 count = 0;

    for ( ; count < Controller::s_maxCount; count++) {
        T a{}, b{};
        ctrl.next(a, b);

        true_fpu.clear_flags();
        const bool trueZ = OP::template func<bool,T,HardFPU>(true_fpu, a, b);
        const fp_flags trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        const bool testZ = OP::template func<bool,T,SoftFPU>(test_fpu, a, b);
        const fp_flags testFlags = test_fpu.get_flags();

        if (!is_ok(trueZ, trueFlags, testZ, testFlags)) {
            std::cout << name << " error n=" << count << fmt::endl
                      << "1: " << ValuePrinter(a) << fmt::endl
                      << "2: " << ValuePrinter(b) << fmt::endl
                      << "true: " << trueFlags << ' ' << ValuePrinter(trueZ) << fmt::endl
                      << "test: " << testFlags << ' ' << ValuePrinter(testZ) << fmt::endl
                      << fmt::endl;
            ++errorCount;
            if (Controller::isEnoughErrors(errorCount)) break;
        }
        Controller::checkEarlyExit(count, errorCount);
    }

    Controller::writeTestsPerformed(count, errorCount);
    BOOST_TEST(errorCount == 0);
}

#define TEST_COMPARE(SRC, OP) BOOST_AUTO_TEST_CASE(SRC ## _ ## OP) { test_compare<SRC,OP>(#SRC "_" #OP); }

TEST_COMPARE(f16,   ordered)
TEST_COMPARE(f16,   ordered_eq)
TEST_COMPARE(f16,   ordered_ne)
TEST_COMPARE(f16,   ordered_lt)
TEST_COMPARE(f16,   ordered_ge)
TEST_COMPARE(f16,   unordered)
TEST_COMPARE(f16,   unordered_eq)
TEST_COMPARE(f16,   unordered_ne)
TEST_COMPARE(f16,   unordered_lt)
TEST_COMPARE(f16,   unordered_ge)

TEST_COMPARE(f32,   ordered)
TEST_COMPARE(f32,   ordered_eq)
TEST_COMPARE(f32,   ordered_ne)
TEST_COMPARE(f32,   ordered_lt)
TEST_COMPARE(f32,   ordered_ge)
TEST_COMPARE(f32,   unordered)
TEST_COMPARE(f32,   unordered_eq)
TEST_COMPARE(f32,   unordered_ne)
TEST_COMPARE(f32,   unordered_lt)
TEST_COMPARE(f32,   unordered_ge)

TEST_COMPARE(f64,   ordered)
TEST_COMPARE(f64,   ordered_eq)
TEST_COMPARE(f64,   ordered_ne)
TEST_COMPARE(f64,   ordered_lt)
TEST_COMPARE(f64,   ordered_ge)
TEST_COMPARE(f64,   unordered)
TEST_COMPARE(f64,   unordered_eq)
TEST_COMPARE(f64,   unordered_ne)
TEST_COMPARE(f64,   unordered_lt)
TEST_COMPARE(f64,   unordered_ge)

TEST_COMPARE(f128,  ordered)
TEST_COMPARE(f128,  ordered_eq)
TEST_COMPARE(f128,  ordered_ne)
TEST_COMPARE(f128,  ordered_lt)
TEST_COMPARE(f128,  ordered_ge)
TEST_COMPARE(f128,  unordered)
TEST_COMPARE(f128,  unordered_eq)
TEST_COMPARE(f128,  unordered_ne)
TEST_COMPARE(f128,  unordered_lt)
TEST_COMPARE(f128,  unordered_ge)

} // namespace fpu
} // namespace postrisc
