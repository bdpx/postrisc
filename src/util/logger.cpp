#include <mutex>       // for std::mutex

#include "logger.hpp"

namespace postrisc {
namespace util {

static std::mutex cout_mutex;
static std::mutex cerr_mutex;
static std::mutex cin_mutex;

cout_streamer::cout_streamer()
{
    cout_mutex.lock();
}

cout_streamer::~cout_streamer()
{
    cout_mutex.unlock();
}

Logger::Logger(log_level level, const char *file, int line, const char *function)
{
    cerr_mutex.lock();

    stream() << GetLevelName(level)
             << ' ' << file
             << '/' << function
             << ':' << line << "> ";
}

Logger::~Logger()
{
    // stream() << "Logger dtor" << std::endl;
    cerr_mutex.unlock();
}

const char *
Logger::GetLevelName(log_level level)
{
    switch (level) {
        case LOG_LEVEL_VERBOSE:   return "verbose";
        case LOG_LEVEL_TRACE:     return "trace";
        case LOG_LEVEL_DEBUG:     return "debug";
        case LOG_LEVEL_INFO:      return "info";
        case LOG_LEVEL_WARNING:   return "warning";
        case LOG_LEVEL_ERROR:     return "error";
        case LOG_LEVEL_FATAL:     return "fatal";
        case LOG_LEVEL_ABORT:     return "abort";
        case LOG_LEVEL_HIGHEST:   return "highest";
        default:
            return "invalid";
    }
}

} // namespace util
} // namespace postrisc

#if 0

namespace util
{

Trace::Trace(char *func_name, const char* argsfmt, ...)
{
    char fmt[256] ={0};
    sprintf(fmt, "%s%s", func_name, argsfmt);
    va_list arglist;
    va_start(arglist, argsfmt);
    LogMsg(depth_, depth_ * 2, fmt,  arglist);
    va_end(arglist);
    ++depth_;
}

Trace::~Trace()
{
    --depth_;
}

void Trace::LogToFile(const char *trace_file)
{
    trace_file_ = trace_file;
}

void
Trace::LogMsg(int depth, int align, const char *fmt, va_list args)
{
    FILE    *fp = fopen(trace_file_.c_str(), "a+");
    if (fp == NULL)
    {
        return;
    }

    time_t curTime;
    time(&curTime);

    char    timeStamp[32] = { 0 };
    strftime(timeStamp, sizeof(timeStamp), "%Y%m%d.%H%M%S", localtime(&curTime));

    // only log the timestamp when the time changes
    unsigned int len = fprintf( fp, "%s %*.*s> (%d)",
              (last_invoke_time_ != curTime) ? 
                timeStamp : "               ",
                    2 * depth,
                    2 * depth,
                    nest_,
                    depth);
            last_invoke_time_ = curTime;
            len += vfprintf(fp, fmt, args);
            len += fwrite("\n", 1, 1, fp);

    fflush(fp);
    fclose(fp);
}

std::string Trace::trace_file_  = "";
int         Trace::depth_       = 0;

// arbitrarily support nesting 34 deep for no particular reason
const char Trace::nest_[70] = 
    "| | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | | ";

time_t      Trace::last_invoke_time_ = 0;

} // namespace util


void foo()
{
	CM_TRACE_FUNC("foo");
}

void bar(int x)
{
	CM_TRACE_FUNC_ARG1("bar", "(%d)", x);
	foo();
}

void foobar(int x, const char* s)
{
    CM_TRACE_FUNC_ARG2("foobar", "(%d, %s)", x, s);
    bar(789);
}

void foobar3(int x, int y, double z)
{
    CM_TRACE_FUNC_ARG3("foobar3", "(%d, %d, %f)", x, y, z);
    foobar(123, "4546");
}

int main()
{
    CM_TRACE_FILE("./trace.log");
    CM_TRACE_FUNC("main");
    foo();
    bar(23);
    foobar(33, "char");
    foobar3(12, 23, 34.45);
    return 0;
}

#endif
