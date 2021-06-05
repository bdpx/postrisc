#include "util/common.hpp"
#include "float.hpp"
#include "controller.hpp"

namespace postrisc {
namespace fpu {

static inline constexpr
uint32_t
X64(unsigned sign, int exponent)
{
    return ( uint64_t(sign) << 63 )
       | ( ( uint64_t(exponent) + f64::BIAS ) << f64::BITS_PER_SIGNIFICAND );
}

static const uint64_t f64_nonfraction[] =
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

uint64_t
CSequence::u64_p1(uint8_t nbits, uint16_t index)
{
    int bitnum = (index >> 2) % (nbits + 1);
    uint64_t value =  uint64_t(1) << bitnum;

    if (index & 1) value = -value;
    if (index & 2) value = ~value;

    uint64_t mask = (uint64_t(1) << nbits) - uint64_t(1);
    value = value & mask;
    return value;
}

void
CSequence::NextP1(f64 & z)
{
    static const uint64_t f64P1[] = {
        UINT64_C( 0x0000000000000000 ),
        UINT64_C( 0x0000000000000001 ),
        UINT64_C( 0x000FFFFFFFFFFFFF ),
        UINT64_C( 0x000FFFFFFFFFFFFE )
    };

    unsigned sigNum = this->term1Num;
    z = f64( f64_nonfraction[ expNum ] | f64P1[ sigNum ] );
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
CSequence::NextP2(f64 & z)
{
    unsigned sigNum = this->term1Num;
    z = f64( f64_nonfraction[ expNum ] | u64_p1(52, sigNum) );
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
uint64_t
CSequence::WeightExponent<f64>(void)
{
    static const struct {
        uint32_t  mask;
        uint32_t  offset;
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

    uint64_t exponent =  uint64_t( random16() & 0x1FF ) << 52;

    const size_t n = random8() % sizearray(InfWeight);
    exponent = ( exponent & InfWeight[n].mask ) + InfWeight[n].offset;
    return exponent;
}

void
CSequence::Random(f64 & z)
{
    uint64_t nonfraction, sign, fraction, exponent;

    switch ( random8() & 7 ) {
        case 0:
        case 1:
        case 2:
            fraction = lobits(u64_p1(52) + u64_p1(52), 52);
            nonfraction = f64_nonfraction[ random8() % sizearray(f64_nonfraction) ];
            z = f64( nonfraction | fraction );
            break;
        case 3:
            fraction = lobits( random64(), 52);
            nonfraction = f64_nonfraction[ random8() % sizearray(f64_nonfraction) ];
            z = f64( nonfraction | fraction );
            break;
        case 4:
        case 5:
        case 6:
            sign     = make_sign<uint64_t>();
            fraction = lobits(u64_p1(52) + u64_p1(52), 52);
            exponent = WeightExponent<f64>();
            z = f64( sign | exponent | fraction );
            break;
        case 7:
            sign     = make_sign<uint64_t>();
            fraction = lobits(random64(), 52);
            exponent = WeightExponent<f64>();
            z = f64( sign | exponent | fraction );
            break;
    }

}

size_t
CSequence::GetTotal(int level, f64 &)
{
    return level == 1 ? 3 * f64_num_nonfractionP1 : 2 * f64_num_nonfractionP2;
}

size_t
CSequence::GetTotal(int level, f64 &, f64 &)
{
     return level == 1 ? 6 * f64_num_nonfractionP1 * f64_num_nonfractionP1 :
                         2 * f64_num_nonfractionP2 * f64_num_nonfractionP2;
}

} // namespace fpu
} // namespace postrisc
