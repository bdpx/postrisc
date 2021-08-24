.text
    alloc	70
    write	"test system instructions (assembler only)"

    addi	%sp, %sp, -32	; alloc stack frame
    write	"test tpa for sp: 0x%x64(sp)"
    tpa		%r4, %sp
    write	"tpa(sp): 0x%x64(r4)"
    addi	%sp, %sp, 32	; rollback stack frame
    
    jmp		system_skip

    ldi		%r45, 1012
    syscall
    nop		0
    sysret
    rfi

    icb.i	%r34, 16
    dcb.t	%r34, 16
    dcb.f	%r34, 16
    dcb.i	%r34, 16


    mf.spr	%r34, %lid
    mt.spr	%r34, %lid
    m.probe	%r34, %r45, %r66
    ret.f	234567

    mf.spr	%r32, %iv
    mf.spr	%r32, %psr

; test system instructions
    ptc		%r10, %r45, %r11

    mf.spr	%r12, %pta
    mf.spr	%r12, %fpcr
    mt.spr	%r11, %rsc

; test atomic fences
    fence.a
    fence.r
    fence.ar
    fence.sc

    mt.dbr	%r44, %r66, 0
    mf.dbr	%r55, %r66, 0
    mt.ibr	%r44, %r66, 0
    mf.ibr	%r55, %r66, 0
    mt.itr	%r44, %r66, %r12
    mt.dtr	%r44, %r66, %r12

;	bpa	b7, %r7
;	bpal	b7, b4, %r6
;	lpr	b7, %r6, label16

    undef
system_skip:
    write	"end test system instructions (assembler only)"
.end
