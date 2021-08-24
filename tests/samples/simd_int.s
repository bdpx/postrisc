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
    alloc	90
    ldar	%r4, mmxdata
    ldq		%r1, %r4,0*16
    ldq		%r2, %r4,1*16
    ldq		%r3, %r4,2*16
    ldq		%r4, %r4,3*16
    write	"r1      %x128(r1)"
    write	"r2      %x128(r2)"

    write	"%vu8(r1)"
    write	"%vu16(r1)"
    write	"%vu32(r1)"
    write	"%vu64(r1)"

    vaddub	%r3, %r1, %r2
    write	"test vadd/vaddc (1 byte)\0"
    vaddcb	%r4, %r1, %r2
    write	"%vu8(r1)"
    write	"%vu16(r2)"
    write	"%vu32(r3)"
    write	"%vu64(r4)"
    write	"test vadd/vaddo signed (1 byte)\0"
    vaddob	%r4, %r1, %r2
    write	"%vi8(r1)"
    write	"%vi16(r2)"
    write	"%vi32(r3)"
    write	"%vu64(r4)"

    vsubub	%r3, %r1, %r2
    write	"test vsub/vsubb (1 byte)\0"
    vsubbb	%r4, %r1, %r2
    write	"%vu8(r1)"
    write	"%vu8(r2)"
    write	"%vu8(r3)"
    write	"%vu8(r4)"
    write	"test vsub/vsubo signed (1 byte)\0"
    vsubob	%r4, %r1, %r2
    write	"%vi8(r1)"
    write	"%vi8(r2)"
    write	"%vi8(r3)"
    write	"%vu8(r4)"

    write	"test vaddusb"
    vaddub	%r3, %r1, %r2
    vaddusb	%r4, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

    write	"test vsubusb"
    vsubub	%r3, %r1, %r2
    vsubusb	%r4, %r1, %r2
    write	"%vu8(r1):\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

    write	"test vaddssb"
    vaddub	%r3, %r1, %r2
    vaddssb	%r4, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

    write	"test vsubssb"
    vsubub	%r3, %r1, %r2
    vsubssb	%r4, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

    write	"test pavgu (1 byte)\0"
    vavgub	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test pavgs (1 byte)\0"
    vavgsb	%r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test vminu (1 byte)\0"
    vminub	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test vmins (1 byte)\0"
    vminsb	%r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test vmaxu (1 byte)\0"
    vmaxub	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test vmaxs (1 byte)\0"
    vmaxsb	%r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test merge low (1 byte)\0"
    vmrglb	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test merge high (1 byte)\0"
    vmrghb	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    vpkuush	%r2, %r3, %r4
    vpksush	%r2, %r3, %r4
    vpksssh	%r2, %r3, %r4

    vpkuusw	%r2, %r3, %r4
    vpksusw	%r2, %r3, %r4
    vpksssw	%r2, %r3, %r4

    vpkuusd	%r2, %r3, %r4
    vpksusd	%r2, %r3, %r4
    vpksssd	%r2, %r3, %r4

;	jmp	endmmx
; d1 abs
    vminsb	%r12, %r61, %r55
    vminsh	%r18, %r61, %r45
    vminsw	%r27, %r61, %r35
    vminsd	%r36, %r61, %r25

    vminub	%r14, %r61, %r15
    vminuh	%r15, %r62, %r75
    vminuw	%r17, %r63, %r85
    vminud	%r16, %r64, %r75

    vmaxsb	%r26, %r71, %r85
    vmaxsh	%r26, %r61, %r54
    vmaxsw	%r16, %r51, %r35
    vmaxsd	%r16, %r41, %r55

    vmaxub	%r11, %r61, %r53
    vmaxuh	%r12, %r55, %r55
    vmaxuw	%r16, %r46, %r56
    vmaxud	%r13, %r31, %r55

    vrolb	%r56, %r61, %r15
    vrolh	%r31, %r61, %r25
    vrolw	%r53, %r61, %r30
    vrold	%r62, %r61, %r41

    vrorb	%r16, %r11, %r52
    vrorh	%r11, %r21, %r63
    vrorw	%r71, %r31, %r74
    vrord	%r81, %r41, %r85

    vsllb	%r16, %r51, %r86
    vsllh	%r24, %r61, %r55
    vsllw	%r69, %r71, %r55
    vslld	%r77, %r81, %r55

    vsrlb	%r21, %r81, %r50
    vsrlh	%r12, %r63, %r51
    vsrlw	%r13, %r62, %r52
    vsrld	%r64, %r63, %r53

    vsrab	%r85, %r64, %r54
    vsrah	%r76, %r65, %r15
    vsraw	%r67, %r66, %r25
    vsrad	%r58, %r67, %r36

    vavgsb	%r49, %r68, %r47
    vavgsh	%r30, %r69, %r58
    vavgsw	%r26, %r11, %r69
    vavgsd	%r16, %r21, %r75

    vavgub	%r14, %r31, %r85
    vavguh	%r15, %r41, %r45
    vavguw	%r56, %r51, %r25
    vavgud	%r87, %r61, %r15

    vaddssb	%r42, %r71, %r15
    vaddssh	%r83, %r81, %r45
    vaddssw	%r74, %r41, %r85
    vaddssd	%r65, %r61, %r75

    vaddub	%r56, %r61, %r75
    vadduh	%r47, %r61, %r65
    vadduw	%r38, %r61, %r55
    vaddud	%r29, %r61, %r55

    vaddusb	%r55, %r61, %r45
    vaddush	%r65, %r61, %r35
    vaddusw	%r74, %r61, %r25
    vaddusd	%r84, %r61, %r15

    vaddcb	%r53, %r61, %r55
    vaddch	%r13, %r61, %r55
    vaddcw	%r12, %r61, %r55
    vaddcd	%r12, %r61, %r55

    vsubssb	%r56, %r61, %r15
    vsubssh	%r67, %r61, %r12
    vsubssw	%r78, %r61, %r13
    vsubssd	%r89, %r61, %r45

    vsubub	%r70, %r61, %r85
    vsubuh	%r86, %r61, %r45
    vsubuw	%r46, %r61, %r13
    vsubud	%r46, %r61, %r75

    vsubusb	%r41, %r68, %r65
    vsubush	%r12, %r37, %r55
    vsubusw	%r23, %r26, %r45
    vsubusd	%r14, %r18, %r35

    vcmpeqb	%r86, %r61, %r25
    vcmpeqh	%r44, %r72, %r15
    vcmpeqw	%r20, %r83, %r55
    vcmpeqd	%r16, %r84, %r55

;	pcmpne	%r106, %r61, %r55
;	pcmpgt	%r106, %r61, %r55
;	pcmpge	%r106, %r61, %r55
;	pcmple	%r106, %r61, %r55

    vcmpltb	%r13, %r61, %r15
    vcmplth	%r14, %r61, %r24
    vcmpltw	%r15, %r61, %r38
    vcmpltd	%r16, %r61, %r45

    vcmpltub	%r19, %r11, %r75
    vcmpltuh	%r18, %r21, %r82
    vcmpltuw	%r16, %r31, %r73
    vcmpltud	%r14, %r71, %r54

    vmrghb	%r11, %r71, %r13
    vmrghh	%r72, %r67, %r27
    vmrghw	%r13, %r58, %r55
    vmrghd	%r14, %r69, %r15

    vmrglb	%r76, %r61, %r11
    vmrglh	%r26, %r11, %r62
    vmrglw	%r16, %r15, %r73
    vmrgld	%r16, %r11, %r85

    write	"end simd(int) test"
endmmx:

.end
