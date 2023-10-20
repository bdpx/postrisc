#!/usr/bin/env bash

# concat LLVM codegen tests as a single source file,
# compile it to obj and disassemble, compile it to asm directly,
# check the differences with the previous clang version.

exec > out_codegen_compile.tmp

# shellcheck source=/dev/null
. ../utils/common.bash

# -DLLVM_ENABLE_STATS -stats

## clang -print-targets -print-supported-cpus
## llc -mcpu=help
## ld.lld
## llvm-mc
## llvm-objdump

## store dags in near temp near dir, not default C:/Users/User/AppData/Local/Temp
#rm -rf ./temp
#mkdir ./temp
#TEMP=./temp
#TMP=./temp

SOURCE_FILE=foo.temp.c


LLC_OPTIONS+=(
    --enable-nullification=1
    --enable-block-merging=1
    --postrisc-enable-dense-calls=1
)
CLANG_OPTIONS+=(
    # -ffreestanding
    -mllvm --enable-nullification=1
    -mllvm --enable-block-merging=1
    -mllvm --postrisc-enable-dense-calls=1
)

cat ${CODEGEN_DIR}/*.c > ${SOURCE_FILE}

CLANG_OPTIONS+=(
     # -isystem ${CODEGEN_DIR}
     -I ${CODEGEN_DIR}
)

if [ 1 -eq 1 ]; then
  echo "use ${CLANG}"
  COMPILER=${CLANG}
else
  echo "use ${CLANGPP}"
  COMPILER=${CLANGPP}
  CLANG_OPTIONS+=(-x c++)
fi

###${COMPILER} -c ${SOURCE_FILE} -target postrisc -fPIE -emit-llvm -###

echo "###########################################################################"
echo "# initial llvm"
echo "###########################################################################"

# ${COMPILER} --target=x86 ${OPTIMIZATION_LEVEL} -integrated-as -S -emit-llvm ${SOURCE_FILE} -o foo_llvm.ll
echo "clang: ${SOURCE_FILE} ==> foo_clang.ll"
${COMPILER} ${CLANG_OPTIONS[@]} -emit-llvm ${SOURCE_FILE} -o foo_clang.ll

echo "llc: foo_llvm.ll => isel-dags"
declare -a DGA_OPTIONS=(
    -fast-isel=false
    -view-dag-combine1-dags  # displays the DAG after being built, before the first optimization pass.
    -view-legalize-dags      # displays the DAG before Legalization.
    -view-dag-combine2-dags  # displays the DAG before the second optimization pass.
    -view-isel-dags          # displays the DAG before the Select phase.
    -view-sched-dags         # displays the DAG before Scheduling.
    -view-sunit-dags         # displays the Scheduler's dependency graph.
)

###${LLC} ${LLC_OPTIONS[@]} ${DGA_OPTIONS[@]} foo_clang.ll

echo "###########################################################################"
echo "# llc-only"
echo "###########################################################################"

echo "llc: foo_clang.ll ==> foo_llc.s"
${LLC} ${LLC_OPTIONS[@]} --filetype=asm -o foo_llc.s foo_clang.ll

echo "llc: foo_clang.ll ==> foo_llc.o"
${LLC} ${LLC_OPTIONS[@]} --filetype=obj -o foo_llc.o foo_clang.ll

echo "objdump: foo_llc.o ==> foo_llc_disasm.s"
${OBJDUMP} ${OBJDUMP_OPTIONS[@]} - <foo_llc.o >foo_llc_disasm.s

echo "###########################################################################"
echo "# clang ll-only"
echo "###########################################################################"

CODEGEN_RESULT_DIR=./codegen

echo "clang: ${SOURCE_FILE} ==> foo_clang.o"
${COMPILER} ${CLANG_OPTIONS[@]} -emit-obj ${SOURCE_FILE} -o ${CODEGEN_RESULT_DIR}/foo_clang.o

echo "clang: ${SOURCE_FILE} ==> ${CODEGEN_RESULT_DIR}/foo_clang.s"
${COMPILER} ${CLANG_OPTIONS[@]} -S ${SOURCE_FILE} -o ${CODEGEN_RESULT_DIR}/foo_clang.s

echo "objdump: foo_clang.o ==> ${CODEGEN_DIR}/foo_clang_disasm.s"
${OBJDUMP} ${OBJDUMP_OPTIONS[@]} - < ${CODEGEN_RESULT_DIR}/foo_clang.o > ${CODEGEN_RESULT_DIR}/foo_clang_disasm.s

echo "###########################################################################"
echo "# assembler"
echo "###########################################################################"

echo "llvm-mc: foo_clang.s ==> foo_mc.o"
${ASSEMBLER} ${ASSEMBLER_OPTIONS[@]} --filetype=obj -o foo_mc.o ${CODEGEN_RESULT_DIR}/foo_clang.s

echo "objdump: foo_mc.o ==> foo_mc_disasm.s"
${OBJDUMP} ${OBJDUMP_OPTIONS[@]} - <foo_mc.o >foo_mc_disasm.s

echo "###########################################################################"
echo "# all objects/disams should be identical"
echo "###########################################################################"

echo "diff: foo_clang_disasm.s foo_llc_disasm.s"
diff ${CODEGEN_RESULT_DIR}/foo_clang_disasm.s foo_llc_disasm.s
echo "diff: foo_llc_disasm.s foo_mc_disasm.s"
diff foo_llc_disasm.s foo_mc_disasm.s
echo "diff: foo_mc_disasm.s foo_clang_disasm.s"
diff foo_mc_disasm.s ${CODEGEN_RESULT_DIR}/foo_clang_disasm.s

echo "diff: foo_clang.o foo_llc.o"
diff ${CODEGEN_RESULT_DIR}/foo_clang.o foo_llc.o
echo "diff: foo_llc.o foo_mc.o"
diff foo_llc.o foo_mc.o
echo "diff: foo_mc.o foo_clang.o"
diff foo_mc.o ${CODEGEN_RESULT_DIR}/foo_clang.o

echo "###########################################################################"
echo "# cleanup"
echo "###########################################################################"

rm foo_llc.s
rm foo_llc.o
rm foo_llc_disasm.s
rm foo_mc.o
rm foo_mc_disasm.s
rm ${SOURCE_FILE}

###########################################################################
# clang with linker (incomplete)
###########################################################################
### echo "${SOURCE_FILE} ==> foo.exe"
### ${COMPILER} -v --target=postrisc -mcpu=${CPUNAME} -fuse-ld=${LINKER} -ffast-math ${OPTIMIZATION_LEVEL} -o foo.exe ${SOURCE_FILE}

###echo "objdump: foo_elf_from_asm.o ==> foo_elf_from_asm_disasm.s"
###${OBJDUMP} -h --all-headers foo_elf_from_asm.o

#${COMPILER} --target=sparc -integrated-as -S -o foo_native_asm_sparc.s ${SOURCE_FILE}
# assemble postrisc asm into ELF
# --relocation-model=pic
#${COMPILER} --target=xcore -integrated-as -S -o foo_native_asm_xcore.s ${SOURCE_FILE}

echo "###########################################################################"
echo "# finish"
echo "###########################################################################"
