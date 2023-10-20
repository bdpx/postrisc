#!/usr/bin/env bash

# shellcheck source=/dev/null
. ../utils/common.bash

echo "###########################################################################"
echo "# help"
echo "###########################################################################"

${CLANG}      -cc1 -help > help_clang_cc1.txt
${CLANGPP}    -cc1 -help > help_clang++_cc1.txt
${CLANG}      -help --help-hidden > help_clang.txt
${CLANGPP}    -help --help-hidden > help_clang++.txt
${LLC}        --help --help-hidden > help_llc.txt  # --help-list --help-list-hidden

${ASSEMBLER}  --help --help-hidden > help_mc.txt
${OBJDUMP}    --help --help-hidden > help_objdump.txt
${LINKER}     --help > help_linker.txt

