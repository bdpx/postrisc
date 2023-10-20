#!/usr/bin/python3

import builtins
import os
import shutil
import subprocess
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

# prereq: libedit-dev zlib1g-dev libxml2-dev

# https://llvm.org/docs/CMake.html

#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -g -s") # -Wa,-mbig-obj
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O1 -g -s") # -Wa,-mbig-obj
# set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}") #-mbig-obj
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -s" )
#set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -g -s" )
#set(LLVM_OPTIMIZED_TABLEGEN ON)

# LLVM_FORCE_ENABLE_STATS

def cmakeGenerate(buildType, buildDir):
    print(f"generate: {buildType}, {buildDir}")
    BUILD_DIR=f"..{os.sep}..{os.sep}{buildDir}" # build out of tree

    shutil.rmtree(BUILD_DIR, ignore_errors=True)
    os.makedirs(BUILD_DIR, exist_ok=True)

    DebugFlagOn = "Off" if buildType == "Release" else "On"
    DebugLevel = "g0" if buildType == "Release" else "g1"
    Optimization = "O2" if buildType == "Release" else "O1"
    ARGS=[
        "cmake",
        "-G", f"{CMAKE_GENERATOR}",
        f"-DCMAKE_BUILD_TYPE={buildType}",
        f"-DCMAKE_C_COMPILER={NATIVE_C}",
        f"-DCMAKE_CXX_COMPILER={NATIVE_CPP}",

        # compiler: -g3 -ggdb -gsplit-dwarf -O2
        # linker: -Wl,--gdb-index
        # -g0 no debug information, -g1 minimal debug information, -g default debug information, -g3 maximal debug information
        # __POSTRISC__ is for postrisc-related fixes in common code
        # Debug - g1
        f"-DCMAKE_CXX_FLAGS=-{Optimization} -{DebugLevel} -D__POSTRISC__",
        f"-DCMAKE_EXE_LINKER_FLAGS=-{DebugLevel}",
        f"-DCMAKE_SHARED_LINKER_FLAGS=-{DebugLevel}",

        # LLVM_UNREACHABLE_OPTIMIZE
        # This flag controls the behavior of llvm_unreachable() in release build (when assertions are disabled in general).
        # When ON (default) then llvm_unreachable() is considered 'undefined behavior' and optimized as such.
        # When OFF it is instead replaced with a guaranteed 'trap'.
        "-DLLVM_UNREACHABLE_OPTIMIZE=Off",

        # enable all asserts
        f"-DLLVM_ENABLE_ASSERTIONS={DebugFlagOn}",

        # Enable additional time/memory expensive checking. Defaults to OFF.
        f"-DLLVM_ENABLE_EXPENSIVE_CHECKS={DebugFlagOn}",

        # don't need plugins currently
        "-DLLVM_ENABLE_PLUGINS=Off",
        "-DCLANG_PLUGIN_SUPPORT=Off",

        # Flag indicating if each LLVM component (e.g. Support) is built as a shared library (ON) or as a static library (OFF).
        # Its default value is OFF. On Windows, shared libraries may be used when building with MinGW, including mingw-w64,
        # but not when building with the Microsoft toolchain.
        # BUILD_SHARED_LIBS is only recommended for use by LLVM developers.
        # If you want to build LLVM as a shared library, you should use the LLVM_BUILD_LLVM_DYLIB option.
        # Postrisc: On, to reduce build size
        "-DBUILD_SHARED_LIBS=On", # Off?
        #"-DLLVM_BUILD_SHARED_LIBS=On", # Off?

        # strange errors when compile with shared libs
        "-DLLVM_BUILD_INSTRUMENTED=Off",

        # If enabled, the target for building the libLLVM shared library is added.
        # This library contains all of LLVM components in a single shared library.
        # Defaults to OFF.
        # This cannot be used in conjunction with BUILD_SHARED_LIBS.
        # Tools will only be linked to the libLLVM shared library if LLVM_LINK_LLVM_DYLIB is also ON.
        # The components in the library can be customised by setting LLVM_DYLIB_COMPONENTS
        # to a list of the desired components.
        # This option is not available on Windows.
        # -DLLVM_BUILD_LLVM_DYLIB:Off

        # more ready for usage in C++
        "-DLLVM_ENABLE_RTTI=On",
        "-DLLVM_ENABLE_EH=On",

        # LLVM_OPTIMIZED_TABLEGEN failed with Off
        # /llvm/include/llvm/Target/TargetSelectionDAG.td:972:5: error: Type set is empty for each HW mode in 'vtInt'
        # def vtInt      : PatLeaf<(vt),  [{ return N->getVT().isInteger(); }]>;
        "-DLLVM_OPTIMIZED_TABLEGEN=On",

        # Embed version control revision info (Git revision id).
        # The version info is provided by the LLVM_REVISION macro in llvm/include/llvm/Support/VCSRevision.h.
        # Developers using git who don't need revision info can disable this option
        # to avoid re-linking most binaries after a branch switch.
        # Defaults to ON.
        # Don't embed version control revision info (Git revision id) into each build and result asm files
        # Postrisc: Off to minimize rebuilds
        "-DLLVM_APPEND_VC_REV=Off",

        # to avoid OOMs or going into swap, permit only one link job per 15GB of RAM available on a 32GB machine
        # -DLLVM_PARALLEL_COMPILE_JOBS=8
        # -DLLVM_PARALLEL_LINK_JOBS=4

        # LLVM_ENABLE_PROJECTS: cross-project-tests, all
        # clang;clang-tools-extra;compiler-rt;debuginfo-tests;libc;libclc;libcxx;
        # libcxxabi;libunwind;lld;lldb;openmp;parallel-libs;polly;pstl; cross-project-tests?
        "-DLLVM_ENABLE_PROJECTS=clang;clang-tools-extra;lld;lldb",

        # Existing target to compare with Postrisc:
        # -DLLVM_TARGETS_TO_BUILD="X86;PowerPC;Sparc"
        # -DLLVM_TARGETS_TO_BUILD=HOST
        # or empty string as value
        "-DLLVM_TARGETS_TO_BUILD=AArch64;ARM;LoongArch;Mips;PowerPC;RISCV;Sparc;SystemZ;X86",

        # LLVM_EXPERIMENTAL_TARGETS_TO_BUILD:STRING
        # Semicolon-separated list of experimental targets to build and linked into llvm.
        # This will build the experimental target without needing it to add to the list
        # of all the targets available in the LLVM main CMakeLists.txt.
        "-DLLVM_EXPERIMENTAL_TARGETS_TO_BUILD=Postrisc",

        "-DLLVM_TARGET_ARCH=Postrisc",

        # https://clang.llvm.org/docs/CrossCompilation.html#general-cross-compilation-options-in-clang
        # <arch><subarch>-<vendor>-<os>-<abi>
        # arch = x86_64, i386, arm, thumb, mips, etc.
        # subarch = for ex. on ARM: v5, v6m, v7a, v7m, etc.
        # vendor = pc, apple, nvidia, ibm, etc.
        # os(sys) = none, linux, win32, darwin, cuda, etc.
        # abi(env) = eabi, gnu, android, macho, elf, etc.
        "-DLLVM_DEFAULT_TARGET_TRIPLE=postrisc-pc-linux-gnu",

        # "-DLLVM_VERSION_SUFFIX=-postrisc", # default is "git"
        # don't install
        # -DCMAKE_INSTALL_PREFIX=../install

        "-B", BUILD_DIR,
        "-S", f"..{os.sep}..{os.sep}llvm-project{os.sep}llvm"
    ]

    subprocess.run(ARGS, capture_output=False, text=True, check=True)
    print("finished")

def main():
    if len(sys.argv) != 2:
        print(f"Error: use {__file__} debug|release")
        exit(1)

    if sys.argv[1].lower() == "release":
        cmakeGenerate("Release", "llvm-build-release")
    else:
        cmakeGenerate("Debug", "llvm-build-debug")

if __name__ == "__main__":
    main()
