#!/usr/bin/env bash

CURRENT_DIR="$(dirname "$0")"

# exec > out_postrisc_emulate.tmp

. ../utils/common.bash

eval "$(cpp -D'X(NAME,VALUE)=declare -r NAME=VALUE' ../src/util/logger_subsystems.h)"

declare -ar LOG_CURRENT_FLAGS=(
    $LOG_TOKENIZER
    $LOG_PARSER
    $LOG_EVALUATE
#    $LOG_EMULATOR
#    $LOG_DECODER
    $LOG_LOADER
#    $LOG_PREDICATION
#    $LOG_REGISTER_ROTATION
#    $LOG_REGISTER_STACK
#    $LOG_FPU
    $LOG_EXCEPTION
    $LOG_LOAD
    $LOG_STORE
#    $LOG_ITLB
#    $LOG_ICACHE
#    $LOG_PAGETABLE
#    $LOG_PLATFORM
#    $LOG_CALLSTACK
#    $LOG_DTLB
#    $LOG_DCACHE
#    $LOG_SYSCALL
#    $LOG_DEBUGGER
    $LOG_REGISTER_DATA
#    $LOG_BRANCH
#    $LOG_INTERRUPT
#    $LOG_DISPLAY
#    $LOG_SERIALIZATION
    $LOG_DUMP
    $LOG_INSN_TRACE
    $LOG_DOOM
)

declare LOG_CURRENT_MASK=0
for flag in "${LOG_CURRENT_FLAGS[@]}"; do
    LOG_CURRENT_MASK=$(( ${LOG_CURRENT_MASK} + $flag ))
done
LOG_CURRENT_MASK=`printf "0x%X\n" $LOG_CURRENT_MASK`
echo "LOG_CURRENT_MASK=${LOG_CURRENT_MASK}"

declare -r BUILD_PATH="${ROOT_DIR}/postrisc_debug"

#${BUILD_PATH}/bin/postrisc --exeapp -- ${ROOT_DIR}/postrisc/src/emulator/linux_user/elf/executable.postrisc --config ${BUILD_PATH}/artifacts/config.xml

${LLVM_BUILD_DEBUG_BIN_DIR}/llvm-objdump -d --reloc \
    ${POSTRISC_PROJECT_DIR}/musl/executable.postrisc > executable.postrisc.s

# ${LOG_CURRENT_MASK}
# --config ${BUILD_PATH}/artifacts/config.xml

#gdb --args
${BUILD_PATH}/bin/postrisc \
    --log-level 0 \
    --log-subsystem ${LOG_CURRENT_MASK} \
    --log-file "sample-log-1.html" \
    --env "LS_COLORS=ow=1;105;30:di=1;34" \
    --env USER=user \
    --env LOGNAME=user \
    --env HOME=/home/user \
    --env NAME=DESKTOP \
    --env "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" \
    --env HOSTTYPE=postrisc \
    --exeapp \
    -- \
    ${POSTRISC_PROJECT_DIR}/musl/executable.postrisc \
    --second \
    --3rd \
    --fourth

${BUILD_PATH}/bin/postrisc \
    --log-level 0 \
    --log-subsystem 0 \
    --log-file "sample-log-2.html" \
    --exeapp \
    --env USER=user \
    --env HOME=/home/user \
    -- \
    ${POSTRISC_PROJECT_DIR}/musl/executable.postrisc \
    --help

# LD_SHOW_AUXV=1 ${BUILD_PATH}/bin/postrisc --exeapp -- ${ROOT_DIR}}/postrisc/tests/elf/executable.postrisc
# LD_SHOW_AUXV=1 ${BUILD_PATH}/bin/emulator_syscall ${ROOT_DIR}/musl/build/executable.postrisc -somearg --anothrearg --veryimportantarg  >ooooo.postrisc
# ${BUILD_PATH}/bin/emulator_syscall ${ROOT_DIR}/musl/build/executable.x86_64 >ooooo.x86_64
# ${BUILD_PATH}/bin/emulator_syscall ${ROOT_DIR}/postrisc_samples/bin/doomgeneric.postrisc >zzzzz.postrisc

