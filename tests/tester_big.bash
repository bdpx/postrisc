#!/usr/bin/env bash

set -eu -o pipefail

echo "tester started"

declare -r TEST_FILE=$1
declare -r RESULT_FILE=$2
declare -r EXEFILE=$3
declare -r TEST_DIR=$4
declare -r CONFIG_XML=$5
declare -r OUTPUT_DIR=$6
declare -r SAMPLE_DIR=$7

exec  2> >(tee -ia "${TEST_DIR}"/tester.log >& 2)
#exec {BASH_XTRACEFD}> "${TEST_DIR}"/tester.log
#set -x

CMP="cmp -b --bytes=10"

declare -r EXE_ARGS=(
    --config ${CONFIG_XML}
    --dump-file ${OUTPUT_DIR}/dump.html
)

echo create llvm table info
${EXEFILE} --llvm > PostriscInstrEncoding.td

# run compiled program 'program.o' in emulator
echo "args = ${EXE_ARGS[@]}"

# export preprocessor definitions for config space offsets
#${EXEFILE} --export-definitions ${EXE_ARGS[@]} >${TEST_DIR}/mapping.inc

#copy source file and add final instruction for graceful exit emulator
cp ${TEST_FILE} ${TEST_FILE}.preproc
printf ".text\n\thalt\n.end\n" >>${TEST_FILE}.preproc

echo assemble preprocessed file
${EXEFILE} --assemble ${EXE_ARGS[@]} < ${TEST_FILE}.preproc > program.bin
diff program.bin ${SAMPLE_DIR}/program.bin

echo assemble 'program.s' and output as hex C table 'program.hex' for future usage
# $(EXEFILE) -asmc <program.s > ${OUTPUT_DIR}/program.c

echo disassemble object file 'program.o'
${EXEFILE} --disasm <program.bin > ${OUTPUT_DIR}/out_diz.s

echo disassemble and binary dump object file 'program.o'
# (with instruction fields bit-to-bit description)
${EXEFILE} --dumpbin <program.bin > ${OUTPUT_DIR}/out_dump.s

echo assemble from disassembled 'out_diz.s' to 'out_diz.o'
${EXEFILE} --assemble < ${OUTPUT_DIR}/out_diz.s > ${OUTPUT_DIR}/out_diz.bin

echo assemble from disassembled 'out_dump.s' to 'out_dump.o'
${EXEFILE} --assemble < ${OUTPUT_DIR}/out_dump.s > ${OUTPUT_DIR}/out_dump.bin

echo "compare with SAMPLE_DIR files"
diff ${OUTPUT_DIR}/out_diz.s ${SAMPLE_DIR}/out_diz.s
diff ${OUTPUT_DIR}/out_dump.s ${SAMPLE_DIR}/out_dump.s
diff ${OUTPUT_DIR}/out_diz.bin ${SAMPLE_DIR}/program.bin
diff ${OUTPUT_DIR}/out_dump.bin ${SAMPLE_DIR}/program.bin

#gdb
#--execute program.bin --config ./build/tests/big/patched_config.xml --dump-file ./docs/dump.html

echo execute binary program.bin
${EXEFILE} --execute program.bin > ${OUTPUT_DIR}/result.txt ${EXE_ARGS[@]}
#gdb -ex=r --args ${EXEFILE} -exe program.bin

# ${TEST_SOURCE_DIR}"/result.txt
# diff_result=$(git diff "${OUTPUT_DIR}"/result.txt | tee "${TEST_DIR}"/result.txt.diff)

echo diff results
declare -r diff_result=$(diff ${RESULT_FILE} ${OUTPUT_DIR}/result.txt | tee result.txt.diff)

# echo "diff_result = $diff_result"

if [ "${diff_result}" != "" ]; then
    printf -- 'comparison failed\n'
    # exit 0
fi

#echo "xmllint ${OUTPUT_DIR}/dump.html"
#xmllint --noout --nonet --quiet --valid ${OUTPUT_DIR}/dump.html

# scanner output (tokenized program and some statistic)
# $(EXEFILE) -scan program.s >tokens.txt
# scanner output (tokenized program as HTML)
${EXEFILE} --scan-html <${TEST_FILE} > ${OUTPUT_DIR}/program.html

echo "compare two files (assembled from original & assembled from disassembled) as binary and find differences"
diff program.bin ${OUTPUT_DIR}/out_diz.bin
diff program.bin ${OUTPUT_DIR}/out_dump.bin

rm ${TEST_FILE}.preproc

exit 0
