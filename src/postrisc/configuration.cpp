#include <sstream>     // for std::istringstream
#include <iostream>    // for std::cerr, std::cout
#include <fstream>     // for std::ifstream

#include "util/common.hpp"
#include "arch/isa.hpp"
#include "configuration.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "assembler/html_generator.hpp"
#include "assembler/bin_generator.hpp"
#include "emulator/target_system.hpp"

namespace postrisc {

/***********************************************************************************
* command line parameter parsing helpers
***********************************************************************************/
template <typename T>
bool
extract(const std::string& value, T & dst)
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
extract_hex(const std::string& value, T & dst)
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
* command line parameter handlers
***********************************************************************************/
bool Configuration::parse_assemble(char const * const *UNUSED(argv))
{
    command = command_assemble;
    return true;
}

bool Configuration::parse_generate(char const * const *argv)
{
    command = command_generate;
    return extract(argv[0], codesize)
        && extract(argv[1], rodatasize)
        && extract(argv[2], datasize);
}

bool Configuration::parse_config(char const * const *argv)
{
    return emulator_config.parse(argv[0]);
}

bool Configuration::parse_scan(char const * const *UNUSED(argv))
{
    command = command_tokenize;
    return true;
}

bool Configuration::parse_scan_html(char const * const *UNUSED(argv))
{
    command = command_tokenize_html;
    return true;
}

bool Configuration::parse_assemble_c(char const * const *UNUSED(argv))
{
    command = command_assemble_c;
    return true;
}

bool Configuration::parse_disasm(char const * const *UNUSED(argv))
{
    command = command_disassemble;
    return true;
}

bool Configuration::parse_dumpbin(char const * const *UNUSED(argv))
{
    command = command_dumpbin;
    return true;
}

bool Configuration::parse_execute(char const * const *argv)
{
    file_name = argv[0];
    command = command_execute;
    return true;
}

bool Configuration::parse_html(char const * const *UNUSED(argv))
{
    command = command_export_html;
    return true;
}

bool Configuration::parse_llvm(char const * const *UNUSED(argv))
{
    command = command_export_llvm;
    return true;
}

bool Configuration::parse_help(char const * const *UNUSED(argv))
{
    command = command_help;
    return true;
}

bool Configuration::parse_version(char const * const *UNUSED(argv))
{
    command = command_version;
    return true;
}

bool Configuration::parse_dump_file(char const * const *argv)
{
    emulator_config.dump_file_name = argv[0];
    return true;
}

bool Configuration::parse_export_definitions(char const * const *UNUSED(argv))
{
    command = command_export_definitions;
    return true;
}

/***********************************************************************************
* action handlers
***********************************************************************************/
COMMAND_LINE_ACTION_PROTO(version)
{
    std::cout << ARCH_NAME " virtual processor engine.\n"
                 "version: " PACKAGE_VERSION "\n"
                 "date: " PACKAGE_DATE << "\n"
                 ARCH_COPYRIGHT "\n"
              << std::endl;
    return 0;
}

COMMAND_LINE_ACTION_PROTO(tokenize)
{
    Decoder decoder;
    CAssembler assembler(decoder);
    return assembler.Scan(std::cin, std::cout);
}

COMMAND_LINE_ACTION_PROTO(tokenize_html)
{
    Decoder decoder;
    CAssembler assembler(decoder);
    return assembler.ScanHtml(std::cin, std::cout);
}

int Configuration::Assemble(CAssembler::assemble_mode mode)
{
    Decoder decoder;
    CAssembler assembler(decoder);
    emulator_config.ExportDefinitions(assembler);
    return assembler.Assemble(std::cin, std::cout, mode);
}

COMMAND_LINE_ACTION_PROTO(assemble)
{
    return Assemble(CAssembler::assemble_mode_obj);
}

COMMAND_LINE_ACTION_PROTO(assemble_c)
{
    return Assemble(CAssembler::assemble_mode_hex);
}

COMMAND_LINE_ACTION_PROTO(disassemble)
{
    Decoder decoder;
    return decoder.Disassemble(std::cin, std::cout, postrisc::disasm_mode_std);
}

COMMAND_LINE_ACTION_PROTO(dumpbin)
{
    Decoder decoder;
    return decoder.Disassemble(std::cin, std::cout, postrisc::disasm_mode_dump);
}

COMMAND_LINE_ACTION_PROTO(generate)
{
    Decoder decoder;
    CBinaryRandomGenerator generator(decoder);
    return generator.AssembleGenerate(std::cout,
            UINT64_C(1024) * codesize,
            UINT64_C(1024) * rodatasize,
            UINT64_C(1024) * datasize);
}

COMMAND_LINE_ACTION_PROTO(execute)
{
    std::ifstream ifs;
    ifs.open(file_name, std::ifstream::in | std::ifstream::binary);
    if (!ifs.good()) {
        std::cerr << "can't open program file" << std::endl;
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
        ofs.open(emulator_config.dump_file_name, std::ofstream::out | std::ofstream::binary);
        if (!ofs) {
            std::cerr << "can't open file for dumping: " << emulator_config.dump_file_name << fmt::endl;
            return EXIT_FAILURE;
        }
        targetSystem.dump(ofs);
    }
    return EXIT_SUCCESS;
}

COMMAND_LINE_ACTION_PROTO(export_html)
{
    Decoder decoder;
    return arch_html(decoder, std::cout);
}

COMMAND_LINE_ACTION_PROTO(export_llvm)
{
    return llvm::TableGenGenerateEncoding(std::cout);
}

COMMAND_LINE_ACTION_PROTO(export_definitions)
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
    : command(command_nothing)
    , codesize(0)
    , rodatasize(0)
    , datasize(0)
{
    LOG_DEBUG(LOG_PLATFORM) << "configuration ctor";
}

Configuration::~Configuration(void)
{
    LOG_DEBUG(LOG_PLATFORM) << "configuration dtor";
}

static const struct {
    char   argument[24];
    int    additional_arguments;
    char   description[64];
} ConfigurationCommands[] = {
    #define CMDL_PARAM(PARAM, COUNT, NAME, DESCR) { "--" PARAM, COUNT, DESCR },
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

int Configuration::Parse(int argc, char const * const *argv)
{
    static const struct {
        bool   (Configuration::*func)(char const * const *argv);
    } ConfigurationHandlers[] = {
        #define CMDL_PARAM(PARAM, COUNT, NAME, DESCR) { &Configuration::parse_##NAME },
        DECLARE_COMMAND_LINE_PARAMETERS_TABLE(CMDL_PARAM) 
        #undef CMDL_PARAM
    };

    for (int i = 1; i < argc; /*nothing here*/) {
        LOG_DEBUG(LOG_PLATFORM) << "argument: " << argv[i];
        int const k = FindArgument(argv[i]);
        if (k < 0) {
            std::cerr << "unknown argument: " << argv[i] << std::endl;
            return EXIT_FAILURE;
        }
        if (i + 1 + ConfigurationCommands[k].additional_arguments > argc) {
            std::cerr << "not enough arguments for: " << argv[i] << std::endl;
            return EXIT_FAILURE;
        }
        bool const result = (this->*ConfigurationHandlers[k].func)(&argv[i+1]);
        if (!result) {
            std::cerr << "argument parsing failed: " << argv[i] << std::endl;
            return EXIT_FAILURE;
        }
        i += 1; // skip command itself
        i += ConfigurationCommands[k].additional_arguments;
    }

    return 0;
}

int Configuration::usage(std::ostream& out, char const *name) const
{
    out << "Usage:\n"
        << "        " << name << " -option [< input-file] [> output-file]\n";

    out << "Available command line options:"
        << std::endl;
    for (const auto& cmd : ConfigurationCommands) {
        out << "    " << cmd.argument << fmt::endl
            << "        " << cmd.description
            << fmt::endl;
    }
    return 0;
}

int Configuration::run(int argc, char const * const *argv)
{
    Parse(argc, argv);

    switch (command) {
#define COMMAND_LINE_ACTION_CASE(NAME) case command_##NAME: return do_##NAME();
    DECLARE_COMMAND_LINE_ACTIONS_TABLE(COMMAND_LINE_ACTION_CASE)
#undef COMMAND_LINE_ACTION_CASE
        case command_help:
            return usage(std::cout, argv[0]);
        case command_nothing:
            std::cerr << "no action set\n";
            break;
    }

    return EXIT_FAILURE;
}

} // namespace postrisc
