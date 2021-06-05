project(arch)

add_library( ${PROJECT_NAME} OBJECT
    abi.hpp
    insn_table.hpp
    instruction_fields.hpp
    instructions.cpp
    instructions.hpp
    interruption.cpp
    interruption.hpp
    isa.hpp
    opcode.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
