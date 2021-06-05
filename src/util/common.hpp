#pragma once

#include <type_traits>

#define ARCH_NAME        "POSTRISC"
#define ARCH_COPYRIGHT   "Copyright (C) 2003-" PACKAGE_DATE_YEAR " Dmitry Buvaylo."

/***************************************************************************
* this file contain base infrastructure for my C++
* PS:
* I don't really know C, but I can fake it so the compiler thinks I know.
***************************************************************************/

#include "fmt.hpp"
#include "logger.hpp"

namespace postrisc {

template <typename T, size_t N> inline constexpr size_t sizearray( T (&)[N] )  { return N; }

namespace util {


void setup(void);
bool load_file(const std::string& filepath, std::vector<std::byte> &data);
void terminate(bool abort = false);

inline bool isoctal(char value)
{
    return value >= '0' && value <= '7';
}

inline bool isbinary(char value)
{
    return value == '0' || value == '1';
}

/***************************************************************************
* wrapper for array initialization
* for objects without default constructor
***************************************************************************/
template<typename T, T default_value> class default_initializer {
    public:
        constexpr default_initializer() : value(default_value) {}
        explicit constexpr default_initializer(T v) : value(v) {}
        operator T () const { return value; }
        default_initializer& operator=(T v) { value = v; return *this; }
    private:
        T value;
};


/***************************************************************************
* one bits mask form size, ones began from zero bit
***************************************************************************/
template<typename T> constexpr inline T makemask(unsigned int nbits)
{
    static_assert(std::is_same<T, i128>::value ||
                  std::is_same<T, u128>::value ||
                  std::is_integral<T>::value);

    return (nbits < sizeof(T)*8) ? ((static_cast<T>(1) << nbits) - static_cast<T>(1)) : (~static_cast<T>(0));
}

template<typename T> constexpr inline T lobits(T value, int nbits)
{
    return value & ( (T(1) << nbits) - T(1) );
}

/***************************************************************************
* architecture use implementation dependent endian mode.
* to assume assembler right order, all references to instruction bundles accessed
* at 8-byte (double word) granularity level with compiler-driven endianness control.
* one non-portable problem - order of 8-bytes in 16-byte instruction bundle or vector register.
* we assume little-endian order is default, but must use lopart/hipart
* constants for access least/most significant 8-bytes in 16-byte item.
***************************************************************************/
enum { lopart = 0, hipart = 1 };


/***************************************************************************
* sign or zero extension functions
***************************************************************************/

inline i32 sext32(i32 src, unsigned int bitnum) { return (src << (32 - bitnum)) >> (32 - bitnum); }
inline u32 zext32(u32 src, unsigned int bitnum) { return (src << (32 - bitnum)) >> (32 - bitnum); }
inline i64 sext64(i64 src, unsigned int bitnum) { return (src << (64 - bitnum)) >> (64 - bitnum); }
inline u64 zext64(u64 src, unsigned int bitnum) { return (src << (64 - bitnum)) >> (64 - bitnum); }

/***************************************************************************
* alignment to low numbits-power of 2 by mask least significant bits.
***************************************************************************/

template<typename T> constexpr inline T rounddown(T val, unsigned int numbits)
{
   return val & ~makemask<T>(numbits);
}

template<typename T> constexpr inline T roundup(T val, unsigned int numbits)
{
   return (val + makemask<T>(numbits)) & ~makemask<T>(numbits);
}

template<typename T> constexpr inline bool is_pow2(T value)
{
    return (value & (value - 1)) == 0;
}

template<typename T> constexpr inline T rounddown_pow2(T value, T pow2)
{
    assert(is_pow2(pow2));
    return value & ~(pow2 - 1);
}

template<typename T> constexpr inline T roundup_pow2(T value, T pow2)
{
    assert(is_pow2(pow2));
    return rounddown_pow2(value + pow2 - 1, pow2);
}

template<typename D, typename T> constexpr bool aligned(T value)
{
    return (value & static_cast<T>(sizeof(D) - 1)) == 0;
}

static_assert( aligned<u8 >(U64C(0x123450)));
static_assert( aligned<u16>(U64C(0x123450)));
static_assert(!aligned<u16>(U64C(0x123451)));
static_assert( aligned<u32>(U64C(0x123450)));
static_assert(!aligned<u32>(U64C(0x123452)));
static_assert( aligned<u64>(U64C(0x123450)));
static_assert(!aligned<u64>(U64C(0x123454)));

inline void *rounddown_pow2(void *ptr, size_t pow2)
{
    size_t p = reinterpret_cast<size_t>(ptr);
    p = rounddown_pow2(p, pow2);
    return reinterpret_cast<void *>(p);
}

static_assert(is_pow2<u8>(0x01));
static_assert(is_pow2<u8>(0x02));
static_assert(is_pow2<u8>(0x04));
static_assert(is_pow2<u8>(0x08));
static_assert(is_pow2<u8>(0x10));
static_assert(is_pow2<u8>(0x20));
static_assert(is_pow2<u8>(0x40));
static_assert(is_pow2<u8>(0x80));

static_assert(!is_pow2<u8>(0x03));
static_assert(!is_pow2<u8>(0x05));
static_assert(!is_pow2<u8>(0x30));
static_assert(!is_pow2<u8>(0x71));

template<typename T> constexpr inline bool is_aligned(T value, T alignment)
{
    return (value & (alignment - 1)) == 0;
}

/////////////////////////////////////////////////////////
//
class population {
private:
    template<typename T>
    static inline T helper_step(T sum, T mask, unsigned shift)
    {
        return (sum & mask) + ((sum >> shift) & mask);
    }

public:
    static u64 func(u64 sum)
    {
        // ZZZZ(mask, shift) sum = (sum & mask) + ((sum >> (shift)) & mask)
        sum = helper_step(sum, U64C( 0x5555555555555555 ),  1 );
        sum = helper_step(sum, U64C( 0x3333333333333333 ),  2 );
        sum = helper_step(sum, U64C( 0x0f0f0f0f0f0f0f0f ),  4 );
        sum = helper_step(sum, U64C( 0x00ff00ff00ff00ff ),  8 );
        sum = helper_step(sum, U64C( 0x0000ffff0000ffff ), 16 );
        sum = helper_step(sum, U64C( 0x00000000ffffffff ), 32 );

        return sum;
    }

    static u32 func(u32 sum)
    {
        // ZZZZ(mask, shift) sum = (sum & mask) + ((sum >> (shift)) & mask)
        sum = helper_step(sum, U32C( 0x55555555 ),  1 );
        sum = helper_step(sum, U32C( 0x33333333 ),  2 );
        sum = helper_step(sum, U32C( 0x0f0f0f0f ),  4 );
        sum = helper_step(sum, U32C( 0x00ff00ff ),  8 );
        sum = helper_step(sum, U32C( 0x0000ffff ), 16 );

        return sum;
    }
};

class count_leading_zeros {
public:
    static unsigned func(u64 z)
    {
        assert(z != 0);

        unsigned result = 0;

        if ((z & 0xffffffff) == z) {  result += 32;  }
        if ((z & 0x0000ffff) == z) {  result += 16;  }
        if ((z & 0x000000ff) == z) {  result +=  8;  }
        if ((z & 0x0000000f) == z) {  result +=  4;  }
        if ((z & 0x00000003) == z) {  result +=  2;  }
        if ((z & 0x00000001) == z) {  result +=  1;  }
        return result;
    }
};

class count_trailing_zeros {
public:
    static unsigned func(u64 z)
    {
        assert(z != 0);

        unsigned result = 0;

        if ((z & 0xffffffff) == 0) {  result += 32; z >>= 32; }
        if ((z & 0x0000ffff) == 0) {  result += 16; z >>= 16; }
        if ((z & 0x000000ff) == 0) {  result +=  8; z >>=  8; }
        if ((z & 0x0000000f) == 0) {  result +=  4; z >>=  4; }
        if ((z & 0x00000003) == 0) {  result +=  2; z >>=  2; }
        if ((z & 0x00000001) == 0) {  result +=  1;  }
        return result;
    }
};

class count_trailing_ones {
public:
    static unsigned func(u64 z)
    {
        unsigned result = 0;

        if ((z & 0xffffffff) == 0xffffffff) {  z >>= 32;   result += 32;  }
        if ((z & 0x0000ffff) == 0x0000ffff) {  z >>= 16;   result += 16;  }
        if ((z & 0x000000ff) == 0x000000ff) {  z >>=  8;   result +=  8;  }
        if ((z & 0x0000000f) == 0x0000000f) {  z >>=  4;   result +=  4;  }
        if ((z & 0x00000003) == 0x00000003) {  z >>=  2;   result +=  2;  }
        if ((z & 0x00000001) == 0x00000001) {  z >>=  1;   result +=  1;  }

        return result;
    }
};

/*
The WEITEK XL-Series RISC processors do this in one cycle with the
Perfect Exchange (pexch) instruction, which does field swaps on 2, 4, 8, 16, and 32-bit boundaries.
An endian-conversion would be done with (source and dest are registers)
 pexch source, 0b1100, dest
a bit-reverse operation (move bit 0 to bit 31, bit 1 to bit 30, etc.) is done with
 pexch source, 0b1111, dest
and so on.
*/
class permute {
private:
    template<typename T>
    static inline T helper_step(T sum, T mask, unsigned shift)
    {
        return ((sum & mask) << shift) | ((sum & ~mask) >> shift);
    }

public:
    static u64 func(u64 sum, unsigned shift_mask)
    {
        // ZZZZ(mask, shift) sum = ((sum & mask) << (shift)) | ((sum & ~mask) >> (shift))
        sum = helper_step(sum, U64C( 0x5555555555555555 ), shift_mask &  1 );
        sum = helper_step(sum, U64C( 0x3333333333333333 ), shift_mask &  2 );
        sum = helper_step(sum, U64C( 0x0f0f0f0f0f0f0f0f ), shift_mask &  4 );
        sum = helper_step(sum, U64C( 0x00ff00ff00ff00ff ), shift_mask &  8 );
        sum = helper_step(sum, U64C( 0x0000ffff0000ffff ), shift_mask & 16 );
        sum = helper_step(sum, U64C( 0x00000000ffffffff ), shift_mask & 32 );

        return sum;
    }

    static u32 func(u32 sum, unsigned shift_mask)
    {
        // ZZZZ(mask, shift) sum = ((sum & mask) << (shift)) | ((sum & ~mask) >> (shift))
        sum = helper_step(sum, U32C( 0x55555555 ), shift_mask &  1 );
        sum = helper_step(sum, U32C( 0x33333333 ), shift_mask &  2 );
        sum = helper_step(sum, U32C( 0x0f0f0f0f ), shift_mask &  4 );
        sum = helper_step(sum, U32C( 0x00ff00ff ), shift_mask &  8 );
        sum = helper_step(sum, U32C( 0x0000ffff ), shift_mask & 16 );

        return sum;
    }
};

/////////////////////////////////////////////////////////
// gray codes
inline u64 gray2bin(u64 gray)
{
    gray ^= (gray >> 32);
    gray ^= (gray >> 16);
    gray ^= (gray >> 8);
    gray ^= (gray >> 4);
    gray ^= (gray >> 2);
    gray ^= (gray >> 1);
    return gray;
}

inline u64 bin2gray(u64 num)
{
    return (num >> 1) ^ num;
}

} // namespace util

template<typename T>
constexpr inline T AddCarryOutVector(T op1, T op2)
{
    return (op1 & op2) | ((op1 | op2) & (~(op1 + op2)));
}

template<typename T>
constexpr inline T SubCarryOutVector(T op1, T op2)
{
    return (~op1 & op2) | ((~(op1 ^ op2)) & (op1 - op2));
}

} // namespace postrisc

// Define operator overloads
// to enable bit operations on enum values that are used to define flags.
// Use DEFINE_ENUM_FLAG_OPERATOR_SET(YOUR_TYPE) to enable these operators on YOUR_TYPE.

#define DEFINE_ENUM_FLAG_OPERATOR_SET(ENUMTYPE) \
    inline ENUMTYPE operator | (ENUMTYPE  a, ENUMTYPE b) { return  ENUMTYPE (static_cast<std::underlying_type<ENUMTYPE>::type>(a) | static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE operator & (ENUMTYPE  a, ENUMTYPE b) { return  ENUMTYPE (static_cast<std::underlying_type<ENUMTYPE>::type>(a) & static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE operator ^ (ENUMTYPE  a, ENUMTYPE b) { return  ENUMTYPE (static_cast<std::underlying_type<ENUMTYPE>::type>(a) ^ static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE operator ~ (ENUMTYPE  a            ) { return  ENUMTYPE (~static_cast<std::underlying_type<ENUMTYPE>::type>(a)); } \
    inline ENUMTYPE& operator |= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(reinterpret_cast<std::underlying_type<ENUMTYPE>::type &>(a) |= static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE& operator &= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(reinterpret_cast<std::underlying_type<ENUMTYPE>::type &>(a) &= static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE& operator ^= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(reinterpret_cast<std::underlying_type<ENUMTYPE>::type &>(a) ^= static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \


#define STRUCT_MEMBER_SIZEOF(TYPE, MEMBER) (sizeof(((TYPE*)nullptr)->MEMBER))
