#pragma once

namespace postrisc {
namespace util {

constexpr inline u16 change_endian(u16 num)
{
    return (num >> 8) |
           (num << 8);
}

constexpr inline u32 change_endian(u32 num)
{
    return ( (num >> 24) & U32C(0x000000ff) ) |  // move byte 3 to byte 0
           ( (num >>  8) & U32C(0x0000ff00) ) |  // move byte 2 to byte 1
           ( (num <<  8) & U32C(0x00ff0000) ) |  // move byte 1 to byte 2
           ( (num << 24) & U32C(0xff000000) );   // move byte 0 to byte 3
}

constexpr inline u64 change_endian(u64 num)
{
    return ( (num >> 56) & U64C(0x00000000000000ff) ) |  // move byte 7 to byte 0
           ( (num >> 40) & U64C(0x000000000000ff00) ) |  // move byte 6 to byte 1
           ( (num >> 24) & U64C(0x0000000000ff0000) ) |  // move byte 5 to byte 2
           ( (num >>  8) & U64C(0x00000000ff000000) ) |  // move byte 4 to byte 3
           ( (num <<  8) & U64C(0x000000ff00000000) ) |  // move byte 3 to byte 4
           ( (num << 24) & U64C(0x0000ff0000000000) ) |  // move byte 2 to byte 5
           ( (num << 40) & U64C(0x00ff000000000000) ) |  // move byte 1 to byte 6
           ( (num << 56) & U64C(0xff00000000000000) );   // move byte 0 to byte 7
}

inline u128 change_endian(u128 num)
{
    u64 const hi = change_endian(static_cast<u64>(num >> 64));
    u64 const lo = change_endian(static_cast<u64>(num));
    return u128( (static_cast<u128>(lo) << 64) | hi );
}

constexpr inline i16 change_endian(i16 num)
{
    return change_endian(static_cast<u16>(num));
}

constexpr inline i32 change_endian(i32 num)
{
    return change_endian(static_cast<u32>(num));
}

constexpr inline i64 change_endian(i64 num)
{
    return change_endian(static_cast<u64>(num));
}

constexpr inline i8 change_endian(i8 num)
{
    return num;
}

constexpr inline u8 change_endian(u8 num)
{
    return num;
}

} // namespace util
} // namespace postrisc
