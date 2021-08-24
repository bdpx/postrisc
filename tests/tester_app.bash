#!/usr/bin/env bash

set -eu -o pipefail

echo "tester started"

declare -r TEST_FILE=$1
declare -r RESULT_FILE=$2
declare -r EXEFILE=$3
declare -r OUTPUT_DIR=$4
declare -r CONFIG_XML=$5

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
    --config ${CONFIG_XML}
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
