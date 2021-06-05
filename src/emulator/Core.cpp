#include <chrono>      // for std::chrono::steady_clock/duration/etc

#include "util/common.hpp"
#include "config/configuration.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "Core.hpp"
#include "Utils.hpp"
#include "Debugger.hpp"
#include "binary_format/file_header.hpp"
#include "AddressSpace.hpp"
#include "disassembler/Decoder.hpp"

namespace postrisc {

CoreConfigSpace::CoreConfigSpace()
    : header(0x0010, sizeof(CoreConfigSpace))
    , m_InterruptVector(0xaaaa)
    , m_TimeCmp(0xdeadbeaf)
{
}

void
CoreConfigSpace::dump(const DumpFormatter& out) const
{
    header.dump(out);
    out << fmt::table("TimeCmp") << fmt::hex(m_TimeCmp) << fmt::endl;
}

Core::Core(const std::string& name, u32 core_id, unsigned flags)
    : Device(name)
    , random(core_id)
    , m_config_flags(flags)
{
}

Core::~Core(void)
{
}

u64 Core::size(void) const
{
    // core should not be mapped as normal MMIO device, only config space is available
    return 0;
}

u64 Core::config_size(void) const
{
    return sizeof(config);
}

Core *Core::get_bootstrap_core(void)
{
    return this;
}

// Core as a device doesn't support any io-mapped read/write
Result Core::read_u8   (const DeviceOffset UNUSED(device_offset),   u8& UNUSED(value)) const { return Result::device_error; }
Result Core::read_u16  (const DeviceOffset UNUSED(device_offset),  u16& UNUSED(value)) const { return Result::device_error; }
Result Core::read_u32  (const DeviceOffset UNUSED(device_offset),  u32& UNUSED(value)) const { return Result::device_error; }
Result Core::read_u64  (const DeviceOffset UNUSED(device_offset),  u64& UNUSED(value)) const { return Result::device_error; }
Result Core::read_u128 (const DeviceOffset UNUSED(device_offset), u128& UNUSED(value)) const { return Result::device_error; }
Result Core::write_u8  (const DeviceOffset UNUSED(device_offset),    u8 UNUSED(value)) { return Result::device_error; }
Result Core::write_u16 (const DeviceOffset UNUSED(device_offset),   u16 UNUSED(value)) { return Result::device_error; }
Result Core::write_u32 (const DeviceOffset UNUSED(device_offset),   u32 UNUSED(value)) { return Result::device_error; }
Result Core::write_u64 (const DeviceOffset UNUSED(device_offset),   u64 UNUSED(value)) { return Result::device_error; }
Result Core::write_u128(const DeviceOffset UNUSED(device_offset),  u128 UNUSED(value)) { return Result::device_error; }

Result Core::read(const DeviceOffset device_offset, size_t len, void* UNUSED(bytes)) const
{
    LOG_ERROR(LOG_INTERRUPT) << "address=" << device_offset << " len=" << len;
    return Result::device_error;
}

Result Core::write(const DeviceOffset device_offset, size_t len, const void* UNUSED(bytes))
{
    LOG_ERROR(LOG_INTERRUPT) << "address=" << device_offset << " len=" << len;
    return Result::device_error;
}

// Core as a device supports config read/write (IPI etc)
Result Core::read_config(const DeviceOffset device_offset, size_t len, void *bytes) const
{
    return read_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

Result Core::write_config(const DeviceOffset device_offset, size_t len, const void *bytes)
{
    LOG_OUTPUT(LOG_INTERRUPT)
         << "CoreArray write: pid=" << pic.lid
         << " address=" << device_offset
         << " len=" << len << fmt::endl;

    u64 const offset = device_offset.get_offset();
    if (offset == offsetof(CoreConfigSpace, m_InterruptVector) && len == sizeof(config.m_InterruptVector))
    {
        u64 iv = *reinterpret_cast<u64 const*>(bytes);
        LOG_OUTPUT(LOG_INTERRUPT) << "iv=" << fmt::hex(iv) << fmt::endl;
        CHECK_RESULT(post_interrupt(Result::ConvertFromVector(iv)));
    }
    else if (offset == offsetof(CoreConfigSpace, m_TimeCmp) && len == sizeof(config.m_TimeCmp))
    {
        u64 timecmp = *reinterpret_cast<u64 const*>(bytes);
        LOG_OUTPUT(LOG_INTERRUPT) << "timecmp=" << fmt::hex(timecmp) << fmt::endl;
        config.m_TimeCmp = timecmp;
    } else {
        return Result::device_error;
    }

    return write_config_raw(device_offset, &config, sizeof(config), len, bytes);
}

void
Core::dump_config(const PhysAddress address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<const std::byte*>(&config), sizeof(config), out);
}

Result
Core::complex_operation(const DeviceOffset device_offset, size_t len, MemoryOperation& UNUSED(op))
{
    LOG_ERROR(LOG_PLATFORM) << "invalid atomic: " << device_offset << ' ' << len;
    return Result::device_error;
}

void
Core::ResetCodeCache(void)
{
    m_instruction_cache_tags.fill(0);
    // cache_data.fill(???);
}

Result
Core::post_interrupt(Result vector_index)
{
    if (IsStopped()) {
        // ignore interrupt broadcast
        return Result::continue_execution;
    }
    LOG_DEBUG(LOG_INTERRUPT) << "vector_index=" << vector_index;
    return pic.post_interrupt(vector_index);
}

/***************************************************************************
* this function do processor startup.
***************************************************************************/
void
Core::reset(const unsigned coreid)
{
    gpr.fill(GeneralRegister(U64C(0)));

    ip = 0;
    iip = 0;
    iipa = 0;

    fpu.set_control_word(0);

    rsp = 0;
    bsp = 0;
    fetched_bundles = 0;
    itc = std::numeric_limits<u64>::max();
    peb = 0;
    teb = 0;

    kip  = U64C(0xFFFFFFFF00000000);
    ksp  = 0;
    krsp = 0;

    psr.reset();
    ipsr.reset();

    (void)mmu.SetConfig(0);

    ResetCodeCache();

    iva = 0;
    cause.imm = 0;
    ifa = 0;

    pic.reset(coreid);
    rsc.reset();

    itm = 0;

    for (auto & x : ibr) {
        x.reset();
    }

    for (auto & x : dbr) {
        x.reset();
    }

    performance_monitor.reset();

    fpu.clear_flags();
    exception_state.reset();

    SetupDebugPlace(ip, 0);
}

void
Core::SetupDebugPlace(u64 address, unsigned rs)
{
    debug_ip = address;
    debug_restart_slot = rs;
    LOG_DEBUG(LOG_EXCEPTION | LOG_PREDICATION)
        << " ip=" << fmt::hex(debug_ip)
        << " rs=" << debug_restart_slot;
}

/***************************************************************************
* This function extend register frame
***************************************************************************/
Result
Core::AllocateFrame(AddressSpace& as, const unsigned framesize)
{
    static_assert(max_stacked_framesize <= phys_stacked_regs);

    unsigned const sof = framesize;
    if (unlikely(sof > max_stacked_framesize)) {
        LOG_ERROR(LOG_EXCEPTION)
            << "Invalid register frame: " << sof
            << " maximum " << max_stacked_framesize;

        return Result::illegal_instruction;
    }

    // if invalid+clean register group is less than created frame,
    // spill dirty registers to backing store
    while (!rsc.CanAllocateFrame(sof)) {
        const VirtAddress address(rsp);
        unsigned const bod = rsc.get_bod();
        GeneralRegister const * dirty = &gpr[base_rotate_regs + bod];
        unsigned const batch_granularity = RegisterStackControlRegister::BatchGranularity;
        assert(base_rotate_regs + bod + batch_granularity <= gpr.size());
        LOG_DEBUG(LOG_REGISTER_STACK)
            << "spill " << bod
            << " address=" << address
            << " regs:" << fmt::span<GeneralRegister>(dirty, batch_granularity);

        CHECK_RESULT_ADDRESS(*this, address,
            as.mmu_write_backing_store(*this, address, batch_granularity * sizeof(gpr[0]), dirty));

#if defined(DEBUG_BACKSTORE_STACK)
        for (size_t i = 0; i < batch_granularity; i++) {
            m_debug_backstore_stack.push_back(dirty[i]);
        }
#endif
        // commit changes
        rsp += batch_granularity * sizeof(gpr[0]);
        rsc.ShrinkDirtySection(batch_granularity);
        performance_monitor.report_event(PMC_REGISTER_SPILL, batch_granularity);
    }

    // setup new rsc
    rsc.AllocateFrame(sof);
    return Result::continue_execution;
}

/***************************************************************************
* this function handle callx
***************************************************************************/
Result
Core::CallSubroutine(AddressSpace& as, const unsigned preserve, GeneralRegister& retreg, u64 address)
{
    const unsigned preservedFrameSize = preserve - base_rotate_regs;
    if (unlikely(preservedFrameSize % frame_base_alignment)) {
        LOG_ERROR(LOG_EXCEPTION)
            << "unaligned preserved frame size: " << preservedFrameSize;
        return Result::illegal_instruction;
    }
    if (unlikely(preservedFrameSize > rsc.GetSizeOfFrame())) {
        LOG_ERROR(LOG_EXCEPTION)
            << "preserved size " << preservedFrameSize
            << " is bigger than frame size " << rsc.GetSizeOfFrame();
        return Result::illegal_instruction;
    }

    const unsigned outsize = rsc.PreserveFrame(preservedFrameSize);

    const u32 future = psr.future.get();
    u64 ret_addr = ip;

    const unsigned projectedRestartingSlot = psr.rs.get() + cmd.num_slots;
    LOG_DEBUG(LOG_CALLSTACK | LOG_PREDICATION)
        << "psr.rs=" << psr.rs.get()
        << " projectedRestartingSlot=" << projectedRestartingSlot;

    assert(projectedRestartingSlot > 0);
    assert(projectedRestartingSlot <= slots_per_bundle);

    if (projectedRestartingSlot == slots_per_bundle) {
        // go to next bundle after return, no restarting slots
        ret_addr += sizeof(Bundle);
    } else {
        // stay in current bundle, skip restarting slots
        ret_addr |= projectedRestartingSlot;
    }

    // pack together return addresses & preserved frame size
    const u64 eip = exception_state.GetEIP();
    const u64 eip_offset = eip ? (eip - RoundToBundle(ret_addr)) >> log_bytes_per_bundle : 0;

    // check that eip offset fit in bits_per_eip_offset
    if (unlikely(eip_offset != util::zext64(eip_offset, bits_per_eip_offset))) {
        LOG_ERROR(LOG_EXCEPTION)
            << "eip-offset overflow:"
            << " ip=" << fmt::hex(ip)
            << " eip=" << fmt::hex(eip)
            << " eip_offset=" << fmt::hex(eip_offset);
        return Result::instruction_invalid_address;
    }

    LOG_DEBUG(LOG_CALLSTACK | LOG_PREDICATION)
        << " future=" << fmt::hex(future)
        << " eip_offset=" << fmt::hex(eip_offset)
        << " ret_addr=" << fmt::hex(ret_addr);

    retreg.link_info.link_address         = ret_addr;
    retreg.link_info.preserved_frame_size = preservedFrameSize;
    retreg.link_info.out_size             = outsize;
    retreg.link_info.eip_offset           = eip_offset;
    retreg.link_info.future               = future;

    // now already jump
    LOG_DEBUG(LOG_EMULATOR)
        << "call address=" << fmt::hex(address)
        << " retaddr=" << fmt::hex(ret_addr)
        << " preservedFrameSize=" << preservedFrameSize
        << " outsize=" << outsize;

#if defined(DEBUG_LINK_INFO_STACK)
    m_debug_link_info_stack.emplace_back(ret_addr,
                                         r_static_reg(reg_sp).base(),
                                         r_static_reg(reg_fp).base());
#endif
    return ExecuteJump(as, RoundToBundle(address));
}

/***************************************************************************
* This function handle subroutine returns and register stack pop
***************************************************************************/
Result
Core::ReturnFromSubroutine(AddressSpace& as, const GeneralRegister link)
{
    // unpack link-info from link register
    u64 const retaddr            = link.link_info.link_address;
    u32 const preservedFrameSize = link.link_info.preserved_frame_size;
    u32 const outFrameSize       = link.link_info.out_size;
    u32 const future             = link.link_info.future;
    u64 const eip_offset         = link.link_info.eip_offset;

    // form 4 low bits only 2 is used for slot num, but all 4 are reserved
    const unsigned restarting_slots = retaddr & util::makemask<u64>(4);
    if (restarting_slots >= slots_per_bundle) {
        LOG_ERROR(LOG_EXCEPTION)
            << "invalid restarting slot: " << restarting_slots;
        return Result::invalid_bundle_slot;
    }

    u64 const eretaddr = eip_offset ? RoundToBundle(retaddr) + (eip_offset << log_bytes_per_bundle) : 0;
    if (unlikely(exception_state.IsException())) {
        if (!eretaddr) {
            return Result::invalid_parameter;
        }
    }

#if 0
    if (unlikely(rsc.GetSizeOfFrame() < framesize)) {
        LOG_ERROR(LOG_CALLSTACK)
            << "Invalid return frame size: " << framesize
            << " SizeOfFrame=" << rsc.GetSizeOfFrame();

        return Result::invalid_parameter;
    }
#endif

    LOG_DEBUG(LOG_EMULATOR)
        << "return: retaddr=" << fmt::hex(retaddr)
        << " preservedFrameSize=" << preservedFrameSize
        << " outFrameSize=" << outFrameSize
        << " addr=" << fmt::hex(retaddr);

    if (unlikely(preservedFrameSize % frame_base_alignment)) {
        LOG_ERROR(LOG_EXCEPTION)
            << "unaligned preserved frame size: " << preservedFrameSize;
        return Result::invalid_parameter;
    }

    if (unlikely(preservedFrameSize + outFrameSize > max_stacked_framesize)) {
        LOG_ERROR(LOG_EXCEPTION)
            << "Invalid return frame size - too big full size: " << preservedFrameSize
            << '+' << outFrameSize
            << " maximum: " << max_stacked_framesize;

        return Result::invalid_parameter;
    }

    CHECK_RESULT(rsc.ShrinkFrame(outFrameSize));

    // if dirty register group is less than restored frame, fill them from backing store
    while (!rsc.CanRestoreFrame(preservedFrameSize)) {
        unsigned const batch_granularity = RegisterStackControlRegister::BatchGranularity;
        const VirtAddress address(rsp - batch_granularity * sizeof(gpr[0]));
        unsigned const entry = rsc.GetFillEntry();
        assert(base_rotate_regs + entry + batch_granularity <= gpr.size());
        GeneralRegister * dirty = &gpr[base_rotate_regs + entry];
        CHECK_RESULT_ADDRESS(*this, address,
            as.mmu_read_backing_store(*this, address, batch_granularity * sizeof(gpr[0]), dirty));

#if defined(DEBUG_BACKSTORE_STACK)
        assert(m_debug_backstore_stack.size() >= batch_granularity);
        for (ssize_t i = batch_granularity - 1; i >= 0; i--) {
            GeneralRegister const expected = m_debug_backstore_stack.back();
            m_debug_backstore_stack.pop_back();
            if (expected != dirty[i]) {
                LOG_ERROR(LOG_REGISTER_STACK) << "debug_backstore_stack mismatch[" << i << "]:"
                    << " expected=" << expected
                    << " dirty=" << dirty[i] << std::endl;
                // return Result::invalid_parameter;
            }
        }
#endif
        LOG_DEBUG(LOG_REGISTER_STACK)
            << "rfill entry=" << entry
            << " address=" << address
            << " reg=" << fmt::span<GeneralRegister>(dirty, batch_granularity);

        // commit changes
        rsp = address.get_address();
        rsc.EnhanceDirtySection(batch_granularity);
        performance_monitor.report_event(PMC_REGISTER_FILL, batch_granularity);
    }

    rsc.RestoreOnReturn(preservedFrameSize, outFrameSize);

    // no more exceptions
    // below is non-restartable part
    // rollback stack pointer

    // if there is an actual exception in progress:
    // return to eretaddr, with cleared future
    if (unlikely(exception_state.IsException())) {
        exception_state.SetEIP(eretaddr);
        return ExecuteJump(as, eretaddr);
    }

    exception_state.SetEIP(eretaddr);
    Result const ret = ExecuteJump(as, RoundToBundle(retaddr));

    // ExecuteJump clears future and restarting slot, so setup new later
    psr.rs.set(restarting_slots);
    psr.future.set(future);
    return ret;
}

#if defined(DEBUG_LINK_INFO_STACK)
Result
Core::CheckStackOnReturn(void)
{
    if (m_debug_link_info_stack.empty()) {
        LOG_ERROR(LOG_EXCEPTION) << "m_debug_link_info_stack is empty";
        return Result::invalid_parameter;
    }
    DebugLinkInfo const old_stack = m_debug_link_info_stack.back();
    m_debug_link_info_stack.pop_back();

    u64 const sp = r_static_reg(reg_sp).base();
    u64 const fp = r_static_reg(reg_fp).base();

    if (sp != old_stack.sp || fp != old_stack.fp || ip != old_stack.ip) {
        LOG_ERROR(LOG_EXCEPTION) << "m_debug_link_info_stack invalid:"
            << " cur_sp=" << fmt::hex(sp) << " old_sp=" << fmt::hex(old_stack.sp)
            << " cur_fp=" << fmt::hex(fp) << " old_fp=" << fmt::hex(old_stack.fp)
            << " ret_ip=" << fmt::hex(ip) << " old_ip=" << fmt::hex(old_stack.ip);
        return Result::invalid_parameter;
    }
    return Result::continue_execution;
}
#endif

/***************************************************************************
* this function read from special purpose register
***************************************************************************/
Result
Core::GetSpecialRegister(unsigned int regnum, GeneralRegister& out)
{
    if (unlikely(arch_spr_regtype[regnum] == ESpecialRegisterSubset::reserved)) {
        LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register in mfspr: " << regnum;
        return Result::illegal_instruction;
    }

    if (unlikely(!psr.IsKernelMode() &&
                 arch_spr_regtype[regnum] != ESpecialRegisterSubset::user &&
                 arch_spr_regtype[regnum] != ESpecialRegisterSubset::ruser)) {
        LOG_ERROR(LOG_EXCEPTION) << "privileged register in mfspr: " << regnum;
        return Result::general_protection;
    }

    switch (regnum) {
        case reg_ip:       out = ip;                         break;
        case reg_eip:      out = exception_state.GetEIP();   break;
        case reg_reip:     out = exception_state.GetREIP();  break;
        case reg_fpcr:     out = fpu.get_control_word();     break;
        case reg_rsc:      out = rsc.get();                  break;
        case reg_rsp:      out = rsp;                        break;
        case reg_bsp:      out = bsp;                        break;

        case reg_eca:      out = exception_state.GetContext(); break;

        case reg_kip:      out = kip;                      break;
        case reg_krsp:     out = krsp;                     break;
        case reg_ksp:      out = ksp;                      break;

        case reg_peb:      out = peb;                      break;
        case reg_teb:      out = teb;                      break;
        case reg_itc:      out = itc;                      break;
        case reg_itm:      out = itm;                      break;
        case reg_pta:      out = mmu.GetConfig();          break;
        case reg_psr:      out = psr.get();                break;
        case reg_iva:      out = iva;                      break;
        case reg_iip:      out = iip;                      break;
        case reg_iib:      out = iib.get_u128();           break;
        case reg_iipa:     out = iipa;                     break;
        case reg_ifa:      out = ifa;                      break;
        case reg_ipsr:     out = ipsr.get();               break;
        case reg_cause:    out = cause.imm;                break;
        case reg_lid:      out = pic.lid;                  break;
        case reg_tpr:      out = pic.tpr;                  break;

        case reg_itcv:     out = pic.itcv.get();           break;
        case reg_tsv:      out = pic.tsv.get();            break;
        case reg_pmv:      out = pic.pmv.get();            break;
        case reg_cmcv:     out = pic.cmcv.get();           break;

        case reg_irr0:     out = pic.irr[0].load();        break;
        case reg_irr1:     out = pic.irr[1].load();        break;
        case reg_irr2:     out = pic.irr[2].load();        break;
        case reg_irr3:     out = pic.irr[3].load();        break;

        case reg_isr0:     out = pic.isr[0];               break;
        case reg_isr1:     out = pic.isr[1];               break;
        case reg_isr2:     out = pic.isr[2];               break;
        case reg_isr3:     out = pic.isr[3];               break;

        case reg_iv:       out = pic.read_vector();        break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register number " << regnum << " in mfspr";
            return Result::illegal_instruction;
    }

    return Result::continue_execution;
}

/***************************************************************************
* this function write to special purpose register
***************************************************************************/
Result
Core::SetSpecialRegister(unsigned int regnum, GeneralRegister value)
{
    if (unlikely(arch_spr_regtype[regnum] == ESpecialRegisterSubset::reserved)) {
        LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register in mtspr: " << regnum;
        return Result::illegal_instruction;
    }

    if (unlikely(!psr.IsKernelMode() && arch_spr_regtype[regnum] != ESpecialRegisterSubset::user)) {
        LOG_ERROR(LOG_EXCEPTION) << "privileged register in mtspr: " << regnum;
        return Result::general_protection;
    }

    switch (regnum) {
        case reg_ip:
            return Result::illegal_instruction;

        case reg_eip:
            if (RoundToBundle(value.base()) != value.base()) {
               LOG_ERROR(LOG_EXCEPTION) << "Invalid EIP write value: " << fmt::hex(value.base());
               return Result::invalid_parameter;
            }
            exception_state.SetEIP(value.base());
            break;

        case reg_reip:
            if (RoundToBundle(value.base()) != value.base()) {
               LOG_ERROR(LOG_EXCEPTION) << "Invalid REIP write value: " << fmt::hex(value.base());
               return Result::invalid_parameter;
            }
            exception_state.SetREIP(value.base());
            break;

        case reg_fpcr:     fpu.set_control_word(value.base());      break;

        case reg_eca:      exception_state.SetContext(value.base()); break;

        case reg_kip:      kip  = value.base();   break;
        case reg_krsp:     krsp = value.base();   break;
        case reg_ksp:      ksp  = value.base();   break;

        case reg_peb:      peb = value.base();    break;
        case reg_teb:      teb = value.base();    break;
        case reg_itc:      itc = value.base();    break;
        case reg_itm:      itm = value.base();    break;

        case reg_rsc:
            if (unlikely((value.base() & util::makemask<u64>(bits_per_register)) != value.base())) {
               LOG_ERROR(LOG_EXCEPTION) << "Invalid RSC write value: " << fmt::hex(value.base());
               return Result::invalid_parameter;
            }
            rsc.set(value.base());
            break;

        case reg_rsp:
            rsp = value.base() & ~util::makemask<u64>(log_bytes_per_address);
            break;

        case reg_bsp:
            bsp = value.base() & ~util::makemask<u64>(log_bytes_per_address);
            break;

        case reg_pta:
            return mmu.SetConfig(value.base());
            break;

        case reg_psr:
            if (unlikely((value.base() & ProcessorStatusRegister::FLAGS_ALL) != value.base())) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid psr write value: " << value;
                return Result::invalid_parameter;
            }
            psr.set(value.base());
            break;

        /*******************************************************************
        * interruptions handler
        *******************************************************************/

        case reg_iva:
            if (unlikely((value.base() & ~util::makemask<u64>(bits_per_iva_alignment)) != value.base())) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid iva write value: " << fmt::hex(value.base());
                return Result::invalid_parameter;
            }
            iva = value.base();
            break;

        case reg_ifa:
            ifa = value.base();
            break;

        case reg_iib:
            LOG_ERROR(LOG_EXCEPTION) << "invalid iib write";
            return Result::invalid_parameter;
            break;

        case reg_iip:
            iip = RoundToBundle(value.base());
            break;

        case reg_iipa:
            iipa = RoundToBundle(value.base());
            break;

        case reg_ipsr:
            ipsr.set(value.base());
            break;

        /*******************************************************************
        * external interrupts controller
        *******************************************************************/

        case reg_lid:
            if (unlikely((value.base() & util::makemask<u64>(bits_per_processor_id)) != value.base())) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid pid write value: " << fmt::hex(value.base());
                return Result::invalid_parameter;
            }
            pic.lid = static_cast<unsigned>(value.base());
            break;

        case reg_tpr:
            if (unlikely((value.base() & util::makemask<u64>(bits_per_int_vector)) != value.base())) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid tpr write value: " << fmt::hex(value.base());
                return Result::invalid_parameter;
            }
            pic.tpr = static_cast<u8>(value.base());
            break;

        case reg_itcv:     pic.itcv.Set(value.base());      break;
        case reg_tsv:      pic.tsv.Set(value.base());       break;
        case reg_pmv:      pic.pmv.Set(value.base());       break;
        case reg_cmcv:     pic.cmcv.Set(value.base());      break;

        case reg_irr0:
        case reg_irr1:
        case reg_irr2:
        case reg_irr3:
            LOG_ERROR(LOG_EXCEPTION) << "write to irr special registers";
            return Result::illegal_instruction;

        case reg_isr0:
        case reg_isr1:
        case reg_isr2:
        case reg_isr3:
            LOG_ERROR(LOG_EXCEPTION) << "write to isr special registers";
            return Result::illegal_instruction;

        case reg_iv:
            pic.EndOfInterrupt(value.base());
            break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register number in mtspr: " << regnum;
            return Result::illegal_instruction;
    }

    return Result::continue_execution;
}

Result
Core::OutputSpecialRegister(const DumpFormatter& out, const unsigned regnum) const
{
    switch (regnum) {
        case reg_ip:       out << fmt::hex(ip);               break;
        case reg_rsp:      out << fmt::hex(rsp);              break;
        case reg_bsp:      out << fmt::hex(bsp);              break;

        case reg_eip:      out << fmt::hex(exception_state.GetEIP()); break;
        case reg_reip:     out << fmt::hex(exception_state.GetREIP()); break;
        case reg_eca:      out << fmt::hex(exception_state.GetContext()); break;

        case reg_kip:      out << fmt::hex(kip);              break;
        case reg_krsp:     out << fmt::hex(krsp);             break;
        case reg_ksp:      out << fmt::hex(ksp);              break;

        case reg_peb:      out << fmt::hex(peb);              break;
        case reg_teb:      out << fmt::hex(teb);              break;
        case reg_itc:      out << fmt::hex(itc);              break;
        case reg_itm:      out << fmt::hex(itm);              break;
        case reg_iva:      out << fmt::hex(iva);              break;
        case reg_ifa:      out << fmt::hex(ifa);              break;
        case reg_iib:      out << fmt::hex(iib.get_hi())
                               << '_'
                               << fmt::hex(iib.get_lo());
                           break;
        case reg_iip:      out << fmt::hex(iip);              break;
        case reg_iipa:     out << fmt::hex(iipa);             break;
        case reg_fpcr:     out << fpu;                        break;
        case reg_rsc:      out << rsc;                        break;
        case reg_psr:      out << psr;                        break;
        case reg_ipsr:     out << ipsr;                       break;
        case reg_cause:    out << cause;                      break;
        case reg_pta:      out << mmu;                        break;

        case reg_tpr:
            out << fmt::hex<u64>(pic.tpr) << " vector=" << (pic.tpr & util::makemask<u64>(bits_per_int_vector));
            break;

        case reg_itcv:    out << pic.itcv;      break;
        case reg_tsv:     out << pic.tsv;       break;
        case reg_pmv:     out << pic.pmv;       break;
        case reg_cmcv:    out << pic.cmcv;      break;

        case reg_lid:     out << fmt::hex<u64>(pic.lid);    break;

        case reg_irr0:    out << fmt::hex(pic.irr[0].load());    break;
        case reg_irr1:    out << fmt::hex(pic.irr[1].load());    break;
        case reg_irr2:    out << fmt::hex(pic.irr[2].load());    break;
        case reg_irr3:    out << fmt::hex(pic.irr[3].load());    break;

        case reg_isr0:    out << fmt::hex(pic.isr[0]);    break;
        case reg_isr1:    out << fmt::hex(pic.isr[1]);    break;
        case reg_isr2:    out << fmt::hex(pic.isr[2]);    break;
        case reg_isr3:    out << fmt::hex(pic.isr[3]);    break;

        case reg_iv:      out << fmt::hex(u64(0)); break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "reserved special register number " << regnum;
            return Result::illegal_instruction;
    }

    return Result::continue_execution;
}

/***************************************************************************
* this function print processor state
***************************************************************************/
static void DumpGpr(GeneralRegister gpr, const char *name, const DumpFormatter& out, bool first, bool last)
{
    int const name_width = 5;
    bool const html = out.is_html();

    if (html) {
        if (first) {
            out << "<tr>";
        }
        out << "<td>";
    } else {
        out << std::right << std::setfill(' ') << std::setw(name_width);
    }

    out << name;

    if (html) {
        out << "</td><td><span class=\"dump\">";
    } else {
        out << std::left << ' ';
    }

    out << gpr;

    if (html) {
        out << "</span></td>";
        if (last) {
            out << "</tr>\n";
        }
    } else {
        if (last) {
            out << fmt::endl;
        }
    }
}

void
Core::dump_state(const DumpFormatter& out) const
{
    bool const html = out.is_html();

    if (html) {
        out << "\n<table><caption>special registers</caption>\n"
               "<thead><tr>"
               "<th>name</th><th>value</th>"
               "</tr></thead><tbody>\n";
    } else {
        out << "special registers:\n";
    }

    for (size_t i = 0; i < arch_spr_regtype.size(); i++) {
        if (arch_spr_regtype[i] != ESpecialRegisterSubset::reserved) {
            if (html) {
                out << "<tr><td>";
            } else {
                out << std::left << std::setfill(' ') << std::setw(8);
            }
            out << Decoder::GetSprName(i);
            if (html) {
                out  << "</td><td><span class=\"dump\">";
                (void)OutputSpecialRegister(out, i);
                out << "</span></td></tr>\n";
            } else {
                (void)OutputSpecialRegister(out, i);
                out << fmt::endl;
            }
        }
    }

    if (html) {
        out << "</tbody></table>\n";
    }

    unsigned const mask = html ? 1 : 3;

    if (html) {
        out << "\n<table><caption>general registers</caption><thead><tr>";
        for (unsigned i = 0; i <= mask; i++) {
            out << "<th>name</th><th>value</th>";
        }
        out << "</tr></thead><tbody>\n";
    } else {
        out << "\ngeneral registers:\n";
    }

    for (unsigned i = base_rotate_regs; i < base_rotate_regs + phys_stacked_regs; i++) {
        const unsigned n = rsc.rotate(i);
        char const *name = i < base_rotate_regs + max_stacked_framesize ? Decoder::GetGprName(i) : "---";
        DumpGpr(gpr[n], name, out, (i & mask) == 0, (i & mask) == mask);
    }

    for (unsigned i = base_static_regs; i < base_static_regs + num_static_regs; i++) {
        const unsigned n = rsc.RegStaticToPhys(i);
        DumpGpr(gpr[n], Decoder::GetGprName(i), out, (i & mask) == 0, (i & mask) == mask);
    }

    if (html) {
        out << "</tbody></table>";
    }
    out << '\n';

    cpuid.dump(out);
}

void
Core::dump_mapping(const PhysMemoryMapping& UNUSED(mapping), const DumpFormatter& UNUSED(out)) const
{
}

static const std::array<EInstructionFormat, insn_reserved> insn_format = {
#define X(name, subset, format, descr) inst_fmt_##format,
#include "arch/insn_table.hpp"
#undef X
};

static const std::array<util::default_initializer<
    Result (*)(Core& core, AddressSpace& as), &Core::insn_illegal>, insn_reserved> insn_table = {
#define X(name, subset, format, descr) \
    util::default_initializer<Result (*)(Core& core, AddressSpace& as), &Core::insn_illegal>(&Core::insn_##name),
#include "arch/insn_table.hpp"
#undef X
};

#define DO_INSN return insn_table[insn](core, as)

Result Core::trigger_register_fault(unsigned rnum) const
{
    LOG_ERROR(LOG_REGISTER_ROTATION | LOG_EXCEPTION)
        << "rotate reg: " << rnum
        << " SizeOfFrame=" << rsc.GetSizeOfFrame();
    return Result::register_window_protection;
}

DEFINE_INSN_FMT_IMPLEMENTATION(ld_imm)
{
    STATIC_ROTATE_REG(a)
    core.cmd.imm = cmd.get_ld_imm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(call)
{
    STATIC_ROTATE_REG(a)
    core.cmd.imm = cmd.GetJumpDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(mem_iprel)
{
    STATIC_ROTATE_REG(a)
    core.cmd.imm = cmd.GetForwardBranch();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(write)
{
    core.cmd.imm = cmd.GetForwardBranch();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(jmp)
{
    core.cmd.imm = cmd.GetJumpDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(nop)
{
    core.cmd.imm = cmd.get_ld_imm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(cache_simm)
{
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.get_bin_simm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(loop)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.GetLoopDisplacement();
    core.cmd.brc_imm = 1 + cmd.GetLoopStride();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(bin_simm)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.get_bin_simm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(bin_uimm)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.get_bin_uimm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(mid_bin_simm)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.get_mid_bin_simm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(br_eh)
{
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.GetBranchDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(br_rr)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.GetBranchDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(br_rs)
{
    STATIC_ROTATE_REG(a)
    core.cmd.rb = cmd.get_shamt_b();
    core.cmd.imm = cmd.GetBranchDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(br_simm)
{
    STATIC_ROTATE_REG(a)
    core.cmd.brc_imm = cmd.get_brc_simm();
    core.cmd.imm = cmd.GetBranchDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(br_uimm)
{
    STATIC_ROTATE_REG(a)
    core.cmd.brc_imm = cmd.get_brc_uimm();
    core.cmd.imm = cmd.GetBranchDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(nul_simm)
{
    STATIC_ROTATE_REG(a)
    core.cmd.brc_imm = cmd.get_brc_simm();
    core.cmd.dist_yes = cmd.get_nullify_yes();
    core.cmd.dist_no  = cmd.get_nullify_not();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(nul_uimm)
{
    STATIC_ROTATE_REG(a)
    core.cmd.brc_imm = cmd.get_brc_uimm();
    core.cmd.dist_yes = cmd.get_nullify_yes();
    core.cmd.dist_no  = cmd.get_nullify_not();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(nul_rs)
{
    STATIC_ROTATE_REG(a)
    core.cmd.rb = cmd.get_shamt_b();
    core.cmd.dist_yes = cmd.get_nullify_yes();
    core.cmd.dist_no  = cmd.get_nullify_not();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(nul_rr)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.dist_yes = cmd.get_nullify_yes();
    core.cmd.dist_no  = cmd.get_nullify_not();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(r4)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    STATIC_ROTATE_REG(c)
    STATIC_ROTATE_REG(d)
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(r3s1)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    STATIC_ROTATE_REG(c)
    core.cmd.rd = cmd.get_shamt_d();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(r2s2)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.rc = cmd.get_shamt_c();
    core.cmd.rd = cmd.get_shamt_d();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(r3s2)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    STATIC_ROTATE_REG(c)
    core.cmd.rd = cmd.get_shamt_d();
    core.cmd.re = cmd.get_shamt_e();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(gmemx)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    STATIC_ROTATE_REG(c)
    core.cmd.scale       = cmd.GetIndexedScale();
    core.cmd.imm         = cmd.GetIndexedDisplacement();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(RbcScale)
{
    STATIC_ROTATE_REG(b)
    STATIC_ROTATE_REG(c)
    core.cmd.scale = cmd.GetIndexedScale();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(Rbc)
{
    STATIC_ROTATE_REG(b)
    STATIC_ROTATE_REG(c)
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(alloc)
{
    core.cmd.rb = cmd.rb();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(alloc_sp)
{
    core.cmd.rb = cmd.rb();
    core.cmd.imm = cmd.get_bin_uimm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(retf)
{
    core.cmd.imm = cmd.get_bin_uimm();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(r3)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    STATIC_ROTATE_REG(c)
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(r2s1)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.rc = cmd.get_shamt_c();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(mspr)
{
    STATIC_ROTATE_REG(a)
    core.cmd.rc = cmd.rc();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(r2)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(gmemu)
{
    STATIC_ROTATE_REG(a)
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.GetUpdateStride();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(int)
{
    STATIC_ROTATE_REG(b)
    core.cmd.imm = cmd.GetUpdateStride();
    DO_INSN;
}

DEFINE_INSN_FMT_IMPLEMENTATION(NoArgs)
{
    UNREFERENCED_PARAMETER(cmd);
    DO_INSN;
}

template<typename CMD>
void Core::report_fault(AddressSpace& as, const CMD insn_cmd, Result result) const
{
    //util::cout_streamer streamer;
    //std::ostream& stream = streamer.stream();
    LOG_ERROR(LOG_EMULATOR) << "exception " << result //LOG_ERROR(LOG_EMULATOR)
       << fmt::endl
       << " core=" << pic.lid
       << " ip=" << fmt::hex(ip)
       << " ifa=" << fmt::hex(ifa)
       << fmt::endl
       << " cmd=" << insn_cmd
       << ": " << InstructionDisassembler<CMD>(as.get_decoder(), insn_cmd, ip)
       << fmt::endl
       << " psr=" << psr
       << fmt::endl;

    LOG_OUTPUT(LOG_LOADER) << result
        << " core=" << pic.lid
        << " ip=" << fmt::hex(ip) << " " << as.get_decoder().get_address_info(ip)
        << " psr=" << psr
        << " cmd=" << insn_cmd
        << ": " << InstructionDisassembler<CMD>(as.get_decoder(), insn_cmd, ip) << std::endl;

    // as.dump(stream);
}

Result Core::Raise(Result result)
{
#if defined(DEBUG_LINK_INFO_STACK)
        WITH_DEBUG(LOG_EXCEPTION) {
            while (!m_debug_link_info_stack.empty()) {
                const DebugLinkInfo item = m_debug_link_info_stack.back();
                m_debug_link_info_stack.pop_back();
                dbgs() << "stack:"
                   << " sp=" << fmt::hex(item.sp)
                   << " fp=" << fmt::hex(item.fp)
                   << " ip=" << as.get_decoder().get_address_info(item.ip)
                   << std::endl;
            }
        }
#endif

    LOG_DEBUG(LOG_EXCEPTION) << (result.IsFault() ? "fault" : "trap")
                             << ": iv=" << result
                             << " psr.rs=" << psr.rs.get();

    if (result.IsFault()) {
        // Don't retire instruction
    } else {
        assert(result.IsTrap());
        // Retire instruction
        psr.rs.extend(cmd.num_slots);
        if (psr.rs.get() == slots_per_bundle) {
            ip += sizeof(Bundle);
            psr.rs.clear();
        }
    }

    SetupDebugPlace(ip, psr.rs.get());

    LOG_DEBUG(LOG_EMULATOR)
        << "\nevent: ip=" << fmt::hex(ip)
        << " iipa=" << fmt::hex(iipa)
        << " result=" << result << fmt::endl
        << "psr=" << psr << fmt::endl;

    const unsigned code = result.GetVector();

    // interruptions are disabled but interruption occurs
    //if( psr.IsInterruptDisabled() )
    //{
    //    out << "machine abort: ip=" << fmt::hex(ip) << fmt::endl;
    //    return Result::double_fault;
    //}

    // check if interruption collection possible and save program state
    //if( psr.is_interrupt_collection_enabled() )
    //{
    //    psr.disable_interrupt_collection();
        ipsr            = psr;
        iip             = ip;
        cause.u.vector  = code;
    //} else {
    //    LOG_ERROR(LOG_EMULATOR) << "red state: ip=" << fmt::hex(ip);
    //    return Result::double_fault;
    //}

    // compute entry into interruption vector table
    // TODO
    // ExecuteJump(iva | (u64(code) << bits_per_vector_handler));

    return result;
}

template<typename CMD> ALWAYS_INLINE Result
Core::ExecuteInstruction(AddressSpace& as, const CMD insn_cmd)
{
    static const insn_fmt_handler<CMD> inst_fmt_table[inst_fmt_last] = {
#define INSTRUCTION_FORMAT_HANDLER_X(name, extname, descr, format, txt) insn_fmt_handler_##name<CMD>,
        DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_HANDLER_X)
    };

    if (unlikely(0 == (itc--))) {
        // LOG_ERROR(LOG_EXCEPTION) << "instruction limit exceed (itc)";
        dbgs() << "instruction limit exceed (itc)" << std::endl;
        return Result::instruction_limit;
    }

    bool const is_nullified = psr.future.pop();
    LOG_DEBUG(LOG_INSN_TRACE)
        << "psr.rs=" << psr.rs.get()
        << (is_nullified ? " nul" : "    ")
        << ' ' << insn_cmd
        << ' ' << InstructionDisassembler<CMD>(as.get_decoder(), insn_cmd, ip);

    performance_monitor.count(insn_cmd);

    Result result = Result::continue_execution;
    cmd.num_slots = CMD::SlotsNum;

    if (unlikely(is_nullified)) {
        LOG_DEBUG(LOG_PREDICATION) << "nullified " << insn_cmd;
        performance_monitor.report_event(PMC_QUALIFIED_NOP_INSTRUCTION);
    } else {
        EInstructionID const id = Decoder::Decode(insn_cmd);
        m_instructionStatisticCollector.count(id);
        result = inst_fmt_table[insn_format[id]](*this, as, id, insn_cmd);
    }

    LOG_DEBUG(LOG_DECODER) << "result=" << result;

    if (likely(result.success())) {  // continue or jump

        // check if we have async interrupt
        if (unlikely(pic.IsInterruptRequest())) {
            const unsigned vector = pic.read_vector();
            LOG_DEBUG(LOG_EMULATOR)
                << "vector=" << vector
                << " core=" << pic.lid
                << " ip=" << fmt::hex(ip)
                << " psr=" << psr
                << " cmd=" << insn_cmd
                << ": " << InstructionDisassembler<CMD>(as.get_decoder(), insn_cmd, ip);

            return Raise(Result::ConvertFromVector(vector));
        }

        if (likely(result.continuable())) {
            psr.rs.extend(CMD::SlotsNum);
        }

        return result;

    } else { // !result.success()

        if (result != Result::halt) {
            report_fault<CMD>(as, insn_cmd, result);
        }

        result = Raise(result);
    }

    return result;
}

/***************************************************************************
* this function execute one bundle
***************************************************************************/
ALWAYS_INLINE
Result
Core::ExecuteBundle(AddressSpace& as, const Bundle bundle)
{
    const unsigned restarting_slot = psr.rs.get();

    LOG_DEBUG(LOG_INSN_TRACE)
        << "bundle: " << bundle
        << " restarting_slot=" << restarting_slot
        << " ip=" << as.get_decoder().get_address_info(ip);

    performance_monitor.report_event(PMC_SHADOWED_SLOT, restarting_slot);

    if (unlikely(m_config_flags & CONFIG_PROFILE)) {
        m_profile[ip]++;
    }

    ++fetched_bundles;
    iipa = ip;

    Result result;
    EBundleTemplate const bundle_template = bundle.GetTemplate();
#define CASE_X(SLOT_NUM, TEMPLATE) ((SLOT_NUM) * 4 + static_cast<unsigned>(TEMPLATE))

    switch (CASE_X(restarting_slot, bundle_template)) {
        case CASE_X(0, EBundleTemplate::bundle_sss):
            LOG_DEBUG(LOG_DECODER) << "sss: " << bundle;
            result = ExecuteInstruction(as, bundle.s0());
            if (unlikely(!result.continuable())) break;
            [[fallthrough]];

        case CASE_X(1, EBundleTemplate::bundle_sss):
            result = ExecuteInstruction(as, bundle.s1());
            if (unlikely(!result.continuable())) break;
            [[fallthrough]];

        case CASE_X(2, EBundleTemplate::bundle_sss):
            result = ExecuteInstruction(as, bundle.s2());
            break;

        case CASE_X(0, EBundleTemplate::bundle_sll):
            LOG_DEBUG(LOG_DECODER) << "sll: " << bundle;
            result = ExecuteInstruction(as, bundle.s0());
            if (unlikely(!result.continuable())) break;
            [[fallthrough]];

        case CASE_X(1, EBundleTemplate::bundle_sll):
            result = ExecuteInstruction(as, bundle.l1());
            break;

        case CASE_X(0, EBundleTemplate::bundle_lls):
            LOG_DEBUG(LOG_DECODER) << "lls: " << bundle;
            result = ExecuteInstruction(as, bundle.l0());
            if (unlikely(!result.continuable())) break;
            [[fallthrough]];

        case CASE_X(2, EBundleTemplate::bundle_lls):
            result = ExecuteInstruction(as, bundle.s2());
            break;

        case CASE_X(2, EBundleTemplate::bundle_sll):
            LOG_ERROR(LOG_EXCEPTION) << "attempt to return into middle of long instruction (SLL)";
            result = Result::invalid_bundle_slot;
            break;

        case CASE_X(1, EBundleTemplate::bundle_lls):
            LOG_ERROR(LOG_EXCEPTION) << "attempt to return into middle of long instruction (LLS)";
            result = Result::invalid_bundle_slot;
            break;

        case CASE_X(0, EBundleTemplate::bundle_lll):
        case CASE_X(1, EBundleTemplate::bundle_lll):
        case CASE_X(2, EBundleTemplate::bundle_lll):
            LOG_ERROR(LOG_EXCEPTION) << "invalid bundle template";
            result = Result::illegal_bundle_template;
            break;

        case CASE_X(3, EBundleTemplate::bundle_sss):
        case CASE_X(3, EBundleTemplate::bundle_sll):
        case CASE_X(3, EBundleTemplate::bundle_lls):
        case CASE_X(3, EBundleTemplate::bundle_lll):
        default:
            LOG_ERROR(LOG_EXCEPTION) << "invalid restarting slot";
            result = Result::invalid_bundle_slot;
            break;
    }

    if (likely(result.continuable())) {
        assert(psr.rs.get() == slots_per_bundle);
        psr.rs.clear();
        ip += sizeof(Bundle);
        LOG_DEBUG(LOG_EMULATOR) << "fin bundle\n";
        return result;
    } else {
        if (!result.success()) {
            iib = bundle;
        }
    }

    LOG_DEBUG(LOG_EMULATOR) << "fin bundle with stop\n";
    return result;
}

void
Core::SetIp(const u64 start_ip)
{
    ip = start_ip;
    iip = start_ip;
    iipa = start_ip;
    ipsr = psr;
    SetupDebugPlace(ip, 0);
}

Result
Core::execute_until_interruption(AddressSpace& as)
{
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    ip = iip;
    psr = ipsr;
    LOG_DEBUG(LOG_LOADER) << "running: core=" << pic.lid
                          << " ip=" << fmt::hex(ip)
                          << " psr=" << psr;
    Result Result(Result::continue_execution);
    while (Result.success()) {
        const u64 cacheMask = ~static_cast<u64>(ICACHE_LINESIZE - 1);
        const VirtAddress fetch_ip(ip & cacheMask);
        LOG_DEBUG(LOG_DECODER) << "fetch ip=" << fetch_ip;
        InstructionCacheLine<ICACHE_LINESIZE> bundles;
        Result = as.mmu_fetch_code(*this, fetch_ip, sizeof(bundles), &bundles);
        assert(!Result.is_taken_branch());
        while (Result.success() && ((ip & cacheMask) == fetch_ip.get_address())) {
            Result = ExecuteBundle(as, bundles.GetBundle(ip));
        }
    }
    m_Stopped = Result.halted();

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    const auto diff = end - start;
    const u64 run_time = std::chrono::duration <u64, std::nano> (diff).count();
    performance_monitor.report_event(PMC_RUNTIME, run_time);
    return Result;
}

class BundleAddressSorter {
public:
    BundleAddressSorter(u64 _address, u64 _freq) : address(_address), freq(_freq) {}
    bool operator>(const BundleAddressSorter& r) const {
        return freq > r.freq || (freq == r.freq && address < r.address);
    }
public:
    u64 address;
    u64 freq;
};

void
Core::DumpProfile(const DumpFormatter& out) const
{
    std::vector<BundleAddressSorter> freq_info;
    u64 all = 0;
    for (const auto& x : m_profile) {
        if (x.second > 0) {
            freq_info.emplace_back(x.first, x.second);
            all += x.second;
        }
    }

    std::sort(freq_info.begin(), freq_info.end(), std::greater<BundleAddressSorter>());

    out << "<table><caption>profile</caption>\n"
           "<thead>"
           "<tr>"
           "<th>position</th>"
           "<th>frequency</th>"
           "<th>percent</th>\n"
           "<th>cumulative<br/>percent</th>\n"
           "<th>address</th>"
           "</tr>"
           "</thead><tbody>\n";

    u64 reported = 0;
    int position = 0;
    double const percent = 100.0 / all;

    out << std::fixed << std::setprecision(6);

    for (const auto& x : freq_info) {
        reported += x.freq;
        if (++position > 200) break;

        out << "<tr><td>" << position << "</td><td>"
            << x.freq << "</td><td>"
            << (x.freq   * percent) << "</td><td>"
            << (reported * percent) << "</td><td>"
            << out.get_decoder().get_address_info(x.address) << "</td></tr>\n";
    }
    out << "</tbody></table>\n\n";
}

void
Core::dump_statistic(const DumpFormatter& out) const
{
    if (out.is_html()) {
        out << "<pre>\n";
    }
    out << "core id=" << pic.lid << fmt::endl;
    Device::dump_statistic(out);
    if (out.is_html()) {
        out << "</pre>\n";
    }

    if (out.is_html()) {
        out << "<pre>\n";
    }
    config.dump(out);
    if (out.is_html()) {
        out << "</pre>\n";
    }

    dump_state(out);
    performance_monitor.dump_statistic(fetched_bundles, m_config_flags & CONFIG_TIMING, out);

    m_instructionStatisticCollector.dump(out, "Dynamic instruction frequencies");

    if (m_config_flags & CONFIG_PROFILE) {
        DumpProfile(out);
    }
}

void
Core::DisassembleDynamic(AddressSpace& as, std::ostream& out, i64 start, i64 stop)
{
    DecoderContext decoderContext(disasm_mode_html);

    LOG_DEBUG(LOG_EMULATOR) << "<pre>debug_ip=0x" << fmt::hex(debug_ip)
        << " ip=0x" << fmt::hex(ip)
        << " debug_restart_slot=" << debug_restart_slot
        << " psr.rs=" << psr.rs.get()
        << "</pre>" << fmt::endl;

    for (i64 offset = start; offset < stop; offset += sizeof(Bundle)) {
        u64 const address = debug_ip + offset;
        LOG_DEBUG(LOG_EMULATOR) << "address=" << fmt::hex(address);

        InstructionCacheLine<ICACHE_LINESIZE> bundles;
        const VirtAddress cacheLine(address & ~u64(ICACHE_LINESIZE-1));
        const Result result = as.mmu_fetch_code(*this, cacheLine, sizeof(bundles), &bundles);
        if (!result.success()) {
            out << "<pre>" << fmt::hex(address) << " error</pre>" << fmt::endl;
            continue;
        }

        Bundle const& bundle = bundles.GetBundle(address);

        // we assume that this function is called from debug trap
        // check if it's highlighted bundle and pass restarting slot
        // otherwise pass invalid for non-highlighted
        if (offset == 0) {
            decoderContext.restart_slot = debug_restart_slot;
            decoderContext.future_predication = psr.future.get();
        } else {
            decoderContext.restart_slot = DecoderContext::unmatched;
            // but don't zero future
        }

        out << "<pre>";
        as.get_decoder().DisassembleBundle(bundle, address, out, decoderContext);
        out << "</pre>";
    }
}

Result
Core::PrepareThread(u64 new_bsp, u64 sp, u64 bp, u64 tp, u64 eip)
{
    exception_state.SetEIP(eip);
    exception_state.SetREIP(eip);

    w_static_reg(reg_sp) = sp;
    w_static_reg(reg_fp) = sp;
    rsp = bp;
    bsp = new_bsp;
    w_static_reg(reg_tp) = tp;

    LOG_DEBUG(LOG_LOADER | LOG_EMULATOR) << "  rsp        = " << fmt::hex(rsp);
    LOG_DEBUG(LOG_LOADER | LOG_EMULATOR) << "  tp         = " << fmt::hex(r_static_reg(reg_tp).base());

    return Result::continue_execution;
}

} // namespace postrisc
