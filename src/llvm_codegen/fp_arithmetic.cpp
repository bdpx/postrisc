#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(FpToInt)
{
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

            out << CHECK_LABEL << fname.str() << std::endl;
            out << itype << " " << fname.str() << "(" << ftype << " value)" << std::endl;
            out << "{" << std::endl;

            out << CHECK << "trunc_" << ftype << "_" << itype << " %r1, %r1\n";
            out << "    return (" << itype << ")value;" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
            }

            {
            std::ostringstream fname;
            fname << "convert_" << itype << "_" << ftype;

            out << CHECK_LABEL << fname.str() << std::endl;
            out << CHECK << "cvt_" << itype << "_" << ftype << " %r1, %r1\n";

            out << ftype << " " << fname.str() << "(" << itype << " value)" << std::endl;
            out << "{" << std::endl;
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

#define FP_UNARY_FUNCS(FTYPE, UTYPE, LAST_BIT, SUFFIX) \
    { "neg",           insn_neg_       ## FTYPE, #FTYPE, #FTYPE, R2, "(-x)" }, \
    { "abs",           insn_abs_       ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_fabs"      SUFFIX "(x)" }, \
    { "neg_abs",       insn_nabs_      ## FTYPE, #FTYPE, #FTYPE, R2, "- __builtin_fabs"    SUFFIX "(x)" }, \
    { "sqrt",          insn_sqrt_      ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_sqrt"      SUFFIX "(x)" }, \
    { "roundeven",     insn_roundeven_ ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_roundeven" SUFFIX "(x)" }, \
    { "round",         insn_round_     ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_round"     SUFFIX "(x)" }, \
    { "floor",         insn_floor_     ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_floor"     SUFFIX "(x)" }, \
    { "ceil",          insn_ceil_      ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_ceil"      SUFFIX "(x)" }, \
    { "trunc",         insn_trunc_     ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_trunc"     SUFFIX "(x)" }, \
    { "rint",          insn_rint_      ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_rint"      SUFFIX "(x)" }, \
    { "nearbyint",     insn_nearbyint_ ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_nearbyint" SUFFIX "(x)" }, \
    { "builtin_abs",   insn_abs_       ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_postrisc_abs_"   #FTYPE "(x)" }, \
    { "builtin_nabs",  insn_nabs_      ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_postrisc_nabs_"  #FTYPE "(x)" }, \
    { "builtin_neg",   insn_neg_       ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_postrisc_neg_"   #FTYPE "(x)" }, \
    { "builtin_sqrt",  insn_sqrt_      ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_postrisc_sqrt_"  #FTYPE "(x)" }, \
    { "builtin_rsqrt", insn_rsqrt_     ## FTYPE, #FTYPE, #FTYPE, R2, "__builtin_postrisc_rsqrt_" #FTYPE "(x)" }, \
    { "cvt",           insn_cvt_i32_   ## FTYPE, #FTYPE, "i32",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_i64_   ## FTYPE, #FTYPE, "i64",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_i128_  ## FTYPE, #FTYPE, "i128", R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_u32_   ## FTYPE, #FTYPE, "u32",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_u64_   ## FTYPE, #FTYPE, "u64",  R2, "(" #FTYPE ")(x)" }, \
    { "cvt",           insn_cvt_u128_  ## FTYPE, #FTYPE, "u128", R2, "(" #FTYPE ")(x)" }, \
    { "trunc",         insn_trunc_     ## FTYPE ## _i32,  "i32",  #FTYPE, R2, "(i32)(x)" }, \
    { "trunc",         insn_trunc_     ## FTYPE ## _i64,  "i64",  #FTYPE, R2, "(i64)(x)" }, \
    { "trunc",         insn_trunc_     ## FTYPE ## _i128, "i128", #FTYPE, R2, "(i128)(x)" }, \
    { "trunc",         insn_trunc_     ## FTYPE ## _u32,  "u32",  #FTYPE, R2, "(u32)(x)" }, \
    { "trunc",         insn_trunc_     ## FTYPE ## _u64,  "u64",  #FTYPE, R2, "(u64)(x)" }, \
    { "trunc",         insn_trunc_     ## FTYPE ## _u128, "u128", #FTYPE, R2, "(u128)(x)" }, \

// not one-instruction for f16
#define FP_UNARY_FUNCS_NON_F16(FTYPE, UTYPE, LAST_BIT, SUFFIX) \
    { "signbit",       insn_srl_imm_   ## UTYPE, "bool", #FTYPE, "%r1, %r1, " #LAST_BIT, "__builtin_signbit"   SUFFIX "(x)" }, \

#define FP_UNARY_VECTOR_FUNCS(FTYPE, ITYPE, UTYPE, FCTYPE, ICTYPE, UCTYPE) \
    { "builtin_abs",       insn_abs_       ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_abs_"       #FTYPE "(x)" }, \
    { "builtin_nabs",      insn_nabs_      ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_nabs_"      #FTYPE "(x)" }, \
    { "builtin_neg",       insn_neg_       ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_neg_"       #FTYPE "(x)" }, \
    { "builtin_sqrt",      insn_sqrt_      ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_sqrt_"      #FTYPE "(x)" }, \
    { "builtin_rsqrt",     insn_rsqrt_     ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_rsqrt_"     #FTYPE "(x)" }, \
    { "builtin_roundeven", insn_roundeven_ ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_roundeven_" #FTYPE "(x)" }, \
    { "builtin_round",     insn_round_     ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_round_"     #FTYPE "(x)" }, \
    { "builtin_floor",     insn_floor_     ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_floor_"     #FTYPE "(x)" }, \
    { "builtin_ceil",      insn_ceil_      ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_ceil_"      #FTYPE "(x)" }, \
    { "builtin_trunc",     insn_trunc_     ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_trunc_"     #FTYPE "(x)" }, \
    { "builtin_rint",      insn_rint_      ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_rint_"      #FTYPE "(x)" }, \
    { "builtin_nearbyint", insn_nearbyint_ ## FTYPE, #FCTYPE, #FCTYPE, R2, "__builtin_postrisc_nearbyint_" #FTYPE "(x)" }, \
    { "builtin_trunc" #FTYPE "_" #ITYPE, insn_trunc_ ## FTYPE ## _ ## ITYPE, #FCTYPE, #ICTYPE, R2, "__builtin_postrisc_trunc_" #FTYPE "_" #ITYPE "(x)" }, \
    { "builtin_trunc" #FTYPE "_" #UTYPE, insn_trunc_ ## FTYPE ## _ ## UTYPE, #FCTYPE, #UCTYPE, R2, "__builtin_postrisc_trunc_" #FTYPE "_" #UTYPE "(x)" }, \
    { "builtin_cvt"   #ITYPE "_" #FTYPE, insn_cvt_   ## ITYPE ## _ ## FTYPE, #ICTYPE, #FCTYPE, R2, "__builtin_postrisc_cvt_"   #ITYPE "_" #FTYPE "(x)" }, \
    { "builtin_cvt"   #UTYPE "_" #FTYPE, insn_cvt_   ## UTYPE ## _ ## FTYPE, #UCTYPE, #FCTYPE, R2, "__builtin_postrisc_cvt_"   #UTYPE "_" #FTYPE "(x)" }, \

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

        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
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
    { "min",               insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fmin" SUFFIX "(a, b)" }, \
    { "max",               insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_fmax" SUFFIX "(a, b)" }, \
    { "builtin_min",       insn_min_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_min_"     #TYPE "(a, b)" }, \
    { "builtin_max",       insn_max_       ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_max_"     #TYPE "(a, b)" }, \
    { "builtin_abs_min",   insn_abs_min_   ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_abs_min_" #TYPE "(a, b)" }, \
    { "builtin_abs_max",   insn_abs_max_   ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_abs_max_" #TYPE "(a, b)" }, \
    { "builtin_minimum",   insn_minnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_minnum_"  #TYPE "(a, b)" }, \
    { "builtin_maximum",   insn_maxnum_    ## TYPE, #TYPE,  #TYPE, #TYPE, R3,       "__builtin_postrisc_maxnum_"  #TYPE "(a, b)" }, \

#define FP_BINARY_VECTOR_FUNCS(TYPE, CTYPE, ITYPE) \
    { "add",                 insn_add_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "a + b"    }, \
    { "nadd",                insn_nadd_       ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "-(a + b)" }, \
    { "sub",                 insn_sub_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "a - b"    }, \
    { "mul",                 insn_mul_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "a * b"    }, \
    { "nmul",                insn_nmul_       ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3_SWAP,   "-(a * b)" }, \
    { "div",                 insn_div_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "a / b"    }, \
    { "builtin_add",         insn_add_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_add_"        #TYPE "(a, b)" }, \
    { "builtin_nadd",        insn_nadd_       ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_nadd_"       #TYPE "(a, b)" }, \
    { "builtin_sub",         insn_sub_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_sub_"        #TYPE "(a, b)" }, \
    { "builtin_mul",         insn_mul_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_mul_"        #TYPE "(a, b)" }, \
    { "builtin_nmul",        insn_nmul_       ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_nmul_"       #TYPE "(a, b)" }, \
    { "builtin_div",         insn_div_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_div_"        #TYPE "(a, b)" }, \
    { "builtin_abs_max",     insn_abs_max_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_abs_max_"    #TYPE "(a, b)" }, \
    { "builtin_abs_min",     insn_abs_min_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_abs_min_"    #TYPE "(a, b)" }, \
    { "builtin_max",         insn_max_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_max_"        #TYPE "(a, b)" }, \
    { "builtin_min",         insn_min_        ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_min_"        #TYPE "(a, b)" }, \
    { "builtin_maxnum",      insn_maxnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_maxnum_"     #TYPE "(a, b)" }, \
    { "builtin_minnum",      insn_minnum_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_minnum_"     #TYPE "(a, b)" }, \
    { "builtin_add_alt",     insn_add_alt_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_add_alt_"    #TYPE "(a, b)" }, \
    { "builtin_sub_alt",     insn_sub_alt_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_sub_alt_"    #TYPE "(a, b)" }, \
    { "builtin_add_horiz",   insn_add_horiz_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_add_horiz_"  #TYPE "(a, b)" }, \
    { "builtin_sub_horiz",   insn_sub_horiz_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_sub_horiz_"  #TYPE "(a, b)" }, \
    { "builtin_mul_horiz",   insn_mul_horiz_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_mul_horiz_"  #TYPE "(a, b)" }, \
    { "builtin_abs_diff",    insn_abs_diff_   ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_abs_diff_"   #TYPE "(a, b)" }, \
    { "builtin_nabs_diff",   insn_nabs_diff_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_nabs_diff_"  #TYPE "(a, b)" }, \
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

//    { "builtin_merge_low",   insn_merge_low_  ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_merge_low_"  #TYPE "(a, b)" },
//    { "builtin_merge_high",  insn_merge_high_ ## TYPE, #CTYPE, #CTYPE, #CTYPE, R3,        "__builtin_postrisc_merge_high_" #TYPE "(a, b)" },


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

#define FP_TERNARY_FUNCS(TYPE, SUFFIX) \
    { "fma",            insn_madd_   ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(a, b, c)" }, \
    { "fma_nega",       insn_nmadd_  ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(-a, b, c)" }, \
    { "fma_negc",       insn_msub_   ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(a, b, -c)" }, \
    { "fma_nega_negc",  insn_nmsub_  ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_fma" SUFFIX "(-a, b, -c)" }, \
    { "merge",          insn_merge_  ## TYPE, #TYPE, #TYPE, #TYPE, #TYPE, R4, "__builtin_postrisc_merge_" #TYPE "(a, b, c)" }, \

#define FP_TERNARY_VECTOR_FUNCS(TYPE, CTYPE) \
    { "fma",            insn_madd_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_madd_"     #TYPE "(a, b, c)" }, \
    { "fma_nega",       insn_nmadd_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_madd_"     #TYPE "(-a, b, c)" }, \
    { "fma_negc",       insn_msub_     ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_madd_"     #TYPE "(a, b, -c)" }, \
    { "fma_nega_negc",  insn_nmsub_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_madd_"     #TYPE "(-a, b, -c)" }, \
    { "fmadd_alt",      insn_madd_alt_ ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_madd_alt_" #TYPE "(a, b, c)" }, \
    { "fmsub_alt",      insn_msub_alt_ ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_msub_alt_" #TYPE "(a, b, c)" }, \
    { "merge",          insn_merge_    ## TYPE, #CTYPE, #CTYPE, #CTYPE, #CTYPE, R4, "__builtin_postrisc_merge_"    #TYPE "(a, b, c)" }, \

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

        out << CHECK_LABEL << fname.str() << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 4" << std::endl;
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b,"
                   << func.src3_type << " c)" << std::endl;
        out << "{" << std::endl;
        out << "    return " << func.fragment << ";" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
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

} // namespace llvm
} // namespace postrisc
