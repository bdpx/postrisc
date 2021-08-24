.text
    alloc 96
    write "test base addressing with indexed post-update"
    ld_imm %r12, 1
    add_imm_i64 %r45, %sp, -512

    ld_mia_u8  %r23, %r45, 2
    ld_mia_u16  %r23, %r45, 2
    ld_mia_u32  %r23, %r45, 4
    ld_mia_u64  %r23, %r45, 8
    ld_mia_i128 %r23, %r45, 16

    ld_mia_i8  %r23, %r45, 2
    ld_mia_i16 %r23, %r45, 2
    ld_mia_i32 %r23, %r45, 4
    ld_mia_i64 %r23, %r45, 8

    st_mia_i8   %r23, %r45, 2
    st_mia_i16  %r23, %r45, 2
    st_mia_i32  %r23, %r45, 4
    st_mia_i64  %r23, %r45, 8
    st_mia_i128 %r23, %r45, 16
    write "end_indexed_modify_test"
.end
