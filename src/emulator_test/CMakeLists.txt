project(emulator_test)

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
    aes.cpp
    emulator_test_fixture.cpp
    emulator_test_fixture.hpp
    insn_compare_and_branch.cpp
    insn_compute.cpp
    insn_vector.cpp
    memory.cpp
    translation_unit.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/emulator/
)

target_link_libraries(${PROJECT_NAME}
    arch
    assembler
    binary_format
    config
    disassembler
    emulator
    emulator_app
    emulator_system
    util
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_SERIALIZATION_LIBRARY}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)
