project(softfp)

if("${CMAKE_CXX_COMPILER_ARCHITECTURE_ID}" STREQUAL "x86_64")
    message(STATUS "Use x86_64 options for ${PROJECT_NAME}")
    add_compile_options(
        -mfpmath=sse
        -mfma
    )
endif()

add_compile_options(
#    $<$<CXX_COMPILER_ID:GNU>:-fno-fast-math>
#    $<$<CXX_COMPILER_ID:GNU>:-frounding-math>
#    $<$<CXX_COMPILER_ID:GNU>:-ftrapping-math>
#   $<$<CXX_COMPILER_ID:GNU>:-ffp-contract=off>
#    $<$<CXX_COMPILER_ID:CLANG>:-ffp-model=strict>
)

add_executable(${PROJECT_NAME}
    bigint.hpp
    multiprecision.hpp
    scalar_uint.hpp
    softfptest.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    util
    arch
)

target_link_libraries(${PROJECT_NAME}
    ${FLOAT128_LIBRARIES}
)
