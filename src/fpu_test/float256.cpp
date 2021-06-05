#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

static inline constexpr
uint64_t
X256(unsigned sign, int exponent)
{
     return ( uint64_t(sign) << 63 )
        | ( ( uint64_t(exponent) + f256::BIAS ) << f256::BITS_PER_HIGH_SIGNIFICAND );
}

static const uint64_t f256_nonfraction[] =
{
    X256(0,-262143), //subnormal
    X256(0,-262142),  X256(0,-262141),
    X256(0,-162101),  X256(0,-162100),
    X256(0,-16383),  X256(0,-16382),  X256(0,-16381),
    X256(0,-1025),  X256(0,-1024),  X256(0,-1023),  X256(0,-1022),
    X256(0,-129),  X256(0,-128),  X256(0,-127),  X256(0,-126),  X256(0,-113),  X256(0,-64),
    X256(0,-4),  X256(0,-3),  X256(0,-2),  X256(0,-1),  X256(0,0),  X256(0,1),  X256(0,2),  X256(0,3),  X256(0,4),
    X256(0,29),  X256(0,30),  X256(0,31),  X256(0,32),  X256(0,61),  X256(0,62),  X256(0,63),  X256(0,64),
    X256(0,113),  X256(0,127),  X256(0,128),  X256(0,129),
    X256(0,1023),  X256(0,1024),  X256(0,1025),  X256(0,16382),  X256(0,16383), X256(0,16384),
    X256(0,162100),  X256(0,162101),
    X256(0,262142), X256(0,262143),
    X256(0,262144), //infinity or NaN
    X256(1,-262143), //subnormal
    X256(1,-262142),  X256(1,-262141),
    X256(1,-162101),  X256(1,-162100),
    X256(1,-16383),  X256(1,-16382),  X256(1,-16381),
    X256(1,-1025),  X256(1,-1024),  X256(1,-1023),  X256(1,-1022),
    X256(1,-129),  X256(1,-128),  X256(1,-127),  X256(1,-126),  X256(1,-113),  X256(1,-64),
    X256(1,-4),  X256(1,-3),  X256(1,-2),  X256(1,-1),  X256(1,0),  X256(1,1),  X256(1,2),  X256(1,3),  X256(1,4),
    X256(1,29),  X256(1,30),  X256(1,31),  X256(1,32),  X256(1,61),  X256(1,62),  X256(1,63),  X256(1,64),
    X256(1,113),  X256(1,127),  X256(1,128),  X256(1,129),
    X256(1,1023),  X256(1,1024),  X256(1,1025),
    X256(1,16382),  X256(1,16383), X256(1,16384),
    X256(1,162100),  X256(1,162101),
    X256(1,262142),  X256(1,262143),
    X256(1,262144), //infinity or NaN
};

enum {
    f256_num_nonfraction = sizearray(f256_nonfraction),
    f256NumP2   = f256::BITS_PER_SIGNIFICAND * 4,
    f256_num_nonfractionP1 = f256_num_nonfraction * 4,
    f256_num_nonfractionP2 = f256_num_nonfraction * f256NumP2,
};

u256
CSequence::u256_p1(uint16_t index)
{
    u256 value;
    const uint32_t nbits = 256 - (f256::BITS_PER_EXPONENT + 1);

    int bitnum = (index >> 2) % (nbits + 1);

         if (bitnum < 1*64) {  value = u256(0,0,0, uint64_t(1) << (bitnum - 0*64) );  }
    else if (bitnum < 2*64) {  value = u256(0,0, uint64_t(1) << (bitnum - 1*64), 0);  }
    else if (bitnum < 3*64) {  value = u256(0, uint64_t(1) << (bitnum - 2*64), 0,0);  }
    else {                     value = u256( uint64_t(1) << (bitnum - 3*64), 0,0,0);  }


    if ( index & 1 ) value = -value;
    if ( index & 2 ) value = ~value;

    value.m0 &= makemask64(f256::BITS_PER_HIGH_SIGNIFICAND);
    return value;
}

void
CSequence::NextP1(f256 & z)
{
    static const struct {
        uint64_t m0, m1, m2, m3;
    } f256_p1[] = {
        { UINT64_C( 0x0000000000000000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0x0000000000000000 ), UINT64_C( 0x0000000000000000 ) },
        { UINT64_C( 0x0000000000000000 ), UINT64_C( 0x0000000000000000 ),
          UINT64_C( 0x0000000000000000 ), UINT64_C( 0x0000000000000001 ) },
        { UINT64_C( 0x00000FFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFFFF ),
          UINT64_C( 0xFFFFFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFFFF ) },
        { UINT64_C( 0x00000FFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFFFF ),
          UINT64_C( 0xFFFFFFFFFFFFFFFF ), UINT64_C( 0xFFFFFFFFFFFFFFFE ) }
    };

    unsigned sigNum = this->term1Num;
    z.m0 = f256_p1[ sigNum ].m0 | f256_nonfraction[ expNum ];
    z.m1 = f256_p1[ sigNum ].m1;
    z.m2 = f256_p1[ sigNum ].m2;
    z.m3 = f256_p1[ sigNum ].m3;
    ++sigNum;
    if ( sizearray(f256_p1) <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f256_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

void
CSequence::NextP2(f256 & z)
{
    unsigned sigNum = this->term1Num;
    z = u256_p1( sigNum );
    z.m0 = lobits(z.m0, z.BITS_PER_HIGH_SIGNIFICAND) | f256_nonfraction[ expNum ];
    ++sigNum;
    if ( f256NumP2 <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f256_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

template<>
uint64_t
CSequence::WeightExponent<f256>(void)
{
    static const struct {
        uint32_t  mask;
        uint32_t  offset;
    } InfWeight[] = {
        { 0x7FFFF, 0x00000 },
        { 0x7FFFF, 0x00000 },
        { 0x3FFFF, 0x20000 },
        { 0x1FFFF, 0x30000 },
        { 0x07FFF, 0x38000 },
        { 0x07FFF, 0x3C000 },
        { 0x03FFF, 0x3E000 },
        { 0x01FFF, 0x3F000 },
        { 0x00FFF, 0x3F800 },
        { 0x007FF, 0x3FC00 },
        { 0x003FF, 0x3FE00 },
        { 0x001FF, 0x3FF00 },
        { 0x000FF, 0x3FF80 },
        { 0x0007F, 0x3FFC0 },
        { 0x0003F, 0x3FFE0 },
        { 0x0001F, 0x3FFF0 },
        { 0x0000F, 0x3FFF8 },
        { 0x00007, 0x3FFFC },
    };

    const size_t n = random8() % sizearray(InfWeight);
    const uint64_t exponent = (random32() & InfWeight[n].mask) + InfWeight[n].offset;
    return exponent << 32;
}

void
CSequence::Random(f256 & z)
{
    uint64_t nonfraction;

    switch ( random8() & 7 )
    {
        case 0:
        case 1:
        case 2:
            z = u256_p1() + u256_p1();
            nonfraction = f256_nonfraction[ random8() % sizearray(f256_nonfraction) ];
            z.m0 = nonfraction | lobits( z.m0, z.BITS_PER_HIGH_SIGNIFICAND);
            break;
        case 3:
            z.m3 = random64();
            z.m2 = random64();
            z.m1 = random64();
            nonfraction = f256_nonfraction[ random8() % sizearray(f256_nonfraction) ];
            z.m0 = nonfraction | lobits( random64(), z.BITS_PER_HIGH_SIGNIFICAND);
            break;
        case 4:
        case 5:
        case 6:
            z = u256_p1() + u256_p1();
            z.m0 = make_sign<uint64_t>() | WeightExponent<f256>() | lobits( z.m0, z.BITS_PER_HIGH_SIGNIFICAND);
            break;
        case 7:
            z.m3 = random64();
            z.m2 = random64();
            z.m1 = random64();
            z.m0 = make_sign<uint64_t>() | WeightExponent<f256>() | lobits( random64(), z.BITS_PER_HIGH_SIGNIFICAND);
            break;
    }
}

size_t
CSequence::GetTotal(int level, f256 &)
{
    return level == 1 ? 3 * f256_num_nonfractionP1 : 2 * f256_num_nonfractionP2;
}

size_t
CSequence::GetTotal(int level, f256 &, f256 &)
{
     return level == 1 ? UINT64_C(6) * f256_num_nonfractionP1 * f256_num_nonfractionP1 :
                         UINT64_C(2) * f256_num_nonfractionP2 * f256_num_nonfractionP2;
}

} // namespace fpu
} // namespace postrisc
