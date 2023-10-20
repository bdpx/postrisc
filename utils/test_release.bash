#!/usr/bin/env bash

# shellcheck source=/dev/null
. ./common.bash

ctest --test-dir ${ROOT_DIR}/postrisc_release # -j2 2>out_postrisc_release_test.tmp
