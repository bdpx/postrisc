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
//    floating-point quadruple (fpuq),
//    floating-point double scalar/packed (fpud),
//    floating-point single scalar/packed (fpus),
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

//===------------------------------------------------------===//
// add/subtract
//===------------------------------------------------------===//
X("add",       add,     base, r3, "add")
X("add.add",   add_add,  base, r4, "add 3 values")
X("add.add.c", add_addc, base, r4, "add-add with carry-out")
X("addc",      addc,    base, r3, "add carry-out")
X("addi",      addi,    base, RabSI21, "add immediate")
X("addi.ws",   addi_ws, base, RabSI21, "add immediate word, sext")
X("addi.wz",   addi_wz, base, RabSI21, "add immediate word, zext")
X("addo",      addo,    base, r3, "add overflow")
X("add.sub",   add_sub, base, r4, "add-subtract")
X("add.ws",    add_ws,  base, r3, "add word sign-extend")
X("add.wz",    add_wz,  base, r3, "add word zero-extend")
X("add.q",     add_q,   intq, r3, "add quadword")

X("sub",       sub,      base, r3, "subtract")
X("subb",      subb,     base, r3, "subtract with borrow")
X("subfi",     subfi,    base, RabSI21, "subtract from immediate")
X("subfi.ws",  subfi_ws, base, RabSI21, "subtract from immediate word, sext")
X("subfi.wz",  subfi_wz, base, RabSI21, "subtract from immediate word, zext")
X("subo",      subo,     base, r3, "subtract overflow")
X("sub.sub",   sub_sub,  base, r4, "subtract twice")
X("sub.sub.b", sub_subb, base, r4, "subtract-subtract with borrow-out")
X("sub.ws",    sub_ws,   base, r3, "subtract word sign-extend")
X("sub.wz",    sub_wz,   base, r3, "subtract word zero-extend")
X("sub.q",     sub_q,    intq, r3, "subtract quadword")

//===------------------------------------------------------===//
// bitwise
//===------------------------------------------------------===//
X("and",     and,     base, r3, "and bitwise")
X("and.dec", and_dec, base, r3, "and bitwise decremented")
X("andn",    andn,    base, r3, "and not bitwise")
X("and.neg", and_neg, base, r3, "and bitwise negate")
X("nand",    nand,    base, r3, "bitwise not and")
X("nor",     nor,     base, r3, "bitwise not or")
X("or",      or,      base, r3, "bitwise or")
X("orn",     orn,     base, r3, "bitwise or not")
X("xnor",    xnor,    base, r3, "bitwise exclusive nor")
X("xor",     xor,     base, r3, "bitwise exclusive or")
X("xor.dec", xor_dec, base, r3, "bitwise exclusive-or decremented")

X("andi",  andi,    base, RabSI21, "and bitwise with immediate")
X("andni", andni,   base, RabSI21, "and not bitwise with immediate")
X("ori",   ori,     base, RabSI21, "bitwise or with immediate")
X("orni",  orni,    base, RabSI21, "bitwise or not immediate")
X("xori",  xori,    base, RabSI21, "bitwise exclusive or with immediate")
X("not",   not,     base, Not, "bitwise not")

X("nop", nop,     base, RaxU28,   "no operation")

//===------------------------------------------------------===//
//
//===------------------------------------------------------===//
X("absd",     absd,    base,  r3, "absolute difference")
X("absd.w",   absd_w,  base,  r3, "absolute difference word")
X("algnup",   algnup,  base,  RabScd,  "align up shifted")
X("dep",      dep,     base,  RabcSde, "deposit")
X("dep.q",    dep_q,   intq,  FabcSd,  "deposit quadword")
X("dep.a",    dep_a,   base,  RabScd,  "deposit alter")
X("dep.c",    dep_c,   base,  RabScd,  "deposit clear")
X("dep.s",    dep_s,   base,  RabScd,  "deposit set")
X("ldi",      ldi,     base,  ldi,     "load immediate")
X("ldi.h",    ldih,    base,  ldi,     "load immediate high")
X("mov",      mov,     base,  r2, "move general register")
X("mov2",     mov2,    base,  r4, "move 2 general registers")
X("mul",      mul,     base,  r3, "multiply")
X("mul.ws",   mul_ws,  base,  r3, "multiply word sign extend")
X("mul.wz",   mul_wz,  base,  r3, "multiply word zero extend")
X("mul.add",  mul_add,  base,  r4, "multiply-add")
X("mul.h",    mulh,    base,  r3, "multiply high")
X("muli",     muli,    base,  RabSI21,  "multiply immediate")
X("muli.ws",  muli_ws, base,  RabSI21,  "multiply word sign extended immediate")
X("muli.wz",  muli_wz, base,  RabUI21,  "multiply word zero extended immediate")
X("mul.sub",  mul_sub,  base,  r4, "multiply-subtract")
X("mul.subf", mul_subf, base,  r4, "multiply-subtract from")
X("rlmi",     rlmi,    base,  RabScde, "rotate left and mask immediate")
X("mbsel",    mbsel,   base,  r4, "masked bit selection")

//===------------------------------------------------------===//
// conditional move
//===------------------------------------------------------===//
X("cmov.lsb",  cmov_lsb, base, r4, "conditional move if least significand bit")
X("cmov.eq.w", cmov_eq_w, base, r4, "conditional move word if equal zero")
X("cmov.lt.w", cmov_lt_w, base, r4, "conditional move word if less than zero")
X("cmov.le.w", cmov_le_w, base, r4, "conditional move word if less than or equal zero")
X("cmov.eq.d", cmov_eq_d, base, r4, "conditional move doubleword if equal zero")
X("cmov.lt.d", cmov_lt_d, base, r4, "conditional move doubleword if less than zero")
X("cmov.le.d", cmov_le_d, base, r4, "conditional move doubleword if less than or equal zero")

//===------------------------------------------------------===//
// shifts
//===------------------------------------------------------===//
X("sll",    sll,     base, r3,    "shift left logical")
X("sll.w",  sll_w,   base, r3,    "shift left logical word")
X("slli",   slli,    base, RabSc, "shift left logical immediate")
X("slli.q", slli_q,  intq, RabSc, "sllqi")
X("slli.w", slli_w,  base, RabSc, "shift left logical immediate word")

X("srl",    srl,     base, r3,    "shift right logical")
X("srl.w",  srl_w,   base, r3,    "shift right logical word")
X("srli",   srli,    base, RabSc, "shift right logical immediate")
X("srli.q", srli_q,  intq, RabSc, "srlqi")
X("srli.w", srli_w,  base, RabSc, "shift right logical immediate word")

X("sra",    sra,     base, r3,    "shift right algebraic")
X("sra.w",  sra_w,   base, r3,    "shift right algebraic word")
X("srai",   srai,    base, RabSc, "shift right algebraic immediate")
X("srai.q", srai_q,  intq, RabSc, "sraqi")
X("srai.w", srai_w,  base, RabSc, "shift right algebraic immediate word")

X("srd",    srd,     base, r3,    "shift right dividing")
X("srdi",   srdi,    base, RabSc, "shift right dividing immediate")

X("slsra",   slsra,   base, r4,     "shift left and shift right algebraic")
X("slsrai",  slsrai,  base, RabScd, "shift left and shift right algebraic immediate")
X("slsrl",   slsrl,   base, r4,     "shift left and shift right logical")
X("slsrli",  slsrli,  base, RabScd, "shift left and shift right logical immediate")
X("slsrl.w", slsrl_w, base, r4,     "shift left and shift right logical word")
X("slsra.w", slsra_w, base, r4,     "shift left and shift right algebraic word")

X("sl.add",     sl_add,      base, RabcSd, "shift left and add")
X("sl.add.ws",  sl_add_ws,   base, RabcSd, "shift left and add word sign-extend")
X("sl.add.wz",  sl_add_wz,   base, RabcSd, "shift left and add word zero-extend")
X("sl.sub",     sl_sub,      base, RabcSd, "shift left and subtract")
X("sl.sub.ws",  sl_sub_ws,   base, RabcSd, "shift left and subtract word sign-extend")
X("sl.sub.wz",  sl_sub_wz,   base, RabcSd, "shift left and subtract word zero-extend")
X("sl.subf",    sl_subf,     base, RabcSd, "shift left and subtract from")
X("sl.subf.ws", sl_subf_ws,  base, RabcSd, "shift left and subtract from word sign-extend")
X("sl.subf.wz", sl_subf_wz,  base, RabcSd, "shift left and subtract from zero sign-extend")
X("sl.or",      sl_or,       base, RabcSd, "shift left and or")
X("sl.xor",     sl_xor,      base, RabcSd, "shift left and xor")

X("slp",    slp,      base, r4, "shift left pair")
X("slp.w",  slp_w,    base, r4, "shift left pair word")
X("slp.q",  slp_q,    intq, r4, "shift left pair quadword")
X("srp",    srp,      base, r4, "shift right pair")
X("srp.w",  srp_w,    base, r4, "shift right pair word")
X("srp.q",  srp_q,    intq, r4, "shift right pair quadword")
X("srpi",   srpi,     base, RabcSd, "shift right pair immediate")
X("srpi.w", srpi_w,   base, RabcSd, "shift right pair immediate word")
X("srpi.q", srpi_q,   intq, FabcSd, "shift right pair immediate quadword")

//===------------------------------------------------------===//
// divide/modulo
//===------------------------------------------------------===//
X("div",    div,     base, r3, "divide")
X("div.w",  div_w,   base, r3, "divide word")
X("divi",   divi,    base, RabSI21, "divide immediate")
X("divu",   divu,    base, r3, "divide unsigned")
X("divu.w", divu_w,  base, r3, "divide unsigned word")
X("divui",  divui,   base, RabUI21, "divide unsigned immediate")
X("mod",    mod,     base, r3, "module")
X("mod.w",  mod_w,   base, r3, "module word")
X("modi",   modi,    base, RabSI21, "module immediate")
X("modu",   modu,    base, r3, "module unsigned")
X("modu.w", modu_w,  base, r3, "module unsigned word")
X("modui",  modui,   base, RabUI21, "module unsigned immediate")

//===------------------------------------------------------===//
// min/max
//===------------------------------------------------------===//
X("mins.w", mins_w,    base, r3, "minimum signed word")
X("maxs.w", maxs_w,    base, r3, "maximum signed word")
X("minu.w", minu_w,    base, r3, "minimum unsigned word")
X("maxu.w", maxu_w,    base, r3, "maximum unsigned word")

X("mins", mins,    base, r3, "minimum signed")
X("maxs", maxs,    base, r3, "maximum signed")
X("minu", minu,    base, r3, "minimum unsigned")
X("maxu", maxu,    base, r3, "maximum unsigned")

X("minsi", minsi,   base, RabSI21, "minimum signed immediate")
X("maxsi", maxsi,   base, RabSI21, "maximum signed immediate")
X("minui", minui,   base, RabUI21, "minimum unsigned immediate")
X("maxui", maxui,   base, RabUI21, "maximum unsigned immediate")

//===------------------------------------------------------===//
// compare 64 bit
//===------------------------------------------------------===//
#define INSN_BASE_TYPED(NAME, TYPE) \
    X("cmp.eq."   #NAME, cmp_eq_   ## NAME,  base,        r3,        "compare " TYPE " equal") \
    X("cmp.ne."   #NAME, cmp_ne_   ## NAME,  base,        r3,        "compare " TYPE " not equal") \
    X("cmps.lt."  #NAME, cmps_lt_  ## NAME,  base,        r3,        "compare signed " TYPE " less") \
    X("cmpu.lt."  #NAME, cmpu_lt_  ## NAME,  base,        r3,        "compare unsigned " TYPE " less") \
    X("cmps.le."  #NAME, cmps_le_  ## NAME,  base,        r3,        "compare " TYPE " less or equal") \
    X("cmpu.le."  #NAME, cmpu_le_  ## NAME,  base,        r3,        "compare unsigned " TYPE " less or equal") \
    X("cmpi.eq."  #NAME, cmpi_eq_  ## NAME,  base,        RabSI21,   "compare " TYPE " equal immediate") \
    X("cmpi.ne."  #NAME, cmpi_ne_  ## NAME,  base,        RabSI21,   "compare " TYPE " not equal immediate") \
    X("cmpsi.lt." #NAME, cmpsi_lt_ ## NAME,  base,        RabSI21,   "compare signed " TYPE " less immediate") \
    X("cmpsi.gt." #NAME, cmpsi_gt_ ## NAME,  base,        RabSI21,   "compare signed " TYPE " greater immediate") \
    X("cmpui.lt." #NAME, cmpui_lt_ ## NAME,  base,        RabUI21,   "compare unsigned " TYPE " less immediate") \
    X("cmpui.gt." #NAME, cmpui_gt_ ## NAME,  base,        RabUI21,   "compare unsigned " TYPE " greater immediate") \
    X("cmps.gt."  #NAME, cmps_gt_  ## NAME,  base,        Racb,      "compare signed " TYPE " greater") /*pseudo*/ \
    X("cmpu.gt."  #NAME, cmpu_gt_  ## NAME,  base,        Racb,      "compare unsigned " TYPE " greater") /*pseudo*/ \
    X("cmps.ge."  #NAME, cmps_ge_  ## NAME,  base,        Racb,      "compare signed " TYPE " greater or equal") /*pseudo*/ \
    X("cmpu.ge."  #NAME, cmpu_ge_  ## NAME,  base,        Racb,      "compare unsigned " TYPE " greater or equal") /*pseudo*/ \
    X("cmpsi.ge." #NAME, cmpsi_ge_ ## NAME,  base,        RabSI21M,  "compare signed " TYPE " greater or equal immediate") /*pseudo*/ \
    X("cmpsi.le." #NAME, cmpsi_le_ ## NAME,  base,        RabSI21P,  "compare signed " TYPE " less or equal immediate") /*pseudo*/ \
    X("cmpui.ge." #NAME, cmpui_ge_ ## NAME,  base,        RabUI21M,  "compare unsigned " TYPE " greater or equal immediate") /*pseudo*/ \
    X("cmpui.le." #NAME, cmpui_le_ ## NAME,  base,        RabUI21P,  "compare unsigned " TYPE " less or equal immediate") /*pseudo*/ \
    X("b.eq."     #NAME, b_eq_     ## NAME,  branch,      RabBBS,    "branch if " TYPE " equal") \
    X("b.ne."     #NAME, b_ne_     ## NAME,  branch,      RabBBS,    "branch if " TYPE " not equal") \
    X("bs.lt."    #NAME, bs_lt_    ## NAME,  branch,      RabBBS,    "branch if " TYPE " less") \
    X("bu.lt."    #NAME, bu_lt_    ## NAME,  branch,      RabBBS,    "branch if " TYPE " less unsigned") \
    X("bs.le."    #NAME, bs_le_    ## NAME,  branch,      RabBBS,    "branch if " TYPE " less or equal") \
    X("bu.le."    #NAME, bu_le_    ## NAME,  branch,      RabBBS,    "branch if " TYPE " less or equal unsigned") \
    X("bs.gt."    #NAME, bs_gt_    ## NAME,  branch,      RbaBBS,    "branch if " TYPE " greater") /*pseudo*/ \
    X("bu.gt."    #NAME, bu_gt_    ## NAME,  branch,      RbaBBS,    "branch if " TYPE " greater unsigned") /*pseudo*/ \
    X("bs.ge."    #NAME, bs_ge_    ## NAME,  branch,      RbaBBS,    "branch if " TYPE " greater or equal") /*pseudo*/ \
    X("bu.ge."    #NAME, bu_ge_    ## NAME,  branch,      RbaBBS,    "branch if " TYPE " greater or equal unsigned") /*pseudo*/ \
    X("bi.eq."    #NAME, bi_eq_    ## NAME,  branch,      RaSIBBS,   "branch if " TYPE " equal immediate") \
    X("bi.ne."    #NAME, bi_ne_    ## NAME,  branch,      RaSIBBS,   "branch if " TYPE " not equal immediate") \
    X("bsi.lt."   #NAME, bsi_lt_   ## NAME,  branch,      RaSIBBS,   "branch if " TYPE " less immediate") \
    X("bsi.ge."   #NAME, bsi_ge_   ## NAME,  branch,      RaSIBBS,   "branch if " TYPE " greater or equal immediate") \
    X("bsi.le."   #NAME, bsi_le_   ## NAME,  branch,      RaSIPBBS,  "branch if " TYPE " less or equal immediate") /*pseudo*/ \
    X("bsi.gt."   #NAME, bsi_gt_   ## NAME,  branch,      RaSIPBBS,  "branch if " TYPE " greater immediate") /*pseudo*/ \
    X("bui.lt."   #NAME, bui_lt_   ## NAME,  branch,      RaUIBBS,   "branch if " TYPE " less unsigned immediate") \
    X("bui.ge."   #NAME, bui_ge_   ## NAME,  branch,      RaUIBBS,   "branch if " TYPE " greater or equal unsigned immediate") \
    X("bui.le."   #NAME, bui_le_   ## NAME,  branch,      RaUIPBBS,  "branch if " TYPE " less or equal unsigned immediate") /*pseudo*/ \
    X("bui.gt."   #NAME, bui_gt_   ## NAME,  branch,      RaUIPBBS,  "branch if " TYPE " greater unsigned immediate") /*pseudo*/ \
    X("nul.eq."   #NAME, nul_eq_   ## NAME,  nullifying,  r2m2,     "nullify if " TYPE " equal") \
    X("nul.ne."   #NAME, nul_ne_   ## NAME,  nullifying,  r2m2,     "nullify if " TYPE " not-equal") \
    X("nuls.lt."  #NAME, nuls_lt_  ## NAME,  nullifying,  r2m2,     "nullify if " TYPE " less") \
    X("nuls.le."  #NAME, nuls_le_  ## NAME,  nullifying,  r2m2,     "nullify if " TYPE " less or equal") \
    X("nulu.lt."  #NAME, nulu_lt_  ## NAME,  nullifying,  r2m2,     "nullify if " TYPE " less unsigned") \
    X("nulu.le."  #NAME, nulu_le_  ## NAME,  nullifying,  r2m2,     "nullify if " TYPE " less or equal unsigned") \
    X("nuli.eq."  #NAME, nuli_eq_  ## NAME,  nullifying,  RaSIm2,   "nullify if " TYPE " equal immediate") \
    X("nuli.ne."  #NAME, nuli_ne_  ## NAME,  nullifying,  RaSIm2,   "nullify if " TYPE " not-equal immediate") \
    X("nulsi.lt." #NAME, nulsi_lt_ ## NAME,  nullifying,  RaSIm2,   "nullify if " TYPE " less immediate") \
    X("nulsi.gt." #NAME, nulsi_gt_ ## NAME,  nullifying,  RaSIm2,   "nullify if " TYPE " greater immediate") \
    X("nului.lt." #NAME, nului_lt_ ## NAME,  nullifying,  RaUIm2,   "nullify if " TYPE " less unsigned immediate") \
    X("nului.gt." #NAME, nului_gt_ ## NAME,  nullifying,  RaUIm2,   "nullify if " TYPE " greater unsigned immediate") \

INSN_BASE_TYPED(w, "word")
INSN_BASE_TYPED(d, "doubleword")

//===------------------------------------------------------===//
// compare 128 bit
//===------------------------------------------------------===//
X("cmp.eq.q",  cmp_eq_q,   intq, r3, "compare quadword equal")
X("cmp.ne.q",  cmp_ne_q,   intq, r3, "compare quadword not equal")
X("cmps.lt.q", cmps_lt_q,  intq, r3, "compare signed quadword less")
X("cmps.le.q", cmps_le_q,  intq, r3, "compare signed quadword less or equal")
X("cmpu.lt.q", cmpu_lt_q,  intq, r3, "compare unsigned quadword less")
X("cmpu.le.q", cmpu_le_q,  intq, r3, "compare unsigned quadword less or equal")
// pseudos
X("cmps.gt.q", cmps_gt_q,  intq, r3, "compare signed quadword greater")
X("cmps.ge.q", cmps_ge_q,  intq, r3, "compare signed quadword greater or equal")
X("cmpu.gt.q", cmpu_gt_q,  intq, r3, "compare unsigned quadword greater")
X("cmpu.ge.q", cmpu_ge_q,  intq, r3, "compare unsigned quadword greater or equal")

//===------------------------------------------------------===//
// bit manipulation/counting
//===------------------------------------------------------===//
X("cnt.lz",  cnt_lz,   bitmanip, RabSc, "count leading zeros")
X("cnt.tz",  cnt_tz,   bitmanip, RabSc, "count trailing zeros")
X("cnt.pop", cnt_pop,  bitmanip, RabSc, "count population")

X("gtb",     gtb,     bitmanip, r2, "graycode to binary")
X("perm",    perm,    bitmanip, r4, "permute bytes")
X("permb",   permb,   bitmanip, RabSc, "permute bits")
X("mb.gath", mbgath,  bitmanip, r3, "masked bit gather")
X("mb.scat", mbscat,  bitmanip, r3, "masked bit scatter")

//===------------------------------------------------------===//
// branch/nullify if mask/bit set/clear
//===------------------------------------------------------===//
X("b.bc",  b_bc,   branch, RabBBS,   "branch if bit clear")
X("b.bci", b_bci,  branch, RaSbBBS,  "branch if bit clear immediate")
X("b.bs",  b_bs,   branch, RabBBS,   "branch if bit set")
X("b.bsi", b_bsi,  branch, RaSbBBS,  "branch if bit set immediate")

X("bm.all",    bm_all,    branch, RaUIBBS,  "branch if mask immediate all bits set")
X("bm.any",    bm_any,    branch, RaUIBBS,  "branch if mask immediate any bit set")
X("bm.none",   bm_none,   branch, RaUIBBS,  "branch if mask immediate none bit set")
X("bm.notall", bm_notall, branch, RaUIBBS,  "branch if mask immediate not all bits set")

X("nul.bc",  nul_bc,      nullifying, r2m2,   "nullify if bit clear")
X("nul.bci", nul_bci,     nullifying, RaSbm2, "nullify if bit clear immediate")
X("nul.bs",  nul_bs,      nullifying, r2m2,   "nullify if bit set")
X("nul.bsi", nul_bsi,     nullifying, RaSbm2, "nullify if bit set immediate")

X("nulm.all",    nulm_all,    nullifying, RaUIm2, "nullify if mask all bit set")
X("nulm.notall", nulm_notall, nullifying, RaUIm2, "nullify if mask not all bit set")
X("nulm.none",   nulm_none,   nullifying, RaUIm2, "nullify if mask none bit set")
X("nulm.any",    nulm_any,    nullifying, RaUIm2, "nullify if mask any bit set")

//===------------------------------------------------------===//
// loops
//===------------------------------------------------------===//
X("reps.lt.d",  reps_lt_d,  branch, RabBBS, "repeat on signed less doubleword")
X("reps.le.d",  reps_le_d,  branch, RabBBS, "repeat on signed less or equal doubleword")
X("reps.gt.d",  reps_gt_d,  branch, RabBBS, "repeat on signed greater doubleword")
X("reps.ge.d",  reps_ge_d,  branch, RabBBS, "repeat on signed greater or equal doubleword")
X("repu.lt.d",  repu_lt_d,  branch, RabBBS, "repeat on unsigned less doubleword")
X("repu.le.d",  repu_le_d,  branch, RabBBS, "repeat on unsigned less or equal doubleword")
X("repu.gt.d",  repu_gt_d,  branch, RabBBS, "repeat on unsigned greater doubleword")
X("repu.ge.d",  repu_ge_d,  branch, RabBBS, "repeat on unsigned greater or equal doubleword")

//===------------------------------------------------------===//
// unconditional jumps
//===------------------------------------------------------===//
X("jmp",      jmp,      branch, br,        "jump relative")
X("jmp.r",    jmp_r,    branch, RbcScale,  "jump register indirect")
X("jmp.t",    jmp_t,    branch, Rbc,       "jump table")
X("jmp.t.ws", jmp_t_ws, branch, Rbc,       "jump table word signed index")
X("jmp.t.wz", jmp_t_wz, branch, Rbc,       "jump table word unsigned index")
X("jmp.mi",   jmp_mi,   special, RbcScale, "jmpmi") // workaround

//===------------------------------------------------------===//
// call/return/regframe
//===------------------------------------------------------===//
X("call.mi",  call_mi,  branch, RabS10,    "call memory indirect")
X("call.mrw", call_mrw, branch, RabS10,    "call memory indirect relative word")
X("call.r",   call_r,   branch, callr,     "call relative")
X("call.ri",  call_ri,  branch, r3,        "call register indirect")
X("call.rvt", call_rvt, branch, RabS10,   "call relative vtable")
X("call.plt", call_plt, branch, gmemr,    "call procedure linkage table")
X("ret",      ret,      branch, NoArgs,    "return from subroutine")
X("ret.f",    retf,     branch, RaxUI21,   "return from subroutine and rollback stack frame")

//===------------------------------------------------------===//
// special
//===------------------------------------------------------===//
X("alloc",    alloc,    special, alloc,    "allocate register frame and update eip")
X("alloc.sp", alloc_sp, special, alloc_sp, "allocate register frame, update sp, update eip")
X("syscall",  syscall,  special, NoArgs,   "system call")
X("undef",    undef,    special, NoArgs,   "undefined instruction")
X("random",   random,   special, r2,       "random")
X("mf.spr",   mfspr,    special, mspr,     "move from special-purpose register")
X("mt.spr",   mtspr,    special, mspr,     "move to special-purpose register")
X("icb.i",    icbi,     special, RaxbSI21, "instruction cache block invalidate")
X("int",      int,      special, RbS10,    "interrupt")
X("m.probe",  mprobe,   special, r3,       "memory probe access")
X("cpuid",    cpuid,    special, RabS10,   "cpu identification")
X("dcb.f",    dcbf,     special, RaxbSI21, "data cache block flush")
X("dcb.t",    dcbt,     special, RaxbSI21, "data cache block touch")

//===------------------------------------------------------===//
// software exceptions
//===------------------------------------------------------===//
X("eh.adj",   eh_adj,    special, br,      "exception handler adjust eip")
X("eh.catch", eh_catch,  special, RaxbBBS, "exception handler catch")
X("eh.next",  eh_next,   special, RaxbBBS, "exception handler next")
X("eh.throw", eh_throw,  special, RaxbSI21, "exception handler throw")

//===------------------------------------------------------===//
// software emulation
//===------------------------------------------------------===//
X("write", write,    special, write,   "write string formatted")

//===------------------------------------------------------===//
// privileged
//===------------------------------------------------------===//
X("sysret", sysret,  privileged, NoArgs,   "system return")
X("rfi",    rfi,     privileged, NoArgs,   "return from interruption")
X("rs.cover", rscover, privileged, NoArgs,   "register stack cover")
X("rs.flush", rsflush, privileged, NoArgs,   "register stack flush")
X("rs.load",  rsload,  privileged, NoArgs,   "register stack load")
X("halt", halt,    privileged, NoArgs,   "halt processor")
X("dcb.i", dcbi,    privileged, RaxbSI21, "data cache block invalidate")

//===------------------------------------------------------===//
// privileged, TLB
//===------------------------------------------------------===//
X("tpa", tpa, privileged, r2, "translate to physical address")
X("ptc", ptc, privileged, r3, "purge translation cache by page address and ASID")

X("mt.dtr", mtdtr, privileged, r3, "move to data translation register")
X("mt.itr", mtitr, privileged, r3, "move to instruction translation register")

//===------------------------------------------------------===//
// debug/monitoring
//===------------------------------------------------------===//
X("mf.ibr", mfibr,  privileged, RabS10, "move from instruction breakpoint register")
X("mt.ibr", mtibr,  privileged, RabS10, "move to instruction breakpoint register")
X("mf.dbr", mfdbr,  privileged, RabS10, "move from data breakpoint register")
X("mt.dbr", mtdbr,  privileged, RabS10, "move to data breakpoint register")
X("mf.mr",  mfmr,   privileged, RabS10, "move from monitor register")
X("mt.mr",  mtmr,   privileged, RabS10, "move to monitor register")

//===------------------------------------------------------===//
// load/store relative/indexed
//===------------------------------------------------------===//
X("ca.r",    ca_r,    base,  ca_r,    "compute address relative")
X("ca.rf",   ca_rf,   base,  gmemr,   "compute address relative forward")
X("ca.xd",   ca_xds,  base,  gmemx,   "compute address indexed dword")
X("ca.xw",   ca_xws,  base,  gmemx,   "compute address indexed word")
X("ca.xuw",  ca_xwz,  base,  gmemx,   "compute address indexed unsigned word")
X("ca.n",    ca_n,    base,  RabSI21, "compute address near")
X("ca.nrc",  ca_nrc,  base,  RabSI21, "compute address near relative")


#define INSN_LOAD_STORE(NAME, TYPE) \
    X("lds." #NAME ,       lds_ ## NAME,          memory, RabSI21, "load "  TYPE " sext") \
    X("ldz." #NAME ,       ldz_ ## NAME,          memory, RabSI21, "load "  TYPE " zext") \
    X("st."  #NAME     ,   st_  ## NAME,          memory, RabSI21, "store " TYPE) \
    X("lds." #NAME ".r",   lds_ ## NAME ## _r,    memory, gmemr,   "load "  TYPE " sext relative") \
    X("ldz." #NAME ".r",   ldz_ ## NAME ## _r,    memory, gmemr,   "load "  TYPE " zext relative") \
    X("st."  #NAME ".r",   st_  ## NAME ## _r,    memory, gmemr,   "store " TYPE " relative") \
    X("lds." #NAME ".xd",  lds_ ## NAME ## _xds,  memory, gmemx,   "load "  TYPE " sext indexed dword") \
    X("ldz." #NAME ".xd",  ldz_ ## NAME ## _xds,  memory, gmemx,   "load "  TYPE " zext indexed dword") \
    X("st."  #NAME ".xd",  st_  ## NAME ## _xds,  memory, gmemx,   "store " TYPE " indexed dword") \
    X("lds." #NAME ".xw",  lds_ ## NAME ## _xws,  memory, gmemx,   "load "  TYPE " sext indexed word") \
    X("ldz." #NAME ".xw",  ldz_ ## NAME ## _xws,  memory, gmemx,   "load "  TYPE " zext indexed word") \
    X("st."  #NAME ".xw",  st_  ## NAME ## _xws,  memory, gmemx,   "store " TYPE " indexed word") \
    X("lds." #NAME ".xuw", lds_ ## NAME ## _xwz,  memory, gmemx,   "load "  TYPE " sext indexed unsigned word") \
    X("ldz." #NAME ".xuw", ldz_ ## NAME ## _xwz,  memory, gmemx,   "load "  TYPE " zext indexed unsigned word") \
    X("st."  #NAME ".xuw", st_  ## NAME ## _xwz,  memory, gmemx,   "store " TYPE " indexed unsigned word") \
    X("lds." #NAME ".mia", lds_ ## NAME ## _mia,  memory, gmemu,   "load "  TYPE " sext and modify immediate after") \
    X("ldz." #NAME ".mia", ldz_ ## NAME ## _mia,  memory, gmemu,   "load "  TYPE " zext and modify immediate after") \
    X("st."  #NAME ".mia", st_  ## NAME ## _mia,  memory, gmemu,   "store " TYPE " and modify immediate after") \
    X("lds." #NAME ".mib", lds_ ## NAME ## _mib,  memory, gmemu,   "load "  TYPE " sext and modify immediate before") \
    X("ldz." #NAME ".mib", ldz_ ## NAME ## _mib,  memory, gmemu,   "load "  TYPE " zext and modify immediate before") \
    X("st."  #NAME ".mib", st_  ## NAME ## _mib,  memory, gmemu,   "store " TYPE " and modify immediate before") \

INSN_LOAD_STORE(b, "byte")
INSN_LOAD_STORE(h, "halfword")
INSN_LOAD_STORE(w, "word")
INSN_LOAD_STORE(d, "doubleword")

X("ld.q",     ld_q,      memory, RabSI21, "load quadword")
X("st.q",     st_q,      memory, RabSI21, "store quadword")
X("ld.q.r",   ld_q_r,    memory, gmemr,   "load quadword relative")
X("st.q.r",   st_q_r,    memory, gmemr,   "store quadword relative")
X("ld.q.xd",  ld_q_xds,  memory, gmemx,   "load quadword indexed dword")
X("st.q.xd",  st_q_xds,  memory, gmemx,   "store quadword indexed dword")
X("ld.q.xw",  ld_q_xws,  memory, gmemx,   "load quadword indexed word")
X("st.q.xw",  st_q_xws,  memory, gmemx,   "store quadword indexed word")
X("ld.q.xuw", ld_q_xwz,  memory, gmemx,   "load quadword indexed unsigned word")
X("st.q.xuw", st_q_xwz,  memory, gmemx,   "store quadword indexed unsigned word")
X("ld.q.mia", ld_q_mia,  memory, gmemu,   "load quadword and modify immediate after")
X("st.q.mia", st_q_mia,  memory, gmemu,   "store quadword and modify immediate after")
X("ld.q.mib", ld_q_mib,  memory, gmemu,   "load quadword and modify immediate before")
X("st.q.mib", st_q_mib,  memory, gmemu,   "store quadword and modify immediate before")

//===------------------------------------------------------===//
// atomics
//===------------------------------------------------------===//
#define INSN_TABLE_ATOMIC_LD(X, NAME, TYPE, TYPE_NAME) \
    X(#NAME "." #TYPE     , NAME ## _ ## TYPE      , atomic, r2, "load atomic " TYPE_NAME " relaxed") \
    X(#NAME "." #TYPE ".a", NAME ## _ ## TYPE ## _a, atomic, r2, "load atomic " TYPE_NAME " acquire")

#define INSN_TABLE_ATOMIC_ST(X, NAME, TYPE, TYPE_NAME) \
    X(#NAME "." #TYPE     , NAME ## _ ## TYPE      , atomic, r2, "store atomic " TYPE_NAME " relaxed") \
    X(#NAME "." #TYPE ".r", NAME ## _ ## TYPE ## _r, atomic, r2, "store atomic " TYPE_NAME " release")

#define INSN_TABLE_ATOMIC_LD_OP(X, MNEM, NAME, TYPE, TYPE_NAME, DESCR) \
    X(MNEM "." #TYPE      , NAME ## _ ## TYPE       , atomic, r3, "atomic " DESCR " " TYPE_NAME " relaxed") \
    X(MNEM "." #TYPE  ".a", NAME ## _ ## TYPE ## _a , atomic, r3, "atomic " DESCR " " TYPE_NAME " acquire") \
    X(MNEM "." #TYPE  ".r", NAME ## _ ## TYPE ## _r , atomic, r3, "atomic " DESCR " " TYPE_NAME " release") \
    X(MNEM "." #TYPE ".ar", NAME ## _ ## TYPE ## _ar, atomic, r3, "atomic " DESCR " " TYPE_NAME " acq_rel")

#define INSN_TABLE_ATOMIC_ST_OP(X, MNEM, NAME, TYPE, TYPE_NAME, DESCR) \
    X(MNEM "." #TYPE,      NAME ## _ ## TYPE      , atomic, Rbc, "atomic " DESCR " " TYPE_NAME " relaxed") \
    X(MNEM "." #TYPE ".r", NAME ## _ ## TYPE ## _r, atomic, Rbc, "atomic " DESCR " " TYPE_NAME " release")

#define INSN_TABLE_ATOMIC_FULL_X(X, TYPE, TYPE_NAME) \
    INSN_TABLE_ATOMIC_LD(X, lda, TYPE, TYPE_NAME) \
    INSN_TABLE_ATOMIC_ST(X, sta, TYPE, TYPE_NAME) \
    INSN_TABLE_ATOMIC_LD_OP(X, "swap",    swap,    TYPE, TYPE_NAME, "swap") \
    INSN_TABLE_ATOMIC_LD_OP(X, "cas",     cas,     TYPE, TYPE_NAME, "compare and swap") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.add",  ld_add,  TYPE, TYPE_NAME, "load add") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.and",  ld_and,  TYPE, TYPE_NAME, "load bitwise and") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.or",   ld_or,   TYPE, TYPE_NAME, "load bitwise or") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.xor",  ld_xor,  TYPE, TYPE_NAME, "load bitwise xor") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.smin", ld_smin, TYPE, TYPE_NAME, "load signed minimum") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.smax", ld_smax, TYPE, TYPE_NAME, "load signed maximum") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.umin", ld_umin, TYPE, TYPE_NAME, "load unsigned minimum") \
    INSN_TABLE_ATOMIC_LD_OP(X, "ld.umax", ld_umax, TYPE, TYPE_NAME, "load unsigned maximum") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.add",  st_add,  TYPE, TYPE_NAME, "store add") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.and",  st_and,  TYPE, TYPE_NAME, "store bitwise and") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.or",   st_or,   TYPE, TYPE_NAME, "store bitwise or") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.xor",  st_xor,  TYPE, TYPE_NAME, "store bitwise xor") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.smin", st_smin, TYPE, TYPE_NAME, "store signed minimum") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.smax", st_smax, TYPE, TYPE_NAME, "store signed maximum") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.umin", st_umin, TYPE, TYPE_NAME, "store unsigned minimum") \
    INSN_TABLE_ATOMIC_ST_OP(X, "st.umax", st_umax, TYPE, TYPE_NAME, "store unsigned maximum") \

INSN_TABLE_ATOMIC_FULL_X(X, b, "byte")
INSN_TABLE_ATOMIC_FULL_X(X, h, "halfword")
INSN_TABLE_ATOMIC_FULL_X(X, w, "word")
INSN_TABLE_ATOMIC_FULL_X(X, d, "doubleword")
INSN_TABLE_ATOMIC_FULL_X(X, q, "quadword")

X("fence.a",  fence_acq,     atomic, NoArgs, "fence acquire")
X("fence.r",  fence_rel,     atomic, NoArgs, "fence release")
X("fence.ar", fence_acq_rel, atomic, NoArgs, "fence acq-rel")
X("fence.sc", fence_seq_cst, atomic, NoArgs, "fence seq-cst")

#undef INSN_TABLE_ATOMIC_FULL_X
#undef INSN_TABLE_ATOMIC_LD
#undef INSN_TABLE_ATOMIC_ST
#undef INSN_TABLE_ATOMIC_LD_OP
#undef INSN_TABLE_ATOMIC_ST_OP

//===------------------------------------------------------===//
// cipher/aes/hashes
//===------------------------------------------------------===//
X("aes.dec",           aes_dec,           cipher, r3, "aes decrypt round")
X("aes.dec.last",      aes_dec_last,      cipher, r3, "aes decrypt last round")
X("aes.enc",           aes_enc,           cipher, r3, "aes encrypt round")
X("aes.enc.last",      aes_enc_last,      cipher, r3, "aes encrypt last round")
X("aes.imc",           aes_imc,           cipher, r2, "aes inverse mix columns")
X("aes.keygen.assist", aes_keygen_assist, cipher, RabS10, "aes key generation assist")

X("clmul.ll", clmulll, cipher, r3, "carry-less multiply low-low")
X("clmul.hl", clmulhl, cipher, r3, "carry-less multiply high-low")
X("clmul.hh", clmulhh, cipher, r3, "carry-less multiply high-high")
X("crc32c",   crc32c,  cipher, r4, "crc32c")

//===------------------------------------------------------===//
// floating point immediate load pseudos
//===------------------------------------------------------===//
X("fldi.d",  flddi,  fpud, flddi,  "fp load double immediate")
X("fldri.d", flddri, fpud, flddri, "fp load double relative")
X("fldri.q", fldqri, fpuq, fldqri, "fp load quadruple relative")
X("fldi.s",  fldsi,  fpus, fldsi,  "fp load single immediate")
X("fldri.s", fldsri, fpus, fldsri, "fp load single relative")

//===------------------------------------------------------===//
// floating-point
//===------------------------------------------------------===//
#define INSN_FPU_SCALAR_COMMON(NAME, GROUP, TYPE) \
    X("fabs."     #NAME, fabs_     ## NAME,    GROUP, r2,     "fp absolute value " TYPE) \
    X("fnabs."    #NAME, fnabs_    ## NAME,    GROUP, r2,     "fp negate absolute value " TYPE) \
    X("fneg."     #NAME, fneg_     ## NAME,    GROUP, r2,     "fp negate " TYPE) \
    X("fadd."     #NAME, fadd_     ## NAME,    GROUP, r3,     "fp add " TYPE) \
    X("fsub."     #NAME, fsub_     ## NAME,    GROUP, r3,     "fp subtract " TYPE) \
    X("fnadd."    #NAME, fnadd_    ## NAME,    GROUP, r3,     "fp negate add " TYPE) \
    X("fabsd."    #NAME, fabsd_    ## NAME,    GROUP, r3,     "fp absolute difference " TYPE) \
    X("fmul."     #NAME, fmul_     ## NAME,    GROUP, r3,     "fp multiply " TYPE) \
    X("fnmul."    #NAME, fnmul_    ## NAME,    GROUP, r3,     "fp negate multiply " TYPE) \
    X("fnabsd."   #NAME, fnabsd_   ## NAME,    GROUP, r3,     "fp negate absolute difference " TYPE) \
    X("fcl."      #NAME, fcl_      ## NAME,    GROUP, FabSc,  "fp classify " TYPE) \
    X("frsqrt."   #NAME, frsqrt_   ## NAME,    GROUP, r2,     "fp reciprocal square root " TYPE) \
    X("fsqrt."    #NAME, fsqrt_    ## NAME,    GROUP, r2,     "fp square root " TYPE) \
    X("fdiv."     #NAME, fdiv_     ## NAME,    GROUP, r3,     "fp divide " TYPE) \
    X("fmax."     #NAME, fmax_     ## NAME,    GROUP, r3,     "fp maximum " TYPE) \
    X("fmin."     #NAME, fmin_     ## NAME,    GROUP, r3,     "fp minimum " TYPE) \
    X("fmaxnm."   #NAME, fmaxnm_   ## NAME,    GROUP, r3,     "fp maximum number " TYPE) \
    X("fminnm."   #NAME, fminnm_   ## NAME,    GROUP, r3,     "fp minimum number " TYPE) \
    X("famax."    #NAME, famax_    ## NAME,    GROUP, r3,     "fp absolute maximum " TYPE) \
    X("famin."    #NAME, famin_    ## NAME,    GROUP, r3,     "fp absolute minimum " TYPE) \
    X("fmadd."    #NAME, fmadd_    ## NAME,    GROUP, r4,     "fp multiply-add " TYPE) \
    X("fmsub."    #NAME, fmsub_    ## NAME,    GROUP, r4,     "fp multiply-subtract " TYPE) \
    X("fnmadd."   #NAME, fnmadd_   ## NAME,    GROUP, r4,     "fp negate multiply-add " TYPE) \
    X("fnmsub."   #NAME, fnmsub_   ## NAME,    GROUP, r4,     "fp negate multiply-subtract " TYPE) \
    X("fmerge."   #NAME, fmerge_   ## NAME,    GROUP, r4,     "fp merge " TYPE) \
    X("frnd."     #NAME, frnd_     ## NAME,    GROUP, Round,  "fp round " TYPE) \
    X("fcmpo.eq." #NAME, fcmpo_eq_ ## NAME,    GROUP, r3,     "fp compare ordered and equal " TYPE) \
    X("fcmpo.ne." #NAME, fcmpo_ne_ ## NAME,    GROUP, r3,     "fp compare ordered and not-equal " TYPE) \
    X("fcmpo.lt." #NAME, fcmpo_lt_ ## NAME,    GROUP, r3,     "fp compare ordered and less " TYPE) \
    X("fcmpo.le." #NAME, fcmpo_le_ ## NAME,    GROUP, r3,     "fp compare ordered and less-equal " TYPE) \
    X("fcmpo."    #NAME, fcmpo_    ## NAME,    GROUP, r3,     "fp compare ordered " TYPE) \
    X("fcmpu.eq." #NAME, fcmpu_eq_ ## NAME,    GROUP, r3,     "fp compare unordered or equal " TYPE) \
    X("fcmpu.ne." #NAME, fcmpu_ne_ ## NAME,    GROUP, r3,     "fp compare unordered or not-equal " TYPE) \
    X("fcmpu.lt." #NAME, fcmpu_lt_ ## NAME,    GROUP, r3,     "fp compare unordered or less " TYPE) \
    X("fcmpu.le." #NAME, fcmpu_le_ ## NAME,    GROUP, r3,     "fp compare unordered or less-equal " TYPE) \
    X("fcmpu."    #NAME, fcmpu_    ## NAME,    GROUP, r3,     "fp compare unordered " TYPE) \
    X("fcvt."     #NAME ".iw", fcvt_    ## NAME ## _iw,    GROUP, Round,  "fp convert " TYPE " to signed word") \
    X("fcvt."     #NAME ".uw", fcvt_    ## NAME ## _uw,    GROUP, Round,  "fp convert " TYPE " to unsigned word") \
    X("fcvt."     #NAME ".id", fcvt_    ## NAME ## _id,    GROUP, Round,  "fp convert " TYPE " to signed doubleword") \
    X("fcvt."     #NAME ".ud", fcvt_    ## NAME ## _ud,    GROUP, Round,  "fp convert " TYPE " to unsigned doubleword") \
    X("fcvt."     #NAME ".iq", fcvt_    ## NAME ## _iq,    GROUP, Round,  "fp convert " TYPE " to signed quadword") \
    X("fcvt."     #NAME ".uq", fcvt_    ## NAME ## _uq,    GROUP, Round,  "fp convert " TYPE " to unsigned quadword") \
    X("fcvt.iw."  #NAME      , fcvt_iw_ ## NAME,           GROUP, Round,  "fp convert signed word to " TYPE) \
    X("fcvt.uw."  #NAME      , fcvt_uw_ ## NAME,           GROUP, Round,  "fp convert unsigned word to " TYPE) \
    X("fcvt.id."  #NAME      , fcvt_id_ ## NAME,           GROUP, Round,  "fp convert signed doubleword to " TYPE) \
    X("fcvt.ud."  #NAME      , fcvt_ud_ ## NAME,           GROUP, Round,  "fp convert unsigned doubleword to " TYPE) \
    X("fcvt.iq."  #NAME      , fcvt_iq_ ## NAME,           GROUP, Round,  "fp convert signed quadword to " TYPE) \
    X("fcvt.uq."  #NAME      , fcvt_uq_ ## NAME,           GROUP, Round,  "fp convert unsigned quadword to " TYPE) \

INSN_FPU_SCALAR_COMMON(sh, fpuh, "scalar half")
INSN_FPU_SCALAR_COMMON(ss, fpus, "scalar single")
INSN_FPU_SCALAR_COMMON(sd, fpud, "scalar double")
INSN_FPU_SCALAR_COMMON(sq, fpuq, "scalar quadruple")

#define INSN_FPU_SCALAR_BRANCH(NAME, GROUP, TYPE) \
    X("nulfo.eq."   #NAME, nulfo_eq_   ## NAME, nullifying,  r2m2,     "nullify if fp ordered and equal " TYPE) \
    X("nulfo.ne."   #NAME, nulfo_ne_   ## NAME, nullifying,  r2m2,     "nullify if fp ordered and not-equal " TYPE) \
    X("nulfo.lt."   #NAME, nulfo_lt_   ## NAME, nullifying,  r2m2,     "nullify if fp ordered and less " TYPE) \
    X("nulfo.le."   #NAME, nulfo_le_   ## NAME, nullifying,  r2m2,     "nullify if fp ordered and less-or-equal " TYPE) \
    X("nulfo."      #NAME, nulfo_      ## NAME, nullifying,  r2m2,     "nullify if fp ordered " TYPE) \
    X("nulfu.eq."   #NAME, nulfu_eq_   ## NAME, nullifying,  r2m2,     "nullify if fp unordered or equal " TYPE) \
    X("nulfu.ne."   #NAME, nulfu_ne_   ## NAME, nullifying,  r2m2,     "nullify if fp unordered or not-equal " TYPE) \
    X("nulfu.lt."   #NAME, nulfu_lt_   ## NAME, nullifying,  r2m2,     "nullify if fp unordered or less " TYPE) \
    X("nulfu.le."   #NAME, nulfu_le_   ## NAME, nullifying,  r2m2,     "nullify if fp unordered or less-or-equal " TYPE) \
    X("nulfu."      #NAME, nulfu_      ## NAME, nullifying,  r2m2,     "nullify if fp unordered " TYPE) \
    X("nulf.class." #NAME, nulf_class_ ## NAME, nullifying,  RaSbm2,   "nullify if fp classify " TYPE) \
    X("bfo.eq."     #NAME, bfo_eq_     ## NAME, GROUP,       RabBBS,   "branch if fp ordered and equal " TYPE) \
    X("bfo.ne."     #NAME, bfo_ne_     ## NAME, GROUP,       RabBBS,   "branch if fp ordered and not-equal " TYPE) \
    X("bfo.lt."     #NAME, bfo_lt_     ## NAME, GROUP,       RabBBS,   "branch if fp ordered and less " TYPE) \
    X("bfo.le."     #NAME, bfo_le_     ## NAME, GROUP,       RabBBS,   "branch if fp ordered and less-or-equal " TYPE) \
    X("bfo."        #NAME, bfo_        ## NAME, GROUP,       RabBBS,   "branch if fp ordered " TYPE) \
    X("bfu.eq."     #NAME, bfu_eq_     ## NAME, GROUP,       RabBBS,   "branch if fp unordered or equal " TYPE) \
    X("bfu.ne."     #NAME, bfu_ne_     ## NAME, GROUP,       RabBBS,   "branch if fp unordered or not-equal " TYPE) \
    X("bfu.lt."     #NAME, bfu_lt_     ## NAME, GROUP,       RabBBS,   "branch if fp unordered or less " TYPE) \
    X("bfu.le."     #NAME, bfu_le_     ## NAME, GROUP,       RabBBS,   "branch if fp unordered or less-or-equal " TYPE) \
    X("bfu."        #NAME, bfu_        ## NAME, GROUP,       RabBBS,   "branch if fp unordered " TYPE) \
    X("bf.class."   #NAME, bf_class_   ## NAME, GROUP,       RaSbBBS,  "branch if fp classify " TYPE) \

INSN_FPU_SCALAR_BRANCH(ss, fpus, "scalar single")
INSN_FPU_SCALAR_BRANCH(sd, fpud, "scalar double")
INSN_FPU_SCALAR_BRANCH(sq, fpuq, "scalar quadruple")

#define INSN_FPU_VECTOR_COMMON(NAME, INAME, GROUP, TYPE, ITYPE) \
    X("fabs."     #NAME, fabs_     ## NAME,   GROUP,  r2,    "fp absolute difference " TYPE) \
    X("fnabs."    #NAME, fnabs_    ## NAME,   GROUP,  r2,    "fp absolute value " TYPE) \
    X("fneg."     #NAME, fneg_     ## NAME,   GROUP,  r2,    "fp negate " TYPE) \
    X("fadd."     #NAME, fadd_     ## NAME,   GROUP,  r3,    "fp add " TYPE) \
    X("fmul."     #NAME, fmul_     ## NAME,   GROUP,  r3,    "fp multiply " TYPE) \
    X("fnabsd."   #NAME, fnabsd_   ## NAME,   GROUP,  r3,    "fp negate absolute difference " TYPE) \
    X("fnadd."    #NAME, fnadd_    ## NAME,   GROUP,  r3,    "fp negate add " TYPE) \
    X("fnmul."    #NAME, fnmul_    ## NAME,   GROUP,  r3,    "fp negate multiply " TYPE) \
    X("fsub."     #NAME, fsub_     ## NAME,   GROUP,  r3,    "fp subtract " TYPE) \
    X("fdiv."     #NAME, fdiv_     ## NAME,   GROUP,  r3,    "fp divide " TYPE) \
    X("fabsd."    #NAME, fabsd_    ## NAME,   GROUP,  r3,    "fp absolute difference " TYPE) \
    X("frsqrt."   #NAME, frsqrt_   ## NAME,   GROUP,  r2,    "fp reciprocal square root " TYPE) \
    X("fsqrt."    #NAME, fsqrt_    ## NAME,   GROUP,  r2,    "fp square root " TYPE) \
    X("fmax."     #NAME, fmax_     ## NAME,   GROUP,  r3,    "fp maximum " TYPE) \
    X("fmin."     #NAME, fmin_     ## NAME,   GROUP,  r3,    "fp minimum " TYPE) \
    X("famax."    #NAME, famax_    ## NAME,   GROUP,  r3,    "fp absolute maximum " TYPE) \
    X("famin."    #NAME, famin_    ## NAME,   GROUP,  r3,    "fp absolute minimum " TYPE) \
    X("fdot."     #NAME, fdot_     ## NAME,   GROUP,  r3,    "fp dot-product " TYPE) \
    X("frnd."     #NAME, frnd_     ## NAME,   GROUP,  Round, "fp round " TYPE) \
    X("fmadd."    #NAME, fmadd_    ## NAME,   GROUP,  r4,    "fp multiply-add " TYPE) \
    X("fmsub."    #NAME, fmsub_    ## NAME,   GROUP,  r4,    "fp multiply-subtract " TYPE) \
    X("fnmadd."   #NAME, fnmadd_   ## NAME,   GROUP,  r4,    "fp negate multiply-add " TYPE) \
    X("fnmsub."   #NAME, fnmsub_   ## NAME,   GROUP,  r4,    "fp negate multiply-subtract " TYPE) \
    X("fmadda."   #NAME, fmadda_   ## NAME,   GROUP,  r4,    "fp multiply-alternating add-subtract " TYPE) \
    X("fmsuba."   #NAME, fmsuba_   ## NAME,   GROUP,  r4,    "fp multiply-alternating subtract-add " TYPE) \
    X("fmerge."   #NAME, fmerge_   ## NAME,   GROUP,  r4,    "fp merge " TYPE) \
    X("fmrgh."    #NAME, fmrgh_    ## NAME,   GROUP,  r3,    "fp merge high parts " TYPE) \
    X("fmrgl."    #NAME, fmrgl_    ## NAME,   GROUP,  r3,    "fp merge low parts " TYPE) \
    X("faddc."    #NAME, faddc_    ## NAME,   GROUP,  r3,    "fp add conjugate " TYPE) \
    X("faddh."    #NAME, faddh_    ## NAME,   GROUP,  r3,    "fp add horizontal " TYPE) \
    X("fsubc."    #NAME, fsubc_    ## NAME,   GROUP,  r3,    "fp subtract conjugate " TYPE) \
    X("fsubh."    #NAME, fsubh_    ## NAME,   GROUP,  r3,    "fp subtract horizontal " TYPE) \
    X("fmulh."    #NAME, fmulh_    ## NAME,   GROUP,  r3,    "fp multiply horizontal " TYPE) \
    X("fpk."      #NAME, fpk_      ## NAME,   GROUP,  r3,    "fp pack" TYPE) \
    X("funpkh."   #NAME, funpkh_   ## NAME,   GROUP,  r2,    "fp unpack high part " TYPE) \
    X("funpkl."   #NAME, funpkl_   ## NAME,   GROUP,  r2,    "fp unpack low part " TYPE) \
    X("fcmpo.eq." #NAME, fcmpo_eq_ ## NAME,   GROUP,  r3,    "fp compare ordered and equal " TYPE) \
    X("fcmpo.ne." #NAME, fcmpo_ne_ ## NAME,   GROUP,  r3,    "fp compare ordered and not-equal " TYPE) \
    X("fcmpo.lt." #NAME, fcmpo_lt_ ## NAME,   GROUP,  r3,    "fp compare ordered and less " TYPE) \
    X("fcmpo.le." #NAME, fcmpo_le_ ## NAME,   GROUP,  r3,    "fp compare ordered and less-equal " TYPE) \
    X("fcmpo."    #NAME, fcmpo_    ## NAME,   GROUP,  r3,    "fp compare ordered " TYPE) \
    X("fcmpu.eq." #NAME, fcmpu_eq_ ## NAME,   GROUP,  r3,    "fp compare unordered or equal " TYPE) \
    X("fcmpu.ne." #NAME, fcmpu_ne_ ## NAME,   GROUP,  r3,    "fp compare unordered or not-equal " TYPE) \
    X("fcmpu.lt." #NAME, fcmpu_lt_ ## NAME,   GROUP,  r3,    "fp compare unordered or less " TYPE) \
    X("fcmpu.le." #NAME, fcmpu_le_ ## NAME,   GROUP,  r3,    "fp compare unordered or less-equal " TYPE) \
    X("fcmpu."    #NAME, fcmpu_    ## NAME,   GROUP,  r3,    "fp compare unordered " TYPE) \
    X("fcvt.i" #INAME "." #NAME, fcvt_i  ## INAME ## _  ## NAME,  GROUP,  Round, "fp convert signed " ITYPE " to " TYPE) \
    X("fcvt.u" #INAME "." #NAME, fcvt_u  ## INAME ## _  ## NAME,  GROUP,  Round, "fp convert unsigned " ITYPE " to " TYPE) \
    X("fcvt." #NAME ".i" #INAME, fcvt_   ## NAME  ## _i ## INAME, GROUP,  Round, "fp convert " TYPE " to signed " ITYPE) \
    X("fcvt." #NAME ".u" #INAME, fcvt_   ## NAME  ## _u ## INAME, GROUP,  Round, "fp convert " TYPE " to unsigned " ITYPE) \

INSN_FPU_VECTOR_COMMON(ph, h, fpuh, "packed half", "halfwords")
INSN_FPU_VECTOR_COMMON(ps, w, fpus, "packed single", "words")
INSN_FPU_VECTOR_COMMON(pd, d, fpud, "packed double", "doublewords")

//===------------------------------------------------------===//
// custom fpu
//===------------------------------------------------------===//

X("fscale", fscale,  fpuq, FabSc, "fp scale")

X("fcvt.sd.ss", fcvt_sd_ss, fpud, Round, "fp convert scalar double to scalar single")
X("fcvt.sq.sd", fcvt_sq_sd, fpuq, Round, "fp convert scalar quadruple to scalar double")
X("fcvt.sq.ss", fcvt_sq_ss, fpuq, Round, "fp convert scalar quadruple to scalar single")
X("fcvt.ss.sh", fcvt_ss_sh, fpuh, Round, "fp convert scalar single to scalar halffloat")
X("fcvt.sd.sh", fcvt_sd_sh, fpuh, Round, "fp convert scalar double to scalar halffloat")
X("fcvt.sq.sh", fcvt_sq_sh, fpuh, Round, "fp convert scalar quadruple to scalar halffloat")

X("fext.sh.ss", fext_sh_ss, fpuh, r2, "fp extend scalar halffloat to scalar single")
X("fext.sh.sd", fext_sh_sd, fpuh, r2, "fp extend scalar halffloat to scalar double")
X("fext.sh.sq", fext_sh_sq, fpuh, r2, "fp extend scalar halffloat to scalar quadruple")
X("fext.ss.sd", fext_ss_sd, fpus, r2, "fp extend scalar single to scalar double")
X("fext.ss.sq", fext_ss_sq, fpuq, r2, "fp extend scalar single to scalar quadruple")
X("fext.sd.sq", fext_sd_sq, fpuq, r2, "fp extend scalar double to scalar quadruple")

//===------------------------------------------------------===//
// integer vectors
//===------------------------------------------------------===//
#define INSN_MMX_COMMON(NAME, TYPE) \
    X("vaddc."    #NAME, vaddc_    ## NAME, mmx, r3,  "vector add carry-out " TYPE) \
    X("vaddo."    #NAME, vaddo_    ## NAME, mmx, r3,  "vector add overflow " TYPE) \
    X("vaddss."   #NAME, vaddss_   ## NAME, mmx, r3,  "vector add signed saturate " TYPE) \
    X("vaddu."    #NAME, vaddu_    ## NAME, mmx, r3,  "vector add unsigned " TYPE) \
    X("vaddus."   #NAME, vaddus_   ## NAME, mmx, r3,  "vector add unsigned saturate " TYPE) \
    X("vavgs."    #NAME, vavgs_    ## NAME, mmx, r3,  "vector average signed " TYPE) \
    X("vavgu."    #NAME, vavgu_    ## NAME, mmx, r3,  "vector average unsigned " TYPE) \
    X("vcmp.eq."  #NAME, vcmp_eq_  ## NAME, mmx, r3,  "vector compare equal " TYPE) \
    X("vcmp.lt."  #NAME, vcmp_lt_  ## NAME, mmx, r3,  "vector compare less " TYPE) \
    X("vcmp.ltu." #NAME, vcmp_ltu_ ## NAME, mmx, r3,  "vector compare less unsigned " TYPE) \
    X("vmaxs."    #NAME, vmaxs_    ## NAME, mmx, r3,  "vector maximum signed " TYPE) \
    X("vmaxu."    #NAME, vmaxu_    ## NAME, mmx, r3,  "vector maximum unsigned " TYPE) \
    X("vmins."    #NAME, vmins_    ## NAME, mmx, r3,  "vector minimum signed " TYPE) \
    X("vminu."    #NAME, vminu_    ## NAME, mmx, r3,  "vector minimum unsigned " TYPE) \
    X("vmrg.h."   #NAME, vmrgh_    ## NAME, mmx, r3,  "vector merge high " TYPE) \
    X("vmrg.l."   #NAME, vmrgl_    ## NAME, mmx, r3,  "vector merge low " TYPE) \
    X("vrol."     #NAME, vrol_     ## NAME, mmx, r3,  "vector rotate left " TYPE) \
    X("vror."     #NAME, vror_     ## NAME, mmx, r3,  "vector rotate right " TYPE) \
    X("vsll."     #NAME, vsll_     ## NAME, mmx, r3,  "vector shift left logical " TYPE) \
    X("vslli."    #NAME, vslli_    ## NAME, mmx, FabSc, "vector shift left logical immediate " TYPE) \
    X("vsra."     #NAME, vsra_     ## NAME, mmx, r3,  "vector shift right algebraic " TYPE) \
    X("vsrai."    #NAME, vsrai_    ## NAME, mmx, FabSc, "vector shift right algebraic immediate " TYPE) \
    X("vsrl."     #NAME, vsrl_     ## NAME, mmx, r3,  "vector shift right logical " TYPE) \
    X("vsrli."    #NAME, vsrli_    ## NAME, mmx, FabSc, "vector shift right logical immediate " TYPE) \
    X("vsubb."    #NAME, vsubb_    ## NAME, mmx, r3,  "vector subtract borrow " TYPE) \
    X("vsubo."    #NAME, vsubo_    ## NAME, mmx, r3,  "vector subtract overflow " TYPE) \
    X("vsubss."   #NAME, vsubss_   ## NAME, mmx, r3,  "vector subtract signed saturated " TYPE) \
    X("vsubu."    #NAME, vsubu_    ## NAME, mmx, r3,  "vector subtract unsigned " TYPE) \
    X("vsubus."   #NAME, vsubus_   ## NAME, mmx, r3,  "vector subtract unsigned saturated " TYPE) \

INSN_MMX_COMMON(b, "byte")
INSN_MMX_COMMON(h, "halfword")
INSN_MMX_COMMON(w, "word")
INSN_MMX_COMMON(d, "doubleword")

//===------------------------------------------------------===//
// custom vector
//===------------------------------------------------------===//

X("vupkhs.b", vupkhs_b,  mmx, r2, "vector unpack high signed byte")
X("vupkls.b", vupkls_b,  mmx, r2, "vector unpack low signed byte")

X("vpksss.h", vpksss_h,  mmx, r3, "vector pack signed as signed saturate halfword")
X("vpksus.h", vpksus_h,  mmx, r3, "vector pack signed as unsigned saturate halfword")
X("vpkuum.h", vpkuum_h,  mmx, r3, "vector pack unsigned as unsigned modulo halfword")
X("vpkuus.h", vpkuus_h,  mmx, r3, "vector pack unsigned as unsigned saturate halfword")
X("vupkhs.h", vupkhs_h,  mmx, r2, "vector unpack high signed halfword")
X("vupkls.h", vupkls_h,  mmx, r2, "vector unpack low signed halfword")

X("vpksss.w", vpksss_w,  mmx, r3, "vector pack signed as signed saturate word")
X("vpksus.w", vpksus_w,  mmx, r3, "vector pack signed as unsigned saturate word")
X("vpkuum.w", vpkuum_w,  mmx, r3, "vector pack unsigned as unsigned modulo word")
X("vpkuus.w", vpkuus_w,  mmx, r3, "vector pack unsigned as unsigned saturate word")
X("vupkhs.w", vupkhs_w,  mmx, r2, "vector unpack high signed word")
X("vupkls.w", vupkls_w,  mmx, r2, "vector unpack low signed word")

X("vpksss.d", vpksss_d,  mmx, r3, "vector pack signed as signed saturate doubleword")
X("vpksus.d", vpksus_d,  mmx, r3, "vector pack signed as unsigned saturate doubleword")
X("vpkuum.d", vpkuum_d,  mmx, r3, "vector pack unsigned as unsigned modulo doubleword")
X("vpkuus.d", vpkuus_d,  mmx, r3, "vector pack unsigned as unsigned saturate doubleword")

//===------------------------------------------------------===//
// end macrotable
//===------------------------------------------------------===//
