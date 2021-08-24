.text
backward_target:
    alloc 61
    add_imm_i64 %r2, %r2, 1
    br_eq_i64 %r2, %r2, branch_test_exit

    br_eq_i64	%r23, %r34, backward_target
    br_eq_i64.l	%r23, %r34, backward_target
    br_eq_i64	%r23, %r34, forward_target
    br_eq_i64.l	%r23, %r34, forward_target
    br_eq_imm_i64	%r23,34, backward_target
    br_eq_imm_i64.l	%r23,34, backward_target
    br_eq_imm_i64	%r23,34, forward_target
    br_eq_imm_i64.l	%r23,34, forward_target

    br_eq_i32	%r23, %r34, backward_target
    br_eq_i32.l	%r23, %r34, backward_target
    br_eq_i32	%r23, %r34, forward_target
    br_eq_i32.l	%r23, %r34, forward_target
    br_eq_imm_i32	%r23,34, backward_target
    br_eq_imm_i32.l	%r23,34, backward_target
    br_eq_imm_i32	%r23,34, forward_target
    br_eq_imm_i32.l	%r23,34, forward_target

    br_ne_i64	%r23, %r34, backward_target
    br_ne_i64.l	%r23, %r34, backward_target
    br_ne_i64	%r23, %r34, forward_target
    br_ne_i64.l	%r23, %r34, forward_target
    br_ne_imm_i64	%r23,34, backward_target
    br_ne_imm_i64.l	%r23,34, backward_target
    br_ne_imm_i64	%r23,34, forward_target
    br_ne_imm_i64.l	%r23,34, forward_target

    br_ne_i32	%r23, %r34, backward_target
    br_ne_i32.l	%r23, %r34, backward_target
    br_ne_i32	%r23, %r34, forward_target
    br_ne_i32.l	%r23, %r34, forward_target
    br_ne_imm_i32	%r23,34, backward_target
    br_ne_imm_i32.l	%r23,34, backward_target
    br_ne_imm_i32	%r23,34, forward_target
    br_ne_imm_i32.l	%r23,34, forward_target

    br_ge_i64	%r23, %r34, backward_target
    br_ge_i64.l	%r23, %r34, backward_target
    br_ge_i64	%r23, %r34, forward_target
    br_ge_i64.l	%r23, %r34, forward_target

    br_ge_i32	%r23, %r34, backward_target
    br_ge_i32.l	%r23, %r34, backward_target
    br_ge_i32	%r23, %r34, forward_target
    br_ge_i32.l	%r23, %r34, forward_target

    br_lt_i64	%r23, %r34, backward_target
    br_lt_i64.l	%r23, %r34, backward_target
    br_lt_i64	%r23, %r34, forward_target
    br_lt_i64.l	%r23, %r34, forward_target
    br_lt_imm_i64	%r23,34, backward_target
    br_lt_imm_i64.l	%r23,34, backward_target
    br_lt_imm_i64	%r23,34, forward_target
    br_lt_imm_i64.l	%r23,34, forward_target

    br_lt_i32	%r23, %r34, backward_target
    br_lt_i32.l	%r23, %r34, backward_target
    br_lt_i32	%r23, %r34, forward_target
    br_lt_i32.l	%r23, %r34, forward_target
    br_lt_imm_i32	%r23,34, backward_target
    br_lt_imm_i32.l	%r23,34, backward_target
    br_lt_imm_i32	%r23,34, forward_target
    br_lt_imm_i32.l	%r23,34, forward_target

    br_ge_imm_u64	%r23,34, backward_target
    br_ge_imm_u64.l	%r23,34, backward_target
    br_ge_imm_u64	%r23,34, forward_target
    br_ge_imm_u64.l	%r23,34, forward_target

    br_ge_imm_u32	%r23,34, backward_target
    br_ge_imm_u32.l	%r23,34, backward_target
    br_ge_imm_u32	%r23,34, forward_target
    br_ge_imm_u32.l	%r23,34, forward_target

    br_ge_u64	%r23, %r34, backward_target
    br_ge_u64.l	%r23, %r34, backward_target
    br_ge_u64	%r23, %r34, forward_target
    br_ge_u64.l	%r23, %r34, forward_target

    br_ge_u32	%r23, %r34, backward_target
    br_ge_u32.l	%r23, %r34, backward_target
    br_ge_u32	%r23, %r34, forward_target
    br_ge_u32.l	%r23, %r34, forward_target

    br_lt_u64	%r23, %r34, backward_target
    br_lt_u64.l	%r23, %r34, backward_target
    br_lt_u64	%r23, %r34, forward_target
    br_lt_u64.l	%r23, %r34, forward_target

    br_lt_u32	%r23, %r34, backward_target
    br_lt_u32.l	%r23, %r34, backward_target
    br_lt_u32	%r23, %r34, forward_target
    br_lt_u32.l	%r23, %r34, forward_target

    br_ge_imm_u64	%r23,34, backward_target
    br_ge_imm_u64.l	%r23,34, backward_target
    br_ge_imm_u64	%r23,34, forward_target
    br_ge_imm_u64.l	%r23,34, forward_target

    br_ge_imm_u32	%r23,34, backward_target
    br_ge_imm_u32.l	%r23,34, backward_target
    br_ge_imm_u32	%r23,34, forward_target
    br_ge_imm_u32.l	%r23,34, forward_target

    br_mask_all	%r23, 34, backward_target
    br_mask_all.l	%r23, 34, backward_target
    br_mask_all	%r23, 34, forward_target
    br_mask_all.l	%r23, 34, forward_target

    br_mask_notall	%r23, 34, backward_target
    br_mask_notall.l	%r23, 34, backward_target
    br_mask_notall	%r23, 34, forward_target
    br_mask_notall.l	%r23, 34, forward_target

    br_mask_any   %r23, 34, backward_target
    br_mask_any.l %r23, 34, backward_target
    br_mask_any   %r23, 34, forward_target
    br_mask_any.l %r23, 34, forward_target

    br_mask_none   %r23, 34, backward_target
    br_mask_none.l %r23, 34, backward_target
    br_mask_none   %r23, 34, forward_target
    br_mask_none.l %r23, 34, forward_target

forward_target:
branch_test_exit:

    jmp branch_exit

label:
    br_eq_i64 %r12, %r13, qwe
    srp_imm_i64 %r10, %r11, %r12, 45
    deposit_r %r61, %r91, %r32, %r10
    mbsel %r62, %r91, %r32, %r10
    perm %r63, %r91, %r32, %r10
qwe:
    br_ne_i64 %r15, %r46, label
    br_eq_i64 %r25, %r45, label
    br_lt_i64 %r25, %r44, label
    br_ge_i64 %r35, %r43, label
    br_lt_u64 %r55, %r76, label
    br_ne_imm_i64 %r55, 140, label
    br_eq_imm_i64 %r65, 141, label
    br_lt_imm_i64 %r65, 142, label
    br_lt_imm_u64 %r75, 170, label
    br_ge_imm_u64 %r85, 160, label

    add_imm_i64.l %r45, %r34, 1234
    br_bs_imm %r85, 26, label
    br_bc_imm.l %r85, 36, label
    br_bs_imm %r95, 46, label
    br_bc_imm.l %r95, 56, label

    jmp_r	%r45, %r23, 1
branch_exit:
    write	"end branch test"
.end
