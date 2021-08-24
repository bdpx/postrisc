.text
    alloc 96
    write "test atomic fetch-op"
    add_imm_i64 %r5, %sp, -64
    write "atomic base: %x64(r5)"
    ld_imm  %r10, 5
    ld_imm  %r12, 10
    ld_imm  %r56, 5

    write "test amo-add"

    amx_ld_add_u8 %r4, %r5, %r10
    amq_ld_add_u8 %r4, %r5, %r10
    amr_ld_add_u8 %r4, %r5, %r10
    amz_ld_add_u8 %r4, %r5, %r10

    amx_ld_add_u16  %r4, %r5, %r10
    amq_ld_add_u16 %r4, %r5, %r10
    amr_ld_add_u16 %r4, %r5, %r10
    amz_ld_add_u16 %r4, %r5, %r10

    amx_ld_add_u32 %r4, %r5, %r10
    amq_ld_add_u32 %r4, %r5, %r10
    amr_ld_add_u32 %r4, %r5, %r10
    amz_ld_add_u32 %r4, %r5, %r10

    amx_ld_add_u64 %r4, %r5, %r10
    amq_ld_add_u64 %r4, %r5, %r10
    amr_ld_add_u64 %r4, %r5, %r10
    amz_ld_add_u64 %r4, %r5, %r10

    amx_ld_add_u128 %r4, %r5, %r10
    amq_ld_add_u128 %r4, %r5, %r10
    amr_ld_add_u128 %r4, %r5, %r10
    amz_ld_add_u128 %r4, %r5, %r10

    write "test amo-and"

    amx_ld_and_u8 %r4, %r5, %r10
    amq_ld_and_u8 %r4, %r5, %r10
    amr_ld_and_u8 %r4, %r5, %r10
    amz_ld_and_u8 %r4, %r5, %r10

    amx_ld_and_u16 %r4, %r5, %r10
    amq_ld_and_u16 %r4, %r5, %r10
    amr_ld_and_u16 %r4, %r5, %r10
    amz_ld_and_u16 %r4, %r5, %r10

    amx_ld_and_u32 %r4, %r5, %r10
    amq_ld_and_u32 %r4, %r5, %r10
    amr_ld_and_u32 %r4, %r5, %r10
    amz_ld_and_u32 %r4, %r5, %r10

    amx_ld_and_u64 %r4, %r5, %r10
    amq_ld_and_u64 %r4, %r5, %r10
    amr_ld_and_u64 %r4, %r5, %r10
    amz_ld_and_u64 %r4, %r5, %r10

    amx_ld_and_u128 %r4, %r5, %r10
    amq_ld_and_u128 %r4, %r5, %r10
    amr_ld_and_u128 %r4, %r5, %r10
    amz_ld_and_u128 %r4, %r5, %r10

    write "test amo-or"

    amx_ld_or_u8 %r4, %r5, %r10
    amq_ld_or_u8 %r4, %r5, %r10
    amr_ld_or_u8 %r4, %r5, %r10
    amz_ld_or_u8 %r4, %r5, %r10

    amx_ld_or_u16 %r4, %r5, %r10
    amq_ld_or_u16 %r4, %r5, %r10
    amr_ld_or_u16 %r4, %r5, %r10
    amz_ld_or_u16 %r4, %r5, %r10

    amx_ld_or_u32 %r4, %r5, %r10
    amq_ld_or_u32 %r4, %r5, %r10
    amr_ld_or_u32 %r4, %r5, %r10
    amz_ld_or_u32 %r4, %r5, %r10

    amx_ld_or_u64 %r4, %r5, %r10
    amq_ld_or_u64 %r4, %r5, %r10
    amr_ld_or_u64 %r4, %r5, %r10
    amz_ld_or_u64 %r4, %r5, %r10

    amx_ld_or_u128 %r4, %r5, %r10
    amq_ld_or_u128 %r4, %r5, %r10
    amr_ld_or_u128 %r4, %r5, %r10
    amz_ld_or_u128 %r4, %r5, %r10

    write "test amo-xor"

    amx_ld_xor_u8 %r4, %r5, %r10
    amq_ld_xor_u8 %r4, %r5, %r10
    amr_ld_xor_u8 %r4, %r5, %r10
    amz_ld_xor_u8 %r4, %r5, %r10

    amx_ld_xor_u16 %r4, %r5, %r10
    amq_ld_xor_u16 %r4, %r5, %r10
    amr_ld_xor_u16 %r4, %r5, %r10
    amz_ld_xor_u16 %r4, %r5, %r10

    amx_ld_xor_u32 %r4, %r5, %r10
    amq_ld_xor_u32 %r4, %r5, %r10
    amr_ld_xor_u32 %r4, %r5, %r10
    amz_ld_xor_u32 %r4, %r5, %r10

    amx_ld_xor_u64 %r4, %r5, %r10
    amq_ld_xor_u64 %r4, %r5, %r10
    amr_ld_xor_u64 %r4, %r5, %r10
    amz_ld_xor_u64 %r4, %r5, %r10

    amx_ld_xor_u128 %r4, %r5, %r10
    amq_ld_xor_u128 %r4, %r5, %r10
    amr_ld_xor_u128 %r4, %r5, %r10
    amz_ld_xor_u128 %r4, %r5, %r10

    write "test amo-smin"
    amx_ld_min_i8     %r4, %r5, %r10
    amq_ld_min_i8     %r4, %r5, %r10
    amr_ld_min_i8     %r4, %r5, %r10
    amz_ld_min_i8    %r4, %r5, %r10

    amx_ld_min_i16   %r4, %r5, %r10
    amq_ld_min_i16   %r4, %r5, %r10
    amr_ld_min_i16   %r4, %r5, %r10
    amz_ld_min_i16  %r4, %r5, %r10

    amx_ld_min_i32    %r4, %r5, %r10
    amq_ld_min_i32    %r4, %r5, %r10
    amr_ld_min_i32    %r4, %r5, %r10
    amz_ld_min_i32   %r4, %r5, %r10

    amx_ld_min_i64   %r4, %r5, %r10
    amq_ld_min_i64   %r4, %r5, %r10
    amr_ld_min_i64   %r4, %r5, %r10
    amz_ld_min_i64  %r4, %r5, %r10

    amx_ld_min_i128   %r4, %r5, %r10
    amq_ld_min_i128   %r4, %r5, %r10
    amr_ld_min_i128   %r4, %r5, %r10
    amz_ld_min_i128  %r4, %r5, %r10

    write "test amo-smax"
    amx_ld_max_i8   %r4, %r5, %r10
    amq_ld_max_i8   %r4, %r5, %r10
    amr_ld_max_i8   %r4, %r5, %r10
    amz_ld_max_i8  %r4, %r5, %r10

    amx_ld_max_i16   %r4, %r5, %r10
    amq_ld_max_i16   %r4, %r5, %r10
    amr_ld_max_i16   %r4, %r5, %r10
    amz_ld_max_i16  %r4, %r5, %r10

    amx_ld_max_i32   %r4, %r5, %r10
    amq_ld_max_i32   %r4, %r5, %r10
    amr_ld_max_i32   %r4, %r5, %r10
    amz_ld_max_i32  %r4, %r5, %r10

    amx_ld_max_i64   %r4, %r5, %r10
    amq_ld_max_i64   %r4, %r5, %r10
    amr_ld_max_i64   %r4, %r5, %r10
    amz_ld_max_i64  %r4, %r5, %r10

    amx_ld_max_i128   %r4, %r5, %r10
    amq_ld_max_i128   %r4, %r5, %r10
    amr_ld_max_i128   %r4, %r5, %r10
    amz_ld_max_i128  %r4, %r5, %r10

    write "test amo-umin"
    amx_ld_min_u8   %r4, %r5, %r10
    amq_ld_min_u8   %r4, %r5, %r10
    amr_ld_min_u8   %r4, %r5, %r10
    amz_ld_min_u8  %r4, %r5, %r10

    amx_ld_min_u16   %r4, %r5, %r10
    amq_ld_min_u16   %r4, %r5, %r10
    amr_ld_min_u16   %r4, %r5, %r10
    amz_ld_min_u16  %r4, %r5, %r10

    amx_ld_min_u32  %r4, %r5, %r10
    amq_ld_min_u32  %r4, %r5, %r10
    amr_ld_min_u32  %r4, %r5, %r10
    amz_ld_min_u32 %r4, %r5, %r10

    amx_ld_min_u64   %r4, %r5, %r10
    amq_ld_min_u64   %r4, %r5, %r10
    amr_ld_min_u64   %r4, %r5, %r10
    amz_ld_min_u64  %r4, %r5, %r10

    amx_ld_min_u128   %r4, %r5, %r10
    amq_ld_min_u128   %r4, %r5, %r10
    amr_ld_min_u128   %r4, %r5, %r10
    amz_ld_min_u128  %r4, %r5, %r10

    write "test amo-umax"
    amx_ld_max_u8    %r4, %r5, %r10
    amq_ld_max_u8    %r4, %r5, %r10
    amr_ld_max_u8    %r4, %r5, %r10
    amz_ld_max_u8   %r4, %r5, %r10

    amx_ld_max_u16    %r4, %r5, %r10
    amq_ld_max_u16    %r4, %r5, %r10
    amr_ld_max_u16    %r4, %r5, %r10
    amz_ld_max_u16   %r4, %r5, %r10

    amx_ld_max_u32    %r4, %r5, %r10
    amq_ld_max_u32    %r4, %r5, %r10
    amr_ld_max_u32    %r4, %r5, %r10
    amz_ld_max_u32   %r4, %r5, %r10

    amx_ld_max_u64    %r4, %r5, %r10
    amq_ld_max_u64    %r4, %r5, %r10
    amr_ld_max_u64    %r4, %r5, %r10
    amz_ld_max_u64   %r4, %r5, %r10

    amx_ld_max_u128   %r4, %r5, %r10
    amq_ld_max_u128   %r4, %r5, %r10
    amr_ld_max_u128   %r4, %r5, %r10
    amz_ld_max_u128  %r4, %r5, %r10

    write "test cas"

    amx_cas_u8    %r12, %r5, %r56, %r34
    amq_cas_u8    %r12, %r5, %r56, %r34
    amr_cas_u8    %r12, %r5, %r56, %r34
    amz_cas_u8   %r12, %r5, %r56, %r34

    amx_cas_u16   %r12, %r5, %r56, %r34
    amq_cas_u16   %r12, %r5, %r56, %r34
    amr_cas_u16   %r12, %r5, %r56, %r34
    amz_cas_u16  %r12, %r5, %r56, %r34

    amx_cas_u32   %r12, %r5, %r56, %r34
    amq_cas_u32   %r12, %r5, %r56, %r34
    amr_cas_u32   %r12, %r5, %r56, %r34
    amz_cas_u32  %r12, %r5, %r56, %r34

    amx_cas_u64   %r12, %r5, %r56, %r34
    amq_cas_u64   %r12, %r5, %r56, %r34
    amr_cas_u64   %r12, %r5, %r56, %r34
    amz_cas_u64  %r12, %r5, %r56, %r34

    amx_cas_u128  %r12, %r5, %r56, %r34
    amq_cas_u128  %r12, %r5, %r56, %r34
    amr_cas_u128  %r12, %r5, %r56, %r34
    amz_cas_u128 %r12, %r5, %r56, %r34

    write "test load atomic relaxed"
    amx_ld_u8    %r12, %r5
    amx_ld_u16   %r12, %r5
    amx_ld_u32   %r12, %r5
    amx_ld_u64   %r12, %r5
    amx_ld_u128  %r12, %r5

    write "test load atomic acquire"
    amq_ld_u8    %r12, %r5
    amq_ld_u16   %r12, %r5
    amq_ld_u32   %r12, %r5
    amq_ld_u64   %r12, %r5
    amq_ld_u128  %r12, %r5

    write "test store atomic relaxed"
    amx_st_u8    %r12, %r5
    amx_st_u16   %r12, %r5
    amx_st_u32   %r12, %r5
    amx_st_u64   %r12, %r5
    amx_st_u128  %r12, %r5

    write "test store atomic release"
    amr_st_u8    %r12, %r5
    amr_st_u16   %r12, %r5
    amr_st_u32   %r12, %r5
    amr_st_u64   %r12, %r5
    amr_st_u128  %r12, %r5

.end
