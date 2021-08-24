.text
    alloc	20
    write	"test compare-with-zero-and-long-branch"
compare_with_zero_test_continue:
compare_with_zero_backward_target:
    addi	%r2, %r2, 1
    b.eq.d	%r2, %r2, compare_with_zero_test_exit

    b.eq.d	%r1, %gz, compare_with_zero_forward_target
    b.eq.d.l	%r1, %gz, compare_with_zero_forward_target
    b.eq.d	%r1, %gz, compare_with_zero_backward_target
    b.eq.d.l	%r1, %gz, compare_with_zero_backward_target
    b.ne.d	%r1, %gz, compare_with_zero_forward_target
    b.ne.d.l	%r1, %gz, compare_with_zero_forward_target
    b.ne.d	%r1, %gz, compare_with_zero_backward_target
    b.ne.d.l	%r1, %gz, compare_with_zero_backward_target
    bs.lt.d	%r1, %gz, compare_with_zero_forward_target
    bs.lt.d.l	%r1, %gz, compare_with_zero_forward_target
    bs.lt.d	%r1, %gz, compare_with_zero_backward_target
    bs.lt.d.l	%r1, %gz, compare_with_zero_backward_target
    bs.le.d	%r1, %gz, compare_with_zero_forward_target
    bs.le.d.l	%r1, %gz, compare_with_zero_forward_target
    bs.le.d	%r1, %gz, compare_with_zero_backward_target
    bs.le.d.l	%r1, %gz, compare_with_zero_backward_target
    bs.gt.d	%r1, %gz, compare_with_zero_forward_target
    bs.gt.d.l	%r1, %gz, compare_with_zero_forward_target
    bs.gt.d	%r1, %gz, compare_with_zero_backward_target
    bs.gt.d.l	%r1, %gz, compare_with_zero_backward_target
    bs.ge.d	%r1, %gz, compare_with_zero_forward_target
    bs.ge.d.l	%r1, %gz, compare_with_zero_forward_target
    bs.ge.d	%r1, %gz, compare_with_zero_backward_target
    bs.ge.d.l	%r1, %gz, compare_with_zero_backward_target

compare_with_zero_forward_target:
    jmp		compare_with_zero_test_continue
compare_with_zero_test_exit:
    write	"end test compare-with-zero-and-long-branch"
.end
