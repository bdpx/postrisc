#!/usr/bin/env bash

. ../utils/common.bash

export ARCH=postrisc
export CLANG_PATH="${LLVM_BUILD_DEBUG_BIN_DIR}/clang"

declare -a CLANG_OPTIONS=(
    -mcpu=pv1
    --gcc-toolchain=${ROOT_DIR}/llvm-build/bin
    -mllvm -verify-analysis-invalidation=0

    -mllvm --enable-block-merging=1
    -mllvm --postrisc-enable-dense-calls=1
    -mllvm --enable-nullification=0 # FIXME: issues
    -mllvm --enable-move-merging=1

    # -S -emit-llvm

    -fvisibility-inlines-hidden   # FIXME: shared object workaround
    #-fvisibility=hidden           # FIXME: shared object workaround
    -fvisibility=protected # FIXME: allow internal ip-rel usage without PLT/GOT
    # -fno-semantic-interposition # doom build failed with this
)

declare -a LINKER_OPTIONS=(
#    -Bsymbolic # failed
#    -fno-semantic-interposition # failed
)

. ./build_lib.bash

