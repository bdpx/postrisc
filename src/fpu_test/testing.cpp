#include <boost/test/unit_test.hpp>

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

template<typename SOURCE, typename RESULT>
class BaseErrorData
{
public:
    BaseErrorData(TestController& _ctrl, RESULT _trueZ, float_flag _trueFlags, RESULT _testZ, float_flag _testFlags)
       : ctrl(_ctrl), trueZ(_trueZ), trueFlags(_trueFlags), testZ(_testZ), testFlags(_testFlags)
    {}

protected:
    void writeHeader(std::ostream& out) const
    {
        ctrl.writeErrorFound(out);
    }

    void writeResult(std::ostream& out) const
    {
        ctrl.writeOutput(out, trueZ, trueFlags, testZ, testFlags);
    }

private:
    TestController& ctrl;
    RESULT            trueZ;
    float_flag        trueFlags;
    RESULT            testZ;
    float_flag        testFlags;
};

template<typename SOURCE, typename RESULT>
class ErrorData_1 : public BaseErrorData<SOURCE, RESULT>
{
public:
    ErrorData_1(TestController& _ctrl, SOURCE _a,
                 RESULT _trueZ, float_flag _trueFlags,
                 RESULT _testZ, float_flag _testFlags)
       : BaseErrorData<SOURCE, RESULT>(_ctrl, _trueZ, _trueFlags, _testZ, _testFlags)
       , a(_a) {}

    friend std::ostream& operator<<(std::ostream& out, const ErrorData_1& data) {
        return data.write(out);
    }

private:
    std::ostream& write(std::ostream& out) const
    {
        BaseErrorData<SOURCE, RESULT>::writeHeader(out);
        out << "1: " << fmt::hex(a) << fmt::endl;
        BaseErrorData<SOURCE, RESULT>::writeResult(out);
        return out;
    }

private:
    SOURCE            a;
};

template<typename SOURCE, typename RESULT>
class ErrorData_2 : public BaseErrorData<SOURCE, RESULT>
{
public:
    ErrorData_2(TestController& _ctrl, SOURCE _a, SOURCE _b,
                 RESULT _trueZ, float_flag _trueFlags,
                 RESULT _testZ, float_flag _testFlags)
       : BaseErrorData<SOURCE, RESULT>(_ctrl, _trueZ, _trueFlags, _testZ, _testFlags)
       , a(_a), b(_b) {}

    friend std::ostream& operator<<(std::ostream& out, const ErrorData_2& data) {
        return data.write(out);
    }

private:
    std::ostream& write(std::ostream& out) const
    {
        BaseErrorData<SOURCE, RESULT>::writeHeader(out);
        out << "1: " << fmt::hex(a) << fmt::endl
            << "2: " << fmt::hex(b) << fmt::endl;

        BaseErrorData<SOURCE, RESULT>::writeResult(out);
        return out;
    }

private:
    SOURCE            a;
    SOURCE            b;
};

template<typename SOURCE, typename RESULT>
class ErrorData_3 : public BaseErrorData<SOURCE, RESULT>
{
public:
    ErrorData_3(TestController& _ctrl, SOURCE _a, SOURCE _b, SOURCE _c,
                 RESULT _trueZ, float_flag _trueFlags,
                 RESULT _testZ, float_flag _testFlags)
       : BaseErrorData<SOURCE, RESULT>(_ctrl, _trueZ, _trueFlags, _testZ, _testFlags)
       , a(_a), b(_b), c(_c) {}

    friend std::ostream& operator<<(std::ostream& out, const ErrorData_3& data)
    {
        return data.write(out);
    }

private:
    std::ostream& write(std::ostream& out) const
    {
        BaseErrorData<SOURCE, RESULT>::writeHeader(out);
        out << "1: " << fmt::hex(a) << fmt::endl
            << "2: " << fmt::hex(b) << fmt::endl
            << "3: " << fmt::hex(c) << fmt::endl;

        BaseErrorData<SOURCE, RESULT>::writeResult(out);
        return out;
    }

private:
    SOURCE            a;
    SOURCE            b;
    SOURCE            c;
};

template<typename T>
void
TestController::writeOutput(std::ostream& out, T trueZ, float_flag trueFlags, T testZ, float_flag testFlags )
{
    out << trueName << ": " << fmt::hex(trueZ) << ' ' << trueFlags << fmt::endl
        << testName << ": " << fmt::hex(testZ) << ' ' << testFlags << fmt::endl
        << std::endl;
}



template<typename INT, typename FLOAT, typename TRUE_FPU, typename TEST_FPU>
void
TestController::test_i_f(
    FLOAT (TRUE_FPU::*trueFunction)( INT ),
    FLOAT (TEST_FPU::*testFunction)( INT ),
    TRUE_FPU& true_fpu,
    TEST_FPU& test_fpu )
{
    u64       count = 0;
    INT       a;

    errorCount = 0;
    start(a);
    writeTestsTotal();

    while (!done || forever) {
        next(a);

        true_fpu.clear_flags();
        FLOAT trueZ = (true_fpu.*trueFunction)( a );
        float_flag trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        FLOAT testZ = (test_fpu.*testFunction)( a );
        float_flag testFlags = test_fpu.get_flags();

        checkEarlyExit(count);

        if ((trueZ.val() != testZ.val()) || (checkFlags && trueFlags != testFlags)) {
            const bool ok = (
                    ! checkNaNs
                 && trueZ.IsNan()
                 && testZ.IsNan()
                 && ! testZ.IsSignalingNan()
                 && ( !checkFlags || trueFlags == testFlags )
               );

            if (!ok) {
                ++errorCount;
                ErrorData_1<INT, FLOAT> loginfo(*this, a, trueZ, trueFlags, testZ, testFlags);
                BOOST_TEST(ok, "\n" << loginfo);

                if (errorCount == maxErrorCount) {
                    break;
                }
            }
        }
    }

    writeTestsPerformed(count);
}


template<typename FLOAT, typename INT, typename TRUE_FPU, typename TEST_FPU>
void
TestController::test_f_i(
    INT (TRUE_FPU::*trueFunction)( FLOAT ),
    INT (TEST_FPU::*testFunction)( FLOAT ),
    TRUE_FPU& true_fpu,
    TEST_FPU& test_fpu )
{
    u64      count = 0;
    FLOAT    a;

    errorCount = 0;
    start(a);
    writeTestsTotal();
    while (!done || forever) {
        next(a);

        true_fpu.clear_flags();
        INT trueZ = (true_fpu.*trueFunction)( a );
        float_flag trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        INT testZ = (test_fpu.*testFunction)( a );
        float_flag testFlags = test_fpu.get_flags();

        checkEarlyExit(count);

        if ((trueZ != testZ) || (checkFlags && trueFlags != testFlags)) {
            if (!checkNaNs && a.IsSignalingNan()) {
                trueFlags = trueFlags | FLOAT_FLAG_INVALID;
            }

            const bool ok = (
                    ( ( trueZ == std::numeric_limits<INT>::max() ) || ( trueZ == std::numeric_limits<INT>::min() ) )
                 && ( ( testZ == std::numeric_limits<INT>::max() ) || ( testZ == std::numeric_limits<INT>::min() ) )
            //     && ! checkFlags
                 && ( trueFlags & FLOAT_FLAG_INVALID ) // if we have INVALID - ignore INEXACT and other flags
                 && ( testFlags & FLOAT_FLAG_INVALID )
               );

            if (!ok) {
                ++errorCount;
                ErrorData_1<FLOAT, INT> loginfo(*this, a, trueZ, trueFlags, testZ, testFlags);
                BOOST_TEST(ok, "\n" << loginfo);

                if (errorCount == maxErrorCount) {
                    break;
                }
            }
        }
    }

    writeTestsPerformed(count);
}


template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
void
TestController::test_f_f(
    RESULT (TRUE_FPU::*trueFunction)( T ),
    RESULT (TEST_FPU::*testFunction)( T ),
    TRUE_FPU& true_fpu,
    TEST_FPU& test_fpu )
{
    u64      count = 0;
    T        a;

    errorCount = 0;
    start(a);
    writeTestsTotal();

    while (!done || forever) {
        next(a);

        true_fpu.clear_flags();
        RESULT trueZ = (true_fpu.*trueFunction)( a );
        float_flag trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        RESULT testZ = (test_fpu.*testFunction)( a );
        float_flag testFlags = test_fpu.get_flags();

        checkEarlyExit(count);

        if ((trueZ.val() != testZ.val()) || (checkFlags && trueFlags != testFlags)) {
            if (!checkNaNs && a.IsSignalingNan()) {
                trueFlags = trueFlags | FLOAT_FLAG_INVALID;
            }

            const bool ok = ( ! checkNaNs
                 && trueZ.IsNan()
                 && testZ.IsNan()
                 && ! testZ.IsSignalingNan()
                 && ( !checkFlags || trueFlags == testFlags )
               );

            if (!ok) {
                ++errorCount;
                ErrorData_1<T,RESULT> loginfo(*this, a, trueZ, trueFlags, testZ, testFlags);
                BOOST_TEST(ok, "\n" << loginfo);

                if (errorCount == maxErrorCount) {
                    break;
                }
            }
        }
    }

    writeTestsPerformed(count);
}


template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
void
TestController::test_af_f(
    RESULT (TRUE_FPU::*trueFunction)( T ),
    RESULT (TEST_FPU::*testFunction)( T ),
    TRUE_FPU& true_fpu,
    TEST_FPU& test_fpu )
{
    u64      count = 0;
    T        a;

    errorCount = 0;
    start(a);
    writeTestsTotal();

    while (!done || forever) {
        next(a);

        true_fpu.clear_flags();
        RESULT trueZ = (true_fpu.*trueFunction)( a.abs() );
        float_flag trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        RESULT testZ = (test_fpu.*testFunction)( a.abs() );
        float_flag testFlags = test_fpu.get_flags();

        checkEarlyExit(count);

        if ((trueZ.val() != testZ.val()) || (checkFlags && trueFlags != testFlags)) {
            if (!checkNaNs && a.IsSignalingNan()) {
                trueFlags = trueFlags | FLOAT_FLAG_INVALID;
            }

            const bool ok = ( ! checkNaNs
                 && trueZ.IsNan()
                 && testZ.IsNan()
                 && ! testZ.IsSignalingNan()
                 && ( !checkFlags || trueFlags == testFlags )
               );

            if (!ok) {
                ++errorCount;
                ErrorData_1<T,RESULT> loginfo(*this, a, trueZ, trueFlags, testZ, testFlags);
                BOOST_TEST(ok, "\n" << loginfo);

                if (errorCount == maxErrorCount) {
                    break;
                }
            }
        }
    }

    writeTestsPerformed(count);
}


template<typename FLOAT, typename FLAG, typename TRUE_FPU, typename TEST_FPU>
void
TestController::test_ff_b(
    FLAG (TRUE_FPU::*trueFunction)( FLOAT, FLOAT ),
    FLAG (TEST_FPU::*testFunction)( FLOAT, FLOAT ),
    TRUE_FPU& true_fpu,
    TEST_FPU& test_fpu )
{
    u64      count = 0;
    FLOAT    a, b;

    errorCount = 0;
    start(a, b);
    writeTestsTotal();

    while (!done || forever) {
        next(a, b);

        true_fpu.clear_flags();
        FLAG trueZ = (true_fpu.*trueFunction)( a, b );
        float_flag trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        FLAG testZ = (test_fpu.*testFunction)( a, b );
        float_flag testFlags = test_fpu.get_flags();

        checkEarlyExit(count);

        if ((trueZ != testZ) || (checkFlags && trueFlags != testFlags)) {
            if ( ! checkNaNs
                 && (    a.IsSignalingNan()
                      || b.IsSignalingNan() )
               )
            {
                trueFlags = trueFlags | FLOAT_FLAG_INVALID;
            }

            if ((trueZ != testZ) || (checkFlags && trueFlags != testFlags)) {
                ++errorCount;
                ErrorData_2<FLOAT,FLAG> loginfo(*this, a, b, trueZ, trueFlags, testZ, testFlags);
                BOOST_TEST(0, "" << loginfo);

                if (errorCount == maxErrorCount) {
                    break;
                }
            }
        }
    }

    writeTestsPerformed(count);
}


template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
void
TestController::test_ff_f(
    RESULT (TRUE_FPU::*trueFunction)( T, T ),
    RESULT (TEST_FPU::*testFunction)( T, T ),
    TRUE_FPU& true_fpu,
    TEST_FPU& test_fpu )
{
    u64       count = 0;
    T         a, b;

    errorCount = 0;
    start(a, b);
    writeTestsTotal();

    while (!done || forever) {
        next(a, b);

        true_fpu.clear_flags();
        RESULT trueZ = (true_fpu.*trueFunction)( a, b );
        float_flag trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        RESULT testZ = (test_fpu.*testFunction)( a, b );
        float_flag testFlags = test_fpu.get_flags();

        checkEarlyExit(count);

        if ((trueZ.val() != testZ.val()) || (checkFlags && trueFlags != testFlags)) {
            if (    ! checkNaNs
                 && (    a.IsSignalingNan()
                      || b.IsSignalingNan() )
               )
            {
                trueFlags = trueFlags | FLOAT_FLAG_INVALID;
            }

            const bool ok = ( ! checkNaNs
                 && trueZ.IsNan()
                 && testZ.IsNan()
                 && ! testZ.IsSignalingNan()
                 && ( !checkFlags || trueFlags == testFlags )
               );

            if (!ok) {
                ++errorCount;
                ErrorData_2<T,RESULT> loginfo(*this, a, b, trueZ, trueFlags, testZ, testFlags);
                BOOST_TEST(ok, "\n" << loginfo);

                if (errorCount == maxErrorCount) {
                    break;
                }
            }
        }
    }

    writeTestsPerformed(count);
}


template<typename T, typename RESULT, typename TRUE_FPU, typename TEST_FPU>
void
TestController::test_fff_f(
    RESULT (TRUE_FPU::*trueFunction)( T, T, T, muladd_negate_t flags ),
    RESULT (TEST_FPU::*testFunction)( T, T, T, muladd_negate_t flags ),
    TRUE_FPU& true_fpu,
    TEST_FPU& test_fpu )
{
    u64       count = 0;
    T         a, b, c;

    errorCount = 0;
    start(a, b, c);
    writeTestsTotal();

    while ( count < total || forever ) {
        next(a, b, c);

        true_fpu.clear_flags();
        RESULT trueZ = (true_fpu.*trueFunction)( a, b, c, FLOAT_MULADD_NEGATE_NONE );
        float_flag trueFlags = true_fpu.get_flags();

        test_fpu.clear_flags();
        RESULT testZ = (test_fpu.*testFunction)( a, b, c, FLOAT_MULADD_NEGATE_NONE );
        float_flag testFlags = test_fpu.get_flags();

        checkEarlyExit(count);

        if ((trueZ.val() != testZ.val()) || (checkFlags && trueFlags != testFlags)) {
            if (!checkNaNs && ( a.IsSignalingNan() || b.IsSignalingNan() || c.IsSignalingNan())) {
                trueFlags = trueFlags | FLOAT_FLAG_INVALID;
            }

            const bool ok = ( ! checkNaNs
                 && trueZ.IsNan()
                 && testZ.IsNan()
                 && ! testZ.IsSignalingNan()
                 && ( !checkFlags || trueFlags == testFlags )
               );

            if (!ok) {
                ++errorCount;
                ErrorData_3<T,RESULT> loginfo(*this, a, b, c, trueZ, trueFlags, testZ, testFlags);
                BOOST_TEST(ok, "\n" << loginfo);

                if (errorCount == maxErrorCount) {
                    break;
                }
            }
        }
    }

    writeTestsPerformed(count);
}


BOOST_AUTO_TEST_SUITE(test)

#define FPU_TESTING_TEST_CASE(TRUE_FPU_TYPE, TEST_FPU_TYPE, Templt, supported, T1, T2, method, hardware) \
    BOOST_FIXTURE_TEST_CASE(test_ ## T1 ## _ ## method, FpuTestFixture)                                  \
    {                                                                                            \
        int const functionCode = T1 ## _ ## method;                                              \
        unsigned const supportedMask = hardware ? SUPPORTED_HARD : SUPPORTED_SOFT;               \
        TestFunctionPtr func = [](TestController& ctr, TestContext context)                      \
        {                                                                                        \
            ctr.init_sequencer();                                                                \
            ctr.writeFunctionName( std::cout << "Testing " );                                    \
            ctr.trueName = #TRUE_FPU_TYPE;                                                       \
            ctr.testName = #TEST_FPU_TYPE;                                                       \
            TRUE_FPU_TYPE     true_fpu;                                                          \
            TEST_FPU_TYPE     test_fpu;                                                          \
            ctr.setup_fpu(true_fpu, context);                                                    \
            ctr.setup_fpu(test_fpu, context);                                                    \
            ctr.test_ ## Templt < T1, T2, TRUE_FPU_TYPE, TEST_FPU_TYPE >                         \
                (&TRUE_FPU_TYPE::method, &TEST_FPU_TYPE::method, true_fpu, test_fpu );           \
        };                                                                                       \
        if ((SUPPORTED_##supported) & supportedMask)                                             \
        {                                                                                        \
            BOOST_CHECK_NO_THROW(ctrl.testingFunction(functionCode, func, hardware));            \
        }                                                                                        \
    }

#if 1

BOOST_AUTO_TEST_SUITE(hard_soft)
#define FPU_HARD_SOFT_TEST_FIXTURE(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) \
    FPU_TESTING_TEST_CASE(HardFPU, SoftFPU, Templt, supported, T1, T2, method, true)
DECLARE_HARDWARE_FUNCTIONS_TABLE(FPU_HARD_SOFT_TEST_FIXTURE)
BOOST_AUTO_TEST_SUITE_END() // hard_soft

BOOST_AUTO_TEST_SUITE(slow_soft)
#define FPU_TESTSOFT_SOFT_TEST_FIXTURE(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) \
    FPU_TESTING_TEST_CASE(TestFPU, SoftFPU, Templt, supported, T1, T2, method, false)
DECLARE_FUNCTIONS_TABLE(FPU_TESTSOFT_SOFT_TEST_FIXTURE)
BOOST_AUTO_TEST_SUITE_END() // testsoft_soft

BOOST_AUTO_TEST_SUITE(hard_slow)
#define FPU_TESTSOFT_HARD_TEST_FIXTURE(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) \
    FPU_TESTING_TEST_CASE(HardFPU, TestFPU, Templt, supported, T1, T2, method, true)
DECLARE_HARDWARE_FUNCTIONS_TABLE(FPU_TESTSOFT_HARD_TEST_FIXTURE)
BOOST_AUTO_TEST_SUITE_END() // testsoft_hard

#endif

BOOST_AUTO_TEST_SUITE_END() // test

} // namespace fpu
} // namespace postrisc
