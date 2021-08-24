.text
    write "test bit-field insert (deposit)"
    alloc 96
    ld_imm.l %r30, 0xaaaaaaaaaaaaaaaa
    ld_imm.l %r40, 0xeeeeeeeeeeeeeeee
    deposit %r20, %r30, %r40, 40, 24
    write "dep: %x64(r20)"
    deposit %r20, %r40, %r30, 40, 24
    write "dep: %x64(r20)"

    write "test vector deposit (dep16)"
    nor %r3, %r4, %r4
    deposit %r5, %r3, %r4, 100, 40
    write "dep16: %x128(r5)"
    write "end deposit test"
.end
