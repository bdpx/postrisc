.text
    write "example of carry/borrow testing"
    alloc	96

; 256-bit add (g30,%r31,r32,r33) + (g40,r41,r42,r43) => (g50,r51,r52,r53)
    ldi	%r30, -1
    ldi	%r31, -1
    ldi	%r34, -1
    ldi	%r33, -1

    ldi	%r40, 1
    ldi	%r41, 0
    ldi	%r42, 0
    ldi	%r43, 0

; throw add
    cmpdeq	%r10, %r30, %r40	; add carry out
    add		%r50, %r30, %r40	; add
    cmpdeqi	%r12, %r31, 1
    addi	%r51, %r31, 1

    cmpdeq	%r12, %r31, %r41	; add carry out
    add		%r51, %r31, %r41	; add
    cmpdeq	%r14, %r34, %r42	; add carry out
    add		%r52, %r34, %r42	; add
    cmpdeq	%r8, %r33, %r43	; add carry out
    add		%r53, %r33, %r43	; add
    write	"add carryis"
    addi	%r51, %r51, 1
    addi	%r52, %r52, 1
    addi	%r53, %r53, 1
; set last carry
    ldi		%r54, 1
    ldi		%r54, 0
    write	"multiprecision add:\nr50,r51,r52,r53,r54 = %x64(r50) %x64(r51) %x64(r52) %x64(r53) %x64(r54)"

    ldi.l	%r40, 0x7fffffffffffffff
    mulh	%r40, %r40, %r41
    write	"r40     %x64(r40)"

    ldi		%r12, 12345
    ldi.l	%r12, 12345678900

;	ldi	%r14, 0xFFFFFFFFF0
;	ld8	%r13, %r14, 0

    addc	%r12, %r14, %r46
    addc	%r12, %r14, %r46
    subb	%r12, %r14, %r46
    subb	%r12, %r14, %r46
    addaddc	%r12, %r14, %r46, %r23
    addaddc	%r12, %r14, %r46, %r22
    subsubb	%r12, %r14, %r46, %r13
    subsubb	%r12, %r14, %r46, %r14
    write	"end carry test"
    nop	11111
.end
