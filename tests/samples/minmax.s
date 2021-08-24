.text
    alloc 69
    write "test min/max"
    min_i64 %r34, %r56, %r67
    min_u64 %r34, %r56, %r67
    max_i64 %r34, %r56, %r67
    max_u64 %r34, %r56, %r67

    min_imm_i64 %r34, %r56, 2671
    min_imm_u64 %r34, %r56, 2671
    max_imm_i64 %r34, %r56, 2671
    max_imm_u64 %r34, %r56, 2671
    write "test minmax end"

.end
