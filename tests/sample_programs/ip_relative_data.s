.rodata
rodata1:
    d1	123
    align	2
rodata2:
    d2	12345
    align	4
rodata4:
    d4	123456789
    align	8
rodata8:
    d8	1234567890123456789

.data
data1:
    d1	123
    align	2
data2:
    d2	12345
    align	4
data4:
    d4	123456789
    align	8
data8:
    d8	1234567890123456789

.text
	alloc	r95, end_ip_relative_data_test

	write "test ip-relative data addressing"
	ld1r	r34, rodata1
	ld2r	r34, rodata2
	ld4r	r34, rodata4
	ld8r	r34, rodata8

	lds1r	r34, rodata1
	lds2r	r34, rodata2
	lds4r	r34, rodata4
	lds8r	r34, rodata8

	ld1r	r34, data1
	ld2r	r34, data2
	ld4r	r34, data4
	ld8r	r34, data8

	lds1r	r34, data1
	lds2r	r34, data2
	lds4r	r34, data4
	lds8r	r34, data8

	st1r	r34, data1 
	st2r	r34, data2
	st4r	r34, data4
	st8r	r34, data8
end_ip_relative_data_test:	
	write	"end ip-relative data test"
.end