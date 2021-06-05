#pragma once

#include "util/common.hpp"

namespace postrisc {
namespace fpu {

typedef bool carry_flag;

inline constexpr uint64_t add_init( uint64_t a, uint64_t b, carry_flag & carry )
{
    uint64_t z = a + b;
    carry = ( z < a );
    return z;
}

inline constexpr uint64_t add_next( uint64_t a, uint64_t b, carry_flag & carry )
{
    uint64_t z = a + b + carry;
    carry = ( z < a ) | ( carry & ( z == a) );
    return z;
}

inline constexpr uint64_t add_last( uint64_t a, uint64_t b, carry_flag & carry )
{
    return a + b + carry;
}

inline constexpr uint64_t sub_init( uint64_t a, uint64_t b, carry_flag & carry )
{
    uint64_t z = a - b;
    carry = ( a < b );
    return z;
}

inline constexpr uint64_t sub_next( uint64_t a, uint64_t b, carry_flag & carry )
{
    uint64_t z = a - b - carry;
    carry = ( a < b ) | ( carry & ( a == b ) );
    return z;
}

inline constexpr uint64_t sub_last( uint64_t a, uint64_t b, carry_flag & carry )
{
    return a - b - carry;
}

} // namespace fpu
} // namespace postrisc
