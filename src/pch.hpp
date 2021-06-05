#pragma once

#include <cstdint>     // for uintxx_t/UINTXX_C
#include <cassert>     // for assert
#include <cstring>     // for memcpy, memset, strcmp, strlen, etc
#include <cctype>      // for isalpha, isalnum, etc

#include <limits>      // for std::numeric_limits
#include <vector>      // for std::vector
#include <array>       // for std::array
#include <bitset>      // for std::bitset
#include <iostream>    // for std::cout/cin/cerr
#include <sstream>     // for std::ostringstream
#include <fstream>     // for std::oftream
#include <iomanip>     // for std::endl/hex/width/etc
#include <tuple>       // for std::tuple
#include <span>        // for std::span
#include <bit>         // for std::bit_cast
#include <type_traits> // for is_save_v
#include <algorithm>   // for std::sort

#include <boost/serialization/nvp.hpp>

#if defined(__unix__)
    int _getch(void);
    int _getche(void);
    int _kbhit(void);
#else
    #include <conio.h>
#endif

/***************************************************************************
* undo min/max macro
***************************************************************************/
#if defined(min)
    #undef min
#endif

#if defined(max)
    #undef max
#endif

/***************************************************************************
* set lost macro for standard stdin/stdout/stderr
***************************************************************************/
#if !defined(STDIN_FILENO)
    #define STDIN_FILENO 0
#endif

#if !defined(STDOUT_FILENO)
    #define STDOUT_FILENO 1
#endif

#if !defined(STDERR_FILENO)
    #define STDERR_FILENO 2
#endif

/***************************************************************************
* undo UNREFERENCED_PARAMETER macro, make our const-friendly
***************************************************************************/
#if defined(UNREFERENCED_PARAMETER)
#undef UNREFERENCED_PARAMETER
#endif

#define UNREFERENCED_PARAMETER(param) (void)(param)

/***************************************************************************
* OS/compiler specific
***************************************************************************/

// define OS
#if defined(WIN64) || defined(_WIN64) || defined(__WIN64__)
#  define WINDOWS
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#  define WINDOWS
#elif defined(__GNUC__) && !defined(__MINGW32__)
#  define LINUX
#else
#  error unknown OS
#endif

// check C++ version
#if defined(__unix__) || defined(__GNUC__)
#if (__cplusplus < 201103)
#  error C++ 2011 required
#endif
#endif

// define compiler
#if defined(__GNUC__) || defined(__clang__)
#  define ALWAYS_INLINE      inline __attribute__((always_inline))
#  define NORETURN           __attribute__((noreturn))
#  define ALIGNED(nbytes)    __attribute__((aligned(nbytes)))
#  define UNUSED(NAME)       UNUSED_##NAME __attribute__((__unused__))
#else
#  define ALWAYS_INLINE      inline __forceinline //MSVC
#  define NORETURN           __declspec(noreturn)
#  define ALIGNED(nbytes)    __declspec(align(nbytes))
#  define UNUSED(NAME)       [[maybe_unused]] UNUSED_##NAME
#endif

/* Non-GNU don't know about 'function-attributes' extension */
#if !defined(__GNUC__)
#  define __attribute__(p) /* nothing */
#endif

/***************************************************************************
* quadruple support defines
***************************************************************************/
namespace postrisc {

using i8   =      int8_t;
using u8   =     uint8_t;
using i16  =     int16_t;
using u16  =    uint16_t;
using i32  =     int32_t;
using u32  =    uint32_t;
using i64  =     int64_t;
using u64  =    uint64_t;

#if !defined(__SIZEOF_INT128__)
#error "__SIZEOF_INT128__ not defined, can't use i128/u128"
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
using i128 =  __int128_t;
using u128 = __uint128_t;
#pragma GCC diagnostic pop

#define  I8C(x)  INT8_C(x)
#define I16C(x) INT16_C(x)
#define I32C(x) INT32_C(x)
#define I64C(x) INT64_C(x)

#define  U8C(x)  UINT8_C(x)
#define U16C(x) UINT16_C(x)
#define U32C(x) UINT32_C(x)
#define U64C(x) UINT64_C(x)
#define U128C(hi, lo) (((u128)(U64C(hi))) << 64 | (U64C(lo)))

std::ostream& operator<< (std::ostream& os, i128 v);
std::ostream& operator<< (std::ostream& os, u128 v);

namespace native {
    using f16  = _Float16;
    using f32  = float;
    using f64  = double;
    using f128 = __float128;

    static_assert(sizeof(f16) == 2U);
    static_assert(sizeof(f32) == 4U);
    static_assert(sizeof(f64) == 8U);
    static_assert(sizeof(f128) == 16U);

    using f80  = long double;
}

#define LFLT_C(imm)               imm##q

#if !defined(likely)
    #define likely(x)      __builtin_expect(!!(x), 1) 
#endif

#if !defined(unlikely)
    #define unlikely(x)    __builtin_expect(!!(x), 0)
#endif

} // namespace postrisc
