.text
    write "test compare"
    alloc 96
    ld_imm %r20, 4
    ld_imm %r21, 3
    ld_imm %r22, -4
    ld_imm %r23, -12
    write "test compare instructions"

    cmp_eq_i64 %r12, %r20, %r21
    cmp_lt_i64 %r12, %r20, %r21
    cmp_lt_u64 %r12, %r20, %r21
    cmp_eq_imm_i64 %r12, %r20, 123456
    cmp_lt_imm_i64 %r12, %r20, 123456
    cmp_lt_imm_u64 %r12, %r20, 123456
    cmp_ne_i64 %r12, %r20, %r21
    cmp_ne_imm_i64 %r12, %r20, 123456
    cmp_ge_imm_i64 %r12, %r20, 123456
    cmp_ge_imm_u64 %r12, %r20, 123456
    cmp_ge_i64 %r12, %r20, %r21
    cmp_ge_u64 %r12, %r20, %r21

    cmp_ge_imm_i32 %r12, %r20, 123456
    cmp_ge_imm_u32 %r12, %r20, 123456
    cmp_ge_imm_i64 %r12, %r20, 123456
    cmp_ge_imm_u64 %r12, %r20, 123456

    cmp_eq_i32 %r12, %r20, %r21
    cmp_lt_i32 %r12, %r20, %r21
    cmp_lt_u32 %r12, %r20, %r21
    cmp_eq_imm_i32 %r12, %r20, 123456
    cmp_lt_imm_i32 %r12, %r20, 123456
    cmp_lt_imm_u32 %r12, %r20, 123456
    cmp_ne_i32 %r12, %r20, %r21
    cmp_ne_imm_i32 %r12, %r20, 123456
    cmp_ge_imm_i32 %r12, %r20, 123456
    cmp_ge_imm_u32 %r12, %r20, 123456
    cmp_ge_i32 %r12, %r20, %r21
    cmp_ge_u32 %r12, %r20, %r21

    cmp_lt_i64 %r12, %r20, %r21
    cmp_lt_u64 %r12, %r20, %r21
    cmp_lt_imm_i64 %r12, %r20, 123456
    cmp_lt_imm_u64 %r12, %r20, 123456
    cmp_lt_i64 %r12, %r20, %r21
    cmp_lt_u64 %r12, %r20, %r21
    cmp_ge_imm_i64 %r12, %r20, 123456
    cmp_ge_imm_u64 %r12, %r20, 123456


    cmp_lt_i32 %r12, %r20, %r21
    cmp_lt_u32 %r12, %r20, %r21
    cmp_lt_imm_i32 %r12, %r20, 123456
    cmp_lt_imm_u32 %r12, %r20, 123456
    cmp_lt_i32 %r12, %r20, %r21
    cmp_lt_u32 %r12, %r20, %r21
    cmp_ge_imm_i32 %r12, %r20, 123456
    cmp_ge_imm_u32 %r12, %r20, 123456

; TESTS
    cmp_eq_i64 %r14, %r12, %r45
    cmp_ne_i64 %r14, %r12, %r45

    cmp_eq_i64 %r14, %r45, %r34
    cmp_eq_imm_i64 %r14, %r45, 123
    cmp_eq_imm_i64.l %r14, %r45, 1234567890123
    cmp_lt_imm_i64 %r14, %r45, 123
    cmp_lt_imm_i64.l %r14, %r45, 1234567890123
    cmp_ge_imm_i64 %r14, %r45, 123
    cmp_ge_imm_i64.l %r14, %r45, 1234567890123
    cmp_lt_i64 %r14, %r45, %r34
    cmp_ge_imm_u64 %r14, %r45, 123
    cmp_ge_imm_u64.l %r14, %r45, 1234567890123
    cmp_ge_imm_u64 %r14, %r45, 123
    cmp_ge_imm_u64.l %r14, %r45, 1234567890123
    cmp_lt_u64 %r14, %r45, %r34

    cmp_eq_i64 %r41, %r34, %r56
    cmp_lt_i64 %r66, %r45, %r57
    cmp_eq_imm_i64 %r64, %r56, 0
.end
