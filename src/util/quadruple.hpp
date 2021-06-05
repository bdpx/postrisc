#pragma once

#if USE_QUADMATH
#include <quadmath.h>
#endif

namespace postrisc {

#if USE_QUADMATH
inline bool iszero(native::f128 f) { return f == 0.0; }
//native::f128 mod(native::f128 a, native::f128 b) { return fmodq(a, b); }
//native::f128 rem(native::f128 a, native::f128 b) { return fmodq(a, b); }
#endif

std::ostream& operator<< (std::ostream& os, native::f128 f);
std::istream& operator>> (std::istream& os, native::f128& f);

ALWAYS_INLINE std::ostream& operator<< (std::ostream& os, native::f16 f) { return os << (native::f32)f; }

} // namespace postrisc
