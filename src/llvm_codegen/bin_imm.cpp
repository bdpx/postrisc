#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

 // x <= 0 => x <  1
 // x >  0 => x >= 1

class zeroes_block {
public:
    template<typename T> static T func(int size, int pos) { return ~(util::makemask<T>(size) << pos); }
};

class ones_block {
public:
    template<typename T> static T func(int size, int pos) { return util::makemask<T>(size) << pos; }
};

template<typename T, typename OP>
static bool CheckBlock(T value, int& fit_size, int& fit_pos)
{
    for (int size = 1; size < static_cast<int>(8*sizeof(T)); size++) {
        for(int pos = 0; pos + size <= static_cast<int>(8*sizeof(T)); pos++) {
            const T mask = OP::template func<T>(size, pos);
            if (value == mask) {
                fit_size = size;
                fit_pos = pos;
                return true;
            }
        }
    }
    return false;
}

template<typename ITYPE, typename UTYPE>
void Generator::BinaryRegImmCase(std::ostream& out, const func_info& frag, const imm_data<ITYPE>& imm, const std::string& name)
{
    const int population = util::population::func(static_cast<UTYPE>(imm.value));

    // FIXME: llvm overoptimization for min/max imm?
    // can't emit - minuid.l %r1, %r1, 9223372036854775807
    if (frag.insn == insn_minuid || frag.insn == insn_maxuid) {
        if (imm.value == 0x7FFFFFFFFFFFFFFF && (
            name == "min_ge_baab_u64_9223372036854775807" ||
            name == "min_le_abab_u64_9223372036854775807" ||
            name == "max_le_abba_u64_9223372036854775807" ||
            name == "max_ge_baba_u64_9223372036854775807"
         )) {
            out << CHECK << insn_ldi << ".l %r2, " << 0x7FFFFFFFFFFFFFFF << '\n';
            out << CHECK_EMPTY << '\n';
            out << CHECK_NEXT << "! BUNDLE\n";
            if (frag.insn == insn_minuid)
                out << CHECK_NEXT << "cmovltd %r1, %r1, %r2, %r1\n";
            else
                out << CHECK_NEXT << "cmovltd %r1, %r1, %r1, %r2\n";
            return;
        }
    }

    if (frag.insn == insn_addid) {
        // check possible replacement: ADD_IMM_I64 ===> LDA_N
        if constexpr (std::is_same<i64, ITYPE>::value) {
            const int skip = 64 - bits_per_mid_binimm - bits_per_lda_near_shift;
            if ((imm.value & util::makemask<u64>(bits_per_lda_near_shift)) == 0 && ((imm.value << skip) >> skip) == imm.value) {
                out << CHECK << insn_ldan << " %r1, %r1, " << (imm.value >> bits_per_lda_near_shift) << '\n';
                return;
            }
        }
    }

    if (frag.insn == insn_andi) {
        // AND_IMM with one-zero mask is replaced by BIT_CLEAR
        if (population == sizeof(ITYPE)*8-1) {
            const unsigned bitpos = util::count_trailing_zeros::func(~imm.value);
            out << CHECK << insn_bitclr << " %r1, %r1, " << bitpos << '\n';
            return;
        }

        // check possible replacement: AND_IMM ===> DEPOSIT
        if (name == "and_imm_i64_p4611686018427387903") {
            out << CHECK << insn_deposit << " %r1, %gz, %r1, 0, 62\n";
            return;
        }
        if (name == "and_imm_i64_m4611686018427387904") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 0, 62\n";
            return;
        }
        if (name == "and_imm_i32_m134217728") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 0, 27\n";
            return;
        }
        if (name == "and_imm_i64_p1048575") {
            out << CHECK << insn_deposit << " %r1, %gz, %r1, 0, 20\n";
            return;
        }
        if (name == "and_imm_i32_m1048576" || name == "and_imm_i64_m1048576") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 0, 20\n";
            return;
        }
        if (name == "and_imm_i32_p1048575") {
            out << CHECK << insn_deposit << " %r1, %gz, %r1, 0, 20\n";
            return;
        }
        if (name == "and_imm_i32_p134217727") {
            out << CHECK << insn_deposit << " %r1, %gz, %r1, 0, 27\n";
            return;
        }
        if (name == "and_imm_i64_m4611686018427387901") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 2, 60\n";
            return;
        }
        if (name == "and_imm_i64_p4611686018427387903") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 62, 2\n";
            return;
        }
        if (name == "and_imm_i64_m1048573" || name == "and_imm_i32_m1048573") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 2, 18\n";
           return;
        }
        if (name == "and_imm_i64_p1048575") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 20, 44\n";
            return;
        }

        if (name == "and_imm_i32_m2147483645" || name == "and_imm_i64_m2147483645") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 2, 29\n";
            return;
        }

        if (name == "and_imm_i32_p1048575") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 20, 12\n";
            return;
        }

        if (name == "and_imm_i32_p134217727") {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, 27, 5\n";
            return;
        }

/*
        int fit_size = 0;
        int fit_pos = 0;
        if (CheckBlock<ITYPE, zeroes_block>(imm.value, fit_size, fit_pos)) {
            out << CHECK << insn_deposit << " %r1, %r1, %gz, " << fit_pos << ", " << fit_size << '\n';
            return;
        }
        if (CheckBlock<ITYPE, ones_block>(imm.value, fit_size, fit_pos)) {
            out << CHECK << insn_deposit << " %r1, %gz, %r1, " << fit_pos << ", " << fit_size << '\n';
            return;
        }
*/
    }

    if (std::is_signed_v<ITYPE> && imm.value < 0 && (
        frag.insn == insn_remsiw ||
        frag.insn == insn_remsid
      )) {
        // FIXME: .l prefix changed
        //out << CHECK << frag.insn << (imm.mode == LONG ? ".l" : "")
        //    << " %r1, %r1, " << (-imm.value) << '\n';
        return;
    }

    // div_imm_uXX by 2^N replaced by srl_i64[32]
    // rem_imm_uXX by 2^N replaced by AND with (2^N-1)
    if (population == 1 && (
        frag.insn == insn_divuiw ||
        frag.insn == insn_divuid ||
        frag.insn == insn_remuiw ||
        frag.insn == insn_remuid
        )) {

        const unsigned bitpos = util::count_trailing_zeros::func(imm.value);
        switch (frag.insn) {
            case insn_divuiw:
                out << CHECK << insn_srliw << " %r1, %r1, " << bitpos << '\n';
                break;
            case insn_divuid:
                out << CHECK << insn_srlid << " %r1, %r1, " << bitpos << '\n';
                break;
            case insn_remuiw:
            case insn_remuid:
                // FIXME: .l prefix changed
                // out << CHECK << insn_andi << " %r1, %r1, " << (imm.value-1) << '\n';
                break;
            default:
                break;
        }
        return;
    }

    // mul_imm by -2^N replaced by sl_sub_i64[32]: 0 - (X << N)
    if ((frag.insn == insn_muliws || frag.insn == insn_mulid) &&
         util::population::func(static_cast<UTYPE>(-imm.value)) == 1) {

        const unsigned bitpos = util::count_trailing_zeros::func(-imm.value);
        // sl_sub_i64 %r1, %gz, %r1, 20
        switch (frag.insn) {
            case insn_muliws:
                out << CHECK << insn_slsubws << " %r1, %gz, %r1, " << bitpos << '\n';
                break;
            case insn_mulid:
                out << CHECK << insn_slsubd << " %r1, %gz, %r1, " << bitpos << '\n';
                break;
            default:
                break;
        }
        return;
    }

    // CHECK: op_imm.l %r1, %r1, imm
    out << CHECK << InsnCounter(this, frag.insn) << (imm.mode == LONG ? ".l" : "")
        << " %r1, %r1, " << imm.value << '\n';
}

template<typename ITYPE, typename UTYPE>
void Generator::BinaryRegImm(std::ostream& out, const std::span<const func_info> tests)
{
    for (const auto& frag : tests) {
        for (const auto& imm : immediates<ITYPE>::binimm_immediates) {
            std::ostringstream name;
            name << frag.name << "_" << frag.type;

            if constexpr (std::is_signed_v<ITYPE>) {
                if (imm.value < 0) {
                    name << "_m" << -imm.value;
                } else {
                    name << "_p" << imm.value;
                }
            } else {
                name << "_" << imm.value;
            }

            out << CHECK_LABEL << name.str() << '\n';
            BinaryRegImmCase<ITYPE, UTYPE>(out, frag, imm, name.str());

            out << frag.type << " " << name.str()  << "(" << frag.type << " value) {\n";
            const ITYPE cpp_imm = imm.value - frag.offset;
            out << "    const " << frag.type << " imm = " << cpp_imm << ";\n";
            out << "    return " << frag.c_fragment << ";\n";
            out << "}\n\n";
        }

        out << '\n';
    }
}

// for all i32,i64,i128 same bitwise instructions are used
// cm-_eq|ne is signed for both i32|u32

static const func_info func_info_i32[] = {
    { "and_imm",     insn_andi,        IW,  "value & imm",     0 },
    { "or_imm",      insn_ori,         IW,  "value | imm",     0 },
    { "xor_imm",     insn_xori,        IW,  "value ^ imm",     0 },
    { "andn_imm",    insn_andni,       IW,  "(~value) & imm",  0 },
    { "orn_imm",     insn_orni,        IW,  "(~value) | imm",  0 },
    { "add_imm",     insn_addiws,      IW,  "value + imm",     0 },
    { "cmp_eq",      insn_cmpeqiw,     IW,  "value == imm",    0 },
    { "cmp_ne",      insn_cmpneiw,     IW,  "value != imm",    0 },
    { "cmp_lt",      insn_cmpltsiw,    IW,  "value < imm",     0 },
    { "cmp_ge",      insn_cmpgesiw,    IW,  "value >= imm",    0 },
    { "cmp_le",      insn_cmpltsiw,    IW,  "value <= imm",    1 },
    { "cmp_gt",      insn_cmpgesiw,    IW,  "value > imm",     1 },
    { "subr_imm",    insn_subriws,     IW,  "imm - value",     0 },
    { "mul_imm",     insn_muliws,      IW,  "value * imm",     0 },
    { "div_imm",     insn_divsiw,      IW,  "value / imm",     0 },
    { "rem_imm",     insn_remsiw,      IW,  "value % imm",     0 },

    { "min_lt_abab", insn_minsiw,  IW,  "value <  imm ? value : imm", 0 },
    { "max_ge_abab", insn_maxsiw,  IW,  "value >= imm ? value : imm", 0 },
    { "min_le_abab", insn_minsiw,  IW,  "value <= imm ? value : imm", 0 },
    { "max_gt_abab", insn_maxsiw,  IW,  "value >  imm ? value : imm", 0 },
    { "max_lt_baab", insn_maxsiw,  IW,  "imm <  value ? value : imm", 0 },
    { "min_ge_baab", insn_minsiw,  IW,  "imm >= value ? value : imm", 0 },
    { "max_le_baab", insn_maxsiw,  IW,  "imm <= value ? value : imm", 0 },
    { "min_gt_baab", insn_minsiw,  IW,  "imm >  value ? value : imm", 0 },
    { "max_lt_abba", insn_maxsiw,  IW,  "value <  imm ? imm : value", 0 },
    { "min_ge_abba", insn_minsiw,  IW,  "value >= imm ? imm : value", 0 },
    { "max_le_abba", insn_maxsiw,  IW,  "value <= imm ? imm : value", 0 },
    { "min_gt_abba", insn_minsiw,  IW,  "value >  imm ? imm : value", 0 },
    { "min_lt_baba", insn_minsiw,  IW,  "imm <  value ? imm : value", 0 },
    { "max_ge_baba", insn_maxsiw,  IW,  "imm >= value ? imm : value", 0 },
    { "min_le_baba", insn_minsiw,  IW,  "imm <= value ? imm : value", 0 },
    { "max_gt_baba", insn_maxsiw,  IW,  "imm >  value ? imm : value", 0 },
};

static const func_info func_info_u32[] = {
    { "cmp_lt",      insn_cmpltuiw, UW,  "value < imm",  0 },
    { "cmp_ge",      insn_cmpgeuiw, UW,  "value >= imm", 0 },
    { "cmp_le",      insn_cmpltuiw, UW,  "value <= imm", 1 },
    { "cmp_gt",      insn_cmpgeuiw, UW,  "value > imm",  1 },
    { "div_imm",     insn_divuiw,   UW,  "value / imm", 0 },
    { "rem_imm",     insn_remuiw,   UW,  "value % imm", 0 },

    { "min_lt_abab", insn_minuiw,  UW,  "value <  imm ? value : imm", 0 },
    { "max_ge_abab", insn_maxuiw,  UW,  "value >= imm ? value : imm", 0 },
    { "min_le_abab", insn_minuiw,  UW,  "value <= imm ? value : imm", 0 },
    { "max_gt_abab", insn_maxuiw,  UW,  "value >  imm ? value : imm", 0 },
    { "max_lt_baab", insn_maxuiw,  UW,  "imm <  value ? value : imm", 0 },
    { "min_ge_baab", insn_minuiw,  UW,  "imm >= value ? value : imm", 0 },
    { "max_le_baab", insn_maxuiw,  UW,  "imm <= value ? value : imm", 0 },
    { "min_gt_baab", insn_minuiw,  UW,  "imm >  value ? value : imm", 0 },
    { "max_lt_abba", insn_maxuiw,  UW,  "value <  imm ? imm : value", 0 },
    { "min_ge_abba", insn_minuiw,  UW,  "value >= imm ? imm : value", 0 },
    { "max_le_abba", insn_maxuiw,  UW,  "value <= imm ? imm : value", 0 },
    { "min_gt_abba", insn_minuiw,  UW,  "value >  imm ? imm : value", 0 },
    { "min_lt_baba", insn_minuiw,  UW,  "imm <  value ? imm : value", 0 },
    { "max_ge_baba", insn_maxuiw,  UW,  "imm >= value ? imm : value", 0 },
    { "min_le_baba", insn_minuiw,  UW,  "imm <= value ? imm : value", 0 },
    { "max_gt_baba", insn_maxuiw,  UW,  "imm >  value ? imm : value", 0 },
};

static const func_info func_info_i64[] = {
    { "and_imm",     insn_andi,      ID,  "value & imm",     0 },
    { "or_imm",      insn_ori,       ID,  "value | imm",     0 },
    { "xor_imm",     insn_xori,      ID,  "value ^ imm",     0 },
    { "andn_imm",    insn_andni,     ID,  "(~value) & imm",  0 },
    { "orn_imm",     insn_orni,      ID,  "(~value) | imm",  0 },
    { "add_imm",     insn_addid,     ID,  "value + imm",     0 },
    { "cmp_eq",      insn_cmpeqid,   ID,  "value == imm",    0 },
    { "cmp_ne",      insn_cmpneid,   ID,  "value != imm",    0 },
    { "cmp_lt",      insn_cmpltsid,  ID,  "value < imm",     0 },
    { "cmp_ge",      insn_cmpgesid,  ID,  "value >= imm",    0 },
    { "cmp_le",      insn_cmpltsid,  ID,  "value <= imm",    1 },
    { "cmp_gt",      insn_cmpgesid,  ID,  "value > imm",     1 },
    { "subr_imm",    insn_subrid,    ID,  "imm - value",     0 },
    { "mul_imm",     insn_mulid,     ID,  "value * imm",     0 },
    { "div_imm",     insn_divsid,    ID,  "value / imm",     0 },
    { "rem_imm",     insn_remsid,    ID,  "value % imm",     0 },

    { "min_lt_abab", insn_minsid,  ID,  "value <  imm ? value : imm", 0 },
    { "max_ge_abab", insn_maxsid,  ID,  "value >= imm ? value : imm", 0 },
    { "min_le_abab", insn_minsid,  ID,  "value <= imm ? value : imm", 0 },
    { "max_gt_abab", insn_maxsid,  ID,  "value >  imm ? value : imm", 0 },
    { "max_lt_baab", insn_maxsid,  ID,  "imm <  value ? value : imm", 0 },
    { "min_ge_baab", insn_minsid,  ID,  "imm >= value ? value : imm", 0 },
    { "max_le_baab", insn_maxsid,  ID,  "imm <= value ? value : imm", 0 },
    { "min_gt_baab", insn_minsid,  ID,  "imm >  value ? value : imm", 0 },
    { "max_lt_abba", insn_maxsid,  ID,  "value <  imm ? imm : value", 0 },
    { "min_ge_abba", insn_minsid,  ID,  "value >= imm ? imm : value", 0 },
    { "max_le_abba", insn_maxsid,  ID,  "value <= imm ? imm : value", 0 },
    { "min_gt_abba", insn_minsid,  ID,  "value >  imm ? imm : value", 0 },
    { "min_lt_baba", insn_minsid,  ID,  "imm <  value ? imm : value", 0 },
    { "max_ge_baba", insn_maxsid,  ID,  "imm >= value ? imm : value", 0 },
    { "min_le_baba", insn_minsid,  ID,  "imm <= value ? imm : value", 0 },
    { "max_gt_baba", insn_maxsid,  ID,  "imm >  value ? imm : value", 0 },
};

static const func_info func_info_u64[] = {
    { "cmp_lt",      insn_cmpltuid, UD,  "value < imm",  0 },
    { "cmp_ge",      insn_cmpgeuid, UD,  "value >= imm", 0 },
    { "cmp_le",      insn_cmpltuid, UD,  "value <= imm", 1 },
    { "cmp_gt",      insn_cmpgeuid, UD,  "value > imm",  1 },
    { "div_imm",     insn_divuid,   UD,  "value / imm", 0 },
    { "rem_imm",     insn_remuid,   UD,  "value % imm", 0 },

    { "min_lt_abab", insn_minuid,  UD,  "value <  imm ? value : imm", 0 },
    { "max_ge_abab", insn_maxuid,  UD,  "value >= imm ? value : imm", 0 },
    { "min_le_abab", insn_minuid,  UD,  "value <= imm ? value : imm", 0 },
    { "max_gt_abab", insn_maxuid,  UD,  "value >  imm ? value : imm", 0 },
    { "max_lt_baab", insn_maxuid,  UD,  "imm <  value ? value : imm", 0 },
    { "min_ge_baab", insn_minuid,  UD,  "imm >= value ? value : imm", 0 },
    { "max_le_baab", insn_maxuid,  UD,  "imm <= value ? value : imm", 0 },
    { "min_gt_baab", insn_minuid,  UD,  "imm >  value ? value : imm", 0 },
    { "max_lt_abba", insn_maxuid,  UD,  "value <  imm ? imm : value", 0 },
    { "min_ge_abba", insn_minuid,  UD,  "value >= imm ? imm : value", 0 },
    { "max_le_abba", insn_maxuid,  UD,  "value <= imm ? imm : value", 0 },
    { "min_gt_abba", insn_minuid,  UD,  "value >  imm ? imm : value", 0 },
    { "min_lt_baba", insn_minuid,  UD,  "imm <  value ? imm : value", 0 },
    { "max_ge_baba", insn_maxuid,  UD,  "imm >= value ? imm : value", 0 },
    { "min_le_baba", insn_minuid,  UD,  "imm <= value ? imm : value", 0 },
    { "max_gt_baba", insn_maxuid,  UD,  "imm >  value ? imm : value", 0 },
};

static const func_info func_info_i128[] = {
    { "and_imm",     insn_andi,     IQ,  "value & imm",     0 },
    { "or_imm",      insn_ori,      IQ,  "value | imm",     0 },
    { "xor_imm",     insn_xori,     IQ,  "value ^ imm",     0 },
    { "andn_imm",    insn_andni,    IQ,  "(~value) & imm",  0 },
    { "orn_imm",     insn_orni,     IQ,  "(~value) | imm",  0 },
    { "add_imm",     insn_addiq,    IQ,  "value + imm",     0 },
    { "cmp_eq",      insn_cmpeqiq,  IQ,  "value == imm",    0 },
    { "cmp_ne",      insn_cmpneiq,  IQ,  "value != imm",    0 },
    { "cmp_lt",      insn_cmpltsiq, IQ,  "value < imm",     0 },
    { "cmp_ge",      insn_cmpgesiq, IQ,  "value >= imm",    0 },
    { "cmp_le",      insn_cmpltsiq, IQ,  "value <= imm",    1 },
    { "cmp_gt",      insn_cmpgesiq, IQ,  "value > imm",     1 },
};

static const func_info func_info_u128[] = {
    { "cmp_lt",      insn_cmpltuiq, UQ,  "value < imm",  0 },
    { "cmp_ge",      insn_cmpgeuiq, UQ,  "value >= imm", 0 },
    { "cmp_le",      insn_cmpltuiq, UQ,  "value <= imm", 1 },
    { "cmp_gt",      insn_cmpgeuiq, UQ,  "value > imm",  1 },
};

CODEGEN_HANDLER(BinaryRegImmI32)  { BinaryRegImm<i32, u32>(out, func_info_i32); }
CODEGEN_HANDLER(BinaryRegImmU32)  { BinaryRegImm<u32, u32>(out, func_info_u32); }
CODEGEN_HANDLER(BinaryRegImmI64)  { BinaryRegImm<i64, u64>(out, func_info_i64); }
CODEGEN_HANDLER(BinaryRegImmU64)  { BinaryRegImm<u64, u64>(out, func_info_u64); }
CODEGEN_HANDLER(BinaryRegImmI128) { BinaryRegImm<i64, u64>(out, func_info_i128); }
CODEGEN_HANDLER(BinaryRegImmU128) { BinaryRegImm<u64, u64>(out, func_info_u128); }

} // namespace llvm
} // namespace postrisc
