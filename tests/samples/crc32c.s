.rodata
    align 16
crc32c_test_string:
    ascii "The quick brown fox jumps over the lazy dog" ; 43 bytes
.text
    write "crc32c = 0x22620404 (expected)"
    alloc 20
    ldi %r12, -1  ; crc32c initial value
    ldard %r11, crc32c_test_string
    ldq %r13, %r11, 0
    crc32cq %r12, %r12, %r13
    ldq %r13, %r11, 16
    crc32cq %r12, %r12, %r13
    ldud %r13, %r11, 32
    crc32cd %r12, %r12, %r13
    lduh %r13, %r11, 40
    crc32ch %r12, %r12, %r13
    ldub %r13, %r11, 42
    crc32cb %r12, %r12, %r13
    xori %r12, %r12, -1
    write "crc32c = 0x%x32(r12) (computed)"
.end
