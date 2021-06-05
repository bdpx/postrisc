#pragma once

#include <cfenv>   // for fegetround/fesetround/fegetexceptflag/etc

#if USE_QUADMATH
#include <quadmath.h>
#else
#include "util/mpreal.hpp"
#endif

namespace postrisc {
namespace fpu {

/********************************************************************
* x87 IEEE
********************************************************************/

namespace x87 {
// Rounding modes.
enum fp_round_t {
    ROUND_N = 0,  // round to nearest
    ROUND_M,      // round down towards minus infinity
    ROUND_P,      // round up towards plus infinity
    ROUND_Z       // truncate
};

// Precision (i.e., rounding precision) modes.
enum fp_prec_t {
    PRECISION_32       = 0,  // single-precision
    PRECISION_RESERVED = 1,  // reserved
    PRECISION_64       = 2,  // double-precision
    PRECISION_80       = 3   // extended-precision
};

typedef int fp_except_t;

// Exception bit masks.
enum {
    FLAG_INVALID   =  0x01,    // invalid operation
    FLAG_DENORMAL  =  0x02,    // denormal
    FLAG_DIVBYZERO =  0x04,    // zero divide
    FLAG_OVERFLOW  =  0x08,    // overflow
    FLAG_UNDERFLOW =  0x10,    // underflow
    FLAG_INEXACT   =  0x20,    // (im)precision
    FLAG_STACK     =  0x40     // stack fault
};

// FPU control word bit-field masks.
enum {
    FP_FLAGS_FLD       = 0x3f,   // exception masks field
    FP_PRECISION_FLD  = 0x300,   // precision control field
    FP_ROUND_FLD      = 0xc00,   // rounding control field

// FPU status word bit-field masks.
    FP_STICKY_FLD     =  0x3f    // sticky flags field
};

enum {
// FPU control word bit-field offsets (shift counts).
    FP_FLAGS_OFFSET      =  0,    // exception masks offset
    FP_PRECISION_OFFSET  =  8,    // precision control offset
    FP_ROUND_OFFSET      =  10,   // rounding control offset
// FPU status word bit-field offsets (shift counts).
    FP_STICKY_OFFSET     =  0     // sticky flags offset
};

typedef struct _fp_env_t {
    u32 cw, sw, other[5];
} fp_env_t;

#define    __fldcw(addr)     __asm __volatile("fldcw %0" : : "m" (*(addr)))
#define    __fldenv(addr)    __asm __volatile("fldenv %0" : : "m" (*(addr)))
#define    __fnclex()        __asm __volatile("fnclex")
#define    __fnstcw(addr)    __asm __volatile("fnstcw %0" : "=m" (*(addr)))
#define    __fnstenv(addr)   __asm __volatile("fnstenv %0" : "=m" (*(addr)))
#define    __fnstsw(addr)    __asm __volatile("fnstsw %0" : "=m" (*(addr)))

/*
 * Load the control word.  Be careful not to trap if there is a currently
 * unmasked exception (ones that will become freshly unmasked are not a problem).
 * This case must be handled by a save/restore of the environment or even of the full x87 state.
 * Accessing the environment is very inefficient, so only do it when necessary.
 */
inline void __fnldcw(u16 cw, u16 newcw)
{
    fp_env_t  env;
    u16    sw;

    if ((cw & FP_FLAGS_FLD) != FP_FLAGS_FLD) {
        __fnstsw(&sw);
        if (((sw & ~cw) & FP_STICKY_FLD) != 0) {
            __fnstenv(&env);
            env.cw = newcw;
            __fldenv(&env);
            return;
        }
    }
    __fldcw(&newcw);
}

inline fp_round_t fpgetround(void)
{
    u16 cw;

    __fnstcw(&cw);
    return (fp_round_t)((cw & FP_ROUND_FLD) >> FP_ROUND_OFFSET);
}

inline fp_round_t fpsetround(fp_round_t mode)
{
    fp_round_t   oldmode;
    u16       cw, newcw;

    __fnstcw(&cw);
    oldmode = (fp_round_t)((cw & FP_ROUND_FLD) >> FP_ROUND_OFFSET);
    newcw = cw & ~FP_ROUND_FLD;
    newcw |= (mode << FP_ROUND_OFFSET) & FP_ROUND_FLD;
    __fnldcw(cw, newcw);
    return (oldmode);
}

inline fp_prec_t fpgetprec(void)
{
    u16 cw;

    __fnstcw(&cw);
    return ((fp_prec_t)((cw & FP_PRECISION_FLD) >> FP_PRECISION_OFFSET));
}

inline fp_prec_t fpsetprec(fp_prec_t mode)
{
    fp_prec_t oldmode;
    u16 cw, newcw;

    __fnstcw(&cw);
    oldmode = (fp_prec_t)((cw & FP_PRECISION_FLD) >> FP_PRECISION_OFFSET);
    newcw = cw & ~FP_PRECISION_FLD;
    newcw |= (mode << FP_PRECISION_OFFSET) & FP_PRECISION_FLD;
    __fnldcw(cw, newcw);
    return (oldmode);
}

/* Get or set the exception mask.
 * Note that the x87 mask bits are inverted by the API -- a mask bit of 1
 * means disable for x87 and SSE, but for fp*mask() it means enable. */

inline fp_except_t fpgetmask(void)
{
    u16 cw;

    __fnstcw(&cw);
    return ((~cw & FP_FLAGS_FLD) >> FP_FLAGS_OFFSET);
}

inline fp_except_t fpsetmask(fp_except_t mode)
{
    fp_except_t  oldmode;
    u16          cw, newcw;

    __fnstcw(&cw);
    oldmode = (~cw & FP_FLAGS_FLD) >> FP_FLAGS_OFFSET;
    newcw = cw & ~FP_FLAGS_FLD;
    newcw |= (~mode << FP_FLAGS_OFFSET) & FP_FLAGS_FLD;
    __fnldcw(cw, newcw);
    return (oldmode);
}

inline fp_except_t fpgetsticky(void)
{
    u32  ex;
    u16  sw;

    __fnstsw(&sw);
    ex = (sw & FP_STICKY_FLD) >> FP_STICKY_OFFSET;
    return (fp_except_t)ex;
}

inline fp_except_t fpresetsticky(fp_except_t mode)
{
    fp_env_t     env;
    fp_except_t  oldmode;

    mode &= FP_STICKY_FLD >> FP_STICKY_OFFSET;
    oldmode = fpgetsticky();
    if ((oldmode & ~mode) == oldmode)
        return (oldmode);
    if ((oldmode & ~mode) == 0) {
        __fnclex();
        return (oldmode);
    }
    __fnstenv(&env);
    env.sw &= ~mode;
    __fldenv(&env);
    return (oldmode);
}

} // end x87 namespace

/********************************************************************
*  Hardware FPU class
********************************************************************/

class HardFPU : public BaseFPU
{
public:
    void clear_flags( void );
    void set_rounding_mode(rounding_mode_t val);
    float_flag get_flags(void) const;
//    void set_flush_to_zero(bool val);
    void set_rounding_precision(rounding_precision_t val);

    // for conversions - just simple assignment
    f32  to_f32 ( i32 a ) {  return f32::make( native::f32 ( a ) ); }
    f64  to_f64 ( i32 a ) {  return f64::make( native::f64 ( a ) ); }
    f80  to_f80 ( i32 a ) {  return f80::make( native::f80 ( a ) ); }
    f128 to_f128( i32 a ) {  return f128::make( native::f128( a ) ); }

    f32  to_f32 ( i64 a ) {  return f32::make( native::f32 ( a )); }
    f64  to_f64 ( i64 a ) {  return f64::make( native::f64 ( a )); }
    f80  to_f80 ( i64 a ) {  return f80::make( native::f80 ( a )); }
    f128 to_f128( i64 a ) {  return f128::make( native::f128( a )); }

    f64  to_f64 ( f32  a ) {  return  f64::make( native::f64 ( a.hard() )); }
    f80  to_f80 ( f32  a ) {  return  f80::make( native::f80 ( a.hard() )); }
    f128 to_f128( f32  a ) {  return f128::make( native::f128( a.hard() )); }
    f32  to_f32 ( f64  a ) {  return  f32::make( native::f32 ( a.hard() )); }
    f80  to_f80 ( f64  a ) {  return  f80::make( native::f80 ( a.hard() )); }
    f128 to_f128( f64  a ) {  return f128::make( native::f128( a.hard() )); }
    f32  to_f32 ( f80  a ) {  return  f32::make( native::f32 ( a.hard() )); }
    f64  to_f64 ( f80  a ) {  return  f64::make( native::f64 ( a.hard() )); }
    f128 to_f128( f80  a ) {  return f128::make( native::f128( a.hard() )); }
    f32  to_f32 ( f128 a ) {  return  f32::make( native::f32 ( a.hard() )); }
    f64  to_f64 ( f128 a ) {  return  f64::make( native::f64 ( a.hard() )); }
    f80  to_f80 ( f128 a ) {  return  f80::make( native::f80 ( a.hard() )); }

    // use current rounding mode (rint-like functions)
    i32 to_i32( f32  a ) {  return   rintf( a.hard() ); } // cant fint i32rint
    i64 to_i64( f32  a ) {  return llrintf( a.hard() ); }
    i32 to_i32( f64  a ) {  return   rint ( a.hard() ); } // cant fint i32rint
    i64 to_i64( f64  a ) {  return llrint ( a.hard() ); }
    i32 to_i32( f80  a ) {  return   rintl( a.hard() ); } // cant fint i32rint
    i64 to_i64( f80  a ) {  return llrintl( a.hard() ); }
#if USE_QUADMATH
    i32 to_i32( f128 a ) {  return   (i32)rintq( a.hard() ); } // cant fint i32rint
    i64 to_i64( f128 a ) {  return   (i64)rintq( a.hard() ); }
#else
    i32 to_i32( f128 a ) {  return   (i32)mpfr::rint( a.hard() ); } // cant fint i32rint
    i64 to_i64( f128 a ) {  return   (i64)mpfr::rint( a.hard() ); }
#endif
    // C++ always convert float to int by rounding toward zero, so we just return
    i16 to_i16_round_to_zero( f32  a ) {  return a.hard(); }
    i32 to_i32_round_to_zero( f32  a ) {  return a.hard(); }
    i64 to_i64_round_to_zero( f32  a ) {  return a.hard(); }
    i32 to_i32_round_to_zero( f64  a ) {  return a.hard(); }
    i64 to_i64_round_to_zero( f64  a ) {  return a.hard(); }
    i32 to_i32_round_to_zero( f80  a ) {  return a.hard(); }
    i64 to_i64_round_to_zero( f80  a ) {  return a.hard(); }
    i32 to_i32_round_to_zero( f128 a ) {  return a.hard(); }
    i64 to_i64_round_to_zero( f128 a ) {  return a.hard(); }

    // use current rounding mode (rint-like functions)
    f32  round_to_int( f32  a ) {  return f32::make( rintf( a.hard() ) ); }
    f64  round_to_int( f64  a ) {  return f64::make( rint ( a.hard() ) ); }
    f80  round_to_int( f80  a ) {  return f80::make( rintl( a.hard() ) ); }
#if USE_QUADMATH
    f128 round_to_int( f128 a ) {  return f128::make( (native::f128)(i64)rintq( a.hard() ) ); }
#else
    f128 round_to_int( f128 a ) {  return f128::make( (native::f128)(i64)mpfr::rint( a.hard() ) ); }
#endif
    // don't work properly, since compiler use quiet variant
    bool eq_signaling( f32  a, f32  b ) {  return ! ( a.hard() != b.hard() ); }
    bool le_signaling( f32  a, f32  b ) {  return ! ( a.hard() >  b.hard() ); }
    bool lt_signaling( f32  a, f32  b ) {  return ! ( a.hard() >= b.hard() ); }
    bool eq_signaling( f64  a, f64  b ) {  return ! ( a.hard() != b.hard() ); }
    bool le_signaling( f64  a, f64  b ) {  return ! ( a.hard() >  b.hard() ); }
    bool lt_signaling( f64  a, f64  b ) {  return ! ( a.hard() >= b.hard() ); }
    bool eq_signaling( f80  a, f80  b ) {  return ! ( a.hard() != b.hard() ); }
    bool le_signaling( f80  a, f80  b ) {  return ! ( a.hard() >  b.hard() ); }
    bool lt_signaling( f80  a, f80  b ) {  return ! ( a.hard() >= b.hard() ); }
    bool eq_signaling( f128 a, f128 b ) {  return ! ( a.hard() != b.hard() ); }
    bool le_signaling( f128 a, f128 b ) {  return ! ( a.hard() >  b.hard() ); }
    bool lt_signaling( f128 a, f128 b ) {  return ! ( a.hard() >= b.hard() ); }

    bool eq_quiet( f32  a, f32  b ) {  return a.hard() == b.hard(); }
    bool le_quiet( f32  a, f32  b ) {  return a.hard() <= b.hard(); }
    bool lt_quiet( f32  a, f32  b ) {  return a.hard() <  b.hard(); }
    bool eq_quiet( f80  a, f80  b ) {  return a.hard() == b.hard(); }
    bool le_quiet( f80  a, f80  b ) {  return a.hard() <= b.hard(); }
    bool lt_quiet( f80  a, f80  b ) {  return a.hard() <  b.hard(); }
    bool eq_quiet( f64  a, f64  b ) {  return a.hard() == b.hard(); }
    bool le_quiet( f64  a, f64  b ) {  return a.hard() <= b.hard(); }
    bool lt_quiet( f64  a, f64  b ) {  return a.hard() <  b.hard(); }
    bool eq_quiet( f128 a, f128 b ) {  return a.hard() == b.hard(); }
    bool le_quiet( f128 a, f128 b ) {  return a.hard() <= b.hard(); }
    bool lt_quiet( f128 a, f128 b ) {  return a.hard() <  b.hard(); }

    f32 add( f32 a, f32 b ) {  return f32::make( a.hard() + b.hard() ); }
    f32 sub( f32 a, f32 b ) {  return f32::make( a.hard() - b.hard() ); }
    f32 mul( f32 a, f32 b ) {  return f32::make( a.hard() * b.hard() ); }
    f32 div( f32 a, f32 b ) {  return f32::make( a.hard() / b.hard() ); }

    f64 add( f64 a, f64 b ) {  return f64::make( a.hard() + b.hard() ); }
    f64 sub( f64 a, f64 b ) {  return f64::make( a.hard() - b.hard() ); }
    f64 mul( f64 a, f64 b ) {  return f64::make( a.hard() * b.hard() ); }
    f64 div( f64 a, f64 b ) {  return f64::make( a.hard() / b.hard() ); }

    f80 add( f80 a, f80 b ) {  return f80::make( a.hard() + b.hard() ); }
    f80 sub( f80 a, f80 b ) {  return f80::make( a.hard() - b.hard() ); }
    f80 mul( f80 a, f80 b ) {  return f80::make( a.hard() * b.hard() ); }
    f80 div( f80 a, f80 b ) {  return f80::make( a.hard() / b.hard() ); }

    f128 add( f128 a, f128 b ) {  return f128::make( a.hard() + b.hard() ); }
    f128 sub( f128 a, f128 b ) {  return f128::make( a.hard() - b.hard() ); }
    f128 mul( f128 a, f128 b ) {  return f128::make( a.hard() * b.hard() ); }
    f128 div( f128 a, f128 b ) {  return f128::make( a.hard() / b.hard() ); }

    f32 exp2( f32 a ) { return f32::make( ::exp2f( a.hard() ) ); }
    f64 exp2( f64 a ) { return f64::make( ::exp2 ( a.hard() ) ); }
    f80 exp2( f80 a ) { return f80::make( ::exp2l( a.hard() ) ); }
//    f128 exp2( f128 a ) { f128 z;  z.hard() = ::exp2q( a.hard() );   return z; }

    f32  log2( f32  a ) { return f32::make( ::log2f( a.hard() ) ); }
    f64  log2( f64  a ) { return f64::make( ::log2 ( a.hard() ) ); }
    f80  log2( f80  a ) { return f80::make( ::log2l( a.hard() ) ); }
#if USE_QUADMATH
    f128 log2( f128 a ) { return f128::make( (native::f128)log2q( a.hard() ) ); }
#else
    f128 log2( f128 a ) { return f128::make( (native::f128)mpfr::log2( a.hard() ) ); }
#endif

    f32  sqrt( f32 a ) { return f32::make( ::sqrtf( a.hard() ) ); }
    f64  sqrt( f64 a ) { return f64::make( ::sqrt ( a.hard() ) ); }
    f80  sqrt( f80 a ) { return f80::make( ::sqrtl( a.hard() ) ); }
#if USE_QUADMATH
    f128 sqrt( f128 a ) { return f128::make( (native::f128)sqrtq( a.hard() ) ); }
#else
    f128 sqrt( f128 a ) { return f128::make( (native::f128)mpfr::sqrt( a.hard() ) ); }
#endif

    f32  rem( f32  a, f32  b ) {  return f32::make( remainderf( a.hard(), b.hard() ) ); }
    f64  rem( f64  a, f64  b ) {  return f64::make( remainder ( a.hard(), b.hard() ) ); }
    f80  rem( f80  a, f80  b ) {  return f80::make( remainderl( a.hard(), b.hard() ) ); }
#if USE_QUADMATH
    f128 rem( f128 a, f128 b ) {  return f128::make( (native::f128)remainderq( a.hard(), b.hard() ) ); }
#else
    f128 rem( f128 a, f128 b ) {  return f128::make( (native::f128)mpfr::remainder( a.hard(), b.hard() ) ); }
#endif

    f32  muladd( f32  a, f32  b, f32  c, muladd_negate_t ) {   return f32::make( fmaf( a.hard(), b.hard(), c.hard() ) ); }
    f64  muladd( f64  a, f64  b, f64  c, muladd_negate_t ) {   return f64::make( fma ( a.hard(), b.hard(), c.hard() ) ); }
    f80  muladd( f80  a, f80  b, f80  c, muladd_negate_t ) {   return f80::make( fmal( a.hard(), b.hard(), c.hard() ) ); }
#if USE_QUADMATH
    f128 muladd( f128 a, f128 b, f128 c, muladd_negate_t ) {   return f128::make( (native::f128)fmaq( a.hard(), b.hard(), c.hard() ) ); }
#else
    f128 muladd( f128 a, f128 b, f128 c, muladd_negate_t ) {   return f128::make( (native::f128)mpfr::fma( a.hard(), b.hard(), c.hard() ) ); }
#endif

};

/********************************************************************
* Hardware fpu
********************************************************************/

inline void HardFPU::clear_flags( void )
{
    BaseFPU::clear_flags();
    std::feclearexcept (FE_ALL_EXCEPT);
}


inline void HardFPU::set_rounding_mode(rounding_mode_t val)
{
    BaseFPU::set_rounding_mode(val);
    int rdir = FE_TONEAREST;
    switch (val) {
        default:
        case FLOAT_ROUND_TONEAREST_EVEN:  rdir = FE_TONEAREST;   break;
        case FLOAT_ROUND_DOWNWARD:        rdir = FE_DOWNWARD;    break;
        case FLOAT_ROUND_UPWARD:          rdir = FE_UPWARD;      break;
        case FLOAT_ROUND_TOWARDZERO:      rdir = FE_TOWARDZERO;  break;
    }
    std::fesetround (rdir);
}


inline float_flag HardFPU::get_flags(void) const
{
    fexcept_t hw_flags;
    std::fegetexceptflag (&hw_flags, FE_ALL_EXCEPT);
    float_flag flags = FLOAT_FLAG_NOERROR;
    if (hw_flags & FE_INEXACT)   flags = flags | FLOAT_FLAG_INEXACT;
    if (hw_flags & FE_INVALID)   flags = flags | FLOAT_FLAG_INVALID;
    if (hw_flags & FE_DIVBYZERO) flags = flags | FLOAT_FLAG_DIVBYZERO;
    if (hw_flags & FE_OVERFLOW)  flags = flags | FLOAT_FLAG_OVERFLOW;
    if (hw_flags & FE_UNDERFLOW) flags = flags | FLOAT_FLAG_UNDERFLOW;
    return flags;
}


inline void HardFPU::set_rounding_precision(rounding_precision_t val)
{
    x87::fp_prec_t mode;
    switch (val) {
        default:
        case FLOAT_ROUND_PRECISION_80:  mode = x87::PRECISION_80;  break;
        case FLOAT_ROUND_PRECISION_64:  mode = x87::PRECISION_64;  break;
        case FLOAT_ROUND_PRECISION_32:  mode = x87::PRECISION_32;  break;
    }
    x87::fpsetprec(mode);
}


} // namespace fpu
} // namespace postrisc
