.text
;*****************************************************************
; ARITHMETIC
;*****************************************************************
    alloc	96
    write	"test load constant (1234567)"
    ldi		%r1, 1234567
    write	"ldi: %i64(r1)"

    write	"test load long constant (123456789012345678)"
    ldi.l	%r1, 123456789012345678
    write	"ldi long: %i64(r1)"

    write	"test simple arithmetic"
    ldi		%r1, 1
    ldi		%r2, 2
    ldi		%r3, 3

    write	"add 1+2"
    add		%r4, %r1, %r2
    write	"add: %i64(r4)"

    write	"add immediate 1+6"
    addi	%r4, %r1, 6
    write	"addi: %i64(r4)"

    write	"sub 1-2"
    sub		%r4, %r1, %r2
    write	"sub: %i64(r4)"

    write	"sub from immediate 6-1"
    subfi	%r4, %r1, 6
    write	"subfi: %i64(r4)"

    write	"mul 3*4"
    ldi		%r1, 3
    ldi		%r2, 4
    mul		%r4, %r1, %r2
    write	"mul: %i64(r4)"

    write	"12 div 4"
    ldi		%r1, 12
    ldi		%r2, 4
    div		%r4, %r1, %r2
    write	"%i64(r4)"

    write	"15 mod 4"
    ldi		%r1, 15
    ldi		%r2, 4
    mod		%r4, %r1, %r2
    write	"mod: %i64(r4)"

    write	"test int32_t add"
    ldi.l	%r1, 0xFFFFFFFF
    ldi.l	%r2, 0xFFFFFFF0
    addw	%r3, %r1, %r2
    write	"add4: %i64(r3)"
    addiws.l	%r3, %r1, 0xFFFFFFFF
    write	"addis4.l: %i64(r3)"


    addi	%r45, %r45, 12
    mov		%r54, %r56
    sub		%r45, %r56, %r50
    addi	%r45, %r55, -1000
    cmpdne	%r12, %r56, %r10
    subfi	%r45, %r56, -10000
    subfi	%r45, %r56, -20000
    cmpdeq	%r13, %r56, %r50
    add		%r45, %r56, %r50
    addi	%r45, %r56, -10000
    mul		%r45, %r56, %r50
    muli	%r45, %r56, -10000
    mov		%r55, %r20
    ldi		%r55, 1200
    ldi		%r55, 987654
    ldi.l	%r56, 98765432198765432
    addi	%r12, %r13, -789
    cmpdne	%r14, %r13, %r77
    nand	%r43, %r44, %r34
    nor		%r43, %r44, %r34
    addi	%r56, %sp, 0
    ; callr	%r0, quadrat
    add		%r56, %sp, %sp

    ldi.l	%r55, -9223372036854775808
    addi	%r56, %sp, -64
    subfi.l	%r55, %r56,12345678901234567
    nor		%r12, %r14, %r14
    addi	%r56, %sp, -64
    nor		%r12, %r14, %r14
    subfi.l	%r55, %r56, 12345678901234567
    addi	%r56, %sp, -64
    subfi.l	%r55, %r56, -12345678901234567
    addi	%r56, %sp, -64
    subfi.l	%r55, %r56, -12345678901234567
    addi.l	%r45, %r56, 12345678



    ldi.l	%r5, 0xaFFFFFFF12345677
    ldi.l	%r6, 0xaFFFFFFF12345678

    write	"test signed overflow: %i64(r5) %i64(r6)"

    write	"add overflow"
    addo	%r2, %r5, %r6
    write	"addo: %i64(r2)"

    write	"subtract overflow"
    subo	%r2, %r5, %r6
    write	"subo: %i64(r2)"

    write	"test unsigned add carry"
    ldi		%r7, -1
    ldi		%r5, -2
    ldi		%r6, -1
    addaddc	%r2, %r5, %r6, %r7
    write	"addaddc: %u64(r5) %u64(r6) %u64(r7) => %i64(r2)"

    write	"test unsigned subtract borrow"
    ldi		%r7, -1
    ldi		%r5, 12
    ldi		%r6, -1
    subsubb	%r2, %r5, %r6, %r7
    write	"subsub: %u64(r5) %u64(r6) %u64(r7) => %i64(r2)"

    muladd	%r34, %r45, %r67, %r80
    mulsub	%r34, %r45, %r67, %r80
    mulsubf	%r34, %r45, %r67, %r80
    addadd	%r34, %r45, %r67, %r80
    addsub	%r34, %r45, %r67, %r80
    subsub	%r34, %r45, %r67, %r80

.end
