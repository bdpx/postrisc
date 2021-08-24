.text
	write	"test bit-field insert (deposit)"
	alloc	r95, end_deposit_test
	ldi.l	r30, 0xaaaaaaaaaaaaaaaa
	ldi.l	r40, 0xeeeeeeeeeeeeeeee
	dep	r20, r30, r40, 48, 24
	write	"dep: %x64(r20)"
	dep	r20, r40, r30, 48, 24
	write	"dep: %x64(r20)"

	write	"test vector deposit (dep16)"
	nor	r3, r4, r4
	dep16	r5, r3, r4, 100
	write	"dep16: %x128(r5)"
end_deposit_test:
	write	"end deposit test"
.end
