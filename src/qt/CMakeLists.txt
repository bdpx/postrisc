cmake_minimum_required(VERSION 3.1.0)

project(qtpostrisc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)

add_executable(${PROJECT_NAME}
    main.cpp
    AssemblerHighLighter.cpp
    AssemblerHighLighter.hpp
    AssemblerTextEdit.cpp
    AssemblerTextEdit.hpp
    CoreWidget.cpp
    CoreWidget.hpp
    DisassemblerWidget.cpp
    DisassemblerWidget.hpp
    MainWindow.cpp
    MainWindow.hpp
    LoggerWidget.cpp
    LoggerWidget.hpp
    OpenGLWindow.cpp
    OpenGLWindow.hpp
    icons/icons.qrc
)

target_include_directories( ${PROJECT_NAME}
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
    Qt5::Widgets
    Qt5::OpenGL
)
