#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static const char label[] = "[[TARGET:.LBB[0-9]+_[0-9]+]]";
static const char jumptable_label[] = "[[TARGET:.LJTI[0-9]+_[0-9]+]]";

static const struct {
    EInstructionID br;
    EInstructionID br_op;
    EInstructionID nul;
    EInstructionID nul_op;
} br_cases[] = {
    { insn_breqw,    insn_brnew,     insn_nuleqw,     insn_nulnew },
    { insn_brltsw,   insn_brgesw,    insn_nulltsw,    insn_nulgesw },
    { insn_brltuw,   insn_brgeuw,    insn_nulltuw,    insn_nulgeuw },
    { insn_breqiw,   insn_brneiw,    insn_nuleqiw,    insn_nulneiw },
    { insn_brltsiw,  insn_brgesiw,   insn_nulltsiw,   insn_nulgesiw },
    { insn_brltuiw,  insn_brgeuiw,   insn_nulltuiw,   insn_nulgeuiw },

    { insn_breqd,    insn_brned,     insn_nuleqd,     insn_nulned },
    { insn_brltsd,   insn_brgesd,    insn_nulltsd,    insn_nulgesd },
    { insn_brltud,   insn_brgeud,    insn_nulltud,    insn_nulgeud },
    { insn_breqid,   insn_brneid,    insn_nuleqid,    insn_nulneid },
    { insn_brltsid,  insn_brgesid,   insn_nulltsid,   insn_nulgesid },
    { insn_brltuid,  insn_brgeuid,   insn_nulltuid,   insn_nulgeuid },

    { insn_breqq,    insn_brneq,     insn_nuleqq,     insn_nulneq },
    { insn_brltsq,   insn_brgesq,    insn_nulltsq,    insn_nulgesq },
    { insn_brltuq,   insn_brgeuq,    insn_nulltuq,    insn_nulgeuq },
    { insn_breqiq,   insn_brneiq,    insn_nuleqiq,    insn_nulneiq },
    { insn_brltsiq,  insn_brgesiq,   insn_nulltsiq,   insn_nulgesiq },
    { insn_brltuiq,  insn_brgeuiq,   insn_nulltuiq,   insn_nulgeuiq },

    { insn_brbs,     insn_brbc,      insn_nulbs,      insn_nulbc },
    { insn_brbsi,    insn_brbci,     insn_nulbsi,     insn_nulbci },
    { insn_brmall,   insn_brmnotall, insn_nulmall,    insn_nulmnotall },
    { insn_brmany,   insn_brmnone,   insn_nulmany,    insn_nulmnone },

    { insn_fbroeqh,  insn_fbruneh,   insn_fnuloeqh,   insn_fnuluneh },
    { insn_fbrolth,  insn_fbrugeh,   insn_fnulolth,   insn_fnulugeh },
    { insn_fbrueqh,  insn_fbroneh,   insn_fnulueqh,   insn_fnuloneh },
    { insn_fbrulth,  insn_fbrogeh,   insn_fnululth,   insn_fnulogeh },
    { insn_fbroh,    insn_fbruh,     insn_fnuloh,     insn_fnuluh },

    { insn_fbroeqs,  insn_fbrunes,   insn_fnuloeqs,   insn_fnulunes },
    { insn_fbrolts,  insn_fbruges,   insn_fnulolts,   insn_fnuluges },
    { insn_fbrueqs,  insn_fbrones,   insn_fnulueqs,   insn_fnulones },
    { insn_fbrults,  insn_fbroges,   insn_fnulults,   insn_fnuloges },
    { insn_fbros,    insn_fbrus,     insn_fnulos,     insn_fnulus },

    { insn_fbroeqd,  insn_fbruned,   insn_fnuloeqd,   insn_fnuluned },
    { insn_fbroltd,  insn_fbruged,   insn_fnuloltd,   insn_fnuluged },
    { insn_fbrueqd,  insn_fbroned,   insn_fnulueqd,   insn_fnuloned },
    { insn_fbrultd,  insn_fbroged,   insn_fnulultd,   insn_fnuloged },
    { insn_fbrod,    insn_fbrud,     insn_fnulod,     insn_fnulud },

    { insn_fbroeqq,  insn_fbruneq,   insn_fnuloeqq,   insn_fnuluneq },
    { insn_fbroltq,  insn_fbrugeq,   insn_fnuloltq,   insn_fnulugeq },
    { insn_fbrueqq,  insn_fbroneq,   insn_fnulueqq,   insn_fnuloneq },
    { insn_fbrultq,  insn_fbrogeq,   insn_fnulultq,   insn_fnulogeq },
    { insn_fbroq,    insn_fbruq,     insn_fnuloq,     insn_fnuluq },
};

static EInstructionID GetNullificationInstructionForBranch(EInstructionID insn)
{
    for (const auto& x : br_cases) {
        if (x.br    == insn) return x.nul;
        if (x.br_op == insn) return x.nul_op;
    }
    std::cerr << "Can't find nullification for branch instruction: " << insn << std::endl;
    assert(!"not branch instruction");
    return insn_undef;
}

static EInstructionID GetOppositeInstruction(EInstructionID insn)
{
    for (const auto& x : br_cases) {
        if (x.br     == insn) return x.br_op;
        if (x.br_op  == insn) return x.br;
        if (x.nul    == insn) return x.nul_op;
        if (x.nul_op == insn) return x.nul;
    }

    std::cerr << "Can't find opposite for: " << insn << std::endl;
    assert(!"not branch or nullification instruction");
    return insn_undef;
}

//===----------------------------------------------------------------------===//
// Branch/Nullify on compare reg-reg
//===----------------------------------------------------------------------===//
static const struct {
    char name[32];
    EInstructionID insn;
    ValueType src1_type;
    ValueType src2_type;
    char args[32];
    char fragment[64];
} BranchRegReg_tests[] = {
    { "eq", insn_breqw,  IW, IW, " %r1, %r2", "a == b" },
    { "ne", insn_brnew,  IW, IW, " %r1, %r2", "a != b" },
    { "lt", insn_brltsw, IW, IW, " %r1, %r2", "a < b"  },
    { "ge", insn_brgesw, IW, IW, " %r1, %r2", "a >= b" },
    { "le", insn_brgesw, IW, IW, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_brltsw, IW, IW, " %r2, %r1", "a > b"  }, //swap
    { "lt", insn_brltuw, UW, UW, " %r1, %r2", "a < b"  },
    { "ge", insn_brgeuw, UW, UW, " %r1, %r2", "a >= b" },
    { "le", insn_brgeuw, UW, UW, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_brltuw, UW, UW, " %r2, %r1", "a > b"  }, //swap

    { "eq", insn_breqd,  ID, ID, " %r1, %r2", "a == b" },
    { "ne", insn_brned,  ID, ID, " %r1, %r2", "a != b" },
    { "lt", insn_brltsd, ID, ID, " %r1, %r2", "a < b"  },
    { "ge", insn_brgesd, ID, ID, " %r1, %r2", "a >= b" },
    { "le", insn_brgesd, ID, ID, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_brltsd, ID, ID, " %r2, %r1", "a > b"  }, //swap
    { "lt", insn_brltud, UD, UD, " %r1, %r2", "a < b"  },
    { "ge", insn_brgeud, UD, UD, " %r1, %r2", "a >= b" },
    { "le", insn_brgeud, UD, UD, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_brltud, UD, UD, " %r2, %r1", "a > b"  }, //swap

    { "eq", insn_breqq,  IQ, IQ, " %r1, %r2", "a == b" },
    { "ne", insn_brneq,  IQ, IQ, " %r1, %r2", "a != b" },
    { "lt", insn_brltsq, IQ, IQ, " %r1, %r2", "a < b"  },
    { "ge", insn_brgesq, IQ, IQ, " %r1, %r2", "a >= b" },
    { "le", insn_brgesq, IQ, IQ, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_brltsq, IQ, IQ, " %r2, %r1", "a > b"  }, //swap
    { "lt", insn_brltuq, UQ, UQ, " %r1, %r2", "a < b"  },
    { "ge", insn_brgeuq, UQ, UQ, " %r1, %r2", "a >= b" },
    { "le", insn_brgeuq, UQ, UQ, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_brltuq, UQ, UQ, " %r2, %r1", "a > b"  }, //swap
};

void Generator::CompareRegRegAndBranchOrNullify(std::ostream& out, bool nullify)
{
    out << "//===----------------------------------------------------------------------===//\n";
    out << "// compare reg-reg and " << (nullify ? "nullify" : "branch") << " instructions\n";
    out << "//===----------------------------------------------------------------------===//\n\n";

    for (const auto& test : BranchRegReg_tests) {
        std::ostringstream name;
        name << (nullify ? "nullify_rr_" : "branch_rr_") << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << '\n';

        // br_ge_imm_i32 %r1, 0, LABEL
        if (nullify) {
            const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
            out << CHECK << InsnCounter(this, insn) << test.args << ", 2, 1\n";
        } else {
            out << CHECK << InsnCounter(this, test.insn) << test.args << ", " << label << '\n';
        }

        // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b)\n";
        out << "{\n";
        if (nullify) {
            out << "    CHECK_NULLIFICATION(" << test.fragment << ");\n";
            // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
            out << "    printf(\"some trick after nullification\");\n";
        } else {
            out << "    CHECK_BRANCH(" << test.fragment << ");\n";
        }
        out << "}\n\n";
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

            out << CHECK_LABEL << name.str() << '\n';
            if (nullify) {
                const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
                out << CHECK << InsnCounter(this, insn)
                             << (imm.mode == LONG ? ".l" : "")
                             << " %r1, " << imm.value << ", 2, 1\n";
            } else {
                out << CHECK << InsnCounter(this, test.insn)
                             << (imm.mode == LONG ? ".l" : "")
                             << " %r1, " << imm.value << ", " << label << '\n';
            }

            out << "void " << name.str()  << "(" << test.type << " value) {\n";
            const TYPE cpp_imm = imm.value - test.offset;
            if (nullify) {
                out << "    CHECK_NULLIFICATION(" << test.c_fragment << " " << cpp_imm << ");\n";
                // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
                out << "    printf(\"some trick after nullification\");\n";
            } else {
                out << "    CHECK_BRANCH(" << test.c_fragment << " " << cpp_imm << ");\n";
            }
            out << "}\n\n";
        }
    }
}

//===----------------------------------------------------------------------===//
// signed:
// x <= imm ===> x <  imm+1 (1 offset)
// x >  imm ===> x >= imm+1 (1 offset)
// unsigned x == imm ===> signed x == imm
// unsigned x != imm ===> signed x != imm
//===----------------------------------------------------------------------===//
static const func_info branch_imm_i32[]  = {
    { "eq", insn_breqiw,  IW, "value == ", 0 },
    { "ne", insn_brneiw,  IW, "value != ", 0 },
    { "eq", insn_breqiw,  UW, "value == ", 0 },
    { "ne", insn_brneiw,  UW, "value != ", 0 },

    { "lt", insn_brltsiw, IW, "value <  ", 0 },
    { "ge", insn_brgesiw, IW, "value >= ", 0 },
    { "le", insn_brltsiw, IW, "value <= ", 1 },
    { "gt", insn_brgesiw, IW, "value >  ", 1 },
};

static const func_info branch_imm_i64[]  = {
    { "eq", insn_breqid, ID, "value == ", 0 },
    { "ne", insn_brneid, ID, "value != ", 0 },
    { "eq", insn_breqid, UD, "value == ", 0 },
    { "ne", insn_brneid, UD, "value != ", 0 },

    { "lt", insn_brltsid, ID, "value <  ", 0 },
    { "ge", insn_brgesid, ID, "value >= ", 0 },
    { "le", insn_brltsid, ID, "value <= ", 1 },
    { "gt", insn_brgesid, ID, "value >  ", 1 },
};

static const func_info branch_imm_i128[] = {
    { "eq", insn_breqiq, IQ, "value == ", 0 },
    { "ne", insn_brneiq, IQ, "value != ", 0 },
    { "eq", insn_breqiq, UQ, "value == ", 0 },
    { "ne", insn_brneiq, UQ, "value != ", 0 },

    { "lt", insn_brltsiq, IQ, "value <  ", 0 },
    { "ge", insn_brgesiq, IQ, "value >= ", 0 },
    { "le", insn_brltsiq, IQ, "value <= ", 1 },
    { "gt", insn_brgesiq, IQ, "value >  ", 1 },
};

static const func_info branch_imm_u32[]  = {
    { "lt", insn_brltuiw, UW, "value <  ", 0 },
    { "ge", insn_brgeuiw, UW, "value >= ", 0 },
    { "le", insn_brltuiw, UW, "value <= ", 1 },
    { "gt", insn_brgeuiw, UW, "value >  ", 1 },
};

static const func_info branch_imm_u64[]  = {
    { "lt", insn_brltuid, UD, "value <  ", 0 },
    { "ge", insn_brgeuid, UD, "value >= ", 0 },
    { "le", insn_brltuid, UD, "value <= ", 1 },
    { "gt", insn_brgeuid, UD, "value >  ", 1 },
};

static const func_info branch_imm_u128[] = {
    { "lt", insn_brltuiq, UQ, "value <  ", 0 },
    { "ge", insn_brgeuiq, UQ, "value >= ", 0 },
    { "le", insn_brltuiq, UQ, "value <= ", 1 },
    { "gt", insn_brgeuiq, UQ, "value >  ", 1 },
};

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
static const struct {
    char name[8];
    EInstructionID insn;
    ValueType type;
    char fragment[16];
    int  offset;
} BranchRegZero_tests[] = {
    { "eq", insn_breqiw, IW, "value == 0", 0 },
    { "ne", insn_brneiw, IW, "value != 0", 0 },
    { "eq", insn_breqiw, UW, "value == 0", 0 }, // unsigned x==0 => signed x==0
    { "ne", insn_brneiw, UW, "value != 0", 0 }, // unsigned x!=0 => signed x!=0

    { "eq", insn_breqid, ID, "value == 0", 0 },
    { "ne", insn_brneid, ID, "value != 0", 0 },
    { "eq", insn_breqid, UD, "value == 0", 0 }, // unsigned x==0 => signed x==0
    { "ne", insn_brneid, UD, "value != 0", 0 }, // unsigned x!=0 => signed x!=0

    { "eq", insn_breqiq, IQ, "value == 0", 0 },
    { "ne", insn_brneiq, IQ, "value != 0", 0 },
    { "eq", insn_breqiq, UQ, "value == 0", 0 }, // unsigned x==0 => signed x==0
    { "ne", insn_brneiq, UQ, "value != 0", 0 }, // unsigned x!=0 => signed x!=0

    { "lt", insn_brltsiw, IW, "value <  0", 0 },
    { "ge", insn_brgesiw, IW, "value >= 0", 0 },
    { "le", insn_brltsiw, IW, "value <= 0", 1 }, // x <= 0 => x <  1
    { "gt", insn_brgesiw, IW, "value >  0", 1 }, // x >  0 => x >= 1
    { "gt", insn_brneiw,  UW, "value >  0", 0 }, // unsigned x >0 => signed x!=0

    { "lt", insn_brltsid, ID, "value <  0", 0 },
    { "ge", insn_brgesid, ID, "value >= 0", 0 },
    { "le", insn_brltsid, ID, "value <= 0", 1 }, // x <= 0 => x <  1
    { "gt", insn_brgesid, ID, "value >  0", 1 }, // x >  0 => x >= 1
    { "gt", insn_brneid,  UD, "value >  0", 0 }, // unsigned x >0 => signed x!=0

    { "lt", insn_brltsiq, IQ, "value <  0", 0 },
    { "ge", insn_brgesiq, IQ, "value >= 0", 0 },
    { "le", insn_brltsiq, IQ, "value <= 0", 1 }, // x <= 0 => x <  1
    { "gt", insn_brgesiq, IQ, "value >  0", 1 }, // x >  0 => x >= 1
    { "gt", insn_brneiq,  UQ, "value >  0", 0 }, // unsigned x >0 => signed x!=0
};

CODEGEN_HANDLER(BranchCompareZero)
{
    for (const auto& test : BranchRegZero_tests) {
        std::ostringstream name;
        name << "branch_zero_" << test.name << "_" << test.type;

        out << CHECK_LABEL << name.str() << '\n';

        // CHECK: br_ge_imm_i32 %r1, 0, LABEL
        out << CHECK << InsnCounter(this, test.insn) << " %r1, " << test.offset << ", " << label << '\n';

        // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
        out << "void " << name.str()  << "(" << test.type << " value) {\n";
        out << "    CHECK_BRANCH(" << test.fragment << ");\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(NullifyCompareZero)
{
    for (const auto& test : BranchRegZero_tests) {
        std::ostringstream name;
        name << "nullify_zero_" << test.name << "_" << test.type;

        out << CHECK_LABEL << name.str() << '\n';

        const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
        out << CHECK << InsnCounter(this, insn) << " %r1, " << test.offset << ", 2, 1\n";

        out << "void " << name.str()  << "(" << test.type << " value) {\n";
        out << "    CHECK_NULLIFICATION(" << test.fragment << ");\n";
        // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
        out << "    printf(\"some trick after nullification\");\n";
        out << "}\n\n";
    }
}

//===----------------------------------------------------------------------===//
// FP fast-math Branch/Nullify on compare
//===----------------------------------------------------------------------===//
static const struct {
    char name[32];
    EInstructionID insn;
    ValueType src1_type;
    ValueType src2_type;
    char args[32];
    char fragment[64];
} FpBranchRegReg_tests[] = {
    { "eq", insn_fbrueqs, FS, FS, " %r1, %r2", "a == b" },
    { "ne", insn_fbrunes, FS, FS, " %r1, %r2", "a != b" },
    { "lt", insn_fbrults, FS, FS, " %r1, %r2", "a < b"  },
    { "ge", insn_fbruges, FS, FS, " %r1, %r2", "a >= b" },
    { "le", insn_fbruges, FS, FS, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_fbrults, FS, FS, " %r2, %r1", "a > b"  }, //swap

    { "eq", insn_fbrueqd, FD, FD, " %r1, %r2", "a == b" },
    { "ne", insn_fbruned, FD, FD, " %r1, %r2", "a != b" },
    { "lt", insn_fbrultd, FD, FD, " %r1, %r2", "a < b"  },
    { "ge", insn_fbruged, FD, FD, " %r1, %r2", "a >= b" },
    { "le", insn_fbruged, FD, FD, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_fbrultd, FD, FD, " %r2, %r1", "a > b"  }, //swap

    { "eq", insn_fbrueqq, FQ, FQ, " %r1, %r2", "a == b" },
    { "ne", insn_fbruneq, FQ, FQ, " %r1, %r2", "a != b" },
    { "lt", insn_fbrultq, FQ, FQ, " %r1, %r2", "a < b"  },
    { "ge", insn_fbrugeq, FQ, FQ, " %r1, %r2", "a >= b" },
    { "le", insn_fbrugeq, FQ, FQ, " %r2, %r1", "a <= b" }, //swap
    { "gt", insn_fbrultq, FQ, FQ, " %r2, %r1", "a > b"  }, //swap
};

CODEGEN_HANDLER(FpBranchCompare)
{
    for (const auto& test : FpBranchRegReg_tests) {
        std::ostringstream name;
        name << "fp_branch_check_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << InsnCounter(this, test.insn) << test.args << ", " << label << '\n';

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b) {\n";
        out << "    CHECK_BRANCH(" << test.fragment << ");\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(FpBranchCompareGoto)
{
    for (const auto& test : FpBranchRegReg_tests) {
        std::ostringstream name;
        name << "fp_branch_goto_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << InsnCounter(this, GetOppositeInstruction(test.insn)) << test.args << ", " << label << '\n';

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b) {\n";
        out << "  if (" << test.fragment << ") { // goto skip;\n";
        out << "  printf(\"test " << name.str() << " " << test.fragment << "\\n\");\n";
        out << "  } //skip: ;\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(NullifyFpCompare)
{
    out << "//===----------------------------------------------------------------------===//\n";
    out << "// compare reg-reg and nullify instructions\n";
    out << "//===----------------------------------------------------------------------===//\n\n";

    for (const auto& test : FpBranchRegReg_tests) {
        std::ostringstream name;
        name << "fp_nullification_rr_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << '\n';

        const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);

        out << CHECK << InsnCounter(this, insn) << test.args << ", 2, 1\n";

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b)\n";
        out << "{\n";
        out << "    CHECK_NULLIFICATION(" << test.fragment << ");\n";
        // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
        out << "    printf(\"some trick after nullification\");\n";
        out << "}\n\n";
    }
}

//===----------------------------------------------------------------------===//
// FP Strict Branch/Nullify on compare
// FIXME: 2 !__builtin_isnan's it is: ONE ===> UNE
//===----------------------------------------------------------------------===//
static const struct {
    char name[32];
    EInstructionID insn;
    ValueType src1_type;
    ValueType src2_type;
    char args[32];
    char fragment[64];
} FpBranchStrictRegReg_tests[] = {
    { "oeq", insn_fbroeqh, FH, FH, " %r1, %r2", "a == b" },
    { "one", insn_fbroneh, FH, FH, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" },
    { "olt", insn_fbrolth, FH, FH, " %r1, %r2", "a < b"  },
    { "oge", insn_fbrogeh, FH, FH, " %r1, %r2", "a >= b" },
    { "ole", insn_fbrogeh, FH, FH, " %r2, %r1", "a <= b" }, /*swap*/
    { "ogt", insn_fbrolth, FH, FH, " %r2, %r1", "a > b"  }, /*swap*/
    { "ueq", insn_fbrueqh, FH, FH, " %r1, %r2", "!(a < b || a > b)" }, /* trick to force UEQ */
    { "une", insn_fbruneh, FH, FH, " %r1, %r2", "!(a == b)" },
    { "ult", insn_fbrulth, FH, FH, " %r1, %r2", "!(a >= b)"  },
    { "ule", insn_fbrugeh, FH, FH, " %r2, %r1", "!(a > b)" }, /*swap*/
    { "ugt", insn_fbrulth, FH, FH, " %r2, %r1", "!(a <= b)"  }, /*swap*/
    { "uge", insn_fbrugeh, FH, FH, " %r1, %r2", "!(a < b)" },
    { "u",   insn_fbruh,   FH, FH, " %r1, %r2", "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "o",   insn_fbroh,   FH, FH, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b)" },

    { "oeq", insn_fbroeqs, FS, FS, " %r1, %r2", "a == b" },
    { "one", insn_fbrones, FS, FS, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" },
    { "olt", insn_fbrolts, FS, FS, " %r1, %r2", "a < b"  },
    { "oge", insn_fbroges, FS, FS, " %r1, %r2", "a >= b" },
    { "ole", insn_fbroges, FS, FS, " %r2, %r1", "a <= b" }, /*swap*/
    { "ogt", insn_fbrolts, FS, FS, " %r2, %r1", "a > b"  }, /*swap*/
    { "ueq", insn_fbrueqs, FS, FS, " %r1, %r2", "!(a < b || a > b)" }, /* trick to force UEQ */
    { "une", insn_fbrunes, FS, FS, " %r1, %r2", "!(a == b)" },
    { "ult", insn_fbrults, FS, FS, " %r1, %r2", "!(a >= b)"  },
    { "ule", insn_fbruges, FS, FS, " %r2, %r1", "!(a > b)" }, /*swap*/
    { "ugt", insn_fbrults, FS, FS, " %r2, %r1", "!(a <= b)"  }, /*swap*/
    { "uge", insn_fbruges, FS, FS, " %r1, %r2", "!(a < b)" },
    { "u",   insn_fbrus,   FS, FS, " %r1, %r2", "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "o",   insn_fbros,   FS, FS, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b)" },

    { "oeq", insn_fbroeqd, FD, FD, " %r1, %r2", "a == b" },
    { "one", insn_fbroned, FD, FD, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" },
    { "olt", insn_fbroltd, FD, FD, " %r1, %r2", "a < b"  },
    { "oge", insn_fbroged, FD, FD, " %r1, %r2", "a >= b" },
    { "ole", insn_fbroged, FD, FD, " %r2, %r1", "a <= b" }, /*swap*/
    { "ogt", insn_fbroltd, FD, FD, " %r2, %r1", "a > b"  }, /*swap*/
    { "ueq", insn_fbrueqd, FD, FD, " %r1, %r2", "!(a < b || a > b)" }, /* trick to force UEQ */
    { "une", insn_fbruned, FD, FD, " %r1, %r2", "!(a == b)" },
    { "ult", insn_fbrultd, FD, FD, " %r1, %r2", "!(a >= b)"  },
    { "ule", insn_fbruged, FD, FD, " %r2, %r1", "!(a > b)" }, /*swap*/
    { "ugt", insn_fbrultd, FD, FD, " %r2, %r1", "!(a <= b)"  }, /*swap*/
    { "uge", insn_fbruged, FD, FD, " %r1, %r2", "!(a < b)" },
    { "u",   insn_fbrud,   FD, FD, " %r1, %r2", "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "o",   insn_fbrod,   FD, FD, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b)" },

    { "oeq", insn_fbroeqq, FQ, FQ, " %r1, %r2", "a == b" },
    { "one", insn_fbroneq, FQ, FQ, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b) && a != b" },
    { "olt", insn_fbroltq, FQ, FQ, " %r1, %r2", "a < b"  },
    { "oge", insn_fbrogeq, FQ, FQ, " %r1, %r2", "a >= b" },
    { "ole", insn_fbrogeq, FQ, FQ, " %r2, %r1", "a <= b" }, /*swap*/
    { "ogt", insn_fbroltq, FQ, FQ, " %r2, %r1", "a > b"  }, /*swap*/
    { "ueq", insn_fbrueqq, FQ, FQ, " %r1, %r2", "!(a < b || a > b)" }, /* trick to force UEQ */
    { "une", insn_fbruneq, FQ, FQ, " %r1, %r2", "!(a == b)" },
    { "ult", insn_fbrultq, FQ, FQ, " %r1, %r2", "!(a >= b)"  },
    { "ule", insn_fbrugeq, FQ, FQ, " %r2, %r1", "!(a > b)" }, /*swap*/
    { "ugt", insn_fbrultq, FQ, FQ, " %r2, %r1", "!(a <= b)"  }, /*swap*/
    { "uge", insn_fbrugeq, FQ, FQ, " %r1, %r2", "!(a < b)" },
    { "u",   insn_fbruq,   FQ, FQ, " %r1, %r2", "__builtin_isnan(a) || __builtin_isnan(b)" },
    { "o",   insn_fbroq,   FQ, FQ, " %r1, %r2", "!__builtin_isnan(a) && !__builtin_isnan(b)" },
};

CODEGEN_HANDLER(FpBranchCompareStrict)
{
    for (const auto& test : FpBranchStrictRegReg_tests) {
        std::ostringstream name;
        name << "fp_branch_strict_check_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << '\n';
        out << CHECK << InsnCounter(this, test.insn) << test.args << ", " << label << '\n';

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b) {\n";
        out << "    CHECK_BRANCH(" << test.fragment << ");\n";
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(NullifyFpCompareStrict)
{
    for (const auto& test : FpBranchStrictRegReg_tests) {
        std::ostringstream name;
        name << "fp_nullification_strict_rr_" << test.name << "_" << test.src1_type << "_" << test.src2_type;

        out << CHECK_LABEL << name.str() << '\n';

        const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);

        out << CHECK << InsnCounter(this, insn) << test.args << ", 2, 1\n";

        out << "void " << name.str()  << "(" << test.src1_type << " a, " << test.src2_type << " b)\n";
        out << "{\n";
        out << "    CHECK_NULLIFICATION(" << test.fragment << ");\n";
        // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
        out << "    printf(\"some trick after nullification\");\n";
        out << "}\n\n";
    }
}

//===----------------------------------------------------------------------===//
// Branch/Nullify ob bit test
//===----------------------------------------------------------------------===//
static const struct {
    EInstructionID insn;
    ValueType type1;
    ValueType type2;
    char insn_fragment[32];
    char fragment[64];
} BranchOnBit_tests[] = {
    { insn_brbs,      IW, IW, " %r1, %r2",          "a & (1 << b)" },
    { insn_brbc,      IW, IW, " %r1, %r2",          "(a & (1 << b)) == 0"  },
    { insn_brbs,      IW, IW, " %r1, %r2",          "(a >> b) & 1" },
    { insn_brbs,      IW, IW, " %r1, %r2",          "(a & (1 << b)) == (1 << b)" },
    { insn_brbc,      IW, IW, " %r1, %r2",          "(a & (1 << b)) != (1 << b)" },
    { insn_brbci,     IW, IW, " %r1, 15",           "((a >> 15) & 1) == 0" },
    { insn_brbsi,     IW, IW, " %r1, 14",           "((a >> 14) & 1) == 1" },
    { insn_brbsi,     IW, IW, " %r1, 24",           "a & (1 << 24)" },
    { insn_brbsi,     IW, IW, " %r1, 10",           "a & (1 << 10)" },
    { insn_brbci,     IW, IW, " %r1, 10",           "(a & (1 << 10)) == 0" },
    { insn_brbsi,     IW, IW, " %r1, 10",           "(a & (1 << 10)) == (1 << 10)" },
    { insn_brbsi,     IW, IW, " %r1, 20",           "a & (1 << 20)" },
    { insn_brbci,     IW, IW, " %r1, 20",           "(a & (1 << 20)) == 0" },
    { insn_brbsi,     IW, IW, " %r1, 20",           "(a & (1 << 20)) == (1 << 20)" },
    { insn_brmall,    IW, IW, " %r1, 100",          "(a & 100) == 100" },
    { insn_brmnotall, IW, IW, " %r1, 100",          "(a & 100) != 100" },
    { insn_brmany,    IW, IW, " %r1, 100",          "(a & 100) != 0" },
    { insn_brmnone,   IW, IW, " %r1, 100",          "(a & 100) == 0" },
    { insn_brmall,    IW, IW, ".l %r1, 10000",      "(a & 10000) == 10000" },
    { insn_brmnotall, IW, IW, ".l %r1, 10000",      "(a & 10000) != 10000" },
    { insn_brmany,    IW, IW, ".l %r1, 10000",      "(a & 10000) != 0" },
    { insn_brmnone,   IW, IW, ".l %r1, 10000",      "(a & 10000) == 0" },
    { insn_brmall,    IW, IW, ".l %r1, 10000000",   "(a & 10000000) == 10000000" },
    { insn_brmnotall, IW, IW, ".l %r1, 10000000",   "(a & 10000000) != 10000000" },
    { insn_brmany,    IW, IW, ".l %r1, 10000000",   "(a & 10000000) != 0" },
    { insn_brmnone,   IW, IW, ".l %r1, 10000000",   "(a & 10000000) == 0" },

    { insn_brbs,      ID, ID, " %r1, %r2",          "a & (1LL << b)" },
    { insn_brbc,      ID, ID, " %r1, %r2",          "(a & (1LL << b)) == 0"  },
    { insn_brbs,      ID, ID, " %r1, %r2",          "(a >> b) & 1LL" },
    { insn_brbs,      ID, ID, " %r1, %r2",          "(a & (1LL << b)) == (1LL << b)" },
    { insn_brbc,      ID, ID, " %r1, %r2",          "(a & (1LL << b)) != (1LL << b)" },
    { insn_brbci,     ID, ID, " %r1, 15",           "((a >> 15) & 1LL) == 0" },
    { insn_brbsi,     ID, ID, " %r1, 14",           "((a >> 14) & 1LL) == 1" },
    { insn_brbsi,     ID, ID, " %r1, 24",           "a & (1LL << 24)" },
    { insn_brbsi,     ID, ID, " %r1, 10",           "a & (1LL << 10)" },
    { insn_brbci,     ID, ID, " %r1, 10",           "(a & (1LL << 10)) == 0" },
    { insn_brbsi,     ID, ID, " %r1, 10",           "(a & (1LL << 10)) == (1LL << 10)" },
    { insn_brbsi,     ID, ID, " %r1, 50",           "a & (1LL << 50)" },
    { insn_brbci,     ID, ID, " %r1, 50",           "(a & (1LL << 50)) == 0" },
    { insn_brbsi,     ID, ID, " %r1, 50",           "(a & (1LL << 50)) == (1LL << 50)" },
    { insn_brmall,    ID, ID, " %r1, 100",          "(a & 100) == 100" },
    { insn_brmnotall, ID, ID, " %r1, 100",          "(a & 100) != 100" },
    { insn_brmany,    ID, ID, " %r1, 100",          "(a & 100) != 0" },
    { insn_brmnone,   ID, ID, " %r1, 100",          "(a & 100) == 0" },
    { insn_brmall,    ID, ID, ".l %r1, 10000",      "(a & 10000) == 10000" },
    { insn_brmnotall, ID, ID, ".l %r1, 10000",      "(a & 10000) != 10000" },
    { insn_brmany,    ID, ID, ".l %r1, 10000",      "(a & 10000) != 0" },
    { insn_brmnone,   ID, ID, ".l %r1, 10000",      "(a & 10000) == 0" },
    { insn_brmall,    ID, ID, ".l %r1, 10000000",   "(a & 10000000) == 10000000" },
    { insn_brmnotall, ID, ID, ".l %r1, 10000000",   "(a & 10000000) != 10000000" },
    { insn_brmany,    ID, ID, ".l %r1, 10000000",   "(a & 10000000) != 0" },
    { insn_brmnone,   ID, ID, ".l %r1, 10000000",   "(a & 10000000) == 0" },

    { insn_brbs,      IQ, IQ, " %r1, %r2",          "a & ((i128)1 << b)" },
    { insn_brbc,      IQ, IQ, " %r1, %r2",          "(a & ((i128)1 << b)) == 0"  },
    { insn_brbs,      IQ, IQ, " %r1, %r2",          "(a >> b) & 1" },
    { insn_brbs,      IQ, IQ, " %r1, %r2",          "(a & ((i128)1 << b)) == ((i128)1 << b)" },
    { insn_brbc,      IQ, IQ, " %r1, %r2",          "(a & ((i128)1 << b)) != ((i128)1 << b)" },
    { insn_brbci,     IQ, IQ, " %r1, 15",           "((a >> 15) & 1) == 0" },
    { insn_brbsi,     IQ, IQ, " %r1, 14",           "((a >> 14) & 1) == 1" },
    { insn_brbsi,     IQ, IQ, " %r1, 24",           "a & ((i128)1 << 24)" },
    { insn_brbsi,     IQ, IQ, " %r1, 10",           "a & ((i128)1 << 10)" },
    { insn_brbci,     IQ, IQ, " %r1, 10",           "(a & ((i128)1 << 10)) == 0" },
    { insn_brbsi,     IQ, IQ, " %r1, 10",           "(a & ((i128)1 << 10)) == ((i128)1 << 10)" },
    { insn_brbsi,     IQ, IQ, " %r1, 80",           "a & ((i128)1 << 80)" },
    { insn_brbci,     IQ, IQ, " %r1, 80",           "(a & ((i128)1 << 80)) == 0" },
    { insn_brbsi,     IQ, IQ, " %r1, 80",           "(a & ((i128)1 << 80)) == ((i128)1 << 80)" },
    { insn_brmall,    IQ, IQ, " %r1, 100",          "(a & 100) == 100" },
    { insn_brmnotall, IQ, IQ, " %r1, 100",          "(a & 100) != 100" },
    { insn_brmany,    IQ, IQ, " %r1, 100",          "(a & 100) != 0" },
    { insn_brmnone,   IQ, IQ, " %r1, 100",          "(a & 100) == 0" },
    { insn_brmall,    IQ, IQ, ".l %r1, 10000",      "(a & 10000) == 10000" },
    { insn_brmnotall, IQ, IQ, ".l %r1, 10000",      "(a & 10000) != 10000" },
    { insn_brmany,    IQ, IQ, ".l %r1, 10000",      "(a & 10000) != 0" },
    { insn_brmnone,   IQ, IQ, ".l %r1, 10000",      "(a & 10000) == 0" },
    { insn_brmall,    IQ, IQ, ".l %r1, 10000000",   "(a & 10000000) == 10000000" },
    { insn_brmnotall, IQ, IQ, ".l %r1, 10000000",   "(a & 10000000) != 10000000" },
    { insn_brmany,    IQ, IQ, ".l %r1, 10000000",   "(a & 10000000) != 0" },
    { insn_brmnone,   IQ, IQ, ".l %r1, 10000000",   "(a & 10000000) == 0" },

    { insn_brbsi,      ID, IW, " %r1, 44",   "a & (1LL << 44)" },
    { insn_brbsi,      IW, IW, " %r1, 15",   "(a & 0xFFFF7FFF) != a" },
    { insn_brbci,      IW, IW, " %r1, 15",   "(a & 0xFFFF7FFF) == a" },
    { insn_brbsi,      ID, IW, " %r1, 47",   "(a & 0xFFFF7FFFFFFFFFFF) != a" },
    { insn_brbci,      ID, IW, " %r1, 47",   "(a & 0xFFFF7FFFFFFFFFFF) == a" },
    { insn_brbsi,      IQ, IW, " %r1, 70",   "a & ((i128)1 << 70)" },
    { insn_brbsi,      IQ, IW, " %r1, 120",  "a & ((i128)1 << 120)" },
    { insn_brbsi,      IQ, IW, " %r1, 44",   "a & (1LL << 44)" },
};

void Generator::CompareRegBitAndBranchOrNullify(std::ostream& out, bool nullify)
{
    out << "//===----------------------------------------------------------------------===//\n";
    out << "// compare reg-bit and " << (nullify ? "nullify" : "branch") << " instructions\n";
    out << "//===----------------------------------------------------------------------===//\n\n";

    int counter = 0;
    for (const auto& test : BranchOnBit_tests) {

        std::ostringstream name;
        name << (nullify ? "nullification_bit_" : "branch_bit_") << test.type1 << "_" << test.type2 << "_" << counter;
        ++counter;

        out << CHECK_LABEL << name.str() << '\n';
        if (nullify) {
            const EInstructionID insn = GetNullificationInstructionForBranch(test.insn);
            out << CHECK << InsnCounter(this, insn) << test.insn_fragment << ", 2, 1\n";
        } else {
            // br_bs_imm_i32 %r1, bitnum, LABEL
            out << CHECK << InsnCounter(this, test.insn) << test.insn_fragment << ", " << label << '\n';
        }

        // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
        out << "void " << name.str()  << "(" << test.type1 << " a, " << test.type2 << " b) {\n";
        if (nullify) {
            out << "    CHECK_NULLIFICATION(" << test.fragment << ");\n";
            // FIXME: nullification doesn't work if MBB contains return, so add another unmergeable big block (printf)
            out << "    printf(\"some trick after nullification\");\n";
        } else {
            out << "    CHECK_BRANCH(" << test.fragment << ");\n";
        }
        out << "}\n\n";
    }
}

CODEGEN_HANDLER(BranchOnBit) { CompareRegBitAndBranchOrNullify(out, false); }
CODEGEN_HANDLER(NullificationOnBit) { CompareRegBitAndBranchOrNullify(out, true); }

CODEGEN_HANDLER(ComputedGoto)
{
    static const char func[] = "test_cgoto";

    out <<
    CHECK_LABEL << func << "\n"
    "int " << func << "(int c)\n"
    "{\n"
    "    void *targets[] = { &&label_a, &&label_b };\n"
    << CHECK << InsnCounter(this, insn_jmpr) << " %r1, %gz, 0\n"
    "    goto *targets[c];\n"
    "label_a:\n"
    "    printf(\"Label A\\n\");\n"
    "    return 0;\n"
    "label_b:\n"
    "    printf(\"Label B\\n\");\n"
    "    return 0;\n"
    "}\n";
}

CODEGEN_HANDLER(SwitchTable)
{
    for (const auto Vt : {IW, UW, ID, UD}) {
        std::ostringstream name;
        name << "test_switch_table_" << Vt;

        out << CHECK_LABEL << name.str() << '\n';
        out << "int " << name.str() << "(" << Vt << " c)\n";
        out << "{\n";
        out << CHECK << InsnCounter(this, insn_allocsp) << " 5, 16\n";
        if (Vt == IW || Vt == UW) {
            out << CHECK << InsnCounter(this, insn_brgeuiw) << " %r1, 6, " << label << "\n";
            out << CHECK << InsnCounter(this, insn_zextw) << " %r1, %r1\n";
        } else {
            out << CHECK << InsnCounter(this, insn_brgeuid) << " %r1, 6, " << label << "\n";
        }
        out << CHECK << InsnCounter(this, insn_ldard) << " %r2, %pcfwd(" << jumptable_label << ")\n";
        out << CHECK << InsnCounter(this, insn_jmpt) << " %r2, %r1\n";

        out << "    switch (c) {\n";
        out << "      case 0:    printf(\"case 0 %lld %lld\", c+1, c-1);    break;\n";
        out << "      case 1:    printf(\"case 1 %lld %lld\", c+1, c&122);  break;\n";
        out << "      case 2:    printf(\"case 2 %lld %lld\", c+1, c|2);    break;\n";
        out << "      case 3:    printf(\"case 3 %lld %lld\", c+1, c^2);    break;\n";
        out << "      case 4:    printf(\"case 4 %lld %lld\", c+1, c*2);    break;\n";
        out << "      case 5:    printf(\"case 5 %lld %lld\", c+1, c+22);   break;\n";
        out << "      default: return 0;\n";
        out << "    }\n";
        out << "    return 0;\n";
        out << "}\n";
    }
}

CODEGEN_HANDLER(SwitchTableNonzero)
{
    for (const auto Vt : {IW, UW, ID, UD}) {
        std::ostringstream name;
        name << "test_switch_table_nonzero_" << Vt;

        out << CHECK_LABEL << name.str() << '\n';
        out << "int " << name.str() << "(" << Vt << " c)\n";
        out << "{\n";
        out << CHECK << InsnCounter(this, insn_allocsp) << " 5, 16\n";
        if (Vt == IW || Vt == UW) {
            out << CHECK << InsnCounter(this, insn_addiws) << " %r2, %r1, -10\n";
            out << CHECK << InsnCounter(this, insn_brgeuiw) << " %r2, 6, " << label << "\n";
            out << CHECK << InsnCounter(this, insn_addiwz) << " %r1, %r1, -10\n"; // FIXME: addiwz - merge or remove
        } else {
            out << CHECK << InsnCounter(this, insn_addid) << " %r1, %r1, -10\n";
            out << CHECK << InsnCounter(this, insn_brgeuid) << " %r1, 6, " << label << "\n";
        }
        out << CHECK << InsnCounter(this, insn_ldard) << " %r2, %pcfwd(" << jumptable_label << ")\n";
        out << CHECK << InsnCounter(this, insn_jmpt) << " %r2, %r1\n";

        out << "    switch (c) {\n";
        out << "      case 10:    printf(\"case 10 %lld %lld\", c+1, c-1);    break;\n";
        out << "      case 11:    printf(\"case 11 %lld %lld\", c+1, c&122);  break;\n";
        out << "      case 12:    printf(\"case 12 %lld %lld\", c+1, c|2);    break;\n";
        out << "      case 13:    printf(\"case 13 %lld %lld\", c+1, c^2);    break;\n";
        out << "      case 14:    printf(\"case 14 %lld %lld\", c+1, c*2);    break;\n";
        out << "      case 15:    printf(\"case 15 %lld %lld\", c+1, c+22);   break;\n";
        out << "      default: return 0;\n";
        out << "    }\n";
        out << "    return 0;\n";
        out << "}\n";
    }
}

} // namespace llvm
} // namespace postrisc
