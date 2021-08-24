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
    --rom ${ROM_IMAGE_PATH} 0x0000000fcc000000 0xffffffffcc000000
)

echo "run compiled program in emulator"
$EXEFILE --exeapp ${TEST_FILE} >"${OUTPUT_DIR}/${TEST_NAME}.out" ${EXE_ARGS[@]}

if ! [ -r "$RESULT_FILE" ]; then
    printf -- 'comparison file %s does not exist, this is probably a new test\n' "$RESULT_FILE"
    exit 1
fi

echo "diff: ${RESULT_FILE} ${OUTPUT_DIR}/${TEST_NAME}.out"

diff_result=$(diff -u "$RESULT_FILE" "${OUTPUT_DIR}/${TEST_NAME}.out" | tee "${OUTPUT_DIR}/${TEST_NAME}".diff)
echo "diff_result = $diff_result"

if [ "${diff_result}" != "" ]; then
    printf -- 'comparison failed\n' "$RESULT_FILE"
    exit 1
fi

exit 0
