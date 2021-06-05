project(assembler)

add_library( ${PROJECT_NAME} OBJECT
    Assembler.cpp
    Assembler.hpp
    ExpressionEvaluator.cpp
    ProgramSection.cpp
    ProgramSection.hpp
    Tokenizer.cpp
    Tokenizer.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    util
    disassembler
    binary_format
    ${FLOAT128_LIBRARIES}
)
