#pragma once

#include "Core.hpp"

namespace postrisc {

class Debugger
{
public:
    Debugger(AddressSpace& as_, boost::intrusive_ptr<Core> _core) : as(as_), core(_core) {}
    ~Debugger(void) {}
    void execute();

    static void setup();

    using command_handler = bool (Debugger::*)(std::istream& in);

    bool debug_dump(std::istream& in);
    bool debug_go(std::istream& in);
    bool debug_help(std::istream& in);
    bool debug_outsr(std::istream& in);
    bool debug_out(std::istream& in);
    bool debug_step(std::istream& in);

private:
    static void debug_prompt();

private:
    AddressSpace& as;
    boost::intrusive_ptr<Core> core;
};

} // namespace postrisc
