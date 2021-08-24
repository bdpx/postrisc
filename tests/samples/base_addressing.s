.text
.data
data_lbl:
    d1	25
    d1	26
    d1	27
    d1	28

.text
program_start:
; Here we test references to data section.
; Absolute offset from begin of section
    write	"base addressing"
    alloc	96
    ldar	%r17, program_start
    ldi		%r12, data_lbl
    write	"data_lbl: %i64(r12)"

    ldi		%r12, data_hi(data_lbl)
    write	"data_hi(data_lbl): %i64(r12)"
    ldi		%r12, data_lo(data_lbl)
    write	"data_lo(data_lbl): %i64(r12)"
    ldafr	%r13, data_lbl
    write	"ldafr(data_lbl): %x64(r13)"
    ldafr.l	%r13, data_lbl
    write	"ldafr(data_lbl): %x64(r13)"

    addi	%r13, %r17, data_hi(data_lbl)
    write	"r13     %i64(r13)"
    addi	%r14, %r13, data_lo(data_lbl)+0
    write	"r14     %i64(r14)"

    addi	%r13, %r17, data_hi(data_lbl)
    write	"r13     %i64(r13)"
    ldbz	%r25, %r13, data_lo(data_lbl)+0
    ldbz	%r26, %r13, data_lo(data_lbl)+1
    ldbz	%r27, %r13, data_lo(data_lbl)+2
    ldbz	%r28, %r13, data_lo(data_lbl)+3
    write	"r25     %i64(r25)" ; must be 25
    write	"r26     %i64(r26)" ; must be 26
    write	"r27     %i64(r27)" ; must be 27
    write	"r28     %i64(r28)" ; must be 28

; test load context
    lddz	%r1, %sp, -16
    std		%r1, %sp, -16
    jmp		skipaddr
    jmp.l	skipaddr

; test indexed load/store
    stbx	%r12, %r15, %r30, 4, 14
    sthx	%r12, %r15, %r30, 4, 14
    stwx	%r12, %r15, %r30, 4, 14
    stdx	%r12, %r15, %r30, 4, 14

    ldaq.relaxed %r30, %r56
    staq.relaxed %r43, %r56

    sladd	%r43, %r56, %r23, 4
    slsub	%r43, %r56, %r23, 42
    slsubf	%r43, %r56, %r23, 12

    ldwz	%r30, %r5, 66*4	; load mid
    lddzx	%r40, %tp, %r30, 0, 4	; load base

    lddsx	%r12, %r23, %r40, 3, 114
    lddsx	%r12, %r23, %r40, 3, 114
    lddzx	%r12, %r23, %r40, 3, 114
    lddzx	%r12, %r23, %r40, 3, 114
    stwx	%r12, %r23, %r40, 3, 114
    stdx	%r12, %r23, %r40, 3, 114

    ldbsx	%r12, %r23, %r40, 3, 114
    ldbsx	%r12, %r23, %r40, 3, 114
    ldbzx	%r12, %r23, %r40, 3, 114
    ldbzx	%r12, %r23, %r40, 3, 114
    stbx	%r12, %r23, %r40, 3, 114
    stbx	%r12, %r23, %r40, 3, 114

    ldhsx	%r12, %r23, %r40, 3, 114
    ldhsx	%r12, %r23, %r40, 3, 114
    ldhzx	%r12, %r23, %r40, 3, 114
    ldhzx	%r12, %r23, %r40, 3, 114
    sthx	%r12, %r23, %r40, 3, 114
    sthx	%r12, %r23, %r40, 3, 114

.text
; LOAD/STORE
    sladd	%r54, %r56, %r12, 5

    ldbz	%r16, %r45, 8900
    ldbs	%r15, %r46, 8900
    ldbzx	%r54, %r56, %r12, 2, 37
    ldbsx	%r53, %r65, %r12, 2, 37
    ldbzx.l	%r54, %r56, %r12, 2, 37000000
    ldbsx.l	%r53, %r65, %r12, 2, -37000000
    ldbzmia	%r52, %r75, 10
    ldbsmia	%r51, %r76, 10
    ldbzmib	%r52, %r75, 10
    ldbsmib	%r51, %r76, 10
    stbmia	%r51, %r76, 10
    stbmib	%r52, %r75, 10

    ldhz	%r12, %r45, 8900
    ldhs	%r12, %r45, 8900
    ldhzx	%r54, %r56, %r12, 3, -157
    ldhsx	%r54, %r56, %r12, 2, 237
    ldhzx.l	%r54, %r56, %r12, 2, 37000000
    ldhsx.l	%r53, %r65, %r12, 2, -37000000
    ldhzmia	%r54, %r56, 12
    ldhsmia	%r54, %r56, -60
    ldhzmib	%r54, %r56, 12
    ldhsmib	%r54, %r56, -60
    sthmia	%r51, %r76, 10
    sthmib	%r52, %r75, 10

    ldwz	%r12, %r45, 8900
    ldws	%r12, %r45, 8900
    ldwzx	%r54, %r56, %r12, 2, 7
    ldwsx	%r54, %r56, %r12, 2, 7
    ldwzx.l	%r54, %r56, %r12, 2, 37000000
    ldwsx.l	%r53, %r65, %r12, 2, -37000000
    ldwzmia	%r54, %r56, 12
    ldwsmia	%r54, %r56, 32
    ldwzmib	%r54, %r56, 12
    ldwsmib	%r54, %r56, 32
    stwmia	%r51, %r76, 10
    stwmib	%r52, %r75, 10

    lddz	%r54, %r56, 5600
    ldds	%r54, %r56, 5600
    lddz.l	%r53, %r46, 98765432
    lddz	%r52, %r45, -5600
    lddz.l	%r51, %r55, -98765432
    lddzx	%r50, %r56, %r12, 2, 37
    lddsx	%r50, %r56, %r12, 2, 37
    lddzx.l	%r54, %r56, %r12, 2, 37000000
    lddsx.l	%r53, %r65, %r12, 2, -37000000
    lddzmia	%r57, %r56, -12
    lddzmia	%r57, %r56, -12
    lddsmia	%r57, %r56, -12
    lddsmia	%r57, %r56, -12
    lddzmib	%r57, %r56, -12
    lddzmib	%r57, %r56, -12
    lddsmib	%r57, %r56, -12
    lddsmib	%r57, %r56, -12
    stdmia	%r51, %r76, 10
    stdmib	%r52, %r75, 10

    ldq		%r16, %r45, 8900
    ldq.l	%r16, %r45, 8900000
    ldq.l	%r16, %r45, -8900000
    ldqx	%r54, %r56, %r12, 2, 37
    ldqx.l	%r54, %r56, %r12, 2, 37000000
    ldqx.l	%r54, %r56, %r12, 2, -37000000
    ldqmia	%r52, %r75, 10
    ldqmia	%r52, %r75, 10
    ldqmib	%r52, %r75, 10
    ldqmib	%r52, %r75, 10
    stqmia	%r51, %r76, 10
    stqmib	%r52, %r75, 10

    stb		%r12, %r45, 8900
    sth		%r12, %r45, 8900
    stw		%r12, %r45, 8900
    std		%r12, %r45, 890*8

    lddz	%r12, %r45, 8048
    std		%r12, %r45, 8064
    lddzx	%r12, %r45, %r13, 3, 7
    stdx	%r12, %r45, %r13, 3, 7

    lddz	%r60, %r55, 56
    lddz	%r60, %r56, 56
    lddz	%r46, %r55, 120
    std		%r47, %r55, 56

    lddz	%r60, %sp, 624
    std		%r60, %sp, 624
    lddzx	%r60, %sp, %r12, 3, 28
    stdx	%r60, %sp, %r12, 3, 26
    lddz	%r56, %r57, 567
    std		%r56, %r57, 567

    ldwz	%r34, %r12, 900
    lddz	%r34, %r12, 900
    stw		%r23, %r12, 900
    std		%r23, %r12, 900

    ldq		%r34, %r13, 55*16
    stq		%r35, %r13, 55*16
    ldqx	%r34, %r13, %r45, 3, 80
    stqx	%r34, %r13, %r45, 3, 80

skipaddr:
    nop	0
.end
