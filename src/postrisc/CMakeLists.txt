project(postrisc)

# for winpthread/atomic/seh dlls static import
#set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_executable(${PROJECT_NAME}
    configuration.cpp
    configuration.hpp
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
    emulator
    fpu
    util
)

target_link_libraries(${PROJECT_NAME}
    atomic
    pthread
    ${BOOST_UNITTEST_LIB}
)
