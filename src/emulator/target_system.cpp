#include <fstream>
#include <thread>      // for std::this_thread
#include <chrono>      // for std::chrono::steady_clock/duration/etc
#include <boost/range/adaptor/reversed.hpp>

#include "util/common.hpp"
#include "arch/interruption.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "binary_format/file_header.hpp"
#include "target_system.hpp"
#include "core.hpp"
#include "utils.hpp"
#include "debugger.hpp"
#include "configuration.hpp"

#include "devices/memory.hpp"
#include "devices/rom.hpp"
#include "devices/video.hpp"
#include "devices/array.hpp"
#include "dump/formatter_html.hpp"
#include "dump/formatter_txt.hpp"

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
Serialize(TargetSystem& ts, const std::string & path)
{
    std::ofstream ofs;
    ofs.open(path, std::ofstream::out | std::ofstream::binary);
    if (!ofs) {
        std::cerr << "Serialization open failed" << fmt::endl;
        return;
    }

    boost::archive::binary_oarchive oa(ofs);
    oa << BOOST_SERIALIZATION_NVP(ts);
}

void
Deserialize(TargetSystem& ts, const std::string & path)
{
    std::ifstream ifs;
    ifs.open(path, std::ofstream::in | std::ofstream::binary);
    if (!ifs) {
        std::cerr << "Deserialization open failed" << fmt::endl;
        return;
    }

    boost::archive::binary_iarchive ia(ifs);
    ia >> BOOST_SERIALIZATION_NVP(ts);
}
#endif

TargetSystem::TargetSystem(const Decoder& decoder)
    : m_CoreNumber(0)
    , m_configuration_flags(0)
    , m_bits_per_physical_address(40)
    , m_page_size(4 * 1024)
    , m_NumAllocatedPages(0)
    , m_Decoder(decoder)
{
}

TargetSystem::~TargetSystem(void)
{
}

void
TargetSystem::dump_mapping(const DumpFormatter& out) const
{
    out.chapter("physical memory dump");
    out << m_bits_per_physical_address << " implemented physical address bits" << fmt::endl;

    uint64_t size = 0;
    for (auto const & mapping : boost::adaptors::reverse(m_phys_memory_map)) {
        uint64_t address = mapping.first;
        out << "mapping: ";
        out.dump_physical_address(address);
        out << "...";
        out.dump_physical_address(address + mapping.second.m_size - 1);
        out << fmt::endl;
        size += mapping.second.m_size;

        out << "internal range: "
            << fmt::hex(mapping.second.get_offset())
            << "..." << fmt::hex(mapping.second.get_offset() + mapping.second.get_size() - 1)
            << ", " << fmt::hex(mapping.second.get_size())
            << " bytes from " << fmt::hex(mapping.second.m_device->size())
            << fmt::endl;

        out << "device_name: " << mapping.second.m_device->get_name() << fmt::endl;

        if (!mapping.second.m_page_bitmap.empty()) {
            size_t i = 0;
            for (bool const x : mapping.second.m_page_bitmap) {
                if ((i & 63) == 0) {
                    out << "  ";
                    out.dump_physical_address_ref(address);
                    out << " page_bitmap: ";
                }
                out << (x ? '1' : '0');
                if ((i & 63) == 63) {
                    out << fmt::endl;
                }
                i++;
                address += m_page_size;
            }
            out << fmt::endl;
        }

        mapping.second.m_device->dump_mapping(mapping.second, out);
        out << fmt::endl;
    }
    out << "mapped physical memory " << size << " bytes" << fmt::endl;
}

void
TargetSystem::dump_statistic(const DumpFormatter& out) const
{
    m_DeviceArray->dump_statistic(out);
}

CStatus
TargetSystem::add_memory_mapping(uint64_t address, PhysMemoryMapping& mapping, bool is_memory)
{
    if ((address & PHYS_ADDRESS_MASK) != address) {
        LOG_FATAL(LOG_PLATFORM) << "unimplemented physical address bits: " << fmt::hex(address);
        return CStatus(CStatus::data_unimplemented_address);
    }
    if ((address & util::makemask(16)) != 0) {
        LOG_FATAL(LOG_PLATFORM) << "unaligned device physical address bits: " << fmt::hex(address);
        return CStatus(CStatus::data_unaligned_address);
    }
    uint64_t const end = address + mapping.m_size;
    if (end < address) {
        LOG_FATAL(LOG_PLATFORM) << "device address wraparound: "
                                << fmt::hex(address) << " - " << fmt::hex(end);
        return CStatus(CStatus::platform_management);
    }

    unsigned const page_size = get_page_size();

    for (const auto & x : m_phys_memory_map) {
        const uint64_t dev_begin = x.first;
        const uint64_t dev_end = dev_begin + x.second.get_size();
        const bool intersection =
               ( dev_begin >= address && dev_begin <  end )
            || ( dev_end   >  address && dev_end   <= end )
            || ( address >= dev_begin && address <  dev_end )
            || ( end     >  dev_begin && end     <= dev_end );

        if (intersection) {
            LOG_FATAL(LOG_PLATFORM) << "ranges are overlapped: "
                                    << fmt::hex(address)
                                    << " and " << fmt::hex(dev_begin);
            return CStatus(CStatus::platform_management);
        }
    }

    if (is_memory) {
        uint64_t const allocatable_size = util::rounddown(mapping.m_size, page_size);
        size_t const num_pages = static_cast<size_t>(allocatable_size / page_size);
        mapping.m_page_bitmap.assign(num_pages, false);
    }
    m_phys_memory_map.insert(std::make_pair(address, mapping));
    return CStatus(CStatus::continue_execution);
}

uint64_t
TargetSystem::size(void) const
{
    uint64_t size = 0;
    for (const auto & x : m_phys_memory_map) {
        size += x.second.get_size();
    }
    return size;
}

Core *
TargetSystem::get_bootstrap_core(void) const
{
    return m_DeviceArray->get_bootstrap_core();
}

void
TargetSystem::activate_execution(void)
{
    return m_DeviceArray->activate_execution(*this);
}

CStatus
TargetSystem::post_interrupt(CStatus ivec)
{
    return m_DeviceArray->post_interrupt(ivec);
}

void TargetSystem::Clear()
{
    m_phys_memory_map.clear();
    m_CoreNumber = 0;
    m_Tasks.clear();
}

CStatus
TargetSystem::Setup(EmulatorConfiguration const & configuration)
{
    Clear();
    m_configuration_flags = configuration.get_config_flags();
    m_bits_per_physical_address = configuration.bits_per_physical_address;

    unsigned const bytes_per_page = UINT32_C(1) << configuration.bitsPerPageOffset;
    m_page_size = bytes_per_page;

    // prepare special device: config space for all other devices
    LOG_TRACE(LOG_LOADER) << "create DeviceArray";
    m_DeviceArray = boost::make_intrusive<DeviceArray>(this, "DeviceArray",
        configuration.device_array_config.max_config_space_size);
    // DeviceArray controller will be automatically the first device in config space
    // don't add it to itself
    // order of other devices may be varied

    // add memory devices/mappings
    // we cam't map to virtual space before adding memory devices
    if (configuration.memory_devices.empty()) {
        EMULATOR_OUT(std::cerr) << "no memory devices" << std::endl;
        return CStatus(CStatus::platform_management);
    }
    unsigned id = 0;
    for (const auto& device_info : configuration.memory_devices) {
        uint64_t const size = device_info.size;
        const std::string name = std::string("memory") + std::to_string(id++);
        boost::intrusive_ptr<device_memory> memory = boost::make_intrusive<device_memory>(this, name, size);

        // memory device should be mapped to config space
        CHECK_STATUS(m_DeviceArray->add_device(memory));

        bool const is_memory = true;
        for (const auto& mapping_info : device_info.mappings) {
            PhysMemoryMapping mapping(
                mapping_info.address,
                mapping_info.size,
                mapping_info.offset,
                memory);
            CHECK_STATUS(add_memory_mapping(mapping_info.address, mapping, is_memory));
        }
    }

    // after memory creation we may alloc page table root page
    LOG_TRACE(LOG_LOADER) << "allocate root page";
    uint64_t const pta = alloc_page();
    LOG_DEBUG(LOG_EMULATOR | LOG_LOADER) << "pta=" << fmt::hex(pta);

    // recursive hook for page table itself
    //*reinterpret_cast<uint64_t *>(phys_memory->GetPointer(pta)) =
    //    pta | VM_PAGE_FLAG_VALID | VM_PAGE_FLAG_KERNEL | VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE;

    LOG_TRACE(LOG_LOADER) << "create processor cores";
    uint32_t core_id = 0;
    for (const auto& core_info : configuration.core_devices) {
        boost::intrusive_ptr<Core> core = boost::make_intrusive<Core>(this, core_info.name, core_id, m_configuration_flags);
        core->reset(core_id);
        core->get_mmu().SetPagingParameters(pta,
            configuration.bitsPerPageOffset, configuration.numberOfPagingLevels);
        // processor cores are only added to config space
        CHECK_STATUS(m_DeviceArray->add_device(core));
        ++core_id;
    }

    // get first added core
    Core *core = m_DeviceArray->get_bootstrap_core();
    
    LOG_TRACE(LOG_LOADER) << "setup video";
    boost::intrusive_ptr<device_video> video = boost::make_intrusive<device_video>(
        this, "video", configuration.video_device.width, configuration.video_device.height);
    uint64_t const v_phys_address = configuration.video_device.phys_address;
    uint64_t const v_virt_address = configuration.video_device.virt_address;
    PhysMemoryMapping video_mapping(v_phys_address, video);
    CHECK_STATUS(add_memory_mapping(v_phys_address, video_mapping, false));
    CHECK_STATUS(core->VirtualMap(v_virt_address, v_phys_address,
        video->size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));
    CHECK_STATUS(m_DeviceArray->add_device(video));

    for (const auto& device_info : configuration.rom_devices) {
        LOG_TRACE(LOG_LOADER) << "setup ROM " << device_info.data.size() << " bytes";
        //rom_data.resize(bytes_per_page, 0xff);
        boost::intrusive_ptr<Device> rom = boost::make_intrusive<device_rom>(this, "rom", device_info.data);
        uint64_t const phys_address = device_info.phys_address;
        PhysMemoryMapping mapping(phys_address, rom);
        CHECK_STATUS(add_memory_mapping(phys_address, mapping, false));
        CHECK_STATUS(core->VirtualMap(device_info.virt_address, phys_address, rom->size(), VM_PAGE_RIGHT_READ));
        CHECK_STATUS(m_DeviceArray->add_device(rom));
    }

    // map device array last, after all changes of its size
    {
        uint64_t const phys_address = configuration.device_array_config.phys_address;
        uint64_t const virt_address = configuration.device_array_config.virt_address;
        m_DeviceArray->config.device_array_address = phys_address;
        m_DeviceArray->config.device_array_len = m_DeviceArray->size();
        PhysMemoryMapping mapping(phys_address, m_DeviceArray);
        CHECK_STATUS(add_memory_mapping(phys_address, mapping, false));
        CHECK_STATUS(core->VirtualMap(virt_address, phys_address,
            m_DeviceArray->size(), VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE));
    }

    LOG_TRACE(LOG_LOADER) << "targetSystem setup completed";
    return CStatus(CStatus::continue_execution);
}

void
TargetSystem::JoinAllCores()
{
    for (auto & task : m_Tasks) {
        CStatus status = task.get();
        UNREFERENCED_PARAMETER(status);
    }

    m_Tasks.clear();
}

CStatus
TargetSystem::activate_execution_handler(Core * core)
{
    CStatus status = core->execute_until_interruption();

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
TargetSystem::Interactive(std::ostream & out)
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
            CStatus status = post_interrupt(CStatus(CStatus::single_step_trap));
            UNREFERENCED_PARAMETER(status);
            JoinAllCores();
            LOG_DEBUG(LOG_EMULATOR) << "debugger start";
            Core * core = get_bootstrap_core();
            Debugger(core).execute();
            LOG_DEBUG(LOG_EMULATOR) << "debugger stop";
            activate_execution();
        }
    }
    JoinAllCores();
    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "program complete!" << std::endl;
        DumpFormatterTxt formatter(out);
        dump_statistic(formatter);
        EMULATOR_OUT(out) << "finish" << std::endl;
    }
}

CStatus
TargetSystem::StartEmulation(EmulatorConfiguration const & configuration,
    std::istream & input, std::ostream & out)
{
    uint64_t const vbase = configuration.base_address;

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "host: " << (bits_per_byte * sizeof(void*)) << " bit\n";
        EMULATOR_OUT(out) << "vbase = " << fmt::hex(vbase) << fmt::endl;
        EMULATOR_OUT(out) << "bitsPerPageOffset = " << configuration.bitsPerPageOffset << fmt::endl;
        EMULATOR_OUT(out) << "numberOfPagingLevels = " << configuration.numberOfPagingLevels << fmt::endl;
    }

    // setup core and virtual memory
    CHECK_STATUS(Setup(configuration));

    Core * core = get_bootstrap_core();
    TranslationUnit & mmu = core->get_mmu();

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "virtual address space depth (in bits) = " << mmu.GetVirtualAddressBits() << fmt::endl;
        EMULATOR_OUT(out) << "highest user address = " << fmt::hex(mmu.GetHighestUserAddress()) << fmt::endl;
        EMULATOR_OUT(out) << "lowest kernel address = " << fmt::hex(mmu.GetLowestKernelAddress()) << fmt::endl;
    }

    CExeFileHeader header;
    CHECK_STATUS(core->LoadModule(vbase, input, header));
    CHECK_STATUS(core->PrepareStack        (vbase + UINT64_C(0x2000000), m_page_size/*header.size_stack*/));
    CHECK_STATUS(core->PrepareBackingStore (vbase + UINT64_C(0x4000000), m_page_size/*header.size_stack*/));
    CHECK_STATUS(core->PrepareThreadStorage(vbase + UINT64_C(0x1000000), m_page_size/*0x4000*/));

    uint64_t const start_ip = vbase + header.entry_point;
    core->SetIp(start_ip);

    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "start" << fmt::endl;
    }

    return CStatus(CStatus::continue_execution);
}

void TargetSystem::dump(std::ostream& out) const
{
    DumpFormatterHtml formatter(out);
    formatter.start_dump();
    dump_statistic(formatter);
    dump_mapping(formatter);
    (void)get_bootstrap_core()->dump_virt_map(formatter);
    formatter.finish_dump();
}

void
TargetSystem::WaitFinishEmulation(std::ostream & out)
{
    JoinAllCores();
    if (m_configuration_flags & CONFIG_VERBOSE) {
        EMULATOR_OUT(out) << "program complete!" << std::endl;
        DumpFormatterTxt formatter(out);
        dump_statistic(formatter);
    }
}

#define FIND_DEVICE(address, len, OFFSET)                                      \
    /* Find the device with the base address */                                \
    /* closest to but less than or equal address */                            \
    auto it = m_phys_memory_map.lower_bound(address);                          \
    /* at least null device should be presented, plus CoreArray */             \
    assert(!m_phys_memory_map.empty());                                        \
    if (unlikely(it == m_phys_memory_map.end())) {                             \
        return CStatus(CStatus::bus_unimplemented_address);                    \
    }                                                                          \
    /* address should be after device base address */                          \
    assert(it->first <= address);                                              \
    if (unlikely(address + len > it->first + it->second.m_size)) {             \
        LOG_ERROR(LOG_EXCEPTION | LOG_PLATFORM)                                \
            << "unimplemented address: " << fmt::hex(address)                  \
            << " size=" << len;                                                \
        return CStatus(CStatus::bus_unimplemented_address);                    \
    }                                                                          \
    address -= it->first;                                                      \
    address += OFFSET;                                                         \
    auto& mapping = it->second;

CStatus
TargetSystem::read(phys_address_t address, size_t len, void *bytes) const
{
    FIND_DEVICE(address, len, it->second.m_offset)
    mapping.m_device->m_reads_count++;
    return mapping.m_device->read(address, len, bytes);
}

CStatus
TargetSystem::write(phys_address_t address, size_t len, const void *bytes) const
{
    FIND_DEVICE(address, len, it->second.m_offset)
    mapping.m_device->m_writes_count++;
    return mapping.m_device->write(address, len, bytes);
}

CStatus
TargetSystem::complex_operation(phys_address_t address, IMemoryOperation& op) const
{
    FIND_DEVICE(address, op.size(), it->second.m_offset)
    mapping.m_device->m_complex_ops_count++;
    return mapping.m_device->complex_operation(address, op);
}

CStatus
TargetSystem::free_page(phys_address_t address, size_t pagesize)
{
    // don't offset to device-internal address,
    // we need raw mapping offset for bitmap
    FIND_DEVICE(address, pagesize, 0)
    size_t const pagenum = address / m_page_size;
    mapping.m_page_bitmap[pagenum] = false;
    --m_NumAllocatedPages;
    return CStatus(CStatus::continue_execution);
}

phys_address_t
TargetSystem::alloc_page(void)
{
    // find free page from lesser addresses
    for (auto & mapping : boost::adaptors::reverse(m_phys_memory_map)) {
        if (mapping.second.m_page_bitmap.empty()) continue;
        for (size_t pagenum = 0; pagenum < mapping.second.m_page_bitmap.size(); ++pagenum) {
            if (mapping.second.m_page_bitmap[pagenum]) continue;
            mapping.second.m_page_bitmap[pagenum] = true;
            ++m_NumAllocatedPages;
            phys_address_t const address = mapping.first + m_page_size * pagenum;
            LOG_TRACE(LOG_PLATFORM) << "page allocated: pagenum=" << pagenum
                                    << " address=" << fmt::hex(address);
            // memset(&m_Data[address], 0, m_page_size);
            return address;
        }
    }

    LOG_FATAL(LOG_PLATFORM | LOG_EXCEPTION)
        << "can't allocate page: NumAllocatedPages=" << m_NumAllocatedPages
        << " size=" << (m_NumAllocatedPages * m_page_size);
    return INVALID_PHYS_ADDRESS;
}

} // namespace postrisc
