.text
	alloc	r95, end_ldan_test
	write	"test ldan"
	ldan	r22, r21,-1
	ldan	r21, tp, 0x4500
	write	"ldan: %x64(r21)"
	ldan	r21, r0, 0x4500
	write	"ldan: %x64(r21)"
	ldan	r21, tp, 0x4500
	write	"ldan: %x64(r21)"
	ldan	r21, tp, 0x4500
	write	"ldan: %x64(r21)"
	ldan	r21, tp, 0x10000
	write	"ldan: %x64(r21)"
end_ldan_test:
	write	"end_ldan_test"
.end