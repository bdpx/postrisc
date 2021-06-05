#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

static const char label[] = "[[TARGET:.LBB[0-9]+_[0-9]+]]";

enum BranchMode { NONE, SWAP, PLUS_1 };

CODEGEN_HANDLER(BranchCompare)
{
    static const struct {
        char type;
        char op[4];
        char func[4];
        char insn_func[4];
        BranchMode mode;
    } ops[] = {
        { 'i', "==", "eq", "eq", NONE },
        { 'i', "!=", "ne", "ne", NONE },
        { 'i', "<",  "lt", "lt", NONE },
        { 'i', ">=", "ge", "ge", NONE },
        { 'i', "<=", "le", "ge", SWAP }, // x <= y => y >= x
        { 'i', ">",  "gt", "lt", SWAP }, // x > y => y < x
        { 'u', "<",  "lt", "lt", NONE },
        { 'u', ">=", "ge", "ge", NONE },
        { 'u', "<=", "le", "ge", SWAP }, // x <= y => y >= x
        { 'u', ">",  "gt", "lt", SWAP }, // x > y => y < x
    };

    for (const int size : {32, 64, 128}) {
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << "// compare reg-reg and branch instructions" << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << std::endl;

        for (const auto& op : ops) {
            std::ostringstream type;
            type << op.type << size;

            std::ostringstream name;
            name << "branch_rr_" << op.func << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: br_ge_imm_i32 %r1, 0, LABEL
            out << "// CHECK: br_" << op.insn_func << "_" << type.str();
            if (op.mode == SWAP) {
                out << " %r2, %r1";
            } else {
                out << " %r1, %r2";
            }
            out << ", " << label << std::endl;

            // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
            out << "void " << name.str()  << "(" << type.str() << " a, " << type.str() << " b) {"<< std::endl;
            out << "    CHECK_BRANCH(a " << op.op << " b);"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }
    }
}

template<typename ITYPE, typename UTYPE> static void BranchCompareImm(std::ostream& out, int size)
{
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// compare register and nonzero " << size << "-bit immediate then branch instructions" << std::endl;
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << std::endl;

    static const struct {
        char type;
        char asm_type;
        char op[4];
        char func[4];
        char insn_func[4];
        BranchMode mode;
    } imm_ops[] = {
        { 'i', 'i', "==", "eq", "eq", NONE },
        { 'i', 'i', "!=", "ne", "ne", NONE },
        { 'i', 'i', "<",  "lt", "lt", NONE },
        { 'i', 'i', ">=", "ge", "ge", NONE },
        { 'i', 'i', "<=", "le", "lt", PLUS_1 }, // x <= 0 => x <  1
        { 'i', 'i', ">",  "gt", "ge", PLUS_1 }, // x >  0 => x >= 1
        { 'u', 'i', "==", "eq", "eq", NONE },
        { 'u', 'i', "!=", "ne", "ne", NONE },
        { 'u', 'u', "<",  "lt", "lt", NONE },
        { 'u', 'u', ">=", "ge", "ge", NONE },
        { 'u', 'u', "<=", "le", "lt", PLUS_1 }, // x <= 0 => x <  1
        { 'u', 'u', ">",  "gt", "ge", PLUS_1 }, // x >  0 => x >= 1
    };

    for (const auto& op : imm_ops) {
        std::ostringstream type;
        type << op.type << size;

        if (op.asm_type == 'i') {
            for (const auto& imm : immediates<ITYPE>::branch_immediates) {
                // branch_imm_iXX_minus_1000
                std::ostringstream name;
                name << "branch_imm_" << op.func << "_" << type.str() << "_";
                if (imm.value < 0) {
                    name << "minus_" << -(i64)imm.value;
                } else {
                    name << "plus_" << imm.value;
                }

                out << "// CHECK-LABEL: @" << name.str() << std::endl;

                // CHECK: br_op_imm_i64.l %r1, imm, LABEL
                out << "// CHECK: br_" << op.insn_func << "_imm_" << op.asm_type << size;
                if (imm.mode == LONG) {
                    out << ".l";
                }
                out << " %r1, " << imm.value << ", " << label << std::endl;

                // void br_ge_imm_i64(i64 a) { CHECK_BRANCH(a >= 0); }
                out << "void " << name.str()  << "(" << type.str() << " value) {"<< std::endl;
                const i64 cpp_imm = (op.mode == PLUS_1) ? (imm.value - 1) : imm.value;
                out << "    CHECK_BRANCH(value " << op.op << " " << cpp_imm << ");"<< std::endl;
                out << "}"<< std::endl;
                out << std::endl;
            }
        } else {
            for (const auto& imm : immediates<UTYPE>::branch_immediates) {
                std::ostringstream name;
                name << "branch_imm_" << op.func << "_" << type.str() << "_" << imm.value;

                out << "// CHECK-LABEL: @" << name.str() << std::endl;

                // CHECK: br_op_imm_i64.l %r1, imm, LABEL
                out << "// CHECK: br_" << op.insn_func << "_imm_" << op.asm_type << size;
                if (imm.mode == LONG) {
                    out << ".l";
                }
                // FIXME: llvm uses signed only immediates?
                out << " %r1, " << imm.value << ", " << label << std::endl;

                // void br_ge_zero_i64(i64 a) { CHECK_BRANCH(a >= 0); }
                out << "void " << name.str()  << "(" << type.str() << " value) {"<< std::endl;
                const u64 cpp_imm = (op.mode == PLUS_1) ? (imm.value - 1) : imm.value;
                out << "    CHECK_BRANCH(value " << op.op << " " << cpp_imm << ");"<< std::endl;
                out << "}"<< std::endl;
                out << std::endl;
            }
        }
    }
}

CODEGEN_HANDLER(BranchCompareImm32)  { BranchCompareImm<i32, u32>(out, 32); }
CODEGEN_HANDLER(BranchCompareImm64)  { BranchCompareImm<i64, u64>(out, 64); }
CODEGEN_HANDLER(BranchCompareImm128) { BranchCompareImm<i64, u64>(out, 128); }

CODEGEN_HANDLER(BranchCompareZero)
{
    static const struct {
        char type;
        char asm_type;
        char op[4];
        char func[4];
        char insn_func[4];
        BranchMode mode;
    } ops[] = {
        { 'i', 'i', "==", "eq", "eq", NONE },
        { 'i', 'i', "!=", "ne", "ne", NONE },
        { 'i', 'i', "<",  "lt", "lt", NONE },
        { 'i', 'i', ">=", "ge", "ge", NONE },
        { 'i', 'i', "<=", "le", "lt", PLUS_1 }, // x <= 0 => x <  1
        { 'i', 'i', ">",  "gt", "ge", PLUS_1 }, // x >  0 => x >= 1
        { 'u', 'i', "==", "eq", "eq", NONE }, // unsigned x==0 => signed x==0
        { 'u', 'i', "!=", "ne", "ne", NONE }, // unsigned x!=0 => signed x!=0
        { 'u', 'i', ">",  "gt", "ne", NONE }, // unsigned x>0 => signed x!=0
    };

    for (const int size : {32, 64, 128}) {
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << "// " << size << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << std::endl;

        for (const auto& op : ops) {
            std::ostringstream type;
            type << op.type << size;

            std::ostringstream name;
            name << "branch_zero_" << op.func << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: br_ge_imm_i32 %r1, 0, LABEL
            out << "// CHECK: br_" << op.insn_func << "_imm_" << op.asm_type << size << " %r1, ";
            if (op.mode == PLUS_1) {
                out << "1";
            } else {
                out << "0";
            }
            out << ", " << label << std::endl;

            // void br_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
            out << "void " << name.str()  << "(" << type.str() << " value) {"<< std::endl;
            out << "    CHECK_BRANCH(value " << op.op << " 0);"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }
    }
}

CODEGEN_HANDLER(FpBranchCompare)
{
    static const struct {
        char op[4];
        char func[4];
        char insn_func[4];
        BranchMode mode;
    } ops[] = {
        { "==", "eq", "eq", NONE },
        { "!=", "ne", "ne", NONE },
        { "<",  "lt", "lt", NONE },
        { ">=", "ge", "ge", NONE },
        { "<=", "le", "ge", SWAP }, // x <= y => y >= x
        { ">",  "gt", "lt", SWAP }, // x > y => y < x
    };

    static const struct {
        char op[4];
        char func[4];
        char insn_func[4];
        BranchMode mode;
    } ordered_ops[] = {
        { "<",  "lt", "lt", NONE },
        { ">=", "ge", "ge", NONE },
        { "<=", "le", "ge", SWAP }, // x <= y => y >= x
        { ">",  "gt", "lt", SWAP }, // x > y => y < x
    };


    for (const int size : {32, 64, 128}) {
        std::ostringstream type;
        type << 'f' << size;

        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << "// " << type.str() << " compare and branch instructions" << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << std::endl;

        for (const auto& op : ops) {
            std::ostringstream name;
            name << "fp_branch_unordered_" << op.func << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: br_oge_f32 %r1, %r2, LABEL
            out << "// CHECK: br_u" << op.insn_func << "_" << type.str();
            if (op.mode == SWAP) {
                out << " %r2, %r1";
            } else {
                out << " %r1, %r2";
            }
            out << ", " << label << std::endl;

            // void fp_branch_ge_f32(f32 a, f32 b) { CHECK_BRANCH(a >= b); }
            out << "void " << name.str()  << "(" << type.str() << " a, " << type.str() << " b) {"<< std::endl;
            out << "    CHECK_BRANCH(a " << op.op << " b);"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }

        for (const auto& op : ordered_ops) {
            std::ostringstream name;
            name << "fp_branch_ordered_" << op.func << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: br_oge_f32 %r1, %r2, LABEL
            out << "// CHECK: br_o" << op.insn_func << "_" << type.str();
            if (op.mode == SWAP) {
                out << " %r2, %r1";
            } else {
                out << " %r1, %r2";
            }
            out << ", " << label << std::endl;

            // void fp_branch_ge_f32(f32 a, f32 b) { CHECK_BRANCH(a >= b); }
            out << "void " << name.str()  << "(" << type.str() << " a, " << type.str() << " b) {"<< std::endl;
            out << "  if (a " << op.op << " b) goto skip;"<< std::endl;
            out << "  printf(\"test " << name.str() << " " << op.op << "\\n\");"<< std::endl;
            out << "skip: ;"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }
    }
}

} // namespace llvm
} // namespace postrisc
