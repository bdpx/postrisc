#pragma once

#include <inttypes.h>
#include <type_traits>

#include "util/common.hpp"
#include "util/fmt.hpp"
#include "bigint.hpp"

//#define LOG_OUT if (1) {} else debug_stream

// this works at least with gcc
#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined"
#endif

namespace postrisc {

namespace fpu {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"

typedef __int128 int128_t;
typedef unsigned __int128 uint128_t;
#pragma GCC diagnostic pop
}

namespace fmt {
template<> class hexer<fpu::uint128_t> {
private:
    fpu::uint128_t v;
public:
    hexer(fpu::uint128_t val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        out << fmt::hex((uint64_t)(a.v >> 64)) << '_' << fmt::hex((uint64_t)a.v);
        return out;
    }
};

template<> class hexer<uint<128>> {
private:
    uint<128> v;
public:
    hexer(uint<128> val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        out << fmt::hex((uint64_t)(a.v >> 64)) << '_' << fmt::hex((uint64_t)a.v);
        return out;
    }
};

} // namespace fmt

namespace fpu {

enum RoundingModeEnum {
    RM_RNE, /* Round to Nearest, ties to Even */
    RM_RTZ, /* Round towards Zero */
    RM_RDN, /* Round Down */
    RM_RUP, /* Round Up */
    RM_RMM, /* Round to Nearest, ties to Max Magnitude */
};

enum fp_flags : uint8_t {
    FFLAG_NOERROR     = 0,
    FFLAG_INVALID_OP  = 1 << 4,
    FFLAG_DIVIDE_ZERO = 1 << 3,
    FFLAG_OVERFLOW    = 1 << 2,
    FFLAG_UNDERFLOW   = 1 << 1,
    FFLAG_INEXACT     = 1 << 0,
};

inline fp_flags operator | ( fp_flags a, fp_flags b ) { return fp_flags( uint8_t(a) | uint8_t(b) ); }
inline fp_flags operator & ( fp_flags a, fp_flags b ) { return fp_flags( uint8_t(a) & uint8_t(b) ); }
inline fp_flags operator ~ ( fp_flags a ) { return fp_flags( ~uint8_t(a) ); }

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

enum compare_mode_t {
    FLOAT_COMPARE_LESS        = 0x01,
    FLOAT_COMPARE_GREATER     = 0x02,
    FLOAT_COMPARE_EQUAL       = 0x04,
    FLOAT_COMPARE_UNORDERED   = 0x08,
    FLOAT_COMPARE_SIGNALED    = 0x10,
    FLOAT_COMPARE_LESS_EQUAL     = FLOAT_COMPARE_LESS | FLOAT_COMPARE_EQUAL,
    FLOAT_COMPARE_GREATER_EQUAL  = FLOAT_COMPARE_GREATER | FLOAT_COMPARE_EQUAL,
    FLOAT_COMPARE_LESS_NOT_EQUAL = FLOAT_COMPARE_LESS | FLOAT_COMPARE_GREATER,
};

template<size_t FF_SIZE, size_t TEMPLATE_EXP_SIZE, typename FF_UINT>
class float_base {
public:
    using F_UINT = FF_UINT;

    static constexpr int const F_SIZE = FF_SIZE;
    static constexpr int const EXP_SIZE = TEMPLATE_EXP_SIZE;
    static constexpr int const MANT_SIZE = F_SIZE - EXP_SIZE - 1;
    static constexpr int const EXP_MASK = (1 << EXP_SIZE) - 1;
    static constexpr F_UINT const MANT_MASK = (F_UINT{1} << MANT_SIZE) - 1;
    static constexpr F_UINT const SIGN_MASK = F_UINT{1} << (F_SIZE - 1);
    static constexpr int const IMANT_SIZE = F_SIZE - 2; /* internal mantissa size */
    static constexpr int const RND_SIZE = IMANT_SIZE - MANT_SIZE;
    static constexpr F_UINT const QNAN_MASK = F_UINT{1} << (MANT_SIZE - 1);
    static constexpr F_UINT const F_QNAN = (F_UINT{EXP_MASK} << MANT_SIZE) | (F_UINT{1} << (MANT_SIZE - 1));
    static constexpr int const BIAS = (1 << (EXP_SIZE - 1)) - 1;

    static constexpr F_UINT make_bit(int bitnum) { return F_UINT{1} << bitnum; }

    uint32_t constexpr get_sign(void) const { return (uint32_t)(v >> (F_SIZE - 1)); }
    int32_t constexpr get_exp(void) const { return (uint32_t)(v >> MANT_SIZE) & EXP_MASK; }
    F_UINT constexpr get_fraction(void) const { return v & MANT_MASK; }

    F_UINT v;
};

class float32 : public float_base<32, 8, uint32_t> {
public:
    using F_ULONG = uint64_t;
    using F_HARD = float;

    F_HARD hard(void) const { union { F_UINT i; F_HARD f; } u; u.i = v; return u.f; }
};

#define FLOAT32_C(V) float32{ UINT32_C(V) }

class float64: public float_base<64, 11, uint64_t> {
public:
    using F_ULONG = uint128_t;
    using F_HARD = double;
    F_HARD hard(void) const { union { F_UINT i; F_HARD f; } u; u.i = v; return u.f; }
};

#define FLOAT64_C(V) float64{ UINT64_C(V) }

class float128 : public float_base<128, 15, uint128_t> { //uint<128>
public:
    // constexpr float128() noexcept = default;
    using F_HARD = __float128;
    //F_HARD hard(void) const { union { F_UINT i; F_HARD f; } u; u.i = v; return u.f; }
    F_HARD hard(void) const { return reinterpret_cast<F_HARD const&>(v); }
};

#define FLOAT128_C(HI, LO) float128{ (uint128_t(UINT64_C(HI)) << 64) | uint128_t(UINT64_C(LO)) }

enum muladd_negate_t : uint8_t {
    FLOAT_MULADD_NEGATE_NONE    = 0,
    FLOAT_MULADD_NEGATE_ADDEND  = 1,
    FLOAT_MULADD_NEGATE_PRODUCT = 2,
    FLOAT_MULADD_NEGATE_RESULT  = FLOAT_MULADD_NEGATE_ADDEND | FLOAT_MULADD_NEGATE_PRODUCT
};

class FPU {
public:
    constexpr FPU(RoundingModeEnum rm)
        : m_rounding_mode(rm)
        , m_exception_flags(FFLAG_NOERROR)
    { }

    constexpr RoundingModeEnum get_rounding_mode(void) const { return m_rounding_mode; }
    void set_rounding_mode(RoundingModeEnum val) { m_rounding_mode = val; }

    constexpr fp_flags get_flags(void) const { return m_exception_flags; }
    void set_exception_flags(fp_flags val) { m_exception_flags = val; }
    void clear_flags(void) { m_exception_flags = FFLAG_NOERROR; }
    void clear_flags(fp_flags mask) { m_exception_flags = m_exception_flags & (~ mask); }

    uint64_t get_control_word(void) const;
    void set_control_word( uint64_t word );
    void change_control_word(uint64_t and_mask, uint64_t or_mask);

    friend std::ostream & operator << ( std::ostream & out, const FPU& fpu );

    template<class Archive> void serialize(Archive & ar, const unsigned int version);

    template<typename FLOAT> FLOAT add(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT sub(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT mul(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT div(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT min(FLOAT a, FLOAT b);
    template<typename FLOAT> FLOAT max(FLOAT a, FLOAT b);

    template<typename FLOAT_SRC, typename FLOAT_DST> FLOAT_DST cvt_sf_sf(FLOAT_SRC a);
    template<typename FLOAT_SRC, typename FLOAT_DST> FLOAT_DST cvt_sf_sf(FLOAT_SRC a, RoundingModeEnum rm);
    template<typename FLOAT> FLOAT round_to_int(FLOAT a, RoundingModeEnum rm);
    template<typename FLOAT, typename INTEGER> INTEGER cvt_sf_i(FLOAT a, RoundingModeEnum rm);
    template<typename FLOAT, typename INTEGER> FLOAT cvt_i_sf(INTEGER a, RoundingModeEnum rm);

    template<typename FLOAT> int eq_quiet_sf(FLOAT a, FLOAT b);
    template<typename FLOAT> int lt_sf(FLOAT a, FLOAT b);
    template<typename FLOAT> int le_sf(FLOAT a, FLOAT b);
    template<class FLOAT> compare_mode_t compare_internal(FLOAT a, FLOAT b, compare_mode_t mode);

    template<typename FLOAT> uint32_t classify(FLOAT a);

    template<typename FLOAT> FLOAT muladd(FLOAT a, FLOAT b, FLOAT c, muladd_negate_t);

    template<typename FLOAT> FLOAT sqrt(FLOAT a);
    template<typename FLOAT> FLOAT log2(FLOAT a);
    template<typename FLOAT> FLOAT exp(FLOAT a);
    template<typename FLOAT> FLOAT scalbn(FLOAT a, int exp);

    template<typename FLOAT> static FLOAT pack_sf(uint32_t a_sign, uint32_t a_exp, typename FLOAT::F_UINT a_mant);
    template<typename FLOAT> static FLOAT zero_sf(uint32_t a_sign);

    void raise(fp_flags flags) { m_exception_flags = (m_exception_flags | flags); }

private:
    template<typename FLOAT> FLOAT round_pack_sf(uint32_t a_sign, int a_exp,
        typename FLOAT::F_UINT a_mant, RoundingModeEnum rm);
    template<typename FLOAT> FLOAT normalize_sf(uint32_t a_sign, int a_exp,
        typename FLOAT::F_UINT a_mant, RoundingModeEnum rm);
    template<typename FLOAT> FLOAT normalize2_sf(uint32_t a_sign, int a_exp,
        typename FLOAT::F_UINT a_mant1, typename FLOAT::F_UINT a_mant0, RoundingModeEnum rm);

private:
    RoundingModeEnum     m_rounding_mode;
    fp_flags             m_exception_flags;
};

template<class Archive>
void
FPU::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(m_rounding_mode);
    ar & BOOST_SERIALIZATION_NVP(m_exception_flags);
}

enum {
  disp_rounding_mode       = 0,
  disp_flags               = 4,
};

uint64_t
FPU::get_control_word(void) const
{
    uint64_t word = 0;
    word |= static_cast<uint64_t>(get_flags())                << disp_flags;
    word |= static_cast<uint64_t>(get_rounding_mode())        << disp_rounding_mode;
    return word;
}

void
FPU::set_control_word(uint64_t word)
{
    set_exception_flags(    (fp_flags) util::lobits(word >> disp_flags, 8) );
    set_rounding_mode( (RoundingModeEnum) util::lobits(word >> disp_rounding_mode, 2) );
}

// TODO - how to about local ieee flags? clear or keep?
void
FPU::change_control_word(uint64_t and_mask, uint64_t or_mask)
{
    const uint64_t mask = (UINT64_C(1) << 7) - 1;
    uint64_t old = get_control_word();
    uint64_t tmp = old & ~mask;
    old = ((old & mask) | or_mask);
    tmp = tmp | (old & ~and_mask);
    set_control_word( tmp );
}

std::ostream &
operator << (std::ostream & out, const FPU& fpu)
{
    return out
    << fmt::hex(fpu.get_control_word())
    << " fe=" << fpu.get_flags()
    << " rm=" << int(fpu.get_rounding_mode());
}

ALWAYS_INLINE int clz(uint32_t a)
{
    return (a == 0) ? 32 : __builtin_clz(a);
}

ALWAYS_INLINE int clz(uint64_t a)
{
    return (a == 0) ? 64 : __builtin_clzll(a);
}

ALWAYS_INLINE int clz(uint128_t a)
{
    int r;
    if (a == 0) {
        r = 128;
    } else {
        uint64_t ah, al;
        ah = a >> 64;
        al = a;
        if (ah)
            r = __builtin_clzll(ah);
        else
            r = __builtin_clzll(al) + 64;
    }
    return r;
}

template<typename FLOAT>
inline
FLOAT FPU::pack_sf(uint32_t a_sign, uint32_t a_exp, typename FLOAT::F_UINT a_mant)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " mant=" << fmt::hex(a_mant);
    return FLOAT{ ((typename FLOAT::F_UINT)a_sign << (FLOAT::F_SIZE - 1)) |
        (((typename FLOAT::F_UINT)a_exp) << FLOAT::MANT_SIZE) | 
        (a_mant & (typename FLOAT::F_UINT)FLOAT::MANT_MASK) };
}

template<typename FLOAT>
inline
FLOAT FPU::zero_sf(uint32_t sign)
{
    return pack_sf<FLOAT>(sign, 0, 0);
}

template<typename FLOAT>
typename FLOAT::F_UINT
rshift_rnd(typename FLOAT::F_UINT a, int d)
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

/* a_mant is considered to have its MSB at F_SIZE - 2 bits */
template<typename FLOAT>
FLOAT
FPU::round_pack_sf(uint32_t a_sign, int a_exp, typename FLOAT::F_UINT a_mant,
              RoundingModeEnum rm)
{
    LOG_DEBUG(LOG_EVALUATE) << " exp=" << a_exp << " mant=" << fmt::hex(a_mant);

    //LOG_OUT << "s=" << a_sign
    //        << " exp=" << a_exp
    //        << " a_mant=" << fmt::hex(a_mant)
    //        << fmt::endl;

    uint32_t addend, rnd_bits;

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
        rnd_bits = (uint32_t)a_mant & ((1U << FLOAT::RND_SIZE) - 1);
        if (is_subnormal && rnd_bits != 0) {
            raise(FFLAG_UNDERFLOW);
        }
        a_exp = 1;
    } else {
        rnd_bits = (uint32_t)a_mant & ((1U << FLOAT::RND_SIZE) - 1);
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
FPU::normalize_sf(uint32_t a_sign, int a_exp,
    typename FLOAT::F_UINT a_mant, RoundingModeEnum rm)
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
FPU::normalize2_sf(uint32_t a_sign, int a_exp,
    typename FLOAT::F_UINT a_mant1, typename FLOAT::F_UINT a_mant0,
    RoundingModeEnum rm)
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

template<typename FLOAT>
bool isinf_sf(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_mant = a.get_fraction();
    return (a_exp == FLOAT::EXP_MASK && !a_mant);
}

template<typename FLOAT>
bool isnan_sf(FLOAT a)
{
    auto const a_exp = a.get_exp();
    auto const a_mant = a.get_fraction();
    return (a_exp == FLOAT::EXP_MASK && !!a_mant);
}

template<typename FLOAT>
bool issignan_sf(FLOAT a)
{
    uint32_t const a_exp1 = uint32_t(a.v >> (FLOAT::MANT_SIZE - 1)) & ((1U << (FLOAT::EXP_SIZE + 1)) - 1);
    auto const a_mant = a.get_fraction();
    return (a_exp1 == (2 * FLOAT::EXP_MASK) && !!a_mant);
}

template<typename FLOAT>
FLOAT
FPU::add(FLOAT a, FLOAT b)
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
            if (!(a_mant & (FLOAT::QNAN_MASK << 3)) || issignan_sf(b))
                raise(FFLAG_INVALID_OP);
            return FLOAT{FLOAT::F_QNAN};
        } else if (b_exp == FLOAT::EXP_MASK && a_sign != b_sign) {
            raise(FFLAG_INVALID_OP);
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

template<typename FLOAT>
FLOAT
FPU::sub(FLOAT a, FLOAT b)
{
    b.v ^= FLOAT::SIGN_MASK;
    return add(a, b);
}

template<typename FLOAT>
inline
typename FLOAT::F_UINT normalize_subnormal_sf(int32_t *pa_exp, typename FLOAT::F_UINT a_mant)
{
    int const shift = FLOAT::MANT_SIZE - ((FLOAT::F_SIZE - 1 - clz(a_mant)));
    *pa_exp = 1 - shift;
    return a_mant << shift;
}

template<typename F_UINT>
F_UINT
mul_u(F_UINT *plow, F_UINT a, F_UINT b);

template<>
uint32_t mul_u(uint32_t *plow, uint32_t a, uint32_t b)
{
    uint64_t r = (uint64_t)a * (uint64_t)b;
    *plow = r;
    return r >> 32;
}

template<>
uint64_t mul_u(uint64_t *plow, uint64_t a, uint64_t b)
{
    uint128_t r = (uint128_t)a * (uint128_t)b;
    *plow = r;
    return r >> 64;
}

template<>
uint128_t
mul_u(uint128_t *plow, uint128_t a, uint128_t b)
{
    using F_UINT = uint128_t;
    using F_UHALF = uint64_t;

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

    *plow = ((F_UINT)r1 << FH_SIZE) | r0;
    return  ((F_UINT)r3 << FH_SIZE) | r2;
}

template<>
uint<128>
mul_u(uint<128> *plow, uint<128> a, uint<128> b)
{
    uint<256> dd = umul(a, b);

    *plow = (uint<128>)dd;
    return  (uint<128>)(dd >> 128);
}

template<typename FLOAT>
FLOAT
FPU::mul(FLOAT a, FLOAT b)
{
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto r_sign = a_sign ^ b_sign;
    auto a_exp = a.get_exp();
    auto b_exp = b.get_exp();
    auto a_mant = a.get_fraction();
    auto b_mant = b.get_fraction();

    if (a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK) {
        if (isnan_sf(a) || isnan_sf(b)) {
            if (issignan_sf(a) || issignan_sf(b)) {
                raise(FFLAG_INVALID_OP);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else {
            // infinity
            if ((a_exp == FLOAT::EXP_MASK && (b_exp == 0 && b_mant == 0)) ||
                (b_exp == FLOAT::EXP_MASK && (a_exp == 0 && a_mant == 0))) {
                raise(FFLAG_INVALID_OP);
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
    
    typename FLOAT::F_UINT r_mant, r_mant_low;
    r_mant = mul_u<typename FLOAT::F_UINT>(&r_mant_low,
        a_mant << FLOAT::RND_SIZE,
        b_mant << (FLOAT::RND_SIZE + 1));
    r_mant |= !!r_mant_low; // != 0
    return normalize_sf<FLOAT>(r_sign, r_exp, r_mant, get_rounding_mode());
}

template<typename FLOAT>
FLOAT
FPU::muladd(FLOAT a, FLOAT b, FLOAT c, muladd_negate_t negate_mask)
{
    using F_UINT = typename FLOAT::F_UINT;
    F_UINT r_mant1, r_mant0, c_mant1, c_mant0;

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
        if (isnan_sf(a) || isnan_sf(b) || isnan_sf(c)) {
            if (issignan_sf(a) || issignan_sf(b) || issignan_sf(c)) {
                raise(FFLAG_INVALID_OP);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else {
            /* infinities */
            if ((a_exp == FLOAT::EXP_MASK && (b_exp == 0 && b_mant == 0)) ||
                (b_exp == FLOAT::EXP_MASK && (a_exp == 0 && a_mant == 0)) ||
                ((a_exp == FLOAT::EXP_MASK || b_exp == FLOAT::EXP_MASK) &&
                 (c_exp == FLOAT::EXP_MASK && r_sign != c_sign)))
            {
                raise(FFLAG_INVALID_OP);
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
    
    r_mant1 = mul_u<F_UINT>(&r_mant0, a_mant << FLOAT::RND_SIZE, b_mant << FLOAT::RND_SIZE);
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

    //    printf("r_s=%d r_exp=%d r_mant=%08x %08x\n", r_sign, r_exp, (uint32_t)r_mant1, (uint32_t)r_mant0);
    //    printf("c_s=%d c_exp=%d c_mant=%08x %08x\n", c_sign, c_exp, (uint32_t)c_mant1, (uint32_t)c_mant0);

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

    //    printf("  r_mant=%08x %08x\n", (uint32_t)r_mant1, (uint32_t)r_mant0);
    //    printf("  c_mant=%08x %08x\n", (uint32_t)c_mant1, (uint32_t)c_mant0);

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
    //    printf("  r1_mant=%08x %08x\n", (uint32_t)r_mant1, (uint32_t)r_mant0);
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

/* XXX: optimize */
template<typename F_UINT>
F_UINT
divrem_u(F_UINT *pr, F_UINT a1, F_UINT a0, F_UINT b)
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
    *pr = a1;
    return a0;
}

template<>
uint32_t
divrem_u(uint32_t *pr, uint32_t ah, uint32_t al, uint32_t b)
{
    uint64_t a;
    a = ((uint64_t)ah << 32) | al;
    *pr = a % b;
    return a / b;
}

template<>
uint64_t
divrem_u(uint64_t *pr, uint64_t ah, uint64_t al, uint64_t b)
{
    uint128_t a;
    a = ((uint128_t)ah << 64) | al;
    *pr = a % b;
    return a / b;
}

template<typename FLOAT>
FLOAT
FPU::div(FLOAT a, FLOAT b)
{
    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    auto r_sign = a_sign ^ b_sign;
    auto a_exp = a.get_exp();
    auto b_exp = b.get_exp();
    auto a_mant = a.get_fraction();
    auto b_mant = b.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_mant || isnan_sf(b)) {
            if (issignan_sf(a) || issignan_sf(b)) {
                raise(FFLAG_INVALID_OP);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else if (b_exp == FLOAT::EXP_MASK) {
            raise(FFLAG_INVALID_OP);
            return FLOAT{FLOAT::F_QNAN};
        } else {
            return pack_sf<FLOAT>(r_sign, FLOAT::EXP_MASK, 0);
        }
    } else if (b_exp == FLOAT::EXP_MASK) {
        if (b_mant) {
            if (issignan_sf(a) || issignan_sf(b)) {
                raise(FFLAG_INVALID_OP);
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
                raise(FFLAG_INVALID_OP);
                return FLOAT{FLOAT::F_QNAN};
            } else {
                raise(FFLAG_DIVIDE_ZERO);
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
    int32_t r_exp = a_exp - b_exp + FLOAT::BIAS; //(1 << (FLOAT::EXP_SIZE - 1)) - 1;
    typename FLOAT::F_UINT r_mant, r;
    r_mant = divrem_u<typename FLOAT::F_UINT>(&r, a_mant, 0, b_mant << 2);
    if (r != 0)
        r_mant |= 1;
    return normalize_sf<FLOAT>(r_sign, r_exp, r_mant, get_rounding_mode());
}

#ifdef F_ULONG

/* compute sqrt(a) with a = ah*2^F_SIZE+al and a < 2^(F_SIZE - 2)
   return true if not exact square. */
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
    typename FLOAT::F_UINT u, s, r, q, sq0, sq1;

    /* 2^l >= a */
    if (a1 != 0) {
        l = 2 * F_SIZE - clz(a1 - 1);
    } else {
        if (a0 == 0) {
            *pr = 0;
            return 0;
        }
        l = F_SIZE - clz(a0 - 1);
    }
    u = (typename FLOAT::F_UINT)1 << ((l + 1) / 2);
    for(;;) {
        s = u;
        q = divrem_u(&r, a1, a0, s);
        u = (q + s) / 2;
        if (u >= s)
            break;
    }
    sq1 = mul_u<typename FLOAT::F_UINT>(&sq0, s, s);
    inexact = (sq0 != a0 || sq1 != a1);
    *pr = s;
    return inexact;
}

#endif

template<typename FLOAT>
FLOAT
FPU::sqrt(FLOAT a)
{
    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT::EXP_MASK) {
        if (a_mant != 0) {
            if (issignan_sf(a)) {
                raise(FFLAG_INVALID_OP);
            }
            return FLOAT{FLOAT::F_QNAN};
        } else if (a_sign) {
            // raise(FFLAG_INVALID_OP); // FIXME-SQRT-NEG
            return FLOAT{FLOAT::F_QNAN};
        } else {
            return a; /* +infinity */
        }
    }
    if (a_sign) {
        if (a_exp == 0 && !a_mant)
            return a; /* -zero */
        //raise(FFLAG_INVALID_OP); // FIXME-SQRT-NEG
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

/* comparisons */

template<typename FLOAT>
FLOAT
FPU::min(FLOAT a, FLOAT b)
{
    if (isnan_sf(a) || isnan_sf(b)) {
        if (issignan_sf(a) || issignan_sf(b)) {
            raise(FFLAG_INVALID_OP);
            return FLOAT{FLOAT::F_QNAN};
        } else if (isnan_sf(a)) {
            if (isnan_sf(b)) 
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

template<typename FLOAT>
FLOAT
FPU::max(FLOAT a, FLOAT b)
{
    if (isnan_sf(a) || isnan_sf(b)) {
        if (issignan_sf(a) || issignan_sf(b)) {
            raise(FFLAG_INVALID_OP);
            return FLOAT{FLOAT::F_QNAN};
        } else if (isnan_sf(a)) {
            if (isnan_sf(b)) 
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

template<typename FLOAT>
int
FPU::eq_quiet_sf(FLOAT a, FLOAT b)
{
    if (isnan_sf(a) || isnan_sf(b)) {
        if (issignan_sf(a) || issignan_sf(b)) {
            raise(FFLAG_INVALID_OP);
        }
        return 0;
    }

    if ((typename FLOAT::F_UINT)((a.v | b.v) << 1) == 0)
        return 1; /* zero case */
    return (a.v == b.v);
}

template<typename FLOAT>
int
FPU::le_sf(FLOAT a, FLOAT b)
{
    if (isnan_sf(a) || isnan_sf(b)) {
        raise(FFLAG_INVALID_OP);
        return 0;
    }

    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    if (a_sign != b_sign) {
        return (a_sign || ((typename FLOAT::F_UINT)((a.v | b.v) << 1) == 0));
    } else {
        if (a_sign) {
            return (a.v >= b.v);
        } else {
            return (a.v <= b.v);
        }
    }
}

template<typename FLOAT>
int
FPU::lt_sf(FLOAT a, FLOAT b)
{
    if (isnan_sf(a) || isnan_sf(b)) {
        raise(FFLAG_INVALID_OP);
        return 0;
    }

    auto a_sign = a.get_sign();
    auto b_sign = b.get_sign();
    if (a_sign != b_sign) {
        return (a_sign && ((typename FLOAT::F_UINT)((a.v | b.v) << 1) != 0));
    } else {
        if (a_sign) {
            return (a.v > b.v);
        } else {
            return (a.v < b.v);
        }
    }
}

template<class FLOAT>
compare_mode_t
FPU::compare_internal(FLOAT a, FLOAT b, compare_mode_t mode)
{
    if (isnan_sf(a) || isnan_sf(b)) {
        if ((mode & FLOAT_COMPARE_SIGNALED) || issignan_sf(a) || issignan_sf(b)) {
            raise(FFLAG_INVALID_OP);
        }
        return FLOAT_COMPARE_UNORDERED;
    }

    auto const a_sign = a.get_sign();
    auto const b_sign = b.get_sign();

    if (a_sign != b_sign) {
        bool const is_both_zero = ((a.v | b.v) & (((typename FLOAT::F_UINT)1 << FLOAT::F_SIZE) - 1)) == 0;
        if (is_both_zero) {
            return FLOAT_COMPARE_EQUAL;
        } else {
            return a_sign ? FLOAT_COMPARE_LESS : FLOAT_COMPARE_GREATER;
        }
    } else {
        if (a.v == b.v) {
           return FLOAT_COMPARE_EQUAL;
        } else {
           return (a_sign ^ (a.v < b.v)) ? FLOAT_COMPARE_LESS : FLOAT_COMPARE_GREATER;
        }
    }

    return mode;
}

template<typename FLOAT>
uint32_t
FPU::classify(FLOAT a)
{
    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_mant = a.get_fraction();

    uint32_t ret = 0;
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

// conversions between floats
template<typename FLOAT_SRC, typename FLOAT_DST>
FLOAT_DST
FPU::cvt_sf_sf(FLOAT_SRC a)
{
    static_assert(sizeof(FLOAT_SRC) < sizeof(FLOAT_DST));

    auto a_sign = a.get_sign();
    auto a_exp  = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT_SRC::EXP_MASK) {
        if (a_mant) {
            // NaN
            if (issignan_sf(a)) {
                raise(FFLAG_INVALID_OP);
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

template<typename FLOAT_SRC, typename FLOAT_DST>
FLOAT_DST
FPU::cvt_sf_sf(FLOAT_SRC a, RoundingModeEnum rm)
{
    static_assert(sizeof(FLOAT_SRC) > sizeof(FLOAT_DST));

    auto a_sign = a.get_sign();
    auto a_exp  = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT_SRC::EXP_MASK) {
        if (a_mant) {
            // NaN
            if (issignan_sf(a)) {
                raise(FFLAG_INVALID_OP);
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
FPU::round_to_int(FLOAT a, RoundingModeEnum rm)
{
    auto a_sign = a.get_sign();
    auto aExp   = a.get_exp();
    auto aSig   = a.get_fraction();

    if (aExp == FLOAT::EXP_MASK) {
        if (aSig) {
            // NaN
            if (issignan_sf(a)) {
                raise(FFLAG_INVALID_OP);
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
        if (!(a.v << 1)) return a;
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

template<typename FLOAT, typename INTEGER>
INTEGER
FPU::cvt_sf_i(FLOAT a, RoundingModeEnum rm)
{
    using UNSIGNED_INTEGER = typename std::make_unsigned<INTEGER>::type;
    uint32_t addend;
    UNSIGNED_INTEGER r, r_max;

    auto a_sign = a.get_sign();
    auto a_exp = a.get_exp();
    auto a_mant = a.get_fraction();

    if (a_exp == FLOAT::EXP_MASK && a_mant)
        a_sign = 0; /* NaN is like +infinity */
    if (a_exp == 0) {
        a_exp = 1;
    } else {
        a_mant |= (typename FLOAT::F_UINT)1 << FLOAT::MANT_SIZE;
    }
    a_mant <<= FLOAT::RND_SIZE;
    a_exp = a_exp - FLOAT::BIAS - FLOAT::MANT_SIZE;

    if (std::is_unsigned<INTEGER>())
        r_max = (UNSIGNED_INTEGER)a_sign - 1;
    else
        r_max = ((UNSIGNED_INTEGER)1 << (8 * sizeof(INTEGER) - 1)) - (UNSIGNED_INTEGER)(a_sign ^ 1);

    if (a_exp >= 0) {
        if (a_exp <= (int)(8 * sizeof(INTEGER) - 1 - FLOAT::MANT_SIZE)) {
            r = (UNSIGNED_INTEGER)(a_mant >> FLOAT::RND_SIZE) << a_exp;
            if (r > r_max) {
                raise(FFLAG_INVALID_OP);
                return r_max;
            }
        } else {
            raise(FFLAG_INVALID_OP);
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
        
        uint32_t const rnd_bits = ((uint32_t)a_mant) & ((1 << FLOAT::RND_SIZE ) - 1);
        a_mant = (a_mant + addend) >> FLOAT::RND_SIZE;
        // half way: select even result
        if (rm == RM_RNE && rnd_bits == (1 << (FLOAT::RND_SIZE - 1)))
            a_mant &= ~(typename FLOAT::F_UINT)1;
        if (a_mant > r_max) {
            raise(FFLAG_INVALID_OP);
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
FPU::cvt_i_sf(INTEGER a, RoundingModeEnum rm)
{
    using UNSIGNED_INTEGER = typename std::make_unsigned<INTEGER>::type;
    
    uint32_t a_sign;
    UNSIGNED_INTEGER r;

    if (!std::is_unsigned<INTEGER>() && a < 0) {
        a_sign = 1;
        r = -(UNSIGNED_INTEGER)a;
    } else {
        a_sign = 0;
        r = a;
    }
    int32_t a_exp = FLOAT::BIAS + FLOAT::F_SIZE - 2;
    /* need to reduce range before generic float normalization */
    int const l = 8*sizeof(INTEGER) - clz(r) - (FLOAT::F_SIZE - 1);
    if (l > 0) {
        UNSIGNED_INTEGER const mask = (UNSIGNED_INTEGER{1} << l) - 1;
        r = (r >> l) | ((r & mask) != 0);
        a_exp += l;
    }
    typename FLOAT::F_UINT a_mant = r;
    return normalize_sf<FLOAT>(a_sign, a_exp, a_mant, rm);
}

/*----------------------------------------------------------------------------
| Returns the binary log of the double-precision floating-point value `a'.
| The operation is performed according to the IEC/IEEE Standard for Binary
| Floating-Point Arithmetic.
*----------------------------------------------------------------------------*/
template<typename FLOAT>
FLOAT
FPU::log2(FLOAT a)
{
    using F_UINT = typename FLOAT::F_UINT;

    auto a_sign = a.get_sign();
    auto aExp = a.get_exp();
    auto aSig = a.get_fraction();

    if ( aExp == 0 ) {
        if ( aSig == 0 ) {
           raise(FFLAG_DIVIDE_ZERO);
           return pack_sf<FLOAT>(1, FLOAT::EXP_MASK, 0); // -INF
        }
        aSig = normalize_subnormal_sf<FLOAT>(&aExp, aSig);
    }
    if ( a_sign ) {
        raise(FFLAG_INVALID_OP);
        //if ( aExp == FLOAT::EXP_MASK && aSig ) return propagate_nan(a);
        return FLOAT{FLOAT::F_QNAN};
    }
    if (aExp == FLOAT::EXP_MASK) {
        if ( aSig ) return FLOAT{FLOAT::F_QNAN};
        return a;
    }

    aExp -= FLOAT::BIAS;
    aSig |= F_UINT{1} << (FLOAT::MANT_SIZE);

    int32_t zSign = aExp < 0;
    F_UINT zSig = (F_UINT)(aExp) << (FLOAT::MANT_SIZE);

    F_UINT const mask = F_UINT{1} << (FLOAT::MANT_SIZE + 1);

    for (F_UINT i = F_UINT{1} << (FLOAT::MANT_SIZE - 1); i /* > 0*/; i >>= 1) {
        F_UINT aSig0, aSig1;
        aSig0 = mul_u<F_UINT>(&aSig1, aSig, aSig);
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

template<typename FLOAT>
FLOAT
FPU::exp(FLOAT a)
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
    for (int i = N-1; i>=0; i--) {
        r = add(r, s[i]);
    }
    r = add(r, f_one);

    return r;
}

} // namespace fpu
} // namespace postrisc
