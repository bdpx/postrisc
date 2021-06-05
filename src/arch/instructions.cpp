#include "instructions.hpp"
#include "isa.hpp"
#include "fpu/float.hpp"

namespace postrisc {

/***************************************************************************
*
* assembler instructions names (and other opcode names)
*
***************************************************************************/
const std::array<ESpecialRegisterSubset, (1 << bits_per_register)> arch_spr_regtype =
{
#define SPECIAL_REGISTER_SUBSET_X(name, regtype, descr) ESpecialRegisterSubset::regtype,
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_SUBSET_X)
};

const std::array<CInstructionInfo, insn_reserved> asm_info =
{{
#define X(name, subset, inst_format, format, iic, tmplt, descr) \
  { INSN_TMPLT(tmplt), \
    instr_subset_##subset, \
    (insn_##name == insn_##tmplt) ? instr_htype_hw : instr_htype_sw, \
    asm_fmt_##format, \
    inst_fmt_##inst_format \
  },

# include "arch/insn_table.hpp"
#undef X
}};

const std::array<char [12], insn_reserved> asm_name =
{
#define X(name, subset, inst_format, format, iic, tmplt, descr) #name,
# include "arch/insn_table.hpp"
#undef X
    "last",      // insn_last
#define NAME_X(name) #name,
    DECLARE_OPCODES_WITH_EXTENDED_CODES(NAME_X)
};

const std::array<std::array<EInstructionField, 16>, asm_fmt_last> asm_fmt_field =
{{
#define ASM_FORMAT_NAME_X(name, fields) fields,
    DECLARE_ASM_FORMATS(ASM_FORMAT_NAME_X)
}};

std::ostream &
operator<< (std::ostream& out, const CInsn1 cmd)
{
    return out << std::setfill('0')
               << std::hex
               << std::setw(nibbles_per_slot)
               << cmd.GetRaw()
               << std::dec;
}

std::ostream &
operator<< (std::ostream& out, const CInsn2 cmd)
{
    return out << std::setfill('0')
               << std::hex
               << std::setw(nibbles_per_slot) << cmd.GetRaw()
               << ' '
               << std::setw(nibbles_per_slot) << cmd.GetRawExtension()
               << std::dec;
}

CBundle::CBundle(const uint64_t slot[slots_per_bundle], EBundleTemplate tmplt)
{
    const uint64_t mask = util::makemask(bits_per_slot);

    const uint64_t s0 = slot[0] & mask;
    const uint64_t s1 = slot[1] & mask;
    const uint64_t s2 = slot[2] & mask;

    lo = static_cast<uint64_t>(tmplt) |
         (s0 << bits_per_bundle_template) |
         (s1 << (bits_per_bundle_template + bits_per_slot));

    hi = (s1 >> (64 - bits_per_slot - bits_per_bundle_template)) |
         (s2 << (64 - bits_per_slot));
}

std::ostream &
operator<< (std::ostream & out, const CBundle & bundle)
{
     /*
     out << ";oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
            "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
            "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee"
            "tt\n;" << std::bitset<64>(a0) << std::bitset<64>(a1) << '\n';
      */
    return out << fmt::hex(bundle.hi)
               << '_'
               << fmt::hex(bundle.lo);
}

} // namespace postrisc
