.text
    alloc	61
    write	"Example of test bit and branch"
    ldi		%r19, 0x20
    ldi		%r20, 12+3
    write	"%i64(r20)"
    ldi		%r10, 0
    b.bci	%r10, 10, xxx_n
    ldi.l	%r20, 123456789012345	; load immediate
    ldi		%r21, 321		; load immediate
    add		%r23, %r20, %r21	; add
    write	"%i64(r43)"
xxx_n:	write	"%i64(r23)"

    ldi		%r46, 0xabcdef
    b.bci	%r46, 56, branch_bit_exit
    b.bsi	%r46, 56, branch_bit_exit
    ldi		%r56, 56
    b.bc	%r46, %r56, branch_bit_exit
    b.bs	%r46, %r56, branch_bit_exit

branch_bit_exit:
    write	"end branch_bit test"
.end
