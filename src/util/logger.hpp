#pragma once

#include <stdexcept>         // for std::runtime_error
#include <source_location>   // for std::source_location


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
// logging flags may be used for command line mask

#define DEFINE_LOG_OPTIONS_TABLE(X) \
    X(LOG_ALWAYS            , 0x00000001)  /* always log */ \
    X(LOG_TOKENIZER         , 0x00000002)  /* assembler tokenizer */ \
    X(LOG_PARSER            , 0x00000004)  /* assembler parser */ \
    X(LOG_EVALUATE          , 0x00000008)  /* assembler evaluator */ \
    X(LOG_EMULATOR          , 0x00000010)  /* common emulator */ \
    X(LOG_DECODER           , 0x00000020)  /* instructions decoding */ \
    X(LOG_LOADER            , 0x00000040)  /* raw/elf loader */ \
    X(LOG_PREDICATION       , 0x00000080)  /* predicated execution */ \
    X(LOG_REGISTER_ROTATION , 0x00000100)  /* register rotation */ \
    X(LOG_REGISTER_STACK    , 0x00000200)  /* register backing store */ \
    X(LOG_FPU               , 0x00000400)  /* floating point unit */ \
    X(LOG_EXCEPTION         , 0x00000800)  /* software traps, hardware interruptions/interrupts */ \
    X(LOG_LOAD              , 0x00001000)  /* memory load access */ \
    X(LOG_STORE             , 0x00002000)  /* memory store access */ \
    X(LOG_ITLB              , 0x00004000)  /* instruction TLB */ \
    X(LOG_ICACHE            , 0x00008000)  /* instruction cache */ \
    X(LOG_PAGETABLE         , 0x00010000)  /* in-memory page table */ \
    X(LOG_PLATFORM          , 0x00020000)  /* physical memory emulator, devices etc */ \
    X(LOG_CALLSTACK         , 0x00040000)  /* call/return/alloc/enter/leave */ \
    X(LOG_DTLB              , 0x00080000)  /* data TLB */ \
    X(LOG_DCACHE            , 0x00100000)  /* data cache */ \
    X(LOG_SYSCALL           , 0x00200000)  /* system calls/returns */ \
    X(LOG_DEBUGGER          , 0x00400000)  /* internal debugger */ \
    X(LOG_REGISTER_DATA     , 0x00800000)  /* register content */ \
    X(LOG_BRANCH            , 0x01000000)  /* taken/untaken branches, ip monitoring */ \
    X(LOG_INTERRUPT         , 0x02000000)  /* interrupts, IPI device */ \
    X(LOG_DISPLAY           , 0x04000000)  /* video device */ \
    X(LOG_SERIALIZATION     , 0x08000000)  /* serialization */ \
    X(LOG_DUMP              , 0x10000000)  /* dumping state */ \
    X(LOG_INSN_TRACE        , 0x20000000)  /* bundle/instruction tracing */ \
    X(LOG_DOOM              , 0x40000000)  /* doom emulator */ \


enum log_subsystem : u32 {
#define X(NAME,VALUE) NAME = VALUE,
    DEFINE_LOG_OPTIONS_TABLE(X)
#undef X

#if defined(POSTRISC_RUNTIME_LOGS)
// no default mask
#else
    LOG_CURRENT_MASK = 0
#define X(NAME,VALUE) | NAME
    DEFINE_LOG_OPTIONS_TABLE(X)
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
    Logger(const std::source_location location = std::source_location::current());

    std::ostream& stream() const {
        return dbgs();
    }

    static void ExportLogOptions(std::ostream& out);
    static void OpenLogFile(const std::string& filename);
    static void CloseLogFile(void);

#if defined(POSTRISC_RUNTIME_LOGS)
    static void set_level(u32 level) { s_log_level = level; }
    static void set_subsystem_mask(u32 mask) { s_subsystem_mask = mask | LOG_ALWAYS; }

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
#define WITH_LOG_IMPL(LEVEL, FLAG) \
   if (postrisc::util::Logger::enabled(postrisc::LOG_LEVEL_##LEVEL, postrisc::FLAG))

#define LOG_IMPL(LEVEL, FLAG) \
   if (!postrisc::util::Logger::enabled(postrisc::LOG_LEVEL_##LEVEL, postrisc::FLAG)) {} \
   else postrisc::util::LogFinalizer() += \
        postrisc::util::Logger(std::source_location::current()).stream()


#define LOG_DEBUG(flags)   LOG_IMPL(DEBUG,   flags)
#define LOG_INFO(flags)    LOG_IMPL(INFO,    flags)
#define LOG_NOTICE(flags)  LOG_IMPL(NOTICE,  flags)
#define LOG_WARNING(flags) LOG_IMPL(WARNING, flags)
#define LOG_ERROR(flags)   LOG_IMPL(ERROR,   flags)
#define LOG_CRIT(flags)    LOG_IMPL(CRIT,    flags)

#define WITH_DEBUG(flags)   WITH_LOG_IMPL(DEBUG,   flags)
#define WITH_INFO(flags)    WITH_LOG_IMPL(INFO,    flags)
#define WITH_WARNING(flags) WITH_LOG_IMPL(WARNING, flags)
#define WITH_ERROR(flags)   WITH_LOG_IMPL(ERROR,   flags)

#define LOG_OUTPUT(flags)  util::cout_streamer().stream()

#define LOG_ABORT postrisc::util::LogAbort() += \
    postrisc::util::Logger(std::source_location::current()).stream()

} // namespace postrisc
