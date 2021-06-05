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
    void dump(const DumpFormatter& out, char const *title) const;

private:
    std::array<u64, insn_reserved> m_stat;
};

} // namespace postrisc
