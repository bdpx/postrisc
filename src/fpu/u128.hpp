#pragma once

#include "util/common.hpp"

namespace postrisc {
namespace fpu {

/****************************************************************************
*  128-bit unsigned integer
****************************************************************************/
class alignas(16) u128 {
    friend class boost::serialization::access;
    template<typename Archive>
    void serialize(Archive & ar, const unsigned int version);

public:
    u64  m1, m0;   // little endian order!!! (low, high)
public:
    u64 & high() { return m0; }
    u64 & low() { return m1; }
    const u64 & high() const { return m0; }
    const u64 & low() const { return m1; }

    u128(void) {}
    constexpr explicit u128(u64 x1) : m1(x1), m0(0) {}
    constexpr explicit u128(u64 x0, u64 x1) : m1(x1), m0(x0) {}

    void clear(void) {  m1 = m0 = 0;  }
    operator bool (void) const { return ( ( m0 | m1 ) != 0 ); }

    void operator |= ( const u128 & r ) {  m1 |= r.m1;   m0 |= r.m0;  }
    void operator &= ( const u128 & r ) {  m1 &= r.m1;   m0 &= r.m0;  }
    void operator ^= ( const u128 & r ) {  m1 ^= r.m1;   m0 ^= r.m0;  }

    u128 operator & (const u128 & r ) const {  return u128( m0 & r.m0, m1 & r.m1 );  }
    u128 operator | (const u128 & r ) const {  return u128( m0 | r.m0, m1 | r.m1 );  }
    u128 operator ^ (const u128 & r ) const {  return u128( m0 ^ r.m0, m1 ^ r.m1 );  }

    u128 operator << (int nbits) const
    {
        assert(nbits > 0);
        assert(nbits < 64);
        return u128( ( m0 << nbits ) | ( m1 >> (64 - nbits) ),
                          ( m1 << nbits ) );
    }
    u128 operator >> (int nbits) const
    {
        assert(nbits > 0);
        assert(nbits < 64);
        return u128( ( m0 >> nbits ),
                          ( m1 >> nbits ) | ( m0 << (64 - nbits) ) );
    }

    void operator <<= (int nbits) {  (*this) = (*this) << nbits;  }
    void operator >>= (int nbits) {  (*this) = (*this) >> nbits;  }

    u128 operator + (const u128 & r ) const { return u128(m0 + r.m0 + (m1 + r.m1 < m1), m1 + r.m1); }
    u128 operator - (const u128 & r ) const { return u128(m0 - r.m0 - (m1 < r.m1), m1 - r.m1); }
    u128 operator - (void) const { return u128(0,0) - (*this); }
    u128 operator ~ (void) const { return u128(~m0, ~m1); }

    void operator += (const u128 & r ) { (*this) = (*this) + r; }
    void operator -= (const u128 & r ) { (*this) = (*this) - r; }
    void operator ++ (void) { (*this) = (*this) + u128(0,1); }
    void operator -- (void) { (*this) = (*this) - u128(0,1); }

    bool operator == ( const u128 & r ) const {  return ( m0 == r.m0 ) &&   ( m1 == r.m1 );  }
    bool operator != ( const u128 & r ) const {  return ( m0 != r.m0 ) ||   ( m1 != r.m1 );  }
    bool operator <= ( const u128 & r ) const {  return ( m0 <  r.m0 ) || ( ( m0 == r.m0 ) && ( m1 <= r.m1 ) ); }
    bool operator <  ( const u128 & r ) const {  return ( m0 <  r.m0 ) || ( ( m0 == r.m0 ) && ( m1 <  r.m1 ) ); }

    void jamming(int count);
    u64 jamming(int count, u64 a2);
    int normalize_subnormal(void);

    friend std::ostream & operator << (std::ostream & out, const u128 & r);
};

template<typename Archive>
void
u128::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m1);
    ar & BOOST_SERIALIZATION_NVP(m0);
}

} // namespace fpu
} // namespace postrisc
