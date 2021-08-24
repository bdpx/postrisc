.text
    ; at the beginning of the program, the register stack is empty
    alloc 54   ; expand frame to 54 registers
    eh_adj simple_func_end
    ld_imm %r47, 1  ; will be saved when called
    ld_imm %r53, 3  ; first argument
    ld_imm %r52, 2  ; second argument
    ld_imm %r51, 1  ; third argument
    ; func procedure call, all registers up to 50 will be saved,
    ; return address, eip, frame size (50) are saved in r50
    call %r50, simple_func
    ; at this point, after returning, the frame will be again 53
    jmp simple_func_end
simple_func:
    ; at the starting point, the func procedure has a 5-register frame
    ; their previous numbers are 50, 51, 52, 53, new - 0, 1, 2, 3
    ; extend the frame to 10 registers (another 4,5,6,7,8,9)
    alloc 10
    write "r0 = %x128(r0)"; print packed return info
    write "r1 = %i64(r1)" ; print 1st argument
    write "r2 = %i64(r2)" ; print 2nd argument
    write "r3 = %i64(r3)" ; print 3rd argument
    ret
simple_func_end:
    nop 123
.end
