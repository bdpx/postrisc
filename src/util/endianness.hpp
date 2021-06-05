#pragma once

#include "fpu/u128.hpp"

namespace postrisc {
namespace util {

using postrisc::fpu::u128;

constexpr inline uint16_t change_endian(uint16_t num)
{
    return (num >> 8) |
           (num << 8);
}

constexpr inline uint32_t change_endian(uint32_t num)
{
    return ( (num >> 24) & UINT32_C(0x000000ff) ) |  // move byte 3 to byte 0
           ( (num >>  8) & UINT32_C(0x0000ff00) ) |  // move byte 2 to byte 1
           ( (num <<  8) & UINT32_C(0x00ff0000) ) |  // move byte 1 to byte 2
           ( (num << 24) & UINT32_C(0xff000000) );   // move byte 0 to byte 3
}

constexpr inline uint64_t change_endian(uint64_t num)
{
    return ( (num >> 56) & UINT64_C(0x00000000000000ff) ) |  // move byte 7 to byte 0
           ( (num >> 40) & UINT64_C(0x000000000000ff00) ) |  // move byte 6 to byte 1
           ( (num >> 24) & UINT64_C(0x0000000000ff0000) ) |  // move byte 5 to byte 2
           ( (num >>  8) & UINT64_C(0x00000000ff000000) ) |  // move byte 4 to byte 3
           ( (num <<  8) & UINT64_C(0x000000ff00000000) ) |  // move byte 3 to byte 4
           ( (num << 24) & UINT64_C(0x0000ff0000000000) ) |  // move byte 2 to byte 5
           ( (num << 40) & UINT64_C(0x00ff000000000000) ) |  // move byte 1 to byte 6
           ( (num << 56) & UINT64_C(0xff00000000000000) );   // move byte 0 to byte 7
}

inline u128 change_endian(u128 num)
{
    return u128( change_endian(num.m1), change_endian(num.m0) );
}

constexpr inline int16_t change_endian(int16_t num)
{
    return change_endian(static_cast<uint16_t>(num));
}

constexpr inline int32_t change_endian(int32_t num)
{
    return change_endian(static_cast<uint32_t>(num));
}

constexpr inline int64_t change_endian(int64_t num)
{
    return change_endian(static_cast<uint64_t>(num));
}

constexpr inline int8_t change_endian(int8_t num)
{
    return num;
}

constexpr inline uint8_t change_endian(uint8_t num)
{
    return num;
}

} // namespace util
} // namespace postrisc
