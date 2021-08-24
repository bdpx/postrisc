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
    ca.r	%r17, program_start
    ldi		%r12, data_lbl
    write	"data_lbl: %i64(r12)"

    ldi		%r12, data_hi(data_lbl)
    write	"data_hi(data_lbl): %i64(r12)"
    ldi		%r12, data_lo(data_lbl)
    write	"data_lo(data_lbl): %i64(r12)"
    ca.rf	%r13, data_lbl
    write	"ca.rf(data_lbl): %x64(r13)"
    ca.rf.l	%r13, data_lbl
    write	"ca.rf(data_lbl): %x64(r13)"

    addi	%r13, %r17, data_hi(data_lbl)
    write	"r13     %i64(r13)"
    addi	%r14, %r13, data_lo(data_lbl)+0
    write	"r14     %i64(r14)"

    addi	%r13, %r17, data_hi(data_lbl)
    write	"r13     %i64(r13)"
    ldz.b	%r25, %r13, data_lo(data_lbl)+0
    ldz.b	%r26, %r13, data_lo(data_lbl)+1
    ldz.b	%r27, %r13, data_lo(data_lbl)+2
    ldz.b	%r28, %r13, data_lo(data_lbl)+3
    write	"r25     %i64(r25)" ; must be 25
    write	"r26     %i64(r26)" ; must be 26
    write	"r27     %i64(r27)" ; must be 27
    write	"r28     %i64(r28)" ; must be 28

; test load context
    ldz.d	%r1, %sp, -16
    st.d	%r1, %sp, -16
    jmp		skipaddr
    jmp.l	skipaddr

; test indexed load/store
    st.b.xd	%r12, %r15, %r30, 4, 14
    st.h.xd	%r12, %r15, %r30, 4, 14
    st.w.xd	%r12, %r15, %r30, 4, 14
    st.d.xd	%r12, %r15, %r30, 4, 14

    lda.q %r30, %r56
    sta.q %r43, %r56

    sl.add	%r43, %r56, %r23, 4
    sl.sub	%r43, %r56, %r23, 42
    sl.subf	%r43, %r56, %r23, 12

    ldz.w	%r30, %r5, 66*4	; load mid
    ldz.d.xd	%r40, %tp, %r30, 0, 4	; load base

    lds.d.xd	%r12, %r23, %r40, 3, 54
    lds.d.xd	%r12, %r23, %r40, 3, 54
    ldz.d.xd	%r12, %r23, %r40, 3, 54
    ldz.d.xd	%r12, %r23, %r40, 3, 54
    st.w.xd	%r12, %r23, %r40, 3, 54
    st.d.xd	%r12, %r23, %r40, 3, 54

    lds.b.xd	%r12, %r23, %r40, 3, 54
    lds.b.xd	%r12, %r23, %r40, 3, 54
    ldz.b.xd	%r12, %r23, %r40, 3, 54
    ldz.b.xd	%r12, %r23, %r40, 3, 54
    st.b.xd	%r12, %r23, %r40, 3, 54
    st.b.xd	%r12, %r23, %r40, 3, 54

    lds.h.xd	%r12, %r23, %r40, 3, 54
    lds.h.xd	%r12, %r23, %r40, 3, 54
    ldz.h.xd	%r12, %r23, %r40, 3, 54
    ldz.h.xd	%r12, %r23, %r40, 3, 54
    st.h.xd	%r12, %r23, %r40, 3, 54
    st.h.xd	%r12, %r23, %r40, 3, 54

.text
; LOAD/STORE
    sl.add	%r54, %r56, %r12, 5

    ldz.b	%r16, %r45, 8900
    lds.b	%r15, %r46, 8900
    ldz.b.xd	%r54, %r56, %r12, 2, 37
    lds.b.xd	%r53, %r65, %r12, 2, 37
    ldz.b.xd.l	%r54, %r56, %r12, 2, 37000000
    lds.b.xd.l	%r53, %r65, %r12, 2, -37000000
    ldz.b.mia	%r52, %r75, 10
    lds.b.mia	%r51, %r76, 10
    ldz.b.mib	%r52, %r75, 10
    lds.b.mib	%r51, %r76, 10
    st.b.mia	%r51, %r76, 10
    st.b.mib	%r52, %r75, 10

    ldz.h	%r12, %r45, 8900
    lds.h	%r12, %r45, 8900
    ldz.h.xd	%r54, %r56, %r12, 3, -57
    lds.h.xd	%r54, %r56, %r12, 2, 37
    ldz.h.xd.l	%r54, %r56, %r12, 2, 37000000
    lds.h.xd.l	%r53, %r65, %r12, 2, -37000000
    ldz.h.mia	%r54, %r56, 12
    lds.h.mia	%r54, %r56, -60
    ldz.h.mib	%r54, %r56, 12
    lds.h.mib	%r54, %r56, -60
    st.h.mia	%r51, %r76, 10
    st.h.mib	%r52, %r75, 10

    ldz.w	%r12, %r45, 8900
    lds.w	%r12, %r45, 8900
    ldz.w.xd	%r54, %r56, %r12, 2, 7
    lds.w.xd	%r54, %r56, %r12, 2, 7
    ldz.w.xd.l	%r54, %r56, %r12, 2, 37000000
    lds.w.xd.l	%r53, %r65, %r12, 2, -37000000
    ldz.w.mia	%r54, %r56, 12
    lds.w.mia	%r54, %r56, 32
    ldz.w.mib	%r54, %r56, 12
    lds.w.mib	%r54, %r56, 32
    st.w.mia	%r51, %r76, 10
    st.w.mib	%r52, %r75, 10

    ldz.d	%r54, %r56, 5600
    lds.d	%r54, %r56, 5600
    ldz.d.l	%r53, %r46, 98765432
    ldz.d	%r52, %r45, -5600
    ldz.d.l	%r51, %r55, -98765432
    ldz.d.xd	%r50, %r56, %r12, 2, 37
    lds.d.xd	%r50, %r56, %r12, 2, 37
    ldz.d.xd.l	%r54, %r56, %r12, 2, 37000000
    lds.d.xd.l	%r53, %r65, %r12, 2, -37000000
    ldz.d.mia	%r57, %r56, -12
    ldz.d.mia	%r57, %r56, -12
    lds.d.mia	%r57, %r56, -12
    lds.d.mia	%r57, %r56, -12
    ldz.d.mib	%r57, %r56, -12
    ldz.d.mib	%r57, %r56, -12
    lds.d.mib	%r57, %r56, -12
    lds.d.mib	%r57, %r56, -12
    st.d.mia	%r51, %r76, 10
    st.d.mib	%r52, %r75, 10

    ld.q		%r16, %r45, 8900
    ld.q.l	%r16, %r45, 8900000
    ld.q.l	%r16, %r45, -8900000
    ld.q.xd	%r54, %r56, %r12, 2, 37
    ld.q.xd.l	%r54, %r56, %r12, 2, 37000000
    ld.q.xd.l	%r54, %r56, %r12, 2, -37000000
    ld.q.mia	%r52, %r75, 10
    ld.q.mia	%r52, %r75, 10
    ld.q.mib	%r52, %r75, 10
    ld.q.mib	%r52, %r75, 10
    st.q.mia	%r51, %r76, 10
    st.q.mib	%r52, %r75, 10

    st.b	%r12, %r45, 8900
    st.h	%r12, %r45, 8900
    st.w	%r12, %r45, 8900
    st.d	%r12, %r45, 890*8

    ldz.d	%r12, %r45, 8048
    st.d	%r12, %r45, 8064
    ldz.d.xd	%r12, %r45, %r13, 3, 7
    st.d.xd	%r12, %r45, %r13, 3, 7

    ldz.d	%r60, %r55, 56
    ldz.d	%r60, %r56, 56
    ldz.d	%r46, %r55, 120
    st.d	%r47, %r55, 56

    ldz.d	%r60, %sp, 624
    st.d	%r60, %sp, 624
    ldz.d.xd	%r60, %sp, %r12, 3, 28
    st.d.xd	%r60, %sp, %r12, 3, 26
    ldz.d	%r56, %r57, 567
    st.d	%r56, %r57, 567

    ldz.w	%r34, %r12, 900
    ldz.d	%r34, %r12, 900
    st.w	%r23, %r12, 900
    st.d	%r23, %r12, 900

    ld.q	%r34, %r13, 55*16
    st.q	%r35, %r13, 55*16
    ld.q.xd	%r34, %r13, %r45, 3, 60
    st.q.xd	%r34, %r13, %r45, 3, 60

skipaddr:
    nop	0
.end
