.text
.data
data_lbl:
    d1 25
    d1 26
    d1 27
    d1 28

.text
program_start:
; Here we test references to data section.
; Absolute offset from begin of section
    write "base addressing"
    alloc 96
    lda_r %r17, program_start
    ld_imm %r12, data_lbl
    write "data_lbl: %i64(r12)"

    ld_imm %r12, data_hi(data_lbl)
    write "data_hi(data_lbl): %i64(r12)"
    ld_imm %r12, data_lo(data_lbl)
    write "data_lo(data_lbl): %i64(r12)"
    lda_iprel %r13, data_lbl
    write "ca.rf(data_lbl): %x64(r13)"
    lda_iprel.l %r13, data_lbl
    write "ca.rf(data_lbl): %x64(r13)"

    add_imm_i64 %r13, %r17, data_hi(data_lbl)
    write "r13     %i64(r13)"
    add_imm_i64 %r14, %r13, data_lo(data_lbl)+0
    write "r14     %i64(r14)"

    add_imm_i64 %r13, %r17, data_hi(data_lbl)
    write "r13     %i64(r13)"
    ld_u8 %r25, %r13, data_lo(data_lbl)+0
    ld_u8 %r26, %r13, data_lo(data_lbl)+1
    ld_u8 %r27, %r13, data_lo(data_lbl)+2
    ld_u8 %r28, %r13, data_lo(data_lbl)+3
    write "r25     %i64(r25)" ; must be 25
    write "r26     %i64(r26)" ; must be 26
    write "r27     %i64(r27)" ; must be 27
    write "r28     %i64(r28)" ; must be 28

; test load context
    ld_u64 %r1, %sp, -16
    st_i64 %r1, %sp, -16
    jmp skipaddr
    jmp.l skipaddr

; test indexed load/store
    st_xi64_i8  %r12, %r15, %r30, 4, 14
    st_xi64_i16 %r12, %r15, %r30, 4, 14
    st_xi64_i32 %r12, %r15, %r30, 4, 14
    st_xi64_i64 %r12, %r15, %r30, 4, 14

    amx_ld_u128 %r30, %r56
    amx_st_u128 %r43, %r56

    sl_add_i64 %r43, %r56, %r23, 4
    sl_sub_i64 %r43, %r56, %r23, 42
    sl_subr_i64 %r43, %r56, %r23, 12

    ld_u32 %r30, %r5, 66*4 ; load mid
    ld_xi64_u64 %r40, %tp, %r30, 0, 4 ; load base

    ld_xi64_i64 %r12, %r23, %r40, 3, 54
    ld_xi64_i64 %r12, %r23, %r40, 3, 54
    ld_xi64_u64 %r12, %r23, %r40, 3, 54
    ld_xi64_u64 %r12, %r23, %r40, 3, 54
    st_xi64_i32 %r12, %r23, %r40, 3, 54
    st_xi64_i64 %r12, %r23, %r40, 3, 54

    ld_xi64_i8 %r12, %r23, %r40, 3, 54
    ld_xi64_i8 %r12, %r23, %r40, 3, 54
    ld_xi64_u8 %r12, %r23, %r40, 3, 54
    ld_xi64_u8 %r12, %r23, %r40, 3, 54
    st_xi64_i8 %r12, %r23, %r40, 3, 54
    st_xi64_i8 %r12, %r23, %r40, 3, 54

    ld_xi64_i16 %r12, %r23, %r40, 3, 54
    ld_xi64_i16 %r12, %r23, %r40, 3, 54
    ld_xi64_u16 %r12, %r23, %r40, 3, 54
    ld_xi64_u16 %r12, %r23, %r40, 3, 54
    st_xi64_i16 %r12, %r23, %r40, 3, 54
    st_xi64_i16 %r12, %r23, %r40, 3, 54

.text
; LOAD/STORE
    sl_add_i64 %r54, %r56, %r12, 5

    ld_u8 %r16, %r45, 8900
    ld_i8 %r15, %r46, 8900
    ld_xi64_u8 %r54, %r56, %r12, 2, 37
    ld_xi64_i8 %r53, %r65, %r12, 2, 37
    ld_xi64_u8.l %r54, %r56, %r12, 2, 37000000
    ld_xi64_i8.l %r53, %r65, %r12, 2, -37000000
    ld_mia_u8 %r52, %r75, 10
    ld_mia_i8 %r51, %r76, 10
    ld_mib_u8 %r52, %r75, 10
    ld_mib_i8 %r51, %r76, 10
    st_mia_i8 %r51, %r76, 10
    st_mib_i8 %r52, %r75, 10

    ld_u16 %r12, %r45, 8900
    ld_i16 %r12, %r45, 8900
    ld_xi64_u16 %r54, %r56, %r12, 3, -57
    ld_xi64_i16 %r54, %r56, %r12, 2, 37
    ld_xi64_u16.l %r54, %r56, %r12, 2, 37000000
    ld_xi64_i16.l %r53, %r65, %r12, 2, -37000000
    ld_mia_u16 %r54, %r56, 12
    ld_mia_i16 %r54, %r56, -60
    ld_mib_u16 %r54, %r56, 12
    ld_mib_i16 %r54, %r56, -60
    st_mia_i16 %r51, %r76, 10
    st_mib_i16 %r52, %r75, 10

    ld_u32 %r12, %r45, 8900
    ld_i32 %r12, %r45, 8900
    ld_xi64_u32 %r54, %r56, %r12, 2, 7
    ld_xi64_i32 %r54, %r56, %r12, 2, 7
    ld_xi64_u32.l %r54, %r56, %r12, 2, 37000000
    ld_xi64_i32.l %r53, %r65, %r12, 2, -37000000
    ld_mia_u32 %r54, %r56, 12
    ld_mia_i32 %r54, %r56, 32
    ld_mib_u32 %r54, %r56, 12
    ld_mib_i32 %r54, %r56, 32
    st_mia_i32 %r51, %r76, 10
    st_mib_i32 %r52, %r75, 10

    ld_u64   %r54, %r56, 5600
    ld_i64   %r54, %r56, 5600
    ld_u64.l %r53, %r46, 98765432
    ld_u64    %r52, %r45, -5600
    ld_u64.l   %r51, %r55, -98765432
    ld_xi64_u64  %r50, %r56, %r12, 2, 37
    ld_xi64_i64 %r50, %r56, %r12, 2, 37
    ld_xi64_u64.l %r54, %r56, %r12, 2, 37000000
    ld_xi64_i64.l %r53, %r65, %r12, 2, -37000000
    ld_mia_u64 %r57, %r56, -12
    ld_mia_u64 %r57, %r56, -12
    ld_mia_i64 %r57, %r56, -12
    ld_mia_i64 %r57, %r56, -12
    ld_mib_u64 %r57, %r56, -12
    ld_mib_u64 %r57, %r56, -12
    ld_mib_i64 %r57, %r56, -12
    ld_mib_i64 %r57, %r56, -12
    st_mia_i64 %r51, %r76, 10
    st_mib_i64 %r52, %r75, 10

    ld_i128  %r16, %r45, 8900
    ld_i128.l %r16, %r45, 8900000
    ld_i128.l %r16, %r45, -8900000
    ld_xi64_i128 %r54, %r56, %r12, 2, 37
    ld_xi64_i128.l %r54, %r56, %r12, 2, 37000000
    ld_xi64_i128.l %r54, %r56, %r12, 2, -37000000
    ld_mia_i128 %r52, %r75, 10
    ld_mia_i128 %r52, %r75, 10
    ld_mib_i128 %r52, %r75, 10
    ld_mib_i128 %r52, %r75, 10
    st_mia_i128 %r51, %r76, 10
    st_mib_i128 %r52, %r75, 10

    st_i8  %r12, %r45, 8900
    st_i16 %r12, %r45, 8900
    st_i32 %r12, %r45, 8900
    st_i64 %r12, %r45, 890*8

    ld_u64    %r12, %r45, 8048
    st_i64    %r12, %r45, 8064
    ld_xi64_u64 %r12, %r45, %r13, 3, 7
    st_xi64_i64 %r12, %r45, %r13, 3, 7

    ld_u64  %r60, %r55, 56
    ld_u64  %r60, %r56, 56
    ld_u64  %r46, %r55, 120
    st_i64  %r47, %r55, 56

    ld_u64     %r60, %sp, 624
    st_i64     %r60, %sp, 624
    ld_xi64_u64  %r60, %sp, %r12, 3, 28
    st_xi64_i64  %r60, %sp, %r12, 3, 26
    ld_u64     %r56, %r57, 567
    st_i64     %r56, %r57, 567

    ld_u32 %r34, %r12, 900
    ld_u64 %r34, %r12, 900
    st_i32 %r23, %r12, 900
    st_i64 %r23, %r12, 900

    ld_i128 %r34, %r13, 55*16
    st_i128 %r35, %r13, 55*16
    ld_xi64_i128 %r34, %r13, %r45, 3, 60
    st_xi64_i128 %r34, %r13, %r45, 3, 60

skipaddr:
    nop 0
.end
