#include <boost/test/unit_test.hpp>

#include "util/common.hpp"

/********************************************************************************************
Because of possible using single compilation unit for building (CMAKE_UNITY_BUILD ON),
it's impossible to setup properly main initialization in one separated compilation unit
using boost test macros.
We can't use BOOST_TEST_MODULE or similar macro before including boost test headers.
Order of including sources in jumbo build may varied.

So common rules for all test projects:
1) include boost/test/unit_test.hpp without any user definitions in CMakeLists.txt:
add_compile_options(
    "SHELL:-include boost/test/unit_test.hpp"
)
2) add this file to sources to use custom init_unit_test_suite:
add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
)
*********************************************************************************************/

::boost::unit_test::test_suite* init_unit_test_suite(int argc, char** argv)
{
    postrisc::util::setup(argc, argv);
    return 0;
}
