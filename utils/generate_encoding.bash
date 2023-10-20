#!/usr/bin/env bash

# shellcheck source=/dev/null
. ./common.bash

${ROOT_DIR}/postrisc_debug/bin/postrisc --llvm > PostriscInstrEncoding.td

diff PostriscInstrEncoding.td ${ROOT_DIR}/llvm-project/llvm/lib/Target/Postrisc/PostriscInstrEncoding.td
