.text
	write	"test random"
	alloc	r95, end_random_test

	rdrand	r3
	write "rdrand: %x64(r3)"
	rdrand	r3
	write "rdrand: %x64(r3)"
	rdseed	r3
	write "rdseed: %x64(r3)"
end_random_test:
	write	"end_random_test"
.end