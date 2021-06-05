#include <sstream>     // for std::istringstream
#include <fstream>     // for std::ifstream

//#include <boost/property_tree/ptree.hpp>
//#include <boost/property_tree/xml_parser.hpp>

#include "util/common.hpp"
#include "arch/isa.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "doom/CliDoomInterface.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "emulator_app/TargetSingleApp.hpp"

#include "emulator/TranslationUnit.hpp"
#include "emulator/PerformanceMonitor.hpp"
#include "emulator/Core.hpp"
#include "emulator_system/DeviceMemory.hpp"
#include "emulator_system/DeviceRom.hpp"
#include "emulator_system/DeviceVideo.hpp"
#include "emulator_system/DeviceArray.hpp"

//#include "property_tree_helper.hpp"

#include "configuration.hpp"

namespace postrisc {

#define DECLARE_COMMAND_LINE_PARAMETERS_TABLE(X) \
    X("assemble",            0, assemble,             "assemble to binary: <file.s >file.bin") \
    X("assemble-c",          0, assemble_c,           "switch to assemble for embedding in C/C++") \
    X("base-address",        1, base_address,         "set base address for loaded image") \
    X("device-array",        3, device_array,         "set device array info: paddr vaddr config-size") \
    X("disasm",              0, disasm,               "disassemble binary file (*.bin): <file.bin >file.s") \
    X("dumpbin",             0, dumpbin,              "disassemble and dump binary: <file.bin >file.s") \
    X("dump-file",           1, dump_file,            "dump final state to file") \
    X("env",                 1, env,                  "add guest app environment variable: --env key=value") \
    X("execute",             1, execute,              "execute binary in emulator: file.bin >result.txt") \
    X("exeapp",              0, exeapp,               "execute app binary in emulator: file.bin >result.txt") \
    X("export-definitions",  0, export_definitions,   "export asm known predefined constants") \
    X("help",                0, help,                 "display this information.") \
    X("instruction-limit",   1, instruction_limit,    "set instruction limit (number)") \
    X("log-file",            1, log_file,             "set log file path") \
    X("log-level",           1, log_level,            "set logging level") \
    X("log-subsystem",       1, log_subsystem,        "set logging subsystems mask") \
    X("memory",              2, memory,               "add memory device: size paddr") \
    X("paging",              2, paging,               "set paging info: page_offset (in bits), nlevels") \
    X("pa-size",             1, pa_size,              "set physical address size in bits") \
    X("profiling",           0, profiling,            "do profiling (per bundle)") \
    X("rom",                 3, rom,                  "add ROM image: path paddr vaddr") \
    X("scan",                0, scan,                 "scan and tokenize source: <file.s >file.txt") \
    X("scan-html",           0, scan_html,            "scan and html-markup source: <file.s >file.html") \
    X("timing-info",         0, timing_info,          "report timing info") \
    X("verbose",             0, verbose,              "verbose") \
    X("version",             0, version,              "display this program's version information") \
    X("video",               4, video,                "add video device: paddr vaddr width height") \

enum command_t {
    command_nothing,
#define COMMAND_LINE_ACTION_ENUM(PARAM, NUM, NAME, DESCR) command_##NAME,
    DECLARE_COMMAND_LINE_PARAMETERS_TABLE(COMMAND_LINE_ACTION_ENUM)
#undef COMMAND_LINE_ACTION_ENUM
};

/***********************************************************************************
* command line parameter parsing helpers
***********************************************************************************/
template <typename T> bool extract(const std::string& value, T& dst)
{
    char a, b, c;
    std::istringstream is(value);
    T result;

    // hexadecimal
    if (value.size() >= 2 && (value[0] == '0') && (value[1] == 'x' || value[1] == 'X')) {
        if ((is >> a >> b >> std::boolalpha >> std::hex >> result) && !(is >> c)) {
            dst = result;
            return true;
        } else {
            return false;
        }
    }

    if ((is >> std::boolalpha >> result) && !(is >> c)) {
        dst = result;
        return true;
    } else {
        return false;
    }
}

template<> bool extract(const std::string& value, std::string& dst)
{
    dst = value;
    return true;
}

/***********************************************************************************
* configuration
***********************************************************************************/
Configuration::Configuration(void)
{
    LOG_DEBUG(LOG_PLATFORM) << "configuration ctor";
    // SetDefaults();
}

Configuration::~Configuration(void)
{
    LOG_DEBUG(LOG_PLATFORM) << "configuration dtor";
}

void Configuration::Clear(void)
{
    m_memory_devices.clear();
    m_rom_devices.clear();
}

void Configuration::SetDefaults(void)
{
    Clear();

    m_bitsPerPageOffset = 13;
    m_numberOfPagingLevels = 3;
    m_bits_per_physical_address = 44;
    m_base_address = U64C(0xffffffffffa00000);

    m_ncores = 1;

    m_memory_devices.emplace_back(PhysAddress(U64C(0x0000005432100000)), 0x400000);

    m_rom_devices.emplace_back();
    m_rom_devices.back().phys_address = PhysAddress(U64C(0x0000000fcc000000));
    m_rom_devices.back().virt_address = VirtAddress(U64C(0xffffffffcc000000));
    m_rom_devices.back().data = {{ std::byte{0x12}, std::byte{0x13}, std::byte{0x14}, std::byte{0x15}, std::byte{0x16}, std::byte{0x17}, std::byte{0x18} }};

    m_video_device.phys_address = PhysAddress(U64C(0x0000000fb0000000));
    m_video_device.virt_address = VirtAddress(U64C(0xffffffffb0000000));
    m_video_device.width = 240;
    m_video_device.height = 160;

    m_device_array_config.phys_address = PhysAddress(U64C(0x0000000fee000000));
    m_device_array_config.virt_address = VirtAddress(U64C(0xffffffffee000000));
    m_device_array_config.max_config_space_size = 4 * 1024;
}

#define DEFINE_OFFSET(NAME, TYPE, FIELD) assembler.RegisterDefine(#NAME, offsetof(TYPE, FIELD))

void
Configuration::ExportDefinitions(Assembler& assembler) const
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

    assembler.RegisterDefine("DEVICE_CONFIG_VIRT_BASE", m_device_array_config.virt_address.get_address());
    assembler.RegisterDefine("DEVICE_CONFIG_SPACE_SIZE", m_device_array_config.max_config_space_size);

    if (!m_rom_devices.empty()) {
        assembler.RegisterDefine("ROM_VIRT_BASE", m_rom_devices.back().virt_address.get_address());
    }

    // setup memory devices offsets
    unsigned id = 0;
    unsigned mem_id = 0;
    for (const auto& UNUSED(device_info) : m_memory_devices) {
        id++;
        std::string name = std::string("CONFIG_OFFSET_MEMORY_") + std::to_string(mem_id);
        assembler.RegisterDefine(name.c_str(), id);
        mem_id++;
    }

    // setup core offsets
    unsigned core_id = 0;
    for (u32 i = 0; i < m_ncores; i++) {
        id++;
        std::string name = std::string("CONFIG_OFFSET_CORE_") + std::to_string(core_id);
        assembler.RegisterDefine(name.c_str(), id);
        core_id++;
    }

    // setup video offsets
    id++;
    assembler.RegisterDefine("CONFIG_OFFSET_VIDEO", id);
    assembler.RegisterDefine("VIDEO_VIRT_BASE", m_video_device.virt_address.get_address());
    assembler.RegisterDefine("VIDEO_COMMAND_VIRT_BASE",
        m_device_array_config.virt_address.get_address() + id * m_device_array_config.max_config_space_size);

    // setup ROM offsets
    unsigned rom_id = 0;
    for (const auto& UNUSED(device_info) : m_rom_devices) {
        id++;
        std::string name = std::string("CONFIG_OFFSET_ROM_") + std::to_string(rom_id);
        assembler.RegisterDefine(name.c_str(), id);
        rom_id++;
    }
}

/***********************************************************************************
* command line parser
***********************************************************************************/
static const struct {
    char        argument[24];
    int         additional_arguments;
    command_t   command;
    char        description[80];
} ConfigurationCommands[] = {
    #define CMDL_PARAM(PARAM, COUNT, NAME, DESCR) { "--" PARAM, COUNT, command_##NAME, DESCR },
    DECLARE_COMMAND_LINE_PARAMETERS_TABLE(CMDL_PARAM)
    #undef CMDL_PARAM
};

static int FindArgument(char const *argument)
{
    for (size_t k = 0; k < sizearray(ConfigurationCommands); ++k) {
        if (0 == std::strcmp(argument, ConfigurationCommands[k].argument)) {
            return k;
        }
    }
    return -1;
}

int Configuration::parse(int argc, char **argv)
{
    Clear();
    u64 size;
    u64 phys_addr;
    u64 virt_addr;
    std::vector<std::byte> data;

    for (int i = 1; i < argc && argv[i] != nullptr; /*nothing here*/) {
        LOG_DEBUG(LOG_PLATFORM) << "argument: " << argv[i];
        int const k = FindArgument(argv[i]);
        if (k < 0) {
            if (0 == std::strcmp(argv[i], "--")) {
                return i + 1; // +1 for skip "--"
            }
            std::cerr << "unknown argument: " << argv[i] << std::endl;
            return -1;
        }
        if (i + 1 + ConfigurationCommands[k].additional_arguments > argc) {
            std::cerr << "not enough arguments for: " << argv[i] << std::endl;
            return -1;
        }
        bool result = true;
        switch (ConfigurationCommands[k].command) {
            case command_instruction_limit: {
                u64 instruction_limit = 0;
                result = extract(argv[i+1], instruction_limit);
                if (result) {
                    m_instruction_limit = instruction_limit;
                }
                break;
            }
            case command_log_file:
                util::Logger::OpenLogFile(argv[i+1]);
                xhtml_header(dbgs(), "Postrisc log", "Postrisc log");
                break;
            case command_log_level: {
#if defined(POSTRISC_RUNTIME_LOGS)
                u32 log_level = 0;
                result = extract(argv[i+1], log_level);
                if (result) {
                    util::Logger::set_level(log_level);
                }
#else
                std::cerr << "Dynamic logging doesn't supported in release, recompile with POSTRISC_RUNTIME_LOGS\n";
#endif
                break;
            }
            case command_log_subsystem: {
#if defined(POSTRISC_RUNTIME_LOGS)
                u32 subsystem_mask = 0;
                result = extract(argv[i+1], subsystem_mask);
                if (result) {
                    util::Logger::set_subsystem_mask(subsystem_mask);
                }
#else
                std::cerr << "Dynamic logging doesn't supported in release, recompile with POSTRISC_RUNTIME_LOGS\n";
#endif
                break;
            }
            case command_base_address:
                result = extract(argv[i+1], m_base_address);
                break;
            case command_device_array: {
                u64 item_size = 0;
                result = extract(argv[i+1], phys_addr) &&
                         extract(argv[i+2], virt_addr) &&
                         extract(argv[i+3], item_size);

                m_device_array_config.phys_address = PhysAddress(phys_addr);
                m_device_array_config.virt_address = VirtAddress(virt_addr);
                m_device_array_config.max_config_space_size = item_size;
                } break;
            case command_execute:
                m_command = ConfigurationCommands[k].command;
                m_file_name = argv[i+1];
                break;
            case command_dump_file:
                m_dump_file_name = argv[i+1];
                break;
            case command_env: {
                std::string s(argv[i+1]);
                const auto pos = s.find('=');
                if (pos != std::string::npos && pos > 0) {
                    std::string key = s.substr(0, pos);
                    std::string value = s.substr(pos+1);
                    m_env[key] = value;
                    // std::cerr << "env[" << key << "]=" << value << std::endl;
                } else {
                    std::cerr << "env error: " << s << std::endl;
                    result = false;
                }
                } break;
            case command_memory:
                result = extract(argv[i+1], size) &&
                         extract(argv[i+2], phys_addr);
                if (result) {
                    m_memory_devices.emplace_back(PhysAddress(phys_addr), size);
                }
                break;
            case command_exeapp:
                m_command = ConfigurationCommands[k].command;
                break;
            case command_assemble_c:
                m_assemble_mode = Assembler::assemble_mode_hex;
                break;
            case command_profiling:
                LOG_DEBUG(LOG_PLATFORM) << "profiling";
                m_config_flags |= CONFIG_PROFILE;
                break;
            case command_rom:
                // --rom path 0x0000000fcc000000 0xffffffffcc000000
                result = extract(argv[i+2], phys_addr) &&
                         extract(argv[i+3], virt_addr) &&
                         util::load_file(argv[i+1], data);

                if (result) {
                    m_rom_devices.emplace_back();
                    auto& rom_device = m_rom_devices.back();
                    rom_device.phys_address = PhysAddress(phys_addr);
                    rom_device.virt_address = VirtAddress(virt_addr);
                    rom_device.data.swap(data);
                }
                break;
            case command_timing_info:
                LOG_DEBUG(LOG_PLATFORM) << "timing-info";
                m_config_flags |= CONFIG_TIMING;
                break;
            case command_verbose:
                LOG_DEBUG(LOG_PLATFORM) << "verbose";
                m_config_flags |= CONFIG_VERBOSE;
                break;
            case command_paging:
                result = extract(argv[i+1], m_bitsPerPageOffset) &&
                         extract(argv[i+2], m_numberOfPagingLevels);
                break;
            case command_pa_size:
                result = extract(argv[i+1], m_bits_per_physical_address);
                break;
            case command_video:
                result = extract(argv[i+1], phys_addr) &&
                         extract(argv[i+2], virt_addr) &&
                         extract(argv[i+3], m_video_device.width) &&
                         extract(argv[i+4], m_video_device.height);

                m_video_device.phys_address = PhysAddress(phys_addr);
                m_video_device.virt_address = VirtAddress(virt_addr);
                break;
            case command_version:
            case command_scan:
            case command_scan_html:
            case command_assemble:
            case command_disasm:
            case command_dumpbin:
            case command_export_definitions:
            case command_help:
                m_command = ConfigurationCommands[k].command;
                break;
            default:
                std::cerr << "unknown command: " << (unsigned)(ConfigurationCommands[k].command) << std::endl;
                break;
        }
        if (!result) {
            std::cerr << "argument parsing failed: " << argv[i] << std::endl;
            return -1;
        }
        i += 1; // skip command itself
        i += ConfigurationCommands[k].additional_arguments;
    }

    if (!check()) {
        return -1;
    }
    return argc;
}

/***********************************************************************************
* action handlers
***********************************************************************************/
int Configuration::usage(std::ostream& out) const
{
    out << "Available command line options (with additional parameters):\n";
    size_t max_len = 0;
    for (const auto& cmd : ConfigurationCommands) {
        max_len = std::max(max_len, std::strlen(cmd.argument));
    }
    for (const auto& cmd : ConfigurationCommands) {
        out << std::left << std::setw(max_len) << cmd.argument << "  " << cmd.description << fmt::endl;
    }
    return 0;
}

int Configuration::do_version(void)
{
    std::cout << ARCH_NAME " virtual processor engine.\n"
                 "version: " PACKAGE_VERSION "\n"
                 "date: " PACKAGE_DATE << "\n"
                 ARCH_COPYRIGHT "\n"
              << std::endl;
    return 0;
}

int Configuration::do_tokenize(void)
{
    Decoder decoder;
    Assembler assembler(decoder);
    return assembler.Scan(std::cin, std::cout);
}

int Configuration::do_tokenize_html(void)
{
    Decoder decoder;
    Assembler assembler(decoder);
    xhtml_header(std::cout, "html-program", "Postrisc assembler sample program");
    return assembler.ScanHtml(std::cin, std::cout);
}

int Configuration::do_assemble(void)
{
    Decoder decoder;
    Assembler assembler(decoder);
    ExportDefinitions(assembler);
    return assembler.Assemble(std::cin, std::cout, m_assemble_mode);
}

int Configuration::do_disassemble(void)
{
    Decoder decoder;
    return decoder.Disassemble(std::cin, std::cout, postrisc::disasm_mode_std);
}

int Configuration::do_dumpbin(void)
{
    Decoder decoder;
    return decoder.Disassemble(std::cin, std::cout, postrisc::disasm_mode_dump);
}

int Configuration::do_execute(void)
{
    std::ifstream ifs;
    ifs.open(m_file_name, std::ios_base::in | std::ios_base::binary);
    if (!ifs.good()) {
        std::cerr << "can't open program file " << m_file_name << std::endl;
        return EXIT_FAILURE;
    }

    Decoder decoder;
    TargetSystem targetSystem(decoder);

    Result result = targetSystem.StartEmulation(*this, ifs, std::cout);
    if (!result.continuable()) {
        return EXIT_FAILURE;
    }
    targetSystem.activate_execution();
    targetSystem.WaitFinishEmulation(std::cout);

    if (!m_dump_file_name.empty()) {
        std::ofstream ofs;
        ofs.open(m_dump_file_name, std::ios_base::out | std::ios_base::binary);
        if (!ofs) {
            std::cerr << "can't open file for dumping: " << m_dump_file_name << fmt::endl;
            return EXIT_FAILURE;
        }
        targetSystem.dump(targetSystem.FindMMU(), ofs);
    }
    return EXIT_SUCCESS;
}

int Configuration::do_execute_app(DoomInterface& doom, int argc, char**argv)
{
    Decoder decoder;
    TargetSingleApp target(decoder, doom);

    int const result = target.StartEmulation(*this, argc, argv, m_env);
    if (result < 0) {
        return EXIT_FAILURE;
    }

    if (!m_dump_file_name.empty()) {
        std::ofstream ofs;
        ofs.open(m_dump_file_name, std::ios_base::out | std::ios_base::binary);
        if (!ofs) {
            std::cerr << "can't open file for dumping: " << m_dump_file_name << fmt::endl;
            return EXIT_FAILURE;
        }
        target.dump(ofs);
    }

    return EXIT_SUCCESS;
}

int Configuration::do_export_definitions(void)
{
    Decoder decoder;
    Assembler assembler(decoder);
    ExportDefinitions(assembler);
    assembler.ExportDefinitions(std::cout);
    return EXIT_SUCCESS;
}

int Configuration::do_run(DoomInterface& doom, int argc, char **argv)
{
    int ret = 0;
    switch (m_command) {
        case command_version:            ret = do_version(); break;
        case command_scan:               ret = do_tokenize(); break;
        case command_scan_html:          ret = do_tokenize_html(); break;
        case command_assemble:           ret = do_assemble(); break;
        case command_disasm:             ret = do_disassemble(); break;
        case command_dumpbin:            ret = do_dumpbin(); break;
        case command_execute:            ret = do_execute(); break;
        case command_exeapp:             ret = do_execute_app(doom, argc, argv); break;
        case command_export_definitions: ret = do_export_definitions(); break;
        case command_help:               ret = usage(std::cout); break;
        default:
            ret = EXIT_FAILURE;
            std::cerr << "no action set\n";
            break;
    }

    util::Logger::CloseLogFile();
    return ret;
}

int Configuration::run(int argc, char **argv)
{
    std::vector<char*> host_argv;
    int host_argc = 0;
    int guest_argc_pos = 0;
    for (int i = 0; i < argc; i++) {
        if (0 == std::strcmp(argv[i], "--")) {
            guest_argc_pos = i+1;
            m_command = command_exeapp;
            break;
        }
        host_argv.emplace_back(argv[i]);
        host_argc++;
    }

    int const parsed = parse(host_argc, host_argv.data());
    if (parsed < 0) {
        return EXIT_FAILURE;
    }

    int const guest_argc = argc - guest_argc_pos;
    char **guest_argv = argv + guest_argc_pos;

    CliDoomInterface doom;
    return do_run(doom, guest_argc, guest_argv);
}

#define CHECK_ADDRESS_BITS(ADDRESS, MAXBITS)                                 \
     if (((ADDRESS) & util::makemask<u64>(MAXBITS)) != ADDRESS) {            \
         std::cerr << "mapping_address " << fmt::hex(ADDRESS)                \
                   << " contains unimplemented physical address bits"        \
                   << std::endl;                                             \
         return false;                                                       \
     }

#define CHECK_ADDRESS_ALIGNMENT(ADDRESS, ALIGNMENT)                          \
     if (util::rounddown_pow2<u64>((ADDRESS), ALIGNMENT) != (ADDRESS)) {     \
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

bool Configuration::check(void)
{
    if (m_bits_per_physical_address < 32 || m_bits_per_physical_address > 60) {
        std::cerr << "invalid bits_per_physical_address: " << m_bits_per_physical_address << std::endl;
        return false;
    }
    if (m_bitsPerPageOffset < 12 || m_bitsPerPageOffset > 16) {
        std::cerr << "page_offset_bits should be between 12 and 16 bits: " << m_bitsPerPageOffset << std::endl;
        return false;
    }

    u64 const page_size = U64C(1) << m_bitsPerPageOffset;
    unsigned const virt_address_bits = m_bitsPerPageOffset + (m_bitsPerPageOffset - 3/*log2(8)*/) * m_numberOfPagingLevels;
    if (m_numberOfPagingLevels > 6 || virt_address_bits > 64) {
        std::cerr << "page_table_levels is too big or exceed 64bit for virtual addressing" << std::endl;
        return false;
    }

    TranslationUnit mmu;
    mmu.SetPagingParameters(PhysAddress(0), m_bitsPerPageOffset, m_numberOfPagingLevels);
    CHECK_CANONICAL_ADDRESS(m_base_address);
    CHECK_ADDRESS_ALIGNMENT(m_base_address, page_size);

    if (m_memory_devices.empty()) {
        m_memory_devices.emplace_back(PhysAddress(U64C(0x0000001234500000)), 0x30000);
        m_bits_per_physical_address = std::max(m_bits_per_physical_address, 40U);
    }
    for (const auto&  m : m_memory_devices) {
        CHECK_ADDRESS_BITS(m.address.get_address(), m_bits_per_physical_address);
        CHECK_ADDRESS_ALIGNMENT(m.address.get_address(), page_size);
    }

    for (const auto& rom : m_rom_devices) {
        CHECK_ADDRESS_BITS(rom.phys_address.get_address(), m_bits_per_physical_address);
        CHECK_ADDRESS_ALIGNMENT(rom.phys_address.get_address(), page_size);
        CHECK_CANONICAL_ADDRESS(rom.virt_address.get_address());
        CHECK_ADDRESS_ALIGNMENT(rom.virt_address.get_address(), page_size);
    }

    CHECK_ADDRESS_BITS(m_video_device.phys_address.get_address(), m_bits_per_physical_address);
    CHECK_ADDRESS_ALIGNMENT(m_video_device.phys_address.get_address(), page_size);
    CHECK_CANONICAL_ADDRESS(m_video_device.virt_address.get_address());
    CHECK_ADDRESS_ALIGNMENT(m_video_device.virt_address.get_address(), page_size);

    u64 const cs_size = m_device_array_config.max_config_space_size;
    if (!util::is_pow2(cs_size) || cs_size < 4 * 1024 || cs_size > 64 * 1024) {
        std::cerr << "max_config_space_size should be the power of 2 between 4KiB and 64KiB" << std::endl;
        return false;
    }

    CHECK_ADDRESS_BITS(m_device_array_config.phys_address.get_address(), m_bits_per_physical_address);
    CHECK_ADDRESS_ALIGNMENT(m_device_array_config.phys_address.get_address(), std::max(page_size, cs_size));
    CHECK_CANONICAL_ADDRESS(m_device_array_config.virt_address.get_address());
    CHECK_ADDRESS_ALIGNMENT(m_device_array_config.virt_address.get_address(), std::max(page_size, cs_size));

    return true;
}

} // namespace postrisc
