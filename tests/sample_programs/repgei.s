.text
; test simple long loop
	ldi	r13, 1000000
	mov	r14, r13
	write	"loop limit: %i64(r14)"
	ldi	r15, 88
repeat_long_loop_start: (128)
	addi	r13, r13,3
	add	r13, r13,r15
	srpi	r13, r13,r15,8
	addi	r13, r13,4
	add	r13, r13,r15
	srpi	r13, r13,r15,7
	addi	r13, r13,5
	add	r13, r13,r15
	srpi	r13, r13,r15,6
	addi	r13, r13,6
	add	r13, r13,r15
	srpi	r13, r13,r15,5
	add	r30, r31,r14
	sub	r31, r30,r15
	slli	r40, r40,12
	ldax	r41, r40,r12,3,-12
	ldax	r41, r40,r12,4,112
	repgt	r14, gz, -1, repeat_long_loop_start
	br	repeat_exit

	reple	r56, r60, 1, repeat_long_loop_start
	repge	r56, r60, -1, repeat_long_loop_start
	repleu	r56, r20, 1, repeat_long_loop_start
	repgeu	r56, r20, -1, repeat_long_loop_start

repeat_exit:
	write	"end loop repeat test"
.end