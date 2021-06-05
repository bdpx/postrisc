#pragma once

#include "util/common.hpp"

namespace postrisc {

enum {
    bits_per_bit              = 1,
    bits_per_byte             = 8,
    bits_per_address          = 64,
    log_bytes_per_address     = 3,    // log2(8)
    bytes_per_bundle          = 16,
    log_bytes_per_bundle      = 4,    // log2(16)
    bits_per_bundle_template  = 2,    // 128 % 3
    bits_per_slot             = 42,   // 128 / 3
    nibbles_per_slot          = 11,   // 42/4 round up
    slots_per_bundle          = 3,
    bytes_per_vector          = 16,   // vector register length
};

/***************************************************************************
* instruction field sizes in bits.
* we assume that each field is strictly continuous bit sequence
* (no extensions from this rule for now).
***************************************************************************/
enum EInstructionSize : uint8_t {
// base
    bits_per_opcode             = 7,    // 128 main opcodes maximum
    bits_per_register           = 7,    // 128 general/floating-point/special registers
    bits_per_shamt              = 6,    // log2(64 bit) shift amount
    bits_per_vshamt             = 7,
// derived
    bits_per_unknown                    = bits_per_slot - bits_per_opcode,
    bits_per_pal                        = bits_per_slot - bits_per_opcode,
    bits_per_branch                     = bits_per_pal - 1 * bits_per_register,  // 28 bits
    bits_per_binimm                     = bits_per_pal - 2 * bits_per_register,  // 21 bits
    bits_per_fused_opx                  = bits_per_slot - bits_per_opcode - 4 * bits_per_register,  // 7 bits extended opcode (128 codes maximum)
    bits_per_misc_opx                   = 9, // 1024 codes extended codes maximum
    bits_per_misc_opx_null              = 5,
    bits_per_nullify_opx                = 4,
    bits_per_update_stride              = 2 * bits_per_register,
    bits_per_indexed_scale              = 3,
    bits_per_indexed_scale_zero         = bits_per_misc_opx_null - bits_per_indexed_scale,
    bits_per_indexed_disp               = bits_per_misc_opx,
    bits_per_brc_branch                 = bits_per_binimm,        // 21 bits
    bits_per_loop_step                  = bits_per_register,
    // compare reg-imm and branch
    bits_per_brci_imm                   = 14,                                    // immediate in place for rb+rc
    bits_per_short_branch               = bits_per_branch - bits_per_brci_imm,   // 14 bits
    bits_per_ldan_shift                 = bits_per_binimm - 1,                   // 20 bits - for nearest +/-1MiB region
    // conditional nullification
    bits_per_nullify_dist               = 5,
    bits_per_nop                        = 3 * bits_per_register,
    bits_per_fp_compare_mode            = 5,  // bits: LT,GT,EQ,UN,SIG
    bits_per_fp_compare_mode_reserved   = bits_per_register - bits_per_fp_compare_mode, // 2
    bits_per_memory_ordering            = 2,  // now 4 memory orderings for atomic RMW operations (relaxed, acquire, release, acquire_release)
    bits_per_memory_ordering_zero       = bits_per_misc_opx_null - bits_per_memory_ordering,
    bits_per_rounding_mode              = 3,  // 4 explicit floating-point rounding modes - NEAREST,+INF,-INF,ZERO, and dynamic rounding mode
    bits_per_rounding_mode_zero         = bits_per_misc_opx_null - bits_per_rounding_mode,
    bits_per_clmul_index                = 2,  // high/low switch for first and second args
};

// second slot extensions
enum EInstructionExtensionSize : uint8_t {
    bits_per_branch_ext             = 64 - log_bytes_per_bundle - bits_per_branch,
    bits_per_ldi_ext                = 64 - bits_per_branch,  // bits_per_slot is too enough
    bits_per_binimm_ext             = bits_per_slot,
    // compare reg-reg and branch
    bits_per_brc_branch_ext         = 64 - log_bytes_per_bundle - bits_per_brc_branch,
    // compare reg-imm and branch
    bits_per_brci_imm_ext           = 40 - bits_per_brci_imm,
    bits_per_short_branch_ext       = bits_per_slot - bits_per_brci_imm_ext,
    bits_per_indexed_disp_ext       = bits_per_slot,
};

enum EInstructionFullSize : uint8_t {
    bits_per_branch_long             = bits_per_branch           + bits_per_branch_ext,
    bits_per_ldi_long                = bits_per_branch           + bits_per_ldi_ext,
    bits_per_binimm_long             = bits_per_binimm           + bits_per_binimm_ext,
    bits_per_brc_branch_long         = bits_per_brc_branch       + bits_per_brc_branch_ext,
    bits_per_short_branch_long       = bits_per_short_branch     + bits_per_short_branch_ext,
    bits_per_brci_imm_long           = bits_per_brci_imm         + bits_per_brci_imm_ext,
    bits_per_indexed_disp_long       = bits_per_indexed_disp     + bits_per_indexed_disp_ext,
};

/***************************************************************************
* instruction field displacements in bits.
***************************************************************************/
enum EInstructionOffset : uint8_t {
    disp_binimm            = 0,
    disp_unknown           = 0,
    disp_pal               = 0,
    disp_branch            = 0,
    disp_opx               = 0,
    disp_opcode            = bits_per_fused_opx + 4 * bits_per_register,
    disp_reg_a             = bits_per_fused_opx + 3 * bits_per_register,
    disp_reg_b             = bits_per_fused_opx + 2 * bits_per_register,
    disp_reg_c             = bits_per_fused_opx + 1 * bits_per_register,
    disp_reg_d             = bits_per_fused_opx + 0 * bits_per_register,
    disp_reg_e             = 0,
    disp_indexed_disp      = 0,
    disp_indexed_scale     = bits_per_indexed_disp,
    disp_rounding_mode     = disp_indexed_scale,
    disp_update_stride     = bits_per_register,
    disp_fp_compare_mode   = disp_indexed_scale,
    disp_brc_branch        = disp_binimm,
    disp_loop_step         = disp_reg_c,
    // compare reg-imm and branch
    disp_short_branch      = disp_binimm,
    disp_brci_imm          = disp_short_branch + bits_per_short_branch,
    // conditional nullification
    disp_nullify_dist_yes  = bits_per_nullify_opx,
    disp_nullify_dist_no   = disp_nullify_dist_yes + bits_per_nullify_dist,
    disp_memory_ordering   = disp_indexed_scale
};

// second slot extensions
enum EInstructionExtensionOffset : uint8_t {
    disp_branch_ext            = 0,
    disp_ldi_ext               = 0,
    disp_binimm_ext            = 0,
    // compare reg-reg and branch
    disp_brc_branch_ext        = 0,
    // compare reg-imm and branch
    disp_short_branch_ext      = 0,
    disp_brci_imm_ext          = disp_short_branch_ext + bits_per_short_branch_ext,
    disp_indexed_disp_ext      = 0,
};


/***************************************************************************
* addressing & special register fields
***************************************************************************/
enum {
    bits_per_region_id          = 24,
    bits_per_minpageoffset      = 12,    // log2(4 KiB)
    bits_per_maxpageoffset      = 16,    // log2(64 KiB)
// virtual memory
    bits_per_pagesize           = bits_per_shamt,
    bits_per_ppn                = 40 - bits_per_minpageoffset,
    bits_per_vpn                = bits_per_ppn + 8,
    bits_per_translation        = 5,    // log2(32)
    bits_per_minpagetable       = bits_per_minpageoffset + 4,
    bits_per_maxpagetable       = bits_per_ppn + bits_per_translation + 3,
// module ABI
    bits_per_segment_offset     = bits_per_maxpageoffset, // 64 KiB for maximum page size
    bytes_per_sector            = 512,
// special regs
    bits_per_privilegy          = 2,    // 4 levels (rings) of privilegy
    bits_per_precision_control  = 2,    // 4 precisions - single,double,extended,quadruple
    bits_per_access             = 3,    // 8 access modes
    bits_per_memory_attribute   = 4,
    bits_per_slot_num           = 2,    // slot number - 0,1,2 (value 3 is reserved)
// monitor registers
    bits_per_monitor_counter    = 48,
    bits_per_monitor_event      = 8,
// implementation dependent
    bits_per_phys_address       = 50,    // max is 64 bits
    bits_per_phys_mem_attrib    = 4,     // part of non-memory addressing as 1 / (2**pp)
// architecture dependent regs partition
    num_static_regs             = 16,
    max_stacked_framesize       = (1 << bits_per_register) - num_static_regs,
    base_static_regs            = max_stacked_framesize,
    base_rotate_regs            = 0,
// implementation dependent cyclic register buffer
    phys_stacked_regs           = 128
};

} // namespace postrisc
