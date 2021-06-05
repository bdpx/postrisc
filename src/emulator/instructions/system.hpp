
#include "core.hpp"

namespace postrisc {

DEFINE_INSN_IMPLEMENTATION(rdrand)
{
    GET_WRA;
    a = random.Read();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(rdseed)
{
    GET_WRA;
    a = random.Read();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ehthrow)
{
    GET_RRB;
    uint64_t const address = b.base() + cmd.get_bin_simm();

    CHECK_STATUS(exception_state.RaiseException(address));
    return ExecuteJump(exception_state.GetEIP());
}

DEFINE_INSN_IMPLEMENTATION(ehcatch)
{
    GET_WRA;

    uint64_t const address = cmd.ComputeJumpRelative(ip);
    exception_state.SetEIP(address);

    // copy ECA (exception context address) in register, clear ECA
    a = exception_state.GetContext();
    exception_state.SetContext(0);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ehadj)
{
    if (exception_state.IsException()) {
        return ExecuteJump(exception_state.GetEIP());
    }

    uint64_t const address = cmd.ComputeJumpRelative(ip);
    exception_state.SetEIP(address);
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(ehnext)
{
    GET_RRA;

    // setup new eip, where is next destructor call or function return
    uint64_t const address = cmd.ComputeJumpRelative(ip);
    exception_state.SetEIP(address);

    // check double fault
    uint64_t const context = exception_state.GetContext();
    if (context && a.base()) {
        LOG_TRACE(LOG_EMULATOR)
            << "previous_context=" << context
            << " new_context=" << a.base();
        return CStatus(CStatus::software_double_exception);
    }

    exception_state.SetContext(a.base());

    if (exception_state.IsException()) {
        return ExecuteJump(exception_state.GetEIP());
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(cpuid)
{
    GET_WRA;
    GET_RRB;

    uint64_t const index = b.base() + cmd.GetUintStride();
    uint64_t const value = cpuid.Get(index);
    a = value;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mfspr)
{
    GET_WRA;
    CHECK_STATUS(GetSpecialRegister(cmd.rs(), a));
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mtspr)
{
    GET_RRA;
    return SetSpecialRegister(cmd.rs(), a);
}

DEFINE_INSN_IMPLEMENTATION(sprsr)
{
    CHECK_KERNEL_MODE();

    GET_WRA;
    GET_RRC;
    GET_RRD;

    const unsigned sprnum = cmd.rs();

    CGeneralRegister state;
    CHECK_STATUS(GetSpecialRegister(sprnum, state));

    CGeneralRegister new_state = state;
    new_state |= c;
    new_state &= ~d;
    CHECK_STATUS(SetSpecialRegister(sprnum, new_state));
    a = state;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mfmr)
{
    GET_WRA;
    GET_RRB;

    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << fmt::endl;

    CHECK_STATUS(performance_monitor.Get(index, a.scalar<uint64_t>()));

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mtmr)
{
    GET_RRB;
    GET_RRC;
    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << fmt::endl;

    CHECK_STATUS(performance_monitor.Set(c.scalar<uint64_t>(), index));

    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mfibr)
{
    GET_WRA;
    GET_RRB;

    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << fmt::endl;

    if (index < ibr.size()) {
        a = ibr[index].Get();
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mtibr)
{
    GET_RRB;
    GET_RRC;

    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << fmt::endl;

    if (index < ibr.size()) {
        ibr[index].Set(c);
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mfdbr)
{
    GET_WRA;
    GET_RRB;

    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << fmt::endl;

    if (index < dbr.size()) {
        a = dbr[index].Get();
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mtdbr)
{
    GET_RRB;
    GET_RRC;

    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << fmt::endl;

    if (index < dbr.size()) {
        dbr[index].Set(c);
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mtitr)
{
    GET_RRB;
    GET_RRC;

    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << c << fmt::endl;
    // TODO
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mtdtr)
{
    GET_RRB;
    GET_RRC;

    uint64_t const index = b.base();
    LOG_TRACE(LOG_EMULATOR) << "index=" << index << c << fmt::endl;
    // TODO
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(rscover)
{
    CHECK_KERNEL_MODE();
    rsc.CoverFrame();
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(rfi)
{
    CHECK_KERNEL_MODE();

    // restore instruction pointer, clear future
    CHECK_STATUS(ExecuteJump(RoundToBundle(iip)));
    // restore program status (and old future)
    psr = ipsr;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(syscall)
{
    GET_WRA;
    GET_RRB;

    uint64_t const address = kip;
    uint64_t const code = b.base() + cmd.GetUintStride();

    LOG_TRACE(LOG_SYSCALL | LOG_CALLSTACK)
        << "syscall: code=" << code << ", kip=" << fmt::hex(address);

    psr.set_kernel_mode();
    return CallSubroutine(cmd.ra(), a, address);
}

DEFINE_INSN_IMPLEMENTATION(msync)
{
    UNREFERENCED_PARAMETER(cmd);
    // do nothing in emulator for flushing caches or write-buffers
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(icbi)
{
    GET_RRB;
    // check address
    uint64_t const address = b.base() + cmd.get_bin_simm();
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for invalidating cache
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(dcbt)
{
    GET_RRB;
    // check address
    uint64_t const address = b.base() + cmd.get_bin_simm();
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for cache
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(dcbf)
{
    GET_RRB;
    // check address
    uint64_t const address = b.base() + cmd.get_bin_simm();
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for flushing caches or write-buffers
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(dcbi)
{
    CHECK_KERNEL_MODE();
    GET_RRB;
    uint64_t const address = b.base() + cmd.get_bin_simm();
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for invalidating cache
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(mprobe)
{
    return unimplemented(cmd);
}

DEFINE_INSN_IMPLEMENTATION(rsflush)
{
    CHECK_KERNEL_MODE();
    return unimplemented(cmd);
}

DEFINE_INSN_IMPLEMENTATION(rsload)
{
    CHECK_KERNEL_MODE();
    return unimplemented(cmd);
}

DEFINE_INSN_IMPLEMENTATION(ptc)
{
    CHECK_KERNEL_MODE();
    GET_RRB;
    const uint64_t address = b.base() + cmd.get_bin_simm();
    UNREFERENCED_PARAMETER(address);
    return unimplemented(cmd);
}

DEFINE_INSN_IMPLEMENTATION(sysret)
{
    LOG_TRACE(LOG_SYSCALL) << "sysret";
    CHECK_KERNEL_MODE();
    return unimplemented(cmd);
}

DEFINE_INSN_IMPLEMENTATION(tpa)
{
    LOG_TRACE(LOG_SYSCALL) << "tpa";
    CHECK_KERNEL_MODE();
    GET_WRA;
    GET_RRB;
    uint64_t const address = b.base();

    uint64_t phys_address;
    CHECK_STATUS_ADDRESS(address, mmu_translate(address, phys_address));

    a = phys_address;
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(int)
{
    GET_RRB;
    uint64_t const value = b.base() + cmd.GetUintStride();
    uint8_t const int_vector = value & util::makemask(bits_per_vector_num);
    switch (int_vector) {
        case CStatus::instruction_debug:
            Debugger(this).execute();
            break;
        default:
            CHECK_STATUS(post_interrupt(CStatus(int_vector)));
            break;
    }
    return FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(halt)
{
    CHECK_KERNEL_MODE();

    //trap(CStatus::machine_reset);
    m_Stopped = true;

    // save info for post-mortem debug
    debug_ip = ip - sizeof(CBundle);
    debug_ri = psr.GetShadowSize();

    return CStatus(CStatus::halt);
}

} //namespace postrisc
