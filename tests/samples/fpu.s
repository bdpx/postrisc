.text
; floating-point extension example
    alloc	96

    write	"test float128 immediate load (low/high parts)"
    fldqri	%r12, 3.1415926115461431423612436243
    write	"fldqri: %f128(r12)"

    write	"test fpcr modification (127,100)"
    ldi		%r1, 127
    ldi		%r2, 100
    sprsr	%r1, %fpcr, %r1, %r2
    write	"fpcr: %s(fpcr)"
    write	"test fpcr modification (0,60)"
    ldi		%r1, 0
    ldi		%r2, 60
    sprsr	%r1, %fpcr, %r1, %r2
    write	"fpcr: %s(fpcr)"
    write	"test fpcr modification (127,80)"
    ldi		%r1, 127
    ldi		%r2, 80
    sprsr	%r1, %fpcr, %r1, %r2
    write	"fpcr: %s(fpcr)"
    write	"test fpcr modification (0,90)"
    ldi		%r1, 0
    ldi		%r2, 90
    sprsr	%r1, %fpcr, %r1, %r2
    write	"fpcr: %s(fpcr)"

    write	"compare fldqri (full mantissa) & long fldi (63-bit mantissa)"
    fldqri	%r30, 3.14159265358979323846123456789012e+400
    write	"fldqri: %x128(r30) %f128(r30)"
    flddi	%r31, 3.14159265358979323846123456789012
    write	"flddi: %x128(r31) %f64(r31)"
    write	"compare fldqri (full mantissa) & short fldi (21-bit mantissa)"
    fldqri	%r30, 3.14159265358979323846123456789012
    write	"r30     %x128(r30)"
    flddi	%r31, 3.14159265358979323846123456789012
    write	"r31     %x128(r31)"
    write	"before1"
    write	"r30     %f128(r30)"
    write	"before2"
    write	"r31     %vf64(r31)"
    write	"after"
    flddi	%r30, -12.3456789e+04
.rodata
    align	16
float64data:
    double	1.234567890123456789124141241241
    double	3.1415925678888734535345231234564561
    double	3.4566345634563456346535463463456
.text
    ldar	%r21, float64data
    lddz	%r11, %r21, 8*0
    lddz	%r12, %r21, 8*1
    lddz	%r13, %r21, 8*2
    write	"ld8(f64): %f64(r11) %f64(r12) %f64(r13)"
    fldqri	%r14, 2.7182818289201
    write	"fldqri: %f128(r14)"

    fextsd2sq	%r11, %r11
    fextsd2sq	%r12, %r12
    fextsd2sq	%r13, %r13

    write	"test binary"
    fmulsq	%r15, %r11, %r14
    write	"fmulsq:  %f128(r15)"
    fnmulsq	%r15, %r11, %r14
    write	"fnmulsq: %f128(r15)"
    faddsq	%r15, %r11, %r14
    write	"faddsq:  %f128(r15)"
    fnaddsq	%r15, %r11, %r14
    write	"fnaddsq: %f128(r15)"
    fsubsq	%r15, %r14, %r11
    write	"fsubsq:  %f128(r15)"
    fdivsq	%r15, %r14, %r11
    write	"fdivsq:  %f128(r15)"

    write	"test fused fma"
;   jmp	skipfma
    fmaddsq	%r15, %r14, %r11, %r12
    write	"fmaddsq:  %f128(r15)"
    fnmaddsq %r15, %r14, %r11, %r12
    write	"fnmaddsq: %f128(r15)"
    fmsubsq	%r15, %r14, %r11, %r12
    write	"fmsubsq:  %f128(r15)"
    fnmsubsq %r15, %r14, %r11, %r12
    write	"fnmsubsq: %f128(r15)"

    write	"test unary"
    mov		%r16, %r15
    write	"r16     %f128(r16)"
    fabssq	%r16, %r15
    write	"r16     %f128(r16)"
    fnegsq	%r16, %r15
    write	"r16     %f128(r16)"
    fnabssq	%r16, %r15
    write	"r16     %f128(r16)"
    fsqrtsq	%r16, %r12
    write	"r16     %f128(r16)"
    frsqrtsq	%r16, %r12
    write	"r16     %f128(r16)"

    write	"test rounding"
    frndsq	%r17, %r12, 4
    write	"r17     %f128(r17)"
    frndsq	%r17, %r12, 3
    write	"r17     %f128(r17)"
    frndsq	%r17, %r12, 2
    write	"r17     %f128(r17)"
    frndsq	%r17, %r12, 0
    write	"r17     %f128(r17)"
    fcvtsq2iw	%r17, %r12,0
    write	"r17     %i64(r17)"
    ldi		%r17, 123456
    fcvtiw2sq	%r17, %r7,0
    write	"r17     %f128(r17)"

    write	"test fp minmax"
    fmaxsq	%r8, %r11, %r12
    write	"r8      %f128(r8)"
    fminsq	%r8, %r11, %r12
    write	"r8      %f128(r8)"
    write	"test fp abs minmax"
    famaxsq	%r8, %r11, %r12
    write	"r8      %f128(r8)"
    faminsq	%r8, %r11, %r12
    write	"r8      %f128(r8)"

    write	"test fmergesq"
    fmergesq	%r8, %r11, %r12, %r14
    write	"r8      %f128(r8)"
    fmergesq	%r8, %r14, %r11, %r12
    write	"r8      %f128(r8)"


.rodata
    align	16
xxxd:	double	1.122
    double	0.9999765432
.text
    ldar	%r21, xxxd
    ldi		%r15, 100
    lddz	%r25, %r21, 8*0
    lddz	%r26, %r21, 8*1
    fsubsq	%r22, %r25, %r16
    write	"r22     %f128(r22)"
xxloop:
    fmaddsq	%r22, %r25, %r16, %r22
    fmsubsq	%r22, %r25, %r16, %r22
    repdge	%r15, %gz, xxloop
    write	"r22     %f128(r22)"

    write	"other FPU"
    fmaddsq  %r60, %r61, %r62, %r63
    fmsubsq  %r61, %r61, %r72, %r73
    fnmaddsq %r62, %r71, %r82, %r63
    fnmsubsq %r63, %r81, %r12, %r53

    fmulsq	%r64, %r61, %r22
    fdivsq	%r65, %r11, %r27
    faddsq	%r66, %r17, %r42
    fsubsq	%r67, %r31, %r23
    fnaddsq	%r68, %r41, %r62
    fmaxsq	%r60, %r61, %r62
    fminsq	%r60, %r61, %r62
    famaxsq	%r60, %r61, %r62
    faminsq	%r60, %r61, %r62

    fcmpsqolt	%r10, %r61, %r72
    fcmpsqole	%r11, %r52, %r21
    fcmpsqole	%r12, %r43, %r12
    fcmpsqoeq	%r10, %r34, %r44
    fcmpsqueq	%r13, %r25, %r22
    fcmpsqule	%r12, %r15, %r23
    fcmpsquo	%r11, %r86, %r86

    fnegsq	%r24, %r58
    fabsdsq	%r45, %r61, %r20
    fnabsdsq	%r56, %r32, %r20
    frndsq	%r78, %r74,2
    frndsq	%r89, %r65,3
    frndsq	%r81, %r76,0
    frndsq	%r62, %r67,1
    fsqrtsq	%r63, %r78
    frsqrtsq %r64, %r69

    addi	%r45, %sp,-4800
    ldi		%r13, 2

    ldwz	%r12, %r45, 4*1
    stw		%r12, %r45, 4*1
    lddz	%r12, %r45, 8*3
    std		%r12, %r45, 8*3
    ldwzx	%r12, %r45, %r13, 2, 200
    stwx	%r12, %r45, %r13, 2, 200
    lddzx	%r12, %r45, %r13, 3, 200
    stdx	%r12, %r45, %r13, 3, 200

    faddsq	%r23, %r24, %r25
    fmaddsq	%r23, %r60, %r55, %r33
    fmulsq	%r23, %r60, %r55
    lddz	%r60, %r45, 8*6
    fmaddsq	%r23, %r60, %r55, %r33
    fmaddsq	%r24, %r61, %r25, %r32
    fmaddsq	%r25, %r62, %r55, %r23
    fmaddsq	%r26, %r63, %r75, %r73
    fmaddsq	%r27, %r64, %r75, %r73
    fmaddsq	%r28, %r65, %r85, %r63
    fmaddsq	%r29, %r66, %r85, %r63
    fmaddsq	%r30, %r67, %r55, %r23
    fmaddsq	%r31, %r68, %r55, %r23
    fmaddsq	%r12, %r32, %r76, %r85
    fmaddsq	%r12, %r32, %r76, %r85
    fmaddsq	%r10, %r32, %r76, %r85
    fmaddsq	%r10, %r32, %r76, %r85
    fmaddsq	%r10, %r32, %r76, %r85
    fmaddsq	%r13, %r32, %r76, %r85
    fmaddsq	%r14, %r32, %r76, %r85
    fmaddsq	%r15, %r32, %r76, %r85
    fmaddsq	%r16, %r32, %r76, %r85
    fmaddsq	%r17, %r32, %r76, %r85

    fcvtsq2iw	%r56, %r45, 0
    fcvtsq2uw	%r56, %r45, 0
    fcvtiw2sq	%r45, %r56, 0
    fcvtuw2sq	%r45, %r56, 0

    ldi		%r5, 0
    fldqri	%r4, 1.0
    fldqri	%r5, 1.0
    fldqri	%r6, 1.0
    fldqri	%r7, 1.0
    ldi		%r24, 128
tri_repeat:
    write	"r7      %x128(r7)"
    faddsq	%r5, %r5, %r4
    fmulsq	%r6, %r6, %r5
    fdivsq	%r7, %r4, %r6
;   write "%x128(r6)"
    repdle.l %r5, %r24, tri_repeat

    write	"test taylor series"
    fldqri	%r2, 0.44567	; f2 ,  x
    write	"x:   %f128(r2)"		; test value
    write	"test sin(x)"
    fldqri	%r5, sin(0.44567)
    write	"sin: %f128(r5)"		; test value
    ldi		%r3, 0		; s ,  0
    fmulsq	%r4, %r2, %r2	; f4 ,  x*x
    fmaddsq	%r3, %r3, %r4, %r25	; s ,  s * x*x + 1/25!
    fmsubsq	%r3, %r3, %r4, %r23	; s ,  s * x*x - 1/23!
    fmaddsq	%r3, %r3, %r4, %r21
    fmsubsq	%r3, %r3, %r4, %r19
    fmaddsq	%r3, %r3, %r4, %r17
    fmsubsq	%r3, %r3, %r4, %r15
    fmaddsq	%r3, %r3, %r4, %r13
    fmsubsq	%r3, %r3, %r4, %r11
    fmaddsq	%r3, %r3, %r4, %r9
    fmsubsq	%r3, %r3, %r4, %r7
    fmaddsq	%r3, %r3, %r4, %r5
    fmsubsq	%r3, %r3, %r4, %r3
    fmaddsq	%r3, %r3, %r4, %r1
    fmulsq	%r3, %r3, %r2	; s ,  s * x
    write	"sin: %f128(r3)"

    write	"test cos(x)"
    fldqri	%r5, cos(0.44567)
    write	"cos: %f128(r5)"		; test value
    ldi		%r3, 0		; s ,  0
    fmulsq	%r4, %r2, %r2	; f4 ,  x*x
    fmsubsq	%r3, %r3, %r4, %r26
    fmaddsq	%r3, %r3, %r4, %r24
    fmsubsq	%r3, %r3, %r4, %r22
    fmaddsq	%r3, %r3, %r4, %r20
    fmsubsq	%r3, %r3, %r4, %r18
    fmaddsq	%r3, %r3, %r4, %r16
    fmsubsq	%r3, %r3, %r4, %r14
    fmaddsq	%r3, %r3, %r4, %r12
    fmsubsq	%r3, %r3, %r4, %r10
    fmaddsq	%r3, %r3, %r4, %r8
    fmsubsq	%r3, %r3, %r4, %r6
    fmaddsq	%r3, %r3, %r4, %r4
    fmsubsq	%r3, %r3, %r4, %r2
    fmaddsq	%r3, %r3, %r4, %r1
    write	"cos: %f128(r3)"

    write	"test exp(x)"
    fldqri	%r5, exp(0.44567)
    write	"exp: %f128(r5)"	; test value
    ldi		%r3, 0			; s ,  0.0
    mov		%r4, %r2		; f4 ,  x
    flddi	%r6, 0.125
;   write	"%f128(r6)"
    fmulsq	%r4, %r4, %r6	; x ,  x/8
    fmaddsq	%r3, %r3, %r4, %r15
    fmaddsq	%r3, %r3, %r4, %r14
    fmaddsq	%r3, %r3, %r4, %r13
    fmaddsq	%r3, %r3, %r4, %r12
    fmaddsq	%r3, %r3, %r4, %r11
    fmaddsq	%r3, %r3, %r4, %r10
    fmaddsq	%r3, %r3, %r4, %r9
    fmaddsq	%r3, %r3, %r4, %r8
    fmaddsq	%r3, %r3, %r4, %r7
    fmaddsq	%r3, %r3, %r4, %r6
    fmaddsq	%r3, %r3, %r4, %r5
    fmaddsq	%r3, %r3, %r4, %r4
    fmaddsq	%r3, %r3, %r4, %r3
    fmaddsq	%r3, %r3, %r4, %r2
    fmaddsq	%r3, %r3, %r4, %r1
    fmaddsq	%r3, %r3, %r4, %r1
    fmulsq	%r3, %r3, %r3	; (e^x) ^ 8
    fmulsq	%r3, %r3, %r3
    fmulsq	%r3, %r3, %r3
    write	"exp: %f128(r3)"

    faddsq	%r1, %r2, %r3
    fmaddsq	%r2, %r10, %r20, %r30
    fmaddsq	%r1, %r11, %r21, %r31

    ; classification
    fclss	%r4, %r5, 120
    fclsd	%r4, %r5, 120
    fclsq	%r4, %r5, 120
    jmp		skipfma

fpu_backward_target:
; single branches
    bfssoeq	%r23, %r34, fpu_backward_target
    bfssoeq.l	%r23, %r34, fpu_backward_target
    bfssoeq	%r23, %r34, fpu_forward_target
    bfssoeq.l	%r23, %r34, fpu_forward_target

    bfssueq	%r23, %r34, fpu_backward_target
    bfssueq.l	%r23, %r34, fpu_backward_target
    bfssueq	%r23, %r34, fpu_forward_target
    bfssueq.l	%r23, %r34, fpu_forward_target

    bfssone	%r23, %r34, fpu_backward_target
    bfssone.l	%r23, %r34, fpu_backward_target
    bfssone	%r23, %r34, fpu_forward_target
    bfssone.l	%r23, %r34, fpu_forward_target

    bfssune	%r23, %r34, fpu_backward_target
    bfssune.l	%r23, %r34, fpu_backward_target
    bfssune	%r23, %r34, fpu_forward_target
    bfssune.l	%r23, %r34, fpu_forward_target

    bfssolt	%r23, %r34, fpu_backward_target
    bfssolt.l	%r23, %r34, fpu_backward_target
    bfssolt	%r23, %r34, fpu_forward_target
    bfssolt.l	%r23, %r34, fpu_forward_target

    bfssult	%r23, %r34, fpu_backward_target
    bfssult.l	%r23, %r34, fpu_backward_target
    bfssult	%r23, %r34, fpu_forward_target
    bfssult.l	%r23, %r34, fpu_forward_target

    bfssole	%r23, %r34, fpu_backward_target
    bfssole.l	%r23, %r34, fpu_backward_target
    bfssole	%r23, %r34, fpu_forward_target
    bfssole.l	%r23, %r34, fpu_forward_target

    bfssule	%r23, %r34, fpu_backward_target
    bfssule.l	%r23, %r34, fpu_backward_target
    bfssule	%r23, %r34, fpu_forward_target
    bfssule.l	%r23, %r34, fpu_forward_target

    bfsso	%r23, %r34, fpu_backward_target
    bfsso.l	%r23, %r34, fpu_backward_target
    bfsso	%r23, %r34, fpu_forward_target
    bfsso.l	%r23, %r34, fpu_forward_target

    bfssuo	%r23, %r34, fpu_backward_target
    bfssuo.l	%r23, %r34, fpu_backward_target
    bfssuo	%r23, %r34, fpu_forward_target
    bfssuo.l	%r23, %r34, fpu_forward_target

    bfssclass	%r23, 34, fpu_backward_target
    bfssclass.l	%r23, 34, fpu_backward_target
    bfssclass	%r23, 34, fpu_forward_target
    bfssclass.l	%r23, 34, fpu_forward_target

; double branches
    bfsdoeq	%r23, %r34, fpu_backward_target
    bfsdoeq.l	%r23, %r34, fpu_backward_target
    bfsdoeq	%r23, %r34, fpu_forward_target
    bfsdoeq.l	%r23, %r34, fpu_forward_target

    bfsdueq	%r23, %r34, fpu_backward_target
    bfsdueq.l	%r23, %r34, fpu_backward_target
    bfsdueq	%r23, %r34, fpu_forward_target
    bfsdueq.l	%r23, %r34, fpu_forward_target

    bfsdone	%r23, %r34, fpu_backward_target
    bfsdone.l	%r23, %r34, fpu_backward_target
    bfsdone	%r23, %r34, fpu_forward_target
    bfsdone.l	%r23, %r34, fpu_forward_target

    bfsdune	%r23, %r34, fpu_backward_target
    bfsdune.l	%r23, %r34, fpu_backward_target
    bfsdune	%r23, %r34, fpu_forward_target
    bfsdune.l	%r23, %r34, fpu_forward_target

    bfsdolt	%r23, %r34, fpu_backward_target
    bfsdolt.l	%r23, %r34, fpu_backward_target
    bfsdolt	%r23, %r34, fpu_forward_target
    bfsdolt.l	%r23, %r34, fpu_forward_target

    bfsdult	%r23, %r34, fpu_backward_target
    bfsdult.l	%r23, %r34, fpu_backward_target
    bfsdult	%r23, %r34, fpu_forward_target
    bfsdult.l	%r23, %r34, fpu_forward_target

    bfsdole	%r23, %r34, fpu_backward_target
    bfsdole.l	%r23, %r34, fpu_backward_target
    bfsdole	%r23, %r34, fpu_forward_target
    bfsdole.l	%r23, %r34, fpu_forward_target

    bfsdule	%r23, %r34, fpu_backward_target
    bfsdule.l	%r23, %r34, fpu_backward_target
    bfsdule	%r23, %r34, fpu_forward_target
    bfsdule.l	%r23, %r34, fpu_forward_target

    bfsdo	%r23, %r34, fpu_backward_target
    bfsdo.l	%r23, %r34, fpu_backward_target
    bfsdo	%r23, %r34, fpu_forward_target
    bfsdo.l	%r23, %r34, fpu_forward_target

    bfsduo	%r23, %r34, fpu_backward_target
    bfsduo.l	%r23, %r34, fpu_backward_target
    bfsduo	%r23, %r34, fpu_forward_target
    bfsduo.l	%r23, %r34, fpu_forward_target

    bfsdclass	%r23, 34, fpu_backward_target
    bfsdclass.l	%r23, 34, fpu_backward_target
    bfsdclass	%r23, 34, fpu_forward_target
    bfsdclass.l	%r23, 34, fpu_forward_target

; quadruple branches
    bfsqoeq	%r23, %r34, fpu_backward_target
    bfsqoeq.l	%r23, %r34, fpu_backward_target
    bfsqoeq	%r23, %r34, fpu_forward_target
    bfsqoeq.l	%r23, %r34, fpu_forward_target

    bfsqueq	%r23, %r34, fpu_backward_target
    bfsqueq.l	%r23, %r34, fpu_backward_target
    bfsqueq	%r23, %r34, fpu_forward_target
    bfsqueq.l	%r23, %r34, fpu_forward_target

    bfsqone	%r23, %r34, fpu_backward_target
    bfsqone.l	%r23, %r34, fpu_backward_target
    bfsqone	%r23, %r34, fpu_forward_target
    bfsqone.l	%r23, %r34, fpu_forward_target

    bfsqune	%r23, %r34, fpu_backward_target
    bfsqune.l	%r23, %r34, fpu_backward_target
    bfsqune	%r23, %r34, fpu_forward_target
    bfsqune.l	%r23, %r34, fpu_forward_target

    bfsqolt	%r23, %r34, fpu_backward_target
    bfsqolt.l	%r23, %r34, fpu_backward_target
    bfsqolt	%r23, %r34, fpu_forward_target
    bfsqolt.l	%r23, %r34, fpu_forward_target

    bfsqult	%r23, %r34, fpu_backward_target
    bfsqult.l	%r23, %r34, fpu_backward_target
    bfsqult	%r23, %r34, fpu_forward_target
    bfsqult.l	%r23, %r34, fpu_forward_target

    bfsqole	%r23, %r34, fpu_backward_target
    bfsqole.l	%r23, %r34, fpu_backward_target
    bfsqole	%r23, %r34, fpu_forward_target
    bfsqole.l	%r23, %r34, fpu_forward_target

    bfsqule	%r23, %r34, fpu_backward_target
    bfsqule.l	%r23, %r34, fpu_backward_target
    bfsqule	%r23, %r34, fpu_forward_target
    bfsqule.l	%r23, %r34, fpu_forward_target

    bfsqo	%r23, %r34, fpu_backward_target
    bfsqo.l	%r23, %r34, fpu_backward_target
    bfsqo	%r23, %r34, fpu_forward_target
    bfsqo.l	%r23, %r34, fpu_forward_target

    bfsquo	%r23, %r34, fpu_backward_target
    bfsquo.l	%r23, %r34, fpu_backward_target
    bfsquo	%r23, %r34, fpu_forward_target
    bfsquo.l	%r23, %r34, fpu_forward_target

    bfsqclass	%r23, 34, fpu_backward_target
    bfsqclass.l	%r23, 34, fpu_backward_target
    bfsqclass	%r23, 34, fpu_forward_target
    bfsqclass.l	%r23, 34, fpu_forward_target

fpu_forward_target:

    nulfssune	%r23, %r34, 1, 1
    nulfsdune	%r23, %r34, 1, 1
    nulfsqune	%r23, %r34, 1, 1

    nulfssone	%r23, %r34, 1, 1
    nulfsdone	%r23, %r34, 1, 1
    nulfsqone	%r23, %r34, 1, 1

    nulfssueq	%r23, %r34, 1, 1
    nulfsdueq	%r23, %r34, 1, 1
    nulfsqueq	%r23, %r34, 1, 1

    nulfssoeq	%r23, %r34, 1, 1
    nulfsdoeq	%r23, %r34, 1, 1
    nulfsqoeq	%r23, %r34, 1, 1

    nulfssclass	%r23, 94, 1, 1
    nulfsdclass	%r23, 94, 1, 1
    nulfsqclass	%r23, 94, 1, 1
skipfma:
    write	"end fpu"
.end
