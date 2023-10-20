#!/usr/bin/python3

# MitchAlsup My66000
# https://github.com/bagel99/llvm-my66000
# git remote add my66000 git@github.com:bagel99/llvm-my66000.git
# git fetch my66000

import shutil
import os
import subprocess

PostriscFile="../../llvm-project/llvm/lib/Target/Postrisc/PostriscNullificationPass.cpp"
My66000File="../../llvm-my66000/llvm/lib/Target/My66000/My66000PredicatePass.cpp"
My66000LocalFile="My66000PredicatePass.cpp"

with open(My66000File, 'r') as file:
    filedata = file.read()
    filedata = filedata.replace("My66000PredBlock", "PostriscNullification")
    filedata = filedata.replace('My66000', 'Postrisc')
    filedata = filedata.replace('my66000', 'postrisc')
    with open(My66000LocalFile, 'w') as file:
        file.write(filedata)

subprocess.run(["meld", PostriscFile, My66000LocalFile])

os.remove(My66000LocalFile)


# subprocess.run(["meld", "../../llvm-project/llvm/lib/Target/ARC/ARCOptAddrMode.cpp", "../../llvm-project/llvm/lib/Target/Postrisc/PostriscOptAddrMode.cpp"])
