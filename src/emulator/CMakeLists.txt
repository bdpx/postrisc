project(emulator)

add_library(${PROJECT_NAME} OBJECT
    configuration.cpp
    configuration.hpp
    core.cpp
    core.hpp
    debugger.cpp
    debugger.hpp

    devices/device.cpp
    devices/device.hpp
    devices/array.cpp
    devices/array.hpp
    devices/memory.cpp
    devices/memory.hpp
    devices/rom.cpp
    devices/rom.hpp
    devices/video.cpp
    devices/video.hpp

    dump/formatter.hpp
    dump/formatter_html.cpp
    dump/formatter_html.hpp
    dump/formatter_txt.cpp
    dump/formatter_txt.hpp

    memory.cpp
    memory.hpp

    instructions/aes.cpp
    instructions/aes.hpp
    instructions/atomic.hpp
    instructions/base.hpp
    instructions/emulator.hpp
    instructions/fpu.hpp
    instructions/system.hpp
    instructions/vector.hpp

    target_system.cpp
    target_system.hpp

    units/cpuid.cpp
    units/cpuid.hpp
    units/debug_register.hpp
    units/general_register.cpp
    units/general_register.hpp
    units/instruction_cache.hpp
    units/performance_monitor.cpp
    units/performance_monitor.hpp
    units/pic.cpp
    units/pic.hpp
    units/random.hpp
    units/register.cpp
    units/register_stack.hpp
    units/soft_exception.hpp
    units/status_register.cpp
    units/status_register.hpp
    units/translation_cache.hpp
    units/translation_unit.cpp
    units/translation_unit.hpp

    utils.hpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    disassembler
    binary_format
    fpu
    util
)

target_link_libraries(${PROJECT_NAME}
    atomic
    pthread
    ${Boost_SERIALIZATION_LIBRARY}
)
