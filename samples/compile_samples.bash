#!/usr/bin/env bash

exec > out_samples.tmp

# shellcheck source=/dev/null
. ../utils/common.bash

COMPILER=${CLANG}

echo "###########################################################################"
echo "# compare code density and quality between native and postrisc"
echo "# compiled using host system headers"
echo "###########################################################################"

LLC_OPTIONS+=(
    --enable-block-merging=1
    --postrisc-enable-dense-calls=1
    --enable-nullification=0 # FIXME: issues
    --enable-move-merging=1
)

# Postrisc specifiec parameters, pass from CLANG to LLC
CLANG_OPTIONS+=(
    -mllvm --enable-nullification=0
    -mllvm --enable-block-merging=1
    -mllvm --enable-move-merging=1
    -mllvm --postrisc-enable-dense-calls=1
    -mllvm -inline-threshold=100
#    -fno-unroll-loops
)

# settings to use system headers
CLANG_OPTIONS+=(
    -D__x86_64__
    -isystem /usr/include
    -isystem /usr/include/x86_64-linux-gnu
    # -isystem ${ROOT_DIR}/musl/include
    # -isystem ${ROOT_DIR}/musl/arch/postrisc
    # -debug-only=postrisc-instinfo
)

NATIVE_OPTIONS+=(
    ${OPTIMIZATION_LEVEL}
)

compile_postrisc() {
    local -r SOURCE=$1
    local -r SOURCE_PATH="${SOURCE}.c"

    echo "###########################################################################"
    echo "# ${SOURCE}: compile postrisc-target"
    echo "###########################################################################"

    echo "clang: ${SOURCE} ==> ${SOURCE}.ll"
    ${COMPILER} ${CLANG_OPTIONS[@]} -S -emit-llvm ${SOURCE_PATH} -o ${SOURCE}.ll

    echo "clang: ${SOURCE} ==> ${SOURCE}.s"
    ${COMPILER} ${CLANG_OPTIONS[@]} -S ${SOURCE_PATH} -o ${SOURCE}.s

    echo "clang: ${SOURCE} ==> ${SOURCE}.o"
    ${COMPILER} ${CLANG_OPTIONS[@]} -emit-obj ${SOURCE_PATH} -o ${SOURCE}.o

    echo "objdump: ${SOURCE}.o ==> ${SOURCE}_disasm.s"
    ${OBJDUMP} ${OBJDUMP_OPTIONS[@]} - < ${SOURCE}.o > ${SOURCE}_disasm.s
}

compile_native() {
    local -r SOURCE=$1
    local -r SOURCE_PATH="${SOURCE}.c"

    echo "###########################################################################"
    echo "# ${SOURCE}: compile host-native"
    echo "###########################################################################"

    echo "gcc: ${SOURCE} ==> ${SOURCE}_native.o"
    gcc -c ${NATIVE_OPTIONS[@]} ${SOURCE_PATH} -o ${SOURCE}_native.o

    echo "gcc: ${SOURCE} ==> ${SOURCE}_disasm.s"
    gcc -S -c ${NATIVE_OPTIONS[@]} ${SOURCE_PATH} -o ${SOURCE}_native.s

    # --disassemble-all
    echo "objdump: ${SOURCE}_native.o ==> ${SOURCE}_native_disasm.s"
    objdump -h --reloc --syms --disassemble ${SOURCE}_native.o > ${SOURCE}_native_disasm.s
}

SAMPLE_PATH="${POSTRISC_PROJECT_DIR}/samples"

SAMPLE="${SAMPLE_PATH}/ll_glibc"
compile_postrisc "${SAMPLE}"
compile_native   "${SAMPLE}"

SAMPLE="${SAMPLE_PATH}/sample"
compile_postrisc "${SAMPLE}"
compile_native   "${SAMPLE}"

echo "###########################################################################"
echo "# doom"
echo "###########################################################################"

#echo "linker: ${SAMPLE}.o ==> ${SAMPLE}.exxe"
#${LINKER} ${LINKER_OPTIONS[@]} --library-path "${ROOT_DIR}/musl_postrisc" --library "musl" -o "${SAMPLE}.exxe" "${SAMPLE}.o"

echo "###########################################################################"
echo "# finish"
echo "###########################################################################"
