#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Utils.hpp"
#include "Debugger.hpp"
#include "DumpFormatter.hpp"

namespace postrisc {

static const struct {
    char name[8];
    Debugger::command_handler func;
    char const *description; //[40];
} cmd_handlers[] = {
   { "help",  &Debugger::debug_help,  "print this help" },
   { "dump",  &Debugger::debug_dump,  "dump full core state" },
   { "go",    &Debugger::debug_go,    "exit debugger, continue execution" },
   { "out",   &Debugger::debug_out,   "print register content" },
   { "step",  &Debugger::debug_step,  "execute single instruction" },
};


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
    PhysMemoryMapping mapping(PhysAddress(0), as.get_page_size(), core);
    DumpFormatter formatter(as.get_decoder(), std::cerr, false);
    core->dump_mapping(mapping, formatter);
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
    for (const auto& cmd : cmd_handlers) {
        std::cerr << cmd.name
                  << "\n    " << cmd.description << '\n';
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

        for (const auto & x : cmd_handlers) {
            if (command == x.name) {
                const bool continue_debugging = (this->*x.func)(stream);
                if (!continue_debugging) return;
                break;
            }
        }
    }
}

} // namespace postrisc
