.text
    alloc  96
    write  "Example of strided loop instructions"
; fast_check
    ldi    %r12, 10000  ; load loop number (10)
stride_loop_start:
;  write  "%i64(r12)"
    cmpeqd %r4, %r12, %r12
    addd %r14, %r14, %r46
    repgtd %r12, %gz, 1, stride_loop_start

    write  "counter=%i64(r12)"

; Second example of strided loop.
; fast_check
    ldi    %r12, 10000  ; load loop number (10)
    ldi    %r14, 10000  ; load loop number (10)
stride_loop_start2:
;   write  "%i64(r12)"
    cmpeqd %r4, %r12, %r12
    addid %r14, %r14, -2
    repgtd %r12, %gz, 1, stride_loop_start2

    write  "%i64(r12) %i64(r14)"

;*****************************************************************
; 3x inner loop example
;*****************************************************************
    ldi    %r3, 0
    ldi    %r20, 0
    ldi    %r33, 80
    mov    %r10, %r33
    mov    %r11, %r33
    mov    %r12, %r33
ccloop:
;   write  "%i64(r12)"
    addid %r20, %r20, 1
    addid %r12, %r12, -1
    cmpltsd %r2, %r3, %r12
;   jmp  ccloop
;   write  "%i64(r11)"
    addid %r11, %r11, -1
    cmpltsd %r4, %r3, %r11
    mov %r12, %r33
;   jmp ccloop
;   write "%i64(r10)"
    addid %r10, %r10, -1
    cmpltsd %r6, %r3, %r10
    mov %r11, %r33
    mov %r12, %r33
;   jmp ccloop

    write  "%i64(r20)"

; for(i=0; i<100; i++)

    ldi  %r8, 0
start1:
;   write "%i64(r8)"
    addid  %r8, %r8,1
    cmpltsid  %r7, %r8,128
    brneid %r7,0,start1

; for(i=100; i>0; i--)
    ldi %r8, 100
start2:
    write "%i64(r8)"
    addid %r8, %r8,-1 ; current error
    cmpltsd %r2, %r3, %r8
    brneid %r2, 0, start2

    write "r3      %x64(r3)"
; setspr %r3, %rsc


; for(i=100; i>0; i--) write "%x64((i)"
    ldi %r10, 100
qqq: cmpltsd %r2, %r3, %r10
    write "r10     %x64(r10)"
    addid %r10, %r10, -1
;   jmp qqq
sss:

    andi.l %r55, %r55,0x000FFFFF00003F0F
    setspr %r12, %ifa
; test some special regs
    ldi.l %r9, 0x123456789
;   setspr %r9, psr
    write "ip: %s(ip) psr: %s(psr)"
;   setspr %r3, psr
    ldi %r55, 120
    setspr %r55, %tpr
    write "fpcr    %s(fpcr)"
    write "psr     %s(psr)"

    write "test long loop"
; test simple loop
; fast_check
    ldi %r13, 350000 ; 35
    ldi %r14, 350000 ; 35
    ldi %r15, 88
    write "%i64(r14)"
repeat_loop_start: (128)
; write "%i64(r12)"
    addid %r13, %r13, 3
    addd %r13, %r13, %r15
    srpid %r13, %r13, %r15, 8

    addid %r13, %r13, 4
    addd %r13, %r13, %r15
    srpid %r13, %r13, %r15, 7

    addid %r13, %r13, 5
    addd %r13, %r13, %r15
    srpid %r13, %r13, %r15, 6

    addid %r13, %r13, 6
    addd %r13, %r13, %r15
    srpid %r13, %r13, %r15, 5

    subd %r13, %r13, %r15
    sladdd %r13, %r13, %r15, 5
    sladdd %r13, %r13, %r15, 5

    xor %r13, %r14, %r15
    slld %r13, %r13, %r13
    repgtd %r14, %gz, 1, repeat_loop_start

    write "%i64(r13) %i64(r14)"

    write "end test long loop"
.end
