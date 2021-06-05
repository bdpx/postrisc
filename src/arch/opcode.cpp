#include "util/common.hpp"
#include "opcode.hpp"
#include "isa.hpp"

namespace postrisc {

const char *RoundingMode2Str(RoundingMode rm)
{
    switch (rm) {
        case RoundingMode::RNE: return "rne";
        case RoundingMode::RDN: return "rdn";
        case RoundingMode::RUP: return "rup";
        case RoundingMode::RTZ: return "rtz";
        case RoundingMode::RMM: return "rmm";
        case RoundingMode::RNO: return "rno";
        case RoundingMode::RTO: return "rto";
        case RoundingMode::DYN: return "dyn";
    }
    assert(!"unknown rounding mode encoding");
    return nullptr;
}

const char *MemoryOrdering2Str(MemoryOrdering ordering)
{
    switch (ordering) {
        case MemoryOrdering::RELAXED: return "relaxed";
        case MemoryOrdering::ACQUIRE: return "acquire";
        case MemoryOrdering::RELEASE: return "release";
        case MemoryOrdering::ACQ_REL: return "acq_rel";
        case MemoryOrdering::SEQ_CST: return "seq_cst";
    }
    assert(!"unknown memory ordering");
    return nullptr;
}

std::ostream& operator<<(std::ostream& out, RoundingMode value)
{
    return out << static_cast<u32>(value);
}

std::ostream& operator<<(std::ostream& out, MemoryOrdering value)
{
    return out << static_cast<u32>(value);
}

std::ostream& operator<<(std::ostream& out, FClassMask classMask)
{
    out << static_cast<u32>(classMask);
    if ((classMask & FClassMask::SignalingNaN)      != FClassMask::Empty)  out << " @SNAN";
    if ((classMask & FClassMask::QuietNaN)          != FClassMask::Empty)  out << " @QNAN";
    if ((classMask & FClassMask::NegativeInfinity)  != FClassMask::Empty)  out << " @NINF";
    if ((classMask & FClassMask::NegativeNormal)    != FClassMask::Empty)  out << " @NNORM";
    if ((classMask & FClassMask::NegativeSubnormal) != FClassMask::Empty)  out << " @NSUB";
    if ((classMask & FClassMask::NegativeZero)      != FClassMask::Empty)  out << " @NZERO";
    if ((classMask & FClassMask::PositiveInfinity)  != FClassMask::Empty)  out << " @PINF";
    if ((classMask & FClassMask::PositiveNormal)    != FClassMask::Empty)  out << " @PNORM";
    if ((classMask & FClassMask::PositiveSubnormal) != FClassMask::Empty)  out << " @PSUB";
    if ((classMask & FClassMask::PositiveZero)      != FClassMask::Empty)  out << " @PZERO";
    return out;
}


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

const std::array<InstructionInfo, INSTRUCTION_LIST_SIZE> asm_info =
{{
#define X(name, code, subset, format, descr) { INSN_TMPLT(name), instr_subset_##subset, asm_fmt_##format, inst_fmt_##format },
    DECLARE_INSN_TABLE(X)
#undef X
    { 0, instr_subset_base, asm_fmt_noargs, inst_fmt_noargs }, // illegal
}};

const std::array<char [16], INSTRUCTION_LIST_SIZE> asm_name =
{
#define X(name, code, subset, format, descr) #name,
    DECLARE_INSN_TABLE(X)
#undef X
    "illegal"
};

const std::array<std::array<EInstructionField, 16>, asm_fmt_last> asm_fmt_field =
{{
#define ASM_FORMAT_NAME_X(name, fields) fields,
    DECLARE_ASM_FORMATS(ASM_FORMAT_NAME_X)
}};

const char *to_string(EInstructionID insn) {
    return asm_name[insn];
}

std::ostream& operator<<(std::ostream& out, EInstructionID eid)
{
    return out << to_string(eid);
}

} // namespace postrisc
