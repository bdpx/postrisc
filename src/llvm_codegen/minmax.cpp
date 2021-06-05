#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(MinMax)
{
    static const struct {
        char type;
        char left[4];
        char op[4];
        char right[4];
        char first[4];
        char second[4];
        char func[4];
        char insn_func[4];
        SwapMode mode;
    } ops[] = {
        { 'i', "a", "<",  "b", "a", "b", "lt", "min", NONE },
        { 'i', "a", ">=", "b", "a", "b", "ge", "max", NONE },
        { 'i', "a", "<=", "b", "a", "b", "le", "min", NONE },
        { 'i', "a", ">",  "b", "a", "b", "gt", "max", NONE },
        { 'u', "a", "<",  "b", "a", "b", "lt", "min", NONE },
        { 'u', "a", ">=", "b", "a", "b", "ge", "max", NONE },
        { 'u', "a", "<=", "b", "a", "b", "le", "min", NONE },
        { 'u', "a", ">",  "b", "a", "b", "gt", "max", NONE },
        // swap left/right
        { 'i', "b", "<",  "a", "a", "b", "lt", "max", SWAP },
        { 'i', "b", ">=", "a", "a", "b", "ge", "min", SWAP },
        { 'i', "b", "<=", "a", "a", "b", "le", "max", SWAP },
        { 'i', "b", ">",  "a", "a", "b", "gt", "min", SWAP },
        { 'u', "b", "<",  "a", "a", "b", "lt", "max", SWAP },
        { 'u', "b", ">=", "a", "a", "b", "ge", "min", SWAP },
        { 'u', "b", "<=", "a", "a", "b", "le", "max", SWAP },
        { 'u', "b", ">",  "a", "a", "b", "gt", "min", SWAP },
        // swap first/second
        { 'i', "a", "<",  "b", "b", "a", "lt", "max", NONE },
        { 'i', "a", ">=", "b", "b", "a", "ge", "min", NONE },
        { 'i', "a", "<=", "b", "b", "a", "le", "max", NONE },
        { 'i', "a", ">",  "b", "b", "a", "gt", "min", NONE },
        { 'u', "a", "<",  "b", "b", "a", "lt", "max", NONE },
        { 'u', "a", ">=", "b", "b", "a", "ge", "min", NONE },
        { 'u', "a", "<=", "b", "b", "a", "le", "max", NONE },
        { 'u', "a", ">",  "b", "b", "a", "gt", "min", NONE },
        // swap left/right, first/second
        { 'i', "b", "<",  "a", "b", "a", "lt", "min", SWAP },
        { 'i', "b", ">=", "a", "b", "a", "ge", "max", SWAP },
        { 'i', "b", "<=", "a", "b", "a", "le", "min", SWAP },
        { 'i', "b", ">",  "a", "b", "a", "gt", "max", SWAP },
        { 'u', "b", "<",  "a", "b", "a", "lt", "min", SWAP },
        { 'u', "b", ">=", "a", "b", "a", "ge", "max", SWAP },
        { 'u', "b", "<=", "a", "b", "a", "le", "min", SWAP },
        { 'u', "b", ">",  "a", "b", "a", "gt", "max", SWAP },
    };

    for (const int size : {32, 64, 128}) {
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << "// compare reg-reg min/max " << size << " instructions" << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << std::endl;

        for (const auto& op : ops) {
            std::ostringstream type;
            type << op.type << size;

            std::ostringstream name;
            name << op.insn_func << "_rr_" << op.left << "_" << op.func << "_" << op.right << "_" << op.first << "_" << op.second << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: min_i32 %r1, %r1, %r2
            out << "// CHECK: " << op.insn_func << "_" << type.str() << " %r1, ";
            if (op.mode == SWAP) {
                out << "%r2, %r1";
            } else {
                out << "%r1, %r2";
            }
            out << std::endl;

            // i32 mim[max]_a_ge_b_a_b_i32(i32 a, i32 b) { return (a >= b) ? a : b; }
            out << type.str() << " " << name.str()  << "(" << type.str() << " a, " << type.str() << " b) {"<< std::endl;
            out << "    return (" << op.left << " " << op.op << " " << op.right << ") ? " << op.first << " : " << op.second << ";" << std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }
    }
}

struct imm_op {
    char type;
    char op[4];
    char func[4];
    char insn_func[4];
    char insn_func_alt[4];
};

template<typename T>
static void MinMaxImm(std::ostream& out, const std::string& type, const imm_op& op, const imm_data<T>& imm, bool cmp_reg_first, bool choose_reg_first)
{
    // min[max]_reg[imm]_OP_imm[reg]_minus_1000
    std::ostringstream name;
    name << op.insn_func << op.insn_func_alt;
    if (cmp_reg_first) {
        name << "_reg_" << op.func << "_imm";
    } else {
        name << "_imm_" << op.func << "_reg";
    }
    if (choose_reg_first) {
        name << "_then_reg_else_imm_";
    } else {
        name << "_then_imm_else_reg_";
    }
    name << type;
    if constexpr (std::is_signed_v<T>) {
        if (imm.value < 0) {
            name << "_m" << -(i64)imm.value;
        } else {
            name << "_p" << imm.value;
        }
    } else {
        name << "_" << imm.value;
    }

    out << "// CHECK-LABEL: @" << name.str() << std::endl;

    if constexpr (std::is_unsigned_v<T>) {
        // FIXME: llvm issue for min/max imm?
        // minmax_imm_le_u64_9223372036854775807 - can't emit - min_imm_u64.l %r1, %r1, 9223372036854775807
        if (imm.value == 0x7FFFFFFFFFFFFFFF && name.str() == "maxmin_imm_ge_reg_then_reg_else_imm_u64_9223372036854775807") {
            out << "// CHECK: ld_imm.l %r2, 9223372036854775807" << std::endl;
            out << "// CHECK: cmov_lt_i64 %r1, %r1, %r2, %r1" << std::endl;
        }
    } else {
        // NORMAL:
        // CHECK: min[max]_imm_i64.l %r1, %r1, imm
        out << "// CHECK: ";
        if (cmp_reg_first) {
            if (choose_reg_first) {
                out << op.insn_func;
            } else {
                out << op.insn_func_alt;
            }
        } else {
            if (choose_reg_first) {
                out << op.insn_func_alt;
            } else {
                out << op.insn_func;
            }
        }
        out << "_imm_" << type;
        if (imm.mode == LONG) {
            out << ".l";
        }
        out << " %r1, %r1, " << imm.value << std::endl;

        if (imm.mode == LONG) {
            out << "// CHECK-EMPTY:" << std::endl;
            out << "// CHECK-NEXT: ! BUNDLE" << std::endl;
            out << "// CHECK-NEXT: retf 0" << std::endl;
        } else {
            out << "// CHECK-NEXT: retf 0" << std::endl;
        }
    }

    // i64 min[max]_ge_imm_i64(i64 a) { CHECK_BRANCH(a >= 0); }
    out << type << " " << name.str()  << "(" << type << " value) {"<< std::endl;
    out << "    return (";
    if (cmp_reg_first) {
        out << "value " << op.op << " " << imm.value;
    } else {
        out << imm.value << " " << op.op << " value";
    }
    out << ") ? ";
    if (choose_reg_first) {
        out << "value : " << imm.value << ";"<< std::endl;
    } else {
        out << imm.value << " : value" << ";"<< std::endl;
    }
    out << "}"<< std::endl;
    out << std::endl;
}

template<typename ITYPE, typename UTYPE> static void MinMaxRegImm(std::ostream& out, int size)
{
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << "// min/max register and immediate" << std::endl;
    out << "//===----------------------------------------------------------------------===//" << std::endl;
    out << std::endl;

    static const imm_op imm_ops[] = {
        { 'i', "<",  "lt", "min", "max" },
        { 'i', ">=", "ge", "max", "min" },
        { 'i', "<=", "le", "min", "max" },
        { 'i', ">",  "gt", "max", "min" },
        { 'u', "<",  "lt", "min", "max" },
        { 'u', ">=", "ge", "max", "min" },
        { 'u', "<=", "le", "min", "max" },
        { 'u', ">",  "gt", "max", "min" },
    };

    for (const auto& op : imm_ops) {
        std::ostringstream type;
        type << op.type << size;

        if (op.type == 'i') {
            for (const auto& imm : immediates<ITYPE>::binimm_immediates) {
                MinMaxImm<ITYPE>(out, type.str(), op, imm, true,  true);
                MinMaxImm<ITYPE>(out, type.str(), op, imm, true,  false);
                MinMaxImm<ITYPE>(out, type.str(), op, imm, false, true);
                MinMaxImm<ITYPE>(out, type.str(), op, imm, false, false);
            }
        } else {
            for (const auto& imm : immediates<UTYPE>::binimm_immediates) {
                MinMaxImm<UTYPE>(out, type.str(), op, imm, true,  true);
                MinMaxImm<UTYPE>(out, type.str(), op, imm, true,  false);
                MinMaxImm<UTYPE>(out, type.str(), op, imm, false, true);
                MinMaxImm<UTYPE>(out, type.str(), op, imm, false, false);
            }
        }
    }
}

CODEGEN_HANDLER(MinMaxImm32)  { MinMaxRegImm<i32, u32>(out, 32); }
CODEGEN_HANDLER(MinMaxImm64)  { MinMaxRegImm<i64, u64>(out, 64); }

static void FpMinMaxBuiltin(std::ostream& out, const char *builtinName, const char *insnName, const std::string& type)
{
    std::ostringstream name;
    name << "fp_" << builtinName << "_" << type;

    out << "// CHECK-LABEL: @" << name.str() << std::endl;

    // CHECK: min[max]num_i32 %r1, %r1, %r2
    out << "// CHECK: " << insnName << "_" << type << " %r1, %r1, %r2" << std::endl;

    out << type << " " << name.str()  << "(" << type << " a, " << type << " b) {"<< std::endl;
    out << "    return " << builtinName << "(a, b);" << std::endl;
    out << "}"<< std::endl;
    out << std::endl;
}

CODEGEN_HANDLER(FpMinMax)
{
    static const struct {
        char left[4];
        char op[4];
        char right[4];
        char first[4];
        char second[4];
        char func[4];
        char insn_func[4];
        SwapMode mode;
    } ops[] = {
        { "a", "<",  "b", "a", "b", "lt", "min", NONE },
        { "a", ">=", "b", "a", "b", "ge", "max", NONE },
        { "a", "<=", "b", "a", "b", "le", "min", NONE },
        { "a", ">",  "b", "a", "b", "gt", "max", NONE },
        // swap left/right
        { "b", "<",  "a", "a", "b", "lt", "max", SWAP },
        { "b", ">=", "a", "a", "b", "ge", "min", SWAP },
        { "b", "<=", "a", "a", "b", "le", "max", SWAP },
        { "b", ">",  "a", "a", "b", "gt", "min", SWAP },
        // swap first/second
        { "a", "<",  "b", "b", "a", "lt", "max", NONE },
        { "a", ">=", "b", "b", "a", "ge", "min", NONE },
        { "a", "<=", "b", "b", "a", "le", "max", NONE },
        { "a", ">",  "b", "b", "a", "gt", "min", NONE },
        // swap left/right, first/second
        { "b", "<",  "a", "b", "a", "lt", "min", SWAP },
        { "b", ">=", "a", "b", "a", "ge", "max", SWAP },
        { "b", "<=", "a", "b", "a", "le", "min", SWAP },
        { "b", ">",  "a", "b", "a", "gt", "max", SWAP },
    };

    static const struct {
        int size;
        char minname[32];
        char maxname[32];
    } types[] = {
        { 16, "__builtin_postrisc_minnum_f16", "__builtin_postrisc_maxnum_f16"},
        { 32,  "__builtin_fminf", "__builtin_fmaxf" },
        { 64,  "__builtin_fmin",  "__builtin_fmax" },
        { 128, "__builtin_fminl", "__builtin_fmaxl" },
    };

    for (const auto& t : types) {
        std::ostringstream type;
        type << 'f' << t.size;

        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << "// fp compare reg-reg min/max " << type.str() << " instructions" << std::endl;
        out << "//===----------------------------------------------------------------------===//" << std::endl;
        out << std::endl;

        for (const auto& op : ops) {
            std::ostringstream name;
            name << "fp_" << op.insn_func << "num_" << op.left << "_" << op.func << "_" << op.right << "_" << op.first << "_" << op.second << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: min_i32 %r1, %r1, %r2
            out << "// CHECK: " << op.insn_func << "num_" << type.str() << " %r1, ";
            if (op.mode == SWAP) {
                out << "%r2, %r1";
            } else {
                out << "%r1, %r2";
            }
            out << std::endl;

            // i32 mim[max]_a_ge_b_a_b_i32(i32 a, i32 b) { return (a >= b) ? a : b; }
            out << type.str() << " " << name.str()  << "(" << type.str() << " a, " << type.str() << " b) {"<< std::endl;
            out << "    return (" << op.left << " " << op.op << " " << op.right << ") ? " << op.first << " : " << op.second << ";" << std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }

        FpMinMaxBuiltin(out, t.minname, "minnum", type.str());
        FpMinMaxBuiltin(out, t.maxname, "maxnum", type.str());
    }
}

} // namespace llvm
} // namespace postrisc
