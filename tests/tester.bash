#!/usr/bin/env bash

set -eu -o pipefail

echo "tester started"

declare -r TEST_FILE=$1
declare -r RESULT_FILE=$2
declare -r EXEFILE=$3
declare -r OUTPUT_DIR=$4
declare -r ROM_IMAGE_PATH=$5

declare -r TEST_NAME=$(basename "${TEST_FILE%.*}")

echo "EXEFILE = ${EXEFILE}"
echo "TEST_FILE = ${TEST_FILE}"
echo "RESULT_FILE = ${RESULT_FILE}"
echo "TEST_NAME = ${TEST_NAME}"
echo "OUTPUT_DIR = ${OUTPUT_DIR}"

exec  2> >(tee -ia "${OUTPUT_DIR}"/"${TEST_NAME}".log >& 2)
exec {BASH_XTRACEFD}> "${OUTPUT_DIR}"/"${TEST_NAME}".log
set -x

EXE_ARGS=(
    --base-address 0xfffffffff1230000
    --memory 0x30000 0x0000001234500000
    --memory 0x30000 0x0000003234500000
    --pa-size 40
    --paging 14 3
    --device-array 0x0000000fee000000 0xffffffffee000000 0x1000
    --video 0x0000000fb0000000 0xffffffffb0000000 160 120
    --rom "${ROM_IMAGE_PATH}" 0x0000000fcc000000 0xffffffffcc000000
)

ASSEMBLE_ARGS=(
    --log-level 0
    --log-subsystem 0x4
    --log-file "${OUTPUT_DIR}"/"${TEST_NAME}.filelog"
    ${EXE_ARGS[@]}
)

declare -r PREPROC_FILE=${OUTPUT_DIR}/${TEST_NAME}.preproc
echo "PREPROC_FILE = ${PREPROC_FILE}"

cp ${TEST_FILE} ${PREPROC_FILE}
# add final instruction for exit emulator
printf ".text\n\thalt\n.end\n" >>${PREPROC_FILE}

${EXEFILE} --assemble ${ASSEMBLE_ARGS[@]} < ${PREPROC_FILE} > ${OUTPUT_DIR}/${TEST_NAME}.bin

${EXEFILE} --export-definitions ${ASSEMBLE_ARGS[@]} >${OUTPUT_DIR}/${TEST_NAME}.defs

# assemble 'program.s' and output as hex C table 'program.hex' for future usage
# ${EXEFILE} -asmc <program.s >program.c

# disassemble object file 'program.o'
${EXEFILE} --disasm <${OUTPUT_DIR}/${TEST_NAME}.bin >${OUTPUT_DIR}/${TEST_NAME}_diz.s

# disassemble and binary dump object file 'program.o'
# (with instruction fields bit-to-bit description)
${EXEFILE} --dumpbin <${OUTPUT_DIR}/${TEST_NAME}.bin >${OUTPUT_DIR}/${TEST_NAME}_dump.s

# assemble from disassembled 'out_diz.s' to 'out_diz.o'
#${EXEFILE} --assemble <${OUTPUT_DIR}/${TEST_NAME}_diz.s >${OUTPUT_DIR}/${TEST_NAME}_diz.bin

# assemble from disassembled 'out_dump.s' to 'out_dump.o'
#${EXEFILE} --assemble <${OUTPUT_DIR}/${TEST_NAME}_dump.s >${OUTPUT_DIR}/${TEST_NAME}_dump.bin

echo "run compiled program in emulator"
$EXEFILE --execute ${OUTPUT_DIR}/${TEST_NAME}.bin >"${OUTPUT_DIR}/${TEST_NAME}.out" ${EXE_ARGS[@]}

if ! [ -r "$RESULT_FILE" ]; then
    printf -- 'comparison file %s does not exist, this is probably a new test\n' "$RESULT_FILE"
    exit 1
fi

echo "diff: ${RESULT_FILE} ${OUTPUT_DIR}/${TEST_NAME}.out"

diff_result=$(diff --text -u "$RESULT_FILE" "${OUTPUT_DIR}/${TEST_NAME}.out" | tee "${OUTPUT_DIR}/${TEST_NAME}".diff)
echo "diff_result = $diff_result"

if [ "${diff_result}" != "" ]; then
    printf -- 'comparison failed\n' "$RESULT_FILE"
    exit 1
fi

# ${EXEFILE} --dumpbin < ${OUTPUT_DIR}/${TEST_NAME}.bin > ${OUTPUT_DIR}/${TEST_NAME}.dump.s
# diff ${OUTPUT_DIR}/${TEST_NAME}.dump.s ${TEST_FILE}.dump >${OUTPUT_DIR}/${TEST_NAME}.dump.diff

rm ${OUTPUT_DIR}/${TEST_NAME}.bin
rm ${PREPROC_FILE}

#remove empty diff files for success tests
# rm ${OUTPUT_DIR}/${TEST_NAME}.diff
#remove preprocessor defines
#rm ${OUTPUT_DIR}/mapping.inc

exit 0
