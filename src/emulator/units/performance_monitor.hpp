#pragma once

#include "arch/interruption.hpp"
#include "arch/instructions.hpp"

namespace postrisc {

using postrisc::fpu::u128;

#define DEFINE_PERFORMANCE_MONITOR_TABLE(X) \
    X(RUNTIME) \
    X(SHORT_INSTRUCTION) \
    X(LONG_INSTRUCTION) \
    X(NOP_INSTRUCTION) \
    X(QUALIFIED_NOP_INSTRUCTION) \
    X(REGISTER_SPILL) \
    X(REGISTER_FILL) \
    X(ICACHE_HIT) \
    X(ICACHE_MISS) \
    X(DCACHE_HIT) \
    X(DCACHE_MISS) \
    X(INSTRUCTION_TRANSLATION_HIT) \
    X(INSTRUCTION_TRANSLATION_MISS) \
    X(DATA_TRANSLATION_HIT) \
    X(DATA_TRANSLATION_MISS) \
    X(BACKSTORE_TRANSLATION_HIT) \
    X(BACKSTORE_TRANSLATION_MISS) \
    X(LAST) \

enum monitor_event {
#define DEFINE_PERFORMANCE_MONITOR_ENUM(NAME) PMC_##NAME,
    DEFINE_PERFORMANCE_MONITOR_TABLE(DEFINE_PERFORMANCE_MONITOR_ENUM)
#undef DEFINE_PERFORMANCE_MONITOR_ENUM
};

class CPerformanceMonitor {
    friend class boost::serialization::access;
    template<class Archive> void serialize(Archive & ar, const unsigned int version);

public:
    void reset(void) {
        for (auto & x : mr) {
            x = 0;
        }
    }

    void report_event(monitor_event event)
    {
        ++mr[event];
    }
    void report_event(monitor_event event, uint64_t inc)
    {
        mr[event] += inc;
    }

    void count(const CInsn1 &) { report_event(PMC_SHORT_INSTRUCTION); }
    void count(const CInsn2 &) { report_event(PMC_LONG_INSTRUCTION); }

    CStatus Get(uint64_t index, uint64_t& a)
    {
        if (index < mr.size()) {
            a = mr[index];
            return CStatus(CStatus::continue_execution);
        }
        return CStatus(CStatus::device_error);
    }

    CStatus Set(uint64_t c, uint64_t index)
    {
        if (index < mr.size()) {
            mr[index] = c;
            return CStatus(CStatus::continue_execution);
        }
        return CStatus(CStatus::device_error);
    }

    void dump_statistic(uint64_t itc, bool timing, const DumpFormatter& out) const;

private:
    std::array<uint64_t, PMC_LAST>  mr;
};

template<class Archive>
void
CPerformanceMonitor::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(mr);
}

} // namespace postrisc
