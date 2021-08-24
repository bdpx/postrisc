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
    ldi %r4, 1
    ldard %r5, table_cases
    jmpt %r5, %r4

label_0:
    write "case 0"
    cmpeqq  %r12, %r24, %gz
    cmpneq  %r12, %r24, %gz
    depositv %r18, %r20, %r13, %r32
    deposit %r19, %r23, %r12, 13, 32
    ldi %r12, -1234
    ldi %r13, 3456
    jmp  label_after_switch

label_1:
    write "case 1"
    andi %r45, %r44, 12345
    sladdd %r14, %sp, %r12, 2
    sladdd %r12, %r23, %r44, 3
    mov %r12, %r13
    ldi %r24, 0
    setspr %r24, %psr
    getspr %r12, %psr
    nand %r34, %r34, %r45
    slld %r12, %r23, %r45
    sllid %r12, %r23, 45
    jmp label_after_switch

label_2:
    write   "case 2"
    addid  %r34, %r34,-1
    mov     %r58, %r45
    sladdd  %r12, %r15, %r30, 14
    sladdd  %r12, %r15, %r30, 5
    sladdd  %r12, %r15, %r30, 5
    divp2d     %r34, %r56, %r40
    divp2id    %r34, %r56, 40
    depositv   %r40, %r78, %r40, %r20
    sladdd  %r54, %r45, %r22, 4
    sladdd  %r54, %r45, %r22, 20
    ldaxsd  %r3, %r45, %tp, 3, 55
    jmp  label_after_switch

label_after_switch:
    write "end table switch test"
.end
