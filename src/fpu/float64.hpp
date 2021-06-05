#pragma once

#include "float_base.hpp"

namespace postrisc {
namespace fpu {

/**************************************************************************************
* double precision
* 666655555555 5544444444443333333333222222222211111111110000000000
* 321098765432 1098765432109876543210987654321098765432109876543210
* ?sssssssssss ????????????????????????????????????????????????????  Normalized finite
* ?00000000000 nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn  Denormalized   finite
* 000000000000 0000000000000000000000000000000000000000000000000000  Positive zero
* 100000000000 0000000000000000000000000000000000000000000000000000  Negative zero
* 011111111111 0000000000000000000000000000000000000000000000000000  Positive Infinity
* 111111111111 0000000000000000000000000000000000000000000000000000  Negative Infinity
* ?11111111111 1???????????????????????????????????????????????????  QNAN
* ?11111111111 1000000000000000000000000000000000000000000000000000  QNAN indefinite
* ?11111111111 0nnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnnn  SNAN
*
* ???? - any
* nnnn - at least a single 1
* aaaa - at least single 1 but not all 1's
**************************************************************************************/

struct f64 {
    u64 v;

    using fraction_t = u64;

    FLOAT_DETAILS(64, 11, 1)

    static const u64 HIDDEN_BIT = u64(1) << BITS_PER_SIGNIFICAND;

    u64 val(void) const { return v; }
    sign_type sign(void) const { return (sign_type)(v>>63); }
    fp_exponent_t exponent(void) const { return ( v >> BITS_PER_SIGNIFICAND ) & EINF; }
    fraction_t fraction(void) const { return lobits(v, BITS_PER_SIGNIFICAND); }

    bool IsNan(void) const;
    bool IsSignalingNan(void) const;
    bool IsQuietNan(void) const;
    bool IsZero(void) const;
    bool IsInfinity(void) const;
    bool IsZeroOrDenormal(void) const;
    bool IsNeg(void) const;

    static constexpr f64 make(void) { return f64{0}; }
    static constexpr f64 make(u64 val) { return f64{val}; }
    static constexpr f64 make(sign_type sign, int power) { return f64{(u64(sign) << 63) | (u64(power + BIAS) << BITS_PER_SIGNIFICAND)}; }
    static           f64 make( double x ) { union { u64 i; double f; } u; u.f = x; return f64{u.i}; }

    double hard(void) const;
    void operator = (const double & r);
    friend bool IsBothZero(f64 a, f64 b) { return ( ( a.v | b.v )<<1 ) == 0; }

//    friend bool operator == (const f64 & a, const f64 & b) {  return ( a.v == b.v);  }
//    friend bool operator <= (const f64 & a, const f64 & b) {  return ( a.v <= b.v);  }
//    friend bool operator <  (const f64 & a, const f64 & b) {  return ( a.v <  b.v);  }
    friend bool operator != (const f64 & a, const f64 & b) {  return ( a.v != b.v);  }

    static constexpr f64 default_nan(void) { return f64::make(U64C( 0xFFF8000000000000 )); }
    static constexpr f64 inf(sign_type s) { return f64::make((u64(s) << 63) | (u64(EINF) << BITS_PER_SIGNIFICAND)); }
    static constexpr f64 zero(sign_type s) { return f64::make(u64(s) << 63); }

    f64  abs(void) const { return f64::make(v & ~(u64(1) << 63));  }
    f64  chs(void) const { return f64::make(v ^  (u64(1) << 63));  }
    f64 nabs(void) const { return f64::make(v |  (u64(1) << 63));  }

    bool IsLargerSignificant(f64 b) const;

    template<typename Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(v);
    }
};

#define F64_C(val) f64::make(U64C(val))

void writeHex(std::ostream & out, const f64 & f );

//bool IsNan(void) const { return ((val() & ~(U64C(1U) << 63)) > U64C(0x7ff0000000000000U)); }
//bool IsNan(void) const { return ( exponent() == EINF ) && fraction(); }
inline bool f64::IsNan(void) const
{
    return U64C( 0x7FF0000000000000 ) < lobits(val(), 63);
}

inline bool f64::IsSignalingNan(void) const
{
    return ( ( ( val()>>(BITS_PER_SIGNIFICAND-1) ) & 0xFFF ) == 0xFFE )
             && ( val() & U64C( 0x0007FFFFFFFFFFFF ) );
}

inline bool f64::IsQuietNan(void) const
{
    return ( U64C( 0xFFF0000000000000 ) <= ( val()<<1 ) );
}

inline bool f64::IsZero(void) const
{
    return lobits(val(), 63) == 0;
}

inline bool f64::IsInfinity(void) const
{
    return lobits(val(), 63) == U64C(0x7ff0000000000000);
}

inline bool f64::IsZeroOrDenormal(void) const
{
    return (val() & U64C(0x7ff0000000000000)) == 0;
}

inline bool f64::IsNeg(void) const
{
    return val() >> 63;
}

inline double f64::hard(void) const
{
    union {
        u64 i;
        double f;
    } u;
    u.i = v;
    return u.f;
}

inline void f64::operator = (const double & r)
{
    union {
        u64 i;
        double f;
    } u;
    u.f = r;
    v = u.i;
}

inline bool f64::IsLargerSignificant(f64 b) const
{
    return (u64(val()<<1) < u64(b.val()<<1)) ? false :
           (u64(val()<<1) > u64(b.val()<<1)) ? true  :
           (val() < b.val());
}

} //namespace fpu

namespace fmt {

template<> class hexer<fpu::f64> {
private:
    fpu::f64 v;
public:
    explicit hexer(fpu::f64 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
