.rodata
    align 16
crc32c_test_string:
    ascii	"The quick brown fox jumps over the lazy dog"
.text
    write	"crc32c = 0x22620404 (expected)"
    alloc	20
    ldi		%r12, -1  ; crc32c
    ldi		%r15, 43 ; length
    mov		%r14, %r15
    ca.rf	%r11, crc32c_test_string
crc32c_loop:
    ld.q.mia	%r13, %r11, 16
    crc32c	%r12, %r12, %r13, %r14
    addi	%r14, %r14, -16
    bs.gt.d	%r14, %gz, crc32c_loop
    xori	%r12, %r12, -1
    write	"crc32c = 0x%x32(r12) (computed)"
.end
