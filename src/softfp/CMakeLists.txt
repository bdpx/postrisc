project(softfp)

add_compile_options(
#    -mfpmath=sse
    -mfma
)

add_executable(${PROJECT_NAME}
    bigint.hpp
    multiprecision.hpp
    scalar_uint.hpp
    softfp.hpp
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
    ${MPFR_LIBRARIES}
)
