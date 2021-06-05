#include "Bundle.hpp"
#include "isa.hpp"

namespace postrisc {

Bundle::Bundle(const u64 slot[slots_per_bundle], EBundleTemplate tmplt)
{
    const u64 mask = util::makemask<u64>(bits_per_slot);

    const u64 s0 = slot[0] & mask;
    const u64 s1 = slot[1] & mask;
    const u64 s2 = slot[2] & mask;

    value = static_cast<u128>(tmplt) |
         (static_cast<u128>(s0) << (bits_per_bundle_template + 0*bits_per_slot)) |
         (static_cast<u128>(s1) << (bits_per_bundle_template + 1*bits_per_slot)) |
         (static_cast<u128>(s2) << (bits_per_bundle_template + 2*bits_per_slot));
}

char const *Bundle::bundle_template_2_str(EBundleTemplate tmplt)
{
    switch (tmplt) {
        case EBundleTemplate::bundle_sss: return "sss";
        case EBundleTemplate::bundle_sll: return "sll";
        case EBundleTemplate::bundle_lls: return "lls";
        case EBundleTemplate::bundle_lll: return "lll";
    }
    assert(!"unknown EBundleTemplate");
    return nullptr;
}

std::ostream& operator<<(std::ostream& out, const Bundle& bundle)
{
    /*
    out << ";"
           "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
           "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
           "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
           "tt\n;" << std::bitset<64>(a0) << std::bitset<64>(a1) << '\n';
    */
    return out
        << fmt::hex(static_cast<u64>(bundle.value >> 64))
        << '_'
        << fmt::hex(static_cast<u64>(bundle.value))
        << " A=" << fmt::hex<u64, 11>(bundle.GetSlotA())
        << " B=" << fmt::hex<u64, 11>(bundle.GetSlotB())
        << " C=" << fmt::hex<u64, 11>(bundle.GetSlotC())
        << " tmplt=" << static_cast<u32>(bundle.GetTemplate())
        << '(' << Bundle::bundle_template_2_str(bundle.GetTemplate()) << ')';
}

} // namespace postrisc
