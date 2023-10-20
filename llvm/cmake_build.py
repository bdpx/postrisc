#!/usr/bin/python3

import builtins
import os
import shutil
import subprocess
import sys
import multiprocessing
import math

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

def cmakeBuild(buildType, buildDir):
    num_cores = math.trunc(multiprocessing.cpu_count() - 0)
    print(f"build: {buildType}, {buildDir}, {num_cores} jobs")
    BUILD_DIR=f"..{os.sep}..{os.sep}{buildDir}"
    with open(f"out_build-{buildType}.txt", "w") as outfile:
        subprocess.run(["cmake", "--build", BUILD_DIR, "-j", f"{num_cores}"],
            stderr=outfile, text=True, check=True)

def main():
    if len(sys.argv) != 2:
        print(f"Error: use {__file__} debug|release")
        exit(1)

    if sys.argv[1].lower() == "release":
        cmakeBuild("Release", "llvm-build-release")
    else:
        cmakeBuild("Debug", "llvm-build-debug")

if __name__ == "__main__":
    main()
