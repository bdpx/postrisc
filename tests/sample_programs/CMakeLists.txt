project(sample_programs)

#patch xml to replace ${NAME} entry points to real paths/values
# ROM_IMAGE_PATH var is used in config.xml
set(ORIGINAL_XML "${CMAKE_SOURCE_DIR}/tests/config.xml")
set(PATCHED_CONFIG_XML "${CMAKE_CURRENT_BINARY_DIR}/patched_config.xml")
set(TIMING_INFO "false")
set(VERBOSE_INFO "false")
get_filename_component(ROM_IMAGE_PATH "${CMAKE_SOURCE_DIR}/LICENSE.md" ABSOLUTE)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/patched_config.xml
    COMMAND ${CMAKE_COMMAND}
        -D XML_SRC=${ORIGINAL_XML}
        -D XML_DST=${PATCHED_CONFIG_XML}
        -D PATCHED_CONFIG_XML=${PATCHED_CONFIG_XML}
        -D TIMING_INFO=false
        -D VERBOSE_INFO=false
        -D ROM_IMAGE_PATH=${ROM_IMAGE_PATH}
        -P ${CMAKE_SOURCE_DIR}/tests/configure_xml_config.cmake
    DEPENDS
        ${CMAKE_SOURCE_DIR}/tests/configure_xml_config.cmake
        ${ORIGINAL_XML}
    COMMENT "Configuring ${CMAKE_CURRENT_BINARY_DIR}/patched_config.xml"
    VERBATIM
)

add_custom_target(configure_json_samples ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/patched_config.xml
)

list(APPEND PROGRAM_SRCS
    aes.s
    arithmetic.s
    atomic.s
    base_addressing.s
    bit_population.s
    bit_reverse.s
    bitwise_logical.s
    blti.s
    call_example.s
    call_recursive.s
    carry.s
    compare.s
    compare_and_branch.s
    compare_bit_and_branch.s
    compare_with_zero_and_long_branch.s
    cpuid.s
    data_lo.s
    data_types.s
    deposit.s
    device_control.s
    device_map.s
    exception.s
    fpu.s
    indexed_modify.s
    ip_relative_data.s
    ldan.s
    mbsel.s
    mfspr.s
    minmax.s
    nullification.s
    performance_monitor.s
    performance_test.s
    performance_test_2.s
    random.s
    repgei.s
    shift.s
    simd_fpu.s
    simd_int.s
    system.s
    table_branch.s
    trap_conditional.s
    write.s
)

foreach(program_src ${PROGRAM_SRCS})
    # message(STATUS "asm test file: ${FILENAME}")
    set(NAME parse-${program_src})
    add_test(
        NAME ${NAME}
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tester.bash ${program_src} $<TARGET_FILE:postrisc> ${CMAKE_CURRENT_SOURCE_DIR}
    )
    set_property(TEST ${NAME}        PROPERTY ENVIRONMENT TEST_DIR=${CMAKE_CURRENT_BINARY_DIR})
    set_property(TEST ${NAME} APPEND PROPERTY ENVIRONMENT CONFIG_XML=${PATCHED_CONFIG_XML})
endforeach(program_src)

###############################################################################
# all-in-one test
###############################################################################

list(TRANSFORM PROGRAM_SRCS PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/" OUTPUT_VARIABLE PROGRAM_SRCS_1)
list(TRANSFORM PROGRAM_SRCS_1 APPEND ".out" OUTPUT_VARIABLE PROGRAM_SRCS_OUT)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/all_in_one.s 
    COMMAND cat ${PROGRAM_SRCS_1} > ${CMAKE_CURRENT_BINARY_DIR}/all_in_one.s
    DEPENDS ${PROGRAM_SRCS}
    COMMENT "generate all-in-one assembler file"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/all_in_one.s.out
    COMMAND cat ${PROGRAM_SRCS_OUT} > ${CMAKE_CURRENT_BINARY_DIR}/all_in_one.s.out
    DEPENDS ${PROGRAM_SRCS}
    COMMENT "generate all-in-one result out file"
)

add_custom_target(cut_all_in_one_sample ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/all_in_one.s 
        ${CMAKE_CURRENT_BINARY_DIR}/all_in_one.s.out
    COMMENT "generate all-in-one files"
)

set(PATCHED_CONFIG_XML_BIG "${CMAKE_CURRENT_BINARY_DIR}/patched_config_big.xml")

add_custom_command(
    OUTPUT ${PATCHED_CONFIG_XML_BIG}
    COMMAND ${CMAKE_COMMAND}
        -D XML_SRC=${ORIGINAL_XML}
        -D XML_DST=${PATCHED_CONFIG_XML_BIG}
        -D PATCHED_CONFIG_XML=${PATCHED_CONFIG_XML_BIG}
        -D TIMING_INFO=false
        -D VERBOSE_INFO=false
        -D ROM_IMAGE_PATH=${ROM_IMAGE_PATH}
        -P ${CMAKE_SOURCE_DIR}/tests/configure_xml_config.cmake
    DEPENDS
        ${CMAKE_SOURCE_DIR}/tests/configure_xml_config.cmake
        ${ORIGINAL_XML}
    COMMENT "Configuring ${PATCHED_CONFIG_XML_BIG}"
    VERBATIM
)

add_custom_target(configure_json_big_test ALL
    DEPENDS ${PATCHED_CONFIG_XML_BIG}
)

set(TEST_NAME "big")
add_test(
    NAME ${TEST_NAME}
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tester_big.bash ${CMAKE_CURRENT_BINARY_DIR}/all_in_one.s $<TARGET_FILE:postrisc> ${CMAKE_CURRENT_BINARY_DIR}
)
set_property(TEST ${TEST_NAME}        PROPERTY ENVIRONMENT TEST_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR})
set_property(TEST ${TEST_NAME} APPEND PROPERTY ENVIRONMENT TEST_SAMPLES_DIR=${CMAKE_SOURCE_DIR}/tests/sample_programs)
set_property(TEST ${TEST_NAME} APPEND PROPERTY ENVIRONMENT TEST_DIR=${CMAKE_CURRENT_BINARY_DIR})
set_property(TEST ${TEST_NAME} APPEND PROPERTY ENVIRONMENT EXEFILE=$<TARGET_FILE:postrisc>)
set_property(TEST ${TEST_NAME} APPEND PROPERTY ENVIRONMENT CONFIG_XML=${PATCHED_CONFIG_XML_BIG})
set_property(TEST ${TEST_NAME} APPEND PROPERTY ENVIRONMENT DOCS_DIR=${CMAKE_SOURCE_DIR}/docs)
