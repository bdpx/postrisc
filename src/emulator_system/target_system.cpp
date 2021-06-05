#include <fstream>
#include <thread>      // for std::this_thread
#include <chrono>      // for std::chrono::steady_clock/duration/etc
#include <boost/range/adaptor/reversed.hpp>

#include "util/common.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "binary_format/file_header.hpp"
#include "config/emulator_configuration.hpp"
#include "emulator/interruption.hpp"
#include "emulator/core.hpp"
#include "emulator/debugger.hpp"
#include "emulator/dump/formatter_html.hpp"
#include "emulator/dump/formatter_txt.hpp"
#include "devices/memory.hpp"
#include "devices/rom.hpp"
#include "devices/video.hpp"
#include "devices/array.hpp"
#include "target_system.hpp"

#include <boost/smart_ptr/make_intrusive.hpp>

#define EMULATOR_OUT(out) out << "EMULATOR> "

#if 0
// supported archive types
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

// for STL serialization
#include <boost/serialization/string.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/set.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/unique_ptr.hpp>
#include "boost/serialization/intrusive_ptr.hpp"

#include <boost/serialization/serialization.hpp>
#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(postrisc::Core)
BOOST_CLASS_EXPORT(postrisc::device_memory)
BOOST_CLASS_EXPORT(postrisc::device_rom)
BOOST_CLASS_EXPORT(postrisc::device_video)
BOOST_CLASS_EXPORT(postrisc::DeviceArray)
BOOST_CLASS_EXPORT(postrisc::TargetSystem)
#endif

namespace postrisc {

#if 0
void
Serialize(TargetSystem& ts, const std::string& path)
{
    std::ofstream ofs;
    ofs.open(path, std::ios_base::out | std::ios_base::binary);
    if (!ofs) {
        std::cerr << "Serialization open failed" << fmt::endl;
        return;
    }

    boost::archive::binary_oarchive oa(ofs);
    oa << BOOST_SERIALIZATION_NVP(ts);
}

void
Deserialize(TargetSystem& ts, const std::string& path)
{
    std::ifstream ifs;
    ifs.open(path, std::ios_base::in | std::ios_base::binary);
    if (!ifs) {
        std::cerr << "Deserialization open failed" << fmt::endl;
        return;
    }

    boost::archive::binary_iarchive ia(ifs);
    ia >> BOOST_SERIALIZATION_NVP(ts);
}
#endif

TargetSystem::TargetSystem(Decoder& decoder)
    : IManagedAddressSpace<TargetSystem>(12, decoder)
    , m_CoreNumber(0)
    , m_configuration_flags(0)
    , m_bits_per_physical_address(40)
    , m_NumAllocatedPages(0)
{
}

TargetSystem::~TargetSystem(void)
{
}

void
TargetSystem::dump_mapping(const DumpFormatter& out) const
{
    out.chapter("physical memory dump");

    out.start_section();
    out << m_bits_per_physical_address << " implemented physical address bits" << fmt::endl;
    out.finish_section();

    u64 size = 0;
    for (auto const& mapping : boost::adaptors::reverse(m_phys_memory_map)) {
        u64 const address = mapping.first;
        out.start_section();
        out << "mapping: ";
        out.dump_physical_address(address);
        out << "...";
        out.dump_physical_address(address + mapping.second.get_size() - 1);
        size += mapping.second.get_size();

        out << " size: " << fmt::hex(mapping.second.get_size())
            << " (" << fmt::hex(mapping.second.m_device->size()) << ")"
            << " device_name: " << mapping.second.m_device->get_name()
            << fmt::endl;

        if (!mapping.second.m_page_bitmap.empty()) {
            size_t i = 0;
            u64 page_address = address;
            for (bool const x : mapping.second.m_page_bitmap) {
                if ((i & 63) == 0) {
                    out << "  ";
                    out.dump_physical_address_ref(page_address);
                    out << " page_bitmap: ";
                }
                out << (x ? '1' : '0');
                if ((i & 63) == 63) {
                    out << fmt::endl;
                }
                i++;
                page_address += get_page_size();
            }
            out << fmt::endl;
        }
        out.finish_section();

        mapping.second.m_device->dump_mapping(mapping.second, out);
        out << fmt::endl;
    }

    out.start_section();
    out << "mapped physical memory " << size << " bytes" << fmt::endl;
    out.finish_section();
}

void
TargetSystem::dump_statistic(const DumpFormatter& out) const
{
    m_DeviceArray->dump_statistic(out);
}

CStatus
TargetSystem::add_memory_mapping(const CPhysAddress pa, PhysMemoryMapping& mapping, bool is_memory)
{
    u64 const address = pa.get_address();
    if ((address & PHYS_ADDRESS_MASK) != address) {
        LOG_ERROR(LOG_PLATFORM) << "unimplemented physical address bits: " << fmt::hex(address);
        return CStatus::data_unimplemented_address;
    }
    if ((address & util::makemask(16)) != 0) {
        LOG_ERROR(LOG_PLATFORM) << "unaligned device physical address bits: " << fmt::hex(address);
        return CStatus::data_unaligned_address;
    }
    u64 const end = address + mapping.get_size();
    if (end < address) {
        LOG_ERROR(LOG_PLATFORM) << "device address wraparound: "
                                << fmt::hex(address) << " - " << fmt::hex(end);
        return CStatus::platform_management;
    }

    unsigned const page_size = get_page_size();

    for (const auto& x : m_phys_memory_map) {
        const u64 dev_begin = x.first;
        const u64 dev_end = dev_begin + x.second.get_size();
        const bool intersection =
               ( dev_begin >= address && dev_begin <  end )
            || ( dev_end   >  address && dev_end   <= end )
            || ( address >= dev_begin && address <  dev_end )
            || ( end     >  dev_begin && end     <= dev_end );

        if (intersection) {
            LOG_ERROR(LOG_PLATFORM) << "ranges are overlapped: "
                                    << fmt::hex(address)
                                    << " and " << fmt::hex(dev_begin);
            return CStatus::platform_management;
        }
    }

    if (is_memory) {
        u64 const allocatable_size = util::rounddown(mapping.get_size(), page_size);
        size_t const num_pages = static_cast<size_t>(allocatable_size / page_size);
        mapping.m_page_bitmap.assign(num_pages, false);
    }
    m_phys_memory_map.insert(std::make_pair(address, mapping));
    return CStatus::continue_execution;
}

u64
TargetSystem::size(void) const
{
    u64 size = 0;
    for (const auto& x : m_phys_memory_map) {
        size += x.second.get_size();
    }
    return size;
}

Core *
TargetSystem::get_bootstrap_core(void) const
{
    return m_DeviceArray->get_bootstrap_core();
}

void TargetSystem::FlushTranslationBuffer(void)
{
    return m_DeviceArray->FlushTranslationBuffer();
}

void TargetSystem::FlushTranslationBuffer(u64 address)
{
    return m_DeviceArray->FlushTranslationBuffer(address);
}

void
TargetSystem::activate_execution(void)
{
    return m_DeviceArray->activate_execution(*this);
}

CStatus
TargetSystem::broadcast_interrupt(CStatus ivec)
{
    return m_DeviceArray->broadcast_interrupt(ivec);
}

void TargetSystem::Clear()
{
    m_phys_memory_map.clear();
    m_CoreNumber = 0;
    m_Tasks.clear();
}

CStatus
TargetSystem::Setup(const EmulatorConfiguration& conf)
{
    Clear();
    m_configuration_flags = conf.get_config_flags();
    m_bits_per_physical_address = conf.bits_per_physical_address;

    unsigned const bytes_per_page = U32C(1) << conf.bitsPerPageOffset;
    set_bits_per_page_offset(conf.bitsPerPageOffset);

    // prepare special device: config space for all other devices
    LOG_DEBUG(LOG_LOADER) << "create DeviceArray";
    m_DeviceArray = boost::make_intrusive<DeviceArray>("DeviceArray", conf.device_array_config.max_config_space_size);
    // DeviceArray controller will be automatically the first device in config space
    // don't add it to itself
    // order of other devices may be varied

    // add memory devices/mappings
    // we cam't map to virtual space before adding memory devices
    if (conf.memory_devices.empty()) {
        EMULATOR_OUT(std::cerr) << "no memory devices" << std::endl;
        return CStatus::platform_management;
    }

    unsigned id = 0;
    for (const auto& device_info : conf.memory_devices) {
        size_t const size = device_info.size;
        CPhysAddress const address = device_info.address;
        const std::string name = std::string("memory") + std::to_string(id++);
        boost::intrusive_ptr<device_memory> memory = boost::make_intrusive<device_memory>(name, size);
        CHECK_STATUS(m_DeviceArray->add_device(memory));
        PhysMemoryMapping mapping(address, bytes_per_page, memory);
        CHECK_STATUS(add_memory_mapping(address, mapping, true/*is_memory*/));
    }

    // after memory creation we may alloc page table root page
    LOG_DEBUG(LOG_LOADER) << "allocate root page";
    CPhysAddress const pta = alloc_page();
    LOG_DEBUG(LOG_EMULATOR | LOG_LOADER) << "pta=" << pta;

    // recursive hook for page table itself
    //*reinterpret_cast<u64 *>(phys_memory->GetPointer(pta)) =
    //    pta | VM_PAGE_FLAG_VALID | VM_PAGE_FLAG_KERNEL | VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE;

    LOG_DEBUG(LOG_LOADER) << "create processor cores";
    u32 core_id = 0;
    for (const auto& core_info : conf.core_devices) {
        boost::intrusive_ptr<Core> core = boost::make_intrusive<Core>(core_info.name, core_id, m_configuration_flags);
        core->reset(core_id);
        core->FlushTranslationBuffer();
        core->get_mmu().SetPagingParameters(pta, conf.bitsPerPageOffset, conf.numberOfPagingLevels);
        // processor cores are only added to config space, but not memory-mapped
        CHECK_STATUS(m_DeviceArray->add_device(core));
        ++core_id;
    }

    // get first added core
    Core *core = m_DeviceArray->get_bootstrap_core();

    {
        LOG_DEBUG(LOG_LOADER) << "setup video";
        m_video = boost::make_intrusive<device_video>("video", conf.video_device.width, conf.video_device.height);
        CPhysAddress const pa = conf.video_device.phys_address;
        CVirtAddress const va = conf.video_device.virt_address;
        PhysMemoryMapping video_mapping(pa, bytes_per_page, m_video);
        CHECK_STATUS(add_memory_mapping(pa, video_mapping, false));
        CHECK_STATUS(VirtualMap(core->GetMMU(), va, pa, m_video->size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));
        CHECK_STATUS(m_DeviceArray->add_device(m_video));
    }

    for (const auto& device_info : conf.rom_devices) {
        LOG_DEBUG(LOG_LOADER) << "setup ROM " << device_info.data.size() << " bytes";
        //rom_data.resize(bytes_per_page, 0xff);
        boost::intrusive_ptr<Device> rom = boost::make_intrusive<device_rom>("rom", device_info.data);
        CHECK_STATUS(m_DeviceArray->add_device(rom));
        CPhysAddress const pa = device_info.phys_address;
        PhysMemoryMapping mapping(pa, bytes_per_page, rom);
        CHECK_STATUS(add_memory_mapping(pa, mapping, false));
        // additionally map ROM to virtual memory
        CHECK_STATUS(VirtualMap(core->GetMMU(), device_info.virt_address, pa, rom->size(), VM_PAGE_RIGHT_READ));
    }

    // map device array last, after all changes of its size
    {
        CPhysAddress const pa = conf.device_array_config.phys_address;
        CVirtAddress const va = conf.device_array_config.virt_address;
        m_DeviceArray->config.device_array_address = pa.get_address();
        m_DeviceArray->config.device_array_len = m_DeviceArray->size();
        PhysMemoryMapping mapping(pa, bytes_per_page, m_DeviceArray);
        CHECK_STATUS(add_memory_mapping(pa, mapping, false));
        CHECK_STATUS(VirtualMap(core->GetMMU(), va, pa, m_DeviceArray->size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));
    }

    LOG_DEBUG(LOG_LOADER) << "targetSystem setup completed";
    return CStatus::continue_execution;
}

void
TargetSystem::JoinAllCores()
{
    for (auto& task : m_Tasks) {
        CStatus status = task.get();
        UNREFERENCED_PARAMETER(status);
    }

    m_Tasks.clear();
}

CStatus
TargetSystem::activate_execution_handler(Core *core)
{
    CStatus status = core->execute_until_interruption(*this);

    --m_CoreNumber;
    return status;
}

void
TargetSystem::activate_execution(Core *core)
{
    if (core->IsStopped()) return;

    ++m_CoreNumber;
    m_Tasks.emplace_back(std::async(std::launch::async,
        &TargetSystem::activate_execution_handler, this, core));
}

void
TargetSystem::Interactive(std::ostream& out)
{
    Debugger::setup();
    bool is_debugged = false;
    while (m_CoreNumber.load() > 0) {
        if (!_kbhit()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }
        const int c = _getch();
        if (c == 'd' && !is_debugged) {
            CStatus status = broadcast_interrupt(CStatus::single_step_trap);
            UNREFERENCED_PARAMETER(status);
            JoinAllCores();
            LOG_DEBUG(LOG_EMULATOR) << "debugger start";
            Core *core = get_bootstrap_core();
            Debugger(*this, core).execute();
            LOG_DEBUG(LOG_EMULATOR) << "debugger stop";
            activate_execution();
        }
    }
    JoinAllCores();
    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "program complete!" << std::endl;
        DumpFormatterTxt formatter(get_decoder(), out);
        dump_statistic(formatter);
        EMULATOR_OUT(out) << "finish" << std::endl;
    }
}

CStatus
TargetSystem::StartEmulation(const EmulatorConfiguration& configuration, std::istream& input, std::ostream& out)
{
    u64 const vbase = configuration.base_address;

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "host: " << (bits_per_byte * sizeof(void*)) << " bit\n";
        EMULATOR_OUT(out) << "vbase = " << fmt::hex(vbase) << fmt::endl;
        EMULATOR_OUT(out) << "bitsPerPageOffset = " << configuration.bitsPerPageOffset << fmt::endl;
        EMULATOR_OUT(out) << "numberOfPagingLevels = " << configuration.numberOfPagingLevels << fmt::endl;
    }

    // setup core and virtual memory
    CHECK_STATUS(Setup(configuration));

    Core *core = get_bootstrap_core();
    const TranslationUnit &mmu = core->GetMMU();

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "virtual address space depth (in bits) = " << mmu.GetVirtualAddressBits() << fmt::endl;
        EMULATOR_OUT(out) << "highest user address = " << fmt::hex(mmu.GetHighestUserAddress()) << fmt::endl;
        EMULATOR_OUT(out) << "lowest kernel address = " << fmt::hex(mmu.GetLowestKernelAddress()) << fmt::endl;
    }

    CExeFileHeader header;
    CHECK_STATUS(LoadModule(*core, vbase, input, header));
    CHECK_STATUS(PrepareThread(*core, vbase, header));

    u64 const start_ip = vbase + header.entry_point;
    core->SetIp(start_ip);

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "start" << fmt::endl;
    }

    return CStatus::continue_execution;
}

CStatus
TargetSystem::PrepareThread(Core& core, const u64 module_base, const CExeFileHeader &header)
{
    u64 const vbase = module_base + std::max<u64>(U64C(0x400000), header.size_text);
    u64 const eip = module_base + header.size_text;
    u64 const sp = vbase;
    u64 stack_size = get_page_size();
    stack_size = util::roundup(stack_size, bits_per_segment_offset);
    CHECK_STATUS(VirtualAllocate(core.GetMMU(), sp, stack_size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));

    u64 const bp = vbase + U64C(0x100000);
    // backing store stack grows upward
    CHECK_STATUS(VirtualAllocate(core.GetMMU(), bp, get_page_size(),
         VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL));

    u64 const tp = vbase + U64C(0x200000);
    u64 tp_size = util::roundup(get_page_size(), bits_per_segment_offset);
    CHECK_STATUS(VirtualAllocate(core.GetMMU(), tp, tp_size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));

    // standard stack grows downward
    u64 const stack_hi = sp + stack_size;
    CHECK_STATUS(core.PrepareThread(sp, stack_hi, bp, tp, eip));

    return CStatus::continue_execution;
}

void TargetSystem::dump(const TranslationUnit &mmu, std::ostream& out)
{
    DumpFormatterHtml formatter(get_decoder(), out);
    formatter.start_dump();
    dump_statistic(formatter);
    dump_mapping(formatter);
    (void)dump_virt_map(mmu, formatter);
    formatter.finish_dump();
}

void
TargetSystem::WaitFinishEmulation(std::ostream& out)
{
    JoinAllCores();
    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "program complete!" << std::endl;
        DumpFormatterTxt formatter(get_decoder(), out);
        dump_statistic(formatter);
    }
}

CStatus
TargetSystem::mmu_fetch_code(Core &core, const CVirtAddress va, size_t len, void *data)
{
    LOG_DEBUG(LOG_ICACHE)
        << "fetch: " << va;

    CHECK_ALIGNMENT(CInstructionHelper, va, sizeof(CBundle));

    auto [status, phys_address] =
        TranslateAddress<CInstructionHelper>(core, va,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE);

    CHECK_STATUS(status);

    return core.fetch_code(*this, phys_address, len, data);
}

CStatus
TargetSystem::free_page(CPhysAddress pa, size_t pagesize)
{
    // don't offset to device-internal address,
    // we need raw mapping offset for bitmap
    FIND_DEVICE(pa, pagesize)
    size_t const pagenum = device_offset.get_offset() / get_page_size();
    mapping.m_page_bitmap[pagenum] = false;
    --m_NumAllocatedPages;
    return CStatus::continue_execution;
}

CPhysAddress TargetSystem::alloc_page(void)
{
    // find free page from lesser addresses
    for (auto& mapping : boost::adaptors::reverse(m_phys_memory_map)) {
        if (mapping.second.m_page_bitmap.empty()) continue;
        for (size_t pagenum = 0; pagenum < mapping.second.m_page_bitmap.size(); ++pagenum) {
            if (mapping.second.m_page_bitmap[pagenum]) continue;
            mapping.second.m_page_bitmap[pagenum] = true;
            ++m_NumAllocatedPages;
            CPhysAddress pa(mapping.first + get_page_size() * pagenum);
            LOG_DEBUG(LOG_PLATFORM) << "page allocated: pagenum=" << pagenum
                                    << " address=" << pa;
            // memset(&m_Data[address], 0, get_page_size());
            return pa;
        }
    }

    LOG_ERROR(LOG_PLATFORM | LOG_EXCEPTION)
        << "can't allocate page: NumAllocatedPages=" << m_NumAllocatedPages
        << " size=" << (m_NumAllocatedPages * get_page_size());
    return CPhysAddress(INVALID_PHYS_ADDRESS);
}

CStatus
TargetSystem::InsertPage(const TranslationUnit &mmu,
    u64 virt_address, const CPhysAddress pa, u64 attributes)
{
    CPhysAddress curpta = mmu.GetPageTableRootAddress();
    LOG_DEBUG(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address)
                             << " root_pta=" << curpta
                             << " mask=" << fmt::hex(VM_PAGE_PHYS_ADDRESS_MASK);

    u64 entry_phys_address = pa.get_address();
    // u64 const pagesize = GetPageSize();

    u64 next_pta;
    for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
        const CPhysAddress next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
        CHECK_STATUS(bus_read(next_te_pa, sizeof(next_pta), &next_pta));

        if (!CTranslationEntry(next_pta).is_valid()) {
            CPhysAddress const page = alloc_page();
            LOG_DEBUG(LOG_PAGETABLE) << "page=" << page;
            next_pta = page.get_address() | VM_PAGE_FLAG_VALID;
            CHECK_STATUS(bus_write(CPhysAddress(next_te_pa), sizeof(next_pta), &next_pta));
        }

        curpta = CPhysAddress(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        LOG_DEBUG(LOG_PAGETABLE) << "curpta=" << curpta;
    }

    // last level (0)
    const CPhysAddress next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
    CHECK_STATUS(bus_read(next_te_pa, sizeof(next_pta), &next_pta));

    if (CTranslationEntry(next_pta).is_valid()) {
        LOG_ERROR(LOG_PAGETABLE)
            << "replaced page entry at address " << next_te_pa
            << " is valid: " << fmt::hex(next_pta);
        return CStatus::general_protection;
    }

    next_pta = (entry_phys_address & ~VM_PAGE_FLAGS_ALL) | attributes | VM_PAGE_FLAG_VALID;
    CHECK_STATUS(bus_write(CPhysAddress(next_te_pa), sizeof(next_pta), &next_pta));

    return CStatus::continue_execution;
}

CStatus
TargetSystem::VirtualAllocate(const TranslationUnit &mmu,
    u64 base, u64 size, u64 attributes)
{
    u64 const pagesize = mmu.GetPageSize();

    base = util::align_down<u64> (base, pagesize);
    size = util::align_up<u64> (size, pagesize);

    LOG_DEBUG(LOG_PAGETABLE) << "mmu: " << mmu;

    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        CPhysAddress const page = alloc_page();
        LOG_DEBUG(LOG_PAGETABLE) << "phys_page=" << page;
        CHECK_STATUS(InsertPage(mmu, virt_address, page, attributes));
    }
    return CStatus::continue_execution;
}

CStatus
TargetSystem::VirtualMap(const TranslationUnit &mmu,
    const CVirtAddress va, const CPhysAddress pa, u64 size, u64 attributes)
{
    u64 const pagesize = mmu.GetPageSize();
    u64 paddr = pa.get_address();

    u64 const base = util::align_down<u64>(va.get_address(), pagesize);
    size = util::align_up<u64>(size, pagesize);

    LOG_DEBUG(LOG_PAGETABLE) << "mmu: " << mmu;
    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_DEBUG(LOG_PAGETABLE) << "mmio page=" << fmt::hex(paddr);
        CHECK_STATUS(InsertPage(mmu, virt_address, CPhysAddress(paddr), attributes));
        paddr += pagesize;
    }
    return CStatus::continue_execution;
}

CStatus
TargetSystem::VirtualProtect(const TranslationUnit &mmu, u64 base, u64 size, u64 attributes)
{
    u64 const pagesize = mmu.GetPageSize();

    base = util::align_down<u64> (base, pagesize);
    size = util::align_up<u64> (size, pagesize);

    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_DEBUG(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address);
        CPhysAddress curpta = mmu.GetPageTableRootAddress();
        for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
            CPhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
            u64 next_pta;
            CHECK_STATUS(bus_read(next_te_pa, sizeof(u64), &next_pta));

            if (!CTranslationEntry(next_pta).is_valid()) {
                LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
                    << "invalid page in protect" << fmt::hex(next_pta);
                return CStatus::data_page_not_present;
            }

            curpta = CPhysAddress(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        }

        // last level
        CPhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
        u64 next_pta;
        CHECK_STATUS(bus_read(next_te_pa, sizeof(u64), &next_pta));

        next_pta &= ~VM_PAGE_RIGHT_MASK;
        next_pta |= attributes & VM_PAGE_RIGHT_MASK;
        CHECK_STATUS(bus_write(CPhysAddress(next_te_pa), sizeof(u64), &next_pta));
        FlushTranslationBuffer(virt_address);
    }

    return CStatus::continue_execution;
}

CStatus
TargetSystem::VirtualFree(const TranslationUnit &mmu, u64 base, u64 size)
{
    u64 const pagesize = mmu.GetPageSize();

    base = util::align_down<u64> (base, pagesize);
    size = util::align_up<u64> (size, pagesize);

    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_DEBUG(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address);
        CPhysAddress curpta = mmu.GetPageTableRootAddress();
        for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
            CPhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
            u64 next_pta;
            CHECK_STATUS(bus_read(next_te_pa, sizeof(u64), &next_pta));
            if (!CTranslationEntry(next_pta).is_valid()) {
                return CStatus::data_page_not_present;
            }
            curpta = CPhysAddress(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        }

        // last level
        CPhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
        u64 next_pta;
        CHECK_STATUS(bus_read(next_te_pa, sizeof(u64), &next_pta));
        CPhysAddress page(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        CHECK_STATUS(free_page(page, pagesize));
        next_pta = 0;
        CHECK_STATUS(bus_write(next_te_pa, sizeof(u64), &next_pta));
        FlushTranslationBuffer(virt_address);
    }

    return CStatus::continue_execution;
}

class CMemoryPageWriter : public IMemoryOperation {
public:
    CMemoryPageWriter(void const *_data, size_t _len, size_t _pagesize)
        : data(_data)
        , len(_len)
        , pagesize(_pagesize) {}
    ~CMemoryPageWriter() override {}
    CStatus complex_operation(void *address) override {
        if (len > 0) {
            memcpy(address, data, len);
        }
        if (len < pagesize) {
            memset(reinterpret_cast<u8*>(address) + len, 0, pagesize - len);
        }
        return CStatus::continue_execution;
    }

private:
    void const *data;
    size_t      len;
    size_t      pagesize;
};

CStatus
TargetSystem::LoadSegment(Core& core, u64 base, size_t file_size,
                          const u8 *data, u64 final_rights)
{
    if (file_size == 0) {
        return CStatus::continue_execution;
    }

    u64 const pagesize = core.GetMMU().GetPageSize();

    assert(file_size % bytes_per_sector == 0);
    assert(base % pagesize == 0);
    assert(pagesize % bytes_per_sector == 0);

    const size_t virt_size = util::roundup(file_size, core.GetMMU().GetPageOffsetSize());

    CHECK_STATUS(VirtualAllocate(core.GetMMU(), base, virt_size, VM_PAGE_RIGHT_WRITE));

    for (size_t i = 0; i < file_size; i += pagesize) {
        const size_t len = std::min(file_size - i, pagesize);
        CMemoryPageWriter op(&data[i], len, pagesize);
        CHECK_STATUS(mmu_complex_operation(core, CVirtAddress(base + i), pagesize,
             VM_PAGE_RIGHT_WRITE,
             VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
             op));
    }

    CHECK_STATUS(VirtualProtect(core.GetMMU(), base, virt_size, final_rights));

    LOG_DEBUG(LOG_LOADER) << "segment set up: base=" << fmt::hex(base)
                          << " size=" << virt_size;
    return CStatus::continue_execution;
}

CStatus
TargetSystem::LoadModule(Core& core, const u64 vbase, std::istream& input, CExeFileHeader& header)
{
    header.read(input);

    const size_t textsze    = header.size_text;
    const size_t stacksze   = header.size_stack;
    const size_t rstacksize = header.size_stack;
    const size_t datasze    = header.size_data;
    const size_t heapsze    = header.size_heap;
    const size_t rodatasze  = header.size_rodata;

    LOG_DEBUG(LOG_LOADER)
         << "image: text " << header.size_text
         << " rodata "    << header.size_rodata
         << " data "      << header.size_data
         << " stack "     << stacksze
         << " rstack "    << rstacksize
         << " heap "      << heapsze << " bytes\n";

    // read input file
    const size_t filelen = textsze + rodatasze + datasze;
    std::vector<u8> file_src(filelen);
    // load program into low part of virtual address space
    if (!input.read(reinterpret_cast<char *>(&file_src[0]), filelen)) {
        LOG_ABORT << "unable read from file";
    }

    size_t virt_offset = 0;
    size_t file_offset = 0;
    CHECK_STATUS(LoadSegment(core, vbase + virt_offset, textsze, &file_src[file_offset],
                 VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL));

    virt_offset += util::roundup(textsze, bits_per_segment_offset);
    file_offset += textsze;
    CHECK_STATUS(LoadSegment(core, vbase + virt_offset, rodatasze, &file_src[file_offset],
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_FINAL));

    virt_offset += util::roundup(rodatasze, bits_per_segment_offset);
    file_offset += rodatasze;
    CHECK_STATUS(LoadSegment(core, vbase + virt_offset, datasze, &file_src[file_offset],
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_FINAL));

    LOG_DEBUG(LOG_LOADER)
        <<   "  base       = " << fmt::hex(vbase)
        << "\n  entry      = " << fmt::hex(vbase + header.entry_point)
        << "\n  rodata     = " << fmt::hex(vbase + util::roundup(textsze, bits_per_segment_offset))
        << "\n  data       = " << fmt::hex(vbase + util::roundup(textsze, bits_per_segment_offset)
                                                 + util::roundup(rodatasze, bits_per_segment_offset))
        << '\n';

    return CStatus::continue_execution;
}

CStatus
TargetSystem::dump_virt_map_sub_dir(
    const TranslationUnit &mmu, const DumpFormatter &out,
    const CPhysAddress pta, u64 virt_address,
    u64 &used, u64 &extra_used, int level)
{
    u64 const pagesize = mmu.GetPageSize();
    extra_used += pagesize; // root page

    u64 const virt_offset = u64(pagesize) << (level * mmu.GetBitsPerPageIndex());

    CTranslationEntry const root_pta(pta);
    out << "pta level=" << level << ": ";
    root_pta.dump(out);
    out << fmt::endl;

    std::vector<u8> rootPageData(pagesize);
    CHECK_STATUS(bus_read(root_pta.GetAddress(), pagesize, rootPageData.data()));

    for (size_t offset = 0; offset < pagesize; offset += sizeof(u64), virt_address += virt_offset) {
         const u64 pta_next = *reinterpret_cast<u64*>(&rootPageData[offset]);
         const CTranslationEntry cpta( pta_next );

         if (!cpta.is_valid()) {
             LOG_DEBUG(LOG_PAGETABLE)
                 << " reserved page: "
                 << fmt::hex(virt_address);
                 // << cpta.dump(out, flags);
             continue;
         }

         if (level > 0) {
             CHECK_STATUS(dump_virt_map_sub_dir(mmu, out, cpta.GetAddress(),
                                                virt_address, used, extra_used, level-1));
         } else {
             used += pagesize;
             u64 const canonical_address = mmu.compute_canonical_address(virt_address);
             out.dump_virtual_address(canonical_address);
             out << ": ";
             cpta.dump(out);
             out << fmt::endl;
         }
    }

    return CStatus::continue_execution;
}

CStatus
TargetSystem::dump_virt_map(const TranslationUnit &mmu, const DumpFormatter& out)
{
    out.chapter("virtual memory map");
    out.start_section();
    out << "page_offset " << mmu.GetPageOffsetSize() << " bits, "
        << (U64C(1) << mmu.GetPageOffsetSize()) << " bytes per page" << fmt::endl
        << "page_index " << mmu.GetBitsPerPageIndex() << " bits" << fmt::endl
        << "numberOfPagingLevels=" << mmu.GetNumberOfPagingLevels() << fmt::endl
        << "bits_per_virtual_address=" << mmu.GetVirtualAddressBits() << fmt::endl;

    out << "pta=" << mmu.GetPageTableRootAddress() << fmt::endl;

    if (mmu.is_real_mode()) {
        out << "translation not enabled" << fmt::endl;
        out.finish_section();
        return CStatus::continue_execution;
    }

    u64 used = 0;
    u64 extra_used = 0;

    u64 const user_space_begin_address = 0;
    CStatus status = dump_virt_map_sub_dir(mmu, out,
        mmu.GetPageTableRootAddress(),
        user_space_begin_address, used, extra_used,
        mmu.GetNumberOfPagingLevels() - 1);

    out << "mapped virtual memory: " << used << " bytes\n";
    out << "used physical pagetable memory: " << extra_used << " bytes\n";
    out.finish_section();
    return status;
}

} // namespace postrisc
