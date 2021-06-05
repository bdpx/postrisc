#pragma once

#include <map>

#include "arch/isa.hpp"
#include "arch/Bundle.hpp"

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

class LabelMarker;

class Symbol {
public:
    Symbol(u64 _address, u64 _size, size_t _name_offset)
        : address(_address), size(_size), name_offset(_name_offset) {}

    u64 get_address(void) const { return address; }
    u64 get_size(void) const { return size; }
    size_t get_name_offset(void) const { return name_offset; }

    bool operator>(const Symbol& r) const { return address > r.address; }

private:
    u64      address;
    u64      size;
    size_t   name_offset;
};

class DecoderContext {
public:
    explicit DecoderContext(disassembler_mode _mode) : mode(_mode) {}
    static unsigned const unmatched = 4; // unmatched never match with 0,1,2 instruction slot numbers
public:
    disassembler_mode  mode;
    unsigned           restart_slot = DecoderContext::unmatched;
    i64                instr_number = std::numeric_limits<i64>::min();
    u32                future_predication = 0;
};


class AddressInfo {
public:
    explicit AddressInfo(u64 address, char const *name, size_t offset) : m_address(address), m_name(name), m_offset(offset) {}

    friend std::ostream& operator<<(std::ostream& out, const AddressInfo& r) {
        if (r.m_name) {
            out << r.m_name;
            if (r.m_offset) {
                out << '+' << r.m_offset;
            }
        } else {
             out << 'L' << fmt::hex(r.m_address);
        }
        return out;
    }
private:
    u64         m_address;
    const char *m_name;
    size_t      m_offset;
};

class Decoder {
public:
    explicit Decoder(void);
    ~Decoder(void);

    static void WriteRegister(std::ostream& out, unsigned regnum, disassembler_mode mode);

    int Disassemble(std::istream& in, std::ostream& out, disassembler_mode mode) const;

    template<typename CMD>
    EInstructionID Disassemble(std::ostream& out, const CMD cmd, u64 ip, disassembler_mode mode) const;

    void DisassembleBundle(const Bundle bundle, u64 ip, std::ostream& out, DecoderContext& decoderContext) const;

    void add_symbol(u64 address, u64 size, char const *name);
    void add_section_name(u64 address, u64 size, char const *name);
    AddressInfo get_address_info(u64 address) const;

private:
    size_t SaveName(char const *name);
    AddressInfo get_section_info(u64 address) const;
    template<typename CMD>
    void DisassembleInstructionField(std::ostream& out, CMD cmd, u64 ip, const EInstructionField field, disassembler_mode mode) const;

    template<typename CMD>
    void PrintInstruction(const CMD cmd, std::ostream& out, u64 ip, unsigned slotNumber, DecoderContext& context) const;

    void DisassembleDataSection(std::ostream& out, char const *section_name,
            u8 const data[], size_t section_len, u64 start_address, u64 address,
            const LabelMarker& labels, disassembler_mode mode) const;

private:
    std::map<u64, Symbol, std::greater<u64>>     m_symbols;
    std::map<u64, Symbol, std::greater<u64>>     m_sections;
    std::vector<char>                            m_symbol_names;
};

/***************************************************************************
* instruction disassemble helper
***************************************************************************/
template<typename CMD>
class InstructionDisassembler {
public:
    InstructionDisassembler(const Decoder& _decoder, CMD cmd, u64 _ip)
        : decoder(_decoder)
        , instruction(cmd)
        , ip(_ip)
    {}

    friend std::ostream& operator<<(std::ostream& out, const InstructionDisassembler& r)
    {
        r.decoder.Disassemble(out, r.instruction, r.ip, disasm_mode_std);
        return out;
    }

private:
    const Decoder& decoder;
    CMD            instruction;
    u64            ip;           // context for label naming
};

} // namespace postrisc
