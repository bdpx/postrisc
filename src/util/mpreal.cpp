
#include "mpreal.hpp"

#if USE_QUADMATH == 0

namespace mpfr {

std::ostream& mpreal::output(std::ostream& os) const
{
    std::ostringstream format;
    const std::ios::fmtflags flags = os.flags();

    format << ((flags & std::ios::showpos) ? "%+" : "%");
    if (os.precision() >= 0)
        format << '.' << os.precision() << "R*"
               << ((flags & std::ios::floatfield) == std::ios::fixed ? 'f' :
                   (flags & std::ios::floatfield) == std::ios::scientific ? 'e' :
                   'g');
    else
        format << "R*e";

    char *s = NULL;
    if (!(mpfr_asprintf(&s, format.str().c_str(), mpfr::mpreal::get_default_rnd(), mpfr_srcptr()) < 0)) {
        os << std::string(s);
        mpfr_free_str(s);
    }
    return os;
}

} // namespace mpfr

#endif