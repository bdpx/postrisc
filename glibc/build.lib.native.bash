#!/usr/bin/env bash

. ../utils/common.bash

export ARCH=$(uname -m)
export CLANG_PATH="/usr/bin"

# https://www.linaro.org/blog/building-glibc-with-llvm-the-how-and-why/
# clang requires 15 version

BUILD_DIR=${ROOT_DIR}/glibc_$ARCH

rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

${ROOT_DIR}/glibc/configure \
  --prefix=${ROOT_DIR}/postrisc/utils/glibc/$ARCH \
  --exec-prefix=${ROOT_DIR}/postrisc/utils/glibc/$ARCH \
  --host=$ARCH-linux-gnu \
  --build=$ARCH-linux-gnu \
  CC="clang-16" \
  CXX="clang++-16" \
  CFLAGS="-O2" \
  CXXFLAGS="-O2"

