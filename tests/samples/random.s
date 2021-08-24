.text
    write "test random"
    alloc 96

    random %r3, %gz
    write "random: %x64(r3)"
    random %r3, %gz
    write "random: %x64(r3)"
    ld_imm %r4, 1
    random %r3, %r4
    write "random seed: %x64(r3)"

    write "end_random_test"
.end
