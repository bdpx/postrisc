#!/usr/bin/env bash

#exec > out_generate.tmp

# shellcheck source=/dev/null
. ../utils/common.bash

# https://llvm.org/docs/CMake.html

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -g -s") # -Wa,-mbig-obj
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1 -g -s") # -Wa,-mbig-obj
# set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}") #-mbig-obj
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -s" )
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -s" )
#set(LLVM_OPTIMIZED_TABLEGEN ON)

# LLVM_FORCE_ENABLE_STATS

GENERATOR="Unix Makefiles"
echo "GENERATOR=${GENERATOR}"

mkdir -p ${LLVM_BUILD_DIR}
echo "LLVM_BUILD_DIR=${LLVM_BUILD_DIR}"

declare -r CMAKE_ARGS=(
    -G "${GENERATOR}"

    # Debug, RelWithDebInfo, Release
    -DCMAKE_BUILD_TYPE=Debug

    -DCMAKE_CXX_COMPILER=clang++
    -DCMAKE_C_COMPILER=clang

    # compiler: -g3 -ggdb -gsplit-dwarf -O2
    # linker: -Wl,--gdb-index
    # -g0 no debug information, -g1 minimal debug information, -g default debug information, -g3 maximal debug information
    # __POSTRISC__ is for postrisc-related fixes in common code
    -DCMAKE_CXX_FLAGS="-O2 -g1 -D__POSTRISC__"
    -DCMAKE_EXE_LINKER_FLAGS="-g1"
    -DCMAKE_SHARED_LINKER_FLAGS="-g1"

    # LLVM_UNREACHABLE_OPTIMIZE
    # This flag controls the behavior of llvm_unreachable() in release build (when assertions are disabled in general).
    # When ON (default) then llvm_unreachable() is considered 'undefined behavior' and optimized as such.
    # When OFF it is instead replaced with a guaranteed 'trap'.
    -DLLVM_UNREACHABLE_OPTIMIZE=Off

    # enable all asserts
    -DLLVM_ENABLE_ASSERTIONS=On

    # Enable additional time/memory expensive checking. Defaults to OFF.
    -DLLVM_ENABLE_EXPENSIVE_CHECKS=On

    # don't need plugins currently
    -DLLVM_ENABLE_PLUGINS=Off
    -DCLANG_PLUGIN_SUPPORT=Off

    # Flag indicating if each LLVM component (e.g. Support) is built as a shared library (ON) or as a static library (OFF).
    # Its default value is OFF. On Windows, shared libraries may be used when building with MinGW, including mingw-w64,
    # but not when building with the Microsoft toolchain.
    # BUILD_SHARED_LIBS is only recommended for use by LLVM developers.
    # If you want to build LLVM as a shared library, you should use the LLVM_BUILD_LLVM_DYLIB option.
    # Postrisc: On, to reduce build size
    -DBUILD_SHARED_LIBS=On

    # If enabled, the target for building the libLLVM shared library is added.
    # This library contains all of LLVM’s components in a single shared library.
    # Defaults to OFF.
    # This cannot be used in conjunction with BUILD_SHARED_LIBS.
    # Tools will only be linked to the libLLVM shared library if LLVM_LINK_LLVM_DYLIB is also ON.
    # The components in the library can be customised by setting LLVM_DYLIB_COMPONENTS
    # to a list of the desired components.
    # This option is not available on Windows.
    # -DLLVM_BUILD_LLVM_DYLIB:Off

    # more ready for usage in C++
    -DLLVM_ENABLE_RTTI=On
    -DLLVM_ENABLE_EH=On

    # LLVM_OPTIMIZED_TABLEGEN failed with Off
    # /llvm/include/llvm/Target/TargetSelectionDAG.td:972:5: error: Type set is empty for each HW mode in 'vtInt'
    # def vtInt      : PatLeaf<(vt),  [{ return N->getVT().isInteger(); }]>;
    -DLLVM_OPTIMIZED_TABLEGEN=On

    # Embed version control revision info (Git revision id).
    # The version info is provided by the LLVM_REVISION macro in llvm/include/llvm/Support/VCSRevision.h.
    # Developers using git who don't need revision info can disable this option
    # to avoid re-linking most binaries after a branch switch.
    # Defaults to ON.
    # Don't embed version control revision info (Git revision id) into each build and result asm files
    # Postrisc: Off to minimize rebuilds
    -DLLVM_APPEND_VC_REV=Off

    # seems obsolete
    -DCLANG_TOOLING_BUILD_AST_INTROSPECTION=Off

    # to avoid OOMs or going into swap, permit only one link job per 15GB of RAM available on a 32GB machine
    # -DLLVM_PARALLEL_COMPILE_JOBS=8
    # -DLLVM_PARALLEL_LINK_JOBS=4

    # LLVM_ENABLE_PROJECTS: cross-project-tests, all
    # clang;clang-tools-extra;compiler-rt;debuginfo-tests;libc;libclc;libcxx;
    # libcxxabi;libunwind;lld;lldb;openmp;parallel-libs;polly;pstl; cross-project-tests?
    -DLLVM_ENABLE_PROJECTS="clang;lld"

    # Existing target to compare with Postrisc:
    # -DLLVM_TARGETS_TO_BUILD="X86;PowerPC;Sparc"
    # -DLLVM_TARGETS_TO_BUILD=HOST
    -DLLVM_TARGETS_TO_BUILD=""

    # LLVM_EXPERIMENTAL_TARGETS_TO_BUILD:STRING
    # Semicolon-separated list of experimental targets to build and linked into llvm.
    # This will build the experimental target without needing it to add to the list
    # of all the targets available in the LLVM main CMakeLists.txt.
    -DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD="Postrisc"

    -DLLVM_TARGET_ARCH="Postrisc"

    # https://clang.llvm.org/docs/CrossCompilation.html#general-cross-compilation-options-in-clang
    # <arch><subarch>-<vendor>-<os>-<abi>
    # arch = x86_64, i386, arm, thumb, mips, etc.
    # sub = for ex. on ARM: v5, v6m, v7a, v7m, etc.
    # vendor = pc, apple, nvidia, ibm, etc.
    # sys = none, linux, win32, darwin, cuda, etc.
    # env = eabi, gnu, android, macho, elf, etc.
    -DLLVM_DEFAULT_TARGET_TRIPLE="postrisc-pc-linux-gnu"

    # don't install
    # -DCMAKE_INSTALL_PREFIX=../install

    -B ${LLVM_BUILD_DIR}

    -S ${LLVM_CMAKE_ROOT_DIR}
)

echo "###########################################################################"
echo "CMAKE_ARGS = ${CMAKE_ARGS[@]}"
for i in "${CMAKE_ARGS[@]}"
do
    printf "   %s\n" "${i}"
done
echo "###########################################################################"

cmake "${CMAKE_ARGS[@]}"

echo "###########################################################################"
echo "finish"
echo "###########################################################################"
