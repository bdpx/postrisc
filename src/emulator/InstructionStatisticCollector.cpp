#include "InstructionStatisticCollector.hpp"

namespace postrisc {

class InstructionSorter {
public:
    InstructionSorter(EInstructionID _id, u64 _freq) : id(_id), freq(_freq) {}
    bool operator>(const InstructionSorter& r) const {
        return freq > r.freq || (freq == r.freq && std::strcmp(insn_name(id), insn_name(r.id)) < 0);
    }
public:
    EInstructionID id;
    u64    freq;
};

void InstructionStatisticCollector::dump(const DumpFormatter& out, char const *title) const
{
    std::vector<InstructionSorter> freq_info;
    u64 all = 0;
    for (size_t i = 0; i < m_stat.size(); ++i) {
        if (m_stat[i] > 0) {
            freq_info.emplace_back(static_cast<EInstructionID>(i), m_stat[i]);
            all += m_stat[i];
        }
    }

    if (all == 0) return;

    if (out.is_html()) {
        out << "<table>\n";
        out << "<caption>" << title << ": " << all << "</caption>\n";
    } else {
        out << title << ": " << all << '\n';
    }

    std::sort(freq_info.begin(), freq_info.end(), std::greater<InstructionSorter>());

    if (out.is_html()) {
        out << "<thead>\n"
               "<tr>\n"
               "<th>position</th>\n"
               "<th>instruction</th>\n"
               "<th>occurrence</th>\n"
               "<th>percent</th>\n"
               "<th>cumulative<br/>percent</th>\n"
               "</tr>\n"
               "</thead>\n";
    }

    u64 reported = 0;
    int position = 0;
    double const percent = 100.0 / all;

    if (out.is_html()) {
        out << "<tbody>\n";
    }
    out << std::fixed << std::setprecision(6);
    for (const auto& x : freq_info) {
        reported += x.freq;
        ++position;
        if (out.is_html()) {
            out
            << "<tr>"
            << "<td>" << position << "</td>"
            << "<td>" << insn_name(x.id) << "</td>"
            << "<td>" << x.freq << "</td>"
            << "<td>" << (x.freq   * percent) << "</td>"
            << "<td>" << (reported * percent) << "</td>"
            << "</tr>\n";
        } else {
            out << std::setw(4) << position
                << ' ' << std::setw(12) << x.freq
                << ' ' << std::setw(10) << (x.freq   * percent)
                << ' ' << std::setw(10) << (reported * percent)
                << ' ' << insn_name(x.id)
                << '\n';
        }
        // if (reported >= all * 0.995) break;
    }
    if (out.is_html()) {
        out << "</tbody>\n"
               "</table>\n";
    }
}

} // namespace postrisc
