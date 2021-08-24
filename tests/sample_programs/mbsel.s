.text
	alloc	r95, end_mbsel_test
	write	"check mbsel instruction"
	ldi.l	r6, ((0x3333333333333333 ^ 0x5555555555555555) & 0xff00ff00ff00ff00) ^ 0x5555555555555555
	write	"mbsel: %x64(r6)"
	ldi.l	r3, 0x3333333333333333
	ldi.l	r4, 0x5555555555555555
	ldi.l	r5, 0xff00ff00ff00ff00
	mbsel	r6, r3, r4, r5
	write	"mbsel: %x64(r6)"
end_mbsel_test:
	write	"end_mbsel_test"
.end