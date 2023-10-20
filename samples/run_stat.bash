#!/usr/bin/env bash

exec > out_stat.tmp

# shellcheck source=/dev/null
. ../utils/common.bash


PATTERN_INSN="^\t[a-z][a-z|0-9|.]+"
PATTERN_BLOCKS="^((\.LBB)|(! %))"
PATTERN_BLOCKS_NATIVE="^(([a-z]+)|(\.LBB)|(! %))"

compute_statistic() {
    local -r FILENAME=$1

    echo "Postrisc instructions: ${FILENAME}"
    grep -Po "${PATTERN_INSN}" ${FILENAME}.s | wc -l

    echo "BLOCKS:"
    grep -P "${PATTERN_BLOCKS}" ${FILENAME}.s | wc -l

    grep -Po "${PATTERN_INSN}" ${FILENAME}.s | sort | uniq -c | /usr/bin/sort --reverse

    echo "Native instructions:"
    grep -Po "${PATTERN_INSN}" ${FILENAME}_native.s | wc -l

    echo "BLOCKS:"
    grep -P "${PATTERN_BLOCKS_NATIVE}" ${FILENAME}.s | wc -l

    grep -Po "${PATTERN_INSN}" ${FILENAME}_native.s | sort | uniq -c | /usr/bin/sort --reverse
}

compute_statistic "sample"
compute_statistic "ll_glibc"
