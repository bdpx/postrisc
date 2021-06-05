#pragma once

#include "float_base.hpp"
#include "u512.hpp"

namespace postrisc {
namespace fpu {

/**************************************************************************************
*
**************************************************************************************/
#define F512_C(a,b,c,d,e,f,g,h) f512(U64C(a), U64C(b), U64C(c), U64C(d), U64C(e), U64C(f), U64C(g), U64C(h))

class f512 {
public:
    u64  m7, m6, m5, m4, m3, m2, m1, m0;

    FLOAT_DETAILS(512, 23, 1)

public:
    static const u64 HIDDEN_BIT = u64(1) << BITS_PER_HIGH_SIGNIFICAND;
    static const u64 SIGN_BIT = u64(1) << 63;

    u512 val(void) const { return u512(m0, m1, m2, m3, m4, m5, m6, m7); }
    sign_type sign(void) const { return (sign_type)(m0 >> 63); }
    fp_exponent_t exponent(void) const { return ( m0 >> BITS_PER_HIGH_SIGNIFICAND ) & EINF; }
    u512 fraction(void) const { return u512(lobits(m0, BITS_PER_HIGH_SIGNIFICAND), m1, m2, m3, m4, m5, m6, m7); }

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    constexpr explicit f512(void)
        : m7(0), m6(0), m5(0), m4(0), m3(0), m2(0), m1(0), m0(0) {}
    constexpr explicit f512( u64 x0, u64 x1, u64 x2, u64 x3, u64 x4, u64 x5, u64 x6, u64 x7 )
        : m7(x7), m6(x6), m5(x5), m4(x4), m3(x3), m2(x2), m1(x1), m0(x0) {}
    constexpr explicit f512(sign_type sign, int power)
        : m7(0), m6(0), m5(0), m4(0), m3(0), m2(0), m1(0),
          m0( (u64(sign) << 63) | (u64(power + BIAS) << BITS_PER_HIGH_SIGNIFICAND) ) {}

    friend bool IsBothZero(const f512 & a, const f512 & b);
    void operator = (const u512 & u);

    static constexpr f512 default_nan(void) {
        return f512( U64C( 0xFFFFFF8000000000 ), 0, 0, 0, 0, 0, 0, 0 ); }
    static constexpr f512 inf(sign_type s)  {
        return f512( (u64(s) << 63) | (u64(EINF) << BITS_PER_HIGH_SIGNIFICAND), 0, 0, 0, 0, 0, 0, 0 ); }
    static constexpr f512 zero(sign_type s) {
        return f512( u64(s) << 63, 0, 0, 0, 0, 0, 0, 0 ); }

    f512 abs (void) const { return f512( m0 & ~SIGN_BIT, m1, m2, m3, m4, m5, m6, m7 );  }
    f512 chs (void) const { return f512( m0 ^  SIGN_BIT, m1, m2, m3, m4, m5, m6, m7 );  }
    f512 nabs(void) const { return f512( m0 |  SIGN_BIT, m1, m2, m3, m4, m5, m6, m7 );  }

    bool IsLargerSignificant( f512 ) const { return true; }

    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(m7);
        ar & BOOST_SERIALIZATION_NVP(m6);
        ar & BOOST_SERIALIZATION_NVP(m5);
        ar & BOOST_SERIALIZATION_NVP(m4);
        ar & BOOST_SERIALIZATION_NVP(m3);
        ar & BOOST_SERIALIZATION_NVP(m2);
        ar & BOOST_SERIALIZATION_NVP(m1);
        ar & BOOST_SERIALIZATION_NVP(m0);
    }
};

void writeHex(std::ostream & out, const f512 & f );

inline bool f512::IsNan(void) const
{
    return ((m0 >> BITS_PER_HIGH_SIGNIFICAND) & EINF) == EINF
           && (m1 | m2 | m3 | m4 | m5 | m6 | m7 | lobits(m0, BITS_PER_HIGH_SIGNIFICAND)) != 0;
}

inline bool f512::IsSignalingNan(void) const
{
    return ( ( ( m0 >> (BITS_PER_HIGH_SIGNIFICAND - 1) ) & 0xFFFFFF ) == 0xFFFFFE )
           && ( m7 | m6 | m5 | m4 | m3 | m2 | m1 | lobits(m0, (BITS_PER_HIGH_SIGNIFICAND - 1)) );
}

inline bool f512::IsQuietNan(void) const
{
    return ( U64C( 0xFFFFFE0000000000 ) <= u64( m0<<1 ) )
           && ( m7 | m6 | m5 | m4 | m3 | m2 | m1 | lobits(m0, BITS_PER_HIGH_SIGNIFICAND) );
}

inline bool f512::IsZero(void) const
{
    return (lobits(m0, 63) | m1 | m2 | m3 | m4 | m5 | m6 | m7) == 0;
}

inline bool f512::IsInfinity(void) const
{
    return lobits(m0, 63) == U64C(0x7FFFFF0000000000)
           && (m1 | m2 | m3 | m4 | m5 | m6 | m7) == 0;
}

inline bool f512::IsZeroOrDenormal(void) const
{
    return (m0 & U64C(0x7FFFFF0000000000)) == 0;
}

inline bool f512::IsNeg(void) const
{
    return m0 >> 63;
}

inline void f512::operator = (const u512 & u)
{
    m0 = u.m0;
    m1 = u.m1;
    m2 = u.m2;
    m3 = u.m3;
    m4 = u.m4;
    m5 = u.m5;
    m6 = u.m6;
    m7 = u.m7;
}

inline bool IsBothZero(const f512 & a, const f512 & b)
{
    return ( ( ( a.m0 | b.m0 ) << 1)
          | a.m1 | a.m2 | a.m3 | a.m4 | a.m5 | a.m6 | a.m7
          | b.m1 | b.m2 | b.m3 | b.m4 | b.m5 | b.m6 | b.m7 ) == 0;
}

} // namespace fpu

namespace fmt {
template<> class hexer<fpu::f512> {
private:
    fpu::f512 v;
public:
    explicit hexer(fpu::f512 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
