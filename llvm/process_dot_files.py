#!/usr/bin/python3

import shutil
import os
import subprocess
import pathlib

# LLVM llc stores DAG graphs in /tmp
DIRECTORY="/tmp"

# prepare local tmp
shutil.rmtree("./tmp", ignore_errors=True)
os.makedirs("./tmp", exist_ok=True)


directory_path = pathlib.Path(DIRECTORY)

# copy graphviz dot files to local tmp, convert to png
matching_files = list(directory_path.glob("*.dot"))
for file in matching_files:
    print(file)
    shutil.copyfile(f"{DIRECTORY}/{file}", f"./{file}")
    os.remove(f"{DIRECTORY}/{file}")
    with open(f"{file}.png", "w") as outfile:
        subprocess.run(["dot", "-Tpng", f".{file}"], stdout=outfile)

