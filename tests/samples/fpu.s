.text
; floating-point extension example
    alloc 96

    write "test f128 ld rel"
    ld_iprel_f128 %r12, 3.1415926115461431423612436243
    write "f128 ld rel: %f128(r12)"

    write "test fpcr modification (rm=3)"
    ld_imm  %r2, 3
    set_spr %r2, %fpcr
    write "fpcr: %s(fpcr)"
    write "test fpcr modification (rm=2)"
    ld_imm  %r2, 2
    set_spr %r2, %fpcr
    write "fpcr: %s(fpcr)"
    write "test fpcr modification (rm=1)"
    ld_imm  %r2, 1
    set_spr %r2, %fpcr
    write "fpcr: %s(fpcr)"
    write "test fpcr modification (rm=0)"
    ld_imm  %r2, 0
    set_spr %r2, %fpcr
    write "fpcr: %s(fpcr)"

    write "compare f128 ld rel (full mantissa) & long f128 ld imm (63-bit mantissa)"
    ld_iprel_f128 %r30, 3.14159265358979323846123456789012e+400
    write "f128 ld rel: %x128(r30) %f128(r30)"
    ld_imm_f64 %r31, 3.14159265358979323846123456789012
    write "f128 ld imm: %x128(r31) %f64(r31)"
    write "compare f128 ld rel (full mantissa) & short f128 ld imm (21-bit mantissa)"
    ld_iprel_f128 %r30, 3.14159265358979323846123456789012
    write "r30     %x128(r30)"
    ld_imm_f64 %r31, 3.14159265358979323846123456789012
    write "r31     %x128(r31)"
    write "before1"
    write "r30     %f128(r30)"
    write "before2"
    write "r31     %vf64(r31)"
    write "after"
    ld_imm_f64 %r30, -12.3456789e+04
.rodata
    align 16
float64data:
    double 1.234567890123456789124141241241
    double 3.1415925678888734535345231234564561
    double 3.4566345634563456346535463463456
.text
    lda_r %r21, float64data
    ld_u64 %r11, %r21, 8*0
    ld_u64 %r12, %r21, 8*1
    ld_u64 %r13, %r21, 8*2
    write "ld8(f64): %f64(r11) %f64(r12) %f64(r13)"
    ld_iprel_f128 %r14, 2.7182818289201
    write "f128 ld rel: %f128(r14)"

    extend_f64_f128 %r11, %r11
    extend_f64_f128 %r12, %r12
    extend_f64_f128 %r13, %r13

    write "test binary"
    mul_f128 %r15, %r11, %r14
    write "f128 mul:  %f128(r15)"
    nmul_f128 %r15, %r11, %r14
    write "f128 nmul: %f128(r15)"
    add_f128 %r15, %r11, %r14
    write "f128 add:  %f128(r15)"
    nadd_f128 %r15, %r11, %r14
    write "f128 nadd: %f128(r15)"
    sub_f128 %r15, %r14, %r11
    write "f128 subs:  %f128(r15)"
    div_f128 %r15, %r14, %r11
    write "f128 div:  %f128(r15)"

    write "test fused fma"
;   jmp skipfma
    madd_f128 %r15, %r14, %r11, %r12
    write "f128 madd:  %f128(r15)"
    nmadd_f128 %r15, %r14, %r11, %r12
    write "f128 nmadd: %f128(r15)"
    msub_f128 %r15, %r14, %r11, %r12
    write "f128 msub:  %f128(r15)"
    nmsub_f128 %r15, %r14, %r11, %r12
    write "f128 nmsub: %f128(r15)"

    write "test unary"
    mov  %r16, %r15
    write "r16     %f128(r16)"
    abs_f128 %r16, %r15
    write "r16     %f128(r16)"
    neg_f128 %r16, %r15
    write "r16     %f128(r16)"
    nabs_f128 %r16, %r15
    write "r16     %f128(r16)"
    sqrt_f128 %r16, %r12
    write "r16     %f128(r16)"
    rsqrt_f128 %r16, %r12
    write "r16     %f128(r16)"

    write "test rounding"
    trunc_f128 %r17, %r12
    write "r17     %f128(r17)"
    floor_f128 %r17, %r12
    write "r17     %f128(r17)"
    ceil_f128 %r17, %r12
    write "r17     %f128(r17)"
    roundeven_f128 %r17, %r12
    write "r17     %f128(r17)"
    trunc_f128_i32 %r17, %r12
    write "r17     %i64(r17)"
    ld_imm %r17, 123456
    cvt_i32_f128 %r17, %r7
    write "r17     %f128(r17)"

    write "test fp minmax"
    max_f128 %r8, %r11, %r12
    write "r8      %f128(r8)"
    min_f128 %r8, %r11, %r12
    write "r8      %f128(r8)"
    write "test fp abs minmax"
    abs_max_f128 %r8, %r11, %r12
    write "r8      %f128(r8)"
    abs_min_f128 %r8, %r11, %r12
    write "r8      %f128(r8)"

    write "test f128 merge"
    merge_f128 %r8, %r11, %r12, %r14
    write "r8      %f128(r8)"
    merge_f128 %r8, %r14, %r11, %r12
    write "r8      %f128(r8)"


.rodata
    align 16
xxxd: double 1.122
    double 0.9999765432
.text
    lda_r %r21, xxxd
    ld_imm  %r15, 100
    ld_u64 %r25, %r21, 8*0
    ld_u64 %r26, %r21, 8*1
    sub_f128 %r22, %r25, %r16
    write "r22     %f128(r22)"
xxloop:
    madd_f128 %r22, %r25, %r16, %r22
    msub_f128 %r22, %r25, %r16, %r22
    rep_ge_i64 %r15, %gz, 1, xxloop
    write "r22     %f128(r22)"

    write "other FPU"
    madd_f128  %r60, %r61, %r62, %r63
    msub_f128  %r61, %r61, %r72, %r73
    nmadd_f128 %r62, %r71, %r82, %r63
    nmsub_f128 %r63, %r81, %r12, %r53

    mul_f128 %r64, %r61, %r22
    div_f128 %r65, %r11, %r27
    add_f128 %r66, %r17, %r42
    sub_f128 %r67, %r31, %r23
    nadd_f128 %r68, %r41, %r62
    max_f128 %r60, %r61, %r62
    min_f128 %r60, %r61, %r62
    abs_max_f128 %r60, %r61, %r62
    abs_min_f128 %r60, %r61, %r62

    cmp_olt_f128 %r10, %r61, %r72
    cmp_oge_f128 %r11, %r52, %r21
    cmp_oge_f128 %r12, %r43, %r12
    cmp_oeq_f128 %r10, %r34, %r44
    cmp_ueq_f128 %r13, %r25, %r22
    cmp_uge_f128 %r12, %r15, %r23
    cmp_u_f128 %r11, %r86, %r86

    neg_f128 %r24, %r58
    abs_diff_f128 %r45, %r61, %r20
    nabs_diff_f128 %r56, %r32, %r20
    round_f128 %r78, %r74
    trunc_f128 %r89, %r65
    floor_f128 %r81, %r76
    ceil_f128 %r62, %r67
    sqrt_f128 %r63, %r78
    rsqrt_f128 %r64, %r69

    add_imm_i64 %r45, %sp,-4800
    ld_imm %r13, 2

    ld_u32 %r12, %r45, 4*1
    st_i32 %r12, %r45, 4*1
    ld_u64 %r12, %r45, 8*3
    st_i64 %r12, %r45, 8*3
    ld_xi64_u32 %r12, %r45, %r13, 2, 60
    st_xi64_i32 %r12, %r45, %r13, 2, 60
    ld_xi64_u64 %r12, %r45, %r13, 3, 60
    st_xi64_i64 %r12, %r45, %r13, 3, 60

    add_f128 %r23, %r24, %r25
    madd_f128 %r23, %r60, %r55, %r33
    mul_f128 %r23, %r60, %r55
    ld_u64 %r60, %r45, 8*6
    madd_f128 %r23, %r60, %r55, %r33
    madd_f128 %r24, %r61, %r25, %r32
    madd_f128 %r25, %r62, %r55, %r23
    madd_f128 %r26, %r63, %r75, %r73
    madd_f128 %r27, %r64, %r75, %r73
    madd_f128 %r28, %r65, %r85, %r63
    madd_f128 %r29, %r66, %r85, %r63
    madd_f128 %r30, %r67, %r55, %r23
    madd_f128 %r31, %r68, %r55, %r23
    madd_f128 %r12, %r32, %r76, %r85
    madd_f128 %r12, %r32, %r76, %r85
    madd_f128 %r10, %r32, %r76, %r85
    madd_f128 %r10, %r32, %r76, %r85
    madd_f128 %r10, %r32, %r76, %r85
    madd_f128 %r13, %r32, %r76, %r85
    madd_f128 %r14, %r32, %r76, %r85
    madd_f128 %r15, %r32, %r76, %r85
    madd_f128 %r16, %r32, %r76, %r85
    madd_f128 %r17, %r32, %r76, %r85

    trunc_f128_i32 %r56, %r45
    trunc_f128_u32 %r56, %r45
    cvt_i32_f128 %r45, %r56
    cvt_u32_f128 %r45, %r56

    ld_imm  %r3, 0
    ld_iprel_f128 %r4, 1.0
    ld_iprel_f128 %r5, 1.0
    ld_iprel_f128 %r6, 1.0
    ld_iprel_f128 %r7, 1.0
    ld_imm  %r24, 128
tri_repeat:
    write "r7      %x128(r7)"
    add_f128 %r5, %r5, %r4
    mul_f128 %r6, %r6, %r5
    div_f128 %r7, %r4, %r6
;   write "%x128(r6)"
    rep_le_i64.l %r3, %r24, 1, tri_repeat

    write "test taylor series"
    ld_iprel_f128 %r2, 0.44567 ; f2 ,  x
    write "x:   %f128(r2)"  ; test value
    write "test sin(x)"
    ld_iprel_f128 %r5, sin(0.44567)
    write "sin: %f128(r5)"  ; test value
    ld_imm  %r3, 0  ; s ,  0
    mul_f128 %r4, %r2, %r2 ; f4 ,  x*x
    madd_f128 %r3, %r3, %r4, %r25 ; s ,  s * x*x + 1/25!
    msub_f128 %r3, %r3, %r4, %r23 ; s ,  s * x*x - 1/23!
    madd_f128 %r3, %r3, %r4, %r21
    msub_f128 %r3, %r3, %r4, %r19
    madd_f128 %r3, %r3, %r4, %r17
    msub_f128 %r3, %r3, %r4, %r15
    madd_f128 %r3, %r3, %r4, %r13
    msub_f128 %r3, %r3, %r4, %r11
    madd_f128 %r3, %r3, %r4, %r9
    msub_f128 %r3, %r3, %r4, %r7
    madd_f128 %r3, %r3, %r4, %r5
    msub_f128 %r3, %r3, %r4, %r3
    madd_f128 %r3, %r3, %r4, %r1
    mul_f128 %r3, %r3, %r2 ; s ,  s * x
    write "sin: %f128(r3)"

    write "test cos(x)"
    ld_iprel_f128 %r5, cos(0.44567)
    write "cos: %f128(r5)"  ; test value
    ld_imm  %r3, 0  ; s ,  0
    mul_f128 %r4, %r2, %r2 ; f4 ,  x*x
    msub_f128 %r3, %r3, %r4, %r26
    madd_f128 %r3, %r3, %r4, %r24
    msub_f128 %r3, %r3, %r4, %r22
    madd_f128 %r3, %r3, %r4, %r20
    msub_f128 %r3, %r3, %r4, %r18
    madd_f128 %r3, %r3, %r4, %r16
    msub_f128 %r3, %r3, %r4, %r14
    madd_f128 %r3, %r3, %r4, %r12
    msub_f128 %r3, %r3, %r4, %r10
    madd_f128 %r3, %r3, %r4, %r8
    msub_f128 %r3, %r3, %r4, %r6
    madd_f128 %r3, %r3, %r4, %r4
    msub_f128 %r3, %r3, %r4, %r2
    madd_f128 %r3, %r3, %r4, %r1
    write "cos: %f128(r3)"

    write "test exp(x)"
    ld_iprel_f128 %r5, exp(0.44567)
    write "exp: %f128(r5)" ; test value
    ld_imm %r3, 0 ; s ,  0.0
    mov %r4, %r2 ; f4 ,  x
    ld_imm_f64 %r6, 0.125
;   write "%f128(r6)"
    mul_f128 %r4, %r4, %r6 ; x ,  x/8
    madd_f128 %r3, %r3, %r4, %r15
    madd_f128 %r3, %r3, %r4, %r14
    madd_f128 %r3, %r3, %r4, %r13
    madd_f128 %r3, %r3, %r4, %r12
    madd_f128 %r3, %r3, %r4, %r11
    madd_f128 %r3, %r3, %r4, %r10
    madd_f128 %r3, %r3, %r4, %r9
    madd_f128 %r3, %r3, %r4, %r8
    madd_f128 %r3, %r3, %r4, %r7
    madd_f128 %r3, %r3, %r4, %r6
    madd_f128 %r3, %r3, %r4, %r5
    madd_f128 %r3, %r3, %r4, %r4
    madd_f128 %r3, %r3, %r4, %r3
    madd_f128 %r3, %r3, %r4, %r2
    madd_f128 %r3, %r3, %r4, %r1
    madd_f128 %r3, %r3, %r4, %r1
    mul_f128 %r3, %r3, %r3 ; (e^x) ^ 8
    mul_f128 %r3, %r3, %r3
    mul_f128 %r3, %r3, %r3
    write "exp: %f128(r3)"

    add_f128 %r1, %r2, %r3
    madd_f128 %r2, %r10, %r20, %r30
    madd_f128 %r1, %r11, %r21, %r31

    ; classification
    class_f32 %r4, %r5, 120
    class_f64 %r4, %r5, 120
    class_f128 %r4, %r5, 120
    jmp skipfma

fpu_backward_target:
; single branches
    br_oeq_f32 %r23, %r34, fpu_backward_target
    br_oeq_f32.l %r23, %r34, fpu_backward_target
    br_oeq_f32 %r23, %r34, fpu_forward_target
    br_oeq_f32.l %r23, %r34, fpu_forward_target

    br_ueq_f32 %r23, %r34, fpu_backward_target
    br_ueq_f32.l %r23, %r34, fpu_backward_target
    br_ueq_f32 %r23, %r34, fpu_forward_target
    br_ueq_f32.l %r23, %r34, fpu_forward_target

    br_one_f32 %r23, %r34, fpu_backward_target
    br_one_f32.l %r23, %r34, fpu_backward_target
    br_one_f32 %r23, %r34, fpu_forward_target
    br_one_f32.l %r23, %r34, fpu_forward_target

    br_une_f32 %r23, %r34, fpu_backward_target
    br_une_f32.l %r23, %r34, fpu_backward_target
    br_une_f32 %r23, %r34, fpu_forward_target
    br_une_f32.l %r23, %r34, fpu_forward_target

    br_olt_f32 %r23, %r34, fpu_backward_target
    br_olt_f32.l %r23, %r34, fpu_backward_target
    br_olt_f32 %r23, %r34, fpu_forward_target
    br_olt_f32.l %r23, %r34, fpu_forward_target

    br_ult_f32 %r23, %r34, fpu_backward_target
    br_ult_f32.l %r23, %r34, fpu_backward_target
    br_ult_f32 %r23, %r34, fpu_forward_target
    br_ult_f32.l %r23, %r34, fpu_forward_target

    br_oge_f32 %r23, %r34, fpu_backward_target
    br_oge_f32.l %r23, %r34, fpu_backward_target
    br_oge_f32 %r23, %r34, fpu_forward_target
    br_oge_f32.l %r23, %r34, fpu_forward_target

    br_uge_f32 %r23, %r34, fpu_backward_target
    br_uge_f32.l %r23, %r34, fpu_backward_target
    br_uge_f32 %r23, %r34, fpu_forward_target
    br_uge_f32.l %r23, %r34, fpu_forward_target

    br_o_f32 %r23, %r34, fpu_backward_target
    br_o_f32.l %r23, %r34, fpu_backward_target
    br_o_f32 %r23, %r34, fpu_forward_target
    br_o_f32.l %r23, %r34, fpu_forward_target

    br_u_f32 %r23, %r34, fpu_backward_target
    br_u_f32.l %r23, %r34, fpu_backward_target
    br_u_f32 %r23, %r34, fpu_forward_target
    br_u_f32.l %r23, %r34, fpu_forward_target

; double branches
    br_oeq_f64 %r23, %r34, fpu_backward_target
    br_oeq_f64.l %r23, %r34, fpu_backward_target
    br_oeq_f64 %r23, %r34, fpu_forward_target
    br_oeq_f64.l %r23, %r34, fpu_forward_target

    br_ueq_f64 %r23, %r34, fpu_backward_target
    br_ueq_f64.l %r23, %r34, fpu_backward_target
    br_ueq_f64 %r23, %r34, fpu_forward_target
    br_ueq_f64.l %r23, %r34, fpu_forward_target

    br_one_f64 %r23, %r34, fpu_backward_target
    br_one_f64.l %r23, %r34, fpu_backward_target
    br_one_f64 %r23, %r34, fpu_forward_target
    br_one_f64.l %r23, %r34, fpu_forward_target

    br_une_f64 %r23, %r34, fpu_backward_target
    br_une_f64.l %r23, %r34, fpu_backward_target
    br_une_f64 %r23, %r34, fpu_forward_target
    br_une_f64.l %r23, %r34, fpu_forward_target

    br_olt_f64 %r23, %r34, fpu_backward_target
    br_olt_f64.l %r23, %r34, fpu_backward_target
    br_olt_f64 %r23, %r34, fpu_forward_target
    br_olt_f64.l %r23, %r34, fpu_forward_target

    br_ult_f64 %r23, %r34, fpu_backward_target
    br_ult_f64.l %r23, %r34, fpu_backward_target
    br_ult_f64 %r23, %r34, fpu_forward_target
    br_ult_f64.l %r23, %r34, fpu_forward_target

    br_oge_f64 %r23, %r34, fpu_backward_target
    br_oge_f64.l %r23, %r34, fpu_backward_target
    br_oge_f64 %r23, %r34, fpu_forward_target
    br_oge_f64.l %r23, %r34, fpu_forward_target

    br_uge_f64 %r23, %r34, fpu_backward_target
    br_uge_f64.l %r23, %r34, fpu_backward_target
    br_uge_f64 %r23, %r34, fpu_forward_target
    br_uge_f64.l %r23, %r34, fpu_forward_target

    br_o_f64 %r23, %r34, fpu_backward_target
    br_o_f64.l %r23, %r34, fpu_backward_target
    br_o_f64 %r23, %r34, fpu_forward_target
    br_o_f64.l %r23, %r34, fpu_forward_target

    br_u_f64 %r23, %r34, fpu_backward_target
    br_u_f64.l %r23, %r34, fpu_backward_target
    br_u_f64 %r23, %r34, fpu_forward_target
    br_u_f64.l %r23, %r34, fpu_forward_target

; quadruple branches
    br_oeq_f128 %r23, %r34, fpu_backward_target
    br_oeq_f128.l %r23, %r34, fpu_backward_target
    br_oeq_f128 %r23, %r34, fpu_forward_target
    br_oeq_f128.l %r23, %r34, fpu_forward_target

    br_ueq_f128 %r23, %r34, fpu_backward_target
    br_ueq_f128.l %r23, %r34, fpu_backward_target
    br_ueq_f128 %r23, %r34, fpu_forward_target
    br_ueq_f128.l %r23, %r34, fpu_forward_target

    br_one_f128 %r23, %r34, fpu_backward_target
    br_one_f128.l %r23, %r34, fpu_backward_target
    br_one_f128 %r23, %r34, fpu_forward_target
    br_one_f128.l %r23, %r34, fpu_forward_target

    br_une_f128 %r23, %r34, fpu_backward_target
    br_une_f128.l %r23, %r34, fpu_backward_target
    br_une_f128 %r23, %r34, fpu_forward_target
    br_une_f128.l %r23, %r34, fpu_forward_target

    br_olt_f128 %r23, %r34, fpu_backward_target
    br_olt_f128.l %r23, %r34, fpu_backward_target
    br_olt_f128 %r23, %r34, fpu_forward_target
    br_olt_f128.l %r23, %r34, fpu_forward_target

    br_ult_f128 %r23, %r34, fpu_backward_target
    br_ult_f128.l %r23, %r34, fpu_backward_target
    br_ult_f128 %r23, %r34, fpu_forward_target
    br_ult_f128.l %r23, %r34, fpu_forward_target

    br_oge_f128 %r23, %r34, fpu_backward_target
    br_oge_f128.l %r23, %r34, fpu_backward_target
    br_oge_f128 %r23, %r34, fpu_forward_target
    br_oge_f128.l %r23, %r34, fpu_forward_target

    br_uge_f128 %r23, %r34, fpu_backward_target
    br_uge_f128.l %r23, %r34, fpu_backward_target
    br_uge_f128 %r23, %r34, fpu_forward_target
    br_uge_f128.l %r23, %r34, fpu_forward_target

    br_o_f128 %r23, %r34, fpu_backward_target
    br_o_f128.l %r23, %r34, fpu_backward_target
    br_o_f128 %r23, %r34, fpu_forward_target
    br_o_f128.l %r23, %r34, fpu_forward_target

    br_u_f128    %r23, %r34, fpu_backward_target
    br_u_f128.l  %r23, %r34, fpu_backward_target
    br_u_f128    %r23, %r34, fpu_forward_target
    br_u_f128.l  %r23, %r34, fpu_forward_target

fpu_forward_target:

    nul_une_f32  %r23, %r34, 1, 1
    nul_une_f64  %r23, %r34, 1, 1
    nul_une_f128 %r23, %r34, 1, 1

    nul_one_f32  %r23, %r34, 1, 1
    nul_one_f64  %r23, %r34, 1, 1
    nul_one_f128 %r23, %r34, 1, 1

    nul_ueq_f32  %r23, %r34, 1, 1
    nul_ueq_f64  %r23, %r34, 1, 1
    nul_ueq_f128 %r23, %r34, 1, 1

    nul_oeq_f32  %r23, %r34, 1, 1
    nul_oeq_f64  %r23, %r34, 1, 1
    nul_oeq_f128 %r23, %r34, 1, 1

skipfma:
    write "end fpu"
.end
