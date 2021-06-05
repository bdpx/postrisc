#pragma once

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
    frame_base_alignment      = 1,    // FIXME: make 2 later
    forward_offset_alignment  = 1,    // (ip + 4 * imm) for ldafr, ldNr, stNr
};

/***************************************************************************
* instruction field sizes in bits.
* we assume that each field is strictly continuous bit sequence
* (no extensions from this rule for now).
***************************************************************************/
enum EInstructionSize : u8 {
    // base
    bits_per_opcode               = 7, // 128 main opcodes maximum
    bits_per_register             = 7, // 128 general registers
    bits_per_shamt                = 6, // log2(64 bit) shift amount
    bits_per_vshamt               = 7,
    // derived
    bits_per_pal                  = bits_per_slot - bits_per_opcode,
    bits_per_unknown              = bits_per_pal,
    bits_per_call                 = bits_per_pal - 1 * bits_per_register, // 28 bits
    bits_per_binimm               = bits_per_pal - 2 * bits_per_register, // 21 bits
    bits_per_fused_opx            = bits_per_pal - 4 * bits_per_register, // 7 bits extended opcode
    bits_per_misc_opx             = 11, // 2048 codes extended codes maximum
    bits_per_misc_opx_null        = 2 * bits_per_register - bits_per_misc_opx, // 4
    bits_per_update_stride        = bits_per_register + bits_per_misc_opx_null, // 11
    bits_per_indexed_scale        = 3,
    bits_per_indexed_scale_mode   = 2, // (32|64+sign|zero)
    bits_per_indexed_disp         = 2 * bits_per_register - bits_per_indexed_scale - bits_per_indexed_scale_mode, // 9
    bits_per_branch               = 17, // +/- 1 Mib intra-function branch
    bits_per_branch_opx           = bits_per_binimm - bits_per_branch, // 4 bit
    bits_per_framesize            = bits_per_register, // 7
    bits_per_brci_imm             = bits_per_call - bits_per_branch, // 12
    bits_per_nullify_dist         = 5,
    bits_per_misc_noarg           = 3 * bits_per_register + bits_per_misc_opx_null, // 24 bits
    bits_per_rounding_mode        = 3, // 4 FP rounding modes (NEAREST,+INF,-INF,ZERO) + dynamic rounding mode
    bits_per_memory_order         = 3,
    bits_per_nullify_future       = 1 << bits_per_nullify_dist, // 2**5 = 32
};

static_assert(bits_per_fused_opx == bits_per_register);
static_assert(bits_per_misc_opx_null == bits_per_rounding_mode);
static_assert(bits_per_misc_opx_null == bits_per_memory_order);

// second slot extensions
enum EInstructionExtensionSize : u8 {
    bits_per_call_ext               = 64 - log_bytes_per_bundle - bits_per_call,
    bits_per_ldi_ext                = 64 - bits_per_call,  // bits_per_slot is too enough
    bits_per_binimm_ext             = bits_per_slot,
    bits_per_brci_imm_ext           = 40 - bits_per_brci_imm,
    bits_per_branch_ext             = bits_per_slot - bits_per_brci_imm_ext,
    bits_per_indexed_disp_ext       = bits_per_slot,
};

enum EInstructionFullSize : u8 {
    bits_per_call_long               = bits_per_call             + bits_per_call_ext,
    bits_per_branch_long             = bits_per_branch           + bits_per_branch_ext,
    bits_per_ldi_long                = bits_per_call             + bits_per_ldi_ext,
    bits_per_binimm_long             = bits_per_binimm           + bits_per_binimm_ext,
    bits_per_brci_imm_long           = bits_per_brci_imm         + bits_per_brci_imm_ext,
    bits_per_indexed_disp_long       = bits_per_indexed_disp     + bits_per_indexed_disp_ext,
};

/***************************************************************************
* instruction field displacements in bits.
***************************************************************************/
enum EInstructionOffset : u8 {
    disp_binimm                 = 0,
    disp_unknown                = 0,
    disp_pal                    = 0,
    disp_call                   = 0,
    disp_misc_opx               = 0,
    disp_fused_opx              = 0,
    disp_opcode                 = bits_per_fused_opx + 4 * bits_per_register,
    disp_reg_a                  = bits_per_fused_opx + 3 * bits_per_register,
    disp_reg_b                  = bits_per_fused_opx + 2 * bits_per_register,
    disp_reg_c                  = bits_per_fused_opx + 1 * bits_per_register,
    disp_reg_d                  = bits_per_fused_opx + 0 * bits_per_register,
    disp_reg_e                  = 0,
    disp_framesize              = disp_reg_b,
    disp_indexed_disp           = 0,
    disp_indexed_scale_mode     = bits_per_indexed_disp,
    disp_indexed_scale          = disp_indexed_scale_mode + bits_per_indexed_scale_mode,
    disp_rounding_mode          = bits_per_misc_opx,
    disp_update_stride          = bits_per_misc_opx,
    disp_memory_order           = bits_per_misc_opx,
    disp_branch                 = disp_binimm,
    disp_branch_opx             = disp_branch + bits_per_branch,
    disp_brci_imm               = disp_branch_opx,
    disp_nullify_dist_yes       = disp_fused_opx + bits_per_fused_opx,
    disp_nullify_dist_no        = disp_nullify_dist_yes + bits_per_nullify_dist,
};

// second slot extensions
enum EInstructionExtensionOffset : u8 {
    disp_call_ext              = 0,
    disp_ldi_ext               = 0,
    disp_binimm_ext            = 0,
    disp_branch_ext            = 0,
    // compare reg-imm and branch
    disp_brci_imm_ext          = disp_branch_ext + bits_per_branch_ext,
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
    bytes_per_sector            = 64, // section/segment file alignment
// special regs
    bits_per_privilegy          = 2,    // 4 levels (rings) of privilegy
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
    num_static_regs             = 8,
    max_stacked_framesize       = (1 << bits_per_register) - num_static_regs,
    base_static_regs            = max_stacked_framesize,
    base_rotate_regs            = 0,
// link partitions
    bits_per_frame_size         = bits_per_register,
    bits_per_eip_offset         = 32 - bits_per_frame_size - bits_per_frame_size,
// implementation dependent cyclic register buffer
    phys_stacked_regs           = 128
};

} // namespace postrisc
