.text
    jmp		endfpsimd
; SSE double (SSE2)
    fmaddpd	%r16, %r71, %r69, %r13
    fmsubpd	%r15, %r78, %r58, %r23
    fnmaddpd	%r14, %r67, %r47, %r13
    fnmsubpd	%r13, %r86, %r36, %r16
    fmaddapd	%r82, %r52, %r69, %r63
    fmsubapd	%r50, %r91, %r69, %r63
    faddpd	%r12, %r86, %r25
    fnaddpd	%r11, %r82, %r19
    fsubpd	%r10, %r63, %r28
    faddcpd	%r81, %r61, %r37
    fsubcpd	%r82, %r81, %r46
    faddhpd	%r83, %r81, %r55
    fsubhpd	%r84, %r71, %r64
    fmulpd	%r81, %r71, %r11
    fmulhpd	%r60, %r11, %r22
    fdotpd	%r85, %r81, %r13
    fminpd	%r86, %r84, %r14
    fmaxpd	%r87, %r61, %r15
    faminpd	%r30, %r52, %r16
    famaxpd	%r61, %r51, %r17

    fcmppdoeq	%r80, %r81, %r63
    fcmppdone	%r11, %r81, %r32
    fcmppdolt	%r15, %r81, %r32
    fcmppdolt	%r60, %r81, %r82
    fcmppdone	%r62, %r72, %r83
    fcmppdole	%r62, %r72, %r62

    fpkpd	%r60, %r61, %r62
    fnegpd	%r61, %r51
    fabsdpd	%r61, %r51, %r3
    fnabsdpd	%r61, %r61, %r3
    frndpd	%r60, %r77,3
    frndpd	%r62, %r61,2
    frndpd	%r62, %r71,0
    frndpd	%r83, %r67,1
    fdivpd	%r83, %r67, %r20
    fsqrtpd	%r68, %r81
    frsqrtpd	%r68, %r81


; quadruple floating-point extension example
.rodata
    align	16
a:	quad	1.234567890123456789124141241241
b:	quad	3.1415925678888734535345231234564561
c:	quad	3.4566345634563456346535463463456
.text
    ldar	%r21, a
    ldq		%r3, %r21,0*16
    ldq		%r1, %r21,1*16
    ldq		%r2, %r21,2*16
    write	"%vf64(r3)"
    write	"%vf64(r1)"
    write	"%vf64(r2)"

    write	"test binary\0"
    fmulsd	%r3, %r1, %r2
    write	"%vf64(r3)"
    fnmulsd	%r3, %r1, %r2
    write	"%vf64(r3)"
    faddsd	%r4, %r1, %r2
    write	"%vf64(r4)"
    fnaddsd	%r4, %r1, %r2
    write	"%vf64(r4)"
    fsubsd	%r4, %r2, %r1
    write	"%vf64(r4)"
    fdivsd	%r4, %r2, %r1
    write	"%vf64(r4)"

    write	"test fused fma\0"
    fmaddsd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"
    fnmaddsd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"
    fmsubsd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"
    fnmsubsd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"

    write	"test unary\0"
    mov		%r6, %r5
    write	"%vf64(r6)"
    fabssd	%r6, %r5
    write	"%vf64(r6)"
    fnegsd	%r6, %r5
    write	"%vf64(r6)"
    fnabssd	%r6, %r5
    write	"%vf64(r6)"
    fsqrtsd	%r6, %r2
    write	"%vf64(r6)"
    frsqrtsd	%r6, %r2
    write	"%vf64(r6)"

    write	"test rounding\0"
    frndsd	%r7, %r2,4
    write	"%vf64(r7)"
    frndsd	%r7, %r2,2
    write	"%vf64(r7)"
    frndsd	%r7, %r2,1
    write	"%vf64(r7)"
    frndsd	%r7, %r2,0
    write	"%vf64(r7)"
    fcvtsd2iw	%r7, %r2,0
    write	"r7=%i64(r7)"
    ldi		%r7, 123456
    fcvtiw2sd	%r7, %r7,0
    write	"%vf64(r7)"

    write	"test minmax, abs minmax"
    fmaxsd	%r8, %r1, %r2
    write	"%vf64(r8)"
    fminsd	%r8, %r1, %r2
    write	"%vf64(r8)"
    famaxsd	%r8, %r1, %r2
    write	"%vf64(r8)"
    faminsd	%r8, %r1, %r2
    write	"%vf64(r8)"

    write	"test fmergesq\0"

.rodata
    align	16
xxxq:	quad	1.122
    quad	0.9999765432
.text
    ldar	%r21, a
; fast_check
    ldi		%r15, 100000 ; 10
    ldq		%r15, %r21, 0*16
    ldq		%r16, %r21, 1*16
    fsubsd	%r22, %r15, %r16
    write	"%vf64(r22)"
yyloop:
    fmaddsd	%r22, %r15, %r16, %r22
    fmsubsd	%r22, %r15, %r16, %r22
    repdge	%r15, %gz, yyloop
    write	"%vf64(r22)"


.rodata
    align	16
    quad	1.189731495357231765085759326628007e+4932
qqqq:   quad	1.23456789 + 32.0
    quad	0.2345678901234567890123456789012345678 + 0.2
    quad	2*asin(1)
    quad	255
dbl1:	double	acos(sin(3.1415926)) ;-1.2345678e+200
    double	444.689679
float1:	float	0.123456789123456789e+30
    float	2.123456789122233
    float	0.0
    float	1.0
octquad:
    quad	0.25
f32:	d4	0x3fff1234
.text
    ldar	%r45, qqqq
    ldar	%r46, dbl1
    ldar	%r47, float1
    write	"r45     %x64(r45)"
    ldq		%r63, %r45,0
    write	"%vf64(r63) %x128(r63)"
    ldq		%r63, %r45,0
    write	"%vf64(r63) %x128(r63)"
    fmulsq	%r62, %r63, %r63
    write	"%vf64(r62)"
    ldwz	%r60, %r47,0
    write	"%vf64(r60)"
    lddz	%r59, %r46,0
    ldwz	%r58, %r47,4
    ldwz	%r57, %r47,8
    write	"%vf64(r57)"
    write	"%vf64(r58)"
    write	"%vf64(r59)"
    ldq		%r53, %r45,1*16
    write	"%vf64(r53)"
    ldq		%r50, %r45,2*16
    write	"%vf64(r50)"
    ldq		%r49, %r45,3*16
    write	"%vf64(r49) %x128(r49)"
    ldwz	%r48, %r47,3*4
    write	"%vf64(r48)"
    fnegsq	%r46, %r48
    write	"%vf64(r46)"
    fmaddsq	%r40, %r52, %r52, %r53
    write	"%m(dump)"

.rodata
    align	16
__yyy:
    quad	0.5
    quad	1.0
    quad	2.25
    quad	22252.22424
    quad	-22252.22424
    quad	34.125
    quad	2.0 / 72.0
    d8	0xffffffffffffffff
    d8	0x3ffe
    d8	0xffffffffffffffff
    d8	0x3ff0
    d8	0x8000000000000000
    d8	0xbff3
    d8	0x8000000000000000
    d8	0xc003
    quad	-1.234567890123456789012345e+6
    d8	0x8000000000000000
    d8	0x3fe0
.text
    ldar	%r12, __yyy
    ldq		%r23, %r12, 0
    write	"%vf64(r23) %x128(r23)"
    ldq		%r23, %r12, 1*16
    write	"%vf64(r23) %x128(r23)"
    ldq		%r23, %r12, 2*16
    write	"%vf64(r23) %x128(r23)"
    ldq		%r23, %r12, 3*16
    write	"%vf64(r23) %x128(r23)"
    ldq		%r23, %r12, 4*16
    write	"%vf64(r23) %x128(r23)"
    ldq		%r23, %r12, 5*16
    write	"%vf64(r23) %x128(r23)"
    ldq		%r23, %r12, 6*16
    write	"%vf64(r23) %x128(r23)"
    ldq		%r27, %r12, 7*16
    write	"%vf64(r27) %x128(r27)"
    ldq		%r27, %r12, 8*16
    write	"%vf64(r27) %x128(r27)"
    ldq		%r27, %r12, 9*16
    write	"%vf64(r27) %x128(r27)"
    ldq		%r27, %r12, 10*16
    write	"%vf64(r27) %x128(r27)"
;   flddi	%r24, 8.5899345919999999995e+09 ;-1.234567890123456789012345e+6
;   write	"%vf64(r24) %x128(f24)"
;   flddi	%r24, 0.125 ; 4.656612873077392578125e-10 ; 4.656612873077392578125e-10
;   write	"%vf64(r24) %x128(f24)"
    ldq		%r25, %r12, 11*16
    write	"%vf64(r25) %x128(r25)"
    ldq		%r25, %r12, 12*16
    write	"%vf64(r25) %x128(r25)"
    fldqri	%r40, 4.345678912345678901234567890123456789012345678
    write	"%vf64(r40)"


    fmaddsd	%r23, %r60, %r55, %r33
    fmaddsd	%r24, %r61, %r25, %r32
    fmaddsd	%r25, %r62, %r55, %r23
    fmaddsd	%r26, %r63, %r75, %r73
    fmaddsd	%r27, %r64, %r75, %r73
    fmaddsd	%r28, %r65, %r85, %r63
    fmaddsd	%r29, %r66, %r85, %r63
    fmaddsd	%r30, %r67, %r95, %r23
    fmaddsd	%r31, %r68, %r95, %r23
    fmaddsd	%r10, %r21, %r26, %r27
    fmaddsd	%r13, %r21, %r26, %r27
    fmaddsd	%r10, %r21, %r26, %r27
    fmaddsd	%r12, %r21, %r26, %r27
    fmaddsd	%r11, %r21, %r26, %r27
    fmaddsd	%r13, %r21, %r26, %r27
    fmaddsd	%r14, %r21, %r26, %r27
    fmaddsd	%r15, %r21, %r26, %r27
    fmaddsd	%r16, %r21, %r26, %r27
    fmaddsd	%r17, %r21, %r26, %r27

    stq	%r16, %sp,16*2
    stq	%r17, %sp,16*3
    stq	%r18, %sp,16*4
    stq	%r19, %sp,16*5
    stq	%r20, %sp,16*6
    stq	%r21, %sp,16*7
    stq	%r22, %sp,16*8
    stq	%r23, %sp,16*9
    stq	%r24, %sp,16*10
    stq	%r25, %sp,16*11
    stq	%r26, %sp,16*12
    stq	%r27, %sp,16*13
    stq	%r28, %sp,16*14
    stq	%r29, %sp,16*15
    stq	%r30, %sp,16*16
    stq	%r31, %sp,16*17


; SSE single
    fmaddps	%r58, %r61, %r92, %r63
    fmsubps	%r82, %r52, %r92, %r63
    fnmaddps	%r82, %r52, %r69, %r63
    fnmsubps	%r50, %r91, %r69, %r63
    fmaddaps	%r82, %r52, %r69, %r63
    fmsubaps	%r50, %r91, %r69, %r63
    faddps	%r61, %r94, %r69
    fnaddps	%r68, %r54, %r72
    fsubps	%r68, %r61, %r82
    faddcps	%r81, %r71, %r82
    fsubcps	%r82, %r71, %r82
    faddhps	%r62, %r61, %r82
    fsubhps	%r62, %r61, %r62
    fmulps	%r62, %r51, %r62
    fmulhps	%r63, %r51, %r62
    fdotps	%r83, %r51, %r62
    fminps	%r83, %r61, %r62
    fmaxps	%r63, %r71, %r62
    faminps	%r64, %r71, %r82
    famaxps	%r64, %r71, %r82

    fcmppsone	%r65, %r61, %r62
    fcmppsolt	%r74, %r61, %r62
    fcmppsole	%r83, %r61, %r62
    fcmppsule	%r72, %r61, %r62
    fcmppsule	%r11, %r61, %r62
    fcmppsuo	%r20, %r61, %r62

    fpkps	%r33, %r64, %r62
    fnegps	%r60, %r69
    fabsdps	%r61, %r68, %r3
    fnabsdps	%r62, %r67, %r3
    frndps	%r63, %r66,0
    frndps	%r64, %r65,2
    frndps	%r65, %r64,1
    frndps	%r66, %r63,0
    fdivps	%r67, %r62, %r20
    fsqrtps	%r68, %r61
    frsqrtps	%r69, %r60

    faddps	%r24, %r61, %r60
    fmulpd	%r47, %r60, %r46

endfpsimd:

.end
