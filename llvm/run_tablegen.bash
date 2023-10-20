#!/usr/bin/env bash

# shellcheck source=/dev/null
. ../utils/common.bash

#  -print-records
#  -gen-dag-isel
#  --write-if-changed -o ${LLVM_BUILD_RELEASE_DIR}/lib/Target/Postrisc/PostriscGenDAGISel.inc 

# C:\llvm-project\build\bin\

${LLVM_BUILD_RELEASE_DIR}/NATIVE/bin/llvm-tblgen \
    -gen-dag-isel \
    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target/Postrisc \
    -I ${LLVM_BUILD_RELEASE_DIR}/include \
    -I ${LLVM_PROJECT_DIR}/llvm/include \
    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target \
    ${LLVM_PROJECT_DIR}/llvm/lib/Target/Postrisc/Postrisc.td \
    --write-if-changed \
    -o ${LLVM_BUILD_RELEASE_DIR}/lib/Target/Postrisc/PostriscGenDAGISel.inc

#${TABLEGEN} \
#    -gen-dag-isel \
#    -debug \
#    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target/Postrisc \
#    -I ${LLVM_PROJECT_DIR}/build/include \
#    -I ${LLVM_PROJECT_DIR}/llvm/include \
#    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target \
#    ${LLVM_PROJECT_DIR}/llvm/lib/Target/Postrisc/Postrisc.td

# ${TABLEGEN} \
#    -print-records -debug \
#    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target/Postrisc \
#    -I ${LLVM_BUILD_RELEASE_DIR}/include \
#    -I ${LLVM_PROJECT_DIR}/llvm/include \
#    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target \
#    ${LLVM_PROJECT_DIR}/llvm/lib/Target/Postrisc/Postrisc.td


# ${TABLEGEN} \
#    -gen-dag-isel -debug \
#    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target/Sparc \
#    -I ${LLVM_BUILD_RELEASE_DIR}/include \
#    -I ${LLVM_PROJECT_DIR}/llvm/include \
#    -I ${LLVM_PROJECT_DIR}/llvm/lib/Target \
#    ${LLVM_PROJECT_DIR}/llvm/lib/Target/Sparc/Sparc.td
