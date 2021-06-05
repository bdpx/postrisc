#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

#define C_RNE ", rne"
#define C_RMM ", rmm"
#define C_RDN ", rdn"
#define C_RUP ", rup"
#define C_RTZ ", rtz"
#define C_DYN ", dyn"

#define R2      "%r1, %r1"
#define R3      "%r1, %r1, %r2"
#define R3_SWAP "%r1, %r2, %r1"
#define R4_SIGN "%r1, %r2, %r1, %r1"
#define R4      "%r1, %r1, %r2, %r3"

namespace postrisc {
namespace llvm {

// FLOAT-FLOAT rounding
//
// roundeven:       rounds to the nearest integer, with halfway cases (e.g., .5) rounding to the nearest even
// rint, nearbyint: rounds the floating-point argument to an integer value in floating-point format, using the current rounding mode.
// round:           computes the nearest integer value to arg (in floating-point format), rounding halfway cases away from zero, regardless of the current rounding mode.
// trunc:           rounds to nearest integer not greater in magnitude than the given value
// floor:           computes largest integer not greater than the given value
// ceil:            computes smallest integer not less than the given value

static const struct {
    RoundingMode rm;
    char         constant[24];
} rounding_modes[] = {
    { RoundingMode::RNE, "__POSTRISC_RM_RNE" },
    { RoundingMode::RTZ, "__POSTRISC_RM_RTZ" },
    { RoundingMode::RDN, "__POSTRISC_RM_RDN" },
    { RoundingMode::RUP, "__POSTRISC_RM_RUP" },
    { RoundingMode::RMM, "__POSTRISC_RM_RMM" },
    { RoundingMode::RNO, "__POSTRISC_RM_RNO" },
    { RoundingMode::RTO, "__POSTRISC_RM_RTO" },
    { RoundingMode::DYN, "__POSTRISC_RM_DYN" },
};

static const unary_info unary_functions[] = {
    { "neg",           insn_fnegh,   FH, FH, R2,       "(-x)" },
    { "abs",           insn_fabsh,   FH, FH, R2,       "__builtin_fabsf16(x)" },
    { "neg_abs",       insn_fnabsh,  FH, FH, R2,       "- __builtin_fabsf16(x)" },
    { "sqrt",          insn_fsqrth,  FH, FH, R2,       "__builtin_sqrtf16(x)" },
    { "roundeven",     insn_frndh,   FH, FH, R2 C_RNE, "__builtin_roundevenf16(x)" },
    { "round",         insn_frndh,   FH, FH, R2 C_RMM, "__builtin_roundf16(x)" },
    { "floor",         insn_frndh,   FH, FH, R2 C_RDN, "__builtin_floorf16(x)" },
    { "ceil",          insn_frndh,   FH, FH, R2 C_RUP, "__builtin_ceilf16(x)" },
    { "trunc",         insn_frndh,   FH, FH, R2 C_RTZ, "__builtin_truncf16(x)" },
    { "nearbyint",     insn_frndh,   FH, FH, R2 C_DYN, "__builtin_nearbyintf16(x)" },
    { "rint",          insn_frndxh,  FH, FH, R2 C_DYN, "__builtin_rintf16(x)" },
    { "cvt",           insn_fiwtoh,  FH, IW, R2 C_RMM, "(f16)(x)" },
    { "cvt",           insn_fidtoh,  FH, ID, R2 C_RMM, "(f16)(x)" },
    { "cvt",           insn_fiqtoh,  FH, IQ, R2 C_RMM, "(f16)(x)" },
    { "cvt",           insn_fuwtoh,  FH, UW, R2 C_RMM, "(f16)(x)" },
    { "cvt",           insn_fudtoh,  FH, UD, R2 C_RMM, "(f16)(x)" },
    { "cvt",           insn_fuqtoh,  FH, UQ, R2 C_RMM, "(f16)(x)" },
    { "trunc_def",     insn_fhtoiw,  IW, FH, R2 C_RTZ, "(i32)(x)" },
    { "trunc_def",     insn_fhtoid,  ID, FH, R2 C_RTZ, "(i64)(x)" },
    { "trunc_def",     insn_fhtoiq,  IQ, FH, R2 C_RTZ, "(i128)(x)" },
    { "trunc_def",     insn_fhtouw,  UW, FH, R2 C_RTZ, "(u32)(x)" },
    { "trunc_def",     insn_fhtoud,  UD, FH, R2 C_RTZ, "(u64)(x)" },
    { "trunc_def",     insn_fhtouq,  UQ, FH, R2 C_RTZ, "(u128)(x)" },
    { "lrint",         insn_fhtoid,  ID, FH, R2 C_DYN, "__builtin_lrintf16(x)" },
    { "llrint",        insn_fhtoid,  ID, FH, R2 C_DYN, "__builtin_llrintf16(x)" },
    { "lround",        insn_fhtoid,  ID, FH, R2 C_RMM, "__builtin_lroundf16(x)" },
    { "llround",       insn_fhtoid,  ID, FH, R2 C_RMM, "__builtin_llroundf16(x)" },
    { "roundeven",     insn_fhtoiw,  IW, FH, R2 C_RNE, "(i32)__builtin_roundevenf16(x)" },
    { "round",         insn_fhtoiw,  IW, FH, R2 C_RMM, "(i32)__builtin_roundf16(x)" },
    { "floor",         insn_fhtoiw,  IW, FH, R2 C_RDN, "(i32)__builtin_floorf16(x)" },
    { "ceil",          insn_fhtoiw,  IW, FH, R2 C_RUP, "(i32)__builtin_ceilf16(x)" },
    { "trunc",         insn_fhtoiw,  IW, FH, R2 C_RTZ, "(i32)__builtin_truncf16(x)" },
    { "nearbyint",     insn_fhtoiw,  IW, FH, R2 C_DYN, "(i32)__builtin_nearbyintf16(x)" },
    { "roundeven",     insn_fhtouw,  UW, FH, R2 C_RNE, "(u32)__builtin_roundevenf16(x)" },
    { "round",         insn_fhtouw,  UW, FH, R2 C_RMM, "(u32)__builtin_roundf16(x)" },
    { "floor",         insn_fhtouw,  UW, FH, R2 C_RDN, "(u32)__builtin_floorf16(x)" },
    { "ceil",          insn_fhtouw,  UW, FH, R2 C_RUP, "(u32)__builtin_ceilf16(x)" },
    { "trunc",         insn_fhtouw,  UW, FH, R2 C_RTZ, "(u32)__builtin_truncf16(x)" },
    { "nearbyint",     insn_fhtouw,  UW, FH, R2 C_DYN, "(u32)__builtin_nearbyintf16(x)" },
    { "roundeven",     insn_fhtoid,  ID, FH, R2 C_RNE, "(i64)__builtin_roundevenf16(x)" },
    { "round",         insn_fhtoid,  ID, FH, R2 C_RMM, "(i64)__builtin_roundf16(x)" },
    { "floor",         insn_fhtoid,  ID, FH, R2 C_RDN, "(i64)__builtin_floorf16(x)" },
    { "ceil",          insn_fhtoid,  ID, FH, R2 C_RUP, "(i64)__builtin_ceilf16(x)" },
    { "trunc",         insn_fhtoid,  ID, FH, R2 C_RTZ, "(i64)__builtin_truncf16(x)" },
    { "nearbyint",     insn_fhtoid,  ID, FH, R2 C_DYN, "(i64)__builtin_nearbyintf16(x)" },
    { "roundeven",     insn_fhtoud,  UD, FH, R2 C_RNE, "(u64)__builtin_roundevenf16(x)" },
    { "round",         insn_fhtoud,  UD, FH, R2 C_RMM, "(u64)__builtin_roundf16(x)" },
    { "floor",         insn_fhtoud,  UD, FH, R2 C_RDN, "(u64)__builtin_floorf16(x)" },
    { "ceil",          insn_fhtoud,  UD, FH, R2 C_RUP, "(u64)__builtin_ceilf16(x)" },
    { "trunc",         insn_fhtoud,  UD, FH, R2 C_RTZ, "(u64)__builtin_truncf16(x)" },
    { "nearbyint",     insn_fhtoud,  UD, FH, R2 C_DYN, "(u64)__builtin_nearbyintf16(x)" },
    { "roundeven",     insn_fhtoiq,  IQ, FH, R2 C_RNE, "(i128)__builtin_roundevenf16(x)" },
    { "round",         insn_fhtoiq,  IQ, FH, R2 C_RMM, "(i128)__builtin_roundf16(x)" },
    { "floor",         insn_fhtoiq,  IQ, FH, R2 C_RDN, "(i128)__builtin_floorf16(x)" },
    { "ceil",          insn_fhtoiq,  IQ, FH, R2 C_RUP, "(i128)__builtin_ceilf16(x)" },
    { "trunc",         insn_fhtoiq,  IQ, FH, R2 C_RTZ, "(i128)__builtin_truncf16(x)" },
    { "nearbyint",     insn_fhtoiq,  IQ, FH, R2 C_DYN, "(i128)__builtin_nearbyintf16(x)" },
    { "roundeven",     insn_fhtouq,  UQ, FH, R2 C_RNE, "(u128)__builtin_roundevenf16(x)" },
    { "round",         insn_fhtouq,  UQ, FH, R2 C_RMM, "(u128)__builtin_roundf16(x)" },
    { "floor",         insn_fhtouq,  UQ, FH, R2 C_RDN, "(u128)__builtin_floorf16(x)" },
    { "ceil",          insn_fhtouq,  UQ, FH, R2 C_RUP, "(u128)__builtin_ceilf16(x)" },
    { "trunc",         insn_fhtouq,  UQ, FH, R2 C_RTZ, "(u128)__builtin_truncf16(x)" },
    { "nearbyint",     insn_fhtouq,  UQ, FH, R2 C_DYN, "(u128)__builtin_nearbyintf16(x)" },

    { "neg",           insn_fnegs,    FS, FS, R2,       "(-x)" },
    { "abs",           insn_fabss,    FS, FS, R2,       "__builtin_fabsf(x)" },
    { "neg_abs",       insn_fnabss,   FS, FS, R2,       "- __builtin_fabsf(x)" },
    { "sqrt",          insn_fsqrts,   FS, FS, R2,       "__builtin_sqrtf(x)" },
    { "roundeven",     insn_frnds,    FS, FS, R2 C_RNE, "__builtin_roundevenf(x)" },
    { "round",         insn_frnds,    FS, FS, R2 C_RMM, "__builtin_roundf(x)" },
    { "floor",         insn_frnds,    FS, FS, R2 C_RDN, "__builtin_floorf(x)" },
    { "ceil",          insn_frnds,    FS, FS, R2 C_RUP, "__builtin_ceilf(x)" },
    { "trunc",         insn_frnds,    FS, FS, R2 C_RTZ, "__builtin_truncf(x)" },
    { "nearbyint",     insn_frnds,    FS, FS, R2 C_DYN, "__builtin_nearbyintf(x)" },
    { "rint",          insn_frndxs,   FS, FS, R2 C_DYN, "__builtin_rintf(x)" },
    { "cvt",           insn_fiwtos,   FS, IW, R2 C_RMM, "(f32)(x)" },
    { "cvt",           insn_fidtos,   FS, ID, R2 C_RMM, "(f32)(x)" },
    { "cvt",           insn_fiqtos,   FS, IQ, R2 C_RMM, "(f32)(x)" },
    { "cvt",           insn_fuwtos,   FS, UW, R2 C_RMM, "(f32)(x)" },
    { "cvt",           insn_fudtos,   FS, UD, R2 C_RMM, "(f32)(x)" },
    { "cvt",           insn_fuqtos,   FS, UQ, R2 C_RMM, "(f32)(x)" },
    { "trunc_def",     insn_fstoiw,   IW, FS, R2 C_RTZ, "(i32)(x)" },
    { "trunc_def",     insn_fstoid,   ID, FS, R2 C_RTZ, "(i64)(x)" },
    { "trunc_def",     insn_fstoiq,   IQ, FS, R2 C_RTZ, "(i128)(x)" },
    { "trunc_def",     insn_fstouw,   UW, FS, R2 C_RTZ, "(u32)(x)" },
    { "trunc_def",     insn_fstoud,   UD, FS, R2 C_RTZ, "(u64)(x)" },
    { "trunc_def",     insn_fstouq,   UQ, FS, R2 C_RTZ, "(u128)(x)" },
    { "lrint",         insn_fstoid,   ID, FS, R2 C_DYN, "__builtin_lrintf(x)" },
    { "llrint",        insn_fstoid,   ID, FS, R2 C_DYN, "__builtin_llrintf(x)" },
    { "lround",        insn_fstoid,   ID, FS, R2 C_RMM, "__builtin_lroundf(x)" },
    { "llround",       insn_fstoid,   ID, FS, R2 C_RMM, "__builtin_llroundf(x)" },

    { "roundeven",     insn_fstoiw,  IW, FS, R2 C_RNE, "(i32)__builtin_roundevenf(x)" },
    { "round",         insn_fstoiw,  IW, FS, R2 C_RMM, "(i32)__builtin_roundf(x)" },
    { "floor",         insn_fstoiw,  IW, FS, R2 C_RDN, "(i32)__builtin_floorf(x)" },
    { "ceil",          insn_fstoiw,  IW, FS, R2 C_RUP, "(i32)__builtin_ceilf(x)" },
    { "trunc",         insn_fstoiw,  IW, FS, R2 C_RTZ, "(i32)__builtin_truncf(x)" },
    { "nearbyint",     insn_fstoiw,  IW, FS, R2 C_DYN, "(i32)__builtin_nearbyintf(x)" },
    { "roundeven",     insn_fstouw,  UW, FS, R2 C_RNE, "(u32)__builtin_roundevenf(x)" },
    { "round",         insn_fstouw,  UW, FS, R2 C_RMM, "(u32)__builtin_roundf(x)" },
    { "floor",         insn_fstouw,  UW, FS, R2 C_RDN, "(u32)__builtin_floorf(x)" },
    { "ceil",          insn_fstouw,  UW, FS, R2 C_RUP, "(u32)__builtin_ceilf(x)" },
    { "trunc",         insn_fstouw,  UW, FS, R2 C_RTZ, "(u32)__builtin_truncf(x)" },
    { "nearbyint",     insn_fstouw,  UW, FS, R2 C_DYN, "(u32)__builtin_nearbyintf(x)" },

    { "roundeven",     insn_fstoid,  ID, FS, R2 C_RNE, "(i64)__builtin_roundevenf(x)" },
    { "round",         insn_fstoid,  ID, FS, R2 C_RMM, "(i64)__builtin_roundf(x)" },
    { "floor",         insn_fstoid,  ID, FS, R2 C_RDN, "(i64)__builtin_floorf(x)" },
    { "ceil",          insn_fstoid,  ID, FS, R2 C_RUP, "(i64)__builtin_ceilf(x)" },
    { "trunc",         insn_fstoid,  ID, FS, R2 C_RTZ, "(i64)__builtin_truncf(x)" },
    { "nearbyint",     insn_fstoid,  ID, FS, R2 C_DYN, "(i64)__builtin_nearbyintf(x)" },
    { "roundeven",     insn_fstoud,  UD, FS, R2 C_RNE, "(u64)__builtin_roundevenf(x)" },
    { "round",         insn_fstoud,  UD, FS, R2 C_RMM, "(u64)__builtin_roundf(x)" },
    { "floor",         insn_fstoud,  UD, FS, R2 C_RDN, "(u64)__builtin_floorf(x)" },
    { "ceil",          insn_fstoud,  UD, FS, R2 C_RUP, "(u64)__builtin_ceilf(x)" },
    { "trunc",         insn_fstoud,  UD, FS, R2 C_RTZ, "(u64)__builtin_truncf(x)" },
    { "nearbyint",     insn_fstoud,  UD, FS, R2 C_DYN, "(u64)__builtin_nearbyintf(x)" },

    { "roundeven",     insn_fstoiq, IQ, FS, R2 C_RNE, "(i128)__builtin_roundevenf(x)" },
    { "round",         insn_fstoiq, IQ, FS, R2 C_RMM, "(i128)__builtin_roundf(x)" },
    { "floor",         insn_fstoiq, IQ, FS, R2 C_RDN, "(i128)__builtin_floorf(x)" },
    { "ceil",          insn_fstoiq, IQ, FS, R2 C_RUP, "(i128)__builtin_ceilf(x)" },
    { "trunc",         insn_fstoiq, IQ, FS, R2 C_RTZ, "(i128)__builtin_truncf(x)" },
    { "nearbyint",     insn_fstoiq, IQ, FS, R2 C_DYN, "(i128)__builtin_nearbyintf(x)" },
    { "roundeven",     insn_fstouq, UQ, FS, R2 C_RNE, "(u128)__builtin_roundevenf(x)" },
    { "round",         insn_fstouq, UQ, FS, R2 C_RMM, "(u128)__builtin_roundf(x)" },
    { "floor",         insn_fstouq, UQ, FS, R2 C_RDN, "(u128)__builtin_floorf(x)" },
    { "ceil",          insn_fstouq, UQ, FS, R2 C_RUP, "(u128)__builtin_ceilf(x)" },
    { "trunc",         insn_fstouq, UQ, FS, R2 C_RTZ, "(u128)__builtin_truncf(x)" },
    { "nearbyint",     insn_fstouq, UQ, FS, R2 C_DYN, "(u128)__builtin_nearbyintf(x)" },

    { "neg",           insn_fnegd,   FD, FD, R2,       "(-x)" },
    { "abs",           insn_fabsd,   FD, FD, R2,       "__builtin_fabs(x)" },
    { "neg_abs",       insn_fnabsd,  FD, FD, R2,       "- __builtin_fabs(x)" },
    { "sqrt",          insn_fsqrtd,  FD, FD, R2,       "__builtin_sqrt(x)" },
    { "roundeven",     insn_frndd,   FD, FD, R2 C_RNE, "__builtin_roundeven(x)" },
    { "round",         insn_frndd,   FD, FD, R2 C_RMM, "__builtin_round(x)" },
    { "floor",         insn_frndd,   FD, FD, R2 C_RDN, "__builtin_floor(x)" },
    { "ceil",          insn_frndd,   FD, FD, R2 C_RUP, "__builtin_ceil(x)" },
    { "trunc",         insn_frndd,   FD, FD, R2 C_RTZ, "__builtin_trunc(x)" },
    { "nearbyint",     insn_frndd,   FD, FD, R2 C_DYN, "__builtin_nearbyint(x)" },
    { "rint",          insn_frndxd,  FD, FD, R2 C_DYN, "__builtin_rint(x)" },
    { "cvt",           insn_fiwtod,  FD, IW, R2 C_RMM, "(f64)(x)" },
    { "cvt",           insn_fidtod,  FD, ID, R2 C_RMM, "(f64)(x)" },
    { "cvt",           insn_fiqtod,  FD, IQ, R2 C_RMM, "(f64)(x)" },
    { "cvt",           insn_fuwtod,  FD, UW, R2 C_RMM, "(f64)(x)" },
    { "cvt",           insn_fudtod,  FD, UD, R2 C_RMM, "(f64)(x)" },
    { "cvt",           insn_fuqtod,  FD, UQ, R2 C_RMM, "(f64)(x)" },
    { "trunc_def",     insn_fdtoiw,  IW, FD, R2 C_RTZ, "(i32)(x)" },
    { "trunc_def",     insn_fdtoid,  ID, FD, R2 C_RTZ, "(i64)(x)" },
    { "trunc_def",     insn_fdtoiq,  IQ, FD, R2 C_RTZ, "(i128)(x)" },
    { "trunc_def",     insn_fdtouw,  UW, FD, R2 C_RTZ, "(u32)(x)" },
    { "trunc_def",     insn_fdtoud,  UD, FD, R2 C_RTZ, "(u64)(x)" },
    { "trunc_def",     insn_fdtouq,  UQ, FD, R2 C_RTZ, "(u128)(x)" },
    { "lrint",         insn_fdtoid,  ID, FD, R2 C_DYN, "__builtin_lrint(x)" },
    { "llrint",        insn_fdtoid,  ID, FD, R2 C_DYN, "__builtin_llrint(x)" },
    { "lround",        insn_fdtoid,  ID, FD, R2 C_RMM, "__builtin_lround(x)" },
    { "llround",       insn_fdtoid,  ID, FD, R2 C_RMM, "__builtin_llround(x)" },

    { "roundeven",     insn_fdtoiw,  IW, FD, R2 C_RNE, "(i32)__builtin_roundeven(x)" },
    { "round",         insn_fdtoiw,  IW, FD, R2 C_RMM, "(i32)__builtin_round(x)" },
    { "floor",         insn_fdtoiw,  IW, FD, R2 C_RDN, "(i32)__builtin_floor(x)" },
    { "ceil",          insn_fdtoiw,  IW, FD, R2 C_RUP, "(i32)__builtin_ceil(x)" },
    { "trunc",         insn_fdtoiw,  IW, FD, R2 C_RTZ, "(i32)__builtin_trunc(x)" },
    { "nearbyint",     insn_fdtoiw,  IW, FD, R2 C_DYN, "(i32)__builtin_nearbyint(x)" },
    { "roundeven",     insn_fdtouw,  UW, FD, R2 C_RNE, "(u32)__builtin_roundeven(x)" },
    { "round",         insn_fdtouw,  UW, FD, R2 C_RMM, "(u32)__builtin_round(x)" },
    { "floor",         insn_fdtouw,  UW, FD, R2 C_RDN, "(u32)__builtin_floor(x)" },
    { "ceil",          insn_fdtouw,  UW, FD, R2 C_RUP, "(u32)__builtin_ceil(x)" },
    { "trunc",         insn_fdtouw,  UW, FD, R2 C_RTZ, "(u32)__builtin_trunc(x)" },
    { "nearbyint",     insn_fdtouw,  UW, FD, R2 C_DYN, "(u32)__builtin_nearbyint(x)" },

    { "roundeven",     insn_fdtoid,  ID, FD, R2 C_RNE, "(i64)__builtin_roundeven(x)" },
    { "round",         insn_fdtoid,  ID, FD, R2 C_RMM, "(i64)__builtin_round(x)" },
    { "floor",         insn_fdtoid,  ID, FD, R2 C_RDN, "(i64)__builtin_floor(x)" },
    { "ceil",          insn_fdtoid,  ID, FD, R2 C_RUP, "(i64)__builtin_ceil(x)" },
    { "trunc",         insn_fdtoid,  ID, FD, R2 C_RTZ, "(i64)__builtin_trunc(x)" },
    { "nearbyint",     insn_fdtoid,  ID, FD, R2 C_DYN, "(i64)__builtin_nearbyint(x)" },
    { "roundeven",     insn_fdtoud,  UD, FD, R2 C_RNE, "(u64)__builtin_roundeven(x)" },
    { "round",         insn_fdtoud,  UD, FD, R2 C_RMM, "(u64)__builtin_round(x)" },
    { "floor",         insn_fdtoud,  UD, FD, R2 C_RDN, "(u64)__builtin_floor(x)" },
    { "ceil",          insn_fdtoud,  UD, FD, R2 C_RUP, "(u64)__builtin_ceil(x)" },
    { "trunc",         insn_fdtoud,  UD, FD, R2 C_RTZ, "(u64)__builtin_trunc(x)" },
    { "nearbyint",     insn_fdtoud,  UD, FD, R2 C_DYN, "(u64)__builtin_nearbyint(x)" },

    { "roundeven",     insn_fdtoiq, IQ, FD, R2 C_RNE, "(i128)__builtin_roundeven(x)" },
    { "round",         insn_fdtoiq, IQ, FD, R2 C_RMM, "(i128)__builtin_round(x)" },
    { "floor",         insn_fdtoiq, IQ, FD, R2 C_RDN, "(i128)__builtin_floor(x)" },
    { "ceil",          insn_fdtoiq, IQ, FD, R2 C_RUP, "(i128)__builtin_ceil(x)" },
    { "trunc",         insn_fdtoiq, IQ, FD, R2 C_RTZ, "(i128)__builtin_trunc(x)" },
    { "nearbyint",     insn_fdtoiq, IQ, FD, R2 C_DYN, "(i128)__builtin_nearbyint(x)" },
    { "roundeven",     insn_fdtouq, UQ, FD, R2 C_RNE, "(u128)__builtin_roundeven(x)" },
    { "round",         insn_fdtouq, UQ, FD, R2 C_RMM, "(u128)__builtin_round(x)" },
    { "floor",         insn_fdtouq, UQ, FD, R2 C_RDN, "(u128)__builtin_floor(x)" },
    { "ceil",          insn_fdtouq, UQ, FD, R2 C_RUP, "(u128)__builtin_ceil(x)" },
    { "trunc",         insn_fdtouq, UQ, FD, R2 C_RTZ, "(u128)__builtin_trunc(x)" },
    { "nearbyint",     insn_fdtouq, UQ, FD, R2 C_DYN, "(u128)__builtin_nearbyint(x)" },

    // quadruple as long double
    { "neg",           insn_fnegq,   FQ, FQ, R2,       "(-x)" },
    { "abs",           insn_fabsq,   FQ, FQ, R2,       "__builtin_fabsl(x)" },
    { "neg_abs",       insn_fnabsq,  FQ, FQ, R2,       "- __builtin_fabsl(x)" },
    { "sqrt",          insn_fsqrtq,  FQ, FQ, R2,       "__builtin_sqrtl(x)" },
    { "roundeven",     insn_frndq,   FQ, FQ, R2 C_RNE, "__builtin_roundevenl(x)" },
    { "round",         insn_frndq,   FQ, FQ, R2 C_RMM, "__builtin_roundl(x)" },
    { "floor",         insn_frndq,   FQ, FQ, R2 C_RDN, "__builtin_floorl(x)" },
    { "ceil",          insn_frndq,   FQ, FQ, R2 C_RUP, "__builtin_ceill(x)" },
    { "trunc",         insn_frndq,   FQ, FQ, R2 C_RTZ, "__builtin_truncl(x)" },
    { "nearbyint",     insn_frndq,   FQ, FQ, R2 C_DYN, "__builtin_nearbyintl(x)" },
    { "rint",          insn_frndxq,  FQ, FQ, R2 C_DYN, "__builtin_rintl(x)" },
    { "cvt",           insn_fiwtoq,  FQ, IW, R2 C_RMM, "(f128)(x)" },
    { "cvt",           insn_fidtoq,  FQ, ID, R2 C_RMM, "(f128)(x)" },
    { "cvt",           insn_fiqtoq,  FQ, IQ, R2 C_RMM, "(f128)(x)" },
    { "cvt",           insn_fuwtoq,  FQ, UW, R2 C_RMM, "(f128)(x)" },
    { "cvt",           insn_fudtoq,  FQ, UD, R2 C_RMM, "(f128)(x)" },
    { "cvt",           insn_fuqtoq,  FQ, UQ, R2 C_RMM, "(f128)(x)" },
    { "trunc_def",     insn_fqtoiw,  IW, FQ, R2 C_RTZ, "(i32)(x)" },
    { "trunc_def",     insn_fqtoid,  ID, FQ, R2 C_RTZ, "(i64)(x)" },
    { "trunc_def",     insn_fqtoiq,  IQ, FQ, R2 C_RTZ, "(i128)(x)" },
    { "trunc_def",     insn_fqtouw,  UW, FQ, R2 C_RTZ, "(u32)(x)" },
    { "trunc_def",     insn_fqtoud,  UD, FQ, R2 C_RTZ, "(u64)(x)" },
    { "trunc_def",     insn_fqtouq,  UQ, FQ, R2 C_RTZ, "(u128)(x)" },
    { "lrint",         insn_fqtoid,  ID, FQ, R2 C_DYN, "__builtin_lrintl(x)" },
    { "llrint",        insn_fqtoid,  ID, FQ, R2 C_DYN, "__builtin_llrintl(x)" },
    { "lround",        insn_fqtoid,  ID, FQ, R2 C_RMM, "__builtin_lroundl(x)" },
    { "llround",       insn_fqtoid,  ID, FQ, R2 C_RMM, "__builtin_llroundl(x)" },

    { "roundeven",     insn_fqtoiw,  IW, FQ, R2 C_RNE, "(i32)__builtin_roundevenl(x)" },
    { "round",         insn_fqtoiw,  IW, FQ, R2 C_RMM, "(i32)__builtin_roundl(x)" },
    { "floor",         insn_fqtoiw,  IW, FQ, R2 C_RDN, "(i32)__builtin_floorl(x)" },
    { "ceil",          insn_fqtoiw,  IW, FQ, R2 C_RUP, "(i32)__builtin_ceill(x)" },
    { "trunc",         insn_fqtoiw,  IW, FQ, R2 C_RTZ, "(i32)__builtin_truncl(x)" },
    { "nearbyint",     insn_fqtoiw,  IW, FQ, R2 C_DYN, "(i32)__builtin_nearbyintl(x)" },
    { "roundeven",     insn_fqtouw,  UW, FQ, R2 C_RNE, "(u32)__builtin_roundevenl(x)" },
    { "round",         insn_fqtouw,  UW, FQ, R2 C_RMM, "(u32)__builtin_roundl(x)" },
    { "floor",         insn_fqtouw,  UW, FQ, R2 C_RDN, "(u32)__builtin_floorl(x)" },
    { "ceil",          insn_fqtouw,  UW, FQ, R2 C_RUP, "(u32)__builtin_ceill(x)" },
    { "trunc",         insn_fqtouw,  UW, FQ, R2 C_RTZ, "(u32)__builtin_truncl(x)" },
    { "nearbyint",     insn_fqtouw,  UW, FQ, R2 C_DYN, "(u32)__builtin_nearbyintl(x)" },

    { "roundeven",     insn_fqtoid,  ID, FQ, R2 C_RNE, "(i64)__builtin_roundevenl(x)" },
    { "round",         insn_fqtoid,  ID, FQ, R2 C_RMM, "(i64)__builtin_roundl(x)" },
    { "floor",         insn_fqtoid,  ID, FQ, R2 C_RDN, "(i64)__builtin_floorl(x)" },
    { "ceil",          insn_fqtoid,  ID, FQ, R2 C_RUP, "(i64)__builtin_ceill(x)" },
    { "trunc",         insn_fqtoid,  ID, FQ, R2 C_RTZ, "(i64)__builtin_truncl(x)" },
    { "nearbyint",     insn_fqtoid,  ID, FQ, R2 C_DYN, "(i64)__builtin_nearbyintl(x)" },
    { "roundeven",     insn_fqtoud,  UD, FQ, R2 C_RNE, "(u64)__builtin_roundevenl(x)" },
    { "round",         insn_fqtoud,  UD, FQ, R2 C_RMM, "(u64)__builtin_roundl(x)" },
    { "floor",         insn_fqtoud,  UD, FQ, R2 C_RDN, "(u64)__builtin_floorl(x)" },
    { "ceil",          insn_fqtoud,  UD, FQ, R2 C_RUP, "(u64)__builtin_ceill(x)" },
    { "trunc",         insn_fqtoud,  UD, FQ, R2 C_RTZ, "(u64)__builtin_truncl(x)" },
    { "nearbyint",     insn_fqtoud,  UD, FQ, R2 C_DYN, "(u64)__builtin_nearbyintl(x)" },

    { "roundeven",     insn_fqtoiq,  IQ, FQ, R2 C_RNE, "(i128)__builtin_roundevenl(x)" },
    { "round",         insn_fqtoiq,  IQ, FQ, R2 C_RMM, "(i128)__builtin_roundl(x)" },
    { "floor",         insn_fqtoiq,  IQ, FQ, R2 C_RDN, "(i128)__builtin_floorl(x)" },
    { "ceil",          insn_fqtoiq,  IQ, FQ, R2 C_RUP, "(i128)__builtin_ceill(x)" },
    { "trunc",         insn_fqtoiq,  IQ, FQ, R2 C_RTZ, "(i128)__builtin_truncl(x)" },
    { "nearbyint",     insn_fqtoiq,  IQ, FQ, R2 C_DYN, "(i128)__builtin_nearbyintl(x)" },
    { "roundeven",     insn_fqtouq,  UQ, FQ, R2 C_RNE, "(u128)__builtin_roundevenl(x)" },
    { "round",         insn_fqtouq,  UQ, FQ, R2 C_RMM, "(u128)__builtin_roundl(x)" },
    { "floor",         insn_fqtouq,  UQ, FQ, R2 C_RDN, "(u128)__builtin_floorl(x)" },
    { "ceil",          insn_fqtouq,  UQ, FQ, R2 C_RUP, "(u128)__builtin_ceill(x)" },
    { "trunc",         insn_fqtouq,  UQ, FQ, R2 C_RTZ, "(u128)__builtin_truncl(x)" },
    { "nearbyint",     insn_fqtouq,  UQ, FQ, R2 C_DYN, "(u128)__builtin_nearbyintl(x)" },

    // same quadruple but via __Float128
    { "neg",           insn_fnegq,   FX, FX, R2,       "(-x)" },
    { "abs",           insn_fabsq,   FX, FX, R2,       "__builtin_fabsf128(x)" },
    { "neg_abs",       insn_fnabsq,  FX, FX, R2,       "- __builtin_fabsf128(x)" },
    { "sqrt",          insn_fsqrtq,  FX, FX, R2,       "__builtin_sqrtf128(x)" },
    { "roundeven",     insn_frndq,   FX, FX, R2 C_RNE, "__builtin_roundevenf128(x)" },
    { "round",         insn_frndq,   FX, FX, R2 C_RMM, "__builtin_roundf128(x)" },
    { "floor",         insn_frndq,   FX, FX, R2 C_RDN, "__builtin_floorf128(x)" },
    { "ceil",          insn_frndq,   FX, FX, R2 C_RUP, "__builtin_ceilf128(x)" },
    { "trunc",         insn_frndq,   FX, FX, R2 C_RTZ, "__builtin_truncf128(x)" },
    { "nearbyint",     insn_frndq,   FX, FX, R2 C_DYN, "__builtin_nearbyintf128(x)" },
    { "rint",          insn_frndxq,  FX, FX, R2 C_DYN, "__builtin_rintf128(x)" },
    { "cvt",           insn_fiwtoq,  FX, IW, R2 C_RMM, "(F128)(x)" },
    { "cvt",           insn_fidtoq,  FX, ID, R2 C_RMM, "(F128)(x)" },
    { "cvt",           insn_fiqtoq,  FX, IQ, R2 C_RMM, "(F128)(x)" },
    { "cvt",           insn_fuwtoq,  FX, UW, R2 C_RMM, "(F128)(x)" },
    { "cvt",           insn_fudtoq,  FX, UD, R2 C_RMM, "(F128)(x)" },
    { "cvt",           insn_fuqtoq,  FX, UQ, R2 C_RMM, "(F128)(x)" },
    { "trunc_def",     insn_fqtoiw,  IW, FX, R2 C_RTZ, "(i32)(x)" },
    { "trunc_def",     insn_fqtoid,  ID, FX, R2 C_RTZ, "(i64)(x)" },
    { "trunc_def",     insn_fqtoiq,  IQ, FX, R2 C_RTZ, "(i128)(x)" },
    { "trunc_def",     insn_fqtouw,  UW, FX, R2 C_RTZ, "(u32)(x)" },
    { "trunc_def",     insn_fqtoud,  UD, FX, R2 C_RTZ, "(u64)(x)" },
    { "trunc_def",     insn_fqtouq,  UQ, FX, R2 C_RTZ, "(u128)(x)" },
    { "lrint",         insn_fqtoid,  ID, FX, R2 C_DYN, "__builtin_lrintf128(x)" },
    { "llrint",        insn_fqtoid,  ID, FX, R2 C_DYN, "__builtin_llrintf128(x)" },
    { "lround",        insn_fqtoid,  ID, FX, R2 C_RMM, "__builtin_lroundf128(x)" },
    { "llround",       insn_fqtoid,  ID, FX, R2 C_RMM, "__builtin_llroundf128(x)" },

    { "roundeven",     insn_fqtoiw,  IW, FX, R2 C_RNE, "(i32)__builtin_roundevenf128(x)" },
    { "round",         insn_fqtoiw,  IW, FX, R2 C_RMM, "(i32)__builtin_roundf128(x)" },
    { "floor",         insn_fqtoiw,  IW, FX, R2 C_RDN, "(i32)__builtin_floorf128(x)" },
    { "ceil",          insn_fqtoiw,  IW, FX, R2 C_RUP, "(i32)__builtin_ceilf128(x)" },
    { "trunc",         insn_fqtoiw,  IW, FX, R2 C_RTZ, "(i32)__builtin_truncf128(x)" },
    { "nearbyint",     insn_fqtoiw,  IW, FX, R2 C_DYN, "(i32)__builtin_nearbyintf128(x)" },
    { "roundeven",     insn_fqtouw,  UW, FX, R2 C_RNE, "(u32)__builtin_roundevenf128(x)" },
    { "round",         insn_fqtouw,  UW, FX, R2 C_RMM, "(u32)__builtin_roundf128(x)" },
    { "floor",         insn_fqtouw,  UW, FX, R2 C_RDN, "(u32)__builtin_floorf128(x)" },
    { "ceil",          insn_fqtouw,  UW, FX, R2 C_RUP, "(u32)__builtin_ceilf128(x)" },
    { "trunc",         insn_fqtouw,  UW, FX, R2 C_RTZ, "(u32)__builtin_truncf128(x)" },
    { "nearbyint",     insn_fqtouw,  UW, FX, R2 C_DYN, "(u32)__builtin_nearbyintf128(x)" },

    { "roundeven",     insn_fqtoid,  ID, FX, R2 C_RNE, "(i64)__builtin_roundevenf128(x)" },
    { "round",         insn_fqtoid,  ID, FX, R2 C_RMM, "(i64)__builtin_roundf128(x)" },
    { "floor",         insn_fqtoid,  ID, FX, R2 C_RDN, "(i64)__builtin_floorf128(x)" },
    { "ceil",          insn_fqtoid,  ID, FX, R2 C_RUP, "(i64)__builtin_ceilf128(x)" },
    { "trunc",         insn_fqtoid,  ID, FX, R2 C_RTZ, "(i64)__builtin_truncf128(x)" },
    { "nearbyint",     insn_fqtoid,  ID, FX, R2 C_DYN, "(i64)__builtin_nearbyintf128(x)" },
    { "roundeven",     insn_fqtoud,  UD, FX, R2 C_RNE, "(u64)__builtin_roundevenf128(x)" },
    { "round",         insn_fqtoud,  UD, FX, R2 C_RMM, "(u64)__builtin_roundf128(x)" },
    { "floor",         insn_fqtoud,  UD, FX, R2 C_RDN, "(u64)__builtin_floorf128(x)" },
    { "ceil",          insn_fqtoud,  UD, FX, R2 C_RUP, "(u64)__builtin_ceilf128(x)" },
    { "trunc",         insn_fqtoud,  UD, FX, R2 C_RTZ, "(u64)__builtin_truncf128(x)" },
    { "nearbyint",     insn_fqtoud,  UD, FX, R2 C_DYN, "(u64)__builtin_nearbyintf128(x)" },

    { "roundeven",     insn_fqtoiq,  IQ, FX, R2 C_RNE, "(i128)__builtin_roundevenf128(x)" },
    { "round",         insn_fqtoiq,  IQ, FX, R2 C_RMM, "(i128)__builtin_roundf128(x)" },
    { "floor",         insn_fqtoiq,  IQ, FX, R2 C_RDN, "(i128)__builtin_floorf128(x)" },
    { "ceil",          insn_fqtoiq,  IQ, FX, R2 C_RUP, "(i128)__builtin_ceilf128(x)" },
    { "trunc",         insn_fqtoiq,  IQ, FX, R2 C_RTZ, "(i128)__builtin_truncf128(x)" },
    { "nearbyint",     insn_fqtoiq,  IQ, FX, R2 C_DYN, "(i128)__builtin_nearbyintf128(x)" },
    { "roundeven",     insn_fqtouq,  UQ, FX, R2 C_RNE, "(u128)__builtin_roundevenf128(x)" },
    { "round",         insn_fqtouq,  UQ, FX, R2 C_RMM, "(u128)__builtin_roundf128(x)" },
    { "floor",         insn_fqtouq,  UQ, FX, R2 C_RDN, "(u128)__builtin_floorf128(x)" },
    { "ceil",          insn_fqtouq,  UQ, FX, R2 C_RUP, "(u128)__builtin_ceilf128(x)" },
    { "trunc",         insn_fqtouq,  UQ, FX, R2 C_RTZ, "(u128)__builtin_truncf128(x)" },
    { "nearbyint",     insn_fqtouq,  UQ, FX, R2 C_DYN, "(u128)__builtin_nearbyintf128(x)" },

    // vector float
    { "builtin_neg",         insn_vfnegh,   VFH, VFH, R2,        "(-x)" },
    { "vector_abs",          insn_vfabsh,   VFH, VFH, R2,        "__builtin_elementwise_abs(x)" },
    { "builtin_nabs",        insn_vfnabsh,  VFH, VFH, R2,        "- __builtin_elementwise_abs(x)" },
    { "builtin_sqrt",        insn_vfsqrth,  VFH, VFH, R2,        "__builtin_elementwise_sqrt(x)" },
    { "vector_roundeven",    insn_vfrndh,   VFH, VFH, R2 C_RNE,  "__builtin_elementwise_roundeven(x)" },
    { "vector_round",        insn_vfrndh,   VFH, VFH, R2 C_RMM,  "__builtin_elementwise_round(x)" },
    { "vector_floor",        insn_vfrndh,   VFH, VFH, R2 C_RDN,  "__builtin_elementwise_floor(x)" },
    { "vector_ceil",         insn_vfrndh,   VFH, VFH, R2 C_RUP,  "__builtin_elementwise_ceil(x)" },
    { "vector_trunc",        insn_vfrndh,   VFH, VFH, R2 C_RTZ,  "__builtin_elementwise_trunc(x)" },
    { "vector_nearbyint",    insn_vfrndh,   VFH, VFH, R2 C_DYN,  "__builtin_elementwise_nearbyint(x)" },
    { "vector_rint",         insn_vfrndxh,  VFH, VFH, R2 C_DYN,  "__builtin_elementwise_rint(x)" },
    { "builtin_unpack_low",  insn_vfupklh,  VFH, VFH, R2,        "__builtin_postrisc_vfupklh(x)" },
    { "builtin_unpack_high", insn_vfupkhh,  VFH, VFH, R2,        "__builtin_postrisc_vfupkhh(x)" },

    { "builtin_neg",         insn_vfnegs,   VFW, VFW, R2,        "(-x)" },
    { "vector_abs",          insn_vfabss,   VFW, VFW, R2,        "__builtin_elementwise_abs(x)" },
    { "builtin_nabs",        insn_vfnabss,  VFW, VFW, R2,        "- __builtin_elementwise_abs(x)" },
    { "builtin_sqrt",        insn_vfsqrts,  VFW, VFW, R2,        "__builtin_elementwise_sqrt(x)" },
    { "vector_roundeven",    insn_vfrnds,   VFW, VFW, R2 C_RNE,  "__builtin_elementwise_roundeven(x)" },
    { "vector_round",        insn_vfrnds,   VFW, VFW, R2 C_RMM,  "__builtin_elementwise_round(x)" },
    { "vector_floor",        insn_vfrnds,   VFW, VFW, R2 C_RDN,  "__builtin_elementwise_floor(x)" },
    { "vector_ceil",         insn_vfrnds,   VFW, VFW, R2 C_RUP,  "__builtin_elementwise_ceil(x)" },
    { "vector_trunc",        insn_vfrnds,   VFW, VFW, R2 C_RTZ,  "__builtin_elementwise_trunc(x)" },
    { "vector_nearbyint",    insn_vfrnds,   VFW, VFW, R2 C_DYN,  "__builtin_elementwise_nearbyint(x)" },
    { "vector_rint",         insn_vfrndxs,  VFW, VFW, R2 C_DYN,  "__builtin_elementwise_rint(x)" },
    { "builtin_unpack_low",  insn_vfupkls,  VFW, VFW, R2,        "__builtin_postrisc_vfupkls(x)" },
    { "builtin_unpack_high", insn_vfupkhs,  VFW, VFW, R2,        "__builtin_postrisc_vfupkhs(x)" },

    { "builtin_neg",         insn_vfnegd,   VFD, VFD, R2,        "(-x)" },
    { "vector_abs",          insn_vfabsd,   VFD, VFD, R2,        "__builtin_elementwise_abs(x)" },
    { "builtin_nabs",        insn_vfnabsd,  VFD, VFD, R2,        "- __builtin_elementwise_abs(x)" },
    { "builtin_sqrt",        insn_vfsqrtd,  VFD, VFD, R2,        "__builtin_elementwise_sqrt(x)" },
    { "vector_roundeven",    insn_vfrndd,   VFD, VFD, R2 C_RNE,  "__builtin_elementwise_roundeven(x)" },
    { "vector_round",        insn_vfrndd,   VFD, VFD, R2 C_RMM,  "__builtin_elementwise_round(x)" },
    { "vector_floor",        insn_vfrndd,   VFD, VFD, R2 C_RDN,  "__builtin_elementwise_floor(x)" },
    { "vector_ceil",         insn_vfrndd,   VFD, VFD, R2 C_RUP,  "__builtin_elementwise_ceil(x)" },
    { "vector_trunc",        insn_vfrndd,   VFD, VFD, R2 C_RTZ,  "__builtin_elementwise_trunc(x)" },
    { "vector_nearbyint",    insn_vfrndd,   VFD, VFD, R2 C_DYN,  "__builtin_elementwise_nearbyint(x)" },
    { "vector_rint",         insn_vfrndxd,  VFD, VFD, R2 C_DYN,  "__builtin_elementwise_rint(x)" },
    { "builtin_unpack_low",  insn_vfupkld,  VFD, VFD, R2,        "__builtin_postrisc_vfupkld(x)" },
    { "builtin_unpack_high", insn_vfupkhd,  VFD, VFD, R2,        "__builtin_postrisc_vfupkhd(x)" },

    // { "", insn_class_f64,      BOOL, FD, "__builtin_isnan(x)" },

    // { "signbit", insn_slsrl_u32,    BOOL, FH,  "%r1, %r1, 16, 31",  "__builtin_signbitf16(x)" },
    { "signbit", insn_srliw, BOOL, FS, "%r1, %r1, 31",   "__builtin_signbitf(x)" },
    { "signbit", insn_srlid, BOOL, FD, "%r1, %r1, 63",   "__builtin_signbit(x)" },
    { "signbit", insn_srliq, BOOL, FQ, "%r1, %r1, 127",  "__builtin_signbitl(x)" },

    { "extend", insn_fhtos, FS, FH, R2, "(x)" },
    { "extend", insn_fhtod, FD, FH, R2, "(x)" },
    { "extend", insn_fhtoq, FQ, FH, R2, "(x)" },
    { "extend", insn_fstod, FD, FS, R2, "(x)" },
    { "extend", insn_fstoq, FQ, FS, R2, "(x)" },
    { "extend", insn_fdtoq, FQ, FD, R2, "(x)" },

    { "narrow", insn_fstoh, FH, FS, R2 C_DYN, "(x)" },
    { "narrow", insn_fdtoh, FH, FD, R2 C_DYN, "(x)" },
    { "narrow", insn_fqtoh, FH, FQ, R2 C_DYN, "(x)" },
    { "narrow", insn_fdtos, FS, FD, R2 C_DYN, "(x)" },
    { "narrow", insn_fqtos, FS, FQ, R2 C_DYN, "(x)" },
    { "narrow", insn_fqtod, FD, FQ, R2 C_DYN, "(x)" },
};

CODEGEN_HANDLER(FpUnary)
{
    for (const auto& func : unary_functions) {
        std::ostringstream fname;
        fname << "test_" << func.name << "_" << func.src_type << "_" << func.dst_type;

        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

static const struct {
    EInstructionID insn;
    ValueType dst_type;
    ValueType src_type;
    char args[32];
    char fragment[64];
} unary_rm_functions[] = {
    { insn_fsqrth,  FH, FH, R2, "__builtin_postrisc_fsqrth" },
    { insn_frsqrth, FH, FH, R2, "__builtin_postrisc_frsqrth" },
    { insn_frndh,   FH, FH, R2, "__builtin_postrisc_frndh" },
    { insn_frndxh,  FH, FH, R2, "__builtin_postrisc_frndxh" },

    { insn_fsqrts,  FS, FS, R2, "__builtin_postrisc_fsqrts" },
    { insn_frsqrts, FS, FS, R2, "__builtin_postrisc_frsqrts" },
    { insn_frnds,   FS, FS, R2, "__builtin_postrisc_frnds" },
    { insn_frndxs,  FS, FS, R2, "__builtin_postrisc_frndxs" },

    { insn_fsqrtd,  FD, FD, R2, "__builtin_postrisc_fsqrtd" },
    { insn_frsqrtd, FD, FD, R2, "__builtin_postrisc_frsqrtd" },
    { insn_frndd,   FD, FD, R2, "__builtin_postrisc_frndd" },
    { insn_frndxd,  FD, FD, R2, "__builtin_postrisc_frndxd" },

    { insn_fsqrtq,  FQ, FQ, R2, "__builtin_postrisc_fsqrtq" },
    { insn_frsqrtq, FQ, FQ, R2, "__builtin_postrisc_frsqrtq" },
    { insn_frndq,   FQ, FQ, R2, "__builtin_postrisc_frndq" },
    { insn_frndxq,  FQ, FQ, R2, "__builtin_postrisc_frndxq" },

    { insn_fqtoh,  FH, FQ, R2, "__builtin_postrisc_fqtoh" },
    { insn_fqtos,  FS, FQ, R2, "__builtin_postrisc_fqtos" },
    { insn_fqtod,  FD, FQ, R2, "__builtin_postrisc_fqtod" },
    { insn_fdtoh,  FH, FD, R2, "__builtin_postrisc_fdtoh" },
    { insn_fdtos,  FS, FD, R2, "__builtin_postrisc_fdtos" },
    { insn_fstoh,  FH, FS, R2, "__builtin_postrisc_fstoh" },

    { insn_fhtoiw, IW, FH, R2, "__builtin_postrisc_fhtoiw" },
    { insn_fhtouw, UW, FH, R2, "__builtin_postrisc_fhtouw" },
    { insn_fhtoid, ID, FH, R2, "__builtin_postrisc_fhtoid" },
    { insn_fhtoud, UD, FH, R2, "__builtin_postrisc_fhtoud" },
    { insn_fhtoiq, IQ, FH, R2, "__builtin_postrisc_fhtoiq" },
    { insn_fhtouq, UQ, FH, R2, "__builtin_postrisc_fhtouq" },

    { insn_fiwtoh,  FH, IW, R2, "__builtin_postrisc_fiwtoh" },
    { insn_fuwtoh,  FH, UW, R2, "__builtin_postrisc_fuwtoh" },
    { insn_fidtoh,  FH, ID, R2, "__builtin_postrisc_fidtoh" },
    { insn_fudtoh,  FH, UD, R2, "__builtin_postrisc_fudtoh" },
    { insn_fiqtoh,  FH, IQ, R2, "__builtin_postrisc_fiqtoh" },
    { insn_fuqtoh,  FH, UQ, R2, "__builtin_postrisc_fuqtoh" },

    { insn_fstoiw,  IW, FS, R2, "__builtin_postrisc_fstoiw" },
    { insn_fstouw,  UW, FS, R2, "__builtin_postrisc_fstouw" },
    { insn_fstoid,  ID, FS, R2, "__builtin_postrisc_fstoid" },
    { insn_fstoud,  UD, FS, R2, "__builtin_postrisc_fstoud" },
    { insn_fstoiq,  IQ, FS, R2, "__builtin_postrisc_fstoiq" },
    { insn_fstouq,  UQ, FS, R2, "__builtin_postrisc_fstouq" },

    { insn_fiwtos,  FS, IW, R2, "__builtin_postrisc_fiwtos" },
    { insn_fuwtos,  FS, UW, R2, "__builtin_postrisc_fuwtos" },
    { insn_fidtos,  FS, ID, R2, "__builtin_postrisc_fidtos" },
    { insn_fudtos,  FS, UD, R2, "__builtin_postrisc_fudtos" },
    { insn_fiqtos,  FS, IQ, R2, "__builtin_postrisc_fiqtos" },
    { insn_fuqtos,  FS, UQ, R2, "__builtin_postrisc_fuqtos" },

    { insn_fdtoiw,  IW, FD, R2, "__builtin_postrisc_fdtoiw" },
    { insn_fdtouw,  UW, FD, R2, "__builtin_postrisc_fdtouw" },
    { insn_fdtoid,  ID, FD, R2, "__builtin_postrisc_fdtoid" },
    { insn_fdtoud,  UD, FD, R2, "__builtin_postrisc_fdtoud" },
    { insn_fdtoiq,  IQ, FD, R2, "__builtin_postrisc_fdtoiq" },
    { insn_fdtouq,  UQ, FD, R2, "__builtin_postrisc_fdtouq" },

    { insn_fiwtod,  FD, IW, R2, "__builtin_postrisc_fiwtod" },
    { insn_fuwtod,  FD, UW, R2, "__builtin_postrisc_fuwtod" },
    { insn_fidtod,  FD, ID, R2, "__builtin_postrisc_fidtod" },
    { insn_fudtod,  FD, UD, R2, "__builtin_postrisc_fudtod" },
    { insn_fiqtod,  FD, IQ, R2, "__builtin_postrisc_fiqtod" },
    { insn_fuqtod,  FD, UQ, R2, "__builtin_postrisc_fuqtod" },

    { insn_fqtoiw,  IW, FQ, R2, "__builtin_postrisc_fqtoiw" },
    { insn_fqtouw,  UW, FQ, R2, "__builtin_postrisc_fqtouw" },
    { insn_fqtoid,  ID, FQ, R2, "__builtin_postrisc_fqtoid" },
    { insn_fqtoud,  UD, FQ, R2, "__builtin_postrisc_fqtoud" },
    { insn_fqtoiq,  IQ, FQ, R2, "__builtin_postrisc_fqtoiq" },
    { insn_fqtouq,  UQ, FQ, R2, "__builtin_postrisc_fqtouq" },

    { insn_fiwtoq,  FQ, IW, R2, "__builtin_postrisc_fiwtoq" },
    { insn_fuwtoq,  FQ, UW, R2, "__builtin_postrisc_fuwtoq" },
    { insn_fidtoq,  FQ, ID, R2, "__builtin_postrisc_fidtoq" },
    { insn_fudtoq,  FQ, UD, R2, "__builtin_postrisc_fudtoq" },
    { insn_fiqtoq,  FQ, IQ, R2, "__builtin_postrisc_fiqtoq" },
    { insn_fuqtoq,  FQ, UQ, R2, "__builtin_postrisc_fuqtoq" },

    { insn_vfsqrth,  VFH, VFH, R2, "__builtin_postrisc_vfsqrth" },
    { insn_vfrsqrth, VFH, VFH, R2, "__builtin_postrisc_vfrsqrth" },
    { insn_vfrndh,   VFH, VFH, R2, "__builtin_postrisc_vfrndh" },
    { insn_vfrndxh,  VFH, VFH, R2, "__builtin_postrisc_vfrndxh" },

    { insn_vfsqrts,  VFW, VFW, R2, "__builtin_postrisc_vfsqrts" },
    { insn_vfrsqrts, VFW, VFW, R2, "__builtin_postrisc_vfrsqrts" },
    { insn_vfrnds,   VFW, VFW, R2, "__builtin_postrisc_vfrnds" },
    { insn_vfrndxs,  VFW, VFW, R2, "__builtin_postrisc_vfrndxs" },

    { insn_vfsqrtd,  VFD, VFD, R2, "__builtin_postrisc_vfsqrtd" },
    { insn_vfrsqrtd, VFD, VFD, R2, "__builtin_postrisc_vfrsqrtd" },
    { insn_vfrndd,   VFD, VFD, R2, "__builtin_postrisc_vfrndd" },
    { insn_vfrndxd,  VFD, VFD, R2, "__builtin_postrisc_vfrndxd" },

    { insn_vfhtoih,  VIH, VFH, R2, "__builtin_postrisc_vfhtoih" },
    { insn_vfhtouh,  VUH, VFH, R2, "__builtin_postrisc_vfhtouh" },
    { insn_vfihtoh,  VFH, VIH, R2, "__builtin_postrisc_vfihtoh" },
    { insn_vfuhtoh,  VFH, VUH, R2, "__builtin_postrisc_vfuhtoh" },

    { insn_vfstoiw,  VIW, VFW, R2, "__builtin_postrisc_vfstoiw" },
    { insn_vfstouw,  VUW, VFW, R2, "__builtin_postrisc_vfstouw" },
    { insn_vfiwtos,  VFW, VIW, R2, "__builtin_postrisc_vfiwtos" },
    { insn_vfuwtos,  VFW, VUW, R2, "__builtin_postrisc_vfuwtos" },

    { insn_vfdtoid,  VID, VFD, R2, "__builtin_postrisc_vfdtoid" },
    { insn_vfdtoud,  VUD, VFD, R2, "__builtin_postrisc_vfdtoud" },
    { insn_vfidtod,  VFD, VID, R2, "__builtin_postrisc_vfidtod" },
    { insn_vfudtod,  VFD, VUD, R2, "__builtin_postrisc_vfudtod" },
};

CODEGEN_HANDLER(FpUnaryRm)
{
    for (const auto& func : unary_rm_functions) {
        for (const auto& round : rounding_modes) {
            std::ostringstream fname;
            fname << "test_" << func.insn << "_" << RoundingMode2Str(round.rm)
                  << "_" << func.src_type << "_" << func.dst_type;

            out << CHECK_LABEL << fname.str() << '\n';
            out << CHECK << insn_alloc << " 2\n";
            out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args;
            if (RoundingMode::DYN != round.rm) {
                out << ", " <<  RoundingMode2Str(round.rm);
            }
            out << '\n';
            out << CHECK_NEXT << insn_retf << " 0\n";

            out << func.dst_type << " " << fname.str() << "(" << func.src_type << " x)\n";
            out << "{\n";
            out << "    return " << func.fragment << "(x, " << round.constant << ");\n";
            out << "}\n\n";
        }
    }
}

static const binary_info binary_functions[] = {
    { "add",               insn_faddh,    FH, FH, FH, R3_SWAP,  "a + b" },
    { "nadd",              insn_fnaddh,   FH, FH, FH, R3_SWAP,  "-(a + b)" },
    { "sub",               insn_fsubh,    FH, FH, FH, R3,       "a - b" },
    { "mul",               insn_fmulh,    FH, FH, FH, R3_SWAP,  "a * b" },
    { "nmul",              insn_fnmulh,   FH, FH, FH, R3_SWAP,  "- (a * b)" },
    { "div",               insn_fdivh,    FH, FH, FH, R3,       "a / b" },
    { "abs_diff",          insn_fabdh,    FH, FH, FH, R3,       "__builtin_fabsf16(a - b)" },
    { "nabs_diff",         insn_fnabdh,   FH, FH, FH, R3,       "- __builtin_fabsf16(a - b)" },
    { "copysign",          insn_fmergeh,  FH, FH, FH, R4_SIGN,  "__builtin_copysignf16(a, b)" },
    { "min_lt_abab",       insn_fminnmh,  FH, FH, FH, R3,       "a <  b ? a : b" },
    { "max_ge_abab",       insn_fmaxnmh,  FH, FH, FH, R3,       "a >= b ? a : b" },
    { "min_le_abab",       insn_fminnmh,  FH, FH, FH, R3,       "a <= b ? a : b" },
    { "max_gt_abab",       insn_fmaxnmh,  FH, FH, FH, R3,       "a >  b ? a : b" },
    { "max_lt_baab",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b <  a ? a : b" },
    { "min_ge_baab",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b >= a ? a : b" },
    { "max_le_baab",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b <= a ? a : b" },
    { "min_gt_baab",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b >  a ? a : b" },
    { "max_lt_abba",       insn_fmaxnmh,  FH, FH, FH, R3,       "a <  b ? b : a" },
    { "min_ge_abba",       insn_fminnmh,  FH, FH, FH, R3,       "a >= b ? b : a" },
    { "max_le_abba",       insn_fmaxnmh,  FH, FH, FH, R3,       "a <= b ? b : a" },
    { "min_gt_abba",       insn_fminnmh,  FH, FH, FH, R3,       "a >  b ? b : a" },
    { "min_lt_baba",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b <  a ? b : a" },
    { "max_ge_baba",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b >= a ? b : a" },
    { "min_le_baba",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b <= a ? b : a" },
    { "max_gt_baba",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b >  a ? b : a" },
    { "builtin_minimum",   insn_fminh,    FH, FH, FH, R3,       "__builtin_postrisc_fminimumh(a, b)" },
    { "builtin_maximum",   insn_fmaxh,    FH, FH, FH, R3,       "__builtin_postrisc_fmaximumh(a, b)" },
    { "min",               insn_fminnmh,  FH, FH, FH, R3,       "__builtin_fminf16(a, b)" },
    { "max",               insn_fmaxnmh,  FH, FH, FH, R3,       "__builtin_fmaxf16(a, b)" },
    { "fminimum_num",      insn_fminnmh,  FH, FH, FH, R3,       "__builtin_fminimum_numf16(a, b)" },
    { "fmaximum_num",      insn_fmaxnmh,  FH, FH, FH, R3,       "__builtin_fmaximum_numf16(a, b)" },
    { "builtin_abs_min",   insn_faminnmh, FH, FH, FH, R3,       "__builtin_fminf16(__builtin_fabsf16(a), __builtin_fabsf16(b))" },
    { "builtin_abs_max",   insn_famaxnmh, FH, FH, FH, R3,       "__builtin_fmaxf16(__builtin_fabsf16(a), __builtin_fabsf16(b))" },

    { "fcmpoeq",           insn_fcmpoeqh, BOOL, FH, FH, R3,       "a == b" },
    { "fcmpone",           insn_fcmponeh, BOOL, FH, FH, R3,       "a != b" },
    { "fcmpolt",           insn_fcmpolth, BOOL, FH, FH, R3,       "a < b" },
    { "fcmpole",           insn_fcmpogeh, BOOL, FH, FH, R3_SWAP,  "a <= b" },
    { "fcmpogt",           insn_fcmpolth, BOOL, FH, FH, R3_SWAP,  "a > b" },
    { "fcmpoge",           insn_fcmpogeh, BOOL, FH, FH, R3,       "a >= b" },

    { "add",               insn_fadds,      FS, FS, FS, R3_SWAP,  "a + b" },
    { "nadd",              insn_fnadds,     FS, FS, FS, R3_SWAP,  "-(a + b)" },
    { "sub",               insn_fsubs,      FS, FS, FS, R3,       "a - b" },
    { "mul",               insn_fmuls,      FS, FS, FS, R3_SWAP,  "a * b" },
    { "nmul",              insn_fnmuls,     FS, FS, FS, R3_SWAP,  "- (a * b)" },
    { "div",               insn_fdivs,      FS, FS, FS, R3,       "a / b" },
    { "abs_diff",          insn_fabds,      FS, FS, FS, R3,       "__builtin_fabsf(a - b)" },
    { "nabs_diff",         insn_fnabds,     FS, FS, FS, R3,       "- __builtin_fabsf(a - b)" },
    { "copysign",          insn_fmerges,    FS, FS, FS, R4_SIGN,  "__builtin_copysignf(a, b)" },
    { "fcmpoeq",           insn_fcmpoeqs, BOOL, FS, FS, R3,       "a == b" },
    { "fcmpone",           insn_fcmpones, BOOL, FS, FS, R3,       "a != b" },
    { "fcmpolt",           insn_fcmpolts, BOOL, FS, FS, R3,       "a < b" },
    { "fcmpole",           insn_fcmpoges, BOOL, FS, FS, R3_SWAP,  "a <= b" },
    { "fcmpogt",           insn_fcmpolts, BOOL, FS, FS, R3_SWAP,  "a > b" },
    { "fcmpoge",           insn_fcmpoges, BOOL, FS, FS, R3,       "a >= b" },
    { "min_lt_abab",       insn_fminnms,    FS, FS, FS, R3,       "a <  b ? a : b" },
    { "max_ge_abab",       insn_fmaxnms,  FS, FS, FS, R3,       "a >= b ? a : b" },
    { "min_le_abab",       insn_fminnms,  FS, FS, FS, R3,       "a <= b ? a : b" },
    { "max_gt_abab",       insn_fmaxnms,  FS, FS, FS, R3,       "a >  b ? a : b" },
    { "max_lt_baab",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b <  a ? a : b" },
    { "min_ge_baab",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b >= a ? a : b" },
    { "max_le_baab",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b <= a ? a : b" },
    { "min_gt_baab",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b >  a ? a : b" },
    { "max_lt_abba",       insn_fmaxnms,  FS, FS, FS, R3,       "a <  b ? b : a" },
    { "min_ge_abba",       insn_fminnms,  FS, FS, FS, R3,       "a >= b ? b : a" },
    { "max_le_abba",       insn_fmaxnms,  FS, FS, FS, R3,       "a <= b ? b : a" },
    { "min_gt_abba",       insn_fminnms,  FS, FS, FS, R3,       "a >  b ? b : a" },
    { "min_lt_baba",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b <  a ? b : a" },
    { "max_ge_baba",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b >= a ? b : a" },
    { "min_le_baba",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b <= a ? b : a" },
    { "max_gt_baba",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b >  a ? b : a" },
    { "builtin_minimum",   insn_fmins,     FS, FS, FS, R3,       "__builtin_postrisc_fminimums(a, b)" },
    { "builtin_maximum",   insn_fmaxs,     FS, FS, FS, R3,       "__builtin_postrisc_fmaximums(a, b)" },
    { "min",               insn_fminnms,  FS, FS, FS, R3,       "__builtin_fminf(a, b)" },
    { "max",               insn_fmaxnms,  FS, FS, FS, R3,       "__builtin_fmaxf(a, b)" },
    { "fminimum_num",      insn_fminnms,  FS, FS, FS, R3,       "__builtin_fminimum_numf(a, b)" },
    { "fmaximum_num",      insn_fmaxnms,  FS, FS, FS, R3,       "__builtin_fmaximum_numf(a, b)" },
    { "builtin_abs_min",   insn_faminnms, FS, FS, FS, R3,       "__builtin_fminf(__builtin_fabsf(a), __builtin_fabsf(b))" },
    { "builtin_abs_max",   insn_famaxnms, FS, FS, FS, R3,       "__builtin_fmaxf(__builtin_fabsf(a), __builtin_fabsf(b))" },

    { "add",               insn_faddd,      FD, FD, FD, R3_SWAP,  "a + b" },
    { "nadd",              insn_fnaddd,     FD, FD, FD, R3_SWAP,  "-(a + b)" },
    { "sub",               insn_fsubd,      FD, FD, FD, R3,       "a - b" },
    { "mul",               insn_fmuld,      FD, FD, FD, R3_SWAP,  "a * b" },
    { "nmul",              insn_fnmuld,     FD, FD, FD, R3_SWAP,  "- (a * b)" },
    { "div",               insn_fdivd,      FD, FD, FD, R3,       "a / b" },
    { "abs_diff",          insn_fabdd,      FD, FD, FD, R3,       "__builtin_fabs(a - b)" },
    { "nabs_diff",         insn_fnabdd,     FD, FD, FD, R3,       "- __builtin_fabs(a - b)" },
    { "copysign",          insn_fmerged,    FD, FD, FD, R4_SIGN,  "__builtin_copysign(a, b)" },
    { "fcmpoeq",           insn_fcmpoeqd, BOOL, FD, FD, R3,       "a == b" },
    { "fcmpone",           insn_fcmponed, BOOL, FD, FD, R3,       "a != b" },
    { "fcmpolt",           insn_fcmpoltd, BOOL, FD, FD, R3,       "a < b" },
    { "fcmpole",           insn_fcmpoged, BOOL, FD, FD, R3_SWAP,  "a <= b" },
    { "fcmpogt",           insn_fcmpoltd, BOOL, FD, FD, R3_SWAP,  "a > b" },
    { "fcmpoge",           insn_fcmpoged, BOOL, FD, FD, R3,       "a >= b" },
    { "min_lt_abab",       insn_fminnmd,   FD, FD, FD, R3,       "a <  b ? a : b" },
    { "max_ge_abab",       insn_fmaxnmd,   FD, FD, FD, R3,       "a >= b ? a : b" },
    { "min_le_abab",       insn_fminnmd,   FD, FD, FD, R3,       "a <= b ? a : b" },
    { "max_gt_abab",       insn_fmaxnmd,   FD, FD, FD, R3,       "a >  b ? a : b" },
    { "max_lt_baab",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b <  a ? a : b" },
    { "min_ge_baab",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b >= a ? a : b" },
    { "max_le_baab",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b <= a ? a : b" },
    { "min_gt_baab",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b >  a ? a : b" },
    { "max_lt_abba",       insn_fmaxnmd,   FD, FD, FD, R3,       "a <  b ? b : a" },
    { "min_ge_abba",       insn_fminnmd,   FD, FD, FD, R3,       "a >= b ? b : a" },
    { "max_le_abba",       insn_fmaxnmd,   FD, FD, FD, R3,       "a <= b ? b : a" },
    { "min_gt_abba",       insn_fminnmd,   FD, FD, FD, R3,       "a >  b ? b : a" },
    { "min_lt_baba",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b <  a ? b : a" },
    { "max_ge_baba",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b >= a ? b : a" },
    { "min_le_baba",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b <= a ? b : a" },
    { "max_gt_baba",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b >  a ? b : a" },
    { "builtin_minimum",   insn_fmind,     FD, FD, FD, R3,       "__builtin_postrisc_fminimumd(a, b)" },
    { "builtin_maximum",   insn_fmaxd,     FD, FD, FD, R3,       "__builtin_postrisc_fmaximumd(a, b)" },
    { "min",               insn_fminnmd,   FD, FD, FD, R3,       "__builtin_fmin(a, b)" },
    { "max",               insn_fmaxnmd,   FD, FD, FD, R3,       "__builtin_fmax(a, b)" },
    { "fminimum_num",      insn_fminnmd,   FD, FD, FD, R3,       "__builtin_fminimum_num(a, b)" },
    { "fmaximum_num",      insn_fmaxnmd,   FD, FD, FD, R3,       "__builtin_fmaximum_num(a, b)" },
    { "builtin_abs_min",   insn_faminnmd,  FD, FD, FD, R3,       "__builtin_fmin(__builtin_fabs(a), __builtin_fabs(b))" },
    { "builtin_abs_max",   insn_famaxnmd,  FD, FD, FD, R3,       "__builtin_fmax(__builtin_fabs(a), __builtin_fabs(b))" },

    { "add",               insn_faddq,      FQ, FQ, FQ, R3_SWAP,  "a + b" },
    { "nadd",              insn_fnaddq,     FQ, FQ, FQ, R3_SWAP,  "-(a + b)" },
    { "sub",               insn_fsubq,      FQ, FQ, FQ, R3,       "a - b" },
    { "mul",               insn_fmulq,      FQ, FQ, FQ, R3_SWAP,  "a * b" },
    { "nmul",              insn_fnmulq,     FQ, FQ, FQ, R3_SWAP,  "- (a * b)" },
    { "div",               insn_fdivq,      FQ, FQ, FQ, R3,       "a / b" },
    { "abs_diff",          insn_fabdq,      FQ, FQ, FQ, R3,       "__builtin_fabsl(a - b)" },
    { "nabs_diff",         insn_fnabdq,     FQ, FQ, FQ, R3,       "- __builtin_fabsl(a - b)" },
    { "copysign",          insn_fmergeq,    FQ, FQ, FQ, R4_SIGN,  "__builtin_copysignl(a, b)" },
    { "fcmpoeq",           insn_fcmpoeqq, BOOL, FQ, FQ, R3,       "a == b" },
    { "fcmpone",           insn_fcmponeq, BOOL, FQ, FQ, R3,       "a != b" },
    { "fcmpolt",           insn_fcmpoltq, BOOL, FQ, FQ, R3,       "a < b" },
    { "fcmpole",           insn_fcmpogeq, BOOL, FQ, FQ, R3_SWAP,  "a <= b" },
    { "fcmpogt",           insn_fcmpoltq, BOOL, FQ, FQ, R3_SWAP,  "a > b" },
    { "fcmpoge",           insn_fcmpogeq, BOOL, FQ, FQ, R3,       "a >= b" },
    { "min_lt_abab",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a <  b ? a : b" },
    { "max_ge_abab",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a >= b ? a : b" },
    { "min_le_abab",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a <= b ? a : b" },
    { "max_gt_abab",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a >  b ? a : b" },
    { "max_lt_baab",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b <  a ? a : b" },
    { "min_ge_baab",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b >= a ? a : b" },
    { "max_le_baab",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b <= a ? a : b" },
    { "min_gt_baab",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b >  a ? a : b" },
    { "max_lt_abba",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a <  b ? b : a" },
    { "min_ge_abba",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a >= b ? b : a" },
    { "max_le_abba",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a <= b ? b : a" },
    { "min_gt_abba",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a >  b ? b : a" },
    { "min_lt_baba",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b <  a ? b : a" },
    { "max_ge_baba",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b >= a ? b : a" },
    { "min_le_baba",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b <= a ? b : a" },
    { "max_gt_baba",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b >  a ? b : a" },
    { "builtin_minimum",   insn_fminq,    FQ, FQ, FQ, R3,       "__builtin_postrisc_fminimumq(a, b)" },
    { "builtin_maximum",   insn_fmaxq,    FQ, FQ, FQ, R3,       "__builtin_postrisc_fmaximumq(a, b)" },
    { "min",               insn_fminnmq,  FQ, FQ, FQ, R3,       "__builtin_fminl(a, b)" },
    { "max",               insn_fmaxnmq,  FQ, FQ, FQ, R3,       "__builtin_fmaxl(a, b)" },
    { "fminimum_num",      insn_fminnmq,  FQ, FQ, FQ, R3,       "__builtin_fminimum_numl(a, b)" },
    { "fmaximum_num",      insn_fmaxnmq,  FQ, FQ, FQ, R3,       "__builtin_fmaximum_numl(a, b)" },
    { "builtin_abs_min",   insn_faminnmq, FQ, FQ, FQ, R3,       "__builtin_fminl(__builtin_fabsl(a), __builtin_fabsl(b))" },
    { "builtin_abs_max",   insn_famaxnmq, FQ, FQ, FQ, R3,       "__builtin_fmaxl(__builtin_fabsl(a), __builtin_fabsl(b))" },

    { "add",               insn_faddq,      FX, FX, FX, R3_SWAP,  "a + b" },
    { "nadd",              insn_fnaddq,     FX, FX, FX, R3_SWAP,  "-(a + b)" },
    { "sub",               insn_fsubq,      FX, FX, FX, R3,       "a - b" },
    { "mul",               insn_fmulq,      FX, FX, FX, R3_SWAP,  "a * b" },
    { "nmul",              insn_fnmulq,     FX, FX, FX, R3_SWAP,  "- (a * b)" },
    { "div",               insn_fdivq,      FX, FX, FX, R3,       "a / b" },
    { "abs_diff",          insn_fabdq,      FX, FX, FX, R3,       "__builtin_fabsf128(a - b)" },
    { "nabs_diff",         insn_fnabdq,     FX, FX, FX, R3,       "- __builtin_fabsf128(a - b)" },
    { "copysign",          insn_fmergeq,    FX, FX, FX, R4_SIGN,  "__builtin_copysignf128(a, b)" },
    { "fcmpoeq",           insn_fcmpoeqq, BOOL, FX, FX, R3,       "a == b" },
    { "fcmpone",           insn_fcmponeq, BOOL, FX, FX, R3,       "a != b" },
    { "fcmpolt",           insn_fcmpoltq, BOOL, FX, FX, R3,       "a < b" },
    { "fcmpole",           insn_fcmpogeq, BOOL, FX, FX, R3_SWAP,  "a <= b" },
    { "fcmpogt",           insn_fcmpoltq, BOOL, FX, FX, R3_SWAP,  "a > b" },
    { "fcmpoge",           insn_fcmpogeq, BOOL, FX, FX, R3,       "a >= b" },
    { "min_lt_abab",       insn_fminnmq,  FX, FX, FX, R3,       "a <  b ? a : b" },
    { "max_ge_abab",       insn_fmaxnmq,  FX, FX, FX, R3,       "a >= b ? a : b" },
    { "min_le_abab",       insn_fminnmq,  FX, FX, FX, R3,       "a <= b ? a : b" },
    { "max_gt_abab",       insn_fmaxnmq,  FX, FX, FX, R3,       "a >  b ? a : b" },
    { "max_lt_baab",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b <  a ? a : b" },
    { "min_ge_baab",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b >= a ? a : b" },
    { "max_le_baab",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b <= a ? a : b" },
    { "min_gt_baab",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b >  a ? a : b" },
    { "max_lt_abba",       insn_fmaxnmq,  FX, FX, FX, R3,       "a <  b ? b : a" },
    { "min_ge_abba",       insn_fminnmq,  FX, FX, FX, R3,       "a >= b ? b : a" },
    { "max_le_abba",       insn_fmaxnmq,  FX, FX, FX, R3,       "a <= b ? b : a" },
    { "min_gt_abba",       insn_fminnmq,  FX, FX, FX, R3,       "a >  b ? b : a" },
    { "min_lt_baba",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b <  a ? b : a" },
    { "max_ge_baba",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b >= a ? b : a" },
    { "min_le_baba",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b <= a ? b : a" },
    { "max_gt_baba",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b >  a ? b : a" },
    { "builtin_minimum",   insn_fminq,    FX, FX, FX, R3,       "__builtin_postrisc_fminimumq(a, b)" },
    { "builtin_maximum",   insn_fmaxq,    FX, FX, FX, R3,       "__builtin_postrisc_fmaximumq(a, b)" },
    { "min",               insn_fminnmq,  FX, FX, FX, R3,       "__builtin_fminf128(a, b)" },
    { "max",               insn_fmaxnmq,  FX, FX, FX, R3,       "__builtin_fmaxf128(a, b)" },
    { "fminimum_num",      insn_fminnmq,  FX, FX, FX, R3,       "__builtin_fminimum_numf128(a, b)" },
    { "fmaximum_num",      insn_fmaxnmq,  FX, FX, FX, R3,       "__builtin_fmaximum_numf128(a, b)" },
    { "builtin_abs_min",   insn_faminnmq, FX, FX, FX, R3,       "__builtin_fminf128(__builtin_fabsf128(a), __builtin_fabsf128(b))" },
    { "builtin_abs_max",   insn_famaxnmq, FX, FX, FX, R3,       "__builtin_fmaxf128(__builtin_fabsf128(a), __builtin_fabsf128(b))" },

    { "add",                 insn_vfaddh,     VFH, VFH, VFH, R3_SWAP, "a + b" },
    { "nadd",                insn_vfnaddh,    VFH, VFH, VFH, R3_SWAP, "-(a + b)" },
    { "sub",                 insn_vfsubh,     VFH, VFH, VFH, R3,      "a - b" },
    { "mul",                 insn_vfmulh,     VFH, VFH, VFH, R3_SWAP, "a * b" },
    { "nmul",                insn_vfnmulh,    VFH, VFH, VFH, R3_SWAP, "-(a * b)" },
    { "div",                 insn_vfdivh,     VFH, VFH, VFH, R3,      "a / b" },
    { "abs_diff",            insn_vfabdh,     VFH, VFH, VFH, R3,      "__builtin_elementwise_abs(a - b)" },
    { "nabs_diff",           insn_vfnabdh,    VFH, VFH, VFH, R3,      "- __builtin_elementwise_abs(a - b)" },
    { "vector_abs_max",      insn_vfamaxnmh,  VFH, VFH, VFH, R3,      "__builtin_elementwise_maxnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" },
    { "vector_abs_min",      insn_vfaminnmh,  VFH, VFH, VFH, R3,      "__builtin_elementwise_minnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" },
    { "vector_copysign",     insn_vfmergeh,   VFH, VFH, VFH, R4_SIGN, "__builtin_elementwise_copysign(a, b)" },
    { "vector_minimum",      insn_vfminh,     VFH, VFH, VFH, R3,      "__builtin_elementwise_minimum(a, b)" },
    { "vector_maximum",      insn_vfmaxh,     VFH, VFH, VFH, R3,      "__builtin_elementwise_maximum(a, b)" },
    { "vector_max",          insn_vfmaxnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_max(a, b)" },
    { "vector_min",          insn_vfminnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_min(a, b)" },
    { "vector_minnum",       insn_vfminnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_minnum(a, b)" },
    { "vector_maxnum",       insn_vfmaxnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_maxnum(a, b)" },
    { "vector_minimumnum",   insn_vfminnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_minimumnum(a, b)" },
    { "vector_maximumnum",   insn_vfmaxnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_maximumnum(a, b)" },
    { "builtin_cmpoeq",      insn_vfcmpoeqh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpoeqh(a, b)" },
    { "builtin_cmpone",      insn_vfcmponeh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmponeh(a, b)" },
    { "builtin_cmpolt",      insn_vfcmpolth,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpolth(a, b)" },
    { "builtin_cmpoge",      insn_vfcmpogeh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpogeh(a, b)" },
    { "builtin_cmpo",        insn_vfcmpoh,    VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpoh(a, b)" },
    { "builtin_cmpueq",      insn_vfcmpueqh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpueqh(a, b)" },
    { "builtin_cmpune",      insn_vfcmpuneh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpuneh(a, b)" },
    { "builtin_cmpult",      insn_vfcmpulth,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpulth(a, b)" },
    { "builtin_cmpuge",      insn_vfcmpugeh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpugeh(a, b)" },
    { "builtin_cmpu",        insn_vfcmpuh,    VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpuh(a, b)" },
    { "builtin_merge_low",   insn_vfmergelh,  VFH, VFH, VFH, R3,      "__builtin_postrisc_vfmergelh(a, b)" },
    { "builtin_merge_high",  insn_vfmergehh,  VFH, VFH, VFH, R3,      "__builtin_postrisc_vfmergehh(a, b)" },

    { "add",                 insn_vfadds,     VFW, VFW, VFW, R3_SWAP, "a + b" },
    { "nadd",                insn_vfnadds,    VFW, VFW, VFW, R3_SWAP, "-(a + b)" },
    { "sub",                 insn_vfsubs,     VFW, VFW, VFW, R3,      "a - b" },
    { "mul",                 insn_vfmuls,     VFW, VFW, VFW, R3_SWAP, "a * b" },
    { "nmul",                insn_vfnmuls,    VFW, VFW, VFW, R3_SWAP, "-(a * b)" },
    { "div",                 insn_vfdivs,     VFW, VFW, VFW, R3,      "a / b" },
    { "abs_diff",            insn_vfabds,     VFW, VFW, VFW, R3,      "__builtin_elementwise_abs(a - b)" },
    { "nabs_diff",           insn_vfnabds,    VFW, VFW, VFW, R3,      "- __builtin_elementwise_abs(a - b)" },
    { "vector_abs_max",      insn_vfamaxnms,  VFW, VFW, VFW, R3,      "__builtin_elementwise_maxnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" },
    { "vector_abs_min",      insn_vfaminnms,  VFW, VFW, VFW, R3,      "__builtin_elementwise_minnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" },
    { "vector_copysign",     insn_vfmerges,   VFW, VFW, VFW, R4_SIGN, "__builtin_elementwise_copysign(a, b)" },
    { "vector_minimum",      insn_vfmins,     VFW, VFW, VFW, R3,      "__builtin_elementwise_minimum(a, b)" },
    { "vector_maximum",      insn_vfmaxs,     VFW, VFW, VFW, R3,      "__builtin_elementwise_maximum(a, b)" },
    { "vector_max",          insn_vfmaxnms,   VFW, VFW, VFW, R3,      "__builtin_elementwise_max(a, b)" },
    { "vector_min",          insn_vfminnms,   VFW, VFW, VFW, R3,      "__builtin_elementwise_min(a, b)" },
    { "vector_minnum",       insn_vfminnms,   VFW, VFW, VFW, R3,      "__builtin_elementwise_minnum(a, b)" },
    { "vector_maxnum",       insn_vfmaxnms,   VFW, VFW, VFW, R3,      "__builtin_elementwise_maxnum(a, b)" },
    { "vector_minimumnum",   insn_vfminnms,   VFW, VFW, VFW, R3,      "__builtin_elementwise_minimumnum(a, b)" },
    { "vector_maximumnum",   insn_vfmaxnms,   VFW, VFW, VFW, R3,      "__builtin_elementwise_maximumnum(a, b)" },
    { "builtin_cmpoeq",      insn_vfcmpoeqs,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpoeqs(a, b)" },
    { "builtin_cmpone",      insn_vfcmpones,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpones(a, b)" },
    { "builtin_cmpolt",      insn_vfcmpolts,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpolts(a, b)" },
    { "builtin_cmpoge",      insn_vfcmpoges,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpoges(a, b)" },
    { "builtin_cmpo",        insn_vfcmpos,    VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpos(a, b)" },
    { "builtin_cmpueq",      insn_vfcmpueqs,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpueqs(a, b)" },
    { "builtin_cmpune",      insn_vfcmpunes,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpunes(a, b)" },
    { "builtin_cmpult",      insn_vfcmpults,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpults(a, b)" },
    { "builtin_cmpuge",      insn_vfcmpuges,  VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpuges(a, b)" },
    { "builtin_cmpu",        insn_vfcmpus,    VIW, VFW, VFW, R3,      "__builtin_postrisc_vfcmpus(a, b)" },
    { "builtin_merge_low",   insn_vfmergels,  VFW, VFW, VFW, R3,      "__builtin_postrisc_vfmergels(a, b)" },
    { "builtin_merge_high",  insn_vfmergehs,  VFW, VFW, VFW, R3,      "__builtin_postrisc_vfmergehs(a, b)" },

    { "add",                 insn_vfaddd,     VFD, VFD, VFD, R3_SWAP, "a + b" },
    { "nadd",                insn_vfnaddd,    VFD, VFD, VFD, R3_SWAP, "-(a + b)" },
    { "sub",                 insn_vfsubd,     VFD, VFD, VFD, R3,      "a - b" },
    { "mul",                 insn_vfmuld,     VFD, VFD, VFD, R3_SWAP, "a * b" },
    { "nmul",                insn_vfnmuld,    VFD, VFD, VFD, R3_SWAP, "-(a * b)" },
    { "div",                 insn_vfdivd,     VFD, VFD, VFD, R3,      "a / b" },
    { "abs_diff",            insn_vfabdd,     VFD, VFD, VFD, R3,      "__builtin_elementwise_abs(a - b)" },
    { "nabs_diff",           insn_vfnabdd,    VFD, VFD, VFD, R3,      "- __builtin_elementwise_abs(a - b)" },
    { "vector_abs_max",      insn_vfamaxnmd,  VFD, VFD, VFD, R3,      "__builtin_elementwise_maxnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" },
    { "vector_abs_min",      insn_vfaminnmd,  VFD, VFD, VFD, R3,      "__builtin_elementwise_minnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))" },
    { "vector_copysign",     insn_vfmerged,   VFD, VFD, VFD, R4_SIGN, "__builtin_elementwise_copysign(a, b)" },
    { "vector_minimum",      insn_vfmind,     VFD, VFD, VFD, R3,      "__builtin_elementwise_minimum(a, b)" },
    { "vector_maximum",      insn_vfmaxd,     VFD, VFD, VFD, R3,      "__builtin_elementwise_maximum(a, b)" },
    { "vector_max",          insn_vfmaxnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_max(a, b)" },
    { "vector_min",          insn_vfminnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_min(a, b)" },
    { "vector_minnum",       insn_vfminnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_minnum(a, b)" },
    { "vector_maxnum",       insn_vfmaxnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_maxnum(a, b)" },
    { "vector_minimumnum",   insn_vfminnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_minimumnum(a, b)" },
    { "vector_maximumnum",   insn_vfmaxnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_maximumnum(a, b)" },
    { "builtin_cmpoeq",      insn_vfcmpoeqd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpoeqd(a, b)" },
    { "builtin_cmpone",      insn_vfcmponed,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmponed(a, b)" },
    { "builtin_cmpolt",      insn_vfcmpoltd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpoltd(a, b)" },
    { "builtin_cmpoge",      insn_vfcmpoged,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpoged(a, b)" },
    { "builtin_cmpo",        insn_vfcmpod,    VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpod(a, b)" },
    { "builtin_cmpueq",      insn_vfcmpueqd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpueqd(a, b)" },
    { "builtin_cmpune",      insn_vfcmpuned,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpuned(a, b)" },
    { "builtin_cmpult",      insn_vfcmpultd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpultd(a, b)" },
    { "builtin_cmpuge",      insn_vfcmpuged,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpuged(a, b)" },
    { "builtin_cmpu",        insn_vfcmpud,    VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpud(a, b)" },
    { "builtin_merge_low",   insn_vfmergeld,  VFD, VFD, VFD, R3,      "__builtin_postrisc_vfmergeld(a, b)" },
    { "builtin_merge_high",  insn_vfmergehd,  VFD, VFD, VFD, R3,      "__builtin_postrisc_vfmergehd(a, b)" },
};

CODEGEN_HANDLER(FpBinary)
{
    for (const auto& func : binary_functions) {
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

static const struct {
    EInstructionID insn;
    ValueType dst_type;
    ValueType src1_type;
    ValueType src2_type;
    char args[32];
    char fragment[64];
} binary_rm_functions[] = {
    { insn_fabdh,    FH,  FH,  FH,  R3, "__builtin_postrisc_fabdh" },
    { insn_fnabdh,   FH,  FH,  FH,  R3, "__builtin_postrisc_fnabdh" },
    { insn_faddh,    FH,  FH,  FH,  R3, "__builtin_postrisc_faddh" },
    { insn_fnaddh,   FH,  FH,  FH,  R3, "__builtin_postrisc_fnaddh" },
    { insn_fsubh,    FH,  FH,  FH,  R3, "__builtin_postrisc_fsubh" },
    { insn_fmulh,    FH,  FH,  FH,  R3, "__builtin_postrisc_fmulh" },
    { insn_fnmulh,   FH,  FH,  FH,  R3, "__builtin_postrisc_fnmulh" },
    { insn_fdivh,    FH,  FH,  FH,  R3, "__builtin_postrisc_fdivh" },

    { insn_fabds,    FS,  FS,  FS,  R3, "__builtin_postrisc_fabds" },
    { insn_fnabds,   FS,  FS,  FS,  R3, "__builtin_postrisc_fnabds" },
    { insn_fadds,    FS,  FS,  FS,  R3, "__builtin_postrisc_fadds" },
    { insn_fnadds,   FS,  FS,  FS,  R3, "__builtin_postrisc_fnadds" },
    { insn_fsubs,    FS,  FS,  FS,  R3, "__builtin_postrisc_fsubs" },
    { insn_fmuls,    FS,  FS,  FS,  R3, "__builtin_postrisc_fmuls" },
    { insn_fnmuls,   FS,  FS,  FS,  R3, "__builtin_postrisc_fnmuls" },
    { insn_fdivs,    FS,  FS,  FS,  R3, "__builtin_postrisc_fdivs" },

    { insn_fabdd,    FD,  FD,  FD,  R3, "__builtin_postrisc_fabdd" },
    { insn_fnabdd,   FD,  FD,  FD,  R3, "__builtin_postrisc_fnabdd" },
    { insn_faddd,    FD,  FD,  FD,  R3, "__builtin_postrisc_faddd" },
    { insn_fnaddd,   FD,  FD,  FD,  R3, "__builtin_postrisc_fnaddd" },
    { insn_fsubd,    FD,  FD,  FD,  R3, "__builtin_postrisc_fsubd" },
    { insn_fmuld,    FD,  FD,  FD,  R3, "__builtin_postrisc_fmuld" },
    { insn_fnmuld,   FD,  FD,  FD,  R3, "__builtin_postrisc_fnmuld" },
    { insn_fdivd,    FD,  FD,  FD,  R3, "__builtin_postrisc_fdivd" },

    { insn_fabdq,    FQ,  FQ,  FQ,  R3, "__builtin_postrisc_fabdq" },
    { insn_fnabdq,   FQ,  FQ,  FQ,  R3, "__builtin_postrisc_fnabdq" },
    { insn_faddq,    FQ,  FQ,  FQ,  R3, "__builtin_postrisc_faddq" },
    { insn_fnaddq,   FQ,  FQ,  FQ,  R3, "__builtin_postrisc_fnaddq" },
    { insn_fsubq,    FQ,  FQ,  FQ,  R3, "__builtin_postrisc_fsubq" },
    { insn_fmulq,    FQ,  FQ,  FQ,  R3, "__builtin_postrisc_fmulq" },
    { insn_fnmulq,   FQ,  FQ,  FQ,  R3, "__builtin_postrisc_fnmulq" },
    { insn_fdivq,    FQ,  FQ,  FQ,  R3, "__builtin_postrisc_fdivq" },

    { insn_vfabdh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfabdh" },
    { insn_vfnabdh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfnabdh" },
    { insn_vfaddh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfaddh" },
    { insn_vfnaddh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfnaddh" },
    { insn_vfsubh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfsubh" },
    { insn_vfmulh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfmulh" },
    { insn_vfnmulh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfnmulh" },
    { insn_vfdivh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfdivh" },
    { insn_vfaddsubh,  VFH, VFH, VFH, R3, "__builtin_postrisc_vfaddsubh" },
    { insn_vfsubaddh,  VFH, VFH, VFH, R3, "__builtin_postrisc_vfsubaddh" },
    { insn_vfhaddh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfhaddh" },
    { insn_vfhsubh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfhsubh" },
    { insn_vfhmulh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfhmulh" },
    { insn_vfdoth,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfdoth" },

    { insn_vfabds,     VFW, VFW, VFW, R3, "__builtin_postrisc_vfabds" },
    { insn_vfnabds,    VFW, VFW, VFW, R3, "__builtin_postrisc_vfnabds" },
    { insn_vfadds,     VFW, VFW, VFW, R3, "__builtin_postrisc_vfadds" },
    { insn_vfnadds,    VFW, VFW, VFW, R3, "__builtin_postrisc_vfnadds" },
    { insn_vfsubs,     VFW, VFW, VFW, R3, "__builtin_postrisc_vfsubs" },
    { insn_vfmuls,     VFW, VFW, VFW, R3, "__builtin_postrisc_vfmuls" },
    { insn_vfnmuls,    VFW, VFW, VFW, R3, "__builtin_postrisc_vfnmuls" },
    { insn_vfdivs,     VFW, VFW, VFW, R3, "__builtin_postrisc_vfdivs" },
    { insn_vfaddsubs,  VFW, VFW, VFW, R3, "__builtin_postrisc_vfaddsubs" },
    { insn_vfsubadds,  VFW, VFW, VFW, R3, "__builtin_postrisc_vfsubadds" },
    { insn_vfhadds,    VFW, VFW, VFW, R3, "__builtin_postrisc_vfhadds" },
    { insn_vfhsubs,    VFW, VFW, VFW, R3, "__builtin_postrisc_vfhsubs" },
    { insn_vfhmuls,    VFW, VFW, VFW, R3, "__builtin_postrisc_vfhmuls" },
    { insn_vfdots,     VFW, VFW, VFW, R3, "__builtin_postrisc_vfdots" },

    { insn_vfabdd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfabdd" },
    { insn_vfnabdd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfnabdd" },
    { insn_vfaddd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfaddd" },
    { insn_vfnaddd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfnaddd" },
    { insn_vfsubd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfsubd" },
    { insn_vfmuld,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfmuld" },
    { insn_vfnmuld,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfnmuld" },
    { insn_vfdivd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfdivd" },
    { insn_vfaddsubd,  VFD, VFD, VFD, R3, "__builtin_postrisc_vfaddsubd" },
    { insn_vfsubaddd,  VFD, VFD, VFD, R3, "__builtin_postrisc_vfsubaddd" },
    { insn_vfhaddd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfhaddd" },
    { insn_vfhsubd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfhsubd" },
    { insn_vfhmuld,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfhmuld" },
    { insn_vfdotd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfdotd" },
};

CODEGEN_HANDLER(FpBinaryRm)
{
    for (const auto& func : binary_rm_functions) {
        for (const auto& round : rounding_modes) {
            std::ostringstream fname;
            fname << "test_" << func.insn << "_" << RoundingMode2Str(round.rm)
                   << "_" << func.src1_type << "_" << func.src2_type << "_" << func.dst_type;

            out << CHECK_LABEL << fname.str() << '\n';
            out << CHECK << insn_alloc << " 3\n";
            out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args;
            if (RoundingMode::DYN != round.rm) {
                out << ", " <<  RoundingMode2Str(round.rm);
            }
            out << '\n';
            out << CHECK_NEXT << insn_retf << " 0\n";

            out << func.dst_type << " " << fname.str() << "(" << func.src1_type << " a, " << func.src2_type << " b)\n";
            out << "{\n";
            out << "    return " << func.fragment << "(a, b, " << round.constant << ");\n";
            out << "}\n\n";
        }
    }
}

static const ternary_info ternary_functions[] = {
    { "fma",            insn_fmaddh,   FH, FH, FH, FH, R4, "__builtin_fmaf16(a, b, c)" },
    { "fma_nega",       insn_fnmaddh,  FH, FH, FH, FH, R4, "__builtin_fmaf16(-a, b, c)" },
    { "fma_negc",       insn_fmsubh,   FH, FH, FH, FH, R4, "__builtin_fmaf16(a, b, -c)" },
    { "fma_nega_negc",  insn_fnmsubh,  FH, FH, FH, FH, R4, "__builtin_fmaf16(-a, b, -c)" },

    { "fma",            insn_fmadds,   FS, FS, FS, FS, R4, "__builtin_fmaf(a, b, c)" },
    { "fma_nega",       insn_fnmadds,  FS, FS, FS, FS, R4, "__builtin_fmaf(-a, b, c)" },
    { "fma_negc",       insn_fmsubs,   FS, FS, FS, FS, R4, "__builtin_fmaf(a, b, -c)" },
    { "fma_nega_negc",  insn_fnmsubs,  FS, FS, FS, FS, R4, "__builtin_fmaf(-a, b, -c)" },

    { "fma",            insn_fmaddd,   FD, FD, FD, FD, R4, "__builtin_fma(a, b, c)" },
    { "fma_nega",       insn_fnmaddd,  FD, FD, FD, FD, R4, "__builtin_fma(-a, b, c)" },
    { "fma_negc",       insn_fmsubd,   FD, FD, FD, FD, R4, "__builtin_fma(a, b, -c)" },
    { "fma_nega_negc",  insn_fnmsubd,  FD, FD, FD, FD, R4, "__builtin_fma(-a, b, -c)" },

    { "fma",            insn_fmaddq,  FQ, FQ, FQ, FQ, R4, "__builtin_fmal(a, b, c)" },
    { "fma_nega",       insn_fnmaddq, FQ, FQ, FQ, FQ, R4, "__builtin_fmal(-a, b, c)" },
    { "fma_negc",       insn_fmsubq,  FQ, FQ, FQ, FQ, R4, "__builtin_fmal(a, b, -c)" },
    { "fma_nega_negc",  insn_fnmsubq, FQ, FQ, FQ, FQ, R4, "__builtin_fmal(-a, b, -c)" },

    { "fma",            insn_fmaddq,  FX, FX, FX, FX, R4, "__builtin_fmaf128(a, b, c)" },
    { "fma_nega",       insn_fnmaddq, FX, FX, FX, FX, R4, "__builtin_fmaf128(-a, b, c)" },
    { "fma_negc",       insn_fmsubq,  FX, FX, FX, FX, R4, "__builtin_fmaf128(a, b, -c)" },
    { "fma_nega_negc",  insn_fnmsubq, FX, FX, FX, FX, R4, "__builtin_fmaf128(-a, b, -c)" },

    { "vector_madd",   insn_vfmaddh,  VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(a, b, c)" },
    { "vector_msub",   insn_vfmsubh,  VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(a, b, -c)" },
    { "vector_nmadd",  insn_vfnmaddh, VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(-a, b, c)" },
    { "vector_nmsub",  insn_vfnmsubh, VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(-a, b, -c)" },

    { "vector_madd",   insn_vfmadds,  VFW, VFW, VFW, VFW, R4, "__builtin_elementwise_fma(a, b, c)" },
    { "vector_msub",   insn_vfmsubs,  VFW, VFW, VFW, VFW, R4, "__builtin_elementwise_fma(a, b, -c)" },
    { "vector_nmadd",  insn_vfnmadds, VFW, VFW, VFW, VFW, R4, "__builtin_elementwise_fma(-a, b, c)" },
    { "vector_nmsub",  insn_vfnmsubs, VFW, VFW, VFW, VFW, R4, "__builtin_elementwise_fma(-a, b, -c)" },

    { "vector_madd",   insn_vfmaddd,  VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(a, b, c)" },
    { "vector_msub",   insn_vfmsubd,  VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(a, b, -c)" },
    { "vector_nmadd",  insn_vfnmaddd, VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(-a, b, c)" },
    { "vector_nmsub",  insn_vfnmsubd, VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(-a, b, -c)" },

    { "merge",          insn_fmergeh,  FH, FH, FH, FH, R4, "__builtin_postrisc_fmergeh(a, b, c)" },
    { "merge",          insn_fmerges,  FS, FS, FS, FS, R4, "__builtin_postrisc_fmerges(a, b, c)" },
    { "merge",          insn_fmerged,  FD, FD, FD, FD, R4, "__builtin_postrisc_fmerged(a, b, c)" },
    { "merge",          insn_fmergeq,  FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fmergeq(a, b, c)" },

    { "postrisc_merge", insn_vfmergeh, VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmergeh(a, b, c)" },
    { "postrisc_merge", insn_vfmerges, VFW, VFW, VFW, VFW, R4, "__builtin_postrisc_vfmerges(a, b, c)" },
    { "postrisc_merge", insn_vfmerged, VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmerged(a, b, c)" },

    { "vpermb",         insn_vpermb,   VUB, VUB, VUB, VUB, R4, "__builtin_postrisc_vpermb(a, b, c)" },
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
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << func.dst_type << " " << fname.str()
            << "(" << func.src1_type << " a, "
                   << func.src2_type << " b,"
                   << func.src3_type << " c)\n";
        out << "{\n";
        out << "    return " << func.fragment << ";\n";
        out << "}\n\n";
    }
}

static const struct {
    EInstructionID insn;
    ValueType dst_type;
    ValueType src1_type;
    ValueType src2_type;
    ValueType src3_type;
    char args[32];
    char fragment[64];
} ternary_rm_functions[] = {
    { insn_fmaddh,   FH, FH, FH, FH, R4, "__builtin_postrisc_fmaddh" },
    { insn_fmsubh,   FH, FH, FH, FH, R4, "__builtin_postrisc_fmsubh" },
    { insn_fnmaddh,  FH, FH, FH, FH, R4, "__builtin_postrisc_fnmaddh" },
    { insn_fnmsubh,  FH, FH, FH, FH, R4, "__builtin_postrisc_fnmsubh" },

    { insn_fmadds,   FS, FS, FS, FS, R4, "__builtin_postrisc_fmadds" },
    { insn_fmsubs,   FS, FS, FS, FS, R4, "__builtin_postrisc_fmsubs" },
    { insn_fnmadds,  FS, FS, FS, FS, R4, "__builtin_postrisc_fnmadds" },
    { insn_fnmsubs,  FS, FS, FS, FS, R4, "__builtin_postrisc_fnmsubs" },

    { insn_fmaddd,   FD, FD, FD, FD, R4, "__builtin_postrisc_fmaddd" },
    { insn_fmsubd,   FD, FD, FD, FD, R4, "__builtin_postrisc_fmsubd" },
    { insn_fnmaddd,  FD, FD, FD, FD, R4, "__builtin_postrisc_fnmaddd" },
    { insn_fnmsubd,  FD, FD, FD, FD, R4, "__builtin_postrisc_fnmsubd" },

    { insn_fmaddq,   FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fmaddq" },
    { insn_fmsubq,   FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fmsubq" },
    { insn_fnmaddq,  FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fnmaddq" },
    { insn_fnmsubq,  FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fnmsubq" },

    { insn_vfmaddh,     VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmaddh" },
    { insn_vfmsubh,     VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmsubh" },
    { insn_vfnmaddh,    VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfnmaddh" },
    { insn_vfnmsubh,    VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfnmsubh" },
    { insn_vfmaddsubh,  VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmaddsubh" },
    { insn_vfmsubaddh,  VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmsubaddh" },

    { insn_vfmadds,     VFW, VFW, VFW, VFW, R4, "__builtin_postrisc_vfmadds" },
    { insn_vfmsubs,     VFW, VFW, VFW, VFW, R4, "__builtin_postrisc_vfmsubs" },
    { insn_vfnmadds,    VFW, VFW, VFW, VFW, R4, "__builtin_postrisc_vfnmadds" },
    { insn_vfnmsubs,    VFW, VFW, VFW, VFW, R4, "__builtin_postrisc_vfnmsubs" },
    { insn_vfmaddsubs,  VFW, VFW, VFW, VFW, R4, "__builtin_postrisc_vfmaddsubs" },
    { insn_vfmsubadds,  VFW, VFW, VFW, VFW, R4, "__builtin_postrisc_vfmsubadds" },

    { insn_vfmaddd,     VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmaddd" },
    { insn_vfmsubd,     VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmsubd" },
    { insn_vfnmaddd,    VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfnmaddd" },
    { insn_vfnmsubd,    VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfnmsubd" },
    { insn_vfmaddsubd,  VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmaddsubd" },
    { insn_vfmsubaddd,  VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmsubaddd" },
};

CODEGEN_HANDLER(FpTernaryRm)
{
    for (const auto& func : ternary_rm_functions) {
        for (const auto& round : rounding_modes) {
            std::ostringstream fname;
            fname << "test_" << func.insn << "_" << RoundingMode2Str(round.rm)
                   << "_" << func.src1_type << "_" << func.src2_type << "_" << func.src3_type << "_" << func.dst_type;

            out << CHECK_LABEL << fname.str() << '\n';
            out << CHECK << insn_alloc << " 4\n";
            out << CHECK_NEXT << InsnCounter(this, func.insn) << " " << func.args;
            if (RoundingMode::DYN != round.rm) {
                out << ", " <<  RoundingMode2Str(round.rm);
            }
            out << '\n';
            out << CHECK_NEXT << insn_retf << " 0\n";

            out << func.dst_type << " " << fname.str() << "(" << func.src1_type << " a, " << func.src2_type  << " b, " << func.src3_type << " c)\n";
            out << "{\n";
            out << "    return " << func.fragment << "(a, b, c, " << round.constant << ");\n";
            out << "}\n\n";
        }
    }
}

static const binary_info binary_strict_functions[] = {
    { "add",      insn_faddh,  FH, FH, FH, R3, "a + b" },
    { "nadd",     insn_fnaddh, FH, FH, FH, R3, "-(a + b)" },
    { "sub",      insn_fsubh,  FH, FH, FH, R3, "a - b" },
    { "mul",      insn_fmulh,  FH, FH, FH, R3, "a * b" },
    { "nmul",     insn_fnmulh, FH, FH, FH, R3, "-(a * b)" },
    { "div",      insn_fdivh,  FH, FH, FH, R3, "a / b" },

    { "add",      insn_fadds,  FS, FS, FS, R3, "a + b" },
    { "nadd",     insn_fnadds, FS, FS, FS, R3, "-(a + b)" },
    { "sub",      insn_fsubs,  FS, FS, FS, R3, "a - b" },
    { "mul",      insn_fmuls,  FS, FS, FS, R3, "a * b" },
    { "nmul",     insn_fnmuls, FS, FS, FS, R3, "-(a * b)" },
    { "div",      insn_fdivs,  FS, FS, FS, R3, "a / b" },

    { "add",      insn_faddd,  FD, FD, FD, R3, "a + b" },
    { "nadd",     insn_fnaddd, FD, FD, FD, R3, "-(a + b)" },
    { "sub",      insn_fsubd,  FD, FD, FD, R3, "a - b" },
    { "mul",      insn_fmuld,  FD, FD, FD, R3, "a * b" },
    { "nmul",     insn_fnmuld, FD, FD, FD, R3, "-(a * b)" },
    { "div",      insn_fdivd,  FD, FD, FD, R3, "a / b" },

    { "add",      insn_faddq,  FQ, FQ, FQ, R3, "a + b" },
    { "nadd",     insn_fnaddq, FQ, FQ, FQ, R3, "-(a + b)" },
    { "sub",      insn_fsubq,  FQ, FQ, FQ, R3, "a - b" },
    { "mul",      insn_fmulq,  FQ, FQ, FQ, R3, "a * b" },
    { "nmul",     insn_fnmulq, FQ, FQ, FQ, R3, "-(a * b)" },
    { "div",      insn_fdivq,  FQ, FQ, FQ, R3, "a / b" },

    { "fcmpoeq",  insn_fcmpoeqh, BOOL, FH, FH, R3, "a == b" },
    { "fcmpone",  insn_fcmponeh, BOOL, FH, FH, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" }, /*FIXME: ONE===>UNE*/
    { "fcmpolt",  insn_fcmpolth, BOOL, FH, FH, R3, "a < b"  },
    { "fcmpoge",  insn_fcmpogeh, BOOL, FH, FH, R3, "a >= b" },
    { "fcmpueq",  insn_fcmpueqh, BOOL, FH, FH, R3, "!(a < b || a > b)" }, /* trick to force UEQ */
    { "fcmpune",  insn_fcmpuneh, BOOL, FH, FH, R3, "!(a == b)" },
    { "fcmpult",  insn_fcmpulth, BOOL, FH, FH, R3, "!(a >= b)"  },
    { "fcmpuge",  insn_fcmpugeh, BOOL, FH, FH, R3, "!(a < b)" },
    { "fcmpu",    insn_fcmpuh,   BOOL, FH, FH, R3, "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "fcmpo",    insn_fcmpoh,   BOOL, FH, FH, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)" },

    { "fcmpole",  insn_fcmpogeh, BOOL, FH, FH, R3_SWAP, "a <= b" },
    { "fcmpogt",  insn_fcmpolth, BOOL, FH, FH, R3_SWAP, "a > b"  },
    { "fcmpule",  insn_fcmpugeh, BOOL, FH, FH, R3_SWAP, "!(a > b)" },
    { "fcmpugt",  insn_fcmpulth, BOOL, FH, FH, R3_SWAP, "!(a <= b)"  },

    { "fcmpoeq",  insn_fcmpoeqs, BOOL, FS, FS, R3, "a == b" },
    { "fcmpone",  insn_fcmpones, BOOL, FS, FS, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" }, /*FIXME: ONE===>UNE*/
    { "fcmpolt",  insn_fcmpolts, BOOL, FS, FS, R3, "a < b"  },
    { "fcmpoge",  insn_fcmpoges, BOOL, FS, FS, R3, "a >= b" },
    { "fcmpueq",  insn_fcmpueqs, BOOL, FS, FS, R3, "!(a < b || a > b)" }, /* trick to force UEQ */
    { "fcmpune",  insn_fcmpunes, BOOL, FS, FS, R3, "!(a == b)" },
    { "fcmpult",  insn_fcmpults, BOOL, FS, FS, R3, "!(a >= b)"  },
    { "fcmpuge",  insn_fcmpuges, BOOL, FS, FS, R3, "!(a < b)" },
    { "fcmpu",    insn_fcmpus,   BOOL, FS, FS, R3, "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "fcmpo",    insn_fcmpos,   BOOL, FS, FS, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)" },

    { "fcmpole",  insn_fcmpoges, BOOL, FS, FS, R3_SWAP, "a <= b" },
    { "fcmpogt",  insn_fcmpolts, BOOL, FS, FS, R3_SWAP, "a > b"  },
    { "fcmpule",  insn_fcmpuges, BOOL, FS, FS, R3_SWAP, "!(a > b)" },
    { "fcmpugt",  insn_fcmpults, BOOL, FS, FS, R3_SWAP, "!(a <= b)"  },

    { "fcmpoeq",  insn_fcmpoeqd, BOOL, FD, FD, R3, "a == b" },
    { "fcmpone",  insn_fcmponed, BOOL, FD, FD, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" }, /*FIXME: ONE===>UNE*/
    { "fcmpolt",  insn_fcmpoltd, BOOL, FD, FD, R3, "a < b"  },
    { "fcmpoge",  insn_fcmpoged, BOOL, FD, FD, R3, "a >= b" },
    { "fcmpueq",  insn_fcmpueqd, BOOL, FD, FD, R3, "!(a < b || a > b)" }, /* trick to force UEQ */
    { "fcmpune",  insn_fcmpuned, BOOL, FD, FD, R3, "!(a == b)" },
    { "fcmpult",  insn_fcmpultd, BOOL, FD, FD, R3, "!(a >= b)"  },
    { "fcmpuge",  insn_fcmpuged, BOOL, FD, FD, R3, "!(a < b)" },
    { "fcmpu",    insn_fcmpud,   BOOL, FD, FD, R3, "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "fcmpo",    insn_fcmpod,   BOOL, FD, FD, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)" },

    { "fcmpole",  insn_fcmpoged, BOOL, FD, FD, R3_SWAP, "a <= b" },
    { "fcmpogt",  insn_fcmpoltd, BOOL, FD, FD, R3_SWAP, "a > b" },
    { "fcmpule",  insn_fcmpuged, BOOL, FD, FD, R3_SWAP, "!(a > b)" },
    { "fcmpugt",  insn_fcmpultd, BOOL, FD, FD, R3_SWAP, "!(a <= b)" },

    { "fcmpoeq",  insn_fcmpoeqq, BOOL, FQ, FQ, R3, "a == b" },
    { "fcmpone",  insn_fcmponeq, BOOL, FQ, FQ, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" }, /*FIXME: ONE===>UNE*/
    { "fcmpolt",  insn_fcmpoltq, BOOL, FQ, FQ, R3, "a < b"  },
    { "fcmpoge",  insn_fcmpogeq, BOOL, FQ, FQ, R3, "a >= b" },
    { "fcmpueq",  insn_fcmpueqq, BOOL, FQ, FQ, R3, "!(a < b || a > b)" }, /* trick to force UEQ */
    { "fcmpune",  insn_fcmpuneq, BOOL, FQ, FQ, R3, "!(a == b)" },
    { "fcmpult",  insn_fcmpultq, BOOL, FQ, FQ, R3, "!(a >= b)"  },
    { "fcmpuge",  insn_fcmpugeq, BOOL, FQ, FQ, R3, "!(a < b)" },
    { "fcmpu",    insn_fcmpuq,   BOOL, FQ, FQ, R3, "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "fcmpo",    insn_fcmpoq,   BOOL, FQ, FQ, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)" },

    { "fcmpole",  insn_fcmpogeq, BOOL, FQ, FQ, R3_SWAP, "a <= b" },
    { "fcmpogt",  insn_fcmpoltq, BOOL, FQ, FQ, R3_SWAP, "a > b"  },
    { "fcmpule",  insn_fcmpugeq, BOOL, FQ, FQ, R3_SWAP, "!(a > b)" },
    { "fcmpugt",  insn_fcmpultq, BOOL, FQ, FQ, R3_SWAP, "!(a <= b)"  },
};

CODEGEN_HANDLER(FpStrict)
{
    for (const auto& func : binary_strict_functions) {
        std::ostringstream fname;
        fname << "test_strict_" << func.name << "_" << func.dst_type << "_" << func.src1_type << "_" << func.src2_type;
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

void Generator::FpClassify(std::ostream& out, EInstructionID insn, ValueType type)
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
         { "__builtin_issignaling", FClassMask::SignalingNaN },
         { "__builtin_isinf",       FClassMask::NegativeInfinity | FClassMask::PositiveInfinity },
         { "__builtin_isnormal",    FClassMask::NegativeNormal | FClassMask::PositiveNormal  },
         { "__builtin_issubnormal", FClassMask::NegativeSubnormal | FClassMask::PositiveSubnormal },
         { "__builtin_isfinite",    FClassMask::NegativeNormal | FClassMask::NegativeSubnormal | FClassMask::NegativeZero |
                                    FClassMask::PositiveNormal | FClassMask::PositiveSubnormal | FClassMask::PositiveZero },

         // { "__builtin_isinf_sign", FClassMask::NegativeInfinity | FClassMask::PositiveInfinity }, // bug in clang/llvm
         // { "__builtin_isnan",   FClassMask::SignalingNaN | FClassMask::QuietNaN  },
         // { "__builtin_iszero",  FClassMask::NegativeZero | FClassMask::PositiveZero },

    };

    for (const auto& test : tests) {
        std::ostringstream fname;
        fname << "test_classify_" << type << "_" << test.func;
        out << "// mask "<< static_cast<u32>(test.mask) << " :" << test.mask << '\n';
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn) << " %r1, %r1, " << static_cast<u32>(test.mask) << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";
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
        out << CHECK << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn) << " %r1, %r1, " << mask << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << "bool " << fname.str() << "(" << type << " a)\n";
        out << "{\n";
        out << "    return __builtin_isfpclass(a, " << mask << ");\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(FpClassifyF16)  { FpClassify(out, insn_fclassh, FH); }
CODEGEN_HANDLER(FpClassifyF32)  { FpClassify(out, insn_fclasss, FS); }
CODEGEN_HANDLER(FpClassifyF64)  { FpClassify(out, insn_fclassd, FD); }
CODEGEN_HANDLER(FpClassifyF128) { FpClassify(out, insn_fclassq, FQ); }

} // namespace llvm
} // namespace postrisc
