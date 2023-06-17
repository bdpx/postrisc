#!/usr/bin/python3

import builtins
import os
import shutil
import subprocess
import sys
import multiprocessing
import math
import platform

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

def muslBuild(ARCH):
    if ARCH.lower() == "postrisc":
        CLANG_PATH=f"{LLVM_BUILD_DEBUG_BIN_DIR}/clang"
        ARCH="postrisc"
    else:
        CLANG_PATH=f"{NATIVE_C}"
        ARCH=platform.machine()

    os.environ['ARCH'] = ARCH
    os.environ['CLANG_PATH'] = CLANG_PATH

    CLANG_OPTIONS=[
        "-O2",
        #"-v",
        "-D_GNU_SOURCE",
        "-D_BSD_SOURCE",
        "-mllvm -inline-threshold=200",
        "-fno-unroll-loops",
        "-fno-optimize-sibling-calls",
        "-Wall",
        "-pedantic",
        "-std=c11",
        "-fomit-frame-pointer",
        "-fno-builtin",
        "-fuse-init-array",
        "-fPIC",
        #"-ffast-math", # detect and error:
        #"-fno-vectorize",
        #   "-fno-unroll-loops",
        #   "-mllvm -inline-threshold=0",
        #   "-nostartfiles -nolibc -nodefaultlibs -nostdlib
        "-fno-slp-vectorize", # superword-level parallelism
        "-disable-loop-vectorization",
        "-disable-vectorization",
        "-fno-vectorize",
        "-fno-tree-vectorize",
    ]

    if ARCH == "postrisc":
        POSTRISC_CLANG_OPTIONS=[
            "-mcpu=pv1",
            "--gcc-toolchain=${ROOT_DIR}/llvm-build/bin",
            "-mllvm", "-verify-analysis-invalidation=0",

            "-mllvm", "--enable-block-merging=1",
            "-mllvm", "--postrisc-enable-dense-calls=1",
            "-mllvm", "--enable-nullification=1",
            "-mllvm", "--enable-move-merging=1",
            "-mllvm", "--enable-pre-post-update=0",
            "-mllvm", "-debug-only=postrisc-addr-mode", # FIXME

            #"-S -emit-llvm",

            "-fvisibility-inlines-hidden",    # FIXME: shared object workaround
            #"-fvisibility=hidden",           # FIXME: shared object workaround
            "-fvisibility=protected",         # FIXME: allow internal ip-rel usage without PLT/GOT
            #"-fno-semantic-interposition",   # doom build failed with this
        ]
        CLANG_OPTIONS.extend(POSTRISC_CLANG_OPTIONS)

    LINKER_OPTIONS=[
        #    "-Bsymbolic", # failed
        #    "-fno-semantic-interposition", # failed
    ]

    # prefix for cross compiler and tools [none]
    os.environ['CROSS_COMPILE'] = ""
    # C compiler command [detected]
    os.environ['CC'] = f"{CLANG_PATH} -mcmodel=small -fPIC" #--target=postrisc -mcpu=pv1
    # C compiler flags [-Os -pipe ...]
    os.environ['CFLAGS'] = " ".join(map(str, CLANG_OPTIONS))
    # compiler runtime library [detected]
    os.environ['LIBCC'] = ""
    # Linker flags
    os.environ['LDFLAGS'] = " ".join(map(str, LINKER_OPTIONS))

    # create dir and build
    BUILD_DIR=f"..{os.sep}..{os.sep}/musl_{ARCH}"
    print(f"build: {ARCH}: {BUILD_DIR}")

    shutil.rmtree(BUILD_DIR, ignore_errors=True)
    os.makedirs(BUILD_DIR, exist_ok=True)
    os.chdir(BUILD_DIR)

    print("call configure")
    CONFIGURE_OPTIONS=[
        f"..{os.sep}musl{os.sep}configure",
        # f"--prefix=~/postrisc/musl/build/${ARCH}",
        # f"--exec-prefix=~/postrisc/musl/build/${ARCH}",
        # "--disable-shared", #inhibit building shared library
        "--with-malloc=mallocng",
        ARCH
    ]
    subprocess.run(CONFIGURE_OPTIONS, text=True, check=True)

    print("call make")
    subprocess.run(["make", f"-j{multiprocessing.cpu_count()}"], text=True, check=True)

def main():
    if len(sys.argv) != 2:
        print(f"Error: use {__file__} debug|release")
        exit(1)

    if sys.argv[1].lower() == "postrisc":
        muslBuild("postrisc")
    else:
        muslBuild(platform.machine())

if __name__ == "__main__":
    main()
