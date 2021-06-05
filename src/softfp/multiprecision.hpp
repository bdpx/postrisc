#pragma once

#include "scalar_uint.hpp"
#include "bigint.hpp"

namespace postrisc {
namespace multiprecision {

template<typename wrapper, size_t BITS_PER_FLOAT, size_t BITS_PER_EXPONENT>
class float_base {
public:
    using word_type = wrapper;

    static_assert(BITS_PER_FLOAT > 0, "invalid fp bit-size");
    static_assert(BITS_PER_EXPONENT > 0, "invalid exponent bit-size");
    static_assert(BITS_PER_FLOAT >= 8 * sizeof(word_type), "fp size is less than machine word size");
    static_assert(BITS_PER_FLOAT > 1 + BITS_PER_EXPONENT, "no space for fraction");
    static_assert(1 + BITS_PER_EXPONENT <= 8 * sizeof(word_type), "sign and exponent should feet in one machine word");

    constexpr bool is_float_type() const { return true; } // for SFINAE

    static size_t const BITS_PER_HIDDEN_BIT        = 1;
    static size_t const BITS_PER_SIGNIFICAND       = BITS_PER_FLOAT - BITS_PER_EXPONENT - 1;
    static size_t const BITS_PER_HIGH_SIGNIFICAND  = BITS_PER_SIGNIFICAND % (8*sizeof(word_type));
    static size_t const PRECISION                  = BITS_PER_SIGNIFICAND + BITS_PER_HIDDEN_BIT;
    // EINF is a mask from all ones
    static word_type const EINF = (static_cast<word_type>(1) << BITS_PER_EXPONENT) - 1;
    // BIAS is a mask from all ones but one bit shorter
    static word_type const BIAS = (static_cast<word_type>(1) << (BITS_PER_EXPONENT - 1)) - 1;
    static word_type const EMAX = BIAS;
    static word_type const EMIN = static_cast<word_type>(1) - EMAX;
    // mask with fraction hidden bit on
    static const word_type HIDDEN_BIT = static_cast<word_type>(1) << BITS_PER_HIGH_SIGNIFICAND;
};

template<size_t BITS_PER_FLOAT, size_t BITS_PER_EXPONENT>
class ieee_float;

template<size_t BITS_PER_EXPONENT>
class ieee_float<8, BITS_PER_EXPONENT> : public float_base<u8, 8, BITS_PER_EXPONENT>
{
private:
    scalar_uint<u8> value;
};

template<size_t BITS_PER_EXPONENT>
class ieee_float<16, BITS_PER_EXPONENT> : public float_base<u16, 16, BITS_PER_EXPONENT>
{
private:
    scalar_uint<u16> value;
};

template<size_t BITS_PER_EXPONENT>
class ieee_float<32, BITS_PER_EXPONENT> : public float_base<u32, 32, BITS_PER_EXPONENT>
{
private:
    scalar_uint<u32> value;
};

template<size_t BITS_PER_EXPONENT>
class ieee_float<64, BITS_PER_EXPONENT> : public float_base<u64, 64, BITS_PER_EXPONENT>
{
private:
    scalar_uint<u64> value;
};

template<size_t BITS_PER_FLOAT, size_t BITS_PER_EXPONENT>
class ieee_float
    : public float_base<typename uint<BITS_PER_FLOAT>::word_type, BITS_PER_FLOAT, BITS_PER_EXPONENT>
{
public:
    using word_type = typename uint<BITS_PER_FLOAT>::word_type;

public:
    //word_type get_exponent(void) const { return (uint<BITS_PER_FLOAT>::hi_limb() << 1) >> (BITS_PER_HIGH_SIGNIFICAND+1); }
    //word_type get_sign(void) const { return uint<BITS_PER_FLOAT>::hi_limb() >> (BITS_PER_EXPONENT + BITS_PER_HIGH_SIGNIFICAND); }
    uint<BITS_PER_FLOAT>& get_fraction(void) { return value; }
    const uint<BITS_PER_FLOAT>& get_fraction(void) const { return value; }

private:
    uint<BITS_PER_FLOAT> value;
};

using float8   = ieee_float<  8,  3>;
using float16  = ieee_float< 16,  5>;
using float32  = ieee_float< 32,  8>;
using float64  = ieee_float< 64, 11>;
using float128 = ieee_float<128, 15>;
using float256 = ieee_float<256, 19>;
using float512 = ieee_float<512, 23>;

static_assert(sizeof(float8) == 1);
static_assert(sizeof(float16) == 2);
static_assert(sizeof(float32) == 4);
static_assert(sizeof(float64) == 8);
static_assert(sizeof(float128) == 16);
static_assert(sizeof(float256) == 32);
static_assert(sizeof(float512) == 64);

static_assert(sizeof(float8::word_type) == 1);
static_assert(sizeof(float16::word_type) == 2);
static_assert(sizeof(float32::word_type) == 4);
static_assert(sizeof(float64::word_type) == 8);
static_assert(sizeof(float128::word_type) == 8);
static_assert(sizeof(float256::word_type) == 8);
static_assert(sizeof(float512::word_type) == 8);

} // namespace multiprecision
} // namespace postrisc
