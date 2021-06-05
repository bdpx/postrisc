#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldan)
{
    u64 const address = core.rb().base() + (core.cmd.imm << bits_per_lda_near_shift);
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldanrc)
{
    u64 const address = core.ip + core.rb().base() + (core.cmd.imm << bits_per_lda_near_shift);
    core.ra() = address;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldarc)
{
    core.ra() = core.ip + sizeof(Bundle) * core.cmd.imm;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ldard)
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
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsraid)
{
    const unsigned left_shift  = core.cmd.rc;
    const unsigned right_shift = core.cmd.rd;

    core.ra() = sra<i64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrlid)
{
    const unsigned left_shift  = core.cmd.rc;
    const unsigned right_shift = core.cmd.rd;

    core.ra() = srl<u64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrad)
{
    const unsigned left_shift  = get_shift_amount(core.rc().scalar<u64>());
    const unsigned right_shift = get_shift_amount(core.rd().scalar<u64>());

    core.ra() = sra<i64>(sll<u64>(core.rb().scalar<u64>(), left_shift), right_shift);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(slsrld)
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
template<typename T, typename OP> ALWAYS_INLINE Result Core::fused_reg_reg_shamt(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>(), cmd.rd );
    ra() = value;
    return FinalizeInstruction();
}

// shift left and (add|subtract[reverse]|or|xor)
DEFINE_INSN_IMPLEMENTATION_NOMEM(sladdd)   { return core.fused_reg_reg_shamt<u64, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sladdws)  { return core.fused_reg_reg_shamt<i32, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sladdwz)  { return core.fused_reg_reg_shamt<u32, shifted_add         >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsubd)   { return core.fused_reg_reg_shamt<u64, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsubws)  { return core.fused_reg_reg_shamt<i32, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsubwz)  { return core.fused_reg_reg_shamt<u32, shifted_subtract    >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsubrd)  { return core.fused_reg_reg_shamt<u64, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsubrws) { return core.fused_reg_reg_shamt<i32, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slsubrwz) { return core.fused_reg_reg_shamt<u32, shifted_subtract_rev>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slor)     { return core.fused_reg_reg_shamt<u64, shifted_or          >(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slxor)    { return core.fused_reg_reg_shamt<u64, shifted_xor         >(); }

// fused shift right pair
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpiw) { return core.fused_reg_reg_shamt < u32, shift_right_pair_imm > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpid) { return core.fused_reg_reg_shamt < u64, shift_right_pair_imm > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpiq) { return core.fused_reg_reg_shamt < u128,shift_right_pair_imm > (); }

//****************************************************************************************
// fused (multiply|add)-(add|subtract[from])
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::fused_reg_reg_reg(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>(), rd().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(muladdd)  {  return core.fused_reg_reg_reg <u64, muladd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulsubd)  {  return core.fused_reg_reg_reg <u64, mulsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulsubrd) {  return core.fused_reg_reg_reg <u64, mulsub_rev       > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(addaddd) { return core.fused_reg_reg_reg <i64, addadd           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addsubd) { return core.fused_reg_reg_reg <i64, addsub           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subsubd) { return core.fused_reg_reg_reg <i64, subsub           > (); }
// select
DEFINE_INSN_IMPLEMENTATION_NOMEM(bitslct) { return core.fused_reg_reg_reg <u128, bitwise_select  > (); }


//****************************************************************************************
// multiprecision arithmetic
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(addcd)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();

    core.ra() = (static_cast<u128>(vb) + static_cast<u128>(vc));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(subcd)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();

    core.ra() = (static_cast<u128>(vb) - static_cast<u128>(vc));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(addc3d)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();
    const u128 vd = core.rd().scalar<u128>();

    core.ra() = (static_cast<u128>(vb) + static_cast<u128>(vc) + (vd >> 64));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(subc3d)
{
    const u64 vb = core.rb().scalar<u64>();
    const u64 vc = core.rc().scalar<u64>();
    const i128 vd = core.rd().scalar<u128>();

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(sextb) { return core.unary_reg <i8,  sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sexth) { return core.unary_reg <i16, sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sextw) { return core.unary_reg <i32, sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sextd) { return core.unary_reg <i64, sign_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zextb) { return core.unary_reg <u8,  zero_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zexth) { return core.unary_reg <u16, zero_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zextw) { return core.unary_reg <u32, zero_extend> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(zextd) { return core.unary_reg <u64, zero_extend> (); }

//****************************************************************************************
// binary reg-reg
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::binary_reg_reg(void)
{
    const T value = OP::func( rb().scalar<T>(), rc().scalar<T>() );
    ra() = value;
    return FinalizeInstruction();
}

// bitwise
DEFINE_INSN_IMPLEMENTATION_NOMEM(and)  { return core.binary_reg_reg <u128, bitwise_and > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(or)   { return core.binary_reg_reg <u128, bitwise_or  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xor)  { return core.binary_reg_reg <u128, bitwise_xor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xnor) { return core.binary_reg_reg <u128, bitwise_xnor> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nand) { return core.binary_reg_reg <u128, bitwise_nand> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nor)  { return core.binary_reg_reg <u128, bitwise_nor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(andn) { return core.binary_reg_reg <u128, bitwise_andn> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(orn)  { return core.binary_reg_reg <u128, bitwise_orn > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(bitset)  { return core.binary_reg_reg<u128, bit_set  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bitclr)  { return core.binary_reg_reg<u128, bit_clear> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bitflip) { return core.binary_reg_reg<u128, bit_flip > (); }

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

DEFINE_INSN_IMPLEMENTATION_NOMEM(andneg) { return core.binary_reg_reg < u64, bitwise_andneg> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xordec) { return core.binary_reg_reg < u64, bitwise_xordec> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(anddec) { return core.binary_reg_reg < u64, bitwise_anddec> (); }

// like bmi2 pdep & pext
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbscat) { return core.binary_reg_reg < u64, bit_scatter> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mbgath) { return core.binary_reg_reg < u64, bit_gather> (); }

//****************************************************************************************
// binary register-immediate
//****************************************************************************************

DEFINE_INSN_IMPLEMENTATION_NOMEM(addid)  { return core.BinaryRegSimm< i64,  add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addiws) { return core.BinaryRegSimm< i32,  add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addiwz) { return core.BinaryRegSimm< u32,  add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addiq)  { return core.BinaryRegSimm< i128, add> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(subrid)  {  return core.BinaryRegSimm< i64, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subriws) {  return core.BinaryRegSimm< i32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subriwz) {  return core.BinaryRegSimm< u32, subtract_rev> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulid)   {  return core.BinaryRegSimm< i64, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(muliws)  {  return core.BinaryRegSimm< i32, mul         > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(muliwz)  {  return core.BinaryRegSimm< u32, mul         > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsid) { return core.BinaryRegSimm<i64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsid) { return core.BinaryRegSimm<i64, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxuid) { return core.BinaryRegSimm<u64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minuid) { return core.BinaryRegSimm<u64, min> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsiw) { return core.BinaryRegSimm<i32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsiw) { return core.BinaryRegSimm<i32, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxuiw) { return core.BinaryRegSimm<u32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minuiw) { return core.BinaryRegSimm<u32, min> (); }

// binary register-immediate logical
DEFINE_INSN_IMPLEMENTATION_NOMEM(andi) {     return core.BinaryRegSimm< i128, bitwise_and > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(ori) {      return core.BinaryRegSimm< i128, bitwise_or  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(xori) {     return core.BinaryRegSimm< i128, bitwise_xor > (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(xnor_imm) {    return core.BinaryRegSimm< i128, bitwise_xnor> (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(nand_imm) {    return core.BinaryRegSimm< i128, bitwise_nand> (); }
// DEFINE_INSN_IMPLEMENTATION_NOMEM(nor_imm) {     return core.BinaryRegSimm< i128, bitwise_nor > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(andni) {    return core.BinaryRegSimm< i128, bitwise_andn> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(orni) {     return core.BinaryRegSimm< i128, bitwise_orn > (); }


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

DEFINE_INSN_IMPLEMENTATION_NOMEM(divsid) { return core.binary_reg_simm_divmod< i64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remsid) { return core.binary_reg_simm_divmod< i64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divuid) { return core.binary_reg_simm_divmod< u64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remuid) { return core.binary_reg_simm_divmod< u64, mod > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(divsiw) { return core.binary_reg_simm_divmod< i32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remsiw) { return core.binary_reg_simm_divmod< i32, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divuiw) { return core.binary_reg_simm_divmod< u32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remuiw) { return core.binary_reg_simm_divmod< u32, mod > (); }

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
DEFINE_INSN_IMPLEMENTATION(allocsp)
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

DEFINE_INSN_IMPLEMENTATION(jmpr)
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

DEFINE_INSN_IMPLEMENTATION(jmpmi)
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

DEFINE_INSN_IMPLEMENTATION(callri)
{
    u64 const ea = core.rb().base() + core.rc().base();
    u64 const address = RoundToBundle(ea);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(callmi)
{
    const VirtAddress ea(core.rb().base() + core.cmd.imm);
    u64 loaded;
    CHECK_RESULT_ADDRESS(core, ea, as.mmu_read_u64/*FIXME:_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(callrvt)
{
    u64 const base = core.rb().base();
    const VirtAddress ea(base + core.cmd.imm);
    i32 loaded;
    CHECK_RESULT_ADDRESS(core, ea, as.mmu_read/*FIXME:_i32_finalized*/(core, ea, loaded));
    u64 const address = RoundToBundle(base + loaded);
    LOG_DEBUG(LOG_CALLSTACK) << "call=" << fmt::hex(address);
    return core.CallSubroutine(as, core.cmd.ra, core.ra(), address);
}

DEFINE_INSN_IMPLEMENTATION(callplt)
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

DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbc) { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbs) { return core.NullifyConditionally(core.ra().get_bit(core.rb().base() & 127) != 0); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbci) { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulbsi) { return core.NullifyConditionally(core.ra().get_bit(core.cmd.rb) != 0); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmall)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_all   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmany)    { return core.NullifyConditionallyRegSimm<u64, cmp_mask_any   > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmnone)   { return core.NullifyConditionallyRegSimm<u64, cmp_mask_none  > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulmnotall) { return core.NullifyConditionallyRegSimm<u64, cmp_mask_notall> (); }

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

DEFINE_INSN_IMPLEMENTATION(ldub) { return core.read_base<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(lduh) { return core.read_base<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(lduw) { return core.read_base<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldud) { return core.read_base<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsb) { return core.read_base<i8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsh) { return core.read_base<i16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsw) { return core.read_base<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsd) { return core.read_base<i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldq)  { return core.read_base<i128>(as); }

template<typename T> ALWAYS_INLINE Result Core::write_base(AddressSpace& as)
{
    const VirtAddress address(rb().base() + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
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
template<typename T> ALWAYS_INLINE Result Core::read_ip_relative(AddressSpace& as)
{
    const VirtAddress address(ip + cmd.imm);
    T value;
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_read(*this, address, value));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ldubr) { return core.read_ip_relative<u8>(as); }
DEFINE_INSN_IMPLEMENTATION(lduhr) { return core.read_ip_relative<u16>(as); }
DEFINE_INSN_IMPLEMENTATION(lduwr) { return core.read_ip_relative<u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldudr) { return core.read_ip_relative<u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsbr) { return core.read_ip_relative<i8>(as); }
DEFINE_INSN_IMPLEMENTATION(ldshr) { return core.read_ip_relative<i16>(as); }
DEFINE_INSN_IMPLEMENTATION(ldswr) { return core.read_ip_relative<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsdr) { return core.read_ip_relative<i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldqr)  { return core.read_ip_relative<i128>(as); }

template<typename T> ALWAYS_INLINE Result Core::write_ip_relative(AddressSpace& as)
{
    const VirtAddress address(ip + cmd.imm);
    const T value = ra().scalar<T>();
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
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

DEFINE_INSN_IMPLEMENTATION(ldubxsd) { return core.read_indexed<u8,    i64>(as); }
DEFINE_INSN_IMPLEMENTATION(lduhxsd) { return core.read_indexed<u16,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(lduwxsd) { return core.read_indexed<u32,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldudxsd) { return core.read_indexed<u64,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsbxsd) { return core.read_indexed<i8,    i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldshxsd) { return core.read_indexed<i16,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldswxsd) { return core.read_indexed<i32,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsdxsd) { return core.read_indexed<i64,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldqxsd)  { return core.read_indexed<i128,  i64>(as); }

DEFINE_INSN_IMPLEMENTATION(stbxsd) { return core.write_indexed<u8,   i64>(as); }
DEFINE_INSN_IMPLEMENTATION(sthxsd) { return core.write_indexed<u16,  i64>(as); }
DEFINE_INSN_IMPLEMENTATION(stwxsd) { return core.write_indexed<u32,  i64>(as); }
DEFINE_INSN_IMPLEMENTATION(stdxsd) { return core.write_indexed<u64,  i64>(as); }
DEFINE_INSN_IMPLEMENTATION(stqxsd) { return core.write_indexed<u128, i64>(as); }

DEFINE_INSN_IMPLEMENTATION(ldaxsd) { return core.compute_address_indexed<i64>(as); }

DEFINE_INSN_IMPLEMENTATION(ldubxsw) { return core.read_indexed<u8,    i32>(as); }
DEFINE_INSN_IMPLEMENTATION(lduhxsw) { return core.read_indexed<u16,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(lduwxsw) { return core.read_indexed<u32,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldudxsw) { return core.read_indexed<u64,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsbxsw) { return core.read_indexed<i8,    i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldshxsw) { return core.read_indexed<i16,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldswxsw) { return core.read_indexed<i32,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsdxsw) { return core.read_indexed<i64,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldqxsw)  { return core.read_indexed<i128,  i32>(as); }

DEFINE_INSN_IMPLEMENTATION(stbxsw) { return core.write_indexed<u8,   i32>(as); }
DEFINE_INSN_IMPLEMENTATION(sthxsw) { return core.write_indexed<u16,  i32>(as); }
DEFINE_INSN_IMPLEMENTATION(stwxsw) { return core.write_indexed<u32,  i32>(as); }
DEFINE_INSN_IMPLEMENTATION(stdxsw) { return core.write_indexed<u64,  i32>(as); }
DEFINE_INSN_IMPLEMENTATION(stqxsw) { return core.write_indexed<u128, i32>(as); }

DEFINE_INSN_IMPLEMENTATION(ldaxsw) { return core.compute_address_indexed<i32>(as); }

DEFINE_INSN_IMPLEMENTATION(ldubxud) { return core.read_indexed<u8,    u64>(as); }
DEFINE_INSN_IMPLEMENTATION(lduhxud) { return core.read_indexed<u16,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(lduwxud) { return core.read_indexed<u32,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldudxud) { return core.read_indexed<u64,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsbxud) { return core.read_indexed<i8,    u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldshxud) { return core.read_indexed<i16,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldswxud) { return core.read_indexed<i32,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsdxud) { return core.read_indexed<i64,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(ldqxud)  { return core.read_indexed<i128,  u64>(as); }

DEFINE_INSN_IMPLEMENTATION(stbxud) { return core.write_indexed<u8,   u64>(as); }
DEFINE_INSN_IMPLEMENTATION(sthxud) { return core.write_indexed<u16,  u64>(as); }
DEFINE_INSN_IMPLEMENTATION(stwxud) { return core.write_indexed<u32,  u64>(as); }
DEFINE_INSN_IMPLEMENTATION(stdxud) { return core.write_indexed<u64,  u64>(as); }
DEFINE_INSN_IMPLEMENTATION(stqxud) { return core.write_indexed<u128, u64>(as); }

DEFINE_INSN_IMPLEMENTATION(ldaxud) { return core.compute_address_indexed<u64>(as); }

DEFINE_INSN_IMPLEMENTATION(ldubxuw) { return core.read_indexed<u8,    u32>(as); }
DEFINE_INSN_IMPLEMENTATION(lduhxuw) { return core.read_indexed<u16,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(lduwxuw) { return core.read_indexed<u32,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldudxuw) { return core.read_indexed<u64,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsbxuw) { return core.read_indexed<i8,    u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldshxuw) { return core.read_indexed<i16,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldswxuw) { return core.read_indexed<i32,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldsdxuw) { return core.read_indexed<i64,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(ldqxuw)  { return core.read_indexed<i128,  u32>(as); }

DEFINE_INSN_IMPLEMENTATION(stbxuw) { return core.write_indexed<u8,   u32>(as); }
DEFINE_INSN_IMPLEMENTATION(sthxuw) { return core.write_indexed<u16,  u32>(as); }
DEFINE_INSN_IMPLEMENTATION(stwxuw) { return core.write_indexed<u32,  u32>(as); }
DEFINE_INSN_IMPLEMENTATION(stdxuw) { return core.write_indexed<u64,  u32>(as); }
DEFINE_INSN_IMPLEMENTATION(stqxuw) { return core.write_indexed<u128, u32>(as); }

DEFINE_INSN_IMPLEMENTATION(ldaxuw) { return core.compute_address_indexed<u32>(as); }

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

DEFINE_INSN_IMPLEMENTATION(ldubmia) { return core.LoadModifyImmediateAfter<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(lduhmia) { return core.LoadModifyImmediateAfter<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(lduwmia) { return core.LoadModifyImmediateAfter<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(ldudmia) { return core.LoadModifyImmediateAfter<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ldsbmia) { return core.LoadModifyImmediateAfter<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldshmia) { return core.LoadModifyImmediateAfter<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldswmia) { return core.LoadModifyImmediateAfter<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(ldsdmia) { return core.LoadModifyImmediateAfter<i64> (as); }
DEFINE_INSN_IMPLEMENTATION(ldqmia)  { return core.LoadModifyImmediateAfter<u128>(as); }

template<typename T> ALWAYS_INLINE Result Core::StoreModifyImmediateAfter(AddressSpace& as)
{
    const T value = ra().scalar<T>();
    const VirtAddress address(rb().base()); // + cmd.GetIndexedDisplacement();
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
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

DEFINE_INSN_IMPLEMENTATION(ldubmib) { return core.LoadModifyImmediateBefore<u8 > (as); }
DEFINE_INSN_IMPLEMENTATION(lduhmib) { return core.LoadModifyImmediateBefore<u16> (as); }
DEFINE_INSN_IMPLEMENTATION(lduwmib) { return core.LoadModifyImmediateBefore<u32> (as); }
DEFINE_INSN_IMPLEMENTATION(ldudmib) { return core.LoadModifyImmediateBefore<u64> (as); }
DEFINE_INSN_IMPLEMENTATION(ldsbmib) { return core.LoadModifyImmediateBefore<i8 > (as); }
DEFINE_INSN_IMPLEMENTATION(ldshmib) { return core.LoadModifyImmediateBefore<i16> (as); }
DEFINE_INSN_IMPLEMENTATION(ldswmib) { return core.LoadModifyImmediateBefore<i32> (as); }
DEFINE_INSN_IMPLEMENTATION(ldsdmib) { return core.LoadModifyImmediateBefore<i64> (as); }
DEFINE_INSN_IMPLEMENTATION(ldqmib)  { return core.LoadModifyImmediateBefore<i128>(as); }

template<typename T> ALWAYS_INLINE Result Core::StoreModifyImmediateBefore(AddressSpace& as)
{
    const T value = ra().scalar<T>();
    const VirtAddress address(rb().base() + cmd.imm);
    CHECK_RESULT_ADDRESS(*this, address, as.mmu_write(*this, address, value));
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

DEFINE_INSN_IMPLEMENTATION(brmall)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_all   > (as); }
DEFINE_INSN_IMPLEMENTATION(brmany)    { return core.CompareRegSimmAndBranch<u64, cmp_mask_any   > (as); }
DEFINE_INSN_IMPLEMENTATION(brmnone)   { return core.CompareRegSimmAndBranch<u64, cmp_mask_none  > (as); }
DEFINE_INSN_IMPLEMENTATION(brmnotall) { return core.CompareRegSimmAndBranch<u64, cmp_mask_notall> (as); }

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

DEFINE_INSN_IMPLEMENTATION(brbc) { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) == 0); }
DEFINE_INSN_IMPLEMENTATION(brbs) { return core.DoConditionalBranch(as, core.ra().get_bit(core.rb().base() & 127) != 0); }

DEFINE_INSN_IMPLEMENTATION(brbci) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) == 0); }
DEFINE_INSN_IMPLEMENTATION(brbsi) { return core.DoConditionalBranch(as, core.ra().get_bit(core.cmd.rb) != 0); }

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

DEFINE_INSN_IMPLEMENTATION(repled) { return core.RepeatBranch<i64, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repltd) { return core.RepeatBranch<i64, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repged) { return core.RepeatBranch<i64, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(repgtd) { return core.RepeatBranch<i64, cmp_gt, subtract> (as); }

DEFINE_INSN_IMPLEMENTATION(repleud) { return core.RepeatBranch<u64, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repltud) { return core.RepeatBranch<u64, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repgeud) { return core.RepeatBranch<u64, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(repgtud) { return core.RepeatBranch<u64, cmp_gt, subtract> (as); }

DEFINE_INSN_IMPLEMENTATION(replew) { return core.RepeatBranch<i32, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repltw) { return core.RepeatBranch<i32, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repgew) { return core.RepeatBranch<i32, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(repgtw) { return core.RepeatBranch<i32, cmp_gt, subtract> (as); }

DEFINE_INSN_IMPLEMENTATION(repleuw) { return core.RepeatBranch<u32, cmp_le, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repltuw) { return core.RepeatBranch<u32, cmp_lt, add> (as); }
DEFINE_INSN_IMPLEMENTATION(repgeuw) { return core.RepeatBranch<u32, cmp_ge, subtract> (as); }
DEFINE_INSN_IMPLEMENTATION(repgtuw) { return core.RepeatBranch<u32, cmp_gt, subtract> (as); }

//****************************************************************************************
// branches, ip-relative
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(ldi)
{
    const i64 imm = core.cmd.imm;
    core.ra() = static_cast<i128>(imm);
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

DEFINE_INSN_IMPLEMENTATION(jmpt)   { return core.tabular_branch<i64>(as); }
DEFINE_INSN_IMPLEMENTATION(jmptsw) { return core.tabular_branch<i32>(as); }
DEFINE_INSN_IMPLEMENTATION(jmptuw) { return core.tabular_branch<u32>(as); }

//****************************************************************************************
// reg + shamt
//****************************************************************************************
template<typename T, typename OP> ALWAYS_INLINE Result Core::BinaryRegShamt(void)
{
    const T value = OP::func(rb().scalar<T>(), static_cast<unsigned>(cmd.rc));
    ra() = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(cnttz)  { return core.BinaryRegShamt< u64, trailing_zeros            > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cntlz)  { return core.BinaryRegShamt< u64, leading_zeros             > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cntpop) { return core.BinaryRegShamt< u64, population                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(permb)  { return core.BinaryRegShamt< u64, permute                   > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(bitseti)  { return core.BinaryRegShamt< u128, bit_set_immediate > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bitclri)  { return core.BinaryRegShamt< u128, bit_clear_immediate > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(bitflipi) { return core.BinaryRegShamt< u128, bit_flip_immediate > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(gtb)
{
    core.ra() = util::gray2bin( core.rb().scalar<u64>() );
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(depositv)
{
    const GeneralRegister& b = core.rb();
    const GeneralRegister& c = core.rc();
    const GeneralRegister& d = core.rd();

    core.ra() = deposit(b, c, d.template scalar<i32>());
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(alignup)
{
    unsigned const shift = core.cmd.rc;
    unsigned const shamt = core.cmd.rd;

    u64 const mask = util::makemask<u64>(shamt);
    u64 const src = core.rb().base();
    u64 const dst = ((src << shift) + mask) & (~mask);

    LOG_DEBUG(LOG_EMULATOR)
        << "alignup: src=" << fmt::hex(src)
        << " dst=" << fmt::hex(dst);

    core.ra() = dst;
    return core.FinalizeInstruction();
}

//****************************************************************************************
// conditional moves
//****************************************************************************************
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmovlsb)
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

DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsw) { return core.binary_reg_reg <i32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxuw) { return core.binary_reg_reg <u32, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsw) { return core.binary_reg_reg <i32, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minuw) { return core.binary_reg_reg <u32, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsd) { return core.binary_reg_reg <i64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxud) { return core.binary_reg_reg <u64, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsd) { return core.binary_reg_reg <i64, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minud) { return core.binary_reg_reg <u64, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxsq) { return core.binary_reg_reg <i128, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(maxuq) { return core.binary_reg_reg <u128, max> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minsq) { return core.binary_reg_reg <i128, min> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(minuq) { return core.binary_reg_reg <u128, min> (); }


DEFINE_INSN_IMPLEMENTATION_NOMEM(absw) { return core.unary_reg <i32, absolute_value> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(negw) { return core.unary_reg <i32, neg> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(abdw) { return core.binary_reg_reg <i32, absolute_difference> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addws) { return core.binary_reg_reg <i32, add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subws) { return core.binary_reg_reg <i32, subtract> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulws) { return core.binary_reg_reg <i32, mul> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divsw) { return core.binary_reg_reg_divmod <i32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divuw) { return core.binary_reg_reg_divmod <u32, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remsw) { return core.binary_reg_reg_divmod <i32, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remuw) { return core.binary_reg_reg_divmod <u32, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sllw) { return core.binary_reg_reg <u32, shift_left > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srlw) { return core.binary_reg_reg <u32, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraw) { return core.binary_reg_reg <i32, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2w) { return core.binary_reg_reg <i32, divide_pow2> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slliw) { return core.BinaryRegShamt<u32, shift_left_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srliw) { return core.BinaryRegShamt<u32, shift_right_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraiw) { return core.BinaryRegShamt<i32, shift_right_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2iw) { return core.BinaryRegShamt<i32, divide_pow2_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpw) { return core.fused_reg_reg_reg <u32, shift_right_pair > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slpw) { return core.fused_reg_reg_reg <u32, shift_left_pair> (); }
// custom, arithmetic
DEFINE_INSN_IMPLEMENTATION_NOMEM(addwz) { return core.binary_reg_reg < u32, add                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subwz) { return core.binary_reg_reg < u32, subtract           > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulwz) { return core.binary_reg_reg < u32, mul                > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulh)  { return core.binary_reg_reg < u64, multiply_high      > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addod) { return core.binary_reg_reg < u64, add_overflow       > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subod) { return core.binary_reg_reg < u64, subtract_overflow  > (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(absd) { return core.unary_reg <i64, absolute_value> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(negd) { return core.unary_reg <i64, neg> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(abdd) { return core.binary_reg_reg <i64, absolute_difference> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addd) { return core.binary_reg_reg <i64, add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subd) { return core.binary_reg_reg <i64, subtract> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(muld) { return core.binary_reg_reg <i64, mul> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divsd) { return core.binary_reg_reg_divmod <i64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divud) { return core.binary_reg_reg_divmod <u64, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remsd) { return core.binary_reg_reg_divmod <i64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remud) { return core.binary_reg_reg_divmod <u64, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slld) { return core.binary_reg_reg <u64, shift_left > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srld) { return core.binary_reg_reg <u64, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srad) { return core.binary_reg_reg <i64, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2d) { return core.binary_reg_reg <i64, divide_pow2> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sllid) { return core.BinaryRegShamt<u64, shift_left_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srlid) { return core.BinaryRegShamt<u64, shift_right_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraid) { return core.BinaryRegShamt<i64, shift_right_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2id) { return core.BinaryRegShamt<i64, divide_pow2_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpd) { return core.fused_reg_reg_reg <u64, shift_right_pair > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slpd) { return core.fused_reg_reg_reg <u64, shift_left_pair> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(absq) { return core.unary_reg <i128, absolute_value> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(negq) { return core.unary_reg <i128, neg> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(abdq) { return core.binary_reg_reg <i128, absolute_difference> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(addq) { return core.binary_reg_reg <i128, add> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(subq) { return core.binary_reg_reg <i128, subtract> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(mulq) { return core.binary_reg_reg <i128, mul> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divsq) { return core.binary_reg_reg_divmod <i128, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divuq) { return core.binary_reg_reg_divmod <u128, div > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remsq) { return core.binary_reg_reg_divmod <i128, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(remuq) { return core.binary_reg_reg_divmod <u128, mod > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sllq) { return core.binary_reg_reg <u128, shift_left > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srlq) { return core.binary_reg_reg <u128, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraq) { return core.binary_reg_reg <i128, shift_right> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2q) { return core.binary_reg_reg <i128, divide_pow2> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slliq) { return core.BinaryRegShamt<u128, shift_left_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srliq) { return core.BinaryRegShamt<u128, shift_right_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(sraiq) { return core.BinaryRegShamt<i128, shift_right_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(divp2iq) { return core.BinaryRegShamt<i128, divide_pow2_immediate> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(srpq) { return core.fused_reg_reg_reg <u128, shift_right_pair > (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(slpq) { return core.fused_reg_reg_reg <u128, shift_left_pair> (); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpeqw)   { return core.BinaryRegRegBool <u32, cmp_eq> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpnew)   { return core.BinaryRegRegBool <u32, cmp_ne> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltsw)  { return core.BinaryRegRegBool <i32, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltuw)  { return core.BinaryRegRegBool <u32, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgesw)  { return core.BinaryRegRegBool <i32, cmp_ge> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgeuw)  { return core.BinaryRegRegBool <u32, cmp_ge> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpeqiw)  { return core.BinaryRegSimm<i32, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpneiw)  { return core.BinaryRegSimm<i32, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltsiw) { return core.BinaryRegSimm<i32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltuiw) { return core.BinaryRegSimm<u32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgesiw) { return core.BinaryRegSimm<i32, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgeuiw) { return core.BinaryRegSimm<u32, cmp_ge>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpeqd)   { return core.BinaryRegRegBool <u64, cmp_eq> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpned)   { return core.BinaryRegRegBool <u64, cmp_ne> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltsd)  { return core.BinaryRegRegBool <i64, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltud)  { return core.BinaryRegRegBool <u64, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgesd)  { return core.BinaryRegRegBool <i64, cmp_ge> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgeud)  { return core.BinaryRegRegBool <u64, cmp_ge> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpeqid)  { return core.BinaryRegSimm<i64, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpneid)  { return core.BinaryRegSimm<i64, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltsid) { return core.BinaryRegSimm<i64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltuid) { return core.BinaryRegSimm<u64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgesid) { return core.BinaryRegSimm<i64, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgeuid) { return core.BinaryRegSimm<u64, cmp_ge>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpeqq)   { return core.BinaryRegRegBool <u128, cmp_eq> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpneq)   { return core.BinaryRegRegBool <u128, cmp_ne> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltsq)  { return core.BinaryRegRegBool <i128, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltuq)  { return core.BinaryRegRegBool <u128, cmp_lt> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgesq)  { return core.BinaryRegRegBool <i128, cmp_ge> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgeuq)  { return core.BinaryRegRegBool <u128, cmp_ge> (); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpeqiq)  { return core.BinaryRegSimm<i128, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpneiq)  { return core.BinaryRegSimm<i128, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltsiq) { return core.BinaryRegSimm<i128, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpltuiq) { return core.BinaryRegSimm<u128, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgesiq) { return core.BinaryRegSimm<i128, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmpgeuiq) { return core.BinaryRegSimm<u128, cmp_ge>(); }


DEFINE_INSN_IMPLEMENTATION_NOMEM(cmoveqw) { return core.ConditionalMove<i32, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmovltw) { return core.ConditionalMove<i32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmovlew) { return core.ConditionalMove<i32, cmp_le>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmoveqd) { return core.ConditionalMove<i64, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmovltd) { return core.ConditionalMove<i64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmovled) { return core.ConditionalMove<i64, cmp_le>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmoveqq) { return core.ConditionalMove<i128, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmovltq) { return core.ConditionalMove<i128, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(cmovleq) { return core.ConditionalMove<i128, cmp_le>(); }


DEFINE_INSN_IMPLEMENTATION(breqw)   { return core.CompareRegRegAndBranch<i32, cmp_eq>(as); }
DEFINE_INSN_IMPLEMENTATION(brnew)   { return core.CompareRegRegAndBranch<i32, cmp_ne>(as); }
DEFINE_INSN_IMPLEMENTATION(brltsw)  { return core.CompareRegRegAndBranch<i32, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgesw)  { return core.CompareRegRegAndBranch<i32, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(brltuw)  { return core.CompareRegRegAndBranch<u32, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgeuw)  { return core.CompareRegRegAndBranch<u32, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(breqiw)  { return core.CompareRegSimmAndBranch<i32, cmp_eq>(as); }
DEFINE_INSN_IMPLEMENTATION(brneiw)  { return core.CompareRegSimmAndBranch<i32, cmp_ne>(as); }
DEFINE_INSN_IMPLEMENTATION(brltsiw) { return core.CompareRegSimmAndBranch<i32, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgesiw) { return core.CompareRegSimmAndBranch<i32, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(brltuiw) { return core.CompareRegSimmAndBranch<u32, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgeuiw) { return core.CompareRegSimmAndBranch<u32, cmp_ge>(as); }

DEFINE_INSN_IMPLEMENTATION(breqd)   { return core.CompareRegRegAndBranch<i64, cmp_eq>(as); }
DEFINE_INSN_IMPLEMENTATION(brned)   { return core.CompareRegRegAndBranch<i64, cmp_ne>(as); }
DEFINE_INSN_IMPLEMENTATION(brltsd)  { return core.CompareRegRegAndBranch<i64, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgesd)  { return core.CompareRegRegAndBranch<i64, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(brltud)  { return core.CompareRegRegAndBranch<u64, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgeud)  { return core.CompareRegRegAndBranch<u64, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(breqid)  { return core.CompareRegSimmAndBranch<i64, cmp_eq>(as); }
DEFINE_INSN_IMPLEMENTATION(brneid)  { return core.CompareRegSimmAndBranch<i64, cmp_ne>(as); }
DEFINE_INSN_IMPLEMENTATION(brltsid) { return core.CompareRegSimmAndBranch<i64, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgesid) { return core.CompareRegSimmAndBranch<i64, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(brltuid) { return core.CompareRegSimmAndBranch<u64, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgeuid) { return core.CompareRegSimmAndBranch<u64, cmp_ge>(as); }

DEFINE_INSN_IMPLEMENTATION(breqq)   { return core.CompareRegRegAndBranch<i128, cmp_eq>(as); }
DEFINE_INSN_IMPLEMENTATION(brneq)   { return core.CompareRegRegAndBranch<i128, cmp_ne>(as); }
DEFINE_INSN_IMPLEMENTATION(brltsq)  { return core.CompareRegRegAndBranch<i128, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgesq)  { return core.CompareRegRegAndBranch<i128, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(brltuq)  { return core.CompareRegRegAndBranch<u128, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgeuq)  { return core.CompareRegRegAndBranch<u128, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(breqiq)  { return core.CompareRegSimmAndBranch<i128, cmp_eq>(as); }
DEFINE_INSN_IMPLEMENTATION(brneiq)  { return core.CompareRegSimmAndBranch<i128, cmp_ne>(as); }
DEFINE_INSN_IMPLEMENTATION(brltsiq) { return core.CompareRegSimmAndBranch<i128, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgesiq) { return core.CompareRegSimmAndBranch<i128, cmp_ge>(as); }
DEFINE_INSN_IMPLEMENTATION(brltuiq) { return core.CompareRegSimmAndBranch<u128, cmp_lt>(as); }
DEFINE_INSN_IMPLEMENTATION(brgeuiq) { return core.CompareRegSimmAndBranch<u128, cmp_ge>(as); }


DEFINE_INSN_IMPLEMENTATION_NOMEM(nuleqw)   { return core.NullifyConditionallyRegReg<i32, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulnew)   { return core.NullifyConditionallyRegReg<i32, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltsw)  { return core.NullifyConditionallyRegReg<i32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltuw)  { return core.NullifyConditionallyRegReg<u32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgesw)  { return core.NullifyConditionallyRegReg<i32, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgeuw)  { return core.NullifyConditionallyRegReg<u32, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nuleqiw)  { return core.NullifyConditionallyRegSimm<i32, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulneiw)  { return core.NullifyConditionallyRegSimm<i32, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltsiw) { return core.NullifyConditionallyRegSimm<i32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltuiw) { return core.NullifyConditionallyRegSimm<u32, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgesiw) { return core.NullifyConditionallyRegSimm<i32, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgeuiw) { return core.NullifyConditionallyRegSimm<u32, cmp_ge>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nuleqd)   { return core.NullifyConditionallyRegReg<i64, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulned)   { return core.NullifyConditionallyRegReg<i64, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltsd)  { return core.NullifyConditionallyRegReg<i64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltud)  { return core.NullifyConditionallyRegReg<u64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgesd)  { return core.NullifyConditionallyRegReg<i64, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgeud)  { return core.NullifyConditionallyRegReg<u64, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nuleqid)  { return core.NullifyConditionallyRegSimm<i64, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulneid)  { return core.NullifyConditionallyRegSimm<i64, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltsid) { return core.NullifyConditionallyRegSimm<i64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltuid) { return core.NullifyConditionallyRegSimm<u64, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgesid) { return core.NullifyConditionallyRegSimm<i64, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgeuid) { return core.NullifyConditionallyRegSimm<u64, cmp_ge>(); }

DEFINE_INSN_IMPLEMENTATION_NOMEM(nuleqq)   { return core.NullifyConditionallyRegReg<i128, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulneq)   { return core.NullifyConditionallyRegReg<i128, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltsq)  { return core.NullifyConditionallyRegReg<i128, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltuq)  { return core.NullifyConditionallyRegReg<u128, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgesq)  { return core.NullifyConditionallyRegReg<i128, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgeuq)  { return core.NullifyConditionallyRegReg<u128, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nuleqiq)  { return core.NullifyConditionallyRegSimm<i128, cmp_eq>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulneiq)  { return core.NullifyConditionallyRegSimm<i128, cmp_ne>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltsiq) { return core.NullifyConditionallyRegSimm<i128, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulltuiq) { return core.NullifyConditionallyRegSimm<u128, cmp_lt>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgesiq) { return core.NullifyConditionallyRegSimm<i128, cmp_ge>(); }
DEFINE_INSN_IMPLEMENTATION_NOMEM(nulgeuiq) { return core.NullifyConditionallyRegSimm<u128, cmp_ge>(); }

// pseudos
#define DEFINE_PSEUDO(NAME) DEFINE_INSN_IMPLEMENTATION_NOMEM(NAME) { return core.unimplemented(); }

DEFINE_PSEUDO(fldis)
DEFINE_PSEUDO(fldid)
DEFINE_PSEUDO(fldsr)
DEFINE_PSEUDO(flddr)
DEFINE_PSEUDO(fldqr)

DECLARE_SUBGOUP_OPCODES(DEFINE_PSEUDO)

} // namespace postrisc
