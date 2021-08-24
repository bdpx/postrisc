.text
    write "test compare"
    alloc 96
    ldi %r20, 4
    ldi %r21, 3
    ldi %r22, -4
    ldi %r23, -12
    write "test compare instructions"

    cmpeqd %r12, %r20, %r21
    cmpltsd %r12, %r20, %r21
    cmpltud %r12, %r20, %r21
    cmpeqid %r12, %r20, 123456
    cmpltsid %r12, %r20, 123456
    cmpltuid %r12, %r20, 123456
    cmpned %r12, %r20, %r21
    cmpneid %r12, %r20, 123456
    cmpgesid %r12, %r20, 123456
    cmpgeuid %r12, %r20, 123456
    cmpgesd %r12, %r20, %r21
    cmpgeud %r12, %r20, %r21

    cmpgesiw %r12, %r20, 123456
    cmpgeuiw %r12, %r20, 123456
    cmpgesid %r12, %r20, 123456
    cmpgeuid %r12, %r20, 123456

    cmpeqw %r12, %r20, %r21
    cmpltsw %r12, %r20, %r21
    cmpltuw %r12, %r20, %r21
    cmpeqiw %r12, %r20, 123456
    cmpltsiw %r12, %r20, 123456
    cmpltuiw %r12, %r20, 123456
    cmpnew %r12, %r20, %r21
    cmpneiw %r12, %r20, 123456
    cmpgesiw %r12, %r20, 123456
    cmpgeuiw %r12, %r20, 123456
    cmpgesw %r12, %r20, %r21
    cmpgeuw %r12, %r20, %r21

    cmpltsd %r12, %r20, %r21
    cmpltud %r12, %r20, %r21
    cmpltsid %r12, %r20, 123456
    cmpltuid %r12, %r20, 123456
    cmpltsd %r12, %r20, %r21
    cmpltud %r12, %r20, %r21
    cmpgesid %r12, %r20, 123456
    cmpgeuid %r12, %r20, 123456


    cmpltsw %r12, %r20, %r21
    cmpltuw %r12, %r20, %r21
    cmpltsiw %r12, %r20, 123456
    cmpltuiw %r12, %r20, 123456
    cmpltsw %r12, %r20, %r21
    cmpltuw %r12, %r20, %r21
    cmpgesiw %r12, %r20, 123456
    cmpgeuiw %r12, %r20, 123456

; TESTS
    cmpeqd %r14, %r12, %r45
    cmpned %r14, %r12, %r45

    cmpeqd %r14, %r45, %r34
    cmpeqid %r14, %r45, 123
    cmpeqid.l %r14, %r45, 1234567890123
    cmpltsid %r14, %r45, 123
    cmpltsid.l %r14, %r45, 1234567890123
    cmpgesid %r14, %r45, 123
    cmpgesid.l %r14, %r45, 1234567890123
    cmpltsd %r14, %r45, %r34
    cmpgeuid %r14, %r45, 123
    cmpgeuid.l %r14, %r45, 1234567890123
    cmpgeuid %r14, %r45, 123
    cmpgeuid.l %r14, %r45, 1234567890123
    cmpltud %r14, %r45, %r34

    cmpeqd %r41, %r34, %r56
    cmpltsd %r66, %r45, %r57
    cmpeqid %r64, %r56, 0
.end
