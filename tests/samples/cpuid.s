.text
    write	"cpuid implemented number"
    alloc	96
    ldi		%r13, 0
    cpuid	%r14, %r13, 0
    write	"cpuid len %x64(r14)"
    write	"cpuid loop"
cpuid_loop:
    cpuid	%r15, %r13, 0
    write	"cpuid[%i64(r13)] = %x64(r15)"
    repdlt	%r13, %r14, cpuid_loop
.end
