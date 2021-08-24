.text
	alloc	r95, end_atomic_test
	write	"test atomic fetch-op"
	addi	r5, sp, -32
	write	"atomic base: %x64(r5)"
	ldi	r10, 5
	ldi	r12, 10
	ldi	r56, 5

	write	"test amoadd"
	amoadd4	r4, r5,r10
	amoadd8	r4, r5,r10
	amoadd4	r4, r5,r10
	amoadd8	r4, r5,r10

	write	"test amo-binary"
	amoand4	r4, r5,r10
	amoand8	r4, r5,r10
	amoor4	r4, r5,r10
	amoor8	r4, r5,r10
	amoxor4	r4, r5,r10
	amoxor8	r4, r5,r10

	write	"test amomin"
	amomin4	r4, r5,r10
	amomin8	r4, r5,r10
	amomax4	r4, r5,r10
	amomax8	r4, r5,r10

	write	"test amominu"

	amominu4	r4, r5,r10
	amominu8	r4, r5,r10
	amomaxu4	r4, r5,r10
	amomaxu8	r4, r5,r10

	write	"test cas"

	cas4	r12, r5, r56, acquire
	cas8	r12, r5, r56, acquire
	cas16	r12, r5, r56, acqrel

	write	"test load atomic"
	ld4a	r12, r5, acquire
	ld8a	r12, r5, acquire
	ld16a	r12, r5, acquire

	write	"test store atomic"
	st4a	r12, r5, release
	st8a	r12, r5, release
	st16a	r12, r5, release

end_atomic_test:
.end
