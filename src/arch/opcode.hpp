
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
#define LOAD_STORE_OPCODES(X, SUFFIX, N) \
    X(ld ## SUFFIX ## _u8,     N + 0) \
    X(ld ## SUFFIX ## _i8,     N + 1) \
    X(st ## SUFFIX ## _i8,     N + 2) \
    X(ld ## SUFFIX ## _u16,    N + 3) \
    X(ld ## SUFFIX ## _i16,    N + 4) \
    X(st ## SUFFIX ## _i16,    N + 5) \
    X(ld ## SUFFIX ## _u32,    N + 6) \
    X(ld ## SUFFIX ## _i32,    N + 7) \
    X(st ## SUFFIX ## _i32,    N + 8) \
    X(ld ## SUFFIX ## _u64,    N + 9) \
    X(ld ## SUFFIX ## _i64,    N + 10) \
    X(st ## SUFFIX ## _i64,    N + 11) \
    X(ld ## SUFFIX ## _i128,   N + 12) \
    X(st ## SUFFIX ## _i128,   N + 13) \

#define DECLARE_INT_CMP_OPS(X, PREFIX, SUFFIX, SIZE, N) \
    X(PREFIX ## _eq ## SUFFIX ## _i ## SIZE, N + 0) \
    X(PREFIX ## _ne ## SUFFIX ## _i ## SIZE, N + 1) \
    X(PREFIX ## _lt ## SUFFIX ## _i ## SIZE, N + 2) \
    X(PREFIX ## _ge ## SUFFIX ## _i ## SIZE, N + 3) \
    X(PREFIX ## _lt ## SUFFIX ## _u ## SIZE, N + 4) \
    X(PREFIX ## _ge ## SUFFIX ## _u ## SIZE, N + 5) \

#define DECLARE_FP_CMP_OPS(X, PREFIX, TYPE, N) \
    X(PREFIX ## _oeq_ ## TYPE, N + 0) \
    X(PREFIX ## _one_ ## TYPE, N + 1) \
    X(PREFIX ## _olt_ ## TYPE, N + 2) \
    X(PREFIX ## _oge_ ## TYPE, N + 3) \
    X(PREFIX ## _o_   ## TYPE, N + 4) \
    X(PREFIX ## _ueq_ ## TYPE, N + 5) \
    X(PREFIX ## _une_ ## TYPE, N + 6) \
    X(PREFIX ## _ult_ ## TYPE, N + 7) \
    X(PREFIX ## _uge_ ## TYPE, N + 8) \
    X(PREFIX ## _u_   ## TYPE, N + 9) \

#define DECLARE_BR_OR_NUL_IMM_OPCODES(X, PREFIX, N) \
    DECLARE_INT_CMP_OPS(X, PREFIX, _imm,  32, N + 0) \
    DECLARE_INT_CMP_OPS(X, PREFIX, _imm,  64, N + 6) \
    DECLARE_INT_CMP_OPS(X, PREFIX, _imm, 128, N + 12) \
    X(PREFIX ## _mask_all,     N + 18) \
    X(PREFIX ## _mask_notall,  N + 19) \
    X(PREFIX ## _mask_none,    N + 20) \
    X(PREFIX ## _mask_any,     N + 21) \


#define DECLARE_BR_OR_NUL_EXTENDED_OPCODES(X, PREFIX, SIZE) \
    DECLARE_INT_CMP_OPS(X, PREFIX, /*empty*/, SIZE, 0) \
    DECLARE_FP_CMP_OPS(X, PREFIX, f ## SIZE, 6) \

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
    OPCODE_TABLE_ATOMIC_LD(X,    UTYPE, ld,     N + 0) \
    OPCODE_TABLE_ATOMIC_ST(X,    UTYPE, st,     N + 2) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, swap,   N + 4) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_add, N + 12) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_and, N + 16) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_or,  N + 20) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_xor, N + 24) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, ITYPE, ld_min, N + 28) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, ITYPE, ld_max, N + 32) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_min, N + 36) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, UTYPE, ld_max, N + 40) \

/*
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_add, N + 44) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_and, N + 46) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_or,  N + 48) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_xor, N + 50) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, ITYPE, st_min, N + 52) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, ITYPE, st_max, N + 54) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_min, N + 56) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, UTYPE, st_max, N + 58) \
*/

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
    X(fma,             11) \
    X(mem_xu64,        12) /* reserved: 13-15 */ \
    X(ld_imm,          16) \
    X(ld_imm_high,     17) \
    X(andn_imm,        18) \
    X(orn_imm,         19) \
    X(and_imm,         20) \
    X(or_imm,          21) \
    X(xor_imm,         22) \
    X(deposit,         23) \
    X(add_imm_i32,     24) \
    X(add_imm_u32,     25) \
    X(subr_imm_i32,    26) \
    X(subr_imm_u32,    27) \
    X(mul_imm_i32,     28) \
    X(mul_imm_u32,     29) \
    X(add_imm_i64,     30) \
    X(subr_imm_i64,    31) \
    X(mul_imm_i64,     32) /* reserved: 33-35 */ \
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
    DECLARE_INT_CMP_OPS(X, cmp, _imm,  64, 52) \
    DECLARE_INT_CMP_OPS(X, cmp, _imm,  32, 58) \
    DECLARE_INT_CMP_OPS(X, cmp, _imm, 128, 64) \
    X(add_imm_i128,    70) /* reserved: 71 */ \
    LOAD_STORE_OPCODES(X, /* none */, 72) /* reserved: 86, 87 */ \
    LOAD_STORE_OPCODES(X, _iprel, 88) \
    X(lda_iprel,       102) \
    X(call_plt,        103) \
    X(call,            104) \
    X(lda_r,           105) \
    DECLARE_BR_OR_NUL_IMM_OPCODES(X, br, 106)

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
    X(deposit_r,       43) \
    X(slp_i128,        44) \
    X(srp_i128,        45) \
    X(srp_imm_i128,    46) \
    X(sl_or,           50) \
    X(sl_xor,          51) /* reserved: 52-59 */ \
    X(call_mi,        100) \
    X(call_rvt,       101) \
    X(lda_n,          102) \
    X(lda_nrc,        103) \
    DECLARE_BR_OR_NUL_IMM_OPCODES(X, nul, 106)

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
#define FMA_SCALAR_OPS(X, TYPE, N) \
    X(madd_  ## TYPE,  N + 0) \
    X(msub_  ## TYPE,  N + 1) \
    X(nmadd_ ## TYPE,  N + 2) \
    X(nmsub_ ## TYPE,  N + 3) \
    X(merge_ ## TYPE,  N + 4) \

#define FMA_VECTOR_OPS(X, TYPE, N) \
    X(madd_ ## TYPE,       N + 0) \
    X(msub_ ## TYPE,       N + 1) \
    X(nmadd_ ## TYPE,      N + 2) \
    X(nmsub_ ## TYPE,      N + 3) \
    X(madd_alt_ ## TYPE,   N + 4) \
    X(msub_alt_ ## TYPE,   N + 5) \
    X(merge_ ## TYPE,      N + 6) \

#define DECLARE_FMA_EXTENDED_OPCODES(X) \
    X(cmov_lsb,        0) \
    X(cmov_eq_i32,     1) \
    X(cmov_lt_i32,     2) \
    X(cmov_le_i32,     3) \
    X(cmov_eq_i64,     5) \
    X(cmov_lt_i64,     6) \
    X(cmov_le_i64,     7) \
    X(cmov_eq_i128,    9) \
    X(cmov_lt_i128,    10) \
    X(cmov_le_i128,    11) \
    X(mul_add,         12) \
    X(mul_sub,         13) \
    X(mul_subr,        14) \
    FMA_SCALAR_OPS(X, f128, 20) \
    FMA_SCALAR_OPS(X, f64,  36) \
    FMA_VECTOR_OPS(X, vf64, 36 + 8) \
    FMA_SCALAR_OPS(X, f32,  60) \
    FMA_VECTOR_OPS(X, vf32, 60 + 8) \
    FMA_SCALAR_OPS(X, f16,  84) \
    FMA_VECTOR_OPS(X, vf16, 84 + 8) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, u8,   cas,    108) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, u16,  cas,    112) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, u32,  cas,    116) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, u64,  cas,    120) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, u128, cas,    124) \

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
    LOAD_STORE_OPCODES(X, SUFFIX, 0) \
    X(lda ## SUFFIX, 14) \

#define DECLARE_MEM_XI64_EXTENDED_OPCODES(X) DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, _xi64)
enum mem_xi64_opx {
    DECLARE_MEM_XI64_EXTENDED_OPCODES(OPCODE_X)
    mem_xi64_opx_last,
};

inline constexpr u64 insn_tmplt(const mem_xi64_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_mem_xi64)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

#define DECLARE_MEM_XU64_EXTENDED_OPCODES(X) DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, _xu64)
enum mem_xu64_opx {
    DECLARE_MEM_XU64_EXTENDED_OPCODES(OPCODE_X)
    mem_xu64_opx_last,
};

inline constexpr u64 insn_tmplt(const mem_xu64_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_mem_xu64)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

#define DECLARE_MEM_XI32_EXTENDED_OPCODES(X) DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, _xi32)
enum mem_xi32_opx {
    DECLARE_MEM_XI32_EXTENDED_OPCODES(OPCODE_X)
    mem_xi32_opx_last,
};

inline constexpr u64 insn_tmplt(const mem_xi32_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_mem_xi32)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

#define DECLARE_MEM_XU32_EXTENDED_OPCODES(X) DECLARE_MEM_INDEXED_EXTENDED_OPCODES(X, _xu32)
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
    X(trunc_        ## TYPE,          N + 23) \
    X(roundeven_    ## TYPE,          N + 24) \
    X(nearbyint_    ## TYPE,          N + 25) \
    X(rint_         ## TYPE,          N + 26) \
    DECLARE_FP_CMP_OPS(X, cmp, TYPE,  N + 28) /* 10 ops */ \
    X(trunc_        ## TYPE ## _i32,  N + 40) \
    X(trunc_        ## TYPE ## _u32,  N + 41) \
    X(cvt_i32_      ## TYPE,          N + 42) \
    X(cvt_u32_      ## TYPE,          N + 43) \
    X(trunc_        ## TYPE ## _i64,  N + 44) \
    X(trunc_        ## TYPE ## _u64,  N + 45) \
    X(cvt_i64_      ## TYPE,          N + 46) \
    X(cvt_u64_      ## TYPE,          N + 47) \
    X(trunc_        ## TYPE ## _i128, N + 48) \
    X(trunc_        ## TYPE ## _u128, N + 49) \
    X(cvt_i128_     ## TYPE,          N + 50) \
    X(cvt_u128_     ## TYPE,          N + 51) \

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
    X(trunc_        ## TYPE,        N + 22) \
    X(roundeven_    ## TYPE,        N + 23) \
    X(nearbyint_    ## TYPE,        N + 24) \
    X(rint_         ## TYPE,        N + 25) \
    DECLARE_FP_CMP_OPS(X,cmp,TYPE,  N + 28) /* 10 ops */ \
    X(add_alt_      ## TYPE,        N + 40) \
    X(sub_alt_      ## TYPE,        N + 41) \
    X(add_horiz_    ## TYPE,        N + 42) \
    X(sub_horiz_    ## TYPE,        N + 43) \
    X(mul_horiz_    ## TYPE,        N + 44) \
    X(dot_          ## TYPE,        N + 45) \
    X(merge_low_    ## TYPE,        N + 46) \
    X(merge_high_   ## TYPE,        N + 47) \
    X(unpack_high_  ## TYPE,        N + 48) \
    X(unpack_low_   ## TYPE,        N + 49) \
    X(pack_         ## TYPE,        N + 50) \
    X(trunc_ ## TYPE  ## _vi ## ITYPE, N + 51) \
    X(trunc_ ## TYPE  ## _vu ## ITYPE, N + 52) \
    X(cvt_vi ## ITYPE ## _   ## TYPE,  N + 53) \
    X(cvt_vu ## ITYPE ## _   ## TYPE,  N + 54) \

#define OPCODE_TABLE_MMX_OPS(X, STYPE, UTYPE, N) \
    X(max_        ## STYPE, N +  0) \
    X(max_        ## UTYPE, N +  1) \
    X(min_        ## STYPE, N +  2) \
    X(min_        ## UTYPE, N +  3) \
    X(add_        ## UTYPE, N +  4) \
    X(sub_        ## UTYPE, N +  5) \
    X(addo_       ## STYPE, N +  6) \
    X(subo_       ## STYPE, N +  7) \
    X(addc_       ## UTYPE, N +  8) \
    X(subb_       ## UTYPE, N +  9) \
    X(add_sat_    ## UTYPE, N + 10) \
    X(add_sat_    ## STYPE, N + 11) \
    X(sub_sat_    ## STYPE, N + 12) \
    X(sub_sat_    ## UTYPE, N + 13) \
    X(avg_        ## STYPE, N + 14) \
    X(avg_        ## UTYPE, N + 15) \
    X(cmp_eq_     ## STYPE, N + 16) \
    X(cmp_lt_     ## STYPE, N + 17) \
    X(cmp_lt_     ## UTYPE, N + 18) \
    X(sll_        ## UTYPE, N + 19) \
    X(sll_imm_    ## UTYPE, N + 20) \
    X(srl_        ## UTYPE, N + 21) \
    X(srl_imm_    ## UTYPE, N + 22) \
    X(sra_        ## STYPE, N + 23) \
    X(sra_imm_    ## STYPE, N + 24) \
    X(rol_        ## UTYPE, N + 25) \
    X(ror_        ## UTYPE, N + 26) \
    X(merge_high_ ## UTYPE, N + 27) \
    X(merge_low_  ## UTYPE, N + 28) \

#define MISC_INT_SCALAR_OPS_32(X, SIZE, N) \
    X(add_i       ## SIZE,  N + 0) \
    X(sub_i       ## SIZE,  N + 1) \
    X(mul_i       ## SIZE,  N + 2) \
    X(neg_i       ## SIZE,  N + 3) \
    X(sll_imm_u   ## SIZE,  N + 4) \
    X(srl_imm_u   ## SIZE,  N + 5) \
    X(sra_imm_i   ## SIZE,  N + 6) \
    X(divp2_imm_i ## SIZE,  N + 7) \
    X(sll_u       ## SIZE,  N + 8) \
    X(srl_u       ## SIZE,  N + 9) \
    X(sra_i       ## SIZE,  N + 10) \
    X(divp2_i     ## SIZE,  N + 11) \
    DECLARE_INT_CMP_OPS(X, cmp, /*empty*/, SIZE, N + 12) /* 6 ops */ \
    X(abs_diff_i  ## SIZE,  N + 18) \
    X(abs_i       ## SIZE,  N + 19) \
    X(div_i       ## SIZE,  N + 20) \
    X(div_u       ## SIZE,  N + 21) \
    X(rem_i       ## SIZE,  N + 22) \
    X(rem_u       ## SIZE,  N + 23) \
    X(max_i       ## SIZE,  N + 24) \
    X(max_u       ## SIZE,  N + 25) \
    X(min_i       ## SIZE,  N + 26) \
    X(min_u       ## SIZE,  N + 27) \

enum {
     INT_TYPED_BASE  = 128,
     INT_TYPED_SIZE  = 64 + 16,
     INT_TYPED_COMMON_SIZE = 32,
     INT_BASE_32     = INT_TYPED_BASE + 0 * INT_TYPED_SIZE,
     INT_BASE_64     = INT_TYPED_BASE + 1 * INT_TYPED_SIZE,
     INT_BASE_128    = INT_TYPED_BASE + 2 * INT_TYPED_SIZE,
     SPECIAL_BASE    = 512,
     ATOMICS_BASE    = 512 + 256,
     ATOMICS_SIZE    = 80,
     FPU_BASE        = 1024 + 128 + 64, // ATOMICS_BASE + ATOMICS_SIZE * 5 + xxx,
     FPU_SIZE        = 128 + 32,
     FPU_COMMON_SIZE = 56,
     FPU_SCALAR_SIZE = 64 + 16,
     FPU16_BASE      = FPU_BASE + 0 * FPU_SIZE,
     FPU32_BASE      = FPU_BASE + 1 * FPU_SIZE,
     FPU64_BASE      = FPU_BASE + 2 * FPU_SIZE,
     FPU128_BASE     = FPU_BASE + 3 * FPU_SIZE,
     MMX_SIZE        = 64,
     MMX_COMMON_SIZE = 32,
     MMX_BASE        = 1024 + 512 + 256,
     MMX_BASE_8      = MMX_BASE + 0 * MMX_SIZE,
     MMX_BASE_16     = MMX_BASE + 1 * MMX_SIZE,
     MMX_BASE_32     = MMX_BASE + 2 * MMX_SIZE,
     MMX_BASE_64     = MMX_BASE + 3 * MMX_SIZE,
};

#define DECLARE_MISC_EXTENDED_OPCODES(X) \
    X(undef,         0) \
    X(mov,           1) \
    X(ret,           2) \
    X(not,           3) \
    X(and,           4) \
    X(or,            5) \
    X(xor,           6) \
    X(andn,          7) \
    X(nand,          8) \
    X(nor,           9) \
    X(xnor,          10) \
    X(orn,           11) \
    X(jmp_r,         12) \
    X(jmp_t,         13) \
    X(jmp_t_i32,     14) \
    X(jmp_t_u32,     15) \
    X(call_ri,       16) \
    X(mbgath,        20) \
    X(mbscat,        21) \
    X(gtb,           22) \
    X(cnt_pop,       24) \
    X(cnt_lz,        25) \
    X(cnt_tz,        26) \
    X(permb,         27) \
    X(bit_clear,     28) \
    X(bit_clear_imm, 29) \
    X(bit_set,       30) \
    X(bit_set_imm,   31) \
    X(bit_flip,      32) \
    X(bit_flip_imm,  33) \
    X(sext_i8,       36) \
    X(sext_i16,      37) \
    X(sext_i32,      38) \
    X(sext_i64,      39) \
    X(zext_i8,       40) \
    X(zext_i16,      41) \
    X(zext_i32,      42) \
    X(zext_i64,      43) \
    LOAD_STORE_OPCODES(X, _mia, 96) \
    LOAD_STORE_OPCODES(X, _mib, 96 + 16) \
    MISC_INT_SCALAR_OPS_32(X, 32, INT_BASE_32) \
    X(mul_u32,      INT_BASE_32 + INT_TYPED_COMMON_SIZE + 0) \
    X(add_u32,      INT_BASE_32 + INT_TYPED_COMMON_SIZE + 1) \
    X(sub_u32,      INT_BASE_32 + INT_TYPED_COMMON_SIZE + 2) \
    MISC_INT_SCALAR_OPS_32(X, 64, INT_BASE_64) \
    X(xor_dec,      INT_BASE_64 + INT_TYPED_COMMON_SIZE + 0) \
    X(and_dec,      INT_BASE_64 + INT_TYPED_COMMON_SIZE + 1) \
    X(and_neg,      INT_BASE_64 + INT_TYPED_COMMON_SIZE + 2) \
    X(addo_i64,     INT_BASE_64 + INT_TYPED_COMMON_SIZE + 3) \
    X(subo_i64,     INT_BASE_64 + INT_TYPED_COMMON_SIZE + 4) \
    X(addc_u64,     INT_BASE_64 + INT_TYPED_COMMON_SIZE + 5) \
    X(subb_u64,     INT_BASE_64 + INT_TYPED_COMMON_SIZE + 6) \
    X(mul_h,        INT_BASE_64 + INT_TYPED_COMMON_SIZE + 7) \
    MISC_INT_SCALAR_OPS_32(X, 128, INT_BASE_128) \
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
    X(jmp_mi,            SPECIAL_BASE + 51) \
    X(fence_a,           SPECIAL_BASE + 52) \
    X(fence_r,           SPECIAL_BASE + 53) \
    X(fence_ar,          SPECIAL_BASE + 54) \
    X(fence_sc,          SPECIAL_BASE + 55) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i8,   u8,   ATOMICS_BASE + ATOMICS_SIZE * 0) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i16,  u16,  ATOMICS_BASE + ATOMICS_SIZE * 1) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i32,  u32,  ATOMICS_BASE + ATOMICS_SIZE * 2) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i64,  u64,  ATOMICS_BASE + ATOMICS_SIZE * 3) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, i128, u128, ATOMICS_BASE + ATOMICS_SIZE * 4) \
    /* fpu */ \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f16,    FPU16_BASE) \
    X(extend_f16_f32,     FPU16_BASE + FPU_COMMON_SIZE + 0) \
    X(extend_f16_f64,     FPU16_BASE + FPU_COMMON_SIZE + 1) \
    X(cvt_f32_f16,        FPU16_BASE + FPU_COMMON_SIZE + 2) \
    X(cvt_f64_f16,        FPU16_BASE + FPU_COMMON_SIZE + 3) \
    OPCODE_TABLE_FP_PACKED_OPS(X, vf16, 16, FPU16_BASE + FPU_SCALAR_SIZE) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f32,      FPU32_BASE) \
    X(extend_f32_f64,     FPU32_BASE + FPU_COMMON_SIZE + 0) \
    X(cvt_f64_f32,        FPU32_BASE + FPU_COMMON_SIZE + 1) \
    OPCODE_TABLE_FP_PACKED_OPS(X, vf32, 32, FPU32_BASE + FPU_SCALAR_SIZE) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f64,      FPU64_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, vf64, 64, FPU64_BASE + FPU_SCALAR_SIZE) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, f128,    FPU128_BASE) \
    X(extend_f32_f128,  FPU128_BASE + FPU_COMMON_SIZE + 0) \
    X(extend_f64_f128,  FPU128_BASE + FPU_COMMON_SIZE + 1) \
    X(extend_f16_f128,  FPU128_BASE + FPU_COMMON_SIZE + 2) \
    X(cvt_f128_f64,     FPU128_BASE + FPU_COMMON_SIZE + 3) \
    X(cvt_f128_f32,     FPU128_BASE + FPU_COMMON_SIZE + 4) \
    X(cvt_f128_f16,     FPU128_BASE + FPU_COMMON_SIZE + 5) \
    X(scale_f128,       FPU128_BASE + FPU_COMMON_SIZE + 6) \
    /* mmx */ \
    OPCODE_TABLE_MMX_OPS(X, vi8, vu8, MMX_BASE_8) \
    X(unpack_low_vi8,   MMX_BASE_8 + MMX_COMMON_SIZE + 0) \
    X(unpack_high_vi8,  MMX_BASE_8 + MMX_COMMON_SIZE + 1) \
    X(unpack_low_vu8,   MMX_BASE_8 + MMX_COMMON_SIZE + 2) \
    X(unpack_high_vu8,  MMX_BASE_8 + MMX_COMMON_SIZE + 3) \
    OPCODE_TABLE_MMX_OPS(X, vi16, vu16, MMX_BASE_16) \
    X(unpack_low_vi16,  MMX_BASE_16 + MMX_COMMON_SIZE + 0) \
    X(unpack_high_vi16, MMX_BASE_16 + MMX_COMMON_SIZE + 1) \
    X(unpack_low_vu16,  MMX_BASE_16 + MMX_COMMON_SIZE + 2) \
    X(unpack_high_vu16, MMX_BASE_16 + MMX_COMMON_SIZE + 3) \
    X(pack_sat_vi16,    MMX_BASE_16 + MMX_COMMON_SIZE + 4) \
    X(pack_sat_vu16,    MMX_BASE_16 + MMX_COMMON_SIZE + 5) \
    X(pack_mod_vu16,    MMX_BASE_16 + MMX_COMMON_SIZE + 6) \
    X(pack_usat_vi16,   MMX_BASE_16 + MMX_COMMON_SIZE + 7) \
    OPCODE_TABLE_MMX_OPS(X, vi32, vu32, MMX_BASE_32) \
    X(unpack_low_vi32,  MMX_BASE_32 + MMX_COMMON_SIZE + 0) \
    X(unpack_high_vi32, MMX_BASE_32 + MMX_COMMON_SIZE + 1) \
    X(unpack_low_vu32,  MMX_BASE_32 + MMX_COMMON_SIZE + 2) \
    X(unpack_high_vu32, MMX_BASE_32 + MMX_COMMON_SIZE + 3) \
    X(pack_sat_vi32,    MMX_BASE_32 + MMX_COMMON_SIZE + 4) \
    X(pack_sat_vu32,    MMX_BASE_32 + MMX_COMMON_SIZE + 5) \
    X(pack_mod_vu32,    MMX_BASE_32 + MMX_COMMON_SIZE + 6) \
    X(pack_usat_vi32,   MMX_BASE_32 + MMX_COMMON_SIZE + 7) \
    OPCODE_TABLE_MMX_OPS(X, vi64, vu64, MMX_BASE_64) \
    X(pack_sat_vi64,    MMX_BASE_64 + MMX_COMMON_SIZE + 4) \
    X(pack_sat_vu64,    MMX_BASE_64 + MMX_COMMON_SIZE + 5) \
    X(pack_mod_vu64,    MMX_BASE_64 + MMX_COMMON_SIZE + 6) \
    X(pack_usat_vi64,   MMX_BASE_64 + MMX_COMMON_SIZE + 7) \


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
