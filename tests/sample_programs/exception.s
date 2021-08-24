.text
	write	"begin exception test"
	alloc	r95, exception_exit

	ldr	r2, catch
	mtspr	r2, eip

; constructor 1
	ldi	r4, 1
	ehadj	call_destructor_1
	write	"eip: %s(eip)"
; constructor 2
	ldi	r5, 2
	ehadj	call_destructor_2
	write	"eip: %s(eip)"

	ldi	r3, 0xFFFFFFFFFFFF1230
	ehthrow	r3, 0    ; set eca, jump to eip
	write	"normal execution (never occurs)"

call_destructor_2:
	write	"call_destructor_2"
	ehcatch	r6, end_destructor_2
	; here dtor called
	ldi	r4, 0
end_destructor_2:
	ehnext	r6, call_destructor_1
	write	"normal continue after destructor_2"

call_destructor_1:
	write	"call_destructor_1"
	ehcatch	r6, end_destructor_1
	; here dtor called
	ldi	r5, 0
end_destructor_1:
	ehnext	r6, catch
	write	"normal continue after destructor_1"

call_ret:
	write	"normal exit"
	br	exception_exit

catch:
	write	"caught exception, exit"
	ehcatch	r12, exception_exit
	write	"caught exception context: r12=%x64(r12)"
exception_exit:
	nop	1234567
	nop	7654321
.end
