#!/usr/bin/python3

import builtins
import os
import shutil
import subprocess
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

LOG_CURRENT_MASK=0

# ${LLVM_BUILD_BIN_DIR}/llvm-objdump -d --reloc ${ROOT_DIR}/postrisc_doom/bin/doomgeneric.postrisc > doomgeneric.postrisc.s

# On Windows we may build only using gcc
if platform.system() == 'Windows':
    POSTRISC_APP = "../../postrisc_release_gcc/bin/postrisc.exe"
else:
    POSTRISC_APP = "../../postrisc_release/bin/postrisc"

ARGS=[
    POSTRISC_APP, # we will be inside postrisc_doom/bin
    "--gui",
    "--log-level=0",
    f"--log-subsystem={LOG_CURRENT_MASK}",
    f"--log-file=../../postrisc/docs/doom-log.html",
    "--profiling",
    "--env=LS_COLORS=ow=1;105;30:di=1;34",
    "--env=USER=user",
    "--env=LOGNAME=user",
    "--env=HOME=/home/user", # UNIX delimiters
    "--env=NAME=DESKTOP",
    "--env=PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", # UNIX delimiters
    "--env=HOSTTYPE=postrisc",
    "--exeapp",
    "--param=doomgeneric.postrisc"
]

def main():
    print("start doom!")
    os.chdir(f"../../postrisc_doom/bin")
    subprocess.run(ARGS, check=True)

if __name__ == "__main__":
    main()
