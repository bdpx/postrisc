project(assembler_test)

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
    assembler.cpp
    evaluator.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/assembler/
)

target_link_libraries(${PROJECT_NAME}
    util
    arch
    assembler
    disassembler
    binary_format
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)

