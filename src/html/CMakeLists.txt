project(html)

add_executable(${PROJECT_NAME}
    html_generator.cpp
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

add_custom_command(
    OUTPUT ${ARTIFACTS_DIR}/gen.html
    COMMAND $<TARGET_FILE:html> > ${ARTIFACTS_DIR}/gen.html
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
        COMMAND xmllint --noout --valid --nonet ${ARTIFACTS_DIR}/gen.html
        COMMENT "xmllint ISA html database: ${ARTIFACTS_DIR}/gen.html"
    )
endif()
