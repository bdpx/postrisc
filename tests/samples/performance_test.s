.text
; Simple test program
; 20! factorial compute
.text
    alloc	61
    ldi		%r15, -100
loop_stop_sard:
    srdi	%r13, %r15, 5
    repdle	%r15, %gz, loop_stop_sard

; performance test - long loop
; for(i = 1000000; i>0; i--) DoSome();

    ldi		%r20, 2500000
    ldi		%r15, 20		; maximum factorial number
    ldi		%r21, 5
loop_stop: (64)
    addi	%r13, %r13, 5
    sub		%r14, %r14, %r55
    cmpdlt	%r24, %r14, %r14
    addi	%r13, %r13, 4
    sub		%r14, %r14, %r55
    cmpdlt	%r22, %r14, %r14
    addi	%r13, %r13, 33
    srpi	%r14, %r14, %r55, 13
    sub		%r14, %r13, %r21
    srai	%r14, %r14, 7
    repdgt	%r20, %gz, loop_stop
; print loop counter after loop (must be 0)
    write	"%i64(r20) factorials"
    ldi		%r13, 1
    ldi		%r14, 1
start:
    mul		%r13, %r13, %r14
    write	"factorial: %u64(r13)"
    repdle	%r14, %r15, start

    write	"%i64(r14) %i64(r13)"
.end
