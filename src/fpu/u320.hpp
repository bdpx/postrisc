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
    uint64_t   m0, m1, m2, m3, m4;
public:
    explicit u320(void) { }
    explicit u320(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3, uint64_t x4)
             : m0(x0), m1(x1), m2(x2), m3(x3), m4(x4) { }

    operator bool (void) const { return ( ( m0 | m1 | m2 | m3 | m4 ) != 0 ); }

    void mul( const u256 & a, uint64_t b );

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
