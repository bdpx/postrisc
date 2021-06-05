#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static const noargs_info builtin_noargs_functions[] = {

    { "imm_i32",           insn_ld_imm,   "i32",    " %r1, -123456789",       "-123456789" },
    { "imm_u32",           insn_ld_imm,   "u32",    " %r1, 123456789",        "123456789" },
    { "imm_i32_l",         insn_ld_imm,   "i32",    ".l %r1, -1234567890",    "-1234567890" },
    { "imm_u32_l",         insn_ld_imm,   "u32",    ".l %r1, 1234567890",        "1234567890" },

    { "imm_i64",           insn_ld_imm,   "i64",    " %r1, -123456789",             "-123456789" },
    { "imm_u64",           insn_ld_imm,   "u64",    " %r1, 123456789",              "123456789" },
    { "imm_i64_l",         insn_ld_imm,   "i64",    ".l %r1, -1234567890123",       "-1234567890123" },
    { "imm_u64_l",         insn_ld_imm,   "u64",    ".l %r1, 1234567890123",        "1234567890123" },
    { "imm_u64_max",       insn_ld_imm,   "u64",    ".l %r1, 9223372036854775807",  "0xffffffffffffffffu / 2" },

    { "imm_i128",          insn_ld_imm,   "i128",    " %r1, 12345778",              "12345778ull" },
    { "imm_i128_l",        insn_ld_imm,   "i128",    ".l %r1, 987654321123456789",  "987654321123456789ull" },

    { "imm_f16",           insn_ld_imm,   "f16",    " %r1, 12263",                  "0.123456f16" },
    { "imm_f32",           insn_ld_imm,   "f32",    ".l %r1, 1039980160",             "0.123456f" },
    { "imm_f64",           insn_ld_imm,   "f64",    ".l %r1, -4629811673862064449",   "-0.123456" },

#define SPECIAL_REGISTER_ENUM_X(value, name, type, regclass, descr) { "get_" #name, insn_get_spr, #type, " %r1, %" #name, "__builtin_postrisc_get_" #name "()" },
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_ENUM_X)
#undef SPECIAL_REGISTER_ENUM_X

    { "readcyclecounter",     insn_get_spr,  "u64",    " %r1, %itc",       "__builtin_readcyclecounter()" },
    { "thread_pointer",       insn_mov,      "PVOID",  " %r1, %tp",        "__builtin_thread_pointer()" },
    { "stack_pointer",        insn_mov,      "PVOID",  " %r1, %sp",        "(void*)__builtin_postrisc_stack_pointer()" },

    { "frame_address",        insn_mov,      "PVOID",  " %r1, %fp",        "(void*)__builtin_frame_address(0)" },
    { "return_address",       insn_mov,      "PVOID",  " %r1, %r0",        "(void*)__builtin_return_address(0)" },

    { "random",               insn_random,   "u64",    " %r1, %gz",        "__builtin_postrisc_random()" },

    { "syscall",              insn_syscall,  "void",  "",                 "__builtin_postrisc_syscall()" },
    { "sysret",               insn_sysret,   "void",  "",                 "__builtin_postrisc_sysret()" },

    { "halt",                 insn_halt,     "void",  "",                 "__builtin_postrisc_halt()" },
    { "undef",                insn_undef,    "void",  "",                 "__builtin_postrisc_undef()" },
    { "rfi",                  insn_rfi,      "void",  "",                 "__builtin_postrisc_rfi()" },

    { "nop1",                 insn_nop,      "void",  " 1234567",                "__builtin_postrisc_nop(1234567)" },
    { "nop2",                 insn_nop,      "void",  ".l 123456789123456789",   "__builtin_postrisc_nop(123456789123456789ULL)" },
    { "nop3",                 insn_nop,      "void",  " 123",                    "__builtin_postrisc_nop(123)" },
    { "nop4",                 insn_nop,      "void",  ".l 123",                  "__builtin_postrisc_nop_l(123)" },


    { "int1",                 insn_int,      "void",  " %gz, 100",                "__builtin_postrisc_int(100)" },
    { "int2",                 insn_int,      "void",  " %gz, 200",                "__builtin_postrisc_int(200)" },
    { "int3",                 insn_int,      "void",  " %gz, 255",                "__builtin_postrisc_int(255)" },
    { "debugtrap",            insn_int,      "void",  " %gz, 1",                  "__builtin_debugtrap()" },
    { "trap",                 insn_int,      "void",  " %gz, 5",                  "__builtin_trap()" }, // no retf
};

CODEGEN_HANDLER(BuiltinNoArgs)
{
    for (const auto& func : builtin_noargs_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type;
        out << CHECK_LABEL << fname.str() << std::endl;
        // out << "// CHECK: " << insn_name(insn_alloc) << " 2" << std::endl;
        out << CHECK << InsnCounter(this, func.insn) << func.args << std::endl;
        if (func.args[0] == '.') {
            // we have double-slot main instruction, next will be in the next bundle
            out << CHECK_EMPTY << std::endl;
            out << CHECK_NEXT << "! BUNDLE" << std::endl;
        }
        if (strcmp(func.name, "trap") == 0) {
            out << CHECK_NEXT << insn_name(insn_nop) << " 0" << std::endl;
        } else {
            // we expect return, but frame_address is more complex (save/restore fp)
            if (strcmp(func.name, "frame_address") != 0) {
                out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
            }
        }

        out << func.dst_type << " " << fname.str() << "(void)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

static const unary_info builtin_unary_functions[] =
{
#define SPECIAL_REGISTER_ENUM_X(value, name, type, regclass, descr) { "set_" #name, insn_set_spr, "void", #type, " %r1, %" #name, "__builtin_postrisc_set_" #name "(x)" },
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_ENUM_X)
#undef SPECIAL_REGISTER_ENUM_X

    { "cpuid1",     insn_cpuid,  "u64", "i64", " %r1, %r1, 33",       "__builtin_postrisc_cpuid(x+33)" },
    { "cpuid2",     insn_cpuid,  "u64", "i64", " %r1, %r1, 0",        "__builtin_postrisc_cpuid(x)" },
    { "cpuid3",     insn_cpuid,  "u64", "i64", " %r1, %r1, 255",       "__builtin_postrisc_cpuid(x+255)" },

    { "popcount",    insn_cnt_pop, "int", "i32", " %r1, %r1, 31",       "__builtin_popcount(x)" },
    { "popcount",    insn_cnt_pop, "int", "u32", " %r1, %r1, 31",       "__builtin_popcount(x)" },
    { "popcount1",   insn_cnt_pop, "int", "i64", " %r1, %r1, 63",       "__builtin_popcountl(x)" },
    { "popcount1",   insn_cnt_pop, "int", "u64", " %r1, %r1, 63",       "__builtin_popcountl(x)" },
    { "popcount2",   insn_cnt_pop, "int", "i64", " %r1, %r1, 63",       "__builtin_popcountll(x)" },
    { "popcount2",   insn_cnt_pop, "int", "u64", " %r1, %r1, 63",       "__builtin_popcountll(x)" },

    { "ctz",    insn_cnt_tz, "int", "i32", " %r1, %r1, 31",       "__builtin_ctz(x)" },
    { "ctz",    insn_cnt_tz, "int", "u32", " %r1, %r1, 31",       "__builtin_ctz(x)" },
    { "ctz1",   insn_cnt_tz, "int", "i64", " %r1, %r1, 63",       "__builtin_ctzl(x)" },
    { "ctz1",   insn_cnt_tz, "int", "u64", " %r1, %r1, 63",       "__builtin_ctzl(x)" },
    { "ctz2",   insn_cnt_tz, "int", "i64", " %r1, %r1, 63",       "__builtin_ctzll(x)" },
    { "ctz2",   insn_cnt_tz, "int", "u64", " %r1, %r1, 63",       "__builtin_ctzll(x)" },

    { "clz",    insn_cnt_lz, "int", "i32", " %r1, %r1, 31",       "__builtin_clz(x)" },
    { "clz",    insn_cnt_lz, "int", "u32", " %r1, %r1, 31",       "__builtin_clz(x)" },
    { "clz1",   insn_cnt_lz, "int", "i64", " %r1, %r1, 63",       "__builtin_clzl(x)" },
    { "clz1",   insn_cnt_lz, "int", "u64", " %r1, %r1, 63",       "__builtin_clzl(x)" },
    { "clz2",   insn_cnt_lz, "int", "i64", " %r1, %r1, 63",       "__builtin_clzll(x)" },
    { "clz2",   insn_cnt_lz, "int", "u64", " %r1, %r1, 63",       "__builtin_clzll(x)" },

    { "bitreverse32",    insn_permb, "i32", "i32", " %r1, %r1, 31",       "__builtin_bitreverse32(x)" },
    { "bitreverse32",    insn_permb, "u32", "u32", " %r1, %r1, 31",       "__builtin_bitreverse32(x)" },
    { "bitreverse64",    insn_permb, "i64", "i64", " %r1, %r1, 63",       "__builtin_bitreverse64(x)" },
    { "bitreverse64",    insn_permb, "u64", "u64", " %r1, %r1, 63",       "__builtin_bitreverse64(x)" },

    { "set_thread_pointer",   insn_mov, "void", "PVOID", " %tp, %r1",       "__builtin_postrisc_set_thread_pointer(x)" },

    { "int1",             insn_int, "void", "i64", " %r1, 0",         "__builtin_postrisc_int(x)" },
    { "int2",             insn_int, "void", "i64", " %r1, 100",       "__builtin_postrisc_int(x+100)" },

    { "random_n",   insn_random,   "u64", "u64", " %r1, %r1",        "__builtin_postrisc_random_n(x)" },

    { "aesimc",     insn_aes_imc,  "v2i64", "v2i64", " %r1, %r1",     "__builtin_postrisc_aesimc(x)" },

    { "aeskeygenassist1",     insn_aes_keygen_assist,  "v2i64", "v2i64", " %r1, %r1, 100",     "__builtin_postrisc_aeskeygenassist(x, 100)" },
    { "aeskeygenassist2",     insn_aes_keygen_assist,  "v2i64", "v2i64", " %r1, %r1, -1",      "__builtin_postrisc_aeskeygenassist(x, 255)" },

    { "get_mr",     insn_get_mr,  "u64", "i64", " %r1, %r1, 1",       "__builtin_postrisc_get_mr(x+1)" },
    { "get_ibr",    insn_get_ibr, "u64", "i64", " %r1, %r1, 2",       "__builtin_postrisc_get_ibr(x+2)" },
    { "get_dbr",    insn_get_dbr, "u64", "i64", " %r1, %r1, 3",       "__builtin_postrisc_get_dbr(x+3)" },

    { "dcbf1",      insn_dcbf,    "void", "PVOID", " %r1, 0",              "__builtin_postrisc_dcbf(x)" },
    { "dcbf2",      insn_dcbf,    "void", "PVOID", " %r1, 1000",           "__builtin_postrisc_dcbf(x+1000)" },
    { "dcbf3",      insn_dcbf,    "void", "PVOID", ".l %r1, 1000000000",   "__builtin_postrisc_dcbf(x+1000000000)" },

    { "dcbt1",      insn_dcbt,    "void", "PVOID", " %r1, 0",              "__builtin_postrisc_dcbt(x)" },
    { "dcbt2",      insn_dcbt,    "void", "PVOID", " %r1, 1000",           "__builtin_postrisc_dcbt(x+1000)" },
    { "dcbt3",      insn_dcbt,    "void", "PVOID", ".l %r1, 1000000000",   "__builtin_postrisc_dcbt(x+1000000000)" },

    { "dcbi1",      insn_dcbi,    "void", "PVOID", " %r1, 0",              "__builtin_postrisc_dcbi(x)" },
    { "dcbi2",      insn_dcbi,    "void", "PVOID", " %r1, 1000",           "__builtin_postrisc_dcbi(x+1000)" },
    { "dcbi3",      insn_dcbi,    "void", "PVOID", ".l %r1, 1000000000",   "__builtin_postrisc_dcbi(x+1000000000)" },

    { "icbi1",      insn_icbi,    "void", "PVOID", " %r1, 0",              "__builtin_postrisc_icbi(x)" },
    { "icbi2",      insn_icbi,    "void", "PVOID", " %r1, 1000",           "__builtin_postrisc_icbi(x+1000)" },
    { "icbi3",      insn_icbi,    "void", "PVOID", ".l %r1, 1000000000",   "__builtin_postrisc_icbi(x+1000000000)" },

    { "unpack_high_vi8",    insn_unpack_high_vi8,   "v8i16", "v16i8", " %r1, %r1",     "__builtin_postrisc_unpack_high_vi8(x)" },
    { "unpack_high_vi16",   insn_unpack_high_vi16,  "v4i32", "v8i16", " %r1, %r1",     "__builtin_postrisc_unpack_high_vi16(x)" },
    { "unpack_high_vi32",   insn_unpack_high_vi32,  "v2i64", "v4i32", " %r1, %r1",     "__builtin_postrisc_unpack_high_vi32(x)" },

    { "unpack_low_vi8",    insn_unpack_low_vi8,   "v8i16", "v16i8", " %r1, %r1",     "__builtin_postrisc_unpack_low_vi8(x)" },
    { "unpack_low_vi16",   insn_unpack_low_vi16,  "v4i32", "v8i16", " %r1, %r1",     "__builtin_postrisc_unpack_low_vi16(x)" },
    { "unpack_low_vi32",   insn_unpack_low_vi32,  "v2i64", "v4i32", " %r1, %r1",     "__builtin_postrisc_unpack_low_vi32(x)" },

    { "unpack_high_vu8",    insn_unpack_high_vu8,   "v8i16", "v16i8", " %r1, %r1",     "__builtin_postrisc_unpack_high_vu8(x)" },
    { "unpack_high_vu16",   insn_unpack_high_vu16,  "v4i32", "v8i16", " %r1, %r1",     "__builtin_postrisc_unpack_high_vu16(x)" },
    { "unpack_high_vu32",   insn_unpack_high_vu32,  "v2i64", "v4i32", " %r1, %r1",     "__builtin_postrisc_unpack_high_vu32(x)" },

    { "unpack_low_vu8",    insn_unpack_low_vu8,   "v8i16", "v16i8", " %r1, %r1",     "__builtin_postrisc_unpack_low_vu8(x)" },
    { "unpack_low_vu16",   insn_unpack_low_vu16,  "v4i32", "v8i16", " %r1, %r1",     "__builtin_postrisc_unpack_low_vu16(x)" },
    { "unpack_low_vu32",   insn_unpack_low_vu32,  "v2i64", "v4i32", " %r1, %r1",     "__builtin_postrisc_unpack_low_vu32(x)" },
};

CODEGEN_HANDLER(BuiltinUnary)
{
    for (const auto& func : builtin_unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src_type;
        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, func.insn) << func.args << std::endl;
        if (func.args[0] == '.') {
            out << CHECK_EMPTY << std::endl;
            out << CHECK_NEXT << "! BUNDLE" << std::endl;
        }
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

static const binary_info builtin_binary_functions[] =
{
    { "set_mr",       insn_set_mr,   "void", "i64", "i64", "%r1, %r2, 2",       "__builtin_postrisc_set_mr(a, b+2)" },
    { "set_ibr",      insn_set_ibr,  "void", "i64", "i64", "%r1, %r2, 2",       "__builtin_postrisc_set_ibr(a, b+2)" },
    { "set_dbr",      insn_set_dbr,  "void", "i64", "i64", "%r1, %r2, 2",       "__builtin_postrisc_set_dbr(a, b+2)" },
    { "tpa",          insn_tpa,      "u64",  "PVOID", "u32", "%r1, %r1, %r2",     "__builtin_postrisc_tpa(a, b)" },

    { "aesenc",          insn_aes_enc,      "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_aesenc(a, b)" },
    { "aesenclast",      insn_aes_enc_last, "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_aesenclast(a, b)" },
    { "aesdec",          insn_aes_dec,      "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_aesdec(a, b)" },
    { "aesdeclast",      insn_aes_dec_last, "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_aesdeclast(a, b)" },

    { "clmul_ll",        insn_clmul_ll,     "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_clmul_ll(a, b)" },
    { "clmul_hl",        insn_clmul_hl,     "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_clmul_hl(a, b)" },
    { "clmul_hh",        insn_clmul_hh,     "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_clmul_hh(a, b)" },

#define VECTOR_BINARY_BUILTINS(X) \
  X(add_vu) X(addc_vu) X(add_sat_vu) X(add_sat_vi) X(addo_vi) X(avg_vu) X(avg_vi) \
  X(cmp_eq_vi) X(cmp_lt_vi) X(cmp_lt_vu) X(max_vi) X(max_vu) X(min_vi) X(min_vu) \
  X(rol_vu) X(ror_vu) X(sll_vu) X(sra_vi) X(srl_vu) \
  X(subb_vu) X(subo_vi) X(sub_vu) X(sub_sat_vi) X(sub_sat_vu) \
  X(merge_high_vu) X(merge_low_vu)

#define VECTOR_X(NAME) \
    { #NAME "8",  insn_ ## NAME ## 8,  "v16i8", "v16i8", "v16i8", "%r1, %r1, %r2", "__builtin_postrisc_" #NAME "8(a, b)" }, \
    { #NAME "16", insn_ ## NAME ## 16, "v8i16", "v8i16", "v8i16", "%r1, %r1, %r2", "__builtin_postrisc_" #NAME "16(a, b)" }, \
    { #NAME "32", insn_ ## NAME ## 32, "v4i32", "v4i32", "v4i32", "%r1, %r1, %r2", "__builtin_postrisc_" #NAME "32(a, b)" }, \
    { #NAME "64", insn_ ## NAME ## 64, "v2i64", "v2i64", "v2i64", "%r1, %r1, %r2", "__builtin_postrisc_" #NAME "64(a, b)" }, \

 VECTOR_BINARY_BUILTINS(VECTOR_X)

    { "pack_sat_vi16",    insn_pack_sat_vi16,  "v16i8", "v8i16", "v8i16", "%r1, %r1, %r2",     "__builtin_postrisc_pack_sat_vi16(a, b)" },
    { "pack_sat_vi32",    insn_pack_sat_vi32,  "v8i16", "v4i32", "v4i32", "%r1, %r1, %r2",     "__builtin_postrisc_pack_sat_vi32(a, b)" },
    { "pack_sat_vi64",    insn_pack_sat_vi64,  "v4i32", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_pack_sat_vi64(a, b)" },

    { "pack_usat_vi16",    insn_pack_usat_vi16,  "v16i8", "v8i16", "v8i16", "%r1, %r1, %r2",     "__builtin_postrisc_pack_usat_vi16(a, b)" },
    { "pack_usat_vi32",    insn_pack_usat_vi32,  "v8i16", "v4i32", "v4i32", "%r1, %r1, %r2",     "__builtin_postrisc_pack_usat_vi32(a, b)" },
    { "pack_usat_vi64",    insn_pack_usat_vi64,  "v4i32", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_pack_usat_vi64(a, b)" },

    { "pack_mod_vu16",    insn_pack_mod_vu16,  "v16i8", "v8i16", "v8i16", "%r1, %r1, %r2",     "__builtin_postrisc_pack_mod_vu16(a, b)" },
    { "pack_mod_vu32",    insn_pack_mod_vu32,  "v8i16", "v4i32", "v4i32", "%r1, %r1, %r2",     "__builtin_postrisc_pack_mod_vu32(a, b)" },
    { "pack_mod_vu64",    insn_pack_mod_vu64,  "v4i32", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_pack_mod_vu64(a, b)" },

    { "pack_sat_vu16",    insn_pack_sat_vu16,  "v16i8", "v8i16", "v8i16", "%r1, %r1, %r2",     "__builtin_postrisc_pack_sat_vu16(a, b)" },
    { "pack_sat_vu32",    insn_pack_sat_vu32,  "v8i16", "v4i32", "v4i32", "%r1, %r1, %r2",     "__builtin_postrisc_pack_sat_vu32(a, b)" },
    { "pack_sat_vu64",    insn_pack_sat_vu64,  "v4i32", "v2i64", "v2i64", "%r1, %r1, %r2",     "__builtin_postrisc_pack_sat_vu64(a, b)" },
};

CODEGEN_HANDLER(BuiltinBinary)
{
    for (const auto& func : builtin_binary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type;
        out << "// CHECK-LABEL: @" << fname.str() << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src1_type << " a, " << func.src2_type << " b)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: " << InsnCounter(this, func.insn) << " " << func.args << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

static const ternary_info builtin_ternary_functions[] =
{
    { "ptc",       insn_ptc,      "void", "PVOID", "PVOID", "u32", "%r1, %r2, %r3",       "__builtin_postrisc_tlb_purge(a, b, c)" },
    { "crc32c",    insn_crc32c,   "u32", "u32", "i128", "u64", "%r1, %r1, %r2, %r3",       "__builtin_postrisc_crc32c(a, b, c)" },
};

CODEGEN_HANDLER(BuiltinTernary)
{
    for (const auto& func : builtin_ternary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type;
        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << InsnCounter(this, func.insn) << " " << func.args << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src1_type << " a, " << func.src2_type << " b," << func.src3_type << " c)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

} // namespace llvm
} // namespace postrisc
