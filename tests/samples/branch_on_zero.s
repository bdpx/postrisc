.text
    alloc	20
    write	"test compare-with-zero-and-long-branch"
compare_with_zero_test_continue:
compare_with_zero_backward_target:
    addi	%r2, %r2, 1
    bdeq	%r2, %r2, compare_with_zero_test_exit

    bdeq	%r1, %gz, compare_with_zero_forward_target
    bdeq.l	%r1, %gz, compare_with_zero_forward_target
    bdeq	%r1, %gz, compare_with_zero_backward_target
    bdeq.l	%r1, %gz, compare_with_zero_backward_target
    bdne	%r1, %gz, compare_with_zero_forward_target
    bdne.l	%r1, %gz, compare_with_zero_forward_target
    bdne	%r1, %gz, compare_with_zero_backward_target
    bdne.l	%r1, %gz, compare_with_zero_backward_target
    bdlt	%r1, %gz, compare_with_zero_forward_target
    bdlt.l	%r1, %gz, compare_with_zero_forward_target
    bdlt	%r1, %gz, compare_with_zero_backward_target
    bdlt.l	%r1, %gz, compare_with_zero_backward_target
    bdle	%r1, %gz, compare_with_zero_forward_target
    bdle.l	%r1, %gz, compare_with_zero_forward_target
    bdle	%r1, %gz, compare_with_zero_backward_target
    bdle.l	%r1, %gz, compare_with_zero_backward_target
    bdgt	%r1, %gz, compare_with_zero_forward_target
    bdgt.l	%r1, %gz, compare_with_zero_forward_target
    bdgt	%r1, %gz, compare_with_zero_backward_target
    bdgt.l	%r1, %gz, compare_with_zero_backward_target
    bdge	%r1, %gz, compare_with_zero_forward_target
    bdge.l	%r1, %gz, compare_with_zero_forward_target
    bdge	%r1, %gz, compare_with_zero_backward_target
    bdge.l	%r1, %gz, compare_with_zero_backward_target

compare_with_zero_forward_target:
    jmp		compare_with_zero_test_continue
compare_with_zero_test_exit:
    write	"end test compare-with-zero-and-long-branch"
.end
