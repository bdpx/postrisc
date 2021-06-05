#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static const char label[] = "[[TARGET:.LBB[0-9]+_[0-9]+]]";

//===----------------------------------------------------------------------===//
// Branch/Nullify on compare reg-reg
//===----------------------------------------------------------------------===//

#define BRANCH_REG_REG_TESTS(ITYPE, UTYPE) \
    { "eq", insn_br_eq_ ## ITYPE, #ITYPE, #ITYPE, " %r1, %r2", "a == b" }, \
    { "ne", insn_br_ne_ ## ITYPE, #ITYPE, #ITYPE, " %r1, %r2", "a != b" }, \
    { "lt", insn_br_lt_ ## ITYPE, #ITYPE, #ITYPE, " %r1, %r2", "a < b"  }, \
    { "ge", insn_br_ge_ ## ITYPE, #ITYPE, #ITYPE, " %r1, %r2", "a >= b" }, \
    { "le", insn_br_ge_ ## ITYPE, #ITYPE, #ITYPE, " %r2, %r1", "a <= b" }, /*swap*/ \
    { "gt", insn_br_lt_ ## ITYPE, #ITYPE, #ITYPE, " %r2, %r1", "a > b"  }, /*swap*/ \
    { "lt", insn_br_lt_ ## UTYPE, #UTYPE, #UTYPE, " %r1, %r2", "a < b"  }, \
    { "ge", insn_br_ge_ ## UTYPE, #UTYPE, #UTYPE, " %r1, %r2", "a >= b" }, \
    { "le", insn_br_ge_ ## UTYPE, #UTYPE, #UTYPE, " %r2, %r1", "a <= b" }, /*swap*/ \
    { "gt", insn_br_lt_ ## UTYPE, #UTYPE, #UTYPE, " %r2, %r1", "a > b"  }, /*swap*/ \

static const struct {
    char name[32];
    EInstructionID insn;
    char src1_type[8];
    char src2_type[8];
    char args[32];
    char fragment[64];
} BranchRegReg_tests[] = {
    BRANCH_REG_REG_TESTS(i32,  u32)
    BRANCH_REG_REG_TESTS(i64,  u64)
    BRANCH_REG_REG_TESTS(i128, u128)
};

void Generator::CompareRegRegAndBranchOrNullify(std::ostream& out, bool nullify)
{
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// compare reg-reg and " << (nullify ? "nullify" : "branch") << " instructions" << std::endl;
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << std::endl;

    for (const auto& test : BranchRegReg_tests) {
        std::ostringstream name;
        name << (nullify ? "nullify_rr_" : "branch_rr_") << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << std::endl;

        // br_ge_imm_i32 %r1, 0, LABEL
        if (nullify) {
            const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
            out << CHECK << InsnCounter(this, GetOppositeInstruction(insn)) << test.args << ", 1, 2" << std::endl;
        } else {
            out << CHECK << InsnCounter(this, test.insn) << test.args << ", " << label << std::endl;
        }

        // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b)" << std::endl;
        out << "{"<< std::endl;
        if (nullify) {
            out << "    CHECK_NULLIFICATION(" << test.fragment << ");"<< std::endl;
            // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
            out << "    printf(\"some trick after nullification\");" << std::endl;
        } else {
            out << "    CHECK_BRANCH(" << test.fragment << ");"<< std::endl;
        }
        out << "}"<< std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(BranchCompareRegReg) { CompareRegRegAndBranchOrNullify(out, false); }
CODEGEN_HANDLER(NullifyCompareRegReg) { CompareRegRegAndBranchOrNullify(out, true); }

//===----------------------------------------------------------------------===//
// Branch/Nullify on compare reg-imm
//===----------------------------------------------------------------------===//
template<typename TYPE>
void Generator::CompareImmediateAndBranchOrNullify(std::ostream& out,
    const std::span<const func_info> tests, bool nullify)
{
    for (const auto& test : tests) {
        for (const auto& imm : immediates<TYPE>::branch_immediates) {
            std::ostringstream name;
            name << (nullify ? "nullify_imm_" : "branch_imm_") << test.name << "_" << test.type;

            if constexpr (std::is_signed_v<TYPE>) {
                if (imm.value < 0) {
                    name << "_m" << -imm.value;
                } else {
                    name << "_p" << imm.value;
                }
            } else {
                name << "_" << imm.value;
            }

            out << CHECK_LABEL << name.str() << std::endl;
            if (nullify) {
                const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
                out << CHECK << InsnCounter(this, GetOppositeInstruction(insn))
                             << (imm.mode == LONG ? ".l" : "")
                             << " %r1, " << imm.value << ", 1, 2" << std::endl;
            } else {
                out << CHECK << InsnCounter(this, test.insn)
                             << (imm.mode == LONG ? ".l" : "")
                             << " %r1, " << imm.value << ", " << label << std::endl;
            }

            out << "void " << name.str()  << "(" << test.type << " value) {"<< std::endl;
            const TYPE cpp_imm = imm.value - test.offset;
            if (nullify) {
                out << "    CHECK_NULLIFICATION(" << test.c_fragment << " " << cpp_imm << ");"<< std::endl;
                // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
                out << "    printf(\"some trick after nullification\");" << std::endl;
            } else {
                out << "    CHECK_BRANCH(" << test.c_fragment << " " << cpp_imm << ");"<< std::endl;
            }
            out << "}"<< std::endl;
            out << std::endl;
        }
    }
}

#define BRANCH_REG_SIMM_TESTS(ITYPE, UTYPE) \
    { "eq", insn_br_eq_imm_ ## ITYPE, #ITYPE, "value == ", 0 }, \
    { "ne", insn_br_ne_imm_ ## ITYPE, #ITYPE, "value != ", 0 }, \
    { "lt", insn_br_lt_imm_ ## ITYPE, #ITYPE, "value <  ", 0 }, \
    { "ge", insn_br_ge_imm_ ## ITYPE, #ITYPE, "value >= ", 0 }, \
    { "le", insn_br_lt_imm_ ## ITYPE, #ITYPE, "value <= ", 1 }, /* x <= imm ===> x <  imm+1 */ \
    { "gt", insn_br_ge_imm_ ## ITYPE, #ITYPE, "value >  ", 1 }, /* x >  imm ===> x >= imm+1 */ \
    { "eq", insn_br_eq_imm_ ## ITYPE, #UTYPE, "value == ", 0 }, /* unsigned x == imm ===> signed x == imm */ \
    { "ne", insn_br_ne_imm_ ## ITYPE, #UTYPE, "value != ", 0 }, /* unsigned x != imm ===> signed x != imm */ \

#define BRANCH_REG_UIMM_TESTS(UTYPE) \
    { "lt", insn_br_lt_imm_ ## UTYPE, #UTYPE, "value <  ", 0 }, \
    { "ge", insn_br_ge_imm_ ## UTYPE, #UTYPE, "value >= ", 0 }, \
    { "le", insn_br_lt_imm_ ## UTYPE, #UTYPE, "value <= ", 1 }, /* x <= imm ===> x <  imm+1 */ \
    { "gt", insn_br_ge_imm_ ## UTYPE, #UTYPE, "value >  ", 1 }, /* x >  imm ===> x >= imm+1 */ \

static const func_info branch_imm_i32[]  = { BRANCH_REG_SIMM_TESTS(i32,  u32) };
static const func_info branch_imm_i64[]  = { BRANCH_REG_SIMM_TESTS(i64,  u64) };
static const func_info branch_imm_i128[] = { BRANCH_REG_SIMM_TESTS(i128, u128) };

static const func_info branch_imm_u32[]  = { BRANCH_REG_UIMM_TESTS(u32) };
static const func_info branch_imm_u64[]  = { BRANCH_REG_UIMM_TESTS(u64) };
static const func_info branch_imm_u128[] = { BRANCH_REG_UIMM_TESTS(u128) };

CODEGEN_HANDLER(BranchCompareImmI32)  { CompareImmediateAndBranchOrNullify<i32>(out, branch_imm_i32, false); }
CODEGEN_HANDLER(BranchCompareImmI64)  { CompareImmediateAndBranchOrNullify<i64>(out, branch_imm_i64, false); }
CODEGEN_HANDLER(BranchCompareImmI128) { CompareImmediateAndBranchOrNullify<i64>(out, branch_imm_i128, false); }

CODEGEN_HANDLER(BranchCompareImmU32)  { CompareImmediateAndBranchOrNullify<u32>(out, branch_imm_u32, false); }
CODEGEN_HANDLER(BranchCompareImmU64)  { CompareImmediateAndBranchOrNullify<u64>(out, branch_imm_u64, false); }
CODEGEN_HANDLER(BranchCompareImmU128) { CompareImmediateAndBranchOrNullify<u64>(out, branch_imm_u128, false); }

CODEGEN_HANDLER(NullifyCompareImmI32)  { CompareImmediateAndBranchOrNullify<i32>(out, branch_imm_i32, true); }
CODEGEN_HANDLER(NullifyCompareImmI64)  { CompareImmediateAndBranchOrNullify<i64>(out, branch_imm_i64, true); }
CODEGEN_HANDLER(NullifyCompareImmI128) { CompareImmediateAndBranchOrNullify<i64>(out, branch_imm_i128, true); }

CODEGEN_HANDLER(NullifyCompareImmU32)  { CompareImmediateAndBranchOrNullify<u32>(out, branch_imm_u32, true); }
CODEGEN_HANDLER(NullifyCompareImmU64)  { CompareImmediateAndBranchOrNullify<u64>(out, branch_imm_u64, true); }
CODEGEN_HANDLER(NullifyCompareImmU128) { CompareImmediateAndBranchOrNullify<u64>(out, branch_imm_u128, true); }

//===----------------------------------------------------------------------===//
// Branch/Nullify on compare reg-zero
//===----------------------------------------------------------------------===//

#define BRANCH_REG_ZERO_TESTS(ITYPE, UTYPE) \
    { "eq", insn_br_eq_imm_ ## ITYPE, #ITYPE, "value == 0", 0 }, \
    { "ne", insn_br_ne_imm_ ## ITYPE, #ITYPE, "value != 0", 0 }, \
    { "lt", insn_br_lt_imm_ ## ITYPE, #ITYPE, "value <  0", 0 }, \
    { "ge", insn_br_ge_imm_ ## ITYPE, #ITYPE, "value >= 0", 0 }, \
    { "le", insn_br_lt_imm_ ## ITYPE, #ITYPE, "value <= 0", 1 }, /* x <= 0 => x <  1 */ \
    { "gt", insn_br_ge_imm_ ## ITYPE, #ITYPE, "value >  0", 1 }, /* x >  0 => x >= 1 */ \
    { "eq", insn_br_eq_imm_ ## ITYPE, #UTYPE, "value == 0", 0 }, /* unsigned x==0 => signed x==0 */ \
    { "ne", insn_br_ne_imm_ ## ITYPE, #UTYPE, "value != 0", 0 }, /* unsigned x!=0 => signed x!=0 */ \
    { "gt", insn_br_ne_imm_ ## ITYPE, #UTYPE, "value >  0", 0 }, /* unsigned x >0 => signed x!=0 */ \

static const struct {
    char name[8];
    EInstructionID insn;
    char type[8];
    char fragment[16];
    int  offset;
} BranchRegZero_tests[] = {
    BRANCH_REG_ZERO_TESTS(i32,  u32)
    BRANCH_REG_ZERO_TESTS(i64,  u64)
    BRANCH_REG_ZERO_TESTS(i128, u128)
};

CODEGEN_HANDLER(BranchCompareZero)
{
    for (const auto& test : BranchRegZero_tests) {
        std::ostringstream name;
        name << "branch_zero_" << test.name << "_" << test.type;

        out << CHECK_LABEL << name.str() << std::endl;

        // CHECK: br_ge_imm_i32 %r1, 0, LABEL
        out << CHECK << InsnCounter(this, test.insn) << " %r1, " << test.offset << ", " << label << std::endl;

        // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
        out << "void " << name.str()  << "(" << test.type << " value) {"<< std::endl;
        out << "    CHECK_BRANCH(" << test.fragment << ");"<< std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(NullifyCompareZero)
{
    for (const auto& test : BranchRegZero_tests) {
        std::ostringstream name;
        name << "nullify_zero_" << test.name << "_" << test.type;

        out << CHECK_LABEL << name.str() << std::endl;

        const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
        out << CHECK << InsnCounter(this, GetOppositeInstruction(insn)) << " %r1, " << test.offset << ", 1, 2" << std::endl;

        out << "void " << name.str()  << "(" << test.type << " value) {"<< std::endl;
        out << "    CHECK_NULLIFICATION(" << test.fragment << ");"<< std::endl;
        // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
        out << "    printf(\"some trick after nullification\");" << std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

//===----------------------------------------------------------------------===//
// FP fast-math Branch/Nullify on compare
//===----------------------------------------------------------------------===//

#define FP_BRANCH_REG_REG_TESTS(TYPE) \
    { "eq", insn_br_ueq_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "a == b" }, \
    { "ne", insn_br_une_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "a != b" }, \
    { "lt", insn_br_ult_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "a < b"  }, \
    { "ge", insn_br_uge_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "a >= b" }, \
    { "le", insn_br_uge_ ## TYPE, #TYPE, #TYPE, " %r2, %r1", "a <= b" }, /*swap*/ \
    { "gt", insn_br_ult_ ## TYPE, #TYPE, #TYPE, " %r2, %r1", "a > b"  }, /*swap*/ \

static const struct {
    char name[32];
    EInstructionID insn;
    char src1_type[8];
    char src2_type[8];
    char args[32];
    char fragment[64];
} FpBranchRegReg_tests[] = {
    FP_BRANCH_REG_REG_TESTS(f32)
    FP_BRANCH_REG_REG_TESTS(f64)
    FP_BRANCH_REG_REG_TESTS(f128)
};

CODEGEN_HANDLER(FpBranchCompare)
{
    for (const auto& test : FpBranchRegReg_tests) {
        std::ostringstream name;
        name << "fp_branch_check_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << InsnCounter(this, test.insn) << test.args << ", " << label << std::endl;

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b) {"<< std::endl;
        out << "    CHECK_BRANCH(" << test.fragment << ");"<< std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(FpBranchCompareGoto)
{
    for (const auto& test : FpBranchRegReg_tests) {
        std::ostringstream name;
        name << "fp_branch_goto_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << InsnCounter(this, GetOppositeInstruction(test.insn)) << test.args << ", " << label << std::endl;

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b) {"<< std::endl;
        out << "  if (" << test.fragment << ") { // goto skip;"<< std::endl;
        out << "  printf(\"test " << name.str() << " " << test.fragment << "\\n\");"<< std::endl;
        out << "  } //skip: ;"<< std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(NullifyFpCompare)
{
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// compare reg-reg and nullify instructions" << std::endl;
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << std::endl;

    for (const auto& test : FpBranchRegReg_tests) {
        std::ostringstream name;
        name << "fp_nullification_rr_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << std::endl;

        const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);

        out << CHECK << InsnCounter(this, GetOppositeInstruction(insn)) << test.args << ", 1, 2" << std::endl;

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b)" << std::endl;
        out << "{"<< std::endl;
        out << "    CHECK_NULLIFICATION(" << test.fragment << ");"<< std::endl;
        // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
        out << "    printf(\"some trick after nullification\");" << std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

//===----------------------------------------------------------------------===//
// FP Strict Branch/Nullify on compare
//===----------------------------------------------------------------------===//

#define FP_BRANCH_STRICT_REG_REG_TESTS(TYPE) \
    { "oeq", insn_br_oeq_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "a == b" }, \
    { "one", insn_br_one_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" },/*FIXME: 2 !__builtin_isnan's it is: ONE ===> UNE*/ \
    { "olt", insn_br_olt_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "a < b"  }, \
    { "oge", insn_br_oge_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "a >= b" }, \
    { "ole", insn_br_oge_ ## TYPE, #TYPE, #TYPE, " %r2, %r1", "a <= b" }, /*swap*/ \
    { "ogt", insn_br_olt_ ## TYPE, #TYPE, #TYPE, " %r2, %r1", "a > b"  }, /*swap*/ \
    { "ueq", insn_br_ueq_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "!(a < b || a > b)" }, /* trick to force UEQ */ \
    { "une", insn_br_une_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "!(a == b)" }, \
    { "ult", insn_br_ult_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "!(a >= b)"  }, \
    { "ule", insn_br_uge_ ## TYPE, #TYPE, #TYPE, " %r2, %r1", "!(a > b)" }, /*swap*/ \
    { "ugt", insn_br_ult_ ## TYPE, #TYPE, #TYPE, " %r2, %r1", "!(a <= b)"  }, /*swap*/ \
    { "uge", insn_br_uge_ ## TYPE, #TYPE, #TYPE, " %r1, %r2", "!(a < b)" }, \
    { "u",   insn_br_u_   ## TYPE, #TYPE, #TYPE, " %r1, %r2", "__builtin_isnan(a) || __builtin_isnan(b)" }, \
    { "o",   insn_br_o_   ## TYPE, #TYPE, #TYPE, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b)" }, \

static const struct {
    char name[32];
    EInstructionID insn;
    char src1_type[8];
    char src2_type[8];
    char args[32];
    char fragment[64];
} FpBranchStrictRegReg_tests[] = {
    FP_BRANCH_STRICT_REG_REG_TESTS(f32)
    FP_BRANCH_STRICT_REG_REG_TESTS(f64)
    FP_BRANCH_STRICT_REG_REG_TESTS(f128)
};

CODEGEN_HANDLER(FpBranchCompareStrict)
{
    for (const auto& test : FpBranchStrictRegReg_tests) {
        std::ostringstream name;
        name << "fp_branch_strict_check_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << std::endl;
        out << CHECK << InsnCounter(this, test.insn) << test.args << ", " << label << std::endl;

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b) {"<< std::endl;
        out << "    CHECK_BRANCH(" << test.fragment << ");"<< std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(NullifyFpCompareStrict)
{
    for (const auto& test : FpBranchStrictRegReg_tests) {
        std::ostringstream name;
        name << "fp_nullification_strict_rr_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << std::endl;

        const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);

        out << CHECK << InsnCounter(this, GetOppositeInstruction(insn)) << test.args << ", 1, 2" << std::endl;

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b)" << std::endl;
        out << "{"<< std::endl;
        out << "    CHECK_NULLIFICATION(" << test.fragment << ");"<< std::endl;
        // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
        out << "    printf(\"some trick after nullification\");" << std::endl;
        out << "}"<< std::endl;
        out << std::endl;
    }
}

//===----------------------------------------------------------------------===//
// Branch/Nullify ob bit test
//===----------------------------------------------------------------------===//

#define BRANCH_ON_BIT_TESTS(TYPE, ONE, BITNUM) \
    { insn_br_bs,          #TYPE,  "i32", " %r1, %r2",          "a & (" ONE " << b)" }, \
    { insn_br_bc,          #TYPE,  "i32", " %r1, %r2",          "(a & (" ONE " << b)) == 0"  }, \
    { insn_br_bs,          #TYPE,  "i32", " %r1, %r2",          "(a >> b) & 1" }, \
    { insn_br_bs,          #TYPE,  "i32", " %r1, %r2",          "(a & (" ONE " << b)) == (" ONE " << b)" }, \
    { insn_br_bc,          #TYPE,  "i32", " %r1, %r2",          "(a & (" ONE " << b)) != (" ONE " << b)" }, \
    { insn_br_bc_imm,      #TYPE,  "i32", " %r1, 15",           "((a >> 15) & 1) == 0" }, \
    { insn_br_bs_imm,      #TYPE,  "i32", " %r1, 14",           "((a >> 14) & 1) == 1" }, \
    { insn_br_bs_imm,      #TYPE,  "i32", " %r1, 24",           "a & (1 << 24)" }, \
    { insn_br_bs_imm,      #TYPE,  "i32", " %r1, 10",           "a & (1 << 10)" }, \
    { insn_br_bc_imm,      #TYPE,  "i32", " %r1, 10",           "(a & (1 << 10)) == 0" }, \
    { insn_br_bs_imm,      #TYPE,  "i32", " %r1, 10",           "(a & (1 << 10)) == (1 << 10)" }, \
    { insn_br_bs_imm,      #TYPE,  "i32", " %r1, " #BITNUM,     "a & (" ONE " << " #BITNUM ")" }, \
    { insn_br_bc_imm,      #TYPE,  "i32", " %r1, " #BITNUM,     "(a & (" ONE " << " #BITNUM ")) == 0" }, \
    { insn_br_bs_imm,      #TYPE,  "i32", " %r1, " #BITNUM,     "(a & (" ONE " << " #BITNUM ")) == (" ONE " << " #BITNUM ")" }, \
    { insn_br_mask_all,    #TYPE,  "i32", " %r1, 100",          "(a & 100) == 100" }, \
    { insn_br_mask_notall, #TYPE,  "i32", " %r1, 100",          "(a & 100) != 100" }, \
    { insn_br_mask_any,    #TYPE,  "i32", " %r1, 100",          "(a & 100) != 0" }, \
    { insn_br_mask_none,   #TYPE,  "i32", " %r1, 100",          "(a & 100) == 0" }, \
    { insn_br_mask_all,    #TYPE,  "i32", ".l %r1, 10000",      "(a & 10000) == 10000" }, \
    { insn_br_mask_notall, #TYPE,  "i32", ".l %r1, 10000",      "(a & 10000) != 10000" }, \
    { insn_br_mask_any,    #TYPE,  "i32", ".l %r1, 10000",      "(a & 10000) != 0" }, \
    { insn_br_mask_none,   #TYPE,  "i32", ".l %r1, 10000",      "(a & 10000) == 0" }, \
    { insn_br_mask_all,    #TYPE,  "i32", ".l %r1, 10000000",   "(a & 10000000) == 10000000" }, \
    { insn_br_mask_notall, #TYPE,  "i32", ".l %r1, 10000000",   "(a & 10000000) != 10000000" }, \
    { insn_br_mask_any,    #TYPE,  "i32", ".l %r1, 10000000",   "(a & 10000000) != 0" }, \
    { insn_br_mask_none,   #TYPE,  "i32", ".l %r1, 10000000",   "(a & 10000000) == 0" }, \

static const struct {
    EInstructionID insn;
    char type1[8];
    char type2[8];
    char insn_fragment[32];
    char fragment[64];
} BranchOnBit_tests[] = {
    BRANCH_ON_BIT_TESTS(i32,  "1", 20)
    BRANCH_ON_BIT_TESTS(i64,  "1LL", 50)
    BRANCH_ON_BIT_TESTS(i128, "(i128)1", 80)
    { insn_br_bs_imm,      "i64",  "i32", " %r1, 44",    "a & (1LL << 44)" },
    { insn_br_bs_imm,      "i32",  "i32", " %r1, 15",    "(a & 0xFFFF7FFF) != a" }, \
    { insn_br_bc_imm,      "i32",  "i32", " %r1, 15",    "(a & 0xFFFF7FFF) == a" }, \
    { insn_br_bs_imm,      "i64",  "i32", " %r1, 47",    "(a & 0xFFFF7FFFFFFFFFFF) != a" },
    { insn_br_bc_imm,      "i64",  "i32", " %r1, 47",    "(a & 0xFFFF7FFFFFFFFFFF) == a" },
    { insn_br_bs_imm,      "i128", "i32", " %r1, 70",    "a & ((i128)1 << 70)" },
    { insn_br_bs_imm,      "i128", "i32", " %r1, 120",   "a & ((i128)1 << 120)" },
    { insn_br_bs_imm,      "i128",  "i32", " %r1, 44",   "a & (1LL << 44)" },
};

void Generator::CompareRegBitAndBranchOrNullify(std::ostream& out, bool nullify)
{
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// compare reg-bit and " << (nullify ? "nullify" : "branch") << " instructions" << std::endl;
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << std::endl;

    int counter = 0;
    for (const auto& test : BranchOnBit_tests) {

        std::ostringstream name;
        name << (nullify ? "nullification_bit_" : "branch_bit_") << test.type1 << "_" << test.type2 << "_" << counter;
        ++counter;

        out << CHECK_LABEL << name.str() << std::endl;
        if (nullify) {
            const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
            out << CHECK << InsnCounter(this, GetOppositeInstruction(insn)) << test.insn_fragment << ", 1, 2" << std::endl;
        } else {
            // br_bs_imm_i32 %r1, bitnum, LABEL
            out << CHECK << InsnCounter(this, test.insn) << test.insn_fragment << ", " << label << std::endl;
        }

        // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
        out << "void " << name.str()  << "(" << test.type1 << " a, " << test.type2 << " b) {"<< std::endl;
        if (nullify) {
            out << "    CHECK_NULLIFICATION(" << test.fragment << ");"<< std::endl;
            // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
            out << "    printf(\"some trick after nullification\");" << std::endl;
        } else {
            out << "    CHECK_BRANCH(" << test.fragment << ");"<< std::endl;
        }
        out << "}"<< std::endl;
        out << std::endl;
    }
}

CODEGEN_HANDLER(BranchOnBit) { CompareRegBitAndBranchOrNullify(out, false); }
CODEGEN_HANDLER(NullificationOnBit) { CompareRegBitAndBranchOrNullify(out, true); }

} // namespace llvm
} // namespace postrisc
