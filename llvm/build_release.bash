#!/usr/bin/env bash

# shellcheck source=/dev/null
. ../utils/common.bash

NPROC="$(nproc)"
echo "NPROC=${NPROC}"

NJOBS="$((${NPROC} - 0))" # reserve some cores if needed
echo "NJOBS=${NJOBS}"

cmake --build ${LLVM_BUILD_RELEASE_DIR} -j${NJOBS} 2>out_build.tmp
