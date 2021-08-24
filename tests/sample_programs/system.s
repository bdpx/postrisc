.text
	write	"test system instructions (assembler only)"

	addi	sp, sp, -32	; alloc stack frame
	write	"test tpa for sp: 0x%x64(sp)"
	tpa	r4, sp
	write	"tpa(sp): 0x%x64(r4)"
	addi	sp, sp, 32	; rollback stack frame
	
	br	system_skip

	ldi	r45, 1012
	syscall	r23, r43, 412
after_syscall:

	sysret
	rfi

	icbi	r34, 16
	dcbt	r34, 16
	dcbf	r34, 16
	dcbi	r34, 16


	mfspr	r34, lid
	mtspr	r34, lid
	mprobe	r34, r45,r66
	ret     r0

	mfspr	r32, iv
	sprsr	r32, psr, r32, r32

; test system instructions
	ptc	r18, 16
	ptc	r45, 16
	mfspr	r12, pta
	msync
	mfspr	r12, fpcr
	mtspr	r11, rsc

	mtdbr	r44, r66
	mfdbr	r55, r66
	mtibr	r44, r66
	mfibr	r55, r66
	mtitr	r44, r66
	mtdtr	r44, r66

;	bpa	b7,r7
;	bpal	b7,b4,r6
;	lpr	b7,r6,label16

	undef	60000000
system_skip:
	write	"end test system instructions (assembler only)"
.end