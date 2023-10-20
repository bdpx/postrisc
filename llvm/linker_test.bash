#!/usr/bin/env bash

exec > out_linker_test.tmp

# shellcheck source=/dev/null
. ../utils/common.bash

## store dags in near temp near dir, not default C:/Users/User/AppData/Local/Temp
rm -rf ./temp
mkdir ./temp
TEMP=./temp
TMP=./temp

echo "use ${CLANG_DEBUG}"
COMPILER=${CLANG_DEBUG}

declare -a OPTIONS=(
    #code_model = 'medium'
    #tls_model = 'local-dynamic'
    -v
    --target=postrisc
    -mcpu=pv1
    -fPIC
    -ffast-math
    -O2
    -fomit-frame-pointer
    -mllvm -verify-analysis-invalidation=0
    -mllvm --enable-nullification
    #-fuse-ld=${LINKER}
)

cd ./linker_test

echo "###########################################################################"
echo "# clang"
echo "###########################################################################"

echo "clang: main.c second.c ==> foo_clang.exe"
#${COMPILER} ${OPTIONS[@]} -emit-obj ${SOURCE_FILE} -o ./foo_clang.o
### echo "${SOURCE_FILE} ==> foo.exe"
${COMPILER} ${OPTIONS[@]} -c ./main.c
${COMPILER} ${OPTIONS[@]} -c ./second.c

${OBJDUMP_DEBUG} ${OBJDUMP_OPTIONS[@]} - <main.o >main_disasm.s
${OBJDUMP_DEBUG} ${OBJDUMP_OPTIONS[@]} - <second.o >second_disasm.s

echo "linker: main.o second.o ==> foo.exxe"
${LINKER_DEBUG} ${LINKER_OPTIONS[@]} -o foo.exxe main.o second.o

echo "objdump: foo.exxe ==> foo_exxe_disasm.s"
${OBJDUMP_DEBUG} ${OBJDUMP_OPTIONS[@]} - <foo.exxe >foo_exxe_disasm.s

echo "###########################################################################"
echo "# finish"
echo "###########################################################################"
