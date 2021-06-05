#include <mutex>       // for std::mutex

#include "common.hpp"
#include "logger.hpp"
#include "fmt.hpp"

namespace postrisc {

std::ofstream log_stream;

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

const char *Logger::GetLevelName(log_level level)
{
    switch (level) {
        case LOG_LEVEL_DEBUG:     return "debug";
        case LOG_LEVEL_INFO:      return "info";
        case LOG_LEVEL_WARNING:   return "warning";
        case LOG_LEVEL_ERROR:     return "error";
        case LOG_LEVEL_CRIT:      return "crit";
        case LOG_LEVEL_NOTICE:    return "notice";
    }
    return "invalid";
}

void Logger::OpenLogFile(const std::string& filename)
{
    log_stream.open(filename, std::ios_base::out | std::ios_base::binary);
}

void Logger::CloseLogFile(void)
{
    log_stream <<
        "<p></p>\n"
        "<hr/>\n"
        "<p class=\"copyright\">" ARCH_NAME " virtual processor emulator</p>\n"
        "<p class=\"copyright\">" ARCH_COPYRIGHT "</p>\n"
        "<p class=\"copyright\">This file was generated automatically.</p>\n"
        "</body>\n"
        "</html>" << std::endl;
}

} // namespace util

void xhtml_header(std::ostream& out, const char *title, const char *description, void (*callback)(std::ostream& out))
{
    out << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
        "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.1//EN\" \"http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd\">\n"
        "<html lang=\"en\" xml:lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">\n"
        "<head>\n";

    if (title)
        out << "<title>" << title << "</title>\n";
    else
        out << "<title>Piped file</title>\n";

    out << "<meta http-equiv=\"content-type\" content=\"application/xhtml+xml; charset=utf-8\"/>\n"
        "<meta http-equiv=\"content-style-type\" content=\"text/css\"/>\n"
        "<meta name=\"language\" content=\"en\"/>\n"
        "<meta name=\"description\" content=\"" << description << "\"/>\n"
        "<meta name=\"viewport\" content=\"width=device-width\"/>\n"
        "<link href=\"styles.css\" rel=\"stylesheet\" type=\"text/css\"/>\n";

    if (callback)
       callback(out);

    out << "</head>\n"
           "<body>\n";

    if (title)
        out << "<h1>" << title << "</h1>\n";
}

} // namespace postrisc
