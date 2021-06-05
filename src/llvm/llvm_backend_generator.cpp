#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"
#include "emulator/SoftFpu.hpp"

/***********************************************************************
* llvm tablegen generator
***********************************************************************/

namespace postrisc {

void Decoder::tablegen_general_registerfile(std::ostream& out) const
{
    size_t const NBITS = bits_per_register;
    size_t const SIZE = (1 << NBITS);

    out << "\n\n"
        "def gpr_reg : GenericEnum {\n"
        "  let FilterClass = \"gpr_reg\";\n"
        "  let NameField = \"name\";\n"
        "  let ValueField = \"value\";\n"
        "}\n"
        "\n"
        "class gpr_reg<bits<" << NBITS << "> val, string s> {\n"
        "  string name = NAME;\n"
        "  string mnem = s;\n"
        "  bits<" << NBITS << "> value = val;\n"
        "}\n\n";

    for (size_t i = 0; i < SIZE; i++) {
        out << "def gpr_reg_"
            << std::left << std::setw(16) << gpr_names[i].data()
            << ": gpr_reg<" << i << ", \"" << gpr_names[i].data() << "\">;\n";
    }
}

void Decoder::tablegen_special_registerfile(std::ostream& out) const
{
    size_t const NBITS = bits_per_register;
    size_t const SIZE = (1 << NBITS);

    out << "\n\n"
        "def spr_reg : GenericEnum {\n"
        "  let FilterClass = \"spr_reg\";\n"
        "  let NameField = \"name\";\n"
        "  let ValueField = \"value\";\n"
        "}\n"
        "\n"
        "class spr_reg<bits<" << NBITS << "> val, string s> {\n"
        "  string name = NAME;\n"
        "  string mnem = s;\n"
        "  bits<" << NBITS << "> value = val;\n"
        "}\n\n";

    for (size_t i = 0; i < SIZE; i++) {
        if (arch_spr_regtype[i] != ESpecialRegisterSubset::reserved) {
            out << "def spr_reg_"
                << std::left << std::setw(16) << spr_names[i].data()
                << ": spr_reg<" << i << ", \"" << spr_names[i].data() << "\">;\n";
        }
    }
}

static std::string to_attribute(std::string input)
{
   for (auto& x : input) {
     x = (x == '.') ? '_' : x;
   }
   return input;
}

static const std::array<char [40], insn_reserved> asm_enum_name =
{
#define X(name, subset, format, descr) #name,
#include "arch/insn_table.hpp"
#undef X
#define NAME_X(name) #name,
    DECLARE_OPCODES_WITH_EXTENDED_CODES(NAME_X)
#undef NAME_X
    "illegal"
};

template<size_t NBITS, size_t SIZE = (1 << NBITS)>
static void
tablegen_opcodes(std::ostream& out,
    const Decoder::TInstructionMnemonicTable<SIZE>& opcode_names,
    const char *class_name)
{
    out << "\n\n";
    out << "class " << class_name << "<bits<" << NBITS << "> val, string _mnem> {\n"
           "    bits<" << NBITS << "> value = val;\n"
           "    string mnem = _mnem;\n"
           "}\n\n";

    for (size_t i = 0; i < SIZE; i++) {
        if (opcode_names[i] != insn_illegal) {
            out << "def opcode_"
                << std::left << std::setw(24) << to_attribute(asm_enum_name[opcode_names[i]])
                << ": " << class_name
                << "<" << i << ", \"" << insn_name(opcode_names[i]) << "\">;\n";
        }
    }
}

static void tablegen_opcodes(std::ostream& out)
{
    tablegen_opcodes<bits_per_opcode>      (out, Decoder::arch_info_opcode,   "opcode_opx");

    tablegen_opcodes<bits_per_fused_opx>   (out, Decoder::arch_info_fused,    "fused_opx");
    tablegen_opcodes<bits_per_fused_opx>   (out, Decoder::arch_info_fma,      "fma_opx");
    // fused subcodes
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nul_misc, "nul_misc_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nul_32,   "nul_32_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nul_64,   "nul_64_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nul_128,  "nul_128_opx");

    tablegen_opcodes<bits_per_misc_opx>    (out, Decoder::arch_info_misc,     "misc_opx");
    tablegen_opcodes<bits_per_register>    (out, Decoder::arch_info_raopx,    "raopx_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_br_misc,  "br_misc_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_br_32,    "br_32_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_br_64,    "br_64_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_br_128,   "br_128_opx");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_loop,     "loop_opx");

    tablegen_opcodes<bits_per_indexed_opx> (out, Decoder::arch_info_mem_xi64, "mem_xi64_opx");
    tablegen_opcodes<bits_per_indexed_opx> (out, Decoder::arch_info_mem_xu64, "mem_xu64_opx");
    tablegen_opcodes<bits_per_indexed_opx> (out, Decoder::arch_info_mem_xi32, "mem_xi32_opx");
    tablegen_opcodes<bits_per_indexed_opx> (out, Decoder::arch_info_mem_xu32, "mem_xu32_opx");
}

int
Decoder::LLVMTableGenGenerateEncoding(std::ostream& out) const
{
    out <<
        "//===-- PostriscInstrEncoding.td - Postrisc Instruction Encoding -*- tablegen -*-===//\n"
        "//\n"
        "//                     The LLVM Compiler Infrastructure\n"
        "//\n"
        "// This file is distributed under the University of Illinois Open Source\n"
        "// License. See LICENSE.TXT for details.\n"
        "// Autogenerated. Do not edit.\n"
        "//\n"
        "//===----------------------------------------------------------------------------===//\n";

    tablegen_opcodes(out);
    tablegen_general_registerfile(out);
    tablegen_special_registerfile(out);
    return 0;
}

} // namespace postrisc

int main(void)
{
    postrisc::util::setup();

    // "Generate LLVM TableGen ISA definition: >PostriscInstrEncoding.td";
    postrisc::Decoder decoder;
    return decoder.LLVMTableGenGenerateEncoding(std::cout);
}
