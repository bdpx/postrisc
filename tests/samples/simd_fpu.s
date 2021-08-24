.text
    jmp  endfpsimd
; SSE double (SSE2)
    madd_vf64 %r16, %r71, %r69, %r13
    msub_vf64 %r15, %r78, %r58, %r23
    nmadd_vf64 %r14, %r67, %r47, %r13
    nmsub_vf64 %r13, %r86, %r36, %r16
    madd_alt_vf64 %r82, %r52, %r69, %r63
    msub_alt_vf64 %r50, %r91, %r69, %r63
    add_vf64 %r12, %r86, %r25
    nadd_vf64 %r11, %r82, %r19
    sub_vf64 %r10, %r63, %r28
    add_alt_vf64 %r81, %r61, %r37
    sub_alt_vf64 %r82, %r81, %r46
    add_horiz_vf64 %r83, %r81, %r55
    sub_horiz_vf64 %r84, %r71, %r64
    mul_vf64 %r81, %r71, %r11
    mul_horiz_vf64 %r60, %r11, %r22
    dot_vf64 %r85, %r81, %r13
    min_vf64 %r86, %r84, %r14
    max_vf64 %r87, %r61, %r15
    abs_min_vf64 %r30, %r52, %r16
    abs_max_vf64 %r61, %r51, %r17

    cmp_oeq_vf64 %r80, %r81, %r63
    cmp_one_vf64 %r11, %r81, %r32
    cmp_olt_vf64 %r15, %r81, %r32
    cmp_olt_vf64 %r60, %r81, %r82
    cmp_one_vf64 %r62, %r72, %r83
    cmp_oge_vf64 %r62, %r72, %r62

    pack_vf64 %r60, %r61, %r62
    neg_vf64 %r61, %r51
    abs_diff_vf64 %r61, %r51, %r3
    nabs_diff_vf64 %r61, %r61, %r3
    floor_vf64 %r60, %r77
    ceil_vf64 %r62, %r61
    roundeven_vf64 %r62, %r71
    trunc_vf64 %r83, %r67
    div_vf64 %r83, %r67, %r20
    sqrt_vf64 %r68, %r81
    rsqrt_vf64 %r68, %r81


; quadruple floating-point extension example
.rodata
    align 16
a: quad 1.234567890123456789124141241241
b: quad 3.1415925678888734535345231234564561
c: quad 3.4566345634563456346535463463456
.text
    lda_r %r21, a
    ld_i128  %r3, %r21,0*16
    ld_i128  %r1, %r21,1*16
    ld_i128  %r2, %r21,2*16
    write "%vf64(r3)"
    write "%vf64(r1)"
    write "%vf64(r2)"

    write "test binary\0"
    mul_f64 %r3, %r1, %r2
    write "%vf64(r3)"
    nmul_f64 %r3, %r1, %r2
    write "%vf64(r3)"
    add_f64 %r4, %r1, %r2
    write "%vf64(r4)"
    nadd_f64 %r4, %r1, %r2
    write "%vf64(r4)"
    sub_f64 %r4, %r2, %r1
    write "%vf64(r4)"
    div_f64 %r4, %r2, %r1
    write "%vf64(r4)"

    write "test fused fma\0"
    madd_f64 %r5, %r4, %r1, %r2
    write "%vf64(r5)"
    nmadd_f64 %r5, %r4, %r1, %r2
    write "%vf64(r5)"
    msub_f64 %r5, %r4, %r1, %r2
    write "%vf64(r5)"
    nmsub_f64 %r5, %r4, %r1, %r2
    write "%vf64(r5)"

    write "test unary\0"
    mov  %r6, %r5
    write "%vf64(r6)"
    abs_f64 %r6, %r5
    write "%vf64(r6)"
    neg_f64 %r6, %r5
    write "%vf64(r6)"
    nabs_f64 %r6, %r5
    write "%vf64(r6)"
    sqrt_f64 %r6, %r2
    write "%vf64(r6)"
    rsqrt_f64 %r6, %r2
    write "%vf64(r6)"

    write "test rounding\0"
    ceil_f64 %r7, %r2
    write "%vf64(r7)"
    trunc_f64 %r7, %r2
    write "%vf64(r7)"
    floor_f64 %r7, %r2
    write "%vf64(r7)"
    roundeven_f64 %r7, %r2
    write "%vf64(r7)"
    trunc_f64_i32 %r7, %r2
    write "r7=%i64(r7)"
    ld_imm %r7, 123456
    cvt_i32_f64 %r7, %r7
    write "%vf64(r7)"

    write "test minmax, abs minmax"
    max_f64 %r8, %r1, %r2
    write "%vf64(r8)"
    min_f64 %r8, %r1, %r2
    write "%vf64(r8)"
    abs_max_f64 %r8, %r1, %r2
    write "%vf64(r8)"
    abs_min_f64 %r8, %r1, %r2
    write "%vf64(r8)"

    write "test fmergesq\0"

.rodata
    align 16
xxxq: quad 1.122
    quad 0.9999765432
.text
    lda_r %r21, a
; fast_check
    ld_imm %r15, 100000 ; 10
    ld_i128 %r15, %r21, 0*16
    ld_i128 %r16, %r21, 1*16
    sub_f64 %r22, %r15, %r16
    write "%vf64(r22)"
yyloop:
    madd_f64 %r22, %r15, %r16, %r22
    msub_f64 %r22, %r15, %r16, %r22
    rep_ge_i64 %r15, %gz, 1, yyloop
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
    lda_r %r45, qqqq
    lda_r %r46, dbl1
    lda_r %r47, float1
    write "r45     %x64(r45)"
    ld_i128  %r63, %r45,0
    write "%vf64(r63) %x128(r63)"
    ld_i128  %r63, %r45,0
    write "%vf64(r63) %x128(r63)"
    mul_f128 %r62, %r63, %r63
    write "%vf64(r62)"
    ld_u32 %r60, %r47,0
    write "%vf64(r60)"
    ld_u64 %r59, %r46,0
    ld_u32 %r58, %r47,4
    ld_u32 %r57, %r47,8
    write "%vf64(r57)"
    write "%vf64(r58)"
    write "%vf64(r59)"
    ld_i128 %r53, %r45,1*16
    write "%vf64(r53)"
    ld_i128 %r50, %r45,2*16
    write "%vf64(r50)"
    ld_i128 %r49, %r45,3*16
    write "%vf64(r49) %x128(r49)"
    ld_u32 %r48, %r47,3*4
    write "%vf64(r48)"
    neg_f128 %r46, %r48
    write "%vf64(r46)"
    madd_f128 %r40, %r52, %r52, %r53
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
    lda_r %r12, __yyy
    ld_i128 %r23, %r12, 0
    write "%vf64(r23) %x128(r23)"
    ld_i128 %r23, %r12, 1*16
    write "%vf64(r23) %x128(r23)"
    ld_i128 %r23, %r12, 2*16
    write "%vf64(r23) %x128(r23)"
    ld_i128 %r23, %r12, 3*16
    write "%vf64(r23) %x128(r23)"
    ld_i128 %r23, %r12, 4*16
    write "%vf64(r23) %x128(r23)"
    ld_i128 %r23, %r12, 5*16
    write "%vf64(r23) %x128(r23)"
    ld_i128 %r23, %r12, 6*16
    write "%vf64(r23) %x128(r23)"
    ld_i128 %r27, %r12, 7*16
    write "%vf64(r27) %x128(r27)"
    ld_i128 %r27, %r12, 8*16
    write "%vf64(r27) %x128(r27)"
    ld_i128 %r27, %r12, 9*16
    write "%vf64(r27) %x128(r27)"
    ld_i128 %r27, %r12, 10*16
    write "%vf64(r27) %x128(r27)"
;   flddi %r24, 8.5899345919999999995e+09 ;-1.234567890123456789012345e+6
;   write "%vf64(r24) %x128(f24)"
;   flddi %r24, 0.125 ; 4.656612873077392578125e-10 ; 4.656612873077392578125e-10
;   write "%vf64(r24) %x128(f24)"
    ld_i128 %r25, %r12, 11*16
    write "%vf64(r25) %x128(r25)"
    ld_i128 %r25, %r12, 12*16
    write "%vf64(r25) %x128(r25)"
    ld_iprel_f128 %r40, 4.345678912345678901234567890123456789012345678
    write "%vf64(r40)"


    madd_f64 %r23, %r60, %r55, %r33
    madd_f64 %r24, %r61, %r25, %r32
    madd_f64 %r25, %r62, %r55, %r23
    madd_f64 %r26, %r63, %r75, %r73
    madd_f64 %r27, %r64, %r75, %r73
    madd_f64 %r28, %r65, %r85, %r63
    madd_f64 %r29, %r66, %r85, %r63
    madd_f64 %r30, %r67, %r95, %r23
    madd_f64 %r31, %r68, %r95, %r23
    madd_f64 %r10, %r21, %r26, %r27
    madd_f64 %r13, %r21, %r26, %r27
    madd_f64 %r10, %r21, %r26, %r27
    madd_f64 %r12, %r21, %r26, %r27
    madd_f64 %r11, %r21, %r26, %r27
    madd_f64 %r13, %r21, %r26, %r27
    madd_f64 %r14, %r21, %r26, %r27
    madd_f64 %r15, %r21, %r26, %r27
    madd_f64 %r16, %r21, %r26, %r27
    madd_f64 %r17, %r21, %r26, %r27

    st_i128 %r16, %sp,16*2
    st_i128 %r17, %sp,16*3
    st_i128 %r18, %sp,16*4
    st_i128 %r19, %sp,16*5
    st_i128 %r20, %sp,16*6
    st_i128 %r21, %sp,16*7
    st_i128 %r22, %sp,16*8
    st_i128 %r23, %sp,16*9
    st_i128 %r24, %sp,16*10
    st_i128 %r25, %sp,16*11
    st_i128 %r26, %sp,16*12
    st_i128 %r27, %sp,16*13
    st_i128 %r28, %sp,16*14
    st_i128 %r29, %sp,16*15
    st_i128 %r30, %sp,16*16
    st_i128 %r31, %sp,16*17


; SSE single
    madd_vf32 %r58, %r61, %r92, %r63
    msub_vf32 %r82, %r52, %r92, %r63
    nmadd_vf32 %r82, %r52, %r69, %r63
    nmsub_vf32 %r50, %r91, %r69, %r63
    madd_alt_vf32 %r82, %r52, %r69, %r63
    msub_alt_vf32 %r50, %r91, %r69, %r63
    add_vf32 %r61, %r94, %r69
    nadd_vf32 %r68, %r54, %r72
    sub_vf32 %r68, %r61, %r82
    add_alt_vf32 %r81, %r71, %r82
    sub_alt_vf32 %r82, %r71, %r82
    add_horiz_vf32 %r62, %r61, %r82
    sub_horiz_vf32 %r62, %r61, %r62
    mul_vf32 %r62, %r51, %r62
    mul_horiz_vf32 %r63, %r51, %r62
    dot_vf32 %r83, %r51, %r62
    min_vf32 %r83, %r61, %r62
    max_vf32 %r63, %r71, %r62
    abs_min_vf32 %r64, %r71, %r82
    abs_max_vf32 %r64, %r71, %r82

    cmp_one_vf32 %r65, %r61, %r62
    cmp_olt_vf32 %r74, %r61, %r62
    cmp_oge_vf32 %r83, %r61, %r62
    cmp_uge_vf32 %r72, %r61, %r62
    cmp_uge_vf32 %r11, %r61, %r62
    cmp_u_vf32 %r20, %r61, %r62

    pack_vf32 %r33, %r64, %r62
    neg_vf32 %r60, %r69
    abs_diff_vf32 %r61, %r68, %r3
    nabs_diff_vf32 %r62, %r67, %r3
    floor_vf32 %r63, %r66
    ceil_vf32 %r64, %r65
    roundeven_vf32 %r65, %r64
    trunc_vf32 %r66, %r63
    div_vf32 %r67, %r62, %r20
    sqrt_vf32 %r68, %r61
    rsqrt_vf32 %r69, %r60

    add_vf32 %r24, %r61, %r60
    mul_vf64 %r47, %r60, %r46

endfpsimd:

.end
