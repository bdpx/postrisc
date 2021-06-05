#include "util/common.hpp"
#include "arch/isa.hpp"
#include "DumpFormatter.hpp"
#include "PerformanceMonitor.hpp"

namespace postrisc {

template<typename T> static void dump(const DumpFormatter& out, const char *name, T value)
{
    if (out.is_html()) {
        out << "<tr><td>";
    }
    out << name;
    if (out.is_html()) {
        out << "</td><td>";
    } else {
        out << ": ";
    }
    out << value;
    if (out.is_html()) {
        out << "</td></tr>";
    }
    out << '\n';
}

template<typename T> static void dump(const DumpFormatter& out, const char *name, T value, double percent)
{
    if (out.is_html()) {
        out << "<tr><td>";
    }
    out << name;
    if (out.is_html()) {
        out << "</td><td>";
    } else {
        out << ": ";
    }
    out << value << ' ' << (percent * value) << '%';
    if (out.is_html()) {
        out << "</td></tr>";
    }
    out << '\n';
}

void
PerformanceMonitor::dump_statistic(u64 fetched_bundles, bool timing, const DumpFormatter& out) const
{
    if (out.is_html()) {
        out << "<table>"
           "<caption>performance monitor</caption>"
           "<thead>"
           "<tr>"
           "<th>name</th>"
           "<th>value</th>"
           "</tr>"
           "</thead>"
           "<tbody>";
    } else {
       out << "performance monitor\n";
    }

    u64 const summary = GetPerfInsn();

    out << std::fixed << std::setprecision(6);

    // don't output timing info in unittests
    if (timing) {
        const double mips = 1.0e-6 * summary; // from instructions to mips
        const double runtime = 1.0e-9 * mr[PMC_RUNTIME]; // from nanoseconds to seconds
        const double sys_runtime = 1.0e-9 * mr[PMC_SYS_RUNTIME];
        const double user_runtime = runtime - sys_runtime;

        if (out.is_html()) {
            out << "<tr><td>performance</td><td>";
        } else {
            out << "performance: ";
        }
        if (user_runtime > 0.001) {
            out << (mips / user_runtime) << " mips";
            if (out.is_html()) {
                out << "</td></tr>";
            }
            out << '\n';
        } else {
            out << "not defined, time too short";
            if (out.is_html()) {
                out << "</td></tr>";
            }
            out << '\n';
        }

        if (out.is_html()) {
            out << "<tr><td>user time</td><td>";
        } else {
            out << "user time: ";
        }
        out << mr[PMC_RUNTIME] << ' ' << user_runtime << " seconds";
        if (out.is_html()) {
            out << "</td></tr>";
        }
        out << '\n';

        if (out.is_html()) {
            out << "<tr><td>sys time</td><td>";
        } else {
            out << "sys time: ";
        }
        out << mr[PMC_SYS_RUNTIME] << ' ' << sys_runtime << " seconds";
        if (out.is_html()) {
            out << "</td></tr>";
        }
        out << '\n';
    }

    dump(out, "bundles fetched", fetched_bundles);
    dump(out, "slots fetched", slots_per_bundle * fetched_bundles);
    dump(out, "instructions issued", summary);

    if (summary > 0) {
        double const percent = 100.0 / summary;
        dump(out, "short instructions", mr[PMC_SHORT_INSTRUCTION], percent);
        dump(out, "long instructions", mr[PMC_LONG_INSTRUCTION], percent);
        dump(out, "shadowed slots", mr[PMC_SHADOWED_SLOT], percent);
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
    dump(out, "unaligned reads", mr[PMC_UNALIGNED_READ]);
    dump(out, "unaligned writes", mr[PMC_UNALIGNED_WRITE]);

    if (out.is_html()) {
        out << "</tbody></table>";
    }
    out << '\n';
}

} //namespace postrisc
