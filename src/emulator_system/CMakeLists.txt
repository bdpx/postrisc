project(emulator_system)

add_library(${PROJECT_NAME} OBJECT
    DeviceArray.cpp
    DeviceArray.hpp
    DeviceMemory.cpp
    DeviceMemory.hpp
    DeviceRom.cpp
    DeviceRom.hpp
    DeviceVideo.cpp
    DeviceVideo.hpp

    TargetSystem.cpp
    TargetSystem.hpp
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
