project(emulator_app)

add_library(${PROJECT_NAME} OBJECT
    elf_loader.cpp
    elf_loader.hpp
    target_app.cpp
    target_app.hpp
    target_app_syscall.cpp
    target_app_syscall_doom.cpp
    windows/win.cpp
    windows/posix.h
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
    atomic
    pthread
    ${Boost_SERIALIZATION_LIBRARY}
)
