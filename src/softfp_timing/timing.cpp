#include <boost/test/unit_test.hpp>

#include <chrono>      // for std::chrono::steady_clock/duration/etc

#include "util/common.hpp"
#include "emulator/SoftFpu.hpp"

namespace postrisc {

/****************************************************************************************
* performance tests templates
****************************************************************************************/
enum {
    NUM_INPUTS = 32,
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
    const double duration_threshold = 0.25; // seconds
    return duration_in_seconds(start) < duration_threshold;
}

static void
reportTime(i64 count, double seconds, const std::string& name)
{
    const double mops = (count / seconds) * 1.0e-6;
    std::cout << std::fixed << std::setprecision(6)
              << std::setw(10) << mops << " Mops/s: "
              << name << std::endl;
    // std::cout << " rounding " << roundingModeName;
}

template<typename T> class test_data
{
public:
    static const std::array<T, NUM_INPUTS> values;
};

template<>
const std::array<i32, NUM_INPUTS> test_data<i32>::values =
{
    i32( I32C( 0xFFFFBB79 )), i32( I32C( 0x405CF80F )),
    i32( I32C( 0x00000000 )), i32( I32C( 0xFFFFFD04 )),
    i32( I32C( 0xFFF20002 )), i32( I32C( 0x0C8EF795 )),
    i32( I32C( 0xF00011FF )), i32( I32C( 0x000006CA )),
    i32( I32C( 0x00009BFE )), i32( I32C( 0xFF4862E3 )),
    i32( I32C( 0x9FFFEFFE )), i32( I32C( 0xFFFFFFB7 )),
    i32( I32C( 0x0BFF7FFF )), i32( I32C( 0x0000F37A )),
    i32( I32C( 0x0011DFFE )), i32( I32C( 0x00000006 )),
    i32( I32C( 0xFFF02006 )), i32( I32C( 0xFFFFF7D1 )),
    i32( I32C( 0x10200003 )), i32( I32C( 0xDE8DF765 )),
    i32( I32C( 0x00003E02 )), i32( I32C( 0x000019E8 )),
    i32( I32C( 0x0008FFFE )), i32( I32C( 0xFFFFFB5C )),
    i32( I32C( 0xFFDF7FFE )), i32( I32C( 0x07C42FBF )),
    i32( I32C( 0x0FFFE3FF )), i32( I32C( 0x040B9F13 )),
    i32( I32C( 0xBFFFFFF8 )), i32( I32C( 0x0001BF56 )),
    i32( I32C( 0x000017F6 )), i32( I32C( 0x000A908A ))
};

template<>
const std::array<i64, NUM_INPUTS> test_data<i64>::values =
{
    i64( I64C( 0xFBFFC3FFFFFFFFFF )),
    i64( I64C( 0x0000000003C589BC )),
    i64( I64C( 0x00000000400013FE )),
    i64( I64C( 0x0000000000186171 )),
    i64( I64C( 0xFFFFFFFFFFFEFBFA )),
    i64( I64C( 0xFFFFFD79E6DFFC73 )),
    i64( I64C( 0x0000000010001DFF )),
    i64( I64C( 0xDD1A0F0C78513710 )),
    i64( I64C( 0xFFFF83FFFFFEFFFE )),
    i64( I64C( 0x00756EBD1AD0C1C7 )),
    i64( I64C( 0x0003FDFFFFFFFFBE )),
    i64( I64C( 0x0007D0FB2C2CA951 )),
    i64( I64C( 0x0007FC0007FFFFFE )),
    i64( I64C( 0x0000001F942B18BB )),
    i64( I64C( 0x0000080101FFFFFE )),
    i64( I64C( 0xFFFFFFFFFFFF0978 )),
    i64( I64C( 0x000000000008BFFF )),
    i64( I64C( 0x0000000006F5AF08 )),
    i64( I64C( 0xFFDEFF7FFFFFFFFE )),
    i64( I64C( 0x0000000000000003 )),
    i64( I64C( 0x3FFFFFFFFF80007D )),
    i64( I64C( 0x0000000000000078 )),
    i64( I64C( 0xFFF80000007FDFFD )),
    i64( I64C( 0x1BBC775B78016AB0 )),
    i64( I64C( 0xFFF9001FFFFFFFFE )),
    i64( I64C( 0xFFFD4767AB98E43F )),
    i64( I64C( 0xFFFFFEFFFE00001E )),
    i64( I64C( 0xFFFFFFFFFFF04EFD )),
    i64( I64C( 0x07FFFFFFFFFFF7FF )),
    i64( I64C( 0xFFFC9EAA38F89050 )),
    i64( I64C( 0x00000020FBFFFFFE )),
    i64( I64C( 0x0000099AE6455357 ))
};

template<>
const std::array<fpu::f16, NUM_INPUTS> test_data<fpu::f16>::values =
{
    FLOAT16_C( 0x4EFA ),
    FLOAT16_C( 0xC1D0 ),
    FLOAT16_C( 0x8000 ),
    FLOAT16_C( 0x3E69 ),
    FLOAT16_C( 0xAF80 ),
    FLOAT16_C( 0x3F80 ),
    FLOAT16_C( 0x17BF ),
    FLOAT16_C( 0xE74A ),
    FLOAT16_C( 0x4E01 ),
    FLOAT16_C( 0x7EE3 ),
    FLOAT16_C( 0xBD80 ),
    FLOAT16_C( 0xBFFE ),
    FLOAT16_C( 0x7981 ),
    FLOAT16_C( 0x431F ),
    FLOAT16_C( 0xC100 ),
    FLOAT16_C( 0x3D87 ),
    FLOAT16_C( 0x4103 ),
    FLOAT16_C( 0xBC00 ),
    FLOAT16_C( 0xBF01 ),
    FLOAT16_C( 0x4E6C ),
    FLOAT16_C( 0xC187 ),
    FLOAT16_C( 0xC58B ),
    FLOAT16_C( 0x4F88 ),
    FLOAT16_C( 0xDF00 ),
    FLOAT16_C( 0xB7FF ),
    FLOAT16_C( 0x7E80 ),
    FLOAT16_C( 0x46EF ),
    FLOAT16_C( 0x31C1 ),
    FLOAT16_C( 0xDB42 ),
    FLOAT16_C( 0x33F8 ),
    FLOAT16_C( 0xA3BF ),
    FLOAT16_C( 0x537B ),
};

template<>
const std::array<fpu::f32, NUM_INPUTS> test_data<fpu::f32>::values =
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
const std::array<fpu::f64, NUM_INPUTS> test_data<fpu::f64>::values =
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
const std::array<fpu::f128, NUM_INPUTS> test_data<fpu::f128>::values =
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

inline u32 next_index(u32 input)
{
    return (input + 1) & (NUM_INPUTS - 1);
}

template<typename OP, typename FPU> static void timing(const OP& op, FPU& fpu, const std::string& name)
{
    TimePoint startClock = Time::now();
    i64 count = 0;
    const i64 MIN_ITERATIONS = 500 * 1000;
    do {
        op.func(fpu, MIN_ITERATIONS);
        count += MIN_ITERATIONS;
    } while (duration_is_enough(startClock));

    startClock = Time::now();
    op.func(fpu, count);
    reportTime(count, duration_in_seconds(startClock), name);
}

template<typename OP, typename FPU> static void timing_rm(const OP& op, FPU& fpu, const std::string& name)
{
    static const struct {
        fpu::RoundingModeEnum rm;
        char name[10];
    } rms[] = {
        { fpu::RM_RNE, " neareven" },
        { fpu::RM_RTZ, " tozero" },
        { fpu::RM_RDN, " down" },
        { fpu::RM_RUP, " up" },
        { fpu::RM_RMM, " maxmag" },
    };

    for (auto const& rm : rms) {
        fpu.set_rounding_mode(rm.rm);
        timing(op, fpu, name+rm.name);
    }
}

template<typename T, typename FPU, typename OP> class op_f1 {
public:
    void func(FPU& fpu, i64 count) const {
        u32 inputNum = 0;
        for (i64 i = count; i != 0; --i) {
            volatile auto result = OP::template func<T,FPU>(fpu,
                test_data<T>::values[inputNum]);
            UNREFERENCED_PARAMETER(result);
            inputNum = next_index(inputNum);
        }
    }
};

template<typename T, typename FPU, typename OP> class op_f2 {
public:
    void func(FPU& fpu, i64 count) const {
        u32 inputNumA = 0;
        u32 inputNumB = 0;
        for (i64 i = count; i != 0; --i) {
            volatile auto result = OP::template func<T,FPU>(fpu,
                test_data<T>::values[inputNumA], test_data<T>::values[inputNumB]);
            UNREFERENCED_PARAMETER(result);
            inputNumA = next_index(inputNumA);
            if (inputNumA == 0)
                inputNumB = next_index(inputNumB);
        }
    }
};

template<typename T, typename FPU, typename OP> class op_f3 {
public:
    void func(FPU& fpu, i64 count) const {
        u32 inputNumA = 0;
        u32 inputNumB = 0;
        u32 inputNumC = 0;
        for (i64 i = count; i != 0; --i) {
            volatile auto result = OP::template func<T,FPU>(fpu,
                test_data<T>::values[inputNumA], test_data<T>::values[inputNumB], test_data<T>::values[inputNumC]);
            UNREFERENCED_PARAMETER(result);
            inputNumA = next_index(inputNumA);
            if (inputNumA == 0) {
                inputNumB = next_index(inputNumB);
                if (inputNumB == 0)
                    inputNumC = next_index(inputNumC);
            }
        }
    }
};

class fadd    { public: template<typename T, typename FPU> static T func(FPU& fpu, T a, T b     ) { return fpu.add(a, b); }};
class fsub    { public: template<typename T, typename FPU> static T func(FPU& fpu, T a, T b     ) { return fpu.sub(a, b); }};
class fmul    { public: template<typename T, typename FPU> static T func(FPU& fpu, T a, T b     ) { return fpu.mul(a, b); }};
class fdiv    { public: template<typename T, typename FPU> static T func(FPU& fpu, T a, T b     ) { return fpu.div(a, b); }};
class fmuladd { public: template<typename T, typename FPU> static T func(FPU& fpu, T a, T b, T c) { return fpu.muladd(a, b, c, fpu::FLOAT_MULADD_NEGATE_NONE); }};

template<typename T, typename FPU, typename OP> static void test_f2(const std::string& name)
{
    FPU fpu;
    op_f2<T, FPU, OP> op;
    timing_rm(op, fpu, name);
}

template<typename T, typename FPU, typename OP> static void test_f3(const std::string& name)
{
    FPU fpu;
    op_f3<T, FPU, OP> op;
    timing_rm(op, fpu, name);
}

template<typename FPU, typename OP> static void test_f2_timing(const std::string& name)
{
    test_f2<fpu::f16,  FPU, OP>(name+":f16");
    test_f2<fpu::f32,  FPU, OP>(name+":f32");
    test_f2<fpu::f64,  FPU, OP>(name+":f64");
    test_f2<fpu::f128, FPU, OP>(name+":f128");
}

template<typename FPU, typename OP> static void test_f3_timing(const std::string& name)
{
    test_f3<fpu::f16,  FPU, OP>(name+":f16");
    test_f3<fpu::f32,  FPU, OP>(name+":f32");
    test_f3<fpu::f64,  FPU, OP>(name+":f64");
    test_f3<fpu::f128, FPU, OP>(name+":f128");
}

BOOST_AUTO_TEST_SUITE(test_timing)

BOOST_AUTO_TEST_CASE(timing_add) { test_f2_timing<fpu::SoftFPU, fadd>("add"); }
BOOST_AUTO_TEST_CASE(timing_sub) { test_f2_timing<fpu::SoftFPU, fsub>("sub"); }
BOOST_AUTO_TEST_CASE(timing_mul) { test_f2_timing<fpu::SoftFPU, fmul>("mul"); }
BOOST_AUTO_TEST_CASE(timing_div) { test_f2_timing<fpu::SoftFPU, fdiv>("div"); }
BOOST_AUTO_TEST_CASE(timing_muladd) { test_f3_timing<fpu::SoftFPU, fmuladd>("muladd"); }

BOOST_AUTO_TEST_SUITE_END() // timing

} // namespace postrisc
