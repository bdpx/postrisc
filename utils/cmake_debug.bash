#!/usr/bin/env bash

. ./common.bash

BUILD_DIR=${ROOT_DIR}/postrisc_debug

rm -rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cmake -G "Unix Makefiles" \
    -DCMAKE_BUILD_TYPE=DebWithAsan \
    -DCMAKE_CXX_COMPILER=clang++ \
    -B ${BUILD_DIR} \
    -S ${ROOT_DIR}/postrisc
