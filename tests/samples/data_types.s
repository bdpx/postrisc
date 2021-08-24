.text
    alloc 96
    add_imm_i64 %r20, %gz, 128
    add_imm_i64 %sp, %sp, -32
    ld_imm.l %r12, 0x07060504030201
    st_i64 %r12, %sp,0

.data
    ascii "data section marker"
    align 8
.rodata
    ascii "rodata section marker"
    align 8

.data
    d2 1234
first_byte:
    d1 12
.text
    lda_iprel %r22, first_byte

; test interval time mask
    ld_imm %r22, 0xFFFFFFFFFFFFFFFF
    ld_imm %r15, 11

.rodata	; open rodata (read-only data) section
    align 8
text_begin: ; this is label
    d8 1 ; signed 8-bytes
    d8 -2
    d1 101 ; signed byte
    d1 102
    d1 103
    align 4
    d4 10000 ; signed 4byte
    d2 10000 ; signed 2byte
    space 4 ; insert zeroed bytes
    d2 20000
.data	; open data (read-write) section
    align 8
eexxx: d8 12345678 ; signed 8-byte
    d8 1234567890
ssxxx: d8 123456789012
    d8 12345678901234
.rodata
    d4 4555 ; signed 4-byte
    d2 4555 ; signed 2-byte
    align 8
    d8 11
text2:
.text ; open code (read-execute) section

.data ; switch to data section
    d1 120
    align 2
    d2 13400
align 8
dataname:
    d4 654321890
    d4 654321890
    d8 1234545345345
    d8 6789023356977
align 8
someplaceindata:
    d8 0x0000000000000001
    d8 0x0000000000000002
    d8 0x0000000000000003
    d8 0x0000000000000004
    d8 0x0000000000000005
    d8 0x0000000000000006
    d8 0x0000000000000007
    d8 0x0000000000000008
.text
    lda_iprel %r11, someplaceindata
    ld_imm.l %r15, 987777777777
    ld_imm %r46, 100000
    st_i64 %r46, %r11, 8*3
    ld_u64 %r46, %r11, 8*3
    write "%i64(r46)"
    mul_i64 %r18, %r15, %r46
    add_i64 %r17, %r15, %r46
    andn %r17, %r15, %r46
    cmp_lt_i64 %r12, %r17, %r15
    write "%i64(r15) %i64(r46) %i64(r17)"
    add_imm_i64 %r17, %r17, 22
    write "%i64(r17) %i64(r17)"
    get_spr %r27, %itc
    write "itc: %x64(r27)"
    write "%m(dump)"
.end
