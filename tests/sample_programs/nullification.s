
.text
	write	"test nullification (explicit masks)"
	alloc	r95, end_nullification_test
	ldi	r10, 0
	cneq	r10, r10, 5, 4
	write	"0" ; nullified
	write	"1" ; nullified
	write	"2" ; nullified
	write	"3" ; nullified
	write	"4" ; nullified
	write	"5" ; else
	write	"6" ; else
	write	"7" ; else
	write	"8" ; else

	write	"test nullification (predicate names)"
	ldi	r10, 0
	cneq	r10, r10, equal, nonequal
	write	"0"
	write	"1"
	write	"2"
	write	"3"
	write	"4" (equal)
	write	"5"
	write	"6"
	write	"7"
	write	"8" (nonequal)


	write	"test nullification"
	ldi	r10, 0
	cneq	r10, r10, 4, 3
	addi	r10, r10, 2
	addi	r10, r10, 2
	addi	r10, r10, 2
	addi	r10, r10, 1
	addi	r10, r10, 1
	addi	r10, r10, 1
	addi	r10, r10, 1

	write	"test nullification"
	ldi	r10, 0
	cneq	r10, r10, true, false
	addi	r10, r10, 2
	addi	r10, r10, 2
	addi	r10, r10, 2
	addi	r10, r10, 2
	addi	r10, r10, 1 (true)
	addi	r10, r10, 1
	addi	r10, r10, 1 (false)

	nop	0
	nop	0
	cneq	r12, r10, 4, 3
	write	"branch1: psr=%s(psr)"
	write	"branch1: %i64(r10)"
	write	"branch1: %i64(r10)"
	write	"branch1: %i64(r10)"
	write	"branch2: psr=%s(psr)"
	write	"branch2: %i64(r20)"
	write	"branch2: %i64(r20)"


	cneq	r23,r45,0b1100,0b0101
	cnlt	r23,r45,0b1100,0b0101
	cnltu	r23,r45,0b1100,0b0101

	cneqi	r23,45,0b1100,0b0101
	cnlti	r23,-45,0b1100,0b0101
	cnltui	r23,45,0b1100,0b0101

	cneqi.l   r23,45000000000,0b1100,0b0101
	cnlti.l   r23,-45000000000,0b1100,0b0101
	cnltui.l  r23,45000000000,0b1100,0b0101

	cnbs	r23,r45,0b1100,0b0101
	cnbsi	r23,45,0b1100,0b0101
	nop	1
	nop	2
	nop	3
	nop	4
	nop	5
	nop	6
	nop	7

	cneq	r10, r10, same_equal, same_nonequal
	write	"0e"
	write	"1e"
	write	"2e" (same_equal, same_nonequal)

	cnne	r10, r10, same_equal2, same_nonequal2
	write	"0ne"
	write	"1ne"
	write	"2ne" (same_equal2, same_nonequal2)

end_nullification_test:
	write	"end_nullification_test"
.end
