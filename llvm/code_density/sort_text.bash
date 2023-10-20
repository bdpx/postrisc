#!/usr/bin/env bash

for f in *.o; do echo -e "$(size -A "$f" | awk '/\.text/ {print $2}')\t$f"; done | sort -n
