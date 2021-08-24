.text
;*****************************************************************
; ARITHMETIC
;*****************************************************************
    alloc	96
    write	"test load constant (1234567)"
    ld_imm %r1, 1234567
    write	"ldi: %i64(r1)"

    write	"test load long constant (123456789012345678)"
    ld_imm.l	%r1, 123456789012345678
    write	"ldi long: %i64(r1)"

    write	"test simple arithmetic"
    ld_imm %r1, 1
    ld_imm %r2, 2
    ld_imm %r3, 3

    write "add 1+2"
    add_i64 %r4, %r1, %r2
    write "add: %i64(r4)"

    write "add immediate 1+6"
    add_imm_i64 %r4, %r1, 6
    write "addi: %i64(r4)"

    write "sub 1-2"
    sub_i64 %r4, %r1, %r2
    write "sub: %i64(r4)"

    write "sub reverse 6-1"
    subr_imm_i64 %r4, %r1, 6
    write "sub reverse: %i64(r4)"

    write	"mul 3*4"
    ld_imm %r1, 3
    ld_imm %r2, 4
    mul_i64 %r4, %r1, %r2
    write	"mul: %i64(r4)"

    write	"12 div 4"
    ld_imm %r1, 12
    ld_imm %r2, 4
    div_i64 %r4, %r1, %r2
    write	"%i64(r4)"

    write	"15 mod 4"
    ld_imm %r1, 15
    ld_imm %r2, 4
    rem_i64 %r4, %r1, %r2
    write	"mod: %i64(r4)"

    write	"test int32_t add"
    ld_imm.l %r1, 0xFFFFFFFF
    ld_imm.l %r2, 0xFFFFFFF0
    add_i32 %r3, %r1, %r2
    write "add4: %i64(r3)"
    add_imm_i32.l %r3, %r1, 0xFFFFFFFF
    write "addis4.l: %i64(r3)"


    add_imm_i64	%r45, %r45, 12
    mov		%r54, %r56
    sub_i64		%r45, %r56, %r50
    add_imm_i64	%r45, %r55, -1000
    cmp_ne_i64 %r12, %r56, %r10
    subr_imm_i64 %r45, %r56, -10000
    subr_imm_i64 %r45, %r56, -20000
    cmp_eq_i64 %r13, %r56, %r50
    add_i64 %r45, %r56, %r50
    add_imm_i64 %r45, %r56, -10000
    mul_i64 %r45, %r56, %r50
    mul_imm_i64 %r45, %r56, -10000
    mov %r55, %r20
    ld_imm %r55, 1200
    ld_imm %r55, 987654
    ld_imm.l %r56, 98765432198765432
    add_imm_i64 %r12, %r13, -789
    cmp_ne_i64 %r14, %r13, %r77
    nand %r43, %r44, %r34
    nor %r43, %r44, %r34
    add_imm_i64 %r56, %sp, 0
    ; call %r0, quadrat
    add_i64 %r56, %sp, %sp

    ld_imm.l %r55, -9223372036854775808
    add_imm_i64  %r56, %sp, -64
    subr_imm_i64.l %r55, %r56,12345678901234567
    nor %r12, %r14, %r14
    add_imm_i64 %r56, %sp, -64
    nor %r12, %r14, %r14
    subr_imm_i64.l %r55, %r56, 12345678901234567
    add_imm_i64 %r56, %sp, -64
    subr_imm_i64.l %r55, %r56, -12345678901234567
    add_imm_i64   %r56, %sp, -64
    subr_imm_i64.l %r55, %r56, -12345678901234567
    add_imm_i64.l %r45, %r56, 12345678



    ld_imm.l %r5, 0xaFFFFFFF12345677
    ld_imm.l %r6, 0xaFFFFFFF12345678

    write "test signed overflow: %i64(r5) %i64(r6)"

    write "add overflow"
    addo_i64 %r2, %r5, %r6
    write "addo: %i64(r2)"

    write "subtract overflow"
    subo_i64 %r2, %r5, %r6
    write "subo: %i64(r2)"

    write "test unsigned add carry"
    ld_imm %r7, -1
    ld_imm %r5, -2
    ld_imm %r6, -1
    add_addc_u64 %r2, %r5, %r6, %r7
    write "addaddc: %u64(r5) %u64(r6) %u64(r7) => %i64(r2)"

    write "test unsigned subtract borrow"
    ld_imm %r7, -1
    ld_imm %r5, 12
    ld_imm %r6, -1
    sub_subb_u64 %r2, %r5, %r6, %r7
    write "subsub: %u64(r5) %u64(r6) %u64(r7) => %i64(r2)"

    mul_add   %r34, %r45, %r67, %r80
    mul_sub   %r34, %r45, %r67, %r80
    mul_subr  %r34, %r45, %r67, %r80

    add_add_i64   %r34, %r45, %r67, %r80
    add_sub_i64   %r34, %r45, %r67, %r80
    sub_sub_i64   %r34, %r45, %r67, %r80

    sext_i8  %r34, %r34
    sext_i16 %r34, %r34
    sext_i32 %r34, %r34
    sext_i64 %r34, %r34

    zext_i8  %r34, %r34
    zext_i16 %r34, %r34
    zext_i32 %r34, %r34
    zext_i64 %r34, %r34

.end
