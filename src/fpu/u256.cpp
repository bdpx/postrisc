#include "util/common.hpp"
#include "u256.hpp"
#include "util.hpp"
#include "float256.hpp"

namespace postrisc {
namespace fpu {

void
u256::mul( const u128 & a, const u128 & b )
{
    u64  z1, z2, carry;

/*      a0a1 x b0b1
        a0b0----
        --a0b1--
        --a1b0--
        ----a1b1
        r0r1r2r3
*/

    mul64To128( a.m1, b.m1, &m2, &m3 );
    mul64To128( a.m0, b.m1, &m1, &z2 );  m2 += z2;  carry  = m2 < z2;
    mul64To128( a.m1, b.m0, &z1, &z2 );  m2 += z2;  carry += m2 < z2;

    m1 += carry;  carry  = m1 < carry;
    m1 += z1;     carry += m1 < z1;

    mul64To128( a.m0, b.m0, &m0, &z1 );  m1 += z1;  carry += m1 < z1;

    m0 += carry;
}

u256
u256::operator + (const u256 & r) const
{
    u64      z0, z1, z2, z3;
    carry_flag    carry;

    z3 = add_init( m3, r.m3, carry );
    z2 = add_next( m2, r.m2, carry );
    z1 = add_next( m1, r.m1, carry );
    z0 = add_last( m0, r.m0, carry );

    return u256(z0, z1, z2, z3);
}

u256
u256::operator - (const u256 & r) const
{
    u64      z0, z1, z2, z3;
    carry_flag    carry;

    z3 = sub_init( m3, r.m3, carry );
    z2 = sub_next( m2, r.m2, carry );
    z1 = sub_next( m1, r.m1, carry );
    z0 = sub_last( m0, r.m0, carry );

    return u256(z0, z1, z2, z3);
}

u256 u256::operator << (int nbits) const
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    return u256( ( m0 << nbits ) | ( m1 >> negshift ),
                 ( m1 << nbits ) | ( m2 >> negshift ),
                 ( m2 << nbits ) | ( m3 >> negshift ),
                 ( m3 << nbits ) );
}

u256 u256::operator >> (int nbits) const
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    return u256( ( m0 >> nbits ),
                 ( m1 >> nbits ) | ( m0 << negshift ),
                 ( m2 >> nbits ) | ( m1 << negshift ),
                 ( m3 >> nbits ) | ( m2 << negshift ) );
}

bool u256::operator == (const u256 & r) const
{
    return ( m0 == r.m0 ) &&
           ( m1 == r.m1 ) &&
           ( m2 == r.m2 ) &&
           ( m3 == r.m3 );
}

bool u256::operator != (const u256 & r) const
{
    return ( m0 != r.m0 ) ||
           ( m1 != r.m1 ) ||
           ( m2 != r.m2 ) ||
           ( m3 != r.m3 );
}

bool u256::operator <  (const u256 & r) const
{
    return               ( ( m0 < r.m0 ) || (
       ( m0 == r.m0 ) && ( ( m1 < r.m1 ) || (
       ( m1 == r.m1 ) && ( ( m2 < r.m2 ) || (
       ( m2 == r.m2 ) && ( ( m3 < r.m3 )
       ) ) ) ) ) ) );
}

bool u256::operator <= (const u256 & r) const
{
    return               ( ( m0 <  r.m0 ) || (
       ( m0 == r.m0 ) && ( ( m1 <  r.m1 ) || (
       ( m1 == r.m1 ) && ( ( m2 <  r.m2 ) || (
       ( m2 == r.m2 ) && ( ( m3 <= r.m3 )
       ) ) ) ) ) ) );
}

void u256::jamming(int count)
{
    if ( count >= 255 ) {
        m3 = ( (m0 | m1 | m2 | m3) != 0);
        m2 = m1 = m0 = 0;
        return;
    }

    while ( count >= 64 ) {
        m3 = m2 | (m3 != 0);
        m2 = m1;
        m1 = m0;
        m0 = 0;
        count -= 64;
    }

    if ( count > 0 ) {
        const int negCount = 64 - count;
        m3 = ( m3 >> count ) | ( m2 << negCount ) | ( ( m3 << negCount ) != 0 );
        m2 = ( m2 >> count ) | ( m1 << negCount );
        m1 = ( m1 >> count ) | ( m0 << negCount );
        m0 = ( m0 >> count );
    }
}

u64 u256::jamming(int count, u64 a4)
{
    if ( count >= 255 + 64 ) {
        a4 = ( (m0 | m1 | m2 | m3 | a4) != 0);
        m0 = m1 = m2 = m3 = 0;
        return a4;
    }

    while ( count >= 64 ) {
        a4 = m3 | (a4 != 0);
        m3 = m2;
        m2 = m1;
        m1 = m0;
        m0 = 0;
        count -= 64;
    }

    if ( count > 0 ) {
        const int negCount = 64 - count;
        a4 = ( a4 >> count ) | ( m3 << negCount ) | ( ( a4 << negCount ) != 0 );
        m3 = ( m3 >> count ) | ( m2 << negCount );
        m2 = ( m2 >> count ) | ( m1 << negCount );
        m1 = ( m1 >> count ) | ( m0 << negCount );
        m0 = ( m0 >> count );
    }

    return a4;
}

std::ostream & operator << (std::ostream & out, const u256 & r)
{
    return out << fmt::hex(r.m0)
        << '_' << fmt::hex(r.m1)
        << '_' << fmt::hex(r.m2)
        << '_' << fmt::hex(r.m3);
}

int
u256::normalize_subnormal(void)
{
    int  count, left, right;

    assert(m0 | m1 | m2 | m3);

    if ( m0 ) {
        count = countLeadingZeros64( m0 ) - f256::BITS_PER_EXPONENT;
        if ( count > 0 ) {
           right = 64 - count;
           m0 = ( m0 << count ) | ( m1 >> right );
           m1 = ( m1 << count ) | ( m2 >> right );
           m2 = ( m2 << count ) | ( m3 >> right );
           m3 = ( m3 << count );
        }
        return 1 - count;
    } else if ( m1 ) {
        count = countLeadingZeros64( m1 ) - f256::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m1 >> right );
            m1 = ( m2 >> right ) | ( m1 << left );
            m2 = ( m3 >> right ) | ( m2 << left );
            m3 =                   ( m3 << left );
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m1 << count ) | ( m2 >> right );
            m1 = ( m2 << count ) | ( m3 >> right );
            m2 = ( m3 << count );
            m3 = 0;
        } else {
            m0 = m1;
            m1 = m2;
            m2 = m3;
            m3 = 0;
        }
        return 1 - 64 - count;
    } else if ( m2 ) {
        count = countLeadingZeros64( m2 ) - f256::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m2 >> right );
            m1 = ( m3 >> right ) | ( m2 << left );
            m2 =                   ( m3 << left );
            m3 = 0;
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m2 << count ) | ( m3 >> right );
            m1 = ( m3 << count );
            m2 = 0;
            m3 = 0;
        } else {
            m0 = m2;
            m1 = m3;
            m2 = 0;
            m3 = 0;
        }
        return 1 - 2*64 - count;
    } else { // if ( m3 )
        assert( m3 );
        count = countLeadingZeros64( m3 ) - f256::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            m0 = m3 >> ( - count );
            m1 = m3 << ( count & 63 );
            m2 = 0;
            m3 = 0;
        } else {
            m0 = m3 << count;
            m1 = 0;
            m2 = 0;
            m3 = 0;
        }
        return 1 - 3*64 - count;
    }
}

} // namespace fpu
} // namespace postrisc
