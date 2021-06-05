#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(IntRegReg)
{
    static const struct {
         char asm_name[16];
         char c_fragment[16];
         SwapMode swap_mode;
    } fragments[] = {
        { "add", "a + b", SWAP },
        { "sub", "a - b", NONE },
        { "mul", "a * b", SWAP },
        { "div", "a / b", NONE },
        { "rem", "a % b", NONE },
    };

    for (const int size : {32, 64}) {
        std::ostringstream type;
        type << 'i' << size;

        for (const auto& frag : fragments) {
            std::ostringstream name;
            name << frag.asm_name << "_" << type.str();

            out << CHECK_LABEL << name.str() << std::endl;

            // CHECK: op_i32 %r1, %r1, %r2
            out << CHECK << frag.asm_name << "_" << type.str() << " %r1, ";
            if (frag.swap_mode == SWAP) {
               out << "%r2, %r1" << std::endl;
            } else {
               out << "%r1, %r2" << std::endl;
            }

            // void br_ge_zero_i64(i64 a) { CHECK_BRANCH(a >= 0); }
            out << type.str() << " " << name.str()  << "(" << type.str() << " a," << type.str() << " b) {"<< std::endl;
            out << "    return " << frag.c_fragment << ";"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }

        out << std::endl;
    }
}

// for all i32,i64,i128 same bitwise instructions are used

template<typename ITYPE, typename UTYPE>
void Generator::IntRegImm(std::ostream& out, int size)
{
    static const struct {
         char asm_name[16];
         char c_fragment[16];
    } fragments[] = {
        { "add_imm",  "value + imm" },
        { "subr_imm", "imm - value" },
        { "mul_imm",  "value * imm" },
        { "div_imm",  "value / imm" },
        { "rem_imm",  "value % imm" },
    };

    for (const auto& frag : fragments) {
        std::ostringstream type;
        type << 'i' << size;
        for (const auto& imm : immediates<ITYPE>::binimm_immediates) {
            std::ostringstream name;
            name << frag.asm_name << "_" << type.str() << "_";
            if (imm.value < 0) {
                name << "minus_" << -(i64)imm.value;
            } else {
                name << "plus_" << imm.value;
            }

            out << CHECK_LABEL << name.str() << std::endl;

            //const int population = util::population::func(static_cast<UTYPE>(imm.value));
            // FIXME: op_imm_i64.l %r1, %r1, imm
            out << "// FIXMECHECK: " << frag.asm_name << "_" << type.str();
            if (imm.mode == LONG) {
                out << ".l";
            }
            out << " %r1, %r1, " << imm.value << std::endl;

            // i64 OP_imm_i64(i64 a) { return value OP imm; }
            out << type.str() << " " << name.str()  << "(" << type.str() << " value) {"<< std::endl;
            out << "    const " << type.str() << " imm = " << imm.value << ";"<< std::endl;
            out << "    return " << frag.c_fragment << ";"<< std::endl;
            out << "}"<< std::endl;
            out << std::endl;
        }

        out << std::endl;
    }
}

CODEGEN_HANDLER(IntRegImm32)  { IntRegImm<i32, u32>(out, 32); }
CODEGEN_HANDLER(IntRegImm64)  { IntRegImm<i64, u64>(out, 64); }
CODEGEN_HANDLER(IntRegImm128) { IntRegImm<i64, u64>(out, 128); }

} // namespace llvm
} // namespace postrisc
