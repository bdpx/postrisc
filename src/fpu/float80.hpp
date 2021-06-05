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

#define F80_C(hi, lo)  f80::make(U16C(hi), U64C(lo))

struct f80 {
    u64 low;
    u16 high;

    using fraction_t = u64;

    FLOAT_DETAILS(80, 15, 0)

    // f80 does not have hidden bit, but have explicit integer part
    static const u64 UNHIDDEN_BIT  = u64(1) << 63;         // U64C(0x8000000000000000);
    static const u16 SIGN_BIT = u16(1) << 15;

    u128 val(void) const { return u128(u64(high), low); }
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

    static constexpr f80 make(void) { return f80{0, 0}; }
    static constexpr f80 make(u16 exp, u64 mant) { return f80{mant, exp};  }
    static constexpr f80 make(sign_type sign, int power) { return f80{UNHIDDEN_BIT, (u16)((u16(sign) << BITS_PER_EXPONENT) | (power + BIAS)) }; }
    static           f80 make( native::f80 x ) { union { struct { u64 low; u16 high; } s; native::f80 f; } u; u.f = x; return f80{u.s.low, u.s.high}; }

    native::f80 hard(void) const;
    void operator = (const native::f80 & r);

    friend bool IsBothZero(const f80 & a, const f80 & b) { return ( u16( (a.high | b.high) << 1) | a.low | b.low) == 0; }

    friend bool operator == (const f80 & a, const f80 & b) {  return ( a.high == b.high ) && ( a.low == b.low );  }
//    friend bool operator <= (const f80 & a, const f80 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <= b.low ) ); }
//    friend bool operator <  (const f80 & a, const f80 & b) {  return ( a.high <  b.high ) || ( ( a.high == b.high ) && ( a.low <  b.low ) );  }
    friend bool operator != (const f80 & a, const f80 & b) {  return ( a.high != b.high ) || ( a.low != b.low );  }

    static constexpr f80 default_nan(void) { return f80::make(0xFFFF, U64C( 0xC000000000000000 )); }
    static constexpr f80 inf(sign_type s) { return f80::make( (u16(s) << BITS_PER_EXPONENT) | u16(EINF), UNHIDDEN_BIT ); }
    static constexpr f80 zero(sign_type s) { return f80::make( (u16(s) << BITS_PER_EXPONENT), 0 ); }

    f80  abs(void) const { return f80::make( high & ~SIGN_BIT, low );  }
    f80  chs(void) const { return f80::make( high ^  SIGN_BIT, low );  }
    f80 nabs(void) const { return f80::make( high |  SIGN_BIT, low );  }

    bool IsLargerSignificant(f80 b) const;

    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(low);
        ar & BOOST_SERIALIZATION_NVP(high);
    }
};

static_assert(f80::EINF == 0x7fff);

void writeHex(std::ostream & out, const f80 & f );

//bool IsNan(void) const { return ((high & EINF) == EINF) && (low<<1); }
//bool IsNan(void) const { return (exponent() == EINF) && (fraction()<<1); }
inline bool f80::IsNan(void) const
{
    return ( exponent() == EINF ) && ((low << 1) != 0);
}

inline bool f80::IsSignalingNan(void) const
{
//    const u64 a_low = low & ~ U64C( 0x4000000000000000 );
//    return ( ( high & EINF ) == EINF ) && (u64) ( a_low<<1 ) && ( low == a_low );
    return ((high & EINF) == EINF)
            && ! (low & U64C( 0x4000000000000000 ))
            && (low & U64C( 0x3FFFFFFFFFFFFFFF));
}

inline bool f80::IsQuietNan(void) const
{
    return ( ( high & EINF ) == EINF )
             && (U64C( 0x8000000000000000 ) <= u64( low<<1 ));
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

inline native::f80 f80::hard(void) const
{
    union {
        struct { u64 low; u16 high; } s;
        native::f80 f;
    } u;

    u.s.low = low;
    u.s.high = high;

    return u.f;
}

inline void f80::operator = (const native::f80 & r)
{
    union {
        struct { u64 low; u16 high; } s;
        native::f80 f;
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
    explicit hexer(fpu::f80 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
