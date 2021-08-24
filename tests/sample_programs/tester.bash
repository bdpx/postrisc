#!/usr/bin/env bash

set -eu -o pipefail

echo "tester started"

declare -r TEST_FILE=$1
declare -r POSTRISC_EXE=$2
declare -r SOURCE_DIR=$3

echo "exe = ${POSTRISC_EXE}"

declare -r EXEFILE=${POSTRISC_EXE}

echo "testfile = ${TEST_FILE}" 

declare -r TEST_NAME=$(basename "${TEST_FILE%.*}")

echo "testname = ${TEST_NAME}"
echo "testdir = ${TEST_DIR}"

exec  2> >(tee -ia "${TEST_DIR}"/"${TEST_NAME}".log >& 2)
exec {BASH_XTRACEFD}> "${TEST_DIR}"/"${TEST_NAME}".log
set -x

EXE_ARGS=(
    --config ${CONFIG_XML}
)

# export preprocessor definitions for config space offsets
#${EXEFILE} --export_definitions ${EXE_ARGS[@]} >${TEST_DIR}/mapping.inc

# here we use gcc as preprocessor and import defines from mapping.inc
# so we can use #define macros in asm code
#gcc -E -include ${TEST_DIR}/mapping.inc \
#    -x assembler-with-cpp \
#    ${SOURCE_DIR}/${TEST_FILE} \
#    >${TEST_DIR}/${TEST_NAME}.preproc

cp ${SOURCE_DIR}/${TEST_FILE} ${TEST_DIR}/${TEST_NAME}.preproc
# add final instruction for exit emulator
printf ".text\n\thalt\n.end\n" >>${TEST_DIR}/${TEST_NAME}.preproc

${EXEFILE} --assemble ${EXE_ARGS[@]} < ${TEST_DIR}/${TEST_NAME}.preproc > ${TEST_DIR}/${TEST_NAME}.bin

rm ${TEST_DIR}/${TEST_NAME}.preproc

# assemble 'program.s' and output as hex C table 'program.hex' for future usage
# $(EXEFILE) -asmc <program.s >program.c

# disassemble object file 'program.o'
#${EXEFILE} -disasm <${TEST_DIR}/${TEST_NAME}.bin >${TEST_DIR}/${TEST_NAME}_diz.s

# disassemble and binary dump object file 'program.o'
# (with instruction fields bit-to-bit description)
#${EXEFILE} -dumpbin <${TEST_DIR}/${TEST_NAME}.bin >${TEST_DIR}/${TEST_NAME}_dump.s

# assemble from disassembled 'out_diz.s' to 'out_diz.o'
#${EXEFILE} --assemble <${TEST_DIR}/${TEST_NAME}_diz.s >${TEST_DIR}/${TEST_NAME}_diz.bin

# assemble from disassembled 'out_dump.s' to 'out_dump.o'
#${EXEFILE} --assemble <${TEST_DIR}/${TEST_NAME}_dump.s >${TEST_DIR}/${TEST_NAME}_dump.bin

# run compiled program 'program.o' in emulator
$EXEFILE --execute ${TEST_DIR}/${TEST_NAME}.bin >"${TEST_DIR}/${TEST_NAME}.s.out" ${EXE_ARGS[@]}

rm ${TEST_DIR}/${TEST_NAME}.bin

declare -r CHECK_FILE=${TEST_DIR}/${TEST_FILE}.out
echo "checkfile = $CHECK_FILE"

if ! [ -r "$CHECK_FILE" ]; then
    printf -- 'comparison file %s does not exist, this is probably a new test\n' "$CHECK_FILE"
    exit 1
fi

echo "diff: " "$CHECK_FILE"  "${SOURCE_DIR}/${TEST_FILE}".out

diff_result=$(diff -u "${SOURCE_DIR}/${TEST_FILE}".out "$CHECK_FILE" | tee "${TEST_DIR}/${TEST_NAME}".diff)
echo "diff_result = $diff_result"

if [ "${diff_result}" != "" ]; then
    printf -- 'comparison failed\n' "$CHECK_FILE"
    exit 1
fi

#remove empty diff files for success tests
# rm ${TEST_DIR}/${TEST_NAME}.diff
#remove preprocessor defines
#rm ${TEST_DIR}/mapping.inc

exit 0
