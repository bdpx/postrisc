#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <fcntl.h>

#if defined(__linux__)
#include <sys/mman.h>        // mmap
#else
#include "windows/posix.h"
#endif // __linux__

#include <vector>
#include <algorithm>
#include <iostream>

#include <boost/align/aligned_alloc.hpp>

#include "util/fmt.hpp"
#include "util/common.hpp"
#include "util/logger.hpp"
#include "ElfLoader.hpp"
#include "TargetSingleApp.hpp"
#include "emulator/InstructionStatisticCollector.hpp"

#define ENSURE(COND, MSG)                          \
    do {                                           \
        if (!(COND)) {                             \
            LOG_ERROR(LOG_LOADER) << MSG;          \
            return -1;                             \
        }                                          \
    } while(0)

namespace postrisc {

/*
how file works:
if Elf32_Ehdr.e_type == ET_EXEC -> executable
else if Elf32_Ehdr.e_type == ET_DYN
    if DT_FLAGS_1 dynamic section entry is present
        if DF_1_PIE is set in DT_FLAGS_1: -> pie executable
        else -> shared object
    else
        if file is executable by user, group or others -> pie executable
        else -> shared object

Executable generation        ELF type  DT_FLAGS_1  DF_1_PIE  chdmod +x      file 5.36
---------------------------  --------  ----------  --------  -------------- --------------
gcc -fpie -pie               ET_DYN    y           y         y              pie executable
gcc -fno-pie -no-pie         ET_EXEC   n           n         y              executable
gcc -shared                  ET_DYN    n           n         y              pie executable
gcc -shared                  ET_DYN    n           n         n              shared object
ld                           ET_EXEC   n           n         y              executable
ld -pie --dynamic-linker     ET_DYN    y           y         y              pie executable
ld -pie --no-dynamic-linker  ET_DYN    y           y         y              pie executable
*/

#if 0

using ADDR = uintptr_t;

static ADDR text_base, text_end;
static ADDR entry_ip;

char *sim_root = NULL;
size_t sim_root_len = 0;
ADDR dataStart = 0;        /* used to set initial location of data window */
BOOL alwaysNat = NO;

/* Insert symbols from the ELF section scn into the symbol table
 *   the symbol's address must be in the range low..high
 *   the st_name field must be nonzero
 *   the binding part of the st_info field must be STB_GLOBAL or STB_LOCAL
 *   the type part of the st_info field must be STT_FUNC or STT_OBJECT
 *   the st_shndx field must not be SHN_UNDEF?
 *   the address in the ELF section will be increased by reloc_addr
 */
static void elf64_symbols(Elf *elfptr, Elf_Scn *scn, Elf64_Word strndx,
                          ADDR reloc_addr, ADDR low, ADDR high)
{
    Elf_Data *data;
    Elf_Scn *strscn;
    Elf64_Sym *syms;
    Elf64_Half shndx;
    int i, nsyms;

    /* XXX - will this "fall out" naturally? */
    if (strndx == SHN_UNDEF)        /* No symbols found */
        return;
    data = elf_getdata(scn, 0);
    syms = data->d_buf;
    /* Use sh_entsize instead of sizeof? */
    nsyms = data->d_size / sizeof (Elf64_Sym);
    strscn = elf_getscn(elfptr, strndx);
    data = elf_getdata(strscn, 0);
    for (i = 1; i < nsyms; i++) {
        if (!syms[i].st_name)
            continue;
        char *name = (char *)data->d_buf + syms[i].st_name;
        if ((shndx = syms[i].st_shndx) == SHN_UNDEF)
            continue;
        Elf64_Addr value = syms[i].st_value;
        if (value < low || value > high)
            continue;
        if (reloc_addr) {
/*            value -= elf64_getshdr(elf_getscn(elfptr, shndx))->sh_addr;*/
            value -= low;
            value += reloc_addr;
        }
        /* XXX - the cast below is used to silence the 703 Migration warning;
           it might be better to place this cast in the definition of the
           ELF macro or to fix the compiler to not complain here */
        unsigned bind = ELF64_ST_BIND((unsigned)syms[i].st_info);
        unsigned type = ELF64_ST_TYPE(syms[i].st_info);
        /* XXX - Should dataStart be based on something else like address of
                 lowest data segment? */
        if (!strcmp(name, "_data_start") || !strcmp(name, "__data_start")) {
            dataStart = value;
        }
        /* XXX - what to do with STB_WEAK symbols? */
        /* XXX - hack to avoid inserting $DEBUG_xxx symbols which are
                 currently of type OBJT */
        if (strlen(name) > 12 && !strncmp(name, "$DEBUG_", 7))
            continue;

        if (bind != STB_GLOBAL && bind != STB_LOCAL)
            continue;

        if (type != STT_FUNC && type != STT_OBJECT && type != STT_NOTYPE)
            continue;
        // symInsert(name, value, 0);
    }
}

static void elf32_symbols(Elf *elfptr, Elf_Scn *scn, Elf32_Word strndx,
                          ADDR reloc_addr, ADDR low, ADDR high)
{
    /* XXX - will this "fall out" naturally? */
    if (strndx == SHN_UNDEF)        /* No symbols found */
        return;
    Elf_Data *data = elf_getdata(scn, 0);
    Elf32_Sym *syms = data->d_buf;
    /* Use sh_entsize instead of sizeof? */
    int nsyms = data->d_size / sizeof (Elf32_Sym);
    Elf_Scn *strscn = elf_getscn(elfptr, strndx);
    data = elf_getdata(strscn, 0);
    for (int i = 1; i < nsyms; i++) {
        if (!syms[i].st_name)
            continue;
        char *name = (char *)data->d_buf + syms[i].st_name;
        Elf32_Half shndx = syms[i].st_shndx;
        if (shndx == SHN_UNDEF)
            continue;
        unsigned type = ELF32_ST_TYPE(syms[i].st_info);
        if (type == STT_SECTION)
            continue;
        ADDR value = syms[i].st_value;
        if (value < low || value > high)
            continue;
        if (reloc_addr) {
/*            value -= elf32_getshdr(elf_getscn(elfptr, shndx))->sh_addr;*/
            value -= low;
            value += reloc_addr;
        }
        /* XXX - the cast below is used to silence the 703 Migration warning;
           it might be better to place this cast in the definition of the
           ELF macro or to fix the compiler to not complain here */
        unsigned bind = ELF32_ST_BIND((unsigned)syms[i].st_info);
        /* XXX - Should dataStart be based on something else like address of
                 lowest data segment? */
        if (!strcmp(name, "_data_start") || !strcmp(name, "__data_start")) {
            dataStart = value;
        }
        /* XXX - what to do with STB_WEAK symbols? */
        /* XXX - hack to avoid inserting $DEBUG_xxx symbols which are
                 currently of type OBJT */
        if (strlen(name) > 12 && !strncmp(name, "$DEBUG_", 7))
            continue;

        if (bind != STB_GLOBAL && bind != STB_LOCAL)
            continue;

        if (type != STT_FUNC && type != STT_OBJECT)
            continue;
        symInsert(name, value, 0);
    }
}

static void elf64_slurp_all_symbols (Elf *elfptr, Elf64_Ehdr *ehdr,
             Elf64_Phdr *phdr, ADDR load_base)
{
    for (Elf_Scn *scn = NULL; (scn = elf_nextscn(elfptr, scn)) != NULL;) {
        Elf64_Shdr *shdr = elf64_getshdr(scn);
        if (shdr->sh_type == SHT_SYMTAB || shdr->sh_type == SHT_DYNSYM) {
            for (int i = 0; i < ehdr->e_phnum; ++i) {
                if (phdr[i].p_type != PT_LOAD)
                    continue;
                elf64_symbols(elfptr, scn, shdr->sh_link, load_base,
                              phdr[i].p_vaddr,
                              phdr[i].p_vaddr + phdr[i].p_memsz - 1);
            }
        }
    }
}

void mmapSyms(int fd, ADDR start, ADDR len, ADDR offset)
{
    Elf *elfptr;
    char *ident;
    ADDR end = start + len - 1;

    if (class == ELFCLASS64) {
        Elf64_Ehdr *ehdr;
        Elf64_Phdr *phdr;
        ADDR pstart = 0, pend = 0;
        unsigned i;

        symDeleteAddr(start, end);
        for (i = 0; i < ehdr->e_phnum; i++) {
            if (phdr[i].p_type == PT_IA_64_UNWIND) {
                addLM5("shlib", phdr, ehdr->e_phnum, start, 0);
                break;
            }
        }
        for (i = 0; i < ehdr->e_phnum; i++) {
            if (phdr[i].p_type != PT_LOAD)
                continue;
            ADDR fstart = phdr[i].p_offset;
            ADDR fend = fstart + phdr[i].p_filesz;
            if (offset >= fstart && offset < fend) {
                pstart = phdr[i].p_vaddr + offset - fstart;
                pend = pstart + len - 1;
                break;
            }
        }
        elf64_slurp_all_symbols(elfptr, ehdr, phdr, start);
    } else {        /* ELFCLASS32 */
    }
}

void munmapSyms(ADDR low, ADDR high)
{
    symDeleteAddr(low, high);
}

BOOL elfSymLoad(const char *file_name)
{
    Elf *elfptr;
    int fd;
    unsigned char class;
    ADDR unwind_base = 0;

    /* XXX - should 2nd arg be stderr?  What if not ELF? */
    elfptr = elfOpen(file_name, NULL, &fd, EO_SYMS, &class);
    if (elfptr == NULL)
        return NO;

    Elf64_Ehdr *ehdr;
    Elf64_Phdr *phdr;
    for (unsigned i = 0; i < ehdr->e_phnum; i++)
        if (phdr[i].p_type == PT_IA_64_UNWIND)
            unwind_base = phdr[i].p_vaddr;
        else if (phdr[i].p_type == PT_LOAD && phdr[i].p_flags & PF_X) {
            text_base = phdr[i].p_vaddr;
            text_end = text_base + phdr[i].p_memsz - 1;
        }
    addLM5(file_name, phdr, ehdr->e_phnum, 0, 1);
    elf64_slurp_all_symbols(elfptr, ehdr, phdr, 0);
    return YES;
}

BOOL elfLoad(const char *file_name, int s_argc, char *s_argv[])
{
    struct os_proc proc = {};
    int fd;
    unsigned char elf_class;
    ADDR unwind_base = 0, end_addr = 0;
    REG rsc;
    BOOL abi;

    DEBUG("%s('%s')\n", __func__, file_name);
    Elf *elfptr = elfOpen(file_name, stderr, &fd, EO_EXEC, &elf_class);
    if (!elfptr) return NO;

    abi = unixABI;

    /* XXX - for now so that memory pages are auto-allocated */
    setABI(YES);

    proc.has_rtld = 0;
    proc.aout_base = ~0ULL;
    proc.aout_phdr = ~0ULL;

    Elf64_Ehdr *ehdr;
    Elf64_Phdr *phdr;
    unsigned i;
    ADDR pstart, pend;
    ADDR bias = 0;
    if (ehdr->e_type == ET_DYN) {
        // PIE: a base that looks like ld.so but doesn't conflict with it. TODO: make it random
        bias = 0x2000000100000000ULL;
        DEBUG("bias: %#llx\n", bias);
    }

    entry_ip = bias + ehdr->e_entry;
    DEBUG("entry: %#lx\n", ehdr->e_entry);
    proc.ehdr_flags = ehdr->e_flags;

    DEBUG("program headers: %u\n", ehdr->e_phnum);
    for (i = 0; i < ehdr->e_phnum; i++)
        switch (phdr[i].p_type) {
        case PT_PHDR:
            /* XXX - check alignment?  Must be 4K aligned? */
            /* TODO: account for ELF bias? */
            proc.aout_phdr = bias + phdr[i].p_vaddr;
            break;
        case PT_IA_64_UNWIND:
            /* TODO: account for ELF bias? */
            addLM5(file_name, phdr, ehdr->e_phnum, bias, 1);
            unwind_base = bias + phdr[i].p_vaddr;
            /*unwind_end = bias + phdr[i].p_vaddr + phdr[i].p_filesz;*/
            DEBUG("PT_IA_64_UNWIND: %#llx\n", unwind_base);
            break;
        case PT_INTERP:
            break;
        case PT_LOAD:
            pstart = bias + phdr[i].p_vaddr;
            pend = pstart + phdr[i].p_memsz - 1;
            DEBUG("PT_LOAD: pstart: %#llx - %#llx\n", pstart, pend);
            if (!elf64SegmentLoad(&phdr[i], pstart, fd, file_name)) {
                elfClose(elfptr, fd);
                return NO;
            }
            if (phdr[i].p_flags & PF_X) {
                // XXX - shouldn't assume aout_base == text_base? especially if aout_base needs swapping?
                text_base = proc.aout_base = pstart;
                text_end = pend;
                DEBUG("*** PT_LOAD: text_base: %#llx - %#llx\n", text_base, text_end);
            } else
                end_addr = pend + 1;
        }
    elf64_slurp_all_symbols(elfptr, ehdr, phdr, bias);
    proc.phdr_count = ehdr->e_phnum;
    proc.phdr_addr = proc.aout_base + ehdr->e_phoff;
    proc.proc_entry = bias + ehdr->e_entry;

    if (proc.aout_phdr == ~0ULL) {
        (void)fprintf(stderr, "%s - missing PT_PHDR segment\n", file_name);
        elfClose(elfptr, fd);
        return NO;
    }

    if (proc.aout_base == ~0ULL) {
        (void)fprintf(stderr, "%s - missing text segment\n", file_name);
        elfClose(elfptr, fd);
        return NO;
    }

    return YES;
}

#endif // 0

/*==================================================================================*/
// ELF sections
/*==================================================================================*/
template<typename ELF_WORD>
static char const *shdr_type_name(ELF_WORD type)
{
    switch (type) {
#define X(NAME) case SHT_##NAME: return #NAME;
    X(NULL) X(PROGBITS) X(SYMTAB) X(STRTAB)
    X(RELA) X(HASH)
    X(DYNAMIC) X(NOTE) X(NOBITS) X(REL)
    X(SHLIB) X(DYNSYM)
    X(INIT_ARRAY) X(FINI_ARRAY) X(PREINIT_ARRAY)
    X(GROUP) X(SYMTAB_SHNDX)

// Ubuntu has no RELR defs
#if !defined(SHT_RELR)
    #define SHT_RELR 19
#endif
    X(RELR)

    X(GNU_ATTRIBUTES) X(GNU_HASH) X(GNU_LIBLIST)
    X(CHECKSUM)
    X(SUNW_move) X(SUNW_COMDAT) X(SUNW_syminfo)
    X(GNU_verdef) X(GNU_verneed) X(GNU_versym)
#undef X
    }
    return nullptr;
}

template<typename SH_FLAGS>
static void parse_sh_flags(SH_FLAGS flags, std::ostream& out)
{
#define X(NAME) if (flags & SHF_##NAME) { flags ^= SHF_##NAME; out << " " #NAME; }
    X(WRITE)            /* Writable */
    X(ALLOC)            /* Occupies memory during execution */
    X(EXECINSTR)        /* Executable */
    X(MERGE)            /* Might be merged */
    X(STRINGS)          /* Contains nul-terminated strings */
    X(INFO_LINK)        /* `sh_info' contains SHT index */
    X(LINK_ORDER)       /* Preserve order after combining */
    X(OS_NONCONFORMING) /* Non-standard OS specific handling required */
    X(GROUP)            /* Section is member of a group.  */
    X(TLS)              /* Section hold thread-local data.  */
    X(COMPRESSED)       /* Section with compressed data. */
    X(GNU_RETAIN)       /* Not to be GCed by linker.  */
    X(ORDERED)          /* Special ordering requirement (Solaris).  */
    X(EXCLUDE)          /* Section is excluded unless referenced or allocated (Solaris).*/
#undef X
    if (flags) {
        out << " + 0x" << std::hex << flags << std::dec;
    }
}

template<typename ELF>
static void dump_shdrs(const std::span<const typename ELF::ELF_SHDR> sections, std::ostream& out, const std::span<const char> shstrtab)
{
    out <<
        "<table>"
        "<caption>Section headers</caption>\n"
        "<thead><tr>"
        "<th>id</th>"
        "<th>offset</th>"
        "<th>addr</th>"
        "<th>size</th>"
        "<th>align</th>"
        "<th>link</th>"
        "<th>info</th>"
        "<th>entsize</th>"
        "<th>type</th>"
        "<th>name</th>"
        "<th>flags</th>"
        "</tr></thead>"
        "<tbody>\n";

    for (size_t i = 0; i < sections.size(); i++) {
        const typename ELF::ELF_SHDR& shdr = sections[i];
        out << "<tr><td>" << i
            << "</td><td>" << shdr.sh_offset      // Section file offset
            << "</td><td>" << shdr.sh_addr        // Section virtual addr at execution
            << "</td><td>" << shdr.sh_size        // Section size in bytes
            << "</td><td>" << shdr.sh_addralign   // Section alignment
            << "</td><td>" << shdr.sh_link        // Link to another section
            << "</td><td>" << shdr.sh_info        // Additional section information
            << "</td><td>" << shdr.sh_entsize;    // Entry size if section holds table

        // Segment type
        out << "</td><td>";
        char const *type_name = shdr_type_name(shdr.sh_type);
        if (type_name)
            out << type_name;
        else
            out << shdr.sh_type;

        // Section name (string tbl index)
        out << "</td><td>" << &shstrtab[shdr.sh_name]
            << "</td><td>";
        // out << " " << shdr.sh_name;

        parse_sh_flags(shdr.sh_flags, out); // Section flags

        out << "</td></tr>\n";
    }
    out << "</tbody></table>\n";
}


template<typename ELF_WORD>
static char const *phdr_type_name(ELF_WORD type)
{
    switch (type) {
    case PT_NULL:          return "null";         // Program header table entry unused
    case PT_LOAD:          return "load";         // Loadable program segment
    case PT_DYNAMIC:       return "dynamic";      // Dynamic linking information
    case PT_INTERP:        return "interp";       // Program interpreter
    case PT_NOTE:          return "note";         // Auxiliary information
    case PT_SHLIB:         return "shlib";        // Reserved
    case PT_PHDR:          return "phdr";         // Entry for header table itself
    case PT_TLS:           return "tls";          // Thread-local storage segment
    case PT_GNU_EH_FRAME:  return "eh_frame";     // GCC .eh_frame_hdr segment
    case PT_GNU_STACK:     return "stack";        // Indicates stack executability
    case PT_GNU_RELRO:     return "relro";        // Read-only after relocation
    case PT_GNU_PROPERTY:  return "property";     // GNU property
    case PT_SUNWBSS:       return "sunwbss";      // Sun Specific segment
    case PT_SUNWSTACK:     return "sunwstack";    // Stack segment
    }
    return nullptr;
}

static void dump_protection_flags(int flags, std::ostream& out)
{
    out << ((flags & PF_R) ? 'r' : '-');
    out << ((flags & PF_W) ? 'w' : '-');
    out << ((flags & PF_X) ? 'x' : '-');
    flags &= ~(PF_R | PF_W | PF_X);
    if (flags)
        out << ',' << flags;
}

template<typename ELF>
static void
dump_phdrs(
    const std::span<const typename ELF::ELF_PHDR> program_headers,
    const std::span<const typename ELF::ELF_SHDR> sections,
    const std::span<const char> shstrtab,
    std::ostream& out)
{
    out << "<table>"
        "<caption>Program headers</caption>"
        "<thead><tr>"
        "<th>id</th>"
        "<th>offset</th>"
        "<th>vaddr</th>"
        "<th>paddr</th>"
        "<th>filesz</th>"
        "<th>memsz</th>"
        "<th>align</th>"
        "<th>rwx</th>"
        "<th>type</th>"
        "<th>sections</th>"
        "</tr></thead>"
        "<tbody>\n";

    for (size_t i = 0; i < program_headers.size(); i++) {
        const typename ELF::ELF_PHDR& phdr = program_headers[i];
        out << "<tr><td>" << i
            << "</td><td>" << phdr.p_offset  // Segment file offset
            << "</td><td>" << phdr.p_vaddr   // Segment virtual address
            << "</td><td>" << phdr.p_paddr   // Segment physical address
            << "</td><td>" << phdr.p_filesz  // Segment size in file
            << "</td><td>" << phdr.p_memsz   // Segment size in memory
            << "</td><td>" << phdr.p_align   // Segment alignment
            << "</td><td>";
        // Segment flags
        dump_protection_flags(phdr.p_flags, out);

        // Segment type
        char const *type_name = phdr_type_name(phdr.p_type);
        if (type_name)
            out << "</td><td>" << type_name;
        else
            out << "</td><td>" << phdr.p_type;

        out << "</td><td>";
        bool first = true;
        for (const auto& section : sections) {
            if (phdr.p_vaddr <= section.sh_addr && section.sh_size > 0 && section.sh_addr != 0 &&
                section.sh_addr + section.sh_size <= phdr.p_vaddr + phdr.p_memsz) {
                if (!first) out << ' ';
                out << &shstrtab[section.sh_name];
                first = false;
            }
        }
        out << "</td></tr>\n";
    }
    out << "</tbody></table>\n";
}

static char const *get_dynamic_type_name(unsigned val)
{
    switch (val) {
#define X(NAME) case DT_##NAME: return #NAME;
    X(NULL) X(NEEDED) X(PLTRELSZ) X(PLTGOT) X(HASH) X(STRTAB) X(SYMTAB)
    X(RELA) X(RELASZ) X(RELAENT)
    X(STRSZ) X(SYMENT) X(INIT) X(FINI)
    X(SONAME) X(RPATH) X(SYMBOLIC) X(REL) X(RELSZ) X(RELENT) X(PLTREL)
    X(DEBUG) X(TEXTREL) X(JMPREL)
    X(BIND_NOW) X(INIT_ARRAY) X(FINI_ARRAY) X(INIT_ARRAYSZ) X(FINI_ARRAYSZ)
    X(RUNPATH) X(FLAGS) X(PREINIT_ARRAY) X(PREINIT_ARRAYSZ)
    X(SYMTAB_SHNDX)

// Ubuntu has no DT_RELR-related defs
#if !defined(DT_RELRSZ)
    #define DT_RELRSZ 35
#endif
#if !defined(DT_RELR)
    #define DT_RELR 36
#endif
#if !defined(DT_RELRENT)
    #define DT_RELRENT 37
#endif
    X(RELRSZ) X(RELR) X(RELRENT)

    X(VALRNGLO)
    X(GNU_PRELINKED) X(GNU_CONFLICTSZ) X(GNU_LIBLISTSZ)
    X(CHECKSUM) X(PLTPADSZ) X(MOVEENT) X(MOVESZ)
    X(FEATURE_1) X(POSFLAG_1) X(SYMINSZ) X(SYMINENT)
    X(ADDRRNGLO)
    X(GNU_HASH) X(TLSDESC_PLT) X(TLSDESC_GOT) X(GNU_CONFLICT) X(GNU_LIBLIST)
    X(CONFIG) X(DEPAUDIT) X(AUDIT) X(PLTPAD) X(MOVETAB) X(SYMINFO)
    X(VERSYM) X(RELACOUNT) X(RELCOUNT) X(FLAGS_1)
    X(VERDEF) X(VERDEFNUM) X(VERNEED) X(VERNEEDNUM)
    X(AUXILIARY) X(FILTER)
#undef X
    }
    return nullptr;
}

static void dump_dynamic_df_1(unsigned val, std::ostream& out)
{
#define X(FLAG) if (val & (DF_1_##FLAG)) { out <<  ' ' << #FLAG; val ^= (DF_1_##FLAG); }
    X(NOW)          /* Set RTLD_NOW for this object.  */
    X(GLOBAL)       /* Set RTLD_GLOBAL for this object.  */
    X(GROUP)        /* Set RTLD_GROUP for this object.  */
    X(NODELETE)     /* Set RTLD_NODELETE for this object.*/
    X(LOADFLTR)     /* Trigger filtee loading at runtime.*/
    X(INITFIRST)    /* Set RTLD_INITFIRST for this object*/
    X(NOOPEN)       /* Set RTLD_NOOPEN for this object.  */
    X(ORIGIN)       /* $ORIGIN must be handled.  */
    X(DIRECT)       /* Direct binding enabled.  */
    X(TRANS)
    X(INTERPOSE)    /* Object is used to interpose.  */
    X(NODEFLIB)     /* Ignore default lib search path.  */
    X(NODUMP)       /* Object can't be dldump'ed.  */
    X(CONFALT)      /* Configuration alternative created.*/
    X(ENDFILTEE)    /* Filtee terminates filters search. */
    X(DISPRELDNE)   /* Disp reloc applied at build time. */
    X(DISPRELPND)   /* Disp reloc applied at run-time.  */
    X(NODIRECT)     /* Object has no-direct binding. */
    X(IGNMULDEF)
    X(NOKSYMS)
    X(NOHDR)
    X(EDITED)       /* Object is modified after built.  */
    X(NORELOC)
    X(SYMINTPOSE)   /* Object has individual interposers.  */
    X(GLOBAUDIT)    /* Global auditing required.  */
    X(SINGLETON)    /* Singleton symbols are used.  */
    X(STUB)
    X(PIE)
    X(KMOD)
    X(WEAKFILTER)
    X(NOCOMMON)
#undef X
    if (val) {
        out << ' ' << fmt::hex(val);
    }
}

template<typename ELF>
int
TargetSingleApp::parse_code(const typename ELF::ELF_SHDR& section, const std::span<const std::byte> image, Decoder& tempDecoder)
{
    Bundle const *bundle = reinterpret_cast<Bundle const *>(&image[section.sh_addr]);
    LOG_DEBUG(LOG_LOADER) << "CODE=" << fmt::hex(std::bit_cast<uintptr_t>(bundle));

    //WITH_DEBUG(LOG_LOADER) {
        InstructionStatisticCollector collector;
        DecoderContext decoderContext(disasm_mode_std | disasm_mode_names);
        dbgs() << "<div><pre>";
        dbgs() << "";
        for (size_t k = 0; k < section.sh_size / sizeof(Bundle); k++) {
            u64 const ip = section.sh_addr + k * sizeof(Bundle); // reinterpret_cast<uintptr_t>(&bundle[k]);
            tempDecoder.DisassembleBundle(bundle[k], ip, dbgs(), decoderContext);
            switch (bundle[k].GetTemplate()) {
                case EBundleTemplate::bundle_sss:
                    collector.count(Decoder::Decode(bundle[k].s0()));
                    collector.count(Decoder::Decode(bundle[k].s1()));
                    collector.count(Decoder::Decode(bundle[k].s2()));
                    break;
                case EBundleTemplate::bundle_sll:
                    collector.count(Decoder::Decode(bundle[k].s0()));
                    collector.count(Decoder::Decode(bundle[k].l1()));
                    break;
                case EBundleTemplate::bundle_lls:
                    collector.count(Decoder::Decode(bundle[k].l0()));
                    collector.count(Decoder::Decode(bundle[k].s2()));
                    break;
                case EBundleTemplate::bundle_lll:
                    LOG_ERROR(LOG_LOADER) << "invalid bundle template";
                    return -1;
            }
        }
        dbgs() << "</pre></div>\n";
        DumpFormatter formatter(get_decoder(), dbgs(), true);
        collector.dump(formatter, "Static instruction frequencies");
    //}
    return 0;
}

template<typename ELF>
int
TargetSingleApp::parse_dynamic(const typename ELF::ELF_SHDR& section,
    const std::span<const std::byte> original_file, const std::span<const std::byte> image, ElfInfo& elf_info)
{
    ENSURE(section.sh_entsize == sizeof(typename ELF::ELF_DYN), "unknown dynamic entry size " << section.sh_entsize);
    ENSURE(0 == (section.sh_size % sizeof(typename ELF::ELF_DYN)), "partial dynamic data");
    ENSURE(section.sh_offset + section.sh_size <= original_file.size(), "can't place dynamic");

    ENSURE(section.sh_addr != 0, "SHT_DYNAMIC section is found but it is not loadable");

    elf_info.dynamic = std::bit_cast<uintptr_t>(&image[section.sh_addr]);

    dbgs() << "<table>"
        "<caption>_DYNAMIC offset=" << section.sh_addr << "</caption>\n"
        "<thead><tr>"
        "<th>tag</th>"
        "<th>value</th>"
        "<th>description</th>"
        "</tr></thead>"
        "<tbody>\n";

    typename ELF::ELF_DYN const *dynamic = reinterpret_cast<typename ELF::ELF_DYN const *>(&original_file[section.sh_offset]);

    for (size_t k = 0; k < section.sh_size / sizeof(typename ELF::ELF_DYN); k++) {
        char const *name = get_dynamic_type_name(dynamic[k].d_tag);
        dbgs() << "<tr><td>";
        if (name)
            dbgs() << name;
        else
            dbgs() << dynamic[k].d_tag;

        dbgs() << "</td><td>" << dynamic[k].d_un.d_val << "</td><td>";
        switch (dynamic[k].d_tag) {
        case DT_FLAGS_1:
            dump_dynamic_df_1(dynamic[k].d_un.d_val, dbgs());
            break;
        }
        dbgs() << "</td></tr>\n";
    }
    dbgs() << "</tbody></table>\n";
    return 0;
}

/*==================================================================================*/
// ELF relocations
/*==================================================================================*/
char const *postrisc_reloc_2_str(postrisc_reloc reloc)
{
    switch (reloc) {
#define ELF_RELOC(NAME, VALUE) case NAME: return #NAME;
#include "postrisc_relocs.def"
#undef ELF_RELOC
    }
    return nullptr;
}

template<typename ELF>
int
TargetSingleApp::parse_rel(const typename ELF::ELF_SHDR& section, const std::span<const std::byte> original_file, const std::span<std::byte> image)
{
    size_t const nrel = section.sh_size / sizeof(typename ELF::ELF_REL);
    dbgs() << "\nREL nrel=" << nrel << fmt::endl;

    ENSURE(section.sh_entsize == sizeof(typename ELF::ELF_REL), "unknown rel entry size " << section.sh_entsize);
    ENSURE(0 == (section.sh_size % sizeof(typename ELF::ELF_REL)), "partial rel data");
    ENSURE(section.sh_offset + section.sh_size <= original_file.size(), "can't place rel");

    typename ELF::ELF_REL const *rel =
        reinterpret_cast<typename ELF::ELF_REL const *>(&original_file[section.sh_offset]);

    for (size_t k = 0; k < nrel; k++) {
        auto const rtype = ELF::get_r_type(rel[k].r_info);
        uintptr_t& value = *reinterpret_cast<uintptr_t*>(&image[rel[k].r_offset]);
        WITH_DEBUG(LOG_DUMP) {
            dbgs() << "offset=0x" << fmt::hex(rel[k].r_offset)
                << " info=" << rtype;
            char const *reloc_name = postrisc_reloc_2_str((postrisc_reloc)rtype);
            if (reloc_name)
                dbgs()<< " " << reloc_name;
            if (value != 0)
                dbgs() << " value=0x" << fmt::hex(value);
            dbgs() << std::endl;
        }
        switch(rtype) {
        case R_POSTRISC_RELATIVE:
            value += std::bit_cast<uintptr_t>(image.data());
            break;
        // FIXME: add another relocation types
        default:
             LOG_ERROR(LOG_LOADER) << "unexpected relocation type " << rtype;
             return -1;
        }
    }
    return 0;
}

template<typename ELF>
int
TargetSingleApp::parse_rela(const typename ELF::ELF_SHDR& section, const std::span<const std::byte> original_file, const std::span<std::byte> image)
{
    size_t const nrel = section.sh_size / sizeof(typename ELF::ELF_RELA);

    ENSURE(section.sh_entsize == sizeof(typename ELF::ELF_RELA), "unknown rela entry size " << section.sh_entsize);
    ENSURE(0 == (section.sh_size % sizeof(typename ELF::ELF_RELA)), "partial rela data");
    ENSURE(section.sh_offset + section.sh_size <= original_file.size(), "can't place rela");
    typename ELF::ELF_RELA const *rel = reinterpret_cast<typename ELF::ELF_RELA const *>(&original_file[section.sh_offset]);

    dbgs() <<
        "<table>"
        "<caption>RELA: " << nrel << " relocs</caption>"
        "<thead><tr>"
        "<th>offset</th>"
        "<th>addend</th>"
        "<th>value</th>"
        "<th>info</th>"
        "</tr></thead><tbody>\n";

    for (size_t k = 0; k < nrel; k++) {
        intptr_t& value = *reinterpret_cast<intptr_t*>(&image[rel[k].r_offset]);
        intptr_t const old_value = value;
        value = reinterpret_cast<intptr_t>(&image[rel[k].r_addend]);
        dbgs() << "<tr><td>" << rel[k].r_offset
               << "</td><td>" << rel[k].r_addend
               << "</td><td>" << old_value
               << "</td><td>" << ELF::get_r_type(rel[k].r_info)
               << "</td></tr>\n";
    }
    dbgs() << "</tbody></table>\n\n";
    return 0;
}

/*==================================================================================*/
// ELF symbols
/*==================================================================================*/

static char const *section_name_index(u16 index)
{
  switch (index) {
#define X(NAME) case SHN_##NAME: return #NAME;
    X(UNDEF)/* Undefined section */
    X(LORESERVE) /* Start of reserved indices */
    X(AFTER) /* Order section after all others (Solaris).  */
    X(HIPROC) /* End of processor-specific */
    X(LOOS) /* Start of OS-specific */
    X(HIOS) /* End of OS-specific */
    X(ABS) /* Associated symbol is absolute */
    X(COMMON) /* Associated symbol is common */
    X(XINDEX) /* Index is in extra table.  */
#undef X
    }
    return nullptr;
}

// Legal values for ST_BIND subfield of st_info (symbol type)
static char const *symbol_bind_2_str(u8 index)
{
    switch (index) {
        case STB_LOCAL:      return "local";   // Local symbol
        case STB_GLOBAL:     return "global";  // Global symbol
        case STB_WEAK:       return "weak";    // Weak symbol
        case STB_GNU_UNIQUE: return "unique";  // Unique symbol
    }
    return "unknown";
}

// Legal values for ST_TYPE subfield of st_info (symbol type)
static char const *symbol_type_2_str(u8 index)
{
    switch (index) {
        case STT_NOTYPE:     return "notype";  // Symbol type is unspecified
        case STT_OBJECT:     return "object";  // Symbol is a data object
        case STT_FUNC:       return "func";    // Symbol is a code object
        case STT_SECTION:    return "section"; // Symbol associated with a section
        case STT_FILE:       return "file";    // Symbol's name is file name
        case STT_COMMON:     return "common";  // Symbol is a common data object
        case STT_TLS:        return "tls";     // Symbol is thread-local data object
        case STT_GNU_IFUNC:  return "ifunc";   // Symbol is indirect code object
    }
    return "unknown";
}

/* Symbol visibility specification encoded in the st_other field.  */
static char const *symbol_visibility_2_str(u8 index)
{
    switch (index) {
        case STV_DEFAULT:   return "default";    // Default symbol visibility rules
        case STV_INTERNAL:  return "internal";   // Processor specific hidden class
        case STV_HIDDEN:    return "hidden";     // Sym unavailable in other modules
        case STV_PROTECTED: return "protected";  // Not preemptible, not exported
    }
    return "unknown";
}

template<typename ELF>
class symtab_comparator {
public:
    bool operator()(const typename ELF::ELF_SYM& a, const typename ELF::ELF_SYM& b) const { return a.st_value < b.st_value; }
};

template<typename ELF>
int
TargetSingleApp::parse_symtab(const std::span<const typename ELF::ELF_SHDR> shdr,
    const typename ELF::ELF_SHDR& section,
    const typename ELF::ELF_SHDR& str_section,
    const std::span<const char> shstrtab,
    const std::span<const std::byte> original_file, const std::span<const std::byte> image,
    Decoder& tempDecoder)
{
    size_t const nsyms = section.sh_size / sizeof(typename ELF::ELF_SYM);

    if (nsyms < 1)
        return -1;

    if (nsyms <= 1)
        return 0;

    ENSURE(section.sh_entsize == sizeof(typename ELF::ELF_SYM), "unknown symtab entry size " << section.sh_entsize);
    ENSURE(0 == (section.sh_size % sizeof(typename ELF::ELF_SYM)), "partial symtab data");
    ENSURE(section.sh_offset + section.sh_size <= original_file.size(), "can't place symtab");

    typename ELF::ELF_SYM const *symtab = reinterpret_cast<typename ELF::ELF_SYM const *>(&original_file[section.sh_offset]);
    char const *strtab = reinterpret_cast<char const *>(&original_file[str_section.sh_offset]);

    std::vector<typename ELF::ELF_SYM> syms(nsyms - 1);
    memcpy(syms.data(), symtab + 1, (nsyms - 1) * sizeof(typename ELF::ELF_SYM));

    std::sort(syms.begin(), syms.end(), symtab_comparator<ELF>());

    dbgs() <<
        "<table><caption>Symtab: " << syms.size() << " symbols</caption>"
        "<thead><tr>"
        "<th>value</th>"
        "<th>size</th>"
        "<th>bind</th>"
        "<th>type</th>"
        "<th>visiblty</th>"
        "<th>section</th>"
        "<th>name</th>"
        "</tr>"
        "</thead><tbody>\n";

    for (const auto& sym : syms) {
        // Symbol name (string tbl index)
        ENSURE(sym.st_name < str_section.sh_size, "invalid sym.st_name=" << sym.st_name);

        dbgs() << "<tr><td>" << sym.st_value
              << "</td><td>" << sym.st_size;

        u8 const st_bind = ELF::get_st_bind(sym.st_info);
        char const *bind_name = symbol_bind_2_str(st_bind);
        if (bind_name)
            dbgs() << "</td><td>" << bind_name;
        else
            dbgs() << "</td><td>" << static_cast<unsigned>(st_bind);

        u8 const st_type = ELF::get_st_type(sym.st_info);
        char const *type_name = symbol_type_2_str(st_type);
        if (type_name)
            dbgs() << "</td><td>" << type_name;
        else
            dbgs() << "</td><td>" << static_cast<unsigned>(st_type);

        // symbol visibility
        u8 const st_visibility = ELF::get_st_visibility(sym.st_other);
        char const *visibility_name = symbol_visibility_2_str(st_visibility);
        if (visibility_name)
            dbgs() << "</td><td>" << visibility_name;
        else
            dbgs() << "</td><td>" << static_cast<unsigned>(sym.st_other);

        // Section index
        char const *name = section_name_index(sym.st_shndx);
        if (name)
            dbgs() << "</td><td>" << name;
        else {
            ENSURE(sym.st_shndx < shdr.size(), "shdr index out of size");
            ENSURE(shdr[sym.st_shndx].sh_name < shstrtab.size(), "shstrtab index out of size");
            dbgs() << "</td><td>" << &shstrtab[shdr[sym.st_shndx].sh_name];
        }

        dbgs() << "</td><td>" << &strtab[sym.st_name] << "</td></tr>\n";

        if (sym.st_value && sym.st_name > 0) {
            // for static disassembling
            tempDecoder.add_symbol(sym.st_value, sym.st_size, &strtab[sym.st_name]);
            // for runtime decoder
            uintptr_t const address = sym.st_value + std::bit_cast<uintptr_t>(image.data());
            get_decoder().add_symbol(address, sym.st_size, &strtab[sym.st_name]);
        }
    }

    dbgs() << "</tbody></table>" << std::right;
    return 0;
}

/*==================================================================================*/
// ELF loading map
/*==================================================================================*/
template<typename ELF>
size_t
TargetSingleApp::ComputeMapSize(const std::span<const typename ELF::ELF_PHDR> program_headers)
{
    size_t map_size = 0;
    size_t mapped_size = 0;
    for (const auto & ph : program_headers) {
        map_size = std::max<size_t>(map_size, ph.p_vaddr + ph.p_memsz);
        if (ph.p_memsz > 0 && ph.p_type == PT_LOAD)
            mapped_size += PageSizeRoundUp(ph.p_memsz);
    }
    map_size = PageSizeRoundUp(map_size);
    LOG_DEBUG(LOG_LOADER)
        << "map_size=0x" << fmt::hex(map_size)
        << " mapped_size=0x" << fmt::hex(mapped_size);
    return map_size;
}

template<typename ELF>
static bool CheckEntryPoint(const std::span<const typename ELF::ELF_PHDR> program_headers, uintptr_t entry_point)
{
    for (const auto& ph : program_headers) {
        if ((ph.p_flags & PF_X) != 0 &&
             ph.p_vaddr <= entry_point && entry_point < ph.p_vaddr + ph.p_filesz) {
            return true;
        }
    }
    return false;
}

template<typename ELF>
int
TargetSingleApp::map_image(const std::span<std::byte> image, const std::span<const std::byte> original_file,
    const std::span<const typename ELF::ELF_PHDR> program_headers)
{
    uintptr_t prev_segment_end = 0;

    for (size_t i = 0; i < program_headers.size(); i++) {
        const typename ELF::ELF_PHDR &ph = program_headers[i];
        if (ph.p_memsz == 0 || ph.p_type != PT_LOAD)
            continue;

        WITH_DEBUG(LOG_DUMP) {
            dbgs() << "\nSEGMENT: index=" << i << " flags=";
            dump_protection_flags(ph.p_flags, dbgs());
            dbgs() << std::endl;
        }
        ENSURE(ph.p_memsz >= ph.p_filesz, "Bad ELF segment: p_memsz=" << ph.p_memsz << " < p_filesz=" << ph.p_filesz);

        uintptr_t const file_start = PageSizeRoundDown(ph.p_offset);
        uintptr_t const file_end   = PageSizeRoundUp  (ph.p_offset + ph.p_filesz);
        size_t const filesize = file_end - file_start;
        LOG_DEBUG(LOG_LOADER) << "file   used: " << fmt::hex(ph.p_offset)
                  << ".." << fmt::hex(ph.p_offset + ph.p_filesz)
                  << " filesz=" << ph.p_filesz;
        LOG_DEBUG(LOG_LOADER) << "file mapped: " << fmt::hex(file_start)
                  << ".." << fmt::hex(file_end)
                  << " filemap=" << filesize;
        LOG_DEBUG(LOG_LOADER) << " mem   used: " << fmt::hex(ph.p_vaddr)
                  << ".." << fmt::hex(ph.p_vaddr + ph.p_memsz)
                  << " memsz=" << ph.p_memsz;

        uintptr_t const segment_start = PageSizeRoundDown(ph.p_vaddr);
        uintptr_t const segment_end   = PageSizeRoundUp  (ph.p_vaddr + ph.p_memsz);
        size_t const segment_size = segment_end - segment_start;
        ENSURE(segment_start >= prev_segment_end, "PT_LOAD segments overlap or are not sorted");

        prev_segment_end = segment_end;
        LOG_DEBUG(LOG_LOADER) << " mem mapped: " << fmt::hex(segment_start)
                  << ".." << fmt::hex(segment_end)
                  << " memsz=" << segment_size;

        // std::byte *seg_map = &image[segment_start];
        assert(ph.p_vaddr + ph.p_filesz < image.size());
        memcpy(&image[ph.p_vaddr], &original_file[ph.p_offset], ph.p_filesz);
        /*
        u8 *seg_map = reinterpret_cast<u8 *>(mmap(&image[segment_start],
                 file_end - file_start,
                 PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_FIXED,
                 fd, file_start));
        if (seg_map == MAP_FAILED || seg_map != &image[segment_start]) {
            LOG_ERROR(LOG_LOADER) << "part image failed, phnum=" << i;
            return -1;
        }
        LOG_DEBUG(LOG_LOADER)
            << " map biased: " << fmt::hex(std::bit_cast<uintptr_t>(seg_map))
            << ".." << fmt::hex(std::bit_cast<uintptr_t>(seg_map) + segment_size);

        // zerofill overaligned parts
        size_t const extra_low_len = ph.p_vaddr - segment_start;
        if (extra_low_len > 0) {
            LOG_DEBUG(LOG_LOADER) << "memset low:"
                << " segment_start=" << fmt::hex(segment_start)
                << " map=" << fmt::hex(std::bit_cast<uintptr_t>(seg_map))
                << " size=" << extra_low_len;
            memset(seg_map, 0, extra_low_len);
        }
        */

        // Handle the BSS
        if (ph.p_memsz > ph.p_filesz) {
            ENSURE((ph.p_flags & PF_W) != 0, "Bad ELF segment: non-writable segment with BSS");
        }

        uintptr_t const bss_start     = ph.p_vaddr + ph.p_filesz;
        uintptr_t const bss_map_start = PageSizeRoundUp(bss_start);
        if (bss_start < bss_map_start) {
            // Zero the BSS to the end of the page
            LOG_DEBUG(LOG_LOADER) << "memset high:"
                << " start=" << fmt::hex(bss_start)
                << " map=" << fmt::hex(std::bit_cast<uintptr_t>(&image[bss_start]))
                << " size=" << (bss_map_start - bss_start);
            // memset(&image[bss_start], 0, bss_map_start - bss_start);
        }
        /*
        if (bss_map_start < segment_end) {
            // rest BSS is pagefile mapped
            void *map_addr = &image[bss_map_start];
            void *map_result = mmap(map_addr, segment_end - bss_map_start,
                              PROT_READ | PROT_WRITE,
                              MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED,
                              -1, 0);
            ENSURE(map_result == map_addr, "Failed to map BSS for ELF segment");
        }

        WITH_DEBUG(LOG_DUMP) {
            dbgs() << "\n" << fmt::hex_data(seg_map, segment_size, segment_start);
        }
            */
    }

    LOG_DEBUG(LOG_LOADER) << "mapping success";
    return 0;
}

/*==================================================================================*/
// ELF map protection
/*==================================================================================*/

static int ElfFlagsToMmapFlags(int pflags)
{
    return ((pflags & PF_X) != 0 ? PROT_EXEC  : 0) |
           ((pflags & PF_R) != 0 ? PROT_READ  : 0) |
           ((pflags & PF_W) != 0 ? PROT_WRITE : 0);
}

int TargetSingleApp::TargetProtectionToHost(int protection)
{
    int p = 0;
    if (protection & PROT_READ)  p |= PROT_READ;
    if (protection & PROT_WRITE) p |= PROT_WRITE;
    // emulator doesn't need PROT_EXEC host access, but need PROT_READ
    if (protection & PROT_EXEC)  p |= PROT_READ;
    return p;
}

u64 TargetSingleApp::TargetProtectionToEmulator(int protection)
{
    u64 p = 0;
    if (protection & PROT_READ) {
        p |= VM_PAGE_RIGHT_READ;
        // if readonly - make finalized
        if ((protection & PROT_WRITE) == 0)
            p |= VM_PAGE_RIGHT_FINAL;
    }
    if (protection & PROT_WRITE) {
        p |= VM_PAGE_RIGHT_WRITE;
    }
    if (protection & PROT_EXEC) {
        // emulated VM need finalize EXEC without RW
        p |= VM_PAGE_RIGHT_EXECUTE | VM_PAGE_RIGHT_FINAL;
        p &= ~(VM_PAGE_RIGHT_READ | VM_PAGE_RIGHT_WRITE);
    }
    return p;
}

template<typename ELF>
int
TargetSingleApp::map_protect(const std::span<const std::byte> image, const std::span<const typename ELF::ELF_PHDR> program_headers)
{
    LOG_DEBUG(LOG_LOADER) << "set protection after modify/relocation:";

    for (const auto& ph : program_headers) {
        if (ph.p_memsz == 0 || ph.p_type != PT_LOAD)
            continue;
        uintptr_t const segment_start = PageSizeRoundDown(ph.p_vaddr);
        uintptr_t const segment_end   = PageSizeRoundUp  (ph.p_vaddr + ph.p_memsz);

        int const prot = ElfFlagsToMmapFlags(ph.p_flags);
        // ENSURE(mprotect(&image[segment_start], segment_end - segment_start, TargetProtectionToHost(prot)) == 0, "mprotect failed: " << strerror(errno));
        AddHostMemory(&image[segment_start], segment_end - segment_start, TargetProtectionToEmulator(prot));
    }
    return 0;
}

/*==================================================================================*/
// ELF 64/32 loading
/*==================================================================================*/

template<typename ELF>
int
TargetSingleApp::load_program(const std::span<const std::byte> original_file, ElfInfo& elf_info, std::span<std::byte>& out_image)
{
    // locate/check elf header
    const typename ELF::ELF_EHDR& ehdr = *reinterpret_cast<const typename ELF::ELF_EHDR*>(&original_file[0]);
    ENSURE(sizeof(ehdr) < original_file.size(), "Failed to read ELF file header");

    dbgs() << "<table><caption>ELF header</caption>\n"
              "<tbody>\n";
    dbgs() << "<tr><td>e_ident</td><td>";
    for (unsigned i = 0; i < EI_NIDENT; i++) {
        dbgs() << ' ' << fmt::hex(static_cast<u8>(ehdr.e_ident[i]));
    }
    dbgs() << "</td></tr>\n"
#define TR(NAME, VALUE) "<tr><td>" #NAME "</td><td>" << VALUE << "</td></tr>\n"
        << TR(class, static_cast<unsigned>(ehdr.e_ident[EI_CLASS]))
        << TR(data, static_cast<unsigned>(ehdr.e_ident[EI_DATA]))
        << TR(version, static_cast<unsigned>(ehdr.e_ident[EI_VERSION]))
        << TR(osabi, static_cast<unsigned>(ehdr.e_ident[EI_OSABI]))
        << TR(abiversion, static_cast<unsigned>(ehdr.e_ident[EI_ABIVERSION]))
        << TR(type, ehdr.e_type)                         // Object file type
        << TR(machine, "0x" << fmt::hex(ehdr.e_machine)) // Architecture
        << TR(version, ehdr.e_version)                   // Object file version
        << TR(entry, ehdr.e_entry)                       // Entry point virtual address
        << TR(phoff, ehdr.e_phoff)                       // Program header table file offset
        << TR(shoff, ehdr.e_shoff)                       // Section header table file offset
        << TR(flags, "0x" << fmt::hex(ehdr.e_flags))     // Processor-specific flags
        << TR(ehsize, ehdr.e_ehsize)                     // ELF header size in bytes
        << TR(phentsize, ehdr.e_phentsize)               // Program header table entry size
        << TR(phnum, ehdr.e_phnum)                       // Program header table entry count
        << TR(shentsize, ehdr.e_shentsize)               // Section header table entry size
        << TR(shnum, ehdr.e_shnum)                       // Section header table entry count
        << TR(shstrndx, ehdr.e_shstrndx)                 // Section header string table index
        << "</tbody></table>\n";
#undef TR

    ENSURE(ehdr.e_version == EV_CURRENT, "Unexpected ELF e_version");
    ENSURE(ehdr.e_type == ET_DYN, "ELF e_type is not ET_DYN, only PIC/IE supported");
    ENSURE(ehdr.e_phentsize == sizeof(typename ELF::ELF_PHDR), "ELF e_phentsize: " << ehdr.e_phentsize << " " << sizeof(typename ELF::ELF_PHDR));
    ENSURE(ehdr.e_shentsize == sizeof(typename ELF::ELF_SHDR), "ELF e_shentsize: " << ehdr.e_shentsize << " " << sizeof(typename ELF::ELF_SHDR));

    elf_info.ph_size = ehdr.e_phentsize;
    elf_info.ph_num  = ehdr.e_phnum;
    elf_info.sh_size = ehdr.e_shentsize;
    elf_info.sh_num  = ehdr.e_shnum;

    // locate phdr
    ENSURE(ehdr.e_phoff + ehdr.e_phnum * sizeof(typename ELF::ELF_PHDR) <= original_file.size(), "no place for phdrs");
    const typename ELF::ELF_PHDR *phdr = reinterpret_cast<const typename ELF::ELF_PHDR *>(&original_file[ehdr.e_phoff]);

    // locate section headers
    ENSURE(ehdr.e_shoff + ehdr.e_shnum * sizeof(typename ELF::ELF_SHDR) <= original_file.size(), "can't place shdrs");
    const typename ELF::ELF_SHDR *shdr = reinterpret_cast<const typename ELF::ELF_SHDR *>(&original_file[ehdr.e_shoff]);

    // locate shstrtab
    ENSURE(ehdr.e_shstrndx < ehdr.e_shnum, "can't load shstrtab:" << " e_shstrndx=" << ehdr.e_shstrndx << " e_shnum" << ehdr.e_shnum);

    const typename ELF::ELF_SHDR& shstrtab_section = shdr[ehdr.e_shstrndx];
    ENSURE(shstrtab_section.sh_offset + shstrtab_section.sh_size <= original_file.size(), "can't load shstrtab");
    const std::span<const char> shstrtab = { reinterpret_cast<char const *>(&original_file[shstrtab_section.sh_offset]), shstrtab_section.sh_size };

    const std::span<const typename ELF::ELF_SHDR> section_headers = { shdr, ehdr.e_shnum };
    dump_shdrs<ELF>(section_headers, dbgs(), shstrtab);

    const std::span<const typename ELF::ELF_PHDR> program_headers = { phdr, ehdr.e_phnum };
    dump_phdrs<ELF>(program_headers, section_headers, shstrtab, dbgs());

    ENSURE(CheckEntryPoint<ELF>(program_headers, ehdr.e_entry), "ELF entry point does not point into an executable PT_LOAD segment");

    LOG_DEBUG(LOG_LOADER) << "Mapping image...";

    size_t const map_size = ComputeMapSize<ELF>(program_headers);

    //std::byte* image_base = reinterpret_cast<std::byte*>(mmap(nullptr, map_size, PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
    //if (image_base == MAP_FAILED) {
    const std::span<std::byte> image = virtual_alloc(map_size);
    if (!image.data()) {
        LOG_ERROR(LOG_LOADER) << "initial image_base failed";
        return -1;
    }
    memset(image.data(), 0, map_size);
    dbgs() << "<p>mapped image: 0x" << fmt::hex(std::bit_cast<uintptr_t>(image.data())) << "</p>\n";

    ENSURE(map_image<ELF>(image, original_file, program_headers) == 0, "map_image failed");

    elf_info.load_base = std::bit_cast<uintptr_t>(image.data());
    elf_info.entry     = std::bit_cast<uintptr_t>(image.data()) + ehdr.e_entry;
    elf_info.ph_base   = std::bit_cast<uintptr_t>(image.data()) + ehdr.e_phoff; // FIXME
    elf_info.dynamic   = 0;

    LOG_DEBUG(LOG_LOADER) << "load entry point: " << fmt::hex(elf_info.entry);
    LOG_DEBUG(LOG_LOADER) << "\nParse sections, relocate biased image...";

    Decoder tempDecoder; // used to log disassembling with image-relative addresses

    for (size_t i = 0; i < ehdr.e_shnum; i++) {
        const typename ELF::ELF_SHDR& sh = shdr[i];
        dbgs() << "<h2>SECTION: " << i << " name=" << &shstrtab[sh.sh_name] << "</h2>\n";

        // register section name for absolute/relative disassembling
        tempDecoder.add_section_name(sh.sh_addr, sh.sh_size, &shstrtab[sh.sh_name]);
        uintptr_t const address = std::bit_cast<uintptr_t>(&image[sh.sh_addr]);
        get_decoder().add_section_name(address, sh.sh_size, &shstrtab[sh.sh_name]);

        auto const sh_link = sh.sh_link;
        ENSURE(sh_link < ehdr.e_shnum, "invalid sh_link=" << sh_link);
        if (sh.sh_type != SHT_NULL && sh.sh_type != SHT_NOBITS) {
            std::byte const *sec_data = &original_file[sh.sh_offset];
            WITH_DEBUG(LOG_DUMP) {
                 dbgs() << fmt::hex_data(sec_data, std::min<size_t>(sh.sh_size, 4 * 64), 0);
            }
        }
        switch (sh.sh_type) {
        case SHT_DYNAMIC:
            LOG_DEBUG(LOG_LOADER) << "Dynamic found: " << i;
            ENSURE(parse_dynamic<ELF>(sh, original_file, image, elf_info) == 0, "parse_dynamic failed");
            break;
        case SHT_SYMTAB:
        case SHT_DYNSYM:
            ENSURE(parse_symtab<ELF>(section_headers, sh, shdr[sh_link], shstrtab, original_file, image, tempDecoder) == 0, "parse_symtab failed");
            break;
        case SHT_REL:
            ENSURE(parse_rel<ELF>(sh, original_file, image) == 0, "parse_rel failed");
            break;
        case SHT_RELA:
            ENSURE(parse_rela<ELF>(sh, original_file, image) == 0, "parse_rela failed");
            break;
        case SHT_GNU_HASH:
            LOG_DEBUG(LOG_LOADER) << "GNU_HASH";
            break;
        }
    }

    dbgs() << "<h2>second pass, disassemble with known names</h2>";

    for (size_t i = 0; i < ehdr.e_shnum; i++) {
        const typename ELF::ELF_SHDR& sh = shdr[i];
        switch (sh.sh_type) {
        case SHT_PROGBITS:
            if (sh.sh_flags & SHF_EXECINSTR) {
                dbgs() << "<h2>SECTION: " << i << " name=" << &shstrtab[sh.sh_name] << "</h2>\n";
                ENSURE(parse_code<ELF>(sh, image, tempDecoder) == 0, "parse_code failed");
                elf_info.text_base = std::bit_cast<uintptr_t>(&image[sh.sh_addr]);
                elf_info.text_size = sh.sh_size;
            }
            break;
        }
    }

    ENSURE(map_protect<ELF>(image, program_headers) == 0, "map_protect failed");
    out_image = image;
    return 0;
}

int
TargetSingleApp::load_program(const std::span<const std::byte> original_file, ElfInfo& elf_info, std::span<std::byte>& image)
{
    ENSURE(original_file.size() >= EI_NIDENT, "Invalid ELF file: no place for e_ident");

    ENSURE(std::memcmp(&original_file[0], ELFMAG, SELFMAG) == 0, "Invalid ELF file: no ELF magic");
    ENSURE(static_cast<u8>(original_file[EI_DATA]) == ELFDATA2LSB, "Unsupported: not a little-endian ELF file");

    switch (static_cast<u8>(original_file[EI_CLASS])) {
    case ELFCLASS64:
        LOG_DEBUG(LOG_LOADER) << "64-bit";
        return load_program<ELF64>(original_file, elf_info, image);
    case ELFCLASS32:
        LOG_DEBUG(LOG_LOADER) << "32-bit";
        return load_program<ELF32>(original_file, elf_info, image);
    }

    LOG_ERROR(LOG_LOADER) << "Invalid ELF class " << static_cast<unsigned>(original_file[EI_CLASS]);
    return -1;
}

int
TargetSingleApp::load_elf_module(char const *file_name, ElfInfo& elf_info, std::span<std::byte>& image)
{
    std::vector<std::byte> original_file;
    ENSURE(util::load_file(file_name, original_file), "loading file failed: " << file_name);
    dbgs() << "<p>ELF file `" << file_name << "` size=" << original_file.size() << "</p>\n";

    ENSURE(load_program({ original_file.data(), original_file.size() }, elf_info, image) == 0, "load_program failed: " << file_name);
    return 0;
}

} // namespace postrisc
