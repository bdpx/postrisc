.text
    alloc	25
    ldi.l	%r23, 0x1234567890abcdef
    write	"test population statistic instructions"
    cnt.pop	%r12, %r23, 3
    write	"cntpop: %i64(r12)"
    cnt.lz	%r12, %r23, 0
    write	"cntlz %i64(r12)"
    cnt.tz	%r12, %r23, 1
    cnt.lz	%r12, %r23, 2
    cnt.tz	%r12, %r23, 3
    cnt.lz	%r12, %r23, 4
    cnt.tz	%r12, %r23, 5
.end
