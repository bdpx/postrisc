![cmake-main](https://github.com/bdpx/postrisc/actions/workflows/cmake.yml/badge.svg?branch=main)

# POSTRISC virtual processor

## What it is:
This is a virtual processor implementation: assembler, disassembler, emulator etc.

## hardware features
- fixed fetch format with 128-bit instruction bundles (2-bit template and 3 x 42-bit instructions, or 42+84, or 84+42)
- non-destructive instruction formats
- ternary fused operations
- load/store architecture with aligned access only
- addressing: IP-relative, base+displacement, base+scaled index+displacement
- 128 x 128-bit unified (logical/integer/address/float/vector) general purpose register file
- the register file rotation (112 stacked and 16 global registers)
- secured register stack with no access via the standard store instructions, only fill/spill via the register rotation hardware
- various special registers: ip, psr, pta ...
- implicit guard predication (via future instructions masking)
- virtual memory with 4,8,16,32,64 KiB pages and 2,3,4 levels of top-down indexing
- standard RWX page rights have extended with additional B (the backing store of register windows)
- additional F page flag (finalized) disables protection changes and copy on write, hardware visible analogue of the software SEC_NO_CHANGES
- secured instructions for call via vtable/PLT tables (load address and check the finalized state of page)
- FPU with 32,64,128 bit IEEE floating point support, fused multiply-add (fma)
- SIMD integer instructions and packed floating point instructions
- programmable interrupt controller
- atomic instructions: acquired loads, released stores, various RMW, CAS
- physical memory - bus with mapped devices: memories, rom, ipi, video memory, etc
- hardware assisted zero-cost software exceptions (without table based runtime)

project site: [https://bdpx.github.io/postrisc/](https://bdpx.github.io/postrisc)