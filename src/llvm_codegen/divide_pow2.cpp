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
        { insn_divp2w, insn_divp2iw, 32 },
        { insn_divp2d, insn_divp2id, 64 },
        { insn_divp2q, insn_divp2iq, 128 },
    };

    for (const auto& test : tests) {

        for (const int shift_size : {32, 64}) {
            std::ostringstream name;
            name << "divide_pow2_i" << test.size << "_i" << shift_size;

            out << CHECK_LABEL << name.str() << '\n';
            out << CHECK << InsnCounter(this, test.insn) << " %r1, %r1, %r2\n";
            out << CHECK_NEXT << insn_retf << " 0\n";

            out << "i" << test.size << " " << name.str() << "(i" << test.size << " value, i" << shift_size << " shift)\n";
            out << "{\n";
            out << "    return value / ((i" << test.size << ")1 << shift);\n";
            out << "}\n\n";
        }

        for (int i = 1; i < std::min(63, test.size - 1); i++) {
             const i64 imm = (1LL << i);
             std::ostringstream name;
             name << "divide_pow2_i" << test.size << "_2x" << i;

             out << CHECK_LABEL << name.str() << '\n';
             out << CHECK << InsnCounter(this, test.insn_imm) << " %r1, %r1, " << i << '\n';
             out << CHECK_NEXT << insn_retf << " 0\n";

             out << "i" << test.size << " " << name.str() << "(i" << test.size << " value)\n";
             out << "{\n";
             out << "    return value / " << imm << ";\n";
             out << "}\n\n";
        }
    }
}

CODEGEN_HANDLER(Mov2)
{

  out << "extern i32 test_mov2_callback(i32 a, i32 b, i32 c, i32 d);\n";

  out << CHECK_LABEL << "test_mov2\n";
  out << "int test_mov2(i32 a, i32 b, i32 c, i32 d)\n";
  out << "{\n";
  out << CHECK << InsnCounter(this, insn_alloc) << " 11\n";
  out << "  i32 ret = a;\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r7, %r8, %r3, %r4\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r9, %r10, %r2, %r1\n";
  out << CHECK << InsnCounter(this, insn_call) << " %r6, %jpc(test_mov2_callback)\n";
  out << CHECK << InsnCounter(this, insn_addws) << " %r5, %r7, %r1\n";
  out << "  ret += test_mov2_callback(c, d, b, a);\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r7, %r8, %r3, %r4\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r9, %r10, %r2, %gz\n";
  out << CHECK << InsnCounter(this, insn_call) << " %r6, %jpc(test_mov2_callback)\n";
  out << CHECK << InsnCounter(this, insn_addws) << " %r2, %r5, %r7\n";
  out << "  ret += test_mov2_callback(c, d, b, 0);\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r7, %r8, %r3, %r4\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r9, %r10, %gz, %gz\n";
  out << CHECK << InsnCounter(this, insn_call) << " %r6, %jpc(test_mov2_callback)\n";
  out << CHECK << InsnCounter(this, insn_addws) << " %r2, %r2, %r7\n";
  out << "  ret += test_mov2_callback(c, d, 0, 0);\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r7, %r8, %r3, %gz\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r9, %r10, %gz, %gz\n";
  out << CHECK << InsnCounter(this, insn_call) << " %r6, %jpc(test_mov2_callback)\n";
  out << CHECK << InsnCounter(this, insn_addws) << " %r2, %r2, %r7\n";
  out << "  ret += test_mov2_callback(c, 0, 0, 0);\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r7, %r8, %gz, %gz\n";
  out << CHECK << InsnCounter(this, insn_mov2) << " %r9, %r10, %gz, %gz\n";
  out << CHECK << InsnCounter(this, insn_call) << " %r6, %jpc(test_mov2_callback)\n";
  out << "  ret += test_mov2_callback(0, 0, 0, 0);\n";
  out << CHECK  << InsnCounter(this, insn_addws) << " %r1, %r2, %r7\n";
  out << CHECK << InsnCounter(this, insn_retf) << " 0\n";
  out << "  return ret;\n";
  out << "}\n";

}

} // namespace llvm
} // namespace postrisc
