#!/usr/bin/env bash

ARCH=$(uname -m)
CLANG_PATH="/usr/bin"
READELF=$CLANG_PATH/readelf
OBJDUMP=$CLANG_PATH/objdump

OPTIONS+=(
)

. ./compile.bash
