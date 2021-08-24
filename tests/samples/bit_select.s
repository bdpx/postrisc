.text
    alloc 96
    write "check bit selection instruction"
    ldi.l %r6, ((0x3333333333333333 ^ 0x5555555555555555) & 0xff00ff00ff00ff00) ^ 0x5555555555555555
    write "expect: %x64(r6)"
    ldi.l %r3, 0x3333333333333333
    ldi.l %r4, 0x5555555555555555
    ldi.l %r5, 0xff00ff00ff00ff00
    bitslct %r6, %r3, %r4, %r5
    write "result: %x64(r6)"

    write "end_bitselect_test"
.end
