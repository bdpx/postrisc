#!/usr/bin/python3

import shutil
import os
import subprocess
import pathlib

# LLVM llc stores DAG graphs in /tmp
SRC_DIR="../../postrisc_release/src/llvm/llvm/"
FILE_NAME="PostriscInstrEncoding.td"

DST1="../src/llvm/"
DST2="../../llvm-project/llvm/lib/Target/Postrisc/"

shutil.copyfile(f"{SRC_DIR}{FILE_NAME}", f"{DST1}{FILE_NAME}")
shutil.copyfile(f"{SRC_DIR}{FILE_NAME}", f"{DST2}{FILE_NAME}")

