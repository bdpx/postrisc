project(postrisc)

# for winpthread/atomic/seh dlls static import
#set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive")

add_executable(${PROJECT_NAME}
    main.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    assembler
    binary_format
    config
    disassembler
    emulator
    emulator_app
    emulator_system
    util
)

target_link_libraries(${PROJECT_NAME}
    atomic
    pthread
)

add_custom_command(
    OUTPUT ${ARTIFACTS_DIR}/gen.html
    COMMAND $<TARGET_FILE:postrisc> --html > ${ARTIFACTS_DIR}/gen.html
    DEPENDS ${PROJECT_NAME}
    COMMENT "Generated ISA html database: ${ARTIFACTS_DIR}/gen.html"
    VERBATIM
)
add_custom_target(generate_isa_html ALL
    DEPENDS ${ARTIFACTS_DIR}/gen.html
)

# XML validation requires w3c-sgml-lib DTD files
# MinGW hasn't offline SGML lib for XHTML checking,
# so don't verify xhtml on Windows
if(LINUX)
    add_custom_command(TARGET generate_isa_html
        POST_BUILD
        COMMAND xmllint --noout --valid --nonet ${ARTIFACTS_DIR}/gen.html
        COMMENT "xmllint ISA html database: ${ARTIFACTS_DIR}/gen.html"
    )
endif()
