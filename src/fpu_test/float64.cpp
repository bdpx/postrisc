#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

static inline constexpr
u32
X64(unsigned sign, int exponent)
{
    return ( u64(sign) << 63 )
       | ( ( u64(exponent) + f64::BIAS ) << f64::BITS_PER_SIGNIFICAND );
}

static const u64 f64_nonfraction[] =
{
    X64(0,-1023), //subnormal
    X64(0,-1022),  X64(0,-1021),
    X64(0,-129),  X64(0,-128),  X64(0,-127),  X64(0,-126),  X64(0,-53),
    X64(0,-4),  X64(0,-3),  X64(0,-2),  X64(0,-1),  X64(0,0),  X64(0,1),  X64(0,2),  X64(0,3),  X64(0,4),
    X64(0,29),  X64(0,30),  X64(0,31),  X64(0,32),  X64(0,53),  X64(0,61),  X64(0,62),  X64(0,63),  X64(0,64),
    X64(0,127),  X64(0,128),  X64(0,129),
    X64(0,1022),  X64(0,1023),
    X64(0,1024), //infinity or NaN
    X64(1,-1023), //subnormal
    X64(1,-1022),  X64(1,-1021),
    X64(1,-129),  X64(1,-128),  X64(1,-127),  X64(1,-126),  X64(1,-53),
    X64(1,-4),  X64(1,-3),  X64(1,-2),  X64(1,-1),  X64(1,0),  X64(1,1),  X64(1,2),  X64(1,3),  X64(1,4),
    X64(1,29),  X64(1,30),  X64(1,31),  X64(1,32),  X64(1,53),  X64(1,61),  X64(1,62),  X64(1,63),  X64(1,64),
    X64(1,127),  X64(1,128),  X64(1,129),
    X64(1,1022),  X64(1,1023),
    X64(1,1024) //infinity or NaN
};

enum {
    f64_num_nonfraction   = sizearray(f64_nonfraction),
    f64NumP2              = f64::BITS_PER_SIGNIFICAND * 4,
    f64_num_nonfractionP1 = f64_num_nonfraction * 4,
    f64_num_nonfractionP2 = f64_num_nonfraction * f64NumP2,

};

u64
TestSequencer::u64_p1(u8 nbits, u16 index)
{
    int bitnum = (index >> 2) % (nbits + 1);
    u64 value =  u64(1) << bitnum;

    if (index & 1) value = -value;
    if (index & 2) value = ~value;

    u64 mask = (u64(1) << nbits) - u64(1);
    value = value & mask;
    return value;
}

void
TestSequencer::NextP1(f64 & z)
{
    static const u64 f64P1[] = {
        U64C( 0x0000000000000000 ),
        U64C( 0x0000000000000001 ),
        U64C( 0x000FFFFFFFFFFFFF ),
        U64C( 0x000FFFFFFFFFFFFE )
    };

    unsigned sigNum = this->term1Num;
    z = f64::make( f64_nonfraction[ expNum ] | f64P1[ sigNum ] );
    ++sigNum;
    if ( sizearray(f64P1) <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f64_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

void
TestSequencer::NextP2(f64 & z)
{
    unsigned sigNum = this->term1Num;
    z = f64::make( f64_nonfraction[ expNum ] | u64_p1(52, sigNum) );
    ++sigNum;
    if ( f64NumP2 <= sigNum ) {
        sigNum = 0;
        ++expNum;
        if ( sizearray(f64_nonfraction) <= expNum ) {
            expNum = 0;
            this->done = true;
        }
    }
    this->term1Num = sigNum;
}

template<>
u64
TestSequencer::WeightExponent<f64>(void)
{
    static const struct {
        u32  mask;
        u32  offset;
    } InfWeight[] = {
        { 0x7FF00000, 0x00000000 },
        { 0x7FF00000, 0x00000000 },
        { 0x3FF00000, 0x20000000 },
        { 0x1FF00000, 0x30000000 },
        { 0x0FF00000, 0x38000000 },
        { 0x07F00000, 0x3C000000 },
        { 0x03F00000, 0x3E000000 },
        { 0x01F00000, 0x3F000000 },
        { 0x00F00000, 0x3F800000 },
        { 0x00700000, 0x3FC00000 }
    };

    u64 exponent =  u64( random16() & 0x1FF ) << 52;

    const size_t n = random8() % sizearray(InfWeight);
    exponent = ( exponent & InfWeight[n].mask ) + InfWeight[n].offset;
    return exponent;
}

void
TestSequencer::Random(f64 & z)
{
    u64 nonfraction, sign, fraction, exponent;

    switch ( random8() & 7 ) {
        case 0:
        case 1:
        case 2:
            fraction = lobits(u64_p1(52) + u64_p1(52), 52);
            nonfraction = f64_nonfraction[ random8() % sizearray(f64_nonfraction) ];
            z = f64::make( nonfraction | fraction );
            break;
        case 3:
            fraction = lobits( random64(), 52);
            nonfraction = f64_nonfraction[ random8() % sizearray(f64_nonfraction) ];
            z = f64::make( nonfraction | fraction );
            break;
        case 4:
        case 5:
        case 6:
            sign     = make_sign<u64>();
            fraction = lobits(u64_p1(52) + u64_p1(52), 52);
            exponent = WeightExponent<f64>();
            z = f64::make( sign | exponent | fraction );
            break;
        case 7:
            sign     = make_sign<u64>();
            fraction = lobits(random64(), 52);
            exponent = WeightExponent<f64>();
            z = f64::make( sign | exponent | fraction );
            break;
    }

}

size_t
TestSequencer::GetTotal(int level, f64 &)
{
    return level == 1 ? 3 * f64_num_nonfractionP1 : 2 * f64_num_nonfractionP2;
}

size_t
TestSequencer::GetTotal(int level, f64 &, f64 &)
{
     return level == 1 ? 6 * f64_num_nonfractionP1 * f64_num_nonfractionP1 :
                         2 * f64_num_nonfractionP2 * f64_num_nonfractionP2;
}

} // namespace fpu
} // namespace postrisc
