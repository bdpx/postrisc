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
    ldard %r12, test_memory
    write "test_memory: %x64(r12)"
    ldi %r11, 0
    ldi %r14, 0
memory_loop: (32)
    ldudxsd %r13, %r12, %r11, 3, 0
    addid %r11, %r11, 1
    addid %r14, %r14, 1
    andi %r11, %r11, 7
; fast_check
    brltsid.l %r14, 200000, memory_loop
    write "counter: %i64(r14)"
.end
