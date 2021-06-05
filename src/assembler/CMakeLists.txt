project(assembler)

add_library( ${PROJECT_NAME} OBJECT
    assembler.cpp
    assembler.hpp
    bin_generator.cpp
    bin_generator.hpp
    expression_evaluator.cpp
    html_generator.cpp
    html_generator.hpp
    llvm_backend_generator.cpp
    program_section.cpp
    program_section.hpp
    tokenizer.cpp
    tokenizer.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    util
    disassembler
    binary_format
    ${MPFR_LIBRARIES}
)
