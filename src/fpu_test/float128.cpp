#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

static inline constexpr
uint16_t
X128(unsigned sign, int exponent)
{
    return ( uint16_t(sign) << 15 )
           | uint16_t( (exponent) + f128::BIAS );
}

static const uint16_t f128_nonfraction[] =
{
    X128(0,-16383), //subnormal
    X128(0,-16382),  X128(0,-16381),
    X128(0,-1025),  X128(0,-1024),  X128(0,-1023),  X128(0,-1022),
    X128(0,-129),  X128(0,-128),  X128(0,-127),  X128(0,-126),  X128(0,-113),  X128(0,-64),
    X128(0,-4),  X128(0,-3),  X128(0,-2),  X128(0,-1),  X128(0,0),  X128(0,1),  X128(0,2),  X128(0,3),  X128(0,4),
    X128(0,29),  X128(0,30),  X128(0,31),  X128(0,32),  X128(0,61),  X128(0,62),  X128(0,63),  X128(0,64),
    X128(0,113),  X128(0,127),  X128(0,128),  X128(0,129),
    X128(0,1023),  X128(0,1024),  X128(0,1025),  X128(0,16382),  X128(0,16383),
    X128(0,16384), //infinity or NaN
    X128(1,-16383), //subnormal
    X128(1,-16382),  X128(1,-16381),
    X128(1,-1025),  X128(1,-1024),  X128(1,-1023),  X128(1,-1022),
    X128(1,-129),  X128(1,-128),  X128(1,-127),  X128(1,-126),  X128(1,-113),  X128(1,-64),
    X128(1,-4),  X128(1,-3),  X128(1,-2),  X128(1,-1),  X128(1,0),  X128(1,1),  X128(1,2),  X128(1,3),  X128(1,4),
    X128(1,29),  X128(1,30),  X128(1,31),  X128(1,32),  X128(1,61),  X128(1,62),  X128(1,63),  X128(1,64),
    X128(1,113),  X128(1,127),  X128(1,128),  X128(1,129),
    X128(1,1023),  X128(1,1024),  X128(1,1025),
    X128(1,16382),  X128(1,16383),
    X128(1,16384) //infinity or NaN
};

enum {
    f128_num_nonfraction = sizearray(f128_nonfraction),
    f128NumP2   = f128::BITS_PER_SIGNIFICAND * 4,
    f128_num_nonfractionP1 = f128_num_nonfraction * 4,
    f128_num_nonfractionP2 = f128_num_nonfraction * f128NumP2,
};

u128
CSequence::u128_p1(uint8_t nbits, uint16_t index)
{
    int bitnum = (index >> 2) % (nbits + 1);
    const u128 one(0,1);
    u128 value = one << bitnum;

    if (index & 1) value = -value;
    if (index & 2) value = ~value;

    u128 mask = (one << nbits) - one;
    value = value & mask;
    return value;
}

void
CSequence::NextP1(f128 & z)
{
    static const struct {
        uint64_t  high;
        uint64_t  low;
    } f128P1[] = {
        { UINT64_C( 0x0000000000000000 ), UINT64_C( 0x0000000000000000 ) },
        { UINT64_C( 0x0000000000000000 ), UINT64_C( 0x0000000000000001 ) },
        { UINT64_C( 0x0000FFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFFFF ) },
        { UINT64_C( 0x0000FFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFFFE ) }
    };

    unsigned sigNum = this->term1Num;
    z.high = f128P1[ sigNum ].high;
    z.low  = f128P1[ sigNum ].low;
    z.high |= uint64_t(f128_nonfraction[ expNum ]) << z.BITS_PER_HIGH_SIGNIFICAND;
    ++sigNum;

    if ( sizearray(f128P1) <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f128_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

void
CSequence::NextP2(f128 & z)
{
    unsigned sigNum = this->term1Num;
    u128 frac = u128_p1(112, sigNum);
    frac.m0 |= uint64_t(f128_nonfraction[ expNum ]) << z.BITS_PER_HIGH_SIGNIFICAND;
    z = f128(frac.m0, frac.m1);
    ++sigNum;
    if ( f128NumP2 <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f128_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

template<>
uint64_t
CSequence::WeightExponent<f128>(void)
{
    static const struct {
        uint16_t  mask;
        uint16_t  offset;
    } InfWeight[] = {
        { 0x7FFF, 0x0000 },
        { 0x7FFF, 0x0000 },
        { 0x3FFF, 0x2000 },
        { 0x1FFF, 0x3000 },
        { 0x07FF, 0x3800 },
        { 0x07FF, 0x3C00 },
        { 0x03FF, 0x3E00 },
        { 0x01FF, 0x3F00 },
        { 0x00FF, 0x3F80 },
        { 0x007F, 0x3FC0 },
        { 0x003F, 0x3FE0 },
        { 0x001F, 0x3FF0 },
        { 0x000F, 0x3FF8 },
        { 0x0007, 0x3FFC }
    };

    const size_t n = random8() % sizearray(InfWeight);
    const uint16_t exponent = (random16() & InfWeight[n].mask) + InfWeight[n].offset;
    return exponent;
}

void
CSequence::Random(f128 & z)
{
    uint16_t   nonfraction;
    u128       fraction;

    switch ( random8() & 7 ) {
        case 0:
        case 1:
        case 2:
            fraction = u128_p1(112) + u128_p1(112);
            nonfraction = f128_nonfraction[ random8() % sizearray(f128_nonfraction) ];
            z = f128( (uint64_t(nonfraction) << z.BITS_PER_HIGH_SIGNIFICAND) | lobits( fraction.m0, z.BITS_PER_HIGH_SIGNIFICAND), fraction.m1 );
            break;
        case 3:
            z.low = random64();
            nonfraction = f128_nonfraction[ random8() % sizearray(f128_nonfraction) ];
            z.high = (uint64_t(nonfraction) << z.BITS_PER_HIGH_SIGNIFICAND) | lobits( random64(), z.BITS_PER_HIGH_SIGNIFICAND);
            break;
        case 4:
        case 5:
        case 6:
            fraction = u128_p1(112) + u128_p1(112);
            z = f128( make_sign<uint64_t>()
                    | ( uint64_t( WeightExponent<f128>() ) << z.BITS_PER_HIGH_SIGNIFICAND)
                    | lobits( fraction.m0, z.BITS_PER_HIGH_SIGNIFICAND), fraction.m1 );
            break;
        case 7:
            z.low = random64();
            z.high = make_sign<uint64_t>()
                     | ( uint64_t( WeightExponent<f128>() ) << z.BITS_PER_HIGH_SIGNIFICAND)
                     | lobits( random64(), z.BITS_PER_HIGH_SIGNIFICAND);
            break;
    }
}

size_t
CSequence::GetTotal(int level, f128 &)
{
    return level == 1 ? 3 * f128_num_nonfractionP1 : 2 * f128_num_nonfractionP2;
}

size_t
CSequence::GetTotal(int level, f128 &, f128 &)
{
    return level == 1 ? UINT64_C(6) * f128_num_nonfractionP1 * f128_num_nonfractionP1 :
                        UINT64_C(2) * f128_num_nonfractionP2 * f128_num_nonfractionP2;
}

} // namespace fpu
} // namespace postrisc
