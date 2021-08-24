.text
    alloc	96
    write	"test base addressing with indexed post-update"
    ldi		%r12, 1
    addi	%r45, %sp, -512

    ldbzmia	%r23, %r45, 2
    ldhzmia	%r23, %r45, 2
    ldwzmia	%r23, %r45, 4
    lddzmia	%r23, %r45, 8
    ldqmia	%r23, %r45, 16

    ldbsmia	%r23, %r45, 2
    ldhsmia	%r23, %r45, 2
    ldwsmia	%r23, %r45, 4
    lddsmia	%r23, %r45, 8

    stbmia	%r23, %r45, 2 
    sthmia	%r23, %r45, 2
    stwmia	%r23, %r45, 4
    stdmia	%r23, %r45, 8
    stqmia	%r23, %r45, 16
    write	"end_indexed_modify_test"
.end