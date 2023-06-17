#!/usr/bin/env bash

. ../utils/common.bash

export ARCH=$(uname -m)
export CLANG_PATH=${NATIVE_CLANG_PATH}/clang

declare -a CLANG_OPTIONS=(
)

declare -a LINKER_OPTIONS=(
)

. ./build_lib.bash
