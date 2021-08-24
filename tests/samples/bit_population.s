.text
    alloc 25
    ld_imm.l %r23, 0x1234567890abcdef
    write "test population statistic instructions"
    cnt_pop %r12, %r23, 3
    write "cntpop: %i64(r12)"
    cnt_lz %r12, %r23, 0
    write "cntlz %i64(r12)"
    cnt_tz %r12, %r23, 1
    cnt_lz %r12, %r23, 2
    cnt_tz %r12, %r23, 3
    cnt_lz %r12, %r23, 4
    cnt_tz %r12, %r23, 5
.end
