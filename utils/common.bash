#!/usr/bin/env bash

#########################################################################################
# expected placement:
# ~/postrisc (root dir)
#     llvm-build-debug
#         bin
#     llvm-build-releaqse
#         bin
#     llvm-project
#         llvm
#     musl
#         arch
#             postrisc
#     musl_postrisc (musl postrisc build)
#         lib
#     postrisc (this project)
#         utils
#         musl (scripts for musl build)
#     postrisc_debug (this project build)
#         bin
#     postrisc_release (this project build)
#         bin
#     postrisc_doom
#         bin
#             doomgeneric.postrisc (experimantal port)
#########################################################################################

set -eu -o pipefail
exec 2>&1

# mark this script output with prefix
PREFIX="SCRIPT: "
function echo() { /bin/echo $PREFIX$*; }

#########################################################################################
# setup your native clang compiler here
#########################################################################################

NATIVE_CLANG_PATH=/usr/bin
NATIVE_CLANGPP=${NATIVE_CLANG_PATH}/clang++
NATIVE_CLANG=${NATIVE_CLANG_PATH}/clang
#echo "NATIVE_CLANGPP=${NATIVE_CLANGPP} NATIVE_CLANG=${NATIVE_CLANG}"

#########################################################################################
# start from git top level folder
#########################################################################################
POSTRISC_PROJECT_DIR="$(git -C $(dirname "$0") rev-parse --show-toplevel)"
#echo "POSTRISC_PROJECT_DIR=${POSTRISC_PROJECT_DIR}"

ROOT_DIR="$(dirname "${POSTRISC_PROJECT_DIR}")"
#echo "ROOT_DIR=${ROOT_DIR}"

LLVM_PROJECT_DIR="${ROOT_DIR}/llvm-project"

LLVM_CMAKE_ROOT_DIR="${LLVM_PROJECT_DIR}/llvm"
#echo "LLVM_CMAKE_ROOT_DIR=${LLVM_CMAKE_ROOT_DIR}"

LLVM_BUILD_DEBUG_DIR="${ROOT_DIR}/llvm-build-debug"
LLVM_BUILD_RELEASE_DIR="${ROOT_DIR}/llvm-build-release"
#echo "LLVM_BUILD_DEBUG_DIR=${LLVM_BUILD_DEBUG_DIR}"

LLVM_BUILD_DEBUG_BIN_DIR="${LLVM_BUILD_DEBUG_DIR}/bin"
LLVM_BUILD_RELEASE_BIN_DIR="${LLVM_BUILD_RELEASE_DIR}/bin"
#echo "LLVM_BUILD_DEBUG_BIN_DIR=${LLVM_BUILD_DEBUG_BIN_DIR}"

#########################################################################################
# codegen tests
#########################################################################################
CODEGEN_DIR=${LLVM_PROJECT_DIR}/llvm/test/CodeGen/Postrisc
#echo "CODEGEN_DIR=${CODEGEN_DIR}"

#########################################################################################
# cross-compile tools with path
#########################################################################################
CLANG_DEBUG=${LLVM_BUILD_DEBUG_BIN_DIR}/clang
CLANGPP_DEBUG=${LLVM_BUILD_DEBUG_BIN_DIR}/clang++
LLC_DEBUG=${LLVM_BUILD_DEBUG_BIN_DIR}/llc
ASSEMBLER_DEBUG=${LLVM_BUILD_DEBUG_BIN_DIR}/llvm-mc
OBJDUMP_DEBUG=${LLVM_BUILD_DEBUG_BIN_DIR}/llvm-objdump
LINKER_DEBUG=${LLVM_BUILD_DEBUG_BIN_DIR}/ld.lld
TABLEGEN_DEBUG=${LLVM_BUILD_DEBUG_BIN_DIR}/llvm-tblgen

CLANG_RELEASE=${LLVM_BUILD_RELEASE_BIN_DIR}/clang
CLANGPP_RELEASE=${LLVM_BUILD_RELEASE_BIN_DIR}/clang++
LLC_RELEASE=${LLVM_BUILD_RELEASE_BIN_DIR}/llc
ASSEMBLER_RELEASE=${LLVM_BUILD_RELEASE_BIN_DIR}/llvm-mc
OBJDUMP_RELEASE=${LLVM_BUILD_RELEASE_BIN_DIR}/llvm-objdump
LINKER_RELEASE=${LLVM_BUILD_RELEASE_BIN_DIR}/ld.lld
TABLEGEN_RELEASE=${LLVM_BUILD_RELEASE_BIN_DIR}/llvm-tblgen

#echo "INCLUDE PATHS:"
#echo "$(echo | cpp -v)"
#for Windows: cpp -dM -E - < NUL

OPTIMIZATION_LEVEL="-O2"

ARCHNAME=postrisc
CPUNAME=pv1

#ARCHNAME=sparcv9
#CPUNAME=ultrasparc

CODE_MODEL=small # tiny, medium, large
RELOCATION_MODEL=pic #static

## possible debug options
unused_options=\
postrisc-asmparser,\
postrisc-asmprinter,\
postrisc-disassembler,\
postrisc-frame-fixer,\
postrisc-framelowering,\
postrisc-bundler,\
postrisc-instinfo,\
postrisc-instprinter,\
postrisc-isel,\
postrisc-isellowering,\
postrisc-mclower,\
postrisc-subtarget,\
postrisc-asmbackend,\
postrisc-elfobjwriter,\
postrisc-mccodeemitter,\
postrisc-mcexpr,\
postrisc-nullification

## enabled debug options
DEBUG_OPTIONS="-debug-only=" #postrisc-nullification,postrisc-instinfo,postrisc-bundler
#postrisc-mclower,postrisc-instprinter
#postrisc-frame-fixer,postrisc-framelowering

#-fpic
#Generate position-independent code (PIC) suitable for use in a shared library, if supported for the target machine.
#Such code accesses all constant addresses through a global offset table (GOT).
#The dynamic loader resolves the GOT entries when the program starts (the dynamic loader is not part of GCC; it is part of the operating system).
#If the GOT size for the linked executable exceeds a machine-specific maximum size, you get an error message from the linker indicating that -fpic does not work;
#in that case, recompile with -fPIC instead. (These maximums are 8k on the SPARC, 28k on AArch64 and 32k on the m68k and RS/6000. The x86 has no such limit.)
#Position-independent code requires special support, and therefore works only on certain machines.
#For the x86, GCC supports PIC for System V but not for the Sun 386i. Code generated for the IBM RS/6000 is always position-independent.
#When this flag is set, the macros __pic__ and __PIC__ are defined to 1.
#-fPIC
#If supported for the target machine, emit position-independent code, suitable for dynamic linking and avoiding any limit on the size of the global offset table.
#This option makes a difference on AArch64, m68k, PowerPC and SPARC.
#Position-independent code requires special support, and therefore works only on certain machines.

TLS_MODEL="local-dynamic"

declare -a CLANG_OPTIONS=(
    -cc1                                   # call native clang, not gcc-faking driver
    -v                                     # verbose
    -analyzer-stats                        # show llvm statistic, don't know how to pass it to llc
    # -print-stats                         # Print performance metrics and statistics
    -triple ${ARCHNAME}                    # target
    -target-cpu ${CPUNAME}                 # cpu-model
    -mrelocation-model ${RELOCATION_MODEL} # pic/statis
    -mcmodel=${CODE_MODEL}                 # small/medium/large

    # -pic-is-pie

    # Alter the thread-local storage model to be used (see Thread-Local).
    # The model argument should be one of ‘global-dynamic’, ‘local-dynamic’, ‘initial-exec’ or ‘local-exec’.
    # Note that the choice is subject to optimization:
    # the compiler may use a more efficient model for symbols not visible outside of the translation unit,
    # or if -fpic is not given on the command line.
    # The default without -fpic is ‘initial-exec’; with -fpic the default is ‘global-dynamic’.
    -ftls-model=${TLS_MODEL}

    ${OPTIMIZATION_LEVEL}
    -ffast-math
    -mframe-pointer=none
    #### exception support
    # -fexceptions                         # Enable support for exception handling
    # -fcxx-exceptions                     # Enable C++ exceptions

    # default -   default exception handling model
    # dwarf   -   DWARF-like CFI based exception handling
    # sjlj    -   SjLj exception handling
    # arm     -   ARM EHABI exceptions
    # wineh   -   Windows exception model
    # wasm    -   WebAssembly exception handling
    # -exception-model dwarf               #dwarf sjlj

    # -mllvm option passes arg to llvm/llc
    -mllvm ${DEBUG_OPTIONS}

    #Postrisc specifiec parameters, pass from CLANG to LLC
    -mllvm -verify-analysis-invalidation=0 #FIXME: workaround for analyser
    # -mllvm --postrisc-enable-dense-calls
)

# echo "CLANG_OPTIONS=${CLANG_OPTIONS[@]}"
## -integrated-as
## -###    ##show options but not run

## pic/pie-level set via command-line for clang, but passed to llc in ll file as "PIC Level" module flag
declare -a LLC_OPTIONS=(
    ${DEBUG_OPTIONS}
    --march=${ARCHNAME} -mcpu=${CPUNAME}   # target
    --relocation-model=${RELOCATION_MODEL} # pic/statis
    --code-model=${CODE_MODEL}             # small/medium/large
    ${OPTIMIZATION_LEVEL}
    --frame-pointer=none

    # Enable FP math optimizations that assume no +-Infs
    --enable-no-infs-fp-math

    # Enable FP math optimizations that assume no NaNs
    --enable-no-nans-fp-math

    #Postrisc specifiec LLC parameters
    # --postrisc-enable-dense-calls
)

declare -a ASSEMBLER_OPTIONS=(
    -assemble
    -arch=${ARCHNAME}
    -mcpu=${CPUNAME}
    --show-encoding       # Show instruction encodings
    --show-inst           # Show internal instruction representation
    #--show-inst-operands # Show instructions operands as parsed
)

declare -a LINKER_OPTIONS=(
    --error-limit=0
)

declare -a OBJDUMP_OPTIONS=(
    --disassemble
    --arch-name=${ARCHNAME}
    --mcpu=${CPUNAME}
    --syms               # Display the symbol table
    --symbolize-operands # Symbolize instruction operands when disassembling
    --show-all-symbols   # Show all symbols during disassembly, even if multiple symbols are defined at the same location
    --reloc              # Display the relocation entries in the file
    --all-headers        # Display all available header information, relocation entries and the symbol table
    --dynamic-syms       # Display the contents of the dynamic symbol table
    --show-lma           # Display LMA column when dumping ELF section headers
    # --full-contents"   # Display the content of each section
    --file-headers       # Display the contents of the overall file header
    --private-headers    # Display format specific file headers
)
