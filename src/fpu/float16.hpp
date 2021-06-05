#pragma once

#include "float_base.hpp"

namespace postrisc {
namespace fpu {

/**************************************************************************************
* half-float precision
* 111111 0000000000
* 543210 9876543210
* ?sssss ??????????  Normalized finite
* ?00000 nnnnnnnnnn  Denormalized finite
* 000000 0000000000  Positive zero
* 100000 0000000000  Negative zero
* 011111 0000000000  Positive Infinity
* 111111 0000000000  Negative Infinity
* ?11111 1?????????  QNAN
* ?11111 1000000000  QNAN indefinite
* ?11111 0nnnnnnnnn  SNAN
*
* ???? - any
* nnnn - at least a single 1
* aaaa - at least single 1 but not all 1's
**************************************************************************************/

class f16 {
public:
    u16 v;

    FLOAT_DETAILS(16, 5, 1)

public:
    static const u16 HIDDEN_BIT = U16C(1) << BITS_PER_SIGNIFICAND; //0x0400

    u16 val(void) const { return v; }

    sign_type sign(void) const { return (sign_type)(v>>15); }
    fp_exponent_t exponent(void) const { return ( v >> BITS_PER_SIGNIFICAND ) & EINF; }
    u16 fraction(void) const { return lobits(v, BITS_PER_SIGNIFICAND); }

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    constexpr explicit f16(void) : v(0) {}
    constexpr explicit f16(u16 val) : v(val) {}

//    friend bool operator == (const f16 & a, const f16 & b) {  return ( a.v == b.v);  }
//    friend bool operator <= (const f16 & a, const f16 & b) {  return ( a.v <= b.v);  }
//    friend bool operator <  (const f16 & a, const f16 & b) {  return ( a.v <  b.v);  }
//    friend bool operator != (const f16 & a, const f16 & b) {  return ( a.v != b.v);  }

    static constexpr f16 default_nan(void) { return f16(0xFE00); }
    static constexpr f16 inf(sign_type s)  { return f16((static_cast<u16>(s) << 15) | (u16(EINF) << BITS_PER_SIGNIFICAND)); }
    static constexpr f16 zero(sign_type s) { return f16(static_cast<u16>(s) << 15); }

    f16  abs(void) const { return f16(v & ~(U16C(1) << 15));  }
    f16  chs(void) const { return f16(v ^  (U16C(1) << 15));  }
    f16 nabs(void) const { return f16(v |  (U16C(1) << 15));  }

    bool IsLargerSignificant(f16 b) const;

    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(v);
    }
};

//bool IsNan(void) const { return ( exponent() == EINF ) && fraction(); }
inline bool f16::IsNan(void) const
{
    return ((val() & 0x7fff) > 0x7c00);
}

inline bool f16::IsSignalingNan(void) const
{
    return (((val() >> (BITS_PER_SIGNIFICAND-1)) & 0x3F) == 0x3E)
           && (val() & 0x1FF);
}

inline bool f16::IsQuietNan(void) const
{
    return ((val() & 0x7fff) >= 0x7c80);
}

inline bool f16::IsZero(void) const
{
    return lobits(val(), 15) == 0;
}

inline bool f16::IsInfinity(void) const
{
    return (val() & 0x7fff) == 0x7c00;
}

inline bool f16::IsZeroOrDenormal(void) const
{
    return (val() & 0x7c00) == 0;
}

inline bool f16::IsNeg(void) const
{
    return val() >> 15;
}

inline bool f16::IsLargerSignificant(f16 b) const
{
    return (u16(val()<<1) < u16(b.val()<<1)) ? false :
           (u16(val()<<1) > u16(b.val()<<1)) ? true  :
           (val() < b.val());
}

} //namespace fpu
} // namespace postrisc
