.text
    alloc	96
    write	"Example of strided loop instructions"
; fast_check
    ld_imm		%r12, 10000	; load loop number (10)
stride_loop_start:
;	write	"%i64(r12)"
    cmp_eq_i64 %r4, %r12, %r12
    add_i64		%r14, %r14, %r46
    rep_gt_i64 %r12, %gz, 1, stride_loop_start

    write	"counter=%i64(r12)"

; Second example of strided loop.
; fast_check
    ld_imm		%r12, 10000	; load loop number (10)
    ld_imm		%r14, 10000	; load loop number (10)
stride_loop_start2:
;   write	"%i64(r12)"
    cmp_eq_i64 %r4, %r12, %r12
    add_imm_i64	%r14, %r14, -2
    rep_gt_i64 %r12, %gz, 1, stride_loop_start2

    write	"%i64(r12) %i64(r14)"

;*****************************************************************
; 3x inner loop example
;*****************************************************************
    ld_imm		%r3, 0
    ld_imm		%r20, 0
    ld_imm		%r33, 80
    mov		%r10, %r33
    mov		%r11, %r33
    mov		%r12, %r33
ccloop:
;   write	"%i64(r12)"
    add_imm_i64	%r20, %r20, 1
    add_imm_i64	%r12, %r12, -1
    cmp_lt_i64	%r2, %r3, %r12
;   jmp	ccloop
;   write	"%i64(r11)"
    add_imm_i64	%r11, %r11, -1
    cmp_lt_i64	%r4, %r3, %r11
    mov		%r12, %r33
;   jmp		ccloop
;   write	"%i64(r10)"
    add_imm_i64	%r10, %r10, -1
    cmp_lt_i64	%r6, %r3, %r10
    mov		%r11, %r33
    mov		%r12, %r33
;   jmp		ccloop

    write	"%i64(r20)"

; for(i=0; i<100; i++)

    ld_imm	%r8, 0
start1:
;   write "%i64(r8)"
    add_imm_i64 %r8, %r8,1
    cmp_lt_imm_i64 %r7, %r8,128
    br_ne_imm_i64 %r7,0,start1

; for(i=100; i>0; i--)
    ld_imm %r8, 100
start2:
    write "%i64(r8)"
    add_imm_i64 %r8, %r8,-1 ; current error
    cmp_lt_i64 %r2, %r3, %r8
    br_ne_imm_i64 %r2, 0, start2

    write "r3      %x64(r3)"
; set_spr %r3, %rsc


; for(i=100; i>0; i--) write "%x64((i)"
    ld_imm %r10, 100
qqq: cmp_lt_i64 %r2, %r3, %r10
    write "r10     %x64(r10)"
    add_imm_i64 %r10, %r10, -1
;   jmp qqq
sss:

    and_imm.l %r55, %r55,0x000FFFFF00003F0F
    set_spr %r12, %ifa
; test some special regs
    ld_imm.l %r9, 0x123456789
;   set_spr %r9, psr
    write "ip: %s(ip) psr: %s(psr)"
;   set_spr %r3, psr
    ld_imm %r55, 120
    set_spr %r55, %tpr
    write "fpcr    %s(fpcr)"
    write "psr     %s(psr)"

    write "test long loop"
; test simple loop
; fast_check
    ld_imm %r13, 350000 ; 35
    ld_imm %r14, 350000 ; 35
    ld_imm %r15, 88
    write "%i64(r14)"
repeat_loop_start: (128)
; write "%i64(r12)"
    add_imm_i64 %r13, %r13, 3
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 8

    add_imm_i64	%r13, %r13, 4
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 7

    add_imm_i64	%r13, %r13, 5
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 6

    add_imm_i64 %r13, %r13, 6
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 5

    sub_i64 %r13, %r13, %r15
    sl_add_i64 %r13, %r13, %r15, 5
    sl_add_i64 %r13, %r13, %r15, 5

    xor %r13, %r14, %r15
    sll_u64 %r13, %r13, %r13
    rep_gt_i64 %r14, %gz, 1, repeat_loop_start

    write "%i64(r13) %i64(r14)"

    write "end test long loop"
.end
