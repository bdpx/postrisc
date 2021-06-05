#pragma once

namespace postrisc {

#if defined(__x86_64__) // no default operators
std::ostream& operator<<(std::ostream& os, native::f128 f);
std::istream& operator>>(std::istream& os, native::f128& f);
#endif

#if defined(__clang__)
// clang++ doesn't have f16 operator, but g++ has in c++23
ALWAYS_INLINE std::ostream& operator<<(std::ostream& os, native::f16 f) { return os << static_cast<native::f32>(f); }
#endif

#if defined(__x86_64__)
using postrisc::operator>>;
namespace fpu {
    using postrisc::operator>>;
}
#endif

#if defined(__x86_64__) || defined(__clang__)
using postrisc::operator<<;
namespace fpu {
    using postrisc::operator<<;
}
#endif

} // namespace postrisc
