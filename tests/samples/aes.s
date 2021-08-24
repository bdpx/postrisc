.text
    alloc	96
    write	"test carry-less multiply"
    clmulll	%r34, %r21, %r22
    clmulhl	%r34, %r21, %r22
    clmulhl	%r34, %r21, %r22
    clmulhh	%r34, %r21, %r22
.rodata
align 16
vector_a:
    d8	0x7b5b546573745665
    d8	0x63746f725d53475d
vector_b:
    d8	0x4869285368617929
    d8	0x5b477565726f6e5d
result_00:
    d8	0x1d4d84c85c3440c0
    d8	0x929633d5d36f0451
result_01:
    d8	0x1bd17c8d556ab5a1
    d8	0x7fa540ac2a281315
result_10:
    d8	0x1a2bf6db3a30862f
    d8	0xbabf262df4b7d5c9
result_11:
    d8	0x1d1e1f2c592e7c45
    d8	0xd66ee03e410fd4ed
.text
    ldqr	%r12, vector_a
    ldqr	%r13, vector_b

    clmulll	%r11, %r12, %r13
    ldqr	%r21, result_00
    write	"clmul: %x128(r11) %x128(r21)"
    clmulhl	%r11, %r13, %r12
    ldqr	%r21, result_01
    write	"clmul: %x128(r11) %x128(r21)"
    clmulhl	%r11, %r12, %r13
    ldqr	%r21, result_10
    write	"clmul: %x128(r11) %x128(r21)"
    clmulhh	%r11, %r12, %r13
    ldqr	%r21, result_11
    write	"clmul: %x128(r11) %x128(r21)"

    write	"test aes"
    aesdec	%r11, %r12, %r13
    aesdeclast	%r11, %r12, %r13
    aesenc	%r11, %r12, %r13
    aesenclast	%r11, %r12, %r13
    aesimc	%r11, %r12
    aeskeygenassist %r11, %r12, 250
    write	"end aes test"
.end
