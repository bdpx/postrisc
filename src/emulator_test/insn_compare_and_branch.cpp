#include <boost/test/unit_test.hpp>

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

BOOST_AUTO_TEST_CASE(insn_bbc)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( I64C(1) ), CGeneralRegister( 0 ), 0 },
        { CGeneralRegister( I64C(2) ), CGeneralRegister( 1 ), 0 },
        { CGeneralRegister( I64C(1) ), CGeneralRegister( 3 ), 1 },
        { CGeneralRegister( I64C(-1) ), CGeneralRegister( 0 ), 0 },
        { CGeneralRegister( I64C(-122) ), CGeneralRegister( 11 ), 0 },
        { CGeneralRegister( U64C(1), I64C(-122) ), CGeneralRegister( 64 ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bbc", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bbs)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( I64C(1) ), CGeneralRegister( 0 ), 1 },
        { CGeneralRegister( I64C(2) ), CGeneralRegister( 1 ), 1 },
        { CGeneralRegister( I64C(1) ), CGeneralRegister( 3 ), 0 },
        { CGeneralRegister( I64C(-1) ), CGeneralRegister( 0 ), 1 },
        { CGeneralRegister( I64C(-122) ), CGeneralRegister( 11 ), 1 },
        { CGeneralRegister( U64C(1), I64C(-122) ), CGeneralRegister( 64 ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bbs", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_beq)
{
    // beq compares full 128 bit regs
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ), 1 },
        { CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000002) ), 0 },
        { CGeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000002), U64C(0x0000000000000001) ), 1 },
        { CGeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bdeq", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bne)
{
    // bne compares full 128 bit regs
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ), 0 },
        { CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000002) ), 1 },
        { CGeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000002), U64C(0x0000000000000001) ), 0 },
        { CGeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          CGeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bdne", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_ble)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( I64C(1) ), CGeneralRegister( I64C(1) ), 1 },
        { CGeneralRegister( I64C(1) ), CGeneralRegister( I64C(2) ), 1 },
        { CGeneralRegister( I64C(1) ), CGeneralRegister( I64C(3) ), 1 },
        { CGeneralRegister( I64C(-1) ), CGeneralRegister( I64C(0) ), 1 },
        { CGeneralRegister( I64C(-122) ), CGeneralRegister( I64C(11) ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bdle", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bleu)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(1) ), 1 },
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(2) ), 1 },
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(3) ), 1 },
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(0) ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bdleu", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_blt)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( I64C(1) ), CGeneralRegister( I64C(1) ), 0 },
        { CGeneralRegister( I64C(1) ), CGeneralRegister( I64C(2) ), 1 },
        { CGeneralRegister( I64C(1) ), CGeneralRegister( I64C(3) ), 1 },
        { CGeneralRegister( I64C(-1) ), CGeneralRegister( I64C(0) ), 1 },
        { CGeneralRegister( I64C(-122) ), CGeneralRegister( I64C(11) ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bdlt", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bltu)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(1) ), 0 },
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(2) ), 1 },
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(3) ), 1 },
        { CGeneralRegister( U64C(1) ), CGeneralRegister( U64C(0) ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("bdltu", test);
    }
}

BOOST_AUTO_TEST_SUITE_END() // instructions

////////////////////////////////////////////////////////////////////////////
// test suite end
////////////////////////////////////////////////////////////////////////////

} // namespace postrisc
