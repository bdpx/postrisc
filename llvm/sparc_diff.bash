#!/usr/bin/env bash

# shellcheck source=/dev/null
. ../utils/common.bash

declare -r TARGET_PATH="${LLVM_PROJECT_DIR}/llvm/lib/Target"

cp -R ${TARGET_PATH}/Sparc .

# this function should be reusable for dumping csv in production
format_files() {
    if [ $# -eq 1 ]; then
        local -r folder=$1
        echo "folder=${folder}"
        cd ${folder}
    fi

    for file in Sparc*;
    do
      if [ -f "${file}" ]; then
        echo rename ${file}
        mv ${file} ${file//Sparc/Postrisc};
      fi
    done

    for file in Leon*;
    do
      if [ -f "${file}" ]; then
        echo rename ${file}
        mv ${file} ${file//Leon/Postrisc};
      fi
    done

    for file in *;
    do
      if [ -f "${file}" ]; then
        echo replace ${file}
        sed -i "s/SPARC/POSTRISC/g" ${file}
        sed -i  "s/Sparc/Postrisc/g" ${file}
        sed -i "s/sparc/postrisc/g" ${file}
        sed -i "s/LEON/POSTRISC/g" ${file}
        sed -i  "s/Leon/Postrisc/g" ${file}
      fi
    done

    if [ $# -eq 1 ]; then
        cd ..
    fi
}

cd Sparc

format_files
format_files "AsmParser"
format_files "Disassembler"

format_files "MCTargetDesc"
format_files "TargetInfo"

cd ..

#diff ./Sparc ${TARGET_PATH}/Postrisc > diff.txt

kdiff3 ./Sparc ${TARGET_PATH}/Postrisc &
