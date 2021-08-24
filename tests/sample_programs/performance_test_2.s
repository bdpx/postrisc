.text
	alloc	r95, end_test_long_loop
	write "Example of strided loop instructions"
; fast_check
	ldi	r12, 10000	; load loop number (10)
stride_loop_start:
;	write	"%i64(r12)"
	cmpeq	r4, r12,r12
	add	r14, r14,r46
	repgt	r12, gz, -1, stride_loop_start

	write	"counter=%i64(r12)"

; Second example of strided loop.
; fast_check
	ldi	r12, 10000	; load loop number (10)
	ldi	r14, 10000	; load loop number (10)
stride_loop_start2:
;	write	"%i64(r12)"
	cmpeq	r4, r12,r12
	addi	r14, r14,-2
	repgt	r12, gz, -1, stride_loop_start2

	write	"%i64(r12) %i64(r14)"

;*****************************************************************
; 3x inner loop example
;*****************************************************************
	ldi	r3, 0
	ldi	r20, 0
	ldi	r33, 80
	mov	r10, r33
	mov	r11, r33
	mov	r12, r33
ccloop:
;	write	"%i64(r12)"
	addi	r20, r20,1
	addi	r12, r12,-1
	cmplt	r2, r3,r12
;	br	ccloop
;	write	"%i64(r11)"
	addi	r11, r11,-1
	cmplt	r4, r3,r11
	mov	r12, r33
;	br	ccloop
;	write	"%i64(r10)"
	addi	r10, r10,-1
	cmplt	r6, r3,r10
	mov	r11, r33
	mov	r12, r33
;	br	ccloop

	write	"%i64(r20)"

; for(i=0; i<100; i++)

	ldi	r8, 0
start1:
;	write	"%i64(r8)"
	addi	r8, r8,1
	cmplti	r7, r8,128
	bnei	r7,0,start1

; for(i=100; i>0; i--)
	ldi	r8, 100
start2:
	write	"%i64(r8)"
	addi	r8, r8,-1		; current error
	cmplt	r2, r3,r8
	bnei	r2,0,start2

	write	"r3      %x64(r3)"
;	mtspr	r3, rsc


; for(i=100; i>0; i--) write "%x64((i)"
	ldi	r10, 100
qqq:	cmplt	r2, r3,r10
	write	"r10     %x64(r10)"
	addi	r10, r10,-1
;	br	qqq
sss:

	andi.l	r55, r55,0x000FFFFF00003F0F
	mtspr	r12, ifa
; test some special regs
	ldi.l	r9, 0x123456789
;	mtspr	r9, psr
	write	"ip: %s(ip) psr: %s(psr)"
;	mtspr	r3, psr
	ldi	r55, 120
	mtspr	r55, tpr
	write	"fpcr    %s(fpcr)"
	write	"psr     %s(psr)"

	write	"test long loop"
; test simple loop
; fast_check
	ldi	r13, 350000 ; 35
	ldi	r14, 350000 ; 35
	ldi	r15, 88
	write	"%i64(r14)"
repeat_loop_start: (128)
;	write	"%i64(r12)"
	addi	r13, r13, 3
	add	r13, r13, r15
	srpi	r13, r13, r15, 8

	addi	r13, r13, 4
	add	r13, r13, r15
	srpi	r13, r13, r15, 7

	addi	r13, r13, 5
	add	r13, r13, r15
	srpi	r13, r13, r15, 6

	addi	r13, r13, 6
	add	r13, r13, r15
	srpi	r13, r13, r15, 5

	sub	r13, r13, r15
	sladd	r13, r13, r15, 5
	sladd	r13, r13, r15, 5

	xor	r13, r14, r15
	sll	r13, r13, r13
	repgt	r14, gz, -1, repeat_loop_start

	write	"%i64(r13) %i64(r14)"
end_test_long_loop:
	write	"end test long loop"
.end
