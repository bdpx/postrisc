#pragma once

/***************************************************************************
* this file contain all references to the
* virtual processor instruction set architecture (ISA)
***************************************************************************/

#include "util/common.hpp"
#include "instructions.hpp"

#define ARCH_NAME        "POSTRISC"
#define ARCH_COPYRIGHT   "Copyright (C) 2003-2022 Dmitry Buvaylo."

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
    X(misc) X(fused) X(raopx) X(nullify) \

enum EInstructionID : uint16_t {
#define X(name, subset, inst_format, format, iic, tmplt, descr) insn_##name,
#include "insn_table.hpp"
#undef X
    insn_last,
// another mnemonics provided for opcodes naming only
#define INSN_X(name) insn_##name,
    DECLARE_OPCODES_WITH_EXTENDED_CODES(INSN_X)
    insn_reserved
};

inline std::ostream& operator << (std::ostream& out, EInstructionID eid)
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

enum EInstructionFieldKind : uint8_t {
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
  X (none,                     bits_per_unknown,                  disp_unknown,          opx,    "unknown",    "end-marker for field-list") \
  X (comma,                    bits_per_unknown,                  disp_unknown,          opx,    ",",          "comma") \
  X (comma_opt,                bits_per_unknown,                  disp_unknown,          opx,    ",",          "optional comma") \
  X (lprefix,                  bits_per_unknown,                  disp_unknown,          opx,    "",           "long prefix") \
  X (opcode,                   bits_per_opcode,                   disp_opcode,           opc,    "opcode",     "main opcode") \
  X (gpra,                     bits_per_register,                 disp_reg_a,            gpr,    "ra",         "general register in 1 position") \
  X (gprb,                     bits_per_register,                 disp_reg_b,            gpr,    "rb",         "general register in 2 position") \
  X (gprc,                     bits_per_register,                 disp_reg_c,            gpr,    "rc",         "general register in 3 position") \
  X (gprd,                     bits_per_register,                 disp_reg_d,            gpr,    "rd",         "general register in 4 position") \
  X (spr,                      bits_per_register,                 disp_reg_c,            spr,    "spr",        "special purpose register in 3 position") \
  X (ldi,                      bits_per_branch,                   disp_branch,           imm,    "simm28",     "signed immediate for load in register") \
  X (flddi,                    bits_per_branch,                   disp_branch,           imm,    "fp64",       "floating-point inline immediate") \
  X (fldsi,                    bits_per_branch,                   disp_branch,           imm,    "fp32",       "floating-point inline immediate") \
  X (binimm,                   bits_per_binimm,                   disp_binimm,           imm,    "simm21",     "signed immediate for binary") \
  X (binimmu,                  bits_per_binimm,                   disp_binimm,           imm,    "uimm21",     "unsigned immediate for binary") \
  X (indexed_disp,             bits_per_indexed_disp,             disp_indexed_disp,     imm,    "disp",       "displacement for indexed addressing") \
  X (indexed_scale,            bits_per_indexed_scale,            disp_indexed_scale,    mod,    "scale",      "scale for indexed addressing") \
  X (indexed_scale_zero,       bits_per_indexed_scale_zero,       disp_unknown,          res,    "0",          "reserved") \
  X (brc_simm,                 bits_per_brci_imm,                 disp_brci_imm,         imm,    "simm14",     "signed immediate for compare-and-branch") \
  X (brc_uimm,                 bits_per_brci_imm,                 disp_brci_imm,         imm,    "uimm14",     "unsigned immediate for compare-and-branch") \
  X (rip_label,                bits_per_branch,                   disp_branch,           imm,    "uimm28",     "forward ip-relative displacement") \
  X (rip_string,               bits_per_branch,                   disp_branch,           imm,    "uimm28",     "label in rodata as string") \
  X (rip_float32,              bits_per_branch,                   disp_branch,           imm,    "fp32",       "forward ip-relative displacement (rodata immediate single)") \
  X (rip_float64,              bits_per_branch,                   disp_branch,           imm,    "fp64",       "forward ip-relative displacement (rodata immediate double)") \
  X (rip_float128,             bits_per_branch,                   disp_branch,           imm,    "fp128",      "forward ip-relative displacement (rodata immediate quadruple)") \
  X (float128_low,             bits_per_branch,                   disp_branch,           imm,    "fp128",      "immediate quadruple (low 64 bit)") \
  X (float128_high,            bits_per_branch,                   disp_branch,           imm,    "fp128",      "immediate quadruple (high 64 bit)") \
  X (brc_branch,               bits_per_brc_branch,               disp_brc_branch,       imm,    "simm21x16",  "middle branch label") \
  X (short_branch,             bits_per_short_branch,             disp_short_branch,     imm,    "simm14x16",  "short branch label (imm)") \
  X (branch,                   bits_per_branch,                   disp_branch,           imm,    "simm28x16",  "long branch label text") \
  X (ldar,                     bits_per_branch,                   disp_branch,           imm,    "simm28x16",  "long label text+rodata") \
  X (shamt_b,                  bits_per_shamt,                    disp_reg_b,            tdb,    "sb",         "shift value in 2 position") \
  X (shamt_c,                  bits_per_shamt,                    disp_reg_c,            tdb,    "sc",         "shift value in 3 position") \
  X (shamt_d,                  bits_per_shamt,                    disp_reg_d,            tdb,    "sd",         "shift value in 4 position") \
  X (shamt_e,                  bits_per_shamt,                    disp_reg_e,            tdb,    "se",         "shift value in 5 position") \
  X (vshamt_b,                 bits_per_register,                 disp_reg_b,            tdb,    "shift",      "vector bit count in 2 position") \
  X (vshamt_c,                 bits_per_register,                 disp_reg_c,            tdb,    "shift",      "vector bit count in 3 position") \
  X (vshamt_d,                 bits_per_register,                 disp_reg_d,            tdb,    "shift",      "vector bit count in 4 position") \
  X (vshamt_e,                 bits_per_register,                 disp_reg_e,            tdb,    "shift",      "vector bit count in 5 position") \
  X (mask_yes,                 bits_per_nullify_dist,             disp_nullify_dist_yes, imm,    "dist-yes",   "dist-yes") \
  X (mask_no,                  bits_per_nullify_dist,             disp_nullify_dist_no,  imm,    "dist-not",   "dist-not") \
  X (fused_opx,                bits_per_fused_opx,                disp_opx,              opx,    "opx",        "extended opcode for fused (7)") \
  X (misc_opx,                 bits_per_misc_opx,                 disp_opx,              opx,    "opx",        "extended opcode for misc (10)") \
  X (raopx,                    bits_per_register,                 disp_reg_a,            opx,    "opx",        "extended opcode for opx7 (raopx)") \
  X (nullify_opx,              bits_per_nullify_opx,              disp_opx,              opx,    "opx",        "nullify extended opcode") \
  X (update_stride,            bits_per_update_stride,            disp_update_stride,    imm,    "stride",     "signed immediate stride for base post-update") \
  X (uimm14,                   bits_per_update_stride,            disp_update_stride,    imm,    "uimm14",     "unsigned immediate offset for syscall,int,cpuid,callmi") \
  X (loop_step,                bits_per_loop_step,                disp_loop_step,        imm,    "step",       "immediate loop step") \
  X (fp_compare_mode,          bits_per_fp_compare_mode,          disp_reg_d,            mod,    "cmode",      "floating-point compare mode") \
  X (clmul_index,              bits_per_clmul_index,              disp_indexed_scale,    imm,    "index",      "clmul index") \
  X (rounding_mode,            bits_per_rounding_mode,            disp_indexed_scale,    mod,    "rm",         "rounding_mode") \
  X (rounding_mode_zero,       bits_per_rounding_mode_zero,       disp_unknown,          res,    "0",          "reserved") \
  X (memory_ordering,          bits_per_memory_ordering,          disp_memory_ordering,  mod,    "mo",         "memory ordering for atomic RMW operations") \
  X (memory_ordering_zero,     bits_per_memory_ordering_zero,     disp_unknown,          res,    "0",          "reserved") \
  X (unused_reg,               bits_per_register,                 disp_unknown,          res,    "0",          "field should be zero") \
  X (misc_opx_null,            bits_per_misc_opx_null,            disp_opx,              res,    "0",          "field should be zero") \
  X (nop,                      bits_per_nop,                      disp_reg_c,            res,    "0",          "field should be zero") \



enum EInstructionField : uint8_t {
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
*                   see table for this bits
*
***************************************************************************/
#define DECLARE_INSTRUCTION_FORMATS(X) \
  X (ldi,         ldi,    "oooooooaaaaaaaiiiiiiiiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_gpra, cf_ldi), "load immediate") \
  X (ldar,        ldar,   "oooooooaaaaaaapppppppppppppppppppppppppppp",   VARRAY(cf_gpra, cf_branch), "branch/load ip-relative address") \
  X (RaU28,       ldr,    "oooooooaaaaaaaiiiiiiiiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_gpra, cf_rip_label), "load/store ip-relative") \
  X (br,          ldar,   "oooooooxxxxxxxiiiiiiiiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_raopx, cf_branch), "br/load ip-relative") \
  X (RaxU28,      ldr,    "oooooooxxxxxxxiiiiiiiiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_raopx, cf_rip_label), "br/write ip-relative") \
  X (RaxbI21,     imbin,  "oooooooxxxxxxxbbbbbbbiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_raopx, cf_gprb, cf_binimm), "Rb with signed imm") \
  X (RaxbU21,     imbin,  "oooooooxxxxxxxbbbbbbbiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_raopx, cf_gprb, cf_binimmu), "Rb with unsigned imm") \
  X (RabI21,      imbin,  "oooooooaaaaaaabbbbbbbiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_gpra, cf_gprb, cf_binimm), "binary register with signed imm") \
  X (RabU21,      imbin,  "oooooooaaaaaaabbbbbbbiiiiiiiiiiiiiiiiiiiii",   VARRAY(cf_gpra, cf_gprb, cf_binimmu), "binary register with unsigned imm") \
  X (RabB21,      brc,    "oooooooaaaaaaabbbbbbbppppppppppppppppppppp",   VARRAY(cf_gpra, cf_gprb, cf_brc_branch), "compare registers and branch") \
  X (RaSbB21,     brc,    "oooooooaaaaaaasssssssppppppppppppppppppppp",   VARRAY(cf_gpra, cf_vshamt_b, cf_brc_branch), "compare bit and branch") \
  X (loop,        brci,   "oooooooaaaaaaabbbbbbbiiiiiiipppppppppppppp",   VARRAY(cf_gpra, cf_gprb, cf_loop_step, cf_short_branch), "repeat loop") \
  X (RaI14B14,    brci,   "oooooooaaaaaaaiiiiiiiiiiiiiipppppppppppppp",   VARRAY(cf_gpra, cf_brc_simm, cf_short_branch), "compare register with signed imm and branch") \
  X (RaU14B14,    brci,   "oooooooaaaaaaaiiiiiiiiiiiiiipppppppppppppp",   VARRAY(cf_gpra, cf_brc_uimm, cf_short_branch), "compare register with unsigned imm and branch") \
  X (RaI14m2,     cni,    "oooooooaaaaaaaiiiiiiiiiiiiiinnnnnyyyyyxxxx",   VARRAY(cf_gpra, cf_brc_simm, cf_mask_no, cf_mask_yes, cf_nullify_opx), "cond reg-imm nullify") \
  X (RaU14m2,     cni,    "oooooooaaaaaaaiiiiiiiiiiiiiinnnnnyyyyyxxxx",   VARRAY(cf_gpra, cf_brc_uimm, cf_mask_no, cf_mask_yes, cf_nullify_opx), "cond reg-imm nullify") \
  X (r2m2,        none,   "oooooooaaaaaaabbbbbbb-------nnnnnyyyyyxxxx",   VARRAY(cf_gpra, cf_gprb, cf_unused_reg, cf_mask_no, cf_mask_yes, cf_nullify_opx), "cond reg-reg nullify") \
  X (RaSbm2,      none,   "oooooooaaaaaaasssssss-------nnnnnyyyyyxxxx",   VARRAY(cf_gpra, cf_vshamt_b, cf_unused_reg, cf_mask_no, cf_mask_yes, cf_nullify_opx), "cond reg-shift nullify") \
  X (gmemx,       imbin,  "oooooooaaaaaaabbbbbbbccccccc--sssiiiiiiiii",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_indexed_scale_zero, cf_indexed_scale, cf_indexed_disp), "scaled index load/store") \
  X (gmemux,      none,   "oooooooaaaaaaabbbbbbbccccccc--sssxxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_indexed_scale_zero, cf_indexed_scale, cf_misc_opx), "load/store indexed base-update") \
  X (amo,         none,   "oooooooaaaaaaabbbbbbbccccccc---mmxxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_memory_ordering_zero, cf_memory_ordering, cf_misc_opx), "fetch-op") \
  X (aload,       none,   "oooooooaaaaaaabbbbbbb----------mmxxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_unused_reg, cf_memory_ordering_zero, cf_memory_ordering, cf_misc_opx), "atomic load/store") \
  X (fpcmp,       none,   "oooooooaaaaaaabbbbbbbcccccccmmmmmxxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_fp_compare_mode, cf_misc_opx), "fp compare mode") \
  X (Rabc,        none,   "oooooooaaaaaaabbbbbbbccccccc-----xxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_misc_opx_null, cf_misc_opx), "binary register-register") \
  X (RabSc,       none,   "oooooooaaaaaaabbbbbbbsssssss-----xxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_vshamt_c, cf_misc_opx_null, cf_misc_opx), "binary with shift amount") \
  X (mspr,        none,   "oooooooaaaaaaakkkkkkk------------xxxxxxxxx",   VARRAY(cf_gpra, cf_spr, cf_unused_reg, cf_misc_opx_null, cf_misc_opx), "move from/to special-purpose register") \
  X (Rab,         none,   "oooooooaaaaaaabbbbbbb------------xxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_unused_reg, cf_misc_opx_null, cf_misc_opx), "unary") \
  X (Ra,          none,   "oooooooaaaaaaa-------------------xxxxxxxxx",   VARRAY(cf_gpra, cf_unused_reg, cf_unused_reg, cf_misc_opx_null, cf_misc_opx), "no-args") \
  X (Round,       none,   "oooooooaaaaaaabbbbbbb---------mmmxxxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_unused_reg, cf_rounding_mode_zero, cf_rounding_mode, cf_misc_opx), "round") \
  X (Rbc,         none,   "ooooooo-------bbbbbbbccccccc-----xxxxxxxxx",   VARRAY(cf_unused_reg, cf_gprb, cf_gprc, cf_misc_opx_null, cf_misc_opx), "two args, no result") \
  X (Rb,          none,   "ooooooo-------bbbbbbb------------xxxxxxxxx",   VARRAY(cf_unused_reg, cf_gprb, cf_unused_reg, cf_misc_opx_null, cf_misc_opx), "one arg, no result") \
  X (NoArgs,      none,   "ooooooo--------------------------xxxxxxxxx",   VARRAY(cf_nop, cf_misc_opx_null, cf_misc_opx), "instruction without parameters") \
  X (RabcSde,     none,   "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_vshamt_d, cf_vshamt_e), "bit-field insert") \
  X (RabScde,     none,   "oooooooaaaaaaabbbbbbbcccccccdddddddeeeeeee",   VARRAY(cf_gpra, cf_gprb, cf_vshamt_c, cf_vshamt_d, cf_vshamt_e), "rotate masked imm") \
  X (Rabcd,       none,   "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_gprd, cf_fused_opx), "add/subtract with carry out") \
  X (sprsr,       none,   "oooooooaaaaaaakkkkkkkcccccccdddddddxxxxxxx",   VARRAY(cf_gpra, cf_spr, cf_gprc, cf_gprd, cf_fused_opx), "special register set/reset") \
  X (RabcSd,      none,   "oooooooaaaaaaabbbbbbbcccccccdddddddxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_gprc, cf_vshamt_d, cf_fused_opx), "shift variable and mask(clear)") \
  X (RabScd,      none,   "oooooooaaaaaaabbbbbbb-cccccc-ddddddxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_vshamt_c, cf_vshamt_d, cf_fused_opx), "shift fixed and mask(clear)") \
  X (gmemu,       none,   "oooooooaaaaaaabbbbbbbttttttttttttttxxxxxxx",   VARRAY(cf_gpra, cf_gprb, cf_update_stride, cf_fused_opx), "general [base with update] load/store, AES keygen assist with round constant") \

enum EInstructionFormat : uint8_t {
#define INSTRUCTION_FORMAT_ENUM_X(name, extname, descr, format, txt) inst_fmt_##name,
    DECLARE_INSTRUCTION_FORMATS(INSTRUCTION_FORMAT_ENUM_X)
    inst_fmt_last
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
  X (none,     "------------------------------------------",    "without extension") \
  X (ldi,      "------iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",    "extension of load-immediate") \
  X (imbin,    "iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",    "extension of binary-immediate") \
  X (ldar,     "----------pppppppppppppppppppppppppppppppp",    "extension of branch-relative") \
  X (ldr,      "------pppppppppppppppppppppppppppppppppppp",    "extension of load-immediate") \
  X (brci,     "iiiiiiiiiiiiiiiiiiiiiiiiiipppppppppppppppp",    "extension of compare-with-immediate-and-branch") \
  X (brc,      "---ppppppppppppppppppppppppppppppppppppppp",    "extension of compare-regs-and-branch") \
  X (cni,      "iiiiiiiiiiiiiiiiiiiiiiiiii----------------",    "extension of compare-with-immediate-and-branch") \

enum EInstructionExtensionFormat : uint8_t {
#define INSTRUCTION_EXTENSION_FORMAT_ENUM_X(name, bitdescr, txt) inst_ext_fmt_##name,
    DECLARE_INSTRUCTION_EXTENSION_FORMATS(INSTRUCTION_EXTENSION_FORMAT_ENUM_X)
    inst_ext_fmt_last
};

/***************************************************************************
*
* assembler instructions syntax templates
*
* X(name, fields, descr)
*    1 enumeration name (like asm_fmt_##name)
*    2 array of arguments and delimiters
*    3 format
*
****************************************************************************/
#define DECLARE_ASM_FORMATS(X) \
  X (ldi,        VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_ldi)) \
  X (fldsi,      VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_fldsi)) \
  X (flddi,      VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_flddi)) \
  X (RaxU28,     VARRAY(cf_lprefix, cf_ldi)) \
  X (RaxbI21,    VARRAY(cf_lprefix, cf_gprb, cf_comma, cf_binimm)) \
  X (RaxbU21,    VARRAY(cf_lprefix, cf_gprb, cf_comma, cf_binimmu)) \
  X (RabI21,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_binimm)) \
  X (RabU21,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_binimmu)) \
  X (trap,       VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_binimm)) \
  X (RabB21,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_brc_branch)) \
  X (RbaB21,     VARRAY(cf_lprefix, cf_gprb, cf_comma, cf_gpra, cf_comma, cf_brc_branch))  /* pseudo for RabB21 */ \
  X (RaSbB21,    VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_vshamt_b, cf_comma, cf_brc_branch)) \
  X (RaI14B14,   VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_brc_simm, cf_comma, cf_short_branch)) \
  X (RaU14B14,   VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_brc_uimm, cf_comma, cf_short_branch)) \
  X (loop,       VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_loop_step, cf_comma, cf_short_branch)) \
  X (callr,      VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_branch)) \
  X (brzr,       VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_branch)) \
  X (br,         VARRAY(cf_lprefix, cf_branch)) \
  X (ldar,       VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_ldar)) \
  X (gmemr,      VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_rip_label)) \
  X (write,      VARRAY(cf_lprefix, cf_rip_string)) \
  X (fldsri,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_rip_float32)) \
  X (flddri,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_rip_float64)) \
  X (fldqri,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_rip_float128)) \
  X (fldqi,      VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_float128_low)) \
  X (fldqih,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_float128_high)) \
  X (gmemx,      VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_indexed_scale, cf_comma, cf_indexed_disp)) \
  X (gmemu,      VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_update_stride)) \
  X (RabU14,     VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_gprb, cf_comma, cf_uimm14)) \
  X (Rabcd,      VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_gprd)) \
  X (RabcSd,     VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_shamt_d)) \
  X (FabcSd,     VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_vshamt_d)) \
  X (RabScd,     VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c, cf_comma, cf_shamt_d)) \
  X (RabcSde,    VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_shamt_d, cf_comma, cf_shamt_e)) \
  X (RabScde,    VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c, cf_comma, cf_shamt_d, cf_comma, cf_shamt_e)) \
  X (Rabc,       VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc)) \
  X (Racb,       VARRAY(cf_gpra, cf_comma, cf_gprc, cf_comma, cf_gprb)) /* pseudo for Rabc */ \
  X (RabSc,      VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_shamt_c)) \
  X (Rab,        VARRAY(cf_gpra, cf_comma, cf_gprb)) \
  X (xchg,       VARRAY(cf_gpra, cf_comma, cf_gprb)) \
  X (Ra,         VARRAY(cf_gpra)) \
  X (Rb,         VARRAY(cf_gprb)) \
  X (Rbc,        VARRAY(cf_gprb, cf_comma, cf_gprc)) \
  X (Rcb,        VARRAY(cf_gprc, cf_comma, cf_gprb)) \
  X (r2m2,       VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (RaI14m2,    VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_brc_simm, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (RaU14m2,    VARRAY(cf_lprefix, cf_gpra, cf_comma, cf_brc_uimm, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (RaSbm2,     VARRAY(cf_gpra, cf_comma, cf_vshamt_b, cf_comma, cf_mask_yes, cf_comma, cf_mask_no)) \
  X (gmemux,     VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_indexed_scale)) \
  X (amo,        VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma_opt, cf_memory_ordering)) \
  X (aload,      VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma_opt, cf_memory_ordering)) \
  X (fpcmp,      VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_fp_compare_mode)) \
  X (clmul,      VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_gprc, cf_comma, cf_clmul_index)) \
  X (Round,      VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_rounding_mode)) \
  X (NoArgs,     VARRAY(cf_none)) \
  X (FabSc,      VARRAY(cf_gpra, cf_comma, cf_gprb, cf_comma, cf_vshamt_c)) \
  X (mspr,       VARRAY(cf_gpra, cf_comma, cf_spr)) \
  X (sprsr,      VARRAY(cf_gpra, cf_comma, cf_spr, cf_comma, cf_gprc, cf_comma, cf_gprd)) \


enum EInstructionAssemblerFormat : uint8_t {
#define ASM_FORMAT_ENUM_X(name, fields) asm_fmt_##name,
    DECLARE_ASM_FORMATS(ASM_FORMAT_ENUM_X)
    asm_fmt_last
};

extern const std::array<std::array<EInstructionField, 16>, asm_fmt_last> asm_fmt_field;

/***************************************************************************
* macrotable for instruction subsets
***************************************************************************/
#define DECLARE_INSTRUCTION_SUBSETS(X) \
  X(base) \
  X(fpuq) \
  X(fpud) \
  X(fpus) \
  X(mmx) \
  X(system) \
  X(atomic) \
  X(kernel) \
  X(cipher) \


enum EInstructionSubset : uint8_t {
#define INSTRUCTION_SUBSET_ENUM_X(name) instr_subset_##name,
    DECLARE_INSTRUCTION_SUBSETS(INSTRUCTION_SUBSET_ENUM_X)
    instr_subset_last
};


/***************************************************************************
* instruction hardware types
***************************************************************************/
enum EInstructionHardwareType : uint8_t {
    instr_htype_sw,    // assembler pseudo-instructions
    instr_htype_hw,    // true hardware
    instr_htype_last
};

class CInstructionInfo {
    public:
        uint64_t                       tmplt;
        EInstructionSubset             subset;
        EInstructionHardwareType       hard;
        EInstructionAssemblerFormat    sw_format;
        EInstructionFormat             hw_format;
};

extern const std::array<CInstructionInfo, insn_reserved> asm_info;
extern const std::array<char [12], insn_reserved> asm_name;

inline bool is_static_reg(unsigned int n) { return  n >= base_rotate_regs + max_stacked_framesize; }
inline bool is_rotate_reg(unsigned int n) { return  n <  base_rotate_regs + max_stacked_framesize; }

/**************************************************************************
* ISA/ABI convention for general registers
**************************************************************************/
enum {
    reg_sp    = base_static_regs + 0,    // stack pointer
    reg_fp    = base_static_regs + 1,    // frame pointer
    reg_tp    = base_static_regs + 2,    // thread pointer
    reg_gz    = base_static_regs + 3,    // zero register

    reg_first_free_global = reg_gz + 1
};

/***************************************************************************
*
* special purpose registers
*
***************************************************************************/
enum class ESpecialRegisterSubset : uint8_t {
    reserved,   // not exist
    user,       // user read/write
    ruser,      // user read-only
    debug,
    system,     // system read/write
    interrupt,  // interruptions handler
    lapic,      // local advanced programmable interrupt controller
    rstack
};

enum class ESpecialRegisterRight : uint8_t {
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
    X (ip,       user,         "instruction pointer") \
    X (eip,      user,         "exception instruction pointer") \
    X (fpcr,     user,         "floating-point control register") \
    X (eca,      user,         "exception context address") \
    X (rsc,      rstack,       "register stack control") \
    X (rsp,      rstack,       "register stack pointer") \
    X (psr,      system,       "processor status register") \
    X (kip,      system,       "kernel instruction pointer") \
    X (ksp,      system,       "kernel stack pointer") \
    X (krsp,     system,       "kernel register stack pointer") \
    X (peb,      system,       "process env block") \
    X (teb,      system,       "thread env block") \
    X (itc,      system,       "interval time counter") \
    X (itm,      system,       "interval time match register") \
    X (pta,      system,       "page table address") \
    X (iva,      system,       "interruption vector address") \
    X (iip,      interrupt,    "interruption instruction pointer") \
    X (iipa,     interrupt,    "interruption instruction previous address") \
    X (ipsr,     interrupt,    "interruption processor status register") \
    X (cause,    interrupt,    "interruption cause register") \
    X (ifa,      interrupt,    "interruption faulting address") \
    X (iib,      interrupt,    "interruption instruction bundle") \
    X (irr0,     lapic,        "interrupt requests register 0") \
    X (irr1,     lapic,        "interrupt requests register 1") \
    X (irr2,     lapic,        "interrupt requests register 2") \
    X (irr3,     lapic,        "interrupt requests register 3") \
    X (isr0,     lapic,        "interrupt service register 0") \
    X (isr1,     lapic,        "interrupt service register 1") \
    X (isr2,     lapic,        "interrupt service register 2") \
    X (isr3,     lapic,        "interrupt service register 3") \
    X (iv,       lapic,        "interrupt vector") \
    X (lid,      lapic,        "local identification register") \
    X (tpr,      lapic,        "task priority register") \
    X (itcv,     lapic,        "interval time counter vector") \
    X (tsv,      lapic,        "termal sensor vector") \
    X (pmv,      lapic,        "performance monitor vector") \
    X (cmcv,     lapic,        "correct machine check vector") \

enum ESpecialRegister {
#define SPECIAL_REGISTER_ENUM_X(name, regclass, descr) reg_##name,
    DECLARE_SPECIAL_REGISTERS(SPECIAL_REGISTER_ENUM_X)
    reg_spr_last
};

} // namespace postrisc
