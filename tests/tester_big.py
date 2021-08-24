#!/usr/bin/python3

import os
import shutil
import subprocess
import sys
import time

sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import common
from common import *

if len(sys.argv) != 8:
   print("args required: test_file result-file exe outdir rom-image")
   exit(1)

TEST_FILE=sys.argv[1]
RESULT_FILE=sys.argv[2]
EXEFILE=sys.argv[3]
TEST_DIR=sys.argv[4]
ROM_IMAGE_PATH=sys.argv[5]
OUTPUT_DIR=sys.argv[6]
SAMPLE_DIR=sys.argv[7]

sys.stderr = open(f"{TEST_DIR}{os.sep}tester-big.log", 'w')

EXE_ARGS=[
    "--instruction-limit", "200000000", # 200M instructions
    "--dump-file", f"{OUTPUT_DIR}/dump.html",
    "--timing-info",
    "--verbose",
    "--base-address", "0xfffffffff1230000",
    "--memory", "0x30000", "0x0000001234500000",
    "--memory", "0x30000", "0x0000003234500000",
    "--pa-size", "40",
    "--paging", "14", "3",
    "--device-array", "0x0000000fee000000", "0xffffffffee000000", "0x1000",
    "--video", "0x0000000fb0000000", "0xffffffffb0000000", "160", "120",
    "--rom", ROM_IMAGE_PATH, "0x0000000fcc000000", "0xffffffffcc000000",
]

# export preprocessor definitions for config space offsets

with open(f"{TEST_DIR}/mapping.inc", "w") as outfile:
    subprocess.run([EXEFILE, "--export-definitions"] + EXE_ARGS,
        stdout=outfile, check=True)

PREPROC_FILE=f"{TEST_FILE}.preproc"

# copy file and add final instruction for exit emulator
shutil.copyfile(TEST_FILE, PREPROC_FILE)
with open(PREPROC_FILE, "a") as file:
    file.write(".text\n")
    file.write("\n\thalt\n")
    file.write(".end\n")

print("assemble preprocessed file")
with open(PREPROC_FILE, "r") as infile:
    with open("program.bin", "w") as outfile:
        subprocess.run([EXEFILE, "--assemble"] + EXE_ARGS, stdin=infile, stdout=outfile, check=True)

#print("assemble 'program.s' and output as hex C table 'program.hex' for future usage")
#with open(PREPROC_FILE, "r") as infile:
#    with open(f"{OUTPUT_DIR}/program.c", "w") as outfile:
#        subprocess.run([EXEFILE, "--assemble-c"] + EXE_ARGS, stdin=infile, stdout=outfile, check=True)

print("disassemble object file 'program.o'")
with open(f"program.bin", "r") as infile:
    with open(f"{OUTPUT_DIR}{os.sep}out_diz.s", "w") as outfile:
        subprocess.run([EXEFILE, "--disasm"], stdin=infile, stdout=outfile, capture_output=False, text=True, check=True)

print("disassemble and binary dump object file 'program.o'")
# (with instruction fields bit-to-bit description)
with open(f"program.bin", "r") as infile:
    with open(f"{OUTPUT_DIR}{os.sep}out_dump.s", "w") as outfile:
        subprocess.run([EXEFILE, "--dumpbin"], stdin=infile, stdout=outfile, capture_output=False, text=True, check=True)

print("assemble from disassembled 'out_diz.s' to 'out_diz.o'")
with open(f"{OUTPUT_DIR}{os.sep}out_diz.s", "r") as infile:
    with open(f"{OUTPUT_DIR}{os.sep}out_diz.bin", "w") as outfile:
        subprocess.run([EXEFILE, "--assemble"], stdin=infile, stdout=outfile, capture_output=False, text=True, check=True)

print("assemble from disassembled 'out_dump.s' to 'out_dump.o'")
with open(f"{OUTPUT_DIR}{os.sep}out_dump.s", "r") as infile:
    with open(f"{OUTPUT_DIR}{os.sep}out_dump.bin", "w") as outfile:
        subprocess.run([EXEFILE, "--assemble"], stdin=infile, stdout=outfile, capture_output=False, text=True, check=True)

print(f"compare with {SAMPLE_DIR} files")
compare_text_files(f"{OUTPUT_DIR}{os.sep}out_diz.s", f"{SAMPLE_DIR}{os.sep}out_diz.s")
compare_text_files(f"{OUTPUT_DIR}{os.sep}out_dump.s", f"{SAMPLE_DIR}{os.sep}out_dump.s")
compare_binary_files(f"{OUTPUT_DIR}{os.sep}out_diz.bin", f"{SAMPLE_DIR}{os.sep}program.bin")
compare_binary_files(f"{OUTPUT_DIR}{os.sep}out_dump.bin", f"{SAMPLE_DIR}{os.sep}program.bin")
compare_binary_files("program.bin", f"{SAMPLE_DIR}{os.sep}program.bin")

#gdb
#--execute program.bin --config ./build/tests/big/patched_config.xml --dump-file ./docs/dump.html

print("execute binary program.bin")
with open(f"{OUTPUT_DIR}/result.txt", "w") as outfile:
    result = subprocess.run([EXEFILE, "--execute", "program.bin"] + EXE_ARGS, stdout=outfile, capture_output=False, text=True, check=True)
#gdb -ex=r --args ${EXEFILE} -exe program.bin

print("diff results")
compare_text_files(f"{RESULT_FILE}", f"{OUTPUT_DIR}/result.txt", False)

#print(f"xmllint ${OUTPUT_DIR}/dump.html")
#xmllint --noout --nonet --quiet --valid ${OUTPUT_DIR}/dump.html

# scanner output (tokenized program and some statistic)
# $(EXEFILE) -scan program.s >tokens.txt
# scanner output (tokenized program as HTML)
with open(TEST_FILE, "r") as infile:
    with open(f"{OUTPUT_DIR}/program.html", "w") as outfile:
        subprocess.run([EXEFILE, "--scan-html"], stdin=infile, stdout=outfile, capture_output=False, text=True, check=True)

print("compare two files (assembled from original & assembled from disassembled) as binary and find differences")
compare_binary_files("program.bin", f"{OUTPUT_DIR}{os.sep}out_diz.bin")
compare_binary_files("program.bin", f"{OUTPUT_DIR}{os.sep}out_dump.bin")

os.remove(PREPROC_FILE)
exit(0)
