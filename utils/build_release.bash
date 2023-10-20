#!/usr/bin/env bash

# shellcheck source=/dev/null
. ./common.bash

NPROC="$(nproc)"
echo "NPROC=${NPROC}"

NJOBS="$((${NPROC} - 4))"
echo "NJOBS=${NJOBS}"

cmake --build ${ROOT_DIR}/postrisc_release -j${NJOBS} 2>out_postrisc_build_release.tmp
