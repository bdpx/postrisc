#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

static inline constexpr
u64
X512(unsigned sign, int exponent)
{
     return ( u64(sign) << 63 )
        | ( ( u64(exponent) + f512::BIAS ) << f512::BITS_PER_HIGH_SIGNIFICAND );
}

static const u64 f512_nonfraction[] =
{
    X512(0, - (1 << 22) + 1),
    X512(0,-262143), //subnormal
    X512(0,-262142),  X512(0,-262141),
    X512(0,-162101),  X512(0,-162100),
    X512(0,-16383),  X512(0,-16382),  X512(0,-16381),
    X512(0,-1025),  X512(0,-1024),  X512(0,-1023),  X512(0,-1022),
    X512(0,-129),  X512(0,-128),  X512(0,-127),  X512(0,-126),  X512(0,-113),  X512(0,-64),
    X512(0,-4),  X512(0,-3),  X512(0,-2),  X512(0,-1),  X512(0,0),  X512(0,1),  X512(0,2),  X512(0,3),  X512(0,4),
    X512(0,29),  X512(0,30),  X512(0,31),  X512(0,32),  X512(0,61),  X512(0,62),  X512(0,63),  X512(0,64),
    X512(0,113),  X512(0,127),  X512(0,128),  X512(0,129),
    X512(0,1023),  X512(0,1024),  X512(0,1025),  X512(0,16382),  X512(0,16383), X512(0,16384),
    X512(0,162100),  X512(0,162101),
    X512(0,262142), X512(0,262143),
    X512(0,262144), //infinity or NaN
    X512(1, - (1 << 22) + 1),
    X512(1,-262143), //subnormal
    X512(1,-262142),  X512(1,-262141),
    X512(1,-162101),  X512(1,-162100),
    X512(1,-16383),  X512(1,-16382),  X512(1,-16381),
    X512(1,-1025),  X512(1,-1024),  X512(1,-1023),  X512(1,-1022),
    X512(1,-129),  X512(1,-128),  X512(1,-127),  X512(1,-126),  X512(1,-113),  X512(1,-64),
    X512(1,-4),  X512(1,-3),  X512(1,-2),  X512(1,-1),  X512(1,0),  X512(1,1),  X512(1,2),  X512(1,3),  X512(1,4),
    X512(1,29),  X512(1,30),  X512(1,31),  X512(1,32),  X512(1,61),  X512(1,62),  X512(1,63),  X512(1,64),
    X512(1,113),  X512(1,127),  X512(1,128),  X512(1,129),
    X512(1,1023),  X512(1,1024),  X512(1,1025),
    X512(1,16382),  X512(1,16383), X512(1,16384),
    X512(1,162100),  X512(1,162101),
    X512(1,262142),  X512(1,262143),
    X512(1,262144), //infinity or NaN
};

enum {
    f512_num_nonfraction = sizearray(f512_nonfraction),
    f512NumP2   = f512::BITS_PER_SIGNIFICAND * 4,
    f512_num_nonfractionP1 = f512_num_nonfraction * 4,
    f512_num_nonfractionP2 = f512_num_nonfraction * f512NumP2
};

u512
TestSequencer::u512_p1(u16 index)
{
    u512 value;
    const u32 nbits = 512 - (f512::BITS_PER_EXPONENT + 1);

    int bitnum = (index >> 2) % (nbits + 1);

           if (bitnum < 1*64) {   value = u512(0,0,0,0,0,0,0, u64(1) << (bitnum - 0*64) );
    } else if (bitnum < 2*64) {   value = u512(0,0,0,0,0,0, u64(1) << (bitnum - 1*64), 0);
    } else if (bitnum < 3*64) {   value = u512(0,0,0,0,0, u64(1) << (bitnum - 2*64), 0,0);
    } else if (bitnum < 4*64) {   value = u512(0,0,0,0, u64(1) << (bitnum - 3*64), 0,0,0);
    } else if (bitnum < 5*64) {   value = u512(0,0,0, u64(1) << (bitnum - 4*64), 0,0,0,0);
    } else if (bitnum < 6*64) {   value = u512(0,0, u64(1) << (bitnum - 5*64), 0,0,0,0,0);
    } else if (bitnum < 7*64) {   value = u512(0, u64(1) << (bitnum - 6*64), 0,0,0,0,0,0);
    } else {                      value = u512( u64(1) << (bitnum - 7*64), 0,0,0,0,0,0,0);
    }

    if ( index & 1 ) value.negate();
    if ( index & 2 ) value = ~value;

    value.m0 &= makemask64(f512::BITS_PER_HIGH_SIGNIFICAND);
    return value;
}

void
TestSequencer::NextP1(f512 & z)
{
    static const f512 float_p1[] = {
        F512_C(0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
               0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000),
        F512_C(0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000,
               0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001),
        F512_C(0x00000FFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
               0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF),
        F512_C(0x00000FFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
               0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE),
    };

    unsigned sigNum = this->term1Num;
    z = float_p1[ sigNum ];
    z.m0 |= f512_nonfraction[ expNum ];
    ++sigNum;
    if ( sizearray(float_p1) <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f512_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

void
TestSequencer::NextP2(f512 & z)
{
    unsigned sigNum = this->term1Num;
    z = u512_p1( sigNum );
    z.m0 = lobits(z.m0, z.BITS_PER_HIGH_SIGNIFICAND) | f512_nonfraction[ expNum ];
    ++sigNum;
    if ( f512NumP2 <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f512_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

template<>
u64
TestSequencer::WeightExponent<f512>(void)
{
    static const struct {
        u32  mask;
        u32  offset;
    } InfWeight[] = {
        { 0x7FFFFF, 0x000000 },
        { 0x7FFFFF, 0x000000 },
        { 0x3FFFFF, 0x200000 },
        { 0x1FFFFF, 0x300000 },
        { 0x07FFFF, 0x380000 },
        { 0x07FFFF, 0x3C0000 },
        { 0x03FFFF, 0x3E0000 },
        { 0x01FFFF, 0x3F0000 },
        { 0x00FFFF, 0x3F8000 },
        { 0x007FFF, 0x3FC000 },
        { 0x003FFF, 0x3FE000 },
        { 0x001FFF, 0x3FF000 },
        { 0x000FFF, 0x3FF800 },
        { 0x0007FF, 0x3FFC00 },
        { 0x0003FF, 0x3FFE00 },
        { 0x0001FF, 0x3FFF00 },
        { 0x0000FF, 0x3FFF80 },
        { 0x00007F, 0x3FFFC0 },
        { 0x00003F, 0x3FFFE0 },
        { 0x00001F, 0x3FFFF0 },
        { 0x00000F, 0x3FFFF8 },
        { 0x000007, 0x3FFFFC },
    };

    const size_t n = random8() % sizearray(InfWeight);
    const u64 exponent = (random32() & InfWeight[n].mask) + InfWeight[n].offset;
    return exponent << 32;
}

void
TestSequencer::Random(f512 & z)
{
    u64   nonfraction;
    u512  fraction;

    switch ( random8() & 7 ) {
        case 0:
        case 1:
        case 2:
            fraction = u512_p1();
            fraction += u512_p1();
            z = fraction;
            nonfraction = f512_nonfraction[ random8() % sizearray(f512_nonfraction) ];
            z.m0 = nonfraction | lobits( z.m0, z.BITS_PER_HIGH_SIGNIFICAND);
            break;
        case 3:
            z.m7 = random64();
            z.m6 = random64();
            z.m5 = random64();
            z.m4 = random64();
            z.m3 = random64();
            z.m2 = random64();
            z.m1 = random64();
            nonfraction = f512_nonfraction[ random8() % sizearray(f512_nonfraction) ];
            z.m0 = nonfraction | lobits( random64(), z.BITS_PER_HIGH_SIGNIFICAND);
            break;
        case 4:
        case 5:
        case 6:
            fraction = u512_p1();
            fraction += u512_p1();
            z = fraction;
            z.m0 = make_sign<u64>() | WeightExponent<f512>() | lobits( z.m0, z.BITS_PER_HIGH_SIGNIFICAND);
            break;
        case 7:
            z.m7 = random64();
            z.m6 = random64();
            z.m5 = random64();
            z.m4 = random64();
            z.m3 = random64();
            z.m2 = random64();
            z.m1 = random64();
            z.m0 = make_sign<u64>() | WeightExponent<f512>() | lobits( random64(), z.BITS_PER_HIGH_SIGNIFICAND);
            break;
    }
}

size_t
TestSequencer::GetTotal(int level, f512 &)
{
    return level == 1 ? 3 * f512_num_nonfractionP1 : 2 * f512_num_nonfractionP2;
}

size_t
TestSequencer::GetTotal(int level, f512 &, f512 &)
{
     return level == 1 ? U64C(6) * f512_num_nonfractionP1 * f512_num_nonfractionP1 :
                         U64C(2) * f512_num_nonfractionP2 * f512_num_nonfractionP2;
}

} // namespace fpu
} // namespace postrisc
