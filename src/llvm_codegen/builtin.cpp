#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(BuiltinSpecialRegs)
{
    static const struct {
        char name[12];
        char type[8];
    } regs[] = {
#define SPECIAL_REGISTER_X(value, name, type, regclass, descr) \
    { #name, #type },
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_X)
#undef SPECIAL_REGISTER_X
    };

    for (const auto& test : regs) {
        std::ostringstream fname;
        fname << "test_get_" << test.name;
        out << CHECK_LABEL << fname.str() << '\n';
        out << "// CHECK: " << insn_alloc << " 2\n";
        out << CHECK << InsnCounter(this, insn_getspr) << " %r1, %" << test.name << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.type << " " << fname.str() << "(void)\n";
        out << "{\n";
        out << "    return __builtin_postrisc_get_" << test.name << "();\n";
        out << "}\n\n";
    }

    for (const auto& test : regs) {
        std::ostringstream fname;
        fname << "test_set_" << test.name;
        out << CHECK_LABEL << fname.str() << '\n';
        out << "// CHECK: " << insn_alloc << " 2\n";
        out << CHECK << InsnCounter(this, insn_setspr) <<  " %r1, %" << test.name << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "void " << fname.str() << "(" <<  test.type << " x)\n";
        out << "{\n";
        out << "    __builtin_postrisc_set_" << test.name << "(x);\n";
        out << "}\n\n";
    }
}


static const noargs_info builtin_noargs_functions[] = {
    { "imm_i32",           insn_ldi, IW, " %r1, -123456789",       "-123456789" },
    { "imm_u32",           insn_ldi, UW, " %r1, 123456789",        "123456789" },
    { "imm_i32_l",         insn_ldi, IW, ".l %r1, -1234567890",    "-1234567890" },
    { "imm_u32_l",         insn_ldi, UW, ".l %r1, 1234567890",        "1234567890" },

    { "imm_i64",           insn_ldi, ID, " %r1, -123456789",             "-123456789" },
    { "imm_u64",           insn_ldi, UD, " %r1, 123456789",              "123456789" },
    { "imm_i64_l",         insn_ldi, ID, ".l %r1, -1234567890123",       "-1234567890123" },
    { "imm_u64_l",         insn_ldi, UD, ".l %r1, 1234567890123",        "1234567890123" },
    { "imm_u64_max",       insn_ldi, UD, ".l %r1, 9223372036854775807",  "0xffffffffffffffffu / 2" },

    { "imm_i128",          insn_ldi, IQ, " %r1, 12345778",              "12345778ull" },
    { "imm_i128_l",        insn_ldi, IQ, ".l %r1, 987654321123456789",  "987654321123456789ull" },

    { "imm_f16",           insn_ldi, FH, " %r1, 12263",                  "0.123456f16" },
    { "imm_f32",           insn_ldi, FS, ".l %r1, 1039980160",             "0.123456f" },
    { "imm_f64",           insn_ldi, FD, ".l %r1, -4629811673862064449",   "-0.123456" },

    { "rscover",           insn_rscover,  VOID,  "",  "__builtin_postrisc_rscover()" },
    { "rsflush",           insn_rsflush,  VOID,  "",  "__builtin_postrisc_rsflush()" },
    { "rsload",            insn_rsload,   VOID,  "",  "__builtin_postrisc_rsload()"  },
    { "readcyclecounter",  insn_getspr,   UD,    " %r1, %itc",       "__builtin_readcyclecounter()" },
    { "thread_pointer",    insn_mov,      PVOID, " %r1, %tp",        "__builtin_thread_pointer()" },
    { "stack_pointer",     insn_mov,      PVOID, " %r1, %sp",        "(void*)__builtin_postrisc_stack_pointer()" },
    { "frame_address",     insn_mov,      PVOID, " %r1, %fp",        "(void*)__builtin_frame_address(0)" },
    { "return_address",    insn_mov,      PVOID, " %r1, %r0",        "(void*)__builtin_return_address(0)" },
    { "random",            insn_random,   UD,    " %r1, %gz",        "__builtin_postrisc_random()" },
    { "syscall",           insn_syscall,  VOID,  "",                 "__builtin_postrisc_syscall()" },
    { "sysret",            insn_sysret,   VOID,  "",                 "__builtin_postrisc_sysret()" },
    { "halt",              insn_halt,     VOID,  "",                 "__builtin_postrisc_halt()" },
    { "undef",             insn_undef,    VOID,  "",                 "__builtin_postrisc_undef()" },
    { "rfi",               insn_rfi,      VOID,  "",                 "__builtin_postrisc_rfi()" },
    { "nop1",              insn_nop,      VOID,  " 1234567",                "__builtin_postrisc_nop(1234567)" },
    { "nop2",              insn_nop,      VOID,  ".l 123456789123456789",   "__builtin_postrisc_nop(123456789123456789ULL)" },
    { "nop3",              insn_nop,      VOID,  " 123",                    "__builtin_postrisc_nop(123)" },
    { "nop4",              insn_nop,      VOID,  ".l 123",                  "__builtin_postrisc_nop_l(123)" },
    { "int1",              insn_int,      VOID,  " %gz, 100",                "__builtin_postrisc_int(100)" },
    { "int2",              insn_int,      VOID,  " %gz, 200",                "__builtin_postrisc_int(200)" },
    { "int3",              insn_int,      VOID,  " %gz, 255",                "__builtin_postrisc_int(255)" },
    { "debugtrap",         insn_int,      VOID,  " %gz, 1",                  "__builtin_debugtrap()" },
    { "trap",              insn_int,      VOID,  " %gz, 5",                  "__builtin_trap()" }, // no retf
};

CODEGEN_HANDLER(BuiltinNoArgs)
{
    for (const auto& func : builtin_noargs_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type;
        out << CHECK_LABEL << fname.str() << '\n';
        // out << "// CHECK: " << insn_alloc << " 2\n";
        out << CHECK << InsnCounter(this, func.insn) << func.args << '\n';
        if (func.args[0] == '.') {
            // we have double-slot main instruction, next will be in the next bundle
            out << CHECK_EMPTY << '\n';
            out << CHECK_NEXT << "! BUNDLE\n";
        }
        if (strcmp(func.name, "trap") == 0) {
            out << CHECK_NEXT << insn_nop << " 0\n";
        } else {
            // we expect return, but frame_address is more complex (save/restore fp)
            if (strcmp(func.name, "frame_address") != 0) {
                out << CHECK_NEXT << insn_retf << " 0\n";
            }
        }

        out << func.dst_type << " " << fname.str() << "(void)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

static const unary_info builtin_unary_functions[] =
{
    { "cpuid1",     insn_cpuid,  UD, ID, " %r1, %r1, 33",       "__builtin_postrisc_cpuid(x+33)" },
    { "cpuid2",     insn_cpuid,  UD, ID, " %r1, %r1, 0",        "__builtin_postrisc_cpuid(x)" },
    { "cpuid3",     insn_cpuid,  UD, ID, " %r1, %r1, 255",       "__builtin_postrisc_cpuid(x+255)" },

    { "popcount",    insn_cntpop, IW, IW, " %r1, %r1, 31",       "__builtin_popcount(x)" },
    { "popcount",    insn_cntpop, IW, UW, " %r1, %r1, 31",       "__builtin_popcount(x)" },
    { "popcount1",   insn_cntpop, IW, ID, " %r1, %r1, 63",       "__builtin_popcountl(x)" },
    { "popcount1",   insn_cntpop, IW, UD, " %r1, %r1, 63",       "__builtin_popcountl(x)" },
    { "popcount2",   insn_cntpop, IW, ID, " %r1, %r1, 63",       "__builtin_popcountll(x)" },
    { "popcount2",   insn_cntpop, IW, UD, " %r1, %r1, 63",       "__builtin_popcountll(x)" },

    { "ctz",    insn_cnttz, IW, IW, " %r1, %r1, 31",       "__builtin_ctz(x)" },
    { "ctz",    insn_cnttz, IW, UW, " %r1, %r1, 31",       "__builtin_ctz(x)" },
    { "ctz1",   insn_cnttz, IW, ID, " %r1, %r1, 63",       "__builtin_ctzl(x)" },
    { "ctz1",   insn_cnttz, IW, UD, " %r1, %r1, 63",       "__builtin_ctzl(x)" },
    { "ctz2",   insn_cnttz, IW, ID, " %r1, %r1, 63",       "__builtin_ctzll(x)" },
    { "ctz2",   insn_cnttz, IW, UD, " %r1, %r1, 63",       "__builtin_ctzll(x)" },

    { "clz",    insn_cntlz, IW, IW, " %r1, %r1, 31",       "__builtin_clz(x)" },
    { "clz",    insn_cntlz, IW, UW, " %r1, %r1, 31",       "__builtin_clz(x)" },
    { "clz1",   insn_cntlz, IW, ID, " %r1, %r1, 63",       "__builtin_clzl(x)" },
    { "clz1",   insn_cntlz, IW, UD, " %r1, %r1, 63",       "__builtin_clzl(x)" },
    { "clz2",   insn_cntlz, IW, ID, " %r1, %r1, 63",       "__builtin_clzll(x)" },
    { "clz2",   insn_cntlz, IW, UD, " %r1, %r1, 63",       "__builtin_clzll(x)" },

    { "bitreverse32",    insn_permb, IW, IW, " %r1, %r1, 31",       "__builtin_bitreverse32(x)" },
    { "bitreverse32",    insn_permb, UW, UW, " %r1, %r1, 31",       "__builtin_bitreverse32(x)" },
    { "bitreverse64",    insn_permb, ID, ID, " %r1, %r1, 63",       "__builtin_bitreverse64(x)" },
    { "bitreverse64",    insn_permb, UD, UD, " %r1, %r1, 63",       "__builtin_bitreverse64(x)" },

    { "set_thread_pointer",   insn_mov, VOID, PVOID, " %tp, %r1",       "__builtin_postrisc_set_thread_pointer(x)" },

    { "int1",             insn_int, VOID, ID, " %r1, 0",         "__builtin_postrisc_int(x)" },
    { "int2",             insn_int, VOID, ID, " %r1, 100",       "__builtin_postrisc_int(x+100)" },

    { "random_n",   insn_random,   UD, UD, " %r1, %r1",        "__builtin_postrisc_random_n(x)" },

    { "aesimc",     insn_aesimc,  VID, VID, " %r1, %r1",     "__builtin_postrisc_aesimc(x)" },

    { "aeskeygenassist1",     insn_aeskeygenassist,  VID, VID, " %r1, %r1, 100",  "__builtin_postrisc_aeskeygenassist(x, 100)" },
    { "aeskeygenassist2",     insn_aeskeygenassist,  VID, VID, " %r1, %r1, 255",  "__builtin_postrisc_aeskeygenassist(x, 255)" },

    { "getmr",     insn_getmr,  UD, ID, " %r1, %r1, 1",       "__builtin_postrisc_get_mr(x+1)" },
    { "getibr",    insn_getibr, UD, ID, " %r1, %r1, 2",       "__builtin_postrisc_get_ibr(x+2)" },
    { "getdbr",    insn_getdbr, UD, ID, " %r1, %r1, 3",       "__builtin_postrisc_get_dbr(x+3)" },

    { "dcbf1",      insn_dcbf,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_dcbf(x)" },
    { "dcbf2",      insn_dcbf,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_dcbf(x+1000)" },
    { "dcbf3",      insn_dcbf,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_dcbf(x+1000000000)" },

    { "dcbt1",      insn_dcbt,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_dcbt(x)" },
    { "dcbt2",      insn_dcbt,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_dcbt(x+1000)" },
    { "dcbt3",      insn_dcbt,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_dcbt(x+1000000000)" },

    { "dcbi1",      insn_dcbi,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_dcbi(x)" },
    { "dcbi2",      insn_dcbi,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_dcbi(x+1000)" },
    { "dcbi3",      insn_dcbi,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_dcbi(x+1000000000)" },

    { "icbi1",      insn_icbi,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_icbi(x)" },
    { "icbi2",      insn_icbi,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_icbi(x+1000)" },
    { "icbi3",      insn_icbi,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_icbi(x+1000000000)" },

    { "builtin_sra_imm",   insn_vsraib,  VIB, VIB, " %r1, %r1, 5",    "__builtin_postrisc_vsraib(x, 5)" },
    { "builtin_sll_imm",   insn_vsllib,  VUB, VUB, " %r1, %r1, 5",    "__builtin_postrisc_vsllib(x, 5)" },
    { "builtin_srl_imm",   insn_vsrlib,  VUB, VUB, " %r1, %r1, 5",    "__builtin_postrisc_vsrlib(x, 5)" },
    { "vec_sll_imm",       insn_vsllib,  VUB, VUB, " %r1, %r1, 5",    "(x << 5)" },
    { "vec_srl_imm",       insn_vsrlib,  VUB, VUB, " %r1, %r1, 5",    "(x >> 5)" },
    { "vec_sra_imm",       insn_vsraib,  VIB, VIB, " %r1, %r1, 5",    "(x >> 5)" },

    { "builtin_sll_imm",   insn_vsllih,  VUH, VUH, " %r1, %r1, 10",   "__builtin_postrisc_vsllih(x, 10)" },
    { "builtin_srl_imm",   insn_vsrlih,  VUH, VUH, " %r1, %r1, 10",   "__builtin_postrisc_vsrlih(x, 10)" },
    { "builtin_sra_imm",   insn_vsraih,  VIH, VIH, " %r1, %r1, 10",   "__builtin_postrisc_vsraih(x, 10)" },
    { "vec_sll_imm",       insn_vsllih,  VUH, VUH, " %r1, %r1, 10",   "(x << 10)" },
    { "vec_srl_imm",       insn_vsrlih,  VUH, VUH, " %r1, %r1, 10",   "(x >> 10)" },
    { "vec_sra_imm",       insn_vsraih,  VIH, VIH, " %r1, %r1, 10",   "(x >> 10)" },

    { "builtin_srl_imm",   insn_vsrliw,  VUW, VUW, " %r1, %r1, 20",   "__builtin_postrisc_vsrliw(x, 20)" },
    { "builtin_sll_imm",   insn_vslliw,  VUW, VUW, " %r1, %r1, 20",   "__builtin_postrisc_vslliw(x, 20)" },
    { "builtin_sra_imm",   insn_vsraiw,  VIW, VIW, " %r1, %r1, 20",   "__builtin_postrisc_vsraiw(x, 20)" },
    { "vec_sll_imm",       insn_vslliw,  VUW, VUW, " %r1, %r1, 20",   "(x << 20)" },
    { "vec_srl_imm",       insn_vsrliw,  VUW, VUW, " %r1, %r1, 20",   "(x >> 20)" },
    { "vec_sra_imm",       insn_vsraiw,  VIW, VIW, " %r1, %r1, 20",   "(x >> 20)" },

    { "builtin_sll_imm",   insn_vsllid,  VUD, VUD, " %r1, %r1, 40",   "__builtin_postrisc_vsllid(x, 40)" },
    { "builtin_srl_imm",   insn_vsrlid,  VUD, VUD, " %r1, %r1, 40",   "__builtin_postrisc_vsrlid(x, 40)" },
    { "builtin_sra_imm",   insn_vsraid,  VID, VID, " %r1, %r1, 40",   "__builtin_postrisc_vsraid(x, 40)" },
    { "vec_sll_imm",       insn_vsllid,  VUD, VUD, " %r1, %r1, 40",   "(x << 40)" },
    { "vec_srl_imm",       insn_vsrlid,  VUD, VUD, " %r1, %r1, 40",   "(x >> 40)" },
    { "vec_sra_imm",       insn_vsraid,  VID, VID, " %r1, %r1, 40",   "(x >> 40)" },

    // splat immediate
    { "vec_splat0",  insn_vsplatib, VUB, VUB,  " %r1, %r1, 0",  "__builtin_postrisc_vsplatb(x, 0)" },
    { "vec_splat1",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 1",  "__builtin_postrisc_vsplath(x, 1)" },
    { "vec_splat2",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 2",  "__builtin_postrisc_vsplath(x, 2)" },
    { "vec_splat3",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 3",  "__builtin_postrisc_vsplath(x, 3)" },
    { "vec_splat4",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 4",  "__builtin_postrisc_vsplath(x, 4)" },
    { "vec_splat5",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 5",  "__builtin_postrisc_vsplath(x, 5)" },
    { "vec_splat6",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 6",  "__builtin_postrisc_vsplath(x, 6)" },

    { "vec_splat0",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 0",  "__builtin_postrisc_vsplatw(x, 0)" },
    { "vec_splat1",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 1",  "__builtin_postrisc_vsplatw(x, 1)" },
    { "vec_splat2",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 2",  "__builtin_postrisc_vsplatw(x, 2)" },
    { "vec_splat3",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 3",  "__builtin_postrisc_vsplatw(x, 3)" },

    { "vec_splat0",  insn_vsplatid, VUD, VUD,  " %r1, %r1, 0",  "__builtin_postrisc_vsplatd(x, 0)" },
    { "vec_splat1",  insn_vsplatid, VUD, VUD,  " %r1, %r1, 1",  "__builtin_postrisc_vsplatd(x, 1)" },

    // rotate right shift immediate
    { "vec_ror_imm",       insn_vsrpib, VUB, VUB, " %r1, %r1, %r1, 5",   "(x <<  3) | (x >> 5)" },
    { "vec_ror_imm",       insn_vsrpih, VUH, VUH, " %r1, %r1, %r1, 6",   "(x << 10) | (x >> 6)" },
    { "vec_ror_imm",       insn_vsrpiw, VUW, VUW, " %r1, %r1, %r1, 12",  "(x << 20) | (x >> 12)" },
    { "vec_ror_imm",       insn_vsrpid, VUD, VUD, " %r1, %r1, %r1, 24",  "(x << 40) | (x >> 24)" },

    // rotate rotate left immediate => funel rotate right, swap args, SIZE-imm
    { "vec_rol_imm",       insn_vsrpib, VUB, VUB, " %r1, %r1, %r1, 3",   "(x >>  3) | (x << 5)" },
    { "vec_rol_imm",       insn_vsrpih, VUH, VUH, " %r1, %r1, %r1, 10",  "(x >> 10) | (x << 6)" },
    { "vec_rol_imm",       insn_vsrpiw, VUW, VUW, " %r1, %r1, %r1, 20",  "(x >> 20) | (x << 12)" },
    { "vec_rol_imm",       insn_vsrpid, VUD, VUD, " %r1, %r1, %r1, 40",  "(x >> 40) | (x << 24)" },

    { "unpack_high_vi8",   insn_vupkhsb, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupkhsb(x)" },
    { "unpack_high_vi16",  insn_vupkhsh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupkhsh(x)" },
    { "unpack_high_vi32",  insn_vupkhsw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupkhsw(x)" },

    { "unpack_low_vi8",    insn_vupklsb, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupklsb(x)" },
    { "unpack_low_vi16",   insn_vupklsh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupklsh(x)" },
    { "unpack_low_vi32",   insn_vupklsw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupklsw(x)" },

    { "unpack_high_vu8",   insn_vupkhub, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupkhub(x)" },
    { "unpack_high_vu16",  insn_vupkhuh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupkhuh(x)" },
    { "unpack_high_vu32",  insn_vupkhuw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupkhuw(x)" },

    { "unpack_low_vu8",    insn_vupklub, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupklub(x)" },
    { "unpack_low_vu16",   insn_vupkluh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupkluh(x)" },
    { "unpack_low_vu32",   insn_vupkluw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupkluw(x)" },
};

CODEGEN_HANDLER(BuiltinUnary)
{
    for (const auto& func : builtin_unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << func.args << '\n';
        if (func.args[0] == '.') {
            out << CHECK_EMPTY << '\n';
            out << CHECK_NEXT << "! BUNDLE\n";
        }
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

static const binary_info builtin_binary_functions[] =
{
    { "setmr",       insn_setmr,   VOID, ID, ID, "%r1, %r2, 2",       "__builtin_postrisc_set_mr(a, b+2)" },
    { "setibr",      insn_setibr,  VOID, ID, ID, "%r1, %r2, 2",       "__builtin_postrisc_set_ibr(a, b+2)" },
    { "setdbr",      insn_setdbr,  VOID, ID, ID, "%r1, %r2, 2",       "__builtin_postrisc_set_dbr(a, b+2)" },
    { "tpa",         insn_tpa,     UD,  PVOID, UD, "%r1, %r1, %r2",     "__builtin_postrisc_tpa(a, b)" },

    { "setitr",      insn_setitr,   VOID, UD, PVOID, "%r1, %r2, %r1",       "__builtin_postrisc_set_itr(a, b, a)" },
    { "setdtr",      insn_setdtr,   VOID, UD, PVOID, "%r1, %r2, %r1",       "__builtin_postrisc_set_dtr(a, b, a)" },

    { "bit_gather",  insn_mbgath, UD, UD, UD,  "%r1, %r1, %r2",  "__builtin_postrisc_bit_gather(a, b)" },
    { "bit_scatter", insn_mbscat, UD, UD, UD,  "%r1, %r1, %r2",  "__builtin_postrisc_bit_scatter(a, b)" },

    { "aesenc",          insn_aesenc,     VID, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_aesenc(a, b)" },
    { "aesenclast",      insn_aesenclast, VID, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_aesenclast(a, b)" },
    { "aesdec",          insn_aesdec,     VID, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_aesdec(a, b)" },
    { "aesdeclast",      insn_aesdeclast, VID, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_aesdeclast(a, b)" },

    { "clmul_ll",        insn_clmul,     VID, VID, VID, "%r1, %r1, %r2, 0",     "__builtin_postrisc_clmul_ll(a, b)" },
    { "clmul_hl",        insn_clmul,     VID, VID, VID, "%r1, %r1, %r2, 1",     "__builtin_postrisc_clmul_hl(a, b)" },
    { "clmul_hh",        insn_clmul,     VID, VID, VID, "%r1, %r1, %r2, 3",     "__builtin_postrisc_clmul_hh(a, b)" },

    { "crc32cb",             insn_crc32cb,   UW,  UW,  UW, "%r1, %r1, %r2",  "__builtin_postrisc_crc32cb(a, b)" },
    { "crc32ch",             insn_crc32ch,   UW,  UW,  UW, "%r1, %r1, %r2",  "__builtin_postrisc_crc32ch(a, b)" },
    { "crc32cw",             insn_crc32cw,   UW,  UW,  UW, "%r1, %r1, %r2",  "__builtin_postrisc_crc32cw(a, b)" },
    { "crc32cd",             insn_crc32cd,   UW,  UW,  UD, "%r1, %r1, %r2",  "__builtin_postrisc_crc32cd(a, b)" },
    { "crc32cq",             insn_crc32cq,   UW,  UW,  UQ, "%r1, %r1, %r2",  "__builtin_postrisc_crc32cq(a, b)" },

    { "builtin_add",         insn_vaddb,     VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vaddb(a, b)" },
    { "builtin_sub",         insn_vsubb,     VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vsubb(a, b)" },
    { "builtin_addc",        insn_vaddcb,    VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vaddcb(a, b)" },
    { "builtin_subc",        insn_vsubcb,    VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vsubcb(a, b)" },
    { "builtin_addo",        insn_vaddob,    VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vaddob(a, b)" },
    { "builtin_subo",        insn_vsubob,    VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vsubob(a, b)" },
    { "builtin_add_sat",     insn_vaddusatb, VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vaddusatb(a, b)" },
    { "builtin_sub_sat",     insn_vsubusatb, VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vsubusatb(a, b)" },
    { "builtin_add_sat",     insn_vaddssatb, VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vaddssatb(a, b)" },
    { "builtin_sub_sat",     insn_vsubssatb, VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vsubssatb(a, b)" },
    { "builtin_min",         insn_vminsb,    VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vminsb(a, b)" },
    { "builtin_max",         insn_vmaxsb,    VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vmaxsb(a, b)" },
    { "builtin_min",         insn_vminub,    VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vminub(a, b)" },
    { "builtin_max",         insn_vmaxub,    VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vmaxub(a, b)" },
    { "builtin_avg",         insn_vavgub,    VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vavgub(a, b)" },
    { "builtin_avg",         insn_vavgsb,    VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vavgsb(a, b)" },
    { "builtin_sll",         insn_vsllb,     VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vsllb(a, b)" },
    { "builtin_srl",         insn_vsrlb,     VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vsrlb(a, b)" },
    { "builtin_sra",         insn_vsrab,     VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vsrab(a, b)" },
    { "builtin_cmp_eq",      insn_vcmpeqb,   VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vcmpeqb(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltsb,  VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltsb(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltub,  VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltub(a, b)" },
    { "builtin_merge_high",  insn_vmergehb,  VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vmergehb(a, b)" },
    { "builtin_merge_low",   insn_vmergelb,  VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_postrisc_vmergelb(a, b)" },
    { "vec_sll",             insn_vsllb,     VUB, VUB, VUB, "%r1, %r1, %r2", "(a << b)" },
    { "vec_srl",             insn_vsrlb,     VUB, VUB, VUB, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_sra",             insn_vsrab,     VIB, VIB, VIB, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_add",             insn_vaddb,     VUB, VUB, VUB, "%r1, %r2, %r1", "(a + b)" },
    { "vec_sub",             insn_vsubb,     VUB, VUB, VUB, "%r1, %r1, %r2", "(a - b)" },

    { "builtin_srp_imm",     insn_vsrpib,    VUB, VUB, VUB, "%r1, %r1, %r2, 5",    "__builtin_postrisc_vsrpib(a, b, 5)" },
    { "builtin_srp_imm",     insn_vsrpih,    VUH, VUH, VUH, "%r1, %r1, %r2, 10",   "__builtin_postrisc_vsrpih(a, b, 10)" },
    { "builtin_srp_imm",     insn_vsrpiw,    VUW, VUW, VUW, "%r1, %r1, %r2, 20",   "__builtin_postrisc_vsrpiw(a, b, 20)" },
    { "builtin_srp_imm",     insn_vsrpid,    VUD, VUD, VUD, "%r1, %r1, %r2, 40",   "__builtin_postrisc_vsrpid(a, b, 40)" },

    // funnel right shift immediate
    { "vec_ror_imm",         insn_vsrpib,    VUB, VUB, VUB, "%r1, %r1, %r2, 5",   "(a <<  3) | (b >> 5)" },
    { "vec_ror_imm",         insn_vsrpih,    VUH, VUH, VUH, "%r1, %r1, %r2, 6",   "(a << 10) | (b >> 6)" },
    { "vec_ror_imm",         insn_vsrpiw,    VUW, VUW, VUW, "%r1, %r1, %r2, 12",  "(a << 20) | (b >> 12)" },
    { "vec_ror_imm",         insn_vsrpid,    VUD, VUD, VUD, "%r1, %r1, %r2, 24",  "(a << 40) | (b >> 24)" },

    // funnel rotate left immediate => funel rotate right, swap args, SIZE-imm
    { "vec_rol_imm",         insn_vsrpib,    VUB, VUB, VUB, "%r1, %r2, %r1, 3",   "(a >>  3) | (b << 5)" },
    { "vec_rol_imm",         insn_vsrpih,    VUH, VUH, VUH, "%r1, %r2, %r1, 10",  "(a >> 10) | (b << 6)" },
    { "vec_rol_imm",         insn_vsrpiw,    VUW, VUW, VUW, "%r1, %r2, %r1, 20",  "(a >> 20) | (b << 12)" },
    { "vec_rol_imm",         insn_vsrpid,    VUD, VUD, VUD, "%r1, %r2, %r1, 40",  "(a >> 40) | (b << 24)" },

    // { "vec_fshr_imm",        insn_vsrpib,    VUB, VUB, VUB, "%r1, %r1, %r2, 5",  "__builtin_elementwise_fshl(a, b, 5)" }, // FIXME: non-immediate

    { "vec_min",             insn_vminsb,    VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxsb,    VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_min",             insn_vminub,    VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxub,    VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_add_sat",         insn_vaddssatb, VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubssatb, VIB, VIB, VIB, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_add_sat",         insn_vaddusatb, VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubusatb, VUB, VUB, VUB, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_avgu",            insn_vavgub,    VUB, VUB, VUB, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },
    { "vec_avgs",            insn_vavgsb,    VIB, VIB, VIB, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },


    { "builtin_add",         insn_vaddh,     VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vaddh(a, b)" },
    { "builtin_sub",         insn_vsubh,     VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vsubh(a, b)" },
    { "builtin_addc",        insn_vaddch,    VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vaddch(a, b)" },
    { "builtin_subc",        insn_vsubch,    VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vsubch(a, b)" },
    { "builtin_addo",        insn_vaddoh,    VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vaddoh(a, b)" },
    { "builtin_subo",        insn_vsuboh,    VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vsuboh(a, b)" },
    { "builtin_add_sat",     insn_vaddusath, VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vaddusath(a, b)" },
    { "builtin_sub_sat",     insn_vsubusath, VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vsubusath(a, b)" },
    { "builtin_add_sat",     insn_vaddssath, VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vaddssath(a, b)" },
    { "builtin_sub_sat",     insn_vsubssath, VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vsubssath(a, b)" },
    { "builtin_min",         insn_vminsh,    VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vminsh(a, b)" },
    { "builtin_max",         insn_vmaxsh,    VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vmaxsh(a, b)" },
    { "builtin_min",         insn_vminuh,    VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vminuh(a, b)" },
    { "builtin_max",         insn_vmaxuh,    VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vmaxuh(a, b)" },
    { "builtin_avg",         insn_vavguh,    VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vavguh(a, b)" },
    { "builtin_avg",         insn_vavgsh,    VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vavgsh(a, b)" },
    { "builtin_sll",         insn_vsllh,     VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vsllh(a, b)" },
    { "builtin_srl",         insn_vsrlh,     VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vsrlh(a, b)" },
    { "builtin_sra",         insn_vsrah,     VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vsrah(a, b)" },
    { "builtin_cmp_eq",      insn_vcmpeqh,   VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vcmpeqh(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltsh,  VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltsh(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltuh,  VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltuh(a, b)" },
    { "builtin_merge_high",  insn_vmergehh,  VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vmergehh(a, b)" },
    { "builtin_merge_low",   insn_vmergelh,  VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_postrisc_vmergelh(a, b)" },
    { "vec_sll",             insn_vsllh,     VUH, VUH, VUH, "%r1, %r1, %r2", "(a << b)" },
    { "vec_srl",             insn_vsrlh,     VUH, VUH, VUH, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_sra",             insn_vsrah,     VIH, VIH, VIH, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_add",             insn_vaddh,     VUH, VUH, VUH, "%r1, %r2, %r1", "(a + b)" },
    { "vec_sub",             insn_vsubh,     VUH, VUH, VUH, "%r1, %r1, %r2", "(a - b)" },
    { "vec_min",             insn_vminsh,    VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxsh,    VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_min",             insn_vminuh,    VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxuh,    VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_add_sat",         insn_vaddssath, VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubssath, VIH, VIH, VIH, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_add_sat",         insn_vaddusath, VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubusath, VUH, VUH, VUH, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_avgu",            insn_vavguh,    VUH, VUH, VUH, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },
    { "vec_avgs",            insn_vavgsh,    VIH, VIH, VIH, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },


    { "builtin_add",         insn_vaddw,     VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vaddw(a, b)" },
    { "builtin_sub",         insn_vsubw,     VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vsubw(a, b)" },
    { "builtin_addc",        insn_vaddcw,    VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vaddcw(a, b)" },
    { "builtin_subc",        insn_vsubcw,    VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vsubcw(a, b)" },
    { "builtin_addo",        insn_vaddow,    VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vaddow(a, b)" },
    { "builtin_subo",        insn_vsubow,    VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vsubow(a, b)" },
    { "builtin_add_sat",     insn_vaddusatw, VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vaddusatw(a, b)" },
    { "builtin_sub_sat",     insn_vsubusatw, VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vsubusatw(a, b)" },
    { "builtin_add_sat",     insn_vaddssatw, VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vaddssatw(a, b)" },
    { "builtin_sub_sat",     insn_vsubssatw, VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vsubssatw(a, b)" },
    { "builtin_min",         insn_vminsw,    VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vminsw(a, b)" },
    { "builtin_max",         insn_vmaxsw,    VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vmaxsw(a, b)" },
    { "builtin_min",         insn_vminuw,    VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vminuw(a, b)" },
    { "builtin_max",         insn_vmaxuw,    VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vmaxuw(a, b)" },
    { "builtin_avg",         insn_vavguw,    VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vavguw(a, b)" },
    { "builtin_avg",         insn_vavgsw,    VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vavgsw(a, b)" },
    { "builtin_sll",         insn_vsllw,     VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vsllw(a, b)" },
    { "builtin_srl",         insn_vsrlw,     VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vsrlw(a, b)" },
    { "builtin_sra",         insn_vsraw,     VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vsraw(a, b)" },
    { "builtin_cmp_eq",      insn_vcmpeqw,   VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vcmpeqw(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltsw,  VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltsw(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltuw,  VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltuw(a, b)" },
    { "builtin_merge_high",  insn_vmergehw,  VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vmergehw(a, b)" },
    { "builtin_merge_low",   insn_vmergelw,  VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_postrisc_vmergelw(a, b)" },
    { "vec_sll",             insn_vsllw,     VUW, VUW, VUW, "%r1, %r1, %r2", "(a << b)" },
    { "vec_srl",             insn_vsrlw,     VUW, VUW, VUW, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_sra",             insn_vsraw,     VIW, VIW, VIW, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_add",             insn_vaddw,     VUW, VUW, VUW, "%r1, %r2, %r1", "(a + b)" },
    { "vec_sub",             insn_vsubw,     VUW, VUW, VUW, "%r1, %r1, %r2", "(a - b)" },
    { "vec_min",             insn_vminsw,    VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxsw,    VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_min",             insn_vminuw,    VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxuw,    VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_add_sat",         insn_vaddssatw, VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubssatw, VIW, VIW, VIW, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_add_sat",         insn_vaddusatw, VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubusatw, VUW, VUW, VUW, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_avgu",            insn_vavguw,    VUW, VUW, VUW, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },
    { "vec_avgs",            insn_vavgsw,    VIW, VIW, VIW, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },


    { "builtin_add",         insn_vaddd,     VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vaddd(a, b)" },
    { "builtin_sub",         insn_vsubd,     VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vsubd(a, b)" },
    { "builtin_addc",        insn_vaddcd,    VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vaddcd(a, b)" },
    { "builtin_subc",        insn_vsubcd,    VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vsubcd(a, b)" },
    { "builtin_addo",        insn_vaddod,    VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vaddod(a, b)" },
    { "builtin_subo",        insn_vsubod,    VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vsubod(a, b)" },
    { "builtin_add_sat",     insn_vaddusatd, VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vaddusatd(a, b)" },
    { "builtin_sub_sat",     insn_vsubusatd, VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vsubusatd(a, b)" },
    { "builtin_add_sat",     insn_vaddssatd, VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vaddssatd(a, b)" },
    { "builtin_sub_sat",     insn_vsubssatd, VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vsubssatd(a, b)" },
    { "builtin_min",         insn_vminsd,    VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vminsd(a, b)" },
    { "builtin_max",         insn_vmaxsd,    VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vmaxsd(a, b)" },
    { "builtin_min",         insn_vminud,    VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vminud(a, b)" },
    { "builtin_max",         insn_vmaxud,    VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vmaxud(a, b)" },
    { "builtin_avg",         insn_vavgud,    VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vavgud(a, b)" },
    { "builtin_avg",         insn_vavgsd,    VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vavgsd(a, b)" },
    { "builtin_sll",         insn_vslld,     VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vslld(a, b)" },
    { "builtin_srl",         insn_vsrld,     VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vsrld(a, b)" },
    { "builtin_sra",         insn_vsrad,     VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vsrad(a, b)" },
    { "builtin_cmp_eq",      insn_vcmpeqd,   VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vcmpeqd(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltsd,  VID, VID, VID, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltsd(a, b)" },
    { "builtin_cmp_lt",      insn_vcmpltud,  VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vcmpltud(a, b)" },
    { "builtin_merge_high",  insn_vmergehd,  VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vmergehd(a, b)" },
    { "builtin_merge_low",   insn_vmergeld,  VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_postrisc_vmergeld(a, b)" },
    { "vec_sll",             insn_vslld,     VUD, VUD, VUD, "%r1, %r1, %r2", "(a << b)" },
    { "vec_srl",             insn_vsrld,     VUD, VUD, VUD, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_sra",             insn_vsrad,     VID, VID, VID, "%r1, %r1, %r2", "(a >> b)" },
    { "vec_add",             insn_vaddd,     VUD, VUD, VUD, "%r1, %r2, %r1", "(a + b)" },
    { "vec_sub",             insn_vsubd,     VUD, VUD, VUD, "%r1, %r1, %r2", "(a - b)" },
    { "vec_min",             insn_vminsd,    VID, VID, VID, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxsd,    VID, VID, VID, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_min",             insn_vminud,    VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_elementwise_min(a, b)" },
    { "vec_max",             insn_vmaxud,    VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_elementwise_max(a, b)" },
    { "vec_add_sat",         insn_vaddssatd, VID, VID, VID, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubssatd, VID, VID, VID, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_add_sat",         insn_vaddusatd, VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_elementwise_add_sat(a, b)" },
    { "vec_sub_sat",         insn_vsubusatd, VUD, VUD, VUD, "%r1, %r1, %r2", "__builtin_elementwise_sub_sat(a, b)" },
    { "vec_avgu",            insn_vavgud,    VUD, VUD, VUD, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },
    { "vec_avgs",            insn_vavgsd,    VID, VID, VID, "%r1, %r2, %r1", "((a | b) & 1) + (a >> 1) + (b >> 1)" },

    // splat variable
    { "vec_splat",  insn_vsplatb, VUB, VUB, UW,  "%r1, %r1, %r2",  "__builtin_postrisc_vsplatb(a, b)" },
    { "vec_splat",  insn_vsplath, VUH, VUH, UW,  "%r1, %r1, %r2",  "__builtin_postrisc_vsplath(a, b)" },
    { "vec_splat",  insn_vsplatw, VUW, VUW, UW,  "%r1, %r1, %r2",  "__builtin_postrisc_vsplatw(a, b)" },
    { "vec_splat",  insn_vsplatd, VUD, VUD, UW,  "%r1, %r1, %r2",  "__builtin_postrisc_vsplatd(a, b)" },

    // rotate left variable
    { "vec_rol",  insn_vslpb, VUB, VUB, VUB,  "%r1, %r1, %r1, %r2",  "(a << b) | (a >> ( 8 - b))" },
    { "vec_rol",  insn_vslph, VUH, VUH, VUH,  "%r1, %r1, %r1, %r2",  "(a << b) | (a >> (16 - b))" },
    { "vec_rol",  insn_vslpw, VUW, VUW, VUW,  "%r1, %r1, %r1, %r2",  "(a << b) | (a >> (32 - b))" },
    { "vec_rol",  insn_vslpd, VUD, VUD, VUD,  "%r1, %r1, %r1, %r2",  "(a << b) | (a >> (64 - b))" },

    // rotate right variable
    { "vec_ror",  insn_vsrpb, VUB, VUB, VUB,  "%r1, %r1, %r1, %r2",  "(a >> b) | (a << ( 8 - b))" },
    { "vec_ror",  insn_vsrph, VUH, VUH, VUH,  "%r1, %r1, %r1, %r2",  "(a >> b) | (a << (16 - b))" },
    { "vec_ror",  insn_vsrpw, VUW, VUW, VUW,  "%r1, %r1, %r1, %r2",  "(a >> b) | (a << (32 - b))" },
    { "vec_ror",  insn_vsrpd, VUD, VUD, VUD,  "%r1, %r1, %r1, %r2",  "(a >> b) | (a << (64 - b))" },

    { "pack_sat",    insn_vpkssath,  VIB, VIH, VIH, "%r1, %r1, %r2",     "__builtin_postrisc_vpkssath(a, b)" },
    { "pack_sat",    insn_vpkssatw,  VIH, VIW, VIW, "%r1, %r1, %r2",     "__builtin_postrisc_vpkssatw(a, b)" },
    { "pack_sat",    insn_vpkssatd,  VIW, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_vpkssatd(a, b)" },

    { "pack_satu",   insn_vpkusath,  VIB, VIH, VIH, "%r1, %r1, %r2",     "__builtin_postrisc_vpkusath(a, b)" },
    { "pack_satu",   insn_vpkusatw,  VIH, VIW, VIW, "%r1, %r1, %r2",     "__builtin_postrisc_vpkusatw(a, b)" },
    { "pack_satu",   insn_vpkusatd,  VIW, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_vpkusatd(a, b)" },

    { "pack_usat",   insn_vpkusatsh, VIB, VIH, VIH, "%r1, %r1, %r2",     "__builtin_postrisc_vpkusatsh(a, b)" },
    { "pack_usat",   insn_vpkusatsw, VIH, VIW, VIW, "%r1, %r1, %r2",     "__builtin_postrisc_vpkusatsw(a, b)" },
    { "pack_usat",   insn_vpkusatsd, VIW, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_vpkusatsd(a, b)" },

    { "pack_mod",    insn_vpkumodh,  VIB, VIH, VIH, "%r1, %r1, %r2",     "__builtin_postrisc_vpkumodh(a, b)" },
    { "pack_mod",    insn_vpkumodw,  VIH, VIW, VIW, "%r1, %r1, %r2",     "__builtin_postrisc_vpkumodw(a, b)" },
    { "pack_mod",    insn_vpkumodd,  VIW, VID, VID, "%r1, %r1, %r2",     "__builtin_postrisc_vpkumodd(a, b)" },
};

CODEGEN_HANDLER(BuiltinBinary)
{
    for (const auto& func : builtin_binary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << func.dst_type << " " << fname.str() << "(" << func.src1_type << " a, " << func.src2_type << " b)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

static const ternary_info builtin_ternary_functions[] =
{
    { "ptc",       insn_ptc,    VOID, PVOID, PVOID, UW, "%r1, %r2, %r3",       "__builtin_postrisc_tlb_purge(a, b, c)" },

    { "postrisc_slp",   insn_vslpb,  VUB, VUB, VUB, VUB, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vslpb(a, b, c)" },
    { "postrisc_slp",   insn_vslph,  VUH, VUH, VUH, VUH, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vslph(a, b, c)" },
    { "postrisc_slp",   insn_vslpw,  VUW, VUW, VUW, VUW, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vslpw(a, b, c)" },
    { "postrisc_slp",   insn_vslpd,  VUD, VUD, VUD, VUD, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vslpd(a, b, c)" },

    { "builtin_fshl",   insn_vslpb,  VUB, VUB, VUB, VUB, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshl(a, b, c)" },
    { "builtin_fshl",   insn_vslph,  VUH, VUH, VUH, VUH, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshl(a, b, c)" },
    { "builtin_fshl",   insn_vslpw,  VUW, VUW, VUW, VUW, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshl(a, b, c)" },
    { "builtin_fshl",   insn_vslpd,  VUD, VUD, VUD, VUD, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshl(a, b, c)" },

    { "postrisc_srp",   insn_vsrpb,  VUB, VUB, VUB, VUB, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vsrpb(a, b, c)" },
    { "postrisc_srp",   insn_vsrph,  VUH, VUH, VUH, VUH, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vsrph(a, b, c)" },
    { "postrisc_srp",   insn_vsrpw,  VUW, VUW, VUW, VUW, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vsrpw(a, b, c)" },
    { "postrisc_srp",   insn_vsrpd,  VUD, VUD, VUD, VUD, "%r1, %r1, %r2, %r3", "__builtin_postrisc_vsrpd(a, b, c)" },

    { "builtin_fshr",   insn_vsrpb,  VUB, VUB, VUB, VUB, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshr(a, b, c)" },
    { "builtin_fshr",   insn_vsrph,  VUH, VUH, VUH, VUH, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshr(a, b, c)" },
    { "builtin_fshr",   insn_vsrpw,  VUW, VUW, VUW, VUW, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshr(a, b, c)" },
    { "builtin_fshr",   insn_vsrpd,  VUD, VUD, VUD, VUD, "%r1, %r1, %r2, %r3", "__builtin_elementwise_fshr(a, b, c)" },
};

CODEGEN_HANDLER(BuiltinTernary)
{
    for (const auto& func : builtin_ternary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << func.dst_type << " " << fname.str() << "(" << func.src1_type << " a, " << func.src2_type << " b," << func.src3_type << " c)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

} // namespace llvm
} // namespace postrisc
