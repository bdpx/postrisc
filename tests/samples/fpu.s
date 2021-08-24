.text
; floating-point extension example
    alloc 96

    write "test f128 ld rel"
    fldqr %r12, 3.1415926115461431423612436243
    write "f128 ld rel: %f128(r12)"

    write "test fpcr modification (rm=3)"
    ldi  %r2, 3
    setspr %r2, %fpcr
    write "fpcr: %s(fpcr)"
    write "test fpcr modification (rm=2)"
    ldi  %r2, 2
    setspr %r2, %fpcr
    write "fpcr: %s(fpcr)"
    write "test fpcr modification (rm=1)"
    ldi  %r2, 1
    setspr %r2, %fpcr
    write "fpcr: %s(fpcr)"
    write "test fpcr modification (rm=0)"
    ldi  %r2, 0
    setspr %r2, %fpcr
    write "fpcr: %s(fpcr)"

    write "compare f128 ld rel (full mantissa) & long f128 ld imm (63-bit mantissa)"
    fldqr %r30, 3.14159265358979323846123456789012e+400
    write "f128 ld rel: %x128(r30) %f128(r30)"
    fldid %r31, 3.14159265358979323846123456789012
    write "f128 ld imm: %x128(r31) %f64(r31)"
    write "compare f128 ld rel (full mantissa) & short f128 ld imm (21-bit mantissa)"
    fldqr %r30, 3.14159265358979323846123456789012
    write "r30     %x128(r30)"
    fldid %r31, 3.14159265358979323846123456789012
    write "r31     %x128(r31)"
    write "before1"
    write "r30     %f128(r30)"
    write "before2"
    write "r31     %vf64(r31)"
    write "after"
    fldid %r30, -12.3456789e+04
.rodata
    align 16
float64data:
    double 1.234567890123456789124141241241
    double 3.1415925678888734535345231234564561
    double 3.4566345634563456346535463463456
.text
    ldarc %r21, float64data
    ldud %r11, %r21, 8*0
    ldud %r12, %r21, 8*1
    ldud %r13, %r21, 8*2
    write "ld8(f64): %f64(r11) %f64(r12) %f64(r13)"
    fldqr %r14, 2.7182818289201
    write "f128 ld rel: %f128(r14)"

    fdtoq %r11, %r11
    fdtoq %r12, %r12
    fdtoq %r13, %r13

    write "test binary"
    fmulq %r15, %r11, %r14
    write "f128 mul:  %f128(r15)"
    fnmulq %r15, %r11, %r14
    write "f128 nmul: %f128(r15)"
    faddq %r15, %r11, %r14
    write "f128 add:  %f128(r15)"
    fnaddq %r15, %r11, %r14
    write "f128 nadd: %f128(r15)"
    fsubq %r15, %r14, %r11
    write "f128 subs:  %f128(r15)"
    fdivq %r15, %r14, %r11
    write "f128 div:  %f128(r15)"

    write "test fused fma"
;   jmp skipfma
    fmaddq %r15, %r14, %r11, %r12
    write "f128 madd:  %f128(r15)"
    fnmaddq %r15, %r14, %r11, %r12
    write "f128 nmadd: %f128(r15)"
    fmsubq %r15, %r14, %r11, %r12
    write "f128 msub:  %f128(r15)"
    fnmsubq %r15, %r14, %r11, %r12
    write "f128 nmsub: %f128(r15)"

    write "test unary"
    mov  %r16, %r15
    write "r16     %f128(r16)"
    fabsq %r16, %r15
    write "r16     %f128(r16)"
    fnegq %r16, %r15
    write "r16     %f128(r16)"
    fnabsq %r16, %r15
    write "r16     %f128(r16)"
    fsqrtq %r16, %r12
    write "r16     %f128(r16)"
    frsqrtq %r16, %r12
    write "r16     %f128(r16)"

    write "test rounding"
    frndq %r17, %r12, rtz
    write "r17     %f128(r17)"
    frndq %r17, %r12, rdn
    write "r17     %f128(r17)"
    frndq %r17, %r12, rup
    write "r17     %f128(r17)"
    frndq %r17, %r12, rne
    write "r17     %f128(r17)"
    fqtoiw %r17, %r12, rtz
    write "r17     %i64(r17)"
    ldi %r17, 123456
    fiwtoq %r17, %r7
    write "r17     %f128(r17)"

    write "test fp minmax"
    fmaxq %r8, %r11, %r12
    write "r8      %f128(r8)"
    fminq %r8, %r11, %r12
    write "r8      %f128(r8)"
    write "test fp abs minmax"
    famaxnmq %r8, %r11, %r12
    write "r8      %f128(r8)"
    faminnmq %r8, %r11, %r12
    write "r8      %f128(r8)"

    write "test f128 merge"
    fmergeq %r8, %r11, %r12, %r14
    write "r8      %f128(r8)"
    fmergeq %r8, %r14, %r11, %r12
    write "r8      %f128(r8)"


.rodata
    align 16
xxxd: double 1.122
    double 0.9999765432
.text
    ldarc %r21, xxxd
    ldi  %r15, 100
    ldud %r25, %r21, 8*0
    ldud %r26, %r21, 8*1
    fsubq %r22, %r25, %r16
    write "r22     %f128(r22)"
xxloop:
    fmaddq %r22, %r25, %r16, %r22
    fmsubq %r22, %r25, %r16, %r22
    repged %r15, %gz, 1, xxloop
    write "r22     %f128(r22)"

    write "other FPU"
    fmaddq  %r60, %r61, %r62, %r63
    fmsubq  %r61, %r61, %r72, %r73
    fnmaddq %r62, %r71, %r82, %r63
    fnmsubq %r63, %r81, %r12, %r53

    fmulq %r64, %r61, %r22
    fdivq %r65, %r11, %r27
    faddq %r66, %r17, %r42
    fsubq %r67, %r31, %r23
    fnaddq %r68, %r41, %r62
    fmaxq %r60, %r61, %r62
    fminq %r60, %r61, %r62
    famaxnmq %r60, %r61, %r62
    faminnmq %r60, %r61, %r62

    fcmpoltq %r10, %r61, %r72
    fcmpogeq %r11, %r52, %r21
    fcmpogeq %r12, %r43, %r12
    fcmpoeqq %r10, %r34, %r44
    fcmpueqq %r13, %r25, %r22
    fcmpugeq %r12, %r15, %r23
    fcmpuq %r11, %r86, %r86

    fnegq %r24, %r58
    fabdq %r45, %r61, %r20
    fnabdq %r56, %r32, %r20
    frndq %r78, %r74, rmm
    frndq %r89, %r65, rtz
    frndq %r81, %r76, rdn
    frndq %r62, %r67, rup
    fsqrtq %r63, %r78
    frsqrtq %r64, %r69

    addid %r45, %sp,-4800
    ldi %r13, 2

    lduw %r12, %r45, 4*1
    stw %r12, %r45, 4*1
    ldud %r12, %r45, 8*3
    std %r12, %r45, 8*3
    lduwxsd %r12, %r45, %r13, 2, 60
    stwxsd %r12, %r45, %r13, 2, 60
    ldudxsd %r12, %r45, %r13, 3, 60
    stdxsd %r12, %r45, %r13, 3, 60

    faddq %r23, %r24, %r25
    fmaddq %r23, %r60, %r55, %r33
    fmulq %r23, %r60, %r55
    ldud %r60, %r45, 8*6
    fmaddq %r23, %r60, %r55, %r33
    fmaddq %r24, %r61, %r25, %r32
    fmaddq %r25, %r62, %r55, %r23
    fmaddq %r26, %r63, %r75, %r73
    fmaddq %r27, %r64, %r75, %r73
    fmaddq %r28, %r65, %r85, %r63
    fmaddq %r29, %r66, %r85, %r63
    fmaddq %r30, %r67, %r55, %r23
    fmaddq %r31, %r68, %r55, %r23
    fmaddq %r12, %r32, %r76, %r85
    fmaddq %r12, %r32, %r76, %r85
    fmaddq %r10, %r32, %r76, %r85
    fmaddq %r10, %r32, %r76, %r85
    fmaddq %r10, %r32, %r76, %r85
    fmaddq %r13, %r32, %r76, %r85
    fmaddq %r14, %r32, %r76, %r85
    fmaddq %r15, %r32, %r76, %r85
    fmaddq %r16, %r32, %r76, %r85
    fmaddq %r17, %r32, %r76, %r85

    fqtoiw %r56, %r45, rmm
    fqtouw %r56, %r45, rmm
    fiwtoq %r45, %r56, rmm
    fuwtoq %r45, %r56, rmm

    ldi  %r3, 0
    fldqr %r4, 1.0
    fldqr %r5, 1.0
    fldqr %r6, 1.0
    fldqr %r7, 1.0
    ldi  %r24, 128
tri_repeat:
    write "r7      %x128(r7)"
    faddq %r5, %r5, %r4
    fmulq %r6, %r6, %r5
    fdivq %r7, %r4, %r6
;   write "%x128(r6)"
    repled.l %r3, %r24, 1, tri_repeat

    write "test taylor series"
    fldqr %r2, 0.44567 ; f2 ,  x
    write "x:   %f128(r2)"  ; test value
    write "test sin(x)"
    fldqr %r5, sin(0.44567)
    write "sin: %f128(r5)"  ; test value
    ldi  %r3, 0  ; s ,  0
    fmulq %r4, %r2, %r2 ; f4 ,  x*x
    fmaddq %r3, %r3, %r4, %r25 ; s ,  s * x*x + 1/25!
    fmsubq %r3, %r3, %r4, %r23 ; s ,  s * x*x - 1/23!
    fmaddq %r3, %r3, %r4, %r21
    fmsubq %r3, %r3, %r4, %r19
    fmaddq %r3, %r3, %r4, %r17
    fmsubq %r3, %r3, %r4, %r15
    fmaddq %r3, %r3, %r4, %r13
    fmsubq %r3, %r3, %r4, %r11
    fmaddq %r3, %r3, %r4, %r9
    fmsubq %r3, %r3, %r4, %r7
    fmaddq %r3, %r3, %r4, %r5
    fmsubq %r3, %r3, %r4, %r3
    fmaddq %r3, %r3, %r4, %r1
    fmulq %r3, %r3, %r2 ; s ,  s * x
    write "sin: %f128(r3)"

    write "test cos(x)"
    fldqr %r5, cos(0.44567)
    write "cos: %f128(r5)"  ; test value
    ldi  %r3, 0  ; s ,  0
    fmulq %r4, %r2, %r2 ; f4 ,  x*x
    fmsubq %r3, %r3, %r4, %r26
    fmaddq %r3, %r3, %r4, %r24
    fmsubq %r3, %r3, %r4, %r22
    fmaddq %r3, %r3, %r4, %r20
    fmsubq %r3, %r3, %r4, %r18
    fmaddq %r3, %r3, %r4, %r16
    fmsubq %r3, %r3, %r4, %r14
    fmaddq %r3, %r3, %r4, %r12
    fmsubq %r3, %r3, %r4, %r10
    fmaddq %r3, %r3, %r4, %r8
    fmsubq %r3, %r3, %r4, %r6
    fmaddq %r3, %r3, %r4, %r4
    fmsubq %r3, %r3, %r4, %r2
    fmaddq %r3, %r3, %r4, %r1
    write "cos: %f128(r3)"

    write "test exp(x)"
    fldqr %r5, exp(0.44567)
    write "exp: %f128(r5)" ; test value
    ldi %r3, 0 ; s ,  0.0
    mov %r4, %r2 ; f4 ,  x
    fldid %r6, 0.125
;   write "%f128(r6)"
    fmulq %r4, %r4, %r6 ; x ,  x/8
    fmaddq %r3, %r3, %r4, %r15
    fmaddq %r3, %r3, %r4, %r14
    fmaddq %r3, %r3, %r4, %r13
    fmaddq %r3, %r3, %r4, %r12
    fmaddq %r3, %r3, %r4, %r11
    fmaddq %r3, %r3, %r4, %r10
    fmaddq %r3, %r3, %r4, %r9
    fmaddq %r3, %r3, %r4, %r8
    fmaddq %r3, %r3, %r4, %r7
    fmaddq %r3, %r3, %r4, %r6
    fmaddq %r3, %r3, %r4, %r5
    fmaddq %r3, %r3, %r4, %r4
    fmaddq %r3, %r3, %r4, %r3
    fmaddq %r3, %r3, %r4, %r2
    fmaddq %r3, %r3, %r4, %r1
    fmaddq %r3, %r3, %r4, %r1
    fmulq %r3, %r3, %r3 ; (e^x) ^ 8
    fmulq %r3, %r3, %r3
    fmulq %r3, %r3, %r3
    write "exp: %f128(r3)"

    faddq %r1, %r2, %r3
    fmaddq %r2, %r10, %r20, %r30
    fmaddq %r1, %r11, %r21, %r31

    ; classification
    fclassh %r4, %r5, 120
    fclasss %r4, %r5, 120
    fclassd %r4, %r5, 120
    fclassq %r4, %r5, 120

    fclassh %r4, %r5, 1023
    fclasss %r4, %r5, 1023
    fclassd %r4, %r5, 1023
    fclassq %r4, %r5, 1023
    jmp skipfma

fpu_backward_target:
; single branches
    fbroeqs %r23, %r34, fpu_backward_target
    fbroeqs.l %r23, %r34, fpu_backward_target
    fbroeqs %r23, %r34, fpu_forward_target
    fbroeqs.l %r23, %r34, fpu_forward_target

    fbrueqs %r23, %r34, fpu_backward_target
    fbrueqs.l %r23, %r34, fpu_backward_target
    fbrueqs %r23, %r34, fpu_forward_target
    fbrueqs.l %r23, %r34, fpu_forward_target

    fbrones %r23, %r34, fpu_backward_target
    fbrones.l %r23, %r34, fpu_backward_target
    fbrones %r23, %r34, fpu_forward_target
    fbrones.l %r23, %r34, fpu_forward_target

    fbrunes %r23, %r34, fpu_backward_target
    fbrunes.l %r23, %r34, fpu_backward_target
    fbrunes %r23, %r34, fpu_forward_target
    fbrunes.l %r23, %r34, fpu_forward_target

    fbrolts %r23, %r34, fpu_backward_target
    fbrolts.l %r23, %r34, fpu_backward_target
    fbrolts %r23, %r34, fpu_forward_target
    fbrolts.l %r23, %r34, fpu_forward_target

    fbrults %r23, %r34, fpu_backward_target
    fbrults.l %r23, %r34, fpu_backward_target
    fbrults %r23, %r34, fpu_forward_target
    fbrults.l %r23, %r34, fpu_forward_target

    fbroges %r23, %r34, fpu_backward_target
    fbroges.l %r23, %r34, fpu_backward_target
    fbroges %r23, %r34, fpu_forward_target
    fbroges.l %r23, %r34, fpu_forward_target

    fbruges %r23, %r34, fpu_backward_target
    fbruges.l %r23, %r34, fpu_backward_target
    fbruges %r23, %r34, fpu_forward_target
    fbruges.l %r23, %r34, fpu_forward_target

    fbros %r23, %r34, fpu_backward_target
    fbros.l %r23, %r34, fpu_backward_target
    fbros %r23, %r34, fpu_forward_target
    fbros.l %r23, %r34, fpu_forward_target

    fbrus %r23, %r34, fpu_backward_target
    fbrus.l %r23, %r34, fpu_backward_target
    fbrus %r23, %r34, fpu_forward_target
    fbrus.l %r23, %r34, fpu_forward_target

; double branches
    fbroeqd %r23, %r34, fpu_backward_target
    fbroeqd.l %r23, %r34, fpu_backward_target
    fbroeqd %r23, %r34, fpu_forward_target
    fbroeqd.l %r23, %r34, fpu_forward_target

    fbrueqd %r23, %r34, fpu_backward_target
    fbrueqd.l %r23, %r34, fpu_backward_target
    fbrueqd %r23, %r34, fpu_forward_target
    fbrueqd.l %r23, %r34, fpu_forward_target

    fbroned %r23, %r34, fpu_backward_target
    fbroned.l %r23, %r34, fpu_backward_target
    fbroned %r23, %r34, fpu_forward_target
    fbroned.l %r23, %r34, fpu_forward_target

    fbruned %r23, %r34, fpu_backward_target
    fbruned.l %r23, %r34, fpu_backward_target
    fbruned %r23, %r34, fpu_forward_target
    fbruned.l %r23, %r34, fpu_forward_target

    fbroltd %r23, %r34, fpu_backward_target
    fbroltd.l %r23, %r34, fpu_backward_target
    fbroltd %r23, %r34, fpu_forward_target
    fbroltd.l %r23, %r34, fpu_forward_target

    fbrultd %r23, %r34, fpu_backward_target
    fbrultd.l %r23, %r34, fpu_backward_target
    fbrultd %r23, %r34, fpu_forward_target
    fbrultd.l %r23, %r34, fpu_forward_target

    fbroged %r23, %r34, fpu_backward_target
    fbroged.l %r23, %r34, fpu_backward_target
    fbroged %r23, %r34, fpu_forward_target
    fbroged.l %r23, %r34, fpu_forward_target

    fbruged %r23, %r34, fpu_backward_target
    fbruged.l %r23, %r34, fpu_backward_target
    fbruged %r23, %r34, fpu_forward_target
    fbruged.l %r23, %r34, fpu_forward_target

    fbrod %r23, %r34, fpu_backward_target
    fbrod.l %r23, %r34, fpu_backward_target
    fbrod %r23, %r34, fpu_forward_target
    fbrod.l %r23, %r34, fpu_forward_target

    fbrud %r23, %r34, fpu_backward_target
    fbrud.l %r23, %r34, fpu_backward_target
    fbrud %r23, %r34, fpu_forward_target
    fbrud.l %r23, %r34, fpu_forward_target

; quadruple branches
    fbroeqq %r23, %r34, fpu_backward_target
    fbroeqq.l %r23, %r34, fpu_backward_target
    fbroeqq %r23, %r34, fpu_forward_target
    fbroeqq.l %r23, %r34, fpu_forward_target

    fbrueqq %r23, %r34, fpu_backward_target
    fbrueqq.l %r23, %r34, fpu_backward_target
    fbrueqq %r23, %r34, fpu_forward_target
    fbrueqq.l %r23, %r34, fpu_forward_target

    fbroneq %r23, %r34, fpu_backward_target
    fbroneq.l %r23, %r34, fpu_backward_target
    fbroneq %r23, %r34, fpu_forward_target
    fbroneq.l %r23, %r34, fpu_forward_target

    fbruneq %r23, %r34, fpu_backward_target
    fbruneq.l %r23, %r34, fpu_backward_target
    fbruneq %r23, %r34, fpu_forward_target
    fbruneq.l %r23, %r34, fpu_forward_target

    fbroltq %r23, %r34, fpu_backward_target
    fbroltq.l %r23, %r34, fpu_backward_target
    fbroltq %r23, %r34, fpu_forward_target
    fbroltq.l %r23, %r34, fpu_forward_target

    fbrultq %r23, %r34, fpu_backward_target
    fbrultq.l %r23, %r34, fpu_backward_target
    fbrultq %r23, %r34, fpu_forward_target
    fbrultq.l %r23, %r34, fpu_forward_target

    fbrogeq %r23, %r34, fpu_backward_target
    fbrogeq.l %r23, %r34, fpu_backward_target
    fbrogeq %r23, %r34, fpu_forward_target
    fbrogeq.l %r23, %r34, fpu_forward_target

    fbrugeq %r23, %r34, fpu_backward_target
    fbrugeq.l %r23, %r34, fpu_backward_target
    fbrugeq %r23, %r34, fpu_forward_target
    fbrugeq.l %r23, %r34, fpu_forward_target

    fbroq %r23, %r34, fpu_backward_target
    fbroq.l %r23, %r34, fpu_backward_target
    fbroq %r23, %r34, fpu_forward_target
    fbroq.l %r23, %r34, fpu_forward_target

    fbruq    %r23, %r34, fpu_backward_target
    fbruq.l  %r23, %r34, fpu_backward_target
    fbruq    %r23, %r34, fpu_forward_target
    fbruq.l  %r23, %r34, fpu_forward_target

fpu_forward_target:

    fnulunes %r23, %r34, 1, 1
    fnuluned %r23, %r34, 1, 1
    fnuluneq %r23, %r34, 1, 1

    fnulones %r23, %r34, 1, 1
    fnuloned %r23, %r34, 1, 1
    fnuloneq %r23, %r34, 1, 1

    fnulueqs %r23, %r34, 1, 1
    fnulueqd %r23, %r34, 1, 1
    fnulueqq %r23, %r34, 1, 1

    fnuloeqs %r23, %r34, 1, 1
    fnuloeqd %r23, %r34, 1, 1
    fnuloeqq %r23, %r34, 1, 1

    frndh %r23, %r33, dyn
    frndh %r23, %r33, rne
    frndh %r23, %r33, rdn
    frndh %r23, %r33, rup
    frndh %r23, %r33, rtz
    frndh %r23, %r33, rmm

    frnds %r23, %r33, dyn
    frnds %r23, %r33, rne
    frnds %r23, %r33, rdn
    frnds %r23, %r33, rup
    frnds %r23, %r33, rtz
    frnds %r23, %r33, rmm

    frndd %r23, %r33, dyn
    frndd %r23, %r33, rne
    frndd %r23, %r33, rdn
    frndd %r23, %r33, rup
    frndd %r23, %r33, rtz
    frndd %r23, %r33, rmm

    frndq %r23, %r33, dyn
    frndq %r23, %r33, rne
    frndq %r23, %r33, rdn
    frndq %r23, %r33, rup
    frndq %r23, %r33, rtz
    frndq %r23, %r33, rmm

    frndxh %r23, %r33, dyn
    frndxh %r23, %r33, rne
    frndxh %r23, %r33, rdn
    frndxh %r23, %r33, rup
    frndxh %r23, %r33, rtz
    frndxh %r23, %r33, rmm

    frndxs %r23, %r33, dyn
    frndxs %r23, %r33, rne
    frndxs %r23, %r33, rdn
    frndxs %r23, %r33, rup
    frndxs %r23, %r33, rtz
    frndxs %r23, %r33, rmm

    frndxd %r23, %r33, dyn
    frndxd %r23, %r33, rne
    frndxd %r23, %r33, rdn
    frndxd %r23, %r33, rup
    frndxd %r23, %r33, rtz
    frndxd %r23, %r33, rmm

    frndxq %r23, %r33, dyn
    frndxq %r23, %r33, rne
    frndxq %r23, %r33, rdn
    frndxq %r23, %r33, rup
    frndxq %r23, %r33, rtz
    frndxq %r23, %r33, rmm

skipfma:
    write "end fpu"
.end
