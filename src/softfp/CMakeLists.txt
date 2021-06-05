project(softfp)

add_compile_options(
    -mfpmath=sse
    -mfma
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
