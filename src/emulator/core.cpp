#include <chrono>      // for std::chrono::steady_clock/duration/etc

#include "util/common.hpp"
#include "configuration.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"

#include "arch/abi.hpp"

#include "core.hpp"
#include "utils.hpp"
#include "debugger.hpp"
#include "binary_format/file_header.hpp"

#include "instructions/base.hpp"
#include "instructions/atomic.hpp"
#include "instructions/system.hpp"
#include "instructions/fpu.hpp"
#include "instructions/vector.hpp"
#include "instructions/aes.hpp"
#include "instructions/emulator.hpp"

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

Core::Core(TargetSystem *targetSystem, const std::string& name, uint32_t core_id, unsigned flags)
    : Device(targetSystem, name)
    , random(core_id)
    , config_flags(flags)
    , m_Stopped(false)
{
}

Core::~Core(void)
{
}

uint64_t
Core::size(void) const
{
    // core should not be mapped as normal MMIO device, only config space is available
    return 0;
}

uint64_t
Core::get_config_size(void) const
{
    return sizeof(config);
}

Core *
Core::get_bootstrap_core(void)
{
    return this;
}

CStatus
Core::read(device_address_t address, size_t len, void* UNUSED(bytes))
{
    LOG_FATAL(LOG_INTERRUPT) << "address=" << fmt::hex(address) << " len=" << len;
    return CStatus(CStatus::device_error);
}

CStatus
Core::write(device_address_t address, size_t len, const void* UNUSED(bytes))
{
    LOG_FATAL(LOG_INTERRUPT) << "address=" << fmt::hex(address) << " len=" << len;
    return CStatus(CStatus::device_error);
}

CStatus
Core::read_config(device_address_t address, size_t len, void *bytes)
{
    return read_config_raw(address, &config, sizeof(config), len, bytes);
}

CStatus
Core::write_config(device_address_t address, size_t len, const void *bytes)
{
   typedef CoreConfigSpace space;

    LOG_OUTPUT(LOG_INTERRUPT)
         << "CoreArray write: pid=" << pic.lid
         << " address=" << fmt::hex(address)
         << " len=" << len << fmt::endl;

    if (address == offsetof(space, m_InterruptVector)
        && len == STRUCT_MEMBER_SIZE(space, m_InterruptVector))
    {
        uint64_t iv = *reinterpret_cast<uint64_t const*>(bytes);
        LOG_OUTPUT(LOG_INTERRUPT) << "iv=" << fmt::hex(iv) << fmt::endl;
        CHECK_STATUS(post_interrupt(CStatus(iv)));
    }
    else if (address == offsetof(space, m_TimeCmp)
        && len == STRUCT_MEMBER_SIZE(space, m_TimeCmp))
    {
        uint64_t timecmp = *reinterpret_cast<uint64_t const*>(bytes);
        LOG_OUTPUT(LOG_INTERRUPT) << "timecmp=" << fmt::hex(timecmp) << fmt::endl;
        config.m_TimeCmp = timecmp;
        // return CStatus(CStatus::continue_execution);
    } else {
        return CStatus(CStatus::device_error);
    }

    return write_config_raw(address, &config, sizeof(config), len, bytes);
}

void
Core::dump_config(uint64_t address, const DumpFormatter& out) const
{
    dump_config_raw(address, reinterpret_cast<uint8_t const*>(&config), sizeof(config), out);
}

CStatus
Core::complex_operation(device_address_t address, IMemoryOperation& UNUSED(op))
{
    LOG_ERROR(LOG_PLATFORM) << "invalid atomic: " << fmt::hex(address);
    return CStatus(CStatus::device_error);
}



void
Core::FlushTranslationBuffer(void)
{
    m_DataTranslationCache.Flush();
    m_CodeTranslationCache.Flush();
}

void
Core::FlushTranslationBuffer(uint64_t virt_address)
{
    m_DataTranslationCache.Flush(virt_address, mmu.GetPageOffsetSize());
    m_CodeTranslationCache.Flush(virt_address, mmu.GetPageOffsetSize());
}

void
Core::ResetCodeCache(void)
{
    m_instruction_cache_tags.fill(0);
    // cache_data.fill(???);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// exception helpers
/////////////////////////////////////////////////////////////////////////////////////////////

#define CHECK_ALIGNMENT(helper, address, size)                \
    if (unlikely((address | size) & (size - 1))) {            \
        LOG_ERROR(LOG_EXCEPTION)                              \
            << "unaligned address: " << fmt::hex(address)     \
            << " size=" << size;                              \
                                                              \
        return helper::unaligned_address();                   \
    }                                                         \

class CInstructionHelper {
public:
    static CStatus invalid_address      () { return CStatus(CStatus::instruction_invalid_address); }
    static CStatus unaligned_address    () { return CStatus(CStatus::instruction_unaligned_address); }
    static CStatus unimplemented_address() { return CStatus(CStatus::instruction_unimplemented_address); }
    static CStatus unsupported_reference() { return CStatus(CStatus::instruction_unsupported_reference); }
    static CStatus tlb_miss             () { return CStatus(CStatus::instruction_tlb_miss); }
    static CStatus page_not_present     () { return CStatus(CStatus::instruction_page_not_present); }
    static CStatus access_right         () { return CStatus(CStatus::instruction_access_right); }
    static CStatus dirty_bit            () { return CStatus(CStatus::instruction_dirty_bit); }
    static CStatus access_bit           () { return CStatus(CStatus::instruction_access_bit); }
    static CStatus debug                () { return CStatus(CStatus::instruction_debug); }
    static void report_tlb_hit(CPerformanceMonitor& pm) { pm.report_event(PMC_INSTRUCTION_TRANSLATION_HIT); }
    static void report_tlb_miss(CPerformanceMonitor& pm) { pm.report_event(PMC_INSTRUCTION_TRANSLATION_MISS); }
};

class CDataHelper {
public:
    static CStatus invalid_address      () { return CStatus(CStatus::data_invalid_address); }
    static CStatus unaligned_address    () { return CStatus(CStatus::data_unaligned_address); }
    static CStatus unimplemented_address() { return CStatus(CStatus::data_unimplemented_address); }
    static CStatus unsupported_reference() { return CStatus(CStatus::data_unsupported_reference); }
    static CStatus tlb_miss             () { return CStatus(CStatus::data_tlb_miss); }
    static CStatus page_not_present     () { return CStatus(CStatus::data_page_not_present); }
    static CStatus access_right         () { return CStatus(CStatus::data_access_right); }
    static CStatus dirty_bit            () { return CStatus(CStatus::data_dirty_bit); }
    static CStatus access_bit           () { return CStatus(CStatus::data_access_bit); }
    static CStatus debug                () { return CStatus(CStatus::data_debug); }
    static void report_tlb_hit(CPerformanceMonitor& pm) { pm.report_event(PMC_DATA_TRANSLATION_HIT); }
    static void report_tlb_miss(CPerformanceMonitor& pm) { pm.report_event(PMC_DATA_TRANSLATION_MISS); }
};

class CBackstoreHelper {
public:
    static CStatus invalid_address      () { return CStatus(CStatus::backstore_invalid_address); }
    static CStatus unaligned_address    () { return CStatus(CStatus::backstore_unaligned_address); }
    static CStatus unimplemented_address() { return CStatus(CStatus::backstore_unimplemented_address); }
    static CStatus unsupported_reference() { return CStatus(CStatus::backstore_unsupported_reference); }
    static CStatus tlb_miss             () { return CStatus(CStatus::backstore_tlb_miss); }
    static CStatus page_not_present     () { return CStatus(CStatus::backstore_page_not_present); }
    static CStatus access_right         () { return CStatus(CStatus::backstore_access_right); }
    static CStatus dirty_bit            () { return CStatus(CStatus::backstore_dirty_bit); }
    static CStatus access_bit           () { return CStatus(CStatus::backstore_access_bit); }
    static CStatus debug                () { return CStatus(CStatus::backstore_debug); }
    static void report_tlb_hit(CPerformanceMonitor& pm) { pm.report_event(PMC_BACKSTORE_TRANSLATION_HIT); }
    static void report_tlb_miss(CPerformanceMonitor& pm) { pm.report_event(PMC_BACKSTORE_TRANSLATION_MISS); }
};

/////////////////////////////////////////////////////////////////////////////////////////////
// translation and page table access
/////////////////////////////////////////////////////////////////////////////////////////////
template<class helper>
CStatus
Core::translate_address(
    CCoreTranslationCache & tc,
    const uint64_t virt_address,
    uint64_t requested_rights,
    uint64_t unexpected_rights,
    phys_address_t& phys_address)
{
    if (unlikely(mmu.is_real_mode())) {
        LOG_TRACE(LOG_PAGETABLE) << "no translation, address=" << fmt::hex(virt_address);
        phys_address = virt_address;
        return CStatus(CStatus::continue_execution);
    }

    LOG_TRACE(LOG_PAGETABLE)
        << "virt_address=" << fmt::hex(virt_address)
        << " rights=" << fmt::hex(requested_rights);

    if (unlikely(!mmu.is_canonical(virt_address))) {
        LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
            << "noncanonical virtual address=" << fmt::hex(virt_address);

        return helper::invalid_address();
    }

    // uint64_t ptx[numberOfPagingLevels];
    uint64_t const vpn = virt_address >> mmu.GetPageOffsetSize();
    unsigned const idx = static_cast<unsigned>(vpn % tc.cache.size());

    if (likely(tc.cache[idx].virt_address == vpn)) {
        helper::report_tlb_hit(performance_monitor);
        phys_address = tc.cache[idx].phys_address;

        LOG_TRACE(LOG_PAGETABLE)
            << "TLB hit: va=" << fmt::hex(virt_address)
            << " pa=" << fmt::hex(phys_address);
    } else {
        LOG_TRACE(LOG_PAGETABLE | LOG_EXCEPTION) << "TLB miss: va=" << fmt::hex(virt_address);
        helper::report_tlb_miss(performance_monitor);
        // walk page table
        phys_address = mmu.GetPageTableRootAddress();
        for (int level = mmu.GetNumberOfPagingLevels() - 1; level >= 0; --level) {
            uint64_t const next_te_pa = mmu.get_next_te_pa(virt_address, phys_address, level);
            uint64_t next_pta;
            CHECK_STATUS(get_target_system()->read(next_te_pa, sizeof(next_pta), &next_pta));

            if (unlikely(!CTranslationEntry(next_pta).is_valid())) {
                LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
                    << "invalid pta: level=" << level
                    << " va=" << fmt::hex(virt_address)
                    << " pta=" << fmt::hex(next_pta);

                return helper::page_not_present();
            }
            phys_address = next_pta;
        }

        LOG_TRACE(LOG_PAGETABLE)
            << "walk pt: va=" << fmt::hex(virt_address)
            << " pa=" << fmt::hex(phys_address);

        tc.cache[idx].virt_address = vpn;
        tc.cache[idx].phys_address = phys_address;
    }

    // check access
    if (unlikely((phys_address & (requested_rights | unexpected_rights)) != requested_rights)) {
        LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
                  << "requested rights error: virt_address=" << fmt::hex(virt_address)
                  << " requested rights=" << fmt::hex(requested_rights)
                  << " unexpected_rights=" << fmt::hex(unexpected_rights)
                  << " current rights=" << fmt::hex<unsigned>(phys_address & VM_PAGE_RIGHT_MASK);

        return helper::access_right();
    }

    uint64_t const page_offset = mmu.ComputePageOffset(virt_address);

    // reconstruct
    // uint64_t raddress = page_offset;

    //for(int i=GetNumberOfPagingLevels() - 1; i >= 0; --i) {
    //   raddress |= uint64_t(idx[i]) << (GetPageOffsetSize() + i * GetBitsPerPageIndex());
    //}

    //std::cout << "reconstruction=" << fmt::hex(raddress)
    //          << " " << fmt::hex(compute_canonical_address(raddress)) << std::endl;

    phys_address = (phys_address & VM_PAGE_PHYS_ADDRESS_MASK) | page_offset;

    LOG_TRACE(LOG_PAGETABLE)
        << "page_offset=" << fmt::hex(page_offset)
        << " result=" << fmt::hex(phys_address);

    return CStatus(CStatus::continue_execution);
}

/////////////////////////////////////////////////////////////////////////////////////////////
// MMU interface for CPU
/////////////////////////////////////////////////////////////////////////////////////////////
CStatus
Core::mmu_fetch_code(uint64_t virt_address, InstructionCacheLine<ICACHE_LINESIZE> & codeChunk)
{
    LOG_TRACE(LOG_ICACHE)
        << "fetch bundle: " << fmt::hex(virt_address);

    // CHECK_ALIGNMENT(CInstructionHelper, virt_address, sizeof(CBundle));

    // for instruction read only we use special instruction translation lookaside buffer (ITLB)
    uint64_t phys_address;
    CHECK_STATUS(translate_address<CInstructionHelper>(m_CodeTranslationCache,
                 virt_address,
                 VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE,
                 phys_address));

    // serach instruction cache
    phys_address_t const tag = phys_address / ICACHE_LINESIZE;
    size_t const index = tag % m_instruction_cache_data.size();

    phys_address_t const cache_address = util::align_down<phys_address_t>(phys_address, ICACHE_LINESIZE);

    if (unlikely(m_instruction_cache_tags[index] != cache_address)) {
        // icache miss, refill
        performance_monitor.report_event(PMC_ICACHE_MISS);
        LOG_TRACE(LOG_ICACHE) << "icache miss: " << fmt::hex(cache_address);
        CHECK_STATUS(get_target_system()->read(cache_address,
            sizeof(m_instruction_cache_data[index]),
            &m_instruction_cache_data[index]));
        m_instruction_cache_tags[index] = cache_address;
    } else {
        performance_monitor.report_event(PMC_ICACHE_HIT);
        LOG_TRACE(LOG_ICACHE) << "icache hit: " << fmt::hex(cache_address);
    }

    codeChunk = m_instruction_cache_data[index];

    return CStatus(CStatus::continue_execution);
}

CStatus
Core::mmu_read(uint64_t virt_address, size_t len, void * data)
{
    LOG_TRACE(LOG_LOAD)
        << "read " << len << "bytes, "
        << fmt::hex(virt_address);

    CHECK_ALIGNMENT(CDataHelper, virt_address, len);

    uint64_t phys_address;
    CHECK_STATUS(translate_address<CDataHelper>(m_DataTranslationCache,
                 virt_address,
                 VM_PAGE_RIGHT_READ,
                 VM_PAGE_RIGHT_NONE,
                 phys_address));

    return get_target_system()->read(phys_address, len, data);
}

CStatus
Core::mmu_read_finalized(uint64_t virt_address, size_t len, void * data)
{
    LOG_TRACE(LOG_LOAD)
        << "read finalized " << len << " bytes, "
        << fmt::hex(virt_address);

    CHECK_ALIGNMENT(CDataHelper, virt_address, len);

    uint64_t phys_address;
    CHECK_STATUS(translate_address<CDataHelper>(m_DataTranslationCache,
                 virt_address,
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_FINAL,
                 VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE,
                 phys_address));

    return get_target_system()->read(phys_address, len, data);
}

CStatus
Core::mmu_write(uint64_t virt_address, size_t len, const void * data)
{
    LOG_TRACE(LOG_STORE) << "write " << len << " bytes, " << fmt::hex(virt_address);

    CHECK_ALIGNMENT(CDataHelper, virt_address, len);

    uint64_t phys_address;
    CHECK_STATUS(translate_address<CDataHelper>(m_DataTranslationCache,
                 virt_address,
                 VM_PAGE_RIGHT_WRITE,
                 VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_BACKING_STORE,
                 phys_address));

    return get_target_system()->write(phys_address, len, data);
}

CStatus
Core::mmu_read_backing_store(uint64_t virt_address, size_t len, void * data)
{
    LOG_TRACE(LOG_LOAD)
        << "read backstore " << len << " bytes, "
        << fmt::hex(virt_address);

    CHECK_ALIGNMENT(CBackstoreHelper, virt_address, len);

    uint64_t phys_address;
    CHECK_STATUS(translate_address<CBackstoreHelper>(m_DataTranslationCache,
                 virt_address,
                 VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL,
                 VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_EXECUTE, // usual read is allowed
                 phys_address));

    return get_target_system()->read(phys_address, len, data);
}

CStatus
Core::mmu_write_backing_store(uint64_t virt_address, size_t len, const void * data)
{
    LOG_TRACE(LOG_STORE)
        << "write backstore "
        << len << " bytes, " << fmt::hex(virt_address);

    CHECK_ALIGNMENT(CBackstoreHelper, virt_address, len);

    uint64_t phys_address;
    CHECK_STATUS(translate_address<CBackstoreHelper>(m_DataTranslationCache,
                 virt_address,
                 VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL,
                 VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_EXECUTE, // usual read is allowed
                 phys_address));

    return get_target_system()->write(phys_address, len, data);
}

CStatus
Core::mmu_translate(uint64_t virt_address, phys_address_t& phys_address)
{
    return translate_address<CDataHelper>(m_DataTranslationCache, virt_address, 0, 0, phys_address);
}

CStatus
Core::mmu_complex_operation(uint64_t virt_address, uint64_t requested_rights, IMemoryOperation & op)
{
    const size_t len = op.size();

    LOG_TRACE(LOG_STORE)
        << "atomic "
        << len << " bytes, " << fmt::hex(virt_address);

    CHECK_ALIGNMENT(CDataHelper, virt_address, len);

    uint64_t phys_address;
    CHECK_STATUS(translate_address<CDataHelper>(m_DataTranslationCache,
                              virt_address,
                              requested_rights,
                              VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
                              phys_address));

    return get_target_system()->complex_operation(phys_address, op);
}

inline
CStatus
Core::FinalizeInstruction()
{
    return CStatus(CStatus::continue_execution);
}

inline
CStatus
Core::ExecuteJump(uint64_t target)
{
    LOG_TRACE(LOG_BRANCH)
        << "ip=" << fmt::hex(ip)
        << "target=" << fmt::hex(target);

    // save branch source info in iipa
    iipa = ip | psr.GetShadowSize();

    // avoid trap-handler shadow setup for incomplete branch
    psr.ClearShadow();

    // clear possible future predication mask
    psr.ClearFuture();

    ip = target;

    return CStatus(CStatus::taken_branch);
}

CStatus
Core::post_interrupt(CStatus vector_index)
{
    if (IsStopped()) {
        // ignore interrupt broadcast
        return CStatus(CStatus::continue_execution);
    }
    LOG_TRACE(LOG_INTERRUPT) << "vector_index=" << vector_index;
    return pic.post_interrupt(vector_index);
}

/***************************************************************************
* this function do processor startup.
***************************************************************************/
void
Core::reset(const unsigned coreid)
{
    gpr.fill(CGeneralRegister(0));

    ip = 0;
    iip = 0;
    iipa = 0;

    fpu.set_control_word(0);

    rsp = 0;
    itc = 0;
    peb = 0;
    teb = 0;

    kip  = UINT64_C(0xFFFFFFFF00000000);
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

    debug_ip = ip;
    debug_ri = 1;

}

/***************************************************************************
* this function perform interruption
* don't call it directly, only raise & trap
***************************************************************************/
void
Core::TriggerEvent(CStatus event)
{
    LOG_TRACE(LOG_EMULATOR)
        << "\nevent: ip=" << fmt::hex(ip)
        << " iipa=" << fmt::hex(iipa)
        << " event=" << event << fmt::endl
        << "psr=" << psr << fmt::endl;

    const unsigned code = event.GetVector();

    // interruptions are disabled but interruption occurs
    //if( psr.IsInterruptDisabled() )
    //{
    //    out << "machine abort: ip=" << fmt::hex(ip) << fmt::endl;
    //    return CStatus::double_fault;
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
    //    return CStatus::double_fault;
    //}

    // compute entry into interruption vector table
    // TODO
    // ExecuteJump(iva | (uint64_t(code) << bits_per_vector_handler));
    // return event;
}

void
Core::RaiseFault(const CStatus status)
{
    // check if error occurs after fetch (and ip update) in some instruction
    if (psr.GetShadowSize() > 0) {
        // restart current bundle
        psr.Raise();
        ip -= sizeof(CBundle);    // stay in current bundle
    } else {
        LOG_ERROR(LOG_EXCEPTION | LOG_PREDICATION) << "first instruction, no rollback";
    }

    TriggerEvent(status);
}

static
unsigned
GetBundleInstructionNumber(EBundleTemplate bundle_template)
{
    switch (bundle_template)
    {
        case EBundleTemplate::bundle_sss:  return 3;
        case EBundleTemplate::bundle_sll:  return 2;
        case EBundleTemplate::bundle_lls:  return 2;
        case EBundleTemplate::bundle_lll:
            LOG_FATAL(LOG_EXCEPTION | LOG_PREDICATION) << "invalid bundle template after decode";
            return -1;
    }

    LOG_FATAL(LOG_EXCEPTION | LOG_PREDICATION) << "unpredicted bundle template after decode";
    return -2;
}

void
Core::RaiseTrap(const CStatus status, const CBundle bundle)
{
    // check if all bundle instructions are in the predication shadow
    const unsigned ninsn = GetBundleInstructionNumber(bundle.GetTemplate());
    const unsigned shadow_size = psr.GetShadowSize();

    LOG_TRACE(LOG_EXCEPTION | LOG_PREDICATION)
        << "shadow_size=" << shadow_size
        << " ninsn=" << ninsn;

    const bool is_last_instruction = (
        shadow_size == 0      // after taken branch
     || shadow_size == ninsn  // after last instruction
     );

    if (!is_last_instruction) { // restart current bundle
        debug_ip = ip - sizeof(CBundle);
        debug_ri = shadow_size;
        LOG_DEBUG(LOG_EXCEPTION) << "debug1 ip=" << fmt::hex(debug_ip) << " ri=" << debug_ri;
        psr.Trap();
        ip -= sizeof(CBundle);  // stay in current bundle
        LOG_ERROR(LOG_EXCEPTION | LOG_PREDICATION) << "restart bundle: ip=" << fmt::hex(ip) << fmt::endl;
    } else { // don't restart current bundle
        if (shadow_size == 0) { // it is after taken branch, use iipa-stored info from ExecuteJump
            debug_ip = RoundToBundle(iipa) - sizeof(CBundle);
            debug_ri = iipa & 3;
        } else { // shadow size == ninsn (full bundle completed)
            debug_ip = ip - sizeof(CBundle);
            debug_ri = ninsn;
        }
        LOG_DEBUG(LOG_EXCEPTION) << "debug2 ip=" << fmt::hex(debug_ip) << " ri=" << debug_ri;
        LOG_ERROR(LOG_EXCEPTION | LOG_PREDICATION) << "last instruction, no rollback";
        psr.ClearShadow();
    }

    TriggerEvent(status);
}

/***************************************************************************
* This function extend register frame
***************************************************************************/
CStatus
Core::AllocateFrame(const unsigned regnum)
{
    if (unlikely(is_static_reg(regnum))) {
        LOG_ERROR(LOG_EXCEPTION) << "Invalid framesize allocate: " << GetDecoder().gpr_names[regnum];
        return CStatus(CStatus::illegal_instruction);
    }

    STATIC_ASSERT(max_stacked_framesize <= phys_stacked_regs);

    const unsigned sof = (regnum - base_rotate_regs) + 1;

    if (unlikely(sof > max_stacked_framesize)) {
        LOG_ERROR(LOG_EXCEPTION)
            << "Invalid register frame: " << sof
            << " maximum " << max_stacked_framesize;

        return CStatus(CStatus::illegal_instruction);
    }

    // if invalid+clean register group is less than created frame,
    // spill dirty registers to backing store
    while (!rsc.CanAllocateFrame(sof)) {
        uint64_t const address = rsp;
        unsigned const bod = rsc.get_bod();
        CGeneralRegister const * dirty = &gpr[base_rotate_regs + bod];
        unsigned const batch_granularity = rsc.GetBatchGranularity();
        LOG_DEBUG(LOG_EMULATOR)
            << "spill " << bod
            << " address=" << fmt::hex(address)
            << " regs:" << fmt::span<CGeneralRegister>(dirty, batch_granularity);

        CHECK_STATUS_ADDRESS(address,
            mmu_write_backing_store(address, batch_granularity * sizeof(gpr[0]), dirty));
        // commit changes
        rsp += batch_granularity * sizeof(gpr[0]);
        rsc.ShrinkDirtySection(batch_granularity);
        performance_monitor.report_event(PMC_REGISTER_SPILL, batch_granularity);
    }

    // setup new rsc
    rsc.AllocateFrame(sof);
    return CStatus(CStatus::continue_execution);
}

/***************************************************************************
* this function handle callx
***************************************************************************/
CStatus
Core::CallSubroutine(const unsigned preserve, CGeneralRegister & retreg, uint64_t address)
{
    unsigned const preservedFrameSize = preserve - base_rotate_regs;
    if (preservedFrameSize > rsc.GetSizeOfFrame()) {
        LOG_ERROR(LOG_EXCEPTION)
            << "Invalid preserved size: " << preservedFrameSize
            << " frame size - " << rsc.GetSizeOfFrame();
        return CStatus(CStatus::illegal_instruction);
    }

    unsigned const outsize = rsc.PreserveFrame(preservedFrameSize);

    // pack together return address & preserved frame size
    retreg = fpu::u128( (exception_state.GetEIP() << 3) | preservedFrameSize,
                        ( ip << 3) | outsize);

    LOG_TRACE(LOG_EMULATOR)
        << "call address=" << fmt::hex(address)
        << " retaddr=" << fmt::hex(ip)
        << " preservedFrameSize=" << preservedFrameSize
        << " outsize" << outsize;

    // setup new address
    return ExecuteJump(RoundToBundle(address));
}

/***************************************************************************
* This function handle subroutine returns and register stack pop
***************************************************************************/
CStatus
Core::ReturnFromSubroutine(const CGeneralRegister & ret, const unsigned disp)
{
    // unpack from link register the return address and the previous frame part sizes
    uint64_t const retaddr = RoundToBundle((int64_t)ret.low() >> 3);
    uint64_t const eretaddr = RoundToBundle((int64_t)ret.high() >> 3);
    unsigned const preservedFrameSize = static_cast<unsigned>(ret.high() & 0x7F);
    unsigned const outFrameSize = static_cast<unsigned>(ret.low() & 0x7F);

#if 0
    if (unlikely(rsc.GetSizeOfFrame() < framesize)) {
        LOG_ERROR(LOG_EXCEPTION)
            << "Invalid return frame size: " << framesize
            << " SizeOfFrame=" << rsc.GetSizeOfFrame();

        return CStatus::invalid_parameter;
    }
#endif

    LOG_DEBUG(LOG_EMULATOR)
        << "return: retaddr=" << fmt::hex(retaddr)
        << " preservedFrameSize=" << preservedFrameSize
        << " outFrameSize=" << outFrameSize
        << " addr=" << fmt::hex(retaddr);

    if (unlikely(preservedFrameSize + outFrameSize > max_stacked_framesize)) {
        LOG_ERROR(LOG_EXCEPTION)
            << "Invalid return frame size - too big full size: " << preservedFrameSize
            << '+' << outFrameSize
            << " maximum: " << max_stacked_framesize;

        return CStatus(CStatus::invalid_parameter);
    }

    CHECK_STATUS(AllocateFrame(outFrameSize));

    // if dirty register group is less than restored frame, fill them from backing store
    while (!rsc.CanRestoreFrame(preservedFrameSize)) {
        unsigned const batch_granularity = rsc.GetBatchGranularity();
        uint64_t const address = rsp - batch_granularity * sizeof(gpr[0]);
        unsigned const entry = rsc.GetFillEntry();
        CGeneralRegister * dirty = &gpr[base_rotate_regs + entry];
        CHECK_STATUS_ADDRESS(address,
            mmu_read_backing_store(address, batch_granularity * sizeof(gpr[0]), dirty));
        LOG_DEBUG(LOG_EMULATOR)
            << "rfill entry=" << entry
            << " address=" << fmt::hex(address)
            << " reg=" << fmt::span<CGeneralRegister>(dirty, batch_granularity);

        // commit changes
        rsp = address;
        rsc.EnhanceDirtySection(batch_granularity);
        performance_monitor.report_event(PMC_REGISTER_FILL, batch_granularity);
    }

    rsc.RestoreOnReturn(preservedFrameSize, outFrameSize);

    // no more exceptions
    // below is non-restartable part
    // rollback stack pointer

    XW(reg_sp).base() -= disp;

    exception_state.SetEIP(eretaddr);
    return ExecuteJump( RoundToBundle(retaddr) );
}


/***************************************************************************
* this function read from special purpose register
***************************************************************************/
CStatus
Core::GetSpecialRegister(unsigned int regnum, CGeneralRegister & out)
{
    if (unlikely(arch_spr_regtype[regnum] == ESpecialRegisterSubset::reserved)) {
        LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register in mfspr: " << regnum;
        return CStatus(CStatus::illegal_instruction);
    }

    if (unlikely(!psr.IsKernelMode() &&
                 arch_spr_regtype[regnum] != ESpecialRegisterSubset::user &&
                 arch_spr_regtype[regnum] != ESpecialRegisterSubset::ruser)) {
        LOG_ERROR(LOG_EXCEPTION) << "privileged register in mfspr: " << regnum;
        return CStatus(CStatus::general_protection);
    }

    switch (regnum) {
        case reg_ip:       out = ip;                       break;
        case reg_eip:      out = exception_state.GetEIP(); break;
        case reg_fpcr:     out = fpu.get_control_word();   break;
        case reg_rsc:      out = rsc.get();                break;
        case reg_rsp:      out = rsp;                      break;

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
        case reg_iib:      out = iib.v128();               break;
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

        case reg_irr0:     out = pic.irr[0];               break;
        case reg_irr1:     out = pic.irr[1];               break;
        case reg_irr2:     out = pic.irr[2];               break;
        case reg_irr3:     out = pic.irr[3];               break;

        case reg_isr0:     out = pic.isr[0];               break;
        case reg_isr1:     out = pic.isr[1];               break;
        case reg_isr2:     out = pic.isr[2];               break;
        case reg_isr3:     out = pic.isr[3];               break;

        case reg_iv:       out = pic.read_vector();        break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register number " << regnum << " in mfspr";
            return CStatus(CStatus::illegal_instruction);
    }

    return CStatus(CStatus::continue_execution);
}


/***************************************************************************
* this function write to special purpose register
***************************************************************************/
CStatus
Core::SetSpecialRegister(unsigned int regnum, CGeneralRegister new_val)
{
    if (unlikely(arch_spr_regtype[regnum] == ESpecialRegisterSubset::reserved)) {
        LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register in mtspr: " << regnum;
        return CStatus(CStatus::illegal_instruction);
    }

    if (unlikely(!psr.IsKernelMode() && arch_spr_regtype[regnum] != ESpecialRegisterSubset::user)) {
        LOG_ERROR(LOG_EXCEPTION) << "privileged register in mtspr: " << regnum;
        return CStatus(CStatus::general_protection);
    }

    switch (regnum) {
        case reg_ip:
            return CStatus(CStatus::illegal_instruction);

        case reg_eip:      exception_state.SetEIP(new_val); break;

        case reg_fpcr:     fpu.set_control_word(new_val);      break;

        case reg_eca:      exception_state.SetContext(new_val); break;

        case reg_kip:      kip = new_val;    break;
        case reg_krsp:     krsp = new_val;   break;
        case reg_ksp:      ksp = new_val;    break;

        case reg_peb:      peb = new_val;    break;
        case reg_teb:      teb = new_val;    break;
        case reg_itc:      itc = new_val;    break;
        case reg_itm:      itm = new_val;    break;

        case reg_rsc:
            if (unlikely((new_val.base() & util::makemask(bits_per_register)) != new_val.base())) {
               LOG_ERROR(LOG_EXCEPTION) << "Invalid RSC write value: " << new_val;
               return CStatus(CStatus::invalid_parameter);
            }
            rsc.set(new_val);
            break;

        case reg_rsp:
            rsp = new_val & ~util::makemask(log_bytes_per_address);
            break;

        case reg_pta:
            return mmu.SetConfig(new_val);
            break;

        case reg_psr:
            if (unlikely((new_val & CProcessorStatusRegister::FLAGS_ALL) != new_val)) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid psr write value: " << new_val;
                return CStatus(CStatus::invalid_parameter);
            }
            psr.set(new_val);
            break;

        /*******************************************************************
        * interruptions handler
        *******************************************************************/

        case reg_iva:
            if (unlikely((new_val & ~util::makemask(bits_per_iva_alignment)) != new_val)) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid iva write value: " << new_val;
                return CStatus(CStatus::invalid_parameter);
            }
            iva = new_val;
            break;

        case reg_ifa:
            ifa = new_val;
            break;

        case reg_iib:
            LOG_ERROR(LOG_EXCEPTION) << "invalid iib write";
            return CStatus(CStatus::invalid_parameter);
            break;

        case reg_iip:
            iip = RoundToBundle(new_val);
            break;

        case reg_iipa:
            iipa = RoundToBundle(new_val);
            break;

        case reg_ipsr:
            ipsr.set(new_val);
            break;

        /*******************************************************************
        * external interrupts controller
        *******************************************************************/

        case reg_lid:
            if (unlikely((new_val & util::makemask(bits_per_processor_id)) != new_val)) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid pid write value: " << new_val;
                return CStatus(CStatus::invalid_parameter);
            }
            pic.lid = static_cast<unsigned>(new_val);
            break;

        case reg_tpr:
            if (unlikely((new_val & util::makemask(bits_per_int_vector)) != new_val)) {
                LOG_ERROR(LOG_EXCEPTION) << "invalid tpr write value: " << new_val;
                return CStatus(CStatus::invalid_parameter);
            }
            pic.tpr = static_cast<uint8_t>(new_val);
            break;

        case reg_itcv:     pic.itcv.Set(new_val);      break;
        case reg_tsv:      pic.tsv.Set(new_val);       break;
        case reg_pmv:      pic.pmv.Set(new_val);       break;
        case reg_cmcv:     pic.cmcv.Set(new_val);      break;

        case reg_irr0:
        case reg_irr1:
        case reg_irr2:
        case reg_irr3:
            LOG_ERROR(LOG_EXCEPTION) << "write to irr special registers";
            return CStatus(CStatus::illegal_instruction);

        case reg_isr0:
        case reg_isr1:
        case reg_isr2:
        case reg_isr3:
            LOG_ERROR(LOG_EXCEPTION) << "write to isr special registers";
            return CStatus(CStatus::illegal_instruction);

        case reg_iv:
            pic.EndOfInterrupt(new_val);
            break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "reserved special purpose register number in mtspr: " << regnum;
            return CStatus(CStatus::illegal_instruction);
    }

    return CStatus(CStatus::continue_execution);
}

CStatus
Core::OutputSpecialRegister(const DumpFormatter& out, const unsigned regnum) const
{
    switch (regnum) {
        case reg_ip:       out << fmt::hex(ip);               break;
        case reg_rsp:      out << fmt::hex(rsp);              break;

        case reg_eip:      out << fmt::hex(exception_state.GetEIP()); break;
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
        case reg_iib:      out << iib;                        break;
        case reg_iip:      out << fmt::hex(iip);              break;
        case reg_iipa:     out << fmt::hex(iipa);             break;
        case reg_fpcr:     out << fpu;                        break;
        case reg_rsc:      out << rsc;                        break;
        case reg_psr:      out << psr;                        break;
        case reg_ipsr:     out << ipsr;                       break;
        case reg_cause:    out << cause;                      break;
        case reg_pta:      out << mmu;                        break;

        case reg_tpr:
            out << fmt::hex<uint64_t>(pic.tpr) << " vector=" << (pic.tpr & util::makemask(bits_per_int_vector));
            break;

        case reg_itcv:    out << pic.itcv;      break;
        case reg_tsv:     out << pic.tsv;       break;
        case reg_pmv:     out << pic.pmv;       break;
        case reg_cmcv:    out << pic.cmcv;      break;

        case reg_lid:     out << fmt::hex<uint64_t>(pic.lid);    break;

        case reg_irr0:    out << fmt::hex(pic.irr[0]);    break;
        case reg_irr1:    out << fmt::hex(pic.irr[1]);    break;
        case reg_irr2:    out << fmt::hex(pic.irr[2]);    break;
        case reg_irr3:    out << fmt::hex(pic.irr[3]);    break;

        case reg_isr0:    out << fmt::hex(pic.isr[0]);    break;
        case reg_isr1:    out << fmt::hex(pic.isr[1]);    break;
        case reg_isr2:    out << fmt::hex(pic.isr[2]);    break;
        case reg_isr3:    out << fmt::hex(pic.isr[3]);    break;

        case reg_iv:      out << fmt::hex(uint64_t(0)); break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "reserved special register number " << regnum;
            return CStatus(CStatus::illegal_instruction);
    }

    return CStatus(CStatus::continue_execution);
}

/***************************************************************************
* this function print processor state
***************************************************************************/
void
Core::dump_state(const DumpFormatter& out) const
{
    out << "special registers:\n";
    for (unsigned i = 0; i < arch_spr_regtype.size(); i++) {
        if (arch_spr_regtype[i] != ESpecialRegisterSubset::reserved) {
            out << std::left
                << std::setfill(' ')
                << std::setw(8)
                << GetDecoder().spr_names[i];
            (void)OutputSpecialRegister(out, i);
            out << fmt::endl;
        }
    }

    const unsigned mask = 3;
    const int width = 5;

    out << "\ngeneral registers:\n";
    for (unsigned i = base_rotate_regs; i < base_rotate_regs + max_stacked_framesize; i++) {
        const unsigned n = rsc.rotate(i);

        out << std::right << std::setfill(' ') << std::setw(width) << GetDecoder().gpr_names[i]
            << std::left << " " << gpr[n];

        if ((i & mask) == mask)
            out << fmt::endl;
    }

    for (unsigned i = base_rotate_regs + max_stacked_framesize; i < base_rotate_regs + phys_stacked_regs; i++) {
        const unsigned n = rsc.rotate(i);

        out << std::right << std::setfill(' ') << std::setw(width) << "---"
            << std::left << " " << gpr[n];

        if ((i & mask) == mask)
            out << fmt::endl;
    }

    for (unsigned i = base_static_regs; i < base_static_regs + num_static_regs; i++) {
        const unsigned n = rsc.RegStaticToPhys(i);

        out << std::right << std::setfill(' ') << std::setw(width) << GetDecoder().gpr_names[i]
            << std::left << " " << gpr[n];

        if ((i & mask) == mask)
            out << fmt::endl;
    }

    cpuid.dump(out);
}

void
Core::dump_mapping(const PhysMemoryMapping& UNUSED(mapping), const DumpFormatter& UNUSED(out)) const
{
}

/***************************************************************************
*
***************************************************************************/
template<class CMD> class CInsnHandler {
public:
    using PHandlerFunction = CStatus (*)(Core& code, const CMD cmd);

    // handler initializer (with undef implementation for undefined opcodes)
    using PInitedHandlerFunction = util::default_initializer<PHandlerFunction, &Core::insn_undef<CMD>>;

    // handlers arrays
    using opcode_table_type  = std::array<PInitedHandlerFunction, 1 << bits_per_opcode>;
    using misc_table_type    = std::array<PInitedHandlerFunction, 1 << bits_per_misc_opx>;
    using fused_table_type   = std::array<PInitedHandlerFunction, 1 << bits_per_fused_opx>;
    using raopx_table_type   = std::array<PInitedHandlerFunction, 1 << bits_per_register>;
    using nullify_table_type = std::array<PInitedHandlerFunction, 1 << bits_per_nullify_opx>;

    static const  opcode_table_type   opcode_table;
    static const    misc_table_type     misc_table;
    static const   fused_table_type    fused_table;
    static const   raopx_table_type    raopx_table;
    static const nullify_table_type  nullify_table;
};

#define DELEGATE(CMD) DELEGATE_##CMD

#define DELEGATE_X1(name) &Core::insn_##name<CInsn1>,
#define DELEGATE_X2(name) &Core::insn_##name<CInsn2>,

template<>
const CInsnHandler<CInsn1>::opcode_table_type
CInsnHandler<CInsn1>::opcode_table =
{
    DECLARE_PRIMARY_OPCODES(DELEGATE_X1)
};

template<>
const CInsnHandler<CInsn1>::fused_table_type
CInsnHandler<CInsn1>::fused_table =
{
    DECLARE_FUSED_EXTENDED_OPCODES(DELEGATE_X1)
};

template<>
const CInsnHandler<CInsn1>::misc_table_type
CInsnHandler<CInsn1>::misc_table =
{
    DECLARE_MISC_EXTENDED_OPCODES(DELEGATE_X1)
};

template<>
const CInsnHandler<CInsn1>::raopx_table_type
CInsnHandler<CInsn1>::raopx_table =
{
    DECLARE_RAOPX_EXTENDED_OPCODES(DELEGATE_X1)
};

template<>
const CInsnHandler<CInsn1>::nullify_table_type
CInsnHandler<CInsn1>::nullify_table =
{
    DECLARE_NULLIFY_EXTENDED_OPCODES(DELEGATE_X1)
};

template<>
const CInsnHandler<CInsn2>::raopx_table_type
CInsnHandler<CInsn2>::raopx_table =
{
    DECLARE_RAOPX_EXTENDED_OPCODES(DELEGATE_X2)
};

template<>
const CInsnHandler<CInsn2>::opcode_table_type
CInsnHandler<CInsn2>::opcode_table =
{
    DECLARE_PRIMARY_OPCODES(DELEGATE_X2)
};

template<>
const CInsnHandler<CInsn2>::nullify_table_type
CInsnHandler<CInsn2>::nullify_table =
{
    DECLARE_NULLIFY_EXTENDED_OPCODES(DELEGATE_X2)
};

/***************************************************************************
*
***************************************************************************/
DEFINE_INSN_IMPLEMENTATION(misc)
{
    const unsigned opx = cmd.GetMiscOpx();
    LOG_TRACE(LOG_EMULATOR) << "opx=" << opx;

    //both single/double 'misc' instructions are identical
    //extension is ignored
    return CInsnHandler<CInsn1>::misc_table[opx](*this, cmd);
}

DEFINE_INSN_IMPLEMENTATION(fused)
{
    const unsigned opx = cmd.GetFusedOpx();
    LOG_TRACE(LOG_EMULATOR) << "opx=" << opx;

    //both single/double instructions are identical
    //extension is ignored
    return CInsnHandler<CInsn1>::fused_table[opx](*this, cmd);
}

DEFINE_INSN_IMPLEMENTATION(raopx)
{
    const unsigned opx = cmd.GetRaOpx();
    LOG_TRACE(LOG_EMULATOR) << "opx=" << opx;

    return CInsnHandler<CMD>::raopx_table[opx](*this, cmd);
}

DEFINE_INSN_IMPLEMENTATION(nullify)
{
    const unsigned opx = cmd.GetNullifyOpx();
    LOG_TRACE(LOG_EMULATOR) << "opx=" << opx;

    return CInsnHandler<CMD>::nullify_table[opx](*this, cmd);
}

/***************************************************************************
* execute functions
***************************************************************************/
template<class CMD>
ALWAYS_INLINE
CStatus
Core::execute_instruction(const CMD cmd)
{
    if (unlikely(psr.IsShadowed(ipsr.GetShadowSize()))) {
        return FinalizeInstruction();
    }
    LOG_TRACE(LOG_EMULATOR)
        << "cmd: " << cmd << '\n'
        << CInstructionDisassembler<CMD>(GetDecoder(), cmd, ip);

    performance_monitor.count(cmd);
    bool const is_nullified = psr.PopFuture();

    CStatus status;
    if (unlikely(is_nullified)) {
        LOG_TRACE(LOG_PREDICATION) << "nullified " << cmd;
        performance_monitor.report_event(PMC_QUALIFIED_NOP_INSTRUCTION);
        // don't check debugger on nullified instructions
        status = FinalizeInstruction();
    } else {
        // restore read-only zero registers
        //...
        unsigned const opcode = cmd.GetOpcode();
        LOG_TRACE(LOG_EMULATOR) << "opcode=" << opcode;
        //single/double instructions are different
        status = CInsnHandler<CMD>::opcode_table[opcode](*this, cmd);
        LOG_TRACE(LOG_EMULATOR) << "status=" << status << '\n' << '\n';
    }

    if (unlikely(pic.IsInterruptRequest())) {
        LOG_TRACE(LOG_EMULATOR) << "core=" << pic.lid
            << " ip=" << fmt::hex(ip)
            << " psr=" << psr
            << " cmd=" << cmd
            << ": " << CInstructionDisassembler<CMD>(GetDecoder(), cmd, ip)
            << fmt::endl;

        unsigned const vector = pic.read_vector();
        LOG_DEBUG(LOG_EMULATOR) << "vector=" << vector;
        status = CStatus(vector); //trap
    }

    if (unlikely(!status.success()) && status != CStatus(CStatus::halt)) {
        LOG_FATAL(LOG_EMULATOR) << "exception " << status
            << fmt::endl
            << " core=" << pic.lid
            << " ip=" << fmt::hex(ip)
            << " ifa=" << fmt::hex(ifa)
            << fmt::endl
            << " cmd=" << cmd
            << ": " << CInstructionDisassembler<CMD>(GetDecoder(), cmd, ip)
            << fmt::endl
            << " psr=" << psr
            ;
    }

    return status;
}

/***************************************************************************
* this function execute one bundle
***************************************************************************/
ALWAYS_INLINE
CStatus
Core::execute_bundle(const CBundle bundle)
{
    psr.ClearShadow();

    if (unlikely(config_flags & CONFIG_PROFILE)) {
        profile[ip]++;
    }

    ++itc;
    iipa = ip;
    ip += sizeof(CBundle);

    CStatus status;
    EBundleTemplate const bundle_template = bundle.GetTemplate();

    switch (bundle_template) {
        case EBundleTemplate::bundle_sss:
            LOG_TRACE(LOG_EMULATOR) << "sss: " << bundle;
            status = execute_instruction(bundle.s0());
            if (likely(status.continuable())) {
               status = execute_instruction(bundle.s1());
               if (likely(status.continuable())) {
                   status = execute_instruction(bundle.s2());
               }
            }
            break;

        case EBundleTemplate::bundle_sll:
            LOG_TRACE(LOG_EMULATOR) << "sll: " << bundle;
            status = execute_instruction(bundle.s0());
            if (likely(status.continuable())) {
                status = execute_instruction(bundle.l1());
            }
            break;

        case EBundleTemplate::bundle_lls:
            LOG_TRACE(LOG_EMULATOR) << "lls: " << bundle;
            status = execute_instruction(bundle.l0());
            if (likely(status.continuable())) {
                status = execute_instruction(bundle.s2());
            }
            break;

        default:
            LOG_ERROR(LOG_EXCEPTION) << "invalid bundle template " << static_cast<unsigned>(bundle_template);
            status = CStatus(CStatus::illegal_instruction);
    }

    if (likely(status.success())) {
        // bundle is finalized without interruptions,
        // so clear the shadow from possible previous interrupt
        ipsr.ClearShadow();
    } else {
        iib = bundle;
        if (status.IsFault()) {
            LOG_TRACE(LOG_LOADER) << "fault: iv=" << status << fmt::endl;
            RaiseFault(status);
        } else if (status.IsTrap()) {
            LOG_TRACE(LOG_LOADER) << "trap: iv=" << status << fmt::endl;
            RaiseTrap(status, bundle);
        }
    }

    LOG_TRACE(LOG_EMULATOR) << "fin bundle\n";
    return status;
}

void
Core::SetIp(const uint64_t start_ip)
{
    ip = start_ip;
    iip = start_ip;
    iipa = start_ip;
    ipsr = psr;

    // debug
    debug_ip = ip;
    debug_ri = 1;
}

CStatus
Core::execute_until_interruption(void)
{
    const std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    ip = iip;
    psr = ipsr;
    LOG_TRACE(LOG_LOADER) << "running: core=" << pic.lid
                          << " ip=" << fmt::hex(ip)
                          << " psr=" << psr << fmt::endl;
    CStatus status(CStatus::continue_execution);
    while (status.success()) {
        const uint64_t cacheMask = ICACHE_LINESIZE - 1;
        const uint64_t fetch_ip = ip & ~cacheMask;
        LOG_TRACE(LOG_EMULATOR) << "fetch ip=" << fmt::hex(fetch_ip);
        InstructionCacheLine<ICACHE_LINESIZE> bundles;
        status = mmu_fetch_code(fetch_ip, bundles);
        if (status.continuable()) {
            while (status.success() && ((ip & ~cacheMask) == fetch_ip)) {
                LOG_TRACE(LOG_EMULATOR) << "ip=" << fmt::hex(ip);
                status = execute_bundle(bundles.GetBundle(ip));
            }
        }
    }
    m_Stopped = status.halted();

    const std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    const auto diff = end - start;
    const uint64_t run_time = std::chrono::duration <uint64_t, std::nano> (diff).count();
    performance_monitor.report_event(PMC_RUNTIME, run_time);
    return status;
}

void
Core::DumpProfile(const DumpFormatter& out) const
{
    out << "\nbundle usage statistic:\n";
    uint64_t prev_address = 0;
    for (const auto & x : profile) {
        out << fmt::hex(x.first) << ':' << x.second << fmt::endl;
        if (prev_address != 0 && x.first != prev_address) {
            out << fmt::hex(prev_address) << "..." << fmt::hex(x.first) << " - gap\n";
        }
        prev_address = x.first + sizeof(CBundle);
    }
}

void
Core::dump_statistic(const DumpFormatter& out) const
{
    out << "core id=" << pic.lid << fmt::endl;
    Device::dump_statistic(out);
    config.dump(out);
    dump_state(out);

    performance_monitor.dump_statistic(itc, config_flags & CONFIG_TIMING, out);
    if (config_flags & CONFIG_PROFILE) {
        DumpProfile(out);
    }
}

CStatus
Core::PrepareStack(uint64_t vbase, size_t stack_size)
{
    LOG_DEBUG(LOG_EMULATOR) << "Core: "
          << fmt::hex(reinterpret_cast<size_t>(this))
          << " " << sizeof(*this) << " bytes";

    stack_size = util::roundup(stack_size, bits_per_segment_offset);
    CHECK_STATUS(VirtualAllocate(vbase, stack_size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));

    // standard stack grows downward
    const uint64_t stack_hi = vbase + stack_size;

    XW(reg_sp) = stack_hi;
    XW(reg_fp) = stack_hi;

    /*
    LOG_TRACE(LOG_LOADER | LOG_EMULATOR)
        << "thread:" << fmt::endl
        << "  stackbase  = " << fmt::hex(vbase) << fmt::endl
        << "  sp         = " << fmt::hex(XR(reg_sp).base()) << fmt::endl
        << "  fp         = " << fmt::hex(XR(reg_fp).base()) << fmt::endl;
*/
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::PrepareBackingStore(uint64_t vbase, size_t rstack_size)
{
    // backing store stack grows upward
    CHECK_STATUS(VirtualAllocate(vbase, rstack_size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL));

    rsp = vbase;
    LOG_TRACE(LOG_LOADER | LOG_EMULATOR) << "  rsp        = " << fmt::hex(rsp) << fmt::endl;
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::PrepareThreadStorage(uint64_t vbase, size_t size)
{
    // thread env block
    XW(reg_tp) = vbase;
    CHECK_STATUS(VirtualAllocate(vbase, util::roundup(size, bits_per_segment_offset), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));

    LOG_TRACE(LOG_LOADER | LOG_EMULATOR) << "  tp         = " << fmt::hex(XR(reg_tp).base()) << fmt::endl;
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::disassemble_bundle(std::ostream & out, int64_t offset, disassembler_mode mode)
{
    uint64_t const address = debug_ip + offset;

    LOG_TRACE(LOG_EMULATOR) << "address=" << fmt::hex(address);

    InstructionCacheLine<ICACHE_LINESIZE> bundles;
    CStatus const status = mmu_fetch_code(address & ~(ICACHE_LINESIZE-1), bundles);
    if (!status.success()) {
        out << fmt::hex(address) << " error" << fmt::endl;
        return status;
    }

    CBundle const& bundle = bundles.GetBundle(address);

    // we assume that this function is called from debug trap
    // check if it's highlighted bundle and pass shadow size
    // otherwise pass 0(invalid) for non-highlighted
    unsigned const ri = (offset == 0) ? debug_ri : 0;
    GetDecoder().Disassemble(bundle, address + sizeof(bundle), out, mode, ri);
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::dump_virt_map_sub_dir(
    const DumpFormatter& out,
    const phys_address_t pta,
    uint64_t virt_address,
    uint64_t & used,
    uint64_t & extra_used,
    int level) const
{
    uint64_t const pagesize = mmu.GetPageSize();
    extra_used += pagesize; // root page

    uint64_t const virt_offset = uint64_t(pagesize) << (level * mmu.GetBitsPerPageIndex());

    CTranslationEntry const root_pta(pta);
    out << "pta level=" << level << ": ";
    root_pta.dump(out);
    out << fmt::endl;

    std::vector<uint8_t> rootPageData(pagesize);
    CHECK_STATUS(get_target_system()->read(root_pta.GetAddress(), pagesize, rootPageData.data()));

    for (size_t offset = 0; offset < pagesize; offset += sizeof(uint64_t), virt_address += virt_offset) {
         const uint64_t pta_next = *reinterpret_cast<uint64_t*>(&rootPageData[offset]);
         const CTranslationEntry cpta( pta_next );

         if (!cpta.is_valid()) {
             LOG_ERROR(LOG_PAGETABLE)
                 << fmt::hex(virt_address)
                 << ": reserved ";
                 // << cpta.dump(out, flags);
             continue;
         }

         if (level > 0) {
             CHECK_STATUS(dump_virt_map_sub_dir(out, cpta.GetAddress(), virt_address, used, extra_used, level-1));
         } else {
             used += pagesize;
             uint64_t const canonical_address = mmu.compute_canonical_address(virt_address);
             out.dump_virtual_address(canonical_address);
             out << ": ";
             cpta.dump(out);
             out << fmt::endl;
         }
    }

    return CStatus(CStatus::continue_execution);
}

CStatus
Core::dump_virt_map(const DumpFormatter& out) const
{
    out.chapter("virtual memory map");
    out << "page_offset " << mmu.GetPageOffsetSize() << " bits, "
        << (UINT64_C(1) << mmu.GetPageOffsetSize()) << " bytes per page" << fmt::endl
        << "page_index " << mmu.GetBitsPerPageIndex() << " bits" << fmt::endl
        << "numberOfPagingLevels=" << mmu.GetNumberOfPagingLevels() << fmt::endl
        << "bits_per_virtual_address=" << mmu.GetVirtualAddressBits() << fmt::endl;

    out << "pta=" << fmt::hex(mmu.GetPageTableRootAddress()) << fmt::endl;

    if (mmu.is_real_mode()) {
        out << "translation not enabled" << fmt::endl;
        return CStatus(CStatus::continue_execution);
    }

    uint64_t used = 0;
    uint64_t extra_used = 0;

    uint64_t const user_space_begin_address = 0;
    CStatus status = dump_virt_map_sub_dir(out,
        mmu.GetPageTableRootAddress(),
        user_space_begin_address, used, extra_used,
        mmu.GetNumberOfPagingLevels() - 1);

    out << "mapped virtual memory: " << used << " bytes\n";
    out << "used physical pagetable memory: " << extra_used << " bytes\n";
    return status;
}

CStatus
Core::InsertPage(uint64_t virt_address, phys_address_t entry_phys_address, uint64_t attributes)
{
    phys_address_t curpta = mmu.GetPageTableRootAddress();
    LOG_TRACE(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address)
                             << " root_pta=" << fmt::hex(curpta)
                             << " mask=" << fmt::hex(VM_PAGE_PHYS_ADDRESS_MASK);

    // uint64_t const pagesize = GetPageSize();

    phys_address_t next_pta;
    for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
        const uint64_t next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
        CHECK_STATUS(get_target_system()->read(next_te_pa, sizeof(next_pta), &next_pta));

        if (!CTranslationEntry(next_pta).is_valid()) {
            const uint64_t page = get_target_system()->alloc_page();
            LOG_TRACE(LOG_PAGETABLE)
                << "memory page=" << fmt::hex(page);
            next_pta = page | VM_PAGE_FLAG_VALID;
            CHECK_STATUS(get_target_system()->write(next_te_pa, sizeof(next_pta), &next_pta));
        }

        curpta = next_pta & VM_PAGE_PHYS_ADDRESS_MASK;
        LOG_TRACE(LOG_PAGETABLE) << "curpta=" << fmt::hex(curpta);
    }

    // last level (0)
    const uint64_t next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
    CHECK_STATUS(get_target_system()->read(next_te_pa, sizeof(next_pta), &next_pta));

    if (CTranslationEntry(next_pta).is_valid()) {
        LOG_FATAL(LOG_PAGETABLE)
            << "replaced page entry at address " << fmt::hex(next_te_pa)
            << " is valid: " << fmt::hex(next_pta);
    }
                                                                         
    next_pta = (entry_phys_address & ~VM_PAGE_FLAGS_ALL) | attributes | VM_PAGE_FLAG_VALID;
    CHECK_STATUS(get_target_system()->write(next_te_pa, sizeof(next_pta), &next_pta));

    return CStatus(CStatus::continue_execution);
}

CStatus
Core::VirtualAllocate(uint64_t base, uint64_t size, uint64_t attributes)
{
    uint64_t const pagesize = mmu.GetPageSize();

    base = util::align_down<uint64_t> (base, pagesize);
    size = util::align_up<uint64_t> (size, pagesize);

    LOG_TRACE(LOG_PAGETABLE) << "mmu: " << mmu;

    for (uint64_t virt_address = base; virt_address < base + size; virt_address += pagesize) {
        const uint64_t page = get_target_system()->alloc_page();
        LOG_TRACE(LOG_PAGETABLE) << "phys_page=" << fmt::hex(page);
        CHECK_STATUS(InsertPage(virt_address, page, attributes));
    }
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::VirtualMap(uint64_t base, phys_address_t phys_address, uint64_t size, uint64_t attributes)
{
    uint64_t const pagesize = mmu.GetPageSize();

    base = util::align_down<uint64_t> (base, pagesize);
    size = util::align_up<uint64_t> (size, pagesize);

    LOG_TRACE(LOG_PAGETABLE) << "mmu: " << mmu;
    for (uint64_t virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_TRACE(LOG_PAGETABLE) << "mmio page=" << fmt::hex(phys_address);
        CHECK_STATUS(InsertPage(virt_address, phys_address, attributes));
        phys_address += pagesize;
    }
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::VirtualFree(uint64_t base, uint64_t size)
{
    uint64_t const pagesize = mmu.GetPageSize();

    base = util::align_down<uint64_t> (base, pagesize);
    size = util::align_up<uint64_t> (size, pagesize);

    for (uint64_t virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_TRACE(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address);
        uint64_t curpta = mmu.GetPageTableRootAddress();
        for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
            uint64_t const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
            uint64_t next_pta;
            CHECK_STATUS(get_target_system()->read(next_te_pa, sizeof(uint64_t), &next_pta));
            if (!CTranslationEntry(next_pta).is_valid()) {
                return CStatus(CStatus::data_page_not_present);
            }
            curpta = next_pta & VM_PAGE_PHYS_ADDRESS_MASK;
        }

        // last level
        uint64_t const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
        uint64_t next_pta;
        CHECK_STATUS(get_target_system()->read(next_te_pa, sizeof(uint64_t), &next_pta));
        CHECK_STATUS(get_target_system()->free_page(next_pta & VM_PAGE_PHYS_ADDRESS_MASK, pagesize));
        next_pta = 0;
        CHECK_STATUS(get_target_system()->write(next_te_pa, sizeof(uint64_t), &next_pta));
        FlushTranslationBuffer(virt_address);
    }
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::VirtualProtect(uint64_t base, uint64_t size, uint64_t attributes)
{
    uint64_t const pagesize = mmu.GetPageSize();

    base = util::align_down<uint64_t> (base, pagesize);
    size = util::align_up<uint64_t> (size, pagesize);

    for (uint64_t virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_TRACE(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address);
        uint64_t curpta = mmu.GetPageTableRootAddress();
        for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
            uint64_t const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
            uint64_t next_pta;
            CHECK_STATUS(get_target_system()->read(next_te_pa, sizeof(uint64_t), &next_pta));

            if (!CTranslationEntry(next_pta).is_valid()) {
                LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
                    << "invalid page in protect" << fmt::hex(next_pta);
                return CStatus(CStatus::data_page_not_present);
            }

            curpta = next_pta & VM_PAGE_PHYS_ADDRESS_MASK;
        }

        // last level
        uint64_t const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
        uint64_t next_pta;
        CHECK_STATUS(get_target_system()->read(next_te_pa, sizeof(uint64_t), &next_pta));

        next_pta &= ~VM_PAGE_RIGHT_MASK;
        next_pta |= attributes & VM_PAGE_RIGHT_MASK;
        CHECK_STATUS(get_target_system()->write(next_te_pa, sizeof(uint64_t), &next_pta));
        FlushTranslationBuffer(virt_address);
    }

    //FlushTranslationBuffer();
    return CStatus(CStatus::continue_execution);
}

class CMemoryPageWriter : public IMemoryOperation {
public:
    CMemoryPageWriter(void const * _data, size_t _len, size_t _pagesize)
        : data(_data)
        , len(_len)
        , pagesize(_pagesize) {}
    ~CMemoryPageWriter() override {}
    size_t size(void) const override { return pagesize; }
    CStatus complex_operation(void * address) override
    {
        if (len > 0) {
            memcpy(address, data, len);
        }
        if (len < pagesize) {
            memset(reinterpret_cast<uint8_t*>(address) + len, 0, pagesize - len);
        }
        return CStatus(CStatus::continue_execution);
    }

private:
    void const *   data;
    size_t         len;
    size_t         pagesize;
};

CStatus
Core::LoadSegment(uint64_t base, size_t file_size, const uint8_t *data, uint64_t final_rights)
{
    if (file_size == 0) {
        return CStatus(CStatus::continue_execution);
    }

    uint64_t const pagesize = mmu.GetPageSize();

    assert(file_size % bytes_per_sector == 0);
    assert(base % pagesize == 0);
    assert(pagesize % bytes_per_sector == 0);

    const size_t virt_size = util::roundup(file_size, mmu.GetPageOffsetSize());

    CHECK_STATUS(VirtualAllocate(base, virt_size, VM_PAGE_RIGHT_WRITE));

    for (size_t i = 0; i < file_size; i += pagesize) {
        const size_t len = std::min(file_size - i, pagesize);
        CMemoryPageWriter op(&data[i], len, pagesize);
        CHECK_STATUS(mmu_complex_operation(base + i, VM_PAGE_RIGHT_WRITE, op));
    }

    CHECK_STATUS(VirtualProtect(base, virt_size, final_rights));

    LOG_DEBUG(LOG_LOADER) << "segment set up: base=" << fmt::hex(base)
                          << " size=" << virt_size;
    return CStatus(CStatus::continue_execution);
}

CStatus
Core::LoadModule(const uint64_t vbase,
    std::istream & input, CExeFileHeader & header)
{
    header.read(input);

    const size_t textsze    = header.size_text;
    const size_t stacksze   = header.size_stack;
    const size_t rstacksize = header.size_stack;
    const size_t datasze    = header.size_data;
    const size_t heapsze    = header.size_heap;
    const size_t rodatasze  = header.size_rodata;

    LOG_TRACE(LOG_LOADER)
         << "image: text " << header.size_text
         << " rodata "    << header.size_rodata
         << " data "      << header.size_data
         << " stack "     << stacksze
         << " rstack "    << rstacksize
         << " heap "      << heapsze << " bytes\n";

    // read input file
    const size_t filelen = textsze + rodatasze + datasze;
    std::vector<uint8_t> file_src(filelen);
    // load program into low part of virtual address space
    if (!input.read(reinterpret_cast<char *>(&file_src[0]), filelen)) {
        LOG_ABORT << "unable read from file";
    }

    size_t virt_offset = 0;
    size_t file_offset = 0;
    CHECK_STATUS(LoadSegment(vbase + virt_offset, textsze, &file_src[file_offset],
                 VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL));

    virt_offset += util::roundup(textsze, bits_per_segment_offset);
    file_offset += textsze;
    CHECK_STATUS(LoadSegment(vbase + virt_offset, rodatasze, &file_src[file_offset],
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_FINAL));

    virt_offset += util::roundup(rodatasze, bits_per_segment_offset);
    file_offset += rodatasze;
    CHECK_STATUS(LoadSegment(vbase + virt_offset, datasze, &file_src[file_offset],
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_FINAL));

    LOG_TRACE(LOG_LOADER)
        <<   "  base       = " << fmt::hex(vbase)
        << "\n  entry      = " << fmt::hex(vbase + header.entry_point)
        << "\n  rodata     = " << fmt::hex(vbase + util::roundup(textsze, bits_per_segment_offset))
        << "\n  data       = " << fmt::hex(vbase + util::roundup(textsze, bits_per_segment_offset)
                                                 + util::roundup(rodatasze, bits_per_segment_offset))
        << '\n';

    return CStatus(CStatus::continue_execution);
}

} // namespace postrisc
