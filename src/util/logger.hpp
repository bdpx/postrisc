#pragma once

#include <stdexcept>  // for runtime_error

// globally visible
namespace postrisc {

enum log_level {
    LOG_LEVEL_VERBOSE,
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_ABORT,
    LOG_LEVEL_HIGHEST,
    LOG_LEVEL_CURRENT = LOG_LEVEL_FATAL
};

enum log_subsystem : uint32_t {
    LOG_ONE                 = 1,
    LOG_TOKENIZER           = LOG_ONE <<  0,
    LOG_PARSER              = LOG_ONE <<  1,
    LOG_EVALUATE            = LOG_ONE <<  2,
    LOG_EMULATOR            = LOG_ONE <<  3,
    LOG_LOADER              = LOG_ONE <<  4,
    LOG_PREDICATION         = LOG_ONE <<  5,  // predicated execution
    LOG_REGISTER_ROTATION   = LOG_ONE <<  6,  // register rotation
    LOG_REGISTER_STACK      = LOG_ONE <<  7,  // register backing store
    LOG_FPU                 = LOG_ONE <<  8,  // floating point unit
    LOG_EXCEPTION           = LOG_ONE <<  9,  // software traps, hardware interruptions/interrupts
    LOG_LOAD                = LOG_ONE << 10,  // memory load access
    LOG_STORE               = LOG_ONE << 11,  // memory store access
    LOG_ITLB                = LOG_ONE << 12,  // instruction TLB
    LOG_ICACHE              = LOG_ONE << 13,  // instruction cache
    LOG_PAGETABLE           = LOG_ONE << 14,  // in-memory page table
    LOG_PLATFORM            = LOG_ONE << 15,  // physical memory emulator, devices etc
    LOG_CALLSTACK           = LOG_ONE << 16,  // call/return/alloc/enter/leave
    LOG_DTLB                = LOG_ONE << 17,  // data TLB
    LOG_DCACHE              = LOG_ONE << 18,  // data cache
    LOG_SYSCALL             = LOG_ONE << 19,  // system calls/returns
    LOG_DEBUGGER            = LOG_ONE << 20,  // internal debugger
    LOG_REGISTER_DATA       = LOG_ONE << 21,  // register content
    LOG_BRANCH              = LOG_ONE << 22,  // taken/untaken branches, ip monitoring
    LOG_INTERRUPT           = LOG_ONE << 23,  // interrupts, IPI device
    LOG_DISPLAY             = LOG_ONE << 24,  // video device
    LOG_SERIALIZATION       = LOG_ONE << 25,  // serialization

    LOG_CURRENT_MASK = 0
               | LOG_TOKENIZER
               | LOG_PARSER
               | LOG_EVALUATE
               | LOG_EMULATOR
               | LOG_LOADER
               | LOG_PREDICATION
               | LOG_REGISTER_ROTATION
               | LOG_REGISTER_STACK
               | LOG_FPU
               | LOG_EXCEPTION
               | LOG_LOAD
               | LOG_STORE
               | LOG_ITLB
               | LOG_ICACHE
               | LOG_PAGETABLE
               | LOG_PLATFORM
               | LOG_CALLSTACK
               | LOG_DTLB
               | LOG_DCACHE
               | LOG_SYSCALL
               | LOG_DEBUGGER
               | LOG_REGISTER_DATA
               | LOG_BRANCH
               | LOG_INTERRUPT
               | LOG_DISPLAY
               | LOG_SERIALIZATION
};

constexpr inline log_subsystem operator | (log_subsystem a, log_subsystem b)
{
    return static_cast<log_subsystem>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

constexpr inline log_subsystem operator & (log_subsystem a, log_subsystem b)
{
    return static_cast<log_subsystem>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
}

namespace util {

class cout_streamer {
public:
    cout_streamer();
    ~cout_streamer();
    std::ostream & stream() const {
        return std::cout;
    }
};

class Logger {
public:
    Logger(log_level level, const char *file, int line, const char *function);
    ~Logger();

    std::ostream & stream() const {
        return std::cerr;
    }

    static constexpr bool enabled(enum log_level level, enum log_subsystem subsystem_flags)
    {
        return (level >= LOG_LEVEL_CURRENT)
               && (subsystem_flags & LOG_CURRENT_MASK) != 0;
    }

private:
    static const char * GetLevelName(log_level level);
};

class LogFinalizer {
public:
    LogFinalizer() {}
    void operator += (std::ostream & stream) const {
        stream << std::endl;
    }
};

/****************************************************************************
*  fatal error messaging
****************************************************************************/
class LogAbort {
public:
    LogAbort() { }

    void operator += (std::ostream & stream) const {
        stream << std::endl;
        throw std::runtime_error("syntax error");
    }
};

} // namespace util

// assumed operator precedence: +=, <<
// see root CMakeList for __FILENAME__ definition
#define LOG_IMPL(LEVEL, FLAG) \
   if (!postrisc::util::Logger::enabled(postrisc::LOG_LEVEL_##LEVEL, postrisc::FLAG)) {} \
   else postrisc::util::LogFinalizer() += \
        postrisc::util::Logger(postrisc::LOG_LEVEL_##LEVEL, __FILENAME__, __LINE__, __FUNCTION__).stream()


#define LOG_VERBOSE(flags) LOG_IMPL(VERBOSE, flags)
#define LOG_TRACE(flags)   LOG_IMPL(TRACE,   flags)
#define LOG_DEBUG(flags)   LOG_IMPL(DEBUG,   flags)
#define LOG_INFO(flags)    LOG_IMPL(INFO,    flags)
#define LOG_WARNING(flags) LOG_IMPL(WARNING, flags)
#define LOG_ERROR(flags)   LOG_IMPL(ERROR,   flags)
#define LOG_FATAL(flags)   LOG_IMPL(FATAL,   flags)

#define LOG_OUTPUT(flags)  util::cout_streamer().stream()

#define LOG_ABORT \
   postrisc::util::LogAbort() += \
   postrisc::util::Logger(postrisc::LOG_LEVEL_ABORT, __FILE__, __LINE__, __FUNCTION__).stream()


#define CM_TRACE_FILE(trace_file)    postrisc::util::Trace::LogToFile(trace_file)
#define CM_TRACE_FUNC(func_name)    postrisc::util::Trace __CM_TRACE__(func_name, "()")
#define CM_TRACE_FUNC_ARG1(func_name, argfmt, arg)  postrisc::util::Trace __CM_TRACE__(func_name, argfmt, arg)
#define CM_TRACE_FUNC_ARG2(func_name, argfmt, arg1, arg2) postrisc::util::Trace __CM_TRACE__(func_name, argfmt, arg1, arg2)

namespace util {

class Trace {
public:
    explicit Trace(char *func_name, const char* argsfmt, ...);
    ~Trace();
    static void LogToFile(const char *trace_file);

private:
    void LogMsg(int depth, int align, const char *fmt, va_list args);

private:
    // the debug trace filename
    static std::string    trace_file_;

    // function call stack depth
    static int          depth_;
    static const char   nest_[70];
    static time_t       last_invoke_time_;
};

} // namespace util
} // namespace postrisc
