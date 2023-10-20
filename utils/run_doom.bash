#!/usr/bin/env bash

CURRENT_DIR="$(dirname "$0")"

exec > out_postrisc_doom.tmp

. ./common.bash

eval "$(cpp -D'X(NAME,VALUE)=declare -r NAME=VALUE' ../src/util/logger_subsystems.h)"

declare -ar LOG_CURRENT_FLAGS=(
#    $LOG_TOKENIZER
#    $LOG_PARSER
#    $LOG_EVALUATE
#    $LOG_EMULATOR
#    $LOG_DECODER
    $LOG_LOADER
#    $LOG_PREDICATION
#    $LOG_REGISTER_ROTATION
#    $LOG_REGISTER_STACK
#    $LOG_FPU
    $LOG_EXCEPTION
#    $LOG_LOAD
#    $LOG_STORE
#    $LOG_ITLB
#    $LOG_ICACHE
#    $LOG_PAGETABLE
#    $LOG_PLATFORM
#    $LOG_CALLSTACK
#    $LOG_DTLB
#    $LOG_DCACHE
#    $LOG_SYSCALL
#    $LOG_DEBUGGER
#    $LOG_REGISTER_DATA
#    $LOG_BRANCH
#    $LOG_INTERRUPT
#    $LOG_DISPLAY
#    $LOG_SERIALIZATION
#    $LOG_DUMP
#    $LOG_INSN_TRACE
#    $LOG_DOOM
)

declare LOG_CURRENT_MASK=0
for flag in "${LOG_CURRENT_FLAGS[@]}"; do
    LOG_CURRENT_MASK=$(( ${LOG_CURRENT_MASK} + $flag ))
done
LOG_CURRENT_MASK=`printf "0x%X\n" $LOG_CURRENT_MASK`
echo "LOG_CURRENT_MASK=${LOG_CURRENT_MASK}"

declare -r BUILD_PATH="${ROOT_DIR}/postrisc_debug" # _release

# ${BUILD_PATH}/bin/qtpostrisc --log-level 0 --log-subsystem ${LOG_CURRENT_MASK} --exeapp -- ${POSTRISC_PROJECT_DIR}/utils/musl/executable.postrisc --second --3rd --fourth

${LLVM_BUILD_BIN_DIR}/llvm-objdump -d --reloc ${ROOT_DIR}/postrisc_doom/bin/doomgeneric.postrisc > doomgeneric.postrisc.s

cd ${ROOT_DIR}/postrisc_doom/bin

#
cd ${ROOT_DIR}/postrisc_doom/bin/

${BUILD_PATH}/bin/qtpostrisc --log-level 0 --log-subsystem ${LOG_CURRENT_MASK} --exeapp -- ${ROOT_DIR}/postrisc_doom/bin/doomgeneric.postrisc
#${BUILD_PATH}/bin/qtpostrisc --log-level 0 --log-subsystem 0 --exeapp -- ${ROOT_DIR}/postrisc_doom/bin/doomgeneric.postrisc
