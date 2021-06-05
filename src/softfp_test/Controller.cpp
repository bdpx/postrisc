#include <boost/test/unit_test.hpp>

#include <cstdlib>    // for exit
#include <csignal>
#include <stdexcept>  // for runtime_error
#include <unistd.h>   // for isatty
#include <cfenv>      // for fegetexceptflag
//#include <mpfr.h>     // for mpfr_fma

#include "util/common.hpp"
#include "Controller.hpp"
#include "HardFPU.hpp"

namespace postrisc {
namespace fpu {

/******************************************************************************
* global test fixture
******************************************************************************/
Controller::Controller()
{
    BOOST_TEST_MESSAGE("Controller ctor");
}

void Controller::setup()
{
    BOOST_TEST_MESSAGE("Controller setup");
    std::signal(SIGINT, catchSIGINT);
    std::signal(SIGTERM, catchSIGINT);

    std::cout <<
        "Available testing options:\n"
        "   forever=0|1         - run forever one test\n"
        "   maxCount=num        - maximum iterations (-1: no limits)\n"
        "   maxErrorCount=num   - maximum errors (-1: no limits)\n"
        "   checknans=0|1       - check NaN values\n"
        "   checkintnans=0|1    - check integer NaN values\n"
        "   checkflags=0|1      - check FP flags\n";
;

    ParseCommandLine(
        boost::unit_test::framework::master_test_suite().argc,
        boost::unit_test::framework::master_test_suite().argv);

    std::cout << "Current config:"
              << " fp16=" << (HardFPU::avx_fp16_supports() ? "avx512-fp16" : "lib")
              << " checkNaNs=" << s_checkNaNs
              << " checkIntNaNs=" << s_checkIntNaNs
              << " checkFlags=" << s_checkFlags
              << " forever=" << s_forever
              << " maxCount=" << s_maxCount
              << " maxErrorCount=" << s_maxErrorCount
              << fmt::endl;

    s_isatty = isatty(STDOUT_FILENO);

    int seed = 1;
    std::srand(seed);
}

void Controller::teardown()
{
    BOOST_TEST_MESSAGE("Controller teardown");
}

Controller::~Controller()
{
    BOOST_TEST_MESSAGE("Controller dtor");
}

BOOST_TEST_GLOBAL_FIXTURE(Controller);

bool Controller::s_checkNaNs = false;
bool Controller::s_checkIntNaNs = false;
bool Controller::s_checkFlags = true;
bool Controller::s_forever = false;
bool Controller::s_isatty = false;

u64 Controller::s_maxCount = 100 * 1000;
u64 Controller::s_maxErrorCount = 50;
u32 Controller::s_reportInterval = 10 * 1000UL * 1000UL;
u32 Controller::s_stop = 0;

void Controller::catchSIGINT(int UNUSED(signalCode))
{
    if (s_stop > 2) exit(EXIT_FAILURE);
    ++s_stop;
}

bool
Controller::GetHardwareTininessMode(void)
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
int Controller::CheckHardwareTininessMode(FPU& fpu)
{
    fpu.clear_flags(FFLAG_ALL_EXCEPT);
    volatile float m = std::numeric_limits<float>::min();
    volatile f32 mm = fpu.mul(f32::soft(m), f32::soft(m), RM_RNE);
    UNREFERENCED_PARAMETER(mm);

    if ((fpu.get_flags() & FFLAG_UNDERFLOW) != FFLAG_UNDERFLOW) {
        std::cout << "underflow exceptions not supported at runtime, can't take hardware tininess mode\n";
        return -1; // after
    }

    fpu.clear_flags(FFLAG_ALL_EXCEPT);
    volatile float a = 0x1.fffp-126;
    volatile float b = 0x1.0008p-1;
    volatile f32 c = fpu.mul(f32::soft(a), f32::soft(b), RM_RNE);
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
    BOOST_TEST(Controller::GetHardwareTininessMode());
    SoftFPU soft_fpu;
    HardFPU hard_fpu;
    const int soft_tininess = Controller::CheckHardwareTininessMode(soft_fpu);
    const int hard_tininess = Controller::CheckHardwareTininessMode(hard_fpu);
    BOOST_TEST(soft_tininess >= 0);
    BOOST_TEST(hard_tininess >= 0);
    BOOST_TEST(soft_tininess == hard_tininess);
}

void Controller::writeTestsTotal(const char *name)
{
    std::cout << "Testing " << name << ' ';
    if (s_forever) {
        std::cout << "Unbounded tests." << std::endl;
    } else {
        std::cout << fmt::dec(s_maxCount) << " tests total." << std::endl;
    }
}

void Controller::writeTestsPerformed(u64 count, u64 errorCount)
{
    if (s_isatty)
       std::cout << '\r';

    if (errorCount) {
        std::cout << fmt::dec(count) << " tests performed; " << fmt::dec(errorCount) << " errors found" << std::endl;
    } else {
        std::cout << fmt::dec(count) << " tests performed; No errors found" << std::endl;
    }
}

void Controller::checkEarlyExit(u64 count, u64 errorCount)
{
    if (s_stop) {
        writeTestsPerformed(count, errorCount);
        // throw std::runtime_error("test aborted");
        exit(1);
    }

    if (s_isatty && ((count % s_reportInterval) == 0)) {
        std::cout << '\r' << fmt::dec(count) << std::flush;
    }
}

int Controller::ParseCommandLine(int argc, char **argv)
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

        while (arg[0] == '-') ++arg;

        std::string_view arg_view = arg;

        if (arg_view.starts_with("maxCount=")) {
            s_maxCount = strtoull(arg + sizeof("maxCount=") - 1, nullptr, 0);
        }
        else if (arg_view.starts_with("maxErrorCount=")) {
            s_maxErrorCount = strtoull(arg + sizeof("maxErrorCount=") - 1, nullptr, 0);
        }
        else if (arg_view.starts_with("forever=")) { s_forever = atoi(arg + sizeof("forever=") - 1); }
        else if (arg_view.starts_with("checknans=")) { s_checkNaNs = atoi(arg + sizeof("checknans=") - 1); }
        else if (arg_view.starts_with("checkintnans=")) { s_checkIntNaNs = atoi(arg + sizeof("checkintnans=") - 1); }
        else if (arg_view.starts_with("checkflags=")) { s_checkFlags = atoi(arg + sizeof("checkflags=") - 1); }
        else {
             std::cerr << "unknown command line parameter: " << arg << std::endl;
             exit(1);
        }
    }

    return EXIT_SUCCESS;
}

} // namespace fpu
} // namespace postrisc
