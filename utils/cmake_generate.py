#!/usr/bin/python3

import builtins
import os
import shutil
import subprocess
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

def cmakeGenerate(buildType, buildDir, CC, CPPC):
    print(f"generate: {buildType}, {buildDir}")
    BUILD_DIR=f"..{os.sep}..{os.sep}{buildDir}" # build out of tree

    shutil.rmtree(BUILD_DIR, ignore_errors=True)
    os.mkdir(BUILD_DIR)

    # VCPKG_ROOT="E:\\vcpkg"
    subprocess.run(["cmake",
        "-G", f"{CMAKE_GENERATOR}",
        # f"-DCMAKE_TOOLCHAIN_FILE={VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake",
        f"-DCMAKE_BUILD_TYPE={buildType}",
        f"-DCMAKE_C_COMPILER={CC}",
        f"-DCMAKE_CXX_COMPILER={CPPC}",
        "-DUSE_QUADMATH=Off", # use MPFR by default
        "-B", BUILD_DIR,
        "-S", f"..{os.sep}"], capture_output=False, text=True, check=True)

def main():
    if len(sys.argv) != 2:
        print(f"Error: use {__file__} Clang|Gcc")
        exit(1)

    if sys.argv[1].lower() == "clang":
        cmakeGenerate("Release", "postrisc_release", NATIVE_C, NATIVE_CPP)
        cmakeGenerate("DebWithAsan", "postrisc_debug", NATIVE_C, NATIVE_CPP)
    elif sys.argv[1].lower() == "gcc":
        cmakeGenerate("Debug", "postrisc_debug_gcc", "gcc", "g++")
        cmakeGenerate("Release", "postrisc_release_gcc", "gcc", "g++")
    else:
        print(f"Error: Unknown option: {sys.argv[1]}")
        exit(1)

if __name__ == "__main__":
    main()
