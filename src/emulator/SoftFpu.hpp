#pragma once

#include <bit>
#include <type_traits>

#include "util/common.hpp"
#include "util/fmt.hpp"

// this works at least with gcc
#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

namespace postrisc {
namespace fpu {

enum RoundingModeEnum {
    RM_RNE, /* Round to Nearest, ties to Even */
    RM_RDN, /* Round Down */
    RM_RUP, /* Round Up */
    RM_RTZ, /* Round towards Zero */
    RM_RMM, /* Round to Nearest, ties to Max Magnitude */
};

enum fp_flags : u8 {
    FFLAG_NOERROR         = 0,
    FFLAG_INVALID         = 1 << 0,
    FFLAG_DIVBYZERO       = 1 << 1,
    FFLAG_OVERFLOW        = 1 << 2,
    FFLAG_UNDERFLOW       = 1 << 3,
    FFLAG_INEXACT         = 1 << 4,
    FFLAG_ALL_EXCEPT      = FFLAG_INVALID | FFLAG_DIVBYZERO | FFLAG_OVERFLOW | FFLAG_UNDERFLOW | FFLAG_INEXACT
};

inline fp_flags operator | ( fp_flags a, fp_flags b ) { return fp_flags( u8(a) | u8(b) ); }
inline fp_flags operator & ( fp_flags a, fp_flags b ) { return fp_flags( u8(a) & u8(b) ); }
inline fp_flags operator ~ ( fp_flags a ) { return fp_flags( ~u8(a) ); }

enum fp_class {
    FCLASS_ZERO           = 1U << 0,
    FCLASS_SIGNED         = 1U << 1,
    FCLASS_INFINITE       = 1U << 2,
    FCLASS_NORMAL         = 1U << 3,
    FCLASS_SUBNORMAL      = 1U << 4,
    FCLASS_QUIET_NAN      = 1U << 5,
    FCLASS_SIGNALING_NAN  = 1U << 6,
    FCLASS_NAN            = FCLASS_QUIET_NAN | FCLASS_SIGNALING_NAN,
};

template<typename T> struct integer_traits;

template<> struct integer_traits<i16>  { using unsigned_type = u16;  static bool constexpr is_unsigned(void) { return false; } };
template<> struct integer_traits<u16>  { using unsigned_type = u16;  static bool constexpr is_unsigned(void) { return true;  } };
template<> struct integer_traits<i32>  { using unsigned_type = u32;  static bool constexpr is_unsigned(void) { return false; } };
template<> struct integer_traits<u32>  { using unsigned_type = u32;  static bool constexpr is_unsigned(void) { return true;  } };
template<> struct integer_traits<i64>  { using unsigned_type = u64;  static bool constexpr is_unsigned(void) { return false; } };
template<> struct integer_traits<u64>  { using unsigned_type = u64;  static bool constexpr is_unsigned(void) { return true;  } };
template<> struct integer_traits<i128> { using unsigned_type = u128; static bool constexpr is_unsigned(void) { return false; } };
template<> struct integer_traits<u128> { using unsigned_type = u128; static bool constexpr is_unsigned(void) { return true;  } };

ALWAYS_INLINE constexpr int clz(u16 a) { return std::countl_zero(a); }
ALWAYS_INLINE constexpr int clz(u32 a) { return std::countl_zero(a); }
ALWAYS_INLINE constexpr int clz(u64 a) { return std::countl_zero(a); }
ALWAYS_INLINE constexpr int clz(u128 a)
{
    if (a == 0) { return 128;
    } else {
        u64 const ah = a >> 64;
        return ah ? std::countl_zero(ah) : std::countl_zero((u64)a) + 64;
    }
}

#define FLOAT_BASE(FF_NAME, FF_SIZE, TEMPLATE_EXP_SIZE, FF_UINT, FF_FLOAT)                                           \
    using F_UINT = FF_UINT;                                                                                          \
    F_UINT v;                                                                                                        \
    using F_HARD = FF_FLOAT;                                                                                         \
    F_HARD hard(void) const { return std::bit_cast<F_HARD>(v); }                                                     \
    static FF_NAME soft(F_HARD f) { return FF_NAME{std::bit_cast<F_UINT>(f)}; }                                      \
    static constexpr const int F_SIZE = FF_SIZE;                                                                     \
    static constexpr const int EXP_SIZE = TEMPLATE_EXP_SIZE;                                                         \
    static constexpr const int MANT_SIZE = F_SIZE - EXP_SIZE - 1;                                                    \
    static constexpr const int EXP_MASK = (1 << EXP_SIZE) - 1;                                                       \
    static constexpr const int IMANT_SIZE = F_SIZE - 2; /* internal mantissa size */                                 \
    static constexpr const int RND_SIZE = IMANT_SIZE - MANT_SIZE;                                                    \
    static constexpr const int BIAS = (1 << (EXP_SIZE - 1)) - 1;                                                     \
    static constexpr const F_UINT MANT_MASK = (F_UINT{1} << MANT_SIZE) - 1;                                          \
    static constexpr const F_UINT SIGN_MASK = F_UINT{1} << (F_SIZE - 1);                                             \
    static constexpr const F_UINT QNAN_MASK = F_UINT{1} << (MANT_SIZE - 1);                                          \
    static constexpr const F_UINT F_QNAN = (F_UINT{EXP_MASK} << MANT_SIZE) | (F_UINT{1} << (MANT_SIZE - 1));         \
    static constexpr F_UINT make_bit(int bitnum) { return F_UINT{1} << bitnum; }                                     \
    u32 constexpr get_sign(void) const { return (u32)(v >> (F_SIZE - 1)); }                                          \
    i32 constexpr get_exp(void) const { return (u32)(v >> MANT_SIZE) & EXP_MASK; }                                   \
    F_UINT constexpr get_fraction(void) const { return v & MANT_MASK; }                                              \
    F_UINT constexpr val(void) const { return v; }

class f16 {
public:
    FLOAT_BASE(f16, 16, 5, u16, native::f16)
    using F_ULONG = u32;
};

#define FLOAT16_C(V) fpu::f16{ U16C(V) }

class f32 {
public:
    FLOAT_BASE(f32, 32, 8, u32, native::f32)
    using F_ULONG = u64;
};

#define FLOAT32_C(V) fpu::f32{ U32C(V) }

class f64 {
public:
    FLOAT_BASE(f64, 64, 11, u64, native::f64)
    using F_ULONG = u128;
};

#define FLOAT64_C(V) fpu::f64{ U64C(V) }

class f128 {
public:
    FLOAT_BASE(f128, 128, 15, u128, native::f128)
};

#define FLOAT128_C(HI, LO) fpu::f128{ (u128(U64C(HI)) << 64) | u128(U64C(LO)) }

template<typename T> struct fp_traits { static bool constexpr is_fp(void) { return false; }};
template<> struct fp_traits<f16>  { static bool constexpr is_fp(void) { return true; }};
template<> struct fp_traits<f32>  { static bool constexpr is_fp(void) { return true; }};
template<> struct fp_traits<f64>  { static bool constexpr is_fp(void) { return true; }};
template<> struct fp_traits<f128> { static bool constexpr is_fp(void) { return true; }};

enum muladd_negate_t : u8 {
    FLOAT_MULADD_NEGATE_NONE    = 0,
    FLOAT_MULADD_NEGATE_ADDEND  = 1,
    FLOAT_MULADD_NEGATE_PRODUCT = 2,
    FLOAT_MULADD_NEGATE_RESULT  = FLOAT_MULADD_NEGATE_ADDEND | FLOAT_MULADD_NEGATE_PRODUCT
};

class SoftFPU {
public:
    constexpr RoundingModeEnum get_rounding_mode(void) const { return m_rounding_mode; }
    void set_rounding_mode(RoundingModeEnum val) { m_rounding_mode = val; }
    constexpr fp_flags get_flags(void) const { return m_exception_flags; }
    void set_exception_flags(fp_flags val) { m_exception_flags = val; }
    void clear_flags(fp_flags mask = FFLAG_ALL_EXCEPT) { m_exception_flags = m_exception_flags & (~mask); }
    u64 get_control_word(void) const;
    void set_control_word(u64 word);
    void change_control_word(u64 and_mask, u64 or_mask);

    friend std::ostream& operator<<(std::ostream& out, const SoftFPU& fpu);

    template<typename Archive> void serialize(Archive& ar, const unsigned int version);

    template<typename FLOAT> FLOAT abs(FLOAT v);
    template<typename FLOAT> FLOAT chs(FLOAT v);
    template<typename FLOAT> FLOAT nabs(FLOAT v);
    template<typename FLOAT> FLOAT merge(FLOAT sign, FLOAT exponent, FLOAT mantissa);

    template<typename FLOAT> FLOAT add(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT sub(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT mul(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT div(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT min(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT max(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT muladd(FLOAT a, FLOAT b, FLOAT c, muladd_negate_t);
    template<typename FLOAT> FLOAT sqrt(FLOAT a);
    template<typename FLOAT> FLOAT rsqrt(FLOAT a);
    template<typename FLOAT> FLOAT log2(FLOAT a);
    template<typename FLOAT> FLOAT exp(FLOAT a);
    template<typename FLOAT> FLOAT scalbn(FLOAT a, int exp);
    template<typename FLOAT> FLOAT round_to_int(FLOAT a, RoundingModeEnum rm);

    template<typename FLOAT_DST, typename FLOAT_SRC> FLOAT_DST extend(FLOAT_SRC a);
    template<typename FLOAT_DST, typename FLOAT_SRC> FLOAT_DST cvt_f_f(FLOAT_SRC a, RoundingModeEnum rm);
    template<typename INTEGER, typename FLOAT> INTEGER cvt_f_i(FLOAT a, RoundingModeEnum rm);
    template<typename FLOAT, typename INTEGER> FLOAT cvt_i_f(INTEGER a, RoundingModeEnum rm);

    template<typename FLOAT> bool compare_ordered(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_ordered_eq(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_ordered_ne(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_ordered_lt(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_ordered_ge(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_unordered(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_unordered_eq(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_unordered_ne(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_unordered_lt(FLOAT a, FLOAT b);
    template<typename FLOAT> bool compare_unordered_ge(FLOAT a, FLOAT b);

    template<typename FLOAT> u32 classify(FLOAT a);

    template<typename FLOAT> static FLOAT pack_sf(u32 a_sign, u32 a_exp, typename FLOAT::F_UINT a_mant);

private:
    void raise(fp_flags flags) { m_exception_flags = (m_exception_flags | flags); }

    template<typename FLOAT> static bool is_both_zero(FLOAT a, FLOAT b) { return (( a.v | b.v ) << 1) == 0; }
    template<typename FLOAT> static FLOAT zero_sf(u32 a_sign);

    template<typename FLOAT> FLOAT round_pack_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_mant, RoundingModeEnum rm);
    template<typename FLOAT> FLOAT normalize_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_mant, RoundingModeEnum rm);
    template<typename FLOAT> FLOAT normalize2_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_mant1, typename FLOAT::F_UINT a_mant0, RoundingModeEnum rm);

private:
    RoundingModeEnum     m_rounding_mode = RM_RNE;
    fp_flags             m_exception_flags = FFLAG_NOERROR;
};

template<typename Archive>
void SoftFPU::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m_rounding_mode);
    ar & BOOST_SERIALIZATION_NVP(m_exception_flags);
}

enum {
  disp_rounding_mode       = 0,
  disp_flags               = 8,
};

inline u64
SoftFPU::get_control_word(void) const
{
    u64 word = 0;
    word |= static_cast<u64>(get_flags())                << disp_flags;
    word |= static_cast<u64>(get_rounding_mode())        << disp_rounding_mode;
    return word;
}

inline void SoftFPU::set_control_word(u64 word)
{
    set_exception_flags(static_cast<fp_flags>(util::lobits(word >> disp_flags, 8)));
    set_rounding_mode(static_cast<RoundingModeEnum>(util::lobits(word >> disp_rounding_mode, 2)));
}

// TODO - how to about local ieee flags? clear or keep?
inline void SoftFPU::change_control_word(u64 and_mask, u64 or_mask)
{
    const u64 mask = (U64C(1) << 7) - 1;
    u64 old = get_control_word();
    u64 tmp = old & ~mask;
    old = ((old & mask) | or_mask);
    tmp = tmp | (old & ~and_mask);
    set_control_word( tmp );
}

ALWAYS_INLINE std::ostream& operator<<(std::ostream& out, fp_flags flags)
{
    return out
    << static_cast<char>((flags & FFLAG_INVALID  ) ? 'V' : 'v')
    << static_cast<char>((flags & FFLAG_DIVBYZERO) ? 'Z' : 'z')
    << static_cast<char>((flags & FFLAG_OVERFLOW ) ? 'O' : 'o')
    << static_cast<char>((flags & FFLAG_UNDERFLOW) ? 'U' : 'u')
    << static_cast<char>((flags & FFLAG_INEXACT  ) ? 'X' : 'x');
}

inline std::ostream& operator<<(std::ostream& out, const SoftFPU& fpu)
{
    return out
    << fmt::hex(fpu.get_control_word())
    << " fe=" << fpu.get_flags()
    << " rm=" << int(fpu.get_rounding_mode());
}

template<typename FLOAT> inline FLOAT SoftFPU::pack_sf(u32 a_sign, u32 a_exp, typename FLOAT::F_UINT a_mant)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " mant=" << fmt::hex(a_mant);
    const typename FLOAT::F_UINT tmp = ((typename FLOAT::F_UINT)a_sign << (FLOAT::F_SIZE - 1)) |
        (((typename FLOAT::F_UINT)a_exp) << FLOAT::MANT_SIZE) | 
        (a_mant & (typename FLOAT::F_UINT)FLOAT::MANT_MASK);
    return FLOAT{tmp};
}

template<typename FLOAT> inline FLOAT SoftFPU::zero_sf(u32 sign)
{
    return pack_sf<FLOAT>(sign, 0, 0);
}

template<typename FLOAT>
typename FLOAT::F_UINT rshift_rnd(typename FLOAT::F_UINT a, int d)
{
    if (d != 0) {
        if (d >= FLOAT::F_SIZE) {
            a = !!a;
        } else {
            auto const mask = ((typename FLOAT::F_UINT)1 << d) - 1;
            a = (a >> d) | (!!(a & mask));
        }
    }
    return a;
}

template<typename FLOAT> FLOAT SoftFPU::abs(FLOAT a)
{
    const typename FLOAT::F_UINT tmp = a.v & ~(typename FLOAT::F_UINT{1} << (FLOAT::F_SIZE - 1));
    return FLOAT{tmp};
}

template<typename FLOAT> FLOAT SoftFPU::chs(FLOAT a)
{
    const typename FLOAT::F_UINT tmp = a.v ^ (typename FLOAT::F_UINT{1} << (FLOAT::F_SIZE - 1));
    return FLOAT{tmp};
}

template<typename FLOAT> FLOAT SoftFPU::nabs(FLOAT a)
{
    const typename FLOAT::F_UINT tmp = a.v | (typename FLOAT::F_UINT{1} << (FLOAT::F_SIZE - 1));
    return FLOAT{tmp};
}

template<typename FLOAT>
FLOAT
SoftFPU::merge(FLOAT sign, FLOAT exponent, FLOAT mantissa)
{
    const typename FLOAT::F_UINT tmp =
           (    sign.val() & (((typename FLOAT::F_UINT{1} << (FLOAT::F_SIZE - 1 ))    )                               )) |
           (mantissa.val() & (((typename FLOAT::F_UINT{1} << (FLOAT::MANT_SIZE  )) - 1)                               )) |
           (exponent.val() & (((typename FLOAT::F_UINT{1} << (FLOAT::EXP_SIZE   )) - 1) << FLOAT::MANT_SIZE));
    return FLOAT{tmp};
}

/* a_mant is considered to have its MSB at F_SIZE - 2 bits */
template<typename FLOAT>
FLOAT
SoftFPU::round_pack_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_mant, RoundingModeEnum rm)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " mant=" << fmt::hex(a_mant);

    //LOG_OUT << "s=" << a_sign
    //        << " exp=" << a_exp
    //        << " a_mant=" << fmt::hex(a_mant)
    //        << fmt::endl;

    u32 addend, rnd_bits;

    switch (rm) {
    case RM_RNE:
    case RM_RMM:
        addend = (1U << (FLOAT::RND_SIZE - 1));
        break;
    case RM_RTZ:
        addend = 0;
        break;
    default:
    case RM_RDN:
    case RM_RUP:
        if (a_sign ^ (rm == RM_RUP)) // rm & 1
            addend = (1U << FLOAT::RND_SIZE) - 1;
        else
            addend = 0;
        break;
    }

    // potentially subnormal
    if (a_exp <= 0) {
        // Note: we set the underflow flag if the rounded result is subnormal and inexact
        bool const is_subnormal = (a_exp < 0 || 
              (a_mant + addend) < ((typename FLOAT::F_UINT)1 << (FLOAT::F_SIZE - 1)));
        int const diff = 1 - a_exp;
        a_mant = rshift_rnd<FLOAT>(a_mant, diff);
        rnd_bits = (u32)a_mant & ((1U << FLOAT::RND_SIZE) - 1);
        if (is_subnormal && rnd_bits != 0) {
            raise(FFLAG_UNDERFLOW);
        }
        a_exp = 1;
    } else {
        rnd_bits = (u32)a_mant & ((1U << FLOAT::RND_SIZE) - 1);
    }
    if (rnd_bits != 0)
        raise(FFLAG_INEXACT);
    a_mant = (a_mant + (typename FLOAT::F_UINT)addend) >> FLOAT::RND_SIZE;
    // half way: select even result
    if (rm == RM_RNE && rnd_bits == (1U << (FLOAT::RND_SIZE - 1U)))
        a_mant &= ~(typename FLOAT::F_UINT)1;
    // Note the rounding adds at least 1, so this is the maximum value
    a_exp += (int)(a_mant >> (FLOAT::MANT_SIZE + 1));
    if (a_mant <= FLOAT::MANT_MASK) {
        // denormalized or zero
        a_exp = 0;
    } else if (a_exp >= FLOAT::EXP_MASK) {
        // overflow
        //LOG_OUT << "OVERFLOW: s=" << a_sign
        //        << " exp=" << a_exp
        //        << " addend=" << addend
        //        << fmt::endl;
        if (addend == 0) {
            //LOG_OUT << "switch to largest normal" << fmt::endl;
            a_exp = FLOAT::EXP_MASK - 1;
            a_mant = FLOAT::MANT_MASK;
        } else {
            // infinity
            //LOG_OUT << "switch to infinity" << fmt::endl;
            a_exp = FLOAT::EXP_MASK;
            a_mant = 0;
        }
        raise(FFLAG_OVERFLOW | FFLAG_INEXACT);
    }
    return pack_sf<FLOAT>(a_sign, a_exp, a_mant);
}

/* a_mant is considered to have at most F_SIZE - 1 bits */
template<typename FLOAT>
FLOAT
SoftFPU::normalize_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_mant, RoundingModeEnum rm)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " mant=" << fmt::hex(a_mant);

    int const shift = clz(a_mant) - (FLOAT::F_SIZE - 1 - FLOAT::IMANT_SIZE);
    assert(shift >= 0);
    a_exp -= shift;
    a_mant <<= shift;
    return round_pack_sf<FLOAT>(a_sign, a_exp, a_mant, rm);
}

/* same as normalize_sf() but with a double word mantissa. a_mant1 is
   considered to have at most F_SIZE - 1 bits */
template<typename FLOAT>
FLOAT
SoftFPU::normalize2_sf(u32 a_sign, int a_exp,
    typename FLOAT::F_UINT a_mant1, typename FLOAT::F_UINT a_mant0, RoundingModeEnum rm)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp
        << " mant=" << fmt::hex(a_mant1) << '_' << fmt::hex(a_mant0);

    //LOG_OUT << "s=" << a_sign
    //        << " exp=" << a_exp
    //        << " a_mant=" << fmt::hex(a_mant1) << ' ' << fmt::hex(a_mant0)
    //        << fmt::endl;

    int const l = a_mant1 ? clz(a_mant1) : FLOAT::F_SIZE + clz(a_mant0);
    int const shift = l - (FLOAT::F_SIZE - 1 - FLOAT::IMANT_SIZE);
    assert(shift >= 0);
    a_exp -= shift;
    if (shift == 0) {
        a_mant1 |= (a_mant0 != 0);
    } else if (shift < FLOAT::F_SIZE) {
        a_mant1 = (a_mant1 << shift) | (a_mant0 >> (FLOAT::F_SIZE - shift));
        a_mant0 <<= shift;
        a_mant1 |= (a_mant0 != 0);
    } else {
        a_mant1 = a_mant0 << (shift - FLOAT::F_SIZE);
    }
    return round_pack_sf<FLOAT>(a_sign, a_exp, a_mant1, rm);
}

template<typename FLOAT> bool is_inf(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_mant = a.get_fraction();
    return (a_exp == FLOAT::EXP_MASK && !a_mant);
}

template<typename FLOAT> bool is_nan(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_mant = a.get_fraction();
    return (a_exp == FLOAT::EXP_MASK && !!a_mant);
}

template<typename FLOAT> bool is_signan(FLOAT a)
{
    u32 const a_exp1 = u32(a.v >> (FLOAT::MANT_SIZE - 1)) & ((1U << (FLOAT::EXP_SIZE + 1)) - 1);
    auto const a_mant = a.get_fraction();
    return (a_exp1 == (2 * FLOAT::EXP_MASK) && !!a_mant);
}

template<typename FLOAT> FLOAT SoftFPU::add(FLOAT a, FLOAT b)
{
    // swap so that abs(a) >= abs(b)
    if ((a.v & ~FLOAT::SIGN_MASK) < (b.v & ~FLOAT::SIGN_MASK)) {
        std::swap(a,b);
    }

    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto a_exp  = a.get_exp();
    auto b_exp  = b.get_exp();
    auto a_mant = a.get_fraction() << 3;
    auto b_mant = b.get_fraction() << 3;

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_mant) {
            // NaN result
            if (!(a_mant & (FLOAT::QNAN_MASK << 3)) || is_signan(b))
                raise(FFLAG_INVALID);
            return FLOAT{FLOAT::F_QNAN};
        } else if (b_exp == FLOAT::EXP_MASK && a_sign != b_sign) {
            raise(FFLAG_INVALID);
            return FLOAT{FLOAT::F_QNAN};
        } else {
            /* infinity */
            return a;
        }
    }

    if (a_exp == 0) {
        a_exp = 1;
    } else {
        a_mant |= (typename FLOAT::F_UINT)1 << (FLOAT::MANT_SIZE + 3);
    }
    if (b_exp == 0) {
        b_exp = 1;
    } else {
        b_mant |= (typename FLOAT::F_UINT)1 << (FLOAT::MANT_SIZE + 3);
    }

    b_mant = rshift_rnd<FLOAT>(b_mant, a_exp - b_exp);
    if (a_sign == b_sign) {
        // same signs: add the absolute values
        a_mant += b_mant;
    } else {
        // different signs: subtract the absolute values
        a_mant -= b_mant;
        if (a_mant == 0) {
            // zero result: the sign needs a specific handling
            a_sign = (get_rounding_mode() == RM_RDN);
        }
    }
    a_exp += (FLOAT::RND_SIZE - 3);
    return normalize_sf<FLOAT>(a_sign, a_exp, a_mant, get_rounding_mode());
}

template<typename FLOAT> FLOAT SoftFPU::sub(FLOAT a, FLOAT b)
{
    b.v ^= FLOAT::SIGN_MASK;
    return add(a, b);
}

template<typename FLOAT>
inline
typename FLOAT::F_UINT normalize_subnormal_sf(i32 *pa_exp, typename FLOAT::F_UINT a_mant)
{
    int const shift = FLOAT::MANT_SIZE - ((FLOAT::F_SIZE - 1 - clz(a_mant)));
    *pa_exp = 1 - shift;
    return a_mant << shift;
}

template<typename F_UINT> std::tuple<F_UINT, F_UINT> mul_u(F_UINT a, F_UINT b);

template<> inline std::tuple<u16, u16> mul_u(u16 a, u16 b)
{
    u32 r = (u32)a * (u32)b;
    return {r >> 16, r};
}

template<> inline std::tuple<u32, u32> mul_u(u32 a, u32 b)
{
    u64 r = (u64)a * (u64)b;
    return {r >> 32, r};
}

template<> inline std::tuple<u64, u64> mul_u(u64 a, u64 b)
{
    u128 r = (u128)a * (u128)b;
    return {r >> 64, r};
}

template<> inline std::tuple<u128, u128> mul_u(u128 a, u128 b)
{
    using F_UINT = u128;
    using F_UHALF = u64;

    size_t const FH_SIZE  = 8 * sizeof(F_UHALF);

    F_UHALF const a0 = a;
    F_UHALF const a1 = a >> FH_SIZE;
    F_UHALF const b0 = b;
    F_UHALF const b1 = b >> FH_SIZE;

    F_UINT const r00 = (F_UINT)a0 * (F_UINT)b0;
    F_UINT const r01 = (F_UINT)a0 * (F_UINT)b1;
    F_UINT const r10 = (F_UINT)a1 * (F_UINT)b0;
    F_UINT const r11 = (F_UINT)a1 * (F_UINT)b1;

    F_UHALF const r0 = r00;
    F_UINT c = (r00 >> FH_SIZE) + (F_UHALF)r01 + (F_UHALF)r10;
    F_UHALF const r1 = c;
    c = (c >> FH_SIZE) + (r01 >> FH_SIZE) + (r10 >> FH_SIZE) + (F_UHALF)r11;
    F_UHALF const r2 = c;
    F_UHALF const r3 = (c >> FH_SIZE) + (r11 >> FH_SIZE);

    return  { ((F_UINT)r3 << FH_SIZE) | r2, ((F_UINT)r1 << FH_SIZE) | r0 };
}

template<typename FLOAT> FLOAT SoftFPU::mul(FLOAT a, FLOAT b)
{
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto r_sign = a_sign ^ b_sign;
    auto a_exp = a.get_exp();
    auto b_exp = b.get_exp();
    auto a_mant = a.get_fraction();
    auto b_mant = b.get_fraction();

    if (a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else {
            // infinity
            if ((a_exp == FLOAT::EXP_MASK && (b_exp == 0 && b_mant == 0)) ||
                (b_exp == FLOAT::EXP_MASK && (a_exp == 0 && a_mant == 0))) {
                raise(FFLAG_INVALID);
                return FLOAT{FLOAT::F_QNAN};
            } else {
                return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
            }
        }
    }
    if (a_exp == 0) {
        if (a_mant == 0)
            return zero_sf<FLOAT>(r_sign);
        a_mant = normalize_subnormal_sf<FLOAT>(&a_exp, a_mant);
    } else {
        a_mant |= (typename FLOAT::F_UINT)1 << FLOAT::MANT_SIZE;
    }
    if (b_exp == 0) {
        if (b_mant == 0)
            return zero_sf<FLOAT>(r_sign);
        b_mant = normalize_subnormal_sf<FLOAT>(&b_exp, b_mant);
    } else {
        b_mant |= (typename FLOAT::F_UINT)1 << FLOAT::MANT_SIZE;
    }
    int r_exp = a_exp + b_exp - FLOAT::BIAS + 1; //(1 << (FLOAT::EXP_SIZE - 1)) + 2;

    auto [r_mant, r_mant_low] = mul_u<typename FLOAT::F_UINT>(a_mant << FLOAT::RND_SIZE, b_mant << (FLOAT::RND_SIZE + 1));
    r_mant |= !!r_mant_low; // != 0
    return normalize_sf<FLOAT>(r_sign, r_exp, r_mant, get_rounding_mode());
}

template<typename FLOAT>
FLOAT
SoftFPU::muladd(FLOAT a, FLOAT b, FLOAT c, muladd_negate_t negate_mask)
{
    using F_UINT = typename FLOAT::F_UINT;
    F_UINT c_mant1, c_mant0;

    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto c_sign = c.get_sign();

    auto r_sign = a_sign ^ b_sign;
    if (negate_mask & FLOAT_MULADD_NEGATE_PRODUCT) {
        r_sign ^= 1;
    }
    if (negate_mask & FLOAT_MULADD_NEGATE_ADDEND) {
        c_sign ^= 1;
    }

    auto a_exp = a.get_exp();
    auto b_exp = b.get_exp();
    auto c_exp = c.get_exp();
    auto a_mant = a.get_fraction();
    auto b_mant = b.get_fraction();
    auto c_mant = c.get_fraction();

    if (a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK || c_exp == FLOAT::EXP_MASK) {
        if (is_nan(a) || is_nan(b) || is_nan(c)) {
            if (is_signan(a) || is_signan(b) || is_signan(c)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else {
            /* infinities */
            if ((a_exp == FLOAT::EXP_MASK && (b_exp == 0 && b_mant == 0)) ||
                (b_exp == FLOAT::EXP_MASK && (a_exp == 0 && a_mant == 0)) ||
                ((a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK) &&
                 (c_exp == FLOAT::EXP_MASK && r_sign != c_sign)))
            {
                raise(FFLAG_INVALID);
                return FLOAT{FLOAT::F_QNAN};
            } else if (c_exp == FLOAT::EXP_MASK) {
                return pack_sf<FLOAT>(c_sign, FLOAT::EXP_MASK, 0);
            } else {
                return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
            }
        }
    }
    if (a_exp == 0) {
        if (a_mant == 0)
            goto mul_zero;
        a_mant = normalize_subnormal_sf<FLOAT>(&a_exp, a_mant);
    } else {
        a_mant |= F_UINT{1} << FLOAT::MANT_SIZE;
    }
    if (b_exp == 0) {
        if (b_mant == 0) {
        mul_zero:
            if (c_exp == 0 && c_mant == 0) {
                if (c_sign != r_sign)
                    r_sign = (get_rounding_mode() == RM_RDN);
                return zero_sf<FLOAT>(r_sign);
            } else {
                return c;
            }
        }
        b_mant = normalize_subnormal_sf<FLOAT>(&b_exp, b_mant);
    } else {
        b_mant |= F_UINT{1} << FLOAT::MANT_SIZE;
    }

    // multiply
    auto r_exp = a_exp + b_exp - FLOAT::BIAS + 2; //(1 << (FLOAT::EXP_SIZE - 1)) + 3;

    auto [r_mant1, r_mant0] = mul_u<F_UINT>(a_mant << FLOAT::RND_SIZE, b_mant << FLOAT::RND_SIZE);
    // normalize to F_SIZE - 3
    if (r_mant1 < (F_UINT{1} << (FLOAT::F_SIZE - 3))) {
        r_mant1 = (r_mant1 << 1) | (r_mant0 >> (FLOAT::F_SIZE - 1));
        r_mant0 <<= 1;
        r_exp--;
    }

    // add
    if (c_exp == 0) {
        if (c_mant == 0) {
            // add zero
            r_mant1 |= (r_mant0 != 0);
            return normalize_sf<FLOAT>(r_sign, r_exp, r_mant1, get_rounding_mode());
        }
        c_mant = normalize_subnormal_sf<FLOAT>(&c_exp, c_mant);
    } else {
        c_mant |= F_UINT{1} << FLOAT::MANT_SIZE;
    }
    c_exp++;
    c_mant1 = c_mant << (FLOAT::RND_SIZE - 1);
    c_mant0 = 0;

    //    printf("r_s=%d r_exp=%d r_mant=%08x %08x\n", r_sign, r_exp, (u32)r_mant1, (u32)r_mant0);
    //    printf("c_s=%d c_exp=%d c_mant=%08x %08x\n", c_sign, c_exp, (u32)c_mant1, (u32)c_mant0);

    // ensure that abs(r) >= abs(c)
    if (!(r_exp > c_exp || (r_exp == c_exp && r_mant1 >= c_mant1))) {
        std::swap(r_mant1, c_mant1);
        std::swap(r_mant0, c_mant0);
        std::swap(r_exp, c_exp);
        std::swap(r_sign, c_sign);
    }

    // right shift c_mant
    int const shift = r_exp - c_exp;
    if (shift >= 2 * FLOAT::F_SIZE) {
        c_mant0 = (c_mant0 | c_mant1) != 0;
        c_mant1 = 0;
    } else if (shift >= FLOAT::F_SIZE + 1) {
        c_mant0 = rshift_rnd<FLOAT>(c_mant1, shift - FLOAT::F_SIZE);
        c_mant1 = 0;
    } else if (shift == FLOAT::F_SIZE) {
        c_mant0 = c_mant1 | (c_mant0 != 0);
        c_mant1 = 0;
    } else if (shift != 0) {
        F_UINT const mask = (F_UINT{1} << shift) - 1;
        c_mant0 = (c_mant1 << (FLOAT::F_SIZE - shift)) | (c_mant0 >> shift) | ((c_mant0 & mask) != 0);
        c_mant1 = c_mant1 >> shift;
    }

    //    printf("  r_mant=%08x %08x\n", (u32)r_mant1, (u32)r_mant0);
    //    printf("  c_mant=%08x %08x\n", (u32)c_mant1, (u32)c_mant0);

    // add or subtract
    if (r_sign == c_sign) {
        r_mant0 += c_mant0;
        r_mant1 += c_mant1 + (r_mant0 < c_mant0);
    } else {
        F_UINT tmp = r_mant0;
        r_mant0 -= c_mant0;
        r_mant1 = r_mant1 - c_mant1 - (r_mant0 > tmp);
        if ((r_mant0 | r_mant1) == 0) {
            /* zero result : the sign needs a specific handling */
            r_sign = (get_rounding_mode() == RM_RDN);
        }
    }
#if 0
    //    printf("  r1_mant=%08x %08x\n", (u32)r_mant1, (u32)r_mant0);
    /* normalize */
    if (r_mant1 == 0) {
        r_mant1 = r_mant0;
        r_exp -= F_SIZE;
    } else {
        shift = clz(r_mant1) - (F_SIZE - 1 - IMANT_SIZE);
        if (shift != 0) {
            r_mant1 = (r_mant1 << shift) | (r_mant0 >> (F_SIZE - shift));
            r_mant0 <<= shift;
            r_exp -= shift;
        }
        r_mant1 |= (r_mant0 != 0);
    }
    return normalize_sf<FLOAT>(r_sign, r_exp, r_mant1, get_rounding_mode());
#else
    return normalize2_sf<FLOAT>(r_sign, r_exp, r_mant1, r_mant0, get_rounding_mode());
#endif
}

// FIXME: optimize u128
template<typename F_UINT> std::tuple<F_UINT, F_UINT> divrem_u(F_UINT a1, F_UINT a0, F_UINT b)
{
    size_t const F_SIZE = 8 * sizeof(F_UINT);

    assert(a1 < b);
    for (size_t i = 0; i < F_SIZE; i++) {
        int qb, ab;
        ab = (int)(a1 >> (F_SIZE - 1));
        a1 = (a1 << 1) | (a0 >> (F_SIZE - 1));
        if (ab || a1 >= b) {
            a1 -= b;
            qb = 1;
        } else {
            qb = 0;
        }
        a0 = (a0 << 1) | qb;
    }
    return {a0, a1};
}

template<> inline std::tuple<u16, u16> divrem_u(u16 ah, u16 al, u16 b)
{
    u32 a = ((u32)ah << 16) | al;
    return {a / b, a % b};
}

template<> inline std::tuple<u32, u32> divrem_u(u32 ah, u32 al, u32 b)
{
    u64 a = ((u64)ah << 32) | al;
    return {a / b,  a % b};
}

template<> inline std::tuple<u64, u64> divrem_u(u64 ah, u64 al, u64 b)
{
    u128 a = ((u128)ah << 64) | al;
    return {a / b, a % b};
}

template<typename FLOAT> FLOAT SoftFPU::div(FLOAT a, FLOAT b)
{
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto r_sign = a_sign ^ b_sign;
    auto a_exp = a.get_exp();
    auto b_exp = b.get_exp();
    auto a_mant = a.get_fraction();
    auto b_mant = b.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_mant || is_nan(b)) {
            if (is_signan(a) || is_signan(b)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else if (b_exp == FLOAT::EXP_MASK) {
            raise(FFLAG_INVALID);
            return FLOAT{FLOAT::F_QNAN};
        } else {
            return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
        }
    } else if (b_exp == FLOAT::EXP_MASK) {
        if (b_mant) {
            if (is_signan(a) || is_signan(b)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else {
            return zero_sf<FLOAT>(r_sign);
        }
    }

    if (b_exp == 0) {
        if (!b_mant) {
            /* zero */
            if (a_exp == 0 && !a_mant) {
                raise(FFLAG_INVALID);
                return FLOAT{FLOAT::F_QNAN};
            } else {
                raise(FFLAG_DIVBYZERO);
                return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
            }
        }
        b_mant = normalize_subnormal_sf<FLOAT>(&b_exp, b_mant);
    } else {
        b_mant |= (typename FLOAT::F_UINT)1 << FLOAT::MANT_SIZE;
    }
    if (a_exp == 0) {
        if (!a_mant)
            return zero_sf<FLOAT>(r_sign);
        a_mant = normalize_subnormal_sf<FLOAT>(&a_exp, a_mant);
    } else {
        a_mant |= (typename FLOAT::F_UINT)1 << FLOAT::MANT_SIZE;
    }
    i32 r_exp = a_exp - b_exp + FLOAT::BIAS; //(1 << (FLOAT::EXP_SIZE - 1)) - 1;
    auto [r_mant, r] = divrem_u<typename FLOAT::F_UINT>(a_mant, 0, b_mant << 2);
    if (r != 0)
        r_mant |= 1;
    return normalize_sf<FLOAT>(r_sign, r_exp, r_mant, get_rounding_mode());
}

#ifdef F_ULONG

// compute sqrt(a) with a = ah*2^F_SIZE+al and a < 2^(F_SIZE - 2)
// return true if not exact square.
static int sqrtrem_u(F_UINT *pr, F_UINT ah, F_UINT al)
{
    F_ULONG a, u, s;
    int l, inexact;

    /* 2^l >= a */
    if (ah != 0) {
        l = 2 * F_SIZE - clz(ah - 1);
    } else {
        if (al == 0) {
            *pr = 0;
            return 0;
        }
        l = F_SIZE - clz(al - 1);
    }
    a = ((F_ULONG)ah << F_SIZE) | al;
    u = (F_ULONG)1 << ((l + 1) / 2);
    for(;;) {
        s = u;
        u = ((a / s) + s) / 2;
        if (u >= s)
            break;
    }
    inexact = (a - s * s) != 0;
    *pr = s;
    return inexact;
}

#else

template<typename FLOAT>
int
sqrtrem_u(typename FLOAT::F_UINT *pr, typename FLOAT::F_UINT a1, typename FLOAT::F_UINT a0)
{
    size_t const F_SIZE = 8 * sizeof(typename FLOAT::F_UINT);
    int l, inexact;
    typename FLOAT::F_UINT u, s;

    /* 2^l >= a */
    if (a1 != 0) {
        l = 2 * F_SIZE - clz(static_cast<typename FLOAT::F_UINT>(a1 - 1));
    } else {
        if (a0 == 0) {
            *pr = 0;
            return 0;
        }
        l = F_SIZE - clz(static_cast<typename FLOAT::F_UINT>(a0 - 1));
    }
    u = (typename FLOAT::F_UINT)1 << ((l + 1) / 2);
    for(;;) {
        s = u;
        const auto [q, r] = divrem_u(a1, a0, s);
        u = (q + s) / 2;
        if (u >= s)
            break;
    }
    auto [sq1, sq0] = mul_u<typename FLOAT::F_UINT>(s, s);
    inexact = (sq0 != a0 || sq1 != a1);
    *pr = s;
    return inexact;
}

#endif

template<typename FLOAT> FLOAT SoftFPU::sqrt(FLOAT a)
{
    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_mant != 0) {
            if (is_signan(a)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else if (a_sign) {
            // raise(FFLAG_INVALID); // FIXME-SQRT-NEG
            return FLOAT{FLOAT::F_QNAN};
        } else {
            return a; /* +infinity */
        }
    }
    if (a_sign) {
        if (a_exp == 0 && !a_mant)
            return a; /* -zero */
        //raise(FFLAG_INVALID); // FIXME-SQRT-NEG
        return FLOAT{FLOAT::F_QNAN};
    }
    if (a_exp == 0) {
        if (a_mant == 0)
            return zero_sf<FLOAT>(0);
        a_mant = normalize_subnormal_sf<FLOAT>(&a_exp, a_mant);
    } else {
        a_mant |= (typename FLOAT::F_UINT)1 << FLOAT::MANT_SIZE;
    }
    a_exp -= FLOAT::BIAS;
    /* simpler to handle an even exponent */
    if (a_exp & 1) {
        a_exp--;
        a_mant <<= 1;
    }
    a_exp = (a_exp >> 1) + FLOAT::BIAS;
    a_mant <<= (FLOAT::F_SIZE - 4 - FLOAT::MANT_SIZE);
    if (sqrtrem_u<FLOAT>(&a_mant, a_mant, 0))
        a_mant |= 1;
    return normalize_sf<FLOAT>(a_sign, a_exp, a_mant, get_rounding_mode());
}

template<typename FLOAT> FLOAT SoftFPU::rsqrt(FLOAT a)
{
    FLOAT one = FLOAT{ typename FLOAT::F_UINT{FLOAT::BIAS} << FLOAT::MANT_SIZE };
    return div( one, sqrt(a));
}

/* comparisons */

template<typename FLOAT> FLOAT SoftFPU::min(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
            return FLOAT{FLOAT::F_QNAN};
        } else if (is_nan(a)) {
            if (is_nan(b)) 
                return FLOAT{FLOAT::F_QNAN};
            else
                return b;
        } else {
            return a;
        }
    }
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (a_sign)
            return a;
        else
            return b;
    } else {
        if ((a.v < b.v) ^ a_sign)
            return a;
        else
            return b;
    }
}

template<typename FLOAT> FLOAT SoftFPU::max(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
            return FLOAT{FLOAT::F_QNAN};
        } else if (is_nan(a)) {
            if (is_nan(b)) 
                return FLOAT{FLOAT::F_QNAN};
            else
                return b;
        } else {
            return a;
        }
    }
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (a_sign)
            return b;
        else
            return a;
    } else {
        if ((a.v < b.v) ^ a_sign)
            return b;
        else
            return a;
    }
}

template<typename FLOAT> u32 SoftFPU::classify(FLOAT a)
{
    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_mant = a.get_fraction();

    u32 ret = 0;
    if (a_sign) {
        ret |= FCLASS_SIGNED;
    }
    if (a_exp == FLOAT::EXP_MASK) {
        if (a_mant) {
            if (a_mant & FLOAT::QNAN_MASK)
                ret |= FCLASS_QUIET_NAN;
            else
                ret |= FCLASS_SIGNALING_NAN;
        } else {
            ret |= FCLASS_INFINITE;
        }
    } else if (a_exp == 0) {
        if (!a_mant) {
            ret |= FCLASS_ZERO;
        } else {
            ret |= FCLASS_SUBNORMAL;
        }
    } else {
        ret |= FCLASS_NORMAL;
    }
    return ret;
}

// conversions between floats (to bigger type)
template<typename FLOAT_DST, typename FLOAT_SRC>
FLOAT_DST
SoftFPU::extend(FLOAT_SRC a)
{
    static_assert(fp_traits<FLOAT_SRC>::is_fp());
    static_assert(fp_traits<FLOAT_DST>::is_fp());
    static_assert(sizeof(FLOAT_SRC) < sizeof(FLOAT_DST));

    auto a_sign = a.get_sign();
    auto a_exp  = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT_SRC::EXP_MASK) {
        if (a_mant) {
            // NaN
            if (is_signan(a)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT_DST{FLOAT_DST::F_QNAN};
        } else {
            // infinity
            return pack_sf<FLOAT_DST>(a_sign, FLOAT_DST::EXP_MASK, 0);
        }
    }
    if (a_exp == 0) {
        if (!a_mant)
            return pack_sf<FLOAT_DST>(a_sign, 0, a_mant); /* zero */
        a_mant = normalize_subnormal_sf<FLOAT_SRC>(&a_exp, a_mant);
    }
    // convert the exponent value
    a_exp = a_exp - FLOAT_SRC::BIAS/*0x7f*/ + FLOAT_DST::BIAS;
    // shift the mantissa
    typename FLOAT_DST::F_UINT adst_mant = ((typename FLOAT_DST::F_UINT)a_mant) << (FLOAT_DST::MANT_SIZE - FLOAT_SRC::MANT_SIZE); //23
    // We assume the target float is large enough to that no normalization is necessary
    return pack_sf<FLOAT_DST>(a_sign, a_exp, adst_mant);
}

template<typename FLOAT_DST, typename FLOAT_SRC>
FLOAT_DST
SoftFPU::cvt_f_f(FLOAT_SRC a, RoundingModeEnum rm)
{
    static_assert(sizeof(FLOAT_SRC) > sizeof(FLOAT_DST));

    auto a_sign = a.get_sign();
    auto a_exp  = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT_SRC::EXP_MASK) {
        if (a_mant) {
            // NaN
            if (is_signan(a)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT_DST{FLOAT_DST::F_QNAN};
        } else {
            // infinity
            return pack_sf<FLOAT_DST>(a_sign, FLOAT_DST::EXP_MASK, 0);
        }
    }
    if (a_exp == 0) {
        if (!a_mant)
            return zero_sf<FLOAT_DST>(a_sign);
        normalize_subnormal_sf<FLOAT_DST>(&a_exp, (typename FLOAT_DST::F_UINT)a_mant); // don't update a_mant here!
    } else {
        a_mant |= (typename FLOAT_SRC::F_UINT)1 << FLOAT_SRC::MANT_SIZE;
    }
    // convert the exponent value
    a_exp = a_exp - FLOAT_SRC::BIAS + FLOAT_DST::BIAS;
    // shift the mantissa
    a_mant = rshift_rnd<FLOAT_SRC>(a_mant, FLOAT_SRC::MANT_SIZE - (8*sizeof(FLOAT_DST)/*32*/ - 2));
    return normalize_sf<FLOAT_DST>(a_sign, a_exp, (typename FLOAT_DST::F_UINT)a_mant, rm);
}

/*----------------------------------------------------------------------------
| Rounds the floating-point value `a' to an integer, and
| returns the result as a floating-point value.
*----------------------------------------------------------------------------*/
template<typename FLOAT>
FLOAT
SoftFPU::round_to_int(FLOAT a, RoundingModeEnum rm)
{
    auto a_sign = a.get_sign();
    auto aExp   = a.get_exp();
    auto aSig   = a.get_fraction();

    if (aExp == FLOAT::EXP_MASK) {
        if (aSig) {
            // NaN
            if (is_signan(a)) {
                raise(FFLAG_INVALID);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else {
            // infinity
            return pack_sf<FLOAT>(a_sign, FLOAT::EXP_MASK, 0);
        }
    }

    if ( (FLOAT::BIAS + FLOAT::MANT_SIZE) <= aExp) {
        if ((aExp == FLOAT::EXP_MASK) && aSig) {
            return FLOAT{FLOAT::F_QNAN};
        }
        return a;
    }
    if (aExp < FLOAT::BIAS) {
        if ((a.v << 1) == 0) return a;
        raise(FFLAG_INEXACT);
        switch (rm) {
         case RM_RNE:
             if ((aExp == FLOAT::BIAS - 1) && aSig) {
                 return pack_sf<FLOAT>(a_sign, FLOAT::BIAS, 0);
             }
             break;
         case RM_RDN:
             return a_sign ? pack_sf<FLOAT>(a_sign, FLOAT::BIAS, 0) : zero_sf<FLOAT>(0);
         case RM_RUP:
             return a_sign ? zero_sf<FLOAT>(a_sign) : pack_sf<FLOAT>(0, FLOAT::BIAS, 0);
         default:
         case RM_RTZ:
             break;
        }
        return zero_sf<FLOAT>(a_sign);
    }

    typename FLOAT::F_UINT lastBitMask = 1;
    lastBitMask <<= (FLOAT::BIAS + FLOAT::MANT_SIZE) - aExp; //0x433
    typename FLOAT::F_UINT roundBitsMask = lastBitMask - 1;
    auto z = a.v;
    if (rm == RM_RNE) {
        z += lastBitMask >> 1;
        if ( !( z & roundBitsMask ) ) z &= ~ lastBitMask;
    }
    else if (rm != RM_RTZ) {
        if ( FLOAT{z}.get_sign() ^ (rm == RM_RUP) ) {
            z += roundBitsMask;
        }
    }
    z &= ~ roundBitsMask;
    if ( z != a.v )
        raise(FFLAG_INEXACT);
    return FLOAT{z};
}

template<typename INTEGER, typename FLOAT>
INTEGER
SoftFPU::cvt_f_i(FLOAT a, RoundingModeEnum rm)
{
    static_assert(std::numeric_limits<INTEGER>::is_integer);
    static_assert(fp_traits<FLOAT>::is_fp());

    using UNSIGNED_INTEGER = typename integer_traits<INTEGER>::unsigned_type;
    u32 addend;
    UNSIGNED_INTEGER r, r_max;

    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) { // NAN or INF
        raise(FFLAG_INVALID);
        if constexpr (integer_traits<INTEGER>::is_unsigned())
            r_max = (UNSIGNED_INTEGER)a_sign - 1;
        else
            r_max = ((UNSIGNED_INTEGER)1 << (8 * sizeof(INTEGER) - 1)) - (UNSIGNED_INTEGER)(a_sign ^ 1);
        return r_max;
    }

    if (a_exp == FLOAT::EXP_MASK && a_mant)
        a_sign = 0; /* NaN is like +infinity */
    if (a_exp == 0) {
        a_exp = 1;
    } else {
        a_mant |= (typename FLOAT::F_UINT)1 << FLOAT::MANT_SIZE;
    }
    a_mant <<= FLOAT::RND_SIZE;
    a_exp = a_exp - FLOAT::BIAS - FLOAT::MANT_SIZE;

    if constexpr (integer_traits<INTEGER>::is_unsigned())
        r_max = (UNSIGNED_INTEGER)a_sign - 1;
    else
        r_max = ((UNSIGNED_INTEGER)1 << (8 * sizeof(INTEGER) - 1)) - (UNSIGNED_INTEGER)(a_sign ^ 1);

    if (a_exp >= 0) {
        if (a_exp <= (int)(8 * sizeof(INTEGER) - 1 - FLOAT::MANT_SIZE)) {
            r = (UNSIGNED_INTEGER)(a_mant >> FLOAT::RND_SIZE) << a_exp;
            if (r > r_max) {
                raise(FFLAG_INVALID);
                return r_max;
            }
        } else {
            raise(FFLAG_INVALID);
            return r_max;
        }
    } else {
        a_mant = rshift_rnd<FLOAT>(a_mant, -a_exp);

        switch(rm) {
        case RM_RNE:
        case RM_RMM:
            addend = (1 << (FLOAT::RND_SIZE - 1));
            break;
        case RM_RTZ:
            addend = 0;
            break;
        default:
        case RM_RDN:
        case RM_RUP:
            if (a_sign ^ (rm == RM_RUP/*& 1*/))
                addend = (1 << FLOAT::RND_SIZE) - 1;
            else
                addend = 0;
            break;
        }

        u32 const rnd_bits = ((u32)a_mant) & ((1 << FLOAT::RND_SIZE ) - 1);
        a_mant = (a_mant + addend) >> FLOAT::RND_SIZE;
        // half way: select even result
        if (rm == RM_RNE && rnd_bits == (1 << (FLOAT::RND_SIZE - 1)))
            a_mant &= ~(typename FLOAT::F_UINT)1;
        if (a_mant > r_max) {
            raise(FFLAG_INVALID);
            return r_max;
        }
        r = (UNSIGNED_INTEGER)a_mant;
        if (rnd_bits != 0)
            raise(FFLAG_INEXACT);
    }
    if (a_sign)
        r = -r;
    return r;
}

// conversions between float and integers
template<typename FLOAT, typename INTEGER>
FLOAT
SoftFPU::cvt_i_f(INTEGER a, RoundingModeEnum rm)
{
    static_assert(std::numeric_limits<INTEGER>::is_integer);
    static_assert(fp_traits<FLOAT>::is_fp());

    using UNSIGNED_INTEGER = typename integer_traits<INTEGER>::unsigned_type;

    u32 a_sign;
    UNSIGNED_INTEGER r;

    if (!integer_traits<INTEGER>::is_unsigned() && a < 0) {
        a_sign = 1;
        r = -(UNSIGNED_INTEGER)a;
    } else {
        a_sign = 0;
        r = a;
    }
    i32 a_exp = FLOAT::BIAS + FLOAT::F_SIZE - 2;
    /* need to reduce range before generic float normalization */
    int const l = 8*sizeof(INTEGER) - clz(r) - (FLOAT::F_SIZE - 1);
    if (l > 0) {
        assert((size_t)l < 8 * sizeof(UNSIGNED_INTEGER));
        UNSIGNED_INTEGER const mask = (UNSIGNED_INTEGER{1} << l) - 1;
        r = (r >> l) | ((r & mask) != 0);
        a_exp += l;
    }
    typename FLOAT::F_UINT a_mant = r;
    return normalize_sf<FLOAT>(a_sign, a_exp, a_mant, rm);
}

template<typename FLOAT> bool SoftFPU::compare_ordered(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        //if (is_signan(a) || is_signan(b)) {
        //    raise(FFLAG_INVALID);
        //}
        return false;
    }
    return true;
}

template<typename FLOAT> bool SoftFPU::compare_unordered(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        //if (is_signan(a) || is_signan(b)) {
        //    raise(FFLAG_INVALID);
        //}
        return true;
    }
    return false;
}

template<typename FLOAT> bool SoftFPU::compare_ordered_eq(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return false;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return true;
        } else {
            return false;
        }
    } else {
        if (a.val() == b.val()) {
           return true;
        } else {
           return false;
        }
    }
}

template<typename FLOAT> bool SoftFPU::compare_unordered_eq(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return true;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return true;
        } else {
            return false;
        }
    } else {
        if (a.val() == b.val()) {
           return true;
        } else {
           return false;
        }
    }
}

template<typename FLOAT> bool SoftFPU::compare_ordered_ne(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return false;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return false;
        } else {
            return true;
        }
    } else {
        if (a.val() == b.val()) {
           return false;
        } else {
           return true;
        }
    }
}

template<typename FLOAT> bool SoftFPU::compare_unordered_ne(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return true;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return false;
        } else {
            return true;
        }
    } else {
        if (a.val() == b.val()) {
           return false;
        } else {
           return true;
        }
    }
}

template<typename FLOAT> bool SoftFPU::compare_ordered_lt(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return false;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return false;
        } else {
            return a_sign ? true : false;
        }
    } else {
        if (a.val() == b.val()) {
           return false;
        } else {
           return (a_sign ^ ( a.val() < b.val() )) ? true : false;
        }
    }
}

template<typename FLOAT> bool SoftFPU::compare_unordered_lt(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return true;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return false;
        } else {
            return a_sign ? true : false;
        }
    } else {
        if (a.val() == b.val()) {
           return false;
        } else {
           return (a_sign ^ ( a.val() < b.val() )) ? true : false;
        }
    }
}

template<typename FLOAT> bool SoftFPU::compare_ordered_ge(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return false;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return true;
        } else {
            return a_sign ? true : false;
        }
    } else {
        if (a.val() == b.val()) {
           return true;
        } else {
           return (a_sign ^ ( a.val() > b.val() )) ? true : false;
        }
    }
}

template<typename FLOAT> bool SoftFPU::compare_unordered_ge(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return true;
    }

    const auto a_sign = a.get_sign();
    const auto b_sign = b.get_sign();

    if (a_sign != b_sign) {
        if (is_both_zero(a,b)) {
            return true;
        } else {
            return a_sign ? true : false;
        }
    } else {
        if (a.val() == b.val()) {
           return true;
        } else {
           return (a_sign ^ ( a.val() > b.val() )) ? true : false;
        }
    }
}

/*----------------------------------------------------------------------------
| Returns the binary log of the double-precision floating-point value `a'.
| The operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
template<typename FLOAT> FLOAT SoftFPU::log2(FLOAT a)
{
    using F_UINT = typename FLOAT::F_UINT;

    auto a_sign = a.get_sign();
    auto aExp = a.get_exp();
    auto aSig = a.get_fraction();

    if ( aExp == 0 ) {
        if ( aSig == 0 ) {
           raise(FFLAG_DIVBYZERO);
           return pack_sf<FLOAT>(1, FLOAT::EXP_MASK, 0); // -INF
        }
        aSig = normalize_subnormal_sf<FLOAT>(&aExp, aSig);
    }
    if ( a_sign ) {
        raise(FFLAG_INVALID);
        //if ( aExp == FLOAT::EXP_MASK && aSig ) return propagate_nan(a);
        return FLOAT{FLOAT::F_QNAN};
    }
    if (aExp == FLOAT::EXP_MASK) {
        if ( aSig ) return FLOAT{FLOAT::F_QNAN};
        return a;
    }

    aExp -= FLOAT::BIAS;
    aSig |= F_UINT{1} << (FLOAT::MANT_SIZE);

    i32 zSign = aExp < 0;
    F_UINT zSig = (F_UINT)(aExp) << (FLOAT::MANT_SIZE);

    F_UINT const mask = F_UINT{1} << (FLOAT::MANT_SIZE + 1);

    for (F_UINT i = F_UINT{1} << (FLOAT::MANT_SIZE - 1); i /* > 0*/; i >>= 1) {
        auto [aSig0, aSig1] = mul_u<F_UINT>(aSig, aSig);
        aSig = (aSig0 << (FLOAT::F_SIZE - FLOAT::MANT_SIZE)) | (aSig1 >> (FLOAT::MANT_SIZE));
        if (aSig & mask) {
            aSig >>= 1;
            zSig |= i;
        }
    }

    if (zSign)
        zSig = F_UINT{0} - zSig; // unary minus unsigned error

    return normalize_sf<FLOAT>(zSign, FLOAT::BIAS + FLOAT::EXP_SIZE - 1, zSig, get_rounding_mode());
}

/* Multiply A by 2 raised to the power N.  */
template<typename FLOAT> FLOAT SoftFPU::scalbn(FLOAT a, int n)
{
    auto a_sign = a.get_sign();
    auto aSig   = a.get_fraction();
    auto aExp   = a.get_exp();

    if ( aExp == FLOAT::EXP_MASK ) {
        if ( aSig ) {
            return FLOAT{FLOAT::F_QNAN};
        }
        return a;
    }
    if ( aExp != 0 )
        aSig |= (typename FLOAT::F_UINT{1} << FLOAT::MANT_SIZE);
    else if ( aSig == 0 )
        return a;

    i32 const ddd = 1 << (FLOAT::EXP_SIZE + 1);
    if (n > ddd) {
        n = ddd;
    } else if (n < -ddd) {
        n = -ddd;
    }

    aExp += n - 1;
    aSig <<= (FLOAT::F_SIZE - FLOAT::MANT_SIZE - 2);
    return normalize_sf<FLOAT>(a_sign, aExp, aSig, get_rounding_mode());
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

template<typename FLOAT> FLOAT SoftFPU::exp(FLOAT a)
{
    auto aSig = a.get_fraction();
    auto aExp = a.get_exp();
    auto a_sign = a.get_sign();

    FLOAT const f_one = pack_sf<FLOAT>(0, FLOAT::BIAS, 0);

    if ( aExp == FLOAT::EXP_MASK) {
        if ( aSig ) return FLOAT{FLOAT::F_QNAN};
        return (a_sign) ? zero_sf<FLOAT>(0) : a;
    }
    if (aExp == 0) {
        if (aSig == 0) return f_one;
    }

    raise(FFLAG_INEXACT);
    if (a_sign && aExp > FLOAT::BIAS + 32) {
       return zero_sf<FLOAT>(0);
    }
    if (!a_sign && aExp > FLOAT::BIAS + 32) {
       return pack_sf<FLOAT>(0, FLOAT::EXP_MASK, 0); // INF
    }

    FLOAT x = a;
    // x = mul(x, f64_ln2);

    const int N = 18;
    FLOAT s[N];
    FLOAT xn = x;
    FLOAT n = f_one;
    for (int i = 0 ; i < N; i++) {
        s[i] = xn;
        n = add(n, f_one);
        xn = mul(xn, x);
        xn = div(xn, n);
    }

    FLOAT r = zero_sf<FLOAT>(0);
    for (int i = N - 1; i >= 0; i--) {
        r = add(r, s[i]);
    }
    r = add(r, f_one);

    return r;
}

} // namespace fpu
} // namespace postrisc
