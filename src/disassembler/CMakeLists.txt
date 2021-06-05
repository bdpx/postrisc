project(disassembler)

add_library( ${PROJECT_NAME} OBJECT
    Decoder.cpp
    Decoder.hpp
    ExeFileHeader.cpp
    ExeFileHeader.hpp
    LabelMarker.cpp
    LabelMarker.hpp
    html_generator.cpp
    llvm_backend_generator.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
