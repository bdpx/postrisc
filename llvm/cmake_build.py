#!/usr/bin/python3

import builtins, os, shutil, subprocess, sys, multiprocessing, math, platform

def cmakeBuild(buildType, buildDir):
    num_cores = math.trunc(multiprocessing.cpu_count())
    print(f"build: buildType={buildType}, buildDir={buildDir}, {num_cores} jobs")
    BUILD_DIR=f"..{os.sep}..{os.sep}{buildDir}"
    with open(f"out_build-{buildType}.txt", "w") as outfile:
        subprocess.run(["cmake", "--build", BUILD_DIR, "-j", f"{num_cores}"],
            stderr=outfile, text=True, check=True)

def main():
    buildType = "debug"
    if len(sys.argv) > 1:
        buildType = sys.argv[1]

    buildType = buildType.lower()

    if buildType == "release":
        cmakeBuild("Release", "llvm-build-release")
    else:
        cmakeBuild("Debug", "llvm-build-debug")

if __name__ == "__main__":
    main()
