#!/usr/bin/python3

import builtins
import os
import shutil
import subprocess
import sys
import multiprocessing
import math

sys.path.append(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
import common
from common import *

GENERATED_CODEGEN_DIR = f"{CODEGEN_DIR}/generated"

# automate running LLVM codegen tests from postrisc repo.
def main():
    with open("out_codegen_tests_generated.tmp", "w") as outfile:
        shutil.rmtree(GENERATED_CODEGEN_DIR, ignore_errors=True)
        os.mkdir(GENERATED_CODEGEN_DIR)
        subprocess.run(["/home/user/risc/postrisc_release/bin/llvm_codegen"], cwd=GENERATED_CODEGEN_DIR, stdout=outfile, stderr=outfile, text=True, check=True)
        # print(f"LLVM_BUILD_DEBUG_BIN_DIR={LLVM_BUILD_DEBUG_BIN_DIR}")
        os.chdir(f"{LLVM_BUILD_DEBUG_BIN_DIR}")

        if platform.system() == 'Windows':
            subprocess.run(["python3", "./llvm-lit.py", "-v", GENERATED_CODEGEN_DIR], stdout=outfile, stderr=outfile, text=True, check=True)
        else:
            subprocess.run(["./llvm-lit",               "-v", GENERATED_CODEGEN_DIR], stdout=outfile, stderr=outfile, text=True, check=True)

if __name__ == "__main__":
    main()
