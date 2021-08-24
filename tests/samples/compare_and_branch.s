.text
backward_target:
    alloc	61
    addi	%r2, %r2, 1
    bdeq	%r2, %r2, branch_test_exit

    bdeq	%r23, %r34, backward_target
    bdeq.l	%r23, %r34, backward_target
    bdeq	%r23, %r34, forward_target
    bdeq.l	%r23, %r34, forward_target
    bdeqi	%r23,34, backward_target
    bdeqi.l	%r23,34, backward_target
    bdeqi	%r23,34, forward_target
    bdeqi.l	%r23,34, forward_target

    bweq	%r23, %r34, backward_target
    bweq.l	%r23, %r34, backward_target
    bweq	%r23, %r34, forward_target
    bweq.l	%r23, %r34, forward_target
    bweqi	%r23,34, backward_target
    bweqi.l	%r23,34, backward_target
    bweqi	%r23,34, forward_target
    bweqi.l	%r23,34, forward_target

    bdne	%r23, %r34, backward_target
    bdne.l	%r23, %r34, backward_target
    bdne	%r23, %r34, forward_target
    bdne.l	%r23, %r34, forward_target
    bdnei	%r23,34, backward_target
    bdnei.l	%r23,34, backward_target
    bdnei	%r23,34, forward_target
    bdnei.l	%r23,34, forward_target

    bwne	%r23, %r34, backward_target
    bwne.l	%r23, %r34, backward_target
    bwne	%r23, %r34, forward_target
    bwne.l	%r23, %r34, forward_target
    bwnei	%r23,34, backward_target
    bwnei.l	%r23,34, backward_target
    bwnei	%r23,34, forward_target
    bwnei.l	%r23,34, forward_target

    bdle	%r23, %r34, backward_target
    bdle.l	%r23, %r34, backward_target
    bdle	%r23, %r34, forward_target
    bdle.l	%r23, %r34, forward_target
    bdlei	%r23,34, backward_target
    bdlei.l	%r23,34, backward_target
    bdlei	%r23,34, forward_target
    bdlei.l	%r23,34, forward_target

    bwle	%r23, %r34, backward_target
    bwle.l	%r23, %r34, backward_target
    bwle	%r23, %r34, forward_target
    bwle.l	%r23, %r34, forward_target
    bwlei	%r23,34, backward_target
    bwlei.l	%r23,34, backward_target
    bwlei	%r23,34, forward_target
    bwlei.l	%r23,34, forward_target

    bdlt	%r23, %r34, backward_target
    bdlt.l	%r23, %r34, backward_target
    bdlt	%r23, %r34, forward_target
    bdlt.l	%r23, %r34, forward_target
    bdlti	%r23,34, backward_target
    bdlti.l	%r23,34, backward_target
    bdlti	%r23,34, forward_target
    bdlti.l	%r23,34, forward_target

    bwlt	%r23, %r34, backward_target
    bwlt.l	%r23, %r34, backward_target
    bwlt	%r23, %r34, forward_target
    bwlt.l	%r23, %r34, forward_target
    bwlti	%r23,34, backward_target
    bwlti.l	%r23,34, backward_target
    bwlti	%r23,34, forward_target
    bwlti.l	%r23,34, forward_target

    bdge	%r23, %r34, backward_target
    bdge.l	%r23, %r34, backward_target
    bdge	%r23, %r34, forward_target
    bdge.l	%r23, %r34, forward_target
    bdgeui	%r23,34, backward_target
    bdgeui.l	%r23,34, backward_target
    bdgeui	%r23,34, forward_target
    bdgeui.l	%r23,34, forward_target

    bwge	%r23, %r34, backward_target
    bwge.l	%r23, %r34, backward_target
    bwge	%r23, %r34, forward_target
    bwge.l	%r23, %r34, forward_target
    bwgeui	%r23,34, backward_target
    bwgeui.l	%r23,34, backward_target
    bwgeui	%r23,34, forward_target
    bwgeui.l	%r23,34, forward_target

    bdgt	%r23, %r34, backward_target
    bdgt.l	%r23, %r34, backward_target
    bdgt	%r23, %r34, forward_target
    bdgt.l	%r23, %r34, forward_target
    bdgti	%r23,34, backward_target
    bdgti.l	%r23,34, backward_target
    bdgti	%r23,34, forward_target
    bdgti.l	%r23,34, forward_target

    bwgt	%r23, %r34, backward_target
    bwgt.l	%r23, %r34, backward_target
    bwgt	%r23, %r34, forward_target
    bwgt.l	%r23, %r34, forward_target
    bwgti	%r23,34, backward_target
    bwgti.l	%r23,34, backward_target
    bwgti	%r23,34, forward_target
    bwgti.l	%r23,34, forward_target

    bdleu	%r23, %r34, backward_target
    bdleu.l	%r23, %r34, backward_target
    bdleu	%r23, %r34, forward_target
    bdleu.l	%r23, %r34, forward_target
    bdleui	%r23,34, backward_target
    bdleui.l	%r23,34, backward_target
    bdleui	%r23,34, forward_target
    bdleui.l	%r23,34, forward_target

    bwleu	%r23, %r34, backward_target
    bwleu.l	%r23, %r34, backward_target
    bwleu	%r23, %r34, forward_target
    bwleu.l	%r23, %r34, forward_target
    bwleui	%r23,34, backward_target
    bwleui.l	%r23,34, backward_target
    bwleui	%r23,34, forward_target
    bwleui.l	%r23,34, forward_target

    bdltu	%r23, %r34, backward_target
    bdltu.l	%r23, %r34, backward_target
    bdltu	%r23, %r34, forward_target
    bdltu.l	%r23, %r34, forward_target
    bdltui	%r23,34, backward_target
    bdltui.l	%r23,34, backward_target
    bdltui	%r23,34, forward_target
    bdltui.l	%r23,34, forward_target

    bwltu	%r23, %r34, backward_target
    bwltu.l	%r23, %r34, backward_target
    bwltu	%r23, %r34, forward_target
    bwltu.l	%r23, %r34, forward_target
    bwltui	%r23,34, backward_target
    bwltui.l	%r23,34, backward_target
    bwltui	%r23,34, forward_target
    bwltui.l	%r23,34, forward_target

    bdgeu	%r23, %r34, backward_target
    bdgeu.l	%r23, %r34, backward_target
    bdgeu	%r23, %r34, forward_target
    bdgeu.l	%r23, %r34, forward_target
    bdgeui	%r23,34, backward_target
    bdgeui.l	%r23,34, backward_target
    bdgeui	%r23,34, forward_target
    bdgeui.l	%r23,34, forward_target

    bwgeu	%r23, %r34, backward_target
    bwgeu.l	%r23, %r34, backward_target
    bwgeu	%r23, %r34, forward_target
    bwgeu.l	%r23, %r34, forward_target
    bwgeui	%r23,34, backward_target
    bwgeui.l	%r23,34, backward_target
    bwgeui	%r23,34, forward_target
    bwgeui.l	%r23,34, forward_target

    bdgtu	%r23, %r34, backward_target
    bdgtu.l	%r23, %r34, backward_target
    bdgtu	%r23, %r34, forward_target
    bdgtu.l	%r23, %r34, forward_target
    bdgtui	%r23, 34, backward_target
    bdgtui.l	%r23, 34, backward_target
    bdgtui	%r23, 34, forward_target
    bdgtui.l	%r23, 34, forward_target

    bwgtu	%r23, %r34, backward_target
    bwgtu.l	%r23, %r34, backward_target
    bwgtu	%r23, %r34, forward_target
    bwgtu.l	%r23, %r34, forward_target
    bwgtui	%r23, 34, backward_target
    bwgtui.l	%r23, 34, backward_target
    bwgtui	%r23, 34, forward_target
    bwgtui.l	%r23, 34, forward_target

    bmall	%r23, 34, backward_target
    bmall.l	%r23, 34, backward_target
    bmall	%r23, 34, forward_target
    bmall.l	%r23, 34, forward_target

    bmnotall	%r23, 34, backward_target
    bmnotall.l	%r23, 34, backward_target
    bmnotall	%r23, 34, forward_target
    bmnotall.l	%r23, 34, forward_target

    bmany	%r23, 34, backward_target
    bmany.l	%r23, 34, backward_target
    bmany	%r23, 34, forward_target
    bmany.l	%r23, 34, forward_target

    bmnone	%r23, 34, backward_target
    bmnone.l	%r23, 34, backward_target
    bmnone	%r23, 34, forward_target
    bmnone.l	%r23, 34, forward_target

forward_target:
branch_test_exit:

    jmp		branch_exit

label:
    bdeq	%r12, %r13, qwe
    srpi	%r10, %r11, %r12, 45
    depq	%r61, %r91, %r32, 10
    mbsel	%r62, %r91, %r32, %r10
    perm	%r63, %r91, %r32, %r10
qwe:
    bdne	%r15, %r46, label
    bdeq	%r25, %r45, label
    bdlt	%r25, %r44, label
    bdle	%r35, %r43, label
    bdgtu	%r35, %r42, label
    bdgeu	%r45, %r41, label
    bdgt	%r45, %r40, label
    bdltu	%r55, %r76, label
    bdnei	%r55, 140, label
    bdeqi	%r65, 141, label
    bdlti	%r65, 142, label
    bdgti	%r75, 143, label
    bdltui	%r75, 170, label
    bdgtui	%r85, 160, label

    addi.l	%r45, %r34, 1234
    bbsi	%r85, 26, label
    bbci.l	%r85, 36, label
    bbsi	%r95, 46, label
    bbci.l	%r95, 56, label

    jmpr	%r45, %r23, 1
branch_exit:
    write	"end branch test"
.end
