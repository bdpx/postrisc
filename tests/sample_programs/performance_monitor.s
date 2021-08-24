.text
	ldi	r12, PMC_LAST
	write	"PMC_LAST = %i64(r12)"
	ldi	r12, PMC_RUNTIME
;	don't report runtine in unittests, this is non-reproducible
	mfmr	r14, r12
;	write	"PMC_RUNTIME = %i64(r14)"
	ldi	r12, PMC_SHORT_INSTRUCTION
	mfmr	r14, r12
	write	"PMC_SHORT_INSTRUCTION = %i64(r14)"
	ldi	r12, PMC_LONG_INSTRUCTION
	mfmr	r14, r12
	write	"PMC_LONG_INSTRUCTION = %i64(r14)"
	ldi	r12, PMC_NOP_INSTRUCTION
	mfmr	r14, r12
	write	"PMC_NOP_INSTRUCTION = %i64(r14)"
	ldi	r12, PMC_QUALIFIED_NOP_INSTRUCTION
	mfmr	r14, r12
	write	"PMC_QUALIFIED_NOP_INSTRUCTION = %i64(r14)"
	ldi	r12, PMC_REGISTER_SPILL
	mfmr	r14, r12
	write	"PMC_REGISTER_SPILL = %i64(r14)"
	ldi	r12, PMC_REGISTER_FILL
	mfmr	r14, r12
	write	"PMC_REGISTER_FILL = %i64(r14)"
	ldi	r12, PMC_ICACHE_HIT
	mfmr	r14, r12
	write	"PMC_ICACHE_HIT = %i64(r14)"
	ldi	r12, PMC_ICACHE_MISS
	mfmr	r14, r12
	write	"PMC_ICACHE_MISS = %i64(r14)"
	ldi	r12, PMC_DCACHE_HIT
	mfmr	r14, r12
	write	"PMC_DCACHE_HIT = %i64(r14)"
	ldi	r12, PMC_DCACHE_MISS
	mfmr	r14, r12
	write	"PMC_DCACHE_MISS = %i64(r14)"
	ldi	r12, PMC_INSTRUCTION_TRANSLATION_HIT
	mfmr	r14, r12
	write	"PMC_INSTRUCTION_TRANSLATION_HIT = %i64(r14)"
	ldi	r12, PMC_INSTRUCTION_TRANSLATION_MISS
	mfmr	r14, r12
	write	"PMC_INSTRUCTION_TRANSLATION_MISS = %i64(r14)"
	ldi	r12, PMC_DATA_TRANSLATION_HIT
	mfmr	r14, r12
	write	"PMC_DATA_TRANSLATION_HIT = %i64(r14)"
	ldi	r12, PMC_DATA_TRANSLATION_MISS
	mfmr	r14, r12
	write	"PMC_DATA_TRANSLATION_MISS = %i64(r14)"
	ldi	r12, PMC_BACKSTORE_TRANSLATION_HIT
	mfmr	r14, r12
	write	"PMC_BACKSTORE_TRANSLATION_HIT = %i64(r14)"
	ldi	r12, PMC_BACKSTORE_TRANSLATION_MISS
	mfmr	r14, r12
	write	"PMC_BACKSTORE_TRANSLATION_MISS = %i64(r14)"

	mtmr	r14, r12
	mfmr	r15, r14
	write	"old pm reg = %i64(r15)"
.end
