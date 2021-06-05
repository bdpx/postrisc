project(llvm_codegen)

add_executable(${PROJECT_NAME}
    atomics.cpp
    bitcast.cpp
    bitwise.cpp
    branches.cpp
    compare.cpp
    divide_pow2.cpp

    fp_arithmetic.cpp
    int_arithmetic.cpp

    Generator.cpp
    Generator.hpp

    immediates.cpp
    immediates.hpp

    integer.cpp
    load_store.cpp
    main.cpp
    minmax.cpp
    misc.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    binary_format
    disassembler
    util
)
