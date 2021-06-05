#include "util/common.hpp"
#include "float.hpp"
#include "floatx.hpp"
#include "controller.hpp"

/********************************************************************
* Alternate softfpu implementation for testing
********************************************************************/

namespace postrisc {
namespace fpu {

//////////////////////////////////////////////////////////////////////
// floatX
//////////////////////////////////////////////////////////////////////

const floatX floatX::NaN          (true,  false, false, false, 0);
const floatX floatX::PositiveZero (false, false, true,  false, 0);
const floatX floatX::NegativeZero (false, false, true,  true,  0);

floatX::floatX( i32 a )
{
    isNaN  = false;
    isInf  = false;
    isZero = a == 0;
    sign   = a < 0;
    sig    = u512( a < 0 ? -u64(a) : a, 0, 0, 0, 0, 0, 0, 0);

    if ( a == 0 ) {
        isZero = true;
        return;
    }
    isZero = false;
    sig.m0 <<= (BITS_PER_HIGH_PRECISION - 32);
    exp = 31;
    while ( sig.m0 < U64C( 0x0080000000000000 ) ) {
        sig.m0 <<= 1;
        --exp;
    }
}

floatX::floatX( i64 a )
{
    isNaN  = false;
    isInf  = false;
    isZero = a == 0;
    sign   = a < 0;
    sig    = u512(0, a < 0 ? -u64(a) : a, 0, 0, 0, 0, 0, 0);

    sig.m0 = 0;
    if ( a == 0 ) {
        isZero = true;
        return;
    }
    isZero = false;
    sig <<= BITS_PER_HIGH_PRECISION;
    exp = 63;
    while ( sig.m0 < U64C( 0x0080000000000000 ) ) {
        sig <<= 1;
        --exp;
    }
}

floatX::floatX( f32 a )
{
    isNaN = false;
    isInf = false;
    isZero = false;
    sign = a.sign();
    int expField = a.exponent();
    sig = u512( a.fraction(), 0, 0, 0, 0, 0, 0, 0);
    sig.m0 <<= (BITS_PER_HIGH_PRECISION - 24);
    if ( expField == 0 ) {
        if ( sig.m0 == 0 ) {
            isZero = true;
            //TODO
            exp = expField;
        }
        else {
            expField = 1 - a.BIAS;
            do {
                sig.m0 <<= 1;
                --expField;
            } while ( sig.m0 < U64C( 0x0080000000000000 ) );
            exp = expField;
        }
    }
    else if ( expField == a.EINF ) {
        if ( sig.m0 == 0 ) {
            isInf = true;
        }
        else {
            isNaN = true;
        }
        //TODO
        exp = expField;
    }
    else {
        sig.m0 |= U64C( 0x0080000000000000 );
        exp = expField - a.BIAS;
    }
}


floatX::floatX( f64 a )
{
    isNaN = false;
    isInf = false;
    isZero = false;

    sign = a.sign();
    int expField = a.exponent();
    sig = u512( a.fraction(), 0, 0, 0, 0, 0, 0, 0 );

    if ( expField == 0 ) {
        if ( sig.m0 == 0 ) {
            isZero = true;
            //TODO
            exp = expField;
        }
        else {
            expField = 1 - a.BIAS;
            do {
                sig.m0 <<= 1;
                --expField;
            } while ( sig.m0 < U64C( 0x0010000000000000 ) );
            exp = expField;
        }
    }
    else if ( expField == a.EINF ) {
        if ( sig.m0 == 0 ) {
            isInf = true;
        }
        else {
            isNaN = true;
        }
        //TODO
        exp = expField;
    }
    else {
        exp = expField - a.BIAS;
        sig.m0 |= U64C( 0x0010000000000000 );
    }
    sig.m0 <<= (BITS_PER_HIGH_PRECISION - 53);
}


floatX::floatX( f80 a )
{
    isNaN = false;
    isInf = false;
    isZero = false;
    sign = a.sign();
    int expField = a.exponent();
    sig = u512(0, a.fraction(), 0, 0, 0 ,0, 0, 0);

    if ( expField == 0 ) {
        if ( sig.m1 == 0 ) {
            isZero = true;
        }
        else {
            expField = 1 - a.BIAS;
            while ( sig.m1 < U64C( 0x8000000000000000 ) ) {
                sig.m1 <<= 1;
                --expField;
            }
            exp = expField;
        }
    }
    else if ( expField == a.EINF ) {
        if ( ( sig.m1 & U64C( 0x7FFFFFFFFFFFFFFF ) ) == 0 ) {
            isInf = true;
        }
        else {
            isNaN = true;
        }
    }
    else {
        exp = expField - a.BIAS;
    }
    sig <<= BITS_PER_HIGH_PRECISION;
}


floatX::floatX( f128 a )
{
    isNaN = false;
    isInf = false;
    isZero = false;
    sign = a.sign();
    int expField = a.exponent();
    u128 fraction = a.fraction();
    sig = u512(fraction.m0, fraction.m1, 0, 0, 0, 0, 0, 0);
    if ( expField == 0 ) {
        if ( ! sig ) {
            isZero = true;
        }
        else {
            expField = 1 - a.BIAS;
            do {
                sig <<= 1;
                --expField;
            } while ( sig.m0 < U64C( 0x0001000000000000 ) );
            exp = expField;
        }
    }
    else if ( expField == a.EINF ) {
        if ( sig ) {
            isNaN = true;
        }
        else {
            isInf = true;
        }
    }
    else {
        exp = expField - a.BIAS;
        sig.m0 |= a.HIDDEN_BIT;
    }
    sig <<= ( BITS_PER_HIGH_PRECISION - ( a.BITS_PER_HIGH_SIGNIFICAND + 1 ) );
}


floatX::floatX( f256 a )
{
    isNaN = false;
    isInf = false;
    isZero = false;
    sign = a.sign();
    i32 expField = a.exponent();
    sig = u512(a.fraction());
    if ( expField == 0 ) {
        if ( ! sig ) {
            isZero = true;
        }
        else {
            expField = 1 - a.BIAS;
            do {
                sig <<= 1;
                --expField;
            } while ( sig.m0 < U64C( 0x0000100000000000 ) );
            exp = expField;
        }
    }
    else if ( expField == a.EINF ) {
        if ( sig ) {
            isNaN = true;
        }
        else {
            isInf = true;
        }
    }
    else {
        exp = expField - a.BIAS;
        sig.m0 |= a.HIDDEN_BIT;
    }
    sig <<= ( BITS_PER_HIGH_PRECISION - ( a.BITS_PER_HIGH_SIGNIFICAND + 1 ) );
}


floatX::floatX( f512 a )
{
    isNaN = false;
    isInf = false;
    isZero = false;
    sign = a.sign();
    i32 expField = a.exponent();
    sig = a.fraction();
    if ( expField == 0 ) {
        if ( ! sig ) {
            isZero = true;
        }
        else {
            expField = 1 - a.BIAS;
            do {
                sig <<= 1;
                --expField;
            } while ( sig.m0 < U64C( 0x0000010000000000 ) );
            exp = expField;
        }
    }
    else if ( expField == a.EINF ) {
        if ( sig ) {
            isNaN = true;
        }
        else {
            isInf = true;
        }
    }
    else {
        exp = expField - a.BIAS;
        sig.m0 |= a.HIDDEN_BIT;
    }
    sig <<= ( BITS_PER_HIGH_PRECISION - ( a.BITS_PER_HIGH_SIGNIFICAND + 1 ) );
}

//////////////////////////////////////////////////////////////////////
// TestFPU for floatX
//////////////////////////////////////////////////////////////////////

void TestFPU::round_to_f32( bool isTiny, floatX & zx )
{
    zx.sig.m0 |= ( (zx.sig.m1 | zx.sig.m2 | zx.sig.m3 | zx.sig.m4 | zx.sig.m5 | zx.sig.m6 | zx.sig.m7) != 0 );
    zx.sig.m1 = zx.sig.m2 = zx.sig.m3 = zx.sig.m4 = zx.sig.m5 = zx.sig.m6 = zx.sig.m7 = 0;
    u32 roundBits = zx.sig.m0 & 0xFFFFFFFF;
    zx.sig.m0 -= roundBits;
    if ( roundBits ) {
        raise( FLOAT_FLAG_INEXACT);
        if ( isTiny ) raise( FLOAT_FLAG_UNDERFLOW);
        switch ( get_rounding_mode() ) {
         default:
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( roundBits < 0x80000000 ) goto noIncrement;
            if (    ( roundBits == 0x80000000 )
                 && ! ( zx.sig.m0 & U64C( 0x100000000 ) ) ) {
                goto noIncrement;
            }
            break;
         case FLOAT_ROUND_TOWARDZERO:
            goto noIncrement;
         case FLOAT_ROUND_DOWNWARD:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case FLOAT_ROUND_UPWARD:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        zx.sig.m0 += U64C( 0x100000000 );
        if ( zx.sig.m0 == U64C( 0x0100000000000000 ) ) {
            zx.sig.m0 = U64C( 0x0080000000000000 );
            ++zx.exp;
        }
    }
 noIncrement:
    return;
}


void TestFPU::round_to_f64( bool isTiny, floatX & zx )
{
    zx.sig.m0 |= ( (zx.sig.m1 | zx.sig.m2 | zx.sig.m3 | zx.sig.m4 | zx.sig.m5 | zx.sig.m6 | zx.sig.m7) != 0 );
    zx.sig.m1 = zx.sig.m2 = zx.sig.m3 = zx.sig.m4 = zx.sig.m5 = zx.sig.m6 = zx.sig.m7 = 0;
    int roundBits = zx.sig.m0 & 7;
    zx.sig.m0 -= roundBits;
    if ( roundBits ) {
        raise( FLOAT_FLAG_INEXACT);
        if ( isTiny ) raise( FLOAT_FLAG_UNDERFLOW);
        switch ( get_rounding_mode() ) {
         default:
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( roundBits < 4 ) goto noIncrement;
            if ( ( roundBits == 4 ) && ! ( zx.sig.m0 & 8 ) ) goto noIncrement;
            break;
         case FLOAT_ROUND_TOWARDZERO:
            goto noIncrement;
         case FLOAT_ROUND_DOWNWARD:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case FLOAT_ROUND_UPWARD:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        zx.sig.m0 += 8;
        if ( zx.sig.m0 == U64C( 0x0100000000000000 ) ) {
            zx.sig.m0 = U64C( 0x0080000000000000 );
            ++zx.exp;
        }
    }
 noIncrement:
    return;
}


void TestFPU::round_to_f80( bool isTiny, floatX & zx )
{
    zx.sig.m1 |= ( (zx.sig.m2 | zx.sig.m3 | zx.sig.m4 | zx.sig.m5 | zx.sig.m6 | zx.sig.m7) != 0 );
    zx.sig.m2 = zx.sig.m3 = zx.sig.m4 = zx.sig.m5 = zx.sig.m6 = zx.sig.m7 = 0;
    u64 roundBits = zx.sig.m1 & U64C( 0x00FFFFFFFFFFFFFF );
    zx.sig.m1 -= roundBits;
    if ( roundBits ) {
        raise( FLOAT_FLAG_INEXACT);
        if ( isTiny ) raise( FLOAT_FLAG_UNDERFLOW);
        switch ( get_rounding_mode() ) {
         default:
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( roundBits < U64C( 0x0080000000000000 ) ) goto noIncrement;
            if (    ( roundBits == U64C( 0x0080000000000000 ) )
                 && ! ( zx.sig.m1 & U64C( 0x0100000000000000 ) ) ) {
                goto noIncrement;
            }
            break;
         case FLOAT_ROUND_TOWARDZERO:
            goto noIncrement;
         case FLOAT_ROUND_DOWNWARD:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case FLOAT_ROUND_UPWARD:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        zx.sig.m1 += U64C( 0x0100000000000000 );
        zx.sig.m0 += ( zx.sig.m1 == 0 );
        if ( zx.sig.m0 == U64C( 0x0100000000000000 ) ) {
            zx.sig.m0 = U64C( 0x0080000000000000 );
            ++zx.exp;
        }
    }
 noIncrement:
    return;
}


void TestFPU::round_to_f128( bool isTiny, floatX & zx )
{
    zx.sig.m1 |= ( (zx.sig.m2 | zx.sig.m3 | zx.sig.m4 | zx.sig.m5 | zx.sig.m6 | zx.sig.m7) != 0 );
    zx.sig.m2 = zx.sig.m3 = zx.sig.m4 = zx.sig.m5 = zx.sig.m6 = zx.sig.m7 = 0;
    int roundBits = zx.sig.m1 & 0x7F;
    zx.sig.m1 -= roundBits;
    if ( roundBits ) {
        raise( FLOAT_FLAG_INEXACT);
        if ( isTiny ) raise( FLOAT_FLAG_UNDERFLOW);
        switch ( get_rounding_mode() ) {
         default:
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( roundBits < 0x40 ) goto noIncrement;
            if (    ( roundBits == 0x40 )
                 && ! ( zx.sig.m1 & 0x80 ) ) goto noIncrement;
            break;
         case FLOAT_ROUND_TOWARDZERO:
            goto noIncrement;
         case FLOAT_ROUND_DOWNWARD:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case FLOAT_ROUND_UPWARD:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        zx.sig.m1 += 0x80;
        zx.sig.m0 += ( zx.sig.m1 == 0 );
        if ( zx.sig.m0 == U64C( 0x0100000000000000 ) ) {
            zx.sig.m0 = U64C( 0x0080000000000000 );
            ++zx.exp;
        }
    }
 noIncrement:
    return;
}


void TestFPU::round_to_f256( bool isTiny, floatX & zx )
{
    zx.sig.m3 |= ( (zx.sig.m4 | zx.sig.m5 | zx.sig.m6 | zx.sig.m7) != 0 );
    zx.sig.m4 = zx.sig.m5 = zx.sig.m6 = zx.sig.m7 = 0;
    int roundBits = zx.sig.m3 & 0x7FF;
    zx.sig.m3 -= roundBits;
    if ( roundBits ) {
        raise( FLOAT_FLAG_INEXACT);
        if ( isTiny ) raise( FLOAT_FLAG_UNDERFLOW);
        switch ( get_rounding_mode() ) {
         default:
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( roundBits < 0x400 ) goto noIncrement;
            if (    ( roundBits == 0x400 )
                 && ! ( zx.sig.m3 & 0x800 ) ) goto noIncrement;
            break;
         case FLOAT_ROUND_TOWARDZERO:
            goto noIncrement;
         case FLOAT_ROUND_DOWNWARD:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case FLOAT_ROUND_UPWARD:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        zx.sig.add( u512(0, 0, 0, 0x800, 0, 0, 0, 0) );
        if ( zx.sig.m0 == U64C( 0x0100000000000000 ) ) {
            zx.sig.m0 = U64C( 0x0080000000000000 );
            ++zx.exp;
        }
    }
 noIncrement:
    return;
}


void TestFPU::round_to_f512( bool isTiny, floatX & zx )
{
    int roundBits = zx.sig.m7 & 0x7FFF;
    zx.sig.m7 -= roundBits;
    if ( roundBits ) {
        raise( FLOAT_FLAG_INEXACT);
        if ( isTiny ) raise( FLOAT_FLAG_UNDERFLOW);
        switch ( get_rounding_mode() ) {
         default:
         case FLOAT_ROUND_TONEAREST_EVEN:
            if ( roundBits < 0x4000 ) goto noIncrement;
            if (    ( roundBits == 0x4000 )
                 && ! ( zx.sig.m7 & 0x8000 ) ) goto noIncrement;
            break;
         case FLOAT_ROUND_TOWARDZERO:
            goto noIncrement;
         case FLOAT_ROUND_DOWNWARD:
            if ( ! zx.sign ) goto noIncrement;
            break;
         case FLOAT_ROUND_UPWARD:
            if ( zx.sign ) goto noIncrement;
            break;
        }
        zx.sig.add( u512(0, 0, 0, 0, 0, 0, 0, 0x8000) );
        if ( zx.sig.m0 == U64C( 0x0100000000000000 ) ) {
            zx.sig.m0 = U64C( 0x0080000000000000 );
            ++zx.exp;
        }
    }
 noIncrement:
    return;
}

i32 TestFPU::to_i32( floatX ax )
{
    if ( ax.isInf || ax.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
        return ( ax.isInf & ax.sign ) ? std::numeric_limits<i32>::min() : std::numeric_limits<i32>::max();
    }
    if ( ax.isZero ) return 0;
    i32 shiftCount = 52 - ax.exp;
    if ( 56 < shiftCount ) {
        ax.sig = u512(0, 0, 0, 0, 0, 0, 0, 1);
    }
    else {
        while ( 0 < shiftCount ) {
            ax.sig.jamming( 1 );
            --shiftCount;
        }
    }
    round_to_f64( false, ax );
    ax.sig.jamming( 3 );
    i32 z = ax.sig.m0;
    if ( ax.sign ) z = - z;
    if (    ( shiftCount < 0 )
         || ( ax.sig.m0>>32 )
         || ( ( z != 0 ) && ( ( ax.sign ^ ( z < 0 ) ) != 0 ) )
       ) {
        raise(FLOAT_FLAG_INVALID);
        clear_flags(FLOAT_FLAG_INEXACT);
        return ax.sign ? std::numeric_limits<i32>::min() : std::numeric_limits<i32>::max();
    }
    return z;
}


i64 TestFPU::to_i64( floatX ax )
{
    if ( ax.isInf || ax.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
        return ( ax.isInf & ax.sign ) ? std::numeric_limits<i64>::min() : std::numeric_limits<i64>::max();
    }
    if ( ax.isZero ) return 0;
    int shiftCount = 112 - ax.exp;
    if ( 116 < shiftCount ) {
        ax.sig = u512(0, 0, 0, 0, 0, 0, 0, 1);
    }
    else {
        while ( 0 < shiftCount ) {
            ax.sig.jamming( 1 );
            --shiftCount;
        }
    }
    round_to_f128( false, ax );
    ax.sig.jamming( 7 );
    i64 z = ax.sig.m1;
    if ( ax.sign ) z = - z;
    if (    ( shiftCount < 0 )
         || ax.sig.m0
         || ( ( z != 0 ) && ( ( ax.sign ^ ( z < 0 ) ) != 0 ) )
       ) {
        raise(FLOAT_FLAG_INVALID);
        clear_flags(FLOAT_FLAG_INEXACT);
        return ax.sign ? std::numeric_limits<i64>::min() : std::numeric_limits<i64>::max();
    }
    return z;
}


f32 TestFPU::to_f32( floatX zx )
{
    u32 z;

    if ( zx.isZero ) return f32::make(zx.sign ? 0x80000000 : 0);
    if ( zx.isInf ) return f32::make(zx.sign ? 0xFF800000 : 0x7F800000);
    if ( zx.isNaN ) return f32::make(0xFFFFFFFF);
    while ( U64C( 0x0100000000000000 ) <= zx.sig.m0 ) {
        zx.sig.jamming( 1 );
        ++zx.exp;
    }
    while ( zx.sig.m0 < U64C( 0x0080000000000000 ) ) {
        zx.sig <<= 1;
        --zx.exp;
    }
    floatX savedZ = zx;
    bool isTiny = tininess_before_rounding() && ( zx.exp + 0x7F <= 0 );
    round_to_f32( isTiny, zx );
    int expField = zx.exp + 0x7F;
    if ( 0xFF <= expField ) {
        raise( FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
        if ( zx.sign ) {
            switch ( get_rounding_mode() ) {
               default:
               case FLOAT_ROUND_TONEAREST_EVEN:
               case FLOAT_ROUND_DOWNWARD:
                  z = 0xFF800000;
                  break;
               case FLOAT_ROUND_TOWARDZERO:
               case FLOAT_ROUND_UPWARD:
                  z = 0xFF7FFFFF;
                  break;
            }
        }
        else {
            switch ( get_rounding_mode() ) {
               default:
               case FLOAT_ROUND_TONEAREST_EVEN:
               case FLOAT_ROUND_UPWARD:
                   z = 0x7F800000;
                   break;
               case FLOAT_ROUND_TOWARDZERO:
               case FLOAT_ROUND_DOWNWARD:
                   z = 0x7F7FFFFF;
                   break;
            }
        }
        return f32::make(z);
    }
    if ( expField <= 0 ) {
        isTiny = true;
        zx = savedZ;
        expField = zx.exp + 0x7F;
        if ( expField < -27 ) {
            zx.sig.m1 = ( zx.sig.m0 != 0 ) || ( zx.sig.m1 != 0 );
            zx.sig.m0 = 0;
        }
        else {
            while ( expField <= 0 ) {
                zx.sig.jamming( 1 );
                ++expField;
            }
        }
        round_to_f32( isTiny, zx );
        expField = ( U64C( 0x0080000000000000 ) <= zx.sig.m0 ) ? 1 : 0;
    }
    z = expField;
    z <<= f32::BITS_PER_SIGNIFICAND;
    if ( zx.sign ) z |= 0x80000000;
    z |= lobits( zx.sig.m0 >> 32, f32::BITS_PER_SIGNIFICAND);
    return f32::make(z);
}


f64 TestFPU::to_f64( floatX zx )
{
    u64 z;

    if ( zx.isZero ) return f64::make(zx.sign ? U64C( 0x8000000000000000 ) : 0);
    if ( zx.isInf ) {
        return f64::make(
              zx.sign ? U64C( 0xFFF0000000000000 )
                      : U64C( 0x7FF0000000000000 ));
    }
    if ( zx.isNaN ) return f64::make(U64C( 0xFFFFFFFFFFFFFFFF ));
    while ( U64C( 0x0100000000000000 ) <= zx.sig.m0 ) {
        zx.sig.jamming( 1 );
        ++zx.exp;
    }
    while ( zx.sig.m0 < U64C( 0x0080000000000000 ) ) {
        zx.sig <<= 1;
        --zx.exp;
    }
    floatX savedZ = zx;
    bool isTiny = tininess_before_rounding() && ( zx.exp + 0x3FF <= 0 );
    round_to_f64( isTiny, zx );
    int expField = zx.exp + 0x3FF;
    if ( 0x7FF <= expField ) {
        raise(FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
        if ( zx.sign ) {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_DOWNWARD:
                z = U64C( 0xFFF0000000000000 );
                break;
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_UPWARD:
                z = U64C( 0xFFEFFFFFFFFFFFFF );
                break;
            }
        }
        else {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_UPWARD:
                z = U64C( 0x7FF0000000000000 );
                break;
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_DOWNWARD:
                z = U64C( 0x7FEFFFFFFFFFFFFF );
                break;
            }
        }
        return f64::make(z);
    }
    if ( expField <= 0 ) {
        isTiny = true;
        zx = savedZ;
        expField = zx.exp + 0x3FF;
        if ( expField < -56 ) {
            zx.sig.m1 = ( zx.sig.m0 != 0 ) || ( zx.sig.m1 != 0 );
            zx.sig.m0 = 0;
        }
        else {
            while ( expField <= 0 ) {
                zx.sig.jamming( 1 );
                ++expField;
            }
        }
        round_to_f64( isTiny, zx );
        expField = ( U64C( 0x0080000000000000 ) <= zx.sig.m0 ) ? 1 : 0;
    }
    zx.sig.m0 >>= 3;
    z = expField;
    z <<= f64::BITS_PER_SIGNIFICAND;
    if ( zx.sign ) z |= U64C( 0x8000000000000000 );
    z |= zx.sig.m0 & U64C( 0x000FFFFFFFFFFFFF );
    return f64::make(z);
}


f80 TestFPU::to_f80( floatX zx )
{
    f80 z;

    if ( zx.isZero ) {
        return f80::make( zx.sign ? 0x8000 : 0, 0 );
    }
    if ( zx.isInf ) {
        return f80::make( zx.sign ? 0xFFFF : 0x7FFF, f80::UNHIDDEN_BIT );
    }
    if ( zx.isNaN ) {
        return f80::make( 0xFFFF, U64C( 0xFFFFFFFFFFFFFFFF ) );
    }

    while ( U64C( 0x0100000000000000 ) <= zx.sig.m0 ) {
        zx.sig.jamming( 1 );
        ++zx.exp;
    }
    while ( zx.sig.m0 < U64C( 0x0080000000000000 ) ) {
        zx.sig <<= 1;
        --zx.exp;
    }

    floatX savedZ = zx;
    bool isTiny = tininess_before_rounding() && ( zx.exp + 0x3FFF <= 0 );
    switch ( get_rounding_precision() ) {
        case FLOAT_ROUND_PRECISION_32:   round_to_f32( isTiny, zx );   break;
        case FLOAT_ROUND_PRECISION_64:   round_to_f64( isTiny, zx );   break;
        default:                         round_to_f80( isTiny, zx );   break;
    }

    int expField = zx.exp + 0x3FFF;
    if ( 0x7FFF <= expField ) {
        raise( FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
        if ( zx.sign ) {
            switch ( get_rounding_mode() ) {
               default:
               case FLOAT_ROUND_TONEAREST_EVEN:
               case FLOAT_ROUND_DOWNWARD:
                   z.low = z.UNHIDDEN_BIT;
                   z.high = 0xFFFF;
                   break;
               case FLOAT_ROUND_TOWARDZERO:
               case FLOAT_ROUND_UPWARD:
                   switch ( get_rounding_precision() ) {
                       case FLOAT_ROUND_PRECISION_32:   z.low = U64C( 0xFFFFFF0000000000 );   break;
                       case FLOAT_ROUND_PRECISION_64:   z.low = U64C( 0xFFFFFFFFFFFFF800 );   break;
                       default:                         z.low = U64C( 0xFFFFFFFFFFFFFFFF );   break;
                   }
                   z.high = 0xFFFE;
                   break;
            }
        }
        else {
            switch ( get_rounding_mode() ) {
              default:
              case FLOAT_ROUND_TONEAREST_EVEN:
              case FLOAT_ROUND_UPWARD:
                   z.low = z.UNHIDDEN_BIT;
                   z.high = 0x7FFF;
                   break;
              case FLOAT_ROUND_TOWARDZERO:
              case FLOAT_ROUND_DOWNWARD:
                   switch ( get_rounding_precision() ) {
                       case FLOAT_ROUND_PRECISION_32:   z.low = U64C( 0xFFFFFF0000000000 );   break;
                       case FLOAT_ROUND_PRECISION_64:   z.low = U64C( 0xFFFFFFFFFFFFF800 );   break;
                       default:                         z.low = U64C( 0xFFFFFFFFFFFFFFFF );   break;
                   }
                   z.high = 0x7FFE;
                   break;
            }
        }
        return z;
    }

    if ( expField <= 0 ) {
        isTiny = true;
        zx = savedZ;
        expField = zx.exp + 0x3FFF;
        if ( expField < -70 ) {
            zx.sig.m1 = ( zx.sig.m0 != 0 ) || ( zx.sig.m1 != 0 );
            zx.sig.m0 = 0;
        }
        else {
            while ( expField <= 0 ) {
                zx.sig.jamming( 1 );
                ++expField;
            }
        }
        switch ( get_rounding_precision() ) {
            case FLOAT_ROUND_PRECISION_32:   round_to_f32( isTiny, zx );    break;
            case FLOAT_ROUND_PRECISION_64:   round_to_f64( isTiny, zx );    break;
            default:                         round_to_f80( isTiny, zx );    break;
        }
        expField = ( U64C( 0x0080000000000000 ) <= zx.sig.m0 ) ? 1 : 0;
    }
    zx.sig.jamming( 56 );
    z.low = zx.sig.m1;
    z.high = expField;
    if ( zx.sign ) z.high |= 0x8000;
    return z;
}


f128 TestFPU::to_f128( floatX zx )
{
    f128 z;

    if ( zx.isZero ) {
        return f128::make( zx.sign ? U64C( 0x8000000000000000 ) : 0, 0 );
    }
    if ( zx.isInf ) {
        return f128::make( zx.sign ? U64C( 0xFFFF000000000000 ) : U64C( 0x7FFF000000000000 ), 0 );
    }
    if ( zx.isNaN ) {
        return f128::make( U64C( 0xFFFFFFFFFFFFFFFF ),
                         U64C( 0xFFFFFFFFFFFFFFFF ) );
    }
    while ( U64C( 0x0100000000000000 ) <= zx.sig.m0 ) {
        zx.sig.jamming( 1 );
        ++zx.exp;
    }
    while ( zx.sig.m0 < U64C( 0x0080000000000000 ) ) {
        zx.sig <<= 1;
        --zx.exp;
    }
    floatX savedZ = zx;
    bool isTiny = tininess_before_rounding() && ( zx.exp + 0x3FFF <= 0 );
    round_to_f128( isTiny, zx );
    int expField = zx.exp + 0x3FFF;
    if ( 0x7FFF <= expField ) {
        raise( FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
        if ( zx.sign ) {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_DOWNWARD:
                return f128::make( U64C( 0xFFFF000000000000 ), 0 );
                break;
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_UPWARD:
                return f128::make( U64C( 0xFFFEFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ) );
            }
        }
        else {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_UPWARD:
                return f128::make( U64C( 0x7FFF000000000000 ), 0 );
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_DOWNWARD:
                return f128::make( U64C( 0x7FFEFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ) );
            }
        }
        //return z;
    }
    if ( expField <= 0 ) {
        isTiny = true;
        zx = savedZ;
        expField = zx.exp + 0x3FFF;
        if ( expField < -120 ) {
            zx.sig.m1 = ( zx.sig.m0 != 0 ) || ( zx.sig.m1 != 0 );
            zx.sig.m0 = 0;
        }
        else {
            while ( expField <= 0 ) {
                zx.sig.jamming( 1 );
                ++expField;
            }
        }
        round_to_f128( isTiny, zx );
        expField = ( U64C( 0x0080000000000000 ) <= zx.sig.m0 ) ? 1 : 0;
    }
    zx.sig.jamming( 15 - 8 );
    z.low = zx.sig.m1;
    z.high = expField;
    z.high <<= f128::BITS_PER_HIGH_SIGNIFICAND;
    if ( zx.sign ) z.high |= U64C( 0x8000000000000000 );
    z.high |= lobits(zx.sig.m0, f128::BITS_PER_HIGH_SIGNIFICAND);
    return z;
}


f256 TestFPU::to_f256( floatX zx )
{
    f256 z;

    if ( zx.isZero ) {
        return f256(zx.sign ? U64C( 0x8000000000000000 ) : 0, 0, 0, 0);
    }
    if ( zx.isInf ) {
        return f256( zx.sign ? U64C( 0xFFFFF00000000000 ) : U64C( 0x7FFFF00000000000 ), 0, 0, 0 );
    }
    if ( zx.isNaN ) {
        return f256( U64C( 0xFFFFFFFFFFFFFFFF ), 0, 0, 0 );
    }
    while ( U64C( 0x0100000000000000 ) <= zx.sig.m0 ) {
        zx.sig.jamming( 1 );
        ++zx.exp;
    }
    while ( zx.sig.m0 < U64C( 0x0080000000000000 ) ) {
        zx.sig <<= 1;
        --zx.exp;
    }
    floatX savedZ = zx;
    bool isTiny = tininess_before_rounding() && ( zx.exp + 0x3FFFF <= 0 );
    round_to_f256( isTiny, zx );
    int expField = zx.exp + 0x3FFFF;
    if ( 0x7FFFF <= expField ) {
        raise( FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
        if ( zx.sign ) {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_DOWNWARD:
                return f256( U64C( 0xFFFFF00000000000 ), 0, 0, 0 );
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_UPWARD:
                return f256( U64C( 0xFFFFEFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ) );
            }
        }
        else {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_UPWARD:
                return f256( U64C( 0x7FFFF00000000000 ), 0, 0, 0 );
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_DOWNWARD:
                return f256( U64C( 0x7FFFEFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ) );
            }
        }
        //return z;
    }
    if ( expField <= 0 ) {
        isTiny = true;
        zx = savedZ;
        expField = zx.exp + 0x3FFFF;
        if ( expField < -floatX::BITS_PER_PRECISION ) {
            zx.sig.m3 = ( zx.sig.m0 != 0 ) || ( zx.sig.m1 != 0 ) || ( zx.sig.m2 != 0 ) || ( zx.sig.m3 != 0 );
            zx.sig.m2 = 0;
            zx.sig.m1 = 0;
            zx.sig.m0 = 0;
        }
        else {
            while ( expField <= 0 ) {
                zx.sig.jamming( 1 );
                ++expField;
            }
        }
        round_to_f256( isTiny, zx );
        expField = ( U64C( 0x0080000000000000 ) <= zx.sig.m0 ) ? 1 : 0;
    }
    zx.sig.jamming( 19 - 8 );

    z.m3 = zx.sig.m3;
    z.m2 = zx.sig.m2;
    z.m1 = zx.sig.m1;
    z.m0 = u64(expField) << f256::BITS_PER_HIGH_SIGNIFICAND;
    if ( zx.sign ) z.m0 |= U64C( 0x8000000000000000 );
    z.m0 |= lobits(zx.sig.m0, f256::BITS_PER_HIGH_SIGNIFICAND);
    return z;
}


f512 TestFPU::to_f512( floatX zx )
{
    f512 z;

    if ( zx.isZero ) {
        return f512(zx.sign ? U64C( 0x8000000000000000 ) : 0, 0, 0, 0, 0, 0, 0, 0);
    }
    if ( zx.isInf ) {
        return f512( zx.sign ? U64C( 0xFFFFFF0000000000 ) : U64C( 0x7FFFFF0000000000 ), 0, 0, 0, 0, 0, 0, 0 );
    }
    if ( zx.isNaN ) {
        return f512( U64C( 0xFFFFFFFFFFFFFFFF ), 0, 0, 0, 0, 0, 0, 0 );
    }
    while ( U64C( 0x0100000000000000 ) <= zx.sig.m0 ) {
        zx.sig.jamming( 1 );
        ++zx.exp;
    }
    while ( zx.sig.m0 < U64C( 0x0080000000000000 ) ) {
        zx.sig <<= 1;
        --zx.exp;
    }
    floatX savedZ = zx;
    bool isTiny = tininess_before_rounding() && ( zx.exp + 0x3FFFFF <= 0 );
    round_to_f512( isTiny, zx );
    int expField = zx.exp + 0x3FFFFF;
    if ( 0x7FFFFF <= expField ) {
        raise( FLOAT_FLAG_OVERFLOW | FLOAT_FLAG_INEXACT);
        if ( zx.sign ) {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_DOWNWARD:
                return f512( U64C( 0xFFFFFF0000000000 ), 0, 0, 0, 0, 0, 0, 0 );
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_UPWARD:
                return f512( U64C( 0xFFFFFEFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ) );
            }
        }
        else {
            switch ( get_rounding_mode() ) {
             default:
             case FLOAT_ROUND_TONEAREST_EVEN:
             case FLOAT_ROUND_UPWARD:
                return f512( U64C( 0x7FFFFF0000000000 ), 0, 0, 0, 0, 0, 0, 0 );
             case FLOAT_ROUND_TOWARDZERO:
             case FLOAT_ROUND_DOWNWARD:
                return f512( U64C( 0x7FFFFEFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ),
                                 U64C( 0xFFFFFFFFFFFFFFFF ) );
            }
        }
        //return z;
    }
    if ( expField <= 0 ) {
        isTiny = true;
        zx = savedZ;
        expField = zx.exp + 0x3FFFFF;
        if ( expField < -floatX::BITS_PER_PRECISION ) {
            zx.sig.m7 = ( ( zx.sig.m0 | zx.sig.m1 | zx.sig.m2 | zx.sig.m3 | zx.sig.m4 | zx.sig.m5 | zx.sig.m6 | zx.sig.m7 ) != 0 );
            zx.sig.m6 = 0;
            zx.sig.m5 = 0;
            zx.sig.m4 = 0;
            zx.sig.m3 = 0;
            zx.sig.m2 = 0;
            zx.sig.m1 = 0;
            zx.sig.m0 = 0;
        }
        else {
            while ( expField <= 0 ) {
                zx.sig.jamming( 1 );
                ++expField;
            }
        }
        round_to_f512( isTiny, zx );
        expField = ( U64C( 0x0080000000000000 ) <= zx.sig.m0 ) ? 1 : 0;
    }
    zx.sig.jamming( 23 - 8 );

    z.m7 = zx.sig.m7;
    z.m6 = zx.sig.m6;
    z.m5 = zx.sig.m5;
    z.m4 = zx.sig.m4;
    z.m3 = zx.sig.m3;
    z.m2 = zx.sig.m2;
    z.m1 = zx.sig.m1;
    z.m0 = u64(expField) << f512::BITS_PER_HIGH_SIGNIFICAND;
    if ( zx.sign ) z.m0 |= U64C( 0x8000000000000000 );
    z.m0 |= lobits(zx.sig.m0, f512::BITS_PER_HIGH_SIGNIFICAND);
    return z;
}


floatX TestFPU::floatXInvalid( void )
{
    raise( FLOAT_FLAG_INVALID);
    return floatX::NaN;
}


floatX TestFPU::round_to_int( floatX ax )
{
    int shiftCount;
    const int count = 512 - 24;

    if ( ax.isNaN || ax.isInf ) return ax;
    shiftCount = count - ax.exp;
    if ( shiftCount <= 0 ) return ax;
    if ( floatX::BITS_PER_PRECISION - 1 < shiftCount ) {
        ax.exp = count;
        ax.sig = u512(0, 0, 0, 0, 0, 0, 0, ! ax.isZero);
    }
    else {
        while ( 0 < shiftCount ) {
            ax.sig.jamming( 1 );
            ++ax.exp;
            --shiftCount;
        }
    }
    round_to_f512( false, ax );
    if ( ! ax.sig ) ax.isZero = true;
    return ax;

}


floatX TestFPU::add( floatX ax, floatX bx )
{
    if ( ax.isNaN ) return ax;
    if ( bx.isNaN ) return bx;
    if ( ax.isInf && bx.isInf ) {
        if ( ax.sign == bx.sign ) return ax;
        return floatXInvalid();
    }
    if ( ax.isInf ) return ax;
    if ( bx.isInf ) return bx;
    if ( ax.isZero && bx.isZero ) {
        if ( ax.sign == bx.sign ) return ax;
        goto completeCancellation;
    }
    if (    ( ax.sign != bx.sign ) && ( ax.exp == bx.exp ) && ( ax.sig == bx.sig ) ) {
 completeCancellation:
        return ( get_rounding_mode() == FLOAT_ROUND_DOWNWARD ) ? floatX::NegativeZero : floatX::PositiveZero;
    }
    if ( ax.isZero ) return bx;
    if ( bx.isZero ) return ax;
    int expDiff = ax.exp - bx.exp;

    floatX zx;
    if ( expDiff < 0 ) {
        zx = ax;
        zx.exp = bx.exp;
        if ( expDiff < -floatX::BITS_PER_PRECISION ) {
            zx.sig = u512(0, 0, 0, 0, 0, 0, 0, 1);
        }
        else {
            while ( expDiff < 0 ) {
                zx.sig.jamming( 1 );
                ++expDiff;
            }
        }
        if ( ax.sign != bx.sign ) zx.sig.negate();
        zx.sign = bx.sign;
        zx.sig.add(bx.sig);
    }
    else {
        zx = bx;
        zx.exp = ax.exp;
        if ( floatX::BITS_PER_PRECISION < expDiff ) {
            zx.sig = u512(0, 0, 0, 0, 0, 0, 0, 1);
        }
        else {
            while ( 0 < expDiff ) {
                zx.sig.jamming( 1 );
                --expDiff;
            }
        }
        if ( ax.sign != bx.sign ) zx.sig.negate();
        zx.sign = ax.sign;
        zx.sig.add(ax.sig);
    }
    if ( zx.sig.m0 & U64C( 0x8000000000000000 ) ) {
        zx.sig.negate();
        zx.sign = ! zx.sign;
    }
    return zx;
}


floatX TestFPU::mul( floatX ax, floatX bx )
{
    if ( ax.isNaN ) return ax;
    if ( bx.isNaN ) return bx;
    if ( ax.isInf ) {
        if ( bx.isZero ) return floatXInvalid();
        if ( bx.sign ) ax.sign = ! ax.sign;
        return ax;
    }
    if ( bx.isInf ) {
        if ( ax.isZero ) return floatXInvalid();
        if ( ax.sign ) bx.sign = ! bx.sign;
        return bx;
    }
    floatX zx = ax;
    zx.sign ^= bx.sign;
    if ( ax.isZero || bx.isZero ) {
        return zx.sign ? floatX::NegativeZero : floatX::PositiveZero;
    }
    zx.exp += bx.exp + 1;
    zx.sig.clear();
    for (int bitNum = 0; bitNum < floatX::BITS_PER_PRECISION - 1; ++bitNum ) {
        if ( bx.sig.m7 & 2 )
           zx.sig.add(ax.sig);
        bx.sig.jamming( 1 );
        zx.sig.jamming( 1 );
    }
    return zx;
}


floatX TestFPU::div( floatX ax, floatX bx )
{
    if ( ax.isNaN ) return ax;
    if ( bx.isNaN ) return bx;
    if ( ax.isInf ) {
        if ( bx.isInf ) return floatXInvalid();
        if ( bx.sign ) ax.sign = ! ax.sign;
        return ax;
    }
    if ( bx.isZero ) {
        if ( ax.isZero ) return floatXInvalid();
        raise( FLOAT_FLAG_DIVBYZERO);
        if ( ax.sign ) bx.sign = ! bx.sign;
        bx.isZero = false;
        bx.isInf = true;
        return bx;
    }
    floatX zx = ax;
    zx.sign ^= bx.sign;
    if ( ax.isZero || bx.isInf ) {
        return zx.sign ? floatX::NegativeZero : floatX::PositiveZero;
    }
    zx.exp -= bx.exp + 1;
    zx.sig.clear();
    u512 negBSig = bx.sig;
    negBSig.negate();
    for (int bitNum = 0; bitNum < floatX::BITS_PER_PRECISION; ++bitNum ) {
        if ( bx.sig <= ax.sig ) {
            zx.sig.m7 |= 1;
            ax.sig.add(negBSig);
        }
        ax.sig <<= 1;
        zx.sig <<= 1;
    }
    if ( ax.sig ) zx.sig.m7 |= 1;
    return zx;
}


floatX TestFPU::rem( floatX ax, floatX bx )
{
    if ( ax.isNaN ) return ax;
    if ( bx.isNaN ) return bx;
    if ( ax.isInf || bx.isZero ) return floatXInvalid();
    if ( ax.isZero || bx.isInf ) return ax;
    --bx.exp;
    if ( ax.exp < bx.exp ) return ax;
    bx.sig <<= 1;
    u512 negBSig = bx.sig;
    negBSig.negate();
    while ( bx.exp < ax.exp ) {
        if ( bx.sig <= ax.sig ) ax.sig.add(negBSig);
        ax.sig <<= 1;
        --ax.exp;
    }
    bool lastQuotientBit = ( bx.sig <= ax.sig );
    if ( lastQuotientBit ) ax.sig.add(negBSig);
    u512 savedASig = ax.sig;
    ax.sig.add(negBSig);
    ax.sig.negate();
    if ( ax.sig < savedASig ) {
        ax.sign = ! ax.sign;
    }
    else if ( savedASig < ax.sig ) {
        ax.sig = savedASig;
    }
    else {
        if ( lastQuotientBit ) {
            ax.sign = ! ax.sign;
        }
        else {
            ax.sig = savedASig;
        }
    }
    if ( ! ax.sig ) ax.isZero = true;
    return ax;
}


floatX TestFPU::sqrt( floatX ax )
{
    if ( ax.isNaN || ax.isZero ) return ax;
    if ( ax.sign ) return floatXInvalid();
    if ( ax.isInf ) return ax;
    floatX zx = ax;
    zx.exp >>= 1;
    if ( ( ax.exp & 1 ) == 0 ) ax.sig.jamming( 1 );
    zx.sig.clear();
    u512 bitSig = u512( U64C( 0x0080000000000000 ), 0, 0, 0, 0, 0, 0, 0);
    for (int bitNum = 0; bitNum < floatX::BITS_PER_PRECISION; ++bitNum ) {
        u512 savedASig = ax.sig;
        ax.sig.sub(zx.sig);
        ax.sig <<= 1;
        ax.sig.sub(bitSig);
        if ( ax.sig.m0 & U64C( 0x8000000000000000 ) ) {
            ax.sig = savedASig << 1;
        }
        else {
            zx.sig |= bitSig;
        }
        bitSig.jamming( 1 );
    }
    if ( ax.sig ) zx.sig.m7 |= 1;
    return zx;
}


bool TestFPU::eq( floatX ax, floatX bx )
{
    if ( ax.isNaN || bx.isNaN ) return false;
    if ( ax.isZero && bx.isZero ) return true;
    if ( ax.sign != bx.sign ) return false;
    if ( ax.isInf || bx.isInf ) return ax.isInf && bx.isInf;
    return ( ax.exp == bx.exp ) && ( ax.sig == bx.sig );
}


bool TestFPU::le( floatX ax, floatX bx )
{
    if ( ax.isNaN || bx.isNaN ) return false;
    if ( ax.isZero && bx.isZero ) return true;
    if ( ax.sign != bx.sign ) return ax.sign;
    if ( ax.sign ) {
        if ( ax.isInf || bx.isZero ) return true;
        if ( bx.isInf || ax.isZero ) return false;
        if ( bx.exp < ax.exp ) return true;
        if ( ax.exp < bx.exp ) return false;
        return ( bx.sig <= ax.sig );
    }
    else {
        if ( bx.isInf || ax.isZero ) return true;
        if ( ax.isInf || bx.isZero ) return false;
        if ( ax.exp < bx.exp ) return true;
        if ( bx.exp < ax.exp ) return false;
        return ( ax.sig <= bx.sig );
    }
}


bool TestFPU::lt( floatX ax, floatX bx )
{
    if ( ax.isNaN || bx.isNaN ) return false;
    if ( ax.isZero && bx.isZero ) return false;
    if ( ax.sign != bx.sign ) return ax.sign;
    if ( ax.isInf && bx.isInf ) return false;
    if ( ax.sign ) {
        if ( ax.isInf || bx.isZero ) return true;
        if ( bx.isInf || ax.isZero ) return false;
        if ( bx.exp < ax.exp ) return true;
        if ( ax.exp < bx.exp ) return false;
        return ( bx.sig < ax.sig );
    }
    else {
        if ( bx.isInf || ax.isZero ) return true;
        if ( ax.isInf || bx.isZero ) return false;
        if ( ax.exp < bx.exp ) return true;
        if ( bx.exp < ax.exp ) return false;
        return ( ax.sig < bx.sig );
    }
}


i32 TestFPU::to_i32_round_to_zero( f32 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode(FLOAT_ROUND_TOWARDZERO);
    i32 z = to_i32( floatX(a) );
    set_rounding_mode(savedRoundingMode);
    return z;
}

i64 TestFPU::to_i64_round_to_zero( f32 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode(FLOAT_ROUND_TOWARDZERO);
    i64 z = to_i64( floatX(a) );
    set_rounding_mode(savedRoundingMode);
    return z;
}


bool TestFPU::le_signaling( f32 a, f32 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return le( ax, bx );
}

bool TestFPU::lt_signaling( f32 a, f32 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return lt( ax, bx );
}

bool TestFPU::eq_signaling( f32 a, f32 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return eq( ax, bx );
}


i32 TestFPU::to_i32_round_to_zero( f64 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode( FLOAT_ROUND_TOWARDZERO);
    i32 z = to_i32( floatX(a) );
    set_rounding_mode( savedRoundingMode);
    return z;
}

i64 TestFPU::to_i64_round_to_zero( f64 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode( FLOAT_ROUND_TOWARDZERO);
    i64 z = to_i64( floatX(a) );
    set_rounding_mode( savedRoundingMode);
    return z;
}


bool TestFPU::le_signaling( f64 a, f64 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return le( ax, bx );
}

bool TestFPU::lt_signaling( f64 a, f64 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return lt( ax, bx );
}

bool TestFPU::eq_signaling( f64 a, f64 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return eq( ax, bx );
}


i32 TestFPU::to_i32_round_to_zero( f80 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode( FLOAT_ROUND_TOWARDZERO);
    i32 z = to_i32( floatX(a) );
    set_rounding_mode(savedRoundingMode);
    return z;
}


i64 TestFPU::to_i64_round_to_zero( f80 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode( FLOAT_ROUND_TOWARDZERO);
    i64 z = to_i64( floatX(a) );
    set_rounding_mode( savedRoundingMode);
    return z;
}


bool TestFPU::le_signaling( f80 a, f80 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return le( ax, bx );

}

bool TestFPU::lt_signaling( f80 a, f80 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return lt( ax, bx );
}

bool TestFPU::eq_signaling( f80 a, f80 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return eq( ax, bx );
}


i32 TestFPU::to_i32_round_to_zero( f128 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode( FLOAT_ROUND_TOWARDZERO);
    i32 z = to_i32( floatX(a) );
    set_rounding_mode( savedRoundingMode);
    return z;
}


i64 TestFPU::to_i64_round_to_zero( f128 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode(FLOAT_ROUND_TOWARDZERO);
    i64 z = to_i64( floatX(a) );
    set_rounding_mode(savedRoundingMode);
    return z;
}


bool TestFPU::le_signaling( f128 a, f128 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return le( ax, bx );
}

bool TestFPU::lt_signaling( f128 a, f128 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return lt( ax, bx );
}

bool TestFPU::eq_signaling( f128 a, f128 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise(FLOAT_FLAG_INVALID);
    }
    return eq( ax, bx );
}


i32 TestFPU::to_i32_round_to_zero( f256 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode( FLOAT_ROUND_TOWARDZERO);
    i32 z = to_i32( floatX(a) );
    set_rounding_mode( savedRoundingMode);
    return z;
}


i64 TestFPU::to_i64_round_to_zero( f256 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode(FLOAT_ROUND_TOWARDZERO);
    i64 z = to_i64( floatX(a) );
    set_rounding_mode(savedRoundingMode);
    return z;
}


bool TestFPU::le_signaling( f256 a, f256 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return le( ax, bx );
}

bool TestFPU::lt_signaling( f256 a, f256 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return lt( ax, bx );
}

bool TestFPU::eq_signaling( f256 a, f256 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise(FLOAT_FLAG_INVALID);
    }
    return eq( ax, bx );
}


i32 TestFPU::to_i32_round_to_zero( f512 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode( FLOAT_ROUND_TOWARDZERO);
    i32 const z = to_i32( floatX(a) );
    set_rounding_mode( savedRoundingMode);
    return z;
}


i64 TestFPU::to_i64_round_to_zero( f512 a )
{
    rounding_mode_t savedRoundingMode = get_rounding_mode();
    set_rounding_mode(FLOAT_ROUND_TOWARDZERO);
    i64 const z = to_i64( floatX(a) );
    set_rounding_mode(savedRoundingMode);
    return z;
}


bool TestFPU::le_signaling( f512 a, f512 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return le( ax, bx );
}

bool TestFPU::lt_signaling( f512 a, f512 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise( FLOAT_FLAG_INVALID);
    }
    return lt( ax, bx );
}

bool TestFPU::eq_signaling( f512 a, f512 b )
{
    floatX ax( a );
    floatX bx( b );
    if ( ax.isNaN || bx.isNaN ) {
        raise(FLOAT_FLAG_INVALID);
    }
    return eq( ax, bx );
}

} // namespace fpu
} // namespace postrisc
