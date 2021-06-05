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
        out << "def gpr_reg_" << gpr_names[i].data()
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
            out << "def spr_reg_" << spr_names[i].data()
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

static u32 ilog2(size_t size)
{
    u32 nbits = 0;
    while (size >>= 1) {
        ++nbits;
    }
    return nbits;
}

template<class TABLE>
static void
tablegen_opcodes(std::ostream& out, const TABLE& opcode_names, const char *class_name)
{
    const u32 nbits = ilog2(opcode_names.size());

    out << "\n\n";
    out << "class " << class_name << "<bits<" << nbits << "> val, string _mnem> {\n"
           "    bits<" <<  nbits << "> value = val;\n"
           "    string mnem = _mnem;\n"
           "}\n\n";

    for (size_t i = 0; i < opcode_names.size(); i++) {
        if (opcode_names[i] != insn_illegal) {
            out << "def opcode_" << to_attribute(asm_enum_name[opcode_names[i]])
                << ": " << class_name
                << "<" << i << ", \"" << insn_name(opcode_names[i]) << "\">;\n";
        }
    }
}

static void tablegen_opcodes(std::ostream& out)
{
    tablegen_opcodes(out, Decoder::arch_info_opcode,   "opcode_opx");

    tablegen_opcodes(out, Decoder::arch_info_fused,    "fused_opx");
    // fused subcodes
    tablegen_opcodes(out, Decoder::arch_info_nul_misc, "nul_misc_opx");
    tablegen_opcodes(out, Decoder::arch_info_nul_32,   "nul_32_opx");
    tablegen_opcodes(out, Decoder::arch_info_nul_64,   "nul_64_opx");
    tablegen_opcodes(out, Decoder::arch_info_nul_128,  "nul_128_opx");

    tablegen_opcodes(out, Decoder::arch_info_misc,     "misc_opx");
    tablegen_opcodes(out, Decoder::arch_info_raopx,    "raopx_opx");
    tablegen_opcodes(out, Decoder::arch_info_br_misc,  "br_misc_opx");
    tablegen_opcodes(out, Decoder::arch_info_br_32,    "br_32_opx");
    tablegen_opcodes(out, Decoder::arch_info_br_64,    "br_64_opx");
    tablegen_opcodes(out, Decoder::arch_info_br_128,   "br_128_opx");
    tablegen_opcodes(out, Decoder::arch_info_loop,     "loop_opx");

    tablegen_opcodes(out, Decoder::arch_info_mem_xi64, "mem_xi64_opx");
    tablegen_opcodes(out, Decoder::arch_info_mem_xu64, "mem_xu64_opx");
    tablegen_opcodes(out, Decoder::arch_info_mem_xi32, "mem_xi32_opx");
    tablegen_opcodes(out, Decoder::arch_info_mem_xu32, "mem_xu32_opx");

    tablegen_opcodes(out, Decoder::arch_info_fma_f16,  "fma_f16_opx");
    tablegen_opcodes(out, Decoder::arch_info_fma_f32,  "fma_f32_opx");
    tablegen_opcodes(out, Decoder::arch_info_fma_f64,  "fma_f64_opx");
    tablegen_opcodes(out, Decoder::arch_info_fma_f128, "fma_f128_opx");
}

static void generate_rounding_modes(std::ostream& out)
{
    out <<
    "\n\n"
    "def RoundingMode : GenericEnum {\n"
    "  let FilterClass = \"rounding_mode_opx\";\n"
    "  let NameField = \"mnem\";\n"
    "  let ValueField = \"value\";\n"
    "}\n\n";

    out <<
    "class rounding_mode_opx<bits<" << (unsigned)bits_per_rounding_mode << "> val, string _mnem> {\n"
    "  bits<" << (unsigned)bits_per_rounding_mode << "> value = val;\n"
    "  string mnem = _mnem;\n"
    "}\n\n";

    static const struct {
        RoundingMode rounding_mode;
        char name[16];
    } values[] = {
        { RoundingMode::RNE, "RNE" },
        { RoundingMode::RTZ, "RTZ" },
        { RoundingMode::RDN, "RDN" },
        { RoundingMode::RUP, "RUP" },
        { RoundingMode::RMM, "RMM" },
        { RoundingMode::DYN, "DYN" },
    };

    for (const auto& value : values) {
        const char *name = RoundingMode2Str(value.rounding_mode);
        out << "def rounding_mode_" << name << " : rounding_mode_opx<" << static_cast<u32>(value.rounding_mode) << ", \"" << value.name << "\">;\n";
    }
}

static void generate_memory_orderings(std::ostream& out)
{
    out <<
    "\n\n"
    "def MemoryOrdering : GenericEnum {\n"
    "  let FilterClass = \"memory_ordering_opx\";\n"
    "  let NameField = \"mnem\";\n"
    "  let ValueField = \"value\";\n"
    "}\n\n";

    out <<
    "class memory_ordering_opx<bits<" << (unsigned)bits_per_memory_ordering << "> val, string _mnem> {\n"
    "  bits<" << (unsigned)bits_per_memory_ordering << "> value = val;\n"
    "  string mnem = _mnem;\n"
    "}\n\n";

    static const struct {
        MemoryOrdering ordering;
        char name[16];
    } values[] = {
        { MemoryOrdering::RELAXED, "RELAXED" },
        { MemoryOrdering::ACQUIRE, "ACQUIRE" },
        { MemoryOrdering::RELEASE, "RELEASE" },
        { MemoryOrdering::ACQ_REL, "ACQ_REL" },
        { MemoryOrdering::SEQ_CST, "SEQ_CST" },
    };

    for (const auto& value : values) {
        const char *name = MemoryOrdering2Str(value.ordering);
        out << "def memory_ordering_" << name << " : memory_ordering_opx<" << static_cast<u32>(value.ordering) << ", \"" << value.name << "\">;\n";
    }
}

static void generate_fp_classify_flags(std::ostream& out)
{
    out <<
    "\n\n"
    "def FpClassifyFlag : GenericEnum {\n"
    "  let FilterClass = \"classify_flag_opx\";\n"
    "  let NameField = \"Name\";\n"
    "  let ValueField = \"value\";\n"
    "}\n\n";

    out <<
    "class classify_flag_opx<bits<" << (unsigned)bits_per_update_stride << "> val> {\n"
    "  bits<" << (unsigned)bits_per_update_stride << "> value = val;\n"
    "  string Name = NAME;\n"
    "}\n\n";

    static const struct {
        FClassMask ordering;
        char name[32];
    } values[] = {
#define X(NAME) { FClassMask::NAME, #NAME },
        X(SignalingNaN)
        X(QuietNaN)
        X(NegativeInfinity)
        X(NegativeNormal)
        X(NegativeSubnormal)
        X(NegativeZero)
        X(PositiveZero)
        X(PositiveSubnormal)
        X(PositiveNormal)
        X(PositiveInfinity)
    };

    for (const auto& value : values) {
        out << "def " << value.name << " : classify_flag_opx<" << static_cast<u32>(value.ordering) << ">;\n";
    }
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
    generate_memory_orderings(out);
    generate_fp_classify_flags(out);
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
