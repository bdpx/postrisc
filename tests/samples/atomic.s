.text
  alloc 96
  write "test atomic fetch-op"
  add_imm_i64 %r5, %sp, -64
  write "atomic base: %x64(r5)"
  ld_imm %r10, 5
  ld_imm %r12, 10
  ld_imm %r56, 5

  write "test amo-add"

  amo_add_u8 %r4, %r5, %r10, relaxed
  amo_add_u8 %r4, %r5, %r10, acquire
  amo_add_u8 %r4, %r5, %r10, release
  amo_add_u8 %r4, %r5, %r10, acq_rel

  amo_add_u16 %r4, %r5, %r10, relaxed
  amo_add_u16 %r4, %r5, %r10, acquire
  amo_add_u16 %r4, %r5, %r10, release
  amo_add_u16 %r4, %r5, %r10, acq_rel

  amo_add_u32 %r4, %r5, %r10, relaxed
  amo_add_u32 %r4, %r5, %r10, acquire
  amo_add_u32 %r4, %r5, %r10, release
  amo_add_u32 %r4, %r5, %r10, acq_rel

  amo_add_u64 %r4, %r5, %r10, relaxed
  amo_add_u64 %r4, %r5, %r10, acquire
  amo_add_u64 %r4, %r5, %r10, release
  amo_add_u64 %r4, %r5, %r10, acq_rel

  amo_add_u128 %r4, %r5, %r10, relaxed
  amo_add_u128 %r4, %r5, %r10, acquire
  amo_add_u128 %r4, %r5, %r10, release
  amo_add_u128 %r4, %r5, %r10, acq_rel

  write "test amo-sub"

  amo_sub_u8 %r4, %r5, %r10, relaxed
  amo_sub_u8 %r4, %r5, %r10, acquire
  amo_sub_u8 %r4, %r5, %r10, release
  amo_sub_u8 %r4, %r5, %r10, acq_rel

  amo_sub_u16 %r4, %r5, %r10, relaxed
  amo_sub_u16 %r4, %r5, %r10, acquire
  amo_sub_u16 %r4, %r5, %r10, release
  amo_sub_u16 %r4, %r5, %r10, acq_rel

  amo_sub_u32 %r4, %r5, %r10, relaxed
  amo_sub_u32 %r4, %r5, %r10, acquire
  amo_sub_u32 %r4, %r5, %r10, release
  amo_sub_u32 %r4, %r5, %r10, acq_rel

  amo_sub_u64 %r4, %r5, %r10, relaxed
  amo_sub_u64 %r4, %r5, %r10, acquire
  amo_sub_u64 %r4, %r5, %r10, release
  amo_sub_u64 %r4, %r5, %r10, acq_rel

  amo_sub_u128 %r4, %r5, %r10, relaxed
  amo_sub_u128 %r4, %r5, %r10, acquire
  amo_sub_u128 %r4, %r5, %r10, release
  amo_sub_u128 %r4, %r5, %r10, acq_rel

  write "test amo-and"

  amo_and_u8 %r4, %r5, %r10, relaxed
  amo_and_u8 %r4, %r5, %r10, acquire
  amo_and_u8 %r4, %r5, %r10, release
  amo_and_u8 %r4, %r5, %r10, acq_rel

  amo_and_u16 %r4, %r5, %r10, relaxed
  amo_and_u16 %r4, %r5, %r10, acquire
  amo_and_u16 %r4, %r5, %r10, release
  amo_and_u16 %r4, %r5, %r10, acq_rel

  amo_and_u32 %r4, %r5, %r10, relaxed
  amo_and_u32 %r4, %r5, %r10, acquire
  amo_and_u32 %r4, %r5, %r10, release
  amo_and_u32 %r4, %r5, %r10, acq_rel

  amo_and_u64 %r4, %r5, %r10, relaxed
  amo_and_u64 %r4, %r5, %r10, acquire
  amo_and_u64 %r4, %r5, %r10, release
  amo_and_u64 %r4, %r5, %r10, acq_rel

  amo_and_u128 %r4, %r5, %r10, relaxed
  amo_and_u128 %r4, %r5, %r10, acquire
  amo_and_u128 %r4, %r5, %r10, release
  amo_and_u128 %r4, %r5, %r10, acq_rel

  write "test amo-or"

  amo_or_u8 %r4, %r5, %r10, relaxed
  amo_or_u8 %r4, %r5, %r10, acquire
  amo_or_u8 %r4, %r5, %r10, release
  amo_or_u8 %r4, %r5, %r10, acq_rel

  amo_or_u16 %r4, %r5, %r10, relaxed
  amo_or_u16 %r4, %r5, %r10, acquire
  amo_or_u16 %r4, %r5, %r10, release
  amo_or_u16 %r4, %r5, %r10, acq_rel

  amo_or_u32 %r4, %r5, %r10, relaxed
  amo_or_u32 %r4, %r5, %r10, acquire
  amo_or_u32 %r4, %r5, %r10, release
  amo_or_u32 %r4, %r5, %r10, acq_rel

  amo_or_u64 %r4, %r5, %r10, relaxed
  amo_or_u64 %r4, %r5, %r10, acquire
  amo_or_u64 %r4, %r5, %r10, release
  amo_or_u64 %r4, %r5, %r10, acq_rel

  amo_or_u128 %r4, %r5, %r10, relaxed
  amo_or_u128 %r4, %r5, %r10, acquire
  amo_or_u128 %r4, %r5, %r10, release
  amo_or_u128 %r4, %r5, %r10, acq_rel

  write "test amo-xor"

  amo_xor_u8 %r4, %r5, %r10, relaxed
  amo_xor_u8 %r4, %r5, %r10, acquire
  amo_xor_u8 %r4, %r5, %r10, release
  amo_xor_u8 %r4, %r5, %r10, acq_rel

  amo_xor_u16 %r4, %r5, %r10, relaxed
  amo_xor_u16 %r4, %r5, %r10, acquire
  amo_xor_u16 %r4, %r5, %r10, release
  amo_xor_u16 %r4, %r5, %r10, acq_rel

  amo_xor_u32 %r4, %r5, %r10, relaxed
  amo_xor_u32 %r4, %r5, %r10, acquire
  amo_xor_u32 %r4, %r5, %r10, release
  amo_xor_u32 %r4, %r5, %r10, acq_rel

  amo_xor_u64 %r4, %r5, %r10, relaxed
  amo_xor_u64 %r4, %r5, %r10, acquire
  amo_xor_u64 %r4, %r5, %r10, release
  amo_xor_u64 %r4, %r5, %r10, acq_rel

  amo_xor_u128 %r4, %r5, %r10, relaxed
  amo_xor_u128 %r4, %r5, %r10, acquire
  amo_xor_u128 %r4, %r5, %r10, release
  amo_xor_u128 %r4, %r5, %r10, acq_rel

  write "test amo-smin"
  amo_min_i8   %r4, %r5, %r10, relaxed
  amo_min_i8   %r4, %r5, %r10, acquire
  amo_min_i8   %r4, %r5, %r10, release
  amo_min_i8  %r4, %r5, %r10, acq_rel

  amo_min_i16  %r4, %r5, %r10, relaxed
  amo_min_i16  %r4, %r5, %r10, acquire
  amo_min_i16  %r4, %r5, %r10, release
  amo_min_i16 %r4, %r5, %r10, acq_rel

  amo_min_i32  %r4, %r5, %r10, relaxed
  amo_min_i32  %r4, %r5, %r10, acquire
  amo_min_i32  %r4, %r5, %r10, release
  amo_min_i32  %r4, %r5, %r10, acq_rel

  amo_min_i64  %r4, %r5, %r10, relaxed
  amo_min_i64  %r4, %r5, %r10, acquire
  amo_min_i64  %r4, %r5, %r10, release
  amo_min_i64  %r4, %r5, %r10, acq_rel

  amo_min_i128  %r4, %r5, %r10, relaxed
  amo_min_i128  %r4, %r5, %r10, acquire
  amo_min_i128  %r4, %r5, %r10, release
  amo_min_i128  %r4, %r5, %r10, acq_rel

  write "test amo-smax"
  amo_max_i8  %r4, %r5, %r10, relaxed
  amo_max_i8  %r4, %r5, %r10, acquire
  amo_max_i8  %r4, %r5, %r10, release
  amo_max_i8  %r4, %r5, %r10, acq_rel

  amo_max_i16  %r4, %r5, %r10, relaxed
  amo_max_i16  %r4, %r5, %r10, acquire
  amo_max_i16  %r4, %r5, %r10, release
  amo_max_i16  %r4, %r5, %r10, acq_rel

  amo_max_i32  %r4, %r5, %r10, relaxed
  amo_max_i32  %r4, %r5, %r10, acquire
  amo_max_i32  %r4, %r5, %r10, release
  amo_max_i32  %r4, %r5, %r10, acq_rel

  amo_max_i64  %r4, %r5, %r10, relaxed
  amo_max_i64  %r4, %r5, %r10, acquire
  amo_max_i64  %r4, %r5, %r10, release
  amo_max_i64  %r4, %r5, %r10, acq_rel

  amo_max_i128  %r4, %r5, %r10, relaxed
  amo_max_i128  %r4, %r5, %r10, acquire
  amo_max_i128  %r4, %r5, %r10, release
  amo_max_i128  %r4, %r5, %r10, acq_rel

  write "test amo-umin"
  amo_min_u8  %r4, %r5, %r10, relaxed
  amo_min_u8  %r4, %r5, %r10, acquire
  amo_min_u8  %r4, %r5, %r10, release
  amo_min_u8  %r4, %r5, %r10, acq_rel

  amo_min_u16  %r4, %r5, %r10, relaxed
  amo_min_u16  %r4, %r5, %r10, acquire
  amo_min_u16  %r4, %r5, %r10, release
  amo_min_u16  %r4, %r5, %r10, acq_rel

  amo_min_u32 %r4, %r5, %r10, relaxed
  amo_min_u32 %r4, %r5, %r10, acquire
  amo_min_u32 %r4, %r5, %r10, release
  amo_min_u32 %r4, %r5, %r10, acq_rel

  amo_min_u64  %r4, %r5, %r10, relaxed
  amo_min_u64  %r4, %r5, %r10, acquire
  amo_min_u64  %r4, %r5, %r10, release
  amo_min_u64  %r4, %r5, %r10, acq_rel

  amo_min_u128  %r4, %r5, %r10, relaxed
  amo_min_u128  %r4, %r5, %r10, acquire
  amo_min_u128  %r4, %r5, %r10, release
  amo_min_u128  %r4, %r5, %r10, acq_rel

  write "test amo-umax"
  amo_max_u8  %r4, %r5, %r10, relaxed
  amo_max_u8  %r4, %r5, %r10, acquire
  amo_max_u8  %r4, %r5, %r10, release
  amo_max_u8  %r4, %r5, %r10, acq_rel

  amo_max_u16  %r4, %r5, %r10, relaxed
  amo_max_u16  %r4, %r5, %r10, acquire
  amo_max_u16  %r4, %r5, %r10, release
  amo_max_u16  %r4, %r5, %r10, acq_rel

  amo_max_u32  %r4, %r5, %r10, relaxed
  amo_max_u32  %r4, %r5, %r10, acquire
  amo_max_u32  %r4, %r5, %r10, release
  amo_max_u32  %r4, %r5, %r10, acq_rel

  amo_max_u64  %r4, %r5, %r10, relaxed
  amo_max_u64  %r4, %r5, %r10, acquire
  amo_max_u64  %r4, %r5, %r10, release
  amo_max_u64  %r4, %r5, %r10, acq_rel

  amo_max_u128 %r4, %r5, %r10, relaxed
  amo_max_u128 %r4, %r5, %r10, acquire
  amo_max_u128 %r4, %r5, %r10, release
  amo_max_u128 %r4, %r5, %r10, acq_rel

  write "test cas"

  amo_cas_u8  %r12, %r5, %r56, %r34, relaxed
  amo_cas_u8  %r12, %r5, %r56, %r34, acquire
  amo_cas_u8  %r12, %r5, %r56, %r34, release
  amo_cas_u8  %r12, %r5, %r56, %r34, acq_rel

  amo_cas_u16 %r12, %r5, %r56, %r34, relaxed
  amo_cas_u16 %r12, %r5, %r56, %r34, acquire
  amo_cas_u16 %r12, %r5, %r56, %r34, release
  amo_cas_u16 %r12, %r5, %r56, %r34, acq_rel

  amo_cas_u32 %r12, %r5, %r56, %r34, relaxed
  amo_cas_u32 %r12, %r5, %r56, %r34, acquire
  amo_cas_u32 %r12, %r5, %r56, %r34, release
  amo_cas_u32 %r12, %r5, %r56, %r34, acq_rel

  amo_cas_u64 %r12, %r5, %r56, %r34, relaxed
  amo_cas_u64 %r12, %r5, %r56, %r34, acquire
  amo_cas_u64 %r12, %r5, %r56, %r34, release
  amo_cas_u64 %r12, %r5, %r56, %r34, acq_rel

  amo_cas_u128 %r12, %r5, %r56, %r34, relaxed
  amo_cas_u128 %r12, %r5, %r56, %r34, acquire
  amo_cas_u128 %r12, %r5, %r56, %r34, release
  amo_cas_u128 %r12, %r5, %r56, %r34, acq_rel

  write "test load atomic relaxed"
  amo_ld_u8   %r12, %r5, relaxed
  amo_ld_u16  %r12, %r5, relaxed
  amo_ld_u32  %r12, %r5, relaxed
  amo_ld_u64  %r12, %r5, relaxed
  amo_ld_u128 %r12, %r5, relaxed

  write "test load atomic acquire"
  amo_ld_u8   %r12, %r5, acquire
  amo_ld_u16  %r12, %r5, acquire
  amo_ld_u32  %r12, %r5, acquire
  amo_ld_u64  %r12, %r5, acquire
  amo_ld_u128 %r12, %r5, acquire

  write "test store atomic relaxed"
  amo_st_u8   %r12, %r5, relaxed
  amo_st_u16  %r12, %r5, relaxed
  amo_st_u32  %r12, %r5, relaxed
  amo_st_u64  %r12, %r5, relaxed
  amo_st_u128 %r12, %r5, relaxed

  write "test store atomic release"
  amo_st_u8   %r12, %r5, release
  amo_st_u16  %r12, %r5, release
  amo_st_u32  %r12, %r5, release
  amo_st_u64  %r12, %r5, release
  amo_st_u128 %r12, %r5, release

.end
