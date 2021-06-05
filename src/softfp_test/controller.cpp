#include <boost/test/unit_test.hpp>

#include <cstdlib>    // for exit
#include <csignal>
#include <stdexcept>  // for runtime_error
#include <unistd.h>   // for isatty
#include <cfenv>      // for fegetexceptflag
//#include <mpfr.h>     // for mpfr_fma

#include "util/common.hpp"
#include "controller.hpp"
#include "hardfloat.hpp"

namespace postrisc {
namespace fpu {

/******************************************************************************
* global test fixture
******************************************************************************/
TestController::TestController()
{
    BOOST_TEST_MESSAGE("TestController ctor");
}

void TestController::setup()
{
    BOOST_TEST_MESSAGE("TestController setup");
    std::signal(SIGINT, catchSIGINT);
    std::signal(SIGTERM, catchSIGINT);
    ParseCommandLine(
        boost::unit_test::framework::master_test_suite().argc,
        boost::unit_test::framework::master_test_suite().argv);

    std::cout << "errorStop=" << errorStop
              << " checkNaNs=" << checkNaNs
              << " checkFlags=" << checkFlags
              << " forever=" << forever
              << " maxErrorCount=" << maxErrorCount << fmt::endl;
}

void TestController::teardown()
{
    BOOST_TEST_MESSAGE("TestController teardown");
}

TestController::~TestController()
{
    BOOST_TEST_MESSAGE("TestController dtor");
}

BOOST_TEST_GLOBAL_FIXTURE(TestController);

int TestController::stop = 0;
bool TestController::checkNaNs = false;
bool TestController::checkFlags = false;
bool TestController::forever = false;
bool TestController::errorStop = false;
bool TestController::is_cout_tty = false;
u32 TestController::maxErrorCount = 5;

void TestController::catchSIGINT(int UNUSED(signalCode))
{
    if (stop > 2) exit(EXIT_FAILURE);
    ++stop;
}

bool
TestController::GetHardwareTininessMode(void)
{
    volatile float a = 0x1.fffp-126;
    volatile float b = 0x1.0008p-1;
    volatile float c;
    volatile float m = std::numeric_limits<float>::min();
    volatile float mm;

    std::feclearexcept(FE_ALL_EXCEPT);
    mm = m * m;
    UNREFERENCED_PARAMETER(mm);

    if (!std::fetestexcept(FE_UNDERFLOW)) {
        std::cout << "underflow exceptions not supported at runtime, can't take hardware tininess mode\n";
        return true; // after
    }

    std::feclearexcept(FE_ALL_EXCEPT);
    c = a * b;
    UNREFERENCED_PARAMETER(c);

    if (std::fetestexcept(FE_UNDERFLOW)) {
       std::cout << "hardware detect tininess before rounding (32-bit ARM?)\n";
       return false;
    } else {
       std::cout << "hardware detect tininess after rounding (normal)\n";
       return true;
    }
}

template<typename FPU>
int TestController::CheckHardwareTininessMode(FPU& fpu)
{
    fpu.clear_flags(FFLAG_ALL_EXCEPT);
    volatile float m = std::numeric_limits<float>::min();
    volatile f32 mm = fpu.mul(f32::soft(m), f32::soft(m));
    UNREFERENCED_PARAMETER(mm);

    if ((fpu.get_flags() & FFLAG_UNDERFLOW) != FFLAG_UNDERFLOW) {
        std::cout << "underflow exceptions not supported at runtime, can't take hardware tininess mode\n";
        return -1; // after
    }

    fpu.clear_flags(FFLAG_ALL_EXCEPT);
    volatile float a = 0x1.fffp-126;
    volatile float b = 0x1.0008p-1;
    volatile f32 c = fpu.mul(f32::soft(a), f32::soft(b));
    UNREFERENCED_PARAMETER(c);

    if ((fpu.get_flags() & FFLAG_UNDERFLOW) == FFLAG_UNDERFLOW) {
       std::cout << "hardware detect tininess before rounding (32-bit ARM?)\n";
       return 0;
    } else {
       std::cout << "hardware detect tininess after rounding (normal)\n";
       return 1;
    }
}

BOOST_AUTO_TEST_CASE(test_tininess)
{
    BOOST_TEST(TestController::GetHardwareTininessMode());
    SoftFPU soft_fpu;
    HardFPU hard_fpu;
    const int soft_tininess = TestController::CheckHardwareTininessMode(soft_fpu);
    const int hard_tininess = TestController::CheckHardwareTininessMode(hard_fpu);
    BOOST_TEST(soft_tininess >= 0);
    BOOST_TEST(hard_tininess >= 0);
    BOOST_TEST(soft_tininess == hard_tininess);
}

void
TestController::writeTestsTotal(u64 total)
{
    if (forever) {
        std::cout << "Unbounded tests.\n";
    } else {
        std::cout << fmt::dec(total) << " tests total.\n";
    }
}

void
TestController::writeTestsPerformed(u64 count, u64 errorCount)
{
    if (is_cout_tty)
       std::cout << '\r';

    if (errorCount) {
        std::cout << fmt::dec(count) << " tests performed; " << fmt::dec(errorCount) << " errors found\n";
    } else {
        std::cout << fmt::dec(count) << " tests performed; No errors found\n";
    }
}

void
TestController::checkEarlyExit(u64& count, u64 errorCount)
{
    const unsigned TEST_INTERVAL = 50000;

    ++count;
    if (count % TEST_INTERVAL != 0) {
        return;
    }

    if (stop) {
        writeTestsPerformed(count, errorCount);
        abort_testing();
    }

    if (is_cout_tty) {
       std::cout << '\r' << fmt::dec(count) << std::flush;
    }
}

void
TestController::abort_testing(void)
{
    throw std::runtime_error("test aborted");
}

static inline bool streq(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

int
TestController::ParseCommandLine(int argc, char **argv)
{
    --argc;
    ++argv;

    if (argc <= 0)
       return EXIT_SUCCESS;

    for (; argc; --argc,  ++argv) {
        const char *arg = argv[0];
        if (!arg) {
            break;
        }

        if (arg[0] == '-') ++arg;

        if (streq(arg, "errors")) {
            if (argc < 2) {
                LOG_ABORT << argv[0] << " option requires numeric argument";
            }
            maxErrorCount = atoi(argv[1]);
            --argc;
            ++argv;
        }
        else if (streq(arg, "errorstop")) { errorStop = true; }
        else if (streq(arg, "forever")) { forever = true; }
        else if (streq(arg, "checknans")) { checkNaNs = true; }
        else if (streq(arg, "nochecknans")) { checkNaNs = false; }
        else if (streq(arg, "nocheckflags")) { checkFlags = false; }
        else if (streq(arg, "checkflags")) { checkFlags = true; }
    }

    return EXIT_SUCCESS;
}

} // namespace fpu
} // namespace postrisc
