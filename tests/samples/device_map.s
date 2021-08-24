.text
    write	"test core mapping DEVICE_CONFIG_VIRT_BASE"
    alloc	96
    ldi.l	%r20, DEVICE_CONFIG_VIRT_BASE
    write	"DEVICE_CONFIG_VIRT_BASE: %x64(r20)"
    ldi.l	%r20, DEVICE_CONFIG_SPACE_SIZE
    write	"DEVICE_CONFIG_SPACE_SIZE: %x64(r20)"
    ldi.l	%r20, CONFIG_OFFSET_CORE_0
    write	"CONFIG_OFFSET_CORE_0: %x64(r20)"
    ldi.l	%r20, DEVICE_CORE_TIMECMP
    write	"DEVICE_CORE_TIMECMP: %x64(r20)"

    ldi.l	%r20, DEVICE_CONFIG_VIRT_BASE + CONFIG_OFFSET_CORE_0 * DEVICE_CONFIG_SPACE_SIZE ; core config
    ldi		%r19, 0xabcdef

    write	"test interrupt vector %x64(r20)"
    st.d	%r19, %r20, DEVICE_CORE_TIMECMP ; use DEVICE_CORE_INTERRUPT_VECTOR in place of DEVICE_CORE_TIMECMP for real interrupt

    write	"test timecmp"
    st.d	%r19, %r20, DEVICE_CORE_TIMECMP

    write	"test rom mapping ROM_VIRT_BASE"
    ldi.l	%r20, ROM_VIRT_BASE
    ldz.d	%r19, %r20, 0
    write	"mem[ROM_VIRT_BASE] %x64(r19)"

    write	"test video commands VIDEO_COMMAND_VIRT_BASE"
    ldi.l	%r20, VIDEO_COMMAND_VIRT_BASE
    ldi		%r21, 0x1234
    st.w	%r21, %r20, 0x88	; clear
    st.w	%r21, %r20, 0x8c	; redraw

    write	"video width/height base: %x64(r20)"
    ldz.w	%r21, %r20, 0x80 ; width
    ldz.w	%r22, %r20, 0x84 ; height
    write	"width=%i64(r21) heigth=%i64(r22)"

    write	"test video memory VIDEO_VIRT_BASE"
    ldi.l	%r20, VIDEO_VIRT_BASE
    write	"r20     %x64(r20)"

    ldi.l	%r25, 0x12345678
    st.w	%r25, %r20, 0

    ldi		%r24, 0   ; y
loop_y: (64)
;	write	"%i64(r24)"
    ldi	%r23, 0   ; x
loop_x:
;	add	%r25, %r23, %r24
    st.b	%r25, %r20, 0
    addi	%r20, %r20, 1
    addi	%r23, %r23, 1
    bs.lt.d	%r23, %r21, loop_x

    addi	%r24, %r24,1
    bs.lt.d	%r24, %r22, loop_y
    ; debug
    write	"end test video memory"
    nop		1234567
.end
