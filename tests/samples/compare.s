.text
    write	"test compare"
    alloc	96
    ldi		%r20, 4
    ldi		%r21, 3
    ldi		%r22, -4
    ldi		%r23, -12
    write	"test compare instructions"
    cmp.eq.d	%r12, %r20, %r21
    cmps.lt.d	%r12, %r20, %r21
    cmpu.lt.d	%r12, %r20, %r21
    cmpi.eq.d	%r12, %r20, 123456
    cmpsi.lt.d	%r12, %r20, 123456
    cmpui.lt.d	%r12, %r20, 123456
    cmp.ne.d	%r12, %r20, %r21
    cmpi.ne.d	%r12, %r20, 123456
    cmpsi.gt.d	%r12, %r20, 123456
    cmpui.gt.d	%r12, %r20, 123456
    cmps.le.d	%r12, %r20, %r21
    cmpu.le.d	%r12, %r20, %r21

    cmpsi.ge.d	%r12, %r20, 123456
    cmpui.ge.d	%r12, %r20, 123456
    cmpsi.le.d	%r12, %r20, 123456
    cmpui.le.d	%r12, %r20, 123456

    cmp.eq.w	%r12, %r20, %r21
    cmps.lt.w	%r12, %r20, %r21
    cmpu.lt.w	%r12, %r20, %r21
    cmpi.eq.w	%r12, %r20, 123456
    cmpsi.lt.w	%r12, %r20, 123456
    cmpui.lt.w	%r12, %r20, 123456
    cmp.ne.w	%r12, %r20, %r21
    cmpi.ne.w	%r12, %r20, 123456
    cmpsi.gt.w	%r12, %r20, 123456
    cmpui.gt.w	%r12, %r20, 123456
    cmps.le.w	%r12, %r20, %r21
    cmpu.le.w	%r12, %r20, %r21

    write	"compare aliases (pseudo-instructions)"
    cmps.gt.d	%r12, %r20, %r21	; cmplt   r12, %r21, r20
    cmpu.gt.d	%r12, %r20, %r21	; cmpltu  r12, %r21, r20
    cmpsi.lt.d	%r12, %r20, 123456	; cmplti  r12, %r20, 12346
    cmpui.lt.d	%r12, %r20, 123456	; cmpltui r12, %r20, 12346
    cmps.ge.d	%r12, %r20, %r21	; cmpleq  r12, %r21, r20
    cmpu.ge.d	%r12, %r20, %r21	; cmpleu  r12, %r21, r20
    cmpsi.gt.d	%r12, %r20, 123456	; cmpgti  r12, %r20, 12346
    cmpui.gt.d	%r12, %r20, 123456	; cmpgtui r12, %r20, 12346


    cmps.gt.w	%r12, %r20, %r21	; cmplt4   r12, %r21, %r20
    cmpu.gt.w	%r12, %r20, %r21	; cmpltu4  r12, %r21, %r20
    cmpsi.lt.w	%r12, %r20, 123456	; cmplti4  r12, %r20, 12346
    cmpui.lt.w	%r12, %r20, 123456	; cmpltui4 r12, %r20, 12346
    cmps.ge.w	%r12, %r20, %r21	; cmpleq4  r12, %r21, r20
    cmpu.ge.w	%r12, %r20, %r21	; cmpleu4  r12, %r21, r20
    cmpsi.gt.w	%r12, %r20, 123456	; cmpgti4  r12, %r20, 12346
    cmpui.gt.w	%r12, %r20, 123456	; cmpgtui4 r12, %r20, 12346

; TESTS
    cmp.eq.d	%r14, %r12, %r45
    cmp.ne.d	%r14, %r12, %r45

    cmp.eq.d	%r14, %r45, %r34
    cmpi.eq.d	%r14, %r45, 123
    cmpi.eq.d.l	%r14, %r45, 1234567890123
    cmpsi.lt.d	%r14, %r45, 123
    cmpsi.lt.d.l	%r14, %r45, 1234567890123
    cmpsi.le.d	%r14, %r45, 123
    cmpsi.le.d.l	%r14, %r45, 1234567890123
    cmps.lt.d	%r14, %r45, %r34
    cmpui.gt.d	%r14, %r45, 123
    cmpui.gt.d.l	%r14, %r45, 1234567890123
    cmpui.ge.d	%r14, %r45, 123
    cmpui.ge.d.l	%r14, %r45, 1234567890123
    cmpu.gt.d	%r14, %r45, %r34

    cmp.eq.d	%r41, %r34, %r56
    cmps.lt.d	%r66, %r45, %r57
    cmpi.eq.d	%r64, %r56, 0
.end
