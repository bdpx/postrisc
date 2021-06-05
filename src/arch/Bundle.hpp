#pragma once

#include "InstructionSingle.hpp"
#include "InstructionDouble.hpp"

/***************************************************************************
* bundle of instructions
***************************************************************************/

namespace postrisc {

enum class EBundleTemplate {
    bundle_sss = 0, // short, short, short
    bundle_sll = 1, // short, then long
    bundle_lls = 2, // long, then short
    bundle_lll = 3  // reserved
};

class Bundle {
public:
    explicit Bundle() : lo(0), hi(0) {}
    explicit Bundle(const u64 slot[slots_per_bundle], EBundleTemplate tmplt);

    u64 get_lo(void) const { return lo; }
    u64 get_hi(void) const { return hi; }
    u128 get_u128(void) const { return ((u128)hi << 64) | lo; }

    EBundleTemplate GetTemplate(void) const {
        return static_cast<EBundleTemplate>(lo & util::makemask<u64>(bits_per_bundle_template));
    }

    const InstructionSingle s0(void) const { return InstructionSingle(GetSlotA()); }
    const InstructionSingle s1(void) const { return InstructionSingle(GetSlotB()); }
    const InstructionSingle s2(void) const { return InstructionSingle(GetSlotC()); }

    const InstructionDouble l0(void) const { return InstructionDouble(GetSlotA(), GetSlotB()); }
    const InstructionDouble l1(void) const { return InstructionDouble(GetSlotB(), GetSlotC()); }

    u128 v128(void) const
    {
        return (u128(hi) << 64) | lo;
    }

    friend std::ostream& operator<<(std::ostream& out, const Bundle& bundle);

    template<typename Archive> void serialize(Archive& ar, const unsigned int UNUSED(version))
    {
        ar & BOOST_SERIALIZATION_NVP(lo);
        ar & BOOST_SERIALIZATION_NVP(hi);
    }
    static char const *bundle_template_2_str(EBundleTemplate tmplt);

private:
    u64 GetSlotA(void) const {
        return  (lo >> bits_per_bundle_template) & util::makemask<u64>(bits_per_slot);
    }
    u64 GetSlotB(void) const {
        return ((lo >> (bits_per_bundle_template + bits_per_slot)) |
                (hi << (64 - bits_per_slot - bits_per_bundle_template)))
               & util::makemask<u64>(bits_per_slot);
    }
    u64 GetSlotC(void) const {
        return  (hi >> (64 - bits_per_slot)) & util::makemask<u64>(bits_per_slot);
    }

private:
    u64   lo, hi; // assume little endian order (architecture dependent)
};

static_assert(sizeof(Bundle) == 16U);

} // namespace postrisc
