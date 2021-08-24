.rodata
align 4
    d4 table_cases
    d4 label_0
    d4 label_1
    d4 label_2

table_cases:
    i4 label_0 - table_cases
    i4 label_1 - table_cases
    i4 label_2 - table_cases

.text
    alloc 80
    write "test table switch to case 1"
    ld_imm %r4, 1
    lda_iprel %r5, table_cases
    jmp_t %r5, %r4

label_0:
    write "case 0"
    cmp_eq_i128  %r12, %r24, %gz
    cmp_ne_i128  %r12, %r24, %gz
    deposit_r %r18, %r20, %r13, %r32
    deposit %r19, %r23, %r12, 13, 32
    ld_imm %r12, -1234
    ld_imm %r13, 3456
    jmp  label_after_switch

label_1:
    write "case 1"
    and_imm %r45, %r44, 12345
    sl_add_i64 %r14, %sp, %r12, 2
    sl_add_i64 %r12, %r23, %r44, 3
    mov %r12, %r13
    ld_imm %r24, 0
    set_spr %r24, %psr
    get_spr %r12, %psr
    nand %r34, %r34, %r45
    sll_u64 %r12, %r23, %r45
    sll_imm_u64 %r12, %r23, 45
    jmp label_after_switch

label_2:
    write   "case 2"
    add_imm_i64  %r34, %r34,-1
    mov     %r58, %r45
    sl_add_i64  %r12, %r15, %r30, 14
    sl_add_i64  %r12, %r15, %r30, 5
    sl_add_i64  %r12, %r15, %r30, 5
    divp2_i64     %r34, %r56, %r40
    divp2_imm_i64    %r34, %r56, 40
    deposit_r   %r40, %r78, %r40, %r20
    sl_add_i64  %r54, %r45, %r22, 4
    sl_add_i64  %r54, %r45, %r22, 20
    lda_xi64  %r3, %r45, %tp, 3, 55
    jmp  label_after_switch

label_after_switch:
    write "end table switch test"
.end
