#pragma once

#include "float_base.hpp"

namespace postrisc {
namespace fpu {

/**************************************************************************************
* quadruple precision
* 2222222211111111 1100000000009999999999888888888877777777776666666666555555555544444444443333333333222222222211111111110000000000
* 7654321098765432 1098765432109876543210987654321098765432109876543210987654321098765432109876543210987654321098765432109876543210
* ?sssssssssssssss ????????????????????????????????????????????????????????????????????????????????????????????????????????????????  Normalized finite
* ?000000000000000 nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn  Denormalized   finite
* 0000000000000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000  Positive zero
* 1000000000000000 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000  Negative zero
* 0111111111111111 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000  Positive Infinity
* 1111111111111111 0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000  Negative Infinity
* ?111111111111111 1???????????????????????????????????????????????????????????????????????????????????????????????????????????????  QNAN
* ?111111111111111 1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000  QNAN indefinite
* ?111111111111111 0nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn  SNAN
*
* ???? - any
* nnnn - at least a single 1
* aaaa - at least single 1 but not all 1's
**************************************************************************************/

class alignas(16) f128 : public CBaseFloat<128,15,1> {
public:
    using fraction_t = u128;

public:
    static const uint64_t HIDDEN_BIT = uint64_t(1) << BITS_PER_HIGH_SIGNIFICAND;
    static const uint64_t SIGN_BIT = uint64_t(1) << 63;

    fraction_t val(void) const { return u128(high, low); }
    sign_type sign(void) const { return (sign_type)(high>>63); }
    fp_exponent_t exponent(void) const { return ( high >> BITS_PER_HIGH_SIGNIFICAND ) & EINF; }
//    uint64_t fraction1(void) const { return low; } //LSB
//    uint64_t fraction0(void) const { return lobits(high, 112 - 64); }  //MSB
    fraction_t fraction(void) const { return u128(lobits(high, BITS_PER_HIGH_SIGNIFICAND), low); }

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    constexpr explicit f128(void) : low(0), high(0) {}
    constexpr explicit f128(uint64_t hi, uint64_t lo) :  low(lo), high(hi) {}
    constexpr explicit f128(sign_type sign, int power) : low(0), high( (uint64_t(sign) << 63) | (uint64_t(power + BIAS) << BITS_PER_HIGH_SIGNIFICAND) ) {}
              explicit f128(quadruple x ) { union { struct { uint64_t low; uint64_t high; } s; quadruple f; } u; u.f = x; high = u.s.high; low = u.s.low; }

    quadruple hard(void) const;
    void operator = (const quadruple & r);

    friend bool IsBothZero(const f128 & a, const f128 & b) { return ( ( ( a.high | b.high ) << 1) | a.low | b.low) == 0; }
    friend bool operator == (const f128 & a, const f128 & b) {  return ( a.high == b.high ) && ( a.low == b.low );  }
//    friend bool operator <= (const f128 & a, const f128 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <= b.low ) ); }
//    friend bool operator <  (const f128 & a, const f128 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <  b.low ) );  }
//    friend bool operator != (const f128 & a, const f128 & b) {  return ( a.high != b.high ) || ( a.low != b.low );  }

    static constexpr f128 default_nan(void) { return f128(UINT64_C( 0xFFFF800000000000 ), UINT64_C( 0x0000000000000000 )); }
    static constexpr f128 inf(sign_type s) { return f128( (uint64_t(s) << 63) | (uint64_t(EINF) << BITS_PER_HIGH_SIGNIFICAND), 0 ); }
    static constexpr f128 zero(sign_type s) { return f128( uint64_t(s) << 63, 0 ); }

    f128 abs (void) const { return f128( high & ~SIGN_BIT, low );  }
    f128 chs (void) const { return f128( high ^  SIGN_BIT, low );  }
    f128 nabs(void) const { return f128( high |  SIGN_BIT, low );  }

    bool IsLargerSignificant(f128) const;

    template<class Archive>
    void
    serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(low);
        ar & BOOST_SERIALIZATION_NVP(high);
    }

public:
    // little-endian order
    uint64_t  low;
    uint64_t  high;
};

void writeHex(std::ostream & out, const f128 & f );

//bool IsNan(void) const { return ((high >> 48) & EINF) == EINF && ((low != 0) || (lobits(high, 48) != 0)); }
// bool IsNan(void) const { return (exponent() == EINF) && fraction(); }
inline bool f128::IsNan(void) const
{
    const uint64_t absAHigh = lobits(high, 63);
    return ( UINT64_C( 0x7FFF000000000000 ) < absAHigh )
            || ( ( absAHigh == UINT64_C( 0x7FFF000000000000 ) ) && low );
}

inline bool f128::IsSignalingNan(void) const
{
    return ( ( ( high >> (BITS_PER_HIGH_SIGNIFICAND - 1) ) & 0xFFFF ) == 0xFFFE )
           && ( low | lobits(high, 47) );
}

inline bool f128::IsQuietNan(void) const
{
    return ( UINT64_C( 0xFFFE000000000000 ) <= uint64_t( high<<1 ) ) && fraction();
}

inline bool f128::IsZero(void) const
{
    return (lobits(high, 63) | low) == 0;
}

inline bool f128::IsInfinity(void) const
{
    return lobits(high, 63) == UINT64_C(0x7fff000000000000) && low == 0;
}

inline bool f128::IsZeroOrDenormal(void) const
{
    return (high & UINT64_C(0x7fff000000000000)) == 0;
}

inline bool f128::IsNeg(void) const
{
    return high >> 63;
}

inline quadruple f128::hard(void) const
{
    union {
        struct { uint64_t low, high; } s;
        quadruple f;
    } u;

    u.s.low = low;
    u.s.high = high;

    return u.f;
}

inline void f128::operator = (const quadruple & r)
{
    union {
       struct { uint64_t low, high; } s;
       quadruple f;
    } u;

    u.f = r;

    high = u.s.high;
    low = u.s.low;
}

inline bool f128::IsLargerSignificant( f128 ) const
{
    // TODO
    return true;
}

} // namespace fpu

namespace fmt {
template<> class hexer<fpu::f128> {
private:
    fpu::f128 v;
public:
    hexer(fpu::f128 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
