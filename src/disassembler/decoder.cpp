#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "fpu/float.hpp"
#include "decoder.hpp"
#include "label_marker.hpp"
#include "binary_format/file_header.hpp"

namespace postrisc {

class as_label {
public:
    explicit as_label(uint64_t _v, disassembler_mode _mode)
        : v(_v), mode(_mode) {}

    friend std::ostream & operator << (std::ostream & out, const as_label a)
    {
        if (a.mode & disasm_mode_html)
            out << "<span class=\"label\">";
        out << 'L' << std::hex << a.v << std::dec;
        if (a.mode & disasm_mode_html)
            out << "</span>";
        return out;
    }

private:
    uint64_t v;
    disassembler_mode mode;
};

enum { INSTRUCTION_OFFSET = 12 };

/***************************************************************************
* assembler mnemonics for machine instructions
***************************************************************************/
const Decoder::TInstructionMnemonicTable<(1 << bits_per_opcode)>       Decoder::arch_info_opcode =  { DECLARE_PRIMARY_OPCODES(INSN_X) };
const Decoder::TInstructionMnemonicTable<(1 << bits_per_misc_opx)>     Decoder::arch_info_misc   =  { DECLARE_MISC_EXTENDED_OPCODES(INSN_X) };
const Decoder::TInstructionMnemonicTable<(1 << bits_per_fused_opx)>    Decoder::arch_info_fused  =  { DECLARE_FUSED_EXTENDED_OPCODES(INSN_X) };
const Decoder::TInstructionMnemonicTable<(1 << bits_per_register)>     Decoder::arch_info_raopx   = { DECLARE_RAOPX_EXTENDED_OPCODES(INSN_X) };
const Decoder::TInstructionMnemonicTable<(1 << bits_per_nullify_opx)>  Decoder::arch_info_nullify = { DECLARE_NULLIFY_EXTENDED_OPCODES(INSN_X) };

Decoder::Decoder()
{
    for (int i = 0; i < max_stacked_framesize; i++) {
       std::string rname = std::string("r") + std::to_string(i);
       const int real_index = base_rotate_regs + i;
       gpr_names[real_index] = rname;
       m_GeneralRegisters[rname] = real_index;
    }

    for (int i = 0; i < num_static_regs; i++) {
       std::string rname = std::string("g") + std::to_string(i);
       const int real_index = base_static_regs + i;
       gpr_names[real_index] = rname;
       m_GeneralRegisters[rname] = real_index;
    }

    gpr_names[reg_sp] = "sp";
    gpr_names[reg_fp] = "fp";
    gpr_names[reg_tp] = "tp";
    gpr_names[reg_gz] = "gz";

    m_GeneralRegisters["sp"] = reg_sp;
    m_GeneralRegisters["fp"] = reg_fp;
    m_GeneralRegisters["tp"] = reg_tp;
    m_GeneralRegisters["gz"] = reg_gz;

#define SPECIAL_REGISTER_NAME_X(name, regtype, descr) spr_names[reg_##name] = #name;
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_NAME_X)
#undef SPECIAL_REGISTER_NAME_X

    for (int i = reg_spr_last; i < (1<<bits_per_register); i++) {
       std::string rname = std::string("sr") + std::to_string(i);
       spr_names[i] = rname;
    }

#define ADD_SPECIAL_REGISTER(name, regclass, descr) \
    m_SpecialRegisters[spr_names[reg_##name]] = reg_##name;

    DECLARE_SPECIAL_REGISTERS(ADD_SPECIAL_REGISTER)
#undef ADD_SPECIAL_REGISTER

    for (int i = 1; i < insn_last; i++) {
       if (std::strcmp(asm_name[i-1], asm_name[i]) >= 0) {
          std::cerr << "finded unsorted commands: " << asm_name[i-1] << ' ' << asm_name[i] << ' ' << i << '\n';
       }
    }

    for (int i = 0; i < insn_last; i++) {
       asm_names[std::string(asm_name[i])] = i;
    }
}

Decoder::~Decoder()
{
    // to not inline
}

const std::string
Decoder::get_insn_name(EInstructionID iid) const
{
    return std::string(asm_name[iid]);
}

EInstructionID
Decoder::Decode(CInsn1 cmd)
{
    unsigned         opx;
    EInstructionID   mnem;
    EInstructionOpcode const opcode = static_cast<EInstructionOpcode>(cmd.GetOpcode());

    switch (opcode) {
        case EInstructionOpcode::opcode_misc:
            opx = static_cast<unsigned>(cmd.GetMiscOpx());
            mnem = arch_info_misc[opx];
            break; 
        case EInstructionOpcode::opcode_fused:
            opx = static_cast<unsigned>(cmd.GetFusedOpx());
            mnem = arch_info_fused[opx];
            break;
        case EInstructionOpcode::opcode_raopx:
            opx = static_cast<unsigned>(cmd.GetRaOpx());
            mnem = arch_info_raopx[opx];
            break; 
        case EInstructionOpcode::opcode_nullify:
            opx = static_cast<unsigned>(cmd.GetNullifyOpx());
            mnem = arch_info_nullify[opx];
            break; 
        default:
            assert(opcode < arch_info_opcode.size());
            mnem = arch_info_opcode[opcode];
            break;
    }

    assert(mnem < insn_last);
    return mnem;
}

void
Decoder::WriteRegister(std::ostream & out, unsigned regnum, disassembler_mode mode) const
{
    if (mode & disasm_mode_html)
        out << "<span class=\"gpr\">";
    out << gpr_names[regnum];
    if (mode & disasm_mode_html)
        out << "</span>";
}

template<class T>
class as_imm
{
public:
    explicit as_imm(T _imm, disassembler_mode _mode) : imm(_imm), mode(_mode) {}

    friend std::ostream & operator << (std::ostream & out, const as_imm a)
    {
        if (a.mode & disasm_mode_html)
            out << "<span class=\"imm\">";
        out << a.imm;
        if (a.mode & disasm_mode_html)
            out << "</span>";
        return out;
    }

private:
    T imm;
    disassembler_mode mode;
};

/***************************************************************************
* this function disassemble short instruction
***************************************************************************/
static
void
print_memory_ordering(std::ostream & out, EMemoryOrdering order)
{
    switch (order) {
        case order_relaxed:          out << ", relaxed";   break;
        case order_acquire:          out << ", acquire";   break;
        case order_release:          out << ", release";   break;
        case order_acquire_release:  out << ", acqrel";    break;

        default:
            LOG_ABORT << "unknown memory ordering " << fmt::dec(order);
            break;
    }
}

static
void
PrintFpCompareMode(std::ostream & out, fpu::compare_mode_t fpmode, disassembler_mode mode)
{
    static const struct {
        fpu::compare_mode_t  mode;
        char                 name[7];
    } data[] = {
        { fpu::FLOAT_COMPARE_LESS,      "LT"  },
        { fpu::FLOAT_COMPARE_GREATER,   "GT"  },
        { fpu::FLOAT_COMPARE_EQUAL,     "EQ"  },
        { fpu::FLOAT_COMPARE_UNORDERED, "UN"  },
        { fpu::FLOAT_COMPARE_SIGNALED,  "SIG" },
    };

    bool first = true;
    for (size_t i=0; i<sizearray(data); i++) {
        if (fpmode & data[i].mode) {
            out << (first ? " " : " | ");
            out << data[i].name;
            first = false;
        }
    }

    if (first) {
        out << " " << as_imm(fpmode, mode);
    }
}

template<class CMD>
void
Decoder::DisassembleInstructionField(std::ostream& out, CMD cmd, uint64_t ip,
    const EInstructionField field, disassembler_mode mode) const
{
    switch (field) {
        case cf_lprefix:  break;
        case cf_comma:    out << ','; break;

        case cf_gpra:  WriteRegister(out, cmd.ra(), mode);  break;
        case cf_gprb:  WriteRegister(out, cmd.rb(), mode);  break;
        case cf_gprc:  WriteRegister(out, cmd.rc(), mode);  break;
        case cf_gprd:  WriteRegister(out, cmd.rd(), mode);  break;

        case cf_spr:   out << spr_names[cmd.rs()];  break;

        case cf_shamt_b:   out << as_imm(cmd.get_shamt_b(), mode);  break;
        case cf_shamt_c:   out << as_imm(cmd.get_shamt_c(), mode);  break;
        case cf_shamt_d:   out << as_imm(cmd.get_shamt_d(), mode);  break;
        case cf_shamt_e:   out << as_imm(cmd.get_shamt_e(), mode);  break;

        case cf_vshamt_b:  out << as_imm(cmd.rb(), mode);   break;
        case cf_vshamt_c:  out << as_imm(cmd.rc(), mode);   break;
        case cf_vshamt_d:  out << as_imm(cmd.rd(), mode);   break;

        case cf_mask_yes:        out << as_imm(cmd.get_nullify_yes(), mode);         break;
        case cf_mask_no:         out << as_imm(cmd.get_nullify_not(), mode);         break;
        case cf_indexed_disp:    out << as_imm(cmd.GetIndexedDisplacement(), mode);  break;
        case cf_indexed_scale:   out << as_imm(cmd.GetIndexedScale(), mode);         break;
        case cf_uimm14:          out << as_imm(cmd.GetUintStride(), mode);           break;
        case cf_update_stride:   out << as_imm(cmd.GetUpdateStride(), mode);         break;
        case cf_ldi:             out << as_imm(cmd.get_ldi(), mode);                 break;
        case cf_brc_simm:        out << as_imm(cmd.get_brc_simm(), mode);            break;
        case cf_brc_uimm:        out << as_imm(cmd.get_brc_uimm(), mode);            break;
        case cf_binimm:          out << as_imm(cmd.get_bin_simm(), mode);            break;
        case cf_binimmu:         out << as_imm(cmd.get_bin_uimm(), mode);            break;
        case cf_clmul_index:     out << as_imm(cmd.GetIndexedScale(), mode);         break;
        case cf_rounding_mode:   out << as_imm(cmd.GetRoundingMode(), mode);         break;
        case cf_loop_step:       out << as_imm(cmd.GetLoopStep(), mode);             break;

        case cf_flddi:           out << as_imm(util::change_bit_presentation<uint64_t, double>
                                        (cmd.get_ldi()), mode); break;

        case cf_fp_compare_mode:
            PrintFpCompareMode(out, cmd.GetFpCompareMode(), mode);
            break;

        // labels
        case cf_ldar:
        case cf_branch:         out << as_label(cmd.ComputeJumpRelative(ip), mode);   break;
        case cf_brc_branch:     out << as_label(cmd.ComputeBranchRelative(ip), mode);   break;
        case cf_short_branch:   out << as_label(cmd.ComputeShortBranchRelative(ip), mode);  break;

        case cf_rip_string:
        case cf_rip_label:      out << as_label( cmd.ComputeForwardRelative(ip), mode);     break;

        case cf_comma_opt:
            break;
        case cf_memory_ordering:
            print_memory_ordering(out, cmd.GetMemoryOrdering());
            break;

        default:
            out << "unknown cf_field " << field;

    } // end switch
}

EInstructionID
Decoder::Disassemble(std::ostream& out, const CInsn1 cmd, uint64_t ip, disassembler_mode mode) const
{
    const EInstructionID  mnem = Decode(cmd);

    if (mnem == insn_undef &&
        !(cmd.GetOpcode() == EInstructionOpcode::opcode_raopx && cmd.GetRaOpx() == opcode_undef))
    {
       out << "unknown instruction: " << cmd;
       return mnem;
    }

    const EInstructionAssemblerFormat fmt = asm_info[mnem].sw_format;

    if (mode & disasm_mode_html)
        out << "<span class=\"opc\">";
    if (asm_fmt_field[fmt][0] != cf_none)
       out << std::left << std::setfill(' ') << std::setw(INSTRUCTION_OFFSET);
    out << asm_name[mnem];
    if (mode & disasm_mode_html)
        out << "</span>";

    out << std::right;

    for (const EInstructionField field : asm_fmt_field[fmt]) {
        if (field == cf_none)
           break;

        DisassembleInstructionField(out, cmd, ip, field, mode);
    }

    return mnem;
}

/***************************************************************************
* this function disassemble long instruction
***************************************************************************/
EInstructionID
Decoder::Disassemble(std::ostream& out, const CInsn2 cmd, uint64_t ip, disassembler_mode mode) const
{
    const EInstructionID  mnem = Decode(cmd);

    if (mnem == insn_undef && cmd.GetRaw() != INSN_TMPLT(undef)) {
       out << "unknown instruction: " << cmd;
       return mnem;
    }

    const EInstructionAssemblerFormat fmt = asm_info[mnem].sw_format;

    assert(asm_fmt_field[fmt][0] == cf_lprefix);

    if (mode & disasm_mode_html)
        out << "<span class=\"opc\">";
    out << asm_name[mnem];
    out << std::left << std::setfill(' ')
        << std::setw(INSTRUCTION_OFFSET - std::strlen(asm_name[mnem]))
        << ".l";
    if (mode & disasm_mode_html)
        out << "</span>";

    out << std::right;

    for (const EInstructionField field : asm_fmt_field[fmt]) {
        if (field == cf_none)
           break;
        DisassembleInstructionField(out, cmd, ip, field, mode);
    }

    return mnem;
}


/***************************************************************************
* this function dump instruction in binary form
***************************************************************************/
// add zero byte and 8-byte padding
static const char inst_fmt_dump[inst_fmt_last][bits_per_slot + 6] =
{
#define INSTRUCTION_FORMAT_DESCRIPTION_X(name, extname, descr, format, txt) descr,
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_DESCRIPTION_X)
};

static const EInstructionExtensionFormat inst_fmt_ext[inst_fmt_last] =
{
#define INSTRUCTION_FORMAT_EXTNAME_X(name, extname, descr, format, txt) inst_ext_fmt_##extname,
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_EXTNAME_X)
};

static const char inst_ext_fmt_dump[inst_ext_fmt_last][bits_per_slot + 6] =
{
#define INSTRUCTION_EXTENSION_FORMAT_BITS_X(name, bitdescr, txt) bitdescr,
    DECLARE_INSTRUCTION_EXTENSION_FORMATS(INSTRUCTION_EXTENSION_FORMAT_BITS_X)
};

void
Decoder::PrintInstruction(const CInsn1 cmd, std::ostream &out, uint64_t ip,
    disassembler_mode mode, int insnNumber, int restartInsnNumber) const
{
    if ((mode & disasm_mode_html) && insnNumber == restartInsnNumber) {
        out << "<span class=\"current\">";
    }

    if (mode & disasm_mode_html) {
        out << fmt::hex(ip-sizeof(CBundle)) << "[" << insnNumber << "]";
    }

    out << '\t';
    const EInstructionID mnem = Disassemble(out, cmd, ip, mode);

    if ((mode & disasm_mode_html) && insnNumber == restartInsnNumber) {
        out << "</span>";
    }
    out << '\n';

    if (mode & disasm_mode_dump) {
        const EInstructionFormat fmt = asm_info[mnem].hw_format;
        out << ';' << cmd
            << '\n'
            << ';' << inst_fmt_dump[fmt]
            << '\n'
            << ';' << std::bitset<bits_per_slot>(cmd.GetRaw())
            << '\n';
    }
}

void
Decoder::PrintInstruction(const CInsn2 cmd, std::ostream &out, uint64_t ip,
    disassembler_mode mode, int insnNumber, int restartInsnNumber) const
{
    if ((mode & disasm_mode_html) && insnNumber == restartInsnNumber) {
        out << "<span class=\"current\">";
    }

    if (mode & disasm_mode_html) {
        out << fmt::hex(ip-sizeof(CBundle)) << "[" << insnNumber << "]";
    }

    out << '\t';
    const EInstructionID mnem = Disassemble(out, cmd, ip, mode);

    if ((mode & disasm_mode_html) && insnNumber == restartInsnNumber) {
        out << "</span>";
    }
    out << '\n';

    if (mode & disasm_mode_dump) {
        const EInstructionFormat fmt = asm_info[mnem].hw_format;
        out << ';' << cmd
            << '\n'
            << ';' << inst_fmt_dump[fmt]
            << ' ' << inst_ext_fmt_dump[inst_fmt_ext[fmt]]
            << '\n'
            << ';' << std::bitset<bits_per_slot>(cmd.GetRaw())
            << ' ' << std::bitset<bits_per_slot>(cmd.GetRawExtension())
            << '\n';
    }
}

void
Decoder::Disassemble(const CBundle bundle, uint64_t ip, std::ostream& out,
    disassembler_mode mode, unsigned restartInstructionNumber) const
{
    const EBundleTemplate tmplt = bundle.GetTemplate();

    if (mode & disasm_mode_dump) {
       out << "\n;ip=" << fmt::hex(ip - sizeof(CBundle))
           << ": tmplt=" << static_cast<unsigned>(tmplt)
           << '\n'
           << ';' << bundle
           << '\n';
    }

    switch (tmplt) {
        case EBundleTemplate::bundle_sss:
            PrintInstruction(bundle.s0(), out, ip, mode, 1, restartInstructionNumber);
            PrintInstruction(bundle.s1(), out, ip, mode, 2, restartInstructionNumber);
            PrintInstruction(bundle.s2(), out, ip, mode, 3, restartInstructionNumber);
            break;

        case EBundleTemplate::bundle_lls:
            PrintInstruction(bundle.l0(), out, ip, mode, 1, restartInstructionNumber);
            PrintInstruction(bundle.s2(), out, ip, mode, 2, restartInstructionNumber);
            break;

        case EBundleTemplate::bundle_sll:
            PrintInstruction(bundle.s0(), out, ip, mode, 1, restartInstructionNumber);
            PrintInstruction(bundle.l1(), out, ip, mode, 2, restartInstructionNumber);
            break;

        default:
            out << "invalid bundle template\n";
            break;
    }
}

void
Decoder::DisassembleDataSection(std::ostream &out, char const *section_name,
    uint8_t const data[], size_t section_len, uint64_t start_address, uint64_t address,
    const LabelMarker& labels, disassembler_mode mode) const
{
    out << section_name << "\n";
    for (size_t i = 0; i < section_len; i++) {
        if (labels.is_label(address + i)) {
            out << as_label(start_address + address + i, mode) << ":\n";
        }
        uint8_t const dbval = data[i];
        out << "\td1\t" << std::dec << +dbval;

        if (mode == disasm_mode_dump) {
            if (std::isprint(dbval))
                out << "\t; '" << static_cast<char>(dbval) << '\'';
            else
                out << "\t; 0x" << fmt::hex(dbval);
        }
        out << '\n';
    }
}

/***************************************************************************
* this function disassemble input stream
***************************************************************************/
int
Decoder::Disassemble(std::istream &in, std::ostream &out, disassembler_mode mode) const
{
    size_t          address;
    CExeFileHeader  header;
    const size_t    start_address = 0;

    header.read(in);

    //size_t  filelen = header.size_text + header.size_rodata + header.size_data;

    size_t const virt_size =
         util::roundup(header.size_text,   bits_per_segment_offset)
       + util::roundup(header.size_rodata, bits_per_segment_offset)
       + util::roundup(header.size_data,   bits_per_segment_offset);

    std::vector<uint8_t> filedata(virt_size, 0);

    size_t virt_offset = 0;
    if (!in.read(reinterpret_cast<char *>(&filedata[virt_offset]), header.size_text))
       LOG_ABORT << "unable read code from file";

    virt_offset += util::roundup(header.size_text, bits_per_segment_offset);
    if (!in.read(reinterpret_cast<char *>(&filedata[virt_offset]), header.size_rodata))
       LOG_ABORT << "unable read rodata from file";

    virt_offset += util::roundup(header.size_rodata, bits_per_segment_offset);
    if (!in.read(reinterpret_cast<char *>(&filedata[virt_offset]), header.size_data))
       LOG_ABORT << "unable read data from file";

    LabelMarker labels(virt_size);
    labels.MarkLabels(*this, reinterpret_cast<CBundle *>(&filedata[0]), header.size_text, virt_size);

    out << "; disassembled\n"
           "; text:   " << header.size_text   << " bytes\n"
           "; rodata: " << header.size_rodata << " bytes\n"
           "; data:   " << header.size_data   << " bytes\n";

    out << ".text\n";

    if (mode == disasm_mode_dump)
        out << "; bitfields description:\n"
          ";\n"
          "; o\t- primary opcode (8 bit - 256 opcodes)\n"
          "; x\t- extended opcode (variable bits)\n"
          "; abcd\t- register number (7 bit - 128 registers per file)\n"
          "; i\t- immediate value (variable bits) sign or zero extended\n"
          "; s\t- shift, bit count or bit position (6 bits - 0..63)\n"
          "; k\t- special purpose register\n"
          "; -\t- not used bits, must be zero\n\n";

    for (address=0; address < header.size_text; address += sizeof(CBundle)) {
        uint64_t const current_address = start_address + address;
        if (labels.is_label(address)) {
            out << as_label(current_address, mode) << ":\n";
        }

        CBundle const bundle = *reinterpret_cast<const CBundle*>(filedata.data() + address);
        uint64_t const ip = current_address + sizeof(CBundle);
        Disassemble(bundle, ip, out, mode);
    }

    const size_t rodata_offset = util::roundup(header.size_text, bits_per_segment_offset);
    const uint8_t *data = &filedata[rodata_offset];
    address = rodata_offset;

    size_t const size_rodata = header.size_rodata;
    if (size_rodata > 0) {
        DisassembleDataSection(out, ".rodata", data, size_rodata, start_address, address, labels, mode);
        size_t const size_in_memory = util::roundup(size_rodata, bits_per_segment_offset);
        data += size_in_memory;
        address += size_in_memory;
    }

    if (header.size_data > 0) {
        DisassembleDataSection(out, ".data", data, header.size_data, start_address, address, labels, mode);
    }

    out << ".end\n";
    return 0;
}

} // namespace postrisc
