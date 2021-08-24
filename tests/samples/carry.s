.text
    write "example of carry/borrow testing"
    alloc 96

; 256-bit add (g30,%r31,r32,r33) + (g40,r41,r42,r43) => (g50,r51,r52,r53)
    ld_imm  %r30, -1
    ld_imm  %r31, -1
    ld_imm  %r34, -1
    ld_imm  %r33, -1

    ld_imm  %r40, 1
    ld_imm  %r41, 0
    ld_imm  %r42, 0
    ld_imm  %r43, 0

; throw add
    cmp_eq_i64     %r10, %r30, %r40 ; add carry out
    add_i64      %r50, %r30, %r40 ; add
    cmp_eq_imm_i64    %r12, %r31, 1
    add_imm_i64   %r51, %r31, 1

    cmp_eq_i64 %r12, %r31, %r41 ; add carry out
    add_i64  %r51, %r31, %r41 ; add
    cmp_eq_i64 %r14, %r34, %r42 ; add carry out
    add_i64  %r52, %r34, %r42 ; add
    cmp_eq_i64 %r8, %r33, %r43 ; add carry out
    add_i64 %r53, %r33, %r43 ; add
    write "add carryis"
    add_imm_i64 %r51, %r51, 1
    add_imm_i64 %r52, %r52, 1
    add_imm_i64 %r53, %r53, 1
; set last carry
    ld_imm  %r54, 1
    ld_imm  %r54, 0
    write "multiprecision add:\nr50,r51,r52,r53,r54 = %x64(r50) %x64(r51) %x64(r52) %x64(r53) %x64(r54)"

    ld_imm.l %r40, 0x7fffffffffffffff
    mul_h %r40, %r40, %r41
    write "r40     %x64(r40)"

    ld_imm   %r12, 12345
    ld_imm.l %r12, 12345678900

; ld_imm %r14, 0xFFFFFFFFF0
; ld8 %r13, %r14, 0

    addc_u64 %r12, %r14, %r46
    addc_u64 %r12, %r14, %r46
    subb_u64 %r12, %r14, %r46
    subb_u64 %r12, %r14, %r46
    add_addc_u64 %r12, %r14, %r46, %r23
    add_addc_u64 %r12, %r14, %r46, %r22
    sub_subb_u64 %r12, %r14, %r46, %r13
    sub_subb_u64 %r12, %r14, %r46, %r14
    write "end carry test"
    nop 11111
.end
