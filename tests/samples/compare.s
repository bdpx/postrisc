.text
    write	"test compare"
    alloc	96
    ldi		%r20, 4
    ldi		%r21, 3
    ldi		%r22, -4
    ldi		%r23, -12
    write	"test compare instructions"
    cmpdeq	%r12, %r20, %r21
    cmpdlt	%r12, %r20, %r21
    cmpdltu	%r12, %r20, %r21
    cmpdeqi	%r12, %r20, 123456
    cmpdlti	%r12, %r20, 123456
    cmpdltui	%r12, %r20, 123456
    cmpdne	%r12, %r20, %r21
    cmpdnei	%r12, %r20, 123456
    cmpdgti	%r12, %r20, 123456
    cmpdgtui	%r12, %r20, 123456
    cmpdle	%r12, %r20, %r21
    cmpdleu	%r12, %r20, %r21

    cmpdgei	%r12, %r20, 123456
    cmpdgeui	%r12, %r20, 123456
    cmpdlei	%r12, %r20, 123456
    cmpdleui	%r12, %r20, 123456

    cmpweq	%r12, %r20, %r21
    cmpwlt	%r12, %r20, %r21
    cmpwltu	%r12, %r20, %r21
    cmpweqi	%r12, %r20, 123456
    cmpwlti	%r12, %r20, 123456
    cmpwltui	%r12, %r20, 123456
    cmpwne	%r12, %r20, %r21
    cmpwnei	%r12, %r20, 123456
    cmpwgti	%r12, %r20, 123456
    cmpwgtui	%r12, %r20, 123456
    cmpwle	%r12, %r20, %r21
    cmpwleu	%r12, %r20, %r21

    write	"compare aliases (pseudo-instructions)"
    cmpdgt	%r12, %r20, %r21	; cmplt	  r12, %r21, r20
    cmpdgtu	%r12, %r20, %r21	; cmpltu  r12, %r21, r20
    cmpdlti	%r12, %r20, 123456	; cmplti  r12, %r20, 12346
    cmpdltui	%r12, %r20, 123456	; cmpltui r12, %r20, 12346
    cmpdge	%r12, %r20, %r21	; cmpleq  r12, %r21, r20
    cmpdgeu	%r12, %r20, %r21	; cmpleu  r12, %r21, r20
    cmpdgti	%r12, %r20, 123456	; cmpgti  r12, %r20, 12346
    cmpdgtui	%r12, %r20, 123456	; cmpgtui r12, %r20, 12346


    cmpwgt	%r12, %r20, %r21	; cmplt4   r12, %r21, %r20
    cmpwgtu	%r12, %r20, %r21	; cmpltu4  r12, %r21, %r20
    cmpwlti	%r12, %r20, 123456	; cmplti4  r12, %r20, 12346
    cmpwltui	%r12, %r20, 123456	; cmpltui4 r12, %r20, 12346
    cmpwge	%r12, %r20, %r21	; cmpleq4  r12, %r21, r20
    cmpwgeu	%r12, %r20, %r21	; cmpleu4  r12, %r21, r20
    cmpwgti	%r12, %r20, 123456	; cmpgti4  r12, %r20, 12346
    cmpwgtui	%r12, %r20, 123456	; cmpgtui4 r12, %r20, 12346

; TESTS
    cmpdeq	%r14, %r12, %r45
    cmpdne	%r14, %r12, %r45

    cmpdeq	%r14, %r45, %r34
    cmpdeqi	%r14, %r45, 123
    cmpdeqi.l	%r14, %r45, 1234567890123
    cmpdlti	%r14, %r45, 123
    cmpdlti.l	%r14, %r45, 1234567890123
    cmpdlei	%r14, %r45, 123
    cmpdlei.l	%r14, %r45, 1234567890123
    cmpdlt	%r14, %r45, %r34
    cmpdgtui	%r14, %r45, 123
    cmpdgtui.l	%r14, %r45, 1234567890123
    cmpdgeui	%r14, %r45, 123
    cmpdgeui.l	%r14, %r45, 1234567890123
    cmpdgtu	%r14, %r45, %r34

    cmpdeq	%r41, %r34, %r56
    cmpdlt	%r66, %r45, %r57
    cmpdeqi	%r64, %r56, 0
.end