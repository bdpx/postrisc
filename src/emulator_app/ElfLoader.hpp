#pragma once

#if defined(__linux__)
#include <elf.h>
#else
#include "windows/elf.h"
#endif // __linux__

namespace postrisc {

class ELF64 {
public:
    using ELF_EHDR    = Elf64_Ehdr;
    using ELF_PHDR    = Elf64_Phdr;
    using ELF_SHDR    = Elf64_Shdr;
    using ELF_DYN     = Elf64_Dyn;
    using ELF_REL     = Elf64_Rel;
    using ELF_RELA    = Elf64_Rela;
    using ELF_SYM     = Elf64_Sym;
    using ELF_AUXV_T  = Elf64_auxv_t;

    static u32 get_r_sym(Elf64_Xword val) { return ELF64_R_SYM(val); }
    static u32 get_r_type(Elf64_Xword val) { return ELF64_R_TYPE(val); }

    static u8 get_st_bind(u8 val) { return ELF64_ST_BIND(val); }
    static u8 get_st_type(u8 val) { return ELF64_ST_TYPE(val); }
    static u8 get_st_visibility(u8 val) { return ELF64_ST_VISIBILITY(val); }
};

class ELF32 {
public:
    using ELF_EHDR    = Elf32_Ehdr;
    using ELF_PHDR    = Elf32_Phdr;
    using ELF_SHDR    = Elf32_Shdr;
    using ELF_DYN     = Elf32_Dyn;
    using ELF_REL     = Elf32_Rel;
    using ELF_RELA    = Elf32_Rela;
    using ELF_SYM     = Elf32_Sym;
    using ELF_AUXV_T  = Elf32_auxv_t;

    static u32 get_r_sym(Elf64_Word val) { return ELF32_R_SYM(val); }
    static u32 get_r_type(Elf64_Word val) { return ELF32_R_TYPE(val); }

    static u8 get_st_bind(u8 val) { return ELF32_ST_BIND(val); }
    static u8 get_st_type(u8 val) { return ELF32_ST_TYPE(val); }
    static u8 get_st_visibility(u8 val) { return ELF32_ST_VISIBILITY(val); }
};

class ElfInfo {
public:
    uintptr_t load_base;
    uintptr_t text_base;
    uintptr_t text_size;
    uintptr_t entry;
    uintptr_t dynamic;
    uintptr_t stack_top;
    uintptr_t stack_bottom;
    uintptr_t ph_base;
    u32  ph_num;
    u32  ph_size;
    u32  sh_num;
    u32  sh_size;
};

enum postrisc_reloc {
#define ELF_RELOC(NAME, VALUE) NAME = VALUE,
#include "postrisc_relocs.def"
#undef ELF_RELOC
};

char const *postrisc_reloc_2_str(postrisc_reloc reloc);

} // namespace postrisc
