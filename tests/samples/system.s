.text
    alloc 70
    write "test system instructions (assembler only)"

    add_imm_i64 %sp, %sp, -32 ; alloc stack frame
    write "test tpa for sp: 0x%x64(sp)"
    tpa %r4, %sp, %gz
    write "tpa(sp): 0x%x64(r4)"
    add_imm_i64 %sp, %sp, 32 ; rollback stack frame

    jmp system_skip

    ld_imm %r45, 1012
    syscall
    nop 0
    sysret
    rfi

    icbi %r34, 16
    dcbt %r34, 16
    dcbf %r34, 16
    dcbi %r34, 16


    get_spr %r34, %lid
    set_spr %r34, %lid
    mprobe %r34, %r45, %r66
    retf 234567

    get_spr %r32, %iv
    get_spr %r32, %psr

; test system instructions
    ptc %r10, %r45, %r11

    get_spr %r12, %pta
    get_spr %r12, %fpcr
    get_spr %r11, %rsc

; test atomic fences
    fence_a
    fence_r
    fence_ar
    fence_sc

    set_dbr %r44, %r66, 0
    get_dbr %r55, %r66, 0
    set_ibr %r44, %r66, 0
    get_ibr %r55, %r66, 0
    set_itr %r44, %r66, %r12
    set_dtr %r44, %r66, %r12

; bpa b7, %r7
; bpal b7, b4, %r6
; lpr b7, %r6, label16

    undef
system_skip:
    write "end test system instructions (assembler only)"
.end
