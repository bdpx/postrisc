.rodata
    align	16
console_test_quad:
    quad	1.189731495357231765085759326628007e+4932
console_test_quad2:
    quad	1.23456789 + 32.0
console_test_quad3:
    quad	0.2345678901234567890123456789012345678 + 0.2
    quad	2*asin(1)
    quad	255
console_test_double:
    double	acos(sin(3.1415926)) ;-1.2345678e+200
    double	444.689679
console_test_float:
    float	0.123456789123456789e+30
    float	2.123456789122233
    float	0.0
    float	1.0
.text
    alloc	35
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
    write	"%x64(tp)"
    write	"%x64(sp)"

    write	"end test write general regs"

    ld_iprel_i128 %r22, console_test_quad
    write "r22 = %x128(r22) %f128(r22)"
    ld_iprel_i128 %r22, console_test_quad2
    write "r22 = %x128(r22) %f128(r22)"
    ld_iprel_i128 %r22, console_test_quad3
    write "r22 = %x128(r22) %f128(r22)"
    ld_iprel_u64 %r22, console_test_double
    write "r22 = %x64(r22) %f64(r22)"
    ld_iprel_u32 %r22, console_test_float
    write "r22 = %x32(r22) %f32(r22)"

    write "end test write fp regs"
.end
