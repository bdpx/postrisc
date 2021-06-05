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

struct alignas(16) f128 {
    // little-endian order
    u64  low;
    u64  high;

    using fraction_t = u128;

    FLOAT_DETAILS(128, 15, 1)

    static const u64 HIDDEN_BIT = u64(1) << BITS_PER_HIGH_SIGNIFICAND;
    static const u64 SIGN_BIT = u64(1) << 63;

    fraction_t val(void) const { return u128(high, low); }
    sign_type sign(void) const { return (sign_type)(high>>63); }
    fp_exponent_t exponent(void) const { return ( high >> BITS_PER_HIGH_SIGNIFICAND ) & EINF; }
//    u64 fraction1(void) const { return low; } //LSB
//    u64 fraction0(void) const { return lobits(high, 112 - 64); }  //MSB
    fraction_t fraction(void) const { return u128(lobits(high, BITS_PER_HIGH_SIGNIFICAND), low); }

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    static constexpr f128 make(void) { return f128{0, 0}; }
    static constexpr f128 make(u64 hi, u64 lo) { return f128{lo, hi}; }
    static constexpr f128 make(sign_type sign, int power) { return f128{0, (u64(sign) << 63) | (u64(power + BIAS) << BITS_PER_HIGH_SIGNIFICAND)}; }
    static           f128 make(native::f128 x ) { union { struct { u64 low; u64 high; } s; native::f128 f; } u; u.f = x; return f128{u.s.low, u.s.high}; }

    native::f128 hard(void) const;
    void operator = (const native::f128 & r);

    friend bool IsBothZero(const f128 & a, const f128 & b) { return ( ( ( a.high | b.high ) << 1) | a.low | b.low) == 0; }
    friend bool operator == (const f128 & a, const f128 & b) {  return ( a.high == b.high ) && ( a.low == b.low );  }
//    friend bool operator <= (const f128 & a, const f128 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <= b.low ) ); }
//    friend bool operator <  (const f128 & a, const f128 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <  b.low ) );  }
//    friend bool operator != (const f128 & a, const f128 & b) {  return ( a.high != b.high ) || ( a.low != b.low );  }

    static constexpr f128 default_nan(void) { return f128::make(U64C( 0xFFFF800000000000 ), U64C( 0x0000000000000000 )); }
    static constexpr f128 inf(sign_type s) { return f128::make( (u64(s) << 63) | (u64(EINF) << BITS_PER_HIGH_SIGNIFICAND), 0 ); }
    static constexpr f128 zero(sign_type s) { return f128::make( u64(s) << 63, 0 ); }

    f128 abs (void) const { return f128::make( high & ~SIGN_BIT, low );  }
    f128 chs (void) const { return f128::make( high ^  SIGN_BIT, low );  }
    f128 nabs(void) const { return f128::make( high |  SIGN_BIT, low );  }

    bool IsLargerSignificant(f128) const;

    template<typename Archive>
    void
    serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(low);
        ar & BOOST_SERIALIZATION_NVP(high);
    }
};

#define F128_C(hi, lo) f128::make(U64C(hi), U64C(lo))

void writeHex(std::ostream & out, const f128 & f );

//bool IsNan(void) const { return ((high >> 48) & EINF) == EINF && ((low != 0) || (lobits(high, 48) != 0)); }
// bool IsNan(void) const { return (exponent() == EINF) && fraction(); }
inline bool f128::IsNan(void) const
{
    const u64 absAHigh = lobits(high, 63);
    return ( U64C( 0x7FFF000000000000 ) < absAHigh )
            || ( ( absAHigh == U64C( 0x7FFF000000000000 ) ) && low );
}

inline bool f128::IsSignalingNan(void) const
{
    return ( ( ( high >> (BITS_PER_HIGH_SIGNIFICAND - 1) ) & 0xFFFF ) == 0xFFFE )
           && ( low | lobits(high, 47) );
}

inline bool f128::IsQuietNan(void) const
{
    return ( U64C( 0xFFFE000000000000 ) <= u64( high<<1 ) ) && fraction();
}

inline bool f128::IsZero(void) const
{
    return (lobits(high, 63) | low) == 0;
}

inline bool f128::IsInfinity(void) const
{
    return lobits(high, 63) == U64C(0x7fff000000000000) && low == 0;
}

inline bool f128::IsZeroOrDenormal(void) const
{
    return (high & U64C(0x7fff000000000000)) == 0;
}

inline bool f128::IsNeg(void) const
{
    return high >> 63;
}

inline native::f128 f128::hard(void) const
{
    return std::bit_cast<native::f128>(*this);
}

inline void f128::operator = (const native::f128 & r)
{
    union {
       struct { u64 low, high; } s;
       native::f128 f;
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
    explicit hexer(fpu::f128 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
