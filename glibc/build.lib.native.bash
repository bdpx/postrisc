#!/usr/bin/env bash

. ../utils/common.bash

# https://www.linaro.org/blog/building-glibc-with-llvm-the-how-and-why/
# clang requires 15 version

cd ${ROOT_DIR}/postrisc/utils/glibc/x86_64

#  --disable-shared \

${ROOT_DIR}/glibc/configure \
  --prefix=${ROOT_DIR}/postrisc/utils/glibc/x86_64 \
  --host=x86_64-linux-gnu \
  --build=x86_64-linux-gnu \
  CC="gcc" \
  CXX="gcc++" \
  CFLAGS="-O2" \
  CXXFLAGS="-O2"

