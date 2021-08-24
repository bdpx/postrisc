.text
.data
data_unaligned:
align 16
    d1	0x00
    d1	0x01
    d1	0x02
    d1	0x03
    d1	0x04
    d1	0x05
    d1	0x06
    d1	0x07
    d1	0x08
    d1	0x09
    d1	0x0a
    d1	0x0b
    d1	0x0c
    d1	0x0d
    d1	0x0e
    d1	0x0f

    d1	0x10
    d1	0x11
    d1	0x12
    d1	0x13
    d1	0x14
    d1	0x15
    d1	0x16
    d1	0x17
    d1	0x18
    d1	0x19
    d1	0x1a
    d1	0x1b
    d1	0x1c
    d1	0x1d
    d1	0x1e
    d1	0x1f

.text
    write "load/store unaligned"
    alloc 96
    lda_iprel %r17, data_unaligned

    ld_u16	%r3, %r17, 0
    write	"%x16(r3)"
    ld_u16	%r3, %r17, 1
    write	"%x16(r3)"
    ld_u16	%r3, %r17, 2
    write	"%x16(r3)"

    ld_u32	%r3, %r17, 0
    write	"%x32(r3)"
    ld_u32	%r3, %r17, 1
    write	"%x32(r3)"
    ld_u32	%r3, %r17, 2
    write	"%x32(r3)"
    ld_u32	%r3, %r17, 3
    write	"%x32(r3)"
    ld_u32	%r3, %r17, 4
    write	"%x32(r3)"

    ld_u64	%r3, %r17, 0
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 1
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 2
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 3
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 4
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 5
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 6
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 7
    write	"%x64(r3)"
    ld_u64	%r3, %r17, 8
    write	"%x64(r3)"

    ld_i128	%r3, %r17, 0
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 1
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 2
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 3
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 4
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 5
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 6
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 7
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 8
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 9
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 10
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 11
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 12
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 13
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 14
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 15
    write	"%x128(r3)"
    ld_i128	%r3, %r17, 16
    write	"%x128(r3)"
.end
