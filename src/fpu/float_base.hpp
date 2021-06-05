#pragma once

#include "util/common.hpp"

namespace postrisc {
namespace fpu {

using fp_exponent_t = int32_t;

constexpr inline uint32_t makemask32(int nbits) { return (uint32_t(1) << nbits) - uint32_t(1); }
constexpr inline uint64_t makemask64(int nbits) { return (uint64_t(1) << nbits) - uint64_t(1); }

template<class T> constexpr inline T lobits(T value, int nbits)
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
enum sign_type : uint32_t {
    SIGN_POSITIVE = 0,
    SIGN_NEGATIVE = 1
};

template<class T> inline sign_type get_sign(T v) { return sign_type(v < 0); }
inline sign_type operator | (sign_type a, sign_type b) { return sign_type(uint32_t(a) | uint32_t(b)); }
inline sign_type operator & (sign_type a, sign_type b) { return sign_type(uint32_t(a) & uint32_t(b)); }
inline sign_type operator ^ (sign_type a, sign_type b) { return sign_type(uint32_t(a) ^ uint32_t(b)); }
inline sign_type operator ! (sign_type a) { return sign_type(uint32_t(a) ^ UINT32_C(1)); }

/*****************************************************************************
* Software IEC/IEEE floating-point class.
******************************************************************************/
enum float_class_t : uint8_t {
    FC_NORMAL        = 1 << 0,
    FC_NEGATIVE      = 1 << 1,
    FC_INFINITE      = 1 << 2,
    FC_QUIET_NAN     = 1 << 3,
    FC_SIGNALING_NAN = 1 << 4,
    FC_SUBNORMAL     = 1 << 5,
    FC_ZERO          = 1 << 6,
    FC_NAN           = FC_QUIET_NAN | FC_SIGNALING_NAN,
    FLOAT_ZERO,
    FLOAT_NAN,
    FLOAT_NEGATIVE_INF,
    FLOAT_POSITIVE_INF,
    FLOAT_SUBNORMAL,
    FLOAT_NORMALIZED
};

template<int TMPLT_SIZE, int TMPLT_EXPONENT, int TMPLT_HIDDENBIT>
class CBaseFloat {
public:
    constexpr bool is_float_type() const { return true; } // for SFINAE
    enum
    {
        SIZE                       = TMPLT_SIZE,
        BITS_PER_EXPONENT          = TMPLT_EXPONENT,    // round(4*log2(size)) - 13, except <= f32
        hidden_bit                 = TMPLT_HIDDENBIT,
        BITS_PER_SIGNIFICAND       = SIZE - BITS_PER_EXPONENT - 1,
        BITS_PER_HIGH_SIGNIFICAND  = BITS_PER_SIGNIFICAND % 64,
        PRECISION                  = BITS_PER_SIGNIFICAND + hidden_bit,
        EINF                       = ( 1 << BITS_PER_EXPONENT ) - 1,
        BIAS                       = ( 1 << ( BITS_PER_EXPONENT - 1 ) ) - 1,
        EMAX                       = BIAS,
        EMIN                       = 1 - EMAX
    };

};

} //namespace fpu
} // namespace postrisc
