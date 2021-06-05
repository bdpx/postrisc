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

class f64 : public CBaseFloat<64,11,1> {
public:
    using fraction_t = uint64_t;
    static const uint64_t HIDDEN_BIT = uint64_t(1) << BITS_PER_SIGNIFICAND;

    uint64_t val(void) const { return v; }
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

    constexpr explicit f64(void) : v(0) {}
    constexpr explicit f64(uint64_t val) : v(val) {}
    constexpr explicit f64(sign_type sign, int power) : v( (uint64_t(sign) << 63) | (uint64_t(power + BIAS) << BITS_PER_SIGNIFICAND) ) {}
    explicit f64( double x ) { union { uint64_t i; double f; } u; u.f = x; v = u.i; }

    double hard(void) const;
    void operator = (const double & r);
    friend bool IsBothZero(f64 a, f64 b) { return ( ( a.v | b.v )<<1 ) == 0; }

//    friend bool operator == (const f64 & a, const f64 & b) {  return ( a.v == b.v);  }
//    friend bool operator <= (const f64 & a, const f64 & b) {  return ( a.v <= b.v);  }
//    friend bool operator <  (const f64 & a, const f64 & b) {  return ( a.v <  b.v);  }
    friend bool operator != (const f64 & a, const f64 & b) {  return ( a.v != b.v);  }

    static constexpr f64 default_nan(void) { return f64(UINT64_C( 0xFFF8000000000000 )); }
    static constexpr f64 inf(sign_type s) { return f64((uint64_t(s) << 63) | (uint64_t(EINF) << BITS_PER_SIGNIFICAND)); }
    static constexpr f64 zero(sign_type s) { return f64(uint64_t(s) << 63); }

    f64  abs(void) const { return f64(v & ~(uint64_t(1) << 63));  }
    f64  chs(void) const { return f64(v ^  (uint64_t(1) << 63));  }
    f64 nabs(void) const { return f64(v |  (uint64_t(1) << 63));  }

    bool IsLargerSignificant(f64 b) const;

    template<class Archive> void serialize(Archive & ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(v);
    }
private:
    uint64_t v;
};

void writeHex(std::ostream & out, const f64 & f );

//bool IsNan(void) const { return ((val() & ~(UINT64_C(1U) << 63)) > UINT64_C(0x7ff0000000000000U)); }
//bool IsNan(void) const { return ( exponent() == EINF ) && fraction(); }
inline bool f64::IsNan(void) const
{
    return UINT64_C( 0x7FF0000000000000 ) < lobits(val(), 63);
}

inline bool f64::IsSignalingNan(void) const
{
    return ( ( ( val()>>(BITS_PER_SIGNIFICAND-1) ) & 0xFFF ) == 0xFFE )
             && ( val() & UINT64_C( 0x0007FFFFFFFFFFFF ) );
}

inline bool f64::IsQuietNan(void) const
{
    return ( UINT64_C( 0xFFF0000000000000 ) <= ( val()<<1 ) );
}

inline bool f64::IsZero(void) const
{
    return lobits(val(), 63) == 0;
}

inline bool f64::IsInfinity(void) const
{
    return lobits(val(), 63) == UINT64_C(0x7ff0000000000000);
}

inline bool f64::IsZeroOrDenormal(void) const
{
    return (val() & UINT64_C(0x7ff0000000000000)) == 0;
}

inline bool f64::IsNeg(void) const
{
    return val() >> 63;
}

inline double f64::hard(void) const
{
    union {
        uint64_t i;
        double f;
    } u;
    u.i = v;
    return u.f;
}

inline void f64::operator = (const double & r)
{
    union {
        uint64_t i;
        double f;
    } u;
    u.f = r;
    v = u.i;
}

inline bool f64::IsLargerSignificant(f64 b) const
{
    return (uint64_t(val()<<1) < uint64_t(b.val()<<1)) ? false :
           (uint64_t(val()<<1) > uint64_t(b.val()<<1)) ? true  :
           (val() < b.val());
}

} //namespace fpu

namespace fmt {

template<> class hexer<fpu::f64> {
private:
    fpu::f64 v;
public:
    hexer(fpu::f64 val) : v(val) {}
    friend std::ostream & operator << (std::ostream & out, const hexer a)
    {
        fpu::writeHex(out, a.v);
        return out;
    }
};

} // namespace fmt
} // namespace postrisc
