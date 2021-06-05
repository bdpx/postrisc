#pragma once

#include <future>

#include "config/emulator_configuration.hpp"
#include "assembler/assembler.hpp"

namespace postrisc {

enum cmd_parameter_t {
};

#define DECLARE_COMMAND_LINE_PARAMETERS_TABLE(X) \
    X("assemble",            0, assemble,             "assemble to binary: <file.s >file.bin") \
    X("assemble-c",          0, assemble_c,           "switch to assemble for embedding in C/C++") \
    X("config",              1, config,               "import xml configuration file") \
    X("disasm",              0, disasm,               "disassemble binary file (*.bin): <file.bin > file.s") \
    X("dumpbin",             0, dumpbin,              "disassemble and dump binary: -dumpbin < file.bin > file.s") \
    X("execute",             1, execute,              "execute binary in emulator: file.bin > result.txt") \
    X("exeapp",              0, exeapp,               "execute app binary in emulator: file.bin > result.txt") \
    X("generate",            3, generate,             "generate testing binary: code rdata data > file.bin") \
    X("help",                0, help,                 "Display this information.") \
    X("html",                0, html,                 "generate help in html about ISA: > file.html") \
    X("llvm",                0, llvm,                 "write llvm tablegen file: >PostriscInstrEncoding.td") \
    X("dump-file",           1, dump_file,            "dump final state to file") \
    X("log-file",            1, log_file,             "set log file path") \
    X("export-definitions",  0, export_definitions,   "export asm known predefined constants") \
    X("scan",                0, scan,                 "scan and tokenize source: < file.s > file.txt") \
    X("scan-html",           0, scan_html,            "scan and html-markup source: < file.s > file.html") \
    X("version",             0, version,              "Display this program's version information.") \
    X("log-level",           1, log_level,            "set logging level.") \
    X("log-subsystem",       1, log_subsystem,        "set logging subsystem mask.") \

enum command_t {
    command_nothing,
#define COMMAND_LINE_ACTION_ENUM(PARAM, NUM, NAME, DESCR) command_##NAME,
    DECLARE_COMMAND_LINE_PARAMETERS_TABLE(COMMAND_LINE_ACTION_ENUM)
#undef COMMAND_LINE_ACTION_ENUM
};

class IDoomInterface;

class Configuration {
public:
    Configuration(void);
    ~Configuration(void);
    int Parse(int argc, char **argv);
    int run(int argc, char **argv, char **env);
    int usage(std::ostream& out) const;
    int do_run(IDoomInterface& doom, int argc, char **argv, char **env);

private:
    int do_version(void);
    int do_tokenize(void);
    int do_tokenize_html(void);
    int do_assemble(void);
    int do_disassemble(void);
    int do_dumpbin(void);
    int do_generate(void);
    int do_execute(void);
    int do_execute_app(IDoomInterface& doom, int argc, char**argv, char**env);
    int do_export_html(void);
    int do_export_llvm(void);
    int do_export_definitions(void);

private:
    command_t      command = command_nothing;
    unsigned       codesize = 0;
    unsigned       rodatasize = 0;
    unsigned       datasize = 0;
    std::string    file_name;

    std::future<int>            m_Task;
    CAssembler::assemble_mode   m_assemble_mode = CAssembler::assemble_mode_obj;

    EmulatorConfiguration emulator_config;
};

void xhtml_header(std::ostream& out, const char *title, const char *description, bool colormap = false);

} // namespace postrisc
