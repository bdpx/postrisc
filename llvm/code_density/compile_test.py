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

def compileSample(arch, arch_options, optimization, dense_calls=1, nullification=1, prefix=""):
    CLANG   = f"{LLVM_BUILD_RELEASE_BIN_DIR}/clang"
    READELF = f"{LLVM_BUILD_RELEASE_BIN_DIR}/llvm-readelf"
    OBJDUMP = f"{LLVM_BUILD_RELEASE_BIN_DIR}/llvm-objdump"

    MUSL_PATH     = f"..{os.sep}..{os.sep}..{os.sep}musl"
    MUSL_LIB_PATH = f"..{os.sep}..{os.sep}..{os.sep}musl_postrisc"

    OPTIONS=[
        "-c",
        f"--target={arch}",
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
        "-nodefaultlibs",
        "-nostdlib",
        "-fvisibility-inlines-hidden",   # FIXME: shared object workaround
        "-fvisibility=hidden",           # FIXME: maybe for release only
        #"-fvisibility=protected",         # FIXME: allow internal ip-rel usage without PLT/GOT
        #"-fno-semantic-interposition",   # doom build failed with this

        "-fno-slp-vectorize",  # superword-level parallelism
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

    OPTIONS.extend(arch_options)

    if arch == "postrisc":
        POSTRISC_OPTIONS=[
            "-mcmodel=small",
            "-mcpu=pv1",
            "-mllvm", "-verify-analysis-invalidation=0",
            "-mllvm", "--enable-block-merging=1",
            "-mllvm", f"--postrisc-enable-dense-calls={dense_calls}",
            "-mllvm", f"--enable-nullification={nullification}",
            "-mllvm", "--enable-move-merging=1",
        ]
        OPTIONS.extend(POSTRISC_OPTIONS)

    OBJFILE = f"sqlite3.{arch}{prefix}.{optimization}.o"

    return subprocess.Popen([CLANG] + OPTIONS + ["sqlite3.c", "-o", OBJFILE], text=True) #, check=True

    #with open(f"{OBJFILE}.relocs", "w") as outfile:
    #    subprocess.run([READELF, "-r", OBJFILE], stdout=outfile, text=True, check=True)

    #with open(f"{OBJFILE}.headers", "w") as outfile:
    #    subprocess.run([OBJDUMP, "--all-headers", "-d", OBJFILE], stdout=outfile, text=True, check=True)

class Run:
    def __init__(self, arch, prefix="", options=[], dense_calls=0, nullification=0):
        self.arch = arch
        self.prefix = prefix
        self.options = options
        self.dense_calls = dense_calls
        self.nullification = nullification

runs = [
    Run("arm", prefix="32", options=["-march=armv8"]),
    Run("thumb", prefix="32"),
    Run("arm64"),
    Run("powerpc", prefix="32"),
    Run("amd64"),
    Run("i686"),
    Run("riscv64", prefix="g",  options=["-march=rv64g"]),
    Run("riscv64", prefix="gc", options=["-march=rv64gc"]),
    Run("riscv32", prefix="g",  options=["-march=rv32g"]),
    Run("riscv32", prefix="gc", options=["-march=rv32gc"]),
    Run("powerpc64"),
    Run("sparc", prefix="32"),
    Run("sparc64"),
    Run("mips", prefix="32"),
    Run("mips64", prefix="r6", options=["-march=mips64r6"]),
    Run("loongarch32"),
    Run("loongarch64"),
    Run("s390x", prefix="64"),
    Run("postrisc", prefix="-dense",        dense_calls=1, nullification=0),
    Run("postrisc", prefix="-dense-null",   dense_calls=1, nullification=1),
    Run("postrisc", prefix="-aligned",      dense_calls=0, nullification=0),
    Run("postrisc", prefix="-aligned-null", dense_calls=0, nullification=1),
]

def main():
    sqlite_ar = "sqlite-refs_heads_chromium-version-3.33.0-amalgamation_dev.tar.gz"

    subprocess.run(["tar", "-xf", sqlite_ar, "sqlite3.c"], text=True, check=True)
    subprocess.run(["tar", "-xf", sqlite_ar, "sqlite3.h"], text=True, check=True)

    processes = []

    for run in runs:
        for optimization in ["O2", "Os"]:
            p = compileSample(
                arch=run.arch,
                arch_options=run.options,
                optimization=optimization,
                prefix=run.prefix,
                dense_calls=run.dense_calls,
                nullification=run.nullification)
            processes.append(p)

    for p in processes:
        p.wait()
        #f.seek(0)
        #logfile.write(f.read())
        #f.close()

    # output text+data+bss sections in decimal: size -G -d *.o
    subprocess.run("size -d *.o | sort", shell=True, text=True, check=True)

    os.remove("sqlite3.h")
    os.remove("sqlite3.c")

if __name__ == "__main__":
    main()
