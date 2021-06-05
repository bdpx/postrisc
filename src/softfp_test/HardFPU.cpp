#if WORKFLOW_CI_BUILD == 0 && defined(__x86_64__)
#warning WORKFLOW_CI_BUILD == 0, X86_64 INTRINSICS USED
#include <emmintrin.h>
#include <immintrin.h>
#endif

#include "HardFPU.hpp"

#if USE_QUADMATH
#warning quadmath used
#include <quadmath.h>
//#define FMAQ   ::fmaq
#define SQRTQ  ::sqrtq
#define LOG2Q  ::log2q
#define RINTQ  ::rintq
#else
#warning mpreal used
#include "util/mpreal.hpp"
//#define FMAQ   mpfr::fma
#define SQRTQ  mpfr::sqrt
#define LOG2Q  mpfr::log2
#define RINTQ  mpfr::rint
#endif

/********************************************************************
* Hardware fpu
********************************************************************/

namespace postrisc {
namespace fpu {

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

#if 0
    const int xx = _MM_GET_EXCEPTION_STATE(); // _MM_SET_EXCEPTION_STATE(x)
    if (xx & _MM_EXCEPT_INVALID)   flags = flags | FFLAG_INVALID;
    if (xx & _MM_EXCEPT_DIV_ZERO)  flags = flags | FFLAG_DIVBYZERO;
    // if (xx & _MM_EXCEPT_DENORM)    flags |=
    if (xx & _MM_EXCEPT_OVERFLOW)  flags = flags | FFLAG_OVERFLOW;
    if (xx & _MM_EXCEPT_UNDERFLOW) flags = flags | FFLAG_UNDERFLOW;
    if (xx & _MM_EXCEPT_INEXACT)   flags = flags | FFLAG_INEXACT;
#endif

    return flags;
}

#if USE_QUADMATH == 0
mpfr_rnd_t setup_mpfr(RoundingModeEnum rm)
{
    mpfr_rnd_t rdir = MPFR_RNDN;
    switch (rm) {
        case RM_RNO:
        case RM_RTO:
        case RM_RNE:  rdir = MPFR_RNDN;  break;
        case RM_RDN:  rdir = MPFR_RNDD;  break;
        case RM_RUP:  rdir = MPFR_RNDU;  break;
        case RM_RTZ:  rdir = MPFR_RNDZ;  break;
        case RM_RMM:  rdir = MPFR_RNDA;  break;
        default:
            std::cerr << "invalid rounding mode" << std::endl;
            exit(1);
    }
    mpfr::mpreal::set_default_prec(113);
    mpfr::mpreal::set_default_rnd(rdir);
    return rdir;
}

fp_flags get_mpft_flags()
{
   mpfr_flags_t const mf = mpfr_flags_test(MPFR_FLAGS_ALL);
   fp_flags ret = FFLAG_NOERROR;
   if (mf & MPFR_FLAGS_UNDERFLOW) ret = ret | FFLAG_UNDERFLOW;
   if (mf & MPFR_FLAGS_OVERFLOW ) ret = ret | FFLAG_OVERFLOW;
   if (mf & MPFR_FLAGS_NAN      ) ret = ret | FFLAG_INVALID;
   if (mf & MPFR_FLAGS_INEXACT  ) ret = ret | FFLAG_INEXACT;
   if (mf & MPFR_FLAGS_ERANGE   ) ret = ret | FFLAG_NOERROR;
   if (mf & MPFR_FLAGS_DIVBY0   ) ret = ret | FFLAG_DIVBYZERO;
   return ret;
}
#endif

void HardFPU::set_rounding_mode(RoundingModeEnum val)
{
    int rdir = FE_TONEAREST;
    switch (val) {
        case RM_RMM:
        case RM_RNO:
        case RM_RTO:
        case RM_RNE:  rdir = FE_TONEAREST;   break;
        case RM_RDN:  rdir = FE_DOWNWARD;    break;
        case RM_RUP:  rdir = FE_UPWARD;      break;
        case RM_RTZ:  rdir = FE_TOWARDZERO;  break;
        default:
            std::cerr << "unsupported test rounding mode: " << (int)val << std::endl;
            exit(1);
    }
    std::fesetround(rdir);
    m_rounding_mode = val;

    // fedisableexcept(FE_ALL_EXCEPT);

#if WORKFLOW_CI_BUILD == 0 && defined(__x86_64__)
    _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_OFF);
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_OFF);
#endif

#if USE_QUADMATH == 0
    setup_mpfr(val);
#endif
}

native::f128 muladd(native::f128 a, native::f128 b, native::f128 c, RoundingModeEnum rm)
{
#if USE_QUADMATH
    UNREFERENCED_PARAMETER(rm);
    //set_flags(rm);
    native::f128 ret = fmaq(a, b, c);
    //flags = get_flags();
    return ret;
#else
    mpfr_clear_flags();
    mpfr_flags_clear(MPFR_FLAGS_ALL);
    mpfr_rnd_t const rdir = setup_mpfr(rm);
    mpfr_set_emin(-f128::BIAS - f128::FRAC_SIZE + 1);
    mpfr_set_emax(f128::BIAS + 1);

    native::f128 ret = mpfr::fma(mpfr::mpreal(a, 113, rdir),
                     mpfr::mpreal(b, 113, rdir),
                     mpfr::mpreal(c, 113, rdir), rdir).toFloat128(rdir);

    const fp_flags soft_flags = get_mpft_flags();

    int flags = 0;

    if (soft_flags & FFLAG_INEXACT)   flags |= FE_INEXACT;
    if (soft_flags & FFLAG_INVALID)   flags |= FE_INVALID;
    if (soft_flags & FFLAG_DIVBYZERO) flags |= FE_DIVBYZERO;
    if (soft_flags & FFLAG_OVERFLOW)  flags |= FE_OVERFLOW;
    if (soft_flags & FFLAG_UNDERFLOW) flags |= FE_UNDERFLOW;

    if (flags)
        std::feraiseexcept(flags);

    return ret;
#endif
}

// need AVX-512 FP16 support
// call via Intel SDE emulator: sde64 -spr -- ./softfp_test -t f16_to_u64_rdn
#if WORKFLOW_CI_BUILD == 0 && defined(__x86_64__)

#define AVX_FP16 __attribute__((target("avx512fp16")))
#define SSE_FMA __attribute__((target("fma")))

static AVX_FP16 i16 convert_i16(__m128i value) { i16 out[8]; _mm_storeu_si128((__m128i*)out, value); return out[0]; }
static AVX_FP16 u16 convert_u16(__m128i value) { u16 out[8]; _mm_storeu_si128((__m128i*)out, value); return out[0]; }

// convert fp vector to scalar
static AVX_FP16 f16 convert(__m128h value) { native::f16 out[8]; _mm_storeu_ph(out, value); return f16::soft(out[0]); }
static AVX_FP16 f32 convert(__m128  value) { native::f32 out[4]; _mm_storeu_ps(out, value); return f32::soft(out[0]); }
static AVX_FP16 f64 convert(__m128d value) { native::f64 out[2]; _mm_storeu_pd(out, value); return f64::soft(out[0]); }

static AVX_FP16 f16 fp16_fma(f16 a, f16 b, f16 c) { return convert(_mm_fmadd_sh(_mm_set_sh(a.hard()), _mm_set_sh(b.hard()), _mm_set_sh(c.hard()))); }

static AVX_FP16 f16 fp16_add(f16 a, f16 b) { return convert(_mm_add_sh(_mm_set_sh(a.hard()), _mm_set_sh(b.hard()))); }
static AVX_FP16 f16 fp16_sub(f16 a, f16 b) { return convert(_mm_sub_sh(_mm_set_sh(a.hard()), _mm_set_sh(b.hard()))); }
static AVX_FP16 f16 fp16_mul(f16 a, f16 b) { return convert(_mm_mul_sh(_mm_set_sh(a.hard()), _mm_set_sh(b.hard()))); }
static AVX_FP16 f16 fp16_div(f16 a, f16 b) { return convert(_mm_div_sh(_mm_set_sh(a.hard()), _mm_set_sh(b.hard()))); }

static AVX_FP16 i16 fp16_f16_to_i16(f16 a) { return convert_i16(_mm_cvtph_epi16(_mm_set_sh(a.hard()))); }
static AVX_FP16 u16 fp16_f16_to_u16(f16 a) { return convert_u16(_mm_cvtph_epu16(_mm_set_sh(a.hard()))); }

static AVX_FP16 i32 fp16_f16_to_i32(f16 a) { return _mm_cvtsh_i32(_mm_set_sh(a.hard())); }
static AVX_FP16 u32 fp16_f16_to_u32(f16 a) { return _mm_cvtsh_u32(_mm_set_sh(a.hard())); }
static AVX_FP16 i64 fp16_f16_to_i64(f16 a) { return _mm_cvtsh_i64(_mm_set_sh(a.hard())); }
static AVX_FP16 u64 fp16_f16_to_u64(f16 a) { return _mm_cvtsh_u64(_mm_set_sh(a.hard())); }

static AVX_FP16 f16 fp16_i32_to_f16(i32 a) { return convert(_mm_cvti32_sh(_mm_setzero_ph(), a)); }
static AVX_FP16 f16 fp16_u32_to_f16(u32 a) { return convert(_mm_cvtu32_sh(_mm_setzero_ph(), a)); }
static AVX_FP16 f16 fp16_i64_to_f16(i64 a) { return convert(_mm_cvti64_sh(_mm_setzero_ph(), a)); }
static AVX_FP16 f16 fp16_u64_to_f16(u64 a) { return convert(_mm_cvtu64_sh(_mm_setzero_ph(), a)); }

static AVX_FP16 f16 fp16_f32_to_f16(f32 a) { return convert(_mm_cvtss_sh(_mm_setzero_ph(), _mm_set_ss(a.hard()))); }
static AVX_FP16 f16 fp16_f64_to_f16(f64 a) { return convert(_mm_cvtsd_sh(_mm_setzero_ph(), _mm_set_sd(a.hard()))); }

static SSE_FMA f32 fp32_fma(f32 a, f32 b, f32 c) { return convert(_mm_fmadd_ss(_mm_set_ss(a.hard()), _mm_set_ss(b.hard()), _mm_set_ss(c.hard()))); }
static SSE_FMA f64 fp64_fma(f64 a, f64 b, f64 c) { return convert(_mm_fmadd_sd(_mm_set_sd(a.hard()), _mm_set_sd(b.hard()), _mm_set_sd(c.hard()))); }

#define AVX_FP16_CODE(CODE) if (avx_fp16_supports()) { return CODE; }
#define SSE_FMA_CODE(CODE)  if (__builtin_cpu_supports("fma")) { return CODE; }

#else // non-x86 or WORKFLOW_CI_BUILD

#define AVX_FP16_CODE(CODE) ;
#define SSE_FMA_CODE(CODE)  ;

#endif

template<> f16 HardFPU::muladd<f16>( f16  a, f16  b, f16  c, RoundingModeEnum UNUSED(rm), muladd_negate_t UNUSED(negate)) {
    AVX_FP16_CODE(fp16_fma(a, b, c));
    //return f16::soft( (f16::F_HARD)__builtin_fmaf16(a.hard(), b.hard(), c.hard()));
    return f16::soft(static_cast<f16::F_HARD>(std::fmaf(a.hard(), b.hard(), c.hard())));
}

template<> f16 HardFPU::add(f16 a, f16 b, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_add(a, b));
    return f16::soft( a.hard() + b.hard() );
}

template<> f16 HardFPU::sub(f16 a, f16 b, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_sub(a, b));
    return f16::soft( a.hard() - b.hard() );
}

template<> f16 HardFPU::mul(f16 a, f16 b, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_mul(a, b));
    return f16::soft( a.hard() * b.hard() );
}

template<> f16 HardFPU::div(f16 a, f16 b, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_div(a, b));
    return f16::soft( a.hard() / b.hard() );
}

template<> i16 HardFPU::cvt_f_i<i16, f16>(f16 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f16_to_i16(a));
    i32 r =static_cast<native::f16>(rintf(a.hard()));
    if (std::fetestexcept(FE_INVALID)) {
        r >>= 16;
    } else if (static_cast<i16>(r) != r) {
        std::feraiseexcept(FE_INVALID);
        r = 0x8000;
    }
    return static_cast<i16>(r);
}

template<> u16 HardFPU::cvt_f_i<u16, f16>(f16 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f16_to_u16(a));
    u32 r = static_cast<native::f16>(rintf(a.hard()));
    if (std::fetestexcept(FE_INVALID)) {
        r >>= 16;
    } else if (static_cast<u16>(r) != r) {
        std::feraiseexcept(FE_INVALID);
        r = 0xFFFF;
    }
    return static_cast<u16>(r);
}

template<> i32 HardFPU::cvt_f_i<i32, f16>(f16 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f16_to_i32(a));
    return static_cast<native::f16>(rintf( a.hard() ));
}

template<> u32 HardFPU::cvt_f_i<u32, f16>(f16 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f16_to_u32(a));
    return static_cast<native::f16>(rintf( a.hard() ));
}

template<> i64 HardFPU::cvt_f_i<i64, f16>(f16 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f16_to_i64(a));
    return static_cast<native::f16>(rintf( a.hard() ));
}

template<> u64 HardFPU::cvt_f_i<u64, f16>(f16 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f16_to_u64(a));
    return static_cast<native::f16>(rintf( a.hard() ));
}

template<> f16 HardFPU::cvt_i_f<f16, i32>(i32 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_i32_to_f16(a));
    return f16::soft((f16::F_HARD)(a));
}

template<> f16 HardFPU::cvt_i_f<f16, u32>(u32 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_u32_to_f16(a));
    return f16::soft((f16::F_HARD)(a));
}

template<> f16 HardFPU::cvt_i_f<f16, i64>(i64 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_i64_to_f16(a));
    return f16::soft((f16::F_HARD)(a));
}

template<> f16 HardFPU::cvt_i_f<f16, u64>(u64 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_u64_to_f16(a));
    return f16::soft((f16::F_HARD)(a));
}

template<> f16 HardFPU::cvt_f_f(f32 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f32_to_f16(a));
    return f16::soft((typename f16::F_HARD)(a.hard()));
}

template<> f16 HardFPU::cvt_f_f(f64 a, RoundingModeEnum UNUSED(rm)) {
    AVX_FP16_CODE(fp16_f64_to_f16(a));
    return f16::soft((typename f16::F_HARD)(a.hard()));
}

template<> f32 HardFPU::muladd<f32>(f32 a, f32 b, f32 c, RoundingModeEnum UNUSED(rm), muladd_negate_t UNUSED(negate)) {
    SSE_FMA_CODE(fp32_fma(a, b, c));
    return f32::soft(std::fmaf(a.hard(), b.hard(), c.hard()));
}

template<> f64 HardFPU::muladd<f64>(f64 a, f64 b, f64 c, RoundingModeEnum UNUSED(rm), muladd_negate_t UNUSED(negate)) {
    SSE_FMA_CODE(fp64_fma(a, b, c));
    return f64::soft(std::fma(a.hard(), b.hard(), c.hard()));
}

template<> f128 HardFPU::muladd<f128>(f128 a, f128 b, f128 c, RoundingModeEnum rm, muladd_negate_t UNUSED(negate)) {
    return f128::soft(fpu::muladd( a.hard(), b.hard(), c.hard(), rm));
}


template<> f16  HardFPU::sqrt<f16> (f16  a, RoundingModeEnum UNUSED(rm)) { return f16 ::soft( (f16::F_HARD)::sqrtf( a.hard() ) ); }
template<> f32  HardFPU::sqrt<f32> (f32  a, RoundingModeEnum UNUSED(rm)) { return f32 ::soft( ::sqrtf( a.hard() ) ); }
template<> f64  HardFPU::sqrt<f64> (f64  a, RoundingModeEnum UNUSED(rm)) { return f64 ::soft( ::sqrt ( a.hard() ) ); }
template<> f128 HardFPU::sqrt<f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return f128::soft( (native::f128)SQRTQ( a.hard() ) ); }

template<> f32 HardFPU::exp<f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return f32::soft( ::expf( a.hard() ) ); }
template<> f64 HardFPU::exp<f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return f64::soft( ::exp ( a.hard() ) ); }
//    f128 exp2( f128 a ) { f128 z;  z.hard() = ::exp2q( a.hard() );   return z; }

template<> f16  HardFPU::log2<f16 >( f16 a, RoundingModeEnum UNUSED(rm)) { return f16::soft( static_cast<native::f16>(std::log2f( a.hard() ))); }
template<> f32  HardFPU::log2<f32 >( f32 a, RoundingModeEnum UNUSED(rm)) { return f32::soft( std::log2f( a.hard() ) ); }
template<> f64  HardFPU::log2<f64 >( f64 a, RoundingModeEnum UNUSED(rm)) { return f64::soft( std::log2 ( a.hard() ) ); }
template<> f128 HardFPU::log2<f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return f128::soft( (native::f128)LOG2Q( a.hard() ) ); }

template<> f16  HardFPU::round<f16> (f16  a, RoundingModeEnum UNUSED(rm), bool UNUSED(inexact)) { return f16::soft(static_cast<native::f16>(rintf(a.hard()))); }
template<> f32  HardFPU::round<f32> (f32  a, RoundingModeEnum UNUSED(rm), bool UNUSED(inexact)) { return f32::soft(rintf(a.hard())); }
template<> f64  HardFPU::round<f64> (f64  a, RoundingModeEnum UNUSED(rm), bool UNUSED(inexact)) { return f64::soft(rint(a.hard())); }
template<> f128 HardFPU::round<f128>(f128 a, RoundingModeEnum UNUSED(rm), bool UNUSED(inexact)) { return f128::soft((native::f128)RINTQ(a.hard())); }

#if WORKFLOW_CI_BUILD == 0 && defined(__x86_64__)
#warning SSE-FP32|64 INTRINSICS USED
template<> f32 HardFPU::add(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_add_ss(_mm_set_ss(a.hard()), _mm_set_ss(b.hard())))); }
template<> f32 HardFPU::sub(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_sub_ss(_mm_set_ss(a.hard()), _mm_set_ss(b.hard())))); }
template<> f32 HardFPU::mul(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_mul_ss(_mm_set_ss(a.hard()), _mm_set_ss(b.hard())))); }
template<> f32 HardFPU::div(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_div_ss(_mm_set_ss(a.hard()), _mm_set_ss(b.hard())))); }

template<> f64 HardFPU::add(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_add_sd(_mm_set_sd(a.hard()), _mm_set_sd(b.hard())))); }
template<> f64 HardFPU::sub(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_sub_sd(_mm_set_sd(a.hard()), _mm_set_sd(b.hard())))); }
template<> f64 HardFPU::mul(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_mul_sd(_mm_set_sd(a.hard()), _mm_set_sd(b.hard())))); }
template<> f64 HardFPU::div(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_div_sd(_mm_set_sd(a.hard()), _mm_set_sd(b.hard())))); }

template<> i32 HardFPU::cvt_f_i<i32, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtss_i32(_mm_set_ss(a.hard())); }
template<> u32 HardFPU::cvt_f_i<u32, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtss_u32(_mm_set_ss(a.hard())); }
template<> i64 HardFPU::cvt_f_i<i64, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtss_i64(_mm_set_ss(a.hard())); }
template<> u64 HardFPU::cvt_f_i<u64, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtss_u64(_mm_set_ss(a.hard())); }

template<> i32 HardFPU::cvt_f_i<i32, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtsd_i32(_mm_set_sd(a.hard())); }
template<> u32 HardFPU::cvt_f_i<u32, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtsd_u32(_mm_set_sd(a.hard())); }
template<> i64 HardFPU::cvt_f_i<i64, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtsd_i64(_mm_set_sd(a.hard())); }
template<> u64 HardFPU::cvt_f_i<u64, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return _mm_cvtsd_u64(_mm_set_sd(a.hard())); }

template<> f32 HardFPU::cvt_i_f<f32, i32>(i32 a, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_cvti32_ss(_mm_setzero_ps(), a))); }
template<> f32 HardFPU::cvt_i_f<f32, i64>(i64 a, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_cvti64_ss(_mm_setzero_ps(), a))); }
template<> f32 HardFPU::cvt_i_f<f32, u32>(u32 a, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_cvtu32_ss(_mm_setzero_ps(), a))); }
template<> f32 HardFPU::cvt_i_f<f32, u64>(u64 a, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_cvtu64_ss(_mm_setzero_ps(), a))); }

template<> f64 HardFPU::cvt_i_f<f64, i32>(i32 a, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_cvti32_sd(_mm_setzero_pd(), a))); }
template<> f64 HardFPU::cvt_i_f<f64, i64>(i64 a, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_cvti64_sd(_mm_setzero_pd(), a))); }
template<> f64 HardFPU::cvt_i_f<f64, u32>(u32 a, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_cvtu32_sd(_mm_setzero_pd(), a))); }
template<> f64 HardFPU::cvt_i_f<f64, u64>(u64 a, RoundingModeEnum UNUSED(rm)) { return f64::soft(_mm_cvtsd_f64(_mm_cvtu64_sd(_mm_setzero_pd(), a))); }

template<> f32 HardFPU::cvt_f_f(f64  a, RoundingModeEnum UNUSED(rm)) { return f32::soft(_mm_cvtss_f32(_mm_cvtsd_ss(_mm_setzero_ps(), _mm_set_sd(a.hard())))); }
#else
template<> f32 HardFPU::add(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft( a.hard() + b.hard() ); }
template<> f32 HardFPU::sub(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft( a.hard() - b.hard() ); }
template<> f32 HardFPU::mul(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft( a.hard() * b.hard() ); }
template<> f32 HardFPU::div(f32 a, f32 b, RoundingModeEnum UNUSED(rm)) { return f32::soft( a.hard() / b.hard() ); }

template<> f64 HardFPU::add(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft( a.hard() + b.hard() ); }
template<> f64 HardFPU::sub(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft( a.hard() - b.hard() ); }
template<> f64 HardFPU::mul(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft( a.hard() * b.hard() ); }
template<> f64 HardFPU::div(f64 a, f64 b, RoundingModeEnum UNUSED(rm)) { return f64::soft( a.hard() / b.hard() ); }

template<> i32 HardFPU::cvt_f_i<i32, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return rintf(a.hard()); }
template<> u32 HardFPU::cvt_f_i<u32, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return rintf(a.hard()); }
template<> i64 HardFPU::cvt_f_i<i64, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return rintf(a.hard()); }
template<> u64 HardFPU::cvt_f_i<u64, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return rintf(a.hard()); }

template<> i32 HardFPU::cvt_f_i<i32, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return rint(a.hard()); }
template<> u32 HardFPU::cvt_f_i<u32, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return rint(a.hard()); }
template<> i64 HardFPU::cvt_f_i<i64, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return rint(a.hard()); }
template<> u64 HardFPU::cvt_f_i<u64, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return rint(a.hard()); }

template<> f32 HardFPU::cvt_i_f<f32, i32>(i32 a, RoundingModeEnum UNUSED(rm)) { return f32::soft((f32::F_HARD)(a)); }
template<> f32 HardFPU::cvt_i_f<f32, i64>(i64 a, RoundingModeEnum UNUSED(rm)) { return f32::soft((f32::F_HARD)(a)); }
template<> f32 HardFPU::cvt_i_f<f32, u32>(u32 a, RoundingModeEnum UNUSED(rm)) { return f32::soft((f32::F_HARD)(a)); }
template<> f32 HardFPU::cvt_i_f<f32, u64>(u64 a, RoundingModeEnum UNUSED(rm)) { return f32::soft((f32::F_HARD)(a)); }

template<> f64 HardFPU::cvt_i_f<f64, i32>(i32 a, RoundingModeEnum UNUSED(rm)) { return f64::soft((f64::F_HARD)(a)); }
template<> f64 HardFPU::cvt_i_f<f64, u32>(u32 a, RoundingModeEnum UNUSED(rm)) { return f64::soft((f64::F_HARD)(a)); }
template<> f64 HardFPU::cvt_i_f<f64, i64>(i64 a, RoundingModeEnum UNUSED(rm)) { return f64::soft((f64::F_HARD)(a)); }
template<> f64 HardFPU::cvt_i_f<f64, u64>(u64 a, RoundingModeEnum UNUSED(rm)) { return f64::soft((f64::F_HARD)(a)); }

template<> f32 HardFPU::cvt_f_f(f64  a, RoundingModeEnum UNUSED(rm)) { return f32::soft((typename f32::F_HARD)(a.hard())); }
#endif

template<> f16 HardFPU::cvt_f_f(f128 a, RoundingModeEnum UNUSED(rm)) { return f16::soft((typename f16::F_HARD)(a.hard())); }
template<> f32 HardFPU::cvt_f_f(f128 a, RoundingModeEnum UNUSED(rm)) { return f32::soft((typename f32::F_HARD)(a.hard())); }
template<> f64 HardFPU::cvt_f_f(f128 a, RoundingModeEnum UNUSED(rm)) { return f64::soft((typename f64::F_HARD)(a.hard())); }

// i128, u128, f128 - no hardware
template<> f128 HardFPU::add(f128 a, f128 b, RoundingModeEnum UNUSED(rm)) { return f128::soft( a.hard() + b.hard() ); }
template<> f128 HardFPU::sub(f128 a, f128 b, RoundingModeEnum UNUSED(rm)) { return f128::soft( a.hard() - b.hard() ); }
template<> f128 HardFPU::mul(f128 a, f128 b, RoundingModeEnum UNUSED(rm)) { return f128::soft( a.hard() * b.hard() ); }
template<> f128 HardFPU::div(f128 a, f128 b, RoundingModeEnum UNUSED(rm)) { return f128::soft( a.hard() / b.hard() ); }

template<> i128 HardFPU::cvt_f_i<i128, f16>(f16 a, RoundingModeEnum UNUSED(rm)) { return static_cast<native::f16>(rintf(a.hard())); }
template<> u128 HardFPU::cvt_f_i<u128, f16>(f16 a, RoundingModeEnum UNUSED(rm)) { return static_cast<native::f16>(rintf(a.hard())); }

template<> i128 HardFPU::cvt_f_i<i128, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }
template<> u128 HardFPU::cvt_f_i<u128, f32>(f32 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }
template<> i128 HardFPU::cvt_f_i<i128, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }
template<> u128 HardFPU::cvt_f_i<u128, f64>(f64 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }

template<> i32  HardFPU::cvt_f_i<i32,  f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }
template<> i64  HardFPU::cvt_f_i<i64,  f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }
template<> i128 HardFPU::cvt_f_i<i128, f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }

template<> u32  HardFPU::cvt_f_i<u32,  f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }
template<> u64  HardFPU::cvt_f_i<u64,  f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }
template<> u128 HardFPU::cvt_f_i<u128, f128>(f128 a, RoundingModeEnum UNUSED(rm)) { return (native::f128)RINTQ(a.hard()); }

template<> f16  HardFPU::cvt_i_f<f16, i128>(i128 a, RoundingModeEnum UNUSED(rm)) { return f16::soft((f16::F_HARD)(a)); }
template<> f16  HardFPU::cvt_i_f<f16, u128>(u128 a, RoundingModeEnum UNUSED(rm)) { return f16::soft((f16::F_HARD)(a)); }

template<> f128 HardFPU::cvt_i_f<f128, i32>(i32 a, RoundingModeEnum UNUSED(rm)) { return f128::soft((f128::F_HARD)(a)); }
template<> f128 HardFPU::cvt_i_f<f128, u32>(u32 a, RoundingModeEnum UNUSED(rm)) { return f128::soft((f128::F_HARD)(a)); }
template<> f128 HardFPU::cvt_i_f<f128, i64>(i64 a, RoundingModeEnum UNUSED(rm)) { return f128::soft((f128::F_HARD)(a)); }
template<> f128 HardFPU::cvt_i_f<f128, u64>(u64 a, RoundingModeEnum UNUSED(rm)) { return f128::soft((f128::F_HARD)(a)); }

template<> f32  HardFPU::cvt_i_f<f32, i128>(i128 a, RoundingModeEnum UNUSED(rm)) { return  f32::soft(( f32::F_HARD)(a)); }
template<> f64  HardFPU::cvt_i_f<f64, i128>(i128 a, RoundingModeEnum UNUSED(rm)) { return  f64::soft(( f64::F_HARD)(a)); }
template<> f128 HardFPU::cvt_i_f<f128,i128>(i128 a, RoundingModeEnum UNUSED(rm)) { return f128::soft((f128::F_HARD)(a)); }

template<> f32  HardFPU::cvt_i_f<f32, u128>(u128 a, RoundingModeEnum UNUSED(rm)) { return  f32::soft(( f32::F_HARD)(a)); }
template<> f64  HardFPU::cvt_i_f<f64, u128>(u128 a, RoundingModeEnum UNUSED(rm)) { return  f64::soft(( f64::F_HARD)(a)); }
template<> f128 HardFPU::cvt_i_f<f128,u128>(u128 a, RoundingModeEnum UNUSED(rm)) { return f128::soft((f128::F_HARD)(a)); }

} // namespace fpu
} // namespace postrisc
