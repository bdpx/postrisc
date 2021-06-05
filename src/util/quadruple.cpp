#include "util/common.hpp"
#include "mpreal.hpp"

namespace postrisc {

std::ostream& operator<< (std::ostream& os, quadruple f)
{
    mpfr::mpreal xx(f, 113+113);
    auto const precision = os.precision();

    os << std::scientific
       << std::showpos
       << std::setprecision(34)
       << xx
       << std::fixed
       << std::noshowpos
       << std::setprecision(precision);
    return os;
}

std::istream& operator>> (std::istream& is, quadruple& f)
{
    mpfr::mpreal xx(0, 113+113);
    is >> xx;
    f = (quadruple)xx;
    return is;
}

} // namespace postrisc
