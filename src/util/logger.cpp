#include <fstream>
#include <mutex>       // for std::mutex

#include "logger.hpp"
#include "fmt.hpp"

namespace postrisc {

static std::ofstream log_stream;

std::ostream& dbgs(void)
{
    // return log_stream.is_open() ? log_stream : std::cerr;
    return log_stream;
}

namespace util {

cout_streamer::cout_streamer()
{
}

cout_streamer::~cout_streamer()
{
}

Logger::Logger(const char *file, int line, const char *function)
{
    stream() << file
             << '/' << function
             << ':' << line << "> ";
}

#if defined(POSTRISC_RUNTIME_LOGS)
u32 Logger::s_log_level = LOG_LEVEL_ERROR;
u32 Logger::s_subsystem_mask = 0;
#endif

const char *
Logger::GetLevelName(log_level level)
{
    switch (level) {
        case LOG_LEVEL_DEBUG:     return "debug";
        case LOG_LEVEL_INFO:      return "info";
        case LOG_LEVEL_WARNING:   return "warning";
        case LOG_LEVEL_ERROR:     return "error";
        case LOG_LEVEL_CRIT:      return "crit";
        default:
            return "invalid";
    }
}

void Logger::OpenLogFile(const std::string& filename)
{
    log_stream.open(filename, std::ios_base::out);
}


} // namespace util
} // namespace postrisc
