#include "util/common.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "emulator/interruption.hpp"
#include "emulator/core.hpp"
#include "emulator_system/target_system.hpp"
#include "assembler/assembler.hpp"
#include "emulator_test_fixture.hpp"

namespace postrisc {

////////////////////////////////////////////////////////////////////////////
// test suite begin
////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE(instructions, emulator_test_fixture)

BOOST_AUTO_TEST_CASE(insn_add)
{
    static const TestData_binop test_cases[] = {
        { CGeneralRegister(0, 1), CGeneralRegister(0, 2), CGeneralRegister(0, 3) },
        { CGeneralRegister(0,-1), CGeneralRegister(0,-2), CGeneralRegister(0,-3) },
        { CGeneralRegister(0,-3), CGeneralRegister(0, 2), CGeneralRegister(0,-1) },
    };

    for (const auto & test : test_cases) {
         test_binop("add", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_addi)
{
    static const TestData_binimm test_cases[] = {
        { CGeneralRegister(0, 1),  2, CGeneralRegister(0, 3) },
        { CGeneralRegister(0,-1), -2, CGeneralRegister(0,-3) },
        { CGeneralRegister(0,-3),  2, CGeneralRegister(0,-1) },
    };

    for (const auto & test : test_cases) {
         test_binimm("addi", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_alloc)
{
    test_alloc();
}

BOOST_AUTO_TEST_CASE(insn_and)
{
    static const TestData_binop test_cases[] = {
        { CGeneralRegister(U64C(0xffffffffffffffff), U64C(0xffffffffffffffff)),
          CGeneralRegister(U64C(0x7fffffffffffffff), U64C(0xffffffffffffffff)),
          CGeneralRegister(U64C(0x7fffffffffffffff), U64C(0xffffffffffffffff))
        },
        { CGeneralRegister(U64C(0x3333333333333333), U64C(0xffffffffffffffff)),
          CGeneralRegister(U64C(0x7fffffffffffffff), U64C(0xffffffffffffffff)),
          CGeneralRegister(U64C(0x3333333333333333), U64C(0xffffffffffffffff))
        },
    };

    for (const auto & test : test_cases) {
        test_binop("and", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_clmul)
{
    // Constants taken from https://software.intel.com/sites/default/files/managed/72/cc/clmul-wp-rev-2.02-2014-04-20.pdf

    CGeneralRegister a(U64C(0x7b5b546573745665), U64C(0x63746f725d53475d));
    CGeneralRegister b(U64C(0x4869285368617929), U64C(0x5b477565726f6e5d));

    CGeneralRegister r00(U64C(0x1d4d84c85c3440c0), U64C(0x929633d5d36f0451));
    CGeneralRegister r01(U64C(0x1bd17c8d556ab5a1), U64C(0x7fa540ac2a281315));
    CGeneralRegister r10(U64C(0x1a2bf6db3a30862f), U64C(0xbabf262df4b7d5c9));
    CGeneralRegister r11(U64C(0x1d1e1f2c592e7c45), U64C(0xd66ee03e410fd4ed));

    CGeneralRegister a0(U64C(0x0000000000000000), U64C(0x8000000000000000));
    CGeneralRegister r(U64C(0x4000000000000000), U64C(0x0000000000000000));

    TestData_clmul test_cases[] = {
        { a,  b,  "clmulll", r00 },
        { b,  a,  "clmulhl", r01 },
        { a,  b,  "clmulhl", r10 },
        { a,  b,  "clmulhh", r11 },
        { a0, a0, "clmulll", r   },
    };

    for (const auto & test : test_cases) {
        test_clmul(test.insn, test);
    }
}

BOOST_AUTO_TEST_CASE(insn_cntlz)
{
    static const TestData_RaAssignRbShamt test_cases[] = {
        { CGeneralRegister(0, U64C(0x0)), 63, CGeneralRegister(0, 64) },
        { CGeneralRegister(0, U64C(0x1)), 31, CGeneralRegister(0, 31) },
        { CGeneralRegister(0, U64C(0x3)), 15, CGeneralRegister(0, 14) },
        { CGeneralRegister(0, U64C(0x7)),  7, CGeneralRegister(0,  5) },
    };

    for (const auto & test : test_cases) {
         test_bin_shamt("cntlz", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_cntpop)
{
    static const TestData_RaAssignRbShamt test_cases[] = {
        { CGeneralRegister(0, U64C(0x0)), 63, CGeneralRegister(0, 0) },
        { CGeneralRegister(0, U64C(0x1)), 32, CGeneralRegister(0, 1) },
        { CGeneralRegister(0, U64C(0x3)), 16, CGeneralRegister(0, 2) },
        { CGeneralRegister(0, U64C(0x7)),  8, CGeneralRegister(0, 3) },
    };

    for (const auto & test : test_cases) {
         test_bin_shamt("cntpop", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_cnttz)
{
    static const TestData_RaAssignRbShamt test_cases[] = {
        { CGeneralRegister(0, U64C(0x0)), 63, CGeneralRegister(0, 64) },
        { CGeneralRegister(0, U64C(0x1)), 32, CGeneralRegister(0,  0) },
        { CGeneralRegister(0, U64C(0x2)), 16, CGeneralRegister(0,  1) },
        { CGeneralRegister(0, U64C(0x4)),  8, CGeneralRegister(0,  2) },
    };

    for (const auto & test : test_cases) {
         test_bin_shamt("cnttz", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_faddss)
{
    static const TestData_binop test_cases[] = {
        { CGeneralRegister(0, 0, 0, U32C(0x3f000000)),
          CGeneralRegister(0, 0, 0, U32C(0x3ee66666)),
          CGeneralRegister(0, 0, 0, U32C(0x3f733333))
        },
    };

    for (const auto & test : test_cases) {
        test_binop("faddss", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_or)
{
    static const TestData_binop test_cases[] = {
        { CGeneralRegister(U64C(0xffffffffffffffff), U64C(0xffffffffffffffff)),
          CGeneralRegister(U64C(0x7fffffffffffffff), U64C(0xffffffffffffffff)),
          CGeneralRegister(U64C(0xffffffffffffffff), U64C(0xffffffffffffffff))
        },
    };

    for (const auto & test : test_cases) {
        test_binop("or", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_permb)
{
    const u64 val = U64C(0x1234567890abcdef);

    static const TestData_RaAssignRbShamt test_cases[] = {
        { CGeneralRegister(0, val), 63, CGeneralRegister(0, U64C(0xf7b3d5091e6a2c48)) },
        { CGeneralRegister(0, val), 32, CGeneralRegister(0, U64C(0x90abcdef12345678)) }, // swap u32
        { CGeneralRegister(0, val), 16, CGeneralRegister(0, U64C(0x56781234cdef90ab)) }, // swap u16
        { CGeneralRegister(0, val),  8, CGeneralRegister(0, U64C(0x34127856ab90efcd)) }, // swap u8
    };

    for (const auto & test : test_cases) {
         test_bin_shamt("permb", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_slli)
{
    const u64 val = U64C(0x1234567890abcdef);

    static const TestData_RaAssignRbShamt test_cases[] = {
        { CGeneralRegister(0, val), 63, CGeneralRegister(0, val << 63) },
        { CGeneralRegister(0, val), 32, CGeneralRegister(0, val << 32) },
        { CGeneralRegister(0, val), 16, CGeneralRegister(0, val << 16) },
        { CGeneralRegister(0, val),  8, CGeneralRegister(0, val <<  8) },
    };

    for (const auto & test : test_cases) {
         test_bin_shamt("slli", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_sub)
{
    static const TestData_binop test_cases[] = {
        { CGeneralRegister(0, 1), CGeneralRegister(0, 2), CGeneralRegister(0,-1) },
        { CGeneralRegister(0,-1), CGeneralRegister(0,-2), CGeneralRegister(0, 1) },
        { CGeneralRegister(0,-3), CGeneralRegister(0, 2), CGeneralRegister(0,-5) },
    };

    for (const auto & test : test_cases) {
         test_binop("sub", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_xnor)
{
    static const TestData_binop test_cases[] = {
        { CGeneralRegister(0, 1), CGeneralRegister(0, 2), CGeneralRegister(~U64C(0), ~U64C(3)) },
        { CGeneralRegister(0,-1), CGeneralRegister(0,-2), CGeneralRegister(~U64C(0), ~U64C(1)) },
        { CGeneralRegister(0,-3), CGeneralRegister(0, 2), CGeneralRegister(~U64C(0), ~U64C(-1)) },
        { CGeneralRegister(U64C(0xffffffffffffffff), -3),
          CGeneralRegister(U64C(0x7fffffffffffffff),  2),
          CGeneralRegister(~U64C(0x8000000000000000), ~U64C(-1)) },
    };

    for (const auto & test : test_cases) {
        test_binop("xnor", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_xor)
{
    static const TestData_binop test_cases[] = {
        { CGeneralRegister(0, 1), CGeneralRegister(0, 2), CGeneralRegister(0, 3) },
        { CGeneralRegister(0,-1), CGeneralRegister(0,-2), CGeneralRegister(0, 1) },
        { CGeneralRegister(0,-3), CGeneralRegister(0, 2), CGeneralRegister(0,-1) },
        { CGeneralRegister(U64C(0xffffffffffffffff), -3),
          CGeneralRegister(U64C(0x7fffffffffffffff),  2),
          CGeneralRegister(U64C(0x8000000000000000), -1) },
    };

    for (const auto & test : test_cases) {
        test_binop("xor", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_xori)
{
    static const TestData_binimm test_cases[] = {
        { CGeneralRegister(0, 1),  2, CGeneralRegister(0, 3) },
        { CGeneralRegister(0,-1), -2, CGeneralRegister(0, 1) },
        { CGeneralRegister(0,-3),  2, CGeneralRegister(0,-1) },
    };

    for (const auto & test : test_cases) {
         test_binimm("xori", test);
    }
}

BOOST_AUTO_TEST_SUITE_END() // instructions

////////////////////////////////////////////////////////////////////////////
// test suite end
////////////////////////////////////////////////////////////////////////////

} // namespace postrisc
