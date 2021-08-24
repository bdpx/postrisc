.text
    alloc	96
    write	"test base addressing with indexed post-update"
    ldi		%r12, 1
    addi	%r45, %sp, -512

    ldz.b.mia	%r23, %r45, 2
    ldz.h.mia	%r23, %r45, 2
    ldz.w.mia	%r23, %r45, 4
    ldz.d.mia	%r23, %r45, 8
    ld.q.mia	%r23, %r45, 16

    lds.b.mia	%r23, %r45, 2
    lds.h.mia	%r23, %r45, 2
    lds.w.mia	%r23, %r45, 4
    lds.d.mia	%r23, %r45, 8

    st.b.mia	%r23, %r45, 2 
    st.h.mia	%r23, %r45, 2
    st.w.mia	%r23, %r45, 4
    st.d.mia	%r23, %r45, 8
    st.q.mia	%r23, %r45, 16
    write	"end_indexed_modify_test"
.end
