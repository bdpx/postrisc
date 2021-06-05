project(emulator_system)

add_library(${PROJECT_NAME} OBJECT
    devices/array.cpp
    devices/array.hpp
    devices/memory.cpp
    devices/memory.hpp
    devices/rom.cpp
    devices/rom.hpp
    devices/video.cpp
    devices/video.hpp

    target_system.cpp
    target_system.hpp
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
