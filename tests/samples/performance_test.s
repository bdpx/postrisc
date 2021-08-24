.text
; Simple test program
; 20! factorial compute
.text
    alloc 61
    ld_imm %r15, -100
loop_stop_sard:
    divp2_imm_i64 %r13, %r15, 5
    rep_le_i64 %r15, %gz, 1, loop_stop_sard

; performance test - long loop
; for(i = 1000000; i>0; i--) DoSome();

    ld_imm %r20, 2500000
    ld_imm %r15, 20 ; maximum factorial number
    ld_imm %r21, 5
loop_stop: (64)
    add_imm_i64 %r13, %r13, 5
    sub_i64 %r14, %r14, %r55
    cmp_lt_i64 %r24, %r14, %r14
    add_imm_i64 %r13, %r13, 4
    sub_i64 %r14, %r14, %r55
    cmp_lt_i64 %r22, %r14, %r14
    add_imm_i64 %r13, %r13, 33
    srp_imm_i64 %r14, %r14, %r55, 13
    sub_i64 %r14, %r13, %r21
    sra_imm_i64 %r14, %r14, 7
    rep_gt_i64 %r20, %gz, 1, loop_stop
; print loop counter after loop (must be 0)
    write "%i64(r20) factorials"
    ld_imm %r13, 1
    ld_imm %r14, 1
start:
    mul_i64 %r13, %r13, %r14
    write "factorial: %u64(r13)"
    rep_le_i64 %r14, %r15, 1, start

    write "%i64(r14) %i64(r13)"
.end
