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

// (fptoui (ffloor x)) -> (fptoui x).
// Flooring agrees with truncation for non-negative inputs.
// Negative inputs floor to at most -1, which makes the original conversion poison.
#define C_FLR ", rtz"

#define R2      " %r1, %r1"
#define R3      " %r1, %r1, %r2"
#define R3_SWAP " %r1, %r2, %r1"
#define R4_SIGN " %r1, %r2, %r1, %r1"
#define R4      " %r1, %r1, %r2, %r3"

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

void Generator::UnaryRm(std::ostream& out, EInstructionID insn,
    ValueType dst_type, ValueType src1_type,
    const char *args, const char *fragment)
{
    for (const auto& round : rounding_modes) {
        std::ostringstream fname;
        fname << "test_" << insn << "_" << RoundingMode2Str(round.rm)
               << "_" << dst_type << "_" << src1_type;

        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn) << " " << args;
        if (RoundingMode::DYN != round.rm) {
            out << ", " <<  RoundingMode2Str(round.rm);
        }
        out << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << dst_type << " " << fname.str()
            << "(" << src1_type << " a)\n";
        out << "{\n";
        out << "    return " << fragment << "(a, " << round.constant << ");\n";
        out << "}\n\n";
    }
}

void Generator::BinaryRm(std::ostream& out, EInstructionID insn,
    ValueType dst_type, ValueType src1_type, ValueType src2_type,
    const char *args, const char *fragment)
{
    for (const auto& round : rounding_modes) {
        std::ostringstream fname;
        fname << "test_" << insn << "_" << RoundingMode2Str(round.rm)
               << "_" << dst_type << "_" << src1_type << "_" << src2_type;

        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, insn) << " " << args;
        if (RoundingMode::DYN != round.rm) {
            out << ", " <<  RoundingMode2Str(round.rm);
        }
        out << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << dst_type << " " << fname.str()
            << "(" << src1_type << " a, "
                   << src2_type << " b)\n";
        out << "{\n";
        out << "    return " << fragment << "(a, b, " << round.constant << ");\n";
        out << "}\n\n";
    }
}

void Generator::TernaryRm(std::ostream& out, EInstructionID insn,
    ValueType dst_type, ValueType src1_type, ValueType src2_type, ValueType src3_type,
    const char *args, const char *fragment)
{
    for (const auto& round : rounding_modes) {
        std::ostringstream fname;
        fname << "test_" << insn << "_" << RoundingMode2Str(round.rm)
               << "_" << dst_type << "_" << src1_type << "_" << src2_type << "_" << src3_type;

        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK << insn_alloc << " 4\n";
        out << CHECK_NEXT << InsnCounter(this, insn) << " " << args;
        if (RoundingMode::DYN != round.rm) {
            out << ", " <<  RoundingMode2Str(round.rm);
        }
        out << '\n';
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << dst_type << " " << fname.str()
            << "(" << src1_type << " a, "
                   << src2_type << " b, "
                   << src3_type << " c)\n";
        out << "{\n";
        out << "    return " << fragment << "(a, b, c, " << round.constant << ");\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(FpUnary)
{
    Unary(out, "neg",           insn_fnegh,   FH, FH, R2,       "(-x)");
    Unary(out, "abs",           insn_fabsh,   FH, FH, R2,       "__builtin_fabsf16(x)");
    Unary(out, "neg_abs",       insn_fnabsh,  FH, FH, R2,       "- __builtin_fabsf16(x)");
    Unary(out, "sqrt",          insn_fsqrth,  FH, FH, R2,       "__builtin_sqrtf16(x)");
    Unary(out, "roundeven",     insn_frndh,   FH, FH, R2 C_RNE, "__builtin_roundevenf16(x)");
    Unary(out, "round",         insn_frndh,   FH, FH, R2 C_RMM, "__builtin_roundf16(x)");
    Unary(out, "floor",         insn_frndh,   FH, FH, R2 C_RDN, "__builtin_floorf16(x)");
    Unary(out, "ceil",          insn_frndh,   FH, FH, R2 C_RUP, "__builtin_ceilf16(x)");
    Unary(out, "trunc",         insn_frndh,   FH, FH, R2 C_RTZ, "__builtin_truncf16(x)");
    Unary(out, "nearbyint",     insn_frndh,   FH, FH, R2 C_DYN, "__builtin_nearbyintf16(x)");
    Unary(out, "rint",          insn_frndxh,  FH, FH, R2 C_DYN, "__builtin_rintf16(x)");
    Unary(out, "cvt",           insn_fiwtoh,  FH, IW, R2 C_RMM, "(f16)(x)");
    Unary(out, "cvt",           insn_fidtoh,  FH, ID, R2 C_RMM, "(f16)(x)");
    Unary(out, "cvt",           insn_fiqtoh,  FH, IQ, R2 C_RMM, "(f16)(x)");
    Unary(out, "cvt",           insn_fuwtoh,  FH, UW, R2 C_RMM, "(f16)(x)");
    Unary(out, "cvt",           insn_fudtoh,  FH, UD, R2 C_RMM, "(f16)(x)");
    Unary(out, "cvt",           insn_fuqtoh,  FH, UQ, R2 C_RMM, "(f16)(x)");
    Unary(out, "trunc_def",     insn_fhtoiw,  IW, FH, R2 C_RTZ, "(i32)(x)");
    Unary(out, "trunc_def",     insn_fhtoid,  ID, FH, R2 C_RTZ, "(i64)(x)");
    Unary(out, "trunc_def",     insn_fhtoiq,  IQ, FH, R2 C_RTZ, "(i128)(x)");
    Unary(out, "trunc_def",     insn_fhtouw,  UW, FH, R2 C_RTZ, "(u32)(x)");
    Unary(out, "trunc_def",     insn_fhtoud,  UD, FH, R2 C_RTZ, "(u64)(x)");
    Unary(out, "trunc_def",     insn_fhtouq,  UQ, FH, R2 C_RTZ, "(u128)(x)");
    Unary(out, "lrint",         insn_fhtoid,  ID, FH, R2 C_DYN, "__builtin_lrintf16(x)");
    Unary(out, "llrint",        insn_fhtoid,  ID, FH, R2 C_DYN, "__builtin_llrintf16(x)");
    Unary(out, "lround",        insn_fhtoid,  ID, FH, R2 C_RMM, "__builtin_lroundf16(x)");
    Unary(out, "llround",       insn_fhtoid,  ID, FH, R2 C_RMM, "__builtin_llroundf16(x)");
    Unary(out, "roundeven",     insn_fhtoiw,  IW, FH, R2 C_RNE, "(i32)__builtin_roundevenf16(x)");
    Unary(out, "round",         insn_fhtoiw,  IW, FH, R2 C_RMM, "(i32)__builtin_roundf16(x)");
    Unary(out, "floor",         insn_fhtoiw,  IW, FH, R2 C_RDN, "(i32)__builtin_floorf16(x)");
    Unary(out, "ceil",          insn_fhtoiw,  IW, FH, R2 C_RUP, "(i32)__builtin_ceilf16(x)");
    Unary(out, "trunc",         insn_fhtoiw,  IW, FH, R2 C_RTZ, "(i32)__builtin_truncf16(x)");
    Unary(out, "nearbyint",     insn_fhtoiw,  IW, FH, R2 C_DYN, "(i32)__builtin_nearbyintf16(x)");
    Unary(out, "roundeven",     insn_fhtouw,  UW, FH, R2 C_RNE, "(u32)__builtin_roundevenf16(x)");
    Unary(out, "round",         insn_fhtouw,  UW, FH, R2 C_RMM, "(u32)__builtin_roundf16(x)");
    Unary(out, "floor",         insn_fhtouw,  UW, FH, R2 C_FLR, "(u32)__builtin_floorf16(x)");
    Unary(out, "ceil",          insn_fhtouw,  UW, FH, R2 C_RUP, "(u32)__builtin_ceilf16(x)");
    Unary(out, "trunc",         insn_fhtouw,  UW, FH, R2 C_RTZ, "(u32)__builtin_truncf16(x)");
    Unary(out, "nearbyint",     insn_fhtouw,  UW, FH, R2 C_DYN, "(u32)__builtin_nearbyintf16(x)");
    Unary(out, "roundeven",     insn_fhtoid,  ID, FH, R2 C_RNE, "(i64)__builtin_roundevenf16(x)");
    Unary(out, "round",         insn_fhtoid,  ID, FH, R2 C_RMM, "(i64)__builtin_roundf16(x)");
    Unary(out, "floor",         insn_fhtoid,  ID, FH, R2 C_RDN, "(i64)__builtin_floorf16(x)");
    Unary(out, "ceil",          insn_fhtoid,  ID, FH, R2 C_RUP, "(i64)__builtin_ceilf16(x)");
    Unary(out, "trunc",         insn_fhtoid,  ID, FH, R2 C_RTZ, "(i64)__builtin_truncf16(x)");
    Unary(out, "nearbyint",     insn_fhtoid,  ID, FH, R2 C_DYN, "(i64)__builtin_nearbyintf16(x)");
    Unary(out, "roundeven",     insn_fhtoud,  UD, FH, R2 C_RNE, "(u64)__builtin_roundevenf16(x)");
    Unary(out, "round",         insn_fhtoud,  UD, FH, R2 C_RMM, "(u64)__builtin_roundf16(x)");
    Unary(out, "floor",         insn_fhtoud,  UD, FH, R2 C_FLR, "(u64)__builtin_floorf16(x)");
    Unary(out, "ceil",          insn_fhtoud,  UD, FH, R2 C_RUP, "(u64)__builtin_ceilf16(x)");
    Unary(out, "trunc",         insn_fhtoud,  UD, FH, R2 C_RTZ, "(u64)__builtin_truncf16(x)");
    Unary(out, "nearbyint",     insn_fhtoud,  UD, FH, R2 C_DYN, "(u64)__builtin_nearbyintf16(x)");
    Unary(out, "roundeven",     insn_fhtoiq,  IQ, FH, R2 C_RNE, "(i128)__builtin_roundevenf16(x)");
    Unary(out, "round",         insn_fhtoiq,  IQ, FH, R2 C_RMM, "(i128)__builtin_roundf16(x)");
    Unary(out, "floor",         insn_fhtoiq,  IQ, FH, R2 C_RDN, "(i128)__builtin_floorf16(x)");
    Unary(out, "ceil",          insn_fhtoiq,  IQ, FH, R2 C_RUP, "(i128)__builtin_ceilf16(x)");
    Unary(out, "trunc",         insn_fhtoiq,  IQ, FH, R2 C_RTZ, "(i128)__builtin_truncf16(x)");
    Unary(out, "nearbyint",     insn_fhtoiq,  IQ, FH, R2 C_DYN, "(i128)__builtin_nearbyintf16(x)");
    Unary(out, "roundeven",     insn_fhtouq,  UQ, FH, R2 C_RNE, "(u128)__builtin_roundevenf16(x)");
    Unary(out, "round",         insn_fhtouq,  UQ, FH, R2 C_RMM, "(u128)__builtin_roundf16(x)");
    Unary(out, "floor",         insn_fhtouq,  UQ, FH, R2 C_FLR, "(u128)__builtin_floorf16(x)");
    Unary(out, "ceil",          insn_fhtouq,  UQ, FH, R2 C_RUP, "(u128)__builtin_ceilf16(x)");
    Unary(out, "trunc",         insn_fhtouq,  UQ, FH, R2 C_RTZ, "(u128)__builtin_truncf16(x)");
    Unary(out, "nearbyint",     insn_fhtouq,  UQ, FH, R2 C_DYN, "(u128)__builtin_nearbyintf16(x)");

    Unary(out, "neg",           insn_fnegs,    FS, FS, R2,       "(-x)");
    Unary(out, "abs",           insn_fabss,    FS, FS, R2,       "__builtin_fabsf(x)");
    Unary(out, "neg_abs",       insn_fnabss,   FS, FS, R2,       "- __builtin_fabsf(x)");
    Unary(out, "sqrt",          insn_fsqrts,   FS, FS, R2,       "__builtin_sqrtf(x)");
    Unary(out, "roundeven",     insn_frnds,    FS, FS, R2 C_RNE, "__builtin_roundevenf(x)");
    Unary(out, "round",         insn_frnds,    FS, FS, R2 C_RMM, "__builtin_roundf(x)");
    Unary(out, "floor",         insn_frnds,    FS, FS, R2 C_RDN, "__builtin_floorf(x)");
    Unary(out, "ceil",          insn_frnds,    FS, FS, R2 C_RUP, "__builtin_ceilf(x)");
    Unary(out, "trunc",         insn_frnds,    FS, FS, R2 C_RTZ, "__builtin_truncf(x)");
    Unary(out, "nearbyint",     insn_frnds,    FS, FS, R2 C_DYN, "__builtin_nearbyintf(x)");
    Unary(out, "rint",          insn_frndxs,   FS, FS, R2 C_DYN, "__builtin_rintf(x)");
    Unary(out, "cvt",           insn_fiwtos,   FS, IW, R2 C_RMM, "(f32)(x)");
    Unary(out, "cvt",           insn_fidtos,   FS, ID, R2 C_RMM, "(f32)(x)");
    Unary(out, "cvt",           insn_fiqtos,   FS, IQ, R2 C_RMM, "(f32)(x)");
    Unary(out, "cvt",           insn_fuwtos,   FS, UW, R2 C_RMM, "(f32)(x)");
    Unary(out, "cvt",           insn_fudtos,   FS, UD, R2 C_RMM, "(f32)(x)");
    Unary(out, "cvt",           insn_fuqtos,   FS, UQ, R2 C_RMM, "(f32)(x)");
    Unary(out, "trunc_def",     insn_fstoiw,   IW, FS, R2 C_RTZ, "(i32)(x)");
    Unary(out, "trunc_def",     insn_fstoid,   ID, FS, R2 C_RTZ, "(i64)(x)");
    Unary(out, "trunc_def",     insn_fstoiq,   IQ, FS, R2 C_RTZ, "(i128)(x)");
    Unary(out, "trunc_def",     insn_fstouw,   UW, FS, R2 C_RTZ, "(u32)(x)");
    Unary(out, "trunc_def",     insn_fstoud,   UD, FS, R2 C_RTZ, "(u64)(x)");
    Unary(out, "trunc_def",     insn_fstouq,   UQ, FS, R2 C_RTZ, "(u128)(x)");
    Unary(out, "lrint",         insn_fstoid,   ID, FS, R2 C_DYN, "__builtin_lrintf(x)");
    Unary(out, "llrint",        insn_fstoid,   ID, FS, R2 C_DYN, "__builtin_llrintf(x)");
    Unary(out, "lround",        insn_fstoid,   ID, FS, R2 C_RMM, "__builtin_lroundf(x)");
    Unary(out, "llround",       insn_fstoid,   ID, FS, R2 C_RMM, "__builtin_llroundf(x)");

    Unary(out, "roundeven",     insn_fstoiw,  IW, FS, R2 C_RNE, "(i32)__builtin_roundevenf(x)");
    Unary(out, "round",         insn_fstoiw,  IW, FS, R2 C_RMM, "(i32)__builtin_roundf(x)");
    Unary(out, "floor",         insn_fstoiw,  IW, FS, R2 C_RDN, "(i32)__builtin_floorf(x)");
    Unary(out, "ceil",          insn_fstoiw,  IW, FS, R2 C_RUP, "(i32)__builtin_ceilf(x)");
    Unary(out, "trunc",         insn_fstoiw,  IW, FS, R2 C_RTZ, "(i32)__builtin_truncf(x)");
    Unary(out, "nearbyint",     insn_fstoiw,  IW, FS, R2 C_DYN, "(i32)__builtin_nearbyintf(x)");
    Unary(out, "roundeven",     insn_fstouw,  UW, FS, R2 C_RNE, "(u32)__builtin_roundevenf(x)");
    Unary(out, "round",         insn_fstouw,  UW, FS, R2 C_RMM, "(u32)__builtin_roundf(x)");
    Unary(out, "floor",         insn_fstouw,  UW, FS, R2 C_FLR, "(u32)__builtin_floorf(x)");
    Unary(out, "ceil",          insn_fstouw,  UW, FS, R2 C_RUP, "(u32)__builtin_ceilf(x)");
    Unary(out, "trunc",         insn_fstouw,  UW, FS, R2 C_RTZ, "(u32)__builtin_truncf(x)");
    Unary(out, "nearbyint",     insn_fstouw,  UW, FS, R2 C_DYN, "(u32)__builtin_nearbyintf(x)");

    Unary(out, "roundeven",     insn_fstoid,  ID, FS, R2 C_RNE, "(i64)__builtin_roundevenf(x)");
    Unary(out, "round",         insn_fstoid,  ID, FS, R2 C_RMM, "(i64)__builtin_roundf(x)");
    Unary(out, "floor",         insn_fstoid,  ID, FS, R2 C_RDN, "(i64)__builtin_floorf(x)");
    Unary(out, "ceil",          insn_fstoid,  ID, FS, R2 C_RUP, "(i64)__builtin_ceilf(x)");
    Unary(out, "trunc",         insn_fstoid,  ID, FS, R2 C_RTZ, "(i64)__builtin_truncf(x)");
    Unary(out, "nearbyint",     insn_fstoid,  ID, FS, R2 C_DYN, "(i64)__builtin_nearbyintf(x)");
    Unary(out, "roundeven",     insn_fstoud,  UD, FS, R2 C_RNE, "(u64)__builtin_roundevenf(x)");
    Unary(out, "round",         insn_fstoud,  UD, FS, R2 C_RMM, "(u64)__builtin_roundf(x)");
    Unary(out, "floor",         insn_fstoud,  UD, FS, R2 C_FLR, "(u64)__builtin_floorf(x)");
    Unary(out, "ceil",          insn_fstoud,  UD, FS, R2 C_RUP, "(u64)__builtin_ceilf(x)");
    Unary(out, "trunc",         insn_fstoud,  UD, FS, R2 C_RTZ, "(u64)__builtin_truncf(x)");
    Unary(out, "nearbyint",     insn_fstoud,  UD, FS, R2 C_DYN, "(u64)__builtin_nearbyintf(x)");

    Unary(out, "roundeven",     insn_fstoiq, IQ, FS, R2 C_RNE, "(i128)__builtin_roundevenf(x)");
    Unary(out, "round",         insn_fstoiq, IQ, FS, R2 C_RMM, "(i128)__builtin_roundf(x)");
    Unary(out, "floor",         insn_fstoiq, IQ, FS, R2 C_RDN, "(i128)__builtin_floorf(x)");
    Unary(out, "ceil",          insn_fstoiq, IQ, FS, R2 C_RUP, "(i128)__builtin_ceilf(x)");
    Unary(out, "trunc",         insn_fstoiq, IQ, FS, R2 C_RTZ, "(i128)__builtin_truncf(x)");
    Unary(out, "nearbyint",     insn_fstoiq, IQ, FS, R2 C_DYN, "(i128)__builtin_nearbyintf(x)");
    Unary(out, "roundeven",     insn_fstouq, UQ, FS, R2 C_RNE, "(u128)__builtin_roundevenf(x)");
    Unary(out, "round",         insn_fstouq, UQ, FS, R2 C_RMM, "(u128)__builtin_roundf(x)");
    Unary(out, "floor",         insn_fstouq, UQ, FS, R2 C_FLR, "(u128)__builtin_floorf(x)");
    Unary(out, "ceil",          insn_fstouq, UQ, FS, R2 C_RUP, "(u128)__builtin_ceilf(x)");
    Unary(out, "trunc",         insn_fstouq, UQ, FS, R2 C_RTZ, "(u128)__builtin_truncf(x)");
    Unary(out, "nearbyint",     insn_fstouq, UQ, FS, R2 C_DYN, "(u128)__builtin_nearbyintf(x)");

    Unary(out, "neg",           insn_fnegd,   FD, FD, R2,       "(-x)");
    Unary(out, "abs",           insn_fabsd,   FD, FD, R2,       "__builtin_fabs(x)");
    Unary(out, "neg_abs",       insn_fnabsd,  FD, FD, R2,       "- __builtin_fabs(x)");
    Unary(out, "sqrt",          insn_fsqrtd,  FD, FD, R2,       "__builtin_sqrt(x)");
    Unary(out, "roundeven",     insn_frndd,   FD, FD, R2 C_RNE, "__builtin_roundeven(x)");
    Unary(out, "round",         insn_frndd,   FD, FD, R2 C_RMM, "__builtin_round(x)");
    Unary(out, "floor",         insn_frndd,   FD, FD, R2 C_RDN, "__builtin_floor(x)");
    Unary(out, "ceil",          insn_frndd,   FD, FD, R2 C_RUP, "__builtin_ceil(x)");
    Unary(out, "trunc",         insn_frndd,   FD, FD, R2 C_RTZ, "__builtin_trunc(x)");
    Unary(out, "nearbyint",     insn_frndd,   FD, FD, R2 C_DYN, "__builtin_nearbyint(x)");
    Unary(out, "rint",          insn_frndxd,  FD, FD, R2 C_DYN, "__builtin_rint(x)");
    Unary(out, "cvt",           insn_fiwtod,  FD, IW, R2 C_RMM, "(f64)(x)");
    Unary(out, "cvt",           insn_fidtod,  FD, ID, R2 C_RMM, "(f64)(x)");
    Unary(out, "cvt",           insn_fiqtod,  FD, IQ, R2 C_RMM, "(f64)(x)");
    Unary(out, "cvt",           insn_fuwtod,  FD, UW, R2 C_RMM, "(f64)(x)");
    Unary(out, "cvt",           insn_fudtod,  FD, UD, R2 C_RMM, "(f64)(x)");
    Unary(out, "cvt",           insn_fuqtod,  FD, UQ, R2 C_RMM, "(f64)(x)");
    Unary(out, "trunc_def",     insn_fdtoiw,  IW, FD, R2 C_RTZ, "(i32)(x)");
    Unary(out, "trunc_def",     insn_fdtoid,  ID, FD, R2 C_RTZ, "(i64)(x)");
    Unary(out, "trunc_def",     insn_fdtoiq,  IQ, FD, R2 C_RTZ, "(i128)(x)");
    Unary(out, "trunc_def",     insn_fdtouw,  UW, FD, R2 C_RTZ, "(u32)(x)");
    Unary(out, "trunc_def",     insn_fdtoud,  UD, FD, R2 C_RTZ, "(u64)(x)");
    Unary(out, "trunc_def",     insn_fdtouq,  UQ, FD, R2 C_RTZ, "(u128)(x)");
    Unary(out, "lrint",         insn_fdtoid,  ID, FD, R2 C_DYN, "__builtin_lrint(x)");
    Unary(out, "llrint",        insn_fdtoid,  ID, FD, R2 C_DYN, "__builtin_llrint(x)");
    Unary(out, "lround",        insn_fdtoid,  ID, FD, R2 C_RMM, "__builtin_lround(x)");
    Unary(out, "llround",       insn_fdtoid,  ID, FD, R2 C_RMM, "__builtin_llround(x)");

    Unary(out, "roundeven",     insn_fdtoiw,  IW, FD, R2 C_RNE, "(i32)__builtin_roundeven(x)");
    Unary(out, "round",         insn_fdtoiw,  IW, FD, R2 C_RMM, "(i32)__builtin_round(x)");
    Unary(out, "floor",         insn_fdtoiw,  IW, FD, R2 C_RDN, "(i32)__builtin_floor(x)");
    Unary(out, "ceil",          insn_fdtoiw,  IW, FD, R2 C_RUP, "(i32)__builtin_ceil(x)");
    Unary(out, "trunc",         insn_fdtoiw,  IW, FD, R2 C_RTZ, "(i32)__builtin_trunc(x)");
    Unary(out, "nearbyint",     insn_fdtoiw,  IW, FD, R2 C_DYN, "(i32)__builtin_nearbyint(x)");
    Unary(out, "roundeven",     insn_fdtouw,  UW, FD, R2 C_RNE, "(u32)__builtin_roundeven(x)");
    Unary(out, "round",         insn_fdtouw,  UW, FD, R2 C_RMM, "(u32)__builtin_round(x)");
    Unary(out, "floor",         insn_fdtouw,  UW, FD, R2 C_FLR, "(u32)__builtin_floor(x)");
    Unary(out, "ceil",          insn_fdtouw,  UW, FD, R2 C_RUP, "(u32)__builtin_ceil(x)");
    Unary(out, "trunc",         insn_fdtouw,  UW, FD, R2 C_RTZ, "(u32)__builtin_trunc(x)");
    Unary(out, "nearbyint",     insn_fdtouw,  UW, FD, R2 C_DYN, "(u32)__builtin_nearbyint(x)");

    Unary(out, "roundeven",     insn_fdtoid,  ID, FD, R2 C_RNE, "(i64)__builtin_roundeven(x)");
    Unary(out, "round",         insn_fdtoid,  ID, FD, R2 C_RMM, "(i64)__builtin_round(x)");
    Unary(out, "floor",         insn_fdtoid,  ID, FD, R2 C_RDN, "(i64)__builtin_floor(x)");
    Unary(out, "ceil",          insn_fdtoid,  ID, FD, R2 C_RUP, "(i64)__builtin_ceil(x)");
    Unary(out, "trunc",         insn_fdtoid,  ID, FD, R2 C_RTZ, "(i64)__builtin_trunc(x)");
    Unary(out, "nearbyint",     insn_fdtoid,  ID, FD, R2 C_DYN, "(i64)__builtin_nearbyint(x)");
    Unary(out, "roundeven",     insn_fdtoud,  UD, FD, R2 C_RNE, "(u64)__builtin_roundeven(x)");
    Unary(out, "round",         insn_fdtoud,  UD, FD, R2 C_RMM, "(u64)__builtin_round(x)");
    Unary(out, "floor",         insn_fdtoud,  UD, FD, R2 C_FLR, "(u64)__builtin_floor(x)");
    Unary(out, "ceil",          insn_fdtoud,  UD, FD, R2 C_RUP, "(u64)__builtin_ceil(x)");
    Unary(out, "trunc",         insn_fdtoud,  UD, FD, R2 C_RTZ, "(u64)__builtin_trunc(x)");
    Unary(out, "nearbyint",     insn_fdtoud,  UD, FD, R2 C_DYN, "(u64)__builtin_nearbyint(x)");

    Unary(out, "roundeven",     insn_fdtoiq, IQ, FD, R2 C_RNE, "(i128)__builtin_roundeven(x)");
    Unary(out, "round",         insn_fdtoiq, IQ, FD, R2 C_RMM, "(i128)__builtin_round(x)");
    Unary(out, "floor",         insn_fdtoiq, IQ, FD, R2 C_RDN, "(i128)__builtin_floor(x)");
    Unary(out, "ceil",          insn_fdtoiq, IQ, FD, R2 C_RUP, "(i128)__builtin_ceil(x)");
    Unary(out, "trunc",         insn_fdtoiq, IQ, FD, R2 C_RTZ, "(i128)__builtin_trunc(x)");
    Unary(out, "nearbyint",     insn_fdtoiq, IQ, FD, R2 C_DYN, "(i128)__builtin_nearbyint(x)");
    Unary(out, "roundeven",     insn_fdtouq, UQ, FD, R2 C_RNE, "(u128)__builtin_roundeven(x)");
    Unary(out, "round",         insn_fdtouq, UQ, FD, R2 C_RMM, "(u128)__builtin_round(x)");
    Unary(out, "floor",         insn_fdtouq, UQ, FD, R2 C_FLR, "(u128)__builtin_floor(x)");
    Unary(out, "ceil",          insn_fdtouq, UQ, FD, R2 C_RUP, "(u128)__builtin_ceil(x)");
    Unary(out, "trunc",         insn_fdtouq, UQ, FD, R2 C_RTZ, "(u128)__builtin_trunc(x)");
    Unary(out, "nearbyint",     insn_fdtouq, UQ, FD, R2 C_DYN, "(u128)__builtin_nearbyint(x)");

    // quadruple as long double
    Unary(out, "neg",           insn_fnegq,   FQ, FQ, R2,       "(-x)");
    Unary(out, "abs",           insn_fabsq,   FQ, FQ, R2,       "__builtin_fabsl(x)");
    Unary(out, "neg_abs",       insn_fnabsq,  FQ, FQ, R2,       "- __builtin_fabsl(x)");
    Unary(out, "sqrt",          insn_fsqrtq,  FQ, FQ, R2,       "__builtin_sqrtl(x)");
    Unary(out, "roundeven",     insn_frndq,   FQ, FQ, R2 C_RNE, "__builtin_roundevenl(x)");
    Unary(out, "round",         insn_frndq,   FQ, FQ, R2 C_RMM, "__builtin_roundl(x)");
    Unary(out, "floor",         insn_frndq,   FQ, FQ, R2 C_RDN, "__builtin_floorl(x)");
    Unary(out, "ceil",          insn_frndq,   FQ, FQ, R2 C_RUP, "__builtin_ceill(x)");
    Unary(out, "trunc",         insn_frndq,   FQ, FQ, R2 C_RTZ, "__builtin_truncl(x)");
    Unary(out, "nearbyint",     insn_frndq,   FQ, FQ, R2 C_DYN, "__builtin_nearbyintl(x)");
    Unary(out, "rint",          insn_frndxq,  FQ, FQ, R2 C_DYN, "__builtin_rintl(x)");
    Unary(out, "cvt",           insn_fiwtoq,  FQ, IW, R2 C_RMM, "(f128)(x)");
    Unary(out, "cvt",           insn_fidtoq,  FQ, ID, R2 C_RMM, "(f128)(x)");
    Unary(out, "cvt",           insn_fiqtoq,  FQ, IQ, R2 C_RMM, "(f128)(x)");
    Unary(out, "cvt",           insn_fuwtoq,  FQ, UW, R2 C_RMM, "(f128)(x)");
    Unary(out, "cvt",           insn_fudtoq,  FQ, UD, R2 C_RMM, "(f128)(x)");
    Unary(out, "cvt",           insn_fuqtoq,  FQ, UQ, R2 C_RMM, "(f128)(x)");
    Unary(out, "trunc_def",     insn_fqtoiw,  IW, FQ, R2 C_RTZ, "(i32)(x)");
    Unary(out, "trunc_def",     insn_fqtoid,  ID, FQ, R2 C_RTZ, "(i64)(x)");
    Unary(out, "trunc_def",     insn_fqtoiq,  IQ, FQ, R2 C_RTZ, "(i128)(x)");
    Unary(out, "trunc_def",     insn_fqtouw,  UW, FQ, R2 C_RTZ, "(u32)(x)");
    Unary(out, "trunc_def",     insn_fqtoud,  UD, FQ, R2 C_RTZ, "(u64)(x)");
    Unary(out, "trunc_def",     insn_fqtouq,  UQ, FQ, R2 C_RTZ, "(u128)(x)");
    Unary(out, "lrint",         insn_fqtoid,  ID, FQ, R2 C_DYN, "__builtin_lrintl(x)");
    Unary(out, "llrint",        insn_fqtoid,  ID, FQ, R2 C_DYN, "__builtin_llrintl(x)");
    Unary(out, "lround",        insn_fqtoid,  ID, FQ, R2 C_RMM, "__builtin_lroundl(x)");
    Unary(out, "llround",       insn_fqtoid,  ID, FQ, R2 C_RMM, "__builtin_llroundl(x)");

    Unary(out, "roundeven",     insn_fqtoiw,  IW, FQ, R2 C_RNE, "(i32)__builtin_roundevenl(x)");
    Unary(out, "round",         insn_fqtoiw,  IW, FQ, R2 C_RMM, "(i32)__builtin_roundl(x)");
    Unary(out, "floor",         insn_fqtoiw,  IW, FQ, R2 C_RDN, "(i32)__builtin_floorl(x)");
    Unary(out, "ceil",          insn_fqtoiw,  IW, FQ, R2 C_RUP, "(i32)__builtin_ceill(x)");
    Unary(out, "trunc",         insn_fqtoiw,  IW, FQ, R2 C_RTZ, "(i32)__builtin_truncl(x)");
    Unary(out, "nearbyint",     insn_fqtoiw,  IW, FQ, R2 C_DYN, "(i32)__builtin_nearbyintl(x)");
    Unary(out, "roundeven",     insn_fqtouw,  UW, FQ, R2 C_RNE, "(u32)__builtin_roundevenl(x)");
    Unary(out, "round",         insn_fqtouw,  UW, FQ, R2 C_RMM, "(u32)__builtin_roundl(x)");
    Unary(out, "floor",         insn_fqtouw,  UW, FQ, R2 C_FLR, "(u32)__builtin_floorl(x)");
    Unary(out, "ceil",          insn_fqtouw,  UW, FQ, R2 C_RUP, "(u32)__builtin_ceill(x)");
    Unary(out, "trunc",         insn_fqtouw,  UW, FQ, R2 C_RTZ, "(u32)__builtin_truncl(x)");
    Unary(out, "nearbyint",     insn_fqtouw,  UW, FQ, R2 C_DYN, "(u32)__builtin_nearbyintl(x)");

    Unary(out, "roundeven",     insn_fqtoid,  ID, FQ, R2 C_RNE, "(i64)__builtin_roundevenl(x)");
    Unary(out, "round",         insn_fqtoid,  ID, FQ, R2 C_RMM, "(i64)__builtin_roundl(x)");
    Unary(out, "floor",         insn_fqtoid,  ID, FQ, R2 C_RDN, "(i64)__builtin_floorl(x)");
    Unary(out, "ceil",          insn_fqtoid,  ID, FQ, R2 C_RUP, "(i64)__builtin_ceill(x)");
    Unary(out, "trunc",         insn_fqtoid,  ID, FQ, R2 C_RTZ, "(i64)__builtin_truncl(x)");
    Unary(out, "nearbyint",     insn_fqtoid,  ID, FQ, R2 C_DYN, "(i64)__builtin_nearbyintl(x)");
    Unary(out, "roundeven",     insn_fqtoud,  UD, FQ, R2 C_RNE, "(u64)__builtin_roundevenl(x)");
    Unary(out, "round",         insn_fqtoud,  UD, FQ, R2 C_RMM, "(u64)__builtin_roundl(x)");
    Unary(out, "floor",         insn_fqtoud,  UD, FQ, R2 C_FLR, "(u64)__builtin_floorl(x)");
    Unary(out, "ceil",          insn_fqtoud,  UD, FQ, R2 C_RUP, "(u64)__builtin_ceill(x)");
    Unary(out, "trunc",         insn_fqtoud,  UD, FQ, R2 C_RTZ, "(u64)__builtin_truncl(x)");
    Unary(out, "nearbyint",     insn_fqtoud,  UD, FQ, R2 C_DYN, "(u64)__builtin_nearbyintl(x)");

    Unary(out, "roundeven",     insn_fqtoiq,  IQ, FQ, R2 C_RNE, "(i128)__builtin_roundevenl(x)");
    Unary(out, "round",         insn_fqtoiq,  IQ, FQ, R2 C_RMM, "(i128)__builtin_roundl(x)");
    Unary(out, "floor",         insn_fqtoiq,  IQ, FQ, R2 C_RDN, "(i128)__builtin_floorl(x)");
    Unary(out, "ceil",          insn_fqtoiq,  IQ, FQ, R2 C_RUP, "(i128)__builtin_ceill(x)");
    Unary(out, "trunc",         insn_fqtoiq,  IQ, FQ, R2 C_RTZ, "(i128)__builtin_truncl(x)");
    Unary(out, "nearbyint",     insn_fqtoiq,  IQ, FQ, R2 C_DYN, "(i128)__builtin_nearbyintl(x)");
    Unary(out, "roundeven",     insn_fqtouq,  UQ, FQ, R2 C_RNE, "(u128)__builtin_roundevenl(x)");
    Unary(out, "round",         insn_fqtouq,  UQ, FQ, R2 C_RMM, "(u128)__builtin_roundl(x)");
    Unary(out, "floor",         insn_fqtouq,  UQ, FQ, R2 C_FLR, "(u128)__builtin_floorl(x)");
    Unary(out, "ceil",          insn_fqtouq,  UQ, FQ, R2 C_RUP, "(u128)__builtin_ceill(x)");
    Unary(out, "trunc",         insn_fqtouq,  UQ, FQ, R2 C_RTZ, "(u128)__builtin_truncl(x)");
    Unary(out, "nearbyint",     insn_fqtouq,  UQ, FQ, R2 C_DYN, "(u128)__builtin_nearbyintl(x)");

    // same quadruple but via _Float128
    Unary(out, "neg",           insn_fnegq,   FX, FX, R2,       "(-x)");
    Unary(out, "abs",           insn_fabsq,   FX, FX, R2,       "__builtin_fabsf128(x)");
    Unary(out, "neg_abs",       insn_fnabsq,  FX, FX, R2,       "- __builtin_fabsf128(x)");
    Unary(out, "sqrt",          insn_fsqrtq,  FX, FX, R2,       "__builtin_sqrtf128(x)");
    Unary(out, "roundeven",     insn_frndq,   FX, FX, R2 C_RNE, "__builtin_roundevenf128(x)");
    Unary(out, "round",         insn_frndq,   FX, FX, R2 C_RMM, "__builtin_roundf128(x)");
    Unary(out, "floor",         insn_frndq,   FX, FX, R2 C_RDN, "__builtin_floorf128(x)");
    Unary(out, "ceil",          insn_frndq,   FX, FX, R2 C_RUP, "__builtin_ceilf128(x)");
    Unary(out, "trunc",         insn_frndq,   FX, FX, R2 C_RTZ, "__builtin_truncf128(x)");
    Unary(out, "nearbyint",     insn_frndq,   FX, FX, R2 C_DYN, "__builtin_nearbyintf128(x)");
    Unary(out, "rint",          insn_frndxq,  FX, FX, R2 C_DYN, "__builtin_rintf128(x)");
    Unary(out, "cvt",           insn_fiwtoq,  FX, IW, R2 C_RMM, "(F128)(x)");
    Unary(out, "cvt",           insn_fidtoq,  FX, ID, R2 C_RMM, "(F128)(x)");
    Unary(out, "cvt",           insn_fiqtoq,  FX, IQ, R2 C_RMM, "(F128)(x)");
    Unary(out, "cvt",           insn_fuwtoq,  FX, UW, R2 C_RMM, "(F128)(x)");
    Unary(out, "cvt",           insn_fudtoq,  FX, UD, R2 C_RMM, "(F128)(x)");
    Unary(out, "cvt",           insn_fuqtoq,  FX, UQ, R2 C_RMM, "(F128)(x)");
    Unary(out, "trunc_def",     insn_fqtoiw,  IW, FX, R2 C_RTZ, "(i32)(x)");
    Unary(out, "trunc_def",     insn_fqtoid,  ID, FX, R2 C_RTZ, "(i64)(x)");
    Unary(out, "trunc_def",     insn_fqtoiq,  IQ, FX, R2 C_RTZ, "(i128)(x)");
    Unary(out, "trunc_def",     insn_fqtouw,  UW, FX, R2 C_RTZ, "(u32)(x)");
    Unary(out, "trunc_def",     insn_fqtoud,  UD, FX, R2 C_RTZ, "(u64)(x)");
    Unary(out, "trunc_def",     insn_fqtouq,  UQ, FX, R2 C_RTZ, "(u128)(x)");
    Unary(out, "lrint",         insn_fqtoid,  ID, FX, R2 C_DYN, "__builtin_lrintf128(x)");
    Unary(out, "llrint",        insn_fqtoid,  ID, FX, R2 C_DYN, "__builtin_llrintf128(x)");
    Unary(out, "lround",        insn_fqtoid,  ID, FX, R2 C_RMM, "__builtin_lroundf128(x)");
    Unary(out, "llround",       insn_fqtoid,  ID, FX, R2 C_RMM, "__builtin_llroundf128(x)");

    Unary(out, "roundeven",     insn_fqtoiw,  IW, FX, R2 C_RNE, "(i32)__builtin_roundevenf128(x)");
    Unary(out, "round",         insn_fqtoiw,  IW, FX, R2 C_RMM, "(i32)__builtin_roundf128(x)");
    Unary(out, "floor",         insn_fqtoiw,  IW, FX, R2 C_RDN, "(i32)__builtin_floorf128(x)");
    Unary(out, "ceil",          insn_fqtoiw,  IW, FX, R2 C_RUP, "(i32)__builtin_ceilf128(x)");
    Unary(out, "trunc",         insn_fqtoiw,  IW, FX, R2 C_RTZ, "(i32)__builtin_truncf128(x)");
    Unary(out, "nearbyint",     insn_fqtoiw,  IW, FX, R2 C_DYN, "(i32)__builtin_nearbyintf128(x)");
    Unary(out, "roundeven",     insn_fqtouw,  UW, FX, R2 C_RNE, "(u32)__builtin_roundevenf128(x)");
    Unary(out, "round",         insn_fqtouw,  UW, FX, R2 C_RMM, "(u32)__builtin_roundf128(x)");
    Unary(out, "floor",         insn_fqtouw,  UW, FX, R2 C_FLR, "(u32)__builtin_floorf128(x)");
    Unary(out, "ceil",          insn_fqtouw,  UW, FX, R2 C_RUP, "(u32)__builtin_ceilf128(x)");
    Unary(out, "trunc",         insn_fqtouw,  UW, FX, R2 C_RTZ, "(u32)__builtin_truncf128(x)");
    Unary(out, "nearbyint",     insn_fqtouw,  UW, FX, R2 C_DYN, "(u32)__builtin_nearbyintf128(x)");

    Unary(out, "roundeven",     insn_fqtoid,  ID, FX, R2 C_RNE, "(i64)__builtin_roundevenf128(x)");
    Unary(out, "round",         insn_fqtoid,  ID, FX, R2 C_RMM, "(i64)__builtin_roundf128(x)");
    Unary(out, "floor",         insn_fqtoid,  ID, FX, R2 C_RDN, "(i64)__builtin_floorf128(x)");
    Unary(out, "ceil",          insn_fqtoid,  ID, FX, R2 C_RUP, "(i64)__builtin_ceilf128(x)");
    Unary(out, "trunc",         insn_fqtoid,  ID, FX, R2 C_RTZ, "(i64)__builtin_truncf128(x)");
    Unary(out, "nearbyint",     insn_fqtoid,  ID, FX, R2 C_DYN, "(i64)__builtin_nearbyintf128(x)");
    Unary(out, "roundeven",     insn_fqtoud,  UD, FX, R2 C_RNE, "(u64)__builtin_roundevenf128(x)");
    Unary(out, "round",         insn_fqtoud,  UD, FX, R2 C_RMM, "(u64)__builtin_roundf128(x)");
    Unary(out, "floor",         insn_fqtoud,  UD, FX, R2 C_FLR, "(u64)__builtin_floorf128(x)");
    Unary(out, "ceil",          insn_fqtoud,  UD, FX, R2 C_RUP, "(u64)__builtin_ceilf128(x)");
    Unary(out, "trunc",         insn_fqtoud,  UD, FX, R2 C_RTZ, "(u64)__builtin_truncf128(x)");
    Unary(out, "nearbyint",     insn_fqtoud,  UD, FX, R2 C_DYN, "(u64)__builtin_nearbyintf128(x)");

    Unary(out, "roundeven",     insn_fqtoiq,  IQ, FX, R2 C_RNE, "(i128)__builtin_roundevenf128(x)");
    Unary(out, "round",         insn_fqtoiq,  IQ, FX, R2 C_RMM, "(i128)__builtin_roundf128(x)");
    Unary(out, "floor",         insn_fqtoiq,  IQ, FX, R2 C_RDN, "(i128)__builtin_floorf128(x)");
    Unary(out, "ceil",          insn_fqtoiq,  IQ, FX, R2 C_RUP, "(i128)__builtin_ceilf128(x)");
    Unary(out, "trunc",         insn_fqtoiq,  IQ, FX, R2 C_RTZ, "(i128)__builtin_truncf128(x)");
    Unary(out, "nearbyint",     insn_fqtoiq,  IQ, FX, R2 C_DYN, "(i128)__builtin_nearbyintf128(x)");
    Unary(out, "roundeven",     insn_fqtouq,  UQ, FX, R2 C_RNE, "(u128)__builtin_roundevenf128(x)");
    Unary(out, "round",         insn_fqtouq,  UQ, FX, R2 C_RMM, "(u128)__builtin_roundf128(x)");
    Unary(out, "floor",         insn_fqtouq,  UQ, FX, R2 C_FLR, "(u128)__builtin_floorf128(x)");
    Unary(out, "ceil",          insn_fqtouq,  UQ, FX, R2 C_RUP, "(u128)__builtin_ceilf128(x)");
    Unary(out, "trunc",         insn_fqtouq,  UQ, FX, R2 C_RTZ, "(u128)__builtin_truncf128(x)");
    Unary(out, "nearbyint",     insn_fqtouq,  UQ, FX, R2 C_DYN, "(u128)__builtin_nearbyintf128(x)");

    // vector float
    Unary(out, "builtin_neg",         insn_vfnegh,   VFH, VFH, R2,        "(-x)");
    Unary(out, "vector_abs",          insn_vfabsh,   VFH, VFH, R2,        "__builtin_elementwise_abs(x)");
    Unary(out, "builtin_nabs",        insn_vfnabsh,  VFH, VFH, R2,        "- __builtin_elementwise_abs(x)");
    Unary(out, "builtin_sqrt",        insn_vfsqrth,  VFH, VFH, R2,        "__builtin_elementwise_sqrt(x)");
    Unary(out, "vector_roundeven",    insn_vfrndh,   VFH, VFH, R2 C_RNE,  "__builtin_elementwise_roundeven(x)");
    Unary(out, "vector_round",        insn_vfrndh,   VFH, VFH, R2 C_RMM,  "__builtin_elementwise_round(x)");
    Unary(out, "vector_floor",        insn_vfrndh,   VFH, VFH, R2 C_RDN,  "__builtin_elementwise_floor(x)");
    Unary(out, "vector_ceil",         insn_vfrndh,   VFH, VFH, R2 C_RUP,  "__builtin_elementwise_ceil(x)");
    Unary(out, "vector_trunc",        insn_vfrndh,   VFH, VFH, R2 C_RTZ,  "__builtin_elementwise_trunc(x)");
    Unary(out, "vector_nearbyint",    insn_vfrndh,   VFH, VFH, R2 C_DYN,  "__builtin_elementwise_nearbyint(x)");
    Unary(out, "vector_rint",         insn_vfrndxh,  VFH, VFH, R2 C_DYN,  "__builtin_elementwise_rint(x)");
    Unary(out, "builtin_unpack_low",  insn_vfupklh,  VFH, VFH, R2,        "__builtin_postrisc_vfupklh(x)");
    Unary(out, "builtin_unpack_high", insn_vfupkhh,  VFH, VFH, R2,        "__builtin_postrisc_vfupkhh(x)");

    Unary(out, "builtin_neg",         insn_vfnegs,   VFS, VFS, R2,        "(-x)");
    Unary(out, "vector_abs",          insn_vfabss,   VFS, VFS, R2,        "__builtin_elementwise_abs(x)");
    Unary(out, "builtin_nabs",        insn_vfnabss,  VFS, VFS, R2,        "- __builtin_elementwise_abs(x)");
    Unary(out, "builtin_sqrt",        insn_vfsqrts,  VFS, VFS, R2,        "__builtin_elementwise_sqrt(x)");
    Unary(out, "vector_roundeven",    insn_vfrnds,   VFS, VFS, R2 C_RNE,  "__builtin_elementwise_roundeven(x)");
    Unary(out, "vector_round",        insn_vfrnds,   VFS, VFS, R2 C_RMM,  "__builtin_elementwise_round(x)");
    Unary(out, "vector_floor",        insn_vfrnds,   VFS, VFS, R2 C_RDN,  "__builtin_elementwise_floor(x)");
    Unary(out, "vector_ceil",         insn_vfrnds,   VFS, VFS, R2 C_RUP,  "__builtin_elementwise_ceil(x)");
    Unary(out, "vector_trunc",        insn_vfrnds,   VFS, VFS, R2 C_RTZ,  "__builtin_elementwise_trunc(x)");
    Unary(out, "vector_nearbyint",    insn_vfrnds,   VFS, VFS, R2 C_DYN,  "__builtin_elementwise_nearbyint(x)");
    Unary(out, "vector_rint",         insn_vfrndxs,  VFS, VFS, R2 C_DYN,  "__builtin_elementwise_rint(x)");
    Unary(out, "builtin_unpack_low",  insn_vfupkls,  VFS, VFS, R2,        "__builtin_postrisc_vfupkls(x)");
    Unary(out, "builtin_unpack_high", insn_vfupkhs,  VFS, VFS, R2,        "__builtin_postrisc_vfupkhs(x)");

    Unary(out, "builtin_neg",         insn_vfnegd,   VFD, VFD, R2,        "(-x)");
    Unary(out, "vector_abs",          insn_vfabsd,   VFD, VFD, R2,        "__builtin_elementwise_abs(x)");
    Unary(out, "builtin_nabs",        insn_vfnabsd,  VFD, VFD, R2,        "- __builtin_elementwise_abs(x)");
    Unary(out, "builtin_sqrt",        insn_vfsqrtd,  VFD, VFD, R2,        "__builtin_elementwise_sqrt(x)");
    Unary(out, "vector_roundeven",    insn_vfrndd,   VFD, VFD, R2 C_RNE,  "__builtin_elementwise_roundeven(x)");
    Unary(out, "vector_round",        insn_vfrndd,   VFD, VFD, R2 C_RMM,  "__builtin_elementwise_round(x)");
    Unary(out, "vector_floor",        insn_vfrndd,   VFD, VFD, R2 C_RDN,  "__builtin_elementwise_floor(x)");
    Unary(out, "vector_ceil",         insn_vfrndd,   VFD, VFD, R2 C_RUP,  "__builtin_elementwise_ceil(x)");
    Unary(out, "vector_trunc",        insn_vfrndd,   VFD, VFD, R2 C_RTZ,  "__builtin_elementwise_trunc(x)");
    Unary(out, "vector_nearbyint",    insn_vfrndd,   VFD, VFD, R2 C_DYN,  "__builtin_elementwise_nearbyint(x)");
    Unary(out, "vector_rint",         insn_vfrndxd,  VFD, VFD, R2 C_DYN,  "__builtin_elementwise_rint(x)");
    Unary(out, "builtin_unpack_low",  insn_vfupkld,  VFD, VFD, R2,        "__builtin_postrisc_vfupkld(x)");
    Unary(out, "builtin_unpack_high", insn_vfupkhd,  VFD, VFD, R2,        "__builtin_postrisc_vfupkhd(x)");

    // Unary(out, "", insn_class_f64,      BOOL, FD, "__builtin_isnan(x)");

    // Unary(out, "signbit", insn_slsrl_u32,    BOOL, FH,  "%r1, %r1, 16, 31",  "__builtin_signbitf16(x)");
    Unary(out, "signbit", insn_srliw, BOOL, FS, " %r1, %r1, 31",   "__builtin_signbitf(x)");
    Unary(out, "signbit", insn_srlid, BOOL, FD, " %r1, %r1, 63",   "__builtin_signbit(x)");
    Unary(out, "signbit", insn_srliq, BOOL, FQ, " %r1, %r1, 127",  "__builtin_signbitl(x)");

    Unary(out, "extend", insn_fhtos, FS, FH, R2, "(x)");
    Unary(out, "extend", insn_fhtod, FD, FH, R2, "(x)");
    Unary(out, "extend", insn_fhtoq, FQ, FH, R2, "(x)");
    Unary(out, "extend", insn_fstod, FD, FS, R2, "(x)");
    Unary(out, "extend", insn_fstoq, FQ, FS, R2, "(x)");
    Unary(out, "extend", insn_fdtoq, FQ, FD, R2, "(x)");

    Unary(out, "narrow", insn_fstoh, FH, FS, R2 C_DYN, "(x)");
    Unary(out, "narrow", insn_fdtoh, FH, FD, R2 C_DYN, "(x)");
    Unary(out, "narrow", insn_fqtoh, FH, FQ, R2 C_DYN, "(x)");
    Unary(out, "narrow", insn_fdtos, FS, FD, R2 C_DYN, "(x)");
    Unary(out, "narrow", insn_fqtos, FS, FQ, R2 C_DYN, "(x)");
    Unary(out, "narrow", insn_fqtod, FD, FQ, R2 C_DYN, "(x)");
}

CODEGEN_HANDLER(FpUnaryRm)
{
    UnaryRm(out, insn_fsqrth,  FH, FH, R2, "__builtin_postrisc_fsqrth");
    UnaryRm(out, insn_frsqrth, FH, FH, R2, "__builtin_postrisc_frsqrth");
    UnaryRm(out, insn_frndh,   FH, FH, R2, "__builtin_postrisc_frndh");
    UnaryRm(out, insn_frndxh,  FH, FH, R2, "__builtin_postrisc_frndxh");

    UnaryRm(out, insn_fsqrts,  FS, FS, R2, "__builtin_postrisc_fsqrts");
    UnaryRm(out, insn_frsqrts, FS, FS, R2, "__builtin_postrisc_frsqrts");
    UnaryRm(out, insn_frnds,   FS, FS, R2, "__builtin_postrisc_frnds");
    UnaryRm(out, insn_frndxs,  FS, FS, R2, "__builtin_postrisc_frndxs");

    UnaryRm(out, insn_fsqrtd,  FD, FD, R2, "__builtin_postrisc_fsqrtd");
    UnaryRm(out, insn_frsqrtd, FD, FD, R2, "__builtin_postrisc_frsqrtd");
    UnaryRm(out, insn_frndd,   FD, FD, R2, "__builtin_postrisc_frndd");
    UnaryRm(out, insn_frndxd,  FD, FD, R2, "__builtin_postrisc_frndxd");

    UnaryRm(out, insn_fsqrtq,  FQ, FQ, R2, "__builtin_postrisc_fsqrtq");
    UnaryRm(out, insn_frsqrtq, FQ, FQ, R2, "__builtin_postrisc_frsqrtq");
    UnaryRm(out, insn_frndq,   FQ, FQ, R2, "__builtin_postrisc_frndq");
    UnaryRm(out, insn_frndxq,  FQ, FQ, R2, "__builtin_postrisc_frndxq");

    UnaryRm(out, insn_fqtoh,  FH, FQ, R2, "__builtin_postrisc_fqtoh");
    UnaryRm(out, insn_fqtos,  FS, FQ, R2, "__builtin_postrisc_fqtos");
    UnaryRm(out, insn_fqtod,  FD, FQ, R2, "__builtin_postrisc_fqtod");
    UnaryRm(out, insn_fdtoh,  FH, FD, R2, "__builtin_postrisc_fdtoh");
    UnaryRm(out, insn_fdtos,  FS, FD, R2, "__builtin_postrisc_fdtos");
    UnaryRm(out, insn_fstoh,  FH, FS, R2, "__builtin_postrisc_fstoh");

    UnaryRm(out, insn_fhtoiw, IW, FH, R2, "__builtin_postrisc_fhtoiw");
    UnaryRm(out, insn_fhtouw, UW, FH, R2, "__builtin_postrisc_fhtouw");
    UnaryRm(out, insn_fhtoid, ID, FH, R2, "__builtin_postrisc_fhtoid");
    UnaryRm(out, insn_fhtoud, UD, FH, R2, "__builtin_postrisc_fhtoud");
    UnaryRm(out, insn_fhtoiq, IQ, FH, R2, "__builtin_postrisc_fhtoiq");
    UnaryRm(out, insn_fhtouq, UQ, FH, R2, "__builtin_postrisc_fhtouq");

    UnaryRm(out, insn_fiwtoh,  FH, IW, R2, "__builtin_postrisc_fiwtoh");
    UnaryRm(out, insn_fuwtoh,  FH, UW, R2, "__builtin_postrisc_fuwtoh");
    UnaryRm(out, insn_fidtoh,  FH, ID, R2, "__builtin_postrisc_fidtoh");
    UnaryRm(out, insn_fudtoh,  FH, UD, R2, "__builtin_postrisc_fudtoh");
    UnaryRm(out, insn_fiqtoh,  FH, IQ, R2, "__builtin_postrisc_fiqtoh");
    UnaryRm(out, insn_fuqtoh,  FH, UQ, R2, "__builtin_postrisc_fuqtoh");

    UnaryRm(out, insn_fstoiw,  IW, FS, R2, "__builtin_postrisc_fstoiw");
    UnaryRm(out, insn_fstouw,  UW, FS, R2, "__builtin_postrisc_fstouw");
    UnaryRm(out, insn_fstoid,  ID, FS, R2, "__builtin_postrisc_fstoid");
    UnaryRm(out, insn_fstoud,  UD, FS, R2, "__builtin_postrisc_fstoud");
    UnaryRm(out, insn_fstoiq,  IQ, FS, R2, "__builtin_postrisc_fstoiq");
    UnaryRm(out, insn_fstouq,  UQ, FS, R2, "__builtin_postrisc_fstouq");

    UnaryRm(out, insn_fiwtos,  FS, IW, R2, "__builtin_postrisc_fiwtos");
    UnaryRm(out, insn_fuwtos,  FS, UW, R2, "__builtin_postrisc_fuwtos");
    UnaryRm(out, insn_fidtos,  FS, ID, R2, "__builtin_postrisc_fidtos");
    UnaryRm(out, insn_fudtos,  FS, UD, R2, "__builtin_postrisc_fudtos");
    UnaryRm(out, insn_fiqtos,  FS, IQ, R2, "__builtin_postrisc_fiqtos");
    UnaryRm(out, insn_fuqtos,  FS, UQ, R2, "__builtin_postrisc_fuqtos");

    UnaryRm(out, insn_fdtoiw,  IW, FD, R2, "__builtin_postrisc_fdtoiw");
    UnaryRm(out, insn_fdtouw,  UW, FD, R2, "__builtin_postrisc_fdtouw");
    UnaryRm(out, insn_fdtoid,  ID, FD, R2, "__builtin_postrisc_fdtoid");
    UnaryRm(out, insn_fdtoud,  UD, FD, R2, "__builtin_postrisc_fdtoud");
    UnaryRm(out, insn_fdtoiq,  IQ, FD, R2, "__builtin_postrisc_fdtoiq");
    UnaryRm(out, insn_fdtouq,  UQ, FD, R2, "__builtin_postrisc_fdtouq");

    UnaryRm(out, insn_fiwtod,  FD, IW, R2, "__builtin_postrisc_fiwtod");
    UnaryRm(out, insn_fuwtod,  FD, UW, R2, "__builtin_postrisc_fuwtod");
    UnaryRm(out, insn_fidtod,  FD, ID, R2, "__builtin_postrisc_fidtod");
    UnaryRm(out, insn_fudtod,  FD, UD, R2, "__builtin_postrisc_fudtod");
    UnaryRm(out, insn_fiqtod,  FD, IQ, R2, "__builtin_postrisc_fiqtod");
    UnaryRm(out, insn_fuqtod,  FD, UQ, R2, "__builtin_postrisc_fuqtod");

    UnaryRm(out, insn_fqtoiw,  IW, FQ, R2, "__builtin_postrisc_fqtoiw");
    UnaryRm(out, insn_fqtouw,  UW, FQ, R2, "__builtin_postrisc_fqtouw");
    UnaryRm(out, insn_fqtoid,  ID, FQ, R2, "__builtin_postrisc_fqtoid");
    UnaryRm(out, insn_fqtoud,  UD, FQ, R2, "__builtin_postrisc_fqtoud");
    UnaryRm(out, insn_fqtoiq,  IQ, FQ, R2, "__builtin_postrisc_fqtoiq");
    UnaryRm(out, insn_fqtouq,  UQ, FQ, R2, "__builtin_postrisc_fqtouq");

    UnaryRm(out, insn_fiwtoq,  FQ, IW, R2, "__builtin_postrisc_fiwtoq");
    UnaryRm(out, insn_fuwtoq,  FQ, UW, R2, "__builtin_postrisc_fuwtoq");
    UnaryRm(out, insn_fidtoq,  FQ, ID, R2, "__builtin_postrisc_fidtoq");
    UnaryRm(out, insn_fudtoq,  FQ, UD, R2, "__builtin_postrisc_fudtoq");
    UnaryRm(out, insn_fiqtoq,  FQ, IQ, R2, "__builtin_postrisc_fiqtoq");
    UnaryRm(out, insn_fuqtoq,  FQ, UQ, R2, "__builtin_postrisc_fuqtoq");

    UnaryRm(out, insn_fqtoiw,  IW, FX, R2, "__builtin_postrisc_fqtoiw");
    UnaryRm(out, insn_fqtouw,  UW, FX, R2, "__builtin_postrisc_fqtouw");
    UnaryRm(out, insn_fqtoid,  ID, FX, R2, "__builtin_postrisc_fqtoid");
    UnaryRm(out, insn_fqtoud,  UD, FX, R2, "__builtin_postrisc_fqtoud");
    UnaryRm(out, insn_fqtoiq,  IQ, FX, R2, "__builtin_postrisc_fqtoiq");
    UnaryRm(out, insn_fqtouq,  UQ, FX, R2, "__builtin_postrisc_fqtouq");

    UnaryRm(out, insn_fiwtoq,  FX, IW, R2, "__builtin_postrisc_fiwtoq");
    UnaryRm(out, insn_fuwtoq,  FX, UW, R2, "__builtin_postrisc_fuwtoq");
    UnaryRm(out, insn_fidtoq,  FX, ID, R2, "__builtin_postrisc_fidtoq");
    UnaryRm(out, insn_fudtoq,  FX, UD, R2, "__builtin_postrisc_fudtoq");
    UnaryRm(out, insn_fiqtoq,  FX, IQ, R2, "__builtin_postrisc_fiqtoq");
    UnaryRm(out, insn_fuqtoq,  FX, UQ, R2, "__builtin_postrisc_fuqtoq");

    UnaryRm(out, insn_vfsqrth,  VFH, VFH, R2, "__builtin_postrisc_vfsqrth");
    UnaryRm(out, insn_vfrsqrth, VFH, VFH, R2, "__builtin_postrisc_vfrsqrth");
    UnaryRm(out, insn_vfrndh,   VFH, VFH, R2, "__builtin_postrisc_vfrndh");
    UnaryRm(out, insn_vfrndxh,  VFH, VFH, R2, "__builtin_postrisc_vfrndxh");

    UnaryRm(out, insn_vfsqrts,  VFS, VFS, R2, "__builtin_postrisc_vfsqrts");
    UnaryRm(out, insn_vfrsqrts, VFS, VFS, R2, "__builtin_postrisc_vfrsqrts");
    UnaryRm(out, insn_vfrnds,   VFS, VFS, R2, "__builtin_postrisc_vfrnds");
    UnaryRm(out, insn_vfrndxs,  VFS, VFS, R2, "__builtin_postrisc_vfrndxs");

    UnaryRm(out, insn_vfsqrtd,  VFD, VFD, R2, "__builtin_postrisc_vfsqrtd");
    UnaryRm(out, insn_vfrsqrtd, VFD, VFD, R2, "__builtin_postrisc_vfrsqrtd");
    UnaryRm(out, insn_vfrndd,   VFD, VFD, R2, "__builtin_postrisc_vfrndd");
    UnaryRm(out, insn_vfrndxd,  VFD, VFD, R2, "__builtin_postrisc_vfrndxd");

    UnaryRm(out, insn_vfhtoih,  VIH, VFH, R2, "__builtin_postrisc_vfhtoih");
    UnaryRm(out, insn_vfhtouh,  VUH, VFH, R2, "__builtin_postrisc_vfhtouh");
    UnaryRm(out, insn_vfihtoh,  VFH, VIH, R2, "__builtin_postrisc_vfihtoh");
    UnaryRm(out, insn_vfuhtoh,  VFH, VUH, R2, "__builtin_postrisc_vfuhtoh");

    UnaryRm(out, insn_vfstoiw,  VIW, VFS, R2, "__builtin_postrisc_vfstoiw");
    UnaryRm(out, insn_vfstouw,  VUW, VFS, R2, "__builtin_postrisc_vfstouw");
    UnaryRm(out, insn_vfiwtos,  VFS, VIW, R2, "__builtin_postrisc_vfiwtos");
    UnaryRm(out, insn_vfuwtos,  VFS, VUW, R2, "__builtin_postrisc_vfuwtos");

    UnaryRm(out, insn_vfdtoid,  VID, VFD, R2, "__builtin_postrisc_vfdtoid");
    UnaryRm(out, insn_vfdtoud,  VUD, VFD, R2, "__builtin_postrisc_vfdtoud");
    UnaryRm(out, insn_vfidtod,  VFD, VID, R2, "__builtin_postrisc_vfidtod");
    UnaryRm(out, insn_vfudtod,  VFD, VUD, R2, "__builtin_postrisc_vfudtod");
}

CODEGEN_HANDLER(FpBinary)
{
    Binary(out, "add",               insn_faddh,    FH, FH, FH, R3_SWAP,  "a + b");
    Binary(out, "nadd",              insn_fnaddh,   FH, FH, FH, R3_SWAP,  "-(a + b)");
    Binary(out, "sub",               insn_fsubh,    FH, FH, FH, R3,       "a - b");
    Binary(out, "mul",               insn_fmulh,    FH, FH, FH, R3_SWAP,  "a * b");
    Binary(out, "nmul",              insn_fnmulh,   FH, FH, FH, R3_SWAP,  "- (a * b)");
    Binary(out, "div",               insn_fdivh,    FH, FH, FH, R3,       "a / b");
    Binary(out, "abs_diff",          insn_fabdh,    FH, FH, FH, R3,       "__builtin_fabsf16(a - b)");
    Binary(out, "nabs_diff",         insn_fnabdh,   FH, FH, FH, R3,       "- __builtin_fabsf16(a - b)");
    Binary(out, "copysign",          insn_fmergeh,  FH, FH, FH, R4_SIGN,  "__builtin_copysignf16(a, b)");
    Binary(out, "min_lt_abab",       insn_fminnmh,  FH, FH, FH, R3,       "a <  b ? a : b");
    Binary(out, "max_ge_abab",       insn_fmaxnmh,  FH, FH, FH, R3,       "a >= b ? a : b");
    Binary(out, "min_le_abab",       insn_fminnmh,  FH, FH, FH, R3,       "a <= b ? a : b");
    Binary(out, "max_gt_abab",       insn_fmaxnmh,  FH, FH, FH, R3,       "a >  b ? a : b");
    Binary(out, "max_lt_baab",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b <  a ? a : b");
    Binary(out, "min_ge_baab",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b >= a ? a : b");
    Binary(out, "max_le_baab",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b <= a ? a : b");
    Binary(out, "min_gt_baab",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b >  a ? a : b");
    Binary(out, "max_lt_abba",       insn_fmaxnmh,  FH, FH, FH, R3,       "a <  b ? b : a");
    Binary(out, "min_ge_abba",       insn_fminnmh,  FH, FH, FH, R3,       "a >= b ? b : a");
    Binary(out, "max_le_abba",       insn_fmaxnmh,  FH, FH, FH, R3,       "a <= b ? b : a");
    Binary(out, "min_gt_abba",       insn_fminnmh,  FH, FH, FH, R3,       "a >  b ? b : a");
    Binary(out, "min_lt_baba",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b <  a ? b : a");
    Binary(out, "max_ge_baba",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b >= a ? b : a");
    Binary(out, "min_le_baba",       insn_fminnmh,  FH, FH, FH, R3_SWAP,  "b <= a ? b : a");
    Binary(out, "max_gt_baba",       insn_fmaxnmh,  FH, FH, FH, R3_SWAP,  "b >  a ? b : a");
    Binary(out, "builtin_minimum",   insn_fminh,    FH, FH, FH, R3,       "__builtin_postrisc_fminimumh(a, b)");
    Binary(out, "builtin_maximum",   insn_fmaxh,    FH, FH, FH, R3,       "__builtin_postrisc_fmaximumh(a, b)");
    Binary(out, "min",               insn_fminnmh,  FH, FH, FH, R3,       "__builtin_fminf16(a, b)");
    Binary(out, "max",               insn_fmaxnmh,  FH, FH, FH, R3,       "__builtin_fmaxf16(a, b)");
    Binary(out, "fminimum_num",      insn_fminnmh,  FH, FH, FH, R3,       "__builtin_fminimum_numf16(a, b)");
    Binary(out, "fmaximum_num",      insn_fmaxnmh,  FH, FH, FH, R3,       "__builtin_fmaximum_numf16(a, b)");
    Binary(out, "builtin_abs_min",   insn_faminnmh, FH, FH, FH, R3,       "__builtin_fminf16(__builtin_fabsf16(a), __builtin_fabsf16(b))");
    Binary(out, "builtin_abs_max",   insn_famaxnmh, FH, FH, FH, R3,       "__builtin_fmaxf16(__builtin_fabsf16(a), __builtin_fabsf16(b))");

    Binary(out, "fcmpoeq",           insn_fcmpoeqh, BOOL, FH, FH, R3,       "a == b");
    Binary(out, "fcmpone",           insn_fcmponeh, BOOL, FH, FH, R3,       "a != b");
    Binary(out, "fcmpolt",           insn_fcmpolth, BOOL, FH, FH, R3,       "a < b");
    Binary(out, "fcmpole",           insn_fcmpogeh, BOOL, FH, FH, R3_SWAP,  "a <= b");
    Binary(out, "fcmpogt",           insn_fcmpolth, BOOL, FH, FH, R3_SWAP,  "a > b");
    Binary(out, "fcmpoge",           insn_fcmpogeh, BOOL, FH, FH, R3,       "a >= b");

    Binary(out, "add",               insn_fadds,      FS, FS, FS, R3_SWAP,  "a + b");
    Binary(out, "nadd",              insn_fnadds,     FS, FS, FS, R3_SWAP,  "-(a + b)");
    Binary(out, "sub",               insn_fsubs,      FS, FS, FS, R3,       "a - b");
    Binary(out, "mul",               insn_fmuls,      FS, FS, FS, R3_SWAP,  "a * b");
    Binary(out, "nmul",              insn_fnmuls,     FS, FS, FS, R3_SWAP,  "- (a * b)");
    Binary(out, "div",               insn_fdivs,      FS, FS, FS, R3,       "a / b");
    Binary(out, "abs_diff",          insn_fabds,      FS, FS, FS, R3,       "__builtin_fabsf(a - b)");
    Binary(out, "nabs_diff",         insn_fnabds,     FS, FS, FS, R3,       "- __builtin_fabsf(a - b)");
    Binary(out, "copysign",          insn_fmerges,    FS, FS, FS, R4_SIGN,  "__builtin_copysignf(a, b)");
    Binary(out, "fcmpoeq",           insn_fcmpoeqs, BOOL, FS, FS, R3,       "a == b");
    Binary(out, "fcmpone",           insn_fcmpones, BOOL, FS, FS, R3,       "a != b");
    Binary(out, "fcmpolt",           insn_fcmpolts, BOOL, FS, FS, R3,       "a < b");
    Binary(out, "fcmpole",           insn_fcmpoges, BOOL, FS, FS, R3_SWAP,  "a <= b");
    Binary(out, "fcmpogt",           insn_fcmpolts, BOOL, FS, FS, R3_SWAP,  "a > b");
    Binary(out, "fcmpoge",           insn_fcmpoges, BOOL, FS, FS, R3,       "a >= b");
    Binary(out, "min_lt_abab",       insn_fminnms,    FS, FS, FS, R3,       "a <  b ? a : b");
    Binary(out, "max_ge_abab",       insn_fmaxnms,  FS, FS, FS, R3,       "a >= b ? a : b");
    Binary(out, "min_le_abab",       insn_fminnms,  FS, FS, FS, R3,       "a <= b ? a : b");
    Binary(out, "max_gt_abab",       insn_fmaxnms,  FS, FS, FS, R3,       "a >  b ? a : b");
    Binary(out, "max_lt_baab",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b <  a ? a : b");
    Binary(out, "min_ge_baab",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b >= a ? a : b");
    Binary(out, "max_le_baab",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b <= a ? a : b");
    Binary(out, "min_gt_baab",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b >  a ? a : b");
    Binary(out, "max_lt_abba",       insn_fmaxnms,  FS, FS, FS, R3,       "a <  b ? b : a");
    Binary(out, "min_ge_abba",       insn_fminnms,  FS, FS, FS, R3,       "a >= b ? b : a");
    Binary(out, "max_le_abba",       insn_fmaxnms,  FS, FS, FS, R3,       "a <= b ? b : a");
    Binary(out, "min_gt_abba",       insn_fminnms,  FS, FS, FS, R3,       "a >  b ? b : a");
    Binary(out, "min_lt_baba",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b <  a ? b : a");
    Binary(out, "max_ge_baba",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b >= a ? b : a");
    Binary(out, "min_le_baba",       insn_fminnms,  FS, FS, FS, R3_SWAP,  "b <= a ? b : a");
    Binary(out, "max_gt_baba",       insn_fmaxnms,  FS, FS, FS, R3_SWAP,  "b >  a ? b : a");
    Binary(out, "builtin_minimum",   insn_fmins,     FS, FS, FS, R3,       "__builtin_postrisc_fminimums(a, b)");
    Binary(out, "builtin_maximum",   insn_fmaxs,     FS, FS, FS, R3,       "__builtin_postrisc_fmaximums(a, b)");
    Binary(out, "min",               insn_fminnms,  FS, FS, FS, R3,       "__builtin_fminf(a, b)");
    Binary(out, "max",               insn_fmaxnms,  FS, FS, FS, R3,       "__builtin_fmaxf(a, b)");
    Binary(out, "fminimum_num",      insn_fminnms,  FS, FS, FS, R3,       "__builtin_fminimum_numf(a, b)");
    Binary(out, "fmaximum_num",      insn_fmaxnms,  FS, FS, FS, R3,       "__builtin_fmaximum_numf(a, b)");
    Binary(out, "builtin_abs_min",   insn_faminnms, FS, FS, FS, R3,       "__builtin_fminf(__builtin_fabsf(a), __builtin_fabsf(b))");
    Binary(out, "builtin_abs_max",   insn_famaxnms, FS, FS, FS, R3,       "__builtin_fmaxf(__builtin_fabsf(a), __builtin_fabsf(b))");

    Binary(out, "add",               insn_faddd,      FD, FD, FD, R3_SWAP,  "a + b");
    Binary(out, "nadd",              insn_fnaddd,     FD, FD, FD, R3_SWAP,  "-(a + b)");
    Binary(out, "sub",               insn_fsubd,      FD, FD, FD, R3,       "a - b");
    Binary(out, "mul",               insn_fmuld,      FD, FD, FD, R3_SWAP,  "a * b");
    Binary(out, "nmul",              insn_fnmuld,     FD, FD, FD, R3_SWAP,  "- (a * b)");
    Binary(out, "div",               insn_fdivd,      FD, FD, FD, R3,       "a / b");
    Binary(out, "abs_diff",          insn_fabdd,      FD, FD, FD, R3,       "__builtin_fabs(a - b)");
    Binary(out, "nabs_diff",         insn_fnabdd,     FD, FD, FD, R3,       "- __builtin_fabs(a - b)");
    Binary(out, "copysign",          insn_fmerged,    FD, FD, FD, R4_SIGN,  "__builtin_copysign(a, b)");
    Binary(out, "fcmpoeq",           insn_fcmpoeqd, BOOL, FD, FD, R3,       "a == b");
    Binary(out, "fcmpone",           insn_fcmponed, BOOL, FD, FD, R3,       "a != b");
    Binary(out, "fcmpolt",           insn_fcmpoltd, BOOL, FD, FD, R3,       "a < b");
    Binary(out, "fcmpole",           insn_fcmpoged, BOOL, FD, FD, R3_SWAP,  "a <= b");
    Binary(out, "fcmpogt",           insn_fcmpoltd, BOOL, FD, FD, R3_SWAP,  "a > b");
    Binary(out, "fcmpoge",           insn_fcmpoged, BOOL, FD, FD, R3,       "a >= b");
    Binary(out, "min_lt_abab",       insn_fminnmd,   FD, FD, FD, R3,       "a <  b ? a : b");
    Binary(out, "max_ge_abab",       insn_fmaxnmd,   FD, FD, FD, R3,       "a >= b ? a : b");
    Binary(out, "min_le_abab",       insn_fminnmd,   FD, FD, FD, R3,       "a <= b ? a : b");
    Binary(out, "max_gt_abab",       insn_fmaxnmd,   FD, FD, FD, R3,       "a >  b ? a : b");
    Binary(out, "max_lt_baab",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b <  a ? a : b");
    Binary(out, "min_ge_baab",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b >= a ? a : b");
    Binary(out, "max_le_baab",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b <= a ? a : b");
    Binary(out, "min_gt_baab",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b >  a ? a : b");
    Binary(out, "max_lt_abba",       insn_fmaxnmd,   FD, FD, FD, R3,       "a <  b ? b : a");
    Binary(out, "min_ge_abba",       insn_fminnmd,   FD, FD, FD, R3,       "a >= b ? b : a");
    Binary(out, "max_le_abba",       insn_fmaxnmd,   FD, FD, FD, R3,       "a <= b ? b : a");
    Binary(out, "min_gt_abba",       insn_fminnmd,   FD, FD, FD, R3,       "a >  b ? b : a");
    Binary(out, "min_lt_baba",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b <  a ? b : a");
    Binary(out, "max_ge_baba",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b >= a ? b : a");
    Binary(out, "min_le_baba",       insn_fminnmd,   FD, FD, FD, R3_SWAP,  "b <= a ? b : a");
    Binary(out, "max_gt_baba",       insn_fmaxnmd,   FD, FD, FD, R3_SWAP,  "b >  a ? b : a");
    Binary(out, "builtin_minimum",   insn_fmind,     FD, FD, FD, R3,       "__builtin_postrisc_fminimumd(a, b)");
    Binary(out, "builtin_maximum",   insn_fmaxd,     FD, FD, FD, R3,       "__builtin_postrisc_fmaximumd(a, b)");
    Binary(out, "min",               insn_fminnmd,   FD, FD, FD, R3,       "__builtin_fmin(a, b)");
    Binary(out, "max",               insn_fmaxnmd,   FD, FD, FD, R3,       "__builtin_fmax(a, b)");
    Binary(out, "fminimum_num",      insn_fminnmd,   FD, FD, FD, R3,       "__builtin_fminimum_num(a, b)");
    Binary(out, "fmaximum_num",      insn_fmaxnmd,   FD, FD, FD, R3,       "__builtin_fmaximum_num(a, b)");
    Binary(out, "builtin_abs_min",   insn_faminnmd,  FD, FD, FD, R3,       "__builtin_fmin(__builtin_fabs(a), __builtin_fabs(b))");
    Binary(out, "builtin_abs_max",   insn_famaxnmd,  FD, FD, FD, R3,       "__builtin_fmax(__builtin_fabs(a), __builtin_fabs(b))");

    Binary(out, "add",               insn_faddq,      FQ, FQ, FQ, R3_SWAP,  "a + b");
    Binary(out, "nadd",              insn_fnaddq,     FQ, FQ, FQ, R3_SWAP,  "-(a + b)");
    Binary(out, "sub",               insn_fsubq,      FQ, FQ, FQ, R3,       "a - b");
    Binary(out, "mul",               insn_fmulq,      FQ, FQ, FQ, R3_SWAP,  "a * b");
    Binary(out, "nmul",              insn_fnmulq,     FQ, FQ, FQ, R3_SWAP,  "- (a * b)");
    Binary(out, "div",               insn_fdivq,      FQ, FQ, FQ, R3,       "a / b");
    Binary(out, "abs_diff",          insn_fabdq,      FQ, FQ, FQ, R3,       "__builtin_fabsl(a - b)");
    Binary(out, "nabs_diff",         insn_fnabdq,     FQ, FQ, FQ, R3,       "- __builtin_fabsl(a - b)");
    Binary(out, "copysign",          insn_fmergeq,    FQ, FQ, FQ, R4_SIGN,  "__builtin_copysignl(a, b)");
    Binary(out, "fcmpoeq",           insn_fcmpoeqq, BOOL, FQ, FQ, R3,       "a == b");
    Binary(out, "fcmpone",           insn_fcmponeq, BOOL, FQ, FQ, R3,       "a != b");
    Binary(out, "fcmpolt",           insn_fcmpoltq, BOOL, FQ, FQ, R3,       "a < b");
    Binary(out, "fcmpole",           insn_fcmpogeq, BOOL, FQ, FQ, R3_SWAP,  "a <= b");
    Binary(out, "fcmpogt",           insn_fcmpoltq, BOOL, FQ, FQ, R3_SWAP,  "a > b");
    Binary(out, "fcmpoge",           insn_fcmpogeq, BOOL, FQ, FQ, R3,       "a >= b");
    Binary(out, "min_lt_abab",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a <  b ? a : b");
    Binary(out, "max_ge_abab",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a >= b ? a : b");
    Binary(out, "min_le_abab",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a <= b ? a : b");
    Binary(out, "max_gt_abab",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a >  b ? a : b");
    Binary(out, "max_lt_baab",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b <  a ? a : b");
    Binary(out, "min_ge_baab",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b >= a ? a : b");
    Binary(out, "max_le_baab",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b <= a ? a : b");
    Binary(out, "min_gt_baab",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b >  a ? a : b");
    Binary(out, "max_lt_abba",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a <  b ? b : a");
    Binary(out, "min_ge_abba",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a >= b ? b : a");
    Binary(out, "max_le_abba",       insn_fmaxnmq,  FQ, FQ, FQ, R3,       "a <= b ? b : a");
    Binary(out, "min_gt_abba",       insn_fminnmq,  FQ, FQ, FQ, R3,       "a >  b ? b : a");
    Binary(out, "min_lt_baba",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b <  a ? b : a");
    Binary(out, "max_ge_baba",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b >= a ? b : a");
    Binary(out, "min_le_baba",       insn_fminnmq,  FQ, FQ, FQ, R3_SWAP,  "b <= a ? b : a");
    Binary(out, "max_gt_baba",       insn_fmaxnmq,  FQ, FQ, FQ, R3_SWAP,  "b >  a ? b : a");
    Binary(out, "builtin_minimum",   insn_fminq,    FQ, FQ, FQ, R3,       "__builtin_postrisc_fminimumq(a, b)");
    Binary(out, "builtin_maximum",   insn_fmaxq,    FQ, FQ, FQ, R3,       "__builtin_postrisc_fmaximumq(a, b)");
    Binary(out, "min",               insn_fminnmq,  FQ, FQ, FQ, R3,       "__builtin_fminl(a, b)");
    Binary(out, "max",               insn_fmaxnmq,  FQ, FQ, FQ, R3,       "__builtin_fmaxl(a, b)");
    Binary(out, "fminimum_num",      insn_fminnmq,  FQ, FQ, FQ, R3,       "__builtin_fminimum_numl(a, b)");
    Binary(out, "fmaximum_num",      insn_fmaxnmq,  FQ, FQ, FQ, R3,       "__builtin_fmaximum_numl(a, b)");
    Binary(out, "builtin_abs_min",   insn_faminnmq, FQ, FQ, FQ, R3,       "__builtin_fminl(__builtin_fabsl(a), __builtin_fabsl(b))");
    Binary(out, "builtin_abs_max",   insn_famaxnmq, FQ, FQ, FQ, R3,       "__builtin_fmaxl(__builtin_fabsl(a), __builtin_fabsl(b))");

    Binary(out, "add",               insn_faddq,      FX, FX, FX, R3_SWAP,  "a + b");
    Binary(out, "nadd",              insn_fnaddq,     FX, FX, FX, R3_SWAP,  "-(a + b)");
    Binary(out, "sub",               insn_fsubq,      FX, FX, FX, R3,       "a - b");
    Binary(out, "mul",               insn_fmulq,      FX, FX, FX, R3_SWAP,  "a * b");
    Binary(out, "nmul",              insn_fnmulq,     FX, FX, FX, R3_SWAP,  "- (a * b)");
    Binary(out, "div",               insn_fdivq,      FX, FX, FX, R3,       "a / b");
    Binary(out, "abs_diff",          insn_fabdq,      FX, FX, FX, R3,       "__builtin_fabsf128(a - b)");
    Binary(out, "nabs_diff",         insn_fnabdq,     FX, FX, FX, R3,       "- __builtin_fabsf128(a - b)");
    Binary(out, "copysign",          insn_fmergeq,    FX, FX, FX, R4_SIGN,  "__builtin_copysignf128(a, b)");
    Binary(out, "fcmpoeq",           insn_fcmpoeqq, BOOL, FX, FX, R3,       "a == b");
    Binary(out, "fcmpone",           insn_fcmponeq, BOOL, FX, FX, R3,       "a != b");
    Binary(out, "fcmpolt",           insn_fcmpoltq, BOOL, FX, FX, R3,       "a < b");
    Binary(out, "fcmpole",           insn_fcmpogeq, BOOL, FX, FX, R3_SWAP,  "a <= b");
    Binary(out, "fcmpogt",           insn_fcmpoltq, BOOL, FX, FX, R3_SWAP,  "a > b");
    Binary(out, "fcmpoge",           insn_fcmpogeq, BOOL, FX, FX, R3,       "a >= b");
    Binary(out, "min_lt_abab",       insn_fminnmq,  FX, FX, FX, R3,       "a <  b ? a : b");
    Binary(out, "max_ge_abab",       insn_fmaxnmq,  FX, FX, FX, R3,       "a >= b ? a : b");
    Binary(out, "min_le_abab",       insn_fminnmq,  FX, FX, FX, R3,       "a <= b ? a : b");
    Binary(out, "max_gt_abab",       insn_fmaxnmq,  FX, FX, FX, R3,       "a >  b ? a : b");
    Binary(out, "max_lt_baab",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b <  a ? a : b");
    Binary(out, "min_ge_baab",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b >= a ? a : b");
    Binary(out, "max_le_baab",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b <= a ? a : b");
    Binary(out, "min_gt_baab",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b >  a ? a : b");
    Binary(out, "max_lt_abba",       insn_fmaxnmq,  FX, FX, FX, R3,       "a <  b ? b : a");
    Binary(out, "min_ge_abba",       insn_fminnmq,  FX, FX, FX, R3,       "a >= b ? b : a");
    Binary(out, "max_le_abba",       insn_fmaxnmq,  FX, FX, FX, R3,       "a <= b ? b : a");
    Binary(out, "min_gt_abba",       insn_fminnmq,  FX, FX, FX, R3,       "a >  b ? b : a");
    Binary(out, "min_lt_baba",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b <  a ? b : a");
    Binary(out, "max_ge_baba",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b >= a ? b : a");
    Binary(out, "min_le_baba",       insn_fminnmq,  FX, FX, FX, R3_SWAP,  "b <= a ? b : a");
    Binary(out, "max_gt_baba",       insn_fmaxnmq,  FX, FX, FX, R3_SWAP,  "b >  a ? b : a");
    Binary(out, "builtin_minimum",   insn_fminq,    FX, FX, FX, R3,       "__builtin_postrisc_fminimumq(a, b)");
    Binary(out, "builtin_maximum",   insn_fmaxq,    FX, FX, FX, R3,       "__builtin_postrisc_fmaximumq(a, b)");
    Binary(out, "min",               insn_fminnmq,  FX, FX, FX, R3,       "__builtin_fminf128(a, b)");
    Binary(out, "max",               insn_fmaxnmq,  FX, FX, FX, R3,       "__builtin_fmaxf128(a, b)");
    Binary(out, "fminimum_num",      insn_fminnmq,  FX, FX, FX, R3,       "__builtin_fminimum_numf128(a, b)");
    Binary(out, "fmaximum_num",      insn_fmaxnmq,  FX, FX, FX, R3,       "__builtin_fmaximum_numf128(a, b)");
    Binary(out, "builtin_abs_min",   insn_faminnmq, FX, FX, FX, R3,       "__builtin_fminf128(__builtin_fabsf128(a), __builtin_fabsf128(b))");
    Binary(out, "builtin_abs_max",   insn_famaxnmq, FX, FX, FX, R3,       "__builtin_fmaxf128(__builtin_fabsf128(a), __builtin_fabsf128(b))");

    Binary(out, "add",                 insn_vfaddh,     VFH, VFH, VFH, R3_SWAP, "a + b");
    Binary(out, "nadd",                insn_vfnaddh,    VFH, VFH, VFH, R3_SWAP, "-(a + b)");
    Binary(out, "sub",                 insn_vfsubh,     VFH, VFH, VFH, R3,      "a - b");
    Binary(out, "mul",                 insn_vfmulh,     VFH, VFH, VFH, R3_SWAP, "a * b");
    Binary(out, "nmul",                insn_vfnmulh,    VFH, VFH, VFH, R3_SWAP, "-(a * b)");
    Binary(out, "div",                 insn_vfdivh,     VFH, VFH, VFH, R3,      "a / b");
    Binary(out, "abs_diff",            insn_vfabdh,     VFH, VFH, VFH, R3,      "__builtin_elementwise_abs(a - b)");
    Binary(out, "nabs_diff",           insn_vfnabdh,    VFH, VFH, VFH, R3,      "- __builtin_elementwise_abs(a - b)");
    Binary(out, "vector_abs_max",      insn_vfamaxnmh,  VFH, VFH, VFH, R3,      "__builtin_elementwise_maxnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))");
    Binary(out, "vector_abs_min",      insn_vfaminnmh,  VFH, VFH, VFH, R3,      "__builtin_elementwise_minnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))");
    Binary(out, "vector_copysign",     insn_vfmergeh,   VFH, VFH, VFH, R4_SIGN, "__builtin_elementwise_copysign(a, b)");
    Binary(out, "vector_minimum",      insn_vfminh,     VFH, VFH, VFH, R3,      "__builtin_elementwise_minimum(a, b)");
    Binary(out, "vector_maximum",      insn_vfmaxh,     VFH, VFH, VFH, R3,      "__builtin_elementwise_maximum(a, b)");
    Binary(out, "vector_max",          insn_vfmaxnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_max(a, b)");
    Binary(out, "vector_min",          insn_vfminnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_min(a, b)");
    Binary(out, "vector_minnum",       insn_vfminnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_minnum(a, b)");
    Binary(out, "vector_maxnum",       insn_vfmaxnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_maxnum(a, b)");
    Binary(out, "vector_minimumnum",   insn_vfminnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_minimumnum(a, b)");
    Binary(out, "vector_maximumnum",   insn_vfmaxnmh,   VFH, VFH, VFH, R3,      "__builtin_elementwise_maximumnum(a, b)");
    Binary(out, "builtin_cmpoeq",      insn_vfcmpoeqh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpoeqh(a, b)");
    Binary(out, "builtin_cmpone",      insn_vfcmponeh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmponeh(a, b)");
    Binary(out, "builtin_cmpolt",      insn_vfcmpolth,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpolth(a, b)");
    Binary(out, "builtin_cmpoge",      insn_vfcmpogeh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpogeh(a, b)");
    Binary(out, "builtin_cmpo",        insn_vfcmpoh,    VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpoh(a, b)");
    Binary(out, "builtin_cmpueq",      insn_vfcmpueqh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpueqh(a, b)");
    Binary(out, "builtin_cmpune",      insn_vfcmpuneh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpuneh(a, b)");
    Binary(out, "builtin_cmpult",      insn_vfcmpulth,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpulth(a, b)");
    Binary(out, "builtin_cmpuge",      insn_vfcmpugeh,  VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpugeh(a, b)");
    Binary(out, "builtin_cmpu",        insn_vfcmpuh,    VIH, VFH, VFH, R3,      "__builtin_postrisc_vfcmpuh(a, b)");
    Binary(out, "builtin_merge_low",   insn_vfmergelh,  VFH, VFH, VFH, R3,      "__builtin_postrisc_vfmergelh(a, b)");
    Binary(out, "builtin_merge_high",  insn_vfmergehh,  VFH, VFH, VFH, R3,      "__builtin_postrisc_vfmergehh(a, b)");

    Binary(out, "add",                 insn_vfadds,     VFS, VFS, VFS, R3_SWAP, "a + b");
    Binary(out, "nadd",                insn_vfnadds,    VFS, VFS, VFS, R3_SWAP, "-(a + b)");
    Binary(out, "sub",                 insn_vfsubs,     VFS, VFS, VFS, R3,      "a - b");
    Binary(out, "mul",                 insn_vfmuls,     VFS, VFS, VFS, R3_SWAP, "a * b");
    Binary(out, "nmul",                insn_vfnmuls,    VFS, VFS, VFS, R3_SWAP, "-(a * b)");
    Binary(out, "div",                 insn_vfdivs,     VFS, VFS, VFS, R3,      "a / b");
    Binary(out, "abs_diff",            insn_vfabds,     VFS, VFS, VFS, R3,      "__builtin_elementwise_abs(a - b)");
    Binary(out, "nabs_diff",           insn_vfnabds,    VFS, VFS, VFS, R3,      "- __builtin_elementwise_abs(a - b)");
    Binary(out, "vector_abs_max",      insn_vfamaxnms,  VFS, VFS, VFS, R3,      "__builtin_elementwise_maxnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))");
    Binary(out, "vector_abs_min",      insn_vfaminnms,  VFS, VFS, VFS, R3,      "__builtin_elementwise_minnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))");
    Binary(out, "vector_copysign",     insn_vfmerges,   VFS, VFS, VFS, R4_SIGN, "__builtin_elementwise_copysign(a, b)");
    Binary(out, "vector_minimum",      insn_vfmins,     VFS, VFS, VFS, R3,      "__builtin_elementwise_minimum(a, b)");
    Binary(out, "vector_maximum",      insn_vfmaxs,     VFS, VFS, VFS, R3,      "__builtin_elementwise_maximum(a, b)");
    Binary(out, "vector_max",          insn_vfmaxnms,   VFS, VFS, VFS, R3,      "__builtin_elementwise_max(a, b)");
    Binary(out, "vector_min",          insn_vfminnms,   VFS, VFS, VFS, R3,      "__builtin_elementwise_min(a, b)");
    Binary(out, "vector_minnum",       insn_vfminnms,   VFS, VFS, VFS, R3,      "__builtin_elementwise_minnum(a, b)");
    Binary(out, "vector_maxnum",       insn_vfmaxnms,   VFS, VFS, VFS, R3,      "__builtin_elementwise_maxnum(a, b)");
    Binary(out, "vector_minimumnum",   insn_vfminnms,   VFS, VFS, VFS, R3,      "__builtin_elementwise_minimumnum(a, b)");
    Binary(out, "vector_maximumnum",   insn_vfmaxnms,   VFS, VFS, VFS, R3,      "__builtin_elementwise_maximumnum(a, b)");
    Binary(out, "builtin_cmpoeq",      insn_vfcmpoeqs,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpoeqs(a, b)");
    Binary(out, "builtin_cmpone",      insn_vfcmpones,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpones(a, b)");
    Binary(out, "builtin_cmpolt",      insn_vfcmpolts,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpolts(a, b)");
    Binary(out, "builtin_cmpoge",      insn_vfcmpoges,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpoges(a, b)");
    Binary(out, "builtin_cmpo",        insn_vfcmpos,    VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpos(a, b)");
    Binary(out, "builtin_cmpueq",      insn_vfcmpueqs,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpueqs(a, b)");
    Binary(out, "builtin_cmpune",      insn_vfcmpunes,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpunes(a, b)");
    Binary(out, "builtin_cmpult",      insn_vfcmpults,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpults(a, b)");
    Binary(out, "builtin_cmpuge",      insn_vfcmpuges,  VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpuges(a, b)");
    Binary(out, "builtin_cmpu",        insn_vfcmpus,    VIW, VFS, VFS, R3,      "__builtin_postrisc_vfcmpus(a, b)");
    Binary(out, "builtin_merge_low",   insn_vfmergels,  VFS, VFS, VFS, R3,      "__builtin_postrisc_vfmergels(a, b)");
    Binary(out, "builtin_merge_high",  insn_vfmergehs,  VFS, VFS, VFS, R3,      "__builtin_postrisc_vfmergehs(a, b)");

    Binary(out, "add",                 insn_vfaddd,     VFD, VFD, VFD, R3_SWAP, "a + b");
    Binary(out, "nadd",                insn_vfnaddd,    VFD, VFD, VFD, R3_SWAP, "-(a + b)");
    Binary(out, "sub",                 insn_vfsubd,     VFD, VFD, VFD, R3,      "a - b");
    Binary(out, "mul",                 insn_vfmuld,     VFD, VFD, VFD, R3_SWAP, "a * b");
    Binary(out, "nmul",                insn_vfnmuld,    VFD, VFD, VFD, R3_SWAP, "-(a * b)");
    Binary(out, "div",                 insn_vfdivd,     VFD, VFD, VFD, R3,      "a / b");
    Binary(out, "abs_diff",            insn_vfabdd,     VFD, VFD, VFD, R3,      "__builtin_elementwise_abs(a - b)");
    Binary(out, "nabs_diff",           insn_vfnabdd,    VFD, VFD, VFD, R3,      "- __builtin_elementwise_abs(a - b)");
    Binary(out, "vector_abs_max",      insn_vfamaxnmd,  VFD, VFD, VFD, R3,      "__builtin_elementwise_maxnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))");
    Binary(out, "vector_abs_min",      insn_vfaminnmd,  VFD, VFD, VFD, R3,      "__builtin_elementwise_minnum(__builtin_elementwise_abs(a), __builtin_elementwise_abs(b))");
    Binary(out, "vector_copysign",     insn_vfmerged,   VFD, VFD, VFD, R4_SIGN, "__builtin_elementwise_copysign(a, b)");
    Binary(out, "vector_minimum",      insn_vfmind,     VFD, VFD, VFD, R3,      "__builtin_elementwise_minimum(a, b)");
    Binary(out, "vector_maximum",      insn_vfmaxd,     VFD, VFD, VFD, R3,      "__builtin_elementwise_maximum(a, b)");
    Binary(out, "vector_max",          insn_vfmaxnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_max(a, b)");
    Binary(out, "vector_min",          insn_vfminnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_min(a, b)");
    Binary(out, "vector_minnum",       insn_vfminnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_minnum(a, b)");
    Binary(out, "vector_maxnum",       insn_vfmaxnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_maxnum(a, b)");
    Binary(out, "vector_minimumnum",   insn_vfminnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_minimumnum(a, b)");
    Binary(out, "vector_maximumnum",   insn_vfmaxnmd,   VFD, VFD, VFD, R3,      "__builtin_elementwise_maximumnum(a, b)");
    Binary(out, "builtin_cmpoeq",      insn_vfcmpoeqd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpoeqd(a, b)");
    Binary(out, "builtin_cmpone",      insn_vfcmponed,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmponed(a, b)");
    Binary(out, "builtin_cmpolt",      insn_vfcmpoltd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpoltd(a, b)");
    Binary(out, "builtin_cmpoge",      insn_vfcmpoged,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpoged(a, b)");
    Binary(out, "builtin_cmpo",        insn_vfcmpod,    VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpod(a, b)");
    Binary(out, "builtin_cmpueq",      insn_vfcmpueqd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpueqd(a, b)");
    Binary(out, "builtin_cmpune",      insn_vfcmpuned,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpuned(a, b)");
    Binary(out, "builtin_cmpult",      insn_vfcmpultd,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpultd(a, b)");
    Binary(out, "builtin_cmpuge",      insn_vfcmpuged,  VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpuged(a, b)");
    Binary(out, "builtin_cmpu",        insn_vfcmpud,    VID, VFD, VFD, R3,      "__builtin_postrisc_vfcmpud(a, b)");
    Binary(out, "builtin_merge_low",   insn_vfmergeld,  VFD, VFD, VFD, R3,      "__builtin_postrisc_vfmergeld(a, b)");
    Binary(out, "builtin_merge_high",  insn_vfmergehd,  VFD, VFD, VFD, R3,      "__builtin_postrisc_vfmergehd(a, b)");
}

CODEGEN_HANDLER(FpBinaryRm)
{
    BinaryRm(out, insn_fabdh,    FH, FH, FH, R3, "__builtin_postrisc_fabdh");
    BinaryRm(out, insn_fnabdh,   FH, FH, FH, R3, "__builtin_postrisc_fnabdh");
    BinaryRm(out, insn_faddh,    FH, FH, FH, R3, "__builtin_postrisc_faddh");
    BinaryRm(out, insn_fnaddh,   FH, FH, FH, R3, "__builtin_postrisc_fnaddh");
    BinaryRm(out, insn_fsubh,    FH, FH, FH, R3, "__builtin_postrisc_fsubh");
    BinaryRm(out, insn_fmulh,    FH, FH, FH, R3, "__builtin_postrisc_fmulh");
    BinaryRm(out, insn_fnmulh,   FH, FH, FH, R3, "__builtin_postrisc_fnmulh");
    BinaryRm(out, insn_fdivh,    FH, FH, FH, R3, "__builtin_postrisc_fdivh");

    BinaryRm(out, insn_fabds,    FS, FS, FS, R3, "__builtin_postrisc_fabds");
    BinaryRm(out, insn_fnabds,   FS, FS, FS, R3, "__builtin_postrisc_fnabds");
    BinaryRm(out, insn_fadds,    FS, FS, FS, R3, "__builtin_postrisc_fadds");
    BinaryRm(out, insn_fnadds,   FS, FS, FS, R3, "__builtin_postrisc_fnadds");
    BinaryRm(out, insn_fsubs,    FS, FS, FS, R3, "__builtin_postrisc_fsubs");
    BinaryRm(out, insn_fmuls,    FS, FS, FS, R3, "__builtin_postrisc_fmuls");
    BinaryRm(out, insn_fnmuls,   FS, FS, FS, R3, "__builtin_postrisc_fnmuls");
    BinaryRm(out, insn_fdivs,    FS, FS, FS, R3, "__builtin_postrisc_fdivs");

    BinaryRm(out, insn_fabdd,    FD, FD, FD, R3, "__builtin_postrisc_fabdd");
    BinaryRm(out, insn_fnabdd,   FD, FD, FD, R3, "__builtin_postrisc_fnabdd");
    BinaryRm(out, insn_faddd,    FD, FD, FD, R3, "__builtin_postrisc_faddd");
    BinaryRm(out, insn_fnaddd,   FD, FD, FD, R3, "__builtin_postrisc_fnaddd");
    BinaryRm(out, insn_fsubd,    FD, FD, FD, R3, "__builtin_postrisc_fsubd");
    BinaryRm(out, insn_fmuld,    FD, FD, FD, R3, "__builtin_postrisc_fmuld");
    BinaryRm(out, insn_fnmuld,   FD, FD, FD, R3, "__builtin_postrisc_fnmuld");
    BinaryRm(out, insn_fdivd,    FD, FD, FD, R3, "__builtin_postrisc_fdivd");

    BinaryRm(out, insn_fabdq,    FQ, FQ, FQ, R3, "__builtin_postrisc_fabdq");
    BinaryRm(out, insn_fnabdq,   FQ, FQ, FQ, R3, "__builtin_postrisc_fnabdq");
    BinaryRm(out, insn_faddq,    FQ, FQ, FQ, R3, "__builtin_postrisc_faddq");
    BinaryRm(out, insn_fnaddq,   FQ, FQ, FQ, R3, "__builtin_postrisc_fnaddq");
    BinaryRm(out, insn_fsubq,    FQ, FQ, FQ, R3, "__builtin_postrisc_fsubq");
    BinaryRm(out, insn_fmulq,    FQ, FQ, FQ, R3, "__builtin_postrisc_fmulq");
    BinaryRm(out, insn_fnmulq,   FQ, FQ, FQ, R3, "__builtin_postrisc_fnmulq");
    BinaryRm(out, insn_fdivq,    FQ, FQ, FQ, R3, "__builtin_postrisc_fdivq");

    BinaryRm(out, insn_fabdq,    FX, FX, FX, R3, "__builtin_postrisc_fabdq");
    BinaryRm(out, insn_fnabdq,   FX, FX, FX, R3, "__builtin_postrisc_fnabdq");
    BinaryRm(out, insn_faddq,    FX, FX, FX, R3, "__builtin_postrisc_faddq");
    BinaryRm(out, insn_fnaddq,   FX, FX, FX, R3, "__builtin_postrisc_fnaddq");
    BinaryRm(out, insn_fsubq,    FX, FX, FX, R3, "__builtin_postrisc_fsubq");
    BinaryRm(out, insn_fmulq,    FX, FX, FX, R3, "__builtin_postrisc_fmulq");
    BinaryRm(out, insn_fnmulq,   FX, FX, FX, R3, "__builtin_postrisc_fnmulq");
    BinaryRm(out, insn_fdivq,    FX, FX, FX, R3, "__builtin_postrisc_fdivq");

    BinaryRm(out, insn_vfabdh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfabdh");
    BinaryRm(out, insn_vfnabdh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfnabdh");
    BinaryRm(out, insn_vfaddh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfaddh");
    BinaryRm(out, insn_vfnaddh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfnaddh");
    BinaryRm(out, insn_vfsubh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfsubh");
    BinaryRm(out, insn_vfmulh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfmulh");
    BinaryRm(out, insn_vfnmulh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfnmulh");
    BinaryRm(out, insn_vfdivh,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfdivh");
    BinaryRm(out, insn_vfaddsubh,  VFH, VFH, VFH, R3, "__builtin_postrisc_vfaddsubh");
    BinaryRm(out, insn_vfsubaddh,  VFH, VFH, VFH, R3, "__builtin_postrisc_vfsubaddh");
    BinaryRm(out, insn_vfhaddh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfhaddh");
    BinaryRm(out, insn_vfhsubh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfhsubh");
    BinaryRm(out, insn_vfhmulh,    VFH, VFH, VFH, R3, "__builtin_postrisc_vfhmulh");
    BinaryRm(out, insn_vfdoth,     VFH, VFH, VFH, R3, "__builtin_postrisc_vfdoth");

    BinaryRm(out, insn_vfabds,     VFS, VFS, VFS, R3, "__builtin_postrisc_vfabds");
    BinaryRm(out, insn_vfnabds,    VFS, VFS, VFS, R3, "__builtin_postrisc_vfnabds");
    BinaryRm(out, insn_vfadds,     VFS, VFS, VFS, R3, "__builtin_postrisc_vfadds");
    BinaryRm(out, insn_vfnadds,    VFS, VFS, VFS, R3, "__builtin_postrisc_vfnadds");
    BinaryRm(out, insn_vfsubs,     VFS, VFS, VFS, R3, "__builtin_postrisc_vfsubs");
    BinaryRm(out, insn_vfmuls,     VFS, VFS, VFS, R3, "__builtin_postrisc_vfmuls");
    BinaryRm(out, insn_vfnmuls,    VFS, VFS, VFS, R3, "__builtin_postrisc_vfnmuls");
    BinaryRm(out, insn_vfdivs,     VFS, VFS, VFS, R3, "__builtin_postrisc_vfdivs");
    BinaryRm(out, insn_vfaddsubs,  VFS, VFS, VFS, R3, "__builtin_postrisc_vfaddsubs");
    BinaryRm(out, insn_vfsubadds,  VFS, VFS, VFS, R3, "__builtin_postrisc_vfsubadds");
    BinaryRm(out, insn_vfhadds,    VFS, VFS, VFS, R3, "__builtin_postrisc_vfhadds");
    BinaryRm(out, insn_vfhsubs,    VFS, VFS, VFS, R3, "__builtin_postrisc_vfhsubs");
    BinaryRm(out, insn_vfhmuls,    VFS, VFS, VFS, R3, "__builtin_postrisc_vfhmuls");
    BinaryRm(out, insn_vfdots,     VFS, VFS, VFS, R3, "__builtin_postrisc_vfdots");

    BinaryRm(out, insn_vfabdd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfabdd");
    BinaryRm(out, insn_vfnabdd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfnabdd");
    BinaryRm(out, insn_vfaddd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfaddd");
    BinaryRm(out, insn_vfnaddd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfnaddd");
    BinaryRm(out, insn_vfsubd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfsubd");
    BinaryRm(out, insn_vfmuld,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfmuld");
    BinaryRm(out, insn_vfnmuld,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfnmuld");
    BinaryRm(out, insn_vfdivd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfdivd");
    BinaryRm(out, insn_vfaddsubd,  VFD, VFD, VFD, R3, "__builtin_postrisc_vfaddsubd");
    BinaryRm(out, insn_vfsubaddd,  VFD, VFD, VFD, R3, "__builtin_postrisc_vfsubaddd");
    BinaryRm(out, insn_vfhaddd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfhaddd");
    BinaryRm(out, insn_vfhsubd,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfhsubd");
    BinaryRm(out, insn_vfhmuld,    VFD, VFD, VFD, R3, "__builtin_postrisc_vfhmuld");
    BinaryRm(out, insn_vfdotd,     VFD, VFD, VFD, R3, "__builtin_postrisc_vfdotd");
}

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


    Ternary(out, "fma",            insn_fmaddh,   FH, FH, FH, FH, R4, "__builtin_fmaf16(a, b, c)");
    Ternary(out, "fma_nega",       insn_fnmaddh,  FH, FH, FH, FH, R4, "__builtin_fmaf16(-a, b, c)");
    Ternary(out, "fma_negc",       insn_fmsubh,   FH, FH, FH, FH, R4, "__builtin_fmaf16(a, b, -c)");
    Ternary(out, "fma_nega_negc",  insn_fnmsubh,  FH, FH, FH, FH, R4, "__builtin_fmaf16(-a, b, -c)");

    Ternary(out, "fma",            insn_fmadds,   FS, FS, FS, FS, R4, "__builtin_fmaf(a, b, c)");
    Ternary(out, "fma_nega",       insn_fnmadds,  FS, FS, FS, FS, R4, "__builtin_fmaf(-a, b, c)");
    Ternary(out, "fma_negc",       insn_fmsubs,   FS, FS, FS, FS, R4, "__builtin_fmaf(a, b, -c)");
    Ternary(out, "fma_nega_negc",  insn_fnmsubs,  FS, FS, FS, FS, R4, "__builtin_fmaf(-a, b, -c)");

    Ternary(out, "fma",            insn_fmaddd,   FD, FD, FD, FD, R4, "__builtin_fma(a, b, c)");
    Ternary(out, "fma_nega",       insn_fnmaddd,  FD, FD, FD, FD, R4, "__builtin_fma(-a, b, c)");
    Ternary(out, "fma_negc",       insn_fmsubd,   FD, FD, FD, FD, R4, "__builtin_fma(a, b, -c)");
    Ternary(out, "fma_nega_negc",  insn_fnmsubd,  FD, FD, FD, FD, R4, "__builtin_fma(-a, b, -c)");

    Ternary(out, "fma",            insn_fmaddq,  FQ, FQ, FQ, FQ, R4, "__builtin_fmal(a, b, c)");
    Ternary(out, "fma_nega",       insn_fnmaddq, FQ, FQ, FQ, FQ, R4, "__builtin_fmal(-a, b, c)");
    Ternary(out, "fma_negc",       insn_fmsubq,  FQ, FQ, FQ, FQ, R4, "__builtin_fmal(a, b, -c)");
    Ternary(out, "fma_nega_negc",  insn_fnmsubq, FQ, FQ, FQ, FQ, R4, "__builtin_fmal(-a, b, -c)");

    Ternary(out, "fma",            insn_fmaddq,  FX, FX, FX, FX, R4, "__builtin_fmaf128(a, b, c)");
    Ternary(out, "fma_nega",       insn_fnmaddq, FX, FX, FX, FX, R4, "__builtin_fmaf128(-a, b, c)");
    Ternary(out, "fma_negc",       insn_fmsubq,  FX, FX, FX, FX, R4, "__builtin_fmaf128(a, b, -c)");
    Ternary(out, "fma_nega_negc",  insn_fnmsubq, FX, FX, FX, FX, R4, "__builtin_fmaf128(-a, b, -c)");

    Ternary(out, "vector_madd",   insn_vfmaddh,  VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(a, b, c)");
    Ternary(out, "vector_msub",   insn_vfmsubh,  VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(a, b, -c)");
    Ternary(out, "vector_nmadd",  insn_vfnmaddh, VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(-a, b, c)");
    Ternary(out, "vector_nmsub",  insn_vfnmsubh, VFH, VFH, VFH, VFH, R4, "__builtin_elementwise_fma(-a, b, -c)");

    Ternary(out, "vector_madd",   insn_vfmadds,  VFS, VFS, VFS, VFS, R4, "__builtin_elementwise_fma(a, b, c)");
    Ternary(out, "vector_msub",   insn_vfmsubs,  VFS, VFS, VFS, VFS, R4, "__builtin_elementwise_fma(a, b, -c)");
    Ternary(out, "vector_nmadd",  insn_vfnmadds, VFS, VFS, VFS, VFS, R4, "__builtin_elementwise_fma(-a, b, c)");
    Ternary(out, "vector_nmsub",  insn_vfnmsubs, VFS, VFS, VFS, VFS, R4, "__builtin_elementwise_fma(-a, b, -c)");

    Ternary(out, "vector_madd",   insn_vfmaddd,  VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(a, b, c)");
    Ternary(out, "vector_msub",   insn_vfmsubd,  VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(a, b, -c)");
    Ternary(out, "vector_nmadd",  insn_vfnmaddd, VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(-a, b, c)");
    Ternary(out, "vector_nmsub",  insn_vfnmsubd, VFD, VFD, VFD, VFD, R4, "__builtin_elementwise_fma(-a, b, -c)");

    Ternary(out, "merge",          insn_fmergeh,  FH, FH, FH, FH, R4, "__builtin_postrisc_fmergeh(a, b, c)");
    Ternary(out, "merge",          insn_fmerges,  FS, FS, FS, FS, R4, "__builtin_postrisc_fmerges(a, b, c)");
    Ternary(out, "merge",          insn_fmerged,  FD, FD, FD, FD, R4, "__builtin_postrisc_fmerged(a, b, c)");
    Ternary(out, "merge",          insn_fmergeq,  FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fmergeq(a, b, c)");

    Ternary(out, "postrisc_merge", insn_vfmergeh, VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmergeh(a, b, c)");
    Ternary(out, "postrisc_merge", insn_vfmerges, VFS, VFS, VFS, VFS, R4, "__builtin_postrisc_vfmerges(a, b, c)");
    Ternary(out, "postrisc_merge", insn_vfmerged, VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmerged(a, b, c)");

    Ternary(out, "vpermb",         insn_vpermb,   VUB, VUB, VUB, VUB, R4, "__builtin_postrisc_vpermb(a, b, c)");
}

CODEGEN_HANDLER(FpTernaryRm)
{
    TernaryRm(out, insn_fmaddh,   FH, FH, FH, FH, R4, "__builtin_postrisc_fmaddh");
    TernaryRm(out, insn_fmsubh,   FH, FH, FH, FH, R4, "__builtin_postrisc_fmsubh");
    TernaryRm(out, insn_fnmaddh,  FH, FH, FH, FH, R4, "__builtin_postrisc_fnmaddh");
    TernaryRm(out, insn_fnmsubh,  FH, FH, FH, FH, R4, "__builtin_postrisc_fnmsubh");

    TernaryRm(out, insn_fmadds,   FS, FS, FS, FS, R4, "__builtin_postrisc_fmadds");
    TernaryRm(out, insn_fmsubs,   FS, FS, FS, FS, R4, "__builtin_postrisc_fmsubs");
    TernaryRm(out, insn_fnmadds,  FS, FS, FS, FS, R4, "__builtin_postrisc_fnmadds");
    TernaryRm(out, insn_fnmsubs,  FS, FS, FS, FS, R4, "__builtin_postrisc_fnmsubs");

    TernaryRm(out, insn_fmaddd,   FD, FD, FD, FD, R4, "__builtin_postrisc_fmaddd");
    TernaryRm(out, insn_fmsubd,   FD, FD, FD, FD, R4, "__builtin_postrisc_fmsubd");
    TernaryRm(out, insn_fnmaddd,  FD, FD, FD, FD, R4, "__builtin_postrisc_fnmaddd");
    TernaryRm(out, insn_fnmsubd,  FD, FD, FD, FD, R4, "__builtin_postrisc_fnmsubd");

    TernaryRm(out, insn_fmaddq,   FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fmaddq");
    TernaryRm(out, insn_fmsubq,   FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fmsubq");
    TernaryRm(out, insn_fnmaddq,  FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fnmaddq");
    TernaryRm(out, insn_fnmsubq,  FQ, FQ, FQ, FQ, R4, "__builtin_postrisc_fnmsubq");

    TernaryRm(out, insn_fmaddq,   FX, FX, FX, FX, R4, "__builtin_postrisc_fmaddq");
    TernaryRm(out, insn_fmsubq,   FX, FX, FX, FX, R4, "__builtin_postrisc_fmsubq");
    TernaryRm(out, insn_fnmaddq,  FX, FX, FX, FX, R4, "__builtin_postrisc_fnmaddq");
    TernaryRm(out, insn_fnmsubq,  FX, FX, FX, FX, R4, "__builtin_postrisc_fnmsubq");

    TernaryRm(out, insn_vfmaddh,     VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmaddh");
    TernaryRm(out, insn_vfmsubh,     VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmsubh");
    TernaryRm(out, insn_vfnmaddh,    VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfnmaddh");
    TernaryRm(out, insn_vfnmsubh,    VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfnmsubh");
    TernaryRm(out, insn_vfmaddsubh,  VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmaddsubh");
    TernaryRm(out, insn_vfmsubaddh,  VFH, VFH, VFH, VFH, R4, "__builtin_postrisc_vfmsubaddh");

    TernaryRm(out, insn_vfmadds,     VFS, VFS, VFS, VFS, R4, "__builtin_postrisc_vfmadds");
    TernaryRm(out, insn_vfmsubs,     VFS, VFS, VFS, VFS, R4, "__builtin_postrisc_vfmsubs");
    TernaryRm(out, insn_vfnmadds,    VFS, VFS, VFS, VFS, R4, "__builtin_postrisc_vfnmadds");
    TernaryRm(out, insn_vfnmsubs,    VFS, VFS, VFS, VFS, R4, "__builtin_postrisc_vfnmsubs");
    TernaryRm(out, insn_vfmaddsubs,  VFS, VFS, VFS, VFS, R4, "__builtin_postrisc_vfmaddsubs");
    TernaryRm(out, insn_vfmsubadds,  VFS, VFS, VFS, VFS, R4, "__builtin_postrisc_vfmsubadds");

    TernaryRm(out, insn_vfmaddd,     VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmaddd");
    TernaryRm(out, insn_vfmsubd,     VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmsubd");
    TernaryRm(out, insn_vfnmaddd,    VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfnmaddd");
    TernaryRm(out, insn_vfnmsubd,    VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfnmsubd");
    TernaryRm(out, insn_vfmaddsubd,  VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmaddsubd");
    TernaryRm(out, insn_vfmsubaddd,  VFD, VFD, VFD, VFD, R4, "__builtin_postrisc_vfmsubaddd");
}

CODEGEN_HANDLER(FpStrict)
{
    Binary(out, "add",      insn_faddh,  FH, FH, FH, R3, "a + b");
    Binary(out, "nadd",     insn_fnaddh, FH, FH, FH, R3, "-(a + b)");
    Binary(out, "sub",      insn_fsubh,  FH, FH, FH, R3, "a - b");
    Binary(out, "mul",      insn_fmulh,  FH, FH, FH, R3, "a * b");
    Binary(out, "nmul",     insn_fnmulh, FH, FH, FH, R3, "-(a * b)");
    Binary(out, "div",      insn_fdivh,  FH, FH, FH, R3, "a / b");

    Binary(out, "add",      insn_fadds,  FS, FS, FS, R3, "a + b");
    Binary(out, "nadd",     insn_fnadds, FS, FS, FS, R3, "-(a + b)");
    Binary(out, "sub",      insn_fsubs,  FS, FS, FS, R3, "a - b");
    Binary(out, "mul",      insn_fmuls,  FS, FS, FS, R3, "a * b");
    Binary(out, "nmul",     insn_fnmuls, FS, FS, FS, R3, "-(a * b)");
    Binary(out, "div",      insn_fdivs,  FS, FS, FS, R3, "a / b");

    Binary(out, "add",      insn_faddd,  FD, FD, FD, R3, "a + b");
    Binary(out, "nadd",     insn_fnaddd, FD, FD, FD, R3, "-(a + b)");
    Binary(out, "sub",      insn_fsubd,  FD, FD, FD, R3, "a - b");
    Binary(out, "mul",      insn_fmuld,  FD, FD, FD, R3, "a * b");
    Binary(out, "nmul",     insn_fnmuld, FD, FD, FD, R3, "-(a * b)");
    Binary(out, "div",      insn_fdivd,  FD, FD, FD, R3, "a / b");

    Binary(out, "add",      insn_faddq,  FQ, FQ, FQ, R3, "a + b");
    Binary(out, "nadd",     insn_fnaddq, FQ, FQ, FQ, R3, "-(a + b)");
    Binary(out, "sub",      insn_fsubq,  FQ, FQ, FQ, R3, "a - b");
    Binary(out, "mul",      insn_fmulq,  FQ, FQ, FQ, R3, "a * b");
    Binary(out, "nmul",     insn_fnmulq, FQ, FQ, FQ, R3, "-(a * b)");
    Binary(out, "div",      insn_fdivq,  FQ, FQ, FQ, R3, "a / b");

    Binary(out, "fcmpoeq",  insn_fcmpoeqh, BOOL, FH, FH, R3, "a == b");
    Binary(out, "fcmpone",  insn_fcmponeh, BOOL, FH, FH, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b"); /*FIXME: ONE===>UNE*/
    Binary(out, "fcmpolt",  insn_fcmpolth, BOOL, FH, FH, R3, "a < b" );
    Binary(out, "fcmpoge",  insn_fcmpogeh, BOOL, FH, FH, R3, "a >= b");
    Binary(out, "fcmpueq",  insn_fcmpueqh, BOOL, FH, FH, R3, "!(a < b || a > b)"); /* trick to force UEQ */
    Binary(out, "fcmpune",  insn_fcmpuneh, BOOL, FH, FH, R3, "!(a == b)");
    Binary(out, "fcmpult",  insn_fcmpulth, BOOL, FH, FH, R3, "!(a >= b)" );
    Binary(out, "fcmpuge",  insn_fcmpugeh, BOOL, FH, FH, R3, "!(a < b)");
    Binary(out, "fcmpu",    insn_fcmpuh,   BOOL, FH, FH, R3, "__builtin_isnan(a) || __builtin_isnan(b)");
    Binary(out, "fcmpo",    insn_fcmpoh,   BOOL, FH, FH, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)");

    Binary(out, "fcmpole",  insn_fcmpogeh, BOOL, FH, FH, R3_SWAP, "a <= b");
    Binary(out, "fcmpogt",  insn_fcmpolth, BOOL, FH, FH, R3_SWAP, "a > b" );
    Binary(out, "fcmpule",  insn_fcmpugeh, BOOL, FH, FH, R3_SWAP, "!(a > b)");
    Binary(out, "fcmpugt",  insn_fcmpulth, BOOL, FH, FH, R3_SWAP, "!(a <= b)" );

    Binary(out, "fcmpoeq",  insn_fcmpoeqs, BOOL, FS, FS, R3, "a == b");
    Binary(out, "fcmpone",  insn_fcmpones, BOOL, FS, FS, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b"); /*FIXME: ONE===>UNE*/
    Binary(out, "fcmpolt",  insn_fcmpolts, BOOL, FS, FS, R3, "a < b" );
    Binary(out, "fcmpoge",  insn_fcmpoges, BOOL, FS, FS, R3, "a >= b");
    Binary(out, "fcmpueq",  insn_fcmpueqs, BOOL, FS, FS, R3, "!(a < b || a > b)"); /* trick to force UEQ */
    Binary(out, "fcmpune",  insn_fcmpunes, BOOL, FS, FS, R3, "!(a == b)");
    Binary(out, "fcmpult",  insn_fcmpults, BOOL, FS, FS, R3, "!(a >= b)" );
    Binary(out, "fcmpuge",  insn_fcmpuges, BOOL, FS, FS, R3, "!(a < b)");
    Binary(out, "fcmpu",    insn_fcmpus,   BOOL, FS, FS, R3, "__builtin_isnan(a) || __builtin_isnan(b)");
    Binary(out, "fcmpo",    insn_fcmpos,   BOOL, FS, FS, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)");

    Binary(out, "fcmpole",  insn_fcmpoges, BOOL, FS, FS, R3_SWAP, "a <= b");
    Binary(out, "fcmpogt",  insn_fcmpolts, BOOL, FS, FS, R3_SWAP, "a > b" );
    Binary(out, "fcmpule",  insn_fcmpuges, BOOL, FS, FS, R3_SWAP, "!(a > b)");
    Binary(out, "fcmpugt",  insn_fcmpults, BOOL, FS, FS, R3_SWAP, "!(a <= b)" );

    Binary(out, "fcmpoeq",  insn_fcmpoeqd, BOOL, FD, FD, R3, "a == b");
    Binary(out, "fcmpone",  insn_fcmponed, BOOL, FD, FD, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b"); /*FIXME: ONE===>UNE*/
    Binary(out, "fcmpolt",  insn_fcmpoltd, BOOL, FD, FD, R3, "a < b" );
    Binary(out, "fcmpoge",  insn_fcmpoged, BOOL, FD, FD, R3, "a >= b");
    Binary(out, "fcmpueq",  insn_fcmpueqd, BOOL, FD, FD, R3, "!(a < b || a > b)"); /* trick to force UEQ */
    Binary(out, "fcmpune",  insn_fcmpuned, BOOL, FD, FD, R3, "!(a == b)");
    Binary(out, "fcmpult",  insn_fcmpultd, BOOL, FD, FD, R3, "!(a >= b)" );
    Binary(out, "fcmpuge",  insn_fcmpuged, BOOL, FD, FD, R3, "!(a < b)");
    Binary(out, "fcmpu",    insn_fcmpud,   BOOL, FD, FD, R3, "__builtin_isnan(a) || __builtin_isnan(b)");
    Binary(out, "fcmpo",    insn_fcmpod,   BOOL, FD, FD, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)");

    Binary(out, "fcmpole",  insn_fcmpoged, BOOL, FD, FD, R3_SWAP, "a <= b");
    Binary(out, "fcmpogt",  insn_fcmpoltd, BOOL, FD, FD, R3_SWAP, "a > b");
    Binary(out, "fcmpule",  insn_fcmpuged, BOOL, FD, FD, R3_SWAP, "!(a > b)");
    Binary(out, "fcmpugt",  insn_fcmpultd, BOOL, FD, FD, R3_SWAP, "!(a <= b)");

    Binary(out, "fcmpoeq",  insn_fcmpoeqq, BOOL, FQ, FQ, R3, "a == b");
    Binary(out, "fcmpone",  insn_fcmponeq, BOOL, FQ, FQ, R3, "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b"); /*FIXME: ONE===>UNE*/
    Binary(out, "fcmpolt",  insn_fcmpoltq, BOOL, FQ, FQ, R3, "a < b" );
    Binary(out, "fcmpoge",  insn_fcmpogeq, BOOL, FQ, FQ, R3, "a >= b");
    Binary(out, "fcmpueq",  insn_fcmpueqq, BOOL, FQ, FQ, R3, "!(a < b || a > b)"); /* trick to force UEQ */
    Binary(out, "fcmpune",  insn_fcmpuneq, BOOL, FQ, FQ, R3, "!(a == b)");
    Binary(out, "fcmpult",  insn_fcmpultq, BOOL, FQ, FQ, R3, "!(a >= b)" );
    Binary(out, "fcmpuge",  insn_fcmpugeq, BOOL, FQ, FQ, R3, "!(a < b)");
    Binary(out, "fcmpu",    insn_fcmpuq,   BOOL, FQ, FQ, R3, "__builtin_isnan(a) || __builtin_isnan(b)");
    Binary(out, "fcmpo",    insn_fcmpoq,   BOOL, FQ, FQ, R3, "!__builtin_isnan(a) && !__builtin_isnan(b)");

    Binary(out, "fcmpole",  insn_fcmpogeq, BOOL, FQ, FQ, R3_SWAP, "a <= b");
    Binary(out, "fcmpogt",  insn_fcmpoltq, BOOL, FQ, FQ, R3_SWAP, "a > b" );
    Binary(out, "fcmpule",  insn_fcmpugeq, BOOL, FQ, FQ, R3_SWAP, "!(a > b)");
    Binary(out, "fcmpugt",  insn_fcmpultq, BOOL, FQ, FQ, R3_SWAP, "!(a <= b)" );

    Binary(out, "vfcmpoeq",  insn_vfcmpoeqh, VIH, VFH, VFH, R3, "a == b");
    Binary(out, "vfcmpolt",  insn_vfcmpolth, VIH, VFH, VFH, R3, "a < b" );
    Binary(out, "vfcmpoge",  insn_vfcmpogeh, VIH, VFH, VFH, R3, "a >= b");
    Binary(out, "vfcmpueq",  insn_vfcmpueqh, VIH, VFH, VFH, R3, "!(a < b || a > b)"); /* trick to force UEQ */
    Binary(out, "vfcmpune",  insn_vfcmpuneh, VIH, VFH, VFH, R3, "!(a == b)");
    Binary(out, "vfcmpult",  insn_vfcmpulth, VIH, VFH, VFH, R3, "!(a >= b)" );
    Binary(out, "vfcmpuge",  insn_vfcmpugeh, VIH, VFH, VFH, R3, "!(a < b)");
    Binary(out, "vfcmpole",  insn_vfcmpogeh, VIH, VFH, VFH, R3_SWAP, "a <= b");
    Binary(out, "vfcmpogt",  insn_vfcmpolth, VIH, VFH, VFH, R3_SWAP, "a > b");
    Binary(out, "vfcmpule",  insn_vfcmpugeh, VIH, VFH, VFH, R3_SWAP, "!(a > b)");
    Binary(out, "vfcmpugt",  insn_vfcmpulth, VIH, VFH, VFH, R3_SWAP, "!(a <= b)");

    Binary(out, "vfcmpoeq",  insn_vfcmpoeqs, VIW, VFS, VFS, R3, "a == b");
    Binary(out, "vfcmpolt",  insn_vfcmpolts, VIW, VFS, VFS, R3, "a < b" );
    Binary(out, "vfcmpoge",  insn_vfcmpoges, VIW, VFS, VFS, R3, "a >= b");
    Binary(out, "vfcmpueq",  insn_vfcmpueqs, VIW, VFS, VFS, R3, "!(a < b || a > b)"); /* trick to force UEQ */
    Binary(out, "vfcmpune",  insn_vfcmpunes, VIW, VFS, VFS, R3, "!(a == b)");
    Binary(out, "vfcmpult",  insn_vfcmpults, VIW, VFS, VFS, R3, "!(a >= b)" );
    Binary(out, "vfcmpuge",  insn_vfcmpuges, VIW, VFS, VFS, R3, "!(a < b)");
    Binary(out, "vfcmpole",  insn_vfcmpoges, VIW, VFS, VFS, R3_SWAP, "a <= b");
    Binary(out, "vfcmpogt",  insn_vfcmpolts, VIW, VFS, VFS, R3_SWAP, "a > b");
    Binary(out, "vfcmpule",  insn_vfcmpuges, VIW, VFS, VFS, R3_SWAP, "!(a > b)");
    Binary(out, "vfcmpugt",  insn_vfcmpults, VIW, VFS, VFS, R3_SWAP, "!(a <= b)");

    Binary(out, "vfcmpoeq",  insn_vfcmpoeqd, VID, VFD, VFD, R3, "a == b");
    Binary(out, "vfcmpolt",  insn_vfcmpoltd, VID, VFD, VFD, R3, "a < b" );
    Binary(out, "vfcmpoge",  insn_vfcmpoged, VID, VFD, VFD, R3, "a >= b");
    Binary(out, "vfcmpueq",  insn_vfcmpueqd, VID, VFD, VFD, R3, "!(a < b || a > b)"); /* trick to force UEQ */
    Binary(out, "vfcmpune",  insn_vfcmpuned, VID, VFD, VFD, R3, "!(a == b)");
    Binary(out, "vfcmpult",  insn_vfcmpultd, VID, VFD, VFD, R3, "!(a >= b)" );
    Binary(out, "vfcmpuge",  insn_vfcmpuged, VID, VFD, VFD, R3, "!(a < b)");
    Binary(out, "vfcmpole",  insn_vfcmpoged, VID, VFD, VFD, R3_SWAP, "a <= b");
    Binary(out, "vfcmpogt",  insn_vfcmpoltd, VID, VFD, VFD, R3_SWAP, "a > b");
    Binary(out, "vfcmpule",  insn_vfcmpuged, VID, VFD, VFD, R3_SWAP, "!(a > b)");
    Binary(out, "vfcmpugt",  insn_vfcmpultd, VID, VFD, VFD, R3_SWAP, "!(a <= b)");
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
    static const u16 excludes[] = {
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
    for (const u16 x : excludes) {
         out << "// " << std::bitset<10>(x) << ' ' << x << '\n';
    }
    out << '\n';

    static const constexpr struct {
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
