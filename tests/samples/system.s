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
    setdbr %r44, %r66, 100
    setdbr.l %r44, %r66, 10000000
    getdbr %r55, %r66, 0
    getdbr %r55, %r66, 100
    getdbr.l %r55, %r66, 1000000
    setibr %r44, %r66, 0
    setibr %r44, %r66, 100
    setibr.l %r44, %r66, 1000000
    getibr %r55, %r66, 0
    getibr %r55, %r66, 100
    getibr.l %r55, %r66, 1000000000
    setitr %r44, %r66, %r12
    setdtr %r44, %r66, %r12

    int %r44, 0
    int %r44, 100
    int.l %r44, 10000000
    int.l %r44, 10000000000000
    int.l %r44, 1000000000000005

    setmr %r14, %gz, 100
    setmr.l %r14, %gz, 1000000
    setmr.l %r14, %gz, 1000000000

    getmr %r15, %gz, 100
    getmr.l %r15, %gz, 1000000
    getmr.l %r15, %gz, 1000000000

; bpa b7, %r7
; bpal b7, b4, %r6
; lpr b7, %r6, label16

    undef
system_skip:
    write "end test system instructions (assembler only)"
.end
