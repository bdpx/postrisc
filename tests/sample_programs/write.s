.text
	write	"ip=%s(ip), eip=%s(eip), psr=%s(psr)"

	write	"end test write special regs"

	write	"\ntest write: general register"

	write	"%%i8(sp)  = %i8(sp)"
	write	"%%i16(sp) = %i16(sp)"
	write	"%%i32(sp) = %i32(sp)"
	write	"%%i64(sp) = %i64(sp)"
	write	"%%u8(sp)  = %u8(sp)"
	write	"%%u16(sp) = %u16(sp)"
	write	"%%u32(sp) = %u32(sp)"
	write	"%%u64(sp) = %u64(sp)"
	write	"%%x8(sp)  = 0x%x8(sp)"
	write	"%%x16(sp) = 0x%x16(sp)"
	write	"%%x32(sp) = 0x%x32(sp)"
	write	"%%x64(sp) = 0x%x64(sp)"

	write	"%x64(r0)"
	write	"%x64(r1)"
	write	"%x64(r2)"
	write	"%x64(r22)"
	write	"%x64(r33)"
	write	"%x64(g0)"
	write	"%x64(g1)"
	write	"%x64(g2)"
	write	"%x64(sp)"
	write	"%x64(fp)"

	write	"end test write general regs"
.end