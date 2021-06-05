
#include "core.hpp"

namespace postrisc {

template<class CMD>
CStatus
Core::unimplemented(const CMD cmd)
{
    LOG_ERROR(LOG_EXCEPTION) << "not yet implemented opx " << cmd;
    return CStatus(CStatus::illegal_instruction);
}

DEFINE_INSN_IMPLEMENTATION(nop)
{
    UNREFERENCED_PARAMETER(cmd);
    LOG_TRACE(LOG_EMULATOR) << "nop";

    performance_monitor.report_event(PMC_NOP_INSTRUCTION);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mov)
{
    GET_WRA;
    GET_RRB;

    a = b;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(xchg)
{
    GET_WRA;
    GET_WRB;

    std::swap(a, b);
    return FinalizeInstruction();
}

// deposits
DEFINE_INSN_IMPLEMENTATION(deps)
{
    GET_WRA;
    GET_RRB;

    const unsigned offset   = cmd.get_shamt_c();
    const unsigned num_ones = cmd.get_shamt_d();

    const uint64_t mask = rol64(util::makemask(num_ones), offset);

    a = b.base() | mask;

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(depc)
{
    GET_WRA;
    GET_RRB;

    const unsigned offset   = cmd.get_shamt_c();
    const unsigned num_ones = cmd.get_shamt_d();

    const uint64_t mask = rol64(util::makemask(num_ones), offset);

    a = b.base() & ~mask;

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(depa)
{
    GET_WRA;
    GET_RRB;

    const unsigned offset   = cmd.get_shamt_c();
    const unsigned num_ones = cmd.get_shamt_d();

    const uint64_t mask = rol64(util::makemask(num_ones), offset);

    a = b.base() ^ mask;

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(dep)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const unsigned offset   = cmd.get_shamt_d();
    const unsigned num_ones = cmd.get_shamt_e();

    const uint64_t mask = rol64(util::makemask(num_ones), offset);
    a = bitwise_select::func<uint64_t>(rol64(c, offset), b, mask);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(rlmi)
{
    GET_WRA;
    GET_RRB;

    const unsigned offset   = cmd.get_shamt_d();
    const unsigned num_ones = cmd.get_shamt_e();

    const uint64_t mask = rol64(util::makemask(num_ones), offset);

    a = rol64(b, cmd.get_shamt_c()) & mask;

    return FinalizeInstruction();
}

// fused shifts
DEFINE_INSN_IMPLEMENTATION(slsrai)
{
    GET_WRA;
    GET_RRB;

    const unsigned left_shift  = cmd.get_shamt_c();
    const unsigned right_shift = cmd.get_shamt_d();

    a = sar64(shl64(b, left_shift), right_shift);

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(slsrli)
{
    GET_WRA;
    GET_RRB;

    const unsigned left_shift  = cmd.get_shamt_c();
    const unsigned right_shift = cmd.get_shamt_d();

    a = shr64(shl64(b, left_shift), right_shift);

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(slsra)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    const unsigned left_shift  = get_shift_amount(c);
    const unsigned right_shift = get_shift_amount(d);

    a = sar64(shl64(b, left_shift), right_shift);

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(slsrl)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    const unsigned left_shift  = get_shift_amount(c);
    const unsigned right_shift = get_shift_amount(d);

    a = shr64(shl64(b, left_shift), right_shift);

    return FinalizeInstruction();
}

//****************************************************************************************
// fused shifted-(add|subtract[from])
//****************************************************************************************
template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::fused_reg_reg_shamt(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const T value = OP::func( static_cast<T>(b), static_cast<T>(c), cmd.get_shamt_d() );
    a = value;
    return FinalizeInstruction();
}

// shift left and (add|subtract) [reverse]
DEFINE_INSN_IMPLEMENTATION(sladd)  { return fused_reg_reg_shamt < uint64_t, shifted_add          > (cmd); }
DEFINE_INSN_IMPLEMENTATION(slsub)  { return fused_reg_reg_shamt < uint64_t, shifted_subtract     > (cmd); }
DEFINE_INSN_IMPLEMENTATION(slsubf) { return fused_reg_reg_shamt < uint64_t, shifted_subtract_rev > (cmd); }
// fused shift right pair
DEFINE_INSN_IMPLEMENTATION(srpi)   { return fused_reg_reg_shamt < uint64_t, shift_right_pair_imm > (cmd); }

//****************************************************************************************
// fused (multiply|add)-(add|subtract[from])
//****************************************************************************************
template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::fused_reg_reg_reg(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    const T value = OP::func( static_cast<T>(b), static_cast<T>(c), static_cast<T>(d) );
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(muladd)  {  return fused_reg_reg_reg <uint64_t, muladd           > (cmd); }
DEFINE_INSN_IMPLEMENTATION(mulsub)  {  return fused_reg_reg_reg <uint64_t, mulsub           > (cmd); }
DEFINE_INSN_IMPLEMENTATION(mulsubf) {  return fused_reg_reg_reg <uint64_t, mulsub_rev       > (cmd); }
DEFINE_INSN_IMPLEMENTATION(addadd)  {  return fused_reg_reg_reg <uint64_t, addadd           > (cmd); }
DEFINE_INSN_IMPLEMENTATION(addsub)  {  return fused_reg_reg_reg <uint64_t, addsub           > (cmd); }
DEFINE_INSN_IMPLEMENTATION(subsub)  {  return fused_reg_reg_reg <uint64_t, subsub           > (cmd); }
// select
DEFINE_INSN_IMPLEMENTATION(mbsel)   {  return fused_reg_reg_reg <u128, bitwise_select  > (cmd); }
// funnel shifts
DEFINE_INSN_IMPLEMENTATION(srp)     {  return fused_reg_reg_reg <uint64_t, shift_right_pair > (cmd); }
DEFINE_INSN_IMPLEMENTATION(slp)     {  return fused_reg_reg_reg <uint64_t, shift_left_pair  > (cmd); }


//****************************************************************************************
// multiprecision arithmetic
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION(addc)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const uint64_t vb = b;
    const uint64_t vc = c;

    a = u128(add_carry::func(vb, vc), vb+vc);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(addaddc)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    const uint64_t vb = b;
    const uint64_t vc = c;
    const uint64_t vd = d.high();

    a = u128(add_carry::func(vb, vc, vd), vb+vc+vd);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(subb)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const uint64_t vb = b;
    const uint64_t vc = c;

    a = u128(subtract_borrow::func(vb, vc), vb+vc);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(subsubb)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    const uint64_t vb = b;
    const uint64_t vc = c;
    const uint64_t vd = d.high();

    a = u128(subtract_borrow::func(vb, vc,vd), vb-vc-vd);
    return FinalizeInstruction();
}

//****************************************************************************************
// binary reg-reg
//****************************************************************************************
template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::binary_reg_reg(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const T value = OP::func( static_cast<T>(b), static_cast<T>(c) );
    a = value;
    return FinalizeInstruction();
}

// arithmetic
DEFINE_INSN_IMPLEMENTATION(add)  { return binary_reg_reg < uint64_t, add                > (cmd); }
DEFINE_INSN_IMPLEMENTATION(add4) { return binary_reg_reg <  int32_t, add                > (cmd); }
DEFINE_INSN_IMPLEMENTATION(sub)  { return binary_reg_reg < uint64_t, subtract           > (cmd); }
DEFINE_INSN_IMPLEMENTATION(sub4) { return binary_reg_reg <  int32_t, subtract           > (cmd); }
DEFINE_INSN_IMPLEMENTATION(mul)  { return binary_reg_reg < uint64_t, mul                > (cmd); }
DEFINE_INSN_IMPLEMENTATION(mulh) { return binary_reg_reg < uint64_t, multiply_high      > (cmd); }
DEFINE_INSN_IMPLEMENTATION(addo) { return binary_reg_reg < uint64_t, add_overflow       > (cmd); }
DEFINE_INSN_IMPLEMENTATION(subo) { return binary_reg_reg < uint64_t, subtract_overflow  > (cmd); }
DEFINE_INSN_IMPLEMENTATION(absd) { return binary_reg_reg <  int64_t, absolute_difference> (cmd); }
DEFINE_INSN_IMPLEMENTATION(absd4){ return binary_reg_reg <  int32_t, absolute_difference> (cmd); }

// bitwise
DEFINE_INSN_IMPLEMENTATION(and)  { return binary_reg_reg <u128, bitwise_and > (cmd); }
DEFINE_INSN_IMPLEMENTATION(or)   { return binary_reg_reg <u128, bitwise_or  > (cmd); }
DEFINE_INSN_IMPLEMENTATION(xor)  { return binary_reg_reg <u128, bitwise_xor > (cmd); }
DEFINE_INSN_IMPLEMENTATION(xnor) { return binary_reg_reg <u128, bitwise_xnor> (cmd); }
DEFINE_INSN_IMPLEMENTATION(nand) { return binary_reg_reg <u128, bitwise_nand> (cmd); }
DEFINE_INSN_IMPLEMENTATION(nor)  { return binary_reg_reg <u128, bitwise_nor > (cmd); }
DEFINE_INSN_IMPLEMENTATION(andn) { return binary_reg_reg <u128, bitwise_andn> (cmd); }
DEFINE_INSN_IMPLEMENTATION(orn)  { return binary_reg_reg <u128, bitwise_orn > (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::binary_reg_reg_fallible(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    T value;
    CHECK_STATUS(OP::func(static_cast<T>(b), static_cast<T>(c), value));
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(div)  { return binary_reg_reg_fallible < int64_t, div > (cmd); }
DEFINE_INSN_IMPLEMENTATION(divu) { return binary_reg_reg_fallible < uint64_t, div > (cmd); }
DEFINE_INSN_IMPLEMENTATION(mod)  { return binary_reg_reg_fallible < int64_t, mod > (cmd); }
DEFINE_INSN_IMPLEMENTATION(modu) { return binary_reg_reg_fallible < uint64_t, mod > (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::BinaryRegRegBool(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const bool value = OP::func( static_cast<T>(b), static_cast<T>(c) );
    a = value;
    return FinalizeInstruction();
}

// set on compare
DEFINE_INSN_IMPLEMENTATION(cmpeq) {    return BinaryRegRegBool <  int64_t, cmp_eq> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmpne) {    return BinaryRegRegBool <  int64_t, cmp_ne> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmplt) {    return BinaryRegRegBool <  int64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmple) {    return BinaryRegRegBool <  int64_t, cmp_le> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmpltu) {   return BinaryRegRegBool < uint64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmpleu) {   return BinaryRegRegBool < uint64_t, cmp_le> (cmd); }

DEFINE_INSN_IMPLEMENTATION(andneg) {   return binary_reg_reg < uint64_t, bitwise_andneg> (cmd); }
DEFINE_INSN_IMPLEMENTATION(xordec) {   return binary_reg_reg < uint64_t, bitwise_xordec> (cmd); }
DEFINE_INSN_IMPLEMENTATION(anddec) {   return binary_reg_reg < uint64_t, bitwise_anddec> (cmd); }

DEFINE_INSN_IMPLEMENTATION(maxs) {     return binary_reg_reg < int64_t, max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(maxu) {     return binary_reg_reg < uint64_t, max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(mins) {     return binary_reg_reg < int64_t, min> (cmd); }
DEFINE_INSN_IMPLEMENTATION(minu) {     return binary_reg_reg < uint64_t, min> (cmd); }

// shifts
DEFINE_INSN_IMPLEMENTATION(sll) {      return binary_reg_reg < uint64_t, shift_left > (cmd); }
DEFINE_INSN_IMPLEMENTATION(srl) {      return binary_reg_reg < uint64_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(sra) {      return binary_reg_reg < int64_t, shift_right> (cmd); }
DEFINE_INSN_IMPLEMENTATION(srd) {      return binary_reg_reg < int64_t, shift_right_div> (cmd); }

// like bmi2 pdep & pext
DEFINE_INSN_IMPLEMENTATION(mbscat) {   return binary_reg_reg < uint64_t, bit_scatter> (cmd); }
DEFINE_INSN_IMPLEMENTATION(mbgath) {   return binary_reg_reg < uint64_t, bit_gather> (cmd); }


//****************************************************************************************
// binary register-immediate
//****************************************************************************************
template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::BinaryRegSimm(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    const T value = OP::func( static_cast<T>(b), static_cast<T>(cmd.get_bin_simm()) );
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(addi) {     return BinaryRegSimm< int64_t, add         > (cmd); }
DEFINE_INSN_IMPLEMENTATION(addi4) {    return BinaryRegSimm< int32_t, add         > (cmd); }
DEFINE_INSN_IMPLEMENTATION(subfi) {    return BinaryRegSimm< int64_t, subtract_rev> (cmd); }
DEFINE_INSN_IMPLEMENTATION(subfi4) {   return BinaryRegSimm< int32_t, subtract_rev> (cmd); }
DEFINE_INSN_IMPLEMENTATION(muli) {     return BinaryRegSimm< int64_t, mul         > (cmd); }
DEFINE_INSN_IMPLEMENTATION(maxsi) {    return BinaryRegSimm< int64_t, max         > (cmd); }
DEFINE_INSN_IMPLEMENTATION(minsi) {    return BinaryRegSimm< int64_t, min         > (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::BinaryRegUimm(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    const T value = OP::func( static_cast<T>(b), static_cast<T>(cmd.get_bin_uimm()) );
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(maxui) {    return BinaryRegUimm< uint64_t, max> (cmd); }
DEFINE_INSN_IMPLEMENTATION(minui) {    return BinaryRegUimm< uint64_t, min> (cmd); }

// binary register-immediate logical
DEFINE_INSN_IMPLEMENTATION(andi) {     return BinaryRegSimm< int64_t, bitwise_and > (cmd); }
DEFINE_INSN_IMPLEMENTATION(ori) {      return BinaryRegSimm< int64_t, bitwise_or  > (cmd); }
DEFINE_INSN_IMPLEMENTATION(xori) {     return BinaryRegSimm< int64_t, bitwise_xor > (cmd); }
// DEFINE_INSN_IMPLEMENTATION(xnori) {    return BinaryRegSimm< int64_t, bitwise_xnor> (cmd); }
// DEFINE_INSN_IMPLEMENTATION(nandi) {    return BinaryRegSimm< int64_t, bitwise_nand> (cmd); }
// DEFINE_INSN_IMPLEMENTATION(nori) {     return BinaryRegSimm< int64_t, bitwise_nor > (cmd); }
DEFINE_INSN_IMPLEMENTATION(andni) {    return BinaryRegSimm< int64_t, bitwise_andn> (cmd); }
DEFINE_INSN_IMPLEMENTATION(orni) {     return BinaryRegSimm< int64_t, bitwise_orn > (cmd); }

DEFINE_INSN_IMPLEMENTATION(cmpeqi) {   return BinaryRegSimm< int64_t, cmp_eq> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmpnei) {   return BinaryRegSimm< int64_t, cmp_ne> (cmd); }

DEFINE_INSN_IMPLEMENTATION(cmplti) {   return BinaryRegSimm< int64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmpgti) {   return BinaryRegSimm< int64_t, cmp_gt> (cmd); }

DEFINE_INSN_IMPLEMENTATION(cmpltui) {  return BinaryRegUimm< uint64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(cmpgtui) {  return BinaryRegUimm< uint64_t, cmp_gt> (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::binary_reg_uimm_fallible(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    T value;
    CHECK_STATUS(OP::func(static_cast<T>(b), static_cast<T>(cmd.get_bin_uimm()), value));
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(divui) { return binary_reg_uimm_fallible< uint64_t, div> (cmd); }
DEFINE_INSN_IMPLEMENTATION(modui) { return binary_reg_uimm_fallible< uint64_t, mod> (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::binary_reg_simm_fallible(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    T value;
    CHECK_STATUS(OP::func(static_cast<T>(b), static_cast<T>(cmd.get_bin_simm()), value));
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(divi) { return binary_reg_simm_fallible< int64_t, div > (cmd); }
DEFINE_INSN_IMPLEMENTATION(modi) { return binary_reg_simm_fallible< int64_t, mod > (cmd); }

DEFINE_INSN_IMPLEMENTATION(alloc)
{
    const unsigned last_allowed_regnum = cmd.ra();

    LOG_TRACE(LOG_CALLSTACK)
        << "last_allowed_regnum=" << last_allowed_regnum;

    CHECK_STATUS(AllocateFrame(last_allowed_regnum));

    // non-restartable part
    // set new EIP
    uint64_t const address = cmd.ComputeJumpRelative(ip);
    exception_state.SetEIP(address);

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(enter)
{
    GET_WRB;
    // GET_RRB;

    // reg_sp/reg_fp should be static registers
    CGeneralRegister & sp = XW(reg_sp);
    CGeneralRegister & fp = XW(reg_fp);

    uint64_t const sum = cmd.get_bin_simm();

    b = fpu::u128(fp, sp);

    fp = sp;
    sp += sum;

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(bri)
{
    GET_RRB;
    GET_RRC;

    uint64_t const ea = b.base() + c.base();
    uint64_t const address = RoundToBundle(ea);

    LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
        << "bri: " << fmt::hex(address);

    return ExecuteJump(address);
}

DEFINE_INSN_IMPLEMENTATION(callr)
{
    GET_WRA;

    const uint64_t address = cmd.ComputeJumpRelative(ip);

    LOG_TRACE(LOG_CALLSTACK)
        << "call=" << fmt::hex(address);

    return CallSubroutine(cmd.ra(), a, address);
}

DEFINE_INSN_IMPLEMENTATION(callri)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    const uint64_t ea = b.base() + c.base();
    const uint64_t address = RoundToBundle(ea);

    LOG_TRACE(LOG_CALLSTACK)
        << "call=" << fmt::hex(address);

    return CallSubroutine(cmd.ra(), a, address);
}

DEFINE_INSN_IMPLEMENTATION(callmi)
{
    GET_WRA;
    GET_RRB;

    uint64_t loaded;
    const uint64_t ea = b.base() + sizeof(loaded) * cmd.GetUintStride();

    CHECK_STATUS_ADDRESS(ea, mmu_read_finalized(ea, sizeof(loaded), &loaded));

    const uint64_t address = RoundToBundle(loaded);

    LOG_TRACE(LOG_CALLSTACK)
        << "call=" << fmt::hex(address);

    return CallSubroutine(cmd.ra(), a, address);
}

DEFINE_INSN_IMPLEMENTATION(callplt)
{
    GET_WRA;
    const uint64_t ea = cmd.ComputeForwardRelative(ip);

    uint64_t loaded;
    CHECK_STATUS_ADDRESS(ea, mmu_read_finalized(ea, sizeof(loaded), &loaded));

    const uint64_t address = RoundToBundle(loaded);

    LOG_TRACE(LOG_CALLSTACK)
        << "call=" << fmt::hex(address);

    return CallSubroutine(cmd.ra(), a, address);
}

DEFINE_INSN_IMPLEMENTATION(ret)
{
    GET_RRB;
    // const unsigned disp = cmd.get_bin_uimm();

    return ReturnFromSubroutine(b, 0);
}

DEFINE_INSN_IMPLEMENTATION(leave)
{
    GET_RRB;
    const CGeneralRegister c = b;

    XW(reg_sp) = c.low();
    XW(reg_fp) = c.high();

    return FinalizeInstruction();
}

//****************************************************************************************
// future's nullifying
//****************************************************************************************
template<class CMD>
ALWAYS_INLINE
CStatus
Core::NullifyConditionally(const CMD cmd, bool condition)
{
    LOG_TRACE(LOG_PREDICATION)
        << "condition=" << condition
        << "psr=" << psr;

    unsigned const dist_yes = cmd.get_nullify_yes();
    unsigned const dist_not = cmd.get_nullify_not();

    // nullify first instruction-block
    // nullify second instruction-block, forward future mask over first block
    uint64_t const mask =
    condition ? util::makemask(dist_yes)
              : (util::makemask(dist_not) << dist_yes);
    
    psr.NullifyFuture(mask);

    LOG_TRACE(LOG_PREDICATION) << "psr=" << psr;
    return FinalizeInstruction();
}

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::NullifyConditionallyRegReg(const CMD cmd)
{
    GET_RRA;
    GET_RRB;

    const bool condition = OP::func(T(a), T(b));

    return NullifyConditionally(cmd, condition);
}

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::NullifyConditionallyRegSimm(const CMD cmd)
{
    GET_RRA;
    const T b = cmd.get_brc_simm();

    const bool condition = OP::func(T(a), b);

    return NullifyConditionally(cmd, condition);
}

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::NullifyConditionallyRegUimm(const CMD cmd)
{
    GET_RRA;
    const T b = cmd.get_brc_uimm();

    const bool condition = OP::func(T(a), b);
    return NullifyConditionally(cmd, condition);
}

DEFINE_INSN_IMPLEMENTATION(cneq)   { return NullifyConditionallyRegReg<uint64_t, cmp_eq>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cnne)   { return NullifyConditionallyRegReg<uint64_t, cmp_ne>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cnlt)   { return NullifyConditionallyRegReg< int64_t, cmp_lt>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cnltu)  { return NullifyConditionallyRegReg<uint64_t, cmp_lt>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cngt)   { return NullifyConditionallyRegReg< int64_t, cmp_gt>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cngtu)  { return NullifyConditionallyRegReg<uint64_t, cmp_gt>(cmd); }

DEFINE_INSN_IMPLEMENTATION(cneqi)  { return NullifyConditionallyRegSimm<uint64_t, cmp_eq>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cnnei)  { return NullifyConditionallyRegSimm<uint64_t, cmp_ne>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cnlti)  { return NullifyConditionallyRegSimm< int64_t, cmp_lt>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cnltui) { return NullifyConditionallyRegUimm<uint64_t, cmp_lt>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cngti)  { return NullifyConditionallyRegSimm< int64_t, cmp_gt>(cmd); }
DEFINE_INSN_IMPLEMENTATION(cngtui) { return NullifyConditionallyRegUimm<uint64_t, cmp_gt>(cmd); }

DEFINE_INSN_IMPLEMENTATION(cnbc)   { GET_RRA; GET_RRB; return NullifyConditionally(cmd, a.get_bit(b.base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION(cnbs)   { GET_RRA; GET_RRB; return NullifyConditionally(cmd, a.get_bit(b.base() & 127) != 0); }
DEFINE_INSN_IMPLEMENTATION(cnbci)  { GET_RRA;          return NullifyConditionally(cmd, a.get_bit(cmd.get_vshamt_b()) == 0); }
DEFINE_INSN_IMPLEMENTATION(cnbsi)  { GET_RRA;          return NullifyConditionally(cmd, a.get_bit(cmd.get_vshamt_b()) != 0); }

//****************************************************************************************
// load/store base+displacement
//****************************************************************************************
template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::read_base(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    const uint64_t address = b.base() + cmd.get_bin_simm();
    T value;
    CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(T), &value));
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld1) { return read_base<uint8_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld2) { return read_base<uint16_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld4) { return read_base<uint32_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld8) { return read_base<uint64_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld16) { return read_base<u128>(cmd); }

DEFINE_INSN_IMPLEMENTATION(lds1) { return read_base<int8_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(lds2) { return read_base<int16_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(lds4) { return read_base<int32_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(lds8) { return read_base<int64_t>(cmd); }

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::write_base(const CMD cmd)
{
    GET_RRA;
    GET_RRB;
    const uint64_t address = b.base() + cmd.get_bin_simm();
    const T value = static_cast<T>(a);
    CHECK_STATUS_ADDRESS(address, mmu_write(address, sizeof(T), &value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st1) { return write_base<uint8_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st2) { return write_base<uint16_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st4) { return write_base<uint32_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st8) { return write_base<uint64_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st16) { return write_base<u128>(cmd); }

//****************************************************************************************
// load/store ip-relative
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION(ldr)
{
    const uint64_t address = cmd.ComputeForwardRelative(ip);

    // write pseudo-instruction
    if (unlikely(cmd.ra() == 0)) {
        return OutputString(address);
    }

    GET_WRA;
    a = address;
    return FinalizeInstruction();
}

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::read_ip_relative(const CMD cmd)
{
    GET_WRA;
    const uint64_t address = cmd.ComputeForwardRelative(ip);
    T value;
    CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(T), &value));
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld1r) { return read_ip_relative<uint8_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld2r) { return read_ip_relative<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld4r) { return read_ip_relative<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld8r) { return read_ip_relative<uint64_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld16r) { return read_ip_relative<u128> (cmd); }

DEFINE_INSN_IMPLEMENTATION(lds1r) { return read_ip_relative<int8_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds2r) { return read_ip_relative<int16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds4r) { return read_ip_relative<int32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds8r) { return read_ip_relative<int64_t> (cmd); }

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::write_ip_relative(const CMD cmd)
{
    GET_RRA;
    const uint64_t address = cmd.ComputeForwardRelative(ip);
    const T value = static_cast<T>(a);
    CHECK_STATUS_ADDRESS(address, mmu_write(address, sizeof(T), &value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st1r) { return write_ip_relative<uint8_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st2r) { return write_ip_relative<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st4r) { return write_ip_relative<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st8r) { return write_ip_relative<uint64_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st16r) { return write_ip_relative<u128> (cmd); }

//****************************************************************************************
// load/store indexed (scaled)
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION(ldax)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    const uint64_t address = b.base() + ( c.base() << cmd.GetIndexedScale() ) + cmd.GetIndexedDisplacement();
    a = address;
    return FinalizeInstruction();
}

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::read_indexed(const CMD cmd)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    const uint64_t address = b.base() + ( c.base() << cmd.GetIndexedScale() ) + cmd.GetIndexedDisplacement();
    T value;
    CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(T), &value));
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld1x) { return read_indexed<uint8_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld2x) { return read_indexed<uint16_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld4x) { return read_indexed<uint32_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld8x) { return read_indexed<uint64_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(ld16x) { return read_indexed<u128>(cmd); }

DEFINE_INSN_IMPLEMENTATION(lds1x) { return read_indexed<int8_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(lds2x) { return read_indexed<int16_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(lds4x) { return read_indexed<int32_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(lds8x) { return read_indexed<int64_t>(cmd); }

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::write_indexed(const CMD cmd)
{
    GET_RRA;
    GET_RRB;
    GET_RRC;
    const uint64_t address = b.base() + ( c.base() << cmd.GetIndexedScale() ) + cmd.GetIndexedDisplacement();
    const T value = static_cast<T>(a);
    CHECK_STATUS_ADDRESS(address, mmu_write(address, sizeof(T), &value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st1x) { return write_indexed<uint8_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st2x) { return write_indexed<uint16_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st4x) { return write_indexed<uint32_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st8x) { return write_indexed<uint64_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(st16x) { return write_indexed<u128>(cmd); }

//****************************************************************************************
// base+disp load/store with immediate base update (after)
//****************************************************************************************
#define CHECK_BASE_UPDATE_INSN                                               \
    if (&a == &b) {                                                          \
        LOG_ERROR(LOG_EXCEPTION) << "target and base registers are same: "   \
            << cmd.ra() << " " << cmd.rb();                                  \
        return CStatus(CStatus::illegal_instruction);                        \
    }

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::LoadModifyImmediateAfter(const CMD cmd)
{
    GET_WRA;
    GET_WRB;

    CHECK_BASE_UPDATE_INSN

    const uint64_t address = b.base(); // + cmd.GetIndexedDisplacement();
    T value;
    CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(T), &value));
    b.base() += cmd.GetUpdateStride();
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld1mia) { return LoadModifyImmediateAfter<uint8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld2mia) { return LoadModifyImmediateAfter<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld4mia) { return LoadModifyImmediateAfter<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld8mia) { return LoadModifyImmediateAfter<uint64_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld16mia) { return LoadModifyImmediateAfter<u128 > (cmd); }

DEFINE_INSN_IMPLEMENTATION(lds1mia) { return LoadModifyImmediateAfter<int8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds2mia) { return LoadModifyImmediateAfter<int16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds4mia) { return LoadModifyImmediateAfter<int32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds8mia) { return LoadModifyImmediateAfter<int64_t> (cmd); }

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::StoreModifyImmediateAfter(const CMD cmd)
{
    GET_RRA;
    GET_WRB;
    const T value = static_cast<T>(a);
    const uint64_t address = b.base(); // + cmd.GetIndexedDisplacement();
    CHECK_STATUS_ADDRESS(address, mmu_write(address, sizeof(T), &value));
    b.base() += cmd.GetUpdateStride();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st1mia) { return StoreModifyImmediateAfter<uint8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(st2mia) { return StoreModifyImmediateAfter<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st4mia) { return StoreModifyImmediateAfter<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st8mia) { return StoreModifyImmediateAfter<uint64_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st16mia) { return StoreModifyImmediateAfter<u128 > (cmd); }

//****************************************************************************************
// base+disp load/store with immediate base update (before)
//****************************************************************************************
template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::LoadModifyImmediateBefore(const CMD cmd)
{
    GET_WRA;
    GET_WRB;

    CHECK_BASE_UPDATE_INSN

    const uint64_t address = b.base() + cmd.GetUpdateStride();
    T value;
    CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(T), &value));
    b.base() = address;
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld1mib) { return LoadModifyImmediateBefore<uint8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld2mib) { return LoadModifyImmediateBefore<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld4mib) { return LoadModifyImmediateBefore<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld8mib) { return LoadModifyImmediateBefore<uint64_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(ld16mib) { return LoadModifyImmediateBefore<u128 > (cmd); }

DEFINE_INSN_IMPLEMENTATION(lds1mib) { return LoadModifyImmediateBefore<int8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds2mib) { return LoadModifyImmediateBefore<int16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds4mib) { return LoadModifyImmediateBefore<int32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(lds8mib) { return LoadModifyImmediateBefore<int64_t> (cmd); }

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::StoreModifyImmediateBefore(const CMD cmd)
{
    GET_RRA;
    GET_WRB;
    const T value = static_cast<T>(a);
    const uint64_t address = b.base() + cmd.GetUpdateStride();
    CHECK_STATUS_ADDRESS(address, mmu_write(address, sizeof(T), &value));
    b.base() = address;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st1mib) { return StoreModifyImmediateBefore<uint8_t > (cmd); }
DEFINE_INSN_IMPLEMENTATION(st2mib) { return StoreModifyImmediateBefore<uint16_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st4mib) { return StoreModifyImmediateBefore<uint32_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st8mib) { return StoreModifyImmediateBefore<uint64_t> (cmd); }
DEFINE_INSN_IMPLEMENTATION(st16mib) { return StoreModifyImmediateBefore<u128 > (cmd); }

//****************************************************************************************
// base load/store with scaled index base update
//****************************************************************************************
template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::LoadModifyIndexedAfter(const CMD cmd)
{
    GET_WRA;
    GET_WRB;
    GET_RRC;
    const uint64_t index = c;
    T value;
    uint64_t const address = b.base();
    CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(T), &value));
    b += (index << cmd.GetIndexedScale());
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld1mxa)  { return LoadModifyIndexedAfter< uint8_t>   (cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld2mxa)  { return LoadModifyIndexedAfter< uint16_t>  (cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld4mxa)  { return LoadModifyIndexedAfter< uint32_t>  (cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld8mxa)  { return LoadModifyIndexedAfter< uint64_t>  (cmd);  }
DEFINE_INSN_IMPLEMENTATION(ld16mxa) { return LoadModifyIndexedAfter< u128> (cmd);  }

DEFINE_INSN_IMPLEMENTATION(lds1mxa) { return LoadModifyIndexedAfter< int8_t>  (cmd);  }
DEFINE_INSN_IMPLEMENTATION(lds2mxa) { return LoadModifyIndexedAfter< int16_t> (cmd);  }
DEFINE_INSN_IMPLEMENTATION(lds4mxa) { return LoadModifyIndexedAfter< int32_t> (cmd);  }
DEFINE_INSN_IMPLEMENTATION(lds8mxa) { return LoadModifyIndexedAfter< int64_t> (cmd);  }

template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::StoreModifyIndexedAfter(const CMD cmd)
{
    GET_RRA;
    GET_WRB;
    GET_RRC;

    const T value = static_cast<T>(a);
    const uint64_t index = c;
    uint64_t const address = b.base();
    CHECK_STATUS_ADDRESS(address, mmu_write(address, sizeof(T), &value));
    b += (index << cmd.GetIndexedScale());
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st1mxa)  { return StoreModifyIndexedAfter<uint8_t>   (cmd);  }
DEFINE_INSN_IMPLEMENTATION(st2mxa)  { return StoreModifyIndexedAfter<uint16_t>  (cmd);  }
DEFINE_INSN_IMPLEMENTATION(st4mxa)  { return StoreModifyIndexedAfter<uint32_t>  (cmd);  }
DEFINE_INSN_IMPLEMENTATION(st8mxa)  { return StoreModifyIndexedAfter<uint64_t>  (cmd);  }
DEFINE_INSN_IMPLEMENTATION(st16mxa) { return StoreModifyIndexedAfter<u128> (cmd);  }

//****************************************************************************************
// compare register-register and branch
//****************************************************************************************
template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::CompareRegRegAndBranch(const CMD cmd)
{
    GET_RRA;
    GET_RRB;
    bool const condition = OP::func( T(a), T(b) );

    if (condition) {
        const uint64_t address = cmd.ComputeBranchRelative(ip);
        LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);

        return ExecuteJump( address );
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(beq)  { return CompareRegRegAndBranch< u128, cmp_eq > (cmd); }
DEFINE_INSN_IMPLEMENTATION(bne)  { return CompareRegRegAndBranch< u128, cmp_ne > (cmd); }
DEFINE_INSN_IMPLEMENTATION(blt)  { return CompareRegRegAndBranch< int64_t, cmp_lt > (cmd); }
DEFINE_INSN_IMPLEMENTATION(ble)  { return CompareRegRegAndBranch< int64_t, cmp_le > (cmd); }
DEFINE_INSN_IMPLEMENTATION(bltu) { return CompareRegRegAndBranch< uint64_t, cmp_lt > (cmd); }
DEFINE_INSN_IMPLEMENTATION(bleu) { return CompareRegRegAndBranch< uint64_t, cmp_le > (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::CompareAndTrap(const CMD cmd)
{
    GET_RRA;
    GET_RRB;
    bool const condition = OP::func( T(a), T(b.base() + cmd.get_bin_simm()) );

    if (condition) {
        LOG_TRACE(LOG_EMULATOR | LOG_BRANCH) << "taken program trap";
        return CStatus(CStatus::program_trap);
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(teq)  { return CompareAndTrap< uint64_t, cmp_eq > (cmd); }
DEFINE_INSN_IMPLEMENTATION(tne)  { return CompareAndTrap< uint64_t, cmp_ne > (cmd); }
DEFINE_INSN_IMPLEMENTATION(tlt)  { return CompareAndTrap< int64_t,  cmp_lt > (cmd); }
DEFINE_INSN_IMPLEMENTATION(tltu) { return CompareAndTrap< uint64_t, cmp_lt > (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::CompareRegZeroAndBranch(const CMD cmd)
{
    GET_RRA;
    bool const condition = OP::func(T(a));

    if (condition) {
        uint64_t const address = cmd.ComputeJumpRelative(ip);
        LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);

        return ExecuteJump( address );
    }

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(beqz)  { return CompareRegZeroAndBranch<u128, cmp_eqz> (cmd); }
DEFINE_INSN_IMPLEMENTATION(bnez)  { return CompareRegZeroAndBranch<u128, cmp_nez> (cmd); }
DEFINE_INSN_IMPLEMENTATION(bltz)  { return CompareRegZeroAndBranch<int64_t, cmp_ltz> (cmd); }
DEFINE_INSN_IMPLEMENTATION(blez)  { return CompareRegZeroAndBranch<int64_t, cmp_lez> (cmd); }
DEFINE_INSN_IMPLEMENTATION(bgtz)  { return CompareRegZeroAndBranch<int64_t, cmp_gtz> (cmd); }
DEFINE_INSN_IMPLEMENTATION(bgez)  { return CompareRegZeroAndBranch<int64_t, cmp_gez> (cmd); }

//****************************************************************************************
// compare register-immediate and branch
//****************************************************************************************
template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::CompareRegSimmAndBranch(const CMD cmd)
{
    GET_RRA;
    bool const condition = OP::func(T(a), cmd.get_brc_simm());

    if (condition) {
        const uint64_t address = cmd.ComputeShortBranchRelative(ip);

        LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);

       return ExecuteJump(address);
    }

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(beqi) { return CompareRegSimmAndBranch< int64_t, cmp_eq> (cmd); }
DEFINE_INSN_IMPLEMENTATION(bnei) { return CompareRegSimmAndBranch< int64_t, cmp_ne> (cmd); }
DEFINE_INSN_IMPLEMENTATION(blti) { return CompareRegSimmAndBranch< int64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(bgti) { return CompareRegSimmAndBranch< int64_t, cmp_gt> (cmd); }

template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::CompareRegUimmAndBranch(const CMD cmd)
{
    GET_RRA;
    bool const condition = OP::func(T(a), cmd.get_brc_uimm());

    if (condition) {
        const uint64_t address = cmd.ComputeShortBranchRelative(ip);
        LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);

       return ExecuteJump(address);
    }

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(bltui) { return CompareRegUimmAndBranch<uint64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(bgtui) { return CompareRegUimmAndBranch<uint64_t, cmp_gt> (cmd); }

//****************************************************************************************
// compare-register-bit-and-branch instructions group
//****************************************************************************************
template<class CMD>
ALWAYS_INLINE
CStatus
Core::DoConditionalBranch(const CMD cmd, bool condition)
{
    if (condition) {
        const uint64_t address = cmd.ComputeBranchRelative(ip);
        LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
        return ExecuteJump(address);
    } else
        return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(bbc)  { GET_RRA; GET_RRB; return DoConditionalBranch(cmd, getbit(a, b.base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION(bbs)  { GET_RRA; GET_RRB; return DoConditionalBranch(cmd, getbit(a, b.base() & 127) != 0); }
DEFINE_INSN_IMPLEMENTATION(bbci) { GET_RRA;          return DoConditionalBranch(cmd, a.get_bit(cmd.get_vshamt_b()) == 0); }
DEFINE_INSN_IMPLEMENTATION(bbsi) { GET_RRA;          return DoConditionalBranch(cmd, a.get_bit(cmd.get_vshamt_b()) != 0); }

//****************************************************************************************
// add/subtract, compare register-register and branch
//****************************************************************************************
template<class T, class CMP, class CMD>
ALWAYS_INLINE
CStatus
Core::RepeatBranch(const CMD cmd)
{
    GET_WRA;
    GET_WRB;

    const T bb = b;
    const T aa = (a.base() += cmd.GetLoopStep());

    if (CMP::func(aa, bb)) {
        const uint64_t address = cmd.ComputeShortBranchRelative(ip);
        LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);

       return ExecuteJump(address);
    }

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(reple) { return RepeatBranch<int64_t, cmp_le> (cmd); }
DEFINE_INSN_IMPLEMENTATION(replt) { return RepeatBranch<int64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(repge) { return RepeatBranch<int64_t, cmp_ge> (cmd); }
DEFINE_INSN_IMPLEMENTATION(repgt) { return RepeatBranch<int64_t, cmp_gt> (cmd); }

DEFINE_INSN_IMPLEMENTATION(repleu) { return RepeatBranch<uint64_t, cmp_le> (cmd); }
DEFINE_INSN_IMPLEMENTATION(repltu) { return RepeatBranch<uint64_t, cmp_lt> (cmd); }
DEFINE_INSN_IMPLEMENTATION(repgeu) { return RepeatBranch<uint64_t, cmp_ge> (cmd); }
DEFINE_INSN_IMPLEMENTATION(repgtu) { return RepeatBranch<uint64_t, cmp_gt> (cmd); }

//****************************************************************************************
// branches, ip-relative
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION(ldi)
{
    GET_WRA;
    const uint64_t imm = cmd.get_ldi();
    a = imm;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldih)
{
    GET_WRA;
    const uint64_t imm = cmd.get_ldi();
    a.vector<uint64_t>(1) = imm;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldar)
{
    GET_WRA;
    a = cmd.ComputeJumpRelative(ip);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldan)
{
    GET_WRA;
    GET_RRB;

    const uint64_t base = b.base();
    const uint64_t address = cmd.GetLoadAddressNear(base);
    a = address;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(br)
{
    const uint64_t address = cmd.ComputeJumpRelative(ip);

    return ExecuteJump(address);
}

//****************************************************************************************
// table-based branches
//****************************************************************************************
template<class T, class CMD>
ALWAYS_INLINE
CStatus
Core::tabular_branch(const CMD cmd)
{
    GET_RRB;
    GET_RRC;
    uint64_t const address = b.base() + c.base() * sizeof(T);
    T index;
    CHECK_STATUS_ADDRESS(address, mmu_read(address, sizeof(T), &index));

    uint64_t const target_address = ip + sizeof(CBundle) * index;

    LOG_TRACE(LOG_EMULATOR | LOG_BRANCH)
        << "taken jump=" << fmt::hex(target_address);

    return ExecuteJump(target_address);
}

DEFINE_INSN_IMPLEMENTATION(tb1) { return tabular_branch<uint8_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(tb2) { return tabular_branch<uint16_t>(cmd); }
DEFINE_INSN_IMPLEMENTATION(tb4) { return tabular_branch<uint32_t>(cmd); }

//****************************************************************************************
// reg + shamt
//****************************************************************************************
template<class T, class OP, class CMD>
ALWAYS_INLINE
CStatus
Core::BinaryRegShamt(const CMD cmd)
{
    GET_WRA;
    GET_RRB;

    const T value = OP::func( static_cast<T>(b), cmd.get_shamt_c() );
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(cnttz)  { return BinaryRegShamt< uint64_t, trailing_zeros            > (cmd); }
DEFINE_INSN_IMPLEMENTATION(cntlz)  { return BinaryRegShamt< uint64_t, leading_zeros             > (cmd); }
DEFINE_INSN_IMPLEMENTATION(cntpop) { return BinaryRegShamt< uint64_t, population                > (cmd); }
DEFINE_INSN_IMPLEMENTATION(permb)  { return BinaryRegShamt< uint64_t, permute                   > (cmd); }
DEFINE_INSN_IMPLEMENTATION(slli)   { return BinaryRegShamt< uint64_t, shift_left_immediate      > (cmd); }
DEFINE_INSN_IMPLEMENTATION(srli)   { return BinaryRegShamt< uint64_t, shift_right_immediate     > (cmd); }
DEFINE_INSN_IMPLEMENTATION(srai)   { return BinaryRegShamt<  int64_t, shift_right_immediate     > (cmd); }
DEFINE_INSN_IMPLEMENTATION(srdi)   { return BinaryRegShamt<  int64_t, shift_right_div_immediate > (cmd); }

DEFINE_INSN_IMPLEMENTATION(gtb)
{
    GET_WRA;
    GET_RRB;

    a = util::gray2bin( b );
    return FinalizeInstruction();
}

// test all bits set/clear
DEFINE_INSN_IMPLEMENTATION(tabc)
{
    GET_WRA;
    GET_RRB;

    a = b.IsAllBitClear();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(tabs)
{
    GET_WRA;
    GET_RRB;

    a = b.IsAllBitSet();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(dep16)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a = deposit(b, c, cmd.get_vshamt_d());
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(srpi16)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;

    a = funnel_shift_right(b, c, cmd.get_vshamt_d());
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(perm)
{
    GET_WRA;
    GET_RRB;
    GET_RRC;
    GET_RRD;

    CGeneralRegister tmp;

    for (size_t i=0; i<b.size<uint8_t>(); i++) {
        const unsigned n = d.template vector<uint8_t>(i);
        const unsigned shift = n & (sizeof(tmp) - 1);
        if( (n & sizeof(tmp)) == 0)
            tmp.template vector<uint8_t>(i) = b.template vector<uint8_t>(shift);
        else
            tmp.template vector<uint8_t>(i) = c.template vector<uint8_t>(shift);
    }

    a = tmp;

    return FinalizeInstruction();
}

//****************************************************************************************
// all undefined instructions
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION(undef)
{
    LOG_ERROR(LOG_EXCEPTION) << "undefined instruction" << cmd;
    return CStatus(CStatus::illegal_instruction);
}

} // namespace postrisc
