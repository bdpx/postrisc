#!/usr/bin/python3

import builtins
import math
import multiprocessing
import os
import shutil
import subprocess
import sys
import platform

sys.path.append(os.path.dirname(
                os.path.dirname(
                os.path.dirname(
                os.path.abspath(__file__)))))
import common
from common import *

def compileSample(arch, optimization, dense_calls=1, nullification=1, prefix=""):
    if arch.lower() == "postrisc":
        arch = "postrisc"
        CLANG   = f"{LLVM_BUILD_RELEASE_BIN_DIR}/clang"
        READELF = f"{LLVM_BUILD_RELEASE_BIN_DIR}/llvm-readelf"
        OBJDUMP = f"{LLVM_BUILD_RELEASE_BIN_DIR}/llvm-objdump"
    else:
        arch = platform.machine()
        CLANG   = f"{NATIVE_C}"
        READELF = "readelf"
        OBJDUMP = "objdump"

    MUSL_PATH     = f"..{os.sep}..{os.sep}..{os.sep}musl"
    MUSL_LIB_PATH = f"..{os.sep}..{os.sep}..{os.sep}musl_{arch}"

    OPTIONS=[
        "-c",
        "-mcmodel=small",
        "--verbose",
        #"-v",
        f"-{optimization}",
        "-fPIE",
        "-fPIC",
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
        "-fvisibility-inlines-hidden",   # FIXME: shared object workaround
        "-fvisibility=hidden",           # FIXME: maybe for release only
        #"-fvisibility=protected",         # FIXME: allow internal ip-rel usage without PLT/GOT
        #"-fno-semantic-interposition",   # doom build failed with this

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
        "-fno-unroll-loops",
        "-fno-optimize-sibling-calls",

        "-mllvm", "-inline-threshold=100",

        #"-fomit-frame-pointer",
        #"-fno-builtin",
        #"-fuse-init-array",

        # sqlite-specific for portable build
        "-D__STRICT_ANSI__",
        "-DSQLITE_PTRSIZE=8",
        "-DSQLITE_BYTEORDER=1234",
    ]

    if arch == "postrisc":
        POSTRISC_OPTIONS=[
            "-mcpu=pv1",
            "-mllvm", "-verify-analysis-invalidation=0",
            "-mllvm", "--enable-block-merging=1",
            "-mllvm", f"--postrisc-enable-dense-calls={dense_calls}",
            "-mllvm", f"--enable-nullification={nullification}",
            "-mllvm", "--enable-move-merging=1",
        ]
        OPTIONS.extend(POSTRISC_OPTIONS)

    OBJFILE = f"sqlite3.{arch}.{optimization}{prefix}.o"

    subprocess.run([CLANG] + OPTIONS + ["sqlite3.c", "-o", OBJFILE], text=True, check=True)

    with open(f"{OBJFILE}.relocs", "w") as outfile:
        subprocess.run([READELF, "-r", OBJFILE], stdout=outfile, text=True, check=True)

    with open(f"{OBJFILE}.headers", "w") as outfile:
        subprocess.run([OBJDUMP, "--all-headers", "-d", OBJFILE], stdout=outfile, text=True, check=True)

def main():
    compileSample(arch="postrisc", optimization="O2", dense_calls=1, nullification=1, prefix="-dense-calls-null")
    compileSample(arch="postrisc", optimization="O2", dense_calls=1, nullification=0, prefix="-dense-calls")
    compileSample(arch="postrisc", optimization="O2", dense_calls=0, nullification=1, prefix="-aligned-calls-null")
    compileSample(arch="postrisc", optimization="O2", dense_calls=0, nullification=0, prefix="-aligned-calls")

    compileSample(arch="postrisc", optimization="Os", dense_calls=1, nullification=1, prefix="-dense-calls-null")
    compileSample(arch="postrisc", optimization="Os", dense_calls=1, nullification=0, prefix="-dense-calls")
    compileSample(arch="postrisc", optimization="Os", dense_calls=0, nullification=1, prefix="-aligned-calls-null")
    compileSample(arch="postrisc", optimization="Os", dense_calls=0, nullification=0, prefix="-aligned-calls")

    compileSample(arch=platform.machine(), optimization="O2")
    compileSample(arch=platform.machine(), optimization="Os")

    # output text+data+bss sections in decimal: size -G -d *.o
    subprocess.run(["size", "-G", "-d", "*.o"], text=True, check=True)

if __name__ == "__main__":
    main()
