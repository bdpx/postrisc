#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(DividePow2)
{
    for (const auto& size : {32, 64}) {
        {
        std::ostringstream name;
        name << "divide_pow2_i" << size;

        out << "// CHECK-LABEL: @" << name.str() << std::endl;
        out << "i" << size << " " << name.str() << "(i" << size << " value, i32 shift)" << std::endl;
        out << "{" << std::endl;
        out << "// CHECK: srd_i" << size << " %r1, %r1, %r2" << std::endl;
        out << "// CHECK-NEXT: retf 0" << std::endl;
        out << "    return value / ((i" << size << ")1 << shift);" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
        }
        for (int i=1; i<size-1; i++) {
             const i64 imm = (1LL << i);
             std::ostringstream name;
             name << "divide_pow2_i" << size << "_" << imm;
             out << "// CHECK-LABEL: @" << name.str() << std::endl;
             out << "i" << size << " " << name.str() << "(i" << size << " value, i32 shift)" << std::endl;
             out << "{" << std::endl;
             out << "// CHECK: srd_imm_i" << size << " %r1, %r1, " << i << std::endl;
             out << "// CHECK-NEXT: retf 0" << std::endl;
             out << "    return value / " << imm << ";" << std::endl;
             out << "}" << std::endl;
             out << std::endl;
        }
    }
}

} // namespace llvm
} // namespace postrisc
