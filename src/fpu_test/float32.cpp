#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

static inline constexpr
u32
X32(unsigned sign, int exponent)
{
    return ( u32(sign) << 31 )
       | ( ( u32(exponent) + f32::BIAS ) << f32::BITS_PER_SIGNIFICAND );
}

static const u32 f32_nonfraction[] =
{
    X32(0,-127), //subnormal
    X32(0,-126),  X32(0,-125),  X32(0,-24),
    X32(0,-4),  X32(0,-3),  X32(0,-2),  X32(0,-1),  X32(0,0),  X32(0,1),  X32(0,2),  X32(0,3),  X32(0,4),
    X32(0,24),  X32(0,29),  X32(0,30),  X32(0,31),  X32(0,32),
    X32(0,61),  X32(0,62),  X32(0,63),  X32(0,64),
    X32(0,126),  X32(0,127),
    X32(0,128), //infinity or NaN
    X32(1,-127), //subnormal
    X32(1,-126),  X32(1,-125),  X32(1,-24),
    X32(1,-4),  X32(1,-3),  X32(1,-2),  X32(1,-1),  X32(1,0),  X32(1,1),  X32(1,2),  X32(1,3),  X32(1,4),
    X32(1,24),  X32(1,29),  X32(1,30),  X32(1,31),  X32(1,32),
    X32(1,61),  X32(1,62),  X32(1,63),  X32(1,64),
    X32(1,126),  X32(1,127),
    X32(1,128) //infinity or NaN
};

enum {
    f32_num_nonfraction   = sizearray(f32_nonfraction),
    f32NumP2              = f32::BITS_PER_SIGNIFICAND * 4,
    f32_num_nonfractionP1 = f32_num_nonfraction * 4,
    f32_num_nonfractionP2 = f32_num_nonfraction * f32NumP2,
};

void
TestSequencer::NextP1(f32 & z)
{
    static const u32 f32P1[] = {
        0x00000000,
        0x00000001,
        0x007FFFFF,
        0x007FFFFE
    };

    unsigned sigNum = this->term1Num;
    z = f32::make( f32_nonfraction[ expNum ] | f32P1[ sigNum ] );
    ++sigNum;
    if ( sizearray(f32P1) <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f32_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

void
TestSequencer::NextP2(f32 & z)
{
    unsigned sigNum = this->term1Num;
    z = f32::make( f32_nonfraction[ expNum ] | u32_p1(z.BITS_PER_SIGNIFICAND, sigNum) );
    ++sigNum;
    if ( f32NumP2 <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f32_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

template<>
u64
TestSequencer::WeightExponent<f32>(void)
{
    static const struct {
        u32  mask;
        u32  offset;
    } InfWeight[] = {
       { 0x7F800000, 0x00000000 },
       { 0x7F800000, 0x00000000 },
       { 0x3F800000, 0x20000000 },
       { 0x1F800000, 0x30000000 },
       { 0x0F800000, 0x38000000 },
       { 0x07800000, 0x3C000000 },
       { 0x03800000, 0x3E000000 }
    };

    u32 exponent =  u32( random16() & 0x1FF ) << f32::BITS_PER_SIGNIFICAND;

    const size_t n = random8() % sizearray(InfWeight);
    exponent = ( exponent & InfWeight[n].mask ) + InfWeight[n].offset;
    return exponent;
}

void
TestSequencer::Random(f32 & z)
{
    u32 nonfraction, sign, fraction, exponent;

    switch ( random8() & 7 ) {
        case 0:
        case 1:
        case 2:
            fraction = lobits( u32_p1(z.BITS_PER_SIGNIFICAND) + u32_p1(z.BITS_PER_SIGNIFICAND), z.BITS_PER_SIGNIFICAND);
            nonfraction = f32_nonfraction[ random8() % sizearray(f32_nonfraction) ];
            z = f32::make( nonfraction | fraction );
            break;
        case 3:
            fraction = lobits( random32(), z.BITS_PER_SIGNIFICAND);
            nonfraction = f32_nonfraction[ random8() % sizearray(f32_nonfraction) ];
            z = f32::make( nonfraction | fraction );
            break;
        case 4:
        case 5:
        case 6:
            sign     =  make_sign<u32>();
            fraction =  lobits( u32_p1(z.BITS_PER_SIGNIFICAND) + u32_p1(z.BITS_PER_SIGNIFICAND), z.BITS_PER_SIGNIFICAND);
            exponent =  WeightExponent<f32>();
            z = f32::make( sign | exponent | fraction );
            break;
        case 7:
            sign     =  make_sign<u32>();
            fraction =  lobits(random32(), z.BITS_PER_SIGNIFICAND);
            exponent =  WeightExponent<f32>();
            z = f32::make( sign | exponent | fraction );
            break;
    }
}

size_t
TestSequencer::GetTotal(int level, f32 &)
{
    return level == 1 ? 3 * f32_num_nonfractionP1 : 2 * f32_num_nonfractionP2;
}

size_t
TestSequencer::GetTotal(int level, f32 &, f32 &)
{
     return level == 1 ? 6 * f32_num_nonfractionP1 * f32_num_nonfractionP1 :
                         2 * f32_num_nonfractionP2 * f32_num_nonfractionP2;
}

} // namespace fpu
} // namespace postrisc
