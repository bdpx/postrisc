#pragma once

#if USE_QUADMATH
#include <quadmath.h>
#endif

namespace postrisc {

#if USE_QUADMATH
inline bool iszero(quadruple f) { return f == 0.0; }
//quadruple mod(quadruple a, quadruple b) { return fmodq(a, b); }
//quadruple rem(quadruple a, quadruple b) { return fmodq(a, b); }
#endif

std::ostream& operator<< (std::ostream& os, quadruple f);
std::istream& operator>> (std::istream& os, quadruple& f);

} // namespace postrisc
