.text
	alloc	r95, end_indexed_modify_test
	write	"test base addressing with indexed post-update"
	ldi	r12, 1
	addi	r45, sp, -512

	ld1mxa	r23, r45, r12, 2
	ld2mxa	r23, r45, r12, 2
	ld4mxa	r23, r45, r12, 3
	ld8mxa	r23, r45, r12, 4
	ld16mxa	r23, r45, r12, 4

	lds1mxa	r23, r45, r12, 2
	lds2mxa	r23, r45, r12, 2
	lds4mxa	r23, r45, r12, 3
	lds8mxa	r23, r45, r12, 3

	st1mxa	r23, r45, r12, 2 
	st2mxa	r23, r45, r12, 2
	st4mxa	r23, r45, r12, 3
	st8mxa	r23, r45, r12, 3
	st16mxa	r23, r45, r12, 4
end_indexed_modify_test:
	write	"end_indexed_modify_test"
.end