#include "instructions.hpp"
#include "isa.hpp"

namespace postrisc {

/***************************************************************************
*
* assembler instructions names (and other opcode names)
*
***************************************************************************/
#define SPECIAL_REGISTER_SUBSET_X(value, name, regtype, descr) \
    arr[ESpecialRegister::reg_##name] = ESpecialRegisterSubset::regtype;

constexpr const std::array<ESpecialRegisterSubset, (1 << bits_per_register)> arch_spr_regtype =
    []() constexpr -> auto {
        std::array<ESpecialRegisterSubset, (1 << bits_per_register)> arr = {};
        for (size_t i = 0; i < (1 << bits_per_register); i++) {
            arr[i] = ESpecialRegisterSubset::reserved;
        }
        DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_SUBSET_X)
        return arr;
    } ();

const std::array<CInstructionInfo, insn_reserved> asm_info =
{{
#define X(mnem, name, subset, format, descr) \
  { INSN_TMPLT(name), \
    instr_subset_##subset, \
    asm_fmt_##format, \
    inst_fmt_##format \
  },
#include "arch/insn_table.hpp"
#undef X
#define NAME_X(name) { 0, instr_subset_group, asm_fmt_NoArgs, inst_fmt_NoArgs },
    DECLARE_OPCODES_WITH_EXTENDED_CODES(NAME_X)
#undef NAME_X
    { 0, instr_subset_base, asm_fmt_NoArgs, inst_fmt_NoArgs }, // illegal
}};

const std::array<char [40], insn_reserved> asm_name =
{
#define X(mnem, name, subset, format, descr) mnem,
#include "arch/insn_table.hpp"
#undef X
#define NAME_X(name) #name,
    DECLARE_OPCODES_WITH_EXTENDED_CODES(NAME_X)
#undef NAME_X
    "illegal"
};

const std::array<std::array<EInstructionField, 16>, asm_fmt_last> asm_fmt_field =
{{
#define ASM_FORMAT_NAME_X(name, fields) fields,
    DECLARE_ASM_FORMATS(ASM_FORMAT_NAME_X)
}};

std::ostream& operator<<(std::ostream& out, const CInsn1 cmd)
{
    return out << fmt::hex<u64, nibbles_per_slot>(cmd.GetRaw());
}

std::ostream& operator<<(std::ostream& out, const CInsn2 cmd)
{
    return out << fmt::hex<u64, nibbles_per_slot>(cmd.GetRaw())
               << ' '
               << fmt::hex<u64, nibbles_per_slot>(cmd.GetRawExtension());
}

CBundle::CBundle(const u64 slot[slots_per_bundle], EBundleTemplate tmplt)
{
    const u64 mask = util::makemask(bits_per_slot);

    const u64 s0 = slot[0] & mask;
    const u64 s1 = slot[1] & mask;
    const u64 s2 = slot[2] & mask;

    lo = static_cast<u64>(tmplt) |
         (s0 << bits_per_bundle_template) |
         (s1 << (bits_per_bundle_template + bits_per_slot));

    hi = (s1 >> (64 - bits_per_slot - bits_per_bundle_template)) |
         (s2 << (64 - bits_per_slot));
}

char const *CBundle::bundle_template_2_str(EBundleTemplate tmplt)
{
    switch (tmplt) {
        case EBundleTemplate::bundle_sss: return "sss";
        case EBundleTemplate::bundle_sll: return "sll";
        case EBundleTemplate::bundle_lls: return "lls";
        case EBundleTemplate::bundle_lll: return "lll";
    }
    return nullptr;
}

std::ostream& operator<<(std::ostream& out, const CBundle& bundle)
{
    /*
    out << ";"
           "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
           "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
           "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
           "tt\n;" << std::bitset<64>(a0) << std::bitset<64>(a1) << '\n';
    */
    return out
        << fmt::hex(bundle.hi) << '_'
        << fmt::hex(bundle.lo)
        << " A=" << fmt::hex<u64, 11>(bundle.GetSlotA())
        << " B=" << fmt::hex<u64, 11>(bundle.GetSlotB())
        << " C=" << fmt::hex<u64, 11>(bundle.GetSlotC())
        << " tmplt=" << (unsigned)bundle.GetTemplate()
        << '(' << CBundle::bundle_template_2_str(bundle.GetTemplate()) << ')';
}

} // namespace postrisc
