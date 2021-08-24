.text
; Here we test instructions for partial rotate register by fixed bitcount.
    alloc  90
    write  "initial values"
    ldi.l  %r50, 0x1234567890ABCDEF
    write  "%x64(r50)"
    write  "rotate left"
    srpid %r51, %r50, %r50, 40-1
    write  "%x64(r51)"
    write  "rotate right"
    srpid %r51, %r50, %r50, 64-40-1  ; same as previous
    write  "%x64(r51)"
    write  "rotate left immediate"
    srpid %r51, %r50, %r50, 64-40-1
    write  "%x64(r51)"
    write  "rotate right immediate"
    srpid %r51, %r50, %r50, 40-1  ; same as previous "rD+1-rC"
    write  "%x64(r51)"

; Here we test instructions for shift and mask register by fixed bitcount.
    write "shift signed|unsigned by immediate 12 bit"
    ldi.l %r50, 0xfedcba0123456789
    write "%x64(r50)"
    sraid %r51, %r50, 12
    write "%x64(r51)"
    srlid %r51, %r50, 12
    write "%x64(r51)"
    sllid %r51, %r50, 12
    write "%x64(r51)"
    sllid %r51, %r50, 12
    write "%x64(r51)"

;  jmp  ddd
    ldi %r10, 16
    slpd %r51, %r50, %r50, %r10
    write "%x64(r51)"

    ldi.l %r40, 0x1234567890abcdef
    ldi.l %r50, 0xfedcba0987654321
    slsrlid %r41, %r40, 8, 40
    write "%x64(r41)"
    slsraid %r41, %r40, 11, 40
    write "%x64(r41)"

    write "test srpi"
    ldi.l %r40, 0x1234123412341234
    ldi.l %r50, 0x5678567856785678
    srpid %r41, %r40, %r50, 39
    write "%x64(r41)"
    srpid %r41, %r50, %r40, 23
    write "%x64(r41)"
    srpid %r41, %r40, %r40, 24
    write "%x64(r41)"

    write "test vector shift right pair (srpi16) instruction"
    xor %r2, %r2, %r2 ; all zeroes
    nor %r3, %r2, %r2 ; all ones
    write "r2      %x128(r2)"
    write "r3      %x128(r3)"
    srpiq %r4, %r2, %r3, 60
    write "r4      %x128(r4)"
    srpiq %r4, %r3, %r2, 60
    write "r4      %x128(r4)"
    srpiq %r4, %r2, %r3, 100
    write "r4      %x128(r4)"
    srpiq %r4, %r3, %r2, 100
    write "r4      %x128(r4)"

; SHIFTS
    slld %r42, %r33, %r34
    slld %r42, %r33, %r34
    srad %r52, %r73, %r44
    srld %r62, %r73, %r44
    slpd %r72, %r17, %r17, %r24
    srpd %r82, %r16, %r16, %r15
    srpid %r72, %r15, %r24, 32
    deposit %r10, %r14, %r85, 32, 30

    sllid %r12, %r67, 13
    sllid %r13, %r57, 13
    sraid %r14, %r48, 14
    srlid %r15, %r38, 14
    srpid %r16, %r39, %r13, 13
    srpid %r17, %r29, %r13, 64-13


    write "test packed bitwise logical"
    and %r10, %r71, %r13
    andn %r21, %r81, %r22
    or %r32, %r71, %r32
    orn %r43, %r61, %r43
    nand %r54, %r51, %r54
    nor %r65, %r41, %r64
    xnor %r76, %r31, %r73
    xor %r87, %r21, %r83


    ldi %r20, 65
    write "r20     %c(r20)"   ; should be 'A'

    ldi   %r3, 0
    ldi.l %r22, 0x12345FFFFFFFFFFF
    write "%x64(r22)"
    deposit %r23, %r22, %gz, 0, 23
    write "%x64(r23)"

    ldi.l %r22, 0x1234567890ABCDEF
    ldi.l %r23, 0xFEDCBA9876543210
    srpid %r22, %r22, %r23, 24
    write "%x64(r22)"

    ldi.l %r24, 0x4321F00000000
    write "%x64(r24)"
    subrid %r25, %r24, 0
    write "%x64(r25)"
    not %r25, %r25
    write "%x64(r25)"
    xor  %r25, %r25, %r24
    write "%x64(r25)"

; Example of abs_diff
    ldi %r12, -10000
    abdd %r12, %r12, %gz
    write "r12: %i64(r12)"
.end
