#pragma once

#include <cstdint>
#include <vector>
#include <sstream>

class ElfHeader {
public:
    enum class WordSize : u8 {
        b32 = 1,
        b64 = 2,
        WordCount = 3,
    };

    enum class Endian : u8 {
        little = 1,
        big = 2,
    };

    enum class Architecture : u16 {
        Undefined = 0,
        Sparc = 2,
        x86 = 3,
        MIPS = 8,
        PowerPC = 0x14,
        ARM = 0x28,
        IA64 = 0x32,
        x86_64 = 0x3E,
    };

    enum class Type : u16 {
        relocatable = 1,
        executable = 2,
        shared = 3,
        core = 4
    };

    WordSize word;
    Endian endian;
    u8 version;
    u8 abi;
    Type type;
    Architecture arch;
    uintptr_t program_entry_pos;
    uintptr_t program_header_table_pos;
    uintptr_t program_section_table_pos;
    u32 flags;
    u16 header_size;
    u16 program_header_table_entry_count;
    u16 program_header_table_entry_size;
    u16 section_header_table_entry_count;
    u16 section_header_table_entry_size;
    u16 section_header_name_index;
};

class ElfProgramHeader {
public:
    enum class Type : u32 {
        null = 0,
        load = 1,
        dynamic = 2,
        interpreted = 3,
        note = 4,
        shlib = 5,
        phdr = 6,
        tls = 7,
        loos = 0x60000000,
        hios = 0x6fffffff,
        loproc = 0x70000000,
        hiproc = 0x7fffffff,
    };

    enum Flags : u32 {
        executable = 1,
        writeable = 2,
        readable = 4
    };

    ElfProgramHeader()= default;
    ElfProgramHeader(Type type, u32 flags, uint64_t fileOffset, uint64_t memOffset,
                     uint64_t fileSize, uint64_t memSize, uint64_t alignment)
    : type(type), flags(flags), file_offset(fileOffset), mem_offset(memOffset),
      file_size(fileSize), mem_size(memSize), alignment(alignment)
    {}

    Type type;
    u32 flags;
    uint64_t file_offset;
    uint64_t mem_offset;
    uint64_t file_size;
    uint64_t mem_size;
    uint64_t alignment;
};

class ElfSectionHeader {
public:
    enum class Type : u32 {
        null = 0,
        progbits = 1,
        symtab = 2,
        strtab = 3,
        rela = 4,
        hash = 5,
        dynamic = 6,
        note = 7,
        nobits = 8,
        rel = 9,
        shlib = 0x0A,
        dynsym = 0x0B,
        init_array = 0x0E,
        fini_array = 0x0F,
        preinit_array  = 0x10,
        group = 0x11,
        symtab_shndx = 0x12,
        num = 0x13
    };

    enum Flags : uint64_t {
        write = 1,
        alloc = 2,
        execinstr = 3,
        merge = 4,
        strings = 5,
        info_link = 6,
        link_order = 0,
        os_nonconforming = 0,
        group = 0,
        tls = 0,
        maskos = 0,
        maskproc = 0,
        ordered = 0,
        exclude = 0
    };

    u32 name_strtab_offset;
    std::string name;
    Type type;
    uint64_t flags;
    uint64_t mem_offset;
    uint64_t file_offset;
    uint64_t file_size;
    u32 link;
    u32 info;
    uint64_t alignment;
    uint64_t entry_size;
};

class Elf {
public:
    ElfHeader header;
    std::vector<ElfProgramHeader> program_headers;
    std::vector<ElfSectionHeader> section_headers;
    std::string binary_data;
    std::string name;
};
