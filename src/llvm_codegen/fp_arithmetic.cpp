#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(FpToInt)
{
    out << '\n';

    for (const char * ftype : {"f16", "f32", "f64", "f128"}) {
        out << '\n';
        out << "//===----------------------------------------------------------------------===//\n";
        out << "// " << ftype << '\n';
        out << "//===----------------------------------------------------------------------===//\n\n";

        for (const char * itype : {"i32", "u32", "i64", "u64", "i128", "u128"}) {
            {
            std::ostringstream fname;
            fname << "trunc_" << ftype << "_" << itype;

            out << CHECK_LABEL << fname.str() << '\n';
            out << itype << " " << fname.str() << "(" << ftype << " value)\n";
            out << "{\n";

            out << CHECK << "cvt_" << ftype << "_" << itype << " %r1, %r1, rtz\n";
            out << "    return (" << itype << ")value;\n";
            out << "}\n\n";
            }

            {
            std::ostringstream fname;
            fname << "convert_" << itype << "_" << ftype;

            out << CHECK_LABEL << fname.str() << '\n';
            out << CHECK << "cvt_" << itype << "_" << ftype << " %r1, %r1\n";

            out << ftype << " " << fname.str() << "(" << itype << " value)\n";
            out << "{\n";
            out << "    return (" << ftype << ")value;\n";
            out << "}\n\n";
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

#define FP_UNARY_FUNCS(FTYPE, UTYPE, LAST_BIT, SUFFIX) \
    { "neg",           insn_neg_       ## FTYPE, #FTYPE, #FTYPE, R2, "(-x)" }, \
    { "abs",           insn_abs_       ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_fabs"      SUFFIX "(x)" }, \
    { "neg_abs",       insn_nabs_      ## FTYPE, #FTYPE, #FTYPE, R2, "- __builtin_fabs"    SUFFIX "(x)" }, \
    { "sqrt",          insn_sqrt_      ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_sqrt"      SUFFIX "(x)" }, \
    { "roundeven",     insn_round_     ## FTYPE, #FTYPE, #FTYPE, R2 ", rne",   "__builtin_roundeven" SUFFIX "(x)" }, \
    { "round",         insn_round_     ## FTYPE, #FTYPE, #FTYPE, R2 ", rmm",   "__builtin_round"     SUFFIX "(x)" }, \
    { "floor",         insn_round_     ## FTYPE, #FTYPE, #FTYPE, R2 ", rdn",   "__builtin_floor"     SUFFIX "(x)" }, \
    { "ceil",          insn_round_     ## FTYPE, #FTYPE, #FTYPE, R2 ", rup",   "__builtin_ceil"      SUFFIX "(x)" }, \
    { "trunc",         insn_round_     ## FTYPE, #FTYPE, #FTYPE, R2 ", rtz",   "__builtin_trunc"     SUFFIX "(x)" }, \
    { "nearbyint",     insn_round_     ## FTYPE, #FTYPE, #FTYPE, R2 ", dyn",   "__builtin_nearbyint" SUFFIX "(x)" }, \
    { "rint",          insn_roundnx_   ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_rint"      SUFFIX "(x)" }, \
    { "builtin_rsqrt", insn_rsqrt_     ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_postrisc_rsqrt_" #FTYPE "(x)" }, \
    { "cvt",           insn_cvt_i32_   ## FTYPE, #FTYPE, "i32",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_i64_   ## FTYPE, #FTYPE, "i64",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_i128_  ## FTYPE, #FTYPE, "i128", R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_u32_   ## FTYPE, #FTYPE, "u32",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_u64_   ## FTYPE, #FTYPE, "u64",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_u128_  ## FTYPE, #FTYPE, "u128", R2, "(" #FTYPE ")(x)" }, \
    { "trunc",         insn_cvt_ ## FTYPE ## _i32,  "i32",  #FTYPE, R2 ", rtz", "(i32)(x)" }, \
    { "trunc",         insn_cvt_ ## FTYPE ## _i64,  "i64",  #FTYPE, R2 ", rtz", "(i64)(x)" }, \
    { "trunc",         insn_cvt_ ## FTYPE ## _i128, "i128", #FTYPE, R2 ", rtz", "(i128)(x)" }, \
    { "trunc",         insn_cvt_ ## FTYPE ## _u32,  "u32",  #FTYPE, R2 ", rtz", "(u32)(x)" }, \
    { "trunc",         insn_cvt_ ## FTYPE ## _u64,  "u64",  #FTYPE, R2 ", rtz", "(u64)(x)" }, \
    { "trunc",         insn_cvt_ ## FTYPE ## _u128, "u128", #FTYPE, R2 ", rtz", "(u128)(x)" }, \
    { "lrint",         insn_cvt_ ## FTYPE ## _i64,  "i64",  #FTYPE, R2 ", dyn", "__builtin_lrint" SUFFIX "(x)" }, \
    { "llrint",        insn_cvt_ ## FTYPE ## _i64,  "i64",  #FTYPE, R2 ", dyn", "__builtin_llrint" SUFFIX "(x)" }, \
    { "lround",        insn_cvt_ ## FTYPE ## _i64,  "i64",  #FTYPE, R2 ", rmm", "__builtin_lround" SUFFIX "(x)" }, \
    { "llround",       insn_cvt_ ## FTYPE ## _i64,  "i64",  #FTYPE, R2 ", rmm", "__builtin_llround" SUFFIX "(x)" }, \

//    { "irint",         insn_cvt_ ## FTYPE ## _i32,  "i32",  #FTYPE, R2 ", dyn", "__builtin_irint" SUFFIX "(x)" },

// not one-instruction for f16
#define FP_UNARY_FUNCS_NON_F16(FTYPE, UTYPE, LAST_BIT, SUFFIX) \
    { "signbit",       insn_srl_imm_   ## UTYPE, "bool", #FTYPE, "%r1, %r1, " #LAST_BIT, "__builtin_signbit"   SUFFIX "(x)" }, \

#define FP_UNARY_VECTOR_FUNCS(FTYPE, ITYPE, UTYPE, FCTYPE, ICTYPE, UCTYPE) \
    { "builtin_neg",         insn_neg_         ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "(-x)" }, \
    { "vector_abs",          insn_abs_         ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "__builtin_elementwise_abs(x)" }, \
    { "builtin_nabs",        insn_nabs_        ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "- __builtin_elementwise_abs(x)" }, \
    { "builtin_sqrt",        insn_sqrt_        ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "__builtin_elementwise_sqrt(x)" }, \
    { "builtin_rsqrt",       insn_rsqrt_       ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "__builtin_postrisc_rsqrt_"       #FTYPE "(x)" }, \
    { "vector_roundeven",    insn_round_       ## FTYPE, #FCTYPE, #FCTYPE, R2 ", rne",  "__builtin_elementwise_roundeven(x)" }, \
    { "vector_round",        insn_round_       ## FTYPE, #FCTYPE, #FCTYPE, R2 ", rmm",  "__builtin_elementwise_round(x)" }, \
    { "vector_floor",        insn_round_       ## FTYPE, #FCTYPE, #FCTYPE, R2 ", rdn",  "__builtin_elementwise_floor(x)" }, \
    { "vector_ceil",         insn_round_       ## FTYPE, #FCTYPE, #FCTYPE, R2 ", rup",  "__builtin_elementwise_ceil(x)" }, \
    { "vector_trunc",        insn_round_       ## FTYPE, #FCTYPE, #FCTYPE, R2 ", rtz",  "__builtin_elementwise_trunc(x)" }, \
    { "vector_nearbyint",    insn_round_       ## FTYPE, #FCTYPE, #FCTYPE, R2 ", dyn",  "__builtin_elementwise_nearbyint(x)" }, \
    { "vector_rint",         insn_roundnx_     ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "__builtin_elementwise_rint(x)" }, \
    { "builtin_unpack_low",  insn_unpack_low_  ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "__builtin_postrisc_unpack_low_"  #FTYPE "(x)" }, \
    { "builtin_unpack_high", insn_unpack_high_ ## FTYPE, #FCTYPE, #FCTYPE, R2        ,  "__builtin_postrisc_unpack_high_" #FTYPE "(x)" }, \
    { "builtin_trunc",       insn_cvt_ ## FTYPE ## _ ## ITYPE, #FCTYPE, #ICTYPE, R2 ", rtz", "__builtin_postrisc_trunc_" #FTYPE "_" #ITYPE "(x)" }, \
    { "builtin_trunc",       insn_cvt_ ## FTYPE ## _ ## UTYPE, #FCTYPE, #UCTYPE, R2 ", rtz", "__builtin_postrisc_trunc_" #FTYPE "_" #UTYPE "(x)" }, \
    { "builtin_cvt",         insn_cvt_ ## ITYPE ## _ ## FTYPE, #ICTYPE, #FCTYPE, R2 ", rtz", "__builtin_postrisc_cvt_"   #ITYPE "_" #FTYPE "(x)" }, \
    { "builtin_cvt",         insn_cvt_ ## UTYPE ## _ ## FTYPE, #UCTYPE, #FCTYPE, R2 ", rtz", "__builtin_postrisc_cvt_"   #UTYPE "_" #FTYPE "(x)" }, \

#define R2 "%r1, %r1"

static const unary_info unary_functions[] = {
    FP_UNARY_FUNCS(f32,  u32,  31,  "f")
    FP_UNARY_FUNCS(f64,  u64,  63,  "")
    FP_UNARY_FUNCS(f128, u128, 127, "l")
    FP_UNARY_FUNCS(f16,  u32,  15,  "f16")
    FP_UNARY_FUNCS_NON_F16(f32,  u32,  31,  "f")
    FP_UNARY_FUNCS_NON_F16(f64,  u64,  63,  "")
    FP_UNARY_FUNCS_NON_F16(f128, u128, 127, "l")
    FP_UNARY_VECTOR_FUNCS(vf16, vi16, vu16, v8f16, v8i16, v8u16)
    FP_UNARY_VECTOR_FUNCS(vf32, vi32, vu32, v4f32, v4i32, v4u32)
    FP_UNARY_VECTOR_FUNCS(vf64, vi64, vu64, v2f64, v2i64, v2u64)
    // { "", insn_class_f64,      "bool", "f64", "__builtin_isnan(x)" },

    { "extend", insn_extend_f16_f32,  "f32",  "f16",  R2, "(x)" },
    { "extend", insn_extend_f16_f64,  "f64",  "f16",  R2, "(x)" },
    { "extend", insn_extend_f16_f128, "f128", "f16",  R2, "(x)" },
    { "extend", insn_extend_f32_f64,  "f64",  "f32",  R2, "(x)" },
    { "extend", insn_extend_f32_f128, "f128", "f32",  R2, "(x)" },
    { "extend", insn_extend_f64_f128, "f128", "f64",  R2, "(x)" },
    { "narrow", insn_cvt_f32_f16,     "f16",  "f32",  R2, "(x)" },
    { "narrow", insn_cvt_f64_f16,     "f16",  "f64",  R2, "(x)" },
    { "narrow", insn_cvt_f128_f16,    "f16",  "f128", R2, "(x)" },
    { "narrow", insn_cvt_f64_f32,     "f32",  "f64",  R2, "(x)" },
    { "narrow", insn_cvt_f128_f32,    "f32",  "f128", R2, "(x)" },
    { "narrow", insn_cvt_f128_f64,    "f64",  "f128", R2, "(x)" },
};

CODEGEN_HANDLER(FpUnary)
{
    for (const auto& func : unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.src_type << "_" << func.dst_type;

        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_name(insn_alloc) << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_name(insn_retf) << " 0\n";

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

#define FP_BINARY_FUNCS(TYPE, SUFFIX) \
    { "add",               insn_add_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "a + b"     }, \
    { "nadd",              insn_nadd_      ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "-(a + b)"  }, \
    { "sub",               insn_sub_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a - b"     }, \
    { "mul",               insn_mul_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "a * b"     }, \
    { "nmul",              insn_nmul_      ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "- (a * b)" }, \
    { "div",               insn_div_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a / b"     }, \
    { "abs_diff",          insn_abs_diff_  ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fabs"   SUFFIX "(a - b)" }, \
    { "nabs_diff",         insn_nabs_diff_ ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "- __builtin_fabs" SUFFIX "(a - b)" }, \
    { "copysign",          insn_merge_     ## TYPE, #TYPE,  #TYPE, #TYPE, "%r1, %r2, %r1, %r1",  "__builtin_copysign" SUFFIX "(a, b)" }, \
    { "cmp_oeq",           insn_cmp_oeq_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "a == b" }, \
    { "cmp_one",           insn_cmp_one_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "a != b" }, \
    { "cmp_olt",           insn_cmp_olt_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "a < b"  }, \
    { "cmp_ole",           insn_cmp_oge_   ## TYPE, "bool", #TYPE, #TYPE, R3_SWAP,  "a <= b" }, \
    { "cmp_ogt",           insn_cmp_olt_   ## TYPE, "bool", #TYPE, #TYPE, R3_SWAP,  "a > b"  }, \
    { "cmp_oge",           insn_cmp_oge_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "a >= b" }, \
    { "min_lt_abab",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a <  b ? a : b" }, \
    { "max_ge_abab",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a >= b ? a : b" }, \
    { "min_le_abab",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a <= b ? a : b" }, \
    { "max_gt_abab",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a >  b ? a : b" }, \
    { "max_lt_baab",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b <  a ? a : b" }, \
    { "min_ge_baab",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b >= a ? a : b" }, \
    { "max_le_baab",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b <= a ? a : b" }, \
    { "min_gt_baab",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b >  a ? a : b" }, \
    { "max_lt_abba",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a <  b ? b : a" }, \
    { "min_ge_abba",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a >= b ? b : a" }, \
    { "max_le_abba",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a <= b ? b : a" }, \
    { "min_gt_abba",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "a >  b ? b : a" }, \
    { "min_lt_baba",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b <  a ? b : a" }, \
    { "max_ge_baba",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b >= a ? b : a" }, \
    { "min_le_baba",       insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b <= a ? b : a" }, \
    { "max_gt_baba",       insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3_SWAP,  "b >  a ? b : a" }, \
    { "builtin_minimum",   insn_min_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_minimum_" #TYPE "(a, b)" }, \
    { "builtin_maximum",   insn_max_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_maximum_" #TYPE "(a, b)" }, \
    { "min",               insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fmin" SUFFIX "(a, b)" }, \
    { "max",               insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fmax" SUFFIX "(a, b)" }, \
    { "fminimum_num",      insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fminimum_num" SUFFIX "(a, b)" }, \
    { "fmaximum_num",      insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fmaximum_num" SUFFIX "(a, b)" }, \
    { "builtin_abs_min",   insn_abs_min_   ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fmin" SUFFIX "(__builtin_fabs" SUFFIX "(a), __builtin_fabs" SUFFIX "(b))" }, \
    { "builtin_abs_max",   insn_abs_max_   ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fmax" SUFFIX "(__builtin_fabs" SUFFIX "(a), __builtin_fabs" SUFFIX "(b))" }, \

#define FP_BINARY_VECTOR_FUNCS(TYPE, CTYPE, ITYPE) \
    { "add",                 insn_add_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "a + b"    }, \
    { "nadd",                insn_nadd_       ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "-(a + b)" }, \
    { "sub",                 insn_sub_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "a - b"    }, \
    { "mul",                 insn_mul_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "a * b"    }, \
    { "nmul",                insn_nmul_       ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "-(a * b)" }, \
    { "div",                 insn_div_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "a / b"    }, \
    { "abs_diff",            insn_abs_diff_   ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_abs(a - b)" }, \
    { "nabs_diff",           insn_nabs_diff_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "- __builtin_elementwise_abs(a - b)" }, \
    { "vector_abs_max",      insn_abs_max_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_maxnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" }, \
    { "vector_abs_min",      insn_abs_min_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_minnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" }, \
    { "vector_copysign",     insn_merge_      ## TYPE, #CTYPE, #CTYPE, #CTYPE, "%r1, %r2, %r1, %r1",  "__builtin_elementwise_copysign(a, b)" }, \
    { "vector_minimum",      insn_min_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_minimum(a, b)" }, \
    { "vector_maximum",      insn_max_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_maximum(a, b)" }, \
    { "vector_max",          insn_maxnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_max(a, b)" }, \
    { "vector_min",          insn_minnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_min(a, b)" }, \
    { "vector_minnum",       insn_minnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_minnum(a, b)" }, \
    { "vector_maxnum",       insn_maxnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_maxnum(a, b)" }, \
    { "vector_minimumnum",   insn_minnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_minimumnum(a, b)" }, \
    { "vector_maximumnum",   insn_maxnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_elementwise_maximumnum(a, b)" }, \
    { "builtin_dot",         insn_dot_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_dot_" #TYPE "(a, b)" }, \
    { "builtin_add_alt",     insn_add_alt_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_add_alt_"    #TYPE "(a, b)" }, \
    { "builtin_sub_alt",     insn_sub_alt_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_sub_alt_"    #TYPE "(a, b)" }, \
    { "builtin_add_horiz",   insn_add_horiz_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_add_horiz_"  #TYPE "(a, b)" }, \
    { "builtin_sub_horiz",   insn_sub_horiz_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_sub_horiz_"  #TYPE "(a, b)" }, \
    { "builtin_mul_horiz",   insn_mul_horiz_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_mul_horiz_"  #TYPE "(a, b)" }, \
    { "builtin_cmp_oeq",     insn_cmp_oeq_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_oeq_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_one",     insn_cmp_one_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_one_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_olt",     insn_cmp_olt_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_olt_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_oge",     insn_cmp_oge_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_oge_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_o",       insn_cmp_o_      ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_o_"      #TYPE "(a, b)" }, \
    { "builtin_cmp_ueq",     insn_cmp_ueq_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_ueq_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_une",     insn_cmp_une_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_une_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_ult",     insn_cmp_ult_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_ult_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_uge",     insn_cmp_uge_    ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_uge_"    #TYPE "(a, b)" }, \
    { "builtin_cmp_u",       insn_cmp_u_      ## TYPE, #ITYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_cmp_u_"      #TYPE "(a, b)" }, \
    { "builtin_merge_low",   insn_merge_low_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_merge_low_"  #TYPE "(a, b)" }, \
    { "builtin_merge_high",  insn_merge_high_ ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_merge_high_" #TYPE "(a, b)" }, \


#define R3      "%r1, %r1, %r2"
#define R3_SWAP "%r1, %r2, %r1"

static const binary_info binary_functions[] = {
    FP_BINARY_FUNCS(f32,  "f")
    FP_BINARY_FUNCS(f64,  "")
    FP_BINARY_FUNCS(f128, "l")
    FP_BINARY_FUNCS(f16,  "f16")
    FP_BINARY_VECTOR_FUNCS(vf16, v8f16, v8i16)
    FP_BINARY_VECTOR_FUNCS(vf32, v4f32, v4i32)
    FP_BINARY_VECTOR_FUNCS(vf64, v2f64, v2i64)
};

CODEGEN_HANDLER(FpBinary)
{
    for (const auto& func : binary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.dst_type << "_" << func.src1_type << "_" << func.src2_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_name(insn_alloc) << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_name(insn_retf) << " 0\n";

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

#define FP_TERNARY_FUNCS(TYPE, SUFFIX) \
    { "fma",            insn_madd_   ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(a, b, c)" }, \
    { "fma_nega",       insn_nmadd_  ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(-a, b, c)" }, \
    { "fma_negc",       insn_msub_   ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(a, b, -c)" }, \
    { "fma_nega_negc",  insn_nmsub_  ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(-a, b, -c)" }, \
    { "merge",          insn_merge_  ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_postrisc_merge_" #TYPE "(a, b, c)" }, \

#define FP_TERNARY_VECTOR_FUNCS(TYPE, CTYPE) \
    { "vector_madd",   insn_madd_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_elementwise_fma(a, b, c)" }, \
    { "vector_msub",   insn_msub_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_elementwise_fma(a, b, -c)" }, \
    { "vector_nmadd",  insn_nmadd_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_elementwise_fma(-a, b, c)" }, \
    { "vector_nmsub",  insn_nmsub_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_elementwise_fma(-a, b, -c)" }, \
    { "builtin_postrisc_fmadd_alt", insn_madd_alt_ ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_madd_alt_" #TYPE "(a, b, c)" }, \
    { "builtin_postrisc_fmsub_alt", insn_msub_alt_ ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_msub_alt_" #TYPE "(a, b, c)" }, \
    { "builtin_postrisc_merge",     insn_merge_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_merge_"    #TYPE "(a, b, c)" }, \

#define R4 "%r1, %r1, %r2, %r3"

static const ternary_info ternary_functions[] = {
    FP_TERNARY_FUNCS(f16,  "f16")
    FP_TERNARY_FUNCS(f32,  "f")
    FP_TERNARY_FUNCS(f64,  "")
    FP_TERNARY_FUNCS(f128, "l")

    FP_TERNARY_VECTOR_FUNCS(vf16, v8f16)
    FP_TERNARY_VECTOR_FUNCS(vf32, v4f32)
    FP_TERNARY_VECTOR_FUNCS(vf64, v2f64)

    { "perm_vu8",  insn_perm, "v16u8", "v16u8", "v16u8", "v16u8", R4, "__builtin_postrisc_perm_vu8(a, b, c)" },
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
        fname << "test_" << func.name << "_" << func.src1_type << "_" << func.dst_type;

        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_name(insn_alloc) << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_name(insn_retf) << " 0\n";

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b,"
                   << func.src3_type << " c)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

#define FP_BINARY_STRICT_FUNCS(TYPE, SUFFIX) \
    { "add",               insn_add_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,  "a + b"     }, \
    { "nadd",              insn_nadd_      ## TYPE, #TYPE,  #TYPE, #TYPE, R3,  "-(a + b)"  }, \
    { "sub",               insn_sub_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,  "a - b"     }, \
    { "mul",               insn_mul_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,  "a * b"     }, \
    { "nmul",              insn_nmul_      ## TYPE, #TYPE,  #TYPE, #TYPE, R3,  "- (a * b)" }, \
    { "div",               insn_div_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,  "a / b"     }, \
    { "cmp_oeq",           insn_cmp_oeq_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "a == b" }, \
    { "cmp_one",           insn_cmp_one_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" },/*FIXME: ONE===>UNE*/ \
    { "cmp_olt",           insn_cmp_olt_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "a < b"  }, \
    { "cmp_ole",           insn_cmp_oge_   ## TYPE, "bool", #TYPE, #TYPE, R3_SWAP,  "a <= b" }, \
    { "cmp_ogt",           insn_cmp_olt_   ## TYPE, "bool", #TYPE, #TYPE, R3_SWAP,  "a > b"  }, \
    { "cmp_oge",           insn_cmp_oge_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "a >= b" }, \
    { "cmp_ueq",           insn_cmp_ueq_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "!(a < b || a > b)" }, /* trick to force UEQ */ \
    { "cmp_une",           insn_cmp_une_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "!(a == b)" }, \
    { "cmp_ult",           insn_cmp_ult_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "!(a >= b)"  }, \
    { "cmp_ule",           insn_cmp_uge_   ## TYPE, "bool", #TYPE, #TYPE, R3_SWAP,  "!(a > b)" }, \
    { "cmp_ugt",           insn_cmp_ult_   ## TYPE, "bool", #TYPE, #TYPE, R3_SWAP,  "!(a <= b)"  }, \
    { "cmp_uge",           insn_cmp_uge_   ## TYPE, "bool", #TYPE, #TYPE, R3,       "!(a < b)" }, \
    { "cmp_u",             insn_cmp_u_     ## TYPE, "bool", #TYPE, #TYPE, R3,       "__builtin_isnan(a) || __builtin_isnan(b)" }, \
    { "cmp_o",             insn_cmp_o_     ## TYPE, "bool", #TYPE, #TYPE, R3,       "!__builtin_isnan(a) && !__builtin_isnan(b)" }, \

static const binary_info binary_strict_functions[] = {
    FP_BINARY_STRICT_FUNCS(f32,  "f")
    FP_BINARY_STRICT_FUNCS(f64,  "")
    FP_BINARY_STRICT_FUNCS(f128, "l")
    FP_BINARY_STRICT_FUNCS(f16,  "f16")
};

CODEGEN_HANDLER(FpStrict)
{
    for (const auto& func : binary_strict_functions) {
        std::ostringstream fname;
        fname << "test_strict_" << func.name << "_" << func.dst_type << "_" << func.src1_type << "_" << func.src2_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_name(insn_alloc) << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_name(insn_retf) << " 0\n";

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

/*
FClassMask:
  SignalingNaN      = 0x001,
  QuietNaN          = 0x002,
  NegativeInfinity  = 0x004,
  NegativeNormal    = 0x008,
  NegativeSubnormal = 0x010,
  NegativeZero      = 0x020,
  PositiveInfinity  = 0x040,
  PositiveNormal    = 0x080,
  PositiveSubnormal = 0x100,
  PositiveZero      = 0x200,
*/

void Generator::FpClassify(std::ostream& out, EInstructionID insn, const char *type)
{
    // FIXME: clang doesn't use class_fXX instruction, but switches to [load_imm+]cmp_YYY_fXX in these cases:
    static const u32 excludes[] = {
        3, // 0000000011, unordered
        4, // 0000000100, ordered and  == -INF
        7, // 0000000111, unordered or == -INF
       28, // 0000011100, ordered and  <  0.0
       31, // 0000011111, unordered or <  0.0
       96, // 0001100000, ordered and  == 0.0
       99, // 0001100011, unordered or == 0.0
      124, // 0001111100, ordered and  <= 0.0
      127, // 0001111111, unordered or <= 0.0
      508, // 0111111100, ordered and  <  +INF
      511, // 0111111111, unordered or <  +INF
      512, // 1000000000, ordered and  == +INF
      515, // 1000000011, unordered or == +INF
      896, // 1110000000, ordered and  >  0.0
      899, // 1110000011, unordered or >  0.0
      924, // 1110011100, ordered and  != 0.0
      927, // 1110011111, unordered or != 0.0
      992, // 1111100000, ordered and  >= 0.0
      995, // 1111100011, unordered or >= 0.0
     1016, // 1111111000, ordered and  > -INF
     1019, // 1111111011, unordered or > -INF
     1020, // 1111111100, ordered
    };

    out << "// excluded classify masks:\n";
    for (const u32 x : excludes) {
         out << "// " << std::bitset<10>(x) << ' ' << x << '\n';
    }
    out << '\n';

    static const struct {
        char       func[32];
        FClassMask mask;
    } tests[] = {
         { "__builtin_isinf",    FClassMask::NegativeInfinity | FClassMask::PositiveInfinity },
         { "__builtin_isnormal", FClassMask::NegativeNormal | FClassMask::PositiveNormal  },
         // { "__builtin_isnan",    FClassMask::SignalingNaN | FClassMask::QuietNaN  },
         { "__builtin_isfinite", FClassMask::NegativeNormal | FClassMask::NegativeSubnormal | FClassMask::NegativeZero |
                                 FClassMask::PositiveNormal | FClassMask::PositiveSubnormal | FClassMask::PositiveZero },
    };

    for (const auto& test : tests) {
        std::ostringstream fname;
        fname << "test_classify_" << type << "_" << test.func;
        out << "// mask "<< static_cast<u32>(test.mask) << " :" << test.mask << '\n';
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_name(insn_alloc) << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn) << " %r1, %r1, " << static_cast<u32>(test.mask) << '\n';
        out << CHECK_NEXT << insn_name(insn_retf) << " 0\n";
        out << "bool " << fname.str() << "(" << type << " a)\n";
        out << "{\n";
        out << "    return " << test.func << "(a);\n";
        out << "}\n\n";
    }

    for (size_t mask = 1; mask < (1 << 10)-1; mask++) {
        bool skip = false;
        for (const u32 x : excludes) {
             if (mask == x) { skip = true; break; }
        }
        if (skip) continue;

        std::ostringstream fname;
        fname << "test_builtin_isfpclass_" << type << "_" << mask;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_name(insn_alloc) << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn) << " %r1, %r1, " << mask << '\n';
        out << CHECK_NEXT << insn_name(insn_retf) << " 0\n";

        out << "bool " << fname.str() << "(" << type << " a)\n";
        out << "{\n";
        out << "    return __builtin_isfpclass(a, " << mask << ");\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(FpClassifyF16)  { FpClassify(out, insn_class_f16,  "f16");  }
CODEGEN_HANDLER(FpClassifyF32)  { FpClassify(out, insn_class_f32,  "f32");  }
CODEGEN_HANDLER(FpClassifyF64)  { FpClassify(out, insn_class_f64,  "f64");  }
CODEGEN_HANDLER(FpClassifyF128) { FpClassify(out, insn_class_f128, "f128"); }

} // namespace llvm
} // namespace postrisc
