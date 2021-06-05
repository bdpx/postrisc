project(softfp_test)

add_compile_options(
    -mfpmath=sse
    -mfma
)

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
    controller.cpp
    controller.hpp
    hardfloat.cpp
    hardfloat.hpp
    sequencer.hpp
    testing.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    util
)

target_link_libraries(${PROJECT_NAME}
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${FLOAT128_LIBRARIES}
)

#add_test(
#    NAME ${PROJECT_NAME}
#    COMMAND ${PROJECT_NAME}
#)

