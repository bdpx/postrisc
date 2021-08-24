
.text
    write "test control device memory-mapped registers"
    alloc 96

    ; device_control base address
    ld_imm.l %r24, DEVICE_CONFIG_VIRT_BASE

    write "test pci"

    ld_imm.l %r21, 0x1234567890abcdef

    ld_u64 %r20, %r24, DEVICE_CONTROL_DID
    write "mem[DEVICE_CONTROL_DID] %x64(r20)"
    st_i64 %r21, %r24, DEVICE_CONTROL_DID
    ld_u64 %r20, %r24, DEVICE_CONTROL_DID
    write "mem[DEVICE_CONTROL_DID] %x64(r20)"

    ld_u64 %r20, %r24, DEVICE_CONTROL_CMD
    write "mem[DEVICE_CONTROL_CMD] %x64(r20)"
    st_i64 %r21, %r24, DEVICE_CONTROL_CMD
    ld_u64 %r20, %r24, DEVICE_CONTROL_CMD
    write "mem[DEVICE_CONTROL_CMD] %x64(r20)"

    ld_u64 %r20, %r24, DEVICE_CONTROL_ARRAY_ADDRESS
    write "mem[DEVICE_CONTROL_ARRAY_ADDRESS] (r20)"

    ld_u64 %r20, %r24, DEVICE_CONTROL_ARRAY_LEN
    write "mem[DEVICE_CONTROL_ARRAY_LEN] %i64(r20)"

    ld_imm  %r22, '\n'

    write "test command"
    ld_imm.l %r21, 0xabcdef1234567890
    st_i64 %r21, %r24, DEVICE_CONTROL_CMD

    write "end_device_control_test"
.end
