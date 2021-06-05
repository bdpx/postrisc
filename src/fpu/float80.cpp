#include "util/common.hpp"
#include "util.hpp"
#include "floatimpl.hpp"

namespace postrisc {
namespace fpu {

commonNaNT SoftFPU::ToCommonNaN(f80 a)
{
    commonNaNT z;

    if ( a.IsSignalingNan() )
       raise(FLOAT_FLAG_INVALID);

    if ( a.fraction() & a.UNHIDDEN_BIT ) {
        z.sign = a.sign();
        z.low  = 0;
        z.high = a.fraction() << 1; // skip explicit bit
    } else {
        // explicit bit not set!!!!!
        z.sign = a.default_nan().sign();
        z.low  = 0;
        z.high = a.default_nan().fraction() << 1;
    }
    return z;
}


f80 SoftFPU::to_f80(commonNaNT a)
{
    if (default_nan_mode()) {
        return f80::default_nan();
    }

    u64 mantissa = a.high >> 1;
    if (mantissa)
        return f80::make( ( u16(a.sign)<<15 ) | f80::EINF, f80::UNHIDDEN_BIT | U64C( 0x4000000000000000 ) | mantissa);
    else
        return f80::default_nan();
}


/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and extended significand formed by the concatenation of `zSig0' and `zSig1',
| and returns the proper extended double-precision floating-point value
| corresponding to the abstract input.  Ordinarily, the abstract value is
| rounded and packed into the extended double-precision format, with the
| inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded to
| a subnormal number, and the underflow and inexact exceptions are raised if
| the abstract input cannot be represented exactly as a subnormal extended
| double-precision floating-point number.
|     If `roundingPrecision' is 32 or 64, the result is rounded to the same
| number of bits as single or double precision, respectively.  Otherwise, the
| result is rounded to the full precision of the extended double-precision
| format.
|     The input significand must be normalized or smaller.  If the input
| significand is not normalized, `zExp' must be 0; in that case, the result
| returned is a subnormal number, and it must not require rounding.  The
| handling of underflow and overflow follows the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f80 SoftFPU::RoundPackTo_f80(rounding_precision_t roundingPrecision,
        sign_type zSign, fp_exponent_t zExp, f80::fraction_t zSig0, u64 zSig1)
{
    bool    increment;
    u64  roundIncrement, roundMask, roundBits;  //???

    const fp_exponent_t NNNN = f80::EINF - 2; //0x7FFD

    rounding_mode_t roundingMode = get_rounding_mode();
    bool roundNearestEven = ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN );
    if ( roundingPrecision == FLOAT_ROUND_PRECISION_80 ) goto precision80;
    if ( roundingPrecision == FLOAT_ROUND_PRECISION_64 ) {
        roundIncrement = u64(1) << (64 - 53 - 1);
        roundMask = makemask64(64 - 53);
    }
    else if ( roundingPrecision == FLOAT_ROUND_PRECISION_32 ) {
        roundIncrement = u64(1) << (64 - 24 - 1);
        roundMask = makemask64(64 - 24);
    }
    else {
        goto precision80;
    }
    zSig0 |= ( zSig1 != 0 );
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = roundMask;
            if ( zSign ) {
                if ( roundingMode == FLOAT_ROUND_UPWARD ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLOAT_ROUND_DOWNWARD ) roundIncrement = 0;
            }
        }
    }
    roundBits = zSig0 & roundMask;

    if ( u32(NNNN) <= u32( zExp - 1 ) ) {
        if (    ( 0x7FFE < zExp )
             || ( ( zExp == 0x7FFE ) && ( zSig0 + roundIncrement < zSig0 ) )
           ) {
            goto overflow;
        }
        if ( zExp <= 0 ) {
            if (flush_to_zero()) {
                raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                return f80::zero(zSign);
            }
            bool isTiny = tininess_before_rounding()
                || ( zExp < 0 )
                || ( zSig0 <= zSig0 + roundIncrement );
            zSig0 = shift64RightJamming( zSig0, 1 - zExp);
            zExp = 0;
            roundBits = zSig0 & roundMask;
            if ( isTiny && roundBits ) raise(FLOAT_FLAG_UNDERFLOW);
            if ( roundBits ) raise(FLOAT_FLAG_INEXACT);
            zSig0 += roundIncrement;
            if ( i64(zSig0) < 0 ) zExp = 1;
            roundIncrement = roundMask + 1;
            if ( roundNearestEven && ( roundBits<<1 == roundIncrement ) ) {
                roundMask |= roundIncrement;
            }
            zSig0 &= ~ roundMask;
            return pack_f80( zSign, zExp, zSig0 );
        }
    }
    if ( roundBits ) raise(FLOAT_FLAG_INEXACT);
    zSig0 += roundIncrement;
    if ( zSig0 < roundIncrement ) {
        ++zExp;
        zSig0 = f80::UNHIDDEN_BIT;
    }
    roundIncrement = roundMask + 1;
    if ( roundNearestEven && ( roundBits<<1 == roundIncrement ) ) {
        roundMask |= roundIncrement;
    }
    zSig0 &= ~ roundMask;
    if ( zSig0 == 0 ) zExp = 0;
    return pack_f80( zSign, zExp, zSig0 );

 precision80:
    increment = ( i64(zSig1) < 0 );
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            increment = false;
        }
        else {
            if ( zSign ) {
                increment = ( roundingMode == FLOAT_ROUND_DOWNWARD ) && zSig1;
            }
            else {
                increment = ( roundingMode == FLOAT_ROUND_UPWARD ) && zSig1;
            }
        }
    }
    if ( 0x7FFD <= u32( zExp - 1 ) ) {
        if (    ( 0x7FFE < zExp )
             || (    ( zExp == 0x7FFE )
                  && ( zSig0 == std::numeric_limits<u64>::max() )
                  && increment
                )
           ) {
            roundMask = 0;
 overflow:
            raise(FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
            if (                 ( roundingMode == FLOAT_ROUND_TOWARDZERO )
                 || (   zSign && ( roundingMode == FLOAT_ROUND_UPWARD     ) )
                 || ( ! zSign && ( roundingMode == FLOAT_ROUND_DOWNWARD   ) )
               ) {
                return pack_f80( zSign, 0x7FFE, ~ roundMask );
            }
            return f80::inf(zSign);
        }
        if ( zExp <= 0 ) {
            bool isTiny = tininess_before_rounding()
                || ( zExp < 0 )
                || ! increment
                || ( zSig0 < std::numeric_limits<u64>::max() );
            shift64ExtraRightJamming( zSig0, zSig1, 1 - zExp, &zSig0, &zSig1 );
            zExp = 0;
            if ( isTiny && zSig1 ) raise(FLOAT_FLAG_UNDERFLOW);
            if ( zSig1 ) raise(FLOAT_FLAG_INEXACT);
            if ( roundNearestEven ) {
                increment = ( i64(zSig1) < 0 );
            }
            else {
                if ( zSign ) {
                    increment = ( roundingMode == FLOAT_ROUND_DOWNWARD ) && zSig1;
                }
                else {
                    increment = ( roundingMode == FLOAT_ROUND_UPWARD ) && zSig1;
                }
            }
            if ( increment ) {
                ++zSig0;
                zSig0 &=  ~ f80::fraction_t( ( u64( zSig1<<1 ) == 0 ) & roundNearestEven );
                if ( i64(zSig0) < 0 ) zExp = 1;
            }
            return pack_f80( zSign, zExp, zSig0 );
        }
    }
    if ( zSig1 ) raise(FLOAT_FLAG_INEXACT);
    if ( increment ) {
        ++zSig0;
        if ( zSig0 == 0 ) {
            ++zExp;
            zSig0 = f80::UNHIDDEN_BIT;
        }
        else {
            zSig0 &= ~ f80::fraction_t( ( ( zSig1<<1 ) == 0 ) & roundNearestEven );
        }
    }
    else {
        if ( zSig0 == 0 ) zExp = 0;
    }
    return pack_f80( zSign, zExp, zSig0 );
}

/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent
| `zExp', and significand formed by the concatenation of `zSig0' and `zSig1',
| and returns the proper extended double-precision floating-point value
| corresponding to the abstract input.  This routine is just like
| `RoundPackTo_f80' except that the input significand does not have to be
| normalized.
*----------------------------------------------------------------------------*/

f80 SoftFPU::NormalizeRoundPackTo_f80(
     rounding_precision_t roundingPrecision,
     sign_type zSign, fp_exponent_t zExp, u64 zSig0, u64 zSig1)
{
    if ( zSig0 == 0 ) {
        zSig0 = zSig1;
        zSig1 = 0;
        zExp -= 64;
    }
    assert( zSig0 );
    int shiftCount = countLeadingZeros64( zSig0 );
    shortShift128Left( zSig0, zSig1, shiftCount, &zSig0, &zSig1 );
    zExp -= shiftCount;
    return RoundPackTo_f80( roundingPrecision, zSign, zExp, zSig0, zSig1);
}


f80 SoftFPU::to_f80( i32 a )
{
    sign_type   zSign;
    u32      absA;
    u64      zSig;

    if ( a == 0 ) return f80::zero(SIGN_POSITIVE);
    zSign = get_sign(a);
    absA = zSign ? - a : a;
    int shiftCount = countLeadingZeros32( absA ) + 32;
    zSig = absA;
    return pack_f80( zSign, f80::BIAS + 63 - shiftCount, zSig<<shiftCount ); //0x403E
}


f80 SoftFPU::to_f80( i64 a )
{
    sign_type   zSign;
    u64      absA;

    if ( a == 0 ) return f80::zero( SIGN_POSITIVE);
    zSign = get_sign(a);
    absA = zSign ? - a : a;
    int shiftCount = countLeadingZeros64( absA );
    return pack_f80( zSign, f80::BIAS + 63 - shiftCount, absA<<shiftCount ); //0x403E

}

/*----------------------------------------------------------------------------
| Returns the result of converting the extended double-precision floating-
| point value `a' to the 32-bit two's complement integer format.  The
| conversion is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic---which means in particular that the conversion
| is rounded according to the current rounding mode.  If `a' is a NaN, the
| largest positive integer is returned.  Otherwise, if the conversion
| overflows, the largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32( f80 a )
{
    f80::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type   a_sign = a.sign();

    if ( a.IsNan() ) a_sign = SIGN_POSITIVE;
    fp_exponent_t shiftCount = ( a.BIAS + (63 - 7) ) - aExp;  // 0x4037  //56
    if ( shiftCount <= 0 ) shiftCount = 1;
    aSig = shift64RightJamming( aSig, shiftCount);
    return round_pack_to_i32(INT_NAN_F80, a_sign, aSig );

}

/*----------------------------------------------------------------------------
| Returns the result of converting the extended double-precision floating-
| point value `a' to the 32-bit two's complement integer format.  The
| conversion is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic, except that the conversion is always rounded
| toward zero.  If `a' is a NaN, the largest positive integer is returned.
| Otherwise, if the conversion overflows, the largest integer with the same
| sign as `a' is returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32_round_to_zero( f80 a )
{
    sign_type   a_sign;
    fp_exponent_t aExp, shiftCount;
    f80::fraction_t aSig, savedASig;
    i32       z;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    if ( (a.BIAS + 31) < aExp ) {  //0x401E
        if ( ( aExp == a.EINF ) && u64( aSig<<1 ) ) a_sign = SIGN_POSITIVE;
        goto invalid;
    }
    else if ( aExp < a.BIAS ) {
        if ( aExp || aSig ) raise(FLOAT_FLAG_INEXACT);
        return 0;
    }
    shiftCount = (a.BIAS + 63) - aExp; //0x403E
    savedASig = aSig;
    aSig >>= shiftCount;
    z = aSig;
    if ( a_sign ) z = - z;
    if ( ( z < 0 ) ^ a_sign ) {
 invalid:
        raise(FLOAT_FLAG_INVALID);
        return a_sign ? std::numeric_limits<i32>::min() : std::numeric_limits<i32>::max();
        //return std::numeric_limits<i32>::min();
    }
    if ( ( aSig<<shiftCount ) != savedASig ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    return z;

}

/*----------------------------------------------------------------------------
| Returns the result of converting the extended double-precision floating-
| point value `a' to the 64-bit two's complement integer format.  The
| conversion is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic---which means in particular that the conversion
| is rounded according to the current rounding mode.  If `a' is a NaN,
| the largest positive integer is returned.  Otherwise, if the conversion
| overflows, the largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i64 SoftFPU::to_i64( f80 a )
{
    sign_type   a_sign;
    fp_exponent_t aExp, shiftCount;
    f80::fraction_t aSig, aSigExtra;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    shiftCount = (a.BIAS + 63) - aExp; //0x403E
    if ( shiftCount <= 0 ) {
        if ( shiftCount ) {
            raise(FLOAT_FLAG_INVALID);
            if (    ! a_sign || a.IsNan() ) {
                return std::numeric_limits<i64>::max();
            }
            return std::numeric_limits<i64>::min();
        }
        aSigExtra = 0;
    }
    else {
        shift64ExtraRightJamming( aSig, 0, shiftCount, &aSig, &aSigExtra );
    }
    return round_pack_to_i64(INT_NAN_F80, a_sign, aSig, aSigExtra );

}

/*----------------------------------------------------------------------------
| Returns the result of converting the extended double-precision floating-
| point value `a' to the 64-bit two's complement integer format.  The
| conversion is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic, except that the conversion is always rounded
| toward zero.  If `a' is a NaN, the largest positive integer is returned.
| Otherwise, if the conversion overflows, the largest integer with the same
| sign as `a' is returned.
*----------------------------------------------------------------------------*/

i64 SoftFPU::to_i64_round_to_zero( f80 a )
{
    sign_type   a_sign;
    fp_exponent_t aExp, shiftCount;
    f80::fraction_t  aSig;
    i64       z;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    shiftCount = aExp - (a.BIAS + 63); //0x403E
    if ( 0 <= shiftCount ) {
        if ( a != f80::make(SIGN_NEGATIVE, 63) ) { //0xC03E
            raise(FLOAT_FLAG_INVALID);
            if ( ! a_sign || a.IsNan() ) {
                return std::numeric_limits<i64>::max();
            }
        }
        return std::numeric_limits<i64>::min();
    }
    else if ( aExp < a.BIAS ) {
        if ( aExp | aSig ) raise(FLOAT_FLAG_INEXACT);
        return 0;
    }
    z = aSig>>( - shiftCount );
    if ( u64( aSig<<( shiftCount & 63 ) ) ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    if ( a_sign ) z = - z;
    return z;

}

/*----------------------------------------------------------------------------
| Rounds the extended double-precision floating-point value `a' to an integer,
| and returns the result as an extended quadruple-precision floating-point
| value.  The operation is performed according to the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f80 SoftFPU::round_to_int( f80 a )
{
    sign_type        a_sign;
    u64           lastBitMask, roundBitsMask;
    rounding_mode_t  roundingMode;

    fp_exponent_t aExp = a.exponent();
    if ( (a.BIAS + 63) <= aExp ) { //0x403E
        if ( ( aExp == a.EINF ) && u64( a.fraction()<<1 ) ) {
            return propagate_nan(a);
        }
        return a;
    }
    if ( aExp < a.BIAS ) {
        if ( ( aExp == 0 ) && ( u64( a.fraction()<<1 ) == 0 ) ) {
            return a;
        }
        raise(FLOAT_FLAG_INEXACT);
        a_sign = a.sign();
        switch ( get_rounding_mode() ) {
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( ( aExp == a.BIAS - 1 ) && u64( a.fraction()<<1 ) ) {
                return  pack_f80( a_sign, a.BIAS, a.UNHIDDEN_BIT);
            }
            break;
         case FLOAT_ROUND_DOWNWARD:
            return  a_sign ? pack_f80( SIGN_NEGATIVE, a.BIAS, a.UNHIDDEN_BIT) : a.zero( SIGN_POSITIVE );
         case FLOAT_ROUND_UPWARD:
            return  a_sign ? a.zero( SIGN_NEGATIVE ) : pack_f80( SIGN_POSITIVE, a.BIAS, a.UNHIDDEN_BIT);
         default:
         case FLOAT_ROUND_TOWARDZERO:
             break;
        }
        return a.zero( a_sign );
    }
    lastBitMask = 1;
    lastBitMask <<= (a.BIAS + 63) - aExp; //0x403E
    roundBitsMask = lastBitMask - 1;

    f80 z = a;
    roundingMode = get_rounding_mode();
    if ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN ) {
        z.low += lastBitMask>>1;
        if ( ( z.low & roundBitsMask ) == 0 ) z.low &= ~ lastBitMask;
    }
    else if ( roundingMode != FLOAT_ROUND_TOWARDZERO ) {
        if ( z.sign() ^ ( roundingMode == FLOAT_ROUND_UPWARD ) ) {
            z.low += roundBitsMask;
        }
    }
    z.low &= ~ roundBitsMask;
    if ( z.low == 0 ) {
        ++z.high;
        z.low = z.UNHIDDEN_BIT;
    }
    if ( z.low != a.low ) raise(FLOAT_FLAG_INEXACT);
    return z;
}


f80 SoftFPU::round_to_int(f80 a, rounding_mode_t rmode)
{
    rounding_mode_t old = get_rounding_mode();
    set_rounding_mode(rmode);
    f80 t = round_to_int(a);
    set_rounding_mode(old);
    return t;
}


/*----------------------------------------------------------------------------
| Returns the binary log of the extended-precision floating-point value `a'.
| The operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
f80 SoftFPU::log2(f80 a)
{
    sign_type  a_sign, zSign;
    fp_exponent_t aExp;
    f80::fraction_t aSig, aSig0, aSig1, zSig, i;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    if ( aExp == 0 ) {
        if ( aSig == 0 ) {
           raise(FLOAT_FLAG_DIVBYZERO);
           return a.inf(SIGN_NEGATIVE);
        }
        aExp = normalize_subnormal_f80( aSig );
    }
    if ( a_sign ) {
        raise(FLOAT_FLAG_INVALID);
        if ( aExp == a.EINF && aSig ) return propagate_nan(a);
        return a.default_nan();
    }
    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a);
        return a;
    }

    aExp -= a.BIAS;
    //aSig |= a.HIDDEN_BIT;
    zSign = get_sign(aExp);
    zSig = u64(aExp) << (63);
    //aSig = (aSig >> 1) | (aSig & 1);

#if 0
    zSig = u64(aExp) << a.BITS_PER_SIGNIFICAND;
    for (i = u64(1) << 51; i > 0; i >>= 1) {
        mul64To128( aSig, aSig, &aSig0, &aSig1 );
        aSig = ( aSig0 << 12 ) | ( aSig1 >> f64::BITS_PER_SIGNIFICAND );
        if ( aSig & ( u64(1) << (f64::BITS_PER_SIGNIFICAND + 1) ) ) {
            aSig >>= 1;
            zSig |= i;
        }
    }
#endif

    for (i = u64(1) << 62; i > 0; i >>= 1) {
        mul64To128( aSig, aSig, &aSig0, &aSig1 );
        aSig = ( aSig0 << 1 ) | ( aSig1 >> 63 );
        if ( (aSig0 >> 63) & 1) {
            aSig >>= 1;
            aSig |= u64(1) << 63;
            zSig |= i;
        }
    }

    if ( zSign ) {
        if ( zSig ) {
          zSig = 0 - zSig; // unary minus unsigned error
          aExp = ~ aExp;
        } else {
          aExp = - aExp;
        }
    }

    return NormalizeRoundPackTo_f80(get_rounding_precision(), zSign, a.BIAS + 64, aExp >> 1, zSig); //0x408
}


bool SoftFPU::eq_signaling( f80 a, f80 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    return
           ( a.low == b.low ) && (    ( a.high == b.high )
             || (    ( a.low == 0 )
                  && ( u16( ( a.high | b.high )<<1 ) == 0 ) )
           );
}


bool SoftFPU::le_signaling( f80 a, f80 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    if ( a_sign != b_sign ) {
        return a_sign || IsBothZero(a,b);
    }
    return
          a_sign ? le128( b.high, b.low, a.high, a.low )
                : le128( a.high, a.low, b.high, b.low );
}


bool SoftFPU::lt_signaling( f80 a, f80 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    if ( a_sign != b_sign ) {
        return a_sign && !IsBothZero(a,b);
    }
    return
          a_sign ? lt128( b.high, b.low, a.high, a.low )
                : lt128( a.high, a.low, b.high, b.low );
}


bool SoftFPU::unordered( f80 a, f80 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return true;
    }
    return false;
}


bool SoftFPU::eq_quiet( f80 a, f80 b )
{

    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    return
           ( a.low == b.low )
        && (    ( a.high == b.high )
             || (    ( a.low == 0 )
                  && ( u16 ( ( a.high | b.high )<<1 ) == 0 ) )
           );
}


bool SoftFPU::le_quiet( f80 a, f80 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    if ( a_sign != b_sign ) {
        return a_sign || IsBothZero(a,b);
    }
    return
          a_sign ? le128( b.high, b.low, a.high, a.low )
                : le128( a.high, a.low, b.high, b.low );
}


bool SoftFPU::lt_quiet( f80 a, f80 b )
{
    if ( a.IsNan() || b.IsNan() ) {
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
          a_sign ? lt128( b.high, b.low, a.high, a.low )
                : lt128( a.high, a.low, b.high, b.low );
}


bool SoftFPU::unordered_quiet( f80 a, f80 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return true;
    }
    return false;
}


f80 SoftFPU::to_f80( f64 a )
{
    f80::fraction_t aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type   a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f80( ToCommonNaN(a) );
        return f80::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f80::zero( a_sign );
        aExp = normalize_subnormal_f64( aSig );
    }
    aSig = ( aSig | a.HIDDEN_BIT ) << ( 63 - a.BITS_PER_SIGNIFICAND );
    return pack_f80(a_sign, aExp - a.BIAS + f80::BIAS, aSig); //0x3C00
}


f32 SoftFPU::to_f32( f80 a )
{
    f80::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( u64( aSig<<1 ) ) {
            return to_f32( ToCommonNaN(a) );
        }
        return f32::inf(a_sign);
    }
    aSig = shift64RightJamming( aSig, (63 - f32::BITS_PER_SIGNIFICAND - 7) );
    if ( aExp || aSig ) aExp -= (a.BIAS - f32::BIAS + 1); //0x3F81;
    return RoundPackTo_f32( a_sign, aExp, aSig );

}


f64 SoftFPU::to_f64( f80 a )
{
    f80::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( u64( aSig<<1 ) ) {
            return to_f64( ToCommonNaN(a) );
        }
        return f64::inf(a_sign);
    }
    f80::fraction_t zSig = shift64RightJamming( aSig, 1);
    if ( aExp || aSig ) aExp -= (a.BIAS - f64::BIAS + 1); //0x3C01;
    return RoundPackTo_f64( a_sign, aExp, zSig );

}


f80 SoftFPU::to_f80( f32 a )
{
    u32  aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f80( ToCommonNaN(a) );
        return f80::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f80::zero( a_sign );
        aExp = normalize_subnormal_f32( aSig );
    }
    aSig |= a.HIDDEN_BIT;
    return pack_f80( a_sign, aExp - a.BIAS + f80::BIAS, u64( aSig ) << (64 - 24) ); //0x3F80

}

// Multiply A by 2 raised to the power N.
f80 SoftFPU::scalbn( f80 a, int n )
{
    f80::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig<<1 ) {
            return propagate_nan(a);
        }
        return a;
    }

    if (aExp == 0 && aSig == 0)
        return a;

    if (n > 0x10000) {
        n = 0x10000;
    } else if (n < -0x10000) {
        n = -0x10000;
    }

    aExp += n;
    return NormalizeRoundPackTo_f80(get_rounding_precision(), a_sign, aExp, aSig, 0 );
}


f80 SoftFPU::merge(f80 sign, f80 exponent, f80 mantissa)
{
    u16 high = (sign.high & (U16C(1) << f80::BITS_PER_EXPONENT) ) |
              lobits(exponent.high, f80::BITS_PER_EXPONENT);
    return f80::make(high, mantissa.low);
}


float_relation SoftFPU::compare(f80 a, f80 b) {  return compare_internal(a, b, false);  }
float_relation SoftFPU::compare_quiet(f80 a, f80 b) {  return compare_internal(a, b, true);  }

f80 SoftFPU::min(f80 a, f80 b, bool ignore_nans) {  return minmax<f80>(a, b, ignore_nans, true);  }
f80 SoftFPU::max(f80 a, f80 b, bool ignore_nans) {  return minmax<f80>(a, b, ignore_nans, false); }

/*----------------------------------------------------------------------------
| Returns the result of adding the absolute values of the extended double-
| precision floating-point values `a' and `b'.  If `zSign' is 1, the sum is
| negated before being returned.  `zSign' is ignored if the result is a NaN.
| The addition is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f80
SoftFPU::addSigs( f80 a, f80 b, sign_type zSign)
{
    fp_exponent_t aExp, bExp, zExp;
    f80::fraction_t aSig, bSig, zSig0, zSig1;
    int      expDiff;

    aSig = a.fraction();
    aExp = a.exponent();
    bSig = b.fraction();
    bExp = b.exponent();

    expDiff = aExp - bExp;

    if ( 0 < expDiff ) {
        if ( aExp == a.EINF ) {
            if ( u64( aSig<<1 ) ) return propagate_nan(a, b);
            return a;
        }
        if ( bExp == 0 ) --expDiff;
        shift64ExtraRightJamming( bSig, 0, expDiff, &bSig, &zSig1 );
        zExp = aExp;
    }
    else if ( expDiff < 0 ) {
        if ( bExp == b.EINF ) {
            if ( u64( bSig<<1 ) ) return propagate_nan(a, b);
            return b.inf(zSign);
        }
        if ( aExp == 0 ) ++expDiff;
        shift64ExtraRightJamming( aSig, 0, - expDiff, &aSig, &zSig1 );
        zExp = bExp;
    }
    else {
        if ( aExp == a.EINF ) {
            if ( u64( ( aSig | bSig )<<1 ) ) {
                return propagate_nan(a, b);
            }
            return a;
        }
        zSig1 = 0;
        zSig0 = aSig + bSig;
        if ( aExp == 0 ) {
            zExp = normalize_subnormal_f80( zSig0 );
            goto roundAndPack;
        }
        zExp = aExp;
        goto shiftRight1;
    }
    zSig0 = aSig + bSig;
    if ( i64(zSig0) < 0 ) goto roundAndPack;
 shiftRight1:
    shift64ExtraRightJamming( zSig0, zSig1, 1, &zSig0, &zSig1 );
    zSig0 |= a.UNHIDDEN_BIT;
    ++zExp;
 roundAndPack:
    return RoundPackTo_f80(get_rounding_precision(), zSign, zExp, zSig0, zSig1 );
}

/*----------------------------------------------------------------------------
| Returns the result of subtracting the absolute values of the extended
| double-precision floating-point values `a' and `b'.  If `zSign' is 1, the
| difference is negated before being returned.  `zSign' is ignored if the
| result is a NaN.  The subtraction is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f80
SoftFPU::subSigs( f80 a, f80 b, sign_type zSign )
{
    fp_exponent_t aExp, bExp, zExp;
    f80::fraction_t aSig, bSig, zSig0, zSig1;
    int     expDiff;

    aSig = a.fraction();
    aExp = a.exponent();
    bSig = b.fraction();
    bExp = b.exponent();

    expDiff = aExp - bExp;

    if ( 0 < expDiff ) goto aExpBigger;
    if ( expDiff < 0 ) goto bExpBigger;
    if ( aExp == a.EINF ) {
        if ( u64( ( aSig | bSig )<<1 ) ) {
            return propagate_nan(a, b);
        }
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( aExp == 0 ) {
        aExp = 1;
        bExp = 1;
    }
    zSig1 = 0;
    if ( bSig < aSig ) goto aBigger;
    if ( aSig < bSig ) goto bBigger;
    return a.zero( sign_type(get_rounding_mode() == FLOAT_ROUND_DOWNWARD) );
 bExpBigger:
    if ( bExp == b.EINF ) {
        if ( u64( bSig<<1 ) ) return propagate_nan(a, b);
        return b.inf(zSign ^ SIGN_NEGATIVE);
    }
    if ( aExp == 0 ) ++expDiff;
    shift128RightJamming( aSig, 0, - expDiff, &aSig, &zSig1 );
 bBigger:
    sub128( bSig, 0, aSig, zSig1, &zSig0, &zSig1 );
    zExp = bExp;
    zSign = ! zSign;
    goto normalizeRoundAndPack;
 aExpBigger:
    if ( aExp == a.EINF ) {
        if ( u64( aSig<<1 ) ) return propagate_nan(a, b);
        return a;
    }
    if ( bExp == 0 ) --expDiff;
    shift128RightJamming( bSig, 0, expDiff, &bSig, &zSig1 );
 aBigger:
    sub128( aSig, 0, bSig, zSig1, &zSig0, &zSig1 );
    zExp = aExp;
 normalizeRoundAndPack:
    return NormalizeRoundPackTo_f80(get_rounding_precision(), zSign, zExp, zSig0, zSig1 );
}


f80 SoftFPU::add( f80 a, f80 b )
{
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    if ( a_sign == b_sign ) {
        return addSigs( a, b, a_sign );
    } else {
        return subSigs( a, b, a_sign );
    }
}


f80 SoftFPU::sub( f80 a, f80 b )
{
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    if ( a_sign == b_sign ) {
        return subSigs( a, b, a_sign );
    } else {
        return addSigs( a, b, a_sign );
    }
}


f80 SoftFPU::mul( f80 a, f80 b )
{
    sign_type    a_sign, b_sign, zSign;
    fp_exponent_t aExp, bExp, zExp;
    f80::fraction_t aSig, bSig, zSig0, zSig1;

    aSig  = a.fraction();
    aExp  = a.exponent();
    a_sign = a.sign();

    bSig  = b.fraction();
    bExp  = b.exponent();
    b_sign = b.sign();

    zSign = a_sign ^ b_sign;

    if ( aExp == a.EINF ) {
        if (    u64( aSig<<1 )
             || ( ( bExp == b.EINF ) && u64( bSig<<1 ) ) ) {
            return propagate_nan(a, b);
        }
        if ( ( bExp | bSig ) == 0 ) goto invalid;
        return a.inf(zSign);
    }

    if ( bExp == b.EINF ) {
        if ( u64( bSig<<1 ) ) return propagate_nan(a, b);
        if ( ( aExp | aSig ) == 0 ) {
 invalid:
            raise(FLOAT_FLAG_INVALID);
            return a.default_nan();
        }
        return b.inf(zSign);
    }

    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a.zero( zSign );
        aExp = normalize_subnormal_f80( aSig );
    }

    if ( bExp == 0 ) {
        if ( bSig == 0 ) return b.zero( zSign );
        bExp = normalize_subnormal_f80( bSig );
    }

    zExp = aExp + bExp - (b.BIAS - 1);
    mul64To128( aSig, bSig, &zSig0, &zSig1 );

    if ( 0 < i64(zSig0) ) {
        shortShift128Left( zSig0, zSig1, 1, &zSig0, &zSig1 );
        --zExp;
    }

    return RoundPackTo_f80(get_rounding_precision(), zSign, zExp, zSig0, zSig1 );
}


f80 SoftFPU::div( f80 a, f80 b )
{
    sign_type  a_sign, b_sign, zSign;
    fp_exponent_t aExp, bExp, zExp;
    f80::fraction_t aSig, bSig, zSig0, zSig1;
    u64     rem0, rem1, rem2, term0, term1, term2;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();
    zSign = a_sign ^ b_sign;
    if ( aExp == a.EINF ) {
        if ( u64( aSig<<1 ) ) return propagate_nan(a, b);
        if ( bExp == b.EINF ) {
            if ( u64( bSig<<1 ) ) return propagate_nan(a, b);
            goto invalid;
        }
        return b.inf(zSign);
    }
    if ( bExp == b.EINF ) {
        if ( u64( bSig<<1 ) ) return propagate_nan(a, b);
        return b.zero( zSign );
    }
    if ( bExp == 0 ) {
        if ( bSig == 0 ) {
            if ( ( aExp | aSig ) == 0 ) {
 invalid:
                raise(FLOAT_FLAG_INVALID);
                return b.default_nan();
            }
            raise(FLOAT_FLAG_DIVBYZERO);
            return b.inf(zSign);
        }
        bExp = normalize_subnormal_f80( bSig );
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a.zero( zSign );
        aExp = normalize_subnormal_f80( aSig );
    }
    zExp = aExp - bExp + (b.BIAS - 1);
    rem1 = 0;
    if ( bSig <= aSig ) {
        shift128Right( aSig, 0, 1, &aSig, &rem1 );
        ++zExp;
    }
    zSig0 = estimateDiv128To64( aSig, rem1, bSig );
    mul64To128( bSig, zSig0, &term0, &term1 );
    sub128( aSig, rem1, term0, term1, &rem0, &rem1 );
    while ( i64(rem0) < 0 ) {
        --zSig0;
        add128( rem0, rem1, 0, bSig, &rem0, &rem1 );
    }
    zSig1 = estimateDiv128To64( rem1, 0, bSig );
    if ( u64( zSig1<<1 ) <= 8 ) {
        mul64To128( bSig, zSig1, &term1, &term2 );
        sub128( rem1, 0, term1, term2, &rem1, &rem2 );
        while ( i64(rem1) < 0 ) {
            --zSig1;
            add128( rem1, rem2, 0, bSig, &rem1, &rem2 );
        }
        zSig1 |= ( ( rem1 | rem2 ) != 0 );
    }
    return RoundPackTo_f80(get_rounding_precision(), zSign, zExp, zSig0, zSig1 );

}


f80 SoftFPU::rem( f80 a, f80 b )
{
    sign_type  a_sign, zSign;
    fp_exponent_t aExp, bExp, expDiff;
    f80::fraction_t aSig0, aSig1, bSig;
    u64     q, term0, term1, alternateASig0, alternateASig1;

    aSig0 = a.fraction();
    aExp  = a.exponent();
    a_sign = a.sign();

    bSig = b.fraction();
    bExp = b.exponent();

    if ( aExp == a.EINF ) {
        if (    u64( aSig0<<1 )
             || ( ( bExp == b.EINF ) && u64( bSig<<1 ) ) ) {
            return propagate_nan(a, b);
        }
        goto invalid;
    }
    if ( bExp == b.EINF ) {
        if ( u64( bSig<<1 ) ) return propagate_nan(b);
        return a;
    }
    if ( bExp == 0 ) {
        if ( bSig == 0 ) {
 invalid:
            raise(FLOAT_FLAG_INVALID);
            return b.default_nan();
        }
        bExp = normalize_subnormal_f80( bSig );
    }
    if ( aExp == 0 ) {
        if ( u64( aSig0<<1 ) == 0 ) return a;
        aExp = normalize_subnormal_f80( aSig0 );
    }
    bSig |= b.UNHIDDEN_BIT;
    zSign = a_sign;
    expDiff = aExp - bExp;
    aSig1 = 0;
    if ( expDiff < 0 ) {
        if ( expDiff < -1 ) return a;
        shift128Right( aSig0, 0, 1, &aSig0, &aSig1 );
        expDiff = 0;
    }
    q = ( bSig <= aSig0 );
    if ( q ) aSig0 -= bSig;
    expDiff -= 64;
    while ( 0 < expDiff ) {
        q = estimateDiv128To64( aSig0, aSig1, bSig );
        q = ( 2 < q ) ? q - 2 : 0;
        mul64To128( bSig, q, &term0, &term1 );
        sub128( aSig0, aSig1, term0, term1, &aSig0, &aSig1 );
        shortShift128Left( aSig0, aSig1, 62, &aSig0, &aSig1 );
        expDiff -= 62;
    }
    expDiff += 64;
    if ( 0 < expDiff ) {
        q = estimateDiv128To64( aSig0, aSig1, bSig );
        q = ( 2 < q ) ? q - 2 : 0;
        q >>= 64 - expDiff;
        mul64To128( bSig, q<<( 64 - expDiff ), &term0, &term1 );
        sub128( aSig0, aSig1, term0, term1, &aSig0, &aSig1 );
        shortShift128Left( 0, bSig, 64 - expDiff, &term0, &term1 );
        while ( le128( term0, term1, aSig0, aSig1 ) ) {
            ++q;
            sub128( aSig0, aSig1, term0, term1, &aSig0, &aSig1 );
        }
    }
    else {
        term1 = 0;
        term0 = bSig;
    }
    sub128( term0, term1, aSig0, aSig1, &alternateASig0, &alternateASig1 );
    if (    lt128( alternateASig0, alternateASig1, aSig0, aSig1 )
         || (    eq128( alternateASig0, alternateASig1, aSig0, aSig1 )
              && ( q & 1 ) )
       ) {
        aSig0 = alternateASig0;
        aSig1 = alternateASig1;
        zSign = ! zSign;
    }
    return NormalizeRoundPackTo_f80(FLOAT_ROUND_PRECISION_80, zSign, bExp + expDiff, aSig0, aSig1 );

}


f80 SoftFPU::sqrt( f80 a )
{
    sign_type   a_sign;
    fp_exponent_t aExp, zExp;
    f80::fraction_t aSig0, aSig1, zSig0, zSig1, doubleZSig0;
    u64      rem0, rem1, rem2, rem3, term0, term1, term2, term3;

    aSig0 = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    if ( aExp == a.EINF ) {
        if ( u64( aSig0<<1 ) ) return propagate_nan(a);
        if ( ! a_sign ) return a;
        goto invalid;
    }
    if ( a_sign ) {
        if ( ( aExp | aSig0 ) == 0 ) return a;
 invalid:
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( aExp == 0 ) {
        if ( aSig0 == 0 ) return a.zero( SIGN_POSITIVE );
        aExp = normalize_subnormal_f80( aSig0 );
    }
    zExp = ( ( aExp - a.BIAS )>>1 ) + a.BIAS;
    zSig0 = estimateSqrt32( aExp, aSig0 >> 32 );
    shift128Right( aSig0, 0, 2 + ( aExp & 1 ), &aSig0, &aSig1 );
    zSig0 = estimateDiv128To64( aSig0, aSig1, zSig0<<32 ) + ( zSig0<<30 );
    doubleZSig0 = zSig0<<1;
    mul64To128( zSig0, zSig0, &term0, &term1 );
    sub128( aSig0, aSig1, term0, term1, &rem0, &rem1 );
    while ( i64(rem0) < 0 ) {
        --zSig0;
        doubleZSig0 -= 2;
        add128( rem0, rem1, zSig0>>63, doubleZSig0 | 1, &rem0, &rem1 );
    }
    zSig1 = estimateDiv128To64( rem1, 0, doubleZSig0 );
    if ( lobits(zSig1, 62) <= 5 ) {
        if ( zSig1 == 0 ) zSig1 = 1;
        mul64To128( doubleZSig0, zSig1, &term1, &term2 );
        sub128( rem1, 0, term1, term2, &rem1, &rem2 );
        mul64To128( zSig1, zSig1, &term2, &term3 );
        sub192( rem1, rem2, 0, 0, term2, term3, &rem1, &rem2, &rem3 );
        while ( i64(rem1) < 0 ) {
            --zSig1;
            shortShift128Left( 0, zSig1, 1, &term2, &term3 );
            term3 |= 1;
            term2 |= doubleZSig0;
            add192( rem1, rem2, rem3, 0, term2, term3, &rem1, &rem2, &rem3 );
        }
        zSig1 |= ( ( rem1 | rem2 | rem3 ) != 0 );
    }
    shortShift128Left( 0, zSig1, 1, &zSig0, &zSig1 );
    zSig0 |= doubleZSig0;
    return RoundPackTo_f80(get_rounding_precision(), SIGN_POSITIVE, zExp, zSig0, zSig1 );

}


f80 SoftFPU::muladd(f80 a, f80 b, f80 c, muladd_negate_t flags)
{
    sign_type       a_sign, b_sign, cSign, zSign;
    fp_exponent_t   aExp, bExp, cExp, pExp, zExp, expDiff;
    f80::fraction_t aSig, bSig, cSig;
    bool            pInf, pZero, infzero;
    u64             pSig0, pSig1, pSig2, cSig0, cSig1, cSig2, zSig0, zSig1, zSig2;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();

    cSig = c.fraction();
    cExp = c.exponent();
    cSign = c.sign();

    infzero = ((aExp == 0 && aSig == 0 && bExp == b.EINF && bSig == b.UNHIDDEN_BIT) ||
               (bExp == 0 && bSig == 0 && aExp == a.EINF && aSig == a.UNHIDDEN_BIT));

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
    sign_type pSign = a_sign ^ b_sign;
    if (flags & FLOAT_MULADD_NEGATE_PRODUCT) {
        pSign = ! pSign;
    }
    pInf = (aExp == a.EINF) || (bExp == b.EINF);
    pZero = ((aExp | aSig) == 0) || ((bExp | bSig) == 0);

    if (cExp == c.EINF) {
        if (pInf && (pSign ^ cSign)) {
            // addition of opposite-signed infinities => InvalidOperation
            raise(FLOAT_FLAG_INVALID);
            return c.default_nan();
        }
        // Otherwise generate an infinity of the same sign
        return c.inf(cSign);
    }

    if (pInf) {
        return a.inf(pSign);
    }

    if (pZero) {
        if (cExp == 0) {
            if (cSig == 0) {
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
        return pack_f80(cSign, cExp, cSig);
    }

    if (aExp == 0) {
        aExp = normalize_subnormal_f80( aSig );
    }
    if (bExp == 0) {
        bExp = normalize_subnormal_f80( bSig );
    }

    /* Calculate the actual result a * b + c */

    /* Multiply first; this is easy. */
    /* NB: we subtract 0x3fe where mul() subtracts 0x3ff
     * because we want the true exponent, not the "one-less-than"
     * flavour that RoundPackTo_f64() takes.
     */
    pExp = aExp + bExp - (a.BIAS - 1); //0x3fe
    //aSig |= a.UNHIDDEN_BIT;
    //bSig |= b.UNHIDDEN_BIT;
    mul64To128(aSig, bSig, &pSig0, &pSig1);
    if ( i64(pSig0) > 0 ) {
        shortShift128Left(pSig0, pSig1, 1, &pSig0, &pSig1);
        pExp--;
    }

    zSign = pSign;

    // Now [pSig0:pSig1] is the significand of the multiply, with the explicit bit in position 127.
    if ( cExp == 0 ) {
        if ( !cSig ) {
            // Throw out the special case of c being an exact zero now
            return RoundPackTo_f80(get_rounding_precision(), zSign, pExp, pSig0, pSig1);
        }
        cExp = normalize_subnormal_f80( cSig );
    }

    /* Shift cSig and add the explicit bit so [cSig0:cSig1] is the
     * significand of the addend, with the explicit bit in position 127.
     */
    cSig0 = cSig;
    cSig1 = 0;
    cSig2 = 0;
    pSig2 = 0;
    expDiff = pExp - cExp;

    if (pSign == cSign) {
        // Addition
        if (expDiff > 0) {
            // scale c to match p
            shift192RightJamming(cSig0, cSig1, cSig2, expDiff, &cSig0, &cSig1, &cSig2);
            zExp = pExp;
        } else if (expDiff < 0) {
            // scale p to match c
            shift192RightJamming(pSig0, pSig1, pSig2, -expDiff, &pSig0, &pSig1, &pSig2);
            zExp = cExp;
        } else {
            // no scaling needed
            zExp = cExp;
        }

        // Add significands
        add192(pSig0, pSig1, pSig2, cSig0, cSig1, cSig2, &zSig0, &zSig1, &zSig2);
        // check overflow
        if (lt128(zSig0, zSig1, pSig0, pSig1)) {
           shift192RightJamming(zSig0, zSig1, zSig2, 1, &zSig0, &zSig1, &zSig2);
           zSig0 |= U64C(1) << 63;
           zExp++;
        }

        return RoundPackTo_f80( get_rounding_precision(), zSign, zExp, zSig0, zSig1 | (zSig2 != 0) );

    }
    else {
        // Subtraction
        if (expDiff > 0) {
            shift192RightJamming(cSig0, cSig1, cSig2, expDiff, &cSig0, &cSig1, &cSig2);
            //if (expDiff >= 128) {
            //   if (pSig1 == U64C(0x8000000000000001) && cSig1 == U64C(1)) {
            //      cSig1 = 0;
            //   }
            //}
            sub192(pSig0, pSig1, pSig2, cSig0, cSig1, cSig2, &zSig0, &zSig1, &zSig2);
            zExp = pExp;
        } else if (expDiff < 0) {
            shift192RightJamming(pSig0, pSig1, pSig2, -expDiff, &pSig0, &pSig1, &pSig2);
            sub192(cSig0, cSig1, cSig2, pSig0, pSig1, pSig2, &zSig0, &zSig1, &zSig2);
            zExp = cExp;
            zSign = ! zSign;
        } else {
            zExp = pExp;
            if (lt128(cSig0, cSig1, pSig0, pSig1)) {
                sub192(pSig0, pSig1, pSig2, cSig0, cSig1, cSig2, &zSig0, &zSig1, &zSig2);
            } else if (lt128(pSig0, pSig1, cSig0, cSig1)) {
                sub192(cSig0, cSig1, cSig2, pSig0, pSig1, pSig2, &zSig0, &zSig1, &zSig2);
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

        return NormalizeRoundPackTo_f80( get_rounding_precision(), zSign, zExp, zSig0, zSig1 | (zSig2 != 0) );
    }

#if 0
    //extern f80 mpfr_test_fmal(f80 a, f80 b, f80 c);
    f80 x = NormalizeRoundPackTo_f80( get_rounding_precision(), zSign, zExp, zSig0, zSig1 | (zSig2 != 0) );
    f80 t;
//    t = mpfr_test_fmal(a, b, c);
    t.hard() = std::fmal(a.hard(), b.hard(), c.hard());
    if (x != t) {
       LOG_DEBUG(LOG_FPU) << "mul: " << u192(pSig0,pSig1,pSig2) << ' ' << int(pSign) << ' ' << std::dec << pExp << "\n";
       LOG_DEBUG(LOG_FPU) << "c:   " << u192(cSig0,cSig1,cSig2) << ' ' << int(cSign) << ' ' << std::dec << cExp << "\n";
       LOG_DEBUG(LOG_FPU) << "z:   " << u192(zSig0,zSig1,zSig2) << ' ' << int(zSign) << ' ' << std::dec << zExp << ' ' << expDiff << "\n";
    }
    return x;
#endif
}

} //namespace fpu
} // namespace postrisc
