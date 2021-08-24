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
    ldz.b.r	%r34, rodata1
    ldz.h.r	%r34, rodata2
    ldz.w.r	%r34, rodata4
    ldz.d.r	%r34, rodata8

    lds.b.r	%r34, rodata1
    lds.h.r	%r34, rodata2
    lds.w.r	%r34, rodata4
    lds.d.r	%r34, rodata8

    ldz.b.r	%r34, data1
    ldz.h.r	%r34, data2
    ldz.w.r	%r34, data4
    ldz.d.r	%r34, data8

    lds.b.r	%r34, data1
    lds.h.r	%r34, data2
    lds.w.r	%r34, data4
    lds.d.r	%r34, data8

    st.b.r	%r34, data1
    st.h.r	%r34, data2
    st.w.r	%r34, data4
    st.d.r	%r34, data8

    write	"end ip-relative data test"
.end
