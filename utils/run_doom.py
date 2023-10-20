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

ARGS=[
    f"..{os.sep}..{os.sep}postrisc_release{os.sep}bin{os.sep}qtpostrisc", # we will be inside postrisc_doom/bin
    "--log-level", "0",
    "--log-subsystem", f"{LOG_CURRENT_MASK}",
    "--log-file", f"..{os.sep}..{os.sep}postrisc{os.sep}docs{os.sep}doom-log.html",
    "--profiling",
    "--env", "LS_COLORS=ow=1;105;30:di=1;34",
    "--env", "USER=user",
    "--env", "LOGNAME=user",
    "--env", "HOME=/home/user", # UNIX delimiters
    "--env", "NAME=DESKTOP",
    "--env", "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin", # UNIX delimiters
    "--env", "HOSTTYPE=postrisc",
    "--exeapp",
    "--",
    "doomgeneric.postrisc"
]

def main():
    print("start doom!")
    os.chdir(f"..{os.sep}..{os.sep}postrisc_doom{os.sep}bin")
    subprocess.run(ARGS, check=True)

if __name__ == "__main__":
    main()
