project(emulator)

add_library(${PROJECT_NAME} OBJECT
    AddressSpace.hpp

    Core.cpp
    Core.hpp

    CoreAes.cpp
    CoreAtomic.cpp
    CoreBase.cpp
    CoreEmulator.cpp
    CoreFpu.cpp
    CoreSystem.cpp
    CoreVector.cpp

    Debugger.cpp
    Debugger.hpp

    Device.cpp
    Device.hpp

    CpuIdentificationStore.cpp
    CpuIdentificationStore.hpp

    DataBreakpointRegister.hpp

    DumpFormatter.hpp
    DumpFormatter.cpp

    GeneralRegister.hpp

    InstructionBreakpointRegister.hpp

    InstructionCache.hpp

    InstructionStatisticCollector.cpp
    InstructionStatisticCollector.hpp

    ManagedAddressSpace.hpp

    ProcessorStatusRegister.cpp
    ProcessorStatusRegister.hpp

    PerformanceMonitor.cpp
    PerformanceMonitor.hpp

    ProgrammableInterruptController.cpp
    ProgrammableInterruptController.hpp

    RandomUnit.hpp

    RegisterStackControlRegister.hpp

    Result.cpp
    Result.hpp

    SoftExceptionState.hpp

    TranslateBuffer.cpp
    TranslateBuffer.hpp

    TranslationHelpers.hpp

    TranslationUnit.cpp
    TranslationUnit.hpp

    Utils.hpp
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
#    $<$<CONFIG:DEBUG>:DEBUG_LINK_INFO_STACK>
#    $<$<CONFIG:DEBUG>:DEBUG_BACKSTORE_STACK>
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
    ${ATOMIC_LIBRARY}
    pthread
    ${Boost_SERIALIZATION_LIBRARY}
)
