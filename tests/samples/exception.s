.text
    write "begin exception test"
    alloc 96

    lda_iprel %r2, catch
    set_spr %r2, %eip

; constructor 1
    ld_imm %r4, 1
    eh_adj call_destructor_1
    write "eip: %s(eip)"
; constructor 2
    ld_imm %r5, 2
    eh_adj call_destructor_2
    write "eip: %s(eip)"

    ld_imm %r3, 0xFFFFFFFFFFFF1230
    eh_throw %r3, 0    ; set eca, jump to eip
    write "normal execution (never occurs)"

call_destructor_2:
    write "call_destructor_2"
    eh_catch %r6, end_destructor_2
    ; here dtor called
    ld_imm %r4, 0
end_destructor_2:
    eh_next %r6, call_destructor_1
    write "normal continue after destructor_2"

call_destructor_1:
    write "call_destructor_1"
    eh_catch %r6, end_destructor_1
    ; here dtor called
    ld_imm %r5, 0
end_destructor_1:
    eh_next %r6, catch
    write "normal continue after destructor_1"

call_ret:
    write "normal exit"
    jmp exception_exit

catch:
    write "caught exception, exit"
    eh_catch %r12, exception_exit
    write "caught exception context: r12=%x64(r12)"
exception_exit:
    nop 1234567
    nop 7654321
.end
