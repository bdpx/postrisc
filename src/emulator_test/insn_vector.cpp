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

BOOST_AUTO_TEST_CASE(instr_vaddc1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18),
          GeneralRegister(243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255),
          GeneralRegister(  1 ,  1,   1,   1,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1)
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vaddcb, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vaddo1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127,  -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13,  -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(    0,    0,   0,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vaddob, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vadds1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127,  -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13,  -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister( +114, -102,-106, +77, -89, -86, -77, +10, -18, -52, -86,-101,+119, +85, +51, +17 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vaddb, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vaddss1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister( +114,-102,-106,-128,+127,+127,+127, +10, -18, -52, -86,-101,+119, +85, +51, +17 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vaddssatb, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vaddu1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18),
          GeneralRegister(243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255),
          GeneralRegister(114, 154, 150,  77, 167, 170, 179,  10, 238, 204, 170, 155, 119,  85,  51,  17)
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vaddb, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vaddus1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  255, 255, 255, 255, 167, 170, 179, 255, 255, 255, 255, 255, 255, 255, 255, 255 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vaddusatb, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vavgs1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(  +57, -51, -53, -89, +84, +85, +90,  +5,  -9, -26, -43, -50, +60, +43, +26,  +9 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vavgsb, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vavgu1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  185, 205, 203, 167,  84,  85,  90, 133, 247, 230, 213, 206, 188, 171, 154, 137 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vavgub, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vmaxs1)
{
    static const TestData_binop test_cases[] = {
        {
            GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
            GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
            GeneralRegister( +127, -51, -21, -81,+117, +86,+127, +18,  -1,  -1,  -1,  -1,+120, +86, +52, +18 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vmaxsb, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vmaxu1)
{
    static const TestData_binop test_cases[] = {
        {
            GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
            GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
            GeneralRegister(  243, 205, 235, 175, 117,  86, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vmaxub, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vmins1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(  -13, -51, -85, -98, +50, +84, +52,  -8, -17, -51, -85,-100,  -1,  -1,  -1,  -1 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vminsb, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vminu1)
{
    static const TestData_binop test_cases[] = {
        {
           GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
           GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
           GeneralRegister(  127, 205, 171, 158,  50,  84,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vminub, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vmrgh1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  127, 243, 205, 205, 171, 235, 158, 175, 117,  50,  86,  84,  52, 127,  18, 248 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vmergehb, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vmrgl1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  239, 255, 205, 255, 171, 255, 156, 255, 120, 255,  86, 255,  52, 255,  18, 255 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vmergelb, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vsubc1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(    1,    0,   1,   1,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,  1 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vsubcb, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vsubo1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127,  205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243,  205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(    1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0  )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vsubob, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vsubss1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister( +127,  +0, -64, -17, +67,  +2, -75, +26, -16, -50, -84, -99,+121, +87, +53, +19 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vsubssatb, test);
    }
}

BOOST_AUTO_TEST_CASE(instr_vsubu1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127,  205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243,  205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  140,    0, 192, 239,  67,   2, 181,  26, 240, 206, 172, 157, 121,  87,  53,  19 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vsubb, test);
    }
}


BOOST_AUTO_TEST_CASE(instr_vsubus1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(    0,   0,   0,   0,  67,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop(insn_vsubusatb, test);
    }
}

BOOST_AUTO_TEST_SUITE_END() // instructions

////////////////////////////////////////////////////////////////////////////
// test suite end
////////////////////////////////////////////////////////////////////////////

} // namespace postrisc
