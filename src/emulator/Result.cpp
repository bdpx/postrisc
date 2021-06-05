#if defined(__unix__)
//#define BOOST_STACKTRACE_USE_ADDR2LINE
#else
//#define BOOST_STACKTRACE_USE_WINDBG
#endif

#include <boost/stacktrace.hpp>
//g++ -fno-pie -ggdb3 -O0 -no-pie -o main.out -std=c++11 -Wall -Wextra -pedantic-errors main.cpp -ldl

#include "Result.hpp"
#include "util/common.hpp"

namespace postrisc {

char const *Result::GetInterruptionName(void) const
{
    switch (m_Code) {
        case taken_branch:       return "taken_branch";
        case continue_execution: return "continue_execution";
#define NAME_X(name) case name: return #name;
    DECLARE_INTERRUPTION_VECTORS(NAME_X)
#undef NAME_X
        case last: break;
    }
    return "unknown interruption";
}

std::ostream& operator<<(std::ostream& out, const CInterruptionCauseRegister& cause)
{
    return out
        << fmt::hex(cause.imm)
        << " x=" << cause.u.x
        << " w=" << cause.u.w
        << " r=" << cause.u.r
        << " a=" << cause.u.a
        << " n=" << cause.u.n
        << " d=" << cause.u.d
        << " ei=" << cause.u.ei
        << " code=" << cause.u.code
        << " vector=" << cause.u.vector;
}

} // namespace postrisc
