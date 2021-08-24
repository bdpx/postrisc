.rodata
rodata1:
    d1 123
    align 2
rodata2:
    d2 12345
    align 4
rodata4:
    d4 123456789
    align 8
rodata8:
    d8 1234567890123456789

.data
data1:
    d1 123
    align 2
data2:
    d2 12345
    align 4
data4:
    d4 123456789
    align 8
data8:
    d8 1234567890123456789

.text
    alloc 96

    write "test ip-relative data addressing"
    ldubr %r34, rodata1
    lduhr %r34, rodata2
    lduwr %r34, rodata4
    ldudr %r34, rodata8

    ldsbr %r34, rodata1
    ldshr %r34, rodata2
    ldswr %r34, rodata4
    ldsdr %r34, rodata8

    ldubr %r34, data1
    lduhr %r34, data2
    lduwr %r34, data4
    ldudr %r34, data8

    ldsbr %r34, data1
    ldshr %r34, data2
    ldswr %r34, data4
    ldsdr %r34, data8

    stbr %r34, data1
    sthr %r34, data2
    stwr %r34, data4
    stdr %r34, data8

    write "end ip-relative data test"
.end
