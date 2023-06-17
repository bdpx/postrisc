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

cppResult = subprocess.run(["cpp", "-DX(NAME,VALUE)=NAME=VALUE", "../src/util/logger_subsystems.h"],
    capture_output=True, text=True, check=True)

# print(f"cpp: {cppResult.stdout}")
exec(compile(cppResult.stdout, '<string>', 'exec'))

LOG_CURRENT_MASK= LOG_TOKENIZER | LOG_PARSER | LOG_EVALUATE \
    | LOG_LOADER \
    | LOG_EXCEPTION | LOG_LOAD | LOG_STORE \
    | LOG_REGISTER_DATA \
    | LOG_DUMP | LOG_INSN_TRACE | LOG_DOOM
    # LOG_EMULATOR | LOG_DECODER \
    # | LOG_PREDICATION | LOG_REGISTER_ROTATION | LOG_REGISTER_STACK | LOG_FPU \
    # | LOG_ITLB | LOG_ICACHE | LOG_PAGETABLE | LOG_PLATFORM | LOG_CALLSTACK | LOG_DTLB | LOG_DCACHE | LOG_SYSCALL | LOG_DEBUGGER \
    # | LOG_BRANCH | LOG_INTERRUPT | LOG_DISPLAY | LOG_SERIALIZATION \

print(f"{LOG_CURRENT_MASK:x}")

BUILD_PATH=f"{ROOT_DIR}/postrisc_debug"

#${BUILD_PATH}/bin/postrisc --exeapp -- ${ROOT_DIR}/postrisc/src/emulator/linux_user/elf/executable.postrisc --config ${BUILD_PATH}/artifacts/config.xml

with open(f"executable.postrisc.s", "w") as outfile:
    subprocess.run([f"{LLVM_BUILD_DEBUG_BIN_DIR}/llvm-objdump", "-d", "--reloc", f"{POSTRISC_PROJECT_DIR}/musl/executable.postrisc"], stdout=outfile, text=True, check=True)


# ${LOG_CURRENT_MASK}
# --config ${BUILD_PATH}/artifacts/config.xml

#gdb --args
print("1st run")
sys.stdout.flush()

subprocess.run([
    f"{BUILD_PATH}/bin/postrisc",
    "--log-level", "0",
    "--log-subsystem", f"{LOG_CURRENT_MASK}",
    "--log-file", "sample-log-1.html",
    "--env", "LS_COLORS=ow=1;105;30:di=1;34",
    "--env", "USER=user",
    "--env", "LOGNAME=user",
    "--env", "HOME=/home/user",
    "--env", "NAME=DESKTOP",
    "--env", "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
    "--env", "HOSTTYPE=postrisc",
    "--exeapp",
    "--",
    f"{POSTRISC_PROJECT_DIR}/musl/executable.postrisc",
    "--second",
    "--3rd",
    "--fourth"
])

print("2nd run")
sys.stdout.flush()

subprocess.run([
    f"{BUILD_PATH}/bin/postrisc",
    "--log-level", "0",
    "--log-subsystem", "0",
    "--log-file", "sample-log-2.html",
    "--env", "USER=user",
    "--env", "HOME=/home/user",
    "--exeapp",
    "--",
    f"{POSTRISC_PROJECT_DIR}/musl/executable.postrisc",
    "--help"
])

# LD_SHOW_AUXV=1 ${BUILD_PATH}/bin/postrisc --exeapp -- ${ROOT_DIR}}/postrisc/tests/elf/executable.postrisc
# LD_SHOW_AUXV=1 ${BUILD_PATH}/bin/emulator_syscall ${ROOT_DIR}/musl/build/executable.postrisc -somearg --anothrearg --veryimportantarg  >ooooo.postrisc
# ${BUILD_PATH}/bin/emulator_syscall ${ROOT_DIR}/musl/build/executable.x86_64 >ooooo.x86_64
# ${BUILD_PATH}/bin/emulator_syscall ${ROOT_DIR}/postrisc_samples/bin/doomgeneric.postrisc >zzzzz.postrisc

