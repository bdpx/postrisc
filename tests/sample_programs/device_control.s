
.rodata
	align	16
console_test_quad:
	quad	1.189731495357231765085759326628007e+4932
        quad	1.23456789 + 32.0
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
	write	"test control device memory-mapped registers"
	alloc	r95, end_device_control_test

	; device_control base address
	ldi.l	r24, IPI_VIRT_BASE

	write	"test pci"

	ldi.l	r21, 0x1234567890abcdef

	ld8	r20, r24, DEVICE_CONTROL_DID
	write	"mem[DEVICE_CONTROL_DID] %x64(r20)"
	st8	r21, r24, DEVICE_CONTROL_DID
	ld8	r20, r24, DEVICE_CONTROL_DID
	write	"mem[DEVICE_CONTROL_DID] %x64(r20)"

	ld8	r20, r24, DEVICE_CONTROL_CMD
	write	"mem[DEVICE_CONTROL_CMD] %x64(r20)"
	st8	r21, r24, DEVICE_CONTROL_CMD
	ld8	r20, r24, DEVICE_CONTROL_CMD
	write	"mem[DEVICE_CONTROL_CMD] %x64(r20)"

	ld8	r20, r24, DEVICE_CONTROL_ARRAY_ADDRESS
	write	"mem[DEVICE_CONTROL_ARRAY_ADDRESS] (r20)"

	ld8	r20, r24, DEVICE_CONTROL_ARRAY_LEN
	write	"mem[DEVICE_CONTROL_ARRAY_LEN] %i64(r20)"

	ldi	r22, '\n'

	write	"test command"
	ldi.l	r21, 0xabcdef1234567890
	st8	r21, r24, DEVICE_CONTROL_CMD

end_device_control_test:
	write	"end_device_control_test"
.end

