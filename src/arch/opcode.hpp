
#include "instruction_fields.hpp"

namespace postrisc {

inline constexpr u64 makepart(u64 item, unsigned int base)
{
    return item << base;
}

#define OPCODE_X(ID, VALUE)      opcode_##ID = VALUE,
#define PSEUDO_INSN(ID, ORIG_ID) opcode_##ID = opcode_##ORIG_ID,

/***************************************************************************
* instruction-subsets generators
***************************************************************************/
#define OPCODE_TABLE_ATOMIC_LD(X, TYPE, NAME, N) \
    X(amx_ ## NAME ## _ ## TYPE, N + 0) \
    X(amq_ ## NAME ## _ ## TYPE, N + 1)

#define OPCODE_TABLE_ATOMIC_ST(X, TYPE, NAME, N) \
    X(amx_ ## NAME ## _ ## TYPE, N + 0) \
    X(amr_ ## NAME ## _ ## TYPE, N + 1)

#define OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, NAME, N) \
    X(amx_ ## NAME ## _ ## TYPE, N + 0) \
    X(amq_ ## NAME ## _ ## TYPE, N + 1) \
    X(amr_ ## NAME ## _ ## TYPE, N + 2) \
    X(amz_ ## NAME ## _ ## TYPE, N + 3)

#define OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, NAME, N) \
    X(amx_ ## NAME ## _ ## TYPE, N + 0) \
    X(amr_ ## NAME ## _ ## TYPE, N + 1)

#define OPCODE_TABLE_ATOMIC_FULL_OPS(X, ITYPE, UTYPE, N) \
    OPCODE_TABLE_ATOMIC_LD(X,    ITYPE, ld,     N + 0) \
    OPCODE_TABLE_ATOMIC_ST(X,    ITYPE, st,     N + 2) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, swap,   N + 4) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_add, N + 12) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_and, N + 16) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_or,  N + 20) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_xor, N + 24) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, ITYPE, ld_min, N + 28) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, ITYPE, ld_max, N + 32) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_min, N + 36) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_max, N + 40) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_add, N + 44) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_and, N + 46) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_or,  N + 48) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_xor, N + 50) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, ITYPE, st_min, N + 52) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, ITYPE, st_max, N + 54) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_min, N + 56) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_max, N + 58) \

/***************************************************************************
*
* main opcode - 7 bit
*
***************************************************************************/

#define DECLARE_PRIMARY_OPCODES(X) \
    X(misc,            0) \
    X(fused,           1) \
    X(raopx,           2) \
    X(br_misc,         3) \
    X(loop,            4) \
    X(br_32,           5) \
    X(br_64,           6) \
    X(br_128,          7) \
    X(mem_xi64,        8) \
    X(mem_xi32,        9) \
    X(mem_xu32,        10) \
    X(fma,             11) /* reserved: 12-15 */ \
    X(ld_imm,          16) \
    X(ld_imm_high,     17) \
    X(andn_imm,        18) \
    X(orn_imm,         19) \
    X(and_imm,         20) \
    X(or_imm,          21) \
    X(xor_imm,         22) \
    X(dep,             23) /* reserved: 39 */ \
    X(add_imm_i64,     24) \
    X(add_imm_i128,    25) \
    X(add_imm_i32,     26) \
    X(add_imm_u32,     27) \
    X(subr_imm_i64,    28) /* reserved: 29 */ \
    X(subr_imm_i32,    30) \
    X(subr_imm_u32,    31) \
    X(mul_imm_i64,     32) /* reserved: 33 */ \
    X(mul_imm_i32,     34) \
    X(mul_imm_u32,     35) \
    X(div_imm_i64,     36) \
    X(div_imm_u64,     37) \
    X(rem_imm_i64,     38) \
    X(rem_imm_u64,     39) \
    X(div_imm_i32,     40) \
    X(div_imm_u32,     41) \
    X(rem_imm_i32,     42) \
    X(rem_imm_u32,     43) \
    X(max_imm_i64,     44) \
    X(max_imm_u64,     45) \
    X(min_imm_i64,     46) \
    X(min_imm_u64,     47) \
    X(max_imm_i32,     48) \
    X(max_imm_u32,     49) \
    X(min_imm_i32,     50) \
    X(min_imm_u32,     51) \
    X(cmp_eq_imm_i64,  52) \
    X(cmp_ne_imm_i64,  53) \
    X(cmp_lt_imm_i64,  54) \
    X(cmp_lt_imm_u64,  55) \
    X(cmp_ge_imm_i64,  56) \
    X(cmp_ge_imm_u64,  57) \
    X(cmp_eq_imm_i32,  58) \
    X(cmp_ne_imm_i32,  59) \
    X(cmp_lt_imm_i32,  60) \
    X(cmp_lt_imm_u32,  61) \
    X(cmp_ge_imm_i32,  62) \
    X(cmp_ge_imm_u32,  63) \
    X(cmp_eq_imm_i128, 64) \
    X(cmp_ne_imm_i128, 65) \
    X(cmp_lt_imm_i128, 66) \
    X(cmp_lt_imm_u128, 67) \
    X(cmp_ge_imm_i128, 68) \
    X(cmp_ge_imm_u128, 69) /* reserved: 70, 71 */ \
    X(ld_u8,           72) \
    X(ld_i8,           73) \
    X(st_i8,           74) \
    X(ld_u16,          75) \
    X(ld_i16,          76) \
    X(st_i16,          77) \
    X(ld_u32,          78) \
    X(ld_i32,          79) \
    X(st_i32,          80) \
    X(ld_u64,          81) \
    X(ld_i64,          82) \
    X(st_i64,          83) \
    X(ld_i128,         84) \
    X(st_i128,         85) \
    X(lda_n,           86) \
    X(lda_nrc,         87) \
    X(ld_iprel_u8,     88) \
    X(ld_iprel_i8,     89) \
    X(st_iprel_i8,     90) \
    X(ld_iprel_u16,    91) \
    X(ld_iprel_i16,    92) \
    X(st_iprel_i16,    93) \
    X(ld_iprel_u32,    94) \
    X(ld_iprel_i32,    95) \
    X(st_iprel_i32,    96) \
    X(ld_iprel_u64,    97) \
    X(ld_iprel_i64,    98) \
    X(st_iprel_i64,    99) \
    X(ld_iprel_i128,   100) \
    X(st_iprel_i128,   101) \
    X(lda_iprel,       102) \
    X(call_plt,        103) \
    X(call,            104) \
    X(lda_r,           105) \
    X(br_eq_imm_i64,   106) \
    X(br_ne_imm_i64,   107) \
    X(br_lt_imm_i64,   108) \
    X(br_lt_imm_u64,   109) \
    X(br_ge_imm_i64,   110) \
    X(br_ge_imm_u64,   111) \
    X(br_eq_imm_i32,   112) \
    X(br_ne_imm_i32,   113) \
    X(br_lt_imm_i32,   114) \
    X(br_lt_imm_u32,   115) \
    X(br_ge_imm_i32,   116) \
    X(br_ge_imm_u32,   117) \
    X(br_eq_imm_i128,  118) \
    X(br_ne_imm_i128,  119) \
    X(br_lt_imm_i128,  120) \
    X(br_lt_imm_u128,  121) \
    X(br_ge_imm_i128,  122) \
    X(br_ge_imm_u128,  123) \
    X(br_mask_all,     124) \
    X(br_mask_notall,  125) \
    X(br_mask_none,    126) \
    X(br_mask_any,     127) \

enum EInstructionOpcode {
    DECLARE_PRIMARY_OPCODES(OPCODE_X)
    opcode_last,
    PSEUDO_INSN(ld_imm_f64,      ld_imm)
    PSEUDO_INSN(ld_imm_f32,      ld_imm)
    PSEUDO_INSN(ld_iprel_f32,    ld_iprel_u32)
    PSEUDO_INSN(ld_iprel_f64,    ld_iprel_u64)
    PSEUDO_INSN(ld_iprel_f128,   ld_iprel_i128)
};

inline constexpr u64 insn_tmplt(const EInstructionOpcode opc)
{
    return makepart(static_cast<u64>(opc), disp_opcode);
}

/**************************************************************************************
* opcode_fused extended opcodes (7 bit)
**************************************************************************************/
#define DECLARE_FUSED_EXTENDED_OPCODES(X) \
    X(nul_misc,        0) \
    X(nul_32,          1) \
    X(nul_64,          2) \
    X(nul_128,         3) /* reserved 4-7 */ \
    X(add_add_i64,     8) \
    X(add_sub_i64,     9) \
    X(sub_sub_i64,     10) \
    X(add_addc_u64,    11) \
    X(sub_subb_u64,    12) \
    X(slp_i32,         13) \
    X(srp_i32,         14) \
    X(srp_imm_i32,     15) \
    X(slp_i64,         16) \
    X(srp_i64,         17) \
    X(srp_imm_i64,     18) \
    X(slsrl_u64,       19) \
    X(slsra_i64,       20) \
    X(slsrl_imm_u64,   21) \
    X(slsra_imm_i64,   22) \
    X(sl_add_i64,      23) \
    X(sl_sub_i64,      24) \
    X(sl_subr_i64,     25) \
    X(slsrl_u32,       26) \
    X(slsra_i32,       27) \
    X(mbsel,           28) \
    X(perm,            29) \
    X(mov2,            30) \
    X(alignup_u64,     31) \
    X(crc32c,          32) /* reserved: 33-36 */ \
    X(sl_add_i32,      37) \
    X(sl_add_u32,      38) \
    X(sl_subr_i32,     39) \
    X(sl_subr_u32,     40) \
    X(sl_sub_i32,      41) \
    X(sl_sub_u32,      42) \
    X(dep_r,           43) \
    X(slp_i128,        44) \
    X(srp_i128,        45) \
    X(srp_imm_i128,    46) \
    X(sl_or,           50) \
    X(sl_xor,          51) /* reserved: 52-59 */ \
    X(cmov_lsb,        60) \
    X(cmov_eq_i32,     61) \
    X(cmov_lt_i32,     62) \
    X(cmov_le_i32,     63) \
    X(cmov_eq_i64,     65) \
    X(cmov_lt_i64,     66) \
    X(cmov_le_i64,     67) \
    X(nul_eq_imm_i128, 106) \
    X(nul_ne_imm_i128, 107) \
    X(nul_lt_imm_i128, 108) \
    X(nul_lt_imm_u128, 109) \
    X(nul_ge_imm_i128, 110) \
    X(nul_ge_imm_u128, 111) \
    X(nul_eq_imm_i32,  112) \
    X(nul_ne_imm_i32,  113) \
    X(nul_lt_imm_i32,  114) \
    X(nul_lt_imm_u32,  115) \
    X(nul_ge_imm_i32,  116) \
    X(nul_ge_imm_u32,  117) \
    X(nul_eq_imm_i64,  118) \
    X(nul_ne_imm_i64,  119) \
    X(nul_lt_imm_i64,  120) \
    X(nul_lt_imm_u64,  121) \
    X(nul_ge_imm_i64,  122) \
    X(nul_ge_imm_u64,  123) \
    X(nul_mask_all,    124) \
    X(nul_mask_notall, 125) \
    X(nul_mask_none,   126) \
    X(nul_mask_any,    127) \

enum fused_opx {
    DECLARE_FUSED_EXTENDED_OPCODES(OPCODE_X)
    fused_opx_last
};

inline constexpr u64 insn_tmplt(const fused_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_fused)
         | makepart(static_cast<u64>(opx), disp_fused_opx);
}

/**************************************************************************************
* opcode_fma extended opcodes (7 bit)
**************************************************************************************/
#define DECLARE_FMA_EXTENDED_OPCODES(X) \
    X(mul_add,         8) \
    X(mul_sub,         9) \
    X(mul_subr,        10) \
    X(merge_f128,      12) \
    X(madd_f128,       13) \
    X(msub_f128,       14) \
    X(nmadd_f128,      15) \
    X(nmsub_f128,      16) /* reserved: 17-35 */ \
    X(merge_f64,       36) \
    X(madd_f64,        37) \
    X(msub_f64,        38) \
    X(nmadd_f64,       39) \
    X(nmsub_f64,       40) \
    X(merge_vf64,      41) \
    X(madd_vf64,       42) \
    X(msub_vf64,       43) \
    X(nmadd_vf64,      44) \
    X(nmsub_vf64,      45) \
    X(madd_alt_vf64,   46) \
    X(msub_alt_vf64,   47) /* reserved: 48-59 */ \
    X(merge_f32,       60) \
    X(madd_f32,        61) \
    X(msub_f32,        62) \
    X(nmadd_f32,       63) \
    X(nmsub_f32,       64) \
    X(merge_vf32,      65) \
    X(madd_vf32,       66) \
    X(msub_vf32,       67) \
    X(nmadd_vf32,      68) \
    X(nmsub_vf32,      69) \
    X(madd_alt_vf32,   70) \
    X(msub_alt_vf32,   71) /* reserved: 72-83 */ \
    X(merge_f16,       84) \
    X(madd_f16,        85) \
    X(msub_f16,        86) \
    X(nmadd_f16,       87) \
    X(nmsub_f16,       88) \
    X(merge_vf16,      89) \
    X(madd_vf16,       90) \
    X(msub_vf16,       91) \
    X(nmadd_vf16,      92) \
    X(nmsub_vf16,      93) \
    X(madd_alt_vf16,   94) \
    X(msub_alt_vf16,   95) /* reserved: 96-107 */ \
    OPCODE_TABLE_ATOMIC_LD_OP(X, i8,   cas,    108) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, i16,  cas,    112) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, i32,  cas,    116) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, i64,  cas,    120) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, i128, cas,    124) \

enum fma_opx {
    DECLARE_FMA_EXTENDED_OPCODES(OPCODE_X)
    fma_opx_last
};

inline constexpr u64 insn_tmplt(const fma_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_fma)
         | makepart(static_cast<u64>(opx), disp_fused_opx);
}

/**************************************************************************************
* opcode_[br|nul]_misc extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_BR_OR_NUL_MISC_EXTENDED_OPCODES(X, PREFIX) \
    X(PREFIX ##  _bc,     0) \
    X(PREFIX ##  _bc_imm, 1) \
    X(PREFIX ##  _bs,     2) \
    X(PREFIX ##  _bs_imm, 3) \

#define DECLARE_BR_MISC_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_MISC_EXTENDED_OPCODES(X, br)
enum br_misc_opx {
    DECLARE_BR_MISC_EXTENDED_OPCODES(OPCODE_X)
    br_misc_opx_last,
};

inline constexpr u64 insn_tmplt(const br_misc_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_br_misc)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

#define DECLARE_NUL_MISC_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_MISC_EXTENDED_OPCODES(X, nul)
enum nul_misc_opx {
    DECLARE_NUL_MISC_EXTENDED_OPCODES(OPCODE_X)
    nul_misc_opx_last
};

inline constexpr u64 insn_tmplt(const nul_misc_opx opx)
{
    return insn_tmplt(fused_opx::opcode_nul_misc)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_[br|nul]_[32|64|128] extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, PREFIX, SIZE) \
    X(PREFIX ## _eq_i  ## SIZE, 0) \
    X(PREFIX ## _ne_i  ## SIZE, 1) \
    X(PREFIX ## _lt_i  ## SIZE, 2) \
    X(PREFIX ## _ge_i  ## SIZE, 3) \
    X(PREFIX ## _lt_u  ## SIZE, 4) \
    X(PREFIX ## _ge_u  ## SIZE, 5) \
    X(PREFIX ## _oeq_f ## SIZE, 6) \
    X(PREFIX ## _one_f ## SIZE, 7) \
    X(PREFIX ## _olt_f ## SIZE, 8) \
    X(PREFIX ## _oge_f ## SIZE, 9) \
    X(PREFIX ## _ueq_f ## SIZE, 10) \
    X(PREFIX ## _une_f ## SIZE, 11) \
    X(PREFIX ## _ult_f ## SIZE, 12) \
    X(PREFIX ## _uge_f ## SIZE, 13) \
    X(PREFIX ## _o_f   ## SIZE, 14) \
    X(PREFIX ## _u_f   ## SIZE, 15) \

#define DECLARE_BR_32_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, br, 32)
enum br_32_opx {
    DECLARE_BR_32_EXTENDED_OPCODES(OPCODE_X)
    br_32_opx_last,
};

inline constexpr u64 insn_tmplt(const br_32_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_br_32)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

#define DECLARE_NUL_32_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, nul, 32)
enum nul_32_opx {
    DECLARE_NUL_32_EXTENDED_OPCODES(OPCODE_X)
    nul_32_opx_last,
};

inline constexpr u64 insn_tmplt(const nul_32_opx opx)
{
    return insn_tmplt(fused_opx::opcode_nul_32)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

#define DECLARE_BR_64_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, br, 64)
enum br_64_opx {
    DECLARE_BR_64_EXTENDED_OPCODES(OPCODE_X)
    br_64_opx_last,
};

inline constexpr u64 insn_tmplt(const br_64_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_br_64)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

#define DECLARE_NUL_64_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, nul, 64)
enum nul_64_opx {
    DECLARE_NUL_64_EXTENDED_OPCODES(OPCODE_X)
    nul_64_opx_last,
};

inline constexpr u64 insn_tmplt(const nul_64_opx opx)
{
    return insn_tmplt(fused_opx::opcode_nul_64)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

#define DECLARE_BR_128_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, br, 128)
enum br_128_opx {
    DECLARE_BR_128_EXTENDED_OPCODES(OPCODE_X)
    br_128_opx_last,
};

inline constexpr u64 insn_tmplt(const br_128_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_br_128)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

#define DECLARE_NUL_128_EXTENDED_OPCODES(X) DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, nul, 128)
enum nul_128_opx {
    DECLARE_NUL_128_EXTENDED_OPCODES(OPCODE_X)
    nul_128_opx_last,
};

inline constexpr u64 insn_tmplt(const nul_128_opx opx)
{
    return insn_tmplt(fused_opx::opcode_nul_128)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_loop extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_LOOP_EXTENDED_OPCODES(X) \
    X(rep_lt_i64,   0) \
    X(rep_gt_i64,   1) \
    X(rep_le_i64,   2) \
    X(rep_ge_i64,   3) \
    X(rep_lt_u64,   4) \
    X(rep_gt_u64,   5) \
    X(rep_le_u64,   6) \
    X(rep_ge_u64,   7) \
    X(rep_lt_i32,   8) \
    X(rep_gt_i32,   9) \
    X(rep_le_i32,   10) \
    X(rep_ge_i32,   11) \
    X(rep_lt_u32,   12) \
    X(rep_gt_u32,   13) \
    X(rep_le_u32,   14) \
    X(rep_ge_u32,   15) \

enum ELoopOpx {
    DECLARE_LOOP_EXTENDED_OPCODES(OPCODE_X)
    loop_opx_last,
};

inline constexpr u64 insn_tmplt(const ELoopOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_loop)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_raopx extended opcodes (7 bit)
**************************************************************************************/
#define DECLARE_RAOPX_EXTENDED_OPCODES(X) \
    X(nop,       0) \
    X(jmp,       1) \
    X(retf,      2) \
    X(alloc,     3) \
    X(alloc_sp,  4) \
    X(eh_adj,    8) \
    X(eh_throw,  9) \
    X(eh_catch, 10) \
    X(eh_next,  11) \
    X(dcbt,     16) \
    X(dcbf,     17) \
    X(dcbi,     18) \
    X(icbi,     19) \
    X(write,    127) \

enum raopx_opx {
    DECLARE_RAOPX_EXTENDED_OPCODES(OPCODE_X)
    raopx_opx_last
};

inline constexpr u64 insn_tmplt(const raopx_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_raopx)
         | makepart(static_cast<u64>(opx), disp_reg_a);
}

/**************************************************************************************
* opcode_mem_xi32|xu32|xi64 extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, SUFFIX) \
    X(ld_  ## SUFFIX ## _u8,   0) \
    X(ld_  ## SUFFIX ## _i8,   1) \
    X(st_  ## SUFFIX ## _i8,   2) \
    X(ld_  ## SUFFIX ## _u16,  3) \
    X(ld_  ## SUFFIX ## _i16,  4) \
    X(st_  ## SUFFIX ## _i16,  5) \
    X(ld_  ## SUFFIX ## _u32,  6) \
    X(ld_  ## SUFFIX ## _i32,  7) \
    X(st_  ## SUFFIX ## _i32,  8) \
    X(ld_  ## SUFFIX ## _u64,  9) \
    X(ld_  ## SUFFIX ## _i64,  10) \
    X(st_  ## SUFFIX ## _i64,  11) \
    X(ld_  ## SUFFIX ## _i128, 12) \
    X(st_  ## SUFFIX ## _i128, 13) \
    X(lda_ ## SUFFIX,          14) \

#define DECLARE_MEM_XI64_EXTENDED_OPCODES(X) DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, xi64)
enum mem_xi64_opx {
    DECLARE_MEM_XI64_EXTENDED_OPCODES(OPCODE_X)
    mem_xi64_opx_last,
};

inline constexpr u64 insn_tmplt(const mem_xi64_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_mem_xi64)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

#define DECLARE_MEM_XI32_EXTENDED_OPCODES(X) DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, xi32)
enum mem_xi32_opx {
    DECLARE_MEM_XI32_EXTENDED_OPCODES(OPCODE_X)
    mem_xi32_opx_last,
};

inline constexpr u64 insn_tmplt(const mem_xi32_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_mem_xi32)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

#define DECLARE_MEM_XU32_EXTENDED_OPCODES(X) DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, xu32)
enum mem_xu32_opx {
    DECLARE_MEM_XU32_EXTENDED_OPCODES(OPCODE_X)
    mem_xu32_opx_last,
};

inline constexpr u64 insn_tmplt(const mem_xu32_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_mem_xu32)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

/**************************************************************************************
* opcode_misc extended opcodes (11 bit)
**************************************************************************************/
#define OPCODE_TABLE_FP_SCALAR_OPS(X, TYPE, N) \
    X(class_        ## TYPE,          N +  0) \
    X(sqrt_         ## TYPE,          N +  1) \
    X(rsqrt_        ## TYPE,          N +  2) \
    X(add_          ## TYPE,          N +  3) \
    X(sub_          ## TYPE,          N +  4) \
    X(nadd_         ## TYPE,          N +  5) \
    X(mul_          ## TYPE,          N +  6) \
    X(nmul_         ## TYPE,          N +  7) \
    X(div_          ## TYPE,          N +  8) \
    X(neg_          ## TYPE,          N +  9) \
    X(abs_          ## TYPE,          N + 10) \
    X(nabs_         ## TYPE,          N + 11) \
    X(abs_diff_     ## TYPE,          N + 12) \
    X(nabs_diff_    ## TYPE,          N + 13) \
    X(min_          ## TYPE,          N + 14) \
    X(max_          ## TYPE,          N + 15) \
    X(minnum_       ## TYPE,          N + 16) \
    X(maxnum_       ## TYPE,          N + 17) \
    X(abs_min_      ## TYPE,          N + 18) \
    X(abs_max_      ## TYPE,          N + 19) \
    X(round_        ## TYPE,          N + 20) \
    X(ceil_         ## TYPE,          N + 21) \
    X(floor_        ## TYPE,          N + 22) \
    X(trunk_        ## TYPE,          N + 23) \
    X(roundeven_    ## TYPE,          N + 24) \
    X(nearbyint_    ## TYPE,          N + 25) \
    X(rint_         ## TYPE,          N + 26) \
    X(cmp_oeq_      ## TYPE,          N + 27) \
    X(cmp_one_      ## TYPE,          N + 28) \
    X(cmp_olt_      ## TYPE,          N + 29) \
    X(cmp_oge_      ## TYPE,          N + 30) \
    X(cmp_o_        ## TYPE,          N + 31) \
    X(cmp_ueq_      ## TYPE,          N + 32) \
    X(cmp_une_      ## TYPE,          N + 33) \
    X(cmp_ult_      ## TYPE,          N + 34) \
    X(cmp_uge_      ## TYPE,          N + 35) \
    X(cmp_u_        ## TYPE,          N + 36) \
    X(trunk_        ## TYPE ## _i32,  N + 37) \
    X(trunk_        ## TYPE ## _u32,  N + 38) \
    X(cvt_i32_      ## TYPE,          N + 39) \
    X(cvt_u32_      ## TYPE,          N + 40) \
    X(trunk_        ## TYPE ## _i64,  N + 41) \
    X(trunk_        ## TYPE ## _u64,  N + 42) \
    X(cvt_i64_      ## TYPE,          N + 43) \
    X(cvt_u64_      ## TYPE,          N + 44) \
    X(trunk_        ## TYPE ## _i128, N + 45) \
    X(trunk_        ## TYPE ## _u128, N + 46) \
    X(cvt_i128_     ## TYPE,          N + 47) \
    X(cvt_u128_     ## TYPE,          N + 48) \

#define OPCODE_TABLE_FP_PACKED_OPS(X, TYPE, ITYPE, N) \
    X(neg_          ## TYPE,        N +  0) \
    X(abs_          ## TYPE,        N +  1) \
    X(nabs_         ## TYPE,        N +  2) \
    X(abs_diff_     ## TYPE,        N +  3) \
    X(nabs_diff_    ## TYPE,        N +  4) \
    X(rsqrt_        ## TYPE,        N +  5) \
    X(sqrt_         ## TYPE,        N +  6) \
    X(add_          ## TYPE,        N +  7) \
    X(sub_          ## TYPE,        N +  8) \
    X(nadd_         ## TYPE,        N +  9) \
    X(mul_          ## TYPE,        N + 10) \
    X(nmul_         ## TYPE,        N + 11) \
    X(div_          ## TYPE,        N + 12) \
    X(min_          ## TYPE,        N + 13) \
    X(max_          ## TYPE,        N + 14) \
    X(minnum_       ## TYPE,        N + 15) \
    X(maxnum_       ## TYPE,        N + 16) \
    X(abs_min_      ## TYPE,        N + 17) \
    X(abs_max_      ## TYPE,        N + 18) \
    X(round_        ## TYPE,        N + 19) \
    X(ceil_         ## TYPE,        N + 20) \
    X(floor_        ## TYPE,        N + 21) \
    X(trunk_        ## TYPE,        N + 22) \
    X(roundeven_    ## TYPE,        N + 23) \
    X(nearbyint_    ## TYPE,        N + 24) \
    X(rint_         ## TYPE,        N + 25) \
    X(cmp_oeq_      ## TYPE,        N + 26) \
    X(cmp_one_      ## TYPE,        N + 27) \
    X(cmp_olt_      ## TYPE,        N + 28) \
    X(cmp_oge_      ## TYPE,        N + 29) \
    X(cmp_o_        ## TYPE,        N + 30) \
    X(cmp_ueq_      ## TYPE,        N + 31) \
    X(cmp_une_      ## TYPE,        N + 32) \
    X(cmp_ult_      ## TYPE,        N + 33) \
    X(cmp_uge_      ## TYPE,        N + 34) \
    X(cmp_u_        ## TYPE,        N + 35) \
    X(add_alt_      ## TYPE,        N + 36) \
    X(sub_alt_      ## TYPE,        N + 37) \
    X(add_horiz_    ## TYPE,        N + 38) \
    X(sub_horiz_    ## TYPE,        N + 39) \
    X(mul_horiz_    ## TYPE,        N + 40) \
    X(dot_          ## TYPE,        N + 41) \
    X(merge_low_    ## TYPE,        N + 42) \
    X(merge_high_   ## TYPE,        N + 43) \
    X(unpack_high_  ## TYPE,        N + 44) \
    X(unpack_low_   ## TYPE,        N + 45) \
    X(pack_         ## TYPE,        N + 46) \
    X(trunk_ ## TYPE  ## _vi ## ITYPE, N + 47) \
    X(trunk_ ## TYPE  ## _vu ## ITYPE, N + 48) \
    X(cvt_vi ## ITYPE ## _   ## TYPE,  N + 49) \
    X(cvt_vu ## ITYPE ## _   ## TYPE,  N + 50) \

#define OPCODE_TABLE_MMX_OPS(X, STYPE, UTYPE, N) \
    X(max_       ## STYPE, N +  0) \
    X(max_       ## UTYPE, N +  1) \
    X(min_       ## STYPE, N +  2) \
    X(min_       ## UTYPE, N +  3) \
    X(add_       ## UTYPE, N +  4) \
    X(sub_       ## UTYPE, N +  5) \
    X(addo_      ## STYPE, N +  6) \
    X(subo_      ## STYPE, N +  7) \
    X(addc_      ## UTYPE, N +  8) \
    X(subb_      ## UTYPE, N +  9) \
    X(add_sat_   ## UTYPE, N + 10) \
    X(add_sat_   ## STYPE, N + 11) \
    X(sub_sat_   ## STYPE, N + 12) \
    X(sub_sat_   ## UTYPE, N + 13) \
    X(avg_       ## STYPE, N + 14) \
    X(avg_       ## UTYPE, N + 15) \
    X(cmp_eq_    ## STYPE, N + 16) \
    X(cmp_lt_    ## STYPE, N + 17) \
    X(cmp_lt_    ## UTYPE, N + 18) \
    X(sll_       ## UTYPE, N + 19) \
    X(sll_imm_   ## UTYPE, N + 20) \
    X(srl_       ## UTYPE, N + 21) \
    X(srl_imm_   ## UTYPE, N + 22) \
    X(sra_       ## STYPE, N + 23) \
    X(sra_imm_   ## STYPE, N + 24) \
    X(rol_       ## UTYPE, N + 25) \
    X(ror_       ## UTYPE, N + 26) \
    X(merge_high_## UTYPE, N + 27) \
    X(merge_low_ ## UTYPE, N + 28) \

enum {
     SPECIAL_BASE  = 256,
     ATOMICS_BASE  = 256 + 64,
     FPU32_BASE    = 512 + 256,
     FPU64_BASE    = FPU32_BASE + 128,
     FPU128_BASE   = FPU64_BASE + 128,
     FPU16_BASE    = FPU128_BASE + 128,
     MMXB_BASE     = 1024 + 256,
     MMX_SIZE      = 128,
     MMXH_BASE     = MMXB_BASE + MMX_SIZE,
     MMXW_BASE     = MMXH_BASE + MMX_SIZE,
     MMXD_BASE     = MMXW_BASE + MMX_SIZE,
};

#define DECLARE_MISC_EXTENDED_OPCODES(X) \
    X(undef,        0) \
    X(mov,          1) \
    X(add_i32,      2) \
    X(sub_i32,      3) \
    X(mul_i32,      4) \
    X(mul_u32,      5) \
    X(add_u32,      6) \
    X(sub_u32,      7) \
    X(cmp_eq_i64,   8) \
    X(cmp_ne_i64,   9) \
    X(cmp_lt_i64,   10) \
    X(cmp_ge_i64,   11) \
    X(cmp_lt_u64,   12) \
    X(cmp_ge_u64,   13) \
    X(neg_i32,      14) \
    X(neg_i64,      15) \
    X(cmp_eq_i32,   16) \
    X(cmp_ne_i32,   17) \
    X(cmp_lt_i32,   18) \
    X(cmp_ge_i32,   19) \
    X(cmp_lt_u32,   20) \
    X(cmp_ge_u32,   21) \
    X(abs_i32,      22) \
    X(abs_i64,      23) \
    X(and,          24) \
    X(or,           25) \
    X(xor,          26) \
    X(andn,         27) \
    X(nand,         28) \
    X(nor,          29) \
    X(xnor,         30) \
    X(orn,          31) \
    X(add_i64,      32) \
    X(sub_i64,      33) \
    X(addo_i64,     34) \
    X(subo_i64,     35) \
    X(addc_u64,     36) \
    X(subb_u64,     37) \
    X(mul_i64,      38) \
    X(mul_h,        39) \
    X(div_i64,      40) \
    X(div_u64,      41) \
    X(div_i32,      42) \
    X(div_u32,      43) \
    X(rem_i64,      44) \
    X(rem_u64,      45) \
    X(rem_i32,      46) \
    X(rem_u32,      47) \
    X(max_i64,      48) \
    X(max_u64,      49) \
    X(min_i64,      50) \
    X(min_u64,      51) \
    X(max_i32,      52) \
    X(max_u32,      53) \
    X(min_i32,      54) \
    X(min_u32,      55) \
    X(sll_u64,      56) \
    X(sra_i64,      57) \
    X(srl_u64,      58) \
    X(srd_i64,      59) \
    X(sll_imm_u64,  60) \
    X(sra_imm_i64,  61) \
    X(srl_imm_u64,  62) \
    X(srd_imm_i64,  63) \
    X(sll_u32,      64) \
    X(sra_i32,      65) \
    X(srl_u32,      66) \
    X(sll_imm_u32,  67) \
    X(sra_imm_i32,  68) \
    X(srl_imm_u32,  69) \
    X(abs_diff_i64, 70) \
    X(abs_diff_i32, 71) \
    X(and_neg,      72) \
    X(xor_dec,      73) \
    X(and_dec,      74) \
    X(mul_i128,     75) /* reserved 76-79 */ \
    X(jmp_r,        80) \
    X(jmp_t,        81) \
    X(jmp_t_i32,    82) \
    X(jmp_t_u32,    83) \
    X(call_rvt,     84) \
    X(call_ri,      85) \
    X(call_mi,      86) \
    X(not,          87) \
    X(ret,          88) \
    X(mbgath,       89) \
    X(mbscat,       90) \
    X(cnt_pop,      91) \
    X(cnt_lz,       92) \
    X(cnt_tz,       93) \
    X(permb,        94) \
    X(gtb,          95) \
    X(add_i128,     96) \
    X(sub_i128,     97) \
    X(cmp_eq_i128,  98) \
    X(cmp_ne_i128,  99) \
    X(cmp_lt_i128,  100) \
    X(cmp_ge_i128,  101) \
    X(cmp_lt_u128,  102) \
    X(cmp_ge_u128,  103) \
    X(srd_i32,      104) \
    X(srd_imm_i32,  105) \
    X(neg_i128,     106) \
    X(abs_i128,     107) \
    X(bit_clear,    108) \
    X(bit_clear_imm,109) \
    X(bit_set,      110) \
    X(bit_set_imm,  111) \
    X(bit_flip,     112) \
    X(bit_flip_imm, 113) /* reserved 114-119 */ \
    X(extend_f32_f64,     120) \
    X(extend_f32_f128,    121) \
    X(extend_f64_f128,    122) \
    X(extend_f16_f32,     123) \
    X(extend_f16_f64,     124) \
    X(extend_f16_f128,    125) \
    X(cvt_f64_f32,   126) \
    X(cvt_f128_f64,  127) \
    X(cvt_f128_f32,  128) \
    X(cvt_f32_f16,   129) \
    X(cvt_f64_f16,   130) \
    X(cvt_f128_f16,  131) /* reserved: 132-135 */ \
    X(ld_mia_u8,    136) \
    X(ld_mia_i8,    137) \
    X(st_mia_i8,    138) \
    X(ld_mia_u16,   139) \
    X(ld_mia_i16,   140) \
    X(st_mia_i16,   141) \
    X(ld_mia_u32,   142) \
    X(ld_mia_i32,   143) \
    X(st_mia_i32,   144) \
    X(ld_mia_u64,   145) \
    X(ld_mia_i64,   146) \
    X(st_mia_i64,   147) \
    X(ld_mia_i128,  148) \
    X(st_mia_i128,  149) \
    X(ld_mib_u8,    152) \
    X(ld_mib_i8,    153) \
    X(st_mib_i8,    154) \
    X(ld_mib_u16,   155) \
    X(ld_mib_i16,   156) \
    X(st_mib_i16,   157) \
    X(ld_mib_u32,   158) \
    X(ld_mib_i32,   159) \
    X(st_mib_i32,   160) \
    X(ld_mib_u64,   161) \
    X(ld_mib_i64,   162) \
    X(st_mib_i64,   163) \
    X(ld_mib_i128,  164) \
    X(st_mib_i128,  165) /* reserved: 166-179 */ \
    X(mprobe,            SPECIAL_BASE +  0) \
    X(cpuid,             SPECIAL_BASE +  1) \
    X(int,               SPECIAL_BASE +  2) \
    X(syscall,           SPECIAL_BASE +  3) \
    X(random,            SPECIAL_BASE +  4) \
    X(get_spr,           SPECIAL_BASE +  5) \
    X(set_spr,           SPECIAL_BASE +  6) \
    X(set_dbr,           SPECIAL_BASE +  7) \
    X(get_dbr,           SPECIAL_BASE +  8) \
    X(set_ibr,           SPECIAL_BASE +  9) \
    X(get_ibr,           SPECIAL_BASE + 10) \
    X(set_mr,            SPECIAL_BASE + 11) \
    X(get_mr,            SPECIAL_BASE + 12) \
    X(set_itr,           SPECIAL_BASE + 13) \
    X(set_dtr,           SPECIAL_BASE + 14) \
    X(rfi,               SPECIAL_BASE + 15) \
    X(halt,              SPECIAL_BASE + 16) \
    X(tpa,               SPECIAL_BASE + 17) \
    X(ptc,               SPECIAL_BASE + 18) \
    X(sysret,            SPECIAL_BASE + 22) \
    X(rscover,           SPECIAL_BASE + 24) \
    X(rsflush,           SPECIAL_BASE + 25) \
    X(rsload,            SPECIAL_BASE + 26) \
    X(clmul_ll,          SPECIAL_BASE + 32) \
    X(clmul_hl,          SPECIAL_BASE + 33) \
    X(clmul_hh,          SPECIAL_BASE + 34) \
    X(aes_enc,           SPECIAL_BASE + 40) \
    X(aes_enc_last,      SPECIAL_BASE + 41) \
    X(aes_dec,           SPECIAL_BASE + 42) \
    X(aes_dec_last,      SPECIAL_BASE + 43) \
    X(aes_imc,           SPECIAL_BASE + 44) \
    X(aes_keygen_assist, SPECIAL_BASE + 45) \
    X(sll_imm_u128,      SPECIAL_BASE + 48) \
    X(srl_imm_u128,      SPECIAL_BASE + 49) \
    X(sra_imm_i128,      SPECIAL_BASE + 50) \
    X(jmp_mi,            SPECIAL_BASE + 51) \
    X(fence_a,           SPECIAL_BASE + 52) \
    X(fence_r,           SPECIAL_BASE + 53) \
    X(fence_ar,          SPECIAL_BASE + 54) \
    X(fence_sc,          SPECIAL_BASE + 55) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i8,   u8,   ATOMICS_BASE + 80 * 0) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i16,  u16,  ATOMICS_BASE + 80 * 1) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i32,  u32,  ATOMICS_BASE + 80 * 2) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i64,  u64,  ATOMICS_BASE + 80 * 3) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i128, u128, ATOMICS_BASE + 80 * 4) \
    /* fpu */ \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f32,      FPU32_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, vf32, 32, FPU32_BASE + 64) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f64,      FPU64_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, vf64, 64, FPU64_BASE + 64) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f128,    FPU128_BASE) \
    X(scale_f128,       FPU128_BASE + 56) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f16,    FPU16_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, vf16, 16, FPU16_BASE + 64) \
    /* mmx */ \
    OPCODE_TABLE_MMX_OPS(X, vi8, vu8, MMXB_BASE) \
    X(unpack_low_vi8,   MMXB_BASE + 32) \
    X(unpack_high_vi8,  MMXB_BASE + 33) \
    X(unpack_low_vu8,   MMXB_BASE + 34) \
    X(unpack_high_vu8,  MMXB_BASE + 35) \
    OPCODE_TABLE_MMX_OPS(X, vi16, vu16, MMXH_BASE) \
    X(unpack_low_vi16,  MMXH_BASE + 32) \
    X(unpack_high_vi16, MMXH_BASE + 33) \
    X(unpack_low_vu16,  MMXH_BASE + 34) \
    X(unpack_high_vu16, MMXH_BASE + 35) \
    X(pack_sat_vi16,    MMXH_BASE + 36) \
    X(pack_sat_vu16,    MMXH_BASE + 37) \
    X(pack_mod_vu16,    MMXH_BASE + 38) \
    X(pack_usat_vi16,   MMXH_BASE + 39) \
    OPCODE_TABLE_MMX_OPS(X, vi32, vu32, MMXW_BASE) \
    X(unpack_low_vi32,  MMXW_BASE + 32) \
    X(unpack_high_vi32, MMXW_BASE + 33) \
    X(unpack_low_vu32,  MMXW_BASE + 34) \
    X(unpack_high_vu32, MMXW_BASE + 35) \
    X(pack_sat_vi32,    MMXW_BASE + 36) \
    X(pack_sat_vu32,    MMXW_BASE + 37) \
    X(pack_mod_vu32,    MMXW_BASE + 38) \
    X(pack_usat_vi32,   MMXW_BASE + 39) \
    OPCODE_TABLE_MMX_OPS(X, vi64, vu64, MMXD_BASE) \
    X(pack_sat_vi64,    MMXD_BASE + 33) \
    X(pack_sat_vu64,    MMXD_BASE + 34) \
    X(pack_mod_vu64,    MMXD_BASE + 36) \
    X(pack_usat_vi64,   MMXD_BASE + 37) \

enum EMiscOpx {
    DECLARE_MISC_EXTENDED_OPCODES(OPCODE_X)
    misc_opx_last,
};

inline constexpr u64 insn_tmplt(const EMiscOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_misc)
         | makepart(static_cast<u64>(opx), disp_misc_opx);
}

#define INSN_TMPLT(opc) insn_tmplt(opcode_##opc)

} // namespace postrisc
