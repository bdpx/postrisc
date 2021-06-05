project(softfp_timing)

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
    timing.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    util
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
)

#add_test(
#    NAME ${PROJECT_NAME}
#    COMMAND ${PROJECT_NAME}
#)
