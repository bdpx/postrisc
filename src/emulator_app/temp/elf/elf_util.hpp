#pragma once

// Copyright 2014 Renato Tegon Forti, Antony Polukhin.
// Copyright Antony Polukhin, 2015-2023.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <cstdint>
#include <cstring>
#include <cassert>
#include <fstream>
#include <limits>
#include <vector>

#include "elf_defs.h"

namespace postrisc { namespace elf {

template <typename AddressOffsetT>
struct Elf_Ehdr_template {
    unsigned char     e_ident[16];    /* Magic number and other info */
    u16               e_type;         /* Object file type */
    u16               e_machine;      /* Architecture */
    u32               e_version;      /* Object file version */
    AddressOffsetT    e_entry;        /* Entry point virtual address */
    AddressOffsetT    e_phoff;        /* Program header table file offset */
    AddressOffsetT    e_shoff;        /* Section header table file offset */
    u32               e_flags;        /* Processor-specific flags */
    u16               e_ehsize;       /* ELF header size in bytes */
    u16               e_phentsize;    /* Program header table entry size */
    u16               e_phnum;        /* Program header table entry count */
    u16               e_shentsize;    /* Section header table entry size */
    u16               e_shnum;        /* Section header table entry count */
    u16               e_shstrndx;     /* Section header string table index */

    void dump(std::ostream& out) const;
    friend std::ostream& operator<<(std::ostream& out, const Elf_Ehdr_template& eh) {
        eh.dump(out);
        return out;
    }
};

typedef Elf_Ehdr_template<u32> Elf32_Ehdr_;
typedef Elf_Ehdr_template<u64> Elf64_Ehdr_;

template <typename AddressOffsetT>
void
Elf_Ehdr_template<AddressOffsetT>::dump(std::ostream& out) const
{
    out << "ELF header:"
    << "\n\tclass       " << static_cast<unsigned>(e_ident[EI_CLASS])
    << "\n\tdata        " << static_cast<unsigned>(e_ident[EI_DATA])
    << "\n\tversion     " << static_cast<unsigned>(e_ident[EI_VERSION])
    << "\n\tosabi       " << static_cast<unsigned>(e_ident[EI_OSABI])
    << "\n\tabiversion  " << static_cast<unsigned>(e_ident[EI_ABIVERSION])
    << "\n\ttype        " << e_type
    << "\n\tmachine     " << e_machine
    << "\n\tversion     " << e_version
    << "\n\tentry       0x" << fmt::hex(e_entry)
    << "\n\tphoff       0x" << fmt::hex(e_phoff)
    << "\n\tshoff       0x" << fmt::hex(e_shoff)
    << "\n\tflags       0x" << fmt::hex(e_flags)
    << "\n\tehsize      " << e_ehsize
    << "\n\tphentsize   " << e_phentsize
    << "\n\tphnum       " << e_phnum
    << "\n\tshentsize   " << e_shentsize
    << "\n\tshnum       " << e_shnum
    << "\n\tshstrndx    " << e_shstrndx;
};

template <typename AddressOffsetT>
struct Elf_Shdr_template {
  u32          sh_name;        /* Section name (string tbl index) */
  u32               sh_type;        /* Section type */
  AddressOffsetT    sh_flags;       /* Section flags */
  AddressOffsetT    sh_addr;        /* Section virtual addr at execution */
  AddressOffsetT    sh_offset;      /* Section file offset */
  AddressOffsetT    sh_size;        /* Section size in bytes */
  u32               sh_link;        /* Link to another section */
  u32               sh_info;        /* Additional section information */
  AddressOffsetT    sh_addralign;   /* Section alignment */
  AddressOffsetT    sh_entsize;     /* Entry size if section holds table */

  void dump(std::ostream& out);
};

typedef Elf_Shdr_template<u32> Elf32_Shdr_;
typedef Elf_Shdr_template<u64> Elf64_Shdr_;


template <typename AddressOffsetT>
struct Elf_Phdr_template;

template <>
struct Elf_Phdr_template<u32> {
  typedef u32 AddressOffsetT;

  u32            p_type;   // Type of segment
  AddressOffsetT p_offset; // File offset where segment is located, in bytes
  AddressOffsetT p_vaddr;  // Virtual address of beginning of segment
  AddressOffsetT p_paddr;  // Physical address of beginning of segment (OS-specific)
  AddressOffsetT p_filesz; // Num. of bytes in file image of segment (may be zero)
  AddressOffsetT p_memsz;  // Num. of bytes in mem image of segment (may be zero)
  u32            p_flags;  // Segment flags
  AddressOffsetT p_align;  // Segment alignment constraint

  void dump(std::ostream& out);
};

// Program header for ELF64.
template <>
struct Elf_Phdr_template<u64> {
    typedef u64 AddressOffsetT;

    u32       p_type;   // Type of segment
    u32       p_flags;  // Segment flags
    AddressOffsetT p_offset; // File offset where segment is located, in bytes
    AddressOffsetT p_vaddr;  // Virtual address of beginning of segment
    AddressOffsetT p_paddr;  // Physical addr of beginning of segment (OS-specific)
    AddressOffsetT p_filesz; // Num. of bytes in file image of segment (may be zero)
    AddressOffsetT p_memsz;  // Num. of bytes in mem image of segment (may be zero)
    AddressOffsetT p_align;  // Segment alignment constraint

    void dump(std::ostream& out) const {
        out << "type=" << fmt::hex(p_type)
        << " flags=" << fmt::hex(p_flags)
        << " offset=" << fmt::hex(p_offset)
        << " vaddr=" << fmt::hex(p_vaddr)
        << " paddr=" << fmt::hex(p_paddr)
        << " filesz=" << fmt::hex(p_filesz)
        << " memsz=" << fmt::hex(p_memsz)
        << " align=" << fmt::hex(p_align);
    }
    friend std::ostream& operator<<(std::ostream& out, const Elf_Phdr_template& ph) {
        ph.dump(out);
        return out;
    }
};

typedef Elf_Phdr_template<u32> Elf32_Phdr;
typedef Elf_Phdr_template<u64> Elf64_Phdr;

template <typename AddressOffsetT>
struct Elf_Sym_template;

template <>
struct Elf_Sym_template<u32> {
  typedef u32 AddressOffsetT;

  u32               st_name;    /* Symbol name (string tbl index) */
  AddressOffsetT    st_value;   /* Symbol value */
  AddressOffsetT    st_size;    /* Symbol size */
  unsigned char     st_info;    /* Symbol type and binding */
  unsigned char     st_other;   /* Symbol visibility */
  u16               st_shndx;   /* Section index */
};

template <>
struct Elf_Sym_template<u64> {
  typedef u64 AddressOffsetT;

  u32               st_name;    /* Symbol name (string tbl index) */
  unsigned char     st_info;    /* Symbol type and binding */
  unsigned char     st_other;   /* Symbol visibility */
  u16               st_shndx;   /* Section index */
  AddressOffsetT    st_value;   /* Symbol value */
  AddressOffsetT    st_size;    /* Symbol size */
};


typedef Elf_Sym_template<u32> Elf32_Sym_;
typedef Elf_Sym_template<u64> Elf64_Sym_;

template <typename AddressOffsetT>
class elf_info {
    typedef Elf_Ehdr_template<AddressOffsetT>  header_t;
    typedef Elf_Shdr_template<AddressOffsetT>  section_t;
    typedef Elf_Sym_template<AddressOffsetT>   symbol_t;

    static const u32 SHT_SYMTAB_ = 2;
    static const u32 SHT_STRTAB_ = 3;
    static const u32 SHT_DYNSYM_ = 11;

    static const unsigned char STB_LOCAL_ = 0;   /* Local symbol */
    static const unsigned char STB_GLOBAL_ = 1;  /* Global symbol */
    static const unsigned char STB_WEAK_ = 2;    /* Weak symbol */

    /* Symbol visibility specification encoded in the st_other field.  */
    static const unsigned char STV_DEFAULT_ = 0;      /* Default symbol visibility rules */
    static const unsigned char STV_INTERNAL_ = 1;     /* Processor specific hidden class */
    static const unsigned char STV_HIDDEN_ = 2;       /* Sym unavailable in other modules */
    static const unsigned char STV_PROTECTED_ = 3;    /* Not preemptible, not exported */

public:
    static bool parsing_supported(std::ifstream& fs) {
        const unsigned char magic_bytes[5] = { 
            0x7f, 'E', 'L', 'F', sizeof(u32) == sizeof(AddressOffsetT) ? 1 : 2
        };

        fs.seekg(0);
        for (std::size_t i = 0; i < sizeof(magic_bytes); ++i) {
            unsigned char ch;
            fs >> ch;
            if (ch != magic_bytes[i]) {
                return false;
            }
        }

        return true;
    }

    static std::vector<std::string> sections(std::ifstream& fs) {
        std::vector<std::string> ret;
        std::vector<char> names;
        sections_names_raw(fs, names);

        const char* name_begin = &names[0];
        const char* const name_end = name_begin + names.size();
        ret.reserve(header(fs).e_shnum);
        do {
            if (*name_begin) {
                ret.push_back(name_begin);
                name_begin += ret.back().size() + 1;
            } else {
                ++name_begin;
            }
        } while (name_begin != name_end);

        return ret;
    }

    static std::vector<Elf_Phdr_template<AddressOffsetT>> program_sections(std::ifstream& fs) {
        const header_t elf = header(fs);
        // section_t section_names_section;
        checked_seekg(fs, elf.e_phoff);
        assert(elf.e_phentsize == sizeof(Elf_Phdr_template<AddressOffsetT>));
        std::vector<Elf_Phdr_template<AddressOffsetT>> ps(elf.e_phnum);
        read_raw(fs, ps[0], elf.e_phnum * sizeof(Elf_Phdr_template<AddressOffsetT>));
        return ps;
    }

private:
    template <typename Integer>
    static void checked_seekg(std::ifstream& fs, Integer pos) {
        /* TODO: use cmp_less, cmp_greater
        if ((std::numeric_limits<std::streamoff>::max)() < pos) {
            boost::throw_exception(std::runtime_error("Integral overflow while getting info from ELF file"));
        }
        if ((std::numeric_limits<std::streamoff>::min)() > pos){
            boost::throw_exception(std::runtime_error("Integral underflow while getting info from ELF file"));
        }
        */
        fs.seekg(static_cast<std::streamoff>(pos));
    }

    template <typename T>
    static void read_raw(std::ifstream& fs, T& value, std::size_t size = sizeof(T)) {
        fs.read(reinterpret_cast<char*>(&value), size);
    }

    static header_t header(std::ifstream& fs) {
        header_t elf;

        fs.seekg(0);
        read_raw(fs, elf);

        return elf;
    }

    static void sections_names_raw(std::ifstream& fs, std::vector<char>& sections) {
        const header_t elf = header(fs);

        section_t section_names_section;
        checked_seekg(fs, elf.e_shoff + elf.e_shstrndx * sizeof(section_t));
        read_raw(fs, section_names_section);

        sections.resize(static_cast<std::size_t>(section_names_section.sh_size) + 1, '\0');
        checked_seekg(fs, section_names_section.sh_offset);
        read_raw(fs, sections[0], static_cast<std::size_t>(section_names_section.sh_size));
    }

    static void symbols_text(std::ifstream& fs, std::vector<symbol_t>& symbols, std::vector<char>& text) {
        std::vector<char> names;
        sections_names_raw(fs, names);
        symbols_text(fs, symbols, text, names);
    }

    static void symbols_text(std::ifstream& fs, std::vector<symbol_t>& symbols, std::vector<char>& text, const std::vector<char>& names) {
        const header_t elf = header(fs);
        checked_seekg(fs, elf.e_shoff);

        // ".dynsym" section may not have info on symbols that could be used while self loading an executable,
        // so we prefer ".symtab" section.
        AddressOffsetT symtab_size = 0;
        AddressOffsetT symtab_offset = 0;
        AddressOffsetT strtab_size = 0;
        AddressOffsetT strtab_offset = 0;

        AddressOffsetT dynsym_size = 0;
        AddressOffsetT dynsym_offset = 0;
        AddressOffsetT dynstr_size = 0;
        AddressOffsetT dynstr_offset = 0;

        for (std::size_t i = 0; i < elf.e_shnum; ++i) {
            section_t section;
            read_raw(fs, section);
            if (section.sh_name >= names.size()) {
                continue;
            }
            const char* name = &names[section.sh_name];

            if (section.sh_type == SHT_SYMTAB_ && !std::strcmp(name, ".symtab")) {
                symtab_size = section.sh_size;
                symtab_offset = section.sh_offset;
            } else if (section.sh_type == SHT_STRTAB_) {
                if (!std::strcmp(name, ".dynstr")) {
                    dynstr_size = section.sh_size;
                    dynstr_offset = section.sh_offset;
                } else if (!std::strcmp(name, ".strtab")) {
                    strtab_size = section.sh_size;
                    strtab_offset = section.sh_offset;
                }
            } else if (section.sh_type == SHT_DYNSYM_ && !std::strcmp(name, ".dynsym")) {
                dynsym_size = section.sh_size;
                dynsym_offset = section.sh_offset;
            }
        }

        if (!symtab_size || !strtab_size) {
            // ".symtab" stripped from the binary and we have to fallback to ".dynsym"
            symtab_size = dynsym_size;
            symtab_offset = dynsym_offset;
            strtab_size = dynstr_size;
            strtab_offset = dynstr_offset;
        }

        if (!symtab_size || !strtab_size) {
            return;
        }

        text.resize(static_cast<std::size_t>(strtab_size) + 1, '\0');
        checked_seekg(fs, strtab_offset);
        read_raw(fs, text[0], static_cast<std::size_t>(strtab_size));

        symbols.resize(static_cast<std::size_t>(symtab_size / sizeof(symbol_t)));
        checked_seekg(fs, symtab_offset);
        read_raw(fs, symbols[0], static_cast<std::size_t>(symtab_size - (symtab_size % sizeof(symbol_t))) );
    }

    static bool is_visible(const symbol_t& sym) {
        const unsigned char visibility = (sym.st_other & 0x03);
        // `(sym.st_info >> 4) != STB_LOCAL_ && !!sym.st_size` check also workarounds the
        // GCC's issue https://sourceware.org/bugzilla/show_bug.cgi?id=13621
        return (visibility == STV_DEFAULT_ || visibility == STV_PROTECTED_)
                && (sym.st_info >> 4) != STB_LOCAL_ && !!sym.st_size;
    }

public:
    static std::vector<std::string> symbols(std::ifstream& fs) {
        std::vector<std::string> ret;

        std::vector<symbol_t> symbols;
        std::vector<char>   text;
        symbols_text(fs, symbols, text);

        ret.reserve(symbols.size());
        for (std::size_t i = 0; i < symbols.size(); ++i) {
            if (is_visible(symbols[i]) && symbols[i].st_name < text.size()) {
                ret.push_back(&text[symbols[i].st_name]);
                if (ret.back().empty()) {
                    ret.pop_back(); // Do not show empty names
                }
            }
        }

        return ret;
    }

    static std::vector<std::string> symbols(std::ifstream& fs, const char* section_name) {
        std::vector<std::string> ret;
        
        std::size_t index = 0;
        std::size_t ptrs_in_section_count = 0;

        std::vector<char> names;
        sections_names_raw(fs, names);

        const header_t elf = header(fs);

        for (; index < elf.e_shnum; ++index) {
            section_t section;
            checked_seekg(fs, elf.e_shoff + index * sizeof(section_t));
            read_raw(fs, section);

            if (!std::strcmp(&names.at(section.sh_name), section_name)) {
                if (!section.sh_entsize) {
                    section.sh_entsize = 1;
                }
                ptrs_in_section_count = static_cast<std::size_t>(section.sh_size / section.sh_entsize);
                break;
            }
        }

        std::vector<symbol_t> symbols;
        std::vector<char> text;
        symbols_text(fs, symbols, text, names);
    
        if (ptrs_in_section_count < symbols.size()) {
            ret.reserve(ptrs_in_section_count);
        } else {
            ret.reserve(symbols.size());
        }

        for (std::size_t i = 0; i < symbols.size(); ++i) {
            if (symbols[i].st_shndx == index && is_visible(symbols[i]) && symbols[i].st_name < text.size()) {
                ret.push_back(&text[symbols[i].st_name]);
                if (ret.back().empty()) {
                    ret.pop_back(); // Do not show empty names
                }
            }
        }

        return ret;
    }
};

typedef elf_info<u32> elf_info32;
typedef elf_info<u64> elf_info64;

#define DEFAULT_STACK_MAX U64C(4096)

class elf_load_info {
public:
    u64  entry; // entry point
};

static inline u64 GetProtection(u32 flags)
{
    u64 protection = VM_PAGE_RIGHT_FINAL;
    if (flags & PF_R) protection |= VM_PAGE_RIGHT_READ;
    if (flags & PF_W) protection |= VM_PAGE_RIGHT_WRITE;
    if (flags & PF_X) protection |= VM_PAGE_RIGHT_EXECUTE;
    if (protection & VM_PAGE_RIGHT_EXECUTE) protection &= ~VM_PAGE_RIGHT_READ;
    return protection;
}

template<typename AddressOffsetT, typename VECTOR>
int load_library(
     const std::vector<u8> &image,        // elf image
                         VECTOR &memory,       // aligned memory device
          std::vector<u64> &PageData,     // per-page info
                         size_t pagesize,      // page size
                         size_t memory_offset, // offset inside `memory` array
                         size_t as_offset,     // base for relocation
                  elf_load_info &load_info)    // as-relative entry address
{
    if (image.size() < sizeof(Elf_Ehdr_template<AddressOffsetT>)) {
        LOG_ERROR(LOG_LOADER) << "can't read eh";
        return -1;
    }
    const Elf_Ehdr_template<AddressOffsetT> &eh =
        *reinterpret_cast<Elf_Ehdr_template<AddressOffsetT> const *>(&image[0]);
    LOG_OUTPUT(LOG_LOADER) << eh << fmt::endl;

    if (eh.e_machine != EM_POSTRISC) {
        LOG_ERROR(LOG_LOADER) << "invalid e_machine";
        return -1;
    }
    if (eh.e_phentsize != sizeof(Elf_Phdr_template<AddressOffsetT>)) {
        LOG_ERROR(LOG_LOADER) << "invalid e_phentsize";
        return -1;
    }
    if (eh.e_type != ET_DYN && eh.e_type != ET_EXEC) {
        LOG_ERROR(LOG_LOADER) << "invalid e_type";
        return -1;
    }
    size_t const phsize = static_cast<size_t>(eh.e_phentsize) * eh.e_phnum;

    Elf_Phdr_template<AddressOffsetT> const *phdrs = (Elf_Phdr_template<AddressOffsetT> const *)&image[eh.e_phoff];
    if (image.size() < static_cast<size_t>(eh.e_phoff) + static_cast<size_t>(eh.e_phnum) * eh.e_phentsize) {
        LOG_ERROR(LOG_LOADER) << "can't read phdrs";
        return -1;
    }

    size_t addr_min=SIZE_MAX, addr_max=0, map_len;
    size_t nsegs = 0;
    u64 /*off_t*/ off_start = 0;
    // Phdr *ph, *ph0;
    unsigned prot;
    // unsigned char *map=MAP_FAILED, *base;
    size_t dyn = 0;
    size_t tls_image = 0;

    AddressOffsetT __default_stacksize = 0;

    for (size_t i = 0; i < eh.e_phnum; i++) {
        Elf_Phdr_template<AddressOffsetT> const &ph = phdrs[i];
        LOG_OUTPUT(LOG_LOADER) << ph << fmt::endl;
        if (((ph.p_vaddr - ph.p_offset) & (pagesize - 1)) != 0) {
            LOG_ERROR(LOG_LOADER) << "ELF load command address/offset not page-aligned";
            return -1;
        }

        if (ph.p_type == PT_DYNAMIC) {
            dyn = ph.p_vaddr;
        } else if (ph.p_type == PT_TLS) {
            tls_image = ph.p_vaddr;
            //dso->tls.align = ph.p_align;
            //dso->tls.len = ph.p_filesz;
            //dso->tls.size = ph.p_memsz;
        } else if (ph.p_type == PT_GNU_RELRO) {
            //dso->relro_start = util::rounddown_pow2(ph.p_vaddr, pagesize);
            //dso->relro_end = util::rounddown_pow2(ph.p_vaddr + ph.p_memsz, pagesize);
        } else if (ph.p_type == PT_GNU_STACK) {
            if (ph.p_memsz > __default_stacksize) {
                __default_stacksize = std::min<AddressOffsetT>(ph.p_memsz, DEFAULT_STACK_MAX);
            }
        }
        if (ph.p_type != PT_LOAD) continue;
        nsegs++;
        if (ph.p_vaddr < addr_min) {
            addr_min = ph.p_vaddr;
            off_start = ph.p_offset;
            //prot = (((ph.p_flags & PF_R) ? PROT_READ : 0) |
            //        ((ph.p_flags & PF_W) ? PROT_WRITE: 0) |
            //        ((ph.p_flags & PF_X) ? PROT_EXEC : 0));
        }
        addr_max = std::max(addr_max, ph.p_vaddr + ph.p_memsz);
    }

    addr_min  = util::rounddown_pow2(addr_min, pagesize);
    LOG_OUTPUT(LOG_LOADER) << "addr_min=" << fmt::hex(addr_min) << std::endl;
    addr_max  = util::roundup_pow2(addr_max, pagesize);
    LOG_OUTPUT(LOG_LOADER) << "addr_max=" << fmt::hex(addr_max) << std::endl;
    off_start = util::rounddown_pow2(off_start, pagesize);
    LOG_OUTPUT(LOG_LOADER) << "off_start=" << fmt::hex(off_start) << std::endl;
    AddressOffsetT entry = memory_offset + eh.e_entry - addr_min;
    LOG_OUTPUT(LOG_LOADER) << "entry=" << fmt::hex(entry) << std::endl;
    AddressOffsetT vaddr = as_offset + eh.e_entry - addr_min;
    LOG_OUTPUT(LOG_LOADER) << "vaddr=" << fmt::hex(vaddr) << std::endl;
    
    load_info.entry = vaddr;

    map_len = addr_max - addr_min + off_start;
    LOG_OUTPUT(LOG_LOADER) << "map_len=" << fmt::hex(map_len) << std::endl;
    /* The first time, we map too much, possibly even more than
     * the length of the file. This is okay because we will not
     * use the invalid part; we just need to reserve the right
     * amount of virtual address space to map over later. */

    if (memory.size() < memory_offset + map_len)
        memory.resize(memory_offset + map_len, 0);
    //dso->map = map;
    //dso->map_len = map_len;

    // map = mmap(addr_min, ...);
    //base = map - addr_min;
    //dso->phdr = 0;
    //dso->phnum = 0;
    for (size_t i = 0; i < eh.e_phnum; i++) {
        Elf_Phdr_template<AddressOffsetT> const &ph = phdrs[i];
        if (ph.p_type != PT_LOAD) continue;
#if 0
        /* Check if the programs headers are in this load segment, and
         * if so, record the address for use by dl_iterate_phdr. */
        if (!dso->phdr &&
            eh.e_phoff >= ph.p_offset &&
            eh.e_phoff + phsize <= ph.p_offset + ph.p_filesz) {
            dso->phdr = (void *)(base + ph.p_vaddr + (eh.e_phoff - ph.p_offset));
            dso->phnum = eh.e_phnum;
            dso->phentsize = eh.e_phentsize;
        }
#endif
        size_t const this_min  = util::rounddown_pow2(ph.p_vaddr, pagesize);
        size_t const this_max  = util::roundup_pow2(ph.p_vaddr + ph.p_memsz, pagesize);
        off_start = util::rounddown_pow2(ph.p_offset, pagesize);
        u64 const protection = GetProtection(ph.p_flags);
        /* Reuse the existing mapping for the lowest-address LOAD */
        if (image.size() < (size_t)ph.p_offset + (size_t)ph.p_filesz) {
            LOG_ERROR(LOG_LOADER) << "ELF load error: filesize=" << image.size()
                                  << " offset=" << ph.p_offset << " copy_len=" << ph.p_filesz;
            return -1;
        }
        size_t const offset = ph.p_vaddr - addr_min + ph.p_filesz;
        if (memory.size() < memory_offset + offset) {
            LOG_ERROR(LOG_LOADER) << "ELF load error: memsize=" << memory.size()
                                  << " offset=" << (memory_offset + offset)
                                  << " copy_len=" << ph.p_filesz;
            return -1;
        }
        memcpy(&memory[memory_offset + ph.p_vaddr - addr_min], &image[ph.p_offset], ph.p_filesz);
        for (size_t pos = this_min; pos < this_max; pos += pagesize) {
            //LOG_OUTPUT(LOG_LOADER) << "pos=" << fmt::hex(pos) << std::endl;
            PageData[(memory_offset - addr_min + pos) / pagesize] = protection;
            //LOG_OUTPUT(LOG_LOADER) << "ok pos=" << fmt::hex(pos) << std::endl;
        }
#if 0
        if (util::rounddown_pow2(ph.p_vaddr, pagesize) != addr_min || DL_NOMMU_SUPPORT)
            if (mmap_fixed(base+this_min, this_max-this_min, prot, MAP_PRIVATE|MAP_FIXED, fd, off_start) == MAP_FAILED)
                goto error;
        if (ph.p_memsz > ph.p_filesz && (ph.p_flags & PF_W)) {
            size_t brk = (size_t)base + ph.p_vaddr + ph.p_filesz;
            size_t pgbrk = util::roundup_pow2(brk, pagesize);
            memset((void *)brk, 0, pgbrk-brk & pagesize-1);
            if (pgbrk-(size_t)base < this_max &&
                mmap_fixed((void *)pgbrk, (size_t)base+this_max-pgbrk, prot, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) == MAP_FAILED)
                goto error;
        }
#endif
    }

#if 0
    for (i=0; ((size_t *)(base+dyn))[i]; i+=2)
        if (((size_t *)(base+dyn))[i]==DT_TEXTREL) {
            if (mprotect(map, map_len, PROT_READ|PROT_WRITE|PROT_EXEC)
                && errno != ENOSYS)
                goto error;
            break;
        }
done_mapping:
    dso->base = base;
    dso->dynv = laddr(dso, dyn);
    if (dso->tls.size) dso->tls.image = laddr(dso, tls_image);
    return map;
noexec:
    errno = ENOEXEC;
error:
    if (map!=MAP_FAILED) unmap_library(dso);
#endif
    return 0;
}

}} // namespace postrisc::elf
