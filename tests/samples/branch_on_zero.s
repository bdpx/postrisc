.text
    alloc 20
    write "test compare-with-zero-and-long-branch"
compare_with_zero_test_continue:
compare_with_zero_backward_target:
    addid    %r2, %r2, 1
    breqd %r2, %r2, compare_with_zero_test_exit

    breqd   %r1, %gz, compare_with_zero_forward_target
    breqd.l %r1, %gz, compare_with_zero_forward_target
    breqd   %r1, %gz, compare_with_zero_backward_target
    breqd.l %r1, %gz, compare_with_zero_backward_target
    brned   %r1, %gz, compare_with_zero_forward_target
    brned.l %r1, %gz, compare_with_zero_forward_target
    brned   %r1, %gz, compare_with_zero_backward_target
    brned.l %r1, %gz, compare_with_zero_backward_target

    brltsd   %r1, %gz, compare_with_zero_forward_target
    brltsd.l %r1, %gz, compare_with_zero_forward_target
    brltsd   %r1, %gz, compare_with_zero_backward_target
    brltsd.l %r1, %gz, compare_with_zero_backward_target
    brgesd   %r1, %gz, compare_with_zero_forward_target
    brgesd.l %r1, %gz, compare_with_zero_forward_target
    brgesd   %r1, %gz, compare_with_zero_backward_target
    brgesd.l %r1, %gz, compare_with_zero_backward_target

compare_with_zero_forward_target:
    jmp compare_with_zero_test_continue
compare_with_zero_test_exit:
    write "end test compare-with-zero-and-long-branch"
.end
