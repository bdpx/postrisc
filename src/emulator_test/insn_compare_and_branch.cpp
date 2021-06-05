#include "util/common.hpp"
#include "arch/interruption.hpp"
#include "arch/instructions.hpp"
#include "arch/isa.hpp"
#include "arch/abi.hpp"
#include "emulator/core.hpp"
#include "emulator/target_system.hpp"
#include "assembler/assembler.hpp"
#include "emulator_test_fixture.hpp"

namespace postrisc {

////////////////////////////////////////////////////////////////////////////
// test suite begin
////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE(instructions, emulator_test_fixture)

BOOST_AUTO_TEST_CASE(insn_beqz)
{
    static const TestData_CompareWithZeroAndBranch test_cases[] = {
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000000) ), 1 },
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000002) ), 0 },
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000007) ), 0 },
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0xffffffffffffffff) ), 0 },
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x000000000000001f) ), 0 },
        { CGeneralRegister( UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000000) ), 0 },
    };

    for (const auto & test : test_cases) {
        TestCompareWithZeroAndBranch("beqz", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bbc)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( 0 ), 0 },
        { CGeneralRegister( INT64_C(2) ), CGeneralRegister( 1 ), 0 },
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( 3 ), 1 },
        { CGeneralRegister( INT64_C(-1) ), CGeneralRegister( 0 ), 0 },
        { CGeneralRegister( INT64_C(-122) ), CGeneralRegister( 11 ), 0 },
        { CGeneralRegister( UINT64_C(1), INT64_C(-122) ), CGeneralRegister( 64 ), 0 },
    };

    for (const auto & test : test_cases) {
        test_compare_and_branch("bbc", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bbs)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( 0 ), 1 },
        { CGeneralRegister( INT64_C(2) ), CGeneralRegister( 1 ), 1 },
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( 3 ), 0 },
        { CGeneralRegister( INT64_C(-1) ), CGeneralRegister( 0 ), 1 },
        { CGeneralRegister( INT64_C(-122) ), CGeneralRegister( 11 ), 1 },
        { CGeneralRegister( UINT64_C(1), INT64_C(-122) ), CGeneralRegister( 64 ), 1 },
    };

    for (const auto & test : test_cases) {
        test_compare_and_branch("bbs", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_beq)
{
    // beq compares full 128 bit regs
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000001) ), 1 },
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000002) ), 0 },
        { CGeneralRegister( UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000002), UINT64_C(0x0000000000000001) ), 0 },
        { CGeneralRegister( UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000001) ), 1 },
    };

    for (const auto & test : test_cases)
    {
        test_compare_and_branch("beq", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bne)
{
    // bne compares full 128 bit regs
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000001) ), 0 },
        { CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000000), UINT64_C(0x0000000000000002) ), 1 },
        { CGeneralRegister( UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000002), UINT64_C(0x0000000000000001) ), 1 },
        { CGeneralRegister( UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000001) ),
          CGeneralRegister( UINT64_C(0x0000000000000001), UINT64_C(0x0000000000000001) ), 0 },
    };

    for (const auto & test : test_cases) {
        test_compare_and_branch("bne", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_ble)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( INT64_C(1) ), 1 },
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( INT64_C(2) ), 1 },
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( INT64_C(3) ), 1 },
        { CGeneralRegister( INT64_C(-1) ), CGeneralRegister( INT64_C(0) ), 1 },
        { CGeneralRegister( INT64_C(-122) ), CGeneralRegister( INT64_C(11) ), 1 },
    };

    for (const auto & test : test_cases) {
        test_compare_and_branch("ble", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bleu)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(1) ), 1 },
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(2) ), 1 },
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(3) ), 1 },
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(0) ), 0 },
    };

    for (const auto & test : test_cases) {
        test_compare_and_branch("bleu", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_blt)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( INT64_C(1) ), 0 },
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( INT64_C(2) ), 1 },
        { CGeneralRegister( INT64_C(1) ), CGeneralRegister( INT64_C(3) ), 1 },
        { CGeneralRegister( INT64_C(-1) ), CGeneralRegister( INT64_C(0) ), 1 },
        { CGeneralRegister( INT64_C(-122) ), CGeneralRegister( INT64_C(11) ), 1 },
    };

    for (const auto & test : test_cases) {
        test_compare_and_branch("blt", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bltu)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(1) ), 0 },
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(2) ), 1 },
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(3) ), 1 },
        { CGeneralRegister( UINT64_C(1) ), CGeneralRegister( UINT64_C(0) ), 0 },
    };

    for (const auto & test : test_cases) {
        test_compare_and_branch("bltu", test);
    }
}

BOOST_AUTO_TEST_SUITE_END() // instructions

////////////////////////////////////////////////////////////////////////////
// test suite end
////////////////////////////////////////////////////////////////////////////

} // namespace postrisc
