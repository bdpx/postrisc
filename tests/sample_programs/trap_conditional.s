
.text
	write	"test conditional traps"
	ldi	r21, 0
	ldi	r22, 1
	teq	r21, r22, 0
	ldi	r21, 0
	ldi	r22, 0
	tne	r21, r22, 0

	tlt	r21, r22, 0
	tlt.l	r21, r22, 0

	tltu	r21, r22, 0
	tltu.l	r21, r22, 0
.end