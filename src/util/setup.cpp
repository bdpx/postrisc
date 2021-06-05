#include <cstdlib>     // for exit
#include <csignal>
#include <iostream>
#include <fstream>

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

void terminate(bool is_abort)
{
#if defined(__unix__) && defined(__clang__) && defined(ASAN)
    __sanitizer_print_stack_trace();
#else
    boost::stacktrace::stacktrace st;
    std::cerr << "stack trace: " << st.size() << " frames" << std::endl << st << std::endl;
#endif
    if (is_abort) {
        abort();  // forces abnormal termination
    } else {
        exit(EXIT_FAILURE);
    }
}

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
    dbgs() << "signal handler called: " << sig << std::endl;
    util::terminate(false);
}

static void terminate_handler(void)
{
    dbgs() << "terminate handler called" << std::endl;
    util::terminate(true);
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

bool load_file(const std::string& filepath, std::vector<std::byte>& data)
{
    std::ifstream fs;
    fs.open(filepath, std::ios_base::in | std::ios_base::binary);
    if (!fs.good()) {
        std::cerr << "can't open file " << filepath << std::endl;
        return false;
    }

    fs.seekg(0, std::ios_base::end);
    auto const length = fs.tellg();
    fs.seekg(0, std::ios_base::beg);

    data.clear();
    data.resize(length);
    fs.read(reinterpret_cast<char*>(data.data()), length);
    return true;
}

} // namespace util
} // namespace postrisc
