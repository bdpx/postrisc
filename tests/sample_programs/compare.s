.text
	write	"test compare"
	alloc	r95, end_compare_test
	ldi	r20, 4
	ldi	r21, 3
	ldi	r22, -4
	ldi	r23, -12
	write	"test compare instructions"
	cmpeq	r12, r20, r21
	cmplt	r12, r20, r21
	cmpltu	r12, r20, r21
	cmpeqi	r12, r20, 123456
	cmplti	r12, r20, 123456
	cmpltui	r12, r20, 123456
	cmpne	r12, r20, r21
	cmpnei	r12, r20, 123456
	cmpgti	r12, r20, 123456
	cmpgtui	r12, r20, 123456
	cmple	r12, r20, r21
	cmpleu	r12, r20, r21

	write	"compare aliases (pseudo-instructions)"
	cmpgt	r12, r20, r21		; cmplt		r12,r21,r20
	cmpgtu	r12, r20, r21		; cmpltu	r12,r21,r20
	cmplti	r12, r20, 123456	; cmplti	r12,r20,12346
	cmpltui	r12, r20, 123456	; cmpltui	r12,r20,12346
	cmpge	r12, r20, r21		; cmpleq	r12,r21,r20
	cmpgeu	r12, r20, r21		; cmpleu	r12,r21,r20
	cmpgti	r12, r20, 123456	; cmpgti	r12,r20,12346
	cmpgtui	r12, r20, 123456	; cmpgtui	r12,r20,12346

; TESTS
	cmpeq	r14, r12, r45
	cmpne	r14, r12, r45

	cmpeq	r14, r45, r34
	cmpeqi	r14, r45, 123
	cmpeqi.l	r14, r45, 1234567890123
	cmplti	r14, r45, 123
	cmplti.l	r14, r45, 1234567890123
	cmplt	r14, r45, r34
	cmpgtui	r14, r45, 123
	cmpgtui.l	r14, r45, 1234567890123
	cmpgtu	r14, r45, r34

	cmpeq	r41, r34, r56
	cmplt	r66, r45, r57
	cmpeqi	r64, r56, 0
end_compare_test:
.end