#include <iostream>
#include <iomanip>
#include <fstream>

#include "util/common.hpp"
#include "elf_util.hpp"
#include "elf_defs.h"

namespace postrisc { namespace elf {

int test(char const *filename, size_t memory_size, size_t module_offset)
{
#if 0
    elf_info64 elf;
    static std::vector<std::string> sections = elf.sections(fs);
    std::cout << "sections:\n";
    for (auto const& sym : sections) {
       std::cout << sym << std::endl;
    }

    std::cout << "symbols:\n";
    std::vector<std::string> symbols = elf.symbols(fs);
    for (auto const& sym : symbols) {
       std::cout << sym << std::endl;
    }
    std::vector<Elf_Phdr_template<u64>> ps = elf.program_sections(fs);
    DumpProgramHeaders<u64>(std::cout, ps.data(), ps.size());

    std::vector<u8> memory(memory_size, 0);
    load_library<u64>(image, memory, module_offset);
    std::ofstream fs("memory.dump", std::ios_base::out | std::ios_base::binary);
    fs.write((char const*)&memory[0], memory.size());
#endif
    return 0;
}

}} // namespace postrisc::elf

int test_main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "usage: prog file" << std::endl;
        return 1;
    }

    size_t memory_size = 1 * 1024 * 1024;
    size_t module_offset = 128 * 1024;
    return postrisc::elf::test(argv[1], memory_size, module_offset);
}
