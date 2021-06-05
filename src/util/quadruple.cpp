#include "util/common.hpp"

#if USE_QUADMATH
#include <quadmath.h>
#else
#include "mpreal.hpp"
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

std::ostream& operator<< (std::ostream& os, native::f128 f)
{
#if USE_QUADMATH
    char buf[128];
    quadmath_snprintf(buf, sizeof(buf), "%+.34Qe", f);
    os << buf;
#else
    mpfr::mpreal xx(f, 113+113);
    auto const precision = os.precision();

    os << std::scientific
       << std::showpos
       << std::setprecision(34)
       << xx
       << std::fixed
       << std::noshowpos
       << std::setprecision(precision);
#endif
    return os;
}

std::istream& operator>>(std::istream& is, native::f128& f)
{
#if USE_QUADMATH
    char s[128];
    is >> s;
    f = strtoflt128(s, NULL);
#else
    mpfr::mpreal xx(0, 113+113);
    is >> xx;
    f = (native::f128)xx;
#endif
    return is;
}

} // namespace postrisc
