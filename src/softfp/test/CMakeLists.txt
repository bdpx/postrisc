project(fpu_test)

add_compile_options(
    "SHELL:-include boost/test/unit_test.hpp"
)

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
    controller.cpp
    controller.hpp
    float64.cpp
    functions.hpp
    hardfloat.hpp
    sequencer.cpp
    sequencer.hpp
    testing.cpp
    timing.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/fpu/
)

target_link_libraries(${PROJECT_NAME}
    fpu
    util
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${MPFR_LIBRARIES}
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)

