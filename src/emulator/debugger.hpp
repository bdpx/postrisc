#pragma once

#include "core.hpp"

namespace postrisc {

class Debugger
{
public:
    Debugger(boost::intrusive_ptr<Core> _core) : core(_core) {}
    ~Debugger(void) {}
    void execute();

    static void setup();

    using command_handler = bool (Debugger::*)(std::istream & in);

    bool debug_dump(std::istream & in);
    bool debug_go(std::istream & in);
    bool debug_help(std::istream & in);
    bool debug_outsr(std::istream & in);
    bool debug_out(std::istream & in);
    bool debug_step(std::istream & in);

private:
    static void debug_prompt();

private:
    boost::intrusive_ptr<Core> core;
};

} // namespace postrisc
