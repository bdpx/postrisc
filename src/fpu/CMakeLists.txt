project(fpu)

add_library( ${PROJECT_NAME} OBJECT
    README.md
    bigint.hpp
    float.cpp
    float.hpp
    float16.hpp
    float32.cpp
    float32.hpp
    float64.cpp
    float64.hpp
    float80.cpp
    float80.hpp
    float128.cpp
    float128.hpp
    float256.cpp
    float256.hpp
    float512.cpp
    float512.hpp
    float_base.hpp
    floatimpl.hpp
    u128.hpp
    u192.hpp
    u256.cpp
    u256.cpp
    u256.hpp
    u320.cpp
    u320.hpp
    u512.cpp
    u512.hpp
    u576.cpp
    u576.hpp
    u1024.cpp
    u1024.hpp
    util.hpp
)

target_include_directories( ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    util
)
