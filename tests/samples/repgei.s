.text
; test simple long loop
    alloc 61
    ldi %r13, 1000000
    mov %r14, %r13
    write "loop limit: %i64(r14)"
    ldi %r15, 88
repeat_long_loop_start: (128)
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
    addd %r30, %r31, %r14
    subd %r31, %r30, %r15
    sllid %r40, %r40, 12
    ldaxsd %r41, %r40, %r12, 3, -12
    ldaxsd %r41, %r40, %r12, 4, 62
    repgtd %r14, %gz, 1, repeat_long_loop_start
    jmp repeat_exit

    repled %r56, %r60, 1, repeat_long_loop_start
    repged %r56, %r60, 1, repeat_long_loop_start
    repleud %r56, %r20, 1, repeat_long_loop_start
    repgeud %r56, %r20, 1, repeat_long_loop_start

    repled.l %r56, %r60, 1, repeat_long_loop_start
    repged.l %r56, %r60, 1, repeat_long_loop_start
    repleud.l %r56, %r20, 1, repeat_long_loop_start
    repgeud.l %r56, %r20, 1, repeat_long_loop_start

repeat_exit:
    write "end loop repeat test"
.end
