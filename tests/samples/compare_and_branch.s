.text
backward_target:
    alloc	61
    addi	%r2, %r2, 1
    b.eq.d	%r2, %r2, branch_test_exit

    b.eq.d	%r23, %r34, backward_target
    b.eq.d.l	%r23, %r34, backward_target
    b.eq.d	%r23, %r34, forward_target
    b.eq.d.l	%r23, %r34, forward_target
    bi.eq.d	%r23,34, backward_target
    bi.eq.d.l	%r23,34, backward_target
    bi.eq.d	%r23,34, forward_target
    bi.eq.d.l	%r23,34, forward_target

    b.eq.w	%r23, %r34, backward_target
    b.eq.w.l	%r23, %r34, backward_target
    b.eq.w	%r23, %r34, forward_target
    b.eq.w.l	%r23, %r34, forward_target
    bi.eq.w	%r23,34, backward_target
    bi.eq.w.l	%r23,34, backward_target
    bi.eq.w	%r23,34, forward_target
    bi.eq.w.l	%r23,34, forward_target

    b.ne.d	%r23, %r34, backward_target
    b.ne.d.l	%r23, %r34, backward_target
    b.ne.d	%r23, %r34, forward_target
    b.ne.d.l	%r23, %r34, forward_target
    bi.ne.d	%r23,34, backward_target
    bi.ne.d.l	%r23,34, backward_target
    bi.ne.d	%r23,34, forward_target
    bi.ne.d.l	%r23,34, forward_target

    b.ne.w	%r23, %r34, backward_target
    b.ne.w.l	%r23, %r34, backward_target
    b.ne.w	%r23, %r34, forward_target
    b.ne.w.l	%r23, %r34, forward_target
    bi.ne.w	%r23,34, backward_target
    bi.ne.w.l	%r23,34, backward_target
    bi.ne.w	%r23,34, forward_target
    bi.ne.w.l	%r23,34, forward_target

    bs.le.d	%r23, %r34, backward_target
    bs.le.d.l	%r23, %r34, backward_target
    bs.le.d	%r23, %r34, forward_target
    bs.le.d.l	%r23, %r34, forward_target
    bsi.le.d	%r23,34, backward_target
    bsi.le.d.l	%r23,34, backward_target
    bsi.le.d	%r23,34, forward_target
    bsi.le.d.l	%r23,34, forward_target

    bs.le.w	%r23, %r34, backward_target
    bs.le.w.l	%r23, %r34, backward_target
    bs.le.w	%r23, %r34, forward_target
    bs.le.w.l	%r23, %r34, forward_target
    bsi.le.w	%r23,34, backward_target
    bsi.le.w.l	%r23,34, backward_target
    bsi.le.w	%r23,34, forward_target
    bsi.le.w.l	%r23,34, forward_target

    bs.lt.d	%r23, %r34, backward_target
    bs.lt.d.l	%r23, %r34, backward_target
    bs.lt.d	%r23, %r34, forward_target
    bs.lt.d.l	%r23, %r34, forward_target
    bsi.lt.d	%r23,34, backward_target
    bsi.lt.d.l	%r23,34, backward_target
    bsi.lt.d	%r23,34, forward_target
    bsi.lt.d.l	%r23,34, forward_target

    bs.lt.w	%r23, %r34, backward_target
    bs.lt.w.l	%r23, %r34, backward_target
    bs.lt.w	%r23, %r34, forward_target
    bs.lt.w.l	%r23, %r34, forward_target
    bsi.lt.w	%r23,34, backward_target
    bsi.lt.w.l	%r23,34, backward_target
    bsi.lt.w	%r23,34, forward_target
    bsi.lt.w.l	%r23,34, forward_target

    bs.ge.d	%r23, %r34, backward_target
    bs.ge.d.l	%r23, %r34, backward_target
    bs.ge.d	%r23, %r34, forward_target
    bs.ge.d.l	%r23, %r34, forward_target
    bui.ge.d	%r23,34, backward_target
    bui.ge.d.l	%r23,34, backward_target
    bui.ge.d	%r23,34, forward_target
    bui.ge.d.l	%r23,34, forward_target

    bs.ge.w	%r23, %r34, backward_target
    bs.ge.w.l	%r23, %r34, backward_target
    bs.ge.w	%r23, %r34, forward_target
    bs.ge.w.l	%r23, %r34, forward_target
    bui.ge.w	%r23,34, backward_target
    bui.ge.w.l	%r23,34, backward_target
    bui.ge.w	%r23,34, forward_target
    bui.ge.w.l	%r23,34, forward_target

    bs.gt.d	%r23, %r34, backward_target
    bs.gt.d.l	%r23, %r34, backward_target
    bs.gt.d	%r23, %r34, forward_target
    bs.gt.d.l	%r23, %r34, forward_target
    bsi.gt.d	%r23,34, backward_target
    bsi.gt.d.l	%r23,34, backward_target
    bsi.gt.d	%r23,34, forward_target
    bsi.gt.d.l	%r23,34, forward_target

    bs.gt.w	%r23, %r34, backward_target
    bs.gt.w.l	%r23, %r34, backward_target
    bs.gt.w	%r23, %r34, forward_target
    bs.gt.w.l	%r23, %r34, forward_target
    bsi.gt.w	%r23,34, backward_target
    bsi.gt.w.l	%r23,34, backward_target
    bsi.gt.w	%r23,34, forward_target
    bsi.gt.w.l	%r23,34, forward_target

    bu.le.d	%r23, %r34, backward_target
    bu.le.d.l	%r23, %r34, backward_target
    bu.le.d	%r23, %r34, forward_target
    bu.le.d.l	%r23, %r34, forward_target
    bui.le.d	%r23,34, backward_target
    bui.le.d.l	%r23,34, backward_target
    bui.le.d	%r23,34, forward_target
    bui.le.d.l	%r23,34, forward_target

    bu.le.w	%r23, %r34, backward_target
    bu.le.w.l	%r23, %r34, backward_target
    bu.le.w	%r23, %r34, forward_target
    bu.le.w.l	%r23, %r34, forward_target
    bui.le.w	%r23,34, backward_target
    bui.le.w.l	%r23,34, backward_target
    bui.le.w	%r23,34, forward_target
    bui.le.w.l	%r23,34, forward_target

    bu.lt.d	%r23, %r34, backward_target
    bu.lt.d.l	%r23, %r34, backward_target
    bu.lt.d	%r23, %r34, forward_target
    bu.lt.d.l	%r23, %r34, forward_target
    bui.lt.d	%r23,34, backward_target
    bui.lt.d.l	%r23,34, backward_target
    bui.lt.d	%r23,34, forward_target
    bui.lt.d.l	%r23,34, forward_target

    bu.lt.w	%r23, %r34, backward_target
    bu.lt.w.l	%r23, %r34, backward_target
    bu.lt.w	%r23, %r34, forward_target
    bu.lt.w.l	%r23, %r34, forward_target
    bui.lt.w	%r23,34, backward_target
    bui.lt.w.l	%r23,34, backward_target
    bui.lt.w	%r23,34, forward_target
    bui.lt.w.l	%r23,34, forward_target

    bu.ge.d	%r23, %r34, backward_target
    bu.ge.d.l	%r23, %r34, backward_target
    bu.ge.d	%r23, %r34, forward_target
    bu.ge.d.l	%r23, %r34, forward_target
    bui.ge.d	%r23,34, backward_target
    bui.ge.d.l	%r23,34, backward_target
    bui.ge.d	%r23,34, forward_target
    bui.ge.d.l	%r23,34, forward_target

    bu.ge.w	%r23, %r34, backward_target
    bu.ge.w.l	%r23, %r34, backward_target
    bu.ge.w	%r23, %r34, forward_target
    bu.ge.w.l	%r23, %r34, forward_target
    bui.ge.w	%r23,34, backward_target
    bui.ge.w.l	%r23,34, backward_target
    bui.ge.w	%r23,34, forward_target
    bui.ge.w.l	%r23,34, forward_target

    bu.gt.d	%r23, %r34, backward_target
    bu.gt.d.l	%r23, %r34, backward_target
    bu.gt.d	%r23, %r34, forward_target
    bu.gt.d.l	%r23, %r34, forward_target
    bui.gt.d	%r23, 34, backward_target
    bui.gt.d.l	%r23, 34, backward_target
    bui.gt.d	%r23, 34, forward_target
    bui.gt.d.l	%r23, 34, forward_target

    bu.gt.w	%r23, %r34, backward_target
    bu.gt.w.l	%r23, %r34, backward_target
    bu.gt.w	%r23, %r34, forward_target
    bu.gt.w.l	%r23, %r34, forward_target
    bui.gt.w	%r23, 34, backward_target
    bui.gt.w.l	%r23, 34, backward_target
    bui.gt.w	%r23, 34, forward_target
    bui.gt.w.l	%r23, 34, forward_target

    bm.all	%r23, 34, backward_target
    bm.all.l	%r23, 34, backward_target
    bm.all	%r23, 34, forward_target
    bm.all.l	%r23, 34, forward_target

    bm.notall	%r23, 34, backward_target
    bm.notall.l	%r23, 34, backward_target
    bm.notall	%r23, 34, forward_target
    bm.notall.l	%r23, 34, forward_target

    bm.any	%r23, 34, backward_target
    bm.any.l	%r23, 34, backward_target
    bm.any	%r23, 34, forward_target
    bm.any.l	%r23, 34, forward_target

    bm.none	%r23, 34, backward_target
    bm.none.l	%r23, 34, backward_target
    bm.none	%r23, 34, forward_target
    bm.none.l	%r23, 34, forward_target

forward_target:
branch_test_exit:

    jmp		branch_exit

label:
    b.eq.d	%r12, %r13, qwe
    srpi	%r10, %r11, %r12, 45
    dep.q	%r61, %r91, %r32, 10
    mbsel	%r62, %r91, %r32, %r10
    perm	%r63, %r91, %r32, %r10
qwe:
    b.ne.d	%r15, %r46, label
    b.eq.d	%r25, %r45, label
    bs.lt.d	%r25, %r44, label
    bs.le.d	%r35, %r43, label
    bu.gt.d	%r35, %r42, label
    bu.ge.d	%r45, %r41, label
    bs.gt.d	%r45, %r40, label
    bu.lt.d	%r55, %r76, label
    bi.ne.d	%r55, 140, label
    bi.eq.d	%r65, 141, label
    bsi.lt.d	%r65, 142, label
    bsi.gt.d	%r75, 143, label
    bui.lt.d	%r75, 170, label
    bui.gt.d	%r85, 160, label

    addi.l	%r45, %r34, 1234
    b.bsi	%r85, 26, label
    b.bci.l	%r85, 36, label
    b.bsi	%r95, 46, label
    b.bci.l	%r95, 56, label

    jmp.r	%r45, %r23, 1
branch_exit:
    write	"end branch test"
.end
