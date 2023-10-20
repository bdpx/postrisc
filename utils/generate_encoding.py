#!/usr/bin/env python3

import builtins
import os
import shutil
import subprocess
import sys

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

def main():
    llvm_file = "PostriscInstrEncoding.td";
    with open(llvm_file, "w") as outfile:
        subprocess.run(["../../postrisc_release/bin/llvm"],
            stdout=outfile, text=True, check=True)

    subprocess.run(["diff", llvm_file, "../../llvm-project/llvm/lib/Target/Postrisc/PostriscInstrEncoding.td"],
            text=True, check=True)
    print("Success");

if __name__ == "__main__":
    main()
