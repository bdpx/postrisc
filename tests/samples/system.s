.text
    alloc 70
    write "test system instructions (assembler only)"

    addid %sp, %sp, -32 ; alloc stack frame
    write "test tpa for sp: 0x%x64(sp)"
    tpa %r4, %sp, %gz
    write "tpa(sp): 0x%x64(r4)"
    addid %sp, %sp, 32 ; rollback stack frame

    jmp system_skip

    ldi %r45, 1012
    syscall
    nop 0
    sysret
    rfi

    icbi %r34, 16
    dcbt %r34, 16
    dcbf %r34, 16
    dcbi %r34, 16


    getspr %r34, %lid
    setspr %r34, %lid
    mprobe %r34, %r45, %r66
    retf 234567

    getspr %r32, %iv
    getspr %r32, %psr

; test system instructions
    ptc %r10, %r45, %r11

    getspr %r12, %pta
    getspr %r12, %fpcr
    getspr %r11, %rsc

; test atomic fences
    fence acquire
    fence release
    fence acq_rel
    fence seq_cst

    setdbr %r44, %r66, 0
    getdbr %r55, %r66, 0
    setibr %r44, %r66, 0
    getibr %r55, %r66, 0
    setitr %r44, %r66, %r12
    setdtr %r44, %r66, %r12

; bpa b7, %r7
; bpal b7, b4, %r6
; lpr b7, %r6, label16

    undef
system_skip:
    write "end test system instructions (assembler only)"
.end
