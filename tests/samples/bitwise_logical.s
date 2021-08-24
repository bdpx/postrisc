.text
    alloc	46
    write	"test bitwise logical"
    and		%r23, %r25, %r45
    andi	%r23, %r25, 12345
    andi.l	%r23, %r25, 1234567890
    andn	%r23, %r25, %r45
    andni	%r23, %r25, 12345
    or		%r23, %r25, %r45
    ori		%r23, %r25, 12345
    ori.l	%r23, %r25, 1234567890
    orn		%r23, %r25, %r45
    orni	%r23, %r25, 12345
    xor		%r23, %r25, %r45
    xori	%r23, %r25, 12345
    xori.l	%r23, %r25, 1234567890
    nor		%r23, %r25, %r45
    nand	%r23, %r25, %r45
    xnor	%r23, %r25, %r45
.end