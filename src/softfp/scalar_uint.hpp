#pragma once

#include <ostream>
#include <istream>
#include <type_traits>

#define DEFINE_BINARY_OP_TABLE(X) X(+) X(-) X(*) X(/) X(%) X(<<) X(>>)
#define DEFINE_COMPARE_OP_TABLE(X) X(<) X(>) X(<=) X(>=) X(!=) X(==)

namespace postrisc {

// use 'scalar' a sname of input scalar param in template
#define TMPLT_CONVERTIBLE                                                   \
    template <typename scalar,                                              \
        typename = std::enable_if<std::is_convertible<scalar, T>::value>    \
        >

template <typename T,
    typename = typename std::enable_if<std::is_unsigned<T>::value>
    >
class scalar_uint
{
public:
    using word_type = T;

    constexpr scalar_uint() noexcept = default;

    TMPLT_CONVERTIBLE constexpr scalar_uint(scalar x) noexcept : value{static_cast<scalar>(x)} {}

    constexpr explicit operator bool() const noexcept { return value != 0; }

    // Explicit converting operator for all builtin integral types.
    template <typename integer,
        typename = typename std::enable_if<std::is_integral<integer>::value> >
    constexpr explicit operator integer() const noexcept
    {
        return static_cast<integer>(value);
    }

#define DECLARE_COMPARE(OPERATOR) \
    TMPLT_CONVERTIBLE friend bool operator OPERATOR (scalar_uint x, const scalar y) noexcept { return x.value OPERATOR y; } \
    TMPLT_CONVERTIBLE friend bool operator OPERATOR (const scalar x, scalar_uint y) noexcept { return x OPERATOR y.value; } \
    friend constexpr bool operator OPERATOR (scalar_uint x, scalar_uint y) noexcept { return x.value OPERATOR y.value; }

    DEFINE_COMPARE_OP_TABLE(DECLARE_COMPARE)

#define DECLARE_BINARY(OPERATOR) \
    TMPLT_CONVERTIBLE friend constexpr scalar_uint operator OPERATOR (scalar_uint x, const scalar y) noexcept { return x.value OPERATOR y; } \
    TMPLT_CONVERTIBLE friend constexpr scalar_uint& operator OPERATOR##= (scalar_uint& x, const scalar y) noexcept { return x = x OPERATOR y; } \
    TMPLT_CONVERTIBLE friend constexpr scalar_uint operator OPERATOR (scalar x, scalar_uint y) noexcept { return x OPERATOR y.value; } \
    TMPLT_CONVERTIBLE friend constexpr scalar& operator OPERATOR##= (scalar& x, scalar_uint y) noexcept { return x = x OPERATOR y.value; } \
    friend constexpr scalar_uint operator OPERATOR (scalar_uint x, scalar_uint y) noexcept { return x.value OPERATOR y.value; } \
    friend constexpr scalar_uint& operator OPERATOR##= (scalar_uint& x, scalar_uint y) noexcept { return x = x OPERATOR y; }

    DEFINE_BINARY_OP_TABLE(DECLARE_BINARY)

    friend std::ostream& operator<< (std::ostream& os, const scalar_uint r)
    {
        return os << r.value;
    }
    friend std::istream& operator>> (std::istream& is, const scalar_uint& r)
    {
        return is >> r.value;
    }
   
private:
    T value;
};

#undef TMPLT_CONVERTIBLE
#undef DEFINE_BINARY_OP_TABLE
#undef DEFINE_COMPARE_OP_TABLE

static_assert(sizeof(scalar_uint< u8>) == sizeof( u8));
static_assert(sizeof(scalar_uint<u16>) == sizeof(u16));
static_assert(sizeof(scalar_uint<u32>) == sizeof(u32));
static_assert(sizeof(scalar_uint<u64>) == sizeof(u64));

} // namespace postrisc
