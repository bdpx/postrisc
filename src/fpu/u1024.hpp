#pragma once

#include "bigint.hpp"
#include "u512.hpp"

namespace postrisc {
namespace fpu {

/***************************************************************************
* 1024 bit integer
* used for 512-bit mul/muladd
***************************************************************************/
class u1024 : public u512 {  // 16x64
public:
    uint64_t   m8, m9, m10, m11,
               m12, m13, m14, m15;
public:
    explicit u1024(void) { }
    explicit u1024(uint64_t x0, uint64_t x1, uint64_t x2, uint64_t x3,
                   uint64_t x4, uint64_t x5, uint64_t x6, uint64_t x7,
                   uint64_t x8, uint64_t x9, uint64_t x10, uint64_t x11,
                   uint64_t x12, uint64_t x13, uint64_t x14, uint64_t x15)
             : u512(x0, x1, x2, x3, x4, x5, x6, x7),
               m8(x8), m9(x9), m10(x10), m11(x11),
               m12(x12), m13(x13), m14(x14), m15(x15) { }

    void mul( const u512 & a, const u512 & b );

    void add ( const u1024 & a, const u1024 & b );
    void add ( const u1024 & b ) { add(*this, b); }

    void sub (const u1024 & a, const u1024 & b);
    void sub (const u1024 & b) { sub(*this, b); }

    void operator <<= (int nbits);
    bool operator <  (const u1024 & r) const;

    void jamming( int count );

    uint64_t extra_jamming(void) const { return m8 | ( ( m9 | m10 | m11 | m12 | m13 | m14 | m15 ) != 0 ); }

    friend std::ostream & operator << (std::ostream & out, const u1024 & r);
};

} // namespace fpu
} // namespace postrisc
