#!/usr/bin/env bash

set -eu -o pipefail

. ../utils/common.bash

CC="$CLANG_PATH/clang -mcmodel=small"

MUSL_PATH="${ROOT_DIR}/musl"
MUSL_LIB_PATH="${ROOT_DIR}/musl_${ARCH}"

OPTIONS+=(
    --verbose
    -Os
    -fPIE
    -Wall
    -pedantic
    -std=c11
    -isystem "${MUSL_PATH}/include"
    -isystem "${MUSL_PATH}/arch/postrisc"
    -isystem "${MUSL_PATH}/arch/generic"
    -isystem "${MUSL_LIB_PATH}/obj/include"
    -fno-builtin
    -nostdinc
    -nostartfiles
    -nolibc
    -nodefaultlibs
    -nostdlib
    -fvisibility-inlines-hidden   # FIXME: maybe for release only
    -fvisibility=hidden           # FIXME: maybe for release only
#    -Wl,-nolibc
    -Wl,-nostdlib
#    -Wl,-nodefaultlibs
#    -Wl,--nostartfiles
    -Wl,-static
    -fno-slp-vectorize
    -disable-loop-vectorization
    -disable-vectorization
    -fno-vectorize
    -fno-tree-vectorize
)

# big mono file, __STRICT_ANSI__ is for fair compare (no asm inlines)
# $CC -c ${OPTIONS[@]} -D__STRICT_ANSI__ -DSQLITE_PTRSIZE=8 -DSQLITE_BYTEORDER=1234 sqlite3.c -o sqlite3.$ARCH.o &>sqlite3.$ARCH.log
# $READELF -r sqlite3.$ARCH.o > sqlite3.$ARCH.o.relocs

$CC -c ${OPTIONS[@]} test.c -o test.$ARCH.o
$READELF -r test.$ARCH.o > test.$ARCH.o.relocs

$CC -c ${OPTIONS[@]} test_2.c -o test_2.$ARCH.o
$READELF -r test_2.$ARCH.o > test_2.$ARCH.o.relocs

# linker is one way, so crt after test, lib after crt
$CC ${OPTIONS[@]} test.c test_2.c ${MUSL_LIB_PATH}/lib/Scrt1.o -L${MUSL_LIB_PATH}/lib -lc  -o executable.$ARCH

$READELF -r executable.$ARCH > executable.$ARCH.relocs

$OBJDUMP --all-headers -d executable.$ARCH > executable.$ARCH.headers
