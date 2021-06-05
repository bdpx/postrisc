#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

enum BranchMode { NONE, SWAP, PLUS_1 };

CODEGEN_HANDLER(CompareRegReg)
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
        out << "// compare reg-reg instructions" << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << std::endl;

        for (const auto& op : ops) {
            std::ostringstream type;
            type << op.type << size;

            std::ostringstream name;
            name << "compare_rr_" << op.func << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: cmp_ge_imm_i32 %r1, %r1, 0, LABEL
            out << "// CHECK: cmp_" << op.insn_func << "_" << type.str() << " %r1,";
            if (op.mode == SWAP) {
                out << " %r2, %r1";
            } else {
                out << " %r1, %r2";
            }
            out << std::endl;

            // void compare_ge_zero_i32(i32 a) { CHECK_BRANCH(a >= 0); }
            out << "bool " << name.str()  << "(" << type.str() << " a, " << type.str() << " b) {"<< std::endl;
            out << "    return (a " << op.op << " b);"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }
    }
}

template<typename ITYPE, typename UTYPE> static void CompareRegImm(std::ostream& out, int size)
{
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// compare register and nonzero " << size << "-bit immediate instructions" << std::endl;
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
            for (const auto& imm : immediates<ITYPE>::binimm_immediates) {
                // compare_imm_iXX_minus_1000
                std::ostringstream name;
                name << "compare_imm_" << op.func << "_" << type.str() << "_";
                if (imm.value < 0) {
                    name << "minus_" << -(i64)imm.value;
                } else {
                    name << "plus_" << imm.value;
                }

                out << "// CHECK-LABEL: @" << name.str() << std::endl;

                // CHECK: br_op_imm_i64.l %r1, imm, LABEL
                out << "// CHECK: cmp_" << op.insn_func << "_imm_" << op.asm_type << size;
                if (imm.mode == LONG) {
                    out << ".l";
                }
                out << " %r1, %r1, " << imm.value << std::endl;

                // void br_ge_imm_i64(i64 a) { CHECK_BRANCH(a >= 0); }
                out << "bool " << name.str()  << "(" << type.str() << " value) {"<< std::endl;
                const i64 cpp_imm = (op.mode == PLUS_1) ? (imm.value - 1) : imm.value;
                out << "    return (value " << op.op << " " << cpp_imm << ");"<< std::endl;
                out << "}"<< std::endl;
                out << std::endl;
            }
        } else {
            for (const auto& imm : immediates<UTYPE>::binimm_immediates) {
                std::ostringstream name;
                name << "cmp_imm_" << op.func << "_" << type.str() << "_" << imm.value;

                out << "// CHECK-LABEL: @" << name.str() << std::endl;

                // CHECK: br_op_imm_i64.l %r1, imm, LABEL
                out << "// CHECK: cmp_" << op.insn_func << "_imm_" << op.asm_type << size;
                if (imm.mode == LONG) {
                    out << ".l";
                }
                // FIXME: llvm uses signed only immediates?
                out << " %r1, %r1, " << imm.value << std::endl;

                // void br_ge_zero_i64(i64 a) { CHECK_BRANCH(a >= 0); }
                out << "bool " << name.str()  << "(" << type.str() << " value) {"<< std::endl;
                const u64 cpp_imm = (op.mode == PLUS_1) ? (imm.value - 1) : imm.value;
                out << "    return (value " << op.op << " " << cpp_imm << ");"<< std::endl;
                out << "}"<< std::endl;
                out << std::endl;
            }
        }
    }
}

CODEGEN_HANDLER(CompareImm32)  { CompareRegImm<i32, u32>(out, 32); }
CODEGEN_HANDLER(CompareImm64)  { CompareRegImm<i64, u64>(out, 64); }
CODEGEN_HANDLER(CompareImm128) { CompareRegImm<i64, u64>(out, 128); }

} // namespace llvm
} // namespace postrisc
