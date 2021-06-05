#include "util/common.hpp"
#include "u1024.hpp"

namespace postrisc {
namespace fpu {

void
u1024::jamming( int count )
{
    if ( count >= 1023 ) {
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
        int negCount = 64 - count;
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

void
u1024::mul(const u512 & a, const u512 & b)
{
/*      a0a1 x b0b1
        a0b0----
        --a0b1--
        --a1b0--
        ----a1b1
        r0r1r2r3
*/
    u512  z00, z10, z01, z11;
    u32   carry;
    u256  a_lo( a.m4, a.m5, a.m6, a.m7 ),
               b_lo( b.m4, b.m5, b.m6, b.m7 );

    z11.mul( a_lo, b_lo );
    z10.mul( a   , b_lo );
    z01.mul( a_lo, b    );
    z00.mul( a   , b    );

    m15 = z11.m7;
    m14 = z11.m6;
    m13 = z11.m5;
    m12 = z11.m4;

    m11 = z11.m3                                       +  z10.m7;  carry  = m11 < z10.m7;   m11 += z01.m7;   carry += m11 < z01.m7;
    m10 = z11.m2 + carry;   carry = m10 < carry;   m10 += z10.m6;  carry += m10 < z10.m6;   m10 += z01.m6;   carry += m10 < z01.m6;
    m9  = z11.m1 + carry;   carry = m9  < carry;   m9  += z10.m5;  carry += m9  < z10.m5;   m9  += z01.m5;   carry += m9  < z01.m5;
    m8  = z11.m0 + carry;   carry = m8  < carry;   m8  += z10.m4;  carry += m8  < z10.m4;   m8  += z01.m4;   carry += m8  < z01.m4;

    m7  = z00.m7 + carry;   carry = m7  < carry;   m7  += z10.m3;  carry += m7  < z10.m3;   m7  += z01.m3;   carry += m7  < z01.m3;
    m6  = z00.m6 + carry;   carry = m6  < carry;   m6  += z10.m2;  carry += m6  < z10.m2;   m6  += z01.m2;   carry += m6  < z01.m2;
    m5  = z00.m5 + carry;   carry = m5  < carry;   m5  += z10.m1;  carry += m5  < z10.m1;   m5  += z01.m1;   carry += m5  < z01.m1;
    m4  = z00.m4 + carry;   carry = m4  < carry;   m4  += z10.m0;  carry += m4  < z10.m0;   m4  += z01.m0;   carry += m4  < z01.m0;

    m3  = z00.m3 + carry;   carry = m3  < carry;
    m2  = z00.m2 + carry;   carry = m2  < carry;
    m1  = z00.m1 + carry;   carry = m1  < carry;
    m0  = z00.m0 + carry;

}

void
u1024::add(const u1024 & a, const u1024 & b)
{
    carry_flag    carry;

    m15 = add_init( a.m15, b.m15, carry );
    m14 = add_next( a.m14, b.m14, carry );
    m13 = add_next( a.m13, b.m13, carry );
    m12 = add_next( a.m12, b.m12, carry );
    m11 = add_next( a.m11, b.m11, carry );
    m10 = add_next( a.m10, b.m10, carry );
    m9  = add_next( a.m9,  b.m9,  carry );
    m8  = add_next( a.m8,  b.m8,  carry );
    m7  = add_next( a.m7,  b.m7,  carry );
    m6  = add_next( a.m6,  b.m6,  carry );
    m5  = add_next( a.m5,  b.m5,  carry );
    m4  = add_next( a.m4,  b.m4,  carry );
    m3  = add_next( a.m3,  b.m3,  carry );
    m2  = add_next( a.m2,  b.m2,  carry );
    m1  = add_next( a.m1,  b.m1,  carry );
    m0  = add_last( a.m0,  b.m0,  carry );
}

void
u1024::sub(const u1024 & a, const u1024 & b)
{
    carry_flag    carry;

    m15 = sub_init( a.m15, b.m15, carry );
    m14 = sub_next( a.m14, b.m14, carry );
    m13 = sub_next( a.m13, b.m13, carry );
    m12 = sub_next( a.m12, b.m12, carry );
    m11 = sub_next( a.m11, b.m11, carry );
    m10 = sub_next( a.m10, b.m10, carry );
    m9  = sub_next( a.m9,  b.m9,  carry );
    m8  = sub_next( a.m8,  b.m8,  carry );
    m7  = sub_next( a.m7,  b.m7,  carry );
    m6  = sub_next( a.m6,  b.m6,  carry );
    m5  = sub_next( a.m5,  b.m5,  carry );
    m4  = sub_next( a.m4,  b.m4,  carry );
    m3  = sub_next( a.m3,  b.m3,  carry );
    m2  = sub_next( a.m2,  b.m2,  carry );
    m1  = sub_next( a.m1,  b.m1,  carry );
    m0  = sub_last( a.m0,  b.m0,  carry );
}

void
u1024::operator <<= (int nbits)
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    m0  = ( m0  << nbits ) | ( m1  >> negshift );
    m1  = ( m1  << nbits ) | ( m2  >> negshift );
    m2  = ( m2  << nbits ) | ( m3  >> negshift );
    m3  = ( m3  << nbits ) | ( m4  >> negshift );
    m4  = ( m4  << nbits ) | ( m5  >> negshift );
    m5  = ( m5  << nbits ) | ( m6  >> negshift );
    m6  = ( m6  << nbits ) | ( m7  >> negshift );
    m7  = ( m7  << nbits ) | ( m8  >> negshift );
    m8  = ( m8  << nbits ) | ( m9  >> negshift );
    m9  = ( m9  << nbits ) | ( m10 >> negshift );
    m10 = ( m10 << nbits ) | ( m11 >> negshift );
    m11 = ( m11 << nbits ) | ( m12 >> negshift );
    m12 = ( m12 << nbits ) | ( m13 >> negshift );
    m13 = ( m13 << nbits ) | ( m14 >> negshift );
    m14 = ( m14 << nbits ) | ( m15 >> negshift );
    m15 = ( m15 << nbits );
}

bool
u1024::operator < (const u1024 & r) const
{
    return                 ( ( m0  < r.m0  ) || (
       ( m0  == r.m0  ) && ( ( m1  < r.m1  ) || (
       ( m1  == r.m1  ) && ( ( m2  < r.m2  ) || (
       ( m2  == r.m2  ) && ( ( m3  < r.m3  ) || (
       ( m3  == r.m3  ) && ( ( m4  < r.m4  ) || (
       ( m4  == r.m4  ) && ( ( m5  < r.m5  ) || (
       ( m5  == r.m5  ) && ( ( m6  < r.m6  ) || (
       ( m6  == r.m6  ) && ( ( m7  < r.m7  ) || (
       ( m7  == r.m7  ) && ( ( m8  < r.m8  ) || (
       ( m8  == r.m8  ) && ( ( m9  < r.m9  ) || (
       ( m9  == r.m9  ) && ( ( m10 < r.m10 ) || (
       ( m10 == r.m10 ) && ( ( m11 < r.m11 ) || (
       ( m11 == r.m11 ) && ( ( m12 < r.m12 ) || (
       ( m12 == r.m12 ) && ( ( m13 < r.m13 ) || (
       ( m13 == r.m13 ) && ( ( m14 < r.m14 ) || (
       ( m14 == r.m14 ) && ( ( m15 < r.m15 )
       ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) ) );
}

std::ostream &
operator << (std::ostream & out, const u1024 & r)
{
    return out << fmt::hex(r.m0)
        << '_' << fmt::hex(r.m1)
        << '_' << fmt::hex(r.m2)
        << '_' << fmt::hex(r.m3) << '\n'
        << '_' << fmt::hex(r.m4)
        << '_' << fmt::hex(r.m5)
        << '_' << fmt::hex(r.m6)
        << '_' << fmt::hex(r.m7) << '\n'
        << '_' << fmt::hex(r.m8)
        << '_' << fmt::hex(r.m9)
        << '_' << fmt::hex(r.m10)
        << '_' << fmt::hex(r.m11) << '\n'
        << '_' << fmt::hex(r.m12)
        << '_' << fmt::hex(r.m13)
        << '_' << fmt::hex(r.m14)
        << '_' << fmt::hex(r.m15) << '\n';
}

} // namespace fpu
} // namespace postrisc
