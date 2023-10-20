# for windows: add --config core.autocrlf=false
git clone git@github.com:bdpx/llvm-project.git
git checkout main

# LLVM upstream
git remote add upstream git@github.com:llvm/llvm-project.git
git remote -v # check upstream
git fetch upstream

#rebase
git rebase -i upstream/main
git push --force

#################################################################
# may help if somebody force-pushed upstream
#################################################################
git update-ref -d refs/remotes/upstream/main

# add executable flag (on Windows)
git update-index --chmod=+x foo.bash

# debug:
# thread apply all bt
# ${LLVM_BUILD_BIN_DIR}/clang -v --target=postrisc -ffast-math -O2 -integrated-as -S -o foo_clang.s foo.c

gdb --args ${LLVM_BUILD_BIN_DIR}/clang -v --target=postrisc -ffast-math -O2 -integrated-as -S -o ./foo_native_asm.s ./foo.temp.c


DAG Graph description:
  * Black arrows mean data flow dependency
  * Red arrows mean glue dependency
  * Blue dashed arrows mean chain dependency


#include <llvm/Support/Debug.h>

DEBUG(dbgs() << "This is a message for debugging\n");
This will cause the message to printed to stderr only if you pass the
-debug command line flag and do a debug build.

DEBUG_WITH_TYPE("mypass", dbgs() << "This is a message for mypass\n");
If you pass the command line flag -debug-only=mypass


To know all the passes that run from IR to assembly use
llc -debug-pass=Structure add.ll

I tried all steps a while ago so searching on the internet you will also find solutions if the following steps do not go well.

You can dump the AST with
clang -Xclang -ast-dump -fsyntax-only add.c -o add.ast

IR is generated with
clang -emit-llvm -S add.c -o add.ll

Transformations from IR to MachineDAG
llc -fast-isel=false -view-dag-combine1-dags add.ll displays the DAG after being built
llc -fast-isel=false -view-legalize-dags add.ll displays the DAG before Legalization
llc -fast-isel=false -view-dag-combine2-dags add.ll displays the DAG before the second optimization pass
llc -fast-isel=false -view-isel-dags add.ll displays the DAG before the Select phase
llc -fast-isel=false -view-sched-dags add.ll displays the DAG before Scheduling
llc -fast-isel=false -view-sunit-dags add.ll displays the Scheduler’s dependency graph

To print machine instruction (MachineInstr)
llc -print-machineinstr add.ll >output >2&1

To see machine instruction after register allocation(basic, greedy, PBQP, fast)
llc add.ll -print-after=greedy -o add.s

Print MCInst as comments to the assembly
llc add.ll -asm-show-inst -o -

Binary encoding along with assembly
llc add.ll -show-mc-encoding -o -

To dump after each pass the transformation from LLVM IR to Machine specific IR(uses fast-isel)
clang -S -mllvm -print-after-all add.c 2> add.mir

Some passes, which are machine-independent, are invoked by opt.
Other passes, which are machine-dependent, are invoked by llc.
You can run llc --help-list-hidden and opt --help-list-hidden to know them.

sparc:
sp - O6
fp - I6

intrinsics: /llvm/include/llvm/IR

old stuff:
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -g -s -Wa,-mbig-obj")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O2 -g -s -Wa,-mbig-obj")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -g -s -Wa,-mbig-obj")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O2 -g -s -Wa,-mbig-obj")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -mbig-obj")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -s" )

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /DEBUG:NONE /INCREMENTAL:NO")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_RELEASE} /O1 /DEBUG:NONE")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DEBUG:NONE /INCREMENTAL:NO")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} /O1 /DEBUG:NONE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:NONE /INCREMENTAL:NO" )
set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /debug:none /INCREMENTAL:NO")
set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} /debug:none /INCREMENTAL:NO")
