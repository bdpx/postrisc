#pragma once

#include "util/common.hpp"

namespace postrisc {
namespace fpu {

using fp_exponent_t = i32;

constexpr inline u32 makemask32(int nbits) { return (u32(1) << nbits) - u32(1); }
constexpr inline u64 makemask64(int nbits) { return (u64(1) << nbits) - u64(1); }

template<typename T> constexpr inline T lobits(T value, int nbits)
{
    return value & ( (T(1) << nbits) - T(1) );
}

/*****************************************************************************************
| Software IEC/IEEE floating-point ordering relations
******************************************************************************************/
enum float_relation {
    float_relation_less      = -1,
    float_relation_equal     =  0,
    float_relation_greater   =  1,
    float_relation_unordered =  2
};

/*****************************************************************************************
| Software IEC/IEEE floating-point types.
******************************************************************************************/
enum sign_type : u32 {
    SIGN_POSITIVE = 0,
    SIGN_NEGATIVE = 1
};

template<typename T> inline sign_type get_sign(T v) { return sign_type(v < 0); }
inline sign_type operator | (sign_type a, sign_type b) { return sign_type(u32(a) | u32(b)); }
inline sign_type operator & (sign_type a, sign_type b) { return sign_type(u32(a) & u32(b)); }
inline sign_type operator ^ (sign_type a, sign_type b) { return sign_type(u32(a) ^ u32(b)); }
inline sign_type operator ! (sign_type a) { return sign_type(u32(a) ^ U32C(1)); }

/*****************************************************************************
* Software IEC/IEEE floating-point class.
******************************************************************************/
enum float_class_t : u8 {
    FC_ZERO          = 1 << 0,
    FC_NEGATIVE      = 1 << 1,
    FC_POSITIVE      = 1 << 2,
    FC_INFINITE      = 1 << 3,
    FC_NORMAL        = 1 << 4,
    FC_SUBNORMAL     = 1 << 5,
    FC_QUIET_NAN     = 1 << 6,
    FC_SIGNALING_NAN = 1 << 7,
    FC_NAN           = FC_QUIET_NAN | FC_SIGNALING_NAN,
};

#define FLOAT_DETAILS(TMPLT_SIZE, TMPLT_EXPONENT, TMPLT_HIDDENBIT)                                                   \
    constexpr bool is_float_type() const { return true; } /* for SFINAE */                                           \
    static int const SIZE                       = TMPLT_SIZE;                                                        \
    static int const BITS_PER_EXPONENT          = TMPLT_EXPONENT; /* round(4*log2(size)) - 13, except <= f32 */      \
    static int const hidden_bit                 = TMPLT_HIDDENBIT;                                                   \
    static int const BITS_PER_SIGNIFICAND       = SIZE - BITS_PER_EXPONENT - 1;                                      \
    static int const BITS_PER_HIGH_SIGNIFICAND  = BITS_PER_SIGNIFICAND % 64;                                         \
    static int const PRECISION                  = BITS_PER_SIGNIFICAND + hidden_bit;                                 \
    static int const EINF                       = ( 1 << BITS_PER_EXPONENT ) - 1;                                    \
    static int const BIAS                       = ( 1 << ( BITS_PER_EXPONENT - 1 ) ) - 1;                            \
    static int const EMAX                       = BIAS;                                                              \
    static int const EMIN                       = 1 - EMAX;                                                          \


} //namespace fpu
} // namespace postrisc
