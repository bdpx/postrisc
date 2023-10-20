#!/usr/bin/env bash

# shellcheck source=/dev/null
. ./common.bash

${ROOT_DIR}/postrisc_release/bin/llvm > PostriscInstrEncoding.td

diff PostriscInstrEncoding.td ${ROOT_DIR}/llvm-project/llvm/lib/Target/Postrisc/PostriscInstrEncoding.td
