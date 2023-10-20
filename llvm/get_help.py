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

with open("help.tmp", 'w') as outfile:
    print("help_clang_cc1:\n", file=outfile)
    outfile.flush()
    subprocess.run([CLANG_DEBUG, "-print-targets"], stdout=outfile, check=True)
    subprocess.run([CLANG_DEBUG, "-cc1", "-help"], stdout=outfile, check=True)
    subprocess.run([CLANGPP_DEBUG, "-cc1", "-help"], stdout=outfile, check=True)
    subprocess.run([CLANG_DEBUG, "-help", "--help-hidden"], stdout=outfile, check=True)
    print("help_clang++:\n", file=outfile)
    outfile.flush()
    subprocess.run([CLANGPP_DEBUG, "-help", "--help-hidden"], stdout=outfile, check=True)
    subprocess.run([LLC_DEBUG, "--help", "--help-hidden"], stdout=outfile, check=True)  # --help-list --help-list-hidden
    subprocess.run([ASSEMBLER_DEBUG, "--help", "--help-hidden"], stdout=outfile, check=True)
    subprocess.run([OBJDUMP_DEBUG, "--help", "--help-hidden"], stdout=outfile, check=True)
    print("help_linker:\n", file=outfile)
    outfile.flush()
    subprocess.run([LINKER_DEBUG, "--help"], stdout=outfile, check=True)

