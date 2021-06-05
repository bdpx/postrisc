#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

// for all i32,i64,i128 same bitwise instructions are used

static void unary(std::ostream& out, const std::string& type, const std::string& op, const std::string& c_op)
{
    std::ostringstream name;
    name << "unary_" << op << "_" << type;

    out << "// CHECK-LABEL: @" << name.str() << std::endl;
    out << "// CHECK: " << op << " %r1, %r1" << std::endl;
    out << "// CHECK-NEXT: retf 0" << std::endl;
    out << type << " " << name.str() << "(" << type << " a) { return " << c_op << "a; }" << std::endl;
}

CODEGEN_HANDLER(BitwiseRegReg)
{
    for (const auto size : {32, 64, 128}) {
        std::ostringstream type;
        type << "u" << size;
        unary(out, type.str(), "not", "~");
        out << std::endl;
    }

    static const struct {
         char asm_name[16];
         char c_fragment[16];
         SwapMode swap_mode;
    } fragments[] = {
        { "and",  "a & b",    SWAP },
        { "or",   "a | b",    SWAP },
        { "xor",  "a ^ b",    SWAP },
        { "nand", "~(a & b)", SWAP },
        { "nor",  "~(a | b)", SWAP },
        { "andn", "(~a) & b", NONE },
        { "orn",  "(~a) | b", NONE },
    };

    for (const int size : {32, 64, 128}) {
        std::ostringstream type;
        type << 'i' << size;

        for (const auto& frag : fragments) {
            std::ostringstream name;
            name << frag.asm_name << "_" << type.str();

            out << "// CHECK-LABEL: @" << name.str() << std::endl;

            // CHECK: op_i32 %r1, %r1, %r2
            out << "// CHECK: " << frag.asm_name << " %r1, ";
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

template<typename ITYPE, typename UTYPE> static void BitwiseRegImm(std::ostream& out, int size)
{
    static const struct {
         char asm_name[16];
         char c_fragment[16];
    } fragments[] = {
        { "and_imm",  "value & " },
        { "or_imm",   "value | " },
        { "xor_imm",  "value ^ " },
        { "andn_imm", "(~value) & " },
        { "orn_imm",  "(~value) | " },
    };

    for (const char t : {'i', 'u'}) {
       std::ostringstream type;
       type << t << size;

        for (const auto& frag : fragments) {
            for (const auto& imm : immediates<ITYPE>::binimm_immediates) {
                std::ostringstream name;
                name << frag.asm_name << "_" << type.str() << "_";
                if (imm.value < 0) {
                    name << "minus_" << -(i64)imm.value;
                } else {
                    name << "plus_" << imm.value;
                }

                out << "// CHECK-LABEL: @" << name.str() << std::endl;

                const int population = util::population::func(static_cast<UTYPE>(imm.value));
                if (population == 1) {
                    // bit_set|flip
                } else if (population == sizeof(ITYPE)*8-1) {
                    // bit_clear|flip
                } else {
                    // CHECK: op_imm.l %r1, imm, LABEL
                    out << "// CHECK: " << frag.asm_name;
                    if (imm.mode == LONG) {
                        out << ".l";
                    }
                    out << " %r1, %r1, " << imm.value << std::endl;
                }

                // void br_ge_zero_i64(i64 a) { CHECK_BRANCH(a >= 0); }
                out << type.str() << " " << name.str()  << "(" << type.str() << " value) {"<< std::endl;
                out << "    return " << frag.c_fragment << imm.value << ";"<< std::endl;
                out << "}"<< std::endl;
                out << std::endl;
            }

            out << std::endl;
        }
    }
}

CODEGEN_HANDLER(BitwiseRegImm32)  { BitwiseRegImm<i32, u32>(out, 32); }
CODEGEN_HANDLER(BitwiseRegImm64)  { BitwiseRegImm<i64, u64>(out, 64); }
CODEGEN_HANDLER(BitwiseRegImm128) { BitwiseRegImm<i64, u64>(out, 128); }

} // namespace llvm
} // namespace postrisc
