#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static const func_info int_unary_functions[] = {
    { "sext",          insn_sext_i32,       "i64",  "i32",  "%r1, %r1", "x" },
    { "sext_cast8",    insn_sext_i8,        "i64",  "i64",  "%r1, %r1", "(i8)x" },
    { "sext_cast16",   insn_sext_i16,       "i64",  "i64",  "%r1, %r1", "(i16)x" },
    { "sext_cast32",   insn_sext_i32,       "i64",  "i64",  "%r1, %r1", "(i32)x" },
    { "sext_cast64",   insn_sext_i64,       "i128", "i128", "%r1, %r1", "(i64)x" },

    { "zext_cast8",    insn_zext_i8,        "u16",  "u16", "%r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zext_i8,        "u32",  "u16", "%r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zext_i8,        "u64",  "u16", "%r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zext_i8,        "u128", "u16", "%r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zext_i8,        "u128", "u32", "%r1, %r1", "(u8)x" },
    { "zext_cast8",    insn_zext_i8,        "u128", "u64", "%r1, %r1", "(u8)x" },

    { "zext_cast16",   insn_zext_i32,        "u64",  "u16", "%r1, %r1", "(u16)x" }, // FIXME?
    { "zext_cast16",   insn_zext_i32,        "u128", "u16", "%r1, %r1", "(u16)x" }, // FIXME?
    { "zext_cast16",   insn_zext_i16,        "u128", "u32", "%r1, %r1", "(u16)x" },
    { "zext_cast16",   insn_zext_i16,        "u128", "u64", "%r1, %r1", "(u16)x" },

    { "zext",          insn_zext_i32,        "u64",  "u32", "%r1, %r1", "x" },
    { "zext_cast32",   insn_zext_i32,        "u128", "u16", "%r1, %r1", "(u32)x" },
    { "zext_cast32",   insn_zext_i32,        "u128", "u32", "%r1, %r1", "(u32)x" },
    { "zext_cast32",   insn_zext_i32,        "u128", "u64", "%r1, %r1", "(u32)x" },

    { "zext",          insn_zext_i64,        "u128", "u64", "%r1, %r1", "x" },
    { "zext_cast64",   insn_zext_i64,        "u128", "u64", "%r1, %r1", "(u64)x" },

    { "zext_mask8",    insn_zext_i8,         "u32",   "u32",  "%r1, %r1", "x & 0xff" },
    { "zext_mask8",    insn_zext_i8,         "u64",   "u64",  "%r1, %r1", "x & 0xff" },
    { "zext_mask8",    insn_zext_i8,         "u128",  "u128", "%r1, %r1", "x & 0xff" },

    { "zext_mask16",   insn_zext_i16,        "u32",   "u32",  "%r1, %r1", "x & 0xffff" },
    { "zext_mask16",   insn_zext_i16,        "u64",   "u64",  "%r1, %r1", "x & 0xffff" },
    { "zext_mask16",   insn_zext_i16,        "u128",  "u128", "%r1, %r1", "x & 0xffff" },

    { "zext_mask32",   insn_zext_i32,        "u64",   "u64",  "%r1, %r1", "x & 0xffffffff" },
    { "zext_mask32",   insn_zext_i32,        "u128",  "u128", "%r1, %r1", "x & 0xffffffff" },

    { "zext_mask64",   insn_zext_i64,        "u128",  "u128",  "%r1, %r1", "x & 0xffffffffffffffff" },

    { "neg",           insn_neg_i32,        "i32", "i32", "%r1, %r1", "(-x)" },
    { "abs",           insn_abs_i32,        "i32", "i32", "%r1, %r1", "(x < 0) ? -x : x" },
    { "builtin_abs",   insn_abs_i32,        "i32", "i32", "%r1, %r1", "__builtin_abs(x)" },
    { "builtin_abs_l", insn_abs_i64,        "i64", "i64", "%r1, %r1", "__builtin_labs(x)" },

    { "neg",           insn_neg_i64,        "i64", "i64", "%r1, %r1", "(-x)" },
    { "abs",           insn_abs_i64,        "i64", "i64", "%r1, %r1", "(x < 0) ? -x : x" },
    { "builtin_abs",   insn_abs_i64,        "i64", "i64", "%r1, %r1", "__builtin_llabs(x)" },

    { "neg",           insn_neg_i128,       "i128", "i128", "%r1, %r1", "(-x)" },
    { "abs",           insn_abs_i128,       "i128", "i128", "%r1, %r1", "(x < 0) ? -x : x" },

    { "bit_clear_imm", insn_bit_clear_imm,    "i32",  "i32",  "%r1, %r1, 20",        "x & ~(1024*1024)" },
    { "bit_clear_imm", insn_bit_clear_imm,    "i64",  "i64",  "%r1, %r1, 30",        "x & ~(1024LL*1024*1024)" },

    { "bit_flip_imm",  insn_bit_flip_imm,     "i32",  "i32",  "%r1, %r1, 20",        "x ^ (1024*1024)" },
    { "bit_flip_imm",  insn_bit_flip_imm,     "i64",  "i64",  "%r1, %r1, 30",        "x ^ (1024LL*1024*1024)" },
    { "bit_flip_imm",  insn_bit_flip_imm,     "i128", "i128", "%r1, %r1, 40",        "x ^ (i128)(1024LL*1024*1024*1024)" },

    { "bit_set_imm",  insn_bit_set_imm,       "i32",  "i32",  "%r1, %r1, 20",        "x | (1024*1024)" },
    { "bit_set_imm",  insn_bit_set_imm,       "i64",  "i32",  "%r1, %r1, 30",        "x | (1024LL*1024*1024)" },
    { "bit_set_imm",  insn_bit_set_imm,       "i128", "i32",  "%r1, %r1, 40",        "x | (1024LL*1024*1024*1024)" },

    // { "bit_clear_imm", insn_bit_clear_imm,      "i128", "i128", "%r1, %r1, 40",        "x & ~(((u128)1024)*1024*1024*1024)" },
};

CODEGEN_HANDLER(IntUnary)
{
    for (const auto& func : int_unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src_type;
        out << "// CHECK-LABEL: @" << fname.str() << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: " << asm_name[func.insn] << " " << func.args << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

static const func_info int_binary_functions[] = {
    { "add",       insn_add_i32,        "i32", "i32", "%r1, %r2, %r1",       "a + b" },
    { "sub",       insn_sub_i32,        "i32", "i32", "%r1, %r1, %r2",       "a - b" },
    { "mul",       insn_mul_i32,        "i32", "i32", "%r1, %r2, %r1",       "a * b" },
    { "div",       insn_div_i32,        "i32", "i32", "%r1, %r1, %r2",       "a / b" },
    { "rem",       insn_rem_i32,        "i32", "i32", "%r1, %r1, %r2",       "a % b" },

    { "cmp_eq",   insn_cmp_eq_i32,   "bool", "i32", "%r1, %r1, %r2",       "a == b" },
    { "cmp_ne",   insn_cmp_ne_i32,   "bool", "i32", "%r1, %r1, %r2",       "a != b" },
    { "cmp_lt",   insn_cmp_lt_i32,   "bool", "i32", "%r1, %r1, %r2",       "a < b" },
    { "cmp_le",   insn_cmp_ge_i32,   "bool", "i32", "%r1, %r2, %r1",       "a <= b" }, // swap
    { "cmp_gt",   insn_cmp_lt_i32,   "bool", "i32", "%r1, %r2, %r1",       "a > b" }, // swap
    { "cmp_ge",   insn_cmp_ge_i32,   "bool", "i32", "%r1, %r1, %r2",       "a >= b" },

    { "add",       insn_add_i64,        "i64", "i64", "%r1, %r2, %r1",       "a + b" },
    { "sub",       insn_sub_i64,        "i64", "i64", "%r1, %r1, %r2",       "a - b" },
    { "mul",       insn_mul_i64,        "i64", "i64", "%r1, %r2, %r1",       "a * b" },
    { "div",       insn_div_i64,        "i64", "i64", "%r1, %r1, %r2",       "a / b" },
    { "rem",       insn_rem_i64,        "i64", "i64", "%r1, %r1, %r2",       "a % b" },

    { "cmp_eq",   insn_cmp_eq_i64,   "bool", "i64", "%r1, %r1, %r2",       "a == b" },
    { "cmp_ne",   insn_cmp_ne_i64,   "bool", "i64", "%r1, %r1, %r2",       "a != b" },
    { "cmp_lt",   insn_cmp_lt_i64,   "bool", "i64", "%r1, %r1, %r2",       "a < b" },
    { "cmp_le",   insn_cmp_ge_i64,   "bool", "i64", "%r1, %r2, %r1",       "a <= b" }, // swap
    { "cmp_gt",   insn_cmp_lt_i64,   "bool", "i64", "%r1, %r2, %r1",       "a > b" }, // swap
    { "cmp_ge",   insn_cmp_ge_i64,   "bool", "i64", "%r1, %r1, %r2",       "a >= b" },

    { "bit_clear1", insn_bit_clear,   "i32", "i32", "%r1, %r1, %r2",        "a & ~(1 << b)" },
    { "bit_clear2", insn_bit_clear,   "i64", "i32", "%r1, %r1, %r2",        "a & ~((i64)1 << b)" },
    { "bit_clear3", insn_bit_clear,   "i64", "i64", "%r1, %r1, %r2",        "a & ~((i64)1 << b)" },

    // { "bit_clear1", insn_bit_clear,   "i128", "i128", "%r1, %r1, %r2",        "a & ~((128)1 << b)" },

    { "bit_flip1",   insn_bit_flip,   "i32",  "i32", "%r1, %r1, %r2",        "a ^ (1 << b)" },
    { "bit_flip2",   insn_bit_flip,   "i64",  "i32", "%r1, %r1, %r2",        "a ^ ((i64)1 << b)" },
    { "bit_flip3",   insn_bit_flip,   "i64",  "i64", "%r1, %r1, %r2",        "a ^ ((i64)1 << b)" },
    { "bit_flip4",   insn_bit_flip,   "i128", "i32", "%r1, %r1, %r2",        "a ^ ((i128)1 << b)" },
    { "bit_flip5",   insn_bit_flip,   "i128", "i64", "%r1, %r1, %r2",        "a ^ ((i128)1 << b)" },

    { "bit_set1",    insn_bit_set,    "i32",  "i32", "%r1, %r1, %r2",        "a | (1 << b)" },
    { "bit_set2",    insn_bit_set,    "i64",  "i32", "%r1, %r1, %r2",        "a | ((i64)1 << b)" },
    { "bit_set3",    insn_bit_set,    "i64",  "i64", "%r1, %r1, %r2",        "a | ((i64)1 << b)" },
    { "bit_set4",    insn_bit_set,    "i128", "i32", "%r1, %r1, %r2",        "a | ((i128)1 << b)" },
    { "bit_set5",    insn_bit_set,    "i128", "i64", "%r1, %r1, %r2",        "a | ((i128)1 << b)" },

    { "abs_diff",            insn_abs_diff_i32,   "i32", "i32", "%r1, %r2, %r1",       "(a < b) ? (b-a) : (a-b)" },
    { "builtin_abs_diff",    insn_abs_diff_i32,   "i32", "i32", "%r1, %r2, %r1",       "__builtin_abs(a - b)" },
    { "builtin_abs_diff_l",  insn_abs_diff_i32,   "i32", "i32", "%r1, %r2, %r1",       "__builtin_labs(a - b)" },

    { "abs_diff",            insn_abs_diff_i64,   "i64", "i64", "%r1, %r2, %r1",       "(a < b) ? (b-a) : (a-b)" },
    { "builtin_abs_diff",    insn_abs_diff_i64,   "i64", "i64", "%r1, %r2, %r1",       "__builtin_llabs(a - b)" },
};

CODEGEN_HANDLER(IntBinary)
{
    for (const auto& func : int_binary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src_type;
        out << "// CHECK-LABEL: @" << fname.str() << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " a, " << func.src_type << " b)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: " << asm_name[func.insn] << " " << func.args << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

static const func_info int_ternary_functions[] = {
    { "cmov_eq",       insn_cmov_eq_i32,       "i32", "i32", "%r1, %r1, %r2, %r3",       "(a == 0) ? b : c" },
    { "cmov_lt",       insn_cmov_lt_i32,       "i32", "i32", "%r1, %r1, %r2, %r3",       "(a <  0) ? b : c" },
    { "cmov_le",       insn_cmov_le_i32,       "i32", "i32", "%r1, %r1, %r2, %r3",       "(a <= 0) ? b : c" },

    { "cmov_eq",       insn_cmov_eq_i64,       "i64", "i64", "%r1, %r1, %r2, %r3",       "(a == 0) ? b : c" },
    { "cmov_lt",       insn_cmov_lt_i64,       "i64", "i64", "%r1, %r1, %r2, %r3",       "(a <  0) ? b : c" },
    { "cmov_le",       insn_cmov_le_i64,       "i64", "i64", "%r1, %r1, %r2, %r3",       "(a <= 0) ? b : c" },

    //{ "cmov_eq",       insn_cmov_eq_i128,      "i128", "i128", "%r1, %r1, %r2, %r3",       "(a == 0) ? b : c" },
    //{ "cmov_lt",       insn_cmov_lt_i128,      "i128", "i128", "%r1, %r1, %r2, %r3",       "(a <  0) ? b : c" },
    //{ "cmov_le",       insn_cmov_le_i128,      "i128", "i128", "%r1, %r1, %r2, %r3",       "(a <= 0) ? b : c" },
};

CODEGEN_HANDLER(IntTernary)
{
    for (const auto& func : int_ternary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src_type;
        out << "// CHECK-LABEL: @" << fname.str() << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " a, " << func.src_type << " b," << func.src_type << " c)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: " << asm_name[func.insn] << " " << func.args << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

} // namespace llvm
} // namespace postrisc
