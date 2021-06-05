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
    X("assemble",            0, "",                    assemble,             "assemble to binary: <file.s >file.bin") \
    X("assemble-c",          0, "",                    assemble_c,           "assemble to embeddable C/C++ array") \
    X("base-address",        1, "=VA",                 base_address,         "set base virtual address for loaded image") \
    X("device-array",        3, "=PA;VA;size",         device_array,         "set device-array info: physical address PA, virtual address VA, config-size") \
    X("disasm",              0, "",                    disasm,               "disassemble binary file (*.bin): <file.bin >file.s") \
    X("disasm-bin",          0, "",                    disasm_bin,           "disassemble and dump binary: <file.bin >file.s") \
    X("dump-file",           1, "=filepath",           dump_file,            "dump final state to file") \
    X("env",                 1, "=key=value",          env,                  "add guest app environment variable") \
    X("exeapp",              0, "",                    exeapp,               "execute app binary in emulator: file.bin >result.txt") \
    X("execute",             1, "=filepath",           execute,              "execute binary in emulator: file.bin >result.txt") \
    X("export-definitions",  0, "",                    export_definitions,   "export asm known predefined constants") \
    X("export-log-options",  0, "",                    export_log_options,   "export logging options") \
    X("generate-html",       0, "",                    generate_html,        "generate html ISA manual") \
    X("generate-llvm",       0, "",                    generate_llvm,        "generate LLVM encoding tablegen file") \
    X("help",                0, "",                    help,                 "display this information.") \
    X("instruction-limit",   1, "=value",              instruction_limit,    "set instruction limit (number)") \
    X("log-file",            1, "=filepath",           log_file,             "set log file path") \
    X("log-level",           1, "=value",              log_level,            "set logging level") \
    X("log-subsystem",       1, "=mask",               log_subsystem,        "set logging subsystems mask") \
    X("memory",              2, "=size;PA",            memory,               "add memory device: size, physical address PA") \
    X("num-cores",           1, "=value",              num_cores,            "set number of cores (1-4)") \
    X("paging",              2, "=nbits;nlevels",      paging,               "set paging info: page_offset (12-16 bits), number of levels (1-4)") \
    X("pa-size",             1, "=nbits",              pa_size,              "set physical address size in bits (40-60)") \
    X("param",               1, "=anytext",            param,                "add guest app parameter") \
    X("profiling",           0, "",                    profiling,            "do profiling (per bundle)") \
    X("rom",                 3, "=filepath;PA;VA",     rom,                  "add ROM image: file path, physical address PA, virtual address VA") \
    X("scan",                0, "",                    scan,                 "scan and tokenize source: <file.s >file.txt") \
    X("scan-html",           0, "",                    scan_html,            "scan and html-markup source: <file.s >file.html") \
    X("timing-info",         0, "",                    timing_info,          "report timing info") \
    X("verbose",             0, "",                    verbose,              "verbose") \
    X("version",             0, "",                    version,              "display this program's version information") \
    X("video",               4, "=PA;VA;width;heigth", video,                "add video device: set physical address PA, virtual address VA, width, height") \

enum command_t {
    command_nothing,
#define COMMAND_LINE_ACTION_ENUM(PARAM, NUM, SAMPLE, NAME, DESCR) command_##NAME,
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

    RomConfig& rom = m_rom_devices.emplace_back();
    rom.phys_address = PhysAddress(U64C(0x0000000fcc000000));
    rom.virt_address = VirtAddress(U64C(0xffffffffcc000000));
    rom.data = {{ std::byte{0x12}, std::byte{0x13}, std::byte{0x14}, std::byte{0x15}, std::byte{0x16}, std::byte{0x17}, std::byte{0x18} }};

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
    u32         additional_arguments;
    command_t   command;
    char        sample[32];
    char        description[80];
} ConfigurationCommands[] = {
    #define CMDL_PARAM(PARAM, COUNT, SAMPLE, NAME, DESCR) { "--" PARAM, COUNT, command_##NAME, SAMPLE, DESCR },
    DECLARE_COMMAND_LINE_PARAMETERS_TABLE(CMDL_PARAM)
    #undef CMDL_PARAM
};

static int FindArgument(const char *argument)
{
    const size_t arg_len = std::strlen(argument);

    for (size_t k = 0; k < sizearray(ConfigurationCommands); k++) {
        const size_t len = std::strlen(ConfigurationCommands[k].argument);

        if (arg_len < len) continue;
        // command should exactly fit in "command" or "command=values" cases
        if (argument[len] != '\0' && argument[len] != '=') continue;

        if (0 == std::strncmp(argument, ConfigurationCommands[k].argument, len)) {
            return k;
        }
    }
    return -1;
}

static std::vector<std::string> split(std::string s, u32 additional_arguments)
{
    std::vector<std::string> tokens;

    if (additional_arguments > 1) {
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(";")) != std::string::npos) {
            token = s.substr(0, pos);
            tokens.push_back(token);
            s.erase(0, pos + 1);
        }
    }

    tokens.push_back(s);
    return tokens;
}

bool Configuration::parse(int argc, char **argv)
{
    Clear();
    u64 size = 0;
    u64 phys_addr = 0;
    u64 virt_addr = 0;
    std::vector<std::byte> data;
    std::vector<std::string> params;

    for (int i = 1; i < argc && argv[i] != nullptr; i++) {
        LOG_DEBUG(LOG_PLATFORM) << "argument: " << argv[i];
        const int k = FindArgument(argv[i]);
        if (k < 0) {
            std::cerr << "unknown argument: " << argv[i] << std::endl;
            continue;
        }
        if (ConfigurationCommands[k].additional_arguments > 0) {
            const char *p = argv[i] + std::strlen(ConfigurationCommands[k].argument);
            if (p[0] != '=') {
                std::cerr << "there is no additional arguments: " << argv[i] << std::endl;
                return false;
            }
            ++p; // skip '='
            params = split(p, ConfigurationCommands[k].additional_arguments);

            if (params.size() != ConfigurationCommands[k].additional_arguments) {
                std::cerr << "not enough arguments for: " << argv[i] << std::endl;
                return false;
            }
        }
        bool result = true;
        switch (ConfigurationCommands[k].command) {
            case command_instruction_limit: {
                u64 instruction_limit = 0;
                result = extract(params[0], instruction_limit);
                if (result) {
                    m_instruction_limit = instruction_limit;
                }
                break;
            }
            case command_log_file:
                util::Logger::OpenLogFile(params[0]);
                xhtml_header(dbgs(), "Postrisc log", "Postrisc log");
                break;
            case command_log_level: {
#if defined(POSTRISC_RUNTIME_LOGS)
                u32 log_level = 0;
                result = extract(params[0], log_level);
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
                result = extract(params[0], subsystem_mask);
                if (result) {
                    util::Logger::set_subsystem_mask(subsystem_mask);
                }
#else
                std::cerr << "Dynamic logging doesn't supported in release, recompile with POSTRISC_RUNTIME_LOGS\n";
#endif
                break;
            }
            case command_base_address:
                result = extract(params[0], m_base_address);
                break;
            case command_device_array: {
                u64 item_size = 0;
                result = extract(params[0], phys_addr) &&
                         extract(params[1], virt_addr) &&
                         extract(params[2], item_size);

                m_device_array_config.phys_address = PhysAddress(phys_addr);
                m_device_array_config.virt_address = VirtAddress(virt_addr);
                m_device_array_config.max_config_space_size = item_size;
                } break;
            case command_execute:
                m_command = ConfigurationCommands[k].command;
                m_file_name = params[0];
                break;
            case command_dump_file:
                m_dump_file_name = params[0];
                break;
            case command_param:
                m_param.push_back(params[0]);
                break;
            case command_env: {
                std::string s = params[0];
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
            case command_num_cores: {
                u32 ncores = 0;
                result = extract(params[0], ncores);
                if (result) {
                    m_ncores = ncores;
                }
                } break;
            case command_memory:
                result = extract(params[0], size) &&
                         extract(params[1], phys_addr);
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
                result = extract(params[1], phys_addr) &&
                         extract(params[2], virt_addr) &&
                         util::load_file(params[0], data);

                if (result) {
                    RomConfig& rom_device = m_rom_devices.emplace_back();
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
                result = extract(params[0], m_bitsPerPageOffset) &&
                         extract(params[1], m_numberOfPagingLevels);
                break;
            case command_pa_size:
                result = extract(params[0], m_bits_per_physical_address);
                break;
            case command_video:
                result = extract(params[0], phys_addr) &&
                         extract(params[1], virt_addr) &&
                         extract(params[2], m_video_device.width) &&
                         extract(params[3], m_video_device.height);

                m_video_device.phys_address = PhysAddress(phys_addr);
                m_video_device.virt_address = VirtAddress(virt_addr);
                break;
            case command_version:
            case command_scan:
            case command_scan_html:
            case command_assemble:
            case command_disasm:
            case command_disasm_bin:
            case command_export_definitions:
            case command_export_log_options:
            case command_generate_html:
            case command_generate_llvm:
            case command_help:
                m_command = ConfigurationCommands[k].command;
                break;
            default:
                std::cerr << "unknown command: " << (unsigned)(ConfigurationCommands[k].command) << std::endl;
                break;
        }
        if (!result) {
            std::cerr << "argument parsing failed: " << argv[i] << std::endl;
            return false;
        }
    }

    if (!check()) {
        return false;
    }
    return true;
}

/***********************************************************************************
* action handlers
***********************************************************************************/
int Configuration::usage(std::ostream& out) const
{
    out << "Available command line options (with additional parameters):\n";
    size_t max_len = 0;
    for (const auto& cmd : ConfigurationCommands) {
        max_len = std::max(max_len, std::strlen(cmd.argument) + std::strlen(cmd.sample));
    }
    for (const auto& cmd : ConfigurationCommands) {
        out << cmd.argument << std::left << std::setw(max_len - std::strlen(cmd.argument)) << cmd.sample
            << "  " << cmd.description << fmt::endl;
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

int Configuration::do_disassemble(disassembler_mode mode)
{
    Decoder decoder;
    return decoder.Disassemble(std::cin, std::cout, mode);
}

int Configuration::do_execute(void)
{
    std::ifstream ifs;
    ifs.open(m_file_name, std::ios_base::in | std::ios_base::binary);
    if (!ifs.good()) {
        std::cerr << "can't open program file " << m_file_name << std::endl;
        return EXIT_FAILURE;
    }

    TargetSystem targetSystem;

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

int Configuration::do_execute_app(DoomInterface& doom)
{
    TargetSingleApp target(doom);

    int const result = target.StartEmulation(*this, m_param, m_env);
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

int Configuration::do_export_log_options(void)
{
    util::Logger::ExportLogOptions(std::cout);
    return EXIT_SUCCESS;
}

int Configuration::do_generate_html(void)
{
    postrisc::Decoder decoder;
    decoder.GenerateHtml(std::cout);
    return EXIT_SUCCESS;
}

int Configuration::do_generate_llvm(void)
{
    postrisc::Decoder decoder;
    decoder.LLVMTableGenGenerateEncoding(std::cout);
    return EXIT_SUCCESS;
}

int Configuration::do_run(DoomInterface& doom)
{
    int ret = 0;
    switch (m_command) {
        case command_version:            ret = do_version(); break;
        case command_scan:               ret = do_tokenize(); break;
        case command_scan_html:          ret = do_tokenize_html(); break;
        case command_assemble:           ret = do_assemble(); break;
        case command_disasm:             ret = do_disassemble(postrisc::disasm_mode_std); break;
        case command_disasm_bin:         ret = do_disassemble(postrisc::disasm_mode_dump); break;
        case command_execute:            ret = do_execute(); break;
        case command_exeapp:             ret = do_execute_app(doom); break;
        case command_export_definitions: ret = do_export_definitions(); break;
        case command_export_log_options: ret = do_export_log_options(); break;
        case command_generate_html:      ret = do_generate_html(); break;
        case command_generate_llvm:      ret = do_generate_llvm(); break;
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
    if (!parse(argc, argv)) {
        return EXIT_FAILURE;
    }

    CliDoomInterface doom;
    return do_run(doom);
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
