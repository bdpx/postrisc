project(src)

include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(arch)
add_subdirectory(assembler)
add_subdirectory(binary_format)
add_subdirectory(bin_generator)
add_subdirectory(config)
add_subdirectory(disassembler)
add_subdirectory(emulator)
add_subdirectory(emulator_app)
add_subdirectory(emulator_system)
add_subdirectory(fpu)
add_subdirectory(llvm)
add_subdirectory(llvm_codegen)
add_subdirectory(util)
add_subdirectory(postrisc)
add_subdirectory(qt)
add_subdirectory(webserver)
add_subdirectory(html)
add_subdirectory(http)

add_subdirectory(assembler_test)
add_subdirectory(emulator_test)
add_subdirectory(fpu_test)
add_subdirectory(util_test)

add_subdirectory(softfp)
add_subdirectory(softfp_test)
add_subdirectory(softfp_timing)

# DOXYGEN support

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]eleaseX")
    # first we can indicate the documentation build as an option and set it to ON by default
    option(BUILD_DOC "Build documentation" ON)

    # check if Doxygen is installed
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/../docs/Doxyfile.in)
        set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target( doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
endif()
