.text
    alloc 69
    write "test min/max"
    minsd %r34, %r56, %r67
    minud %r34, %r56, %r67
    maxsd %r34, %r56, %r67
    maxud %r34, %r56, %r67

    minsid %r34, %r56, 2671
    minuid %r34, %r56, 2671
    maxsid %r34, %r56, 2671
    maxuid %r34, %r56, 2671
    write "test minmax end"

.end
