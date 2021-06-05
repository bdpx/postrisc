#pragma once

#include <type_traits>

/***************************************************************************
* this file contain base infrastructure for my C++
* PS:
* I don't really know C, but I can fake it so the compiler thinks I know.
***************************************************************************/

#include "fmt.hpp"
#include "logger.hpp"
#include "fpu/u128.hpp"

namespace postrisc {

template <typename T, size_t N> inline constexpr size_t sizearray( T (&)[N] )  { return N; }

namespace util {

void setup(void);

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
        constexpr default_initializer(T v) : value(v) {}

        operator T () const { return value; }

        default_initializer & operator = (T v) { value = v; return *this; }

    private:
        T value;
};


/***************************************************************************
* one bits mask form size, ones began from zero bit
***************************************************************************/

constexpr inline uint64_t makemask(unsigned int nbits) { return (UINT64_C(1) << nbits) - UINT64_C(1); }

constexpr inline uint32_t makemask32(int nbits) { return (UINT32_C(1) << nbits) - UINT32_C(1); }
constexpr inline uint64_t makemask64(int nbits) { return (UINT64_C(1) << nbits) - UINT64_C(1); }

template<class T> constexpr inline T lobits(T value, int nbits)
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

inline  int32_t sext32( int32_t src, unsigned int bitnum) { return (src << (32-bitnum)) >> (32-bitnum); }
inline uint32_t zext32(uint32_t src, unsigned int bitnum) { return (src << (32-bitnum)) >> (32-bitnum); }
inline  int64_t sext64( int64_t src, unsigned int bitnum) { return (src << (64-bitnum)) >> (64-bitnum); }
inline uint64_t zext64(uint64_t src, unsigned int bitnum) { return (src << (64-bitnum)) >> (64-bitnum); }

/***************************************************************************
* alignment to low numbits-power of 2 by mask least significant bits.
***************************************************************************/

inline uint64_t rounddown(uint64_t val, unsigned int numbits) { return val & ~makemask(numbits); }
inline uint64_t roundup(uint64_t val, unsigned int numbits) { return (val + makemask(numbits)) & ~makemask(numbits); }

template<class T> constexpr inline T align_down(T value, T pow2)
{
    return value & ~(pow2 - 1);
}

template<class T> constexpr inline T align_up(T value, T pow2)
{
    return (value + pow2 - 1) & ~(pow2 - 1);
}

template<class D, class T> constexpr bool aligned(T value)
{
    return (value & static_cast<T>(sizeof(D) - 1)) == 0;
}

STATIC_ASSERT( aligned<uint8_t >(UINT64_C(0x123450)));
STATIC_ASSERT( aligned<uint16_t>(UINT64_C(0x123450)));
STATIC_ASSERT(!aligned<uint16_t>(UINT64_C(0x123451)));
STATIC_ASSERT( aligned<uint32_t>(UINT64_C(0x123450)));
STATIC_ASSERT(!aligned<uint32_t>(UINT64_C(0x123452)));
STATIC_ASSERT( aligned<uint64_t>(UINT64_C(0x123450)));
STATIC_ASSERT(!aligned<uint64_t>(UINT64_C(0x123454)));

inline void * align_down(void * ptr, size_t pow2)
{
    size_t p = reinterpret_cast<size_t>(ptr);
    p = align_down(p, pow2);
    return reinterpret_cast<void *>(p);
}

template<class T> constexpr inline bool is_pow2(T value)
{
    return (value & (value - 1)) == 0;
}

STATIC_ASSERT(is_pow2<uint8_t>(0x01));
STATIC_ASSERT(is_pow2<uint8_t>(0x02));
STATIC_ASSERT(is_pow2<uint8_t>(0x04));
STATIC_ASSERT(is_pow2<uint8_t>(0x08));
STATIC_ASSERT(is_pow2<uint8_t>(0x10));
STATIC_ASSERT(is_pow2<uint8_t>(0x20));
STATIC_ASSERT(is_pow2<uint8_t>(0x40));
STATIC_ASSERT(is_pow2<uint8_t>(0x80));

STATIC_ASSERT(!is_pow2<uint8_t>(0x03));
STATIC_ASSERT(!is_pow2<uint8_t>(0x05));
STATIC_ASSERT(!is_pow2<uint8_t>(0x30));
STATIC_ASSERT(!is_pow2<uint8_t>(0x71));

/////////////////////////////////////////////////////////
//
class population {
private:
    template<class T>
    static inline T helper_step(T sum, T mask, unsigned shift)
    {
        return (sum & mask) + ((sum >> shift) & mask);
    }

public:
    static uint64_t func(uint64_t sum)
    {
        // ZZZZ(mask, shift) sum = (sum & mask) + ((sum >> (shift)) & mask)
        sum = helper_step(sum, UINT64_C( 0x5555555555555555 ),  1 );
        sum = helper_step(sum, UINT64_C( 0x3333333333333333 ),  2 );
        sum = helper_step(sum, UINT64_C( 0x0f0f0f0f0f0f0f0f ),  4 );
        sum = helper_step(sum, UINT64_C( 0x00ff00ff00ff00ff ),  8 );
        sum = helper_step(sum, UINT64_C( 0x0000ffff0000ffff ), 16 );
        sum = helper_step(sum, UINT64_C( 0x00000000ffffffff ), 32 );

        return sum;
    }

    static uint32_t func(uint32_t sum)
    {
        // ZZZZ(mask, shift) sum = (sum & mask) + ((sum >> (shift)) & mask)
        sum = helper_step(sum, UINT32_C( 0x55555555 ),  1 );
        sum = helper_step(sum, UINT32_C( 0x33333333 ),  2 );
        sum = helper_step(sum, UINT32_C( 0x0f0f0f0f ),  4 );
        sum = helper_step(sum, UINT32_C( 0x00ff00ff ),  8 );
        sum = helper_step(sum, UINT32_C( 0x0000ffff ), 16 );

        return sum;
    }
};

class count_leading_zeros {
public:
    static unsigned func(uint64_t z)
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
    static unsigned func(uint64_t z)
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
    static unsigned func(uint64_t z)
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
    template<class T>
    static inline T helper_step(T sum, T mask, unsigned shift)
    {
        return ((sum & mask) << shift) | ((sum & ~mask) >> shift);
    }

public:
    static uint64_t func(uint64_t sum, unsigned shift_mask)
    {
        // ZZZZ(mask, shift) sum = ((sum & mask) << (shift)) | ((sum & ~mask) >> (shift))
        sum = helper_step(sum, UINT64_C( 0x5555555555555555 ), shift_mask &  1 );
        sum = helper_step(sum, UINT64_C( 0x3333333333333333 ), shift_mask &  2 );
        sum = helper_step(sum, UINT64_C( 0x0f0f0f0f0f0f0f0f ), shift_mask &  4 );
        sum = helper_step(sum, UINT64_C( 0x00ff00ff00ff00ff ), shift_mask &  8 );
        sum = helper_step(sum, UINT64_C( 0x0000ffff0000ffff ), shift_mask & 16 );
        sum = helper_step(sum, UINT64_C( 0x00000000ffffffff ), shift_mask & 32 );

        return sum;
    }

    static uint32_t func(uint32_t sum, unsigned shift_mask)
    {
        // ZZZZ(mask, shift) sum = ((sum & mask) << (shift)) | ((sum & ~mask) >> (shift))
        sum = helper_step(sum, UINT32_C( 0x55555555 ), shift_mask &  1 );
        sum = helper_step(sum, UINT32_C( 0x33333333 ), shift_mask &  2 );
        sum = helper_step(sum, UINT32_C( 0x0f0f0f0f ), shift_mask &  4 );
        sum = helper_step(sum, UINT32_C( 0x00ff00ff ), shift_mask &  8 );
        sum = helper_step(sum, UINT32_C( 0x0000ffff ), shift_mask & 16 );

        return sum;
    }
};

/////////////////////////////////////////////////////////
// gray codes
inline uint64_t gray2bin(uint64_t gray)
{
    gray ^= (gray >> 32);
    gray ^= (gray >> 16);
    gray ^= (gray >>  8);
    gray ^= (gray >>  4);
    gray ^= (gray >>  2);
    gray ^= (gray >>  1);
    return gray;
}

inline uint64_t bin2gray(uint64_t num)
{
    return (num >> 1) ^ num;
}

/****************************************************************************
*
****************************************************************************/
#if GCC_COMPILER
#pragma GCC push_options
#pragma GCC optimize("no-strict-aliasing")
#endif

template<class FROM, class TO> TO change_bit_presentation(FROM value)
{
#if defined(__unix__) || defined(__GNUC__)
    static_assert(sizeof(FROM) == sizeof(TO), "can't represent both types");
#endif

    return reinterpret_cast<TO&>(value);
}

#if GCC_COMPILER
#pragma GCC pop_options
#endif

} // namespace util

template<class T>
constexpr inline T
AddCarryOutVector(T op1, T op2)
{
    return (op1 & op2) | ((op1 | op2) & (~(op1 + op2)));
}

template<class T>
constexpr inline T
SubCarryOutVector(T op1, T op2)
{
    return (~op1 & op2) | ((~(op1 ^ op2)) & (op1 - op2));
}

} // namespace postrisc

// Define operator overloads
// to enable bit operations on enum values that are used to define flags.
// Use DEFINE_ENUM_FLAG_OPERATOR_SET(YOUR_TYPE) to enable these operators on YOUR_TYPE.

#define DEFINE_ENUM_FLAG_OPERATOR_SET(ENUMTYPE) \
    inline ENUMTYPE  operator |  (ENUMTYPE  a, ENUMTYPE b) { return  ENUMTYPE   (static_cast<std::underlying_type<ENUMTYPE>::type>(a) | static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE  operator &  (ENUMTYPE  a, ENUMTYPE b) { return  ENUMTYPE   (static_cast<std::underlying_type<ENUMTYPE>::type>(a) & static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE  operator ^  (ENUMTYPE  a, ENUMTYPE b) { return  ENUMTYPE   (static_cast<std::underlying_type<ENUMTYPE>::type>(a) ^ static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE  operator ~  (ENUMTYPE  a            ) { return  ENUMTYPE   (~static_cast<std::underlying_type<ENUMTYPE>::type>(a)); } \
    inline ENUMTYPE &operator |= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(reinterpret_cast<std::underlying_type<ENUMTYPE>::type &>(a) |= static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE &operator &= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(reinterpret_cast<std::underlying_type<ENUMTYPE>::type &>(a) &= static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \
    inline ENUMTYPE &operator ^= (ENUMTYPE &a, ENUMTYPE b) { return (ENUMTYPE &)(reinterpret_cast<std::underlying_type<ENUMTYPE>::type &>(a) ^= static_cast<std::underlying_type<ENUMTYPE>::type>(b)); } \


#define STRUCT_MEMBER_SIZEOF(TYPE, MEMBER) (sizeof(((TYPE*)nullptr)->MEMBER))
