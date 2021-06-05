project(disassembler)

add_library( ${PROJECT_NAME} OBJECT
    decoder.cpp
    decoder.hpp
    label_marker.cpp
    label_marker.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
