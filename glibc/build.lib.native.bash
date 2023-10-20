#!/usr/bin/env bash

export ARCH=$(uname -m)
export CLANG_PATH="/usr/bin"

# https://www.linaro.org/blog/building-glibc-with-llvm-the-how-and-why/
# clang requires 15 version

BUILD_DIR=../../glibc_$ARCH

rm -rf ${BUILD_DIR}
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

#  --prefix=${ROOT_DIR}/postrisc/utils/glibc/$ARCH \
#  --exec-prefix=${ROOT_DIR}/postrisc/utils/glibc/$ARCH \


../glibc/configure \
  CC="clang --target=$ARCH-linux-gnu" \
  CXX="clang++ --target=$ARCH-linux-gnu" \
  CFLAGS="-O2" \
  CXXFLAGS="-O2" \
  --host=$ARCH-linux-gnu \
  --build=$ARCH-linux-gnu \
  --prefix=/data/risc/glibc_$ARCH/usr \
  --disable-werror \
  --with-clang \
  --with-lld \
  --disable-multi-arch