.text
backward_target:
    alloc 61
    addid %r2, %r2, 1
    breqd %r2, %r2, branch_test_exit

    breqd  %r23, %r34, backward_target
    breqd.l  %r23, %r34, backward_target
    breqd  %r23, %r34, forward_target
    breqd.l  %r23, %r34, forward_target
    breqid  %r23,34, backward_target
    breqid.l  %r23,34, backward_target
    breqid  %r23,34, forward_target
    breqid.l  %r23,34, forward_target

    breqw  %r23, %r34, backward_target
    breqw.l  %r23, %r34, backward_target
    breqw  %r23, %r34, forward_target
    breqw.l  %r23, %r34, forward_target
    breqiw  %r23,34, backward_target
    breqiw.l  %r23,34, backward_target
    breqiw  %r23,34, forward_target
    breqiw.l  %r23,34, forward_target

    brned  %r23, %r34, backward_target
    brned.l  %r23, %r34, backward_target
    brned  %r23, %r34, forward_target
    brned.l  %r23, %r34, forward_target
    brneid  %r23,34, backward_target
    brneid.l  %r23,34, backward_target
    brneid  %r23,34, forward_target
    brneid.l  %r23,34, forward_target

    brnew  %r23, %r34, backward_target
    brnew.l  %r23, %r34, backward_target
    brnew  %r23, %r34, forward_target
    brnew.l  %r23, %r34, forward_target
    brneiw  %r23,34, backward_target
    brneiw.l  %r23,34, backward_target
    brneiw  %r23,34, forward_target
    brneiw.l  %r23,34, forward_target

    brgesd  %r23, %r34, backward_target
    brgesd.l  %r23, %r34, backward_target
    brgesd  %r23, %r34, forward_target
    brgesd.l  %r23, %r34, forward_target

    brgesw  %r23, %r34, backward_target
    brgesw.l  %r23, %r34, backward_target
    brgesw  %r23, %r34, forward_target
    brgesw.l  %r23, %r34, forward_target

    brltsd  %r23, %r34, backward_target
    brltsd.l  %r23, %r34, backward_target
    brltsd  %r23, %r34, forward_target
    brltsd.l  %r23, %r34, forward_target
    brltsid  %r23,34, backward_target
    brltsid.l  %r23,34, backward_target
    brltsid  %r23,34, forward_target
    brltsid.l  %r23,34, forward_target

    brltsw  %r23, %r34, backward_target
    brltsw.l  %r23, %r34, backward_target
    brltsw  %r23, %r34, forward_target
    brltsw.l  %r23, %r34, forward_target
    brltsiw  %r23,34, backward_target
    brltsiw.l  %r23,34, backward_target
    brltsiw  %r23,34, forward_target
    brltsiw.l  %r23,34, forward_target

    brgeuid  %r23,34, backward_target
    brgeuid.l  %r23,34, backward_target
    brgeuid  %r23,34, forward_target
    brgeuid.l  %r23,34, forward_target

    brgeuiw  %r23,34, backward_target
    brgeuiw.l  %r23,34, backward_target
    brgeuiw  %r23,34, forward_target
    brgeuiw.l  %r23,34, forward_target

    brgeud%r23, %r34, backward_target
    brgeud.l  %r23, %r34, backward_target
    brgeud  %r23, %r34, forward_target
    brgeud.l  %r23, %r34, forward_target

    brgeuw  %r23, %r34, backward_target
    brgeuw.l  %r23, %r34, backward_target
    brgeuw  %r23, %r34, forward_target
    brgeuw.l  %r23, %r34, forward_target

    brltud  %r23, %r34, backward_target
    brltud.l  %r23, %r34, backward_target
    brltud  %r23, %r34, forward_target
    brltud.l  %r23, %r34, forward_target

    brltuw  %r23, %r34, backward_target
    brltuw.l  %r23, %r34, backward_target
    brltuw  %r23, %r34, forward_target
    brltuw.l  %r23, %r34, forward_target

    brgeuid  %r23,34, backward_target
    brgeuid.l  %r23,34, backward_target
    brgeuid  %r23,34, forward_target
    brgeuid.l  %r23,34, forward_target

    brgeuiw  %r23,34, backward_target
    brgeuiw.l  %r23,34, backward_target
    brgeuiw  %r23,34, forward_target
    brgeuiw.l  %r23,34, forward_target

    brmall  %r23, 34, backward_target
    brmall.l  %r23, 34, backward_target
    brmall  %r23, 34, forward_target
    brmall.l  %r23, 34, forward_target

    brmnotall  %r23, 34, backward_target
    brmnotall.l  %r23, 34, backward_target
    brmnotall  %r23, 34, forward_target
    brmnotall.l  %r23, 34, forward_target

    brmany   %r23, 34, backward_target
    brmany.l %r23, 34, backward_target
    brmany   %r23, 34, forward_target
    brmany.l %r23, 34, forward_target

    brmnone   %r23, 34, backward_target
    brmnone.l %r23, 34, backward_target
    brmnone   %r23, 34, forward_target
    brmnone.l %r23, 34, forward_target

forward_target:
branch_test_exit:

    jmp branch_exit

label:
    breqd %r12, %r13, qwe
    srpid %r10, %r11, %r12, 45
    depositv %r61, %r91, %r32, %r10
    bitslct %r62, %r91, %r32, %r10
    vpermb %r63, %r91, %r32, %r10
qwe:
    brned %r15, %r46, label
    breqd %r25, %r45, label
    brltsd %r25, %r44, label
    brgesd %r35, %r43, label
    brltud %r55, %r76, label
    brneid %r55, 140, label
    breqid %r65, 141, label
    brltsid %r65, 142, label
    brltuid %r75, 170, label
    brgeuid %r85, 160, label

    addid.l %r45, %r34, 1234
    brbsi %r85, 26, label
    brbci.l %r85, 36, label
    brbsi %r95, 46, label
    brbci.l %r95, 56, label

    jmpr %r45, %r23, 1
branch_exit:
    write  "end branch test"
.end
