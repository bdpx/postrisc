#pragma once

#include "arch/instructions.hpp"
#include "emulator/interruption.hpp"

namespace postrisc {

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

class DumpFormatter;

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
    void report_event(monitor_event event, u64 inc)
    {
        mr[event] += inc;
    }

    void count(const CInsn1 &) { report_event(PMC_SHORT_INSTRUCTION); }
    void count(const CInsn2 &) { report_event(PMC_LONG_INSTRUCTION); }

    CStatus Get(u64 index, u64& a)
    {
        if (index < mr.size()) {
            a = mr[index];
            return CStatus::continue_execution;
        }
        return CStatus::device_error;
    }

    CStatus Set(u64 c, u64 index)
    {
        if (index < mr.size()) {
            mr[index] = c;
            return CStatus::continue_execution;
        }
        return CStatus::device_error;
    }

    void dump_statistic(u64 itc, bool timing, const DumpFormatter& out) const;

    u64 GetPerfInsn(void) const { return mr[PMC_SHORT_INSTRUCTION] + mr[PMC_LONG_INSTRUCTION]; }

private:
    std::array<u64, PMC_LAST>  mr;
};

template<class Archive>
void
CPerformanceMonitor::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(mr);
}

} // namespace postrisc
