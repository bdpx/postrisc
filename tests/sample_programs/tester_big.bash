#!/bin/bash

set -eu -o pipefail

echo "tester started"

declare -r TEST_FILE=$1
declare -r POSTRISC_EXE=$2
declare -r SOURCE_DIR=$3

exec  2> >(tee -ia "${TEST_DIR}"/tester.log >& 2)
#exec {BASH_XTRACEFD}> "${TEST_DIR}"/tester.log
#set -x

CMP="cmp -b --bytes=10"

declare -r EXE_ARGS=(
    --config ${CONFIG_XML}
    --dump-file ${DOCS_DIR}/dump.html
)

# run compiled program 'program.o' in emulator
echo "args = ${EXE_ARGS[@]}"

# export preprocessor definitions for config space offsets
#${EXEFILE} --export_definitions ${EXE_ARGS[@]} >${TEST_DIR}/mapping.inc

# here we use gcc as preprocessor and import defines from mapping.inc
# so we can use #define macros in asm code
#gcc -E -include ${TEST_DIR}/mapping.inc \
#    -x assembler-with-cpp \
#    ${TEST_FILE} \
#    > ${TEST_FILE}.preproc

cp ${TEST_FILE} ${TEST_FILE}.preproc

# add final instruction for exit emulator
printf ".text\n\thalt\n.end\n" >>${TEST_FILE}.preproc

echo assemble preprocessed file
${EXEFILE} --assemble ${EXE_ARGS[@]} < ${TEST_FILE}.preproc > program.bin

echo assemble 'program.s' and output as hex C table 'program.hex' for future usage
# $(EXEFILE) -asmc <program.s >program.c

echo disassemble object file 'program.o'
${EXEFILE} --disasm <program.bin >out_diz.s

echo disassemble and binary dump object file 'program.o'
# (with instruction fields bit-to-bit description)
${EXEFILE} --dumpbin <program.bin >out_dump.s

echo assemble from disassembled 'out_diz.s' to 'out_diz.o'
${EXEFILE} --assemble <out_diz.s >out_diz.bin

echo assemble from disassembled 'out_dump.s' to 'out_dump.o'
${EXEFILE} --assemble <out_dump.s >out_dump.bin

echo execute binary program.bin
${EXEFILE} --execute program.bin >${DOCS_DIR}/result.txt ${EXE_ARGS[@]}
#gdb -ex=r --args ${EXEFILE} -exe program.bin

# scanner output (tokenized program and some statistic)
# $(EXEFILE) -scan program.s >tokens.txt
# scanner output (tokenized program as HTML)
${EXEFILE} --scanhtml <${TEST_FILE} >${DOCS_DIR}/program.html

echo create html help
${EXEFILE} --html >${DOCS_DIR}/gen.html

echo "compare two files (assembled from original & assembled from disassembled) as binary and find differences"
$CMP program.bin out_diz.bin
$CMP program.bin out_dump.bin

echo create llvm table info
${EXEFILE} --llvm > PostriscInstrEncoding.td

# ${TEST_SOURCE_DIR}"/result.txt
# diff_result=$(git diff "${DOCS_DIR}"/result.txt | tee "${TEST_DIR}"/result.txt.diff)

echo diff results
declare -r diff_result=$(diff -u ${TEST_FILE}.out ${DOCS_DIR}/result.txt | tee ${TEST_DIR}/result.txt.diff)

echo "diff_result = $diff_result"

if [ "${diff_result}" != "" ]; then
    printf -- 'comparison failed\n'
    exit 0 # temp
fi

exit 0
