#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

#define INT_UNARY_FUNCS(ITYPE, UTYPE, NBITS) \
    { "abs",           insn_abs_     ## ITYPE, #ITYPE, #ITYPE,  " %r1, %r1",            "(x < 0) ? -x : x" }, \
    { "neg",           insn_neg_     ## ITYPE, #ITYPE, #ITYPE,  " %r1, %r1",            "(-x)" }, \
    { "not",           insn_not,               #ITYPE, #ITYPE,  " %r1, %r1",            "(~x)" }, \
    { "sra_imm",       insn_sra_imm_ ## ITYPE, #ITYPE, #ITYPE,  " %r1, %r1, 20",        "(x >> 20)" }, \
    { "srl_imm",       insn_srl_imm_ ## UTYPE, #UTYPE, #UTYPE,  " %r1, %r1, 20",        "(x >> 20)" }, \
    { "sll_imm",       insn_sll_imm_ ## UTYPE, #UTYPE, #UTYPE,  " %r1, %r1, 20",        "(x << 20)" }, \
    { "sext_cast8",    insn_sext_i8,           #ITYPE, #ITYPE,  " %r1, %r1",            "(i8)x" }, \
    { "sext_cast16",   insn_sext_i16,          #ITYPE, #ITYPE,  " %r1, %r1",            "(i16)x" }, \
    { "zext_cast8",    insn_zext_i8,           #UTYPE, #UTYPE,  " %r1, %r1",            "(u8)x" }, \
    { "zext_cast8",    insn_zext_i8,           #UTYPE, "u16",   " %r1, %r1",            "(u8)x" }, \
    { "zext_cast8",    insn_zext_i8,           "u16",  #UTYPE,  " %r1, %r1",            "(u8)x" }, \
    { "zext_cast16",   insn_zext_i16,          #UTYPE, #UTYPE,  " %r1, %r1",            "(u16)x" }, \
    { "zext_mask8",    insn_zext_i8,           #UTYPE, #UTYPE,  " %r1, %r1",            "x & 0xff" }, \
    { "zext_mask16",   insn_zext_i16,          #UTYPE, #UTYPE,  " %r1, %r1",            "x & 0xffff" }, \
    { "rotate_imm",    insn_srp_imm_ ## ITYPE, #UTYPE, #UTYPE,  " %r1, %r1, %r1, 20",   "(x >> 20) | (x << (" #NBITS " - 20))" }, \

#define INT_UNARY_FUNCS_NO_128(ITYPE, UTYPE, NBITS, SUFFIX) \
    { "mul_imm"         SUFFIX, insn_mul_imm_ ## ITYPE,  #ITYPE, #ITYPE, " %r1, %r1, 98765",       "x * 98765" SUFFIX }, \
    { "mul_imm_ext"     SUFFIX, insn_mul_imm_ ## ITYPE,  #ITYPE, #ITYPE, ".l %r1, %r1, 98765432",  "x * 98765432" SUFFIX }, \
    { "mul_imm"         SUFFIX, insn_mul_imm_ ## ITYPE,  #UTYPE, #UTYPE, " %r1, %r1, 98765",       "x * 98765U" SUFFIX }, /*FIXME(?): mul_imm_iXX*/ \
    { "mul_imm_ext"     SUFFIX, insn_mul_imm_ ## ITYPE,  #UTYPE, #UTYPE, ".l %r1, %r1, 98765432",  "x * 98765432U" SUFFIX }, /*FIXME(?): mul_imm_iXX*/ \
    { "builtin_abs"     SUFFIX, insn_abs_     ## ITYPE,  #ITYPE, #ITYPE, " %r1, %r1",              "__builtin_" SUFFIX "abs(x)" }, \
    { "bit_clear_imm"   SUFFIX, insn_bit_clear_imm,      #ITYPE, #ITYPE, " %r1, %r1, 20",          "x & ~(1024" SUFFIX "*1024)" }, \
    { "shift_right_and" SUFFIX, insn_slsrl_imm_u64,      #ITYPE, #ITYPE, " %r1, %r1, 41, 57",      "(x >> 16) & 127" }, \
    { "shift_right_and" SUFFIX, insn_slsrl_imm_u64,      #UTYPE, #UTYPE, " %r1, %r1, 41, 57",      "(x >> 16) & 127" }, \

static const unary_info int_unary_functions[] = {
    INT_UNARY_FUNCS(i32,  u32,  32)
    INT_UNARY_FUNCS(i64,  u64,  64)
    INT_UNARY_FUNCS(i128, u128, 128)
    INT_UNARY_FUNCS_NO_128(i32,  u32,  32, "")
    INT_UNARY_FUNCS_NO_128(i64,  u64,  64, "l")
    INT_UNARY_FUNCS_NO_128(i64,  u64,  64, "ll")


    { "add_imm_u32",   insn_add_imm_u32,    "u64",  "u32",  " %r1, %r1, 100", "x+100" },
    { "subr_imm_u32",  insn_subr_imm_u32,   "u64",  "u32",  " %r1, %r1, 100", "100-x" },
    { "mul_imm_u32",   insn_mul_imm_u32,    "u64",  "u32",  " %r1, %r1, 100", "x*100" },

    { "bool_not",      insn_xor_imm,       "bool",  "bool",  " %r1, %r1, 1", "!x" },

    { "sext",          insn_sext_i32,       "i64",  "i32",  " %r1, %r1", "x" },
    { "sext_cast32",   insn_sext_i32,       "i64",  "i64",  " %r1, %r1", "(i32)x" },
    { "sext_cast64",   insn_sext_i64,       "i128", "i128", " %r1, %r1", "(i64)x" },

    { "zext",          insn_zext_i32,        "u64",  "u32", " %r1, %r1", "x" },
    { "zext",          insn_zext_i32,        "u128", "u32", " %r1, %r1", "x" },
    { "zext",          insn_zext_i64,        "u128", "u64", " %r1, %r1", "x" },

    { "zext_cast16",   insn_zext_i16,        "u64",  "u32", " %r1, %r1", "(u16)x" },
    { "zext_cast16",   insn_zext_i16,        "u128", "u32", " %r1, %r1", "(u16)x" },
    { "zext_cast16",   insn_zext_i16,        "u128", "u64", " %r1, %r1", "(u16)x" },

    { "zext_cast8",    insn_zext_i8,        "u16",  "u16", " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zext_i8,        "u128", "u32", " %r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zext_i8,        "u128", "u64", " %r1, %r1", "(u8)x" },

    { "zext_cast32",   insn_zext_i32,        "u128", "u16", " %r1, %r1", "(u32)x" },
    { "zext_cast32",   insn_zext_i32,        "u128", "u32", " %r1, %r1", "(u32)x" },
    { "zext_cast32",   insn_zext_i32,        "u128", "u64", " %r1, %r1", "(u32)x" },

    { "zext_cast64",   insn_zext_i64,        "u128", "u64", " %r1, %r1", "(u64)x" },

    { "zext_mask32",   insn_zext_i32,        "u64",   "u64",  " %r1, %r1", "x & 0xffffffff" },
    { "zext_mask32",   insn_zext_i32,        "u128",  "u128", " %r1, %r1", "x & 0xffffffff" },

    { "zext_mask64",   insn_zext_i64,        "u128",  "u128",  " %r1, %r1", "x & 0xffffffffffffffff" },

    // { "bit_clear_imm", insn_bit_clear_imm,      "i128", "i128", "%r1, %r1, 40",        "x & ~(((u128)1024)*1024*1024*1024)" },

    { "bit_flip_imm",  insn_bit_flip_imm,     "i32",  "i32",  " %r1, %r1, 20",      "x ^ (1024*1024)" },
    { "bit_flip_imm",  insn_bit_flip_imm,     "i64",  "i64",  " %r1, %r1, 30",      "x ^ (1024LL*1024*1024)" },
    { "bit_flip_imm",  insn_bit_flip_imm,     "i128", "i128", " %r1, %r1, 40",      "x ^ (i128)(1024LL*1024*1024*1024)" },

    { "bit_set_imm",  insn_bit_set_imm,       "i32",  "i32",  " %r1, %r1, 20",      "x | (1024*1024)" },
    { "bit_set_imm",  insn_bit_set_imm,       "i64",  "i64",  " %r1, %r1, 30",      "x | (1024LL*1024*1024)" },
    { "bit_set_imm",  insn_bit_set_imm,       "i128", "i128", " %r1, %r1, 40",      "x | (1024LL*1024*1024*1024)" },

    // ALIGNED_UP(N, NBITS) = ((N<<scale) + (2^NBITS-1)) & (~(2^NBITS-1))
    { "alignup_1",    insn_alignup_u64,       "u64",  "u64",  " %r1, %r1, 0, 20",    "(x      + ((1LL<<20)-1)) & (~((1LL<<20)-1))" },
    { "alignup_2",    insn_alignup_u64,       "u64",  "u64",  " %r1, %r1, 4, 20",    "((x<<4) + ((1LL<<20)-1)) & (~((1LL<<20)-1))" },

    { "lda_n",        insn_lda_n,             "u64",  "u64",  " %r1, %r1, -1234",    "x + (-1234LL << 20)" },

    { "slsra_imm",    insn_slsra_imm_i64,      "i64", "i64", " %r1, %r1, 10, 20",    "(x << 10) >> 20" },
};

CODEGEN_HANDLER(IntUnary)
{
    for (const auto& func : int_unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src_type;
        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, func.insn) << func.args << std::endl;
        if (func.args[0] == '.') {
            // we have double-slot main instruction, next will be in the next bundle
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

#define INT_BINARY_FUNCS(ITYPE, UTYPE, NBITS) \
    { "add",         insn_add_    ## ITYPE,   #ITYPE, #ITYPE, #ITYPE, R3_SWAP,  "a + b" }, \
    { "sub",         insn_sub_    ## ITYPE,   #ITYPE, #ITYPE, #ITYPE, R3,       "a - b" }, \
    { "div",         insn_div_    ## ITYPE,   #ITYPE, #ITYPE, #ITYPE, R3,       "a / b" }, \
    { "rem",         insn_rem_    ## ITYPE,   #ITYPE, #ITYPE, #ITYPE, R3,       "a % b" }, \
    { "udiv",        insn_div_    ## UTYPE,   #UTYPE, #UTYPE, #UTYPE, R3,       "a / b" }, \
    { "urem",        insn_rem_    ## UTYPE,   #UTYPE, #UTYPE, #UTYPE, R3,       "a % b" }, \
    { "and",         insn_and,                #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "a & b" }, \
    { "or",          insn_or,                 #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "a | b" }, \
    { "xor",         insn_xor,                #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "a ^ b" }, \
    { "xnor",        insn_xnor,               #UTYPE, #UTYPE, #UTYPE, R3,      "~(a ^ b)" }, \
    { "nand",        insn_nand,               #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "~(a & b)" }, \
    { "nor",         insn_nor,                #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "~(a | b)" }, \
    { "andn",        insn_andn,               #UTYPE, #UTYPE, #UTYPE, R3,      "(~a) & b" }, \
    { "orn",         insn_orn,                #UTYPE, #UTYPE, #UTYPE, R3,      "(~a) | b" }, \
    { "cmp_eq",      insn_cmp_eq_ ## ITYPE,   "bool", #ITYPE, #ITYPE, R3,       "a == b" }, \
    { "cmp_ne",      insn_cmp_ne_ ## ITYPE,   "bool", #ITYPE, #ITYPE, R3,       "a != b" }, \
    { "cmp_lt",      insn_cmp_lt_ ## ITYPE,   "bool", #ITYPE, #ITYPE, R3,       "a < b"  }, \
    { "cmp_le",      insn_cmp_ge_ ## ITYPE,   "bool", #ITYPE, #ITYPE, R3_SWAP,  "a <= b" }, \
    { "cmp_gt",      insn_cmp_lt_ ## ITYPE,   "bool", #ITYPE, #ITYPE, R3_SWAP,  "a > b"  }, \
    { "cmp_ge",      insn_cmp_ge_ ## ITYPE,   "bool", #ITYPE, #ITYPE, R3,       "a >= b" }, \
    { "cmp_lt",      insn_cmp_lt_ ## UTYPE,   "bool", #UTYPE, #UTYPE, R3,       "a < b"  }, \
    { "cmp_le",      insn_cmp_ge_ ## UTYPE,   "bool", #UTYPE, #UTYPE, R3_SWAP,  "a <= b" }, \
    { "cmp_gt",      insn_cmp_lt_ ## UTYPE,   "bool", #UTYPE, #UTYPE, R3_SWAP,  "a > b"  }, \
    { "cmp_ge",      insn_cmp_ge_ ## UTYPE,   "bool", #UTYPE, #UTYPE, R3,       "a >= b" }, \
    { "lt_abs_diff", insn_abs_diff_ ## ITYPE, #ITYPE, #ITYPE, #ITYPE, R3_SWAP,  "(a < b) ? (b-a) : (a-b)" }, \
    { "bit_flip32",  insn_bit_flip,           #ITYPE, #ITYPE, "i32",  R3,       "a ^ ((" #ITYPE ")1 << b)" }, \
    { "bit_flip64",  insn_bit_flip,           #ITYPE, #ITYPE, "i64",  R3,       "a ^ ((" #ITYPE ")1 << b)" }, \
    { "bit_set32",   insn_bit_set,            #ITYPE, #ITYPE, "i32",  R3,       "a | ((" #ITYPE ")1 << b)" }, \
    { "bit_set64",   insn_bit_set,            #ITYPE, #ITYPE, "i64",  R3,       "a | ((" #ITYPE ")1 << b)" }, \
    { "test_fshr1",  insn_srp_imm_ ## ITYPE,  #UTYPE, #UTYPE, #UTYPE, "%r1, %r2, %r1, 20",  "(a >> 20) | (b << (" #NBITS " - 20))" }, \
    { "test_fshr2",  insn_srp_imm_ ## ITYPE,  #UTYPE, #UTYPE, #UTYPE, "%r1, %r2, %r1, 11",  "(a >> 11) | (b << (" #NBITS " - 11))" }, \
    { "sra",         insn_sra_     ## ITYPE,  #ITYPE, #ITYPE, "i32",  "%r1, %r1, %r2",   "(a >> b)" }, \
    { "srl",         insn_srl_     ## UTYPE,  #UTYPE, #UTYPE, "i32",  "%r1, %r1, %r2",   "(a >> b)" }, \
    { "sll",         insn_sll_     ## UTYPE,  #UTYPE, #UTYPE, "i32",  "%r1, %r1, %r2",   "(a << b)" }, \
    { "min_lt_abab", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a <  b ? a : b" }, \
    { "max_ge_abab", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a >= b ? a : b" }, \
    { "min_le_abab", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a <= b ? a : b" }, \
    { "max_gt_abab", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a >  b ? a : b" }, \
    { "max_lt_baab", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b <  a ? a : b" }, \
    { "min_ge_baab", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b >= a ? a : b" }, \
    { "max_le_baab", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b <= a ? a : b" }, \
    { "min_gt_baab", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b >  a ? a : b" }, \
    { "max_lt_abba", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a <  b ? b : a" }, \
    { "min_ge_abba", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a >= b ? b : a" }, \
    { "max_le_abba", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a <= b ? b : a" }, \
    { "min_gt_abba", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3,      "a >  b ? b : a" }, \
    { "min_lt_baba", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b <  a ? b : a" }, \
    { "max_ge_baba", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b >= a ? b : a" }, \
    { "min_le_baba", insn_min_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b <= a ? b : a" }, \
    { "max_gt_baba", insn_max_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, R3_SWAP, "b >  a ? b : a" }, \
    { "min_lt_abab", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a <  b ? a : b" }, \
    { "max_ge_abab", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a >= b ? a : b" }, \
    { "min_le_abab", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a <= b ? a : b" }, \
    { "max_gt_abab", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a >  b ? a : b" }, \
    { "max_lt_baab", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b <  a ? a : b" }, \
    { "min_ge_baab", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b >= a ? a : b" }, \
    { "max_le_baab", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b <= a ? a : b" }, \
    { "min_gt_baab", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b >  a ? a : b" }, \
    { "max_lt_abba", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a <  b ? b : a" }, \
    { "min_ge_abba", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a >= b ? b : a" }, \
    { "max_le_abba", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a <= b ? b : a" }, \
    { "min_gt_abba", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3,      "a >  b ? b : a" }, \
    { "min_lt_baba", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b <  a ? b : a" }, \
    { "max_ge_baba", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b >= a ? b : a" }, \
    { "min_le_baba", insn_min_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b <= a ? b : a" }, \
    { "max_gt_baba", insn_max_     ## UTYPE,  #UTYPE, #UTYPE, #UTYPE, R3_SWAP, "b >  a ? b : a" }, \

#define INT_BINARY_FUNCS_NO_128(ITYPE, UTYPE, NBITS, SUFFIX) \
    { "mul"         SUFFIX, insn_mul_     ## ITYPE,  #ITYPE, #ITYPE, #ITYPE,  R3_SWAP,  "a * b" }, \
    { "sl_add"      SUFFIX, insn_sl_add_  ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, "%r1, %r1, %r2, 3",  "a + (b << 3)" }, \
    { "sl_sub"      SUFFIX, insn_sl_sub_  ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, "%r1, %r1, %r2, 3",  "a - (b << 3)" }, \
    { "sl_subr"     SUFFIX, insn_sl_subr_ ## ITYPE,  #ITYPE, #ITYPE, #ITYPE, "%r1, %r1, %r2, 3",  "(b << 3) - a" }, \
    { "sl_or"       SUFFIX, insn_sl_or,              #ITYPE, #ITYPE, #ITYPE, "%r1, %r1, %r2, 3",  "a | (b << 3)" }, \
    { "sl_xor"      SUFFIX, insn_sl_xor,             #ITYPE, #ITYPE, #ITYPE, "%r1, %r1, %r2, 3",  "a ^ (b << 3)" }, \
    { "bit_clear32" SUFFIX, insn_bit_clear,          #ITYPE, #ITYPE, "i32",  R3,       "a & ~((" #ITYPE ")1 << b)" }, \
    { "bit_clear64" SUFFIX, insn_bit_clear,          #ITYPE, #ITYPE, "i64",  R3,       "a & ~((" #ITYPE ")1 << b)" }, \
    { "bit_clear"   SUFFIX, insn_bit_clear,          #ITYPE, #ITYPE, "i32",  R3,       "a & ~(1" SUFFIX " << b)" }, \
    { "bit_clear"   SUFFIX, insn_bit_clear,          #ITYPE, #ITYPE, "i64",  R3,       "a & ~(1" SUFFIX " << b)" }, \
    { "bit_flip"    SUFFIX, insn_bit_flip,           #ITYPE, #ITYPE, "i32",  R3,       "a ^ (1" SUFFIX " << b)" }, \
    { "bit_flip"    SUFFIX, insn_bit_flip,           #ITYPE, #ITYPE, "i64",  R3,       "a ^ (1" SUFFIX " << b)" }, \
    { "bit_set"     SUFFIX, insn_bit_set,            #ITYPE, #ITYPE, "i32",  R3,       "a | (1" SUFFIX " << b)" }, \
    { "bit_set"     SUFFIX, insn_bit_set,            #ITYPE, #ITYPE, "i64",  R3,       "a | (1" SUFFIX " << b)" }, \
    { "abs_diff"    SUFFIX, insn_abs_diff_ ## ITYPE, #ITYPE, #ITYPE, #ITYPE, R3_SWAP,  "__builtin_" SUFFIX "abs(a - b)" },

#define R3      "%r1, %r1, %r2"
#define R3_SWAP "%r1, %r2, %r1"

static const binary_info int_binary_functions[] = {
    INT_BINARY_FUNCS(i32,  u32,  32)
    INT_BINARY_FUNCS(i64,  u64,  64)
    INT_BINARY_FUNCS(i128, u128, 128)
    INT_BINARY_FUNCS_NO_128(i32,  u32,  32, "")
    INT_BINARY_FUNCS_NO_128(i64,  u64,  64, "l")
    INT_BINARY_FUNCS_NO_128(i64,  u64,  64, "ll")

    { "add",         insn_add_u32,  "u64", "u32", "u32", R3_SWAP,  "a + b" },
    { "sub",         insn_sub_u32,  "u64", "u32", "u32", R3,       "a - b" },

    { "sl_add_u32",  insn_sl_add_u32,   "u64", "u32", "u32",   "%r1, %r1, %r2, 3",  "a + (b << 3)" },
    { "sl_sub_u32",  insn_sl_sub_u32,   "u64", "u32", "u32",   "%r1, %r1, %r2, 3",  "a - (b << 3)" },
    { "sl_subr_u32", insn_sl_subr_u32,  "u64", "u32", "u32",   "%r1, %r1, %r2, 3",  "(b << 3) - a" },

    { "bool_and",   insn_and,     "bool", "bool", "bool",  R3,       "a & b" },
    { "bool_or",    insn_or,      "bool", "bool", "bool",  R3,       "a | b" },
    { "bool_xor",   insn_xor,     "bool", "bool", "bool",  R3,       "a ^ b" },

    // sext/zext to a bigger type
    { "mul",   insn_mul_i32,     "i64", "i32", "i32",  R3_SWAP,       "a * b" },
    { "mul",   insn_mul_u32,     "u64", "u32", "u32",  R3_SWAP,       "a * b" },
    { "mul",   insn_mul_i32,     "u64", "i32", "i32",  R3_SWAP,       "a * b" }, //FIXME: why mul_i32?

    { "addc",      insn_addc_u64,      "u128", "u64", "u64", R3_SWAP,  "(u128)a + (u128)b" },
    { "subb",      insn_subb_u64,      "u128", "u64", "u64", R3,       "(u128)a - (u128)b" },

    { "deposit_1", insn_deposit,      "u32", "u32", "u32", "%r1, %r1, %r2, 4, 12",  "(a & 0xFFFF000F) | ((b << 4) & 0x0000FFF0)" },
    { "deposit_2", insn_deposit,      "u32", "u32", "u32", "%r1, %r1, %r2, 8, 12",  "(a & 0xFFF000FF) | ((b << 8) & 0x000FFF00)" },
    { "deposit_1", insn_deposit,      "u64", "u64", "u64", "%r1, %r1, %r2, 4, 12",  "(a & 0xFFFFFFFFFFFF000F) | ((b << 4) & 0x0000FFF0)" },
    { "deposit_2", insn_deposit,      "u64", "u64", "u64", "%r1, %r1, %r2, 8, 12",  "(a & 0xFFFFFFFFFFF000FF) | ((b << 8) & 0x000FFF00)" },
};

CODEGEN_HANDLER(IntBinary)
{
    for (const auto& func : int_binary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type << "_" << func.src2_type;
        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}


#define INT_TERNARY_FUNCS(ITYPE, UTYPE, NBITS) \
    { "cmov_eq",       insn_cmov_eq_ ## ITYPE,   #ITYPE, #ITYPE, #ITYPE, #ITYPE, R4,       "(a == 0) ? b : c" }, \
    { "cmov_lt",       insn_cmov_lt_ ## ITYPE,   #ITYPE, #ITYPE, #ITYPE, #ITYPE, R4,       "(a <  0) ? b : c" }, \
    { "cmov_le",       insn_cmov_le_ ## ITYPE,   #ITYPE, #ITYPE, #ITYPE, #ITYPE, R4,       "(a <= 0) ? b : c" }, \
    { "cmov_lsb_1",    insn_cmov_lsb,            #ITYPE, #ITYPE, #ITYPE, #ITYPE, R4_LSB,   "(a & 1) ? b : c" }, \
    { "cmov_lsb_2",    insn_cmov_lsb,            #ITYPE, #ITYPE, #ITYPE, #ITYPE, R4_LSB,   "((a & 1) != 0) ? b : c" }, \
    { "cmov_lsb_3",    insn_cmov_lsb,            #ITYPE, #ITYPE, #ITYPE, #ITYPE, R4_LSB,   "((a & 1) == 1) ? b : c" }, \
    { "slp",           insn_slp_     ## ITYPE,   #ITYPE, #UTYPE, #UTYPE, "i32",  R4,       "(a << c) | (b >> (" #NBITS " - c))" }, \
    { "srp",           insn_srp_     ## ITYPE,   #ITYPE, #UTYPE, #UTYPE, "i32",  R4,       "(b >> c) | (a << (" #NBITS " - c))" }, \

#define R4     "%r1, %r1, %r2, %r3"
#define R4_LSB "%r1, %r1, %r3, %r2"

static const ternary_info int_ternary_functions[] = {
    INT_TERNARY_FUNCS(i32,  u32,  32)
    INT_TERNARY_FUNCS(i64,  u64,  64)
    INT_TERNARY_FUNCS(i128, u128, 128)

    { "add_add",       insn_add_add_i64,       "u64",  "u64", "u64", "u64",  "%r1, %r2, %r1, %r3",  "a + b + c" },
    { "add_sub",       insn_add_sub_i64,       "u64",  "u64", "u64", "u64",  "%r1, %r2, %r1, %r3",  "a + b - c" },
    { "sub_sub",       insn_sub_sub_i64,       "u64",  "u64", "u64", "u64",  "%r1, %r1, %r2, %r3",  "a - b - c" },
    { "mul_add",       insn_mul_add,           "u64",  "u64", "u64", "u64",  "%r1, %r2, %r1, %r3",  "a * b + c" },
    { "mul_sub",       insn_mul_sub,           "u64",  "u64", "u64", "u64",  "%r1, %r2, %r1, %r3",  "a * b - c" },
    { "mul_subr",      insn_mul_subr,          "u64",  "u64", "u64", "u64",  "%r1, %r2, %r1, %r3",  "c - a * b" },

    { "slsra",         insn_slsra_i32,         "i32",  "i32", "i32", "i32", R4, "(a << b) >> c" },
    { "slsrl",         insn_slsrl_u32,         "u32",  "u32", "u32", "i32", R4, "(a << b) >> c" },
    { "slsra",         insn_slsra_i64,         "i64",  "i64", "i64", "i32", R4, "(a << b) >> c" },
    { "slsrl",         insn_slsrl_u64,         "u64",  "u64", "u64", "i32", R4, "(a << b) >> c" },

    { "add_addc",      insn_add_addc_u64,      "u128", "u64", "u64", "u128", "%r1, %r2, %r1, %r3",  "(u128)a + (u128)b + (c >> 64)" },
    { "sub_subb",      insn_sub_subb_u64,      "u128", "u64", "u64", "u128", R4,                    "(u128)a - (u128)b - (c >> 64)" },
};

CODEGEN_HANDLER(IntTernary)
{
    for (const auto& func : int_ternary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type;
        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << std::endl;

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b, "
                   << func.src3_type << " c)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
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
            out << CHECK_LABEL << fname.str() << std::endl;
            out << CHECK << insn_name(insn_alloc) << " 3" << std::endl;
            out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, " << shift << ", " << len << std::endl;
            out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

            out << "u" << size << " " << fname.str()
                << "(u" << size << " a, "
                 << "u" << size << " b)" << std::endl;
            out << "{" << std::endl;
            out << "    return (a & 0x" << std::hex << mask_inverted << std::dec << suffix
                << ") | ((b";
            if (shift != 0) {
                out << " << " << shift;
            }
            out << ") & 0x" << std::hex << mask_shifted << suffix << std::dec
                << ");" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }
    }
}

CODEGEN_HANDLER(IntAlignUpU64)
{
    for(int align_size = 1; align_size < 64; align_size+=2) {
        // shift_size >= align_size is just shift, ignore such
        for(int shift_size = 1; shift_size < align_size; shift_size+=2) {
            std::ostringstream fname;
            fname << "test_alignup_u64_" << align_size << "_" << shift_size;
            out << CHECK_LABEL << fname.str() << std::endl;
            out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
            out << CHECK_NEXT << InsnCounter(this, insn_alignup_u64) << " %r1, %r1, " << shift_size << ", " << align_size << std::endl;
            out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

            out << "u64 " << fname.str() << "(u64 x)" << std::endl;
            out << "{" << std::endl;
            out << "    return ((x";
            if (shift_size > 0) {
                out << " << " << shift_size;
            }
            out << ") + 0x" << std::hex << ((1LL<<align_size)-1) << std::dec << ") & 0x" << std::hex << (~((1LL<<align_size)-1)) << std::dec << ";" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }
    }
}

CODEGEN_HANDLER(StackArray)
{
    out << "extern int array_consumer(char *data, int size);" << std::endl;

    for(size_t size = 1; size < 256; size++) {
        std::ostringstream fname;
        fname << "test_alloc_sp_" << size;
        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << InsnCounter(this, insn_alloc_sp) << " 5, " << util::roundup_pow2<size_t>(size, 16) << std::endl;
        out << CHECK << InsnCounter(this, insn_call) << " %r2, %jpc(array_consumer)" << std::endl;
        out << CHECK << insn_name(insn_retf) << " " << util::roundup_pow2<size_t>(size, 16) << std::endl;

        out << "int " << fname.str() << "(void)" << std::endl;
        out << "{" << std::endl;
        out << "    char temp[" << size << "];" << std::endl;
        out << "    return array_consumer(temp, sizeof(temp));" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(FunctionPointer)
{
    out << "typedef int (*function_ptr_t)(int data);" << std::endl;

    std::ostringstream fname;
    fname << "test_func_ptr";
    out << CHECK_LABEL << fname.str() << std::endl;
    out << CHECK << InsnCounter(this, insn_alloc) << " 5" << std::endl;
    out << CHECK << InsnCounter(this, insn_call_ri) << " %r3, %r1, %gz" << std::endl;
    out << CHECK << insn_name(insn_retf) << " 0" << std::endl;

    out << "int " << fname.str() << "(function_ptr_t fptr, int arg)" << std::endl;
    out << "{" << std::endl;
    out << "    return fptr(arg);" << std::endl;
    out << "}" << std::endl;
    out << std::endl;

    const size_t offset = 3;
    fname << "test_func_ptr_table";
    out << CHECK_LABEL << fname.str() << std::endl;
    out << CHECK << InsnCounter(this, insn_alloc) << " 5" << std::endl;
    out << CHECK << InsnCounter(this, insn_call_mi) << " %r3, %r1, " << (offset * sizeof(u64)) << std::endl;
    out << CHECK << insn_name(insn_retf) << " 0" << std::endl;

    out << "int " << fname.str() << "(function_ptr_t fptr[5], int arg)" << std::endl;
    out << "{" << std::endl;
    out << "    return fptr[" << offset << "](arg);" << std::endl;
    out << "}" << std::endl;
    out << std::endl;
}

} // namespace llvm
} // namespace postrisc
