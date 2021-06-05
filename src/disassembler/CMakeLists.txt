project(disassembler)

add_library( ${PROJECT_NAME} OBJECT
    Decoder.cpp
    Decoder.hpp
    LabelMarker.cpp
    LabelMarker.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
