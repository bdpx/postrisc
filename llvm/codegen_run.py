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

# automate running LLVM codegen tests from postrisc repo.
def main():
    with open("out_codegen_tests.tmp", "w") as outfile:
        # print(f"LLVM_BUILD_DEBUG_BIN_DIR={LLVM_BUILD_DEBUG_BIN_DIR}")
        os.chdir(f"{LLVM_BUILD_DEBUG_BIN_DIR}")

        if platform.system() == 'Windows':
            subprocess.run(["python3", "./llvm-lit.py", "-v", CODEGEN_DIR], stdout=outfile, stderr=outfile, text=True, check=True)
        else:
            subprocess.run(["./llvm-lit",               "-v", CODEGEN_DIR], stdout=outfile, stderr=outfile, text=True, check=True)

if __name__ == "__main__":
    main()
