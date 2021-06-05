#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

static inline constexpr
u16
X80(unsigned sign, int exponent)
{
    return ( u16(sign) << 15 )
           | u16( (exponent) + f80::BIAS );
}

static const u16 f80_nonfraction[] =
{
    X80(0,-16383), //subnormal
    X80(0,-16382),  X80(0,-16381),
    X80(0,-1025),  X80(0,-1024),  X80(0,-1023),  X80(0,-1022),
    X80(0,-129),  X80(0,-128),  X80(0,-127),  X80(0,-126),  X80(0,-113),  X80(0,-64),
    X80(0,-4),  X80(0,-3),  X80(0,-2),  X80(0,-1),  X80(0,0),  X80(0,1),  X80(0,2),  X80(0,3),  X80(0,4),
    X80(0,29),  X80(0,30),  X80(0,31),  X80(0,32),  X80(0,61),  X80(0,62),  X80(0,63),  X80(0,64),
    X80(0,113),  X80(0,127),  X80(0,128),  X80(0,129),
    X80(0,1023),  X80(0,1024),  X80(0,1025),  X80(0,16382),  X80(0,16383),
    X80(0,16384), //infinity or NaN
    X80(1,-16383), //subnormal
    X80(1,-16382),  X80(1,-16381),
    X80(1,-1025),  X80(1,-1024),  X80(1,-1023),  X80(1,-1022),
    X80(1,-129),  X80(1,-128),  X80(1,-127),  X80(1,-126),  X80(1,-113),  X80(1,-64),
    X80(1,-4),  X80(1,-3),  X80(1,-2),  X80(1,-1),  X80(1,0),  X80(1,1),  X80(1,2),  X80(1,3),  X80(1,4),
    X80(1,29),  X80(1,30),  X80(1,31),  X80(1,32),  X80(1,61),  X80(1,62),  X80(1,63),  X80(1,64),
    X80(1,113),  X80(1,127),  X80(1,128),  X80(1,129),
    X80(1,1023),  X80(1,1024),  X80(1,1025),
    X80(1,16382),  X80(1,16383),
    X80(1,16384) //infinity or NaN
};

enum {
    f80_num_nonfraction   = sizearray(f80_nonfraction),
    f80NumP1              = 4,
    f80NumP2              = (f80::BITS_PER_SIGNIFICAND - 1) * 4,
    f80_num_nonfractionP1 = f80_num_nonfraction * f80NumP1,
    f80_num_nonfractionP2 = f80_num_nonfraction * f80NumP2,
};

void
TestSequencer::NextP1(f80 & z)
{
    static const u64 f80P1[] = {
        U64C( 0x0000000000000000 ),
        U64C( 0x0000000000000001 ),
        U64C( 0x7FFFFFFFFFFFFFFF ),
        U64C( 0x7FFFFFFFFFFFFFFE )
    };

    unsigned sigNum = this->term1Num;
    z.low = f80P1[ sigNum ];
    z.high = f80_nonfraction[ expNum ];
    if ( z.high & 0x7FFF ) z.low |= z.UNHIDDEN_BIT;
    ++sigNum;
    if ( sizearray(f80P1) <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f80_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

void
TestSequencer::NextP2(f80 & z)
{
    unsigned sigNum = this->term1Num;
    z.low = u64_p1(63, sigNum);
    z.high = f80_nonfraction[ expNum ];
    if ( z.high & 0x7FFF ) z.low |= z.UNHIDDEN_BIT;
    ++sigNum;
    if ( f80NumP2 <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f80_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

template<>
u64
TestSequencer::WeightExponent<f80>(void)
{
    static const struct {
        u16  mask;
        u16  offset;
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
    const u16 exponent = (random16() & InfWeight[n].mask) + InfWeight[n].offset;
    return exponent;
}

void
TestSequencer::Random(f80 & z)
{
    switch ( random8() & 7 ) {
        case 0:
        case 1:
        case 2:
            z.low = lobits( u64_p1(63) + u64_p1(63), 63);
            z.high = f80_nonfraction[ random8() % sizearray(f80_nonfraction) ];
            if ( z.high & 0x7FFF ) z.low |= z.UNHIDDEN_BIT;
            break;
        case 3:
            z.low = lobits(random64(), 63);
            z.high = f80_nonfraction[ random8() % sizearray(f80_nonfraction) ];
            if ( z.high & 0x7FFF ) z.low |= z.UNHIDDEN_BIT;
            break;
        case 4:
        case 5:
        case 6:
            z.low = lobits( u64_p1(63) + u64_p1(63), 63);
            z.high = WeightExponent<f80>();
            if ( z.high ) z.low |= z.UNHIDDEN_BIT;
            z.high |= make_sign<u16>();
            break;
        case 7:
            z.low = lobits(random64(), 63);
            z.high = WeightExponent<f80>();
            if ( z.high ) z.low |= z.UNHIDDEN_BIT;
            z.high |= make_sign<u16>();
            break;
    }
}

size_t
TestSequencer::GetTotal(int level, f80 &)
{
    return level == 1 ? 3 * f80_num_nonfractionP1 : 2 * f80_num_nonfractionP2;
}

size_t
TestSequencer::GetTotal(int level, f80 &, f80 &)
{
    return level == 1 ? 6 * f80_num_nonfractionP1 * f80_num_nonfractionP1 :
                        2 * f80_num_nonfractionP2 * f80_num_nonfractionP2;
}

BOOST_AUTO_TEST_CASE(test_f80_snan)
{
    static const struct {
        bool isSNAN;
        f80 value;
    } cases[] = {
        { false, F80_C(0x7fff, 0xcca4224224fb0249) },
        { false, F80_C(0x7fff, 0xfe0000003ffffffe) },
        { false, F80_C(0xffff, 0xcca4224224fb0249) },
        { false, F80_C(0xffff, 0xfe0000003ffffffe) },
        {  true, F80_C(0x7fff, 0xae0000003ffffffe) },
        {  true, F80_C(0xffff, 0xae0000003ffffffe) },
    };

    for (size_t i=0; i<sizearray(cases); i++) {
        BOOST_TEST(cases[i].value.IsSignalingNan() == cases[i].isSNAN);
    }
}

} // namespace fpu
} // namespace postrisc
