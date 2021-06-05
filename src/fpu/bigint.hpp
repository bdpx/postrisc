#pragma once

#include "util/common.hpp"

namespace postrisc {
namespace fpu {

typedef bool carry_flag;

inline constexpr u64 add_init( u64 a, u64 b, carry_flag & carry )
{
    u64 z = a + b;
    carry = ( z < a );
    return z;
}

inline constexpr u64 add_next( u64 a, u64 b, carry_flag & carry )
{
    u64 z = a + b + carry;
    carry = ( z < a ) | ( carry & ( z == a) );
    return z;
}

inline constexpr u64 add_last( u64 a, u64 b, carry_flag carry )
{
    return a + b + carry;
}

inline constexpr u64 sub_init( u64 a, u64 b, carry_flag & carry )
{
    u64 z = a - b;
    carry = ( a < b );
    return z;
}

inline constexpr u64 sub_next( u64 a, u64 b, carry_flag & carry )
{
    u64 z = a - b - carry;
    carry = ( a < b ) | ( carry & ( a == b ) );
    return z;
}

inline constexpr u64 sub_last( u64 a, u64 b, carry_flag carry )
{
    return a - b - carry;
}

} // namespace fpu
} // namespace postrisc
