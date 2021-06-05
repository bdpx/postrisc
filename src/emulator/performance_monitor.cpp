#include "util/common.hpp"
#include "arch/isa.hpp"
#include "dump/formatter.hpp"
#include "performance_monitor.hpp"

namespace postrisc {

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

    out << std::fixed << std::setprecision(6) << std::setfill(' ');

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

    out << "<tr><td>bundles fetched</td><td>" << itc << "</td></tr>\n";
    out << "<tr><td>slots fetched</td><td>" << (slots_per_bundle * itc) << "</td></tr>\n";
    out << "<tr><td>instructions issued</td><td>" << insn_summary << "</td></tr>\n";

    if (insn_summary > 0) {
        double const percent = 100.0 / insn_summary;
        out << "<tr><td>short instructions</td><td>"   << mr[PMC_SHORT_INSTRUCTION]         << ' ' << (percent * mr[PMC_SHORT_INSTRUCTION])         << "%</td></tr>\n";
        out << "<tr><td>long instructions</td><td>"    << mr[PMC_LONG_INSTRUCTION]          << ' ' << (percent * mr[PMC_LONG_INSTRUCTION])          << "%</td></tr>\n";
        out << "<tr><td>shadowed instructions</td><td>"<< mr[PMC_SHADOWED_INSTRUCTION]      << ' ' << (percent * mr[PMC_SHADOWED_INSTRUCTION])      << "%</td></tr>\n";
        out << "<tr><td>nops</td><td>"                 << mr[PMC_NOP_INSTRUCTION]           << ' ' << (percent * mr[PMC_NOP_INSTRUCTION])           << "%</td></tr>\n";
        out << "<tr><td>qualified nops</td><td>"       << mr[PMC_QUALIFIED_NOP_INSTRUCTION] << ' ' << (percent * mr[PMC_QUALIFIED_NOP_INSTRUCTION]) << "%</td></tr>\n";
    }

    out << "<tr><td>register spills</td><td>"      << mr[PMC_REGISTER_SPILL]               << "</td></tr>\n";
    out << "<tr><td>register fills</td><td>"       << mr[PMC_REGISTER_FILL]                << "</td></tr>\n";
    out << "<tr><td>code cache hits</td><td>"      << mr[PMC_ICACHE_HIT]                   << "</td></tr>\n";
    out << "<tr><td>code cache missess</td><td>"   << mr[PMC_ICACHE_MISS]                  << "</td></tr>\n";
    out << "<tr><td>data cache hits</td><td>"      << mr[PMC_DCACHE_HIT]                   << "</td></tr>\n";
    out << "<tr><td>data cache missess</td><td>"   << mr[PMC_DCACHE_MISS]                  << "</td></tr>\n";
    out << "<tr><td>code TLB hits</td><td>"        << mr[PMC_INSTRUCTION_TRANSLATION_HIT]  << "</td></tr>\n";
    out << "<tr><td>code TLB misses</td><td>"      << mr[PMC_INSTRUCTION_TRANSLATION_MISS] << "</td></tr>\n";
    out << "<tr><td>data TLB hits</td><td>"        << mr[PMC_DATA_TRANSLATION_HIT]         << "</td></tr>\n";
    out << "<tr><td>data TLB misses</td><td>"      << mr[PMC_DATA_TRANSLATION_MISS]        << "</td></tr>\n";
    out << "<tr><td>backstore TLB hits</td><td>"   << mr[PMC_BACKSTORE_TRANSLATION_HIT]    << "</td></tr>\n";
    out << "<tr><td>backstore TLB misses</td><td>" << mr[PMC_BACKSTORE_TRANSLATION_MISS]   << "</td></tr>\n";

    out << "</tbody>"
           "</table>\n";
}

} //namespace postrisc
