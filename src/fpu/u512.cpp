#include "util/common.hpp"
#include "util.hpp"
#include "u512.hpp"
#include "float512.hpp"

namespace postrisc {
namespace fpu {

void
u512::mul(const u256 & a, const u256 & b)
{
/*      a0a1 x b0b1
        a0b0----
        --a0b1--
        --a1b0--
        ----a1b1
        r0r1r2r3
*/
    u256  z00, z10, z01, z11;
    u32   carry;
    u128  a_hi( a.m0, a.m1 ), a_lo( a.m2, a.m3 ),
             b_hi( b.m0, b.m1 ), b_lo( b.m2, b.m3 );

    z11.mul( a_lo, b_lo );
    z10.mul( a_hi, b_lo );
    z01.mul( a_lo, b_hi );
    z00.mul( a_hi, b_hi );

    m7 = z11.m3;
    m6 = z11.m2;
    m5 = z11.m1                                      + z10.m3;  carry  = m5 < z10.m3;   m5 += z01.m3;   carry += m5 < z01.m3;
    m4 = z11.m0 + carry;   carry = m4 < carry;   m4 += z10.m2;  carry += m4 < z10.m2;   m4 += z01.m2;   carry += m4 < z01.m2;
    m3 = z00.m3 + carry;   carry = m3 < carry;   m3 += z10.m1;  carry += m3 < z10.m1;   m3 += z01.m1;   carry += m3 < z01.m1;
    m2 = z00.m2 + carry;   carry = m2 < carry;   m2 += z10.m0;  carry += m2 < z10.m0;   m2 += z01.m0;   carry += m2 < z01.m0;
    m1 = z00.m1 + carry;   carry = m1 < carry;
    m0 = z00.m0 + carry;
}

void u512::add(const u512 & a, const u512 & b)
{
    carry_flag    carry;

    m7 = add_init( a.m7, b.m7, carry );
    m6 = add_next( a.m6, b.m6, carry );
    m5 = add_next( a.m5, b.m5, carry );
    m4 = add_next( a.m4, b.m4, carry );
    m3 = add_next( a.m3, b.m3, carry );
    m2 = add_next( a.m2, b.m2, carry );
    m1 = add_next( a.m1, b.m1, carry );
    m0 = add_last( a.m0, b.m0, carry );
}

void u512::sub(const u512 & a, const u512 & b)
{
    carry_flag    carry;

    m7 = sub_init( a.m7, b.m7, carry );
    m6 = sub_next( a.m6, b.m6, carry );
    m5 = sub_next( a.m5, b.m5, carry );
    m4 = sub_next( a.m4, b.m4, carry );
    m3 = sub_next( a.m3, b.m3, carry );
    m2 = sub_next( a.m2, b.m2, carry );
    m1 = sub_next( a.m1, b.m1, carry );
    m0 = sub_last( a.m0, b.m0, carry );
}

u512 u512::operator << (int nbits) const
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    return u512(
        ( m0 << nbits ) | ( m1 >> negshift ),
        ( m1 << nbits ) | ( m2 >> negshift ),
        ( m2 << nbits ) | ( m3 >> negshift ),
        ( m3 << nbits ) | ( m4 >> negshift ),
        ( m4 << nbits ) | ( m5 >> negshift ),
        ( m5 << nbits ) | ( m6 >> negshift ),
        ( m6 << nbits ) | ( m7 >> negshift ),
        ( m7 << nbits )
     );
}

void u512::operator <<= (int nbits)
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    m0 = ( m0 << nbits ) | ( m1 >> negshift );
    m1 = ( m1 << nbits ) | ( m2 >> negshift );
    m2 = ( m2 << nbits ) | ( m3 >> negshift );
    m3 = ( m3 << nbits ) | ( m4 >> negshift );
    m4 = ( m4 << nbits ) | ( m5 >> negshift );
    m5 = ( m5 << nbits ) | ( m6 >> negshift );
    m6 = ( m6 << nbits ) | ( m7 >> negshift );
    m7 = ( m7 << nbits );
}

u512 u512::operator >> (int nbits) const
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    return u512(
        ( m0 >> nbits ),
        ( m1 >> nbits ) | ( m0 << negshift ),
        ( m2 >> nbits ) | ( m1 << negshift ),
        ( m3 >> nbits ) | ( m2 << negshift ),
        ( m4 >> nbits ) | ( m3 << negshift ),
        ( m5 >> nbits ) | ( m4 << negshift ),
        ( m6 >> nbits ) | ( m5 << negshift ),
        ( m7 >> nbits ) | ( m6 << negshift )
     );
}

void u512::operator >>= (int nbits)
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    m7 = ( m7 >> nbits ) | ( m6 << negshift );
    m6 = ( m6 >> nbits ) | ( m5 << negshift );
    m5 = ( m5 >> nbits ) | ( m4 << negshift );
    m4 = ( m4 >> nbits ) | ( m3 << negshift );
    m3 = ( m3 >> nbits ) | ( m2 << negshift );
    m2 = ( m2 >> nbits ) | ( m1 << negshift );
    m1 = ( m1 >> nbits ) | ( m0 << negshift );
    m0 = ( m0 >> nbits );
}

bool
u512::operator == (const u512 & r) const
{
    return (m0 == r.m0) &&
           (m1 == r.m1) &&
           (m2 == r.m2) &&
           (m3 == r.m3) &&
           (m4 == r.m4) &&
           (m5 == r.m5) &&
           (m6 == r.m6) &&
           (m7 == r.m7);
}

bool
u512::operator != (const u512 & r) const
{
    return (m0 != r.m0) ||
           (m1 != r.m1) ||
           (m2 != r.m2) ||
           (m3 != r.m3) ||
           (m4 != r.m4) ||
           (m5 != r.m5) ||
           (m6 != r.m6) ||
           (m7 != r.m7);
}

bool
u512::operator <  (const u512 & r) const
{
    return               ( ( m0 < r.m0 ) || (
       ( m0 == r.m0 ) && ( ( m1 < r.m1 ) || (
       ( m1 == r.m1 ) && ( ( m2 < r.m2 ) || (
       ( m2 == r.m2 ) && ( ( m3 < r.m3 ) || (
       ( m3 == r.m3 ) && ( ( m4 < r.m4 ) || (
       ( m4 == r.m4 ) && ( ( m5 < r.m5 ) || (
       ( m5 == r.m5 ) && ( ( m6 < r.m6 ) || (
       ( m6 == r.m6 ) && ( ( m7 < r.m7 )
       ) ) ) ) ) ) ) ) ) ) ) ) ) ) );
}

bool
u512::operator <= (const u512 & r) const
{
    return               ( ( m0 <  r.m0 ) || (
       ( m0 == r.m0 ) && ( ( m1 <  r.m1 ) || (
       ( m1 == r.m1 ) && ( ( m2 <  r.m2 ) || (
       ( m2 == r.m2 ) && ( ( m3 <  r.m3 ) || (
       ( m3 == r.m3 ) && ( ( m4 <  r.m4 ) || (
       ( m4 == r.m4 ) && ( ( m5 <  r.m5 ) || (
       ( m5 == r.m5 ) && ( ( m6 <  r.m6 ) || (
       ( m6 == r.m6 ) && ( ( m7 <= r.m7 )
       ) ) ) ) ) ) ) ) ) ) ) ) ) ) );
}

void
u512::jamming( int count )
{
    if ( count >= 511 ) {
        m7 = ( (m0 | m1 | m2 | m3 | m4 | m5 | m6 | m7) != 0);
        m6 = m5 = m4 = m3 = m2 = m1 = m0 = 0;
        return;
    }

    while ( count >= 64 ) {
        m7 = m6 | (m7 != 0);
        m6 = m5;
        m5 = m4;
        m4 = m3;
        m3 = m2;
        m2 = m1;
        m1 = m0;
        m0 = 0;
        count -= 64;
    }

    if ( count > 0 ) {
        const int negCount = 64 - count;
        m7 = ( m7 >> count ) | ( m6 << negCount ) | ( ( m7 << negCount ) != 0 );
        m6 = ( m6 >> count ) | ( m5 << negCount );
        m5 = ( m5 >> count ) | ( m4 << negCount );
        m4 = ( m4 >> count ) | ( m3 << negCount );
        m3 = ( m3 >> count ) | ( m2 << negCount );
        m2 = ( m2 >> count ) | ( m1 << negCount );
        m1 = ( m1 >> count ) | ( m0 << negCount );
        m0 = ( m0 >> count );
    }
}

u64
u512::jamming( int count, u64 a8 )
{
    if ( count >= 511 + 64 ) {
        a8 = ( (m0 | m1 | m2 | m3 | m4 | m5 | m6 | m7 | a8) != 0);
        clear();
        return a8;
    }

    while ( count >= 64 ) {
        a8 = m7 | (a8 != 0);
        m7 = m6;
        m6 = m5;
        m5 = m4;
        m4 = m3;
        m3 = m2;
        m2 = m1;
        m1 = m0;
        m0 = 0;
        count -= 64;
    }

    if ( count > 0 ) {
        const int negCount = 64 - count;
        a8 = ( a8 >> count ) | ( m7 << negCount ) | ( ( a8 << negCount ) != 0 );
        m7 = ( m7 >> count ) | ( m6 << negCount );
        m6 = ( m6 >> count ) | ( m5 << negCount );
        m5 = ( m5 >> count ) | ( m4 << negCount );
        m4 = ( m4 >> count ) | ( m3 << negCount );
        m3 = ( m3 >> count ) | ( m2 << negCount );
        m2 = ( m2 >> count ) | ( m1 << negCount );
        m1 = ( m1 >> count ) | ( m0 << negCount );
        m0 = ( m0 >> count );
    }

    return a8;
}

std::ostream &
operator << (std::ostream & out, const u512 & r)
{
    return out << fmt::hex(r.m0)
        << '_' << fmt::hex(r.m1)
        << '_' << fmt::hex(r.m2)
        << '_' << fmt::hex(r.m3)
        << '_' << fmt::hex(r.m4)
        << '_' << fmt::hex(r.m5)
        << '_' << fmt::hex(r.m6)
        << '_' << fmt::hex(r.m7);
}

int
u512::normalize_subnormal( void )
{
    int  count, left, right;

    assert( m0 | m1 | m2 | m3 | m4 | m5 | m6 | m7 );

    if ( m0 ) {

        count = countLeadingZeros64( m0 ) - f512::BITS_PER_EXPONENT;
        if ( count > 0 ) {
           right = 64 - count;
           m0 = ( m0 << count ) | ( m1 >> right );
           m1 = ( m1 << count ) | ( m2 >> right );
           m2 = ( m2 << count ) | ( m3 >> right );
           m3 = ( m3 << count ) | ( m4 >> right );
           m4 = ( m4 << count ) | ( m5 >> right );
           m5 = ( m5 << count ) | ( m6 >> right );
           m6 = ( m6 << count ) | ( m7 >> right );
           m7 = ( m7 << count );
        }
        // if count < 0 don't jam here
        return 1 - count;

    } else if ( m1 ) {

        count = countLeadingZeros64( m1 ) - f512::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m1 >> right );
            m1 = ( m2 >> right ) | ( m1 << left );
            m2 = ( m3 >> right ) | ( m2 << left );
            m3 = ( m4 >> right ) | ( m3 << left );
            m4 = ( m5 >> right ) | ( m4 << left );
            m5 = ( m6 >> right ) | ( m5 << left );
            m6 = ( m7 >> right ) | ( m6 << left );
            m7 =                   ( m7 << left );
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m1 << count ) | ( m2 >> right );
            m1 = ( m2 << count ) | ( m3 >> right );
            m2 = ( m3 << count ) | ( m4 >> right );
            m3 = ( m4 << count ) | ( m5 >> right );
            m4 = ( m5 << count ) | ( m6 >> right );
            m5 = ( m6 << count ) | ( m7 >> right );
            m6 = ( m7 << count );
            m7 = 0;
        } else {
            m0 = m1;
            m1 = m2;
            m2 = m3;
            m3 = m4;
            m4 = m5;
            m5 = m6;
            m6 = m7;
            m7 = 0;
        }
        return 1 - 64 - count;

    } else if ( m2 ) {

        count = countLeadingZeros64( m2 ) - f512::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m2 >> right );
            m1 = ( m3 >> right ) | ( m2 << left );
            m2 = ( m4 >> right ) | ( m3 << left );
            m3 = ( m5 >> right ) | ( m4 << left );
            m4 = ( m6 >> right ) | ( m5 << left );
            m5 = ( m7 >> right ) | ( m6 << left );
            m6 =                   ( m7 << left );
            m7 = 0;
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m2 << count ) | ( m3 >> right );
            m1 = ( m3 << count ) | ( m4 >> right );
            m2 = ( m4 << count ) | ( m5 >> right );
            m3 = ( m5 << count ) | ( m6 >> right );
            m4 = ( m6 << count ) | ( m7 >> right );
            m5 = ( m7 << count );
            m6 = m7 = 0;
        } else {
            m0 = m2;
            m1 = m3;
            m2 = m4;
            m3 = m5;
            m4 = m6;
            m5 = m7;
            m6 = m7 = 0;
        }
        return 1 - 2*64 - count;

    } else if ( m3 ) {

        count = countLeadingZeros64( m3 ) - f512::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m3 >> right );
            m1 = ( m4 >> right ) | ( m3 << left );
            m2 = ( m5 >> right ) | ( m4 << left );
            m3 = ( m6 >> right ) | ( m5 << left );
            m4 = ( m7 >> right ) | ( m6 << left );
            m5 =                   ( m7 << left );
            m6 = m7 = 0;
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m3 << count ) | ( m4 >> right );
            m1 = ( m4 << count ) | ( m5 >> right );
            m2 = ( m5 << count ) | ( m6 >> right );
            m3 = ( m6 << count ) | ( m7 >> right );
            m4 = ( m7 << count );
            m5 = m6 = m7 = 0;
        } else {
            m0 = m3;
            m1 = m4;
            m2 = m5;
            m3 = m6;
            m4 = m7;
            m5 = m6 = m7 = 0;
        }
        return 1 - 3*64 - count;

    } else if ( m4 ) {

        count = countLeadingZeros64( m4 ) - f512::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m4 >> right );
            m1 = ( m5 >> right ) | ( m4 << left );
            m2 = ( m6 >> right ) | ( m5 << left );
            m3 = ( m7 >> right ) | ( m6 << left );
            m4 =                   ( m7 << left );
            m5 = m6 = m7 = 0;
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m4 << count ) | ( m5 >> right );
            m1 = ( m5 << count ) | ( m6 >> right );
            m2 = ( m6 << count ) | ( m7 >> right );
            m3 = ( m7 << count );
            m4 = m5 = m6 = m7 = 0;
        } else {
            m0 = m4;
            m1 = m5;
            m2 = m6;
            m3 = m7;
            m4 = m5 = m6 = m7 = 0;
        }
        return 1 - 4*64 - count;

    } else if ( m5 ) {

        count = countLeadingZeros64( m5 ) - f512::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m5 >> right );
            m1 = ( m6 >> right ) | ( m5 << left );
            m2 = ( m7 >> right ) | ( m6 << left );
            m3 =                   ( m7 << left );
            m4 = m5 = m6 = m7 = 0;
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m5 << count ) | ( m6 >> right );
            m1 = ( m6 << count ) | ( m7 >> right );
            m2 = ( m7 << count );
            m3 = m4 = m5 = m6 = m7 = 0;
        } else {
            m0 = m5;
            m1 = m6;
            m2 = m7;
            m3 = m4 = m5 = m6 = m7 = 0;
        }
        return 1 - 5*64 - count;

    } else if ( m6 ) {

        count = countLeadingZeros64( m6 ) - f512::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            right  = - count;
            left   = 64 - right;
            m0 = ( m6 >> right );
            m1 = ( m7 >> right ) | ( m6 << left );
            m2 =                   ( m7 << left );
            m3 = m4 = m5 = m6 = m7 = 0;
        } else if ( count > 0 ) {
            right = 64 - count;
            m0 = ( m6 << count ) | ( m7 >> right );
            m1 = ( m7 << count );
            m2 = m3 = m4 = m5 = m6 = m7 = 0;
        } else {
            m0 = m6;
            m1 = m7;
            m2 = m3 = m4 = m5 = m6 = m7 = 0;
        }
        return 1 - 6*64 - count;

    } else { // if ( m7 )

        assert( m7 );
        count = countLeadingZeros64( m7 ) - f512::BITS_PER_EXPONENT;
        if ( count < 0 ) {
            m0 = m7 >> ( - count );
            m1 = m7 << ( count & 63 );
            m2 = m3 = m4 = m5 = m6 = m7 = 0;
        } else {
            m0 = m7 << count;
            m1 = m2 = m3 = m4 = m5 = m6 = m7 = 0;
        }
        return 1 - 7*64 - count;

    }
}

} // namespace fpu
} // namespace postrisc
