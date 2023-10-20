#!/usr/bin/env bash

. ../utils/common.bash

export ARCH=postrisc
export CLANG_PATH="${ROOT_DIR}/llvm-build/bin"

declare -a CLANG_OPTIONS=(
    -mcpu=pv1
    --gcc-toolchain=${ROOT_DIR}/llvm-build/bin
    -mllvm -verify-analysis-invalidation=0

    -mllvm --enable-block-merging=1
    -mllvm --postrisc-enable-dense-calls=1
    -mllvm --enable-nullification=0 # FIXME: issues
    -mllvm --enable-move-merging=1

    # -S -emit-llvm

    -mllvm -inline-threshold=200
    -fno-unroll-loops
    -fno-optimize-sibling-calls

    -fvisibility-inlines-hidden   # FIXME: workaround
    -fvisibility=hidden           # FIXME: workaround
    # -fvisibility=protected # FIXME: allow internal ip-rel usage without PLT/GOT
    # -fno-semantic-interposition # doom build failed with this
)

declare -a LINKER_OPTIONS=(
#    -Bsymbolic # failed
#    -fno-semantic-interposition # failed
)

. ./build_lib.bash

