.text
; floating-point extension example
    alloc	96

    write	"test float128 immediate load (low/high parts)"
    fldri.q	%r12, 3.1415926115461431423612436243
    write	"fldqri: %f128(r12)"

    write	"test fpcr modification (rm=3)"
    ldi		%r2, 3
    mt.spr	%r2, %fpcr
    write	"fpcr: %s(fpcr)"
    write	"test fpcr modification (rm=2)"
    ldi		%r2, 2
    mt.spr	%r2, %fpcr
    write	"fpcr: %s(fpcr)"
    write	"test fpcr modification (rm=1)"
    ldi		%r2, 1
    mt.spr	%r2, %fpcr
    write	"fpcr: %s(fpcr)"
    write	"test fpcr modification (rm=0)"
    ldi		%r2, 0
    mt.spr	%r2, %fpcr
    write	"fpcr: %s(fpcr)"

    write	"compare fldqri (full mantissa) & long fldi (63-bit mantissa)"
    fldri.q	%r30, 3.14159265358979323846123456789012e+400
    write	"fldqri: %x128(r30) %f128(r30)"
    fldi.d	%r31, 3.14159265358979323846123456789012
    write	"flddi: %x128(r31) %f64(r31)"
    write	"compare fldqri (full mantissa) & short fldi (21-bit mantissa)"
    fldri.q	%r30, 3.14159265358979323846123456789012
    write	"r30     %x128(r30)"
    fldi.d	%r31, 3.14159265358979323846123456789012
    write	"r31     %x128(r31)"
    write	"before1"
    write	"r30     %f128(r30)"
    write	"before2"
    write	"r31     %vf64(r31)"
    write	"after"
    fldi.d	%r30, -12.3456789e+04
.rodata
    align	16
float64data:
    double	1.234567890123456789124141241241
    double	3.1415925678888734535345231234564561
    double	3.4566345634563456346535463463456
.text
    ca.r	%r21, float64data
    ldz.d	%r11, %r21, 8*0
    ldz.d	%r12, %r21, 8*1
    ldz.d	%r13, %r21, 8*2
    write	"ld8(f64): %f64(r11) %f64(r12) %f64(r13)"
    fldri.q	%r14, 2.7182818289201
    write	"fldqri: %f128(r14)"

    fext.sd.sq	%r11, %r11
    fext.sd.sq	%r12, %r12
    fext.sd.sq	%r13, %r13

    write	"test binary"
    fmul.sq	%r15, %r11, %r14
    write	"fmulsq:  %f128(r15)"
    fnmul.sq	%r15, %r11, %r14
    write	"fnmulsq: %f128(r15)"
    fadd.sq	%r15, %r11, %r14
    write	"faddsq:  %f128(r15)"
    fnadd.sq	%r15, %r11, %r14
    write	"fnaddsq: %f128(r15)"
    fsub.sq	%r15, %r14, %r11
    write	"fsubsq:  %f128(r15)"
    fdiv.sq	%r15, %r14, %r11
    write	"fdivsq:  %f128(r15)"

    write	"test fused fma"
;   jmp	skipfma
    fmadd.sq	%r15, %r14, %r11, %r12
    write	"fmaddsq:  %f128(r15)"
    fnmadd.sq %r15, %r14, %r11, %r12
    write	"fnmaddsq: %f128(r15)"
    fmsub.sq	%r15, %r14, %r11, %r12
    write	"fmsubsq:  %f128(r15)"
    fnmsub.sq %r15, %r14, %r11, %r12
    write	"fnmsubsq: %f128(r15)"

    write	"test unary"
    mov		%r16, %r15
    write	"r16     %f128(r16)"
    fabs.sq	%r16, %r15
    write	"r16     %f128(r16)"
    fneg.sq	%r16, %r15
    write	"r16     %f128(r16)"
    fnabs.sq	%r16, %r15
    write	"r16     %f128(r16)"
    fsqrt.sq	%r16, %r12
    write	"r16     %f128(r16)"
    frsqrt.sq	%r16, %r12
    write	"r16     %f128(r16)"

    write	"test rounding"
    frnd.sq	%r17, %r12, 4
    write	"r17     %f128(r17)"
    frnd.sq	%r17, %r12, 3
    write	"r17     %f128(r17)"
    frnd.sq	%r17, %r12, 2
    write	"r17     %f128(r17)"
    frnd.sq	%r17, %r12, 0
    write	"r17     %f128(r17)"
    fcvt.sq.iw	%r17, %r12,0
    write	"r17     %i64(r17)"
    ldi		%r17, 123456
    fcvt.iw.sq	%r17, %r7,0
    write	"r17     %f128(r17)"

    write	"test fp minmax"
    fmax.sq	%r8, %r11, %r12
    write	"r8      %f128(r8)"
    fmin.sq	%r8, %r11, %r12
    write	"r8      %f128(r8)"
    write	"test fp abs minmax"
    famax.sq	%r8, %r11, %r12
    write	"r8      %f128(r8)"
    famin.sq	%r8, %r11, %r12
    write	"r8      %f128(r8)"

    write	"test fmergesq"
    fmerge.sq	%r8, %r11, %r12, %r14
    write	"r8      %f128(r8)"
    fmerge.sq	%r8, %r14, %r11, %r12
    write	"r8      %f128(r8)"


.rodata
    align	16
xxxd:	double	1.122
    double	0.9999765432
.text
    ca.r	%r21, xxxd
    ldi		%r15, 100
    ldz.d	%r25, %r21, 8*0
    ldz.d	%r26, %r21, 8*1
    fsub.sq	%r22, %r25, %r16
    write	"r22     %f128(r22)"
xxloop:
    fmadd.sq	%r22, %r25, %r16, %r22
    fmsub.sq	%r22, %r25, %r16, %r22
    reps.ge.d	%r15, %gz, xxloop
    write	"r22     %f128(r22)"

    write	"other FPU"
    fmadd.sq  %r60, %r61, %r62, %r63
    fmsub.sq  %r61, %r61, %r72, %r73
    fnmadd.sq %r62, %r71, %r82, %r63
    fnmsub.sq %r63, %r81, %r12, %r53

    fmul.sq	%r64, %r61, %r22
    fdiv.sq	%r65, %r11, %r27
    fadd.sq	%r66, %r17, %r42
    fsub.sq	%r67, %r31, %r23
    fnadd.sq	%r68, %r41, %r62
    fmax.sq	%r60, %r61, %r62
    fmin.sq	%r60, %r61, %r62
    famax.sq	%r60, %r61, %r62
    famin.sq	%r60, %r61, %r62

    fcmpo.lt.sq	%r10, %r61, %r72
    fcmpo.le.sq	%r11, %r52, %r21
    fcmpo.le.sq	%r12, %r43, %r12
    fcmpo.eq.sq	%r10, %r34, %r44
    fcmpu.eq.sq	%r13, %r25, %r22
    fcmpu.le.sq	%r12, %r15, %r23
    fcmpu.sq	%r11, %r86, %r86

    fneg.sq	%r24, %r58
    fabsd.sq	%r45, %r61, %r20
    fnabsd.sq	%r56, %r32, %r20
    frnd.sq	%r78, %r74,2
    frnd.sq	%r89, %r65,3
    frnd.sq	%r81, %r76,0
    frnd.sq	%r62, %r67,1
    fsqrt.sq	%r63, %r78
    frsqrt.sq %r64, %r69

    addi	%r45, %sp,-4800
    ldi		%r13, 2

    ldz.w	%r12, %r45, 4*1
    st.w		%r12, %r45, 4*1
    ldz.d	%r12, %r45, 8*3
    st.d		%r12, %r45, 8*3
    ldz.w.xd	%r12, %r45, %r13, 2, 60
    st.w.xd	%r12, %r45, %r13, 2, 60
    ldz.d.xd	%r12, %r45, %r13, 3, 60
    st.d.xd	%r12, %r45, %r13, 3, 60

    fadd.sq	%r23, %r24, %r25
    fmadd.sq	%r23, %r60, %r55, %r33
    fmul.sq	%r23, %r60, %r55
    ldz.d	%r60, %r45, 8*6
    fmadd.sq	%r23, %r60, %r55, %r33
    fmadd.sq	%r24, %r61, %r25, %r32
    fmadd.sq	%r25, %r62, %r55, %r23
    fmadd.sq	%r26, %r63, %r75, %r73
    fmadd.sq	%r27, %r64, %r75, %r73
    fmadd.sq	%r28, %r65, %r85, %r63
    fmadd.sq	%r29, %r66, %r85, %r63
    fmadd.sq	%r30, %r67, %r55, %r23
    fmadd.sq	%r31, %r68, %r55, %r23
    fmadd.sq	%r12, %r32, %r76, %r85
    fmadd.sq	%r12, %r32, %r76, %r85
    fmadd.sq	%r10, %r32, %r76, %r85
    fmadd.sq	%r10, %r32, %r76, %r85
    fmadd.sq	%r10, %r32, %r76, %r85
    fmadd.sq	%r13, %r32, %r76, %r85
    fmadd.sq	%r14, %r32, %r76, %r85
    fmadd.sq	%r15, %r32, %r76, %r85
    fmadd.sq	%r16, %r32, %r76, %r85
    fmadd.sq	%r17, %r32, %r76, %r85

    fcvt.sq.iw	%r56, %r45, 0
    fcvt.sq.uw	%r56, %r45, 0
    fcvt.iw.sq	%r45, %r56, 0
    fcvt.uw.sq	%r45, %r56, 0

    ldi		%r5, 0
    fldri.q	%r4, 1.0
    fldri.q	%r5, 1.0
    fldri.q	%r6, 1.0
    fldri.q	%r7, 1.0
    ldi		%r24, 128
tri_repeat:
    write	"r7      %x128(r7)"
    fadd.sq	%r5, %r5, %r4
    fmul.sq	%r6, %r6, %r5
    fdiv.sq	%r7, %r4, %r6
;   write "%x128(r6)"
    reps.le.d.l %r5, %r24, tri_repeat

    write	"test taylor series"
    fldri.q	%r2, 0.44567	; f2 ,  x
    write	"x:   %f128(r2)"		; test value
    write	"test sin(x)"
    fldri.q	%r5, sin(0.44567)
    write	"sin: %f128(r5)"		; test value
    ldi		%r3, 0		; s ,  0
    fmul.sq	%r4, %r2, %r2	; f4 ,  x*x
    fmadd.sq	%r3, %r3, %r4, %r25	; s ,  s * x*x + 1/25!
    fmsub.sq	%r3, %r3, %r4, %r23	; s ,  s * x*x - 1/23!
    fmadd.sq	%r3, %r3, %r4, %r21
    fmsub.sq	%r3, %r3, %r4, %r19
    fmadd.sq	%r3, %r3, %r4, %r17
    fmsub.sq	%r3, %r3, %r4, %r15
    fmadd.sq	%r3, %r3, %r4, %r13
    fmsub.sq	%r3, %r3, %r4, %r11
    fmadd.sq	%r3, %r3, %r4, %r9
    fmsub.sq	%r3, %r3, %r4, %r7
    fmadd.sq	%r3, %r3, %r4, %r5
    fmsub.sq	%r3, %r3, %r4, %r3
    fmadd.sq	%r3, %r3, %r4, %r1
    fmul.sq	%r3, %r3, %r2	; s ,  s * x
    write	"sin: %f128(r3)"

    write	"test cos(x)"
    fldri.q	%r5, cos(0.44567)
    write	"cos: %f128(r5)"		; test value
    ldi		%r3, 0		; s ,  0
    fmul.sq	%r4, %r2, %r2	; f4 ,  x*x
    fmsub.sq	%r3, %r3, %r4, %r26
    fmadd.sq	%r3, %r3, %r4, %r24
    fmsub.sq	%r3, %r3, %r4, %r22
    fmadd.sq	%r3, %r3, %r4, %r20
    fmsub.sq	%r3, %r3, %r4, %r18
    fmadd.sq	%r3, %r3, %r4, %r16
    fmsub.sq	%r3, %r3, %r4, %r14
    fmadd.sq	%r3, %r3, %r4, %r12
    fmsub.sq	%r3, %r3, %r4, %r10
    fmadd.sq	%r3, %r3, %r4, %r8
    fmsub.sq	%r3, %r3, %r4, %r6
    fmadd.sq	%r3, %r3, %r4, %r4
    fmsub.sq	%r3, %r3, %r4, %r2
    fmadd.sq	%r3, %r3, %r4, %r1
    write	"cos: %f128(r3)"

    write	"test exp(x)"
    fldri.q	%r5, exp(0.44567)
    write	"exp: %f128(r5)"	; test value
    ldi		%r3, 0			; s ,  0.0
    mov		%r4, %r2		; f4 ,  x
    fldi.d	%r6, 0.125
;   write	"%f128(r6)"
    fmul.sq	%r4, %r4, %r6	; x ,  x/8
    fmadd.sq	%r3, %r3, %r4, %r15
    fmadd.sq	%r3, %r3, %r4, %r14
    fmadd.sq	%r3, %r3, %r4, %r13
    fmadd.sq	%r3, %r3, %r4, %r12
    fmadd.sq	%r3, %r3, %r4, %r11
    fmadd.sq	%r3, %r3, %r4, %r10
    fmadd.sq	%r3, %r3, %r4, %r9
    fmadd.sq	%r3, %r3, %r4, %r8
    fmadd.sq	%r3, %r3, %r4, %r7
    fmadd.sq	%r3, %r3, %r4, %r6
    fmadd.sq	%r3, %r3, %r4, %r5
    fmadd.sq	%r3, %r3, %r4, %r4
    fmadd.sq	%r3, %r3, %r4, %r3
    fmadd.sq	%r3, %r3, %r4, %r2
    fmadd.sq	%r3, %r3, %r4, %r1
    fmadd.sq	%r3, %r3, %r4, %r1
    fmul.sq	%r3, %r3, %r3	; (e^x) ^ 8
    fmul.sq	%r3, %r3, %r3
    fmul.sq	%r3, %r3, %r3
    write	"exp: %f128(r3)"

    fadd.sq	%r1, %r2, %r3
    fmadd.sq	%r2, %r10, %r20, %r30
    fmadd.sq	%r1, %r11, %r21, %r31

    ; classification
    fcl.ss	%r4, %r5, 120
    fcl.sd	%r4, %r5, 120
    fcl.sq	%r4, %r5, 120
    jmp		skipfma

fpu_backward_target:
; single branches
    bfo.eq.ss	%r23, %r34, fpu_backward_target
    bfo.eq.ss.l	%r23, %r34, fpu_backward_target
    bfo.eq.ss	%r23, %r34, fpu_forward_target
    bfo.eq.ss.l	%r23, %r34, fpu_forward_target

    bfu.eq.ss	%r23, %r34, fpu_backward_target
    bfu.eq.ss.l	%r23, %r34, fpu_backward_target
    bfu.eq.ss	%r23, %r34, fpu_forward_target
    bfu.eq.ss.l	%r23, %r34, fpu_forward_target

    bfo.ne.ss	%r23, %r34, fpu_backward_target
    bfo.ne.ss.l	%r23, %r34, fpu_backward_target
    bfo.ne.ss	%r23, %r34, fpu_forward_target
    bfo.ne.ss.l	%r23, %r34, fpu_forward_target

    bfu.ne.ss	%r23, %r34, fpu_backward_target
    bfu.ne.ss.l	%r23, %r34, fpu_backward_target
    bfu.ne.ss	%r23, %r34, fpu_forward_target
    bfu.ne.ss.l	%r23, %r34, fpu_forward_target

    bfo.lt.ss	%r23, %r34, fpu_backward_target
    bfo.lt.ss.l	%r23, %r34, fpu_backward_target
    bfo.lt.ss	%r23, %r34, fpu_forward_target
    bfo.lt.ss.l	%r23, %r34, fpu_forward_target

    bfu.lt.ss	%r23, %r34, fpu_backward_target
    bfu.lt.ss.l	%r23, %r34, fpu_backward_target
    bfu.lt.ss	%r23, %r34, fpu_forward_target
    bfu.lt.ss.l	%r23, %r34, fpu_forward_target

    bfo.le.ss	%r23, %r34, fpu_backward_target
    bfo.le.ss.l	%r23, %r34, fpu_backward_target
    bfo.le.ss	%r23, %r34, fpu_forward_target
    bfo.le.ss.l	%r23, %r34, fpu_forward_target

    bfu.le.ss	%r23, %r34, fpu_backward_target
    bfu.le.ss.l	%r23, %r34, fpu_backward_target
    bfu.le.ss	%r23, %r34, fpu_forward_target
    bfu.le.ss.l	%r23, %r34, fpu_forward_target

    bfo.ss	%r23, %r34, fpu_backward_target
    bfo.ss.l	%r23, %r34, fpu_backward_target
    bfo.ss	%r23, %r34, fpu_forward_target
    bfo.ss.l	%r23, %r34, fpu_forward_target

    bfu.ss	%r23, %r34, fpu_backward_target
    bfu.ss.l	%r23, %r34, fpu_backward_target
    bfu.ss	%r23, %r34, fpu_forward_target
    bfu.ss.l	%r23, %r34, fpu_forward_target

    bf.class.ss	%r23, 34, fpu_backward_target
    bf.class.ss.l	%r23, 34, fpu_backward_target
    bf.class.ss	%r23, 34, fpu_forward_target
    bf.class.ss.l	%r23, 34, fpu_forward_target

; double branches
    bfo.eq.sd	%r23, %r34, fpu_backward_target
    bfo.eq.sd.l	%r23, %r34, fpu_backward_target
    bfo.eq.sd	%r23, %r34, fpu_forward_target
    bfo.eq.sd.l	%r23, %r34, fpu_forward_target

    bfu.eq.sd	%r23, %r34, fpu_backward_target
    bfu.eq.sd.l	%r23, %r34, fpu_backward_target
    bfu.eq.sd	%r23, %r34, fpu_forward_target
    bfu.eq.sd.l	%r23, %r34, fpu_forward_target

    bfo.ne.sd	%r23, %r34, fpu_backward_target
    bfo.ne.sd.l	%r23, %r34, fpu_backward_target
    bfo.ne.sd	%r23, %r34, fpu_forward_target
    bfo.ne.sd.l	%r23, %r34, fpu_forward_target

    bfu.ne.sd	%r23, %r34, fpu_backward_target
    bfu.ne.sd.l	%r23, %r34, fpu_backward_target
    bfu.ne.sd	%r23, %r34, fpu_forward_target
    bfu.ne.sd.l	%r23, %r34, fpu_forward_target

    bfo.lt.sd	%r23, %r34, fpu_backward_target
    bfo.lt.sd.l	%r23, %r34, fpu_backward_target
    bfo.lt.sd	%r23, %r34, fpu_forward_target
    bfo.lt.sd.l	%r23, %r34, fpu_forward_target

    bfu.lt.sd	%r23, %r34, fpu_backward_target
    bfu.lt.sd.l	%r23, %r34, fpu_backward_target
    bfu.lt.sd	%r23, %r34, fpu_forward_target
    bfu.lt.sd.l	%r23, %r34, fpu_forward_target

    bfo.le.sd	%r23, %r34, fpu_backward_target
    bfo.le.sd.l	%r23, %r34, fpu_backward_target
    bfo.le.sd	%r23, %r34, fpu_forward_target
    bfo.le.sd.l	%r23, %r34, fpu_forward_target

    bfu.le.sd	%r23, %r34, fpu_backward_target
    bfu.le.sd.l	%r23, %r34, fpu_backward_target
    bfu.le.sd	%r23, %r34, fpu_forward_target
    bfu.le.sd.l	%r23, %r34, fpu_forward_target

    bfo.sd	%r23, %r34, fpu_backward_target
    bfo.sd.l	%r23, %r34, fpu_backward_target
    bfo.sd	%r23, %r34, fpu_forward_target
    bfo.sd.l	%r23, %r34, fpu_forward_target

    bfu.sd	%r23, %r34, fpu_backward_target
    bfu.sd.l	%r23, %r34, fpu_backward_target
    bfu.sd	%r23, %r34, fpu_forward_target
    bfu.sd.l	%r23, %r34, fpu_forward_target

    bf.class.sd	%r23, 34, fpu_backward_target
    bf.class.sd.l	%r23, 34, fpu_backward_target
    bf.class.sd	%r23, 34, fpu_forward_target
    bf.class.sd.l	%r23, 34, fpu_forward_target

; quadruple branches
    bfo.eq.sq	%r23, %r34, fpu_backward_target
    bfo.eq.sq.l	%r23, %r34, fpu_backward_target
    bfo.eq.sq	%r23, %r34, fpu_forward_target
    bfo.eq.sq.l	%r23, %r34, fpu_forward_target

    bfu.eq.sq	%r23, %r34, fpu_backward_target
    bfu.eq.sq.l	%r23, %r34, fpu_backward_target
    bfu.eq.sq	%r23, %r34, fpu_forward_target
    bfu.eq.sq.l	%r23, %r34, fpu_forward_target

    bfo.ne.sq	%r23, %r34, fpu_backward_target
    bfo.ne.sq.l	%r23, %r34, fpu_backward_target
    bfo.ne.sq	%r23, %r34, fpu_forward_target
    bfo.ne.sq.l	%r23, %r34, fpu_forward_target

    bfu.ne.sq	%r23, %r34, fpu_backward_target
    bfu.ne.sq.l	%r23, %r34, fpu_backward_target
    bfu.ne.sq	%r23, %r34, fpu_forward_target
    bfu.ne.sq.l	%r23, %r34, fpu_forward_target

    bfo.lt.sq	%r23, %r34, fpu_backward_target
    bfo.lt.sq.l	%r23, %r34, fpu_backward_target
    bfo.lt.sq	%r23, %r34, fpu_forward_target
    bfo.lt.sq.l	%r23, %r34, fpu_forward_target

    bfu.lt.sq	%r23, %r34, fpu_backward_target
    bfu.lt.sq.l	%r23, %r34, fpu_backward_target
    bfu.lt.sq	%r23, %r34, fpu_forward_target
    bfu.lt.sq.l	%r23, %r34, fpu_forward_target

    bfo.le.sq	%r23, %r34, fpu_backward_target
    bfo.le.sq.l	%r23, %r34, fpu_backward_target
    bfo.le.sq	%r23, %r34, fpu_forward_target
    bfo.le.sq.l	%r23, %r34, fpu_forward_target

    bfu.le.sq	%r23, %r34, fpu_backward_target
    bfu.le.sq.l	%r23, %r34, fpu_backward_target
    bfu.le.sq	%r23, %r34, fpu_forward_target
    bfu.le.sq.l	%r23, %r34, fpu_forward_target

    bfo.sq	%r23, %r34, fpu_backward_target
    bfo.sq.l	%r23, %r34, fpu_backward_target
    bfo.sq	%r23, %r34, fpu_forward_target
    bfo.sq.l	%r23, %r34, fpu_forward_target

    bfu.sq	%r23, %r34, fpu_backward_target
    bfu.sq.l	%r23, %r34, fpu_backward_target
    bfu.sq	%r23, %r34, fpu_forward_target
    bfu.sq.l	%r23, %r34, fpu_forward_target

    bf.class.sq	%r23, 34, fpu_backward_target
    bf.class.sq.l	%r23, 34, fpu_backward_target
    bf.class.sq	%r23, 34, fpu_forward_target
    bf.class.sq.l	%r23, 34, fpu_forward_target

fpu_forward_target:

    nulfu.ne.ss	%r23, %r34, 1, 1
    nulfu.ne.sd	%r23, %r34, 1, 1
    nulfu.ne.sq	%r23, %r34, 1, 1

    nulfo.ne.ss	%r23, %r34, 1, 1
    nulfo.ne.sd	%r23, %r34, 1, 1
    nulfo.ne.sq	%r23, %r34, 1, 1

    nulfu.eq.ss	%r23, %r34, 1, 1
    nulfu.eq.sd	%r23, %r34, 1, 1
    nulfu.eq.sq	%r23, %r34, 1, 1

    nulfo.eq.ss	%r23, %r34, 1, 1
    nulfo.eq.sd	%r23, %r34, 1, 1
    nulfo.eq.sq	%r23, %r34, 1, 1

    nulf.class.ss	%r23, 94, 1, 1
    nulf.class.sd	%r23, 94, 1, 1
    nulf.class.sq	%r23, 94, 1, 1
skipfma:
    write	"end fpu"
.end
