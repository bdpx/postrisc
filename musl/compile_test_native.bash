#!/usr/bin/env bash

ARCH=$(uname -m)
CLANG_PATH=${NATIVE_CLANG_PATH}
READELF=$CLANG_PATH/readelf
OBJDUMP=$CLANG_PATH/objdump

OPTIONS+=(
)

. ./compile.bash
