#pragma once

#include <bit>
#include <type_traits>

#include "util/common.hpp"
#include "util/fmt.hpp"
#include "arch/opcode.hpp"

// this works at least with gcc
#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

namespace postrisc {
namespace fpu {

// mimic arch rounding modes except dynamic
enum RoundingModeEnum {
    RM_RNE = static_cast<u8>(RoundingMode::RNE), // Round to Nearest, ties to Even
    RM_RDN = static_cast<u8>(RoundingMode::RDN), // Round Down
    RM_RUP = static_cast<u8>(RoundingMode::RUP), // Round Up
    RM_RTZ = static_cast<u8>(RoundingMode::RTZ), // Round towards Zero
    RM_RMM = static_cast<u8>(RoundingMode::RMM), // Round to Nearest, ties to Max Magnitude
    RM_RNO = static_cast<u8>(RoundingMode::RNO), // Round to Nearest, ties to Odd
    RM_RTO = static_cast<u8>(RoundingMode::RTO), // Round to Odd (Von Neumann)
};

inline const char *to_string(RoundingModeEnum rm)
{
    switch (rm) {
    case RM_RNE: return "rne";
    case RM_RDN: return "rdn";
    case RM_RUP: return "rup";
    case RM_RTZ: return "rtz";
    case RM_RMM: return "rmm";
    case RM_RNO: return "rno";
    case RM_RTO: return "rto";
    }
    assert(!"unknown RoundingModeEnum");
    return nullptr;
}

inline std::ostream& operator<<(std::ostream& out, RoundingModeEnum rm)
{
    return out << to_string(rm);
}

enum fp_flags : u8 {
    FFLAG_NOERROR    = 0,
    FFLAG_INVALID    = static_cast<u8>(FExceptionFlag::Invalid),
    FFLAG_DIVBYZERO  = static_cast<u8>(FExceptionFlag::DivideByZero),
    FFLAG_OVERFLOW   = static_cast<u8>(FExceptionFlag::Overflow),
    FFLAG_UNDERFLOW  = static_cast<u8>(FExceptionFlag::Underflow),
    FFLAG_INEXACT    = static_cast<u8>(FExceptionFlag::Inexact),
    FFLAG_ALL_EXCEPT = static_cast<u8>(FExceptionFlag::All),
};

inline fp_flags operator | ( fp_flags a, fp_flags b ) { return fp_flags( u8(a) | u8(b) ); }
inline fp_flags operator & ( fp_flags a, fp_flags b ) { return fp_flags( u8(a) & u8(b) ); }
inline fp_flags operator ~ ( fp_flags a ) { return fp_flags( ~u8(a) ); }

// FIXME: i128/u128 are not is std traits, so we need our one
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
        const u64 ah = a >> 64;
        return ah ? std::countl_zero(ah) : std::countl_zero(static_cast<u64>(a)) + 64;
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
    static constexpr const int FRAC_SIZE = F_SIZE - EXP_SIZE - 1;                                                    \
    static constexpr const int EXP_MASK = (1 << EXP_SIZE) - 1;                                                       \
    static constexpr const int IFRAC_SIZE = F_SIZE - 2; /* internal fraction size */                                 \
    static constexpr const int RND_SIZE = IFRAC_SIZE - FRAC_SIZE;  /* EXP_SIZE - 1 */                                \
    static constexpr const int BIAS = (1 << (EXP_SIZE - 1)) - 1;                                                     \
    static constexpr const F_UINT FRAC_MASK = (F_UINT{1} << FRAC_SIZE) - 1;                                          \
    static constexpr const F_UINT SIGN_MASK = F_UINT{1} << (F_SIZE - 1);                                             \
    static constexpr const F_UINT QNAN_MASK = F_UINT{1} << (FRAC_SIZE - 1);                                          \
    static constexpr const F_UINT F_QNAN = (F_UINT{EXP_MASK} << FRAC_SIZE) | (F_UINT{1} << (FRAC_SIZE - 1));         \
    u32 constexpr get_sign(void) const { return static_cast<u32>(v >> (F_SIZE - 1)); }                               \
    i32 constexpr get_exp(void) const { return static_cast<u32>(v >> FRAC_SIZE) & EXP_MASK; }                        \
    F_UINT constexpr get_fraction(void) const { return v & FRAC_MASK; }                                              \
    F_UINT constexpr val(void) const { return v; }                                                                   \


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
    template<typename FLOAT> FLOAT merge(FLOAT sign, FLOAT exponent, FLOAT fraction);

    template<typename FLOAT> FLOAT add(FLOAT a, FLOAT b, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT sub(FLOAT a, FLOAT b, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT mul(FLOAT a, FLOAT b, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT div(FLOAT a, FLOAT b, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT min(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT max(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT muladd(FLOAT a, FLOAT b, FLOAT c, const RoundingModeEnum rm, muladd_negate_t negate_flags);
    template<typename FLOAT> FLOAT round(FLOAT a, const RoundingModeEnum rm, bool inexact);
    template<typename FLOAT> FLOAT sqrt(FLOAT a, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT rsqrt(FLOAT a, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT log2(FLOAT a, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT exp(FLOAT a, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT scalbn(FLOAT a, int exp, const RoundingModeEnum rm);

    template<typename FLOAT_DST, typename FLOAT_SRC> FLOAT_DST extend(FLOAT_SRC a);
    template<typename FLOAT_DST, typename FLOAT_SRC> FLOAT_DST cvt_f_f(FLOAT_SRC a, const RoundingModeEnum rm);
    template<typename INTEGER, typename FLOAT> INTEGER cvt_f_i(FLOAT a, const RoundingModeEnum rm);
    template<typename FLOAT, typename INTEGER> FLOAT cvt_i_f(INTEGER a, const RoundingModeEnum rm);

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

    template<typename FLOAT> FClassMask classify(FLOAT a);

    template<typename FLOAT> static constexpr FLOAT pack_sf(u32 a_sign, u32 a_exp, typename FLOAT::F_UINT a_frac);

    template<typename FLOAT> static FLOAT propagateNAN(FLOAT a);
    template<typename FLOAT> static FLOAT propagateNAN(FLOAT a, FLOAT b);
    template<typename FLOAT> static FLOAT propagateNAN(FLOAT a, FLOAT b, FLOAT c);

private:
    void raise(fp_flags flags) { m_exception_flags = (m_exception_flags | flags); }

    template<typename FLOAT> static bool is_both_zero(FLOAT a, FLOAT b) { return static_cast<typename FLOAT::F_UINT>(( a.v | b.v ) << 1) == 0; }
    template<typename FLOAT> static FLOAT zero_sf(u32 a_sign);

    template<typename FLOAT> FLOAT round_pack_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_frac, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT normalize_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_frac, const RoundingModeEnum rm);
    template<typename FLOAT> FLOAT normalize2_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_frac1, typename FLOAT::F_UINT a_frac0, const RoundingModeEnum rm);

    template<typename FLOAT> FLOAT add_internal(FLOAT a, FLOAT b, const RoundingModeEnum rm, bool subtract);

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
    word |= static_cast<u64>(get_flags())         << disp_flags;
    word |= static_cast<u64>(get_rounding_mode()) << disp_rounding_mode;
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
    << " rm=" << static_cast<int>(fpu.get_rounding_mode());
}

template<typename FLOAT> constexpr FLOAT SoftFPU::pack_sf(u32 a_sign, u32 a_exp, typename FLOAT::F_UINT a_frac)
{
    // LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " frac=" << fmt::hex(a_frac);
    const typename FLOAT::F_UINT tmp =
        (static_cast<typename FLOAT::F_UINT>(a_sign) << (FLOAT::F_SIZE - 1)) |
        (static_cast<typename FLOAT::F_UINT>(a_exp) << FLOAT::FRAC_SIZE) |
        (a_frac & static_cast<typename FLOAT::F_UINT>(FLOAT::FRAC_MASK));
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
            auto const mask = (static_cast<typename FLOAT::F_UINT>(1) << d) - 1;
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
SoftFPU::merge(FLOAT sign, FLOAT exponent, FLOAT fraction)
{
    const typename FLOAT::F_UINT tmp =
           (    sign.val() & (((static_cast<typename FLOAT::F_UINT>(1) << (FLOAT::F_SIZE - 1 ))    )  )) |
           (fraction.val() & (((static_cast<typename FLOAT::F_UINT>(1) << (FLOAT::FRAC_SIZE  )) - 1)  )) |
           (exponent.val() & (((static_cast<typename FLOAT::F_UINT>(1) << (FLOAT::EXP_SIZE   )) - 1) << FLOAT::FRAC_SIZE));
    return FLOAT{tmp};
}

// a_frac is considered to have its MSB at F_SIZE - 2 bits
template<typename FLOAT>
FLOAT
SoftFPU::round_pack_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_frac, const RoundingModeEnum rm)
{
    LOG_DEBUG(LOG_EVALUATE) << "s=" << a_sign << " exp=" << a_exp << " frac=" << fmt::hex(a_frac);

    u32 addend, rnd_bits;

    switch (rm) {
    case RM_RNE:
    case RM_RMM:
        addend = (1U << (FLOAT::RND_SIZE - 1));
        break;

    case RM_RNO: //FIXME
    case RM_RTO: //FIXME: rtz+jamming
    case RM_RTZ:
        addend = 0;
        break;

    default:
    case RM_RDN:
        if (a_sign)
            addend = (1U << FLOAT::RND_SIZE) - 1;
        else
            addend = 0;
        break;

    case RM_RUP:
        if (a_sign)
            addend = 0;
        else
            addend = (1U << FLOAT::RND_SIZE) - 1;
        break;
    }

    // potentially subnormal
    if (a_exp <= 0) {
        // Note: we set the underflow flag if the rounded result is subnormal and inexact
        const bool is_subnormal = (a_exp < 0 ||
              (a_frac + addend) < (static_cast<typename FLOAT::F_UINT>(1) << (FLOAT::F_SIZE - 1)));
        const int diff = 1 - a_exp;
        a_frac = rshift_rnd<FLOAT>(a_frac, diff);
        rnd_bits = static_cast<u32>(a_frac) & ((1U << FLOAT::RND_SIZE) - 1);
        if (is_subnormal && rnd_bits != 0) {
            raise(FFLAG_UNDERFLOW);
        }
        a_exp = 1;
    } else {
        rnd_bits = static_cast<u32>(a_frac) & ((1U << FLOAT::RND_SIZE) - 1);
    }
    if (rnd_bits != 0) {
        // std::cerr << "INEXACT: " << fmt::hex(a_frac) << " addend=" << fmt::hex(addend) << " rnd_bits=" << rnd_bits << " a_exp=" << a_exp << std::endl;
        raise(FFLAG_INEXACT);
    }
    a_frac = (a_frac + static_cast<typename FLOAT::F_UINT>(addend)) >> FLOAT::RND_SIZE;

    // half way: select even/odd result
    switch (rm) {
    case RM_RNE:
        if (rnd_bits == (1U << (FLOAT::RND_SIZE - 1U)))
            a_frac &= ~static_cast<typename FLOAT::F_UINT>(1);
        break;

    case RM_RNO:
        //if (rnd_bits == (1U << (FLOAT::RND_SIZE - 1U)))
        //    a_frac |= static_cast<typename FLOAT::F_UINT>(1);

        if ((static_cast<u32>(a_frac) & 1) == 0 && rnd_bits == (1 << (FLOAT::RND_SIZE - 1)))
            a_frac |= 1;
        else if (rnd_bits > (1 << (FLOAT::RND_SIZE - 1)))
            a_frac++;
        break;

    case RM_RTO:
        if (rnd_bits)
            a_frac |= 1;
        break;

    default:
        break;
    }

    // Note the rounding adds at least 1, so this is the maximum value
    a_exp += static_cast<int>(a_frac >> (FLOAT::FRAC_SIZE + 1));
    if (a_frac <= FLOAT::FRAC_MASK) {
        // denormalized or zero
        a_exp = 0;
    } else if (a_exp >= FLOAT::EXP_MASK) {

        //Overflow is signaled whenever the destination format’s largest finite number is exceeded in magnitude by what would have been the rounded floating-point result.
        //The result computed is rounding mode specific:
        //   Round-to-nearest (even) (default): +/- Infinity in specified precision
        //   Round-to-zero: +/- Maximum Number in specified precision
        //   Round-to-+Infinity: +Infinity or –(Maximum Positive Number) in specified precision
        //   Round-to--Infinity: (Maximum Positive Number) or -Infinity in specified precision
        //   Round-to-MaxMag +/- Infinity in specified precision (FIXME?)
        //   Round-to-nearest (odd) +/- Infinity in specified precision (FIXME?)

        //LOG_OUT << "OVERFLOW: s=" << a_sign << " exp=" << a_exp << " addend=" << addend << fmt::endl;
        if (addend == 0 && rm != RM_RNO) {
            //LOG_OUT << "switch to largest normal" << fmt::endl;
            a_exp = FLOAT::EXP_MASK - 1;
            a_frac = FLOAT::FRAC_MASK;
        } else {
            // infinity
            //LOG_OUT << "switch to infinity" << fmt::endl;
            a_exp = FLOAT::EXP_MASK;
            a_frac = 0;
        }
        raise(FFLAG_OVERFLOW | FFLAG_INEXACT);
#if 0
    } else {
        // FIXME: avx-fp16 conversion int=>fp16 overflow before rounding?
        if constexpr (std::is_same_v<FLOAT, f16>) {
            //std::cerr << "WHAT: " << fmt::hex(a_frac) << " addend=" << fmt::hex(addend) << " rnd_bits=" << rnd_bits << " a_exp=" << a_exp << fmt::endl;
            if (addend == 0 && a_exp == FLOAT::EXP_MASK - 1 && a_frac == 0x7FF && rnd_bits != 0) {
                //std::cerr << "OVERFLOW!" << fmt::endl;
                raise(FFLAG_OVERFLOW);
            }
        }
#endif
    }
    return pack_sf<FLOAT>(a_sign, a_exp, a_frac);
}

/* a_frac is considered to have at most F_SIZE - 1 bits */
template<typename FLOAT>
FLOAT
SoftFPU::normalize_sf(u32 a_sign, int a_exp, typename FLOAT::F_UINT a_frac, const RoundingModeEnum rm)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " frac=" << fmt::hex(a_frac);

    int const shift = clz(a_frac) - (FLOAT::F_SIZE - 1 - FLOAT::IFRAC_SIZE);
    //if (shift < 0) {
    //    LOG_DEBUG(LOG_EVALUATE) << "failed: a_frac=" << fmt::hex(a_frac) << " shift=" << shift;
    //}
    assert(shift >= 0);
    a_exp -= shift;
    a_frac <<= shift;
    return round_pack_sf<FLOAT>(a_sign, a_exp, a_frac, rm);
}

/* same as normalize_sf() but with a double word fraction. a_frac1 is
   considered to have at most F_SIZE - 1 bits */
template<typename FLOAT>
FLOAT
SoftFPU::normalize2_sf(u32 a_sign, int a_exp,
    typename FLOAT::F_UINT a_frac1, typename FLOAT::F_UINT a_frac0, const RoundingModeEnum rm)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " frac=" << fmt::hex(a_frac1) << '_' << fmt::hex(a_frac0);

    //LOG_OUT << "s=" << a_sign
    //        << " exp=" << a_exp
    //        << " a_frac=" << fmt::hex(a_frac1) << ' ' << fmt::hex(a_frac0)
    //        << fmt::endl;

    const int l = a_frac1 ? clz(a_frac1) : FLOAT::F_SIZE + clz(a_frac0);
    const int shift = l - (FLOAT::F_SIZE - 1 - FLOAT::IFRAC_SIZE);
    assert(shift >= 0);
    a_exp -= shift;
    if (shift == 0) {
        a_frac1 |= (a_frac0 != 0);
    } else if (shift < FLOAT::F_SIZE) {
        a_frac1 = (a_frac1 << shift) | (a_frac0 >> (FLOAT::F_SIZE - shift));
        a_frac0 <<= shift;
        a_frac1 |= (a_frac0 != 0);
    } else {
        a_frac1 = a_frac0 << (shift - FLOAT::F_SIZE);
    }
    return round_pack_sf<FLOAT>(a_sign, a_exp, a_frac1, rm);
}

template<typename FLOAT> bool is_inf(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_frac = a.get_fraction();
    return (a_exp == FLOAT::EXP_MASK && !a_frac);
}

template<typename FLOAT> bool is_zero(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_frac = a.get_fraction();
    return (a_exp == 0 && a_frac == 0);
}

template<typename FLOAT> bool is_subnormal(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_frac = a.get_fraction();
    return (a_exp == 0 && a_frac != 0);
}

template<typename FLOAT> bool is_nan(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_frac = a.get_fraction();
    return (a_exp == FLOAT::EXP_MASK && !!a_frac);
}

template<typename FLOAT> bool is_signan(FLOAT a)
{
    u32 const a_exp1 = static_cast<u32>(a.v >> (FLOAT::FRAC_SIZE - 1)) & ((1U << (FLOAT::EXP_SIZE + 1)) - 1);
    auto const a_frac = a.get_fraction();
    return (a_exp1 == (2 * FLOAT::EXP_MASK) && !!a_frac);
}

template<typename FLOAT>
FLOAT SoftFPU::propagateNAN(FLOAT a)
{
    if (is_nan(a)) {
        return pack_sf<FLOAT>(a.get_sign(), FLOAT::EXP_MASK, FLOAT::QNAN_MASK | a.get_fraction());
    }
    return FLOAT{FLOAT::F_QNAN};
}

template<typename FLOAT>
FLOAT SoftFPU::propagateNAN(FLOAT a, FLOAT b)
{
    if (is_nan(a)) {
        return pack_sf<FLOAT>(a.get_sign(), FLOAT::EXP_MASK, FLOAT::QNAN_MASK | a.get_fraction());
    }
    if (is_nan(b)) {
        return pack_sf<FLOAT>(b.get_sign(), FLOAT::EXP_MASK, FLOAT::QNAN_MASK | b.get_fraction());
    }
    return FLOAT{FLOAT::F_QNAN};
}

template<typename FLOAT>
FLOAT SoftFPU::propagateNAN(FLOAT a, FLOAT b, FLOAT c)
{
    if (is_nan(a)) {
        return pack_sf<FLOAT>(a.get_sign(), FLOAT::EXP_MASK, FLOAT::QNAN_MASK | a.get_fraction());
    }
    if (is_nan(b)) {
        return pack_sf<FLOAT>(b.get_sign(), FLOAT::EXP_MASK, FLOAT::QNAN_MASK | b.get_fraction());
    }
    if (is_nan(c)) {
        return pack_sf<FLOAT>(c.get_sign(), FLOAT::EXP_MASK, FLOAT::QNAN_MASK | c.get_fraction());
    }
    return FLOAT{FLOAT::F_QNAN};
}

template<typename FLOAT> ALWAYS_INLINE FLOAT SoftFPU::add_internal(FLOAT a, FLOAT b, const RoundingModeEnum rm, bool subtract)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return propagateNAN(a, b);
    }

    if (subtract) {
        b.v ^= FLOAT::SIGN_MASK;
    }

    // swap so that abs(a) >= abs(b)
    if ((a.v & ~FLOAT::SIGN_MASK) < (b.v & ~FLOAT::SIGN_MASK)) {
        std::swap(a,b);
    }

    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto a_exp  = a.get_exp();
    auto b_exp  = b.get_exp();
    typename FLOAT::F_UINT a_frac = a.get_fraction() << 3;
    typename FLOAT::F_UINT b_frac = b.get_fraction() << 3;

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_frac) {
            // NaN result
            if (!(a_frac & (FLOAT::QNAN_MASK << 3)) || is_signan(b))
                raise(FFLAG_INVALID);
            return propagateNAN(a, b);
            // return FLOAT{FLOAT::F_QNAN};
        } else if (b_exp == FLOAT::EXP_MASK && a_sign != b_sign) {
            raise(FFLAG_INVALID);
            return pack_sf<FLOAT>(1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
            // return FLOAT{FLOAT::F_QNAN};
        } else {
            // infinity
            return a;
        }
    }

    if (a_exp == 0) {
        a_exp = 1;
    } else {
        a_frac |= static_cast<typename FLOAT::F_UINT>(1) << (FLOAT::FRAC_SIZE + 3);
    }
    if (b_exp == 0) {
        b_exp = 1;
    } else {
        b_frac |= static_cast<typename FLOAT::F_UINT>(1) << (FLOAT::FRAC_SIZE + 3);
    }

    b_frac = rshift_rnd<FLOAT>(b_frac, a_exp - b_exp);
    if (a_sign == b_sign) {
        // same signs: add the absolute values
        a_frac += b_frac;
    } else {
        // different signs: subtract the absolute values
        a_frac -= b_frac;
        if (a_frac == 0) {
            // zero result: the sign needs a specific handling
            a_sign = (rm == RM_RDN);
        }
    }
    a_exp += (FLOAT::RND_SIZE - 3);
    return normalize_sf<FLOAT>(a_sign, a_exp, a_frac, rm);
}

template<typename FLOAT> FLOAT SoftFPU::add(FLOAT a, FLOAT b, const RoundingModeEnum rm)
{
    return add_internal(a, b, rm, false/*subtract*/);
}

template<typename FLOAT> FLOAT SoftFPU::sub(FLOAT a, FLOAT b, const RoundingModeEnum rm)
{
    return add_internal(a, b, rm, true/*subtract*/);
}

template<typename FLOAT>
inline
typename FLOAT::F_UINT normalize_subnormal_sf(i32 *pa_exp, typename FLOAT::F_UINT a_frac)
{
    int const shift = FLOAT::FRAC_SIZE - ((FLOAT::F_SIZE - 1 - clz(a_frac)));
    *pa_exp = 1 - shift;
    return a_frac << shift;
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

template<typename FLOAT> FLOAT SoftFPU::mul(FLOAT a, FLOAT b, const RoundingModeEnum rm)
{
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto r_sign = a_sign ^ b_sign;
    auto a_exp = a.get_exp();
    auto b_exp = b.get_exp();
    auto a_frac = a.get_fraction();
    auto b_frac = b.get_fraction();

    if (a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK) {
        if (is_nan(a) || is_nan(b)) {
            if (is_signan(a) || is_signan(b)) {
                raise(FFLAG_INVALID);
            }
            return propagateNAN(a, b);
            // return FLOAT{FLOAT::F_QNAN};
        } else {
            // infinity
            if ((a_exp == FLOAT::EXP_MASK && (b_exp == 0 && b_frac == 0)) ||
                (b_exp == FLOAT::EXP_MASK && (a_exp == 0 && a_frac == 0))) {
                raise(FFLAG_INVALID);
                return pack_sf<FLOAT>(1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
                // return FLOAT{FLOAT::F_QNAN};
            } else {
                return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
            }
        }
    }
    if (a_exp == 0) {
        if (a_frac == 0)
            return zero_sf<FLOAT>(r_sign);
        a_frac = normalize_subnormal_sf<FLOAT>(&a_exp, a_frac);
    } else {
        a_frac |= static_cast<typename FLOAT::F_UINT>(1) << FLOAT::FRAC_SIZE;
    }
    if (b_exp == 0) {
        if (b_frac == 0)
            return zero_sf<FLOAT>(r_sign);
        b_frac = normalize_subnormal_sf<FLOAT>(&b_exp, b_frac);
    } else {
        b_frac |= static_cast<typename FLOAT::F_UINT>(1) << FLOAT::FRAC_SIZE;
    }
    int r_exp = a_exp + b_exp - FLOAT::BIAS + 1; //(1 << (FLOAT::EXP_SIZE - 1)) + 2;

    auto [r_frac, r_frac_low] = mul_u<typename FLOAT::F_UINT>(a_frac << FLOAT::RND_SIZE, b_frac << (FLOAT::RND_SIZE + 1));
    r_frac |= !!r_frac_low; // != 0
    return normalize_sf<FLOAT>(r_sign, r_exp, r_frac, rm);
}


/*****************************************************************************************************************************
FMA Errors are reported as specified in math_errhandling.
If the implementation supports IEEE floating-point arithmetic (IEC 60559),
  1. If x is zero and y is infinite or if x is infinite and y is zero, and
    a) if z is not a NaN, then NaN is returned and FE_INVALID is raised,
    b) if z is a NaN, then NaN is returned and FE_INVALID may be raised (not raised in x86 hardware, raised in libquadmath).
  2. If x * y is an exact infinity and z is an infinity with the opposite sign, NaN is returned and FE_INVALID is raised.
  3. If x or y are NaN, NaN is returned.
  4. If z is NaN, and x * y is not 0 * Inf or Inf * 0, then NaN is returned (without FE_INVALID).
******************************************************************************************************************************/
template<typename FLOAT>
FLOAT
SoftFPU::muladd(FLOAT a, FLOAT b, FLOAT c, const RoundingModeEnum rm, muladd_negate_t negate_mask)
{
    using F_UINT = typename FLOAT::F_UINT;
    F_UINT c_frac1, c_frac0;

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
    auto a_frac = a.get_fraction();
    auto b_frac = b.get_fraction();
    auto c_frac = c.get_fraction();

    if (a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK || c_exp == FLOAT::EXP_MASK) {
        if (is_nan(a) || is_nan(b) || is_nan(c)) {
            if (is_signan(a) || is_signan(b) || is_signan(c)) {
                raise(FFLAG_INVALID);
            }
            return propagateNAN(a, b, c);
            // return FLOAT{FLOAT::F_QNAN};
        } else {
            // infinities
            if ((a_exp == FLOAT::EXP_MASK && (b_exp == 0 && b_frac == 0)) ||
                (b_exp == FLOAT::EXP_MASK && (a_exp == 0 && a_frac == 0)) ||
                ((a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK) &&
                 (c_exp == FLOAT::EXP_MASK && r_sign != c_sign)))
            {
                raise(FFLAG_INVALID);
                return pack_sf<FLOAT>(1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
                // return FLOAT{FLOAT::F_QNAN};
            } else if (c_exp == FLOAT::EXP_MASK) {
                return pack_sf<FLOAT>(c_sign, FLOAT::EXP_MASK, 0);
            } else {
                return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
            }
        }
    }
    if (a_exp == 0) {
        if (a_frac == 0)
            goto mul_zero;
        a_frac = normalize_subnormal_sf<FLOAT>(&a_exp, a_frac);
    } else {
        a_frac |= static_cast<F_UINT>(1) << FLOAT::FRAC_SIZE;
    }
    if (b_exp == 0) {
        if (b_frac == 0) {
        mul_zero:
            if (c_exp == 0 && c_frac == 0) {
                if (c_sign != r_sign)
                    r_sign = (rm == RM_RDN);
                return zero_sf<FLOAT>(r_sign);
            } else {
                return c;
            }
        }
        b_frac = normalize_subnormal_sf<FLOAT>(&b_exp, b_frac);
    } else {
        b_frac |= static_cast<F_UINT>(1) << FLOAT::FRAC_SIZE;
    }

    // multiply
    auto r_exp = a_exp + b_exp - FLOAT::BIAS + 2; //(1 << (FLOAT::EXP_SIZE - 1)) + 3;

    auto [r_frac1, r_frac0] = mul_u<F_UINT>(a_frac << FLOAT::RND_SIZE, b_frac << FLOAT::RND_SIZE);
    // normalize to F_SIZE - 3
    if (r_frac1 < (static_cast<F_UINT>(1) << (FLOAT::F_SIZE - 3))) {
        r_frac1 = (r_frac1 << 1) | (r_frac0 >> (FLOAT::F_SIZE - 1));
        r_frac0 <<= 1;
        r_exp--;
    }

    // add
    if (c_exp == 0) {
        if (c_frac == 0) {
            // add zero
            r_frac1 |= (r_frac0 != 0);
            return normalize_sf<FLOAT>(r_sign, r_exp, r_frac1, rm);
        }
        c_frac = normalize_subnormal_sf<FLOAT>(&c_exp, c_frac);
    } else {
        c_frac |= static_cast<F_UINT>(1) << FLOAT::FRAC_SIZE;
    }
    c_exp++;
    c_frac1 = c_frac << (FLOAT::RND_SIZE - 1);
    c_frac0 = 0;

    // LOG_DEBUG(LOG_EVALUATE) << "r_s=" << r_sign << " r_exp=" << r_exp << " r_frac=" << fmt::hex(r_frac1) << fmt::hex(r_frac0);
    // LOG_DEBUG(LOG_EVALUATE) << "c_s=" << c_sign << " c_exp=" << c_exp << " c_frac=" << fmt::hex(c_frac1) << fmt::hex(c_frac0);

    // ensure that abs(r) >= abs(c)
    if (!(r_exp > c_exp || (r_exp == c_exp && r_frac1 >= c_frac1))) {
        std::swap(r_frac1, c_frac1);
        std::swap(r_frac0, c_frac0);
        std::swap(r_exp, c_exp);
        std::swap(r_sign, c_sign);
    }

    // right shift c_frac
    const int shift = r_exp - c_exp;
    if (shift >= 2 * FLOAT::F_SIZE) {
        c_frac0 = (c_frac0 | c_frac1) != 0;
        c_frac1 = 0;
    } else if (shift >= FLOAT::F_SIZE + 1) {
        c_frac0 = rshift_rnd<FLOAT>(c_frac1, shift - FLOAT::F_SIZE);
        c_frac1 = 0;
    } else if (shift == FLOAT::F_SIZE) {
        c_frac0 = c_frac1 | (c_frac0 != 0);
        c_frac1 = 0;
    } else if (shift != 0) {
        const F_UINT mask = (F_UINT{1} << shift) - 1;
        c_frac0 = (c_frac1 << (FLOAT::F_SIZE - shift)) | (c_frac0 >> shift) | ((c_frac0 & mask) != 0);
        c_frac1 = c_frac1 >> shift;
    }

    // LOG_DEBUG(LOG_EVALUATE) << " r_frac=" << fmt::hex(r_frac1) << fmt::hex(r_frac0);
    // LOG_DEBUG(LOG_EVALUATE) << " c_frac=" << fmt::hex(c_frac1) << fmt::hex(c_frac0);

    // add or subtract
    if (r_sign == c_sign) {
        r_frac0 += c_frac0;
        r_frac1 += c_frac1 + (r_frac0 < c_frac0);
    } else {
        F_UINT tmp = r_frac0;
        r_frac0 -= c_frac0;
        r_frac1 = r_frac1 - c_frac1 - (r_frac0 > tmp);
        if ((r_frac0 | r_frac1) == 0) {
            // zero result : the sign needs a specific handling
            r_sign = (rm == RM_RDN);
        }
    }
#if 0
    // LOG_DEBUG(LOG_EVALUATE) << " r1_frac=" << fmt::hex(r_frac1) << fmt::hex(r_frac0);
    // normalize
    if (r_frac1 == 0) {
        r_frac1 = r_frac0;
        r_exp -= F_SIZE;
    } else {
        shift = clz(r_frac1) - (F_SIZE - 1 - IFRAC_SIZE);
        if (shift != 0) {
            r_frac1 = (r_frac1 << shift) | (r_frac0 >> (F_SIZE - shift));
            r_frac0 <<= shift;
            r_exp -= shift;
        }
        r_frac1 |= (r_frac0 != 0);
    }
    return normalize_sf<FLOAT>(r_sign, r_exp, r_frac1, rm);
#else
    return normalize2_sf<FLOAT>(r_sign, r_exp, r_frac1, r_frac0, rm);
#endif
}

// naive algorithm, for testing, never used
template<typename F_UINT> std::tuple<F_UINT, F_UINT> divrem_u(F_UINT a1, F_UINT a0, F_UINT b)
{
    static_assert(false); // check that never used

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

template<> ALWAYS_INLINE std::tuple<u16, u16> divrem_u(u16 ah, u16 al, u16 b)
{
    u32 a = ((u32)ah << 16) | al;
    return {a / b, a % b};
}

template<> ALWAYS_INLINE std::tuple<u32, u32> divrem_u(u32 ah, u32 al, u32 b)
{
    u64 a = ((u64)ah << 32) | al;
    return {a / b,  a % b};
}

template<> ALWAYS_INLINE std::tuple<u64, u64> divrem_u(u64 ah, u64 al, u64 b)
{
    u128 a = ((u128)ah << 64) | al;
    return {a / b, a % b};
}

// Knuth's Algorithm D
// http://www.hackersdelight.org/HDCode/divlu.c
// modified for u128
template<> ALWAYS_INLINE std::tuple<u128, u128> divrem_u(u128 u1, u128 u0, u128 v)
{
    const u128 b = static_cast<u128>(1) << 64; // Number base (64 bits).
    u128 un1, un0,        // Norm. dividend LSD's.
         vn1, vn0,        // Norm. divisor digits.
         q1, q0,          // Quotient digits.
         un32, un21, un10,// Dividend digit pairs.
         rhat;            // A remainder.

    assert(u1 < v);
    //if (u1 >= v) { // If overflow, set rem to an impossible value, and return the largest possible quotient.
    //    return { 0xFFFFFFFF, 0xFFFFFFFF };
    //}

    const int s = __builtin_clzll(v >> 64);  // 0 <= s <= 63.
    if (s) {
        v = v << s;               // Normalize divisor.
    }

    // Break divisor up into two 64-bit digits.
    vn1 = v >> 64;
    vn0 = static_cast<u64>(v);

    if (s) {
        // Shift dividend left.
        un32 = (u1 << s) | (u0 >> (64 - s));
        un10 = u0 << s;
    } else {
        un32 = u1;
        un10 = u0;
    }

    un1 = un10 >> 64;         // Break right half of
    un0 = (u64)un10;      // dividend into two digits.

    q1 = un32/vn1;            // Compute the first
    rhat = un32 - q1*vn1;     // quotient digit, q1.

    while (q1 >= b || q1*vn0 > (rhat<<64) + un1) {
        q1 = q1 - 1;
        rhat = rhat + vn1;
        if (rhat >= b) break;
    }

    un21 = (un32<<64) + un1 - q1*v;  // Multiply and subtract.

    q0 = un21/vn1;            // Compute the second
    rhat = un21 - q0*vn1;     // quotient digit, q0.

    while (q0 >= b || q0*vn0 > (rhat<<64) + un0) {
        q0 = q0 - 1;
        rhat = rhat + vn1;
        if (rhat >= b) break;
    }

    return {(q1<<64) + q0, ((un21<<64) + un0 - q0*v) >> s };
}

template<typename FLOAT> FLOAT SoftFPU::div(FLOAT a, FLOAT b, const RoundingModeEnum rm)
{
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto r_sign = a_sign ^ b_sign;
    auto a_exp = a.get_exp();
    auto b_exp = b.get_exp();
    auto a_frac = a.get_fraction();
    auto b_frac = b.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_frac || is_nan(b)) {
            if (is_signan(a) || is_signan(b)) {
                raise(FFLAG_INVALID);
            }
            return propagateNAN(a, b);
            // return FLOAT{FLOAT::F_QNAN};
        } else if (b_exp == FLOAT::EXP_MASK) {
            raise(FFLAG_INVALID);
            return pack_sf<FLOAT>(1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
            // return FLOAT{FLOAT::F_QNAN};
        } else {
            return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
        }
    } else if (b_exp == FLOAT::EXP_MASK) {
        if (b_frac) {
            if (is_signan(a) || is_signan(b)) {
                raise(FFLAG_INVALID);
            }
            return propagateNAN(a, b);
            // return FLOAT{FLOAT::F_QNAN};
        } else {
            return zero_sf<FLOAT>(r_sign);
        }
    }

    if (b_exp == 0) {
        if (!b_frac) {
            // zero
            if (a_exp == 0 && !a_frac) {
                raise(FFLAG_INVALID);
                return pack_sf<FLOAT>(1, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
                // return FLOAT{FLOAT::F_QNAN};
            } else {
                raise(FFLAG_DIVBYZERO);
                return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
            }
        }
        b_frac = normalize_subnormal_sf<FLOAT>(&b_exp, b_frac);
    } else {
        b_frac |= static_cast<typename FLOAT::F_UINT>(1) << FLOAT::FRAC_SIZE;
    }

    if (a_exp == 0) {
        if (!a_frac)
            return zero_sf<FLOAT>(r_sign);
        a_frac = normalize_subnormal_sf<FLOAT>(&a_exp, a_frac);
    } else {
        a_frac |= static_cast<typename FLOAT::F_UINT>(1) << FLOAT::FRAC_SIZE;
    }

    i32 r_exp = a_exp - b_exp + FLOAT::BIAS; //(1 << (FLOAT::EXP_SIZE - 1)) - 1;
    auto [r_frac, r] = divrem_u<typename FLOAT::F_UINT>(a_frac, 0, b_frac << 2);
    if (r != 0)
        r_frac |= 1;

    return normalize_sf<FLOAT>(r_sign, r_exp, r_frac, rm);
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
    a = (static_cast<F_ULONG>(ah) << F_SIZE) | al;
    u = (static_cast<F_ULONG>(1) << ((l + 1) / 2);
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
    u = static_cast<typename FLOAT::F_UINT>(1) << ((l + 1) / 2);
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

template<typename FLOAT> FLOAT SoftFPU::sqrt(FLOAT a, const RoundingModeEnum rm)
{
    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_frac = a.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_frac != 0) {
            if (is_signan(a)) {
                raise(FFLAG_INVALID);
            }
            return pack_sf<FLOAT>(a_sign, FLOAT::EXP_MASK, FLOAT::QNAN_MASK | a_frac);
            // return FLOAT{FLOAT::F_QNAN};
        } else if (a_sign) {
            raise(FFLAG_INVALID);
            return pack_sf<FLOAT>(a_sign, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
            // return FLOAT{FLOAT::F_QNAN};
        } else {
            return a; // +infinity
        }
    }
    if (a_sign) {
        if (a_exp == 0 && a_frac == 0)
            return a; // -zero
        raise(FFLAG_INVALID);
        return pack_sf<FLOAT>(a_sign, FLOAT::EXP_MASK, FLOAT::QNAN_MASK);
        // return FLOAT{FLOAT::F_QNAN};
    }
    if (a_exp == 0) {
        if (a_frac == 0)
            return zero_sf<FLOAT>(0);
        a_frac = normalize_subnormal_sf<FLOAT>(&a_exp, a_frac);
    } else {
        a_frac |= static_cast<typename FLOAT::F_UINT>(1) << FLOAT::FRAC_SIZE;
    }
    a_exp -= FLOAT::BIAS;
    /* simpler to handle an even exponent */
    if (a_exp & 1) {
        a_exp--;
        a_frac <<= 1;
    }
    a_exp = (a_exp >> 1) + FLOAT::BIAS;
    a_frac <<= (FLOAT::F_SIZE - 4 - FLOAT::FRAC_SIZE);
    if (sqrtrem_u<FLOAT>(&a_frac, a_frac, 0))
        a_frac |= 1;
    return normalize_sf<FLOAT>(a_sign, a_exp, a_frac, rm);
}

template<typename FLOAT> FLOAT SoftFPU::rsqrt(FLOAT a, const RoundingModeEnum rm)
{
    FLOAT one = FLOAT{ typename FLOAT::F_UINT{FLOAT::BIAS} << FLOAT::FRAC_SIZE };
    return div(one, sqrt(a, rm), rm);
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

template<typename FLOAT> FClassMask SoftFPU::classify(FLOAT a)
{
    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_frac = a.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) {
        // if nonzero fraction we have special values
        if (a_frac) {
            if (a_frac & FLOAT::QNAN_MASK)
                return FClassMask::QuietNaN;
            else
                return FClassMask::SignalingNaN;
        } else {
            return a_sign ? FClassMask::NegativeInfinity : FClassMask::PositiveInfinity;
        }
    } else if (a_exp == 0) {
        // if nonzero fraction we have subnorm, if zero - zero
        if (a_frac) {
            return a_sign ? FClassMask::NegativeSubnormal : FClassMask::PositiveSubnormal;
        } else {
            return a_sign ? FClassMask::NegativeZero : FClassMask::PositiveZero;
        }
    } else {
        return a_sign ? FClassMask::NegativeNormal : FClassMask::PositiveNormal;
    }
    assert(!"unreachable classify case");
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
    auto a_frac = a.get_fraction();

    if (a_exp == FLOAT_SRC::EXP_MASK) {
        if (a_frac) {
            // NaN
            if (is_signan(a)) {
                raise(FFLAG_INVALID);
            }
            const typename FLOAT_DST::F_UINT dst_frac = static_cast<typename FLOAT_DST::F_UINT>(a_frac) << (FLOAT_DST::FRAC_SIZE - FLOAT_SRC::FRAC_SIZE);
            return pack_sf<FLOAT_DST>(a_sign, FLOAT_DST::EXP_MASK, FLOAT_DST::QNAN_MASK | dst_frac);
            // return FLOAT_DST{FLOAT_DST::F_QNAN};
        } else {
            // infinity
            return pack_sf<FLOAT_DST>(a_sign, FLOAT_DST::EXP_MASK, 0);
        }
    }
    if (a_exp == 0) {
        if (!a_frac)
            return pack_sf<FLOAT_DST>(a_sign, 0, a_frac); /* zero */
        a_frac = normalize_subnormal_sf<FLOAT_SRC>(&a_exp, a_frac);
    }
    // convert the exponent value
    a_exp = a_exp - FLOAT_SRC::BIAS/*0x7f*/ + FLOAT_DST::BIAS;
    // shift the fraction
    typename FLOAT_DST::F_UINT adst_frac = static_cast<typename FLOAT_DST::F_UINT>(a_frac) << (FLOAT_DST::FRAC_SIZE - FLOAT_SRC::FRAC_SIZE); //23
    // We assume the target float is large enough to that no normalization is necessary
    return pack_sf<FLOAT_DST>(a_sign, a_exp, adst_frac);
}

template<typename FLOAT_DST, typename FLOAT_SRC>
FLOAT_DST
SoftFPU::cvt_f_f(FLOAT_SRC a, const RoundingModeEnum rm)
{
    static_assert(sizeof(FLOAT_SRC) > sizeof(FLOAT_DST));

    auto a_sign = a.get_sign();
    auto a_exp  = a.get_exp();
    auto a_frac = a.get_fraction();

    if (a_exp == FLOAT_SRC::EXP_MASK) {
        if (a_frac) {
            // NaN
            if (is_signan(a)) {
                raise(FFLAG_INVALID);
            }
            const typename FLOAT_DST::F_UINT dst_frac = static_cast<typename FLOAT_DST::F_UINT>(a_frac >> (FLOAT_SRC::FRAC_SIZE - FLOAT_DST::FRAC_SIZE));
            return pack_sf<FLOAT_DST>(a_sign, FLOAT_DST::EXP_MASK, FLOAT_DST::QNAN_MASK | dst_frac);
            // return FLOAT_DST{FLOAT_DST::F_QNAN};
        } else {
            // infinity
            return pack_sf<FLOAT_DST>(a_sign, FLOAT_DST::EXP_MASK, 0);
        }
    }
    if (a_exp == 0) {
        if (!a_frac)
            return zero_sf<FLOAT_DST>(a_sign);
        normalize_subnormal_sf<FLOAT_DST>(&a_exp, (typename FLOAT_DST::F_UINT)a_frac); // don't update a_frac here!
    } else {
        a_frac |= static_cast<typename FLOAT_SRC::F_UINT>(1) << FLOAT_SRC::FRAC_SIZE;
    }
    // convert the exponent value
    a_exp = a_exp - FLOAT_SRC::BIAS + FLOAT_DST::BIAS;
    // shift the fraction
    a_frac = rshift_rnd<FLOAT_SRC>(a_frac, FLOAT_SRC::FRAC_SIZE - (8*sizeof(FLOAT_DST)/*32*/ - 2));
    return normalize_sf<FLOAT_DST>(a_sign, a_exp, static_cast<typename FLOAT_DST::F_UINT>(a_frac), rm);
}

/*----------------------------------------------------------------------------
| Rounds the floating-point value `a' to an integer, and
| returns the result as a floating-point value.
*----------------------------------------------------------------------------*/
template<typename FLOAT>
FLOAT
SoftFPU::round(FLOAT a, const RoundingModeEnum rm, bool inexact)
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
            return pack_sf<FLOAT>(a_sign, FLOAT::EXP_MASK, FLOAT::QNAN_MASK | aSig);
            // return FLOAT{FLOAT::F_QNAN};
        } else {
            // infinity
            return pack_sf<FLOAT>(a_sign, FLOAT::EXP_MASK, 0);
        }
    }

    if ( (FLOAT::BIAS + FLOAT::FRAC_SIZE) <= aExp) {
        if ((aExp == FLOAT::EXP_MASK) && aSig) {
            return FLOAT{FLOAT::F_QNAN};
        }
        return a;
    }
    if (aExp < FLOAT::BIAS) {
        if (static_cast<typename FLOAT::F_UINT>(a.v << 1) == 0) return a;
        if (inexact) {
            raise(FFLAG_INEXACT);
        }
        switch (rm) {
         case RM_RNE:
             if ((aExp == FLOAT::BIAS - 1) && aSig) {
                 return pack_sf<FLOAT>(a_sign, FLOAT::BIAS, 0);
             }
             break;

         case RM_RTO:
         case RM_RMM:
             return pack_sf<FLOAT>(a_sign, FLOAT::BIAS, 0);

         case RM_RDN:
             return a_sign ? pack_sf<FLOAT>(a_sign, FLOAT::BIAS, 0) : zero_sf<FLOAT>(0);

         case RM_RUP:
             return a_sign ? zero_sf<FLOAT>(a_sign) : pack_sf<FLOAT>(0, FLOAT::BIAS, 0);

         default:
         case RM_RNO: // FIXME - don't emit zero in underflow for odd rounding?
         case RM_RTZ:
             break;
        }
        return zero_sf<FLOAT>(a_sign);
    }

    typename FLOAT::F_UINT lastBitMask = 1;
    lastBitMask <<= (FLOAT::BIAS + FLOAT::FRAC_SIZE) - aExp; //0x433
    typename FLOAT::F_UINT roundBitsMask = lastBitMask - 1;
    auto z = a.v;

    switch (rm) {
    case RM_RNE:
        z += lastBitMask >> 1;
        if ( !( z & roundBitsMask ) ) z &= ~ lastBitMask;
        break;
    case RM_RNO: // FIXME!
        z += lastBitMask >> 1;
        if ( !( z & roundBitsMask ) ) z |= lastBitMask;
        break;

    case RM_RMM:
    case RM_RTO:
    case RM_RTZ:
        break;

    case RM_RDN:
        if ( FLOAT{z}.get_sign()) {
            z += roundBitsMask;
        }
        break;

    case RM_RUP:
        if ( ! FLOAT{z}.get_sign()) {
            z += roundBitsMask;
        }
        break;
    }

    z &= ~ roundBitsMask;
    if ( z != a.v && inexact)
        raise(FFLAG_INEXACT);
    return FLOAT{z};
}

template<typename UNSIGNED_INTEGER, typename INTEGER> constexpr UNSIGNED_INTEGER int_nan() {
    UNSIGNED_INTEGER r_max;
    if constexpr (integer_traits<INTEGER>::is_unsigned())
        r_max = ~static_cast<UNSIGNED_INTEGER>(0);
    else
        r_max =  (static_cast<UNSIGNED_INTEGER>(1) << (8 * sizeof(INTEGER) - 1));
    return r_max;
}

template<typename INTEGER, typename FLOAT>
INTEGER
SoftFPU::cvt_f_i(FLOAT a, const RoundingModeEnum rm)
{
    static_assert(std::numeric_limits<INTEGER>::is_integer);
    static_assert(fp_traits<FLOAT>::is_fp());

    using UNSIGNED_INTEGER = typename integer_traits<INTEGER>::unsigned_type;
    u32 addend;
    UNSIGNED_INTEGER r, r_max;

    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_frac = a.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) { // NAN or INF
        raise(FFLAG_INVALID);
        //if constexpr (integer_traits<INTEGER>::is_unsigned())
        //    r_max = static_cast<UNSIGNED_INTEGER>(a_sign) - 1;
        //else
        //    r_max = (static_cast<UNSIGNED_INTEGER>(1) << (8 * sizeof(INTEGER) - 1)) - static_cast<UNSIGNED_INTEGER>(a_sign ^ 1);
        return int_nan<UNSIGNED_INTEGER, INTEGER>();
    }

    if (a_exp == FLOAT::EXP_MASK && a_frac)
        a_sign = 0; /* NaN is like +infinity */
    if (a_exp == 0) {
        a_exp = 1;
    } else {
        a_frac |= static_cast<typename FLOAT::F_UINT>(1) << FLOAT::FRAC_SIZE;
    }
    a_frac <<= FLOAT::RND_SIZE;
    a_exp = a_exp - FLOAT::BIAS - FLOAT::FRAC_SIZE;

    if constexpr (integer_traits<INTEGER>::is_unsigned())
        r_max = static_cast<UNSIGNED_INTEGER>(a_sign) - 1;
    else
        r_max = (static_cast<UNSIGNED_INTEGER>(1) << (8 * sizeof(INTEGER) - 1)) - static_cast<UNSIGNED_INTEGER>(a_sign ^ 1);

    if (a_exp >= 0) {
        if (a_exp <= static_cast<i32>(8 * sizeof(INTEGER) - 1 - FLOAT::FRAC_SIZE)) {
            r = static_cast<UNSIGNED_INTEGER>(a_frac >> FLOAT::RND_SIZE) << a_exp;
            if (r > r_max) {
                raise(FFLAG_INVALID);
                return int_nan<UNSIGNED_INTEGER, INTEGER>();
            }
        } else {
            raise(FFLAG_INVALID);
            return int_nan<UNSIGNED_INTEGER, INTEGER>();
        }
    } else {
        a_frac = rshift_rnd<FLOAT>(a_frac, -a_exp);

        switch(rm) {
        case RM_RNE:
        case RM_RMM:
            addend = (1 << (FLOAT::RND_SIZE - 1));
            break;

        case RM_RNO:
        case RM_RTO: // like rtz+jamming
        case RM_RTZ:
            addend = 0;
            break;
        default:
        case RM_RDN:
            if (a_sign)
                addend = (1 << FLOAT::RND_SIZE) - 1;
            else
                addend = 0;
            break;

        case RM_RUP:
            if (a_sign)
                addend = 0;
            else
                addend = (1 << FLOAT::RND_SIZE) - 1;
            break;
        }

        const u32 rnd_bits = static_cast<u32>(a_frac) & ((1 << FLOAT::RND_SIZE) - 1);
        a_frac = (a_frac + addend) >> FLOAT::RND_SIZE;

        // half way: select even/odd result
        switch (rm) {
        case RM_RNE:
            if (rnd_bits == (1 << (FLOAT::RND_SIZE - 1)))
                a_frac &= ~static_cast<typename FLOAT::F_UINT>(1);
            break;
        case RM_RNO:
            // std::cerr << "rnd_bits=" << rnd_bits << " sign=" << a_sign << " frac=" << fmt::hex(a_frac) << fmt::endl;
            if ((static_cast<u32>(a_frac) & 1) == 0 && rnd_bits == (1 << (FLOAT::RND_SIZE - 1)))
                a_frac |= 1;
            else if (rnd_bits > (1 << (FLOAT::RND_SIZE - 1)))
                a_frac++;
            break;

        case RM_RTO: // jamming
            // std::cerr << "rnd_bits=" << rnd_bits << " sign=" << a_sign << " frac=" << fmt::hex(a_frac) << fmt::endl;
            if (rnd_bits)
                a_frac |= 1;
            break;

        default:
            break;
        }

        if (a_frac > r_max) {
            raise(FFLAG_INVALID);
            return int_nan<UNSIGNED_INTEGER, INTEGER>();
        }
        r = static_cast<UNSIGNED_INTEGER>(a_frac);
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
SoftFPU::cvt_i_f(INTEGER a, const RoundingModeEnum rm)
{
    static_assert(std::numeric_limits<INTEGER>::is_integer);
    static_assert(fp_traits<FLOAT>::is_fp());

    using UNSIGNED_INTEGER = typename integer_traits<INTEGER>::unsigned_type;

    u32 a_sign;
    UNSIGNED_INTEGER r;

    if (!integer_traits<INTEGER>::is_unsigned() && a < 0) {
        a_sign = 1;
        r = -static_cast<UNSIGNED_INTEGER>(a);
    } else {
        a_sign = 0;
        r = a;
    }

    i32 a_exp = FLOAT::BIAS + FLOAT::F_SIZE - 2;
    // reduce range before generic float normalization
    const int l = 8*sizeof(INTEGER) - clz(r) - (FLOAT::F_SIZE - 1);
    if (l > 0) {
        //if (l >= static_cast<int>(8 * sizeof(UNSIGNED_INTEGER))) {
        //    LOG_DEBUG(LOG_EVALUATE) << "failed: l=" << l;
        //}
        assert(l < static_cast<int>(8 * sizeof(UNSIGNED_INTEGER)));
        const UNSIGNED_INTEGER mask = (static_cast<UNSIGNED_INTEGER>(1) << l) - 1;
        r = (r >> l) | ((r & mask) != 0); // jamming shifted-out bits
        a_exp += l;
    }
    typename FLOAT::F_UINT a_frac = static_cast<typename FLOAT::F_UINT>(r);
    //if (a_frac != r) {
    //    LOG_DEBUG(LOG_EVALUATE) << "failed: a_frac=" << fmt::hex(a_frac) << " r=" << fmt::hex(r);
    //}
    assert(a_frac == r);
    return normalize_sf<FLOAT>(a_sign, a_exp, a_frac, rm);
}

// Ordered comparisons raise it for both signaling and quiet NaNs.
// Unordered comparison (UCOMISS) only raises this exception if an operand is an SNaN.
template<typename FLOAT> bool SoftFPU::compare_ordered(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
        return false;
    }
    return true;
}

template<typename FLOAT> bool SoftFPU::compare_unordered(FLOAT a, FLOAT b)
{
    if (is_nan(a) || is_nan(b)) {
        if (is_signan(a) || is_signan(b)) {
            raise(FFLAG_INVALID);
        }
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
            return b_sign ? true : false;
        }
    } else {
        if (a.val() == b.val()) {
           return true;
        } else {
           return (a_sign ^ ( a.val() >= b.val() )) ? true : false;
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
            return b_sign ? true : false;
        }
    } else {
        if (a.val() == b.val()) {
           return true;
        } else {
           return (a_sign ^ ( a.val() >= b.val() )) ? true : false;
        }
    }
}

/*----------------------------------------------------------------------------
| Returns the binary log of the double-precision floating-point value `a'.
| The operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
template<typename FLOAT> FLOAT SoftFPU::log2(FLOAT a, const RoundingModeEnum rm)
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
    aSig |= F_UINT{1} << (FLOAT::FRAC_SIZE);

    i32 zSign = aExp < 0;
    F_UINT zSig = (F_UINT)(aExp) << (FLOAT::FRAC_SIZE);

    F_UINT const mask = static_cast<F_UINT>(1) << (FLOAT::FRAC_SIZE + 1);

    for (F_UINT i = static_cast<F_UINT>(1) << (FLOAT::FRAC_SIZE - 1); i /* > 0*/; i >>= 1) {
        auto [aSig0, aSig1] = mul_u<F_UINT>(aSig, aSig);
        aSig = (aSig0 << (FLOAT::F_SIZE - FLOAT::FRAC_SIZE)) | (aSig1 >> (FLOAT::FRAC_SIZE));
        if (aSig & mask) {
            aSig >>= 1;
            zSig |= i;
        }
    }

    if (zSign)
        zSig = static_cast<F_UINT>(0) - zSig; // unary minus unsigned error

    return normalize_sf<FLOAT>(zSign, FLOAT::BIAS + FLOAT::EXP_SIZE - 1, zSig, rm);
}

/* Multiply A by 2 raised to the power N.  */
template<typename FLOAT> FLOAT SoftFPU::scalbn(FLOAT a, int n, const RoundingModeEnum rm)
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
        aSig |= (static_cast<typename FLOAT::F_UINT>(1) << FLOAT::FRAC_SIZE);
    else if ( aSig == 0 )
        return a;

    i32 const ddd = 1 << (FLOAT::EXP_SIZE + 1);
    if (n > ddd) {
        n = ddd;
    } else if (n < -ddd) {
        n = -ddd;
    }

    aExp += n - 1;
    aSig <<= (FLOAT::F_SIZE - FLOAT::FRAC_SIZE - 2);
    return normalize_sf<FLOAT>(a_sign, aExp, aSig, rm);
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

template<typename FLOAT> FLOAT SoftFPU::exp(FLOAT a, const RoundingModeEnum rm)
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
        n = add(n, f_one, rm);
        xn = mul(xn, x, rm);
        xn = div(xn, n, rm);
    }

    FLOAT r = zero_sf<FLOAT>(0);
    for (int i = N - 1; i >= 0; i--) {
        r = add(r, s[i], rm);
    }
    r = add(r, f_one, rm);

    return r;
}

} // namespace fpu
} // namespace postrisc
