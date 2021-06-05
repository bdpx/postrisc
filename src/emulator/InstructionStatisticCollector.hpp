#pragma once

#include "arch/isa.hpp"
#include "DumpFormatter.hpp"

namespace postrisc {

class InstructionStatisticCollector {
public:
    InstructionStatisticCollector(void) {
        m_stat.fill(0);
    }

    void count(EInstructionID id) { m_stat[id]++; }
    void dump(u64 cmd1, u64 cmd2, const DumpFormatter& out, char const *title) const;

private:
    std::array<u64, INSTRUCTION_LIST_SIZE> m_stat;
};

} // namespace postrisc
