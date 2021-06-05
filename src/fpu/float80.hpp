#pragma once

#include "float_base.hpp"

namespace postrisc {
namespace fpu {

/**************************************************************************************
* extended precision
* 7777777777666666 6666555555555544444444443333333333222222222211111111110000000000
* 9876543210987654 3210987654321098765432109876543210987654321098765432109876543210
* ?sssssssssssssss 1???????????????????????????????????????????????????????????????  Normalized finite
* ?000000000000000 0nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn  Denormalized   finite
* 0000000000000000 0000000000000000000000000000000000000000000000000000000000000000  Positive zero
* 1000000000000000 0000000000000000000000000000000000000000000000000000000000000000  Negative zero
* 0111111111111111 0000000000000000000000000000000000000000000000000000000000000000  Positive Infinity
* 1111111111111111 0000000000000000000000000000000000000000000000000000000000000000  Negative Infinity
* ?111111111111111 11??????????????????????????????????????????????????????????????  QNAN
* ?111111111111111 1100000000000000000000000000000000000000000000000000000000000000  QNAN indefinite
* ?111111111111111 10nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn  SNAN
*
* ???? - any
* nnnn - at least a single 1
* aaaa - at least single 1 but not all 1's
**************************************************************************************/

class f80 : public CBaseFloat<80,15,0> {
public:
    using fraction_t = uint64_t;

    // f80 does not have hidden bit, but have explicit integer part
    static const uint64_t UNHIDDEN_BIT  = uint64_t(1) << 63;         // UINT64_C(0x8000000000000000);
    static const uint16_t SIGN_BIT = uint16_t(1) << 15;

    u128 val(void) const { return u128(uint64_t(high), low); }
    sign_type sign(void) const { return (sign_type)(high >> BITS_PER_EXPONENT); }
    fp_exponent_t exponent(void) const { return high & EINF; }
    fraction_t fraction(void) const { return low; }

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    constexpr explicit f80(void) : low(0), high(0) {}
    constexpr explicit f80(uint16_t exp, uint64_t mant) : low(mant), high(exp)  {}
    constexpr explicit f80(sign_type sign, int power) : low(UNHIDDEN_BIT), high( (uint16_t(sign) << BITS_PER_EXPONENT) | uint16_t(power + BIAS) ) {}
    explicit f80( extended x ) { union { struct { uint64_t low; uint16_t high; } s; extended f; } u; u.f = x; high = u.s.high; low = u.s.low; }

    extended hard(void) const;
    void operator = (const extended & r);

    friend bool IsBothZero(const f80 & a, const f80 & b) { return ( uint16_t( (a.high | b.high) << 1) | a.low | b.low) == 0; }

    friend bool operator == (const f80 & a, const f80 & b) {  return ( a.high == b.high ) && ( a.low == b.low );  }
//    friend bool operator <= (const f80 & a, const f80 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <= b.low ) ); }
//    friend bool operator <  (const f80 & a, const f80 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <  b.low ) );  }
    friend bool operator != (const f80 & a, const f80 & b) {  return ( a.high != b.high ) || ( a.low != b.low );  }

    static constexpr f80 default_nan(void) { return f80(0xFFFF, UINT64_C( 0xC000000000000000 )); }
    static constexpr f80 inf(sign_type s) { return f80( (uint16_t(s) << BITS_PER_EXPONENT) | uint16_t(EINF), UNHIDDEN_BIT ); }
    static constexpr f80 zero(sign_type s) { return f80( (uint16_t(s) << BITS_PER_EXPONENT), 0 ); }

    f80  abs(void) const { return f80( high & ~SIGN_BIT, low );  }
    f80  chs(void) const { return f80( high ^  SIGN_BIT, low );  }
    f80 nabs(void) const { return f80( high |  SIGN_BIT, low );  }

    bool IsLargerSignificant(f80 b) const;

    template<class Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(low);
        ar & BOOST_SERIALIZATION_NVP(high);
    }
public:
    uint64_t low;
    uint16_t high;
};

void writeHex(std::ostream & out, const f80 & f );

//bool IsNan(void) const { return ((high & EINF) == EINF) && (low<<1); }
//bool IsNan(void) const { return (exponent() == EINF) && (fraction()<<1); }
inline bool f80::IsNan(void) const
{
    return ( exponent() == EINF ) && ((low << 1) != 0);
}

inline bool f80::IsSignalingNan(void) const
{
//    const uint64_t a_low = low & ~ UINT64_C( 0x4000000000000000 );
//    return ( ( high & EINF ) == EINF ) && (uint64_t) ( a_low<<1 ) && ( low == a_low );
    return ((high & EINF) == EINF)
            && ! (low & UINT64_C( 0x4000000000000000 ))
            && (low & UINT64_C( 0x3FFFFFFFFFFFFFFF));
}

inline bool f80::IsQuietNan(void) const
{
    return ( ( high & EINF ) == EINF )
             && (UINT64_C( 0x8000000000000000 ) <= uint64_t( low<<1 ));
}

inline bool f80::IsZero(void) const
{
    return (high & EINF) == 0 && low == 0;
}

inline bool f80::IsInfinity(void) const
{
    return (high & EINF) == EINF && low == UNHIDDEN_BIT;
}

inline bool f80::IsZeroOrDenormal(void) const
{
    return (high & EINF) == 0;
}

inline bool f80::IsNeg(void) const
{
    return high >> BITS_PER_EXPONENT;
}

inline extended f80::hard(void) const
{
    union {
        struct { uint64_t low; uint16_t high; } s;
        extended f;
    } u;

    u.s.low = low;
    u.s.high = high;

    return u.f;
}

inline void f80::operator = (const extended & r)
{
    union {
        struct { uint64_t low; uint16_t high; } s;
        extended f;
    } u;

    u.f = r;

    high = u.s.high;
    low = u.s.low;
}

inline bool f80::IsLargerSignificant(f80 b) const
{
    return (low < b.low) ? false : (b.low < low) ? true : (high < b.high);
}

} //namespace fpu

namespace fmt {
template<> class hexer<fpu::f80> {
private:
    fpu::f80 v;
public:
    hexer(fpu::f80 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
