project(softfp_test)

add_compile_options(
    -mfpmath=sse
    -msse2
    -mfma
    #-march=native
    #-mavx512f
    #-mavx512fp16
    -fexcess-precision=16
    $<$<CXX_COMPILER_ID:CLANG>:-ffp-model=strict>
    $<$<CXX_COMPILER_ID:GNU>:-frounding-math>
    $<$<CXX_COMPILER_ID:GNU>:-fsignaling-nans>
    $<$<CXX_COMPILER_ID:GNU>:-fno-fast-math>
)

add_executable(${PROJECT_NAME}
    ../boost_test_main.cpp
    HardFPU.cpp
    HardFPU.hpp

    Sequencer.cpp
    Sequencer.hpp

    Controller.cpp
    Controller.hpp

    test_binary.cpp
    test_unary.cpp
    test_compare.cpp
    test_cvt_i_f.cpp
    test_cvt_f_i.cpp
    test_fma.cpp

    ValuePrinter.cpp
    ValuePrinter.hpp
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

