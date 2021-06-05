#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

// Those bitcasts are free:
// f32 <=> i32, f64 <=> i64, f128 <=> i128
// expected result: input is output, return immediately
// vector <=> vector, vector <=> i128, vector <=> f128, vector => scalar(?)

CODEGEN_HANDLER(BitCast)
{
    static const struct {
        char type1[8];
        char type2[8];
    } bitcasts[] = {
        { "f32",  "u32" },
        { "f32",  "i32" },
        { "f64",  "u64" },
        { "f64",  "i64" },
        { "u32",  "f32" },
        { "i32",  "f32" },
        { "u64",  "f64" },
        { "i64",  "f64" },
    };

    static const struct {
        char type[8];
    } vectors[] = {
        { "i128" },
        { "u128" },
        { "f128" },
        { "v16i8" },
        { "v16u8" },
        { "v8i16" },
        { "v8u16" },
        { "v4i32" },
        { "v4u32" },
        { "v2i64" },
        { "v2u64" },
    };

    for (const auto& bc : bitcasts) {
        std::ostringstream name;
        name << "bitcast_" << bc.type1 << "_" << bc.type2;

        out << CHECK_LABEL << name.str() << std::endl;

        out << bc.type2 << " " << name.str() << "(" << bc.type1 << " value)" << std::endl;
        out << "{" << std::endl;
        out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
        out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
        out << "    return BITCAST(" << bc.type2 << ")value;" << std::endl;
        out << "}" << std::endl;
        out << std::endl;
    }

    for (size_t i = 0; i < sizearray(vectors); i++) {
        for (size_t j = 0; j < sizearray(vectors); j++) {
            if (i == j) continue;
            std::ostringstream name;
            name << "bitcast_" << vectors[i].type << "_" << vectors[j].type;

            out << CHECK_LABEL << name.str() << std::endl;

            out << vectors[i].type << " " << name.str() << "(" << vectors[j].type << " value)" << std::endl;
            out << "{" << std::endl;
            out << CHECK << insn_name(insn_alloc) << " 2" << std::endl;
            out << CHECK_NEXT << insn_name(insn_retf) << " 0" << std::endl;
            out << "    return BITCAST(" << vectors[i].type << ")value;" << std::endl;
            out << "}" << std::endl;
            out << std::endl;
        }
    }
}

} // namespace llvm
} // namespace postrisc
