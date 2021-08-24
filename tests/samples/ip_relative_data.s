.rodata
rodata1:
    d1	123
    align	2
rodata2:
    d2	12345
    align	4
rodata4:
    d4	123456789
    align	8
rodata8:
    d8	1234567890123456789

.data
data1:
    d1	123
    align	2
data2:
    d2	12345
    align	4
data4:
    d4	123456789
    align	8
data8:
    d8	1234567890123456789

.text
    alloc	96

    write "test ip-relative data addressing"
    ldbzr	%r34, rodata1
    ldhzr	%r34, rodata2
    ldwzr	%r34, rodata4
    lddzr	%r34, rodata8

    ldbsr	%r34, rodata1
    ldhsr	%r34, rodata2
    ldwsr	%r34, rodata4
    lddsr	%r34, rodata8

    ldbzr	%r34, data1
    ldhzr	%r34, data2
    ldwzr	%r34, data4
    lddzr	%r34, data8

    ldbsr	%r34, data1
    ldhsr	%r34, data2
    ldwsr	%r34, data4
    lddsr	%r34, data8

    stbr	%r34, data1
    sthr	%r34, data2
    stwr	%r34, data4
    stdr	%r34, data8

    write	"end ip-relative data test"
.end