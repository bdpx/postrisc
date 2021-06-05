#include <cfenv>   // for fegetround/fesetround/fegetexceptflag/etc

#include "hardfloat.hpp"

#if USE_QUADMATH
#warning quadmath used
#include <quadmath.h>
#define FMAQ   ::fmaq
#define SQRTQ  ::sqrtq
#define LOG2Q  ::log2q
#define RINTQ  ::rintq
#else
#warning mpreal used
#include "util/mpreal.hpp"
#define FMAQ   mpfr::fma
#define SQRTQ  mpfr::sqrt
#define LOG2Q  mpfr::log2
#define RINTQ  mpfr::rint
#endif

/********************************************************************
* Hardware fpu
********************************************************************/

namespace postrisc {
namespace fpu {

template<> f32  HardFPU::round_to_int<f32>( f32  a ) {  return f32::soft( rintf( a.hard() ) ); }
template<> f64  HardFPU::round_to_int<f64>( f64  a ) {  return f64::soft( rint ( a.hard() ) ); }
template<> f128 HardFPU::round_to_int<f128>( f128 a ) {  return f128::soft( (native::f128)(i64)RINTQ( a.hard() ) ); }

template<> f16  HardFPU::muladd< f16>( f16  a, f16  b, f16  c, muladd_negate_t ) { return  f16::soft( (f16::F_HARD)fmaf( a.hard(), b.hard(), c.hard() )); }
template<> f32  HardFPU::muladd< f32>( f32  a, f32  b, f32  c, muladd_negate_t ) { return  f32::soft( fmaf( a.hard(), b.hard(), c.hard() )); }
template<> f64  HardFPU::muladd< f64>( f64  a, f64  b, f64  c, muladd_negate_t ) { return  f64::soft( fma ( a.hard(), b.hard(), c.hard() )); }
template<> f128 HardFPU::muladd<f128>( f128 a, f128 b, f128 c, muladd_negate_t ) { return f128::soft( (native::f128)FMAQ( a.hard(), b.hard(), c.hard() )); }

template<> f16  HardFPU::sqrt<f16> ( f16  a ) { return f16 ::soft( (f16::F_HARD)::sqrtf( a.hard() ) ); }
template<> f32  HardFPU::sqrt<f32> ( f32  a ) { return f32 ::soft( ::sqrtf( a.hard() ) ); }
template<> f64  HardFPU::sqrt<f64> ( f64  a ) { return f64 ::soft( ::sqrt ( a.hard() ) ); }
template<> f128 HardFPU::sqrt<f128>( f128 a ) { return f128::soft( (native::f128)SQRTQ( a.hard() ) ); }

template<> f32 HardFPU::exp<f32>( f32 a ) { return f32::soft( ::expf( a.hard() ) ); }
template<> f64 HardFPU::exp<f64>( f64 a ) { return f64::soft( ::exp ( a.hard() ) ); }
//    f128 exp2( f128 a ) { f128 z;  z.hard() = ::exp2q( a.hard() );   return z; }

template<> f32  HardFPU::log2<f32>( f32  a ) { return f32::soft( ::log2f( a.hard() ) ); }
template<> f64  HardFPU::log2<f64>( f64  a ) { return f64::soft( ::log2 ( a.hard() ) ); }
template<> f128 HardFPU::log2<f128>( f128 a ) { return f128::soft( (native::f128)LOG2Q( a.hard() ) ); }

// C++ always convert float to int by rounding toward zero, so we just return
// template<typename INTEGER, typename FLOAT> INTEGER cvt_f_i(FLOAT a, RoundingModeEnum) { return a.hard(); }

template<> i32  HardFPU::cvt_f_i<i32,  f16>(f16 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> i64  HardFPU::cvt_f_i<i64,  f16>(f16 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> i128 HardFPU::cvt_f_i<i128, f16>(f16 a, RoundingModeEnum) { return rintf( a.hard() ); }

template<> u32  HardFPU::cvt_f_i<u32,  f16>(f16 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> u64  HardFPU::cvt_f_i<u64,  f16>(f16 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> u128 HardFPU::cvt_f_i<u128, f16>(f16 a, RoundingModeEnum) { return rintf( a.hard() ); }

template<> i32  HardFPU::cvt_f_i<i32,  f32>(f32 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> i64  HardFPU::cvt_f_i<i64,  f32>(f32 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> i128 HardFPU::cvt_f_i<i128, f32>(f32 a, RoundingModeEnum) { return rintf( a.hard() ); }

template<> u32  HardFPU::cvt_f_i<u32,  f32>(f32 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> u64  HardFPU::cvt_f_i<u64,  f32>(f32 a, RoundingModeEnum) { return rintf( a.hard() ); }
template<> u128 HardFPU::cvt_f_i<u128, f32>(f32 a, RoundingModeEnum) { return rintf( a.hard() ); }

template<> i32  HardFPU::cvt_f_i<i32,  f64>(f64 a, RoundingModeEnum) { return rint( a.hard() ); }
template<> i64  HardFPU::cvt_f_i<i64,  f64>(f64 a, RoundingModeEnum) { return rint( a.hard() ); }
template<> i128 HardFPU::cvt_f_i<i128, f64>(f64 a, RoundingModeEnum) { return rint( a.hard() ); }

template<> u32  HardFPU::cvt_f_i<u32,  f64>(f64 a, RoundingModeEnum) { return rint( a.hard() ); }
template<> u64  HardFPU::cvt_f_i<u64,  f64>(f64 a, RoundingModeEnum) { return rint( a.hard() ); }
template<> u128 HardFPU::cvt_f_i<u128, f64>(f64 a, RoundingModeEnum) { return rint( a.hard() ); }

template<> i32  HardFPU::cvt_f_i<i32,  f128>(f128 a, RoundingModeEnum) { return (native::f128)RINTQ( a.hard() ); }
template<> i64  HardFPU::cvt_f_i<i64,  f128>(f128 a, RoundingModeEnum) { return (native::f128)RINTQ( a.hard() ); }
template<> i128 HardFPU::cvt_f_i<i128, f128>(f128 a, RoundingModeEnum) { return (native::f128)RINTQ( a.hard() ); }

template<> u32  HardFPU::cvt_f_i<u32,  f128>(f128 a, RoundingModeEnum) { return (native::f128)RINTQ( a.hard() ); }
template<> u64  HardFPU::cvt_f_i<u64,  f128>(f128 a, RoundingModeEnum) { return (native::f128)RINTQ( a.hard() ); }
template<> u128 HardFPU::cvt_f_i<u128, f128>(f128 a, RoundingModeEnum) { return (native::f128)RINTQ( a.hard() ); }


void HardFPU::set_rounding_mode(RoundingModeEnum val)
{
    int rdir = FE_TONEAREST;
    switch (val) {
        case RM_RNE:  rdir = FE_TONEAREST;   break;
        case RM_RDN:  rdir = FE_DOWNWARD;    break;
        case RM_RUP:  rdir = FE_UPWARD;      break;
        case RM_RTZ:  rdir = FE_TOWARDZERO;  break;
        case RM_RMM:
            std::cerr << "unsupported test rounding mode: " << (int)val << std::endl;
            exit(1);
    }
    std::fesetround(rdir);
    m_rounding_mode = val;

#if USE_QUADMATH
#else
    mp_rnd_t mpfr_rnd_mode = MPFR_RNDN;
    switch (val) {
        case RM_RNE:  mpfr_rnd_mode = MPFR_RNDN;  break;
        case RM_RDN:  mpfr_rnd_mode = MPFR_RNDD;  break;
        case RM_RUP:  mpfr_rnd_mode = MPFR_RNDU;  break;
        case RM_RTZ:  mpfr_rnd_mode = MPFR_RNDZ;  break;
        case RM_RMM:  mpfr_rnd_mode = MPFR_RNDA;  break;
    }
    mpfr::mpreal::set_default_rnd(mpfr_rnd_mode);
    mpfr::mpreal::set_default_prec(113); // for f128
#endif
}

void HardFPU::clear_flags(fp_flags mask)
{
    int flags = 0;

    if (mask & FFLAG_INEXACT)   flags |= FE_INEXACT;
    if (mask & FFLAG_INVALID)   flags |= FE_INVALID;
    if (mask & FFLAG_DIVBYZERO) flags |= FE_DIVBYZERO;
    if (mask & FFLAG_OVERFLOW)  flags |= FE_OVERFLOW;
    if (mask & FFLAG_UNDERFLOW) flags |= FE_UNDERFLOW;

    std::feclearexcept(flags);
}

fp_flags HardFPU::get_flags(void) const
{
    fexcept_t hw_flags = 0;
    std::fegetexceptflag(&hw_flags, FE_ALL_EXCEPT);
    fp_flags flags = FFLAG_NOERROR;

    if (hw_flags & FE_INEXACT)   flags = flags | FFLAG_INEXACT;
    if (hw_flags & FE_INVALID)   flags = flags | FFLAG_INVALID;
    if (hw_flags & FE_DIVBYZERO) flags = flags | FFLAG_DIVBYZERO;
    if (hw_flags & FE_OVERFLOW)  flags = flags | FFLAG_OVERFLOW;
    if (hw_flags & FE_UNDERFLOW) flags = flags | FFLAG_UNDERFLOW;
    return flags;
}

} // namespace fpu
} // namespace postrisc
