#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "html_generator.hpp"
#include "binary_format/file_header.hpp"
#include "assembler.hpp"

/***********************************************************************
* virtual processor assembler
***********************************************************************/

namespace postrisc {

CAssembler::CAssembler(const Decoder& decoder)
    : m_BundleTemplate(EBundleTemplate::bundle_sss)
    , m_CurrentSlotNum(0)
    , m_NopNumber(0)
    , m_InstructionLength(cmd_short)
    , m_InstructionNumber(0)
    , m_PassNumber(EAssemblerPass::compile)
    , m_decoder(decoder)
{
}

CAssembler::~CAssembler(void)
{
}

void
CAssembler::RegisterDefine(const char *name, uint64_t value)
{
    m_defines[name] = m_define_values.size();
    m_define_values.push_back(value);
}

void
CAssembler::ExportDefinitions(std::ostream& out)
{
    for (const auto& x : m_defines) {
        out << x.first << "    0x" << std::hex << m_define_values[x.second] << fmt::endl;
    }
}

uint64_t
CAssembler::make_nop()
{
    //unsigned const linenum = GetLineNum() & util::makemask(bits_per_branch);
    CInsn1 cmd(INSN_TMPLT(nop));
    //cmd.set_ldi(linenum);
    return cmd.GetRaw();
}

template<class UNSIGNED>
void
CAssembler::EvaluateUnsigned(CProgramSection & section)
{
    if (section.size() & (sizeof(UNSIGNED)-1)) {
        REPORT_ERROR(this) << "unaligned " << (bits_per_byte * sizeof(UNSIGNED)) << "-bit unsigned data";
    }

    UNSIGNED imm = static_cast<UNSIGNED>(EvaluateInt(bits_per_byte * sizeof(UNSIGNED), eval_unsigned, 0));
    section.write(imm);
}

template<class FLOAT, class UNSIGNED>
void
CAssembler::EvaluateFloat(CProgramSection & section)
{
    if (section.size() & (sizeof(UNSIGNED)-1)) {
        REPORT_ERROR(this) << "unaligned " << (bits_per_byte * sizeof(FLOAT)) << "-bit floating point data";
    }

    const FLOAT f = static_cast<FLOAT>(EvaluateFloat());
    const UNSIGNED u = util::change_bit_presentation<FLOAT, UNSIGNED>(f);
    section.write(u);
}

/****************************************************************************
* assembler data mnemonics
****************************************************************************/
#define DECLARE_DATA_MNEM(X) \
    X(align) X(ascii) X(asciiz) \
    X(d1) X(d2) X(d4) X(d8) \
    X(double) \
    X(extend) \
    X(float) \
    X(quad) \
    X(space) \
    X(string) \
    X(u1) X(u2) X(u4) X(u8) \

enum {
#define DATA_ENUM_X(name) asm_##name,
    DECLARE_DATA_MNEM(DATA_ENUM_X)
    asm_data_last
};

static const keywordmap asm_data = {
#define DATA_NAME_X(name) { #name, asm_##name },
    DECLARE_DATA_MNEM(DATA_NAME_X)
};


/****************************************************************************
* asembler directives
****************************************************************************/
#define DECLARE_ASM_DIRECTIVE(X) \
    X (data,    "open data section") \
    X (def,     "define symbol") \
    X (end,     "end program") \
    X (file,    "set source file name") \
    X (global,  "export global name") \
    X (import,  "import global name") \
    X (rodata,  "open rodata section") \
    X (text,    "open text section") \

enum {
#define DIRECTIVE_ENUM_X(name, descr) asm_dir_##name,
  DECLARE_ASM_DIRECTIVE(DIRECTIVE_ENUM_X)
  asm_dir_last
};

const keywordmap CAssembler::asm_directives = {{
#define DIRECTIVE_NAME_X(name, descr) { #name, asm_dir_##name },
  DECLARE_ASM_DIRECTIVE(DIRECTIVE_NAME_X)
}};

int64_t
CAssembler::ComputeBundledOffset(int64_t offset) const
{
    if (offset & util::makemask(log_bytes_per_bundle)) {
        REPORT_ERROR(this) << "target label is not 16-byte aligned: " << fmt::hex(offset);
    }
    return offset >> log_bytes_per_bundle;
}

/***************************************************************************
* this function find register name in stream
***************************************************************************/
uint32_t
CAssembler::GetRegister(const keywordmap & regs)
{
    const CParserToken token = GetToken();

    const ETokenKind ttype = token.GetType();
    const uint32_t tdata = token.GetData();

    if (ttype != token_identifier) {
        REPORT_ERROR(this) << "register name expected, token = (" << ttype << "," << tdata << ")";
    }

    const std::string & cmd = GetIdentifierInfo(tdata).GetName();

    const auto it = regs.find(cmd);
    if (it == regs.end()) {
        REPORT_ERROR(this) << "invalid register: " << cmd;
        return -1;
    }

    LOG_DEBUG(LOG_PARSER) << "regs: " << it->second << "cmd: " << cmd;
    return it->second;
}

uint32_t
CAssembler::GetGeneralRegister(void)
{
    return GetRegister(m_decoder.m_GeneralRegisters);
}

uint32_t
CAssembler::GetSpecialRegister(void)
{
    return GetRegister(m_decoder.m_SpecialRegisters);
}

/***************************************************************************
* this function scan token stream for identifier and return id number
***************************************************************************/
uint32_t
CAssembler::GetIdentifier(void)
{
    CParserToken  token = GetToken();

    if (token.GetType() != token_identifier) {
       LOG_ERROR(LOG_PARSER) << "GetType()=" << fmt::dec(token.GetType());
       REPORT_ERROR(this) << "identifier expected";
    }

    return (token.GetData());
}

void
CAssembler::StartNewBundle()
{
    m_BundleTemplate = EBundleTemplate::bundle_sss;
    m_CurrentSlotNum = 0;
}

void
CAssembler::EmitInstruction(CProgramSection & section, uint64_t cmd)
{
    LOG_DEBUG(LOG_PARSER)
         << "pos=" << section.size()
         << " slotnum=" << m_CurrentSlotNum
         << " slot=" << std::bitset<bits_per_slot>(cmd);

    m_Slots[m_CurrentSlotNum++] = cmd;

    m_InstructionNumber++;
    LOG_DEBUG(LOG_PARSER) << "instruction_number=" << fmt::dec(m_InstructionNumber);

    if (cmd == make_nop())
       m_NopNumber++;

    if (m_CurrentSlotNum < slots_per_bundle)
       return;

    /*************************************************************************
    * form bundle from last 3 slots and template
    *************************************************************************/

    CBundle  bundle(m_Slots, m_BundleTemplate);

    LOG_DEBUG(LOG_PARSER) << "bundle=" << bundle;

    section.write(bundle);

    StartNewBundle();
}

void
CAssembler::EmitInstruction(CProgramSection & section, uint64_t cmd, uint64_t ext)
{
    LOG_DEBUG(LOG_PARSER)
        << "cmd=" << std::bitset<bits_per_slot>(cmd)
        << "ext=" << std::bitset<bits_per_slot>(ext);

    switch (m_CurrentSlotNum) {
        case 2:
            LOG_DEBUG(LOG_PARSER) << "insert nop";
            // we have not free space in current bundle, so insert nop and start new bundle
            EmitInstruction(section, make_nop());
            //m_NopNumber++;
            // emit in new bundle
            // fallthrough

        case 0:
            m_BundleTemplate = EBundleTemplate::bundle_lls;
            m_Slots[m_CurrentSlotNum++] = cmd;
            //EmitInstruction(cmd);
            EmitInstruction(section, ext);
            break;

        case 1:
            m_BundleTemplate = EBundleTemplate::bundle_sll;
            m_Slots[m_CurrentSlotNum++] = cmd;
            //EmitInstruction(cmd);
            EmitInstruction(section, ext);
            break;

        default:
            LOG_ABORT << "invalid slot num " << m_CurrentSlotNum;
    }
}


/**********************************************************************
* this function fill last (possible not full) bundle with nops
**********************************************************************/
void
CAssembler::EmitLabel(CProgramSection & section, uint64_t cmd)
{
    switch (m_CurrentSlotNum) {
        case 0:
            break;

        case 1:
            EmitInstruction(section, cmd);
            EmitInstruction(section, cmd);
            m_NopNumber += 2;
            break;

        case 2:
            EmitInstruction(section, cmd);
            m_NopNumber++;
            break;

        default:
            LOG_ABORT << "invalid slot num " << m_CurrentSlotNum;
    }
}

/********************************************************************************
* this function scan token stream for identifier.
* she do nothing else at first compiler pass, but find label at second pass.
* it return zero (first pass) or branch displacement (second pass).
********************************************************************************/
int64_t
CAssembler::GetLabel(int64_t ip, ESectionKind requiredSectionType)
{
    const uint32_t ident = GetIdentifier();
    const std::string & cmd = GetIdentifierInfo(ident).GetName();

    if (m_PassNumber < EAssemblerPass::link) { // first compiler pass
       return 0;
    }

    // second and later compiler passes
    CAssemblerIdentifier & idInfo = GetIdentifierInfo(ident);
    if (!idInfo.IsValidSection()) {
       REPORT_ERROR(this) << "unknown label " <<  cmd;
    }

    const uint32_t sectionId = idInfo.GetSectionId();
    LOG_DEBUG(LOG_PARSER) << "old label: sectionId=" << sectionId << ", " << cmd;

    
    if ((m_Sections[sectionId].GetSectionType() & requiredSectionType) == 0) {
        REPORT_ERROR(this) << "can't use label '" << cmd << "' here";
    }

    int64_t imm = GetIdentifierInfo(ident).data;
    imm = m_Sections[sectionId].ComputeIpRelative(ip, imm);

    if (GetInstructionLength() == cmd_long && m_CurrentSlotNum == 2) {
        LOG_DEBUG(LOG_PARSER) << "long instruction doesn't fit in current bundle, nop will be inserted, ip updated";
        imm -= sizeof(CBundle);
    }

    return imm;
}


/********************************************************************************
* this function build labels table at first compiler pass,
* and do nothing at second pass.
* label is defined as: <pair><identifier/><colon/></pair>
********************************************************************************/
bool
CAssembler::ScanLabel(CProgramSection & section)
{
    const CParserToken token_ident = GetCurrentToken();

    if (token_ident.GetType() != token_identifier)
       return false;

    if (GetCurrentToken(1) != CParserToken(operator_colon))
       return false;

    PopToken(); // skip identifier
    PopToken(); // skip colon

    // label alignment
    // .label_name: (alignment)
    LOG_TRACE(LOG_PARSER) << GetCurrentToken();
    if (GetCurrentToken() == CParserToken(operator_l_parenthese)) {
        LOG_TRACE(LOG_PARSER) << "( found";
        const uint64_t imm = EvaluateInt(bits_per_byte * sizeof(uint64_t), eval_unsigned, 0);
        LOG_TRACE(LOG_PARSER) << "align=" << imm;
        if (imm == 0 || (imm & (imm - 1)) != 0) {
            REPORT_ERROR(this) << "alignment should be power of 2: " << imm;
        }
        if (section.GetSectionType() == ESectionKind::text) {
            if (imm <= sizeof(CBundle)) {
                REPORT_ERROR(this) << "code alignment is too little: " << imm;
            }
            EmitLabel(section, make_nop());
            while ((section.size() & (imm-1)) != 0) {
                EmitInstruction(section, make_nop());
                EmitInstruction(section, make_nop());
                EmitInstruction(section, make_nop());
            }
        } else {
            section.align(imm);
        }
        LOG_TRACE(LOG_PARSER) << "after align=" << imm;
    }

    const uint32_t ident = token_ident.GetData();

    if (section.GetSectionType() == ESectionKind::text) {
        // check if this label has references 
        // if(GetIdentifierInfo(ident).ref_count > 0 || m_PassNumber == EAssemblerPass::compile)
        EmitLabel(section, make_nop());
    }

    CAssemblerIdentifier & id = GetIdentifierInfo(ident);

    if (m_PassNumber == EAssemblerPass::compile) {
        if (id.IsValidSection()) {
            LOG_DEBUG(LOG_PARSER) << "old label: " << GetIdentifierInfo(ident).GetName();
            REPORT_ERROR(this) << "duplicate labels " << GetIdentifierInfo(ident).GetName();
        }
        const uint32_t offset = static_cast<unsigned>(section.size());
        id.SetSectionInfo(section.GetId(), offset);
        LOG_DEBUG(LOG_PARSER) << "new label: " << GetIdentifierInfo(ident).GetName() << " segpos=" << section.size();
    } else if (m_PassNumber == EAssemblerPass::link) {
       if (id.data != section.size() || id.GetSectionId() != section.GetId()) {
           REPORT_ERROR(this) << "2 pass error label: " << GetIdentifierInfo(ident).GetName() << ' ' << id.data;
       }
    }

    return true;
}

void
CAssembler::CheckLineEnd(void)
{
    // check end of string. it must be clear.
    const CParserToken opc = GetCurrentToken();

    if (opc.GetType() != token_eol) {
        LOG_DEBUG(LOG_PARSER) << opc;
        REPORT_ERROR(this) << "endline expected";
    }
}

unsigned
CAssembler::GetImmediateSize(EInstructionSize size1, EInstructionFullSize size2)
{
    const EInstructionLength length = GetInstructionLength();

    switch (length) {
        case cmd_short:  return static_cast<unsigned>(size1);
        case cmd_long:   return static_cast<unsigned>(size2);
        default:
            REPORT_ERROR(this) << "invalid instruction length";
    }

    return 0;
}

void
CAssembler::SetForwardBranch(uint64_t uimm, CInsn2 & cmd)
{
    const unsigned nbits = GetImmediateSize(bits_per_branch, bits_per_branch_long);

    if (m_PassNumber == EAssemblerPass::link && uimm != util::zext64(uimm, nbits)) {
        REPORT_ERROR(this) << "too long distance for forward branch: " << uimm;
    }

    cmd.SetForwardBranch(uimm);
}

unsigned int
CAssembler::GetShiftAmount(void)
{
    return static_cast<unsigned>(EvaluateInt(bits_per_shamt, eval_unsigned, 0));
}

unsigned int
CAssembler::GetVectorShiftAmount(void)
{
    return static_cast<unsigned>(EvaluateInt(bits_per_register, eval_unsigned, 0));
}

uint32_t
CAssembler::GetExpectedInstructionNumber(void) const
{
    const EInstructionLength length = GetInstructionLength();

    if (length == cmd_short) {
        return m_InstructionNumber;
    }

    // analyze the long instruction future placement
    switch (m_CurrentSlotNum) {
        case 0:
            return m_InstructionNumber;

        case 1:
            return m_InstructionNumber;

        case 2:
            // NOP should be emitted before that instruction
            return m_InstructionNumber + 1;

        default:
            LOG_ABORT << "invalid slot num " << m_CurrentSlotNum;
    }

    return m_InstructionNumber;
}

unsigned int
CAssembler::GetNullificationDistance(unsigned offset)
{
    unsigned distance = 0;
    if (GetCurrentToken().GetType() == token_identifier) {
        // identifier
        CParserToken token_ident = GetCurrentToken();
        PopToken();
        const uint32_t ident = token_ident.GetData();
        CAssemblerIdentifier & id = GetIdentifierInfo(ident);

        if (m_PassNumber == EAssemblerPass::compile) {
            id.data = GetExpectedInstructionNumber();
            LOG_DEBUG(LOG_PREDICATION) << "nullify instruction number = " << id.data;
            distance = 0;
        } else {
            if (id.mask < offset) {
             REPORT_ERROR(this) << "else block distance "  << id.mask
                                << " is less thn if block distance " << offset;
            }
            distance = id.mask - offset;
        }
    } else {
        // immediate
        distance = static_cast<unsigned>(EvaluateInt(bits_per_nullify_dist, eval_unsigned, 0));
    }

    if (distance > util::makemask(bits_per_nullify_dist)) {
        REPORT_ERROR(this) << "predicate block distance " << distance
                           << " is greater than max predication block size "
                           << util::makemask(bits_per_nullify_dist);
    }
    return distance;
}

void
CAssembler::ScanPredicate(void)
{
    const CParserToken token_ident = GetCurrentToken();
    if (token_ident.GetType() != token_identifier) {
        REPORT_ERROR(this) << "predicate name expected after colon";
    }
    PopToken(); // skip identifier

    const uint32_t ident = token_ident.GetData();
    CAssemblerIdentifier & id = GetIdentifierInfo(ident);
    if (m_PassNumber != EAssemblerPass::compile) {
        // gather/check/store predicate mask on first pass only
        return;
    }

    const auto base = id.data;
    const auto current_pos = GetExpectedInstructionNumber();

    LOG_DEBUG(LOG_PREDICATION) << "nullify base=" << base << " pos=" << current_pos;

    if (current_pos < base) {
        REPORT_ERROR(this) << "predicate name used before definition: " << GetIdentifierInfo(ident).GetName();
    }

    id.mask = current_pos - base;
    LOG_DEBUG(LOG_PREDICATION) << "nullify mask=" << id.mask;
}

void
CAssembler::ScanPredicates(void)
{
   if (GetCurrentToken() != CParserToken(operator_l_parenthese)) {
       return;
   }
   PopToken(); // skip '('
   ScanPredicate();

   while (GetCurrentToken() == CParserToken(operator_comma)) {
       PopToken(); // skip comma
       ScanPredicate();
   }
   if (GetCurrentToken() != CParserToken(operator_r_parenthese)) {
       REPORT_ERROR(this) << "predicate list must have parnetheses";
   }
   PopToken(); // skip ')'
}

void
CAssembler::SetInstructionLength(EInstructionLength cmdlen)
{
    m_InstructionLength = cmdlen;
}

EInstructionLength
CAssembler::GetInstructionLength(void) const
{
    return m_InstructionLength;
}

template<class FLOAT>
void
CAssembler::ReadonlyFloatForRelative(int64_t ip, CInsn2 & cmd)
{
    CProgramSection & section = m_Sections[ESectionId_rodata];
    section.align(sizeof(FLOAT));
    uint64_t uimm = section.size();
    assert((uimm & (sizeof(FLOAT)-1)) == 0);
    section.write(static_cast<FLOAT>(EvaluateFloat()));
    uimm = section.ComputeIpRelative(ip, uimm);
    SetForwardBranch(uimm, cmd);
}

/***************************************************************************
* field handlers
***************************************************************************/

FIELD_HANDLER_IMPLEMENTATION(comma)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    if (GetToken() != CParserToken(operator_comma)) {
        REPORT_ERROR(this) << "can't find ',' delimiter";
    }
}

// optional '.l' (long) prefix for explicit double-width instruction emit
FIELD_HANDLER_IMPLEMENTATION(lprefix)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    if (GetCurrentToken() != CParserToken(operator_point)) {
        return;
    }
    PopToken();

    const std::string & cmd_name = GetIdentifierInfo(GetIdentifier()).GetName();
    if (cmd_name == "l") {
        SetInstructionLength(cmd_long);
    } else {
        REPORT_ERROR(this) << "invalid instruction option '" << cmd_name << "'";
    }
}

// general register in (a,b,c,d) positions
FIELD_HANDLER_IMPLEMENTATION(gpra)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(a)=" << fmt::dec(regNum);
    cmd.set_ra(regNum);
}

FIELD_HANDLER_IMPLEMENTATION(gprb)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(b)=" << fmt::dec(regNum);
    cmd.set_rb(regNum);
}

FIELD_HANDLER_IMPLEMENTATION(gprc)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(c)=" << fmt::dec(regNum);
    cmd.set_rc(regNum);
}

FIELD_HANDLER_IMPLEMENTATION(gprd)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(d)=" << fmt::dec(regNum);
    cmd.set_rd(regNum);
}

// special register in c
FIELD_HANDLER_IMPLEMENTATION(spr)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned regNum = GetSpecialRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum=" << fmt::dec(regNum);
    cmd.set_rs(regNum);
}

// 6-bit shift amount in (b,c,d,e) position
FIELD_HANDLER_IMPLEMENTATION(shamt_b)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(b)=" << fmt::dec(shift_amout);
    cmd.set_shamt_b(GetShiftAmount());
}

FIELD_HANDLER_IMPLEMENTATION(shamt_c)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(c)=" << fmt::dec(shift_amout);
    cmd.set_shamt_c(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(shamt_d)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(d)=" << fmt::dec(shift_amout);
    cmd.set_shamt_d(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(shamt_e)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(e)=" << fmt::dec(shift_amout);
    cmd.set_shamt_e(shift_amout);
}

// 7-bit vector shift amount in (b,c,d) position
FIELD_HANDLER_IMPLEMENTATION(vshamt_b)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetVectorShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(b)=" << fmt::dec(shift_amout);
    cmd.set_vshamt_b(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(vshamt_c)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetVectorShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(c)=" << fmt::dec(shift_amout);
    cmd.set_vshamt_c(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(vshamt_d)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetVectorShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(d)=" << fmt::dec(shift_amout);
    cmd.set_vshamt_d(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(vshamt_e)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned shift_amout = GetVectorShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(d)=" << fmt::dec(shift_amout);
    cmd.set_vshamt_e(shift_amout);
}

// fpu/nullification masks
FIELD_HANDLER_IMPLEMENTATION(mask_yes)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const distance = GetNullificationDistance(0);
    LOG_DEBUG(LOG_PARSER) << "true nullify distance: " << distance;
    cmd.set_nullify_yes(distance);
}

FIELD_HANDLER_IMPLEMENTATION(mask_no)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const distance = GetNullificationDistance(cmd.get_nullify_yes());
    LOG_DEBUG(LOG_PARSER) << "false nullify distance: " << distance;
    cmd.set_nullify_not(distance);
}

// 3bit scale for indexed load/store
FIELD_HANDLER_IMPLEMENTATION(indexed_scale)
{
    UNREFERENCED_PARAMETER(ip);
    const uint64_t imm = EvaluateInt(bits_per_indexed_scale, eval_unsigned, 0);
    LOG_DEBUG(LOG_PARSER) << "scale=" << imm;
    cmd.SetIndexedScale(static_cast<unsigned>(imm));
}

// 10 bit displacement for indexed load/store
FIELD_HANDLER_IMPLEMENTATION(indexed_disp)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned nbits = GetImmediateSize(bits_per_indexed_disp, bits_per_indexed_disp_long);
    const int64_t imm = EvaluateInt(nbits, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "indexed disp=" << imm;
    cmd.SetIndexedDisplacement(imm);
}

FIELD_HANDLER_IMPLEMENTATION(loop_step)
{
    UNREFERENCED_PARAMETER(ip);
    int64_t const imm = EvaluateInt(bits_per_loop_step, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "loop_step=" << imm;
    cmd.SetLoopStep(imm);
}

FIELD_HANDLER_IMPLEMENTATION(uimm14)
{
    UNREFERENCED_PARAMETER(ip);
    uint64_t const imm = EvaluateInt(bits_per_update_stride, eval_unsigned, 0);
    LOG_DEBUG(LOG_PARSER) << "imm14=" << imm;
    cmd.SetUintStride(imm);
}

FIELD_HANDLER_IMPLEMENTATION(update_stride)
{
    UNREFERENCED_PARAMETER(ip);
    const int64_t imm = EvaluateInt(bits_per_update_stride, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "stride=" << imm;
    if (imm == 0) {
        REPORT_ERROR(this) << "stride immediate for base update must be nonzero (or use instruction without base update)";
    }
    cmd.SetUpdateStride(imm);
}

FIELD_HANDLER_IMPLEMENTATION(clmul_index)
{
    UNREFERENCED_PARAMETER(ip);
    const uint64_t imm = EvaluateInt(bits_per_clmul_index, eval_unsigned, 0);
    LOG_DEBUG(LOG_PARSER) << "clmul_index=" << imm;
    cmd.SetIndexedScale(imm);
}

FIELD_HANDLER_IMPLEMENTATION(rounding_mode)
{
    UNREFERENCED_PARAMETER(ip);
    const uint64_t imm = EvaluateInt(bits_per_indexed_scale, eval_unsigned, 0);
    if (imm > 5) {
        REPORT_ERROR(this) << "invalid rounding mode: " << imm;
    }
    LOG_DEBUG(LOG_PARSER) << "rm=" << imm;
    cmd.SetRoundingMode(imm);
}

// immediate for literal load
FIELD_HANDLER_IMPLEMENTATION(ldi)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned nbits = GetImmediateSize(bits_per_branch, bits_per_ldi_long);
    const int64_t imm = EvaluateInt(nbits, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "ldi immediate value=" << imm;
    cmd.set_ldi(imm);
}

FIELD_HANDLER_IMPLEMENTATION(fldsi)
{
    UNREFERENCED_PARAMETER(ip);
    const float f = static_cast<float>(EvaluateFloat());
    uint64_t uimm = util::change_bit_presentation<float, uint32_t>(f);
    uimm = (uimm << 32) | uimm;
    cmd.set_ldi(uimm);
    SetInstructionLength(cmd_long);
}

FIELD_HANDLER_IMPLEMENTATION(flddi)
{
    UNREFERENCED_PARAMETER(ip);
    const double d = static_cast<double>(EvaluateFloat());
    const uint64_t uimm = util::change_bit_presentation<double, uint64_t>(d);
    cmd.set_ldi(uimm);
    SetInstructionLength(cmd_long);
}

FIELD_HANDLER_IMPLEMENTATION(float128_low)
{
    UNREFERENCED_PARAMETER(ip);
    const quadruple q = static_cast<quadruple>(EvaluateFloat());
    fpu::u128 value = util::change_bit_presentation<quadruple, fpu::u128>(q);
    const uint64_t uimm = value.low();
    cmd.set_ldi(uimm);
    SetInstructionLength(cmd_long);
}

FIELD_HANDLER_IMPLEMENTATION(float128_high)
{
    UNREFERENCED_PARAMETER(ip);
    const quadruple q = static_cast<quadruple>(EvaluateFloat());
    fpu::u128 value = util::change_bit_presentation<quadruple, fpu::u128>(q);
    const uint64_t uimm = value.high();
    cmd.set_ldi(uimm);
    SetInstructionLength(cmd_long);
}

// rodata distance for string
FIELD_HANDLER_IMPLEMENTATION(rip_string)
{
    uint64_t uimm;

    if (GetCurrentToken().GetType() == token_string) {
        CProgramSection & section = m_Sections[ESectionId_rodata];
        uimm = section.size();
        GetString(section);
        LOG_DEBUG(LOG_PARSER) << "rip_string: " << section.GetString(static_cast<size_t>(uimm));
        if (section.size() == 0 || section.back() != '\0')
            section.write('\0');
        uimm = section.ComputeIpRelative(ip, uimm);
    } else {
        uimm = GetLabel(ip, ESectionKind::rodata | ESectionKind::data);
    }

    SetForwardBranch(uimm, cmd);
}

FIELD_HANDLER_IMPLEMENTATION(rip_float32)
{
    ReadonlyFloatForRelative<float>(ip, cmd);
}

FIELD_HANDLER_IMPLEMENTATION(rip_float64)
{
    ReadonlyFloatForRelative<double>(ip, cmd);
}

FIELD_HANDLER_IMPLEMENTATION(rip_float128)
{
    ReadonlyFloatForRelative<quadruple>(ip, cmd);
}

// branch distance for ldr/str instructions (ip+uimm28)
FIELD_HANDLER_IMPLEMENTATION(rip_label)
{
    const int64_t uimm = GetLabel(ip, ESectionKind::text | ESectionKind::rodata | ESectionKind::data);
    SetForwardBranch(uimm, cmd);
}

// branch distance for branch/loadaddress instructions
FIELD_HANDLER_IMPLEMENTATION(branch)
{
    const int64_t imm = ComputeBundledOffset(GetLabel(ip, ESectionKind::text));
    const unsigned nbits = GetImmediateSize(bits_per_branch, bits_per_branch_long);
    if (m_PassNumber == EAssemblerPass::link && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for ip-relative offset: " << imm;
    }
    cmd.SetJumpDisplacement(imm);
}

// branch distance for branch/loadaddress instructions
FIELD_HANDLER_IMPLEMENTATION(ldar)
{
    const int64_t imm = ComputeBundledOffset(GetLabel(ip, ESectionKind::text | ESectionKind::rodata | ESectionKind::data));
    const unsigned nbits = GetImmediateSize(bits_per_branch, bits_per_branch_long);
    if (m_PassNumber == EAssemblerPass::link
        && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for ip-relative offset: " << imm;
    }
    cmd.SetJumpDisplacement(imm);
}

// branch distance for compare-reg-reg-and-branch instructions
FIELD_HANDLER_IMPLEMENTATION(brc_branch)
{
    const unsigned nbits = GetImmediateSize(bits_per_brc_branch, bits_per_brc_branch_long);
    const int64_t imm = ComputeBundledOffset(GetLabel(ip, ESectionKind::text));
    if (m_PassNumber == EAssemblerPass::link
        && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for branch: " << imm;
    }
    cmd.SetBranchDisplacement(imm);
}

// branch distance for compare-reg-imm-and-branch instructions
FIELD_HANDLER_IMPLEMENTATION(short_branch)
{
    const unsigned nbits = GetImmediateSize(bits_per_short_branch, bits_per_short_branch_long);
    const int64_t imm = ComputeBundledOffset(GetLabel(ip, ESectionKind::text));
    if (m_PassNumber == EAssemblerPass::link
        && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for branch: " << imm;
    }
    cmd.SetShortBranchDisplacement(imm);
}

// signed immediate value for compare-with-immediate-and-branch instructions
FIELD_HANDLER_IMPLEMENTATION(brc_simm)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned nbits = GetImmediateSize(bits_per_brci_imm, bits_per_brci_imm_long);
    int64_t imm = EvaluateInt(nbits, eval_signed, 0);
    imm = util::sext64(imm, nbits);
    cmd.set_brc_simm(imm);
}

// unsigned immediate value for compare-with-immediate-and-branch instructions
FIELD_HANDLER_IMPLEMENTATION(brc_uimm)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned nbits = GetImmediateSize(bits_per_brci_imm, bits_per_brci_imm_long);
    uint64_t uimm = EvaluateInt(nbits, eval_unsigned, 0);
    uimm = util::zext64(uimm, nbits);
    cmd.set_brc_simm(uimm);
}

// 21(63)-bit signed immediate
FIELD_HANDLER_IMPLEMENTATION(binimm)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned nbits = GetImmediateSize(bits_per_binimm, bits_per_binimm_long);
    int64_t imm = EvaluateInt(nbits, eval_signed, 0);
    imm = util::sext64(imm, nbits);
    cmd.set_bin_simm(imm);
}

// 21(63)-bit unsigned immediate
FIELD_HANDLER_IMPLEMENTATION(binimmu)
{
    UNREFERENCED_PARAMETER(ip);
    const unsigned nbits = GetImmediateSize(bits_per_binimm, bits_per_binimm_long);
    uint64_t uimm = EvaluateInt(nbits, eval_unsigned, 0);
    uimm = util::zext64(uimm, bits_per_binimm_long);
    cmd.set_bin_uimm(uimm);
}

FIELD_HANDLER_IMPLEMENTATION(comma_opt)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
}

FIELD_HANDLER_IMPLEMENTATION(fp_compare_mode)
{
    UNREFERENCED_PARAMETER(ip);
    const uint64_t imm = EvaluateInt(bits_per_fp_compare_mode, eval_unsigned, 0);
    cmd.SetFpCompareMode(imm);
}


FIELD_HANDLER_IMPLEMENTATION(memory_ordering)
{
    UNREFERENCED_PARAMETER(ip);
    EMemoryOrdering order = order_relaxed;
    if (GetCurrentToken() != CParserToken(operator_comma)) {
        cmd.SetMemoryOrdering(order);
        return;
    }
    PopToken();

    const std::string & cmd_name = GetIdentifierInfo(GetIdentifier()).GetName();
    LOG_DEBUG(LOG_PARSER) << "memory order =" << cmd_name;

           if (cmd_name == "relaxed") { order = order_relaxed;
    } else if (cmd_name == "acquire") { order = order_acquire;
    } else if (cmd_name == "release") { order = order_release;
    } else if (cmd_name == "acqrel")  { order = order_acquire_release;
    } else {
        REPORT_ERROR(this) << "invalid memory order '" << cmd_name << "'";
    }

    cmd.SetMemoryOrdering(order);
}

FIELD_HANDLER_IMPLEMENTATION(unknown)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(none)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(opcode)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(fused_opx)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(misc_opx)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(nullify_opx)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(unused_reg)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(indexed_scale_zero)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(memory_ordering_zero)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(rounding_mode_zero)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(raopx)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(misc_opx_null)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(nop)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

/***************************************************************************
* field handler functions table
***************************************************************************/
const
std::array<CAssembler::CFieldHandlerFunction, cf_last>
CAssembler::s_FieldHandlers =
{{
#define FIELD_HANDLER_ADDRESS(field, size, disp, fieldtype, inst_name, descr) &CAssembler::FieldHandler_##field,
    DECLARE_INSTRUCTION_FIELDS(FIELD_HANDLER_ADDRESS)
}};

/***************************************************************************
* this function scan and assemble one instruction or label
***************************************************************************/
void
CAssembler::CompileCode(int64_t ip, CInsn2 & cmd)
{
    LOG_DEBUG(LOG_PARSER) << "line:" << GetLineNum();

    const std::string & ident = GetIdentifierInfo(GetIdentifier()).GetName();
    const auto it = m_decoder.asm_names.find(ident);
    if (it == m_decoder.asm_names.end()) {
       PopToken();
       REPORT_ERROR(this) << "unknown instruction mnemonic " << ident;
    }
    const uint32_t mnem = it->second;
    const unsigned fmt = asm_info[mnem].sw_format;
    LOG_DEBUG(LOG_PARSER) << "fmt=" << fmt;

    cmd = CInsn2(asm_info[mnem].tmplt, 0);
    SetInstructionLength(cmd_short);

    for (const EInstructionField field : asm_fmt_field[fmt]) {
        if (field == cf_none)
           break;
        LOG_DEBUG(LOG_PARSER) << ' ' << field;
        (this->*s_FieldHandlers[field])(ip, cmd);
    }

    ScanPredicates();
    LOG_DEBUG(LOG_PARSER) << '\n';
    CheckLineEnd();
}

/***************************************************************************
* this function scan and assemble for data/text segment
***************************************************************************/
void
CAssembler::CompileData(CProgramSection & section)
{
    int  cmp;

    const std::string & ident = GetIdentifierInfo(GetIdentifier()).GetName();
    const auto it = asm_data.find(ident);
    if (it == asm_data.end()) {
        REPORT_ERROR(this) << "unknown data mnemonic";
    }

    const uint32_t mid = it->second;
    switch (mid) {
        case asm_align:
            cmp = static_cast<unsigned>(EvaluateInt(bits_per_byte * 2, eval_unsigned, 0));
            if ((cmp == 0) || (((cmp-1) & cmp) != 0)) {
                REPORT_ERROR(this) << "alignment " << cmp << " is not power of 2";
            }
            if (cmp > 64*1024) {
                REPORT_ERROR(this) << "alignment " << cmp << " too big, more than " << 64*1024;
            }
            section.align(cmp);
            break;

        case asm_space:
            cmp = static_cast<unsigned>(EvaluateInt(16, eval_unsigned, 0));
            for (int i=0; i<cmp; i++) {
                section.write(static_cast<uint8_t>(0));
            }
            break;

        case asm_ascii:
            if (GetCurrentToken().GetType() == token_string) {
                GetString(section);
            }
            break;

        case asm_d1:  EvaluateUnsigned< uint8_t>(section);  break;
        case asm_d2:  EvaluateUnsigned<uint16_t>(section);  break;
        case asm_d4:  EvaluateUnsigned<uint32_t>(section);  break;
        case asm_d8:  EvaluateUnsigned<uint64_t>(section);  break;

        case asm_float:   EvaluateFloat<float, uint32_t>(section);          break;
        case asm_double:  EvaluateFloat<double, uint64_t>(section);         break;
        //case asm_extend:  EvaluateFloat<extended, fpu::u128>(section);      break;
        case asm_quad:    EvaluateFloat<quadruple, fpu::u128>(section);   break;

    } // end switch data mnemonic

    CheckLineEnd();
}


/***************************************************************************
* this function test assembler tables at startup
***************************************************************************/
void
CAssembler::Setup(void)
{
    m_Sections.clear();
    m_Sections.emplace_back(0, ESectionKind::text);
    m_Sections.emplace_back(1, ESectionKind::rodata);
    m_Sections.emplace_back(2, ESectionKind::data);
}

void
CAssembler::AssembleForCodeSection(CProgramSection & section)
{
    while (IsNextRowExist()) {
        if (GetCurrentToken() == token_directive) {
            return;
        }
        CInsn2 cmd(0,0);
        int64_t const ip = section.ComputeNextIp();
        // find possible label
        if (ScanLabel(section)) {
            continue;
        }
        CompileCode(ip, cmd);
        if (GetInstructionLength() == cmd_long) {
            EmitInstruction(section, cmd.GetRaw(), cmd.GetRawExtension());
        } else {
            EmitInstruction(section, cmd.GetRaw());
        }
    }

    REPORT_ERROR(this) << "text-section must be closed";
}

void
CAssembler::AssembleForDataSection(CProgramSection & section)
{
    while (IsNextRowExist()) {
        if (GetCurrentToken() == token_directive) {
            return;
        }

        if (ScanLabel(section)) {
            continue;
        }
        CompileData(section);
    }

    REPORT_ERROR(this) << "data-section must be closed";
}

/***************************************************************************
* this function scan token stream for assembler directives
***************************************************************************/
int
CAssembler::CompileFile(void)
{
    for (auto & section : m_Sections) {
        section.clear();
    }

    StartNewBundle();
    CTokenizer::InitPass();

    while (IsNextRowExist()) {
        if (GetCurrentToken() != token_directive) {
            REPORT_ERROR(this) << "can't find start directive";
        }

        PopToken(); // skip token_directive

        const std::string & ident = GetIdentifierInfo(GetIdentifier()).GetName();
        const auto it = asm_directives.find(ident);
        if (it == asm_directives.end()) {
            REPORT_ERROR(this) << "unknown directive: " << ident;
        }

        uint32_t const mnem = it->second;
        LOG_DEBUG(LOG_PARSER) << "asm_directives = " << it->first;
        switch (mnem) {
            case asm_dir_end:
                break;
            case asm_dir_text:
                AssembleForCodeSection(m_Sections[ESectionId_code]);
                break;
            case asm_dir_rodata:
                AssembleForDataSection(m_Sections[ESectionId_rodata]);
                break;
            case asm_dir_data:
                AssembleForDataSection(m_Sections[ESectionId_data]);
                break;
            default:
                REPORT_ERROR(this) << "unknown directive '" << it->first << "'";
        } // end directive switch
    } // end line loading

    const size_t segment_alignment = bytes_per_sector;

    for (auto & section : m_Sections) {
        if (section.GetSectionType() & ESectionKind::text) {
            uint64_t const fillInst = INSN_TMPLT(halt);
            // pack last (possible not full) bundle
            EmitLabel(section, fillInst);
            // align code segment size to offset text segment at link time
            while (section.size() & (segment_alignment-1)) {
                EmitInstruction(section, fillInst);
                EmitInstruction(section, fillInst);
                EmitInstruction(section, fillInst);
            }
        } else {
            section.align(segment_alignment);
        }
    }

    return 0;
}

void
CAssembler::Write(std::ostream& out)
{
    CExeFileHeader header(m_Sections[ESectionId_code].size(),
                          m_Sections[ESectionId_rodata].size(),
                          m_Sections[ESectionId_data].size()
                         );

    header.write(out);
    for (const auto & section : m_Sections) {
        LOG_DEBUG(LOG_PARSER) << "section:" << section.size();
        if (section.size() > 0) {
            section.write(out);
        }
    }
}

void
CAssembler::init_pass(EAssemblerPass passnum)
{
    LOG_DEBUG(LOG_PARSER) << "pass " << static_cast<int>(passnum);

    CTokenizer::InitPass();

    m_PassNumber = passnum;
    m_NopNumber = 0;
    m_InstructionNumber = 0;

    StartNewBundle();
}

int
CAssembler::Assemble(std::istream& input, std::ostream& output, assemble_mode mode)
{
    Setup();
    CTokenizer::Init();
    TokenizeInput(input);
    init_pass(EAssemblerPass::compile);
    CompileFile();

    // save code segment size for future adding to text offsets
    m_Sections[0].global_offset = 0;
    for (size_t i = 1; i < m_Sections.size(); ++i) {
        m_Sections[i].global_offset = m_Sections[i-1].GetNextGlobalOffset();
    }

    init_pass(EAssemblerPass::link);
    CompileFile();

    CExeFileHeader header(m_Sections[ESectionId_code].size(),
                          m_Sections[ESectionId_rodata].size(),
                          m_Sections[ESectionId_data].size());

    if (mode == assemble_mode_hex) {
        output << "\n\nconst unsigned char program[] =\n{\n";
        // print_segment("head",   reinterpret_cast<const unsigned int *>(&header), sizeof(header), output);
        for (auto & section : m_Sections) {
            if (section.size() > 0) {
                section.print("section", output);
            }
        }
        output << "\n/* end image */\n};\n";
    } else {
        header.write(output);
        for (auto & section : m_Sections) {
            if (section.size() > 0) {
                section.write(output);
            }
        }
    }
    LOG_DEBUG(LOG_PARSER) << "compilation complete";
    return 0;
}

} // namespace postrisc
