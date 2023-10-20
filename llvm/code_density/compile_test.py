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

# Oz - 5
# Os - 50
# O1 - 225
# O2 - 225
# O3 - 250

def compileSample(bitness, arch, prefix="", arch_options=[], optimization="Os"):
    BIN_DIR = f"{LLVM_BUILD_RELEASE_BIN_DIR}"
    CLANG   = f"{BIN_DIR}/clang"
    READELF = f"{BIN_DIR}/llvm-readelf"
    OBJDUMP = f"{BIN_DIR}/llvm-objdump"

    MUSL_PATH     = f"..{os.sep}..{os.sep}..{os.sep}musl"
    MUSL_LIB_PATH = f"..{os.sep}..{os.sep}..{os.sep}musl_postrisc"

    real_optimization = optimization

    # little difference with Oz
    if optimization == "Ozz":
        real_optimization = "Oz"

    OPTIONS=[
        "-c",
        f"--target={arch}",
        "--verbose",
        #"-v",
        f"-{real_optimization}",
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
        #"-fno-optimize-sibling-calls",

        #"-fomit-frame-pointer",
        #"-fno-builtin",
        #"-fuse-init-array",

        # sqlite-specific for portable build
        "-D__STRICT_ANSI__",
        f"-DSQLITE_PTRSIZE={bitness // 8}",
        "-DSQLITE_BYTEORDER=1234",
    ]

    if optimization == "Ozz":
        OPTIONS += [
            "-fno-unroll-loops",
            f"-mllvm=-inline-threshold=0"
        ]

    OPTIONS.extend(arch_options)

    LOGFILE = f"sqlite3.{arch}{prefix}.{optimization}.txt"
    OBJFILE = f"sqlite3.{arch}{prefix}.{optimization}.o"

    with open(LOGFILE, 'w') as out:
        return subprocess.Popen([CLANG] + OPTIONS + ["sqlite3.c", "-o", OBJFILE], stdout=out, stderr=out, text=True) #, check=True

    #with open(f"{OBJFILE}.relocs", "w") as outfile:
    #    subprocess.run([READELF, "-r", OBJFILE], stdout=outfile, text=True, check=True)

    #with open(f"{OBJFILE}.headers", "w") as outfile:
    #    subprocess.run([OBJDUMP, "--all-headers", "-d", OBJFILE], stdout=outfile, text=True, check=True)

class Run:
    def __init__(self, bitness, arch, prefix="", options=[]):
        self.bitness = bitness
        self.arch = arch
        self.prefix = prefix
        self.options = options

POSTRISC_OPTIONS=[
    "-mcmodel=small",
    "-mcpu=pv1",
    "-mllvm=-verify-analysis-invalidation=0",
    "-mllvm=--postrisc-enable-block-merging=1",
    "-mllvm=--postrisc-enable-move-merging=1",
    #"-mllvm=--postrisc-enable-dense-calls=1",
    #"-mllvm=--postrisc-enable-nullification=1",
    "-mllvm=--postrisc-max-nullification-blocksize=10",
    "-mllvm=--postrisc-enable-pre-post-update=1",
]

runs = [
    Run(64, "riscv64", prefix="g",  options=["-march=rv64g"]),
    Run(64, "riscv64", prefix="gc", options=["-march=rv64gc"]),
    Run(32, "riscv32", prefix="g",  options=["-march=rv32g"]),
    Run(32, "riscv32", prefix="gc", options=["-march=rv32gc"]),
    Run(32, "arm", prefix="32", options=["-march=armv8", "-mfloat-abi=hard"]),
    Run(32, "thumb", prefix="32", options=["-mfloat-abi=hard"]),
    Run(64, "arm64"),
    Run(64, "amd64"),
    Run(32, "i686"),
    Run(32, "powerpc", prefix="32"),
    Run(64, "powerpc64"),
    Run(32, "sparc", prefix="32"),
    Run(64, "sparc64"),
    Run(32, "mips", prefix="32", options=["-mfloat-abi=hard"]),
    Run(64, "mips64", options=["-mfloat-abi=hard"]),
    Run(64, "mips64", prefix="r6", options=["-march=mips64r6", "-mfloat-abi=hard"]),
    Run(32, "loongarch32"),
    Run(64, "loongarch64"),
    Run(64, "s390x", prefix="64"),
    Run(64, "postrisc", prefix="-dense",        options=POSTRISC_OPTIONS + ["-mllvm=--postrisc-enable-dense-calls=1", "-mllvm=--postrisc-enable-nullification=0"]),
    Run(64, "postrisc", prefix="-dense-null",   options=POSTRISC_OPTIONS + ["-mllvm=--postrisc-enable-dense-calls=1", "-mllvm=--postrisc-enable-nullification=1"]),
    Run(64, "postrisc", prefix="-aligned",      options=POSTRISC_OPTIONS + ["-mllvm=--postrisc-enable-dense-calls=0", "-mllvm=--postrisc-enable-nullification=0"]),
    Run(64, "postrisc", prefix="-aligned-null", options=POSTRISC_OPTIONS + ["-mllvm=--postrisc-enable-dense-calls=0", "-mllvm=--postrisc-enable-nullification=1"]),
]

def main():
    sqlite_ar = "sqlite-refs_heads_chromium-version-3.33.0-amalgamation_dev.tar.gz"

    subprocess.run(["tar", "-xf", sqlite_ar, "sqlite3.c"], text=True, check=True)
    subprocess.run(["tar", "-xf", sqlite_ar, "sqlite3.h"], text=True, check=True)

    processes = []
    optimizations = ["Oz", "Os", "O2"]

    for run in runs:
        for optimization in optimizations:
            p = compileSample(
                bitness       = run.bitness,
                arch          = run.arch,
                prefix        = run.prefix,
                optimization  = optimization,
                arch_options  = run.options)
            processes.append(p)

    for p in processes:
        p.wait()
        #f.seek(0)
        #logfile.write(f.read())
        #f.close()

    # output text+data+bss sections in decimal: size -G -d *.o
    for optimization in optimizations:
        subprocess.run(f"size -d *.{optimization}.o | sort", shell=True, text=True, check=True)

    os.remove("sqlite3.h")
    os.remove("sqlite3.c")

if __name__ == "__main__":
    main()
