project(llvm)

add_executable(${PROJECT_NAME}
    llvm_backend_generator.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    arch
    binary_format
    disassembler
    util
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/llvm)

add_custom_command(
    TARGET llvm
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Running $<TARGET_FILE_NAME:llvm> ..."
    COMMAND $<TARGET_FILE:llvm> > PostriscInstrEncoding.td
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/llvm
)

add_test(
    NAME llvm
    COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_SOURCE_DIR}/PostriscInstrEncoding.td ${CMAKE_CURRENT_BINARY_DIR}/llvm/PostriscInstrEncoding.td
)
