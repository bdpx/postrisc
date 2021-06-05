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
    forward_offset_alignment  = 1,    // (ip + imm) for ca.rf, ld.N.r, st.N.r
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
    bits_per_shamt                = 7, // log2(128 bit) shift amount
    // derived
    bits_per_pal                  = bits_per_slot - bits_per_opcode,
    bits_per_unknown              = bits_per_pal,
    bits_per_call                 = bits_per_pal - 1 * bits_per_register, // 28 bits
    bits_per_binimm               = bits_per_pal - 2 * bits_per_register, // 21 bits
    bits_per_lda_near_shift       = bits_per_binimm - 1, // 20 bits, for lda_n
    bits_per_fused_opx            = bits_per_pal - 4 * bits_per_register, // 7 bits extended opcode
    bits_per_mid_binimm           = 2 * bits_per_register, // 14 bits
    bits_per_misc_opx             = 11, // 2048 codes extended codes maximum
    bits_per_misc_opx_null        = 2 * bits_per_register - bits_per_misc_opx, // 3
    bits_per_update_stride        = bits_per_register + bits_per_misc_opx_null, // 10
    bits_per_indexed_scale        = 3,
    bits_per_indexed_opx          = 4,
    bits_per_indexed_disp         = 2 * bits_per_register - bits_per_indexed_scale - bits_per_indexed_opx, // 7
    bits_per_branch               = 17, // +/- 1 Mib intra-function branch
    bits_per_branch_opx           = bits_per_binimm - bits_per_branch, // 4 bit
    bits_per_framesize            = bits_per_register, // 7
    bits_per_brci_imm             = bits_per_call - bits_per_branch, // 11
    bits_per_loop_stride          = 6,
    bits_per_loop_disp            = bits_per_branch - bits_per_loop_stride,
    bits_per_nullify_dist         = 5,
    bits_per_misc_noarg           = 3 * bits_per_register + bits_per_misc_opx_null, // 24 bits
    bits_per_memory_order         = 3,
    bits_per_nullify_future       = 1 << bits_per_nullify_dist, // 2**5 = 32
};

static_assert(bits_per_fused_opx == bits_per_register);
static_assert(bits_per_misc_opx_null == bits_per_memory_order);

// second slot extensions
enum EInstructionExtensionSize : u8 {
    bits_per_call_ext               = 64 - log_bytes_per_bundle - bits_per_call,
    bits_per_ld_imm_ext             = 64 - bits_per_call,  // bits_per_slot is too enough
    bits_per_binimm_ext             = bits_per_slot,
    bits_per_mid_binimm_ext         = bits_per_slot,
    bits_per_brci_imm_ext           = 40 - bits_per_brci_imm,
    bits_per_branch_ext             = bits_per_slot - bits_per_brci_imm_ext,
    bits_per_indexed_disp_ext       = bits_per_slot,
    bits_per_loop_stride_ext        = 23,
    bits_per_loop_disp_ext          = bits_per_slot - bits_per_loop_stride_ext,
};

enum EInstructionFullSize : u8 {
    bits_per_call_long               = bits_per_call             + bits_per_call_ext,
    bits_per_branch_long             = bits_per_branch           + bits_per_branch_ext,
    bits_per_ld_imm_long             = bits_per_call             + bits_per_ld_imm_ext,
    bits_per_binimm_long             = bits_per_binimm           + bits_per_binimm_ext,
    bits_per_mid_binimm_long         = bits_per_mid_binimm       + bits_per_mid_binimm_ext,
    bits_per_brci_imm_long           = bits_per_brci_imm         + bits_per_brci_imm_ext,
    bits_per_indexed_disp_long       = bits_per_indexed_disp     + bits_per_indexed_disp_ext,
    bits_per_loop_stride_long        = bits_per_loop_stride      + bits_per_loop_stride_ext,
    bits_per_loop_disp_long          = bits_per_loop_disp        + bits_per_loop_disp_ext,
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
    disp_mid_binimm             = bits_per_fused_opx,
    disp_framesize              = disp_reg_b,
    disp_indexed_disp           = 0,
    disp_indexed_opx            = bits_per_indexed_disp,
    disp_indexed_scale          = disp_indexed_opx + bits_per_indexed_opx,
    disp_update_stride          = bits_per_misc_opx,
    disp_memory_order           = bits_per_misc_opx,
    disp_branch                 = disp_binimm,
    disp_branch_opx             = disp_branch + bits_per_branch,
    disp_brci_imm               = disp_branch_opx,
    disp_loop_disp              = 0,
    disp_loop_stride            = disp_loop_disp + bits_per_loop_disp,
    disp_nullify_dist_yes       = disp_fused_opx + bits_per_fused_opx,
    disp_nullify_dist_no        = disp_nullify_dist_yes + bits_per_nullify_dist,
};

// second slot extensions
enum EInstructionExtensionOffset : u8 {
    disp_call_ext              = 0,
    disp_ld_imm_ext            = 0,
    disp_binimm_ext            = 0,
    disp_mid_binimm_ext        = 0,
    disp_branch_ext            = 0,
    // compare reg-imm and branch
    disp_brci_imm_ext          = disp_branch_ext + bits_per_branch_ext,
    disp_indexed_disp_ext      = 0,
    disp_loop_disp_ext         = 0,
    disp_loop_stride_ext       = disp_loop_disp_ext + bits_per_loop_disp_ext,
};


/***************************************************************************
* addressing & special register fields
***************************************************************************/
enum {
    bits_per_region_id          = 24,
    bits_per_minpageoffset      = 12,    // log2(4 KiB)
    bits_per_maxpageoffset      = 16,    // log2(64 KiB)
// virtual memory
    bits_per_pagesize           = 6, // log2(64 bit),
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

static_assert(max_stacked_framesize == 120);

#define STACKED_REGS(X)                                                   \
    X(  0) X(  1) X(  2) X(  3) X(  4) X(  5) X(  6) X(  7) X(  8) X(  9) \
    X( 10) X( 11) X( 12) X( 13) X( 14) X( 15) X( 16) X( 17) X( 18) X( 19) \
    X( 20) X( 21) X( 22) X( 23) X( 24) X( 25) X( 26) X( 27) X( 28) X( 29) \
    X( 30) X( 31) X( 32) X( 33) X( 34) X( 35) X( 36) X( 37) X( 38) X( 39) \
    X( 40) X( 41) X( 42) X( 43) X( 44) X( 45) X( 46) X( 47) X( 48) X( 49) \
    X( 50) X( 51) X( 52) X( 53) X( 54) X( 55) X( 56) X( 57) X( 58) X( 59) \
    X( 60) X( 61) X( 62) X( 63) X( 64) X( 65) X( 66) X( 67) X( 68) X( 69) \
    X( 70) X( 71) X( 72) X( 73) X( 74) X( 75) X( 76) X( 77) X( 78) X( 79) \
    X( 80) X( 81) X( 82) X( 83) X( 84) X( 85) X( 86) X( 87) X( 88) X( 89) \
    X( 90) X( 91) X( 92) X( 93) X( 94) X( 95) X( 96) X( 97) X( 98) X( 99) \
    X(100) X(101) X(102) X(103) X(104) X(105) X(106) X(107) X(108) X(109) \
    X(110) X(111) X(112) X(113) X(114) X(115) X(116) X(117) X(118) X(119) \

static_assert(num_static_regs == 8);

enum {
    reg_g0    = 120,
    reg_g1    = 121,
    reg_g2    = 122,
    reg_g3    = 123,
    reg_tp    = 124,    // thread pointer
    reg_fp    = 125,    // frame pointer
    reg_sp    = 126,    // stack pointer
    reg_gz    = 127,    // zero register
};

#define STATIC_REGS(X) X(g0) X(g1) X(g2) X(g3) X(tp) X(fp) X(sp) X(gz)

} // namespace postrisc
