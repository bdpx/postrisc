//===------------------------------------------------------===//
// assembler instruction mnemonics
//
// X(mnemonic, ident, hw_subset, format, description)
//
// 1  mnemonic:
//    assembler mnemonic
// 2  internal mnemonic:
//    identifier (11 characters maximum)
// 3  instruction subset:
//    common base (base),
//    floating-point f128,
//    floating-point double scalar/packed (f64),
//    floating-point single scalar/packed (f32),
//    dsp-like integer packed (mmx),
//    privileged, special,
//    atomic, cipher (aes+clmul)
//
// 4  sw/hw format:
//    assembler syntax format and
//    instruction slot format (in different sets)
// 5  description:
//    full instruction name
//
//===------------------------------------------------------===//

X(nop,     base, nop,   "no operation")

//===------------------------------------------------------===//
// add/subtract
//===------------------------------------------------------===//
X(add_i64,   base, r3, "add i64")
X(addc_u64,  base, r3, "add carry-out u64")
X(addo_i64,  base, r3, "add overflow i64")
X(add_i32,   base, r3, "add i32")
X(add_u32,   base, r3, "add u32")
X(add_i128,  i128, r3, "add i128")

X(sub_i64,   base, r3, "subtract i64")
X(subb_u64,  base, r3, "subtract with borrow u64")
X(subo_i64,  base, r3, "subtract overflow i64")
X(sub_i32,   base, r3, "subtract i32")
X(sub_u32,   base, r3, "subtract u32")
X(sub_i128,  i128, r3, "subtract i128")

X(add_add_i64,   base, r4, "add-add i64")
X(add_sub_i64,   base, r4, "add-subtract i64")
X(sub_sub_i64,   base, r4, "subtract-subtract i64")
X(add_addc_u64,  base, r4, "add-add with carry-out u64")
X(sub_subb_u64,  base, r4, "subtract-subtract with borrow-out u64")

X(add_imm_i64,  base, bin_simm, "add immediate i64")
X(add_imm_i32,  base, bin_simm, "add immediate i32")
X(add_imm_u32,  base, bin_simm, "add immediate u32")
X(add_imm_i128, i128, bin_simm, "add immediate i128")

X(subr_imm_i64, base, bin_simm, "subtract reverse i64")
X(subr_imm_i32, base, bin_simm, "subtract reverse i32")
X(subr_imm_u32, base, bin_simm, "subtract reverse u32")

X(neg_i32,   base, r2, "neg i32")
X(neg_i64,   base, r2, "neg i64")
X(neg_i128,  base, r2, "neg i128")

//===------------------------------------------------------===//
// bitwise
//===------------------------------------------------------===//
X(not,     base, r2, "bitwise not")
X(and,     base, r3, "bitwise and")
X(or,      base, r3, "bitwise or")
X(xor,     base, r3, "bitwise exclusive or")
X(nand,    base, r3, "bitwise not-and")
X(nor,     base, r3, "bitwise not-or")
X(xnor,    base, r3, "bitwise exclusive not-or")
X(andn,    base, r3, "bitwise and-not")
X(orn,     base, r3, "bitwise or-not")

X(and_imm,    base, bin_simm, "and bitwise with immediate")
X(or_imm,     base, bin_simm, "bitwise or with immediate")
X(xor_imm,    base, bin_simm, "bitwise exclusive or with immediate")
X(andn_imm,   base, bin_simm, "bitwise and-not with immediate")
X(orn_imm,    base, bin_simm, "bitwise or-not immediate")

X(and_dec, base, r3, "bitwise and decremented")
X(and_neg, base, r3, "bitwise and negate")
X(xor_dec, base, r3, "bitwise exclusive-or decremented")

//===------------------------------------------------------===//
//
//===------------------------------------------------------===//
X(abs_i32,  base,  r2, "absolute value i32")
X(abs_i64,  base,  r2, "absolute value i64")
X(abs_i128, base,  r2, "absolute value i128")

X(abs_diff_i64, base,  r3, "absolute difference i64")
X(abs_diff_i32, base,  r3, "absolute difference i32")
X(alignup_u64,  base,  r2s2,  "align up shifted")
X(dep,      base,  r3s2, "deposit")
X(dep_i128, i128,  r3s1,  "deposit i128")
X(dep_a,    base,  r2s2,  "deposit alter")
X(dep_c,    base,  r2s2,  "deposit clear")
X(dep_s,    base,  r2s2,  "deposit set")
X(mov,      base,  r2, "move general register")
X(mov2,     base,  r4, "move 2 general registers")
X(mul_i64,  base,  r3, "multiply i64")
X(mul_i32,  base,  r3, "multiply i32")
X(mul_u32,  base,  r3, "multiply u32")
X(mul_add,  base,  r4, "multiply-add u64")
X(mul_h,    base,  r3, "multiply high")
X(mul_imm_i64, base,  bin_simm,  "multiply immediate i64")
X(mul_imm_i32, base,  bin_simm,  "multiply immediate i32")
X(mul_imm_u32, base,  bin_uimm,  "multiply immediate u32")
X(mul_sub,  base,  r4, "multiply-subtract i64")
X(mul_subr, base,  r4, "multiply-subtract reverse i64")
X(mbsel,    base,  r4, "masked bit selection")
X(mul_i128, i128,  r3,  "multilpy i128")

X(ld_imm,      base,  ld_imm,  "load immediate")
X(ld_imm_high, base,  ld_imm,  "load immediate high")

//===------------------------------------------------------===//
// conditional move
//===------------------------------------------------------===//
X(cmov_lsb,    base, r4, "cond move if least significand bit")
X(cmov_eq_i32, base, r4, "cond move if i32 equal zero")
X(cmov_lt_i32, base, r4, "cond move if i32 less than zero")
X(cmov_le_i32, base, r4, "cond move if i32 less than or equal zero")
X(cmov_eq_i64, base, r4, "cond move if i64 equal zero")
X(cmov_lt_i64, base, r4, "cond move if i64 less than zero")
X(cmov_le_i64, base, r4, "cond move if i64 less than or equal zero")

//===------------------------------------------------------===//
// shifts
//===------------------------------------------------------===//
X(sll_u64,    base, r3,    "shift left logical u64")
X(srl_u64,    base, r3,    "shift right logical u64")
X(sra_i64,    base, r3,    "shift right algebraic i64")
X(srd_i64,    base, r3,    "shift right dividing i64")

X(sll_u32,    base, r3,    "shift left logical u32")
X(srl_u32,    base, r3,    "shift right logical u32")
X(sra_i32,    base, r3,    "shift right algebraic i32")
X(srd_i32,    base, r3,    "shift right dividing i32")

X(sll_imm_u64,   base, r2s1, "shift left logical immediate u64")
X(sll_imm_u128,  i128, r2s1, "sll immediate u128")
X(sll_imm_u32,   base, r2s1, "shift left logical immediate u32")
X(srl_imm_u64,   base, r2s1, "shift right logical immediate u64")
X(srl_imm_u128,  i128, r2s1, "srl right immediate u128")
X(srl_imm_u32,   base, r2s1, "shift right logical immediate u32")
X(sra_imm_i64,   base, r2s1, "shift right algebraic immediate i64")
X(sra_imm_i128,  i128, r2s1, "shift right algebraic immediate i128")
X(sra_imm_i32,   base, r2s1, "shift right algebraic immediate i32")

X(srd_imm_i64,   base, r2s1, "shift right dividing immediate i64")

X(srd_imm_i32,   base, r2s1, "shift right dividing immediate i32")

X(slsra_i64, base, r4,     "shift left and shift right algebraic i64")
X(slsra_i32, base, r4,     "shift left and shift right algebraic i32")
X(slsrl_u64, base, r4,     "shift left and shift right logical u64")
X(slsrl_u32, base, r4,     "shift left and shift right logical i32")

X(slsra_imm_i64,  base, r2s2, "shift left and right algebraic immediate i64")
X(slsrl_imm_u64,  base, r2s2, "shift left and right logical immediate u64")


X(sl_add_i64,  base, r3s1, "shift left and add i64")
X(sl_add_i32,  base, r3s1, "shift left and add i32")
X(sl_add_u32,  base, r3s1, "shift left and add u32")
X(sl_sub_i64,  base, r3s1, "shift left and subtract i64")
X(sl_sub_i32,  base, r3s1, "shift left and subtract i32")
X(sl_sub_u32,  base, r3s1, "shift left and subtract u32")
X(sl_subr_i64, base, r3s1, "shift left and subtract reverse i64")
X(sl_subr_i32, base, r3s1, "shift left and subtract reverse i32")
X(sl_subr_u32, base, r3s1, "shift left and subtract reverse u32")
X(sl_or,       base, r3s1, "shift left and or")
X(sl_xor,      base, r3s1, "shift left and xor")

X(slp_i64,   base, r4, "shift left pair i64")
X(slp_i32,   base, r4, "shift left pair i32")
X(slp_i128,  i128, r4, "shift left pair i128")
X(srp_i64,   base, r4, "shift right pair i64")
X(srp_i32,   base, r4, "shift right pair i32")
X(srp_i128,  i128, r4, "shift right pair i128")

X(srp_imm_i64,  base, r3s1, "shift right pair immediate i64")
X(srp_imm_i32,  base, r3s1, "shift right pair immediate i32")
X(srp_imm_i128, i128, r3s1, "shift right pair immediate i128")

X(bit_set,      bitmanip, r3, "bit set")
X(bit_clear,    bitmanip, r3, "bit clear")
X(bit_flip,     bitmanip, r3, "bit flip")

X(bit_set_imm,  bitmanip, r2s1, "bit set immediate")
X(bit_clear_imm,bitmanip, r2s1, "bit clear immediate")
X(bit_flip_imm, bitmanip, r2s1, "bit flip immediate")

//===------------------------------------------------------===//
// divide/modulo
//===------------------------------------------------------===//
X(div_i64,  base, r3, "divide i64")
X(div_u64,  base, r3, "divide u64")
X(div_i32,  base, r3, "divide u32")
X(div_u32,  base, r3, "divide u32")
X(rem_i64,  base, r3, "remainder i64")
X(rem_u64,  base, r3, "remainder u64")
X(rem_i32,  base, r3, "remainder i32")
X(rem_u32,  base, r3, "remainder u32")

X(div_imm_i64,  base, bin_simm, "divide i64 immediate")
X(div_imm_u64,  base, bin_uimm, "divide u64 immediate")
X(div_imm_i32,  base, bin_simm, "divide i32 immediate")
X(div_imm_u32,  base, bin_uimm, "divide u32 immediate")

X(rem_imm_i64,  base, bin_simm, "remainder i64 immediate")
X(rem_imm_u64,  base, bin_uimm, "remainder u64 immediate")
X(rem_imm_i32,  base, bin_simm, "remainder i32 immediate")
X(rem_imm_u32,  base, bin_uimm, "remainder u32 immediate")

//===------------------------------------------------------===//
// min/max
//===------------------------------------------------------===//
X(min_i32,    base, r3, "minimum i32")
X(max_i32,    base, r3, "maximum i32")
X(min_u32,    base, r3, "minimum u32")
X(max_u32,    base, r3, "maximum u32")

X(min_i64,    base, r3, "minimum i64")
X(max_i64,    base, r3, "maximum i64")
X(min_u64,    base, r3, "minimum u64")
X(max_u64,    base, r3, "maximum u64")

X(min_imm_i32,   base, bin_simm, "minimum immediate i32")
X(max_imm_i32,   base, bin_simm, "maximum immediate i32")
X(min_imm_u32,   base, bin_uimm, "minimum immediate u32")
X(max_imm_u32,   base, bin_uimm, "maximum immediate u32")

X(min_imm_i64,   base, bin_simm, "minimum immediate i64")
X(max_imm_i64,   base, bin_simm, "maximum immediate i64")
X(min_imm_u64,   base, bin_uimm, "minimum immediate u64")
X(max_imm_u64,   base, bin_uimm, "maximum immediate u64")


#define INSN_SCALAR_COMMON(STYPE, UTYPE) \
    X(cmp_eq_      ## STYPE,  base,        r3,              "compare equal " #STYPE) \
    X(cmp_ne_      ## STYPE,  base,        r3,              "compare not equal " #STYPE) \
    X(cmp_lt_      ## STYPE,  base,        r3,              "compare less " #STYPE) \
    X(cmp_le_      ## STYPE,  base,        r3,              "compare less or equal " #STYPE) \
    X(cmp_lt_      ## UTYPE,  base,        r3,              "compare less " #UTYPE) \
    X(cmp_le_      ## UTYPE,  base,        r3,              "compare less or equal " #UTYPE) \
    X(cmp_gt_      ## STYPE,  base,        r3_swapped,      "compare greater " #STYPE) \
    X(cmp_gt_      ## UTYPE,  base,        r3_swapped,      "compare greater " #UTYPE) \
    X(cmp_ge_      ## STYPE,  base,        r3_swapped,      "compare greater or equal " #STYPE) \
    X(cmp_ge_      ## UTYPE,  base,        r3_swapped,      "compare greater or equal " #UTYPE) \
    X(cmp_eq_imm_  ## STYPE,  base,        bin_simm,        "compare equal immediate " #STYPE) \
    X(cmp_ne_imm_  ## STYPE,  base,        bin_simm,        "compare not equal immediate " #STYPE) \
    X(cmp_lt_imm_  ## STYPE,  base,        bin_simm,        "compare less immediate " #STYPE) \
    X(cmp_gt_imm_  ## STYPE,  base,        bin_simm,        "compare greater immediate " #STYPE) \
    X(cmp_lt_imm_  ## UTYPE,  base,        bin_uimm,        "compare less immediate " #UTYPE) \
    X(cmp_gt_imm_  ## UTYPE,  base,        bin_uimm,        "compare greater immediate " #UTYPE) \
    X(cmp_ge_imm_  ## STYPE,  base,        bin_simm_minus,  "compare greater or equal immediate " #STYPE) \
    X(cmp_le_imm_  ## STYPE,  base,        bin_simm_plus,   "compare less or equal immediate " #STYPE) \
    X(cmp_ge_imm_  ## UTYPE,  base,        bin_uimm_minus,  "compare greater or equal immediate " #UTYPE) \
    X(cmp_le_imm_  ## UTYPE,  base,        bin_uimm_plus,   "compare less or equal immediate " #UTYPE) \
    X(br_eq_       ## STYPE,  branch,      br_rr,           "branch if equal " #STYPE) \
    X(br_ne_       ## STYPE,  branch,      br_rr,           "branch if not equal " #STYPE) \
    X(br_lt_       ## STYPE,  branch,      br_rr,           "branch if less " #STYPE) \
    X(br_lt_       ## UTYPE,  branch,      br_rr,           "branch if less " #UTYPE) \
    X(br_le_       ## STYPE,  branch,      br_rr,           "branch if less or equal " #STYPE) \
    X(br_le_       ## UTYPE,  branch,      br_rr,           "branch if less or equal " #UTYPE) \
    X(br_eq_imm_   ## STYPE,  branch,      br_simm,         "branch if equal immediate " #STYPE) \
    X(br_ne_imm_   ## STYPE,  branch,      br_simm,         "branch if not equal immediate " #STYPE) \
    X(br_lt_imm_   ## STYPE,  branch,      br_simm,         "branch if less immediate " #STYPE) \
    X(br_ge_imm_   ## STYPE,  branch,      br_simm,         "branch if greater or equal immediate " #STYPE) \
    X(br_lt_imm_   ## UTYPE,  branch,      br_uimm,         "branch if less immediate " #UTYPE) \
    X(br_ge_imm_   ## UTYPE,  branch,      br_uimm,         "branch if greater or equal immediate " #UTYPE) \
    X(br_gt_       ## STYPE,  branch,      br_rr_swapped,   "branch if greater " #STYPE) \
    X(br_gt_       ## UTYPE,  branch,      br_rr_swapped,   "branch if greater " #UTYPE) \
    X(br_ge_       ## STYPE,  branch,      br_rr_swapped,   "branch if greater or equal " #STYPE) \
    X(br_ge_       ## UTYPE,  branch,      br_rr_swapped,   "branch if greater or equal " #UTYPE) \
    X(br_le_imm_   ## STYPE,  branch,      br_simm_plus,    "branch if less or equal immediate " #STYPE) \
    X(br_gt_imm_   ## STYPE,  branch,      br_simm_plus,    "branch if greater immediate " #STYPE) \
    X(br_le_imm_   ## UTYPE,  branch,      br_uimm_plus,    "branch if less or equal immediate " #UTYPE) \
    X(br_gt_imm_   ## UTYPE,  branch,      br_uimm_plus,    "branch if greater immediate " #UTYPE) \
    X(nul_eq_      ## STYPE,  nullifying,  nul_rr,          "nullify if equal " #STYPE) \
    X(nul_ne_      ## STYPE,  nullifying,  nul_rr,          "nullify if not-equal " #STYPE) \
    X(nul_lt_      ## STYPE,  nullifying,  nul_rr,          "nullify if less " #STYPE) \
    X(nul_le_      ## STYPE,  nullifying,  nul_rr,          "nullify if less or equal " #STYPE) \
    X(nul_lt_      ## UTYPE,  nullifying,  nul_rr,          "nullify if less " #UTYPE) \
    X(nul_le_      ## UTYPE,  nullifying,  nul_rr,          "nullify if less or equal " #UTYPE) \
    X(nul_eq_imm_  ## STYPE,  nullifying,  nul_simm,        "nullify if equal immediate " #STYPE) \
    X(nul_ne_imm_  ## STYPE,  nullifying,  nul_simm,        "nullify if not-equal immediate " #STYPE) \
    X(nul_lt_imm_  ## STYPE,  nullifying,  nul_simm,        "nullify if less immediate " #STYPE) \
    X(nul_gt_imm_  ## STYPE,  nullifying,  nul_simm,        "nullify if greater immediate " #STYPE) \
    X(nul_lt_imm_  ## UTYPE,  nullifying,  nul_uimm,        "nullify if less immediate " #UTYPE) \
    X(nul_gt_imm_  ## UTYPE,  nullifying,  nul_uimm,        "nullify if greater immediate " #UTYPE) \

INSN_SCALAR_COMMON(i32,  u32)
INSN_SCALAR_COMMON(i64,  u64)
INSN_SCALAR_COMMON(i128, u128)

//===------------------------------------------------------===//
// bit manipulation/counting
//===------------------------------------------------------===//
X(cnt_lz,   bitmanip, r2s1, "count leading zeros")
X(cnt_tz,   bitmanip, r2s1, "count trailing zeros")
X(cnt_pop,  bitmanip, r2s1, "count population")

X(gtb,     bitmanip, r2, "graycode to binary")
X(perm,    bitmanip, r4, "permute bytes")
X(permb,   bitmanip, r2s1, "permute bits")
X(mbgath,  bitmanip, r3, "masked bit gather")
X(mbscat,  bitmanip, r3, "masked bit scatter")

//===------------------------------------------------------===//
// branch/nullify if mask/bit set/clear
//===------------------------------------------------------===//
X(br_bc,   branch, br_rr,   "branch if bit clear")
X(br_bs,   branch, br_rr,   "branch if bit set")

X(br_bc_imm,  branch, br_rs,  "branch if bit clear immediate")
X(br_bs_imm,  branch, br_rs,  "branch if bit set immediate")

X(br_mask_all,    branch, br_uimm,  "branch if mask immediate all bits set")
X(br_mask_any,    branch, br_uimm,  "branch if mask immediate any bit set")
X(br_mask_none,   branch, br_uimm,  "branch if mask immediate none bit set")
X(br_mask_notall, branch, br_uimm,  "branch if mask immediate not all bits set")

X(nul_bc,   nullifying, nul_rr,   "nullify if bit clear")
X(nul_bs,   nullifying, nul_rr,   "nullify if bit set")

X(nul_bc_imm,  nullifying, nul_rs, "nullify if bit clear immediate")
X(nul_bs_imm,  nullifying, nul_rs, "nullify if bit set immediate")

X(nul_mask_all,    nullifying, nul_uimm, "nullify if mask immediate all bits set")
X(nul_mask_any,    nullifying, nul_uimm, "nullify if mask immediate any bit set")
X(nul_mask_none,   nullifying, nul_uimm, "nullify if mask immediate none bit set")
X(nul_mask_notall, nullifying, nul_uimm, "nullify if mask immediate not all bits set")

//===------------------------------------------------------===//
// loops
//===------------------------------------------------------===//
X(rep_lt_i64,  branch, loop, "repeat on less i64")
X(rep_le_i64,  branch, loop, "repeat on less or equal i64")
X(rep_gt_i64,  branch, loop, "repeat on greater i64")
X(rep_ge_i64,  branch, loop, "repeat on greater or equal i64")
X(rep_lt_u64,  branch, loop, "repeat on less u64")
X(rep_le_u64,  branch, loop, "repeat on less or equal u64")
X(rep_gt_u64,  branch, loop, "repeat on greater u64")
X(rep_ge_u64,  branch, loop, "repeat on greater or equal u64")

X(rep_lt_i32,  branch, loop, "repeat on less i32")
X(rep_le_i32,  branch, loop, "repeat on less or equal i32")
X(rep_gt_i32,  branch, loop, "repeat on greater i32")
X(rep_ge_i32,  branch, loop, "repeat on greater or equal i32")
X(rep_lt_u32,  branch, loop, "repeat on less u32")
X(rep_le_u32,  branch, loop, "repeat on less or equal u32")
X(rep_gt_u32,  branch, loop, "repeat on greater u32")
X(rep_ge_u32,  branch, loop, "repeat on greater or equal u32")

//===------------------------------------------------------===//
// unconditional jumps
//===------------------------------------------------------===//
X(jmp,       branch, jmp,       "jump relative")
X(jmp_r,     branch, RbcScale,  "jump register indirect")
X(jmp_t,     branch, Rbc,       "jump table")
X(jmp_t_i32, branch, Rbc,       "jump table i32 index")
X(jmp_t_u32, branch, Rbc,       "jump table u32 index")
X(jmp_mi,    special, RbcScale, "jmpmi") // workaround

//===------------------------------------------------------===//
// call/return/regframe
//===------------------------------------------------------===//
X(call_mi,  branch, RabS10,    "call memory indirect")
X(call,     branch, call,      "call relative")
X(call_ri,  branch, r3,        "call register indirect")
X(call_rvt, branch, RabS10,    "call relative vtable")
X(call_plt, branch, mem_iprel, "call procedure linkage table")
X(ret,      branch, NoArgs,    "return from subroutine")
X(retf,     branch, retf,      "return from subroutine (rollback frame)")

//===------------------------------------------------------===//
// special
//===------------------------------------------------------===//
X(alloc,    special, alloc,    "allocate register frame, update eip")
X(alloc_sp, special, alloc_sp, "allocate register frame, update eip,sp")
X(syscall,  special, NoArgs,   "system call")
X(undef,    special, NoArgs,   "undefined instruction")
X(random,   special, r2,       "random")
X(get_spr,  special, mspr,     "get special-purpose register")
X(set_spr,  special, mspr,     "set special-purpose register")
X(icbi,     special, RaxbSI21, "instruction cache block invalidate")
X(int,      special, RbS10,    "interrupt")
X(mprobe,   special, r3,       "memory probe access")
X(cpuid,    special, RabS10,   "cpu identification")
X(dcbf,     special, RaxbSI21, "data cache block flush")
X(dcbt,     special, RaxbSI21, "data cache block touch")

//===------------------------------------------------------===//
// software exceptions
//===------------------------------------------------------===//
X(eh_adj,    special, jmp,      "exception handler adjust eip")
X(eh_catch,  special, br_eh,    "exception handler catch")
X(eh_next,   special, br_eh,    "exception handler next")
X(eh_throw,  special, RaxbSI21, "exception handler throw")

//===------------------------------------------------------===//
// software emulation
//===------------------------------------------------------===//
X(write,    special, write,   "write string formatted")

//===------------------------------------------------------===//
// privileged
//===------------------------------------------------------===//
X(sysret,  privileged, NoArgs,   "system return")
X(rfi,     privileged, NoArgs,   "return from interruption")
X(rscover, privileged, NoArgs,   "register stack cover")
X(rsflush, privileged, NoArgs,   "register stack flush")
X(rsload,  privileged, NoArgs,   "register stack load")
X(halt,    privileged, NoArgs,   "halt processor")
X(dcbi,    privileged, RaxbSI21, "data cache block invalidate")

//===------------------------------------------------------===//
// privileged, TLB
//===------------------------------------------------------===//
X(tpa, privileged, r2, "translate to physical address")
X(ptc, privileged, r3, "purge translation cache")

X(set_dtr, privileged, r3, "set data translation register")
X(set_itr, privileged, r3, "set instruction translation register")

//===------------------------------------------------------===//
// debug/monitoring
//===------------------------------------------------------===//
X(get_ibr,  privileged, RabS10, "get instruction breakpoint register")
X(set_ibr,  privileged, RabS10, "set instruction breakpoint register")
X(get_dbr,  privileged, RabS10, "get data breakpoint register")
X(set_dbr,  privileged, RabS10, "set data breakpoint register")
X(get_mr,   privileged, RabS10, "get monitor register")
X(set_mr,   privileged, RabS10, "set monitor register")

//===------------------------------------------------------===//
// load/store relative/indexed
//===------------------------------------------------------===//
X(lda_r,    base,  lda_r,   "load address relative")
X(lda_n,    base,  bin_simm, "load address near")
X(lda_nrc,  base,  bin_simm, "load address near relative")


X(ld_i8,      memory, bin_simm, "load i8")
X(ld_u8,      memory, bin_simm, "load u8")
X(st_i8,      memory, bin_simm, "store i8")
X(ld_i16,     memory, bin_simm, "load i16")
X(ld_u16,     memory, bin_simm, "load u16")
X(st_i16,     memory, bin_simm, "store i16")
X(ld_i32,     memory, bin_simm, "load i32")
X(ld_u32,     memory, bin_simm, "load u32")
X(st_i32,     memory, bin_simm, "store i32")
X(ld_i64,     memory, bin_simm, "load i64")
X(ld_u64,     memory, bin_simm, "load u64")
X(st_i64,     memory, bin_simm, "store i64")
X(ld_i128,    memory, bin_simm, "load i128")
X(st_i128,    memory, bin_simm, "store i128")

X(lda_iprel,      base,   mem_iprel,   "load address relative forward")
X(ld_iprel_i8,    memory, mem_iprel,   "load relative i8")
X(ld_iprel_u8,    memory, mem_iprel,   "load relative u8")
X(st_iprel_i8,    memory, mem_iprel,   "store relative i8")
X(ld_iprel_i16,   memory, mem_iprel,   "load relative i16")
X(ld_iprel_u16,   memory, mem_iprel,   "load relative u16")
X(st_iprel_i16,   memory, mem_iprel,   "store relative i16")
X(ld_iprel_i32,   memory, mem_iprel,   "load relative i32")
X(ld_iprel_u32,   memory, mem_iprel,   "load relative u32")
X(st_iprel_i32,   memory, mem_iprel,   "store relative i32")
X(ld_iprel_i64,   memory, mem_iprel,   "load relative i64")
X(ld_iprel_u64,   memory, mem_iprel,   "load relative u64")
X(st_iprel_i64,   memory, mem_iprel,   "store relative i64")
X(ld_iprel_i128,  memory, mem_iprel,   "load relative i128")
X(st_iprel_i128,  memory, mem_iprel,   "store relative i128")

X(lda_xi32,     base,   gmemx,   "load address i32-indexed")
X(ld_xi32_i8,   memory, gmemx,   "load i32-indexed i8")
X(ld_xi32_u8,   memory, gmemx,   "load i32-indexed u8")
X(st_xi32_i8,   memory, gmemx,   "store i32-indexed i8")
X(ld_xi32_i16,  memory, gmemx,   "load i32-indexed i16")
X(ld_xi32_u16,  memory, gmemx,   "load i32-indexed u16")
X(st_xi32_i16,  memory, gmemx,   "store i32-indexed i16")
X(ld_xi32_i32,  memory, gmemx,   "load i32-indexed i32")
X(ld_xi32_u32,  memory, gmemx,   "load i32-indexed u32")
X(st_xi32_i32,  memory, gmemx,   "store i32-indexed i32")
X(ld_xi32_i64,  memory, gmemx,   "load i32-indexed i64")
X(ld_xi32_u64,  memory, gmemx,   "load i32-indexed u64")
X(st_xi32_i64,  memory, gmemx,   "store i32-indexed i64")
X(ld_xi32_i128, memory, gmemx,   "load i32-indexed i128")
X(st_xi32_i128, memory, gmemx,   "store i32-indexed i128")

X(lda_xu32,     base,   gmemx,   "load address u32-indexed")
X(ld_xu32_i8,   memory, gmemx,   "load u32-indexed i8")
X(ld_xu32_u8,   memory, gmemx,   "load u32-indexed u8")
X(st_xu32_i8,   memory, gmemx,   "store u32-indexed i8")
X(ld_xu32_i16,  memory, gmemx,   "load u32-indexed i16")
X(ld_xu32_u16,  memory, gmemx,   "load u32-indexed u16")
X(st_xu32_i16,  memory, gmemx,   "store u32-indexed i16")
X(ld_xu32_i32,  memory, gmemx,   "load u32-indexed i32")
X(ld_xu32_u32,  memory, gmemx,   "load u32-indexed u32")
X(st_xu32_i32,  memory, gmemx,   "store u32-indexed i32")
X(ld_xu32_i64,  memory, gmemx,   "load u32-indexed i64")
X(ld_xu32_u64,  memory, gmemx,   "load u32-indexed u64")
X(st_xu32_i64,  memory, gmemx,   "store u32-indexed i64")
X(ld_xu32_i128, memory, gmemx,   "load u32-indexed i128")
X(st_xu32_i128, memory, gmemx,   "store u32-indexed i128")

X(lda_xi64,     base,   gmemx,   "load address i64-indexed")
X(ld_xi64_i8,   memory, gmemx,   "load i64-indexed i8")
X(ld_xi64_u8,   memory, gmemx,   "load i64-indexed u8")
X(st_xi64_i8,   memory, gmemx,   "store i64-indexed i8")
X(ld_xi64_i16,  memory, gmemx,   "load i64-indexed i16")
X(ld_xi64_u16,  memory, gmemx,   "load i64-indexed u16")
X(st_xi64_i16,  memory, gmemx,   "store i64-indexed i16")
X(ld_xi64_i32,  memory, gmemx,   "load i64-indexed i32")
X(ld_xi64_u32,  memory, gmemx,   "load i64-indexed u32")
X(st_xi64_i32,  memory, gmemx,   "store i64-indexed i32")
X(ld_xi64_i64,  memory, gmemx,   "load i64-indexed i64")
X(ld_xi64_u64,  memory, gmemx,   "load i64-indexed u64")
X(st_xi64_i64,  memory, gmemx,   "store i64-indexed i64")
X(ld_xi64_i128, memory, gmemx,   "load i64-indexed i128")
X(st_xi64_i128, memory, gmemx,   "store i64-indexed i128")

X(ld_mia_i8,    memory, gmemu,   "load and modify immediate after i8")
X(ld_mia_u8,    memory, gmemu,   "load and modify immediate after u8")
X(st_mia_i8,    memory, gmemu,   "store and modify immediate after i8")
X(ld_mia_i16,   memory, gmemu,   "load and modify immediate after i16")
X(ld_mia_u16,   memory, gmemu,   "load and modify immediate after u16")
X(st_mia_i16,   memory, gmemu,   "store and modify immediate after i16")
X(ld_mia_i32,   memory, gmemu,   "load and modify immediate after i32")
X(ld_mia_u32,   memory, gmemu,   "load and modify immediate after u32")
X(st_mia_i32,   memory, gmemu,   "store and modify immediate after i32")
X(ld_mia_i64,   memory, gmemu,   "load and modify immediate after i64")
X(ld_mia_u64,   memory, gmemu,   "load and modify immediate after u64")
X(st_mia_i64,   memory, gmemu,   "store and modify immediate after i64")
X(ld_mia_i128,  memory, gmemu,   "load and modify immediate after i128")
X(st_mia_i128,  memory, gmemu,   "store and modify immediate after i128")

X(ld_mib_i8,    memory, gmemu,   "load and modify immediate before i8")
X(ld_mib_u8,    memory, gmemu,   "load and modify immediate before u8")
X(st_mib_i8,    memory, gmemu,   "store and modify immediate before i8")
X(ld_mib_i16,   memory, gmemu,   "load and modify immediate before i16")
X(ld_mib_u16,   memory, gmemu,   "load and modify immediate before u16")
X(st_mib_i16,   memory, gmemu,   "store and modify immediate before i16")
X(ld_mib_i32,   memory, gmemu,   "load and modify immediate before i32")
X(ld_mib_u32,   memory, gmemu,   "load and modify immediate before u32")
X(st_mib_i32,   memory, gmemu,   "store and modify immediate before i32")
X(ld_mib_i64,   memory, gmemu,   "load and modify immediate before i64")
X(ld_mib_u64,   memory, gmemu,   "load and modify immediate before u64")
X(st_mib_i64,   memory, gmemu,   "store and modify immediate before i64")
X(ld_mib_i128,  memory, gmemu,   "load and modify immediate before i128")
X(st_mib_i128,  memory, gmemu,   "store and modify immediate before i128")

//===------------------------------------------------------===//
// atomics
//===------------------------------------------------------===//
#define INSN_TABLE_ATOMIC_LD(X, NAME, TYPE) \
    X(amx_ ## NAME ## _ ## TYPE, atomic, r2, "atomic relaxed load " #TYPE) \
    X(amq_ ## NAME ## _ ## TYPE, atomic, r2, "atomic acquire load " #TYPE)

#define INSN_TABLE_ATOMIC_ST(X, NAME, TYPE) \
    X(amx_ ## NAME ## _ ## TYPE, atomic, r2, "atomic relaxed store " #TYPE) \
    X(amr_ ## NAME ## _ ## TYPE, atomic, r2, "atomic release store " #TYPE)

#define INSN_TABLE_ATOMIC_LD_OP(X, NAME, TYPE, DESCR) \
    X(amx_ ## NAME ## _ ## TYPE, atomic, r3, "atomic relaxed " DESCR " " #TYPE) \
    X(amq_ ## NAME ## _ ## TYPE, atomic, r3, "atomic acquire " DESCR " " #TYPE) \
    X(amr_ ## NAME ## _ ## TYPE, atomic, r3, "atomic release " DESCR " " #TYPE) \
    X(amz_ ## NAME ## _ ## TYPE, atomic, r3, "atomic acq_rel " DESCR " " #TYPE)

#define INSN_TABLE_ATOMIC_ST_OP(X, NAME, TYPE, DESCR) \
    X(amx_ ## NAME ## _ ## TYPE, atomic, Rbc, "atomic relaxed " DESCR " " #TYPE) \
    X(amr_ ## NAME ## _ ## TYPE, atomic, Rbc, "atomic release " DESCR " " #TYPE)

#define INSN_TABLE_ATOMIC_FULL_X(X, ITYPE, UTYPE) \
    INSN_TABLE_ATOMIC_LD(   X, ld,      ITYPE) \
    INSN_TABLE_ATOMIC_ST(   X, st,      ITYPE) \
    INSN_TABLE_ATOMIC_LD_OP(X, swap,    UTYPE, "swap") \
    INSN_TABLE_ATOMIC_LD_OP(X, cas,     ITYPE, "compare and swap") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_add,  UTYPE, "load-add") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_and,  UTYPE, "load-and") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_or,   UTYPE, "load-or") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_xor,  UTYPE, "load-xor") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_min,  ITYPE, "load-minimum") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_max,  ITYPE, "load-maximum") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_min,  UTYPE, "load-minimum") \
    INSN_TABLE_ATOMIC_LD_OP(X, ld_max,  UTYPE, "load-maximum") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_add,  UTYPE, "store-add") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_and,  UTYPE, "store-and") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_or,   UTYPE, "store-or") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_xor,  UTYPE, "store-xor") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_min,  ITYPE, "store-minimum") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_max,  ITYPE, "store-maximum") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_min,  UTYPE, "store-minimum") \
    INSN_TABLE_ATOMIC_ST_OP(X, st_max,  UTYPE, "store-maximum") \

INSN_TABLE_ATOMIC_FULL_X(X, i8,   u8)
INSN_TABLE_ATOMIC_FULL_X(X, i16,  u16)
INSN_TABLE_ATOMIC_FULL_X(X, i32,  u32)
INSN_TABLE_ATOMIC_FULL_X(X, i64,  u64)
INSN_TABLE_ATOMIC_FULL_X(X, i128, u128)

X(fence_a,  atomic, NoArgs, "fence acquire")
X(fence_r,  atomic, NoArgs, "fence release")
X(fence_ar, atomic, NoArgs, "fence acq-rel")
X(fence_sc, atomic, NoArgs, "fence seq-cst")

#undef INSN_TABLE_ATOMIC_FULL_X
#undef INSN_TABLE_ATOMIC_LD
#undef INSN_TABLE_ATOMIC_ST
#undef INSN_TABLE_ATOMIC_LD_OP
#undef INSN_TABLE_ATOMIC_ST_OP

//===------------------------------------------------------===//
// cipher/aes/hashes
//===------------------------------------------------------===//
X(aes_dec,           cipher, r3, "aes decrypt round")
X(aes_dec_last,      cipher, r3, "aes decrypt last round")
X(aes_enc,           cipher, r3, "aes encrypt round")
X(aes_enc_last,      cipher, r3, "aes encrypt last round")
X(aes_imc,           cipher, r2, "aes inverse mix columns")
X(aes_keygen_assist, cipher, RabS10, "aes key generation assist")

X(clmul_ll, cipher, r3, "carry-less multiply low-low")
X(clmul_hl, cipher, r3, "carry-less multiply high-low")
X(clmul_hh, cipher, r3, "carry-less multiply high-high")
X(crc32c,   cipher, r4, "crc32c")

//===------------------------------------------------------===//
// floating point immediate load pseudos
//===------------------------------------------------------===//
X(ld_imm_f32,  f32, ld_imm_f32,  "load immediate f32")
X(ld_imm_f64,  f64, ld_imm_f64,  "load immediate f64")

X(ld_iprel_f32,  f32,  ld_iprel_f32,  "load relative f32")
X(ld_iprel_f64,  f64,  ld_iprel_f64,  "load relative f64")
X(ld_iprel_f128, f128, ld_iprel_f128, "load relative f128")


//===------------------------------------------------------===//
// floating-point
//===------------------------------------------------------===/
#define INSN_FPU_SCALAR_COMMON(TYPE, GROUP) \
    X(abs_          ## TYPE,          GROUP, r2,     "absolute value " #TYPE) \
    X(nabs_         ## TYPE,          GROUP, r2,     "negate absolute value " #TYPE) \
    X(neg_          ## TYPE,          GROUP, r2,     "negate " #TYPE) \
    X(add_          ## TYPE,          GROUP, r3,     "add " #TYPE) \
    X(sub_          ## TYPE,          GROUP, r3,     "subtract " #TYPE) \
    X(nadd_         ## TYPE,          GROUP, r3,     "negate add " #TYPE) \
    X(abs_diff_     ## TYPE,          GROUP, r3,     "absolute difference " #TYPE) \
    X(mul_          ## TYPE,          GROUP, r3,     "multiply " #TYPE) \
    X(nmul_         ## TYPE,          GROUP, r3,     "negate multiply " #TYPE) \
    X(nabs_diff_    ## TYPE,          GROUP, r3,     "negate absolute difference " #TYPE) \
    X(class_        ## TYPE,          GROUP, r2s1,  "classify " #TYPE) \
    X(rsqrt_        ## TYPE,          GROUP, r2,     "reciprocal square root " #TYPE) \
    X(sqrt_         ## TYPE,          GROUP, r2,     "square root " #TYPE) \
    X(div_          ## TYPE,          GROUP, r3,     "divide " #TYPE) \
    X(max_          ## TYPE,          GROUP, r3,     "maximum " #TYPE) \
    X(min_          ## TYPE,          GROUP, r3,     "minimum " #TYPE) \
    X(maxnum_       ## TYPE,          GROUP, r3,     "maximum number " #TYPE) \
    X(minnum_       ## TYPE,          GROUP, r3,     "minimum number " #TYPE) \
    X(abs_max_      ## TYPE,          GROUP, r3,     "absolute maximum " #TYPE) \
    X(abs_min_      ## TYPE,          GROUP, r3,     "absolute minimum " #TYPE) \
    X(madd_         ## TYPE,          GROUP, r4,     "multiply-add " #TYPE) \
    X(msub_         ## TYPE,          GROUP, r4,     "multiply-subtract " #TYPE) \
    X(nmadd_        ## TYPE,          GROUP, r4,     "negate multiply-add " #TYPE) \
    X(nmsub_        ## TYPE,          GROUP, r4,     "negate multiply-subtract " #TYPE) \
    X(merge_        ## TYPE,          GROUP, r4,     "merge " #TYPE) \
    X(round_        ## TYPE,          GROUP, r2,     "round away from zero " #TYPE) \
    X(floor_        ## TYPE,          GROUP, r2,     "floor (round down)" #TYPE) \
    X(ceil_         ## TYPE,          GROUP, r2,     "ceil (round up) " #TYPE) \
    X(trunk_        ## TYPE,          GROUP, r2,     "trunk (round to zero) " #TYPE) \
    X(roundeven_    ## TYPE,          GROUP, r2,     "round to nearest even " #TYPE) \
    X(nearbyint_    ## TYPE,          GROUP, r2,     "round dynamic " #TYPE) \
    X(rint_         ## TYPE,          GROUP, r2,     "round dynamic, detect inexact " #TYPE) \
    X(cmp_oeq_      ## TYPE,          GROUP, r3,     "compare ordered and equal " #TYPE) \
    X(cmp_one_      ## TYPE,          GROUP, r3,     "compare ordered and not-equal " #TYPE) \
    X(cmp_olt_      ## TYPE,          GROUP, r3,     "compare ordered and less " #TYPE) \
    X(cmp_ole_      ## TYPE,          GROUP, r3,     "compare ordered and less-equal " #TYPE) \
    X(cmp_o_        ## TYPE,          GROUP, r3,     "compare ordered " #TYPE) \
    X(cmp_ueq_      ## TYPE,          GROUP, r3,     "compare unordered or equal " #TYPE) \
    X(cmp_une_      ## TYPE,          GROUP, r3,     "compare unordered or not-equal " #TYPE) \
    X(cmp_ult_      ## TYPE,          GROUP, r3,     "compare unordered or less " #TYPE) \
    X(cmp_ule_      ## TYPE,          GROUP, r3,     "compare unordered or less-equal " #TYPE) \
    X(cmp_u_        ## TYPE,          GROUP, r3,     "compare unordered " #TYPE) \
    X(trunk_        ## TYPE ## _i32,  GROUP, r2,     "trunk " #TYPE " to i32") \
    X(trunk_        ## TYPE ## _u32,  GROUP, r2,     "trunk " #TYPE " to u32") \
    X(trunk_        ## TYPE ## _i64,  GROUP, r2,     "trunk " #TYPE " to i64") \
    X(trunk_        ## TYPE ## _u64,  GROUP, r2,     "trunk " #TYPE " to u64") \
    X(trunk_        ## TYPE ## _i128, GROUP, r2,     "trunk " #TYPE " to i128") \
    X(trunk_        ## TYPE ## _u128, GROUP, r2,     "trunk " #TYPE " to u128") \
    X(cvt_i32_      ## TYPE,          GROUP, r2,     "convert i32 to " #TYPE) \
    X(cvt_u32_      ## TYPE,          GROUP, r2,     "convert u32 to " #TYPE) \
    X(cvt_i64_      ## TYPE,          GROUP, r2,     "convert i64 to " #TYPE) \
    X(cvt_u64_      ## TYPE,          GROUP, r2,     "convert u64 to " #TYPE) \
    X(cvt_i128_     ## TYPE,          GROUP, r2,     "convert i128 to " #TYPE) \
    X(cvt_u128_     ## TYPE,          GROUP, r2,     "convert u128 to " #TYPE) \

INSN_FPU_SCALAR_COMMON(f16,  f16)
INSN_FPU_SCALAR_COMMON(f32,  f32)
INSN_FPU_SCALAR_COMMON(f64,  f64)
INSN_FPU_SCALAR_COMMON(f128, f128)

#define INSN_FPU_SCALAR_BRANCH(TYPE, GROUP) \
    X(nul_oeq_   ## TYPE, nullifying,  nul_rr,   "nullify if ordered and equal " #TYPE) \
    X(nul_one_   ## TYPE, nullifying,  nul_rr,   "nullify if ordered and not-equal " #TYPE) \
    X(nul_olt_   ## TYPE, nullifying,  nul_rr,   "nullify if ordered and less " #TYPE) \
    X(nul_ole_   ## TYPE, nullifying,  nul_rr,   "nullify if ordered and less-or-equal " #TYPE) \
    X(nul_o_     ## TYPE, nullifying,  nul_rr,   "nullify if ordered " #TYPE) \
    X(nul_ueq_   ## TYPE, nullifying,  nul_rr,   "nullify if unordered or equal " #TYPE) \
    X(nul_une_   ## TYPE, nullifying,  nul_rr,   "nullify if unordered or not-equal " #TYPE) \
    X(nul_ult_   ## TYPE, nullifying,  nul_rr,   "nullify if unordered or less " #TYPE) \
    X(nul_ule_   ## TYPE, nullifying,  nul_rr,   "nullify if unordered or less-or-equal " #TYPE) \
    X(nul_u_     ## TYPE, nullifying,  nul_rr,   "nullify if unordered " #TYPE) \
    X(nul_class_ ## TYPE, nullifying,  nul_rs,   "nullify if classify " #TYPE) \
    X(br_oeq_    ## TYPE, GROUP,       br_rr,    "branch if ordered and equal " #TYPE) \
    X(br_one_    ## TYPE, GROUP,       br_rr,    "branch if ordered and not-equal " #TYPE) \
    X(br_olt_    ## TYPE, GROUP,       br_rr,    "branch if ordered and less " #TYPE) \
    X(br_ole_    ## TYPE, GROUP,       br_rr,    "branch if ordered and less-or-equal " #TYPE) \
    X(br_o_      ## TYPE, GROUP,       br_rr,    "branch if ordered " #TYPE) \
    X(br_ueq_    ## TYPE, GROUP,       br_rr,    "branch if unordered or equal " #TYPE) \
    X(br_une_    ## TYPE, GROUP,       br_rr,    "branch if unordered or not-equal " #TYPE) \
    X(br_ult_    ## TYPE, GROUP,       br_rr,    "branch if unordered or less " #TYPE) \
    X(br_ule_    ## TYPE, GROUP,       br_rr,    "branch if unordered or less-or-equal " #TYPE) \
    X(br_u_      ## TYPE, GROUP,       br_rr,    "branch if unordered " #TYPE) \
    X(br_class_  ## TYPE, GROUP,       br_rs,    "branch if classify " #TYPE) \

INSN_FPU_SCALAR_BRANCH(f32,  f32)
INSN_FPU_SCALAR_BRANCH(f64,  f64)
INSN_FPU_SCALAR_BRANCH(f128, f128)

#define INSN_FPU_VECTOR_COMMON(TYPE, ITYPE, GROUP) \
    X(abs_          ## TYPE,   GROUP,  r2,    "absolute difference " #TYPE) \
    X(nabs_         ## TYPE,   GROUP,  r2,    "absolute value " #TYPE) \
    X(neg_          ## TYPE,   GROUP,  r2,    "negate " #TYPE) \
    X(add_          ## TYPE,   GROUP,  r3,    "add " #TYPE) \
    X(mul_          ## TYPE,   GROUP,  r3,    "multiply " #TYPE) \
    X(nabs_diff_    ## TYPE,   GROUP,  r3,    "negate absolute difference " #TYPE) \
    X(nadd_         ## TYPE,   GROUP,  r3,    "negate add " #TYPE) \
    X(nmul_         ## TYPE,   GROUP,  r3,    "negate multiply " #TYPE) \
    X(sub_          ## TYPE,   GROUP,  r3,    "subtract " #TYPE) \
    X(div_          ## TYPE,   GROUP,  r3,    "divide " #TYPE) \
    X(abs_diff_     ## TYPE,   GROUP,  r3,    "absolute difference " #TYPE) \
    X(rsqrt_        ## TYPE,   GROUP,  r2,    "reciprocal square root " #TYPE) \
    X(sqrt_         ## TYPE,   GROUP,  r2,    "square root " #TYPE) \
    X(max_          ## TYPE,   GROUP,  r3,    "maximum " #TYPE) \
    X(min_          ## TYPE,   GROUP,  r3,    "minimum " #TYPE) \
    X(maxnum_       ## TYPE,   GROUP,  r3,    "maximum number " #TYPE) \
    X(minnum_       ## TYPE,   GROUP,  r3,    "minimum number " #TYPE) \
    X(abs_max_      ## TYPE,   GROUP,  r3,    "absolute maximum " #TYPE) \
    X(abs_min_      ## TYPE,   GROUP,  r3,    "absolute minimum " #TYPE) \
    X(dot_          ## TYPE,   GROUP,  r3,    "dot-product " #TYPE) \
    X(round_        ## TYPE,   GROUP,  r2,    "round away from zero " #TYPE) \
    X(floor_        ## TYPE,   GROUP,  r2,    "round downward " #TYPE) \
    X(ceil_         ## TYPE,   GROUP,  r2,    "round upward " #TYPE) \
    X(trunk_        ## TYPE,   GROUP,  r2,    "round toward zero " #TYPE) \
    X(roundeven_    ## TYPE,   GROUP,  r2,    "round to nearest even " #TYPE) \
    X(nearbyint_    ## TYPE,   GROUP,  r2,    "round dynamic " #TYPE) \
    X(rint_         ## TYPE,   GROUP,  r2,    "round dynamic, detect inexact " #TYPE) \
    X(madd_         ## TYPE,   GROUP,  r4,    "multiply-add " #TYPE) \
    X(msub_         ## TYPE,   GROUP,  r4,    "multiply-subtract " #TYPE) \
    X(nmadd_        ## TYPE,   GROUP,  r4,    "negate multiply-add " #TYPE) \
    X(nmsub_        ## TYPE,   GROUP,  r4,    "negate multiply-subtract " #TYPE) \
    X(madd_alt_     ## TYPE,   GROUP,  r4,    "multiply-alternating add-subtract " #TYPE) \
    X(msub_alt_     ## TYPE,   GROUP,  r4,    "multiply-alternating subtract-add " #TYPE) \
    X(merge_        ## TYPE,   GROUP,  r4,    "merge " #TYPE) \
    X(merge_high_   ## TYPE,   GROUP,  r3,    "merge high parts " #TYPE) \
    X(merge_low_    ## TYPE,   GROUP,  r3,    "merge low parts " #TYPE) \
    X(add_alt_      ## TYPE,   GROUP,  r3,    "add alternating " #TYPE) \
    X(sub_alt_      ## TYPE,   GROUP,  r3,    "subtract alternating " #TYPE) \
    X(add_horiz_    ## TYPE,   GROUP,  r3,    "add horizontal " #TYPE) \
    X(sub_horiz_    ## TYPE,   GROUP,  r3,    "subtract horizontal " #TYPE) \
    X(mul_horiz_    ## TYPE,   GROUP,  r3,    "multiply horizontal " #TYPE) \
    X(pack_         ## TYPE,   GROUP,  r3,    "pack " #TYPE) \
    X(unpack_high_  ## TYPE,   GROUP,  r2,    "unpack high part " #TYPE) \
    X(unpack_low_   ## TYPE,   GROUP,  r2,    "unpack low part " #TYPE) \
    X(cmp_oeq_      ## TYPE,   GROUP,  r3,    "compare ordered and equal " #TYPE) \
    X(cmp_one_      ## TYPE,   GROUP,  r3,    "compare ordered and not-equal " #TYPE) \
    X(cmp_olt_      ## TYPE,   GROUP,  r3,    "compare ordered and less " #TYPE) \
    X(cmp_ole_      ## TYPE,   GROUP,  r3,    "compare ordered and less-equal " #TYPE) \
    X(cmp_o_        ## TYPE,   GROUP,  r3,    "compare ordered " #TYPE) \
    X(cmp_ueq_      ## TYPE,   GROUP,  r3,    "compare unordered or equal " #TYPE) \
    X(cmp_une_      ## TYPE,   GROUP,  r3,    "compare unordered or not-equal " #TYPE) \
    X(cmp_ult_      ## TYPE,   GROUP,  r3,    "compare unordered or less " #TYPE) \
    X(cmp_ule_      ## TYPE,   GROUP,  r3,    "compare unordered or less-equal " #TYPE) \
    X(cmp_u_        ## TYPE,   GROUP,  r3,    "compare unordered " #TYPE) \
    X(cvt_vi ## ITYPE ## _   ## TYPE,  GROUP,  r2, "convert vi" #ITYPE " to " #TYPE) \
    X(cvt_vu ## ITYPE ## _   ## TYPE,  GROUP,  r2, "convert vu" #ITYPE " to " #TYPE) \
    X(trunk_ ## TYPE  ## _vi ## ITYPE, GROUP,  r2, "trunk " #TYPE " to vi" #ITYPE) \
    X(trunk_ ## TYPE  ## _vu ## ITYPE, GROUP,  r2, "trunk " #TYPE " to vu" #ITYPE) \

INSN_FPU_VECTOR_COMMON(vf16, 16, f16)
INSN_FPU_VECTOR_COMMON(vf32, 32, f32)
INSN_FPU_VECTOR_COMMON(vf64, 64, f64)

//===------------------------------------------------------===//
// mixed fp types
//===------------------------------------------------------===//

X(scale_f128,  f128, r2s1, "scale f128")

X(cvt_f64_f32,  f64,  r2, "convert f64 to f32")
X(cvt_f128_f64, f128, r2, "convert f128 to f64")
X(cvt_f128_f32, f128, r2, "convert f128 to f32")
X(cvt_f32_f16,  f16,  r2, "convert f32 to f16")
X(cvt_f64_f16,  f64,  r2, "convert f64 to f16")
X(cvt_f128_f16, f128, r2, "convert f128 to f16")

X(extend_f16_f32,  f16,  r2, "extend f16 to f32")
X(extend_f16_f64,  f16,  r2, "extend f16 to f64")
X(extend_f16_f128, f128, r2, "extend f16 to f128")
X(extend_f32_f64,  f64,  r2, "extend f32 to f64")
X(extend_f32_f128, f128, r2, "extend f32 to f128")
X(extend_f64_f128, f128, r2, "extend f64 to f128")

//===------------------------------------------------------===//
// integer vectors
//===------------------------------------------------------===//
#define INSN_MMX_COMMON(STYPE, UTYPE) \
    X(addc_       ## UTYPE, mmx, r3,    "add carry-out " #UTYPE) \
    X(addo_       ## STYPE, mmx, r3,    "add overflow " #STYPE) \
    X(add_        ## UTYPE, mmx, r3,    "add " #UTYPE) \
    X(add_sat_    ## STYPE, mmx, r3,    "add saturate " #STYPE) \
    X(add_sat_    ## UTYPE, mmx, r3,    "add saturate " #UTYPE) \
    X(avg_        ## STYPE, mmx, r3,    "average " #STYPE) \
    X(avg_        ## UTYPE, mmx, r3,    "average " #UTYPE) \
    X(cmp_eq_     ## STYPE, mmx, r3,    "compare equal " #STYPE) \
    X(cmp_lt_     ## STYPE, mmx, r3,    "compare less " #STYPE) \
    X(cmp_lt_     ## UTYPE, mmx, r3,    "compare less " #UTYPE) \
    X(max_        ## STYPE, mmx, r3,    "maximum " #STYPE) \
    X(max_        ## UTYPE, mmx, r3,    "maximum " #UTYPE) \
    X(min_        ## STYPE, mmx, r3,    "minimum " #STYPE) \
    X(min_        ## UTYPE, mmx, r3,    "minimum " #UTYPE) \
    X(merge_high_ ## UTYPE, mmx, r3,    "merge high " #UTYPE) \
    X(merge_low_  ## UTYPE, mmx, r3,    "merge low " #UTYPE) \
    X(rol_        ## UTYPE, mmx, r3,    "rotate left " #UTYPE) \
    X(ror_        ## UTYPE, mmx, r3,    "rotate right " #UTYPE) \
    X(sll_        ## UTYPE, mmx, r3,    "shift left logical " #UTYPE) \
    X(sll_imm_    ## UTYPE, mmx, r2s1,  "shift left logical immediate " #UTYPE) \
    X(sra_        ## STYPE, mmx, r3,    "shift right algebraic " #STYPE) \
    X(sra_imm_    ## STYPE, mmx, r2s1,  "shift right algebraic immediate " #STYPE) \
    X(srl_        ## UTYPE, mmx, r3,    "shift right logical " #UTYPE) \
    X(srl_imm_    ## UTYPE, mmx, r2s1,  "shift right logical immediate " #UTYPE) \
    X(subb_       ## UTYPE, mmx, r3,    "subtract borrow " #UTYPE) \
    X(subo_       ## STYPE, mmx, r3,    "subtract overflow " #STYPE) \
    X(sub_        ## UTYPE, mmx, r3,    "subtract " #UTYPE) \
    X(sub_sat_    ## STYPE, mmx, r3,    "subtract saturated " #STYPE) \
    X(sub_sat_    ## UTYPE, mmx, r3,    "subtract saturated " #UTYPE) \

INSN_MMX_COMMON( vi8,  vu8)
INSN_MMX_COMMON(vi16, vu16)
INSN_MMX_COMMON(vi32, vu32)
INSN_MMX_COMMON(vi64, vu64)

//===------------------------------------------------------===//
// custom vector
//===------------------------------------------------------===//
X(pack_sat_vi16,  mmx, r3, "pack saturated vi16")
X(pack_sat_vu16,  mmx, r3, "pack saturated vu16")
X(pack_sat_vi32,  mmx, r3, "pack saturated vi32")
X(pack_sat_vu32,  mmx, r3, "pack saturated vu32")
X(pack_sat_vi64,  mmx, r3, "pack saturated vi64")
X(pack_sat_vu64,  mmx, r3, "pack saturated vu64")

X(pack_usat_vi16, mmx, r3, "pack unsigned saturated vi16")
X(pack_usat_vi32, mmx, r3, "pack unsigned saturated vi32")
X(pack_usat_vi64, mmx, r3, "pack unsigned saturated vi64")

X(pack_mod_vu16,  mmx, r3, "pack unsigned modulo vu16")
X(pack_mod_vu32,  mmx, r3, "pack unsigned modulo vu32")
X(pack_mod_vu64,  mmx, r3, "pack unsigned modulo vu64")

X(unpack_low_vi8,   mmx, r2, "unpack low vi8")
X(unpack_low_vi16,  mmx, r2, "unpack low vi16")
X(unpack_low_vi32,  mmx, r2, "unpack low vi32")
X(unpack_low_vu8,   mmx, r2, "unpack low vu8")
X(unpack_low_vu16,  mmx, r2, "unpack low vu16")
X(unpack_low_vu32,  mmx, r2, "unpack low vu32")

X(unpack_high_vi8,   mmx, r2, "unpack high vi8")
X(unpack_high_vi16,  mmx, r2, "unpack high vi16")
X(unpack_high_vi32,  mmx, r2, "unpack high vi32")
X(unpack_high_vu8,   mmx, r2, "unpack high vu8")
X(unpack_high_vu16,  mmx, r2, "unpack high vu16")
X(unpack_high_vu32,  mmx, r2, "unpack high vu32")


//===------------------------------------------------------===//
// end macrotable
//===------------------------------------------------------===//
