project(qtpostrisc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(${PROJECT_NAME} WIN32 # to avoid console
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
    binary_format
    config
    disassembler
    emulator
    emulator_app
    emulator_system
    util
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    Qt6::OpenGL
)
