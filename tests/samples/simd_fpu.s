.text
    jmp  endfpsimd
; SSE double (SSE2)
    vfmaddd %r16, %r71, %r69, %r13
    vfmsubd %r15, %r78, %r58, %r23
    vfnmaddd %r14, %r67, %r47, %r13
    vfnmsubd %r13, %r86, %r36, %r16
    vfmaddsubd %r82, %r52, %r69, %r63
    vfmsubaddd %r50, %r91, %r69, %r63
    vfaddd %r12, %r86, %r25
    vfnaddd %r11, %r82, %r19
    vfsubd %r10, %r63, %r28
    vfaddsubd %r81, %r61, %r37
    vfsubaddd %r82, %r81, %r46
    vfhaddd %r83, %r81, %r55
    vfhsubd %r84, %r71, %r64
    vfmuld %r81, %r71, %r11
    vfhmuld %r60, %r11, %r22
    vfdotd %r85, %r81, %r13
    vfmind %r86, %r84, %r14
    vfmaxd %r87, %r61, %r15
    vfaminnmd %r30, %r52, %r16
    vfamaxnmd %r61, %r51, %r17

    vfcmpoeqd %r80, %r81, %r63
    vfcmponed %r11, %r81, %r32
    vfcmpoltd %r15, %r81, %r32
    vfcmpoltd %r60, %r81, %r82
    vfcmponed %r62, %r72, %r83
    vfcmpoged %r62, %r72, %r62

    vfpackd %r60, %r61, %r62
    vfnegd %r61, %r51
    vfabdd %r61, %r51, %r3
    vfnabdd %r61, %r61, %r3
    vfrndd %r60, %r77, rdn
    vfrndd %r62, %r61, rup
    vfrndd %r62, %r71, rne
    vfrndd %r83, %r67, rtz
    vfdivd %r83, %r67, %r20
    vfsqrtd %r68, %r81
    vfrsqrtd %r68, %r81


; quadruple floating-point extension example
.rodata
    align 16
a: quad 1.234567890123456789124141241241
b: quad 3.1415925678888734535345231234564561
c: quad 3.4566345634563456346535463463456
.text
    ldarc %r21, a
    ldq  %r3, %r21,0*16
    ldq  %r1, %r21,1*16
    ldq  %r2, %r21,2*16
    write "%vf64(r3)"
    write "%vf64(r1)"
    write "%vf64(r2)"

    write "test binary\0"
    fmuld %r3, %r1, %r2
    write "%vf64(r3)"
    fnmuld %r3, %r1, %r2
    write "%vf64(r3)"
    faddd %r4, %r1, %r2
    write "%vf64(r4)"
    fnaddd %r4, %r1, %r2
    write "%vf64(r4)"
    fsubd %r4, %r2, %r1
    write "%vf64(r4)"
    fdivd %r4, %r2, %r1
    write "%vf64(r4)"

    write "test fused fma\0"
    fmaddd %r5, %r4, %r1, %r2
    write "%vf64(r5)"
    fnmaddd %r5, %r4, %r1, %r2
    write "%vf64(r5)"
    fmsubd %r5, %r4, %r1, %r2
    write "%vf64(r5)"
    fnmsubd %r5, %r4, %r1, %r2
    write "%vf64(r5)"

    write "test unary\0"
    mov  %r6, %r5
    write "%vf64(r6)"
    fabsd %r6, %r5
    write "%vf64(r6)"
    fnegd %r6, %r5
    write "%vf64(r6)"
    fnabsd %r6, %r5
    write "%vf64(r6)"
    fsqrtd %r6, %r2
    write "%vf64(r6)"
    frsqrtd %r6, %r2
    write "%vf64(r6)"

    write "test rounding\0"
    frndd %r7, %r2, rup
    write "%vf64(r7)"
    frndd %r7, %r2, rtz
    write "%vf64(r7)"
    frndd %r7, %r2, rdn
    write "%vf64(r7)"
    frndd %r7, %r2, rne
    write "%vf64(r7)"
    fdtoiw %r7, %r2, rtz
    write "r7=%i64(r7)"
    ldi %r7, 123456
    fiwtod %r7, %r7
    write "%vf64(r7)"

    write "test minmax, abs minmax"
    fmaxd %r8, %r1, %r2
    write "%vf64(r8)"
    fmind %r8, %r1, %r2
    write "%vf64(r8)"
    famaxnmd %r8, %r1, %r2
    write "%vf64(r8)"
    faminnmd %r8, %r1, %r2
    write "%vf64(r8)"

    write "test fmergesq\0"

.rodata
    align 16
xxxq: quad 1.122
    quad 0.9999765432
.text
    ldarc %r21, a
; fast_check
    ldi %r15, 100000 ; 10
    ldq %r15, %r21, 0*16
    ldq %r16, %r21, 1*16
    fsubd %r22, %r15, %r16
    write "%vf64(r22)"
yyloop:
    fmaddd %r22, %r15, %r16, %r22
    fmsubd %r22, %r15, %r16, %r22
    repged %r15, %gz, 1, yyloop
    write "%vf64(r22)"


.rodata
    align 16
    quad 1.189731495357231765085759326628007e+4932
qqqq:   quad 1.23456789 + 32.0
    quad 0.2345678901234567890123456789012345678 + 0.2
    quad 2*asin(1)
    quad 255
dbl1: double acos(sin(3.1415926)) ;-1.2345678e+200
    double 444.689679
float1: float 0.123456789123456789e+30
    float 2.123456789122233
    float 0.0
    float 1.0
octquad:
    quad 0.25
f32: d4 0x3fff1234
.text
    ldarc %r45, qqqq
    ldarc %r46, dbl1
    ldarc %r47, float1
    write "r45     %x64(r45)"
    ldq  %r63, %r45,0
    write "%vf64(r63) %x128(r63)"
    ldq  %r63, %r45,0
    write "%vf64(r63) %x128(r63)"
    fmulq %r62, %r63, %r63
    write "%vf64(r62)"
    lduw %r60, %r47,0
    write "%vf64(r60)"
    ldud %r59, %r46,0
    lduw %r58, %r47,4
    lduw %r57, %r47,8
    write "%vf64(r57)"
    write "%vf64(r58)"
    write "%vf64(r59)"
    ldq %r53, %r45,1*16
    write "%vf64(r53)"
    ldq %r50, %r45,2*16
    write "%vf64(r50)"
    ldq %r49, %r45,3*16
    write "%vf64(r49) %x128(r49)"
    lduw %r48, %r47,3*4
    write "%vf64(r48)"
    fnegq %r46, %r48
    write "%vf64(r46)"
    fmaddq %r40, %r52, %r52, %r53
    write "%m(dump)"

.rodata
    align 16
__yyy:
    quad 0.5
    quad 1.0
    quad 2.25
    quad 22252.22424
    quad -22252.22424
    quad 34.125
    quad 2.0 / 72.0
    d8 0xffffffffffffffff
    d8 0x3ffe
    d8 0xffffffffffffffff
    d8 0x3ff0
    d8 0x8000000000000000
    d8 0xbff3
    d8 0x8000000000000000
    d8 0xc003
    quad -1.234567890123456789012345e+6
    d8 0x8000000000000000
    d8 0x3fe0
.text
    ldarc %r12, __yyy
    ldq %r23, %r12, 0
    write "%vf64(r23) %x128(r23)"
    ldq %r23, %r12, 1*16
    write "%vf64(r23) %x128(r23)"
    ldq %r23, %r12, 2*16
    write "%vf64(r23) %x128(r23)"
    ldq %r23, %r12, 3*16
    write "%vf64(r23) %x128(r23)"
    ldq %r23, %r12, 4*16
    write "%vf64(r23) %x128(r23)"
    ldq %r23, %r12, 5*16
    write "%vf64(r23) %x128(r23)"
    ldq %r23, %r12, 6*16
    write "%vf64(r23) %x128(r23)"
    ldq %r27, %r12, 7*16
    write "%vf64(r27) %x128(r27)"
    ldq %r27, %r12, 8*16
    write "%vf64(r27) %x128(r27)"
    ldq %r27, %r12, 9*16
    write "%vf64(r27) %x128(r27)"
    ldq %r27, %r12, 10*16
    write "%vf64(r27) %x128(r27)"
;   flddi %r24, 8.5899345919999999995e+09 ;-1.234567890123456789012345e+6
;   write "%vf64(r24) %x128(f24)"
;   flddi %r24, 0.125 ; 4.656612873077392578125e-10 ; 4.656612873077392578125e-10
;   write "%vf64(r24) %x128(f24)"
    ldq %r25, %r12, 11*16
    write "%vf64(r25) %x128(r25)"
    ldq %r25, %r12, 12*16
    write "%vf64(r25) %x128(r25)"
    fldqr %r40, 4.345678912345678901234567890123456789012345678
    write "%vf64(r40)"


    fmaddd %r23, %r60, %r55, %r33
    fmaddd %r24, %r61, %r25, %r32
    fmaddd %r25, %r62, %r55, %r23
    fmaddd %r26, %r63, %r75, %r73
    fmaddd %r27, %r64, %r75, %r73
    fmaddd %r28, %r65, %r85, %r63
    fmaddd %r29, %r66, %r85, %r63
    fmaddd %r30, %r67, %r95, %r23
    fmaddd %r31, %r68, %r95, %r23
    fmaddd %r10, %r21, %r26, %r27
    fmaddd %r13, %r21, %r26, %r27
    fmaddd %r10, %r21, %r26, %r27
    fmaddd %r12, %r21, %r26, %r27
    fmaddd %r11, %r21, %r26, %r27
    fmaddd %r13, %r21, %r26, %r27
    fmaddd %r14, %r21, %r26, %r27
    fmaddd %r15, %r21, %r26, %r27
    fmaddd %r16, %r21, %r26, %r27
    fmaddd %r17, %r21, %r26, %r27

    stq %r16, %sp,16*2
    stq %r17, %sp,16*3
    stq %r18, %sp,16*4
    stq %r19, %sp,16*5
    stq %r20, %sp,16*6
    stq %r21, %sp,16*7
    stq %r22, %sp,16*8
    stq %r23, %sp,16*9
    stq %r24, %sp,16*10
    stq %r25, %sp,16*11
    stq %r26, %sp,16*12
    stq %r27, %sp,16*13
    stq %r28, %sp,16*14
    stq %r29, %sp,16*15
    stq %r30, %sp,16*16
    stq %r31, %sp,16*17


; SSE single
    vfmadds %r58, %r61, %r92, %r63
    vfmsubs %r82, %r52, %r92, %r63
    vfnmadds %r82, %r52, %r69, %r63
    vfnmsubs %r50, %r91, %r69, %r63
    vfmaddsubs %r82, %r52, %r69, %r63
    vfmsubadds %r50, %r91, %r69, %r63
    vfadds %r61, %r94, %r69
    vfnadds %r68, %r54, %r72
    vfsubs %r68, %r61, %r82
    vfaddsubs %r81, %r71, %r82
    vfsubadds %r82, %r71, %r82
    vfhadds %r62, %r61, %r82
    vfhsubs %r62, %r61, %r62
    vfmuls %r62, %r51, %r62
    vfhmuls %r63, %r51, %r62
    vfdots %r83, %r51, %r62
    vfmins %r83, %r61, %r62
    vfmaxs %r63, %r71, %r62
    vfaminnms %r64, %r71, %r82
    vfamaxnms %r64, %r71, %r82

    vfcmpones %r65, %r61, %r62
    vfcmpolts %r74, %r61, %r62
    vfcmpoges %r83, %r61, %r62
    vfcmpuges %r72, %r61, %r62
    vfcmpuges %r11, %r61, %r62
    vfcmpus %r20, %r61, %r62

    vfpacks %r33, %r64, %r62
    vfnegs %r60, %r69
    vfabds %r61, %r68, %r3
    vfnabds %r62, %r67, %r3
    vfrnds %r63, %r66, rdn
    vfrnds %r64, %r65, rup
    vfrnds %r65, %r64, rne
    vfrnds %r66, %r63, rtz
    vfdivs %r67, %r62, %r20
    vfsqrts %r68, %r61
    vfrsqrts %r69, %r60

    vfadds %r24, %r61, %r60
    vfmuld %r47, %r60, %r46

endfpsimd:

.end
