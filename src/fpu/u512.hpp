#pragma once

#include "u256.hpp"

namespace postrisc {
namespace fpu {

/***************************************************************************
* 512 bit fraction
***************************************************************************/
class u512 : public u256 {
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

public:
    uint64_t   m4, m5, m6, m7;

public:
    explicit u512(void) {}

    explicit u512(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3,
                     uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7)
      : u256(x0, x1, x2, x3), m4(x4), m5(x5), m6(x6), m7(x7) {}

    explicit u512(const u256 & r)
      : u256(r), m4(0), m5(0), m6(0), m7(0) {}

    void clear(void) {  m0 = m1 = m2 = m3 = m4 = m5 = m6 = m7 = 0;  }

    void mul( const u256 & a, const u256 & b );

    operator bool (void) const { return ( ( m0 | m1 | m2 | m3 | m4 | m5 | m6 | m7 ) != 0 ); }

    void operator |= ( const u512 & r )
    {
        m0 |= r.m0;
        m1 |= r.m1;
        m2 |= r.m2;
        m3 |= r.m3;
        m4 |= r.m4;
        m5 |= r.m5;
        m6 |= r.m6;
        m7 |= r.m7;
    }

    void operator &= ( const u512 & r )
    {
        m0 &= r.m0;
        m1 &= r.m1;
        m2 &= r.m2;
        m3 &= r.m3;
        m4 &= r.m4;
        m5 &= r.m5;
        m6 &= r.m6;
        m7 &= r.m7;
    }

    void operator ^= ( const u512 & r )
    {
        m0 ^= r.m0;
        m1 ^= r.m1;
        m2 ^= r.m2;
        m3 ^= r.m3;
        m4 ^= r.m4;
        m5 ^= r.m5;
        m6 ^= r.m6;
        m7 ^= r.m7;
    }

    u512 operator & (const u512 & r ) const
    {
        return u512( m0 & r.m0,
                        m1 & r.m1,
                        m2 & r.m2,
                        m3 & r.m3,
                        m4 & r.m4,
                        m5 & r.m5,
                        m6 & r.m6,
                        m7 & r.m7 );
    }

    u512 operator | (const u512 & r ) const
    {
        return u512( m0 | r.m0,
                        m1 | r.m1,
                        m2 | r.m2,
                        m3 | r.m3,
                        m4 | r.m4,
                        m5 | r.m5,
                        m6 | r.m6,
                        m7 | r.m7 );
    }

    u512 operator ^ (const u512 & r ) const
    {
        return u512( m0 ^ r.m0,
                        m1 ^ r.m1,
                        m2 ^ r.m2,
                        m3 ^ r.m3,
                        m4 ^ r.m4,
                        m5 ^ r.m5,
                        m6 ^ r.m6,
                        m7 ^ r.m7 );
    }

    void add(const u512 & a, const u512 & b);
    void sub(const u512 & a, const u512 & b);

    void add( const u512 & r ) { add( *this, r ); }
    void sub( const u512 & r ) { sub( *this, r ); }

    void operator ++ (void) { add( *this, u512(0,0,0,0,0,0,0,1) ); }
    void operator -- (void) { sub( *this, u512(0,0,0,0,0,0,0,1) ); }

    void negate (void) { sub( u512(0,0,0,0,0,0,0,0), *this ); }

    u512 operator ~ (void) const { return u512( ~m0, ~m1, ~m2, ~m3, ~m4, ~m5, ~m6, ~m7 ); }

    u512 operator << (int nbits) const;
    void operator <<= (int nbits);
    u512 operator >> (int nbits) const;
    void operator >>= (int nbits);

    bool operator == (const u512 & r) const;
    bool operator != (const u512 & r) const;
    bool operator <  (const u512 & r) const;
    bool operator <= (const u512 & r) const;

    void jamming(int count);
    uint64_t jamming(int count, uint64_t a8);
    uint64_t extra_jamming(void) const { return m4 | ( ( m5 | m6 | m7 ) != 0 ); }
    int normalize_subnormal(void);

    friend std::ostream & operator << (std::ostream & out, const u512 & r);
};

template<class Archive>
void
u512::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(u256);

    ar & BOOST_SERIALIZATION_NVP(m7);
    ar & BOOST_SERIALIZATION_NVP(m6);
    ar & BOOST_SERIALIZATION_NVP(m5);
    ar & BOOST_SERIALIZATION_NVP(m4);
}

} // namespace fpu
} // namespace postrisc
