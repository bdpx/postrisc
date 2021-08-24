.rodata
align 4
table_cases_d4:
    d4	0 ;(label_0 - table_pos) / 16
    d4	3 ;(label_1 - table_pos) / 16
    d4	7 ;(label_2 - table_pos) / 16

align 2
table_cases_d2:
    d2	0 ;(label_0 - table_pos) / 16
    d2	3 ;(label_1 - table_pos) / 16
    d2	7 ;(label_2 - table_pos) / 16

.text
    alloc	80
    write	"test table switch to case 1"
    ldi		%r4, 1
;   ldafr	%r5, table_cases_d4
;   jmptw	%r5, %r4
got_next_to_one_bundle:
    ldafr	%r5, table_cases_d2
    jmpth	%r5, %r4
table_pos:

lable_0:
    write	"case 0"
    cmpqeq	%r12, %r24, %gz
    cmpqne	%r12, %r24, %gz
    deps	%r18, %r20, 13, 32
    depc	%r19, %r23, 13, 32
    ldi		%r12, -1234
    ldi		%r13, 3456
    jmp		label_after_switch
label_1:
    write	"case 1"
    andi	%r45, %r44, 12345
    sladd	%r14, %sp, %r12, 2
    sladd	%r12, %r23, %r44, 3
    mov		%r12, %r13
    ldi		%r24, 0
    mtspr	%r24, %psr
    mfspr	%r12, %psr
    nand	%r34, %r34, %r45
    sll		%r12, %r23, %r45
    slli	%r12, %r23, 45
    jmp		label_after_switch
label_2:
    write	"case 2"
    addi	%r34, %r34,-1
    mov		%r58, %r45
    sladd	%r12, %r15, %r30, 14
    sladd	%r12, %r15, %r30, 5
    sladd	%r12, %r15, %r30, 5
    srd		%r34, %r56, %r40
    srdi	%r34, %r56, 40
    depa	%r40, %r78, 40, 20
    sladd	%r54, %r45, %r22, 4
    sladd	%r54, %r45, %r22, 20
    ldax	%r3, %r45, %tp, 3, 55
    jmp		label_after_switch

label_after_switch:
    write	"end table switch test"
.end
