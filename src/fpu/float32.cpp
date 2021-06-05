#include "util/common.hpp"
#include "util.hpp"
#include "floatimpl.hpp"

namespace postrisc {
namespace fpu {

/*----------------------------------------------------------------------------
| Shifts `a' right by the number of bits given in `count'.  If any nonzero
| bits are shifted off, they are ``jammed'' into the least significant bit of
| the result by setting the least significant bit to 1.  The value of `count'
| can be arbitrarily large; in particular, if `count' is greater than 32(64), the
| result will be either 0 or 1, depending on whether `a' is zero or nonzero.
*----------------------------------------------------------------------------*/
static inline u32 shift32RightJamming(u32 a, int count)
{
    u32 z;

    assert(count >= 0);

    if ( count == 0 ) {
        z = a;
    }
    else if ( count < 32 ) {
        z = ( a>>count ) | ( ( a<<( ( - count ) & 31 ) ) != 0 );
    }
    else {
        z = ( a != 0 );
    }

    return z;
}

commonNaNT SoftFPU::ToCommonNaN(f32 a)
{
    commonNaNT z;

    if ( a.IsSignalingNan() )
       raise(FLOAT_FLAG_INVALID);

    z.sign = a.sign();
    z.low  = 0;
    z.high = u64( a.val() )<< (64 - a.BITS_PER_SIGNIFICAND);
    return z;
}


f32 SoftFPU::to_f32(commonNaNT a)
{
    if (default_nan_mode()) {
        return f32::default_nan();
    }

    const int offset = 64 - f32::BITS_PER_SIGNIFICAND;
    u32 mantissa = a.high >> offset;

    if ( mantissa )
        return f32::make( ( u32(a.sign)<<31 ) | 0x7F800000 | 0x00400000 | mantissa );
    else
        return f32::default_nan();
}


/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper single-precision floating-
| point value corresponding to the abstract input.  Ordinarily, the abstract
| value is simply rounded and packed into the single-precision format, with
| the inexact exception raised if the abstract input cannot be represented
| exactly.  However, if the abstract value is too large, the overflow and
| inexact exceptions are raised and an infinity or maximal finite value is
| returned.  If the abstract value is too small, the input value is rounded to
| a subnormal number, and the underflow and inexact exceptions are raised if
| the abstract input cannot be represented exactly as a subnormal single-
| precision floating-point number.
|     The input significand `zSig' has its binary point between bits 30
| and 29, which is 7 bits to the left of the usual location.  This shifted
| significand must be normalized or smaller.  If `zSig' is not normalized,
| `zExp' must be 0; in that case, the result returned is a subnormal number,
| and it must not require rounding.  In the usual case that `zSig' is
| normalized, `zExp' must be 1 less than the ``true'' floating-point exponent.
| The handling of underflow and overflow follows the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f32 SoftFPU::RoundPackTo_f32(sign_type zSign, fp_exponent_t zExp, f32::fraction_t zSig)
{
    const int NUM_ROUNDING_BITS = 7;
    const int HIGHEST_GUARD_BIT = 1 << (NUM_ROUNDING_BITS - 1); //0x40
    const i32 NNNN = f32::EINF - 2; //0xFD

    const rounding_mode_t roundingMode = get_rounding_mode();
    const bool roundNearestEven = ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN );
    int roundIncrement = HIGHEST_GUARD_BIT;
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = makemask32(NUM_ROUNDING_BITS);
            if ( zSign ) {
                if ( roundingMode == FLOAT_ROUND_UPWARD ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLOAT_ROUND_DOWNWARD ) roundIncrement = 0;
            }
        }
    }
    int roundBits = lobits(zSig, NUM_ROUNDING_BITS);

    if ( static_cast<u32>(NNNN) <= static_cast<u32>(zExp) ) {
        if (    ( NNNN < zExp )
             || (    ( zExp == NNNN )
                  && ( static_cast<i32>( zSig + roundIncrement ) < 0 ) )
           ) {
            raise(FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
            return pack_f32( zSign, f32::EINF, - ( roundIncrement == 0 ));
        }
        if ( zExp < 0 ) {
            if (flush_to_zero()) {
                raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                return f32::zero(zSign);
            }
            bool isTiny = tininess_before_rounding()
                || ( zExp < -1 )
                || ( zSig + roundIncrement < 0x80000000 );
            zSig = shift32RightJamming( zSig, - zExp);
            zExp = 0;
            roundBits = lobits(zSig, NUM_ROUNDING_BITS);
            if ( isTiny && roundBits ) raise(FLOAT_FLAG_UNDERFLOW);
        }
    }
    if ( roundBits )
       raise(FLOAT_FLAG_INEXACT);
    zSig = ( zSig + roundIncrement ) >> NUM_ROUNDING_BITS;
    zSig &= ~ f32::fraction_t( ( ( roundBits ^ HIGHEST_GUARD_BIT ) == 0 ) & roundNearestEven );
    if ( zSig == 0 ) zExp = 0;
    return pack_f32( zSign, zExp, zSig );

}

/*----------------------------------------------------------------------------
| Takes an abstract floating-point value having sign `zSign', exponent `zExp',
| and significand `zSig', and returns the proper single-precision floating-
| point value corresponding to the abstract input.  This routine is just like
| `RoundPackTo_f32' except that `zSig' does not have to be normalized.
| Bit 31 of `zSig' must be zero, and `zExp' must be 1 less than the ``true''
| floating-point exponent.
*----------------------------------------------------------------------------*/

f32 SoftFPU::NormalizeRoundPackTo_f32(sign_type zSign, fp_exponent_t zExp, u32 zSig)
{
    assert( zSig );
    int shiftCount = countLeadingZeros32( zSig ) - 1;
    return RoundPackTo_f32( zSign, zExp - shiftCount, zSig<<shiftCount);
}

/*----------------------------------------------------------------------------
| Returns the result of converting the 32-bit two's complement integer `a'
| to the single-precision floating-point format.  The conversion is performed
| according to the IEC/IEEE Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f32 SoftFPU::to_f32( i32 a )
{
    if ( a == 0 ) return f32_zero;
    if ( a == std::numeric_limits<i32>::min() )
       return pack_f32(SIGN_NEGATIVE, f32::BIAS + 31, 0 );
    sign_type zSign = get_sign(a);
    u64 absA = a < 0 ? -i64(a) : i64(a);
    return NormalizeRoundPackTo_f32( zSign, f32::BIAS + 29, absA );
}

/*----------------------------------------------------------------------------
| Returns the result of converting the 64-bit two's complement integer `a'
| to the single-precision floating-point format.  The conversion is performed
| according to the IEC/IEEE Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f32 SoftFPU::to_f32( i64 a )
{
    if ( a == 0 ) return f32_zero;

    const sign_type zSign = get_sign(a);

    u64 absA = a < 0 ? - a : a;
    int shiftCount = countLeadingZeros64( absA ) - (64 - f32::BITS_PER_SIGNIFICAND - 1); //24

    if ( 0 <= shiftCount )
    {
        return pack_f32( zSign, f32::BIAS + f32::BITS_PER_SIGNIFICAND - 1 - shiftCount, absA<<shiftCount ); //22
    }
    else {
        shiftCount += 7;
        if ( shiftCount < 0 ) {
            absA = shift64RightJamming( absA, - shiftCount);
        } else {
            absA <<= shiftCount;
        }
        return RoundPackTo_f32( zSign, f32::BIAS + 29 - shiftCount, absA ); //0x9C
    }

}

f32 SoftFPU::to_f32( u64 a )
{
    if ( a == 0 ) return f32_zero;
    int shiftCount = countLeadingZeros64(a) - (64 - f32::BITS_PER_SIGNIFICAND - 1); //24

    if ( 0 <= shiftCount )
    {
        return pack_f32(SIGN_POSITIVE, f32::BIAS + f32::BITS_PER_SIGNIFICAND - 1 - shiftCount, a<<shiftCount); //22 //0x95
    }
    else {
        shiftCount += 7;
        if ( shiftCount < 0 ) {
            a = shift64RightJamming( a, - shiftCount);
        }
        else {
            a <<= shiftCount;
        }
        return RoundPackTo_f32(SIGN_POSITIVE, f32::BIAS + 29 - shiftCount, a); //0x9C
    }
}

/*----------------------------------------------------------------------------
| Returns the result of converting the single-precision floating-point value
| `a' to the 32-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32( f32 a )
{
    f32::fraction_t   aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type   a_sign = a.sign();

    if ( a.IsNan() ) a_sign = SIGN_POSITIVE;
    if ( aExp ) aSig |= a.HIDDEN_BIT;

    fp_exponent_t shiftCount = (a.BIAS + (a.BITS_PER_SIGNIFICAND - 7 + 32)) - aExp; //0xAF
    u64 aSig64 = u64(aSig) << 32;
    if ( 0 < shiftCount ) aSig64 = shift64RightJamming( aSig64, shiftCount);
    return round_pack_to_i32(INT_NAN_F32, a_sign, aSig64 );
}

/*----------------------------------------------------------------------------
| Returns the result of converting the single-precision floating-point value
| `a' to the 32-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.
| If `a' is a NaN, the largest positive integer is returned.  Otherwise, if
| the conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

i32 SoftFPU::to_i32_round_to_zero( f32 a )
{
    f32::fraction_t aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type a_sign = a.sign();
    fp_exponent_t shiftCount = aExp - (a.BIAS + 31); //0x9E
    if ( 0 <= shiftCount ) {
        if ( a != f32::make(SIGN_NEGATIVE, 31) ) {
            raise(FLOAT_FLAG_INVALID);
            if ( ! a_sign || a.IsNan() )
               return std::numeric_limits<i32>::max();
        }
        return std::numeric_limits<i32>::min();
    }
    else if ( aExp < a.BIAS ) {
        if ( aExp | aSig ) raise(FLOAT_FLAG_INEXACT);
        return 0;
    }
    aSig = ( aSig | a.HIDDEN_BIT ) << f32::BITS_PER_EXPONENT;
    i32 z = aSig>>( - shiftCount );
    if ( u32( aSig<<( shiftCount & 31 ) ) ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    if ( a_sign ) z = - z;
    return z;

}

/*----------------------------------------------------------------------------
| Returns the result of converting the single-precision floating-point value
| `a' to the 16-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.
| If `a' is a NaN, the largest positive integer is returned.  Otherwise, if
| the conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

int16_t SoftFPU::to_i16_round_to_zero(f32 a)
{
    f32::fraction_t  aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type  a_sign = a.sign();

    fp_exponent_t shiftCount = aExp - a.BIAS - 15; //0x8E

    if ( 0 <= shiftCount ) {
        if ( a != f32::make(SIGN_NEGATIVE, 15) ) {
            raise(FLOAT_FLAG_INVALID);
            if ( ! a_sign || a.IsNan() ) {
                return std::numeric_limits<int16_t>::max();
            }
        }
        return std::numeric_limits<int16_t>::min();
    }
    else if ( aExp < a.BIAS ) {
        if ( aExp | aSig ) {
            raise(FLOAT_FLAG_INEXACT);
        }
        return 0;
    }
    shiftCount -= 16;
    aSig = ( aSig | a.HIDDEN_BIT ) << f32::BITS_PER_EXPONENT;

    i32 z = aSig >> ( - shiftCount );
    if ( u32( aSig<<( shiftCount & 31 ) ) ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    if ( a_sign ) {
        z = - z;
    }
    return z;

}

/*----------------------------------------------------------------------------
| Returns the result of converting the single-precision floating-point value
| `a' to the 64-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic---which means in particular that the conversion is rounded
| according to the current rounding mode.  If `a' is a NaN, the largest
| positive integer is returned.  Otherwise, if the conversion overflows, the
| largest integer with the same sign as `a' is returned.
*----------------------------------------------------------------------------*/

i64 SoftFPU::to_i64( f32 a )
{
    u64 aSig64, aSigExtra;

    f32::fraction_t aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type  a_sign = a.sign();

    fp_exponent_t shiftCount = (a.BIAS + 63) - aExp;  //0xBE

    if ( shiftCount < 0 ) {
        raise(FLOAT_FLAG_INVALID);
        if ( ! a_sign || a.IsNan() ) {
            return std::numeric_limits<i64>::max();
        }
        return std::numeric_limits<i64>::min();
    }

    if ( aExp ) aSig |= a.HIDDEN_BIT;

    aSig64 = aSig;
    aSig64 <<= 40;
    shift64ExtraRightJamming( aSig64, 0, shiftCount, &aSig64, &aSigExtra );
    return round_pack_to_i64( INT_NAN_F32, a_sign, aSig64, aSigExtra );
}

/*----------------------------------------------------------------------------
| Returns the result of converting the single-precision floating-point value
| `a' to the 64-bit two's complement integer format.  The conversion is
| performed according to the IEC/IEEE Standard for Binary Floating-Point
| Arithmetic, except that the conversion is always rounded toward zero.  If
| `a' is a NaN, the largest positive integer is returned.  Otherwise, if the
| conversion overflows, the largest integer with the same sign as `a' is
| returned.
*----------------------------------------------------------------------------*/

i64 SoftFPU::to_i64_round_to_zero( f32 a )
{
    f32::fraction_t aSig = a.fraction();
    fp_exponent_t  aExp = a.exponent();
    sign_type  a_sign = a.sign();

    int shiftCount = aExp - (a.BIAS + 63); //0xBE

    if ( 0 <= shiftCount ) {
        if ( a != f32::make(SIGN_NEGATIVE, 63) ) {
            raise(FLOAT_FLAG_INVALID);
            if ( ! a_sign || a.IsNan() ) {
                return std::numeric_limits<i64>::max();
            }
        }
        return std::numeric_limits<i64>::min();
    }
    else if ( aExp <= a.BIAS - 1 ) { //0x7E
        if ( aExp | aSig ) raise(FLOAT_FLAG_INEXACT);
        return 0;
    }
    u64 aSig64 = aSig | a.HIDDEN_BIT;
    aSig64 <<= 40;
    i64 z = aSig64 >> ( - shiftCount );
    if ( u64( aSig64 << ( shiftCount & 63 ) ) ) {
        raise(FLOAT_FLAG_INEXACT);
    }
    if ( a_sign ) z = - z;
    return z;

}

/*----------------------------------------------------------------------------
| Rounds the single-precision floating-point value `a' to an integer, and
| returns the result as a single-precision floating-point value.  The
| operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f32 SoftFPU::round_to_int( f32 a)
{
    fp_exponent_t aExp = a.exponent();
    if ( a.BIAS + a.BITS_PER_SIGNIFICAND <= aExp ) { //0x96
        if ( a.IsNan() ) {
            return propagate_nan(a);
        }
        return a;
    }
    if ( aExp < a.BIAS ) { //0x7E
        if ( u32 ( a.val()<<1 ) == 0 ) return a;
           raise(FLOAT_FLAG_INEXACT);
        sign_type a_sign = a.sign();
        switch ( get_rounding_mode() ) {
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( ( aExp == a.BIAS - 1 ) && a.fraction() ) { //0x7E
                return pack_f32( a_sign, a.BIAS, 0 );  //0x7F
            }
            break;
         case FLOAT_ROUND_DOWNWARD:
            return f32::make(a_sign ? 0xBF800000 : 0);
         case FLOAT_ROUND_UPWARD:
            return f32::make(a_sign ? 0x80000000 : 0x3F800000);
         default:
         case FLOAT_ROUND_TOWARDZERO:
         break;
        }
        return a.zero(a_sign);
    }
    u32 lastBitMask, roundBitsMask;
    lastBitMask = 1;
    lastBitMask <<= a.BIAS + a.BITS_PER_SIGNIFICAND - aExp; //0x96
    roundBitsMask = lastBitMask - 1;

    f32::fraction_t z = a.val();

    if ( get_rounding_mode() == FLOAT_ROUND_TONEAREST_EVEN ) {
        z += lastBitMask>>1;
        if ( ( z & roundBitsMask ) == 0 ) z &= ~ lastBitMask;
    }
    else if ( get_rounding_mode() != FLOAT_ROUND_TOWARDZERO ) {
        if ( f32::make(z).sign() ^ ( get_rounding_mode() == FLOAT_ROUND_UPWARD ) ) {
            z += roundBitsMask;
        }
    }
    z &= ~ roundBitsMask;
    if ( z != a.val() )
       raise(FLOAT_FLAG_INEXACT);
    return f32::make(z);

}

f32 SoftFPU::round_to_int(f32 a, rounding_mode_t rmode)
{
    rounding_mode_t old = get_rounding_mode();
    set_rounding_mode(rmode);
    f32 t = round_to_int(a);
    set_rounding_mode(old);
    return t;
}




/*----------------------------------------------------------------------------
| Returns the binary exponential of the single-precision floating-point value
| `a'. The operation is performed according to the IEC/IEEE Standard for
| Binary Floating-Point Arithmetic.
|
| Uses the following identities:
|
| 1. -------------------------------------------------------------------------
|      x    x*ln(2)
|     2  = e
|
| 2. -------------------------------------------------------------------------
|                      2     3     4     5           n
|      x        x     x     x     x     x           x
|     e  = 1 + --- + --- + --- + --- + --- + ... + --- + ...
|               1!    2!    3!    4!    5!          n!
*----------------------------------------------------------------------------*/

f32 SoftFPU::exp2( f32 a )
{

    static const u64 f32_exp2_coefficients[15] =
    {
        U64C( 0x3ff0000000000000 ), //  1
        U64C( 0x3fe0000000000000 ), //  2
        U64C( 0x3fc5555555555555 ), //  3
        U64C( 0x3fa5555555555555 ), //  4
        U64C( 0x3f81111111111111 ), //  5
        U64C( 0x3f56c16c16c16c17 ), //  6
        U64C( 0x3f2a01a01a01a01a ), //  7
        U64C( 0x3efa01a01a01a01a ), //  8
        U64C( 0x3ec71de3a556c734 ), //  9
        U64C( 0x3e927e4fb7789f5c ), // 10
        U64C( 0x3e5ae64567f544e4 ), // 11
        U64C( 0x3e21eed8eff8d898 ), // 12
        U64C( 0x3de6124613a86d09 ), // 13
        U64C( 0x3da93974a8c07c9d ), // 14
        U64C( 0x3d6ae7f3e733b81f ), // 15
    };

    sign_type  a_sign;
    fp_exponent_t   aExp;
    f32::fraction_t aSig;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    if ( aExp == a.EINF) {
        if ( aSig ) return propagate_nan(a);
        return (a_sign) ? f32_zero : a;
    }
    if (aExp == 0) {
        if (aSig == 0) return f32_one;
    }

    raise(FLOAT_FLAG_INEXACT);
    if (a_sign && aExp > a.BIAS + 32) {
       return a.zero(SIGN_POSITIVE);
    }

    /* ******************************* */
    /* using f64 for approximation */
    /* ******************************* */
    f64 x = to_f64(a);
    x = mul(x, f64_ln2);

    f64 xn = x;
    f64 r = f64_one;
    for (int i = 0 ; i < 15 ; i++) {
        f64 f = mul(xn, f64::make(f32_exp2_coefficients[i]));
        r = add(r, f);
        xn = mul(xn, x);
    }

    return to_f32(r);
}

/*----------------------------------------------------------------------------
| Returns the binary log of the single-precision floating-point value `a'.
| The operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
f32 SoftFPU::log2( f32 a )
{
    sign_type   a_sign, zSign;
    fp_exponent_t  aExp;
    f32::fraction_t aSig, zSig, i;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();

    if ( aExp == 0 ) {
        if ( aSig == 0 ) {
           raise(FLOAT_FLAG_DIVBYZERO);
           return a.inf(SIGN_NEGATIVE);
        }
        aExp = normalize_subnormal_f32( aSig );
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

    aExp -= a.BIAS; //0x7F;
    aSig |= a.HIDDEN_BIT;
    zSign = get_sign(aExp);
    zSig = aExp << a.BITS_PER_SIGNIFICAND;

    for (i = u32(1) << (a.BITS_PER_SIGNIFICAND - 1); i > 0; i >>= 1) {
        aSig = ( u64(aSig) * aSig ) >> a.BITS_PER_SIGNIFICAND;
        if ( aSig & 0x01000000 ) {
            aSig >>= 1;
            zSig |= i;
        }
    }

    if ( zSign )
        zSig = 0 - zSig; // unary minus unsigned error

    return NormalizeRoundPackTo_f32( zSign, a.BIAS + 6, zSig ); //0x85
}


bool SoftFPU::eq_signaling( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    f32::fraction_t av = a.val();
    f32::fraction_t bv = b.val();
    return ( av == bv ) || IsBothZero(a,b);
}


bool SoftFPU::le_signaling( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    f32::fraction_t av = a.val();
    f32::fraction_t bv = b.val();
    if ( a_sign != b_sign ) return a_sign || IsBothZero(a,b);
    return ( av == bv ) || ( a_sign ^ ( av < bv ) );

}


bool SoftFPU::lt_signaling( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return false;
    }
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    f32::fraction_t av = a.val();
    f32::fraction_t bv = b.val();
    if ( a_sign != b_sign ) return a_sign && !IsBothZero(a,b);
    return ( av != bv ) && ( a_sign ^ ( av < bv ) );

}


bool SoftFPU::unordered( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        raise(FLOAT_FLAG_INVALID);
        return true;
    }
    return false;
}


bool SoftFPU::eq_quiet( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    return ( a.val() == b.val() ) || IsBothZero(a,b);
}


bool SoftFPU::le_quiet( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    f32::fraction_t av = a.val();
    f32::fraction_t bv = b.val();
    if ( a_sign != b_sign ) return a_sign || IsBothZero(a,b);
    return ( av == bv ) || ( a_sign ^ ( av < bv ) );
}


bool SoftFPU::lt_quiet( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return false;
    }
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    f32::fraction_t av = a.val();
    f32::fraction_t bv = b.val();
    if ( a_sign != b_sign ) return a_sign && !IsBothZero(a,b);
    return ( av != bv ) && ( a_sign ^ ( av < bv ) );

}


bool SoftFPU::unordered_quiet( f32 a, f32 b )
{
    if ( a.IsNan() || b.IsNan() ) {
        if ( a.IsSignalingNan() || b.IsSignalingNan() ) {
            raise(FLOAT_FLAG_INVALID);
        }
        return true;
    }
    return false;
}

f32 SoftFPU::to_f32( u32 a )
{
    return to_f32(i64(a));
}

u32 SoftFPU::to_u32( f32 a )
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
        res = v;
    }
    return res;
}

u32 SoftFPU::to_u32_round_to_zero( f32 a )
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

u16 SoftFPU::to_u16_round_to_zero(f32 a)
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

/* Multiply A by 2 raised to the power N.  */
f32 SoftFPU::scalbn( f32 a, int n )
{
    f32::fraction_t aSig = a.fraction();
    fp_exponent_t aExp = a.exponent();
    sign_type a_sign = a.sign();

    if ( aExp == f32::EINF ) {
        if ( aSig ) {
            return propagate_nan(a);
        }
        return a;
    }
    if ( aExp != 0 )
        aSig |= a.HIDDEN_BIT;
    else if ( aSig == 0 )
        return a;

    if (n > 0x200) {
        n = 0x200;
    } else if (n < -0x200) {
        n = -0x200;
    }

    aExp += n - 1;
    aSig <<= 7;
    return NormalizeRoundPackTo_f32( a_sign, aExp, aSig);
}


/***********************************************************************
* merge 3 fp values into one
***********************************************************************/
f32 SoftFPU::merge(f32 sign, f32 exponent, f32 mantissa)
{
    return f32::make(
           (    sign.val() & (makemask32(1) << 31) ) |
           (mantissa.val() &  makemask32(f32::BITS_PER_SIGNIFICAND) ) |
           (exponent.val() & (makemask32(f32::BITS_PER_EXPONENT) << f32::BITS_PER_SIGNIFICAND) ) );
}


float_relation SoftFPU::compare(f32 a, f32 b) {  return compare_internal(a, b, false);  }
float_relation SoftFPU::compare_quiet(f32 a, f32 b) {  return compare_internal(a, b, true);  }

compare_mode_t SoftFPU::compare(f32 a, f32 b, compare_mode_t mode)
{
    return compare_internal(a, b, mode);
}

f32 SoftFPU::min(f32 a, f32 b, bool ignore_nans) {  return minmax<f32>(a, b, ignore_nans, true);  }
f32 SoftFPU::max(f32 a, f32 b, bool ignore_nans) {  return minmax<f32>(a, b, ignore_nans, false); }


/*----------------------------------------------------------------------------
| Returns the result of adding the absolute values of the single-precision
| floating-point values `a' and `b'.  If `zSign' is 1, the sum is negated
| before being returned.  `zSign' is ignored if the result is a NaN.
| The addition is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f32 SoftFPU::addSigs( f32 a, f32 b, sign_type zSign)
{
    fp_exponent_t aExp, bExp, zExp, expDiff;
    f32::fraction_t aSig, bSig, zSig;

    const int FIXED_POINT = 6;
    const f32::fraction_t FIXED_HIDDEN_BIT = u32(1) << (a.BITS_PER_SIGNIFICAND + FIXED_POINT);

    aSig = a.fraction();
    aExp = a.exponent();
    bSig = b.fraction();
    bExp = b.exponent();
    expDiff = aExp - bExp;
    aSig <<= FIXED_POINT;
    bSig <<= FIXED_POINT;
    if ( 0 < expDiff ) {
        if ( aExp == a.EINF ) {
            if ( aSig ) return propagate_nan(a, b);
            return a;
        }
        if ( bExp == 0 ) {
            --expDiff;
        }
        else {
            bSig |= FIXED_HIDDEN_BIT;
        }
        bSig = shift32RightJamming( bSig, expDiff);
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
            aSig |= FIXED_HIDDEN_BIT;
        }
        aSig = shift32RightJamming( aSig, - expDiff);
        zExp = bExp;
    }
    else {
        if ( aExp == a.EINF ) {
            if ( aSig | bSig ) return propagate_nan(a, b);
            return a;
        }
        if ( aExp == 0 ) {
            if (flush_to_zero()) {
                if (aSig | bSig) {
                    raise(FLOAT_FLAG_OUTPUT_DENORMAL);
                }
                return a.zero(zSign);
            }
            return pack_f32( zSign, 0, ( aSig + bSig ) >> FIXED_POINT );
        }
        zSig = 2*FIXED_HIDDEN_BIT + aSig + bSig;
        zExp = aExp;
        goto roundAndPack;
    }
    aSig |= FIXED_HIDDEN_BIT;
    zSig = ( aSig + bSig )<<1;
    --zExp;
    if ( i32(zSig) < 0 ) {
        zSig = aSig + bSig;
        ++zExp;
    }
 roundAndPack:
    return RoundPackTo_f32( zSign, zExp, zSig );

}

/*----------------------------------------------------------------------------
| Returns the result of subtracting the absolute values of the single-
| precision floating-point values `a' and `b'.  If `zSign' is 1, the
| difference is negated before being returned.  `zSign' is ignored if the
| result is a NaN.  The subtraction is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/

f32 SoftFPU::subSigs( f32 a, f32 b, sign_type zSign)
{
    fp_exponent_t aExp, bExp, zExp;
    f32::fraction_t aSig, bSig, zSig;
    int expDiff;

    const int FIXED_POINT = 7;
    const u32 FIXED_HIDDEN_BIT = u32(1) << (a.BITS_PER_SIGNIFICAND + FIXED_POINT);

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
        if ( aSig | bSig ) return propagate_nan(a, b);
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
        if ( bSig ) return propagate_nan(a, b);
        return b.inf(zSign ^ SIGN_NEGATIVE);
    }
    if ( aExp == 0 ) {
        ++expDiff;
    }
    else {
        aSig |= FIXED_HIDDEN_BIT;
    }
    aSig = shift32RightJamming( aSig, - expDiff);
    bSig |= FIXED_HIDDEN_BIT;
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
        bSig |= FIXED_HIDDEN_BIT;
    }
    bSig = shift32RightJamming( bSig, expDiff);
    aSig |= FIXED_HIDDEN_BIT;
 aBigger:
    zSig = aSig - bSig;
    zExp = aExp;
 normalizeRoundAndPack:
    --zExp;
    return NormalizeRoundPackTo_f32( zSign, zExp, zSig );

}


f32 SoftFPU::add( f32 a, f32 b )
{
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    if ( a_sign == b_sign ) {
        return addSigs( a, b, a_sign);
    } else {
        return subSigs( a, b, a_sign );
    }

}


f32 SoftFPU::sub( f32 a, f32 b )
{
    sign_type a_sign = a.sign();
    sign_type b_sign = b.sign();
    if ( a_sign == b_sign ) {
        return subSigs( a, b, a_sign );
    } else {
        return addSigs( a, b, a_sign );
    }

}


f32 SoftFPU::mul( f32 a, f32 b )
{
    sign_type   a_sign, b_sign, zSign;
    fp_exponent_t aExp, bExp, zExp;
    f32::fraction_t aSig, bSig;
    u64  zSig64;
    f32::fraction_t zSig;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();
    zSign = a_sign ^ b_sign;
    if ( aExp == a.EINF ) {
        if ( aSig || ( ( bExp == b.EINF ) && bSig ) ) {
            return propagate_nan(a, b);
        }
        if ( ( bExp | bSig ) == 0 ) {
            raise(FLOAT_FLAG_INVALID);
            return b.default_nan();
        }
        return a.inf(zSign);
    }
    if ( bExp == b.EINF ) {
        if ( bSig ) return propagate_nan(a, b);
        if ( ( aExp | aSig ) == 0 ) {
            raise(FLOAT_FLAG_INVALID);
            return a.default_nan();
        }
        return b.inf(zSign);
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a.zero(zSign);
        aExp = normalize_subnormal_f32( aSig );
    }
    if ( bExp == 0 ) {
        if ( bSig == 0 ) return b.zero(zSign);
        bExp = normalize_subnormal_f32( bSig );
    }
    zExp = aExp + bExp - f32::BIAS; //0x7F;
    aSig = ( aSig | a.HIDDEN_BIT ) << (f32::BITS_PER_EXPONENT - 1);
    bSig = ( bSig | b.HIDDEN_BIT ) << (f32::BITS_PER_EXPONENT);
    zSig64 = shift64RightJamming( u64(aSig) * bSig, 32);
    zSig = static_cast<u32>(zSig64);
    if ( 0 <= i32( zSig<<1 ) ) {
        zSig <<= 1;
        --zExp;
    }
    return RoundPackTo_f32( zSign, zExp, zSig );

}


f32 SoftFPU::div( f32 a, f32 b )
{
    sign_type   a_sign, b_sign, zSign;
    fp_exponent_t aExp, bExp, zExp;
    f32::fraction_t aSig, bSig, zSig;

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
        bExp = normalize_subnormal_f32( bSig );
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a.zero(zSign);
        aExp = normalize_subnormal_f32( aSig );
    }
    zExp = aExp - bExp + 0x7D;
    aSig = ( aSig | a.HIDDEN_BIT ) << (f32::BITS_PER_EXPONENT - 1);
    bSig = ( bSig | b.HIDDEN_BIT ) << (f32::BITS_PER_EXPONENT);
    if ( bSig <= ( aSig + aSig ) ) {
        aSig >>= 1;
        ++zExp;
    }
    zSig = ( u64(aSig)<<32 ) / bSig;
    if ( ( zSig & 0x3F ) == 0 ) {
        zSig |= ( u64(bSig) * zSig != (u64(aSig) << 32) );
    }
    return RoundPackTo_f32( zSign, zExp, zSig );

}


f32 SoftFPU::rem( f32 a, f32 b )
{
    sign_type  a_sign, zSign;
    fp_exponent_t  aExp, bExp, expDiff;
    f32::fraction_t aSig, bSig;
    u32 q;
    u64 aSig64, bSig64, q64;
    u32 alternateASig;
    i32 sigMean;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    bSig = b.fraction();
    bExp = b.exponent();
    if ( aExp == a.EINF ) {
        if ( aSig || ( ( bExp == f32::EINF ) && bSig ) ) {
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
        bExp = normalize_subnormal_f32( bSig );
    }
    if ( aExp == 0 ) {
        if ( aSig == 0 ) return a;
        aExp = normalize_subnormal_f32( aSig );
    }
    expDiff = aExp - bExp;
    aSig |= a.HIDDEN_BIT;
    bSig |= b.HIDDEN_BIT;
    if ( expDiff < 32 ) {
        aSig <<= f32::BITS_PER_EXPONENT;
        bSig <<= f32::BITS_PER_EXPONENT;
        if ( expDiff < 0 ) {
            if ( expDiff < -1 ) return a;
            aSig >>= 1;
        }
        q = ( bSig <= aSig );
        if ( q ) aSig -= bSig;
        if ( 0 < expDiff ) {
            q = ( u64(aSig) << 32 ) / bSig;
            q >>= 32 - expDiff;
            bSig >>= 2;
            aSig = ( ( aSig>>1 )<<( expDiff - 1 ) ) - bSig * q;
        }
        else {
            aSig >>= 2;
            bSig >>= 2;
        }
    }
    else {
        if ( bSig <= aSig ) aSig -= bSig;
        aSig64 = u64( aSig )<<40;
        bSig64 = u64( bSig )<<40;
        expDiff -= 64;
        while ( 0 < expDiff ) {
            q64 = estimateDiv128To64( aSig64, 0, bSig64 );
            q64 = ( 2 < q64 ) ? q64 - 2 : 0;
            aSig64 = 0 - ( ( bSig * q64 )<<38 ); // unary minus unsigned error
            expDiff -= 62;
        }
        expDiff += 64;
        q64 = estimateDiv128To64( aSig64, 0, bSig64 );
        q64 = ( 2 < q64 ) ? q64 - 2 : 0;
        q = q64>>( 64 - expDiff );
        bSig <<= 6;
        aSig = ( ( aSig64>>33 )<<( expDiff - 1 ) ) - bSig * q;
    }
    do {
        alternateASig = aSig;
        ++q;
        aSig -= bSig;
    } while ( 0 <= i32(aSig) );
    sigMean = aSig + alternateASig;
    if ( ( sigMean < 0 ) || ( ( sigMean == 0 ) && ( q & 1 ) ) ) {
        aSig = alternateASig;
    }
    zSign = get_sign( i32(aSig) );
    if ( zSign ) aSig = 0 - aSig; // unary minus unsigned error
    return NormalizeRoundPackTo_f32( a_sign ^ zSign, bExp, aSig );

}


f32 SoftFPU::sqrt( f32 a )
{
    sign_type  a_sign;
    fp_exponent_t aExp, zExp;
    f32::fraction_t aSig, zSig;
    u64 rem, term;

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
        if ( aSig == 0 ) return f32_zero;
        aExp = normalize_subnormal_f32( aSig );
    }
    zExp = ( ( aExp - 0x7F )>>1 ) + 0x7E;
    aSig = ( aSig | a.HIDDEN_BIT ) << f32::BITS_PER_EXPONENT;
    zSig = estimateSqrt32( aExp, aSig ) + 2;
    if ( ( zSig & 0x7F ) <= 5 ) {
        if ( zSig < 2 ) {
            zSig = makemask32(31);
            goto roundAndPack;
        }
        aSig >>= aExp & 1;
        term =  u64( zSig ) * zSig;
        rem = ( u64( aSig ) << 32 ) - term;
        while ( i64(rem) < 0 ) {
            --zSig;
            rem += ( u64(zSig) << 1 ) | 1;
        }
        zSig |= ( rem != 0 );
    }
    zSig = shift32RightJamming( zSig, 1);
 roundAndPack:
    return RoundPackTo_f32( SIGN_POSITIVE, zExp, zSig );

}


/*----------------------------------------------------------------------------
| Returns the result of multiplying the single-precision floating-point values
| `a' and `b' then adding 'c', with no intermediate rounding step after the
| multiplication.  The operation is performed according to the IEC/IEEE
| Standard for Binary Floating-Point Arithmetic 754-2008.
| The flags argument allows the caller to select negation of the
| addend, the intermediate product, or the final result. (The difference
| between this and having the caller do a separate negation is that negating
| externally will flip the sign bit on NaNs.)
*----------------------------------------------------------------------------*/

f32 SoftFPU::muladd(f32 a, f32 b, f32 c, muladd_negate_t flags)
{
    sign_type  a_sign, b_sign, cSign, zSign;
    fp_exponent_t aExp, bExp, cExp, pExp, zExp, expDiff;
    f32::fraction_t aSig, bSig, cSig;
    u64 pSig64, cSig64, zSig64;
    u32 pSig;

    aSig = a.fraction();
    aExp = a.exponent();
    a_sign = a.sign();
    bSig = b.fraction();
    bExp = b.exponent();
    b_sign = b.sign();
    cSig = c.fraction();
    cExp = c.exponent();
    cSign = c.sign();

    bool infzero = ((aExp == 0 && aSig == 0 && bExp == b.EINF && bSig == 0) ||
                    (aExp == a.EINF && aSig == 0 && bExp == 0 && bSig == 0));

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
    bool pInf = (aExp == a.EINF) || (bExp == b.EINF);
    bool pZero = ((aExp | aSig) == 0) || ((bExp | bSig) == 0);

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
        return pack_f32(cSign, cExp, cSig);
    }

    if (aExp == 0) {
        aExp = normalize_subnormal_f32( aSig );
    }
    if (bExp == 0) {
        bExp = normalize_subnormal_f32( bSig );
    }

    /* Calculate the actual result a * b + c */

    /* Multiply first; this is easy. */
    /* NB: we subtract 0x7e where mul() subtracts 0x7f
     * because we want the true exponent, not the "one-less-than"
     * flavour that RoundPackTo_f32() takes.
     */
    pExp = aExp + bExp - (a.BIAS - 1); //0x7e
    aSig = (aSig | a.HIDDEN_BIT) << (f32::BITS_PER_EXPONENT - 1);
    bSig = (bSig | b.HIDDEN_BIT) << (f32::BITS_PER_EXPONENT);
    pSig64 = u64(aSig) * bSig;
    if (i64(pSig64 << 1) >= 0) {
        pSig64 <<= 1;
        pExp--;
    }

    zSign = pSign;

    // Now pSig64 is the significand of the multiply, with the explicit bit in position 62.
    if (cExp == 0) {
        if (!cSig) {
            // Throw out the special case of c being an exact zero now
            pSig64 = shift64RightJamming(pSig64, 32);
            pSig = static_cast<u32>(pSig64);
            return RoundPackTo_f32(zSign, pExp - 1, pSig);
        }
        cExp = normalize_subnormal_f32( cSig );
    }

    cSig64 = u64(cSig) << (62 - a.BITS_PER_SIGNIFICAND);
    cSig64 |= U64C(0x4000000000000000);
    expDiff = pExp - cExp;

    if (pSign == cSign) {
        // Addition
        if (expDiff > 0) {
            // scale c to match p
            cSig64 = shift64RightJamming(cSig64, expDiff);
            zExp = pExp;
        } else if (expDiff < 0) {
            // scale p to match c
            pSig64 = shift64RightJamming(pSig64, -expDiff);
            zExp = cExp;
        } else {
            // no scaling needed
            zExp = cExp;
        }
        // Add significands and make sure explicit bit ends up in posn 62
        zSig64 = pSig64 + cSig64;
        if (i64(zSig64) < 0) {
            zSig64 = shift64RightJamming(zSig64, 1);
        } else {
            zExp--;
        }
    } else {
        // Subtraction
        if (expDiff > 0) {
            cSig64 = shift64RightJamming(cSig64, expDiff);
            zSig64 = pSig64 - cSig64;
            zExp = pExp;
        } else if (expDiff < 0) {
            pSig64 = shift64RightJamming(pSig64, -expDiff);
            zSig64 = cSig64 - pSig64;
            zExp = cExp;
            zSign = ! zSign;
        } else {
            zExp = pExp;
            if (cSig64 < pSig64) {
                zSig64 = pSig64 - cSig64;
            } else if (pSig64 < cSig64) {
                zSig64 = cSig64 - pSig64;
                zSign = ! zSign;
            } else {
                // Exact zero
                //if (get_rounding_mode() == FLOAT_ROUND_DOWNWARD) {
                //    zSign = ! zSign;
                //}
                return c.zero(sign_type(get_rounding_mode() == FLOAT_ROUND_DOWNWARD));
            }
        }
        --zExp;
        // Normalize to put the explicit bit back into bit 62.
        assert( zSig64 );
        int shiftcount = countLeadingZeros64( zSig64 ) - 1;
        zSig64 <<= shiftcount;
        zExp -= shiftcount;
    }
    zSig64 = shift64RightJamming(zSig64, 32);
    return RoundPackTo_f32(zSign, zExp, zSig64);
}

} //namespace fpu
} // namespace postrisc
