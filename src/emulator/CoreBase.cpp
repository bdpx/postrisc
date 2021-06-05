#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

// pseudos
#define DEFINE_PSEUDO(NAME) DEFINE_INSN_IMPLEMENTATION_NOMEM(NAME) { return core.unimplemented(); }

Result Core::unimplemented(void) const
{
    LOG_ERROR(LOG_EXCEPTION) << "not yet implemented opx";
    return Result::illegal_instruction;
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(nop)
{
    core.performance_monitor.report_event(PMC_NOP_INSTRUCTION);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(mov)
{
    GeneralRegister const t = core.rb();
    core.ra() = t;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(mov2)
{
    GeneralRegister const t1 = core.rc();
    GeneralRegister const t2 = core.rd();

    core.ra() = t1;
    core.rb() = t2;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(lda_n)
{
    u64 const address = core.rb().base() + (core.cmd.imm << bits_per_lda_near_shift);
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(lda_nrc)
{
    u64 const address = core.ip + core.rb().base() + (core.cmd.imm << bits_per_lda_near_shift);
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(lda_r)
{
    core.ra() = core.ip + sizeof(Bundle) * core.cmd.imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(lda_iprel)
{
    core.ra() = core.ip + core.cmd.imm;
    return core.FinalizeInstruction();
}

// deposits
DEFINE_INSN_IMPLEMENTATION_NOMEM(deposit)
{
    const GeneralRegister& b = core.rb();
    const GeneralRegister& c = core.rc();

    const unsigned offset   = core.cmd.rd;
    const unsigned num_ones = core.cmd.re;

    const u128 mask  = util::makemask<u128>(num_ones) << offset;
    const u128 value = c.scalar<u128>()               << offset;

    core.ra() = bitwise_select::func<u128>(value, b.scalar<u128>(), mask);

    return core.FinalizeInstruction();
}

// fused shifts
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsra_imm_i64)
{
    const unsigned left_shift  = core.cmd.rc;
    const unsigned right_shift = core.cmd.rd;

    core.ra() = sra<i64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrl_imm_u64)
{
    const unsigned left_shift  = core.cmd.rc;
    const unsigned right_shift = core.cmd.rd;

    core.ra() = srl<u64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsra_i64)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = sra<i64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrl_u64)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = srl<u64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsra_i32)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = sra<i32>(sll<u32>(core.rb().scalar<u32>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrl_u32)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = srl<u32>(sll<u32>(core.rb().scalar<u32>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

//****************************************************************************************
// fused shifted-(add|subtract[from])
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::fused_reg_reg_shamt(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>(), cmd.rd );
    ra() = value;
    return FinalizeInstruction();
}

// shift left and (add|subtract[reverse]|or|xor)
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_add_i64)  { return core.fused_reg_reg_shamt<u64, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_add_i32)  { return core.fused_reg_reg_shamt<i32, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_add_u32)  { return core.fused_reg_reg_shamt<u32, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_sub_i64)  { return core.fused_reg_reg_shamt<u64, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_sub_i32)  { return core.fused_reg_reg_shamt<i32, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_sub_u32)  { return core.fused_reg_reg_shamt<u32, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_subr_i64) { return core.fused_reg_reg_shamt<u64, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_subr_i32) { return core.fused_reg_reg_shamt<i32, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_subr_u32) { return core.fused_reg_reg_shamt<u32, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_or)       { return core.fused_reg_reg_shamt<u64, shifted_or          >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sl_xor)      { return core.fused_reg_reg_shamt<u64, shifted_xor         >(); }

// fused shift right pair
DEFINE_INSN_IMPLEMENTATION_NOMEM(srp_imm_i32)  { return core.fused_reg_reg_shamt < u32, shift_right_pair_imm > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srp_imm_i64)  { return core.fused_reg_reg_shamt < u64, shift_right_pair_imm > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srp_imm_i128) { return core.fused_reg_reg_shamt < u128,shift_right_pair_imm > (); }

//****************************************************************************************
// fused (multiply|add)-(add|subtract[from])
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::fused_reg_reg_reg(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>(), rd().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_add)  {  return core.fused_reg_reg_reg <u64, muladd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_sub)  {  return core.fused_reg_reg_reg <u64, mulsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_subr) {  return core.fused_reg_reg_reg <u64, mulsub_rev       > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(add_add_i64) { return core.fused_reg_reg_reg <i64, addadd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_sub_i64) { return core.fused_reg_reg_reg <i64, addsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_sub_i64) { return core.fused_reg_reg_reg <i64, subsub           > (); }
// select
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbsel)   {  return core.fused_reg_reg_reg <u128, bitwise_select  > (); }


//****************************************************************************************
// multiprecision arithmetic
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(addc_u64)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();

    core.ra() = (static_cast<u128>(vb) + static_cast<u128>(vc));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(add_addc_u64)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();
    const u128 vd = core.rd().scalar<u128>();

    core.ra() = (static_cast<u128>(vb) + static_cast<u128>(vc) + (vd >> 64));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(subb_u64)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();

    core.ra() = (static_cast<u128>(vb) - static_cast<u128>(vc));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_subb_u64)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();
    const u128 vd = core.rd().scalar<u128>();

    core.ra() = (static_cast<u128>(vb) - static_cast<u128>(vc) + (vd >> 64));
    return core.FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::unary_reg(void)
{
    const T value = OP::func( rb().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(not) { return core.unary_reg <i128, bitwise_not> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(sext_i8)  { return core.unary_reg <i8,  sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sext_i16) { return core.unary_reg <i16, sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sext_i32) { return core.unary_reg <i32, sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sext_i64) { return core.unary_reg <i64, sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zext_i8)  { return core.unary_reg <u8,  zero_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zext_i16) { return core.unary_reg <u16, zero_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zext_i32) { return core.unary_reg <u32, zero_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zext_i64) { return core.unary_reg <u64, zero_extend> (); }

//****************************************************************************************
// binary reg-reg
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::binary_reg_reg(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

// custom, arithmetic
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_u32)  { return core.binary_reg_reg < u32, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_u32)  { return core.binary_reg_reg < u32, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_u32)  { return core.binary_reg_reg < u32, mul                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_h)    { return core.binary_reg_reg < u64, multiply_high      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addo_i64) { return core.binary_reg_reg < u64, add_overflow       > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subo_i64) { return core.binary_reg_reg < u64, subtract_overflow  > (); }

// bitwise
DEFINE_INSN_IMPLEMENTATION_NOMEM(and)  { return core.binary_reg_reg <u128, bitwise_and > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(or)   { return core.binary_reg_reg <u128, bitwise_or  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xor)  { return core.binary_reg_reg <u128, bitwise_xor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xnor) { return core.binary_reg_reg <u128, bitwise_xnor> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nand) { return core.binary_reg_reg <u128, bitwise_nand> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nor)  { return core.binary_reg_reg <u128, bitwise_nor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(andn) { return core.binary_reg_reg <u128, bitwise_andn> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(orn)  { return core.binary_reg_reg <u128, bitwise_orn > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(bit_set)  { return core.binary_reg_reg<u128, bit_set  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bit_clear){ return core.binary_reg_reg<u128, bit_clear> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bit_flip) { return core.binary_reg_reg<u128, bit_flip > (); }

template<typename T, typename OP> ALWAYS_INLINE Result Core::binary_reg_reg_divmod(void)
{
    T const divisor = rc().scalar<T>();
    if (divisor == static_cast<T>(0)) {
        LOG_ERROR(LOG_EXCEPTION) << "divide by zero";
        return Result::event_divide_by_zero;
    }
    ra() = OP::func(rb().scalar<T>(), divisor);
    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::BinaryRegRegBool(void)
{
    const bool value = OP::func( rb().scalar<T>(), rc().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::BinaryRegSimm(void)
{
    const T value = OP::func( rb().scalar<T>(), static_cast<T>(cmd.imm) );
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(and_neg) {   return core.binary_reg_reg < u64, bitwise_andneg> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xor_dec) {   return core.binary_reg_reg < u64, bitwise_xordec> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(and_dec) {   return core.binary_reg_reg < u64, bitwise_anddec> (); }

// like bmi2 pdep & pext
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbscat) { return core.binary_reg_reg < u64, bit_scatter> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbgath) { return core.binary_reg_reg < u64, bit_gather> (); }

//****************************************************************************************
// binary register-immediate
//****************************************************************************************

DEFINE_INSN_IMPLEMENTATION_NOMEM(add_imm_i64)  {  return core.BinaryRegSimm< i64,  add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_imm_i32)  {  return core.BinaryRegSimm< i32,  add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_imm_u32)  {  return core.BinaryRegSimm< u32,  add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(add_imm_i128) {  return core.BinaryRegSimm< i128, add> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(subr_imm_i64) {  return core.BinaryRegSimm< i64, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subr_imm_i32) {  return core.BinaryRegSimm< i32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subr_imm_u32) {  return core.BinaryRegSimm< u32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_imm_i64)  {  return core.BinaryRegSimm< i64, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_imm_i32)  {  return core.BinaryRegSimm< i32, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_imm_u32)  {  return core.BinaryRegSimm< u32, mul         > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(max_imm_i64)  {  return core.BinaryRegSimm<i64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(min_imm_i64)  {  return core.BinaryRegSimm<i64, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(max_imm_u64)  {  return core.BinaryRegSimm<u64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(min_imm_u64)  {  return core.BinaryRegSimm<u64, min> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(max_imm_i32)  {  return core.BinaryRegSimm<i32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(min_imm_i32)  {  return core.BinaryRegSimm<i32, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(max_imm_u32)  {  return core.BinaryRegSimm<u32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(min_imm_u32)  {  return core.BinaryRegSimm<u32, min> (); }

// binary register-immediate logical
DEFINE_INSN_IMPLEMENTATION_NOMEM(and_imm) {     return core.BinaryRegSimm< i128, bitwise_and > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(or_imm) {      return core.BinaryRegSimm< i128, bitwise_or  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xor_imm) {     return core.BinaryRegSimm< i128, bitwise_xor > (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(xnor_imm) {    return core.BinaryRegSimm< i128, bitwise_xnor> (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(nand_imm) {    return core.BinaryRegSimm< i128, bitwise_nand> (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(nor_imm) {     return core.BinaryRegSimm< i128, bitwise_nor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(andn_imm) {    return core.BinaryRegSimm< i128, bitwise_andn> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(orn_imm) {     return core.BinaryRegSimm< i128, bitwise_orn > (); }


template<typename T, typename OP> ALWAYS_INLINE Result Core::binary_reg_simm_divmod(void)
{
    T const divisor = static_cast<T>(cmd.imm);
    if (divisor == static_cast<T>(0)) {
        LOG_ERROR(LOG_EXCEPTION) << "divide by zero";
        return Result::event_divide_by_zero;
    }
    ra() = OP::func(rb().scalar<T>(), divisor);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(div_imm_i64) { return core.binary_reg_simm_divmod< i64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(rem_imm_i64) { return core.binary_reg_simm_divmod< i64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(div_imm_u64) { return core.binary_reg_simm_divmod< u64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(rem_imm_u64) { return core.binary_reg_simm_divmod< u64, mod > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(div_imm_i32) { return core.binary_reg_simm_divmod< i32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(rem_imm_i32) { return core.binary_reg_simm_divmod< i32, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(div_imm_u32) { return core.binary_reg_simm_divmod< u32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(rem_imm_u32) { return core.binary_reg_simm_divmod< u32, mod > (); }

DEFINE_INSN_IMPLEMENTATION(alloc)
{
    unsigned const framesize = core.cmd.rb;
    LOG_DEBUG(LOG_CALLSTACK) << "framesize=" << framesize;

    CHECK_RESULT(core.AllocateFrame(as, framesize));

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

    GeneralRegister& sp = core.w_static_reg(reg_sp);
    if (sp.base() < (u64)core.cmd.imm || sp.base() - core.cmd.imm < core.bsp) {
        LOG_ERROR(LOG_CALLSTACK) << "bsp underflow: bsp=" << fmt::hex(core.bsp) << " sp=" << fmt::hex(sp.base());
        return Result::general_protection;
    }

    CHECK_RESULT(core.AllocateFrame(as, framesize));

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
    const GeneralRegister& b = core.rb();
    const GeneralRegister& c = core.rc();

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
    const VirtAddress ea(base + ((i64)(i32)core.rc().base() << core.cmd.scale));
    u32 loaded;
    CHECK_RESULT_ADDRESS(core, ea, as.mmu_read_u32/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(base + (i32)loaded);
    LOG_DEBUG(LOG_BRANCH) << "jump=" << fmt::hex(address);
    return core.ExecuteJump(as, address);
}

DEFINE_INSN_IMPLEMENTATION(call)
{
    u64 const address = core.ip + sizeof(Bundle) * core.cmd.imm;
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
    const VirtAddress ea(core.rb().base() + core.cmd.imm);
    u64 loaded;
    CHECK_RESULT_ADDRESS(core, ea, as.mmu_read_u64/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(call_rvt)
{
    u64 const base = core.rb().base();
    const VirtAddress ea(base + core.cmd.imm);
    i32 loaded;
    CHECK_RESULT_ADDRESS(core, ea, as.mmu_read/*FIXME:_i32_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(base + loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(call_plt)
{
    const VirtAddress ea(core.ip + core.cmd.imm);

    u64 loaded;
    CHECK_RESULT_ADDRESS(core, ea, as.mmu_read_u64/*FIXME:_finalized*/(core, ea, loaded));

    u64 const address = RoundToBundle(loaded);
    LOG_DEBUG(LOG_CALLSTACK)
        << "call=" << fmt::hex(address);

    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(ret)
{
    unsigned temp, ri0_result;
    ROTATE_REG(core, ri0_result, temp, 0)
    const GeneralRegister & link = core.rget( ri0_result );

    const Result result = core.ReturnFromSubroutine(as, link);
    if (!result.success())
        return result;

#if defined(DEBUG_LINK_INFO_STACK)
    CHECK_RESULT(core.CheckStackOnReturn());
#endif
    return result;
}

DEFINE_INSN_IMPLEMENTATION(retf)
{
    unsigned ri_result, temp;
    ROTATE_REG(core, ri_result, temp, 0)
    const GeneralRegister & link = core.rget(ri_result);

    Result result = core.ReturnFromSubroutine(as, link);
    if (!result.success())
        return result;

    GeneralRegister & stack = core.w_static_reg(reg_sp);
    // stack grows back up
    stack = stack.base() + core.cmd.imm;
    LOG_DEBUG(LOG_CALLSTACK)
        << "retf sp=" << fmt::hex(stack.base());

#if defined(DEBUG_LINK_INFO_STACK)
    CHECK_RESULT(core.CheckStackOnReturn());
#endif
    return result;
}

//****************************************************************************************
// future's nullifying
//****************************************************************************************
Result Core::NullifyConditionally(bool condition)
{
    LOG_DEBUG(LOG_PREDICATION)
        << "condition=" << condition
        << "psr=" << psr;

    unsigned const dist_yes = cmd.dist_yes;
    unsigned const dist_not = cmd.dist_no;

    if (unlikely(dist_yes + dist_not > 32)) {
        LOG_ERROR(LOG_PREDICATION)
            << "summary future exceed limit:"
            << " dist_yes=" << dist_yes
            << " dist_not=" << dist_not;
        return Result::illegal_instruction;
    }

    // nullify first instruction-block
    // nullify second instruction-block, forward future mask over first block
    const u32 mask =
    condition ? (util::makemask<u32>(dist_yes)            )
              : (util::makemask<u32>(dist_not) << dist_yes);

    psr.future.nullify(mask);

    LOG_DEBUG(LOG_PREDICATION) << "psr=" << psr;
    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::NullifyConditionallyRegReg(void)
{
    const bool condition = OP::func(ra().scalar<T>(), rb().scalar<T>());
    return NullifyConditionally(condition);
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::NullifyConditionallyRegSimm(void)
{
    const T b = cmd.brc_imm;
    const bool condition = OP::func(ra().scalar<T>(), b);
    return NullifyConditionally(condition);
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bc) { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bs) { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) != 0); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bc_imm) { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_bs_imm) { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) != 0); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_mask_all)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_all   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_mask_any)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_any   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_mask_none)   { return core.NullifyConditionallyRegSimm<u64, cmp_mask_none  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_mask_notall) { return core.NullifyConditionallyRegSimm<u64, cmp_mask_notall> (); }

//****************************************************************************************
// load/store base+displacement
//****************************************************************************************
template<typename T> ALWAYS_INLINE Result Core::read_base(AddressSpace& as)
{
    const GeneralRegister& b = rb();

    const VirtAddress address(b.base() + cmd.imm);
    T value;
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld_u8)   { return core.read_base<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_u16)  { return core.read_base<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_u32)  { return core.read_base<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_u64)  { return core.read_base<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_i8)   { return core.read_base<i8>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_i16)  { return core.read_base<i16>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_i32)  { return core.read_base<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_i64)  { return core.read_base<i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_i128) { return core.read_base<i128>(as); }

template<typename T> ALWAYS_INLINE Result Core::write_base(AddressSpace& as)
{
    const VirtAddress address(rb().base() + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_i8)   { return core.write_base<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(st_i16)  { return core.write_base<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(st_i32)  { return core.write_base<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_i64)  { return core.write_base<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_i128) { return core.write_base<u128>(as); }

//****************************************************************************************
// load/store ip-relative
//****************************************************************************************
template<typename T> ALWAYS_INLINE Result Core::read_ip_relative(AddressSpace& as)
{
    const VirtAddress address(ip + cmd.imm);
    T value;
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld_iprel_u8)   { return core.read_ip_relative<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_u16)  { return core.read_ip_relative<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_u32)  { return core.read_ip_relative<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_u64)  { return core.read_ip_relative<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_i8)   { return core.read_ip_relative<i8>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_i16)  { return core.read_ip_relative<i16>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_i32)  { return core.read_ip_relative<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_i64)  { return core.read_ip_relative<i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_iprel_i128) { return core.read_ip_relative<i128>(as); }

template<typename T> ALWAYS_INLINE Result Core::write_ip_relative(AddressSpace& as)
{
    const VirtAddress address(ip + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_iprel_i8)   { return core.write_ip_relative<u8> (as); }
DEFINE_INSN_IMPLEMENTATION(st_iprel_i16)  { return core.write_ip_relative<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(st_iprel_i32)  { return core.write_ip_relative<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(st_iprel_i64)  { return core.write_ip_relative<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(st_iprel_i128) { return core.write_ip_relative<u128> (as); }

//****************************************************************************************
// load/store indexed (scaled)
//****************************************************************************************
template<typename INDEX> ALWAYS_INLINE Result Core::compute_address_indexed(AddressSpace&)
{
    u64 const address = ComputeIndexingAddress<INDEX>();
    ra() = address;
    return FinalizeInstruction();
}

template<typename T, typename INDEX> ALWAYS_INLINE Result Core::read_indexed(AddressSpace& as)
{
    const VirtAddress address(ComputeIndexingAddress<INDEX>());
    T value;
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    ra() = value;
    return FinalizeInstruction();
}

template<typename T, typename INDEX> ALWAYS_INLINE Result Core::write_indexed(AddressSpace& as)
{
    const VirtAddress address(ComputeIndexingAddress<INDEX>());
    T const value = ra().scalar<T>();
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld_xi64_u8)   { return core.read_indexed<u8,    i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_u16)  { return core.read_indexed<u16,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_u32)  { return core.read_indexed<u32,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_u64)  { return core.read_indexed<u64,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_i8)   { return core.read_indexed<i8,    i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_i16)  { return core.read_indexed<i16,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_i32)  { return core.read_indexed<i32,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_i64)  { return core.read_indexed<i64,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi64_i128) { return core.read_indexed<i128,  i64>(as); }

DEFINE_INSN_IMPLEMENTATION(st_xi64_i8)   { return core.write_indexed<u8,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi64_i16)  { return core.write_indexed<u16,  i64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi64_i32)  { return core.write_indexed<u32,  i64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi64_i64)  { return core.write_indexed<u64,  i64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi64_i128) { return core.write_indexed<u128, i64>(as); }

DEFINE_INSN_IMPLEMENTATION(lda_xi64)     { return core.compute_address_indexed<i64>(as); }

DEFINE_INSN_IMPLEMENTATION(ld_xi32_u8)   { return core.read_indexed<u8,    i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_u16)  { return core.read_indexed<u16,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_u32)  { return core.read_indexed<u32,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_u64)  { return core.read_indexed<u64,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_i8)   { return core.read_indexed<i8,    i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_i16)  { return core.read_indexed<i16,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_i32)  { return core.read_indexed<i32,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_i64)  { return core.read_indexed<i64,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xi32_i128) { return core.read_indexed<i128,  i32>(as); }

DEFINE_INSN_IMPLEMENTATION(st_xi32_i8)   { return core.write_indexed<u8,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi32_i16)  { return core.write_indexed<u16,  i32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi32_i32)  { return core.write_indexed<u32,  i32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi32_i64)  { return core.write_indexed<u64,  i32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xi32_i128) { return core.write_indexed<u128, i32>(as); }

DEFINE_INSN_IMPLEMENTATION(lda_xi32)     { return core.compute_address_indexed<i32>(as); }

DEFINE_INSN_IMPLEMENTATION(ld_xu64_u8)   { return core.read_indexed<u8,    u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_u16)  { return core.read_indexed<u16,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_u32)  { return core.read_indexed<u32,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_u64)  { return core.read_indexed<u64,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_i8)   { return core.read_indexed<i8,    u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_i16)  { return core.read_indexed<i16,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_i32)  { return core.read_indexed<i32,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_i64)  { return core.read_indexed<i64,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu64_i128) { return core.read_indexed<i128,  u64>(as); }

DEFINE_INSN_IMPLEMENTATION(st_xu64_i8)   { return core.write_indexed<u8,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu64_i16)  { return core.write_indexed<u16,  u64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu64_i32)  { return core.write_indexed<u32,  u64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu64_i64)  { return core.write_indexed<u64,  u64>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu64_i128) { return core.write_indexed<u128, u64>(as); }

DEFINE_INSN_IMPLEMENTATION(lda_xu64)     { return core.compute_address_indexed<u64>(as); }

DEFINE_INSN_IMPLEMENTATION(ld_xu32_u8)   { return core.read_indexed<u8,    u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_u16)  { return core.read_indexed<u16,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_u32)  { return core.read_indexed<u32,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_u64)  { return core.read_indexed<u64,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_i8)   { return core.read_indexed<i8,    u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_i16)  { return core.read_indexed<i16,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_i32)  { return core.read_indexed<i32,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_i64)  { return core.read_indexed<i64,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ld_xu32_i128) { return core.read_indexed<i128,  u32>(as); }

DEFINE_INSN_IMPLEMENTATION(st_xu32_i8)   { return core.write_indexed<u8,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu32_i16)  { return core.write_indexed<u16,  u32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu32_i32)  { return core.write_indexed<u32,  u32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu32_i64)  { return core.write_indexed<u64,  u32>(as); }
DEFINE_INSN_IMPLEMENTATION(st_xu32_i128) { return core.write_indexed<u128, u32>(as); }

DEFINE_INSN_IMPLEMENTATION(lda_xu32)     { return core.compute_address_indexed<u32>(as); }

//****************************************************************************************
// base+disp load/store with immediate base update (after)
//****************************************************************************************
#define CHECK_BASE_UPDATE_INSN                                               \
    if (&ra() == &rb()) {                                                    \
        LOG_ERROR(LOG_EXCEPTION) << "target and base registers are same";    \
        return Result::illegal_instruction;                                 \
    }

template<typename T>
ALWAYS_INLINE
Result
Core::LoadModifyImmediateAfter(AddressSpace& as)
{
    CHECK_BASE_UPDATE_INSN
    const VirtAddress address(rb().base()); // + cmd.GetIndexedDisplacement();
    T value;
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    rb().base() += cmd.imm;
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld_mia_u8)   { return core.LoadModifyImmediateAfter<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mia_u16)  { return core.LoadModifyImmediateAfter<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mia_u32)  { return core.LoadModifyImmediateAfter<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mia_u64)  { return core.LoadModifyImmediateAfter<u64> (as); }

DEFINE_INSN_IMPLEMENTATION(ld_mia_i8)   { return core.LoadModifyImmediateAfter<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mia_i16)  { return core.LoadModifyImmediateAfter<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mia_i32)  { return core.LoadModifyImmediateAfter<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mia_i64)  { return core.LoadModifyImmediateAfter<i64> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mia_i128) { return core.LoadModifyImmediateAfter<u128>(as); }

template<typename T> ALWAYS_INLINE Result Core::StoreModifyImmediateAfter(AddressSpace& as)
{
    const T value = ra().scalar<T>();
    const VirtAddress address(rb().base()); // + cmd.GetIndexedDisplacement();
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    rb().base() += cmd.imm;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_mia_i8)   { return core.StoreModifyImmediateAfter<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(st_mia_i16)  { return core.StoreModifyImmediateAfter<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(st_mia_i32)  { return core.StoreModifyImmediateAfter<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(st_mia_i64)  { return core.StoreModifyImmediateAfter<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(st_mia_i128) { return core.StoreModifyImmediateAfter<u128>(as); }

//****************************************************************************************
// base+disp load/store with immediate base update (before)
//****************************************************************************************
template<typename T> ALWAYS_INLINE Result Core::LoadModifyImmediateBefore(AddressSpace& as)
{
    CHECK_BASE_UPDATE_INSN
    const VirtAddress address(rb().base() + cmd.imm);
    T value;
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    rb().base() = address.get_address();
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ld_mib_u8)   { return core.LoadModifyImmediateBefore<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mib_u16)  { return core.LoadModifyImmediateBefore<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mib_u32)  { return core.LoadModifyImmediateBefore<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mib_u64)  { return core.LoadModifyImmediateBefore<u64> (as); }

DEFINE_INSN_IMPLEMENTATION(ld_mib_i8)   { return core.LoadModifyImmediateBefore<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mib_i16)  { return core.LoadModifyImmediateBefore<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mib_i32)  { return core.LoadModifyImmediateBefore<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mib_i64)  { return core.LoadModifyImmediateBefore<i64> (as); }
DEFINE_INSN_IMPLEMENTATION(ld_mib_i128) { return core.LoadModifyImmediateBefore<i128>(as); }

template<typename T> ALWAYS_INLINE Result Core::StoreModifyImmediateBefore(AddressSpace& as)
{
    const T value = ra().scalar<T>();
    const VirtAddress address(rb().base() + cmd.imm);
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
    rb().base() = address.get_address();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(st_mib_i8)   { return core.StoreModifyImmediateBefore<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(st_mib_i16)  { return core.StoreModifyImmediateBefore<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(st_mib_i32)  { return core.StoreModifyImmediateBefore<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(st_mib_i64)  { return core.StoreModifyImmediateBefore<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(st_mib_i128) { return core.StoreModifyImmediateBefore<u128>(as); }

//****************************************************************************************
// compare register-register and branch
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::CompareRegRegAndBranch(AddressSpace& as)
{
    bool const condition = OP::func( ra().scalar<T>(), rb().scalar<T>() );
    if (condition) {
        const u64 address = ip + sizeof(Bundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);

        return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

template<typename T, typename OP> ALWAYS_INLINE Result Core::CompareAndTrap(void)
{
    bool const condition = OP::func( ra().scalar<T>(), T(rb().base() + cmd.imm) );
    if (condition) {
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH) << "taken program trap";
        return Result::program_trap;
    }
    return FinalizeInstruction();
}

//****************************************************************************************
// compare register-immediate and branch
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::CompareRegSimmAndBranch(AddressSpace& as)
{
    bool const condition = OP::func(ra().scalar<T>(), static_cast<T>(cmd.brc_imm));
    if (condition) {
        const u64 address = ip + sizeof(Bundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(br_mask_all)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_all   > (as); }
DEFINE_INSN_IMPLEMENTATION(br_mask_any)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_any   > (as); }
DEFINE_INSN_IMPLEMENTATION(br_mask_none)   { return core.CompareRegSimmAndBranch<u64, cmp_mask_none  > (as); }
DEFINE_INSN_IMPLEMENTATION(br_mask_notall) { return core.CompareRegSimmAndBranch<u64, cmp_mask_notall> (as); }

//****************************************************************************************
// compare-register-bit-and-branch instructions group
//****************************************************************************************
ALWAYS_INLINE Result Core::DoConditionalBranch(AddressSpace& as, bool condition)
{
    if (condition) {
        const u64 address = ip + sizeof(Bundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "taken branch=" << fmt::hex(address);
        return ExecuteJump(as, address);
    } else {
        return FinalizeInstruction();
    }
}

DEFINE_INSN_IMPLEMENTATION(br_bc) { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION(br_bs) { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) != 0); }

DEFINE_INSN_IMPLEMENTATION(br_bc_imm) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION(br_bs_imm) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) != 0); }

//****************************************************************************************
// add/subtract, compare register-register and branch
//****************************************************************************************
template<typename T, typename CMP, typename OP> ALWAYS_INLINE Result
Core::RepeatBranch(AddressSpace& as)
{
    GeneralRegister & a = ra();

    const T bb = rb().scalar<T>();
    const T old_aa = a.scalar<T>();
    const T new_aa = OP::template func<T>(old_aa, cmd.brc_imm);
    a = new_aa;

    if (CMP::func(new_aa, bb)) {
        const u64 address = ip + sizeof(Bundle) * cmd.imm;
        LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
            << "loop branch=" << fmt::hex(address);
       return ExecuteJump(as, address);
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(rep_le_i64) { return core.RepeatBranch<i64, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_lt_i64) { return core.RepeatBranch<i64, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_ge_i64) { return core.RepeatBranch<i64, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_gt_i64) { return core.RepeatBranch<i64, cmp_gt, subtract> (as); }

DEFINE_INSN_IMPLEMENTATION(rep_le_u64) { return core.RepeatBranch<u64, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_lt_u64) { return core.RepeatBranch<u64, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_ge_u64) { return core.RepeatBranch<u64, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_gt_u64) { return core.RepeatBranch<u64, cmp_gt, subtract> (as); }

DEFINE_INSN_IMPLEMENTATION(rep_le_i32) { return core.RepeatBranch<i32, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_lt_i32) { return core.RepeatBranch<i32, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_ge_i32) { return core.RepeatBranch<i32, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_gt_i32) { return core.RepeatBranch<i32, cmp_gt, subtract> (as); }

DEFINE_INSN_IMPLEMENTATION(rep_le_u32) { return core.RepeatBranch<u32, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_lt_u32) { return core.RepeatBranch<u32, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_ge_u32) { return core.RepeatBranch<u32, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(rep_gt_u32) { return core.RepeatBranch<u32, cmp_gt, subtract> (as); }

//****************************************************************************************
// branches, ip-relative
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(ld_imm)
{
    const u64 imm = core.cmd.imm;
    core.ra() = imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ld_imm_high) // replace high part of 128 bit
{
    const u64 imm = core.cmd.imm;
    core.ra().vector<u64>(1) = imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(jmp)
{
    const u64 address = core.ip + sizeof(Bundle) * core.cmd.imm;
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
template<typename T> ALWAYS_INLINE Result Core::tabular_branch(AddressSpace& as)
{
    const GeneralRegister& b = rb();
    const GeneralRegister& c = rc();

    u64 const table_address = b.base();
    i64 const index = static_cast<i64>(c.scalar<T>());
    const VirtAddress address(table_address + index * sizeof(i32));
    i32 offset;
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_read(*this, address, offset));

    u64 const target_address = table_address + static_cast<i64>(offset);

    LOG_DEBUG(LOG_EMULATOR | LOG_BRANCH)
        << "taken jump=" << fmt::hex(target_address);

    return ExecuteJump(as, target_address);
}

DEFINE_INSN_IMPLEMENTATION(jmp_t)     { return core.tabular_branch<i64>(as); }
DEFINE_INSN_IMPLEMENTATION(jmp_t_i32) { return core.tabular_branch<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(jmp_t_u32) { return core.tabular_branch<u32>(as); }

//****************************************************************************************
// reg + shamt
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::BinaryRegShamt(void)
{
    const T value = OP::func(rb().scalar<T>(), static_cast<unsigned>(cmd.rc));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(cnt_tz)  { return core.BinaryRegShamt< u64, trailing_zeros            > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cnt_lz)  { return core.BinaryRegShamt< u64, leading_zeros             > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cnt_pop) { return core.BinaryRegShamt< u64, population                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(permb)   { return core.BinaryRegShamt< u64, permute                   > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(bit_set_imm)   { return core.BinaryRegShamt< u128, bit_set_immediate > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bit_clear_imm) { return core.BinaryRegShamt< u128, bit_clear_immediate > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bit_flip_imm)  { return core.BinaryRegShamt< u128, bit_flip_immediate > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(gtb)
{
    core.ra() = util::gray2bin( core.rb().scalar<u64>() );
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(deposit_r)
{
    const GeneralRegister& b = core.rb();
    const GeneralRegister& c = core.rc();
    const GeneralRegister& d = core.rd();

    core.ra() = deposit(b, c, d.template scalar<i32>());
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(perm)
{
    const GeneralRegister& b = core.rb();
    const GeneralRegister& c = core.rc();
    const GeneralRegister& d = core.rd();

    GeneralRegister tmp;

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(alignup_u64)
{
    unsigned const shift = core.cmd.rc;
    unsigned const shamt = core.cmd.rd;

    u64 const mask = util::makemask<u64>(shamt);
    u64 const src = core.rb().base();
    u64 const dst = ((src << shift) + mask) & (~mask);

    LOG_DEBUG(LOG_EMULATOR)
        << "alignup_u64: src=" << fmt::hex(src)
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

template<typename T, typename OP> ALWAYS_INLINE Result Core::ConditionalMove(void)
{
    bool const condition = OP::func(rb().scalar<T>(), static_cast<T>(0));
    ra() = condition ? rc() : rd();
    return FinalizeInstruction();
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

#define BASE_SCALAR(STYPE, UTYPE) \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_        ## STYPE) { return core.unary_reg <STYPE, absolute_value> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(neg_        ## STYPE) { return core.unary_reg <STYPE, neg> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(abs_diff_   ## STYPE) { return core.binary_reg_reg <STYPE, absolute_difference> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(add_        ## STYPE) { return core.binary_reg_reg <STYPE, add> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sub_        ## STYPE) { return core.binary_reg_reg <STYPE, subtract> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(mul_        ## STYPE) { return core.binary_reg_reg <STYPE, mul> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(div_        ## STYPE) { return core.binary_reg_reg_divmod <STYPE, div > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(div_        ## UTYPE) { return core.binary_reg_reg_divmod <UTYPE, div > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(rem_        ## STYPE) { return core.binary_reg_reg_divmod <STYPE, mod > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(rem_        ## UTYPE) { return core.binary_reg_reg_divmod <UTYPE, mod > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(max_        ## STYPE) { return core.binary_reg_reg <STYPE, max> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(max_        ## UTYPE) { return core.binary_reg_reg <UTYPE, max> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(min_        ## STYPE) { return core.binary_reg_reg <STYPE, min> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(min_        ## UTYPE) { return core.binary_reg_reg <UTYPE, min> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sll_        ## UTYPE) { return core.binary_reg_reg <UTYPE, shift_left > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(srl_        ## UTYPE) { return core.binary_reg_reg <UTYPE, shift_right> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sra_        ## STYPE) { return core.binary_reg_reg <STYPE, shift_right> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2_      ## STYPE) { return core.binary_reg_reg <STYPE, divide_pow2> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sll_imm_    ## UTYPE) { return core.BinaryRegShamt<UTYPE, shift_left_immediate> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(srl_imm_    ## UTYPE) { return core.BinaryRegShamt<UTYPE, shift_right_immediate> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(sra_imm_    ## STYPE) { return core.BinaryRegShamt<STYPE, shift_right_immediate> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2_imm_  ## STYPE) { return core.BinaryRegShamt<STYPE, divide_pow2_immediate> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(srp_        ## STYPE) { return core.fused_reg_reg_reg <UTYPE, shift_right_pair > (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(slp_        ## STYPE) { return core.fused_reg_reg_reg <UTYPE, shift_left_pair> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_eq_     ## STYPE) { return core.BinaryRegRegBool <UTYPE, cmp_eq> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ne_     ## STYPE) { return core.BinaryRegRegBool <UTYPE, cmp_ne> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_lt_     ## STYPE) { return core.BinaryRegRegBool <STYPE, cmp_lt> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_lt_     ## UTYPE) { return core.BinaryRegRegBool <UTYPE, cmp_lt> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ge_     ## STYPE) { return core.BinaryRegRegBool <STYPE, cmp_ge> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ge_     ## UTYPE) { return core.BinaryRegRegBool <UTYPE, cmp_ge> (); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_eq_imm_ ## STYPE) { return core.BinaryRegSimm<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ne_imm_ ## STYPE) { return core.BinaryRegSimm<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_lt_imm_ ## STYPE) { return core.BinaryRegSimm<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_lt_imm_ ## UTYPE) { return core.BinaryRegSimm<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ge_imm_ ## STYPE) { return core.BinaryRegSimm<STYPE, cmp_ge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmp_ge_imm_ ## UTYPE) { return core.BinaryRegSimm<UTYPE, cmp_ge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_eq_    ## STYPE) { return core.ConditionalMove<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_lt_    ## STYPE) { return core.ConditionalMove<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(cmov_le_    ## STYPE) { return core.ConditionalMove<STYPE, cmp_le>(); } \
    DEFINE_INSN_IMPLEMENTATION      (br_eq_      ## STYPE) { return core.CompareRegRegAndBranch<STYPE, cmp_eq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ne_      ## STYPE) { return core.CompareRegRegAndBranch<STYPE, cmp_ne>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_lt_      ## STYPE) { return core.CompareRegRegAndBranch<STYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_lt_      ## UTYPE) { return core.CompareRegRegAndBranch<UTYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ge_      ## STYPE) { return core.CompareRegRegAndBranch<STYPE, cmp_ge>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ge_      ## UTYPE) { return core.CompareRegRegAndBranch<UTYPE, cmp_ge>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_eq_imm_  ## STYPE) { return core.CompareRegSimmAndBranch<STYPE, cmp_eq>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ne_imm_  ## STYPE) { return core.CompareRegSimmAndBranch<STYPE, cmp_ne>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_lt_imm_  ## STYPE) { return core.CompareRegSimmAndBranch<STYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_lt_imm_  ## UTYPE) { return core.CompareRegSimmAndBranch<UTYPE, cmp_lt>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ge_imm_  ## STYPE) { return core.CompareRegSimmAndBranch<STYPE, cmp_ge>(as); } \
    DEFINE_INSN_IMPLEMENTATION      (br_ge_imm_  ## UTYPE) { return core.CompareRegSimmAndBranch<UTYPE, cmp_ge>(as); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_eq_     ## STYPE) { return core.NullifyConditionallyRegReg<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ne_     ## STYPE) { return core.NullifyConditionallyRegReg<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_lt_     ## STYPE) { return core.NullifyConditionallyRegReg<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_lt_     ## UTYPE) { return core.NullifyConditionallyRegReg<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ge_     ## STYPE) { return core.NullifyConditionallyRegReg<STYPE, cmp_ge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ge_     ## UTYPE) { return core.NullifyConditionallyRegReg<UTYPE, cmp_ge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_eq_imm_ ## STYPE) { return core.NullifyConditionallyRegSimm<STYPE, cmp_eq>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ne_imm_ ## STYPE) { return core.NullifyConditionallyRegSimm<STYPE, cmp_ne>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_lt_imm_ ## STYPE) { return core.NullifyConditionallyRegSimm<STYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_lt_imm_ ## UTYPE) { return core.NullifyConditionallyRegSimm<UTYPE, cmp_lt>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ge_imm_ ## STYPE) { return core.NullifyConditionallyRegSimm<STYPE, cmp_ge>(); } \
    DEFINE_INSN_IMPLEMENTATION_NOMEM(nul_ge_imm_ ## UTYPE) { return core.NullifyConditionallyRegSimm<UTYPE, cmp_ge>(); } \

BASE_SCALAR(i32,  u32)
BASE_SCALAR(i64,  u64)
BASE_SCALAR(i128, u128)

DEFINE_PSEUDO(ld_imm_f32)
DEFINE_PSEUDO(ld_imm_f64)
DEFINE_PSEUDO(ld_iprel_f32)
DEFINE_PSEUDO(ld_iprel_f64)
DEFINE_PSEUDO(ld_iprel_f128)

} // namespace postrisc
