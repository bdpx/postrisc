project(arch)

add_library( ${PROJECT_NAME} OBJECT
    Bundle.cpp
    Bundle.hpp
    html_generator.cpp
    instruction_fields.hpp
    InstructionDouble.hpp
    InstructionSingle.hpp
    isa.hpp
    llvm_backend_generator.cpp
    opcode.cpp
    opcode.hpp
    SortedNameTable.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
