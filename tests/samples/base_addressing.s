.text
.data
data_lbl:
    d1 25
    d1 26
    d1 27
    d1 28

.text
program_start:
; Here we test references to data section.
; Absolute offset from begin of section
    write "base addressing"
    alloc 96
    ldarc %r17, program_start
    ldi %r12, data_lbl
    write "data_lbl: %i64(r12)"

    ldi %r12, data_hi(data_lbl)
    write "data_hi(data_lbl): %i64(r12)"
    ldi %r12, data_lo(data_lbl)
    write "data_lo(data_lbl): %i64(r12)"
    ldard %r13, data_lbl
    write "ca.rf(data_lbl): %x64(r13)"
    ldard.l %r13, data_lbl
    write "ca.rf(data_lbl): %x64(r13)"

    addid %r13, %r17, data_hi(data_lbl)
    write "r13     %i64(r13)"
    addid %r14, %r13, data_lo(data_lbl)+0
    write "r14     %i64(r14)"

    addid %r13, %r17, data_hi(data_lbl)
    write "r13     %i64(r13)"
    ldub %r25, %r13, data_lo(data_lbl)+0
    ldub %r26, %r13, data_lo(data_lbl)+1
    ldub %r27, %r13, data_lo(data_lbl)+2
    ldub %r28, %r13, data_lo(data_lbl)+3
    write "r25     %i64(r25)" ; must be 25
    write "r26     %i64(r26)" ; must be 26
    write "r27     %i64(r27)" ; must be 27
    write "r28     %i64(r28)" ; must be 28

; test load context
    ldud %r1, %sp, -16
    std %r1, %sp, -16
    jmp skipaddr
    jmp.l skipaddr

; test indexed load/store
    stbxsd %r12, %r15, %r30, 4, 14
    sthxsd %r12, %r15, %r30, 4, 14
    stwxsd %r12, %r15, %r30, 4, 14
    stdxsd %r12, %r15, %r30, 4, 14

    amoldq %r30, %r56, relaxed
    amostq %r43, %r56, relaxed

    sladdd %r43, %r56, %r23, 4
    slsubd %r43, %r56, %r23, 42
    slsubrd %r43, %r56, %r23, 12

    lduw %r30, %r5, 66*4 ; load mid
    ldudxsd %r40, %tp, %r30, 0, 4 ; load base

    ldsdxsd %r12, %r23, %r40, 3, 54
    ldsdxsd %r12, %r23, %r40, 3, 54
    ldudxsd %r12, %r23, %r40, 3, 54
    ldudxsd %r12, %r23, %r40, 3, 54
    stwxsd %r12, %r23, %r40, 3, 54
    stdxsd %r12, %r23, %r40, 3, 54

    ldsbxsd %r12, %r23, %r40, 3, 54
    ldsbxsd %r12, %r23, %r40, 3, 54
    ldubxsd %r12, %r23, %r40, 3, 54
    ldubxsd %r12, %r23, %r40, 3, 54
    stbxsd %r12, %r23, %r40, 3, 54
    stbxsd %r12, %r23, %r40, 3, 54

    ldshxsd %r12, %r23, %r40, 3, 54
    ldshxsd %r12, %r23, %r40, 3, 54
    lduhxsd %r12, %r23, %r40, 3, 54
    lduhxsd %r12, %r23, %r40, 3, 54
    sthxsd %r12, %r23, %r40, 3, 54
    sthxsd %r12, %r23, %r40, 3, 54

.text
; LOAD/STORE
    sladdd %r54, %r56, %r12, 5

    ldub %r16, %r45, 8900
    ldsb %r15, %r46, 8900
    ldubxsd %r54, %r56, %r12, 2, 37
    ldsbxsd %r53, %r65, %r12, 2, 37
    ldubxsd.l %r54, %r56, %r12, 2, 37000000
    ldsbxsd.l %r53, %r65, %r12, 2, -37000000
    ldubmia %r52, %r75, 10
    ldsbmia %r51, %r76, 10
    ldubmib %r52, %r75, 10
    ldsbmib %r51, %r76, 10
    stbmia %r51, %r76, 10
    stbmib %r52, %r75, 10

    lduh %r12, %r45, 8900
    ldsh %r12, %r45, 8900
    lduhxsd %r54, %r56, %r12, 3, -57
    ldshxsd %r54, %r56, %r12, 2, 37
    lduhxsd.l %r54, %r56, %r12, 2, 37000000
    ldshxsd.l %r53, %r65, %r12, 2, -37000000
    lduhmia %r54, %r56, 12
    ldshmia %r54, %r56, -60
    lduhmib %r54, %r56, 12
    ldshmib %r54, %r56, -60
    sthmia %r51, %r76, 10
    sthmib %r52, %r75, 10

    lduw %r12, %r45, 8900
    ldsw %r12, %r45, 8900
    lduwxsd %r54, %r56, %r12, 2, 7
    ldswxsd %r54, %r56, %r12, 2, 7
    lduwxsd.l %r54, %r56, %r12, 2, 37000000
    ldswxsd.l %r53, %r65, %r12, 2, -37000000
    lduwmia %r54, %r56, 12
    ldswmia %r54, %r56, 32
    lduwmib %r54, %r56, 12
    ldswmib %r54, %r56, 32
    stwmia %r51, %r76, 10
    stwmib %r52, %r75, 10

    ldud   %r54, %r56, 5600
    ldsd   %r54, %r56, 5600
    ldud.l %r53, %r46, 98765432
    ldud   %r52, %r45, -5600
    ldud.l  %r51, %r55, -98765432
    ldudxsd %r50, %r56, %r12, 2, 37
    ldsdxsd %r50, %r56, %r12, 2, 37
    ldudxsd.l %r54, %r56, %r12, 2, 37000000
    ldsdxsd.l %r53, %r65, %r12, 2, -37000000
    ldudmia %r57, %r56, -12
    ldudmia %r57, %r56, -12
    ldsdmia %r57, %r56, -12
    ldsdmia %r57, %r56, -12
    ldudmib %r57, %r56, -12
    ldudmib %r57, %r56, -12
    ldsdmib %r57, %r56, -12
    ldsdmib %r57, %r56, -12
    stdmia %r51, %r76, 10
    stdmib %r52, %r75, 10

    ldq  %r16, %r45, 8900
    ldq.l %r16, %r45, 8900000
    ldq.l %r16, %r45, -8900000
    ldqxsd %r54, %r56, %r12, 2, 37
    ldqxsd.l %r54, %r56, %r12, 2, 37000000
    ldqxsd.l %r54, %r56, %r12, 2, -37000000
    ldqmia %r52, %r75, 10
    ldqmia %r52, %r75, 10
    ldqmib %r52, %r75, 10
    ldqmib %r52, %r75, 10
    stqmia %r51, %r76, 10
    stqmib %r52, %r75, 10

    stb %r12, %r45, 8900
    sth %r12, %r45, 8900
    stw %r12, %r45, 8900
    std %r12, %r45, 890*8

    ldud    %r12, %r45, 8048
    std     %r12, %r45, 8064
    ldudxsd %r12, %r45, %r13, 3, 7
    stdxsd  %r12, %r45, %r13, 3, 7

    ldud  %r60, %r55, 56
    ldud  %r60, %r56, 56
    ldud  %r46, %r55, 120
    std   %r47, %r55, 56

    ldud    %r60, %sp, 624
    std     %r60, %sp, 624
    ldudxsd %r60, %sp, %r12, 3, 28
    stdxsd  %r60, %sp, %r12, 3, 26
    ldud    %r56, %r57, 567
    std     %r56, %r57, 567

    lduw %r34, %r12, 900
    ldud %r34, %r12, 900
    stw %r23, %r12, 900
    std %r23, %r12, 900

    ldq %r34, %r13, 55*16
    stq %r35, %r13, 55*16
    ldqxsd %r34, %r13, %r45, 3, 60
    stqxsd %r34, %r13, %r45, 3, 60

skipaddr:
    nop 0
.end
