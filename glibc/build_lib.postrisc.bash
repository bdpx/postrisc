#!/usr/bin/env bash

. ../utils/common.bash

export NATIVE=$(uname -m)
export ARCH=postrisc
export CLANG_PATH="${ROOT_DIR}/llvm-build/bin"

BUILD_DIR=${ROOT_DIR}/glibc_$ARCH

rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

declare -a COPTIONS=(
    #-isystem /usr/include
    #-isystem /usr/include/x86_64-linux-gnu
    -isystem /home/bdp/postrisc/glibc_headers

    -D__WORDSIZE=64
    -D__TIMESIZE=64
    -D__postrisc_double_float

    #--verbose
    -g
    -O2

    #-std=c23

    -Wall
    #-pedantic
    #-std=c11
    -fomit-frame-pointer
    -fno-builtin
    -fuse-init-array
    #-fPIC
    #-fPIE

    -fno-slp-vectorize # superword-level parallelism
    -disable-loop-vectorization
    -disable-vectorization
    -fno-vectorize
    -fno-tree-vectorize

    -mcpu=pv1
    --gcc-toolchain=/home/bdp/postrisc/llvm-build/bin
    -mllvm -verify-analysis-invalidation=0
    -mllvm --enable-block-merging=1
    -mllvm --postrisc-enable-dense-calls=1
    -mllvm --enable-move-merging=1

    -mllvm --enable-nullification=0 # FIXME: issues

    -mllvm -inline-threshold=200
    -fno-unroll-loops
    -fno-optimize-sibling-calls

    #-fvisibility-inlines-hidden # FIXME: workaround
    #-fvisibility=hidden

    -Wno-unused-command-line-argument
    -Wno-variadic-macros
    -Wno-unsupported-floating-point-opt
    -Wno-gnu-include-next
    -Wno-gnu-zero-variadic-macro-arguments
    -Wno-c23-extensions
    -Wno-extra-semi
    -Wno-gnu-empty-struct
    -Wno-zero-length-array
    -Wno-language-extension-token
    -Wno-empty-translation-unit
    -Wno-gnu-statement-expression-from-macro-expansion
    -Wno-gnu-designator
    -Wno-gnu-pointer-arith
    -Wno-gnu-conditional-omitted-operand
    -Wno-gnu-flexible-array-initializer
    -Wno-macro-redefined # include error
    -Wno-invalid-noreturn # non-asm __longjmp.c
)

export CFLAGS="${COPTIONS[@]}"

# --build=$NATIVE-pc-linux-gnu \

# ../configure arm-none-linux-gnueabi --host=arm-none-linux-gnueabi --target=arm-linux-gnueabi --prefix=`pwd``
# --with-headers=${ROOT_DIR}/postrisc/glibc/include \
# --with-headers=/usr/include \

${ROOT_DIR}/glibc/configure $ARCH-linux-gnu \
  --prefix=${ROOT_DIR}/postrisc/utils/glibc/$ARCH \
  --exec-prefix=${ROOT_DIR}/postrisc/utils/glibc/$ARCH \
  --host=$ARCH-linux-gnu \
  --target=$ARCH-linux-gnu \
  --build=x86_64-pc-linux-gnu \
  --with-headers=${ROOT_DIR}/glibc_headers \
  CC="${CLANG_PATH}/clang" \
  CXX="${CLANG_PATH}/clang++"

