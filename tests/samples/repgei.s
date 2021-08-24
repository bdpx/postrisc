.text
; test simple long loop
    alloc 61
    ld_imm %r13, 1000000
    mov %r14, %r13
    write "loop limit: %i64(r14)"
    ld_imm %r15, 88
repeat_long_loop_start: (128)
    add_imm_i64 %r13, %r13, 3
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 8
    add_imm_i64 %r13, %r13, 4
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 7
    add_imm_i64 %r13, %r13, 5
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 6
    add_imm_i64 %r13, %r13, 6
    add_i64 %r13, %r13, %r15
    srp_imm_i64 %r13, %r13, %r15, 5
    add_i64 %r30, %r31, %r14
    sub_i64 %r31, %r30, %r15
    sll_imm_u64 %r40, %r40, 12
    lda_xi64 %r41, %r40, %r12, 3, -12
    lda_xi64 %r41, %r40, %r12, 4, 62
    rep_gt_i64 %r14, %gz, 1, repeat_long_loop_start
    jmp repeat_exit

    rep_le_i64 %r56, %r60, 1, repeat_long_loop_start
    rep_ge_i64 %r56, %r60, 1, repeat_long_loop_start
    rep_le_u64 %r56, %r20, 1, repeat_long_loop_start
    rep_ge_u64 %r56, %r20, 1, repeat_long_loop_start

    rep_le_i64.l %r56, %r60, 1, repeat_long_loop_start
    rep_ge_i64.l %r56, %r60, 1, repeat_long_loop_start
    rep_le_u64.l %r56, %r20, 1, repeat_long_loop_start
    rep_ge_u64.l %r56, %r20, 1, repeat_long_loop_start

repeat_exit:
    write "end loop repeat test"
.end
