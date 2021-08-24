.text
    alloc	96
    write	"test ldafr"
    ldafr	%r22, ldafr_data
    write	"ldafr: %x64(r22)"

    write	"end_ldafr_test"
.data
ldafr_data:

.end