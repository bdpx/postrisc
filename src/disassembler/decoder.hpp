#pragma once

#include <map>

#include "arch/isa.hpp"
#include "arch/instructions.hpp"

namespace postrisc {

/***************************************************************************
* flags for disassembling mods
***************************************************************************/
enum disassembler_mode {
    disasm_mode_std  = 0,
    disasm_mode_dump = 1,
    disasm_mode_html = 2
};

DEFINE_ENUM_FLAG_OPERATOR_SET(disassembler_mode)

using keywordmap = std::map<std::string, uint32_t>;

class LabelMarker;

class Decoder {
public:
    explicit Decoder(void);
    ~Decoder(void);

    void WriteRegister(std::ostream & out, unsigned regnum, disassembler_mode mode) const;

    template<int N> using TInstructionMnemonicTable =
        std::array<util::default_initializer<EInstructionID, insn_undef>, N>;

    static EInstructionID Decode(CInsn1 cmd);
    int Disassemble(std::istream& in, std::ostream& out, disassembler_mode mode) const;

    EInstructionID Disassemble(std::ostream& out, const CInsn1 cmd, uint64_t ip, disassembler_mode mode) const;
    EInstructionID Disassemble(std::ostream& out, const CInsn2 cmd, uint64_t ip, disassembler_mode mode) const;
    void Disassemble(const CBundle bundle, uint64_t ip, std::ostream& out,
                     disassembler_mode mode, unsigned restartInstructionNumber = 0) const;

    const std::string get_insn_name(EInstructionID) const;

private:
    template<class CMD>
    void DisassembleInstructionField(std::ostream& out,
        CMD cmd, uint64_t ip, const EInstructionField field, disassembler_mode mode) const;

    void PrintInstruction(const CInsn1 cmd, std::ostream &out, uint64_t ip,
                          disassembler_mode mode, int insnNumber, int restartInsnNumber) const;
    void PrintInstruction(const CInsn2 cmd, std::ostream &out, uint64_t ip,
                          disassembler_mode mode, int insnNumber, int restartInsnNumber) const;

    void DisassembleDataSection(std::ostream &out, char const *section_name,
            uint8_t const data[], size_t section_len, uint64_t start_address, uint64_t address,
            const LabelMarker& labels, disassembler_mode mode) const;

public:
    // primary op-codes
    static const TInstructionMnemonicTable<(1 << bits_per_opcode)>     arch_info_opcode;

    // extended op-codes
    static const TInstructionMnemonicTable<(1 << bits_per_misc_opx)>     arch_info_misc;
    static const TInstructionMnemonicTable<(1 << bits_per_fused_opx)>    arch_info_fused;
    static const TInstructionMnemonicTable<(1 << bits_per_register)>     arch_info_raopx;
    static const TInstructionMnemonicTable<(1 << bits_per_nullify_opx)>  arch_info_nullify;

public:
    using register_names_table = std::array<std::string, (1 << bits_per_register)>;

    register_names_table                gpr_names;
    register_names_table                spr_names;
    keywordmap                          m_GeneralRegisters;
    keywordmap                          m_SpecialRegisters;
    keywordmap                          asm_names;
};

/***************************************************************************
* instruction disassemble helper
***************************************************************************/
template<class CMD> class CInstructionDisassembler {
public:
    CInstructionDisassembler(const Decoder& _decoder, CMD cmd, uint64_t _ip)
        : decoder(_decoder)
        , instruction(cmd)
        , ip(_ip)
    {}

    friend std::ostream & operator << (std::ostream& out, const CInstructionDisassembler& r)
    {
        r.decoder.Disassemble(out, r.instruction, r.ip, disasm_mode_std);
        return out;
    }

private:
    const Decoder& decoder;
    CMD            instruction;
    uint64_t       ip;           // context for label naming
};

} // namespace postrisc
