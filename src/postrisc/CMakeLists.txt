project(postrisc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(${PROJECT_NAME} # to avoid console
    main.cpp
    AssemblerHighLighter.cpp
    AssemblerHighLighter.hpp
    AssemblerTextEdit.cpp
    AssemblerTextEdit.hpp
    CoreWidget.cpp
    CoreWidget.hpp
    DisassemblerWidget.cpp
    DisassemblerWidget.hpp
    FrameBufferWindow.cpp
    FrameBufferWindow.hpp
    MainWindow.cpp
    MainWindow.hpp
    LoggerWidget.cpp
    LoggerWidget.hpp
    OpenGLWindow.cpp
    OpenGLWindow.hpp
    icons/icons.qrc
)


target_compile_definitions(${PROJECT_NAME} PRIVATE
    PATH_TO_SAMPLES="${CMAKE_SOURCE_DIR}/tests/samples/"
    SELECTED_SAMPLE="nullification.s"
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    assembler
    disassembler
    emulator
    emulator_app
    emulator_system
    llvm_codegen
    util
    Qt6::Widgets
    Qt6::OpenGL
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
