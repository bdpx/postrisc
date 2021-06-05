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
    // can't emit - min_imm_u64.l %r1, %r1, 9223372036854775807
    if (frag.insn == insn_min_imm_u64 || frag.insn == insn_max_imm_u64) {
        if (imm.value == 0x7FFFFFFFFFFFFFFF && (
            name == "min_ge_baab_u64_9223372036854775807" ||
            name == "min_le_abab_u64_9223372036854775807" ||
            name == "max_le_abba_u64_9223372036854775807" ||
            name == "max_ge_baba_u64_9223372036854775807"
         )) {
            out << CHECK << insn_name(insn_ld_imm) << ".l %r2, " << 0x7FFFFFFFFFFFFFFF << std::endl;
            out << CHECK_EMPTY << std::endl;
            out << CHECK_NEXT << "! BUNDLE" << std::endl;
            if (frag.insn == insn_min_imm_u64)
                out << CHECK_NEXT << "cmov_lt_i64 %r1, %r1, %r2, %r1" << std::endl;
            else
                out << CHECK_NEXT << "cmov_lt_i64 %r1, %r1, %r1, %r2" << std::endl;
            return;
        }
    }

    if (frag.insn == insn_add_imm_i64) {
        // check possible replacement: ADD_IMM_I64 ===> LDA_N
        if constexpr (std::is_same<i64, ITYPE>::value) {
            const int skip = 64 - bits_per_mid_binimm - bits_per_lda_near_shift;
            if ((imm.value & util::makemask<u64>(bits_per_lda_near_shift)) == 0 && ((imm.value << skip) >> skip) == imm.value) {
                out << CHECK << insn_name(insn_lda_n) << " %r1, %r1, " << (imm.value >> bits_per_lda_near_shift) << std::endl;
                return;
            }
        }
    }

    if (frag.insn == insn_and_imm) {
        // AND_IMM with one-zero mask is replaced by BIT_CLEAR
        if (population == sizeof(ITYPE)*8-1) {
            const unsigned bitpos = util::count_trailing_zeros::func(~imm.value);
            out << CHECK << insn_name(insn_bit_clear) << " %r1, %r1, " << bitpos << std::endl;
            return;
        }

        // check possible replacement: AND_IMM ===> DEPOSIT
        if (name == "and_imm_i64_p4611686018427387903") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %gz, %r1, 0, 62" << std::endl;
            return;
        }
        if (name == "and_imm_i64_m4611686018427387904") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 0, 62" << std::endl;
            return;
        }
        if (name == "and_imm_i32_m134217728") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 0, 27" << std::endl;
            return;
        }
        if (name == "and_imm_i64_p1048575") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %gz, %r1, 0, 20" << std::endl;
            return;
        }
        if (name == "and_imm_i32_m1048576" || name == "and_imm_i64_m1048576") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 0, 20" << std::endl;
            return;
        }
        if (name == "and_imm_i32_p1048575") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %gz, %r1, 0, 20" << std::endl;
            return;
        }
        if (name == "and_imm_i32_p134217727") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %gz, %r1, 0, 27" << std::endl;
            return;
        }
        if (name == "and_imm_i64_m4611686018427387901") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 2, 60" << std::endl;
            return;
        }
        if (name == "and_imm_i64_p4611686018427387903") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 62, 2" << std::endl;
            return;
        }
        if (name == "and_imm_i64_m1048573" || name == "and_imm_i32_m1048573") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 2, 18" << std::endl;
           return;
        }
        if (name == "and_imm_i64_p1048575") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 20, 44" << std::endl;
            return;
        }

        if (name == "and_imm_i32_m2147483645" || name == "and_imm_i64_m2147483645") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 2, 29" << std::endl;
            return;
        }

        if (name == "and_imm_i32_p1048575") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 20, 12" << std::endl;
            return;
        }

        if (name == "and_imm_i32_p134217727") {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, 27, 5" << std::endl;
            return;
        }

/*
        int fit_size = 0;
        int fit_pos = 0;
        if (CheckBlock<ITYPE, zeroes_block>(imm.value, fit_size, fit_pos)) {
            out << CHECK << insn_name(insn_deposit) << " %r1, %r1, %gz, " << fit_pos << ", " << fit_size << std::endl;
            return;
        }
        if (CheckBlock<ITYPE, ones_block>(imm.value, fit_size, fit_pos)) {
            out << CHECK << insn_name(insn_deposit) << " %r1, %gz, %r1, " << fit_pos << ", " << fit_size << std::endl;
            return;
        }
*/
    }

    if (std::is_signed_v<ITYPE> && imm.value < 0 && (
        frag.insn == insn_rem_imm_i32 ||
        frag.insn == insn_rem_imm_i64
      )) {
        // FIXME: .l prefix changed
        //out << CHECK << insn_name(frag.insn) << (imm.mode == LONG ? ".l" : "")
        //    << " %r1, %r1, " << (-imm.value) << std::endl;
        return;
    }

    // div_imm_uXX by 2^N replaced by srl_i64[32]
    // rem_imm_uXX by 2^N replaced by AND with (2^N-1)
    if (population == 1 && (
        frag.insn == insn_div_imm_u32 ||
        frag.insn == insn_div_imm_u64 ||
        frag.insn == insn_rem_imm_u32 ||
        frag.insn == insn_rem_imm_u64
        )) {

        const unsigned bitpos = util::count_trailing_zeros::func(imm.value);
        switch (frag.insn) {
            case insn_div_imm_u32:
                out << CHECK << insn_name(insn_srl_imm_u32) << " %r1, %r1, " << bitpos << std::endl;
                break;
            case insn_div_imm_u64:
                out << CHECK << insn_name(insn_srl_imm_u64) << " %r1, %r1, " << bitpos << std::endl;
                break;
            case insn_rem_imm_u32:
            case insn_rem_imm_u64:
                // FIXME: .l prefix changed
                // out << CHECK << insn_name(insn_and_imm) << " %r1, %r1, " << (imm.value-1) << std::endl;
                break;
            default:
                break;
        }
        return;
    }

    // mul_imm by -2^N replaced by sl_sub_i64[32]: 0 - (X << N)
    if ((frag.insn == insn_mul_imm_i32 || frag.insn == insn_mul_imm_i64) &&
         util::population::func(static_cast<UTYPE>(-imm.value)) == 1) {

        const unsigned bitpos = util::count_trailing_zeros::func(-imm.value);
        // sl_sub_i64 %r1, %gz, %r1, 20
        switch (frag.insn) {
            case insn_mul_imm_i32:
                out << CHECK << insn_name(insn_sl_sub_i32) << " %r1, %gz, %r1, " << bitpos << std::endl;
                break;
            case insn_mul_imm_i64:
                out << CHECK << insn_name(insn_sl_sub_i64) << " %r1, %gz, %r1, " << bitpos << std::endl;
                break;
            default:
                break;
        }
        return;
    }

    // CHECK: op_imm.l %r1, %r1, imm
    out << CHECK << InsnCounter(this, frag.insn) << (imm.mode == LONG ? ".l" : "")
        << " %r1, %r1, " << imm.value << std::endl;
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

            out << CHECK_LABEL << name.str() << std::endl;
            BinaryRegImmCase<ITYPE, UTYPE>(out, frag, imm, name.str());

            out << frag.type << " " << name.str()  << "(" << frag.type << " value) {"<< std::endl;
            const ITYPE cpp_imm = imm.value - frag.offset;
            out << "    const " << frag.type << " imm = " << cpp_imm << ";" << std::endl;
            out << "    return " << frag.c_fragment << ";"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }

        out << std::endl;
    }
}

// for all i32,i64,i128 same bitwise instructions are used
// cm-_eq|ne is signed for both i32|u32

#define INT_BINARY_FUNCS(TYPE) \
    { "and_imm",   insn_and_imm,             #TYPE,  "value & imm",     0 }, \
    { "or_imm",    insn_or_imm,              #TYPE,  "value | imm",     0 }, \
    { "xor_imm",   insn_xor_imm,             #TYPE,  "value ^ imm",     0 }, \
    { "andn_imm",  insn_andn_imm,            #TYPE,  "(~value) & imm",  0 }, \
    { "orn_imm",   insn_orn_imm,             #TYPE,  "(~value) | imm",  0 }, \
    { "add_imm",   insn_add_imm_    ## TYPE, #TYPE,  "value + imm",     0 }, \
    { "cmp_eq",    insn_cmp_eq_imm_ ## TYPE, #TYPE,  "value == imm",    0 }, \
    { "cmp_ne",    insn_cmp_ne_imm_ ## TYPE, #TYPE,  "value != imm",    0 }, \
    { "cmp_lt",    insn_cmp_lt_imm_ ## TYPE, #TYPE,  "value < imm",     0 }, \
    { "cmp_ge",    insn_cmp_ge_imm_ ## TYPE, #TYPE,  "value >= imm",    0 }, \
    { "cmp_le",    insn_cmp_lt_imm_ ## TYPE, #TYPE,  "value <= imm",    1 }, \
    { "cmp_gt",    insn_cmp_ge_imm_ ## TYPE, #TYPE,  "value > imm",     1 }, \

#define UINT_BINARY_FUNCS(TYPE) \
    { "cmp_lt",      insn_cmp_lt_imm_ ## TYPE,  #TYPE,  "value < imm",  0 }, \
    { "cmp_ge",      insn_cmp_ge_imm_ ## TYPE,  #TYPE,  "value >= imm", 0 }, \
    { "cmp_le",      insn_cmp_lt_imm_ ## TYPE,  #TYPE,  "value <= imm", 1 }, \
    { "cmp_gt",      insn_cmp_ge_imm_ ## TYPE,  #TYPE,  "value > imm",  1 }, \

#define MINMAX_IMM_FUNCS(TYPE) \
    { "min_lt_abab", insn_min_imm_  ## TYPE,  #TYPE, "value <  imm ? value : imm", 0 }, \
    { "max_ge_abab", insn_max_imm_  ## TYPE,  #TYPE, "value >= imm ? value : imm", 0 }, \
    { "min_le_abab", insn_min_imm_  ## TYPE,  #TYPE, "value <= imm ? value : imm", 0 }, \
    { "max_gt_abab", insn_max_imm_  ## TYPE,  #TYPE, "value >  imm ? value : imm", 0 }, \
    { "max_lt_baab", insn_max_imm_  ## TYPE,  #TYPE, "imm <  value ? value : imm", 0 }, \
    { "min_ge_baab", insn_min_imm_  ## TYPE,  #TYPE, "imm >= value ? value : imm", 0 }, \
    { "max_le_baab", insn_max_imm_  ## TYPE,  #TYPE, "imm <= value ? value : imm", 0 }, \
    { "min_gt_baab", insn_min_imm_  ## TYPE,  #TYPE, "imm >  value ? value : imm", 0 }, \
    { "max_lt_abba", insn_max_imm_  ## TYPE,  #TYPE, "value <  imm ? imm : value", 0 }, \
    { "min_ge_abba", insn_min_imm_  ## TYPE,  #TYPE, "value >= imm ? imm : value", 0 }, \
    { "max_le_abba", insn_max_imm_  ## TYPE,  #TYPE, "value <= imm ? imm : value", 0 }, \
    { "min_gt_abba", insn_min_imm_  ## TYPE,  #TYPE, "value >  imm ? imm : value", 0 }, \
    { "min_lt_baba", insn_min_imm_  ## TYPE,  #TYPE, "imm <  value ? imm : value", 0 }, \
    { "max_ge_baba", insn_max_imm_  ## TYPE,  #TYPE, "imm >= value ? imm : value", 0 }, \
    { "min_le_baba", insn_min_imm_  ## TYPE,  #TYPE, "imm <= value ? imm : value", 0 }, \
    { "max_gt_baba", insn_max_imm_  ## TYPE,  #TYPE, "imm >  value ? imm : value", 0 }, \

#define INT_BINARY_FUNCS_NO128(TYPE) \
    { "subr_imm",    insn_subr_imm_ ## TYPE,  #TYPE, "imm - value", 0 }, \
    { "mul_imm",     insn_mul_imm_  ## TYPE,  #TYPE, "value * imm", 0 }, \
    { "div_imm",     insn_div_imm_  ## TYPE,  #TYPE, "value / imm", 0 }, \
    { "rem_imm",     insn_rem_imm_  ## TYPE,  #TYPE, "value % imm", 0 }, \
    MINMAX_IMM_FUNCS(TYPE)

#define UINT_BINARY_FUNCS_NO128(TYPE) \
    { "div_imm",     insn_div_imm_  ## TYPE,  #TYPE, "value / imm", 0 }, \
    { "rem_imm",     insn_rem_imm_  ## TYPE,  #TYPE, "value % imm", 0 }, \
    MINMAX_IMM_FUNCS(TYPE)

static const func_info func_info_i32[]  = { INT_BINARY_FUNCS(i32) };
static const func_info func_info_u32[]  = { UINT_BINARY_FUNCS(u32) };
static const func_info func_info_i32_spec[]  = { INT_BINARY_FUNCS_NO128(i32) };
static const func_info func_info_u32_spec[]  = { UINT_BINARY_FUNCS_NO128(u32) };

static const func_info func_info_i64[]  = { INT_BINARY_FUNCS(i64) };
static const func_info func_info_u64[]  = { UINT_BINARY_FUNCS(u64) };
static const func_info func_info_i64_spec[]  = { INT_BINARY_FUNCS_NO128(i64) };
static const func_info func_info_u64_spec[]  = { UINT_BINARY_FUNCS_NO128(u64) };

static const func_info func_info_i128[] = { INT_BINARY_FUNCS(i128) };
static const func_info func_info_u128[] = { UINT_BINARY_FUNCS(u128) };

CODEGEN_HANDLER(BinaryRegImmI32) { BinaryRegImm<i32, u32>(out, func_info_i32); }
CODEGEN_HANDLER(BinaryRegImmU32) { BinaryRegImm<u32, u32>(out, func_info_u32); }
CODEGEN_HANDLER(BinaryRegImmI64) { BinaryRegImm<i64, u64>(out, func_info_i64); }
CODEGEN_HANDLER(BinaryRegImmU64) { BinaryRegImm<u64, u64>(out, func_info_u64); }

CODEGEN_HANDLER(BinaryRegImmI32Specifiec) { BinaryRegImm<i32, u32>(out, func_info_i32_spec); }
CODEGEN_HANDLER(BinaryRegImmU32Specifiec) { BinaryRegImm<u32, u32>(out, func_info_u32_spec); }
CODEGEN_HANDLER(BinaryRegImmI64Specifiec) { BinaryRegImm<i64, u64>(out, func_info_i64_spec); }
CODEGEN_HANDLER(BinaryRegImmU64Specifiec) { BinaryRegImm<u64, u64>(out, func_info_u64_spec); }

CODEGEN_HANDLER(BinaryRegImmI128) { BinaryRegImm<i64, u64>(out, func_info_i128); }
CODEGEN_HANDLER(BinaryRegImmU128) { BinaryRegImm<u64, u64>(out, func_info_u128); }

} // namespace llvm
} // namespace postrisc
