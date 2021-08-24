.text
    write "test core mapping DEVICE_CONFIG_VIRT_BASE"
    alloc 96
    ld_imm.l %r20, DEVICE_CONFIG_VIRT_BASE
    write "DEVICE_CONFIG_VIRT_BASE: %x64(r20)"
    ld_imm.l %r20, DEVICE_CONFIG_SPACE_SIZE
    write "DEVICE_CONFIG_SPACE_SIZE: %x64(r20)"
    ld_imm.l %r20, CONFIG_OFFSET_CORE_0
    write "CONFIG_OFFSET_CORE_0: %x64(r20)"
    ld_imm.l %r20, DEVICE_CORE_TIMECMP
    write "DEVICE_CORE_TIMECMP: %x64(r20)"

    ld_imm.l %r20, DEVICE_CONFIG_VIRT_BASE + CONFIG_OFFSET_CORE_0 * DEVICE_CONFIG_SPACE_SIZE ; core config
    ld_imm %r19, 0xabcdef

    write "test interrupt vector %x64(r20)"
    st_i64 %r19, %r20, DEVICE_CORE_TIMECMP ; use DEVICE_CORE_INTERRUPT_VECTOR in place of DEVICE_CORE_TIMECMP for real interrupt

    write "test timecmp"
    st_i64 %r19, %r20, DEVICE_CORE_TIMECMP

    write "test rom mapping ROM_VIRT_BASE"
    ld_imm.l %r20, ROM_VIRT_BASE
    ld_u64 %r19, %r20, 0
    write "mem[ROM_VIRT_BASE] %x64(r19)"

    write "test video commands VIDEO_COMMAND_VIRT_BASE"
    ld_imm.l %r20, VIDEO_COMMAND_VIRT_BASE
    ld_imm %r21, 0x1234
    st_i32 %r21, %r20, 0x88 ; clear
    st_i32 %r21, %r20, 0x8c ; redraw

    write "video width/height base: %x64(r20)"
    ld_u32 %r21, %r20, 0x80 ; width
    ld_u32 %r22, %r20, 0x84 ; height
    write "width=%i64(r21) heigth=%i64(r22)"

    write "test video memory VIDEO_VIRT_BASE"
    ld_imm.l %r20, VIDEO_VIRT_BASE
    write "r20     %x64(r20)"

    ld_imm.l %r25, 0x12345678
    st_i32 %r25, %r20, 0

    ld_imm %r24, 0   ; y
loop_y: (64)
; write "%i64(r24)"
    ld_imm %r23, 0   ; x
loop_x:
; add %r25, %r23, %r24
    st_i8 %r25, %r20, 0
    add_imm_i64 %r20, %r20, 1
    add_imm_i64 %r23, %r23, 1
    br_lt_i64 %r23, %r21, loop_x

    add_imm_i64 %r24, %r24,1
    br_lt_i64 %r24, %r22, loop_y
    ; debug
    write "end test video memory"
    nop 1234567
.end
