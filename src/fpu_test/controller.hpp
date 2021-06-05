#pragma once

#include "float.hpp"
#include "sequencer.hpp"

namespace postrisc {
namespace fpu {

enum type_tested_t {
    type_bool     = 1 << 0,
    type_i32      = 1 << 1,
    type_i64      = 1 << 2,
    type_f32      = 1 << 5,
    type_f64      = 1 << 6,
    type_f80      = 1 << 7,
    type_f128     = 1 << 8,
    type_f256     = 1 << 9,
    type_f512     = 1 << 10,
    type_all      = -1
};

/********************************************************************
* Main class for testing
********************************************************************/
enum test_mode_t {
    TEST_HARDWARE,
    TEST_SOFTWARE,
    TIME_SOFTWARE,
    TIME_HARDWARE,
    TIME_SLOWSOFTWARE
};

class TestContext {
public:
    int                  functionCode;
    rounding_precision_t roundingPrecision;
    rounding_mode_t      roundingMode;
    tininess_mode_t      tininessMode;
};

class TestController;

using TestFunctionPtr = void (*)(TestController & ctrl, TestContext it);

class TestController {
public:
    TestController(void);
    ~TestController(void);

    int ParseCommandLine( int argc, char **argv );
    int run( int argc, char **argv );

    void testingFunction(int functionCode, TestFunctionPtr function, bool hardware);

    static tininess_mode_t GetHardwareTininessMode(void);

    void writeErrorFound(std::ostream& out);
    template<typename T>
    void writeOutput(std::ostream& out, T trueZ, float_flag trueFlags, T testZ, float_flag testFlags);

public:
    static void catchSIGINT(int signalCode);
    static int write_help(std::ostream& out, const char progName[]);
    static int write_list(std::ostream& out);

    void set_defaults(void);
    void setLevel(int levelIn);
    void init_sequencer(void);
    void writeTestsTotal( void );
    void writeFunctionName(std::ostream& out);
    void writeTestsPerformed(u64 count);
    void checkEarlyExit(u64 & count);
    void abort_testing(void);

    // setup start values in sequences
    template<typename T> void start(T & a);
    template<typename T> void start(T & a, T & b);
    template<typename T> void start(T & a, T & b, T & c);

    // get next values in sequence
    template<typename T> void next( T & a );
    template<typename T> void next( T & a, T & b );
    template<typename T> void next( T & a, T & b, T & c );

    // test engine
    void testFunctionVariety( int functionCode, rounding_precision_t roundingPrecision,
         rounding_mode_t roundingMode, tininess_mode_t tininessMode, test_mode_t test_mode );

    void setup_names(TestContext & context);

public:
    template<typename FPU>
    static void setup_fpu(FPU & fpu, const TestContext & context);

    // test templates
    template<typename INT, typename FLOAT, typename TRUE_FPU, typename TEST_FPU>
    void test_i_f(
         FLOAT (TRUE_FPU::*trueFunction)( INT ),
         FLOAT (TEST_FPU::*testFunction)( INT ),
         TRUE_FPU & true_fpu,
         TEST_FPU & test_fpu );

    template<typename FLOAT, typename INT, typename TRUE_FPU, typename TEST_FPU>
    void test_f_i(
         INT (TRUE_FPU::*trueFunction)( FLOAT ),
         INT (TEST_FPU::*testFunction)( FLOAT ),
         TRUE_FPU & true_fpu,
         TEST_FPU & test_fpu );

    template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
    void test_f_f(
         RESULT (TRUE_FPU::*trueFunction)( T ),
         RESULT (TEST_FPU::*testFunction)( T ),
         TRUE_FPU & true_fpu,
         TEST_FPU & test_fpu );

    // like test_f_f, but with absolute value - for sqrt, log2
    template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
    void test_af_f(
         RESULT (TRUE_FPU::*trueFunction)( T ),
         RESULT (TEST_FPU::*testFunction)( T ),
         TRUE_FPU & true_fpu,
         TEST_FPU & test_fpu );

    template<typename FLOAT, typename FLAG, typename TRUE_FPU, typename TEST_FPU>
    void test_ff_b(
         FLAG (TRUE_FPU::*trueFunction)( FLOAT, FLOAT ),
         FLAG (TEST_FPU::*testFunction)( FLOAT, FLOAT ),
         TRUE_FPU & true_fpu,
         TEST_FPU & test_fpu );

    template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
    void test_ff_f(
         RESULT (TRUE_FPU::*trueFunction)( T, T ),
         RESULT (TEST_FPU::*testFunction)( T, T ),
         TRUE_FPU & true_fpu,
         TEST_FPU & test_fpu );

    template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
    void test_fff_f(
         RESULT (TRUE_FPU::*trueFunction)( T, T, T, muladd_negate_t flags ),
         RESULT (TEST_FPU::*testFunction)( T, T, T, muladd_negate_t flags ),
         TRUE_FPU & true_fpu,
         TEST_FPU & test_fpu );

    void reportTime(i64 count, double seconds);

#define NO_OPTIMIZE // __attribute__((optimize("O0")))

    template<typename T, typename RESULT, typename TEST_FPU>
    void time_f1(
        RESULT (TEST_FPU::*function)( T ),
        TEST_FPU & fpu
       ) NO_OPTIMIZE;

    template<typename T, typename RESULT, typename TEST_FPU>
    void time_a1(
        RESULT (TEST_FPU::*function)( T ),
        TEST_FPU & fpu
       ) NO_OPTIMIZE;

    template<typename T, typename RESULT, typename TEST_FPU>
    void time_f2(
        RESULT (TEST_FPU::*function)( T, T ),
        TEST_FPU & fpu
       ) NO_OPTIMIZE;

    template<typename T, typename RESULT, typename TEST_FPU>
    void time_f3(
        RESULT (TEST_FPU::*function)( T, T, T, muladd_negate_t ),
        TEST_FPU & fpu
       ) NO_OPTIMIZE;

private:
    static u8      stop;

public:
    test_mode_t          test_mode;
    //int                  functionCode;
    int                  operands;
    type_tested_t        typesTested;
    rounding_precision_t roundingPrecision;
    rounding_mode_t      roundingMode;
    tininess_mode_t      tininessMode;

public:
    TestSequencer       sequenceA;
    TestSequencer       sequenceB;
    TestSequencer       sequenceC;

    u64                 total;
    int                 level;
    int                 subcase;

    bool                done;
    bool                checkNaNs;
    bool                checkFlags;
    bool                anyErrors;
    bool                forever;
    bool                errorStop;
    bool                is_cout_tty;

    tininess_mode_t     hardware_tininess;
    u32                 maxErrorCount;
    u32                 errorCount;

    std::string         trueName;
    std::string         testName;
    std::string         functionName;
    std::string         roundingPrecisionName;
    std::string         roundingModeName;
    std::string         tininessModeName;

};

template<typename FPU>
void TestController::setup_fpu(FPU& fpu, const TestContext& context)
{
    fpu.set_rounding_precision(context.roundingPrecision);
    fpu.set_rounding_mode(context.roundingMode);
    fpu.set_tininess_mode(context.tininessMode);
}

template<typename T>
void TestController::start(T & a)
{
    total = TestSequencer::GetTotal(level, a);
}

template<typename T>
void TestController::start(T & a, T & b)
{
    total = TestSequencer::GetTotal(level,a,b);

    if (level == 1)
       sequenceA.NextP1(a);
    else
       sequenceA.NextP2(a);
}

#define CASE(level, subcase) ((level) * 10 + (subcase))

template<> void TestController::next<i32>(i32& a);
template<> void TestController::next<i64>(i64& a);

template<typename T>
void TestController::next(T & a)
{
    switch(CASE(level, subcase)) {
        case CASE(1,0):
        case CASE(1,1):
            sequenceA.Random(a);
            break;

        case CASE(1,2):
            sequenceA.NextP1(a);
            done = sequenceA.done;
            subcase = -1;
            break;

        case CASE(2,0):
            sequenceA.Random(a);
            break;

        case CASE(2,1):
            sequenceA.NextP2(a);
            done = sequenceA.done;
            subcase = -1;
            break;
    }
    ++subcase;
}


template<typename T>
void TestController::next(T & a, T & b)
{
    switch (CASE(level, subcase)) {
        case CASE(1,0):
            if ( sequenceB.done ) {
                sequenceB.done = false;
                sequenceA.NextP1(a);
            }
            sequenceB.NextP1(b);
            break;

        case CASE(1,2):
        case CASE(1,4):
            sequenceA.Random(a);
            sequenceB.Random(b);
            break;

        case CASE(1,1):
            //a = current_a_f32;
            sequenceB.Random(b);
            break;

        case CASE(1,3):
            sequenceA.Random(a);
            //b = current_b_f32;
            break;

        case CASE(1,5):
            //a = current_a_f32;
            //b = current_b_f32;
            done = sequenceA.done & sequenceB.done;
            subcase = -1;
            break;

        case CASE(2,0):
            sequenceA.Random(a);
            sequenceB.Random(b);
            break;

        case CASE(2,1):
            if ( sequenceB.done ) {
                sequenceB.done = false;
                sequenceA.NextP2(a);
            }
            //a = current_a_f32;
            sequenceB.NextP2(b);
            done = sequenceA.done & sequenceB.done;
            subcase = -1;
            break;
    }
    ++subcase;
}

template<typename T>
void TestController::start(T &, T &, T &)
{
    total = level == 1 ? 1000 * 1000 : 10 * 1000 * 1000;
}

template<typename T>
void TestController::next(T & a, T & b, T & c)
{
    sequenceA.Random(a);
    sequenceB.Random(b);
    sequenceC.Random(c);
}

class FpuTestFixture {
public:
    FpuTestFixture();
    ~FpuTestFixture();

    void setup(void);

public:
    TestController  ctrl;
};

} // namespace fpu
} // namespace postrisc
