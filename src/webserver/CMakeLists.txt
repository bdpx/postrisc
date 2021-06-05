project(webserver)

add_executable(${PROJECT_NAME}
    connection.cpp
    connection.hpp
    header.hpp
    main.cpp
    reply.cpp
    reply.hpp
    request.hpp
    request_parser.cpp
    request_parser.hpp
    server.cpp
    server.hpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/util/
)

target_link_libraries(${PROJECT_NAME}
    util
    pthread
    ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

if(MINGW)
    target_link_libraries(${PROJECT_NAME} ws2_32 wsock32)
endif()
