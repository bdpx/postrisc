#pragma once

namespace postrisc {
namespace fpu {

//////////////////////////////////////////////////////////////////////////////
// universal floating point class - 32-bit exponent, 504 bit fraction
/////////////////////////////////////////////////////////////////////////////

class floatX {
friend class TestFPU;
public:
    static const floatX NaN;
    static const floatX PositiveZero;
    static const floatX NegativeZero;
    enum {
        BITS_PER_PRECISION        = 512 - 8,
        BITS_PER_HIGH_PRECISION   = BITS_PER_PRECISION % 64
    };
private:
    bool     isNaN, isInf, isZero, sign;
    int      exp;
    u512  sig;

public:
    explicit floatX(void) {}
    explicit floatX(i32);
    explicit floatX(i64);
    explicit floatX(f32);
    explicit floatX(f64);
    explicit floatX(f80);
    explicit floatX(f128);
    explicit floatX(f256);
    explicit floatX(f512);

    explicit floatX(bool nan, bool inf, bool zero, bool sign_, int exp_)
      : isNaN(nan), isInf(inf), isZero(zero), sign(sign_), exp(exp_), sig(0,0,0,0,0,0,0,0) {}

    floatX & neg(void) { sign = ! sign; return *this; }

};

/********************************************************************
* Alternate softfpu implementation for testing
********************************************************************/

class TestFPU : public BaseFPU {
private:
    void round_to_f32( bool isTiny, floatX & zx );
    void round_to_f64( bool isTiny, floatX & zx );
    void round_to_f80( bool isTiny, floatX & zx );
    void round_to_f128( bool isTiny, floatX & zx );
    void round_to_f256( bool isTiny, floatX & zx );
    void round_to_f512( bool isTiny, floatX & zx );

    i32 to_i32(floatX);
    i64 to_i64(floatX);
    f32 to_f32(floatX);
    f64 to_f64(floatX);
    f80 to_f80(floatX);
    f128 to_f128(floatX);
    f256 to_f256(floatX);
    f512 to_f512(floatX);

    floatX floatXInvalid(void);
    floatX round_to_int(floatX);
    floatX add(floatX, floatX);
    floatX mul(floatX, floatX);
    floatX div(floatX, floatX);
    floatX rem(floatX, floatX);
    floatX sqrt(floatX);
    bool eq(floatX, floatX);
    bool le(floatX, floatX);
    bool lt(floatX, floatX);

public:
    i32 to_i32_round_to_zero( f32 );
    i64 to_i64_round_to_zero( f32 );

    bool eq_signaling( f32, f32 );
    bool le_signaling( f32, f32 );
    bool lt_signaling( f32, f32 );

    i32 to_i32_round_to_zero( f64 );
    i64 to_i64_round_to_zero( f64 );

    bool eq_signaling( f64, f64 );
    bool le_signaling( f64, f64 );
    bool lt_signaling( f64, f64 );

    i32 to_i32_round_to_zero( f80 );
    i64 to_i64_round_to_zero( f80 );

    bool eq_signaling( f80, f80 );
    bool le_signaling( f80, f80 );
    bool lt_signaling( f80, f80 );

    i32 to_i32_round_to_zero( f128 );
    i64 to_i64_round_to_zero( f128 );

    bool eq_signaling( f128, f128 );
    bool le_signaling( f128, f128 );
    bool lt_signaling( f128, f128 );

    i32 to_i32_round_to_zero( f256 );
    i64 to_i64_round_to_zero( f256 );

    i32 to_i32_round_to_zero( f512 );
    i64 to_i64_round_to_zero( f512 );

    f64  to_f64 ( f32 a ) {  return to_f64( floatX(a) );  }
    f80  to_f80 ( f32 a ) {  return to_f80( floatX(a) );  }
    f128 to_f128( f32 a ) {  return to_f128( floatX(a) );  }
    f32  to_f32 ( f64 a ) {  return to_f32( floatX(a) );  }
    f80  to_f80 ( f64 a ) {  return to_f80( floatX(a) );  }
    f128 to_f128( f64 a ) {  return to_f128( floatX(a) );  }
    f32  to_f32 ( f80 a ) {  return to_f32( floatX(a) );  }
    f64  to_f64 ( f80 a ) {  return to_f64( floatX(a) );  }
    f128 to_f128( f80 a ) {  return to_f128( floatX(a) );  }
    f32  to_f32 ( f128 a ) {  return to_f32( floatX(a) );  }
    f64  to_f64 ( f128 a ) {  return to_f64( floatX(a) );  }
    f80  to_f80 ( f128 a ) {  return to_f80( floatX(a) );  }

    f32 to_f32( f256 a ) {  return to_f32( floatX(a) );  }
    f64 to_f64( f256 a ) {  return to_f64( floatX(a) );  }
    f80 to_f80( f256 a ) {  return to_f80( floatX(a) );  }
    f128 to_f128( f256 a ) {  return to_f128( floatX(a) );  }

    f32 to_f32( f512 a ) {  return to_f32( floatX(a) );  }
    f64 to_f64( f512 a ) {  return to_f64( floatX(a) );  }
    f80 to_f80( f512 a ) {  return to_f80( floatX(a) );  }
    f128 to_f128( f512 a ) {  return to_f128( floatX(a) );  }
    f256 to_f256( f512 a ) {  return to_f256( floatX(a) );  }

    f256 to_f256( f32 a ) {  return to_f256( floatX(a) );  }
    f256 to_f256( f64 a ) {  return to_f256( floatX(a) );  }
    f256 to_f256( f80 a ) {  return to_f256( floatX(a) );  }
    f256 to_f256( f128 a ) {  return to_f256( floatX(a) );  }

    f512 to_f512( f32 a ) {  return to_f512( floatX(a) );  }
    f512 to_f512( f64 a ) {  return to_f512( floatX(a) );  }
    f512 to_f512( f80 a ) {  return to_f512( floatX(a) );  }
    f512 to_f512( f128 a ) {  return to_f512( floatX(a) );  }
    f512 to_f512( f256 a ) {  return to_f512( floatX(a) );  }

    f32 to_f32( i32 a ) {  return to_f32( floatX(a) );  }
    f64 to_f64( i32 a ) {  return to_f64( floatX(a) );  }
    f80 to_f80( i32 a ) {  return to_f80( floatX(a) );  }
    f128 to_f128( i32 a ) {  return to_f128( floatX(a) );  }
    f256 to_f256( i32 a ) {  return to_f256( floatX(a) ); }
    f512 to_f512( i32 a ) {  return to_f512( floatX(a) ); }

    f32 to_f32( i64 a ) {  return to_f32( floatX(a) );  }
    f64 to_f64( i64 a ) {  return to_f64( floatX(a) );  }
    f80 to_f80( i64 a ) {  return to_f80( floatX(a) );  }
    f128 to_f128( i64 a ) {  return to_f128( floatX(a) );  }
    f256 to_f256( i64 a ) {  return to_f256( floatX(a) ); }
    f512 to_f512( i64 a ) {  return to_f512( floatX(a) ); }

    i32 to_i32( f32 a ) {  return to_i32( floatX(a) );  }
    i64 to_i64( f32 a ) {  return to_i64( floatX(a) );  }
    i32 to_i32( f64 a ) {  return to_i32( floatX(a) );  }
    i64 to_i64( f64 a ) {  return to_i64( floatX(a) );  }
    i32 to_i32( f80 a ) {  return to_i32( floatX(a) );  }
    i64 to_i64( f80 a ) {  return to_i64( floatX(a) );  }
    i32 to_i32( f128 a ) {  return to_i32( floatX(a) );  }
    i64 to_i64( f128 a ) {  return to_i64( floatX(a) );  }
    i32 to_i32( f256 a ) {  return to_i32( floatX(a) );  }
    i64 to_i64( f256 a ) {  return to_i64( floatX(a) );  }
    i32 to_i32( f512 a ) {  return to_i32( floatX(a) );  }
    i64 to_i64( f512 a ) {  return to_i64( floatX(a) );  }

    f32 round_to_int( f32 a ) {  return to_f32( round_to_int( floatX(a) ) ); }
    f64 round_to_int( f64 a ) {  return to_f64( round_to_int( floatX(a) ) ); }
    f80 round_to_int( f80 a ) {  return to_f80( round_to_int( floatX(a) ) ); }
    f128 round_to_int( f128 a ) {  return to_f128( round_to_int( floatX(a) ) ); }
    f256 round_to_int( f256 a ) {  return to_f256( round_to_int( floatX(a) ) ); }
    f512 round_to_int( f512 a ) {  return to_f512( round_to_int( floatX(a) ) ); }

    f32  add( f32 a, f32 b ) {  return to_f32( add( floatX(a), floatX(b) ) ); }
    f64  add( f64 a, f64 b ) {  return to_f64( add( floatX(a), floatX(b) ) ); }
    f80  add( f80 a, f80 b ) {  return to_f80( add( floatX(a), floatX(b) ) ); }
    f128 add( f128 a, f128 b ) {  return to_f128( add( floatX(a), floatX(b) ) ); }
    f256 add( f256 a, f256 b ) { return to_f256( add( floatX(a), floatX(b) ) ); }
    f512 add( f512 a, f512 b ) { return to_f512( add( floatX(a), floatX(b) ) ); }

    f32 sub( f32 a, f32 b ) {  return to_f32( add( floatX(a), floatX(b).neg() ) ); }
    f64 sub( f64 a, f64 b ) {  return to_f64( add( floatX(a), floatX(b).neg() ) ); }
    f80 sub( f80 a, f80 b ) {  return to_f80( add( floatX(a), floatX(b).neg() ) ); }
    f128 sub( f128 a, f128 b ) {  return to_f128( add( floatX(a), floatX(b).neg() ) ); }
    f256 sub( f256 a, f256 b ) { return to_f256( add( floatX(a), floatX(b).neg() ) ); }
    f512 sub( f512 a, f512 b ) { return to_f512( add( floatX(a), floatX(b).neg() ) ); }

    f32 mul( f32 a, f32 b ) {  return to_f32( mul( floatX(a), floatX(b) ) ); }
    f64 mul( f64 a, f64 b ) {  return to_f64( mul( floatX(a), floatX(b) ) ); }
    f80 mul( f80 a, f80 b ) {  return to_f80( mul( floatX(a), floatX(b) ) ); }
    f128 mul( f128 a, f128 b ) {  return to_f128( mul( floatX(a), floatX(b) ) ); }
    f256 mul( f256 a, f256 b ) { return to_f256( mul( floatX(a), floatX(b) ) ); }
    f512 mul( f512 a, f512 b ) { return to_f512( mul( floatX(a), floatX(b) ) ); }

    f32 div( f32 a, f32 b ) {  return to_f32( div( floatX(a), floatX(b) ) ); }
    f64 div( f64 a, f64 b ) {  return to_f64( div( floatX(a), floatX(b) ) ); }
    f80 div( f80 a, f80 b ) {  return to_f80( div( floatX(a), floatX(b) ) ); }
    f128 div( f128 a, f128 b ) {  return to_f128( div( floatX(a), floatX(b) ) ); }
    f256 div( f256 a, f256 b ) { return to_f256( div( floatX(a), floatX(b) ) ); }
    f512 div( f512 a, f512 b ) { return to_f512( div( floatX(a), floatX(b) ) ); }

    f32 rem( f32 a, f32 b ) {  return to_f32( rem( floatX(a), floatX(b) ) ); }
    f64 rem( f64 a, f64 b ) {  return to_f64( rem( floatX(a), floatX(b) ) ); }
    f80 rem( f80 a, f80 b ) {  return to_f80( rem( floatX(a), floatX(b) ) ); }
    f128 rem( f128 a, f128 b ) {  return to_f128( rem( floatX(a), floatX(b) ) ); }
    f256 rem( f256 a, f256 b ) { return to_f256( rem( floatX(a), floatX(b) ) ); }
    f512 rem( f512 a, f512 b ) { return to_f512( rem( floatX(a), floatX(b) ) ); }

    f32 sqrt( f32 a ) {  return to_f32( sqrt( floatX(a) ) );  }
    f64 sqrt( f64 a ) {  return to_f64( sqrt( floatX(a) ) );  }
    f80 sqrt( f80 a ) {  return to_f80( sqrt( floatX(a) ) );  }
    f128 sqrt( f128 a ) {  return to_f128( sqrt( floatX(a) ) );  }
    f256 sqrt( f256 a ) {  return to_f256( sqrt( floatX(a) ) );  }
    f512 sqrt( f512 a ) {  return to_f512( sqrt( floatX(a) ) );  }

    bool eq_quiet( f128 a, f128 b ) {  return eq( floatX(a), floatX(b) ); }
    bool le_quiet( f128 a, f128 b ) {  return le( floatX(a), floatX(b) ); }
    bool lt_quiet( f128 a, f128 b ) {  return lt( floatX(a), floatX(b) ); }

    bool eq_quiet( f80 a, f80 b ) {  return eq( floatX(a), floatX(b) ); }
    bool le_quiet( f80 a, f80 b ) {  return le( floatX(a), floatX(b) ); }
    bool lt_quiet( f80 a, f80 b ) {  return lt( floatX(a), floatX(b) ); }

    bool eq_quiet( f64 a, f64 b ) {  return eq( floatX(a), floatX(b) ); }
    bool le_quiet( f64 a, f64 b ) {  return le( floatX(a), floatX(b) ); }
    bool lt_quiet( f64 a, f64 b ) {  return lt( floatX(a), floatX(b) ); }

    bool eq_quiet( f32 a, f32 b ) {  return eq( floatX(a), floatX(b) ); }
    bool le_quiet( f32 a, f32 b ) {  return le( floatX(a), floatX(b) ); }
    bool lt_quiet( f32 a, f32 b ) {  return lt( floatX(a), floatX(b) ); }

    // already not supported for testing
    f32 exp2( f32 a) { return a; }
    f32 log2( f32 a) { return a; }
    f64 log2( f64 a) { return a; }
    f80 log2( f80 a) { return a; }

    f32 muladd( f32 a, f32 b, f32 c, muladd_negate_t ) {  return to_f32( add(mul( floatX(a), floatX(b) ), floatX(c) ) ); }
    f64 muladd( f64 a, f64 b, f64 c, muladd_negate_t ) {  return to_f64( add(mul( floatX(a), floatX(b) ), floatX(c) ) ); }
    f80 muladd( f80 a, f80 b, f80 c, muladd_negate_t ) {  return to_f80( add(mul( floatX(a), floatX(b) ), floatX(c) ) ); }
    f128 muladd( f128 a, f128 b, f128 c, muladd_negate_t ) {  return to_f128( add(mul( floatX(a), floatX(b) ), floatX(c) ) ); }
    f256 muladd( f256 a, f256 b, f256 c, muladd_negate_t ) {  return to_f256( add(mul( floatX(a), floatX(b) ), floatX(c) ) ); }
    f512 muladd( f512 a, f512 b, f512 c, muladd_negate_t ) {  return to_f512( add(mul( floatX(a), floatX(b) ), floatX(c) ) ); }

    bool eq_signaling( f256, f256 );
    bool le_signaling( f256, f256 );
    bool lt_signaling( f256, f256 );
    bool eq_quiet( f256 a, f256 b ) { return eq( floatX(a), floatX(b) ); }
    bool le_quiet( f256 a, f256 b ) { return le( floatX(a), floatX(b) ); }
    bool lt_quiet( f256 a, f256 b ) { return lt( floatX(a), floatX(b) ); }

    bool eq_signaling( f512, f512 );
    bool le_signaling( f512, f512 );
    bool lt_signaling( f512, f512 );
    bool eq_quiet( f512 a, f512 b ) { return eq( floatX(a), floatX(b) ); }
    bool le_quiet( f512 a, f512 b ) { return le( floatX(a), floatX(b) ); }
    bool lt_quiet( f512 a, f512 b ) { return lt( floatX(a), floatX(b) ); }

};

} // namespace fpu
} // namespace postrisc
