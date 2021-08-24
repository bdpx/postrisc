.rodata
    align 16
crc32c_test_string:
    ascii "The quick brown fox jumps over the lazy dog"
.text
    write "crc32c = 0x22620404 (expected)"
    alloc 20
    ld_imm %r12, -1  ; crc32c
    ld_imm %r15, 43 ; length
    mov %r14, %r15
    lda_iprel %r11, crc32c_test_string
crc32c_loop:
    ld_mia_i128 %r13, %r11, 16
    crc32c %r12, %r12, %r13, %r14
    add_imm_i64 %r14, %r14, -16
    br_lt_i64 %gz, %r14, crc32c_loop
    xor_imm %r12, %r12, -1
    write "crc32c = 0x%x32(r12) (computed)"
.end
