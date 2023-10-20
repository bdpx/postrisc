#!/usr/bin/env bash

# shellcheck source=/dev/null
. ./common.bash

ctest --test-dir ${ROOT_DIR}/postrisc_debug # -j2 2>out_postrisc_debug_test.tmp
