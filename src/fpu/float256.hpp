#pragma once

#include "float_base.hpp"

namespace postrisc {
namespace fpu {

/**************************************************************************************
* octuple precision
**************************************************************************************/
#define F256_C(a,b,c,d) f256(U64C(a), U64C(b), U64C(c), U64C(d))

class f256 {
public:
    u64  m3, m2, m1, m0;
public:
    FLOAT_DETAILS(256, 19, 1)

    static const u64 HIDDEN_BIT = u64(1) << BITS_PER_HIGH_SIGNIFICAND;
    static const u64 SIGN_BIT = u64(1) << 63;

    u256 val(void) const;
    sign_type sign(void) const;
    fp_exponent_t exponent(void) const;
    u256 fraction(void) const;

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    constexpr explicit f256(void) : m3(0), m2(0), m1(0), m0(0) {}
    constexpr explicit f256(u64 x0, u64 x1, u64 x2, u64 x3)
       : m3(x3), m2(x2), m1(x1), m0(x0) {}
    constexpr explicit f256(sign_type sign, int power)
       : m3(0), m2(0), m1(0), m0( (u64(sign) << 63) | (u64(power + BIAS) << BITS_PER_HIGH_SIGNIFICAND) ) {}

    friend bool IsBothZero(const f256 & a, const f256 & b);
    void operator = (const u256 & u);

    static constexpr f256 default_nan(void) { return f256( U64C( 0xFFFFF80000000000 ), 0, 0, 0); }
    static constexpr f256 inf(sign_type s)  { return f256( (u64(s) << 63) | (u64(EINF) << BITS_PER_HIGH_SIGNIFICAND), 0, 0, 0 ); }
    static constexpr f256 zero(sign_type s) { return f256( u64(s) << 63, 0, 0, 0 ); }

    f256 abs (void) const { return f256( m0 & ~SIGN_BIT, m1, m2, m3 );  }
    f256 chs (void) const { return f256( m0 ^  SIGN_BIT, m1, m2, m3 );  }
    f256 nabs(void) const { return f256( m0 |  SIGN_BIT, m1, m2, m3 );  }

    bool IsLargerSignificant( f256 ) const { return true; }

    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(m3);
        ar & BOOST_SERIALIZATION_NVP(m2);
        ar & BOOST_SERIALIZATION_NVP(m1);
        ar & BOOST_SERIALIZATION_NVP(m0);
    }
};

void writeHex(std::ostream & out, const f256 & f );

inline u256 f256::val(void) const
{
    return u256(m0, m1, m2, m3);
}

inline sign_type f256::sign(void) const
{
     return (sign_type)(m0 >> 63);
}

inline fp_exponent_t f256::exponent(void) const
{
    return ( m0 >> BITS_PER_HIGH_SIGNIFICAND ) & EINF;
}

inline u256 f256::fraction(void) const
{
    return u256(lobits(m0, BITS_PER_HIGH_SIGNIFICAND), m1, m2, m3);
}

inline bool f256::IsNan(void) const
{
    return ((m0 >> BITS_PER_HIGH_SIGNIFICAND) & EINF) == EINF
           && (m1 | m2 | m3 | lobits(m0, BITS_PER_HIGH_SIGNIFICAND)) != 0;
}

inline bool f256::IsSignalingNan(void) const
{
    return ( ( ( m0 >> (BITS_PER_HIGH_SIGNIFICAND - 1) ) & 0xFFFFF ) == 0xFFFFE )
           && ( m3 | m2 | m1 | lobits(m0, (BITS_PER_HIGH_SIGNIFICAND - 1)) );
}

inline bool f256::IsQuietNan(void) const
{
    return ( U64C( 0xFFFFE00000000000 ) <= u64( m0<<1 ) )
           && ( m3 | m2 | m1 | lobits(m0, BITS_PER_HIGH_SIGNIFICAND) );
}

inline bool f256::IsZero(void) const
{
    return (lobits(m0, 63) | m1 | m2 | m3) == 0;
}

inline bool f256::IsInfinity(void) const
{
    return lobits(m0, 63) == U64C(0x7FFFF00000000000) && (m1 | m2 | m3) == 0;
}

inline bool f256::IsZeroOrDenormal(void) const
{
    return (m0 & U64C(0x7FFFF00000000000)) == 0;
}

inline bool f256::IsNeg(void) const
{
    return m0 >> 63;
}

inline void f256::operator = (const u256 & u)
{
    m0 = u.m0;
    m1 = u.m1;
    m2 = u.m2;
    m3 = u.m3;
}

inline bool IsBothZero(const f256 & a, const f256 & b)
{
    return ( ( ( a.m0 | b.m0 ) << 1) | a.m1 | a.m2 | a.m3 | b.m1 | b.m2 | b.m3) == 0;
}

} // namespace fpu

namespace fmt {

template<> class hexer<fpu::f256> {
private:
    fpu::f256 v;
public:
    explicit hexer(fpu::f256 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} //namespace fmt
} // namespace postrisc
