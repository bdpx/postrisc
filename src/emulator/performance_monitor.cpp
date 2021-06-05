#include "util/common.hpp"
#include "arch/isa.hpp"
#include "dump/formatter.hpp"
#include "performance_monitor.hpp"

namespace postrisc {

template<typename T> static void dump(const DumpFormatter& out, const char *name, T value)
{
    out << "<tr><td>" << name << "</td><td>" << value << "</td></tr>\n";
}

template<typename T> static void dump(const DumpFormatter& out, const char *name, T value, double percent)
{
    out << "<tr><td>" << name << "</td><td>" << value << ' ' << (percent * value) << "%</td></tr>\n";
}

void
CPerformanceMonitor::dump_statistic(u64 itc, bool timing, const DumpFormatter& out) const
{
    out << "<table>"
           "<caption>performance monitor</caption>"
           "<thead>"
           "<tr>"
           "<th>name</th>"
           "<th>value</th>"
           "</tr>"
           "</thead>"
           "<tbody>";

    u64 const insn_summary = GetPerfInsn();

    out << std::fixed << std::setprecision(6);

    // don't output timing info in unittests
    if (timing) {
        double const mips = 1.0e-6 * insn_summary; // from instructions to mips
        double const runtime = 1.0e-9 * mr[PMC_RUNTIME]; // from nanoseconds to seconds
        out << "<tr><td>performance</td><td>";
        if (runtime > 0.001) {
            out << (mips / runtime) << " mips</td></tr>\n";
        } else {
            out << "not defined, time too short</td></tr>\n";
        }
        out << "<tr><td>time</td><td>" << mr[PMC_RUNTIME] << ' ' << runtime << " seconds</td></tr>\n";
    }

    dump(out, "bundles fetched", itc);
    dump(out, "slots fetched", slots_per_bundle * itc);
    dump(out, "instructions issued", insn_summary);

    if (insn_summary > 0) {
        double const percent = 100.0 / insn_summary;
        dump(out, "short instructions", mr[PMC_SHORT_INSTRUCTION], percent);
        dump(out, "long instructions", mr[PMC_LONG_INSTRUCTION], percent);
        dump(out, "shadowed instructions", mr[PMC_SHADOWED_INSTRUCTION], percent);
        dump(out, "nops", mr[PMC_NOP_INSTRUCTION], percent);
        dump(out, "qualified nops", mr[PMC_QUALIFIED_NOP_INSTRUCTION], percent);
    }

    dump(out, "register spills", mr[PMC_REGISTER_SPILL]);
    dump(out, "register fills", mr[PMC_REGISTER_FILL]);
    dump(out, "code cache hits", mr[PMC_ICACHE_HIT]);
    dump(out, "code cache missess", mr[PMC_ICACHE_MISS]);
    dump(out, "data cache hits", mr[PMC_DCACHE_HIT]);
    dump(out, "data cache missess", mr[PMC_DCACHE_MISS]);
    dump(out, "code TLB hits", mr[PMC_INSTRUCTION_TRANSLATION_HIT]);
    dump(out, "code TLB misses", mr[PMC_INSTRUCTION_TRANSLATION_MISS]);
    dump(out, "data TLB hits", mr[PMC_DATA_TRANSLATION_HIT]);
    dump(out, "data TLB misses", mr[PMC_DATA_TRANSLATION_MISS]);
    dump(out, "backstore TLB hits", mr[PMC_BACKSTORE_TRANSLATION_HIT]);
    dump(out, "backstore TLB misses", mr[PMC_BACKSTORE_TRANSLATION_MISS]);

    out << "</tbody></table>\n";
}

} //namespace postrisc
