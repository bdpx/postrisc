.text
	write	"test compare-with-zero-and-long-branch"
compare_with_zero_test_continue:
compare_with_zero_backward_target:
	addi	r2, r2, 1
	beq	r2, r2, compare_with_zero_test_exit

	beqz	r1, compare_with_zero_forward_target
	beqz.l	r1, compare_with_zero_forward_target
	beqz	r1, compare_with_zero_backward_target
	beqz.l	r1, compare_with_zero_backward_target
	bnez	r1, compare_with_zero_forward_target
	bnez.l	r1, compare_with_zero_forward_target
	bnez	r1, compare_with_zero_backward_target
	bnez.l	r1, compare_with_zero_backward_target
	bltz	r1, compare_with_zero_forward_target
	bltz.l	r1, compare_with_zero_forward_target
	bltz	r1, compare_with_zero_backward_target
	bltz.l	r1, compare_with_zero_backward_target
	blez	r1, compare_with_zero_forward_target
	blez.l	r1, compare_with_zero_forward_target
	blez	r1, compare_with_zero_backward_target
	blez.l	r1, compare_with_zero_backward_target
	bgtz	r1, compare_with_zero_forward_target
	bgtz.l	r1, compare_with_zero_forward_target
	bgtz	r1, compare_with_zero_backward_target
	bgtz.l	r1, compare_with_zero_backward_target
	bgez	r1, compare_with_zero_forward_target
	bgez.l	r1, compare_with_zero_forward_target
	bgez	r1, compare_with_zero_backward_target
	bgez.l	r1, compare_with_zero_backward_target

compare_with_zero_forward_target:
	br	compare_with_zero_test_continue
compare_with_zero_test_exit:
	write	"end test compare-with-zero-and-long-branch"
.end
