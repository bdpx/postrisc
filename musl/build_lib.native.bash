#!/usr/bin/env bash

. ../utils/common.bash

export ARCH=$(uname -m)
export CLANG_PATH="/usr/bin/clang-16"

declare -a CLANG_OPTIONS=(
)

declare -a LINKER_OPTIONS=(
)

. ./build_lib.bash
