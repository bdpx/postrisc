#pragma once

#include "float.hpp"

namespace postrisc {
namespace fpu {

/*
#define FLT128_MAX 1.18973149535723176508575932662800702e4932Q
#define FLT128_MIN 3.36210314311209350626267781732175260e-4932Q
#define FLT128_EPSILON 1.92592994438723585305597794258492732e-34Q
#define FLT128_DENORM_MIN 6.475175119438025110924438958227646552e-4966Q
#define FLT128_MANT_DIG 113
#define FLT128_MIN_EXP (-16381)
#define FLT128_MAX_EXP 16384
#define FLT128_DIG 33
#define FLT128_MIN_10_EXP (-4931)
#define FLT128_MAX_10_EXP 4932
#define FLT128_BIAS 0x3fff
*/

/*****************************************************************************
 jamming, simple (or with extra parameter):
 Shifts the XXXX (or XXXX.YY) value formed by concatenating XXXX & YY right
 by 64 _plus_ the number of bits given in `count'.  The shifted result is
 at most XXXX nonzero bits; these are broken into two 64-bit pieces which are
 stored at the locations pointed to by `z0Ptr' and `z1Ptr'.  The bits shifted
 off form a third 64-bit result as follows:  The _last_ bit shifted off is
 the most-significant bit of the extra result, and the other 63 bits of the
 extra result are all zero if and only if _all_but_the_last_ bits shifted off
 were all zero.  This extra result is stored in the location pointed to by
 `z2Ptr'.  The value of `count' can be arbitrarily large.
     (This routine makes more sense if XXXX, and YY are considered
 to form a fixed-point value with binary point between XXXX and YY.  This
 fixed-point value is shifted right by the number of bits given in `count',
 and the integer part of the result is returned at XXXX.
 The fractional part of the result may be slightly corrupted as described above,
 and is returned explicit)
******************************************************************************/

/*****************************************************************************************
| The patterns for a known const
******************************************************************************************/
#define f32_zero       F32_C(0x00000000)
#define f32_one        F32_C(0x3f800000)
#define f32_ln2        F32_C(0x3f317218)
#define f32_pi         F32_C(0x40490fdb)
#define f32_half       F32_C(0x3f000000)
#define f32_infinity   F32_C(0x7f800000)

#define f64_zero       F64_C(0x0000000000000000)
#define f64_one        F64_C(0x3ff0000000000000)
#define f64_ln2        F64_C(0x3fe62e42fefa39ef)
#define f64_pi         F64_C(0x400921fb54442d18)
#define f64_half       F64_C(0x3fe0000000000000)
#define f64_infinity   F64_C(0x7ff0000000000000) 

#define f80_zero      F80_C(0x0000, 0x0000000000000000)
#define f80_one       F80_C(0x3fff, 0x8000000000000000)
#define f80_ln2       F80_C(0x3ffe, 0xb17217f7d1cf79ac)
#define f80_pi        F80_C(0x4000, 0xc90fdaa22168c235)
#define f80_half      F80_C(0x3ffe, 0x8000000000000000)
#define f80_infinity  F80_C(0x7fff, 0x8000000000000000)

#define f128_zero     F128_C(0x0000000000000000, 0x0000000000000000)
#define f128_one      F128_C(0x3fff000000000000, 0x0000000000000000)
#define f128_pi       F128_C(0x4000921fb54442d1, 0x8469898cc51701b8)
#define f128_half     F128_C(0x3ffe000000000000, 0x0000000000000000)
#define f128_infinity F128_C(0x7fff000000000000, 0x0000000000000000)

/*----------------------------------------------------------------------------
| Returns a quiet NaN if the floating point value is a signaling NaN;
| otherwise returns 'a'.
*----------------------------------------------------------------------------*/
inline f16 maybe_silence_nan(f16 a)
{
    if (a.IsSignalingNan()) {
       return f16(a.val() | (U16C(1) << (a.BITS_PER_SIGNIFICAND-1)));
    }
    return a;
}

inline f32 maybe_silence_nan( f32 a )
{
    if (a.IsSignalingNan()) {
        return f32::make(a.val() | (U32C(1) << (a.BITS_PER_SIGNIFICAND-1)));
    }
    return a;
}

inline f64 maybe_silence_nan( f64 a )
{
    if (a.IsSignalingNan()) {
        return f64::make(a.val() | (U64C(1) << (a.BITS_PER_SIGNIFICAND-1)));
    }
    return a;
}

inline f80 maybe_silence_nan( f80 a )
{
    if (a.IsSignalingNan()) {
        a.low |= U64C( 0xC000000000000000 );
        return a;
    }
    return a;
}

inline f128 maybe_silence_nan( f128 a )
{
    if (a.IsSignalingNan()) {
        a.high |= U64C( 0x0000800000000000 );
        return a;
    }
    return a;
}

inline f256 maybe_silence_nan( f256 a )
{
    if (a.IsSignalingNan()) {
        a.m0 |= U64C( 0x0000080000000000 );
        return a;
    }
    return a;
}

inline f512 maybe_silence_nan( f512 a )
{
    if (a.IsSignalingNan()) {
        a.m0 |= U64C( 0x0000008000000000 );
        return a;
    }
    return a;
}

/*----------------------------------------------------------------------------
| Packs the sign `zSign', exponent `zExp', and significand `zSig' into a floating-point value, returning the result.
| After being shifted into the proper positions, the three fields are simply added together to form the result.
| This means that any integer portion of `zSig' will be added into the exponent.
| Since a properly normalized significand will have an integer portion equal to 1,
| the `zExp' input should be 1 less than the desired result exponent whenever `zSig' is a complete, normalized significand.
| Extended version differ.
*----------------------------------------------------------------------------*/

inline f16 pack_f16(sign_type zSign, fp_exponent_t zExp, u16 zSig)
{
    return f16( (u16(zSign) << 15) + (u16(zExp) << f16::BITS_PER_SIGNIFICAND) + zSig );
}

inline f32 pack_f32(sign_type zSign, fp_exponent_t zExp, u32 zSig)
{
    return f32::make( (u32(zSign) << 31) + (u32(zExp) << f32::BITS_PER_SIGNIFICAND ) + zSig );
}

inline f64 pack_f64(sign_type zSign, fp_exponent_t zExp, u64 zSig)
{
    return f64::make( (u64(zSign) << 63) + (u64(zExp) << f64::BITS_PER_SIGNIFICAND ) + zSig );
}

inline f80 pack_f80( sign_type zSign, fp_exponent_t zExp, u64 zSig )
{
    return f80::make( ( u16(zSign) << f80::BITS_PER_EXPONENT ) + zExp, zSig);
}

inline f128 pack_f128( sign_type zSign, fp_exponent_t zExp, u128 zSig )
{
    return f128::make( ( u64(zSign) << 63 ) + ( u64(zExp) << f128::BITS_PER_HIGH_SIGNIFICAND ) + zSig.m0,
                     zSig.m1);
}

inline f256 pack_f256( sign_type zSign, fp_exponent_t zExp, u256 zSig )
{
    return f256( ( u64(zSign) << 63 ) + ( u64(zExp) << f256::BITS_PER_HIGH_SIGNIFICAND ) + zSig.m0,
                     zSig.m1, zSig.m2, zSig.m3 );
}

inline f512 pack_f512( sign_type zSign, fp_exponent_t zExp, u512 zSig )
{
    return f512( ( u64(zSign) << 63 ) + ( u64(zExp) << f512::BITS_PER_HIGH_SIGNIFICAND ) + zSig.m0,
                     zSig.m1, zSig.m2, zSig.m3, zSig.m4, zSig.m5, zSig.m6, zSig.m7 );
}

/*----------------------------------------------------------------------------
| Primitive arithmetic functions.
*----------------------------------------------------------------------------*/

inline f32 set_sign(f32 a, sign_type sign)
{
    return f32::make(lobits(a.val(), 31) | (u32(sign) << 31));
}
inline f64 set_sign(f64 a, sign_type sign)
{
    return f64::make(lobits(a.val(), 63) | (u64(sign) << 63));
}


/*----------------------------------------------------------------------------
| Returns 1 if the 128-bit value formed by concatenating `a0' and `a1'
| is EQ/LE/LT/NE to the 128-bit value formed by concatenating `b0' and `b1'.
| Otherwise, returns 0.
*----------------------------------------------------------------------------*/

inline bool eq128(u64 a0, u64 a1, u64 b0, u64 b1) {  return ( a0 == b0 ) && ( a1 == b1 );  }
inline bool le128(u64 a0, u64 a1, u64 b0, u64 b1) {  return ( a0 <  b0 ) || ( ( a0 == b0 ) && ( a1 <= b1 ) ); }
inline bool lt128(u64 a0, u64 a1, u64 b0, u64 b1) {  return ( a0 <  b0 ) || ( ( a0 == b0 ) && ( a1 <  b1 ) );  }


/*----------------------------------------------------------------------------
| Shifts `a' right by the number of bits given in `count'.  If any nonzero
| bits are shifted off, they are ``jammed'' into the least significant bit of
| the result by setting the least significant bit to 1.  The value of `count'
| can be arbitrarily large; in particular, if `count' is greater than 32(64), the
| result will be either 0 or 1, depending on whether `a' is zero or nonzero.
*----------------------------------------------------------------------------*/
inline u64 shift64RightJamming(u64 a, int count)
{
    u64 z;

    if ( count == 0 ) {
        z = a;
    }
    else if ( count < 64 ) {
        z = ( a>>count ) | ( ( a<<( ( - count ) & 63 ) ) != 0 );
    }
    else {
        z = ( a != 0 );
    }
    return z;
}

/*----------------------------------------------------------------------------
| Shifts the 128-bit value formed by concatenating `a0' and `a1' right by 64
| _plus_ the number of bits given in `count'.  The shifted result is at most
| 64 nonzero bits; this is stored at the location pointed to by `z0Ptr'.  The
| bits shifted off form a second 64-bit result as follows:  The _last_ bit
| shifted off is the most-significant bit of the extra result, and the other
| 63 bits of the extra result are all zero if and only if _all_but_the_last_
| bits shifted off were all zero.  This extra result is stored in the location
| pointed to by `z1Ptr'.  The value of `count' can be arbitrarily large.
|     (This routine makes more sense if `a0' and `a1' are considered to form
| a fixed-point value with binary point between `a0' and `a1'.  This fixed-
| point value is shifted right by the number of bits given in `count', and
| the integer part of the result is returned at the location pointed to by
| `z0Ptr'.  The fractional part of the result may be slightly corrupted as
| described above, and is returned at the location pointed to by `z1Ptr'.)
*----------------------------------------------------------------------------*/

inline void shift64ExtraRightJamming(
     u64 a0, u64 a1, int count, u64 *z0Ptr, u64 *z1Ptr)
{
    u64 z0, z1;

    if ( count == 0 ) {
        z1 = a1;
        z0 = a0;
    }
    else if ( count < 64 ) {
        int negCount = ( - count ) & 63;
        z1 = ( a0<<negCount ) | ( a1 != 0 );
        z0 = a0>>count;
    }
    else {
        if ( count == 64 ) {
            z1 = a0 | ( a1 != 0 );
        }
        else {
            z1 = ( ( a0 | a1 ) != 0 );
        }
        z0 = 0;
    }
    *z1Ptr = z1;
    *z0Ptr = z0;

}

/*----------------------------------------------------------------------------
| Shifts the 128-bit value formed by concatenating `a0' and `a1' right by the
| number of bits given in `count'.  Any bits shifted off are lost.  The value
| of `count' can be arbitrarily large; in particular, if `count' is greater
| than 128, the result will be 0.  The result is broken into two 64-bit pieces
| which are stored at the locations pointed to by `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/

inline
void shift128Right(u64 a0, u64 a1, int count, u64 *z0Ptr, u64 *z1Ptr)
{
    u64 z0, z1;

    if ( count == 0 ) {
        z1 = a1;
        z0 = a0;
    }
    else if ( count < 64 ) {
        int negCount = ( - count ) & 63;
        z1 = ( a0<<negCount ) | ( a1>>count );
        z0 = a0>>count;
    }
    else {
        z1 = (count < 128) ? (a0 >> (count & 63)) : 0;
        z0 = 0;
    }
    *z1Ptr = z1;
    *z0Ptr = z0;

}

/*----------------------------------------------------------------------------
| Shifts the 128-bit value formed by concatenating `a0' and `a1' right by the
| number of bits given in `count'.  If any nonzero bits are shifted off, they
| are ``jammed'' into the least significant bit of the result by setting the
| least significant bit to 1.  The value of `count' can be arbitrarily large;
| in particular, if `count' is greater than 128, the result will be either
| 0 or 1, depending on whether the concatenation of `a0' and `a1' is zero or
| nonzero.  The result is broken into two 64-bit pieces which are stored at
| the locations pointed to by `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/

ALWAYS_INLINE
void shift128RightJamming(u64 a0, u64 a1, int count, u64 *z0Ptr, u64 *z1Ptr)
{
    u64 z0, z1;
    int negCount = ( - count ) & 63;

    if ( count == 0 ) {
        z1 = a1;
        z0 = a0;
    } else if ( count < 64 ) {
        z1 = ( a0<<negCount ) | ( a1>>count ) | ( ( a1<<negCount ) != 0 );
        z0 = a0>>count;
    } else {
        if ( count == 64 ) {
            z1 = a0 | ( a1 != 0 );
        }
        else if ( count < 128 ) {
            z1 = ( a0>>( count & 63 ) ) | ( ( ( a0<<negCount ) | a1 ) != 0 );
        }
        else {
            z1 = ( ( a0 | a1 ) != 0 );
        }
        z0 = 0;
    }
    *z1Ptr = z1;
    *z0Ptr = z0;
}



inline
void shift192RightJamming(u64 a0, u64 a1, u64 a2,
       int count, u64 *z0Ptr, u64 *z1Ptr, u64 *z2Ptr)
{
    u64 z0, z1, z2;

    z0 = a0;
    z1 = a1;
    z2 = a2;

    if (count >= 192) {
       z2 = ( (z0 | z1 | z2) != 0);
       z1 = 0;
       z0 = 0;
    }
    else {
       while (count >= 64) {
          z2 = z1 | (z2 != 0);
          z1 = z0;
          z0 = 0;
          count -= 64;
       }

       if ( count > 0 ) {
           int negCount = ( - count ) & 63;
           z2 = ( z1<<negCount ) | ( z2>>count ) | ( ( z2<<negCount ) != 0 );
           z1 = ( z0<<negCount ) | ( z1>>count );
           z0 =                      z0>>count;
       }
    }

    *z2Ptr = z2;
    *z1Ptr = z1;
    *z0Ptr = z0;
}


/*----------------------------------------------------------------------------
| Shifts the 128-bit value formed by concatenating `a0' and `a1' left by the
| number of bits given in `count'.  Any bits shifted off are lost.  The value
| of `count' must be less than 64.  The result is broken into two 64-bit
| pieces which are stored at the locations pointed to by `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/

inline void shortShift128Left(
     u64 a0, u64 a1, int count, u64 *z0Ptr, u64 *z1Ptr)
{
    *z1Ptr = a1<<count;
    *z0Ptr = ( count == 0 ) ? a0 : ( a0<<count ) | ( a1>>( ( - count ) & 63 ) );
}


/*----------------------------------------------------------------------------
| Adds the 128-bit value formed by concatenating `a0' and `a1' to the 128-bit
| value formed by concatenating `b0' and `b1'.  Addition is modulo 2^128, so
| any carry out is lost.  The result is broken into two 64-bit pieces which
| are stored at the locations pointed to by `z0Ptr' and `z1Ptr'.
*----------------------------------------------------------------------------*/

inline void add128(u64 a0, u64 a1, u64 b0, u64 b1, u64 *z0Ptr, u64 *z1Ptr )
{
    const u64 z1 = a1 + b1;
    *z1Ptr = z1;
    *z0Ptr = a0 + b0 + ( z1 < a1 );
}

/*----------------------------------------------------------------------------
| Adds the 192-bit value formed by concatenating `a0', `a1', and `a2' to the
| 192-bit value formed by concatenating `b0', `b1', and `b2'.  Addition is
| modulo 2^192, so any carry out is lost.  The result is broken into three
| 64-bit pieces which are stored at the locations pointed to by `z0Ptr',
| `z1Ptr', and `z2Ptr'.
*----------------------------------------------------------------------------*/

inline void add192(
     u64 a0,
     u64 a1,
     u64 a2,
     u64 b0,
     u64 b1,
     u64 b2,
     u64 *z0Ptr,
     u64 *z1Ptr,
     u64 *z2Ptr
 )
{
    u64 z0, z1, z2;

    z2 = a2 + b2;
    const bool carry1 = ( z2 < a2 );
    z1 = a1 + b1;
    const bool carry0 = ( z1 < a1 );
    z0 = a0 + b0;
    z1 += carry1;
    z0 += ( z1 < static_cast<unsigned>(carry1) );
    z0 += carry0;
    *z2Ptr = z2;
    *z1Ptr = z1;
    *z0Ptr = z0;

}

/*----------------------------------------------------------------------------
| Subtracts the 128-bit value formed by concatenating `b0' and `b1' from the
| 128-bit value formed by concatenating `a0' and `a1'.  Subtraction is modulo
| 2^128, so any borrow out (carry out) is lost.  The result is broken into two
| 64-bit pieces which are stored at the locations pointed to by `z0Ptr' and
| `z1Ptr'.
*----------------------------------------------------------------------------*/

inline void sub128(u64 a0, u64 a1, u64 b0, u64 b1, u64 *z0Ptr, u64 *z1Ptr )
{

    *z1Ptr = a1 - b1;
    *z0Ptr = a0 - b0 - ( a1 < b1 );

}

/*----------------------------------------------------------------------------
| Subtracts the 192-bit value formed by concatenating `b0', `b1', and `b2'
| from the 192-bit value formed by concatenating `a0', `a1', and `a2'.
| Subtraction is modulo 2^192, so any borrow out (carry out) is lost.  The
| result is broken into three 64-bit pieces which are stored at the locations
| pointed to by `z0Ptr', `z1Ptr', and `z2Ptr'.
*----------------------------------------------------------------------------*/

inline void sub192(
     u64 a0,
     u64 a1,
     u64 a2,
     u64 b0,
     u64 b1,
     u64 b2,
     u64 *z0Ptr,
     u64 *z1Ptr,
     u64 *z2Ptr
 )
{
    u64 z0, z1, z2;

    z2 = a2 - b2;
    const bool borrow1 = ( a2 < b2 );
    z1 = a1 - b1;
    const bool borrow0 = ( a1 < b1 );
    z0 = a0 - b0;
    z0 -= ( z1 < static_cast<unsigned>(borrow1) );
    z1 -= borrow1;
    z0 -= borrow0;
    *z2Ptr = z2;
    *z1Ptr = z1;
    *z0Ptr = z0;

}

/*****************************************************************************
* Normalizes the subnormal floating-point value represented
* by the denormalized significand (this or aSig).
* The normalized exponent are returned, significand are updated.
*****************************************************************************/

ALWAYS_INLINE fp_exponent_t normalize_subnormal_f32(u32& aSig)
{
    assert( aSig );
    const fp_exponent_t shiftCount = countLeadingZeros32( aSig ) - f32::BITS_PER_EXPONENT;
    aSig <<= shiftCount;
    return (1 - shiftCount);
}

ALWAYS_INLINE fp_exponent_t normalize_subnormal_f64(u64& aSig)
{
    assert( aSig );
    const fp_exponent_t shiftCount = countLeadingZeros64( aSig ) - f64::BITS_PER_EXPONENT;
    aSig <<= shiftCount;
    return (1 - shiftCount);
}

ALWAYS_INLINE fp_exponent_t normalize_subnormal_f80(u64& aSig)
{
    assert( aSig );
    const fp_exponent_t shiftCount = countLeadingZeros64( aSig );
    aSig <<= shiftCount;
    return (1 - shiftCount);
}

/*******************************************************************
*
*******************************************************************/

template<typename FLOAT>
FLOAT SoftFPU::propagate_nan( FLOAT a)
{
    if ( a.IsSignalingNan() ) raise(FLOAT_FLAG_INVALID);

    if ( default_nan_mode() )
        return a.default_nan();

    return maybe_silence_nan(a);
}


/*----------------------------------------------------------------------------
| Takes two floating-point values `a' and `b', one of which is a NaN,
| and returns the appropriate NaN result.  If either `a' or `b' is a signaling NaN,
| the invalid exception is raised.
| IEEE-754 doesn't specify all the details of this, so the algorithm is target-specific.
| The routine is passed various bits of information about the two NaNs
| and should return 0 to select NaN a and 1 for NaN b.
| Note that signalling NaNs are always squashed to quiet NaNs by the caller,
| by calling maybe_silence_nan() before returning them.
|
| aIsLargerSignificand is only valid if both a and b are NaNs of some kind,
| and is true if a has the larger significand,
| or if both a and b have the same significand but a is positive but b is negative.
| It is only needed for the x87 tie-break rule.
*----------------------------------------------------------------------------*/

template<typename FLOAT>
FLOAT SoftFPU::propagate_nan( FLOAT a, FLOAT b)
{
    int which;
    const bool a_is_qnan = a.IsQuietNan();
    const bool a_is_snan = a.IsSignalingNan();
    const bool b_is_qnan = b.IsQuietNan();
    const bool b_is_snan = b.IsSignalingNan();

    if ( a_is_snan | b_is_snan ) raise(FLOAT_FLAG_INVALID);

    if ( default_nan_mode() )
        return a.default_nan();

    const bool aIsLargerSignificand = a.IsLargerSignificant(b);


    /* This implements x87 NaN propagation rules:
     * SNaN + QNaN => return the QNaN
     * two SNaNs => return the one with the larger significand, silenced
     * two QNaNs => return the one with the larger significand
     * SNaN and a non-NaN => return the SNaN, silenced
     * QNaN and a non-NaN => return the QNaN
     *
     * If we get down to comparing significands and they are the same,
     * return the NaN with the positive sign bit (if any).
     * A signaling NaN is always quietened before returning it.
     */
    if (a_is_snan) {
        if (b_is_snan) {
            which = aIsLargerSignificand ? 0 : 1;
        }
        which = b_is_qnan ? 1 : 0;
    }
    else if (a_is_qnan) {
        if (b_is_snan || !b_is_qnan)
            which = 0;
        else {
            which = aIsLargerSignificand ? 0 : 1;
        }
    } else {
        which = 1;
    }

    if (which) {
        return maybe_silence_nan(b);
    } else {
        return maybe_silence_nan(a);
    }
}


/*----------------------------------------------------------------------------
| Takes three floating-point values `a', `b' and `c', one of which is a NaN,
| and returns the appropriate NaN result.  If any of  `a',
| `b' or `c' is a signaling NaN, the invalid exception is raised.
| The input infzero indicates whether a*b was 0*inf or inf*0 (in which case
| obviously c is a NaN, and whether to propagate c or some other NaN is
| implementation defined).
*----------------------------------------------------------------------------*/
template<typename FLOAT>
FLOAT SoftFPU::propagate_nan(FLOAT a, FLOAT b, FLOAT c, bool infzero)
{
    int which;

    const bool a_is_qnan = a.IsQuietNan();
    const bool a_is_snan = a.IsSignalingNan();
    const bool b_is_qnan = b.IsQuietNan();
    const bool b_is_snan = b.IsSignalingNan();
    const bool c_is_qnan = c.IsQuietNan();
    const bool c_is_snan = c.IsSignalingNan();

    if (a_is_snan | b_is_snan | c_is_snan) {
       raise(FLOAT_FLAG_INVALID);
    }


/*----------------------------------------------------------------------------
| Select which NaN to propagate for a three-input operation.
| For the moment we assume that no CPU needs the 'larger significand'
| information.
| Return values : 0 : a; 1 : b; 2 : c; 3 : default-NaN
*----------------------------------------------------------------------------*/

    /* the (inf,zero,qnan) case sets InvalidOp,
       but we prefer to return an input NaN if we have one (ie c)
       rather than generating a default NaN
     */
    if (infzero) {
        raise(FLOAT_FLAG_INVALID);
        which = (c_is_qnan | c_is_snan) ? 2 : 3;
    } // Prefer sNaN over qNaN, in the a, b, c order.
      else if (a_is_snan) {  which = 0;
    } else if (b_is_snan) {  which = 1;
    } else if (c_is_snan) {  which = 2;
    } else if (a_is_qnan) {  which = 0;
    } else if (b_is_qnan) {  which = 1;
    } else {                 which = 2;
    }

    if (default_nan_mode()) {
        /* Note that this check is after pickNaNMulAdd so that function
         * has an opportunity to set the Invalid flag.
         */
        return FLOAT::default_nan();
    }

    switch (which) {
       case 0:    return maybe_silence_nan(a);
       case 1:    return maybe_silence_nan(b);
       case 2:    return maybe_silence_nan(c);
       case 3:
       default:   return FLOAT::default_nan();
    }
}


/* min() and max() functions. These can't be implemented as
 * 'compare and pick one input' because that would mishandle NaNs and +0 vs -0.
 */
template<typename FLOAT>
FLOAT SoftFPU::minmax(FLOAT a, FLOAT b, bool ignore_nans, bool ismin)
{
    const bool a_IsNan = a.IsNan();
    const bool b_IsNan = b.IsNan();

    if (a_IsNan || b_IsNan) {
        if (ignore_nans) {
           if ( ! a_IsNan ) return a;
           if ( ! b_IsNan ) return b;
        }
        return propagate_nan(a, b);
    }

    const sign_type a_sign = a.sign();
    const sign_type b_sign = b.sign();

    if (a_sign != b_sign) {
        if (ismin) {
            return a_sign ? a : b;
        } else {
            return a_sign ? b : a;
        }
    } else {
        if (ismin) {
            return (a_sign ^ (a.val() < b.val())) ? a : b;
        } else {
            return (a_sign ^ (a.val() < b.val())) ? b : a;
        }
    }
}


template<typename FLOAT> float_relation
SoftFPU::compare_internal( FLOAT a, FLOAT b, bool is_quiet)
{
    if (a.IsNan() || b.IsNan()) {
        if (!is_quiet || a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return float_relation_unordered;
    }

    const sign_type a_sign = a.sign();
    const sign_type b_sign = b.sign();

    if (a_sign != b_sign) {
        if (IsBothZero(a,b)) {
            /* zero case */
            return float_relation_equal;
        } else {
            return (float_relation)(1 - (2 * a_sign));
        }
    } else {
        if (a.val() == b.val()) {
           return float_relation_equal;
        } else {
           return (float_relation)(1 - 2 * (a_sign ^ ( a.val() < b.val() )));
        }
    }
}

template<typename FLOAT>
compare_mode_t SoftFPU::compare_internal(FLOAT a, FLOAT b, compare_mode_t mode)
{
    if (a.IsNan() || b.IsNan()) {
        if ((mode & FLOAT_COMPARE_SIGNALED) || a.IsSignalingNan() || b.IsSignalingNan()) {
            raise(FLOAT_FLAG_INVALID);
        }
        return FLOAT_COMPARE_UNORDERED;
    }

    const sign_type a_sign = a.sign();
    const sign_type b_sign = b.sign();

    if (a_sign != b_sign) {
        if (IsBothZero(a,b)) {
            return FLOAT_COMPARE_EQUAL;
        } else {
            return a_sign ? FLOAT_COMPARE_LESS : FLOAT_COMPARE_GREATER;
        }
    } else {
        if (a.val() == b.val()) {
           return FLOAT_COMPARE_EQUAL;
        } else {
           return (a_sign ^ ( a.val() < b.val() )) ? FLOAT_COMPARE_LESS : FLOAT_COMPARE_GREATER;
        }
    }

    return mode;
}

} // namespace fpu
} // namespace postrisc
