#pragma once

#include "float_base.hpp"

namespace postrisc {
namespace fpu {

/**************************************************************************************
* single precision
* 332222222 22211111111110000000000
* 109876543 21098765432109876543210
* ?ssssssss ???????????????????????  Normalized finite
* ?00000000 nnnnnnnnnnnnnnnnnnnnnnn  Denormalized   finite
* 000000000 00000000000000000000000  Positive zero
* 100000000 00000000000000000000000  Negative zero
* 011111111 00000000000000000000000  Positive Infinity
* 111111111 00000000000000000000000  Negative Infinity
* ?11111111 1??????????????????????  QNAN
* ?11111111 10000000000000000000000  QNAN indefinite
* ?11111111 0nnnnnnnnnnnnnnnnnnnnnn  SNAN
*
* ???? - any
* nnnn - at least a single 1
* aaaa - at least single 1 but not all 1's
**************************************************************************************/

struct f32 {
    u32 v;

    using fraction_t = u32;

    FLOAT_DETAILS(32, 8, 1)

    enum : u32 {
        HIDDEN_BIT = u32(1) << BITS_PER_SIGNIFICAND,  //0x00800000;
        SIGN_BIT   = u32(1) << 31,
        QUIET_BIT  = u32(1) << (BITS_PER_SIGNIFICAND - 1)
    };

    fraction_t val(void) const { return v; }
    sign_type sign(void) const { return (sign_type)(v >> 31); }
    fp_exponent_t exponent(void) const { return ( v >> BITS_PER_SIGNIFICAND ) & EINF; }
    fraction_t fraction(void) const { return lobits(v, BITS_PER_SIGNIFICAND); }

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    static constexpr f32 make(void) { return f32{0}; }
    static constexpr f32 make(u32 val) { return f32{val}; }
    static constexpr f32 make(sign_type sign, int power) { return f32{(u32(sign) << 31) | (u32(power + BIAS) << BITS_PER_SIGNIFICAND) }; }
    static           f32 make( float x ) { union { fraction_t i; float f; } u; u.f = x; return f32{u.i}; }

    float hard(void) const { union { fraction_t i; float f; } u; u.i = v; return u.f; }

    void operator = (const float & r);

    friend bool IsBothZero(f32 a, f32 b) { return ( ( a.v | b.v )<<1 ) == 0; }

//    friend bool operator == (const f32 & a, const f32 & b) {  return ( a.v == b.v);  }
//    friend bool operator <= (const f32 & a, const f32 & b) {  return ( a.v <= b.v);  }
//    friend bool operator <  (const f32 & a, const f32 & b) {  return ( a.v <  b.v);  }
    friend bool operator != (const f32 & a, const f32 & b) {  return ( a.v != b.v);  }

    static constexpr f32 default_nan(void) { return f32::make(0xFFC00000); }
    static constexpr f32 inf(sign_type s) { return f32::make((u32(s) << 31) | (u32(EINF) << BITS_PER_SIGNIFICAND)); }
    static constexpr f32 zero(sign_type s) { return f32::make(u32(s) << 31); }

    f32  abs(void) const { return f32::make( val() & ~SIGN_BIT );  }
    f32  chs(void) const { return f32::make( val() ^  SIGN_BIT );  }
    f32 nabs(void) const { return f32::make( val() |  SIGN_BIT );  }

    bool IsLargerSignificant(f32 b) const;

    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(v);
    }
};

#define F32_C(val) f32::make(U32C(val))

void writeHex(std::ostream & out, const f32 & f );

//bool IsNan(void) const { return 0x7F800000 < lobits(val(), 31); }
//bool IsNan(void) const { return ( exponent() == EINF ) && fraction(); }
inline bool f32::IsNan(void) const
{
    return lobits(val(), 31) > 0x7f800000U;
}

inline bool f32::IsSignalingNan(void) const
{
    return ( ( ( val()>>(BITS_PER_SIGNIFICAND-1) ) & 0x1FF ) == 0x1FE ) && ( val() & 0x003FFFFF );
}

inline bool f32::IsQuietNan(void) const
{
    return ( (val()<<1 ) >= 0xFF800000U );
}

inline bool f32::IsZero(void) const
{
    return lobits(val(), 31) == 0;
}

inline bool f32::IsInfinity(void) const
{
    return lobits(val(), 31) == 0x7f800000;
}

inline bool f32::IsZeroOrDenormal(void) const
{
    return (val() & 0x7f800000) == 0;
}

inline bool f32::IsNeg(void) const
{
    return val() >> 31;
}

inline void f32::operator = (const float & r)
{
    union {
        u32 i;
        float f;
    } u;

    u.f = r;
    v = u.i;
}

inline bool f32::IsLargerSignificant(f32 b) const
{
    return (u32(val()<<1) < u32(b.val()<<1)) ? false :
           (u32(val()<<1) > u32(b.val()<<1)) ? true  :
           (val() < b.val());
}

} //namespace fpu

namespace fmt {
template<> class hexer<fpu::f32> {
private:
    fpu::f32 v;
public:
    explicit hexer(fpu::f32 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
