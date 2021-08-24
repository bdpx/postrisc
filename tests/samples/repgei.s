.text
; test simple long loop
    alloc	61
    ldi		%r13, 1000000
    mov		%r14, %r13
    write	"loop limit: %i64(r14)"
    ldi		%r15, 88
repeat_long_loop_start: (128)
    addi	%r13, %r13, 3
    add		%r13, %r13, %r15
    srpi	%r13, %r13, %r15, 8
    addi	%r13, %r13, 4
    add		%r13, %r13, %r15
    srpi	%r13, %r13, %r15, 7
    addi	%r13, %r13, 5
    add		%r13, %r13, %r15
    srpi	%r13, %r13, %r15, 6
    addi	%r13, %r13, 6
    add		%r13, %r13, %r15
    srpi	%r13, %r13, %r15, 5
    add		%r30, %r31, %r14
    sub		%r31, %r30, %r15
    slli	%r40, %r40, 12
    ldax	%r41, %r40, %r12, 3, -12
    ldax	%r41, %r40, %r12, 4, 112
    repdgt	%r14, %gz, repeat_long_loop_start
    jmp		repeat_exit

    repdle	%r56, %r60, repeat_long_loop_start
    repdge	%r56, %r60, repeat_long_loop_start
    repdleu	%r56, %r20, repeat_long_loop_start
    repdgeu	%r56, %r20, repeat_long_loop_start

    repdle.l	%r56, %r60, repeat_long_loop_start
    repdge.l	%r56, %r60, repeat_long_loop_start
    repdleu.l	%r56, %r20, repeat_long_loop_start
    repdgeu.l	%r56, %r20, repeat_long_loop_start

repeat_exit:
    write	"end loop repeat test"
.end