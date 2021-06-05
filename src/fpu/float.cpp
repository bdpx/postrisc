#include "util/quadruple.hpp"
#include "util.hpp"
#include "floatimpl.hpp"
#include "float_base.hpp"

namespace postrisc {
namespace fpu {

enum {
  disp_flush_to_zero       = 0,
  disp_rounding_precision  = disp_flush_to_zero + 1,
  disp_rounding_mode       = disp_rounding_precision + 2,
  disp_tininess_mode       = disp_rounding_mode + 2,
  disp_flush_input_to_zero = disp_tininess_mode + 1,
  disp_default_nan_mode    = disp_flush_input_to_zero + 1,
  disp_flags               = 8,
};

u64
BaseFPU::get_control_word(void) const
{
    u64 word = 0;
    word |= static_cast<u64>(get_flags())                << disp_flags;
    word |= static_cast<u64>(get_rounding_mode())        << disp_rounding_mode;
    word |= static_cast<u64>(get_rounding_precision())   << disp_rounding_precision;
    word |= static_cast<u64>(tininess_before_rounding()) << disp_tininess_mode;
    word |= static_cast<u64>(flush_to_zero())            << disp_flush_to_zero;
    word |= static_cast<u64>(flush_inputs_to_zero())     << disp_flush_input_to_zero;
    word |= static_cast<u64>(default_nan_mode())         << disp_default_nan_mode;
    return word;
}


void
BaseFPU::set_control_word(u64 word)
{
    set_exception_flags(              (float_flag) lobits(word >> disp_flags, 8) );
    set_rounding_mode(           (rounding_mode_t) lobits(word >> disp_rounding_mode, 2) );
    set_rounding_precision( (rounding_precision_t) lobits(word >> disp_rounding_precision, 2) );
    set_tininess_mode(           (tininess_mode_t) lobits(word >> disp_tininess_mode, 1) );
    set_flush_to_zero(                             lobits(word >> disp_flush_to_zero, 1) );
    set_flush_inputs_to_zero(                      lobits(word >> disp_flush_input_to_zero, 1) );
    set_default_nan_mode(                          lobits(word >> disp_default_nan_mode, 1) );
}


// TODO - how to about local ieee flags? clear or keep?
void
BaseFPU::change_control_word(u64 and_mask, u64 or_mask)
{
    const u64 mask = makemask64(7);
    u64 old = get_control_word();
    u64 tmp = old & ~mask;
    old = ((old & mask) | or_mask);
    tmp = tmp | (old & ~and_mask);
    set_control_word( tmp );
}

std::ostream& operator<<(std::ostream& out, float_flag flags)
{
    return out
    << char( flags & FLOAT_FLAG_INVALID         ? 'V' : 'v')
    << char( flags & FLOAT_FLAG_DIVBYZERO       ? 'Z' : 'z')
    << char( flags & FLOAT_FLAG_OVERFLOW        ? 'O' : 'o')
    << char( flags & FLOAT_FLAG_UNDERFLOW       ? 'U' : 'u')
    << char( flags & FLOAT_FLAG_INEXACT         ? 'X' : 'x')
    << char( flags & FLOAT_FLAG_INPUT_DENORMAL  ? 'T' : 't')
    << char( flags & FLOAT_FLAG_OUTPUT_DENORMAL ? 'D' : 'd') ;
}


std::ostream& operator<<(std::ostream& out, const BaseFPU& fpu)
{
    return out
    << fmt::hex(fpu.get_control_word())
    << " fe=" << fpu.get_flags()
    << " rm=" << int(fpu.get_rounding_mode())
    << " rp=" << int(fpu.get_rounding_precision())
    << " tm=" << int(fpu.get_tininess_mode())
    << " ftz=" << int(fpu.flush_to_zero())
    << " fiz=" << int(fpu.flush_inputs_to_zero())
    << " dnm=" << int(fpu.default_nan_mode());
}


/******************************************************************************
* write soft-float as internal hex format for debug
******************************************************************************/
void writeHex(std::ostream& out, const f32& f)
{
    out << fmt::hex<u32, 3>(f.val() >> f.BITS_PER_SIGNIFICAND) << '.' << fmt::hex<u32, 6>(f.fraction()) << ' ' << f.hard();
}

void writeHex(std::ostream& out, const f64& f)
{
    out << fmt::hex<u64, 3>(f.val() >> f.BITS_PER_SIGNIFICAND) << '.' << fmt::hex<u64, 13>(f.fraction()) << ' ' << f.hard();
}

void writeHex(std::ostream& out, const f80& f)
{
    out << fmt::hex<u16, 4>(f.high) << '.' << fmt::hex<u64, 16>(f.fraction()) << ' ' << f.hard();
}

void writeHex(std::ostream& out, const f128& f)
{
    using postrisc::operator<<;

    out << fmt::hex<u64, 4>(f.high >> f.BITS_PER_HIGH_SIGNIFICAND)
        << '.'
        << fmt::hex<u64, 12>(lobits(f.high, f.BITS_PER_HIGH_SIGNIFICAND))
        << fmt::hex<u64, 16>(f.low)
        << ' ' << f.hard();
}

void
writeHex(std::ostream& out, const f256& f)
{
    out << std::setfill('0') << std::hex
        << std::setw(5) << (f.m0 >> f.BITS_PER_HIGH_SIGNIFICAND)
        << '.'
        << std::setw(11) << lobits(f.m0, f.BITS_PER_HIGH_SIGNIFICAND)
        << std::setw(16) << f.m1
        << std::setw(16) << f.m2
        << std::setw(16) << f.m3;
}

void
writeHex(std::ostream& out, const f512& f)
{
    out << std::setfill('0') << std::hex
        << std::setw(6) << (f.m0 >> f.BITS_PER_HIGH_SIGNIFICAND)
        << '.'
        << std::setw(10) << lobits(f.m0, f.BITS_PER_HIGH_SIGNIFICAND)
        << std::setw(16) << f.m1
        << std::setw(16) << f.m2
        << std::setw(16) << f.m3
        << std::setw(16) << f.m4
        << std::setw(16) << f.m5
        << std::setw(16) << f.m6
        << std::setw(16) << f.m7;
}


/*----------------------------------------------------------------------------
| Takes a 64-bit fixed-point value `absZ' with binary point between bits 6
| and 7, and returns the properly rounded 32-bit integer corresponding to the
| input.  If `zSign' is 1, the input is negated before being converted to an
| integer.  Bit 63 of `absZ' must be zero.  Ordinarily, the fixed-point input
| is simply rounded to an integer, with the inexact exception raised if the
| input cannot be represented exactly as an integer.  However, if the fixed-
| point input is too large, the invalid exception is raised and the largest
| positive or negative integer is returned.
*----------------------------------------------------------------------------*/
i32
SoftFPU::round_pack_to_i32(int_nan_mode_t bothSignedNAN, sign_type zSign, u64 absZ)
{
    const int NUM_ROUNDING_BITS = 7;
    const int HIGHEST_GUARD_BIT = 1 << (NUM_ROUNDING_BITS - 1); //0x40

    assert((absZ >> 63) == 0);
    rounding_mode_t roundingMode = get_rounding_mode();
    bool roundNearestEven = ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN );
    int roundIncrement = HIGHEST_GUARD_BIT; //0x40
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            roundIncrement = 0;
        }
        else {
            roundIncrement = makemask32(NUM_ROUNDING_BITS); //0x7F;
            if ( zSign ) {
                if ( roundingMode == FLOAT_ROUND_UPWARD ) roundIncrement = 0;
            }
            else {
                if ( roundingMode == FLOAT_ROUND_DOWNWARD ) roundIncrement = 0;
            }
        }
    }
    int roundBits = lobits(absZ, NUM_ROUNDING_BITS); // & 0x7F;
    absZ = ( absZ + roundIncrement ) >> NUM_ROUNDING_BITS;
    absZ &= ~ u64( ( ( roundBits ^ HIGHEST_GUARD_BIT ) == 0 ) & roundNearestEven );  //0x40
    i32 z = absZ;
    if ( zSign ) z = - z;
    if ( ( absZ>>32 ) || ( z && ( ( z < 0 ) ^ zSign ) ) ) {
        raise(FLOAT_FLAG_INVALID);
        return (zSign || bothSignedNAN == INT_NAN_MINONLY)
              ? std::numeric_limits<i32>::min()
              : std::numeric_limits<i32>::max();
    }
    if ( roundBits ) raise(FLOAT_FLAG_INEXACT);
    return z;

}

/*----------------------------------------------------------------------------
| Takes the 128-bit fixed-point value formed by concatenating `absZ0' and
| `absZ1', with binary point between bits 63 and 64 (between the input words),
| and returns the properly rounded 64-bit integer corresponding to the input.
| If `zSign' is 1, the input is negated before being converted to an integer.
| Ordinarily, the fixed-point input is simply rounded to an integer, with
| the inexact exception raised if the input cannot be represented exactly as
| an integer.  However, if the fixed-point input is too large, the invalid
| exception is raised and the largest positive or negative integer is
| returned.
*----------------------------------------------------------------------------*/

i64
SoftFPU::round_pack_to_i64(int_nan_mode_t bothSignedNAN, sign_type zSign, u64 absZ0, u64 absZ1)
{
    i64 z;

    rounding_mode_t roundingMode = get_rounding_mode();
    bool roundNearestEven = ( roundingMode == FLOAT_ROUND_TONEAREST_EVEN );
    bool increment = ( i64(absZ1) < 0 );
    if ( ! roundNearestEven ) {
        if ( roundingMode == FLOAT_ROUND_TOWARDZERO ) {
            increment = 0;
        }
        else {
            if ( zSign ) {
                increment = ( roundingMode == FLOAT_ROUND_DOWNWARD ) && absZ1;
            }
            else {
                increment = ( roundingMode == FLOAT_ROUND_UPWARD ) && absZ1;
            }
        }
    }
    if ( increment ) {
        ++absZ0;
        if ( absZ0 == 0 ) goto overflow;
        absZ0 &= ~ u64( ( u64( absZ1<<1 ) == 0 ) & roundNearestEven );
    }
    z = absZ0;
    if ( zSign ) z = - z;
    if ( z && ( ( z < 0 ) ^ zSign ) ) {
 overflow:
        raise(FLOAT_FLAG_INVALID);
        return (zSign || bothSignedNAN == INT_NAN_MINONLY)
             ? std::numeric_limits<i64>::min()
             : std::numeric_limits<i64>::max();
    }
    if ( absZ1 ) raise(FLOAT_FLAG_INEXACT);
    return z;
}


/*----------------------------------------------------------------------------
| Returns the result of converting the floating-point NaN to the canonical NaN format.
| If `a' is a signaling NaN, the invalid exception is raised.
*----------------------------------------------------------------------------*/
commonNaNT SoftFPU::ToCommonNaN(f16 a)
{
    commonNaNT z;

    if ( a.IsSignalingNan() )
       raise(FLOAT_FLAG_INVALID);

    z.sign = a.sign();
    z.low = 0;
    z.high = u64(a.val()) << ( 64 - a.BITS_PER_SIGNIFICAND );
    return z;
}


/*----------------------------------------------------------------------------
| Convert the canonical NaN to the floating-point format.
*----------------------------------------------------------------------------*/
f16 SoftFPU::to_f16(commonNaNT a)
{
    if( default_nan_mode() ) {
       return f16::default_nan();
    }

    u16 mantissa = a.high >> ( 64 - f16::BITS_PER_SIGNIFICAND );
    if( mantissa ) {
       return f16( (u16(a.sign) << 15) | (f16::EINF << f16::BITS_PER_SIGNIFICAND) | mantissa);
    } else {
       return f16::default_nan();
    }
}


/* Half precision floats come in two formats: standard IEEE and "ARM" format.
   The latter gains extra exponent range by omitting the NaN/Inf encodings.  */

f32 SoftFPU::to_f32(f16 a, bool ieee)
{
    fp_exponent_t aExp;
    u32  aSig;

    sign_type a_sign = a.sign();
    aExp = a.exponent();
    aSig = a.fraction();

    if (aExp == a.EINF && ieee) {
        if (aSig) {
            return to_f32(ToCommonNaN(a));
        }
        return f32::inf(a_sign);
    }
    if (aExp == 0) {
        // denormal or zero
        if (aSig == 0) {
            // zero
            return f32::zero(a_sign);
        }
        // denormal
        int shiftCount = countLeadingZeros32( aSig ) - (32 - a.BITS_PER_SIGNIFICAND - 1);
        aSig = aSig << shiftCount;
        aExp = -shiftCount;
    }

    return pack_f32(a_sign,
           aExp + f32::BIAS - a.BIAS,
           aSig << (f32::BITS_PER_SIGNIFICAND - a.BITS_PER_SIGNIFICAND));
}


f16 SoftFPU::to_f16(f32 a, bool ieee)
{
    u32     aSig  = a.fraction();
    fp_exponent_t aExp  = a.exponent();
    sign_type  a_sign = a.sign();

    if ( aExp == a.EINF ) {
        if (aSig) {
            // Input is a NaN
            f16 r = to_f16( ToCommonNaN( a));
            if (!ieee) {
                return pack_f16(a_sign, 0, 0);
            }
            return r;
        }
        // Infinity
        if (!ieee) {
            raise(FLOAT_FLAG_INVALID);
            return pack_f16(a_sign, f16::EINF, 0x3ff);
        }
        return pack_f16(a_sign, f16::EINF, 0);
    }
    if (aExp == 0 && aSig == 0) {
        return pack_f16(a_sign, 0, 0);
    }

    // Decimal point between bits 22 and 23.
    aSig |= a.HIDDEN_BIT;
    aExp -= a.BIAS;

    u32  mask;

    if (aExp < -14) {
        mask = makemask32(23 + 1);
        if (aExp >= -24) {
            mask >>= 25 + aExp;
        }
    } else {
        mask = makemask32(13);
    }

    if (aSig & mask) {
        u32 increment;
        raise(FLOAT_FLAG_UNDERFLOW);
        switch (get_rounding_mode()) {
            case FLOAT_ROUND_TONEAREST_EVEN:
                increment = (mask + 1) >> 1;
                if ( (aSig & mask) == increment ) {
                    increment = aSig & (increment << 1);
                }
                break;
            case FLOAT_ROUND_UPWARD:
                increment = a_sign ? 0 : mask;
                break;
            case FLOAT_ROUND_DOWNWARD:
                increment = a_sign ? mask : 0;
                break;
            default: // round_to_zero
            case FLOAT_ROUND_TOWARDZERO:
                increment = 0;
                break;
        }
        aSig += increment;
        if (aSig >= 0x01000000) {
            aSig >>= 1;
            aExp++;
        }
    } else if (aExp < -14 && tininess_before_rounding()) {
        raise(FLOAT_FLAG_UNDERFLOW);
    }

    if (ieee) {
        if (aExp > 15) {
            raise(FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
            return pack_f16(a_sign, f16::EINF, 0);
        }
    } else {
        if (aExp > 16) {
            raise(FLOAT_FLAG_INVALID | FLOAT_FLAG_INEXACT);
            return pack_f16(a_sign, f16::EINF, 0x3ff);
        }
    }
    if (aExp < -24) {
        return pack_f16(a_sign, 0, 0);
    }
    if (aExp < -14) {
        aSig >>= -14 - aExp;
        aExp = -14;
    }
    return pack_f16(a_sign, aExp + 14, aSig >> 13);
}


/*----------------------------------------------------------------------------
| Returns an approximation to the 64-bit integer quotient obtained by dividing
| `b' into the 128-bit value formed by concatenating `a0' and `a1'.  The
| divisor `b' must be at least 2^63.  If q is the exact quotient truncated
| toward zero, the approximation returned lies between q and q + 2 inclusive.
| If the exact quotient q is larger than 64 bits, the maximum positive 64-bit
| unsigned integer is returned.
*----------------------------------------------------------------------------*/

u64 SoftFPU::estimateDiv128To64( u64 a0, u64 a1, u64 b )
{
    u64   b0, b1, rem0, rem1, term0, term1;

    assert( (b >> 63) != 0 );
    if ( b <= a0 ) return std::numeric_limits<u64>::max();
    b0 = b>>32;
    u64 z = ( b0<<32 <= a0 ) ? makemask64(32) << 32 : ( a0 / b0 ) << 32;
    mul64To128( b, z, &term0, &term1 );
    sub128( a0, a1, term0, term1, &rem0, &rem1 );
    while ( i64(rem0) < 0 ) {
        z -= ( u64(1) << 32 );
        b1 = b<<32;
        add128( rem0, rem1, b0, b1, &rem0, &rem1 );
    }
    rem0 = ( rem0<<32 ) | ( rem1>>32 );
    z |= ( b0<<32 <= rem0 ) ? makemask64(32) : rem0 / b0;
    return z;
}


/*----------------------------------------------------------------------------
| Returns an approximation to the square root of the 32-bit significand given
| by `a'.  Considered as an integer, `a' must be at least 2^31.  If bit 0 of
| `aExp' (the least significant bit) is 1, the integer returned approximates
| 2^31*sqrt(`a'/2^31), where `a' is considered an integer.  If bit 0 of `aExp'
| is 0, the integer returned approximates 2^31*sqrt(`a'/2^30).  In either
| case, the approximation returned lies strictly within +/-2 of the exact
| value.
*----------------------------------------------------------------------------*/

u32 SoftFPU::estimateSqrt32(int aExp, u32 a)
{
    static const u16 sqrtOddAdjustments[] = {
        0x0004, 0x0022, 0x005D, 0x00B1, 0x011D, 0x019F, 0x0236, 0x02E0,
        0x039C, 0x0468, 0x0545, 0x0631, 0x072B, 0x0832, 0x0946, 0x0A67
    };
    static const u16 sqrtEvenAdjustments[] = {
        0x0A2D, 0x08AF, 0x075A, 0x0629, 0x051A, 0x0429, 0x0356, 0x029E,
        0x0200, 0x0179, 0x0109, 0x00AF, 0x0068, 0x0034, 0x0012, 0x0002
    };

    u32 z;

    assert( (a >> 31) != 0 );
    int index = ( a>>27 ) & 15;
    if ( aExp & 1 ) {
        z = 0x4000 + ( a>>17 ) - sqrtOddAdjustments[ index ];
        z = ( ( a / z )<<14 ) + ( z<<15 );
        a >>= 1;
    } else {
        z = 0x8000 + ( a>>17 ) - sqrtEvenAdjustments[ index ];
        z = a / z + z;
        z = ( 0x20000 <= z ) ? makemask32(17) << 15 : ( z<<15 );
        if ( z <= a ) return u32( i32(a)>>1 );
    }
    return ( u32 ( ( u64(a)<<31 ) / z ) ) + ( z>>1 );
}

} // namespace fpu
} // namespace postrisc
