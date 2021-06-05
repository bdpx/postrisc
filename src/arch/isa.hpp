#pragma once

/***************************************************************************
* this file contain all references to the
* virtual processor instruction set architecture (ISA)
***************************************************************************/

#include "util/common.hpp"
#include "Bundle.hpp"

namespace postrisc {

/***************************************************************************
*
* macrotable for opcodes with extended codes
* each opcode in this table must have its own macrotable:
*    declare_*_table
* where '*' is opcode name
*
***************************************************************************/
#define DECLARE_OPCODES_WITH_EXTENDED_CODES(X) \
    X(misc) X(fused) X(fma) X(raopx) X(loop) \
    X(br_misc) X(br_32) X(br_64) X(br_128) \
    X(nul_misc) X(nul_32) X(nul_64) X(nul_128) \
    X(mem_xi64) X(mem_xu64) X(mem_xi32) X(mem_xu32)

enum EInstructionID : u16 {
#define X(name, subset, format, descr) insn_##name,
#include "insn_table.hpp"
#undef X
// another mnemonics provided for opcodes naming only
#define INSN_X(NAME) insn_##NAME,
    DECLARE_OPCODES_WITH_EXTENDED_CODES(INSN_X)
#undef INSN_X
    insn_illegal,
    insn_reserved, // for array sizes
    insn_last = insn_misc, // first with extended codes
};

inline std::ostream& operator<<(std::ostream& out, EInstructionID eid)
{
    return out << static_cast<std::underlying_type<EInstructionID>::type>(eid);
}

/***************************************************************************
*
*  macrotable for common instruction field types
*  X(1, 2, 3)
*    1 - enum name (like inst_fieldtype_##name)
*    2 - css class-name (like opc,opx,gpr) for html documentation
*    3 - help description
*  used only for instruction format color highlighting
*
***************************************************************************/
#define DECLARE_INST_FIELD_TYPES(X) \
   X (opc,   "primary opcode") \
   X (opx,   "extended opcode") \
   X (gpr,   "general-purpose register number") \
   X (spr,   "special-purpose register number") \
   X (imm,   "immediate constant") \
   X (tdb,   "shift (bit count)") \
   X (mod,   "modifier") \
   X (res,   "reserved (must be zero)") \

enum EInstructionFieldKind : u8 {
#define INST_FIELDTYPE_ENUM_X(name, descr) inst_fieldtype_##name,
    DECLARE_INST_FIELD_TYPES(INST_FIELDTYPE_ENUM_X)
    inst_fieldtype_last
};

/***************************************************************************
*
*  macrotable for distinct instruction fields
*  X(1, 2, 3, 4, 5, 6)
*    1 - enum name
*    2 - size in bits
*    3 - displacement in bits (or some other if pseudofield)
*    4 - field type (like inst_fieldtype_##name)
*    5 - short description used for instruction format
*    6 - help description
*
***************************************************************************/
#define DECLARE_INSTRUCTION_FIELDS(X) \
  X (none,                     bits_per_unknown,                  disp_unknown,             opx,    "unknown",    "end-marker for field-list") \
  X (argsdelimiter,            bits_per_unknown,                  disp_unknown,             opx,    "",           "args delimiter") \
  X (comma,                    bits_per_unknown,                  disp_unknown,             opx,    ",",          "comma") \
  X (comma_opt,                bits_per_unknown,                  disp_unknown,             opx,    ",",          "optional comma") \
  X (lprefix,                  bits_per_unknown,                  disp_unknown,             opx,    "l",          "long prefix") \
  X (opcode,                   bits_per_opcode,                   disp_opcode,              opc,    "opcode",     "main opcode") \
  X (gpra,                     bits_per_register,                 disp_reg_a,               gpr,    "ra",         "general register in 1 position") \
  X (gprb,                     bits_per_register,                 disp_reg_b,               gpr,    "rb",         "general register in 2 position") \
  X (gprc,                     bits_per_register,                 disp_reg_c,               gpr,    "rc",         "general register in 3 position") \
  X (gprd,                     bits_per_register,                 disp_reg_d,               gpr,    "rd",         "general register in 4 position") \
  X (spr,                      bits_per_register,                 disp_reg_c,               spr,    "spr",        "special purpose register in 3 position") \
  X (ld_imm,                   bits_per_call,                     disp_call,                imm,    "simm28",     "signed immediate for load in register") \
  X (ld_imm_f64,               bits_per_call,                     disp_call,                imm,    "fp64",       "f64 inline immediate") \
  X (ld_imm_f32,               bits_per_call,                     disp_call,                imm,    "fp32",       "f32 inline immediate") \
  X (bin_simm,                 bits_per_binimm,                   disp_binimm,              imm,    "simm21",     "signed immediate for binary") \
  X (bin_uimm,                 bits_per_binimm,                   disp_binimm,              imm,    "uimm21",     "unsigned immediate for binary") \
  X (mid_bin_simm,             bits_per_mid_binimm,               disp_mid_binimm,          imm,    "simm14",     "signed immediate for binary") \
  X (res21,                    bits_per_binimm,                   disp_binimm,              res,    "0",          "reserved") \
  X (indexed_disp,             bits_per_indexed_disp,             disp_indexed_disp,        imm,    "simm7",      "displacement for indexed addressing") \
  X (indexed_scale,            bits_per_indexed_scale,            disp_indexed_scale,       mod,    "scale",      "scale for indexed addressing") \
  X (indexed_opx,              bits_per_indexed_opx,              disp_indexed_opx,         opx,    "opx",        "indexed extended opcode") \
  X (brc_simm,                 bits_per_brci_imm,                 disp_brci_imm,            imm,    "simm11",     "signed immediate for compare-and-branch") \
  X (brc_uimm,                 bits_per_brci_imm,                 disp_brci_imm,            imm,    "uimm11",     "unsigned immediate for compare-and-branch") \
  X (loop_stride,              bits_per_loop_stride,              disp_loop_stride,         imm,    "uimm6",      "loop stride (+1)") \
  X (loop_disp,                bits_per_loop_disp,                disp_loop_disp,           imm,    "simm11x16",  "loop disp") \
  X (rip_label,                bits_per_call,                     disp_call,                imm,    "uimm28",     "forward ip-relative displacement") \
  X (rip_string,               bits_per_call,                     disp_call,                imm,    "uimm28",     "label in rodata as string") \
  X (rip_float32,              bits_per_call,                     disp_call,                imm,    "fp32",       "forward ip-relative displacement (rodata immediate single)") \
  X (rip_float64,              bits_per_call,                     disp_call,                imm,    "fp64",       "forward ip-relative displacement (rodata immediate double)") \
  X (rip_float128,             bits_per_call,                     disp_call,                imm,    "fp128",      "forward ip-relative displacement (rodata immediate quadruple)") \
  X (call,                     bits_per_call,                     disp_call,                imm,    "simm28x16",  "call label text") \
  X (branch,                   bits_per_branch,                   disp_branch,              imm,    "simm17x16",  "middle branch label") \
  X (branch_opx,               bits_per_branch_opx,               disp_branch_opx,          opx,    "opx",        "extended opcode for branch (4 bit)") \
  X (branch_opx_res,           bits_per_branch_opx,               disp_branch_opx,          res,    "0",          "field should be zero") \
  X (lda_r,                    bits_per_call,                     disp_call,                imm,    "simm28x16",  "long label text+rodata") \
  X (framesize,                bits_per_framesize,                disp_framesize,           imm,    "framesize",  "register window frame size") \
  X (shamt_b,                  bits_per_shamt,                    disp_reg_b,               tdb,    "sb",         "shift value in 2 position") \
  X (shamt_c,                  bits_per_shamt,                    disp_reg_c,               tdb,    "sc",         "shift value in 3 position") \
  X (shamt_d,                  bits_per_shamt,                    disp_reg_d,               tdb,    "sd",         "shift value in 4 position") \
  X (shamt_e,                  bits_per_shamt,                    disp_reg_e,               tdb,    "se",         "shift value in 5 position") \
  X (mask_yes,                 bits_per_nullify_dist,             disp_nullify_dist_yes,    imm,    "dy",         "dist-yes") \
  X (mask_no,                  bits_per_nullify_dist,             disp_nullify_dist_no,     imm,    "dn",         "dist-not") \
  X (fused_opx,                bits_per_fused_opx,                disp_fused_opx,           opx,    "opx",        "extended opcode for fused (7)") \
  X (misc_opx,                 bits_per_misc_opx,                 disp_misc_opx,            opx,    "opx",        "extended opcode for misc (10)") \
  X (raopx,                    bits_per_register,                 disp_reg_a,               opx,    "opx",        "extended opcode for opx7 (raopx)") \
  X (update_stride,            bits_per_update_stride,            disp_update_stride,       imm,    "simm10",     "signed immediate stride for base update") \
  X (stride_simm,              bits_per_update_stride,            disp_update_stride,       imm,    "simm10",     "signed immediate for atomic load/store") \
  X (unused_reg,               bits_per_register,                 disp_unknown,             res,    "0",          "field should be zero") \
  X (misc_opx_null,            bits_per_misc_opx_null,            disp_misc_opx,            res,    "0",          "field should be zero") \
  X (misc_noarg,               bits_per_misc_noarg,               disp_indexed_scale,       res,    "0",          "field should be zero") \



enum EInstructionField : u8 {
#define INSTRUCTION_FIELD_ENUM_X(field, size, disp, fieldtype, inst_name, descr) cf_##field,
    DECLARE_INSTRUCTION_FIELDS(INSTRUCTION_FIELD_ENUM_X)
    cf_last
};

/***************************************************************************
*
* macros for embedding arrays in other macros
* cf_none is special end-marker for variable-size lists
*
***************************************************************************/
#define VARRAY(...) { __VA_ARGS__, cf_none }

/***************************************************************************
*
* assembler instructions syntax templates
*
* X(name, fields)
*    1 enumeration name (like asm_fmt_##name)
*    2 array of arguments and delimiters
*
****************************************************************************/
#define DECLARE_ASM_FORMATS(X) \
  X (ld_imm,         VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_ld_imm)) \
  X (ld_imm_f32,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_ld_imm_f32)) \
  X (ld_imm_f64,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_ld_imm_f64)) \
  X (nop,            VARRAY(cf_lprefix, cf_argsdelimiter, cf_ld_imm)) \
  X (cache_simm,     VARRAY(cf_lprefix, cf_argsdelimiter,                    cf_gprb, cf_comma, cf_bin_simm)) \
  X (retf,           VARRAY(cf_lprefix, cf_argsdelimiter,                                       cf_bin_uimm)) \
  X (bin_simm,       VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_simm)) \
  X (bin_uimm,       VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_uimm)) \
  X (mid_bin_simm,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_mid_bin_simm)) \
  X (br_eh,          VARRAY(cf_lprefix, cf_argsdelimiter,                    cf_gprb, cf_comma, cf_branch)) \
  X (br_rr,          VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_branch)) \
  X (br_rs,          VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_shamt_b, cf_comma, cf_branch)) \
  X (br_simm,        VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_simm, cf_comma, cf_branch)) \
  X (br_uimm,        VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_uimm, cf_comma, cf_branch)) \
  X (loop,           VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_loop_stride, cf_comma, cf_loop_disp)) \
  X (call,           VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_call)) \
  X (jmp,            VARRAY(cf_lprefix, cf_argsdelimiter,                    cf_call)) \
  X (lda_r,          VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_lda_r)) \
  X (mem_iprel,      VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_label)) \
  X (write,          VARRAY(cf_lprefix, cf_argsdelimiter,                    cf_rip_string)) \
  X (ld_iprel_f32,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_float32)) \
  X (ld_iprel_f64,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_float64)) \
  X (ld_iprel_f128,  VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_float128)) \
  X (gmemx,          VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_indexed_scale, cf_comma, cf_indexed_disp)) \
  X (RbcScale,       VARRAY(            cf_argsdelimiter,                    cf_gprb, cf_comma, cf_gprc, cf_comma, cf_indexed_scale)) \
  X (gmemu,          VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_update_stride)) \
  X (RabS10,         VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_stride_simm)) \
  X (int,            VARRAY(            cf_argsdelimiter,                    cf_gprb, cf_comma, cf_stride_simm)) \
  X (r4,             VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc,    cf_comma, cf_gprd)) \
  X (r3s1,           VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc,    cf_comma, cf_shamt_d)) \
  X (r2s2,           VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c, cf_comma, cf_shamt_d)) \
  X (r3s2,           VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc,    cf_comma, cf_shamt_d, cf_comma, cf_shamt_e)) \
  X (r3,             VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc)) \
  X (r2s1,           VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c)) \
  X (r2,             VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb)) \
  X (Rbc,            VARRAY(            cf_argsdelimiter, cf_gprb, cf_comma, cf_gprc)) \
  X (alloc,          VARRAY(            cf_argsdelimiter, cf_framesize)) \
  X (alloc_sp,       VARRAY(cf_lprefix, cf_argsdelimiter, cf_framesize, cf_comma, cf_bin_uimm)) \
  X (nul_simm,       VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_simm, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (nul_uimm,       VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_uimm, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (nul_rr,         VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb,     cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (nul_rs,         VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_shamt_b,  cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (mspr,           VARRAY(            cf_argsdelimiter, cf_gpra, cf_comma, cf_spr)) \
  X (NoArgs,         VARRAY(                              cf_none)) \


enum EInstructionAssemblerFormat : u8 {
#define ASM_FORMAT_ENUM_X(name, fields) asm_fmt_##name,
    DECLARE_ASM_FORMATS(ASM_FORMAT_ENUM_X)
    asm_fmt_last
};

extern const std::array<std::array<EInstructionField, 16>, asm_fmt_last> asm_fmt_field;

/***************************************************************************
*
*    mashine instruction formats macrotable
*
*    def_instr_fmt(name, extname, bit-string, fields-array, description)
*        1 format name enumeration (like inst_fmt_##name)
*        2 extended format name enumeration (like inst_fmt_ext_##name)
*        3 bits representation for binary dumping
*        4 array of fields
*        5 description
*
*    order of bits:
*        msb------------------------------------lsb
*        443333333333222222222211111111110000000000
*        109876543210987654321098765432109876543210
*        ooooooo___________________________________
*                    see table for this bits
*
***************************************************************************/
#define DECLARE_INSTRUCTION_FORMATS(X) \
  X (ld_imm,       ld_imm,    "oooooooaaaaaaaiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_ld_imm), "load immediate") \
  X (call,         call,      "oooooooaaaaaaapppppppppppppppppppppppppppp", VARRAY(cf_gpra, cf_call), "call/load ip-relative address") \
  X (mem_iprel,    mem_iprel, "oooooooaaaaaaaiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_rip_label), "load/store ip-relative") \
  X (write,        mem_iprel, "oooooooxxxxxxxiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_rip_label), "write") \
  X (jmp,          call,      "oooooooxxxxxxxiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_call), "jump ip-relative") \
  X (nop,          ld_imm,    "oooooooxxxxxxxiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_ld_imm), "br/write ip-relative") \
  X (alloc,        none,      "oooooooxxxxxxxfffffff---------------------", VARRAY(cf_raopx, cf_framesize, cf_res21), "alloc") \
  X (alloc_sp,     bin_imm,   "oooooooxxxxxxxfffffffiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_framesize, cf_bin_uimm), "allocsp") \
  X (retf,         bin_imm,   "oooooooxxxxxxx-------iiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_unused_reg, cf_bin_uimm), "retf") \
  X (cache_simm,   bin_imm,   "oooooooxxxxxxxbbbbbbbiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_gprb, cf_bin_simm), "Rb with signed imm") \
  X (bin_simm,     bin_imm,   "oooooooaaaaaaabbbbbbbiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_gprb, cf_bin_simm), "binary register with signed imm") \
  X (bin_uimm,     bin_imm,   "oooooooaaaaaaabbbbbbbiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_gprb, cf_bin_uimm), "binary register with unsigned imm") \
  X (loop,         loop,      "oooooooxxxxxxxbbbbbbbxxxxiiiiiippppppppppp", VARRAY(cf_gpra, cf_gprb, cf_branch_opx, cf_loop_stride, cf_loop_disp), "loop") \
  X (br_eh,        br_rr,     "oooooooxxxxxxxbbbbbbb----ppppppppppppppppp", VARRAY(cf_raopx, cf_gprb, cf_branch_opx_res, cf_branch), "eh_adj") \
  X (br_rr,        br_rr,     "oooooooaaaaaaabbbbbbbxxxxppppppppppppppppp", VARRAY(cf_gpra, cf_gprb, cf_branch_opx, cf_branch), "[inc/dec], compare registers, branch") \
  X (br_rs,        br_rr,     "oooooooaaaaaaasssssssxxxxppppppppppppppppp", VARRAY(cf_gpra, cf_shamt_b, cf_branch_opx, cf_branch), "compare bit and branch") \
  X (br_simm,      br_imm,    "oooooooaaaaaaaiiiiiiiiiiippppppppppppppppp", VARRAY(cf_gpra, cf_brc_simm, cf_branch), "compare register with signed imm and branch") \
  X (br_uimm,      br_imm,    "oooooooaaaaaaaiiiiiiiiiiippppppppppppppppp", VARRAY(cf_gpra, cf_brc_uimm, cf_branch), "compare register with unsigned imm and branch") \
  X (nul_simm,     nul_imm,   "oooooooaaaaaaaiiiiiiiiiiinnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_brc_simm, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-simm nullify") \
  X (nul_uimm,     nul_imm,   "oooooooaaaaaaaiiiiiiiiiiinnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_brc_uimm, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-uimm nullify") \
  X (nul_rs,       none,      "oooooooaaaaaaasssssssxxxxnnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_shamt_b, cf_branch_opx, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-shift nullify") \
  X (nul_rr,       none,      "oooooooaaaaaaabbbbbbbxxxxnnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_branch_opx, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-reg nullify") \
  X (mid_bin_simm, bin_imm,   "oooooooaaaaaaabbbbbbbiiiiiiiiiiiiiixxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_mid_bin_simm, cf_fused_opx), "binary register with signed imm") \
  X (r4,           none,      "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_gprd, cf_fused_opx), "add/subtract with carry out") \
  X (r3s1,         none,      "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_shamt_d, cf_fused_opx), "shift variable and mask(clear)") \
  X (r2s2,         none,      "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_shamt_c, cf_shamt_d, cf_fused_opx), "shift fixed and mask(clear)") \
  X (r3s2,         none,      "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_shamt_d, cf_shamt_e), "bit-field insert") \
  X (gmemx,        gmemx,     "oooooooaaaaaaabbbbbbbcccccccsssxxxxiiiiiii", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_indexed_scale, cf_indexed_opx, cf_indexed_disp), "scaled index load/store") \
  X (RbcScale,     none,      "ooooooo-------bbbbbbbcccccccsssxxxxxxxxxxx", VARRAY(cf_unused_reg, cf_gprb, cf_gprc, cf_indexed_scale, cf_misc_opx), "scaled indexed jump") \
  X (Rbc,          none,      "ooooooo-------bbbbbbbccccccc---xxxxxxxxxxx", VARRAY(cf_unused_reg, cf_gprb, cf_gprc, cf_misc_opx_null, cf_misc_opx), "two args, no result") \
  X (mspr,         none,      "oooooooaaaaaaa-------kkkkkkk---xxxxxxxxxxx", VARRAY(cf_gpra, cf_unused_reg, cf_spr, cf_misc_opx_null, cf_misc_opx), "get/set special-purpose register") \
  X (r2,           none,      "oooooooaaaaaaabbbbbbb----------xxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_unused_reg, cf_misc_opx_null, cf_misc_opx), "unary") \
  X (r2s1,         none,      "oooooooaaaaaaabbbbbbbsssssss---xxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_shamt_c, cf_misc_opx_null, cf_misc_opx), "binary with shift amount") \
  X (r3,           none,      "oooooooaaaaaaabbbbbbbccccccc---xxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_misc_opx_null, cf_misc_opx), "binary register-register") \
  X (gmemu,        none,      "oooooooaaaaaaabbbbbbbiiiiiiiiiixxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_stride_simm, cf_misc_opx), "general [base with update] load/store") \
  X (int,          none,      "ooooooo-------bbbbbbbiiiiiiiiiixxxxxxxxxxx", VARRAY(cf_unused_reg, cf_gprb, cf_stride_simm, cf_misc_opx), "interrupt") \
  X (NoArgs,       none,      "ooooooo------------------------xxxxxxxxxxx", VARRAY(cf_misc_noarg, cf_misc_opx), "instruction without parameters") \

enum EInstructionFormat : u8 {
#define INSTRUCTION_FORMAT_ENUM_X(name, extname, descr, format, txt) inst_fmt_##name,
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_ENUM_X)
    inst_fmt_last,
    // pseudo for alternate assembler formats
    inst_fmt_lda_r          = inst_fmt_call,
    inst_fmt_RabS10         = inst_fmt_gmemu,
    inst_fmt_ld_imm_f32     = inst_fmt_ld_imm,
    inst_fmt_ld_imm_f64     = inst_fmt_ld_imm,
    inst_fmt_ld_iprel_f32   = inst_fmt_mem_iprel,
    inst_fmt_ld_iprel_f64   = inst_fmt_mem_iprel,
    inst_fmt_ld_iprel_f128  = inst_fmt_mem_iprel,
};

/***************************************************************************
*
*    mashine extended instruction formats macrotable
*
*    X(name, bit-string, description)
*        1 enum name (like inst_ext_fmt_##name)
*        2 bits representation for binary dumping
*        3 description
*
***************************************************************************/
#define DECLARE_INSTRUCTION_EXTENSION_FORMATS(X) \
  X (none,     "------------------------------------------",  "no extension") \
  X (ld_imm,   "------iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",  "64(28+36) bit ld_imm imm") \
  X (mem_iprel,"------pppppppppppppppppppppppppppppppppppp",  "64(28+36) bit relative forward offset") \
  X (call,     "----------pppppppppppppppppppppppppppppppp",  "60(28+32) bit relative offset") \
  X (bin_imm,  "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",  "63(21+42) bit imm") \
  X (gmemx,    "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",  "49(7+42) bit imm") \
  X (br_imm,   "iiiiiiiiiiiiiiiiiiiiiiiiiiiiippppppppppppp",  "40(11+29) bit imm, 30(17+13) bit branch offset") \
  X (nul_imm,  "iiiiiiiiiiiiiiiiiiiiiiiiiiiii-------------",  "40(11+29) bit imm, no branch (nullification)") \
  X (br_rr,    "-----------------------------ppppppppppppp",  "no imm, 30(17+13) bit branch offset") \
  X (loop,     "iiiiiiiiiiiiiiiiiiiiiiippppppppppppppppppp",  "29(6+23) bit imm, 30(11+19) bit branch offset") \

enum EInstructionExtensionFormat : u8 {
#define INSTRUCTION_EXTENSION_FORMAT_ENUM_X(name, bitdescr, txt) inst_ext_fmt_##name,
    DECLARE_INSTRUCTION_EXTENSION_FORMATS(INSTRUCTION_EXTENSION_FORMAT_ENUM_X)
    inst_ext_fmt_last
};


/***************************************************************************
* macrotable for instruction subsets
***************************************************************************/
#define DECLARE_INSTRUCTION_SUBSETS(X) \
  X(base) \
  X(memory) \
  X(branch) \
  X(jump) \
  X(nullifying) \
  X(bitmanip) \
  X(i128) \
  X(f128) \
  X(f64) \
  X(f32) \
  X(f16) \
  X(mmx) \
  X(special) \
  X(atomic) \
  X(privileged) \
  X(cipher) \
  X(group) \

enum EInstructionSubset : u8 {
#define INSTRUCTION_SUBSET_ENUM_X(name) instr_subset_##name,
    DECLARE_INSTRUCTION_SUBSETS(INSTRUCTION_SUBSET_ENUM_X)
    instr_subset_last
};


class InstructionInfo {
public:
    u64                            tmplt;
    EInstructionSubset             subset;
    EInstructionAssemblerFormat    sw_format;
    EInstructionFormat             hw_format;
};

extern const std::array<InstructionInfo, insn_reserved> asm_info;

ALWAYS_INLINE const char *insn_name(EInstructionID insn) {
    extern const std::array<char [40], insn_reserved> asm_name;
    return asm_name[insn];
}
EInstructionID GetNullificationInstructionForBranch(EInstructionID branch);
EInstructionID GetOppositeInstruction(EInstructionID branch);

inline bool is_static_reg(unsigned int n) { return  n >= base_rotate_regs + max_stacked_framesize; }
inline bool is_rotate_reg(unsigned int n) { return  n <  base_rotate_regs + max_stacked_framesize; }

/***************************************************************************
*
* special purpose registers
*
***************************************************************************/
enum class ESpecialRegisterSubset : u8 {
    reserved,   // not exist
    user,       // user read/write
    ruser,      // user read-only
    debug,
    system,     // system read/write
    interrupt,  // interruptions handler
    lapic,      // local advanced programmable interrupt controller
    rstack
};

enum class ESpecialRegisterRight : u8 {
    none             = 0,
    user_read        = 1,
    user_write       = 2,
    kernel_read      = 4,
    kernel_write     = 8,
    read             = user_read | kernel_read,
    user_readwrite   = user_read | user_write,
    kernel_readwrite = kernel_read | kernel_write,
    read_write       = user_readwrite | kernel_readwrite,
    urkrw            = user_read | kernel_readwrite
};

extern const std::array<ESpecialRegisterSubset, (1 << bits_per_register)> arch_spr_regtype;

#define DECLARE_SPECIAL_REGISTERS(X) \
    X (0,   ip,       u64,   user,         "instruction pointer") \
    X (1,   eip,      u64,   user,         "exception instruction pointer") \
    X (2,   fpcr,     u64,   user,         "floating-point control register") \
    X (3,   eca,      u64,   user,         "exception context address") \
    X (8,   rsc,      u64,   rstack,       "register stack control") \
    X (9,   rsp,      u64,   rstack,       "register stack pointer") \
    X (10,  bsp,      u64,   system,       "bottom stack pointer") \
    X (16,  psr,      u64,   system,       "processor status register") \
    X (17,  reip,     u64,   system,       "returnable exception instruction pointer") \
    X (18,  kip,      u64,   system,       "kernel instruction pointer") \
    X (19,  ksp,      u64,   system,       "kernel stack pointer") \
    X (20,  krsp,     u64,   system,       "kernel register stack pointer") \
    X (21,  peb,      u64,   system,       "process env block") \
    X (22,  teb,      u64,   system,       "thread env block") \
    X (23,  itc,      u64,   system,       "interval time counter") \
    X (24,  itm,      u64,   system,       "interval time match register") \
    X (25,  pta,      u64,   system,       "page table address") \
    X (26,  iva,      u64,   system,       "interruption vector address") \
    X (40,  iip,      u64,   interrupt,    "interruption instruction pointer") \
    X (41,  iipa,     u64,   interrupt,    "interruption instruction previous address") \
    X (42,  ipsr,     u64,   interrupt,    "interruption processor status register") \
    X (43,  cause,    u64,   interrupt,    "interruption cause register") \
    X (44,  ifa,      u64,   interrupt,    "interruption faulting address") \
    X (45,  iib,      u128,  interrupt,    "interruption instruction bundle") \
    X (64,  irr0,     u64,   lapic,        "interrupt requests register 0") \
    X (65,  irr1,     u64,   lapic,        "interrupt requests register 1") \
    X (66,  irr2,     u64,   lapic,        "interrupt requests register 2") \
    X (67,  irr3,     u64,   lapic,        "interrupt requests register 3") \
    X (72,  isr0,     u64,   lapic,        "interrupt service register 0") \
    X (73,  isr1,     u64,   lapic,        "interrupt service register 1") \
    X (74,  isr2,     u64,   lapic,        "interrupt service register 2") \
    X (75,  isr3,     u64,   lapic,        "interrupt service register 3") \
    X (80,  iv,       u64,   lapic,        "interrupt vector") \
    X (81,  lid,      u64,   lapic,        "local identification register") \
    X (82,  tpr,      u64,   lapic,        "task priority register") \
    X (83,  itcv,     u64,   lapic,        "interval time counter vector") \
    X (84,  tsv,      u64,   lapic,        "termal sensor vector") \
    X (85,  pmv,      u64,   lapic,        "performance monitor vector") \
    X (86,  cmcv,     u64,   lapic,        "correct machine check vector") \

enum ESpecialRegister {
#define SPECIAL_REGISTER_ENUM_X(value, name, type, regclass, descr) reg_##name = value,
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_ENUM_X)
#undef SPECIAL_REGISTER_ENUM_X
    reg_spr_last
};

} // namespace postrisc
