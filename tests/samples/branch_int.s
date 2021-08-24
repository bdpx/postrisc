.text
    write "branch-int, test memory"
.data
align 8
test_memory:
    d8 0
    d8 1
    d8 2
    d8 3
    d8 4
    d8 5
    d8 6
    d8 7
.text
    alloc 20
    lda_iprel %r12, test_memory
    write "test_memory: %x64(r12)"
    ld_imm %r11, 0
    ld_imm %r14, 0
memory_loop: (32)
    ld_xi64_u64 %r13, %r12, %r11, 3, 0
    add_imm_i64 %r11, %r11, 1
    add_imm_i64 %r14, %r14, 1
    and_imm %r11, %r11, 7
; fast_check
    br_lt_imm_i64.l %r14, 200000, memory_loop
    write "counter: %i64(r14)"
.end
