.text
    write	"test bit reverse instruction (permb)"
    alloc	80
    ld_imm.l	%r55, 0x1234567890ABCDEF
    write	"initial value: %x64(r55)"
    permb	%r55, %r55, 63
    permb	%r56, %r78, 63
    write	"r55 %x64(r55) %b64(r55)"
    permb	%r55, %r55, 63
    write	"r55 %x64(r55) %b64(r55)"

    permb	%r56, %r55, 0b111111 ;63
    write	"reverse bits: %x64(r56)"

    permb	%r56, %r55, 0b111110  ;32+16+8+4+2
    write	"reverse bit-pairs: %x64(r56)"

    permb	%r56, %r55, 0b111100  ;32+16+8+4
    write	"reverse nibbles (4-bits): %x64(r56)"

    permb	%r56, %r55, 0b111000 ;32+16+8
    write	"reverse 1bytes: %x64(r55) => %x64(r56)"

    permb	%r56, %r55, 0b110000  ;32+16
    write	"reverse 2bytes: %x64(r55) => %x64(r56)"

    permb	%r56, %r55, 0b100000  ;32
    write	"reverse 4bytes: %x64(r55) => %x64(r56)"
.end
