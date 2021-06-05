#!/usr/bin/python3

import os
import shutil
import subprocess
import sys
import time

# https://www.intel.com/content/www/us/en/download/684897/intel-software-development-emulator.html
# Intel Software Development Emulator (SDE) for AVX512-FP16 instruction tests failed: flags
SDE_OPTIONS=[
    "/home/user/risc/misc/sde/sde64", # path to SDE
    "-spr", # Sapphire Rapids (the only one which supports FP16)
    # "-fp16_fast",
    "--",
]

APP_OPTIONS=[
    "maxCount=1000000",   # number of test cases per test
    "maxErrorCount=1000", # number of errors before test fail
    "forever=0",          # run forever one test
    "checknans=1",        # check NaN values
    "checkintnans=1",     # check integer NaN values
    "checkflags=1",       # check FP flags
]

TEST_APP="/home/user/risc/postrisc_release_gcc/bin/softfp_test" # path to compiled tests

result = subprocess.run([TEST_APP, "--list_content"], capture_output=True, text=True)
all_tests = {line.removesuffix("*") for line in result.stderr.splitlines()}

TESTS_F16=[]
TESTS_F16_EXP=[]
TESTS_F128=[]
TESTS_F128_EXP=[]
TESTS_I128=[]
TESTS_I128_EXP=[]
TESTS_MANDATORY=[]
TESTS_EXP=[]


def is_experimental(name):
    return "_rmm" in name or "_rno" in name or "_rto" in name

for line in all_tests:
    if "i128" in line or "u128" in line:
        if is_experimental(line):
            TESTS_I128_EXP.extend([line])
        else:
            TESTS_I128.extend([line])
        continue

    if "f128" in line:
        if is_experimental(line):
            TESTS_F128_EXP.extend([line])
        else:
            TESTS_F128.extend([line])
        continue

    if "f16" in line:
        if is_experimental(line):
            TESTS_F16_EXP.extend([line])
        else:
            TESTS_F16.extend([line])
        continue

    if is_experimental(line):
        TESTS_EXP.extend([line])
        continue

    TESTS_MANDATORY.extend([line])


class Run:
    def __init__(self, test_set, path, sde = False):
        self.test_set = test_set
        self.path = path
        self.sde = sde

runs = [
    Run(TESTS_MANDATORY, "result_mandatory.txt"),
    Run(TESTS_EXP,       "result_experimental.txt"),
    Run(TESTS_F128,      "result_f128.txt"),
    Run(TESTS_F128_EXP,  "result_experimental_f128.txt"),
    Run(TESTS_I128,      "result_i128.txt"),
    Run(TESTS_I128_EXP,  "result_experimental_i128.txt"),
    Run(TESTS_F16,       "result_f16_avx.txt", True),
    Run(TESTS_F16,       "result_f16.txt", False),
    Run(TESTS_F16_EXP,   "result_experimental_f16_avx.txt", True),
    Run(TESTS_F16_EXP,   "result_experimental_f16.txt", False),
]

processes = []

for run in runs:
    print(f"run tests {run.path}")

    test_list = ",".join(run.test_set)
    options=[]
    if run.sde:
        options.extend(SDE_OPTIONS)
    options.extend([
        TEST_APP,
        "--color_output=no",
        f"--run_test={test_list}",
        "--",
    ])
    options.extend(APP_OPTIONS)

    with open(run.path, "w") as outfile:
        p = subprocess.Popen(options, stderr=outfile, stdout=outfile)
        processes.append(p)

for p in processes:
    p.wait()
