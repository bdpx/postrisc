#pragma once

// This macro tests for minimum version of the GNU C++ compiler.
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
# define SOFTFLOAT_GNUC_PREREQ(maj, min)  ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
# define SOFTFLOAT_GNUC_PREREQ(maj, min)  0
#endif

namespace postrisc {
namespace fpu {

/*----------------------------------------------------------------------------
| Multiplies `a' by `b' to obtain a 128-bit product.  The product is broken
| into two 64-bit pieces which are stored at the locations pointed to by
| `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/
inline void mul64To128( uint64_t a, uint64_t b, uint64_t *z0Ptr, uint64_t *z1Ptr )
{
#if defined(__x86_64__)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

    typedef unsigned __int128 UINT128;
    UINT128 tmp = UINT128(a) * b;
    *z1Ptr = uint64_t(tmp);
    *z0Ptr = uint64_t(tmp >> 64);

#pragma GCC diagnostic pop

#else
    uint32_t a_high, a_low, b_high, b_low;
    uint64_t z0, zMiddleA, zMiddleB, z1;

    a_low = static_cast<uint32_t>(a);
    a_high = a>>32;
    b_low = static_cast<uint32_t>(b);
    b_high = b>>32;

    z1 = uint64_t( a_low ) * b_low;
    zMiddleA = uint64_t( a_low ) * b_high;
    zMiddleB = uint64_t( a_high ) * b_low;
    z0 = uint64_t( a_high ) * b_high;

    zMiddleA += zMiddleB;
    z0 += ( ( uint64_t( zMiddleA < zMiddleB ) )<<32 ) + ( zMiddleA>>32 );
    zMiddleA <<= 32;
    z1 += zMiddleA;
    z0 += ( z1 < zMiddleA );
    *z1Ptr = z1;
    *z0Ptr = z0;
#endif
}

/*----------------------------------------------------------------------------
| Returns the number of leading 0 bits before the most-significant 1 bit of
| `a'.  If `a' is zero, 32 is returned.
*----------------------------------------------------------------------------*/

inline int countLeadingZeros32( uint32_t a )
{
#if SOFTFLOAT_GNUC_PREREQ(3, 4)
    assert( a );
    return __builtin_clz(a);
/*    if (a) {
        return __builtin_clz(a);
    } else {
        return 32;
    }
*/
#else
    static const int8_t countLeadingZerosHigh[] = {
        8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4,
        3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    int shiftCount = 0;
    if ( a < 0x10000 ) {
        shiftCount += 16;
        a <<= 16;
    }
    if ( a < 0x1000000 ) {
        shiftCount += 8;
        a <<= 8;
    }
    shiftCount += countLeadingZerosHigh[ a>>24 ];
    return shiftCount;
#endif
}

/*----------------------------------------------------------------------------
| Returns the number of leading 0 bits before the most-significant 1 bit of
| `a'.  If `a' is zero, 64 is returned.
*----------------------------------------------------------------------------*/

inline int countLeadingZeros64( uint64_t a )
{
#if SOFTFLOAT_GNUC_PREREQ(3, 4)
    assert( a );
    return __builtin_clzll(a);
/*    if (a) {
        return __builtin_clzll(a);
    } else {
        return 64;
    }
*/
#else
    int shiftCount = 0;
    if ( a < (uint64_t(1) << 32) ) {
        shiftCount += 32;
    }
    else {
        a >>= 32;
    }
    shiftCount += countLeadingZeros32(static_cast<uint32_t>(a));
    return shiftCount;
#endif
}

} // namespace fpu
} // namespace postrisc