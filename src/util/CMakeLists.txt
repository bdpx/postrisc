project(util)

add_library( ${PROJECT_NAME} OBJECT
    common.hpp
    endianness.hpp
    fmt.cpp
    fmt.hpp
    getch.cpp
    logger.cpp
    logger.hpp
    mpreal.cpp
    mpreal.hpp
    quadruple.cpp
    quadruple.hpp
    setup.cpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    ${CMAKE_DL_LIBS}
    ${FLOAT128_LIBRARIES}
    ${Backtrace_LIBRARY}
    backtrace
)
