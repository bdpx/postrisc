#include <sstream>     // for std::istringstream
#include <fstream>     // for std::ifstream

#include "util/common.hpp"
#include "arch/isa.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "doom/cli_doom_interface.hpp"
#include "assembler/bin_generator.hpp"
#include "emulator_system/target_system.hpp"
#include "emulator_app/target_app.hpp"

#include "configuration.hpp"

namespace postrisc {

/***********************************************************************************
* command line parameter parsing helpers
***********************************************************************************/
template <typename T>
bool
extract(const std::string& value, T& dst)
{
    char c;
    std::istringstream is(value);
    T result;
    if ((is >> std::boolalpha >> result) && !(is >> c)) {
        dst = result;
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool
extract_hex(const std::string& value, T& dst)
{
    char c;
    std::istringstream is(value);
    T result;
    if ((is >> std::boolalpha >> std::hex >> result) && !(is >> c)) {
        dst = result;
        return true;
    } else {
        return false;
    }
}

template<>
bool
extract(const std::string& value, std::string& dst)
{
    dst = value;
    return true;
}

/***********************************************************************************
* action handlers
***********************************************************************************/
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
    CAssembler assembler(decoder);
    return assembler.Scan(std::cin, std::cout);
}

int Configuration::do_tokenize_html(void)
{
    Decoder decoder;
    CAssembler assembler(decoder);
    xhtml_header(std::cout, "html-program", "Postrisc assembler sample program");
    return assembler.ScanHtml(std::cin, std::cout);
}

int Configuration::do_assemble(void)
{
    Decoder decoder;
    CAssembler assembler(decoder);
    emulator_config.ExportDefinitions(assembler);
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

int Configuration::do_generate(void)
{
    Decoder decoder;
    CBinaryRandomGenerator generator(decoder);
    return generator.AssembleGenerate(std::cout,
            U64C(1024) * codesize,
            U64C(1024) * rodatasize,
            U64C(1024) * datasize);
}

int Configuration::do_execute(void)
{
    std::ifstream ifs;
    ifs.open(file_name, std::ios_base::in | std::ios_base::binary);
    if (!ifs.good()) {
        std::cerr << "can't open program file " << file_name << std::endl;
        return EXIT_FAILURE;
    }

    Decoder decoder;
    TargetSystem targetSystem(decoder);

    CStatus status = targetSystem.StartEmulation(emulator_config, ifs, std::cout);
    if (!status.continuable()) {
        return EXIT_FAILURE;
    }
    targetSystem.activate_execution();
    targetSystem.WaitFinishEmulation(std::cout);

    if (!emulator_config.dump_file_name.empty()) {
        std::ofstream ofs;
        ofs.open(emulator_config.dump_file_name, std::ios_base::out | std::ios_base::binary);
        if (!ofs) {
            std::cerr << "can't open file for dumping: " << emulator_config.dump_file_name << fmt::endl;
            return EXIT_FAILURE;
        }
        targetSystem.dump(targetSystem.FindMMU(), ofs);
    }
    return EXIT_SUCCESS;
}

int Configuration::do_execute_app(const IDoomInterface& doom, int argc, char**argv, char**env)
{
    Decoder decoder;
    TargetSingleApp target(decoder, doom);

    emulator_config.base_address = 0;
    emulator_config.config_flags |= CONFIG_TIMING | CONFIG_PROFILE;
    int const result = target.StartEmulation(emulator_config, argc, argv, env);
    if (result < 0) {
        return EXIT_FAILURE;
    }

    if (!emulator_config.dump_file_name.empty()) {
        std::ofstream ofs;
        ofs.open(emulator_config.dump_file_name, std::ios_base::out | std::ios_base::binary);
        if (!ofs) {
            std::cerr << "can't open file for dumping: " << emulator_config.dump_file_name << fmt::endl;
            return EXIT_FAILURE;
        }
        target.dump(ofs);
    }

    return EXIT_SUCCESS;
}

int Configuration::do_export_html(void)
{
    Decoder decoder;
    return decoder.GenerateHtml(std::cout);
}

int Configuration::do_export_llvm(void)
{
    Decoder decoder;
    return decoder.LLVMTableGenGenerateEncoding(std::cout);
}

int Configuration::do_export_definitions(void)
{
    Decoder decoder;
    CAssembler assembler(decoder);
    emulator_config.ExportDefinitions(assembler);
    assembler.ExportDefinitions(std::cout);
    return EXIT_SUCCESS;
}

/***********************************************************************************
*
***********************************************************************************/
Configuration::Configuration(void)
{
    LOG_DEBUG(LOG_PLATFORM) << "configuration ctor";
}

Configuration::~Configuration(void)
{
    LOG_DEBUG(LOG_PLATFORM) << "configuration dtor";
}

static const struct {
    char        argument[24];
    int         additional_arguments;
    command_t   command;
    char        description[64];
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

int Configuration::Parse(int argc, char **argv)
{
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
                std::cerr << "Dynamic logging doesn't supported, recompile with POSTRISC_RUNTIME_LOGS" << std::endl;
#endif
                break;
            }
            case command_log_subsystem: {
#if defined(POSTRISC_RUNTIME_LOGS)
                u32 subsystem_mask = 0;
                result = extract_hex(argv[i+1], subsystem_mask);
                if (result) {
                    util::Logger::set_subsystem_mask(subsystem_mask);
                }
#else
                std::cerr << "Dynamic logging doesn't supported, recompile with POSTRISC_RUNTIME_LOGS" << std::endl;
#endif
                break;
            }
            case command_config:
                result = emulator_config.parse(argv[i+1]);
                break;
            case command_generate:
                command = ConfigurationCommands[k].command;
                result = extract(argv[i+1+0], codesize)
                      && extract(argv[i+1+1], rodatasize)
                      && extract(argv[i+1+2], datasize);
                break;
            case command_execute:
                command = ConfigurationCommands[k].command;
                file_name = argv[i+1];
                break;
            case command_dump_file:
                emulator_config.dump_file_name = argv[i+1];
                break;
            case command_exeapp:
                command = ConfigurationCommands[k].command;
                break;
            case command_assemble_c:
                m_assemble_mode = CAssembler::assemble_mode_hex;
                break;
            case command_version:
            case command_scan:
            case command_scan_html:
            case command_assemble:
            case command_disasm:
            case command_dumpbin:
            case command_html:
            case command_llvm:
            case command_export_definitions:
            case command_help:
                command = ConfigurationCommands[k].command;
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

    return argc;
}

int Configuration::usage(std::ostream& out) const
{
    out << "Usage:  -option [< input-file] [> output-file]\n"
           "Available command line options:" << std::endl;
    for (const auto& cmd : ConfigurationCommands) {
        out << "    " << std::left << std::setw(20) << cmd.argument
            << "    " << cmd.description
            << fmt::endl;
    }
    return 0;
}

int Configuration::do_run(const IDoomInterface& doom, int argc, char **argv, char **env)
{
    int ret = 0;
    switch (command) {
        case command_version:            ret = do_version(); break;
        case command_scan:               ret = do_tokenize(); break;
        case command_scan_html:          ret = do_tokenize_html(); break;
        case command_assemble:           ret = do_assemble(); break;
        case command_disasm:             ret = do_disassemble(); break;
        case command_dumpbin:            ret = do_dumpbin(); break;
        case command_generate:           ret = do_generate(); break;
        case command_execute:            ret = do_execute(); break;
        case command_exeapp:             ret = do_execute_app(doom, argc, argv, env); break;
        case command_html:               ret = do_export_html(); break;
        case command_llvm:               ret = do_export_llvm(); break;
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

int Configuration::run(int argc, char **argv, char **env)
{
    std::vector<char*> host_argv;
    int host_argc = 0;
    int guest_argc_pos = 0;
    for (int i = 0; i < argc; i++) {
        if (0 == std::strcmp(argv[i], "--")) {
            guest_argc_pos = i+1;
            command = command_exeapp;
            break;
        }
        host_argv.emplace_back(argv[i]);
        host_argc++;
    }

    int const parsed = Parse(host_argc, host_argv.data());
    if (parsed < 0) {
        return EXIT_FAILURE;
    }

    int const guest_argc = argc - guest_argc_pos;
    char **guest_argv = argv + guest_argc_pos;

    CliDoomInterface doom;
    return do_run(doom, guest_argc, guest_argv, env);
}

} // namespace postrisc
