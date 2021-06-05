#include "util/common.hpp"
#include "util.hpp"
#include "floatimpl.hpp"

namespace postrisc {
namespace fpu {

void
u128::jamming(int count)
{
    if ( count >= 127 ) {
        m1 = ( (m0 | m1) != 0);
        m0 = 0;
        return;
    }

    if ( count >= 64 ) {
        m1 = m0 | (m1 != 0);
        m0 = 0;
        count -= 64;
    }

    if ( count > 0 ) {
        const int negCount = 64 - count;
        m1 = ( m1 >> count ) | ( m0 << negCount ) | ( ( m1 << negCount ) != 0 );
        m0 = ( m0 >> count );
    }
}

u64
u128::jamming(int count, u64 a2)
{
    if ( count >= 127 + 64 ) {
       a2 = ( (m0 | m1 | a2) != 0);
       m0 = m1 = 0;
       return a2;
    }

    while ( count >= 64 ) {
        a2 = m1 | (a2 != 0);
        m1 = m0;
        m0 = 0;
        count -= 64;
    }

    if ( count > 0 ) {
        const int negCount = 64 - count;
        a2 = ( a2 >> count ) | ( m1 << negCount ) | ( ( a2 << negCount ) != 0 );
        m1 = ( m1 >> count ) | ( m0 << negCount );
        m0 = ( m0 >> count );
    }

    return a2;
}

i32
u128::normalize_subnormal(void)
{
    assert(m0 | m1);

    if ( m0 ) {
        const int shiftCount = fpu::countLeadingZeros64( m0 ) - fpu::f128::BITS_PER_EXPONENT;
        if ( shiftCount > 0 )
           (*this) <<= shiftCount;
        return 1 - shiftCount;
    }
    else {
        assert( m1 );
        const int shiftCount = fpu::countLeadingZeros64( m1 ) - fpu::f128::BITS_PER_EXPONENT;
        if ( shiftCount < 0 ) {
            m0 = m1 >> ( - shiftCount );
            m1 = m1 << ( shiftCount & 63 );
        }
        else {
            m0 = m1 << shiftCount;
            m1 = 0;
        }
        return 1 - 64 - shiftCount;
    }
}

std::ostream &
operator << (std::ostream & out, const u128 & r)
{
    return out << fmt::hex(r.m0)
               << '_'
               << fmt::hex(r.m1);
}


/*----------------------------------------------------------------------------
| Multiplies the 128-bit value formed by concatenating `a0' and `a1' by
| `b' to obtain a 192-bit product.  The product is broken into three 64-bit
| pieces which are stored at the locations pointed to by `z0Ptr', `z1Ptr', and
| `z2Ptr'.
*----------------------------------------------------------------------------*/

void
u192::mul( const u128 & a, u64 b )
{
    u64 z1;

/*      a0a1 x b0
        a0b0--
        --a1b0
        r0r1r2
*/

    mul64To128( a.m1, b, &m1, &m2 );
    mul64To128( a.m0, b, &m0, &z1 );
    m1 += z1;
    m0 += m1 < z1;
}

void
u192::jamming(int count)
{
    if (count >= 191) {
        m2 = ( (m0 | m1 | m2) != 0);
        m1 = m0 = 0;
        return;
    }

    while ( count >= 64 ) {
        m2 = m1 | (m2 != 0);
        m1 = m0;
        m0 = 0;
        count -= 64;
    }

    if ( count > 0 ) {
        const int negCount = 64 - count;
        m2 = ( m2 >> count ) | ( m1 << negCount ) | ( ( m2 << negCount ) != 0 );
        m1 = ( m1 >> count ) | ( m0 << negCount );
        m0 = ( m0 >> count );
    }
}

std::ostream &
operator << (std::ostream & out, const u192 & r)
{
    return out << fmt::hex(r.m0)
        << '_' << fmt::hex(r.m1)
        << '_' << fmt::hex(r.m2);
}

///////////////////////////////////////////////////////////
//
commonNaNT SoftFPU::ToCommonNaN(f128 a)
{
    commonNaNT z;

    if ( a.IsSignalingNan() )
       raise(FLOAT_FLAG_INVALID);
    z.sign = a.sign();
    shortShift128Left( a.high, a.low, 16, &z.high, &z.low );
    return z;
}


f128 SoftFPU::to_f128(commonNaNT a)
{
    u64 high, low;

    if (default_nan_mode()) {
       return f128::default_nan();
    }

    shift128Right( a.high, a.low, f128::BITS_PER_EXPONENT + 1, &high, &low );
    high |= ( u64(a.sign)<<63 ) | U64C( 0x7FFF000000000000 ) | U64C( 0x0000800000000000 );
    return f128::make(high, low);
}


/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and extended significand formed by the concatenation of `zSig0', `zSig1',
| and `zSig2', and returns the proper quadruple-precision floating-point value
| corresponding to the abstract input.  Ordinarily, the abstract value is
| simply rounded and packed into the quadruple-precision format, with the
| inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded to
| a subnormal number, and the underflow and inexact exceptions are raised if
| the abstract input cannot be represented exactly as a subnormal quadruple-
| precision floating-point number.
|     The input significand must be normalized or smaller.  If the input
| significand is not normalized, `zExp' must be 0; in that case, the result
| returned is a subnormal number, and it must not require rounding.  In the
| usual case that the input significand is normalized, `zExp' must be 1 less
| than the ``true'' floating-point exponent.  The handling of underflow and
| overflow follows the IEC/IEEE Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f128 SoftFPU::RoundPackTo_f128(
     sign_type zSign, fp_exponent_t zExp, u128 zSig, u64 zSig2)
{
//    const int NUM_ROUNDING_BITS = 10;
//    const int HIGHEST_GUARD_BIT = 1 << (NUM_ROUNDING_BITS - 1); //0x200
    const fp_exponent_t NNNN = f128::EINF - 2; //0x7FFD

    rounding_mode_t roundingMode = get_rounding_mode();
    bool roundNearestEven = ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN );
    bool increment = ( i64(zSig2) < 0 );
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            increment = false;
        }
        else {
            if ( zSign ) {
                increment = ( roundingMode == FLOAT_ROUND_DOWNWARD ) && zSig2;
            }
            else {
                increment = ( roundingMode == FLOAT_ROUND_UPWARD ) && zSig2;
            }
        }
    }

    if ( u32(NNNN) <= u32(zExp) ) {
        if (    ( NNNN < zExp )
             || (    ( zExp == NNNN )
                  && u128( U64C( 0x0001FFFFFFFFFFFF ), U64C( 0xFFFFFFFFFFFFFFFF ) ) == zSig
                  && increment
                )
           ) {
            raise(FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
            if (                 ( roundingMode == FLOAT_ROUND_TOWARDZERO )
                 || (   zSign && ( roundingMode == FLOAT_ROUND_UPWARD     ) )
                 || ( ! zSign && ( roundingMode == FLOAT_ROUND_DOWNWARD   ) )
               ) {
                return pack_f128(zSign, NNNN + 1, u128(makemask64(f128::BITS_PER_HIGH_SIGNIFICAND), U64C( 0xFFFFFFFFFFFFFFFF )) ); //0x7FFE
            }
            return f128::inf(zSign);
        }
        if ( zExp < 0 ) {
            if ( flush_to_zero() ) {
                raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                return f128::zero(zSign);
            }
            bool isTiny = tininess_before_rounding()
                || ( zExp < -1 )
                || ! increment
                || zSig < u128( U64C( 0x0001FFFFFFFFFFFF ),
                                   U64C( 0xFFFFFFFFFFFFFFFF ) );
            zSig2 = zSig.jamming( - zExp, zSig2 );
            zExp = 0;
            if ( isTiny && zSig2 ) raise(FLOAT_FLAG_UNDERFLOW);
            if ( roundNearestEven ) {
                increment = ( i64(zSig2) < 0 );
            }
            else {
                if ( zSign ) {
                    increment = ( roundingMode == FLOAT_ROUND_DOWNWARD ) && zSig2;
                }
                else {
                    increment = ( roundingMode == FLOAT_ROUND_UPWARD ) && zSig2;
                }
            }
        }
    }
    if ( zSig2 ) raise(FLOAT_FLAG_INEXACT);
    if ( increment ) {
        ++zSig;
        zSig.m1 &= ~ u64( ( zSig2 + zSig2 == 0 ) & roundNearestEven );
    }
    else {
        if ( ! zSig ) zExp = 0;
    }
    return pack_f128( zSign, zExp, zSig );

}

/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand formed by the concatenation of `zSig0' and `zSig1', and
| returns the proper quadruple-precision floating-point value corresponding
| to the abstract input.  This routine is just like `RoundPackTo_f128'
| except that the input significand has fewer bits and does not have to be
| normalized.  In all cases, `zExp' must be 1 less than the ``true'' floating-
| point exponent.
*----------------------------------------------------------------------------*/

f128
SoftFPU::NormalizeRoundPackTo_f128( sign_type zSign, fp_exponent_t zExp, u128 zSig)
{
    u64 zSig2 = 0;

    if ( zSig.m0 == 0 ) {
        zSig.m0 = zSig.m1;
        zSig.m1 = 0;
        zExp -= 64;
    }
    assert( zSig.m0 );
    int shiftCount = countLeadingZeros64( zSig.m0 ) - f128::BITS_PER_EXPONENT;
    if ( shiftCount > 0 ) {
        zSig <<= shiftCount;
    }
    else if ( shiftCount < 0 ) {
        zSig2 = zSig.jamming( - shiftCount, 0 );
    }
    zExp -= shiftCount;
    return RoundPackTo_f128(zSign, zExp, zSig, zSig2);

}


f128 SoftFPU::to_f128( i32 a )
{
    if ( a == 0 ) return f128::zero(SIGN_POSITIVE);
    sign_type zSign = get_sign(a);
    u32 absA = zSign ? - a : a;
    int shiftCount = countLeadingZeros32( absA ) + (f128::BITS_PER_HIGH_SIGNIFICAND + 1 - 32); //17
    u64 zSig0 = absA;
    return pack_f128( zSign,
                          f128::BIAS + f128::BITS_PER_HIGH_SIGNIFICAND - 1 - shiftCount,
                          u128( zSig0<<shiftCount, 0 ) ); //0x402E
}


f128 SoftFPU::to_f128( i64 a )
{
    sign_type     zSign;
    u64      absA;
    fp_exponent_t zExp;
    f128::fraction_t  zSig;

    if ( a == 0 ) return f128::zero(SIGN_POSITIVE);
    zSign = get_sign(a);
    absA = zSign ? - a : a;
    int shiftCount = countLeadingZeros64( absA ) + (f128::BITS_PER_HIGH_SIGNIFICAND + 1);
    zExp = (f128::BIAS + f128::BITS_PER_SIGNIFICAND - 1) - shiftCount; //0x406E
    if ( 64 <= shiftCount ) {
        zSig = u128(absA, 0);
        shiftCount -= 64;
    }
    else {
        zSig = u128(0, absA);
    }
    if (shiftCount > 0)
       zSig <<= shiftCount;
    return pack_f128( zSign, zExp, zSig );

}

f128 SoftFPU::to_f128(u64 a)
{
    if (a == 0) {
        return f128_zero;
    }
    return NormalizeRoundPackTo_f128(SIGN_POSITIVE, f128::BIAS + 112 - 1, u128(a, 0) ); //0x406E
}


/*----------------------------------------------------------------------------
| Returns the result of converting the quadruple-precision floating-point
| value `a' to the 32-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32( f128 a )
{
    i32       shiftCount;

    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( a.IsNan() ) a_sign = SIGN_POSITIVE;
    if ( aExp ) aSig.m0 |= a.HIDDEN_BIT;
    u64 zSig = aSig.m0 | ( aSig.m1 != 0 );
    shiftCount = (a.BIAS + (a.BITS_PER_HIGH_SIGNIFICAND - 7)) - aExp; //0x4028
    if ( 0 < shiftCount ) zSig = shift64RightJamming( zSig, shiftCount);
    return round_pack_to_i32( INT_NAN_F128, a_sign, zSig );

}


/*----------------------------------------------------------------------------
| Returns the result of converting the quadruple-precision floating-point
| value `a' to the 32-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.  If
| `a' is a NaN, the largest positive integer is returned.  Otherwise, if the
| conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32_round_to_zero( f128 a )
{
    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type    a_sign = a.sign();

    u64  zSig = aSig.m0 | ( aSig.m1 != 0 );

    if ( a.BIAS + 31 < aExp ) { //0x401E
        if ( aExp == a.EINF ) {
           //if ( aSig ) goto invalid;
           if ( aSig ) a_sign = SIGN_POSITIVE; //?????
        }
//        goto invalid;
        raise(FLOAT_FLAG_INVALID);
        return a_sign ? std::numeric_limits<i32>::min() : std::numeric_limits<i32>::max();
    }
    else if ( aExp < a.BIAS ) {
        if ( aExp || zSig ) raise(FLOAT_FLAG_INEXACT);
        return 0;
    }
    zSig |= a.HIDDEN_BIT;
    i32 shiftCount = (a.BIAS + 112 - 64) - aExp; //0x402F
    u64 savedASig = zSig;
    zSig >>= shiftCount;
    i32 z = zSig;
    if ( a_sign ) z = - z;
    if ( ( z < 0 ) ^ a_sign ) {
// invalid:
        raise(FLOAT_FLAG_INVALID);
        return a_sign ? std::numeric_limits<i32>::min() : std::numeric_limits<i32>::max();
    }
    if ( ( zSig << shiftCount ) != savedASig ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    return z;

}


/*----------------------------------------------------------------------------
| Returns the result of converting the quadruple-precision floating-point
| value `a' to the 64-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i64 SoftFPU::to_i64( f128 a )
{
    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type    a_sign = a.sign();

    if ( aExp ) aSig.m0 |= a.HIDDEN_BIT;
    i32 shiftCount = (a.BIAS + a.BITS_PER_SIGNIFICAND - 64) - aExp;  //0x402F
    if ( shiftCount <= 0 ) {
        if ( a.BIAS + 63 < aExp ) { //0x403E
            raise(FLOAT_FLAG_INVALID);
            if ( ! a_sign || a.IsNan() ) {
                return std::numeric_limits<i64>::max();
            }
            return a_sign ? std::numeric_limits<i64>::min() : std::numeric_limits<i64>::max();
        }
        if (shiftCount)
           aSig <<= (- shiftCount);
    }
    else {
        aSig.jamming( shiftCount );
    }
    return round_pack_to_i64(INT_NAN_F128, a_sign, aSig.m0, aSig.m1 );

}

/*----------------------------------------------------------------------------
| Returns the result of converting the quadruple-precision floating-point
| value `a' to the 64-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.
| If `a' is a NaN, the largest positive integer is returned.  Otherwise, if
| the conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

i64 SoftFPU::to_i64_round_to_zero( f128 a )
{
    i64      z;

    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp ) aSig.m0 |= a.HIDDEN_BIT;

    i32 shiftCount = aExp - (a.BIAS + a.BITS_PER_SIGNIFICAND - 64); //0x402F;

    if ( 0 < shiftCount ) {
        if ( a.BIAS + 63 <= aExp ) { //0x403E
            // f128(SIGN_NEGATIVE, 63)
            aSig.m0 = lobits(aSig.m0, a.BITS_PER_HIGH_SIGNIFICAND);
            if (    ( a.high == U64C( 0xC03E000000000000 ) )
                 && ( aSig.m1 < U64C( 0x0002000000000000 ) ) ) {
                if ( aSig.m1 ) raise(FLOAT_FLAG_INEXACT);
            }
            else {
                raise(FLOAT_FLAG_INVALID);
                if ( ! a_sign || a.IsNan() ) {
                    return std::numeric_limits<i64>::max();
                }
            }
            return std::numeric_limits<i64>::min();
        }
        z = ( aSig.m0<<shiftCount ) | ( aSig.m1>>( ( - shiftCount ) & 63 ) );
        if ( u64( aSig.m1<<shiftCount ) ) {
            raise(FLOAT_FLAG_INEXACT);
        }
    }
    else {
        if ( aExp < a.BIAS ) {
            if ( aExp || aSig ) {
                raise(FLOAT_FLAG_INEXACT);
            }
            return 0;
        }
        z = aSig.m0>>( - shiftCount );
        if ( aSig.m1 || ( shiftCount && u64( aSig.m0<<( shiftCount & 63 ) ) ) ) {
            raise(FLOAT_FLAG_INEXACT);
        }
    }
    if ( a_sign ) z = - z;
    return z;

}


f32 SoftFPU::to_f32( f128 a )
{
    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return to_f32( ToCommonNaN(a) );
        }
        return f32::inf(a_sign);
    }

    u32 zSig = shift64RightJamming( aSig.m0 | ( aSig.m1 != 0 ), 18);

    if ( aExp || zSig ) {
        zSig |= ( u32(1) << (a.BITS_PER_HIGH_SIGNIFICAND - 18) ); // set hidden bit
        aExp -= a.BIAS - f32::BIAS + 1; //0x3F81
    }
    return RoundPackTo_f32( a_sign, aExp, zSig );

}


f64 SoftFPU::to_f64( f128 a )
{
    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type   a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return to_f64( ToCommonNaN(a) );
        }
        return f64::inf(a_sign);
    }
    aSig <<= 14;
    u64 zSig = aSig.m0 | ( aSig.m1 != 0 );
    if ( aExp || zSig ) {
        zSig |= ( u64(1) << (a.BITS_PER_HIGH_SIGNIFICAND + 14) ); // set hidden bit
        aExp -= a.BIAS - f64::BIAS + 1; //0x3C01;
    }
    return RoundPackTo_f64( a_sign, aExp, zSig );

}


f80 SoftFPU::to_f80( f128 a )
{
    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return to_f80( ToCommonNaN(a) );
        }
        return f80::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( ! aSig ) return f80::zero( a_sign );
        aExp = aSig.normalize_subnormal();
    }
    else {
        aSig.m0 |= a.HIDDEN_BIT;
    }
    aSig <<= f128::BITS_PER_EXPONENT;
    return RoundPackTo_f80( FLOAT_ROUND_PRECISION_80, a_sign, aExp, aSig.m0, aSig.m1 );
}


f128 SoftFPU::round_to_int( f128 a )
{
    sign_type   a_sign;
    fp_exponent_t aExp;
    u64      lastBitMask, roundBitsMask;
    f128      z = a;

    aExp = a.exponent();
    if ( a.BIAS + a.BITS_PER_SIGNIFICAND - 64 <= aExp ) {  //0x402F
        if ( a.BIAS + a.BITS_PER_SIGNIFICAND <= aExp ) { //0x406F
            if ( ( aExp == a.EINF ) && a.fraction() ) {
                return propagate_nan(a);
            }
            return a;
        }
        lastBitMask = 1;
        lastBitMask = ( lastBitMask<<( (a.BIAS + a.BITS_PER_SIGNIFICAND - 1) - aExp ) )<<1;  //0x406E
        roundBitsMask = lastBitMask - 1;
        z = a;
        if ( get_rounding_mode() == FLOAT_ROUND_TONEAREST_EVEN ) {
            if ( lastBitMask ) {
                add128( z.high, z.low, 0, lastBitMask>>1, &z.high, &z.low );
                if ( ( z.low & roundBitsMask ) == 0 ) z.low &= ~ lastBitMask;
            }
            else {
                if ( i64(z.low) < 0 ) {
                    ++z.high;
                    if ( u64( z.low<<1 ) == 0 ) z.high &= ~1;
                }
            }
        }
        else if ( get_rounding_mode() != FLOAT_ROUND_TOWARDZERO ) {
            if (   z.sign() ^ ( get_rounding_mode() == FLOAT_ROUND_UPWARD ) ) {
                add128( z.high, z.low, 0, roundBitsMask, &z.high, &z.low );
            }
        }
        z.low &= ~ roundBitsMask;
    }
    else {
        if ( aExp < a.BIAS ) {
            if ( ( ( u64( a.high<<1 ) ) | a.low ) == 0 ) return a;
            raise(FLOAT_FLAG_INEXACT);
            a_sign = a.sign();
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
                if ( ( aExp == a.BIAS - 1 ) && a.fraction() ) {
                    return pack_f128( a_sign, a.BIAS, u128(0, 0) );
                }
                break;
             case FLOAT_ROUND_DOWNWARD:
                return a_sign ? pack_f128(SIGN_NEGATIVE, a.BIAS, u128(0, 0) ) : a.zero(SIGN_POSITIVE);
             case FLOAT_ROUND_UPWARD:
                return a_sign ? a.zero(SIGN_NEGATIVE) : pack_f128(SIGN_POSITIVE, a.BIAS, u128(0, 0) );
             case FLOAT_ROUND_TOWARDZERO:
                break;
            }
            return a.zero(a_sign);
        }
        lastBitMask = 1;
        lastBitMask <<= a.BIAS + a.BITS_PER_SIGNIFICAND - 64 - aExp; //0x402F
        roundBitsMask = lastBitMask - 1;
        z.low = 0;
        z.high = a.high;
        if ( get_rounding_mode() == FLOAT_ROUND_TONEAREST_EVEN ) {
            z.high += lastBitMask>>1;
            if ( ( ( z.high & roundBitsMask ) | a.low ) == 0 ) {
                z.high &= ~ lastBitMask;
            }
        }
        else if ( get_rounding_mode() != FLOAT_ROUND_TOWARDZERO ) {
            if (   z.sign() ^ ( get_rounding_mode() == FLOAT_ROUND_UPWARD ) ) {
                z.high |= ( a.low != 0 );
                z.high += roundBitsMask;
            }
        }
        z.high &= ~ roundBitsMask;
    }
    if ( ( z.low != a.low ) || ( z.high != a.high ) ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    return z;

}


f128 SoftFPU::round_to_int(f128 a, rounding_mode_t rmode)
{
    rounding_mode_t old = get_rounding_mode();
    set_rounding_mode(rmode);
    f128 t = round_to_int(a);
    set_rounding_mode(old);
    return t;
}


bool SoftFPU::eq_signaling( f128 a, f128 b )
{
    if (a.IsNan() || b.IsNan()) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    return
           ( a.low == b.low ) && (( a.high == b.high )
             || (    ( a.low == 0 ) && ( ( ( a.high | b.high )<<1 ) == 0 ) )
           );

}


bool SoftFPU::le_signaling( f128 a, f128 b )
{
    sign_type a_sign, b_sign;

    if (a.IsNan() || b.IsNan()) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    a_sign = a.sign();
    b_sign = b.sign();
    if ( a_sign != b_sign ) {
        return a_sign || IsBothZero(a,b);
    }
    return
          a_sign ? b.val() <= a.val()
                : a.val() <= b.val();

}


bool SoftFPU::lt_signaling( f128 a, f128 b )
{
    sign_type a_sign, b_sign;

    if (a.IsNan() || b.IsNan()) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    a_sign = a.sign();
    b_sign = b.sign();
    if ( a_sign != b_sign ) {
        return a_sign && ! IsBothZero(a,b);
    }
    return
          a_sign ? b.val() < a.val()
                : a.val() < b.val();

}


bool SoftFPU::eq_quiet( f128 a, f128 b )
{

    if (a.IsNan() || b.IsNan()) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    return
           ( a.low == b.low )
        && (    ( a.high == b.high )
             || ( ( a.low == 0 ) && ( u64( ( a.high | b.high )<<1 ) == 0 ) )
           );

}


bool SoftFPU::le_quiet( f128 a, f128 b )
{
    sign_type a_sign, b_sign;

    if (a.IsNan() || b.IsNan()) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
           raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    a_sign = a.sign();
    b_sign = b.sign();
    if ( a_sign != b_sign ) {
        return a_sign || IsBothZero(a,b);
    }
    return
          a_sign ? b.val() <= a.val()
                : a.val() <= b.val();

}


bool SoftFPU::lt_quiet( f128 a, f128 b )
{
    if (a.IsNan() || b.IsNan()) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
           raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }

    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();

    if ( a_sign != b_sign ) {
        return a_sign && !IsBothZero(a,b);
    }
    return
          a_sign ? b.val() < a.val()
                : a.val() < b.val();

}


bool SoftFPU::unordered( f128 a, f128 b )
{
    if (a.IsNan() || b.IsNan()) {
        raise(FLOAT_FLAG_INVALID);
        return true;
    }
    return false;
}


bool SoftFPU::unordered_quiet( f128 a, f128 b )
{
    if (a.IsNan() || b.IsNan()) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return true;
    }
    return false;
}


f128 SoftFPU::to_f128( f64 a )
{
    u64    aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f128( ToCommonNaN(a) );
        return f128::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f128::zero(a_sign);
        aExp = normalize_subnormal_f64( aSig );
        --aExp;
    }
    f128::fraction_t zSig = u128(aSig, 0) >> (f128::BITS_PER_EXPONENT - f64::BITS_PER_EXPONENT);
    return pack_f128( a_sign, aExp + f128::BIAS - a.BIAS, zSig );
}


f128 SoftFPU::to_f128( f80 a )
{
    fp_exponent_t  aExp;
    u64  aSig;

    aSig = a.fraction();
    aExp = a.exponent();
    sign_type a_sign = a.sign();
    if ( ( aExp == a.EINF ) && u64( aSig<<1 ) ) {
        return to_f128( ToCommonNaN(a) );
    }
    f128::fraction_t zSig = u128( aSig<<1, 0) >> 16;
    return pack_f128( a_sign, aExp, zSig );

}


f128 SoftFPU::to_f128( f32 a )
{
    u32   aSig   = a.fraction();
    fp_exponent_t  aExp   = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f128( ToCommonNaN(a) );
        return f128::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f128::zero(a_sign);
        aExp = normalize_subnormal_f32( aSig );
        --aExp;
    }
    return pack_f128( a_sign, aExp + f128::BIAS - a.BIAS, u128( u64(aSig) << (25), 0) ); //0x3F80

}

/* Multiply A by 2 raised to the power N.  */
f128 SoftFPU::scalbn( f128 a, int n )
{
    f128::fraction_t aSig = a.fraction();
    fp_exponent_t  aExp   = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return propagate_nan(a);
        }
        return a;
    }
    if ( aExp != 0 )
        aSig.m0 |= a.HIDDEN_BIT;
    else if ( ! aSig )
        return a;

    if (n > 0x10000) {
        n = 0x10000;
    } else if (n < -0x10000) {
        n = -0x10000;
    }

    aExp += n - 1;
    return NormalizeRoundPackTo_f128( a_sign, aExp, aSig);
}


f128 SoftFPU::merge(f128 sign, f128 exponent, f128 mantissa)
{
    u64 high =
       (    sign.high & (makemask64(1) << 63) ) |
       (mantissa.high & (makemask64(f128::BITS_PER_SIGNIFICAND - 64)) ) |
       (exponent.high & (makemask64(f128::BITS_PER_EXPONENT) << (f128::BITS_PER_SIGNIFICAND - 64)) );
    return f128::make(high, mantissa.low);
}


float_relation SoftFPU::compare(f128 a, f128 b) {  return compare_internal(a, b, false);  }
float_relation SoftFPU::compare_quiet(f128 a, f128 b) {  return compare_internal(a, b, true);  }

compare_mode_t SoftFPU::compare(f128 a, f128 b, compare_mode_t mode)
{
    return compare_internal(a, b, mode);
}

f128 SoftFPU::min(f128 a, f128 b, bool ignore_nans) {  return minmax<f128>(a, b, ignore_nans, true);  }
f128 SoftFPU::max(f128 a, f128 b, bool ignore_nans) {  return minmax<f128>(a, b, ignore_nans, false); }


/*----------------------------------------------------------------------------
| Returns the result of adding the absolute values of the quadruple-precision
| floating-point values `a' and `b'.  If `zSign' is 1, the sum is negated
| before being returned.  `zSign' is ignored if the result is a NaN.
| The addition is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f128 SoftFPU::addSigs( f128 a, f128 b, sign_type zSign)
{
    fp_exponent_t  zExp;
    f128::fraction_t zSig;
    u64    zSigExtra;

    const u64 OVER_HIDDEN_BIT = a.HIDDEN_BIT << 1; //U64C( 0x0002000000000000 );

    f128::fraction_t aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();

    f128::fraction_t bSig = b.fraction();
    fp_exponent_t bExp = b.exponent();

    fp_exponent_t expDiff = aExp - bExp;

    if ( 0 < expDiff ) {
        if ( aExp == a.EINF ) {
            if ( aSig ) return propagate_nan(a, b);
            return a;
        }
        if ( bExp == 0 ) {
            --expDiff;
        }
        else {
            bSig.m0 |= b.HIDDEN_BIT;
        }
        zSigExtra = bSig.jamming( expDiff, 0 );
        zExp = aExp;
    }
    else if ( expDiff < 0 ) {
        if ( bExp == b.EINF ) {
            if ( bSig ) return propagate_nan(a, b);
            return b.inf(zSign);
        }
        if ( aExp == 0 ) {
            ++expDiff;
        }
        else {
            aSig.m0 |= a.HIDDEN_BIT;
        }
        zSigExtra = aSig.jamming(-expDiff, 0 );
        zExp = bExp;
    }
    else {
        if ( aExp == a.EINF ) {
            if ( aSig || bSig ) {
                return propagate_nan(a, b);
            }
            return a;
        }
        zSig = aSig + bSig;
        if ( aExp == 0 ) {
            if (flush_to_zero()) {
                if (zSig) {
                    raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                }
                return a.zero(zSign);
            }
            return pack_f128( zSign, 0, zSig );
        }
        zSigExtra = 0;
        zSig.m0 |= OVER_HIDDEN_BIT;
        zExp = aExp;
        goto shiftRight1;
    }
    aSig.m0 |= a.HIDDEN_BIT;
    zSig = aSig + bSig;
    --zExp;
    if ( zSig.m0 < OVER_HIDDEN_BIT ) goto roundAndPack;
    ++zExp;
 shiftRight1:
    zSigExtra = zSig.jamming(1, zSigExtra );
 roundAndPack:
    return RoundPackTo_f128( zSign, zExp, zSig, zSigExtra );

}

/*----------------------------------------------------------------------------
| Returns the result of subtracting the absolute values of the quadruple-
| precision floating-point values `a' and `b'.  If `zSign' is 1, the
| difference is negated before being returned.  `zSign' is ignored if the
| result is a NaN.  The subtraction is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f128 SoftFPU::subSigs( f128 a, f128 b, sign_type zSign)
{
    fp_exponent_t  zExp;
    f128::fraction_t  zSig;

    const int FIXED_POINT = 14;
    const u64 FIXED_HIDDEN_BIT = u64(1) << (f128::BITS_PER_HIGH_SIGNIFICAND + FIXED_POINT);

    f128::fraction_t aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();

    f128::fraction_t bSig = b.fraction();
    fp_exponent_t bExp = b.exponent();

    fp_exponent_t expDiff = aExp - bExp;

    aSig <<= FIXED_POINT;
    bSig <<= FIXED_POINT;

    if ( 0 < expDiff ) goto aExpBigger;
    if ( expDiff < 0 ) goto bExpBigger;
    if ( aExp == a.EINF ) {
        if ( aSig || bSig ) {
            return propagate_nan(a, b);
        }
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( aExp == 0 ) {
        aExp = 1;
        bExp = 1;
    }
    if ( bSig.m0 < aSig.m0 ) goto aBigger;
    if ( aSig.m0 < bSig.m0 ) goto bBigger;
    if ( bSig.m1 < aSig.m1 ) goto aBigger;
    if ( aSig.m1 < bSig.m1 ) goto bBigger;
    return a.zero( sign_type(get_rounding_mode() == FLOAT_ROUND_DOWNWARD) );
 bExpBigger:
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(a, b);
        return b.inf(zSign ^ SIGN_NEGATIVE);
    }
    if ( aExp == 0 ) {
        ++expDiff;
    }
    else {
        aSig.m0 |= FIXED_HIDDEN_BIT;
    }
    aSig.jamming( - expDiff );
    bSig.m0 |= FIXED_HIDDEN_BIT;
 bBigger:
    zSig = bSig - aSig;
    zExp = bExp;
    zSign = ! zSign;
    goto normalizeRoundAndPack;
 aExpBigger:
    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a, b);
        return a;
    }
    if ( bExp == 0 ) {
        --expDiff;
    }
    else {
        bSig.m0 |= FIXED_HIDDEN_BIT;
    }
    bSig.jamming( expDiff );
    aSig.m0 |= FIXED_HIDDEN_BIT;
 aBigger:
    zSig = aSig - bSig;
    zExp = aExp;
 normalizeRoundAndPack:
    --zExp;
    return NormalizeRoundPackTo_f128( zSign, zExp - FIXED_POINT, zSig );

}


f128 SoftFPU::add( f128 a, f128 b )
{
    sign_type a_sign, b_sign;

    a_sign = a.sign();
    b_sign = b.sign();
    if ( a_sign == b_sign ) {
        return addSigs( a, b, a_sign );
    } else {
        return subSigs( a, b, a_sign );
    }
}


f128 SoftFPU::sub( f128 a, f128 b )
{
    sign_type a_sign, b_sign;

    a_sign = a.sign();
    b_sign = b.sign();
    if ( a_sign == b_sign ) {
        return subSigs( a, b, a_sign );
    } else {
        return addSigs( a, b, a_sign );
    }
}

f128 SoftFPU::mul( f128 a, f128 b )
{
    f128::fraction_t aSig = a.fraction();
    fp_exponent_t  aExp = a.exponent();
    sign_type   a_sign = a.sign();

    f128::fraction_t bSig = b.fraction();
    fp_exponent_t bExp = b.exponent();
    sign_type   b_sign = b.sign();

    sign_type zSign = a_sign ^ b_sign;

    if ( aExp == a.EINF ) {
        if ( aSig || b.IsNan() ) {
            return propagate_nan(a, b);
        }
        if ( bExp == 0 && !bSig ) goto invalid;
        return b.inf(zSign);
    }
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(b);
        if ( aExp == 0 && !aSig ) {
 invalid:
            raise(FLOAT_FLAG_INVALID);
            return a.default_nan();
        }
        return b.inf(zSign);
    }

    if ( aExp == 0 ) {
        if ( !aSig ) return a.zero(zSign);
        aExp = aSig.normalize_subnormal();
    }
    if ( bExp == 0 ) {
        if ( !bSig ) return b.zero(zSign);
        bExp = bSig.normalize_subnormal();
    }

    fp_exponent_t zExp = aExp + bExp - b.BIAS;

    aSig.m0 |= a.HIDDEN_BIT;
    bSig.m0 |= b.HIDDEN_BIT;
    bSig <<= b.BITS_PER_EXPONENT;

    u256 zSig;
    zSig.mul( aSig, bSig );

    LOG_DEBUG(LOG_FPU) << aSig << '\n' << bSig << '\n' << zSig;

    if ( (zSig.m0 & a.HIDDEN_BIT) == 0 ) {
        zSig <<= 1;
        --zExp;
    }

    return RoundPackTo_f128( zSign, zExp, u128(zSig.m0, zSig.m1), zSig.extra_jamming() );
}

f128 SoftFPU::div( f128 a, f128 b )
{
    sign_type  a_sign, b_sign, zSign;
    fp_exponent_t  zExp;
    f128::fraction_t zSig;
    u192    term;

    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    a_sign = a.sign();

    f128::fraction_t bSig  = b.fraction();
    fp_exponent_t bExp  = b.exponent();
    b_sign = b.sign();

    zSign = a_sign ^ b_sign;

    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a, b);
        if ( bExp == b.EINF ) {
            if ( bSig ) return propagate_nan(a, b);
            goto invalid;
        }
        return a.inf(zSign);
    }
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(a, b);
        return b.zero(zSign);
    }

    if ( bExp == 0 ) {
        if ( ! bSig ) {
            if ( aExp == 0 && !aSig ) {
 invalid:
                raise(FLOAT_FLAG_INVALID);
                return a.default_nan();
            }
            raise(FLOAT_FLAG_DIVBYZERO);
            return b.inf(zSign);
        }
        bExp = bSig.normalize_subnormal();
    }

    if ( aExp == 0 ) {
        if ( !aSig ) return a.zero(zSign);
        aExp = aSig.normalize_subnormal();
    }

    zExp = aExp - bExp + (b.BIAS - 2);

    aSig.m0 |= a.HIDDEN_BIT;
    bSig.m0 |= b.HIDDEN_BIT;
    aSig <<= a.BITS_PER_EXPONENT;
    bSig <<= b.BITS_PER_EXPONENT;

    if ( bSig <= aSig ) {
        aSig >>= 1;
        ++zExp;
    }

    zSig.m0 = estimateDiv128To64( aSig.m0, aSig.m1, bSig.m0 );
    term.mul( bSig, zSig.m0 );
    u192 rem = u192( aSig.m0, aSig.m1, 0) - term;
    while ( i64(rem.m0) < 0 ) {
        --zSig.m0;
        rem += bSig;
    }

    zSig.m1 = estimateDiv128To64( rem.m1, rem.m2, bSig.m0 );
    if ( lobits(zSig.m1, a.BITS_PER_EXPONENT - 1) <= 4 ) {
        term.mul( bSig, zSig.m1 );
        rem = u192( rem.m1, rem.m2, 0) - term;
        while ( i64(rem.m0) < 0 ) {
            --zSig.m1;
            rem += bSig;
        }
        zSig.m1 |= (bool) rem;
    }
    u64 zSigExtra = zSig.jamming( a.BITS_PER_EXPONENT, 0 );
    return RoundPackTo_f128( zSign, zExp, zSig, zSigExtra );

}


f128 SoftFPU::rem( f128 a, f128 b )
{
    u192 term;

    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    f128::fraction_t bSig  = b.fraction();
    fp_exponent_t bExp  = b.exponent();

    if ( aExp == a.EINF ) {
        if ( aSig || ( ( bExp == b.EINF ) && bSig ) ) {
            return propagate_nan(a, b);
        }
        goto invalid;
    }
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(b);
        return a;
    }
    if ( bExp == 0 ) {
        if ( ! bSig ) {
 invalid:
            raise(FLOAT_FLAG_INVALID);
            return b.default_nan();
        }
        bExp = bSig.normalize_subnormal();
    }
    if ( aExp == 0 ) {
        if ( ! aSig ) return a;
        aExp = aSig.normalize_subnormal();
    }

    fp_exponent_t expDiff = aExp - bExp;
    if ( expDiff < -1 ) return a;

    aSig.m0 |= a.HIDDEN_BIT;
    bSig.m0 |= b.HIDDEN_BIT;

    aSig <<= a.BITS_PER_EXPONENT - ( expDiff < 0 );
    bSig <<= b.BITS_PER_EXPONENT;

    u64 q = (bSig <= aSig);
    if ( q ) aSig -= bSig;
    expDiff -= 64;

    while ( 0 < expDiff ) {
        q = estimateDiv128To64( aSig.m0, aSig.m1, bSig.m0 );
        q = ( 4 < q ) ? q - 4 : 0;
        term.mul( bSig, q );
        term <<= 61;
        aSig <<= 61;
        aSig = u128( aSig.m0, 0) - u128( term.m0, term.m1);
        expDiff -= 61;
    }

    if ( -64 < expDiff ) {
        q = estimateDiv128To64( aSig.m0, aSig.m1, bSig.m0 );
        q = ( 4 < q ) ? q - 4 : 0;
        q >>= - expDiff;
        bSig >>= 12;
        expDiff += (64 - 12);
        if ( expDiff < 0 ) {
            aSig >>= ( - expDiff );
        } else if ( expDiff > 0 ) {
            aSig <<= expDiff;
        }
        term.mul( bSig, q );
        aSig -= u128( term.m1, term.m2 );
    }
    else {
        aSig >>= 12;
        bSig >>= 12;
    }

    f128::fraction_t alternateASig;

    do {
        alternateASig = aSig;
        ++q;
        aSig -= bSig;
    } while ( 0 <= i64(aSig.m0) );

    u128 sigMean = aSig + alternateASig;

    if ( ( i64(sigMean.m0) < 0 ) || ( ( ! sigMean ) && ( q & 1 ) ) ) {
        aSig = alternateASig;
    }

    sign_type zSign = get_sign( i64(aSig.m0) );
    if ( zSign ) aSig = -aSig;

    return NormalizeRoundPackTo_f128( a_sign ^ zSign, bExp - 4, aSig );

}


f128 SoftFPU::sqrt( f128 a )
{
    f128::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a);
        if ( ! a_sign ) return a;
        goto invalid;
    }
    if ( a_sign ) {
        // exact zero
        if ( aExp == 0 && !aSig ) return a;
 invalid:
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( aExp == 0 ) {
        if ( !aSig ) return a.zero(SIGN_POSITIVE);
        aExp = aSig.normalize_subnormal();
    }

    f128::fraction_t zSig;
    u64 zSig2, doubleZSig0;

    fp_exponent_t zExp = ( ( aExp - a.BIAS )>>1 ) + (a.BIAS - 1);
    aSig.m0 |= a.HIDDEN_BIT;
    zSig.m0 = estimateSqrt32( aExp, aSig.m0 >> (64 - 32 - a.BITS_PER_EXPONENT) );
    aSig <<= ( a.BITS_PER_EXPONENT - 2 - ( aExp & 1 ) );

    zSig.m0 = estimateDiv128To64( aSig.m0, aSig.m1, zSig.m0<<32 ) + ( zSig.m0<<30 );

    doubleZSig0 = zSig.m0<<1;
    u128 term;
    mul64To128( zSig.m0, zSig.m0, &term.m0, &term.m1 );
    u128 rem = aSig - term;
    while ( i64(rem.m0) < 0 ) {
        --zSig.m0;
        doubleZSig0 -= 2;
        rem += u128( zSig.m0>>63, doubleZSig0 | 1 );
    }
    LOG_DEBUG(LOG_FPU) << rem << '\n';

    zSig.m1 = estimateDiv128To64( rem.m1, 0, doubleZSig0 );

    if ( lobits(zSig.m1, a.BITS_PER_EXPONENT - 2) <= 5 ) {
        if ( zSig.m1 == 0 ) zSig.m1 = 1;
        mul64To128( doubleZSig0, zSig.m1, &term.m0, &term.m1 );
        rem = u128( rem.m1, 0) - term;
        mul64To128( zSig.m1, zSig.m1, &term.m0, &term.m1 );
        u192 rem3 = u192( rem.m0, rem.m1, 0 ) - u192( 0, term.m0, term.m1 );
        while ( i64(rem3.m0) < 0 ) {
            --zSig.m1;
            term = u128(0, zSig.m1) << 1;
            term.m1 |= 1;
            term.m0 |= doubleZSig0;
            rem3 += term;
        }
        zSig.m1 |= (bool) rem3;
    }

    zSig2 = zSig.jamming( a.BITS_PER_EXPONENT - 1, 0 );
    return RoundPackTo_f128( SIGN_POSITIVE, zExp, zSig, zSig2 );

}


/*----------------------------------------------------------------------------
| Returns the result of multiplying the quadruple-precision floating-point values
| `a' and `b' then adding 'c', with no intermediate rounding step after the
| multiplication.  The operation is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic 754-2008.
| The flags argument allows the caller to select negation of the
| addend, the intermediate product, or the final result. (The difference
| between this and having the caller do a separate negation is that negating
| externally will flip the sign bit on NaNs.)
*----------------------------------------------------------------------------*/
extern "C" native::f128 fmaq(native::f128 a, native::f128 b, native::f128 c);

f128 SoftFPU::muladd(f128 a, f128 b, f128 c, muladd_negate_t flags)
{
    sign_type     a_sign, b_sign, cSign, zSign, pSign;
    fp_exponent_t aExp, bExp, cExp, pExp, zExp, expDiff;
    bool          pInf, pZero, infzero;
    u256          zSig;

    f128::fraction_t aSig = a.fraction();
    aExp  = a.exponent();
    a_sign = a.sign();

    f128::fraction_t bSig = b.fraction();
    bExp  = b.exponent();
    b_sign = b.sign();

    f128::fraction_t cSig = c.fraction();
    cExp  = c.exponent();
    cSign = c.sign();

    infzero = ((aExp == 0 && !aSig && bExp == b.EINF && !bSig) ||
               (aExp == a.EINF && !aSig && bExp == 0 && !bSig));

    /* It is implementation-defined whether the cases of (0,inf,qnan)
     * and (inf,0,qnan) raise InvalidOperation or not (and what QNaN
     * they return if they do), so we have to hand this information
     * off to the target-specific pick-a-NaN routine.
     */
    if (a.IsNan() || b.IsNan() || c.IsNan() ) {
        return propagate_nan(a, b, c, infzero);
    }

    if (infzero) {
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }

    if (flags & FLOAT_MULADD_NEGATE_ADDEND) {
        cSign = ! cSign;
    }

    // Work out the sign and type of the product
    pSign = a_sign ^ b_sign;
    if (flags & FLOAT_MULADD_NEGATE_PRODUCT) {
        pSign = ! pSign;
    }
    pInf = (aExp == a.EINF) || (bExp == b.EINF);
    pZero = (aExp == 0 && !aSig) || (bExp == 0 && !bSig);

    if (cExp == c.EINF) {
        if (pInf && (pSign ^ cSign)) {
            // addition of opposite-signed infinities => InvalidOperation
            raise(FLOAT_FLAG_INVALID);
            return a.default_nan();
        }
        // Otherwise generate an infinity of the same sign
        return c.inf(cSign);
    }

    if (pInf) {
        return a.inf(pSign);
    }

    if (pZero) {
        if (cExp == 0) {
            if ( !cSig ) {
                // Adding two exact zeroes
                if (pSign == cSign) {
                    zSign = pSign;
                } else if (get_rounding_mode() == FLOAT_ROUND_DOWNWARD) {
                    zSign = SIGN_NEGATIVE;
                } else {
                    zSign = SIGN_POSITIVE;
                }
                return c.zero(zSign);
            }
            // Exact zero plus a denorm
            if (flush_to_zero()) {
                raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                return c.zero(cSign);
            }
        }
        // Zero plus something non-zero : just return the something
        return pack_f128(cSign, cExp, cSig);
    }

    if (aExp == 0) {
        aExp = aSig.normalize_subnormal();
    }
    if (bExp == 0) {
        bExp = bSig.normalize_subnormal();
    }

    /* Calculate the actual result a * b + c */

    /* Multiply first; this is easy. */
    /* NB: we subtract (bias-1) where mul() subtracts bias
     * because we want the true exponent, not the "one-less-than"
     * flavour that RoundPackTo_f128() takes.
     */
    pExp = aExp + bExp - (b.BIAS - 1);
    aSig.m0 |= a.HIDDEN_BIT;
    bSig.m0 |= b.HIDDEN_BIT;
    aSig <<= f128::BITS_PER_EXPONENT - 1;
    bSig <<= f128::BITS_PER_EXPONENT;

    u256 pSig;
    pSig.mul( aSig, bSig );
    if ( i64(pSig.m0 << 1) >= 0 ) {
        pSig <<= 1;
        pExp--;
    }
    assert ( i64(pSig.m0) >= 0 );

    zSign = pSign;

    // Now [pSig0:pSig3] is the significand of the multiply, with the explicit bit in position 254 (p0:62).
    if (cExp == 0) {
        if ( !cSig ) {
            // Throw out the special case of c being an exact zero now
            pSig.jamming( 14 );
            return RoundPackTo_f128(zSign, pExp-1, u128(pSig.m0, pSig.m1), pSig.extra_jamming() );
        }
        cExp = cSig.normalize_subnormal();
    }

    /* Shift cSig and add the explicit bit so [cSig0:cSig1] is the
     * significand of the addend, with the explicit bit in position 126.
     */
    cSig.m0 |= c.HIDDEN_BIT;
    cSig <<= 14;
    expDiff = pExp - cExp;
    assert ( i64(cSig.m0) >= 0 );

    u256 cSigX(cSig.m0, cSig.m1, 0, 0);

    if (pSign == cSign) {
        // Addition
        if (expDiff > 0) {
            // scale c to match p
            cSigX.jamming( expDiff );
            zExp = pExp;
        } else if (expDiff < 0) {
            // scale p to match c
            pSig.jamming( -expDiff );
            zExp = cExp;
        } else {
            // no scaling needed
            zExp = cExp;
        }

        // Add significands and make sure explicit bit ends up in posn 254
        zSig = pSig + cSigX;
        if ( i64(zSig.m0) < 0 ) {
            zSig.jamming( 14 + 1 );
        } else {
            zSig.jamming( 14 );
            zExp--;
        }

        return RoundPackTo_f128(zSign, zExp, u128(zSig.m0, zSig.m1), zSig.extra_jamming() );

    } else {
        // Subtraction
        if (expDiff > 0) {
            cSigX.jamming( expDiff );
            zSig = pSig - cSigX;
            zExp = pExp;
        } else if (expDiff < 0) {
            pSig.jamming( -expDiff );
            zSig = cSigX - pSig;
            zExp = cExp;
            zSign = ! zSign;
        } else {
            zExp = pExp;
            if ( cSigX < pSig ) {
                zSig = pSig - cSigX;
            } else if ( pSig < cSigX ) {
                zSig = cSigX - pSig;
                zSign = ! zSign;
            } else {
                // Exact zero
                return c.zero(sign_type(get_rounding_mode() == FLOAT_ROUND_DOWNWARD));
            }
        }

        while (zSig.m0 == 0) {
           zSig.m0 = zSig.m1;
           zSig.m1 = zSig.m2;
           zSig.m2 = zSig.m3;
           zSig.m3 = 0;
           zExp -= 64;
        }

        int shiftcount = countLeadingZeros64(zSig.m0) - f128::BITS_PER_EXPONENT;

        if (shiftcount > 0) {
           zSig <<= shiftcount;
           zExp -= shiftcount;
        } else if ( shiftcount < 0 ) {
           zSig.jamming( -shiftcount );
           zExp -= shiftcount;
        }

        return RoundPackTo_f128(zSign, zExp - f128::BITS_PER_EXPONENT,
                    u128(zSig.m0, zSig.m1), zSig.extra_jamming() );

    }

#if 1
//        return RoundPackTo_f128(zSign, zExp, zSig.m0, zSig.m1, zSig.m2 | ( zSig.m3 != 0 ) );
//    return NormalizeRoundPackTo_f128(zSign, zExp - 14, zSig0, zSig1 | ( (zSig2 | zSig3) != 0 ) );
#else
    f128 x = RoundPackTo_f128(zSign, zExp, zSig.m0, zSig.m1, zSig.m2 | ( zSig.m3 != 0 ) );
    f128 t;
//    t = mpfr_test_fmal(a, b, c);
    t.hard() = ::fmaq(a.hard(), b.hard(), c.hard());
    if (x.val() != t.val()) {
       LOG_DEBUG(LOG_FPU) << "mul: " << pSig  << ' ' << int(pSign) << ' ' << std::dec << pExp << "\n";
       LOG_DEBUG(LOG_FPU) << "c:   " << cSigX << ' ' << int(cSign) << ' ' << std::dec << cExp << "\n";
       LOG_DEBUG(LOG_FPU) << "z:   " << zSig  << ' ' << int(zSign) << ' ' << std::dec << zExp << ' ' << expDiff << "\n";
    }
    return x;
#endif
}

} // namespace fpu
} // namespace postrisc
