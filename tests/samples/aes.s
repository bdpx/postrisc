.text
    alloc	96
    write	"test carry-less multiply"
    clmul.ll	%r34, %r21, %r22
    clmul.hl	%r34, %r21, %r22
    clmul.hl	%r34, %r21, %r22
    clmul.hh	%r34, %r21, %r22
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
    ld.q.r	%r12, vector_a
    ld.q.r	%r13, vector_b

    clmul.ll	%r11, %r12, %r13
    ld.q.r	%r21, result_00
    write	"clmul: %x128(r11) %x128(r21)"
    clmul.hl	%r11, %r13, %r12
    ld.q.r	%r21, result_01
    write	"clmul: %x128(r11) %x128(r21)"
    clmul.hl	%r11, %r12, %r13
    ld.q.r	%r21, result_10
    write	"clmul: %x128(r11) %x128(r21)"
    clmul.hh	%r11, %r12, %r13
    ld.q.r	%r21, result_11
    write	"clmul: %x128(r11) %x128(r21)"

    write	"test aes"
    aes.dec	%r11, %r12, %r13
    aes.dec.last	%r11, %r12, %r13
    aes.enc	%r11, %r12, %r13
    aes.enc.last	%r11, %r12, %r13
    aes.imc	%r11, %r12
    aes.keygen.assist %r11, %r12, 250
    write	"end aes test"
.end
