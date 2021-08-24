.text
	write	"test recursive calls"
	ldi.l	r46, 0x7FFFFFFFFFFFFFFF		; comment
	ldi.l	r46, 0x8000000000000000
	addi	r46, r46,-1
	write	"%i64(r46)"

	mfspr	r20, rsc

	alloc	r53, end_call_recursive		; extend frame to 54 regs
	ldi	r48, 1		; 
	ldi	r53, 3		; 1 arg (33+16)
	ldi	r52, 2		; 2 arg (34+16)
	ldi	r51, 1		; 3 arg (35+16)
	write	"rsc: %s(rsc)"
	callr	r50, func	; call func subroutine, safe 50 regs
retfunc:
	write	"r51=%i64(r51) rsc=%s(rsc)"
	ldi	r5, 10
	callr	r4, rekurs
retrekurs:
	write	"rsc: %s(rsc)"
	write	"rsp: %s(rsp)"
;	write	"%m(dump)"
	br	smallend
func:
; at entry point func subroutine has 4 regs in frame
	alloc	r7, end_call_recursive		; extend frame from 4 to 8 regs
	write	"r0      %x64(r0)"		; print packed caller frame and return address
	write	"r1=%i64(r1) r2=%i64(r2) r3=%i64(r3)" ; print args
	ldi	r1, 12345
	ret     r0

rekurs:
	alloc	r5, end_call_recursive
	write	"r0=%x64(r0) r1=%i64(r1)"
	write	"rsc: %s(rsc)"
	write	"rsp: %s(rsp)"
	addi	r3, r1,-1
	ldi	r2, 0
	beq	r1, r2, rekret
;	cneq	r1,r2,1,0
	callr	r2, rekurs
rekret:
	write	"rsp: %s(rsp)"
	write	"r0: %x64(r0)"
	ret     r0
smallend:
	nop	0
	nop	111
	alloc	r95, end_call_recursive
end_call_recursive:
	write	"end_call_recursive"
.end
