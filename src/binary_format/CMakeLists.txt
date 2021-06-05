project(binary_format)

add_library( ${PROJECT_NAME} OBJECT
    file_header.cpp
    file_header.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)
