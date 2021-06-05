project(emulator_app)

add_library(${PROJECT_NAME} OBJECT
    ElfLoader.cpp
    ElfLoader.hpp
    TargetSingleApp.cpp
    TargetSingleApp.hpp
    TargetSingleAppSyscall.cpp
    TargetSingleAppSyscallDoom.cpp
    windows/win.cpp
    windows/posix.h

    # copy of ../../../llvm-project/llvm/include/llvm/BinaryFormat/ELFRelocs/Postrisc.def
    # postrisc_relocs.def
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    binary_format
    disassembler
    emulator
    util
)

target_link_libraries(${PROJECT_NAME}
    ${ATOMIC_LIBRARY}
    pthread
    ${Boost_SERIALIZATION_LIBRARY}
)
