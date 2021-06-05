#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "binary_format/file_header.hpp"
#include "Assembler.hpp"

namespace postrisc {

Assembler::Assembler(const Decoder& decoder)
    : m_decoder(decoder)
{
}

Assembler::~Assembler(void)
{
}

void
Assembler::RegisterDefine(const char *name, u64 value)
{
    m_defines[name] = m_define_values.size();
    m_define_values.push_back(value);
}

void
Assembler::ExportDefinitions(std::ostream& out)
{
    for (const auto& x : m_defines) {
        out << x.first << "    0x" << std::hex << m_define_values[x.second] << fmt::endl;
    }
}

u64
Assembler::make_nop()
{
    //unsigned const linenum = GetLineNum() & util::makemask<u64>(bits_per_call);
    InstructionSingle cmd(INSN_TMPLT(nop));
    //cmd.set_ld_imm(linenum);
    return cmd.GetRaw();
}

template<typename SIGNED>
void
Assembler::EvaluateSigned(ProgramSection& section)
{
    if (section.size() & (sizeof(SIGNED)-1)) {
        REPORT_ERROR(this) << "unaligned " << (bits_per_byte * sizeof(SIGNED)) << "-bit signed data";
    }

    SIGNED imm = static_cast<SIGNED>(EvaluateInt(bits_per_byte * sizeof(SIGNED), eval_signed, 0));
    section.write(imm);
}

template<typename UNSIGNED>
void
Assembler::EvaluateUnsigned(ProgramSection& section)
{
    if (section.size() & (sizeof(UNSIGNED)-1)) {
        REPORT_ERROR(this) << "unaligned " << (bits_per_byte * sizeof(UNSIGNED)) << "-bit unsigned data";
    }

    UNSIGNED imm = static_cast<UNSIGNED>(EvaluateInt(bits_per_byte * sizeof(UNSIGNED), eval_unsigned, 0));
    section.write(imm);
}

template<typename FLOAT, typename UNSIGNED>
void
Assembler::EvaluateFloat(ProgramSection& section)
{
    if (section.size() & (sizeof(UNSIGNED)-1)) {
        REPORT_ERROR(this) << "unaligned " << (bits_per_byte * sizeof(FLOAT)) << "-bit floating point data";
    }

    const FLOAT f = static_cast<FLOAT>(EvaluateFloat());
    const UNSIGNED u = std::bit_cast<UNSIGNED>(f);
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
    X(i1) X(i2) X(i4) X(i8) \
    X(quad) \
    X(space) \
    X(string) \
    X(u1) X(u2) X(u4) X(u8) \

enum asm_data_keyword {
#define DATA_ENUM_X(name) asm_##name,
    DECLARE_DATA_MNEM(DATA_ENUM_X)
    asm_data_last
};

static const keywordmap<asm_data_keyword> asm_data = {
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

enum asm_directive {
#define DIRECTIVE_ENUM_X(name, descr) asm_dir_##name,
  DECLARE_ASM_DIRECTIVE(DIRECTIVE_ENUM_X)
  asm_dir_last
};

const keywordmap<u32> Assembler::asm_directives = {{
#define DIRECTIVE_NAME_X(name, descr) { #name, asm_dir_##name },
  DECLARE_ASM_DIRECTIVE(DIRECTIVE_NAME_X)
}};

i64
Assembler::ComputeBundledOffset(i64 offset) const
{
    if (offset & util::makemask<u64>(log_bytes_per_bundle)) {
        REPORT_ERROR(this) << "target label is not 16-byte aligned: " << fmt::hex(offset);
    }
    return offset >> log_bytes_per_bundle;
}

/***************************************************************************
* this function find register name in stream
***************************************************************************/
u32
Assembler::GetRegister(const keywordmap<u32>& regs)
{
    if (GetToken() != ParserToken(operator_percent)) {
        REPORT_ERROR(this) << "can't find '%' register marker";
    }

    const ParserToken token = GetToken();

    const ETokenKind ttype = token.GetType();
    const u32 tdata = token.GetData();

    if (ttype != token_identifier) {
        REPORT_ERROR(this) << "register name expected, token = (" << ttype << "," << tdata << ")";
    }

    const std::string& cmd = GetIdentifierInfo(tdata).GetName();

    const auto it = regs.find(cmd);
    if (it == regs.end()) {
        REPORT_ERROR(this) << "invalid register: " << cmd;
        return -1;
    }

    LOG_DEBUG(LOG_PARSER) << "regs: " << it->second << "cmd: " << cmd;
    return it->second;
}

u32
Assembler::GetGeneralRegister(void)
{
    return GetRegister(m_decoder.m_GeneralRegisters);
}

u32
Assembler::GetSpecialRegister(void)
{
    return GetRegister(m_decoder.m_SpecialRegisters);
}

/***************************************************************************
* this function scan token stream for identifier and return id number
***************************************************************************/
u32
Assembler::GetIdentifier(void)
{
    ParserToken  token = GetToken();

    if (token.GetType() != token_identifier) {
       LOG_ERROR(LOG_PARSER) << "GetType()=" << fmt::dec(token.GetType());
       REPORT_ERROR(this) << "identifier expected";
    }

    return (token.GetData());
}

void
Assembler::StartNewBundle()
{
    m_BundleTemplate = EBundleTemplate::bundle_sss;
    m_CurrentSlotNum = 0;
}

void
Assembler::EmitInstruction(ProgramSection& section, u64 cmd)
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

    Bundle  bundle(m_Slots, m_BundleTemplate);

    LOG_DEBUG(LOG_PARSER) << "bundle=" << bundle;

    section.write(bundle);

    StartNewBundle();
}

void
Assembler::EmitInstruction(ProgramSection& section, u64 cmd, u64 ext)
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
Assembler::EmitLabel(ProgramSection& section, u64 cmd)
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
i64
Assembler::GetLabel(i64 ip, ESectionKind requiredSectionType)
{
    const u32 ident = GetIdentifier();
    const std::string& cmd = GetIdentifierInfo(ident).GetName();

    if (m_PassNumber < EAssemblerPass::link) { // first compiler pass
       return 0;
    }

    // second and later compiler passes
    const AssemblerIdentifier& idInfo = GetIdentifierInfo(ident);
    if (!idInfo.IsValidSection()) {
       REPORT_ERROR(this) << "unknown label " <<  cmd;
    }

    const u32 sectionId = idInfo.GetSectionId();
    LOG_DEBUG(LOG_PARSER) << "old label: sectionId=" << sectionId << ", " << cmd;

    if ((m_Sections[sectionId].GetSectionType() & requiredSectionType) == 0) {
        REPORT_ERROR(this) << "can't use label '" << cmd << "' here";
    }

    i64 imm = GetIdentifierInfo(ident).data;
    imm = m_Sections[sectionId].ComputeIpRelative(ip, imm);

    if (GetInstructionLength() == cmd_long && m_CurrentSlotNum == 2) {
        LOG_DEBUG(LOG_PARSER) << "long instruction doesn't fit in current bundle, nop will be inserted, ip updated";
        imm -= sizeof(Bundle);
    }

    return imm;
}


/********************************************************************************
* this function build labels table at first compiler pass,
* and do nothing at second pass.
* label is defined as: <pair><identifier/><colon/></pair>
********************************************************************************/
bool
Assembler::ScanLabel(ProgramSection& section)
{
    const ParserToken token_ident = GetCurrentToken();

    if (token_ident.GetType() != token_identifier)
       return false;

    if (GetCurrentToken(1) != ParserToken(operator_colon))
       return false;

    PopToken(); // skip identifier
    PopToken(); // skip colon

    // label alignment
    // .label_name: (alignment)
    LOG_DEBUG(LOG_PARSER) << GetCurrentToken();
    if (GetCurrentToken() == ParserToken(operator_l_parenthese)) {
        LOG_DEBUG(LOG_PARSER) << "( found";
        u64 const alignment = EvaluateInt(bits_per_byte * sizeof(u64), eval_unsigned, 0);
        LOG_DEBUG(LOG_PARSER) << "align=" << alignment;
        if (alignment == 0 || (alignment & (alignment - 1)) != 0) {
            REPORT_ERROR(this) << "alignment should be power of 2: " << alignment;
        }
        if (section.GetSectionType() == ESectionKind::text) {
            if (alignment <= sizeof(Bundle)) {
                REPORT_ERROR(this) << "code alignment is too little: " << alignment;
            }
            EmitLabel(section, make_nop());
            while ((section.size() & (alignment-1)) != 0) {
                EmitInstruction(section, make_nop());
                EmitInstruction(section, make_nop());
                EmitInstruction(section, make_nop());
            }
        } else {
            section.align(std::max<u64>(alignment, forward_offset_alignment));
        }
        LOG_DEBUG(LOG_PARSER) << "after align=" << alignment;
    }

    const u32 ident = token_ident.GetData();

    if (section.GetSectionType() == ESectionKind::text) {
        // check if this label has references 
        // if(GetIdentifierInfo(ident).ref_count > 0 || m_PassNumber == EAssemblerPass::compile)
        EmitLabel(section, make_nop());
    } else {
        // default alignment
        section.align(forward_offset_alignment);
    }

    AssemblerIdentifier& id = GetIdentifierInfo(ident);

    if (m_PassNumber == EAssemblerPass::compile) {
        if (id.IsValidSection()) {
            LOG_DEBUG(LOG_PARSER) << "old label: " << GetIdentifierInfo(ident).GetName();
            REPORT_ERROR(this) << "duplicate labels " << GetIdentifierInfo(ident).GetName();
        }
        const u32 offset = static_cast<unsigned>(section.size());
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
Assembler::CheckLineEnd(void)
{
    // check end of string. it must be clear.
    const ParserToken opc = GetCurrentToken();

    if (opc.GetType() != token_eol) {
        LOG_DEBUG(LOG_PARSER) << opc;
        REPORT_ERROR(this) << "endline expected";
    }
}

unsigned
Assembler::GetImmediateSize(EInstructionSize size1, EInstructionFullSize size2)
{
    const EInstructionLength length = GetInstructionLength();

    switch (length) {
        case cmd_short:  return static_cast<unsigned>(size1);
        case cmd_long:   return static_cast<unsigned>(size2);
    }

    REPORT_ERROR(this) << "invalid instruction length";
    return 0;
}

void
Assembler::SetForwardBranch(u64 uimm, InstructionDouble& cmd)
{
    unsigned const nbits = GetImmediateSize(bits_per_call, bits_per_call_long);

    if (m_PassNumber == EAssemblerPass::link &&
        uimm != util::zext64(uimm, nbits)) {
        REPORT_ERROR(this) << "too long distance for forward branch: " << uimm;
        return;
    }

    if (m_PassNumber == EAssemblerPass::link &&
        uimm % forward_offset_alignment != 0) {
        REPORT_ERROR(this) << "forward branch distance " << uimm << " isn't aligned to " << forward_offset_alignment << " bytes";
        return;
    }

    uimm /= forward_offset_alignment;
    cmd.SetForwardBranch(uimm);
}

unsigned int
Assembler::GetShiftAmount(void)
{
    return static_cast<unsigned>(EvaluateInt(bits_per_shamt, eval_unsigned, 0));
}

u32
Assembler::GetExpectedInstructionNumber(void) const
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
Assembler::GetNullificationDistance(unsigned offset)
{
    unsigned distance = 0;
    if (GetCurrentToken().GetType() == token_identifier) {
        // identifier
        ParserToken token_ident = GetCurrentToken();
        PopToken();
        const u32 ident = token_ident.GetData();
        AssemblerIdentifier& id = GetIdentifierInfo(ident);

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

    if (distance > util::makemask<unsigned>(bits_per_nullify_dist)) {
        REPORT_ERROR(this) << "predicate block distance " << distance
                           << " is greater than max predication block size "
                           << util::makemask<unsigned>(bits_per_nullify_dist);
    }
    return distance;
}

void
Assembler::ScanPredicate(void)
{
    const ParserToken token_ident = GetCurrentToken();
    if (token_ident.GetType() != token_identifier) {
        REPORT_ERROR(this) << "predicate name expected after colon";
        return;
    }
    PopToken(); // skip identifier

    const u32 ident = token_ident.GetData();
    AssemblerIdentifier& id = GetIdentifierInfo(ident);
    if (m_PassNumber != EAssemblerPass::compile) {
        // gather/check/store predicate mask on first pass only
        return;
    }

    const auto base = id.data;
    const auto current_pos = GetExpectedInstructionNumber();

    LOG_DEBUG(LOG_PREDICATION) << "nullify base=" << base << " pos=" << current_pos;

    if (current_pos < base) {
        REPORT_ERROR(this) << "predicate name used before definition: " << GetIdentifierInfo(ident).GetName();
        return;
    }

    id.mask = current_pos - base;
    LOG_DEBUG(LOG_PREDICATION) << "nullify mask=" << id.mask;
}

void
Assembler::ScanPredicates(void)
{
    if (GetCurrentToken() != ParserToken(operator_l_parenthese)) {
        return;
    }
    PopToken(); // skip '('
    ScanPredicate();

    while (GetCurrentToken() == ParserToken(operator_comma)) {
        PopToken(); // skip comma
        ScanPredicate();
    }
    if (GetCurrentToken() != ParserToken(operator_r_parenthese)) {
        REPORT_ERROR(this) << "predicate list must have parnetheses";
        return;
    }
    PopToken(); // skip ')'
}

void
Assembler::SetInstructionLength(EInstructionLength cmdlen)
{
    m_InstructionLength = cmdlen;
}

EInstructionLength
Assembler::GetInstructionLength(void) const
{
    return m_InstructionLength;
}

template<typename FLOAT>
void
Assembler::ReadonlyFloatForRelative(i64 ip, InstructionDouble& cmd)
{
    ProgramSection& section = m_Sections[ESectionId_rodata];
    section.align(sizeof(FLOAT));
    u64 uimm = section.size();
    assert((uimm & (sizeof(FLOAT)-1)) == 0);
    section.write(static_cast<FLOAT>(EvaluateFloat()));
    uimm = section.ComputeIpRelative(ip, uimm);
    SetForwardBranch(uimm, cmd);
}

/***************************************************************************
* field handlers
***************************************************************************/
#define FIELD_HANDLER_IMPLEMENTATION(FIELD_NAME) \
    void Assembler::FieldHandler_##FIELD_NAME(i64 ip, InstructionDouble& cmd)

FIELD_HANDLER_IMPLEMENTATION(comma)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    if (GetToken() != ParserToken(operator_comma)) {
        REPORT_ERROR(this) << "can't find ',' delimiter";
        return;
    }
}

// optional '.l' (long) prefix for explicit double-width instruction emit
FIELD_HANDLER_IMPLEMENTATION(lprefix)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    if (GetCurrentToken() != ParserToken(operator_point)) {
        return;
    }
    PopToken();

    const std::string& cmd_name = GetIdentifierInfo(GetIdentifier()).GetName();
    if (cmd_name == "l") {
        SetInstructionLength(cmd_long);
    } else {
        // this may be scale mode option
        PutBackToken();
        PutBackToken();
    }
}

// general register in (a,b,c,d) positions
FIELD_HANDLER_IMPLEMENTATION(gpra)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(a)=" << fmt::dec(regNum);
    cmd.set_ra(regNum);
}

FIELD_HANDLER_IMPLEMENTATION(gprb)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(b)=" << fmt::dec(regNum);
    cmd.set_rb(regNum);
}

FIELD_HANDLER_IMPLEMENTATION(gprc)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(c)=" << fmt::dec(regNum);
    cmd.set_rc(regNum);
}

FIELD_HANDLER_IMPLEMENTATION(gprd)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const regNum = GetGeneralRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum(d)=" << fmt::dec(regNum);
    cmd.set_rd(regNum);
}

// special register in c
FIELD_HANDLER_IMPLEMENTATION(spr)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const regNum = GetSpecialRegister();
    LOG_DEBUG(LOG_PARSER) << "regNum=" << fmt::dec(regNum);
    cmd.set_rs(regNum);
}

FIELD_HANDLER_IMPLEMENTATION(framesize)
{
    UNREFERENCED_PARAMETER(ip);
    const u64 imm = EvaluateInt(bits_per_framesize, eval_unsigned, 0);
    if (imm > max_stacked_framesize) {
        REPORT_ERROR(this) << "framesize=" << imm << " exceed limit=" << max_stacked_framesize;
        return;
    }
    LOG_DEBUG(LOG_PARSER) << "framesize=" << imm;
    cmd.SetFrameSize(imm);
}

// 6-bit shift amount in (b,c,d,e) position
FIELD_HANDLER_IMPLEMENTATION(shamt_b)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(b)=" << fmt::dec(shift_amout);
    cmd.set_shamt_b(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(shamt_c)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(c)=" << fmt::dec(shift_amout);
    cmd.set_shamt_c(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(shamt_d)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(d)=" << fmt::dec(shift_amout);
    cmd.set_shamt_d(shift_amout);
}

FIELD_HANDLER_IMPLEMENTATION(shamt_e)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const shift_amout = GetShiftAmount();
    LOG_DEBUG(LOG_PARSER) << "shift_amout(e)=" << fmt::dec(shift_amout);
    cmd.set_shamt_e(shift_amout);
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

// 3-bit scale for indexed load/store
FIELD_HANDLER_IMPLEMENTATION(indexed_scale)
{
    UNREFERENCED_PARAMETER(ip);
    const u64 imm = EvaluateInt(bits_per_indexed_scale, eval_unsigned, 0);
    LOG_DEBUG(LOG_PARSER) << "scale=" << imm;
    cmd.SetIndexedScale(static_cast<unsigned>(imm));
}

// 10-bit displacement for indexed load/store
FIELD_HANDLER_IMPLEMENTATION(indexed_disp)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_indexed_disp, bits_per_indexed_disp_long);
    const i64 imm = EvaluateInt(nbits, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "indexed disp=" << imm;
    cmd.SetIndexedDisplacement(imm);
}

FIELD_HANDLER_IMPLEMENTATION(stride_simm)
{
    UNREFERENCED_PARAMETER(ip);
    u64 const imm = EvaluateInt(bits_per_update_stride, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "simm10=" << imm;
    cmd.SetUpdateStride(imm);
}

FIELD_HANDLER_IMPLEMENTATION(update_stride)
{
    UNREFERENCED_PARAMETER(ip);
    const i64 imm = EvaluateInt(bits_per_update_stride, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "stride=" << imm;
    if (imm == 0) {
        REPORT_ERROR(this) << "stride immediate for base update must be nonzero (or use instruction without base update)";
        return;
    }
    cmd.SetUpdateStride(imm);
}

// immediate for literal load
FIELD_HANDLER_IMPLEMENTATION(ld_imm)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_call, bits_per_ld_imm_long);
    const i64 imm = EvaluateInt(nbits, eval_signed, 0);
    LOG_DEBUG(LOG_PARSER) << "ld_imm immediate value=" << imm;
    cmd.set_ld_imm(imm);
}

FIELD_HANDLER_IMPLEMENTATION(ld_imm_f32)
{
    UNREFERENCED_PARAMETER(ip);
    const float f = static_cast<float>(EvaluateFloat());
    u64 uimm = std::bit_cast<u32>(f);
    uimm = (uimm << 32) | uimm;
    cmd.set_ld_imm(uimm);
    SetInstructionLength(cmd_long);
}

FIELD_HANDLER_IMPLEMENTATION(ld_imm_f64)
{
    UNREFERENCED_PARAMETER(ip);
    const double d = static_cast<double>(EvaluateFloat());
    const u64 uimm = std::bit_cast<u64>(d);
    cmd.set_ld_imm(uimm);
    SetInstructionLength(cmd_long);
}

// rodata distance for string
FIELD_HANDLER_IMPLEMENTATION(rip_string)
{
    u64 uimm;

    if (GetCurrentToken().GetType() == token_string) {
        ProgramSection& section = m_Sections[ESectionId_rodata];
        section.align(forward_offset_alignment);
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
    ReadonlyFloatForRelative<native::f32>(ip, cmd);
}

FIELD_HANDLER_IMPLEMENTATION(rip_float64)
{
    ReadonlyFloatForRelative<native::f64>(ip, cmd);
}

FIELD_HANDLER_IMPLEMENTATION(rip_float128)
{
    ReadonlyFloatForRelative<native::f128>(ip, cmd);
}

// branch distance for ldNr/stNr instructions (ip+uimm28)
FIELD_HANDLER_IMPLEMENTATION(rip_label)
{
    const i64 uimm = GetLabel(ip, ESectionKind::text | ESectionKind::rodata | ESectionKind::data);
    SetForwardBranch(uimm, cmd);
}

// branch distance for branch/loadaddress instructions
FIELD_HANDLER_IMPLEMENTATION(call)
{
    const i64 imm = ComputeBundledOffset(
        GetLabel(ip, ESectionKind::text));
    unsigned const nbits = GetImmediateSize(bits_per_call, bits_per_call_long);
    if (m_PassNumber == EAssemblerPass::link && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for ip-relative offset: " << imm;
        return;
    }
    cmd.SetJumpDisplacement(imm);
}

// branch distance for branch/loadaddress instructions
FIELD_HANDLER_IMPLEMENTATION(lda_r)
{
    const i64 imm = ComputeBundledOffset(
       GetLabel(ip, ESectionKind::text | ESectionKind::rodata | ESectionKind::data));
    unsigned const nbits = GetImmediateSize(bits_per_call, bits_per_call_long);
    if (m_PassNumber == EAssemblerPass::link
        && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for ip-relative offset: " << imm;
        return;
    }
    cmd.SetJumpDisplacement(imm);
}

// branch distance for compare-reg-reg-and-branch instructions
FIELD_HANDLER_IMPLEMENTATION(branch)
{
    unsigned const nbits = GetImmediateSize(bits_per_branch, bits_per_branch_long);
    const i64 imm = ComputeBundledOffset(GetLabel(ip, ESectionKind::text));
    if (m_PassNumber == EAssemblerPass::link
        && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for branch: " << imm;
        return;
    }
    cmd.SetBranchDisplacement(imm);
}

// signed immediate value for compare-with-immediate-and-branch instructions
FIELD_HANDLER_IMPLEMENTATION(brc_simm)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_brci_imm, bits_per_brci_imm_long);
    i64 imm = EvaluateInt(nbits, eval_signed, 0);
    imm = util::sext64(imm, nbits);
    cmd.set_brc_simm(imm);
}

// unsigned immediate value for compare-with-immediate-and-branch instructions
FIELD_HANDLER_IMPLEMENTATION(brc_uimm)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_brci_imm, bits_per_brci_imm_long);
    u64 imm = EvaluateInt(nbits, eval_unsigned, 0);
    imm = util::zext64(imm, nbits);
    cmd.set_brc_uimm(imm);
}

// 21(63)-bit signed immediate
FIELD_HANDLER_IMPLEMENTATION(bin_simm)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_binimm, bits_per_binimm_long);
    i64 imm = EvaluateInt(nbits, eval_signed, 0);
    imm = util::sext64(imm, nbits);
    cmd.set_bin_simm(imm);
}

// 21(63)-bit unsigned immediate
FIELD_HANDLER_IMPLEMENTATION(bin_uimm)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_binimm, bits_per_binimm_long);
    u64 imm = EvaluateInt(nbits, eval_unsigned, 0);
    imm = util::zext64(imm, nbits);
    cmd.set_bin_uimm(imm);
}

// 14(56)-bit signed immediate
FIELD_HANDLER_IMPLEMENTATION(mid_bin_simm)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_mid_binimm, bits_per_mid_binimm_long);
    i64 imm = EvaluateInt(nbits, eval_signed, 0);
    imm = util::sext64(imm, nbits);
    cmd.set_mid_bin_simm(imm);
}

FIELD_HANDLER_IMPLEMENTATION(loop_disp)
{
    unsigned const nbits = GetImmediateSize(bits_per_loop_disp, bits_per_loop_disp_long);
    const i64 imm = ComputeBundledOffset(GetLabel(ip, ESectionKind::text));
    if (m_PassNumber == EAssemblerPass::link
        && imm != util::sext64(imm, nbits)) {
        REPORT_ERROR(this) << "too long distance for loop: " << imm;
        return;
    }
    cmd.SetLoopDisplacement(imm);
}

FIELD_HANDLER_IMPLEMENTATION(loop_stride)
{
    UNREFERENCED_PARAMETER(ip);
    unsigned const nbits = GetImmediateSize(bits_per_loop_stride, bits_per_loop_stride_long);
    u64 imm = EvaluateInt(nbits, eval_unsigned, -1);
    imm = util::zext64(imm, nbits);
    cmd.SetLoopStride(imm);
}

FIELD_HANDLER_IMPLEMENTATION(comma_opt)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
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

FIELD_HANDLER_IMPLEMENTATION(argsdelimiter)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
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

FIELD_HANDLER_IMPLEMENTATION(branch_opx)
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

FIELD_HANDLER_IMPLEMENTATION(misc_opx_null)
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

FIELD_HANDLER_IMPLEMENTATION(branch_opx_res)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(indexed_opx)
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

FIELD_HANDLER_IMPLEMENTATION(misc_noarg)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

FIELD_HANDLER_IMPLEMENTATION(res21)
{
    UNREFERENCED_PARAMETER(ip);
    UNREFERENCED_PARAMETER(cmd);
    LOG_ABORT << "reserved field";
}

#undef FIELD_HANDLER_IMPLEMENTATION

/***************************************************************************
* field handler functions table
***************************************************************************/
const
std::array<Assembler::CFieldHandlerFunction, cf_last>
Assembler::s_FieldHandlers =
{{
#define FIELD_HANDLER_ADDRESS(field, size, disp, fieldtype, inst_name, descr) \
    CFieldHandlerFunction(&Assembler::FieldHandler_##field),
    DECLARE_INSTRUCTION_FIELDS(FIELD_HANDLER_ADDRESS)
#undef FIELD_HANDLER_ADDRESS
}};

/***************************************************************************
* this function scan and assemble one instruction or label
***************************************************************************/
void
Assembler::CompileCode(i64 ip, InstructionDouble& cmd)
{
    LOG_DEBUG(LOG_PARSER) << "line:" << GetLineNum();

    std::string ident = GetIdentifierInfo(GetIdentifier()).GetName();

    auto insn_length = cmd_short;
    //if (ident.ends_with(".l")) {
    //    insn_length = cmd_long;
    //    ident = ident.substr(0, ident.size() - 2);
    //}

    const auto it = m_decoder.asm_names.find(ident);
    if (it == m_decoder.asm_names.end()) {
       PopToken();
       REPORT_ERROR(this) << "unknown instruction mnemonic " << ident;
       return;
    }
    const u32 mnem = it->second;
    unsigned const fmt = asm_info[mnem].sw_format;
    LOG_DEBUG(LOG_PARSER) << "fmt=" << fmt;

    cmd = InstructionDouble(asm_info[mnem].tmplt, 0);
    SetInstructionLength(insn_length);

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
Assembler::CompileData(ProgramSection& section)
{
    int  cmp;

    const std::string& ident = GetIdentifierInfo(GetIdentifier()).GetName();
    const auto it = asm_data.find(ident);
    if (it == asm_data.end()) {
        REPORT_ERROR(this) << "unknown data mnemonic";
        return;
    }

    const u32 mid = it->second;
    switch (mid) {
        case asm_align:
            cmp = static_cast<unsigned>(EvaluateInt(bits_per_byte * 2, eval_unsigned, 0));
            if ((cmp == 0) || (((cmp-1) & cmp) != 0)) {
                REPORT_ERROR(this) << "alignment " << cmp << " is not power of 2";
                return;
            }
            if (cmp > 64*1024) {
                REPORT_ERROR(this) << "alignment " << cmp << " too big, more than " << 64*1024;
                return;
            }
            section.align(cmp);
            break;

        case asm_space:
            cmp = static_cast<unsigned>(EvaluateInt(16, eval_unsigned, 0));
            for (int i = 0; i < cmp; i++) {
                section.write(static_cast<u8>(0));
            }
            break;

        case asm_ascii:
            if (GetCurrentToken().GetType() == token_string) {
                GetString(section);
            }
            break;

        case asm_i1:  EvaluateSigned< i8>(section);  break;
        case asm_i2:  EvaluateSigned<i16>(section);  break;
        case asm_i4:  EvaluateSigned<i32>(section);  break;
        case asm_i8:  EvaluateSigned<i64>(section);  break;

        case asm_d1:  EvaluateUnsigned< u8>(section);  break;
        case asm_d2:  EvaluateUnsigned<u16>(section);  break;
        case asm_d4:  EvaluateUnsigned<u32>(section);  break;
        case asm_d8:  EvaluateUnsigned<u64>(section);  break;

        case asm_float:   EvaluateFloat<native::f32,   u32>(section);  break;
        case asm_double:  EvaluateFloat<native::f64,   u64>(section);  break;
        case asm_quad:    EvaluateFloat<native::f128, u128>(section);  break;

    } // end switch data mnemonic

    CheckLineEnd();
}


/***************************************************************************
* this function test assembler tables at startup
***************************************************************************/
void
Assembler::Setup(void)
{
    m_Sections.clear();
    m_Sections.emplace_back(0, ESectionKind::text);
    m_Sections.emplace_back(1, ESectionKind::rodata);
    m_Sections.emplace_back(2, ESectionKind::data);
}

void
Assembler::AssembleForCodeSection(ProgramSection& section)
{
    while (IsNextRowExist()) {
        if (GetCurrentToken() == token_directive) {
            return;
        }
        InstructionDouble cmd(0,0);
        i64 const ip = section.size();
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
Assembler::AssembleForDataSection(ProgramSection& section)
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
Assembler::CompileFile(void)
{
    for (auto& section : m_Sections) {
        section.clear();
    }

    StartNewBundle();
    Tokenizer::InitPass();

    while (IsNextRowExist()) {
        if (GetCurrentToken() != token_directive) {
            REPORT_ERROR(this) << "can't find start directive";
            return 0;
        }

        PopToken(); // skip token_directive

        const std::string& ident = GetIdentifierInfo(GetIdentifier()).GetName();
        const auto it = asm_directives.find(ident);
        if (it == asm_directives.end()) {
            REPORT_ERROR(this) << "unknown directive: " << ident;
            return -1;
        }

        u32 const mnem = it->second;
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
                return -1;
        } // end directive switch
    } // end line loading

    const size_t segment_alignment = bytes_per_sector;

    for (auto& section : m_Sections) {
        if (section.GetSectionType() & ESectionKind::text) {
            u64 const fillInst = INSN_TMPLT(undef);
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
Assembler::Write(std::ostream& out)
{
    ExeFileHeader header(m_Sections[ESectionId_code].size(),
                         m_Sections[ESectionId_rodata].size(),
                         m_Sections[ESectionId_data].size()
                         );

    header.write(out);
    for (const auto& section : m_Sections) {
        LOG_DEBUG(LOG_PARSER) << "section:" << section.size();
        if (section.size() > 0) {
            section.write(out);
        }
    }
}

void
Assembler::init_pass(EAssemblerPass passnum)
{
    LOG_DEBUG(LOG_PARSER) << "pass " << static_cast<int>(passnum);

    Tokenizer::InitPass();

    m_PassNumber = passnum;
    m_NopNumber = 0;
    m_InstructionNumber = 0;

    StartNewBundle();
}

int
Assembler::Assemble(std::istream& input, std::ostream& output, assemble_mode mode)
{
    Setup();
    Tokenizer::Init();
    TokenizeInput(input);
    init_pass(EAssemblerPass::compile);
    CompileFile();

    // compute section placement
    size_t global_offset = 0;
    for (auto& section : m_Sections) {
        section.global_offset = global_offset;
        global_offset += util::roundup<size_t>(section.size(), bits_per_segment_offset);
    }

    init_pass(EAssemblerPass::link);
    CompileFile();

    ExeFileHeader header(m_Sections[ESectionId_code].size(),
                         m_Sections[ESectionId_rodata].size(),
                         m_Sections[ESectionId_data].size());

    if (mode == assemble_mode_hex) {
        output << "\n\nunsigned const char program[] =\n{\n";
        // print_segment("head",   reinterpret_cast<unsigned const int *>(&header), sizeof(header), output);
        for (const auto& section : m_Sections) {
            if (section.size() > 0) {
                section.print("section", output);
            }
        }
        output << "\n/* end image */\n};\n";
    } else {
        header.write(output);
        for (auto& section : m_Sections) {
            if (section.size() > 0) {
                section.write(output);
            }
        }
    }
    LOG_DEBUG(LOG_PARSER) << "compilation complete";
    return 0;
}

} // namespace postrisc
