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

function(make_includable source_name)
    set(bash_script ${CMAKE_SOURCE_DIR}/src/make_includable.bash)
    set(src_file ${CMAKE_CURRENT_SOURCE_DIR}/certificate/${source_name})
    set(dst_file ${CMAKE_CURRENT_BINARY_DIR}/generated/includable_${source_name})
    add_custom_command(
        OUTPUT ${dst_file}
        COMMAND bash ${bash_script} ${src_file} ${dst_file}
        DEPENDS ${bash_script} ${src_file}
        COMMENT "generate includable ${source_name} stringified file"
    )
endfunction()

make_includable(cert.pem)
make_includable(key.pem)
make_includable(dh.pem)

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

endif() # HAVE_BOOST_BEAST_HTTP_H
