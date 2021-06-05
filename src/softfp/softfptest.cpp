#include <cstdlib>
#include <cmath>
#include <cfenv>
#include <xmmintrin.h>
#include <iostream>
#include <iomanip>
#include <exception>
#include <gmp.h>

#include "util/common.hpp"
#include "util/mpreal.hpp"
#include "util/quadruple.hpp"
#include "emulator/SoftFpu.hpp"
#include "scalar_uint.hpp"
#include "bigint.hpp"
#include "multiprecision.hpp"

namespace postrisc {
namespace fpu {

void test_scalar_uint()
{
    scalar_uint<u32> a;
    scalar_uint<u32> b = 2u;
    scalar_uint<u32> c = 2u;
    scalar_uint<u32> d(1u);
    scalar_uint<u32> e(1u);
    scalar_uint<u32> f = a + b;
    d += c/e;
    a <<= 1;
    b >>= 1;
    f <<= 2;
}

void test_256()
{
    uint<256> a = from_string<uint<256>>("1000000000000000000000000000000000000000000000000000999999999999");
    uint<256> b = from_string<uint<256>>("1000000000000000000000000000000000000000000000000009999999999991");

    LOG_ERROR(LOG_EVALUATE) << "a  =" << a;
    LOG_ERROR(LOG_EVALUATE) << "b  =" << b;
    uint<512> c = umul(a, b);

    LOG_ERROR(LOG_EVALUATE) << "c  =" << c;
    LOG_ERROR(LOG_EVALUATE) << "chx=" << hex(c);
    LOG_ERROR(LOG_EVALUATE) << "c10=" << to_string(c, 10);
}

//std::ostringstream FPU::debug_stream;

enum FloatOpEnum {
    OP_FADD,
    OP_FSUB,
    OP_FMUL,
    OP_FDIV,
    OP_FSQRT,
    OP_FLOG2,
    OP_FMIN,
    OP_FMAX,
    OP_FEXP,
};

static char const * const op_to_str[] = {
    "fadd", "fsub", "fmul", "fdiv", "fsqrt", "flog2", "fmin", "fmax", "fexp",
};

static const char * const rm_to_str[5] = { "RNE", "RTZ", "RDN", "RUP", "RMM" };

u32 rrandom_u32(int len);
u64 rrandom_u64(int len);
u128 rrandom_u128(int len);

#if 0
static void print_u128(uint128_t a)
{
    printf("%016" PRIx64 "%016" PRIx64, 
           (u64)(a >> 64), (u64)a);
}
void sqrtrem32_test(void)
{
    u64 a, a0, a1;
    u32 r;
    int inexact, inexact_ref;
    for(;;) {
        a = (u64)random_uint(32) << 32;
        inexact = sqrtrem_u32(&r, a >> 32, a);
        a0 = (u64)r * r;
        a1 = (u64)(r + 1) * (r + 1);
        inexact_ref = (a0 != a);
        if (!(a0 <= a && a < a1) || inexact != inexact_ref) {
            printf("ERROR: a=%" PRId64 " r=%d\n", a, r);
            exit(1);
        }
    }
}
#endif

void set_flags(RoundingModeEnum rm)
{
    int rdir = FE_TONEAREST;
    switch (rm) {
        default:
        case RM_RNE:  rdir = FE_TONEAREST;   break;
        case RM_RDN:  rdir = FE_DOWNWARD;    break;
        case RM_RUP:  rdir = FE_UPWARD;      break;
        case RM_RTZ:  rdir = FE_TOWARDZERO;  break;
    }
    std::fesetround(rdir);
    std::feclearexcept(FE_ALL_EXCEPT);
}

u8 get_flags()
{
    fexcept_t hw_flags = 0;
    std::fegetexceptflag(&hw_flags, FE_ALL_EXCEPT);
    u8 flags = 0;
    if (hw_flags & FE_INEXACT)   flags |= FFLAG_INEXACT;
    if (hw_flags & FE_INVALID)   flags |= FFLAG_INVALID;
    if (hw_flags & FE_DIVBYZERO) flags |= FFLAG_DIVBYZERO;
    if (hw_flags & FE_OVERFLOW)  flags |= FFLAG_OVERFLOW;
    if (hw_flags & FE_UNDERFLOW) flags |= FFLAG_UNDERFLOW;
    return flags;
}

#if USE_QUADMATH == 0
mpfr_rnd_t setup_mpfr(RoundingModeEnum rm)
{
    mpfr_rnd_t rdir = MPFR_RNDN;
    switch (rm) {
        case RM_RNE:  rdir = MPFR_RNDN;  break;
        case RM_RDN:  rdir = MPFR_RNDD;  break;
        case RM_RUP:  rdir = MPFR_RNDU;  break;
        case RM_RTZ:  rdir = MPFR_RNDZ;  break;
        case RM_RMM:  rdir = MPFR_RNDA;  break; //FIXME
    }
    mpfr::mpreal::set_default_prec(113);
    mpfr::mpreal::set_default_rnd(rdir);

//MPFR_RNDN	0	Round to nearest, with ties to even.
//MPFR_RNDZ	1	Round toward zero.
//MPFR_RNDU	2	Round toward +Infinity.
//MPFR_RNDD	3	Round toward -Infinity.
//MPFR_RNDA	4	Round away from zero.
    return rdir;
}

u8 get_mpft_flags()
{
   mpfr_flags_t const mf = mpfr_flags_test(MPFR_FLAGS_ALL);
   u8 ret = 0;
   if (mf & MPFR_FLAGS_UNDERFLOW) ret |= FFLAG_UNDERFLOW;
   if (mf & MPFR_FLAGS_OVERFLOW) ret |= FFLAG_OVERFLOW;
   if (mf & MPFR_FLAGS_NAN) ret |= FFLAG_INVALID;
   if (mf & MPFR_FLAGS_INEXACT) ret |= FFLAG_INEXACT;
   if (mf & MPFR_FLAGS_ERANGE) ret |= 0;
   if (mf & MPFR_FLAGS_DIVBY0) ret |= FFLAG_DIVBYZERO;
   return ret;
}
#endif

template<typename FLOAT>
FLOAT muladd(FLOAT a, FLOAT b, FLOAT c, RoundingModeEnum rm, u8 & flags);

template<>
float muladd<float>(float a, float b, float c, RoundingModeEnum rm, u8 & flags)
{
    set_flags(rm);
    float ret = std::fmaf(a, b, c);
    flags = get_flags();
    return ret;
    //mpfr_rnd_t const rdir = setup_mpfr(rm);
    //return mpfr::fma(mpfr::mpreal(a, 24, rdir),
    //                 mpfr::mpreal(b, 24, rdir),
    //                 mpfr::mpreal(c, 24, rdir), rdir).toFloat(rdir);
}

template<>
double muladd<double>(double a, double b, double c, RoundingModeEnum rm, u8 & flags)
{
    set_flags(rm);
    double ret = std::fma(a, b, c);
    flags = get_flags();
    return ret;
    //mpfr_rnd_t const rdir = setup_mpfr(rm);
    //return mpfr::fma(mpfr::mpreal(a, 53, rdir),
    //                 mpfr::mpreal(b, 53, rdir),
    //                 mpfr::mpreal(c, 53, rdir), rdir).toDouble(rdir);
}

template<>
__float128 muladd<__float128>(__float128 a, __float128 b, __float128 c, RoundingModeEnum rm, u8 & flags)
{
#if USE_QUADMATH
    set_flags(rm);
    __float128 ret = fmaq(a, b, c);
    flags = get_flags();
    return ret;
#else
    mpfr_clear_flags();
    mpfr_flags_clear(MPFR_FLAGS_ALL);
    mpfr_rnd_t const rdir = setup_mpfr(rm);
    mpfr_set_emin(-f128::BIAS - f128::MANT_SIZE + 1);
    mpfr_set_emax(f128::BIAS + 1);

    __float128 ret = mpfr::fma(mpfr::mpreal(a, 113, rdir),
                     mpfr::mpreal(b, 113, rdir),
                     mpfr::mpreal(c, 113, rdir), rdir).toFloat128(rdir);
    flags = get_mpft_flags();
    return ret;
#endif
}

template<typename FLOAT>
FLOAT rint(FLOAT a, RoundingModeEnum rm);

template<>
float rint(float a, RoundingModeEnum) {  return std::rintf(a); }

template<>
double rint(double a, RoundingModeEnum) {  return std::rint(a); }

template<>
__float128 rint(__float128 a, RoundingModeEnum rm)
{
#if USE_QUADMATH
    return rintq(a);
#else
    mpfr_rnd_t const rdir = setup_mpfr(rm);
    return mpfr::rint(mpfr::mpreal(a, 113), rdir).toFloat128(rdir);
#endif
}


template<typename FLOAT>
FLOAT my_sqrt(FLOAT a, RoundingModeEnum rm);

template<>
float my_sqrt(float a, RoundingModeEnum) {  return std::sqrt(a); }

template<>
double my_sqrt(double a, RoundingModeEnum) {  return std::sqrt(a); }

template<>
__float128 my_sqrt(__float128 a, RoundingModeEnum rm)
{
#if USE_QUADMATH
    return sqrtq(a);
#else
    mpfr_rnd_t const rdir = setup_mpfr(rm);
    return mpfr::sqrt(mpfr::mpreal(a, 113), rdir).toFloat128(rdir);
#endif
}

template<typename FLOAT>
FLOAT my_log2(FLOAT a, RoundingModeEnum);

template<>
float my_log2(float a, RoundingModeEnum)
{
    return std::log2f(a);
    //mpfr_rnd_t const rdir = setup_mpfr(rm);
    //return mpfr::log2(mpfr::mpreal(a, 213), rdir).toFloat(rdir);
}

template<>
double my_log2(double a, RoundingModeEnum)
{
   return std::log2(a);
}

template<>
__float128 my_log2(__float128 a, RoundingModeEnum rm)
{
#if USE_QUADMATH
    return log2q(a);
#else
    mpfr_rnd_t const rdir = setup_mpfr(rm);
    return mpfr::log2(mpfr::mpreal(a, 113), rdir).toFloat128(rdir);
#endif
}

template<typename FLOAT>
FLOAT my_exp(FLOAT a, RoundingModeEnum rm);

template<>
float my_exp(float a, RoundingModeEnum) {  return std::exp(a); }

template<>
double my_exp(double a, RoundingModeEnum) {  return std::exp(a); }

template<>
__float128 my_exp(__float128 a, RoundingModeEnum rm)
{
#if USE_QUADMATH
    return sqrtq(a);
#else
    mpfr_rnd_t const rdir = setup_mpfr(rm);
    return mpfr::exp(mpfr::mpreal(a, 113), rdir).toFloat128(rdir);
#endif
}



template<typename INTEGER, typename FLOAT>
INTEGER cvt_int(FLOAT a, RoundingModeEnum rm);

template<typename INTEGER> INTEGER cvt_int(float  a, RoundingModeEnum) {  return (INTEGER)std::rintf(a); /*return std::lrintf (a);*/ }
template<typename INTEGER> INTEGER cvt_int(double a, RoundingModeEnum) {  return (INTEGER)std::rint(a); /*std::lrint  (a);*/ }
template<typename INTEGER> INTEGER cvt_int(__float128 a, RoundingModeEnum rm)
{
#if USE_QUADMATH
    return rintq(a);
#else
    mpfr_rnd_t const rdir = setup_mpfr(rm);
    return (INTEGER)mpfr::rint(mpfr::mpreal(a, 113), rdir);
#endif
}

template<typename FLOAT> class print_sf {
public:
    print_sf(FLOAT v_) : v(v_) {}
    template<typename T> friend std::ostream& operator<< (std::ostream& out, print_sf<T> v);
private:
    FLOAT v;
};

template<typename FLOAT>
std::ostream& operator<< (std::ostream& out, print_sf<FLOAT> v)
{
    auto a_sign = v.v.get_sign();
    auto a_exp = v.v.get_exp();
    auto a_mant = v.v.get_fraction();
    out << a_sign;
    out << ' ';
    out << fmt::hex<typename FLOAT::F_UINT, (FLOAT::EXP_SIZE + 3) / 4>(a_exp);
    out << ' ';
    out << fmt::hex<typename FLOAT::F_UINT, (FLOAT::MANT_SIZE + 3) / 4>(a_mant);
    return out;
}

template<>
std::ostream& operator<< (std::ostream& out, print_sf<f128> v)
{
    auto a_sign = v.v.get_sign();
    auto a_exp = v.v.get_exp();
    auto a_mant = v.v.get_fraction();
    out << a_sign;
    out << ' ' << fmt::hex<u64, (f128::EXP_SIZE + 3) / 4>(a_exp);
    out << ' ' << fmt::hex<u64, ((f128::MANT_SIZE - 64) + 3) / 4>((u64)(a_mant >> 64));
    out << '_' << fmt::hex((u64)(a_mant));
    return out;
}

/* random integer with long sequences of '0' and '1' */
template<typename FLOAT>
typename FLOAT::F_UINT
rrandom_u(size_t len)
{
    size_t bit, pos, n, end;
    typename FLOAT::F_UINT a;

    bit = rand() & 1;
    pos = 0;
    a = 0;
    for(;;) {
        n = (rand() % len) + 1;
        end = pos + n;
        if (end > len)
            end = len;
        if (bit) {
            n = end - pos;
            if (n == FLOAT::F_SIZE)
                a = -(typename FLOAT::F_UINT)1;
            else
                a |= (((typename FLOAT::F_UINT)1 << n) - 1) << pos;
        }
        if (end >= len)
            break;
        pos = end;
        bit ^= 1;
    }
    return a;
}

template<typename FLOAT>
FLOAT rrandom_sf(void)
{
    u32 a_exp, a_sign;
    a_sign = rand() & 1;

    /* generate exponent close to the min/max more often than random */
    switch(rand() & 15) {
    case 0:
        a_exp = (rand() % (2 * FLOAT::MANT_SIZE)) & FLOAT::EXP_MASK;
        break;
    case 1:
        a_exp = (FLOAT::EXP_MASK - (rand() % (2 * FLOAT::MANT_SIZE))) & FLOAT::EXP_MASK;
        break;
    default:
        a_exp = rand() & FLOAT::EXP_MASK;
        break;
    }
    typename FLOAT::F_UINT a_mant = rrandom_u<FLOAT>(FLOAT::MANT_SIZE);
    return SoftFPU::pack_sf<FLOAT>(a_sign, a_exp, a_mant);
}

#define SPECIAL_COUNT 12

template<typename FLOAT>
FLOAT special_sf(int i)
{
    switch(i) {
    case 0: /* zero */
    case 1:
        return SoftFPU::pack_sf<FLOAT>(i & 1, 0, 0);
    case 2: /* infinity */
    case 3:
        return SoftFPU::pack_sf<FLOAT>(i & 1, FLOAT::EXP_MASK, 0);
    case 4: /* 1.0 */
    case 5:
        return SoftFPU::pack_sf<FLOAT>(i & 1, FLOAT::BIAS, 0);
    case 6: /* QNAN */
    case 7:
        return SoftFPU::pack_sf<FLOAT>(i & 1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
    case 8: /* SNAN */
    case 9:
        return SoftFPU::pack_sf<FLOAT>(i & 1, FLOAT::EXP_MASK, 1);
    case 10: /* subnormal */
    case 11:
        return SoftFPU::pack_sf<FLOAT>(i & 1, 0, 1);
    default:
        std::cerr << "abort " << __FILE__ << __LINE__;
        exit(1);
    }
}

template<typename FLOAT>
union float_union {
    constexpr float_union(void) noexcept : u() {}
    FLOAT u;
    typename FLOAT::F_HARD f;
};

template<typename FLOAT>
FLOAT
exec_ref_op(FloatOpEnum op, FLOAT a1, FLOAT b1, RoundingModeEnum rm)
{
    float_union<FLOAT> r, a, b;
    a.u = a1;
    b.u = b1;

    set_flags(rm);

    switch (op) {
    case OP_FADD:
        r.f = a.f + b.f;
        break;
    case OP_FSUB:
        r.f = a.f - b.f;
        break;
    case OP_FMUL:
        r.f = a.f * b.f;
        break;
    case OP_FDIV:
        r.f = a.f / b.f;
        break;
    case OP_FSQRT:
        r.f = my_sqrt(a.f, rm);
        break;
    case OP_FLOG2:
        r.f = my_log2(a.f, rm);
        break;
    case OP_FMIN:
        if (is_nan(a.u) || is_nan(b.u))
            r.u.v = FLOAT::F_QNAN;
        else
            r.f = std::min(a.f, b.f);
        break;
    case OP_FMAX:
        if (is_nan(a.u) || is_nan(b.u))
            r.u.v = FLOAT::F_QNAN;
        else
            r.f = std::max(a.f, b.f);
        break;
    case OP_FEXP:
        r.f = my_exp(a.f, rm);
        break;
    default:
        std::cout << "abort op=" << op << " F_SIZE=" << FLOAT::F_SIZE << " " <<  __FILE__ << " " << __LINE__;
        exit(1);
    }
    if (is_nan(r.u))
        r.u.v = FLOAT::F_QNAN;

    return r.u;
}

template<typename FLOAT>
void
test_op(FloatOpEnum op, FLOAT a, FLOAT b, RoundingModeEnum rm)
{
    FLOAT const ref = exec_ref_op(op, a, b, rm);
    u8 const ref_fflags = get_flags();

    SoftFPU fpu;
    fpu.set_rounding_mode(rm);
    FLOAT r;
    switch (op) {
    case OP_FADD:    r = fpu.add(a, b);       break;
    case OP_FSUB:    r = fpu.sub(a, b);       break;
    case OP_FMUL:    r = fpu.mul(a, b);       break;
    case OP_FDIV:    r = fpu.div(a, b);       break;
    case OP_FSQRT:   r = fpu.sqrt(a);         break;
    case OP_FLOG2:   r = fpu.log2(a);         break;
    case OP_FMIN:    r = fpu.min(a, b);       break;
    case OP_FMAX:    r = fpu.max(a, b);       break;
    case OP_FEXP:    r = fpu.exp(a);          break;
    default:
        std::cerr << "abort " << __FILE__ << __LINE__;
        exit(1);
    }
    u8 const fflags = fpu.get_flags();

    if ((op != OP_FMIN && op != OP_FMAX) &&
        (r.v != ref.v || (op != OP_FSQRT && op != OP_FLOG2 && op != OP_FEXP && ref_fflags != fflags))) {
        std::cout << "ERROR op=" << op_to_str[op] << " size=" << FLOAT::F_SIZE
                  << " rm=" << rm_to_str[rm] << fmt::endl;
        std::cout << "a  = " << print_sf(a) << fmt::endl;
        if (op != OP_FSQRT && op != OP_FLOG2 && op != OP_FEXP) {
            std::cout << "b  = " << print_sf(b) << fmt::endl;
        }
        std::cout << "ref= " << print_sf(ref) << " fflags=0x" << fmt::hex(ref_fflags) << fmt::endl;
        std::cout << "r  = " << print_sf(r) << " fflags=0x" << fmt::hex(fflags) << fmt::endl;
    }
}

template<typename FLOAT_SRC, typename FLOAT_DST>
void
test_cvt_sf_sf(FLOAT_SRC a, RoundingModeEnum rm)
{
    static_assert(sizeof(FLOAT_SRC) > sizeof(FLOAT_DST));
    SoftFPU fpu;
    fpu.set_rounding_mode(rm);
    FLOAT_DST const r = fpu.cvt_f_f<FLOAT_DST, FLOAT_SRC>(a, rm);
    u8 const fflags = fpu.get_flags();

    float_union<FLOAT_DST> ref;
    set_flags(rm);
    ref.f = (typename FLOAT_DST::F_HARD)a.hard();
    auto ref_fflags = get_flags();
    if (is_nan(ref.u))
        ref.u.v = FLOAT_DST::F_QNAN;

    if (r.v != ref.u.v || ref_fflags != fflags) {
        std::cout << "ERROR op=cvt_f_f (f"
                  << FLOAT_SRC::F_SIZE << " => f" << FLOAT_DST::F_SIZE
                  << ") rm=" << rm_to_str[rm] << fmt::endl;
        std::cout << "a  = " << print_sf(a) << fmt::endl;
        std::cout << "ref= " << print_sf(ref.u) << " fflags=0x" << fmt::hex(ref_fflags) << fmt::endl;
        std::cout << "r  = " << print_sf(r) << " fflags=0x" << fmt::hex(fflags) << fmt::endl;
    }
}

template<typename FLOAT_SRC, typename FLOAT_DST>
void
test_cvt_sf_sf(FLOAT_SRC a)
{
    static_assert(sizeof(FLOAT_SRC) < sizeof(FLOAT_DST));
    RoundingModeEnum const rm = RM_RNE;
    SoftFPU fpu;
    fpu.set_rounding_mode(rm);

    FLOAT_DST const r = fpu.extend<FLOAT_DST, FLOAT_SRC>(a);
    u8 const fflags = fpu.get_flags();

    float_union<FLOAT_DST> ref;
    set_flags(rm);
    ref.f = (typename FLOAT_DST::F_HARD)a.hard();
    u8 const ref_fflags = get_flags();
    if (is_nan(ref.u))
        ref.u.v = FLOAT_DST::F_QNAN;

    if (r.v != ref.u.v || ref_fflags != fflags) {
        std::cout << "ERROR op=cvt_f_f (f" << FLOAT_SRC::F_SIZE << " => f"
                  << FLOAT_DST::F_SIZE << ") rm=" << rm_to_str[rm] << fmt::endl;
        std::cout << "a  = " << print_sf(a) << fmt::endl;
        std::cout << "ref= " << print_sf(ref.u) << " fflags=0x" << fmt::hex(ref_fflags) << fmt::endl;
        std::cout << "r  = " << print_sf(r) << " fflags=0x" << fmt::hex(fflags) << fmt::endl;
        //std::cout << "DUMP:\n" << SoftFPU::debug_stream.str() << fmt::endl;
    }
}

template<typename FLOAT>
void
test_rint(FLOAT a, RoundingModeEnum rm)
{
    SoftFPU fpu;
    fpu.set_rounding_mode(rm);

    FLOAT const r = fpu.round_to_int<FLOAT>(a, rm);
    u8 const fflags = fpu.get_flags();

    float_union<FLOAT> ref;
    set_flags(rm);
    ref.f = rint(a.hard(), rm);
    u8 ref_fflags = get_flags();
    if (is_nan(ref.u))
        ref.u.v = FLOAT::F_QNAN;

    if (r.v != ref.u.v /*|| ref_fflags != fflags*/) {
        std::cout << "ERROR op=round_to_int (f" << FLOAT::F_SIZE
                  << ") rm=" << rm_to_str[rm] << fmt::endl;
        std::cout << "a  = " << print_sf(a) << fmt::endl;
        std::cout << "ref= " << print_sf(ref.u) << " fflags=0x" << fmt::hex(ref_fflags) << fmt::endl;
        std::cout << "r  = " << print_sf(r) << " fflags=0x" << fmt::hex(fflags) << fmt::endl;
    }
}

template<typename FLOAT, typename INTEGER>
void
test_cvt_int_sf(FLOAT a, RoundingModeEnum rm)
{
    SoftFPU fpu;
    fpu.set_rounding_mode(rm);

    INTEGER const r = fpu.cvt_f_i<INTEGER, FLOAT>(a, rm);
    u8 const fflags = fpu.get_flags();

    set_flags(rm);
    INTEGER const r_ref = cvt_int<INTEGER>(a.hard(), rm);
    u8 const ref_fflags = get_flags();

    if ((((ref_fflags | fflags) & FFLAG_INVALID) == 0 && r != r_ref) ||
        (sizeof(FLOAT) < 16U && (ref_fflags & FFLAG_INVALID) != (fflags & FFLAG_INVALID))) {
        std::cout << "ERROR op=cvt_f_i (f" << FLOAT::F_SIZE << " => i" << (8*sizeof(INTEGER))
                  << ") rm=" << rm_to_str[rm] << fmt::endl;
        std::cout << "a  = " << print_sf(a) << fmt::endl;
        std::cout << "ref= " << r_ref << " fflags=0x" << fmt::hex(ref_fflags) << fmt::endl;
        std::cout << "r  = " << r     << " fflags=0x" << fmt::hex(fflags) << fmt::endl;
    }
}

template<typename FLOAT>
void
report(std::ostream& out, FLOAT a, FLOAT b, FLOAT c, RoundingModeEnum rm, FLOAT ref, u8 ref_fflags, FLOAT r, u8 fflags)
{
    if (r.v != ref.v)
        out << "ERRVV";
    else if (ref_fflags != fflags)
        out << "ERRFF";

    out << " op=muladd size=" << FLOAT::F_SIZE
        << " rm=" << rm_to_str[rm] << fmt::endl;
    out << "a  = " << print_sf(a) << fmt::endl;
    out << "b  = " << print_sf(b) << fmt::endl;
    out << "c  = " << print_sf(c) << fmt::endl;
    out << "ref= " << print_sf(ref) << " fflags=0x" << fmt::hex(ref_fflags) << fmt::endl;
    out << "r  = " << print_sf(r  ) << " fflags=0x" << fmt::hex(fflags) << fmt::endl;
    //out << "DUMP:\n" << SoftFPU::debug_stream.str() << fmt::endl;
}

template<typename FLOAT>
void
test_fma(FLOAT a, FLOAT b, FLOAT c, RoundingModeEnum rm)
{
    float_union<FLOAT> ref;
    static_assert(sizeof(ref.u) == sizeof(ref.f));

    SoftFPU fpu;
    fpu.set_rounding_mode(rm);

    FLOAT const r = fpu.muladd(a, b, c, FLOAT_MULADD_NEGATE_NONE);
    u8 fflags = fpu.get_flags();

    u8 ref_fflags = 0;
    ref.f = muladd(a.hard(), b.hard(), c.hard(), rm, ref_fflags);
    if (is_nan(FLOAT{ref.u}))
        ref.u.v = FLOAT::F_QNAN;

    // workarounds for fmaq/mpfr_fma
    if constexpr (FLOAT::F_SIZE == 128) {
        // all NaNs are invalid op
        if (is_nan(FLOAT{ref.u})) {
            fflags |= FFLAG_INVALID;
        }
        // do not reporting underflow for subnormals
        if (ref.u.get_exp() == 0 && ref.u.get_fraction()) {
            ref_fflags |= FFLAG_UNDERFLOW;
        }
        // possibly wrong rounding in case of RDN/RUP between INF and MAXIMUM_NORMAL
        if (rm == RM_RDN &&
            r.get_sign() && is_inf(r) &&
            ref.u.v == SoftFPU::pack_sf<FLOAT>(1, FLOAT::EXP_MASK - 1, FLOAT::MANT_MASK).v) {
           return;
        }
        if (rm == RM_RUP &&
            ref.u.get_sign() && is_inf(ref.u) &&
            r.v == SoftFPU::pack_sf<FLOAT>(1, FLOAT::EXP_MASK - 1, FLOAT::MANT_MASK).v) {
           return;
        }
    }

    if (r.v != ref.u.v || ref_fflags != fflags) {
        report(std::cout, a, b, c, rm, ref.u, ref_fflags, r, fflags);
    }
}

template<typename FLOAT>
void
test_cmp(FLOAT a, FLOAT b)
{
    RoundingModeEnum const rm = RM_RNE;

    int r, ref;
    u8 fflags, ref_fflags;
    const char *op_str;

    SoftFPU fpu;
    fpu.set_rounding_mode(rm);

    r = fpu.compare_ordered_eq(a, b);
    fflags = fpu.get_flags();

    set_flags(rm);
    ref = (a.hard() == b.hard());
    ref_fflags = get_flags();
    if (r != ref || ref_fflags != fflags) {
        op_str = "eq_quiet";
        goto error;
    }

    fflags = 0;
    r = fpu.compare_ordered_ge(a, b);
    fflags = fpu.get_flags();

    set_flags(rm);
    ref = (a.hard() <= b.hard());
    ref_fflags = get_flags();

    if (r != ref || ref_fflags != fflags) {
        op_str = "le";
        goto error;
    }

    r = fpu.compare_ordered_lt(a, b);
    fflags = fpu.get_flags();

    set_flags(rm);
    ref = (a.hard() < b.hard());
    ref_fflags = get_flags();

    if (r != ref || ref_fflags != fflags) {
        op_str = "lt";
    error:
        std::cout << "ERROR op=" << op_str
                  << " size=" << FLOAT::F_SIZE
                  << fmt::endl;
        std::cout << "a  = " << print_sf(a) << fmt::endl;
        std::cout << "b  = " << print_sf(b) << fmt::endl;
        std::cout << "ref= " << ref << " fflags=0x" << fmt::hex(ref_fflags) << fmt::endl;
        std::cout << "r  = " << r   << " fflags=0x" << fmt::hex(fflags) << fmt::endl;
    }
}

template<typename FLOAT>
void
test_op_all(FLOAT a, FLOAT b, RoundingModeEnum rm)
{
    test_op(OP_FADD, a, b, rm);
    test_op(OP_FSUB, a, b, rm);
    test_op(OP_FMUL, a, b, rm);
    test_op(OP_FDIV, a, b, rm);
    test_op(OP_FSQRT, a, b, rm);
    //test_op(OP_FLOG2, a, b, rm);
    test_op(OP_FMIN, a, b, rm);
    test_op(OP_FMAX, a, b, rm);
    //test_op(OP_FEXP, a, b, rm);
    test_cmp(a, b);

    if constexpr (sizeof(FLOAT) > sizeof(f32 )) test_cvt_sf_sf<FLOAT, f32 >(a, rm);
    if constexpr (sizeof(FLOAT) > sizeof(f64 )) test_cvt_sf_sf<FLOAT, f64 >(a, rm);
    if constexpr (sizeof(FLOAT) > sizeof(f128)) test_cvt_sf_sf<FLOAT, f128>(a, rm);

    if constexpr (sizeof(FLOAT) < sizeof(f32 )) test_cvt_sf_sf<FLOAT, f32 >(a);
    if constexpr (sizeof(FLOAT) < sizeof(f64 )) test_cvt_sf_sf<FLOAT, f64 >(a);
    if constexpr (sizeof(FLOAT) < sizeof(f128)) test_cvt_sf_sf<FLOAT, f128>(a);

    test_rint(a, rm);

    test_cvt_int_sf<FLOAT, i32>(a, rm);
    test_cvt_int_sf<FLOAT, i64>(a, rm);
}

static const RoundingModeEnum rms[] = {
    RM_RNE, RM_RTZ, RM_RDN, RM_RUP,
    // Note: we cannot test RM_RMM, no hardware
    // RM_RMM,
};

template<typename FLOAT>
void
test_float_special(void)
{
    // test all simple cases
    for (int i = 0; i < SPECIAL_COUNT; i++) {
        FLOAT const a = special_sf<FLOAT>(i);
        for (int j = 0; j < SPECIAL_COUNT; j++) {
            FLOAT const b = special_sf<FLOAT>(j);
            for (RoundingModeEnum rm : rms) {
                test_op_all(a, b, rm);
                for (int k = 0; k < SPECIAL_COUNT; k++) {
                    if constexpr (sizeof(FLOAT) < 16) {
                        FLOAT const c = special_sf<FLOAT>(k);
                        test_fma(a, b, c, rm);
                    }
                }
            }
        }
    }
}

template<typename FLOAT>
void
test_float(size_t n)
{
    for (size_t i = 0; i < n; i++) {
        FLOAT const a = rrandom_sf<FLOAT>();
        FLOAT const b = rrandom_sf<FLOAT>();
        FLOAT const c = rrandom_sf<FLOAT>();
        for (RoundingModeEnum rm : rms) {
            test_op_all(a, b, rm);
            test_op_all(a, c, rm);
            test_op_all(b, a, rm);
            test_op_all(b, c, rm);
            test_op_all(c, a, rm);
            test_op_all(c, b, rm);

            test_fma(a, b, c, rm);
            test_fma(a, c, b, rm);
            test_fma(c, a, b, rm);
            test_fma(c, b, a, rm);
            test_fma(b, a, c, rm);
            test_fma(b, c, a, rm);
        }
    }
}

void test_fpu(size_t n)
{
    std::cerr << "Starting softfp test (stop with Ctrl-C)" << std::endl;
    test_float_special<f32>();
    test_float_special<f64>();
    test_float_special<f128>();
    for (size_t i = 0; i <= 100; i++) {
        test_float<f32>(n);
        test_float<f64>(n);
        test_float<f128>(n);
        std::cerr << (i%10) << std::flush;
    }
    std::cerr << std::endl;
}

void test(void)
{
    static const struct {
        RoundingModeEnum rm;
        f128 a, b, c;
    } tests[] = {
     { RM_RDN,
       FLOAT128_C(0xc5dbffffffffffff, 0xfffffff000000000),
       FLOAT128_C(0x7a34000000000000, 0x001fffffffffffff),
       FLOAT128_C(0x534fffffffffffff, 0xffffffc000000000),
     },
     { RM_RUP,
       FLOAT128_C(0x45dbffffffffffff, 0xfffffff000000000),
       FLOAT128_C(0xfa34000000000000, 0x001fffffffffffff),
       FLOAT128_C(0xd34fffffffffffff, 0xffffffc000000000),
     },
     { RM_RNE,
       FLOAT128_C(0x3c1bffffc0000000, 0x0000000000000000),
       FLOAT128_C(0x1d35ffffffffffff, 0xffffff0000000000),
       FLOAT128_C(0x1961ffffffffffff, 0xfffffffffff3ffff),
     },
    };

    for (auto const& test : tests) {
        SoftFPU fpu;
        fpu.set_rounding_mode(test.rm);
        f128 const r = fpu.muladd(test.a, test.b, test.c, FLOAT_MULADD_NEGATE_NONE);
        u8 const fflags = fpu.get_flags();
        set_flags(test.rm);
        float_union<f128> ref;
        u8 ref_fflags = 0;
        ref.f = muladd(test.a.hard(), test.b.hard(), test.c.hard(), test.rm, ref_fflags);
        report(std::cout, test.a, test.b, test.c, test.rm, ref.u, ref_fflags, r, fflags);
    }
}

} // namespace fpu
} // namespace postrisc

int main(int argc, char **argv)
{
    int seed = 1;
    if (argc >= 2)
        seed = atoi(argv[1]);
    srand(seed);

    postrisc::util::setup();
    postrisc::fpu::test_256();
    postrisc::fpu::test_scalar_uint();

    postrisc::fpu::test();
    postrisc::fpu::test_fpu(1000);

    return EXIT_SUCCESS;
}
