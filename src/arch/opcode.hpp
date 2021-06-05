#pragma once

#include "instruction_fields.hpp"

namespace postrisc {

inline constexpr u64 makepart(u64 item, unsigned int base)
{
    return item << base;
}

#define OPCODE_X(ID, VALUE)      opcode_##ID = VALUE,
#define PSEUDO_INSN(ID, ORIG_ID) opcode_##ID = opcode_##ORIG_ID,

#define DECLARE_INT_COMPARES(X, OP, TYPE, N) \
    X(OP ## _eq_  ## TYPE, N + 0) \
    X(OP ## _ne_  ## TYPE, N + 1) \
    X(OP ## s_lt_ ## TYPE, N + 2) \
    X(OP ## s_le_ ## TYPE, N + 3) \
    X(OP ## u_lt_ ## TYPE, N + 4) \
    X(OP ## u_le_ ## TYPE, N + 5) \

// int compare reg-reg and branch or nullify ops
#define DECLARE_INT_COMPARE_AND_OP(X, OP) \
    DECLARE_INT_COMPARES(X, OP, d, 0) /* 6 ops */ \
    DECLARE_INT_COMPARES(X, OP, w, 6) /* 6 ops */ \
    X(OP ## _bc,   12) \
    X(OP ## _bci,  13) \
    X(OP ## _bs,   14) \
    X(OP ## _bsi,  15) \

// fp branch or nullify ops
#define DECLARE_FP_SCALAR_COMPARE_AND_OP(X, OP, TYPE) \
    X(OP ## o_eq_   ## TYPE, 0) \
    X(OP ## o_ne_   ## TYPE, 1) \
    X(OP ## o_lt_   ## TYPE, 2) \
    X(OP ## o_le_   ## TYPE, 3) \
    X(OP ## u_eq_   ## TYPE, 4) \
    X(OP ## u_ne_   ## TYPE, 5) \
    X(OP ## u_lt_   ## TYPE, 6) \
    X(OP ## u_le_   ## TYPE, 7) \
    X(OP ## o_      ## TYPE, 8) \
    X(OP ## u_      ## TYPE, 9) \
    X(OP ## _class_ ## TYPE, 10) \

/***************************************************************************
*
* main opcode - 7 bit
*
***************************************************************************/
#define OPCODE_TABLE_COMPARE_IMM_OPS(X, TYPE, N) \
    X(cmpi_eq_  ## TYPE, N + 0) \
    X(cmpi_ne_  ## TYPE, N + 1) \
    X(cmpsi_lt_ ## TYPE, N + 2) \
    X(cmpui_lt_ ## TYPE, N + 3) \
    X(cmpsi_gt_ ## TYPE, N + 4) \
    X(cmpui_gt_ ## TYPE, N + 5) \

#define OPCODE_TABLE_LOADSTORE_OPS(X, TYPE, N) \
    X(ldz_b ## TYPE, N +  0) \
    X(lds_b ## TYPE, N +  1) \
    X(st_b  ## TYPE, N +  2) \
    X(ldz_h ## TYPE, N +  3) \
    X(lds_h ## TYPE, N +  4) \
    X(st_h  ## TYPE, N +  5) \
    X(ldz_w ## TYPE, N +  6) \
    X(lds_w ## TYPE, N +  7) \
    X(st_w  ## TYPE, N +  8) \
    X(ldz_d ## TYPE, N +  9) \
    X(lds_d ## TYPE, N + 10) \
    X(st_d  ## TYPE, N + 11) \
    X(ld_q  ## TYPE, N + 12) \
    X(st_q  ## TYPE, N + 13) \


#define DECLARE_PRIMARY_OPCODES(X) \
    X(misc,       0) \
    X(fused,      1) \
    X(raopx,      2) \
    X(brr,        3) \
    X(loop,       4) \
    X(bfss,       5) \
    X(bfsd,       6) \
    X(bfsq,       7) \
    X(indexed_ds, 8) \
    X(indexed_ws, 9) \
    X(indexed_wz,10) /* gap: 11-15 */ \
    X(ldi,       16) \
    X(ldih,      17) \
    X(andni,     18) \
    X(orni,      19) \
    X(andi,      20) \
    X(ori,       21) \
    X(xori,      22) /* gap: 23 */ \
    X(addi,      24) /* gap: 25 */ \
    X(addi_ws,   26) \
    X(addi_wz,   27) \
    X(subfi,     28) /* gap: 29 */ \
    X(subfi_ws,  30) \
    X(subfi_wz,  31) \
    X(muli,      32) /* gap: 33 */ \
    X(muli_ws,   34) \
    X(muli_wz,   35) \
    X(call_r,    36) \
    X(call_plt,  37) \
    X(dep,       38) \
    X(rlmi,      39) \
    X(divi_d,    40) \
    X(divui_d,   41) \
    X(modi_d,    42) \
    X(modui_d,   43) \
    X(maxsi_d,   44) \
    X(maxui_d,   45) \
    X(minsi_d,   46) \
    X(minui_d,   47) \
    X(ca_r,      48) \
    X(ca_n,      49) \
    X(ca_nrc,    50) /* gap: 51 */ \
    X(divi_w,    52) \
    X(divui_w,   53) \
    X(modi_w,    54) \
    X(modui_w,   55) \
    OPCODE_TABLE_COMPARE_IMM_OPS(X, d, 56) /* 6 insns, gap - 2 */ \
    OPCODE_TABLE_COMPARE_IMM_OPS(X, w, 64) /* 6 insns, gap - 2 */ \
    /* gap: 72-79 */ \
    OPCODE_TABLE_LOADSTORE_OPS(X,   , 80) /* 14 insns */ \
    OPCODE_TABLE_LOADSTORE_OPS(X, _r, 96) /* 14 insns */ \
    X(ca_rf,     110) \
    X(bi_eq_d,   112) \
    X(bi_ne_d,   113) \
    X(bsi_lt_d,  114) \
    X(bui_lt_d,  115) \
    X(bsi_ge_d,  116) \
    X(bui_ge_d,  117) \
    X(bi_eq_w,   118) \
    X(bi_ne_w,   119) \
    X(bsi_lt_w,  120) \
    X(bui_lt_w,  121) \
    X(bsi_ge_w,  122) \
    X(bui_ge_w,  123) \
    X(bm_all,    124) \
    X(bm_notall, 125) \
    X(bm_none,   126) \
    X(bm_any,    127) \

enum EInstructionOpcode {
    DECLARE_PRIMARY_OPCODES(OPCODE_X)
    opcode_last,
    PSEUDO_INSN(bsi_le_d,   bsi_lt_d)
    PSEUDO_INSN(bui_le_d,   bui_lt_d)
    PSEUDO_INSN(bsi_gt_d,   bsi_ge_d)
    PSEUDO_INSN(bui_gt_d,   bui_ge_d)
    PSEUDO_INSN(bsi_le_w,   bsi_lt_w)
    PSEUDO_INSN(bui_le_w,   bui_lt_w)
    PSEUDO_INSN(bsi_gt_w,   bsi_ge_w)
    PSEUDO_INSN(bui_gt_w,   bui_ge_w)
    PSEUDO_INSN(cmpsi_ge_d, cmpsi_gt_d)
    PSEUDO_INSN(cmpui_ge_d, cmpui_gt_d)
    PSEUDO_INSN(cmpsi_le_d, cmpsi_lt_d)
    PSEUDO_INSN(cmpui_le_d, cmpui_lt_d)
    PSEUDO_INSN(cmpsi_ge_w, cmpsi_gt_w)
    PSEUDO_INSN(cmpui_ge_w, cmpui_gt_w)
    PSEUDO_INSN(cmpsi_le_w, cmpsi_lt_w)
    PSEUDO_INSN(cmpui_le_w, cmpui_lt_w)
    PSEUDO_INSN(flddi,    ldi)
    PSEUDO_INSN(fldsi,    ldi)
    PSEUDO_INSN(fldsri,   ldz_w_r)
    PSEUDO_INSN(flddri,   ldz_d_r)
    PSEUDO_INSN(fldqri,   ld_q_r)
    PSEUDO_INSN(not,      orni)
};

inline constexpr u64 insn_tmplt(const EInstructionOpcode opc)
{
    return makepart(static_cast<u64>(opc), disp_opcode);
}

/**************************************************************************************
* opcode_fused extended opcodes (7 bit)
**************************************************************************************/

#define OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, TYPE, N) \
    X(fmerge_ ## TYPE, N + 0) \
    X(fmadd_  ## TYPE, N + 1) \
    X(fmsub_  ## TYPE, N + 2) \
    X(fnmadd_ ## TYPE, N + 3) \
    X(fnmsub_ ## TYPE, N + 4) \

#define OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, TYPE, N) \
    X(fmerge_ ## TYPE, N + 0) \
    X(fmadd_  ## TYPE, N + 1) \
    X(fmsub_  ## TYPE, N + 2) \
    X(fnmadd_ ## TYPE, N + 3) \
    X(fnmsub_ ## TYPE, N + 4) \
    X(fmadda_ ## TYPE, N + 5) \
    X(fmsuba_ ## TYPE, N + 6) \

#define DECLARE_FUSED_EXTENDED_OPCODES(X) \
    X(nullrr,     0) \
    X(nulfss,     1) \
    X(nulfsd,     2) \
    X(nulfsq,     3) /* gap 4-7 */ \
    X(add_add,    8) \
    X(add_sub,    9) \
    X(sub_sub,    10) \
    X(add_addc,   11) \
    X(sub_subb,   12) \
    X(slp_w,      13) \
    X(srp_w,      14) \
    X(srpi_w,     15) \
    X(slp,        16) \
    X(srp,        17) \
    X(srpi,       18) \
    X(slsrl,      19) \
    X(slsra,      20) \
    X(slsrli,     21) \
    X(slsrai,     22) \
    X(sl_add,     23) \
    X(sl_sub,     24) \
    X(sl_subf,    25) \
    X(slsrl_w,    26) \
    X(slsra_w,    27) \
    X(mbsel,      28) \
    X(perm,       29) \
    X(mov2,       30) \
    X(algnup,     31) \
    X(crc32c,     32) \
    X(cmov_lsb,   33) \
    X(mul_add,    34) \
    X(mul_sub,    35) \
    X(mul_subf,   36) \
    X(sl_add_ws,  37) \
    X(sl_add_wz,  38) \
    X(sl_subf_ws, 39) \
    X(sl_subf_wz, 40) \
    X(sl_sub_ws,  41) \
    X(sl_sub_wz,  42) \
    X(dep_q,      43) \
    X(slp_q,      44) \
    X(srp_q,      45) \
    X(srpi_q,     46) \
    X(dep_s,      47) \
    X(dep_c,      48) \
    X(dep_a,      49) \
    X(sl_or,      50) \
    X(sl_xor,     51) \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, sq, 56) /* 5 insns */ \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, sd, 64) /* 5 insns */ \
    OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, pd, 64+5) /* 7 insns */ \
    X(cmov_eq_d,  77) \
    X(cmov_lt_d,  78) \
    X(cmov_le_d,  79) \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, ss, 80) /* 5 insns */ \
    OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, ps, 80+5) /* 7 insns */ \
    X(cmov_eq_w,  93) \
    X(cmov_lt_w,  94) \
    X(cmov_le_w,  95) \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, sh, 96) /* 5 insns */ \
    OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, ph, 96+5) /* 7 insns */ \
    X(nuli_eq_w,  112) \
    X(nuli_ne_w,  113) \
    X(nulsi_lt_w, 114) \
    X(nului_lt_w, 115) \
    X(nulsi_gt_w, 116) \
    X(nului_gt_w, 117) \
    X(nuli_eq_d,  118) \
    X(nuli_ne_d,  119) \
    X(nulsi_lt_d, 120) \
    X(nului_lt_d, 121) \
    X(nulsi_gt_d, 122) \
    X(nului_gt_d, 123) \
    X(nulm_all,   124) \
    X(nulm_notall,125) \
    X(nulm_none,  126) \
    X(nulm_any,   127) \

enum EFusedOpx {
    DECLARE_FUSED_EXTENDED_OPCODES(OPCODE_X)
    fused_opx_last
};

inline constexpr u64 insn_tmplt(const EFusedOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_fused)
         | makepart(static_cast<u64>(opx), disp_fused_opx);
}

/**************************************************************************************
* opcode_nullrr extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_NULLRR_EXTENDED_OPCODES(X) DECLARE_INT_COMPARE_AND_OP(X, nul)
enum ENullrrOpx {
    DECLARE_NULLRR_EXTENDED_OPCODES(OPCODE_X)
    nullrr_opx_last
};

inline constexpr u64 insn_tmplt(const ENullrrOpx opx)
{
    return insn_tmplt(EFusedOpx::opcode_nullrr)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_nulfss extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_NULFSS_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, nulf, ss)
enum nulfss_opx {
    DECLARE_NULFSS_EXTENDED_OPCODES(OPCODE_X)
    nulfss_opx_last,
};

inline constexpr u64 insn_tmplt(const nulfss_opx opx)
{
    return insn_tmplt(EFusedOpx::opcode_nulfss)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_nulfsd extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_NULFSD_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, nulf, sd)
enum nulfsd_opx {
    DECLARE_NULFSD_EXTENDED_OPCODES(OPCODE_X)
    nulfsd_opx_last,
};

inline constexpr u64 insn_tmplt(const nulfsd_opx opx)
{
    return insn_tmplt(EFusedOpx::opcode_nulfsd)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_nulfsq extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_NULFSQ_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, nulf, sq)
enum nulfsq_opx {
    DECLARE_NULFSQ_EXTENDED_OPCODES(OPCODE_X)
    nulfsq_opx_last,
};

inline constexpr u64 insn_tmplt(const nulfsq_opx opx)
{
    return insn_tmplt(EFusedOpx::opcode_nulfsq)
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
    X(write,   127) \

enum ERaopxOpx {
    DECLARE_RAOPX_EXTENDED_OPCODES(OPCODE_X)
    raopx_opx_last
};

inline constexpr u64 insn_tmplt(const ERaopxOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_raopx)
         | makepart(static_cast<u64>(opx), disp_reg_a);
}

/**************************************************************************************
* opcode_brr extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_BRR_EXTENDED_OPCODES(X) DECLARE_INT_COMPARE_AND_OP(X, b)
enum EBrrOpx {
    DECLARE_BRR_EXTENDED_OPCODES(OPCODE_X)
    brr_opx_last,
    PSEUDO_INSN(bs_gt_d, bs_lt_d)
    PSEUDO_INSN(bu_gt_d, bu_lt_d)
    PSEUDO_INSN(bs_ge_d, bs_le_d)
    PSEUDO_INSN(bu_ge_d, bu_le_d)
    PSEUDO_INSN(bs_gt_w, bs_lt_w)
    PSEUDO_INSN(bu_gt_w, bu_lt_w)
    PSEUDO_INSN(bs_ge_w, bs_le_w)
    PSEUDO_INSN(bu_ge_w, bu_le_w)
};

inline constexpr u64 insn_tmplt(const EBrrOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_brr)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_brr extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_LOOP_EXTENDED_OPCODES(X) \
    X(reps_lt_d, 0) \
    X(reps_gt_d, 1) \
    X(reps_le_d, 2) \
    X(reps_ge_d, 3) \
    X(repu_lt_d, 4) \
    X(repu_gt_d, 5) \
    X(repu_le_d, 6) \
    X(repu_ge_d, 7) \

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
* opcode_bfss extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_BFSS_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, bf, ss)
enum bfss_opx {
    DECLARE_BFSS_EXTENDED_OPCODES(OPCODE_X)
    bfss_opx_last,
};

inline constexpr u64 insn_tmplt(const bfss_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_bfss)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_bfsd extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_BFSD_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, bf, sd)
enum bfsd_opx {
    DECLARE_BFSD_EXTENDED_OPCODES(OPCODE_X)
    bfsd_opx_last,
};

inline constexpr u64 insn_tmplt(const bfsd_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_bfsd)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_bfsq extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_BFSQ_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, bf, sq)
enum bfsq_opx {
    DECLARE_BFSQ_EXTENDED_OPCODES(OPCODE_X)
    bfsq_opx_last,
};

inline constexpr u64 insn_tmplt(const bfsq_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_bfsq)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

#define OPCODE_TABLE_INDEXED_OPS(X, TYPE, N) \
    X(ldz_b ## TYPE, N +  0) \
    X(lds_b ## TYPE, N +  1) \
    X(st_b  ## TYPE, N +  2) \
    X(ldz_h ## TYPE, N +  3) \
    X(lds_h ## TYPE, N +  4) \
    X(st_h  ## TYPE, N +  5) \
    X(ldz_w ## TYPE, N +  6) \
    X(lds_w ## TYPE, N +  7) \
    X(st_w  ## TYPE, N +  8) \
    X(ldz_d ## TYPE, N +  9) \
    X(lds_d ## TYPE, N + 10) \
    X(st_d  ## TYPE, N + 11) \
    X(ld_q  ## TYPE, N + 12) \
    X(st_q  ## TYPE, N + 13) \
    X(ca    ## TYPE, N + 14) \

#define DECLARE_INDEXED_DS_EXTENDED_OPCODES(X) OPCODE_TABLE_INDEXED_OPS(X, _xds, 0)
#define DECLARE_INDEXED_WS_EXTENDED_OPCODES(X) OPCODE_TABLE_INDEXED_OPS(X, _xws, 0)
#define DECLARE_INDEXED_WZ_EXTENDED_OPCODES(X) OPCODE_TABLE_INDEXED_OPS(X, _xwz, 0)

enum E_Indx_d_Opx {
    DECLARE_INDEXED_DS_EXTENDED_OPCODES(OPCODE_X)
    indx_d_opx_last,
};

inline constexpr u64 insn_tmplt(const E_Indx_d_Opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_indexed_ds)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

enum E_Indx_ws_Opx {
    DECLARE_INDEXED_WS_EXTENDED_OPCODES(OPCODE_X)
    indx_ws_opx_last,
};

inline constexpr u64 insn_tmplt(const E_Indx_ws_Opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_indexed_ws)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

enum E_Indx_wz_Opx {
    DECLARE_INDEXED_WZ_EXTENDED_OPCODES(OPCODE_X)
    indx_wz_opx_last,
};

inline constexpr u64 insn_tmplt(const E_Indx_wz_Opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_indexed_wz)
         | makepart(static_cast<u64>(opx), disp_indexed_opx);
}

/**************************************************************************************
* opcode_misc extended opcodes (10 bit)
**************************************************************************************/
#define OPCODE_TABLE_ATOMIC_LD(X, TYPE, NAME, N) \
    X(NAME ## _ ## TYPE      , N + 0) \
    X(NAME ## _ ## TYPE ## _a, N + 1)

#define OPCODE_TABLE_ATOMIC_ST(X, TYPE, NAME, N) \
    X(NAME ## _ ## TYPE      , N + 0) \
    X(NAME ## _ ## TYPE ## _r, N + 1)

#define OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, NAME, N) \
    X(NAME ## _ ## TYPE       , N + 0) \
    X(NAME ## _ ## TYPE ## _a , N + 1) \
    X(NAME ## _ ## TYPE ## _r , N + 2) \
    X(NAME ## _ ## TYPE ## _ar, N + 3)

#define OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, NAME, N) \
    X(NAME ## _ ## TYPE      , N + 0) \
    X(NAME ## _ ## TYPE ## _r, N + 1)

#define OPCODE_TABLE_ATOMIC_BASE_OPS(X, TYPE, N) \
    OPCODE_TABLE_ATOMIC_LD(X, TYPE, lda, N + 0) \
    OPCODE_TABLE_ATOMIC_ST(X, TYPE, sta, N + 2) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, swap, N + 4) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, cas,  N + 8) \

#define OPCODE_TABLE_ATOMIC_FULL_OPS(X, TYPE, N) \
    OPCODE_TABLE_ATOMIC_BASE_OPS(X, TYPE, N + 0) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_add , N + 12) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_and,  N + 16) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_or,   N + 20) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_xor,  N + 24) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_smin, N + 28) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_smax, N + 32) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_umin, N + 36) \
    OPCODE_TABLE_ATOMIC_LD_OP(X, TYPE, ld_umax, N + 40) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_add,  N + 44) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_and,  N + 46) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_or,   N + 48) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_xor,  N + 50) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_smin, N + 52) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_smax, N + 54) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_umin, N + 56) \
    OPCODE_TABLE_ATOMIC_ST_OP(X, TYPE, st_umax, N + 58) \

#define OPCODE_TABLE_FP_SCALAR_OPS(X, TYPE, N) \
    X(fcl_      ## TYPE,     N +  0) \
    X(fsqrt_    ## TYPE,     N +  1) \
    X(frsqrt_   ## TYPE,     N +  2) \
    X(fadd_     ## TYPE,     N +  3) \
    X(fsub_     ## TYPE,     N +  4) \
    X(fnadd_    ## TYPE,     N +  5) \
    X(fmul_     ## TYPE,     N +  6) \
    X(fnmul_    ## TYPE,     N +  7) \
    X(fdiv_     ## TYPE,     N +  8) \
    X(fneg_     ## TYPE,     N +  9) \
    X(fabs_     ## TYPE,     N + 10) \
    X(fnabs_    ## TYPE,     N + 11) \
    X(fabsd_    ## TYPE,     N + 12) \
    X(fnabsd_   ## TYPE,     N + 13) \
    X(fmin_     ## TYPE,     N + 14) \
    X(fmax_     ## TYPE,     N + 15) \
    X(fminnm_   ## TYPE,     N + 16) \
    X(fmaxnm_   ## TYPE,     N + 17) \
    X(famin_    ## TYPE,     N + 18) \
    X(famax_    ## TYPE,     N + 19) \
    X(frnd_     ## TYPE,     N + 20) \
    X(fcmpo_eq_ ## TYPE,     N + 21) \
    X(fcmpo_ne_ ## TYPE,     N + 22) \
    X(fcmpo_lt_ ## TYPE,     N + 23) \
    X(fcmpo_le_ ## TYPE,     N + 24) \
    X(fcmpo_    ## TYPE,     N + 25) \
    X(fcmpu_eq_ ## TYPE,     N + 26) \
    X(fcmpu_ne_ ## TYPE,     N + 27) \
    X(fcmpu_lt_ ## TYPE,     N + 28) \
    X(fcmpu_le_ ## TYPE,     N + 29) \
    X(fcmpu_    ## TYPE,     N + 30) \
    X(fcvt_     ## TYPE ## _iw, N + 31) \
    X(fcvt_     ## TYPE ## _uw, N + 32) \
    X(fcvt_iw_  ## TYPE,        N + 33) \
    X(fcvt_uw_  ## TYPE,        N + 34) \
    X(fcvt_     ## TYPE ## _id, N + 35) \
    X(fcvt_     ## TYPE ## _ud, N + 36) \
    X(fcvt_id_  ## TYPE,        N + 37) \
    X(fcvt_ud_  ## TYPE,        N + 38) \
    X(fcvt_     ## TYPE ## _iq, N + 39) \
    X(fcvt_     ## TYPE ## _uq, N + 40) \
    X(fcvt_iq_  ## TYPE,        N + 41) \
    X(fcvt_uq_  ## TYPE,        N + 42) \

#define OPCODE_TABLE_FP_PACKED_OPS(X, TYPE, ITYPE, N) \
    X(fneg_     ## TYPE,        N +  0) \
    X(fabs_     ## TYPE,        N +  1) \
    X(fnabs_    ## TYPE,        N +  2) \
    X(fabsd_    ## TYPE,        N +  3) \
    X(fnabsd_   ## TYPE,        N +  4) \
    X(frsqrt_   ## TYPE,        N +  5) \
    X(fsqrt_    ## TYPE,        N +  6) \
    X(fadd_     ## TYPE,        N +  7) \
    X(fsub_     ## TYPE,        N +  8) \
    X(fnadd_    ## TYPE,        N +  9) \
    X(fmul_     ## TYPE,        N + 10) \
    X(fnmul_    ## TYPE,        N + 11) \
    X(fdiv_     ## TYPE,        N + 12) \
    X(fmin_     ## TYPE,        N + 13) \
    X(fmax_     ## TYPE,        N + 14) \
    X(famin_    ## TYPE,        N + 15) \
    X(famax_    ## TYPE,        N + 16) \
    X(frnd_     ## TYPE,        N + 17) \
    X(fcmpo_eq_ ## TYPE,        N + 18) \
    X(fcmpo_ne_ ## TYPE,        N + 19) \
    X(fcmpo_lt_ ## TYPE,        N + 20) \
    X(fcmpo_le_ ## TYPE,        N + 21) \
    X(fcmpo_    ## TYPE,        N + 22) \
    X(fcmpu_eq_ ## TYPE,        N + 23) \
    X(fcmpu_ne_ ## TYPE,        N + 24) \
    X(fcmpu_lt_ ## TYPE,        N + 25) \
    X(fcmpu_le_ ## TYPE,        N + 26) \
    X(fcmpu_    ## TYPE,        N + 27) \
    X(faddc_    ## TYPE,        N + 28) \
    X(fsubc_    ## TYPE,        N + 29) \
    X(faddh_    ## TYPE,        N + 30) \
    X(fsubh_    ## TYPE,        N + 31) \
    X(fmulh_    ## TYPE,        N + 32) \
    X(fdot_     ## TYPE,        N + 33) \
    X(fmrgl_    ## TYPE,        N + 34) \
    X(fmrgh_    ## TYPE,        N + 35) \
    X(funpkh_   ## TYPE,        N + 36) \
    X(funpkl_   ## TYPE,        N + 37) \
    X(fpk_      ## TYPE,        N + 38) \
    X(fcvt_     ## TYPE ## _i ## ITYPE, N + 39) \
    X(fcvt_     ## TYPE ## _u ## ITYPE, N + 40) \
    X(fcvt_i    ## ITYPE ## _ ## TYPE,  N + 41) \
    X(fcvt_u    ## ITYPE ## _ ## TYPE,  N + 42) \

#define OPCODE_TABLE_MMX_OPS(X, TYPE, N) \
    X(vmaxs_    ## TYPE, N +  0) \
    X(vmaxu_    ## TYPE, N +  1) \
    X(vmins_    ## TYPE, N +  2) \
    X(vminu_    ## TYPE, N +  3) \
    X(vaddu_    ## TYPE, N +  4) \
    X(vsubu_    ## TYPE, N +  5) \
    X(vaddo_    ## TYPE, N +  6) \
    X(vsubo_    ## TYPE, N +  7) \
    X(vaddc_    ## TYPE, N +  8) \
    X(vsubb_    ## TYPE, N +  9) \
    X(vaddus_   ## TYPE, N + 10) \
    X(vaddss_   ## TYPE, N + 11) \
    X(vsubss_   ## TYPE, N + 12) \
    X(vsubus_   ## TYPE, N + 13) \
    X(vavgs_    ## TYPE, N + 14) \
    X(vavgu_    ## TYPE, N + 15) \
    X(vcmp_eq_  ## TYPE, N + 16) \
    X(vcmp_lt_  ## TYPE, N + 17) \
    X(vcmp_ltu_ ## TYPE, N + 18) \
    X(vsll_     ## TYPE, N + 19) \
    X(vslli_    ## TYPE, N + 20) \
    X(vsrl_     ## TYPE, N + 21) \
    X(vsrli_    ## TYPE, N + 22) \
    X(vsra_     ## TYPE, N + 23) \
    X(vsrai_    ## TYPE, N + 24) \
    X(vrol_     ## TYPE, N + 25) \
    X(vror_     ## TYPE, N + 26) \
    X(vmrgh_    ## TYPE, N + 27) \
    X(vmrgl_    ## TYPE, N + 28) \

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
    X(undef,     0) \
    X(mov,       1) \
    X(add_ws,    2) \
    X(sub_ws,    3) \
    X(mul_ws,    4) \
    X(mul_wz,    5) \
    X(add_wz,    6) \
    X(sub_wz,    7) /* gap 46-47 */ \
    DECLARE_INT_COMPARES(X, cmp, d, 8) /* 6 ops */ \
    DECLARE_INT_COMPARES(X, cmp, w, 16) /* 6 ops */ \
    X(and,       24) \
    X(or,        25) \
    X(xor,       26) \
    X(andn,      27) \
    X(nand,      28) \
    X(nor,       29) \
    X(xnor,      30) \
    X(orn,       31) \
    X(add,       32) \
    X(sub,       33) \
    X(addo,      34) \
    X(subo,      35) \
    X(addc,      36) \
    X(subb,      37) \
    X(mul,       38) \
    X(mulh,      39) \
    X(div_d,     40) \
    X(divu_d,    41) \
    X(div_w,     42) \
    X(divu_w,    43) \
    X(mod_d,     44) \
    X(modu_d,    45) \
    X(mod_w,     46) \
    X(modu_w,    47) \
    X(maxs_d,    48) \
    X(maxu_d,    49) \
    X(mins_d,    50) \
    X(minu_d,    51) \
    X(maxs_w,    52) \
    X(maxu_w,    53) \
    X(mins_w,    54) \
    X(minu_w,    55) \
    X(sll,       56) \
    X(sra,       57) \
    X(srl,       58) \
    X(srd,       59) \
    X(slli,      60) \
    X(srai,      61) \
    X(srli,      62) \
    X(srdi,      63) \
    X(sll_w,     64) \
    X(sra_w,     65) \
    X(srl_w,     66) \
    X(slli_w,    67) \
    X(srai_w,    68) \
    X(srli_w,    69) \
    X(absd,      70) \
    X(absd_w,    71) \
    X(and_neg,   72) \
    X(xor_dec,   73) \
    X(and_dec,   74) \
    X(mul_q,     75) /* gap 76-79 */ \
    X(jmp_r,     80) \
    X(jmp_t,     81) \
    X(jmp_t_ws,  82) \
    X(jmp_t_wz,  83) \
    X(call_rvt,  84) \
    X(call_ri,   85) \
    X(call_mi,   86) \
    X(call_mrw,  87) \
    X(ret,       88) \
    X(mbgath,    89) \
    X(mbscat,    90) \
    X(cnt_pop,   91) \
    X(cnt_lz,    92) \
    X(cnt_tz,    93) \
    X(permb,     94) \
    X(gtb,       95) \
    X(add_q,     96) \
    X(sub_q,     97) \
    DECLARE_INT_COMPARES(X, cmp, q, 98) /* 6 ops */ \
    OPCODE_TABLE_LOADSTORE_OPS(X, _mia, 136) \
    OPCODE_TABLE_LOADSTORE_OPS(X, _mib, 136 + 16) \
    X(mprobe,          SPECIAL_BASE +  0) \
    X(cpuid,           SPECIAL_BASE +  1) \
    X(int,             SPECIAL_BASE +  2) \
    X(syscall,         SPECIAL_BASE +  3) \
    X(random,          SPECIAL_BASE +  4) \
    X(mfspr,           SPECIAL_BASE +  5) \
    X(mtspr,           SPECIAL_BASE +  6) \
    X(mtdbr,           SPECIAL_BASE +  7) \
    X(mfdbr,           SPECIAL_BASE +  8) \
    X(mtibr,           SPECIAL_BASE +  9) \
    X(mfibr,           SPECIAL_BASE + 10) \
    X(mtmr,            SPECIAL_BASE + 11) \
    X(mfmr,            SPECIAL_BASE + 12) \
    X(mtitr,           SPECIAL_BASE + 13) \
    X(mtdtr,           SPECIAL_BASE + 14) \
    X(rfi,             SPECIAL_BASE + 15) \
    X(halt,            SPECIAL_BASE + 16) \
    X(tpa,             SPECIAL_BASE + 17) \
    X(ptc,             SPECIAL_BASE + 18) \
    X(sysret,          SPECIAL_BASE + 22) \
    X(rscover,         SPECIAL_BASE + 24) \
    X(rsflush,         SPECIAL_BASE + 25) \
    X(rsload,          SPECIAL_BASE + 26) \
    X(clmulll,         SPECIAL_BASE + 32) \
    X(clmulhl,         SPECIAL_BASE + 33) \
    X(clmulhh,         SPECIAL_BASE + 34) \
    X(aes_enc,           SPECIAL_BASE + 40) \
    X(aes_enc_last,      SPECIAL_BASE + 41) \
    X(aes_dec,           SPECIAL_BASE + 42) \
    X(aes_dec_last,      SPECIAL_BASE + 43) \
    X(aes_imc,           SPECIAL_BASE + 44) \
    X(aes_keygen_assist, SPECIAL_BASE + 45) \
    X(slli_q,          SPECIAL_BASE + 48) \
    X(srli_q,          SPECIAL_BASE + 49) \
    X(srai_q,          SPECIAL_BASE + 50) \
    X(jmp_mi,          SPECIAL_BASE + 51) \
    X(fence_acq,       SPECIAL_BASE + 52) \
    X(fence_rel,       SPECIAL_BASE + 53) \
    X(fence_acq_rel,   SPECIAL_BASE + 54) \
    X(fence_seq_cst,   SPECIAL_BASE + 55) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, b, ATOMICS_BASE + 80 * 0) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, h, ATOMICS_BASE + 80 * 1) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, w, ATOMICS_BASE + 80 * 2) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, d, ATOMICS_BASE + 80 * 3) \
    OPCODE_TABLE_ATOMIC_FULL_OPS(X, q, ATOMICS_BASE + 80 * 4) \
    /* fpu */ \
    OPCODE_TABLE_FP_SCALAR_OPS(X, ss,    FPU32_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, ps, w, FPU32_BASE + 64) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, sd,    FPU64_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, pd, d, FPU64_BASE + 64) \
    X(fcvt_sd_ss, FPU64_BASE + 56) \
    X(fext_ss_sd, FPU64_BASE + 57) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, sq,    FPU128_BASE) \
    X(fscale,     FPU128_BASE + 56) \
    X(fext_ss_sq, FPU128_BASE + 57) \
    X(fext_sd_sq, FPU128_BASE + 58) \
    X(fcvt_sq_sd, FPU128_BASE + 59) \
    X(fcvt_sq_ss, FPU128_BASE + 60) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, sh,    FPU16_BASE) \
    X(fext_sh_ss, FPU16_BASE + 56) \
    X(fext_sh_sd, FPU16_BASE + 57) \
    X(fext_sh_sq, FPU16_BASE + 58) \
    X(fcvt_ss_sh, FPU16_BASE + 59) \
    X(fcvt_sd_sh, FPU16_BASE + 60) \
    X(fcvt_sq_sh, FPU16_BASE + 61) \
    OPCODE_TABLE_FP_PACKED_OPS(X, ph, h, FPU16_BASE + 64) \
    /* mmx */ \
    OPCODE_TABLE_MMX_OPS(X, b, MMXB_BASE) \
    X(vupkls_b, MMXB_BASE + 32) \
    X(vupkhs_b, MMXB_BASE + 33) \
    OPCODE_TABLE_MMX_OPS(X, h, MMXH_BASE) \
    X(vupkls_h, MMXH_BASE + 32) \
    X(vupkhs_h, MMXH_BASE + 33) \
    X(vpksss_h, MMXH_BASE + 34) \
    X(vpksus_h, MMXH_BASE + 35) \
    X(vpkuum_h, MMXH_BASE + 36) \
    X(vpkuus_h, MMXH_BASE + 37) \
    OPCODE_TABLE_MMX_OPS(X, w, MMXW_BASE) \
    X(vupkls_w, MMXW_BASE + 32) \
    X(vupkhs_w, MMXW_BASE + 33) \
    X(vpksss_w, MMXW_BASE + 34) \
    X(vpksus_w, MMXW_BASE + 35) \
    X(vpkuum_w, MMXW_BASE + 36) \
    X(vpkuus_w, MMXW_BASE + 37) \
    OPCODE_TABLE_MMX_OPS(X, d, MMXD_BASE) \
    X(vpksss_d, MMXD_BASE + 33) \
    X(vpksus_d, MMXD_BASE + 34) \
    X(vpkuum_d, MMXD_BASE + 36) \
    X(vpkuus_d, MMXD_BASE + 37) \

enum EMiscOpx {
    DECLARE_MISC_EXTENDED_OPCODES(OPCODE_X)
    misc_opx_last,
    PSEUDO_INSN(cmps_gt_w,  cmps_lt_w)
    PSEUDO_INSN(cmpu_gt_w,  cmpu_lt_w)
    PSEUDO_INSN(cmps_ge_w,  cmps_le_w)
    PSEUDO_INSN(cmpu_ge_w,  cmpu_le_w)
    PSEUDO_INSN(cmps_gt_d,  cmps_lt_d)
    PSEUDO_INSN(cmpu_gt_d,  cmpu_lt_d)
    PSEUDO_INSN(cmps_ge_d,  cmps_le_d)
    PSEUDO_INSN(cmpu_ge_d,  cmpu_le_d)
    PSEUDO_INSN(cmps_gt_q,  cmps_lt_q)
    PSEUDO_INSN(cmpu_gt_q,  cmpu_lt_q)
    PSEUDO_INSN(cmps_ge_q,  cmps_le_q)
    PSEUDO_INSN(cmpu_ge_q,  cmpu_le_q)
};

inline constexpr u64 insn_tmplt(const EMiscOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_misc)
         | makepart(static_cast<u64>(opx), disp_misc_opx);
}

#define INSN_TMPLT(opc) insn_tmplt(opcode_##opc)

} // namespace postrisc
