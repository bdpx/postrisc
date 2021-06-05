project(config)

add_library(${PROJECT_NAME} OBJECT
    configuration.cpp
    configuration.hpp
    emulator_configuration.cpp
    emulator_configuration.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    assembler
    binary_format
    disassembler
    emulator
    fpu
    util
)
