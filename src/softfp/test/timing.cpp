#include <unistd.h>   // for isatty
#include <chrono>      // for std::chrono::steady_clock/duration/etc

#include "util/common.hpp"

namespace postrisc {
namespace fpu {

/****************************************************************************************
* performance tests templates
****************************************************************************************/
enum {
    NUM_INPUTS      = 32,
    MIN_ITERATIONS  = 500 * 1000
};

//std::chrono::steady_clock
using Time = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Time>;

static double
duration_in_seconds(const TimePoint& start)
{
    const TimePoint end = Time::now();

    const std::chrono::duration<double> diff = end - start;
    const double run_time = diff.count();
    // std::cout << "run_time=" << std::setprecision(6) << run_time << std::endl;
    return run_time;
}

static bool
duration_is_enough(const TimePoint& start)
{
    const double duration_threshold = 0.5; // seconds
    return duration_in_seconds(start) < duration_threshold;
}

static void
reportTime(int64_t count, double seconds)
{
    const double mops = (count / seconds) * 1.0e-6;
    std::cout << std::fixed << std::setprecision(6)
              << std::setw(10) << mops << " Mops/s: " << std::endl;
    // std::cout << " rounding " << roundingModeName;
}

template<class T> class test_data
{
public:
    static const std::array<T, NUM_INPUTS> values;
};

template<>
const std::array<int32_t, NUM_INPUTS> test_data<int32_t>::values =
{
    int32_t( INT32_C( 0xFFFFBB79 )), int32_t( INT32_C( 0x405CF80F )),
    int32_t( INT32_C( 0x00000000 )), int32_t( INT32_C( 0xFFFFFD04 )),
    int32_t( INT32_C( 0xFFF20002 )), int32_t( INT32_C( 0x0C8EF795 )),
    int32_t( INT32_C( 0xF00011FF )), int32_t( INT32_C( 0x000006CA )),
    int32_t( INT32_C( 0x00009BFE )), int32_t( INT32_C( 0xFF4862E3 )),
    int32_t( INT32_C( 0x9FFFEFFE )), int32_t( INT32_C( 0xFFFFFFB7 )),
    int32_t( INT32_C( 0x0BFF7FFF )), int32_t( INT32_C( 0x0000F37A )),
    int32_t( INT32_C( 0x0011DFFE )), int32_t( INT32_C( 0x00000006 )),
    int32_t( INT32_C( 0xFFF02006 )), int32_t( INT32_C( 0xFFFFF7D1 )),
    int32_t( INT32_C( 0x10200003 )), int32_t( INT32_C( 0xDE8DF765 )),
    int32_t( INT32_C( 0x00003E02 )), int32_t( INT32_C( 0x000019E8 )),
    int32_t( INT32_C( 0x0008FFFE )), int32_t( INT32_C( 0xFFFFFB5C )),
    int32_t( INT32_C( 0xFFDF7FFE )), int32_t( INT32_C( 0x07C42FBF )),
    int32_t( INT32_C( 0x0FFFE3FF )), int32_t( INT32_C( 0x040B9F13 )),
    int32_t( INT32_C( 0xBFFFFFF8 )), int32_t( INT32_C( 0x0001BF56 )),
    int32_t( INT32_C( 0x000017F6 )), int32_t( INT32_C( 0x000A908A ))
};

template<>
const std::array<int64_t, NUM_INPUTS> test_data<int64_t>::values =
{
    int64_t( INT64_C( 0xFBFFC3FFFFFFFFFF )),
    int64_t( INT64_C( 0x0000000003C589BC )),
    int64_t( INT64_C( 0x00000000400013FE )),
    int64_t( INT64_C( 0x0000000000186171 )),
    int64_t( INT64_C( 0xFFFFFFFFFFFEFBFA )),
    int64_t( INT64_C( 0xFFFFFD79E6DFFC73 )),
    int64_t( INT64_C( 0x0000000010001DFF )),
    int64_t( INT64_C( 0xDD1A0F0C78513710 )),
    int64_t( INT64_C( 0xFFFF83FFFFFEFFFE )),
    int64_t( INT64_C( 0x00756EBD1AD0C1C7 )),
    int64_t( INT64_C( 0x0003FDFFFFFFFFBE )),
    int64_t( INT64_C( 0x0007D0FB2C2CA951 )),
    int64_t( INT64_C( 0x0007FC0007FFFFFE )),
    int64_t( INT64_C( 0x0000001F942B18BB )),
    int64_t( INT64_C( 0x0000080101FFFFFE )),
    int64_t( INT64_C( 0xFFFFFFFFFFFF0978 )),
    int64_t( INT64_C( 0x000000000008BFFF )),
    int64_t( INT64_C( 0x0000000006F5AF08 )),
    int64_t( INT64_C( 0xFFDEFF7FFFFFFFFE )),
    int64_t( INT64_C( 0x0000000000000003 )),
    int64_t( INT64_C( 0x3FFFFFFFFF80007D )),
    int64_t( INT64_C( 0x0000000000000078 )),
    int64_t( INT64_C( 0xFFF80000007FDFFD )),
    int64_t( INT64_C( 0x1BBC775B78016AB0 )),
    int64_t( INT64_C( 0xFFF9001FFFFFFFFE )),
    int64_t( INT64_C( 0xFFFD4767AB98E43F )),
    int64_t( INT64_C( 0xFFFFFEFFFE00001E )),
    int64_t( INT64_C( 0xFFFFFFFFFFF04EFD )),
    int64_t( INT64_C( 0x07FFFFFFFFFFF7FF )),
    int64_t( INT64_C( 0xFFFC9EAA38F89050 )),
    int64_t( INT64_C( 0x00000020FBFFFFFE )),
    int64_t( INT64_C( 0x0000099AE6455357 ))
};

template<>
const std::array<float32, NUM_INPUTS> test_data<float32>::values =
{
    FLOAT32_C( 0x4EFA0000 ),
    FLOAT32_C( 0xC1D0B328 ),
    FLOAT32_C( 0x80000000 ),
    FLOAT32_C( 0x3E69A31E ),
    FLOAT32_C( 0xAF803EFF ),
    FLOAT32_C( 0x3F800000 ),
    FLOAT32_C( 0x17BF8000 ),
    FLOAT32_C( 0xE74A301A ),
    FLOAT32_C( 0x4E010003 ),
    FLOAT32_C( 0x7EE3C75D ),
    FLOAT32_C( 0xBD803FE0 ),
    FLOAT32_C( 0xBFFEFF00 ),
    FLOAT32_C( 0x7981F800 ),
    FLOAT32_C( 0x431FFFFC ),
    FLOAT32_C( 0xC100C000 ),
    FLOAT32_C( 0x3D87EFFF ),
    FLOAT32_C( 0x4103FEFE ),
    FLOAT32_C( 0xBC000007 ),
    FLOAT32_C( 0xBF01F7FF ),
    FLOAT32_C( 0x4E6C6B5C ),
    FLOAT32_C( 0xC187FFFE ),
    FLOAT32_C( 0xC58B9F13 ),
    FLOAT32_C( 0x4F88007F ),
    FLOAT32_C( 0xDF004007 ),
    FLOAT32_C( 0xB7FFD7FE ),
    FLOAT32_C( 0x7E8001FB ),
    FLOAT32_C( 0x46EFFBFF ),
    FLOAT32_C( 0x31C10000 ),
    FLOAT32_C( 0xDB428661 ),
    FLOAT32_C( 0x33F89B1F ),
    FLOAT32_C( 0xA3BFEFFF ),
    FLOAT32_C( 0x537BFFBE ),
};

template<>
const std::array<float64, NUM_INPUTS> test_data<float64>::values =
{
    FLOAT64_C( 0x422FFFC008000000 ),
    FLOAT64_C( 0xB7E0000480000000 ),
    FLOAT64_C( 0xF3FD2546120B7935 ),
    FLOAT64_C( 0x3FF0000000000000 ),
    FLOAT64_C( 0xCE07F766F09588D6 ),
    FLOAT64_C( 0x8000000000000000 ),
    FLOAT64_C( 0x3FCE000400000000 ),
    FLOAT64_C( 0x8313B60F0032BED8 ),
    FLOAT64_C( 0xC1EFFFFFC0002000 ),
    FLOAT64_C( 0x3FB3C75D224F2B0F ),
    FLOAT64_C( 0x7FD00000004000FF ),
    FLOAT64_C( 0xA12FFF8000001FFF ),
    FLOAT64_C( 0x3EE0000000FE0000 ),
    FLOAT64_C( 0x0010000080000004 ),
    FLOAT64_C( 0x41CFFFFE00000020 ),
    FLOAT64_C( 0x40303FFFFFFFFFFD ),
    FLOAT64_C( 0x3FD000003FEFFFFF ),
    FLOAT64_C( 0xBFD0000010000000 ),
    FLOAT64_C( 0xB7FC6B5C16CA55CF ),
    FLOAT64_C( 0x413EEB940B9D1301 ),
    FLOAT64_C( 0xC7E00200001FFFFF ),
    FLOAT64_C( 0x47F00021FFFFFFFE ),
    FLOAT64_C( 0xBFFFFFFFF80000FF ),
    FLOAT64_C( 0xC07FFFFFE00FFFFF ),
    FLOAT64_C( 0x001497A63740C5E8 ),
    FLOAT64_C( 0xC4BFFFE0001FFFFF ),
    FLOAT64_C( 0x96FFDFFEFFFFFFFF ),
    FLOAT64_C( 0x403FC000000001FE ),
    FLOAT64_C( 0xFFD00000000001F6 ),
    FLOAT64_C( 0x0640400002000000 ),
    FLOAT64_C( 0x479CEE1E4F789FE0 ),
    FLOAT64_C( 0xC237FFFFFFFFFDFE ),
};

template<>
const std::array<float128, NUM_INPUTS> test_data<float128>::values =
{
    FLOAT128_C( 0x3FDA200000100000, 0x0000000000000000 ),
    FLOAT128_C( 0x3FFF000000000000, 0x0000000000000000 ),
    FLOAT128_C( 0x85F14776190C8306, 0xD8715F4E3D54BB92 ),
    FLOAT128_C( 0xF2B00000007FFFFF, 0xFFFFFFFFFFF7FFFF ),
    FLOAT128_C( 0x8000000000000000, 0x0000000000000000 ),
    FLOAT128_C( 0xBFFFFFFFFFE00000, 0x0000008000000000 ),
    FLOAT128_C( 0x407F1719CE722F3E, 0xDA6B3FE5FF29425B ),
    FLOAT128_C( 0x43FFFF8000000000, 0x0000000000400000 ),
    FLOAT128_C( 0x401E000000000100, 0x0000000000002000 ),
    FLOAT128_C( 0x3FFED71DACDA8E47, 0x4860E3C75D224F28 ),
    FLOAT128_C( 0xBF7ECFC1E90647D1, 0x7A124FE55623EE44 ),
    FLOAT128_C( 0x0DF7007FFFFFFFFF, 0xFFFFFFFFEFFFFFFF ),
    FLOAT128_C( 0x3FE5FFEFFFFFFFFF, 0xFFFFFFFFFFFFEFFF ),
    FLOAT128_C( 0x403FFFFFFFFFFFFF, 0xFFFFFFFFFFFFFBFE ),
    FLOAT128_C( 0xBFFB2FBF7399AFEB, 0xA459EE6A5C16CA55 ),
    FLOAT128_C( 0xBDB8FFFFFFFFFFFC, 0x0000000000000400 ),
    FLOAT128_C( 0x3FC8FFDFFFFFFFFF, 0xFFFFFFFFF0000000 ),
    FLOAT128_C( 0x3FFBFFFFFFDFFFFF, 0xFFF8000000000000 ),
    FLOAT128_C( 0x407043C11737BE84, 0xDDD58212ADC937F4 ),
    FLOAT128_C( 0x8001000000000000, 0x0000001000000001 ),
    FLOAT128_C( 0xC036FFFFFFFFFFFF, 0xFE40000000000000 ),
    FLOAT128_C( 0x4002FFFFFE000002, 0x0000000000000000 ),
    FLOAT128_C( 0x4000C3FEDE897773, 0x326AC4FD8EFBE6DC ),
    FLOAT128_C( 0xBFFF0000000FFFFF, 0xFFFFFE0000000000 ),
    FLOAT128_C( 0x62C3E502146E426D, 0x43F3CAA0DC7DF1A0 ),
    FLOAT128_C( 0xB5CBD32E52BB570E, 0xBCC477CB11C6236C ),
    FLOAT128_C( 0xE228FFFFFFC00000, 0x0000000000000000 ),
    FLOAT128_C( 0x3F80000000000000, 0x0000000080000008 ),
    FLOAT128_C( 0xC1AFFFDFFFFFFFFF, 0xFFFC000000000000 ),
    FLOAT128_C( 0xC96F000000000000, 0x00000001FFFBFFFF ),
    FLOAT128_C( 0x3DE09BFE7923A338, 0xBCC8FBBD7CEC1F4F ),
    FLOAT128_C( 0x401CFFFFFFFFFFFF, 0xFFFFFFFEFFFFFF80 ),
};

inline uint32_t next_index(uint32_t input)
{
    return (input + 1) & (NUM_INPUTS - 1);
}

template<class T, class RESULT, class TEST_FPU>
static void
time_f1(
    RESULT (TEST_FPU::*function)( T ),
    TEST_FPU & fpu )
{
    int64_t count = 0;
    uint32_t inputNum = 0;

    TimePoint startClock = Time::now();

    do {
        for (int i = MIN_ITERATIONS; i; --i) {
            auto result = (fpu.*function)(test_data<T>::values[inputNum]);
            UNREFERENCED_PARAMETER(result);
            inputNum = next_index(inputNum);
        }
        count += MIN_ITERATIONS;
    }
    while (duration_is_enough(startClock));

    inputNum = 0;
    startClock = Time::now();

    for (int64_t i = count; i; --i) {
        auto result = (fpu.*function)(test_data<T>::values[inputNum]);
        UNREFERENCED_PARAMETER(result);
        inputNum = next_index(inputNum);
    }

    reportTime(count, duration_in_seconds(startClock));
}


template<class T, class RESULT, class TEST_FPU>
static void
time_a1(
    RESULT (TEST_FPU::*function)( T ),
    TEST_FPU & fpu )
{
    int64_t count = 0;
    uint32_t inputNum = 0;

    TimePoint startClock = Time::now();

    do {
        for (int i = MIN_ITERATIONS; i; --i) {
            auto result = (fpu.*function)(test_data<T>::values[inputNum].abs());
            UNREFERENCED_PARAMETER(result);
            inputNum = next_index(inputNum);
        }
        count += MIN_ITERATIONS;
    }
    while (duration_is_enough(startClock));

    inputNum = 0;
    startClock = Time::now();

    for (int64_t i = count; i; --i) {
        auto result = (fpu.*function)(test_data<T>::values[inputNum].abs());
        UNREFERENCED_PARAMETER(result);
        inputNum = next_index(inputNum);
    }

    reportTime(count, duration_in_seconds(startClock));
}


template<class T, class RESULT, class TEST_FPU>
static void
time_f2(
    RESULT (TEST_FPU::*function)( T, T ),
    TEST_FPU & fpu )
{
    int64_t count = 0;
    uint32_t inputNumA = 0;
    uint32_t inputNumB = 0;

    TimePoint startClock = Time::now();

    do {
        for (int i = MIN_ITERATIONS; i; --i) {
            auto result =
            (fpu.*function)(test_data<T>::values[inputNumA],
                            test_data<T>::values[inputNumB]);

            UNREFERENCED_PARAMETER(result);
            inputNumA = next_index(inputNumA);
            if (inputNumA == 0)
                inputNumB = next_index(inputNumB);
        }
        count += MIN_ITERATIONS;
    }
    while (duration_is_enough(startClock));

    inputNumA = 0;
    inputNumB = 0;
    startClock = Time::now();

    for (int64_t i = count; i; --i) {
        auto result =
        (fpu.*function)(test_data<T>::values[inputNumA],
                        test_data<T>::values[inputNumB]);

        UNREFERENCED_PARAMETER(result);
        inputNumA = next_index(inputNumA);
        if (inputNumA == 0)
            inputNumB = next_index(inputNumB);
    }

    reportTime(count, duration_in_seconds(startClock));
}


template<class T, class RESULT, class TEST_FPU>
static void
time_muladd(TEST_FPU & fpu)
{
    int64_t count = 0;
    uint32_t inputNumA = 0;
    uint32_t inputNumB = 0;
    uint32_t inputNumC = 0;

    TimePoint startClock = Time::now();

    do {
        for (int i = MIN_ITERATIONS; i; --i) {
            auto result =
            fpu.muladd(test_data<T>::values[inputNumA],
                       test_data<T>::values[inputNumB],
                       test_data<T>::values[inputNumC],
                       FLOAT_MULADD_NEGATE_NONE);

            UNREFERENCED_PARAMETER(result);

            inputNumA = next_index(inputNumA);
            if (inputNumA == 0) {
                inputNumB = next_index(inputNumB);
                if (inputNumB == 0)
                    inputNumC = next_index(inputNumC);
            }
        }
        count += MIN_ITERATIONS;
    }
    while (duration_is_enough(startClock));

    inputNumA = 0;
    inputNumB = 0;
    inputNumC = 0;
    startClock = Time::now();

    for (int64_t i = count; i; --i) {
        auto result =
        fpu.muladd(test_data<T>::values[inputNumA],
                   test_data<T>::values[inputNumB],
                   test_data<T>::values[inputNumC],
                   FLOAT_MULADD_NEGATE_NONE);

        UNREFERENCED_PARAMETER(result);

        inputNumA = next_index(inputNumA);
        if (inputNumA == 0) {
            inputNumB = next_index(inputNumB);
            if (inputNumB == 0)
                inputNumC = next_index(inputNumC);
        }
    }

    reportTime(count, duration_in_seconds(startClock));
}

template<class T, class TEST_FPU>
static void
test_fpu(void)
{
    static const RoundingModeEnum rms[] = {
        RM_RNE, RM_RTZ, RM_RDN, RM_RUP, RM_RMM,
    };

    for (RoundingModeEnum const rm : rms) {
        TEST_FPU fpu(rm);
        std::cout << "rounding " << (unsigned)rm << std::endl;
        time_f2<T,T,TEST_FPU>(&FPU::add, fpu);
        time_f2<T,T,TEST_FPU>(&FPU::sub, fpu);
        time_f2<T,T,TEST_FPU>(&FPU::mul, fpu);
        time_f2<T,T,TEST_FPU>(&FPU::div, fpu);
        //time_a1(&FPU::sqrt, fpu);
        time_muladd<T,T,TEST_FPU>(fpu);
    }
}

static void
test_timing(void)
{
    test_fpu<float32,FPU>();
    test_fpu<float64,FPU>();
    test_fpu<float128,FPU>();
}


} // namespace fpu
} // namespace postrisc
