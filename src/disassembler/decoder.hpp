#pragma once

#include <map>

#include "arch/isa.hpp"
#include "arch/instructions.hpp"

namespace postrisc {

/***************************************************************************
* flags for disassembling mods
***************************************************************************/
enum disassembler_mode {
    disasm_mode_std   = 0,
    disasm_mode_dump  = 1,
    disasm_mode_html  = 2,
    disasm_mode_names = 4,
};

DEFINE_ENUM_FLAG_OPERATOR_SET(disassembler_mode)

using keywordmap = std::map<std::string, u32>;

class LabelMarker;

class Symbol {
friend class Decoder;
public:
    Symbol(u64 _address, u64 _size, size_t _name_offset)
        : address(_address), size(_size), name_offset(_name_offset) {}

    bool operator>(const Symbol& r) const { return address > r.address; }

private:
    u64      address;
    u64      size;
    size_t   name_offset;
};

class DecoderContext {
public:
    explicit DecoderContext(disassembler_mode _mode) : mode(_mode) {}
    static unsigned const unmatched = 4; // unmatched never match with 0,1,2 instruction numbers
public:
    disassembler_mode  mode;
    unsigned           ri = DecoderContext::unmatched;
    i64                insn_number = std::numeric_limits<i64>::min();
    u32                future_predication = 0;
};

class Decoder {
public:
    explicit Decoder(void);
    ~Decoder(void);

    int GenerateHtml(std::ostream& out);
    int LLVMTableGenGenerateEncoding(std::ostream& out) const;

    static void WriteRegister(std::ostream& out, unsigned regnum, disassembler_mode mode);
    static char const *GetGprName(unsigned n);
    static char const *GetSprName(unsigned n);
    static int GetGprIndex(char const *);
    static int GetSprIndex(char const *);

    template<size_t N> using TInstructionMnemonicTable = std::array<EInstructionID, N>;

    template<class CMD> static EInstructionID Decode(CMD cmd);
    int Disassemble(std::istream& in, std::ostream& out, disassembler_mode mode) const;

    template<class CMD>
    EInstructionID Disassemble(std::ostream& out, const CMD cmd, u64 ip, disassembler_mode mode) const;

    void DisassembleBundle(const CBundle bundle, u64 ip, std::ostream& out,
              DecoderContext& decoderContext) const;

    const std::string get_insn_name(EInstructionID) const;

    void add_symbol(u64 address, u64 size, char const *name);
    void add_section_name(u64 address, u64 size, char const *name);
    void print_address_info(u64 address, std::ostream& out) const;

private:
    size_t SaveName(char const *name);
    void print_section_info(u64 address, std::ostream& out) const;
    template<class CMD>
    void DisassembleInstructionField(std::ostream& out, bool last,
        CMD cmd, u64 ip, const EInstructionField field, disassembler_mode mode) const;

    template<class CMD>
    void PrintInstruction(const CMD cmd, std::ostream& out, u64 ip,
                          unsigned bundle_insnNumber, DecoderContext& context) const;

    void DisassembleDataSection(std::ostream& out, char const *section_name,
            u8 const data[], size_t section_len, u64 start_address, u64 address,
            const LabelMarker& labels, disassembler_mode mode) const;

    void tablegen_general_registerfile(std::ostream& out) const;
    void tablegen_special_registerfile(std::ostream& out) const;

public:
    // primary op-codes
    static const TInstructionMnemonicTable<(1 << bits_per_opcode)>     arch_info_opcode;

    // extended op-codes
    static const TInstructionMnemonicTable<(1 << bits_per_misc_opx)>     arch_info_misc;
    static const TInstructionMnemonicTable<(1 << bits_per_fused_opx)>    arch_info_fused;
    static const TInstructionMnemonicTable<(1 << bits_per_register)>     arch_info_raopx;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_brr;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_loop;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_bfss;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_bfsd;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_bfsq;
    // extended subcodes of opcode_fused: nullrr, nulfss, nulfsd, nulfsq
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_nullrr;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_nulfss;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_nulfsd;
    static const TInstructionMnemonicTable<(1 << bits_per_branch_opx)>   arch_info_nulfsq;

    using regname_array = std::array<char [8], (1 << bits_per_register)>;

    static regname_array gpr_names;
    static regname_array spr_names;

public:
    using register_names_table = std::array<std::string, (1 << bits_per_register)>;

    keywordmap                          m_GeneralRegisters;
    keywordmap                          m_SpecialRegisters;
    keywordmap                          asm_names;

private:
    std::map<u64, Symbol, std::greater<u64>>     m_symbols;
    std::map<u64, Symbol, std::greater<u64>>     m_sections;
    std::vector<char>                            m_symbol_names;
};

template<class CMD> EInstructionID Decoder::Decode(CMD cmd)
{
    unsigned opx;
    EInstructionOpcode const opcode = static_cast<EInstructionOpcode>(cmd.GetOpcode());

    switch (opcode) {
        case EInstructionOpcode::opcode_misc:
            return arch_info_misc[ cmd.GetMiscOpx() ];
        case EInstructionOpcode::opcode_fused:
            opx = cmd.GetFusedOpx();
            switch (opx) {
            case opcode_nullrr:  return arch_info_nullrr[ cmd.GetBranchOpx() ];
            case opcode_nulfss:  return arch_info_nulfss[ cmd.GetBranchOpx() ];
            case opcode_nulfsd:  return arch_info_nulfsd[ cmd.GetBranchOpx() ];
            case opcode_nulfsq:  return arch_info_nulfsq[ cmd.GetBranchOpx() ];
            default:             return arch_info_fused[opx];
            }
            break;
        case EInstructionOpcode::opcode_raopx: return arch_info_raopx[cmd.GetRaOpx()];
        case EInstructionOpcode::opcode_brr:   return arch_info_brr[ cmd.GetBranchOpx() ];
        case EInstructionOpcode::opcode_loop:  return arch_info_loop[ cmd.GetBranchOpx() ];
        case EInstructionOpcode::opcode_bfss:  return arch_info_bfss[ cmd.GetBranchOpx() ];
        case EInstructionOpcode::opcode_bfsd:  return arch_info_bfsd[ cmd.GetBranchOpx() ];
        case EInstructionOpcode::opcode_bfsq:  return arch_info_bfsq[ cmd.GetBranchOpx() ];
        default:
            assert(opcode < arch_info_opcode.size());
            return arch_info_opcode[opcode];
    }
    //assert(mnem < insn_last);
    //return mnem;
}

/***************************************************************************
* instruction disassemble helper
***************************************************************************/
template<class CMD>
class CInstructionDisassembler {
public:
    CInstructionDisassembler(const Decoder& _decoder, CMD cmd, u64 _ip)
        : decoder(_decoder)
        , instruction(cmd)
        , ip(_ip)
    {}

    friend std::ostream& operator<<(std::ostream& out, const CInstructionDisassembler& r)
    {
        r.decoder.Disassemble(out, r.instruction, r.ip, disasm_mode_std);
        return out;
    }

private:
    const Decoder& decoder;
    CMD            instruction;
    u64            ip;           // context for label naming
};

// formatting manipulator for address info dumping
namespace fmt {
class addr_info {
public:
    addr_info(const Decoder& _decoder, u64 _address) : decoder(_decoder), address(_address) {}
    friend std::ostream& operator<<(std::ostream& out, const addr_info& r) {
        r.decoder.print_address_info(r.address, out);
        return out;
    }
private:
    const Decoder&  decoder;
    u64             address;
};
} // namespace fmt

} // namespace postrisc
