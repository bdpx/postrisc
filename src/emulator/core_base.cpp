#include "util/common.hpp"
#include "config/emulator_configuration.hpp"
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

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldan)
{
    u64 const address = core.rb().base() + (core.cmd.imm << (bits_per_binimm - 1));
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldanrc)
{
    u64 const address = core.ip + core.rb().base() + (core.cmd.imm << (bits_per_binimm - 1));
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldar)
{
    core.ra() = core.ip + sizeof(CBundle) * core.cmd.imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldafr)
{
    core.ra() = core.ip + core.cmd.imm;
    return core.FinalizeInstruction();
}

// deposits
DEFINE_INSN_IMPLEMENTATION_NOMEM(deps)
{
    const unsigned offset   = core.cmd.rc;
    const unsigned num_ones = core.cmd.rd;
    const u64 mask = rol<u64>(util::makemask(num_ones), offset);
    core.ra() = core.rb().base() | mask;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(depc)
{
    const unsigned offset   = core.cmd.rc;
    const unsigned num_ones = core.cmd.rd;

    const u64 mask = rol<u64>(util::makemask(num_ones), offset);
    core.ra() = core.rb().base() & ~mask;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(depa)
{
    const unsigned offset   = core.cmd.rc;
    const unsigned num_ones = core.cmd.rd;

    const u64 mask = rol<u64>(util::makemask(num_ones), offset);

    core.ra() = core.rb().base() ^ mask;

    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dep)
{
    CGeneralRegister const & b = core.rb();
    CGeneralRegister const & c = core.rc();

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsraw)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = sra<i32>(sll<u32>(core.rb().scalar<u32>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrlw)
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

// shift left and (add|subtract) [reverse]
DEFINE_INSN_IMPLEMENTATION_NOMEM(sladd)  { return core.fused_reg_reg_shamt < u64, shifted_add          > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsub)  { return core.fused_reg_reg_shamt < u64, shifted_subtract     > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsubf) { return core.fused_reg_reg_shamt < u64, shifted_subtract_rev > (); }
// fused shift right pair
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpi)   { return core.fused_reg_reg_shamt < u64, shift_right_pair_imm > (); }

//****************************************************************************************
// fused (multiply|add)-(add|subtract[from])
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::fused_reg_reg_reg(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>(), rd().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(muladd)  {  return core.fused_reg_reg_reg <u64, muladd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulsub)  {  return core.fused_reg_reg_reg <u64, mulsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulsubf) {  return core.fused_reg_reg_reg <u64, mulsub_rev       > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addadd)  {  return core.fused_reg_reg_reg <u64, addadd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addsub)  {  return core.fused_reg_reg_reg <u64, addsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subsub)  {  return core.fused_reg_reg_reg <u64, subsub           > (); }
// select
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbsel)   {  return core.fused_reg_reg_reg <u128, bitwise_select  > (); }
// funnel shifts
DEFINE_INSN_IMPLEMENTATION_NOMEM(srp)     {  return core.fused_reg_reg_reg <u64, shift_right_pair > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slp)     {  return core.fused_reg_reg_reg <u64, shift_left_pair  > (); }


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

DEFINE_INSN_IMPLEMENTATION_NOMEM(addaddc)
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

DEFINE_INSN_IMPLEMENTATION_NOMEM(subsubb)
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
DEFINE_INSN_IMPLEMENTATION_NOMEM(add)  { return core.binary_reg_reg < u64, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addw) { return core.binary_reg_reg < i32, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addq) { return core.binary_reg_reg <i128, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub)  { return core.binary_reg_reg < u64, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subw) { return core.binary_reg_reg < i32, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subq) { return core.binary_reg_reg <i128, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul)  { return core.binary_reg_reg < u64, mul                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulh) { return core.binary_reg_reg < u64, multiply_high      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addo) { return core.binary_reg_reg < u64, add_overflow       > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subo) { return core.binary_reg_reg < u64, subtract_overflow  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(absd) { return core.binary_reg_reg < i64, absolute_difference> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(absdw){ return core.binary_reg_reg < i32, absolute_difference> (); }

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(div)   { return core.binary_reg_reg_divmod < i64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divu)  { return core.binary_reg_reg_divmod < u64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mod)   { return core.binary_reg_reg_divmod < i64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(modu)  { return core.binary_reg_reg_divmod < u64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divw)  { return core.binary_reg_reg_divmod < i32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divuw) { return core.binary_reg_reg_divmod < u32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(modw)  { return core.binary_reg_reg_divmod < i32, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(moduw) { return core.binary_reg_reg_divmod < u32, mod > (); }

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
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpqeq) {    return core.BinaryRegRegBool < u128, cmp_eq> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpqne) {    return core.BinaryRegRegBool < u128, cmp_ne> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(andneg) {   return core.binary_reg_reg < u64, bitwise_andneg> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xordec) {   return core.binary_reg_reg < u64, bitwise_xordec> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(anddec) {   return core.binary_reg_reg < u64, bitwise_anddec> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(maxs) {     return core.binary_reg_reg < i64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxu) {     return core.binary_reg_reg < u64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mins) {     return core.binary_reg_reg < i64, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minu) {     return core.binary_reg_reg < u64, min> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsw) {    return core.binary_reg_reg < i32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxuw) {    return core.binary_reg_reg < u32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsw) {    return core.binary_reg_reg < i32, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minuw) {    return core.binary_reg_reg < u32, min> (); }

// shifts
DEFINE_INSN_IMPLEMENTATION_NOMEM(sll) {      return core.binary_reg_reg < u64, shift_left > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srl) {      return core.binary_reg_reg < u64, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sra) {      return core.binary_reg_reg < i64, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srd) {      return core.binary_reg_reg < i64, shift_right_div> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(sllw) {     return core.binary_reg_reg < u32, shift_left > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srlw) {     return core.binary_reg_reg < u32, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraw) {     return core.binary_reg_reg < i32, shift_right> (); }

// like bmi2 pdep & pext
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbscat) {   return core.binary_reg_reg < u64, bit_scatter> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbgath) {   return core.binary_reg_reg < u64, bit_gather> (); }

//****************************************************************************************
// binary register-immediate
//****************************************************************************************

DEFINE_INSN_IMPLEMENTATION_NOMEM(addi) {     return core.BinaryRegSimm< i64, add         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addiws) {   return core.BinaryRegSimm< i32, add         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addiwz) {   return core.BinaryRegSimm< u32, add         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subfi) {    return core.BinaryRegSimm< i64, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subfiws) {  return core.BinaryRegSimm< i32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subfiwz) {  return core.BinaryRegSimm< u32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(muli) {     return core.BinaryRegSimm< i64, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsi) {    return core.BinaryRegSimm< i64, max         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsi) {    return core.BinaryRegSimm< i64, min         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxui) {    return core.BinaryRegSimm< u64, max         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minui) {    return core.BinaryRegSimm< u64, min         > (); }

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
DEFINE_INSN_IMPLEMENTATION(allocsp)
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

DEFINE_INSN_IMPLEMENTATION(jmpr)
{
    CGeneralRegister const & b = core.rb();
    CGeneralRegister const & c = core.rc();

    u64 const ea = b.base() + (c.base() << core.cmd.scale);
    u64 const address = RoundToBundle(ea);

    LOG_DEBUG(LOG_BRANCH)
        // both base and index may be addresses
        << "jmpr: base=" << fmt::addr_info(as.get_decoder(), b.base())
        << " index=" << fmt::addr_info(as.get_decoder(), c.base())
        << " ea=" << fmt::hex(ea)
        << " address=" << fmt::hex(address);

    return core.ExecuteJump(as, address);
}

DEFINE_INSN_IMPLEMENTATION(jmpmi)
{
    u64 const base = core.rb().base();
    const CVirtAddress ea(base + ((i64)(i32)core.rc().base() << core.cmd.scale));
    u32 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read_u32/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(base + (i32)loaded);
    LOG_DEBUG(LOG_BRANCH) << "jump=" << fmt::hex(address);
    return core.ExecuteJump(as, address);
}

DEFINE_INSN_IMPLEMENTATION(callr)
{
    u64 const address = core.ip + sizeof(CBundle) * core.cmd.imm;
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(callri)
{
    u64 const ea = core.rb().base() + core.rc().base();
    u64 const address = RoundToBundle(ea);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(callmi)
{
    const CVirtAddress ea(core.rb().base() + core.cmd.imm);
    u64 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read_u64/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(callmrw)
{
    u64 const base = core.rb().base();
    const CVirtAddress ea(base + core.cmd.imm);
    u32 loaded;
    CHECK_STATUS_ADDRESS(core, ea, as.mmu_read_u32/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(base + (i32)loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(callplt)
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

DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbc)   { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbs)   { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) != 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbci)  { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbsi)  { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) != 0); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmall)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_all   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmany)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_any   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmnone)   { return core.NullifyConditionallyRegSimm<u64, cmp_mask_none  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmnotall) { return core.NullifyConditionallyRegSimm<u64, cmp_mask_notall> (); }

//****************************************************************************************
// load/store base+displacement
//****************************************************************************************
template<class T> ALWAYS_INLINE CStatus Core::read_base(IAddressSpace& as)
{
    CGeneralRegister const & b = rb();

    const CVirtAddress address(b.base() + cmd.imm);
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, &value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldbz) { return core.read_base<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldhz) { return core.read_base<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldwz) { return core.read_base<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(lddz) { return core.read_base<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldq)  { return core.read_base<u128>(as); }

DEFINE_INSN_IMPLEMENTATION(ldbs) { return core.read_base<i8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldhs) { return core.read_base<i16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldws) { return core.read_base<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldds) { return core.read_base<i64>(as); }

template<class T> ALWAYS_INLINE CStatus Core::write_base(IAddressSpace& as)
{
    const CVirtAddress address(rb().base() + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(stb) { return core.write_base<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(sth) { return core.write_base<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(stw) { return core.write_base<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(std) { return core.write_base<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(stq) { return core.write_base<u128>(as); }

//****************************************************************************************
// load/store ip-relative
//****************************************************************************************
template<class T> ALWAYS_INLINE CStatus Core::read_ip_relative(IAddressSpace& as)
{
    const CVirtAddress address(ip + cmd.imm);
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, &value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldbzr) { return core.read_ip_relative<u8> (as); }
DEFINE_INSN_IMPLEMENTATION(ldhzr) { return core.read_ip_relative<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldwzr) { return core.read_ip_relative<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(lddzr) { return core.read_ip_relative<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ldqr)  { return core.read_ip_relative<u128> (as); }

DEFINE_INSN_IMPLEMENTATION(ldbsr) { return core.read_ip_relative<i8> (as); }
DEFINE_INSN_IMPLEMENTATION(ldhsr) { return core.read_ip_relative<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldwsr) { return core.read_ip_relative<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(lddsr) { return core.read_ip_relative<i64> (as); }

template<class T> ALWAYS_INLINE CStatus Core::write_ip_relative(IAddressSpace& as)
{
    const CVirtAddress address(ip + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(stbr) { return core.write_ip_relative<u8> (as); }
DEFINE_INSN_IMPLEMENTATION(sthr) { return core.write_ip_relative<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(stwr) { return core.write_ip_relative<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(stdr) { return core.write_ip_relative<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(stqr) { return core.write_ip_relative<u128> (as); }

//****************************************************************************************
// load/store indexed (scaled)
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(ldax)
{
    u64 const address = core.ComputeScaledIndex();
    core.ra() = address;
    return core.FinalizeInstruction();
}

template<class T> ALWAYS_INLINE CStatus Core::read_indexed(IAddressSpace& as)
{
    const CVirtAddress address(ComputeScaledIndex());
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, &value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldbzx) { return core.read_indexed<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldhzx) { return core.read_indexed<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldwzx) { return core.read_indexed<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(lddzx) { return core.read_indexed<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldqx)  { return core.read_indexed<u128>(as); }

DEFINE_INSN_IMPLEMENTATION(ldbsx) { return core.read_indexed<i8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldhsx) { return core.read_indexed<i16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldwsx) { return core.read_indexed<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(lddsx) { return core.read_indexed<i64>(as); }

template<class T> ALWAYS_INLINE CStatus Core::write_indexed(IAddressSpace& as)
{
    const CVirtAddress address(ComputeScaledIndex());
    T const value = ra().scalar<T>();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(stbx) { return core.write_indexed<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(sthx) { return core.write_indexed<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(stwx) { return core.write_indexed<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(stdx) { return core.write_indexed<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(stqx) { return core.write_indexed<u128>(as); }

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
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, &value));
    rb().base() += cmd.imm;
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldbzmia) { return core.LoadModifyImmediateAfter<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldhzmia) { return core.LoadModifyImmediateAfter<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldwzmia) { return core.LoadModifyImmediateAfter<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(lddzmia) { return core.LoadModifyImmediateAfter<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ldqmia)  { return core.LoadModifyImmediateAfter<u128>(as); }

DEFINE_INSN_IMPLEMENTATION(ldbsmia) { return core.LoadModifyImmediateAfter<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldhsmia) { return core.LoadModifyImmediateAfter<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldwsmia) { return core.LoadModifyImmediateAfter<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(lddsmia) { return core.LoadModifyImmediateAfter<i64> (as); }

template<class T> ALWAYS_INLINE CStatus Core::StoreModifyImmediateAfter(IAddressSpace& as)
{
    const T value = ra().scalar<T>();
    const CVirtAddress address(rb().base()); // + cmd.GetIndexedDisplacement();
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    rb().base() += cmd.imm;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(stbmia) { return core.StoreModifyImmediateAfter<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(sthmia) { return core.StoreModifyImmediateAfter<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(stwmia) { return core.StoreModifyImmediateAfter<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(stdmia) { return core.StoreModifyImmediateAfter<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(stqmia) { return core.StoreModifyImmediateAfter<u128>(as); }

//****************************************************************************************
// base+disp load/store with immediate base update (before)
//****************************************************************************************
template<class T> ALWAYS_INLINE CStatus Core::LoadModifyImmediateBefore(IAddressSpace& as)
{
    CHECK_BASE_UPDATE_INSN
    const CVirtAddress address(rb().base() + cmd.imm);
    T value;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, &value));
    rb().base() = address.get_address();
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldbzmib) { return core.LoadModifyImmediateBefore<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldhzmib) { return core.LoadModifyImmediateBefore<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldwzmib) { return core.LoadModifyImmediateBefore<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(lddzmib) { return core.LoadModifyImmediateBefore<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ldqmib)  { return core.LoadModifyImmediateBefore<u128> (as); }

DEFINE_INSN_IMPLEMENTATION(ldbsmib) { return core.LoadModifyImmediateBefore<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldhsmib) { return core.LoadModifyImmediateBefore<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldwsmib) { return core.LoadModifyImmediateBefore<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(lddsmib) { return core.LoadModifyImmediateBefore<i64> (as); }

template<class T> ALWAYS_INLINE CStatus Core::StoreModifyImmediateBefore(IAddressSpace& as)
{
    const T value = ra().scalar<T>();
    const CVirtAddress address(rb().base() + cmd.imm);
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    rb().base() = address.get_address();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(stbmib) { return core.StoreModifyImmediateBefore<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(sthmib) { return core.StoreModifyImmediateBefore<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(stwmib) { return core.StoreModifyImmediateBefore<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(stdmib) { return core.StoreModifyImmediateBefore<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(stqmib) { return core.StoreModifyImmediateBefore<u128>(as); }

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

DEFINE_INSN_IMPLEMENTATION(bmall)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_all   > (as); }
DEFINE_INSN_IMPLEMENTATION(bmany)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_any   > (as); }
DEFINE_INSN_IMPLEMENTATION(bmnone)   { return core.CompareRegSimmAndBranch<u64, cmp_mask_none  > (as); }
DEFINE_INSN_IMPLEMENTATION(bmnotall) { return core.CompareRegSimmAndBranch<u64, cmp_mask_notall> (as); }

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

DEFINE_INSN_IMPLEMENTATION(bbc)  { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION(bbs)  { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) != 0); }
DEFINE_INSN_IMPLEMENTATION(bbci) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION(bbsi) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) != 0); }

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

DEFINE_INSN_IMPLEMENTATION(repdle) { return core.RepeatBranch<i64, cmp_le,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(repdlt) { return core.RepeatBranch<i64, cmp_lt,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(repdge) { return core.RepeatBranch<i64, cmp_ge, -1> (as); }
DEFINE_INSN_IMPLEMENTATION(repdgt) { return core.RepeatBranch<i64, cmp_gt, -1> (as); }

DEFINE_INSN_IMPLEMENTATION(repdleu) { return core.RepeatBranch<u64, cmp_le,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(repdltu) { return core.RepeatBranch<u64, cmp_lt,  1> (as); }
DEFINE_INSN_IMPLEMENTATION(repdgeu) { return core.RepeatBranch<u64, cmp_ge, -1> (as); }
DEFINE_INSN_IMPLEMENTATION(repdgtu) { return core.RepeatBranch<u64, cmp_gt, -1> (as); }

//****************************************************************************************
// branches, ip-relative
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(ldi)
{
    const u64 imm = core.cmd.imm;
    core.ra() = imm;
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
    CGeneralRegister const & b = rb();
    CGeneralRegister const & c = rc();

    u64 const table_address = b.base();
    const CVirtAddress address(table_address + c.base() * sizeof(T));
    T index;
    CHECK_STATUS_ADDRESS(*this, address, as.mmu_read(*this, address, &index));

    u64 const target_address = ip + sizeof(CBundle) * index;

    LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
        << "taken jump=" << fmt::hex(target_address);

    return ExecuteJump(as, target_address);
}

DEFINE_INSN_IMPLEMENTATION(jmpth) { return core.tabular_branch<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(jmptw) { return core.tabular_branch<u32>(as); }

//****************************************************************************************
// reg + shamt
//****************************************************************************************
template<class T, class OP> ALWAYS_INLINE CStatus Core::BinaryRegShamt(void)
{
    const T value = OP::func(rb().scalar<T>(), static_cast<unsigned>(cmd.rc));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(cnttz)  { return core.BinaryRegShamt< u64, trailing_zeros            > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cntlz)  { return core.BinaryRegShamt< u64, leading_zeros             > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cntpop) { return core.BinaryRegShamt< u64, population                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(permb)  { return core.BinaryRegShamt< u64, permute                   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slli)   { return core.BinaryRegShamt< u64, shift_left_immediate      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srli)   { return core.BinaryRegShamt< u64, shift_right_immediate     > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srai)   { return core.BinaryRegShamt< i64, shift_right_immediate     > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srdi)   { return core.BinaryRegShamt< i64, shift_right_div_immediate > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(slliw)  { return core.BinaryRegShamt< u32, shift_left_immediate      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srliw)  { return core.BinaryRegShamt< u32, shift_right_immediate     > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraiw)  { return core.BinaryRegShamt< i32, shift_right_immediate     > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(sllqi) { return core.BinaryRegShamt< u128, shift_left_immediate    > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srlqi) { return core.BinaryRegShamt< u128, shift_right_immediate   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraqi) { return core.BinaryRegShamt< i128, shift_right_immediate   > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(gtb)
{
    core.ra() = util::gray2bin( core.rb().scalar<u64>() );
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(depq)
{
    CGeneralRegister const & b = core.rb();
    CGeneralRegister const & c = core.rc();

    core.ra() = deposit(b, c, core.cmd.rd);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(srpiq)
{
    CGeneralRegister const & b = core.rb();
    CGeneralRegister const & c = core.rc();

    core.ra() = funnel_shift_right(b, c, core.cmd.rd);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(perm)
{
    CGeneralRegister const & b = core.rb();
    CGeneralRegister const & c = core.rc();
    CGeneralRegister const & d = core.rd();

    CGeneralRegister tmp;

    for (size_t i=0; i<b.size<u8>(); i++) {
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

DEFINE_INSN_IMPLEMENTATION_NOMEM(select)
{
    bool const condition = (core.rb().base() & 1);
    core.ra() = condition ? core.rc() : core.rd();
    return core.FinalizeInstruction();
}

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
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## eq  ) { return core.BinaryRegRegBool < STYPE, cmp_eq> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## ne  ) { return core.BinaryRegRegBool < STYPE, cmp_ne> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## lt  ) { return core.BinaryRegRegBool < STYPE, cmp_lt> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## le  ) { return core.BinaryRegRegBool < STYPE, cmp_le> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## ltu ) { return core.BinaryRegRegBool < UTYPE, cmp_lt> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## leu ) { return core.BinaryRegRegBool < UTYPE, cmp_le> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## eqi ) { return core.BinaryRegSimm< STYPE, cmp_eq> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## nei ) { return core.BinaryRegSimm< STYPE, cmp_ne> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## lti ) { return core.BinaryRegSimm< STYPE, cmp_lt> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## gti ) { return core.BinaryRegSimm< STYPE, cmp_gt> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## ltui) { return core.BinaryRegSimm< UTYPE, cmp_lt> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp ## NAME ## gtui) { return core.BinaryRegSimm< UTYPE, cmp_gt> (); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## eq  ) { return core.CompareRegRegAndBranch<STYPE, cmp_eq > (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## ne  ) { return core.CompareRegRegAndBranch<STYPE, cmp_ne > (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## lt  ) { return core.CompareRegRegAndBranch<STYPE, cmp_lt > (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## le  ) { return core.CompareRegRegAndBranch<STYPE, cmp_le > (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## ltu ) { return core.CompareRegRegAndBranch<UTYPE, cmp_lt > (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## leu ) { return core.CompareRegRegAndBranch<UTYPE, cmp_le > (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## eqi ) { return core.CompareRegSimmAndBranch<STYPE, cmp_eq> (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## nei ) { return core.CompareRegSimmAndBranch<STYPE, cmp_ne> (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## lti ) { return core.CompareRegSimmAndBranch<STYPE, cmp_lt> (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## gei ) { return core.CompareRegSimmAndBranch<STYPE, cmp_ge> (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## ltui) { return core.CompareRegSimmAndBranch<UTYPE, cmp_lt> (as); } \
    DEFINE_INSN_IMPLEMENTATION      (b   ## NAME ## geui) { return core.CompareRegSimmAndBranch<UTYPE, cmp_ge> (as); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## eq  ) { return core.NullifyConditionallyRegReg<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## ne  ) { return core.NullifyConditionallyRegReg<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## lt  ) { return core.NullifyConditionallyRegReg<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## ltu ) { return core.NullifyConditionallyRegReg<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## le  ) { return core.NullifyConditionallyRegReg<STYPE, cmp_le>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## leu ) { return core.NullifyConditionallyRegReg<UTYPE, cmp_le>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## eqi ) { return core.NullifyConditionallyRegSimm<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## nei ) { return core.NullifyConditionallyRegSimm<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## lti ) { return core.NullifyConditionallyRegSimm<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## gti ) { return core.NullifyConditionallyRegSimm<STYPE, cmp_gt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## ltui) { return core.NullifyConditionallyRegSimm<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul ## NAME ## gtui) { return core.NullifyConditionallyRegSimm<UTYPE, cmp_gt>(); } \

INT_BASE_SCALAR(w, i32, u32)
INT_BASE_SCALAR(d, i64, u64)

// pseudos
#define DEFINE_PSEUDO(NAME) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(NAME) { return core.unimplemented(); }

DEFINE_PSEUDO(cmpdgt)
DEFINE_PSEUDO(cmpdgtu)
DEFINE_PSEUDO(cmpdge)
DEFINE_PSEUDO(cmpdgeu)
DEFINE_PSEUDO(cmpdgei)
DEFINE_PSEUDO(cmpdgeui)
DEFINE_PSEUDO(cmpdlei)
DEFINE_PSEUDO(cmpdleui)
DEFINE_PSEUDO(cmpwge)
DEFINE_PSEUDO(cmpwgeu)
DEFINE_PSEUDO(cmpwgt)
DEFINE_PSEUDO(cmpwgtu)
DEFINE_PSEUDO(cmpwgei)
DEFINE_PSEUDO(cmpwgeui)
DEFINE_PSEUDO(cmpwlei)
DEFINE_PSEUDO(cmpwleui)
DEFINE_PSEUDO(bdgt)
DEFINE_PSEUDO(bdgtu)
DEFINE_PSEUDO(bdge)
DEFINE_PSEUDO(bdgeu)
DEFINE_PSEUDO(bdlei)
DEFINE_PSEUDO(bdleui)
DEFINE_PSEUDO(bdgti)
DEFINE_PSEUDO(bdgtui)
DEFINE_PSEUDO(bwgt)
DEFINE_PSEUDO(bwge)
DEFINE_PSEUDO(bwgtu)
DEFINE_PSEUDO(bwgeu)
DEFINE_PSEUDO(bwlei)
DEFINE_PSEUDO(bwleui)
DEFINE_PSEUDO(bwgti)
DEFINE_PSEUDO(bwgtui)
DEFINE_PSEUDO(flddi)
DEFINE_PSEUDO(flddri)
DEFINE_PSEUDO(fldqri)
DEFINE_PSEUDO(fldsi)
DEFINE_PSEUDO(fldsri)
DEFINE_PSEUDO(not)

} // namespace postrisc
