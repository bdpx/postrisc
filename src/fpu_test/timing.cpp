#include <unistd.h>   // for isatty
//#include <mpfr.h>     // for mpfr_fma
#include <chrono>      // for std::chrono::steady_clock/duration/etc

#include "util/common.hpp"
#include "float.hpp"
#include "floatx.hpp"
#include "controller.hpp"
#include "hardfloat.hpp"
#include "functions.hpp"

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

    const  std::chrono::duration<double> diff = end - start;
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

void
CTestController::reportTime(int64_t count, double seconds)
{
    const double mops = (count / seconds) * 1.0e-6;
    std::cout << std::fixed << std::setprecision(6) << std::setw(10) << mops << " Mops/s: ";
    writeFunctionName(std::cout);
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
const std::array<f32, NUM_INPUTS> test_data<f32>::values =
{
    f32( UINT32_C( 0x4EFA0000 ) ),
    f32( UINT32_C( 0xC1D0B328 ) ),
    f32( UINT32_C( 0x80000000 ) ),
    f32( UINT32_C( 0x3E69A31E ) ),
    f32( UINT32_C( 0xAF803EFF ) ),
    f32( UINT32_C( 0x3F800000 ) ),
    f32( UINT32_C( 0x17BF8000 ) ),
    f32( UINT32_C( 0xE74A301A ) ),
    f32( UINT32_C( 0x4E010003 ) ),
    f32( UINT32_C( 0x7EE3C75D ) ),
    f32( UINT32_C( 0xBD803FE0 ) ),
    f32( UINT32_C( 0xBFFEFF00 ) ),
    f32( UINT32_C( 0x7981F800 ) ),
    f32( UINT32_C( 0x431FFFFC ) ),
    f32( UINT32_C( 0xC100C000 ) ),
    f32( UINT32_C( 0x3D87EFFF ) ),
    f32( UINT32_C( 0x4103FEFE ) ),
    f32( UINT32_C( 0xBC000007 ) ),
    f32( UINT32_C( 0xBF01F7FF ) ),
    f32( UINT32_C( 0x4E6C6B5C ) ),
    f32( UINT32_C( 0xC187FFFE ) ),
    f32( UINT32_C( 0xC58B9F13 ) ),
    f32( UINT32_C( 0x4F88007F ) ),
    f32( UINT32_C( 0xDF004007 ) ),
    f32( UINT32_C( 0xB7FFD7FE ) ),
    f32( UINT32_C( 0x7E8001FB ) ),
    f32( UINT32_C( 0x46EFFBFF ) ),
    f32( UINT32_C( 0x31C10000 ) ),
    f32( UINT32_C( 0xDB428661 ) ),
    f32( UINT32_C( 0x33F89B1F ) ),
    f32( UINT32_C( 0xA3BFEFFF ) ),
    f32( UINT32_C( 0x537BFFBE ) )
};

template<>
const std::array<f64, NUM_INPUTS> test_data<f64>::values =
{
    f64( UINT64_C( 0x422FFFC008000000 ) ),
    f64( UINT64_C( 0xB7E0000480000000 ) ),
    f64( UINT64_C( 0xF3FD2546120B7935 ) ),
    f64( UINT64_C( 0x3FF0000000000000 ) ),
    f64( UINT64_C( 0xCE07F766F09588D6 ) ),
    f64( UINT64_C( 0x8000000000000000 ) ),
    f64( UINT64_C( 0x3FCE000400000000 ) ),
    f64( UINT64_C( 0x8313B60F0032BED8 ) ),
    f64( UINT64_C( 0xC1EFFFFFC0002000 ) ),
    f64( UINT64_C( 0x3FB3C75D224F2B0F ) ),
    f64( UINT64_C( 0x7FD00000004000FF ) ),
    f64( UINT64_C( 0xA12FFF8000001FFF ) ),
    f64( UINT64_C( 0x3EE0000000FE0000 ) ),
    f64( UINT64_C( 0x0010000080000004 ) ),
    f64( UINT64_C( 0x41CFFFFE00000020 ) ),
    f64( UINT64_C( 0x40303FFFFFFFFFFD ) ),
    f64( UINT64_C( 0x3FD000003FEFFFFF ) ),
    f64( UINT64_C( 0xBFD0000010000000 ) ),
    f64( UINT64_C( 0xB7FC6B5C16CA55CF ) ),
    f64( UINT64_C( 0x413EEB940B9D1301 ) ),
    f64( UINT64_C( 0xC7E00200001FFFFF ) ),
    f64( UINT64_C( 0x47F00021FFFFFFFE ) ),
    f64( UINT64_C( 0xBFFFFFFFF80000FF ) ),
    f64( UINT64_C( 0xC07FFFFFE00FFFFF ) ),
    f64( UINT64_C( 0x001497A63740C5E8 ) ),
    f64( UINT64_C( 0xC4BFFFE0001FFFFF ) ),
    f64( UINT64_C( 0x96FFDFFEFFFFFFFF ) ),
    f64( UINT64_C( 0x403FC000000001FE ) ),
    f64( UINT64_C( 0xFFD00000000001F6 ) ),
    f64( UINT64_C( 0x0640400002000000 ) ),
    f64( UINT64_C( 0x479CEE1E4F789FE0 ) ),
    f64( UINT64_C( 0xC237FFFFFFFFFDFE ) )
};

template<>
const std::array<f80, NUM_INPUTS> test_data<f80>::values =
{
    f80( UINT16_C( 0xC03F ), UINT64_C( 0xA9BE15A19C1E8B62 ) ),
    f80( UINT16_C( 0x8000 ), UINT64_C( 0x0000000000000000 ) ),
    f80( UINT16_C( 0x75A8 ), UINT64_C( 0xE59591E4788957A5 ) ),
    f80( UINT16_C( 0xBFFF ), UINT64_C( 0xFFF0000000000040 ) ),
    f80( UINT16_C( 0x0CD8 ), UINT64_C( 0xFC000000000007FE ) ),
    f80( UINT16_C( 0x43BA ), UINT64_C( 0x99A4000000000000 ) ),
    f80( UINT16_C( 0x3FFF ), UINT64_C( 0x8000000000000000 ) ),
    f80( UINT16_C( 0x4081 ), UINT64_C( 0x94FBF1BCEB5545F0 ) ),
    f80( UINT16_C( 0x403E ), UINT64_C( 0xFFF0000000002000 ) ),
    f80( UINT16_C( 0x3FFE ), UINT64_C( 0xC860E3C75D224F28 ) ),
    f80( UINT16_C( 0x407E ), UINT64_C( 0xFC00000FFFFFFFFE ) ),
    f80( UINT16_C( 0x737A ), UINT64_C( 0x800000007FFDFFFE ) ),
    f80( UINT16_C( 0x4044 ), UINT64_C( 0xFFFFFF80000FFFFF ) ),
    f80( UINT16_C( 0xBBFE ), UINT64_C( 0x8000040000001FFE ) ),
    f80( UINT16_C( 0xC002 ), UINT64_C( 0xFF80000000000020 ) ),
    f80( UINT16_C( 0xDE8D ), UINT64_C( 0xFFFFFFFFFFE00004 ) ),
    f80( UINT16_C( 0xC004 ), UINT64_C( 0x8000000000003FFB ) ),
    f80( UINT16_C( 0x407F ), UINT64_C( 0x800000000003FFFE ) ),
    f80( UINT16_C( 0xC000 ), UINT64_C( 0xA459EE6A5C16CA55 ) ),
    f80( UINT16_C( 0x8003 ), UINT64_C( 0xC42CBF7399AEEB94 ) ),
    f80( UINT16_C( 0xBF7F ), UINT64_C( 0xF800000000000006 ) ),
    f80( UINT16_C( 0xC07F ), UINT64_C( 0xBF56BE8871F28FEA ) ),
    f80( UINT16_C( 0xC07E ), UINT64_C( 0xFFFF77FFFFFFFFFE ) ),
    f80( UINT16_C( 0xADC9 ), UINT64_C( 0x8000000FFFFFFFDE ) ),
    f80( UINT16_C( 0xC001 ), UINT64_C( 0xEFF7FFFFFFFFFFFF ) ),
    f80( UINT16_C( 0x4001 ), UINT64_C( 0xBE84F30125C497A6 ) ),
    f80( UINT16_C( 0xC06B ), UINT64_C( 0xEFFFFFFFFFFFFFFF ) ),
    f80( UINT16_C( 0x4080 ), UINT64_C( 0xFFFFFFFFBFFFFFFF ) ),
    f80( UINT16_C( 0x87E9 ), UINT64_C( 0x81FFFFFFFFFFFBFF ) ),
    f80( UINT16_C( 0xA63F ), UINT64_C( 0x801FFFFFFEFFFFFE ) ),
    f80( UINT16_C( 0x403C ), UINT64_C( 0x801FFFFFFFF7FFFF ) ),
    f80( UINT16_C( 0x4018 ), UINT64_C( 0x8000000000080003 ) )
};

template<>
const std::array<f128, NUM_INPUTS> test_data<f128>::values =
{
    f128( UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ) ),
    f128( UINT64_C( 0x3FFF000000000000 ), UINT64_C( 0x0000000000000000 ) ),
    f128( UINT64_C( 0x85F14776190C8306 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f128( UINT64_C( 0xF2B00000007FFFFF ), UINT64_C( 0xFFFFFFFFFFF7FFFF ) ),
    f128( UINT64_C( 0x8000000000000000 ), UINT64_C( 0x0000000000000000 ) ),
    f128( UINT64_C( 0xBFFFFFFFFFE00000 ), UINT64_C( 0x0000008000000000 ) ),
    f128( UINT64_C( 0x407F1719CE722F3E ), UINT64_C( 0xDA6B3FE5FF29425B ) ),
    f128( UINT64_C( 0x43FFFF8000000000 ), UINT64_C( 0x0000000000400000 ) ),
    f128( UINT64_C( 0x401E000000000100 ), UINT64_C( 0x0000000000002000 ) ),
    f128( UINT64_C( 0x3FFED71DACDA8E47 ), UINT64_C( 0x4860E3C75D224F28 ) ),
    f128( UINT64_C( 0xBF7ECFC1E90647D1 ), UINT64_C( 0x7A124FE55623EE44 ) ),
    f128( UINT64_C( 0x0DF7007FFFFFFFFF ), UINT64_C( 0xFFFFFFFFEFFFFFFF ) ),
    f128( UINT64_C( 0x3FE5FFEFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFEFFF ) ),
    f128( UINT64_C( 0x403FFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFBFE ) ),
    f128( UINT64_C( 0xBFFB2FBF7399AFEB ), UINT64_C( 0xA459EE6A5C16CA55 ) ),
    f128( UINT64_C( 0xBDB8FFFFFFFFFFFC ), UINT64_C( 0x0000000000000400 ) ),
    f128( UINT64_C( 0x3FC8FFDFFFFFFFFF ), UINT64_C( 0xFFFFFFFFF0000000 ) ),
    f128( UINT64_C( 0x3FFBFFFFFFDFFFFF ), UINT64_C( 0xFFF8000000000000 ) ),
    f128( UINT64_C( 0x407043C11737BE84 ), UINT64_C( 0xDDD58212ADC937F4 ) ),
    f128( UINT64_C( 0x8001000000000000 ), UINT64_C( 0x0000001000000001 ) ),
    f128( UINT64_C( 0xC036FFFFFFFFFFFF ), UINT64_C( 0xFE40000000000000 ) ),
    f128( UINT64_C( 0x4002FFFFFE000002 ), UINT64_C( 0x0000000000000000 ) ),
    f128( UINT64_C( 0x4000C3FEDE897773 ), UINT64_C( 0x326AC4FD8EFBE6DC ) ),
    f128( UINT64_C( 0xBFFF0000000FFFFF ), UINT64_C( 0xFFFFFE0000000000 ) ),
    f128( UINT64_C( 0x62C3E502146E426D ), UINT64_C( 0x43F3CAA0DC7DF1A0 ) ),
    f128( UINT64_C( 0xB5CBD32E52BB570E ), UINT64_C( 0xBCC477CB11C6236C ) ),
    f128( UINT64_C( 0xE228FFFFFFC00000 ), UINT64_C( 0x0000000000000000 ) ),
    f128( UINT64_C( 0x3F80000000000000 ), UINT64_C( 0x0000000080000008 ) ),
    f128( UINT64_C( 0xC1AFFFDFFFFFFFFF ), UINT64_C( 0xFFFC000000000000 ) ),
    f128( UINT64_C( 0xC96F000000000000 ), UINT64_C( 0x00000001FFFBFFFF ) ),
    f128( UINT64_C( 0x3DE09BFE7923A338 ), UINT64_C( 0xBCC8FBBD7CEC1F4F ) ),
    f128( UINT64_C( 0x401CFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFEFFFFFF80 ) )
};

template<>
const std::array<f256, NUM_INPUTS> test_data<f256>::values =
{
    f256( UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x3FFF000000000000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x85F14776190C8306 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xF2B00000007FFFFF ), UINT64_C( 0xFFFFFFFFFFF7FFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x8000000000000000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xBFFFFFFFFFE00000 ), UINT64_C( 0x0000008000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x407F1719CE722F3E ), UINT64_C( 0xDA6B3FE5FF29425B ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x43FFFF8000000000 ), UINT64_C( 0x0000000000400000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x401E000000000100 ), UINT64_C( 0x0000000000002000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x3FFED71DACDA8E47 ), UINT64_C( 0x4860E3C75D224F28 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xBF7ECFC1E90647D1 ), UINT64_C( 0x7A124FE55623EE44 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x0DF7007FFFFFFFFF ), UINT64_C( 0xFFFFFFFFEFFFFFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x3FE5FFEFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFEFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x403FFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFBFE ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xBFFB2FBF7399AFEB ), UINT64_C( 0xA459EE6A5C16CA55 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xBDB8FFFFFFFFFFFC ), UINT64_C( 0x0000000000000400 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x3FC8FFDFFFFFFFFF ), UINT64_C( 0xFFFFFFFFF0000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x3FFBFFFFFFDFFFFF ), UINT64_C( 0xFFF8000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x407043C11737BE84 ), UINT64_C( 0xDDD58212ADC937F4 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x8001000000000000 ), UINT64_C( 0x0000001000000001 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xC036FFFFFFFFFFFF ), UINT64_C( 0xFE40000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x4002FFFFFE000002 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x4000C3FEDE897773 ), UINT64_C( 0x326AC4FD8EFBE6DC ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xBFFF0000000FFFFF ), UINT64_C( 0xFFFFFE0000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x62C3E502146E426D ), UINT64_C( 0x43F3CAA0DC7DF1A0 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xB5CBD32E52BB570E ), UINT64_C( 0xBCC477CB11C6236C ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xE228FFFFFFC00000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x3F80000000000000 ), UINT64_C( 0x0000000080000008 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xC1AFFFDFFFFFFFFF ), UINT64_C( 0xFFFC000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0xC96F000000000000 ), UINT64_C( 0x00000001FFFBFFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x3DE09BFE7923A338 ), UINT64_C( 0xBCC8FBBD7CEC1F4F ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f256( UINT64_C( 0x401CFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFEFFFFFF80 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) )
};

template<>
const std::array<f512, NUM_INPUTS> test_data<f512>::values =
{
    f512( UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x3FFF000000000000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x85F14776190C8306 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xF2B00000007FFFFF ), UINT64_C( 0xFFFFFFFFFFF7FFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x8000000000000000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xBFFFFFFFFFE00000 ), UINT64_C( 0x0000008000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x407F1719CE722F3E ), UINT64_C( 0xDA6B3FE5FF29425B ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x43FFFF8000000000 ), UINT64_C( 0x0000000000400000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x401E000000000100 ), UINT64_C( 0x0000000000002000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x3FFED71DACDA8E47 ), UINT64_C( 0x4860E3C75D224F28 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xBF7ECFC1E90647D1 ), UINT64_C( 0x7A124FE55623EE44 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x0DF7007FFFFFFFFF ), UINT64_C( 0xFFFFFFFFEFFFFFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x3FE5FFEFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFEFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x403FFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFBFE ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xBFFB2FBF7399AFEB ), UINT64_C( 0xA459EE6A5C16CA55 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xBDB8FFFFFFFFFFFC ), UINT64_C( 0x0000000000000400 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x3FC8FFDFFFFFFFFF ), UINT64_C( 0xFFFFFFFFF0000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x3FFBFFFFFFDFFFFF ), UINT64_C( 0xFFF8000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x407043C11737BE84 ), UINT64_C( 0xDDD58212ADC937F4 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x8001000000000000 ), UINT64_C( 0x0000001000000001 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xC036FFFFFFFFFFFF ), UINT64_C( 0xFE40000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x4002FFFFFE000002 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x4000C3FEDE897773 ), UINT64_C( 0x326AC4FD8EFBE6DC ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xBFFF0000000FFFFF ), UINT64_C( 0xFFFFFE0000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x62C3E502146E426D ), UINT64_C( 0x43F3CAA0DC7DF1A0 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xB5CBD32E52BB570E ), UINT64_C( 0xBCC477CB11C6236C ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xE228FFFFFFC00000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x3F80000000000000 ), UINT64_C( 0x0000000080000008 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xC1AFFFDFFFFFFFFF ), UINT64_C( 0xFFFC000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0xC96F000000000000 ), UINT64_C( 0x00000001FFFBFFFF ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x3DE09BFE7923A338 ), UINT64_C( 0xBCC8FBBD7CEC1F4F ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) ),
    f512( UINT64_C( 0x401CFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFEFFFFFF80 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ),
          UINT64_C( 0x3FDA200000100000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0xD8715F4E3D54BB92 ), UINT64_C( 0xD8715F4E3D54BB92 ) )
};


inline uint32_t next_index(uint32_t input)
{
    return (input + 1) & (NUM_INPUTS - 1);
}

template<class T, class RESULT, class TEST_FPU>
void
CTestController::time_f1(
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
void
CTestController::time_a1(
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
void
CTestController::time_f2(
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
void
CTestController::time_f3(
    RESULT (TEST_FPU::*function)( T, T, T, muladd_negate_t ),
    TEST_FPU & fpu )
{
    int64_t count = 0;
    uint32_t inputNumA = 0;
    uint32_t inputNumB = 0;
    uint32_t inputNumC = 0;

    TimePoint startClock = Time::now();

    do {
        for (int i = MIN_ITERATIONS; i; --i) {
            auto result =
            (fpu.*function)(test_data<T>::values[inputNumA],
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
        (fpu.*function)(test_data<T>::values[inputNumA],
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


BOOST_AUTO_TEST_SUITE(timing)

#define FPU_TIMING_TEST_CASE(FPU_TYPE, NumInputs, supported, T1, T2, method, hardware)           \
    BOOST_FIXTURE_TEST_CASE(time_ ## T1 ## _ ## method, FpuTestFixture)                          \
    {                                                                                            \
        const int functionCode = T1 ## _ ## method;                                              \
        const unsigned supportedMask = hardware ? SUPPORTED_HARD : SUPPORTED_SOFT;               \
        TestFunctionPtr func = [](CTestController & ctr, CTestContext context)                   \
        {                                                                                        \
            FPU_TYPE         fpu;                                                                \
            ctr.setup_fpu(fpu, context);                                                         \
            ctr.time_##NumInputs < T1, T2, FPU_TYPE > ( & FPU_TYPE::method, fpu );               \
        };                                                                                       \
        if ((SUPPORTED_##supported) & supportedMask)                                             \
        {                                                                                        \
            BOOST_CHECK_NO_THROW(ctrl.testingFunction(functionCode, func, hardware));            \
        }                                                                                        \
    }

#if 0

BOOST_AUTO_TEST_SUITE(soft)
#define FPU_SOFT_TIME_FIXTURE(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) \
    FPU_TIMING_TEST_CASE(CSoftFPU, NumInputs, supported, T1, T2, method, false)

DECLARE_FUNCTIONS_TABLE(FPU_SOFT_TIME_FIXTURE)
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(slow)
#define FPU_TESTSOFT_TIME_FIXTURE(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) \
    FPU_TIMING_TEST_CASE(CTestFPU, NumInputs, supported, T1, T2, method, false)

DECLARE_FUNCTIONS_TABLE(FPU_TESTSOFT_TIME_FIXTURE)
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(hard)
#define FPU_HARD_TIME_FIXTURE(NumInputs, Templt, supported, roundingPrecision, roundingMode, tininessMode, tininessModeAtReducedPrecision, T1, T2, method) \
    FPU_TIMING_TEST_CASE(CHardFPU, NumInputs, supported, T1, T2, method, true)

DECLARE_HARDWARE_FUNCTIONS_TABLE(FPU_HARD_TIME_FIXTURE)
BOOST_AUTO_TEST_SUITE_END()

#endif

BOOST_AUTO_TEST_SUITE_END() // timing

} // namespace fpu
} // namespace postrisc
