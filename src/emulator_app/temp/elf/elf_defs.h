#pragma once

#include <cstdint>
#include <cstring>

namespace postrisc {

using Elf32_Addr = uint32_t; // Program address
using Elf32_Off = uint32_t;  // File offset
using Elf32_Half = u16;
using Elf32_Word = uint32_t;
using Elf32_Sword = int32_t;

using Elf64_Addr = uint64_t;
using Elf64_Off = uint64_t;
using Elf64_Half = u16;
using Elf64_Word = uint32_t;
using Elf64_Sword = int32_t;
using Elf64_Xword = uint64_t;
using Elf64_Sxword = int64_t;

// Object file magic string.
static const char ElfMagic[] = {0x7f, 'E', 'L', 'F', '\0'};

// e_ident size and indices.
enum {
  EI_MAG0 = 0,       // File identification index.
  EI_MAG1 = 1,       // File identification index.
  EI_MAG2 = 2,       // File identification index.
  EI_MAG3 = 3,       // File identification index.
  EI_CLASS = 4,      // File class.
  EI_DATA = 5,       // Data encoding.
  EI_VERSION = 6,    // File version.
  EI_OSABI = 7,      // OS/ABI identification.
  EI_ABIVERSION = 8, // ABI version.
  EI_PAD = 9,        // Start of padding bytes.
  EI_NIDENT = 16     // Number of bytes in e_ident.
};

struct Elf32_Ehdr {
  unsigned char e_ident[EI_NIDENT]; // ELF Identification bytes
  Elf32_Half e_type;                // Type of file (see ET_* below)
  Elf32_Half e_machine;   // Required architecture for this file (see EM_*)
  Elf32_Word e_version;   // Must be equal to 1
  Elf32_Addr e_entry;     // Address to jump to in order to start program
  Elf32_Off e_phoff;      // Program header table's file offset, in bytes
  Elf32_Off e_shoff;      // Section header table's file offset, in bytes
  Elf32_Word e_flags;     // Processor-specific flags
  Elf32_Half e_ehsize;    // Size of ELF header, in bytes
  Elf32_Half e_phentsize; // Size of an entry in the program header table
  Elf32_Half e_phnum;     // Number of entries in the program header table
  Elf32_Half e_shentsize; // Size of an entry in the section header table
  Elf32_Half e_shnum;     // Number of entries in the section header table
  Elf32_Half e_shstrndx;  // Sect hdr table index of sect name string table

  bool checkMagic() const {
    return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0;
  }

  unsigned char getFileClass() const { return e_ident[EI_CLASS]; }
  unsigned char getDataEncoding() const { return e_ident[EI_DATA]; }
};

// 64-bit ELF header. Fields are the same as for ELF32, but with different
// types (see above).
struct Elf64_Ehdr {
  unsigned char e_ident[EI_NIDENT];
  Elf64_Half e_type;
  Elf64_Half e_machine;
  Elf64_Word e_version;
  Elf64_Addr e_entry;
  Elf64_Off e_phoff;
  Elf64_Off e_shoff;
  Elf64_Word e_flags;
  Elf64_Half e_ehsize;
  Elf64_Half e_phentsize;
  Elf64_Half e_phnum;
  Elf64_Half e_shentsize;
  Elf64_Half e_shnum;
  Elf64_Half e_shstrndx;

  bool checkMagic() const {
    return (memcmp(e_ident, ElfMagic, strlen(ElfMagic))) == 0;
  }

  unsigned char getFileClass() const { return e_ident[EI_CLASS]; }
  unsigned char getDataEncoding() const { return e_ident[EI_DATA]; }
};

// File types.
// See current registered ELF types at:
//    http://www.sco.com/developers/gabi/latest/ch4.eheader.html
enum {
  ET_NONE = 0,        // No file type
  ET_REL = 1,         // Relocatable file
  ET_EXEC = 2,        // Executable file
  ET_DYN = 3,         // Shared object file
  ET_CORE = 4,        // Core file
  ET_LOOS = 0xfe00,   // Beginning of operating system-specific codes
  ET_HIOS = 0xfeff,   // Operating system-specific
  ET_LOPROC = 0xff00, // Beginning of processor-specific codes
  ET_HIPROC = 0xffff  // Processor-specific
};

// Versioning
enum { EV_NONE = 0, EV_CURRENT = 1 };

// Machine architectures
enum {
  EM_NONE = 0,            // No machine
  EM_LOONGARCH = 258,     // LoongArch
  EM_POSTRISC = 0x1973,   // Postrisc 64-bit virtual processor
};

// Object file classes.
enum {
  ELFCLASSNONE = 0,
  ELFCLASS32 = 1, // 32-bit object file
  ELFCLASS64 = 2  // 64-bit object file
};

// Object file byte orderings.
enum {
  ELFDATANONE = 0, // Invalid data encoding.
  ELFDATA2LSB = 1, // Little-endian object file
  ELFDATA2MSB = 2  // Big-endian object file
};

// OS ABI identification.
enum {
  ELFOSABI_NONE = 0,           // UNIX System V ABI
  ELFOSABI_HPUX = 1,           // HP-UX operating system
  ELFOSABI_NETBSD = 2,         // NetBSD
  ELFOSABI_GNU = 3,            // GNU/Linux
  ELFOSABI_LINUX = 3,          // Historical alias for ELFOSABI_GNU.
  ELFOSABI_HURD = 4,           // GNU/Hurd
  ELFOSABI_SOLARIS = 6,        // Solaris
  ELFOSABI_AIX = 7,            // AIX
  ELFOSABI_IRIX = 8,           // IRIX
  ELFOSABI_FREEBSD = 9,        // FreeBSD
  ELFOSABI_TRU64 = 10,         // TRU64 UNIX
  ELFOSABI_MODESTO = 11,       // Novell Modesto
  ELFOSABI_OPENBSD = 12,       // OpenBSD
  ELFOSABI_OPENVMS = 13,       // OpenVMS
  ELFOSABI_NSK = 14,           // Hewlett-Packard Non-Stop Kernel
  ELFOSABI_AROS = 15,          // AROS
  ELFOSABI_FENIXOS = 16,       // FenixOS
  ELFOSABI_CLOUDABI = 17,      // Nuxi CloudABI
  ELFOSABI_FIRST_ARCH = 64,    // First architecture-specific OS ABI
  ELFOSABI_AMDGPU_HSA = 64,    // AMD HSA runtime
  ELFOSABI_AMDGPU_PAL = 65,    // AMD PAL runtime
  ELFOSABI_AMDGPU_MESA3D = 66, // AMD GCN GPUs (GFX6+) for MESA runtime
  ELFOSABI_ARM = 97,           // ARM
  ELFOSABI_C6000_ELFABI = 64,  // Bare-metal TMS320C6000
  ELFOSABI_C6000_LINUX = 65,   // Linux TMS320C6000
  ELFOSABI_STANDALONE = 255,   // Standalone (embedded) application
  ELFOSABI_LAST_ARCH = 255     // Last Architecture-specific OS ABI
};

#define ELF_RELOC(name, value) name = value,

// Postrisc Specific e_flags
enum {
  EF_POSTRISC_NOREORDER = 0x00000001, // Don't reorder instructions
  EF_POSTRISC_PIC       = 0x00000002, // Position independent code
  EF_POSTRISC_ARCH_32   = 0x50000000, // POSTRISC-32 instruction set per linux not elf.h
  EF_POSTRISC_ARCH      = 0xf0000000, // Mask for applying EF_POSTRISC_ARCH_ variant
};

// ELF Relocation types for Postrisc
enum {
   XXXXXXXXXXXXXXXXXXXXXX
// #include "ELFRelocs/Postrisc.def"
};

#undef ELF_RELOC

// Section header.
struct Elf32_Shdr {
  Elf32_Word sh_name;      // Section name (index into string table)
  Elf32_Word sh_type;      // Section type (SHT_*)
  Elf32_Word sh_flags;     // Section flags (SHF_*)
  Elf32_Addr sh_addr;      // Address where section is to be loaded
  Elf32_Off sh_offset;     // File offset of section data, in bytes
  Elf32_Word sh_size;      // Size of section, in bytes
  Elf32_Word sh_link;      // Section type-specific header table index link
  Elf32_Word sh_info;      // Section type-specific extra information
  Elf32_Word sh_addralign; // Section address alignment
  Elf32_Word sh_entsize;   // Size of records contained within the section
};

// Section header for ELF64 - same fields as ELF32, different types.
struct Elf64_Shdr {
  Elf64_Word sh_name;
  Elf64_Word sh_type;
  Elf64_Xword sh_flags;
  Elf64_Addr sh_addr;
  Elf64_Off sh_offset;
  Elf64_Xword sh_size;
  Elf64_Word sh_link;
  Elf64_Word sh_info;
  Elf64_Xword sh_addralign;
  Elf64_Xword sh_entsize;
};

// Special section indices.
enum {
  SHN_UNDEF = 0,          // Undefined, missing, irrelevant, or meaningless
  SHN_LORESERVE = 0xff00, // Lowest reserved index
  SHN_LOPROC = 0xff00,    // Lowest processor-specific index
  SHN_HIPROC = 0xff1f,    // Highest processor-specific index
  SHN_LOOS = 0xff20,      // Lowest operating system-specific index
  SHN_HIOS = 0xff3f,      // Highest operating system-specific index
  SHN_ABS = 0xfff1,       // Symbol has absolute value; does not need relocation
  SHN_COMMON = 0xfff2,    // FORTRAN COMMON or C external global variables
  SHN_XINDEX = 0xffff,    // Mark that the index is >= SHN_LORESERVE
  SHN_HIRESERVE = 0xffff  // Highest reserved index
};

// Section types.
enum : unsigned {
  SHT_NULL = 0,           // No associated section (inactive entry).
  SHT_PROGBITS = 1,       // Program-defined contents.
  SHT_SYMTAB = 2,         // Symbol table.
  SHT_STRTAB = 3,         // String table.
  SHT_RELA = 4,           // Relocation entries; explicit addends.
  SHT_HASH = 5,           // Symbol hash table.
  SHT_DYNAMIC = 6,        // Information for dynamic linking.
  SHT_NOTE = 7,           // Information about the file.
  SHT_NOBITS = 8,         // Data occupies no space in the file.
  SHT_REL = 9,            // Relocation entries; no explicit addends.
  SHT_SHLIB = 10,         // Reserved.
  SHT_DYNSYM = 11,        // Symbol table.
  SHT_INIT_ARRAY = 14,    // Pointers to initialization functions.
  SHT_FINI_ARRAY = 15,    // Pointers to termination functions.
  SHT_PREINIT_ARRAY = 16, // Pointers to pre-init functions.
  SHT_GROUP = 17,         // Section group.
  SHT_SYMTAB_SHNDX = 18,  // Indices for SHN_XINDEX entries.
  // Experimental support for SHT_RELR sections. For details, see proposal
  // at https://groups.google.com/forum/#!topic/generic-abi/bX460iggiKg
  SHT_RELR = 19,         // Relocation entries; only offsets.
  SHT_LOOS = 0x60000000, // Lowest operating system-specific type.
  // Android packed relocation section types.
  // https://android.googlesource.com/platform/bionic/+/6f12bfece5dcc01325e0abba56a46b1bcf991c69/tools/relocation_packer/src/elf_file.cc#37
  SHT_ANDROID_REL = 0x60000001,
  SHT_ANDROID_RELA = 0x60000002,
  SHT_LLVM_ODRTAB = 0x6fff4c00,         // LLVM ODR table.
  SHT_LLVM_LINKER_OPTIONS = 0x6fff4c01, // LLVM Linker Options.
  SHT_LLVM_ADDRSIG = 0x6fff4c03,        // List of address-significant symbols
                                        // for safe ICF.
  SHT_LLVM_DEPENDENT_LIBRARIES =
      0x6fff4c04,                  // LLVM Dependent Library Specifiers.
  SHT_LLVM_SYMPART = 0x6fff4c05,   // Symbol partition specification.
  SHT_LLVM_PART_EHDR = 0x6fff4c06, // ELF header for loadable partition.
  SHT_LLVM_PART_PHDR = 0x6fff4c07, // Phdrs for loadable partition.
  SHT_LLVM_BB_ADDR_MAP_V0 =
      0x6fff4c08, // LLVM Basic Block Address Map (old version kept for
                  // backward-compatibility).
  SHT_LLVM_CALL_GRAPH_PROFILE = 0x6fff4c09, // LLVM Call Graph Profile.
  SHT_LLVM_BB_ADDR_MAP = 0x6fff4c0a,        // LLVM Basic Block Address Map.
  SHT_LLVM_OFFLOADING = 0x6fff4c0b,         // LLVM device offloading data.
  // Android's experimental support for SHT_RELR sections.
  // https://android.googlesource.com/platform/bionic/+/b7feec74547f84559a1467aca02708ff61346d2a/libc/include/elf.h#512
  SHT_ANDROID_RELR = 0x6fffff00,   // Relocation entries; only offsets.
  SHT_GNU_ATTRIBUTES = 0x6ffffff5, // Object attributes.
  SHT_GNU_HASH = 0x6ffffff6,       // GNU-style hash table.
  SHT_GNU_verdef = 0x6ffffffd,     // GNU version definitions.
  SHT_GNU_verneed = 0x6ffffffe,    // GNU version references.
  SHT_GNU_versym = 0x6fffffff,     // GNU symbol versions table.
  SHT_HIOS = 0x6fffffff,           // Highest operating system-specific type.
  SHT_LOPROC = 0x70000000,         // Lowest processor arch-specific type.
  SHT_RISCV_ATTRIBUTES = 0x70000003U,
  SHT_HIPROC = 0x7fffffff, // Highest processor arch-specific type.
  SHT_LOUSER = 0x80000000, // Lowest type reserved for applications.
  SHT_HIUSER = 0xffffffff  // Highest type reserved for applications.
};

// Section flags.
enum : unsigned {
  // Section data should be writable during execution.
  SHF_WRITE = 0x1,

  // Section occupies memory during program execution.
  SHF_ALLOC = 0x2,

  // Section contains executable machine instructions.
  SHF_EXECINSTR = 0x4,

  // The data in this section may be merged.
  SHF_MERGE = 0x10,

  // The data in this section is null-terminated strings.
  SHF_STRINGS = 0x20,

  // A field in this section holds a section header table index.
  SHF_INFO_LINK = 0x40U,

  // Adds special ordering requirements for link editors.
  SHF_LINK_ORDER = 0x80U,

  // This section requires special OS-specific processing to avoid incorrect
  // behavior.
  SHF_OS_NONCONFORMING = 0x100U,

  // This section is a member of a section group.
  SHF_GROUP = 0x200U,

  // This section holds Thread-Local Storage.
  SHF_TLS = 0x400U,

  // Identifies a section containing compressed data.
  SHF_COMPRESSED = 0x800U,

  // This section should not be garbage collected by the linker.
  SHF_GNU_RETAIN = 0x200000,

  // This section is excluded from the final executable or shared library.
  SHF_EXCLUDE = 0x80000000U,

  // Start of target-specific flags.

  SHF_MASKOS = 0x0ff00000,

  // Solaris equivalent of SHF_GNU_RETAIN.
  SHF_SUNW_NODISCARD = 0x00100000,

  // Bits indicating processor-specific flags.
  SHF_MASKPROC = 0xf0000000,

  /// All sections with the "d" flag are grouped together by the linker to form
  /// the data section and the dp register is set to the start of the section by
  /// the boot code.
  XCORE_SHF_DP_SECTION = 0x10000000,

  /// All sections with the "c" flag are grouped together by the linker to form
  /// the constant pool and the cp register is set to the start of the constant
  /// pool by the boot code.
  XCORE_SHF_CP_SECTION = 0x20000000,

  // If an object file section does not have this flag set, then it may not hold
  // more than 2GB and can be freely referred to in objects using smaller code
  // models. Otherwise, only objects using larger code models can refer to them.
  // For example, a medium code model object can refer to data in a section that
  // sets this flag besides being able to refer to data in a section that does
  // not set it; likewise, a small code model object can refer only to code in a
  // section that does not set this flag.
  SHF_X86_64_LARGE = 0x10000000,

  // All sections with the GPREL flag are grouped into a global data area
  // for faster accesses
  SHF_HEX_GPREL = 0x10000000,

  // Section contains text/data which may be replicated in other sections.
  // Linker must retain only one copy.
  SHF_MIPS_NODUPES = 0x01000000,

  // Linker must generate implicit hidden weak names.
  SHF_MIPS_NAMES = 0x02000000,

  // Section data local to process.
  SHF_MIPS_LOCAL = 0x04000000,

  // Do not strip this section.
  SHF_MIPS_NOSTRIP = 0x08000000,

  // Section must be part of global data area.
  SHF_MIPS_GPREL = 0x10000000,

  // This section should be merged.
  SHF_MIPS_MERGE = 0x20000000,

  // Address size to be inferred from section entry size.
  SHF_MIPS_ADDR = 0x40000000,

  // Section data is string data by default.
  SHF_MIPS_STRING = 0x80000000,

  // Make code section unreadable when in execute-only mode
  SHF_ARM_PURECODE = 0x20000000
};

// Section Group Flags
enum : unsigned {
  GRP_COMDAT = 0x1,
  GRP_MASKOS = 0x0ff00000,
  GRP_MASKPROC = 0xf0000000
};

// Symbol table entries for ELF32.
struct Elf32_Sym {
  Elf32_Word st_name;     // Symbol name (index into string table)
  Elf32_Addr st_value;    // Value or address associated with the symbol
  Elf32_Word st_size;     // Size of the symbol
  unsigned char st_info;  // Symbol's type and binding attributes
  unsigned char st_other; // Must be zero; reserved
  Elf32_Half st_shndx;    // Which section (header table index) it's defined in

  // These accessors and mutators correspond to the ELF32_ST_BIND,
  // ELF32_ST_TYPE, and ELF32_ST_INFO macros defined in the ELF specification:
  unsigned char getBinding() const { return st_info >> 4; }
  unsigned char getType() const { return st_info & 0x0f; }
  void setBinding(unsigned char b) { setBindingAndType(b, getType()); }
  void setType(unsigned char t) { setBindingAndType(getBinding(), t); }
  void setBindingAndType(unsigned char b, unsigned char t) {
    st_info = (b << 4) + (t & 0x0f);
  }
};

// Symbol table entries for ELF64.
struct Elf64_Sym {
  Elf64_Word st_name;     // Symbol name (index into string table)
  unsigned char st_info;  // Symbol's type and binding attributes
  unsigned char st_other; // Must be zero; reserved
  Elf64_Half st_shndx;    // Which section (header tbl index) it's defined in
  Elf64_Addr st_value;    // Value or address associated with the symbol
  Elf64_Xword st_size;    // Size of the symbol

  // These accessors and mutators are identical to those defined for ELF32
  // symbol table entries.
  unsigned char getBinding() const { return st_info >> 4; }
  unsigned char getType() const { return st_info & 0x0f; }
  void setBinding(unsigned char b) { setBindingAndType(b, getType()); }
  void setType(unsigned char t) { setBindingAndType(getBinding(), t); }
  void setBindingAndType(unsigned char b, unsigned char t) {
    st_info = (b << 4) + (t & 0x0f);
  }
};

// The size (in bytes) of symbol table entries.
enum {
  SYMENTRY_SIZE32 = 16, // 32-bit symbol entry size
  SYMENTRY_SIZE64 = 24  // 64-bit symbol entry size.
};

// Symbol bindings.
enum {
  STB_LOCAL = 0,  // Local symbol, not visible outside obj file containing def
  STB_GLOBAL = 1, // Global symbol, visible to all object files being combined
  STB_WEAK = 2,   // Weak symbol, like global but lower-precedence
  STB_GNU_UNIQUE = 10,
  STB_LOOS = 10,   // Lowest operating system-specific binding type
  STB_HIOS = 12,   // Highest operating system-specific binding type
  STB_LOPROC = 13, // Lowest processor-specific binding type
  STB_HIPROC = 15  // Highest processor-specific binding type
};

// Symbol types.
enum {
  STT_NOTYPE = 0,     // Symbol's type is not specified
  STT_OBJECT = 1,     // Symbol is a data object (variable, array, etc.)
  STT_FUNC = 2,       // Symbol is executable code (function, etc.)
  STT_SECTION = 3,    // Symbol refers to a section
  STT_FILE = 4,       // Local, absolute symbol that refers to a file
  STT_COMMON = 5,     // An uninitialized common block
  STT_TLS = 6,        // Thread local data object
  STT_GNU_IFUNC = 10, // GNU indirect function
  STT_LOOS = 10,      // Lowest operating system-specific symbol type
  STT_HIOS = 12,      // Highest operating system-specific symbol type
  STT_LOPROC = 13,    // Lowest processor-specific symbol type
  STT_HIPROC = 15,    // Highest processor-specific symbol type

  // AMDGPU symbol types
  STT_AMDGPU_HSA_KERNEL = 10
};

enum {
  STV_DEFAULT = 0,  // Visibility is specified by binding type
  STV_INTERNAL = 1, // Defined by processor supplements
  STV_HIDDEN = 2,   // Not visible to other components
  STV_PROTECTED = 3 // Visible in other components but not preemptable
};

// Symbol number.
enum { STN_UNDEF = 0 };

// Special relocation symbols used in the MIPS64 ELF relocation entries
enum {
  RSS_UNDEF = 0, // None
  RSS_GP = 1,    // Value of gp
  RSS_GP0 = 2,   // Value of gp used to create object being relocated
  RSS_LOC = 3    // Address of location being relocated
};

// Relocation entry, without explicit addend.
struct Elf32_Rel {
  Elf32_Addr r_offset; // Location (file byte offset, or program virtual addr)
  Elf32_Word r_info;   // Symbol table index and type of relocation to apply

  // These accessors and mutators correspond to the ELF32_R_SYM, ELF32_R_TYPE,
  // and ELF32_R_INFO macros defined in the ELF specification:
  Elf32_Word getSymbol() const { return (r_info >> 8); }
  unsigned char getType() const { return (unsigned char)(r_info & 0x0ff); }
  void setSymbol(Elf32_Word s) { setSymbolAndType(s, getType()); }
  void setType(unsigned char t) { setSymbolAndType(getSymbol(), t); }
  void setSymbolAndType(Elf32_Word s, unsigned char t) {
    r_info = (s << 8) + t;
  }
};

// Relocation entry with explicit addend.
struct Elf32_Rela {
  Elf32_Addr r_offset;  // Location (file byte offset, or program virtual addr)
  Elf32_Word r_info;    // Symbol table index and type of relocation to apply
  Elf32_Sword r_addend; // Compute value for relocatable field by adding this

  // These accessors and mutators correspond to the ELF32_R_SYM, ELF32_R_TYPE,
  // and ELF32_R_INFO macros defined in the ELF specification:
  Elf32_Word getSymbol() const { return (r_info >> 8); }
  unsigned char getType() const { return (unsigned char)(r_info & 0x0ff); }
  void setSymbol(Elf32_Word s) { setSymbolAndType(s, getType()); }
  void setType(unsigned char t) { setSymbolAndType(getSymbol(), t); }
  void setSymbolAndType(Elf32_Word s, unsigned char t) {
    r_info = (s << 8) + t;
  }
};

// Relocation entry without explicit addend or info (relative relocations only).
typedef Elf32_Word Elf32_Relr; // offset/bitmap for relative relocations

// Relocation entry, without explicit addend.
struct Elf64_Rel {
  Elf64_Addr r_offset; // Location (file byte offset, or program virtual addr).
  Elf64_Xword r_info;  // Symbol table index and type of relocation to apply.

  // These accessors and mutators correspond to the ELF64_R_SYM, ELF64_R_TYPE,
  // and ELF64_R_INFO macros defined in the ELF specification:
  Elf64_Word getSymbol() const { return (r_info >> 32); }
  Elf64_Word getType() const { return (Elf64_Word)(r_info & 0xffffffffL); }
  void setSymbol(Elf64_Word s) { setSymbolAndType(s, getType()); }
  void setType(Elf64_Word t) { setSymbolAndType(getSymbol(), t); }
  void setSymbolAndType(Elf64_Word s, Elf64_Word t) {
    r_info = ((Elf64_Xword)s << 32) + (t & 0xffffffffL);
  }
};

// Relocation entry with explicit addend.
struct Elf64_Rela {
  Elf64_Addr r_offset; // Location (file byte offset, or program virtual addr).
  Elf64_Xword r_info;  // Symbol table index and type of relocation to apply.
  Elf64_Sxword r_addend; // Compute value for relocatable field by adding this.

  // These accessors and mutators correspond to the ELF64_R_SYM, ELF64_R_TYPE,
  // and ELF64_R_INFO macros defined in the ELF specification:
  Elf64_Word getSymbol() const { return (r_info >> 32); }
  Elf64_Word getType() const { return (Elf64_Word)(r_info & 0xffffffffL); }
  void setSymbol(Elf64_Word s) { setSymbolAndType(s, getType()); }
  void setType(Elf64_Word t) { setSymbolAndType(getSymbol(), t); }
  void setSymbolAndType(Elf64_Word s, Elf64_Word t) {
    r_info = ((Elf64_Xword)s << 32) + (t & 0xffffffffL);
  }
};

// Relocation entry without explicit addend or info (relative relocations only).
typedef Elf64_Xword Elf64_Relr; // offset/bitmap for relative relocations

// Program header for ELF32.
struct Elf32_Phdr {
  Elf32_Word p_type;   // Type of segment
  Elf32_Off p_offset;  // File offset where segment is located, in bytes
  Elf32_Addr p_vaddr;  // Virtual address of beginning of segment
  Elf32_Addr p_paddr;  // Physical address of beginning of segment (OS-specific)
  Elf32_Word p_filesz; // Num. of bytes in file image of segment (may be zero)
  Elf32_Word p_memsz;  // Num. of bytes in mem image of segment (may be zero)
  Elf32_Word p_flags;  // Segment flags
  Elf32_Word p_align;  // Segment alignment constraint
};

// Program header for ELF64.
struct Elf64_Phdr {
  Elf64_Word p_type;    // Type of segment
  Elf64_Word p_flags;   // Segment flags
  Elf64_Off p_offset;   // File offset where segment is located, in bytes
  Elf64_Addr p_vaddr;   // Virtual address of beginning of segment
  Elf64_Addr p_paddr;   // Physical addr of beginning of segment (OS-specific)
  Elf64_Xword p_filesz; // Num. of bytes in file image of segment (may be zero)
  Elf64_Xword p_memsz;  // Num. of bytes in mem image of segment (may be zero)
  Elf64_Xword p_align;  // Segment alignment constraint
};

// Segment types.
enum {
  PT_NULL = 0,            // Unused segment.
  PT_LOAD = 1,            // Loadable segment.
  PT_DYNAMIC = 2,         // Dynamic linking information.
  PT_INTERP = 3,          // Interpreter pathname.
  PT_NOTE = 4,            // Auxiliary information.
  PT_SHLIB = 5,           // Reserved.
  PT_PHDR = 6,            // The program header table itself.
  PT_TLS = 7,             // The thread-local storage template.
  PT_LOOS = 0x60000000,   // Lowest operating system-specific pt entry type.
  PT_HIOS = 0x6fffffff,   // Highest operating system-specific pt entry type.
  PT_LOPROC = 0x70000000, // Lowest processor-specific program hdr entry type.
  PT_HIPROC = 0x7fffffff, // Highest processor-specific program hdr entry type.

  // x86-64 program header types.
  // These all contain stack unwind tables.
  PT_GNU_EH_FRAME = 0x6474e550,
  PT_SUNW_EH_FRAME = 0x6474e550,
  PT_SUNW_UNWIND = 0x6464e550,

  PT_GNU_STACK = 0x6474e551,    // Indicates stack executability.
  PT_GNU_RELRO = 0x6474e552,    // Read-only after relocation.
  PT_GNU_PROPERTY = 0x6474e553, // .note.gnu.property notes sections.

  PT_OPENBSD_MUTABLE = 0x65a3dbe5,   // Like bss, but not immutable.
  PT_OPENBSD_RANDOMIZE = 0x65a3dbe6, // Fill with random data.
  PT_OPENBSD_WXNEEDED = 0x65a3dbe7,  // Program does W^X violations.
  PT_OPENBSD_BOOTDATA = 0x65a41be6,  // Section for boot arguments.

  // ARM program header types.
  PT_ARM_ARCHEXT = 0x70000000, // Platform architecture compatibility info
  // These all contain stack unwind tables.
  PT_ARM_EXIDX = 0x70000001,
  PT_ARM_UNWIND = 0x70000001,
  // MTE memory tag segment type
  PT_AARCH64_MEMTAG_MTE = 0x70000002,

  // MIPS program header types.
  PT_MIPS_REGINFO = 0x70000000,  // Register usage information.
  PT_MIPS_RTPROC = 0x70000001,   // Runtime procedure table.
  PT_MIPS_OPTIONS = 0x70000002,  // Options segment.
  PT_MIPS_ABIFLAGS = 0x70000003, // Abiflags segment.

  // RISCV program header types.
  PT_RISCV_ATTRIBUTES = 0x70000003,
};

// Segment flag bits.
enum : unsigned {
  PF_X = 1,                // Execute
  PF_W = 2,                // Write
  PF_R = 4,                // Read
  PF_MASKOS = 0x0ff00000,  // Bits for operating system-specific semantics.
  PF_MASKPROC = 0xf0000000 // Bits for processor-specific semantics.
};

// Dynamic table entry for ELF32.
struct Elf32_Dyn {
  Elf32_Sword d_tag; // Type of dynamic table entry.
  union {
    Elf32_Word d_val; // Integer value of entry.
    Elf32_Addr d_ptr; // Pointer value of entry.
  } d_un;
};

// Dynamic table entry for ELF64.
struct Elf64_Dyn {
  Elf64_Sxword d_tag; // Type of dynamic table entry.
  union {
    Elf64_Xword d_val; // Integer value of entry.
    Elf64_Addr d_ptr;  // Pointer value of entry.
  } d_un;
};

// Dynamic table entry tags.
enum {
#define DYNAMIC_TAG(name, value) DT_##name = value,
#define DYNAMIC_TAG_MARKER(name, value) DYNAMIC_TAG(name, value)

//#include "DynamicTags.def"
DYNAMIC_TAG(NULL, 0)        // Marks end of dynamic array.
DYNAMIC_TAG(NEEDED, 1)      // String table offset of needed library.
DYNAMIC_TAG(PLTRELSZ, 2)    // Size of relocation entries in PLT.
DYNAMIC_TAG(PLTGOT, 3)      // Address associated with linkage table.
DYNAMIC_TAG(HASH, 4)        // Address of symbolic hash table.
DYNAMIC_TAG(STRTAB, 5)      // Address of dynamic string table.
DYNAMIC_TAG(SYMTAB, 6)      // Address of dynamic symbol table.
DYNAMIC_TAG(RELA, 7)        // Address of relocation table (Rela entries).
DYNAMIC_TAG(RELASZ, 8)      // Size of Rela relocation table.
DYNAMIC_TAG(RELAENT, 9)     // Size of a Rela relocation entry.
DYNAMIC_TAG(STRSZ, 10)      // Total size of the string table.
DYNAMIC_TAG(SYMENT, 11)     // Size of a symbol table entry.
DYNAMIC_TAG(INIT, 12)       // Address of initialization function.
DYNAMIC_TAG(FINI, 13)       // Address of termination function.
DYNAMIC_TAG(SONAME, 14)     // String table offset of a shared objects name.
DYNAMIC_TAG(RPATH, 15)      // String table offset of library search path.
DYNAMIC_TAG(SYMBOLIC, 16)   // Changes symbol resolution algorithm.
DYNAMIC_TAG(REL, 17)        // Address of relocation table (Rel entries).
DYNAMIC_TAG(RELSZ, 18)      // Size of Rel relocation table.
DYNAMIC_TAG(RELENT, 19)     // Size of a Rel relocation entry.
DYNAMIC_TAG(PLTREL, 20)     // Type of relocation entry used for linking.
DYNAMIC_TAG(DEBUG, 21)      // Reserved for debugger.
DYNAMIC_TAG(TEXTREL, 22)    // Relocations exist for non-writable segments.
DYNAMIC_TAG(JMPREL, 23)     // Address of relocations associated with PLT.
DYNAMIC_TAG(BIND_NOW, 24)   // Process all relocations before execution.
DYNAMIC_TAG(INIT_ARRAY, 25) // Pointer to array of initialization functions.
DYNAMIC_TAG(FINI_ARRAY, 26) // Pointer to array of termination functions.
DYNAMIC_TAG(INIT_ARRAYSZ, 27) // Size of DT_INIT_ARRAY.
DYNAMIC_TAG(FINI_ARRAYSZ, 28) // Size of DT_FINI_ARRAY.
DYNAMIC_TAG(RUNPATH, 29)      // String table offset of lib search path.
DYNAMIC_TAG(FLAGS, 30)        // Flags.
DYNAMIC_TAG_MARKER(ENCODING, 32) // Values from here to DT_LOOS follow the rules
                                 // for the interpretation of the d_un union.

DYNAMIC_TAG(PREINIT_ARRAY, 32)   // Pointer to array of preinit functions.
DYNAMIC_TAG(PREINIT_ARRAYSZ, 33) // Size of the DT_PREINIT_ARRAY array.

DYNAMIC_TAG(SYMTAB_SHNDX, 34) // Address of the SHT_SYMTAB_SHNDX section.

// Experimental support for SHT_RELR sections. For details, see proposal
// at https://groups.google.com/forum/#!topic/generic-abi/bX460iggiKg
DYNAMIC_TAG(RELRSZ, 35)  // Size of Relr relocation table.
DYNAMIC_TAG(RELR, 36)    // Address of relocation table (Relr entries).
DYNAMIC_TAG(RELRENT, 37) // Size of a Relr relocation entry.

DYNAMIC_TAG_MARKER(LOOS, 0x60000000)   // Start of environment specific tags.
DYNAMIC_TAG_MARKER(HIOS, 0x6FFFFFFF)   // End of environment specific tags.
DYNAMIC_TAG_MARKER(LOPROC, 0x70000000) // Start of processor specific tags.
DYNAMIC_TAG_MARKER(HIPROC, 0x7FFFFFFF) // End of processor specific tags.

#undef DYNAMIC_TAG
};

// DT_FLAGS values.
enum {
  DF_ORIGIN = 0x01,    // The object may reference $ORIGIN.
  DF_SYMBOLIC = 0x02,  // Search the shared lib before searching the exe.
  DF_TEXTREL = 0x04,   // Relocations may modify a non-writable segment.
  DF_BIND_NOW = 0x08,  // Process all relocations on load.
  DF_STATIC_TLS = 0x10 // Reject attempts to load dynamically.
};

// State flags selectable in the `d_un.d_val' element of the DT_FLAGS_1 entry.
enum {
  DF_1_NOW = 0x00000001,       // Set RTLD_NOW for this object.
  DF_1_GLOBAL = 0x00000002,    // Set RTLD_GLOBAL for this object.
  DF_1_GROUP = 0x00000004,     // Set RTLD_GROUP for this object.
  DF_1_NODELETE = 0x00000008,  // Set RTLD_NODELETE for this object.
  DF_1_LOADFLTR = 0x00000010,  // Trigger filtee loading at runtime.
  DF_1_INITFIRST = 0x00000020, // Set RTLD_INITFIRST for this object.
  DF_1_NOOPEN = 0x00000040,    // Set RTLD_NOOPEN for this object.
  DF_1_ORIGIN = 0x00000080,    // $ORIGIN must be handled.
  DF_1_DIRECT = 0x00000100,    // Direct binding enabled.
  DF_1_TRANS = 0x00000200,
  DF_1_INTERPOSE = 0x00000400,  // Object is used to interpose.
  DF_1_NODEFLIB = 0x00000800,   // Ignore default lib search path.
  DF_1_NODUMP = 0x00001000,     // Object can't be dldump'ed.
  DF_1_CONFALT = 0x00002000,    // Configuration alternative created.
  DF_1_ENDFILTEE = 0x00004000,  // Filtee terminates filters search.
  DF_1_DISPRELDNE = 0x00008000, // Disp reloc applied at build time.
  DF_1_DISPRELPND = 0x00010000, // Disp reloc applied at run-time.
  DF_1_NODIRECT = 0x00020000,   // Object has no-direct binding.
  DF_1_IGNMULDEF = 0x00040000,
  DF_1_NOKSYMS = 0x00080000,
  DF_1_NOHDR = 0x00100000,
  DF_1_EDITED = 0x00200000, // Object is modified after built.
  DF_1_NORELOC = 0x00400000,
  DF_1_SYMINTPOSE = 0x00800000, // Object has individual interposers.
  DF_1_GLOBAUDIT = 0x01000000,  // Global auditing required.
  DF_1_SINGLETON = 0x02000000,  // Singleton symbols are used.
  DF_1_PIE = 0x08000000,        // Object is a position-independent executable.
};

// DT_MIPS_FLAGS values.
enum {
  RHF_NONE = 0x00000000,                   // No flags.
  RHF_QUICKSTART = 0x00000001,             // Uses shortcut pointers.
  RHF_NOTPOT = 0x00000002,                 // Hash size is not a power of two.
  RHS_NO_LIBRARY_REPLACEMENT = 0x00000004, // Ignore LD_LIBRARY_PATH.
  RHF_NO_MOVE = 0x00000008,                // DSO address may not be relocated.
  RHF_SGI_ONLY = 0x00000010,               // SGI specific features.
  RHF_GUARANTEE_INIT = 0x00000020,         // Guarantee that .init will finish
                                           // executing before any non-init
                                           // code in DSO is called.
  RHF_DELTA_C_PLUS_PLUS = 0x00000040,      // Contains Delta C++ code.
  RHF_GUARANTEE_START_INIT = 0x00000080,   // Guarantee that .init will start
                                           // executing before any non-init
                                           // code in DSO is called.
  RHF_PIXIE = 0x00000100,                  // Generated by pixie.
  RHF_DEFAULT_DELAY_LOAD = 0x00000200,     // Delay-load DSO by default.
  RHF_REQUICKSTART = 0x00000400,           // Object may be requickstarted
  RHF_REQUICKSTARTED = 0x00000800,         // Object has been requickstarted
  RHF_CORD = 0x00001000,                   // Generated by cord.
  RHF_NO_UNRES_UNDEF = 0x00002000,         // Object contains no unresolved
                                           // undef symbols.
  RHF_RLD_ORDER_SAFE = 0x00004000          // Symbol table is in a safe order.
};

// ElfXX_VerDef structure version (GNU versioning)
enum { VER_DEF_NONE = 0, VER_DEF_CURRENT = 1 };

// VerDef Flags (ElfXX_VerDef::vd_flags)
enum { VER_FLG_BASE = 0x1, VER_FLG_WEAK = 0x2, VER_FLG_INFO = 0x4 };

// Special constants for the version table. (SHT_GNU_versym/.gnu.version)
enum {
  VER_NDX_LOCAL = 0,       // Unversioned local symbol
  VER_NDX_GLOBAL = 1,      // Unversioned global symbol
  VERSYM_VERSION = 0x7fff, // Version Index mask
  VERSYM_HIDDEN = 0x8000   // Hidden bit (non-default version)
};

// ElfXX_VerNeed structure version (GNU versioning)
enum { VER_NEED_NONE = 0, VER_NEED_CURRENT = 1 };

// SHT_NOTE section types.

// Generic note types.
enum : unsigned {
  NT_VERSION = 1,
  NT_ARCH = 2,
  NT_GNU_BUILD_ATTRIBUTE_OPEN = 0x100,
  NT_GNU_BUILD_ATTRIBUTE_FUNC = 0x101,
};

// Core note types.
enum : unsigned {
  NT_PRSTATUS = 1,
  NT_FPREGSET = 2,
  NT_PRPSINFO = 3,
  NT_TASKSTRUCT = 4,
  NT_AUXV = 6,
  NT_PSTATUS = 10,
  NT_FPREGS = 12,
  NT_PSINFO = 13,
  NT_LWPSTATUS = 16,
  NT_LWPSINFO = 17,
  NT_WIN32PSTATUS = 18,

  NT_PPC_VMX = 0x100,
  NT_PPC_VSX = 0x102,
  NT_PPC_TAR = 0x103,
  NT_PPC_PPR = 0x104,
  NT_PPC_DSCR = 0x105,
  NT_PPC_EBB = 0x106,
  NT_PPC_PMU = 0x107,
  NT_PPC_TM_CGPR = 0x108,
  NT_PPC_TM_CFPR = 0x109,
  NT_PPC_TM_CVMX = 0x10a,
  NT_PPC_TM_CVSX = 0x10b,
  NT_PPC_TM_SPR = 0x10c,
  NT_PPC_TM_CTAR = 0x10d,
  NT_PPC_TM_CPPR = 0x10e,
  NT_PPC_TM_CDSCR = 0x10f,

  NT_386_TLS = 0x200,
  NT_386_IOPERM = 0x201,
  NT_X86_XSTATE = 0x202,

  NT_S390_HIGH_GPRS = 0x300,
  NT_S390_TIMER = 0x301,
  NT_S390_TODCMP = 0x302,
  NT_S390_TODPREG = 0x303,
  NT_S390_CTRS = 0x304,
  NT_S390_PREFIX = 0x305,
  NT_S390_LAST_BREAK = 0x306,
  NT_S390_SYSTEM_CALL = 0x307,
  NT_S390_TDB = 0x308,
  NT_S390_VXRS_LOW = 0x309,
  NT_S390_VXRS_HIGH = 0x30a,
  NT_S390_GS_CB = 0x30b,
  NT_S390_GS_BC = 0x30c,

  NT_ARM_VFP = 0x400,
  NT_ARM_TLS = 0x401,
  NT_ARM_HW_BREAK = 0x402,
  NT_ARM_HW_WATCH = 0x403,
  NT_ARM_SVE = 0x405,
  NT_ARM_PAC_MASK = 0x406,
  NT_ARM_SSVE = 0x40b,
  NT_ARM_ZA = 0x40c,
  NT_ARM_ZT = 0x40d,

  NT_FILE = 0x46494c45,
  NT_PRXFPREG = 0x46e62b7f,
  NT_SIGINFO = 0x53494749,
};

// LLVM-specific notes.
enum {
  NT_LLVM_HWASAN_GLOBALS = 3,
};

// GNU note types.
enum {
  NT_GNU_ABI_TAG = 1,
  NT_GNU_HWCAP = 2,
  NT_GNU_BUILD_ID = 3,
  NT_GNU_GOLD_VERSION = 4,
  NT_GNU_PROPERTY_TYPE_0 = 5,
  FDO_PACKAGING_METADATA = 0xcafe1a7e,
};

// Memory tagging values used in NT_ANDROID_TYPE_MEMTAG notes.
enum {
  // Enumeration to determine the tagging mode. In Android-land, 'SYNC' means
  // running all threads in MTE Synchronous mode, and 'ASYNC' means to use the
  // kernels auto-upgrade feature to allow for either MTE Asynchronous,
  // Asymmetric, or Synchronous mode. This allows silicon vendors to specify, on
  // a per-cpu basis what 'ASYNC' should mean. Generally, the expectation is
  // "pick the most precise mode that's very fast".
  NT_MEMTAG_LEVEL_NONE = 0,
  NT_MEMTAG_LEVEL_ASYNC = 1,
  NT_MEMTAG_LEVEL_SYNC = 2,
  NT_MEMTAG_LEVEL_MASK = 3,
  // Bits indicating whether the loader should prepare for MTE to be enabled on
  // the heap and/or stack.
  NT_MEMTAG_HEAP = 4,
  NT_MEMTAG_STACK = 8,
};

// Property types used in GNU_PROPERTY_TYPE_0 notes.
enum : unsigned {
  GNU_PROPERTY_STACK_SIZE = 1,
  GNU_PROPERTY_NO_COPY_ON_PROTECTED = 2,
  GNU_PROPERTY_AARCH64_FEATURE_1_AND = 0xc0000000,
  GNU_PROPERTY_X86_FEATURE_1_AND = 0xc0000002,

  GNU_PROPERTY_X86_UINT32_OR_LO = 0xc0008000,
  GNU_PROPERTY_X86_FEATURE_2_NEEDED = GNU_PROPERTY_X86_UINT32_OR_LO + 1,
  GNU_PROPERTY_X86_ISA_1_NEEDED = GNU_PROPERTY_X86_UINT32_OR_LO + 2,

  GNU_PROPERTY_X86_UINT32_OR_AND_LO = 0xc0010000,
  GNU_PROPERTY_X86_FEATURE_2_USED = GNU_PROPERTY_X86_UINT32_OR_AND_LO + 1,
  GNU_PROPERTY_X86_ISA_1_USED = GNU_PROPERTY_X86_UINT32_OR_AND_LO + 2,
};

// aarch64 processor feature bits.
enum : unsigned {
  GNU_PROPERTY_AARCH64_FEATURE_1_BTI = 1 << 0,
  GNU_PROPERTY_AARCH64_FEATURE_1_PAC = 1 << 1,
};

enum {
  GNU_ABI_TAG_LINUX = 0,
  GNU_ABI_TAG_HURD = 1,
  GNU_ABI_TAG_SOLARIS = 2,
  GNU_ABI_TAG_FREEBSD = 3,
  GNU_ABI_TAG_NETBSD = 4,
  GNU_ABI_TAG_SYLLABLE = 5,
  GNU_ABI_TAG_NACL = 6,
};

constexpr const char *ELF_NOTE_GNU = "GNU";

// Android packed relocation group flags.
enum {
  RELOCATION_GROUPED_BY_INFO_FLAG = 1,
  RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG = 2,
  RELOCATION_GROUPED_BY_ADDEND_FLAG = 4,
  RELOCATION_GROUP_HAS_ADDEND_FLAG = 8,
};

// Compressed section header for ELF32.
struct Elf32_Chdr {
  Elf32_Word ch_type;
  Elf32_Word ch_size;
  Elf32_Word ch_addralign;
};

// Compressed section header for ELF64.
struct Elf64_Chdr {
  Elf64_Word ch_type;
  Elf64_Word ch_reserved;
  Elf64_Xword ch_size;
  Elf64_Xword ch_addralign;
};

// Note header for ELF32.
struct Elf32_Nhdr {
  Elf32_Word n_namesz;
  Elf32_Word n_descsz;
  Elf32_Word n_type;
};

// Note header for ELF64.
struct Elf64_Nhdr {
  Elf64_Word n_namesz;
  Elf64_Word n_descsz;
  Elf64_Word n_type;
};

// Legal values for ch_type field of compressed section header.
enum {
  ELFCOMPRESS_ZLIB = 1,            // ZLIB/DEFLATE algorithm.
  ELFCOMPRESS_ZSTD = 2,            // Zstandard algorithm
  ELFCOMPRESS_LOOS = 0x60000000,   // Start of OS-specific.
  ELFCOMPRESS_HIOS = 0x6fffffff,   // End of OS-specific.
  ELFCOMPRESS_LOPROC = 0x70000000, // Start of processor-specific.
  ELFCOMPRESS_HIPROC = 0x7fffffff  // End of processor-specific.
};

} // end namespace postrisc
