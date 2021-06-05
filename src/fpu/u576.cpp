#include "util/common.hpp"
#include "u576.hpp"
#include "util.hpp"

namespace postrisc {
namespace fpu {

u576
u576::operator + (const u576 & b) const
{
    u64        z0, z1, z2, z3, z4, z5, z6, z7, z8;
    carry_flag    carry;

    z8 = add_init( m8, b.m8, carry );
    z7 = add_next( m7, b.m7, carry );
    z6 = add_next( m6, b.m6, carry );
    z5 = add_next( m5, b.m5, carry );
    z4 = add_next( m4, b.m4, carry );
    z3 = add_next( m3, b.m3, carry );
    z2 = add_next( m2, b.m2, carry );
    z1 = add_next( m1, b.m1, carry );
    z0 = add_last( m0, b.m0, carry );

    return u576(z0, z1, z2, z3, z4, z5, z6, z7, z8);
}

u576
u576::operator - (const u576 & b) const
{
    u64        z0, z1, z2, z3, z4, z5, z6, z7, z8;
    carry_flag    carry;

    z8 = sub_init( m8, b.m8, carry );
    z7 = sub_next( m7, b.m7, carry );
    z6 = sub_next( m6, b.m6, carry );
    z5 = sub_next( m5, b.m5, carry );
    z4 = sub_next( m4, b.m4, carry );
    z3 = sub_next( m3, b.m3, carry );
    z2 = sub_next( m2, b.m2, carry );
    z1 = sub_next( m1, b.m1, carry );
    z0 = sub_last( m0, b.m0, carry );

    return u576(z0, z1, z2, z3, z4, z5, z6, z7, z8);
}

u576
u576::operator << (int nbits) const
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    return u576(
        ( m0 << nbits ) | ( m1 >> negshift ),
        ( m1 << nbits ) | ( m2 >> negshift ),
        ( m2 << nbits ) | ( m3 >> negshift ),
        ( m3 << nbits ) | ( m4 >> negshift ),
        ( m4 << nbits ) | ( m5 >> negshift ),
        ( m5 << nbits ) | ( m6 >> negshift ),
        ( m6 << nbits ) | ( m7 >> negshift ),
        ( m7 << nbits ) | ( m8 >> negshift ),
        ( m8 << nbits )
      );
}

void
u576::operator <<= (int nbits)
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
    m7 = ( m7 << nbits ) | ( m8 >> negshift );
    m8 = ( m8 << nbits );
}

std::ostream &
operator << (std::ostream & out, const u576 & r)
{
    return out << fmt::hex(r.m0)
        << '_' << fmt::hex(r.m1)
        << '_' << fmt::hex(r.m2)
        << '_' << fmt::hex(r.m3)
        << '_' << fmt::hex(r.m4)
        << '_' << fmt::hex(r.m5)
        << '_' << fmt::hex(r.m6)
        << '_' << fmt::hex(r.m7)
        << '_' << fmt::hex(r.m8);
}

void
u576::mul( const u512 & a, u64 b )
{
    u64  z1, carry;

/*      a0a1a2a3a4a5a6a7 x
                      b0
        a0b0--------------
        --a1b0------------
        ----a2b0----------
        ------a3b0--------
        --------a4b0------
        ----------a5b0----
        ------------a6b0--
        --------------a7b0
        r0r1r2r3r4r5r6r7r8
*/

    mul64To128( a.m7, b, &m7, &m8 );
    mul64To128( a.m6, b, &m6, &z1 );   m7 += z1;   carry  = m7 < z1;   m6 += carry;   carry = m6 < carry;
    mul64To128( a.m5, b, &m5, &z1 );   m6 += z1;   carry += m6 < z1;   m5 += carry;   carry = m5 < carry;
    mul64To128( a.m4, b, &m4, &z1 );   m5 += z1;   carry += m5 < z1;   m4 += carry;   carry = m4 < carry;
    mul64To128( a.m3, b, &m3, &z1 );   m4 += z1;   carry += m4 < z1;   m3 += carry;   carry = m3 < carry;
    mul64To128( a.m2, b, &m2, &z1 );   m3 += z1;   carry += m3 < z1;   m2 += carry;   carry = m2 < carry;
    mul64To128( a.m1, b, &m1, &z1 );   m2 += z1;   carry += m2 < z1;   m1 += carry;   carry = m1 < carry;
    mul64To128( a.m0, b, &m0, &z1 );   m1 += z1;   carry += m1 < z1;   m0 += carry;

}

} // namespace fpu
} // namespace postrisc
