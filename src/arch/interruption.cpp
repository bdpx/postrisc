#if defined(__unix__)
//#define BOOST_STACKTRACE_USE_ADDR2LINE
#else
//#define BOOST_STACKTRACE_USE_WINDBG
#endif

#include <boost/stacktrace.hpp>
//g++ -fno-pie -ggdb3 -O0 -no-pie -o main.out -std=c++11 -Wall -Wextra -pedantic-errors main.cpp -ldl

#include "util/common.hpp"
#include "interruption.hpp"

namespace postrisc {

static const char interruption_name[1<< bits_per_vector_num][48] =
{
#define NAME_X(name) #name,
    DECLARE_INTERRUPTION_VECTORS(NAME_X)
};

char const * CStatus::GetInterruptionName(void) const
{
    unsigned iv = m_Code;
    iv = iv & util::makemask(bits_per_vector_num);
    return interruption_name[iv];
}

std::ostream &
operator << (std::ostream& out, const CInterruptionCauseRegister & cause)
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
