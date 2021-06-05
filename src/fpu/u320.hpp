#pragma once

#include "u256.hpp"

namespace postrisc {
namespace fpu {

/***************************************************************************
* 320 bit integer
* used for f256-bit div/rem/sqrt
***************************************************************************/
class u320 {  // 5x64
public:
    u64   m0, m1, m2, m3, m4;
public:
    explicit u320(void) { }
    explicit u320(u64 x0, u64 x1, u64 x2, u64 x3, u64 x4)
             : m0(x0), m1(x1), m2(x2), m3(x3), m4(x4) { }

    operator bool (void) const { return ( ( m0 | m1 | m2 | m3 | m4 ) != 0 ); }

    void mul( const u256 & a, u64 b );

    u320 operator + (const u320 & r) const;
    u320 operator - (const u320 & r) const;

    void operator += (const u320 & r) { *this = *this + r; }
    void operator -= (const u320 & r) { *this = *this - r; }
    void operator += (const u256 & r) { *this += u320(0, r.m0, r.m1, r.m2, r.m3);  }

    u320 operator << (int nbits) const;
    void operator <<= (int nbits);

    friend std::ostream & operator << (std::ostream & out, const u320 & r);
};

} // namespace fpu
} // namespace postrisc
