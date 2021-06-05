#include <cstdlib>     // for exit
#include <csignal>
#include <iostream>

#if !defined(__unix__)
#include <io.h>        // for _setmode
#include <fcntl.h>     // for _O_BINARY
#endif

#if defined(__unix__) && defined(__clang__) && defined(ASAN)
#include <sanitizer/common_interface_defs.h>
#else
//#define BOOST_STACKTRACE_USE_BACKTRACE
#include <boost/stacktrace.hpp>
#endif

#include "common.hpp"

namespace postrisc {
namespace util {

#if !defined(__unix__)
static void SetBinaryMode(int fd)
{
    const int previous_mode = _setmode(fd, _O_BINARY);
    if (previous_mode == -1) {
        LOG_ABORT << "binary setmode fault for fd=" << fd;
    }
}
#endif

static void signal_handler(int sig)
{
    std::cerr << "signal handler called: " << sig << std::endl;
#if defined(__unix__) && defined(__clang__) && defined(ASAN)
    __sanitizer_print_stack_trace();
#else
    boost::stacktrace::stacktrace st;
    std::cerr << "stack trace: " << st.size() << " frames" << std::endl
              << st << std::endl;
#endif
    exit(EXIT_FAILURE);
}

static void terminate_handler(void)
{
    std::cerr << "terminate handler called" << std::endl;
#if defined(__unix__) && defined(__clang__) && defined(ASAN)
    __sanitizer_print_stack_trace();
#else
    boost::stacktrace::stacktrace st;
    std::cerr << "stack trace: " << st.size() << " frames" << std::endl
              << st << std::endl;
#endif
    abort();  // forces abnormal termination
}

void setup(void)
{
#if !defined(__unix__)
    SetBinaryMode(STDOUT_FILENO);
    SetBinaryMode( STDIN_FILENO);
    SetBinaryMode(STDERR_FILENO);
#endif

    std::ios_base::sync_with_stdio(false);
    std::cout.sync_with_stdio(false);
    std::cin.tie(nullptr);

    // install our handler
    signal(SIGSEGV, signal_handler);
    signal(SIGFPE,  signal_handler);
#if defined(__unix__)
    signal(SIGBUS,  signal_handler);
    signal(SIGSYS,  signal_handler);
#endif

    std::set_terminate(terminate_handler);
}

} // namespace util
} // namespace postrisc
