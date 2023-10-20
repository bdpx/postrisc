#!/usr/bin/env bash

. ../../utils/common.bash

ARCH="postrisc"
CLANG_PATH="${ROOT_DIR}/llvm-build-release/bin"
READELF=$CLANG_PATH/llvm-readelf
OBJDUMP=$CLANG_PATH/llvm-objdump

OPTIONS+=(
    -mllvm -inline-threshold=100
    -fno-unroll-loops
    -fno-optimize-sibling-calls

    -mllvm --enable-block-merging=1
    -mllvm --postrisc-enable-dense-calls=1
    -mllvm --enable-nullification=0 # FIXME: issues
    -mllvm --enable-move-merging=1
)

. ./compile.bash
