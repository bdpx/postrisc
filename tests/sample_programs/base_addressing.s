.text
.data
data_lbl:
	d1	25
	d1	26
	d1	27
	d1	28

.text
; Here we test references to data section.
; Absolute offset from begin of section
	write	"base addressing"
	ldi	r12, data_lbl
	write	"data_lbl: %i64(r12)"

	ldi	r12, data_hi(data_lbl)
	write	"data_hi(data_lbl): %i64(r12)"
	ldi	r12, data_lo(data_lbl)
	write	"data_lo(data_lbl): %i64(r12)"
	ldr	r13, data_lbl ; high 16-bit part of label 32-bit offset
	write	"ldr(data_lbl): %i64(r13)"

	ldan	r13, tp,data_hi(data_lbl)
	write	"r13     %i64(r13)"
	addi	r14, r13,data_lo(data_lbl)+0
	write	"r14     %i64(r14)"

	ldan	r13, tp,data_hi(data_lbl)
	write	"r13     %i64(r13)"
	ld1	r25, r13,data_lo(data_lbl)+0
	ld1	r26, r13,data_lo(data_lbl)+1
	ld1	r27, r13,data_lo(data_lbl)+2
	ld1	r28, r13,data_lo(data_lbl)+3
	write	"r25     %i64(r25)"	; must be 25
	write	"r26     %i64(r26)"	; must be 26
	write	"r27     %i64(r27)"	; must be 27
	write	"r28     %i64(r28)"	; must be 28

; test load context
	ld8	r1, sp,-16
	st8	r1, sp, -16
	br	skipaddr
	br.l	skipaddr

; test indexed load/store
	st1x	r12, r15,r30,4,14
	st2x	r12, r15,r30,4,14
	st4x	r12, r15,r30,4,14
	st8x	r12, r15,r30,4,14

	ld16a	r30, r56, acquire
	st16a	r43, r56, release

	sladd	r43, r56,r23,4
	slsub	r43, r56,r23,42
	slsubf	r43, r56,r23,12

	ld4	r30, r5,66*4	; load mid
	ld8x	r40, tp,r30,0,4	; load base

	lds8x	r12, r23,r40,3,114
	lds8x	r12, r23,r40,3,114
	ld8x	r12, r23,r40,3,114
	ld8x	r12, r23,r40,3,114
	st8x	r12, r23,r40,3,114
	st8x	r12, r23,r40,3,114

	lds1x	r12, r23,r40,3,114
	lds1x	r12, r23,r40,3,114
	ld1x	r12, r23,r40,3,114
	ld1x	r12, r23,r40,3,114
	st1x	r12, r23,r40,3,114
	st1x	r12, r23,r40,3,114

	lds2x	r12, r23,r40,3,114
	lds2x	r12, r23,r40,3,114
	ld2x	r12, r23,r40,3,114
	ld2x	r12, r23,r40,3,114
	st2x	r12, r23,r40,3,114
	st2x	r12, r23,r40,3,114

.text
; LOAD/STORE
	sladd	r54, r56,r12,5

	ld1	r16, r45,8900
	lds1	r15, r46,8900
	ld1x	r54, r56,r12,2,37
	lds1x	r53, r65,r12,2,37
	ld1x.l	r54, r56,r12,2,37000000
	lds1x.l	r53, r65,r12,2,-37000000
	ld1mia	r52, r75,10
	lds1mia	r51, r76,10
	ld1mib	r52, r75,10
	lds1mib	r51, r76,10
	ld1mxa	r52, r75,r10,2
	lds1mxa	r51, r76,r10,2

	ld2	r12, r45,8900
	lds2	r12, r45,8900
	ld2x	r54, r56,r12,3,-157
	lds2x	r54, r56,r12,2,237
	ld2x.l	r54, r56,r12,2,37000000
	lds2x.l	r53, r65,r12,2,-37000000
	ld2mia	r54, r56,12
	lds2mia	r54, r56,-60
	ld2mib	r54, r56,12
	lds2mib	r54, r56,-60
	ld2mxa	r52, r75,r10,2
	lds2mxa	r51, r76,r10,2

	ld4	r12, r45,8900
	lds4	r12, r45,8900
	ld4x	r54, r56,r12,2,7
	lds4x	r54, r56,r12,2,7
	ld4x.l	r54, r56,r12,2,37000000
	lds4x.l	r53, r65,r12,2,-37000000
	ld4mia	r54, r56,12
	lds4mia	r54, r56,32
	ld4mib	r54, r56,12
	lds4mib	r54, r56,32
	ld4mxa	r52, r75,r10,2
	lds4mxa	r51, r76,r10,2

	ld8	r54, r56,5600
	lds8	r54, r56,5600
	ld8.l	r53, r46,98765432
	ld8	r52, r45,-5600
	ld8.l	r51, r55,-98765432
	ld8x	r50, r56,r12,2,37
	lds8x	r50, r56,r12,2,37
	ld8x.l	r54, r56,r12,2,37000000
	lds8x.l	r53, r65,r12,2,-37000000
	ld8mia	r57, r56,-12
	ld8mia	r57, r56,-12
	lds8mia	r57, r56,-12
	lds8mia	r57, r56,-12
	ld8mib	r57, r56,-12
	ld8mib	r57, r56,-12
	lds8mib	r57, r56,-12
	lds8mib	r57, r56,-12
	ld8mxa	r52, r75,r10,2
	lds8mxa	r51, r76,r10,2

	ld16	r16, r45,8900
	ld16.l	r16, r45,8900000
	ld16.l	r16, r45,-8900000
	ld16x	r54, r56,r12,2,37
	ld16x.l	r54, r56,r12,2,37000000
	ld16x.l	r54, r56,r12,2,-37000000
	ld16mia	r52, r75,10
	ld16mia	r52, r75,10
	ld16mib	r52, r75,10
	ld16mib	r52, r75,10
	ld16mxa	r52, r75,r10,2

	st1	r12, r45,8900
	st2	r12, r45,8900
	st4	r12, r45,8900
	st8	r12, r45,890*8

	ld8	r12, r45,8048
	st8	r12, r45,8064
	ld8x	r12, r45,r13,3,7
	st8x	r12, r45,r13,3,7

	ld8	r60, r55,56
	ld8	r60, r56,56
	ld8	r46, r55,120
	st8	r47, r55,56

	ld8	r60, sp,624
	st8	r60, sp,624
	ld8x	r60, sp,r12,3,28
	st8x	r60, sp,r12,3,26
	ld8	r56, r57,567
	st8	r56, r57,567

	ld4	r34, r12,900
	ld8	r34, r12,900
	st4	r23, r12,900
	st8	r23, r12,900

	ld16	r34, r13,55*16
	st16	r35, r13, 55*16
	ld16x	r34, r13,r45,3,80
	st16x	r34, r13,r45,3,80

skipaddr:
	nop	0
.end
