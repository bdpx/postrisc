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
    config
    disassembler
    emulator
    emulator_app
    emulator_system
    util
)

target_link_libraries(${PROJECT_NAME}
    ${ATOMIC_LIBRARY}
    pthread
)

# generate html ISA manual
add_custom_command(
    OUTPUT ${ARTIFACTS_DIR}/gen.html
    COMMAND $<TARGET_FILE:postrisc> --generate-html > ${ARTIFACTS_DIR}/gen.html
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
        COMMAND xmllint ${XML_MAX_AMPLIFICATION_FACTOR} --noout --valid --nonet ${ARTIFACTS_DIR}/gen.html
        COMMENT "xmllint ISA html database: ${ARTIFACTS_DIR}/gen.html"
    )
endif()


# generate LLVM encoding tablegen
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/llvm)

add_custom_command(
    TARGET postrisc
    POST_BUILD
    COMMAND $<TARGET_FILE:postrisc> --generate-llvm > PostriscInstrEncoding.td
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/llvm
    COMMENT "Running $<TARGET_FILE_NAME:postrisc> --generate-llvm ..."
    VERBATIM
)

add_test(
    NAME llvm-tablegen
    COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/tests/PostriscInstrEncoding.td ${CMAKE_CURRENT_BINARY_DIR}/llvm/PostriscInstrEncoding.td
)
