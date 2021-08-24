project(tests)

#patch xml to replace ${NAME} entry points to real paths/values
# ROM_IMAGE_PATH var is used in config.xml
get_filename_component(ROM_IMAGE_PATH "${CMAKE_SOURCE_DIR}/tests/big/program.bin" ABSOLUTE)

# function to rerun cmake after file change
function(watch)
    set_property(
        DIRECTORY
        APPEND
        PROPERTY CMAKE_CONFIGURE_DEPENDS ${ARGV}
    )
endfunction()

set(ORIGINAL_XML "${CMAKE_SOURCE_DIR}/tests/config.xml")
watch(${ORIGINAL_XML})

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/samples)

list(APPEND PROGRAM_SRCS
    aes
    arithmetic
    atomic
    base_addressing
    bit_population
    bit_reverse
    bitwise_logical
    branch_int
    branch_on_zero
    call_example
    call_recursive
    call_simple
    carry
    compare
    compare_and_branch
    compare_bit_and_branch
    cpuid
    crc32c
    data_lo
    data_types
    dense_calls
    deposit
    device_control
    device_map
    exception
    fpu
    indexed_modify
    ip_relative_data
    ca_rf
    mbsel
    mfspr
    minmax
    nullification
    performance_monitor
    performance_test
    performance_test_2
    random
    repgei
    shift
    simd_fpu
    simd_int
    system
    unaligned
    table_branch
    write
)

foreach(program_src ${PROGRAM_SRCS})
    # message(STATUS "asm test file: ${FILENAME}")
    add_test(
        NAME parse-${program_src}
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tester.py
            ${CMAKE_CURRENT_SOURCE_DIR}/samples/${program_src}.s      # TEST_FILE: asm file
            ${CMAKE_CURRENT_SOURCE_DIR}/samples/${program_src}.out    # RESULT_FILE: out file
            $<TARGET_FILE:postrisc>                                   # EXEFILE
            ${CMAKE_CURRENT_BINARY_DIR}/samples                       # OUTPUT_DIR
            ${ROM_IMAGE_PATH}                                         # sample ROM file
    )
endforeach(program_src)

###############################################################################
# all-in-one test
###############################################################################

list(TRANSFORM PROGRAM_SRCS APPEND ".out" OUTPUT_VARIABLE PROGRAM_SRCS_OUT)
list(TRANSFORM PROGRAM_SRCS APPEND ".s" OUTPUT_VARIABLE PROGRAM_SRCS_S)

list(TRANSFORM PROGRAM_SRCS_S   PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/samples/" OUTPUT_VARIABLE PROGRAM_SRCS_S_FULL)
list(TRANSFORM PROGRAM_SRCS_OUT PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/samples/" OUTPUT_VARIABLE PROGRAM_SRCS_OUT_FULL)

#message(STATUS "PROGRAM_SRCS_1 = ${PROGRAM_SRCS_1}")
#message(STATUS "PROGRAM_SRCS_OUT = ${PROGRAM_SRCS_OUT}")

add_custom_command(
    OUTPUT "${ARTIFACTS_DIR}/all_in_one.s"
    COMMAND
        cat ${PROGRAM_SRCS_S_FULL} > ${ARTIFACTS_DIR}/all_in_one.s
    DEPENDS ${PROGRAM_SRCS_S_FULL}
    #WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/samples
    COMMENT "generate all-in-one asm file"
)

add_custom_command(
    OUTPUT "${ARTIFACTS_DIR}/all_in_one.out"
    COMMAND
        cat ${PROGRAM_SRCS_OUT_FULL} > ${ARTIFACTS_DIR}/all_in_one.out
    DEPENDS ${PROGRAM_SRCS_OUT_FULL}
    #WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/samples
    COMMENT "generate all-in-one asm result file"
)

add_custom_target(concat_all_asm_samples ALL
    DEPENDS "${ARTIFACTS_DIR}/all_in_one.s"
    COMMENT "target: all-in-one asm file"
)

add_custom_target(concat_all_asm_out_samples ALL
    DEPENDS "${ARTIFACTS_DIR}/all_in_one.out"
    COMMENT "target: all-in-one asm result file"
)

set(TEST_NAME "big")

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/big)
add_test(
    NAME ${TEST_NAME}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/big
    COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/tester_big.py
        ${ARTIFACTS_DIR}/all_in_one.s              # TEST_FILE: combined asm file
        ${ARTIFACTS_DIR}/all_in_one.out            # RESULT_FILE: out file
        $<TARGET_FILE:postrisc>                    # EXEFILE: postrisc executable
        ${CMAKE_CURRENT_BINARY_DIR}                # TEST_DIR: output dir
        ${ROM_IMAGE_PATH}                          # sample ROM file
        ${ARTIFACTS_DIR}                           # OUTPUT_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/big            # sample .s/.o files
)

#set_property(TEST ${TEST_NAME}        PROPERTY ENVIRONMENT TEST_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR})
#set_property(TEST ${TEST_NAME} APPEND PROPERTY ENVIRONMENT TEST_SAMPLES_DIR=${CMAKE_SOURCE_DIR}/tests/samples)


# PIE address statistic is not reproducible
#
#file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/elf)
#add_test(
#    NAME elf
#    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/elf
#    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tester_app.bash
#        ${CMAKE_CURRENT_SOURCE_DIR}/elf/executable.postrisc       # TEST_FILE: elf file
#        ${CMAKE_CURRENT_SOURCE_DIR}/elf/foo.out                   # RESULT_FILE: out file
#        $<TARGET_FILE:postrisc>                                   # EXEFILE
#        ${CMAKE_CURRENT_BINARY_DIR}/elf                           # OUTPUT_DIR
#        ${PATCHED_CONFIG_XML}                                     # CONFIG_XML
#)
