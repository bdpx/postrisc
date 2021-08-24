.text
    jmp		endfpsimd
; SSE double (SSE2)
    fmadd.pd	%r16, %r71, %r69, %r13
    fmsub.pd	%r15, %r78, %r58, %r23
    fnmadd.pd	%r14, %r67, %r47, %r13
    fnmsub.pd	%r13, %r86, %r36, %r16
    fmadda.pd	%r82, %r52, %r69, %r63
    fmsuba.pd	%r50, %r91, %r69, %r63
    fadd.pd	%r12, %r86, %r25
    fnadd.pd	%r11, %r82, %r19
    fsub.pd	%r10, %r63, %r28
    faddc.pd	%r81, %r61, %r37
    fsubc.pd	%r82, %r81, %r46
    faddh.pd	%r83, %r81, %r55
    fsubh.pd	%r84, %r71, %r64
    fmul.pd	%r81, %r71, %r11
    fmulh.pd	%r60, %r11, %r22
    fdot.pd	%r85, %r81, %r13
    fmin.pd	%r86, %r84, %r14
    fmax.pd	%r87, %r61, %r15
    famin.pd	%r30, %r52, %r16
    famax.pd	%r61, %r51, %r17

    fcmpo.eq.pd	%r80, %r81, %r63
    fcmpo.ne.pd	%r11, %r81, %r32
    fcmpo.lt.pd	%r15, %r81, %r32
    fcmpo.lt.pd	%r60, %r81, %r82
    fcmpo.ne.pd	%r62, %r72, %r83
    fcmpo.le.pd	%r62, %r72, %r62

    fpk.pd	%r60, %r61, %r62
    fneg.pd	%r61, %r51
    fabsd.pd	%r61, %r51, %r3
    fnabsd.pd	%r61, %r61, %r3
    frnd.pd	%r60, %r77,3
    frnd.pd	%r62, %r61,2
    frnd.pd	%r62, %r71,0
    frnd.pd	%r83, %r67,1
    fdiv.pd	%r83, %r67, %r20
    fsqrt.pd	%r68, %r81
    frsqrt.pd	%r68, %r81


; quadruple floating-point extension example
.rodata
    align	16
a:	quad	1.234567890123456789124141241241
b:	quad	3.1415925678888734535345231234564561
c:	quad	3.4566345634563456346535463463456
.text
    ca.r	%r21, a
    ld.q		%r3, %r21,0*16
    ld.q		%r1, %r21,1*16
    ld.q		%r2, %r21,2*16
    write	"%vf64(r3)"
    write	"%vf64(r1)"
    write	"%vf64(r2)"

    write	"test binary\0"
    fmul.sd	%r3, %r1, %r2
    write	"%vf64(r3)"
    fnmul.sd	%r3, %r1, %r2
    write	"%vf64(r3)"
    fadd.sd	%r4, %r1, %r2
    write	"%vf64(r4)"
    fnadd.sd	%r4, %r1, %r2
    write	"%vf64(r4)"
    fsub.sd	%r4, %r2, %r1
    write	"%vf64(r4)"
    fdiv.sd	%r4, %r2, %r1
    write	"%vf64(r4)"

    write	"test fused fma\0"
    fmadd.sd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"
    fnmadd.sd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"
    fmsub.sd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"
    fnmsub.sd	%r5, %r4, %r1, %r2
    write	"%vf64(r5)"

    write	"test unary\0"
    mov		%r6, %r5
    write	"%vf64(r6)"
    fabs.sd	%r6, %r5
    write	"%vf64(r6)"
    fneg.sd	%r6, %r5
    write	"%vf64(r6)"
    fnabs.sd	%r6, %r5
    write	"%vf64(r6)"
    fsqrt.sd	%r6, %r2
    write	"%vf64(r6)"
    frsqrt.sd	%r6, %r2
    write	"%vf64(r6)"

    write	"test rounding\0"
    frnd.sd	%r7, %r2,4
    write	"%vf64(r7)"
    frnd.sd	%r7, %r2,2
    write	"%vf64(r7)"
    frnd.sd	%r7, %r2,1
    write	"%vf64(r7)"
    frnd.sd	%r7, %r2,0
    write	"%vf64(r7)"
    fcvt.sd.iw	%r7, %r2,0
    write	"r7=%i64(r7)"
    ldi		%r7, 123456
    fcvt.iw.sd	%r7, %r7,0
    write	"%vf64(r7)"

    write	"test minmax, abs minmax"
    fmax.sd	%r8, %r1, %r2
    write	"%vf64(r8)"
    fmin.sd	%r8, %r1, %r2
    write	"%vf64(r8)"
    famax.sd	%r8, %r1, %r2
    write	"%vf64(r8)"
    famin.sd	%r8, %r1, %r2
    write	"%vf64(r8)"

    write	"test fmergesq\0"

.rodata
    align	16
xxxq:	quad	1.122
    quad	0.9999765432
.text
    ca.r	%r21, a
; fast_check
    ldi		%r15, 100000 ; 10
    ld.q	%r15, %r21, 0*16
    ld.q	%r16, %r21, 1*16
    fsub.sd	%r22, %r15, %r16
    write	"%vf64(r22)"
yyloop:
    fmadd.sd	%r22, %r15, %r16, %r22
    fmsub.sd	%r22, %r15, %r16, %r22
    reps.ge.d	%r15, %gz, yyloop
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
    ca.r	%r45, qqqq
    ca.r	%r46, dbl1
    ca.r	%r47, float1
    write	"r45     %x64(r45)"
    ld.q		%r63, %r45,0
    write	"%vf64(r63) %x128(r63)"
    ld.q		%r63, %r45,0
    write	"%vf64(r63) %x128(r63)"
    fmul.sq	%r62, %r63, %r63
    write	"%vf64(r62)"
    ldz.w	%r60, %r47,0
    write	"%vf64(r60)"
    ldz.d	%r59, %r46,0
    ldz.w	%r58, %r47,4
    ldz.w	%r57, %r47,8
    write	"%vf64(r57)"
    write	"%vf64(r58)"
    write	"%vf64(r59)"
    ld.q	%r53, %r45,1*16
    write	"%vf64(r53)"
    ld.q	%r50, %r45,2*16
    write	"%vf64(r50)"
    ld.q	%r49, %r45,3*16
    write	"%vf64(r49) %x128(r49)"
    ldz.w	%r48, %r47,3*4
    write	"%vf64(r48)"
    fneg.sq	%r46, %r48
    write	"%vf64(r46)"
    fmadd.sq	%r40, %r52, %r52, %r53
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
    ca.r	%r12, __yyy
    ld.q	%r23, %r12, 0
    write	"%vf64(r23) %x128(r23)"
    ld.q	%r23, %r12, 1*16
    write	"%vf64(r23) %x128(r23)"
    ld.q	%r23, %r12, 2*16
    write	"%vf64(r23) %x128(r23)"
    ld.q	%r23, %r12, 3*16
    write	"%vf64(r23) %x128(r23)"
    ld.q	%r23, %r12, 4*16
    write	"%vf64(r23) %x128(r23)"
    ld.q	%r23, %r12, 5*16
    write	"%vf64(r23) %x128(r23)"
    ld.q	%r23, %r12, 6*16
    write	"%vf64(r23) %x128(r23)"
    ld.q	%r27, %r12, 7*16
    write	"%vf64(r27) %x128(r27)"
    ld.q	%r27, %r12, 8*16
    write	"%vf64(r27) %x128(r27)"
    ld.q	%r27, %r12, 9*16
    write	"%vf64(r27) %x128(r27)"
    ld.q	%r27, %r12, 10*16
    write	"%vf64(r27) %x128(r27)"
;   flddi	%r24, 8.5899345919999999995e+09 ;-1.234567890123456789012345e+6
;   write	"%vf64(r24) %x128(f24)"
;   flddi	%r24, 0.125 ; 4.656612873077392578125e-10 ; 4.656612873077392578125e-10
;   write	"%vf64(r24) %x128(f24)"
    ld.q	%r25, %r12, 11*16
    write	"%vf64(r25) %x128(r25)"
    ld.q	%r25, %r12, 12*16
    write	"%vf64(r25) %x128(r25)"
    fldri.q	%r40, 4.345678912345678901234567890123456789012345678
    write	"%vf64(r40)"


    fmadd.sd	%r23, %r60, %r55, %r33
    fmadd.sd	%r24, %r61, %r25, %r32
    fmadd.sd	%r25, %r62, %r55, %r23
    fmadd.sd	%r26, %r63, %r75, %r73
    fmadd.sd	%r27, %r64, %r75, %r73
    fmadd.sd	%r28, %r65, %r85, %r63
    fmadd.sd	%r29, %r66, %r85, %r63
    fmadd.sd	%r30, %r67, %r95, %r23
    fmadd.sd	%r31, %r68, %r95, %r23
    fmadd.sd	%r10, %r21, %r26, %r27
    fmadd.sd	%r13, %r21, %r26, %r27
    fmadd.sd	%r10, %r21, %r26, %r27
    fmadd.sd	%r12, %r21, %r26, %r27
    fmadd.sd	%r11, %r21, %r26, %r27
    fmadd.sd	%r13, %r21, %r26, %r27
    fmadd.sd	%r14, %r21, %r26, %r27
    fmadd.sd	%r15, %r21, %r26, %r27
    fmadd.sd	%r16, %r21, %r26, %r27
    fmadd.sd	%r17, %r21, %r26, %r27

    st.q	%r16, %sp,16*2
    st.q	%r17, %sp,16*3
    st.q	%r18, %sp,16*4
    st.q	%r19, %sp,16*5
    st.q	%r20, %sp,16*6
    st.q	%r21, %sp,16*7
    st.q	%r22, %sp,16*8
    st.q	%r23, %sp,16*9
    st.q	%r24, %sp,16*10
    st.q	%r25, %sp,16*11
    st.q	%r26, %sp,16*12
    st.q	%r27, %sp,16*13
    st.q	%r28, %sp,16*14
    st.q	%r29, %sp,16*15
    st.q	%r30, %sp,16*16
    st.q	%r31, %sp,16*17


; SSE single
    fmadd.ps	%r58, %r61, %r92, %r63
    fmsub.ps	%r82, %r52, %r92, %r63
    fnmadd.ps	%r82, %r52, %r69, %r63
    fnmsub.ps	%r50, %r91, %r69, %r63
    fmadda.ps	%r82, %r52, %r69, %r63
    fmsuba.ps	%r50, %r91, %r69, %r63
    fadd.ps	%r61, %r94, %r69
    fnadd.ps	%r68, %r54, %r72
    fsub.ps	%r68, %r61, %r82
    faddc.ps	%r81, %r71, %r82
    fsubc.ps	%r82, %r71, %r82
    faddh.ps	%r62, %r61, %r82
    fsubh.ps	%r62, %r61, %r62
    fmul.ps	%r62, %r51, %r62
    fmulh.ps	%r63, %r51, %r62
    fdot.ps	%r83, %r51, %r62
    fmin.ps	%r83, %r61, %r62
    fmax.ps	%r63, %r71, %r62
    famin.ps	%r64, %r71, %r82
    famax.ps	%r64, %r71, %r82

    fcmpo.ne.ps	%r65, %r61, %r62
    fcmpo.lt.ps	%r74, %r61, %r62
    fcmpo.le.ps	%r83, %r61, %r62
    fcmpu.le.ps	%r72, %r61, %r62
    fcmpu.le.ps	%r11, %r61, %r62
    fcmpu.ps	%r20, %r61, %r62

    fpk.ps	%r33, %r64, %r62
    fneg.ps	%r60, %r69
    fabsd.ps	%r61, %r68, %r3
    fnabsd.ps	%r62, %r67, %r3
    frnd.ps	%r63, %r66,0
    frnd.ps	%r64, %r65,2
    frnd.ps	%r65, %r64,1
    frnd.ps	%r66, %r63,0
    fdiv.ps	%r67, %r62, %r20
    fsqrt.ps	%r68, %r61
    frsqrt.ps	%r69, %r60

    fadd.ps	%r24, %r61, %r60
    fmul.pd	%r47, %r60, %r46

endfpsimd:

.end
