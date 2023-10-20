#!/usr/bin/env bash

# shellcheck source=/dev/null
. ./common.bash

# thread apply all bt
# ${LLVM_BUILD_BIN_DIR}/clang -v --target=postrisc -ffast-math -O2 -integrated-as -S -o foo_clang.s foo.c

gdb --args ${LLVM_BUILD_BIN_DIR}/clang -v --target=postrisc -ffast-math -O2 -integrated-as -S -o ./foo_native_asm.s ./foo.temp.c
