#!/usr/bin/env bash

. ./common.bash

BUILD_DIR=${ROOT_DIR}/postrisc_release

rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cmake -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_C_COMPILER=${NATIVE_CLANG} \
    -DCMAKE_CXX_COMPILER=${NATIVE_CLANGPP} \
    -B ${BUILD_DIR} \
    -S ${ROOT_DIR}/postrisc
