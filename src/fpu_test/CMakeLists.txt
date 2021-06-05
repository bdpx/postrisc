project(fpu_test)

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
    controller.cpp
    controller.hpp
    float32.cpp
    float64.cpp
    float80.cpp
    float128.cpp
    float256.cpp
    float512.cpp
    floatx.cpp
    floatx.hpp
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
    arch
    assembler
    binary_format
    disassembler
    fpu
    util
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${FLOAT128_LIBRARIES}
)

#add_test(
#    NAME ${PROJECT_NAME}
#    COMMAND ${PROJECT_NAME}
#)

