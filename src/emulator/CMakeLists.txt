project(emulator)

add_library(${PROJECT_NAME} OBJECT
    IAddressSpace.hpp
    IManagedAddressSpace.hpp

    core.cpp
    core.hpp

    core_aes.cpp
    core_atomic.cpp
    core_base.cpp
    core_emulator.cpp
    core_fpu.cpp
    core_system.cpp
    core_vector.cpp

    debugger.cpp
    debugger.hpp

    device.cpp
    device.hpp

    interruption.cpp
    interruption.hpp

    dump/formatter.hpp
    dump/formatter_html.cpp
    dump/formatter_html.hpp
    dump/formatter_txt.cpp
    dump/formatter_txt.hpp

    cpuid.cpp
    cpuid.hpp
    debug_register.hpp
    general_register.cpp
    general_register.hpp
    instruction_cache.hpp
    memory.cpp
    memory.hpp
    performance_monitor.cpp
    performance_monitor.hpp
    pic.cpp
    pic.hpp
    random.hpp
    register.cpp
    register_stack.hpp
    soft_exception.hpp
    status_register.cpp
    status_register.hpp
    translation_helpers.hpp
    translation_unit.cpp
    translation_unit.hpp
    utils.hpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:DEBUG>:DEBUG_LINK_INFO_STACK>
    $<$<CONFIG:DEBUG>:DEBUG_BACKSTORE_STACK>
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    disassembler
    binary_format
    util
)

target_link_libraries(${PROJECT_NAME}
    atomic
    pthread
    ${Boost_SERIALIZATION_LIBRARY}
)
