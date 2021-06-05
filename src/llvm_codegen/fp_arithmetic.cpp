#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static void ExtendToWider(std::ostream& out, const char *flesser, const char *fbigger)
{
    std::ostringstream fname;
    fname << "extend_" << flesser << "_" << fbigger;

    out << "// CHECK-LABEL: @" << fname.str() << std::endl;
    out << "// CHECK: extend_" << flesser << "_" << fbigger << " %r1, %r1" << std::endl;
    out << fbigger << " " << fname.str() << "(" << flesser << " value) { return value; }" << std::endl;
}

static void RoundToNarrow(std::ostream& out, const char *fbigger, const char *flesser)
{
    std::ostringstream fname;
    fname << "convert_" << fbigger << "_" << flesser;

    out << "// CHECK-LABEL: @" << fname.str() << std::endl;

    out << "// CHECK: cvt_" << fbigger << "_" << flesser << " %r1, %r1" << std::endl;
    out << flesser << " " << fname.str() << "(" << fbigger << " value) { return value; }" << std::endl;
}

CODEGEN_HANDLER(FpToInt)
{
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// fp extend to wider" << std::endl;
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << std::endl;

    ExtendToWider(out, "f16", "f32");
    ExtendToWider(out, "f16", "f64");
    ExtendToWider(out, "f16", "f128");
    ExtendToWider(out, "f32", "f64");
    ExtendToWider(out, "f32", "f128");
    ExtendToWider(out, "f64", "f128");

    out << std::endl;

    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// fp round to narrow" << std::endl;
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << std::endl;

    RoundToNarrow(out, "f32",  "f16");
    RoundToNarrow(out, "f64",  "f16");
    RoundToNarrow(out, "f128", "f16");
    RoundToNarrow(out, "f64",  "f32");
    RoundToNarrow(out, "f128", "f32");
    RoundToNarrow(out, "f128", "f64");

    out << std::endl;

    for (const char * ftype : {"f16", "f32", "f64", "f128"}) {
        out << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << "// " << ftype << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << std::endl;

        for (const char * itype : {"i32", "u32", "i64", "u64", "i128", "u128"}) {
            {
            std::ostringstream fname;
            fname << "trunc_" << ftype << "_" << itype;

            out << "// CHECK-LABEL: @" << fname.str() << std::endl;
            out << itype << " " << fname.str() << "(" << ftype << " value)" << std::endl;
            out << "{" << std::endl;

            out << "// CHECK: trunc_" << ftype << "_" << itype << " %r1, %r1\n";
            out << "    return (" << itype << ")value;" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
            }

            {
            std::ostringstream fname;
            fname << "convert_" << itype << "_" << ftype;

            out << "// CHECK-LABEL: @" << fname.str() << std::endl;
            out << ftype << " " << fname.str() << "(" << itype << " value)" << std::endl;
            out << "{" << std::endl;

            out << "// CHECK: cvt_" << itype << "_" << ftype << " %r1, %r1\n";
            out << "    return (" << ftype << ")value;" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
            }
        }
    }
}

// FLOAT-FLOAT rounding
//
// roundeven
// rounds to the nearest integer, with halfway cases (e.g., .5) rounding to the nearest even
// rint, nearbyint:
// rounds the floating-point argument to an integer value in floating-point format, using the current rounding mode.
// round:
// Computes the nearest integer value to arg (in floating-point format),
// rounding halfway cases away from zero, regardless of the current rounding mode.
// trunc:
// rounds to nearest integer not greater in magnitude than the given value
// floor:
// computes largest integer not greater than the given value
// ceil:
// computes smallest integer not less than the given value

static const func_info unary_functions[] = {
    { "neg",        insn_neg_f32,        "f32", "f32", "%r1, %r1", "(-x)" },
    { "abs",        insn_abs_f32,        "f32", "f32", "%r1, %r1", "__builtin_fabsf(x)" },
    { "neg_abs",    insn_nabs_f32,       "f32", "f32", "%r1, %r1", "- __builtin_fabsf(x)" },
    { "roundeven",  insn_roundeven_f32,  "f32", "f32", "%r1, %r1", "__builtin_roundevenf(x)" },
    { "round",      insn_round_f32,      "f32", "f32", "%r1, %r1", "__builtin_roundf(x)" },
    { "floor",      insn_floor_f32,      "f32", "f32", "%r1, %r1", "__builtin_floorf(x)" },
    { "ceil",       insn_ceil_f32,       "f32", "f32", "%r1, %r1", "__builtin_ceilf(x)" },
    { "trunc",      insn_trunc_f32,      "f32", "f32", "%r1, %r1", "__builtin_truncf(x)" },
    { "nearbyint",  insn_nearbyint_f32,  "f32", "f32", "%r1, %r1", "__builtin_nearbyintf(x)" },
    { "rint",       insn_rint_f32,       "f32", "f32", "%r1, %r1", "__builtin_rintf(x)" },

    { "neg",        insn_neg_f64,        "f64", "f64", "%r1, %r1", "(-x)" },
    { "abs",        insn_abs_f64,        "f64", "f64", "%r1, %r1", "__builtin_fabs(x)" },
    { "neg_abs",    insn_nabs_f64,       "f64", "f64", "%r1, %r1", "- __builtin_fabs(x)" },
    { "roundeven",  insn_roundeven_f64,  "f64", "f64", "%r1, %r1", "__builtin_roundeven(x)" },
    { "round",      insn_round_f64,      "f64", "f64", "%r1, %r1", "__builtin_round(x)" },
    { "floor",      insn_floor_f64,      "f64", "f64", "%r1, %r1", "__builtin_floor(x)" },
    { "ceil",       insn_ceil_f64,       "f64", "f64", "%r1, %r1", "__builtin_ceil(x)" },
    { "trunc",      insn_trunc_f64,      "f64", "f64", "%r1, %r1", "__builtin_trunc(x)" },
    { "nearbyint",  insn_nearbyint_f64,  "f64", "f64", "%r1, %r1", "__builtin_nearbyint(x)" },
    { "rint",       insn_rint_f64,       "f64", "f64", "%r1, %r1", "__builtin_rint(x)" },

    // { "", insn_class_f64,      "bool", "f64", "__builtin_isnan(x)" },

    { "neg",        insn_neg_f128,       "f128", "f128", "%r1, %r1", "(-x)" },
    { "abs",        insn_abs_f128,       "f128", "f128", "%r1, %r1", "__builtin_fabsl(x)" },
    { "neg_abs",    insn_nabs_f128,      "f128", "f128", "%r1, %r1", "- __builtin_fabsl(x)" },
    { "roundeven",  insn_roundeven_f128, "f128", "f128", "%r1, %r1", "__builtin_roundevenl(x)" },
    { "round",      insn_round_f128,     "f128", "f128", "%r1, %r1", "__builtin_roundl(x)" },
    { "floor",      insn_floor_f128,     "f128", "f128", "%r1, %r1", "__builtin_floorl(x)" },
    { "ceil",       insn_ceil_f128,      "f128", "f128", "%r1, %r1", "__builtin_ceill(x)" },
    { "trunc",      insn_trunc_f128,     "f128", "f128", "%r1, %r1", "__builtin_truncl(x)" },
    { "nearbyint",  insn_nearbyint_f128, "f128", "f128", "%r1, %r1", "__builtin_nearbyintl(x)" },
    { "rint",       insn_rint_f128,      "f128", "f128", "%r1, %r1", "__builtin_rintl(x)" },
};

CODEGEN_HANDLER(FpUnary)
{
    for (const auto& func : unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.src_type << "_" << func.dst_type;
        out << "// CHECK-LABEL: @" << fname.str() << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: " << asm_name[func.insn] << " " << func.args << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

static const func_info binary_functions[] = {
    { "add",       insn_add_f32,        "f32", "f32", "%r1, %r2, %r1",       "a + b" },
    { "sub",       insn_sub_f32,        "f32", "f32", "%r1, %r1, %r2",       "a - b" },
    { "mul",       insn_mul_f32,        "f32", "f32", "%r1, %r2, %r1",       "a * b" },
    { "div",       insn_div_f32,        "f32", "f32", "%r1, %r1, %r2",       "a / b" },
    { "abs_diff",  insn_abs_diff_f32,   "f32", "f32", "%r1, %r1, %r2",       "__builtin_fabsf(a - b)" },
    { "nabs_diff", insn_nabs_diff_f32,  "f32", "f32", "%r1, %r1, %r2",       "- __builtin_fabsf(a - b)" },
    { "copysign",  insn_merge_f32,      "f32", "f32", "%r1, %r2, %r1, %r1",  "__builtin_copysignf(a, b)" },

    { "cmp_oeq",   insn_cmp_oeq_f32,   "bool", "f32", "%r1, %r1, %r2",       "a == b" },
    { "cmp_one",   insn_cmp_one_f32,   "bool", "f32", "%r1, %r1, %r2",       "a != b" },
    { "cmp_olt",   insn_cmp_olt_f32,   "bool", "f32", "%r1, %r1, %r2",       "a < b" },
    { "cmp_ole",   insn_cmp_oge_f32,   "bool", "f32", "%r1, %r2, %r1",       "a <= b" }, // swap
    { "cmp_ogt",   insn_cmp_olt_f32,   "bool", "f32", "%r1, %r2, %r1",       "a > b" }, // swap
    { "cmp_oge",   insn_cmp_oge_f32,   "bool", "f32", "%r1, %r1, %r2",       "a >= b" },

    { "add",       insn_add_f64,        "f64", "f64", "%r1, %r2, %r1",       "a + b" },
    { "sub",       insn_sub_f64,        "f64", "f64", "%r1, %r1, %r2",       "a - b" },
    { "mul",       insn_mul_f64,        "f64", "f64", "%r1, %r2, %r1",       "a * b" },
    { "div",       insn_div_f64,        "f64", "f64", "%r1, %r1, %r2",       "a / b" },
    { "abs_diff",  insn_abs_diff_f64,   "f64", "f64", "%r1, %r1, %r2",       "__builtin_fabs(a - b)" },
    { "nabs_diff", insn_nabs_diff_f64,  "f64", "f64", "%r1, %r1, %r2",       "- __builtin_fabs(a - b)" },
    { "copysign",  insn_merge_f64,      "f64", "f64", "%r1, %r2, %r1, %r1",  "__builtin_copysign(a, b)" },

    { "cmp_oeq",   insn_cmp_oeq_f64,   "bool", "f64", "%r1, %r1, %r2",       "a == b" },
    { "cmp_one",   insn_cmp_one_f64,   "bool", "f64", "%r1, %r1, %r2",       "a != b" },
    { "cmp_olt",   insn_cmp_olt_f64,   "bool", "f64", "%r1, %r1, %r2",       "a < b" },
    { "cmp_ole",   insn_cmp_oge_f64,   "bool", "f64", "%r1, %r2, %r1",       "a <= b" }, // swap
    { "cmp_ogt",   insn_cmp_olt_f64,   "bool", "f64", "%r1, %r2, %r1",       "a > b" }, // swap
    { "cmp_oge",   insn_cmp_oge_f64,   "bool", "f64", "%r1, %r1, %r2",       "a >= b" },

    { "add",       insn_add_f128,       "f128", "f128", "%r1, %r2, %r1",       "a + b" },
    { "sub",       insn_sub_f128,       "f128", "f128", "%r1, %r1, %r2",       "a - b" },
    { "mul",       insn_mul_f128,       "f128", "f128", "%r1, %r2, %r1",       "a * b" },
    { "div",       insn_div_f128,       "f128", "f128", "%r1, %r1, %r2",       "a / b" },
    { "abs_diff",  insn_abs_diff_f128,  "f128", "f128", "%r1, %r1, %r2",       "__builtin_fabsl(a - b)" },
    { "nabs_diff", insn_nabs_diff_f128, "f128", "f128", "%r1, %r1, %r2",       "- __builtin_fabsl(a - b)" },
    { "copysign",  insn_merge_f128,     "f128", "f128", "%r1, %r2, %r1, %r1",  "__builtin_copysignl(a, b)" },

    { "cmp_oeq",   insn_cmp_oeq_f128,   "bool", "f128", "%r1, %r1, %r2",       "a == b" },
    { "cmp_one",   insn_cmp_one_f128,   "bool", "f128", "%r1, %r1, %r2",       "a != b" },
    { "cmp_olt",   insn_cmp_olt_f128,   "bool", "f128", "%r1, %r1, %r2",       "a < b" },
    { "cmp_ole",   insn_cmp_oge_f128,   "bool", "f128", "%r1, %r2, %r1",       "a <= b" }, // swap
    { "cmp_ogt",   insn_cmp_olt_f128,   "bool", "f128", "%r1, %r2, %r1",       "a > b" }, // swap
    { "cmp_oge",   insn_cmp_oge_f128,   "bool", "f128", "%r1, %r1, %r2",       "a >= b" },

    { "add",       insn_add_f16,        "f16", "f16", "%r1, %r2, %r1", "a + b" },
    { "sub",       insn_sub_f16,        "f16", "f16", "%r1, %r1, %r2", "a - b" },
    { "mul",       insn_mul_f16,        "f16", "f16", "%r1, %r2, %r1", "a * b" },
    { "div",       insn_div_f16,        "f16", "f16", "%r1, %r1, %r2", "a / b" },

    { "cmp_oeq",   insn_cmp_oeq_f16,   "bool", "f16", "%r1, %r1, %r2",       "a == b" },
    { "cmp_one",   insn_cmp_one_f16,   "bool", "f16", "%r1, %r1, %r2",       "a != b" },
    { "cmp_olt",   insn_cmp_olt_f16,   "bool", "f16", "%r1, %r1, %r2",       "a < b" },
    { "cmp_ole",   insn_cmp_oge_f16,   "bool", "f16", "%r1, %r2, %r1",       "a <= b" }, // swap
    { "cmp_ogt",   insn_cmp_olt_f16,   "bool", "f16", "%r1, %r2, %r1",       "a > b" }, // swap
    { "cmp_oge",   insn_cmp_oge_f16,   "bool", "f16", "%r1, %r1, %r2",       "a >= b" },
};

CODEGEN_HANDLER(FpBinary)
{
    for (const auto& func : binary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.src_type << "_" << func.dst_type;
        out << "// CHECK-LABEL: @" << fname.str() << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " a, " << func.src_type << " b)" << std::endl;
        out << "{" << std::endl;

        out << "// CHECK: " << asm_name[func.insn] << " " << func.args << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }
}

static const func_info ternary_functions[] = {
    { "fma",            insn_madd_f16,        "f16", "f16", "%r1, %r1, %r2, %r3",       "__builtin_fmaf16(a, b, c)" },
    { "fma_nega",       insn_nmadd_f16,       "f16", "f16", "%r1, %r1, %r2, %r3",       "__builtin_fmaf16(-a, b, c)" },
    { "fma_negc",       insn_msub_f16,        "f16", "f16", "%r1, %r1, %r2, %r3",       "__builtin_fmaf16(a, b, -c)" },
    { "fma_nega_negc",  insn_nmsub_f16,       "f16", "f16", "%r1, %r1, %r2, %r3",       "__builtin_fmaf16(-a, b, -c)" },

    { "fma",            insn_madd_f32,        "f32", "f32", "%r1, %r1, %r2, %r3",       "__builtin_fmaf(a, b, c)" },
    { "fma_nega",       insn_nmadd_f32,       "f32", "f32", "%r1, %r1, %r2, %r3",       "__builtin_fmaf(-a, b, c)" },
    { "fma_negc",       insn_msub_f32,        "f32", "f32", "%r1, %r1, %r2, %r3",       "__builtin_fmaf(a, b, -c)" },
    { "fma_nega_negc",  insn_nmsub_f32,       "f32", "f32", "%r1, %r1, %r2, %r3",       "__builtin_fmaf(-a, b, -c)" },

    { "fma",            insn_madd_f64,        "f64", "f64", "%r1, %r1, %r2, %r3",       "__builtin_fma(a, b, c)" },
    { "fma_nega",       insn_nmadd_f64,       "f64", "f64", "%r1, %r1, %r2, %r3",       "__builtin_fma(-a, b, c)" },
    { "fma_negc",       insn_msub_f64,        "f64", "f64", "%r1, %r1, %r2, %r3",       "__builtin_fma(a, b, -c)" },
    { "fma_nega_negc",  insn_nmsub_f64,       "f64", "f64", "%r1, %r1, %r2, %r3",       "__builtin_fma(-a, b, -c)" },

    { "fma",            insn_madd_f128,       "f128", "f128", "%r1, %r1, %r2, %r3",     "__builtin_fmal(a, b, c)" },
    { "fma_nega",       insn_nmadd_f128,      "f128", "f128", "%r1, %r1, %r2, %r3",     "__builtin_fmal(-a, b, c)" },
    { "fma_negc",       insn_msub_f128,       "f128", "f128", "%r1, %r1, %r2, %r3",     "__builtin_fmal(a, b, -c)" },
    { "fma_nega_negc",  insn_nmsub_f128,      "f128", "f128", "%r1, %r1, %r2, %r3",     "__builtin_fmal(-a, b, -c)" },

    { "fma",            insn_madd_vf16,       "v8f16", "v8f16", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf16(a, b, c)" },
    { "fma_nega",       insn_nmadd_vf16,      "v8f16", "v8f16", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf16(-a, b, c)" },
    { "fma_negc",       insn_msub_vf16,       "v8f16", "v8f16", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf16(a, b, -c)" },
    { "fma_nega_negc",  insn_nmsub_vf16,      "v8f16", "v8f16", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf16(-a, b, -c)" },
    { "fmadd_alt",      insn_madd_alt_vf16,   "v8f16", "v8f16", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_alt_vf16(a, b, c)" },

    { "fma",            insn_madd_vf32,       "v4f32", "v4f32", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf32(a, b, c)" },
    { "fma_nega",       insn_nmadd_vf32,      "v4f32", "v4f32", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf32(-a, b, c)" },
    { "fma_negc",       insn_msub_vf32,       "v4f32", "v4f32", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf32(a, b, -c)" },
    { "fma_nega_negc",  insn_nmsub_vf32,      "v4f32", "v4f32", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf32(-a, b, -c)" },
    { "fmadd_alt",      insn_madd_alt_vf32,   "v4f32", "v4f32", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_alt_vf32(a, b, c)" },

    { "fma",            insn_madd_vf64,       "v2f64", "v2f64", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf64(a, b, c)" },
    { "fma_nega",       insn_nmadd_vf64,      "v2f64", "v2f64", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf64(-a, b, c)" },
    { "fma_negc",       insn_msub_vf64,       "v2f64", "v2f64", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf64(a, b, -c)" },
    { "fma_nega_negc",  insn_nmsub_vf64,      "v2f64", "v2f64", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_vf64(-a, b, -c)" },
    { "fmadd_alt",      insn_madd_alt_vf64,   "v2f64", "v2f64", "%r1, %r1, %r2, %r3",   "__builtin_postrisc_madd_alt_vf64(a, b, c)" },
};

CODEGEN_HANDLER(FpTernary)
{
    out << R"ABRACADABRA(

#if !__has_builtin(__builtin_fmaf16)
  #error "there is no __builtin_fmaf16"
#endif

#if __has_constexpr_builtin(__builtin_fmaf16)
  #warning "has constexpr __builtin_fmaf16"
#else
  #warning "has NO constexpr __builtin_fmaf16"
#endif

)ABRACADABRA";

    for (const auto& func : ternary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.src_type << "_" << func.dst_type;
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
