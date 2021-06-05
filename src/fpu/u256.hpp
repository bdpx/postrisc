#pragma once

#include "util/common.hpp"
#include "u128.hpp"
#include "bigint.hpp"

namespace postrisc {
namespace fpu {

/****************************************************************************
*  256-bit fraction
****************************************************************************/
class u256 {
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive & ar, const unsigned int version);

public:
    u64  m0, m1, m2, m3;
public:

    explicit u256(void) { }
    constexpr explicit u256(u64 x0, u64 x1, u64 x2, u64 x3)
             : m0(x0), m1(x1), m2(x2), m3(x3) { }

    void clear(void)
    {
        m3 = m2 = m1 = m0 = 0;
    }

    void mul( const u128 & a, const u128 & b );

    operator bool (void) const { return ( ( m0 | m1 | m2 | m3 ) != 0 ); }

    void operator |= ( const u256 & r ) {  m0 |= r.m0;   m1 |= r.m1;   m2 |= r.m2;   m3 |= r.m3;   }
    void operator &= ( const u256 & r ) {  m0 &= r.m0;   m1 &= r.m1;   m2 &= r.m2;   m3 &= r.m3;   }
    void operator ^= ( const u256 & r ) {  m0 ^= r.m0;   m1 ^= r.m1;   m2 ^= r.m2;   m3 ^= r.m3;   }

    u256 operator & (const u256 & r ) const {  return u256( m0 & r.m0,  m1 & r.m1,  m2 & r.m2,  m3 & r.m3 );  }
    u256 operator | (const u256 & r ) const {  return u256( m0 | r.m0,  m1 | r.m1,  m2 | r.m2,  m3 | r.m3 );  }
    u256 operator ^ (const u256 & r ) const {  return u256( m0 ^ r.m0,  m1 ^ r.m1,  m2 ^ r.m2,  m3 ^ r.m3 );  }

    u256 operator + (const u256 & r) const;
    u256 operator - (const u256 & r) const;

    u256 operator - (void) const {  return u256(0,0,0,0) - (*this);  }
    u256 operator ~ (void) const {  return u256( ~m0, ~m1, ~m2, ~m3 );  }
    void operator += (const u256 & r) {  (*this) = (*this) + r;  }
    void operator -= (const u256 & r) {  (*this) = (*this) - r;  }
    void operator ++ (void) {  (*this) = (*this) + u256(0,0,0,1);  }
    void operator -- (void) {  (*this) = (*this) - u256(0,0,0,1);  }


    u256 operator << (int nbits) const;
    u256 operator >> (int nbits) const;

    void operator <<= (int nbits) {  (*this) = (*this) << nbits;  }
    void operator >>= (int nbits) {  (*this) = (*this) >> nbits;  }

    bool operator == (const u256 & r) const;
    bool operator != (const u256 & r) const;
    bool operator <  (const u256 & r) const;
    bool operator <= (const u256 & r) const;

    void jamming(int count);
    u64 jamming(int count, u64 a4);
    u64 extra_jamming(void) const { return m2 | ( m3 != 0 ); }
    int normalize_subnormal(void);
    friend std::ostream & operator << (std::ostream & out, const u256 & r);

};

template<typename Archive>
void
u256::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m3);
    ar & BOOST_SERIALIZATION_NVP(m2);
    ar & BOOST_SERIALIZATION_NVP(m1);
    ar & BOOST_SERIALIZATION_NVP(m0);
}

} // namespace fpu
} // namespace postrisc
