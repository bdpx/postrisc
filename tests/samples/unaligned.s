.text
.data
data_unaligned:
    d4	0x33221100
    d4	0x77665544
    d4	0xbbaa9988
    d4	0xffeeddcc

.text
    write	"load/store unaligned"
    alloc	96
    ldafr	%r17, data_unaligned

    ldwz	%r3, %r17, 1
    write	"%x32(r3)"
    ldwz	%r3, %r17, 2
    write	"%x32(r3)"
    ldwz	%r3, %r17, 3
    write	"%x32(r3)"
    ldwz	%r3, %r17, 4
    write	"%x32(r3)"
.end
