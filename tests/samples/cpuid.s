.text
    write "cpuid implemented number"
    alloc 96
    ldi %r13, 0
    cpuid %r14, %r13, 0
    write "cpuid len %x64(r14)"
    write "cpuid loop"
cpuid_loop:
    cpuid %r15, %r13, 0
    write "cpuid[%i64(r13)] = %x64(r15)"
    repltd %r13, %r14, 1, cpuid_loop
    cpuid.l %r15, %r13, 1000000
    cpuid.l %r15, %r13, 1000000000
    cpuid %r15, %r13, 100
.end
