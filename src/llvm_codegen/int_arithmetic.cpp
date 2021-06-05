#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

#define R3      "%r1, %r1, %r2"
#define R3_SWAP "%r1, %r2, %r1"
#define R4      "%r1, %r1, %r2, %r3"
#define R4_LSB  "%r1, %r1, %r3, %r2"

namespace postrisc {
namespace llvm {

static const unary_info int_unary_functions[] = {
    { "not", insn_not,  IW, IW,  " %r1, %r1",  "(~x)" },
    { "not", insn_not,  ID, ID,  " %r1, %r1",  "(~x)" },
    { "not", insn_not,  IQ, IQ,  " %r1, %r1",  "(~x)" },

    { "abs",  insn_absw, IW, IW,  " %r1, %r1",  "(x < 0) ? -x : x" },
    { "abs",  insn_absd, ID, ID,  " %r1, %r1",  "(x < 0) ? -x : x" },
    { "abs",  insn_absq, IQ, IQ,  " %r1, %r1",  "(x < 0) ? -x : x" },

    { "builtin_abs",  insn_absw, IW, IW, " %r1, %r1", "__builtin_abs(x)" },
    { "builtin_abs1", insn_absd, ID, ID, " %r1, %r1", "__builtin_labs(x)" },
    { "builtin_abs2", insn_absd, ID, ID, " %r1, %r1", "__builtin_llabs(x)" },

    { "neg", insn_negw,  IW, IW,   " %r1, %r1",  "(-x)" },
    { "neg", insn_negd,  ID, ID,   " %r1, %r1",  "(-x)" },
    { "neg", insn_negq,  IQ, IQ,  " %r1, %r1",  "(-x)" },

    { "sra_imm",       insn_sraiw, IW, IW,  " %r1, %r1, 20",        "(x >> 20)" },
    { "srl_imm",       insn_srliw, UW, UW,  " %r1, %r1, 20",        "(x >> 20)" },
    { "sll_imm",       insn_slliw, UW, UW,  " %r1, %r1, 20",        "(x << 20)" },
    { "rotate_imm",    insn_srpiw, UW, UW,  " %r1, %r1, %r1, 20",   "(x >> 20) | (x << (32 - 20))" },

    { "sra_imm",       insn_sraid, ID, ID,  " %r1, %r1, 20",        "(x >> 20)" },
    { "srl_imm",       insn_srlid, UD, UD,  " %r1, %r1, 20",        "(x >> 20)" },
    { "sll_imm",       insn_sllid, UD, UD,  " %r1, %r1, 20",        "(x << 20)" },
    { "rotate_imm",    insn_srpid, UD, UD,  " %r1, %r1, %r1, 20",   "(x >> 20) | (x << (64 - 20))" },

    { "sra_imm",       insn_sraiq, IQ, IQ,  " %r1, %r1, 20",        "(x >> 20)" },
    { "srl_imm",       insn_srliq, UQ, UQ,  " %r1, %r1, 20",        "(x >> 20)" },
    { "sll_imm",       insn_slliq, UQ, UQ,  " %r1, %r1, 20",        "(x << 20)" },
    { "rotate_imm",    insn_srpiq, UQ, UQ,  " %r1, %r1, %r1, 20",   "(x >> 20) | (x << (128 - 20))" },


    { "sext_cast8",    insn_sextb,  IW, IW, " %r1, %r1", "(i8)x" },
    { "sext_cast8",    insn_sextb,  ID, ID, " %r1, %r1", "(i8)x" },
    { "sext_cast8",    insn_sextb,  IQ, IQ, " %r1, %r1", "(i8)x" },
    { "sext_cast16",   insn_sexth,  IW, IW, " %r1, %r1", "(i16)x" },
    { "sext_cast16",   insn_sexth,  ID, ID, " %r1, %r1", "(i16)x" },
    { "sext_cast16",   insn_sexth,  IQ, IQ, " %r1, %r1", "(i16)x" },

    { "zext_cast8",    insn_zextb,  UW, UW, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UW, UH, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UH, UW, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UD, UD, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UD, UH, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UH, UD, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UQ, UQ, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UQ, UH, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,  UH, UQ, " %r1, %r1", "(u8)x" },
    { "zext_cast16",   insn_zexth,  UW, UW, " %r1, %r1", "(u16)x" },
    { "zext_cast16",   insn_zexth,  UD, UD, " %r1, %r1", "(u16)x" },
    { "zext_cast16",   insn_zexth,  UQ, UQ, " %r1, %r1", "(u16)x" },

    { "zext_mask8",    insn_zextb,  UW, UW,   " %r1, %r1",  "x & 0xff" },
    { "zext_mask8",    insn_zextb,  UD, UD,   " %r1, %r1",  "x & 0xff" },
    { "zext_mask8",    insn_zextb,  UQ, UQ,  " %r1, %r1",  "x & 0xff" },
    { "zext_mask16",   insn_zexth,  UW, UW,   " %r1, %r1",  "x & 0xffff" },
    { "zext_mask16",   insn_zexth,  UD, UD,   " %r1, %r1",  "x & 0xffff" },
    { "zext_mask16",   insn_zexth,  UQ, UQ,  " %r1, %r1",  "x & 0xffff" },

    { "bit_clear_imm",   insn_bitclri, IW, IW, " %r1, %r1, 20",  "x & ~(1024*1024)" },
    { "bit_clear_imm",   insn_bitclri, ID, ID, " %r1, %r1, 20",  "x & ~(1024L*1024)" },

    { "mul_imm",         insn_muliws,   IW, IW, " %r1, %r1, 98765",       "x * 98765" },
    { "mul_imm_ext",     insn_muliws,   IW, IW, ".l %r1, %r1, 98765432",  "x * 98765432" },
    { "mul_imm",         insn_muliws,   UW, UW, " %r1, %r1, 98765",       "x * 98765U" }, //FIXME(?): mul_imm_iXX
    { "mul_imm_ext",     insn_muliws,   UW, UW, ".l %r1, %r1, 98765432",  "x * 98765432U" }, //FIXME(?): mul_imm_iXX

    { "mul_imm",         insn_mulid,   ID, ID, " %r1, %r1, 98765",       "x * 98765L" },
    { "mul_imm_ext",     insn_mulid,   ID, ID, ".l %r1, %r1, 98765432",  "x * 98765432L" },
    { "mul_imm",         insn_mulid,   UD, UD, " %r1, %r1, 98765",       "x * 98765UL" }, //FIXME(?): mul_imm_iXX
    { "mul_imm_ext",     insn_mulid,   UD, UD, ".l %r1, %r1, 98765432",  "x * 98765432UL" }, //FIXME(?): mul_imm_iXX

    { "shift_right_and", insn_slsrlid, IW, IW, " %r1, %r1, 41, 57",      "(x >> 16) & 127" },
    { "shift_right_and", insn_slsrlid, UW, UW, " %r1, %r1, 41, 57",      "(x >> 16) & 127" },
    { "shift_right_and", insn_slsrlid, ID, ID, " %r1, %r1, 41, 57",      "(x >> 16) & 127" },
    { "shift_right_and", insn_slsrlid, UD, UD, " %r1, %r1, 41, 57",      "(x >> 16) & 127" },

    { "addiwz",    insn_addiwz,    UD,  UW,  " %r1, %r1, 100", "x+100" },
    { "subriwz",   insn_subriwz,   UD,  UW,  " %r1, %r1, 100", "100-x" },
    { "muliwz",    insn_muliwz,    UD,  UW,  " %r1, %r1, 100", "x*100" },

    { "bool_not",      insn_xori,     BOOL,  BOOL,  " %r1, %r1, 1", "!x" },

    { "sext",          insn_sextw,    ID, IW,  " %r1, %r1", "x" },
    { "sext_cast32",   insn_sextw,    ID, ID,  " %r1, %r1", "(i32)x" },
    { "sext_cast64",   insn_sextd,    IQ, IQ, " %r1, %r1", "(i64)x" },

    { "zext",          insn_zextw,    UD, UW, " %r1, %r1", "x" },
    { "zext",          insn_zextw,    UQ, UW, " %r1, %r1", "x" },
    { "zext",          insn_zextd,    UQ, UD, " %r1, %r1", "x" },

    { "zext_cast16",   insn_zexth,    UD, UW, " %r1, %r1", "(u16)x" },
    { "zext_cast16",   insn_zexth,    UQ, UW, " %r1, %r1", "(u16)x" },
    { "zext_cast16",   insn_zexth,    UQ, UD, " %r1, %r1", "(u16)x" },

    { "zext_cast8",    insn_zextb,    UH, UH, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,    UQ, UW, " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zextb,    UQ, UD, " %r1, %r1", "(u8)x" },

    { "zext_cast32",   insn_zextw,   UQ, UH, " %r1, %r1", "(u32)x" },
    { "zext_cast32",   insn_zextw,   UQ, UW, " %r1, %r1", "(u32)x" },
    { "zext_cast32",   insn_zextw,   UQ, UD, " %r1, %r1", "(u32)x" },

    { "zext_cast64",   insn_zextd,   UQ, UD, " %r1, %r1", "(u64)x" },

    { "zext_mask32",   insn_zextw,   UD, UD,  " %r1, %r1", "x & 0xffffffff" },
    { "zext_mask32",   insn_zextw,   UQ, UQ, " %r1, %r1", "x & 0xffffffff" },

    { "zext_mask64",   insn_zextd,   UQ, UQ,  " %r1, %r1", "x & 0xffffffffffffffff" },

    // { "bit_clear_imm", insn_bit_clear_imm,      IQ, IQ, "%r1, %r1, 40",        "x & ~(((u128)1024)*1024*1024*1024)" },

    { "bit_flip_imm",  insn_bitflipi,     IW, IW,  " %r1, %r1, 20",      "x ^ (1024*1024)" },
    { "bit_flip_imm",  insn_bitflipi,     ID, ID,  " %r1, %r1, 30",      "x ^ (1024LL*1024*1024)" },
    { "bit_flip_imm",  insn_bitflipi,     IQ, IQ, " %r1, %r1, 40",      "x ^ (i128)(1024LL*1024*1024*1024)" },

    { "bit_set_imm",  insn_bitseti,       IW, IW,  " %r1, %r1, 20",      "x | (1024*1024)" },
    { "bit_set_imm",  insn_bitseti,       ID, ID,  " %r1, %r1, 30",      "x | (1024LL*1024*1024)" },
    { "bit_set_imm",  insn_bitseti,       IQ, IQ, " %r1, %r1, 40",      "x | (1024LL*1024*1024*1024)" },

    // ALIGNED_UP(N, NBITS) = ((N<<scale) + (2^NBITS-1)) & (~(2^NBITS-1))
    { "alignup1",    insn_alignup,       UD, UD,  " %r1, %r1, 0, 20",    "(x      + ((1LL<<20)-1)) & (~((1LL<<20)-1))" },
    { "alignup2",    insn_alignup,       UD, UD,  " %r1, %r1, 4, 20",    "((x<<4) + ((1LL<<20)-1)) & (~((1LL<<20)-1))" },

    { "ldan",        insn_ldan,          UD, UD,  " %r1, %r1, -1234",    "x + (-1234LL << 20)" },

    { "slsraid",     insn_slsraid,       ID, ID,  " %r1, %r1, 10, 20",   "(x << 10) >> 20" },
};

CODEGEN_HANDLER(IntUnary)
{
    for (const auto& func : int_unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << func.args << '\n';
        if (func.args[0] == '.') {
            // we have double-slot main instruction, next will be in the next bundle
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

static const binary_info int_binary_functions[] = {
    { "add",         insn_addws,   IW, IW, IW, R3_SWAP,  "a + b" },
    { "sub",         insn_subws,   IW, IW, IW, R3,       "a - b" },
    { "div",         insn_divsw,   IW, IW, IW, R3,       "a / b" },
    { "rem",         insn_remsw,   IW, IW, IW, R3,       "a % b" },
    { "udiv",        insn_divuw,   UW, UW, UW, R3,       "a / b" },
    { "urem",        insn_remuw,   UW, UW, UW, R3,       "a % b" },
    { "and",         insn_and,     UW, UW, UW, R3_SWAP, "a & b" },
    { "or",          insn_or,      UW, UW, UW, R3_SWAP, "a | b" },
    { "xor",         insn_xor,     UW, UW, UW, R3_SWAP, "a ^ b" },
    { "xnor",        insn_xnor,    UW, UW, UW, R3,      "~(a ^ b)" },
    { "nand",        insn_nand,    UW, UW, UW, R3_SWAP, "~(a & b)" },
    { "nor",         insn_nor,     UW, UW, UW, R3_SWAP, "~(a | b)" },
    { "andn",        insn_andn,    UW, UW, UW, R3,      "(~a) & b" },
    { "orn",         insn_orn,     UW, UW, UW, R3,      "(~a) | b" },
    { "cmpeq",       insn_cmpeqw,  BOOL, IW, IW, R3,       "a == b" },
    { "cmpne",       insn_cmpnew,  BOOL, IW, IW, R3,       "a != b" },
    { "cmplt",       insn_cmpltsw, BOOL, IW, IW, R3,       "a < b"  },
    { "cmple",       insn_cmpgesw, BOOL, IW, IW, R3_SWAP,  "a <= b" },
    { "cmpgt",       insn_cmpltsw, BOOL, IW, IW, R3_SWAP,  "a > b"  },
    { "cmpge",       insn_cmpgesw, BOOL, IW, IW, R3,       "a >= b" },
    { "cmplt",       insn_cmpltuw, BOOL, UW, UW, R3,       "a < b"  },
    { "cmple",       insn_cmpgeuw, BOOL, UW, UW, R3_SWAP,  "a <= b" },
    { "cmpgt",       insn_cmpltuw, BOOL, UW, UW, R3_SWAP,  "a > b"  },
    { "cmpge",       insn_cmpgeuw, BOOL, UW, UW, R3,       "a >= b" },
    { "lt_abs_diff", insn_abdw,   IW, IW, IW, R3_SWAP,  "(a < b) ? (b-a) : (a-b)" },
    { "test_fshr1",  insn_srpiw,  UW, UW, UW, "%r1, %r2, %r1, 20",  "(a >> 20) | (b << (32 - 20))" },
    { "test_fshr2",  insn_srpiw,  UW, UW, UW, "%r1, %r2, %r1, 11",  "(a >> 11) | (b << (32 - 11))" },
    { "sra",         insn_sraw,   IW, IW, IW,  "%r1, %r1, %r2",   "(a >> b)" },
    { "srl",         insn_srlw,   UW, UW, IW,  "%r1, %r1, %r2",   "(a >> b)" },
    { "sll",         insn_sllw,   UW, UW, IW,  "%r1, %r1, %r2",   "(a << b)" },
    { "min_lt_abab", insn_minsw,  IW, IW, IW, R3,      "a <  b ? a : b" },
    { "max_ge_abab", insn_maxsw,  IW, IW, IW, R3,      "a >= b ? a : b" },
    { "min_le_abab", insn_minsw,  IW, IW, IW, R3,      "a <= b ? a : b" },
    { "max_gt_abab", insn_maxsw,  IW, IW, IW, R3,      "a >  b ? a : b" },
    { "max_lt_baab", insn_maxsw,  IW, IW, IW, R3_SWAP, "b <  a ? a : b" },
    { "min_ge_baab", insn_minsw,  IW, IW, IW, R3_SWAP, "b >= a ? a : b" },
    { "max_le_baab", insn_maxsw,  IW, IW, IW, R3_SWAP, "b <= a ? a : b" },
    { "min_gt_baab", insn_minsw,  IW, IW, IW, R3_SWAP, "b >  a ? a : b" },
    { "max_lt_abba", insn_maxsw,  IW, IW, IW, R3,      "a <  b ? b : a" },
    { "min_ge_abba", insn_minsw,  IW, IW, IW, R3,      "a >= b ? b : a" },
    { "max_le_abba", insn_maxsw,  IW, IW, IW, R3,      "a <= b ? b : a" },
    { "min_gt_abba", insn_minsw,  IW, IW, IW, R3,      "a >  b ? b : a" },
    { "min_lt_baba", insn_minsw,  IW, IW, IW, R3_SWAP, "b <  a ? b : a" },
    { "max_ge_baba", insn_maxsw,  IW, IW, IW, R3_SWAP, "b >= a ? b : a" },
    { "min_le_baba", insn_minsw,  IW, IW, IW, R3_SWAP, "b <= a ? b : a" },
    { "max_gt_baba", insn_maxsw,  IW, IW, IW, R3_SWAP, "b >  a ? b : a" },
    { "min_lt_abab", insn_minuw,  UW, UW, UW, R3,      "a <  b ? a : b" },
    { "max_ge_abab", insn_maxuw,  UW, UW, UW, R3,      "a >= b ? a : b" },
    { "min_le_abab", insn_minuw,  UW, UW, UW, R3,      "a <= b ? a : b" },
    { "max_gt_abab", insn_maxuw,  UW, UW, UW, R3,      "a >  b ? a : b" },
    { "max_lt_baab", insn_maxuw,  UW, UW, UW, R3_SWAP, "b <  a ? a : b" },
    { "min_ge_baab", insn_minuw,  UW, UW, UW, R3_SWAP, "b >= a ? a : b" },
    { "max_le_baab", insn_maxuw,  UW, UW, UW, R3_SWAP, "b <= a ? a : b" },
    { "min_gt_baab", insn_minuw,  UW, UW, UW, R3_SWAP, "b >  a ? a : b" },
    { "max_lt_abba", insn_maxuw,  UW, UW, UW, R3,      "a <  b ? b : a" },
    { "min_ge_abba", insn_minuw,  UW, UW, UW, R3,      "a >= b ? b : a" },
    { "max_le_abba", insn_maxuw,  UW, UW, UW, R3,      "a <= b ? b : a" },
    { "min_gt_abba", insn_minuw,  UW, UW, UW, R3,      "a >  b ? b : a" },
    { "min_lt_baba", insn_minuw,  UW, UW, UW, R3_SWAP, "b <  a ? b : a" },
    { "max_ge_baba", insn_maxuw,  UW, UW, UW, R3_SWAP, "b >= a ? b : a" },
    { "min_le_baba", insn_minuw,  UW, UW, UW, R3_SWAP, "b <= a ? b : a" },
    { "max_gt_baba", insn_maxuw,  UW, UW, UW, R3_SWAP, "b >  a ? b : a" },


    { "add",        insn_addd,   ID, ID, ID, R3_SWAP,  "a + b" },
    { "sub",        insn_subd,   ID, ID, ID, R3,       "a - b" },
    { "div",        insn_divsd,  ID, ID, ID, R3,       "a / b" },
    { "rem",        insn_remsd,  ID, ID, ID, R3,       "a % b" },
    { "udiv",       insn_divud,  UD, UD, UD, R3,       "a / b" },
    { "urem",       insn_remud,  UD, UD, UD, R3,       "a % b" },
    { "and",        insn_and,      UD, UD, UD, R3_SWAP, "a & b" },
    { "or",         insn_or,       UD, UD, UD, R3_SWAP, "a | b" },
    { "xor",        insn_xor,      UD, UD, UD, R3_SWAP, "a ^ b" },
    { "xnor",       insn_xnor,     UD, UD, UD, R3,      "~(a ^ b)" },
    { "nand",       insn_nand,     UD, UD, UD, R3_SWAP, "~(a & b)" },
    { "nor",        insn_nor,      UD, UD, UD, R3_SWAP, "~(a | b)" },
    { "andn",       insn_andn,     UD, UD, UD, R3,      "(~a) & b" },
    { "orn",        insn_orn,      UD, UD, UD, R3,      "(~a) | b" },
    { "cmpeq",      insn_cmpeqd,    BOOL, ID, ID, R3,       "a == b" },
    { "cmpne",      insn_cmpned,    BOOL, ID, ID, R3,       "a != b" },
    { "cmplt",      insn_cmpltsd,   BOOL, ID, ID, R3,       "a < b"  },
    { "cmple",      insn_cmpgesd,   BOOL, ID, ID, R3_SWAP,  "a <= b" },
    { "cmpgt",      insn_cmpltsd,   BOOL, ID, ID, R3_SWAP,  "a > b"  },
    { "cmpge",      insn_cmpgesd,   BOOL, ID, ID, R3,       "a >= b" },
    { "cmplt",      insn_cmpltud,   BOOL, UD, UD, R3,       "a < b"  },
    { "cmple",      insn_cmpgeud,   BOOL, UD, UD, R3_SWAP,  "a <= b" },
    { "cmpgt",      insn_cmpltud,   BOOL, UD, UD, R3_SWAP,  "a > b"  },
    { "cmpge",      insn_cmpgeud,   BOOL, UD, UD, R3,       "a >= b" },

    { "lt_abs_diff", insn_abdd, ID, ID, ID, R3_SWAP,  "(a < b) ? (b-a) : (a-b)" },
    { "test_fshr1",  insn_srpid,  UD, UD, UD, "%r1, %r2, %r1, 20",  "(a >> 20) | (b << (64 - 20))" },
    { "test_fshr2",  insn_srpid,  UD, UD, UD, "%r1, %r2, %r1, 11",  "(a >> 11) | (b << (64 - 11))" },

    { "sra",         insn_srad,   ID, ID, ID,  "%r1, %r1, %r2",   "(a >> b)" },
    { "srl",         insn_srld,   UD, UD, ID,  "%r1, %r1, %r2",   "(a >> b)" },
    { "sll",         insn_slld,   UD, UD, ID,  "%r1, %r1, %r2",   "(a << b)" },
    { "min_lt_abab", insn_minsd,  ID, ID, ID, R3,      "a <  b ? a : b" },
    { "max_ge_abab", insn_maxsd,  ID, ID, ID, R3,      "a >= b ? a : b" },
    { "min_le_abab", insn_minsd,  ID, ID, ID, R3,      "a <= b ? a : b" },
    { "max_gt_abab", insn_maxsd,  ID, ID, ID, R3,      "a >  b ? a : b" },
    { "max_lt_baab", insn_maxsd,  ID, ID, ID, R3_SWAP, "b <  a ? a : b" },
    { "min_ge_baab", insn_minsd,  ID, ID, ID, R3_SWAP, "b >= a ? a : b" },
    { "max_le_baab", insn_maxsd,  ID, ID, ID, R3_SWAP, "b <= a ? a : b" },
    { "min_gt_baab", insn_minsd,  ID, ID, ID, R3_SWAP, "b >  a ? a : b" },
    { "max_lt_abba", insn_maxsd,  ID, ID, ID, R3,      "a <  b ? b : a" },
    { "min_ge_abba", insn_minsd,  ID, ID, ID, R3,      "a >= b ? b : a" },
    { "max_le_abba", insn_maxsd,  ID, ID, ID, R3,      "a <= b ? b : a" },
    { "min_gt_abba", insn_minsd,  ID, ID, ID, R3,      "a >  b ? b : a" },
    { "min_lt_baba", insn_minsd,  ID, ID, ID, R3_SWAP, "b <  a ? b : a" },
    { "max_ge_baba", insn_maxsd,  ID, ID, ID, R3_SWAP, "b >= a ? b : a" },
    { "min_le_baba", insn_minsd,  ID, ID, ID, R3_SWAP, "b <= a ? b : a" },
    { "max_gt_baba", insn_maxsd,  ID, ID, ID, R3_SWAP, "b >  a ? b : a" },
    { "min_lt_abab", insn_minud,  UD, UD, UD, R3,      "a <  b ? a : b" },
    { "max_ge_abab", insn_maxud,  UD, UD, UD, R3,      "a >= b ? a : b" },
    { "min_le_abab", insn_minud,  UD, UD, UD, R3,      "a <= b ? a : b" },
    { "max_gt_abab", insn_maxud,  UD, UD, UD, R3,      "a >  b ? a : b" },
    { "max_lt_baab", insn_maxud,  UD, UD, UD, R3_SWAP, "b <  a ? a : b" },
    { "min_ge_baab", insn_minud,  UD, UD, UD, R3_SWAP, "b >= a ? a : b" },
    { "max_le_baab", insn_maxud,  UD, UD, UD, R3_SWAP, "b <= a ? a : b" },
    { "min_gt_baab", insn_minud,  UD, UD, UD, R3_SWAP, "b >  a ? a : b" },
    { "max_lt_abba", insn_maxud,  UD, UD, UD, R3,      "a <  b ? b : a" },
    { "min_ge_abba", insn_minud,  UD, UD, UD, R3,      "a >= b ? b : a" },
    { "max_le_abba", insn_maxud,  UD, UD, UD, R3,      "a <= b ? b : a" },
    { "min_gt_abba", insn_minud,  UD, UD, UD, R3,      "a >  b ? b : a" },
    { "min_lt_baba", insn_minud,  UD, UD, UD, R3_SWAP, "b <  a ? b : a" },
    { "max_ge_baba", insn_maxud,  UD, UD, UD, R3_SWAP, "b >= a ? b : a" },
    { "min_le_baba", insn_minud,  UD, UD, UD, R3_SWAP, "b <= a ? b : a" },
    { "max_gt_baba", insn_maxud,  UD, UD, UD, R3_SWAP, "b >  a ? b : a" },

    { "add",        insn_addq,      IQ, IQ, IQ, R3_SWAP,  "a + b" },
    { "sub",        insn_subq,      IQ, IQ, IQ, R3,       "a - b" },
    { "div",        insn_divsq,     IQ, IQ, IQ, R3,       "a / b" },
    { "rem",        insn_remsq,     IQ, IQ, IQ, R3,       "a % b" },
    { "udiv",       insn_divuq,     UQ, UQ, UQ, R3,       "a / b" },
    { "urem",       insn_remuq,     UQ, UQ, UQ, R3,       "a % b" },
    { "and",        insn_and,       UQ, UQ, UQ, R3_SWAP, "a & b" },
    { "or",         insn_or,        UQ, UQ, UQ, R3_SWAP, "a | b" },
    { "xor",        insn_xor,       UQ, UQ, UQ, R3_SWAP, "a ^ b" },
    { "xnor",       insn_xnor,      UQ, UQ, UQ, R3,      "~(a ^ b)" },
    { "nand",       insn_nand,      UQ, UQ, UQ, R3_SWAP, "~(a & b)" },
    { "nor",        insn_nor,       UQ, UQ, UQ, R3_SWAP, "~(a | b)" },
    { "andn",       insn_andn,      UQ, UQ, UQ, R3,      "(~a) & b" },
    { "orn",        insn_orn,       UQ, UQ, UQ, R3,      "(~a) | b" },
    { "cmpeq",      insn_cmpeqq,  BOOL, IQ, IQ, R3,       "a == b" },
    { "cmpne",      insn_cmpneq,  BOOL, IQ, IQ, R3,       "a != b" },
    { "cmplt",      insn_cmpltsq, BOOL, IQ, IQ, R3,       "a < b"  },
    { "cmple",      insn_cmpgesq, BOOL, IQ, IQ, R3_SWAP,  "a <= b" },
    { "cmpgt",      insn_cmpltsq, BOOL, IQ, IQ, R3_SWAP,  "a > b"  },
    { "cmpge",      insn_cmpgesq, BOOL, IQ, IQ, R3,       "a >= b" },
    { "cmplt",      insn_cmpltuq, BOOL, UQ, UQ, R3,       "a < b"  },
    { "cmple",      insn_cmpgeuq, BOOL, UQ, UQ, R3_SWAP,  "a <= b" },
    { "cmpgt",      insn_cmpltuq, BOOL, UQ, UQ, R3_SWAP,  "a > b"  },
    { "cmpge",      insn_cmpgeuq, BOOL, UQ, UQ, R3,       "a >= b" },
    { "lt_abs_diff", insn_abdq,   IQ, IQ, IQ, R3_SWAP,  "(a < b) ? (b-a) : (a-b)" },
    { "test_fshr1",  insn_srpiq,  UQ, UQ, UQ, "%r1, %r2, %r1, 20",  "(a >> 20) | (b << (128 - 20))" },
    { "test_fshr2",  insn_srpiq,  UQ, UQ, UQ, "%r1, %r2, %r1, 11",  "(a >> 11) | (b << (128 - 11))" },
    { "sra",         insn_sraq,   IQ, IQ, IQ,  "%r1, %r1, %r2",   "(a >> b)" },
    { "srl",         insn_srlq,   UQ, UQ, IQ,  "%r1, %r1, %r2",   "(a >> b)" },
    { "sll",         insn_sllq,   UQ, UQ, IQ,  "%r1, %r1, %r2",   "(a << b)" },
    { "min_lt_abab", insn_minsq,  IQ, IQ, IQ, R3,      "a <  b ? a : b" },
    { "max_ge_abab", insn_maxsq,  IQ, IQ, IQ, R3,      "a >= b ? a : b" },
    { "min_le_abab", insn_minsq,  IQ, IQ, IQ, R3,      "a <= b ? a : b" },
    { "max_gt_abab", insn_maxsq,  IQ, IQ, IQ, R3,      "a >  b ? a : b" },
    { "max_lt_baab", insn_maxsq,  IQ, IQ, IQ, R3_SWAP, "b <  a ? a : b" },
    { "min_ge_baab", insn_minsq,  IQ, IQ, IQ, R3_SWAP, "b >= a ? a : b" },
    { "max_le_baab", insn_maxsq,  IQ, IQ, IQ, R3_SWAP, "b <= a ? a : b" },
    { "min_gt_baab", insn_minsq,  IQ, IQ, IQ, R3_SWAP, "b >  a ? a : b" },
    { "max_lt_abba", insn_maxsq,  IQ, IQ, IQ, R3,      "a <  b ? b : a" },
    { "min_ge_abba", insn_minsq,  IQ, IQ, IQ, R3,      "a >= b ? b : a" },
    { "max_le_abba", insn_maxsq,  IQ, IQ, IQ, R3,      "a <= b ? b : a" },
    { "min_gt_abba", insn_minsq,  IQ, IQ, IQ, R3,      "a >  b ? b : a" },
    { "min_lt_baba", insn_minsq,  IQ, IQ, IQ, R3_SWAP, "b <  a ? b : a" },
    { "max_ge_baba", insn_maxsq,  IQ, IQ, IQ, R3_SWAP, "b >= a ? b : a" },
    { "min_le_baba", insn_minsq,  IQ, IQ, IQ, R3_SWAP, "b <= a ? b : a" },
    { "max_gt_baba", insn_maxsq,  IQ, IQ, IQ, R3_SWAP, "b >  a ? b : a" },
    { "min_lt_abab", insn_minuq,  UQ, UQ, UQ, R3,      "a <  b ? a : b" },
    { "max_ge_abab", insn_maxuq,  UQ, UQ, UQ, R3,      "a >= b ? a : b" },
    { "min_le_abab", insn_minuq,  UQ, UQ, UQ, R3,      "a <= b ? a : b" },
    { "max_gt_abab", insn_maxuq,  UQ, UQ, UQ, R3,      "a >  b ? a : b" },
    { "max_lt_baab", insn_maxuq,  UQ, UQ, UQ, R3_SWAP, "b <  a ? a : b" },
    { "min_ge_baab", insn_minuq,  UQ, UQ, UQ, R3_SWAP, "b >= a ? a : b" },
    { "max_le_baab", insn_maxuq,  UQ, UQ, UQ, R3_SWAP, "b <= a ? a : b" },
    { "min_gt_baab", insn_minuq,  UQ, UQ, UQ, R3_SWAP, "b >  a ? a : b" },
    { "max_lt_abba", insn_maxuq,  UQ, UQ, UQ, R3,      "a <  b ? b : a" },
    { "min_ge_abba", insn_minuq,  UQ, UQ, UQ, R3,      "a >= b ? b : a" },
    { "max_le_abba", insn_maxuq,  UQ, UQ, UQ, R3,      "a <= b ? b : a" },
    { "min_gt_abba", insn_minuq,  UQ, UQ, UQ, R3,      "a >  b ? b : a" },
    { "min_lt_baba", insn_minuq,  UQ, UQ, UQ, R3_SWAP, "b <  a ? b : a" },
    { "max_ge_baba", insn_maxuq,  UQ, UQ, UQ, R3_SWAP, "b >= a ? b : a" },
    { "min_le_baba", insn_minuq,  UQ, UQ, UQ, R3_SWAP, "b <= a ? b : a" },
    { "max_gt_baba", insn_maxuq,  UQ, UQ, UQ, R3_SWAP, "b >  a ? b : a" },

    // zero extend to bigger type
    { "add",   insn_addwz,  UD, UW, UW, R3_SWAP,  "a + b" },
    { "sub",   insn_subwz,  UD, UW, UW, R3,       "a - b" },

    { "mul",   insn_mulws,  IW, IW, IW, R3_SWAP,  "a * b" },
    { "mul",   insn_muld,   ID, ID, ID, R3_SWAP,  "a * b" },

    // sext/zext to a bigger type
    { "mul",   insn_mulws,  ID, IW, IW, R3_SWAP, "a * b" },
    { "mul",   insn_mulwz,  UD, UW, UW, R3_SWAP, "a * b" },
    { "mul",   insn_mulws,  UD, IW, IW, R3_SWAP, "a * b" }, //FIXME: why mulws?

    { "sladdws",  insn_sladdws,   IW, IW, IW, R3 ", 3",  "a + (b << 3)" },
    { "slsubws",  insn_slsubws,   IW, IW, IW, R3 ", 3",  "a - (b << 3)" },
    { "slsubrw",  insn_slsubrws,  IW, IW, IW, R3 ", 3",  "(b << 3) - a" },
    { "sladdd",   insn_sladdd,   ID, ID, ID, R3 ", 3",  "a + (b << 3)" },
    { "slsubd",   insn_slsubd,   ID, ID, ID, R3 ", 3",  "a - (b << 3)" },
    { "slsubrd",  insn_slsubrd,  ID, ID, ID, R3 ", 3",  "(b << 3) - a" },
    { "sladdwz",  insn_sladdwz,  UD, UW, UW, R3 ", 3",  "a + (b << 3)" },
    { "slsubwz",  insn_slsubwz,  UD, UW, UW, R3 ", 3",  "a - (b << 3)" },
    { "slsubrwz", insn_slsubrwz, UD, UW, UW, R3 ", 3",  "(b << 3) - a" },

    { "slor",  insn_slor,  IW, IW, IW, R3 ", 3",  "a | (b << 3)" },
    { "slxor", insn_slxor, IW, IW, IW, R3 ", 3",  "a ^ (b << 3)" },
    { "slor",  insn_slor,  ID, ID, ID, R3 ", 3",  "a | (b << 3)" },
    { "slxor", insn_slxor, ID, ID, ID, R3 ", 3",  "a ^ (b << 3)" },

    { "abs_diff1", insn_abdw, IW, IW, IW, R3_SWAP,  "__builtin_abs(a - b)" },
    { "abs_diff2", insn_abdd, ID, ID, ID, R3_SWAP,  "__builtin_labs(a - b)" },
    { "abs_diff3", insn_abdd, ID, ID, ID, R3_SWAP,  "__builtin_llabs(a - b)" },

    { "bitclr1",  insn_bitclr,  IW, IW, IW, R3, "a & ~(1 << b)" },
    { "bitclr2",  insn_bitclr,  IW, IW, ID, R3, "a & ~(1 << b)" },
    { "bitclr3",  insn_bitclr,  ID, ID, IW, R3, "a & ~(1l << b)" },
    { "bitclr4",  insn_bitclr,  ID, ID, ID, R3, "a & ~(1l << b)" },
    { "bitclr5",  insn_bitclr,  IW, IW, IW, R3, "a & ~((i32)1 << b)" },
    { "bitclr6",  insn_bitclr,  IW, IW, ID, R3, "a & ~((i32)1 << b)" },
    { "bitclr7",  insn_bitclr,  ID, ID, IW, R3, "a & ~((i64)1 << b)" },
    { "bitclr8",  insn_bitclr,  ID, ID, ID, R3, "a & ~((i64)1 << b)" },
    // FIXME: bit_clear i128 doesn't work
    //{ "bit_clear64", insn_bit_clear,    IQ, IQ, IW, R3, "a & ~((i128)1 << b)" },
    //{ "bit_clear64", insn_bit_clear,    IQ, IQ, ID, R3, "a & ~((i128)1 << b)" },

    { "bitflip1", insn_bitflip,  IW, IW, IW, R3, "a ^ (1 << b)" },
    { "bitflip2", insn_bitflip,  IW, IW, ID, R3, "a ^ (1 << b)" },
    { "bitflip3", insn_bitflip,  ID, ID, IW, R3, "a ^ (1l << b)" },
    { "bitflip4", insn_bitflip,  ID, ID, ID, R3, "a ^ (1l << b)" },
    { "bitflip5", insn_bitflip,  ID, ID, IW, R3, "a ^ (1ll << b)" },
    { "bitflip6", insn_bitflip,  ID, ID, ID, R3, "a ^ (1ll << b)" },
    { "bitflip5", insn_bitflip,  IW, IW, IW, R3, "a ^ ((i32)1 << b)" },
    { "bitflip6", insn_bitflip,  IW, IW, ID, R3, "a ^ ((i32)1 << b)" },
    { "bitflip7", insn_bitflip,  ID, ID, IW, R3, "a ^ ((i64)1 << b)" },
    { "bitflip8", insn_bitflip,  ID, ID, ID, R3, "a ^ ((i64)1 << b)" },
    { "bitflip7", insn_bitflip,  IQ, IQ, IW, R3, "a ^ ((i128)1 << b)" },
    { "bitflip8", insn_bitflip,  IQ, IQ, ID, R3, "a ^ ((i128)1 << b)" },

    { "bitset1",  insn_bitset,  IW, IW, IW, R3, "a | (1 << b)" },
    { "bitset2",  insn_bitset,  IW, IW, ID, R3, "a | (1 << b)" },
    { "bitset3",  insn_bitset,  ID, ID, IW, R3, "a | (1l << b)" },
    { "bitset4",  insn_bitset,  ID, ID, ID, R3, "a | (1l << b)" },
    { "bitset5",  insn_bitset,  IW, IW, IW, R3, "a | ((i32)1 << b)" },
    { "bitset6",  insn_bitset,  IW, IW, ID, R3, "a | ((i32)1 << b)" },
    { "bitset7",  insn_bitset,  ID, ID, IW, R3, "a | ((i64)1 << b)" },
    { "bitset8",  insn_bitset,  ID, ID, ID, R3, "a | ((i64)1 << b)" },
    { "bitset7",  insn_bitset,  IQ, IQ, IW, R3, "a | ((i128)1 << b)" },
    { "bitset8",  insn_bitset,  IQ, IQ, ID, R3, "a | ((i128)1 << b)" },

    { "bool_and",   insn_and,     BOOL, BOOL, BOOL,  R3, "a & b" },
    { "bool_or",    insn_or,      BOOL, BOOL, BOOL,  R3, "a | b" },
    { "bool_xor",   insn_xor,     BOOL, BOOL, BOOL,  R3, "a ^ b" },

    { "addc",      insn_addcd, UQ, UD, UD, R3_SWAP,  "(u128)a + (u128)b" },
    { "subb",      insn_subcd, UQ, UD, UD, R3,       "(u128)a - (u128)b" },

    { "deposit_1", insn_deposit, UW, UW, UW, R3 ", 4, 12",  "(a & 0xFFFF000F) | ((b << 4) & 0x0000FFF0)" },
    { "deposit_2", insn_deposit, UW, UW, UW, R3 ", 8, 12",  "(a & 0xFFF000FF) | ((b << 8) & 0x000FFF00)" },
    { "deposit_1", insn_deposit, UD, UD, UD, R3 ", 4, 12",  "(a & 0xFFFFFFFFFFFF000F) | ((b << 4) & 0x0000FFF0)" },
    { "deposit_2", insn_deposit, UD, UD, UD, R3 ", 8, 12",  "(a & 0xFFFFFFFFFFF000FF) | ((b << 8) & 0x000FFF00)" },
};

CODEGEN_HANDLER(IntBinary)
{
    for (const auto& func : int_binary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type << "_" << func.src2_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

static const ternary_info int_ternary_functions[] = {
    { "cmoveq",      insn_cmoveqw,  IW, IW, IW, IW, R4,       "(a == 0) ? b : c" },
    { "cmovlt",      insn_cmovltw,  IW, IW, IW, IW, R4,       "(a <  0) ? b : c" },
    { "cmovle",      insn_cmovlew,  IW, IW, IW, IW, R4,       "(a <= 0) ? b : c" },
    { "cmovlsb1",    insn_cmovlsb,  IW, IW, IW, IW, R4_LSB,   "(a & 1) ? b : c" },
    { "cmovlsb2",    insn_cmovlsb,  IW, IW, IW, IW, R4_LSB,   "((a & 1) != 0) ? b : c" },
    { "cmovlsb3",    insn_cmovlsb,  IW, IW, IW, IW, R4_LSB,   "((a & 1) == 1) ? b : c" },
    { "slp",         insn_slpw,     IW, UW, UW, IW, R4,       "(a << c) | (b >> (32 - c))" },
    { "srp",         insn_srpw,     UW, UW, UW, IW, R4,       "(b >> c) | (a << (32 - c))" },

    { "cmoveq",      insn_cmoveqd,  ID, ID, ID, ID, R4,       "(a == 0) ? b : c" },
    { "cmovlt",      insn_cmovltd,  ID, ID, ID, ID, R4,       "(a <  0) ? b : c" },
    { "cmovle",      insn_cmovled,  ID, ID, ID, ID, R4,       "(a <= 0) ? b : c" },
    { "cmovlsb1",    insn_cmovlsb,  ID, ID, ID, ID, R4_LSB,   "(a & 1) ? b : c" },
    { "cmovlsb2",    insn_cmovlsb,  ID, ID, ID, ID, R4_LSB,   "((a & 1) != 0) ? b : c" },
    { "cmovlsb3",    insn_cmovlsb,  ID, ID, ID, ID, R4_LSB,   "((a & 1) == 1) ? b : c" },
    { "slp",         insn_slpd,     ID, UD, UD, ID, R4,       "(a << c) | (b >> (64 - c))" },
    { "srp",         insn_srpd,     UD, UD, UD, ID, R4,       "(b >> c) | (a << (64 - c))" },

    { "cmoveq",      insn_cmoveqq,  IQ, IQ, IQ, IQ, R4,       "(a == 0) ? b : c" },
    { "cmovlt",      insn_cmovltq,  IQ, IQ, IQ, IQ, R4,       "(a <  0) ? b : c" },
    { "cmovle",      insn_cmovleq,  IQ, IQ, IQ, IQ, R4,       "(a <= 0) ? b : c" },
    { "cmovlsb1",    insn_cmovlsb,  IQ, IQ, IQ, IQ, R4_LSB,   "(a & 1) ? b : c" },
    { "cmovlsb2",    insn_cmovlsb,  IQ, IQ, IQ, IQ, R4_LSB,   "((a & 1) != 0) ? b : c" },
    { "cmovlsb3",    insn_cmovlsb,  IQ, IQ, IQ, IQ, R4_LSB,   "((a & 1) == 1) ? b : c" },
    { "slp",         insn_slpq,     IQ, UQ, UQ, IQ, R4,       "(a << c) | (b >> (128 - c))" },
    { "srp",         insn_srpq,     UQ, UQ, UQ, IQ, R4,       "(b >> c) | (a << (128 - c))" },

    { "addaddd",     insn_addaddd,   UD,  UD, UD, UD,  "%r1, %r2, %r1, %r3",  "a + b + c" },
    { "addsubd",     insn_addsubd,   UD,  UD, UD, UD,  "%r1, %r2, %r1, %r3",  "a + b - c" },
    { "subsubd",     insn_subsubd,   UD,  UD, UD, UD,  "%r1, %r1, %r2, %r3",  "a - b - c" },
    { "muladdd",     insn_muladdd,   UD,  UD, UD, UD,  "%r1, %r2, %r1, %r3",  "a * b + c" },
    { "mulsubd",     insn_mulsubd,   UD,  UD, UD, UD,  "%r1, %r2, %r1, %r3",  "a * b - c" },
    { "mulsubrd",    insn_mulsubrd,  UD,  UD, UD, UD,  "%r1, %r2, %r1, %r3",  "c - a * b" },

    { "slsra",       insn_slsraw,    IW,  IW, IW, IW, R4, "(a << b) >> c" },
    { "slsrl",       insn_slsrlw,    UW,  UW, UW, IW, R4, "(a << b) >> c" },
    { "slsra",       insn_slsrad,    ID,  ID, ID, IW, R4, "(a << b) >> c" },
    { "slsrl",       insn_slsrld,    UD,  UD, UD, IW, R4, "(a << b) >> c" },

    { "addc3d",     insn_addc3d,  UQ, UD, UD, UQ, "%r1, %r2, %r1, %r3",  "(u128)a + (u128)b + (c >> 64)" },
    { "subc3d",     insn_subc3d,  UQ, UD, UD, IQ, R4,                    "(u128)a - (u128)b + (c >> 64)" },
};

CODEGEN_HANDLER(IntTernary)
{
    for (const auto& func : int_ternary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b, "
                   << func.src3_type << " c)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(IntDeposit32) { Deposit(out, 32); }
CODEGEN_HANDLER(IntDeposit64) { Deposit(out, 64); }

void Generator::Deposit(std::ostream& out, int size)
{
    const char *suffix = (size == 32 ? "" : "LL");
    const int step = size == 32 ? 1 : 1;

    for (int shift = 0; shift < size; shift+=step) {
        for (int len = 1; (shift == 0 ? len < size : len <= size - shift); len+=step) {
            // FIXME: deposit(gz) + sl_xor
            if (shift + len == size) continue;

            // FIXME: and_imm + bit_clear + or
            if (shift == 0 && len == 31 && size == 32) continue;

            const u64 mask = util::makemask<u64>(len);
            const u64 mask_shifted = mask << shift;
            const u64 mask_inverted = mask_shifted ^ util::makemask<u64>(size);

            std::ostringstream fname;
            fname << "deposit_" << size << "_" << shift << "_" << len;
            out << CHECK_LABEL << fname.str() << '\n';
            out << CHECK << insn_alloc << " 3\n";
            out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, " << shift << ", " << len << '\n';
            out << CHECK_NEXT << insn_retf << " 0\n";

            out << "u" << size << " " << fname.str()
                << "(u" << size << " a, "
                 << "u" << size << " b)\n";
            out << "{\n";
            out << "    return (a & 0x" << std::hex << mask_inverted << std::dec << suffix
                << ") | ((b";
            if (shift != 0) {
                out << " << " << shift;
            }
            out << ") & 0x" << std::hex << mask_shifted << suffix << std::dec
                << ");\n";
            out << "}\n\n";
        }
    }
}

CODEGEN_HANDLER(IntAlignUpU64)
{
    for(int align_size = 1; align_size < 64; align_size+=2) {
        // shift_size >= align_size is just shift, ignore such
        for(int shift_size = 1; shift_size < align_size; shift_size+=2) {
            std::ostringstream fname;
            fname << "test_alignup_" << align_size << "_" << shift_size;
            out << CHECK_LABEL << fname.str() << '\n';
            out << CHECK << insn_alloc << " 2\n";
            out << CHECK_NEXT << InsnCounter(this, insn_alignup) << " %r1, %r1, " << shift_size << ", " << align_size << '\n';
            out << CHECK_NEXT << insn_retf << " 0\n";

            out << "u64 " << fname.str() << "(u64 x)\n";
            out << "{\n";
            out << "    return ((x";
            if (shift_size > 0) {
                out << " << " << shift_size;
            }
            out << ") + 0x" << std::hex << ((1LL<<align_size)-1) << std::dec << ") & 0x" << std::hex << (~((1LL<<align_size)-1)) << std::dec << ";\n";
            out << "}\n\n";
        }
    }
}

CODEGEN_HANDLER(StackArray)
{
    out << "extern int array_consumer(char *data, int size);\n";

    for(size_t size = 1; size < 256; size++) {
        std::ostringstream fname;
        fname << "test_allocsp_" << size;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << InsnCounter(this, insn_allocsp) << " 5, " << util::roundup_pow2<size_t>(size, 16) << '\n';
        out << CHECK << InsnCounter(this, insn_call) << " %r2, %jpc(array_consumer)\n";
        out << CHECK << insn_retf << " " << util::roundup_pow2<size_t>(size, 16) << '\n';

        out << "int " << fname.str() << "(void)\n";
        out << "{\n";
        out << "    char temp[" << size << "];\n";
        out << "    return array_consumer(temp, sizeof(temp));\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(FunctionPointer)
{
    out << "typedef int (*function_ptr_t)(int data);\n";

    std::ostringstream fname;
    fname << "test_func_ptr";
    out << CHECK_LABEL << fname.str() << '\n';
    out << CHECK << InsnCounter(this, insn_alloc) << " 5\n";
    out << CHECK << InsnCounter(this, insn_callri) << " %r3, %r1, %gz\n";
    out << CHECK << insn_retf << " 0\n";

    out << "int " << fname.str() << "(function_ptr_t fptr, int arg)\n";
    out << "{\n";
    out << "    return fptr(arg);\n";
    out << "}\n\n";

    const size_t offset = 3;
    fname << "test_func_ptr_table";
    out << CHECK_LABEL << fname.str() << '\n';
    out << CHECK << InsnCounter(this, insn_alloc) << " 5\n";
    out << CHECK << InsnCounter(this, insn_callmi) << " %r3, %r1, " << (offset * sizeof(u64)) << '\n';
    out << CHECK << insn_retf << " 0\n";

    out << "int " << fname.str() << "(function_ptr_t fptr[5], int arg)\n";
    out << "{\n";
    out << "    return fptr[" << offset << "](arg);\n";
    out << "}\n\n";
}

CODEGEN_HANDLER(BitSelect)
{
    static const ValueType types[] = {
        UW, IW, UD, ID, UQ, IQ, VUB, VIB, VUH, VIH, VUW, VIW, VUD, VID
    };

    static const struct {
        char  args[32];
        char  formula[32];
    } cases[] = {
        {  "%r1, %r1, %r2, %r3",  "(c & a) | (~c & b)" },
        {  "%r1, %r1, %r2, %r3",  "((a ^ b) & c) ^ b"  },
        {  "%r1, %r2, %r1, %r3",  "((a ^ b) & ~c) ^ b" }, // swap r1 <=> r2
    };

    int count = 0;

    for (const auto Vt : types) {
    for (const auto& test : cases) {
        std::ostringstream fname;
        fname << "test_bitselect_" << (++count) << "_" << Vt;

        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, insn_bitslct) << " " << test.args << '\n';

        out << Vt << " " << fname.str() << "(" << Vt << " a, " << Vt << " b, "  << Vt << " c)\n";
        out << "{\n";
        out << "    return " << test.formula << ";\n";
        out << "}\n\n";
    }}
}

} // namespace llvm
} // namespace postrisc
