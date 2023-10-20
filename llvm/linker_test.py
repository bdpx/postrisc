#!/usr/bin/python3

import builtins
import glob
import os
import shutil
import subprocess
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *


## store dags in near temp near dir, not default C:/Users/User/AppData/Local/Temp
#rm -rf ./temp
#mkdir ./temp
#TEMP=./temp
#TMP=./temp

print(f"use {CLANG_DEBUG}")
COMPILER=CLANG_DEBUG

OPTIONS=[
    #code_model = 'medium'
    #tls_model = 'local-dynamic'
    "-v",
    "--target=postrisc",
    "-mcpu=pv1",
    "-fPIC",
    "-ffast-math",
    "-O2",
    "-fomit-frame-pointer",
    "-mllvm", "-verify-analysis-invalidation=0",
    "-mllvm", "--enable-nullification=0",
    #f"-fuse-ld={LINKER}",
]

os.chdir("./linker_test")

print("clang")

print("clang: main.c second.c ==> foo_clang.exe")
#${COMPILER} ${OPTIONS[@]} -emit-obj ${SOURCE_FILE} -o ./foo_clang.o
### echo "${SOURCE_FILE} ==> foo.exe"
subprocess.run([COMPILER] + OPTIONS + ["-c", "./main.c"])
subprocess.run([COMPILER] + OPTIONS + ["-c", "./second.c"])

with open("main.o", "rb") as infile:
    with open("main_disasm.s", "w") as outfile:
        subprocess.run([OBJDUMP_DEBUG] + OBJDUMP_OPTIONS + ["-"], stdin=infile, stdout=outfile, check=True)

with open("second.o", "rb") as infile:
    with open("second_disasm.s", "w") as outfile:
        subprocess.run([OBJDUMP_DEBUG] + OBJDUMP_OPTIONS + ["-"], stdin=infile, stdout=outfile, check=True)

print("linker: main.o second.o ==> foo.exxe")
subprocess.run([LINKER_DEBUG] + LINKER_OPTIONS + ["-o", "foo.exxe", "main.o", "second.o"], check=True)

print("objdump: foo.exxe ==> foo_exxe_disasm.s")
with open("foo.exxe", "rb") as infile:
    with open("foo_exxe_disasm.s", "w") as outfile:
        subprocess.run([OBJDUMP_DEBUG] + OBJDUMP_OPTIONS + ["-"], stdin=infile, stdout=outfile, check=True)

print("finish")
