#include <boost/test/unit_test.hpp>

#include <cstdlib>    // for exit
#include <csignal>
#include <stdexcept>  // for runtime_error
#include <unistd.h>   // for isatty
//#include <mpfr.h>     // for mpfr_fma

#include "util/common.hpp"
#include "float.hpp"
#include "floatx.hpp"
#include "controller.hpp"
#include "hardfloat.hpp"
#include "functions.hpp"

namespace postrisc {
namespace fpu {

static const struct {
    char  name[35];
    u8    numInputs                      : 2;
    u8    supported                      : 2;
    u8    roundingPrecision              : 1;
    u8    roundingMode                   : 1;
    u8    tininessMode                   : 1;
    u8    tininessModeAtReducedPrecision : 1;
    u16   types;
} functions[NUM_FUNCTIONS] = {
#define FPU_FUNCTION_STRUCT(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) \
    { \
        #T1 "_" #method , \
        args_##NumInputs, \
        SUPPORTED_##supported, \
        roundingPrecision, \
        roundingMode, \
        tininessMode, \
        tininessModeAtReducedPrecision, \
        type_ ## T1 | type_ ## T2 \
    },

    DECLARE_FUNCTIONS_TABLE(FPU_FUNCTION_STRUCT)
};

//////////////////////
//////////////////////

u8 TestController::stop = 0;

void
TestController::catchSIGINT(int UNUSED(signalCode))
{
    if ( stop > 2 ) exit( EXIT_FAILURE );
    ++stop;
}


enum { NOT_SET = -1 };

/*
f80 mpfr_test_fmal(f80 a, f80 b, f80 c)
{
    mpfr_set_default_prec (64);
    mpfr_set_emin (-16384 -64 +4);
    mpfr_set_emax (16384);

    mpfr_t xx, xy, xz, xr, xm;

    mpfr_init (xx); mpfr_set_ld (xx, a.hard(), GMP_RNDN);
    mpfr_init (xy); mpfr_set_ld (xy, b.hard(), GMP_RNDN);
    mpfr_init (xz); mpfr_set_ld (xz, c.hard(), GMP_RNDN);
    mpfr_init (xr); mpfr_init (xm);

    int i = mpfr_mul (xm, xx, xy, GMP_RNDN);
    mpfr_subnormalize (xm, i, GMP_RNDN);

    i = mpfr_fma (xr, xx, xy, xz, GMP_RNDN);
    mpfr_subnormalize (xr, i, GMP_RNDN);

//    mpfr_printf ("mpfr_mul       %Ra\n"
//                 "mpfr_fma       %Ra\n", xm, xr);
    extended e = mpfr_get_ld(xr, GMP_RNDN);
    mpfr_clear (xx); mpfr_clear (xy); mpfr_clear (xz);
    mpfr_clear (xm); mpfr_clear (xr);
    return f80( e );
}
*/

TestController::TestController(void)
{
    set_defaults();
}

TestController::~TestController(void)
{
}

tininess_mode_t
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
        return FLOAT_TININESS_AFTER_ROUNDING;
    }

    std::feclearexcept(FE_ALL_EXCEPT);
    c = a * b;
    UNREFERENCED_PARAMETER(c);

    if (std::fetestexcept(FE_UNDERFLOW)) {
       std::cout << "hardware detect tininess before rounding (32-bit ARM?)\n";
       return FLOAT_TININESS_BEFORE_ROUNDING;
    } else {
       std::cout << "hardware detect tininess after rounding (normal)\n";
       return FLOAT_TININESS_AFTER_ROUNDING;
    }
}

void
TestController::set_defaults(void)
{
    level         = 1;
    errorStop     = false;
    forever       = false;
    checkNaNs     = false;
    checkFlags    = true;
    anyErrors     = false;
    is_cout_tty   = isatty(STDOUT_FILENO);
    maxErrorCount = 25;
    errorCount    = 0;

    test_mode         = TEST_HARDWARE;
    operands          = 0;
    typesTested       = type_all;

    roundingPrecision = static_cast<rounding_precision_t>(NOT_SET);
    roundingMode      = static_cast<rounding_mode_t>(NOT_SET);
    tininessMode      = static_cast<tininess_mode_t>(NOT_SET);

    trueName.clear();
    testName.clear();
    functionName.clear();
    roundingPrecisionName.clear();
    roundingModeName.clear();
    tininessModeName.clear();
}

void
TestController::setLevel( int levelIn )
{
    if (    (levelIn != 1)
         && (levelIn != 2)
       ) {
       LOG_ABORT << "Invalid testing level: " << levelIn;
    }

    this->level = levelIn;
}

void
TestController::init_sequencer(void)
{
    sequenceA.init(1);
    sequenceB.init(2);
    sequenceC.init(3);
    subcase = 0;
    done = false;
}

void
TestController::writeTestsTotal( void )
{
    if ( forever ) {
        std::cout << "Unbounded tests.\n";
    } else {
        std::cout << std::dec << total << " tests total.\n";
    }
}

void
TestController::writeFunctionName(std::ostream& out)
{
    out << functionName;

    if ( !roundingModeName.empty() ) {
        if ( !roundingPrecisionName.empty() ) {
            out << " precision " << roundingPrecisionName;
        }
        out << " rounding " << roundingModeName;
        if ( !tininessModeName.empty() ) {
            out << " tininess " << tininessModeName << " rounding";
        }
    }

    out << "." << std::endl;
}

void
TestController::writeTestsPerformed(u64 count)
{
    if ( is_cout_tty )
       std::cout << '\r';

    if (errorCount) {
        std::cout << fmt::dec(count) << " tests performed; " << errorCount << " errors found in ";
        writeFunctionName(std::cout);
    } else {
        std::cout << fmt::dec(count) << " tests performed; No errors found in ";
        writeFunctionName(std::cout);
    }
}

void
TestController::checkEarlyExit(u64& count)
{
    const unsigned TEST_INTERVAL = 10000;

    ++count;
    if (count % TEST_INTERVAL != 0) {
        return;
    }

    if (stop) {
        writeTestsPerformed(count);
        abort_testing();
    }

    if (is_cout_tty) {
       std::cout << '\r' << fmt::dec(count) << std::flush;
    }
}

template<>
void
TestController::next<i32>(i32& a)
{
    switch(CASE(level, subcase)) {
        case CASE(1,0):
        case CASE(2,0):
            sequenceA.RandomP3(a);
            break;

        case CASE(1,1):
        case CASE(2,1):
            sequenceA.RandomPInf(a);
            break;

        case CASE(1,2):
            sequenceA.NextP1(a);
            done = sequenceA.done;
            subcase = -1;
            break;

        case CASE(2,2):
            sequenceA.NextP2(a);
            done = sequenceA.done;
            subcase = -1;
            break;
    }
    ++subcase;
}

template<>
void
TestController::next<i64>(i64& a)
{
    switch(CASE(level, subcase)) {
        case CASE(1,0):
        case CASE(2,0):
            sequenceA.RandomP3(a);
            break;

        case CASE(1,1):
        case CASE(2,1):
            sequenceA.RandomPInf(a);
            break;

        case CASE(1,2):
            sequenceA.NextP1(a);
            done = sequenceA.done;
            subcase = -1;
            break;

        case CASE(2,2):
            sequenceA.NextP2(a);
            done = sequenceA.done;
            subcase = -1;
            break;
    }
    ++subcase;
}

void
TestController::writeErrorFound(std::ostream& out)
{
    out << "\nErrors found in ";
    writeFunctionName(out);
    anyErrors = true;
}

void
TestController::setup_names(TestContext& context)
{
    functionName = functions[ context.functionCode ].name;

    switch (context.roundingPrecision) {
        case FLOAT_ROUND_PRECISION_32:  roundingPrecisionName = "32";  break;
        case FLOAT_ROUND_PRECISION_64:  roundingPrecisionName = "64";  break;
        case FLOAT_ROUND_PRECISION_80:  roundingPrecisionName = "80";  break;
        default:
            context.roundingPrecision = FLOAT_ROUND_PRECISION_80;
            roundingPrecisionName = "";
            break;
    }

    switch (context.roundingMode) {
        case FLOAT_ROUND_TONEAREST_EVEN:   roundingModeName = "nearest_even";   break;
        case FLOAT_ROUND_TOWARDZERO:       roundingModeName = "to_zero";        break;
        case FLOAT_ROUND_DOWNWARD:         roundingModeName = "down";           break;
        case FLOAT_ROUND_UPWARD:           roundingModeName = "up";             break;
        default:
            roundingModeName = "";
            context.roundingMode = FLOAT_ROUND_TONEAREST_EVEN;
            break;
    }

    switch (context.tininessMode) {
        case FLOAT_TININESS_BEFORE_ROUNDING:   tininessModeName = "before";    break;
        case FLOAT_TININESS_AFTER_ROUNDING:    tininessModeName = "after";     break;
        default:
            tininessModeName = "";
            context.tininessMode = FLOAT_TININESS_AFTER_ROUNDING;
            break;
    }
}

void
TestController::testingFunction(
    int                  functionCode,
    TestFunctionPtr      function,
    bool                 hardware)
{
    static const rounding_mode_t rounding_modes[] = {
        FLOAT_ROUND_TONEAREST_EVEN,
        FLOAT_ROUND_TOWARDZERO,
        FLOAT_ROUND_DOWNWARD,
        FLOAT_ROUND_UPWARD
    };

    static const rounding_precision_t rounding_precisions[] = {
        FLOAT_ROUND_PRECISION_32,
        FLOAT_ROUND_PRECISION_64,
        FLOAT_ROUND_PRECISION_80
    };

    static const tininess_mode_t tininess_modes[] = {
        FLOAT_TININESS_AFTER_ROUNDING,
        FLOAT_TININESS_BEFORE_ROUNDING
    };

    TestContext ctx;
    ctx.functionCode = functionCode;

    for (size_t rpi = 0; rpi < sizearray(rounding_precisions); ++rpi) {
        bool stopRoundingPrecisionLoop = false;
        rounding_precision_t currentRoundingPrecision = rounding_precisions[rpi];

        if (!functions[functionCode].roundingPrecision) {
            currentRoundingPrecision = static_cast<rounding_precision_t>(NOT_SET);
            stopRoundingPrecisionLoop = true;
        } else if (roundingPrecision != static_cast<rounding_precision_t>(NOT_SET)) {
            currentRoundingPrecision = roundingPrecision;
            stopRoundingPrecisionLoop = true;
        }

        for (size_t rmi = 0; rmi < sizearray(rounding_modes); ++rmi) {
            bool stopRoundingModeLoop = false;
            rounding_mode_t currentRoundingMode = rounding_modes[rmi];

            if (!functions[functionCode].roundingMode) {
                currentRoundingMode = static_cast<rounding_mode_t>(NOT_SET);
                stopRoundingModeLoop = true;
            } else if (roundingMode != static_cast<rounding_mode_t>(NOT_SET)) {
                currentRoundingMode = roundingMode;
                stopRoundingModeLoop = true;
            }

            for (size_t tmi = 0; tmi < sizearray(tininess_modes); ++tmi) {
                bool stopTininessModeLoop = false;
                tininess_mode_t currentTininessMode = tininess_modes[tmi];

                if (hardware) {
                    // in hardware tests we may use only supported tininess mode
                    currentTininessMode = hardware_tininess;
                    stopTininessModeLoop = true;
                } else if ((currentRoundingPrecision == FLOAT_ROUND_PRECISION_32)
                        || (currentRoundingPrecision == FLOAT_ROUND_PRECISION_64)) {
                    if ( !functions[functionCode].tininessModeAtReducedPrecision) {
                        currentTininessMode = static_cast<tininess_mode_t>(NOT_SET);
                        stopTininessModeLoop = true;
                    }
                    else if (tininessMode != static_cast<tininess_mode_t>(NOT_SET)) {
                        currentTininessMode = tininessMode;
                        stopTininessModeLoop = true;
                    }
                } else {
                    if (!functions[functionCode].tininessMode) {
                        currentTininessMode = static_cast<tininess_mode_t>(NOT_SET);
                        stopTininessModeLoop = true;
                    }
                    else if (tininessMode != static_cast<tininess_mode_t>(NOT_SET)) {
                        currentTininessMode = tininessMode;
                        stopTininessModeLoop = true;
                    }
                }

                ctx.roundingPrecision = currentRoundingPrecision;
                ctx.roundingMode      = currentRoundingMode;
                ctx.tininessMode      = currentTininessMode;

                setup_names(ctx);
                function(*this, ctx);

                if ((errorStop && anyErrors) || stop) {
                    abort_testing();
                }

                if (stopTininessModeLoop) break;
            }

            if (stopRoundingModeLoop) break;
        } // next rounding mode

        if (stopRoundingPrecisionLoop) break;
    } // next rounding precision
}


int
TestController::write_help(std::ostream& out, const char progName[])
{
    out <<
"Usage: " << progName << " [boost test parameters] -- [<option>...] <function>\n"
"  <option>:\n"
"    -help            --write this message and exit.\n"
"    -list            --list all testable functions and exit.\n"
"    -testsoft        --testing software implementation\n"
"    -testhard        --testing hardware/compiler implementation (default)\n"
"    -timesoft        --timing software implementation\n"
"    -timeslowsoft    --timing slow (etalon) software implementation\n"
"    -timehard        --timing hardware/compiler implementation\n"
"    -level <num>     --testing level <num>  - 1 or 2 (1 default).\n"
"    -errors <num>    --stop each function test after <num> errors. (25 default)\n"
"    -errorstop       --exit after first function with any error.\n"
"    -forever         --test one function repeatedly (implies `-level 2').\n"
"    -checknans       --check for bitwise correctness of NaN results.\n"
"    -nocheckflags    --no check for correctness of IEEE flags.\n"
"    -precisionXX     --only test rounding precision equivalent to XX bit - 32, 64 or 80.\n"
"    -nearesteven     --only test rounding to nearest/even.\n"
"    -tozero          --only test rounding to zero.\n"
"    -down            --only test rounding down.\n"
"    -up              --only test rounding up.\n"
"    -tininessbefore  --only test underflow tininess before rounding.\n"
"    -tininessafter   --only test underflow tininess after rounding.\n"
"    -allX            --all X-operand functions (X should be 1, 2 or 3).\n"
"    -floatXX         --all functions for some float type (XX should be 32, 64, 80, 128, 256 or 512).\n"
"    -intXX           --all functions for some int type (XX should be 32 or 64).\n"
"    -all             --all functions.\n"
"  <function>:  <int>_to_<float>, <float>_to_<int>, <float>_to_<int>_round_to_zero,\n"
"               <float>_add, <float>_sub, <float>_mul, <float>_div, <float>_rem, <float>_log2\n"
"               <float>_sqrt, <float>_muladd, <float>_to_<float>, <float>_round_to_int\n"
"               <float>_eq_signaling, <float>_eq_quiet, <float>_lt_signaling\n"
"               <float>_lt_quiet, <float>_le_signaling, <float>_le_quiet\n"
"  <int>: i32, i64.\n"
"  <float>: f32, f64, f80, f128, f256, f512.\n";

    return EXIT_FAILURE;
}


int
TestController::write_list(std::ostream& out)
{
    out << "Available functions:\n" << std::left;

    for (int n = 0, functionCode = 0; functionCode < NUM_FUNCTIONS; ++functionCode) {
        if (functions[ functionCode ].supported) {
           ++n;
           if ( (n % 4) == 0 ) {
              out << functions[ functionCode ].name << '\n';
           } else {
              out << std::setw(35) << functions[ functionCode ].name;
           }
        }
    }
    out << '\n';
    return EXIT_FAILURE;
}

void
TestController::abort_testing(void)
{
    throw std::runtime_error("test aborted");
    //std::exit(anyErrors ? EXIT_FAILURE : EXIT_SUCCESS);
}

static inline bool
streq(const char *a, const char *b)
{
    return strcmp(a, b) == 0;
}

static int
FindFunctionCode(const char *arg)
{
    for (int functionCode = 0; functionCode < NUM_FUNCTIONS; ++functionCode) {
        if (streq(arg, functions[functionCode].name)) {
            if (!functions[functionCode].supported) {
                LOG_ABORT << "Function " << arg
                          << " is not supported in hardware or cannot be tested";
            }
            return functionCode;
        }
    }
    return NOT_SET;
}

int
TestController::ParseCommandLine(int argc, char **argv)
{
    int functionCode = NOT_SET;

    set_defaults();

    const char *progName = argv[0];

    --argc;
    ++argv;

    if (argc <= 0)
       return EXIT_SUCCESS;

    bool functionArgument = false;

    for ( ; argc; --argc,  ++argv ) {
        const char *arg = argv[0];
        if (!arg) {
            break;
        }

        if ( arg[0] == '-' ) ++arg;

             if ( streq( arg, "help" ) ) {  return write_help(std::cout, progName);   }
        else if ( streq( arg, "list" ) ) {  return write_list(std::cout);  }
        else if ( streq( arg, "level" ) ) {
            if ( argc < 2 ) goto optionError;
            setLevel( atoi( argv[ 1 ] ) );
            --argc;
            ++argv;
        }
        else if ( streq( arg, "level1" ) ) {  setLevel( 1 );   }
        else if ( streq( arg, "level2" ) ) {  setLevel( 2 );   }
        else if ( streq( arg, "errors" ) ) {
            if ( argc < 2 ) {
     optionError:
                LOG_ABORT << argv[0] << " option requires numeric argument";
            }
            maxErrorCount = atoi( argv[ 1 ] );
            --argc;
            ++argv;
        }
        else if ( streq( arg, "errorstop"      ) ) {  errorStop = true;  }
        else if ( streq( arg, "forever"        ) ) {  setLevel( 2 );   forever = true;   }
        else if ( streq( arg, "checknans"      ) ) {  checkNaNs = true;   }
        else if ( streq( arg, "nocheckflags"   ) ) {  checkFlags = false;   }
        else if ( streq( arg, "precision32"    ) ) {  roundingPrecision = FLOAT_ROUND_PRECISION_32;  }
        else if ( streq( arg, "precision64"    ) ) {  roundingPrecision = FLOAT_ROUND_PRECISION_64;  }
        else if ( streq( arg, "precision80"    ) ) {  roundingPrecision = FLOAT_ROUND_PRECISION_80;  }
        else if ( streq( arg, "nearesteven"    ) ) {  roundingMode = FLOAT_ROUND_TONEAREST_EVEN;  }
        else if ( streq( arg, "tozero"         ) ) {  roundingMode = FLOAT_ROUND_TOWARDZERO;  }
        else if ( streq( arg, "down"           ) ) {  roundingMode = FLOAT_ROUND_DOWNWARD;   }
        else if ( streq( arg, "up"             ) ) {  roundingMode = FLOAT_ROUND_UPWARD;     }
        else if ( streq( arg, "tininessbefore" ) ) {  tininessMode = FLOAT_TININESS_BEFORE_ROUNDING;   }
        else if ( streq( arg, "tininessafter"  ) ) {  tininessMode = FLOAT_TININESS_AFTER_ROUNDING;    }
        else if ( streq( arg, "testsoft"       ) ) {  test_mode = TEST_SOFTWARE;   }
        else if ( streq( arg, "testhard"       ) ) {  test_mode = TEST_HARDWARE;   }
        else if ( streq( arg, "timesoft"       ) ) {  test_mode = TIME_SOFTWARE;   }
        else if ( streq( arg, "timehard"       ) ) {  test_mode = TIME_HARDWARE;   }
        else if ( streq( arg, "timeslowsoft"   ) ) {  test_mode = TIME_SLOWSOFTWARE;   }
        else if ( streq( arg, "i32"            ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_i32;  }
        else if ( streq( arg, "i64"            ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_i64;  }
        else if ( streq( arg, "f32"            ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_f32;  }
        else if ( streq( arg, "f64"            ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_f64;  }
        else if ( streq( arg, "f80"            ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_f80;  }
        else if ( streq( arg, "f128"           ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_f128;  }
        else if ( streq( arg, "f256"           ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_f256;  }
        else if ( streq( arg, "f512"           ) ) {  functionArgument = true;  functionCode = NOT_SET;  typesTested = type_f512;  }
        else if ( streq( arg, "all1"           ) ) {  functionArgument = true;  functionCode = NOT_SET;  operands = 1;  typesTested = type_all;  }
        else if ( streq( arg, "all2"           ) ) {  functionArgument = true;  functionCode = NOT_SET;  operands = 2;  typesTested = type_all;  }
        else if ( streq( arg, "all3"           ) ) {  functionArgument = true;  functionCode = NOT_SET;  operands = 3;  typesTested = type_all;  }
        else if ( streq( arg, "all"            ) ) {  functionArgument = true;  functionCode = NOT_SET;  operands = 0;  typesTested = type_all;  }
        else {
            functionCode = FindFunctionCode(arg);
            functionArgument = (functionCode != NOT_SET);
        }
    }

    if (!functionArgument) {
    //    LOG_ABORT << "Function argument required";
    }

    if (test_mode == TEST_SOFTWARE || test_mode == TEST_HARDWARE) {
       std::signal(SIGINT, catchSIGINT);
       std::signal(SIGTERM, catchSIGINT);
    }

    hardware_tininess = GetHardwareTininessMode();

    if ( (test_mode == TEST_HARDWARE) &&
         (tininessMode != hardware_tininess || tininessMode == static_cast<tininess_mode_t>(NOT_SET))
       ) {
       std::cout << "Switch tininess mode to hardware supported only\n";
       tininessMode = hardware_tininess;
    }

    if (functionCode != NOT_SET) {
        if (forever) {
            if ( roundingPrecision == static_cast<rounding_precision_t>(NOT_SET) ) roundingPrecision = FLOAT_ROUND_PRECISION_80;
            if ( roundingMode      == static_cast<rounding_mode_t     >(NOT_SET) ) roundingMode      = FLOAT_ROUND_TONEAREST_EVEN;
            if ( tininessMode      == static_cast<tininess_mode_t     >(NOT_SET) ) tininessMode      = FLOAT_TININESS_AFTER_ROUNDING;
        }
    } else {
        if (forever) {
            LOG_ABORT << "Can only test one function with `-forever' option";
        }
    }

    return EXIT_SUCCESS;
}

#if 0
int TestController::run(int argc, char **argv)
{
    const int result = ParseCommandLine(argc, argv);
    if (result != EXIT_SUCCESS)
    {
       return result;
    }

    if (functionCode != NOT_SET)
    {
        testFunction( functionCode, roundingPrecision, roundingMode, tininessMode, test_mode );
    }
    else
    {
        for ( functionCode = 0; functionCode < NUM_FUNCTIONS; ++functionCode )
        {
            if ( functions[ functionCode ].supported &&
                 ( operands == 0 || functions[ functionCode ].numInputs == operands ) &&
                 ( functions[ functionCode ].types & typesTested )
               )
            {
                testFunction( functionCode, roundingPrecision, roundingMode, tininessMode, test_mode );
            }
        }
    }

    return anyErrors ? EXIT_FAILURE : EXIT_SUCCESS;
}
#endif

FpuTestFixture::FpuTestFixture()
{
    setup();
}

FpuTestFixture::~FpuTestFixture()
{
    BOOST_TEST_MESSAGE( "FpuTestFixture teardown" );
}

void
FpuTestFixture::setup(void)
{
    BOOST_TEST_MESSAGE( "FpuTestFixture setup" ); 

    const int argc = boost::unit_test::framework::master_test_suite().argc;
    char**    argv = boost::unit_test::framework::master_test_suite().argv;

    ctrl.ParseCommandLine(argc, argv);
}

} // namespace fpu
} // namespace postrisc
