#include "util/common.hpp"

#if defined(WINDOWS) // no strfromf128/strtof128
#include <quadmath.h>
#endif

namespace postrisc {

std::ostream& operator<<(std::ostream& out, i128 v)
{
    if (v < 0) {
        out << '-' << -(u128)v;
    } else {
        out << (u128)v;
    }
    return out;
}

std::ostream& operator<<(std::ostream& out, u128 v)
{
    char temp[40];
    int pos = 0;
    do {
        temp[pos++] = (v % 10) + '0';
        v /= 10;
    } while(v);

    for (int i = pos - 1; i >= 0; i--) {
        out << temp[i];
    }
    return out;
}

#if defined(__x86_64__)

std::ostream& operator<< (std::ostream& os, native::f128 f)
{
    char buf[128] = "";

#if defined(WINDOWS) // quadmath
    quadmath_snprintf(buf, sizeof(buf), "%.34Qe", f);
#else
    strfromf128(buf, sizeof(buf), "%.34e", f);
#endif

    os << buf;
    return os;
}

std::istream& operator>>(std::istream& is, native::f128& f)
{
    char s[128];
    is >> s;

#if defined(WINDOWS) // quadmath
    f = strtoflt128(s, NULL);
#else
    f = strtof128(s, NULL);
#endif

    return is;
}

#endif // x86

} // namespace postrisc
