project(disassembler)

add_library( ${PROJECT_NAME} OBJECT
    decoder.cpp
    decoder.hpp
    label_marker.cpp
    label_marker.hpp
    html_generator.cpp
    llvm_backend_generator.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
