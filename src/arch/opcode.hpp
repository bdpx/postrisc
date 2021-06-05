#pragma once

#include "instruction_fields.hpp"

//===------------------------------------------------------===//
// instruction mnemonics
// X(mnemonic, code, ident, subset, format, description)
//   mnemonic:    assembler mnemonic
//   code:        internal encoding
//   subset:      base, f128, f64, f32, f16, mmx, privileged, special, atomic, cipher
//   format:      assembler syntax format and instruction slot format
//   description: full instruction name
//===------------------------------------------------------===//

namespace postrisc {

/***************************************************************************
* primary opcode - 7 bit
***************************************************************************/
#define DECLARE_PRIMARY_OPCODES(X) \
    X(misc,        0, group,  noargs,    "extended group: misc") \
    X(fused,       1, group,  noargs,    "extended group: fused") \
    X(raopx,       2, group,  noargs,    "extended group: raopx") \
    X(brh,         3, group,  noargs,    "extended group: brh") \
    X(loop,        4, group,  noargs,    "extended group: loop") \
    X(brw,         5, group,  noargs,    "extended group: brw") \
    X(brd,         6, group,  noargs,    "extended group: brd") \
    X(brq,         7, group,  noargs,    "extended group: brq") \
    X(memxsd,      8, group,  noargs,    "extended group: memxsd") \
    X(memxsw,      9, group,  noargs,    "extended group: memxsw") \
    X(memxuw,     10, group,  noargs,    "extended group: memxuw") \
    X(memxud,     11, group,  noargs,    "extended group: memxud") \
    X(fmah,       12, group,  noargs,    "extended group: fmah") \
    X(fmas,       13, group,  noargs,    "extended group: fmas") \
    X(fmad,       14, group,  noargs,    "extended group: fmad") \
    X(fmaq,       15, group,  noargs,    "extended group: fmaq") \
    X(ldi,        16, base,   ldi,       "load immediate") \
    X(ldih,       17, base,   ldi,       "load immediate high") \
    X(andni,      18, base,   bin_simm,  "bitwise and-not with immediate") \
    X(orni,       19, base,   bin_simm,  "bitwise or-not immediate") \
    X(andi,       20, base,   bin_simm,  "and bitwise with immediate") \
    X(ori,        21, base,   bin_simm,  "bitwise or with immediate") \
    X(xori,       22, base,   bin_simm,  "bitwise exclusive or with immediate") \
    X(deposit,    23, base,   r3s2,      "deposit") \
    X(addiws,     24, base,   bin_simm,  "add immediate word sign-extend") \
    X(addiwz,     25, base,   bin_simm,  "add immediate word zero-extend") \
    X(subriws,    26, base,   bin_simm,  "subtract reverse immediate word sign-extend") \
    X(subriwz,    27, base,   bin_simm,  "subtract reverse immediate word zero-extend") \
    X(muliws,     28, base,   bin_simm,  "multiply immediate word sign-extend") \
    X(muliwz,     29, base,   bin_uimm,  "multiply immediate word zero-extend") \
    X(addid,      30, base,   bin_simm,  "add immediate doubleword") \
    X(subrid,     31, base,   bin_simm,  "subtract reverse immediate doubleword") \
    X(mulid,      32, base,   bin_simm,  "multiply immediate doubleword") /* reserved: 33-35 */ \
    X(divsid,     36, base,   bin_simm,  "divide signed immediate doubleword") \
    X(divuid,     37, base,   bin_uimm,  "divide unsigned immediate doubleword") \
    X(remsid,     38, base,   bin_simm,  "remainder signed immediate doubleword") \
    X(remuid,     39, base,   bin_uimm,  "remainder unsigned immediate doubleword") \
    X(divsiw,     40, base,   bin_simm,  "divide signed immediate word") \
    X(divuiw,     41, base,   bin_uimm,  "divide unsigned immediate word") \
    X(remsiw,     42, base,   bin_simm,  "remainder signed immediate word") \
    X(remuiw,     43, base,   bin_uimm,  "remainder unsigned immediate word") \
    X(maxsid,     44, base,   bin_simm,  "maximum signed immediate doubleword") \
    X(maxuid,     45, base,   bin_uimm,  "maximum unsigned immediate doubleword") \
    X(minsid,     46, base,   bin_simm,  "minimum signed immediate doubleword") \
    X(minuid,     47, base,   bin_uimm,  "minimum unsigned immediate doubleword") \
    X(maxsiw,     48, base,   bin_simm,  "maximum signed immediate word") \
    X(maxuiw,     49, base,   bin_uimm,  "maximum unsigned immediate word") \
    X(minsiw,     50, base,   bin_simm,  "minimum signed immediate word") \
    X(minuiw,     51, base,   bin_uimm,  "minimum unsigned immediate word") \
    X(cmpeqid,    52, base,   bin_simm,  "compare equal immediate doubleword") \
    X(cmpneid,    53, base,   bin_simm,  "compare not equal immediate doubleword") \
    X(cmpltsid,   54, base,   bin_simm,  "compare less than signed immediate doubleword") \
    X(cmpgesid,   55, base,   bin_simm,  "compare greater or equal signed immediate doubleword") \
    X(cmpltuid,   56, base,   bin_uimm,  "compare less than unsigned immediate doubleword") \
    X(cmpgeuid,   57, base,   bin_uimm,  "compare greater or equal unsigned immediate doubleword") \
    X(cmpeqiw,    58, base,   bin_simm,  "compare equal immediate word") \
    X(cmpneiw,    59, base,   bin_simm,  "compare not equal immediate word") \
    X(cmpltsiw,   60, base,   bin_simm,  "compare less than signed immediate word") \
    X(cmpgesiw,   61, base,   bin_simm,  "compare greater or equal signed immediate word") \
    X(cmpltuiw,   62, base,   bin_uimm,  "compare less than unsigned immediate word") \
    X(cmpgeuiw,   63, base,   bin_uimm,  "compare greater or equal unsigned immediate word") \
    X(cmpeqiq,    64, i128,   bin_simm,  "compare equal immediate quadword") \
    X(cmpneiq,    65, i128,   bin_simm,  "compare not equal immediate quadword") \
    X(cmpltsiq,   66, i128,   bin_simm,  "compare less than signed immediate quadword") \
    X(cmpgesiq,   67, i128,   bin_simm,  "compare greater or equal signed immediate quadword") \
    X(cmpltuiq,   68, i128,   bin_uimm,  "compare less than unsigned immediate quadword") \
    X(cmpgeuiq,   69, i128,   bin_uimm,  "compare greater or equal unsigned immediate quadword") \
    X(addiq,      70, i128,   bin_simm,  "add immediate quadword") /* reserved: 71 */ \
    X(ldub,       72, memory, bin_simm,  "load unsigned byte") \
    X(ldsb,       73, memory, bin_simm,  "load signed byte") \
    X(stb,        74, memory, bin_simm,  "store byte") \
    X(lduh,       75, memory, bin_simm,  "load unsigned halfword") \
    X(ldsh,       76, memory, bin_simm,  "load signed halfword") \
    X(sth,        77, memory, bin_simm,  "store halfword") \
    X(lduw,       78, memory, bin_simm,  "load unsigned word") \
    X(ldsw,       79, memory, bin_simm,  "load signed word") \
    X(stw,        80, memory, bin_simm,  "store word") \
    X(ldud,       81, memory, bin_simm,  "load unsigned doubleword") \
    X(ldsd,       82, memory, bin_simm,  "load signed doubleword") \
    X(std,        83, memory, bin_simm,  "store doubleword") \
    X(ldq,        84, memory, bin_simm,  "load quadword") \
    X(stq,        85, memory, bin_simm,  "store quadword") /* reserved: 86, 87 */ \
    X(ldubr,      88, memory, memrel,    "load unsigned byte ip-relative") \
    X(ldsbr,      89, memory, memrel,    "load signed byte ip-relative") \
    X(stbr,       90, memory, memrel,    "store byte ip-relative") \
    X(lduhr,      91, memory, memrel,    "load unsigned halfword ip-relative") \
    X(ldshr,      92, memory, memrel,    "load signed halfword ip-relative") \
    X(sthr,       93, memory, memrel,    "store halfword ip-relative") \
    X(lduwr,      94, memory, memrel,    "load unsigned word ip-relative") \
    X(ldswr,      95, memory, memrel,    "load signed word ip-relative") \
    X(stwr,       96, memory, memrel,    "store word ip-relative") \
    X(ldudr,      97, memory, memrel,    "load unsigned doubleword ip-relative") \
    X(ldsdr,      98, memory, memrel,    "load signed doubleword ip-relative") \
    X(stdr,       99, memory, memrel,    "store doubleword ip-relative") \
    X(ldqr,      100, memory, memrel,    "load quadword ip-relative") \
    X(stqr,      101, memory, memrel,    "store quadword ip-relative") \
    X(ldard,     102, memory, memrel,    "load address relative data") \
    X(callplt,   103, branch, memrel,    "call procedure linkage table") \
    X(call,      104, branch, call,      "call relative") \
    X(ldarc,     105, base,   ldarc,     "load address relative code") \
    X(breqiw,    106, branch, br_simm,   "branch if equal immediate word") \
    X(brneiw,    107, branch, br_simm,   "branch if not equal immediate word") \
    X(brltsiw,   108, branch, br_simm,   "branch if less than signed immediate word") \
    X(brgesiw,   109, branch, br_simm,   "branch if greater or equal signed immediate word") \
    X(brltuiw,   110, branch, br_uimm,   "branch if less than unsigned immediate word") \
    X(brgeuiw,   111, branch, br_uimm,   "branch if greater or equal immediate unsigned word") \
    X(breqid,    112, branch, br_simm,   "branch if equal immediate doubleword") \
    X(brneid,    113, branch, br_simm,   "branch if not equal immediate doubleword") \
    X(brltsid,   114, branch, br_simm,   "branch if less than signed immediate doubleword") \
    X(brgesid,   115, branch, br_simm,   "branch if greater or equal signed immediate doubleword") \
    X(brltuid,   116, branch, br_uimm,   "branch if less than unsigned immediate doubleword") \
    X(brgeuid,   117, branch, br_uimm,   "branch if greater or equal unsigned immediate doubleword") \
    X(breqiq,    118, i128,   br_simm,   "branch if equal immediate quadword") \
    X(brneiq,    119, i128,   br_simm,   "branch if not equal immediate quadword") \
    X(brltsiq,   120, i128,   br_simm,   "branch if less than signed immediate quadword") \
    X(brgesiq,   121, i128,   br_simm,   "branch if greater or equal signed immediate quadword") \
    X(brltuiq,   122, i128,   br_uimm,   "branch if less than unsigned immediate quadword") \
    X(brgeuiq,   123, i128,   br_uimm,   "branch if greater or equal unsigned immediate quadword") \
    X(brmall,    124, branch, br_uimm,   "branch if mask immediate all bits set") \
    X(brmnotall, 125, branch, br_uimm,   "branch if mask immediate not all bits set") \
    X(brmnone,   126, branch, br_uimm,   "branch if mask immediate none bit set") \
    X(brmany,    127, branch, br_uimm,   "branch if mask immediate any bit set") \


/**************************************************************************************
* 'fused' extended opcodes (7 bit)
**************************************************************************************/
#define DECLARE_FUSED_EXTENDED_OPCODES(X) \
    X(nulh,         0, group,   noargs,  "extended group: nulh") \
    X(nulw,         1, group,   noargs,  "extended group: nulw") \
    X(nuld,         2, group,   noargs,  "extended group: nuld") \
    X(nulq,         3, group,   noargs,  "extended group: nulq") /* reserved 4-7 */ \
    X(addaddd,      8, base,    r4,      "add-add  doubleword") \
    X(addsubd,      9, base,    r4,      "add-subtract doubleword") \
    X(subsubd,     10, base,    r4,      "subtract-subtract  doubleword") \
    X(addc3d,      11, base,    r4,      "add carry in-out doubleword") \
    X(subc3d,      12, base,    r4,      "subtract carry in-out doubleword") \
    X(slpw,        13, base,    r4,      "shift left pair word") \
    X(srpw,        14, base,    r4,      "shift right pair word") \
    X(srpiw,       15, base,    r3s1,    "shift right pair immediate word") \
    X(slpd,        16, base,    r4,      "shift left pair doubleword") \
    X(srpd,        17, base,    r4,      "shift right pair doubleword") \
    X(srpid,       18, base,    r3s1,    "shift right pair immediate doubleword") \
    X(slsrld,      19, base,    r4,      "shift left and shift right logical doubleword") \
    X(slsrad,      20, base,    r4,      "shift left and shift right algebraic doubleword") \
    X(slsrlid,     21, base,    r2s2,    "shift left and right logical immediate doubleword") \
    X(slsraid,     22, base,    r2s2,    "shift left and right algebraic immediate doubleword") \
    X(sladdd,      23, base,    r3s1,    "shift left and add doubleword") \
    X(slsubd,      24, base,    r3s1,    "shift left and subtract doubleword") \
    X(slsubrd,     25, base,    r3s1,    "shift left and subtract reverse doubleword") \
    X(slsrlw,      26, base,    r4,      "shift left and shift right logical word") \
    X(slsraw,      27, base,    r4,      "shift left and shift right algebraic word") \
    X(bitslct,     28, base,    r4,      "bit selection") \
    X(vpermb,      29, mmx,     r4,      "vector permute bytes") \
    X(mov2,        30, base,    r4,      "move 2 general registers") \
    X(alignup,     31, base,    r2s2,    "align up shifted") /* reserved: 32-36 */ \
    X(sladdws,     37, base,    r3s1,    "shift left and add word sign-extend") \
    X(sladdwz,     38, base,    r3s1,    "shift left and add word zero-extend") \
    X(slsubrws,    39, base,    r3s1,    "shift left and subtract reverse word sign-extend") \
    X(slsubrwz,    40, base,    r3s1,    "shift left and subtract reverse word zero-extend") \
    X(slsubws,     41, base,    r3s1,    "shift left and subtract word sign-extend") \
    X(slsubwz,     42, base,    r3s1,    "shift left and subtract word zero-extend") \
    X(depositv,    43, base,    r4,      "deposit variable") \
    X(slpq,        44, i128,    r4,      "shift left pair quadword") \
    X(srpq,        45, i128,    r4,      "shift right pair quadword") \
    X(srpiq,       46, i128,    r3s1,    "shift right pair immediate quadword") \
    X(slor,        50, base,    r3s1,    "shift left and or") \
    X(slxor,       51, base,    r3s1,    "shift left and xor") /* reserved: 52-59 */ \
    X(cmovlsb,     60, base,    r4,      "conditionally move if least significand bit") \
    X(cmoveqw,     61, base,    r4,      "conditionally move if equal zero word") \
    X(cmovltw,     62, base,    r4,      "conditionally move if less than zero word") \
    X(cmovlew,     63, base,    r4,      "conditionally move if less than or equal zero word") \
    X(cmoveqd,     65, base,    r4,      "conditionally move if equal zero doubleword") \
    X(cmovltd,     66, base,    r4,      "conditionally move if less than zero doubleword") \
    X(cmovled,     67, base,    r4,      "conditionally move if less than or equal zero doubleword") \
    X(cmoveqq,     69, i128,    r4,      "conditionally move if equal zero quadword") \
    X(cmovltq,     70, i128,    r4,      "conditionally move if less than zero quadword") \
    X(cmovleq,     71, i128,    r4,      "conditionally move if less than or equal zero quadword") \
    X(muladdd,     72, base,    r4,      "multiply-add doubleword") \
    X(mulsubd,     73, base,    r4,      "multiply-subtract doubleword") \
    X(mulsubrd,    74, base,    r4,      "multiply-subtract reverse doubleword") \
    X(fmergeh,     75, f16,     r4,      "float merge (sign,exponent,fraction) half") \
    X(fmerges,     76, f32,     r4,      "float merge (sign,exponent,fraction) single") \
    X(fmerged,     77, f64,     r4,      "float merge (sign,exponent,fraction) double") \
    X(fmergeq,     78, f128,    r4,      "float merge (sign,exponent,fraction) quadruple") \
    X(vfmergeh,    79, f16,     r4,      "vector float merge (sign,exponent,fraction) half") \
    X(vfmerges,    80, f32,     r4,      "vector float merge (sign,exponent,fraction) single") \
    X(vfmerged,    81, f64,     r4,      "vector float merge (sign,exponent,fraction) double") \
    X(vsrpib,      82, mmx,     r3s1,    "vector shift right pair immediate byte") \
    X(vsrpih,      83, mmx,     r3s1,    "vector shift right pair immediate halfword") \
    X(vsrpiw,      84, mmx,     r3s1,    "vector shift right pair immediate word") \
    X(vsrpid,      85, mmx,     r3s1,    "vector shift right pair immediate doubleword") \
    X(vslpb,       86, mmx,     r4,      "vector shift left pair byte") \
    X(vslph,       87, mmx,     r4,      "vector shift left pair halfword") \
    X(vslpw,       88, mmx,     r4,      "vector shift left pair word") \
    X(vslpd,       89, mmx,     r4,      "vector shift left pair doubleword") \
    X(vsrpb,       90, mmx,     r4,      "vector shift right pair byte") \
    X(vsrph,       91, mmx,     r4,      "vector shift right pair halfword") \
    X(vsrpw,       92, mmx,     r4,      "vector shift right pair word") \
    X(vsrpd,       93, mmx,     r4,      "vector shift right pair doubleword") \
    X(callmi,     100, branch,  mid_bin_simm, "call memory indirect") \
    X(callrvt,    101, branch,  mid_bin_simm, "call relative vtable") \
    X(ldan,       102, base,    mid_bin_simm, "load address near") \
    X(ldanrc,     103, base,    mid_bin_simm, "load address near relative") /* reserved 104, 105 */ \
    X(nuleqiw,    106, branch,  nul_simm,     "nullify if equal immediate word") \
    X(nulneiw,    107, branch,  nul_simm,     "nullify if not-equal immediate word") \
    X(nulltsiw,   108, branch,  nul_simm,     "nullify if less than signed immediate word") \
    X(nulgesiw,   109, branch,  nul_simm,     "nullify if greater or equal signed immediate word") \
    X(nulltuiw,   110, branch,  nul_uimm,     "nullify if less than unsigned immediate word") \
    X(nulgeuiw,   111, branch,  nul_uimm,     "nullify if greater or equal unsigned immediate word") \
    X(nuleqid,    112, branch,  nul_simm,     "nullify if equal immediate doubleword") \
    X(nulneid,    113, branch,  nul_simm,     "nullify if not-equal immediate doubleword") \
    X(nulltsid,   114, branch,  nul_simm,     "nullify if less than signed immediate doubleword") \
    X(nulgesid,   115, branch,  nul_simm,     "nullify if greater or equal signed immediate doubleword") \
    X(nulltuid,   116, branch,  nul_uimm,     "nullify if less than unsigned immediate doubleword") \
    X(nulgeuid,   117, branch,  nul_uimm,     "nullify if greater or equal unsigned immediate doubleword") \
    X(nuleqiq,    118, i128,    nul_simm,     "nullify if equal immediate quadword") \
    X(nulneiq,    119, i128,    nul_simm,     "nullify if not-equal immediate quadword") \
    X(nulltsiq,   120, i128,    nul_simm,     "nullify if less than signed immediate quadword") \
    X(nulgesiq,   121, i128,    nul_simm,     "nullify if greater or equal signed immediate quadword") \
    X(nulltuiq,   122, i128,    nul_uimm,     "nullify if less than unsigned immediate quadword") \
    X(nulgeuiq,   123, i128,    nul_uimm,     "nullify if greater or equal unsigned immediate quadword") \
    X(nulmall,    124, branch,  nul_uimm,     "nullify if mask immediate all bits set") \
    X(nulmnotall, 125, branch,  nul_uimm,     "nullify if mask immediate not all bits set") \
    X(nulmnone,   126, branch,  nul_uimm,     "nullify if mask immediate none bit set") \
    X(nulmany,    127, branch,  nul_uimm,     "nullify if mask immediate any bit set") \


/**************************************************************************************
* 'fma' extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_FMAH_EXTENDED_OPCODES(X) \
    X(fmaddh,     0, f16, r4rm, "float multiply-add half") \
    X(fmsubh,     1, f16, r4rm, "float multiply-subtract half") \
    X(fnmaddh,    2, f16, r4rm, "float negate multiply-add half") \
    X(fnmsubh,    3, f16, r4rm, "float negate multiply-subtract half") \
    X(vfmaddh,    4, f16, r4rm, "vector float multiply-add half") \
    X(vfmsubh,    5, f16, r4rm, "vector float multiply-subtract half") \
    X(vfnmaddh,   6, f16, r4rm, "vector float negate multiply-add half") \
    X(vfnmsubh,   7, f16, r4rm, "vector float negate multiply-subtract half") \
    X(vfmaddsubh, 8, f16, r4rm, "vector float multiply-add|subtract alternating half") \
    X(vfmsubaddh, 9, f16, r4rm, "vector float multiply-subtract|add alternating half") \

#define DECLARE_FMAS_EXTENDED_OPCODES(X) \
    X(fmadds,     0, f32, r4rm, "float multiply-add single") \
    X(fmsubs,     1, f32, r4rm, "float multiply-subtract single") \
    X(fnmadds,    2, f32, r4rm, "float negate multiply-add single") \
    X(fnmsubs,    3, f32, r4rm, "float negate multiply-subtract single") \
    X(vfmadds,    4, f32, r4rm, "vector float multiply-add single") \
    X(vfmsubs,    5, f32, r4rm, "vector float multiply-subtract single") \
    X(vfnmadds,   6, f32, r4rm, "vector float negate multiply-add single") \
    X(vfnmsubs,   7, f32, r4rm, "vector float negate multiply-subtract single") \
    X(vfmaddsubs, 8, f32, r4rm, "vector float multiply-add|subtract alternating single") \
    X(vfmsubadds, 9, f32, r4rm, "vector float multiply-subtract|add alternating single") \

#define DECLARE_FMAD_EXTENDED_OPCODES(X) \
    X(fmaddd,     0, f64, r4rm, "float multiply-add double") \
    X(fmsubd,     1, f64, r4rm, "float multiply-subtract double") \
    X(fnmaddd,    2, f64, r4rm, "float negate multiply-add double") \
    X(fnmsubd,    3, f64, r4rm, "float negate multiply-subtract double") \
    X(vfmaddd,    4, f64, r4rm, "vector float multiply-add double") \
    X(vfmsubd,    5, f64, r4rm, "vector float multiply-subtract double") \
    X(vfnmaddd,   6, f64, r4rm, "vector float negate multiply-add double") \
    X(vfnmsubd,   7, f64, r4rm, "vector float negate multiply-subtract double") \
    X(vfmaddsubd, 8, f64, r4rm, "vector float multiply-add|subtract alternating double") \
    X(vfmsubaddd, 9, f64, r4rm, "vector float multiply-subtract|add alternating double") \

#define DECLARE_FMAQ_EXTENDED_OPCODES(X) \
    X(fmaddq,   0, f128,   r4rm, "float multiply-add quadruple") \
    X(fmsubq,   1, f128,   r4rm, "float multiply-subtract quadruple") \
    X(fnmaddq,  2, f128,   r4rm, "float negate multiply-add quadruple") \
    X(fnmsubq,  3, f128,   r4rm, "float negate multiply-subtract quadruple") \
    X(amocasb, 11, atomic, r4mo, "atomic compare and swap byte") \
    X(amocash, 12, atomic, r4mo, "atomic compare and swap halfword") \
    X(amocasw, 13, atomic, r4mo, "atomic compare and swap word") \
    X(amocasd, 14, atomic, r4mo, "atomic compare and swap doubleword") \
    X(amocasq, 15, atomic, r4mo, "atomic compare and swap quadword") \

/**************************************************************************************
* 'branch' extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_BRH_EXTENDED_OPCODES(X) \
    X(brbc,     0, branch, br_rr,  "branch if bit clear") \
    X(brbci,    1, branch, br_rs,  "branch if bit clear immediate") \
    X(brbs,     2, branch, br_rr,  "branch if bit set") \
    X(brbsi,    3, branch, br_rs,  "branch if bit set immediate") /* reserved - 4,5 */ \
    X(fbroeqh,  6, f16, br_rr, "float branch if ordered and equal half") \
    X(fbroneh,  7, f16, br_rr, "float branch if ordered and not-equal half") \
    X(fbrolth,  8, f16, br_rr, "float branch if ordered and less than half") \
    X(fbrogeh,  9, f16, br_rr, "float branch if ordered and greater-or-equal half") \
    X(fbroh,   10, f16, br_rr, "float branch if ordered half") \
    X(fbrueqh, 11, f16, br_rr, "float branch if unordered or equal half") \
    X(fbruneh, 12, f16, br_rr, "float branch if unordered or not-equal half") \
    X(fbrulth, 13, f16, br_rr, "float branch if unordered or less than half") \
    X(fbrugeh, 14, f16, br_rr, "float branch if unordered or greater-or-equal half") \
    X(fbruh,   15, f16, br_rr, "float branch if unordered half") \

#define DECLARE_BRW_EXTENDED_OPCODES(X) \
    X(breqw,    0, branch, br_rr, "branch if equal word") \
    X(brnew,    1, branch, br_rr, "branch if not equal word") \
    X(brltsw,   2, branch, br_rr, "branch if less than signed word") \
    X(brgesw,   3, branch, br_rr, "branch if greater or equal signed word") \
    X(brltuw,   4, branch, br_rr, "branch if less than unsigned word") \
    X(brgeuw,   5, branch, br_rr, "branch if greater or equal unsigned word") \
    X(fbroeqs,  6, f32, br_rr, "float branch if ordered and equal single") \
    X(fbrones,  7, f32, br_rr, "float branch if ordered and not-equal single") \
    X(fbrolts,  8, f32, br_rr, "float branch if ordered and less than single") \
    X(fbroges,  9, f32, br_rr, "float branch if ordered and greater-or-equal single") \
    X(fbros,   10, f32, br_rr, "float branch if ordered single") \
    X(fbrueqs, 11, f32, br_rr, "float branch if unordered or equal single") \
    X(fbrunes, 12, f32, br_rr, "float branch if unordered or not-equal single") \
    X(fbrults, 13, f32, br_rr, "float branch if unordered or less than single") \
    X(fbruges, 14, f32, br_rr, "float branch if unordered or greater-or-equal single") \
    X(fbrus,   15, f32, br_rr, "float branch if unordered single") \

#define DECLARE_BRD_EXTENDED_OPCODES(X) \
    X(breqd,    0, branch, br_rr, "branch if equal doubleword") \
    X(brned,    1, branch, br_rr, "branch if not equal doubleword") \
    X(brltsd,   2, branch, br_rr, "branch if less than signed doubleword") \
    X(brgesd,   3, branch, br_rr, "branch if greater or equal signed doubleword") \
    X(brltud,   4, branch, br_rr, "branch if less than unsigned doubleword") \
    X(brgeud,   5, branch, br_rr, "branch if greater or equal unsigned doubleword") \
    X(fbroeqd,  6, f64, br_rr, "float branch if ordered and equal double") \
    X(fbroned,  7, f64, br_rr, "float branch if ordered and not-equal double") \
    X(fbroltd,  8, f64, br_rr, "float branch if ordered and less than double") \
    X(fbroged,  9, f64, br_rr, "float branch if ordered and greater-or-equal double") \
    X(fbrod,   10, f64, br_rr, "float branch if ordered double") \
    X(fbrueqd, 11, f64, br_rr, "float branch if unordered or equal double") \
    X(fbruned, 12, f64, br_rr, "float branch if unordered or not-equal double") \
    X(fbrultd, 13, f64, br_rr, "float branch if unordered or less than double") \
    X(fbruged, 14, f64, br_rr, "float branch if unordered or greater-or-equal double") \
    X(fbrud,   15, f64, br_rr, "float branch if unordered double") \

#define DECLARE_BRQ_EXTENDED_OPCODES(X) \
    X(breqq,    0, i128, br_rr, "branch if equal quadword") \
    X(brneq,    1, i128, br_rr, "branch if not equal quadword") \
    X(brltsq,   2, i128, br_rr, "branch if less than signed quadword") \
    X(brgesq,   3, i128, br_rr, "branch if greater or equal signed quadword") \
    X(brltuq,   4, i128, br_rr, "branch if less than unsigned quadword") \
    X(brgeuq,   5, i128, br_rr, "branch if greater or equal unsigned quadword") \
    X(fbroeqq,  6, f128, br_rr, "float branch if ordered and equal quadruple") \
    X(fbroneq,  7, f128, br_rr, "float branch if ordered and not-equal quadruple") \
    X(fbroltq,  8, f128, br_rr, "float branch if ordered and less than quadruple") \
    X(fbrogeq,  9, f128, br_rr, "float branch if ordered and greater-or-equal quadruple") \
    X(fbroq,   10, f128, br_rr, "float branch if ordered quadruple") \
    X(fbrueqq, 11, f128, br_rr, "float branch if unordered or equal quadruple") \
    X(fbruneq, 12, f128, br_rr, "float branch if unordered or not-equal quadruple") \
    X(fbrultq, 13, f128, br_rr, "float branch if unordered or less than quadruple") \
    X(fbrugeq, 14, f128, br_rr, "float branch if unordered or greater-or-equal quadruple") \
    X(fbruq,   15, f128, br_rr, "float branch if unordered quadruple") \

/**************************************************************************************
* nullify extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_NULH_EXTENDED_OPCODES(X) \
    X(nulbc,     0, branch, nul_rr, "nullify if bit clear") \
    X(nulbci,    1, branch, nul_rs, "nullify if bit clear immediate") \
    X(nulbs,     2, branch, nul_rr, "nullify if bit set") \
    X(nulbsi,    3, branch, nul_rs, "nullify if bit set immediate") \
    X(fnuloeqh,  6, f16, nul_rr, "float nullify if ordered and equal half") \
    X(fnuloneh,  7, f16, nul_rr, "float nullify if ordered and not-equal half") \
    X(fnulolth,  8, f16, nul_rr, "float nullify if ordered and less than half") \
    X(fnulogeh,  9, f16, nul_rr, "float nullify if ordered and greater-equal half") \
    X(fnuloh,   10, f16, nul_rr, "float nullify if ordered half") \
    X(fnulueqh, 11, f16, nul_rr, "float nullify if unordered or equal half") \
    X(fnuluneh, 12, f16, nul_rr, "float nullify if unordered or not-equal half") \
    X(fnululth, 13, f16, nul_rr, "float nullify if unordered or less than half") \
    X(fnulugeh, 14, f16, nul_rr, "float nullify if unordered or greater-equal half") \
    X(fnuluh,   15, f16, nul_rr, "float nullify if unordered half") \

#define DECLARE_NULW_EXTENDED_OPCODES(X) \
    X(nuleqw,    0, branch, nul_rr, "nullify if equal word") \
    X(nulnew,    1, branch, nul_rr, "nullify if not-equal word") \
    X(nulltsw,   2, branch, nul_rr, "nullify if less than signed word") \
    X(nulgesw,   3, branch, nul_rr, "nullify if greater or equal signed word") \
    X(nulltuw,   4, branch, nul_rr, "nullify if less than unsigned word") \
    X(nulgeuw,   5, branch, nul_rr, "nullify if greater or equal unsigned word") \
    X(fnuloeqs,  6, f32, nul_rr, "float nullify if ordered and equal single") \
    X(fnulones,  7, f32, nul_rr, "float nullify if ordered and not-equal single") \
    X(fnulolts,  8, f32, nul_rr, "float nullify if ordered and less than single") \
    X(fnuloges,  9, f32, nul_rr, "float nullify if ordered and greater-equal single") \
    X(fnulos,   10, f32, nul_rr, "float nullify if ordered single") \
    X(fnulueqs, 11, f32, nul_rr, "float nullify if unordered or equal single") \
    X(fnulunes, 12, f32, nul_rr, "float nullify if unordered or not-equal single") \
    X(fnulults, 13, f32, nul_rr, "float nullify if unordered or less than single") \
    X(fnuluges, 14, f32, nul_rr, "float nullify if unordered or greater-equal single") \
    X(fnulus,   15, f32, nul_rr, "float nullify if unordered single") \

#define DECLARE_NULD_EXTENDED_OPCODES(X) \
    X(nuleqd,    0, branch, nul_rr, "nullify if equal doubleword") \
    X(nulned,    1, branch, nul_rr, "nullify if not-equal doubleword") \
    X(nulltsd,   2, branch, nul_rr, "nullify if less than signed doubleword") \
    X(nulgesd,   3, branch, nul_rr, "nullify if greater or equal signed doubleword") \
    X(nulltud,   4, branch, nul_rr, "nullify if less than unsigned doubleword") \
    X(nulgeud,   5, branch, nul_rr, "nullify if greater or equal unsigned doubleword") \
    X(fnuloeqd,  6, f64, nul_rr, "float nullify if ordered and equal double") \
    X(fnuloned,  7, f64, nul_rr, "float nullify if ordered and not-equal double") \
    X(fnuloltd,  8, f64, nul_rr, "float nullify if ordered and less than double") \
    X(fnuloged,  9, f64, nul_rr, "float nullify if ordered and greater-equal double") \
    X(fnulod,   10, f64, nul_rr, "float nullify if ordered double") \
    X(fnulueqd, 11, f64, nul_rr, "float nullify if unordered or equal double") \
    X(fnuluned, 12, f64, nul_rr, "float nullify if unordered or not-equal double") \
    X(fnulultd, 13, f64, nul_rr, "float nullify if unordered or less than double") \
    X(fnuluged, 14, f64, nul_rr, "float nullify if unordered or greater-equal double") \
    X(fnulud,   15, f64, nul_rr, "float nullify if unordered double") \

#define DECLARE_NULQ_EXTENDED_OPCODES(X) \
    X(nuleqq,    0, i128, nul_rr, "nullify if equal quadword") \
    X(nulneq,    1, i128, nul_rr, "nullify if not-equal quadword") \
    X(nulltsq,   2, i128, nul_rr, "nullify if less than signed quadword") \
    X(nulgesq,   3, i128, nul_rr, "nullify if greater or equal signed quadword") \
    X(nulltuq,   4, i128, nul_rr, "nullify if less than unsigned quadword") \
    X(nulgeuq,   5, i128, nul_rr, "nullify if greater or equal unsigned quadword") \
    X(fnuloeqq,  6, f128, nul_rr, "float nullify if ordered and equal quadruple") \
    X(fnuloneq,  7, f128, nul_rr, "float nullify if ordered and not-equal quadruple") \
    X(fnuloltq,  8, f128, nul_rr, "float nullify if ordered and less than quadruple") \
    X(fnulogeq,  9, f128, nul_rr, "float nullify if ordered and greater-equal quadruple") \
    X(fnuloq,   10, f128, nul_rr, "float nullify if ordered quadruple") \
    X(fnulueqq, 11, f128, nul_rr, "float nullify if unordered or equal quadruple") \
    X(fnuluneq, 12, f128, nul_rr, "float nullify if unordered or not-equal quadruple") \
    X(fnulultq, 13, f128, nul_rr, "float nullify if unordered or less than quadruple") \
    X(fnulugeq, 14, f128, nul_rr, "float nullify if unordered or greater-equal quadruple") \
    X(fnuluq,   15, f128, nul_rr, "float nullify if unordered quadruple") \

#define DECLARE_LOOP_EXTENDED_OPCODES(X) \
    X(repltd,   0, branch, loop, "repeat on less than signed doubleword") \
    X(repgtd,   1, branch, loop, "repeat on greater signed doubleword") \
    X(repled,   2, branch, loop, "repeat on less than or equal signed doubleword") \
    X(repged,   3, branch, loop, "repeat on greater or equal signed doubleword") \
    X(repltud,  4, branch, loop, "repeat on less than unsigned doubleword") \
    X(repgtud,  5, branch, loop, "repeat on greater unsigned doubleword") \
    X(repleud,  6, branch, loop, "repeat on less than or equal unsigned doubleword") \
    X(repgeud,  7, branch, loop, "repeat on greater or equal unsigned doubleword") \
    X(repltw,   8, branch, loop, "repeat on less than signed word") \
    X(repgtw,   9, branch, loop, "repeat on greater signed word") \
    X(replew,  10, branch, loop, "repeat on less than or equal signed word") \
    X(repgew,  11, branch, loop, "repeat on greater or equal signed word") \
    X(repltuw, 12, branch, loop, "repeat on less than unsigned word") \
    X(repgtuw, 13, branch, loop, "repeat on greater unsigned word") \
    X(repleuw, 14, branch, loop, "repeat on less than or equal unsigned word") \
    X(repgeuw, 15, branch, loop, "repeat on greater or equal unsigned word") \

/**************************************************************************************
* opcode_raopx extended opcodes (7 bit)
**************************************************************************************/
#define DECLARE_RAOPX_EXTENDED_OPCODES(X) \
    X(nop,       0, base,       nop,        "no operation") \
    X(jmp,       1, branch,     jmp,        "jump relative") \
    X(retf,      2, base,       retf,       "return from subroutine (rollback frame)") \
    X(alloc,     3, base,       alloc,      "allocate register frame, update eip") \
    X(allocsp,   4, base,       allocsp,    "allocate register frame, update eip,sp") \
    X(ehadj,     8, special,    jmp,        "exception handler adjust eip") \
    X(ehthrow,   9, special,    cache_simm, "exception handler throw") \
    X(ehcatch,  10, special,    br_eh,      "exception handler catch") \
    X(ehnext,   11, special,    br_eh,      "exception handler next") \
    X(dcbt,     16, special,    cache_simm, "data cache block touch") \
    X(dcbf,     17, special,    cache_simm, "data cache block flush") \
    X(dcbi,     18, privileged, cache_simm, "data cache block invalidate") \
    X(icbi,     19, special,    cache_simm, "instruction cache block invalidate") \
    X(write,   127, special,    write,      "write string formatted") \

/**************************************************************************************
* indexing extended opcodes (4 bit)
**************************************************************************************/
#define DECLARE_MEMXSD_EXTENDED_OPCODES(X) \
    X(ldubxsd,   0, memory, gmemx, "load unsigned byte indexed by signed doubleword") \
    X(ldsbxsd,   1, memory, gmemx, "load signed byte indexed by signed doubleword") \
    X(stbxsd,    2, memory, gmemx, "store byte indexed by signed doubleword") \
    X(lduhxsd,   3, memory, gmemx, "load unsigned halfword indexed by signed doubleword") \
    X(ldshxsd,   4, memory, gmemx, "load signed halfword indexed by signed doubleword") \
    X(sthxsd,    5, memory, gmemx, "store halfword indexed by signed doubleword") \
    X(lduwxsd,   6, memory, gmemx, "load unsigned word indexed by signed doubleword") \
    X(ldswxsd,   7, memory, gmemx, "load signed word indexed by signed doubleword") \
    X(stwxsd,    8, memory, gmemx, "store word indexed by signed doubleword") \
    X(ldudxsd,   9, memory, gmemx, "load unsigned doubleword indexed by signed doubleword") \
    X(ldsdxsd,  10, memory, gmemx, "load signed doubleword indexed by signed doubleword") \
    X(stdxsd,   11, memory, gmemx, "store doubleword indexed by signed doubleword") \
    X(ldqxsd,   12, memory, gmemx, "load quadword indexed by signed doubleword") \
    X(stqxsd,   13, memory, gmemx, "store quadword indexed by signed doubleword") \
    X(ldaxsd,   14, base,   gmemx, "load address indexed by signed doubleword") \

#define DECLARE_MEMXUD_EXTENDED_OPCODES(X) \
    X(ldubxud,   0, memory, gmemx, "load unsigned byte indexed by unsigned doubleword") \
    X(ldsbxud,   1, memory, gmemx, "load signed byte indexed by unsigned doubleword") \
    X(stbxud,    2, memory, gmemx, "store byte indexed by unsigned doubleword") \
    X(lduhxud,   3, memory, gmemx, "load unsigned halfword indexed by unsigned doubleword") \
    X(ldshxud,   4, memory, gmemx, "load signed halfword indexed by unsigned doubleword") \
    X(sthxud,    5, memory, gmemx, "store indexed by unsigned doubleword") \
    X(lduwxud,   6, memory, gmemx, "load unsigned word indexed by unsigned doubleword") \
    X(ldswxud,   7, memory, gmemx, "load signed word indexed by unsigned doubleword") \
    X(stwxud,    8, memory, gmemx, "store word indexed by unsigned doubleword") \
    X(ldudxud,   9, memory, gmemx, "load unsigned doubleword indexed by unsigned doubleword") \
    X(ldsdxud,  10, memory, gmemx, "load signed doubleword indexed by unsigned doubleword") \
    X(stdxud,   11, memory, gmemx, "store doubleword indexed by unsigned doubleword") \
    X(ldqxud,   12, memory, gmemx, "load quadword indexed by unsigned doubleword") \
    X(stqxud,   13, memory, gmemx, "store quadword indexed by unsigned doubleword") \
    X(ldaxud,   14, base,   gmemx, "load address indexed by unsigned doubleword") \

#define DECLARE_MEMXSW_EXTENDED_OPCODES(X) \
    X(ldubxsw,   0, memory, gmemx, "load unsigned byte indexed by signed word") \
    X(ldsbxsw,   1, memory, gmemx, "load signed byte indexed by signed word") \
    X(stbxsw,    2, memory, gmemx, "store byte indexed by signed word") \
    X(lduhxsw,   3, memory, gmemx, "load unsigned halfword indexed by signed word") \
    X(ldshxsw,   4, memory, gmemx, "load signed halfword indexed by signed word") \
    X(sthxsw,    5, memory, gmemx, "store halfword indexed by signed word") \
    X(lduwxsw,   6, memory, gmemx, "load unsigned word indexed by signed word") \
    X(ldswxsw,   7, memory, gmemx, "load signed word indexed by signed word") \
    X(stwxsw,    8, memory, gmemx, "store word indexed by signed word") \
    X(ldudxsw,   9, memory, gmemx, "load unsigned doubleword indexed by signed word") \
    X(ldsdxsw,  10, memory, gmemx, "load signed doubleword indexed by signed word") \
    X(stdxsw,   11, memory, gmemx, "store doubleword indexed by signed word") \
    X(ldqxsw,   12, memory, gmemx, "load quadword indexed by signed word") \
    X(stqxsw,   13, memory, gmemx, "store quadword indexed by signed word") \
    X(ldaxsw,   14, base,   gmemx, "load address indexed by signed word") \

#define DECLARE_MEMXUW_EXTENDED_OPCODES(X) \
    X(ldubxuw,   0, memory, gmemx, "load unsigned byte indexed by unsigned word") \
    X(ldsbxuw,   1, memory, gmemx, "load signed byte indexed by unsigned word") \
    X(stbxuw,    2, memory, gmemx, "store indexed by unsigned word") \
    X(lduhxuw,   3, memory, gmemx, "load unsigned halfword indexed by unsigned word") \
    X(ldshxuw,   4, memory, gmemx, "load signed halfword indexed by unsigned word") \
    X(sthxuw,    5, memory, gmemx, "store halfword indexed by unsigned word") \
    X(lduwxuw,   6, memory, gmemx, "load unsigned word indexed by unsigned word") \
    X(ldswxuw,   7, memory, gmemx, "load signed word indexed by unsigned word") \
    X(stwxuw,    8, memory, gmemx, "store word indexed by unsigned word") \
    X(ldudxuw,   9, memory, gmemx, "load unsigned doubleword indexed by unsigned word") \
    X(ldsdxuw,  10, memory, gmemx, "load signed doubleword indexed by unsigned word") \
    X(stdxuw,   11, memory, gmemx, "store doubleword indexed by unsigned word") \
    X(ldqxuw,   12, memory, gmemx, "load quadword indexed by unsigned word") \
    X(stqxuw,   13, memory, gmemx, "store quadword indexed by unsigned word") \
    X(ldaxuw,   14, base,   gmemx, "load address indexed by unsigned word") \

/**************************************************************************************
* 'misc' extended opcodes (11 bit)
**************************************************************************************/
enum {
     INT_TYPED_BASE  = 128,
     INT_TYPED_SIZE  = 64 + 16,
     INT_BASE_32     = INT_TYPED_BASE + 0 * INT_TYPED_SIZE,
     INT_BASE_64     = INT_TYPED_BASE + 1 * INT_TYPED_SIZE,
     INT_BASE_128    = INT_TYPED_BASE + 2 * INT_TYPED_SIZE,
     SPECIAL_BASE    = 512,
     ATOMICS_BASE    = 512 + 256,
     ATOMICS_SIZE    = 40,
     ATOMICS_BASE_B  = ATOMICS_BASE + 0 * ATOMICS_SIZE,
     ATOMICS_BASE_H  = ATOMICS_BASE + 1 * ATOMICS_SIZE,
     ATOMICS_BASE_W  = ATOMICS_BASE + 2 * ATOMICS_SIZE,
     ATOMICS_BASE_D  = ATOMICS_BASE + 3 * ATOMICS_SIZE,
     ATOMICS_BASE_Q  = ATOMICS_BASE + 4 * ATOMICS_SIZE,
     FPU_BASE        = 1024 + 128 + 64,
     FPU_SIZE        = 128 + 32,
     FPH_BASE        = FPU_BASE + 0 * FPU_SIZE,
     FPS_BASE        = FPU_BASE + 1 * FPU_SIZE,
     FPD_BASE        = FPU_BASE + 2 * FPU_SIZE,
     FPQ_BASE        = FPU_BASE + 3 * FPU_SIZE,
     FPU_SCALAR_SIZE = 64 + 16,
     VFP16_BASE      = FPH_BASE + FPU_SCALAR_SIZE,
     VFP32_BASE      = FPS_BASE + FPU_SCALAR_SIZE,
     VFP64_BASE      = FPD_BASE + FPU_SCALAR_SIZE,
     MMX_BASE        = 1024 + 512 + 256,
     MMX_SIZE        = 64,
     MMX_BASE_B      = MMX_BASE + 0 * MMX_SIZE,
     MMX_BASE_H      = MMX_BASE + 1 * MMX_SIZE,
     MMX_BASE_W      = MMX_BASE + 2 * MMX_SIZE,
     MMX_BASE_D      = MMX_BASE + 3 * MMX_SIZE,
};

#define DECLARE_MISC_EXTENDED_OPCODES(X) \
    X(undef,       0, special,  noargs,   "undefined instruction") \
    X(mov,         1, base,     r2,       "move general register") \
    X(ret,         2, branch,   noargs,   "return from subroutine") \
    X(not,         3, base,     r2,       "bitwise not") \
    X(and,         4, base,     r3,       "bitwise and") \
    X(or,          5, base,     r3,       "bitwise or") \
    X(xor,         6, base,     r3,       "bitwise exclusive or") \
    X(andn,        7, base,     r3,       "bitwise and-not") \
    X(nand,        8, base,     r3,       "bitwise not-and") \
    X(nor,         9, base,     r3,       "bitwise not-or") \
    X(xnor,       10, base,     r3,       "bitwise exclusive not-or") \
    X(orn,        11, base,     r3,       "bitwise or-not") \
    X(jmpr,       12, branch,   RbcScale, "jump register indirect") \
    X(jmpt,       13, branch,   Rbc,      "jump table") \
    X(jmptsw,     14, branch,   Rbc,      "jump table signed word") \
    X(jmptuw,     15, branch,   Rbc,      "jump table unsigned word") \
    X(callri,     16, branch,   r3,       "call register indirect") \
    X(mbgath,     20, special,  r3,       "masked bit gather") \
    X(mbscat,     21, special,  r3,       "masked bit scatter") \
    X(gtb,        22, special,  r2,       "graycode to binary") \
    X(cntpop,     24, special,  r2s1,     "count population") \
    X(cntlz,      25, special,  r2s1,     "count leading zeros") \
    X(cnttz,      26, special,  r2s1,     "count trailing zeros") \
    X(permb,      27, special,  r2s1,     "permute bits") \
    X(bitclr,     28, base,     r3,       "bit clear") \
    X(bitclri,    29, base,     r2s1,     "bit clear immediate") \
    X(bitset,     30, base,     r3,       "bit set") \
    X(bitseti,    31, base,     r2s1,     "bit set immediate") \
    X(bitflip,    32, base,     r3,       "bit flip") \
    X(bitflipi,   33, base,     r2s1,     "bit flip immediate") \
    X(sextb,      36, base,     r2,       "sign extend byte") \
    X(sexth,      37, base,     r2,       "sign extend halfword") \
    X(sextw,      38, base,     r2,       "sign extend word") \
    X(sextd,      39, base,     r2,       "sign extend doubleword") \
    X(zextb,      40, base,     r2,       "zero extend byte") \
    X(zexth,      41, base,     r2,       "zero extend halfword") \
    X(zextw,      42, base,     r2,       "zero extend word") \
    X(zextd,      43, base,     r2,       "zero extend doubleword") \
    /* mem base pre-post update */ \
    X(ldubmia,    96, memory,   gmemu,    "load unsigned byte and modify immediate after") \
    X(ldsbmia,    97, memory,   gmemu,    "load signed byte and modify immediate after") \
    X(stbmia,     98, memory,   gmemu,    "store byte and modify immediate after") \
    X(lduhmia,    99, memory,   gmemu,    "load unsigned halfword and modify immediate after") \
    X(ldshmia,   100, memory,   gmemu,    "load signed halfword and modify immediate after") \
    X(sthmia,    101, memory,   gmemu,    "store halfword and modify immediate after") \
    X(lduwmia,   102, memory,   gmemu,    "load unsigned word and modify immediate after") \
    X(ldswmia,   103, memory,   gmemu,    "load signed word and modify immediate after") \
    X(stwmia,    104, memory,   gmemu,    "store word and modify immediate after") \
    X(ldudmia,   105, memory,   gmemu,    "load unsigned doubleword and modify immediate after") \
    X(ldsdmia,   106, memory,   gmemu,    "load signed doubleword and modify immediate after") \
    X(stdmia,    107, memory,   gmemu,    "store doubleword and modify immediate after") \
    X(ldqmia,    108, memory,   gmemu,    "load quadword and modify immediate after") \
    X(stqmia,    109, memory,   gmemu,    "store quadword and modify immediate after") /* reserved 110,111 */ \
    X(ldubmib,   112, memory,   gmemu,    "load unsigned byte and modify immediate before") \
    X(ldsbmib,   113, memory,   gmemu,    "load signed byte and modify immediate before") \
    X(stbmib,    114, memory,   gmemu,    "store byte and modify immediate before") \
    X(lduhmib,   115, memory,   gmemu,    "load unsigned halfword and modify immediate before") \
    X(ldshmib,   116, memory,   gmemu,    "load signed halfword and modify immediate before") \
    X(sthmib,    117, memory,   gmemu,    "store halfword and modify immediate before") \
    X(lduwmib,   118, memory,   gmemu,    "load unsigned word and modify immediate before") \
    X(ldswmib,   119, memory,   gmemu,    "load signed word and modify immediate before") \
    X(stwmib,    120, memory,   gmemu,    "store word and modify immediate before") \
    X(ldudmib,   121, memory,   gmemu,    "load unsigned doubleword and modify immediate before") \
    X(ldsdmib,   122, memory,   gmemu,    "load signed doubleword and modify immediate before") \
    X(stdmib,    123, memory,   gmemu,    "store doubleword and modify immediate before") \
    X(ldqmib,    124, memory,   gmemu,    "load quadword and modify immediate before") \
    X(stqmib,    125, memory,   gmemu,    "store quadword and modify immediate before") \
    /* int-32 */ \
    X(addws,      INT_BASE_32 +  0, base, r3,   "add word and sign-extend") \
    X(subws,      INT_BASE_32 +  1, base, r3,   "subtract word and sign-extend") \
    X(mulws,      INT_BASE_32 +  2, base, r3,   "multiply word and sign-extend") \
    X(negw,       INT_BASE_32 +  3, base, r2,   "negate word") \
    X(slliw,      INT_BASE_32 +  4, base, r2s1, "shift left logical immediate word") \
    X(srliw,      INT_BASE_32 +  5, base, r2s1, "shift right logical immediate word") \
    X(sraiw,      INT_BASE_32 +  6, base, r2s1, "shift right algebraic immediate word") \
    X(divp2iw,    INT_BASE_32 +  7, base, r2s1, "divide power-2 immediate word") \
    X(sllw,       INT_BASE_32 +  8, base, r3,   "shift left logical word") \
    X(srlw,       INT_BASE_32 +  9, base, r3,   "shift right logical word") \
    X(sraw,       INT_BASE_32 + 10, base, r3,   "shift right algebraic word") \
    X(divp2w,     INT_BASE_32 + 11, base, r3,   "divide power-2 word") \
    X(cmpeqw,     INT_BASE_32 + 12, base, r3,   "compare equal word") \
    X(cmpnew,     INT_BASE_32 + 13, base, r3,   "compare not equal word") \
    X(cmpltsw,    INT_BASE_32 + 14, base, r3,   "compare less than signed word") \
    X(cmpgesw,    INT_BASE_32 + 15, base, r3,   "compare greater-or-equal signed word") \
    X(cmpltuw,    INT_BASE_32 + 16, base, r3,   "compare less than unsigned word") \
    X(cmpgeuw,    INT_BASE_32 + 17, base, r3,   "compare greater-or-equal unsigned word") \
    X(abdw,       INT_BASE_32 + 18, base, r3,   "absolute difference word") \
    X(absw,       INT_BASE_32 + 19, base, r2,   "absolute value word") \
    X(divsw,      INT_BASE_32 + 20, base, r3,   "divide signed word") \
    X(divuw,      INT_BASE_32 + 21, base, r3,   "divide unsigned word") \
    X(remsw,      INT_BASE_32 + 22, base, r3,   "remainder signed word") \
    X(remuw,      INT_BASE_32 + 23, base, r3,   "remainder unsigned word") \
    X(maxsw,      INT_BASE_32 + 24, base, r3,   "maximum signed word") \
    X(maxuw,      INT_BASE_32 + 25, base, r3,   "maximum unsigned word") \
    X(minsw,      INT_BASE_32 + 26, base, r3,   "minimum signed word") \
    X(minuw,      INT_BASE_32 + 27, base, r3,   "minimum unsigned word") /* reserved 28-31 */ \
    X(mulwz,      INT_BASE_32 + 32, base, r3,   "multiply word and zero-extend") \
    X(addwz,      INT_BASE_32 + 33, base, r3,   "add word and zero-extend") \
    X(subwz,      INT_BASE_32 + 34, base, r3,   "subtract word and zero-extend") \
    /* int-64 */ \
    X(addd,       INT_BASE_64 +  0, base, r3,   "add doubleword") \
    X(subd,       INT_BASE_64 +  1, base, r3,   "subtract doubleword") \
    X(muld,       INT_BASE_64 +  2, base, r3,   "multiply doubleword") \
    X(negd,       INT_BASE_64 +  3, base, r2,   "negate doubleword") \
    X(sllid,      INT_BASE_64 +  4, base, r2s1, "shift left logical immediate doubleword") \
    X(srlid,      INT_BASE_64 +  5, base, r2s1, "shift right logical immediate doubleword") \
    X(sraid,      INT_BASE_64 +  6, base, r2s1, "shift right algebraic immediate doubleword") \
    X(divp2id,    INT_BASE_64 +  7, base, r2s1, "divide power-2 immediate doubleword") \
    X(slld,       INT_BASE_64 +  8, base, r3,   "shift left logical doubleword") \
    X(srld,       INT_BASE_64 +  9, base, r3,   "shift right logical doubleword") \
    X(srad,       INT_BASE_64 + 10, base, r3,   "shift right algebraic doubleword") \
    X(divp2d,     INT_BASE_64 + 11, base, r3,   "divide power-2 doubleword") \
    X(cmpeqd,     INT_BASE_64 + 12, base, r3,   "compare equal doubleword") \
    X(cmpned,     INT_BASE_64 + 13, base, r3,   "compare not equal doubleword") \
    X(cmpltsd,    INT_BASE_64 + 14, base, r3,   "compare less than signed doubleword") \
    X(cmpgesd,    INT_BASE_64 + 15, base, r3,   "compare greater-or-equal signed doubleword") \
    X(cmpltud,    INT_BASE_64 + 16, base, r3,   "compare less than unsigned doubleword") \
    X(cmpgeud,    INT_BASE_64 + 17, base, r3,   "compare greater-or-equal unsigned doubleword") \
    X(abdd,       INT_BASE_64 + 18, base, r3,   "absolute difference doubleword") \
    X(absd,       INT_BASE_64 + 19, base, r2,   "absolute value doubleword") \
    X(divsd,      INT_BASE_64 + 20, base, r3,   "divide signed doubleword") \
    X(divud,      INT_BASE_64 + 21, base, r3,   "divide unsigned doubleword") \
    X(remsd,      INT_BASE_64 + 22, base, r3,   "remainder signed doubleword") \
    X(remud,      INT_BASE_64 + 23, base, r3,   "remainder unsigned doubleword") \
    X(maxsd,      INT_BASE_64 + 24, base, r3,   "maximum signed doubleword") \
    X(maxud,      INT_BASE_64 + 25, base, r3,   "maximum unsigned doubleword") \
    X(minsd,      INT_BASE_64 + 26, base, r3,   "minimum signed doubleword") \
    X(minud,      INT_BASE_64 + 27, base, r3,   "minimum unsigned doubleword") /* reserved 28-31 */ \
    X(xordec,     INT_BASE_64 + 32, base, r3,   "bitwise exclusive-or decremented") \
    X(anddec,     INT_BASE_64 + 33, base, r3,   "bitwise and decremented") \
    X(andneg,     INT_BASE_64 + 34, base, r3,   "bitwise and negate") \
    X(addod,      INT_BASE_64 + 35, base, r3,   "add overflow doubleword") \
    X(subod,      INT_BASE_64 + 36, base, r3,   "subtract overflow doubleword") \
    X(addcd,      INT_BASE_64 + 37, base, r3,   "add carry-out doubleword") \
    X(subcd,      INT_BASE_64 + 38, base, r3,   "subtract with carry doubleword") \
    X(mulh,       INT_BASE_64 + 39, base, r3,   "multiply high") \
    /* int-128 */ \
    X(addq,       INT_BASE_128 + 0,  i128, r3,   "add quadword") \
    X(subq,       INT_BASE_128 + 1,  i128, r3,   "subtract quadword") \
    X(mulq,       INT_BASE_128 + 2,  i128, r3,   "multiply quadword") \
    X(negq,       INT_BASE_128 + 3,  i128, r2,   "negate quadword") \
    X(slliq,      INT_BASE_128 + 4,  i128, r2s1, "shift left logical immediate quadword") \
    X(srliq,      INT_BASE_128 + 5,  i128, r2s1, "shift right logical immediate quadword") \
    X(sraiq,      INT_BASE_128 + 6,  i128, r2s1, "shift right algebraic immediate quadword") \
    X(divp2iq,    INT_BASE_128 + 7,  i128, r2s1, "divide power-2 immediate quadword") \
    X(sllq,       INT_BASE_128 + 8,  i128, r3,   "shift left logical quadword") \
    X(srlq,       INT_BASE_128 + 9,  i128, r3,   "shift right logical quadword") \
    X(sraq,       INT_BASE_128 + 10, i128, r3,   "shift right algebraic quadword") \
    X(divp2q,     INT_BASE_128 + 11, i128, r3,   "divide power-2 quadword") \
    X(cmpeqq,     INT_BASE_128 + 12, i128, r3,   "compare equal quadword") \
    X(cmpneq,     INT_BASE_128 + 13, i128, r3,   "compare not equal quadword") \
    X(cmpltsq,    INT_BASE_128 + 14, i128, r3,   "compare less than signed quadword") \
    X(cmpgesq,    INT_BASE_128 + 15, i128, r3,   "compare greater-or-equal signed quadword") \
    X(cmpltuq,    INT_BASE_128 + 16, i128, r3,   "compare less than unsigned quadword") \
    X(cmpgeuq,    INT_BASE_128 + 17, i128, r3,   "compare greater-or-equal unsigned quadword") \
    X(abdq,       INT_BASE_128 + 18, i128, r3,   "absolute difference quadword") \
    X(absq,       INT_BASE_128 + 19, i128, r2,   "absolute value quadword") \
    X(divsq,      INT_BASE_128 + 20, i128, r3,   "divide signed quadword") \
    X(divuq,      INT_BASE_128 + 21, i128, r3,   "divide unsigned quadword") \
    X(remsq,      INT_BASE_128 + 22, i128, r3,   "remainder signed quadword") \
    X(remuq,      INT_BASE_128 + 23, i128, r3,   "remainder unsigned quadword") \
    X(maxsq,      INT_BASE_128 + 24, i128, r3,   "maximum signed quadword") \
    X(maxuq,      INT_BASE_128 + 25, i128, r3,   "maximum unsigned quadword") \
    X(minsq,      INT_BASE_128 + 26, i128, r3,   "minimum signed quadword") \
    X(minuq,      INT_BASE_128 + 27, i128, r3,   "minimum unsigned quadword") \
    /* special */ \
    X(mprobe,          SPECIAL_BASE +  0, special,    r3,       "memory probe access") \
    X(cpuid,           SPECIAL_BASE +  1, special,    RabS10,   "cpu identification") \
    X(int,             SPECIAL_BASE +  2, special,    int,      "interrupt") \
    X(syscall,         SPECIAL_BASE +  3, special,    noargs,   "system call") \
    X(random,          SPECIAL_BASE +  4, special,    r2,       "random") \
    X(getspr,          SPECIAL_BASE +  5, special,    mspr,     "get special-purpose register") \
    X(setspr,          SPECIAL_BASE +  6, special,    mspr,     "set special-purpose register") \
    X(setdbr,          SPECIAL_BASE +  7, privileged, RabS10,   "set data breakpoint register") \
    X(getdbr,          SPECIAL_BASE +  8, privileged, RabS10,   "get data breakpoint register") \
    X(setibr,          SPECIAL_BASE +  9, privileged, RabS10,   "set instruction breakpoint register") \
    X(getibr,          SPECIAL_BASE + 10, privileged, RabS10,   "get instruction breakpoint register") \
    X(setmr,           SPECIAL_BASE + 11, privileged, RabS10,   "set monitor register") \
    X(getmr,           SPECIAL_BASE + 12, privileged, RabS10,   "get monitor register") \
    X(setitr,          SPECIAL_BASE + 13, privileged, r3,       "set instruction translation register") \
    X(setdtr,          SPECIAL_BASE + 14, privileged, r3,       "set data translation register") \
    X(rfi,             SPECIAL_BASE + 15, privileged, noargs,   "return from interruption") \
    X(halt,            SPECIAL_BASE + 16, privileged, noargs,   "halt processor") \
    X(tpa,             SPECIAL_BASE + 17, privileged, r3,       "translate to physical address") \
    X(ptc,             SPECIAL_BASE + 18, privileged, r3,       "purge translation cache") \
    X(sysret,          SPECIAL_BASE + 22, privileged, noargs,   "system return") \
    X(rscover,         SPECIAL_BASE + 24, privileged, noargs,   "register stack cover") \
    X(rsflush,         SPECIAL_BASE + 25, privileged, noargs,   "register stack flush") \
    X(rsload,          SPECIAL_BASE + 26, privileged, noargs,   "register stack load") \
    X(clmul,           SPECIAL_BASE + 32, cipher,     r3scale,  "carry-less multiply") \
    X(crc32cb,         SPECIAL_BASE + 33, cipher,     r3,       "crc32c byte") \
    X(crc32ch,         SPECIAL_BASE + 34, cipher,     r3,       "crc32c halfword") \
    X(crc32cw,         SPECIAL_BASE + 35, cipher,     r3,       "crc32c word") \
    X(crc32cd,         SPECIAL_BASE + 36, cipher,     r3,       "crc32c doubleword") \
    X(crc32cq,         SPECIAL_BASE + 37, cipher,     r3,       "crc32c quadword") \
    X(aesenc,          SPECIAL_BASE + 40, cipher,     r3,       "aes encrypt round") \
    X(aesenclast,      SPECIAL_BASE + 41, cipher,     r3,       "aes encrypt last round") \
    X(aesdec,          SPECIAL_BASE + 42, cipher,     r3,       "aes decrypt round") \
    X(aesdeclast,      SPECIAL_BASE + 43, cipher,     r3,       "aes decrypt last round") \
    X(aesimc,          SPECIAL_BASE + 44, cipher,     r2,       "aes inverse mix columns") \
    X(aeskeygenassist, SPECIAL_BASE + 45, cipher,     RabS10,   "aes key generation assist") \
    X(jmpmi,           SPECIAL_BASE + 51, special,    RbcScale, "jmpmi") /*workaround*/ \
    X(fence,           SPECIAL_BASE + 52, atomic,     fence,    "atomic fence") \
    /* atomic */ \
    X(amoldb,     ATOMICS_BASE_B +  0, atomic, r2mo, "atomic load byte") \
    X(amostb,     ATOMICS_BASE_B +  1, atomic, r2mo, "atomic store byte") \
    X(amoswapb,   ATOMICS_BASE_B +  2, atomic, r3mo, "atomic swap byte") \
    X(amoaddb,    ATOMICS_BASE_B +  3, atomic, r3mo, "atomic load-add byte") \
    X(amoandb,    ATOMICS_BASE_B +  4, atomic, r3mo, "atomic load-and byte") \
    X(amoorb,     ATOMICS_BASE_B +  5, atomic, r3mo, "atomic load-or byte") \
    X(amoxorb,    ATOMICS_BASE_B +  6, atomic, r3mo, "atomic load-xor byte") \
    X(amominsb,   ATOMICS_BASE_B +  7, atomic, r3mo, "atomic signed minimum byte") \
    X(amomaxsb,   ATOMICS_BASE_B +  8, atomic, r3mo, "atomic signed maximum byte") \
    X(amominub,   ATOMICS_BASE_B +  9, atomic, r3mo, "atomic unsigned minimum byte") \
    X(amomaxub,   ATOMICS_BASE_B + 10, atomic, r3mo, "atomic unsigned maximum byte") \
    X(amosubb,    ATOMICS_BASE_B + 11, atomic, r3mo, "atomic load-subtract byte") \
    X(amoldh,     ATOMICS_BASE_H +  0, atomic, r2mo, "atomic load halfword") \
    X(amosth,     ATOMICS_BASE_H +  1, atomic, r2mo, "atomic store halfword") \
    X(amoswaph,   ATOMICS_BASE_H +  2, atomic, r3mo, "atomic swap halfword") \
    X(amoaddh,    ATOMICS_BASE_H +  3, atomic, r3mo, "atomic load-add halfword") \
    X(amoandh,    ATOMICS_BASE_H +  4, atomic, r3mo, "atomic load-and halfword") \
    X(amoorh,     ATOMICS_BASE_H +  5, atomic, r3mo, "atomic load-or halfword") \
    X(amoxorh,    ATOMICS_BASE_H +  6, atomic, r3mo, "atomic load-xor halfword") \
    X(amominsh,   ATOMICS_BASE_H +  7, atomic, r3mo, "atomic signed minimum halfword") \
    X(amomaxsh,   ATOMICS_BASE_H +  8, atomic, r3mo, "atomic signed maximum halfword") \
    X(amominuh,   ATOMICS_BASE_H +  9, atomic, r3mo, "atomic unsigned minimum halfword") \
    X(amomaxuh,   ATOMICS_BASE_H + 10, atomic, r3mo, "atomic unsigned maximum halfword") \
    X(amosubh,    ATOMICS_BASE_H + 11, atomic, r3mo, "atomic load-subtract halfword") \
    X(amoldw,     ATOMICS_BASE_W +  0, atomic, r2mo, "atomic load word") \
    X(amostw,     ATOMICS_BASE_W +  1, atomic, r2mo, "atomic store word") \
    X(amoswapw,   ATOMICS_BASE_W +  2, atomic, r3mo, "atomic swap word") \
    X(amoaddw,    ATOMICS_BASE_W +  3, atomic, r3mo, "atomic load-add word") \
    X(amoandw,    ATOMICS_BASE_W +  4, atomic, r3mo, "atomic load-and word") \
    X(amoorw,     ATOMICS_BASE_W +  5, atomic, r3mo, "atomic load-or word") \
    X(amoxorw,    ATOMICS_BASE_W +  6, atomic, r3mo, "atomic load-xor word") \
    X(amominsw,   ATOMICS_BASE_W +  7, atomic, r3mo, "atomic signed minimum word") \
    X(amomaxsw,   ATOMICS_BASE_W +  8, atomic, r3mo, "atomic signed maximum word") \
    X(amominuw,   ATOMICS_BASE_W +  9, atomic, r3mo, "atomic unsigned minimum word") \
    X(amomaxuw,   ATOMICS_BASE_W + 10, atomic, r3mo, "atomic unsigned maximum word") \
    X(amosubw,    ATOMICS_BASE_W + 11, atomic, r3mo, "atomic load-subtract word") \
    X(amoldd,     ATOMICS_BASE_D +  0, atomic, r2mo, "atomic load doubleword") \
    X(amostd,     ATOMICS_BASE_D +  1, atomic, r2mo, "atomic store doubleword") \
    X(amoswapd,   ATOMICS_BASE_D +  2, atomic, r3mo, "atomic swap doubleword") \
    X(amoaddd,    ATOMICS_BASE_D +  3, atomic, r3mo, "atomic load-add doubleword") \
    X(amoandd,    ATOMICS_BASE_D +  4, atomic, r3mo, "atomic load-and doubleword") \
    X(amoord,     ATOMICS_BASE_D +  5, atomic, r3mo, "atomic load-or doubleword") \
    X(amoxord,    ATOMICS_BASE_D +  6, atomic, r3mo, "atomic load-xor doubleword") \
    X(amominsd,   ATOMICS_BASE_D +  7, atomic, r3mo, "atomic signed minimum doubleword") \
    X(amomaxsd,   ATOMICS_BASE_D +  8, atomic, r3mo, "atomic signed maximum doubleword") \
    X(amominud,   ATOMICS_BASE_D +  9, atomic, r3mo, "atomic unsigned minimum doubleword") \
    X(amomaxud,   ATOMICS_BASE_D + 10, atomic, r3mo, "atomic unsigned maximum doubleword") \
    X(amosubd,    ATOMICS_BASE_D + 11, atomic, r3mo, "atomic load-subtract doubleword") \
    X(amoldq,     ATOMICS_BASE_Q +  0, atomic, r2mo, "atomic load qword") \
    X(amostq,     ATOMICS_BASE_Q +  1, atomic, r2mo, "atomic store qword") \
    X(amoswapq,   ATOMICS_BASE_Q +  2, atomic, r3mo, "atomic swap qword") \
    X(amoaddq,    ATOMICS_BASE_Q +  3, atomic, r3mo, "atomic load-add qword") \
    X(amoandq,    ATOMICS_BASE_Q +  4, atomic, r3mo, "atomic load-and qword") \
    X(amoorq,     ATOMICS_BASE_Q +  5, atomic, r3mo, "atomic load-or qword") \
    X(amoxorq,    ATOMICS_BASE_Q +  6, atomic, r3mo, "atomic load-xor qword") \
    X(amominsq,   ATOMICS_BASE_Q +  7, atomic, r3mo, "atomic signed minimum qword") \
    X(amomaxsq,   ATOMICS_BASE_Q +  8, atomic, r3mo, "atomic signed maximum qword") \
    X(amominuq,   ATOMICS_BASE_Q +  9, atomic, r3mo, "atomic unsigned minimum qword") \
    X(amomaxuq,   ATOMICS_BASE_Q + 10, atomic, r3mo, "atomic unsigned maximum qword") \
    X(amosubq,    ATOMICS_BASE_Q + 11, atomic, r3mo, "atomic load-subtract qword") \
    /* f16 */ \
    X(fclassh,    FPH_BASE +  0, f16, fpclass, "float classify half") \
    X(fsqrth,     FPH_BASE +  1, f16, r2rm,    "float square root half") \
    X(frsqrth,    FPH_BASE +  2, f16, r2rm,    "float reciprocal square root half") \
    X(faddh,      FPH_BASE +  3, f16, r3rm,    "float add half") \
    X(fsubh,      FPH_BASE +  4, f16, r3rm,    "float subtract half") \
    X(fnaddh,     FPH_BASE +  5, f16, r3rm,    "float negate add half") \
    X(fmulh,      FPH_BASE +  6, f16, r3rm,    "float multiply half") \
    X(fnmulh,     FPH_BASE +  7, f16, r3rm,    "float negate multiply half") \
    X(fdivh,      FPH_BASE +  8, f16, r3rm,    "float divide half") \
    X(fnegh,      FPH_BASE +  9, f16, r2,      "float negate half") \
    X(fabsh,      FPH_BASE + 10, f16, r2,      "float absolute value half") \
    X(fnabsh,     FPH_BASE + 11, f16, r2,      "float negate absolute value half") \
    X(fabdh,      FPH_BASE + 12, f16, r3rm,    "float absolute difference half") \
    X(fnabdh,     FPH_BASE + 13, f16, r3rm,    "float negate absolute difference half") \
    X(fminh,      FPH_BASE + 14, f16, r3,      "float minimum half") \
    X(fmaxh,      FPH_BASE + 15, f16, r3,      "float maximum half") \
    X(fminnmh,    FPH_BASE + 16, f16, r3,      "float minimum number half") \
    X(fmaxnmh,    FPH_BASE + 17, f16, r3,      "float maximum number half") \
    X(faminnmh,   FPH_BASE + 18, f16, r3,      "float absolute minimum number half") \
    X(famaxnmh,   FPH_BASE + 19, f16, r3,      "float absolute maximum number half") \
    X(frndh,      FPH_BASE + 20, f16, r2rm,    "float round half") \
    X(frndxh,     FPH_BASE + 21, f16, r2rm,    "float round and detect inexact half") /* reserved 22,23 */ \
    X(fcmpoeqh,   FPH_BASE + 24, f16, r3,      "float compare ordered and equal half") \
    X(fcmponeh,   FPH_BASE + 25, f16, r3,      "float compare ordered and not-equal half") \
    X(fcmpolth,   FPH_BASE + 26, f16, r3,      "float compare ordered and less than half") \
    X(fcmpogeh,   FPH_BASE + 27, f16, r3,      "float compare ordered and greater-or-equal half") \
    X(fcmpoh,     FPH_BASE + 28, f16, r3,      "float compare ordered half") \
    X(fcmpueqh,   FPH_BASE + 29, f16, r3,      "float compare unordered or equal half") \
    X(fcmpuneh,   FPH_BASE + 30, f16, r3,      "float compare unordered or not-equal half") \
    X(fcmpulth,   FPH_BASE + 31, f16, r3,      "float compare unordered or less than half") \
    X(fcmpugeh,   FPH_BASE + 32, f16, r3,      "float compare unordered or greater-or-equal half") \
    X(fcmpuh,     FPH_BASE + 33, f16, r3,      "float compare unordered half") /* reserved 34-39 */ \
    X(fhtoiw,     FPH_BASE + 40, f16, r2rm,    "float convert half to signed word") \
    X(fhtouw,     FPH_BASE + 41, f16, r2rm,    "float convert half to unsigned word") \
    X(fiwtoh,     FPH_BASE + 42, f16, r2rm,    "float convert signed word to half") \
    X(fuwtoh,     FPH_BASE + 43, f16, r2rm,    "float convert unsigned word to half") \
    X(fhtoid,     FPH_BASE + 44, f16, r2rm,    "float convert half to signed doubleword") \
    X(fhtoud,     FPH_BASE + 45, f16, r2rm,    "float convert half to unsigned doubleword") \
    X(fidtoh,     FPH_BASE + 46, f16, r2rm,    "float convert signed doubleword to half") \
    X(fudtoh,     FPH_BASE + 47, f16, r2rm,    "float convert unsigned doubleword to half") \
    X(fhtoiq,     FPH_BASE + 48, f16, r2rm,    "float convert half to signed quadword") \
    X(fhtouq,     FPH_BASE + 49, f16, r2rm,    "float convert half to unsigned quadword") \
    X(fiqtoh,     FPH_BASE + 50, f16, r2rm,    "float convert signed quadword to half") \
    X(fuqtoh,     FPH_BASE + 51, f16, r2rm,    "float convert unsigned quadword to half") \
    X(fhtos,      FPH_BASE + 56, f16, r2,      "float extend half to single") \
    X(fhtod,      FPH_BASE + 57, f16, r2,      "float extend half to double") \
    X(fstoh,      FPH_BASE + 58, f16, r2rm,    "float convert single to half") \
    X(fdtoh,      FPH_BASE + 59, f16, r2rm,    "float convert double to half") \
    /* vf16 */ \
    X(vfnegh,     VFP16_BASE +  0, f16, r2,     "vector float negate half") \
    X(vfabsh,     VFP16_BASE +  1, f16, r2,     "vector float absolute value half") \
    X(vfnabsh,    VFP16_BASE +  2, f16, r2,     "vector float negate absolute value half") \
    X(vfabdh,     VFP16_BASE +  3, f16, r3rm,   "vector float absolute difference half") \
    X(vfnabdh,    VFP16_BASE +  4, f16, r3rm,   "vector float negate absolute difference half") \
    X(vfrsqrth,   VFP16_BASE +  5, f16, r2rm,   "vector float reciprocal square root half") \
    X(vfsqrth,    VFP16_BASE +  6, f16, r2rm,   "vector float square root half") \
    X(vfaddh,     VFP16_BASE +  7, f16, r3rm,   "vector float add half") \
    X(vfsubh,     VFP16_BASE +  8, f16, r3rm,   "vector float subtract half") \
    X(vfnaddh,    VFP16_BASE +  9, f16, r3rm,   "vector float negate add half") \
    X(vfmulh,     VFP16_BASE + 10, f16, r3rm,   "vector float multiply half") \
    X(vfnmulh,    VFP16_BASE + 11, f16, r3rm,   "vector float negate multiply half") \
    X(vfdivh,     VFP16_BASE + 12, f16, r3rm,   "vector float divide half") \
    X(vfminh,     VFP16_BASE + 13, f16, r3,     "vector float minimum half") \
    X(vfmaxh,     VFP16_BASE + 14, f16, r3,     "vector float maximum half") \
    X(vfminnmh,   VFP16_BASE + 15, f16, r3,     "vector float minimum number half") \
    X(vfmaxnmh,   VFP16_BASE + 16, f16, r3,     "vector float maximum number half") \
    X(vfaminnmh,  VFP16_BASE + 17, f16, r3,     "vector float absolute minimum number half") \
    X(vfamaxnmh,  VFP16_BASE + 18, f16, r3,     "vector float absolute maximum number half") \
    X(vfrndh,     VFP16_BASE + 19, f16, r2rm,   "vector float round half") \
    X(vfrndxh,    VFP16_BASE + 20, f16, r2rm,   "vector float round and detect inexact half")  /* reserved 21-27 */ \
    X(vfcmpoeqh,  VFP16_BASE + 28, f16, r3,     "vector float compare ordered and equal half") \
    X(vfcmponeh,  VFP16_BASE + 29, f16, r3,     "vector float compare ordered and not-equal half") \
    X(vfcmpolth,  VFP16_BASE + 30, f16, r3,     "vector float compare ordered and less than half") \
    X(vfcmpogeh,  VFP16_BASE + 31, f16, r3,     "vector float compare ordered and greater-equal half") \
    X(vfcmpoh,    VFP16_BASE + 32, f16, r3,     "vector float compare ordered half") \
    X(vfcmpueqh,  VFP16_BASE + 33, f16, r3,     "vector float compare unordered or equal half") \
    X(vfcmpuneh,  VFP16_BASE + 34, f16, r3,     "vector float compare unordered or not-equal half") \
    X(vfcmpulth,  VFP16_BASE + 35, f16, r3,     "vector float compare unordered or less than half") \
    X(vfcmpugeh,  VFP16_BASE + 36, f16, r3,     "vector float compare unordered or greater-equal half") \
    X(vfcmpuh,    VFP16_BASE + 37, f16, r3,     "vector float compare unordered half") \
    X(vfaddsubh,  VFP16_BASE + 40, f16, r3rm,   "vector float add|subtract alternating half") \
    X(vfsubaddh,  VFP16_BASE + 41, f16, r3rm,   "vector float subtract|add alternating half") \
    X(vfhaddh,    VFP16_BASE + 42, f16, r3rm,   "vector float horizontal add half") \
    X(vfhsubh,    VFP16_BASE + 43, f16, r3rm,   "vector float horizontal subtract half") \
    X(vfhmulh,    VFP16_BASE + 44, f16, r3rm,   "vector float horizontal multiply half") \
    X(vfdoth,     VFP16_BASE + 45, f16, r3rm,   "vector float dot-product half") \
    X(vfmergelh,  VFP16_BASE + 46, f16, r3rm,   "vector float merge low parts half") \
    X(vfmergehh,  VFP16_BASE + 47, f16, r3rm,   "vector float merge high parts half") \
    X(vfupkhh,    VFP16_BASE + 48, f16, r2,     "vector float unpack high part half") \
    X(vfupklh,    VFP16_BASE + 49, f16, r2,     "vector float unpack low part half") \
    X(vfpackh,    VFP16_BASE + 50, f16, r3,     "vector float pack half") \
    X(vfhtoih,    VFP16_BASE + 51, f16, r2rm,   "vector float convert half to signed halfword") \
    X(vfhtouh,    VFP16_BASE + 52, f16, r2rm,   "vector float convert half to unsigned halfword") \
    X(vfihtoh,    VFP16_BASE + 53, f16, r2rm,   "vector float convert signed halfword to half") \
    X(vfuhtoh,    VFP16_BASE + 54, f16, r2rm,   "vector float convert unsigned halfword to half") \
    /* f32 */ \
    X(fclasss,    FPS_BASE +  0, f32, fpclass, "float classify single") \
    X(fsqrts,     FPS_BASE +  1, f32, r2rm,    "float square root single") \
    X(frsqrts,    FPS_BASE +  2, f32, r2rm,    "float reciprocal square root single") \
    X(fadds,      FPS_BASE +  3, f32, r3rm,    "float add single") \
    X(fsubs,      FPS_BASE +  4, f32, r3rm,    "float subtract single") \
    X(fnadds,     FPS_BASE +  5, f32, r3rm,    "float negate add single") \
    X(fmuls,      FPS_BASE +  6, f32, r3rm,    "float multiply single") \
    X(fnmuls,     FPS_BASE +  7, f32, r3rm,    "float negate multiply single") \
    X(fdivs,      FPS_BASE +  8, f32, r3rm,    "float divide single") \
    X(fnegs,      FPS_BASE +  9, f32, r2,      "float negate single") \
    X(fabss,      FPS_BASE + 10, f32, r2,      "float absolute value single") \
    X(fnabss,     FPS_BASE + 11, f32, r2,      "float negate absolute value single") \
    X(fabds,      FPS_BASE + 12, f32, r3rm,    "float absolute difference single") \
    X(fnabds,     FPS_BASE + 13, f32, r3rm,    "float negate absolute difference single") \
    X(fmins,      FPS_BASE + 14, f32, r3,      "float minimum single") \
    X(fmaxs,      FPS_BASE + 15, f32, r3,      "float maximum single") \
    X(fminnms,    FPS_BASE + 16, f32, r3,      "float minimum number single") \
    X(fmaxnms,    FPS_BASE + 17, f32, r3,      "float maximum number single") \
    X(faminnms,   FPS_BASE + 18, f32, r3,      "float absolute minimum number single") \
    X(famaxnms,   FPS_BASE + 19, f32, r3,      "float absolute maximum number single") \
    X(frnds,      FPS_BASE + 20, f32, r2rm,    "float round single") \
    X(frndxs,     FPS_BASE + 21, f32, r2rm,    "float round and detect inexact single") /* reserved 22,23 */ \
    X(fcmpoeqs,   FPS_BASE + 24, f32, r3,      "float compare ordered and equal single") \
    X(fcmpones,   FPS_BASE + 25, f32, r3,      "float compare ordered and not-equal single") \
    X(fcmpolts,   FPS_BASE + 26, f32, r3,      "float compare ordered and less than single") \
    X(fcmpoges,   FPS_BASE + 27, f32, r3,      "float compare ordered and greater-or-equal single") \
    X(fcmpos,     FPS_BASE + 28, f32, r3,      "float compare ordered single") \
    X(fcmpueqs,   FPS_BASE + 29, f32, r3,      "float compare unordered or equal single") \
    X(fcmpunes,   FPS_BASE + 30, f32, r3,      "float compare unordered or not-equal single") \
    X(fcmpults,   FPS_BASE + 31, f32, r3,      "float compare unordered or less than single") \
    X(fcmpuges,   FPS_BASE + 32, f32, r3,      "float compare unordered or greater-or-equal single") \
    X(fcmpus,     FPS_BASE + 33, f32, r3,      "float compare unordered single") /* reserved 34-39 */ \
    X(fstoiw,     FPS_BASE + 40, f32, r2rm,    "float convert single to signed word") \
    X(fstouw,     FPS_BASE + 41, f32, r2rm,    "float convert single to unsigned word") \
    X(fiwtos,     FPS_BASE + 42, f32, r2rm,    "float convert signed word to single") \
    X(fuwtos,     FPS_BASE + 43, f32, r2rm,    "float convert unsigned word to single") \
    X(fstoid,     FPS_BASE + 44, f32, r2rm,    "float convert single to signed doubleword") \
    X(fstoud,     FPS_BASE + 45, f32, r2rm,    "float convert single to unsigned doubleword") \
    X(fidtos,     FPS_BASE + 46, f32, r2rm,    "float convert signed doubleword to single") \
    X(fudtos,     FPS_BASE + 47, f32, r2rm,    "float convert unsigned doubleword to single") \
    X(fstoiq,     FPS_BASE + 48, f32, r2rm,    "float convert single to signed quadword") \
    X(fstouq,     FPS_BASE + 49, f32, r2rm,    "float convert single to unsigned quadword") \
    X(fiqtos,     FPS_BASE + 50, f32, r2rm,    "float convert signed quadword to single") \
    X(fuqtos,     FPS_BASE + 51, f32, r2rm,    "float convert unsigned quadword to single") \
    X(fstod,      FPS_BASE + 56, f32, r2,      "float extend single to double") \
    X(fdtos,      FPS_BASE + 57, f32, r2rm,    "float convert double to single") \
    /* vf32 */ \
    X(vfnegs,     VFP32_BASE +  0, f32, r2,     "vector float negate single") \
    X(vfabss,     VFP32_BASE +  1, f32, r2,     "vector float absolute value single") \
    X(vfnabss,    VFP32_BASE +  2, f32, r2,     "vector float negate absolute value single") \
    X(vfabds,     VFP32_BASE +  3, f32, r3rm,   "vector float absolute difference single") \
    X(vfnabds,    VFP32_BASE +  4, f32, r3rm,   "vector float negate absolute difference single") \
    X(vfrsqrts,   VFP32_BASE +  5, f32, r2rm,   "vector float reciprocal square root single") \
    X(vfsqrts,    VFP32_BASE +  6, f32, r2rm,   "vector float square root single") \
    X(vfadds,     VFP32_BASE +  7, f32, r3rm,   "vector float add single") \
    X(vfsubs,     VFP32_BASE +  8, f32, r3rm,   "vector float subtract single") \
    X(vfnadds,    VFP32_BASE +  9, f32, r3rm,   "vector float negate add single") \
    X(vfmuls,     VFP32_BASE + 10, f32, r3rm,   "vector float multiply single") \
    X(vfnmuls,    VFP32_BASE + 11, f32, r3rm,   "vector float negate multiply single") \
    X(vfdivs,     VFP32_BASE + 12, f32, r3rm,   "vector float divide single") \
    X(vfmins,     VFP32_BASE + 13, f32, r3,     "vector float minimum single") \
    X(vfmaxs,     VFP32_BASE + 14, f32, r3,     "vector float maximum single") \
    X(vfminnms,   VFP32_BASE + 15, f32, r3,     "vector float minimum number single") \
    X(vfmaxnms,   VFP32_BASE + 16, f32, r3,     "vector float maximum number single") \
    X(vfaminnms,  VFP32_BASE + 17, f32, r3,     "vector float absolute minimum number single") \
    X(vfamaxnms,  VFP32_BASE + 18, f32, r3,     "vector float absolute maximum number single") \
    X(vfrnds,     VFP32_BASE + 19, f32, r2rm,   "vector float round single") \
    X(vfrndxs,    VFP32_BASE + 20, f32, r2rm,   "vector float round and detect inexact single") /* reserved 21-27 */ \
    X(vfcmpoeqs,  VFP32_BASE + 28, f32, r3,     "vector float compare ordered and equal single") \
    X(vfcmpones,  VFP32_BASE + 29, f32, r3,     "vector float compare ordered and not-equal single") \
    X(vfcmpolts,  VFP32_BASE + 30, f32, r3,     "vector float compare ordered and less than single") \
    X(vfcmpoges,  VFP32_BASE + 31, f32, r3,     "vector float compare ordered and greater-equal single") \
    X(vfcmpos,    VFP32_BASE + 32, f32, r3,     "vector float compare ordered single") \
    X(vfcmpueqs,  VFP32_BASE + 33, f32, r3,     "vector float compare unordered or equal single") \
    X(vfcmpunes,  VFP32_BASE + 34, f32, r3,     "vector float compare unordered or not-equal single") \
    X(vfcmpults,  VFP32_BASE + 35, f32, r3,     "vector float compare unordered or less than single") \
    X(vfcmpuges,  VFP32_BASE + 36, f32, r3,     "vector float compare unordered or greater-equal single") \
    X(vfcmpus,    VFP32_BASE + 37, f32, r3,     "vector float compare unordered single") \
    X(vfaddsubs,  VFP32_BASE + 40, f32, r3rm,   "vector float add|subtract alternating single") \
    X(vfsubadds,  VFP32_BASE + 41, f32, r3rm,   "vector float subtract|add alternating single") \
    X(vfhadds,    VFP32_BASE + 42, f32, r3rm,   "vector float horizontal add single") \
    X(vfhsubs,    VFP32_BASE + 43, f32, r3rm,   "vector float horizontal subtract single") \
    X(vfhmuls,    VFP32_BASE + 44, f32, r3rm,   "vector float horizontal multiply single") \
    X(vfdots,     VFP32_BASE + 45, f32, r3rm,   "vector float dot-product single") \
    X(vfmergels,  VFP32_BASE + 46, f32, r3rm,   "vector float merge low parts single") \
    X(vfmergehs,  VFP32_BASE + 47, f32, r3rm,   "vector float merge high parts single") \
    X(vfupkhs,    VFP32_BASE + 48, f32, r2,     "vector float unpack high part single") \
    X(vfupkls,    VFP32_BASE + 49, f32, r2,     "vector float unpack low part single") \
    X(vfpacks,    VFP32_BASE + 50, f32, r3,     "vector float pack single") \
    X(vfstoiw,    VFP32_BASE + 51, f32, r2rm,   "vector float convert single to signed word") \
    X(vfstouw,    VFP32_BASE + 52, f32, r2rm,   "vector float convert single to unsigned word") \
    X(vfiwtos,    VFP32_BASE + 53, f32, r2rm,   "vector float convert signed word to single") \
    X(vfuwtos,    VFP32_BASE + 54, f32, r2rm,   "vector float convert unsigned word to single") \
    /* f64 */ \
    X(fclassd,    FPD_BASE +  0, f64, fpclass, "float classify double") \
    X(fsqrtd,     FPD_BASE +  1, f64, r2rm,    "float square root double") \
    X(frsqrtd,    FPD_BASE +  2, f64, r2rm,    "float reciprocal square root double") \
    X(faddd,      FPD_BASE +  3, f64, r3rm,    "float add double") \
    X(fsubd,      FPD_BASE +  4, f64, r3rm,    "float subtract double") \
    X(fnaddd,     FPD_BASE +  5, f64, r3rm,    "float negate add double") \
    X(fmuld,      FPD_BASE +  6, f64, r3rm,    "float multiply double") \
    X(fnmuld,     FPD_BASE +  7, f64, r3rm,    "float negate multiply double") \
    X(fdivd,      FPD_BASE +  8, f64, r3rm,    "float divide double") \
    X(fnegd,      FPD_BASE +  9, f64, r2,      "float negate double") \
    X(fabsd,      FPD_BASE + 10, f64, r2,      "float absolute value double") \
    X(fnabsd,     FPD_BASE + 11, f64, r2,      "float negate absolute value double") \
    X(fabdd,      FPD_BASE + 12, f64, r3rm,    "float absolute difference double") \
    X(fnabdd,     FPD_BASE + 13, f64, r3rm,    "float negate absolute difference double") \
    X(fmind,      FPD_BASE + 14, f64, r3,      "float minimum double") \
    X(fmaxd,      FPD_BASE + 15, f64, r3,      "float maximum double") \
    X(fminnmd,    FPD_BASE + 16, f64, r3,      "float minimum number double") \
    X(fmaxnmd,    FPD_BASE + 17, f64, r3,      "float maximum number double") \
    X(faminnmd,   FPD_BASE + 18, f64, r3,      "float absolute minimum number double") \
    X(famaxnmd,   FPD_BASE + 19, f64, r3,      "float absolute maximum number double") \
    X(frndd,      FPD_BASE + 20, f64, r2rm,    "float round double") \
    X(frndxd,     FPD_BASE + 21, f64, r2rm,    "float round and detect inexact double") /* reserved 22,23 */ \
    X(fcmpoeqd,   FPD_BASE + 24, f64, r3,      "float compare ordered and equal double") \
    X(fcmponed,   FPD_BASE + 25, f64, r3,      "float compare ordered and not-equal double") \
    X(fcmpoltd,   FPD_BASE + 26, f64, r3,      "float compare ordered and less than double") \
    X(fcmpoged,   FPD_BASE + 27, f64, r3,      "float compare ordered and greater-or-equal double") \
    X(fcmpod,     FPD_BASE + 28, f64, r3,      "float compare ordered double") \
    X(fcmpueqd,   FPD_BASE + 29, f64, r3,      "float compare unordered or equal double") \
    X(fcmpuned,   FPD_BASE + 30, f64, r3,      "float compare unordered or not-equal double") \
    X(fcmpultd,   FPD_BASE + 31, f64, r3,      "float compare unordered or less than double") \
    X(fcmpuged,   FPD_BASE + 32, f64, r3,      "float compare unordered or greater-or-equal double") \
    X(fcmpud,     FPD_BASE + 33, f64, r3,      "float compare unordered double") /* reserved 34-39 */ \
    X(fdtoiw,     FPD_BASE + 40, f64, r2rm,    "float convert double to signed word") \
    X(fdtouw,     FPD_BASE + 41, f64, r2rm,    "float convert double to unsigned word") \
    X(fiwtod,     FPD_BASE + 42, f64, r2rm,    "float convert signed word to double") \
    X(fuwtod,     FPD_BASE + 43, f64, r2rm,    "float convert unsigned word to double") \
    X(fdtoid,     FPD_BASE + 44, f64, r2rm,    "float convert double to signed doubleword") \
    X(fdtoud,     FPD_BASE + 45, f64, r2rm,    "float convert double to unsigned doubleword") \
    X(fidtod,     FPD_BASE + 46, f64, r2rm,    "float convert signed doubleword to double") \
    X(fudtod,     FPD_BASE + 47, f64, r2rm,    "float convert unsigned doubleword to double") \
    X(fdtoiq,     FPD_BASE + 48, f64, r2rm,    "float convert double to signed quadword") \
    X(fdtouq,     FPD_BASE + 49, f64, r2rm,    "float convert double to unsigned quadword") \
    X(fiqtod,     FPD_BASE + 50, f64, r2rm,    "float convert signed quadword to double") \
    X(fuqtod,     FPD_BASE + 51, f64, r2rm,    "float convert unsigned quadword to double") \
    /* vf64 */ \
    X(vfnegd,     VFP64_BASE +  0, f64, r2,     "vector float negate double") \
    X(vfabsd,     VFP64_BASE +  1, f64, r2,     "vector float absolute value double") \
    X(vfnabsd,    VFP64_BASE +  2, f64, r2,     "vector float negate absolute value double") \
    X(vfabdd,     VFP64_BASE +  3, f64, r3rm,   "vector float absolute difference double") \
    X(vfnabdd,    VFP64_BASE +  4, f64, r3rm,   "vector float negate absolute difference double") \
    X(vfrsqrtd,   VFP64_BASE +  5, f64, r2rm,   "vector float reciprocal square root double") \
    X(vfsqrtd,    VFP64_BASE +  6, f64, r2rm,   "vector float square root double") \
    X(vfaddd,     VFP64_BASE +  7, f64, r3rm,   "vector float add double") \
    X(vfsubd,     VFP64_BASE +  8, f64, r3rm,   "vector float subtract double") \
    X(vfnaddd,    VFP64_BASE +  9, f64, r3rm,   "vector float negate add double") \
    X(vfmuld,     VFP64_BASE + 10, f64, r3rm,   "vector float multiply double") \
    X(vfnmuld,    VFP64_BASE + 11, f64, r3rm,   "vector float negate multiply double") \
    X(vfdivd,     VFP64_BASE + 12, f64, r3rm,   "vector float divide double") \
    X(vfmind,     VFP64_BASE + 13, f64, r3,     "vector float minimum double") \
    X(vfmaxd,     VFP64_BASE + 14, f64, r3,     "vector float maximum double") \
    X(vfminnmd,   VFP64_BASE + 15, f64, r3,     "vector float minimum number double") \
    X(vfmaxnmd,   VFP64_BASE + 16, f64, r3,     "vector float maximum number double") \
    X(vfaminnmd,  VFP64_BASE + 17, f64, r3,     "vector float absolute minimum number double") \
    X(vfamaxnmd,  VFP64_BASE + 18, f64, r3,     "vector float absolute maximum number double") \
    X(vfrndd,     VFP64_BASE + 19, f64, r2rm,   "vector float round double") \
    X(vfrndxd,    VFP64_BASE + 20, f64, r2rm,   "vector float round and detect inexact double") /* reserved 21-27 */ \
    X(vfcmpoeqd,  VFP64_BASE + 28, f64, r3,     "vector float compare ordered and equal double") \
    X(vfcmponed,  VFP64_BASE + 29, f64, r3,     "vector float compare ordered and not-equal double") \
    X(vfcmpoltd,  VFP64_BASE + 30, f64, r3,     "vector float compare ordered and less than double") \
    X(vfcmpoged,  VFP64_BASE + 31, f64, r3,     "vector float compare ordered and greater-equal double") \
    X(vfcmpod,    VFP64_BASE + 32, f64, r3,     "vector float compare ordered double") \
    X(vfcmpueqd,  VFP64_BASE + 33, f64, r3,     "vector float compare unordered or equal double") \
    X(vfcmpuned,  VFP64_BASE + 34, f64, r3,     "vector float compare unordered or not-equal double") \
    X(vfcmpultd,  VFP64_BASE + 35, f64, r3,     "vector float compare unordered or less than double") \
    X(vfcmpuged,  VFP64_BASE + 36, f64, r3,     "vector float compare unordered or greater-equal double") \
    X(vfcmpud,    VFP64_BASE + 37, f64, r3,     "vector float compare unordered double") \
    X(vfaddsubd,  VFP64_BASE + 40, f64, r3rm,   "vector float add|subtract alternating double") \
    X(vfsubaddd,  VFP64_BASE + 41, f64, r3rm,   "vector float subtract|add alternating double") \
    X(vfhaddd,    VFP64_BASE + 42, f64, r3rm,   "vector float horizontal add double") \
    X(vfhsubd,    VFP64_BASE + 43, f64, r3rm,   "vector float horizontal subtract double") \
    X(vfhmuld,    VFP64_BASE + 44, f64, r3rm,   "vector float horizontal multiply double") \
    X(vfdotd,     VFP64_BASE + 45, f64, r3rm,   "vector float dot-product double") \
    X(vfmergeld,  VFP64_BASE + 46, f64, r3rm,   "vector float merge low parts double") \
    X(vfmergehd,  VFP64_BASE + 47, f64, r3rm,   "vector float merge high parts double") \
    X(vfupkhd,    VFP64_BASE + 48, f64, r2,     "vector float unpack high part double") \
    X(vfupkld,    VFP64_BASE + 49, f64, r2,     "vector float unpack low part double") \
    X(vfpackd,    VFP64_BASE + 50, f64, r3,     "vector float pack double") \
    X(vfdtoid,    VFP64_BASE + 51, f64, r2rm,   "vector float convert double to signed doubleword") \
    X(vfdtoud,    VFP64_BASE + 52, f64, r2rm,   "vector float convert double to unsigned doubleword") \
    X(vfidtod,    VFP64_BASE + 53, f64, r2rm,   "vector float convert signed doubleword to double") \
    X(vfudtod,    VFP64_BASE + 54, f64, r2rm,   "vector float convert unsigned doubleword to double") \
    /* f128 */ \
    X(fclassq,    FPQ_BASE +  0, f128, fpclass, "float classify quadruple") \
    X(fsqrtq,     FPQ_BASE +  1, f128, r2rm,    "float square root quadruple") \
    X(frsqrtq,    FPQ_BASE +  2, f128, r2rm,    "float reciprocal square root quadruple") \
    X(faddq,      FPQ_BASE +  3, f128, r3rm,    "float add quadruple") \
    X(fsubq,      FPQ_BASE +  4, f128, r3rm,    "float subtract quadruple") \
    X(fnaddq,     FPQ_BASE +  5, f128, r3rm,    "float negate add quadruple") \
    X(fmulq,      FPQ_BASE +  6, f128, r3rm,    "float multiply quadruple") \
    X(fnmulq,     FPQ_BASE +  7, f128, r3rm,    "float negate multiply quadruple") \
    X(fdivq,      FPQ_BASE +  8, f128, r3rm,    "float divide quadruple") \
    X(fnegq,      FPQ_BASE +  9, f128, r2,      "float negate quadruple") \
    X(fabsq,      FPQ_BASE + 10, f128, r2,      "float absolute value quadruple") \
    X(fnabsq,     FPQ_BASE + 11, f128, r2,      "float negate absolute value quadruple") \
    X(fabdq,      FPQ_BASE + 12, f128, r3rm,    "float absolute difference quadruple") \
    X(fnabdq,     FPQ_BASE + 13, f128, r3rm,    "float negate absolute difference quadruple") \
    X(fminq,      FPQ_BASE + 14, f128, r3,      "float minimum quadruple") \
    X(fmaxq,      FPQ_BASE + 15, f128, r3,      "float maximum quadruple") \
    X(fminnmq,    FPQ_BASE + 16, f128, r3,      "float minimum number quadruple") \
    X(fmaxnmq,    FPQ_BASE + 17, f128, r3,      "float maximum number quadruple") \
    X(faminnmq,   FPQ_BASE + 18, f128, r3,      "float absolute minimum number quadruple") \
    X(famaxnmq,   FPQ_BASE + 19, f128, r3,      "float absolute maximum number quadruple") \
    X(frndq,      FPQ_BASE + 20, f128, r2rm,    "float round quadruple") \
    X(frndxq,     FPQ_BASE + 21, f128, r2rm,    "float round and detect inexact quadruple") /* reserved 22,23 */ \
    X(fcmpoeqq,   FPQ_BASE + 24, f128, r3,      "float compare ordered and equal quadruple") \
    X(fcmponeq,   FPQ_BASE + 25, f128, r3,      "float compare ordered and not-equal quadruple") \
    X(fcmpoltq,   FPQ_BASE + 26, f128, r3,      "float compare ordered and less than quadruple") \
    X(fcmpogeq,   FPQ_BASE + 27, f128, r3,      "float compare ordered and greater-or-equal quadruple") \
    X(fcmpoq,     FPQ_BASE + 28, f128, r3,      "float compare ordered quadruple") \
    X(fcmpueqq,   FPQ_BASE + 29, f128, r3,      "float compare unordered or equal quadruple") \
    X(fcmpuneq,   FPQ_BASE + 30, f128, r3,      "float compare unordered or not-equal quadruple") \
    X(fcmpultq,   FPQ_BASE + 31, f128, r3,      "float compare unordered or less than quadruple") \
    X(fcmpugeq,   FPQ_BASE + 32, f128, r3,      "float compare unordered or greater-or-equal quadruple") \
    X(fcmpuq,     FPQ_BASE + 33, f128, r3,      "float compare unordered quadruple") /* reserved 34-39 */ \
    X(fqtoiw,     FPQ_BASE + 40, f128, r2rm,    "float convert quadruple to signed word") \
    X(fqtouw,     FPQ_BASE + 41, f128, r2rm,    "float convert quadruple to unsigned word") \
    X(fiwtoq,     FPQ_BASE + 42, f128, r2rm,    "float convert signed word to quadruple") \
    X(fuwtoq,     FPQ_BASE + 43, f128, r2rm,    "float convert unsigned word to quadruple") \
    X(fqtoid,     FPQ_BASE + 44, f128, r2rm,    "float convert quadruple to signed doubleword") \
    X(fqtoud,     FPQ_BASE + 45, f128, r2rm,    "float convert quadruple to unsigned doubleword") \
    X(fidtoq,     FPQ_BASE + 46, f128, r2rm,    "float convert signed doubleword to quadruple") \
    X(fudtoq,     FPQ_BASE + 47, f128, r2rm,    "float convert unsigned doubleword to quadruple") \
    X(fqtoiq,     FPQ_BASE + 48, f128, r2rm,    "float convert quadruple to signed quadword") \
    X(fqtouq,     FPQ_BASE + 49, f128, r2rm,    "float convert quadruple to unsigned quadword") \
    X(fiqtoq,     FPQ_BASE + 50, f128, r2rm,    "float convert signed quadword to quadruple") \
    X(fuqtoq,     FPQ_BASE + 51, f128, r2rm,    "float convert unsigned quadword to quadruple") \
    X(fstoq,      FPQ_BASE + 56, f128, r2,      "float extend single to quadruple") \
    X(fdtoq,      FPQ_BASE + 57, f128, r2,      "float extend double to quadruple") \
    X(fhtoq,      FPQ_BASE + 58, f128, r2,      "float extend half to quadruple") \
    X(fqtod,      FPQ_BASE + 59, f128, r2rm,    "float convert quadruple to double") \
    X(fqtos,      FPQ_BASE + 60, f128, r2rm,    "float convert quadruple to single") \
    X(fqtoh,      FPQ_BASE + 61, f128, r2rm,    "float convert quadruple to half") \
    X(fscaleq,    FPQ_BASE + 62, f128, r2s1,    "float scale quadruple") \
    /* simd-8 */ \
    X(vmaxsb,     MMX_BASE_B +  0, mmx, r3,   "vector maximum signed byte") \
    X(vmaxub,     MMX_BASE_B +  1, mmx, r3,   "vector maximum unsigned byte") \
    X(vminsb,     MMX_BASE_B +  2, mmx, r3,   "vector minimum signed byte") \
    X(vminub,     MMX_BASE_B +  3, mmx, r3,   "vector minimum unsigned byte") \
    X(vaddb,      MMX_BASE_B +  4, mmx, r3,   "vector add byte") \
    X(vsubb,      MMX_BASE_B +  5, mmx, r3,   "vector subtract byte") \
    X(vaddob,     MMX_BASE_B +  6, mmx, r3,   "vector add oveflow byte") \
    X(vsubob,     MMX_BASE_B +  7, mmx, r3,   "vector subtract overflow byte") \
    X(vaddcb,     MMX_BASE_B +  8, mmx, r3,   "vector add carry byte") \
    X(vsubcb,     MMX_BASE_B +  9, mmx, r3,   "vector subtract carry byte") \
    X(vaddusatb,  MMX_BASE_B + 10, mmx, r3,   "vector add unsigned saturated byte") \
    X(vaddssatb,  MMX_BASE_B + 11, mmx, r3,   "vector add signed saturated byte") \
    X(vsubssatb,  MMX_BASE_B + 12, mmx, r3,   "vector subtract signed saturated byte") \
    X(vsubusatb,  MMX_BASE_B + 13, mmx, r3,   "vector subtract unsigned saturated byte") \
    X(vavgsb,     MMX_BASE_B + 14, mmx, r3,   "vector average ceiled signed byte") \
    X(vavgub,     MMX_BASE_B + 15, mmx, r3,   "vector average ceiled unsigned byte") \
    X(vcmpeqb,    MMX_BASE_B + 16, mmx, r3,   "vector compare equal byte") \
    X(vcmpltsb,   MMX_BASE_B + 17, mmx, r3,   "vector compare less than signed byte") \
    X(vcmpltub,   MMX_BASE_B + 18, mmx, r3,   "vector compare less than unsigned byte") \
    X(vsllb,      MMX_BASE_B + 19, mmx, r3,   "vector shift left logical byte") \
    X(vsllib,     MMX_BASE_B + 20, mmx, r2s1, "vector shift left logical immediate byte") \
    X(vsrlb,      MMX_BASE_B + 21, mmx, r3,   "vector shift right logical byte") \
    X(vsrlib,     MMX_BASE_B + 22, mmx, r2s1, "vector shift right logical immediate byte") \
    X(vsrab,      MMX_BASE_B + 23, mmx, r3,   "vector shift right algebraic byte") \
    X(vsraib,     MMX_BASE_B + 24, mmx, r2s1, "vector shift right algebraic immediate byte") \
    X(vsplatb,    MMX_BASE_B + 25, mmx, r3,   "vector splat byte") \
    X(vsplatib,   MMX_BASE_B + 26, mmx, r2s1, "vector splat immediate byte") \
    X(vmergehb,   MMX_BASE_B + 27, mmx, r3,   "vector merge high byte") \
    X(vmergelb,   MMX_BASE_B + 28, mmx, r3,   "vector merge low byte") /* reserved 29-31 */ \
    X(vupklsb,    MMX_BASE_B + 32, mmx, r2,   "vector unpack low signed byte") \
    X(vupkhsb,    MMX_BASE_B + 33, mmx, r2,   "vector unpack high signed byte") \
    X(vupklub,    MMX_BASE_B + 34, mmx, r2,   "vector unpack low unsigned byte") \
    X(vupkhub,    MMX_BASE_B + 35, mmx, r2,   "vector unpack high unsigned byte") \
    /* simd-16 */ \
    X(vmaxsh,     MMX_BASE_H +  0, mmx, r3,   "vector maximum signed halfword") \
    X(vmaxuh,     MMX_BASE_H +  1, mmx, r3,   "vector maximum unsigned halfword") \
    X(vminsh,     MMX_BASE_H +  2, mmx, r3,   "vector minimum signed halfword") \
    X(vminuh,     MMX_BASE_H +  3, mmx, r3,   "vector minimum unsigned halfword") \
    X(vaddh,      MMX_BASE_H +  4, mmx, r3,   "vector add halfword") \
    X(vsubh,      MMX_BASE_H +  5, mmx, r3,   "vector subtract halfword") \
    X(vaddoh,     MMX_BASE_H +  6, mmx, r3,   "vector add overflow halfword") \
    X(vsuboh,     MMX_BASE_H +  7, mmx, r3,   "vector subtract overflow halfword") \
    X(vaddch,     MMX_BASE_H +  8, mmx, r3,   "vector add carry halfword") \
    X(vsubch,     MMX_BASE_H +  9, mmx, r3,   "vector subtract carry halfword") \
    X(vaddusath,  MMX_BASE_H + 10, mmx, r3,   "vector add unsigned saturated halfword") \
    X(vaddssath,  MMX_BASE_H + 11, mmx, r3,   "vector add signed saturated halfword") \
    X(vsubssath,  MMX_BASE_H + 12, mmx, r3,   "vector subtract signed saturated halfword") \
    X(vsubusath,  MMX_BASE_H + 13, mmx, r3,   "vector subtract unsigned saturated halfword") \
    X(vavgsh,     MMX_BASE_H + 14, mmx, r3,   "vector average ceiled signed halfword") \
    X(vavguh,     MMX_BASE_H + 15, mmx, r3,   "vector average ceiled unsigned halfword") \
    X(vcmpeqh,    MMX_BASE_H + 16, mmx, r3,   "vector compare equal halfword") \
    X(vcmpltsh,   MMX_BASE_H + 17, mmx, r3,   "vector compare less than signed halfword") \
    X(vcmpltuh,   MMX_BASE_H + 18, mmx, r3,   "vector compare less than unsigned halfword") \
    X(vsllh,      MMX_BASE_H + 19, mmx, r3,   "vector shift left logical halfword") \
    X(vsllih,     MMX_BASE_H + 20, mmx, r2s1, "vector shift left logical immediate halfword") \
    X(vsrlh,      MMX_BASE_H + 21, mmx, r3,   "vector shift right logical halfword") \
    X(vsrlih,     MMX_BASE_H + 22, mmx, r2s1, "vector shift right logical immediate halfword") \
    X(vsrah,      MMX_BASE_H + 23, mmx, r3,   "vector shift right algebraic halfword") \
    X(vsraih,     MMX_BASE_H + 24, mmx, r2s1, "vector shift right algebraic immediate halfword") \
    X(vsplath,    MMX_BASE_H + 25, mmx, r3,   "vector splat halfword") \
    X(vsplatih,   MMX_BASE_H + 26, mmx, r2s1, "vector splat immediate halfword") \
    X(vmergehh,   MMX_BASE_H + 27, mmx, r3,   "vector merge high halfword") \
    X(vmergelh,   MMX_BASE_H + 28, mmx, r3,   "vector merge low halfword") /* reserved 29-31 */ \
    X(vupklsh,    MMX_BASE_H + 32, mmx, r2,   "vector unpack low signed halfword" ) \
    X(vupkhsh,    MMX_BASE_H + 33, mmx, r2,   "vector unpack high signed halfword") \
    X(vupkluh,    MMX_BASE_H + 34, mmx, r2,   "vector unpack low unsigned halfword" ) \
    X(vupkhuh,    MMX_BASE_H + 35, mmx, r2,   "vector unpack high unsigned halfword") \
    X(vpkssath,   MMX_BASE_H + 36, mmx, r3,   "vector pack signed saturated halfword") \
    X(vpkusath,   MMX_BASE_H + 37, mmx, r3,   "vector pack unsigned saturated halfword") \
    X(vpkumodh,   MMX_BASE_H + 38, mmx, r3,   "vector pack unsigned modulo halfword") \
    X(vpkusatsh,  MMX_BASE_H + 39, mmx, r3,   "vector pack unsigned saturated signed halfword") \
    /* simd-32 */ \
    X(vmaxsw,     MMX_BASE_W +  0, mmx, r3,   "vector maximum signed word") \
    X(vmaxuw,     MMX_BASE_W +  1, mmx, r3,   "vector maximum unsigned word") \
    X(vminsw,     MMX_BASE_W +  2, mmx, r3,   "vector minimum signed word") \
    X(vminuw,     MMX_BASE_W +  3, mmx, r3,   "vector minimum unsigned word") \
    X(vaddw,      MMX_BASE_W +  4, mmx, r3,   "vector add word") \
    X(vsubw,      MMX_BASE_W +  5, mmx, r3,   "vector subtract word") \
    X(vaddow,     MMX_BASE_W +  6, mmx, r3,   "vector add overflow word") \
    X(vsubow,     MMX_BASE_W +  7, mmx, r3,   "vector subtract overflow word") \
    X(vaddcw,     MMX_BASE_W +  8, mmx, r3,   "vector add carry out word") \
    X(vsubcw,     MMX_BASE_W +  9, mmx, r3,   "vector subtract carry word") \
    X(vaddusatw,  MMX_BASE_W + 10, mmx, r3,   "vector add unsigned saturated word") \
    X(vaddssatw,  MMX_BASE_W + 11, mmx, r3,   "vector add signed saturated word") \
    X(vsubssatw,  MMX_BASE_W + 12, mmx, r3,   "vector subtract signed saturated word") \
    X(vsubusatw,  MMX_BASE_W + 13, mmx, r3,   "vector subtract unsigned saturated word") \
    X(vavgsw,     MMX_BASE_W + 14, mmx, r3,   "vector average ceiled signed word") \
    X(vavguw,     MMX_BASE_W + 15, mmx, r3,   "vector average ceiled unsigned word") \
    X(vcmpeqw,    MMX_BASE_W + 16, mmx, r3,   "vector compare equal word") \
    X(vcmpltsw,   MMX_BASE_W + 17, mmx, r3,   "vector compare less than signed word") \
    X(vcmpltuw,   MMX_BASE_W + 18, mmx, r3,   "vector compare less than unsigned word") \
    X(vsllw,      MMX_BASE_W + 19, mmx, r3,   "vector shift left logical word") \
    X(vslliw,     MMX_BASE_W + 20, mmx, r2s1, "vector shift left logical immediate word") \
    X(vsrlw,      MMX_BASE_W + 21, mmx, r3,   "vector shift right logical word") \
    X(vsrliw,     MMX_BASE_W + 22, mmx, r2s1, "vector shift right logical immediate word") \
    X(vsraw,      MMX_BASE_W + 23, mmx, r3,   "vector shift right algebraic word") \
    X(vsraiw,     MMX_BASE_W + 24, mmx, r2s1, "vector shift right algebraic immediate word") \
    X(vsplatw,    MMX_BASE_W + 25, mmx, r3,   "vector splat word") \
    X(vsplatiw,   MMX_BASE_W + 26, mmx, r2s1, "vector splat immediate word") \
    X(vmergehw,   MMX_BASE_W + 27, mmx, r3,   "vector merge high word") \
    X(vmergelw,   MMX_BASE_W + 28, mmx, r3,   "vector merge low word") /* reserved 29-31 */ \
    X(vupklsw,    MMX_BASE_W + 32, mmx, r2,   "vector unpack low signed word" ) \
    X(vupkhsw,    MMX_BASE_W + 33, mmx, r2,   "vector unpack high signed word" ) \
    X(vupkluw,    MMX_BASE_W + 34, mmx, r2,   "vector unpack low unsigned word" ) \
    X(vupkhuw,    MMX_BASE_W + 35, mmx, r2,   "vector unpack high unsigned word" ) \
    X(vpkssatw,   MMX_BASE_W + 36, mmx, r3,   "vector pack signed saturated word") \
    X(vpkusatw,   MMX_BASE_W + 37, mmx, r3,   "vector pack unsigned saturated word") \
    X(vpkumodw,   MMX_BASE_W + 38, mmx, r3,   "vector pack unsigned modulo word") \
    X(vpkusatsw,  MMX_BASE_W + 39, mmx, r3,   "vector pack unsigned saturated signed word") \
    /* simd-64 */ \
    X(vmaxsd,     MMX_BASE_D +  0, mmx, r3,   "vector maximum signed doubleword") \
    X(vmaxud,     MMX_BASE_D +  1, mmx, r3,   "vector maximum unsigned doubleword") \
    X(vminsd,     MMX_BASE_D +  2, mmx, r3,   "vector minimum signed doubleword") \
    X(vminud,     MMX_BASE_D +  3, mmx, r3,   "vector minimum unsigned doubleword") \
    X(vaddd,      MMX_BASE_D +  4, mmx, r3,   "vector add doubleword") \
    X(vsubd,      MMX_BASE_D +  5, mmx, r3,   "vector subtract doubleword") \
    X(vaddod,     MMX_BASE_D +  6, mmx, r3,   "vector add overflow doubleword" ) \
    X(vsubod,     MMX_BASE_D +  7, mmx, r3,   "vector subtract overflow doubleword") \
    X(vaddcd,     MMX_BASE_D +  8, mmx, r3,   "vector add carry doubleword") \
    X(vsubcd,     MMX_BASE_D +  9, mmx, r3,   "vector subtract carry doubleword") \
    X(vaddusatd,  MMX_BASE_D + 10, mmx, r3,   "vector add unsigned saturated doubleword") \
    X(vaddssatd,  MMX_BASE_D + 11, mmx, r3,   "vector add signed saturated doubleword") \
    X(vsubssatd,  MMX_BASE_D + 12, mmx, r3,   "vector subtract signed saturated doubleword") \
    X(vsubusatd,  MMX_BASE_D + 13, mmx, r3,   "vector subtract unsigned saturated doubleword") \
    X(vavgsd,     MMX_BASE_D + 14, mmx, r3,   "vector average ceiled signed doubleword") \
    X(vavgud,     MMX_BASE_D + 15, mmx, r3,   "vector average ceiled unsigned doubleword") \
    X(vcmpeqd,    MMX_BASE_D + 16, mmx, r3,   "vector compare equal doubleword") \
    X(vcmpltsd,   MMX_BASE_D + 17, mmx, r3,   "vector compare less than signed doubleword") \
    X(vcmpltud,   MMX_BASE_D + 18, mmx, r3,   "vector compare less than unsigned doubleword") \
    X(vslld,      MMX_BASE_D + 19, mmx, r3,   "vector shift left logical doubleword") \
    X(vsllid,     MMX_BASE_D + 20, mmx, r2s1, "vector shift left logical immediate doubleword") \
    X(vsrld,      MMX_BASE_D + 21, mmx, r3,   "vector shift right logical doubleword") \
    X(vsrlid,     MMX_BASE_D + 22, mmx, r2s1, "vector shift right logical immediate doubleword") \
    X(vsrad,      MMX_BASE_D + 23, mmx, r3,   "vector shift right algebraic doubleword") \
    X(vsraid,     MMX_BASE_D + 24, mmx, r2s1, "vector shift right algebraic immediate doubleword") \
    X(vsplatd,    MMX_BASE_D + 25, mmx, r3,   "vector splat doubleword") \
    X(vsplatid,   MMX_BASE_D + 26, mmx, r2s1, "vector splat immediate doubleword") \
    X(vmergehd,   MMX_BASE_D + 27, mmx, r3,   "vector merge high doubleword") \
    X(vmergeld,   MMX_BASE_D + 28, mmx, r3,   "vector merge low doubleword")  /* reserved 29-35 */ \
    X(vpkssatd,   MMX_BASE_D + 36, mmx, r3,   "vector pack signed saturated doubleword") \
    X(vpkusatd,   MMX_BASE_D + 37, mmx, r3,   "vector pack unsigned saturated doubleword") \
    X(vpkumodd,   MMX_BASE_D + 38, mmx, r3,   "vector pack unsigned modulo doubleword") \
    X(vpkusatsd,  MMX_BASE_D + 39, mmx, r3,   "vector pack unsigned saturated signed doubleword") \

/***************************************************************************
* all enums
***************************************************************************/
#define OPCODE_X(ID, VALUE, SUBSET, FORMAT, DESCR) opcode_##ID = VALUE,

enum EInstructionOpcode {
    DECLARE_PRIMARY_OPCODES(OPCODE_X)
    opcode_last,
    // pseudo instructions
    opcode_fldid = opcode_ldi,
    opcode_fldis = opcode_ldi,
    opcode_fldsr = opcode_lduwr,
    opcode_flddr = opcode_ldudr,
    opcode_fldqr = opcode_ldqr,
};

enum fused_opx   { DECLARE_FUSED_EXTENDED_OPCODES(OPCODE_X) };
enum fmah_opx    { DECLARE_FMAH_EXTENDED_OPCODES(OPCODE_X) };
enum fmas_opx    { DECLARE_FMAS_EXTENDED_OPCODES(OPCODE_X) };
enum fmad_opx    { DECLARE_FMAD_EXTENDED_OPCODES(OPCODE_X) };
enum fmaq_opx    { DECLARE_FMAQ_EXTENDED_OPCODES(OPCODE_X) };
enum brh_opx     { DECLARE_BRH_EXTENDED_OPCODES(OPCODE_X) };
enum brw_opx     { DECLARE_BRW_EXTENDED_OPCODES(OPCODE_X) };
enum brd_opx     { DECLARE_BRD_EXTENDED_OPCODES(OPCODE_X) };
enum brq_opx     { DECLARE_BRQ_EXTENDED_OPCODES(OPCODE_X) };
enum loop_opx    { DECLARE_LOOP_EXTENDED_OPCODES(OPCODE_X) };
enum raopx_opx   { DECLARE_RAOPX_EXTENDED_OPCODES(OPCODE_X) };
enum memxsd_opx  { DECLARE_MEMXSD_EXTENDED_OPCODES(OPCODE_X) };
enum memxud_opx  { DECLARE_MEMXUD_EXTENDED_OPCODES(OPCODE_X) };
enum memxsw_opx  { DECLARE_MEMXSW_EXTENDED_OPCODES(OPCODE_X) };
enum memxuw_opx  { DECLARE_MEMXUW_EXTENDED_OPCODES(OPCODE_X) };
enum misc_opx    { DECLARE_MISC_EXTENDED_OPCODES(OPCODE_X) };
enum nulh_opx    { DECLARE_NULH_EXTENDED_OPCODES(OPCODE_X) };
enum nulw_opx    { DECLARE_NULW_EXTENDED_OPCODES(OPCODE_X) };
enum nuld_opx    { DECLARE_NULD_EXTENDED_OPCODES(OPCODE_X) };
enum nulq_opx    { DECLARE_NULQ_EXTENDED_OPCODES(OPCODE_X) };

/***************************************************************************
* instructions bit patterns
***************************************************************************/
inline constexpr u64 makepart(u64 item, unsigned int offset) { return item << offset; }

// 1st level codes
inline constexpr u64 instr_tmplt(EInstructionOpcode opc) { return makepart(static_cast<u64>(opc), disp_opcode); }
// 2nd level codes
inline constexpr u64 instr_tmplt(fused_opx   opx) { return instr_tmplt(EInstructionOpcode::opcode_fused)  | makepart(static_cast<u64>(opx), disp_fused_opx); }
inline constexpr u64 instr_tmplt(fmah_opx    opx) { return instr_tmplt(EInstructionOpcode::opcode_fmah)   | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(fmas_opx    opx) { return instr_tmplt(EInstructionOpcode::opcode_fmas)   | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(fmad_opx    opx) { return instr_tmplt(EInstructionOpcode::opcode_fmad)   | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(fmaq_opx    opx) { return instr_tmplt(EInstructionOpcode::opcode_fmaq)   | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(brh_opx     opx) { return instr_tmplt(EInstructionOpcode::opcode_brh)    | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(brw_opx     opx) { return instr_tmplt(EInstructionOpcode::opcode_brw)    | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(brd_opx     opx) { return instr_tmplt(EInstructionOpcode::opcode_brd)    | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(brq_opx     opx) { return instr_tmplt(EInstructionOpcode::opcode_brq)    | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(loop_opx    opx) { return instr_tmplt(EInstructionOpcode::opcode_loop)   | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(raopx_opx   opx) { return instr_tmplt(EInstructionOpcode::opcode_raopx)  | makepart(static_cast<u64>(opx), disp_reg_a); }
inline constexpr u64 instr_tmplt(memxsd_opx  opx) { return instr_tmplt(EInstructionOpcode::opcode_memxsd) | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(memxud_opx  opx) { return instr_tmplt(EInstructionOpcode::opcode_memxud) | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(memxsw_opx  opx) { return instr_tmplt(EInstructionOpcode::opcode_memxsw) | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(memxuw_opx  opx) { return instr_tmplt(EInstructionOpcode::opcode_memxuw) | makepart(static_cast<u64>(opx), disp_indexed_opx); }
inline constexpr u64 instr_tmplt(misc_opx    opx) { return instr_tmplt(EInstructionOpcode::opcode_misc)   | makepart(static_cast<u64>(opx), disp_misc_opx); }
// 3rd level codes
inline constexpr u64 instr_tmplt(nulh_opx    opx) { return instr_tmplt(fused_opx::opcode_nulh) | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(nulw_opx    opx) { return instr_tmplt(fused_opx::opcode_nulw) | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(nuld_opx    opx) { return instr_tmplt(fused_opx::opcode_nuld) | makepart(static_cast<u64>(opx), disp_branch_opx); }
inline constexpr u64 instr_tmplt(nulq_opx    opx) { return instr_tmplt(fused_opx::opcode_nulq) | makepart(static_cast<u64>(opx), disp_branch_opx); }

#define INSN_TMPLT(opc) instr_tmplt(opcode_##opc)

/***************************************************************************
* all instructions subgroups
***************************************************************************/
#define DECLARE_SUBGOUP_OPCODES(X) \
    X(brh) \
    X(brw) \
    X(brd) \
    X(brq) \
    X(fused) \
    X(nulh) \
    X(nulw) \
    X(nuld) \
    X(nulq) \
    X(memxsd) \
    X(memxud) \
    X(memxsw) \
    X(memxuw) \
    X(fmah) \
    X(fmas) \
    X(fmad) \
    X(fmaq) \
    X(loop) \
    X(raopx) \
    X(misc) \

/***************************************************************************
* predefined instruction fields:
* rounding modes, memory ordering, floating classifications
***************************************************************************/
enum class RoundingMode {
    RNE = 0, // Round to Nearest, ties to Even
    RTZ = 1, // Round towards Zero
    RDN = 2, // Round Down
    RUP = 3, // Round Up
    RMM = 4, // Round to Nearest, ties to Max Magnitude
    RNO = 5, // Round to Nearest, ties to Odd
    RTO = 6, // Round to Odd (Von Neumann)
    DYN = 7, // dynamic mode
    Default = DYN
};

const char *RoundingMode2Str(RoundingMode value);
std::ostream& operator<<(std::ostream& out, RoundingMode value);

enum class FExceptionFlag : u8 {
    Invalid      = 1 << 0,
    DivideByZero = 1 << 1,
    Overflow     = 1 << 2,
    Underflow    = 1 << 3,
    Inexact      = 1 << 4,
    All          = Invalid | DivideByZero | Overflow | Underflow | Inexact,
};

enum class MemoryOrdering {
    RELAXED = 0,
    ACQUIRE = 1,
    RELEASE = 2,
    ACQ_REL = 3,
    SEQ_CST = 7,
    Default = RELAXED
};

const char *MemoryOrdering2Str(MemoryOrdering value);
std::ostream& operator<<(std::ostream& out, MemoryOrdering value);

// Wwe can't use here directly the __FPCLASS defines, because GCC doesn't know about them.
// So use constants and check that they are identical - for CLANG only.
enum class FClassMask {
  Empty             = 0x000,
  SignalingNaN      = 0x001,
  QuietNaN          = 0x002,
  NegativeInfinity  = 0x004,
  NegativeNormal    = 0x008,
  NegativeSubnormal = 0x010,
  NegativeZero      = 0x020,
  PositiveZero      = 0x040,
  PositiveSubnormal = 0x080,
  PositiveNormal    = 0x100,
  PositiveInfinity  = 0x200,
};

#if defined(__clang__)
static_assert(static_cast<u32>(FClassMask::SignalingNaN)      == __FPCLASS_SNAN);
static_assert(static_cast<u32>(FClassMask::QuietNaN)          == __FPCLASS_QNAN);
static_assert(static_cast<u32>(FClassMask::NegativeInfinity)  == __FPCLASS_NEGINF);
static_assert(static_cast<u32>(FClassMask::NegativeNormal)    == __FPCLASS_NEGNORMAL);
static_assert(static_cast<u32>(FClassMask::NegativeSubnormal) == __FPCLASS_NEGSUBNORMAL);
static_assert(static_cast<u32>(FClassMask::NegativeZero)      == __FPCLASS_NEGZERO);
static_assert(static_cast<u32>(FClassMask::PositiveZero)      == __FPCLASS_POSZERO);
static_assert(static_cast<u32>(FClassMask::PositiveSubnormal) == __FPCLASS_POSSUBNORMAL);
static_assert(static_cast<u32>(FClassMask::PositiveNormal)    == __FPCLASS_POSNORMAL);
static_assert(static_cast<u32>(FClassMask::PositiveInfinity)  == __FPCLASS_POSINF);
#endif

DEFINE_ENUM_FLAG_OPERATOR_SET(FClassMask)

std::ostream& operator<<(std::ostream& out, FClassMask value);

/***************************************************************************
* all instructions combined
***************************************************************************/
#define DECLARE_INSN_TABLE(X)            \
    DECLARE_PRIMARY_OPCODES(X)           \
    DECLARE_FUSED_EXTENDED_OPCODES(X)    \
    DECLARE_FMAH_EXTENDED_OPCODES(X)     \
    DECLARE_FMAS_EXTENDED_OPCODES(X)     \
    DECLARE_FMAD_EXTENDED_OPCODES(X)     \
    DECLARE_FMAQ_EXTENDED_OPCODES(X)     \
    DECLARE_BRH_EXTENDED_OPCODES(X)      \
    DECLARE_BRW_EXTENDED_OPCODES(X)      \
    DECLARE_BRD_EXTENDED_OPCODES(X)      \
    DECLARE_BRQ_EXTENDED_OPCODES(X)      \
    DECLARE_LOOP_EXTENDED_OPCODES(X)     \
    DECLARE_RAOPX_EXTENDED_OPCODES(X)    \
    DECLARE_MEMXSD_EXTENDED_OPCODES(X)   \
    DECLARE_MEMXUD_EXTENDED_OPCODES(X)   \
    DECLARE_MEMXSW_EXTENDED_OPCODES(X)   \
    DECLARE_MEMXUW_EXTENDED_OPCODES(X)   \
    DECLARE_MISC_EXTENDED_OPCODES(X)     \
    DECLARE_NULH_EXTENDED_OPCODES(X)     \
    DECLARE_NULW_EXTENDED_OPCODES(X)     \
    DECLARE_NULD_EXTENDED_OPCODES(X)     \
    DECLARE_NULQ_EXTENDED_OPCODES(X)     \
    /* pseudos */                                                  \
    X(fldis,  0, f32,  fldis,  "float load immediate single")      \
    X(fldid,  0, f64,  fldid,  "float load immediate double")      \
    X(fldsr,  0, f32,  fldsr,  "float load single ip-relative")    \
    X(flddr,  0, f64,  flddr,  "float load double ip-relative")    \
    X(fldqr,  0, f128, fldqr,  "float load quadruple ip-relative") \

/***************************************************************************
* all instructions enum
***************************************************************************/
enum EInstructionID : u16 {
#define X(name, code, subset, format, descr) insn_##name,
    DECLARE_INSN_TABLE(X)
#undef X
    insn_illegal,
    INSTRUCTION_LIST_SIZE
};

std::ostream& operator<<(std::ostream& out, EInstructionID eid);
const char *to_string(EInstructionID insn);

} // namespace postrisc
