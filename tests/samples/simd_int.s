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
    lda_r %r4, mmxdata
    ld_i128	%r1, %r4,0*16
    ld_i128	%r2, %r4,1*16
    ld_i128	%r3, %r4,2*16
    ld_i128	%r4, %r4,3*16
    write	"r1      %x128(r1)"
    write	"r2      %x128(r2)"

    write	"%vu8(r1)"
    write	"%vu16(r1)"
    write	"%vu32(r1)"
    write	"%vu64(r1)"

    add_vu8 %r3, %r1, %r2
    write	"test vadd/vaddc (1 byte)\0"
    addc_vu8 %r4, %r1, %r2
    write	"%vu8(r1)"
    write	"%vu16(r2)"
    write	"%vu32(r3)"
    write	"%vu64(r4)"
    write	"test vadd/vaddo signed (1 byte)\0"
    addo_vi8 %r4, %r1, %r2
    write	"%vi8(r1)"
    write	"%vi16(r2)"
    write	"%vi32(r3)"
    write	"%vu64(r4)"

    sub_vu8 %r3, %r1, %r2
    write	"test vsub/vsubb (1 byte)\0"
    subb_vu8 %r4, %r1, %r2
    write	"%vu8(r1)"
    write	"%vu8(r2)"
    write	"%vu8(r3)"
    write	"%vu8(r4)"
    write	"test vsub/vsubo signed (1 byte)\0"
    subo_vi8 %r4, %r1, %r2
    write	"%vi8(r1)"
    write	"%vi8(r2)"
    write	"%vi8(r3)"
    write	"%vu8(r4)"

    write	"test vaddusb"
    add_vu8 %r3, %r1, %r2
    add_sat_vu8 %r4, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

    write	"test vsubusb"
    sub_vu8 %r3, %r1, %r2
    sub_sat_vu8 %r4, %r1, %r2
    write	"%vu8(r1):\n%vu8(r2)\n%vu8(r3)\n%vu8(r4)"

    write	"test vaddssb"
    add_vu8 %r3, %r1, %r2
    add_sat_vi8 %r4, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

    write	"test vsubssb"
    sub_vu8 %r3, %r1, %r2
    sub_sat_vi8 %r4, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)\n%vi8(r4)"

    write	"test pavgu (1 byte)\0"
    avg_vu8 %r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test pavgs (1 byte)\0"
    avg_vi8 %r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test vminu (1 byte)\0"
    min_vu8 %r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test vmins (1 byte)\0"
    min_vi8 %r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test vmaxu (1 byte)\0"
    max_vu8 %r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test vmaxs (1 byte)\0"
    max_vi8 %r3, %r1, %r2
    write	"%vi8(r1)\n%vi8(r2)\n%vi8(r3)"

    write	"test merge low (1 byte)\0"
    merge_low_vu8 %r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    write	"test merge high (1 byte)\0"
    merge_high_vu8 %r3, %r1, %r2
    write	"%vu8(r1)\n%vu8(r2)\n%vu8(r3)"

    pack_usat_vi16 %r2, %r3, %r4
    pack_sat_vu16 %r2, %r3, %r4
    pack_sat_vi16 %r2, %r3, %r4

    pack_usat_vi32 %r2, %r3, %r4
    pack_sat_vu32 %r2, %r3, %r4
    pack_sat_vi32 %r2, %r3, %r4

    pack_usat_vi64 %r2, %r3, %r4
    pack_sat_vu64 %r2, %r3, %r4
    pack_sat_vi64 %r2, %r3, %r4

;	jmp	endmmx
; d1 abs
    min_vi8  %r12, %r61, %r55
    min_vi16 %r18, %r61, %r45
    min_vi32 %r27, %r61, %r35
    min_vi64 %r36, %r61, %r25

    min_vu8  %r14, %r61, %r15
    min_vu16 %r15, %r62, %r75
    min_vu32 %r17, %r63, %r85
    min_vu64 %r16, %r64, %r75

    max_vi8 %r26, %r71, %r85
    max_vi16 %r26, %r61, %r54
    max_vi32 %r16, %r51, %r35
    max_vi64 %r16, %r41, %r55

    max_vu8  %r11, %r61, %r53
    max_vu16 %r12, %r55, %r55
    max_vu32 %r16, %r46, %r56
    max_vu64 %r13, %r31, %r55

    rol_vu8  %r56, %r61, %r15
    rol_vu16 %r31, %r61, %r25
    rol_vu32 %r53, %r61, %r30
    rol_vu64 %r62, %r61, %r41

    ror_vu8  %r16, %r11, %r52
    ror_vu16 %r11, %r21, %r63
    ror_vu32 %r71, %r31, %r74
    ror_vu64 %r81, %r41, %r85

    sll_vu8  %r16, %r51, %r86
    sll_vu16 %r24, %r61, %r55
    sll_vu32 %r69, %r71, %r55
    sll_vu64 %r77, %r81, %r55

    srl_vu8  %r21, %r81, %r50
    srl_vu16 %r12, %r63, %r51
    srl_vu32 %r13, %r62, %r52
    srl_vu64 %r64, %r63, %r53

    sra_vi8  %r85, %r64, %r54
    sra_vi16 %r76, %r65, %r15
    sra_vi32 %r67, %r66, %r25
    sra_vi64 %r58, %r67, %r36

    avg_vi8 %r49, %r68, %r47
    avg_vi16 %r30, %r69, %r58
    avg_vi32 %r26, %r11, %r69
    avg_vi64 %r16, %r21, %r75

    avg_vu8  %r14, %r31, %r85
    avg_vu16 %r15, %r41, %r45
    avg_vu32 %r56, %r51, %r25
    avg_vu64 %r87, %r61, %r15

    add_sat_vi8  %r42, %r71, %r15
    add_sat_vi16 %r83, %r81, %r45
    add_sat_vi32 %r74, %r41, %r85
    add_sat_vi64 %r65, %r61, %r75

    add_vu8  %r56, %r61, %r75
    add_vu16 %r47, %r61, %r65
    add_vu32 %r38, %r61, %r55
    add_vu64 %r29, %r61, %r55

    add_sat_vu8  %r55, %r61, %r45
    add_sat_vu16 %r65, %r61, %r35
    add_sat_vu32 %r74, %r61, %r25
    add_sat_vu64 %r84, %r61, %r15

    addc_vu8  %r53, %r61, %r55
    addc_vu16 %r13, %r61, %r55
    addc_vu32 %r12, %r61, %r55
    addc_vu64 %r12, %r61, %r55

    sub_sat_vi8  %r56, %r61, %r15
    sub_sat_vi16 %r67, %r61, %r12
    sub_sat_vi32 %r78, %r61, %r13
    sub_sat_vi64 %r89, %r61, %r45

    sub_vu8  %r70, %r61, %r85
    sub_vu16 %r86, %r61, %r45
    sub_vu32 %r46, %r61, %r13
    sub_vu64 %r46, %r61, %r75

    sub_sat_vu8  %r41, %r68, %r65
    sub_sat_vu16 %r12, %r37, %r55
    sub_sat_vu32 %r23, %r26, %r45
    sub_sat_vu64 %r14, %r18, %r35

    cmp_eq_vi8  %r86, %r61, %r25
    cmp_eq_vi16 %r44, %r72, %r15
    cmp_eq_vi32 %r20, %r83, %r55
    cmp_eq_vi64 %r16, %r84, %r55

    cmp_lt_vi8  %r13, %r61, %r15
    cmp_lt_vi16 %r14, %r61, %r24
    cmp_lt_vi32 %r15, %r61, %r38
    cmp_lt_vi64 %r16, %r61, %r45

    cmp_lt_vu8  %r19, %r11, %r75
    cmp_lt_vu16 %r18, %r21, %r82
    cmp_lt_vu32 %r16, %r31, %r73
    cmp_lt_vu64 %r14, %r71, %r54

    merge_high_vu8  %r11, %r71, %r13
    merge_high_vu16 %r72, %r67, %r27
    merge_high_vu32 %r13, %r58, %r55
    merge_high_vu64 %r14, %r69, %r15

    merge_low_vu8  %r76, %r61, %r11
    merge_low_vu16 %r26, %r11, %r62
    merge_low_vu32 %r16, %r15, %r73
    merge_low_vu64 %r16, %r11, %r85

    unpack_low_vi8   %r76, %r61
    unpack_low_vi16  %r76, %r61
    unpack_low_vi32  %r76, %r61
    unpack_low_vu8   %r76, %r61
    unpack_low_vu16  %r76, %r61
    unpack_low_vu32  %r76, %r61

    unpack_high_vi8   %r76, %r61
    unpack_high_vi16  %r76, %r61
    unpack_high_vi32  %r76, %r61
    unpack_high_vu8   %r76, %r61
    unpack_high_vu16  %r76, %r61
    unpack_high_vu32  %r76, %r61

    write "end simd(int) test"
endmmx:

.end
