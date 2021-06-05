#pragma once

#include "u128.hpp"

namespace postrisc {
namespace fpu {

/***************************************************************************
* 192 bit fraction
* used for 128-bit div/rem/sqrt, 80-bit muladd
***************************************************************************/
class u192 {
public:
    uint64_t  m0, m1, m2;
public:
    explicit u192(void) {}
    explicit u192(uint64_t x0, uint64_t x1, uint64_t x2)
             : m0(x0), m1(x1), m2(x2) {}

    operator bool (void) const { return ( ( m0 | m1 | m2 ) != 0 ); }

    void mul( const u128 & a, uint64_t b );

    u192 operator + (const u192 & r) const
    {
        carry_flag  carry;

        uint64_t const z2 = add_init( m2, r.m2, carry );
        uint64_t const z1 = add_next( m1, r.m1, carry );
        uint64_t const z0 = add_last( m0, r.m0, carry );

        return u192(z0, z1, z2);
    }

    void operator += (const u192 & r) {  *this = *this + r;  }
    void operator += (const u128 & r) {  *this += u192(0, r.m0, r.m1);  }

    u192 operator - (const u192 & r) const
    {
        carry_flag   carry;

        uint64_t const z2 = sub_init( m2, r.m2, carry );
        uint64_t const z1 = sub_next( m1, r.m1, carry );
        uint64_t const z0 = sub_last( m0, r.m0, carry );

        return u192(z0, z1, z2);
    }

    void operator -= (const u192 & r) {  *this = *this - r;  }

    u192 operator << (int nbits) const
    {
        assert(nbits > 0);
        assert(nbits < 64);

        int negshift = 64 - nbits;

        return u192( ( m0 << nbits ) | ( m1 >> negshift ),
                        ( m1 << nbits ) | ( m2 >> negshift ),
                        ( m2 << nbits ) );
    }

    void operator <<= (int nbits)
    {
        assert(nbits > 0);
        assert(nbits < 64);

        int negshift = 64 - nbits;

        m0 = ( m0 << nbits ) | ( m1 >> negshift );
        m1 = ( m1 << nbits ) | ( m2 >> negshift );
        m2 = ( m2 << nbits );
    }

    void jamming(int count);

    friend std::ostream & operator << (std::ostream & out, const u192 & r);

};

} // namespace fpu
} // namespace postrisc
