#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(DividePow2)
{
    static const struct {
        EInstructionID insn;
        EInstructionID insn_imm;
        int size;
    } tests[] = {
        { insn_divp2_i32,  insn_divp2_imm_i32,   32 },
        { insn_divp2_i64,  insn_divp2_imm_i64,   64 },
        { insn_divp2_i128, insn_divp2_imm_i128, 128 },
    };

    for (const auto& test : tests) {

        for (const int shift_size : {32, 64}) {
            std::ostringstream name;
            name << "divide_pow2_i" << test.size << "_i" << shift_size;

            out << CHECK_LABEL << name.str() << std::endl;
            out << CHECK << InsnCounter(this, test.insn) << " %r1, %r1, %r2" << std::endl;
            out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

            out << "i" << test.size << " " << name.str() << "(i" << test.size << " value, i" << shift_size << " shift)" << std::endl;
            out << "{" << std::endl;
            out << "    return value / ((i" << test.size << ")1 << shift);" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }

        for (int i = 1; i < std::min(63, test.size - 1); i++) {
             const i64 imm = (1LL << i);
             std::ostringstream name;
             name << "divide_pow2_i" << test.size << "_2x" << i;

             out << CHECK_LABEL << name.str() << std::endl;
             out << CHECK << InsnCounter(this, test.insn_imm) << " %r1, %r1, " << i << std::endl;
             out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;

             out << "i" << test.size << " " << name.str() << "(i" << test.size << " value)" << std::endl;
             out << "{" << std::endl;
             out << "    return value / " << imm << ";" << std::endl;
             out << "}" << std::endl;
             out << std::endl;
        }
    }
}

} // namespace llvm
} // namespace postrisc
