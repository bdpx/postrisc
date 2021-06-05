#include "arch/isa.hpp"
#include "Decoder.hpp"
#include "LabelMarker.hpp"
#include "binary_format/file_header.hpp"

namespace postrisc {

class as_label {
public:
    explicit as_label(const Decoder& _decoder, u64 _v, disassembler_mode _mode)
        : decoder(_decoder), v(_v), mode(_mode) {}

    friend std::ostream& operator<<(std::ostream& out, const as_label& r)
    {
        if (r.mode & disasm_mode_html)
            out << "<span class=\"label\">";
        out << r.decoder.get_address_info(r.v);
        // << std::hex << a.v << std::dec;
        if (r.mode & disasm_mode_html)
            out << "</span>";
        return out;
    }

private:
    const Decoder& decoder;
    u64 v;
    disassembler_mode mode;
};

/***************************************************************************
* assembler mnemonics for machine instructions
* initialize readonly tables for opcode->instruction_id conversion
***************************************************************************/
#define INSN_X(NAME, VALUE) \
   assert(arr[opcode_ ## NAME] == insn_illegal && #NAME); \
   arr[opcode_ ## NAME] = insn_ ## NAME;


#define LAMBDA_INITIALIZER(NBITS, XMACRO)                       \
    []() constexpr -> auto {                                    \
        TInstructionMnemonicTable<(1 << NBITS)> arr = {};       \
        for (size_t i = 0; i < (1 << NBITS); i++) {             \
            arr[i] = insn_illegal;                              \
        }                                                       \
        XMACRO(INSN_X)                                          \
        return arr;                                             \
    } ();

const Decoder::TInstructionMnemonicTable<(1 << bits_per_opcode)>       Decoder::arch_info_opcode   = LAMBDA_INITIALIZER(bits_per_opcode,       DECLARE_PRIMARY_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_misc_opx)>     Decoder::arch_info_misc     = LAMBDA_INITIALIZER(bits_per_misc_opx,     DECLARE_MISC_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_fused_opx)>    Decoder::arch_info_fused    = LAMBDA_INITIALIZER(bits_per_fused_opx,    DECLARE_FUSED_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_fused_opx)>    Decoder::arch_info_fma      = LAMBDA_INITIALIZER(bits_per_fused_opx,    DECLARE_FMA_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_register)>     Decoder::arch_info_raopx    = LAMBDA_INITIALIZER(bits_per_register,     DECLARE_RAOPX_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_loop     = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_LOOP_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_br_misc  = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_BR_MISC_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_br_32    = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_BR_32_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_br_64    = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_BR_64_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_br_128   = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_BR_128_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_nul_misc = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_NUL_MISC_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_nul_32   = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_NUL_32_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_nul_64   = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_NUL_64_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   Decoder::arch_info_nul_128  = LAMBDA_INITIALIZER(bits_per_branch_opx,   DECLARE_NUL_128_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_indexed_opx)>  Decoder::arch_info_mem_xi64 = LAMBDA_INITIALIZER(bits_per_indexed_opx,  DECLARE_MEM_XI64_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_indexed_opx)>  Decoder::arch_info_mem_xu64 = LAMBDA_INITIALIZER(bits_per_indexed_opx,  DECLARE_MEM_XU64_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_indexed_opx)>  Decoder::arch_info_mem_xi32 = LAMBDA_INITIALIZER(bits_per_indexed_opx,  DECLARE_MEM_XI32_EXTENDED_OPCODES);
const Decoder::TInstructionMnemonicTable<(1 << bits_per_indexed_opx)>  Decoder::arch_info_mem_xu32 = LAMBDA_INITIALIZER(bits_per_indexed_opx,  DECLARE_MEM_XU32_EXTENDED_OPCODES);

#undef INSN_X
#undef LAMBDA_INITIALIZER

const Decoder::regname_array Decoder::gpr_names =
    []() constexpr -> auto {
        Decoder::regname_array arr = {{
#define STACKED_NAME(NUM) {{ "r" #NUM }},
            STACKED_REGS(STACKED_NAME)
#define STATIC_NAME(NAME) {{ #NAME }},
            STATIC_REGS(STATIC_NAME)
        }};
        return arr;
    } ();

constexpr Decoder::regname_type get_regname_type(const std::string_view str)
{
    Decoder::regname_type regname = {};
    for (size_t i = 0; i < str.size(); i++) regname[i] = str[i];
    return regname;
}

const Decoder::regname_array Decoder::spr_names =
    []() constexpr -> auto {
        Decoder::regname_array arr = {};
#define SET_SPECIAL_REGISTER(value, name, type, regclass, descr) \
        arr[reg_##name] = get_regname_type(#name);

        DECLARE_SPECIAL_REGISTERS(SET_SPECIAL_REGISTER)
#undef SET_SPECIAL_REGISTER
        return arr;
    } ();

Decoder::Decoder()
{
    for (size_t i = 0; i < gpr_names.size(); i++) {
       m_GeneralRegisters[gpr_names[i].data()] = i;
    }

    //for (int i = 0; i < (1 << bits_per_register); i++) {
    //   std::string rname = std::string("sr") + std::to_string(i);
    //   strcpy(&spr_names[i][0], rname.c_str());
    //}

#define ADD_SPECIAL_REGISTER(value, name, type, regclass, descr) \
    /*strcpy(&spr_names[reg_##name][0], #name);*/           \
    m_SpecialRegisters[#name] = reg_##name;

    DECLARE_SPECIAL_REGISTERS(ADD_SPECIAL_REGISTER)
#undef ADD_SPECIAL_REGISTER

    for (int i = 0; i < insn_last; i++) {
       std::string cand(insn_name(static_cast<EInstructionID>(i)));
       if (asm_names.find(cand) != asm_names.end()) {
           std::cerr << "duplicate:: " << cand << std::endl;
       }
       asm_names[cand] = static_cast<EInstructionID>(i);
    }
    if (asm_names.size() != insn_last) {
        std::cerr << "asm_names: " << asm_names.size() << ", expected: " << insn_last << std::endl;
    }
    assert(asm_names.size() == insn_last);
}

Decoder::~Decoder()
{
    // to not inline
}

size_t Decoder::SaveName(char const *name)
{
    size_t const name_offset = m_symbol_names.size();
    // copy name into m_symbol_names store
    for(; *name; ++name) {
        m_symbol_names.push_back(*name);
    }
    m_symbol_names.push_back('\0');
    return name_offset;
}

void Decoder::add_symbol(u64 address, u64 size, char const *name)
{
    size_t const name_offset = SaveName(name);
    m_symbols.insert(std::make_pair(address, Symbol(address, size, name_offset)));
}

void Decoder::add_section_name(u64 address, u64 size, char const *name)
{
    size_t const name_offset = SaveName(name);
    m_sections.insert(std::make_pair(address, Symbol(address, size, name_offset)));
}

AddressInfo Decoder::get_section_info(u64 address) const
{
    auto it = m_sections.lower_bound(address);
    if (unlikely(it == m_sections.end())) {
        return AddressInfo(address, nullptr, 0);
    }
    /* address should be after device base address */
    assert(it->first <= address);
    if (unlikely(address >= it->first + it->second.size)) {
        return AddressInfo(address, nullptr, 0);
    }
    return AddressInfo(address, &m_symbol_names[it->second.name_offset], address - it->first);
}

AddressInfo Decoder::get_address_info(u64 address) const
{
    auto it = m_symbols.lower_bound(address);
    if (unlikely(it == m_symbols.end())) {
        return get_section_info(address);
    }
    /* address should be after device base address */
    assert(it->first <= address);
    if (unlikely(address >= it->first + it->second.size)) {
        return get_section_info(address);
    }
    return AddressInfo(address, &m_symbol_names[it->second.name_offset], address - it->first);
}

const std::string Decoder::get_insn_name(EInstructionID iid) const
{
    return std::string(insn_name(iid));
}

char const *Decoder::GetGprName(unsigned n)
{
    // LOG_DEBUG(LOG_PARSER) << "gpr_names: " << n << ": " << gpr_names[n] << std::endl;
    return gpr_names[n].data();
}

char const *Decoder::GetSprName(unsigned n)
{
    return spr_names[n].data();
}

int Decoder::GetGprIndex(char const *name)
{
    for (int i = 0; i < (int)gpr_names.size(); i++) {
        if (strcmp(name, gpr_names[i].data()) == 0) return i;
    }
    std::cerr << "unknown gpr register: " << name << std::endl;
    return -1;
}

int Decoder::GetSprIndex(char const *name)
{
    for (int i = 0; i < (int)spr_names.size(); i++) {
        if (strcmp(name, spr_names[i].data()) == 0) return i;
    }
    std::cerr << "unknown spr register: " << name << std::endl;
    return -1;
}

void Decoder::WriteRegister(std::ostream& out, unsigned regnum, disassembler_mode mode)
{
    if (mode & disasm_mode_html)
        out << "<span class=\"gpr\">";
    out << '%' << gpr_names[regnum].data();
    if (mode & disasm_mode_html)
        out << "</span>";
}

template<typename T>
class as_imm {
public:
    explicit as_imm(T _imm, disassembler_mode _mode) : imm(_imm), mode(_mode) {}

    friend std::ostream& operator<<(std::ostream& out, const as_imm a)
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

template<typename T> constexpr int size();
template<> constexpr int size<InstructionSingle>() { return 1; }
template<> constexpr int size<InstructionDouble>() { return 2; }

template<typename CMD>
void Decoder::DisassembleInstructionField(std::ostream& out,
    CMD cmd, u64 ip, const EInstructionField field, disassembler_mode mode) const
{
    switch (field) {
        case cf_lprefix:
            if constexpr (size<CMD>() == 2) {
                out << ".l";
            }
            break;
        case cf_comma: out << ','; break;
        case cf_argsdelimiter: out << ' '; break;

        case cf_gpra:  WriteRegister(out, cmd.ra(), mode);  break;
        case cf_gprb:  WriteRegister(out, cmd.rb(), mode);  break;
        case cf_gprc:  WriteRegister(out, cmd.rc(), mode);  break;
        case cf_gprd:  WriteRegister(out, cmd.rd(), mode);  break;

        case cf_spr:   out  << '%' << spr_names[cmd.rs()].data();  break;

        case cf_framesize:  out << as_imm(cmd.GetFrameSize(), mode);  break;

        case cf_shamt_b:   out << as_imm(cmd.get_shamt_b(), mode);  break;
        case cf_shamt_c:   out << as_imm(cmd.get_shamt_c(), mode);  break;
        case cf_shamt_d:   out << as_imm(cmd.get_shamt_d(), mode);  break;
        case cf_shamt_e:   out << as_imm(cmd.get_shamt_e(), mode);  break;

        case cf_mask_yes:        out << as_imm(cmd.get_nullify_yes(), mode);         break;
        case cf_mask_no:         out << as_imm(cmd.get_nullify_not(), mode);         break;
        case cf_indexed_disp:    out << as_imm(cmd.GetIndexedDisplacement(), mode);  break;
        case cf_indexed_scale:   out << as_imm(cmd.GetIndexedScale(), mode);         break;

        case cf_stride_simm:
        case cf_update_stride:   out << as_imm(cmd.GetUpdateStride(), mode);         break;
        case cf_ld_imm:          out << as_imm(cmd.get_ld_imm(), mode);              break;
        case cf_brc_simm:        out << as_imm(cmd.get_brc_simm(), mode);            break;
        case cf_brc_uimm:        out << as_imm(cmd.get_brc_uimm(), mode);            break;
        case cf_bin_simm:        out << as_imm(cmd.get_bin_simm(), mode);            break;
        case cf_bin_uimm:        out << as_imm(cmd.get_bin_uimm(), mode);            break;
        case cf_mid_bin_simm:    out << as_imm(cmd.get_mid_bin_simm(), mode);        break;
        case cf_loop_stride:     out << as_imm(1 + cmd.GetLoopStride(), mode);  break;

        case cf_ld_imm_f32:      out << as_imm(std::bit_cast<float>(static_cast<u32>(cmd.get_ld_imm())), mode); break;
        case cf_ld_imm_f64:      out << as_imm(std::bit_cast<double>(cmd.get_ld_imm()), mode); break;

        // labels
        case cf_lda_r:
        case cf_call:        out << as_label(*this, cmd.ComputeJumpRelative(ip), mode);  break;
        case cf_branch:      out << as_label(*this, cmd.ComputeBranchRelative(ip), mode);  break;
        case cf_loop_disp:   out << as_label(*this, cmd.ComputeLoopRelative(ip), mode);  break;

        case cf_rip_string:
        case cf_rip_label:   out << as_label(*this, cmd.ComputeForwardRelative(ip), mode);  break;

        case cf_comma_opt:
            break;

        default:
            out << "unknown cf_field " << field;
    } // end switch
}

template<typename CMD>
EInstructionID
Decoder::Disassemble(std::ostream& out, const CMD cmd, u64 ip, disassembler_mode mode) const
{
    const EInstructionID  mnem = Decode(cmd);

    if (mnem == insn_illegal) {
       out << "unknown instruction: " << cmd;
       return mnem;
    }

    const EInstructionAssemblerFormat fmt = asm_info[mnem].sw_format;

    if (mode & disasm_mode_html)
        out << "<span class=\"opc\">";
    out << insn_name(mnem);
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

// instantiate to use externally
template EInstructionID Decoder::Disassemble<InstructionSingle>(std::ostream& out, const InstructionSingle cmd, u64 ip, disassembler_mode mode) const;
template EInstructionID Decoder::Disassemble<InstructionDouble>(std::ostream& out, const InstructionDouble cmd, u64 ip, disassembler_mode mode) const;

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

template<typename CMD>
static void
PrintInsnSpecifiec(const CMD cmd, const EInstructionID mnem, std::ostream& out)
{
    const EInstructionFormat fmt = asm_info[mnem].hw_format;
    out << ';' << cmd
        << '\n'
        << ';' << inst_fmt_dump[fmt];

    if constexpr (size<CMD>() == 2) {
        out << ' ' << inst_ext_fmt_dump[inst_fmt_ext[fmt]];
    }
    out << '\n'
        << ';' << std::bitset<bits_per_slot>(cmd.GetRaw());

    if constexpr (size<CMD>() == 2) {
        out << ' ' << std::bitset<bits_per_slot>(cmd.GetRawExtension());
    }
    out << '\n';
}

template<typename CMD>
void
Decoder::PrintInstruction(const CMD cmd, std::ostream& out, u64 ip, unsigned slotNumber, DecoderContext& context) const
{
    // find current instruction
    bool const isCurrent = (slotNumber == context.restart_slot);

    // start context instruction numbering
    if (isCurrent) {
        context.insn_number = 0;
    }

    if (context.mode & disasm_mode_html) {
        if (isCurrent) {
            out << "<span class=\"current\">";
        }
        out << fmt::hex(ip)
            << ':' << static_cast<char>('A' + slotNumber);
        if (isCurrent) {
            out << "</span>";
        }
    }

    if (context.insn_number >= 0 && context.insn_number < bits_per_nullify_future) {
        if (context.mode & disasm_mode_html) {
            if (context.future_predication & 1) {
                out << "<span class=\"nullified\">";
                out << ":N";
                out << "</span>";
            }
        }
        out << ':' << context.insn_number;
        context.future_predication >>= 1;
    }

    context.insn_number++;

    out << ' ' << ' ';
    const EInstructionID mnem = Disassemble(out, cmd, ip, context.mode);
    out << '\n';
    if (context.mode & disasm_mode_dump) {
        PrintInsnSpecifiec(cmd, mnem, out);
    }
}

void Decoder::DisassembleBundle(const Bundle bundle, u64 ip, std::ostream& out, DecoderContext& context) const
{
    const EBundleTemplate tmplt = bundle.GetTemplate();

    if (context.mode & disasm_mode_dump) {
        out << "\n;";
        if (context.mode & disasm_mode_names) {
            out << get_address_info(ip);
        } else {
            out << fmt::hex(ip);
        }
        out << " bundle: " << bundle << '\n';
    } else if (context.mode & disasm_mode_names) {
        out << ";" << get_address_info(ip) << '\n';
    }

    switch (tmplt) {
    case EBundleTemplate::bundle_sss:
        PrintInstruction(bundle.s0(), out, ip, 0, context);
        PrintInstruction(bundle.s1(), out, ip, 1, context);
        PrintInstruction(bundle.s2(), out, ip, 2, context);
        break;

    case EBundleTemplate::bundle_sll:
        PrintInstruction(bundle.s0(), out, ip, 0, context);
        PrintInstruction(bundle.l1(), out, ip, 1, context);
        break;

    case EBundleTemplate::bundle_lls:
        PrintInstruction(bundle.l0(), out, ip, 0, context);
        PrintInstruction(bundle.s2(), out, ip, 2, context);
        break;

    case EBundleTemplate::bundle_lll:
        out << "invalid bundle template\n";
        break;
    }
}

void
Decoder::DisassembleDataSection(std::ostream& out, char const *section_name,
    u8 const data[], size_t section_len, u64 start_address, u64 address,
    const LabelMarker& labels, disassembler_mode mode) const
{
    out << section_name << "\n";
    for (size_t i = 0; i < section_len; i++) {
        if (labels.is_label(address + i)) {
            out << as_label(*this, start_address + address + i, mode) << ":\n";
        }
        u8 const dbval = data[i];
        out << "  d1 " << std::dec << +dbval;

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
Decoder::Disassemble(std::istream& in, std::ostream& out, disassembler_mode mode) const
{
    size_t          address;
    ExeFileHeader   header;
    const size_t    start_address = 0;

    header.read(in);

    //size_t  filelen = header.size_text + header.size_rodata + header.size_data;

    size_t const virt_size =
         util::roundup<u64>(header.size_text,   bits_per_segment_offset)
       + util::roundup<u64>(header.size_rodata, bits_per_segment_offset)
       + util::roundup<u64>(header.size_data,   bits_per_segment_offset);

    std::vector<u8> filedata(virt_size, 0);

    size_t virt_offset = 0;
    if (!in.read(reinterpret_cast<char *>(&filedata[virt_offset]), header.size_text))
       LOG_ABORT << "unable read code from file";

    virt_offset += util::roundup<u64>(header.size_text, bits_per_segment_offset);
    if (!in.read(reinterpret_cast<char *>(&filedata[virt_offset]), header.size_rodata))
       LOG_ABORT << "unable read rodata from file";

    virt_offset += util::roundup<u64>(header.size_rodata, bits_per_segment_offset);
    if (!in.read(reinterpret_cast<char *>(&filedata[virt_offset]), header.size_data))
       LOG_ABORT << "unable read data from file";

    LabelMarker labels(virt_size);
    labels.MarkLabels(*this, reinterpret_cast<Bundle *>(&filedata[0]), header.size_text, virt_size);

    out << "; disassembled\n"
           "; text:   " << header.size_text   << " bytes\n"
           "; rodata: " << header.size_rodata << " bytes\n"
           "; data:   " << header.size_data   << " bytes\n";

    out << ".text\n";

    if (mode == disasm_mode_dump)
        out << "; bitfields description:\n"
          ";\n"
          "; o    - primary opcode (7 bit - 128 opcodes)\n"
          "; x    - extended opcode (variable bits)\n"
          "; abcd - register number (7 bit - 128 registers per file)\n"
          "; i    - immediate value (variable bits) sign or zero extended\n"
          "; s    - shift, bit count or bit position (7 bits - 0..127)\n"
          "; k    - special purpose register\n"
          "; -    - not used bits, must be zero\n\n";

    assert(header.size_text % sizeof(Bundle) == 0);

    DecoderContext decoderContext(mode);
    for (address = 0; address < header.size_text; address += sizeof(Bundle)) {
        u64 const ip = start_address + address;
        if (labels.is_label(address)) {
            out << as_label(*this, ip, mode) << ":\n";
        }

        Bundle const bundle = *reinterpret_cast<const Bundle*>(filedata.data() + address);
        DisassembleBundle(bundle, ip, out, decoderContext);
    }

    const size_t rodata_offset = util::roundup<u64>(header.size_text, bits_per_segment_offset);
    const u8 *data = &filedata[rodata_offset];
    address = rodata_offset;

    size_t const size_rodata = header.size_rodata;
    if (size_rodata > 0) {
        DisassembleDataSection(out, ".rodata", data, size_rodata, start_address, address, labels, mode);
        size_t const size_in_memory = util::roundup<u64>(size_rodata, bits_per_segment_offset);
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
