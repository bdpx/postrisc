![Linux](https://github.com/bdpx/postrisc/actions/workflows/linux.yml/badge.svg?branch=main)
![Windows](https://github.com/bdpx/postrisc/actions/workflows/windows_msys2.yml/badge.svg?branch=main)

# POSTRISC virtual processor

## What it is:
This is a virtual processor implementation: assembler, disassembler, emulator etc.

## ISA features:
* Fixed fetch format with 128-bit instruction bundles (2-bit template and 3 x 42-bit instructions, or 42+84, or 84+42).
* Non-destructive instruction formats.
* Ternary fused operations.
* Load/store architecture with aligned access only.
* Various addressing modes:
  * ip-relative,
  * base + displacement,
  * base + scaled index + displacement,
  * base with pre/post base update.
* Fused compare and branch in one instruction:
  * compare registers and branch,
  * compare register and immediate and branch,
  * compare bit and branch,
  * compare with bitmask and branch if all/none/any set/clear.
* Add-compare-branch in one instruction for loops.
* Implicit predication (via nullification of the future instructions), up to 32 instructions in if-else blocks.
* 128 x 128-bit unified (logical/integer/address/float/vector) general purpose register file.
* Rotating register file windows - 120 local (stacked, rotated) and 8 global registers.
* Secured backing store (shadow stack) for local registers with limited access for register fill/spill only (backstore page access type).
* Various special registers: ip, psr, pta ...
* Virtual memory with 4,8,16,32,64 KiB pages and 2,3,4 levels of top-down indexing.
* RWXBF page access model: read,write,execute,backstore,finalize.
  * Standard RWX page rights have extended with additional B (the backing store of register windows).
  * Additional F page flag (finalized) disables protection changes and copy on write, hardware visible analogue of the software SEC_NO_CHANGES.
  * Secured instructions for call via vtable/PLT tables (load address and check the finalized state of page).
* FPU with 16,32,64,128 bit IEEE floating point support, fused multiply-add (fma), explicit rounding for conversion.
* SIMD integer instructions and packed floating point instructions.
* Programmable interrupt controller.
* Weak memory model, atomic instructions: acquired loads, released stores, various RMW (add, and, or, xor, min, max), CAS, fences.
* Physical memory - bus with mapped devices: memory banks, rom, pci-like devices, video memory, etc.
* Extended device config space for each device, including processor cores. IPI via core config space writes.
* Hardware assisted zero-cost software exceptions (without table based runtime).
* Debug/monitoring facilities, performance counters.

project site: [https://bdpx.github.io/postrisc](https://bdpx.github.io/postrisc)

## other Postrisc projects:

LLVM compiler backend: [https://github.com/bdpx/llvm-project](https://github.com/bdpx/llvm-project)

musl libc port: [https://github.com/bdpx/musl](https://github.com/bdpx/musl)

Doom-1 port: [https://github.com/bdpx/postrisc_doom](https://github.com/bdpx/postrisc_doom)
