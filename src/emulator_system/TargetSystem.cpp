#include <fstream>
#include <thread>      // for std::this_thread
#include <chrono>      // for std::chrono::steady_clock/duration/etc
#include <boost/range/adaptor/reversed.hpp>

#include "util/common.hpp"
#include "arch/isa.hpp"
#include "binary_format/file_header.hpp"
#include "config/configuration.hpp"
#include "emulator/Result.hpp"
#include "emulator/Core.hpp"
#include "emulator/Debugger.hpp"
#include "DeviceMemory.hpp"
#include "DeviceRom.hpp"
#include "DeviceVideo.hpp"
#include "DeviceArray.hpp"
#include "TargetSystem.hpp"

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
BOOST_CLASS_EXPORT(postrisc::DeviceMemory)
BOOST_CLASS_EXPORT(postrisc::DeviceRom)
BOOST_CLASS_EXPORT(postrisc::DeviceVideo)
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
    : ManagedAddressSpace<TargetSystem>(12, decoder)
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

Result
TargetSystem::add_memory_mapping(const PhysAddress pa, PhysMemoryMapping& mapping, bool is_memory)
{
    u64 const address = pa.get_address();
    if ((address & PHYS_ADDRESS_MASK) != address) {
        LOG_ERROR(LOG_PLATFORM) << "unimplemented physical address bits: " << fmt::hex(address);
        return Result::data_unimplemented_address;
    }
    if ((address & util::makemask<u64>(16)) != 0) {
        LOG_ERROR(LOG_PLATFORM) << "unaligned device physical address bits: " << fmt::hex(address);
        return Result::data_unaligned_address;
    }
    u64 const end = address + mapping.get_size();
    if (end < address) {
        LOG_ERROR(LOG_PLATFORM) << "device address wraparound: "
                                << fmt::hex(address) << " - " << fmt::hex(end);
        return Result::platform_management;
    }

    const size_t page_size = get_page_size();

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
            return Result::platform_management;
        }
    }

    if (is_memory) {
        u64 const allocatable_size = util::rounddown_pow2<u64>(mapping.get_size(), page_size);
        size_t const num_pages = static_cast<size_t>(allocatable_size / page_size);
        LOG_DEBUG(LOG_PLATFORM) << " mapping size: "  << mapping.get_size()
                                << " allocatable_size=" << fmt::hex(allocatable_size)
                                << " num_pages=" << num_pages;
        mapping.m_page_bitmap.assign(num_pages, false);
    }
    m_phys_memory_map.insert(std::make_pair(address, mapping));
    return Result::continue_execution;
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

Result
TargetSystem::broadcast_interrupt(Result ivec)
{
    return m_DeviceArray->broadcast_interrupt(ivec);
}

void TargetSystem::Clear()
{
    m_phys_memory_map.clear();
    m_CoreNumber = 0;
    m_Tasks.clear();
}

Result TargetSystem::Setup(const Configuration& conf)
{
    Clear();
    m_configuration_flags = conf.get_config_flags();
    m_bits_per_physical_address = conf.get_bits_per_physical_address();

    unsigned const bytes_per_page = U32C(1) << conf.get_bits_per_page_offset();
    set_bits_per_page_offset(conf.get_bits_per_page_offset());

    // prepare special device: config space for all other devices
    LOG_DEBUG(LOG_LOADER) << "create DeviceArray";
    m_DeviceArray = boost::make_intrusive<DeviceArray>("DeviceArray", conf.get_device_array_config().max_config_space_size);
    // DeviceArray controller will be automatically the first device in config space
    // don't add it to itself
    // order of other devices may be varied

    // add memory devices/mappings
    // we cam't map to virtual space before adding memory devices
    if (conf.get_memory_devices().empty()) {
        EMULATOR_OUT(std::cerr) << "no memory devices" << std::endl;
        return Result::platform_management;
    }

    unsigned id = 0;
    for (const auto& device_info : conf.get_memory_devices()) {
        size_t const size = device_info.size;
        PhysAddress const address = device_info.address;
        const std::string name = std::string("memory") + std::to_string(id++);
        boost::intrusive_ptr<DeviceMemory> memory = boost::make_intrusive<DeviceMemory>(name, size);
        CHECK_RESULT(m_DeviceArray->add_device(memory));
        PhysMemoryMapping mapping(address, bytes_per_page, memory);
        CHECK_RESULT(add_memory_mapping(address, mapping, true/*is_memory*/));
    }

    // after memory creation we may alloc page table root page
    LOG_DEBUG(LOG_LOADER) << "allocate root page";
    PhysAddress const pta = alloc_page();
    LOG_DEBUG(LOG_EMULATOR | LOG_LOADER) << "pta=" << pta;
    if (pta.get_address() == INVALID_PHYS_ADDRESS) {
        EMULATOR_OUT(std::cerr) << "can't allocate root page" << std::endl;
        return Result::platform_management;
    }

    // recursive hook for page table itself
    //*reinterpret_cast<u64 *>(phys_memory->GetPointer(pta)) =
    //    pta | VM_PAGE_FLAG_VALID | VM_PAGE_FLAG_KERNEL | VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE;

    LOG_DEBUG(LOG_LOADER) << "create processor cores";
    for (u32 i = 0; i < conf.get_ncores(); i++) {
        std::string corename = std::string("core:") + std::to_string(i);
        boost::intrusive_ptr<Core> core = boost::make_intrusive<Core>(corename, i, m_configuration_flags);
        core->reset(i);
        core->FlushTranslationBuffer();
        core->get_mmu().SetPagingParameters(pta, conf.get_bits_per_page_offset(), conf.get_paging_levels());
        // processor cores are only added to config space, but not memory-mapped
        CHECK_RESULT(m_DeviceArray->add_device(core));
    }

    // get first added core
    Core *core = m_DeviceArray->get_bootstrap_core();

    {
        const auto& video_device = conf.get_video_device();
        LOG_DEBUG(LOG_LOADER) << "setup video";
        m_video = boost::make_intrusive<DeviceVideo>("video", video_device.width, video_device.height);
        PhysAddress const pa = video_device.phys_address;
        VirtAddress const va = video_device.virt_address;
        PhysMemoryMapping video_mapping(pa, bytes_per_page, m_video);
        CHECK_RESULT(add_memory_mapping(pa, video_mapping, false));
        CHECK_RESULT(VirtualMap(core->GetMMU(), va, pa, m_video->size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));
        CHECK_RESULT(m_DeviceArray->add_device(m_video));
    }

    for (const auto& device_info : conf.get_rom_devices()) {
        LOG_DEBUG(LOG_LOADER) << "setup ROM " << device_info.data.size() << " bytes";
        //rom_data.resize(bytes_per_page, 0xff);
        boost::intrusive_ptr<Device> rom = boost::make_intrusive<DeviceRom>("rom", device_info.data);
        CHECK_RESULT(m_DeviceArray->add_device(rom));
        PhysAddress const pa = device_info.phys_address;
        PhysMemoryMapping mapping(pa, bytes_per_page, rom);
        CHECK_RESULT(add_memory_mapping(pa, mapping, false));
        // additionally map ROM to virtual memory
        CHECK_RESULT(VirtualMap(core->GetMMU(), device_info.virt_address, pa, rom->size(), VM_PAGE_RIGHT_READ));
    }

    // map device array last, after all changes of its size
    {
        const auto& device_array_config = conf.get_device_array_config();
        PhysAddress const pa = device_array_config.phys_address;
        VirtAddress const va = device_array_config.virt_address;
        m_DeviceArray->config.device_array_address = pa.get_address();
        m_DeviceArray->config.device_array_len = m_DeviceArray->size();
        PhysMemoryMapping mapping(pa, bytes_per_page, m_DeviceArray);
        CHECK_RESULT(add_memory_mapping(pa, mapping, false));
        CHECK_RESULT(VirtualMap(core->GetMMU(), va, pa, m_DeviceArray->size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));
    }

    LOG_DEBUG(LOG_LOADER) << "targetSystem setup completed";
    return Result::continue_execution;
}

void
TargetSystem::JoinAllCores()
{
    for (auto& task : m_Tasks) {
        Result result = task.get();
        UNREFERENCED_PARAMETER(result);
    }

    m_Tasks.clear();
}

Result
TargetSystem::activate_execution_handler(Core *core)
{
    Result result = core->execute_until_interruption(*this);

    --m_CoreNumber;
    return result;
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
            Result result = broadcast_interrupt(Result::single_step_trap);
            UNREFERENCED_PARAMETER(result);
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
        DumpFormatter formatter(get_decoder(), out, false);
        dump_statistic(formatter);
        EMULATOR_OUT(out) << "finish" << std::endl;
    }
}

Result
TargetSystem::StartEmulation(const Configuration& configuration, std::istream& input, std::ostream& out)
{
    u64 const vbase = configuration.get_base_address();

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "host: " << (bits_per_byte * sizeof(void*)) << " bit\n";
        EMULATOR_OUT(out) << "vbase = " << fmt::hex(vbase) << fmt::endl;
        EMULATOR_OUT(out) << "bitsPerPageOffset = " << configuration.get_bits_per_page_offset() << fmt::endl;
        EMULATOR_OUT(out) << "numberOfPagingLevels = " << configuration.get_paging_levels() << fmt::endl;
    }

    // setup core and virtual memory
    CHECK_RESULT(Setup(configuration));

    Core *core = get_bootstrap_core();
    const TranslationUnit &mmu = core->GetMMU();

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "virtual address space depth (in bits) = " << mmu.GetVirtualAddressBits() << fmt::endl;
        EMULATOR_OUT(out) << "highest user address = " << fmt::hex(mmu.GetHighestUserAddress()) << fmt::endl;
        EMULATOR_OUT(out) << "lowest kernel address = " << fmt::hex(mmu.GetLowestKernelAddress()) << fmt::endl;
    }

    ExeFileHeader header;
    CHECK_RESULT(LoadModule(*core, vbase, input, header));
    CHECK_RESULT(PrepareThread(*core, vbase, header));

    u64 const start_ip = vbase + header.entry_point;
    core->SetIp(start_ip);

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "start" << fmt::endl;
    }

    return Result::continue_execution;
}

Result
TargetSystem::PrepareThread(Core& core, const u64 module_base, const ExeFileHeader &header)
{
    u64 const vbase = module_base + std::max<u64>(U64C(0x400000), header.size_text);
    u64 const eip = module_base + header.size_text;
    u64 const sp = vbase;
    u64 stack_size = get_page_size();
    stack_size = util::roundup<u64>(stack_size, bits_per_segment_offset);
    CHECK_RESULT(VirtualAllocate(core.GetMMU(), sp, stack_size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));

    u64 const bp = vbase + U64C(0x100000);
    // backing store stack grows upward
    CHECK_RESULT(VirtualAllocate(core.GetMMU(), bp, get_page_size(),
         VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_BACKING_STORE | VM_PAGE_RIGHT_FINAL));

    u64 const tp = vbase + U64C(0x200000);
    u64 tp_size = util::roundup<u64>(get_page_size(), bits_per_segment_offset);
    CHECK_RESULT(VirtualAllocate(core.GetMMU(), tp, tp_size, VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));

    // standard stack grows downward
    u64 const stack_hi = sp + stack_size;
    CHECK_RESULT(core.PrepareThread(sp, stack_hi, bp, tp, eip));

    return Result::continue_execution;
}

void TargetSystem::dump(const TranslationUnit &mmu, std::ostream& out)
{
    DumpFormatter formatter(get_decoder(), out, true);
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
        DumpFormatter formatter(get_decoder(), out, false);
        dump_statistic(formatter);
    }
}

Result
TargetSystem::mmu_fetch_code(Core &core, const VirtAddress va, size_t len, void *data)
{
    LOG_DEBUG(LOG_ICACHE)
        << "fetch: " << va;

    CHECK_ALIGNMENT(InstructionHelper, va, sizeof(Bundle));

    auto [result, phys_address] =
        TranslateAddress<InstructionHelper>(core, va,
            VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
            VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_BACKING_STORE);

    CHECK_RESULT(result);

    return core.fetch_code(*this, phys_address, len, data);
}

Result
TargetSystem::free_page(PhysAddress pa, size_t pagesize)
{
    // don't offset to device-internal address,
    // we need raw mapping offset for bitmap
    FIND_DEVICE(pa, pagesize)
    size_t const pagenum = device_offset.get_offset() / get_page_size();
    mapping.m_page_bitmap[pagenum] = false;
    --m_NumAllocatedPages;
    return Result::continue_execution;
}

PhysAddress TargetSystem::alloc_page(void)
{
    // find free page from lesser addresses
    for (auto& mapping : boost::adaptors::reverse(m_phys_memory_map)) {
        if (mapping.second.m_page_bitmap.empty()) {
            LOG_DEBUG(LOG_PLATFORM | LOG_EXCEPTION) << "memory bitmap is empty, address=" << fmt::hex(mapping.first);
            continue;
        }
        for (size_t pagenum = 0; pagenum < mapping.second.m_page_bitmap.size(); ++pagenum) {
            if (mapping.second.m_page_bitmap[pagenum]) continue;
            mapping.second.m_page_bitmap[pagenum] = true;
            ++m_NumAllocatedPages;
            PhysAddress pa(mapping.first + get_page_size() * pagenum);
            LOG_DEBUG(LOG_PLATFORM) << "page allocated: pagenum=" << pagenum
                                    << " address=" << pa;
            // memset(&m_Data[address], 0, get_page_size());
            return pa;
        }
    }

    LOG_ERROR(LOG_PLATFORM | LOG_EXCEPTION)
        << "can't allocate page: memory devices: " << m_phys_memory_map.size()
        << " NumAllocatedPages=" << m_NumAllocatedPages
        << " pagesize=" << get_page_size()
        << " used bytes: " << (m_NumAllocatedPages * get_page_size());
    return PhysAddress(INVALID_PHYS_ADDRESS);
}

Result
TargetSystem::InsertPage(const TranslationUnit &mmu,
    u64 virt_address, const PhysAddress pa, u64 attributes)
{
    PhysAddress curpta = mmu.GetPageTableRootAddress();
    LOG_DEBUG(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address)
                             << " root_pta=" << curpta
                             << " mask=" << fmt::hex(VM_PAGE_PHYS_ADDRESS_MASK);

    u64 entry_phys_address = pa.get_address();
    // u64 const pagesize = GetPageSize();

    u64 next_pta;
    for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
        const PhysAddress next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
        CHECK_RESULT(bus_read(next_te_pa, sizeof(next_pta), &next_pta));

        if (!TranslationEntry(next_pta).is_valid()) {
            PhysAddress const page = alloc_page();
            LOG_DEBUG(LOG_PAGETABLE) << "page=" << page;
            next_pta = page.get_address() | VM_PAGE_FLAG_VALID;
            CHECK_RESULT(bus_write(PhysAddress(next_te_pa), sizeof(next_pta), &next_pta));
        }

        curpta = PhysAddress(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        LOG_DEBUG(LOG_PAGETABLE) << "curpta=" << curpta;
    }

    // last level (0)
    const PhysAddress next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
    CHECK_RESULT(bus_read(next_te_pa, sizeof(next_pta), &next_pta));

    if (TranslationEntry(next_pta).is_valid()) {
        LOG_ERROR(LOG_PAGETABLE)
            << "replaced page entry at address " << next_te_pa
            << " is valid: " << fmt::hex(next_pta);
        return Result::general_protection;
    }

    next_pta = (entry_phys_address & ~VM_PAGE_FLAGS_ALL) | attributes | VM_PAGE_FLAG_VALID;
    CHECK_RESULT(bus_write(PhysAddress(next_te_pa), sizeof(next_pta), &next_pta));

    return Result::continue_execution;
}

Result
TargetSystem::VirtualAllocate(const TranslationUnit &mmu,
    u64 base, u64 size, u64 attributes)
{
    u64 const pagesize = mmu.GetPageSize();

    base = util::rounddown_pow2<u64> (base, pagesize);
    size = util::roundup_pow2<u64> (size, pagesize);

    LOG_DEBUG(LOG_PAGETABLE) << "mmu: " << mmu;

    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        PhysAddress const page = alloc_page();
        LOG_DEBUG(LOG_PAGETABLE) << "phys_page=" << page;
        CHECK_RESULT(InsertPage(mmu, virt_address, page, attributes));
    }
    return Result::continue_execution;
}

Result
TargetSystem::VirtualMap(const TranslationUnit &mmu,
    const VirtAddress va, const PhysAddress pa, u64 size, u64 attributes)
{
    u64 const pagesize = mmu.GetPageSize();
    u64 paddr = pa.get_address();

    u64 const base = util::rounddown_pow2<u64>(va.get_address(), pagesize);
    size = util::roundup_pow2<u64>(size, pagesize);

    LOG_DEBUG(LOG_PAGETABLE) << "mmu: " << mmu;
    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_DEBUG(LOG_PAGETABLE) << "mmio page=" << fmt::hex(paddr);
        CHECK_RESULT(InsertPage(mmu, virt_address, PhysAddress(paddr), attributes));
        paddr += pagesize;
    }
    return Result::continue_execution;
}

Result
TargetSystem::VirtualProtect(const TranslationUnit &mmu, u64 base, u64 size, u64 attributes)
{
    u64 const pagesize = mmu.GetPageSize();

    base = util::rounddown_pow2<u64> (base, pagesize);
    size = util::roundup_pow2<u64> (size, pagesize);

    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_DEBUG(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address);
        PhysAddress curpta = mmu.GetPageTableRootAddress();
        for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
            PhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
            u64 next_pta;
            CHECK_RESULT(bus_read(next_te_pa, sizeof(u64), &next_pta));

            if (!TranslationEntry(next_pta).is_valid()) {
                LOG_ERROR(LOG_PAGETABLE | LOG_EXCEPTION)
                    << "invalid page in protect" << fmt::hex(next_pta);
                return Result::data_page_not_present;
            }

            curpta = PhysAddress(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        }

        // last level
        PhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
        u64 next_pta;
        CHECK_RESULT(bus_read(next_te_pa, sizeof(u64), &next_pta));

        next_pta &= ~VM_PAGE_RIGHT_MASK;
        next_pta |= attributes & VM_PAGE_RIGHT_MASK;
        CHECK_RESULT(bus_write(PhysAddress(next_te_pa), sizeof(u64), &next_pta));
        FlushTranslationBuffer(virt_address);
    }

    return Result::continue_execution;
}

Result
TargetSystem::VirtualFree(const TranslationUnit &mmu, u64 base, u64 size)
{
    u64 const pagesize = mmu.GetPageSize();

    base = util::rounddown_pow2<u64> (base, pagesize);
    size = util::roundup_pow2<u64> (size, pagesize);

    for (u64 virt_address = base; virt_address < base + size; virt_address += pagesize) {
        LOG_DEBUG(LOG_PAGETABLE) << "page=" << fmt::hex(virt_address);
        PhysAddress curpta = mmu.GetPageTableRootAddress();
        for (int level = mmu.GetNumberOfPagingLevels() - 1; level > 0; --level) {
            PhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, level);
            u64 next_pta;
            CHECK_RESULT(bus_read(next_te_pa, sizeof(u64), &next_pta));
            if (!TranslationEntry(next_pta).is_valid()) {
                return Result::data_page_not_present;
            }
            curpta = PhysAddress(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        }

        // last level
        PhysAddress const next_te_pa = mmu.get_next_te_pa(virt_address, curpta, 0);
        u64 next_pta;
        CHECK_RESULT(bus_read(next_te_pa, sizeof(u64), &next_pta));
        PhysAddress page(next_pta & VM_PAGE_PHYS_ADDRESS_MASK);
        CHECK_RESULT(free_page(page, pagesize));
        next_pta = 0;
        CHECK_RESULT(bus_write(next_te_pa, sizeof(u64), &next_pta));
        FlushTranslationBuffer(virt_address);
    }

    return Result::continue_execution;
}

class MemoryPageWriter : public MemoryOperation {
public:
    MemoryPageWriter(void const *_data, size_t _len, size_t _pagesize)
        : data(_data)
        , len(_len)
        , pagesize(_pagesize) {}
    ~MemoryPageWriter() override {}
    Result complex_operation(void *address) override {
        if (len > 0) {
            memcpy(address, data, len);
        }
        if (len < pagesize) {
            memset(reinterpret_cast<u8*>(address) + len, 0, pagesize - len);
        }
        return Result::continue_execution;
    }

private:
    void const *data;
    size_t      len;
    size_t      pagesize;
};

Result
TargetSystem::LoadSegment(Core& core, u64 base, size_t file_size, const u8 *data, u64 final_rights)
{
    if (file_size == 0) {
        return Result::continue_execution;
    }

    u64 const pagesize = core.GetMMU().GetPageSize();

    assert(file_size % bytes_per_sector == 0);
    assert(base % pagesize == 0);
    assert(pagesize % bytes_per_sector == 0);

    const size_t virt_size = util::roundup<u64>(file_size, core.GetMMU().GetPageOffsetSize());

    CHECK_RESULT(VirtualAllocate(core.GetMMU(), base, virt_size, VM_PAGE_RIGHT_WRITE));

    for (size_t i = 0; i < file_size; i += pagesize) {
        const size_t len = std::min(file_size - i, pagesize);
        MemoryPageWriter op(&data[i], len, pagesize);
        CHECK_RESULT(mmu_complex_operation(core, VirtAddress(base + i), pagesize,
             VM_PAGE_RIGHT_WRITE,
             VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL,
             op));
    }

    CHECK_RESULT(VirtualProtect(core.GetMMU(), base, virt_size, final_rights));

    LOG_DEBUG(LOG_LOADER) << "segment set up: base=" << fmt::hex(base)
                          << " size=" << virt_size;
    return Result::continue_execution;
}

Result
TargetSystem::LoadModule(Core& core, const u64 vbase, std::istream& input, ExeFileHeader& header)
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
    CHECK_RESULT(LoadSegment(core, vbase + virt_offset, textsze, &file_src[file_offset],
                 VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL));

    virt_offset += util::roundup<u64>(textsze, bits_per_segment_offset);
    file_offset += textsze;
    CHECK_RESULT(LoadSegment(core, vbase + virt_offset, rodatasze, &file_src[file_offset],
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_FINAL));

    virt_offset += util::roundup<u64>(rodatasze, bits_per_segment_offset);
    file_offset += rodatasze;
    CHECK_RESULT(LoadSegment(core, vbase + virt_offset, datasze, &file_src[file_offset],
                 VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE | VM_PAGE_RIGHT_FINAL));

    LOG_DEBUG(LOG_LOADER)
        <<   "  base       = " << fmt::hex(vbase)
        << "\n  entry      = " << fmt::hex(vbase + header.entry_point)
        << "\n  rodata     = " << fmt::hex(vbase + util::roundup<u64>(textsze, bits_per_segment_offset))
        << "\n  data       = " << fmt::hex(vbase + util::roundup<u64>(textsze, bits_per_segment_offset)
                                                 + util::roundup<u64>(rodatasze, bits_per_segment_offset))
        << '\n';

    return Result::continue_execution;
}

Result
TargetSystem::dump_virt_map_sub_dir(
    const TranslationUnit &mmu, const DumpFormatter &out,
    const PhysAddress pta, u64 virt_address,
    u64 &used, u64 &extra_used, int level)
{
    u64 const pagesize = mmu.GetPageSize();
    extra_used += pagesize; // root page

    u64 const virt_offset = u64(pagesize) << (level * mmu.GetBitsPerPageIndex());

    TranslationEntry const root_pta(pta);
    out << "pta level=" << level << ": ";
    root_pta.dump(out);
    out << fmt::endl;

    std::vector<u8> rootPageData(pagesize);
    CHECK_RESULT(bus_read(root_pta.GetAddress(), pagesize, rootPageData.data()));

    for (size_t offset = 0; offset < pagesize; offset += sizeof(u64), virt_address += virt_offset) {
         const u64 pta_next = *reinterpret_cast<u64*>(&rootPageData[offset]);
         const TranslationEntry cpta(pta_next);

         if (!cpta.is_valid()) {
             LOG_DEBUG(LOG_PAGETABLE)
                 << " reserved page: "
                 << fmt::hex(virt_address);
                 // << cpta.dump(out, flags);
             continue;
         }

         if (level > 0) {
             CHECK_RESULT(dump_virt_map_sub_dir(mmu, out, cpta.GetAddress(),
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

    return Result::continue_execution;
}

Result
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
        return Result::continue_execution;
    }

    u64 used = 0;
    u64 extra_used = 0;

    u64 const user_space_begin_address = 0;
    Result result = dump_virt_map_sub_dir(mmu, out,
        mmu.GetPageTableRootAddress(),
        user_space_begin_address, used, extra_used,
        mmu.GetNumberOfPagingLevels() - 1);

    out << "mapped virtual memory: " << used << " bytes\n";
    out << "used physical pagetable memory: " << extra_used << " bytes\n";
    out.finish_section();
    return result;
}

} // namespace postrisc
