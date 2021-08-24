.text
    alloc 69
    write "test min/max"
    mins.d %r34, %r56, %r67
    minu.d %r34, %r56, %r67
    maxs.d %r34, %r56, %r67
    maxu.d %r34, %r56, %r67

    minsi.d %r34, %r56, 2671
    minui.d %r34, %r56, 2671
    maxsi.d %r34, %r56, 2671
    maxui.d %r34, %r56, 2671
    write "test minmax end"

.end
