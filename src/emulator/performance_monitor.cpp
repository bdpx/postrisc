#include "util/common.hpp"
#include "arch/isa.hpp"
#include "dump/formatter.hpp"
#include "performance_monitor.hpp"

namespace postrisc {

void
CPerformanceMonitor::dump_statistic(u64 itc, bool timing, const DumpFormatter& out) const
{
    out << "performance monitor:\n";

    u64 const insn_summary = GetPerfInsn();

    out << std::fixed << std::setprecision(6) << std::setfill(' ');

    // don't output timing info in unittests
    if (timing) {
        double const mips = 1.0e-6 * insn_summary; // from instructions to mips
        double const runtime = 1.0e-9 * mr[PMC_RUNTIME]; // from nanoseconds to seconds
        if (runtime > 0.001) {
            out << fmt::table("performance") << (mips / runtime) << " mips";
        } else {
            out << "performance not defined, time too short";
        }
        out << fmt::endl;
        out << fmt::table("time") << mr[PMC_RUNTIME] << ' ' << runtime << " seconds\n";
    }

    out << fmt::table("bundles fetched") << itc << fmt::endl;
    out << fmt::table("slots fetched") << (slots_per_bundle * itc) << fmt::endl;
    out << fmt::table("instructions issued") << insn_summary << fmt::endl;

    if (insn_summary > 0) {
        double const percent = 100.0 / insn_summary;
        out << fmt::table("short instructions") << mr[PMC_SHORT_INSTRUCTION]         << ' ' << (percent * mr[PMC_SHORT_INSTRUCTION])         << '%' << fmt::endl;
        out << fmt::table("long instructions" ) << mr[PMC_LONG_INSTRUCTION]          << ' ' << (percent * mr[PMC_LONG_INSTRUCTION])          << '%' << fmt::endl;
        out << fmt::table("nops"              ) << mr[PMC_NOP_INSTRUCTION]           << ' ' << (percent * mr[PMC_NOP_INSTRUCTION])           << '%' << fmt::endl;
        out << fmt::table("qualified nops"    ) << mr[PMC_QUALIFIED_NOP_INSTRUCTION] << ' ' << (percent * mr[PMC_QUALIFIED_NOP_INSTRUCTION]) << '%' << fmt::endl;
    }

    out << fmt::table("register spills"      ) << mr[PMC_REGISTER_SPILL]               << fmt::endl;
    out << fmt::table("register fills"       ) << mr[PMC_REGISTER_FILL]                << fmt::endl;
    out << fmt::table("code cache hits"      ) << mr[PMC_ICACHE_HIT]                   << fmt::endl;
    out << fmt::table("code cache missess"   ) << mr[PMC_ICACHE_MISS]                  << fmt::endl;
    out << fmt::table("data cache hits"      ) << mr[PMC_DCACHE_HIT]                   << fmt::endl;
    out << fmt::table("data cache missess"   ) << mr[PMC_DCACHE_MISS]                  << fmt::endl;
    out << fmt::table("code TLB hits"        ) << mr[PMC_INSTRUCTION_TRANSLATION_HIT]  << fmt::endl;
    out << fmt::table("code TLB misses"      ) << mr[PMC_INSTRUCTION_TRANSLATION_MISS] << fmt::endl;
    out << fmt::table("data TLB hits"        ) << mr[PMC_DATA_TRANSLATION_HIT]         << fmt::endl;
    out << fmt::table("data TLB misses"      ) << mr[PMC_DATA_TRANSLATION_MISS]        << fmt::endl;
    out << fmt::table("backstore TLB hits"   ) << mr[PMC_BACKSTORE_TRANSLATION_HIT]    << fmt::endl;
    out << fmt::table("backstore TLB misses" ) << mr[PMC_BACKSTORE_TRANSLATION_MISS]   << fmt::endl;
}

} //namespace postrisc
