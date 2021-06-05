#pragma once

#include <stdexcept>  // for runtime_error

// globally visible
namespace postrisc {

void xhtml_header(std::ostream& out, const char *title, const char *description, void (*callback)(std::ostream& out) = nullptr);

enum log_level {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_NOTICE,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_CRIT,
    LOG_LEVEL_CURRENT = LOG_LEVEL_ERROR
};

// --log_level 1
// --log_subsystem 0xffffffff

enum log_subsystem : u32 {
#define X(NAME,VALUE) NAME = VALUE,
#include "logger_subsystems.h"
#undef X

#if defined(POSTRISC_RUNTIME_LOGS)
// no default mask
#else
    LOG_CURRENT_MASK = 0
#define X(NAME,VALUE) | NAME
#include "logger_subsystems.h"
#undef X
#endif
};

constexpr inline log_subsystem operator| (log_subsystem a, log_subsystem b)
{
    return static_cast<log_subsystem>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

constexpr inline log_subsystem operator& (log_subsystem a, log_subsystem b)
{
    return static_cast<log_subsystem>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
}

extern std::ofstream log_stream;

inline std::ostream& dbgs(void)
{
    // return log_stream.is_open() ? log_stream : std::cerr;
    return log_stream;
}


namespace util {

class cout_streamer {
public:
    cout_streamer();
    ~cout_streamer();
    std::ostream& stream() const {
        return std::cout;
    }
};

class Logger {
public:
    Logger(const char *file, int line, const char *function);

    std::ostream& stream() const {
        return dbgs();
    }

    static void OpenLogFile(const std::string& filename);
    static void CloseLogFile(void);

#if defined(POSTRISC_RUNTIME_LOGS)
    static void set_level(u32 level) { s_log_level = level; }
    static void set_subsystem_mask(u32 mask) { s_subsystem_mask = mask; }

    static bool enabled(enum log_level level, enum log_subsystem subsystem_flags)
    {
        return (level >= s_log_level) && (subsystem_flags & s_subsystem_mask) != 0;
    }
#else
    static constexpr bool enabled(enum log_level level, enum log_subsystem subsystem_flags)
    {
        return (level >= LOG_LEVEL_CURRENT) && (subsystem_flags & LOG_CURRENT_MASK) != 0;
    }
#endif

private:
    static const char * GetLevelName(log_level level);

private:
#if defined(POSTRISC_RUNTIME_LOGS)
    static u32 s_log_level;
    static u32 s_subsystem_mask;
#endif
};

class LogFinalizer {
public:
    LogFinalizer() {}
    void operator+=(std::ostream& stream) const {
        stream << std::endl;
    }
};

/****************************************************************************
*  fatal error messaging
****************************************************************************/
class LogAbort {
public:
    LogAbort() {}

    void operator+=(std::ostream& stream) const {
        stream << std::endl;
        throw std::runtime_error("common error");
    }
};

} // namespace util

// assumed operator precedence: +=, <<
// see root CMakeList for __FILENAME__ definition
#define WITH_LOG_IMPL(LEVEL, FLAG) \
   if (postrisc::util::Logger::enabled(postrisc::LOG_LEVEL_##LEVEL, postrisc::FLAG))

#define LOG_IMPL(LEVEL, FLAG) \
   if (!postrisc::util::Logger::enabled(postrisc::LOG_LEVEL_##LEVEL, postrisc::FLAG)) {} \
   else postrisc::util::LogFinalizer() += \
        postrisc::util::Logger(__FILENAME__, __LINE__, __FUNCTION__).stream()


#define LOG_DEBUG(flags)   LOG_IMPL(DEBUG,   flags)
#define LOG_INFO(flags)    LOG_IMPL(INFO,    flags)
#define LOG_WARNING(flags) LOG_IMPL(WARNING, flags)
#define LOG_ERROR(flags)   LOG_IMPL(ERROR,   flags)

#define WITH_DEBUG(flags)   WITH_LOG_IMPL(DEBUG,   flags)
#define WITH_INFO(flags)    WITH_LOG_IMPL(INFO,    flags)
#define WITH_WARNING(flags) WITH_LOG_IMPL(WARNING, flags)
#define WITH_ERROR(flags)   WITH_LOG_IMPL(ERROR,   flags)

#define LOG_OUTPUT(flags)  util::cout_streamer().stream()

#define LOG_ABORT postrisc::util::LogAbort() += \
    postrisc::util::Logger(__FILENAME__, __LINE__, __FUNCTION__).stream()

} // namespace postrisc
