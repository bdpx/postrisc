#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "disassembler/decoder.hpp"
#include "html_generator.hpp"

/***********************************************************************
* llvm tablegen generator
***********************************************************************/

#define DEFINE_UNITS_TABLE(X) \
    X(RESERVED) \
    X(CPU0) \
    X(CPU1) \
    X(AGU0) \
    X(AGU1) \
    X(FPU0) \
    X(FPU1) \

enum UNIT_type : uint8_t {
#define UNIT_ENUM_X(id) UNIT_##id,
    DEFINE_UNITS_TABLE(UNIT_ENUM_X)
};

#define UNIT_ARRAY(...) { __VA_ARGS__, UNIT_RESERVED }

#define DEFINE_INSTRUCTION_ITINERARY_CLASSES_TABLE(X) \
    X(iu_or_fpu_instr,    1, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(iu_instr,           1, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(fpu_normal_instr,   5, 1, UNIT_ARRAY(UNIT_CPU0, UNIT_CPU1)) \
    X(fpu_fast_instr,     3, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(jmp_or_call,        2, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(ldd,                2, 1, UNIT_ARRAY(UNIT_CPU0, UNIT_CPU1)) \
    X(st,                 2, 1, UNIT_ARRAY(UNIT_CPU0, UNIT_CPU1)) \
    X(std,                2, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(iu_smul,            6, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(iu_umul,            6, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(iu_div,            35, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(ticc,               1, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(fpu_muls,          11, 1, UNIT_ARRAY(UNIT_FPU0)) \
    X(fpu_muld,          12, 1, UNIT_ARRAY(UNIT_FPU0)) \
    X(fpu_divs,          22, 1, UNIT_ARRAY(UNIT_FPU0)) \
    X(fpu_divd,          30, 1, UNIT_ARRAY(UNIT_FPU0)) \
    X(fpu_sqrts,         35, 1, UNIT_ARRAY(UNIT_FPU0)) \
    X(fpu_sqrtd,         45, 1, UNIT_ARRAY(UNIT_FPU0)) \
    X(fpu_abs,            1, 1, UNIT_ARRAY(UNIT_FPU0, UNIT_FPU1)) \
    X(fpu_movs,           1, 1, UNIT_ARRAY(UNIT_FPU0, UNIT_FPU1)) \
    X(fpu_negs,           1, 1, UNIT_ARRAY(UNIT_FPU0, UNIT_FPU1)) \
    X(smac_umac,          2, 1, UNIT_ARRAY(UNIT_CPU0)) \
    X(fpu_stod,           2, 1, UNIT_ARRAY(UNIT_CPU0)) \

enum IIC_Class : uint8_t
{
#define ITINERARY_CLASS_ENUM_X(id, delay1, delay2, units_array) IIC_##id,
    DEFINE_INSTRUCTION_ITINERARY_CLASSES_TABLE(ITINERARY_CLASS_ENUM_X)
};

#define DEFINE_FEATURES_TABLE(X) \
    X (SoftMulDiv, "soft-mul-div", "UseSoftMulDiv", true,  "Use software emulation for integer multiply and divide") \
    X (NoFSMULD,   "no-fsmuld",    "HasNoFSMULD",   true,  "Disable the fsmuld instruction.") \
    X (NoFMULS,    "no-fmuls",     "HasNoFMULS",    true,  "Disable the fmuls instruction.") \
    X (V9,         "v9",           "IsV9",          true,  "Enable SPARC-V9 instructions") \
    X (VIS,        "vis",          "IsVIS",         true,  "Enable UltraSPARC Visual Instruction Set extensions") \

enum FEATURE_Type : uint8_t {
#define FEATURE_ENUM_X(enumerate, name, predicate, defvalue, description) FEATURE_##enumerate,
    DEFINE_FEATURES_TABLE(FEATURE_ENUM_X)
};


namespace postrisc {
namespace llvm {

template<size_t NBITS, size_t SIZE = (1 << NBITS)>
void
generate_opcodes(std::ostream & out, const Decoder::TInstructionMnemonicTable<SIZE> & opcode_names, const char * class_name, const char * prefix)
{
    out << "class " << class_name << "<bits<" << NBITS << "> val>\n"
           "{\n"
           "    bits<" << NBITS << "> Value = val;\n"
           "}\n\n";

    for (size_t i = 0; i < SIZE; i++) {
        if (opcode_names[i] != insn_undef)
        out << "def " << prefix
            << std::left << std::setw(16) << asm_name[opcode_names[i]]
            << ": " << class_name
            << "<" << i << ">;\n";
    }

    out << "\n\n";
}

void
generate_opcodes(std::ostream & out)
{
    generate_opcodes<bits_per_opcode>      (out, Decoder::arch_info_opcode,  "OPCODE", "OPCODE_");
    generate_opcodes<bits_per_fused_opx>   (out, Decoder::arch_info_fused,   "FUSED_OPX", "FUSED_OPX_");
    generate_opcodes<bits_per_nullify_opx> (out, Decoder::arch_info_nullify, "NULLIFY_OPX", "NULLIFY_OPX_");
    generate_opcodes<bits_per_misc_opx>    (out, Decoder::arch_info_misc,    "MISC_OPX", "MISC_OPX_");
    generate_opcodes<bits_per_register>    (out, Decoder::arch_info_raopx,   "RAOPX_OPX", "RAOPX_OPX_");
}

void
generate_shedules(std::ostream & out)
{
    out << "\n\n"
           "//Functional units\n";

    static const char unit_names[][16] = {
#define UNIT_NAME_X(id) #id,
    DEFINE_UNITS_TABLE(UNIT_NAME_X)
    };

    for (const auto & x :unit_names) {
        out << "def " << std::left << std::setw(8) << "" << x << ": FuncUnit;\n";
    }

    out << "\n\n"
           "//Instruction itinerary classes\n";

    static const struct {
        char        name[26];
        uint8_t     delay1, delay2;
        UNIT_type   units[8];
    } itinerary_info[] = {
#define ITINERARY_CLASS_DATA_X(id, d1, d2, array) { #id, d1, d2, array },
    DEFINE_INSTRUCTION_ITINERARY_CLASSES_TABLE(ITINERARY_CLASS_DATA_X)
    };

    for (const auto & x : itinerary_info) {
        out << "def " << "IIC_" << x.name << " : InstrItinClass;\n";
    }
    out << "\n\n";

    out << "def DefaultItineraries : ProcessorItineraries<\n";
    out << "[";

    bool first = true;
    for (const auto & x :unit_names) {
        if (!first)
            out << ", ";
        out << x;
        first = false;
    }

    out << "]";
    out << ", []";

    out << ", [\n";

    for (const auto & x : itinerary_info) {
        out << "InstrItinData <" << " IIC_" << std::left << std::setw(24) << x.name;
        out << ", [ InstrStage < 1, [";

        first = true;
        for (const auto e : x.units) {
           if (e == UNIT_RESERVED)
              break;
           if (!first)
               out << ", ";
           out << unit_names[e];
           first = false;
        }

        out << "]>]";
        out << ", [" << +x.delay1 << ", " << +x.delay2 << "]";
        out << ">,\n";
    }

    out << "]>;\n\n\n";
}

void generate_features(std::ostream & out)
{
    out << "//===----------------------------------------------------------------------===//\n";
    out << "// POSTRISC Subtarget features.\n";
    out << "//\n\n";

    static const struct {
        char  enumerate[20];
        char  name[20];
        char  predicate[16];
        bool  defvalue;
        char  description[56];
    } feature_info[] = {
#define FEATURE_DATA_X(enumerate, name, predicate, defvalue, description) { #enumerate, name, predicate, defvalue, description },
    DEFINE_FEATURES_TABLE(FEATURE_DATA_X)
    };

    for (const auto & x : feature_info) {
        out << "def Feature" << std::left << std::setw(12) << x.enumerate
            << " : SubtargetFeature<\"" << x.name << "\""
            ", \"" << x.predicate << "\""
            ", \"" << std::boolalpha << x.defvalue << "\""
            ", \"" << x.description << "\""
            ">;\n";
    }
}

void
generate_processor_models(std::ostream & out)
{
    out << "def : Processor<\"gr210\", DefaultItineraries, [FeatureLeon, UMACSMACSupport, LeonCASA]>;\n";
    out << "def : Processor<\"gr740\", DefaultItineraries, [FeatureLeon, UMACSMACSupport, LeonCASA, LeonCycleCounter]>;\n";
    out << "\n\n";
}

int
TableGenGenerateEncoding(std::ostream & out)
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
        "//===----------------------------------------------------------------------------===//\n"
        "\n\n";

    generate_opcodes(out);

    generate_shedules(out);

    //generate_features(out);
    //generate_processor_models(out);

    return 0;
}

} // namespace llvm
} // namespace postrisc
