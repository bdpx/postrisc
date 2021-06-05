#pragma once

#include "emulator/configuration.hpp"
#include "assembler/assembler.hpp"

namespace postrisc {

enum cmd_parameter_t {
};

#define DECLARE_COMMAND_LINE_PARAMETERS_TABLE(X) \
    X("assemble",            0, assemble,             "assemble to binary: <file.s >file.bin") \
    X("assemble_c",          0, assemble_c,           "assemble for embedding in C/C++: < file.s > file.c") \
    X("config",              1, config,               "import xml configuration file") \
    X("disasm",              0, disasm,               "disassemble binary file (*.bin): <file.bin > file.s") \
    X("dumpbin",             0, dumpbin,              "disassemble and dump binary: -dumpbin < file.bin > file.s") \
    X("execute",             1, execute,              "execute binary in emulator: file.bin > result.txt") \
    X("generate",            3, generate,             "generate testing binary: code rdata data > file.bin") \
    X("help",                0, help,                 "Display this information.") \
    X("html",                0, html,                 "generate help in html about ISA: > file.html") \
    X("llvm",                0, llvm,                 "write llvm tablegen postrisc.td") \
    X("dump-file",           1, dump_file,            "dump to file") \
    X("export_definitions",  0, export_definitions,   "export asm definitions") \
    X("scan",                0, scan,                 "scan and tokenize source: < file.s > file.txt") \
    X("scanhtml",            0, scan_html,            "scan and html-markup source: < file.s > file.html") \
    X("version",             0, version,              "Display this program's version information.") \


#define DECLARE_COMMAND_LINE_ACTIONS_TABLE(X) \
    X(version) \
    X(tokenize) \
    X(tokenize_html) \
    X(assemble) \
    X(assemble_c) \
    X(disassemble) \
    X(dumpbin) \
    X(generate) \
    X(execute) \
    X(export_html) \
    X(export_llvm) \
    X(export_definitions) \

enum command_t {
#define COMMAND_LINE_ACTION_ENUM(NAME) command_##NAME,
    DECLARE_COMMAND_LINE_ACTIONS_TABLE(COMMAND_LINE_ACTION_ENUM)
#undef COMMAND_LINE_ACTION_ENUM
    command_help,
    command_nothing,
};

class Configuration {
public:
    Configuration(void);
    ~Configuration(void);

    int Parse(int argc, char const * const *argv);
    int run(int argc, char const * const *argv);
    int usage(std::ostream & out, char const *name) const;

private:
    #define COMMAND_LINE_PARAMETER_PROTO(PARAM, COUNT, NAME, DESCR) \
        bool parse_##NAME(char const * const *argv);
    DECLARE_COMMAND_LINE_PARAMETERS_TABLE(COMMAND_LINE_PARAMETER_PROTO)
    #undef COMMAND_LINE_PARAMETER_PROTO

    #define COMMAND_LINE_ACTION_PROTO(NAME) \
        int do_##NAME(void);
    DECLARE_COMMAND_LINE_ACTIONS_TABLE(COMMAND_LINE_ACTION_PROTO)
    #undef COMMAND_LINE_ACTION_PROTO
    #define COMMAND_LINE_ACTION_PROTO(NAME) int Configuration::do_##NAME(void)

    int Assemble(CAssembler::assemble_mode);

private:
    command_t      command;
    unsigned       codesize;
    unsigned       rodatasize;
    unsigned       datasize;
    std::string    file_name;

    EmulatorConfiguration emulator_config;
};

} // namespace postrisc
