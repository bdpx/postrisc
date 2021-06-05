#include <boost/test/unit_test.hpp>

#include "util/common.hpp"
#include "arch/Bundle.hpp"
#include "arch/isa.hpp"
#include "emulator/Core.hpp"
#include "emulator_system/TargetSystem.hpp"
#include "assembler/Assembler.hpp"
#include "emulator_test_fixture.hpp"

namespace postrisc {

////////////////////////////////////////////////////////////////////////////
// test suite begin
////////////////////////////////////////////////////////////////////////////

BOOST_FIXTURE_TEST_SUITE(instructions, emulator_test_fixture)

BOOST_AUTO_TEST_CASE(insn_bbc)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( I64C(1) ), GeneralRegister( 0 ), 0 },
        { GeneralRegister( I64C(2) ), GeneralRegister( 1 ), 0 },
        { GeneralRegister( I64C(1) ), GeneralRegister( 3 ), 1 },
        { GeneralRegister( I64C(-1) ), GeneralRegister( 0 ), 0 },
        { GeneralRegister( I64C(-122) ), GeneralRegister( 11 ), 0 },
        { GeneralRegister( U64C(1), I64C(-122) ), GeneralRegister( 64 ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_bc", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bbs)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( I64C(1) ), GeneralRegister( 0 ), 1 },
        { GeneralRegister( I64C(2) ), GeneralRegister( 1 ), 1 },
        { GeneralRegister( I64C(1) ), GeneralRegister( 3 ), 0 },
        { GeneralRegister( I64C(-1) ), GeneralRegister( 0 ), 1 },
        { GeneralRegister( I64C(-122) ), GeneralRegister( 11 ), 1 },
        { GeneralRegister( U64C(1), I64C(-122) ), GeneralRegister( 64 ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_bs", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_beq)
{
    // beq compares full 128 bit regs
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ), 1 },
        { GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000002) ), 0 },
        { GeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000002), U64C(0x0000000000000001) ), 1 },
        { GeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_eq_i64", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bne)
{
    // bne compares full 128 bit regs
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ), 0 },
        { GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000000), U64C(0x0000000000000002) ), 1 },
        { GeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000002), U64C(0x0000000000000001) ), 0 },
        { GeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ),
          GeneralRegister( U64C(0x0000000000000001), U64C(0x0000000000000001) ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_ne_i64", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_br_ge)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( I64C(1) ), GeneralRegister( I64C(1) ), 1 },
        { GeneralRegister( I64C(2) ), GeneralRegister( I64C(1) ), 1 },
        { GeneralRegister( I64C(3) ), GeneralRegister( I64C(1) ), 1 },
        { GeneralRegister( I64C(0) ), GeneralRegister( I64C(-1) ), 1 },
        { GeneralRegister( I64C(11) ), GeneralRegister( I64C(-122) ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_ge_i64", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bleu)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( U64C(1) ), GeneralRegister( U64C(1) ), 1 },
        { GeneralRegister( U64C(2) ), GeneralRegister( U64C(1) ), 1 },
        { GeneralRegister( U64C(3) ), GeneralRegister( U64C(1) ), 1 },
        { GeneralRegister( U64C(0) ), GeneralRegister( U64C(1) ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_ge_u64", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_blt)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( I64C(1) ), GeneralRegister( I64C(1) ), 0 },
        { GeneralRegister( I64C(1) ), GeneralRegister( I64C(2) ), 1 },
        { GeneralRegister( I64C(1) ), GeneralRegister( I64C(3) ), 1 },
        { GeneralRegister( I64C(-1) ), GeneralRegister( I64C(0) ), 1 },
        { GeneralRegister( I64C(-122) ), GeneralRegister( I64C(11) ), 1 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_lt_i64", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_bltu)
{
    static const TestData_CompareAndBranch test_cases[] = {
        { GeneralRegister( U64C(1) ), GeneralRegister( U64C(1) ), 0 },
        { GeneralRegister( U64C(1) ), GeneralRegister( U64C(2) ), 1 },
        { GeneralRegister( U64C(1) ), GeneralRegister( U64C(3) ), 1 },
        { GeneralRegister( U64C(1) ), GeneralRegister( U64C(0) ), 0 },
    };

    for (const auto& test : test_cases) {
        test_compare_and_branch("br_lt_u64", test);
    }
}

BOOST_AUTO_TEST_SUITE_END() // instructions

////////////////////////////////////////////////////////////////////////////
// test suite end
////////////////////////////////////////////////////////////////////////////

} // namespace postrisc
