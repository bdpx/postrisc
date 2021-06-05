#pragma once

#include <cstdint>     // for uintxx_t/UINTXX_C
#include <cassert>     // for asserts
#include <limits>      // for numeric_limits
#include <vector>      // for std::vector
#include <array>       // for std::array
#include <bitset>      // for std::bitset
#include <iostream>    // for std::cout/cin/cerr
#include <sstream>     // for ostringstream
#include <iomanip>     // for std::endl/hex/width/etc
#include <cstring>     // for memcpy, memset, strcmp, strlen, etc
#include <cstdarg>     // for va_start/va_end
#include <cctype>      // for isalpha, isalnum, etc

#include <boost/serialization/nvp.hpp>

#if defined(__GNUC__) && !defined(__clang__)
    #define GCC_COMPILER 1
#else
    #define GCC_COMPILER 0
#endif

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
#if defined(__GNUC__)
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

#define STATIC_ASSERT(cond) static_assert(cond, "static_assert failed: " #cond)

/***************************************************************************
* quadruple support defines
***************************************************************************/
namespace postrisc {

typedef long double extended;
#if defined(__unix__) || defined(__GNUC__)
    typedef __float128 quadruple;
    #define LFLT_C(imm)               imm##q
#else
    // fallback to double
    typedef double quadruple;
    #define LFLT_C(imm)               imm
#endif

#if !defined(likely)
    #define likely(x)      __builtin_expect(!!(x), 1) 
#endif

#if !defined(unlikely)
    #define unlikely(x)    __builtin_expect(!!(x), 0)
#endif

STATIC_ASSERT(sizeof(quadruple) == 16U);

} // namespace postrisc
