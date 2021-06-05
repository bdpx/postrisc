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
    X(OP ## TYPE ## eq,   N + 0) \
    X(OP ## TYPE ## ne,   N + 1) \
    X(OP ## TYPE ## lt,   N + 2) \
    X(OP ## TYPE ## le,   N + 3) \
    X(OP ## TYPE ## ltu,  N + 4) \
    X(OP ## TYPE ## leu,  N + 5) \

// int compare reg-reg and branch or nullify ops
#define DECLARE_INT_COMPARE_AND_OP(X, OP) \
    DECLARE_INT_COMPARES(X, OP, d, 0) /* 6 ops */ \
    DECLARE_INT_COMPARES(X, OP, w, 6) /* 6 ops */ \
    X(OP ## bc,   12) \
    X(OP ## bci,  13) \
    X(OP ## bs,   14) \
    X(OP ## bsi,  15) \

// fp branch or nullify ops
#define DECLARE_FP_SCALAR_COMPARE_AND_OP(X, OP, TYPE) \
    X(OP ## fs ## TYPE ## oeq,    0) \
    X(OP ## fs ## TYPE ## one,    1) \
    X(OP ## fs ## TYPE ## olt,    2) \
    X(OP ## fs ## TYPE ## ole,    3) \
    X(OP ## fs ## TYPE ## ueq,    4) \
    X(OP ## fs ## TYPE ## une,    5) \
    X(OP ## fs ## TYPE ## ult,    6) \
    X(OP ## fs ## TYPE ## ule,    7) \
    X(OP ## fs ## TYPE ## o,      8) \
    X(OP ## fs ## TYPE ## uo,     9) \
    X(OP ## fs ## TYPE ## class, 10) \

/***************************************************************************
*
* main opcode - 7 bit
*
***************************************************************************/
#define OPCODE_TABLE_COMPARE_IMM_OPS(X, TYPE, N) \
    X(cmp ## TYPE ## eqi,  N + 0) \
    X(cmp ## TYPE ## nei,  N + 1) \
    X(cmp ## TYPE ## lti,  N + 2) \
    X(cmp ## TYPE ## ltui, N + 3) \
    X(cmp ## TYPE ## gti,  N + 4) \
    X(cmp ## TYPE ## gtui, N + 5) \

#define OPCODE_TABLE_LOADSTORE_OPS(X, TYPE, N) \
    X(ldbz ## TYPE, N +  0) \
    X(ldbs ## TYPE, N +  1) \
    X(stb  ## TYPE, N +  2) \
    X(ldhz ## TYPE, N +  3) \
    X(ldhs ## TYPE, N +  4) \
    X(sth  ## TYPE, N +  5) \
    X(ldwz ## TYPE, N +  6) \
    X(ldws ## TYPE, N +  7) \
    X(stw  ## TYPE, N +  8) \
    X(lddz ## TYPE, N +  9) \
    X(ldds ## TYPE, N + 10) \
    X(std  ## TYPE, N + 11) \
    X(ldq  ## TYPE, N + 12) \
    X(stq  ## TYPE, N + 13) \


#define DECLARE_PRIMARY_OPCODES(X) \
    X(misc,       0) \
    X(fused,      1) \
    X(raopx,      2) \
    X(brr,        3) \
    X(loop,       4) \
    X(bfss,       5) \
    X(bfsd,       6) \
    X(bfsq,       7) \
    X(ldi,        8) \
    X(andni,      9) \
    X(orni,      10) \
    X(andi,      11) \
    X(ori,       12) \
    X(xori,      13) \
    X(mulwsi,    14) \
    X(mulwzi,    15) \
    X(addi,      16) \
    X(addiws,    17) \
    X(addiwz,    18) \
    X(subfi,     19) \
    X(subfiws,   20) \
    X(subfiwz,   21) \
    X(ldih,      22) \
    X(muli,      23) \
    X(divi,      24) \
    X(divui,     25) \
    X(modi,      26) \
    X(modui,     27) \
    X(maxsi,     28) \
    X(maxui,     29) \
    X(minsi,     30) \
    X(minui,     31) \
    OPCODE_TABLE_COMPARE_IMM_OPS(X, d, 32) /* 6 insns, gap - 2 */ \
    OPCODE_TABLE_COMPARE_IMM_OPS(X, w, 40) /* 6 insns, gap - 2 */ \
    X(ldar,      48) \
    X(ldax,      49) \
    X(ldan,      50) \
    X(dep,       51) \
    X(rlmi,      52) \
    X(ldanrc,    53) /* gap: 54-55 */ \
    X(callr,     56) \
    X(ldafr,     57) \
    X(callplt,   58) /* gap: 59-63 */ \
    OPCODE_TABLE_LOADSTORE_OPS(X,  , 64) /* 14 insns */ \
    OPCODE_TABLE_LOADSTORE_OPS(X, x, 80) /* 14 insns */ \
    OPCODE_TABLE_LOADSTORE_OPS(X, r, 96) /* 14 insns */ \
    X(bdeqi,    112) \
    X(bdnei,    113) \
    X(bdlti,    114) \
    X(bdltui,   115) \
    X(bdgei,    116) \
    X(bdgeui,   117) \
    X(bweqi,    118) \
    X(bwnei,    119) \
    X(bwlti,    120) \
    X(bwltui,   121) \
    X(bwgei,    122) \
    X(bwgeui,   123) \
    X(bmall,    124) \
    X(bmnotall, 125) \
    X(bmnone,   126) \
    X(bmany,    127) \

enum EInstructionOpcode {
    DECLARE_PRIMARY_OPCODES(OPCODE_X)
    opcode_last,
    PSEUDO_INSN(bdlei,    bdlti)
    PSEUDO_INSN(bdleui,   bdltui)
    PSEUDO_INSN(bdgti,    bdgei)
    PSEUDO_INSN(bdgtui,   bdgeui)
    PSEUDO_INSN(bwlei,    bwlti)
    PSEUDO_INSN(bwleui,   bwltui)
    PSEUDO_INSN(bwgti,    bwgei)
    PSEUDO_INSN(bwgtui,   bwgeui)
    PSEUDO_INSN(cmpdgei,  cmpdgti)
    PSEUDO_INSN(cmpdgeui, cmpdgtui)
    PSEUDO_INSN(cmpdlei,  cmpdlti)
    PSEUDO_INSN(cmpdleui, cmpdltui)
    PSEUDO_INSN(cmpwgei,  cmpwgti)
    PSEUDO_INSN(cmpwgeui, cmpwgtui)
    PSEUDO_INSN(cmpwlei,  cmpwlti)
    PSEUDO_INSN(cmpwleui, cmpwltui)
    PSEUDO_INSN(flddi,    ldi)
    PSEUDO_INSN(fldsi,    ldi)
    PSEUDO_INSN(fldsri,   ldwzr)
    PSEUDO_INSN(flddri,   lddzr)
    PSEUDO_INSN(fldqri,   ldqr)
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
    X(fmerge ## TYPE, N + 0) \
    X(fmadd  ## TYPE, N + 1) \
    X(fmsub  ## TYPE, N + 2) \
    X(fnmadd ## TYPE, N + 3) \
    X(fnmsub ## TYPE, N + 4) \

#define OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, TYPE, N) \
    X(fmerge ## TYPE, N + 0) \
    X(fmadd  ## TYPE, N + 1) \
    X(fmsub  ## TYPE, N + 2) \
    X(fnmadd ## TYPE, N + 3) \
    X(fnmsub ## TYPE, N + 4) \
    X(fmadda ## TYPE, N + 5) \
    X(fmsuba ## TYPE, N + 6) \

#define OPCODE_TABLE_FUSED_NULLIFY_OPS(X, TYPE, N) \
    X(nul ## TYPE ## eqi,  N + 0) \
    X(nul ## TYPE ## nei,  N + 1) \
    X(nul ## TYPE ## lti,  N + 2) \
    X(nul ## TYPE ## ltui, N + 3) \
    X(nul ## TYPE ## gti,  N + 4) \
    X(nul ## TYPE ## gtui, N + 5) \

#define DECLARE_FUSED_EXTENDED_OPCODES(X) \
    X(addadd,      0) \
    X(addsub,      1) \
    X(subsub,      2) \
    X(addaddc,     3) \
    X(subsubb,     4) \
    X(slpw,        5) \
    X(srpw,        6) \
    X(srpiw,       7) \
    X(slp,         8) \
    X(srp,         9) \
    X(srpi,       10) \
    X(slsrl,      11) \
    X(slsra,      12) \
    X(slsrli,     13) \
    X(slsrai,     14) \
    X(srpq,       15) \
    X(sladd,      16) \
    X(slsub,      17) \
    X(slsubf,     18) \
    X(deps,       19) \
    X(depc,       20) \
    X(depa,       21) \
    X(slsrlw,     22) \
    X(slsraw,     23) \
    X(slpq,       24) \
    X(mbsel,      25) \
    X(depq,       26) \
    X(perm,       27) \
    X(srpiq,      28) \
    X(cmovlsb,    29) \
    X(mov2,       30) \
    X(algnup,     31) \
    X(crc32c,     33) \
    X(cmovweq,    34) \
    X(cmovwlt,    35) \
    X(cmovwle,    36) \
    X(cmovdeq,    37) \
    X(cmovdlt,    38) \
    X(cmovdle,    39) \
    X(nullrr,     40) \
    X(nulfss,     41) \
    X(nulfsd,     42) \
    X(nulfsq,     43) \
    X(nulmall,    44) \
    X(nulmnotall, 45) \
    X(nulmnone,   46) \
    X(nulmany,    47) \
    X(muladd,     48) \
    X(mulsub,     49) \
    X(mulsubf,    50) \
    X(sladdws,    51) \
    X(sladdwz,    52) \
    X(slsubws,    53) \
    X(slsubwz,    54) \
    X(slor,       55) \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, sh, 56) /* 5 insns */ \
    X(slxor,      61) \
    X(slsubfws,   62) \
    X(slsubfwz,   63) \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, ss, 64) /* 5 insns */ \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, sd, 72) /* 5 insns */ \
    OPCODE_TABLE_FP_FUSED_SCALAR_OPS(X, sq, 80) /* 5 insns */ \
    OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, ps, 88) /* 7 insns */ \
    OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, pd, 96) /* 7 insns */ \
    OPCODE_TABLE_FUSED_NULLIFY_OPS(X, w, 104) /* 6 insns */ \
    OPCODE_TABLE_FUSED_NULLIFY_OPS(X, d, 112) /* 6 insns */ \
    OPCODE_TABLE_FP_FUSED_PACKED_OPS(X, ph, 120) /* 7 insns */ \

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
#define DECLARE_NULFSS_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, nul, s)
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
#define DECLARE_NULFSD_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, nul, d)
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
#define DECLARE_NULFSQ_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, nul, q)
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
    X(allocsp,   4) \
    X(ehadj,     8) \
    X(ehthrow,   9) \
    X(ehcatch,  10) \
    X(ehnext,   11) \
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
    PSEUDO_INSN(bdgt,  bdlt)
    PSEUDO_INSN(bdgtu, bdltu)
    PSEUDO_INSN(bdge,  bdle)
    PSEUDO_INSN(bdgeu, bdleu)
    PSEUDO_INSN(bwgt,  bwlt)
    PSEUDO_INSN(bwgtu, bwltu)
    PSEUDO_INSN(bwge,  bwle)
    PSEUDO_INSN(bwgeu, bwleu)
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
    X(repdlt,  0) \
    X(repdgt,  1) \
    X(repdle,  2) \
    X(repdge,  3) \
    X(repdltu, 4) \
    X(repdgtu, 5) \
    X(repdleu, 6) \
    X(repdgeu, 7) \

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
#define DECLARE_BFSS_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, b, s)
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
#define DECLARE_BFSD_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, b, d)
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
#define DECLARE_BFSQ_EXTENDED_OPCODES(X) DECLARE_FP_SCALAR_COMPARE_AND_OP(X, b, q)
enum bfsq_opx {
    DECLARE_BFSQ_EXTENDED_OPCODES(OPCODE_X)
    bfsq_opx_last,
};

inline constexpr u64 insn_tmplt(const bfsq_opx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_bfsq)
         | makepart(static_cast<u64>(opx), disp_branch_opx);
}

/**************************************************************************************
* opcode_misc extended opcodes (10 bit)
**************************************************************************************/
#define OPCODE_TABLE_ATOMIC_SHORT_OPS(X, TYPE, N) \
    X(lda    ## TYPE, N +  0) \
    X(sta    ## TYPE, N +  1) \
    X(swap   ## TYPE, N +  2) \
    X(cas    ## TYPE, N +  3) \

#define OPCODE_TABLE_ATOMIC_OPS(X, TYPE, N) \
    X(lda    ## TYPE, N +  0) \
    X(sta    ## TYPE, N +  1) \
    X(swap   ## TYPE, N +  2) \
    X(cas    ## TYPE, N +  3) \
    X(ldadd  ## TYPE, N +  4) \
    X(ldand  ## TYPE, N +  5) \
    X(ldor   ## TYPE, N +  6) \
    X(ldxor  ## TYPE, N +  7) \
    X(ldsmin ## TYPE, N +  8) \
    X(ldsmax ## TYPE, N +  9) \
    X(ldumin ## TYPE, N + 10) \
    X(ldumax ## TYPE, N + 11) \
    X(stadd  ## TYPE, N + 12) \
    X(stand  ## TYPE, N + 13) \
    X(stor   ## TYPE, N + 14) \
    X(stxor  ## TYPE, N + 15) \
    X(stsmin ## TYPE, N + 16) \
    X(stsmax ## TYPE, N + 17) \
    X(stumin ## TYPE, N + 18) \
    X(stumax ## TYPE, N + 19) \

#define OPCODE_TABLE_FP_SCALAR_OPS(X, TYPE, N) \
    X(fcl     ## TYPE,            N +  0) \
    X(fsqrt   ## TYPE,            N +  1) \
    X(frsqrt  ## TYPE,            N +  2) \
    X(fadd    ## TYPE,            N +  3) \
    X(fsub    ## TYPE,            N +  4) \
    X(fnadd   ## TYPE,            N +  5) \
    X(fmul    ## TYPE,            N +  6) \
    X(fnmul   ## TYPE,            N +  7) \
    X(fdiv    ## TYPE,            N +  8) \
    X(fneg    ## TYPE,            N +  9) \
    X(fabs    ## TYPE,            N + 10) \
    X(fnabs   ## TYPE,            N + 11) \
    X(fabsd   ## TYPE,            N + 12) \
    X(fnabsd  ## TYPE,            N + 13) \
    X(fmin    ## TYPE,            N + 14) \
    X(fmax    ## TYPE,            N + 15) \
    X(fminnm  ## TYPE,            N + 16) \
    X(fmaxnm  ## TYPE,            N + 17) \
    X(famin   ## TYPE,            N + 18) \
    X(famax   ## TYPE,            N + 19) \
    X(frnd    ## TYPE,            N + 20) \
    X(fcmp    ## TYPE ## oeq,     N + 21) \
    X(fcmp    ## TYPE ## one,     N + 22) \
    X(fcmp    ## TYPE ## olt,     N + 23) \
    X(fcmp    ## TYPE ## ole,     N + 24) \
    X(fcmp    ## TYPE ## o  ,     N + 25) \
    X(fcmp    ## TYPE ## ueq,     N + 26) \
    X(fcmp    ## TYPE ## une,     N + 27) \
    X(fcmp    ## TYPE ## ult,     N + 28) \
    X(fcmp    ## TYPE ## ule,     N + 29) \
    X(fcmp    ## TYPE ## uo ,     N + 30) \
    X(fcvt    ## TYPE ## 2iw,     N + 31) \
    X(fcvt    ## TYPE ## 2uw,     N + 32) \
    X(fcvtiw2 ## TYPE,            N + 33) \
    X(fcvtuw2 ## TYPE,            N + 34) \
    X(fcvt    ## TYPE ## 2id,     N + 35) \
    X(fcvt    ## TYPE ## 2ud,     N + 36) \
    X(fcvtid2 ## TYPE,            N + 37) \
    X(fcvtud2 ## TYPE,            N + 38) \
    X(fcvt    ## TYPE ## 2iq,     N + 39) \
    X(fcvt    ## TYPE ## 2uq,     N + 40) \
    X(fcvtiq2 ## TYPE,            N + 41) \
    X(fcvtuq2 ## TYPE,            N + 42) \

#define OPCODE_TABLE_FP_PACKED_OPS(X, TYPE, ITYPE, N) \
    X(fneg    ## TYPE,                N +  0) \
    X(fabs    ## TYPE,                N +  1) \
    X(fnabs   ## TYPE,                N +  2) \
    X(fabsd   ## TYPE,                N +  3) \
    X(fnabsd  ## TYPE,                N +  4) \
    X(frsqrt  ## TYPE,                N +  5) \
    X(fsqrt   ## TYPE,                N +  6) \
    X(fadd    ## TYPE,                N +  7) \
    X(fsub    ## TYPE,                N +  8) \
    X(fnadd   ## TYPE,                N +  9) \
    X(fmul    ## TYPE,                N + 10) \
    X(fnmul   ## TYPE,                N + 11) \
    X(fdiv    ## TYPE,                N + 12) \
    X(fmin    ## TYPE,                N + 13) \
    X(fmax    ## TYPE,                N + 14) \
    X(famin   ## TYPE,                N + 15) \
    X(famax   ## TYPE,                N + 16) \
    X(frnd    ## TYPE,                N + 17) \
    X(fcmp    ## TYPE ## oeq,         N + 18) \
    X(fcmp    ## TYPE ## one,         N + 19) \
    X(fcmp    ## TYPE ## olt,         N + 20) \
    X(fcmp    ## TYPE ## ole,         N + 21) \
    X(fcmp    ## TYPE ## o  ,         N + 22) \
    X(fcmp    ## TYPE ## ueq,         N + 23) \
    X(fcmp    ## TYPE ## une,         N + 24) \
    X(fcmp    ## TYPE ## ult,         N + 25) \
    X(fcmp    ## TYPE ## ule,         N + 26) \
    X(fcmp    ## TYPE ## uo ,         N + 27) \
    X(faddc   ## TYPE,                N + 28) \
    X(fsubc   ## TYPE,                N + 29) \
    X(faddh   ## TYPE,                N + 30) \
    X(fsubh   ## TYPE,                N + 31) \
    X(fmulh   ## TYPE,                N + 32) \
    X(fdot    ## TYPE,                N + 33) \
    X(fmrgl   ## TYPE,                N + 34) \
    X(fmrgh   ## TYPE,                N + 35) \
    X(funpkh  ## TYPE,                N + 36) \
    X(funpkl  ## TYPE,                N + 37) \
    X(fpk     ## TYPE,                N + 38) \
    X(fcvt    ## TYPE ## 2i ## ITYPE, N + 39) \
    X(fcvt    ## TYPE ## 2u ## ITYPE, N + 40) \
    X(fcvti   ## ITYPE ## 2 ## TYPE,  N + 41) \
    X(fcvtu   ## ITYPE ## 2 ## TYPE,  N + 42) \

#define OPCODE_TABLE_MMX_OPS(X, TYPE, N) \
    X(vmaxs   ## TYPE, N +  0) \
    X(vmaxu   ## TYPE, N +  1) \
    X(vmins   ## TYPE, N +  2) \
    X(vminu   ## TYPE, N +  3) \
    X(vaddu   ## TYPE, N +  4) \
    X(vsubu   ## TYPE, N +  5) \
    X(vaddo   ## TYPE, N +  6) \
    X(vsubo   ## TYPE, N +  7) \
    X(vaddc   ## TYPE, N +  8) \
    X(vsubb   ## TYPE, N +  9) \
    X(vaddus  ## TYPE, N + 10) \
    X(vaddss  ## TYPE, N + 11) \
    X(vsubss  ## TYPE, N + 12) \
    X(vsubus  ## TYPE, N + 13) \
    X(vavgs   ## TYPE, N + 14) \
    X(vavgu   ## TYPE, N + 15) \
    X(vcmpeq  ## TYPE, N + 16) \
    X(vcmplt  ## TYPE, N + 17) \
    X(vcmpltu ## TYPE, N + 18) \
    X(vsll    ## TYPE, N + 19) \
    X(vslli   ## TYPE, N + 20) \
    X(vsrl    ## TYPE, N + 21) \
    X(vsrli   ## TYPE, N + 22) \
    X(vsra    ## TYPE, N + 23) \
    X(vsrai   ## TYPE, N + 24) \
    X(vrol    ## TYPE, N + 25) \
    X(vror    ## TYPE, N + 26) \
    X(vmrgh   ## TYPE, N + 27) \
    X(vmrgl   ## TYPE, N + 28) \

enum {
     SPECIAL_BASE  = 256,
     ATOMICS_BASE  = 512,
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
    X(undef,      0) \
    X(mov,        1) \
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
    X(addws,     36) \
    X(subws,     37) \
    X(addc,      38) \
    X(subb,      39) \
    X(mul,       40) \
    X(mulh,      41) \
    X(mulws,     42) \
    X(mulwz,     43) \
    X(addwz,     44) \
    X(subwz,     45) \
    X(div,       48) \
    X(divu,      49) \
    X(divw,      50) \
    X(divuw,     51) \
    X(mod,       52) \
    X(modu,      53) \
    X(modw,      54) \
    X(moduw,     55) \
    X(maxs,      56) \
    X(maxu,      57) \
    X(mins,      58) \
    X(minu,      59) \
    X(maxsw,     60) \
    X(maxuw,     61) \
    X(minsw,     62) \
    X(minuw,     63) \
    X(sll,       64) \
    X(sra,       65) \
    X(srl,       66) \
    X(srd,       67) \
    X(slli,      68) \
    X(srai,      69) \
    X(srli,      70) \
    X(srdi,      71) \
    X(sllw,      72) \
    X(sraw,      73) \
    X(srlw,      74) \
    X(slliw,     76) \
    X(sraiw,     77) \
    X(srliw,     78) \
    X(absd,      80) \
    X(absdw,     81) \
    X(cmpqeq,    82) \
    X(cmpqne,    83) \
    X(andneg,    84) \
    X(xordec,    85) \
    X(anddec,    86) \
    X(cmpqlt,    87) \
    X(cmpqle,    88) \
    X(addq,      90) \
    X(subq,      91) \
    X(cmpqltu,   92) \
    X(cmpqleu,   93) \
    X(callrvt,   95) \
    X(jmpr,      96) \
    X(jmpt,      97) \
    X(jmptws,    98) \
    X(callri,    99) \
    X(callmi,   100) \
    X(callmrw,  101) \
    X(ret,      102) \
    X(jmptwz,   103) \
    X(mbgath,   104) \
    X(mbscat,   105) \
    X(cntpop,   106) \
    X(cntlz,    107) \
    X(cnttz,    108) \
    X(permb,    109) \
    X(gtb,      110) \
    OPCODE_TABLE_LOADSTORE_OPS(X, mia, 136) \
    OPCODE_TABLE_LOADSTORE_OPS(X, mib, 136 + 16) \
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
    X(aesenc,          SPECIAL_BASE + 40) \
    X(aesenclast,      SPECIAL_BASE + 41) \
    X(aesdec,          SPECIAL_BASE + 42) \
    X(aesdeclast,      SPECIAL_BASE + 43) \
    X(aesimc,          SPECIAL_BASE + 44) \
    X(aeskeygenassist, SPECIAL_BASE + 45) \
    X(sllqi,           SPECIAL_BASE + 48) \
    X(srlqi,           SPECIAL_BASE + 49) \
    X(sraqi,           SPECIAL_BASE + 50) \
    X(jmpmi,           SPECIAL_BASE + 51) \
    X(fence,           SPECIAL_BASE + 52) \
    OPCODE_TABLE_ATOMIC_OPS(X, b, ATOMICS_BASE + 32 * 0) \
    OPCODE_TABLE_ATOMIC_OPS(X, h, ATOMICS_BASE + 32 * 1) \
    OPCODE_TABLE_ATOMIC_OPS(X, w, ATOMICS_BASE + 32 * 2) \
    OPCODE_TABLE_ATOMIC_OPS(X, d, ATOMICS_BASE + 32 * 3) \
    OPCODE_TABLE_ATOMIC_SHORT_OPS(X, q, ATOMICS_BASE + 32 * 4) \
    /* fpu */ \
    OPCODE_TABLE_FP_SCALAR_OPS(X, ss,    FPU32_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, ps, w, FPU32_BASE + 64) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, sd,    FPU64_BASE) \
    OPCODE_TABLE_FP_PACKED_OPS(X, pd, d, FPU64_BASE + 64) \
    X(fcvtsd2ss, FPU64_BASE + 56) \
    X(fextss2sd, FPU64_BASE + 57) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, sq,    FPU128_BASE) \
    X(fscale,    FPU128_BASE + 56) \
    X(fextss2sq, FPU128_BASE + 57) \
    X(fextsd2sq, FPU128_BASE + 58) \
    X(fcvtsq2sd, FPU128_BASE + 59) \
    X(fcvtsq2ss, FPU128_BASE + 60) \
    OPCODE_TABLE_FP_SCALAR_OPS(X, sh,    FPU16_BASE) \
    X(fextsh2ss, FPU16_BASE + 56) \
    X(fextsh2sd, FPU16_BASE + 57) \
    X(fextsh2sq, FPU16_BASE + 58) \
    X(fcvtss2sh, FPU16_BASE + 59) \
    X(fcvtsd2sh, FPU16_BASE + 60) \
    X(fcvtsq2sh, FPU16_BASE + 61) \
    OPCODE_TABLE_FP_PACKED_OPS(X, ph, h, FPU16_BASE + 64) \
    /* mmx */ \
    OPCODE_TABLE_MMX_OPS(X, b, MMXB_BASE) \
    X(vupklsb, MMXB_BASE + 32) \
    X(vupkhsb, MMXB_BASE + 33) \
    OPCODE_TABLE_MMX_OPS(X, h, MMXH_BASE) \
    X(vupklsh, MMXH_BASE + 32) \
    X(vupkhsh, MMXH_BASE + 33) \
    X(vpksssh, MMXH_BASE + 34) \
    X(vpksush, MMXH_BASE + 35) \
    X(vpkuumh, MMXH_BASE + 36) \
    X(vpkuush, MMXH_BASE + 37) \
    OPCODE_TABLE_MMX_OPS(X, w, MMXW_BASE) \
    X(vupklsw, MMXW_BASE + 32) \
    X(vupkhsw, MMXW_BASE + 33) \
    X(vpksssw, MMXW_BASE + 34) \
    X(vpksusw, MMXW_BASE + 35) \
    X(vpkuumw, MMXW_BASE + 36) \
    X(vpkuusw, MMXW_BASE + 37) \
    OPCODE_TABLE_MMX_OPS(X, d, MMXD_BASE) \
    X(vpksssd, MMXD_BASE + 33) \
    X(vpksusd, MMXD_BASE + 34) \
    X(vpkuumd, MMXD_BASE + 36) \
    X(vpkuusd, MMXD_BASE + 37) \

enum EMiscOpx {
    DECLARE_MISC_EXTENDED_OPCODES(OPCODE_X)
    misc_opx_last,
    PSEUDO_INSN(cmpwgt,  cmpwlt)
    PSEUDO_INSN(cmpwgtu, cmpwltu)
    PSEUDO_INSN(cmpwge,  cmpwle)
    PSEUDO_INSN(cmpwgeu, cmpwleu)
    PSEUDO_INSN(cmpdgt,  cmpdlt)
    PSEUDO_INSN(cmpdgtu, cmpdltu)
    PSEUDO_INSN(cmpdge,  cmpdle)
    PSEUDO_INSN(cmpdgeu, cmpdleu)
    PSEUDO_INSN(cmpqgt,  cmpqlt)
    PSEUDO_INSN(cmpqgtu, cmpqltu)
    PSEUDO_INSN(cmpqge,  cmpqle)
    PSEUDO_INSN(cmpqgeu, cmpqleu)
};

inline constexpr u64 insn_tmplt(const EMiscOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_misc)
         | makepart(static_cast<u64>(opx), disp_misc_opx);
}

/***************************************************************************
* instruction modifiers
***************************************************************************/
enum EScaleMode {
    scale_mode_dword     = 0,
    scale_mode_word      = 1,
    scale_mode_signed    = 2,
    scale_mode_zext_word = scale_mode_word | 0,
    scale_mode_sext_word = scale_mode_word | scale_mode_signed,
};
char const *GetScaleModeName(EScaleMode);

enum EMemoryOrder {
    relaxed = 0,
    acquire = 1,
    release = 2,
    acq_rel = acquire | release,
    seq_cst = 4,
};
char const *GetMemoryOrderName(EMemoryOrder);

#define INSN_TMPLT(opc) insn_tmplt(opcode_##opc)

} // namespace postrisc
