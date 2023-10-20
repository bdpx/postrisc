#!/usr/bin/env bash

# automate running LLVM codegen tests from postrisc repo.

exec > out_codegen_tests.tmp

# shellcheck source=/dev/null
. ../utils/common.bash

cd ${LLVM_BUILD_DEBUG_BIN_DIR}

./llvm-lit -v ${CODEGEN_DIR}


