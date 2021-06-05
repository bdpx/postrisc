#pragma once

#include "instruction_fields.hpp"

namespace postrisc {

/***************************************************************************
*
* main opcodes - 7 bit
*
***************************************************************************/
#define DECLARE_PRIMARY_OPCODES(X) \
    X(misc) X(fused) X(raopx) X(nullify) \
    X(ldi) X(ldih) \
    X(andni) X(orni) X(andi) X(ori) X(xori) \
    X(addi) X(addi4) X(subfi) X(subfi4) X(muli) \
    X(divi) X(divui) X(modi) X(modui) \
    X(maxsi) X(maxui) X(minsi) X(minui) \
    X(cmpeqi) X(cmpnei) X(cmplti) X(cmpltui) X(cmpgti) X(cmpgtui) \
    X(callr) X(callplt) \
    X(alloc) \
    X(ldan) X(ldar) X(ldr) X(ldax) \
    X(dep) X(rlmi) \
    X(ld1) X(lds1) X(st1) X(ld1r) X(lds1r) X(st1r) X(ld1x) X(lds1x) X(st1x) \
    X(ld2) X(lds2) X(st2) X(ld2r) X(lds2r) X(st2r) X(ld2x) X(lds2x) X(st2x) \
    X(ld4) X(lds4) X(st4) X(ld4r) X(lds4r) X(st4r) X(ld4x) X(lds4x) X(st4x) \
    X(ld8) X(lds8) X(st8) X(ld8r) X(lds8r) X(st8r) X(ld8x) X(lds8x) X(st8x) \
    X(ld16) X(st16) X(ld16r) X(st16r) X(ld16x) X(st16x) \
    X(blt) X(ble) X(bltu) X(bleu) \
    X(blti) X(bgti) X(bltui) X(bgtui) \
    X(beq) X(beqi) X(bne) X(bnei) \
    X(bbc) X(bbci) X(bbs) X(bbsi) \
    X(beqz) X(bnez) X(bltz) X(blez) X(bgtz) X(bgez) \
    X(teq) X(tne) X(tlt) X(tltu) \
    X(replt) X(repgt) X(reple) X(repge) \
    X(repltu) X(repgtu) X(repleu) X(repgeu) \
    X(ehcatch) X(ehnext) \

#define OPCODE_X(id) opcode_##id,
enum EInstructionOpcode {
  DECLARE_PRIMARY_OPCODES(OPCODE_X)
  opcode_last
};

/**************************************************************************************
* opcode_fused extended opcodes (6 bits)
**************************************************************************************/
#define DECLARE_FUSED_EXTENDED_OPCODES(X) \
    X(addadd) X(addsub) X(subsub) \
    X(addaddc) X(subsubb) \
    X(slp) X(srp) X(srpi) \
    X(slsrl) X(slsra) X(slsrli) X(slsrai) \
    X(sladd) X(slsub) X(slsubf) \
    X(deps) X(depc) X(depa) \
    X(muladd) X(mulsub) X(mulsubf) X(mbsel) \
    X(dep16) X(srpi16) X(perm) \
    X(sprsr) X(callmi) \
    X(fmergess) X(fmaddss) X(fmsubss) X(fnmaddss) X(fnmsubss) \
    X(fmergesd) X(fmaddsd) X(fmsubsd) X(fnmaddsd) X(fnmsubsd) \
    X(fmergesq) X(fmaddsq) X(fmsubsq) X(fnmaddsq) X(fnmsubsq) \
    X(fmergeps) X(fmaddps) X(fmsubps) X(fnmaddps) X(fnmsubps) X(fmaddaps) X(fmsubaps) \
    X(fmergepd) X(fmaddpd) X(fmsubpd) X(fnmaddpd) X(fnmsubpd) X(fmaddapd) X(fmsubapd) \
    X(ld1mia) X(lds1mia) X(st1mia) \
    X(ld2mia) X(lds2mia) X(st2mia) \
    X(ld4mia) X(lds4mia) X(st4mia) \
    X(ld8mia) X(lds8mia) X(st8mia) \
    X(ld16mia) X(st16mia) \
    X(ld1mib) X(lds1mib) X(st1mib) \
    X(ld2mib) X(lds2mib) X(st2mib) \
    X(ld4mib) X(lds4mib) X(st4mib) \
    X(ld8mib) X(lds8mib) X(st8mib) \
    X(ld16mib) X(st16mib) \
    X(cpuid) X(int) X(syscall) X(aeskeygen)

enum EFusedOpx {
  DECLARE_FUSED_EXTENDED_OPCODES(OPCODE_X)
  fused_opx_last
};

/**************************************************************************************
* opcode_fused extended opcodes (6 bits)
**************************************************************************************/
#define DECLARE_RAOPX_EXTENDED_OPCODES(X) \
    X(undef) X(nop) X(br) X(enter) \
    X(ehadj) X(ehthrow) \
    X(dcbt) X(dcbf) X(dcbi) X(icbi) \
    X(ptc) \

enum ERaopxOpx {
  DECLARE_RAOPX_EXTENDED_OPCODES(OPCODE_X)
  raopx_opx_last
};

/**************************************************************************************
* opcode_nullify extended opcodes (4 bits)
**************************************************************************************/
#define DECLARE_NULLIFY_EXTENDED_OPCODES(X) \
    X(cneq) X(cnne) X(cnlt) X(cnltu) X(cngt) X(cngtu) \
    X(cneqi) X(cnnei) X(cnlti) X(cnltui) X(cngti) X(cngtui) \
    X(cnbs) X(cnbsi) X(cnbc) X(cnbci) \

enum ENullifyOpx {
  DECLARE_NULLIFY_EXTENDED_OPCODES(OPCODE_X)
  nullify_opx_last
};

/**************************************************************************************
* opcode_misc extended opcodes (10 bits)
**************************************************************************************/
#define DECLARE_MISC_EXTENDED_OPCODES(X) \
    X(mov) \
    X(cmpeq) X(cmpne) X(cmplt) X(cmpltu) X(cmple) X(cmpleu) \
    X(and) X(or) X(xor) X(andn) X(nand) X(nor) X(xnor) X(orn) \
    X(add) X(sub) X(addo) X(subo) \
    X(add4) X(sub4) \
    X(addc) X(subb) \
    X(mul) X(mulh) \
    X(div) X(divu) X(mod) X(modu) \
    X(maxs) X(maxu) X(mins) X(minu) \
    X(sll) X(slli) \
    X(sra) X(srai) \
    X(srl) X(srli) \
    X(srd) X(srdi) \
    X(bri) X(callri) X(ret) X(leave) \
    X(mbgath) X(mbscat) \
    X(xchg) X(absd) X(absd4) \
    X(cntpop) X(cntlz) X(cnttz) X(permb) X(gtb) \
    X(andneg) X(xordec) X(anddec) \
    X(tabs) X(tabc) \
    X(ld1mxa) X(lds1mxa) X(st1mxa) \
    X(ld2mxa) X(lds2mxa) X(st2mxa) \
    X(ld4mxa) X(lds4mxa) X(st4mxa) \
    X(ld8mxa) X(lds8mxa) X(st8mxa) \
    X(ld16mxa) X(st16mxa) \
    X(tb1) X(tb2) X(tb4) \
    X(mfspr) X(mtspr) \
    X(mtdbr) X(mfdbr) X(mtibr) X(mfibr) \
    X(mtmr) X(mfmr) \
    X(mtitr) X(mtdtr) \
    X(msync) X(mprobe) \
    X(rfi) X(halt) X(tpa) \
    X(sysret) \
    X(rscover) X(rsflush) X(rsload) \
    X(ld1a) X(ld2a) X(ld4a) X(ld8a) X(ld16a) \
    X(st1a) X(st2a) X(st4a) X(st8a) X(st16a) \
    X(cas4) X(cas8) X(cas16) \
    X(amoswap4) X(amoswap8) \
    X(amoadd4) X(amoadd8) \
    X(amoand4) X(amoand8) \
    X(amoor4) X(amoor8) \
    X(amoxor4) X(amoxor8) \
    X(amomin4) X(amomin8) \
    X(amomax4) X(amomax8) \
    X(amominu4) X(amominu8) \
    X(amomaxu4) X(amomaxu8) \
    X(fminsq) X(fmaxsq) X(faminsq) X(famaxsq) \
    X(fsqrtsq) \
    X(fsqrtsd) X(fsqrtpd) \
    X(fsqrtss) X(fsqrtps) \
    X(frsqrtsq) \
    X(frsqrtpd) X(frsqrtsd) \
    X(frsqrtps) X(frsqrtss) \
    X(fscale) \
    X(fcmpsq) X(fcmpsd) X(fcmpss) X(fcmpps) X(fcmppd) \
    X(frndsq) X(frndss) X(frndps) X(frndsd) X(frndpd) \
    X(faddsq) X(faddsd) X(faddpd) X(faddss) X(faddps) \
    X(fsubsq) X(fsubsd) X(fsubpd) X(fsubss) X(fsubps) \
    X(fmulsq) X(fmulsd) X(fmulpd) X(fmulss) X(fmulps) \
    X(fdivsq) X(fdivsd) X(fdivpd) X(fdivss) X(fdivps) \
    X(fnaddsq) X(fnaddsd) X(fnaddpd) X(fnaddss) X(fnaddps) \
    X(fnmulsq) X(fnmulsd) X(fnmulpd) X(fnmulss) X(fnmulps) \
    X(fminsd) X(fmaxsd) X(faminsd) X(famaxsd) \
    X(fminss) X(fmaxss) X(faminss) X(famaxss) \
    X(fminpd) X(fmaxpd) X(faminpd) X(famaxpd) \
    X(fminps) X(fmaxps) X(faminps) X(famaxps) \
    X(fnegsq) X(fnegsd) X(fnegpd) X(fnegss) X(fnegps) \
    X(fabssq) X(fabssd) X(fabsss) \
    X(fnabssq) X(fnabssd) X(fnabsss) \
    X(fabsdsq) X(fabsdsd) X(fabsdss) \
    X(fnabsdsq) X(fnabsdsd) X(fnabsdss) \
    X(fabsdpd) X(fnabsdpd) \
    X(faddcpd) X(fsubcpd) \
    X(faddhpd) X(fsubhpd) X(fmulhpd) \
    X(faddcps) X(fsubcps) \
    X(faddhps) X(fsubhps) X(fmulhps) \
    X(fdotpd) \
    X(fmrglpd) X(fmrghpd) \
    X(funpkhpd) \
    X(funpklpd) X(fpkpd) \
    X(fabsdps) X(fnabsdps) \
    X(fdotps) \
    X(fmrglps) X(fmrghps) \
    X(funpkhps) X(funpklps) \
    X(fpkps) \
    X(cvtss2sd) X(cvtss2sq) \
    X(cvtsd2ss) X(cvtsd2sq) \
    X(cvtsq2sd) X(cvtsq2ss) \
    X(cvtpd2i) X(cvtpd2u) X(cvti2pd) X(cvtu2pd) \
    X(cvtps2i) X(cvtps2u) X(cvti2ps) X(cvtu2ps) \
    X(cvtsd2i) X(cvtsd2u) X(cvti2sd) X(cvtu2sd) \
    X(cvtss2i) X(cvtss2u) X(cvti2ss) X(cvtu2ss) \
    X(cvtsq2i) X(cvtsq2u) X(cvti2sq) X(cvtu2sq) \
    X(vmaxs1) X(vmaxs2) X(vmaxs4) X(vmaxs8) \
    X(vmaxu1) X(vmaxu2) X(vmaxu4) X(vmaxu8) \
    X(vmins1) X(vmins2) X(vmins4) X(vmins8) \
    X(vminu1) X(vminu2) X(vminu4) X(vminu8) \
    X(vaddu1) X(vaddu2) X(vaddu4) X(vaddu8) \
    X(vsubu1) X(vsubu2) X(vsubu4) X(vsubu8) \
    X(vaddo1) X(vaddo2) X(vaddo4) X(vaddo8) \
    X(vsubo1) X(vsubo2) X(vsubo4) X(vsubo8) \
    X(vaddc1) X(vaddc2) X(vaddc4) X(vaddc8) \
    X(vsubb1) X(vsubb2) X(vsubb4) X(vsubb8) \
    X(vaddus1) X(vaddus2) X(vaddus4) X(vaddus8) \
    X(vaddss1) X(vaddss2) X(vaddss4) X(vaddss8) \
    X(vsubss1) X(vsubss2) X(vsubss4) X(vsubss8) \
    X(vsubus1) X(vsubus2) X(vsubus4) X(vsubus8) \
    X(vavgs1) X(vavgs2) X(vavgs4) X(vavgs8) \
    X(vavgu1) X(vavgu2) X(vavgu4) X(vavgu8) \
    X(vcmpeq1) X(vcmpeq2) X(vcmpeq4) X(vcmpeq8) \
    X(vcmplt1) X(vcmplt2) X(vcmplt4) X(vcmplt8) \
    X(vcmpltu1) X(vcmpltu2) X(vcmpltu4) X(vcmpltu8) \
    X(vsll1) X(vsll2) X(vsll4) X(vsll8) \
    X(vslli1) X(vslli2) X(vslli4) X(vslli8) \
    X(vsrl1) X(vsrl2) X(vsrl4) X(vsrl8) \
    X(vsrli1) X(vsrli2) X(vsrli4) X(vsrli8) \
    X(vsra1) X(vsra2) X(vsra4) X(vsra8) \
    X(vsrai1) X(vsrai2) X(vsrai4) X(vsrai8) \
    X(vrol1) X(vrol2) X(vrol4) X(vrol8) \
    X(vror1) X(vror2) X(vror4) X(vror8) \
    X(vmrgh1) X(vmrgh2) X(vmrgh4) X(vmrgh8) \
    X(vmrgl1) X(vmrgl2) X(vmrgl4) X(vmrgl8) \
    X(vpksss2) X(vpksss4) X(vpksss8) \
    X(vpksus2) X(vpksus4) X(vpksus8) \
    X(vpkuum2) X(vpkuum4) X(vpkuum8) \
    X(vpkuus2) X(vpkuus4) X(vpkuus8) \
    X(vupkls1) X(vupkls2) X(vupkls4) \
    X(vupkhs1) X(vupkhs2) X(vupkhs4) \
    X(clmul) \
    X(aesenc) X(aesenclast) X(aesdec) X(aesdeclast) X(aesimc) \
    X(rdrand) X(rdseed) \

enum EMiscOpx {
  DECLARE_MISC_EXTENDED_OPCODES(OPCODE_X)
  misc_opx_last
};

/***************************************************************************
* instruction packers for various opcodes
***************************************************************************/
inline constexpr uint64_t makepart(uint64_t item, unsigned int base)
{
    return item << base;
}

// primary opcodes
inline constexpr uint64_t insn_tmplt(const EInstructionOpcode opc)
{
    return makepart(static_cast<uint64_t>(opc), disp_opcode);
}

// extended opcodes
inline constexpr uint64_t insn_tmplt(const EMiscOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_misc)
         | makepart(static_cast<uint64_t>(opx), disp_opx);
}

inline constexpr uint64_t insn_tmplt(const EFusedOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_fused)
         | makepart(static_cast<uint64_t>(opx), disp_opx);
}

inline constexpr uint64_t insn_tmplt(const ERaopxOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_raopx)
         | makepart(static_cast<uint64_t>(opx), disp_reg_a);
}

inline constexpr uint64_t insn_tmplt(const ENullifyOpx opx)
{
    return insn_tmplt(EInstructionOpcode::opcode_nullify)
         | makepart(static_cast<uint64_t>(opx), disp_opx);
}

#define INSN_TMPLT(opc)    insn_tmplt( opcode_##opc )

enum EMemoryOrdering {
    order_relaxed = 0,
    order_acquire = 1,
    order_release = 2,
    order_acquire_release = order_acquire | order_release
};

} // namespace postrisc
