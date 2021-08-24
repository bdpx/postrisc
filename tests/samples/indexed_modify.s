.text
    alloc 96
    write "test base addressing with indexed post-update"
    ldi %r12, 1
    addid %r45, %sp, -512

    ldubmia  %r23, %r45, 2
    lduhmia  %r23, %r45, 2
    lduwmia  %r23, %r45, 4
    ldudmia  %r23, %r45, 8

    ldqmia %r23, %r45, 16

    ldsbmia %r23, %r45, 2
    ldshmia %r23, %r45, 2
    ldswmia %r23, %r45, 4
    ldsdmia %r23, %r45, 8

    stbmia %r23, %r45, 2
    sthmia %r23, %r45, 2
    stwmia %r23, %r45, 4
    stdmia %r23, %r45, 8
    stqmia %r23, %r45, 16
    write "end_indexed_modify_test"
.end
