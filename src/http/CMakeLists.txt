project(http)

###################################################################
# Detect boost/beast
###################################################################

include(CheckIncludeFileCXX)

cmake_policy(SET CMP0075 NEW)
if(MINGW)
    set(CMAKE_REQUIRED_LIBRARIES "ws2_32" "wsock32")
endif()

check_include_file_cxx("boost/beast/http.hpp" HAVE_BOOST_BEAST_HTTP_H)
if(HAVE_BOOST_BEAST_HTTP_H)
    message(STATUS "boost/beast found")
else()
    message(STATUS "boost/beast not found")
endif(HAVE_BOOST_BEAST_HTTP_H)

###################################################################
# generate includable certificate files
###################################################################
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_cert.pem
    COMMAND
        ${CMAKE_COMMAND}
            -DINPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/certificate/cert.pem
            -DOUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/generated/includable_cert.pem
            -P ${CMAKE_CURRENT_SOURCE_DIR}/make_includable_certificate.cmake
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/make_includable_certificate.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/certificate/cert.pem
    COMMENT "generate includable cert header file"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_key.pem
    COMMAND
        ${CMAKE_COMMAND}
            -DINPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/certificate/key.pem
            -DOUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/generated/includable_key.pem
            -P ${CMAKE_CURRENT_SOURCE_DIR}/make_includable_certificate.cmake
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/make_includable_certificate.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/certificate/key.pem
    COMMENT "generate includable key header file"
)

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_dh.pem
    COMMAND
        ${CMAKE_COMMAND}
            -DINPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/certificate/dh.pem
            -DOUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/generated/includable_dh.pem
            -P ${CMAKE_CURRENT_SOURCE_DIR}/make_includable_certificate.cmake
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/make_includable_certificate.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/certificate/dh.pem
    COMMENT "generate includable dh header file"
)

add_custom_target(generate_includable_certificates ALL
    DEPENDS
        ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_cert.pem
        ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_key.pem
        ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_dh.pem
)

if(HAVE_BOOST_BEAST_HTTP_H)

add_executable(${PROJECT_NAME}
    detect_session.cpp
    detect_session.hpp
    main.cpp
    listener.cpp
    listener.hpp
    server_certificate.hpp
    certificate/cert.pem
    certificate/key.pem
    certificate/dh.pem
    certificate/ssl.conf
    plain_session.hpp
    ssl_session.hpp
    # generated files:
    ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_cert.pem
    ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_key.pem
    ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_dh.pem
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/src/util
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_link_libraries(${PROJECT_NAME}
    util
)

target_link_libraries(${PROJECT_NAME}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
)

if(MINGW)
    target_link_libraries(${PROJECT_NAME}
        ws2_32
        wsock32
    )
endif()

add_dependencies(${PROJECT_NAME}
    generate_includable_certificates
)

endif() # HAVE_BOOST_BEAST_HTTP_H
