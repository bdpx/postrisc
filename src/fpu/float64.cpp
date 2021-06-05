#include "util/common.hpp"
#include "util.hpp"
#include "floatimpl.hpp"

namespace postrisc {
namespace fpu {

commonNaNT SoftFPU::ToCommonNaN(f64 a)
{
    commonNaNT z;

    if ( a.IsSignalingNan() )
       raise(FLOAT_FLAG_INVALID);
    z.sign = a.sign();
    z.low  = 0;
    z.high = a.val() << (64 - a.BITS_PER_SIGNIFICAND);
    return z;
}


f64 SoftFPU::to_f64(commonNaNT a)
{
    if ( default_nan_mode() ) {
        return f64::default_nan();
    }

    u64 mantissa = a.high>>12;
    if ( mantissa )
        return f64::make( ( u64(a.sign)<<63 ) | U64C( 0x7FF0000000000000 ) | U64C( 0x0008000000000000 ) | mantissa);
    else
        return f64::default_nan();
}


/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper double-precision floating-
| point value corresponding to the abstract input.  Ordinarily, the abstract
| value is simply rounded and packed into the double-precision format, with
| the inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded
| to a subnormal number, and the underflow and inexact exceptions are raised
| if the abstract input cannot be represented exactly as a subnormal double-
| precision floating-point number.
|     The input significand `zSig' has its binary point between bits 62
| and 61, which is 10 bits to the left of the usual location.  This shifted
| significand must be normalized or smaller.  If `zSig' is not normalized,
| `zExp' must be 0; in that case, the result returned is a subnormal number,
| and it must not require rounding.  In the usual case that `zSig' is
| normalized, `zExp' must be 1 less than the ``true'' floating-point exponent.
| The handling of underflow and overflow follows the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f64 SoftFPU::RoundPackTo_f64(sign_type zSign, fp_exponent_t zExp, f64::fraction_t zSig)
{
    const int NUM_ROUNDING_BITS = 10;
    const int HIGHEST_GUARD_BIT = 1 << (NUM_ROUNDING_BITS - 1); //0x200
    const fp_exponent_t NNNN = f64::EINF - 2; //0x7FD

    rounding_mode_t roundingMode = get_rounding_mode();
    bool roundNearestEven = ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN );
    int roundIncrement = HIGHEST_GUARD_BIT;

    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = makemask32(NUM_ROUNDING_BITS); //0x3FF;
            if ( zSign ) {
                if ( roundingMode == FLOAT_ROUND_UPWARD ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLOAT_ROUND_DOWNWARD ) roundIncrement = 0;
            }
        }
    }

    int roundBits = lobits(zSig, NUM_ROUNDING_BITS); // & 0x3FF;

    if ( u32(NNNN) <= u32(zExp) ) {
        if (    ( NNNN < zExp )
             || ( ( zExp == NNNN )
                  && ( i64( zSig + roundIncrement ) < 0 ) )
           ) {
            raise(FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
            return pack_f64( zSign, f64::EINF, - ( roundIncrement == 0 ));
        }
        if ( zExp < 0 ) {
            if (flush_to_zero()) {
                raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                return f64::zero(zSign);
            }
            bool isTiny = tininess_before_rounding()
                || ( zExp < -1 )
                || ( zSig + roundIncrement < U64C( 0x8000000000000000 ) );
            zSig = shift64RightJamming( zSig, - zExp);
            zExp = 0;
            roundBits = lobits(zSig, NUM_ROUNDING_BITS); // & 0x3FF;
            if ( isTiny && roundBits ) raise(FLOAT_FLAG_UNDERFLOW);
        }
    }
    if ( roundBits ) raise(FLOAT_FLAG_INEXACT);
    zSig = ( zSig + roundIncrement ) >> NUM_ROUNDING_BITS;
    zSig &= ~ f64::fraction_t( ( ( roundBits ^ HIGHEST_GUARD_BIT ) == 0 ) & roundNearestEven );
    if ( zSig == 0 ) zExp = 0;
    return pack_f64( zSign, zExp, zSig );

}

/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper double-precision floating-
| point value corresponding to the abstract input.  This routine is just like
| `RoundPackTo_f64' except that `zSig' does not have to be normalized.
| Bit 63 of `zSig' must be zero, and `zExp' must be 1 less than the ``true''
| floating-point exponent.
*----------------------------------------------------------------------------*/

f64 SoftFPU::NormalizeRoundPackTo_f64(sign_type zSign, fp_exponent_t zExp, f64::fraction_t zSig)
{
    assert( zSig );
    int shiftCount = countLeadingZeros64( zSig ) - 1;
    return RoundPackTo_f64( zSign, zExp - shiftCount, zSig<<shiftCount);
}

/*----------------------------------------------------------------------------
| Returns the result of converting the 32-bit two's complement integer `a'
| to the double-precision floating-point format.  The conversion is performed
| according to the IEC/IEEE Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f64 SoftFPU::to_f64( i32 a)
{
    if ( a == 0 ) return f64_zero;
    sign_type zSign = get_sign(a);
    f64::fraction_t absA = zSign ? - i64(a) : i64(a);
    int shiftCount = countLeadingZeros32( absA ) + 21;
    return pack_f64( zSign, f64::BIAS + 51 - shiftCount, absA<<shiftCount );
}

/*----------------------------------------------------------------------------
| Returns the result of converting the 64-bit two's complement integer `a'
| to the double-precision floating-point format.  The conversion is performed
| according to the IEC/IEEE Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f64 SoftFPU::to_f64( i64 a)
{
    if ( a == 0 ) return f64_zero;
    if ( a == std::numeric_limits<i64>::min() ) {
        return f64::make(SIGN_NEGATIVE, 63);
    }
    sign_type zSign = get_sign(a);
    return NormalizeRoundPackTo_f64( zSign, f64::BIAS + 61, zSign ? - a : a);
}


f64 SoftFPU::to_f64(u64 a)
{
    fp_exponent_t exp =  f64::BIAS + 61; //0x43C;

    if (a == 0) {
        return f64_zero;
    }
    if (i64(a) < 0) {
        a = shift64RightJamming(a, 1);
        exp += 1;
    }
    return NormalizeRoundPackTo_f64(SIGN_POSITIVE, exp, a);
}

/*----------------------------------------------------------------------------
| Returns the result of converting the double-precision floating-point value
| `a' to the 64-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/
i64 SoftFPU::to_i64( f64 a)
{
    sign_type   a_sign;
    fp_exponent_t aExp, shiftCount;
    f64::fraction_t  aSig, aSigExtra;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    if ( aExp ) aSig |= a.HIDDEN_BIT;
    shiftCount = (a.BIAS + a.BITS_PER_SIGNIFICAND) - aExp; //0x433
    if ( shiftCount <= 0 ) {
        if ( (a.BIAS + 63) < aExp ) { //0x43E
            raise(FLOAT_FLAG_INVALID);
            if ( ! a_sign || a.IsNan() ) {
                return std::numeric_limits<i64>::max();
            }
            return std::numeric_limits<i64>::min();
        }
        aSigExtra = 0;
        aSig <<= - shiftCount;
    }
    else {
        shift64ExtraRightJamming( aSig, 0, shiftCount, &aSig, &aSigExtra );
    }
    return round_pack_to_i64(INT_NAN_F64, a_sign, aSig, aSigExtra);

}

/*----------------------------------------------------------------------------
| Returns the result of converting the double-precision floating-point value
| `a' to the 64-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.
| If `a' is a NaN, the largest positive integer is returned.  Otherwise, if
| the conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/
i64 SoftFPU::to_i64_round_to_zero( f64 a)
{
    sign_type  a_sign;
    fp_exponent_t aExp, shiftCount;
    f64::fraction_t  aSig;
    i64      z;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    if ( aExp ) aSig |= a.HIDDEN_BIT;
    shiftCount = aExp - (a.BIAS + a.BITS_PER_SIGNIFICAND); //0x433
    if ( 0 <= shiftCount ) {
        if ( (a.BIAS + 63) <= aExp ) { //0x43E
            if ( a != f64::make(SIGN_NEGATIVE, 63) ) { //U64C( 0xC3E0000000000000 )
                raise(FLOAT_FLAG_INVALID);
                if ( ! a_sign || a.IsNan() ) {
                    return std::numeric_limits<i64>::max();
                }
            }
            return std::numeric_limits<i64>::min();
        }
        z = aSig<<shiftCount;
    }
    else {
        if ( aExp < a.BIAS - 1 ) { //0x3FE
            if ( aExp | aSig ) raise(FLOAT_FLAG_INEXACT);
            return 0;
        }
        z = aSig>>( - shiftCount );
        if ( u64( aSig<<( shiftCount & 63 ) ) ) {
            raise(FLOAT_FLAG_INEXACT);
        }
    }
    if ( a_sign ) z = - z;
    return z;

}

/*----------------------------------------------------------------------------
| Rounds the double-precision floating-point value `a' to an integer, and
| returns the result as a double-precision floating-point value.  The
| operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f64 SoftFPU::round_to_int( f64 a)
{
    fp_exponent_t aExp = a.exponent();
    f64::fraction_t aSig = a.fraction();

    if ( (a.BIAS + a.BITS_PER_SIGNIFICAND) <= aExp ) { //0x433
        if ( ( aExp == a.EINF ) && aSig ) {
            return propagate_nan(a);
        }
        return a;
    }
    if ( aExp < a.BIAS ) { //0x3FF
        if ( u64( a.val()<<1 ) == 0 ) return a;
            raise(FLOAT_FLAG_INEXACT);
        sign_type a_sign = a.sign();
        switch ( get_rounding_mode() ) {
         case FLOAT_ROUND_TONEAREST_EVEN:
             if ( ( aExp == a.BIAS - 1 ) && aSig ) { //0x3FE
                 return pack_f64( a_sign, f64::BIAS, 0 );
             }
             break;
         case FLOAT_ROUND_DOWNWARD:
             return f64::make(a_sign ? U64C( 0xBFF0000000000000 ) : 0);
         case FLOAT_ROUND_UPWARD:
             return f64::make(a_sign ? U64C( 0x8000000000000000 ) : U64C( 0x3FF0000000000000 ));
         default:
         case FLOAT_ROUND_TOWARDZERO:
             break;
        }
        return a.zero(a_sign);
    }

    u64 lastBitMask = 1;
    lastBitMask <<= (a.BIAS + a.BITS_PER_SIGNIFICAND) - aExp; //0x433
    u64 roundBitsMask = lastBitMask - 1;
    f64::fraction_t z = a.val();
    rounding_mode_t roundingMode = get_rounding_mode();
    if ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN ) {
        z += lastBitMask>>1;
        if ( ( z & roundBitsMask ) == 0 ) z &= ~ lastBitMask;
    }
    else if ( roundingMode != FLOAT_ROUND_TOWARDZERO ) {
        if ( f64::make(z).sign() ^ ( roundingMode == FLOAT_ROUND_UPWARD ) ) {
            z += roundBitsMask;
        }
    }
    z &= ~ roundBitsMask;
    if ( z != a.val() )
        raise(FLOAT_FLAG_INEXACT);
    return f64::make(z);

}


f64 SoftFPU::round_to_int(f64 a, rounding_mode_t rmode)
{
    rounding_mode_t old = get_rounding_mode();
    set_rounding_mode(rmode);
    f64 t = round_to_int(a);
    set_rounding_mode(old);
    return t;
}


f64 SoftFPU::trunc_to_int( f64 a)
{
    rounding_mode_t oldmode = get_rounding_mode();
    set_rounding_mode(FLOAT_ROUND_TOWARDZERO);
    f64 res = round_to_int(a);
    set_rounding_mode(oldmode);
    return res;
}


/*----------------------------------------------------------------------------
| Returns the binary log of the double-precision floating-point value `a'.
| The operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
f64 SoftFPU::log2( f64 a)
{
    sign_type  a_sign, zSign;
    fp_exponent_t aExp;
    f64::fraction_t aSig, aSig0, aSig1, zSig, i;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    if ( aExp == 0 ) {
        if ( aSig == 0 ) {
           raise(FLOAT_FLAG_DIVBYZERO);
           return a.inf(SIGN_NEGATIVE);
        }
        aExp = normalize_subnormal_f64( aSig );
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

    aExp -= a.BIAS; //0x3FF;
    aSig |= a.HIDDEN_BIT;
    zSign = get_sign(aExp);
    zSig = u64(aExp) << a.BITS_PER_SIGNIFICAND;
    for (i = u64(1) << 51; i > 0; i >>= 1) {
        mul64To128( aSig, aSig, &aSig0, &aSig1 );
        aSig = ( aSig0 << 12 ) | ( aSig1 >> 52 );
        if ( aSig & (u64(1) << 53) ) {
            aSig >>= 1;
            zSig |= i;
        }
    }

    if ( zSign )
        zSig = 0 - zSig; // unary minus unsigned error

    return NormalizeRoundPackTo_f64( zSign, a.BIAS + 9, zSig); //0x408
}


bool SoftFPU::eq_signaling( f64 a, f64 b)
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }

    return ( a.val() == b.val() ) || IsBothZero(a,b);
}


bool SoftFPU::le_signaling( f64 a, f64 b)
{
    sign_type  a_sign, b_sign;
    f64::fraction_t  av, bv;

    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    a_sign = a.sign();
    b_sign = b.sign();
    av = a.val();
    bv = b.val();
    if ( a_sign != b_sign )
       return a_sign || IsBothZero(a,b);
    return ( av == bv ) || ( a_sign ^ ( av < bv ) );

}


bool SoftFPU::lt_signaling( f64 a, f64 b)
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }

    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    f64::fraction_t av = a.val();
    f64::fraction_t bv = b.val();

    if ( a_sign != b_sign )
       return a_sign && !IsBothZero(a,b);
    return ( av != bv ) && ( a_sign ^ ( av < bv ) );

}


bool SoftFPU::unordered( f64 a, f64 b)
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return true;
    }
    return false;
}


bool SoftFPU::eq_quiet( f64 a, f64 b)
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }

    return ( a.val() == b.val() ) || IsBothZero(a,b);
}


bool SoftFPU::le_quiet( f64 a, f64 b)
{
    sign_type   a_sign, b_sign;
    f64::fraction_t  av, bv;

    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    a_sign = a.sign();
    b_sign = b.sign();
    av = a.val();
    bv = b.val();
    if ( a_sign != b_sign ) return a_sign || IsBothZero(a,b);
    return ( av == bv ) || ( a_sign ^ ( av < bv ) );

}


bool SoftFPU::lt_quiet( f64 a, f64 b)
{
    sign_type  a_sign, b_sign;
    f64::fraction_t av, bv;

    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    a_sign = a.sign();
    b_sign = b.sign();
    av = a.val();
    bv = b.val();
    if ( a_sign != b_sign ) return a_sign && !IsBothZero(a,b);
    return ( av != bv ) && ( a_sign ^ ( av < bv ) );

}


bool SoftFPU::unordered_quiet( f64 a, f64 b)
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return true;
    }
    return false;
}

/*----------------------------------------------------------------------------
| Returns the result of converting the double-precision floating-point value
| `a' to the 32-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32( f64 a)
{
    f64::fraction_t aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( a.IsNan() ) a_sign = SIGN_POSITIVE;

    if ( aExp ) aSig |= a.HIDDEN_BIT;
    fp_exponent_t shiftCount = (a.BIAS + (a.BITS_PER_SIGNIFICAND - 7)) - aExp; //0x42C
    if ( 0 < shiftCount ) aSig = shift64RightJamming( aSig, shiftCount);
    return round_pack_to_i32(INT_NAN_F64, a_sign, aSig);

}

/*----------------------------------------------------------------------------
| Returns the result of converting the double-precision floating-point value
| `a' to the 32-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.
| If `a' is a NaN, the largest positive integer is returned.  Otherwise, if
| the conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32_round_to_zero( f64 a)
{
    i32    z, shiftCount;
    f64::fraction_t  savedASig;

    f64::fraction_t aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type a_sign = a.sign();

    if ( (a.BIAS + 31) < aExp ) { //0x41E
        if ( ( aExp == a.EINF ) && aSig ) a_sign = SIGN_POSITIVE;
        goto invalid;
//        raise(FLOAT_FLAG_INVALID);
//        return a_sign ? std::numeric_limits<i32>::min() : std::numeric_limits<i32>::max();
//        return std::numeric_limits<i32>::min();
    }
    else if ( aExp < a.BIAS ) { //0x3FF
        if ( aExp || aSig ) raise(FLOAT_FLAG_INEXACT);
        return 0;
    }
    aSig |= a.HIDDEN_BIT;
    shiftCount = (a.BIAS + a.BITS_PER_SIGNIFICAND) - aExp; //0x433
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
| Returns the result of converting the double-precision floating-point value
| `a' to the 16-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.
| If `a' is a NaN, the largest positive integer is returned.  Otherwise, if
| the conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

int16_t SoftFPU::to_i16_round_to_zero(f64 a)
{
    fp_exponent_t aExp, shiftCount;
    f64::fraction_t aSig, savedASig;

    aSig = a.fraction();
    aExp = a.exponent();
    sign_type a_sign = a.sign();
    if ( (a.BIAS + 15) < aExp ) {  //0x40E
        if ( ( aExp == a.EINF ) && aSig ) {
            a_sign = SIGN_POSITIVE;
        }
//        goto invalid;
        raise(FLOAT_FLAG_INVALID);
        return a_sign ? std::numeric_limits<int16_t>::min() : std::numeric_limits<int16_t>::max();
    }
    else if ( aExp < a.BIAS) {  //0x3FF
        if ( aExp || aSig ) {
            raise(FLOAT_FLAG_INEXACT);
        }
        return 0;
    }
    aSig |= a.HIDDEN_BIT;
    shiftCount = a.BIAS + a.BITS_PER_SIGNIFICAND - aExp; //0x433
    savedASig = aSig;
    aSig >>= shiftCount;
    i32 z = static_cast<i32>(aSig);
    if ( a_sign ) {
        z = - z;
    }
    if ( ( int16_t(z) < 0 ) ^ a_sign ) {
// invalid:
        raise(FLOAT_FLAG_INVALID);
        return a_sign ? std::numeric_limits<int16_t>::min() : std::numeric_limits<int16_t>::max();
    }
    if ( ( aSig<<shiftCount ) != savedASig ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    return z;
}


f32 SoftFPU::to_f32( f64 a)
{
    f64::fraction_t  aSig  = a.fraction();
    fp_exponent_t  aExp  = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f32( ToCommonNaN(a));
        return f32::inf(a_sign);
    }
    aSig = shift64RightJamming( aSig, (a.BITS_PER_SIGNIFICAND - f32::BITS_PER_SIGNIFICAND - 7) );
    u32 zSig = static_cast<u32>(aSig);
    if ( aExp || zSig ) {
        zSig |= ( u32(1) << (f32::BITS_PER_SIGNIFICAND + 7) );
        aExp -= a.BIAS - f32::BIAS + 1; //0x381;
    }
    return RoundPackTo_f32( a_sign, aExp, zSig);

}


f64 SoftFPU::to_f64( f32 a)
{
    u32 aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return to_f64( ToCommonNaN(a));
        return f64::inf(a_sign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f64::zero(a_sign);
        aExp = normalize_subnormal_f32( aSig );
        --aExp;
    }
    return pack_f64( a_sign,
                         aExp + f64::BIAS - a.BIAS,
                         u64(aSig) << (f64::BITS_PER_SIGNIFICAND - a.BITS_PER_SIGNIFICAND) ); //0x380

}

f64 SoftFPU::to_f64(u32 a)
{
    return to_f64(i64(a));
}

u32 SoftFPU::to_u32( f64 a)
{
    u32 res;

    i64 v = to_i64(a);
    if (v < 0) {
        res = 0;
        raise(FLOAT_FLAG_INVALID);
    } else if (v > std::numeric_limits<u32>::max()) {
        res = std::numeric_limits<u32>::max();
        raise(FLOAT_FLAG_INVALID);
    } else {
        res = static_cast<u32>(v);
    }
    return res;
}

u32 SoftFPU::to_u32_round_to_zero( f64 a)
{
    u32 res;

    i64 v = to_i64_round_to_zero(a);
    if (v < 0) {
        res = 0;
        raise(FLOAT_FLAG_INVALID);
    } else if (v > std::numeric_limits<u32>::max()) {
        res = std::numeric_limits<u32>::max();
        raise(FLOAT_FLAG_INVALID);
    } else {
        res = v;
    }
    return res;
}

u16 SoftFPU::to_u16_round_to_zero(f64 a)
{
    u16 res;

    i64 v = to_i64_round_to_zero(a);
    if (v < 0) {
        res = 0;
        raise(FLOAT_FLAG_INVALID);
    } else if (v > std::numeric_limits<u16>::max()) {
        res = std::numeric_limits<u16>::max();
        raise(FLOAT_FLAG_INVALID);
    } else {
        res = v;
    }
    return res;
}

/* FIXME: This looks broken.  */
u64 SoftFPU::to_u64 (f64 a)
{
    i64 v;

    v = to_f64( std::numeric_limits<i64>::min() ).val();
    v += a.val();
    v = to_i64( f64::make( u64(v) ) );

    return v - std::numeric_limits<i64>::min();
}

u64 SoftFPU::to_u64_round_to_zero (f64 a)
{
    i64 v;

    v = to_f64(std::numeric_limits<i64>::min()).val();
    v += a.val();
    v = to_i64_round_to_zero( f64::make( u64(v) ) );

    return v - std::numeric_limits<i64>::min();
}

/* Multiply A by 2 raised to the power N.  */
f64 SoftFPU::scalbn( f64 a, int n)
{
    f64::fraction_t aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) {
            return propagate_nan(a);
        }
        return a;
    }
    if ( aExp != 0 )
        aSig |= a.HIDDEN_BIT;
    else if ( aSig == 0 )
        return a;

    if (n > 0x1000) {
        n = 0x1000;
    } else if (n < -0x1000) {
        n = -0x1000;
    }

    aExp += n - 1;
    aSig <<= 10;
    return NormalizeRoundPackTo_f64( a_sign, aExp, aSig );
}


f64 SoftFPU::merge(f64 sign, f64 exponent, f64 mantissa)
{
    return f64::make(
           (    sign.val() & (makemask64(1) << 63) ) |
           (mantissa.val() & (makemask64(f64::BITS_PER_SIGNIFICAND) ) ) |
           (exponent.val() & (makemask64(f64::BITS_PER_EXPONENT) << f64::BITS_PER_SIGNIFICAND) ) );
}


float_relation SoftFPU::compare(f64 a, f64 b) {  return compare_internal(a, b, false);  }
float_relation SoftFPU::compare_quiet(f64 a, f64 b) {  return compare_internal(a, b, true);  }

compare_mode_t SoftFPU::compare(f64 a, f64 b, compare_mode_t mode)
{
    return compare_internal(a, b, mode);
}

f64 SoftFPU::min(f64 a, f64 b, bool ignore_nans) {  return minmax<f64>(a, b, ignore_nans, true);  }
f64 SoftFPU::max(f64 a, f64 b, bool ignore_nans) {  return minmax<f64>(a, b, ignore_nans, false); }



/*----------------------------------------------------------------------------
| Returns the result of adding the absolute values of the double-precision
| floating-point values `a' and `b'.  If `zSign' is 1, the sum is negated
| before being returned.  `zSign' is ignored if the result is a NaN.
| The addition is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
f64 SoftFPU::addSigs( f64 a, f64 b, sign_type zSign)
{
    fp_exponent_t aExp, bExp, zExp;
    f64::fraction_t aSig, bSig, zSig;
    fp_exponent_t expDiff;

    const int FIXED_POINT = 9;
    const u64 FIXED_HIDDEN_BIT = u64(1) << (52 + FIXED_POINT);

    aSig = a.fraction();
    aExp = a.exponent();
    bSig = b.fraction();
    bExp = b.exponent();
    expDiff = aExp - bExp;
    aSig <<= FIXED_POINT;
    bSig <<= FIXED_POINT;
    if ( 0 < expDiff ) {
        if ( aExp == a.EINF ) {
            if ( aSig ) return propagate_nan( a, b);
            return a;
        }
        if ( bExp == 0 ) {
            --expDiff;
        }
        else {
            bSig |= FIXED_HIDDEN_BIT;
        }
        bSig = shift64RightJamming( bSig, expDiff);
        zExp = aExp;
    }
    else if ( expDiff < 0 ) {
        if ( bExp == b.EINF ) {
            if ( bSig ) return propagate_nan( a, b);
            return b.inf(zSign);
        }
        if ( aExp == 0 ) {
            ++expDiff;
        }
        else {
            aSig |= FIXED_HIDDEN_BIT;
        }
        aSig = shift64RightJamming( aSig, - expDiff);
        zExp = bExp;
    }
    else {
        if ( aExp == a.EINF ) {
            if ( aSig | bSig ) return propagate_nan( a, b);
            return a;
        }
        if ( aExp == 0 ) {
            if (flush_to_zero()) {
                if (aSig | bSig) {
                    raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                }
                return a.zero(zSign);
            }
            return pack_f64( zSign, 0, ( aSig + bSig ) >> FIXED_POINT );
        }
        zSig = 2*FIXED_HIDDEN_BIT + aSig + bSig;
        zExp = aExp;
        goto roundAndPack;
    }
    aSig |= FIXED_HIDDEN_BIT;
    zSig = ( aSig + bSig )<<1;
    --zExp;
    if ( i64(zSig) < 0 ) {
        zSig = aSig + bSig;
        ++zExp;
    }
 roundAndPack:
    return RoundPackTo_f64( zSign, zExp, zSig);

}

/*----------------------------------------------------------------------------
| Returns the result of subtracting the absolute values of the double-
| precision floating-point values `a' and `b'.  If `zSign' is 1, the
| difference is negated before being returned.  `zSign' is ignored if the
| result is a NaN.  The subtraction is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
f64 SoftFPU::subSigs( f64 a, f64 b, sign_type zSign)
{
    fp_exponent_t aExp, bExp, zExp;
    f64::fraction_t aSig, bSig, zSig;
    fp_exponent_t expDiff;

    const int FIXED_POINT = 10;
    const u64 FIXED_HIDDEN_BIT = u64(1) << (52 + FIXED_POINT);

    aSig = a.fraction();
    aExp = a.exponent();
    bSig = b.fraction();
    bExp = b.exponent();
    expDiff = aExp - bExp;
    aSig <<= FIXED_POINT;
    bSig <<= FIXED_POINT;
    if ( 0 < expDiff ) goto aExpBigger;
    if ( expDiff < 0 ) goto bExpBigger;
    if ( aExp == a.EINF ) {
        if ( aSig | bSig ) return propagate_nan( a, b);
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( aExp == 0 ) {
        aExp = 1;
        bExp = 1;
    }
    if ( bSig < aSig ) goto aBigger;
    if ( aSig < bSig ) goto bBigger;
    return a.zero( sign_type(get_rounding_mode() == FLOAT_ROUND_DOWNWARD) );
 bExpBigger:
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan( a, b);
        return b.inf(zSign ^ SIGN_NEGATIVE);
    }
    if ( aExp == 0 ) {
        ++expDiff;
    }
    else {
        aSig |= FIXED_HIDDEN_BIT;
    }
    aSig = shift64RightJamming( aSig, - expDiff);
    bSig |= FIXED_HIDDEN_BIT;
 bBigger:
    zSig = bSig - aSig;
    zExp = bExp;
    zSign = ! zSign;
    goto normalizeRoundAndPack;
 aExpBigger:
    if ( aExp == f64::EINF ) {
        if ( aSig ) return propagate_nan( a, b);
        return a;
    }
    if ( bExp == 0 ) {
        --expDiff;
    }
    else {
        bSig |= FIXED_HIDDEN_BIT;
    }
    bSig = shift64RightJamming( bSig, expDiff);
    aSig |= FIXED_HIDDEN_BIT;
 aBigger:
    zSig = aSig - bSig;
    zExp = aExp;
 normalizeRoundAndPack:
    --zExp;
    return NormalizeRoundPackTo_f64( zSign, zExp, zSig);

}


f64 SoftFPU::add( f64 a, f64 b)
{
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();

    if ( a_sign == b_sign ) {
        return addSigs( a, b, a_sign);
    } else {
        return subSigs( a, b, a_sign);
    }

}


f64 SoftFPU::sub( f64 a, f64 b)
{
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();

    if ( a_sign == b_sign ) {
        return subSigs( a, b, a_sign);
    } else {
        return addSigs( a, b, a_sign);
    }

}


f64 SoftFPU::mul( f64 a, f64 b)
{
    sign_type  a_sign, b_sign, zSign;
    fp_exponent_t aExp, bExp, zExp;
    f64::fraction_t  aSig, bSig, zSig0, zSig1;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();

    zSign = a_sign ^ b_sign;

    if ( aExp == a.EINF ) {
        if ( aSig || ( ( bExp == b.EINF ) && bSig ) ) {
            return propagate_nan( a, b);
        }
        if ( ( bExp | bSig ) == 0 ) {
            raise(FLOAT_FLAG_INVALID);
            return b.default_nan();
        }
        return a.inf(zSign);
    }

    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan( a, b);
        if ( ( aExp | aSig ) == 0 ) {
            raise(FLOAT_FLAG_INVALID);
            return a.default_nan();
        }
        return b.inf(zSign);
    }

    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a.zero(zSign);
        aExp = normalize_subnormal_f64( aSig );
    }

    if ( bExp == 0 ) {
        if ( bSig == 0 ) return b.zero(zSign);
        bExp = normalize_subnormal_f64( bSig );
    }

    zExp = aExp + bExp - b.BIAS; //0x3FF;
    aSig = ( aSig | a.HIDDEN_BIT ) << (f64::BITS_PER_EXPONENT - 1);
    bSig = ( bSig | b.HIDDEN_BIT ) << (f64::BITS_PER_EXPONENT);
    mul64To128( aSig, bSig, &zSig0, &zSig1 );
    zSig0 |= ( zSig1 != 0 );

    if ( 0 <= i64( zSig0<<1 ) ) {
        zSig0 <<= 1;
        --zExp;
    }
    return RoundPackTo_f64( zSign, zExp, zSig0);

}


f64 SoftFPU::div( f64 a, f64 b)
{
    sign_type  a_sign, b_sign, zSign;
    fp_exponent_t aExp, bExp, zExp;
    f64::fraction_t aSig, bSig, zSig;
    u64     rem0, rem1;
    u64     term0, term1;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();

    zSign = a_sign ^ b_sign;

    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a, b);
        if ( bExp == b.EINF ) {
            if ( bSig ) return propagate_nan(a, b);
            raise(FLOAT_FLAG_INVALID);
            return b.default_nan();
        }
        return a.inf(zSign);
    }
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(a, b);
        return b.zero(zSign);
    }
    if ( bExp == 0 ) {
        if ( bSig == 0 ) {
            if ( ( aExp | aSig ) == 0 ) {
                raise(FLOAT_FLAG_INVALID);
                return a.default_nan();
            }
            raise(FLOAT_FLAG_DIVBYZERO);
            return b.inf(zSign);
        }
        bExp = normalize_subnormal_f64( bSig );
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a.zero(zSign);
        aExp = normalize_subnormal_f64( aSig );
    }
    zExp = aExp - bExp + 0x3FD;
    aSig = ( aSig | a.HIDDEN_BIT ) << (f64::BITS_PER_EXPONENT - 1);
    bSig = ( bSig | b.HIDDEN_BIT ) << (f64::BITS_PER_EXPONENT);
    if ( bSig <= ( aSig + aSig ) ) {
        aSig >>= 1;
        ++zExp;
    }
    zSig = estimateDiv128To64( aSig, 0, bSig );
    if ( lobits(zSig, 9) <= 2 ) {   // & 0x1FF
        mul64To128( bSig, zSig, &term0, &term1 );
        sub128( aSig, 0, term0, term1, &rem0, &rem1 );
        while ( i64(rem0) < 0 ) {
            --zSig;
            add128( rem0, rem1, 0, bSig, &rem0, &rem1 );
        }
        zSig |= ( rem1 != 0 );
    }
    return RoundPackTo_f64( zSign, zExp, zSig );

}


f64 SoftFPU::rem( f64 a, f64 b )
{
    sign_type  a_sign, zSign;
    fp_exponent_t aExp, bExp, expDiff;
    f64::fraction_t aSig, bSig;
    u64     q, alternateASig;
    i64      sigMean;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    bSig = b.fraction();
    bExp = b.exponent();

    if ( aExp == a.EINF ) {
        if ( aSig || ( ( bExp == b.EINF ) && bSig ) ) {
            return propagate_nan(a, b);
        }
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(a, b);
        return a;
    }
    if ( bExp == 0 ) {
        if ( bSig == 0 ) {
            raise(FLOAT_FLAG_INVALID);
            return b.default_nan();
        }
        bExp = normalize_subnormal_f64( bSig );
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a;
        aExp = normalize_subnormal_f64( aSig );
    }
    expDiff = aExp - bExp;
    aSig = ( aSig | a.HIDDEN_BIT ) << f64::BITS_PER_EXPONENT;
    bSig = ( bSig | b.HIDDEN_BIT ) << f64::BITS_PER_EXPONENT;
    if ( expDiff < 0 ) {
        if ( expDiff < -1 ) return a;
        aSig >>= 1;
    }
    q = ( bSig <= aSig );
    if ( q ) aSig -= bSig;
    expDiff -= 64;
    while ( 0 < expDiff ) {
        q = estimateDiv128To64( aSig, 0, bSig );
        q = ( 2 < q ) ? q - 2 : 0;
        aSig = 0 - ( ( bSig>>2 ) * q );
        expDiff -= 62;
    }
    expDiff += 64;
    if ( 0 < expDiff ) {
        q = estimateDiv128To64( aSig, 0, bSig );
        q = ( 2 < q ) ? q - 2 : 0;
        q >>= 64 - expDiff;
        bSig >>= 2;
        aSig = ( ( aSig>>1 )<<( expDiff - 1 ) ) - bSig * q;
    }
    else {
        aSig >>= 2;
        bSig >>= 2;
    }
    do {
        alternateASig = aSig;
        ++q;
        aSig -= bSig;
    } while ( 0 <= i64(aSig) );
    sigMean = aSig + alternateASig;
    if ( ( sigMean < 0 ) || ( ( sigMean == 0 ) && ( q & 1 ) ) ) {
        aSig = alternateASig;
    }
    zSign = get_sign( i64(aSig) );
    if ( zSign ) aSig = 0 - aSig;
    return NormalizeRoundPackTo_f64( a_sign ^ zSign, bExp, aSig );

}



f64 SoftFPU::sqrt( f64 a)
{
    sign_type  a_sign;
    fp_exponent_t aExp, zExp;
    f64::fraction_t aSig, zSig, doubleZSig;
    u64     term0, term1;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if ( aSig ) return propagate_nan(a);
        if ( ! a_sign ) return a;
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( a_sign ) {
        if ( ( aExp | aSig ) == 0 ) return a;
        raise(FLOAT_FLAG_INVALID);
        return a.default_nan();
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return f64_zero;
        aExp = normalize_subnormal_f64( aSig );
    }

    zExp = ( ( aExp - a.BIAS )>>1 ) + (a.BIAS - 1);

    aSig |= a.HIDDEN_BIT;
    zSig = estimateSqrt32( aExp, aSig >> (53 - 32) );
    aSig <<= ( 9 - ( aExp & 1 ) );

    zSig = estimateDiv128To64( aSig, 0, zSig<<32 ) + ( zSig<<30 );

    if ( lobits( zSig, 9 ) <= 5 ) {
        doubleZSig = zSig<<1;
        mul64To128( zSig, zSig, &term0, &term1 );
        u128 rem = u128(aSig, 0) - u128(term0, term1);
        while ( i64(rem.m0) < 0 ) {
            --zSig;
            doubleZSig -= 2;
            rem += u128( zSig>>63, doubleZSig | 1 );
        }
        zSig |= (bool)rem;
    }
    return RoundPackTo_f64( SIGN_POSITIVE, zExp, zSig );
}


/*----------------------------------------------------------------------------
| Returns the result of multiplying the double-precision floating-point values
| `a' and `b' then adding 'c', with no intermediate rounding step after the
| multiplication.  The operation is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic 754-2008.
| The flags argument allows the caller to select negation of the
| addend, the intermediate product, or the final result. (The difference
| between this and having the caller do a separate negation is that negating
| externally will flip the sign bit on NaNs.)
*----------------------------------------------------------------------------*/

f64 SoftFPU::muladd(f64 a, f64 b, f64 c, muladd_negate_t flags)
{
    sign_type    a_sign, b_sign, cSign, zSign;
    fp_exponent_t aExp, bExp, cExp, pExp, zExp, expDiff;
    f64::fraction_t aSig, bSig, cSig;
    bool         pInf, pZero, infzero;
    f64::fraction_t pSig0, pSig1, cSig0, cSig1, zSig0, zSig1;
    int          shiftcount;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();

    cSig = c.fraction();
    cExp = c.exponent();
    cSign = c.sign();

    infzero = ((aExp == 0 && aSig == 0 && bExp == b.EINF && bSig == 0) ||
               (bExp == 0 && bSig == 0 && aExp == a.EINF && aSig == 0));

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
        return pack_f64(cSign, cExp, cSig);
    }

    if (aExp == 0) {
        aExp = normalize_subnormal_f64( aSig );
    }
    if (bExp == 0) {
        bExp = normalize_subnormal_f64( bSig );
    }

    /* Calculate the actual result a * b + c */

    /* Multiply first; this is easy. */
    /* NB: we subtract 0x3fe where mul() subtracts 0x3ff
     * because we want the true exponent, not the "one-less-than"
     * flavour that RoundPackTo_f64() takes.
     */
    pExp = aExp + bExp - (a.BIAS - 1); //0x3fe
    aSig = ( aSig | a.HIDDEN_BIT ) << (f64::BITS_PER_EXPONENT - 1);
    bSig = ( bSig | b.HIDDEN_BIT ) << (f64::BITS_PER_EXPONENT);
    mul64To128(aSig, bSig, &pSig0, &pSig1);
    if (i64(pSig0 << 1) >= 0) {
        shortShift128Left(pSig0, pSig1, 1, &pSig0, &pSig1);
        pExp--;
    }

    zSign = pSign;

    /* Now [pSig0:pSig1] is the significand of the multiply, with the explicit
     * bit in position 126.
     */
    if (cExp == 0) {
        if (!cSig) {
            // Throw out the special case of c being an exact zero now
            shift128RightJamming(pSig0, pSig1, 64, &pSig0, &pSig1);
            return RoundPackTo_f64(zSign, pExp - 1, pSig1);
        }
        cExp = normalize_subnormal_f64( cSig );
    }

    /* Shift cSig and add the explicit bit so [cSig0:cSig1] is the
     * significand of the addend, with the explicit bit in position 126. */
    cSig0 = cSig << (126 - 64 - a.BITS_PER_SIGNIFICAND);
    cSig1 = 0;
    cSig0 |= U64C( 0x4000000000000000 );
    expDiff = pExp - cExp;

    if (pSign == cSign) {
        // Addition
        if (expDiff > 0) {
            // scale c to match p
            shift128RightJamming(cSig0, cSig1, expDiff, &cSig0, &cSig1);
            zExp = pExp;
        } else if (expDiff < 0) {
            // scale p to match c
            shift128RightJamming(pSig0, pSig1, -expDiff, &pSig0, &pSig1);
            zExp = cExp;
        } else {
            // no scaling needed
            zExp = cExp;
        }
        // Add significands and make sure explicit bit ends up in posn 126
        add128(pSig0, pSig1, cSig0, cSig1, &zSig0, &zSig1);
        if (i64(zSig0) < 0) {
            shift128RightJamming(zSig0, zSig1, 1, &zSig0, &zSig1);
        } else {
            zExp--;
        }
        shift128RightJamming(zSig0, zSig1, 64, &zSig0, &zSig1);
        return RoundPackTo_f64(zSign, zExp, zSig1);
    } else {
        // Subtraction
        if (expDiff > 0) {
            shift128RightJamming(cSig0, cSig1, expDiff, &cSig0, &cSig1);
            sub128(pSig0, pSig1, cSig0, cSig1, &zSig0, &zSig1);
            zExp = pExp;
        } else if (expDiff < 0) {
            shift128RightJamming(pSig0, pSig1, -expDiff, &pSig0, &pSig1);
            sub128(cSig0, cSig1, pSig0, pSig1, &zSig0, &zSig1);
            zExp = cExp;
            zSign = ! zSign;
        } else {
            zExp = pExp;
            if (lt128(cSig0, cSig1, pSig0, pSig1)) {
                sub128(pSig0, pSig1, cSig0, cSig1, &zSig0, &zSig1);
            } else if (lt128(pSig0, pSig1, cSig0, cSig1)) {
                sub128(cSig0, cSig1, pSig0, pSig1, &zSig0, &zSig1);
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
        --zExp;
        /* Do the equivalent of NormalizeRoundPackTo_f64() but
         * starting with the significand in a pair of u64.
         */
        if (zSig0) {
            shiftcount = countLeadingZeros64( zSig0 ) - 1;
            shortShift128Left( zSig0, zSig1, shiftcount, &zSig0, &zSig1 );
            if (zSig1) {
                zSig0 |= 1;
            }
            zExp -= shiftcount;
        } else {
            assert( zSig1 );
            shiftcount = countLeadingZeros64( zSig1 );
            if (shiftcount == 0) {
                zSig0 = (zSig1 >> 1) | (zSig1 & 1);
                zExp -= 63;
            } else {
                shiftcount--;
                zSig0 = zSig1 << shiftcount;
                zExp -= (shiftcount + 64);
            }
        }
        return RoundPackTo_f64(zSign, zExp, zSig0);
    }
}

} // namespace fpu
} // namespace postrisc
