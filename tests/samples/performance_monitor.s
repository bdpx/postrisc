.text
    alloc 21
    ld_imm %r12, PMC_LAST
    write "PMC_LAST = %i64(r12)"
; don't report runtine in unittests, this is non-reproducible
    get_mr %r14, %gz, PMC_RUNTIME
;   write "PMC_RUNTIME = %i64(r14)"
    get_mr %r14, %gz, PMC_SHORT_INSTRUCTION
    write "PMC_SHORT_INSTRUCTION = %i64(r14)"
    get_mr %r14, %gz, PMC_LONG_INSTRUCTION
    write "PMC_LONG_INSTRUCTION = %i64(r14)"
    get_mr %r14, %gz, PMC_SHADOWED_SLOT
    write "PMC_SHADOWED_SLOT = %i64(r14)"
    get_mr %r14, %gz, PMC_NOP_INSTRUCTION
    write "PMC_NOP_INSTRUCTION = %i64(r14)"
    get_mr %r14, %gz, PMC_QUALIFIED_NOP_INSTRUCTION
    write "PMC_QUALIFIED_NOP_INSTRUCTION = %i64(r14)"
    get_mr %r14, %gz, PMC_REGISTER_SPILL
    write "PMC_REGISTER_SPILL = %i64(r14)"
    get_mr %r14, %gz, PMC_REGISTER_FILL
    write "PMC_REGISTER_FILL = %i64(r14)"
    get_mr %r14, %gz, PMC_ICACHE_HIT
    write "PMC_ICACHE_HIT = %i64(r14)"
    get_mr %r14, %gz, PMC_ICACHE_MISS
    write "PMC_ICACHE_MISS = %i64(r14)"
    get_mr %r14, %gz, PMC_DCACHE_HIT
    write "PMC_DCACHE_HIT = %i64(r14)"
    get_mr %r14, %gz, PMC_DCACHE_MISS
    write "PMC_DCACHE_MISS = %i64(r14)"
    get_mr %r14, %gz, PMC_INSTRUCTION_TRANSLATION_HIT
    write "PMC_INSTRUCTION_TRANSLATION_HIT = %i64(r14)"
    get_mr %r14, %gz, PMC_INSTRUCTION_TRANSLATION_MISS
    write "PMC_INSTRUCTION_TRANSLATION_MISS = %i64(r14)"
    get_mr %r14, %gz, PMC_DATA_TRANSLATION_HIT
    write "PMC_DATA_TRANSLATION_HIT = %i64(r14)"
    get_mr %r14, %gz, PMC_DATA_TRANSLATION_MISS
    write "PMC_DATA_TRANSLATION_MISS = %i64(r14)"
    get_mr %r14, %gz, PMC_BACKSTORE_TRANSLATION_HIT
    write "PMC_BACKSTORE_TRANSLATION_HIT = %i64(r14)"
    get_mr %r14, %gz, PMC_BACKSTORE_TRANSLATION_MISS
    write "PMC_BACKSTORE_TRANSLATION_MISS = %i64(r14)"
    set_mr %r14, %gz, PMC_SHORT_INSTRUCTION
    get_mr %r15, %gz, PMC_SHORT_INSTRUCTION
    write "old pm reg = %i64(r15)"
.end
