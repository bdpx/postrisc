#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Utils.hpp"
#include "Debugger.hpp"
#include "DumpFormatter.hpp"

namespace postrisc {

#define DECLARE_DEBUG_COMMANDS(X) \
    X(help, "print this help") \
    X(dump, "dump full core state") \
    X(go,   "exit debugger, continue execution") \
    X(out,  "print register content") \
    X(step, "execute single instruction") \

enum debug_command : u8 {
#define X(name, descr) debug_cmd_##name,
    DECLARE_DEBUG_COMMANDS(X)
#undef X
};

#define X(name, descr) { debug_cmd_##name, #name },
static const constexpr auto debugCommandsTable = createSortedNameTable(
    []() { return std::to_array<NameInfo<debug_command>>({ DECLARE_DEBUG_COMMANDS(X) }); },
    [](size_t N) -> size_t { return N; },
    []() { return std::string_view("?"); }
    );
#undef X

#define X(name, descr) { debug_cmd_##name, descr },
static const constexpr auto debugCommandsDescription = createSortedNameTable(
    []() { return std::to_array<NameInfo<debug_command>>({ DECLARE_DEBUG_COMMANDS(X) }); },
    [](size_t N) -> size_t { return N; },
    []() { return std::string_view("?"); }
    );
#undef X

void Debugger::debug_prompt()
{
    std::cerr << "press CTRL^C do debug" << std::endl;
}

void Debugger::setup()
{
    debug_prompt();
}

bool Debugger::debug_dump(std::istream& UNUSED(in))
{
    DumpFormatter formatter(as.get_decoder(), std::cerr, DumpFormatterOptions::whole_data);
    as.dump_mapping(formatter);
    return true;
}

bool Debugger::debug_go(std::istream& UNUSED(in))
{
    std::cerr << "continue execution..." << std::endl;
    debug_prompt();
    return false;
}

bool Debugger::debug_help(std::istream& UNUSED(in))
{
    for (const debug_command cmd : debugCommandsTable.sorted_range()) {
        std::cerr << debugCommandsTable.name(cmd)
                  << "\n    " << debugCommandsDescription.name(cmd) << '\n';
    }

    std::cerr << std::endl;

    return true;
}

bool Debugger::debug_outsr(std::istream& UNUSED(in))
{
    return true;
}

bool Debugger::debug_out(std::istream& UNUSED(in))
{
    return true;
}

bool Debugger::debug_step(std::istream& UNUSED(in))
{
    LOG_DEBUG(LOG_DEBUGGER) << "single step start";
    Result result = core->post_interrupt(Result::single_step_trap);
    result = core->execute_until_interruption(as);
    assert(result == Result::single_step_trap);
    LOG_DEBUG(LOG_DEBUGGER) << "single step end: " << result;
    return true;
}

void Debugger::execute()
{
    for (;;) {
        std::cerr << "debug:" << std::flush;
        std::string input;
        std::getline(std::cin, input);
        std::istringstream stream(input);

        std::string command;
        stream >> command;
        std::cerr << command << std::endl;

        const std::optional<debug_command> cmd = debugCommandsTable.parse(command);
        if (cmd) {
            switch (*cmd) {
#define X(name, descr) case debug_cmd_ ## name: {                     \
                const bool continue_debugging = debug_##name(stream); \
                if (!continue_debugging) return;                      \
                } break;                                              \

            DECLARE_DEBUG_COMMANDS(X)
            }
#undef X
        } else {
            std::cerr << "unknown debug command: " << command << std::endl;
        }
    }
}

} // namespace postrisc
