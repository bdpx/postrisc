.text
    write	"test core mapping DEVICE_CONFIG_VIRT_BASE"
    alloc	96
    ldi.l	%r20, DEVICE_CONFIG_VIRT_BASE + CONFIG_OFFSET_CORE_0 * DEVICE_CONFIG_SPACE_SIZE ; core config
    ldi		%r19, 0xabcdef

    write	"test interrupt vector"
    std		%r19, %r20, DEVICE_CORE_TIMECMP ; use DEVICE_CORE_INTERRUPT_VECTOR in place of DEVICE_CORE_TIMECMP for real interrupt

    write	"test timecmp"
    std		%r19, %r20, DEVICE_CORE_TIMECMP

    write	"test rom mapping ROM_VIRT_BASE"
    ldi.l	%r20, ROM_VIRT_BASE
    ldwz	%r19, %r20, 0
    write	"mem[ROM_VIRT_BASE] %x64(r19)"

    write	"test video commands VIDEO_COMMAND_VIRT_BASE"
    ldi.l	%r20, VIDEO_COMMAND_VIRT_BASE
    ldi		%r21, 0x1234
    stw		%r21, %r20, 0x88	; clear
    stw		%r21, %r20, 0x8c	; redraw

    write	"video width/height base: %x64(r20)"
    ldwz	%r21, %r20, 0x80 ; width
    ldwz	%r22, %r20, 0x84 ; height
    write	"width=%i64(r21) heigth=%i64(r22)"

    write	"test video memory VIDEO_VIRT_BASE"
    ldi.l	%r20, VIDEO_VIRT_BASE
    write	"r20     %x64(r20)"

    ldi.l	%r25, 0x12345678
    stw		%r25, %r20, 0

    ldi		%r24, 0   ; y
loop_y: (64)
;	write	"%i64(r24)"
    ldi	%r23, 0   ; x
loop_x:
;	add	%r25, %r23, %r24
    stb		%r25, %r20, 0
    addi	%r20, %r20, 1
    addi	%r23, %r23, 1
    bdlt	%r23, %r21, loop_x

    addi	%r24, %r24,1
    bdlt	%r24, %r22, loop_y
    ; debug
    write	"end test video memory"
    nop		1234567
.end
