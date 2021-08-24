.text
    alloc	25
    ldi.l	%r23, 0x1234567890abcdef
    write	"test population statistic instructions"
    cntpop	%r12, %r23, 3
    write	"cntpop: %i64(r12)"
    cntlz	%r12, %r23, 0
    write	"cntlz %i64(r12)"
    cnttz	%r12, %r23, 1
    cntlz	%r12, %r23, 2
    cnttz	%r12, %r23, 3
    cntlz	%r12, %r23, 4
    cnttz	%r12, %r23, 5
.end