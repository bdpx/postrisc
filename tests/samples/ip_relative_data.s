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
    ld_iprel_u8  %r34, rodata1
    ld_iprel_u16 %r34, rodata2
    ld_iprel_u32 %r34, rodata4
    ld_iprel_u64 %r34, rodata8

    ld_iprel_i8  %r34, rodata1
    ld_iprel_i16 %r34, rodata2
    ld_iprel_i32 %r34, rodata4
    ld_iprel_i64 %r34, rodata8

    ld_iprel_u8  %r34, data1
    ld_iprel_u16 %r34, data2
    ld_iprel_u32 %r34, data4
    ld_iprel_u64 %r34, data8

    ld_iprel_i8  %r34, data1
    ld_iprel_i16 %r34, data2
    ld_iprel_i32 %r34, data4
    ld_iprel_i64 %r34, data8

    st_iprel_i8  %r34, data1
    st_iprel_i16 %r34, data2
    st_iprel_i32 %r34, data4
    st_iprel_i64 %r34, data8

    write "end ip-relative data test"
.end
