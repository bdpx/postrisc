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

CODEGEN_HANDLER(BuiltinNoArgs)
{
    NoArgs(out, "imm_i32",           insn_ldi, IW, " %r1, -123456789",       "-123456789");
    NoArgs(out, "imm_u32",           insn_ldi, UW, " %r1, 123456789",        "123456789");
    NoArgs(out, "imm_i32_l",         insn_ldi, IW, ".l %r1, -1234567890",    "-1234567890");
    NoArgs(out, "imm_u32_l",         insn_ldi, UW, ".l %r1, 1234567890",        "1234567890");

    NoArgs(out, "imm_i64",           insn_ldi, ID, " %r1, -123456789",             "-123456789");
    NoArgs(out, "imm_u64",           insn_ldi, UD, " %r1, 123456789",              "123456789");
    NoArgs(out, "imm_i64_l",         insn_ldi, ID, ".l %r1, -1234567890123",       "-1234567890123");
    NoArgs(out, "imm_u64_l",         insn_ldi, UD, ".l %r1, 1234567890123",        "1234567890123");
    NoArgs(out, "imm_u64_max",       insn_ldi, UD, ".l %r1, 9223372036854775807",  "0xffffffffffffffffu / 2");

    NoArgs(out, "imm_i128",          insn_ldi, IQ, " %r1, 12345778",              "12345778ull");
    NoArgs(out, "imm_i128_l",        insn_ldi, IQ, ".l %r1, 987654321123456789",  "987654321123456789ull");

    NoArgs(out, "imm_f16",           insn_ldi, FH, " %r1, 12263",                  "0.123456f16");
    NoArgs(out, "imm_f32",           insn_ldi, FS, ".l %r1, 1039980160",             "0.123456f");
    NoArgs(out, "imm_f64",           insn_ldi, FD, ".l %r1, -4629811673862064449",   "-0.123456");

    NoArgs(out, "rscover",           insn_rscover,  VOID,  "",  "__builtin_postrisc_rscover()");
    NoArgs(out, "rsflush",           insn_rsflush,  VOID,  "",  "__builtin_postrisc_rsflush()");
    NoArgs(out, "rsload",            insn_rsload,   VOID,  "",  "__builtin_postrisc_rsload()" );
    NoArgs(out, "readcyclecounter",  insn_getspr,   UD,    " %r1, %icc",       "__builtin_readcyclecounter()");
    NoArgs(out, "readsteadycounter", insn_getspr,   UD,    " %r1, %itc",       "__builtin_readsteadycounter()");
    NoArgs(out, "thread_pointer",    insn_mov,      PVOID, " %r1, %tp",        "__builtin_thread_pointer()");
    NoArgs(out, "stack_pointer",     insn_mov,      PVOID, " %r1, %sp",        "(void*)__builtin_postrisc_stack_pointer()");
    NoArgs(out, "frame_address",     insn_mov,      PVOID, " %r1, %fp",        "(void*)__builtin_frame_address(0)");
    NoArgs(out, "return_address",    insn_mov,      PVOID, " %r1, %r0",        "(void*)__builtin_return_address(0)");
    NoArgs(out, "random",            insn_random,   UD,    " %r1, %gz",        "__builtin_postrisc_random()");
    NoArgs(out, "syscall",           insn_syscall,  VOID,  "",                 "__builtin_postrisc_syscall()");
    NoArgs(out, "sysret",            insn_sysret,   VOID,  "",                 "__builtin_postrisc_sysret()");
    NoArgs(out, "halt",              insn_halt,     VOID,  "",                 "__builtin_postrisc_halt()");
    NoArgs(out, "undef",             insn_undef,    VOID,  "",                 "__builtin_postrisc_undef()");
    NoArgs(out, "rfi",               insn_rfi,      VOID,  "",                 "__builtin_postrisc_rfi()");
    NoArgs(out, "nop1",              insn_nop,      VOID,  " 1234567",                "__builtin_postrisc_nop(1234567)");
    NoArgs(out, "nop2",              insn_nop,      VOID,  ".l 123456789123456789",   "__builtin_postrisc_nop(123456789123456789ULL)");
    NoArgs(out, "nop3",              insn_nop,      VOID,  " 123",                    "__builtin_postrisc_nop(123)");
    NoArgs(out, "nop4",              insn_nop,      VOID,  ".l 123",                  "__builtin_postrisc_nop_l(123)");
    NoArgs(out, "int_imm_1",         insn_int,      VOID,  " %gz, 100",                "__builtin_postrisc_int(100)");
    NoArgs(out, "int_imm_2",         insn_int,      VOID,  " %gz, 200",                "__builtin_postrisc_int(200)");
    NoArgs(out, "int_imm_3",         insn_int,      VOID,  " %gz, 255",                "__builtin_postrisc_int(255)");
    NoArgs(out, "int_imm_4",         insn_int,      VOID,  ".l %gz, 1000000",                "__builtin_postrisc_int(1000000)");
    NoArgs(out, "debugtrap",         insn_int,      VOID,  " %gz, 1",                  "__builtin_debugtrap()");
    NoArgs(out, "trap",              insn_int,      VOID,  " %gz, 5",                  "__builtin_trap()"); // no retf
}

CODEGEN_HANDLER(BuiltinUnary)
{
    Unary(out, "int_param_1",  insn_int,  VOID, ID, " %r1, 33",       "__builtin_postrisc_int(x+33)");
    Unary(out, "int_param_2",  insn_int,  VOID, ID, " %r1, 0",        "__builtin_postrisc_int(x)");
    Unary(out, "int_param_3",  insn_int,  VOID, ID, " %r1, 255",       "__builtin_postrisc_int(x+255)");
    Unary(out, "int_param_4",  insn_int,  VOID, ID, ".l %r1, 1000000",  "__builtin_postrisc_int(x+1000000)");

    Unary(out, "cpuid1",     insn_cpuid,  UD, ID, " %r1, %r1, 33",       "__builtin_postrisc_cpuid(x+33)");
    Unary(out, "cpuid2",     insn_cpuid,  UD, ID, " %r1, %r1, 0",        "__builtin_postrisc_cpuid(x)");
    Unary(out, "cpuid3",     insn_cpuid,  UD, ID, " %r1, %r1, 255",       "__builtin_postrisc_cpuid(x+255)");
    Unary(out, "cpuid4",     insn_cpuid,  UD, ID, ".l %r1, %r1, 1000000",  "__builtin_postrisc_cpuid(x+1000000)");

    Unary(out, "popcount",    insn_cntpop, IW, IW, " %r1, %r1, 31",       "__builtin_popcount(x)");
    Unary(out, "popcount",    insn_cntpop, IW, UW, " %r1, %r1, 31",       "__builtin_popcount(x)");
    Unary(out, "popcount1",   insn_cntpop, IW, ID, " %r1, %r1, 63",       "__builtin_popcountl(x)");
    Unary(out, "popcount1",   insn_cntpop, IW, UD, " %r1, %r1, 63",       "__builtin_popcountl(x)");
    Unary(out, "popcount2",   insn_cntpop, IW, ID, " %r1, %r1, 63",       "__builtin_popcountll(x)");
    Unary(out, "popcount2",   insn_cntpop, IW, UD, " %r1, %r1, 63",       "__builtin_popcountll(x)");

    Unary(out, "ctz",    insn_cnttz, IW, IW, " %r1, %r1, 31",       "__builtin_ctz(x)");
    Unary(out, "ctz",    insn_cnttz, IW, UW, " %r1, %r1, 31",       "__builtin_ctz(x)");
    Unary(out, "ctz1",   insn_cnttz, IW, ID, " %r1, %r1, 63",       "__builtin_ctzl(x)");
    Unary(out, "ctz1",   insn_cnttz, IW, UD, " %r1, %r1, 63",       "__builtin_ctzl(x)");
    Unary(out, "ctz2",   insn_cnttz, IW, ID, " %r1, %r1, 63",       "__builtin_ctzll(x)");
    Unary(out, "ctz2",   insn_cnttz, IW, UD, " %r1, %r1, 63",       "__builtin_ctzll(x)");

    Unary(out, "clz",    insn_cntlz, IW, IW, " %r1, %r1, 31",       "__builtin_clz(x)");
    Unary(out, "clz",    insn_cntlz, IW, UW, " %r1, %r1, 31",       "__builtin_clz(x)");
    Unary(out, "clz1",   insn_cntlz, IW, ID, " %r1, %r1, 63",       "__builtin_clzl(x)");
    Unary(out, "clz1",   insn_cntlz, IW, UD, " %r1, %r1, 63",       "__builtin_clzl(x)");
    Unary(out, "clz2",   insn_cntlz, IW, ID, " %r1, %r1, 63",       "__builtin_clzll(x)");
    Unary(out, "clz2",   insn_cntlz, IW, UD, " %r1, %r1, 63",       "__builtin_clzll(x)");

    Unary(out, "bitreverse32",    insn_permb, IW, IW, " %r1, %r1, 31",       "__builtin_bitreverse32(x)");
    Unary(out, "bitreverse32",    insn_permb, UW, UW, " %r1, %r1, 31",       "__builtin_bitreverse32(x)");
    Unary(out, "bitreverse64",    insn_permb, ID, ID, " %r1, %r1, 63",       "__builtin_bitreverse64(x)");
    Unary(out, "bitreverse64",    insn_permb, UD, UD, " %r1, %r1, 63",       "__builtin_bitreverse64(x)");

    Unary(out, "set_thread_pointer",   insn_mov, VOID, PVOID, " %tp, %r1",       "__builtin_postrisc_set_thread_pointer(x)");

    Unary(out, "int1",             insn_int, VOID, ID, " %r1, 0",         "__builtin_postrisc_int(x)");
    Unary(out, "int2",             insn_int, VOID, ID, " %r1, 100",       "__builtin_postrisc_int(x+100)");

    Unary(out, "random_n",   insn_random,   UD, UD, " %r1, %r1",        "__builtin_postrisc_random_n(x)");

    Unary(out, "aesimc",     insn_aesimc,  VID, VID, " %r1, %r1",     "__builtin_postrisc_aesimc(x)");

    Unary(out, "aeskeygenassist1",     insn_aeskeygenassist,  VID, VID, " %r1, %r1, 100",  "__builtin_postrisc_aeskeygenassist(x, 100)");
    Unary(out, "aeskeygenassist2",     insn_aeskeygenassist,  VID, VID, " %r1, %r1, 255",  "__builtin_postrisc_aeskeygenassist(x, 255)");

    Unary(out, "getmr",     insn_getmr,  UD, ID, " %r1, %r1, 1",       "__builtin_postrisc_get_mr(x+1)");
    Unary(out, "getibr",    insn_getibr, UD, ID, " %r1, %r1, 2",       "__builtin_postrisc_get_ibr(x+2)");
    Unary(out, "getdbr",    insn_getdbr, UD, ID, " %r1, %r1, 3",       "__builtin_postrisc_get_dbr(x+3)");

    Unary(out, "getmr_ext",   insn_getmr,  UD, ID, ".l %r1, %r1, 1000000",       "__builtin_postrisc_get_mr(x+1000000)");
    Unary(out, "getibr_ext",  insn_getibr, UD, ID, ".l %r1, %r1, 2000000",       "__builtin_postrisc_get_ibr(x+2000000)");
    Unary(out, "getdbr_ext",  insn_getdbr, UD, ID, ".l %r1, %r1, 3000000",       "__builtin_postrisc_get_dbr(x+3000000)");

    Unary(out, "dcbf1",      insn_dcbf,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_dcbf(x)");
    Unary(out, "dcbf2",      insn_dcbf,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_dcbf(x+1000)");
    Unary(out, "dcbf3",      insn_dcbf,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_dcbf(x+1000000000)");

    Unary(out, "dcbt1",      insn_dcbt,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_dcbt(x)");
    Unary(out, "dcbt2",      insn_dcbt,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_dcbt(x+1000)");
    Unary(out, "dcbt3",      insn_dcbt,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_dcbt(x+1000000000)");

    Unary(out, "dcbi1",      insn_dcbi,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_dcbi(x)");
    Unary(out, "dcbi2",      insn_dcbi,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_dcbi(x+1000)");
    Unary(out, "dcbi3",      insn_dcbi,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_dcbi(x+1000000000)");

    Unary(out, "icbi1",      insn_icbi,    VOID, PVOID, " %r1, 0",              "__builtin_postrisc_icbi(x)");
    Unary(out, "icbi2",      insn_icbi,    VOID, PVOID, " %r1, 1000",           "__builtin_postrisc_icbi(x+1000)");
    Unary(out, "icbi3",      insn_icbi,    VOID, PVOID, ".l %r1, 1000000000",   "__builtin_postrisc_icbi(x+1000000000)");

    Unary(out, "builtin_sra_imm",   insn_vsraib,  VIB, VIB, " %r1, %r1, 5",    "__builtin_postrisc_vsraib(x, 5)");
    Unary(out, "builtin_sll_imm",   insn_vsllib,  VUB, VUB, " %r1, %r1, 5",    "__builtin_postrisc_vsllib(x, 5)");
    Unary(out, "builtin_srl_imm",   insn_vsrlib,  VUB, VUB, " %r1, %r1, 5",    "__builtin_postrisc_vsrlib(x, 5)");
    Unary(out, "vec_sll_imm",       insn_vsllib,  VUB, VUB, " %r1, %r1, 5",    "(x << 5)");
    Unary(out, "vec_srl_imm",       insn_vsrlib,  VUB, VUB, " %r1, %r1, 5",    "(x >> 5)");
    Unary(out, "vec_sra_imm",       insn_vsraib,  VIB, VIB, " %r1, %r1, 5",    "(x >> 5)");

    Unary(out, "builtin_sll_imm",   insn_vsllih,  VUH, VUH, " %r1, %r1, 10",   "__builtin_postrisc_vsllih(x, 10)");
    Unary(out, "builtin_srl_imm",   insn_vsrlih,  VUH, VUH, " %r1, %r1, 10",   "__builtin_postrisc_vsrlih(x, 10)");
    Unary(out, "builtin_sra_imm",   insn_vsraih,  VIH, VIH, " %r1, %r1, 10",   "__builtin_postrisc_vsraih(x, 10)");
    Unary(out, "vec_sll_imm",       insn_vsllih,  VUH, VUH, " %r1, %r1, 10",   "(x << 10)");
    Unary(out, "vec_srl_imm",       insn_vsrlih,  VUH, VUH, " %r1, %r1, 10",   "(x >> 10)");
    Unary(out, "vec_sra_imm",       insn_vsraih,  VIH, VIH, " %r1, %r1, 10",   "(x >> 10)");

    Unary(out, "builtin_srl_imm",   insn_vsrliw,  VUW, VUW, " %r1, %r1, 20",   "__builtin_postrisc_vsrliw(x, 20)");
    Unary(out, "builtin_sll_imm",   insn_vslliw,  VUW, VUW, " %r1, %r1, 20",   "__builtin_postrisc_vslliw(x, 20)");
    Unary(out, "builtin_sra_imm",   insn_vsraiw,  VIW, VIW, " %r1, %r1, 20",   "__builtin_postrisc_vsraiw(x, 20)");
    Unary(out, "vec_sll_imm",       insn_vslliw,  VUW, VUW, " %r1, %r1, 20",   "(x << 20)");
    Unary(out, "vec_srl_imm",       insn_vsrliw,  VUW, VUW, " %r1, %r1, 20",   "(x >> 20)");
    Unary(out, "vec_sra_imm",       insn_vsraiw,  VIW, VIW, " %r1, %r1, 20",   "(x >> 20)");

    Unary(out, "builtin_sll_imm",   insn_vsllid,  VUD, VUD, " %r1, %r1, 40",   "__builtin_postrisc_vsllid(x, 40)");
    Unary(out, "builtin_srl_imm",   insn_vsrlid,  VUD, VUD, " %r1, %r1, 40",   "__builtin_postrisc_vsrlid(x, 40)");
    Unary(out, "builtin_sra_imm",   insn_vsraid,  VID, VID, " %r1, %r1, 40",   "__builtin_postrisc_vsraid(x, 40)");
    Unary(out, "vec_sll_imm",       insn_vsllid,  VUD, VUD, " %r1, %r1, 40",   "(x << 40)");
    Unary(out, "vec_srl_imm",       insn_vsrlid,  VUD, VUD, " %r1, %r1, 40",   "(x >> 40)");
    Unary(out, "vec_sra_imm",       insn_vsraid,  VID, VID, " %r1, %r1, 40",   "(x >> 40)");

    Unary(out, "vec_cmp_eq_zero",  insn_vcmpeqb,   VIB, VIB, " %r1, %r1, %gz", "(x == 0)");
    Unary(out, "vec_cmp_ne_zero",  insn_vcmpneb,   VIB, VIB, " %r1, %r1, %gz", "(x != 0)");
    Unary(out, "vec_cmp_gt_zero",  insn_vcmpltsb,  VIB, VIB, " %r1, %gz, %r1", "(x > 0)");
    Unary(out, "vec_cmp_le_zero",  insn_vcmplesb,  VIB, VIB, " %r1, %r1, %gz", "(x <= 0)");
    Unary(out, "vec_cmp_ge_zero",  insn_vcmplesb,  VIB, VIB, " %r1, %gz, %r1", "(x >= 0)");

    Unary(out, "vec_cmp_eq_zero",  insn_vcmpeqh,   VIH, VIH, " %r1, %r1, %gz", "(x == 0)");
    Unary(out, "vec_cmp_ne_zero",  insn_vcmpneh,   VIH, VIH, " %r1, %r1, %gz", "(x != 0)");
    Unary(out, "vec_cmp_gt_zero",  insn_vcmpltsh,  VIH, VIH, " %r1, %gz, %r1", "(x > 0)");
    Unary(out, "vec_cmp_le_zero",  insn_vcmplesh,  VIH, VIH, " %r1, %r1, %gz", "(x <= 0)");
    Unary(out, "vec_cmp_ge_zero",  insn_vcmplesh,  VIH, VIH, " %r1, %gz, %r1", "(x >= 0)");

    Unary(out, "vec_cmp_eq_zero",  insn_vcmpeqw,   VIW, VIW, " %r1, %r1, %gz", "(x == 0)");
    Unary(out, "vec_cmp_ne_zero",  insn_vcmpnew,   VIW, VIW, " %r1, %r1, %gz", "(x != 0)");
    Unary(out, "vec_cmp_gt_zero",  insn_vcmpltsw,  VIW, VIW, " %r1, %gz, %r1", "(x > 0)");
    Unary(out, "vec_cmp_le_zero",  insn_vcmplesw,  VIW, VIW, " %r1, %r1, %gz", "(x <= 0)");
    Unary(out, "vec_cmp_ge_zero",  insn_vcmplesw,  VIW, VIW, " %r1, %gz, %r1", "(x >= 0)");

    Unary(out, "vec_cmp_eq_zero",  insn_vcmpeqd,   VID, VID, " %r1, %r1, %gz", "(x == 0)");
    Unary(out, "vec_cmp_ne_zero",  insn_vcmpned,   VID, VID, " %r1, %r1, %gz", "(x != 0)");
    Unary(out, "vec_cmp_gt_zero",  insn_vcmpltsd,  VID, VID, " %r1, %gz, %r1", "(x > 0)");
    Unary(out, "vec_cmp_le_zero",  insn_vcmplesd,  VID, VID, " %r1, %r1, %gz", "(x <= 0)");
    Unary(out, "vec_cmp_ge_zero",  insn_vcmplesd,  VID, VID, " %r1, %gz, %r1", "(x >= 0)");

    // less-than-zero mask is computed via algebraic shift
    Unary(out, "vec_cmp_lt_zero",  insn_vsraib,  VIB, VIB, " %r1, %r1, 7",  "(x < 0)");
    Unary(out, "vec_cmp_lt_zero",  insn_vsraih,  VIH, VIH, " %r1, %r1, 15", "(x < 0)");
    Unary(out, "vec_cmp_lt_zero",  insn_vsraiw,  VIW, VIW, " %r1, %r1, 31", "(x < 0)");
    Unary(out, "vec_cmp_lt_zero",  insn_vsraid,  VID, VID, " %r1, %r1, 63", "(x < 0)");

    // splat immediate
    Unary(out, "vec_splat0",  insn_vsplatib, VUB, VUB,  " %r1, %r1, 0",  "__builtin_postrisc_vsplatb(x, 0)");
    Unary(out, "vec_splat1",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 1",  "__builtin_postrisc_vsplath(x, 1)");
    Unary(out, "vec_splat2",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 2",  "__builtin_postrisc_vsplath(x, 2)");
    Unary(out, "vec_splat3",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 3",  "__builtin_postrisc_vsplath(x, 3)");
    Unary(out, "vec_splat4",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 4",  "__builtin_postrisc_vsplath(x, 4)");
    Unary(out, "vec_splat5",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 5",  "__builtin_postrisc_vsplath(x, 5)");
    Unary(out, "vec_splat6",  insn_vsplatih, VUH, VUH,  " %r1, %r1, 6",  "__builtin_postrisc_vsplath(x, 6)");

    Unary(out, "vec_splat0",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 0",  "__builtin_postrisc_vsplatw(x, 0)");
    Unary(out, "vec_splat1",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 1",  "__builtin_postrisc_vsplatw(x, 1)");
    Unary(out, "vec_splat2",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 2",  "__builtin_postrisc_vsplatw(x, 2)");
    Unary(out, "vec_splat3",  insn_vsplatiw, VUW, VUW,  " %r1, %r1, 3",  "__builtin_postrisc_vsplatw(x, 3)");

    Unary(out, "vec_splat0",  insn_vsplatid, VUD, VUD,  " %r1, %r1, 0",  "__builtin_postrisc_vsplatd(x, 0)");
    Unary(out, "vec_splat1",  insn_vsplatid, VUD, VUD,  " %r1, %r1, 1",  "__builtin_postrisc_vsplatd(x, 1)");

    // splat immediate
    //Unary(out, "builtin_splat0",  insn_vsplatib, UB, VUB,  " %r1, %r1, 0",  "x[0]");
    //Unary(out, "builtin_splat1",  insn_vsplatih, UH, VUH,  " %r1, %r1, 1",  "x[1]");
    //Unary(out, "builtin_splat2",  insn_vsplatih, UH, VUH,  " %r1, %r1, 2",  "x[2]");
    //Unary(out, "builtin_splat3",  insn_vsplatih, UH, VUH,  " %r1, %r1, 3",  "x[3]");
    //Unary(out, "builtin_splat4",  insn_vsplatih, UH, VUH,  " %r1, %r1, 4",  "x[4]");
    //Unary(out, "builtin_splat5",  insn_vsplatih, UH, VUH,  " %r1, %r1, 5",  "x[5]");
    //Unary(out, "builtin_splat6",  insn_vsplatih, UH, VUH,  " %r1, %r1, 6",  "x[6]");

    Unary(out, "builtin_splat0",  insn_vsplatiw, UW, VUW,  " %r1, %r1, 0",  "x[0]");
    Unary(out, "builtin_splat1",  insn_vsplatiw, UW, VUW,  " %r1, %r1, 1",  "x[1]");
    Unary(out, "builtin_splat2",  insn_vsplatiw, UW, VUW,  " %r1, %r1, 2",  "x[2]");
    Unary(out, "builtin_splat3",  insn_vsplatiw, UW, VUW,  " %r1, %r1, 3",  "x[3]");

    Unary(out, "builtin_splat0",  insn_vsplatid, UD, VUD,  " %r1, %r1, 0",  "x[0]");
    Unary(out, "builtin_splat1",  insn_vsplatid, UD, VUD,  " %r1, %r1, 1",  "x[1]");

    Unary(out, "builtin_splat0",  insn_vsplatiw, FS, VFS,  " %r1, %r1, 0",  "x[0]");
    Unary(out, "builtin_splat1",  insn_vsplatiw, FS, VFS,  " %r1, %r1, 1",  "x[1]");
    Unary(out, "builtin_splat2",  insn_vsplatiw, FS, VFS,  " %r1, %r1, 2",  "x[2]");
    Unary(out, "builtin_splat3",  insn_vsplatiw, FS, VFS,  " %r1, %r1, 3",  "x[3]");

    Unary(out, "builtin_splat0",  insn_vsplatid, FD, VFD,  " %r1, %r1, 0",  "x[0]");
    Unary(out, "builtin_splat1",  insn_vsplatid, FD, VFD,  " %r1, %r1, 1",  "x[1]");

    // rotate right shift immediate
    Unary(out, "vec_ror_imm",       insn_vsrpib, VUB, VUB, " %r1, %r1, %r1, 5",   "(x <<  3) | (x >> 5)");
    Unary(out, "vec_ror_imm",       insn_vsrpih, VUH, VUH, " %r1, %r1, %r1, 6",   "(x << 10) | (x >> 6)");
    Unary(out, "vec_ror_imm",       insn_vsrpiw, VUW, VUW, " %r1, %r1, %r1, 12",  "(x << 20) | (x >> 12)");
    Unary(out, "vec_ror_imm",       insn_vsrpid, VUD, VUD, " %r1, %r1, %r1, 24",  "(x << 40) | (x >> 24)");

    // rotate rotate left immediate => funel rotate right, swap args, SIZE-imm
    Unary(out, "vec_rol_imm",       insn_vsrpib, VUB, VUB, " %r1, %r1, %r1, 3",   "(x >>  3) | (x << 5)");
    Unary(out, "vec_rol_imm",       insn_vsrpih, VUH, VUH, " %r1, %r1, %r1, 10",  "(x >> 10) | (x << 6)");
    Unary(out, "vec_rol_imm",       insn_vsrpiw, VUW, VUW, " %r1, %r1, %r1, 20",  "(x >> 20) | (x << 12)");
    Unary(out, "vec_rol_imm",       insn_vsrpid, VUD, VUD, " %r1, %r1, %r1, 40",  "(x >> 40) | (x << 24)");

    Unary(out, "unpack_high_vi8",   insn_vupkhsb, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupkhsb(x)");
    Unary(out, "unpack_high_vi16",  insn_vupkhsh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupkhsh(x)");
    Unary(out, "unpack_high_vi32",  insn_vupkhsw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupkhsw(x)");

    Unary(out, "unpack_low_vi8",    insn_vupklsb, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupklsb(x)");
    Unary(out, "unpack_low_vi16",   insn_vupklsh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupklsh(x)");
    Unary(out, "unpack_low_vi32",   insn_vupklsw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupklsw(x)");

    Unary(out, "unpack_high_vu8",   insn_vupkhub, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupkhub(x)");
    Unary(out, "unpack_high_vu16",  insn_vupkhuh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupkhuh(x)");
    Unary(out, "unpack_high_vu32",  insn_vupkhuw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupkhuw(x)");

    Unary(out, "unpack_low_vu8",    insn_vupklub, VIH, VIB, " %r1, %r1",  "__builtin_postrisc_vupklub(x)");
    Unary(out, "unpack_low_vu16",   insn_vupkluh, VIW, VIH, " %r1, %r1",  "__builtin_postrisc_vupkluh(x)");
    Unary(out, "unpack_low_vu32",   insn_vupkluw, VID, VIW, " %r1, %r1",  "__builtin_postrisc_vupkluw(x)");
}

CODEGEN_HANDLER(BuiltinBinary)
{
    static const char R112[] = " %r1, %r1, %r2";
    static const char R121[] = " %r1, %r2, %r1";
    static const char R1112[] = " %r1, %r1, %r1, %r2";

    Binary(out, "setmr",       insn_setmr,   VOID, ID, ID, " %r1, %r2, 2",       "__builtin_postrisc_set_mr(a, b+2)");
    Binary(out, "setibr",      insn_setibr,  VOID, ID, ID, " %r1, %r2, 2",       "__builtin_postrisc_set_ibr(a, b+2)");
    Binary(out, "setdbr",      insn_setdbr,  VOID, ID, ID, " %r1, %r2, 2",       "__builtin_postrisc_set_dbr(a, b+2)");

    Binary(out, "setmr_ext",   insn_setmr,   VOID, ID, ID, ".l %r1, %r2, 2000000",       "__builtin_postrisc_set_mr(a, b+2000000)");
    Binary(out, "setibr_ext",  insn_setibr,  VOID, ID, ID, ".l %r1, %r2, 2000000",       "__builtin_postrisc_set_ibr(a, b+2000000)");
    Binary(out, "setdbr_ext",  insn_setdbr,  VOID, ID, ID, ".l %r1, %r2, 2000000",       "__builtin_postrisc_set_dbr(a, b+2000000)");

    Binary(out, "tpa",         insn_tpa,     UD,  PVOID, UD, R112,     "__builtin_postrisc_tpa(a, b)");

    Binary(out, "setitr",      insn_setitr,   VOID, UD, PVOID, R121,       "__builtin_postrisc_set_itr(a, b, a)");
    Binary(out, "setdtr",      insn_setdtr,   VOID, UD, PVOID, R121,       "__builtin_postrisc_set_dtr(a, b, a)");

    Binary(out, "bit_gather",  insn_mbgath, UD, UD, UD,  R112,  "__builtin_postrisc_bit_gather(a, b)");
    Binary(out, "bit_scatter", insn_mbscat, UD, UD, UD,  R112,  "__builtin_postrisc_bit_scatter(a, b)");

    Binary(out, "aesenc",          insn_aesenc,     VID, VID, VID, R112,  "__builtin_postrisc_aesenc(a, b)");
    Binary(out, "aesenclast",      insn_aesenclast, VID, VID, VID, R112,  "__builtin_postrisc_aesenclast(a, b)");
    Binary(out, "aesdec",          insn_aesdec,     VID, VID, VID, R112,  "__builtin_postrisc_aesdec(a, b)");
    Binary(out, "aesdeclast",      insn_aesdeclast, VID, VID, VID, R112,  "__builtin_postrisc_aesdeclast(a, b)");

    Binary(out, "clmul_ll",        insn_clmul,     VID, VID, VID, " %r1, %r1, %r2, 0",  "__builtin_postrisc_clmul_ll(a, b)");
    Binary(out, "clmul_hl",        insn_clmul,     VID, VID, VID, " %r1, %r1, %r2, 1",  "__builtin_postrisc_clmul_hl(a, b)");
    Binary(out, "clmul_hh",        insn_clmul,     VID, VID, VID, " %r1, %r1, %r2, 3",  "__builtin_postrisc_clmul_hh(a, b)");

    Binary(out, "crc32cb",             insn_crc32cb,   UW,  UW,  UW, R112,  "__builtin_postrisc_crc32cb(a, b)");
    Binary(out, "crc32ch",             insn_crc32ch,   UW,  UW,  UW, R112,  "__builtin_postrisc_crc32ch(a, b)");
    Binary(out, "crc32cw",             insn_crc32cw,   UW,  UW,  UW, R112,  "__builtin_postrisc_crc32cw(a, b)");
    Binary(out, "crc32cd",             insn_crc32cd,   UW,  UW,  UD, R112,  "__builtin_postrisc_crc32cd(a, b)");
    Binary(out, "crc32cq",             insn_crc32cq,   UW,  UW,  UQ, R112,  "__builtin_postrisc_crc32cq(a, b)");

    Binary(out, "builtin_add",         insn_vaddb,     VUB, VUB, VUB, R112, "__builtin_postrisc_vaddb(a, b)");
    Binary(out, "builtin_sub",         insn_vsubb,     VUB, VUB, VUB, R112, "__builtin_postrisc_vsubb(a, b)");
    Binary(out, "builtin_addc",        insn_vaddcb,    VUB, VUB, VUB, R112, "__builtin_postrisc_vaddcb(a, b)");
    Binary(out, "builtin_subc",        insn_vsubcb,    VUB, VUB, VUB, R112, "__builtin_postrisc_vsubcb(a, b)");
    Binary(out, "builtin_addo",        insn_vaddob,    VIB, VIB, VIB, R112, "__builtin_postrisc_vaddob(a, b)");
    Binary(out, "builtin_subo",        insn_vsubob,    VIB, VIB, VIB, R112, "__builtin_postrisc_vsubob(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddusatb, VUB, VUB, VUB, R112, "__builtin_postrisc_vaddusatb(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubusatb, VUB, VUB, VUB, R112, "__builtin_postrisc_vsubusatb(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddssatb, VIB, VIB, VIB, R112, "__builtin_postrisc_vaddssatb(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubssatb, VIB, VIB, VIB, R112, "__builtin_postrisc_vsubssatb(a, b)");
    Binary(out, "builtin_min",         insn_vminsb,    VIB, VIB, VIB, R112, "__builtin_postrisc_vminsb(a, b)");
    Binary(out, "builtin_max",         insn_vmaxsb,    VIB, VIB, VIB, R112, "__builtin_postrisc_vmaxsb(a, b)");
    Binary(out, "builtin_min",         insn_vminub,    VUB, VUB, VUB, R112, "__builtin_postrisc_vminub(a, b)");
    Binary(out, "builtin_max",         insn_vmaxub,    VUB, VUB, VUB, R112, "__builtin_postrisc_vmaxub(a, b)");
    Binary(out, "builtin_avg",         insn_vavgub,    VUB, VUB, VUB, R112, "__builtin_postrisc_vavgub(a, b)");
    Binary(out, "builtin_avg",         insn_vavgsb,    VIB, VIB, VIB, R112, "__builtin_postrisc_vavgsb(a, b)");
    Binary(out, "builtin_sll",         insn_vsllb,     VUB, VUB, VUB, R112, "__builtin_postrisc_vsllb(a, b)");
    Binary(out, "builtin_srl",         insn_vsrlb,     VUB, VUB, VUB, R112, "__builtin_postrisc_vsrlb(a, b)");
    Binary(out, "builtin_sra",         insn_vsrab,     VIB, VIB, VIB, R112, "__builtin_postrisc_vsrab(a, b)");
    Binary(out, "builtin_merge_high",  insn_vmergehb,  VUB, VUB, VUB, R112, "__builtin_postrisc_vmergehb(a, b)");
    Binary(out, "builtin_merge_low",   insn_vmergelb,  VUB, VUB, VUB, R112, "__builtin_postrisc_vmergelb(a, b)");
    Binary(out, "vec_sll",             insn_vsllb,     VUB, VUB, VUB, R112, "(a << b)");
    Binary(out, "vec_srl",             insn_vsrlb,     VUB, VUB, VUB, R112, "(a >> b)");
    Binary(out, "vec_sra",             insn_vsrab,     VIB, VIB, VIB, R112, "(a >> b)");
    Binary(out, "vec_add",             insn_vaddb,     VUB, VUB, VUB, R121, "(a + b)");
    Binary(out, "vec_sub",             insn_vsubb,     VUB, VUB, VUB, R112, "(a - b)");

    Binary(out, "builtin_srp_imm",     insn_vsrpib,    VUB, VUB, VUB, " %r1, %r1, %r2, 5",    "__builtin_postrisc_vsrpib(a, b, 5)");
    Binary(out, "builtin_srp_imm",     insn_vsrpih,    VUH, VUH, VUH, " %r1, %r1, %r2, 10",   "__builtin_postrisc_vsrpih(a, b, 10)");
    Binary(out, "builtin_srp_imm",     insn_vsrpiw,    VUW, VUW, VUW, " %r1, %r1, %r2, 20",   "__builtin_postrisc_vsrpiw(a, b, 20)");
    Binary(out, "builtin_srp_imm",     insn_vsrpid,    VUD, VUD, VUD, " %r1, %r1, %r2, 40",   "__builtin_postrisc_vsrpid(a, b, 40)");

    // funnel right shift immediate
    Binary(out, "vec_ror_imm",         insn_vsrpib,    VUB, VUB, VUB, " %r1, %r1, %r2, 5",   "(a <<  3) | (b >> 5)");
    Binary(out, "vec_ror_imm",         insn_vsrpih,    VUH, VUH, VUH, " %r1, %r1, %r2, 6",   "(a << 10) | (b >> 6)");
    Binary(out, "vec_ror_imm",         insn_vsrpiw,    VUW, VUW, VUW, " %r1, %r1, %r2, 12",  "(a << 20) | (b >> 12)");
    Binary(out, "vec_ror_imm",         insn_vsrpid,    VUD, VUD, VUD, " %r1, %r1, %r2, 24",  "(a << 40) | (b >> 24)");

    // funnel rotate left immediate => funel rotate right, swap args, SIZE-imm
    Binary(out, "vec_rol_imm",         insn_vsrpib,    VUB, VUB, VUB, " %r1, %r2, %r1, 3",   "(a >>  3) | (b << 5)");
    Binary(out, "vec_rol_imm",         insn_vsrpih,    VUH, VUH, VUH, " %r1, %r2, %r1, 10",  "(a >> 10) | (b << 6)");
    Binary(out, "vec_rol_imm",         insn_vsrpiw,    VUW, VUW, VUW, " %r1, %r2, %r1, 20",  "(a >> 20) | (b << 12)");
    Binary(out, "vec_rol_imm",         insn_vsrpid,    VUD, VUD, VUD, " %r1, %r2, %r1, 40",  "(a >> 40) | (b << 24)");

    // Binary(out, "vec_fshr_imm",        insn_vsrpib,    VUB, VUB, VUB, " %r1, %r1, %r2, 5",  "__builtin_elementwise_fshl(a, b, 5)"); // FIXME: non-immediate

    Binary(out, "vec_min",             insn_vminsb,    VIB, VIB, VIB, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxsb,    VIB, VIB, VIB, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_min",             insn_vminub,    VUB, VUB, VUB, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxub,    VUB, VUB, VUB, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddssatb, VIB, VIB, VIB, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubssatb, VIB, VIB, VIB, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddusatb, VUB, VUB, VUB, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubusatb, VUB, VUB, VUB, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_avgu",            insn_vavgub,    VUB, VUB, VUB, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");
    Binary(out, "vec_avgs",            insn_vavgsb,    VIB, VIB, VIB, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");


    Binary(out, "builtin_add",         insn_vaddh,     VUH, VUH, VUH, R112, "__builtin_postrisc_vaddh(a, b)");
    Binary(out, "builtin_sub",         insn_vsubh,     VUH, VUH, VUH, R112, "__builtin_postrisc_vsubh(a, b)");
    Binary(out, "builtin_addc",        insn_vaddch,    VUH, VUH, VUH, R112, "__builtin_postrisc_vaddch(a, b)");
    Binary(out, "builtin_subc",        insn_vsubch,    VUH, VUH, VUH, R112, "__builtin_postrisc_vsubch(a, b)");
    Binary(out, "builtin_addo",        insn_vaddoh,    VIH, VIH, VIH, R112, "__builtin_postrisc_vaddoh(a, b)");
    Binary(out, "builtin_subo",        insn_vsuboh,    VIH, VIH, VIH, R112, "__builtin_postrisc_vsuboh(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddusath, VUH, VUH, VUH, R112, "__builtin_postrisc_vaddusath(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubusath, VUH, VUH, VUH, R112, "__builtin_postrisc_vsubusath(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddssath, VIH, VIH, VIH, R112, "__builtin_postrisc_vaddssath(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubssath, VIH, VIH, VIH, R112, "__builtin_postrisc_vsubssath(a, b)");
    Binary(out, "builtin_min",         insn_vminsh,    VIH, VIH, VIH, R112, "__builtin_postrisc_vminsh(a, b)");
    Binary(out, "builtin_max",         insn_vmaxsh,    VIH, VIH, VIH, R112, "__builtin_postrisc_vmaxsh(a, b)");
    Binary(out, "builtin_min",         insn_vminuh,    VUH, VUH, VUH, R112, "__builtin_postrisc_vminuh(a, b)");
    Binary(out, "builtin_max",         insn_vmaxuh,    VUH, VUH, VUH, R112, "__builtin_postrisc_vmaxuh(a, b)");
    Binary(out, "builtin_avg",         insn_vavguh,    VUH, VUH, VUH, R112, "__builtin_postrisc_vavguh(a, b)");
    Binary(out, "builtin_avg",         insn_vavgsh,    VIH, VIH, VIH, R112, "__builtin_postrisc_vavgsh(a, b)");
    Binary(out, "builtin_sll",         insn_vsllh,     VUH, VUH, VUH, R112, "__builtin_postrisc_vsllh(a, b)");
    Binary(out, "builtin_srl",         insn_vsrlh,     VUH, VUH, VUH, R112, "__builtin_postrisc_vsrlh(a, b)");
    Binary(out, "builtin_sra",         insn_vsrah,     VIH, VIH, VIH, R112, "__builtin_postrisc_vsrah(a, b)");
    Binary(out, "builtin_merge_high",  insn_vmergehh,  VUH, VUH, VUH, R112, "__builtin_postrisc_vmergehh(a, b)");
    Binary(out, "builtin_merge_low",   insn_vmergelh,  VUH, VUH, VUH, R112, "__builtin_postrisc_vmergelh(a, b)");
    Binary(out, "vec_sll",             insn_vsllh,     VUH, VUH, VUH, R112, "(a << b)");
    Binary(out, "vec_srl",             insn_vsrlh,     VUH, VUH, VUH, R112, "(a >> b)");
    Binary(out, "vec_sra",             insn_vsrah,     VIH, VIH, VIH, R112, "(a >> b)");
    Binary(out, "vec_add",             insn_vaddh,     VUH, VUH, VUH, R121, "(a + b)");
    Binary(out, "vec_sub",             insn_vsubh,     VUH, VUH, VUH, R112, "(a - b)");
    Binary(out, "vec_min",             insn_vminsh,    VIH, VIH, VIH, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxsh,    VIH, VIH, VIH, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_min",             insn_vminuh,    VUH, VUH, VUH, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxuh,    VUH, VUH, VUH, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddssath, VIH, VIH, VIH, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubssath, VIH, VIH, VIH, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddusath, VUH, VUH, VUH, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubusath, VUH, VUH, VUH, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_avgu",            insn_vavguh,    VUH, VUH, VUH, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");
    Binary(out, "vec_avgs",            insn_vavgsh,    VIH, VIH, VIH, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");


    Binary(out, "builtin_add",         insn_vaddw,     VUW, VUW, VUW, R112, "__builtin_postrisc_vaddw(a, b)");
    Binary(out, "builtin_sub",         insn_vsubw,     VUW, VUW, VUW, R112, "__builtin_postrisc_vsubw(a, b)");
    Binary(out, "builtin_addc",        insn_vaddcw,    VUW, VUW, VUW, R112, "__builtin_postrisc_vaddcw(a, b)");
    Binary(out, "builtin_subc",        insn_vsubcw,    VUW, VUW, VUW, R112, "__builtin_postrisc_vsubcw(a, b)");
    Binary(out, "builtin_addo",        insn_vaddow,    VIW, VIW, VIW, R112, "__builtin_postrisc_vaddow(a, b)");
    Binary(out, "builtin_subo",        insn_vsubow,    VIW, VIW, VIW, R112, "__builtin_postrisc_vsubow(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddusatw, VUW, VUW, VUW, R112, "__builtin_postrisc_vaddusatw(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubusatw, VUW, VUW, VUW, R112, "__builtin_postrisc_vsubusatw(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddssatw, VIW, VIW, VIW, R112, "__builtin_postrisc_vaddssatw(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubssatw, VIW, VIW, VIW, R112, "__builtin_postrisc_vsubssatw(a, b)");
    Binary(out, "builtin_min",         insn_vminsw,    VIW, VIW, VIW, R112, "__builtin_postrisc_vminsw(a, b)");
    Binary(out, "builtin_max",         insn_vmaxsw,    VIW, VIW, VIW, R112, "__builtin_postrisc_vmaxsw(a, b)");
    Binary(out, "builtin_min",         insn_vminuw,    VUW, VUW, VUW, R112, "__builtin_postrisc_vminuw(a, b)");
    Binary(out, "builtin_max",         insn_vmaxuw,    VUW, VUW, VUW, R112, "__builtin_postrisc_vmaxuw(a, b)");
    Binary(out, "builtin_avg",         insn_vavguw,    VUW, VUW, VUW, R112, "__builtin_postrisc_vavguw(a, b)");
    Binary(out, "builtin_avg",         insn_vavgsw,    VIW, VIW, VIW, R112, "__builtin_postrisc_vavgsw(a, b)");
    Binary(out, "builtin_sll",         insn_vsllw,     VUW, VUW, VUW, R112, "__builtin_postrisc_vsllw(a, b)");
    Binary(out, "builtin_srl",         insn_vsrlw,     VUW, VUW, VUW, R112, "__builtin_postrisc_vsrlw(a, b)");
    Binary(out, "builtin_sra",         insn_vsraw,     VIW, VIW, VIW, R112, "__builtin_postrisc_vsraw(a, b)");
    Binary(out, "builtin_merge_high",  insn_vmergehw,  VUW, VUW, VUW, R112, "__builtin_postrisc_vmergehw(a, b)");
    Binary(out, "builtin_merge_low",   insn_vmergelw,  VUW, VUW, VUW, R112, "__builtin_postrisc_vmergelw(a, b)");
    Binary(out, "vec_sll",             insn_vsllw,     VUW, VUW, VUW, R112, "(a << b)");
    Binary(out, "vec_srl",             insn_vsrlw,     VUW, VUW, VUW, R112, "(a >> b)");
    Binary(out, "vec_sra",             insn_vsraw,     VIW, VIW, VIW, R112, "(a >> b)");
    Binary(out, "vec_add",             insn_vaddw,     VUW, VUW, VUW, R121, "(a + b)");
    Binary(out, "vec_sub",             insn_vsubw,     VUW, VUW, VUW, R112, "(a - b)");
    Binary(out, "vec_min",             insn_vminsw,    VIW, VIW, VIW, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxsw,    VIW, VIW, VIW, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_min",             insn_vminuw,    VUW, VUW, VUW, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxuw,    VUW, VUW, VUW, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddssatw, VIW, VIW, VIW, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubssatw, VIW, VIW, VIW, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddusatw, VUW, VUW, VUW, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubusatw, VUW, VUW, VUW, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_avgu",            insn_vavguw,    VUW, VUW, VUW, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");
    Binary(out, "vec_avgs",            insn_vavgsw,    VIW, VIW, VIW, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");


    Binary(out, "builtin_add",         insn_vaddd,     VUD, VUD, VUD, R112, "__builtin_postrisc_vaddd(a, b)");
    Binary(out, "builtin_sub",         insn_vsubd,     VUD, VUD, VUD, R112, "__builtin_postrisc_vsubd(a, b)");
    Binary(out, "builtin_addc",        insn_vaddcd,    VUD, VUD, VUD, R112, "__builtin_postrisc_vaddcd(a, b)");
    Binary(out, "builtin_subc",        insn_vsubcd,    VUD, VUD, VUD, R112, "__builtin_postrisc_vsubcd(a, b)");
    Binary(out, "builtin_addo",        insn_vaddod,    VID, VID, VID, R112, "__builtin_postrisc_vaddod(a, b)");
    Binary(out, "builtin_subo",        insn_vsubod,    VID, VID, VID, R112, "__builtin_postrisc_vsubod(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddusatd, VUD, VUD, VUD, R112, "__builtin_postrisc_vaddusatd(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubusatd, VUD, VUD, VUD, R112, "__builtin_postrisc_vsubusatd(a, b)");
    Binary(out, "builtin_add_sat",     insn_vaddssatd, VID, VID, VID, R112, "__builtin_postrisc_vaddssatd(a, b)");
    Binary(out, "builtin_sub_sat",     insn_vsubssatd, VID, VID, VID, R112, "__builtin_postrisc_vsubssatd(a, b)");
    Binary(out, "builtin_min",         insn_vminsd,    VID, VID, VID, R112, "__builtin_postrisc_vminsd(a, b)");
    Binary(out, "builtin_max",         insn_vmaxsd,    VID, VID, VID, R112, "__builtin_postrisc_vmaxsd(a, b)");
    Binary(out, "builtin_min",         insn_vminud,    VUD, VUD, VUD, R112, "__builtin_postrisc_vminud(a, b)");
    Binary(out, "builtin_max",         insn_vmaxud,    VUD, VUD, VUD, R112, "__builtin_postrisc_vmaxud(a, b)");
    Binary(out, "builtin_avg",         insn_vavgud,    VUD, VUD, VUD, R112, "__builtin_postrisc_vavgud(a, b)");
    Binary(out, "builtin_avg",         insn_vavgsd,    VID, VID, VID, R112, "__builtin_postrisc_vavgsd(a, b)");
    Binary(out, "builtin_sll",         insn_vslld,     VUD, VUD, VUD, R112, "__builtin_postrisc_vslld(a, b)");
    Binary(out, "builtin_srl",         insn_vsrld,     VUD, VUD, VUD, R112, "__builtin_postrisc_vsrld(a, b)");
    Binary(out, "builtin_sra",         insn_vsrad,     VID, VID, VID, R112, "__builtin_postrisc_vsrad(a, b)");
    Binary(out, "builtin_merge_high",  insn_vmergehd,  VUD, VUD, VUD, R112, "__builtin_postrisc_vmergehd(a, b)");
    Binary(out, "builtin_merge_low",   insn_vmergeld,  VUD, VUD, VUD, R112, "__builtin_postrisc_vmergeld(a, b)");
    Binary(out, "vec_sll",             insn_vslld,     VUD, VUD, VUD, R112, "(a << b)");
    Binary(out, "vec_srl",             insn_vsrld,     VUD, VUD, VUD, R112, "(a >> b)");
    Binary(out, "vec_sra",             insn_vsrad,     VID, VID, VID, R112, "(a >> b)");
    Binary(out, "vec_add",             insn_vaddd,     VUD, VUD, VUD, R121, "(a + b)");
    Binary(out, "vec_sub",             insn_vsubd,     VUD, VUD, VUD, R112, "(a - b)");
    Binary(out, "vec_min",             insn_vminsd,    VID, VID, VID, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxsd,    VID, VID, VID, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_min",             insn_vminud,    VUD, VUD, VUD, R112, "__builtin_elementwise_min(a, b)");
    Binary(out, "vec_max",             insn_vmaxud,    VUD, VUD, VUD, R112, "__builtin_elementwise_max(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddssatd, VID, VID, VID, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubssatd, VID, VID, VID, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_add_sat",         insn_vaddusatd, VUD, VUD, VUD, R112, "__builtin_elementwise_add_sat(a, b)");
    Binary(out, "vec_sub_sat",         insn_vsubusatd, VUD, VUD, VUD, R112, "__builtin_elementwise_sub_sat(a, b)");
    Binary(out, "vec_avgu",            insn_vavgud,    VUD, VUD, VUD, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");
    Binary(out, "vec_avgs",            insn_vavgsd,    VID, VID, VID, R121, "((a | b) & 1) + (a >> 1) + (b >> 1)");

    // splat variable
    Binary(out, "vec_splat",  insn_vsplatb, VUB, VUB, UW,  R112,  "__builtin_postrisc_vsplatb(a, b)");
    Binary(out, "vec_splat",  insn_vsplath, VUH, VUH, UW,  R112,  "__builtin_postrisc_vsplath(a, b)");
    Binary(out, "vec_splat",  insn_vsplatw, VUW, VUW, UW,  R112,  "__builtin_postrisc_vsplatw(a, b)");
    Binary(out, "vec_splat",  insn_vsplatd, VUD, VUD, UW,  R112,  "__builtin_postrisc_vsplatd(a, b)");

    // rotate left variable
    Binary(out, "vec_rol",  insn_vslpb, VUB, VUB, VUB,  R1112,  "(a << b) | (a >> ( 8 - b))");
    Binary(out, "vec_rol",  insn_vslph, VUH, VUH, VUH,  R1112,  "(a << b) | (a >> (16 - b))");
    Binary(out, "vec_rol",  insn_vslpw, VUW, VUW, VUW,  R1112,  "(a << b) | (a >> (32 - b))");
    Binary(out, "vec_rol",  insn_vslpd, VUD, VUD, VUD,  R1112,  "(a << b) | (a >> (64 - b))");

    // rotate right variable
    Binary(out, "vec_ror",  insn_vsrpb, VUB, VUB, VUB,  R1112,  "(a >> b) | (a << ( 8 - b))");
    Binary(out, "vec_ror",  insn_vsrph, VUH, VUH, VUH,  R1112,  "(a >> b) | (a << (16 - b))");
    Binary(out, "vec_ror",  insn_vsrpw, VUW, VUW, VUW,  R1112,  "(a >> b) | (a << (32 - b))");
    Binary(out, "vec_ror",  insn_vsrpd, VUD, VUD, VUD,  R1112,  "(a >> b) | (a << (64 - b))");

    Binary(out, "pack_sat",    insn_vpkssath,  VIB, VIH, VIH, R112,     "__builtin_postrisc_vpkssath(a, b)");
    Binary(out, "pack_sat",    insn_vpkssatw,  VIH, VIW, VIW, R112,     "__builtin_postrisc_vpkssatw(a, b)");
    Binary(out, "pack_sat",    insn_vpkssatd,  VIW, VID, VID, R112,     "__builtin_postrisc_vpkssatd(a, b)");

    Binary(out, "pack_satu",   insn_vpkusath,  VIB, VIH, VIH, R112,     "__builtin_postrisc_vpkusath(a, b)");
    Binary(out, "pack_satu",   insn_vpkusatw,  VIH, VIW, VIW, R112,     "__builtin_postrisc_vpkusatw(a, b)");
    Binary(out, "pack_satu",   insn_vpkusatd,  VIW, VID, VID, R112,     "__builtin_postrisc_vpkusatd(a, b)");

    Binary(out, "pack_usat",   insn_vpkusatsh, VIB, VIH, VIH, R112,     "__builtin_postrisc_vpkusatsh(a, b)");
    Binary(out, "pack_usat",   insn_vpkusatsw, VIH, VIW, VIW, R112,     "__builtin_postrisc_vpkusatsw(a, b)");
    Binary(out, "pack_usat",   insn_vpkusatsd, VIW, VID, VID, R112,     "__builtin_postrisc_vpkusatsd(a, b)");

    Binary(out, "pack_mod",    insn_vpkumodh,  VIB, VIH, VIH, R112,     "__builtin_postrisc_vpkumodh(a, b)");
    Binary(out, "pack_mod",    insn_vpkumodw,  VIH, VIW, VIW, R112,     "__builtin_postrisc_vpkumodw(a, b)");
    Binary(out, "pack_mod",    insn_vpkumodd,  VIW, VID, VID, R112,     "__builtin_postrisc_vpkumodd(a, b)");

    Binary(out, "vec_cmp_eq",  insn_vcmpeqb,   VIB, VIB, VIB, R112, "(a == b)");
    Binary(out, "vec_cmp_ne",  insn_vcmpneb,   VIB, VIB, VIB, R112, "(a != b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltsb,  VIB, VIB, VIB, R112, "(a < b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltub,  VIB, VUB, VUB, R112, "(a < b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltsb,  VIB, VIB, VIB, R121, "(a > b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltub,  VIB, VUB, VUB, R121, "(a > b)");
    Binary(out, "vec_cmp_le",  insn_vcmplesb,  VIB, VIB, VIB, R112, "(a <= b)");
    Binary(out, "vec_cmp_le",  insn_vcmpleub,  VIB, VUB, VUB, R112, "(a <= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmplesb,  VIB, VIB, VIB, R121, "(a >= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmpleub,  VIB, VUB, VUB, R121, "(a >= b)");

    Binary(out, "vec_cmp_eq",  insn_vcmpeqh,   VIH, VIH, VIH, R112, "(a == b)");
    Binary(out, "vec_cmp_ne",  insn_vcmpneh,   VIH, VIH, VIH, R112, "(a != b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltsh,  VIH, VIH, VIH, R112, "(a < b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltuh,  VIH, VUH, VUH, R112, "(a < b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltsh,  VIH, VIH, VIH, R121, "(a > b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltuh,  VIH, VUH, VUH, R121, "(a > b)");
    Binary(out, "vec_cmp_le",  insn_vcmplesh,  VIH, VIH, VIH, R112, "(a <= b)");
    Binary(out, "vec_cmp_le",  insn_vcmpleuh,  VIH, VUH, VUH, R112, "(a <= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmplesh,  VIH, VIH, VIH, R121, "(a >= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmpleuh,  VIH, VUH, VUH, R121, "(a >= b)");

    Binary(out, "vec_cmp_eq",  insn_vcmpeqw,   VIW, VIW, VIW, R112, "(a == b)");
    Binary(out, "vec_cmp_ne",  insn_vcmpnew,   VIW, VIW, VIW, R112, "(a != b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltsw,  VIW, VIW, VIW, R112, "(a < b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltuw,  VIW, VUW, VUW, R112, "(a < b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltsw,  VIW, VIW, VIW, R121, "(a > b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltuw,  VIW, VUW, VUW, R121, "(a > b)");
    Binary(out, "vec_cmp_le",  insn_vcmplesw,  VIW, VIW, VIW, R112, "(a <= b)");
    Binary(out, "vec_cmp_le",  insn_vcmpleuw,  VIW, VUW, VUW, R112, "(a <= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmplesw,  VIW, VIW, VIW, R121, "(a >= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmpleuw,  VIW, VUW, VUW, R121, "(a >= b)");

    Binary(out, "vec_cmp_eq",  insn_vcmpeqd,   VID, VID, VID, R112, "(a == b)");
    Binary(out, "vec_cmp_ne",  insn_vcmpned,   VID, VID, VID, R112, "(a != b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltsd,  VID, VID, VID, R112, "(a < b)");
    Binary(out, "vec_cmp_lt",  insn_vcmpltud,  VID, VUD, VUD, R112, "(a < b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltsd,  VID, VID, VID, R121, "(a > b)");
    Binary(out, "vec_cmp_gt",  insn_vcmpltud,  VID, VUD, VUD, R121, "(a > b)");
    Binary(out, "vec_cmp_le",  insn_vcmplesd,  VID, VID, VID, R112, "(a <= b)");
    Binary(out, "vec_cmp_le",  insn_vcmpleud,  VID, VUD, VUD, R112, "(a <= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmplesd,  VID, VID, VID, R121, "(a >= b)");
    Binary(out, "vec_cmp_ge",  insn_vcmpleud,  VID, VUD, VUD, R121, "(a >= b)");
}

CODEGEN_HANDLER(BuiltinTernary)
{
    static const char R4[] = " %r1, %r1, %r2, %r3";

    Ternary(out, "ptc",       insn_ptc,    VOID, PVOID, PVOID, UW, " %r1, %r2, %r3",       "__builtin_postrisc_tlb_purge(a, b, c)");

    Ternary(out, "postrisc_slp",   insn_vslpb,  VUB, VUB, VUB, VUB, R4, "__builtin_postrisc_vslpb(a, b, c)");
    Ternary(out, "postrisc_slp",   insn_vslph,  VUH, VUH, VUH, VUH, R4, "__builtin_postrisc_vslph(a, b, c)");
    Ternary(out, "postrisc_slp",   insn_vslpw,  VUW, VUW, VUW, VUW, R4, "__builtin_postrisc_vslpw(a, b, c)");
    Ternary(out, "postrisc_slp",   insn_vslpd,  VUD, VUD, VUD, VUD, R4, "__builtin_postrisc_vslpd(a, b, c)");

    Ternary(out, "builtin_fshl",   insn_vslpb,  VUB, VUB, VUB, VUB, R4, "__builtin_elementwise_fshl(a, b, c)");
    Ternary(out, "builtin_fshl",   insn_vslph,  VUH, VUH, VUH, VUH, R4, "__builtin_elementwise_fshl(a, b, c)");
    Ternary(out, "builtin_fshl",   insn_vslpw,  VUW, VUW, VUW, VUW, R4, "__builtin_elementwise_fshl(a, b, c)");
    Ternary(out, "builtin_fshl",   insn_vslpd,  VUD, VUD, VUD, VUD, R4, "__builtin_elementwise_fshl(a, b, c)");

    Ternary(out, "postrisc_srp",   insn_vsrpb,  VUB, VUB, VUB, VUB, R4, "__builtin_postrisc_vsrpb(a, b, c)");
    Ternary(out, "postrisc_srp",   insn_vsrph,  VUH, VUH, VUH, VUH, R4, "__builtin_postrisc_vsrph(a, b, c)");
    Ternary(out, "postrisc_srp",   insn_vsrpw,  VUW, VUW, VUW, VUW, R4, "__builtin_postrisc_vsrpw(a, b, c)");
    Ternary(out, "postrisc_srp",   insn_vsrpd,  VUD, VUD, VUD, VUD, R4, "__builtin_postrisc_vsrpd(a, b, c)");

    Ternary(out, "builtin_fshr",   insn_vsrpb,  VUB, VUB, VUB, VUB, R4, "__builtin_elementwise_fshr(a, b, c)");
    Ternary(out, "builtin_fshr",   insn_vsrph,  VUH, VUH, VUH, VUH, R4, "__builtin_elementwise_fshr(a, b, c)");
    Ternary(out, "builtin_fshr",   insn_vsrpw,  VUW, VUW, VUW, VUW, R4, "__builtin_elementwise_fshr(a, b, c)");
    Ternary(out, "builtin_fshr",   insn_vsrpd,  VUD, VUD, VUD, VUD, R4, "__builtin_elementwise_fshr(a, b, c)");
}

} // namespace llvm
} // namespace postrisc
