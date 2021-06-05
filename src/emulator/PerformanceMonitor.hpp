#pragma once

#include "arch/Bundle.hpp"
#include "emulator/Result.hpp"

namespace postrisc {

#define DEFINE_PERFORMANCE_MONITOR_TABLE(X) \
    X(RUNTIME) \
    X(SHORT_INSTRUCTION) \
    X(LONG_INSTRUCTION) \
    X(NOP_INSTRUCTION) \
    X(QUALIFIED_NOP_INSTRUCTION) \
    X(SHADOWED_SLOT) \
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
    X(UNALIGNED_READ) \
    X(UNALIGNED_WRITE) \
    X(LAST) \

enum monitor_event {
#define DEFINE_PERFORMANCE_MONITOR_ENUM(NAME) PMC_##NAME,
    DEFINE_PERFORMANCE_MONITOR_TABLE(DEFINE_PERFORMANCE_MONITOR_ENUM)
#undef DEFINE_PERFORMANCE_MONITOR_ENUM
};

class DumpFormatter;

class PerformanceMonitor {
    friend class boost::serialization::access;
    template<typename Archive> void serialize(Archive & ar, const unsigned int version);

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

    void count(const InstructionSingle &) { report_event(PMC_SHORT_INSTRUCTION); }
    void count(const InstructionDouble &) { report_event(PMC_LONG_INSTRUCTION); }

    Result Get(u64 index, u64& a)
    {
        if (index < mr.size()) {
            a = mr[index];
            return Result::continue_execution;
        }
        return Result::device_error;
    }

    Result Set(u64 c, u64 index)
    {
        if (index < mr.size()) {
            mr[index] = c;
            return Result::continue_execution;
        }
        return Result::device_error;
    }

    void dump_statistic(u64 fetched_bundles, bool timing, const DumpFormatter& out) const;

    u64 GetPerfInsn(void) const { return mr[PMC_SHORT_INSTRUCTION] + mr[PMC_LONG_INSTRUCTION]; }
    u64 GetPerfTime(void) const { return mr[PMC_RUNTIME]; }

private:
    std::array<u64, PMC_LAST>  mr;
};

template<typename Archive>
void
PerformanceMonitor::serialize(Archive & ar, const unsigned int UNUSED(version))
{
    ar & BOOST_SERIALIZATION_NVP(mr);
}

} // namespace postrisc
