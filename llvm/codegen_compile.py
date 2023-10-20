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

# concat LLVM codegen tests as a single source file,
# compile it to obj and disassemble, compile it to asm directly,
# check the differences with the previous clang version.

sys.stdout = open("out_codegen_compile.tmp", 'w')
sys.stderr = sys.stdout

CODEGEN_RESULT_DIR="./codegen"

# -DLLVM_ENABLE_STATS -stats

## clang -print-targets -print-supported-cpus
## llc -mcpu=help
## ld.lld
## llvm-mc
## llvm-objdump

## store dags in near temp near dir, not default C:/Users/User/AppData/Local/Temp
#rm -rf ./temp
#mkdir ./temp

os.environ["TEMP"] = "./temp"
os.environ["TMP"] = "./temp"

SOURCE_FILE = "foo.temp.c"


LLC_OPTIONS.extend([
    "-O2",
    "--enable-nullification=1",
    "--enable-block-merging=1",
    "--postrisc-enable-dense-calls=1",
    "--enable-move-merging=1",
])

CLANG_OPTIONS.extend([
    "-O2",
    # "-ffreestanding",
    "-mllvm", "--enable-nullification=1",
    "-mllvm", "--enable-block-merging=1",
    "-mllvm", "--postrisc-enable-dense-calls=1",
    "-mllvm", "--enable-move-merging=1",
    "-fnative-half-type",
    "-fexperimental-relative-c++-abi-vtables",
    # "-isystem", CODEGEN_DIR,
    "-I", f"{CODEGEN_DIR}",
    "-I", f"{CODEGEN_DIR}/cpp",
])

def process_dot_files(directory_path):
    print(f"process_dot_files: {directory_path}")
    files = sorted(glob.glob(directory_path, recursive=False))
    for xfile in files:
        #convert to png: dot -Tpng input.dot > output.png
        print(f"convert: {xfile} {os.path.basename(xfile)}")
        with open(f"{xfile}.png", "wb") as outfile:
            subprocess.run(["dot", "-Tpng", xfile], stdout=outfile, check=True)


def concatenate_source_files(directory_path, output_filename):
    files = sorted(glob.glob(directory_path, recursive=True))
    with open(output_filename, "wb") as outfile:
        for xfile in files:
            print(f"include: {os.path.basename(xfile)}")
            with open(xfile, "rb") as infile:
                outfile.write(infile.read())
                #outfile.write("\n\n")  # Add a separator between file contents
    print(f"Concatenated content written to '{output_filename}'.")

print(f"directory_path={CODEGEN_DIR}/*.c")
concatenate_source_files(f"{CODEGEN_DIR}/sample.c", SOURCE_FILE)

def objdump_disassemble(obj_file, asm_file):
    print(f"objdump: {obj_file} ==> {asm_file}")
    with open(obj_file, "rb") as infile:
        with open(asm_file, "w") as outfile:
            subprocess.run([OBJDUMP_DEBUG] + OBJDUMP_OPTIONS + ["-"], stdin=infile, stdout=outfile, check=True)



if 1 == 1:
    print(f"use {CLANG_DEBUG}")
    COMPILER=CLANG_DEBUG
else:
    print(f"use {CLANGPP_DEBUG}")
    COMPILER=CLANGPP_DEBUG
    CLANG_OPTIONS.extend(["-x", "c++"])

###${COMPILER} -c ${SOURCE_FILE} -target postrisc -fPIE -emit-llvm -###

print("# initial llvm")

# ${COMPILER} --target=x86 ${OPTIMIZATION_LEVEL} -integrated-as -S -emit-llvm ${SOURCE_FILE} -o foo_llvm.ll
print(f"clang: {SOURCE_FILE} ==> foo_clang.ll")
subprocess.run([COMPILER] + CLANG_OPTIONS + ["-emit-llvm", SOURCE_FILE, "-o", "foo_clang.ll"], check=True)

##############################################################################
# create DAG graphs
##############################################################################

print("llc: foo_llvm.ll => isel-dags")
DGA_OPTIONS = [
    "-fast-isel=false",
    "-view-dag-combine-lt-dags",  # display dags before the post legalize types dag combine pass
    "-view-dag-combine1-dags",    # display dags after being built, before the first dag combine pass
    "-view-dag-combine2-dags",    # displays the DAG before the second optimization pass (second dag combine pass).
    "-view-isel-dags",            # display isel dags as they are selected (DAG before the Select phase)
    "-view-legalize-dags",        # display dags before legalize
    "-view-legalize-types-dags",  # display dags before legalize types
    "-view-misched-dags",         # display MISched dags after they are processed
    "-view-sched-dags",           # display sched dags as they are processed (DAG before Scheduling)
    "-view-sunit-dags",           # display SUnit dags after they are processed (Scheduler's dependency graph)
]

def run_dag():
    print("START");
    shutil.rmtree("./temp", ignore_errors=True)
    os.mkdir("./temp")
    subprocess.run([LLC_DEBUG] + LLC_OPTIONS + DGA_OPTIONS + ["foo_clang.ll"], check=True)
    print("START PNG");
    process_dot_files("./temp/*.dot");
    exit(0)

run_dag()

#copy .dot files for \tmp
#mv \tmp\*.dot .\temp\

#convert to png: dot -Tpng input.dot > output.png

#open png from command line:


##############################################################################
# llc-only
##############################################################################

print("llc-only")

print("llc: foo_clang.ll ==> foo_llc.s")
subprocess.run([LLC_DEBUG] + LLC_OPTIONS + ["--filetype=asm", "-o", "foo_llc.s", "foo_clang.ll"], check=True)

print("llc: foo_clang.ll ==> foo_llc.o")
subprocess.run([LLC_DEBUG] + LLC_OPTIONS + ["--filetype=obj", "-o", "foo_llc.o", "foo_clang.ll"], check=True)

objdump_disassemble("foo_llc.o", "foo_llc_disasm.s")

print("clang c/cpp to ll-only")

print(f"clang: {SOURCE_FILE} ==> foo_clang.o")
subprocess.run([COMPILER] + CLANG_OPTIONS + ["-emit-obj", SOURCE_FILE, "-o", f"{CODEGEN_RESULT_DIR}/foo_clang.o"], check=True)

print(f"clang: {SOURCE_FILE} ==> {CODEGEN_RESULT_DIR}/foo_clang.s")
subprocess.run([COMPILER] + CLANG_OPTIONS + ["-S", SOURCE_FILE, "-o", f"{CODEGEN_RESULT_DIR}/foo_clang.s"], check=True)

objdump_disassemble(f"{CODEGEN_RESULT_DIR}/foo_clang.o", f"{CODEGEN_RESULT_DIR}/foo_clang_disasm.s")

print("assembler to native object")

print("llvm-mc: foo_clang.s ==> foo_mc.o")
subprocess.run([ASSEMBLER_DEBUG] + ASSEMBLER_OPTIONS + ["--filetype=obj", "-o", "foo_mc.o", f"{CODEGEN_RESULT_DIR}/foo_clang.s"], check=True)

objdump_disassemble("foo_mc.o", "foo_mc_disasm.s")

print("all objects/disams should be identical:")

compare_text_files(f"{CODEGEN_RESULT_DIR}/foo_clang_disasm.s", "foo_llc_disasm.s")
compare_text_files("foo_llc_disasm.s", "foo_mc_disasm.s")
compare_text_files("foo_mc_disasm.s", f"{CODEGEN_RESULT_DIR}/foo_clang_disasm.s")

compare_binary_files(f"{CODEGEN_RESULT_DIR}/foo_clang.o", "foo_llc.o")
compare_binary_files("foo_llc.o", "foo_mc.o")
compare_binary_files("foo_mc.o", f"{CODEGEN_RESULT_DIR}/foo_clang.o")

# cleanup

os.remove("foo_llc.s")
os.remove("foo_llc.o")
os.remove("foo_llc_disasm.s")
os.remove("foo_mc.o")
os.remove("foo_mc_disasm.s")
os.remove(SOURCE_FILE)

###########################################################################
# clang with linker (incomplete)
###########################################################################
### echo "${SOURCE_FILE} ==> foo.exe"
### ${COMPILER} -v --target=postrisc -mcpu=${CPUNAME} -fuse-ld=${LINKER} -ffast-math ${OPTIMIZATION_LEVEL} -o foo.exe ${SOURCE_FILE}

###echo "objdump: foo_elf_from_asm.o ==> foo_elf_from_asm_disasm.s"
###${OBJDUMP} -h --all-headers foo_elf_from_asm.o

#${COMPILER} --target=sparc -integrated-as -S -o foo_native_asm_sparc.s ${SOURCE_FILE}
# assemble postrisc asm into ELF
# --relocation-model=pic
#${COMPILER} --target=xcore -integrated-as -S -o foo_native_asm_xcore.s ${SOURCE_FILE}

print("finish")
