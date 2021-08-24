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
    ca.r	%r4, mmxdata
    ld.q	%r1, %r4,0*16
    ld.q	%r2, %r4,1*16
    ld.q	%r3, %r4,2*16
    ld.q	%r4, %r4,3*16
    write	"r1      %x128(r1)"
    write	"r2      %x128(r2)"

    write	"%vu8(r1)"
    write	"%vu16(r1)"
    write	"%vu32(r1)"
    write	"%vu64(r1)"

    vaddu.b	%r3, %r1, %r2
    write	"test vadd/vaddc (1 byte)\0"
    vaddc.b	%r4, %r1, %r2
    write	"%vu8(r1)"
    write	"%vu16(r2)"
    write	"%vu32(r3)"
    write	"%vu64(r4)"
    write	"test vadd/vaddo signed (1 byte)\0"
    vaddo.b	%r4, %r1, %r2
    write	"%vi8(r1)"
    write	"%vi16(r2)"
    write	"%vi32(r3)"
    write	"%vu64(r4)"

    vsubu.b	%r3, %r1, %r2
    write	"test vsub/vsubb (1 byte)\0"
    vsubb.b	%r4, %r1, %r2
    write	"%vu8(r1)"
    write	"%vu8(r2)"
    write	"%vu8(r3)"
    write	"%vu8(r4)"
    write	"test vsub/vsubo signed (1 byte)\0"
    vsubo.b	%r4, %r1, %r2
    write	"%vi8(r1)"
    write	"%vi8(r2)"
    write	"%vi8(r3)"
    write	"%vu8(r4)"

    write	"test vaddusb"
    vaddu.b	%r3, %r1, %r2
    vaddus.b	%r4, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

    write	"test vsubusb"
    vsubu.b	%r3, %r1, %r2
    vsubus.b	%r4, %r1, %r2
    write	"%vu8(r1):\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

    write	"test vaddssb"
    vaddu.b	%r3, %r1, %r2
    vaddss.b	%r4, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

    write	"test vsubssb"
    vsubu.b	%r3, %r1, %r2
    vsubss.b	%r4, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

    write	"test pavgu (1 byte)\0"
    vavgu.b	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test pavgs (1 byte)\0"
    vavgs.b	%r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test vminu (1 byte)\0"
    vminu.b	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test vmins (1 byte)\0"
    vmins.b	%r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test vmaxu (1 byte)\0"
    vmaxu.b	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test vmaxs (1 byte)\0"
    vmaxs.b	%r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test merge low (1 byte)\0"
    vmrg.l.b	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test merge high (1 byte)\0"
    vmrg.h.b	%r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    vpkuus.h	%r2, %r3, %r4
    vpksus.h	%r2, %r3, %r4
    vpksss.h	%r2, %r3, %r4

    vpkuus.w	%r2, %r3, %r4
    vpksus.w	%r2, %r3, %r4
    vpksss.w	%r2, %r3, %r4

    vpkuus.d	%r2, %r3, %r4
    vpksus.d	%r2, %r3, %r4
    vpksss.d	%r2, %r3, %r4

;	jmp	endmmx
; d1 abs
    vmins.b	%r12, %r61, %r55
    vmins.h	%r18, %r61, %r45
    vmins.w	%r27, %r61, %r35
    vmins.d	%r36, %r61, %r25

    vminu.b	%r14, %r61, %r15
    vminu.h	%r15, %r62, %r75
    vminu.w	%r17, %r63, %r85
    vminu.d	%r16, %r64, %r75

    vmaxs.b	%r26, %r71, %r85
    vmaxs.h	%r26, %r61, %r54
    vmaxs.w	%r16, %r51, %r35
    vmaxs.d	%r16, %r41, %r55

    vmaxu.b	%r11, %r61, %r53
    vmaxu.h	%r12, %r55, %r55
    vmaxu.w	%r16, %r46, %r56
    vmaxu.d	%r13, %r31, %r55

    vrol.b	%r56, %r61, %r15
    vrol.h	%r31, %r61, %r25
    vrol.w	%r53, %r61, %r30
    vrol.d	%r62, %r61, %r41

    vror.b	%r16, %r11, %r52
    vror.h	%r11, %r21, %r63
    vror.w	%r71, %r31, %r74
    vror.d	%r81, %r41, %r85

    vsll.b	%r16, %r51, %r86
    vsll.h	%r24, %r61, %r55
    vsll.w	%r69, %r71, %r55
    vsll.d	%r77, %r81, %r55

    vsrl.b	%r21, %r81, %r50
    vsrl.h	%r12, %r63, %r51
    vsrl.w	%r13, %r62, %r52
    vsrl.d	%r64, %r63, %r53

    vsra.b	%r85, %r64, %r54
    vsra.h	%r76, %r65, %r15
    vsra.w	%r67, %r66, %r25
    vsra.d	%r58, %r67, %r36

    vavgs.b	%r49, %r68, %r47
    vavgs.h	%r30, %r69, %r58
    vavgs.w	%r26, %r11, %r69
    vavgs.d	%r16, %r21, %r75

    vavgu.b	%r14, %r31, %r85
    vavgu.h	%r15, %r41, %r45
    vavgu.w	%r56, %r51, %r25
    vavgu.d	%r87, %r61, %r15

    vaddss.b	%r42, %r71, %r15
    vaddss.h	%r83, %r81, %r45
    vaddss.w	%r74, %r41, %r85
    vaddss.d	%r65, %r61, %r75

    vaddu.b	%r56, %r61, %r75
    vaddu.h	%r47, %r61, %r65
    vaddu.w	%r38, %r61, %r55
    vaddu.d	%r29, %r61, %r55

    vaddus.b	%r55, %r61, %r45
    vaddus.h	%r65, %r61, %r35
    vaddus.w	%r74, %r61, %r25
    vaddus.d	%r84, %r61, %r15

    vaddc.b	%r53, %r61, %r55
    vaddc.h	%r13, %r61, %r55
    vaddc.w	%r12, %r61, %r55
    vaddc.d	%r12, %r61, %r55

    vsubss.b	%r56, %r61, %r15
    vsubss.h	%r67, %r61, %r12
    vsubss.w	%r78, %r61, %r13
    vsubss.d	%r89, %r61, %r45

    vsubu.b	%r70, %r61, %r85
    vsubu.h	%r86, %r61, %r45
    vsubu.w	%r46, %r61, %r13
    vsubu.d	%r46, %r61, %r75

    vsubus.b	%r41, %r68, %r65
    vsubus.h	%r12, %r37, %r55
    vsubus.w	%r23, %r26, %r45
    vsubus.d	%r14, %r18, %r35

    vcmp.eq.b	%r86, %r61, %r25
    vcmp.eq.h	%r44, %r72, %r15
    vcmp.eq.w	%r20, %r83, %r55
    vcmp.eq.d	%r16, %r84, %r55

;	pcmpne	%r106, %r61, %r55
;	pcmpgt	%r106, %r61, %r55
;	pcmpge	%r106, %r61, %r55
;	pcmple	%r106, %r61, %r55

    vcmp.lt.b	%r13, %r61, %r15
    vcmp.lt.h	%r14, %r61, %r24
    vcmp.lt.w	%r15, %r61, %r38
    vcmp.lt.d	%r16, %r61, %r45

    vcmp.ltu.b	%r19, %r11, %r75
    vcmp.ltu.h	%r18, %r21, %r82
    vcmp.ltu.w	%r16, %r31, %r73
    vcmp.ltu.d	%r14, %r71, %r54

    vmrg.h.b	%r11, %r71, %r13
    vmrg.h.h	%r72, %r67, %r27
    vmrg.h.w	%r13, %r58, %r55
    vmrg.h.d	%r14, %r69, %r15

    vmrg.l.b	%r76, %r61, %r11
    vmrg.l.h	%r26, %r11, %r62
    vmrg.l.w	%r16, %r15, %r73
    vmrg.l.d	%r16, %r11, %r85

    write	"end simd(int) test"
endmmx:

.end
