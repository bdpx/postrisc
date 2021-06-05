
// common file for c and bash
// logging flags may be used for command line mask

X(LOG_ALWAYS            , 0x00000001)  // always log
X(LOG_TOKENIZER         , 0x00000002)  // assembler tokenizer
X(LOG_PARSER            , 0x00000004)  // assembler parser
X(LOG_EVALUATE          , 0x00000008)  // assembler evaluator
X(LOG_EMULATOR          , 0x00000010)  // common emulator
X(LOG_DECODER           , 0x00000020)  // instructions decoding
X(LOG_LOADER            , 0x00000040)  // raw/elf loader
X(LOG_PREDICATION       , 0x00000080)  // predicated execution
X(LOG_REGISTER_ROTATION , 0x00000100)  // register rotation
X(LOG_REGISTER_STACK    , 0x00000200)  // register backing store
X(LOG_FPU               , 0x00000400)  // floating point unit
X(LOG_EXCEPTION         , 0x00000800)  // software traps, hardware interruptions/interrupts
X(LOG_LOAD              , 0x00001000)  // memory load access
X(LOG_STORE             , 0x00002000)  // memory store access
X(LOG_ITLB              , 0x00004000)  // instruction TLB
X(LOG_ICACHE            , 0x00008000)  // instruction cache
X(LOG_PAGETABLE         , 0x00010000)  // in-memory page table
X(LOG_PLATFORM          , 0x00020000)  // physical memory emulator, devices etc
X(LOG_CALLSTACK         , 0x00040000)  // call/return/alloc/enter/leave
X(LOG_DTLB              , 0x00080000)  // data TLB
X(LOG_DCACHE            , 0x00100000)  // data cache
X(LOG_SYSCALL           , 0x00200000)  // system calls/returns
X(LOG_DEBUGGER          , 0x00400000)  // internal debugger
X(LOG_REGISTER_DATA     , 0x00800000)  // register content
X(LOG_BRANCH            , 0x01000000)  // taken/untaken branches, ip monitoring
X(LOG_INTERRUPT         , 0x02000000)  // interrupts, IPI device
X(LOG_DISPLAY           , 0x04000000)  // video device
X(LOG_SERIALIZATION     , 0x08000000)  // serialization
X(LOG_DUMP              , 0x10000000)  // dumping state
X(LOG_INSN_TRACE        , 0x20000000)  // bundle/instruction tracing
X(LOG_DOOM              , 0x40000000)  // doom emulator
