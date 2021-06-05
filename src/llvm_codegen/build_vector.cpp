#include "util/common.hpp"
#include "arch/isa.hpp"
#include "disassembler/Decoder.hpp"

#include "Generator.hpp"

namespace postrisc {
namespace llvm {

CODEGEN_HANDLER(BuildVector)
{
    static const struct {
        ValueType vector_type;
        ValueType scalar_type;
    } tests_8[] = {
        { VUB, UB },
        { VIB, IB },
    };

    static const struct {
        ValueType vector_type;
        ValueType scalar_type;
    } tests_16[] = {
        { VUH, UH },
        { VIH, IH },
        { VFH, FH },
    };

    static const struct {
        ValueType vector_type;
        ValueType scalar_type;
    } tests_32[] = {
        { VUW, UW },
        { VIW, IW },
        { VFS, FS },
    };

    static const struct {
        ValueType vector_type;
        ValueType scalar_type;
    } tests_64[] = {
        { VUD, UD },
        { VID, ID },
        { VFD, FD },
    };

    // vector from scalar
    for (const auto& test : tests_64) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out <<  test.vector_type << " " << fname.str() << "(" <<  test.scalar_type << " v1)\n";
        out << "{\n";
        out << "    return (" <<  test.vector_type << "){v1, v1};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_32) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK      << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "(" << test.scalar_type << " v1)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v1, v1, v1};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_16) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 32, 32\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK      << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "(" << test.scalar_type << " v1)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v1, v1, v1, v1, v1, v1, v1};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_8) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 2\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 16, 16\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "(" << test.scalar_type << " v1)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v1, v1, v1, v1, v1, v1, v1, v1, v1, v1, v1, v1, v1, v1, v1};\n";
        out << "}\n\n";
    }

    // vector from (r1,r2) repeating pattern
    for (const auto& test : tests_64) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r2_" <<  test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out <<  test.vector_type << " " << fname.str() << "(" << test.scalar_type << " v1, " << test.scalar_type << " v2)\n";
        out << "{\n";
        out << "    return (" <<  test.vector_type << "){v1, v2};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_32) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r2_" <<  test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK      << insn_retf << " 0\n";

        out <<  test.vector_type << " " << fname.str() << "(" << test.scalar_type << " v1, " << test.scalar_type << " v2)\n";
        out << "{\n";
        out << "    return (" <<  test.vector_type << "){v1, v2, v1, v2};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_16) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r2_" <<  test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 32, 32\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out <<  test.vector_type << " " << fname.str() << "(" << test.scalar_type << " v1, " << test.scalar_type << " v2)\n";
        out << "{\n";
        out << "    return (" <<  test.vector_type << "){v1, v2, v1, v2, v1, v2, v1, v2};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_8) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r2_" <<  test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 3\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 16, 16\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out <<  test.vector_type << " " << fname.str() << "(" << test.scalar_type << " v1, " << test.scalar_type << " v2)\n";
        out << "{\n";
        out << "    return (" <<  test.vector_type << "){v1, v2, v1, v2, v1, v2, v1, v2, v1, v2, v1, v2, v1, v2, v1, v2};\n";
        out << "}\n\n";
    }

    // vector from (r1-r4) repeating pattern
    for (const auto& test : tests_32) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r4_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 5\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r3, %r3, %r4, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 32, 32\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r3, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str()
            << "(" << test.scalar_type << " v1, "
                   << test.scalar_type << " v2, "
                   << test.scalar_type << " v3, "
                   << test.scalar_type << " v4)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v2, v3, v4};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_16) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r4_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 5\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r3, %r3, %r4, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 16, 16\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r3, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "("
            << test.scalar_type << " v1, "
            << test.scalar_type << " v2, "
            << test.scalar_type << " v3, "
            << test.scalar_type << " v4)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v2, v3, v4, v1, v2, v3, v4};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_8) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r4_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 5\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r3, %r3, %r4, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 8, 8\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r3, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK      << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "("
            << test.scalar_type << " v1, "
            << test.scalar_type << " v2, "
            << test.scalar_type << " v3, "
            << test.scalar_type << " v4)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v2, v3, v4, v1, v2, v3, v4, v1, v2, v3, v4, v1, v2, v3, v4};\n";
        out << "}\n\n";
    }

    // vector from (r1-r8) repeating pattern
    for (const auto& test : tests_16) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r8_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 9\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r7, %r7, %r8, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r5, %r5, %r6, 16, 16\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r5, %r5, %r7, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r3, %r3, %r4, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 16, 16\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r3, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r5, 64, 64\n";
        out << CHECK_NEXT << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "("
            << test.scalar_type << " v1, "
            << test.scalar_type << " v2, "
            << test.scalar_type << " v3, "
            << test.scalar_type << " v4, "
            << test.scalar_type << " v5, "
            << test.scalar_type << " v6, "
            << test.scalar_type << " v7, "
            << test.scalar_type << " v8)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v2, v3, v4, v5, v6, v7, v8};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_8) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r8_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 9\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r7, %r7, %r8, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r5, %r5, %r6, 8, 8\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r5, %r5, %r7, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r3, %r3, %r4, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 8, 8\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r3, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r5, 32, 32\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r1, 64, 64\n";
        out << CHECK      << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "("
            << test.scalar_type << " v1, "
            << test.scalar_type << " v2, "
            << test.scalar_type << " v3, "
            << test.scalar_type << " v4, "
            << test.scalar_type << " v5, "
            << test.scalar_type << " v6, "
            << test.scalar_type << " v7, "
            << test.scalar_type << " v8)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v2, v3, v4, v5, v6, v7, v8, v1, v2, v3, v4, v5, v6, v7, v8};\n";
        out << "}\n\n";
    }

    for (const auto& test : tests_8) {
        std::ostringstream fname;
        fname << "test_build_vector_r1_r16_" << test.vector_type << "_" << test.scalar_type;
        out << CHECK_LABEL << fname.str() << '\n';
        out << CHECK      << insn_alloc << " 17\n";

        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r15, %r15, %r16, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r13, %r13, %r14, 8, 8\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r13, %r13, %r15, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r11, %r11, %r12, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r9, %r9, %r10, 8, 8\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r9, %r9, %r11, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r9, %r9, %r13, 32, 32\n";

        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r7, %r7, %r8, 8, 8\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r5, %r5, %r6, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r5, %r5, %r7, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r3, %r3, %r4, 8, 8\n";
        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r2, 8, 8\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r3, 16, 16\n";
        out << CHECK_NEXT << InsnCounter(this, insn_deposit) << " %r1, %r1, %r5, 32, 32\n";

        out << CHECK      << InsnCounter(this, insn_deposit) << " %r1, %r1, %r9, 64, 64\n";
        out << CHECK      << insn_retf << " 0\n";

        out << test.vector_type << " " << fname.str() << "("
            << test.scalar_type << " v1, "
            << test.scalar_type << " v2, "
            << test.scalar_type << " v3, "
            << test.scalar_type << " v4, "
            << test.scalar_type << " v5, "
            << test.scalar_type << " v6, "
            << test.scalar_type << " v7, "
            << test.scalar_type << " v8, "
            << test.scalar_type << " v9, "
            << test.scalar_type << " v10, "
            << test.scalar_type << " v11, "
            << test.scalar_type << " v12, "
            << test.scalar_type << " v13, "
            << test.scalar_type << " v14, "
            << test.scalar_type << " v15, "
            << test.scalar_type << " v16)\n";
        out << "{\n";
        out << "    return (" << test.vector_type << "){v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16};\n";
        out << "}\n\n";
    }
}

} // namespace llvm
} // namespace postrisc
