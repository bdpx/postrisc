.text
    ; at the beginning of the program, the register stack is empty
    alloc	54   ; expand frame to 54 registers
    ldar	%r4, dense_call_test_end
    mtspr	%r4, %eip
    mtspr	%r4, %reip
    ldi		%r47, 1  ; will be saved when called
    ldi		%r53, 3  ; first argument
    ldi		%r52, 2  ; second argument
    ldi		%r51, 1  ; third argument
    ; func procedure call, all registers up to 50 will be saved,
    ; return address, eip, frame size (50) are saved in r50
check_label:
    callr	%r48, simple_func_1
    callr	%r50, simple_func_2
    callr	%r52, simple_func_3
    
    jmp	dense_call_test_end

simple_func_1:
    alloc  10
    write  "simple_func_1"
    ret

simple_func_2:
    alloc  10
    write  "simple_func_2"
    ret

simple_func_3:
    alloc  10
    write  "simple_func_3"
    ret

dense_call_test_end:
    nop	123
    nop	123
    nop	123
    nop	123
    nop	123
    nop	123
.end
