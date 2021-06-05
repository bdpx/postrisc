#define BOOST_TEST_MODULE test
#include <boost/test/unit_test.hpp>

#include "util/common.hpp"

/********************************************************************************************
Properly boost test main initialization should be only once.
In case of single compilation unit building (CMAKE_UNITY_BUILD ON)
we need to place BOOST_TEST_MODULE in first SCU-included cpp.
So common rule is: add this file at first place to sources:

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
)
*********************************************************************************************/

namespace postrisc {

struct test_config {
    test_config() {
        util::setup();
        std::cout << "global setup\n";
    }
    ~test_config() {
        std::cout << "global teardown\n";
    }
};

BOOST_GLOBAL_FIXTURE(test_config);

} // namespace postrisc
