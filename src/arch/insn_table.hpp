//===------------------------------------------------------===//
// assembler instruction mnemonics
//
// X(mnemonic, hw_subset, format, description)
//
// 1  mnemonic:
//    assembler mnemonic (11 characters maximum)
// 2  instruction subset:
//    common base (base),
//    floating-point quadruple (fpuq),
//    floating-point double scalar/packed (fpud),
//    floating-point single scalar/packed (fpus),
//    dsp-like integer packed (mmx),
//    privileged, special,
//    atomic, cipher (aes+clmul)
//
// 3  sw/hw format:
//    assembler syntax format and
//    instruction slot format (in different sets)
// 4  description:
//    full instruction name
//
//===------------------------------------------------------===//

//===------------------------------------------------------===//
// add/subtract
//===------------------------------------------------------===//
X(add,     base, Rabc,     "add")
X(addadd,  base, Rabcd,    "add 3 values")
X(addaddc, base, Rabcd,    "add-add with carry-out")
X(addc,    base, Rabc,     "add carry-out")
X(addi,    base, RabSI21,  "add immediate")
X(addiws,  base, RabSI21,  "add immediate word, sext")
X(addiwz,  base, RabSI21,  "add immediate word, zext")
X(addo,    base, Rabc,     "add overflow")
X(addsub,  base, Rabcd,    "add-subtract")
X(addw,    base, Rabc,     "add word")
X(sub,     base, Rabc,     "subtract")
X(subb,    base, Rabc,     "subtract with borrow")
X(subfi,   base, RabSI21,  "subtract from immediate")
X(subfiws, base, RabSI21,  "subtract from immediate word, sext")
X(subfiwz, base, RabSI21,  "subtract from immediate word, zext")
X(subo,    base, Rabc,     "subtract overflow")
X(subsub,  base, Rabcd,    "subtract twice")
X(subsubb, base, Rabcd,    "subtract-subtract with borrow-out")
X(subw,    base, Rabc,     "subtract word")

X(addq,    intq, Rabc,     "add quadword")
X(subq,    intq, Rabc,     "subtract quadword")

//===------------------------------------------------------===//
// bitwise
//===------------------------------------------------------===//
X(and,     base, Rabc,     "and bitwise")
X(anddec,  base, Rabc,     "and bitwise decremented")
X(andi,    base, RabSI21,  "and bitwise with immediate")
X(andn,    base, Rabc,     "and not bitwise")
X(andneg,  base, Rabc,     "and bitwise negate")
X(andni,   base, RabSI21,  "and not bitwise with immediate")
X(nand,    base, Rabc,     "bitwise not and")
X(nop,     base, RaxU28,   "no operation")
X(nor,     base, Rabc,     "bitwise not or")
X(not,     base, Not,      "bitwise not")
X(or,      base, Rabc,     "bitwise or")
X(ori,     base, RabSI21,  "bitwise or with immediate")
X(orn,     base, Rabc,     "bitwise or not")
X(orni,    base, RabSI21,  "bitwise or not immediate")
X(xnor,    base, Rabc,     "bitwise exclusive nor")
X(xor,     base, Rabc,     "bitwise exclusive or")
X(xordec,  base, Rabc,     "bitwise exclusive-or decremented")
X(xori,    base, RabSI21,  "bitwise exclusive or with immediate")

//===------------------------------------------------------===//
//
//===------------------------------------------------------===//
X(absd,    base,  Rabc,    "absolute difference")
X(absdw,   base,  Rabc,    "absolute difference word")
X(algnup,  base,  RabScd,  "align up shifted")
X(dep,     base,  RabcSde, "deposit")
X(depa,    base,  RabScd,  "deposit alter")
X(depc,    base,  RabScd,  "deposit clear")
X(deps,    base,  RabScd,  "deposit set")
X(ldar,    base,  ldar,    "load address relative")
X(ldax,    base,  gmemx,   "load address indexed")
X(ldan,    base,  RabSI21,  "load address near")
X(ldanrc,  base,  RabSI21,  "load address near relative")
X(ldi,     base,  ldi,     "load immediate")
X(mov,     base,  Rab,     "move general register")
X(mov2,    base,  Rabcd,   "move 2 general registers")
X(mul,     base,  Rabc,    "multiply")
X(muladd,  base,  Rabcd,   "multiply-add")
X(mulh,    base,  Rabc,    "multiply high")
X(muli,    base,  RabSI21,  "multiply immediate")
X(mulsub,  base,  Rabcd,   "multiply-subtract")
X(mulsubf, base,  Rabcd,   "multiply-subtract from")
X(rlmi,    base,  RabScde, "rotate left and mask immediate")
X(select,  base,  Rabcd,   "select")
X(mbsel,   base,  Rabcd,   "masked bit selection")

X(depq,    intq,  FabcSd,  "deposit quadword")

//===------------------------------------------------------===//
// shifts
//===------------------------------------------------------===//
X(sladd,   base, RabcSd, "shift left and add")
X(sll,     base, Rabc,   "shift left logical")
X(slli,    base, RabSc,  "shift left logical immediate")
X(slliw,   base, RabSc,  "shift left logical immediate word")
X(sllw,    base, Rabc,   "shift left logical word")
X(slp,     base, Rabcd,  "shift left pair")
X(slsra,   base, Rabcd,  "shift left and shift right algebraic")
X(slsrai,  base, RabScd, "shift left and shift right algebraic immediate")
X(slsrl,   base, Rabcd,  "shift left and shift right logical")
X(slsrli,  base, RabScd, "shift left and shift right logical immediate")
X(slsrlw,  base, Rabcd,  "shift left and shift right logical word")
X(slsraw,  base, Rabcd,  "shift left and shift right algebraic word")
X(slsub,   base, RabcSd, "shift left and subtract")
X(slsubf,  base, RabcSd, "shift left and subtract from")

X(sra,     base, Rabc,   "shift right algebraic")
X(srai,    base, RabSc,  "shift right algebraic immediate")
X(sraiw,   base, RabSc,  "shift right algebraic immediate word")
X(sraw,    base, Rabc,   "shift right algebraic word")
X(srd,     base, Rabc,   "shift right dividing")
X(srdi,    base, RabSc,  "shift right dividing immediate")
X(srl,     base, Rabc,   "shift right logical")
X(srli,    base, RabSc,  "shift right logical immediate")
X(srliw,   base, RabSc,  "shift right logical immediate word")
X(srlw,    base, Rabc,   "shift right logical word")
X(srp,     base, Rabcd,  "shift right pair")
X(srpi,    base, RabcSd, "shift right pair immediate")
X(srpiq,   intq, FabcSd, "shift right pair immediate quadword")

//===------------------------------------------------------===//
// divide/modulo
//===------------------------------------------------------===//
X(div,     base, Rabc,     "divide")
X(divi,    base, RabSI21,  "divide immediate")
X(divu,    base, Rabc,     "divide unsigned")
X(divui,   base, RabUI21,  "divide unsigned immediate")
X(divw,    base, Rabc,     "divide word")
X(divuw,   base, Rabc,     "divide unsigned word")

X(mod,     base, Rabc,     "module")
X(modi,    base, RabSI21,  "module immediate")
X(modu,    base, Rabc,     "module unsigned")
X(modui,   base, RabUI21,  "module unsigned immediate")
X(moduw,   base, Rabc,     "module unsigned word")
X(modw,    base, Rabc,     "module word")

//===------------------------------------------------------===//
// min/max
//===------------------------------------------------------===//
X(minsw,    base, Rabc,   "minimum signed word")
X(maxsw,    base, Rabc,   "maximum signed word")
X(minuw,    base, Rabc,   "minimum unsigned word")
X(maxuw,    base, Rabc,   "maximum unsigned word")

X(mins,    base, Rabc,   "minimum signed")
X(maxs,    base, Rabc,   "maximum signed")
X(minu,    base, Rabc,   "minimum unsigned")
X(maxu,    base, Rabc,   "maximum unsigned")

X(minsi,   base, RabSI21,  "minimum signed immediate")
X(maxsi,   base, RabSI21,  "maximum signed immediate")
X(minui,   base, RabUI21,  "minimum unsigned immediate")
X(maxui,   base, RabUI21,  "maximum unsigned immediate")

//===------------------------------------------------------===//
// compare 64 bit
//===------------------------------------------------------===//
#define INSN_BASE_TYPED(NAME, TYPE) \
    X(cmp ## NAME ## eq,    base,        Rabc,      "compare " TYPE " equal") \
    X(cmp ## NAME ## ne,    base,        Rabc,      "compare " TYPE " not equal") \
    X(cmp ## NAME ## lt,    base,        Rabc,      "compare " TYPE " less") \
    X(cmp ## NAME ## ltu,   base,        Rabc,      "compare " TYPE " less unsigned") \
    X(cmp ## NAME ## le,    base,        Rabc,      "compare " TYPE " less or equal") \
    X(cmp ## NAME ## leu,   base,        Rabc,      "compare " TYPE " less or equal unsigned") \
    X(cmp ## NAME ## eqi,   base,        RabSI21,   "compare " TYPE " equal immediate") \
    X(cmp ## NAME ## nei,   base,        RabSI21,   "compare " TYPE " not equal immediate") \
    X(cmp ## NAME ## lti,   base,        RabSI21,   "compare " TYPE " less immediate") \
    X(cmp ## NAME ## gti,   base,        RabSI21,   "compare " TYPE " greater immediate") \
    X(cmp ## NAME ## ltui,  base,        RabUI21,   "compare " TYPE " less unsigned immediate") \
    X(cmp ## NAME ## gtui,  base,        RabUI21,   "compare " TYPE " greater unsigned immediate") \
    X(cmp ## NAME ## gt,    base,        Racb,      "compare " TYPE " greater") /*pseudo*/ \
    X(cmp ## NAME ## gtu,   base,        Racb,      "compare " TYPE " greater unsigned") /*pseudo*/ \
    X(cmp ## NAME ## ge,    base,        Racb,      "compare " TYPE " greater or equal") /*pseudo*/ \
    X(cmp ## NAME ## geu,   base,        Racb,      "compare " TYPE " greater or equal unsigned") /*pseudo*/ \
    X(cmp ## NAME ## gei,   base,        RabSI21M,  "compare " TYPE " greater or equal immediate") /*pseudo*/ \
    X(cmp ## NAME ## lei,   base,        RabSI21P,  "compare " TYPE " less or equal immediate") /*pseudo*/ \
    X(cmp ## NAME ## geui,  base,        RabUI21M,  "compare " TYPE " greater or equal unsigned immediate") /*pseudo*/ \
    X(cmp ## NAME ## leui,  base,        RabUI21P,  "compare " TYPE " less or equal unsigned immediate") /*pseudo*/ \
    X(b   ## NAME ## eq,    branch,      RabBBS,    "branch if " TYPE " equal") \
    X(b   ## NAME ## ne,    branch,      RabBBS,    "branch if " TYPE " not equal") \
    X(b   ## NAME ## lt,    branch,      RabBBS,    "branch if " TYPE " less") \
    X(b   ## NAME ## ltu,   branch,      RabBBS,    "branch if " TYPE " less unsigned") \
    X(b   ## NAME ## le,    branch,      RabBBS,    "branch if " TYPE " less or equal") \
    X(b   ## NAME ## leu,   branch,      RabBBS,    "branch if " TYPE " less or equal unsigned") \
    X(b   ## NAME ## gt,    branch,      RbaBBS,    "branch if " TYPE " greater") /*pseudo*/ \
    X(b   ## NAME ## gtu,   branch,      RbaBBS,    "branch if " TYPE " greater unsigned") /*pseudo*/ \
    X(b   ## NAME ## ge,    branch,      RbaBBS,    "branch if " TYPE " greater or equal") /*pseudo*/ \
    X(b   ## NAME ## geu,   branch,      RbaBBS,    "branch if " TYPE " greater or equal unsigned") /*pseudo*/ \
    X(b   ## NAME ## eqi,   branch,      RaSIBBS,   "branch if " TYPE " equal immediate") \
    X(b   ## NAME ## nei,   branch,      RaSIBBS,   "branch if " TYPE " not equal immediate") \
    X(b   ## NAME ## lti,   branch,      RaSIBBS,   "branch if " TYPE " less immediate") \
    X(b   ## NAME ## gei,   branch,      RaSIBBS,   "branch if " TYPE " greater or equal immediate") \
    X(b   ## NAME ## lei,   branch,      RaSIPBBS,  "branch if " TYPE " less or equal immediate") /*pseudo*/ \
    X(b   ## NAME ## gti,   branch,      RaSIPBBS,  "branch if " TYPE " greater immediate") /*pseudo*/ \
    X(b   ## NAME ## ltui,  branch,      RaUIBBS,   "branch if " TYPE " less unsigned immediate") \
    X(b   ## NAME ## geui,  branch,      RaUIBBS,   "branch if " TYPE " greater or equal unsigned immediate") \
    X(b   ## NAME ## leui,  branch,      RaUIPBBS,  "branch if " TYPE " less or equal unsigned immediate") /*pseudo*/ \
    X(b   ## NAME ## gtui,  branch,      RaUIPBBS,  "branch if " TYPE " greater unsigned immediate") /*pseudo*/ \
    X(nul ## NAME ## eq,    nullifying,  r2m2,      "nullify if " TYPE " equal") \
    X(nul ## NAME ## ne,    nullifying,  r2m2,      "nullify if " TYPE " not-equal") \
    X(nul ## NAME ## lt,    nullifying,  r2m2,      "nullify if " TYPE " less") \
    X(nul ## NAME ## le,    nullifying,  r2m2,      "nullify if " TYPE " less or equal") \
    X(nul ## NAME ## ltu,   nullifying,  r2m2,      "nullify if " TYPE " less unsigned") \
    X(nul ## NAME ## leu,   nullifying,  r2m2,      "nullify if " TYPE " less or equal unsigned") \
    X(nul ## NAME ## eqi,   nullifying,  RaSIm2,    "nullify if " TYPE " equal immediate") \
    X(nul ## NAME ## nei,   nullifying,  RaSIm2,    "nullify if " TYPE " not-equal immediate") \
    X(nul ## NAME ## lti,   nullifying,  RaSIm2,    "nullify if " TYPE " less immediate") \
    X(nul ## NAME ## gti,   nullifying,  RaSIm2,    "nullify if " TYPE " greater immediate") \
    X(nul ## NAME ## ltui,  nullifying,  RaUIm2,    "nullify if " TYPE " less unsigned immediate") \
    X(nul ## NAME ## gtui,  nullifying,  RaUIm2,    "nullify if " TYPE " greater unsigned immediate") \

INSN_BASE_TYPED(w, "word")
INSN_BASE_TYPED(d, "doubleword")

//===------------------------------------------------------===//
// compare 128 bit
//===------------------------------------------------------===//
X(cmpqeq,   intq, Rabc,  "compare quadword equal")
X(cmpqne,   intq, Rabc,  "compare quadword not equal")

//===------------------------------------------------------===//
// bit manipulation/counting
//===------------------------------------------------------===//
X(cntlz,   bitmanip, RabSc, "count leading zeros")
X(cntpop,  bitmanip, RabSc, "count population")
X(cnttz,   bitmanip, RabSc, "count trailing zeros")

X(gtb,     bitmanip, Rab,   "graycode to binary")
X(perm,    bitmanip, Rabcd, "permute bytes")
X(permb,   bitmanip, RabSc, "permute bits")
X(mbgath,  bitmanip, Rabc,  "masked bit gather")
X(mbscat,  bitmanip, Rabc,  "masked bit scatter")

//===------------------------------------------------------===//
// branches bit checks
//===------------------------------------------------------===//
X(bbc,      branch, RabBBS,   "branch if bit clear")
X(bbci,     branch, RaSbBBS,  "branch if bit clear immediate")
X(bbs,      branch, RabBBS,   "branch if bit set")
X(bbsi,     branch, RaSbBBS,  "branch if bit set immediate")

X(bmall,    branch, RaUIBBS,  "branch if mask immediate all bits set")
X(bmany,    branch, RaUIBBS,  "branch if mask immediate any bit set")
X(bmnone,   branch, RaUIBBS,  "branch if mask immediate none bit set")
X(bmnotall, branch, RaUIBBS,  "branch if mask immediate not all bits set")

//===------------------------------------------------------===//
// loops
//===------------------------------------------------------===//
X(repdlt,  branch, RabBBS, "repeat on doubleword less")
X(repdle,  branch, RabBBS, "repeat on doubleword less or equal")
X(repdltu, branch, RabBBS, "repeat on doubleword less unsigned")
X(repdleu, branch, RabBBS, "repeat on doubleword less or equal unsigned")
X(repdgt,  branch, RabBBS, "repeat on doubleword greater")
X(repdge,  branch, RabBBS, "repeat on doubleword greater or equal")
X(repdgtu, branch, RabBBS, "repeat on doubleword greater unsigned")
X(repdgeu, branch, RabBBS, "repeat on doubleword greater or equal unsigned")

//===------------------------------------------------------===//
// unconditional jumps
//===------------------------------------------------------===//
X(jmp,    branch, br,        "jump relative")
X(jmpr,   branch, RbcScale,  "jump register indirect")
X(jmpth,  branch, Rbc,       "jump table halfword")
X(jmptw,  branch, Rbc,       "jump table word")

//===------------------------------------------------------===//
// call/return/regframe
//===------------------------------------------------------===//
X(callmi,  branch, RabS10,    "call memory indirect")
X(callmrw, branch, RabS10,    "call memory indirect relative word")
X(callr,   branch, callr,     "call relative")
X(callri,  branch, Rabc,      "call register indirect")
X(callplt, branch, gmemr,     "call procedure linkage table")
X(ret,     branch, NoArgs,    "return from subroutine")
X(retf,    branch, RaxUI21,   "return from subroutine and rollback stack frame")

//===------------------------------------------------------===//
// special
//===------------------------------------------------------===//
X(alloc,   special, alloc,    "allocate register frame and update eip")
X(allocsp, special, allocsp,  "allocate register frame, update sp, update eip")
X(syscall, special, NoArgs,   "system call")
X(undef,   special, NoArgs,   "undefined instruction")
X(random,  special, Rab,      "random")
X(mfspr,   special, mspr,     "move from special-purpose register")
X(mtspr,   special, mspr,     "move to special-purpose register")
X(sprsr,   special, sprsr,    "special register set/reset")
X(icbi,    special, RaxbSI21, "instruction cache block invalidate")
X(int,     special, RbS10,    "interrupt")
X(mprobe,  special, Rabc,     "memory probe access")
X(cpuid,   special, RabS10,   "cpu identification")
X(dcbf,    special, RaxbSI21, "data cache block flush")
X(dcbt,    special, RaxbSI21, "data cache block touch")

//===------------------------------------------------------===//
// software exceptions
//===------------------------------------------------------===//
X(ehadj,    special, br,      "exception handler adjust eip")
X(ehcatch,  special, RaxbBBS, "exception handler catch")
X(ehnext,   special, RaxbBBS, "exception handler next")
X(ehthrow,  special, RaxbSI21, "exception handler throw")

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
X(tpa,     privileged, Rab,     "translate to physical address")
X(ptc,     privileged, Rbc,     "purge translation cache by page address and ASID")
X(mtdtr,   privileged, Rabc,    "move to data translation register")
X(mtitr,   privileged, Rabc,    "move to instruction translation register")

//===------------------------------------------------------===//
// debug/monitoring
//===------------------------------------------------------===//
X(mfibr,  privileged, RabS10, "move from instruction breakpoint register")
X(mtibr,  privileged, RabS10, "move to instruction breakpoint register")
X(mfdbr,  privileged, RabS10, "move from data breakpoint register")
X(mtdbr,  privileged, RabS10, "move to data breakpoint register")
X(mfmr,   privileged, RabS10, "move from monitor register")
X(mtmr,   privileged, RabS10, "move to monitor register")

//===------------------------------------------------------===//
// load/store relative
//===------------------------------------------------------===//
X(ldafr,   base,   gmemr,     "load address forward relative")

#define INSN_LOAD_STORE(NAME, TYPE) \
    X(ld ## NAME ## s,     memory, RabSI21,  "load "  TYPE " sext") \
    X(ld ## NAME ## z,     memory, RabSI21,  "load "  TYPE " zext") \
    X(st ## NAME,          memory, RabSI21,  "store " TYPE) \
    X(ld ## NAME ## sr,    memory, gmemr,   "load "  TYPE " sext relative") \
    X(ld ## NAME ## zr,    memory, gmemr,   "load "  TYPE " zext relative") \
    X(st ## NAME ## r,     memory, gmemr,   "store " TYPE " relative") \
    X(ld ## NAME ## sx,    memory, gmemx,   "load "  TYPE " sext indexed") \
    X(ld ## NAME ## zx,    memory, gmemx,   "load "  TYPE " zext indexed") \
    X(st ## NAME ## x,     memory, gmemx,   "store " TYPE " indexed") \
    X(ld ## NAME ## smia,  memory, gmemu,   "load "  TYPE " sext and modify immediate after") \
    X(ld ## NAME ## zmia,  memory, gmemu,   "load "  TYPE " zext and modify immediate after") \
    X(st ## NAME ## mia,   memory, gmemu,   "store " TYPE " and modify immediate after") \
    X(ld ## NAME ## smib,  memory, gmemu,   "load "  TYPE " sext and modify immediate before") \
    X(ld ## NAME ## zmib,  memory, gmemu,   "load "  TYPE " zext and modify immediate before") \
    X(st ## NAME ## mib,   memory, gmemu,   "store " TYPE " and modify immediate before") \

INSN_LOAD_STORE(b, "byte")
INSN_LOAD_STORE(h, "halfword")
INSN_LOAD_STORE(w, "word")
INSN_LOAD_STORE(d, "doubleword")

X(ldqr,    memory, gmemr,   "load quadword relative")
X(stqr,    memory, gmemr,   "store quadword relative")
X(ldq,     memory, RabSI21, "load quadword")
X(stq,     memory, RabSI21, "store quadword")
X(ldqx,    memory, gmemx,   "load quadword indexed")
X(stqx,    memory, gmemx,   "store quadword indexed")
X(ldqmia,  memory, gmemu,   "load quadword and modify immediate after")
X(stqmia,  memory, gmemu,   "store quadword and modify immediate after")
X(ldqmib,  memory, gmemu,   "load quadword and modify immediate before")
X(stqmib,  memory, gmemu,   "store quadword and modify immediate before")

//===------------------------------------------------------===//
// atomics
//===------------------------------------------------------===//
#define INSN_TABLE_ATOMIC_SHORT_X(X, TYPE, TYPE_NAME) \
    X(lda    ## TYPE, atomic, RabMo,  "load atomic " TYPE_NAME) \
    X(sta    ## TYPE, atomic, RabMo,  "store atomic " TYPE_NAME) \
    X(swap   ## TYPE, atomic, RabcMo, "atomic swap " TYPE_NAME) \
    X(cas    ## TYPE, atomic, RabcMo, "atomic compare and swap " TYPE_NAME) \

#define INSN_TABLE_ATOMIC_X(X, TYPE, TYPE_NAME) \
    INSN_TABLE_ATOMIC_SHORT_X(X, TYPE, TYPE_NAME) \
    X(ldadd  ## TYPE, atomic, RabcMo, "atomic load add ") \
    X(ldand  ## TYPE, atomic, RabcMo, "atomic load bitwise and " TYPE_NAME) \
    X(ldor   ## TYPE, atomic, RabcMo, "atomic load bitwise or " TYPE_NAME) \
    X(ldxor  ## TYPE, atomic, RabcMo, "atomic load bitwise xor " TYPE_NAME) \
    X(ldsmin ## TYPE, atomic, RabcMo, "atomic load signed minimum " TYPE_NAME) \
    X(ldsmax ## TYPE, atomic, RabcMo, "atomic load signed maximum " TYPE_NAME) \
    X(ldumin ## TYPE, atomic, RabcMo, "atomic load unsigned minimum " TYPE_NAME) \
    X(ldumax ## TYPE, atomic, RabcMo, "atomic load unsigned maximum " TYPE_NAME) \
    X(stadd  ## TYPE, atomic, RbcMo,  "atomic store add ") \
    X(stand  ## TYPE, atomic, RbcMo,  "atomic store bitwise and " TYPE_NAME) \
    X(stor   ## TYPE, atomic, RbcMo,  "atomic store bitwise or " TYPE_NAME) \
    X(stxor  ## TYPE, atomic, RbcMo,  "atomic store bitwise xor " TYPE_NAME) \
    X(stsmin ## TYPE, atomic, RbcMo,  "atomic store signed minimum " TYPE_NAME) \
    X(stsmax ## TYPE, atomic, RbcMo,  "atomic store signed maximum " TYPE_NAME) \
    X(stumin ## TYPE, atomic, RbcMo,  "atomic store unsigned minimum " TYPE_NAME) \
    X(stumax ## TYPE, atomic, RbcMo,  "atomic store unsigned maximum " TYPE_NAME) \

INSN_TABLE_ATOMIC_X(X, b, "byte")
INSN_TABLE_ATOMIC_X(X, h, "halfword")
INSN_TABLE_ATOMIC_X(X, w, "word")
INSN_TABLE_ATOMIC_X(X, d, "doubleword")
INSN_TABLE_ATOMIC_SHORT_X(X, q, "quadword")

X(fence,   atomic, fence,   "fence")

#undef INSN_TABLE_ATOMIC_X
#undef INSN_TABLE_ATOMIC_SHORT_X

X(nulbc,      nullifying, r2m2,   "nullify if bit clear")
X(nulbci,     nullifying, RaSbm2, "nullify if bit clear immediate")
X(nulbs,      nullifying, r2m2,   "nullify if bit set")
X(nulbsi,     nullifying, RaSbm2, "nullify if bit set immediate")

X(nulmall,    nullifying, RaUIm2, "nullify if mask all bit set")
X(nulmnotall, nullifying, RaUIm2, "nullify if mask not all bit set")
X(nulmnone,   nullifying, RaUIm2, "nullify if mask none bit set")
X(nulmany,    nullifying, RaUIm2, "nullify if mask any bit set")

//===------------------------------------------------------===//
// llvm workarounds (temp)
//===------------------------------------------------------===//
X(sllqi,      intq,   RabSc,      "sllqi")
X(srlqi,      intq,   RabSc,      "srlqi")
X(sraqi,      intq,   RabSc,      "sraqi")

X(jmpmi,      special,   RbcScale,   "jmpmi")

//===------------------------------------------------------===//
// cipher/aes/hashes
//===------------------------------------------------------===//
X(aesdec,          cipher, Rabc,   "aes decrypt round")
X(aesdeclast,      cipher, Rabc,   "aes decrypt last round")
X(aesenc,          cipher, Rabc,   "aes encrypt round")
X(aesenclast,      cipher, Rabc,   "aes encrypt last round")
X(aesimc,          cipher, Rab,    "aes inverse mix columns")
X(aeskeygenassist, cipher, RabS10, "aes key generation assist")
X(clmulll,         cipher, Rabc,   "carry-less multiply low-low")
X(clmulhl,         cipher, Rabc,   "carry-less multiply high-low")
X(clmulhh,         cipher, Rabc,   "carry-less multiply high-high")
X(crc32c,          cipher, Rabcd,  "crc32c")

//===------------------------------------------------------===//
// floating point immediate load pseudos
//===------------------------------------------------------===//
X(flddi,  fpud, flddi,  "fp load double immediate")
X(flddri, fpud, flddri, "fp load double relative")
X(fldqri, fpuq, fldqri, "fp load quadruple relative")
X(fldsi,  fpus, fldsi,  "fp load single immediate")
X(fldsri, fpus, fldsri, "fp load single relative")

//===------------------------------------------------------===//
// floating-point
//===------------------------------------------------------===//
#define INSN_FPU_SCALAR_COMMON(NAME, GROUP, TYPE) \
    X(fabs    ## NAME,           GROUP,       Rab,      "fp absolute value " TYPE) \
    X(fnabs   ## NAME,           GROUP,       Rab,      "fp negate absolute value " TYPE) \
    X(fneg    ## NAME,           GROUP,       Rab,      "fp negate " TYPE) \
    X(fadd    ## NAME,           GROUP,       Rabc,     "fp add " TYPE) \
    X(fsub    ## NAME,           GROUP,       Rabc,     "fp subtract " TYPE) \
    X(fnadd   ## NAME,           GROUP,       Rabc,     "fp negate add " TYPE) \
    X(fabsd   ## NAME,           GROUP,       Rabc,     "fp absolute difference " TYPE) \
    X(fmul    ## NAME,           GROUP,       Rabc,     "fp multiply " TYPE) \
    X(fnmul   ## NAME,           GROUP,       Rabc,     "fp negate multiply " TYPE) \
    X(fnabsd  ## NAME,           GROUP,       Rabc,     "fp negate absolute difference " TYPE) \
    X(fcl     ## NAME,           GROUP,       FabSc,    "fp classify " TYPE) \
    X(frsqrt  ## NAME,           GROUP,       Rab,      "fp reciprocal square root " TYPE) \
    X(fsqrt   ## NAME,           GROUP,       Rab,      "fp square root " TYPE) \
    X(fdiv    ## NAME,           GROUP,       Rabc,     "fp divide " TYPE) \
    X(fmax    ## NAME,           GROUP,       Rabc,     "fp maximum " TYPE) \
    X(fmin    ## NAME,           GROUP,       Rabc,     "fp minimum " TYPE) \
    X(fmaxnm  ## NAME,           GROUP,       Rabc,     "fp maximum number " TYPE) \
    X(fminnm  ## NAME,           GROUP,       Rabc,     "fp minimum number " TYPE) \
    X(famax   ## NAME,           GROUP,       Rabc,     "fp absolute maximum " TYPE) \
    X(famin   ## NAME,           GROUP,       Rabc,     "fp absolute minimum " TYPE) \
    X(fmadd   ## NAME,           GROUP,       Rabcd,    "fp multiply-add " TYPE) \
    X(fmsub   ## NAME,           GROUP,       Rabcd,    "fp multiply-subtract " TYPE) \
    X(fnmadd  ## NAME,           GROUP,       Rabcd,    "fp negate multiply-add " TYPE) \
    X(fnmsub  ## NAME,           GROUP,       Rabcd,    "fp negate multiply-subtract " TYPE) \
    X(fmerge  ## NAME,           GROUP,       Rabcd,    "fp merge " TYPE) \
    X(frnd    ## NAME,           GROUP,       Round,    "fp round " TYPE) \
    X(fcmp    ## NAME ## oeq,    GROUP,       Rabc,     "fp compare " TYPE " ordered and equal") \
    X(fcmp    ## NAME ## one,    GROUP,       Rabc,     "fp compare " TYPE " ordered and not-equal") \
    X(fcmp    ## NAME ## olt,    GROUP,       Rabc,     "fp compare " TYPE " ordered and less") \
    X(fcmp    ## NAME ## ole,    GROUP,       Rabc,     "fp compare " TYPE " ordered and less-equal") \
    X(fcmp    ## NAME ## o,      GROUP,       Rabc,     "fp compare " TYPE " ordered") \
    X(fcmp    ## NAME ## ueq,    GROUP,       Rabc,     "fp compare " TYPE " unordered or equal") \
    X(fcmp    ## NAME ## une,    GROUP,       Rabc,     "fp compare " TYPE " unordered or not-equal") \
    X(fcmp    ## NAME ## ult,    GROUP,       Rabc,     "fp compare " TYPE " unordered or less") \
    X(fcmp    ## NAME ## ule,    GROUP,       Rabc,     "fp compare " TYPE " unordered or less-equal") \
    X(fcmp    ## NAME ## uo,     GROUP,       Rabc,     "fp compare " TYPE " unordered") \
    X(fcvt    ## NAME ## 2iw,    GROUP,       Round,    "fp convert " TYPE " to signed word") \
    X(fcvt    ## NAME ## 2uw,    GROUP,       Round,    "fp convert " TYPE " to unsigned word") \
    X(fcvt    ## NAME ## 2id,    GROUP,       Round,    "fp convert " TYPE " to signed doubleword") \
    X(fcvt    ## NAME ## 2ud,    GROUP,       Round,    "fp convert " TYPE " to unsigned doubleword") \
    X(fcvt    ## NAME ## 2iq,    GROUP,       Round,    "fp convert " TYPE " to signed quadword") \
    X(fcvt    ## NAME ## 2uq,    GROUP,       Round,    "fp convert " TYPE " to unsigned quadword") \
    X(fcvtiw2 ## NAME,           GROUP,       Round,    "fp convert signed word to" TYPE) \
    X(fcvtuw2 ## NAME,           GROUP,       Round,    "fp convert unsigned word to" TYPE) \
    X(fcvtid2 ## NAME,           GROUP,       Round,    "fp convert signed doubleword to" TYPE) \
    X(fcvtud2 ## NAME,           GROUP,       Round,    "fp convert unsigned doubleword to" TYPE) \
    X(fcvtiq2 ## NAME,           GROUP,       Round,    "fp convert signed quadword to" TYPE) \
    X(fcvtuq2 ## NAME,           GROUP,       Round,    "fp convert unsigned quadword" TYPE) \

INSN_FPU_SCALAR_COMMON(sh, fpuh, "scalar half")
INSN_FPU_SCALAR_COMMON(ss, fpus, "scalar single")
INSN_FPU_SCALAR_COMMON(sd, fpud, "scalar double")
INSN_FPU_SCALAR_COMMON(sq, fpuq, "scalar quadruple")

#define INSN_FPU_SCALAR_BRANCH(NAME, GROUP, TYPE) \
    X(nulf    ## NAME ## oeq,    nullifying,  r2m2,     "nullify if fp ordered and equal " TYPE) \
    X(nulf    ## NAME ## one,    nullifying,  r2m2,     "nullify if fp ordered and not-equal " TYPE) \
    X(nulf    ## NAME ## olt,    nullifying,  r2m2,     "nullify if fp ordered and less " TYPE) \
    X(nulf    ## NAME ## ole,    nullifying,  r2m2,     "nullify if fp ordered and less-or-equal " TYPE) \
    X(nulf    ## NAME ## o,      nullifying,  r2m2,     "nullify if fp ordered " TYPE) \
    X(nulf    ## NAME ## ueq,    nullifying,  r2m2,     "nullify if fp unordered or equal " TYPE) \
    X(nulf    ## NAME ## une,    nullifying,  r2m2,     "nullify if fp unordered or not-equal " TYPE) \
    X(nulf    ## NAME ## ult,    nullifying,  r2m2,     "nullify if fp unordered or less " TYPE) \
    X(nulf    ## NAME ## ule,    nullifying,  r2m2,     "nullify if fp unordered or less-or-equal " TYPE) \
    X(nulf    ## NAME ## uo,     nullifying,  r2m2,     "nullify if fp unordered " TYPE) \
    X(nulf    ## NAME ## class,  nullifying,  RaSbm2,   "nullify if fp classify " TYPE) \
    X(bf      ## NAME ## oeq,    GROUP,       RabBBS,   "branch if fp ordered and equal " TYPE) \
    X(bf      ## NAME ## one,    GROUP,       RabBBS,   "branch if fp ordered and not-equal " TYPE) \
    X(bf      ## NAME ## olt,    GROUP,       RabBBS,   "branch if fp ordered and less " TYPE) \
    X(bf      ## NAME ## ole,    GROUP,       RabBBS,   "branch if fp ordered and less-or-equal " TYPE) \
    X(bf      ## NAME ## o,      GROUP,       RabBBS,   "branch if fp ordered " TYPE) \
    X(bf      ## NAME ## ueq,    GROUP,       RabBBS,   "branch if fp unordered or equal " TYPE) \
    X(bf      ## NAME ## une,    GROUP,       RabBBS,   "branch if fp unordered or not-equal " TYPE) \
    X(bf      ## NAME ## ult,    GROUP,       RabBBS,   "branch if fp unordered or less " TYPE) \
    X(bf      ## NAME ## ule,    GROUP,       RabBBS,   "branch if fp unordered or less-or-equal " TYPE) \
    X(bf      ## NAME ## uo,     GROUP,       RabBBS,   "branch if fp unordered " TYPE) \
    X(bf      ## NAME ## class,  GROUP,       RaSbBBS,  "branch if fp classify " TYPE) \

INSN_FPU_SCALAR_BRANCH(ss, fpus, "scalar single")
INSN_FPU_SCALAR_BRANCH(sd, fpud, "scalar double")
INSN_FPU_SCALAR_BRANCH(sq, fpuq, "scalar quadruple")

#define INSN_FPU_VECTOR_COMMON(NAME, INAME, GROUP, TYPE, ITYPE) \
    X(fabs   ## NAME,                 GROUP,  Rab,    "fp absolute difference " TYPE) \
    X(fnabs  ## NAME,                 GROUP,  Rab,    "fp absolute value " TYPE) \
    X(fneg   ## NAME,                 GROUP,  Rab,    "fp negate " TYPE) \
    X(fadd   ## NAME,                 GROUP,  Rabc,   "fp add " TYPE) \
    X(fmul   ## NAME,                 GROUP,  Rabc,   "fp multiply " TYPE) \
    X(fnabsd ## NAME,                 GROUP,  Rabc,   "fp negate absolute difference " TYPE) \
    X(fnadd  ## NAME,                 GROUP,  Rabc,   "fp negate add " TYPE) \
    X(fnmul  ## NAME,                 GROUP,  Rabc,   "fp negate multiply " TYPE) \
    X(fsub   ## NAME,                 GROUP,  Rabc,   "fp subtract " TYPE) \
    X(fdiv   ## NAME,                 GROUP,  Rabc,   "fp divide " TYPE) \
    X(fabsd  ## NAME,                 GROUP,  Rabc,   "fp absolute difference " TYPE) \
    X(frsqrt ## NAME,                 GROUP,  Rab,    "fp reciprocal square root " TYPE) \
    X(fsqrt  ## NAME,                 GROUP,  Rab,    "fp square root " TYPE) \
    X(fmax   ## NAME,                 GROUP,  Rabc,   "fp maximum " TYPE) \
    X(fmin   ## NAME,                 GROUP,  Rabc,   "fp minimum " TYPE) \
    X(famax  ## NAME,                 GROUP,  Rabc,   "fp absolute maximum " TYPE) \
    X(famin  ## NAME,                 GROUP,  Rabc,   "fp absolute minimum " TYPE) \
    X(fdot   ## NAME,                 GROUP,  Rabc,   "fp dot-product " TYPE) \
    X(frnd   ## NAME,                 GROUP,  Round,  "fp round " TYPE) \
    X(fmadd  ## NAME,                 GROUP,  Rabcd,  "fp multiply-add " TYPE) \
    X(fmsub  ## NAME,                 GROUP,  Rabcd,  "fp multiply-subtract " TYPE) \
    X(fnmadd ## NAME,                 GROUP,  Rabcd,  "fp negate multiply-add " TYPE) \
    X(fnmsub ## NAME,                 GROUP,  Rabcd,  "fp negate multiply-subtract " TYPE) \
    X(fmadda ## NAME,                 GROUP,  Rabcd,  "fp multiply-alternating add-subtract " TYPE) \
    X(fmsuba ## NAME,                 GROUP,  Rabcd,  "fp multiply-alternating subtract-add " TYPE) \
    X(fmerge ## NAME,                 GROUP,  Rabcd,  "fp merge " TYPE) \
    X(fmrgh  ## NAME,                 GROUP,  Rabc,   "fp merge high parts " TYPE) \
    X(fmrgl  ## NAME,                 GROUP,  Rabc,   "fp merge low parts " TYPE) \
    X(faddc  ## NAME,                 GROUP,  Rabc,   "fp add conjugate " TYPE) \
    X(faddh  ## NAME,                 GROUP,  Rabc,   "fp add horizontal " TYPE) \
    X(fsubc  ## NAME,                 GROUP,  Rabc,   "fp subtract conjugate " TYPE) \
    X(fsubh  ## NAME,                 GROUP,  Rabc,   "fp subtract horizontal " TYPE) \
    X(fmulh  ## NAME,                 GROUP,  Rabc,   "fp multiply horizontal " TYPE) \
    X(fpk    ## NAME,                 GROUP,  Rabc,   "fp pack" TYPE) \
    X(funpkh ## NAME,                 GROUP,  Rab,    "fp unpack high part " TYPE) \
    X(funpkl ## NAME,                 GROUP,  Rab,    "fp unpack low part " TYPE) \
    X(fcmp   ## NAME ## oeq,          GROUP,  Rabc,   "fp compare " TYPE " ordered and equal") \
    X(fcmp   ## NAME ## one,          GROUP,  Rabc,   "fp compare " TYPE " ordered and not-equal") \
    X(fcmp   ## NAME ## olt,          GROUP,  Rabc,   "fp compare " TYPE " ordered and less") \
    X(fcmp   ## NAME ## ole,          GROUP,  Rabc,   "fp compare " TYPE " ordered and less-equal") \
    X(fcmp   ## NAME ## o,            GROUP,  Rabc,   "fp compare " TYPE " ordered") \
    X(fcmp   ## NAME ## ueq,          GROUP,  Rabc,   "fp compare " TYPE " unordered or equal") \
    X(fcmp   ## NAME ## une,          GROUP,  Rabc,   "fp compare " TYPE " unordered or not-equal") \
    X(fcmp   ## NAME ## ult,          GROUP,  Rabc,   "fp compare " TYPE " unordered or less") \
    X(fcmp   ## NAME ## ule,          GROUP,  Rabc,   "fp compare " TYPE " unordered or less-equal") \
    X(fcmp   ## NAME ## uo,           GROUP,  Rabc,   "fp compare " TYPE " unordered") \
    X(fcvti  ## INAME ## 2  ## NAME,  GROUP,  Round,  "fp convert signed " ITYPE " to " TYPE) \
    X(fcvtu  ## INAME ## 2  ## NAME,  GROUP,  Round,  "fp convert unsigned " ITYPE " to " TYPE) \
    X(fcvt   ## NAME  ## 2i ## INAME, GROUP,  Round,  "fp convert " TYPE " to signed " ITYPE) \
    X(fcvt   ## NAME  ## 2u ## INAME, GROUP,  Round,  "fp convert " TYPE " to unsigned " ITYPE) \

INSN_FPU_VECTOR_COMMON(ph, h, fpuh, "packed half", "halfwords")
INSN_FPU_VECTOR_COMMON(ps, w, fpus, "packed single", "words")
INSN_FPU_VECTOR_COMMON(pd, d, fpud, "packed double", "doublewords")

//===------------------------------------------------------===//
// custom fpu
//===------------------------------------------------------===//

X(fscale,  fpuq, FabSc, "fp scale")

X(fcvtsd2ss, fpud, Round, "fp convert scalar double to scalar single")
X(fcvtsq2sd, fpuq, Round, "fp convert scalar quadruple to scalar double")
X(fcvtsq2ss, fpuq, Round, "fp convert scalar quadruple to scalar single")
X(fcvtss2sh, fpuh, Round, "fp convert scalar single to scalar halffloat")
X(fcvtsd2sh, fpuh, Round, "fp convert scalar double to scalar halffloat")
X(fcvtsq2sh, fpuh, Round, "fp convert scalar quadruple to scalar halffloat")

X(fextsh2ss, fpuh, Rab, "fp extend scalar halffloat to scalar single")
X(fextsh2sd, fpuh, Rab, "fp extend scalar halffloat to scalar double")
X(fextsh2sq, fpuh, Rab, "fp extend scalar halffloat to scalar quadruple")
X(fextss2sd, fpus, Rab, "fp extend scalar single to scalar double")
X(fextss2sq, fpuq, Rab, "fp extend scalar single to scalar quadruple")
X(fextsd2sq, fpuq, Rab, "fp extend scalar double to scalar quadruple")


//===------------------------------------------------------===//
// integer vectors
//===------------------------------------------------------===//
#define INSN_MMX_COMMON(NAME, TYPE) \
    X(vaddc   ## NAME, mmx, Rabc,  "vector add carry-out " TYPE) \
    X(vaddo   ## NAME, mmx, Rabc,  "vector add overflow " TYPE) \
    X(vaddss  ## NAME, mmx, Rabc,  "vector add signed saturate " TYPE) \
    X(vaddu   ## NAME, mmx, Rabc,  "vector add unsigned " TYPE) \
    X(vaddus  ## NAME, mmx, Rabc,  "vector add unsigned saturate " TYPE) \
    X(vavgs   ## NAME, mmx, Rabc,  "vector average signed " TYPE) \
    X(vavgu   ## NAME, mmx, Rabc,  "vector average unsigned " TYPE) \
    X(vcmpeq  ## NAME, mmx, Rabc,  "vector compare equal " TYPE) \
    X(vcmplt  ## NAME, mmx, Rabc,  "vector compare less " TYPE) \
    X(vcmpltu ## NAME, mmx, Rabc,  "vector compare less unsigned " TYPE) \
    X(vmaxs   ## NAME, mmx, Rabc,  "vector maximum signed " TYPE) \
    X(vmaxu   ## NAME, mmx, Rabc,  "vector maximum unsigned " TYPE) \
    X(vmins   ## NAME, mmx, Rabc,  "vector minimum signed " TYPE) \
    X(vminu   ## NAME, mmx, Rabc,  "vector minimum unsigned " TYPE) \
    X(vmrgh   ## NAME, mmx, Rabc,  "vector merge high " TYPE) \
    X(vmrgl   ## NAME, mmx, Rabc,  "vector merge low " TYPE) \
    X(vrol    ## NAME, mmx, Rabc,  "vector rotate left " TYPE) \
    X(vror    ## NAME, mmx, Rabc,  "vector rotate right " TYPE) \
    X(vsll    ## NAME, mmx, Rabc,  "vector shift left logical " TYPE) \
    X(vslli   ## NAME, mmx, FabSc, "vector shift left logical immediate " TYPE) \
    X(vsra    ## NAME, mmx, Rabc,  "vector shift right algebraic " TYPE) \
    X(vsrai   ## NAME, mmx, FabSc, "vector shift right algebraic immediate " TYPE) \
    X(vsrl    ## NAME, mmx, Rabc,  "vector shift right logical " TYPE) \
    X(vsrli   ## NAME, mmx, FabSc, "vector shift right logical immediate " TYPE) \
    X(vsubb   ## NAME, mmx, Rabc,  "vector subtract borrow " TYPE) \
    X(vsubo   ## NAME, mmx, Rabc,  "vector subtract overflow " TYPE) \
    X(vsubss  ## NAME, mmx, Rabc,  "vector subtract signed saturated " TYPE) \
    X(vsubu   ## NAME, mmx, Rabc,  "vector subtract unsigned " TYPE) \
    X(vsubus  ## NAME, mmx, Rabc,  "vector subtract unsigned saturated " TYPE) \

INSN_MMX_COMMON(b, "byte")
INSN_MMX_COMMON(h, "halfword")
INSN_MMX_COMMON(w, "word")
INSN_MMX_COMMON(d, "doubleword")

//===------------------------------------------------------===//
// custom vector
//===------------------------------------------------------===//

X(vupkhsb,  mmx, Rab,   "vector unpack high signed byte")
X(vupklsb,  mmx, Rab,   "vector unpack low signed byte")

X(vpksssh,  mmx, Rabc,  "vector pack signed as signed saturate halfword")
X(vpksush,  mmx, Rabc,  "vector pack signed as unsigned saturate halfword")
X(vpkuumh,  mmx, Rabc,  "vector pack unsigned as unsigned modulo halfword")
X(vpkuush,  mmx, Rabc,  "vector pack unsigned as unsigned saturate halfword")
X(vupkhsh,  mmx, Rab,   "vector unpack high signed halfword")
X(vupklsh,  mmx, Rab,   "vector unpack low signed halfword")

X(vpksssw,  mmx, Rabc,  "vector pack signed as signed saturate word")
X(vpksusw,  mmx, Rabc,  "vector pack signed as unsigned saturate word")
X(vpkuumw,  mmx, Rabc,  "vector pack unsigned as unsigned modulo word")
X(vpkuusw,  mmx, Rabc,  "vector pack unsigned as unsigned saturate word")
X(vupkhsw,  mmx, Rab,   "vector unpack high signed word")
X(vupklsw,  mmx, Rab,   "vector unpack low signed word")

X(vpksssd,  mmx, Rabc,  "vector pack signed as signed saturate doubleword")
X(vpksusd,  mmx, Rabc,  "vector pack signed as unsigned saturate doubleword")
X(vpkuumd,  mmx, Rabc,  "vector pack unsigned as unsigned modulo doubleword")
X(vpkuusd,  mmx, Rabc,  "vector pack unsigned as unsigned saturate doubleword")

//===------------------------------------------------------===//
// end macrotable
//===------------------------------------------------------===//
