#!/usr/bin/python3

import builtins
import os
import shutil
import subprocess
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

def cmakeTest(buildType, buildDir):
    # print(f"test: {buildType}, {buildDir}", flush=True)
    BUILD_DIR=f"..{os.sep}..{os.sep}{buildDir}"
    # -j2 2>out_postrisc_release_test.tmp
    subprocess.run(["ctest", "--test-dir", BUILD_DIR, "-j1"], check=True)

def main():
    if len(sys.argv) != 2:
        print(f"Error: use {__file__} Debug|Release")
        exit(1)

    if sys.argv[1].lower() == "release":
        cmakeTest("Release", "postrisc_release")
    else:
        cmakeTest("Debug", "postrisc_debug")

if __name__ == "__main__":
    main()
