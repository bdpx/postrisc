#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Debugger.hpp"
#include "Core.hpp"
#include "Utils.hpp"

namespace postrisc {

DEFINE_INSN_IMPLEMENTATION_NOMEM(random)
{
    core.ra() = core.random.Read();
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(eh_throw)
{
    u64 const address = core.rb().base() + core.cmd.imm;
    CHECK_RESULT(core.exception_state.RaiseException(address));
    return core.ExecuteJump(as, core.exception_state.GetEIP());
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(eh_catch)
{
    u64 const address = core.ip + sizeof(Bundle) * core.cmd.imm;
    core.exception_state.SetEIP(address);

    // copy ECA (exception context address) in register, clear ECA
    core.rb() = core.exception_state.GetContext();
    core.exception_state.SetContext(0);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(eh_adj)
{
    if (core.exception_state.IsException()) {
        return core.ExecuteJump(as, core.exception_state.GetEIP());
    }

    u64 const address = core.ip + sizeof(Bundle) * core.cmd.imm;
    core.exception_state.SetEIP(address);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(eh_next)
{
    // setup new eip, where is next destructor call or function return
    u64 const address = core.ip + sizeof(Bundle) * core.cmd.imm;
    core.exception_state.SetEIP(address);

    // check double fault
    u64 const context = core.exception_state.GetContext();
    u64 const old = core.rb().base();

    if (context && old) {
        LOG_DEBUG(LOG_EMULATOR)
            << "previous_context=" << context
            << " restored_context=" << old;
        return Result::software_double_exception;
    }

    core.exception_state.SetContext(old);

    if (core.exception_state.IsException()) {
        return core.ExecuteJump(as, core.exception_state.GetEIP());
    }
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(cpuid)
{
    u64 const index = core.rb().base() + core.cmd.imm;
    u64 const value = core.cpuid.Get(index);
    core.ra() = value;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(get_spr)
{
    GeneralRegister value(0, 0);
    CHECK_RESULT(core.GetSpecialRegister(core.cmd.rc, value));
    core.ra() = value;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(set_spr)
{
    return core.SetSpecialRegister(core.cmd.rc, core.ra());
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(get_mr)
{
    u64 const index = core.rb().base() + core.cmd.imm;
    LOG_DEBUG(LOG_EMULATOR) << "index=" << index;
    CHECK_RESULT(core.performance_monitor.Get(index, core.ra().scalar<u64>()));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(set_mr)
{
    u64 const index = core.rb().base() + core.cmd.imm;
    LOG_DEBUG(LOG_EMULATOR) << "index=" << index;
    CHECK_RESULT(core.performance_monitor.Set(core.ra().scalar<u64>(), index));
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(get_ibr)
{
    u64 const index = core.rb().base() + core.cmd.imm;
    LOG_DEBUG(LOG_EMULATOR) << "index=" << index;

    if (index < core.ibr.size()) {
        core.ra() = core.ibr[index].Get();
    } else {
        core.ra() = U64C(0);
    }
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(set_ibr)
{
    u64 const index = core.rb().base() + core.cmd.imm;
    LOG_DEBUG(LOG_EMULATOR) << "index=" << index;

    if (index < core.ibr.size()) {
        core.ibr[index].Set(core.ra());
    }
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(get_dbr)
{
    u64 const index = core.rb().base() + core.cmd.imm;
    LOG_DEBUG(LOG_EMULATOR) << "index=" << index;

    if (index < core.dbr.size()) {
        core.ra() = core.dbr[index].Get();
    }
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(set_dbr)
{
    u64 const index = core.rb().base() + core.cmd.imm;
    LOG_DEBUG(LOG_EMULATOR) << "index=" << index;

    if (index < core.dbr.size()) {
        core.dbr[index].Set(core.ra());
    }
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(set_itr)
{
    u64 const index = core.rb().base();
    LOG_DEBUG(LOG_EMULATOR)
        << "index=" << index << " a=" << core.ra() << " c=" << core.rc();
    // TODO
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(set_dtr)
{
    u64 const index = core.rb().base();
    LOG_DEBUG(LOG_EMULATOR)
        << "index=" << index << " a=" << core.ra() << " c=" << core.rc();
    // TODO
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(rscover)
{
    CHECK_KERNEL_MODE(core);
    core.rsc.CoverFrame();
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(rfi)
{
    CHECK_KERNEL_MODE(core);

    // restore instruction pointer, clear future
    CHECK_RESULT(core.ExecuteJump(as, RoundToBundle(core.iip)));
    // restore program status (and old future)
    core.psr = core.ipsr;
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(icbi)
{
    // check address
    u64 const address = core.rb().base() + core.cmd.imm;
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for invalidating cache
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dcbt)
{
    // check address
    u64 const address = core.rb().base() + core.cmd.imm;
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for cache
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dcbf)
{
    // check address
    u64 const address = core.rb().base() + core.cmd.imm;
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for flushing caches or write-buffers
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(dcbi)
{
    CHECK_KERNEL_MODE(core);
    u64 const address = core.rb().base() + core.cmd.imm;
    UNREFERENCED_PARAMETER(address);
    // but do nothing in emulator for invalidating cache
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(mprobe)
{
    return core.unimplemented();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(rsflush)
{
    CHECK_KERNEL_MODE(core);
    return core.unimplemented();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(rsload)
{
    CHECK_KERNEL_MODE(core);
    return core.unimplemented();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(ptc)
{
    CHECK_KERNEL_MODE(core);
    const u64 base    = core.ra().base();
    const u64 address = core.rb().base();
    const u64 asid    = core.rc().base();
    LOG_DEBUG(LOG_EMULATOR) << "base=" << fmt::hex(base) << " address=" << fmt::hex(address) << " asid=" << asid;
    return core.unimplemented();
}

class TimePeeker {
public:
    TimePeeker(PerformanceMonitor& pm_) : pm(pm_), start(std::chrono::steady_clock::now()) {}
    ~TimePeeker(void) {
        std::chrono::steady_clock::time_point const end = std::chrono::steady_clock::now();
        u64 const run_time = std::chrono::duration <u64, std::nano> (end - start).count();
        pm.report_event(PMC_RUNTIME, -run_time);
    }
private:
    PerformanceMonitor& pm;
    std::chrono::steady_clock::time_point start;
};

DEFINE_INSN_IMPLEMENTATION(syscall)
{
    // TimePeeker time_keeper(core.performance_monitor);

    //u64 const address = kip;
    //LOG_DEBUG(LOG_SYSCALL)
    //    << "syscall: kip=" << fmt::hex(address);

    // syscall instruction should be wrapped in usual call to __syscall func
    // all 6 params should be in regs (filled by zeros) even if syscall has less params
    unsigned temp, rin_result, ri2_result, ri3_result, ri4_result,
                               ri5_result, ri6_result, ri7_result;
    ROTATE_REG(core, rin_result, temp, 1)  u64 rn = core.gpr[rin_result].base();
    ROTATE_REG(core, ri2_result, temp, 2)  u64 r2 = core.gpr[ri2_result].base();
    ROTATE_REG(core, ri3_result, temp, 3)  u64 r3 = core.gpr[ri3_result].base();
    ROTATE_REG(core, ri4_result, temp, 4)  u64 r4 = core.gpr[ri4_result].base();
    ROTATE_REG(core, ri5_result, temp, 5)  u64 r5 = core.gpr[ri5_result].base();
    ROTATE_REG(core, ri6_result, temp, 6)  u64 r6 = core.gpr[ri6_result].base();
    ROTATE_REG(core, ri7_result, temp, 7)  u64 r7 = core.gpr[ri7_result].base();

    int const result = as.DoSyscall(rn, core.gpr[rin_result].scalar<u64>(), r2, r3, r4, r5, r6, r7);
    if (result < 0) { // SYS_exit call detected
        return Result::halt;
    }
    // psr.set_kernel_mode();
    // return CallSubroutine(as, cmd.ra(), a, address);
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(sysret)
{
    LOG_DEBUG(LOG_SYSCALL) << "sysret";
    CHECK_KERNEL_MODE(core);
    return core.unimplemented();
}

DEFINE_INSN_IMPLEMENTATION(tpa)
{
    LOG_DEBUG(LOG_SYSCALL) << "tpa";
    CHECK_KERNEL_MODE(core);
    const VirtAddress va(core.rb().base());
    const u64 asid = core.rc().base();
    LOG_DEBUG(LOG_EMULATOR) << " base=" << va << " asid=" << asid;
    auto [result, pa] = as.mmu_translate(core, va);
    CHECK_RESULT_ADDRESS(core, va, result);
    core.ra() = pa.get_address();
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION(int)
{
    u64 const value = core.rb().base() + core.cmd.imm;
    u8 const int_vector = value & util::makemask<u64>(bits_per_vector_num);
    switch (int_vector) {
        case Result::instruction_debug:
            Debugger(as, &core).execute();
            break;
        default:
            CHECK_RESULT(core.post_interrupt(Result::ConvertFromVector(int_vector)));
            break;
    }
    return core.FinalizeInstruction();
}

DEFINE_INSN_IMPLEMENTATION_NOMEM(halt)
{
    CHECK_KERNEL_MODE(core);

    //trap(Result::machine_reset);
    core.m_Stopped = true;

    // save info for post-mortem debug
    core.SetupDebugPlace(core.ip, core.psr.rs.get());

    LOG_DEBUG(LOG_EXCEPTION) << "HALT!";
    return Result::halt;
}

} //namespace postrisc
