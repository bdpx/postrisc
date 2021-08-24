.text
;*****************************************************************
; ARITHMETIC
;*****************************************************************
    alloc  96
    write  "test load constant (1234567)"
    ldi %r1, 1234567
    write  "ldi: %i64(r1)"

    write  "test load long constant (123456789012345678)"
    ldi.l  %r1, 123456789012345678
    write  "ldi long: %i64(r1)"

    write  "test simple arithmetic"
    ldi %r1, 1
    ldi %r2, 2
    ldi %r3, 3

    write "add 1+2"
    addd %r4, %r1, %r2
    write "add: %i64(r4)"

    write "add immediate 1+6"
    addid %r4, %r1, 6
    write "addi: %i64(r4)"

    write "sub 1-2"
    subd %r4, %r1, %r2
    write "sub: %i64(r4)"

    write "sub reverse 6-1"
    subrid %r4, %r1, 6
    write "sub reverse: %i64(r4)"

    write  "mul 3*4"
    ldi %r1, 3
    ldi %r2, 4
    muld %r4, %r1, %r2
    write  "mul: %i64(r4)"

    write  "12 div 4"
    ldi %r1, 12
    ldi %r2, 4
    divsd %r4, %r1, %r2
    write  "%i64(r4)"

    write  "15 mod 4"
    ldi %r1, 15
    ldi %r2, 4
    remsd %r4, %r1, %r2
    write  "mod: %i64(r4)"

    write  "test int32_t add"
    ldi.l %r1, 0xFFFFFFFF
    ldi.l %r2, 0xFFFFFFF0
    addws %r3, %r1, %r2
    write "add4: %i64(r3)"
    addiws.l %r3, %r1, 0xFFFFFFFF
    write "addis4.l: %i64(r3)"


    addid %r45, %r45, 12
    mov %r54, %r56
    subd %r45, %r56, %r50
    addid %r45, %r55, -1000
    cmpned %r12, %r56, %r10
    subrid %r45, %r56, -10000
    subrid %r45, %r56, -20000
    cmpeqd %r13, %r56, %r50
    addd %r45, %r56, %r50
    addid  %r45, %r56, -10000
    muld %r45, %r56, %r50
    mulid %r45, %r56, -10000
    mov %r55, %r20
    ldi %r55, 1200
    ldi %r55, 987654
    ldi.l %r56, 98765432198765432
    addid  %r12, %r13, -789
    cmpned %r14, %r13, %r77
    nand %r43, %r44, %r34
    nor %r43, %r44, %r34
    addid %r56, %sp, 0
    ; call %r0, quadrat
    addd %r56, %sp, %sp

    ldi.l %r55, -9223372036854775808
    addid  %r56, %sp, -64
    subrid.l %r55, %r56,12345678901234567
    nor %r12, %r14, %r14
    addid %r56, %sp, -64
    nor %r12, %r14, %r14
    subrid.l %r55, %r56, 12345678901234567
    addid %r56, %sp, -64
    subrid.l %r55, %r56, -12345678901234567
    addid   %r56, %sp, -64
    subrid.l %r55, %r56, -12345678901234567
    addid.l %r45, %r56, 12345678



    ldi.l %r5, 0xaFFFFFFF12345677
    ldi.l %r6, 0xaFFFFFFF12345678

    write "test signed overflow: %i64(r5) %i64(r6)"

    write "add overflow"
    addod %r2, %r5, %r6
    write "addo: %i64(r2)"

    write "subtract overflow"
    subod %r2, %r5, %r6
    write "subo: %i64(r2)"

    muladdd   %r34, %r45, %r67, %r80
    mulsubd   %r34, %r45, %r67, %r80
    mulsubrd  %r34, %r45, %r67, %r80

    addaddd   %r34, %r45, %r67, %r80
    addsubd   %r34, %r45, %r67, %r80
    subsubd   %r34, %r45, %r67, %r80

    sextb %r34, %r34
    sexth %r34, %r34
    sextw %r34, %r34
    sextd %r34, %r34

    zextb %r34, %r34
    zexth %r34, %r34
    zextw %r34, %r34
    zextd %r34, %r34

.end
