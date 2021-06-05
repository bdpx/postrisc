#include <string_view>

#include "util/common.hpp"
#include "CpuIdentificationStore.hpp"
#include "PerformanceMonitor.hpp"

namespace postrisc {

CpuIdentificationStore::CpuIdentificationStore()
    : version(1)
    , capabilities(0)
    , L1_ICache(64, 4,       32 * 1024)
    , L1_DCache(64, 4,       32 * 1024)
    , L2_DCache(64, 8,      256 * 1024)
    , L3_DCache(64, 8, 2 * 1024 * 1024)
    , L1_ITLB(64)
    , L1_DTLB(64)
    , L2_TLB(256)
{
}

u64
CpuIdentificationStore::Get(u64 index) const
{
    switch (index) {
        case CPUID_SIZE:            return CPUID_MAX;
        case CPUID_VERSION:         return version;
        case CPUID_CAPABILITIES:    return capabilities;
        case CPUID_L1I:             return L1_ICache.Get();
        case CPUID_L1D:             return L1_DCache.Get();
        case CPUID_L2D:             return L2_DCache.Get();
        case CPUID_L3D:             return L3_DCache.Get();
        case CPUID_L1_ITLB:         return L1_ITLB.Get();
        case CPUID_L1_DTLB:         return L1_DTLB.Get();
        case CPUID_L2_TLB:          return L2_TLB.Get();
        case CPUID_PMR_MAX:         return PMC_LAST;
        case CPUID_MAX: break;
    }

    LOG_DEBUG(LOG_EMULATOR)
        << "index=" << index
        << " is out of supported range 0.." << CPUID_MAX;
    return 0;
}

void
CpuIdentificationStore::dump(const DumpFormatter& out) const
{
    if (out.is_html()) {
        out << "<table><caption>cpuid</caption>"
               "<thead><tr><th>name</th><th>value</th></tr>"
               "</thead><tbody>";

        out << "<tr><td>size</td><td>"        << CPUID_MAX << "</td></tr>\n";
        out << "<tr><td>version</td><td>"     << fmt::hex(version) << "</td></tr>\n";
        out << "<tr><td>capabilities</td><td>"<< fmt::hex(capabilities) << "</td></tr>\n";
        out << "<tr><td>L1 icache</td><td>"   << L1_ICache << "</td></tr>\n";
        out << "<tr><td>L1 dcache</td><td>"   << L1_DCache << "</td></tr>\n";
        out << "<tr><td>L2 dcache</td><td>"   << L2_DCache << "</td></tr>\n";
        out << "<tr><td>L3 dcache</td><td>"   << L3_DCache << "</td></tr>\n";
        out << "<tr><td>L1 itlb</td><td>"     << L1_ITLB << "</td></tr>\n";
        out << "<tr><td>L1 dtlb</td><td>"     << L1_DTLB << "</td></tr>\n";
        out << "<tr><td>L2 tlb</td><td>"      << L2_TLB << "</td></tr>\n";
        out << "<tr><td>PMR_MAX</td><td>"     << PMC_LAST << "</td></tr>\n";
        out << "</tbody></table>";
    } else {
        out << "cpuid:\n";
        out << fmt::table("size")        << CPUID_MAX << fmt::endl;
        out << fmt::table("version")     << fmt::hex(version) << fmt::endl;
        out << fmt::table("capabilities")<< fmt::hex(capabilities) << fmt::endl;
        out << fmt::table("L1 icache")   << L1_ICache << fmt::endl;
        out << fmt::table("L1 dcache")   << L1_DCache << fmt::endl;
        out << fmt::table("L2 dcache")   << L2_DCache << fmt::endl;
        out << fmt::table("L3 dcache")   << L3_DCache << fmt::endl;
        out << fmt::table("L1 itlb")     << L1_ITLB << fmt::endl;
        out << fmt::table("L1 dtlb")     << L1_DTLB << fmt::endl;
        out << fmt::table("L2 tlb")      << L2_TLB << fmt::endl;
        out << fmt::table("PMR_MAX")     << PMC_LAST << fmt::endl;
    }
    out << '\n';
}

} // namespace postrisc
