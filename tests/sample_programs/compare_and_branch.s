.text
backward_target:
	addi	r2, r2, 1
	beq	r2, r2, branch_test_exit

	beq	r23,r34,backward_target
	beq.l	r23,r34,backward_target
	beq	r23,r34,forward_target
	beq.l	r23,r34,forward_target
	beqi	r23,34,backward_target
	beqi.l	r23,34,backward_target
	beqi	r23,34,forward_target
	beqi.l	r23,34,forward_target

	bne	r23,r34,backward_target
	bne.l	r23,r34,backward_target
	bne	r23,r34,forward_target
	bne.l	r23,r34,forward_target
	bnei	r23,34,backward_target
	bnei.l	r23,34,backward_target
	bnei	r23,34,forward_target
	bnei.l	r23,34,forward_target

	ble	r23,r34,backward_target
	ble.l	r23,r34,backward_target
	ble	r23,r34,forward_target
	ble.l	r23,r34,forward_target

	blt	r23,r34,backward_target
	blt.l	r23,r34,backward_target
	blt	r23,r34,forward_target
	blt.l	r23,r34,forward_target
	blti	r23,34,backward_target
	blti.l	r23,34,backward_target
	blti	r23,34,forward_target
	blti.l	r23,34,forward_target

	bge	r23,r34,backward_target
	bge.l	r23,r34,backward_target
	bge	r23,r34,forward_target
	bge.l	r23,r34,forward_target

	bgt	r23,r34,backward_target
	bgt.l	r23,r34,backward_target
	bgt	r23,r34,forward_target
	bgt.l	r23,r34,forward_target
	bgti	r23,34,backward_target
	bgti.l	r23,34,backward_target
	bgti	r23,34,forward_target
	bgti.l	r23,34,forward_target

	bleu	r23,r34,backward_target
	bleu.l	r23,r34,backward_target
	bleu	r23,r34,forward_target
	bleu.l	r23,r34,forward_target

	bltu	r23,r34,backward_target
	bltu.l	r23,r34,backward_target
	bltu	r23,r34,forward_target
	bltu.l	r23,r34,forward_target
	bltui	r23,34,backward_target
	bltui.l	r23,34,backward_target
	bltui	r23,34,forward_target
	bltui.l	r23,34,forward_target

	bgeu	r23,r34,backward_target
	bgeu.l	r23,r34,backward_target
	bgeu	r23,r34,forward_target
	bgeu.l	r23,r34,forward_target

	bgtu	r23,r34,backward_target
	bgtu.l	r23,r34,backward_target
	bgtu	r23,r34,forward_target
	bgtu.l	r23,r34,forward_target
	bgtui	r23,34,backward_target
	bgtui.l	r23,34,backward_target
	bgtui	r23,34,forward_target
	bgtui.l	r23,34,forward_target

forward_target:
branch_test_exit:

	br branch_exit

label:
	beq	r12, r13, qwe
	srpi	r10, r11, r12, 45
	dep16	r61, r91, r32, 10
	mbsel	r62, r91, r32, r10
	perm	r63, r91, r32, r10
qwe:
	bne	r15, r46, label
	beq	r25, r45, label
	blt	r25, r44, label
	ble	r35, r43, label
	bgtu	r35, r42, label
	bgeu	r45, r41, label
	bgt	r45, r40, label
	bltu	r55, r76, label
	bnei	r55, 140, label
	beqi	r65, 141, label
	blti	r65, 142, label
	bgti	r75, 143, label
	bltui	r75, 170, label
	bgtui	r85, 160, label

	addi	.l r45, r34, 1234
	bbsi	r85, 26, label
	bbci	.l r85, 36, label
	bbsi	r95, 46, label
	bbci	.l r95, 56, label

	bri	r45, r23
branch_exit:
	write	"end branch test"
.end
