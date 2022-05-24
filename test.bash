#!/bin/bash

set -eu -o pipefail

asm_file=./build/tests/sample_programs/all_in_one.s
echo "file=${asm_file}"

EXEFILE=./build/bin/postrisc
CMP="cmp -b --bytes=10"

EXE_ARGS=(
    --config ./config.xml
    --dump-file ./docs/dump.html
)

# run compiled program 'program.o' in emulator
echo "args = ${EXE_ARGS[@]}"

# export preprocessor definitions for config space offsets
#$EXEFILE --export_definitions ${EXE_ARGS[@]} >./mapping.inc

# we use gcc as preprocessor
# so we can use #define macros in asm code
#gcc -E -include ./mapping.inc -x assembler-with-cpp ${asm_file} > ${asm_file}.preproc
#rm ./mapping.inc

cp ${asm_file} ${asm_file}.preproc
# add final instruction for exit emulator
printf ".text\n\thalt\n.end\n" >>${asm_file}.preproc

#echo assemble preprocessed file
$EXEFILE --assemble ${EXE_ARGS[@]} < ${asm_file}.preproc > program.bin

#echo assemble 'program.s' and output as hex C table 'program.hex' for future usage
# $(EXEFILE) -asmc <program.s >program.c

#echo disassemble object file 'program.o'
$EXEFILE --disasm <program.bin >out_diz.s

#echo disassemble and binary dump object file 'program.o'
# (with instruction fields bit-to-bit description)
$EXEFILE --dumpbin <program.bin >out_dump.s

#echo assemble from disassembled 'out_diz.s' to 'out_diz.o'
$EXEFILE --assemble <out_diz.s >out_diz.bin

#echo assemble from disassembled 'out_dump.s' to 'out_dump.o'
$EXEFILE --assemble <out_dump.s >out_dump.bin

#gdb
#--execute program.bin --config ./build/tests/big/patched_config.xml --dump-file ./docs/dump.html

#echo execute binary program.bin
$EXEFILE --execute program.bin >./docs/result.txt ${EXE_ARGS[@]}
#gdb -ex=r --args $EXEFILE -exe program.bin

# scanner output (tokenized program and some statistic)
# $(EXEFILE) -scan program.s >tokens.txt
# scanner output (tokenized program as HTML)
$EXEFILE --scanhtml <${asm_file} >docs/program.html

# create html help
$EXEFILE --html >docs/gen.html

# compare two files (assembled from original & assembled from disassembled) as binary and find differences
$CMP program.bin out_diz.bin
$CMP program.bin out_dump.bin

$EXEFILE --llvm > PostriscInstrEncoding.td

if [ $# -eq 0 ]; then
    #git diff docs/result.txt
    diff ./docs/result.txt ${asm_file}.out >out.diff
else
    cat result.txt
fi

#read -p "Press any key to continue... " -n1 -s
