#!/usr/bin/python3

import os
import shutil
import subprocess
import sys
import time

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

if len(sys.argv) != 6:
   print("args required: test_file result-file exe outdir rom-image")
   exit(1)

print("tester started")

TEST_FILE=sys.argv[1]
RESULT_FILE=sys.argv[2]
EXEFILE=sys.argv[3]
OUTPUT_DIR=sys.argv[4]
ROM_IMAGE_PATH=sys.argv[5]

TEST_NAME = os.path.basename(TEST_FILE)
TEST_NAME, extension = os.path.splitext(TEST_NAME)

print(f"EXEFILE={EXEFILE} TEST_FILE={TEST_FILE} RESULT_FILE={RESULT_FILE} TEST_NAME={TEST_NAME} OUTPUT_DIR={OUTPUT_DIR}")

sys.stderr = open(f"{OUTPUT_DIR}{os.sep}{TEST_NAME}.log", 'w')

EXE_ARGS=[
    "--instruction-limit", "100000000", # 100M instructions
    "--log-level", "0",
    "--log-subsystem", "0x00000400",
    "--log-file", f"{OUTPUT_DIR}{os.sep}{TEST_NAME}.filelog",
    "--base-address", "0xfffffffff1230000",
    "--memory", "0x30000", "0x0000001234500000",
    "--memory", "0x30000", "0x0000003234500000",
    "--pa-size", "40",
    "--paging", "14", "3",
    "--device-array", "0x0000000fee000000", "0xffffffffee000000", "0x1000",
    "--video", "0x0000000fb0000000", "0xffffffffb0000000", "160", "120",
    "--rom", ROM_IMAGE_PATH, "0x0000000fcc000000", "0xffffffffcc000000",
]

ASSEMBLE_ARGS=[
#    "--log-level", "0",
#    "--log-subsystem", "0x4",
#    "--log-file", f"{OUTPUT_DIR}{os.sep}{TEST_NAME}.filelog"
]
ASSEMBLE_ARGS.extend(EXE_ARGS)

with open(f"{OUTPUT_DIR}{os.sep}{TEST_NAME}.defs", "w") as outfile:
    subprocess.run([EXEFILE, "--export-definitions"] + EXE_ARGS,
        stdout=outfile, check=True)

BINARY_FILE = f"{OUTPUT_DIR}{os.sep}{TEST_NAME}.bin"
PREPROC_FILE=f"{OUTPUT_DIR}{os.sep}{TEST_NAME}.preproc"
print(f"PREPROC_FILE = {PREPROC_FILE}")

# copy file and add final instruction for exit emulator
shutil.copyfile(TEST_FILE, PREPROC_FILE)
with open(PREPROC_FILE, "a") as file:
    file.write(".text\n")
    file.write("    halt\n")
    file.write(".end\n")

with open(PREPROC_FILE, "r") as infile:
    with open(BINARY_FILE, "w") as outfile:
        subprocess.run([EXEFILE, "--assemble"] + ASSEMBLE_ARGS, stdin=infile, stdout=outfile, check=True)

# assemble 'program.s' and output as hex C table 'program.hex' for future usage
# ${EXEFILE} -asmc <program.s >program.c

# disassemble object file 'program.o'
with open(BINARY_FILE, "r") as infile:
    with open(f"{OUTPUT_DIR}{os.sep}{TEST_NAME}_diz.s", "w") as outfile:
        subprocess.run([EXEFILE, "--disasm"], stdin=infile, stdout=outfile, capture_output=False, text=True, check=True)

# disassemble and binary dump object file 'program.o' (with instruction fields bit-to-bit description)
with open(BINARY_FILE, "r") as infile:
    with open(f"{OUTPUT_DIR}{os.sep}{TEST_NAME}_dump.s", "w") as outfile:
        subprocess.run([EXEFILE, "--dumpbin"], stdin=infile, stdout=outfile, capture_output=False, text=True, check=True)

# assemble from disassembled 'out_diz.s' to 'out_diz.o'
#${EXEFILE} --assemble <${OUTPUT_DIR}/${TEST_NAME}_diz.s >${OUTPUT_DIR}/${TEST_NAME}_diz.bin

# assemble from disassembled 'out_dump.s' to 'out_dump.o'
#${EXEFILE} --assemble <${OUTPUT_DIR}/${TEST_NAME}_dump.s >${OUTPUT_DIR}/${TEST_NAME}_dump.bin

TEST_OUT_FILE=f"{OUTPUT_DIR}{os.sep}{TEST_NAME}.out"
print("run compiled program in emulator")
with open(TEST_OUT_FILE, "w") as outfile:
    result = subprocess.run([EXEFILE, "--execute", BINARY_FILE] + EXE_ARGS, stdout=outfile, capture_output=False, text=True, check=True)

if os.path.exists(RESULT_FILE):
    compare_text_files(TEST_OUT_FILE, RESULT_FILE)
else:
    print(f"comparison file %s does not exist, this is probably a new test: {RESULT_FILE}")
    exit(1)


# ${EXEFILE} --dumpbin < ${OUTPUT_DIR}/${TEST_NAME}.bin > ${OUTPUT_DIR}/${TEST_NAME}.dump.s
# diff ${OUTPUT_DIR}/${TEST_NAME}.dump.s ${TEST_FILE}.dump >${OUTPUT_DIR}/${TEST_NAME}.dump.diff

os.remove(BINARY_FILE)
os.remove(PREPROC_FILE)

#remove empty diff files for success tests
# rm ${OUTPUT_DIR}/${TEST_NAME}.diff
#remove preprocessor defines
#rm ${OUTPUT_DIR}/mapping.inc
exit(0)
