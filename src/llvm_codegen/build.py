#!/usr/bin/python3

import builtins
import math
import multiprocessing
import os
import shutil
import subprocess
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
import common
from common import *

def cmakeBuild(buildDir):
    num_cores = math.trunc(multiprocessing.cpu_count() * 0.75)
    print(f"build: {buildDir}")
    BUILD_DIR=f"..{os.sep}..{os.sep}..{os.sep}{buildDir}"
    subprocess.run(["cmake", "--build", BUILD_DIR, "-j", f"{num_cores}"], capture_output=False, text=True, check=True)

def main():
    cmakeBuild("postrisc_debug")

if __name__ == "__main__":
    main()
