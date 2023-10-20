#!/usr/bin/python3

import shutil
import os
import subprocess

TARGET_PATH="../../llvm-project/llvm/lib/Target"
shutil.rmtree("./Sparc", ignore_errors=True)
shutil.copytree(f"{TARGET_PATH}/Sparc", "./Sparc")


# this function should be reusable for dumping csv in production
def format_files(path):
    print(f"path={path}")
    # Get a list of all files and directories in the current working directory
    contents = os.listdir(path)
    print("Contents of the current directory:", contents)

    # To list only files in the current directory
    # os.listdir(path):
    for filename in contents:
        full_path = f"{path}{os.sep}{filename}"
        print(f"file={full_path}")
        if os.path.isfile(full_path):
            with open(full_path, 'r') as file:
                filedata = file.read()
            filedata = filedata.replace('SPARC', 'POSTRISC')
            filedata = filedata.replace('sparc', 'postrisc')
            filedata = filedata.replace('Sparc', 'Postrisc')
            filedata = filedata.replace('LEON', 'POSTRISC')
            filedata = filedata.replace('Leon', 'Postrisc')
            with open(full_path, 'w') as file:
                file.write(filedata)
        else:
            format_files(full_path)

        new_filename = filename
        new_filename = new_filename.replace('Sparc', 'Postrisc')
        new_filename = new_filename.replace('Leon', 'Postrisc')

        os.rename(f"{path}{os.sep}{filename}", f"{path}{os.sep}{new_filename}")


format_files("./Sparc")

#diff ./Sparc ${TARGET_PATH}/Postrisc > diff.txt
#kdiff3
#winmerge

subprocess.Popen(["meld", "./Sparc", f"{TARGET_PATH}/Postrisc"])
