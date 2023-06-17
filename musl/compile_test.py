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

def muslBuildApp(ARCH):
    if ARCH.lower() == "postrisc":
        ARCH="postrisc"
        CLANG_PATH=f"{LLVM_BUILD_DEBUG_BIN_DIR}/clang"
        READELF=f"{LLVM_BUILD_DEBUG_BIN_DIR}/llvm-readelf"
        OBJDUMP=f"{LLVM_BUILD_DEBUG_BIN_DIR}/llvm-objdump"
    else:
        ARCH=platform.machine()
        CLANG_PATH=f"{NATIVE_C}"
        READELF="readelf"
        OBJDUMP="objdump"

    os.environ['ARCH'] = ARCH
    os.environ['CLANG_PATH'] = CLANG_PATH

    MUSL_PATH     = f"..{os.sep}..{os.sep}musl"
    MUSL_LIB_PATH = f"..{os.sep}..{os.sep}musl_{ARCH}"

    OPTIONS=[
        "-mcmodel=small",
        "--verbose",
        #"-v",
        "-Os",
        "-fPIE",
        "-Wall",
        "-pedantic",
        "-std=c11",
        "-isystem", f"{MUSL_PATH}/include",
        "-isystem", f"{MUSL_PATH}/arch/postrisc",
        "-isystem", f"{MUSL_PATH}/arch/generic",
        "-isystem", f"{MUSL_LIB_PATH}/obj/include",
        "-fno-builtin",
        "-nostdinc",
        "-nostartfiles",
        "-nolibc",
        "-nodefaultlibs",
        "-nostdlib",
        "-fvisibility-inlines-hidden",   # FIXME: maybe for release only
        "-fvisibility=hidden",           # FIXME: maybe for release only
        # "-Wl,-nolibc",
        "-Wl,-nostdlib",
        # "-Wl,-nodefaultlibs",
        # "-Wl,--nostartfiles",
        "-Wl,-static",
        "-fno-slp-vectorize",  # superword-level parallelism
        "-disable-loop-vectorization",
        "-disable-vectorization",
        "-fno-vectorize",
        "-fno-tree-vectorize",

        #"-mllvm -inline-threshold=200",
        #"-fno-unroll-loops",
        #"-fno-optimize-sibling-calls",
        #"-fomit-frame-pointer",
        #"-fno-builtin",
        #"-fuse-init-array",
    ]

    if ARCH == "postrisc":
        POSTRISC_OPTIONS=[
            "-mcpu=pv1",
            "--gcc-toolchain={ROOT_DIR}/llvm-build/bin",
            "-mllvm", "-verify-analysis-invalidation=0",

            "-mllvm", "-inline-threshold=100",
            "-fno-unroll-loops",
            "-fno-optimize-sibling-calls",

            "-mllvm", "--enable-block-merging=1",
            "-mllvm", "--postrisc-enable-dense-calls=1",
            "-mllvm", "--enable-nullification=1",
            "-mllvm", "--enable-move-merging=1",
            "-mllvm", "--enable-pre-post-update=0", # FIXME

            #"-S -emit-llvm",

            "-fvisibility-inlines-hidden",    # FIXME: shared object workaround
            #"-fvisibility=hidden",           # FIXME: shared object workaround
            "-fvisibility=protected",         # FIXME: allow internal ip-rel usage without PLT/GOT
            #"-fno-semantic-interposition",   # doom build failed with this
        ]
        OPTIONS.extend(POSTRISC_OPTIONS)


    CC = f"{CLANG_PATH}"
    MUSL_PATH = f"{ROOT_DIR}/musl"
    MUSL_LIB_PATH = f"..{os.sep}..{os.sep}musl_{ARCH}"

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

    CC_OPTIONS=[CC, "-c",]
    CC_OPTIONS.extend(OPTIONS)
    CC_OPTIONS.extend(["test.c", "-o", f"test.{ARCH}.o"])
    subprocess.run(CC_OPTIONS, text=True, check=True)
    with open(f"test.{ARCH}.o.relocs", "w") as outfile:
        subprocess.run([READELF, "-r", f"test.{ARCH}.o"], stdout=outfile, text=True, check=True)

    CC_OPTIONS=[CC, "-c",]
    CC_OPTIONS.extend(OPTIONS)
    CC_OPTIONS.extend(["test_2.c", "-o", f"test_2.{ARCH}.o"])
    subprocess.run(CC_OPTIONS, text=True, check=True)
    with open(f"test_2.{ARCH}.o.relocs", "w") as outfile:
        subprocess.run([READELF, "-r", f"test_2.{ARCH}.o"], stdout=outfile, text=True, check=True)

    CC_OPTIONS=[CC]
    CC_OPTIONS.extend(OPTIONS)
    CC_OPTIONS.extend(["test.c", "test_2.c", f"{MUSL_LIB_PATH}/lib/Scrt1.o", f"-L{MUSL_LIB_PATH}/lib", "-lc", "-o", f"executable.{ARCH}"])
    subprocess.run(CC_OPTIONS, text=True, check=True)

    # linker is one way, so crt after test, lib after crt
    with open(f"executable.{ARCH}.relocs", "w") as outfile:
        subprocess.run([READELF, "-r", f"executable.{ARCH}"], stdout=outfile, text=True, check=True)

    with open(f"executable.{ARCH}.headers", "w") as outfile:
        subprocess.run([OBJDUMP, "--all-headers", "-d", f"executable.{ARCH}"], stdout=outfile, text=True, check=True)

def main():
    if len(sys.argv) != 2:
        print(f"Error: use {__file__} debug|release")
        exit(1)

    if sys.argv[1].lower() == "postrisc":
        muslBuildApp("postrisc")
    else:
        muslBuildApp(platform.machine())

if __name__ == "__main__":
    main()
