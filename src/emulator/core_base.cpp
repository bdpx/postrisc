#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"

#include "arch/abi.hpp"

#include "core.hpp"
#include "utils.hpp"

namespace postrisc {

CStatus Core::unimplemented(void) const
{
    LOG_ERROR(LOG_EXCEPTION) << "not yet implemented opx";
    return CStatus::illegal_instruction;
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(nop)
{
    core.performance_monitor.report_event(PMC_NOP_INSTRUCTION);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(mov)
{
    CGeneralRegister const t = core.rb();
    core.ra() = t;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(mov2)
{
    CGeneralRegister const t1 = core.rc();
    CGeneralRegister const t2 = core.rd();

    core.ra() = t1;
    core.rb() = t2;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ca_n)
{
    u64 const address = core.rb().base() + (core.cmd.imm << (bits_per_binimm - 1));
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ca_nrc)
{
    u64 const address = core.ip + core.rb().base() + (core.cmd.imm << (bits_per_binimm - 1));
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ca_r)
{
    core.ra() = core.ip + sizeof(CBundle) * core.cmd.imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ca_rf)
{
    core.ra() = core.ip + core.cmd.imm;
    return core.FinalizeInstruction();
}

// deposits
DEFINE_INSN_IMPLEMENTATION_NOMEM(dep_s)
{
    const unsigned offset   = core.cmd.rc;
    const unsigned num_ones = core.cmd.rd;
    const u64 mask = rol<u64>(util::makemask(num_ones), offset);
    core.ra() = core.rb().base() | mask;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dep_c)
{
    const unsigned offset   = core.cmd.rc;
    const unsigned num_ones = core.cmd.rd;

    const u64 mask = rol<u64>(util::makemask(num_ones), offset);
    core.ra() = core.rb().base() & ~mask;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dep_a)
{
    const unsigned offset   = core.cmd.rc;
    const unsigned num_ones = core.cmd.rd;

    const u64 mask = rol<u64>(util::makemask(num_ones), offset);

    core.ra() = core.rb().base() ^ mask;

    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dep)
{
    const CGeneralRegister& b = core.rb();
    const CGeneralRegister& c = core.rc();

    const unsigned offset   = core.cmd.rd;
    const unsigned num_ones = core.cmd.re;

    const u64 mask = rol<u64>(util::makemask(num_ones), offset);
    core.ra() = bitwise_select::func<u64>(rol<u64>(c.scalar<u64>(), offset), b.scalar<u64>(), mask);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(rlmi)
{
    const unsigned offset   = core.cmd.rd;
    const unsigned num_ones = core.cmd.re;

    const u64 mask = rol<u64>(util::makemask(num_ones), offset);

    core.ra() = rol<u64>(core.rb().scalar<u64>(), core.cmd.rc) & mask;
    return core.FinalizeInstruction();
}

// fused shifts
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrai)
{
    const unsigned left_shift  = core.cmd.rc;
    const unsigned right_shift = core.cmd.rd;

    core.ra() = sra<i64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrli)
{
    const unsigned left_shift  = core.cmd.rc;
    const unsigned right_shift = core.cmd.rd;

    core.ra() = srl<u64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsra)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = sra<i64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrl)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = srl<u64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsra_w)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = sra<i32>(sll<u32>(core.rb().scalar<u32>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrl_w)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = srl<u32>(sll<u32>(core.rb().scalar<u32>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

//****************************************************************************************
// fused shifted-(add|subtract[from])
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::fused_reg_reg_shamt(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>(), cmd.rd );
    ra() = value;
    return FinalizeInstruction();
}

// shift left and (add|subtract[reverse]|or|xor)
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_add)     { return core.fused_reg_reg_shamt<u64, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_add_ws)  { return core.fused_reg_reg_shamt<i32, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_add_wz)  { return core.fused_reg_reg_shamt<u32, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_sub)     { return core.fused_reg_reg_shamt<u64, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_sub_ws)  { return core.fused_reg_reg_shamt<i32, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_sub_wz)  { return core.fused_reg_reg_shamt<u32, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_subf)    { return core.fused_reg_reg_shamt<u64, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_subf_ws) { return core.fused_reg_reg_shamt<i32, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_subf_wz) { return core.fused_reg_reg_shamt<u32, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_or)      { return core.fused_reg_reg_shamt<u64, shifted_or          >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_xor)     { return core.fused_reg_reg_shamt<u64, shifted_xor         >(); }

// fused shift right pair
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpi_w)  { return core.fused_reg_reg_shamt < u32, shift_right_pair_imm > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpi)   { return core.fused_reg_reg_shamt < u64, shift_right_pair_imm > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpi_q)  { return core.fused_reg_reg_shamt < u128,shift_right_pair_imm > (); }

//****************************************************************************************
// fused (multiply|add)-(add|subtract[from])
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::fused_reg_reg_reg(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>(), rd().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_add)  {  return core.fused_reg_reg_reg <u64, muladd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_sub)  {  return core.fused_reg_reg_reg <u64, mulsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_subf) {  return core.fused_reg_reg_reg <u64, mulsub_rev       > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_add)  {  return core.fused_reg_reg_reg <u64, addadd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_sub)  {  return core.fused_reg_reg_reg <u64, addsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_sub)  {  return core.fused_reg_reg_reg <u64, subsub           > (); }
// select
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbsel)   {  return core.fused_reg_reg_reg <u128, bitwise_select  > (); }
// funnel shifts
DEFINE_INSN_IMPLEMENTATION_NOMEM(srp_w)    {  return core.fused_reg_reg_reg <u32, shift_right_pair > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slp_w)    {  return core.fused_reg_reg_reg <u32, shift_left_pair  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srp)     {  return core.fused_reg_reg_reg <u64, shift_right_pair > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slp)     {  return core.fused_reg_reg_reg <u64, shift_left_pair  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srp_q)    {  return core.fused_reg_reg_reg <u128,shift_right_pair > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slp_q)    {  return core.fused_reg_reg_reg <u128,shift_left_pair  > (); }


//****************************************************************************************
// multiprecision arithmetic
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(addc)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();

    core.ra() = CGeneralRegister(add_carry::func(vb, vc), vb+vc);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(add_addc)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();
    const u64 vd = core.rd().high();

    core.ra() = CGeneralRegister(add_carry::func(vb, vc, vd), vb+vc+vd);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(subb)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();

    core.ra() = CGeneralRegister(subtract_borrow::func(vb, vc), vb + vc);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_subb)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();
    const u64 vd = core.rd().high();

    core.ra() = CGeneralRegister(subtract_borrow::func(vb, vc,vd), vb-vc-vd);
    return core.FinalizeInstruction();
}

//****************************************************************************************
// binary reg-reg
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::binary_reg_reg(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

// arithmetic
DEFINE_INSN_IMPLEMENTATION_NOMEM(add)    { return core.binary_reg_reg < u64, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_ws) { return core.binary_reg_reg < i32, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_wz) { return core.binary_reg_reg < u32, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_q)  { return core.binary_reg_reg <i128, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub)    { return core.binary_reg_reg < u64, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_ws) { return core.binary_reg_reg < i32, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_wz) { return core.binary_reg_reg < u32, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_q)  { return core.binary_reg_reg <i128, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul)    { return core.binary_reg_reg < u64, mul                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_ws) { return core.binary_reg_reg < i32, mul                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_wz) { return core.binary_reg_reg < u32, mul                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulh)   { return core.binary_reg_reg < u64, multiply_high      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addo)   { return core.binary_reg_reg < u64, add_overflow       > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subo)   { return core.binary_reg_reg < u64, subtract_overflow  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(absd)   { return core.binary_reg_reg < i64, absolute_difference> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(absd_w) { return core.binary_reg_reg < i32, absolute_difference> (); }

// bitwise
DEFINE_INSN_IMPLEMENTATION_NOMEM(and)  { return core.binary_reg_reg <u128, bitwise_and > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(or)   { return core.binary_reg_reg <u128, bitwise_or  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xor)  { return core.binary_reg_reg <u128, bitwise_xor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xnor) { return core.binary_reg_reg <u128, bitwise_xnor> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nand) { return core.binary_reg_reg <u128, bitwise_nand> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nor)  { return core.binary_reg_reg <u128, bitwise_nor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(andn) { return core.binary_reg_reg <u128, bitwise_andn> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(orn)  { return core.binary_reg_reg <u128, bitwise_orn > (); }

template<class T, class OP> ALWAYS_INLINE CStatus Core::binary_reg_reg_divmod(void)
{
    T const divisor = rc().scalar<T>();
    if (divisor == static_cast<T>(0)) {
        LOG_ERROR(LOG_EXCEPTION) << "divide by zero";
        return CStatus::event_divide_by_zero;
    }
    ra() = OP::func(rb().scalar<T>(), divisor);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(div)    { return core.binary_reg_reg_divmod < i64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divu)   { return core.binary_reg_reg_divmod < u64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mod)    { return core.binary_reg_reg_divmod < i64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(modu)   { return core.binary_reg_reg_divmod < u64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(div_w)  { return core.binary_reg_reg_divmod < i32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divu_w) { return core.binary_reg_reg_divmod < u32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mod_w)  { return core.binary_reg_reg_divmod < i32, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(modu_w) { return core.binary_reg_reg_divmod < u32, mod > (); }

template<class T, class OP> ALWAYS_INLINE CStatus Core::BinaryRegRegBool(void)
{
    const bool value = OP::func( rb().scalar<T>(), rc().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::BinaryRegSimm(void)
{
    const T value = OP::func( rb().scalar<T>(), static_cast<T>(cmd.imm) );
    ra() = value;
    return FinalizeInstruction();
}

// set on compare
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_eq_q)  { return core.BinaryRegRegBool < u128, cmp_eq> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ne_q)  { return core.BinaryRegRegBool < u128, cmp_ne> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmps_lt_q) { return core.BinaryRegRegBool < i128, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmps_le_q) { return core.BinaryRegRegBool < i128, cmp_le> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpu_lt_q) { return core.BinaryRegRegBool < u128, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpu_le_q) { return core.BinaryRegRegBool < u128, cmp_le> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(and_neg) {   return core.binary_reg_reg < u64, bitwise_andneg> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xor_dec) {   return core.binary_reg_reg < u64, bitwise_xordec> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(and_dec) {   return core.binary_reg_reg < u64, bitwise_anddec> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(maxs) {     return core.binary_reg_reg < i64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxu) {     return core.binary_reg_reg < u64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mins) {     return core.binary_reg_reg < i64, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minu) {     return core.binary_reg_reg < u64, min> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(maxs_w) {    return core.binary_reg_reg < i32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxu_w) {    return core.binary_reg_reg < u32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mins_w) {    return core.binary_reg_reg < i32, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minu_w) {    return core.binary_reg_reg < u32, min> (); }

// shifts
DEFINE_INSN_IMPLEMENTATION_NOMEM(sll) {      return core.binary_reg_reg < u64, shift_left > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srl) {      return core.binary_reg_reg < u64, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sra) {      return core.binary_reg_reg < i64, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srd) {      return core.binary_reg_reg < i64, shift_right_div> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(sll_w) {     return core.binary_reg_reg < u32, shift_left > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srl_w) {     return core.binary_reg_reg < u32, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sra_w) {     return core.binary_reg_reg < i32, shift_right> (); }

// like bmi2 pdep & pext
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbscat) {   return core.binary_reg_reg < u64, bit_scatter> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbgath) {   return core.binary_reg_reg < u64, bit_gather> (); }

//****************************************************************************************
// binary register-immediate
//****************************************************************************************

DEFINE_INSN_IMPLEMENTATION_NOMEM(addi   )  {  return core.BinaryRegSimm< i64, add         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addi_ws)  {  return core.BinaryRegSimm< i32, add         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addi_wz)  {  return core.BinaryRegSimm< u32, add         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subfi)    {  return core.BinaryRegSimm< i64, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subfi_ws) {  return core.BinaryRegSimm< i32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subfi_wz) {  return core.BinaryRegSimm< u32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(muli)     {  return core.BinaryRegSimm< i64, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(muli_ws)  {  return core.BinaryRegSimm< i32, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(muli_wz)  {  return core.BinaryRegSimm< u32, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsi)    {  return core.BinaryRegSimm< i64, max         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsi)    {  return core.BinaryRegSimm< i64, min         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxui)    {  return core.BinaryRegSimm< u64, max         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minui)    {  return core.BinaryRegSimm< u64, min         > (); }

// binary register-immediate logical
DEFINE_INSN_IMPLEMENTATION_NOMEM(andi) {     return core.BinaryRegSimm< i64, bitwise_and > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(ori) {      return core.BinaryRegSimm< i64, bitwise_or  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xori) {     return core.BinaryRegSimm< i64, bitwise_xor > (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(xnori) {    return core.BinaryRegSimm< i64, bitwise_xnor> (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(nandi) {    return core.BinaryRegSimm< i64, bitwise_nand> (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(nori) {     return core.BinaryRegSimm< i64, bitwise_nor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(andni) {    return core.BinaryRegSimm< i64, bitwise_andn> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(orni) {     return core.BinaryRegSimm< i64, bitwise_orn > (); }


template<class T, class OP> ALWAYS_INLINE CStatus Core::binary_reg_simm_divmod(void)
{
    T const divisor = static_cast<T>(cmd.imm);
    if (divisor == static_cast<T>(0)) {
        LOG_ERROR(LOG_EXCEPTION) << "divide by zero";
        return CStatus::event_divide_by_zero;
    }
    ra() = OP::func(rb().scalar<T>(), divisor);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(divi)  { return core.binary_reg_simm_divmod< i64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(modi)  { return core.binary_reg_simm_divmod< i64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divui) { return core.binary_reg_simm_divmod< u64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(modui) { return core.binary_reg_simm_divmod< u64, mod > (); }

DEFINE_INSN_IMPLEMENTATION(alloc)
{
    unsigned const framesize = core.cmd.rb;
    LOG_DEBUG(LOG_CALLSTACK) << "framesize=" << framesize;

    CHECK_STATUS(core.AllocateFrame(as, framesize));

    // non-restartable part
    // set new EIP
    u64 const address = core.exception_state.GetREIP();
    core.exception_state.SetEIP(address);

    return core.FinalizeInstruction();
}

// same as alloc, but push sp
DEFINE_INSN_IMPLEMENTATION(alloc_sp)
{
    unsigned const framesize = core.cmd.rb;
    LOG_DEBUG(LOG_CALLSTACK)
        << "framesize=" << framesize
        << " sp=" << fmt::hex(core.r_static_reg(reg_sp).base())
        << " new_sp=" << fmt::hex(core.r_static_reg(reg_sp).base() + core.cmd.imm);

    CGeneralRegister& sp = core.w_static_reg(reg_sp);
    if (sp.base() < (u64)core.cmd.imm || sp.base() - core.cmd.imm < core.bsp) {
        LOG_ERROR(LOG_CALLSTACK) << "bsp underflow: bsp=" << fmt::hex(core.bsp) << " sp=" << fmt::hex(sp.base());
        return CStatus::general_protection;
    }

    CHECK_STATUS(core.AllocateFrame(as, framesize));

    // stack grows down
    sp = sp.base() - core.cmd.imm;

    // non-restartable part
    // set new EIP
    u64 const address = core.exception_state.GetREIP();
    core.exception_state.SetEIP(address);

    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(jmp_r)
{
    const CGeneralRegister& b = core.rb();
    const CGeneralRegister& c = core.rc();

    u64 const ea = b.base() + (c.base() << core.cmd.scale);
    u64 const address = RoundToBundle(ea);

    LOG_DEBUG(LOG_BRANCH)
        // both base and index may be addresses
        << "jmpr: base=" << as.get_decoder().get_address_info(b.base())
        << " index=" << as.get_decoder().get_address_info(c.base())
        << " ea=" << fmt::hex(ea)
        << " address=" << fmt::hex(address);

    return core.ExecuteJump(as, address);
}

DEFINE_INSN_IMPLEMENTATION(jmp_mi)
{
    u64 const base = core.rb().base();
    const CVirtAddress ea(base + ((i64)(i32)core.rc().base() << core.cmd.scale));
    u32 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read_u32/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(base + (i32)loaded);
    LOG_DEBUG(LOG_BRANCH) << "jump=" << fmt::hex(address);
    return core.ExecuteJump(as, address);
}

DEFINE_INSN_IMPLEMENTATION(call_r)
{
    u64 const address = core.ip + sizeof(CBundle) * core.cmd.imm;
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(call_ri)
{
    u64 const ea = core.rb().base() + core.rc().base();
    u64 const address = RoundToBundle(ea);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(call_mi)
{
    const CVirtAddress ea(core.rb().base() + core.cmd.imm);
    u64 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read_u64/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(call_mrw)
{
    u64 const base = core.rb().base();
    const CVirtAddress ea(base + core.cmd.imm);
    u32 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read_u32/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(base + (i32)loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(call_rvt)
{
    const CVirtAddress ea(core.rb().base() + core.cmd.imm);
    i32 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read/*FIXME:_i32_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(core.rb().base() + loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(call_plt)
{
    const CVirtAddress ea(core.ip + core.cmd.imm);

    u64 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read_u64/*FIXME:_finalized*/(core, ea, loaded));

    u64 const address = RoundToBundle(loaded);
    LOG_DEBUG(LOG_CALLSTACK)
        << "call=" << fmt::hex(address);

    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(ret)
{
    unsigned temp, ri0_result;
    ROTATE_REG(core, ri0_result, temp, 0)
    const CGeneralRegister & link = core.rget( ri0_result );

    CStatus status = core.ReturnFromSubroutine(as, link);
    if (!status.success())
        return status;

#if defined(DEBUG_LINK_INFO_STACK)
    CHECK_STATUS(core.CheckStackOnReturn());
#endif
    return status;
}

DEFINE_INSN_IMPLEMENTATION(retf)
{
    unsigned ri_result, temp;
    ROTATE_REG(core, ri_result, temp, 0)
    const CGeneralRegister & link = core.rget(ri_result);

    CStatus status = core.ReturnFromSubroutine(as, link);
    if (!status.success())
        return status;

    CGeneralRegister & stack = core.w_static_reg(reg_sp);
    // stack grows back up
    stack = stack.base() + core.cmd.imm;
    LOG_DEBUG(LOG_CALLSTACK)
        << "retf sp=" << fmt::hex(stack.base());

#if defined(DEBUG_LINK_INFO_STACK)
    CHECK_STATUS(core.CheckStackOnReturn());
#endif
    return status;
}

//****************************************************************************************
// future's nullifying
//****************************************************************************************
CStatus Core::NullifyConditionally(bool condition)
{
    LOG_DEBUG(LOG_PREDICATION)
        << "condition=" << condition
        << "psr=" << psr;

    unsigned const dist_yes = cmd.dist_yes;
    unsigned const dist_not = cmd.dist_no;

    // 2 bits are reserved for call/interrupt shadows
    if (unlikely(dist_yes + dist_not > 32 - 2)) {
        LOG_ERROR(LOG_PREDICATION)
            << "summary future exceed limit:"
            << " dist_yes=" << dist_yes
            << " dist_not=" << dist_not;
        return CStatus::illegal_instruction;
    }

    // nullify first instruction-block
    // nullify second instruction-block, forward future mask over first block
    u64 const mask =
    condition ? (util::makemask(dist_yes)            )
              : (util::makemask(dist_not) << dist_yes);

    psr.NullifyFuture(mask);

    LOG_DEBUG(LOG_PREDICATION) << "psr=" << psr;
    return FinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::NullifyConditionallyRegReg(void)
{
    const bool condition = OP::func(ra().scalar<T>(), rb().scalar<T>());
    return NullifyConditionally(condition);
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::NullifyConditionallyRegSimm(void)
{
    const T b = cmd.brc_imm;
    const bool condition = OP::func(ra().scalar<T>(), b);
    return NullifyConditionally(condition);
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bc)   { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bs)   { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) != 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bci)  { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bsi)  { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) != 0); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nulm_all)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_all   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulm_any)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_any   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulm_none)   { return core.NullifyConditionallyRegSimm<u64, cmp_mask_none  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulm_notall) { return core.NullifyConditionallyRegSimm<u64, cmp_mask_notall> (); }

//****************************************************************************************
// load/store base+displacement
//****************************************************************************************
template<class T> ALWAYS_INLINE CStatus Core::read_base(IAddressSpace& as)
{
    const CGeneralRegister& b = rb();

    const CVirtAddress address(b.base() + cmd.imm);
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldz_b) { return core.read_base<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldz_h) { return core.read_base<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldz_w) { return core.read_base<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldz_d) { return core.read_base<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_q)  { return core.read_base<u128>(as); }

DEFINE_INSN_IMPLEMENTATION(lds_b) { return core.read_base<i8>(as); }
DEFINE_INSN_IMPLEMENTATION(lds_h) { return core.read_base<i16>(as); }
DEFINE_INSN_IMPLEMENTATION(lds_w) { return core.read_base<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(lds_d) { return core.read_base<i64>(as); }

template<class T> ALWAYS_INLINE CStatus Core::write_base(IAddressSpace& as)
{
    const CVirtAddress address(rb().base() + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_b) { return core.write_base<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(st_h) { return core.write_base<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(st_w) { return core.write_base<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_d) { return core.write_base<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_q) { return core.write_base<u128>(as); }

//****************************************************************************************
// load/store ip-relative
//****************************************************************************************
template<class T> ALWAYS_INLINE CStatus Core::read_ip_relative(IAddressSpace& as)
{
    const CVirtAddress address(ip + cmd.imm);
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldz_b_r) { return core.read_ip_relative<u8> (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_h_r) { return core.read_ip_relative<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_w_r) { return core.read_ip_relative<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_d_r) { return core.read_ip_relative<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_q_r)  { return core.read_ip_relative<u128> (as); }

DEFINE_INSN_IMPLEMENTATION(lds_b_r) { return core.read_ip_relative<i8> (as); }
DEFINE_INSN_IMPLEMENTATION(lds_h_r) { return core.read_ip_relative<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(lds_w_r) { return core.read_ip_relative<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(lds_d_r) { return core.read_ip_relative<i64> (as); }

template<class T> ALWAYS_INLINE CStatus Core::write_ip_relative(IAddressSpace& as)
{
    const CVirtAddress address(ip + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_b_r) { return core.write_ip_relative<u8> (as); }
DEFINE_INSN_IMPLEMENTATION(st_h_r) { return core.write_ip_relative<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(st_w_r) { return core.write_ip_relative<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(st_d_r) { return core.write_ip_relative<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(st_q_r) { return core.write_ip_relative<u128> (as); }

//****************************************************************************************
// load/store indexed (scaled)
//****************************************************************************************
template<class INDEX> ALWAYS_INLINE CStatus Core::compute_address_indexed(IAddressSpace&)
{
    u64 const address = ComputeScaledIndex<INDEX>();
    ra() = address;
    return FinalizeInstruction();
}

template<class T, class INDEX> ALWAYS_INLINE CStatus Core::read_indexed(IAddressSpace& as)
{
    const CVirtAddress address(ComputeScaledIndex<i64>());
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    ra() = value;
    return FinalizeInstruction();
}

template<class T, class INDEX> ALWAYS_INLINE CStatus Core::write_indexed(IAddressSpace& as)
{
    const CVirtAddress address(ComputeScaledIndex<i64>());
    T const value = ra().scalar<T>();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

#define INDEXED_INSNS(SUFFIX, TYPE)                                                             \
    DEFINE_INSN_IMPLEMENTATION(ldz_b ## SUFFIX) { return core.read_indexed<u8,    TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(ldz_h ## SUFFIX) { return core.read_indexed<u16,   TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(ldz_w ## SUFFIX) { return core.read_indexed<u32,   TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(ldz_d ## SUFFIX) { return core.read_indexed<u64,   TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(ld_q  ## SUFFIX) { return core.read_indexed<u128,  TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(lds_b ## SUFFIX) { return core.read_indexed<i8,    TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(lds_h ## SUFFIX) { return core.read_indexed<i16,   TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(lds_w ## SUFFIX) { return core.read_indexed<i32,   TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(lds_d ## SUFFIX) { return core.read_indexed<i64,   TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(st_b  ## SUFFIX) { return core.write_indexed<u8,   TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(st_h  ## SUFFIX) { return core.write_indexed<u16,  TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(st_w  ## SUFFIX) { return core.write_indexed<u32,  TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(st_d  ## SUFFIX) { return core.write_indexed<u64,  TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(st_q  ## SUFFIX) { return core.write_indexed<u128, TYPE>(as); } \
    DEFINE_INSN_IMPLEMENTATION(ca    ## SUFFIX) { return core.compute_address_indexed<TYPE>(as); }

INDEXED_INSNS(_xds, i64)
INDEXED_INSNS(_xws, i32)
INDEXED_INSNS(_xwz, u32)

//****************************************************************************************
// base+disp load/store with immediate base update (after)
//****************************************************************************************
#define CHECK_BASE_UPDATE_INSN                                               \
    if (&ra() == &rb()) {                                                    \
        LOG_ERROR(LOG_EXCEPTION) << "target and base registers are same";    \
        return CStatus::illegal_instruction;                                 \
    }

template<class T>
ALWAYS_INLINE
CStatus
Core::LoadModifyImmediateAfter(IAddressSpace& as)
{
    CHECK_BASE_UPDATE_INSN
    const CVirtAddress address(rb().base()); // + cmd.GetIndexedDisplacement();
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    rb().base() += cmd.imm;
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldz_b_mia) { return core.LoadModifyImmediateAfter<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_h_mia) { return core.LoadModifyImmediateAfter<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_w_mia) { return core.LoadModifyImmediateAfter<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_d_mia) { return core.LoadModifyImmediateAfter<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_q_mia)  { return core.LoadModifyImmediateAfter<u128>(as); }

DEFINE_INSN_IMPLEMENTATION(lds_b_mia) { return core.LoadModifyImmediateAfter<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(lds_h_mia) { return core.LoadModifyImmediateAfter<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(lds_w_mia) { return core.LoadModifyImmediateAfter<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(lds_d_mia) { return core.LoadModifyImmediateAfter<i64> (as); }

template<class T> ALWAYS_INLINE CStatus Core::StoreModifyImmediateAfter(IAddressSpace& as)
{
    const T value = ra().scalar<T>();
    const CVirtAddress address(rb().base()); // + cmd.GetIndexedDisplacement();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    rb().base() += cmd.imm;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_b_mia) { return core.StoreModifyImmediateAfter<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(st_h_mia) { return core.StoreModifyImmediateAfter<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(st_w_mia) { return core.StoreModifyImmediateAfter<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(st_d_mia) { return core.StoreModifyImmediateAfter<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(st_q_mia) { return core.StoreModifyImmediateAfter<u128>(as); }

//****************************************************************************************
// base+disp load/store with immediate base update (before)
//****************************************************************************************
template<class T> ALWAYS_INLINE CStatus Core::LoadModifyImmediateBefore(IAddressSpace& as)
{
    CHECK_BASE_UPDATE_INSN
    const CVirtAddress address(rb().base() + cmd.imm);
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    rb().base() = address.get_address();
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldz_b_mib) { return core.LoadModifyImmediateBefore<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_h_mib) { return core.LoadModifyImmediateBefore<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_w_mib) { return core.LoadModifyImmediateBefore<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(ldz_d_mib) { return core.LoadModifyImmediateBefore<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_q_mib)  { return core.LoadModifyImmediateBefore<u128> (as); }

DEFINE_INSN_IMPLEMENTATION(lds_b_mib) { return core.LoadModifyImmediateBefore<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(lds_h_mib) { return core.LoadModifyImmediateBefore<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(lds_w_mib) { return core.LoadModifyImmediateBefore<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(lds_d_mib) { return core.LoadModifyImmediateBefore<i64> (as); }

template<class T> ALWAYS_INLINE CStatus Core::StoreModifyImmediateBefore(IAddressSpace& as)
{
    const T value = ra().scalar<T>();
    const CVirtAddress address(rb().base() + cmd.imm);
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    rb().base() = address.get_address();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_b_mib) { return core.StoreModifyImmediateBefore<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(st_h_mib) { return core.StoreModifyImmediateBefore<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(st_w_mib) { return core.StoreModifyImmediateBefore<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(st_d_mib) { return core.StoreModifyImmediateBefore<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(st_q_mib) { return core.StoreModifyImmediateBefore<u128>(as); }

//****************************************************************************************
// compare register-register and branch
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::CompareRegRegAndBranch(IAddressSpace& as)
{
    bool const condition = OP::func( ra().scalar<T>(), rb().scalar<T>() );
    if (condition) {
        const u64 address = ip + sizeof(CBundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);

        return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::CompareAndTrap(void)
{
    bool const condition = OP::func( ra().scalar<T>(), T(rb().base() + cmd.imm) );
    if (condition) {
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH) << "taken program trap";
        return CStatus::program_trap;
    }
    return FinalizeInstruction();
}

//****************************************************************************************
// compare register-immediate and branch
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::CompareRegSimmAndBranch(IAddressSpace& as)
{
    bool const condition = OP::func(ra().scalar<T>(), static_cast<T>(cmd.brc_imm));
    if (condition) {
        const u64 address = ip + sizeof(CBundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(bm_all)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_all   > (as); }
DEFINE_INSN_IMPLEMENTATION(bm_any)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_any   > (as); }
DEFINE_INSN_IMPLEMENTATION(bm_none)   { return core.CompareRegSimmAndBranch<u64, cmp_mask_none  > (as); }
DEFINE_INSN_IMPLEMENTATION(bm_notall) { return core.CompareRegSimmAndBranch<u64, cmp_mask_notall> (as); }

//****************************************************************************************
// compare-register-bit-and-branch instructions group
//****************************************************************************************
ALWAYS_INLINE CStatus Core::DoConditionalBranch(IAddressSpace& as, bool condition)
{
    if (condition) {
        const u64 address = ip + sizeof(CBundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
        return ExecuteJump(as, address);
    } else {
        return FinalizeInstruction();
    }
}

DEFINE_INSN_IMPLEMENTATION(b_bc)  { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION(b_bs)  { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) != 0); }
DEFINE_INSN_IMPLEMENTATION(b_bci) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION(b_bsi) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) != 0); }

//****************************************************************************************
// add/subtract, compare register-register and branch
//****************************************************************************************
template<class T, class CMP, int step> ALWAYS_INLINE CStatus
Core::RepeatBranch(IAddressSpace& as)
{
    CGeneralRegister & a = ra();
    const T bb = rb().scalar<T>();
    const T aa = (a.base() += step);
    if (CMP::func(aa, bb)) {
        const u64 address = ip + sizeof(CBundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(reps_le_d) { return core.RepeatBranch<i64, cmp_le,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(reps_lt_d) { return core.RepeatBranch<i64, cmp_lt,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(reps_ge_d) { return core.RepeatBranch<i64, cmp_ge, -1> (as); }
DEFINE_INSN_IMPLEMENTATION(reps_gt_d) { return core.RepeatBranch<i64, cmp_gt, -1> (as); }

DEFINE_INSN_IMPLEMENTATION(repu_le_d) { return core.RepeatBranch<u64, cmp_le,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(repu_lt_d) { return core.RepeatBranch<u64, cmp_lt,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(repu_ge_d) { return core.RepeatBranch<u64, cmp_ge, -1> (as); }
DEFINE_INSN_IMPLEMENTATION(repu_gt_d) { return core.RepeatBranch<u64, cmp_gt, -1> (as); }

//****************************************************************************************
// branches, ip-relative
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(ldi)
{
    const u64 imm = core.cmd.imm;
    core.ra() = imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldih) // replace high part of 128 bit
{
    const u64 imm = core.cmd.imm;
    core.ra().vector<u64>(1) = imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(jmp)
{
    const u64 address = core.ip + sizeof(CBundle) * core.cmd.imm;
    return core.ExecuteJump(as, address);
}

DEFINE_INSN_IMPLEMENTATION(write)
{
    const u64 address = core.ip + core.cmd.imm;
    return core.OutputString(as, address);
}

//****************************************************************************************
// table-based branches
//****************************************************************************************
template<class T> ALWAYS_INLINE CStatus Core::tabular_branch(IAddressSpace& as)
{
    const CGeneralRegister& b = rb();
    const CGeneralRegister& c = rc();

    u64 const table_address = b.base();
    i64 const index = static_cast<i64>(c.scalar<T>());
    const CVirtAddress address(table_address + index * sizeof(i32));
    i32 offset;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, offset));

    u64 const target_address = table_address + static_cast<i64>(offset);

    LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
        << "taken jump=" << fmt::hex(target_address);

    return ExecuteJump(as, target_address);
}

DEFINE_INSN_IMPLEMENTATION(jmp_t)    { return core.tabular_branch<i64>(as); }
DEFINE_INSN_IMPLEMENTATION(jmp_t_ws) { return core.tabular_branch<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(jmp_t_wz) { return core.tabular_branch<u32>(as); }

//****************************************************************************************
// reg + shamt
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::BinaryRegShamt(void)
{
    const T value = OP::func(rb().scalar<T>(), static_cast<unsigned>(cmd.rc));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(cnt_tz)  { return core.BinaryRegShamt< u64, trailing_zeros            > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cnt_lz)  { return core.BinaryRegShamt< u64, leading_zeros             > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cnt_pop) { return core.BinaryRegShamt< u64, population                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(permb)  { return core.BinaryRegShamt< u64, permute                   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slli)   { return core.BinaryRegShamt< u64, shift_left_immediate      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srli)   { return core.BinaryRegShamt< u64, shift_right_immediate     > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srai)   { return core.BinaryRegShamt< i64, shift_right_immediate     > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srdi)   { return core.BinaryRegShamt< i64, shift_right_div_immediate > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(slli_w)  { return core.BinaryRegShamt< u32, shift_left_immediate      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srli_w)  { return core.BinaryRegShamt< u32, shift_right_immediate     > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srai_w)  { return core.BinaryRegShamt< i32, shift_right_immediate     > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(slli_q) { return core.BinaryRegShamt< u128, shift_left_immediate    > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srli_q) { return core.BinaryRegShamt< u128, shift_right_immediate   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srai_q) { return core.BinaryRegShamt< i128, shift_right_immediate   > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(gtb)
{
    core.ra() = util::gray2bin( core.rb().scalar<u64>() );
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dep_q)
{
    const CGeneralRegister& b = core.rb();
    const CGeneralRegister& c = core.rc();

    core.ra() = deposit(b, c, core.cmd.rd);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(perm)
{
    const CGeneralRegister& b = core.rb();
    const CGeneralRegister& c = core.rc();
    const CGeneralRegister& d = core.rd();

    CGeneralRegister tmp;

    for (size_t i = 0; i < b.size<u8>(); i++) {
        const unsigned n = d.template vector<u8>(i);
        const unsigned shift = n & (sizeof(tmp) - 1);
        if( (n & sizeof(tmp)) == 0)
            tmp.template vector<u8>(i) = b.template vector<u8>(shift);
        else
            tmp.template vector<u8>(i) = c.template vector<u8>(shift);
    }
    core.ra() = tmp;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(algnup)
{
    unsigned const shift = core.cmd.rc;
    unsigned const shamt = core.cmd.rd;

    u64 const mask = util::makemask(shamt);
    u64 const src = core.rb().base();
    u64 const dst = ((src << shift) + mask) & (~mask);

    LOG_DEBUG(LOG_EMULATOR)
        << "algnup: src=" << fmt::hex(src)
        << " dst=" << fmt::hex(dst);

    core.ra() = dst;
    return core.FinalizeInstruction();
}

//****************************************************************************************
// conditional moves
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_lsb)
{
    bool const condition = (core.rb().base() & 1);
    core.ra() = condition ? core.rc() : core.rd();
    return core.FinalizeInstruction();
}

template<class T, class OP> ALWAYS_INLINE CStatus Core::ConditionalMove(void)
{
    bool const condition = OP::func(rb().scalar<T>(), static_cast<T>(0));
    ra() = condition ? rc() : rd();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_eq_w) { return core.ConditionalMove<i32, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_lt_w) { return core.ConditionalMove<i32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_le_w) { return core.ConditionalMove<i32, cmp_le>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_eq_d) { return core.ConditionalMove<i64, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_lt_d) { return core.ConditionalMove<i64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_le_d) { return core.ConditionalMove<i64, cmp_le>(); }

// official undefined instruction
DEFINE_INSN_IMPLEMENTATION_NOMEM(undef)
{
    return core.unimplemented();
}

// all undefined instructions
DEFINE_INSN_IMPLEMENTATION_NOMEM(illegal)
{
    return core.unimplemented();
}

#define INT_BASE_SCALAR(NAME, STYPE, UTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_eq_   ## NAME) { return core.BinaryRegRegBool<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ne_   ## NAME) { return core.BinaryRegRegBool<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmps_lt_  ## NAME) { return core.BinaryRegRegBool<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmps_le_  ## NAME) { return core.BinaryRegRegBool<STYPE, cmp_le>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpu_lt_  ## NAME) { return core.BinaryRegRegBool<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpu_le_  ## NAME) { return core.BinaryRegRegBool<UTYPE, cmp_le>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpi_eq_  ## NAME) { return core.BinaryRegSimm<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpi_ne_  ## NAME) { return core.BinaryRegSimm<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpsi_lt_ ## NAME) { return core.BinaryRegSimm<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpsi_gt_ ## NAME) { return core.BinaryRegSimm<STYPE, cmp_gt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpui_lt_ ## NAME) { return core.BinaryRegSimm<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpui_gt_ ## NAME) { return core.BinaryRegSimm<UTYPE, cmp_gt>(); } \
    DEFINE_INSN_IMPLEMENTATION      (b_eq_     ## NAME) { return core.CompareRegRegAndBranch<STYPE, cmp_eq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (b_ne_     ## NAME) { return core.CompareRegRegAndBranch<STYPE, cmp_ne>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bs_lt_    ## NAME) { return core.CompareRegRegAndBranch<STYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bs_le_    ## NAME) { return core.CompareRegRegAndBranch<STYPE, cmp_le>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bu_lt_    ## NAME) { return core.CompareRegRegAndBranch<UTYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bu_le_    ## NAME) { return core.CompareRegRegAndBranch<UTYPE, cmp_le>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bi_eq_    ## NAME) { return core.CompareRegSimmAndBranch<STYPE, cmp_eq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bi_ne_    ## NAME) { return core.CompareRegSimmAndBranch<STYPE, cmp_ne>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bsi_lt_   ## NAME) { return core.CompareRegSimmAndBranch<STYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bsi_ge_   ## NAME) { return core.CompareRegSimmAndBranch<STYPE, cmp_ge>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bui_lt_   ## NAME) { return core.CompareRegSimmAndBranch<UTYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (bui_ge_   ## NAME) { return core.CompareRegSimmAndBranch<UTYPE, cmp_ge>(as); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_eq_   ## NAME) { return core.NullifyConditionallyRegReg<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ne_   ## NAME) { return core.NullifyConditionallyRegReg<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nuls_lt_  ## NAME) { return core.NullifyConditionallyRegReg<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulu_lt_  ## NAME) { return core.NullifyConditionallyRegReg<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nuls_le_  ## NAME) { return core.NullifyConditionallyRegReg<STYPE, cmp_le>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulu_le_  ## NAME) { return core.NullifyConditionallyRegReg<UTYPE, cmp_le>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nuli_eq_  ## NAME) { return core.NullifyConditionallyRegSimm<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nuli_ne_  ## NAME) { return core.NullifyConditionallyRegSimm<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulsi_lt_ ## NAME) { return core.NullifyConditionallyRegSimm<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nulsi_gt_ ## NAME) { return core.NullifyConditionallyRegSimm<STYPE, cmp_gt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nului_lt_ ## NAME) { return core.NullifyConditionallyRegSimm<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nului_gt_ ## NAME) { return core.NullifyConditionallyRegSimm<UTYPE, cmp_gt>(); } \

INT_BASE_SCALAR(w, i32, u32)
INT_BASE_SCALAR(d, i64, u64)

// pseudos
#define DEFINE_PSEUDO(NAME) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(NAME) { return core.unimplemented(); }

DEFINE_PSEUDO(cmps_gt_d)
DEFINE_PSEUDO(cmpu_gt_d)
DEFINE_PSEUDO(cmps_ge_d)
DEFINE_PSEUDO(cmpu_ge_d)
DEFINE_PSEUDO(cmpsi_ge_d)
DEFINE_PSEUDO(cmpui_ge_d)
DEFINE_PSEUDO(cmpsi_le_d)
DEFINE_PSEUDO(cmpui_le_d)
DEFINE_PSEUDO(cmps_ge_w)
DEFINE_PSEUDO(cmpu_ge_w)
DEFINE_PSEUDO(cmps_gt_w)
DEFINE_PSEUDO(cmpu_gt_w)
DEFINE_PSEUDO(cmps_gt_q)
DEFINE_PSEUDO(cmpu_gt_q)
DEFINE_PSEUDO(cmps_ge_q)
DEFINE_PSEUDO(cmpu_ge_q)
DEFINE_PSEUDO(cmpsi_ge_w)
DEFINE_PSEUDO(cmpui_ge_w)
DEFINE_PSEUDO(cmpsi_le_w)
DEFINE_PSEUDO(cmpui_le_w)
DEFINE_PSEUDO(bs_gt_d)
DEFINE_PSEUDO(bu_gt_d)
DEFINE_PSEUDO(bs_ge_d)
DEFINE_PSEUDO(bu_ge_d)
DEFINE_PSEUDO(bs_gt_w)
DEFINE_PSEUDO(bs_ge_w)
DEFINE_PSEUDO(bu_gt_w)
DEFINE_PSEUDO(bu_ge_w)
DEFINE_PSEUDO(bsi_le_d)
DEFINE_PSEUDO(bui_le_d)
DEFINE_PSEUDO(bsi_gt_d)
DEFINE_PSEUDO(bui_gt_d)
DEFINE_PSEUDO(bsi_le_w)
DEFINE_PSEUDO(bui_le_w)
DEFINE_PSEUDO(bsi_gt_w)
DEFINE_PSEUDO(bui_gt_w)
DEFINE_PSEUDO(flddi)
DEFINE_PSEUDO(flddri)
DEFINE_PSEUDO(fldqri)
DEFINE_PSEUDO(fldsi)
DEFINE_PSEUDO(fldsri)
DEFINE_PSEUDO(not)

} // namespace postrisc
