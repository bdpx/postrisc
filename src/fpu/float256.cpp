#include "util/common.hpp"
#include "util.hpp"
#include "floatimpl.hpp"

namespace postrisc {
namespace fpu {

commonNaNT CSoftFPU::ToCommonNaN(f256 a)
{
    commonNaNT z;

    if ( a.IsSignalingNan() )
       raise(FLOAT_FLAG_INVALID);
    z.sign = a.sign();
    shortShift128Left( a.m0, a.m1 | ((a.m2 | a.m3) != 0), 20, &z.high, &z.low );
    return z;
}

f256 CSoftFPU::to_f256(commonNaNT a)
{
    u64 high, low;

    if (default_nan_mode()) {
       return f256::default_nan();
    }

    shift128Right( a.high, a.low, f256::BITS_PER_EXPONENT + 1, &high, &low );
    high |= ( u64(a.sign)<<63 ) | U64C( 0x7FFFF00000000000 ) | U64C( 0x0000080000000000 );
    return f256(high, low, 0, 0);
}

/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and extended significand formed by the concatenation of `zSig0', `zSig1',
| and `zSigExtra', and returns the proper 256-bit precision floating-point value
| corresponding to the abstract input.  Ordinarily, the abstract value is
| simply rounded and packed into the 256-bit precision format, with the
| inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded to
| a subnormal number, and the underflow and inexact exceptions are raised if
| the abstract input cannot be represented exactly as a subnormal 256-bit
| precision floating-point number.
|     The input significand must be normalized or smaller.  If the input
| significand is not normalized, `zExp' must be 0; in that case, the result
| returned is a subnormal number, and it must not require rounding.  In the
| usual case that the input significand is normalized, `zExp' must be 1 less
| than the ``true'' floating-point exponent.  The handling of underflow and
| overflow follows the IEC/IEEE Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f256 CSoftFPU::RoundPackTo_f256(
    sign_type zSign, fp_exponent_t zExp, u256 & zSig, u64 zSigExtra)
{
    const i32 NNNN = f256::EINF - 2;

    rounding_mode_t roundingMode = get_rounding_mode();
    bool roundNearestEven = ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN );
    bool increment = ( i64(zSigExtra) < 0 );
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            increment = false;
        }
        else {
            if ( zSign ) {
                increment = ( roundingMode == FLOAT_ROUND_DOWNWARD ) && zSigExtra;
            }
            else {
                increment = ( roundingMode == FLOAT_ROUND_UPWARD ) && zSigExtra;
            }
        }
    }

    if ( u32(NNNN) <= u32(zExp) ) {

        const u256 full_ones( U64C( 0x00001FFFFFFFFFFF ),
                                   U64C( 0xFFFFFFFFFFFFFFFF ),
                                   U64C( 0xFFFFFFFFFFFFFFFF ),
                                   U64C( 0xFFFFFFFFFFFFFFFF ) );

        if ( ( NNNN < zExp ) || ( ( zExp == NNNN ) && zSig == full_ones && increment ) ) {
            raise(FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
            if (                 ( roundingMode == FLOAT_ROUND_TOWARDZERO )
                 || (   zSign && ( roundingMode == FLOAT_ROUND_UPWARD     ) )
                 || ( ! zSign && ( roundingMode == FLOAT_ROUND_DOWNWARD   ) )
               ) {
                return pack_f256( zSign, NNNN, full_ones );
            }
            return f256::inf(zSign);
        }
        if ( zExp < 0 ) {
            if ( flush_to_zero() ) {
                raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                return f256::zero(zSign);
            }
            bool isTiny = tininess_before_rounding()
                || ( zExp < -1 )
                || ! increment
                || zSig < full_ones;
            zSigExtra = zSig.jamming( -zExp, zSigExtra );
            zExp = 0;
            if ( isTiny && zSigExtra ) raise(FLOAT_FLAG_UNDERFLOW);
            if ( roundNearestEven ) {
                increment = ( i64(zSigExtra) < 0 );
            }
            else {
                if ( zSign ) {
                    increment = ( roundingMode == FLOAT_ROUND_DOWNWARD ) && zSigExtra;
                }
                else {
                    increment = ( roundingMode == FLOAT_ROUND_UPWARD ) && zSigExtra;
                }
            }
        }
    }
    if ( zSigExtra ) raise(FLOAT_FLAG_INEXACT);
    if ( increment ) {
        ++zSig;
        zSig.m3 &= ~ u64( ( zSigExtra + zSigExtra == 0 ) & roundNearestEven );
    }
    else {
        if ( ! zSig ) zExp = 0;
    }

    return pack_f256( zSign, zExp, zSig );
}


/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand formed by the concatenation of `zSig0' and `zSig1', and
| returns the proper 256-bit precision floating-point value corresponding
| to the abstract input.  This routine is just like `RoundPackTo_f256'
| except that the input significand has fewer bits and does not have to be
| normalized.  In all cases, `zExp' must be 1 less than the ``true'' floating-
| point exponent.
*----------------------------------------------------------------------------*/

f256 CSoftFPU::NormalizeRoundPackTo_f256( sign_type zSign, fp_exponent_t zExp, u256 & zSig )
{
    i32 count = zSig.normalize_subnormal() - 1;

    u64 zSigExtra = 0;
    if ( count > 0 ) {
       zSigExtra = zSig.jamming(count, 0 );
    }
    zExp += count;
    return RoundPackTo_f256(zSign, zExp, zSig, zSigExtra);
}


f256 CSoftFPU::to_f256( i32 a )
{
    if ( a == 0 ) return f256::zero(SIGN_POSITIVE);
    sign_type zSign = get_sign(a);
    u32 absA = zSign ? - a : a;
    int shiftCount = countLeadingZeros32( absA ) + (f256::BITS_PER_HIGH_SIGNIFICAND + 1 - 32);
    u64 zSig0 = absA;
    return pack_f256( zSign, f256::BIAS + 43 - shiftCount, u256(zSig0<<shiftCount, 0, 0, 0) );
}


f256 CSoftFPU::to_f256( i64 a )
{
    sign_type   zSign;
    u64      absA;
    fp_exponent_t zExp;
    u256     zSig;

    if ( a == 0 ) return f256::zero(SIGN_POSITIVE);
    zSign = get_sign(a);
    absA = zSign ? - a : a;
    int shiftCount = countLeadingZeros64( absA ) + (f256::BITS_PER_HIGH_SIGNIFICAND + 1);
    zExp = (f256::BIAS + 64 + f256::BITS_PER_HIGH_SIGNIFICAND - 1) - shiftCount;
    if ( 64 <= shiftCount ) {
        zSig = u256( absA, 0, 0, 0 );
        shiftCount -= 64;
    }
    else {
        zSig = u256( 0, absA, 0, 0 );
    }
    if (shiftCount > 0)
       zSig <<= shiftCount;
    return pack_f256( zSign, zExp, zSig );

}


f256 CSoftFPU::to_f256(u64 a)
{
    if (a == 0) {
        return f256::zero(SIGN_POSITIVE);
    }
    u256 aSig(a, 0, 0, 0);
    return NormalizeRoundPackTo_f256(SIGN_POSITIVE, f256::BIAS + 64 + f256::BITS_PER_HIGH_SIGNIFICAND - 1, aSig );
}


/*----------------------------------------------------------------------------
| Returns the result of converting the 256-bit precision floating-point
| value `a' to the 32-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i32 CSoftFPU::to_i32( f256 a )
{
    sign_type   a_sign;
    fp_exponent_t  aExp, shiftCount;
    u256     aSig;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    if ( a.IsNan() ) a_sign = SIGN_POSITIVE;
    if ( aExp ) aSig.m0 |= a.HIDDEN_BIT;
    aSig.m0 |= ( (aSig.m1 | aSig.m2 | aSig.m3) != 0 );
    shiftCount = ( a.BIAS + (f256::BITS_PER_HIGH_SIGNIFICAND - 7) ) - aExp;
    if ( 0 < shiftCount ) aSig.m0 = shift64RightJamming( aSig.m0, shiftCount);
    return round_pack_to_i32( INT_NAN_F256, a_sign, aSig.m0 );

}


/*----------------------------------------------------------------------------
| Returns the result of converting the 256-bit precision floating-point
| value `a' to the 32-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.  If
| `a' is a NaN, the largest positive integer is returned.  Otherwise, if the
| conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

i32 CSoftFPU::to_i32_round_to_zero( f256 a )
{
    sign_type  a_sign;
    fp_exponent_t aExp, shiftCount;
    u256    aSig;
    u64     savedASig;
    i32      z;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    aSig.m0 |= ( (aSig.m1 | aSig.m2 | aSig.m3) != 0 );
    if ( a.BIAS + 31 < aExp ) { //0x401E
        if ( aExp == a.EINF ) {
           //   if ( aSig ) goto invalid;
           if ( aSig ) a_sign = SIGN_POSITIVE; //?????
        }
        goto invalid;
    }
    else if ( aExp < a.BIAS ) {
        if ( aExp || aSig.m0 ) raise(FLOAT_FLAG_INEXACT);
        return 0;
    }
    aSig.m0 |= a.HIDDEN_BIT;
    shiftCount = ( a.BIAS + a.BITS_PER_HIGH_SIGNIFICAND ) - aExp; //0x402F
    savedASig = aSig.m0;
    aSig.m0 >>= shiftCount;
    z = aSig.m0;
    if ( a_sign ) z = - z;
    if ( ( z < 0 ) ^ a_sign ) {
 invalid:
        raise(FLOAT_FLAG_INVALID);
        return a_sign ? std::numeric_limits<i32>::min() : std::numeric_limits<i32>::max();
    }
    if ( ( aSig.m0<<shiftCount ) != savedASig ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    return z;

}

/*----------------------------------------------------------------------------
| Returns the result of converting the 256-bit precision floating-point
| value `a' to the 64-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i64 CSoftFPU::to_i64( f256 a )
{
    sign_type   a_sign;
    fp_exponent_t aExp, shiftCount;
    u256     aSig;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    if ( aExp ) aSig.m0 |= a.HIDDEN_BIT;
    shiftCount = ( a.BIAS + a.BITS_PER_HIGH_SIGNIFICAND ) - aExp;  //0x402F
    if ( shiftCount <= 0 ) {
        if ( a.BIAS + 63 < aExp ) { //0x403E
            raise(FLOAT_FLAG_INVALID);
            if ( ! a_sign || a.IsNan() ) {
                return std::numeric_limits<i64>::max();
            }
            return a_sign ? std::numeric_limits<i64>::min() : std::numeric_limits<i64>::max();
        }
        aSig <<= -shiftCount;
    }
    else {
        aSig.jamming( shiftCount );
    }
    return round_pack_to_i64(INT_NAN_F256, a_sign, aSig.m0, aSig.m1 | ( (aSig.m2 | aSig.m3) != 0) );
}


/*----------------------------------------------------------------------------
| Returns the result of converting the 256-bit precision floating-point
| value `a' to the 64-bit two's complement integer format.  The conversion
| is performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.
| If `a' is a NaN, the largest positive integer is returned.  Otherwise, if
| the conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

i64 CSoftFPU::to_i64_round_to_zero( f256 a )
{
    sign_type  a_sign;
    fp_exponent_t aExp, shiftCount;
    u256    aSig;
    i64      z;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    if ( aExp ) aSig.m0 |= a.HIDDEN_BIT;
    shiftCount = aExp - (a.BIAS + a.BITS_PER_HIGH_SIGNIFICAND);
    if ( 0 < shiftCount ) {
        if ( a.BIAS + 63 <= aExp ) {
            // f256(SIGN_NEGATIVE, 63)
            aSig.m0 = lobits(aSig.m0, a.BITS_PER_HIGH_SIGNIFICAND);
            if (    ( a.m0 == U64C( 0xC003E00000000000 ) )
                 && ( aSig.m1 < U64C( 0x0000200000000000 ) ) ) {
                if ( aSig.m1 | aSig.m2 | aSig.m3 ) raise(FLOAT_FLAG_INEXACT);
            }
            else {
                raise(FLOAT_FLAG_INVALID);
                if ( ! a_sign || a.IsNan() ) {
                    return std::numeric_limits<i64>::max();
                }
            }
            return std::numeric_limits<i64>::min();
        }
        aSig <<= shiftCount;
        z = aSig.m0;
        if ( aSig.m1 | aSig.m2 | aSig.m3 ) {
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
        z = aSig.m0 >> ( - shiftCount );
        if ( (aSig.m1 | aSig.m2 | aSig.m3) || ( shiftCount && u64( aSig.m0<<( shiftCount & 63 ) ) ) ) {
            raise(FLOAT_FLAG_INEXACT);
        }
    }
    if ( a_sign ) z = - z;
    return z;

}


f256 CSoftFPU::to_f256( f32 a )
{
    u32 aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f256( ToCommonNaN(a) );
        return f256::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f256::zero(a_sign);
        aExp = normalize_subnormal_f32( aSig );
        --aExp;
    }
    return pack_f256( a_sign, aExp + f256::BIAS - a.BIAS,
              u256( u64(aSig) << (32 - (f256::BITS_PER_EXPONENT - f32::BITS_PER_EXPONENT)), 0, 0, 0 ) );

}


f32 CSoftFPU::to_f32( f256 a )
{
    u256   aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return to_f32( ToCommonNaN(a) );
        }
        return f32::inf(a_sign);
    }
    aSig.m0 |= ( (aSig.m1 | aSig.m2 | aSig.m3) != 0 );
    aSig.m0 = shift64RightJamming( aSig.m0, a.BITS_PER_HIGH_SIGNIFICAND - f32::BITS_PER_SIGNIFICAND - 7);
    u32 zSig = aSig.m0;
    if ( aExp || zSig ) {
        zSig |= ( u32(1) << (f32::BITS_PER_SIGNIFICAND + 7) ); // set hidden bit
        aExp -= a.BIAS - f32::BIAS + 1;
    }
    return RoundPackTo_f32( a_sign, aExp, zSig );

}


f256 CSoftFPU::to_f256( f64 a )
{
    fp_exponent_t  aExp;
    u64  aSig, zSig0, zSig1;

    aSig = a.fraction();
    aExp = a.exponent();
    sign_type a_sign = a.sign();
    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f256( ToCommonNaN(a) );
        return f256::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f256::zero(a_sign);
        aExp = normalize_subnormal_f64( aSig );
        --aExp;
    }
    shift128Right( aSig, 0, f256::BITS_PER_EXPONENT - a.BITS_PER_EXPONENT, &zSig0, &zSig1 );
    return pack_f256( a_sign, aExp + f256::BIAS - a.BIAS, u256(zSig0, zSig1, 0, 0) ); //0x3C00
}


f64 CSoftFPU::to_f64( f256 a )
{
    u256    aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return to_f64( ToCommonNaN(a) );
        }
        return f64::inf(a_sign);
    }
    aSig <<= 18;
    aSig.m0 |= ( (aSig.m1 | aSig.m2 | aSig.m3) != 0 );
    if ( aExp || aSig.m0 ) {
        aSig.m0 |= ( u64(1) << (a.BITS_PER_HIGH_SIGNIFICAND + 18) ); // set hidden bit
        aExp -= a.BIAS - f64::BIAS + 1; //0x3C01;
    }
    return RoundPackTo_f64( a_sign, aExp, aSig.m0 );

}


f256 CSoftFPU::to_f256( f80 a )
{
    u64    aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
       if ( u64( aSig<<1 ) )
          return to_f256( ToCommonNaN(a) );
       return f256::inf(a_sign);
    }

    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f256::zero(a_sign);
        aExp = normalize_subnormal_f80( aSig );
    }

    u128 zSig = u128(aSig<<1, 0) >> (f256::BITS_PER_EXPONENT + 1);
    return pack_f256( a_sign, aExp + f256::BIAS - a.BIAS, u256(zSig.m0, zSig.m1, 0, 0) );

}


f80 CSoftFPU::to_f80( f256 a )
{
    u256    aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
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
    aExp -= a.BIAS - f80::BIAS;
    aSig <<= f256::BITS_PER_EXPONENT;
    return RoundPackTo_f80( FLOAT_ROUND_PRECISION_80, a_sign, aExp, aSig.m0, aSig.m1 | ((aSig.m2 | aSig.m3) != 0) );
}


f256 CSoftFPU::to_f256( f128 a )
{
    u128   aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f256( ToCommonNaN(a) );
        return f256::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( ! aSig ) return f256::zero(a_sign);
        aExp = aSig.normalize_subnormal();
        --aExp;
    }

    u256 zSig(aSig.m0, aSig.m1, 0, 0);
    if ( f256::BITS_PER_EXPONENT - a.BITS_PER_EXPONENT > 0 )
       zSig >>= ( f256::BITS_PER_EXPONENT - a.BITS_PER_EXPONENT );
    return pack_f256( a_sign, aExp + f256::BIAS - a.BIAS, zSig );
}


f128 CSoftFPU::to_f128( f256 a )
{
    u256    aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return to_f128( ToCommonNaN(a) );
        }
        return f128::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( ! aSig ) return f128::zero( a_sign );
        aExp = aSig.normalize_subnormal();
    }
    else {
        aSig.m0 |= a.HIDDEN_BIT;
    }
    aExp -= a.BIAS - f128::BIAS + 1;

    if ( a.BITS_PER_EXPONENT - f128::BITS_PER_EXPONENT > 0)
       aSig <<= ( a.BITS_PER_EXPONENT - f128::BITS_PER_EXPONENT );

    return RoundPackTo_f128( a_sign, aExp, u128(aSig.m0, aSig.m1), aSig.m2 | (aSig.m3 != 0) );
}


f256 CSoftFPU::round_to_int( f256 a )
{
    fp_exponent_t aExp = a.exponent();
    u256 aSig = a.fraction();

    if ( (a.BIAS + a.BITS_PER_SIGNIFICAND) <= aExp ) {
        if ( ( aExp == a.EINF ) && aSig ) {
            return propagate_nan(a);
        }
        return a;
    }

    if ( aExp < a.BIAS ) {
        if ( ( ( a.m0<<1 ) | a.m1 | a.m2 | a.m3 ) == 0 ) return a;
        raise(FLOAT_FLAG_INEXACT);
        sign_type a_sign = a.sign();
        switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
                if ( ( aExp == a.BIAS - 1 ) && aSig ) {
                    return pack_f256( a_sign, a.BIAS, u256( 0, 0, 0, 0) );
                }
                break;
             case FLOAT_ROUND_DOWNWARD:
                return a_sign ? pack_f256(SIGN_NEGATIVE, a.BIAS, u256(0, 0, 0, 0) ) : a.zero(SIGN_POSITIVE);
             case FLOAT_ROUND_UPWARD:
                return a_sign ? a.zero(SIGN_NEGATIVE) : pack_f256(SIGN_POSITIVE, a.BIAS, u256(0, 0, 0, 0) );
             case FLOAT_ROUND_TOWARDZERO:
                break;
        }
        return a.zero(a_sign);
    }

#if 0
    u64 lastBitMask = 1;
    lastBitMask <<= (a.BIAS + a.BITS_PER_SIGNIFICAND) - aExp; //0x433
    u64 roundBitsMask = lastBitMask - 1;
    u64 z = a.val();
    rounding_mode_t roundingMode = get_rounding_mode();
    if ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN ) {
        z += lastBitMask>>1;
        if ( ( z & roundBitsMask ) == 0 ) z &= ~ lastBitMask;
    }
    else if ( roundingMode != FLOAT_ROUND_TOWARDZERO ) {
        if ( f64(z).sign() ^ ( roundingMode == FLOAT_ROUND_UPWARD ) ) {
            z += roundBitsMask;
        }
    }
    z &= ~ roundBitsMask;
    if ( z != a.val() )
        raise(FLOAT_FLAG_INEXACT);
    return f64(z);
#endif

    u256 lastBitMask;
    fp_exponent_t count = (a.BIAS + a.BITS_PER_SIGNIFICAND) - aExp;
    //lastBitMask = 1 << count;
    if (count < 64) {
       lastBitMask = u256(0, 0, 0, u64(1) << count);
    } else if (count < 128) {
       lastBitMask = u256(0, 0, u64(1) << (count - 64), 0);
    } else if (count < 192) {
       lastBitMask = u256(0, u64(1) << (count - 128), 0, 0);
    } else if (count < 256) {
       lastBitMask = u256(u64(1) << (count - 192), 0, 0, 0);
    } else {
       //!!!!!!!!!!!!
       lastBitMask = u256(0, 0, 0, 0);
    }

    u256 roundBitsMask = lastBitMask - u256(0,0,0,1);
    u256 z = a.val();
    rounding_mode_t roundingMode = get_rounding_mode();
    if ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN ) {
        z += lastBitMask>>1;
        if ( ! ( z & roundBitsMask ) ) z &= ~ lastBitMask;
    }
    else if ( roundingMode != FLOAT_ROUND_TOWARDZERO ) {
        if ( (z.m0 >> 63) ^ ( roundingMode == FLOAT_ROUND_UPWARD ) ) {
            z += roundBitsMask;
        }
    }
    z &= ~ roundBitsMask;
    if ( z != a.val() )
        raise(FLOAT_FLAG_INEXACT);
    return f256(z.m0, z.m1, z.m2, z.m3);
}


f256 CSoftFPU::round_to_int(f256 a, rounding_mode_t rmode)
{
    rounding_mode_t old = get_rounding_mode();
    set_rounding_mode(rmode);
    f256 t = round_to_int(a);
    set_rounding_mode(old);
    return t;
}


bool CSoftFPU::eq_signaling( f256 a, f256 b )
{
    if (a.IsNan() || b.IsNan()) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }

    return a.val() == b.val() || IsBothZero(a, b);
}


bool CSoftFPU::le_signaling( f256 a, f256 b )
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


bool CSoftFPU::lt_signaling( f256 a, f256 b )
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


bool CSoftFPU::eq_quiet( f256 a, f256 b )
{

    if (a.IsNan() || b.IsNan()) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }

    return a.val() == b.val() || IsBothZero(a, b);
}


bool CSoftFPU::le_quiet( f256 a, f256 b )
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


bool CSoftFPU::lt_quiet( f256 a, f256 b )
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


bool CSoftFPU::unordered( f256 a, f256 b )
{
    if (a.IsNan() || b.IsNan()) {
        raise(FLOAT_FLAG_INVALID);
        return true;
    }
    return false;
}


bool CSoftFPU::unordered_quiet( f256 a, f256 b )
{
    if (a.IsNan() || b.IsNan()) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return true;
    }
    return false;
}


/* Multiply A by 2 raised to the power N.  */
f256 CSoftFPU::scalbn( f256 a, int n )
{
    u256 aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
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
    return NormalizeRoundPackTo_f256( a_sign, aExp, aSig );
}


f256 CSoftFPU::merge(f256 sign, f256 exponent, f256 mantissa)
{
    u64 high =
       (    sign.m0 & (makemask64(1) << 63) ) |
       (mantissa.m0 & (makemask64(mantissa.BITS_PER_HIGH_SIGNIFICAND)) ) |
       (exponent.m0 & (makemask64(exponent.BITS_PER_EXPONENT) << exponent.BITS_PER_HIGH_SIGNIFICAND) );
    return f256(high, mantissa.m1, mantissa.m2, mantissa.m3);
}


float_relation CSoftFPU::compare(f256 a, f256 b) {  return compare_internal(a, b, false);  }
float_relation CSoftFPU::compare_quiet(f256 a, f256 b) {  return compare_internal(a, b, true);  }

f256 CSoftFPU::min(f256 a, f256 b, bool ignore_nans) {  return minmax<f256>(a, b, ignore_nans, true);  }
f256 CSoftFPU::max(f256 a, f256 b, bool ignore_nans) {  return minmax<f256>(a, b, ignore_nans, false); }

/*----------------------------------------------------------------------------
| Returns the result of adding the absolute values of the 256-bit precision
| floating-point values `a' and `b'.  If `zSign' is 1, the sum is negated
| before being returned.  `zSign' is ignored if the result is a NaN.
| The addition is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f256 CSoftFPU::addSigs( f256 a, f256 b, sign_type zSign)
{
    fp_exponent_t aExp, bExp, zExp;
    u256  zSig;
    u64   zSig2;
    fp_exponent_t expDiff;

    const u64 OVER_HIDDEN_BIT = a.HIDDEN_BIT << 1; //U64C( 0x0002000000000000 );

    u256 aSig = a.fraction();
    aExp = a.exponent();
    u256 bSig = b.fraction();
    bExp = b.exponent();
    expDiff = aExp - bExp;
    if ( 0 < expDiff ) {
        if ( aExp == a.EINF ) {
            if ( aSig ) return propagate_nan(a);
            return a;
        }
        if ( bExp == 0 ) {
            --expDiff;
        }
        else {
            bSig.m0 |= b.HIDDEN_BIT;
        }
        zSig2 = bSig.jamming( expDiff, 0 );
        zExp = aExp;
    }
    else if ( expDiff < 0 ) {
        if ( bExp == b.EINF ) {
            if ( bSig ) return propagate_nan(b);
            return b.inf(zSign);
        }
        if ( aExp == 0 ) {
            ++expDiff;
        }
        else {
            aSig.m0 |= a.HIDDEN_BIT;
        }
        zSig2 = aSig.jamming( -expDiff, 0 );
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
            if ( flush_to_zero() ) {
                if ( zSig ) {
                    raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                }
                return a.zero(zSign);
            }
            return pack_f256( zSign, 0, zSig );
        }
        zSig2 = 0;
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
    zSig2 = zSig.jamming( 1, zSig2 );
 roundAndPack:
    return RoundPackTo_f256( zSign, zExp, zSig, zSig2 );

}

/*----------------------------------------------------------------------------
| Returns the result of subtracting the absolute values of the 256-bit
| precision floating-point values `a' and `b'.  If `zSign' is 1, the
| difference is negated before being returned.  `zSign' is ignored if the
| result is a NaN.  The subtraction is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f256 CSoftFPU::subSigs( f256 a, f256 b, sign_type zSign)
{
    fp_exponent_t aExp, bExp, zExp;
    u256   zSig;
    fp_exponent_t expDiff;

    const int FIXED_POINT = f256::BITS_PER_EXPONENT - 1;
    const u64 FIXED_HIDDEN_BIT = u64(1) << (f256::BITS_PER_HIGH_SIGNIFICAND + FIXED_POINT);

    u256 aSig = a.fraction();
    aExp = a.exponent();
    u256 bSig = b.fraction();
    bExp = b.exponent();

    expDiff = aExp - bExp;

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
    if ( bSig.m2 < aSig.m2 ) goto aBigger;
    if ( aSig.m2 < bSig.m2 ) goto bBigger;
    if ( bSig.m3 < aSig.m3 ) goto aBigger;
    if ( aSig.m3 < bSig.m3 ) goto bBigger;
    return a.zero( sign_type(get_rounding_mode() == FLOAT_ROUND_DOWNWARD) );
 bExpBigger:
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(b);
        return b.inf(zSign ^ SIGN_NEGATIVE);
    }
    if ( aExp == 0 ) {
        ++expDiff;
    }
    else {
        aSig.m0 |= FIXED_HIDDEN_BIT;
    }
    aSig.jamming( -expDiff );
    bSig.m0 |= FIXED_HIDDEN_BIT;
 bBigger:
    zSig = bSig - aSig;
    zExp = bExp;
    zSign = ! zSign;
    goto normalizeRoundAndPack;
 aExpBigger:
    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a);
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
    return NormalizeRoundPackTo_f256( zSign, zExp - FIXED_POINT, zSig );
}


f256 CSoftFPU::add( f256 a, f256 b )
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


f256 CSoftFPU::sub( f256 a, f256 b )
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

f256 CSoftFPU::mul( f256 a, f256 b )
{
    sign_type   a_sign, b_sign, zSign;
    fp_exponent_t  aExp, bExp, zExp;

    u256 aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    u256 bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();

    zSign = a_sign ^ b_sign;

    if ( aExp == a.EINF ) {
        if ( aSig || b.IsNan() ) {
            return propagate_nan(a, b);
        }
        if ( ( bExp || bSig ) == 0 ) goto invalid;
        return b.inf(zSign);
    }
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(b);
        if ( ( aExp || aSig ) == 0 ) {
 invalid:
            raise(FLOAT_FLAG_INVALID);
            return a.default_nan();
        }
        return b.inf(zSign);
    }

    if ( aExp == 0 ) {
        if ( ! aSig ) return a.zero(zSign);
        aExp = aSig.normalize_subnormal();
    }
    if ( bExp == 0 ) {
        if ( ! bSig ) return b.zero(zSign);
        bExp = bSig.normalize_subnormal();
    }

    zExp = aExp + bExp - b.BIAS;
    aSig.m0 |= a.HIDDEN_BIT;
    bSig.m0 |= b.HIDDEN_BIT;
    bSig <<= f256::BITS_PER_EXPONENT;

    u512 p;
    p.mul( aSig, bSig );

    LOG_DEBUG(LOG_FPU) << aSig << '\n' << bSig << '\n' << p;

    if ( (p.m0 & a.HIDDEN_BIT) == 0 ) {
        p <<= 1;
        --zExp;
    }

    return RoundPackTo_f256( zSign, zExp, p, p.extra_jamming() );
}


f256 CSoftFPU::div( f256 a, f256 b )
{
    sign_type   a_sign, b_sign, zSign;
    u256   zSig;
    u320     term, rem;

    u256 aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    a_sign = a.sign();

    u256 bSig = b.fraction();
    fp_exponent_t bExp = b.exponent();
    b_sign = b.sign();

    zSign = a_sign ^ b_sign;

    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a, b);
        if ( bExp == b.EINF ) {
            if ( bSig ) return propagate_nan(b);
            goto invalid;
        }
        return a.inf(zSign);
    }

    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(b);
        return b.zero(zSign);
    }

    if ( bExp == 0 ) {
        if ( ! bSig ) {
            if ( ( aExp || aSig ) == 0 ) {
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
        if ( ! aSig ) return a.zero(zSign);
        aExp = aSig.normalize_subnormal();
    }

    fp_exponent_t zExp = aExp - bExp + (b.BIAS - 2);

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
    rem = u320(aSig.m0, aSig.m1, aSig.m2, aSig.m3, 0) - term;
    while ( i64(rem.m0) < 0 ) {
        LOG_DEBUG(LOG_FPU) << "m0\n";
        --zSig.m0;
        rem += bSig;
    }

    zSig.m1 = estimateDiv128To64( rem.m1, rem.m2, bSig.m0 );
    term.mul( bSig, zSig.m1 );
    rem = u320(rem.m1, rem.m2, rem.m3, rem.m4, 0) - term;
    while ( i64(rem.m0) < 0 ) {
        LOG_DEBUG(LOG_FPU) << "m1\n";
        --zSig.m1;
        rem += bSig;
    }

    zSig.m2 = estimateDiv128To64( rem.m1, rem.m2, bSig.m0 );
    term.mul( bSig, zSig.m2 );
    rem = u320(rem.m1, rem.m2, rem.m3, rem.m4, 0) - term;
    while ( i64(rem.m0) < 0 ) {
        LOG_DEBUG(LOG_FPU) << "m2\n";
        --zSig.m2;
        rem += bSig;
    }

    zSig.m3 = estimateDiv128To64( rem.m1, rem.m2, bSig.m0 );
    if ( lobits(zSig.m3, a.BITS_PER_EXPONENT - 1) <= 4 ) {
        term.mul( bSig, zSig.m3 );
        rem = u320(rem.m1, rem.m2, rem.m3, rem.m4, 0) - term;
        while ( i64(rem.m0) < 0 ) {
            LOG_DEBUG(LOG_FPU) << "m3\n";
            --zSig.m3;
            rem += bSig;
        }
        zSig.m3 |= (bool) rem;
    }

    u64 zSigExtra = zSig.jamming( a.BITS_PER_EXPONENT, 0 );
    return RoundPackTo_f256( zSign, zExp, zSig, zSigExtra );
}


f256 CSoftFPU::rem( f256 a, f256 b )
{
    u256   aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type a_sign = a.sign();

    u256 bSig = b.fraction();
    fp_exponent_t  bExp = b.exponent();

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

    u320 term;
    u64 q = (bSig <= aSig);
    if ( q ) aSig -= bSig;
    expDiff -= 64;
    while ( expDiff > 0 ) {
        q = estimateDiv128To64( aSig.m0, aSig.m1, bSig.m0 );
        q = ( q > 4 ) ? q - 4 : 0;
        term.mul( bSig, q );
        term <<= 61;
        aSig <<= 61;
        aSig = u256(aSig.m0, aSig.m1, aSig.m2, 0) - u256(term.m0, term.m1, term.m2, term.m3);
        expDiff -= 61;
    }

    if ( expDiff > -64 ) {
        q = estimateDiv128To64( aSig.m0, aSig.m1, bSig.m0 );
        q = ( q > 4 ) ? q - 4 : 0;
        q >>= - expDiff;
        bSig >>= 12;
        expDiff += (64 - 12);
        if ( expDiff < 0 ) {
            aSig >>= (- expDiff);
        }
        else if ( expDiff > 0 ) {
            aSig <<= expDiff;
        }
        term.mul( bSig, q );
        aSig -= u256( term.m1, term.m2, term.m3, term.m4 );
    }
    else {
        aSig >>= 12;
        bSig >>= 12;
    }

    u256 alternateASig;
    do {
        alternateASig = aSig;
        ++q;
        aSig -= bSig;
    } while ( i64(aSig.m0) >= 0 );

    u256 sigMean = aSig + alternateASig;

    if ( ( i64(sigMean.m0) < 0 ) || ( !sigMean && ( q & 1 ) ) ) {
        aSig = alternateASig;
    }

    sign_type zSign = get_sign( i64(aSig.m0) );
    if ( zSign ) aSig = -aSig;
    return NormalizeRoundPackTo_f256( a_sign ^ zSign, bExp - 8, aSig );
}


/**********************************************************************************
* square root
 N = (2^W a0 + a1) ^ 2 = 2^2W a0a0 + 2^W a0a1 + a1a1
**********************************************************************************/
f256 CSoftFPU::sqrt( f256 a )
{
    u256     zSig;
    u128     term;

    u256    aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type    a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a);
        if ( ! a_sign ) return a;
        goto invalid;
    }
    if ( a_sign ) {
        if ( aExp == 0 && !aSig ) return a;
 invalid:
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( aExp == 0 ) {
        if ( ! aSig ) return a.zero(SIGN_POSITIVE);
        aExp = aSig.normalize_subnormal();
    }

    fp_exponent_t zExp = ( ( aExp - a.BIAS ) >> 1 ) + (a.BIAS - 1);

    aSig.m0 |= a.HIDDEN_BIT;
    zSig.m0 = estimateSqrt32( aExp, u32( aSig.m0 >> (64 - 32 - a.BITS_PER_EXPONENT) ) );
    aSig <<= ( a.BITS_PER_EXPONENT - 2 - ( aExp & 1 ) );

    zSig.m0 = estimateDiv128To64( aSig.m0, aSig.m1, zSig.m0<<32 ) + ( zSig.m0<<30 );
    u64 doubleZSig0 = zSig.m0<<1;
    mul64To128( zSig.m0, zSig.m0, &term.m0, &term.m1 );
    u128 rem2 = u128(aSig.m0, aSig.m1) - term;
    while ( i64(rem2.m0) < 0 ) {
        --zSig.m0;
        doubleZSig0 -= 2;
        rem2 += u128( zSig.m0>>63, doubleZSig0 | 1 );
    }
    assert( (doubleZSig0 >> 63) != 0 );

    zSig.m1 = estimateDiv128To64( rem2.m1, aSig.m2, doubleZSig0 );
    if ( zSig.m1 == 0 ) zSig.m1 = 1;
    mul64To128( doubleZSig0, zSig.m1, &term.m0, &term.m1 );
    rem2 = u128( rem2.m1, aSig.m2 ) - term;
    mul64To128( zSig.m1, zSig.m1, &term.m0, &term.m1 );
    u192 rem3 = u192( rem2.m0, rem2.m1, aSig.m3 ) - u192( 0, term.m0, term.m1 );
    while ( i64(rem3.m0) < 0 ) {
        --zSig.m1;
        u192 tmp = u192(0, zSig.m0, zSig.m1) << 1;
        tmp.m2 |= 1;
        rem3 += tmp;
    }

    zSig.m2 = estimateDiv128To64( rem3.m1, rem3.m2, doubleZSig0 );
    if ( zSig.m2 == 0 ) zSig.m2 = 1;
    mul64To128( doubleZSig0, zSig.m2, &term.m0, &term.m1 );
    rem2 = u128( rem3.m1, rem3.m2 ) - term;
    mul64To128( zSig.m2, zSig.m2, &term.m0, &term.m1 );
    u256 rem4 = u256( rem2.m0, rem2.m1, 0, 0 ) - u256( 0, 0, term.m0, term.m1 );
    mul64To128( zSig.m1, zSig.m2, &term.m0, &term.m1 );
    rem4 -= u256( 0, term.m0, term.m1, 0 ) << 1;
    while ( i64(rem4.m0) < 0 ) {
        --zSig.m2;
        u256 tmp = u256(0, zSig.m0, zSig.m1, zSig.m2) << 1;
        tmp.m3 |= 1;
        rem4 += tmp;
    }

    zSig.m3 = estimateDiv128To64( rem4.m1, rem4.m2, doubleZSig0 );
    if (lobits(zSig.m3, a.BITS_PER_EXPONENT - 2) <= 5 ) {
        if ( zSig.m3 == 0 ) zSig.m3 = 1;

        mul64To128( doubleZSig0, zSig.m3, &term.m0, &term.m1 );
        rem2 = u128( rem4.m1, rem4.m2 ) - term;
        mul64To128( zSig.m3, zSig.m3, &term.m0, &term.m1 );
        u320 rem5 = u320( rem2.m0, rem2.m1, rem4.m3, 0, 0 ) - u320( 0, 0, 0, term.m0, term.m1 );

        mul64To128( zSig.m2, zSig.m3, &term.m0, &term.m1 );
        rem5 -= u320( 0, 0, term.m0, term.m1, 0 ) << 1;

        mul64To128( zSig.m1, zSig.m3, &term.m0, &term.m1 );
        rem5 -= u320( 0, term.m0, term.m1, 0, 0 ) << 1;

        while ( i64(rem5.m0) < 0 ) {
            --zSig.m3;
            u320 tmp(0, zSig.m0, zSig.m1, zSig.m2, zSig.m3);
            tmp <<= 1;
            tmp.m4 |= 1;
            rem5 += tmp;
        }
        zSig.m3 |= (bool) rem5;
    }

    u64 const zSigExtra = zSig.jamming( a.BITS_PER_EXPONENT - 1, 0 );
    return RoundPackTo_f256( SIGN_POSITIVE, zExp, zSig, zSigExtra );
}



/*----------------------------------------------------------------------------
| Returns the result of multiplying the 256-bit precision floating-point values
| `a' and `b' then adding 'c', with no intermediate rounding step after the
| multiplication.  The operation is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic 754-2008.
| The flags argument allows the caller to select negation of the
| addend, the intermediate product, or the final result. (The difference
| between this and having the caller do a separate negation is that negating
| externally will flip the sign bit on NaNs.)
*----------------------------------------------------------------------------*/
f256 CSoftFPU::muladd(f256 a, f256 b, f256 c, muladd_negate_t flags)
{
    sign_type   a_sign, b_sign, cSign, zSign, pSign;
    fp_exponent_t  aExp, bExp, cExp, pExp, zExp, expDiff;
    u256   cSig2, cSig3;
    bool        pInf, pZero, infzero;
    u512   zSig;

    u256 aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    u256 bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();

    u256 cSig = c.fraction();
    cExp = c.exponent();
    cSign = c.sign();

    infzero = ((aExp == 0 && ! aSig && bExp == b.EINF && ! bSig) ||
               (aExp == a.EINF && ! aSig && bExp == 0 && ! bSig));

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
            if ( ! cSig ) {
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
        return pack_f256( cSign, cExp, cSig );
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
     * flavour that RoundPackTo_f256() takes.
     */
    pExp = aExp + bExp - (b.BIAS - 1);
    aSig.m0 |= a.HIDDEN_BIT;
    bSig.m0 |= b.HIDDEN_BIT;
    aSig <<= ( a.BITS_PER_EXPONENT - 1);
    bSig <<= ( b.BITS_PER_EXPONENT    );

    u512 pSig;
    pSig.mul( aSig, bSig );
    if ( i64(pSig.m0 << 1) >= 0 ) {
        pSig <<= 1;
        pExp--;
    }
    assert ( i64(pSig.m0) >= 0 );

    zSign = pSign;

    // Now [pSig0:pSig3] is the significand of the multiply, with the explicit bit in position 254 (p0:62).
    if (cExp == 0) {
        if ( ! cSig ) {
            // Throw out the special case of c being an exact zero now
            pSig.jamming( c.BITS_PER_EXPONENT - 1 );
            return RoundPackTo_f256( zSign, pExp - 1, pSig, pSig.extra_jamming() );
        }
        cExp = cSig.normalize_subnormal();
    }

    /* Shift cSig and add the explicit bit so [cSig0:cSig1] is the
     * significand of the addend, with the explicit bit in position 126.
     */
    cSig.m0 |= c.HIDDEN_BIT;
    cSig <<= ( c.BITS_PER_EXPONENT - 1 );
    u512 cSigX(cSig.m0, cSig.m1, cSig.m2, cSig.m3, 0, 0, 0, 0);
    expDiff = pExp - cExp;
    assert ( i64(cSig.m0) >= 0 );

    if ( pSign == cSign ) {
        // Addition
        if ( expDiff > 0 ) {
            // scale c to match p
            cSigX.jamming( expDiff );
            zExp = pExp;
        } else if ( expDiff < 0 ) {
            // scale p to match c
            pSig.jamming( -expDiff );
            zExp = cExp;
        } else {
            // no scaling needed
            zExp = cExp;
        }

        // Add significands and make sure explicit bit ends up in posn 254
        zSig.add( pSig, cSigX );

        if ( i64(zSig.m0) < 0 ) {
            zSig.jamming( c.BITS_PER_EXPONENT );
        } else {
            zSig.jamming( c.BITS_PER_EXPONENT - 1 );
            zExp--;
        }

        return RoundPackTo_f256(zSign, zExp, zSig, zSig.extra_jamming() );

    } else {
        // Subtraction
        if ( expDiff > 0 ) {
            cSigX.jamming( expDiff );
            zSig.sub( pSig, cSigX );
            zExp = pExp;
        } else if ( expDiff < 0 ) {
            pSig.jamming( -expDiff );
            zSig.sub( cSigX, pSig );
            zExp = cExp;
            zSign = ! zSign;
        } else {
            zExp = pExp;
            if ( cSigX < pSig ) {
                zSig.sub( pSig, cSigX );
            } else if ( pSig < cSigX ) {
                zSig.sub( cSigX, pSig );
                zSign = ! zSign;
            } else {
                // Exact zero
                //zSign = signflip;
                //if (get_rounding_mode() == FLOAT_ROUND_DOWNWARD) {
                //    zSign = ! zSign;
                //}
                return c.zero(sign_type(get_rounding_mode() == FLOAT_ROUND_DOWNWARD));
            }
        }

        while (zSig.m0 == 0) {
           zSig.m0 = zSig.m1;
           zSig.m1 = zSig.m2;
           zSig.m2 = zSig.m3;
           zSig.m3 = zSig.m4;
           zSig.m4 = zSig.m5;
           zSig.m5 = zSig.m6;
           zSig.m6 = zSig.m7;
           zSig.m7 = 0;
           zExp -= 64;
        }

        int shiftcount = countLeadingZeros64(zSig.m0) - f256::BITS_PER_EXPONENT;
        if (shiftcount > 0) {
           zSig <<= shiftcount;
           zExp -= shiftcount;
        } else if ( shiftcount < 0 ) {
           zSig.jamming( - shiftcount );
           zExp -= shiftcount;
        }

        return RoundPackTo_f256(zSign, zExp - f256::BITS_PER_EXPONENT, zSig, zSig.extra_jamming() );

    }

}

} // namespace fpu
} // namespace postrisc
