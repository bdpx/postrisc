#include <fstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "util/common.hpp"
#include "arch/isa.hpp"
#include "emulator/translation_unit.hpp"
#include "emulator/performance_monitor.hpp"
#include "emulator/core.hpp"
#include "emulator_system/devices/memory.hpp"
#include "emulator_system/devices/rom.hpp"
#include "emulator_system/devices/video.hpp"
#include "emulator_system/devices/array.hpp"
#include "emulator/dump/formatter.hpp"

#include "property_tree_helper.hpp"
#include "emulator_configuration.hpp"

namespace postrisc {

EmulatorConfiguration::EmulatorConfiguration(void)
    : base_address(0x0)
    , config_flags(0)
    , bitsPerPageOffset(12)
    , numberOfPagingLevels(3)
{
    SetDefaults();
}

EmulatorConfiguration::~EmulatorConfiguration(void)
{
}

unsigned
EmulatorConfiguration::get_config_flags(void) const
{
    return config_flags;
}

void EmulatorConfiguration::Clear(void)
{
    core_devices.clear();
    memory_devices.clear();
    rom_devices.clear();
}

void EmulatorConfiguration::SetDefaults(void)
{
    Clear();

    bitsPerPageOffset = 13;
    numberOfPagingLevels = 3;
    bits_per_physical_address = 44;
    base_address = U64C(0xffffffffffa00000);

    core_devices.emplace_back();
    core_devices.back().name = "core:0";

    memory_devices.emplace_back(CPhysAddress(U64C(0x0000065432100000)), 0x400000);

    rom_devices.emplace_back();
    rom_devices.back().phys_address = CPhysAddress(U64C(0x0000000fcc000000));
    rom_devices.back().virt_address = CVirtAddress(U64C(0xffffffffcc000000));
    rom_devices.back().data = {{ 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18 }};

    video_device.phys_address = CPhysAddress(U64C(0x0000000fb0000000));
    video_device.virt_address = CVirtAddress(U64C(0xffffffffb0000000));
    video_device.width = 240;
    video_device.height = 160;

    device_array_config.phys_address = CPhysAddress(U64C(0x0000000fee000000));
    device_array_config.virt_address = CVirtAddress(U64C(0xffffffffee000000));
    device_array_config.max_config_space_size = 4 * 1024;
}

#define DEFINE_OFFSET(NAME, TYPE, FIELD) assembler.RegisterDefine(#NAME, offsetof(TYPE, FIELD))

void
EmulatorConfiguration::ExportDefinitions(CAssembler& assembler) const
{
    // compile-time definitions (struct filed offsets, register indexes, etc)
    // performance monitor counters numbers
#define DEFINE_PERFORMANCE_MONITOR_DEF(NAME) assembler.RegisterDefine("PMC_" #NAME, PMC_##NAME);
    DEFINE_PERFORMANCE_MONITOR_TABLE(DEFINE_PERFORMANCE_MONITOR_DEF)
#undef DEFINE_PERFORMANCE_MONITOR_DEF

    // common device mmio offsets
    DEFINE_OFFSET(DEVICE_HEADER_VENDOR,       ConfigSpaceHeader, vendor_id);
    DEFINE_OFFSET(DEVICE_HEADER_CONFIG_SIZE,  ConfigSpaceHeader, size);

    // devicearray mmio offsets
    DEFINE_OFFSET(DEVICE_CONTROL_DID,             DeviceArrayConfigSpace, pci_did);
    DEFINE_OFFSET(DEVICE_CONTROL_CMD,             DeviceArrayConfigSpace, pci_cmd);
    DEFINE_OFFSET(DEVICE_CONTROL_ARRAY_ADDRESS,   DeviceArrayConfigSpace, device_array_address);
    DEFINE_OFFSET(DEVICE_CONTROL_ARRAY_LEN,       DeviceArrayConfigSpace, device_array_len);
    DEFINE_OFFSET(DEVICE_CONTROL_MAX_SIZE,        DeviceArrayConfigSpace, max_size);

    // core device mmio offsets
    DEFINE_OFFSET(DEVICE_CORE_INTERRUPT_VECTOR,   CoreConfigSpace, m_InterruptVector);
    DEFINE_OFFSET(DEVICE_CORE_TIMECMP,            CoreConfigSpace, m_TimeCmp);

    // memory device mmio offsets
    DEFINE_OFFSET(DEVICE_MEMORY_SIZE, MemoryConfigSpace, size);

    // rom device mmio offsets
    DEFINE_OFFSET(DEVICE_ROM_SIZE, RomConfigSpace, size);

    // video device mmio offsets
    DEFINE_OFFSET(DEVICE_VIDEO_WIDTH,             VideoConfigSpace, width);
    DEFINE_OFFSET(DEVICE_VIDEO_HEIGHT,            VideoConfigSpace, height);
    DEFINE_OFFSET(DEVICE_VIDEO_CLEAR,             VideoConfigSpace, clear);
    DEFINE_OFFSET(DEVICE_VIDEO_REDRAW,            VideoConfigSpace, redraw);
    DEFINE_OFFSET(DEVICE_VIDEO_FOREGROUND_COLOR,  VideoConfigSpace, foreground_color);
    DEFINE_OFFSET(DEVICE_VIDEO_BACKGROUND_COLOR,  VideoConfigSpace, background_color);
    DEFINE_OFFSET(DEVICE_VIDEO_CHARACTER_POS_X,   VideoConfigSpace, character_pos_x);
    DEFINE_OFFSET(DEVICE_VIDEO_CHARACTER_POS_Y,   VideoConfigSpace, character_pos_y);
    DEFINE_OFFSET(DEVICE_VIDEO_CHARACTER_VALUE,   VideoConfigSpace, character_value);


    // run-time config definitions
    // config-based definitions

    assembler.RegisterDefine("DEVICE_CONFIG_VIRT_BASE", device_array_config.virt_address.get_address());
    assembler.RegisterDefine("DEVICE_CONFIG_SPACE_SIZE", device_array_config.max_config_space_size);

    if (!rom_devices.empty()) {
        assembler.RegisterDefine("ROM_VIRT_BASE", rom_devices.back().virt_address.get_address());
    }

    // setup memory devices offsets
    unsigned id = 0;
    unsigned mem_id = 0;
    for (const auto& UNUSED(device_info) : memory_devices) {
        id++;
        std::string name = std::string("CONFIG_OFFSET_MEMORY_") + std::to_string(mem_id);
        assembler.RegisterDefine(name.c_str(), id);
        mem_id++;
    }

    // setup core offsets
    unsigned core_id = 0;
    for (const auto& UNUSED(core_info) : core_devices) {
        id++;
        std::string name = std::string("CONFIG_OFFSET_CORE_") + std::to_string(core_id);
        assembler.RegisterDefine(name.c_str(), id);
        core_id++;
    }
    
    // setup video offsets
    id++;
    assembler.RegisterDefine("CONFIG_OFFSET_VIDEO", id);
    assembler.RegisterDefine("VIDEO_VIRT_BASE", video_device.virt_address.get_address());
    assembler.RegisterDefine("VIDEO_COMMAND_VIRT_BASE",
        device_array_config.virt_address.get_address() + id * device_array_config.max_config_space_size);

    // setup ROM offsets
    unsigned rom_id = 0;
    for (const auto& UNUSED(device_info) : rom_devices) {
        id++;
        std::string name = std::string("CONFIG_OFFSET_ROM_") + std::to_string(rom_id);
        assembler.RegisterDefine(name.c_str(), id);
        rom_id++;
    }
}

#define CHECK_ADDRESS_BITS(ADDRESS, MAXBITS)                                 \
     if (((ADDRESS) & util::makemask64(MAXBITS)) != ADDRESS) {               \
         std::cerr << "mapping_address " << fmt::hex(ADDRESS)                \
                   << " contains unimplemented physical address bits"        \
                   << std::endl;                                             \
         return false;                                                       \
     }

#define CHECK_ADDRESS_ALIGNMENT(ADDRESS, ALIGNMENT)                          \
     if (util::align_down<u64>((ADDRESS), ALIGNMENT) != (ADDRESS)) {    \
         std::cerr << "address " << fmt::hex(ADDRESS)                        \
                   << " should be aligned not less than ("                   \
                   << (ALIGNMENT) << ")" << std::endl;                       \
         return false;                                                       \
     }

#define CHECK_CANONICAL_ADDRESS(ADDRESS)                                     \
     if (!mmu.is_canonical(ADDRESS)) {                                       \
         std::cerr << "address " << fmt::hex(ADDRESS)                        \
                   << " should be canonical" << std::endl;                   \
         return false;                                                       \
     }


bool EmulatorConfiguration::parse(char const *filename)
{
    Clear();

    config_flags = 0;
    boost::property_tree::ptree pt;
    read_xml(filename, pt, boost::property_tree::xml_parser::trim_whitespace);

    const auto& config = pt.get_child("config");
    const auto& parameters = config.get_child("parameters");

    bool const verbose = parameters.get<bool>("verbose");
    LOG_DEBUG(LOG_PLATFORM) << "verbose=" << verbose;
    if (verbose) config_flags |= CONFIG_VERBOSE;

    bool const timing = parameters.get<bool>("timing_info");
    LOG_DEBUG(LOG_PLATFORM) << "timing=" << timing;
    if (timing) config_flags |= CONFIG_TIMING;

    bool const profiling = parameters.get<bool>("profiling");
    LOG_DEBUG(LOG_PLATFORM) << "profiling=" << profiling;
    if (profiling) config_flags |= CONFIG_PROFILE;

    bool const dump_host_info = parameters.get<bool>("dump_host_info");
    LOG_DEBUG(LOG_PLATFORM) << "dump_host_info=" << dump_host_info;
    if (dump_host_info) config_flags |= CONFIG_DUMP_HOST_INFO;

    const auto& system = config.get_child("system");

    bits_per_physical_address = system.get<unsigned>("bits_per_physical_address");
    LOG_DEBUG(LOG_PLATFORM) << "bits_per_physical_address=" << bits_per_physical_address;
    if (bits_per_physical_address < 32 || bits_per_physical_address > 60) {
        std::cerr << "invalid bits_per_physical_address: " << bits_per_physical_address << std::endl;
        return false;
    }

    bitsPerPageOffset = system.get<unsigned>("page_offset_bits");
    LOG_DEBUG(LOG_PLATFORM) << "bitsPerPageOffset=" << bitsPerPageOffset;
    if (bitsPerPageOffset < 12 || bitsPerPageOffset > 16) {
        std::cerr << "page_offset_bits should be between 12 and 16 bits" << std::endl;
        return false;
    }
    u64 const page_size = U64C(1) << bitsPerPageOffset;

    numberOfPagingLevels = system.get<unsigned>("page_table_levels");
    LOG_DEBUG(LOG_PLATFORM) << "numberOfPagingLevels=" << numberOfPagingLevels;
    unsigned const virt_address_bits = bitsPerPageOffset + (bitsPerPageOffset - 3) * numberOfPagingLevels;
    if (numberOfPagingLevels > 6 || virt_address_bits > 64) {
        std::cerr << "page_table_levels is too big or exceed 64bit for virtual addressing" << std::endl;
        return false;
    }

    TranslationUnit mmu;
    mmu.SetPagingParameters(CPhysAddress(0), bitsPerPageOffset, numberOfPagingLevels);

    base_address = config.get<util::hexadecimal<u64>>("loader.base_address");
    LOG_DEBUG(LOG_PLATFORM) << "base_address=" << fmt::hex(base_address);
    CHECK_CANONICAL_ADDRESS(base_address);
    CHECK_ADDRESS_ALIGNMENT(base_address, page_size);

    for (const auto& device : config.get_child("devices")) {
        LOG_DEBUG(LOG_PLATFORM) << "device_name=" << device.first;
        auto type = device.second.get_optional<std::string>("<xmlattr>.type");
        if (type) {
            LOG_DEBUG(LOG_PLATFORM) << "type=" << type.get();
        }
        if (device.first == "core") {
            core_devices.emplace_back();
            auto& core_device = core_devices.back();
            core_device.name = device.second.get<std::string>("name");
            LOG_DEBUG(LOG_PLATFORM) << "core_device.name=" << core_device.name;
        } else if (device.first == "memory") {
            u64 const mapping_address = device.second.get<util::hexadecimal<u64>>("address");
            LOG_DEBUG(LOG_PLATFORM) << "mapping=" << fmt::hex(mapping_address);
            CHECK_ADDRESS_BITS(mapping_address, bits_per_physical_address);
            CHECK_ADDRESS_ALIGNMENT(mapping_address, page_size);
            size_t const memory_size = device.second.get<util::hexadecimal<size_t>>("size");
            LOG_DEBUG(LOG_PLATFORM) << "memory_size=" << memory_size;
            memory_devices.emplace_back(CPhysAddress(mapping_address), memory_size);
        } else if (device.first == "rom") {
            rom_devices.emplace_back();
            auto& rom_device = rom_devices.back();

            u64 const phys_address = device.second.get<util::hexadecimal<u64>>("phys_address");
            rom_device.phys_address = CPhysAddress(phys_address);
            CHECK_ADDRESS_BITS(phys_address, bits_per_physical_address);
            CHECK_ADDRESS_ALIGNMENT(phys_address, page_size);

            u64 const virt_address = device.second.get<util::hexadecimal<u64>>("virt_address");
            CHECK_CANONICAL_ADDRESS(virt_address);
            CHECK_ADDRESS_ALIGNMENT(virt_address, page_size);
            rom_device.virt_address = CVirtAddress(virt_address);

            std::string const rom_filename = device.second.get<std::string>("filename");
            std::ifstream ifs;
            ifs.open(rom_filename, std::ifstream::in | std::ifstream::binary);
            if (!ifs.good()) {
                std::cerr << "can't open ROM file" << std::endl;
                return false;
            }
            char chr;
            while (ifs.get(chr)) {
                rom_device.data.push_back(chr);
            }
        } else if (device.first == "video") {
            u64 const phys_address = device.second.get<util::hexadecimal<u64>>("phys_address");
            video_device.phys_address = CPhysAddress(phys_address);
            LOG_DEBUG(LOG_PLATFORM) << "video.phys_address " << fmt::hex(phys_address);
            CHECK_ADDRESS_BITS(phys_address, bits_per_physical_address);
            CHECK_ADDRESS_ALIGNMENT(phys_address, page_size);

            u64 const virt_address = device.second.get<util::hexadecimal<u64>>("virt_address");
            LOG_DEBUG(LOG_PLATFORM) << "video.virt_address " << fmt::hex(virt_address);
            CHECK_CANONICAL_ADDRESS(virt_address);
            CHECK_ADDRESS_ALIGNMENT(virt_address, page_size);
            video_device.virt_address = CVirtAddress(virt_address);

            video_device.width = device.second.get<unsigned>("width");
            video_device.height = device.second.get<unsigned>("height");
        } else if (device.first == "devicearray") {
            u64 const cs_size = device.second.get<util::hexadecimal<u64>>("max_config_space_size");
            device_array_config.max_config_space_size = cs_size;
            if (!util::is_pow2(cs_size) || cs_size < 4 * 1024 || cs_size > 64 * 1024) {
                std::cerr << "max_config_space_size should be the power of 2 between 4KiB and 64KiB" << std::endl;
                return false;
            }
            u64 const phys_address = device.second.get<util::hexadecimal<u64>>("phys_address");
            CHECK_ADDRESS_BITS(phys_address, bits_per_physical_address);
            CHECK_ADDRESS_ALIGNMENT(phys_address, std::max(page_size, cs_size));
            device_array_config.phys_address = CPhysAddress(phys_address);

            u64 const virt_address = device.second.get<util::hexadecimal<u64>>("virt_address");
            CHECK_CANONICAL_ADDRESS(virt_address);
            CHECK_ADDRESS_ALIGNMENT(virt_address, std::max(page_size, cs_size));
            device_array_config.virt_address = CVirtAddress(virt_address);
        } else {
            std::cerr << "unknown device type (ignored): " << device.first << std::endl;
        }
    }

    return true;
}

} // namespace postrisc
