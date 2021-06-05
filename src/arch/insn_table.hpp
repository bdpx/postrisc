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
X(add,     base, r3, "add")
X(addadd,  base, r4, "add 3 values")
X(addaddc, base, r4, "add-add with carry-out")
X(addc,    base, r3, "add carry-out")
X(addi,    base, RabSI21, "add immediate")
X(addiws,  base, RabSI21, "add immediate word, sext")
X(addiwz,  base, RabSI21, "add immediate word, zext")
X(addo,    base, r3, "add overflow")
X(addsub,  base, r4, "add-subtract")
X(addws,    base, r3, "add word sign-extend")
X(addwz,    base, r3, "add word zero-extend")
X(sub,     base, r3, "subtract")
X(subb,    base, r3, "subtract with borrow")
X(subfi,   base, RabSI21, "subtract from immediate")
X(subfiws, base, RabSI21, "subtract from immediate word, sext")
X(subfiwz, base, RabSI21, "subtract from immediate word, zext")
X(subo,    base, r3, "subtract overflow")
X(subsub,  base, r4, "subtract twice")
X(subsubb, base, r4, "subtract-subtract with borrow-out")
X(subws,   base, r3, "subtract word sign-extend")
X(subwz,   base, r3, "subtract word zero-extend")

X(addq,    intq, r3, "add quadword")
X(subq,    intq, r3, "subtract quadword")

//===------------------------------------------------------===//
// bitwise
//===------------------------------------------------------===//
X(and,     base, r3, "and bitwise")
X(anddec,  base, r3, "and bitwise decremented")
X(andn,    base, r3, "and not bitwise")
X(andneg,  base, r3, "and bitwise negate")
X(nand,    base, r3, "bitwise not and")
X(nor,     base, r3, "bitwise not or")
X(or,      base, r3, "bitwise or")
X(orn,     base, r3, "bitwise or not")
X(xnor,    base, r3, "bitwise exclusive nor")
X(xor,     base, r3, "bitwise exclusive or")
X(xordec,  base, r3, "bitwise exclusive-or decremented")

X(andi,    base, RabSI21, "and bitwise with immediate")
X(andni,   base, RabSI21, "and not bitwise with immediate")
X(ori,     base, RabSI21, "bitwise or with immediate")
X(orni,    base, RabSI21, "bitwise or not immediate")
X(xori,    base, RabSI21, "bitwise exclusive or with immediate")
X(not,     base, Not, "bitwise not")

X(nop,     base, RaxU28,   "no operation")

//===------------------------------------------------------===//
//
//===------------------------------------------------------===//
X(absd,    base,  r3, "absolute difference")
X(absdw,   base,  r3, "absolute difference word")
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
X(ldih,    base,  ldi,     "load immediate high")
X(mov,     base,  r2, "move general register")
X(mov2,    base,  r4, "move 2 general registers")
X(mul,     base,  r3, "multiply")
X(mulws,   base,  r3, "multiply word sign extend")
X(mulwz,   base,  r3, "multiply word zero extend")
X(muladd,  base,  r4, "multiply-add")
X(mulh,    base,  r3, "multiply high")
X(muli,    base,  RabSI21,  "multiply immediate")
X(mulwsi,  base,  RabSI21,  "multiply word sign extended immediate")
X(mulwzi,  base,  RabUI21,  "multiply word zero extended immediate")
X(mulsub,  base,  r4, "multiply-subtract")
X(mulsubf, base,  r4, "multiply-subtract from")
X(rlmi,    base,  RabScde, "rotate left and mask immediate")
X(mbsel,   base,  r4, "masked bit selection")

X(depq,    intq,  FabcSd,  "deposit quadword")

//===------------------------------------------------------===//
// conditional move
//===------------------------------------------------------===//
X(cmovlsb, base, r4, "conditional move if least significand bit")
X(cmovweq, base, r4, "conditional move word if equal zero")
X(cmovwlt, base, r4, "conditional move word if less than zero")
X(cmovwle, base, r4, "conditional move word if less than or equal zero")
X(cmovdeq, base, r4, "conditional move doubleword if equal zero")
X(cmovdlt, base, r4, "conditional move doubleword if less than zero")
X(cmovdle, base, r4, "conditional move doubleword if less than or equal zero")

//===------------------------------------------------------===//
// shifts
//===------------------------------------------------------===//
X(sll,     base, r3, "shift left logical")
X(slli,    base, RabSc,  "shift left logical immediate")
X(slliw,   base, RabSc,  "shift left logical immediate word")
X(sllw,    base, r3, "shift left logical word")
X(slsra,   base, r4, "shift left and shift right algebraic")
X(slsrai,  base, RabScd, "shift left and shift right algebraic immediate")
X(slsrl,   base, r4, "shift left and shift right logical")
X(slsrli,  base, RabScd, "shift left and shift right logical immediate")
X(slsrlw,  base, r4, "shift left and shift right logical word")
X(slsraw,  base, r4, "shift left and shift right algebraic word")

X(sra,     base, r3, "shift right algebraic")
X(srai,    base, RabSc,  "shift right algebraic immediate")
X(sraiw,   base, RabSc,  "shift right algebraic immediate word")
X(sraw,    base, r3, "shift right algebraic word")
X(srd,     base, r3, "shift right dividing")
X(srdi,    base, RabSc,  "shift right dividing immediate")
X(srl,     base, r3, "shift right logical")
X(srli,    base, RabSc,  "shift right logical immediate")
X(srliw,   base, RabSc,  "shift right logical immediate word")
X(srlw,    base, r3, "shift right logical word")

X(sladd,     base, RabcSd, "shift left and add")
X(sladdws,   base, RabcSd, "shift left and add word sign-extend")
X(sladdwz,   base, RabcSd, "shift left and add word zero-extend")
X(slsub,     base, RabcSd, "shift left and subtract")
X(slsubws,   base, RabcSd, "shift left and subtract word sign-extend")
X(slsubwz,   base, RabcSd, "shift left and subtract word zero-extend")
X(slsubf,    base, RabcSd, "shift left and subtract from")
X(slsubfws,  base, RabcSd, "shift left and subtract from word sign-extend")
X(slsubfwz,  base, RabcSd, "shift left and subtract from zero sign-extend")
X(slor,      base, RabcSd, "shift left and or")
X(slxor,     base, RabcSd, "shift left and xor")

X(slpw,    base, r4, "shift left pair word")
X(srpw,    base, r4, "shift right pair word")
X(srpiw,   base, RabcSd, "shift right pair immediate word")
X(slp,     base, r4, "shift left pair")
X(srp,     base, r4, "shift right pair")
X(srpi,    base, RabcSd, "shift right pair immediate")
X(srpq,    base, r4, "shift right pair quadword")
X(slpq,    base, r4, "shift left pair quadword")
X(srpiq,   intq, FabcSd, "shift right pair immediate quadword")

//===------------------------------------------------------===//
// divide/modulo
//===------------------------------------------------------===//
X(div,     base, r3, "divide")
X(divu,    base, r3, "divide unsigned")
X(divw,    base, r3, "divide word")
X(divuw,   base, r3, "divide unsigned word")
X(mod,     base, r3, "module")
X(modu,    base, r3, "module unsigned")
X(moduw,   base, r3, "module unsigned word")
X(modw,    base, r3, "module word")

X(divi,    base, RabSI21, "divide immediate")
X(divui,   base, RabUI21, "divide unsigned immediate")
X(modi,    base, RabSI21, "module immediate")
X(modui,   base, RabUI21, "module unsigned immediate")

//===------------------------------------------------------===//
// min/max
//===------------------------------------------------------===//
X(minsw,    base, r3, "minimum signed word")
X(maxsw,    base, r3, "maximum signed word")
X(minuw,    base, r3, "minimum unsigned word")
X(maxuw,    base, r3, "maximum unsigned word")

X(mins,    base, r3, "minimum signed")
X(maxs,    base, r3, "maximum signed")
X(minu,    base, r3, "minimum unsigned")
X(maxu,    base, r3, "maximum unsigned")

X(minsi,   base, RabSI21, "minimum signed immediate")
X(maxsi,   base, RabSI21, "maximum signed immediate")
X(minui,   base, RabUI21, "minimum unsigned immediate")
X(maxui,   base, RabUI21, "maximum unsigned immediate")

//===------------------------------------------------------===//
// compare 64 bit
//===------------------------------------------------------===//
#define INSN_BASE_TYPED(NAME, TYPE) \
    X(cmp ## NAME ## eq,    base,        r3,      "compare " TYPE " equal") \
    X(cmp ## NAME ## ne,    base,        r3,      "compare " TYPE " not equal") \
    X(cmp ## NAME ## lt,    base,        r3,      "compare " TYPE " less") \
    X(cmp ## NAME ## ltu,   base,        r3,      "compare " TYPE " less unsigned") \
    X(cmp ## NAME ## le,    base,        r3,      "compare " TYPE " less or equal") \
    X(cmp ## NAME ## leu,   base,        r3,      "compare " TYPE " less or equal unsigned") \
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
X(cmpqeq,   intq, r3, "compare quadword equal")
X(cmpqne,   intq, r3, "compare quadword not equal")
X(cmpqlt,   intq, r3, "compare quadword less")
X(cmpqle,   intq, r3, "compare quadword less or equal")
X(cmpqltu,  intq, r3, "compare quadword less unsigned")
X(cmpqleu,  intq, r3, "compare quadword less or equal unsigned")
// pseudos
X(cmpqgt,   intq, r3, "compare quadword greater")
X(cmpqge,   intq, r3, "compare quadword greater or equal")
X(cmpqgtu,  intq, r3, "compare quadword greater unsigned")
X(cmpqgeu,  intq, r3, "compare quadword greater or equal unsigned")

//===------------------------------------------------------===//
// bit manipulation/counting
//===------------------------------------------------------===//
X(cntlz,   bitmanip, RabSc, "count leading zeros")
X(cntpop,  bitmanip, RabSc, "count population")
X(cnttz,   bitmanip, RabSc, "count trailing zeros")

X(gtb,     bitmanip, r2, "graycode to binary")
X(perm,    bitmanip, r4, "permute bytes")
X(permb,   bitmanip, RabSc, "permute bits")
X(mbgath,  bitmanip, r3, "masked bit gather")
X(mbscat,  bitmanip, r3, "masked bit scatter")

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
X(jmpt,   branch, Rbc,       "jump table")
X(jmptws, branch, Rbc,       "jump table word signed index")
X(jmptwz, branch, Rbc,       "jump table word unsigned index")

//===------------------------------------------------------===//
// call/return/regframe
//===------------------------------------------------------===//
X(callmi,  branch, RabS10,    "call memory indirect")
X(callmrw, branch, RabS10,    "call memory indirect relative word")
X(callr,   branch, callr,     "call relative")
X(callri,  branch, r3,        "call register indirect")
X(callrvt, branch, RabS10,    "call relative vtable")
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
X(random,  special, r2,      "random")
X(mfspr,   special, mspr,     "move from special-purpose register")
X(mtspr,   special, mspr,     "move to special-purpose register")
X(icbi,    special, RaxbSI21, "instruction cache block invalidate")
X(int,     special, RbS10,    "interrupt")
X(mprobe,  special, r3,     "memory probe access")
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
X(tpa,     privileged, r2, "translate to physical address")
X(ptc,     privileged, r3, "purge translation cache by page address and ASID")
X(mtdtr,   privileged, r3, "move to data translation register")
X(mtitr,   privileged, r3, "move to instruction translation register")

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
    X(ld ## NAME ## s,     memory, RabSI21, "load "  TYPE " sext") \
    X(ld ## NAME ## z,     memory, RabSI21, "load "  TYPE " zext") \
    X(st ## NAME,          memory, RabSI21, "store " TYPE) \
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
X(sllqi,      intq,   RabSc, "sllqi")
X(srlqi,      intq,   RabSc, "srlqi")
X(sraqi,      intq,   RabSc, "sraqi")

X(jmpmi,      special,   RbcScale,   "jmpmi")

//===------------------------------------------------------===//
// cipher/aes/hashes
//===------------------------------------------------------===//
X(aesdec,          cipher, r3, "aes decrypt round")
X(aesdeclast,      cipher, r3, "aes decrypt last round")
X(aesenc,          cipher, r3, "aes encrypt round")
X(aesenclast,      cipher, r3, "aes encrypt last round")
X(aesimc,          cipher, r2, "aes inverse mix columns")
X(aeskeygenassist, cipher, RabS10, "aes key generation assist")
X(clmulll,         cipher, r3, "carry-less multiply low-low")
X(clmulhl,         cipher, r3, "carry-less multiply high-low")
X(clmulhh,         cipher, r3, "carry-less multiply high-high")
X(crc32c,          cipher, r4, "crc32c")

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
    X(fabs    ## NAME,           GROUP,       r2,     "fp absolute value " TYPE) \
    X(fnabs   ## NAME,           GROUP,       r2,     "fp negate absolute value " TYPE) \
    X(fneg    ## NAME,           GROUP,       r2,     "fp negate " TYPE) \
    X(fadd    ## NAME,           GROUP,       r3,     "fp add " TYPE) \
    X(fsub    ## NAME,           GROUP,       r3,     "fp subtract " TYPE) \
    X(fnadd   ## NAME,           GROUP,       r3,     "fp negate add " TYPE) \
    X(fabsd   ## NAME,           GROUP,       r3,     "fp absolute difference " TYPE) \
    X(fmul    ## NAME,           GROUP,       r3,     "fp multiply " TYPE) \
    X(fnmul   ## NAME,           GROUP,       r3,     "fp negate multiply " TYPE) \
    X(fnabsd  ## NAME,           GROUP,       r3,     "fp negate absolute difference " TYPE) \
    X(fcl     ## NAME,           GROUP,       FabSc,  "fp classify " TYPE) \
    X(frsqrt  ## NAME,           GROUP,       r2,     "fp reciprocal square root " TYPE) \
    X(fsqrt   ## NAME,           GROUP,       r2,     "fp square root " TYPE) \
    X(fdiv    ## NAME,           GROUP,       r3,     "fp divide " TYPE) \
    X(fmax    ## NAME,           GROUP,       r3,     "fp maximum " TYPE) \
    X(fmin    ## NAME,           GROUP,       r3,     "fp minimum " TYPE) \
    X(fmaxnm  ## NAME,           GROUP,       r3,     "fp maximum number " TYPE) \
    X(fminnm  ## NAME,           GROUP,       r3,     "fp minimum number " TYPE) \
    X(famax   ## NAME,           GROUP,       r3,     "fp absolute maximum " TYPE) \
    X(famin   ## NAME,           GROUP,       r3,     "fp absolute minimum " TYPE) \
    X(fmadd   ## NAME,           GROUP,       r4,     "fp multiply-add " TYPE) \
    X(fmsub   ## NAME,           GROUP,       r4,     "fp multiply-subtract " TYPE) \
    X(fnmadd  ## NAME,           GROUP,       r4,     "fp negate multiply-add " TYPE) \
    X(fnmsub  ## NAME,           GROUP,       r4,     "fp negate multiply-subtract " TYPE) \
    X(fmerge  ## NAME,           GROUP,       r4,     "fp merge " TYPE) \
    X(frnd    ## NAME,           GROUP,       Round,  "fp round " TYPE) \
    X(fcmp    ## NAME ## oeq,    GROUP,       r3,     "fp compare " TYPE " ordered and equal") \
    X(fcmp    ## NAME ## one,    GROUP,       r3,     "fp compare " TYPE " ordered and not-equal") \
    X(fcmp    ## NAME ## olt,    GROUP,       r3,     "fp compare " TYPE " ordered and less") \
    X(fcmp    ## NAME ## ole,    GROUP,       r3,     "fp compare " TYPE " ordered and less-equal") \
    X(fcmp    ## NAME ## o,      GROUP,       r3,     "fp compare " TYPE " ordered") \
    X(fcmp    ## NAME ## ueq,    GROUP,       r3,     "fp compare " TYPE " unordered or equal") \
    X(fcmp    ## NAME ## une,    GROUP,       r3,     "fp compare " TYPE " unordered or not-equal") \
    X(fcmp    ## NAME ## ult,    GROUP,       r3,     "fp compare " TYPE " unordered or less") \
    X(fcmp    ## NAME ## ule,    GROUP,       r3,     "fp compare " TYPE " unordered or less-equal") \
    X(fcmp    ## NAME ## uo,     GROUP,       r3,     "fp compare " TYPE " unordered") \
    X(fcvt    ## NAME ## 2iw,    GROUP,       Round,  "fp convert " TYPE " to signed word") \
    X(fcvt    ## NAME ## 2uw,    GROUP,       Round,  "fp convert " TYPE " to unsigned word") \
    X(fcvt    ## NAME ## 2id,    GROUP,       Round,  "fp convert " TYPE " to signed doubleword") \
    X(fcvt    ## NAME ## 2ud,    GROUP,       Round,  "fp convert " TYPE " to unsigned doubleword") \
    X(fcvt    ## NAME ## 2iq,    GROUP,       Round,  "fp convert " TYPE " to signed quadword") \
    X(fcvt    ## NAME ## 2uq,    GROUP,       Round,  "fp convert " TYPE " to unsigned quadword") \
    X(fcvtiw2 ## NAME,           GROUP,       Round,  "fp convert signed word to" TYPE) \
    X(fcvtuw2 ## NAME,           GROUP,       Round,  "fp convert unsigned word to" TYPE) \
    X(fcvtid2 ## NAME,           GROUP,       Round,  "fp convert signed doubleword to" TYPE) \
    X(fcvtud2 ## NAME,           GROUP,       Round,  "fp convert unsigned doubleword to" TYPE) \
    X(fcvtiq2 ## NAME,           GROUP,       Round,  "fp convert signed quadword to" TYPE) \
    X(fcvtuq2 ## NAME,           GROUP,       Round,  "fp convert unsigned quadword" TYPE) \

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
    X(fabs   ## NAME,                 GROUP,  r2,    "fp absolute difference " TYPE) \
    X(fnabs  ## NAME,                 GROUP,  r2,    "fp absolute value " TYPE) \
    X(fneg   ## NAME,                 GROUP,  r2,    "fp negate " TYPE) \
    X(fadd   ## NAME,                 GROUP,  r3,    "fp add " TYPE) \
    X(fmul   ## NAME,                 GROUP,  r3,    "fp multiply " TYPE) \
    X(fnabsd ## NAME,                 GROUP,  r3,    "fp negate absolute difference " TYPE) \
    X(fnadd  ## NAME,                 GROUP,  r3,    "fp negate add " TYPE) \
    X(fnmul  ## NAME,                 GROUP,  r3,    "fp negate multiply " TYPE) \
    X(fsub   ## NAME,                 GROUP,  r3,    "fp subtract " TYPE) \
    X(fdiv   ## NAME,                 GROUP,  r3,    "fp divide " TYPE) \
    X(fabsd  ## NAME,                 GROUP,  r3,    "fp absolute difference " TYPE) \
    X(frsqrt ## NAME,                 GROUP,  r2,    "fp reciprocal square root " TYPE) \
    X(fsqrt  ## NAME,                 GROUP,  r2,    "fp square root " TYPE) \
    X(fmax   ## NAME,                 GROUP,  r3,    "fp maximum " TYPE) \
    X(fmin   ## NAME,                 GROUP,  r3,    "fp minimum " TYPE) \
    X(famax  ## NAME,                 GROUP,  r3,    "fp absolute maximum " TYPE) \
    X(famin  ## NAME,                 GROUP,  r3,    "fp absolute minimum " TYPE) \
    X(fdot   ## NAME,                 GROUP,  r3,    "fp dot-product " TYPE) \
    X(frnd   ## NAME,                 GROUP,  Round, "fp round " TYPE) \
    X(fmadd  ## NAME,                 GROUP,  r4,    "fp multiply-add " TYPE) \
    X(fmsub  ## NAME,                 GROUP,  r4,    "fp multiply-subtract " TYPE) \
    X(fnmadd ## NAME,                 GROUP,  r4,    "fp negate multiply-add " TYPE) \
    X(fnmsub ## NAME,                 GROUP,  r4,    "fp negate multiply-subtract " TYPE) \
    X(fmadda ## NAME,                 GROUP,  r4,    "fp multiply-alternating add-subtract " TYPE) \
    X(fmsuba ## NAME,                 GROUP,  r4,    "fp multiply-alternating subtract-add " TYPE) \
    X(fmerge ## NAME,                 GROUP,  r4,    "fp merge " TYPE) \
    X(fmrgh  ## NAME,                 GROUP,  r3,    "fp merge high parts " TYPE) \
    X(fmrgl  ## NAME,                 GROUP,  r3,    "fp merge low parts " TYPE) \
    X(faddc  ## NAME,                 GROUP,  r3,    "fp add conjugate " TYPE) \
    X(faddh  ## NAME,                 GROUP,  r3,    "fp add horizontal " TYPE) \
    X(fsubc  ## NAME,                 GROUP,  r3,    "fp subtract conjugate " TYPE) \
    X(fsubh  ## NAME,                 GROUP,  r3,    "fp subtract horizontal " TYPE) \
    X(fmulh  ## NAME,                 GROUP,  r3,    "fp multiply horizontal " TYPE) \
    X(fpk    ## NAME,                 GROUP,  r3,    "fp pack" TYPE) \
    X(funpkh ## NAME,                 GROUP,  r2,    "fp unpack high part " TYPE) \
    X(funpkl ## NAME,                 GROUP,  r2,    "fp unpack low part " TYPE) \
    X(fcmp   ## NAME ## oeq,          GROUP,  r3,    "fp compare " TYPE " ordered and equal") \
    X(fcmp   ## NAME ## one,          GROUP,  r3,    "fp compare " TYPE " ordered and not-equal") \
    X(fcmp   ## NAME ## olt,          GROUP,  r3,    "fp compare " TYPE " ordered and less") \
    X(fcmp   ## NAME ## ole,          GROUP,  r3,    "fp compare " TYPE " ordered and less-equal") \
    X(fcmp   ## NAME ## o,            GROUP,  r3,    "fp compare " TYPE " ordered") \
    X(fcmp   ## NAME ## ueq,          GROUP,  r3,    "fp compare " TYPE " unordered or equal") \
    X(fcmp   ## NAME ## une,          GROUP,  r3,    "fp compare " TYPE " unordered or not-equal") \
    X(fcmp   ## NAME ## ult,          GROUP,  r3,    "fp compare " TYPE " unordered or less") \
    X(fcmp   ## NAME ## ule,          GROUP,  r3,    "fp compare " TYPE " unordered or less-equal") \
    X(fcmp   ## NAME ## uo,           GROUP,  r3,    "fp compare " TYPE " unordered") \
    X(fcvti  ## INAME ## 2  ## NAME,  GROUP,  Round, "fp convert signed " ITYPE " to " TYPE) \
    X(fcvtu  ## INAME ## 2  ## NAME,  GROUP,  Round, "fp convert unsigned " ITYPE " to " TYPE) \
    X(fcvt   ## NAME  ## 2i ## INAME, GROUP,  Round, "fp convert " TYPE " to signed " ITYPE) \
    X(fcvt   ## NAME  ## 2u ## INAME, GROUP,  Round, "fp convert " TYPE " to unsigned " ITYPE) \

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

X(fextsh2ss, fpuh, r2, "fp extend scalar halffloat to scalar single")
X(fextsh2sd, fpuh, r2, "fp extend scalar halffloat to scalar double")
X(fextsh2sq, fpuh, r2, "fp extend scalar halffloat to scalar quadruple")
X(fextss2sd, fpus, r2, "fp extend scalar single to scalar double")
X(fextss2sq, fpuq, r2, "fp extend scalar single to scalar quadruple")
X(fextsd2sq, fpuq, r2, "fp extend scalar double to scalar quadruple")


//===------------------------------------------------------===//
// integer vectors
//===------------------------------------------------------===//
#define INSN_MMX_COMMON(NAME, TYPE) \
    X(vaddc   ## NAME, mmx, r3,  "vector add carry-out " TYPE) \
    X(vaddo   ## NAME, mmx, r3,  "vector add overflow " TYPE) \
    X(vaddss  ## NAME, mmx, r3,  "vector add signed saturate " TYPE) \
    X(vaddu   ## NAME, mmx, r3,  "vector add unsigned " TYPE) \
    X(vaddus  ## NAME, mmx, r3,  "vector add unsigned saturate " TYPE) \
    X(vavgs   ## NAME, mmx, r3,  "vector average signed " TYPE) \
    X(vavgu   ## NAME, mmx, r3,  "vector average unsigned " TYPE) \
    X(vcmpeq  ## NAME, mmx, r3,  "vector compare equal " TYPE) \
    X(vcmplt  ## NAME, mmx, r3,  "vector compare less " TYPE) \
    X(vcmpltu ## NAME, mmx, r3,  "vector compare less unsigned " TYPE) \
    X(vmaxs   ## NAME, mmx, r3,  "vector maximum signed " TYPE) \
    X(vmaxu   ## NAME, mmx, r3,  "vector maximum unsigned " TYPE) \
    X(vmins   ## NAME, mmx, r3,  "vector minimum signed " TYPE) \
    X(vminu   ## NAME, mmx, r3,  "vector minimum unsigned " TYPE) \
    X(vmrgh   ## NAME, mmx, r3,  "vector merge high " TYPE) \
    X(vmrgl   ## NAME, mmx, r3,  "vector merge low " TYPE) \
    X(vrol    ## NAME, mmx, r3,  "vector rotate left " TYPE) \
    X(vror    ## NAME, mmx, r3,  "vector rotate right " TYPE) \
    X(vsll    ## NAME, mmx, r3,  "vector shift left logical " TYPE) \
    X(vslli   ## NAME, mmx, FabSc, "vector shift left logical immediate " TYPE) \
    X(vsra    ## NAME, mmx, r3,  "vector shift right algebraic " TYPE) \
    X(vsrai   ## NAME, mmx, FabSc, "vector shift right algebraic immediate " TYPE) \
    X(vsrl    ## NAME, mmx, r3,  "vector shift right logical " TYPE) \
    X(vsrli   ## NAME, mmx, FabSc, "vector shift right logical immediate " TYPE) \
    X(vsubb   ## NAME, mmx, r3,  "vector subtract borrow " TYPE) \
    X(vsubo   ## NAME, mmx, r3,  "vector subtract overflow " TYPE) \
    X(vsubss  ## NAME, mmx, r3,  "vector subtract signed saturated " TYPE) \
    X(vsubu   ## NAME, mmx, r3,  "vector subtract unsigned " TYPE) \
    X(vsubus  ## NAME, mmx, r3,  "vector subtract unsigned saturated " TYPE) \

INSN_MMX_COMMON(b, "byte")
INSN_MMX_COMMON(h, "halfword")
INSN_MMX_COMMON(w, "word")
INSN_MMX_COMMON(d, "doubleword")

//===------------------------------------------------------===//
// custom vector
//===------------------------------------------------------===//

X(vupkhsb,  mmx, r2, "vector unpack high signed byte")
X(vupklsb,  mmx, r2, "vector unpack low signed byte")

X(vpksssh,  mmx, r3, "vector pack signed as signed saturate halfword")
X(vpksush,  mmx, r3, "vector pack signed as unsigned saturate halfword")
X(vpkuumh,  mmx, r3, "vector pack unsigned as unsigned modulo halfword")
X(vpkuush,  mmx, r3, "vector pack unsigned as unsigned saturate halfword")
X(vupkhsh,  mmx, r2, "vector unpack high signed halfword")
X(vupklsh,  mmx, r2, "vector unpack low signed halfword")

X(vpksssw,  mmx, r3, "vector pack signed as signed saturate word")
X(vpksusw,  mmx, r3, "vector pack signed as unsigned saturate word")
X(vpkuumw,  mmx, r3, "vector pack unsigned as unsigned modulo word")
X(vpkuusw,  mmx, r3, "vector pack unsigned as unsigned saturate word")
X(vupkhsw,  mmx, r2, "vector unpack high signed word")
X(vupklsw,  mmx, r2, "vector unpack low signed word")

X(vpksssd,  mmx, r3, "vector pack signed as signed saturate doubleword")
X(vpksusd,  mmx, r3, "vector pack signed as unsigned saturate doubleword")
X(vpkuumd,  mmx, r3, "vector pack unsigned as unsigned modulo doubleword")
X(vpkuusd,  mmx, r3, "vector pack unsigned as unsigned saturate doubleword")

//===------------------------------------------------------===//
// end macrotable
//===------------------------------------------------------===//
