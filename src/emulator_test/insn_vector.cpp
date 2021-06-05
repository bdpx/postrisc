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

BOOST_AUTO_TEST_CASE(insn_vaddc1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18),
          GeneralRegister(243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255),
          GeneralRegister(  1 ,  1,   1,   1,   0,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1)
        },
    };

    for (const auto& test : test_cases) {
        test_binop("addc_vu8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vaddo1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127,  -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13,  -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(    0,    0,   0,   1,   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("addo_vi8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vadds1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127,  -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13,  -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister( +114, -102,-106, +77, -89, -86, -77, +10, -18, -52, -86,-101,+119, +85, +51, +17 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("add_vu8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vaddss1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister( +114,-102,-106,-128,+127,+127,+127, +10, -18, -52, -86,-101,+119, +85, +51, +17 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("add_sat_vi8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vaddu1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18),
          GeneralRegister(243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255),
          GeneralRegister(114, 154, 150,  77, 167, 170, 179,  10, 238, 204, 170, 155, 119,  85,  51,  17)
        },
    };

    for (const auto& test : test_cases) {
        test_binop("add_vu8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vaddus1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  255, 255, 255, 255, 167, 170, 179, 255, 255, 255, 255, 255, 255, 255, 255, 255 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("add_sat_vu8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vavgs1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(  +57, -51, -53, -90, +83, +85, +89,  +5,  -9, -26, -43, -51, +59, +42, +25,  +8 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("avg_vi8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vavgu1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  185, 205, 203, 167,  84,  85,  90, 133, 247, 230, 213, 206, 188, 171, 154, 137 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("avg_vu8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vmaxs1)
{
    static const TestData_binop test_cases[] = {
        {
            GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
            GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
            GeneralRegister( +127, -51, -21, -81,+117, +86,+127, +18,  -1,  -1,  -1,  -1,+120, +86, +52, +18 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("max_vi8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vmaxu1)
{
    static const TestData_binop test_cases[] = {
        {
            GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
            GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
            GeneralRegister(  243, 205, 235, 175, 117,  86, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("max_vu8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vmins1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(  -13, -51, -85, -98, +50, +84, +52,  -8, -17, -51, -85,-100,  -1,  -1,  -1,  -1 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("min_vi8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vminu1)
{
    static const TestData_binop test_cases[] = {
        {
           GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
           GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
           GeneralRegister(  127, 205, 171, 158,  50,  84,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("min_vu8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vmrgh1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  127, 243, 205, 205, 171, 235, 158, 175, 117,  50,  86,  84,  52, 127,  18, 248 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("merge_high_vu8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vmrgl1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  239, 255, 205, 255, 171, 255, 156, 255, 120, 255,  86, 255,  52, 255,  18, 255 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("merge_low_vu8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vsubb1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister(    1,    0,   1,   1,   0,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,  1 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("subb_vu8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vsubo1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127,  205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243,  205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(    1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0  )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("subo_vi8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vsubss1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister( +127, -51, -85, -98,+117, +86, +52, +18, -17, -51, -85,-100,+120, +86, +52, +18 ),
          GeneralRegister(  -13, -51, -21, -81, +50, +84,+127,  -8,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1 ),
          GeneralRegister( +127,  +0, -64, -17, +67,  +2, -75, +26, -16, -50, -84, -99,+121, +87, +53, +19 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("sub_sat_vi8", test);
    }
}

BOOST_AUTO_TEST_CASE(insn_vsubu1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127,  205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243,  205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(  140,    0, 192, 239,  67,   2, 181,  26, 240, 206, 172, 157, 121,  87,  53,  19 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("sub_vu8", test);
    }
}


BOOST_AUTO_TEST_CASE(insn_vsubus1)
{
    static const TestData_binop test_cases[] = {
        { GeneralRegister(  127, 205, 171, 158, 117,  86,  52,  18, 239, 205, 171, 156, 120,  86,  52,  18 ),
          GeneralRegister(  243, 205, 235, 175,  50,  84, 127, 248, 255, 255, 255, 255, 255, 255, 255, 255 ),
          GeneralRegister(    0,   0,   0,   0,  67,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0 )
        },
    };

    for (const auto& test : test_cases) {
        test_binop("sub_sat_vu8", test);
    }
}

BOOST_AUTO_TEST_SUITE_END() // instructions

////////////////////////////////////////////////////////////////////////////
// test suite end
////////////////////////////////////////////////////////////////////////////

} // namespace postrisc
