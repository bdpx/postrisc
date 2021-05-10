#!/usr/bin/python3

import builtins
import difflib
import filecmp
import os
import subprocess
import platform

#########################################################################################
# expected placement:
# risc (root dir)
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

# prefix all prints with 'SCRIPT:'
def print(*objs, **kwargs):
    builtins.print("SCRIPT:", *objs, **kwargs)

def get_arch():
    if len(sys.argv) == 2:
        if sys.argv[1].lower() == "postrisc":
            return "postrisc"
        else:
            return platform.machine()
    print(f"arch not set, use native: {platform.machine()}")
    return platform.machine()

def compare_text_files(file1_path, file2_path, fatal=True):
    with open(file1_path, 'r') as file1:
        file1_lines = file1.readlines()
    with open(file2_path, 'r') as file2:
        file2_lines = file2.readlines()

    diff = difflib.unified_diff(
        file1_lines,
        file2_lines,
        fromfile=file1_path,
        tofile=file2_path,
        lineterm=''  # Ensures uniform newline handling
    )

    if len(list(diff)) == 0:
        print(f"compare files: {file1_path} {file2_path} - identical")
        return

    print(f"file diffs: {file1_path} {file2_path}")
    for line in diff:
        builtins.print(line)
    if fatal:
        exit(1)

def compare_binary_files(file1_path, file2_path, fatal=True):
    if filecmp.cmp(file1_path, file2_path, shallow=False):
        print(f"compare files: {file1_path} {file2_path} - identical")
    else:
        print(f"compare files: {file1_path} {file2_path} - different")
        if fatal:
            exit(1)

if platform.system() == 'Windows':
    # modify PATH, add MSYS/CLANG64 paths first
    current_path = os.environ.get('PATH', '')
    new_path_entry = f"C:{os.sep}msys64{os.sep}clang64{os.sep}bin" + os.pathsep + f"C:{os.sep}msys64{os.sep}usr{os.sep}bin"
    os.environ['PATH'] = new_path_entry + os.pathsep + current_path
    #print(f"Updated PATH: {os.environ['PATH']}")

os.environ['PYTHONDONTWRITEBYTECODE'] = "1"

#########################################################################################
# setup your native clang compiler here
#########################################################################################

NATIVE_CLANG_PATH="" #f"C:/msys64/clang64/"
NATIVE_CPP=f"{NATIVE_CLANG_PATH}clang++"
NATIVE_C=f"{NATIVE_CLANG_PATH}clang"
#print(f"NATIVE_CPP={NATIVE_CPP} NATIVE_C={NATIVE_C}");


if platform.system() == 'Windows':
    CMAKE_GENERATOR="MinGW Makefiles"
else:
    CMAKE_GENERATOR="Unix Makefiles"

#########################################################################################
# start from git top level folder
#########################################################################################
POSTRISC_PROJECT_DIR=os.path.dirname(os.path.realpath(__file__))
#print(f"POSTRISC_PROJECT_DIR: {POSTRISC_PROJECT_DIR}")

ROOT_DIR=os.path.dirname(POSTRISC_PROJECT_DIR)
#print(f"ROOT_DIR={ROOT_DIR}")

LLVM_PROJECT_DIR=f"{ROOT_DIR}{os.sep}llvm-project"

LLVM_CMAKE_ROOT_DIR=f"{LLVM_PROJECT_DIR}{os.sep}llvm"
#print(f"LLVM_CMAKE_ROOT_DIR={LLVM_CMAKE_ROOT_DIR}")

LLVM_BUILD_DEBUG_DIR=f"{ROOT_DIR}{os.sep}llvm-build-debug"
LLVM_BUILD_RELEASE_DIR=f"{ROOT_DIR}{os.sep}llvm-build-release"
#print(f"LLVM_BUILD_DEBUG_DIR={LLVM_BUILD_DEBUG_DIR}")

LLVM_BUILD_DEBUG_BIN_DIR=f"{LLVM_BUILD_DEBUG_DIR}{os.sep}bin"
LLVM_BUILD_RELEASE_BIN_DIR=f"{LLVM_BUILD_RELEASE_DIR}{os.sep}bin"
#print(f"LLVM_BUILD_DEBUG_BIN_DIR={LLVM_BUILD_DEBUG_BIN_DIR}")

#########################################################################################
# llvm-lit codegen tests
#########################################################################################
CODEGEN_DIR=f"{LLVM_PROJECT_DIR}{os.sep}llvm{os.sep}test{os.sep}CodeGen{os.sep}Postrisc"
#print(f"CODEGEN_DIR={CODEGEN_DIR}")

#########################################################################################
# cross-compile tools with path
#########################################################################################
CLANG_DEBUG=f"{LLVM_BUILD_DEBUG_BIN_DIR}{os.sep}clang"
CLANGPP_DEBUG=f"{LLVM_BUILD_DEBUG_BIN_DIR}{os.sep}clang++"
LLC_DEBUG=f"{LLVM_BUILD_DEBUG_BIN_DIR}{os.sep}llc"
ASSEMBLER_DEBUG=f"{LLVM_BUILD_DEBUG_BIN_DIR}{os.sep}llvm-mc"
OBJDUMP_DEBUG=f"{LLVM_BUILD_DEBUG_BIN_DIR}{os.sep}llvm-objdump"
LINKER_DEBUG=f"{LLVM_BUILD_DEBUG_BIN_DIR}{os.sep}ld.lld"
TABLEGEN_DEBUG=f"{LLVM_BUILD_DEBUG_BIN_DIR}{os.sep}llvm-tblgen"

CLANG_RELEASE=f"{LLVM_BUILD_RELEASE_BIN_DIR}{os.sep}clang"
CLANGPP_RELEASE=f"{LLVM_BUILD_RELEASE_BIN_DIR}{os.sep}clang++"
LLC_RELEASE=f"{LLVM_BUILD_RELEASE_BIN_DIR}{os.sep}llc"
ASSEMBLER_RELEASE=f"{LLVM_BUILD_RELEASE_BIN_DIR}{os.sep}llvm-mc"
OBJDUMP_RELEASE=f"{LLVM_BUILD_RELEASE_BIN_DIR}{os.sep}llvm-objdump"
LINKER_RELEASE=f"{LLVM_BUILD_RELEASE_BIN_DIR}{os.sep}ld.lld"
TABLEGEN_RELEASE=f"{LLVM_BUILD_RELEASE_BIN_DIR}{os.sep}llvm-tblgen"

#echo "INCLUDE PATHS:"
#echo "$(echo | cpp -v)"
#for Windows: cpp -dM -E - < NUL

OPTIMIZATION_LEVEL="-O2"

ARCHNAME="postrisc"
CPUNAME="pv1"

#ARCHNAME=sparcv9
#CPUNAME=ultrasparc

CODE_MODEL="small" # tiny, medium, large
RELOCATION_MODEL="pic" #static

# possible debug options

unused_options=[
    "postrisc-asmparser",
    "postrisc-asmprinter",
    "postrisc-disassembler",
    "postrisc-frame-fixer",
    "postrisc-framelowering",
    "postrisc-bundler",
    "postrisc-instinfo",
    "postrisc-instprinter",
    "postrisc-isel",
    "postrisc-isellowering",
    "postrisc-mclower",
    "postrisc-subtarget",
    "postrisc-asmbackend",
    "postrisc-elfobjwriter",
    "postrisc-mccodeemitter",
    "postrisc-mcexpr",
    "postrisc-nullification"
]

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

CLANG_OPTIONS=[
    f"-cc1",                                     # call native clang, not gcc-faking driver
    f"-v",                                       # verbose
    f"-analyzer-stats",                          # show llvm statistic, don't know how to pass it to llc
    # f"-print-stats",                           # Print performance metrics and statistics
    "-triple", f"{ARCHNAME}",                    # target
    "-target-cpu", f"{CPUNAME}",                 # cpu-model
    "-mrelocation-model", f"{RELOCATION_MODEL}", # pic/statis
    f"-mcmodel={CODE_MODEL}",                    # small/medium/large

    # -pic-is-pie

    # Alter the thread-local storage model to be used (see Thread-Local).
    # The model argument should be one of global-dynamic, local-dynamic, initial-exec or local-exec.
    # Note that the choice is subject to optimization:
    # the compiler may use a more efficient model for symbols not visible outside of the translation unit,
    # or if -fpic is not given on the command line.
    # The default without -fpic is initial-exec; with -fpic the default is global-dynamic.
    f"-ftls-model={TLS_MODEL}",

    f"{OPTIMIZATION_LEVEL}",
    f"-ffast-math",
    f"-mframe-pointer=none",
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
    "-mllvm", f"{DEBUG_OPTIONS}",

    #Postrisc specifiec parameters, pass from CLANG to LLC
    "-mllvm", "-verify-analysis-invalidation=0", #FIXME: workaround for analyser
    # -mllvm --postrisc-enable-dense-calls
]

# echo "CLANG_OPTIONS=${CLANG_OPTIONS[@]}"
## -integrated-as
## -###    ##show options but not run

## pic/pie-level set via command-line for clang, but passed to llc in ll file as "PIC Level" module flag
LLC_OPTIONS=[
    f"{DEBUG_OPTIONS}",
    f"--march={ARCHNAME}",                    # target
    f"-mcpu={CPUNAME}",                       # target
    f"--relocation-model={RELOCATION_MODEL}", # pic/statis
    f"--code-model={CODE_MODEL}",             # small/medium/large
    f"{OPTIMIZATION_LEVEL}",
    f"--frame-pointer=none",

    # Enable FP math optimizations that assume no +-Infs
    # "--enable-no-infs-fp-math",

    # Enable FP math optimizations that assume no NaNs
    "--enable-no-nans-fp-math",

    #Postrisc specifiec LLC parameters
    # "--postrisc-enable-dense-calls
]

ASSEMBLER_OPTIONS=[
    "-assemble",
    f"-arch={ARCHNAME}",
    f"-mcpu={CPUNAME}",
    "--show-encoding",       # Show instruction encodings
    "--show-inst",           # Show internal instruction representation
    # "--show-inst-operands" # Show instructions operands as parsed
]

LINKER_OPTIONS=[
    "--error-limit=0"
]

OBJDUMP_OPTIONS=[
    "--disassemble",
    f"--arch-name={ARCHNAME}",
    f"--mcpu={CPUNAME}",
    "--syms",               # Display the symbol table
    "--symbolize-operands", # Symbolize instruction operands when disassembling
    "--show-all-symbols",   # Show all symbols during disassembly, even if multiple symbols are defined at the same location
    "--reloc",              # Display the relocation entries in the file
    "--all-headers",        # Display all available header information, relocation entries and the symbol table
    "--dynamic-syms",       # Display the contents of the dynamic symbol table
    "--show-lma",           # Display LMA column when dumping ELF section headers
    # "--full-contents",c    # Display the content of each section
    "--file-headers",       # Display the contents of the overall file header
    "--private-headers",    # Display format specific file headers
]
