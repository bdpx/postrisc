.text
    alloc 20
    write "test compare-with-zero-and-long-branch"
compare_with_zero_test_continue:
compare_with_zero_backward_target:
    add_imm_i64    %r2, %r2, 1
    br_eq_i64 %r2, %r2, compare_with_zero_test_exit

    br_eq_i64   %r1, %gz, compare_with_zero_forward_target
    br_eq_i64.l %r1, %gz, compare_with_zero_forward_target
    br_eq_i64   %r1, %gz, compare_with_zero_backward_target
    br_eq_i64.l %r1, %gz, compare_with_zero_backward_target
    br_ne_i64   %r1, %gz, compare_with_zero_forward_target
    br_ne_i64.l %r1, %gz, compare_with_zero_forward_target
    br_ne_i64   %r1, %gz, compare_with_zero_backward_target
    br_ne_i64.l %r1, %gz, compare_with_zero_backward_target
    br_lt_i64   %r1, %gz, compare_with_zero_forward_target
    br_lt_i64.l %r1, %gz, compare_with_zero_forward_target
    br_lt_i64   %r1, %gz, compare_with_zero_backward_target
    br_lt_i64.l %r1, %gz, compare_with_zero_backward_target
    br_ge_i64   %r1, %gz, compare_with_zero_forward_target
    br_ge_i64.l %r1, %gz, compare_with_zero_forward_target
    br_ge_i64   %r1, %gz, compare_with_zero_backward_target
    br_ge_i64.l %r1, %gz, compare_with_zero_backward_target

compare_with_zero_forward_target:
    jmp compare_with_zero_test_continue
compare_with_zero_test_exit:
    write "end test compare-with-zero-and-long-branch"
.end
