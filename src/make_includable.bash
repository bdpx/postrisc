#!/bin/bash

# to generate includable data file (raw char data)
# char const string[] = 
# include "includable_file.xxx"
# ;

if [ "$#" -ne 2 ]; then
    echo "illegal number of parameters"
    exit 2
fi

declare -r SRC=$1
declare -r DST=$2

declare -r dst_path=$(dirname "$DST")

echo "create directory ${dst_path}"
mkdir -p ${dst_path}

echo "create includable c file ${DST} from ${SRC}"

declare -r DELIMETER=for_c++_include

echo "R\"${DELIMETER}($(<${SRC}))${DELIMETER}\"" >${DST}
