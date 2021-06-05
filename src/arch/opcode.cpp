#include "util/common.hpp"
#include "opcode.hpp"
#include "isa.hpp"

namespace postrisc {

/***************************************************************************
* assembler mnemonics for machine instructions
* initialize readonly tables for opcode ==> instruction_id conversion
***************************************************************************/
#define INSN_X(NAME, VALUE, SUBSET, FORMAT, DESCR) \
   assert(arr[opcode_ ## NAME] == insn_illegal && #NAME); \
   arr[opcode_ ## NAME] = insn_ ## NAME;

#define DEFINE_OPCODE_TABLE(NAME, NBITS, XMACRO)                      \
    const TInstructionMnemonicTable<NBITS> Encoding::NAME =           \
    []() constexpr -> auto {                                          \
        TInstructionMnemonicTable<NBITS> arr = {};                    \
        for (auto& insn : arr) {                                      \
            insn = insn_illegal;                                      \
        }                                                             \
        XMACRO(INSN_X)                                                \
        return arr;                                                   \
    } ();

DEFINE_OPCODE_TABLE(opx_opcode,  bits_per_opcode,       DECLARE_PRIMARY_OPCODES)
DEFINE_OPCODE_TABLE(opx_misc,    bits_per_misc_opx,     DECLARE_MISC_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_fused,   bits_per_fused_opx,    DECLARE_FUSED_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_raopx,   bits_per_register,     DECLARE_RAOPX_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_loop,    bits_per_branch_opx,   DECLARE_LOOP_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_brh,     bits_per_branch_opx,   DECLARE_BRH_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_brw,     bits_per_branch_opx,   DECLARE_BRW_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_brd,     bits_per_branch_opx,   DECLARE_BRD_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_brq,     bits_per_branch_opx,   DECLARE_BRQ_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_nulh,    bits_per_branch_opx,   DECLARE_NULH_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_nulw,    bits_per_branch_opx,   DECLARE_NULW_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_nuld,    bits_per_branch_opx,   DECLARE_NULD_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_nulq,    bits_per_branch_opx,   DECLARE_NULQ_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_memxsd,  bits_per_indexed_opx,  DECLARE_MEMXSD_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_memxud,  bits_per_indexed_opx,  DECLARE_MEMXUD_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_memxsw,  bits_per_indexed_opx,  DECLARE_MEMXSW_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_memxuw,  bits_per_indexed_opx,  DECLARE_MEMXUW_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_fmah,    bits_per_indexed_opx,  DECLARE_FMAH_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_fmas,    bits_per_indexed_opx,  DECLARE_FMAS_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_fmad,    bits_per_indexed_opx,  DECLARE_FMAD_EXTENDED_OPCODES)
DEFINE_OPCODE_TABLE(opx_fmaq,    bits_per_indexed_opx,  DECLARE_FMAQ_EXTENDED_OPCODES)

#undef INSN_X
#undef DEFINE_OPCODE_TABLE

/***************************************************************************
* general registers searchable tables
***************************************************************************/
#define X_NAME(value, name, type, regclass, descr) { reg_ ## name, #name },
static const constexpr auto s_SpecialRegs = createSortedNameTable(
    []() { return std::to_array<NameInfo<ESpecialRegister>>({ DECLARE_SPECIAL_REGISTERS(X_NAME) }); },
    [](size_t N) -> size_t { return std::max<size_t>(N, 1 << bits_per_register); },
    []() { return std::string_view("?"); }
    );
#undef X_NAME

template<> const std::string_view EnumHelper<ESpecialRegister>::name(ESpecialRegister id) {
    return s_SpecialRegs.name(id);
}
template<> std::optional<ESpecialRegister> EnumHelper<ESpecialRegister>::parse(const std::string_view name) {
    return s_SpecialRegs.parse(name);
}
template<> const std::span<const ESpecialRegister> EnumHelper<ESpecialRegister>::sorted_range(void) {
    return s_SpecialRegs.sorted_range();
}
template<> std::string EnumHelper<ESpecialRegister>::regex(void) {
    return s_SpecialRegs.regex();
}

/***************************************************************************
* special registers searchable tables
***************************************************************************/
#define X_STACKED_NAME(NUM) { reg_r ## NUM, "r" #NUM },
#define X_STATIC_NAME(NAME) { reg_ ## NAME, #NAME },
static const constexpr auto s_GeneralRegs = createSortedNameTable(
    []() { return std::to_array<NameInfo<EGeneralRegister>>({ STACKED_REGS(X_STACKED_NAME) STATIC_REGS(X_STATIC_NAME) }); },
    [](size_t N) -> size_t { return std::max<size_t>(N, 1 << bits_per_register); },
    []() { return std::string_view("?"); }
    );
#undef X_STACKED_NAME
#undef X_STATIC_NAME

template<> const std::string_view EnumHelper<EGeneralRegister>::name(EGeneralRegister id) {
    return s_GeneralRegs.name(id);
}
template<> std::optional<EGeneralRegister> EnumHelper<EGeneralRegister>::parse(const std::string_view name) {
    return s_GeneralRegs.parse(name);
}
template<> const std::span<const EGeneralRegister> EnumHelper<EGeneralRegister>::sorted_range(void) {
    return s_GeneralRegs.sorted_range();
}
template<> std::string EnumHelper<EGeneralRegister>::regex(void) {
    return s_GeneralRegs.regex();
}

/***************************************************************************
* instructions/pseudoinstructions/groups mnemonics searchable tables
***************************************************************************/
#define X_NAME(name, code, subset, format, descr) { insn_##name, #name },
static const constexpr auto s_Encodings = createSortedNameTable(
    []() { return std::to_array<NameInfo<EInstructionID>>({ DECLARE_INSN_TABLE(X_NAME) { insn_illegal, "illegal" } }); },
    [](size_t N) -> size_t { return N; },
    []() { return std::string_view("?"); }
    );
#undef X_NAME

template<> const std::string_view EnumHelper<EInstructionID>::name(EInstructionID id) {
    return s_Encodings.name(id);
}
template<> std::optional<EInstructionID> EnumHelper<EInstructionID>::parse(const std::string_view name) {
    return s_Encodings.parse(name);
}
template<> const std::span<const EInstructionID> EnumHelper<EInstructionID>::sorted_range(void) {
    return s_Encodings.sorted_range();
}
template<> std::string EnumHelper<EInstructionID>::regex(void) {
    return s_Encodings.regex();
}

/***************************************************************************
*
***************************************************************************/
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

const constexpr std::array<InstructionInfo, INSTRUCTION_LIST_SIZE> asm_info =
{{
#define X(name, code, subset, format, descr) { INSN_TMPLT(name), instr_subset_##subset, asm_fmt_##format, inst_fmt_##format },
    DECLARE_INSN_TABLE(X)
#undef X
    { 0, instr_subset_base, asm_fmt_noargs, inst_fmt_noargs }, // illegal
}};

const std::array<std::array<EInstructionField, 16>, asm_fmt_last> asm_fmt_field =
{{
#define ASM_FORMAT_NAME_X(name, fields) fields,
    DECLARE_ASM_FORMATS(ASM_FORMAT_NAME_X)
}};

std::ostream& operator<<(std::ostream& out, EGeneralRegister id)
{
    return out << s_GeneralRegs.name(id);
}

std::ostream& operator<<(std::ostream& out, ESpecialRegister id)
{
    return out << s_SpecialRegs.name(id);
}

const std::string_view to_string(EInstructionID insn) {
    return s_Encodings.name(insn);
}

std::ostream& operator<<(std::ostream& out, EInstructionID id)
{
    return out << s_Encodings.name(id);
}

} // namespace postrisc
