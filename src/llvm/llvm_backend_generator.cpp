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
#define X(mnem, name, subset, format, descr) #name,
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
    const char *class_name, const char *prefix)
{
    out << "\n\n";
    out << "class " << class_name << "<bits<" << NBITS << "> val, string _mnem> {\n"
           "    bits<" << NBITS << "> value = val;\n"
           "    string mnem = _mnem;\n"
           "}\n\n";

    for (size_t i = 0; i < SIZE; i++) {
        if (opcode_names[i] != insn_illegal) {
            out << "def " << prefix
                << std::left << std::setw(24) << to_attribute(asm_enum_name[opcode_names[i]])
                << ": " << class_name
                << "<" << i << ", \"" << asm_name[opcode_names[i]] << "\">;\n";
        }
    }
}

static void tablegen_opcodes(std::ostream& out)
{
    tablegen_opcodes<bits_per_opcode>      (out, Decoder::arch_info_opcode,  "opcode_opx", "opcode_");

    tablegen_opcodes<bits_per_fused_opx>   (out, Decoder::arch_info_fused,   "fused_opx", "fused_opx_");
    // fused subcodes
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nullrr,  "nullrr_opx", "nullrr_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nulfss,  "nulfss_opx", "nulfss_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nulfsd,  "nulfsd_opx", "nulfsd_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_nulfsq,  "nulfsq_opx", "nulfsq_opx_");

    tablegen_opcodes<bits_per_misc_opx>    (out, Decoder::arch_info_misc,    "misc_opx",  "misc_opx_");
    tablegen_opcodes<bits_per_register>    (out, Decoder::arch_info_raopx,   "raopx_opx", "raopx_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_brr,     "brr_opx",   "brr_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_loop,    "loop_opx",  "loop_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_bfss,    "bfss_opx",  "bfss_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_bfsd,    "bfsd_opx",  "bfsd_opx_");
    tablegen_opcodes<bits_per_branch_opx>  (out, Decoder::arch_info_bfsq,    "bfsq_opx",  "bfsq_opx_");

    tablegen_opcodes<bits_per_indexed_opx> (out, Decoder::arch_info_indexed_ds, "indexed_ds_opx",  "indexed_ds_opx_");
    tablegen_opcodes<bits_per_indexed_opx> (out, Decoder::arch_info_indexed_ws, "indexed_ws_opx",  "indexed_ws_opx_");
    tablegen_opcodes<bits_per_indexed_opx> (out, Decoder::arch_info_indexed_wz, "indexed_wz_opx",  "indexed_wz_opx_");
}

static void generate_rounding_modes(std::ostream& out)
{
    out << "\n\n";
    out << "class rounding_mode_opx<bits<" << (unsigned)bits_per_rounding_mode << "> val> {\n"
           "    bits<" << (unsigned)bits_per_rounding_mode << "> value = val;\n"
           "}\n\n";

    out << "def rounding_mode_nearesteven   : rounding_mode_opx<" << (unsigned)fpu::RM_RNE << ">;\n";
    out << "def rounding_mode_downward      : rounding_mode_opx<" << (unsigned)fpu::RM_RDN << ">;\n";
    out << "def rounding_mode_upward        : rounding_mode_opx<" << (unsigned)fpu::RM_RUP << ">;\n";
    out << "def rounding_mode_towardzero    : rounding_mode_opx<" << (unsigned)fpu::RM_RTZ << ">;\n";
    out << "def rounding_mode_nearestmaxmag : rounding_mode_opx<" << (unsigned)fpu::RM_RMM << ">;\n";
    out << "def rounding_mode_dynamic       : rounding_mode_opx<" << (unsigned)fpu::RM_DYN << ">;\n";
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
    generate_rounding_modes(out);
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
