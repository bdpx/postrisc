#!/usr/bin/env bash

set -eu -o pipefail

#bdp@bdp:${ROOT_DIR}/musl$ ./configure --help
#Usage: ./configure [OPTION]... [VAR=VALUE]... [TARGET]

#. ../utils/common.bash

declare -ar OPTIONS=(
    #--prefix=~/postrisc/musl/build/${ARCH}
    #--exec-prefix=~/postrisc/musl/build/${ARCH}
    #--disable-shared #inhibit building shared library
    --with-malloc=mallocng
)

#Some influential environment variables:
#  CC                      C compiler command [detected]
#  CFLAGS                  C compiler flags [-Os -pipe ...]
#  CROSS_COMPILE           prefix for cross compiler and tools [none]
#  LIBCC                   compiler runtime library [detected]

export CROSS_COMPILE=""
export CC="${CLANG_PATH} -mcmodel=small -fPIC" #--target=postrisc -mcpu=pv1
echo "CC=$CC"

# Postrisc specifiec parameters, pass from CLANG to LLC
CLANG_OPTIONS+=(
    -O2
#   -v
    -D_GNU_SOURCE
    -D_BSD_SOURCE
    -mllvm -inline-threshold=200
    -fno-unroll-loops
    -fno-optimize-sibling-calls
    -Wall
    -pedantic
    -std=c11
    -fomit-frame-pointer
    -fno-builtin
    -fuse-init-array
    -fPIC
#   -ffast-math" # detect and error:
#   -fno-vectorize"
#   -fno-unroll-loops
#   -mllvm -inline-threshold=0
#   -nostartfiles -nolibc -nodefaultlibs -nostdlib
    -fno-slp-vectorize # superword-level parallelism
    -disable-loop-vectorization
    -disable-vectorization
    -fno-vectorize
    -fno-tree-vectorize
)

LINKER_OPTIONS+=(
)

export CFLAGS="${CLANG_OPTIONS[@]}"
export LIBCC=""

export LDFLAGS="${LINKER_OPTIONS[@]}"

BUILD_DIR=${ROOT_DIR}/musl_$ARCH

rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

${ROOT_DIR}/musl/configure ${OPTIONS[@]} ${ARCH}

make -j10
