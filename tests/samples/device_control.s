
.text
    write	"test control device memory-mapped registers"
    alloc	96

    ; device_control base address
    ldi.l	%r24, DEVICE_CONFIG_VIRT_BASE

    write	"test pci"

    ldi.l	%r21, 0x1234567890abcdef

    lddz	%r20, %r24, DEVICE_CONTROL_DID
    write	"mem[DEVICE_CONTROL_DID] %x64(r20)"
    std		%r21, %r24, DEVICE_CONTROL_DID
    lddz	%r20, %r24, DEVICE_CONTROL_DID
    write	"mem[DEVICE_CONTROL_DID] %x64(r20)"

    lddz	%r20, %r24, DEVICE_CONTROL_CMD
    write	"mem[DEVICE_CONTROL_CMD] %x64(r20)"
    std		%r21, %r24, DEVICE_CONTROL_CMD
    lddz	%r20, %r24, DEVICE_CONTROL_CMD
    write	"mem[DEVICE_CONTROL_CMD] %x64(r20)"

    lddz	%r20, %r24, DEVICE_CONTROL_ARRAY_ADDRESS
    write	"mem[DEVICE_CONTROL_ARRAY_ADDRESS] (r20)"

    lddz	%r20, %r24, DEVICE_CONTROL_ARRAY_LEN
    write	"mem[DEVICE_CONTROL_ARRAY_LEN] %i64(r20)"

    ldi	%r22, '\n'

    write	"test command"
    ldi.l	%r21, 0xabcdef1234567890
    std		%r21, %r24, DEVICE_CONTROL_CMD

    write	"end_device_control_test"
.end

