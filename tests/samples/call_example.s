.text

call_code_target:

.rodata
call_data_target:

.text
    jmp	callexample
;*****************************************************************
; Function  compute A**4 of parameter A, passed in register r33
;*****************************************************************
quadrat:
    write	"function quadrat entered: r0=%x128(r0)"
    alloc	93
    write	"rsc     %s(rsc)"
    write	"psr     %s(psr)"
    write	"rsc     %s(rsc)"
    mul	%r33, %r33, %r33
    mul	%r33, %r33, %r33
    write	"r0=%x128(r0) r33=%i64(r33)"
    write	"%m(dump)"
;	mtspr	%r45, psr
    write	"function quadrat exited"
    ret
end_quadrat:

;*****************************************************************
; Example of calling sequence with branch prediction
callexample:
    alloc	91
    ldi.l	%r90, 0x1234567890abcdef
    write	"arg3 %x64(r90)"
    srpi	%r89, %r90, %r90, 16
    write	"arg2 %x64(r89)"
    srpi	%r88, %r90, %r90, 16
    write	"arg1 %x64(r88)"
    ldi		%r87, 7		; setup arguments
;   write	"%m(dump)"
    write	"rsc: %s(rsc)"
    write	"function quadrat called"
    callr	%r86, quadrat
    write	"rsc: %s(rsc)"
; Rest instructions after return from subroutine
;*****************************************************************
.text	; return to code section

; Here we test registers used by ABI (application binary interface)
; Check loader.
    write	"sp=%x64(sp) tp=%x64(tp) r0=%x128(r0)"
    write	"rsc: %s(rsc)"
    write	"psr: %s(psr)"
    write	"r14: %x64(r14)"
    write	"reta: %i64(r72)"		; out return address
    write	"retv: %i64(r73)"		; out return value
    write	"rsc: %s(rsc)"
    write	"rsc: %s(psr)"
    ldi.l	%r11, 0x407d8bffffccccff
    write	"r11: %x64(r11)"
    addi.l	%r12, %r11, 0x400000
    write	"r12: %x64(r12)"
    xor		%r20, %r19, %r11
    addi.l	%r20, %r20, 0x400000
    ldi		%r10, 10
    ldi		%r11, 11
    cmpdlt	%r2, %r11, %r10
    write	"%i64(r11) %i64(r10)"
    jmp		call_exit

    callr	%r42, quadrat
    callri	%r42, %r34, %gz
    callmi	%r42, %r34, 468
    callplt	%r42, call_data_target
    callri	%r42, %r34, %gz

call_exit:
    write	"end call test"

.end
