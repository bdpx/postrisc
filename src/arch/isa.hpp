#pragma once

/***************************************************************************
* this file contain all references to the
* virtual processor instruction set architecture (ISA)
***************************************************************************/

#include "util/common.hpp"
#include "instructions.hpp"

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
    X(misc) X(fused) X(raopx) X(brr) X(loop) \
    X(bfss) X(bfsd) X(bfsq) \
    X(nullrr) X(nulfss) X(nulfsd) X(nulfsq) \
    X(indexed_ds) X(indexed_ws) X(indexed_wz)

enum EInstructionID : u16 {
#define X(mnem, name, subset, format, descr) insn_##name,
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
  X (ldi,                      bits_per_call,                     disp_call,                imm,    "simm28",     "signed immediate for load in register") \
  X (flddi,                    bits_per_call,                     disp_call,                imm,    "fp64",       "floating-point inline immediate") \
  X (fldsi,                    bits_per_call,                     disp_call,                imm,    "fp32",       "floating-point inline immediate") \
  X (bin_simm,                 bits_per_binimm,                   disp_binimm,              imm,    "simm21",     "signed immediate for binary") \
  X (bin_simm_plus,            bits_per_binimm,                   disp_binimm,              imm,    "simm21",     "signed immediate for binary (+1)") \
  X (bin_simm_minus,           bits_per_binimm,                   disp_binimm,              imm,    "simm21",     "signed immediate for binary (-1)") \
  X (bin_uimm,                 bits_per_binimm,                   disp_binimm,              imm,    "uimm21",     "unsigned immediate for binary") \
  X (bin_uimm_plus,            bits_per_binimm,                   disp_binimm,              imm,    "uimm21",     "unsigned immediate for binary (+1)") \
  X (bin_uimm_minus,           bits_per_binimm,                   disp_binimm,              imm,    "uimm21",     "unsigned immediate for binary (-1)") \
  X (res21,                    bits_per_binimm,                   disp_binimm,              res,    "0",          "reserved") \
  X (indexed_disp,             bits_per_indexed_disp,             disp_indexed_disp,        imm,    "disp",       "displacement for indexed addressing") \
  X (indexed_scale,            bits_per_indexed_scale,            disp_indexed_scale,       mod,    "scale",      "scale for indexed addressing") \
  X (indexed_opx,              bits_per_indexed_opx,              disp_indexed_opx,         opx,    "opx",        "indexed extended opcode") \
  X (brc_simm,                 bits_per_brci_imm,                 disp_brci_imm,            imm,    "simm11",     "signed immediate for compare-and-branch") \
  X (brc_simm_plus,            bits_per_brci_imm,                 disp_brci_imm,            imm,    "simm11",     "signed immediate for compare-and-branch (+1)") \
  X (brc_uimm,                 bits_per_brci_imm,                 disp_brci_imm,            imm,    "uimm11",     "unsigned immediate for compare-and-branch") \
  X (brc_uimm_plus,            bits_per_brci_imm,                 disp_brci_imm,            imm,    "uimm11",     "unsigned immediate for compare-and-branch (+1)") \
  X (rip_label,                bits_per_call,                     disp_call,                imm,    "uimm28",     "forward ip-relative displacement") \
  X (rip_string,               bits_per_call,                     disp_call,                imm,    "uimm28",     "label in rodata as string") \
  X (rip_float32,              bits_per_call,                     disp_call,                imm,    "fp32",       "forward ip-relative displacement (rodata immediate single)") \
  X (rip_float64,              bits_per_call,                     disp_call,                imm,    "fp64",       "forward ip-relative displacement (rodata immediate double)") \
  X (rip_float128,             bits_per_call,                     disp_call,                imm,    "fp128",      "forward ip-relative displacement (rodata immediate quadruple)") \
  X (call,                     bits_per_call,                     disp_call,                imm,    "simm28x16",  "call label text") \
  X (branch,                   bits_per_branch,                   disp_branch,              imm,    "simm17x16",  "middle branch label") \
  X (branch_opx,               bits_per_branch_opx,               disp_branch_opx,          opx,    "opx",        "extended opcode for branch (4 bit)") \
  X (branch_opx_res,           bits_per_branch_opx,               disp_branch_opx,          res,    "0",          "field should be zero") \
  X (ca_r,                     bits_per_call,                     disp_call,                imm,    "simm28x16",  "long label text+rodata") \
  X (framesize,                bits_per_framesize,                disp_framesize,           imm,    "framesize",  "register window frame size") \
  X (shamt_b,                  bits_per_shamt,                    disp_reg_b,               tdb,    "sb",         "shift value in 2 position") \
  X (shamt_c,                  bits_per_shamt,                    disp_reg_c,               tdb,    "sc",         "shift value in 3 position") \
  X (shamt_d,                  bits_per_shamt,                    disp_reg_d,               tdb,    "sd",         "shift value in 4 position") \
  X (shamt_e,                  bits_per_shamt,                    disp_reg_e,               tdb,    "se",         "shift value in 5 position") \
  X (vshamt_b,                 bits_per_register,                 disp_reg_b,               tdb,    "shift",      "vector bit count in 2 position") \
  X (vshamt_c,                 bits_per_register,                 disp_reg_c,               tdb,    "shift",      "vector bit count in 3 position") \
  X (vshamt_d,                 bits_per_register,                 disp_reg_d,               tdb,    "shift",      "vector bit count in 4 position") \
  X (vshamt_e,                 bits_per_register,                 disp_reg_e,               tdb,    "shift",      "vector bit count in 5 position") \
  X (mask_yes,                 bits_per_nullify_dist,             disp_nullify_dist_yes,    imm,    "dy",         "dist-yes") \
  X (mask_no,                  bits_per_nullify_dist,             disp_nullify_dist_no,     imm,    "dn",         "dist-not") \
  X (fused_opx,                bits_per_fused_opx,                disp_fused_opx,           opx,    "opx",        "extended opcode for fused (7)") \
  X (misc_opx,                 bits_per_misc_opx,                 disp_misc_opx,            opx,    "opx",        "extended opcode for misc (10)") \
  X (raopx,                    bits_per_register,                 disp_reg_a,               opx,    "opx",        "extended opcode for opx7 (raopx)") \
  X (update_stride,            bits_per_update_stride,            disp_update_stride,       imm,    "simm10",     "signed immediate stride for base update") \
  X (stride_simm,              bits_per_update_stride,            disp_update_stride,       imm,    "simm10",     "signed immediate for atomic load/store") \
  X (rounding_mode,            bits_per_rounding_mode,            disp_rounding_mode,       mod,    "rm",         "rounding_mode") \
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
  X (ldi,        VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_ldi)) \
  X (fldsi,      VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_fldsi)) \
  X (flddi,      VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_flddi)) \
  X (RaxU28,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_ldi)) \
  X (RaxbSI21,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gprb, cf_comma, cf_bin_simm)) \
  X (RaxUI21,    VARRAY(cf_lprefix, cf_argsdelimiter, cf_bin_uimm)) \
  X (RaxbBBS,    VARRAY(cf_lprefix, cf_argsdelimiter, cf_gprb, cf_comma, cf_branch)) \
  X (RabSI21,    VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_simm)) \
  X (RabSI21P,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_simm_plus)) \
  X (RabSI21M,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_simm_minus)) \
  X (RabUI21,    VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_uimm)) \
  X (RabUI21P,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_uimm_plus)) \
  X (RabUI21M,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_bin_uimm_minus)) \
  X (RabBBS,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_branch)) \
  X (RbaBBS,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gprb, cf_comma, cf_gpra, cf_comma, cf_branch))  /* pseudo for RabB20 */ \
  X (RaSbBBS,    VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_vshamt_b, cf_comma, cf_branch)) \
  X (RaSIBBS,    VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_simm, cf_comma, cf_branch)) \
  X (RaUIBBS,    VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_uimm, cf_comma, cf_branch)) \
  X (RaSIPBBS,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_simm_plus, cf_comma, cf_branch)) \
  X (RaUIPBBS,   VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_uimm_plus, cf_comma, cf_branch)) \
  X (callr,      VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_call)) \
  X (br,         VARRAY(cf_lprefix, cf_argsdelimiter, cf_call)) \
  X (ca_r,       VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_ca_r)) \
  X (gmemr,      VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_label)) \
  X (write,      VARRAY(cf_lprefix, cf_argsdelimiter, cf_rip_string)) \
  X (fldsri,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_float32)) \
  X (flddri,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_float64)) \
  X (fldqri,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_rip_float128)) \
  X (gmemx,      VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_indexed_scale, cf_comma, cf_indexed_disp)) \
  X (gmemu,      VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_update_stride)) \
  X (RabS10,     VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_stride_simm)) \
  X (RbS10,      VARRAY(cf_argsdelimiter,                    cf_gprb, cf_comma, cf_stride_simm)) \
  X (r4,         VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_gprd)) \
  X (RabcSd,     VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_shamt_d)) \
  X (FabcSd,     VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_vshamt_d)) \
  X (RabScd,     VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c, cf_comma, cf_shamt_d)) \
  X (RabcSde,    VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_shamt_d, cf_comma, cf_shamt_e)) \
  X (RabScde,    VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c, cf_comma, cf_shamt_d, cf_comma, cf_shamt_e)) \
  X (r3,         VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc)) \
  X (Racb,       VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprc, cf_comma, cf_gprb)) /* pseudo for r3 */ \
  X (RabSc,      VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c)) \
  X (r2,         VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb)) \
  X (Not,        VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb)) \
  X (Rbc,        VARRAY(cf_argsdelimiter, cf_gprb, cf_comma, cf_gprc)) \
  X (RbcScale,   VARRAY(cf_argsdelimiter, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_indexed_scale)) \
  X (alloc,      VARRAY(cf_argsdelimiter, cf_framesize)) \
  X (alloc_sp,   VARRAY(cf_argsdelimiter, cf_framesize, cf_comma, cf_bin_uimm)) \
  X (r2m2,       VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (RaSIm2,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_simm, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (RaUIm2,     VARRAY(cf_lprefix, cf_argsdelimiter, cf_gpra, cf_comma, cf_brc_uimm, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (RaSbm2,     VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_vshamt_b, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (Round,      VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_rounding_mode)) \
  X (NoArgs,     VARRAY(cf_argsdelimiter, cf_none)) \
  X (FabSc,      VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_vshamt_c)) \
  X (mspr,       VARRAY(cf_argsdelimiter, cf_gpra, cf_comma, cf_spr)) \


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
  X (ldi,      ldi,    "oooooooaaaaaaaiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_ldi), "load immediate") \
  X (ca_r,     ca_r,   "oooooooaaaaaaapppppppppppppppppppppppppppp", VARRAY(cf_gpra, cf_call), "branch/load ip-relative address") \
  X (RaU28,    ca_rf,  "oooooooaaaaaaaiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_rip_label), "load/store ip-relative") \
  X (br,       ca_r,   "oooooooxxxxxxxiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_call), "jump/call ip-relative") \
  X (RaxU28,   ca_rf,  "oooooooxxxxxxxiiiiiiiiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_rip_label), "br/write ip-relative") \
  X (alloc,    none,   "oooooooxxxxxxxfffffff---------------------", VARRAY(cf_raopx, cf_framesize, cf_res21), "alloc") \
  X (alloc_sp, imbin,  "oooooooxxxxxxxfffffffiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_framesize, cf_bin_uimm), "allocsp") \
  X (RaxUI21,  imbin,  "oooooooxxxxxxx-------iiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_unused_reg, cf_bin_uimm), "retf") \
  X (RaxbSI21, imbin,  "oooooooxxxxxxxbbbbbbbiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_raopx, cf_gprb, cf_bin_simm), "Rb with signed imm") \
  X (RabSI21,  imbin,  "oooooooaaaaaaabbbbbbbiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_gprb, cf_bin_simm), "binary register with signed imm") \
  X (RabUI21,  imbin,  "oooooooaaaaaaabbbbbbbiiiiiiiiiiiiiiiiiiiii", VARRAY(cf_gpra, cf_gprb, cf_bin_uimm), "binary register with unsigned imm") \
  X (RaxbBBS,  brc,    "oooooooxxxxxxxbbbbbbb----ppppppppppppppppp", VARRAY(cf_raopx, cf_gprb, cf_branch_opx_res, cf_branch), "eh.adj") \
  X (RabBBS,   brc,    "oooooooaaaaaaabbbbbbbxxxxppppppppppppppppp", VARRAY(cf_gpra, cf_gprb, cf_branch_opx, cf_branch), "[inc/dec], compare registers, branch") \
  X (RaSbBBS,  brc,    "oooooooaaaaaaasssssssxxxxppppppppppppppppp", VARRAY(cf_gpra, cf_vshamt_b, cf_branch_opx, cf_branch), "compare bit and branch") \
  X (RaSIBBS,  brci,   "oooooooaaaaaaaiiiiiiiiiiippppppppppppppppp", VARRAY(cf_gpra, cf_brc_simm, cf_branch), "compare register with signed imm and branch") \
  X (RaUIBBS,  brci,   "oooooooaaaaaaaiiiiiiiiiiippppppppppppppppp", VARRAY(cf_gpra, cf_brc_uimm, cf_branch), "compare register with unsigned imm and branch") \
  X (RaSIm2,   cni,    "oooooooaaaaaaaiiiiiiiiiiinnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_brc_simm, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-simm nullify") \
  X (RaUIm2,   cni,    "oooooooaaaaaaaiiiiiiiiiiinnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_brc_uimm, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-uimm nullify") \
  X (RaSbm2,   none,   "oooooooaaaaaaasssssssxxxxnnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_vshamt_b, cf_branch_opx, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-shift nullify") \
  X (r2m2,     none,   "oooooooaaaaaaabbbbbbbxxxxnnnnnyyyyyxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_branch_opx, cf_mask_no, cf_mask_yes, cf_fused_opx), "cond reg-reg nullify") \
  X (r4,       none,   "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_gprd, cf_fused_opx), "add/subtract with carry out") \
  X (RabcSd,   none,   "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_vshamt_d, cf_fused_opx), "shift variable and mask(clear)") \
  X (RabScd,   none,   "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_vshamt_c, cf_vshamt_d, cf_fused_opx), "shift fixed and mask(clear)") \
  X (RabScde,  none,   "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee", VARRAY(cf_gpra, cf_gprb, cf_vshamt_c, cf_vshamt_d, cf_vshamt_e), "rotate masked imm") \
  X (RabcSde,  none,   "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_vshamt_d, cf_vshamt_e), "bit-field insert") \
  X (gmemx,    imbin,  "oooooooaaaaaaabbbbbbbcccccccsssxxxxiiiiiii", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_indexed_scale, cf_indexed_opx, cf_indexed_disp), "scaled index load/store") \
  X (RbcScale, none,   "ooooooo-------bbbbbbbcccccccsssxxxxxxxxxxx", VARRAY(cf_unused_reg, cf_gprb, cf_gprc, cf_indexed_scale, cf_misc_opx), "scaled indexed jump") \
  X (Rbc,      none,   "ooooooo-------bbbbbbbccccccc---xxxxxxxxxxx", VARRAY(cf_unused_reg, cf_gprb, cf_gprc, cf_misc_opx_null, cf_misc_opx), "two args, no result") \
  X (mspr,     none,   "oooooooaaaaaaa-------kkkkkkk---xxxxxxxxxxx", VARRAY(cf_gpra, cf_unused_reg, cf_spr, cf_misc_opx_null, cf_misc_opx), "move from/to special-purpose register") \
  X (r2,       none,   "oooooooaaaaaaabbbbbbb----------xxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_unused_reg, cf_misc_opx_null, cf_misc_opx), "unary") \
  X (Round,    none,   "oooooooaaaaaaabbbbbbb-------mmmxxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_unused_reg, cf_rounding_mode, cf_misc_opx), "round") \
  X (RabSc,    none,   "oooooooaaaaaaabbbbbbbsssssss---xxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_vshamt_c, cf_misc_opx_null, cf_misc_opx), "binary with shift amount") \
  X (r3,       none,   "oooooooaaaaaaabbbbbbbccccccc---xxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_misc_opx_null, cf_misc_opx), "binary register-register") \
  X (gmemu,    none,   "oooooooaaaaaaabbbbbbbiiiiiiiiiixxxxxxxxxxx", VARRAY(cf_gpra, cf_gprb, cf_stride_simm, cf_misc_opx), "general [base with update] load/store") \
  X (RbS10,    none,   "ooooooo-------bbbbbbbiiiiiiiiiixxxxxxxxxxx", VARRAY(cf_unused_reg, cf_gprb, cf_stride_simm, cf_misc_opx), "interrupt") \
  X (NoArgs,   none,   "ooooooo------------------------xxxxxxxxxxx", VARRAY(cf_misc_noarg, cf_misc_opx), "instruction without parameters") \

enum EInstructionFormat : u8 {
#define INSTRUCTION_FORMAT_ENUM_X(name, extname, descr, format, txt) inst_fmt_##name,
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_ENUM_X)
    inst_fmt_last,
    // pseudo for alternate assembler formats
    inst_fmt_Racb       = inst_fmt_r3,
    inst_fmt_Not        = inst_fmt_RabSI21,
    inst_fmt_RbaBBS     = inst_fmt_RabBBS,   // inverted reg args
    inst_fmt_RabSI21P   = inst_fmt_RabSI21,  // immediate + 1
    inst_fmt_RabSI21M   = inst_fmt_RabSI21,  // immediate - 1
    inst_fmt_RaSIPBBS   = inst_fmt_RaSIBBS,  // immediate + 1
    inst_fmt_RabUI21P   = inst_fmt_RabUI21,  // immediate + 1
    inst_fmt_RabUI21M   = inst_fmt_RabUI21,  // immediate - 1
    inst_fmt_RaUIPBBS   = inst_fmt_RaUIBBS,  // immediate + 1
    inst_fmt_callr      = inst_fmt_ca_r,
    inst_fmt_FabcSd     = inst_fmt_RabcSd,
    inst_fmt_RabS10     = inst_fmt_gmemu,
    inst_fmt_gmemr      = inst_fmt_RaU28,
    inst_fmt_FabSc      = inst_fmt_RabSc,  // restricted Sc (6 bits)
    inst_fmt_write      = inst_fmt_RaxU28,
    inst_fmt_flddi      = inst_fmt_ldi,
    inst_fmt_flddri     = inst_fmt_RaU28,
    inst_fmt_fldqri     = inst_fmt_RaU28,
    inst_fmt_fldsi      = inst_fmt_ldi,
    inst_fmt_fldsri     = inst_fmt_RaU28,
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
  X (none,   "------------------------------------------",  "no extension") \
  X (ca_r,   "----------pppppppppppppppppppppppppppppppp",  "60(28+32) bit relative offset") \
  X (ldi,    "------iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",  "64(28+36) bit ldi imm") \
  X (ca_rf,  "------pppppppppppppppppppppppppppppppppppp",  "64(28+36) bit relative forward offset") \
  X (imbin,  "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",  "63(21+42) bit imm") \
  X (brci,   "iiiiiiiiiiiiiiiiiiiiiiiiiiiiippppppppppppp",  "40(11+29) bit imm, 30(17+13) bit branch offset") \
  X (cni,    "iiiiiiiiiiiiiiiiiiiiiiiiiiiii-------------",  "40(11+29) bit imm, no branch (nullification)") \
  X (brc,    "-----------------------------ppppppppppppp",  "30(17+13) bit branch offset, no imm") \

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
  X(nullifying) \
  X(bitmanip) \
  X(intq) \
  X(fpuq) \
  X(fpud) \
  X(fpus) \
  X(fpuh) \
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


class CInstructionInfo {
public:
    u64                            tmplt;
    EInstructionSubset             subset;
    EInstructionAssemblerFormat    sw_format;
    EInstructionFormat             hw_format;
};

extern const std::array<CInstructionInfo, insn_reserved> asm_info;
extern const std::array<char [40], insn_reserved> asm_name;

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
    X (0,   ip,       user,         "instruction pointer") \
    X (1,   eip,      user,         "exception instruction pointer") \
    X (2,   fpcr,     user,         "floating-point control register") \
    X (3,   eca,      user,         "exception context address") \
    X (8,   rsc,      rstack,       "register stack control") \
    X (9,   rsp,      rstack,       "register stack pointer") \
    X (10,  bsp,      system,       "bottom stack pointer") \
    X (16,  psr,      system,       "processor status register") \
    X (17,  reip,     system,       "returnable exception instruction pointer") \
    X (18,  kip,      system,       "kernel instruction pointer") \
    X (19,  ksp,      system,       "kernel stack pointer") \
    X (20,  krsp,     system,       "kernel register stack pointer") \
    X (21,  peb,      system,       "process env block") \
    X (22,  teb,      system,       "thread env block") \
    X (23,  itc,      system,       "interval time counter") \
    X (24,  itm,      system,       "interval time match register") \
    X (25,  pta,      system,       "page table address") \
    X (26,  iva,      system,       "interruption vector address") \
    X (40,  iip,      interrupt,    "interruption instruction pointer") \
    X (41,  iipa,     interrupt,    "interruption instruction previous address") \
    X (42,  ipsr,     interrupt,    "interruption processor status register") \
    X (43,  cause,    interrupt,    "interruption cause register") \
    X (44,  ifa,      interrupt,    "interruption faulting address") \
    X (45,  iib,      interrupt,    "interruption instruction bundle") \
    X (64,  irr0,     lapic,        "interrupt requests register 0") \
    X (65,  irr1,     lapic,        "interrupt requests register 1") \
    X (66,  irr2,     lapic,        "interrupt requests register 2") \
    X (67,  irr3,     lapic,        "interrupt requests register 3") \
    X (72,  isr0,     lapic,        "interrupt service register 0") \
    X (73,  isr1,     lapic,        "interrupt service register 1") \
    X (74,  isr2,     lapic,        "interrupt service register 2") \
    X (75,  isr3,     lapic,        "interrupt service register 3") \
    X (80,  iv,       lapic,        "interrupt vector") \
    X (81,  lid,      lapic,        "local identification register") \
    X (82,  tpr,      lapic,        "task priority register") \
    X (83,  itcv,     lapic,        "interval time counter vector") \
    X (84,  tsv,      lapic,        "termal sensor vector") \
    X (85,  pmv,      lapic,        "performance monitor vector") \
    X (86,  cmcv,     lapic,        "correct machine check vector") \

enum ESpecialRegister {
#define SPECIAL_REGISTER_ENUM_X(value, name, regclass, descr) reg_##name = value,
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_ENUM_X)
    reg_spr_last
};

} // namespace postrisc
