.text
.rodata
	align	16
mmxdata:
	d8	0x123456759eabcd7f
	d8	0x123456789cabcdef

	d8	0xf87f5432afebcdf3
	d8	0xffffffffffffffff

	d8	0x1234567890abcdef
	d8	0x1234567890abcdef

	d8	0x1234567890abcdef
	d8	0x1234567890abcdef
.text
	ldar	r4, mmxdata
	ld16	r1, r4,0*16
	ld16	r2, r4,1*16
	ld16	r3, r4,2*16
	ld16	r4, r4,3*16
	write	"r1      %x128(r1)"
	write	"r2      %x128(r2)"

	write	"%vu8(r1)"
	write	"%vu16(r1)"
	write	"%vu32(r1)"
	write	"%vu64(r1)"

	vaddu1	r3, r1, r2
	write	"test vadd/vaddc (1 byte)\0"
	vaddc1	r4, r1, r2
	write	"%vu8(r1)"
	write	"%vu16(r2)"
	write	"%vu32(r3)"
	write	"%vu64(r4)"
	write	"test vadd/vaddo signed (1 byte)\0"
	vaddo1	r4, r1, r2
	write	"%vi8(r1)"
	write	"%vi16(r2)"
	write	"%vi32(r3)"
	write	"%vu64(r4)"

	vsubu1	r3, r1, r2
	write	"test vsub/vsubb (1 byte)\0"
	vsubb1	r4, r1, r2
	write	"%vu8(r1)"
	write	"%vu8(r2)"
	write	"%vu8(r3)"
	write	"%vu8(r4)"
	write	"test vsub/vsubo signed (1 byte)\0"
	vsubo1	r4, r1, r2
	write	"%vi8(r1)"
	write	"%vi8(r2)"
	write	"%vi8(r3)"
	write	"%vu8(r4)"

	write	"test vaddus1"
	vaddu1	r3, r1, r2
	vaddus1	r4, r1, r2
	write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

	write	"test vsubus1"
	vsubu1	r3, r1, r2
	vsubus1	r4, r1, r2
	write	"%vu8(r1):\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

	write	"test vaddss1"
	vaddu1	r3, r1, r2
	vaddss1	r4, r1, r2
	write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

	write	"test vsubss1"
	vsubu1	r3, r1, r2
	vsubss1	r4, r1, r2
	write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

	write	"test pavgu (1 byte)\0"
	vavgu1	r3, r1, r2
	write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

	write	"test pavgs (1 byte)\0"
	vavgs1	r3, r1, r2
	write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

	write	"test vminu (1 byte)\0"
	vminu1	r3, r1, r2
	write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

	write	"test vmins (1 byte)\0"
	vmins1	r3, r1, r2
	write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

	write	"test vmaxu (1 byte)\0"
	vmaxu1	r3, r1, r2
	write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

	write	"test vmaxs (1 byte)\0"
	vmaxs1	r3, r1, r2
	write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

	write	"test merge low (1 byte)\0"
	vmrgl1	r3, r1, r2
	write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

	write	"test merge high (1 byte)\0"
	vmrgh1	r3, r1, r2
	write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

	vpkuus2	r2, r3, r4
	vpksus2	r2, r3, r4
	vpksss2	r2, r3, r4

	vpkuus4	r2, r3, r4
	vpksus4	r2, r3, r4
	vpksss4	r2, r3, r4

	vpkuus8	r2, r3, r4
	vpksus8	r2, r3, r4
	vpksss8	r2, r3, r4

;	br	endmmx
; d1 abs
	vmins1	r12, r61, r55
	vmins2	r18, r61, r45
	vmins4	r27, r61, r35
	vmins8	r36, r61, r25

	vminu1	r14, r61, r15
	vminu2	r15, r62, r75
	vminu4	r17, r63, r85
	vminu8	r16, r64, r75

	vmaxs1	r26, r71, r85
	vmaxs2	r26, r61, r54
	vmaxs4	r16, r51, r35
	vmaxs8	r16, r41, r55
                    
	vmaxu1	r11, r61, r53
	vmaxu2	r12, r55, r55
	vmaxu4	r16, r46, r56
	vmaxu8	r13, r31, r55

	vrol1	r56, r61, r15
	vrol2	r31, r61, r25
	vrol4	r53, r61, r30
	vrol8	r62, r61, r41

	vror1	r16, r11, r52
	vror2	r11, r21, r63
	vror4	r71, r31, r74
	vror8	r81, r41, r85

	vsll1	r16, r51, r86
	vsll2	r24, r61, r55
	vsll4	r69, r71, r55
	vsll8	r77, r81, r55

	vsrl1	r21, r81, r50
	vsrl2	r12, r63, r51
	vsrl4	r13, r62, r52
	vsrl8	r64, r63, r53

	vsra1	r85, r64, r54
	vsra2	r76, r65, r15
	vsra4	r67, r66, r25
	vsra8	r58, r67, r36

	vavgs1	r49, r68, r47
	vavgs2	r30, r69, r58
	vavgs4	r26, r11, r69
	vavgs8	r16, r21, r75

	vavgu1	r14, r31, r85
	vavgu2	r15, r41, r45
	vavgu4	r56, r51, r25
	vavgu8	r87, r61, r15

	vaddss1	r42, r71, r15
	vaddss2	r83, r81, r45
	vaddss4	r74, r41, r85
	vaddss8	r65, r61, r75

	vaddu1	r56, r61, r75
	vaddu2	r47, r61, r65
	vaddu4	r38, r61, r55
	vaddu8	r29, r61, r55

	vaddus1	r55, r61, r45
	vaddus2	r65, r61, r35
	vaddus4	r74, r61, r25
	vaddus8	r84, r61, r15

	vaddc1	r53, r61, r55
	vaddc2	r13, r61, r55
	vaddc4	r12, r61, r55
	vaddc8	r12, r61, r55

	vsubss1	r56, r61, r15
	vsubss2	r67, r61, r12
	vsubss4	r78, r61, r13
	vsubss8	r89, r61, r45

	vsubu1	r70, r61, r85
	vsubu2	r86, r61, r45
	vsubu4	r46, r61, r13
	vsubu8	r46, r61, r75

	vsubus1	r41, r68, r65
	vsubus2	r12, r37, r55
	vsubus4	r23, r26, r45
	vsubus8	r14, r18, r35

	vcmpeq1	r86, r61, r25
	vcmpeq2	r44, r72, r15
	vcmpeq4	r20, r83, r55
	vcmpeq8	r16, r84, r55

;	pcmpne	r106, r61, r55
;	pcmpgt	r106, r61, r55
;	pcmpge	r106, r61, r55
;	pcmple	r106, r61, r55

	vcmplt1	r13, r61, r15
	vcmplt2	r14, r61, r24
	vcmplt4	r15, r61, r38
	vcmplt8	r16, r61, r45

	vcmpltu1	r19, r11, r75
	vcmpltu2	r18, r21, r82
	vcmpltu4	r16, r31, r73
	vcmpltu8	r14, r71, r54

	vmrgh1	r11, r71, r13
	vmrgh2	r72, r67, r27
	vmrgh4	r13, r58, r55
	vmrgh8	r14, r69, r15

	vmrgl1	r76, r61, r11
	vmrgl2	r26, r11, r62
	vmrgl4	r16, r15, r73
	vmrgl8	r16, r11, r85

	write	"end simd(int) test"
endmmx:

.end
