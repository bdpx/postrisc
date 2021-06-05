#include "util/common.hpp"
#include "u320.hpp"
#include "util.hpp"

namespace postrisc {
namespace fpu {

void
u320::mul( const u256 & a, u64 b )
{
    u64  z1, carry;

/*      a0a1a2a3 x b0
        a0b0------
        --a1b0----
        ----a2b0--
        ------a3b0
        r0r1r2r3r4
*/

    mul64To128( a.m3, b, &m3, &m4 );
    mul64To128( a.m2, b, &m2, &z1 );   m3 += z1;   carry  = m3 < z1;   m2 += carry;   carry = m2 < carry;
    mul64To128( a.m1, b, &m1, &z1 );   m2 += z1;   carry += m2 < z1;   m1 += carry;   carry = m1 < carry;
    mul64To128( a.m0, b, &m0, &z1 );   m1 += z1;   carry += m1 < z1;   m0 += carry;
}

u320
u320::operator + (const u320 & r) const
{
    u64           z0, z1, z2, z3, z4;
    carry_flag    carry;

    z4 = add_init( m4, r.m4, carry );
    z3 = add_next( m3, r.m3, carry );
    z2 = add_next( m2, r.m2, carry );
    z1 = add_next( m1, r.m1, carry );
    z0 = add_last( m0, r.m0, carry );

    return u320(z0, z1, z2, z3, z4);
}

u320
u320::operator - (const u320 & r) const
{
    u64           z0, z1, z2, z3, z4;
    carry_flag    carry;

    z4 = sub_init( m4, r.m4, carry );
    z3 = sub_next( m3, r.m3, carry );
    z2 = sub_next( m2, r.m2, carry );
    z1 = sub_next( m1, r.m1, carry );
    z0 = sub_last( m0, r.m0, carry );

    return u320(z0, z1, z2, z3, z4);
}

u320
u320::operator << (int nbits) const
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    return u320(
        ( m0 << nbits ) | ( m1 >> negshift ),
        ( m1 << nbits ) | ( m2 >> negshift ),
        ( m2 << nbits ) | ( m3 >> negshift ),
        ( m3 << nbits ) | ( m4 >> negshift ),
        ( m4 << nbits )
     );
}

void
u320::operator <<= (int nbits)
{
    assert(nbits > 0);
    assert(nbits < 64);

    const int negshift = 64 - nbits;

    m0 = ( m0 << nbits ) | ( m1 >> negshift );
    m1 = ( m1 << nbits ) | ( m2 >> negshift );
    m2 = ( m2 << nbits ) | ( m3 >> negshift );
    m3 = ( m3 << nbits ) | ( m4 >> negshift );
    m4 = ( m4 << nbits );
}

std::ostream &
operator << (std::ostream & out, const u320 & r)
{
    return out << fmt::hex(r.m0)
        << '_' << fmt::hex(r.m1)
        << '_' << fmt::hex(r.m2)
        << '_' << fmt::hex(r.m3)
        << '_' << fmt::hex(r.m4);
}

} // namespace fpu
} // namespace postrisc
