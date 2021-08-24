.text
; Simple test program
; 20! factorial compute
.text
    alloc 61
    ldi %r15, -100
loop_stop_sard:
    divp2id %r13, %r15, 5
    repled %r15, %gz, 1, loop_stop_sard

; performance test - long loop
; for(i = 1000000; i>0; i--) DoSome();

    ldi %r20, 2500000
    ldi %r15, 20 ; maximum factorial number
    ldi %r21, 5
loop_stop: (64)
    addid %r13, %r13, 5
    subd %r14, %r14, %r55
    cmpltsd %r24, %r14, %r14
    addid %r13, %r13, 4
    subd %r14, %r14, %r55
    cmpltsd %r22, %r14, %r14
    addid %r13, %r13, 33
    srpid %r14, %r14, %r55, 13
    subd %r14, %r13, %r21
    sraid %r14, %r14, 7
    repgtd %r20, %gz, 1, loop_stop
; print loop counter after loop (must be 0)
    write "%i64(r20) factorials"
    ldi %r13, 1
    ldi %r14, 1
start:
    muld %r13, %r13, %r14
    write "factorial: %u64(r13)"
    repled %r14, %r15, 1, start

    write "%i64(r14) %i64(r13)"
.end
