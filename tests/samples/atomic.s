.text
    alloc 96
    write "test atomic fetch-op"
    addi %r5, %sp, -64
    write "atomic base: %x64(r5)"
    ldi  %r10, 5
    ldi  %r12, 10
    ldi  %r56, 5

    write "test amo-add"

    ld.add.b %r4, %r5, %r10
    ld.add.b.a %r4, %r5, %r10
    ld.add.b.r %r4, %r5, %r10
    ld.add.b.ar %r4, %r5, %r10

    ld.add.h %r4, %r5, %r10
    ld.add.h.a %r4, %r5, %r10
    ld.add.h.r %r4, %r5, %r10
    ld.add.h.ar %r4, %r5, %r10

    ld.add.w %r4, %r5, %r10
    ld.add.w.a %r4, %r5, %r10
    ld.add.w.r %r4, %r5, %r10
    ld.add.w.ar %r4, %r5, %r10

    ld.add.d %r4, %r5, %r10
    ld.add.d.a %r4, %r5, %r10
    ld.add.d.r %r4, %r5, %r10
    ld.add.d.ar %r4, %r5, %r10

    ld.add.q %r4, %r5, %r10
    ld.add.q.a %r4, %r5, %r10
    ld.add.q.r %r4, %r5, %r10
    ld.add.q.ar %r4, %r5, %r10

    write "test amo-and"

    ld.and.b %r4, %r5, %r10
    ld.and.b.a %r4, %r5, %r10
    ld.and.b.r %r4, %r5, %r10
    ld.and.b.ar %r4, %r5, %r10

    ld.and.h %r4, %r5, %r10
    ld.and.h.a %r4, %r5, %r10
    ld.and.h.r %r4, %r5, %r10
    ld.and.h.ar %r4, %r5, %r10

    ld.and.w %r4, %r5, %r10
    ld.and.w.a %r4, %r5, %r10
    ld.and.w.r %r4, %r5, %r10
    ld.and.w.ar %r4, %r5, %r10

    ld.and.d %r4, %r5, %r10
    ld.and.d.a %r4, %r5, %r10
    ld.and.d.r %r4, %r5, %r10
    ld.and.d.ar %r4, %r5, %r10

    ld.and.q %r4, %r5, %r10
    ld.and.q.a %r4, %r5, %r10
    ld.and.q.r %r4, %r5, %r10
    ld.and.q.ar %r4, %r5, %r10

    write "test amo-or"

    ld.or.b %r4, %r5, %r10
    ld.or.b.a %r4, %r5, %r10
    ld.or.b.r %r4, %r5, %r10
    ld.or.b.ar %r4, %r5, %r10

    ld.or.h %r4, %r5, %r10
    ld.or.h.a %r4, %r5, %r10
    ld.or.h.r %r4, %r5, %r10
    ld.or.h.ar %r4, %r5, %r10

    ld.or.w %r4, %r5, %r10
    ld.or.w.a %r4, %r5, %r10
    ld.or.w.r %r4, %r5, %r10
    ld.or.w.ar %r4, %r5, %r10

    ld.or.d %r4, %r5, %r10
    ld.or.d.a %r4, %r5, %r10
    ld.or.d.r %r4, %r5, %r10
    ld.or.d.ar %r4, %r5, %r10

    ld.or.q %r4, %r5, %r10
    ld.or.q.a %r4, %r5, %r10
    ld.or.q.r %r4, %r5, %r10
    ld.or.q.ar %r4, %r5, %r10

    write "test amo-xor"

    ld.xor.b %r4, %r5, %r10
    ld.xor.b.a %r4, %r5, %r10
    ld.xor.b.r %r4, %r5, %r10
    ld.xor.b.ar %r4, %r5, %r10

    ld.xor.h %r4, %r5, %r10
    ld.xor.h.a %r4, %r5, %r10
    ld.xor.h.r %r4, %r5, %r10
    ld.xor.h.ar %r4, %r5, %r10

    ld.xor.w %r4, %r5, %r10
    ld.xor.w.a %r4, %r5, %r10
    ld.xor.w.r %r4, %r5, %r10
    ld.xor.w.ar %r4, %r5, %r10

    ld.xor.d %r4, %r5, %r10
    ld.xor.d.a %r4, %r5, %r10
    ld.xor.d.r %r4, %r5, %r10
    ld.xor.d.ar %r4, %r5, %r10

    ld.xor.q %r4, %r5, %r10
    ld.xor.q.a %r4, %r5, %r10
    ld.xor.q.r %r4, %r5, %r10
    ld.xor.q.ar %r4, %r5, %r10

    write "test amo-smin"
    ld.smin.b %r4, %r5, %r10
    ld.smin.b.a %r4, %r5, %r10
    ld.smin.b.r %r4, %r5, %r10
    ld.smin.b.ar %r4, %r5, %r10

    ld.smin.h %r4, %r5, %r10
    ld.smin.h.a %r4, %r5, %r10
    ld.smin.h.r %r4, %r5, %r10
    ld.smin.h.ar %r4, %r5, %r10

    ld.smin.w %r4, %r5, %r10
    ld.smin.w.a %r4, %r5, %r10
    ld.smin.w.r %r4, %r5, %r10
    ld.smin.w.ar %r4, %r5, %r10

    ld.smin.d %r4, %r5, %r10
    ld.smin.d.a %r4, %r5, %r10
    ld.smin.d.r %r4, %r5, %r10
    ld.smin.d.ar %r4, %r5, %r10

    ld.smin.q %r4, %r5, %r10
    ld.smin.q.a %r4, %r5, %r10
    ld.smin.q.r %r4, %r5, %r10
    ld.smin.q.ar %r4, %r5, %r10

    write "test amo-smax"
    ld.smax.b %r4, %r5, %r10
    ld.smax.b.a %r4, %r5, %r10
    ld.smax.b.r %r4, %r5, %r10
    ld.smax.b.ar %r4, %r5, %r10

    ld.smax.h %r4, %r5, %r10
    ld.smax.h.a %r4, %r5, %r10
    ld.smax.h.r %r4, %r5, %r10
    ld.smax.h.ar %r4, %r5, %r10

    ld.smax.w %r4, %r5, %r10
    ld.smax.w.a %r4, %r5, %r10
    ld.smax.w.r %r4, %r5, %r10
    ld.smax.w.ar %r4, %r5, %r10

    ld.smax.d %r4, %r5, %r10
    ld.smax.d.a %r4, %r5, %r10
    ld.smax.d.r %r4, %r5, %r10
    ld.smax.d.ar %r4, %r5, %r10

    ld.smax.q %r4, %r5, %r10
    ld.smax.q.a %r4, %r5, %r10
    ld.smax.q.r %r4, %r5, %r10
    ld.smax.q.ar %r4, %r5, %r10

    write "test amo-umin"
    ld.umin.b %r4, %r5, %r10
    ld.umin.b.a %r4, %r5, %r10
    ld.umin.b.r %r4, %r5, %r10
    ld.umin.b.ar %r4, %r5, %r10

    ld.umin.h %r4, %r5, %r10
    ld.umin.h.a %r4, %r5, %r10
    ld.umin.h.r %r4, %r5, %r10
    ld.umin.h.ar %r4, %r5, %r10

    ld.umin.w %r4, %r5, %r10
    ld.umin.w.a %r4, %r5, %r10
    ld.umin.w.r %r4, %r5, %r10
    ld.umin.w.ar %r4, %r5, %r10

    ld.umin.d %r4, %r5, %r10
    ld.umin.d.a %r4, %r5, %r10
    ld.umin.d.r %r4, %r5, %r10
    ld.umin.d.ar %r4, %r5, %r10

    ld.umin.q %r4, %r5, %r10
    ld.umin.q.a %r4, %r5, %r10
    ld.umin.q.r %r4, %r5, %r10
    ld.umin.q.ar %r4, %r5, %r10

    write "test amo-umax"
    ld.umax.b %r4, %r5, %r10
    ld.umax.b.a %r4, %r5, %r10
    ld.umax.b.r %r4, %r5, %r10
    ld.umax.b.ar %r4, %r5, %r10

    ld.umax.h %r4, %r5, %r10
    ld.umax.h.a %r4, %r5, %r10
    ld.umax.h.r %r4, %r5, %r10
    ld.umax.h.ar %r4, %r5, %r10

    ld.umax.w %r4, %r5, %r10
    ld.umax.w.a %r4, %r5, %r10
    ld.umax.w.r %r4, %r5, %r10
    ld.umax.w.ar %r4, %r5, %r10

    ld.umax.d %r4, %r5, %r10
    ld.umax.d.a %r4, %r5, %r10
    ld.umax.d.r %r4, %r5, %r10
    ld.umax.d.ar %r4, %r5, %r10

    ld.umax.q %r4, %r5, %r10
    ld.umax.q.a %r4, %r5, %r10
    ld.umax.q.r %r4, %r5, %r10
    ld.umax.q.ar %r4, %r5, %r10

    write "test cas"

    cas.b %r12, %r5, %r56
    cas.b.a %r12, %r5, %r56
    cas.b.r %r12, %r5, %r56
    cas.b.ar %r12, %r5, %r56

    cas.h %r12, %r5, %r56
    cas.h.a %r12, %r5, %r56
    cas.h.r %r12, %r5, %r56
    cas.h.ar %r12, %r5, %r56

    cas.w %r12, %r5, %r56
    cas.w.a %r12, %r5, %r56
    cas.w.r %r12, %r5, %r56
    cas.w.ar %r12, %r5, %r56

    cas.d %r12, %r5, %r56
    cas.d.a %r12, %r5, %r56
    cas.d.r %r12, %r5, %r56
    cas.d.ar %r12, %r5, %r56

    cas.q %r12, %r5, %r56
    cas.q.a %r12, %r5, %r56
    cas.q.r %r12, %r5, %r56
    cas.q.ar %r12, %r5, %r56

    write "test load atomic relaxed"
    lda.b %r12, %r5
    lda.h %r12, %r5
    lda.w %r12, %r5
    lda.d %r12, %r5
    lda.q %r12, %r5

    write "test load atomic acquire"
    lda.b.a %r12, %r5
    lda.h.a %r12, %r5
    lda.w.a %r12, %r5
    lda.d.a %r12, %r5
    lda.q.a %r12, %r5

    write "test store atomic relaxed"
    sta.b %r12, %r5
    sta.h %r12, %r5
    sta.w %r12, %r5
    sta.d %r12, %r5
    sta.q %r12, %r5

    write "test store atomic release"
    sta.b.r %r12, %r5
    sta.h.r %r12, %r5
    sta.w.r %r12, %r5
    sta.d.r %r12, %r5
    sta.q.r %r12, %r5

.end
