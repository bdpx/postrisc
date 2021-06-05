#pragma once

#include "util/common.hpp"
#include "bigint.hpp"
#include "u512.hpp"

namespace postrisc {
namespace fpu {

/***************************************************************************
* 576 bit integer (9x64)
* used for 512-bit div/rem/sqrt
***************************************************************************/
class u576 {
public:
    u64   m0, m1, m2, m3, m4, m5, m6, m7, m8;
public:
    explicit u576(void) { }
    explicit u576(u64 x0, u64 x1, u64 x2, u64 x3,
                  u64 x4, u64 x5, u64 x6, u64 x7,
                  u64 x8)
             : m0(x0), m1(x1), m2(x2), m3(x3), m4(x4), m5(x5), m6(x6), m7(x7), m8(x8) { }

    void mul( const u512 & a, u64 b );

    operator bool (void) const { return ( ( m0 | m1 | m2 | m3 | m4 | m5 | m6 | m7 | m8 ) != 0 ); }

    u576 operator + (const u576 & b) const;
    void operator += (const u576 & r) { *this = *this + r; }
    void operator += (const u512 & r) { *this += u576(0, r.m0, r.m1, r.m2, r.m3, r.m4, r.m5, r.m6, r.m7);  }

    u576 operator - (const u576 & b) const;
    void operator -= (const u576 & r) { *this = *this - r; }

    u576 operator << (int nbits) const;
    void operator <<= (int nbits);
    friend std::ostream & operator << (std::ostream & out, const u576 & r);
};


} // namespace fpu
} // namespace postrisc
