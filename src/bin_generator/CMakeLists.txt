project(bin_generator)

add_executable(${PROJECT_NAME}
    BinaryRandomGenerator.cpp
    BinaryRandomGenerator.hpp
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    assembler
    binary_format
    disassembler
    util
)
