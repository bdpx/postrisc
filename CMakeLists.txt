
# cmake -G generator -DCMAKE_BUILD_TYPE=buildtype -DCMAKE_CXX_COMPILER=compiler ../

#postrisc_debug: cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=DebWithAsan -DCMAKE_CXX_COMPILER=clang++-16 -DCMAKE_C_COMPILER=clang-16 ../postrisc
#postrisc_release: cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++-16 -DCMAKE_C_COMPILER=clang-16 ../postrisc

# generator: "MSYS Makefiles", "Unix Makefiles", "Ninja"
# not supported: "Visual Studio 15 2017 Win64"
# buildtype: Debug, DebWithCovInfo, Release, RelWithDebInfo, DebWithSan, Profiler
# CMAKE_CXX_COMPILER: g++, clang++, etc

cmake_minimum_required (VERSION 3.25)
project(postrisc_all) # LANGUAGES CXX

set(default_build_type "Release")
set(CMAKE_CXX_STANDARD 20)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
else()
    set(LINUX FALSE)
endif()

####################################################################################################
# verbose
####################################################################################################
set(CMAKE_VERBOSE_MAKEFILE OFF)
if(UNIX)
    message(STATUS ">>> Linux")
else()
    message(STATUS ">>> Windows MSYS")
endif()

if(WORKFLOW_CI_BUILD)
    message(STATUS "WORKFLOW_CI_BUILD detected")
else()
    message(STATUS "WORKFLOW_CI_BUILD not detected")
endif()

####################################################################################################
# place for artifacts for github-pages
####################################################################################################
set(ARTIFACTS_DIR ${CMAKE_BINARY_DIR}/artifacts)
file(MAKE_DIRECTORY ${ARTIFACTS_DIR})

####################################################################################################
# detect PkgConfig
####################################################################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckIncludeFiles)

find_package(PkgConfig REQUIRED)

####################################################################################################
# cppcheck
####################################################################################################
option(cppcheck "Run CppCheck static code analysis" Off)

if(UNIX AND cppcheck)
  find_program(cppcheck_exe NAMES cppcheck REQUIRED)
  set(cppcheck_opts --enable=all --inline-suppr --suppressions-list=${PROJECT_SOURCE_DIR}/cppcheck.supp) # --quiet
  set(CMAKE_C_CPPCHECK ${cppcheck_exe} --std=c11 ${cppcheck_opts})
  set(CMAKE_CXX_CPPCHECK ${cppcheck_exe} --std=c++20 ${cppcheck_opts})
  message(STATUS "cppcheck_exe = ${cppcheck_exe}")
endif()

####################################################################################################
# atomic lib
####################################################################################################

if(UNIX)
    set(ATOMIC_LIBRARY "atomic") # Linux GCC/CLANG requires atomic lib
else()
    set(ATOMIC_LIBRARY
        $<$<CXX_COMPILER_ID:GNU>:"atomic"> # Windows GCC requires atomic lib
        $<$<CXX_COMPILER_ID:CLANG>:>       # Windows CLANG64 doesn't have atomic lib
    )
endif()

####################################################################################################
# detect boost
####################################################################################################
set(Boost_USE_STATIC_LIBS        ON) # only find static libs
set(Boost_USE_MULTITHREAD        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
set(Boost_NO_WARN_NEW_VERSIONS   ON)

add_compile_definitions(BOOST_NO_RTTI)
add_compile_definitions(BOOST_NO_TYPEID)

find_package(Boost CONFIG REQUIRED COMPONENTS system filesystem serialization unit_test_framework)
if (Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()
add_compile_definitions(BOOST_TEST_DYN_LYNK)
message(STATUS "Boost_UNIT_TEST_FRAMEWORK_LIBRARY = ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}")

####################################################################################################
# detect backtrace
####################################################################################################
if(UNIX)
    find_package(Backtrace REQUIRED)
    message(STATUS "Backtrace_LIBRARY = ${Backtrace_LIBRARY}")
    message(STATUS "Backtrace_HEADER = ${Backtrace_HEADER}")
endif()

####################################################################################################
# detect OpenSSL
####################################################################################################
set(OPENSSL_USE_STATIC_LIBS TRUE)
find_package(OpenSSL)

####################################################################################################
# setup mpfr (currently detect isn't implemented in cmake)
####################################################################################################
add_compile_definitions(MPFR_WANT_FLOAT128)
add_compile_definitions(MPREAL_DISABLE_MOVE_SEMANTIC)

if(USE_QUADMATH)
    add_compile_definitions(USE_QUADMATH=1)
    set(FLOAT128_LIBRARIES "quadmath")
else()
    add_compile_definitions(USE_QUADMATH=0)
    set(FLOAT128_LIBRARIES "gmp;mpfr")
    if(MINGW)
        set(FLOAT128_LIBRARIES "${FLOAT128_LIBRARIES};gmp.dll")
    endif()
endif()
message(STATUS "FLOAT128_LIBRARIES = ${FLOAT128_LIBRARIES}")

####################################################################################################
# choose build types (http://stackoverflow.com/a/31548693)
####################################################################################################

set(CONFIGURATION_TYPES "Debug;DebWithCovInfo;Release;RelWithDebInfo;DebWithAsan;Profiler")

get_property(isMultiConfig GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
#message("isMultiConfig = ${isMultiConfig}")
if(isMultiConfig)
    # no reason mess with CMAKE_BUILD_TYPE if it's a multiconfig generator.
    set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION_TYPES}")
    set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION_TYPES}" CACHE STRING "" FORCE)
else()
    # No reason to set CMAKE_CONFIGURATION_TYPES if it's not a multiconfig generator
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "CMAKE_BUILD_TYPE is not set, defaulting to 'Release'")
        set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
    endif()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY HELPSTRING "Choose the type of build")
    # set the valid options for cmake-gui drop-down list
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "${CONFIGURATION_TYPES}")
endif()

string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPER)
message(STATUS "CMAKE_BUILD_TYPE_UPPER=${CMAKE_BUILD_TYPE_UPPER}")

####################################################################################################
# detect and use jumbo-build (single compilation units) for release
####################################################################################################

#if("${CMAKE_BUILD_TYPE_UPPER}" STREQUAL "RELEASE")
#    if (${CMAKE_MAJOR_VERSION} GREATER_EQUAL 3 AND ${CMAKE_MINOR_VERSION} GREATER_EQUAL 16)
#        set(CMAKE_UNITY_BUILD ON)
#        set(CMAKE_UNITY_BUILD_BATCH_SIZE 999)
#    endif()
#endif()

####################################################################################################
# set directories for output files
####################################################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for static libraries.")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    CACHE PATH "Output directory for shared libraries.")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    CACHE PATH "Output directory for executables and DLL's.")

foreach(BUILD_TYPE ${CONFIGURATION_TYPES})
    string(TOUPPER ${BUILD_TYPE} BUILD_TYPE)
    #message(STATUS "BUILD_TYPE = ${BUILD_TYPE}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${BUILD_TYPE} "${CMAKE_BINARY_DIR}/bin" CACHE PATH "" FORCE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${BUILD_TYPE} "${CMAKE_BINARY_DIR}/lib" CACHE PATH "" FORCE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${BUILD_TYPE} "${CMAKE_BINARY_DIR}/lib" CACHE PATH "" FORCE)
endforeach()

message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE = ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}")

####################################################################################################
# setup compiler sanity flags
####################################################################################################
include(CheckCXXCompilerFlag)
function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if (flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if (flag_supported)
            add_compile_options(${flag})
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()

enable_cxx_compiler_flag_if_supported(-Wrestrict) # recent gcc
if(UNIX)
    enable_cxx_compiler_flag_if_supported(-fno-semantic-interposition) # unknown on clang 9
endif()

add_compile_options(
    -Wall
    -Wcast-align
    -Wcast-qual
#   -Wconversion
    -Wdate-time
    -Wformat=2
    -Wextra
    -Winit-self
    -Winline
    -Wmissing-field-initializers
    -Wmissing-include-dirs
    -Wno-address-of-packed-member
#   -Wpadded
    -Wpedantic
    -Wpointer-arith
    -Wredundant-decls
    -Wshadow
#   -Wswitch-enum # annoing
    -Wundef
    -Wuninitialized
    -Wunreachable-code
    -Wunused-macros
    -Wwrite-strings

    -Wno-deprecated-enum-enum-conversion # C++20 a lot warnings

#    $<$<C_COMPILER_ID:GNU>:-Wbad-function-cast> # C-only
# FIXME:
#   -Wstrict-prototypes
#   -Wmissing-prototypes
#   -Wredundant-decls
#   -Wstrict-overflow=5
#   -Wnested-externs
#   -Wfloat-equal
)

add_compile_options(
#   -fstrict-overflow
#   -std=c11
#   -std=gnu++11
    -std=c++20
    -pipe
    -march=native
#   -MD
#   -MMD
#   --param large-function-growth=500
#   --param inline-unit-growth=500
)

if(UNIX)
    add_compile_options(-fno-plt)
endif()

add_compile_definitions(
    $<$<CONFIG:DEBUG>:DEBUG>
    $<$<CONFIG:DEBUG>:_DEBUG>
    $<$<CONFIG:DEBWITHASAN>:ASAN>
    $<$<CONFIG:RELEASE>:NDEBUG>
    # build-type markers
    $<$<CONFIG:DEBUG>:BUILD_TYPE_DEBUG>
    $<$<CONFIG:DEBWITHCOVINFO>:BUILD_TYPE_DEBWITHCOVINFO>
    $<$<CONFIG:RELWITHDEBINFO>:BUILD_TYPE_RELWITHDEBINFO>
    $<$<CONFIG:DEBWITHASAN>:BUILD_TYPE_DEBWITHASAN>
    $<$<CONFIG:PROFILER>:BUILD_TYPE_PROFILER>
    $<$<CONFIG:RELEASE>:BUILD_TYPE_RELEASE>

    # POSTRISC_RUNTIME_LOGS for debug builds
    $<$<CONFIG:DEBUG>:POSTRISC_RUNTIME_LOGS>
    $<$<CONFIG:DEBWITHASAN>:POSTRISC_RUNTIME_LOGS>
)

add_compile_options(
    -fvisibility-inlines-hidden   # FIXME: maybe for release only
    -fvisibility=hidden           # FIXME: maybe for release only
#    -fno-rtti # boost ptree issue
    $<$<CONFIG:DEBUG>:-g>
    $<$<CONFIG:DEBUG>:-gdwarf-4> # Clang-16 uses 5 format, workaround for valgrind
    $<$<CONFIG:DEBUG>:-O1>
    $<$<CONFIG:DEBWITHCOVINFO>:-g>
    $<$<CONFIG:DEBWITHCOVINFO>:-O1>
    $<$<CONFIG:DEBWITHCOVINFO>:--coverage>
    $<$<CONFIG:RELWITHDEBINFO>:-g>
    $<$<CONFIG:RELWITHDEBINFO>:-O2>
    $<$<CONFIG:DEBWITHASAN>:-g>
    $<$<CONFIG:DEBWITHASAN>:-fsanitize=address>
    $<$<CONFIG:DEBWITHASAN>:-fsanitize-address-use-after-scope>
    $<$<CONFIG:DEBWITHASAN>:-O2>
    $<$<CONFIG:DEBWITHASAN>:-fPIE>
    $<$<CONFIG:DEBWITHASAN>:-fno-omit-frame-pointer>
    $<$<CONFIG:DEBWITHASAN>:-fno-optimize-sibling-calls>
    $<$<CONFIG:DEBWITHASAN>:-gdwarf-4> # Clang-16 uses 5 format, workaround for valgrind
    $<$<CONFIG:PROFILER>:-pg>
    $<$<CONFIG:PROFILER>:-O2>
    $<$<CONFIG:RELEASE>:-O3>
    $<$<CONFIG:RELEASE>:-fvisibility-inlines-hidden>
    $<$<CONFIG:RELEASE>:-fvisibility=hidden>
    # avoid adding the #ident directive in each object file like "GCC (RevX, build by Y project), versionZ"
    # but this doesn't work with lto
    -fno-ident
#   $<$<CONFIG:RELEASE>:-mtune=native>
#   $<$<CONFIG:RELEASE>:-mcx16>
)

# DEBWITHASAN:
# ASAN_OPTIONS=symbolize=1
# ASAN_SYMBOLIZER_PATH=/usr/bin/llvm-symbolizer-14

add_compile_options(
    # include header with common headers everywhere
    # SHELL is to disable CMake deduplication feature
    # if several -include options will be declared
    "SHELL:-include ${CMAKE_SOURCE_DIR}/src/pch.hpp"
)

add_link_options(
    $<$<CONFIG:DEBUG>:-g>    #default debug information (level number may be added -g3 for more info)
    $<$<CONFIG:DEBWITHASAN>:-fsanitize=address>   #address-sanitizer
    # $<$<CONFIG:DEBWITHASAN>:-static-libasan>    #sanitizer
    $<$<CONFIG:PROFILER>:-pg>      #profile-guided
#    $<$<CONFIG:RELEASE>:-s>      #strip
)

if(WIN32)
    add_link_options(
        -Wl,--no-insert-timestamp    # for build reproducibility
    )
    message(STATUS "WIN32PE timestamp clearing set")
endif()

####################################################################################################
# Profiler usage: gprof .\build\postrisc\postrisc.exe gmon.out >oooo
####################################################################################################

add_compile_options(
    $<$<CXX_COMPILER_ID:GNU>:-Wno-format-truncation> # unknown in clang 9
    $<$<CXX_COMPILER_ID:GNU>:-Wlogical-op> # requires a recent clang (tested with 3.9)
    $<$<CXX_COMPILER_ID:GNU>:-Werror=sizeof-pointer-div>
    $<$<CXX_COMPILER_ID:CLANG>:-stdlib=libc++>
    $<$<CXX_COMPILER_ID:CLANG>:-Wassign-enum> # not available with gcc
    $<$<CXX_COMPILER_ID:CLANG>:-Wconditional-uninitialized> # not available with gcc
    $<$<CXX_COMPILER_ID:CLANG>:-Werror-used-but-marked-unused>
    $<$<CXX_COMPILER_ID:CLANG>:-Werror=sizeof-pointer-div>
    $<$<CXX_COMPILER_ID:MSVC>:/MP>
)

####################################################################################################
# define shorter __FILENAME__ macro
# https://public.kitware.com/pipermail/cmake/2013-January/053117.html
####################################################################################################
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS ">>> ffile-prefix-map supported")
    # here we assume that all sources are in src folder, so no need to report src and its root path
    add_compile_options(-ffile-prefix-map=${CMAKE_SOURCE_DIR}/src/=/)
    add_compile_definitions(__FILENAME__=__FILE__)
else()
    message(STATUS ">>> ffile-prefix-map NOT supported")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -D__FILENAME__='\"$(subst ${CMAKE_SOURCE_DIR}/src/,,$(abspath $<))\"'")
endif()

####################################################################################################
# From https://medium.com/@alasher/colored-c-compiler-output-with-ninja-clang-gcc-10bfe7f2b949
####################################################################################################
option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)
if (${FORCE_COLORED_OUTPUT})
    add_compile_options(
        $<$<CXX_COMPILER_ID:GNU>:-fdiagnostics-color=always>
        $<$<CXX_COMPILER_ID:CLANG>:-fcolor-diagnostics>
    )
endif()

####################################################################################################
# git version/date from last branch/commit
####################################################################################################
#for only hash:
#git describe --always --dirty --abbrev=40
execute_process(COMMAND
    git -C "${CMAKE_CURRENT_SOURCE_DIR}" rev-parse HEAD
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE GIT_PACKAGE_COMMIT_HASH)
message(STATUS "GIT_PACKAGE_COMMIT_HASH= ${GIT_PACKAGE_COMMIT_HASH}")

execute_process(COMMAND
    git -C "${CMAKE_CURRENT_SOURCE_DIR}" describe --all --long --abbrev=40
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE GIT_PACKAGE_VERSION)
message(STATUS "GIT_PACKAGE_VERSION = ${GIT_PACKAGE_VERSION}")

execute_process(COMMAND
    git -C "${CMAKE_CURRENT_SOURCE_DIR}" log -1 --format=%cd "--date=format:%Y-%m-%d %H:%M:%S"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE GIT_PACKAGE_DATE)
message(STATUS "GIT_PACKAGE_DATE = ${GIT_PACKAGE_DATE}")

execute_process(COMMAND
    git -C "${CMAKE_CURRENT_SOURCE_DIR}" log -1 --format=%cd --date=format:%Y
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE GIT_PACKAGE_DATE_YEAR)
message(STATUS "GIT_PACKAGE_DATE_YEAR = ${GIT_PACKAGE_DATE_YEAR}")

add_compile_definitions(
    PACKAGE_COMMIT_HASH="${GIT_PACKAGE_COMMIT_HASH}"
    PACKAGE_VERSION="${GIT_PACKAGE_VERSION}"
    PACKAGE_DATE="${GIT_PACKAGE_DATE}"
    PACKAGE_DATE_YEAR="${GIT_PACKAGE_DATE_YEAR}"
)

####################################################################################################
# add subprojects here
####################################################################################################
enable_testing()
add_subdirectory(src)
add_subdirectory(docs)
add_subdirectory(tests)

####################################################################################################
# Use LTO/IPO if it's supported by the compiler
# This enabled link optimization flags "-flto -fno-fat-lto-objects"
# See Cmake policy: CMP0069
####################################################################################################
#set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
include(CheckIPOSupported)
check_ipo_supported(RESULT result)
set(POSTRISC_USE_LTO FALSE)
if(result)
    if(("${CMAKE_BUILD_TYPE}" STREQUAL "Release"
        OR "${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
       # FIXME: github container has some issues with full IPO/LTO
       AND NOT WORKFLOW_CI_BUILD
       AND FALSE
       # AND "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU"
      )
        message(STATUS "Release[WithDebInfo] and G++ detected")
        set(POSTRISC_USE_LTO TRUE)
    endif()
endif()

if(${POSTRISC_USE_LTO})
    message(STATUS "Enabled LTO/IPO")
    # obsolete: set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
    # preferred: set_property(TARGET ${PROJECT_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION ${POSTRISC_USE_LTO})
    set_property(TARGET arch PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET assembler PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET binary_format PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET disassembler PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET emulator PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET fpu PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    #set_property(TARGET http PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET postrisc PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET softfp PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET util PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET webserver PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

    # qtpostrisc: may have issues with ipo due to qt 
    # set_property(TARGET qtpostrisc PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

    # assembler_test emulator_test fpu_test util_test: aren't ipo due to boost issues
else()
    message(STATUS "LTO/IPO is disabled")
endif()
