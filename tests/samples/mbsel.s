.text
    alloc	96
    write	"check mbsel instruction"
    ld_imm.l	%r6, ((0x3333333333333333 ^ 0x5555555555555555) & 0xff00ff00ff00ff00) ^ 0x5555555555555555
    write	"mbsel: %x64(r6)"
    ld_imm.l	%r3, 0x3333333333333333
    ld_imm.l	%r4, 0x5555555555555555
    ld_imm.l	%r5, 0xff00ff00ff00ff00
    mbsel	%r6, %r3, %r4, %r5
    write	"mbsel: %x64(r6)"

    write	"end_mbsel_test"
.end
