#include "Bundle.hpp"
#include "isa.hpp"

namespace postrisc {

/***************************************************************************
*
* assembler instructions names (and other opcode names)
*
***************************************************************************/
#define SPECIAL_REGISTER_SUBSET_X(value, name, type, regtype, descr) \
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

const std::array<InstructionInfo, insn_reserved> asm_info =
{{
#define X(name, subset, format, descr) { INSN_TMPLT(name), instr_subset_##subset, asm_fmt_##format, inst_fmt_##format },
#include "arch/insn_table.hpp"
#undef X
#define NAME_X(name) { 0, instr_subset_group, asm_fmt_NoArgs, inst_fmt_NoArgs },
    DECLARE_OPCODES_WITH_EXTENDED_CODES(NAME_X)
#undef NAME_X
    { 0, instr_subset_base, asm_fmt_NoArgs, inst_fmt_NoArgs }, // illegal
}};

const std::array<char [40], insn_reserved> asm_name =
{
#define X(name, subset, format, descr) #name,
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


EInstructionID GetNullificationInstructionForBranch(EInstructionID insn)
{
#define CASE_X(NAME, OP_NAME) \
    case insn_br_ ## NAME: return insn_nul_ ## NAME; \
    case insn_br_ ## OP_NAME: return insn_nul_ ## OP_NAME; \

#define BRANCH_INSN_SCALAR_COMMON(X, STYPE, UTYPE) \
    X(eq_ ## STYPE, ne_ ## STYPE) \
    X(lt_ ## STYPE, ge_ ## STYPE) \
    X(lt_ ## UTYPE, ge_ ## UTYPE) \
    X(eq_imm_ ## STYPE, ne_imm_ ## STYPE) \
    X(lt_imm_ ## STYPE, ge_imm_ ## STYPE) \
    X(lt_imm_ ## UTYPE, ge_imm_ ## UTYPE) \

#define BRANCH_INSN_FPU_SCALAR_BRANCH(X, TYPE) \
    X(oeq_ ## TYPE, une_ ## TYPE) \
    X(one_ ## TYPE, ueq_ ## TYPE) \
    X(olt_ ## TYPE, uge_ ## TYPE) \
    X(oge_ ## TYPE, ult_ ## TYPE) \
    X(o_   ## TYPE, u_   ## TYPE) \

    switch (insn) {
        BRANCH_INSN_SCALAR_COMMON(CASE_X, i32,  u32)
        BRANCH_INSN_SCALAR_COMMON(CASE_X, i64,  u64)
        BRANCH_INSN_SCALAR_COMMON(CASE_X, i128, u128)
        CASE_X(bc, bs)
        CASE_X(bc_imm, bs_imm)
        CASE_X(mask_all, mask_notall)
        CASE_X(mask_any, mask_none)
        BRANCH_INSN_FPU_SCALAR_BRANCH(CASE_X, f32)
        BRANCH_INSN_FPU_SCALAR_BRANCH(CASE_X, f64)
        BRANCH_INSN_FPU_SCALAR_BRANCH(CASE_X, f128)
    default:
        std::cerr << "Can't find nullification for branch instruction: " << insn_name(insn) << std::endl;
        assert(!"not branch instruction");
        return insn_undef;
    }
#undef CASE_X
}

EInstructionID GetOppositeInstruction(EInstructionID insn)
{
#define CASE_X(NAME, OP_NAME) \
    case insn_br_  ## NAME:    return insn_br_  ## OP_NAME; \
    case insn_nul_ ## NAME:    return insn_nul_ ## OP_NAME; \
    case insn_br_  ## OP_NAME: return insn_br_  ## NAME; \
    case insn_nul_ ## OP_NAME: return insn_nul_ ## NAME; \

    switch (insn) {
        BRANCH_INSN_SCALAR_COMMON(CASE_X, i32,  u32)
        BRANCH_INSN_SCALAR_COMMON(CASE_X, i64,  u64)
        BRANCH_INSN_SCALAR_COMMON(CASE_X, i128, u128)
        CASE_X(bc, bs)
        CASE_X(bc_imm, bs_imm)
        CASE_X(mask_all, mask_notall)
        CASE_X(mask_any, mask_none)
        BRANCH_INSN_FPU_SCALAR_BRANCH(CASE_X, f32)
        BRANCH_INSN_FPU_SCALAR_BRANCH(CASE_X, f64)
        BRANCH_INSN_FPU_SCALAR_BRANCH(CASE_X, f128)
    default:
        std::cerr << "Can't find opposite for: " << insn_name(insn) << std::endl;
        assert(!"not branch or nullification instruction");
        return insn_undef;
#undef CASE_X
    }
}

Bundle::Bundle(const u64 slot[slots_per_bundle], EBundleTemplate tmplt)
{
    const u64 mask = util::makemask<u64>(bits_per_slot);

    const u64 s0 = slot[0] & mask;
    const u64 s1 = slot[1] & mask;
    const u64 s2 = slot[2] & mask;

    lo = static_cast<u64>(tmplt) |
         (s0 << bits_per_bundle_template) |
         (s1 << (bits_per_bundle_template + bits_per_slot));

    hi = (s1 >> (64 - bits_per_slot - bits_per_bundle_template)) |
         (s2 << (64 - bits_per_slot));
}

char const *Bundle::bundle_template_2_str(EBundleTemplate tmplt)
{
    switch (tmplt) {
        case EBundleTemplate::bundle_sss: return "sss";
        case EBundleTemplate::bundle_sll: return "sll";
        case EBundleTemplate::bundle_lls: return "lls";
        case EBundleTemplate::bundle_lll: return "lll";
    }
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
        << fmt::hex(bundle.hi) << '_'
        << fmt::hex(bundle.lo)
        << " A=" << fmt::hex<u64, 11>(bundle.GetSlotA())
        << " B=" << fmt::hex<u64, 11>(bundle.GetSlotB())
        << " C=" << fmt::hex<u64, 11>(bundle.GetSlotC())
        << " tmplt=" << (unsigned)bundle.GetTemplate()
        << '(' << Bundle::bundle_template_2_str(bundle.GetTemplate()) << ')';
}

} // namespace postrisc
