.text
; floating-point extension example
	alloc	r95, skipfma

	write	"test float128 immediate load (low/high parts)"
	fldqi	r12, 3.1415926115461431423612436243
	fldqih	r12, 3.1415926115461431423612436243
	write	"fldqih: %f128(r12)"

	write	"test fpcr modification (127,100)"
	ldi	r1, 127
	ldi	r2, 100
	sprsr	r1, fpcr,r1,r2
	write	"fpcr: %s(fpcr)"
	write	"test fpcr modification (0,60)"
	ldi	r1, 0
	ldi	r2, 60
	sprsr	r1, fpcr,r1,r2
	write	"fpcr: %s(fpcr)"
	write	"test fpcr modification (127,80)"
	ldi	r1, 127
	ldi	r2, 80
	sprsr	r1, fpcr,r1,r2
	write	"fpcr: %s(fpcr)"
	write	"test fpcr modification (0,90)"
	ldi	r1, 0
	ldi	r2, 90
	sprsr	r1, fpcr,r1,r2
	write	"fpcr: %s(fpcr)"

	write	"compare fldqri (full mantissa) & long fldi (63-bit mantissa)"
	fldqri	r30, 3.14159265358979323846123456789012e+400
	write	"fldqri: %x128(r30) %f128(r30)"
	flddi	r31, 3.14159265358979323846123456789012
	write	"flddi: %x128(r31) %f64(r31)"
	write	"compare fldqri (full mantissa) & short fldi (21-bit mantissa)"
	fldqri	r30, 3.14159265358979323846123456789012
	write	"r30     %x128(r30)"
	flddi	r31, 3.14159265358979323846123456789012
	write	"r31     %x128(r31)"
	write	"before1"
	write	"r30     %f128(r30)"
	write	"before2"
	write	"r31     %vf64(r31)"
	write	"after"
	flddi	r30, -12.3456789e+04
.rodata
	align	16
float64data:
	double	1.234567890123456789124141241241
	double	3.1415925678888734535345231234564561
	double	3.4566345634563456346535463463456
.text
	ldar	r21, float64data
	ld8	r11, r21,8*0
	ld8	r12, r21,8*1
	ld8	r13, r21,8*2
	write	"ld8(f64): %f64(r11) %f64(r12) %f64(r13)"
	fldqri	r14, 2.7182818289201
	write	"fldqri: %f128(r14)"

	cvtsd2sq	r11, r11,0
	cvtsd2sq	r12, r12,0
	cvtsd2sq	r13, r13,0

        write	"test binary"
	fmulsq	r15, r11, r14
	write	"fmulsq:  %f128(r15)"
	fnmulsq	r15, r11, r14
	write	"fnmulsq: %f128(r15)"
	faddsq	r15, r11, r14
	write	"faddsq:  %f128(r15)"
	fnaddsq	r15, r11, r14
	write	"fnaddsq: %f128(r15)"
	fsubsq	r15, r14, r11
	write	"fsubsq:  %f128(r15)"
	fdivsq	r15, r14, r11
	write	"fdivsq:  %f128(r15)"

        write	"test fused fma"
;	br	skipfma
	fmaddsq	r15, r14, r11, r12
	write	"fmaddsq:  %f128(r15)"
	fnmaddsq	r15, r14, r11, r12
	write	"fnmaddsq: %f128(r15)"
	fmsubsq	r15, r14, r11, r12
	write	"fmsubsq:  %f128(r15)"
	fnmsubsq	r15, r14, r11, r12
	write	"fnmsubsq: %f128(r15)"

        write	"test unary"
	mov	r16, r15
	write	"r16     %f128(r16)"
	fabssq	r16, r15
	write	"r16     %f128(r16)"
	fnegsq	r16, r15
	write	"r16     %f128(r16)"
	fnabssq	r16, r15
	write	"r16     %f128(r16)"
	fsqrtsq	r16, r12
	write	"r16     %f128(r16)"
	frsqrtsq	r16, r12
	write	"r16     %f128(r16)"

        write	"test rounding"
	frndsq	r17, r12,4
	write	"r17     %f128(r17)"
	frndsq	r17, r12,3
	write	"r17     %f128(r17)"
	frndsq	r17, r12,2
	write	"r17     %f128(r17)"
	frndsq	r17, r12,0
	write	"r17     %f128(r17)"
	cvtsq2i	r17, r12,0
	write	"r17     %i64(r17)"
	ldi	r17, 123456
	cvti2sq	r17, r7,0
	write	"r17     %f128(r17)"

	write	"test fp minmax"
	fmaxsq	r8, r11, r12
	write	"r8      %f128(r8)"
	fminsq	r8, r11, r12
	write	"r8      %f128(r8)"
	write	"test fp abs minmax"
	famaxsq	r8, r11, r12
	write	"r8      %f128(r8)"
	faminsq	r8, r11, r12
	write	"r8      %f128(r8)"

	write	"test fmergesq"
	fmergesq	r8, r11, r12, r14
	write	"r8      %f128(r8)"
	fmergesq	r8, r14, r11, r12
	write	"r8      %f128(r8)"


.rodata
	align	16
xxxd:	double	1.122
	double	0.9999765432
.text
	ldar	r21, xxxd
	ldi	r15, 100
	ld8	r25, r21,8*0
	ld8	r26, r21,8*1
	fsubsq	r22, r25, r16
	write	"r22     %f128(r22)"
xxloop:
	fmaddsq	r22, r25, r16, r22
	fmsubsq	r22, r25, r16, r22
	repge	r15, gz, -1, xxloop
	write	"r22     %f128(r22)"

	write	"other FPU"
	fmaddsq	r60, r61, r62, r63
	fmsubsq	r61, r61, r72, r73
	fnmaddsq	r62, r71, r82, r63
	fnmsubsq	r63, r81, r12, r53

	fmulsq	r64, r61, r22
	fdivsq	r65, r11, r27
	faddsq	r66, r17, r42
	fsubsq	r67, r31, r23
	fnaddsq	r68, r41, r62
	fmaxsq	r60, r61, r62
	fminsq	r60, r61, r62
	famaxsq	r60, r61, r62
	faminsq	r60, r61, r62

	fcmpsq	r10, r61, r72, LT
	fcmpsq	r11, r52, r21, LT | EQ
	fcmpsq	r12, r43, r12, LT | EQ
	fcmpsq	r10, r34, r44, LT | EQ
	fcmpsq	r13, r25, r22, LT | EQ | UN
	fcmpsq	r12, r15, r23, LT | EQ | UN | SIG
	fcmpsq	r11, r86, r86, UN | SIG

	fnegsq	r24, r58
	fabsdsq	r45, r61, r20
	fnabsdsq	r56, r32, r20
	frndsq	r78, r74,2
	frndsq	r89, r65,3
	frndsq	r81, r76,0
	frndsq	r62, r67,1
	fsqrtsq	r63, r78
	frsqrtsq	r64, r69

	addi	r45, sp,-4800
	ldi	r13, 2

	ld4	r12, r45,4*1
	st4	r12, r45,4*1
	ld8	r12, r45,8*3
	st8	r12, r45,8*3
	ld4x	r12, r45,r13,2,200
	st4x	r12, r45,r13,2,200
	ld8x	r12, r45,r13,3,200
	st8x	r12, r45,r13,3,200

	faddsq	r23, r24, r25
	fmaddsq	r23, r60, r55, r33
	fmulsq	r23, r60, r55
	ld8	r60, r45,8*6
	fmaddsq	r23, r60, r55, r33
	fmaddsq	r24, r61, r25, r32
	fmaddsq	r25, r62, r55, r23
	fmaddsq	r26, r63, r75, r73
	fmaddsq	r27, r64, r75, r73
	fmaddsq	r28, r65, r85, r63
	fmaddsq	r29, r66, r85, r63
	fmaddsq	r30, r67, r55, r23
	fmaddsq	r31, r68, r55, r23
	fmaddsq	r12, r32, r76, r85
	fmaddsq	r12, r32, r76, r85
	fmaddsq	r10, r32, r76, r85
	fmaddsq	r10, r32, r76, r85
	fmaddsq	r10, r32, r76, r85
	fmaddsq	r13, r32, r76, r85
	fmaddsq	r14, r32, r76, r85
	fmaddsq	r15, r32, r76, r85
	fmaddsq	r16, r32, r76, r85
	fmaddsq	r17, r32, r76, r85

	cvtsq2i	r56, r45,0
	cvtsq2u	r56, r45,0
	cvti2sq	r45, r56,0
	cvtu2sq	r45, r56,0

	ldi	r5, 0
	fldqri	r4, 1.0
	fldqri	r5, 1.0
	fldqri	r6, 1.0
	fldqri	r7, 1.0
	ldi	r24, 128
tri_repeat:
	write	"r7      %x128(r7)"
	faddsq	r5, r5, r4
	fmulsq	r6, r6, r5
	fdivsq	r7, r4, r6
;	write "%x128(r6)"
	reple.l	r5, r24, 1, tri_repeat

	write	"test taylor series"
	fldqri	r2, 0.44567	; f2 ,  x
	write	"x:   %f128(r2)"		; test value
	write	"test sin(x)"
	fldqri	r5, sin(0.44567)
	write	"sin: %f128(r5)"		; test value
	ldi	r3, 0		; s ,  0
	fmulsq	r4, r2, r2	; f4 ,  x*x
	fmaddsq	r3, r3, r4, r25	; s ,  s * x*x + 1/25!
	fmsubsq	r3, r3, r4, r23	; s ,  s * x*x - 1/23!
	fmaddsq	r3, r3, r4, r21
	fmsubsq	r3, r3, r4, r19
	fmaddsq	r3, r3, r4, r17
	fmsubsq	r3, r3, r4, r15
	fmaddsq	r3, r3, r4, r13
	fmsubsq	r3, r3, r4, r11
	fmaddsq	r3, r3, r4, r9
	fmsubsq	r3, r3, r4, r7
	fmaddsq	r3, r3, r4, r5
	fmsubsq	r3, r3, r4, r3
	fmaddsq	r3, r3, r4, r1
	fmulsq	r3, r3, r2	; s ,  s * x
	write	"sin: %f128(r3)"

	write	"test cos(x)"
	fldqri	r5, cos(0.44567)
	write	"cos: %f128(r5)"		; test value
	ldi	r3, 0		; s ,  0
	fmulsq	r4, r2, r2	; f4 ,  x*x
	fmsubsq	r3, r3, r4, r26
	fmaddsq	r3, r3, r4, r24
	fmsubsq	r3, r3, r4, r22
	fmaddsq	r3, r3, r4, r20
	fmsubsq	r3, r3, r4, r18
	fmaddsq	r3, r3, r4, r16
	fmsubsq	r3, r3, r4, r14
	fmaddsq	r3, r3, r4, r12
	fmsubsq	r3, r3, r4, r10
	fmaddsq	r3, r3, r4, r8
	fmsubsq	r3, r3, r4, r6
	fmaddsq	r3, r3, r4, r4
	fmsubsq	r3, r3, r4, r2
	fmaddsq	r3, r3, r4, r1
	write	"cos: %f128(r3)"

	write	"test exp(x)"
	fldqri	r5, exp(0.44567)
	write	"exp: %f128(r5)"	; test value
	ldi	r3, 0		; s ,  0.0
	mov	r4, r2		; f4 ,  x
	flddi	r6, 0.125
;	write	"%f128(r6)"
	fmulsq	r4, r4, r6	; x ,  x/8
	fmaddsq	r3, r3, r4, r15
	fmaddsq	r3, r3, r4, r14
	fmaddsq	r3, r3, r4, r13
	fmaddsq	r3, r3, r4, r12
	fmaddsq	r3, r3, r4, r11
	fmaddsq	r3, r3, r4, r10
	fmaddsq	r3, r3, r4, r9
	fmaddsq	r3, r3, r4, r8
	fmaddsq	r3, r3, r4, r7
	fmaddsq	r3, r3, r4, r6
	fmaddsq	r3, r3, r4, r5
	fmaddsq	r3, r3, r4, r4
	fmaddsq	r3, r3, r4, r3
	fmaddsq	r3, r3, r4, r2
	fmaddsq	r3, r3, r4, r1
	fmaddsq	r3, r3, r4, r1
	fmulsq	r3, r3, r3	; (e^x) ^ 8
	fmulsq	r3, r3, r3
	fmulsq	r3, r3, r3
	write	"exp: %f128(r3)"

	faddsq	r1, r2, r3
	fmaddsq	r2, r10, r20, r30
	fmaddsq	r1, r11, r21, r31

skipfma:
	write	"end fpu"

.end
