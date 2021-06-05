project(arch)

add_library( ${PROJECT_NAME} OBJECT
    Bundle.cpp
    Bundle.hpp

    insn_table.hpp
    instruction_fields.hpp

    InstructionDouble.hpp
    InstructionSingle.hpp

    isa.hpp
    opcode.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
