#pragma once

/***************************************************************************
* This file contain all references to
* Application Binary Interface (abi)
***************************************************************************/

namespace postrisc {

typedef struct _segment_info_t {
    u32    file_size;
    u32    memory_size;
    u32    flags;
    u32    name_offset;
} segment_info_t;

enum {
    ei_class      = 0,    // 32 or 64 bit
    ei_data       = 1,    // lsb or msb
    ei_version    = 2,    // file version
    ei_osabi      = 3,    // os version
    ei_abiversion = 4     // abi version
};

enum {
    ei_class_32         = 1,
    ei_class_64         = 2,
    ei_data_2lsb        = 1,    // 2's complement, little endian
    ei_data_2msb        = 2,    // 2's complement, big endian
    ei_version_current  = 1,    // file version
    ei_osabi_sysv       = 1,    // unix system v
    ei_abiversion_sysv  = 1     // unix system v abi version
};

/**************************************************************************
* isa relocation insertion types for linker
**************************************************************************/
enum ERelocationType {
    reloc_none,              // reserved zero index
    reloc_word,              // data 4-byte pointer
    reloc_dword,             // data 8-byte pointer
    reloc_ldi,               // ldi (23 bit check)
    reloc_ldi_long,          // ldi (long)
    reloc_branch,            // br,bpr,ca.r(text) (27 bit, 16-byte align check)
    reloc_branch_long,       // br,bpr,ca.r(text) (long)
    reloc_callr,             // callr(text) (27 bit, 16-byte align check)
    reloc_callr_long,        // callr(text) (long)
    reloc_binimm,            // binimm,load/store (16 bit, check)
    reloc_binimm_long,       // binimm,load/store (57 bit, check)
    reloc_hmem,              // load/store (16 bit, check, 2-byte align)
    reloc_hmem_lo,           // load/store (16 bit, nocheck, 2-byte align)
    reloc_hmem_long,         // load/store (57 bit, check, 2-byte align)
    reloc_brc_simm,          // compare-and-branch (signed 7 bit)
    reloc_brc_simm_long,     // compare-and-branch (signed 32 bit)
    reloc_brc_branch,        // compare-and-branch (16 bit, 16-byte align)
    reloc_brc_branch_long    // compare-and-branch (32 bit, 16-byte align)
};


/**************************************************************************
* isa relocation value types for linker
**************************************************************************/
enum {
    reloc_addr,       // absolute address
    reloc_got,        // absolute got entry
    reloc_gprel,      // gp-relative data
    reloc_tprel,      // tp-relative tdata
    reloc_pcrel,      // text,rodata (16-byte aligned)
    reloc_ltoff,      // gp-relative got
    reloc_pltoff,     // gp-relative fptr
    reloc_tltoff,     // tp-relative got
    reloc_segrel,     // segment-relative
    reloc_secrel,     // section-relative
    reloc_baserel     // 
};

/**************************************************************************
* abi relocation entry types for future linker
**************************************************************************/

enum {
// default (empty) relocation
    r_none,

// absolute address (data)
    r_addr_word,
    r_addr_dword,

// absolute address (instruction only, kernel only)
    r_addr_ldi,
    r_addr_ldi_long,
    r_addr_binimm,
    r_addr_binimm_long,
    r_addr_binimm_hi,
    r_addr_binimm_hi_long,
    r_addr_binimm_lo,

// pc-relative offset (instruction only)
    r_pcrel_branch,
    r_pcrel_branch_long,
    r_pcrel_brc_branch,
    r_pcrel_brc_branch_long,
    r_pcrel_ca_r,
    r_pcrel_ca_r_long,

// gp-relative offset (instruction only)
    r_gprel_ldi,
    r_gprel_ldi_long,
    r_gprel_binimm,
    r_gprel_binimm_long,
    r_gprel_binimm_hi,
    r_gprel_binimm_hi_long,
    r_gprel_binimm_lo,

// gp-relative got-indirect offset (instruction only, doubleword only)
    r_ltoff_ldi,
    r_ltoff_ldi_long,
    r_ltoff_binimm_hi,
    r_ltoff_binimm_hi_long,
    r_ltoff_binimm_lo,
    r_ltoff_binimm,
    r_ltoff_binimm_long,

// tp-relative offset (instruction only)
    r_tprel_ldi,
    r_tprel_ldi_long,
    r_tprel_binimm,
    r_tprel_binimm_long,
    r_tprel_binimm_hi,
    r_tprel_binimm_hi_long,
    r_tprel_binimm_lo,

// segment-base relavive offset
    r_segrel_word,
    r_segrel_dword,
    r_segrel_ldi,
    r_segrel_ldi_long,
    r_segrel_binimm,
    r_segrel_binimm_long,
    r_segrel_binimm_hi,
    r_segrel_binimm_hi_long,
    r_segrel_binimm_lo,

// section-base relavive offset
    r_secrel_word,
    r_secrel_dword,
    r_secrel_ldi,
    r_secrel_ldi_long,
    r_secrel_binimm,
    r_secrel_binimm_long,
    r_secrel_binimm_hi,
    r_secrel_binimm_hi_long,
    r_secrel_binimm_lo
};

} // namespace postrisc
