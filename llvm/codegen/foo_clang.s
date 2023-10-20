	.text
	.file	"foo.temp.c"
	.globl	test_i64_abs                    ! -- Begin function test_i64_abs
	.p2align	4
	.type	test_i64_abs,@function
test_i64_abs:                           ! @test_i64_abs
.Ltest_i64_abs$local:
	.type	.Ltest_i64_abs$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	absd	%r1, %r1, %gz
	retf	0

.Lfunc_end0:
	.size	test_i64_abs, .Lfunc_end0-test_i64_abs
	.size	.Ltest_i64_abs$local, .Lfunc_end0-test_i64_abs
                                        ! -- End function
	.globl	test_i32_abs                    ! -- Begin function test_i32_abs
	.p2align	4
	.type	test_i32_abs,@function
test_i32_abs:                           ! @test_i32_abs
.Ltest_i32_abs$local:
	.type	.Ltest_i32_abs$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	absdw	%r1, %r1, %gz
	retf	0

.Lfunc_end1:
	.size	test_i32_abs, .Lfunc_end1-test_i32_abs
	.size	.Ltest_i32_abs$local, .Lfunc_end1-test_i32_abs
                                        ! -- End function
	.globl	test_i64_abs_difference         ! -- Begin function test_i64_abs_difference
	.p2align	4
	.type	test_i64_abs_difference,@function
test_i64_abs_difference:                ! @test_i64_abs_difference
.Ltest_i64_abs_difference$local:
	.type	.Ltest_i64_abs_difference$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	absd	%r1, %r2, %r1
	retf	0

.Lfunc_end2:
	.size	test_i64_abs_difference, .Lfunc_end2-test_i64_abs_difference
	.size	.Ltest_i64_abs_difference$local, .Lfunc_end2-test_i64_abs_difference
                                        ! -- End function
	.globl	test_i32_abs_difference         ! -- Begin function test_i32_abs_difference
	.p2align	4
	.type	test_i32_abs_difference,@function
test_i32_abs_difference:                ! @test_i32_abs_difference
.Ltest_i32_abs_difference$local:
	.type	.Ltest_i32_abs_difference$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	absdw	%r1, %r2, %r1
	retf	0

.Lfunc_end3:
	.size	test_i32_abs_difference, .Lfunc_end3-test_i32_abs_difference
	.size	.Ltest_i32_abs_difference$local, .Lfunc_end3-test_i32_abs_difference
                                        ! -- End function
	.globl	test_aes                        ! -- Begin function test_aes
	.p2align	4
	.type	test_aes,@function
test_aes:                               ! @test_aes
.Ltest_aes$local:
	.type	.Ltest_aes$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	aesimc	%r2, %r1
	aeskeygenassist	%r2, %r2, 100

	! BUNDLE
	aeskeygenassist	%r2, %r2, -1
	aesenc	%r2, %r1, %r2
	aesdec	%r2, %r1, %r2

	! BUNDLE
	aesenclast	%r2, %r1, %r2
	aesdeclast	%r1, %r1, %r2
	retf	0

.Lfunc_end4:
	.size	test_aes, .Lfunc_end4-test_aes
	.size	.Ltest_aes$local, .Lfunc_end4-test_aes
                                        ! -- End function
	.globl	test_ldi32                      ! -- Begin function test_ldi32
	.p2align	4
	.type	test_ldi32,@function
test_ldi32:                             ! @test_ldi32
.Ltest_ldi32$local:
	.type	.Ltest_ldi32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 12345778
	retf	0

.Lfunc_end5:
	.size	test_ldi32, .Lfunc_end5-test_ldi32
	.size	.Ltest_ldi32$local, .Lfunc_end5-test_ldi32
                                        ! -- End function
	.globl	test_ldi32_long                 ! -- Begin function test_ldi32_long
	.p2align	4
	.type	test_ldi32_long,@function
test_ldi32_long:                        ! @test_ldi32_long
.Ltest_ldi32_long$local:
	.type	.Ltest_ldi32_long$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, 987654321

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end6:
	.size	test_ldi32_long, .Lfunc_end6-test_ldi32_long
	.size	.Ltest_ldi32_long$local, .Lfunc_end6-test_ldi32_long
                                        ! -- End function
	.globl	test_ldi64                      ! -- Begin function test_ldi64
	.p2align	4
	.type	test_ldi64,@function
test_ldi64:                             ! @test_ldi64
.Ltest_ldi64$local:
	.type	.Ltest_ldi64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 12345778
	retf	0

.Lfunc_end7:
	.size	test_ldi64, .Lfunc_end7-test_ldi64
	.size	.Ltest_ldi64$local, .Lfunc_end7-test_ldi64
                                        ! -- End function
	.globl	test_ldi64_long                 ! -- Begin function test_ldi64_long
	.p2align	4
	.type	test_ldi64_long,@function
test_ldi64_long:                        ! @test_ldi64_long
.Ltest_ldi64_long$local:
	.type	.Ltest_ldi64_long$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, 987654321123456789

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end8:
	.size	test_ldi64_long, .Lfunc_end8-test_ldi64_long
	.size	.Ltest_ldi64_long$local, .Lfunc_end8-test_ldi64_long
                                        ! -- End function
	.globl	test_subfi_i32                  ! -- Begin function test_subfi_i32
	.p2align	4
	.type	test_subfi_i32,@function
test_subfi_i32:                         ! @test_subfi_i32
.Ltest_subfi_i32$local:
	.type	.Ltest_subfi_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	subfi	%r1, %r1, 1234
	retf	0

.Lfunc_end9:
	.size	test_subfi_i32, .Lfunc_end9-test_subfi_i32
	.size	.Ltest_subfi_i32$local, .Lfunc_end9-test_subfi_i32
                                        ! -- End function
	.globl	test_subfi_i64                  ! -- Begin function test_subfi_i64
	.p2align	4
	.type	test_subfi_i64,@function
test_subfi_i64:                         ! @test_subfi_i64
.Ltest_subfi_i64$local:
	.type	.Ltest_subfi_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	subfiws	%r1, %r1, 1234
	retf	0

.Lfunc_end10:
	.size	test_subfi_i64, .Lfunc_end10-test_subfi_i64
	.size	.Ltest_subfi_i64$local, .Lfunc_end10-test_subfi_i64
                                        ! -- End function
	.globl	test_addil                      ! -- Begin function test_addil
	.p2align	4
	.type	test_addil,@function
test_addil:                             ! @test_addil
.Ltest_addil$local:
	.type	.Ltest_addil$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	addi.l	%r1, %r1, 123456789123456789

	! BUNDLE
	ldi.l	%r2, 9200848539817279407
	or	%r1, %r1, %r2

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end11:
	.size	test_addil, .Lfunc_end11-test_addil
	.size	.Ltest_addil$local, .Lfunc_end11-test_addil
                                        ! -- End function
	.globl	test_andil                      ! -- Begin function test_andil
	.p2align	4
	.type	test_andil,@function
test_andil:                             ! @test_andil
.Ltest_andil$local:
	.type	.Ltest_andil$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi.l	%r1, %r1, 123456789123456789

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end12:
	.size	test_andil, .Lfunc_end12-test_andil
	.size	.Ltest_andil$local, .Lfunc_end12-test_andil
                                        ! -- End function
	.globl	test_oril                       ! -- Begin function test_oril
	.p2align	4
	.type	test_oril,@function
test_oril:                              ! @test_oril
.Ltest_oril$local:
	.type	.Ltest_oril$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ori.l	%r1, %r1, 987654321123456789

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end13:
	.size	test_oril, .Lfunc_end13-test_oril
	.size	.Ltest_oril$local, .Lfunc_end13-test_oril
                                        ! -- End function
	.globl	test_addadd                     ! -- Begin function test_addadd
	.p2align	4
	.type	test_addadd,@function
test_addadd:                            ! @test_addadd
.Ltest_addadd$local:
	.type	.Ltest_addadd$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	addadd	%r1, %r2, %r1, %r3
	retf	0

.Lfunc_end14:
	.size	test_addadd, .Lfunc_end14-test_addadd
	.size	.Ltest_addadd$local, .Lfunc_end14-test_addadd
                                        ! -- End function
	.globl	test_addsub                     ! -- Begin function test_addsub
	.p2align	4
	.type	test_addsub,@function
test_addsub:                            ! @test_addsub
.Ltest_addsub$local:
	.type	.Ltest_addsub$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	addsub	%r1, %r2, %r1, %r3
	retf	0

.Lfunc_end15:
	.size	test_addsub, .Lfunc_end15-test_addsub
	.size	.Ltest_addsub$local, .Lfunc_end15-test_addsub
                                        ! -- End function
	.globl	test_subsub                     ! -- Begin function test_subsub
	.p2align	4
	.type	test_subsub,@function
test_subsub:                            ! @test_subsub
.Ltest_subsub$local:
	.type	.Ltest_subsub$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	subsub	%r1, %r1, %r2, %r3
	retf	0

.Lfunc_end16:
	.size	test_subsub, .Lfunc_end16-test_subsub
	.size	.Ltest_subsub$local, .Lfunc_end16-test_subsub
                                        ! -- End function
	.globl	test_mul_i32_i32                ! -- Begin function test_mul_i32_i32
	.p2align	4
	.type	test_mul_i32_i32,@function
test_mul_i32_i32:                       ! @test_mul_i32_i32
.Ltest_mul_i32_i32$local:
	.type	.Ltest_mul_i32_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mulws	%r1, %r2, %r1
	retf	0

.Lfunc_end17:
	.size	test_mul_i32_i32, .Lfunc_end17-test_mul_i32_i32
	.size	.Ltest_mul_i32_i32$local, .Lfunc_end17-test_mul_i32_i32
                                        ! -- End function
	.globl	test_mul_i32_imm                ! -- Begin function test_mul_i32_imm
	.p2align	4
	.type	test_mul_i32_imm,@function
test_mul_i32_imm:                       ! @test_mul_i32_imm
.Ltest_mul_i32_imm$local:
	.type	.Ltest_mul_i32_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwsi	%r1, %r1, 98765
	retf	0

.Lfunc_end18:
	.size	test_mul_i32_imm, .Lfunc_end18-test_mul_i32_imm
	.size	.Ltest_mul_i32_imm$local, .Lfunc_end18-test_mul_i32_imm
                                        ! -- End function
	.globl	test_mul_i32_imm_ext            ! -- Begin function test_mul_i32_imm_ext
	.p2align	4
	.type	test_mul_i32_imm_ext,@function
test_mul_i32_imm_ext:                   ! @test_mul_i32_imm_ext
.Ltest_mul_i32_imm_ext$local:
	.type	.Ltest_mul_i32_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwsi.l	%r1, %r1, 98765432

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end19:
	.size	test_mul_i32_imm_ext, .Lfunc_end19-test_mul_i32_imm_ext
	.size	.Ltest_mul_i32_imm_ext$local, .Lfunc_end19-test_mul_i32_imm_ext
                                        ! -- End function
	.globl	test_mul_u32_imm                ! -- Begin function test_mul_u32_imm
	.p2align	4
	.type	test_mul_u32_imm,@function
test_mul_u32_imm:                       ! @test_mul_u32_imm
.Ltest_mul_u32_imm$local:
	.type	.Ltest_mul_u32_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwsi	%r1, %r1, 98765
	retf	0

.Lfunc_end20:
	.size	test_mul_u32_imm, .Lfunc_end20-test_mul_u32_imm
	.size	.Ltest_mul_u32_imm$local, .Lfunc_end20-test_mul_u32_imm
                                        ! -- End function
	.globl	test_mul_u32_imm_ext            ! -- Begin function test_mul_u32_imm_ext
	.p2align	4
	.type	test_mul_u32_imm_ext,@function
test_mul_u32_imm_ext:                   ! @test_mul_u32_imm_ext
.Ltest_mul_u32_imm_ext$local:
	.type	.Ltest_mul_u32_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwsi.l	%r1, %r1, 98765432

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end21:
	.size	test_mul_u32_imm_ext, .Lfunc_end21-test_mul_u32_imm_ext
	.size	.Ltest_mul_u32_imm_ext$local, .Lfunc_end21-test_mul_u32_imm_ext
                                        ! -- End function
	.globl	test_mul_i32_i64                ! -- Begin function test_mul_i32_i64
	.p2align	4
	.type	test_mul_i32_i64,@function
test_mul_i32_i64:                       ! @test_mul_i32_i64
.Ltest_mul_i32_i64$local:
	.type	.Ltest_mul_i32_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mulws	%r1, %r2, %r1
	retf	0

.Lfunc_end22:
	.size	test_mul_i32_i64, .Lfunc_end22-test_mul_i32_i64
	.size	.Ltest_mul_i32_i64$local, .Lfunc_end22-test_mul_i32_i64
                                        ! -- End function
	.globl	test_mul_u32_u64                ! -- Begin function test_mul_u32_u64
	.p2align	4
	.type	test_mul_u32_u64,@function
test_mul_u32_u64:                       ! @test_mul_u32_u64
.Ltest_mul_u32_u64$local:
	.type	.Ltest_mul_u32_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mulwz	%r1, %r2, %r1
	retf	0

.Lfunc_end23:
	.size	test_mul_u32_u64, .Lfunc_end23-test_mul_u32_u64
	.size	.Ltest_mul_u32_u64$local, .Lfunc_end23-test_mul_u32_u64
                                        ! -- End function
	.globl	test_mul_i32_u64                ! -- Begin function test_mul_i32_u64
	.p2align	4
	.type	test_mul_i32_u64,@function
test_mul_i32_u64:                       ! @test_mul_i32_u64
.Ltest_mul_i32_u64$local:
	.type	.Ltest_mul_i32_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mulws	%r1, %r2, %r1
	retf	0

.Lfunc_end24:
	.size	test_mul_i32_u64, .Lfunc_end24-test_mul_i32_u64
	.size	.Ltest_mul_i32_u64$local, .Lfunc_end24-test_mul_i32_u64
                                        ! -- End function
	.globl	test_mul_i32_i64_imm            ! -- Begin function test_mul_i32_i64_imm
	.p2align	4
	.type	test_mul_i32_i64_imm,@function
test_mul_i32_i64_imm:                   ! @test_mul_i32_i64_imm
.Ltest_mul_i32_i64_imm$local:
	.type	.Ltest_mul_i32_i64_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwsi	%r1, %r1, 98765
	retf	0

.Lfunc_end25:
	.size	test_mul_i32_i64_imm, .Lfunc_end25-test_mul_i32_i64_imm
	.size	.Ltest_mul_i32_i64_imm$local, .Lfunc_end25-test_mul_i32_i64_imm
                                        ! -- End function
	.globl	test_mul_i32_i64_imm_ext        ! -- Begin function test_mul_i32_i64_imm_ext
	.p2align	4
	.type	test_mul_i32_i64_imm_ext,@function
test_mul_i32_i64_imm_ext:               ! @test_mul_i32_i64_imm_ext
.Ltest_mul_i32_i64_imm_ext$local:
	.type	.Ltest_mul_i32_i64_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwsi.l	%r1, %r1, 98765432

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end26:
	.size	test_mul_i32_i64_imm_ext, .Lfunc_end26-test_mul_i32_i64_imm_ext
	.size	.Ltest_mul_i32_i64_imm_ext$local, .Lfunc_end26-test_mul_i32_i64_imm_ext
                                        ! -- End function
	.globl	test_mul_u32_u64_imm            ! -- Begin function test_mul_u32_u64_imm
	.p2align	4
	.type	test_mul_u32_u64_imm,@function
test_mul_u32_u64_imm:                   ! @test_mul_u32_u64_imm
.Ltest_mul_u32_u64_imm$local:
	.type	.Ltest_mul_u32_u64_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwzi	%r1, %r1, 98765
	retf	0

.Lfunc_end27:
	.size	test_mul_u32_u64_imm, .Lfunc_end27-test_mul_u32_u64_imm
	.size	.Ltest_mul_u32_u64_imm$local, .Lfunc_end27-test_mul_u32_u64_imm
                                        ! -- End function
	.globl	test_mul_u32_u64_imm_ext        ! -- Begin function test_mul_u32_u64_imm_ext
	.p2align	4
	.type	test_mul_u32_u64_imm_ext,@function
test_mul_u32_u64_imm_ext:               ! @test_mul_u32_u64_imm_ext
.Ltest_mul_u32_u64_imm_ext$local:
	.type	.Ltest_mul_u32_u64_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mulwzi.l	%r1, %r1, 98765432

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end28:
	.size	test_mul_u32_u64_imm_ext, .Lfunc_end28-test_mul_u32_u64_imm_ext
	.size	.Ltest_mul_u32_u64_imm_ext$local, .Lfunc_end28-test_mul_u32_u64_imm_ext
                                        ! -- End function
	.globl	test_mul_i64_i64                ! -- Begin function test_mul_i64_i64
	.p2align	4
	.type	test_mul_i64_i64,@function
test_mul_i64_i64:                       ! @test_mul_i64_i64
.Ltest_mul_i64_i64$local:
	.type	.Ltest_mul_i64_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mul	%r1, %r2, %r1
	retf	0

.Lfunc_end29:
	.size	test_mul_i64_i64, .Lfunc_end29-test_mul_i64_i64
	.size	.Ltest_mul_i64_i64$local, .Lfunc_end29-test_mul_i64_i64
                                        ! -- End function
	.globl	test_atomic_compare_exchange_i8 ! -- Begin function test_atomic_compare_exchange_i8
	.p2align	4
	.type	test_atomic_compare_exchange_i8,@function
test_atomic_compare_exchange_i8:        ! @test_atomic_compare_exchange_i8
.Ltest_atomic_compare_exchange_i8$local:
	.type	.Ltest_atomic_compare_exchange_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	7, 16
	stw	%gz, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.release	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casb.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0

	! BUNDLE
	fence.seq_cst
	mov	%r5, %r3
	casb.relaxed	%r5, %r1, %r6

	! BUNDLE
	cmpweq	%r4, %r5, %r6
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0

	! BUNDLE
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12

	! BUNDLE
	ldbz	%r6, %r2, 0
	fence.seq_cst
	mov	%r5, %r3

	! BUNDLE
	casb.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6
	nulweq	%r5, %r6, 1, 0

	! BUNDLE
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4

	! BUNDLE
	stw	%r4, %sp, 12
	ldbz	%r6, %r2, 0
	fence.seq_cst

	! BUNDLE
	mov	%r5, %r3
	casb.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stb	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldbz	%r4, %r2, 0

	! BUNDLE
	fence.seq_cst
	casb.relaxed	%r3, %r1, %r4
	cmpweq	%r1, %r3, %r4

	! BUNDLE
	nulweq	%r3, %r4, 1, 0
	stb	%r3, %r2, 0
	ldwz	%r2, %sp, 12

	! BUNDLE
	addw	%r1, %r2, %r1
	stw	%r1, %sp, 12
	ldwz	%r1, %sp, 12

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end30:
	.size	test_atomic_compare_exchange_i8, .Lfunc_end30-test_atomic_compare_exchange_i8
	.size	.Ltest_atomic_compare_exchange_i8$local, .Lfunc_end30-test_atomic_compare_exchange_i8
                                        ! -- End function
	.globl	test_atomic_compare_exchange_i16 ! -- Begin function test_atomic_compare_exchange_i16
	.p2align	4
	.type	test_atomic_compare_exchange_i16,@function
test_atomic_compare_exchange_i16:       ! @test_atomic_compare_exchange_i16
.Ltest_atomic_compare_exchange_i16$local:
	.type	.Ltest_atomic_compare_exchange_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	7, 16
	stw	%gz, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.release	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cash.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0

	! BUNDLE
	fence.seq_cst
	mov	%r5, %r3
	cash.relaxed	%r5, %r1, %r6

	! BUNDLE
	cmpweq	%r4, %r5, %r6
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0

	! BUNDLE
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12

	! BUNDLE
	ldhz	%r6, %r2, 0
	fence.seq_cst
	mov	%r5, %r3

	! BUNDLE
	cash.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6
	nulweq	%r5, %r6, 1, 0

	! BUNDLE
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4

	! BUNDLE
	stw	%r4, %sp, 12
	ldhz	%r6, %r2, 0
	fence.seq_cst

	! BUNDLE
	mov	%r5, %r3
	cash.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	sth	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldhz	%r4, %r2, 0

	! BUNDLE
	fence.seq_cst
	cash.relaxed	%r3, %r1, %r4
	cmpweq	%r1, %r3, %r4

	! BUNDLE
	nulweq	%r3, %r4, 1, 0
	sth	%r3, %r2, 0
	ldwz	%r2, %sp, 12

	! BUNDLE
	addw	%r1, %r2, %r1
	stw	%r1, %sp, 12
	ldwz	%r1, %sp, 12

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end31:
	.size	test_atomic_compare_exchange_i16, .Lfunc_end31-test_atomic_compare_exchange_i16
	.size	.Ltest_atomic_compare_exchange_i16$local, .Lfunc_end31-test_atomic_compare_exchange_i16
                                        ! -- End function
	.globl	test_atomic_compare_exchange_i32 ! -- Begin function test_atomic_compare_exchange_i32
	.p2align	4
	.type	test_atomic_compare_exchange_i32,@function
test_atomic_compare_exchange_i32:       ! @test_atomic_compare_exchange_i32
.Ltest_atomic_compare_exchange_i32$local:
	.type	.Ltest_atomic_compare_exchange_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	7, 16
	stw	%gz, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acquire	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.release	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casw.acq_rel	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0

	! BUNDLE
	fence.seq_cst
	mov	%r5, %r3
	casw.relaxed	%r5, %r1, %r6

	! BUNDLE
	cmpweq	%r4, %r5, %r6
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0

	! BUNDLE
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12

	! BUNDLE
	ldwz	%r6, %r2, 0
	fence.seq_cst
	mov	%r5, %r3

	! BUNDLE
	casw.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6
	nulweq	%r5, %r6, 1, 0

	! BUNDLE
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4

	! BUNDLE
	stw	%r4, %sp, 12
	ldwz	%r6, %r2, 0
	fence.seq_cst

	! BUNDLE
	mov	%r5, %r3
	casw.relaxed	%r5, %r1, %r6
	cmpweq	%r4, %r5, %r6

	! BUNDLE
	nulweq	%r5, %r6, 1, 0
	stw	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldwz	%r4, %r2, 0

	! BUNDLE
	fence.seq_cst
	casw.relaxed	%r3, %r1, %r4
	cmpweq	%r1, %r3, %r4

	! BUNDLE
	nulweq	%r3, %r4, 1, 0
	stw	%r3, %r2, 0
	ldwz	%r2, %sp, 12

	! BUNDLE
	addw	%r1, %r2, %r1
	stw	%r1, %sp, 12
	ldwz	%r1, %sp, 12

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end32:
	.size	test_atomic_compare_exchange_i32, .Lfunc_end32-test_atomic_compare_exchange_i32
	.size	.Ltest_atomic_compare_exchange_i32$local, .Lfunc_end32-test_atomic_compare_exchange_i32
                                        ! -- End function
	.globl	test_atomic_compare_exchange_i64 ! -- Begin function test_atomic_compare_exchange_i64
	.p2align	4
	.type	test_atomic_compare_exchange_i64,@function
test_atomic_compare_exchange_i64:       ! @test_atomic_compare_exchange_i64
.Ltest_atomic_compare_exchange_i64$local:
	.type	.Ltest_atomic_compare_exchange_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	7, 16
	stw	%gz, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.relaxed	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acquire	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acquire	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acquire	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acquire	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acquire	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acquire	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.release	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acq_rel	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acq_rel	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casd.acq_rel	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0

	! BUNDLE
	fence.seq_cst
	mov	%r5, %r3
	casd.relaxed	%r5, %r1, %r6

	! BUNDLE
	cmpdeq	%r4, %r5, %r6
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0

	! BUNDLE
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12

	! BUNDLE
	lddz	%r6, %r2, 0
	fence.seq_cst
	mov	%r5, %r3

	! BUNDLE
	casd.relaxed	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6
	nuldeq	%r5, %r6, 1, 0

	! BUNDLE
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4

	! BUNDLE
	stw	%r4, %sp, 12
	lddz	%r6, %r2, 0
	fence.seq_cst

	! BUNDLE
	mov	%r5, %r3
	casd.relaxed	%r5, %r1, %r6
	cmpdeq	%r4, %r5, %r6

	! BUNDLE
	nuldeq	%r5, %r6, 1, 0
	std	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	lddz	%r4, %r2, 0

	! BUNDLE
	fence.seq_cst
	casd.relaxed	%r3, %r1, %r4
	cmpdeq	%r1, %r3, %r4

	! BUNDLE
	nuldeq	%r3, %r4, 1, 0
	std	%r3, %r2, 0
	ldwz	%r2, %sp, 12

	! BUNDLE
	addw	%r1, %r2, %r1
	stw	%r1, %sp, 12
	ldwz	%r1, %sp, 12

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end33:
	.size	test_atomic_compare_exchange_i64, .Lfunc_end33-test_atomic_compare_exchange_i64
	.size	.Ltest_atomic_compare_exchange_i64$local, .Lfunc_end33-test_atomic_compare_exchange_i64
                                        ! -- End function
	.globl	test_atomic_compare_exchange_i128 ! -- Begin function test_atomic_compare_exchange_i128
	.p2align	4
	.type	test_atomic_compare_exchange_i128,@function
test_atomic_compare_exchange_i128:      ! @test_atomic_compare_exchange_i128
.Ltest_atomic_compare_exchange_i128$local:
	.type	.Ltest_atomic_compare_exchange_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	6, 16
	stw	%gz, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.relaxed	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acquire	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acquire	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acquire	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acquire	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acquire	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acquire	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.release	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acq_rel	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acq_rel	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	casq.acq_rel	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	fence.seq_cst
	mov	%r5, %r3
	casq.relaxed	%r5, %r1, %r4

	! BUNDLE
	cmpqeq	%r4, %r5, %r4
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0

	! BUNDLE
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12

	! BUNDLE
	ldq	%r4, %r2, 0
	fence.seq_cst
	mov	%r5, %r3

	! BUNDLE
	casq.relaxed	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4
	nulbsi	%r4, 0, 1, 0

	! BUNDLE
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12
	addw	%r4, %r5, %r4

	! BUNDLE
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0
	fence.seq_cst

	! BUNDLE
	mov	%r5, %r3
	casq.relaxed	%r5, %r1, %r4
	cmpqeq	%r4, %r5, %r4

	! BUNDLE
	nulbsi	%r4, 0, 1, 0
	stq	%r5, %r2, 0
	ldwz	%r5, %sp, 12

	! BUNDLE
	addw	%r4, %r5, %r4
	stw	%r4, %sp, 12
	ldq	%r4, %r2, 0

	! BUNDLE
	fence.seq_cst
	casq.relaxed	%r3, %r1, %r4
	cmpqeq	%r1, %r3, %r4

	! BUNDLE
	nulbsi	%r1, 0, 1, 0
	stq	%r3, %r2, 0
	ldwz	%r2, %sp, 12

	! BUNDLE
	addw	%r1, %r2, %r1
	stw	%r1, %sp, 12
	ldwz	%r1, %sp, 12

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end34:
	.size	test_atomic_compare_exchange_i128, .Lfunc_end34-test_atomic_compare_exchange_i128
	.size	.Ltest_atomic_compare_exchange_i128$local, .Lfunc_end34-test_atomic_compare_exchange_i128
                                        ! -- End function
	.globl	test_atomic_exchange_i8         ! -- Begin function test_atomic_exchange_i8
	.p2align	4
	.type	test_atomic_exchange_i8,@function
test_atomic_exchange_i8:                ! @test_atomic_exchange_i8
.Ltest_atomic_exchange_i8$local:
	.type	.Ltest_atomic_exchange_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	swapb.relaxed	%r1, %r2, %r1
	swapb.acquire	%r1, %r2, %r1

	! BUNDLE
	swapb.acquire	%r1, %r2, %r1
	swapb.release	%r1, %r2, %r1
	swapb.acq_rel	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	swapb.relaxed	%r1, %r2, %r1
	slsrai	%r1, %r1, 56, 56

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end35:
	.size	test_atomic_exchange_i8, .Lfunc_end35-test_atomic_exchange_i8
	.size	.Ltest_atomic_exchange_i8$local, .Lfunc_end35-test_atomic_exchange_i8
                                        ! -- End function
	.globl	test_atomic_exchange_i16        ! -- Begin function test_atomic_exchange_i16
	.p2align	4
	.type	test_atomic_exchange_i16,@function
test_atomic_exchange_i16:               ! @test_atomic_exchange_i16
.Ltest_atomic_exchange_i16$local:
	.type	.Ltest_atomic_exchange_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	swaph.relaxed	%r1, %r2, %r1
	swaph.acquire	%r1, %r2, %r1

	! BUNDLE
	swaph.acquire	%r1, %r2, %r1
	swaph.release	%r1, %r2, %r1
	swaph.acq_rel	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	swaph.relaxed	%r1, %r2, %r1
	slsrai	%r1, %r1, 48, 48

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end36:
	.size	test_atomic_exchange_i16, .Lfunc_end36-test_atomic_exchange_i16
	.size	.Ltest_atomic_exchange_i16$local, .Lfunc_end36-test_atomic_exchange_i16
                                        ! -- End function
	.globl	test_atomic_exchange_i32        ! -- Begin function test_atomic_exchange_i32
	.p2align	4
	.type	test_atomic_exchange_i32,@function
test_atomic_exchange_i32:               ! @test_atomic_exchange_i32
.Ltest_atomic_exchange_i32$local:
	.type	.Ltest_atomic_exchange_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	swapw.relaxed	%r1, %r2, %r1
	swapw.acquire	%r1, %r2, %r1

	! BUNDLE
	swapw.acquire	%r1, %r2, %r1
	swapw.release	%r1, %r2, %r1
	swapw.acq_rel	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	swapw.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end37:
	.size	test_atomic_exchange_i32, .Lfunc_end37-test_atomic_exchange_i32
	.size	.Ltest_atomic_exchange_i32$local, .Lfunc_end37-test_atomic_exchange_i32
                                        ! -- End function
	.globl	test_atomic_exchange_i64        ! -- Begin function test_atomic_exchange_i64
	.p2align	4
	.type	test_atomic_exchange_i64,@function
test_atomic_exchange_i64:               ! @test_atomic_exchange_i64
.Ltest_atomic_exchange_i64$local:
	.type	.Ltest_atomic_exchange_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	swapd.relaxed	%r1, %r2, %r1
	swapd.acquire	%r1, %r2, %r1

	! BUNDLE
	swapd.acquire	%r1, %r2, %r1
	swapd.release	%r1, %r2, %r1
	swapd.acq_rel	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	swapd.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end38:
	.size	test_atomic_exchange_i64, .Lfunc_end38-test_atomic_exchange_i64
	.size	.Ltest_atomic_exchange_i64$local, .Lfunc_end38-test_atomic_exchange_i64
                                        ! -- End function
	.globl	test_atomic_exchange_i128       ! -- Begin function test_atomic_exchange_i128
	.p2align	4
	.type	test_atomic_exchange_i128,@function
test_atomic_exchange_i128:              ! @test_atomic_exchange_i128
.Ltest_atomic_exchange_i128$local:
	.type	.Ltest_atomic_exchange_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	swapq.relaxed	%r1, %r2, %r1
	swapq.acquire	%r1, %r2, %r1

	! BUNDLE
	swapq.acquire	%r1, %r2, %r1
	swapq.release	%r1, %r2, %r1
	swapq.acq_rel	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	swapq.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end39:
	.size	test_atomic_exchange_i128, .Lfunc_end39-test_atomic_exchange_i128
	.size	.Ltest_atomic_exchange_i128$local, .Lfunc_end39-test_atomic_exchange_i128
                                        ! -- End function
	.globl	test_fence_acquire              ! -- Begin function test_fence_acquire
	.p2align	4
	.type	test_fence_acquire,@function
test_fence_acquire:                     ! @test_fence_acquire
.Ltest_fence_acquire$local:
	.type	.Ltest_fence_acquire$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	fence.acquire
	retf	0

.Lfunc_end40:
	.size	test_fence_acquire, .Lfunc_end40-test_fence_acquire
	.size	.Ltest_fence_acquire$local, .Lfunc_end40-test_fence_acquire
                                        ! -- End function
	.globl	test_fence_release              ! -- Begin function test_fence_release
	.p2align	4
	.type	test_fence_release,@function
test_fence_release:                     ! @test_fence_release
.Ltest_fence_release$local:
	.type	.Ltest_fence_release$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	fence.release
	retf	0

.Lfunc_end41:
	.size	test_fence_release, .Lfunc_end41-test_fence_release
	.size	.Ltest_fence_release$local, .Lfunc_end41-test_fence_release
                                        ! -- End function
	.globl	test_fence_acq_rel              ! -- Begin function test_fence_acq_rel
	.p2align	4
	.type	test_fence_acq_rel,@function
test_fence_acq_rel:                     ! @test_fence_acq_rel
.Ltest_fence_acq_rel$local:
	.type	.Ltest_fence_acq_rel$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	fence.acq_rel
	retf	0

.Lfunc_end42:
	.size	test_fence_acq_rel, .Lfunc_end42-test_fence_acq_rel
	.size	.Ltest_fence_acq_rel$local, .Lfunc_end42-test_fence_acq_rel
                                        ! -- End function
	.globl	test_fence_seq_cst              ! -- Begin function test_fence_seq_cst
	.p2align	4
	.type	test_fence_seq_cst,@function
test_fence_seq_cst:                     ! @test_fence_seq_cst
.Ltest_fence_seq_cst$local:
	.type	.Ltest_fence_seq_cst$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	fence.seq_cst
	retf	0

.Lfunc_end43:
	.size	test_fence_seq_cst, .Lfunc_end43-test_fence_seq_cst
	.size	.Ltest_fence_seq_cst$local, .Lfunc_end43-test_fence_seq_cst
                                        ! -- End function
	.globl	test_compiler_fence             ! -- Begin function test_compiler_fence
	.p2align	4
	.type	test_compiler_fence,@function
test_compiler_fence:                    ! @test_compiler_fence
.Ltest_compiler_fence$local:
	.type	.Ltest_compiler_fence$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	retf	0
	nop	0

.Lfunc_end44:
	.size	test_compiler_fence, .Lfunc_end44-test_compiler_fence
	.size	.Ltest_compiler_fence$local, .Lfunc_end44-test_compiler_fence
                                        ! -- End function
	.globl	test_atomic_load_u8_u8          ! -- Begin function test_atomic_load_u8_u8
	.p2align	4
	.type	test_atomic_load_u8_u8,@function
test_atomic_load_u8_u8:                 ! @test_atomic_load_u8_u8
.Ltest_atomic_load_u8_u8$local:
	.type	.Ltest_atomic_load_u8_u8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldab.relaxed	%r1, %r1
	ldab.acquire	%r2, %r2

	! BUNDLE
	ldab.acquire	%r3, %r3
	ldab.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end45:
	.size	test_atomic_load_u8_u8, .Lfunc_end45-test_atomic_load_u8_u8
	.size	.Ltest_atomic_load_u8_u8$local, .Lfunc_end45-test_atomic_load_u8_u8
                                        ! -- End function
	.globl	test_atomic_load_u8_u16         ! -- Begin function test_atomic_load_u8_u16
	.p2align	4
	.type	test_atomic_load_u8_u16,@function
test_atomic_load_u8_u16:                ! @test_atomic_load_u8_u16
.Ltest_atomic_load_u8_u16$local:
	.type	.Ltest_atomic_load_u8_u16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldab.relaxed	%r1, %r1
	ldab.acquire	%r2, %r2

	! BUNDLE
	ldab.acquire	%r3, %r3
	ldab.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end46:
	.size	test_atomic_load_u8_u16, .Lfunc_end46-test_atomic_load_u8_u16
	.size	.Ltest_atomic_load_u8_u16$local, .Lfunc_end46-test_atomic_load_u8_u16
                                        ! -- End function
	.globl	test_atomic_load_u8_u32         ! -- Begin function test_atomic_load_u8_u32
	.p2align	4
	.type	test_atomic_load_u8_u32,@function
test_atomic_load_u8_u32:                ! @test_atomic_load_u8_u32
.Ltest_atomic_load_u8_u32$local:
	.type	.Ltest_atomic_load_u8_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldab.relaxed	%r1, %r1
	ldab.acquire	%r2, %r2

	! BUNDLE
	ldab.acquire	%r3, %r3
	ldab.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end47:
	.size	test_atomic_load_u8_u32, .Lfunc_end47-test_atomic_load_u8_u32
	.size	.Ltest_atomic_load_u8_u32$local, .Lfunc_end47-test_atomic_load_u8_u32
                                        ! -- End function
	.globl	test_atomic_load_u8_u64         ! -- Begin function test_atomic_load_u8_u64
	.p2align	4
	.type	test_atomic_load_u8_u64,@function
test_atomic_load_u8_u64:                ! @test_atomic_load_u8_u64
.Ltest_atomic_load_u8_u64$local:
	.type	.Ltest_atomic_load_u8_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldab.relaxed	%r1, %r1
	ldab.acquire	%r2, %r2

	! BUNDLE
	ldab.acquire	%r3, %r3
	ldab.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	srlwi	%r1, %r1, 0

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end48:
	.size	test_atomic_load_u8_u64, .Lfunc_end48-test_atomic_load_u8_u64
	.size	.Ltest_atomic_load_u8_u64$local, .Lfunc_end48-test_atomic_load_u8_u64
                                        ! -- End function
	.globl	test_atomic_load_u16_u16        ! -- Begin function test_atomic_load_u16_u16
	.p2align	4
	.type	test_atomic_load_u16_u16,@function
test_atomic_load_u16_u16:               ! @test_atomic_load_u16_u16
.Ltest_atomic_load_u16_u16$local:
	.type	.Ltest_atomic_load_u16_u16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldah.relaxed	%r1, %r1
	ldah.acquire	%r2, %r2

	! BUNDLE
	ldah.acquire	%r3, %r3
	ldah.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end49:
	.size	test_atomic_load_u16_u16, .Lfunc_end49-test_atomic_load_u16_u16
	.size	.Ltest_atomic_load_u16_u16$local, .Lfunc_end49-test_atomic_load_u16_u16
                                        ! -- End function
	.globl	test_atomic_load_u16_u32        ! -- Begin function test_atomic_load_u16_u32
	.p2align	4
	.type	test_atomic_load_u16_u32,@function
test_atomic_load_u16_u32:               ! @test_atomic_load_u16_u32
.Ltest_atomic_load_u16_u32$local:
	.type	.Ltest_atomic_load_u16_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldah.relaxed	%r1, %r1
	ldah.acquire	%r2, %r2

	! BUNDLE
	ldah.acquire	%r3, %r3
	ldah.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end50:
	.size	test_atomic_load_u16_u32, .Lfunc_end50-test_atomic_load_u16_u32
	.size	.Ltest_atomic_load_u16_u32$local, .Lfunc_end50-test_atomic_load_u16_u32
                                        ! -- End function
	.globl	test_atomic_load_u16_u64        ! -- Begin function test_atomic_load_u16_u64
	.p2align	4
	.type	test_atomic_load_u16_u64,@function
test_atomic_load_u16_u64:               ! @test_atomic_load_u16_u64
.Ltest_atomic_load_u16_u64$local:
	.type	.Ltest_atomic_load_u16_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldah.relaxed	%r1, %r1
	ldah.acquire	%r2, %r2

	! BUNDLE
	ldah.acquire	%r3, %r3
	ldah.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	srlwi	%r1, %r1, 0

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end51:
	.size	test_atomic_load_u16_u64, .Lfunc_end51-test_atomic_load_u16_u64
	.size	.Ltest_atomic_load_u16_u64$local, .Lfunc_end51-test_atomic_load_u16_u64
                                        ! -- End function
	.globl	test_atomic_load_u32_u32        ! -- Begin function test_atomic_load_u32_u32
	.p2align	4
	.type	test_atomic_load_u32_u32,@function
test_atomic_load_u32_u32:               ! @test_atomic_load_u32_u32
.Ltest_atomic_load_u32_u32$local:
	.type	.Ltest_atomic_load_u32_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldaw.relaxed	%r1, %r1
	ldaw.acquire	%r2, %r2

	! BUNDLE
	ldaw.acquire	%r3, %r3
	ldaw.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end52:
	.size	test_atomic_load_u32_u32, .Lfunc_end52-test_atomic_load_u32_u32
	.size	.Ltest_atomic_load_u32_u32$local, .Lfunc_end52-test_atomic_load_u32_u32
                                        ! -- End function
	.globl	test_atomic_load_u32_u64        ! -- Begin function test_atomic_load_u32_u64
	.p2align	4
	.type	test_atomic_load_u32_u64,@function
test_atomic_load_u32_u64:               ! @test_atomic_load_u32_u64
.Ltest_atomic_load_u32_u64$local:
	.type	.Ltest_atomic_load_u32_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldaw.relaxed	%r1, %r1
	ldaw.acquire	%r2, %r2

	! BUNDLE
	ldaw.acquire	%r3, %r3
	ldaw.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	srlwi	%r1, %r1, 0

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end53:
	.size	test_atomic_load_u32_u64, .Lfunc_end53-test_atomic_load_u32_u64
	.size	.Ltest_atomic_load_u32_u64$local, .Lfunc_end53-test_atomic_load_u32_u64
                                        ! -- End function
	.globl	test_atomic_load_u64_u64        ! -- Begin function test_atomic_load_u64_u64
	.p2align	4
	.type	test_atomic_load_u64_u64,@function
test_atomic_load_u64_u64:               ! @test_atomic_load_u64_u64
.Ltest_atomic_load_u64_u64$local:
	.type	.Ltest_atomic_load_u64_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldad.relaxed	%r1, %r1
	ldad.acquire	%r2, %r2

	! BUNDLE
	ldad.acquire	%r3, %r3
	ldad.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end54:
	.size	test_atomic_load_u64_u64, .Lfunc_end54-test_atomic_load_u64_u64
	.size	.Ltest_atomic_load_u64_u64$local, .Lfunc_end54-test_atomic_load_u64_u64
                                        ! -- End function
	.globl	test_atomic_load_u128_u128      ! -- Begin function test_atomic_load_u128_u128
	.p2align	4
	.type	test_atomic_load_u128_u128,@function
test_atomic_load_u128_u128:             ! @test_atomic_load_u128_u128
.Ltest_atomic_load_u128_u128$local:
	.type	.Ltest_atomic_load_u128_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldaq.relaxed	%r1, %r1
	ldaq.acquire	%r2, %r2

	! BUNDLE
	ldaq.acquire	%r3, %r3
	ldaq.sec_cst	%r4, %r4
	xor	%r1, %r2, %r1

	! BUNDLE
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4
	retf	0

.Lfunc_end55:
	.size	test_atomic_load_u128_u128, .Lfunc_end55-test_atomic_load_u128_u128
	.size	.Ltest_atomic_load_u128_u128$local, .Lfunc_end55-test_atomic_load_u128_u128
                                        ! -- End function
	.globl	test_atomic_load_op_i8          ! -- Begin function test_atomic_load_op_i8
	.p2align	4
	.type	test_atomic_load_op_i8,@function
test_atomic_load_op_i8:                 ! @test_atomic_load_op_i8
.Ltest_atomic_load_op_i8$local:
	.type	.Ltest_atomic_load_op_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddb.relaxed	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddb.relaxed	%r1, %r2, %r1
	ldandb.relaxed	%r1, %r2, %r1
	ldorb.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxorb.relaxed	%r1, %r2, %r1
	ldsminb.relaxed	%r1, %r2, %r1
	ldsmaxb.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddb.acquire	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddb.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandb.acquire	%r1, %r2, %r1
	ldorb.acquire	%r1, %r2, %r1
	ldxorb.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsminb.acquire	%r1, %r2, %r1
	ldsmaxb.acquire	%r1, %r2, %r1
	ldaddb.acquire	%r1, %r2, %r1

	! BUNDLE
	subfiws	%r1, %r1, 0
	ldaddb.acquire	%r1, %r2, %r1
	ldandb.acquire	%r1, %r2, %r1

	! BUNDLE
	ldorb.acquire	%r1, %r2, %r1
	ldxorb.acquire	%r1, %r2, %r1
	ldsminb.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsmaxb.acquire	%r1, %r2, %r1
	ldaddb.release	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddb.release	%r1, %r2, %r1
	ldandb.release	%r1, %r2, %r1
	ldorb.release	%r1, %r2, %r1

	! BUNDLE
	ldxorb.release	%r1, %r2, %r1
	ldsminb.release	%r1, %r2, %r1
	ldsmaxb.release	%r1, %r2, %r1

	! BUNDLE
	ldaddb.acq_rel	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddb.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandb.acq_rel	%r1, %r2, %r1
	ldorb.acq_rel	%r1, %r2, %r1
	ldxorb.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldsminb.acq_rel	%r1, %r2, %r1
	ldsmaxb.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddb.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddb.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandb.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldorb.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxorb.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldsminb.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldsmaxb.relaxed	%r1, %r2, %r1
	slsrai	%r1, %r1, 56, 56

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end56:
	.size	test_atomic_load_op_i8, .Lfunc_end56-test_atomic_load_op_i8
	.size	.Ltest_atomic_load_op_i8$local, .Lfunc_end56-test_atomic_load_op_i8
                                        ! -- End function
	.globl	test_atomic_load_op_u8          ! -- Begin function test_atomic_load_op_u8
	.p2align	4
	.type	test_atomic_load_op_u8,@function
test_atomic_load_op_u8:                 ! @test_atomic_load_op_u8
.Ltest_atomic_load_op_u8$local:
	.type	.Ltest_atomic_load_op_u8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddb.relaxed	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddb.relaxed	%r1, %r2, %r1
	ldandb.relaxed	%r1, %r2, %r1
	ldorb.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxorb.relaxed	%r1, %r2, %r1
	lduminb.relaxed	%r1, %r2, %r1
	ldumaxb.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddb.acquire	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddb.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandb.acquire	%r1, %r2, %r1
	ldorb.acquire	%r1, %r2, %r1
	ldxorb.acquire	%r1, %r2, %r1

	! BUNDLE
	lduminb.acquire	%r1, %r2, %r1
	ldumaxb.acquire	%r1, %r2, %r1
	ldaddb.acquire	%r1, %r2, %r1

	! BUNDLE
	subfiws	%r1, %r1, 0
	ldaddb.acquire	%r1, %r2, %r1
	ldandb.acquire	%r1, %r2, %r1

	! BUNDLE
	ldorb.acquire	%r1, %r2, %r1
	ldxorb.acquire	%r1, %r2, %r1
	lduminb.acquire	%r1, %r2, %r1

	! BUNDLE
	ldumaxb.acquire	%r1, %r2, %r1
	ldaddb.release	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddb.release	%r1, %r2, %r1
	ldandb.release	%r1, %r2, %r1
	ldorb.release	%r1, %r2, %r1

	! BUNDLE
	ldxorb.release	%r1, %r2, %r1
	lduminb.release	%r1, %r2, %r1
	ldumaxb.release	%r1, %r2, %r1

	! BUNDLE
	ldaddb.acq_rel	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddb.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandb.acq_rel	%r1, %r2, %r1
	ldorb.acq_rel	%r1, %r2, %r1
	ldxorb.acq_rel	%r1, %r2, %r1

	! BUNDLE
	lduminb.acq_rel	%r1, %r2, %r1
	ldumaxb.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddb.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddb.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandb.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldorb.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxorb.relaxed	%r1, %r2, %r1
	fence.seq_cst
	lduminb.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldumaxb.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end57:
	.size	test_atomic_load_op_u8, .Lfunc_end57-test_atomic_load_op_u8
	.size	.Ltest_atomic_load_op_u8$local, .Lfunc_end57-test_atomic_load_op_u8
                                        ! -- End function
	.globl	test_atomic_load_op_i16         ! -- Begin function test_atomic_load_op_i16
	.p2align	4
	.type	test_atomic_load_op_i16,@function
test_atomic_load_op_i16:                ! @test_atomic_load_op_i16
.Ltest_atomic_load_op_i16$local:
	.type	.Ltest_atomic_load_op_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddh.relaxed	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddh.relaxed	%r1, %r2, %r1
	ldandh.relaxed	%r1, %r2, %r1
	ldorh.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxorh.relaxed	%r1, %r2, %r1
	ldsminh.relaxed	%r1, %r2, %r1
	ldsmaxh.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddh.acquire	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddh.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandh.acquire	%r1, %r2, %r1
	ldorh.acquire	%r1, %r2, %r1
	ldxorh.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsminh.acquire	%r1, %r2, %r1
	ldsmaxh.acquire	%r1, %r2, %r1
	ldaddh.acquire	%r1, %r2, %r1

	! BUNDLE
	subfiws	%r1, %r1, 0
	ldaddh.acquire	%r1, %r2, %r1
	ldandh.acquire	%r1, %r2, %r1

	! BUNDLE
	ldorh.acquire	%r1, %r2, %r1
	ldxorh.acquire	%r1, %r2, %r1
	ldsminh.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsmaxh.acquire	%r1, %r2, %r1
	ldaddh.release	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddh.release	%r1, %r2, %r1
	ldandh.release	%r1, %r2, %r1
	ldorh.release	%r1, %r2, %r1

	! BUNDLE
	ldxorh.release	%r1, %r2, %r1
	ldsminh.release	%r1, %r2, %r1
	ldsmaxh.release	%r1, %r2, %r1

	! BUNDLE
	ldaddh.acq_rel	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddh.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandh.acq_rel	%r1, %r2, %r1
	ldorh.acq_rel	%r1, %r2, %r1
	ldxorh.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldsminh.acq_rel	%r1, %r2, %r1
	ldsmaxh.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddh.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddh.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandh.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldorh.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxorh.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldsminh.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldsmaxh.relaxed	%r1, %r2, %r1
	slsrai	%r1, %r1, 48, 48

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end58:
	.size	test_atomic_load_op_i16, .Lfunc_end58-test_atomic_load_op_i16
	.size	.Ltest_atomic_load_op_i16$local, .Lfunc_end58-test_atomic_load_op_i16
                                        ! -- End function
	.globl	test_atomic_load_op_u16         ! -- Begin function test_atomic_load_op_u16
	.p2align	4
	.type	test_atomic_load_op_u16,@function
test_atomic_load_op_u16:                ! @test_atomic_load_op_u16
.Ltest_atomic_load_op_u16$local:
	.type	.Ltest_atomic_load_op_u16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddh.relaxed	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddh.relaxed	%r1, %r2, %r1
	ldandh.relaxed	%r1, %r2, %r1
	ldorh.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxorh.relaxed	%r1, %r2, %r1
	lduminh.relaxed	%r1, %r2, %r1
	ldumaxh.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddh.acquire	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddh.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandh.acquire	%r1, %r2, %r1
	ldorh.acquire	%r1, %r2, %r1
	ldxorh.acquire	%r1, %r2, %r1

	! BUNDLE
	lduminh.acquire	%r1, %r2, %r1
	ldumaxh.acquire	%r1, %r2, %r1
	ldaddh.acquire	%r1, %r2, %r1

	! BUNDLE
	subfiws	%r1, %r1, 0
	ldaddh.acquire	%r1, %r2, %r1
	ldandh.acquire	%r1, %r2, %r1

	! BUNDLE
	ldorh.acquire	%r1, %r2, %r1
	ldxorh.acquire	%r1, %r2, %r1
	lduminh.acquire	%r1, %r2, %r1

	! BUNDLE
	ldumaxh.acquire	%r1, %r2, %r1
	ldaddh.release	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddh.release	%r1, %r2, %r1
	ldandh.release	%r1, %r2, %r1
	ldorh.release	%r1, %r2, %r1

	! BUNDLE
	ldxorh.release	%r1, %r2, %r1
	lduminh.release	%r1, %r2, %r1
	ldumaxh.release	%r1, %r2, %r1

	! BUNDLE
	ldaddh.acq_rel	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddh.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandh.acq_rel	%r1, %r2, %r1
	ldorh.acq_rel	%r1, %r2, %r1
	ldxorh.acq_rel	%r1, %r2, %r1

	! BUNDLE
	lduminh.acq_rel	%r1, %r2, %r1
	ldumaxh.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddh.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddh.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandh.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldorh.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxorh.relaxed	%r1, %r2, %r1
	fence.seq_cst
	lduminh.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldumaxh.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end59:
	.size	test_atomic_load_op_u16, .Lfunc_end59-test_atomic_load_op_u16
	.size	.Ltest_atomic_load_op_u16$local, .Lfunc_end59-test_atomic_load_op_u16
                                        ! -- End function
	.globl	test_atomic_load_op_i32         ! -- Begin function test_atomic_load_op_i32
	.p2align	4
	.type	test_atomic_load_op_i32,@function
test_atomic_load_op_i32:                ! @test_atomic_load_op_i32
.Ltest_atomic_load_op_i32$local:
	.type	.Ltest_atomic_load_op_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddw.relaxed	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddw.relaxed	%r1, %r2, %r1
	ldandw.relaxed	%r1, %r2, %r1
	ldorw.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxorw.relaxed	%r1, %r2, %r1
	ldsminw.relaxed	%r1, %r2, %r1
	ldsmaxw.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddw.acquire	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddw.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandw.acquire	%r1, %r2, %r1
	ldorw.acquire	%r1, %r2, %r1
	ldxorw.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsminw.acquire	%r1, %r2, %r1
	ldsmaxw.acquire	%r1, %r2, %r1
	ldaddw.acquire	%r1, %r2, %r1

	! BUNDLE
	subfiws	%r1, %r1, 0
	ldaddw.acquire	%r1, %r2, %r1
	ldandw.acquire	%r1, %r2, %r1

	! BUNDLE
	ldorw.acquire	%r1, %r2, %r1
	ldxorw.acquire	%r1, %r2, %r1
	ldsminw.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsmaxw.acquire	%r1, %r2, %r1
	ldaddw.release	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddw.release	%r1, %r2, %r1
	ldandw.release	%r1, %r2, %r1
	ldorw.release	%r1, %r2, %r1

	! BUNDLE
	ldxorw.release	%r1, %r2, %r1
	ldsminw.release	%r1, %r2, %r1
	ldsmaxw.release	%r1, %r2, %r1

	! BUNDLE
	ldaddw.acq_rel	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddw.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandw.acq_rel	%r1, %r2, %r1
	ldorw.acq_rel	%r1, %r2, %r1
	ldxorw.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldsminw.acq_rel	%r1, %r2, %r1
	ldsmaxw.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddw.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddw.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandw.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldorw.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxorw.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldsminw.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldsmaxw.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end60:
	.size	test_atomic_load_op_i32, .Lfunc_end60-test_atomic_load_op_i32
	.size	.Ltest_atomic_load_op_i32$local, .Lfunc_end60-test_atomic_load_op_i32
                                        ! -- End function
	.globl	test_atomic_load_op_u32         ! -- Begin function test_atomic_load_op_u32
	.p2align	4
	.type	test_atomic_load_op_u32,@function
test_atomic_load_op_u32:                ! @test_atomic_load_op_u32
.Ltest_atomic_load_op_u32$local:
	.type	.Ltest_atomic_load_op_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddw.relaxed	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddw.relaxed	%r1, %r2, %r1
	ldandw.relaxed	%r1, %r2, %r1
	ldorw.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxorw.relaxed	%r1, %r2, %r1
	lduminw.relaxed	%r1, %r2, %r1
	ldumaxw.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddw.acquire	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddw.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandw.acquire	%r1, %r2, %r1
	ldorw.acquire	%r1, %r2, %r1
	ldxorw.acquire	%r1, %r2, %r1

	! BUNDLE
	lduminw.acquire	%r1, %r2, %r1
	ldumaxw.acquire	%r1, %r2, %r1
	ldaddw.acquire	%r1, %r2, %r1

	! BUNDLE
	subfiws	%r1, %r1, 0
	ldaddw.acquire	%r1, %r2, %r1
	ldandw.acquire	%r1, %r2, %r1

	! BUNDLE
	ldorw.acquire	%r1, %r2, %r1
	ldxorw.acquire	%r1, %r2, %r1
	lduminw.acquire	%r1, %r2, %r1

	! BUNDLE
	ldumaxw.acquire	%r1, %r2, %r1
	ldaddw.release	%r1, %r2, %r1
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddw.release	%r1, %r2, %r1
	ldandw.release	%r1, %r2, %r1
	ldorw.release	%r1, %r2, %r1

	! BUNDLE
	ldxorw.release	%r1, %r2, %r1
	lduminw.release	%r1, %r2, %r1
	ldumaxw.release	%r1, %r2, %r1

	! BUNDLE
	ldaddw.acq_rel	%r1, %r2, %r1
	subfiws	%r1, %r1, 0
	ldaddw.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandw.acq_rel	%r1, %r2, %r1
	ldorw.acq_rel	%r1, %r2, %r1
	ldxorw.acq_rel	%r1, %r2, %r1

	! BUNDLE
	lduminw.acq_rel	%r1, %r2, %r1
	ldumaxw.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddw.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfiws	%r1, %r1, 0

	! BUNDLE
	ldaddw.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandw.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldorw.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxorw.relaxed	%r1, %r2, %r1
	fence.seq_cst
	lduminw.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldumaxw.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end61:
	.size	test_atomic_load_op_u32, .Lfunc_end61-test_atomic_load_op_u32
	.size	.Ltest_atomic_load_op_u32$local, .Lfunc_end61-test_atomic_load_op_u32
                                        ! -- End function
	.globl	test_atomic_load_op_i64         ! -- Begin function test_atomic_load_op_i64
	.p2align	4
	.type	test_atomic_load_op_i64,@function
test_atomic_load_op_i64:                ! @test_atomic_load_op_i64
.Ltest_atomic_load_op_i64$local:
	.type	.Ltest_atomic_load_op_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddd.relaxed	%r1, %r2, %r1
	subfi	%r1, %r1, 0

	! BUNDLE
	ldaddd.relaxed	%r1, %r2, %r1
	ldandd.relaxed	%r1, %r2, %r1
	ldord.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxord.relaxed	%r1, %r2, %r1
	ldsmind.relaxed	%r1, %r2, %r1
	ldsmaxd.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddd.acquire	%r1, %r2, %r1
	subfi	%r1, %r1, 0
	ldaddd.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandd.acquire	%r1, %r2, %r1
	ldord.acquire	%r1, %r2, %r1
	ldxord.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsmind.acquire	%r1, %r2, %r1
	ldsmaxd.acquire	%r1, %r2, %r1
	ldaddd.acquire	%r1, %r2, %r1

	! BUNDLE
	subfi	%r1, %r1, 0
	ldaddd.acquire	%r1, %r2, %r1
	ldandd.acquire	%r1, %r2, %r1

	! BUNDLE
	ldord.acquire	%r1, %r2, %r1
	ldxord.acquire	%r1, %r2, %r1
	ldsmind.acquire	%r1, %r2, %r1

	! BUNDLE
	ldsmaxd.acquire	%r1, %r2, %r1
	ldaddd.release	%r1, %r2, %r1
	subfi	%r1, %r1, 0

	! BUNDLE
	ldaddd.release	%r1, %r2, %r1
	ldandd.release	%r1, %r2, %r1
	ldord.release	%r1, %r2, %r1

	! BUNDLE
	ldxord.release	%r1, %r2, %r1
	ldsmind.release	%r1, %r2, %r1
	ldsmaxd.release	%r1, %r2, %r1

	! BUNDLE
	ldaddd.acq_rel	%r1, %r2, %r1
	subfi	%r1, %r1, 0
	ldaddd.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandd.acq_rel	%r1, %r2, %r1
	ldord.acq_rel	%r1, %r2, %r1
	ldxord.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldsmind.acq_rel	%r1, %r2, %r1
	ldsmaxd.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddd.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfi	%r1, %r1, 0

	! BUNDLE
	ldaddd.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandd.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldord.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxord.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldsmind.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldsmaxd.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end62:
	.size	test_atomic_load_op_i64, .Lfunc_end62-test_atomic_load_op_i64
	.size	.Ltest_atomic_load_op_i64$local, .Lfunc_end62-test_atomic_load_op_i64
                                        ! -- End function
	.globl	test_atomic_load_op_u64         ! -- Begin function test_atomic_load_op_u64
	.p2align	4
	.type	test_atomic_load_op_u64,@function
test_atomic_load_op_u64:                ! @test_atomic_load_op_u64
.Ltest_atomic_load_op_u64$local:
	.type	.Ltest_atomic_load_op_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldaddd.relaxed	%r1, %r2, %r1
	subfi	%r1, %r1, 0

	! BUNDLE
	ldaddd.relaxed	%r1, %r2, %r1
	ldandd.relaxed	%r1, %r2, %r1
	ldord.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldxord.relaxed	%r1, %r2, %r1
	ldumind.relaxed	%r1, %r2, %r1
	ldumaxd.relaxed	%r1, %r2, %r1

	! BUNDLE
	ldaddd.acquire	%r1, %r2, %r1
	subfi	%r1, %r1, 0
	ldaddd.acquire	%r1, %r2, %r1

	! BUNDLE
	ldandd.acquire	%r1, %r2, %r1
	ldord.acquire	%r1, %r2, %r1
	ldxord.acquire	%r1, %r2, %r1

	! BUNDLE
	ldumind.acquire	%r1, %r2, %r1
	ldumaxd.acquire	%r1, %r2, %r1
	ldaddd.acquire	%r1, %r2, %r1

	! BUNDLE
	subfi	%r1, %r1, 0
	ldaddd.acquire	%r1, %r2, %r1
	ldandd.acquire	%r1, %r2, %r1

	! BUNDLE
	ldord.acquire	%r1, %r2, %r1
	ldxord.acquire	%r1, %r2, %r1
	ldumind.acquire	%r1, %r2, %r1

	! BUNDLE
	ldumaxd.acquire	%r1, %r2, %r1
	ldaddd.release	%r1, %r2, %r1
	subfi	%r1, %r1, 0

	! BUNDLE
	ldaddd.release	%r1, %r2, %r1
	ldandd.release	%r1, %r2, %r1
	ldord.release	%r1, %r2, %r1

	! BUNDLE
	ldxord.release	%r1, %r2, %r1
	ldumind.release	%r1, %r2, %r1
	ldumaxd.release	%r1, %r2, %r1

	! BUNDLE
	ldaddd.acq_rel	%r1, %r2, %r1
	subfi	%r1, %r1, 0
	ldaddd.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldandd.acq_rel	%r1, %r2, %r1
	ldord.acq_rel	%r1, %r2, %r1
	ldxord.acq_rel	%r1, %r2, %r1

	! BUNDLE
	ldumind.acq_rel	%r1, %r2, %r1
	ldumaxd.acq_rel	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldaddd.relaxed	%r1, %r2, %r1
	fence.seq_cst
	subfi	%r1, %r1, 0

	! BUNDLE
	ldaddd.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldandd.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldord.relaxed	%r1, %r2, %r1
	fence.seq_cst

	! BUNDLE
	ldxord.relaxed	%r1, %r2, %r1
	fence.seq_cst
	ldumind.relaxed	%r1, %r2, %r1

	! BUNDLE
	fence.seq_cst
	ldumaxd.relaxed	%r1, %r2, %r1
	retf	0

.Lfunc_end63:
	.size	test_atomic_load_op_u64, .Lfunc_end63-test_atomic_load_op_u64
	.size	.Ltest_atomic_load_op_u64$local, .Lfunc_end63-test_atomic_load_op_u64
                                        ! -- End function
	.globl	test_atomic_store_i8            ! -- Begin function test_atomic_store_i8
	.p2align	4
	.type	test_atomic_store_i8,@function
test_atomic_store_i8:                   ! @test_atomic_store_i8
.Ltest_atomic_store_i8$local:
	.type	.Ltest_atomic_store_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	stab.relaxed	%r4, %r1
	stab.release	%r4, %r2

	! BUNDLE
	stab.sec_cst	%r4, %r3
	retf	0
	nop	0

.Lfunc_end64:
	.size	test_atomic_store_i8, .Lfunc_end64-test_atomic_store_i8
	.size	.Ltest_atomic_store_i8$local, .Lfunc_end64-test_atomic_store_i8
                                        ! -- End function
	.globl	test_atomic_store_i16           ! -- Begin function test_atomic_store_i16
	.p2align	4
	.type	test_atomic_store_i16,@function
test_atomic_store_i16:                  ! @test_atomic_store_i16
.Ltest_atomic_store_i16$local:
	.type	.Ltest_atomic_store_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	stah.relaxed	%r4, %r1
	stah.release	%r4, %r2

	! BUNDLE
	stah.sec_cst	%r4, %r3
	retf	0
	nop	0

.Lfunc_end65:
	.size	test_atomic_store_i16, .Lfunc_end65-test_atomic_store_i16
	.size	.Ltest_atomic_store_i16$local, .Lfunc_end65-test_atomic_store_i16
                                        ! -- End function
	.globl	test_atomic_store_i32           ! -- Begin function test_atomic_store_i32
	.p2align	4
	.type	test_atomic_store_i32,@function
test_atomic_store_i32:                  ! @test_atomic_store_i32
.Ltest_atomic_store_i32$local:
	.type	.Ltest_atomic_store_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	staw.relaxed	%r4, %r1
	staw.release	%r4, %r2

	! BUNDLE
	staw.sec_cst	%r4, %r3
	retf	0
	nop	0

.Lfunc_end66:
	.size	test_atomic_store_i32, .Lfunc_end66-test_atomic_store_i32
	.size	.Ltest_atomic_store_i32$local, .Lfunc_end66-test_atomic_store_i32
                                        ! -- End function
	.globl	test_atomic_store_i64           ! -- Begin function test_atomic_store_i64
	.p2align	4
	.type	test_atomic_store_i64,@function
test_atomic_store_i64:                  ! @test_atomic_store_i64
.Ltest_atomic_store_i64$local:
	.type	.Ltest_atomic_store_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	stad.relaxed	%r4, %r1
	stad.release	%r4, %r2

	! BUNDLE
	stad.sec_cst	%r4, %r3
	retf	0
	nop	0

.Lfunc_end67:
	.size	test_atomic_store_i64, .Lfunc_end67-test_atomic_store_i64
	.size	.Ltest_atomic_store_i64$local, .Lfunc_end67-test_atomic_store_i64
                                        ! -- End function
	.globl	test_atomic_store_i128          ! -- Begin function test_atomic_store_i128
	.p2align	4
	.type	test_atomic_store_i128,@function
test_atomic_store_i128:                 ! @test_atomic_store_i128
.Ltest_atomic_store_i128$local:
	.type	.Ltest_atomic_store_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	staq.relaxed	%r4, %r1
	staq.release	%r4, %r2

	! BUNDLE
	staq.sec_cst	%r4, %r3
	retf	0
	nop	0

.Lfunc_end68:
	.size	test_atomic_store_i128, .Lfunc_end68-test_atomic_store_i128
	.size	.Ltest_atomic_store_i128$local, .Lfunc_end68-test_atomic_store_i128
                                        ! -- End function
	.globl	test_i32_inv                    ! -- Begin function test_i32_inv
	.p2align	4
	.type	test_i32_inv,@function
test_i32_inv:                           ! @test_i32_inv
.Ltest_i32_inv$local:
	.type	.Ltest_i32_inv$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori	%r1, %r1, -1
	retf	0

.Lfunc_end69:
	.size	test_i32_inv, .Lfunc_end69-test_i32_inv
	.size	.Ltest_i32_inv$local, .Lfunc_end69-test_i32_inv
                                        ! -- End function
	.globl	test_i64_inv                    ! -- Begin function test_i64_inv
	.p2align	4
	.type	test_i64_inv,@function
test_i64_inv:                           ! @test_i64_inv
.Ltest_i64_inv$local:
	.type	.Ltest_i64_inv$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori	%r1, %r1, -1
	retf	0

.Lfunc_end70:
	.size	test_i64_inv, .Lfunc_end70-test_i64_inv
	.size	.Ltest_i64_inv$local, .Lfunc_end70-test_i64_inv
                                        ! -- End function
	.globl	test_i32_and                    ! -- Begin function test_i32_and
	.p2align	4
	.type	test_i32_and,@function
test_i32_and:                           ! @test_i32_and
.Ltest_i32_and$local:
	.type	.Ltest_i32_and$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	and	%r1, %r2, %r1
	retf	0

.Lfunc_end71:
	.size	test_i32_and, .Lfunc_end71-test_i32_and
	.size	.Ltest_i32_and$local, .Lfunc_end71-test_i32_and
                                        ! -- End function
	.globl	test_i64_and                    ! -- Begin function test_i64_and
	.p2align	4
	.type	test_i64_and,@function
test_i64_and:                           ! @test_i64_and
.Ltest_i64_and$local:
	.type	.Ltest_i64_and$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	and	%r1, %r2, %r1
	retf	0

.Lfunc_end72:
	.size	test_i64_and, .Lfunc_end72-test_i64_and
	.size	.Ltest_i64_and$local, .Lfunc_end72-test_i64_and
                                        ! -- End function
	.globl	test_i32_andi                   ! -- Begin function test_i32_andi
	.p2align	4
	.type	test_i32_andi,@function
test_i32_andi:                          ! @test_i32_andi
.Ltest_i32_andi$local:
	.type	.Ltest_i32_andi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi	%r1, %r1, 12345
	retf	0

.Lfunc_end73:
	.size	test_i32_andi, .Lfunc_end73-test_i32_andi
	.size	.Ltest_i32_andi$local, .Lfunc_end73-test_i32_andi
                                        ! -- End function
	.globl	test_i64_andi                   ! -- Begin function test_i64_andi
	.p2align	4
	.type	test_i64_andi,@function
test_i64_andi:                          ! @test_i64_andi
.Ltest_i64_andi$local:
	.type	.Ltest_i64_andi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi	%r1, %r1, 12345
	retf	0

.Lfunc_end74:
	.size	test_i64_andi, .Lfunc_end74-test_i64_andi
	.size	.Ltest_i64_andi$local, .Lfunc_end74-test_i64_andi
                                        ! -- End function
	.globl	test_i32_or                     ! -- Begin function test_i32_or
	.p2align	4
	.type	test_i32_or,@function
test_i32_or:                            ! @test_i32_or
.Ltest_i32_or$local:
	.type	.Ltest_i32_or$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	or	%r1, %r2, %r1
	retf	0

.Lfunc_end75:
	.size	test_i32_or, .Lfunc_end75-test_i32_or
	.size	.Ltest_i32_or$local, .Lfunc_end75-test_i32_or
                                        ! -- End function
	.globl	test_i64_or                     ! -- Begin function test_i64_or
	.p2align	4
	.type	test_i64_or,@function
test_i64_or:                            ! @test_i64_or
.Ltest_i64_or$local:
	.type	.Ltest_i64_or$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	or	%r1, %r2, %r1
	retf	0

.Lfunc_end76:
	.size	test_i64_or, .Lfunc_end76-test_i64_or
	.size	.Ltest_i64_or$local, .Lfunc_end76-test_i64_or
                                        ! -- End function
	.globl	test_i32_ori                    ! -- Begin function test_i32_ori
	.p2align	4
	.type	test_i32_ori,@function
test_i32_ori:                           ! @test_i32_ori
.Ltest_i32_ori$local:
	.type	.Ltest_i32_ori$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ori	%r1, %r1, 12345
	retf	0

.Lfunc_end77:
	.size	test_i32_ori, .Lfunc_end77-test_i32_ori
	.size	.Ltest_i32_ori$local, .Lfunc_end77-test_i32_ori
                                        ! -- End function
	.globl	test_i64_ori                    ! -- Begin function test_i64_ori
	.p2align	4
	.type	test_i64_ori,@function
test_i64_ori:                           ! @test_i64_ori
.Ltest_i64_ori$local:
	.type	.Ltest_i64_ori$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ori	%r1, %r1, 12345
	retf	0

.Lfunc_end78:
	.size	test_i64_ori, .Lfunc_end78-test_i64_ori
	.size	.Ltest_i64_ori$local, .Lfunc_end78-test_i64_ori
                                        ! -- End function
	.globl	test_i32_xor                    ! -- Begin function test_i32_xor
	.p2align	4
	.type	test_i32_xor,@function
test_i32_xor:                           ! @test_i32_xor
.Ltest_i32_xor$local:
	.type	.Ltest_i32_xor$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xor	%r1, %r2, %r1
	retf	0

.Lfunc_end79:
	.size	test_i32_xor, .Lfunc_end79-test_i32_xor
	.size	.Ltest_i32_xor$local, .Lfunc_end79-test_i32_xor
                                        ! -- End function
	.globl	test_i64_xor                    ! -- Begin function test_i64_xor
	.p2align	4
	.type	test_i64_xor,@function
test_i64_xor:                           ! @test_i64_xor
.Ltest_i64_xor$local:
	.type	.Ltest_i64_xor$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xor	%r1, %r2, %r1
	retf	0

.Lfunc_end80:
	.size	test_i64_xor, .Lfunc_end80-test_i64_xor
	.size	.Ltest_i64_xor$local, .Lfunc_end80-test_i64_xor
                                        ! -- End function
	.globl	test_i32_xori                   ! -- Begin function test_i32_xori
	.p2align	4
	.type	test_i32_xori,@function
test_i32_xori:                          ! @test_i32_xori
.Ltest_i32_xori$local:
	.type	.Ltest_i32_xori$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori	%r1, %r1, 12345
	retf	0

.Lfunc_end81:
	.size	test_i32_xori, .Lfunc_end81-test_i32_xori
	.size	.Ltest_i32_xori$local, .Lfunc_end81-test_i32_xori
                                        ! -- End function
	.globl	test_i32_xori_ext               ! -- Begin function test_i32_xori_ext
	.p2align	4
	.type	test_i32_xori_ext,@function
test_i32_xori_ext:                      ! @test_i32_xori_ext
.Ltest_i32_xori_ext$local:
	.type	.Ltest_i32_xori_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori.l	%r1, %r1, 123456789

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end82:
	.size	test_i32_xori_ext, .Lfunc_end82-test_i32_xori_ext
	.size	.Ltest_i32_xori_ext$local, .Lfunc_end82-test_i32_xori_ext
                                        ! -- End function
	.globl	test_i64_xori_ext               ! -- Begin function test_i64_xori_ext
	.p2align	4
	.type	test_i64_xori_ext,@function
test_i64_xori_ext:                      ! @test_i64_xori_ext
.Ltest_i64_xori_ext$local:
	.type	.Ltest_i64_xori_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori.l	%r1, %r1, 123456789

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end83:
	.size	test_i64_xori_ext, .Lfunc_end83-test_i64_xori_ext
	.size	.Ltest_i64_xori_ext$local, .Lfunc_end83-test_i64_xori_ext
                                        ! -- End function
	.globl	test_i1_and                     ! -- Begin function test_i1_and
	.p2align	4
	.type	test_i1_and,@function
test_i1_and:                            ! @test_i1_and
.Ltest_i1_and$local:
	.type	.Ltest_i1_and$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	and	%r1, %r1, %r2
	retf	0

.Lfunc_end84:
	.size	test_i1_and, .Lfunc_end84-test_i1_and
	.size	.Ltest_i1_and$local, .Lfunc_end84-test_i1_and
                                        ! -- End function
	.globl	test_i1_or                      ! -- Begin function test_i1_or
	.p2align	4
	.type	test_i1_or,@function
test_i1_or:                             ! @test_i1_or
.Ltest_i1_or$local:
	.type	.Ltest_i1_or$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	or	%r1, %r1, %r2
	retf	0

.Lfunc_end85:
	.size	test_i1_or, .Lfunc_end85-test_i1_or
	.size	.Ltest_i1_or$local, .Lfunc_end85-test_i1_or
                                        ! -- End function
	.globl	test_i1_xor                     ! -- Begin function test_i1_xor
	.p2align	4
	.type	test_i1_xor,@function
test_i1_xor:                            ! @test_i1_xor
.Ltest_i1_xor$local:
	.type	.Ltest_i1_xor$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xor	%r1, %r1, %r2
	retf	0

.Lfunc_end86:
	.size	test_i1_xor, .Lfunc_end86-test_i1_xor
	.size	.Ltest_i1_xor$local, .Lfunc_end86-test_i1_xor
                                        ! -- End function
	.globl	test_i1_not                     ! -- Begin function test_i1_not
	.p2align	4
	.type	test_i1_not,@function
test_i1_not:                            ! @test_i1_not
.Ltest_i1_not$local:
	.type	.Ltest_i1_not$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori	%r1, %r1, 1
	retf	0

.Lfunc_end87:
	.size	test_i1_not, .Lfunc_end87-test_i1_not
	.size	.Ltest_i1_not$local, .Lfunc_end87-test_i1_not
                                        ! -- End function
	.globl	test_i1_andn                    ! -- Begin function test_i1_andn
	.p2align	4
	.type	test_i1_andn,@function
test_i1_andn:                           ! @test_i1_andn
.Ltest_i1_andn$local:
	.type	.Ltest_i1_andn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xori	%r1, %r1, 1
	and	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end88:
	.size	test_i1_andn, .Lfunc_end88-test_i1_andn
	.size	.Ltest_i1_andn$local, .Lfunc_end88-test_i1_andn
                                        ! -- End function
	.globl	test_i1_orn                     ! -- Begin function test_i1_orn
	.p2align	4
	.type	test_i1_orn,@function
test_i1_orn:                            ! @test_i1_orn
.Ltest_i1_orn$local:
	.type	.Ltest_i1_orn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xori	%r1, %r1, 1
	or	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end89:
	.size	test_i1_orn, .Lfunc_end89-test_i1_orn
	.size	.Ltest_i1_orn$local, .Lfunc_end89-test_i1_orn
                                        ! -- End function
	.globl	test_i1_nand                    ! -- Begin function test_i1_nand
	.p2align	4
	.type	test_i1_nand,@function
test_i1_nand:                           ! @test_i1_nand
.Ltest_i1_nand$local:
	.type	.Ltest_i1_nand$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	and	%r1, %r1, %r2
	xori	%r1, %r1, 1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end90:
	.size	test_i1_nand, .Lfunc_end90-test_i1_nand
	.size	.Ltest_i1_nand$local, .Lfunc_end90-test_i1_nand
                                        ! -- End function
	.globl	test_i1_to_i32                  ! -- Begin function test_i1_to_i32
	.p2align	4
	.type	test_i1_to_i32,@function
test_i1_to_i32:                         ! @test_i1_to_i32
.Ltest_i1_to_i32$local:
	.type	.Ltest_i1_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end91:
	.size	test_i1_to_i32, .Lfunc_end91-test_i1_to_i32
	.size	.Ltest_i1_to_i32$local, .Lfunc_end91-test_i1_to_i32
                                        ! -- End function
	.globl	test_i1_to_i64                  ! -- Begin function test_i1_to_i64
	.p2align	4
	.type	test_i1_to_i64,@function
test_i1_to_i64:                         ! @test_i1_to_i64
.Ltest_i1_to_i64$local:
	.type	.Ltest_i1_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end92:
	.size	test_i1_to_i64, .Lfunc_end92-test_i1_to_i64
	.size	.Ltest_i1_to_i64$local, .Lfunc_end92-test_i1_to_i64
                                        ! -- End function
	.globl	test_i1_inv                     ! -- Begin function test_i1_inv
	.p2align	4
	.type	test_i1_inv,@function
test_i1_inv:                            ! @test_i1_inv
.Ltest_i1_inv$local:
	.type	.Ltest_i1_inv$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 1
	retf	0

.Lfunc_end93:
	.size	test_i1_inv, .Lfunc_end93-test_i1_inv
	.size	.Ltest_i1_inv$local, .Lfunc_end93-test_i1_inv
                                        ! -- End function
	.globl	test_imm_to_i1                  ! -- Begin function test_imm_to_i1
	.p2align	4
	.type	test_imm_to_i1,@function
test_imm_to_i1:                         ! @test_imm_to_i1
.Ltest_imm_to_i1$local:
	.type	.Ltest_imm_to_i1$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 1
	retf	0

.Lfunc_end94:
	.size	test_imm_to_i1, .Lfunc_end94-test_imm_to_i1
	.size	.Ltest_imm_to_i1$local, .Lfunc_end94-test_imm_to_i1
                                        ! -- End function
	.globl	test_write_i1                   ! -- Begin function test_write_i1
	.p2align	4
	.type	test_write_i1,@function
test_write_i1:                          ! @test_write_i1
.Ltest_write_i1$local:
	.type	.Ltest_write_i1$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 1
	stbr	%r1, %pcfwd(test_bool_true)

	! BUNDLE
	stbr	%gz, %pcfwd(test_bool_false)
	stbr	%r1, %pcfwd(test_i8_true)
	stbr	%gz, %pcfwd(test_i8_false)

	! BUNDLE
	sthr	%r1, %pcfwd(test_i16_true)
	sthr	%gz, %pcfwd(test_i16_false)
	stwr	%r1, %pcfwd(test_i32_true)

	! BUNDLE
	stwr	%gz, %pcfwd(test_i32_false)
	stdr	%r1, %pcfwd(test_i64_true)
	stdr	%gz, %pcfwd(test_i64_false)

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end95:
	.size	test_write_i1, .Lfunc_end95-test_write_i1
	.size	.Ltest_write_i1$local, .Lfunc_end95-test_write_i1
                                        ! -- End function
	.globl	test_branch_i64_eqi             ! -- Begin function test_branch_i64_eqi
	.p2align	4
	.type	test_branch_i64_eqi,@function
test_branch_i64_eqi:                    ! @test_branch_i64_eqi
.Ltest_branch_i64_eqi$local:
	.type	.Ltest_branch_i64_eqi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldnei	%r1, 534, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end96:
	.size	test_branch_i64_eqi, .Lfunc_end96-test_branch_i64_eqi
	.size	.Ltest_branch_i64_eqi$local, .Lfunc_end96-test_branch_i64_eqi
                                        ! -- End function
	.globl	test_branch_i64_nei             ! -- Begin function test_branch_i64_nei
	.p2align	4
	.type	test_branch_i64_nei,@function
test_branch_i64_nei:                    ! @test_branch_i64_nei
.Ltest_branch_i64_nei$local:
	.type	.Ltest_branch_i64_nei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldeqi	%r1, 534, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end97:
	.size	test_branch_i64_nei, .Lfunc_end97-test_branch_i64_nei
	.size	.Ltest_branch_i64_nei$local, .Lfunc_end97-test_branch_i64_nei
                                        ! -- End function
	.globl	test_branch_i64_lti             ! -- Begin function test_branch_i64_lti
	.p2align	4
	.type	test_branch_i64_lti,@function
test_branch_i64_lti:                    ! @test_branch_i64_lti
.Ltest_branch_i64_lti$local:
	.type	.Ltest_branch_i64_lti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti	%r1, 534, 0, 2
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end98:
	.size	test_branch_i64_lti, .Lfunc_end98-test_branch_i64_lti
	.size	.Ltest_branch_i64_lti$local, .Lfunc_end98-test_branch_i64_lti
                                        ! -- End function
	.globl	test_branch_i64_lei             ! -- Begin function test_branch_i64_lei
	.p2align	4
	.type	test_branch_i64_lei,@function
test_branch_i64_lei:                    ! @test_branch_i64_lei
.Ltest_branch_i64_lei$local:
	.type	.Ltest_branch_i64_lei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti	%r1, 535, 0, 2
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end99:
	.size	test_branch_i64_lei, .Lfunc_end99-test_branch_i64_lei
	.size	.Ltest_branch_i64_lei$local, .Lfunc_end99-test_branch_i64_lei
                                        ! -- End function
	.globl	test_branch_i64_gti             ! -- Begin function test_branch_i64_gti
	.p2align	4
	.type	test_branch_i64_gti,@function
test_branch_i64_gti:                    ! @test_branch_i64_gti
.Ltest_branch_i64_gti$local:
	.type	.Ltest_branch_i64_gti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti	%r1, 535, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end100:
	.size	test_branch_i64_gti, .Lfunc_end100-test_branch_i64_gti
	.size	.Ltest_branch_i64_gti$local, .Lfunc_end100-test_branch_i64_gti
                                        ! -- End function
	.globl	test_branch_i64_gei             ! -- Begin function test_branch_i64_gei
	.p2align	4
	.type	test_branch_i64_gei,@function
test_branch_i64_gei:                    ! @test_branch_i64_gei
.Ltest_branch_i64_gei$local:
	.type	.Ltest_branch_i64_gei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti	%r1, 534, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end101:
	.size	test_branch_i64_gei, .Lfunc_end101-test_branch_i64_gei
	.size	.Ltest_branch_i64_gei$local, .Lfunc_end101-test_branch_i64_gei
                                        ! -- End function
	.globl	test_branch_ext_i64_eqi         ! -- Begin function test_branch_ext_i64_eqi
	.p2align	4
	.type	test_branch_ext_i64_eqi,@function
test_branch_ext_i64_eqi:                ! @test_branch_ext_i64_eqi
.Ltest_branch_ext_i64_eqi$local:
	.type	.Ltest_branch_ext_i64_eqi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldnei.l	%r1, 12345678, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end102:
	.size	test_branch_ext_i64_eqi, .Lfunc_end102-test_branch_ext_i64_eqi
	.size	.Ltest_branch_ext_i64_eqi$local, .Lfunc_end102-test_branch_ext_i64_eqi
                                        ! -- End function
	.globl	test_branch_ext_i64_nei         ! -- Begin function test_branch_ext_i64_nei
	.p2align	4
	.type	test_branch_ext_i64_nei,@function
test_branch_ext_i64_nei:                ! @test_branch_ext_i64_nei
.Ltest_branch_ext_i64_nei$local:
	.type	.Ltest_branch_ext_i64_nei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldeqi.l	%r1, 12345678, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end103:
	.size	test_branch_ext_i64_nei, .Lfunc_end103-test_branch_ext_i64_nei
	.size	.Ltest_branch_ext_i64_nei$local, .Lfunc_end103-test_branch_ext_i64_nei
                                        ! -- End function
	.globl	test_branch_ext_i64_lti         ! -- Begin function test_branch_ext_i64_lti
	.p2align	4
	.type	test_branch_ext_i64_lti,@function
test_branch_ext_i64_lti:                ! @test_branch_ext_i64_lti
.Ltest_branch_ext_i64_lti$local:
	.type	.Ltest_branch_ext_i64_lti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti.l	%r1, 12345678, 0, 2

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end104:
	.size	test_branch_ext_i64_lti, .Lfunc_end104-test_branch_ext_i64_lti
	.size	.Ltest_branch_ext_i64_lti$local, .Lfunc_end104-test_branch_ext_i64_lti
                                        ! -- End function
	.globl	test_branch_ext_i64_lei         ! -- Begin function test_branch_ext_i64_lei
	.p2align	4
	.type	test_branch_ext_i64_lei,@function
test_branch_ext_i64_lei:                ! @test_branch_ext_i64_lei
.Ltest_branch_ext_i64_lei$local:
	.type	.Ltest_branch_ext_i64_lei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti.l	%r1, 12345679, 0, 2

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end105:
	.size	test_branch_ext_i64_lei, .Lfunc_end105-test_branch_ext_i64_lei
	.size	.Ltest_branch_ext_i64_lei$local, .Lfunc_end105-test_branch_ext_i64_lei
                                        ! -- End function
	.globl	test_branch_ext_i64_gti         ! -- Begin function test_branch_ext_i64_gti
	.p2align	4
	.type	test_branch_ext_i64_gti,@function
test_branch_ext_i64_gti:                ! @test_branch_ext_i64_gti
.Ltest_branch_ext_i64_gti$local:
	.type	.Ltest_branch_ext_i64_gti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti.l	%r1, 12345679, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end106:
	.size	test_branch_ext_i64_gti, .Lfunc_end106-test_branch_ext_i64_gti
	.size	.Ltest_branch_ext_i64_gti$local, .Lfunc_end106-test_branch_ext_i64_gti
                                        ! -- End function
	.globl	test_branch_ext_i64_gei         ! -- Begin function test_branch_ext_i64_gei
	.p2align	4
	.type	test_branch_ext_i64_gei,@function
test_branch_ext_i64_gei:                ! @test_branch_ext_i64_gei
.Ltest_branch_ext_i64_gei$local:
	.type	.Ltest_branch_ext_i64_gei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldlti.l	%r1, 12345678, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end107:
	.size	test_branch_ext_i64_gei, .Lfunc_end107-test_branch_ext_i64_gei
	.size	.Ltest_branch_ext_i64_gei$local, .Lfunc_end107-test_branch_ext_i64_gei
                                        ! -- End function
	.globl	test_branch_i32_eqi             ! -- Begin function test_branch_i32_eqi
	.p2align	4
	.type	test_branch_i32_eqi,@function
test_branch_i32_eqi:                    ! @test_branch_i32_eqi
.Ltest_branch_i32_eqi$local:
	.type	.Ltest_branch_i32_eqi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwnei	%r1, 534, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end108:
	.size	test_branch_i32_eqi, .Lfunc_end108-test_branch_i32_eqi
	.size	.Ltest_branch_i32_eqi$local, .Lfunc_end108-test_branch_i32_eqi
                                        ! -- End function
	.globl	test_branch_i32_nei             ! -- Begin function test_branch_i32_nei
	.p2align	4
	.type	test_branch_i32_nei,@function
test_branch_i32_nei:                    ! @test_branch_i32_nei
.Ltest_branch_i32_nei$local:
	.type	.Ltest_branch_i32_nei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulweqi	%r1, 534, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end109:
	.size	test_branch_i32_nei, .Lfunc_end109-test_branch_i32_nei
	.size	.Ltest_branch_i32_nei$local, .Lfunc_end109-test_branch_i32_nei
                                        ! -- End function
	.globl	test_branch_i32_lti             ! -- Begin function test_branch_i32_lti
	.p2align	4
	.type	test_branch_i32_lti,@function
test_branch_i32_lti:                    ! @test_branch_i32_lti
.Ltest_branch_i32_lti$local:
	.type	.Ltest_branch_i32_lti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti	%r1, 534, 0, 2
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end110:
	.size	test_branch_i32_lti, .Lfunc_end110-test_branch_i32_lti
	.size	.Ltest_branch_i32_lti$local, .Lfunc_end110-test_branch_i32_lti
                                        ! -- End function
	.globl	test_branch_i32_lei             ! -- Begin function test_branch_i32_lei
	.p2align	4
	.type	test_branch_i32_lei,@function
test_branch_i32_lei:                    ! @test_branch_i32_lei
.Ltest_branch_i32_lei$local:
	.type	.Ltest_branch_i32_lei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti	%r1, 535, 0, 2
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end111:
	.size	test_branch_i32_lei, .Lfunc_end111-test_branch_i32_lei
	.size	.Ltest_branch_i32_lei$local, .Lfunc_end111-test_branch_i32_lei
                                        ! -- End function
	.globl	test_branch_i32_gti             ! -- Begin function test_branch_i32_gti
	.p2align	4
	.type	test_branch_i32_gti,@function
test_branch_i32_gti:                    ! @test_branch_i32_gti
.Ltest_branch_i32_gti$local:
	.type	.Ltest_branch_i32_gti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti	%r1, 535, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end112:
	.size	test_branch_i32_gti, .Lfunc_end112-test_branch_i32_gti
	.size	.Ltest_branch_i32_gti$local, .Lfunc_end112-test_branch_i32_gti
                                        ! -- End function
	.globl	test_branch_i32_gei             ! -- Begin function test_branch_i32_gei
	.p2align	4
	.type	test_branch_i32_gei,@function
test_branch_i32_gei:                    ! @test_branch_i32_gei
.Ltest_branch_i32_gei$local:
	.type	.Ltest_branch_i32_gei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti	%r1, 534, 2, 0
	ldafr	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	callr	%r2, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end113:
	.size	test_branch_i32_gei, .Lfunc_end113-test_branch_i32_gei
	.size	.Ltest_branch_i32_gei$local, .Lfunc_end113-test_branch_i32_gei
                                        ! -- End function
	.globl	test_branch_ext_i32_eqi         ! -- Begin function test_branch_ext_i32_eqi
	.p2align	4
	.type	test_branch_ext_i32_eqi,@function
test_branch_ext_i32_eqi:                ! @test_branch_ext_i32_eqi
.Ltest_branch_ext_i32_eqi$local:
	.type	.Ltest_branch_ext_i32_eqi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwnei.l	%r1, 12345678, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end114:
	.size	test_branch_ext_i32_eqi, .Lfunc_end114-test_branch_ext_i32_eqi
	.size	.Ltest_branch_ext_i32_eqi$local, .Lfunc_end114-test_branch_ext_i32_eqi
                                        ! -- End function
	.globl	test_branch_ext_i32_nei         ! -- Begin function test_branch_ext_i32_nei
	.p2align	4
	.type	test_branch_ext_i32_nei,@function
test_branch_ext_i32_nei:                ! @test_branch_ext_i32_nei
.Ltest_branch_ext_i32_nei$local:
	.type	.Ltest_branch_ext_i32_nei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulweqi.l	%r1, 12345678, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end115:
	.size	test_branch_ext_i32_nei, .Lfunc_end115-test_branch_ext_i32_nei
	.size	.Ltest_branch_ext_i32_nei$local, .Lfunc_end115-test_branch_ext_i32_nei
                                        ! -- End function
	.globl	test_branch_ext_i32_lti         ! -- Begin function test_branch_ext_i32_lti
	.p2align	4
	.type	test_branch_ext_i32_lti,@function
test_branch_ext_i32_lti:                ! @test_branch_ext_i32_lti
.Ltest_branch_ext_i32_lti$local:
	.type	.Ltest_branch_ext_i32_lti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti.l	%r1, 12345678, 0, 2

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end116:
	.size	test_branch_ext_i32_lti, .Lfunc_end116-test_branch_ext_i32_lti
	.size	.Ltest_branch_ext_i32_lti$local, .Lfunc_end116-test_branch_ext_i32_lti
                                        ! -- End function
	.globl	test_branch_ext_i32_lei         ! -- Begin function test_branch_ext_i32_lei
	.p2align	4
	.type	test_branch_ext_i32_lei,@function
test_branch_ext_i32_lei:                ! @test_branch_ext_i32_lei
.Ltest_branch_ext_i32_lei$local:
	.type	.Ltest_branch_ext_i32_lei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti.l	%r1, 12345679, 0, 2

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end117:
	.size	test_branch_ext_i32_lei, .Lfunc_end117-test_branch_ext_i32_lei
	.size	.Ltest_branch_ext_i32_lei$local, .Lfunc_end117-test_branch_ext_i32_lei
                                        ! -- End function
	.globl	test_branch_ext_i32_gti         ! -- Begin function test_branch_ext_i32_gti
	.p2align	4
	.type	test_branch_ext_i32_gti,@function
test_branch_ext_i32_gti:                ! @test_branch_ext_i32_gti
.Ltest_branch_ext_i32_gti$local:
	.type	.Ltest_branch_ext_i32_gti$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti.l	%r1, 12345679, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end118:
	.size	test_branch_ext_i32_gti, .Lfunc_end118-test_branch_ext_i32_gti
	.size	.Ltest_branch_ext_i32_gti$local, .Lfunc_end118-test_branch_ext_i32_gti
                                        ! -- End function
	.globl	test_branch_ext_i32_gei         ! -- Begin function test_branch_ext_i32_gei
	.p2align	4
	.type	test_branch_ext_i32_gei,@function
test_branch_ext_i32_gei:                ! @test_branch_ext_i32_gei
.Ltest_branch_ext_i32_gei$local:
	.type	.Ltest_branch_ext_i32_gei$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulwlti.l	%r1, 12345678, 2, 0

	! BUNDLE
	ldafr	%r3, %pcfwd(.Lstr.98)
	callr	%r2, %jpc(puts)
	retf	0

.Lfunc_end119:
	.size	test_branch_ext_i32_gei, .Lfunc_end119-test_branch_ext_i32_gei
	.size	.Ltest_branch_ext_i32_gei$local, .Lfunc_end119-test_branch_ext_i32_gei
                                        ! -- End function
	.globl	test_bit_1                      ! -- Begin function test_bit_1
	.p2align	4
	.type	test_bit_1,@function
test_bit_1:                             ! @test_bit_1
.Ltest_bit_1$local:
	.type	.Ltest_bit_1$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbs	%r1, %r2, 4, 0
	std	%r2, %sp, 8

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.1)
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end120:
	.size	test_bit_1, .Lfunc_end120-test_bit_1
	.size	.Ltest_bit_1$local, .Lfunc_end120-test_bit_1
                                        ! -- End function
	.globl	test_bit_2                      ! -- Begin function test_bit_2
	.p2align	4
	.type	test_bit_2,@function
test_bit_2:                             ! @test_bit_2
.Ltest_bit_2$local:
	.type	.Ltest_bit_2$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbc	%r1, %r2, 4, 0
	std	%r2, %sp, 8

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.2)
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end121:
	.size	test_bit_2, .Lfunc_end121-test_bit_2
	.size	.Ltest_bit_2$local, .Lfunc_end121-test_bit_2
                                        ! -- End function
	.globl	test_bit_3                      ! -- Begin function test_bit_3
	.p2align	4
	.type	test_bit_3,@function
test_bit_3:                             ! @test_bit_3
.Ltest_bit_3$local:
	.type	.Ltest_bit_3$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbs	%r1, %r2, 4, 0
	std	%r2, %sp, 8

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.3)
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end122:
	.size	test_bit_3, .Lfunc_end122-test_bit_3
	.size	.Ltest_bit_3$local, .Lfunc_end122-test_bit_3
                                        ! -- End function
	.globl	test_bit_4                      ! -- Begin function test_bit_4
	.p2align	4
	.type	test_bit_4,@function
test_bit_4:                             ! @test_bit_4
.Ltest_bit_4$local:
	.type	.Ltest_bit_4$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbci	%r1, 15, 4, 0
	std	%r2, %sp, 8

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.4)
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end123:
	.size	test_bit_4, .Lfunc_end123-test_bit_4
	.size	.Ltest_bit_4$local, .Lfunc_end123-test_bit_4
                                        ! -- End function
	.globl	test_bit_5                      ! -- Begin function test_bit_5
	.p2align	4
	.type	test_bit_5,@function
test_bit_5:                             ! @test_bit_5
.Ltest_bit_5$local:
	.type	.Ltest_bit_5$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbsi	%r1, 14, 4, 0
	std	%r2, %sp, 8

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.5)
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end124:
	.size	test_bit_5, .Lfunc_end124-test_bit_5
	.size	.Ltest_bit_5$local, .Lfunc_end124-test_bit_5
                                        ! -- End function
	.globl	test_bit_i32_1                  ! -- Begin function test_bit_i32_1
	.p2align	4
	.type	test_bit_i32_1,@function
test_bit_i32_1:                         ! @test_bit_i32_1
.Ltest_bit_i32_1$local:
	.type	.Ltest_bit_i32_1$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbs	%r1, %r2, 4, 0
	stw	%r2, %sp, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.6)
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end125:
	.size	test_bit_i32_1, .Lfunc_end125-test_bit_i32_1
	.size	.Ltest_bit_i32_1$local, .Lfunc_end125-test_bit_i32_1
                                        ! -- End function
	.globl	test_bit_i32_2                  ! -- Begin function test_bit_i32_2
	.p2align	4
	.type	test_bit_i32_2,@function
test_bit_i32_2:                         ! @test_bit_i32_2
.Ltest_bit_i32_2$local:
	.type	.Ltest_bit_i32_2$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbc	%r1, %r2, 4, 0
	stw	%r2, %sp, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.7)
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end126:
	.size	test_bit_i32_2, .Lfunc_end126-test_bit_i32_2
	.size	.Ltest_bit_i32_2$local, .Lfunc_end126-test_bit_i32_2
                                        ! -- End function
	.globl	test_bit_i32_3                  ! -- Begin function test_bit_i32_3
	.p2align	4
	.type	test_bit_i32_3,@function
test_bit_i32_3:                         ! @test_bit_i32_3
.Ltest_bit_i32_3$local:
	.type	.Ltest_bit_i32_3$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbs	%r1, %r2, 4, 0
	stw	%r2, %sp, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.8)
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end127:
	.size	test_bit_i32_3, .Lfunc_end127-test_bit_i32_3
	.size	.Ltest_bit_i32_3$local, .Lfunc_end127-test_bit_i32_3
                                        ! -- End function
	.globl	test_bit_i32_4                  ! -- Begin function test_bit_i32_4
	.p2align	4
	.type	test_bit_i32_4,@function
test_bit_i32_4:                         ! @test_bit_i32_4
.Ltest_bit_i32_4$local:
	.type	.Ltest_bit_i32_4$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbci	%r1, 15, 4, 0
	stw	%r2, %sp, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.9)
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end128:
	.size	test_bit_i32_4, .Lfunc_end128-test_bit_i32_4
	.size	.Ltest_bit_i32_4$local, .Lfunc_end128-test_bit_i32_4
                                        ! -- End function
	.globl	test_bit_i32_5                  ! -- Begin function test_bit_i32_5
	.p2align	4
	.type	test_bit_i32_5,@function
test_bit_i32_5:                         ! @test_bit_i32_5
.Ltest_bit_i32_5$local:
	.type	.Ltest_bit_i32_5$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	nulbsi	%r1, 14, 4, 0
	stw	%r2, %sp, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.10)
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end129:
	.size	test_bit_i32_5, .Lfunc_end129-test_bit_i32_5
	.size	.Ltest_bit_i32_5$local, .Lfunc_end129-test_bit_i32_5
                                        ! -- End function
	.globl	test_ball                       ! -- Begin function test_ball
	.p2align	4
	.type	test_ball,@function
test_ball:                              ! @test_ball
.Ltest_ball$local:
	.type	.Ltest_ball$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	mov	%r2, %r1
	nulmall	%r1, 2000, 4, 0

	! BUNDLE
	addi	%r1, %r2, 2000
	ldafr	%r4, %pcfwd(.L.str.11)
	std	%r1, %sp, 0

	! BUNDLE
	callr	%r3, %jpc(printf)
	nulmnotall	%r2, 1792, 4, 0
	addi	%r1, %r1, 5000

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.11)
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)

	! BUNDLE
	nulmnone	%r2, 1536, 4, 0
	addi	%r1, %r1, 3000
	ldafr	%r4, %pcfwd(.L.str.11)

	! BUNDLE
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	nulmany	%r2, 448, 4, 0

	! BUNDLE
	addi	%r1, %r1, 4000
	ldafr	%r4, %pcfwd(.L.str.11)
	std	%r1, %sp, 0

	! BUNDLE
	callr	%r3, %jpc(printf)
	retf	16
	nop	0

.Lfunc_end130:
	.size	test_ball, .Lfunc_end130-test_ball
	.size	.Ltest_ball$local, .Lfunc_end130-test_ball
                                        ! -- End function
	.globl	test_builtin_get_special_reg    ! -- Begin function test_builtin_get_special_reg
	.p2align	4
	.type	test_builtin_get_special_reg,@function
test_builtin_get_special_reg:           ! @test_builtin_get_special_reg
.Ltest_builtin_get_special_reg$local:
	.type	.Ltest_builtin_get_special_reg$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mfspr	%r2, %ip
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %eip
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %fpcr

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %eca
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %rsc
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %rsp

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %bsp
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %reip
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %kip

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %ksp
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %krsp
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %peb

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %teb
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %itc
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %itm

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %pta
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %iva
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %iip

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %iipa
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %cause
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %ifa

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %irr0
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %irr1
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %irr2

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %irr3
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %isr0
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %isr1

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %isr2
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %isr3
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %iv

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %lid
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %tpr
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %itcv

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %tsv
	callr	%r1, %jpc(consume_sr64)

	! BUNDLE
	mfspr	%r2, %pmv
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %cmcv

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %iib
	callr	%r1, %jpc(consume_sr128)

	! BUNDLE
	mfspr	%r2, %psr
	callr	%r1, %jpc(consume_sr64)
	mfspr	%r2, %ipsr

	! BUNDLE
	callr	%r1, %jpc(consume_sr64)
	retf	0
	nop	0

.Lfunc_end131:
	.size	test_builtin_get_special_reg, .Lfunc_end131-test_builtin_get_special_reg
	.size	.Ltest_builtin_get_special_reg$local, .Lfunc_end131-test_builtin_get_special_reg
                                        ! -- End function
	.globl	test_builtin_set_special_reg    ! -- Begin function test_builtin_set_special_reg
	.p2align	4
	.type	test_builtin_set_special_reg,@function
test_builtin_set_special_reg:           ! @test_builtin_set_special_reg
.Ltest_builtin_set_special_reg$local:
	.type	.Ltest_builtin_set_special_reg$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mtspr	%r1, %ip
	mtspr	%r1, %eip

	! BUNDLE
	mtspr	%r1, %fpcr
	mtspr	%r1, %eca
	mtspr	%r1, %rsc

	! BUNDLE
	mtspr	%r1, %rsp
	mtspr	%r1, %bsp
	mtspr	%r1, %reip

	! BUNDLE
	mtspr	%r1, %kip
	mtspr	%r1, %ksp
	mtspr	%r1, %krsp

	! BUNDLE
	mtspr	%r1, %peb
	mtspr	%r1, %teb
	mtspr	%r1, %itc

	! BUNDLE
	mtspr	%r1, %itm
	mtspr	%r1, %pta
	mtspr	%r1, %iva

	! BUNDLE
	mtspr	%r1, %iip
	mtspr	%r1, %iipa
	mtspr	%r1, %cause

	! BUNDLE
	mtspr	%r1, %ifa
	mtspr	%r1, %irr0
	mtspr	%r1, %irr1

	! BUNDLE
	mtspr	%r1, %irr2
	mtspr	%r1, %irr3
	mtspr	%r1, %isr0

	! BUNDLE
	mtspr	%r1, %isr1
	mtspr	%r1, %isr2
	mtspr	%r1, %isr3

	! BUNDLE
	mtspr	%r1, %iv
	mtspr	%r1, %lid
	mtspr	%r1, %tpr

	! BUNDLE
	mtspr	%r1, %itcv
	mtspr	%r1, %tsv
	mtspr	%r1, %pmv

	! BUNDLE
	mtspr	%r1, %cmcv
	mtspr	%r1, %psr
	mtspr	%r2, %iib

	! BUNDLE
	mtspr	%r1, %ipsr
	retf	0
	nop	0

.Lfunc_end132:
	.size	test_builtin_set_special_reg, .Lfunc_end132-test_builtin_set_special_reg
	.size	.Ltest_builtin_set_special_reg$local, .Lfunc_end132-test_builtin_set_special_reg
                                        ! -- End function
	.globl	test_i128_load_store            ! -- Begin function test_i128_load_store
	.p2align	4
	.type	test_i128_load_store,@function
test_i128_load_store:                   ! @test_i128_load_store
.Ltest_i128_load_store$local:
	.type	.Ltest_i128_load_store$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldq	%r2, %r1, 16
	stq	%r2, %r1, 0

	! BUNDLE
	ldq	%r1, %r1, 48
	retf	0
	nop	0

.Lfunc_end133:
	.size	test_i128_load_store, .Lfunc_end133-test_i128_load_store
	.size	.Ltest_i128_load_store$local, .Lfunc_end133-test_i128_load_store
                                        ! -- End function
	.globl	test_builtin_va                 ! -- Begin function test_builtin_va
	.p2align	4
	.type	test_builtin_va,@function
test_builtin_va:                        ! @test_builtin_va
.Ltest_builtin_va$local:
	.type	.Ltest_builtin_va$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	7, 16
	addi	%r2, %sp, 16
	ori	%r2, %r2, 4

	! BUNDLE
	std	%r2, %sp, 8
	ldwz	%r2, %sp, 16
	mov2	%r5, %r6, %r1, %r2

	! BUNDLE
	callr	%r4, %jpc(test_builtin_va_consume_i32)
	lddz	%r1, %sp, 8
	algnup	%r1, %r1, 0, 3

	! BUNDLE
	addi	%r3, %r1, 8
	std	%r3, %sp, 8
	lddz	%r6, %r1, 0

	! BUNDLE
	callr	%r5, %jpc(test_builtin_va_consume_f64)
	lddz	%r1, %sp, 8
	algnup	%r1, %r1, 0, 4

	! BUNDLE
	addi	%r3, %r1, 16
	std	%r3, %sp, 8
	ldq	%r6, %r1, 0

	! BUNDLE
	callr	%r5, %jpc(test_builtin_va_consume_f128)
	mov	%r1, %r2
	retf	16

.Lfunc_end134:
	.size	test_builtin_va, .Lfunc_end134-test_builtin_va
	.size	.Ltest_builtin_va$local, .Lfunc_end134-test_builtin_va
                                        ! -- End function
	.globl	test_test_builtin_va            ! -- Begin function test_test_builtin_va
	.p2align	4
	.type	test_test_builtin_va,@function
test_test_builtin_va:                   ! @test_test_builtin_va
.Ltest_test_builtin_va$local:
	.type	.Ltest_test_builtin_va$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	6, 272
	stq	%r3, %sp, 256
	stw	%r1, %sp, 240

	! BUNDLE
	std	%r2, %sp, 232
	stw	%r1, %sp, 224
	stq	%r3, %sp, 208

	! BUNDLE
	std	%r2, %sp, 200
	stw	%r1, %sp, 192
	stq	%r3, %sp, 176

	! BUNDLE
	std	%r2, %sp, 168
	stw	%r1, %sp, 160
	stq	%r3, %sp, 144

	! BUNDLE
	std	%r2, %sp, 136
	stw	%r1, %sp, 128
	stq	%r3, %sp, 112

	! BUNDLE
	std	%r2, %sp, 104
	stw	%r1, %sp, 96
	stq	%r3, %sp, 80

	! BUNDLE
	std	%r2, %sp, 72
	stw	%r1, %sp, 64
	stq	%r3, %sp, 48

	! BUNDLE
	std	%r2, %sp, 40
	stw	%r1, %sp, 32
	stq	%r3, %sp, 16

	! BUNDLE
	std	%r2, %sp, 8
	ldafr	%r5, %pcfwd(.L.str.12)
	stw	%r1, %sp, 0

	! BUNDLE
	callr	%r4, %jpc(test_builtin_va)
	mov	%r1, %r5
	retf	272

.Lfunc_end135:
	.size	test_test_builtin_va, .Lfunc_end135-test_test_builtin_va
	.size	.Ltest_test_builtin_va$local, .Lfunc_end135-test_test_builtin_va
                                        ! -- End function
	.globl	test_builtin_vector_i8          ! -- Begin function test_builtin_vector_i8
	.p2align	4
	.type	test_builtin_vector_i8,@function
test_builtin_vector_i8:                 ! @test_builtin_vector_i8
.Ltest_builtin_vector_i8$local:
	.type	.Ltest_builtin_vector_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	vaddcb	%r2, %r1, %r2
	vaddob	%r2, %r2, %r1

	! BUNDLE
	vaddssb	%r2, %r2, %r1
	vaddusb	%r2, %r2, %r1
	vaddub	%r2, %r2, %r1

	! BUNDLE
	vavgub	%r2, %r2, %r1
	vavgsb	%r2, %r2, %r1
	vcmpeqb	%r2, %r2, %r1

	! BUNDLE
	vcmpltb	%r2, %r2, %r1
	vcmpltub	%r2, %r2, %r1
	vmaxsb	%r2, %r2, %r1

	! BUNDLE
	vmaxub	%r2, %r2, %r1
	vminsb	%r2, %r2, %r1
	vminub	%r2, %r2, %r1

	! BUNDLE
	vrolb	%r2, %r2, %r1
	vrorb	%r2, %r2, %r1
	vsllb	%r2, %r2, %r1

	! BUNDLE
	vsrab	%r2, %r2, %r1
	vsrlb	%r2, %r2, %r1
	vsubbb	%r2, %r2, %r1

	! BUNDLE
	vsubob	%r2, %r2, %r1
	vsubub	%r2, %r2, %r1
	vsubssb	%r2, %r2, %r1

	! BUNDLE
	vsubusb	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end136:
	.size	test_builtin_vector_i8, .Lfunc_end136-test_builtin_vector_i8
	.size	.Ltest_builtin_vector_i8$local, .Lfunc_end136-test_builtin_vector_i8
                                        ! -- End function
	.globl	test_builtin_vector_i16         ! -- Begin function test_builtin_vector_i16
	.p2align	4
	.type	test_builtin_vector_i16,@function
test_builtin_vector_i16:                ! @test_builtin_vector_i16
.Ltest_builtin_vector_i16$local:
	.type	.Ltest_builtin_vector_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	vaddch	%r2, %r1, %r2
	vaddoh	%r2, %r2, %r1

	! BUNDLE
	vaddssh	%r2, %r2, %r1
	vaddush	%r2, %r2, %r1
	vadduh	%r2, %r2, %r1

	! BUNDLE
	vavguh	%r2, %r2, %r1
	vavgsh	%r2, %r2, %r1
	vcmpeqh	%r2, %r2, %r1

	! BUNDLE
	vcmplth	%r2, %r2, %r1
	vcmpltuh	%r2, %r2, %r1
	vmaxsh	%r2, %r2, %r1

	! BUNDLE
	vmaxuh	%r2, %r2, %r1
	vminsh	%r2, %r2, %r1
	vminuh	%r2, %r2, %r1

	! BUNDLE
	vrolh	%r2, %r2, %r1
	vrorh	%r2, %r2, %r1
	vsllh	%r2, %r2, %r1

	! BUNDLE
	vsrah	%r2, %r2, %r1
	vsrlh	%r2, %r2, %r1
	vsubbh	%r2, %r2, %r1

	! BUNDLE
	vsuboh	%r2, %r2, %r1
	vsubuh	%r2, %r2, %r1
	vsubssh	%r2, %r2, %r1

	! BUNDLE
	vsubush	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end137:
	.size	test_builtin_vector_i16, .Lfunc_end137-test_builtin_vector_i16
	.size	.Ltest_builtin_vector_i16$local, .Lfunc_end137-test_builtin_vector_i16
                                        ! -- End function
	.globl	test_builtin_vector_i32         ! -- Begin function test_builtin_vector_i32
	.p2align	4
	.type	test_builtin_vector_i32,@function
test_builtin_vector_i32:                ! @test_builtin_vector_i32
.Ltest_builtin_vector_i32$local:
	.type	.Ltest_builtin_vector_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	vaddcw	%r2, %r1, %r2
	vaddow	%r2, %r2, %r1

	! BUNDLE
	vaddssw	%r2, %r2, %r1
	vaddusw	%r2, %r2, %r1
	vadduw	%r2, %r2, %r1

	! BUNDLE
	vavguw	%r2, %r2, %r1
	vavgsw	%r2, %r2, %r1
	vcmpeqw	%r2, %r2, %r1

	! BUNDLE
	vcmpltw	%r2, %r2, %r1
	vcmpltuw	%r2, %r2, %r1
	vmaxsw	%r2, %r2, %r1

	! BUNDLE
	vmaxuw	%r2, %r2, %r1
	vminsw	%r2, %r2, %r1
	vminuw	%r2, %r2, %r1

	! BUNDLE
	vrolw	%r2, %r2, %r1
	vrorw	%r2, %r2, %r1
	vsllw	%r2, %r2, %r1

	! BUNDLE
	vsraw	%r2, %r2, %r1
	vsrlw	%r2, %r2, %r1
	vsubbw	%r2, %r2, %r1

	! BUNDLE
	vsubow	%r2, %r2, %r1
	vsubuw	%r2, %r2, %r1
	vsubssw	%r2, %r2, %r1

	! BUNDLE
	vsubusw	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end138:
	.size	test_builtin_vector_i32, .Lfunc_end138-test_builtin_vector_i32
	.size	.Ltest_builtin_vector_i32$local, .Lfunc_end138-test_builtin_vector_i32
                                        ! -- End function
	.globl	test_builtin_vector_i64         ! -- Begin function test_builtin_vector_i64
	.p2align	4
	.type	test_builtin_vector_i64,@function
test_builtin_vector_i64:                ! @test_builtin_vector_i64
.Ltest_builtin_vector_i64$local:
	.type	.Ltest_builtin_vector_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	vaddcd	%r2, %r1, %r2
	vaddod	%r2, %r2, %r1

	! BUNDLE
	vaddssd	%r2, %r2, %r1
	vaddusd	%r2, %r2, %r1
	vaddud	%r2, %r2, %r1

	! BUNDLE
	vavgud	%r2, %r2, %r1
	vavgsd	%r2, %r2, %r1
	vcmpeqd	%r2, %r2, %r1

	! BUNDLE
	vcmpltd	%r2, %r2, %r1
	vcmpltud	%r2, %r2, %r1
	vmaxsd	%r2, %r2, %r1

	! BUNDLE
	vmaxud	%r2, %r2, %r1
	vminsd	%r2, %r2, %r1
	vminud	%r2, %r2, %r1

	! BUNDLE
	vrold	%r2, %r2, %r1
	vrord	%r2, %r2, %r1
	vslld	%r2, %r2, %r1

	! BUNDLE
	vsrad	%r2, %r2, %r1
	vsrld	%r2, %r2, %r1
	vsubbd	%r2, %r2, %r1

	! BUNDLE
	vsubod	%r2, %r2, %r1
	vsubud	%r2, %r2, %r1
	vsubssd	%r2, %r2, %r1

	! BUNDLE
	vsubusd	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end139:
	.size	test_builtin_vector_i64, .Lfunc_end139-test_builtin_vector_i64
	.size	.Ltest_builtin_vector_i64$local, .Lfunc_end139-test_builtin_vector_i64
                                        ! -- End function
	.globl	test_builtin_vector_mixed       ! -- Begin function test_builtin_vector_mixed
	.p2align	4
	.type	test_builtin_vector_mixed,@function
test_builtin_vector_mixed:              ! @test_builtin_vector_mixed
.Ltest_builtin_vector_mixed$local:
	.type	.Ltest_builtin_vector_mixed$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	9
	vmrghb	%r1, %r1, %r2
	vmrglb	%r1, %r1, %r2

	! BUNDLE
	vmrghh	%r1, %r1, %r4
	vmrglh	%r1, %r1, %r4
	vmrghw	%r1, %r1, %r6

	! BUNDLE
	vmrglw	%r1, %r1, %r6
	vmrghd	%r1, %r1, %r8
	vmrgld	%r1, %r1, %r8

	! BUNDLE
	vpksssh	%r1, %r1, %r7
	vpksssw	%r1, %r1, %r5
	vpksssd	%r1, %r1, %r7

	! BUNDLE
	vpksush	%r1, %r1, %r3
	vpksusw	%r1, %r1, %r6
	vpksusd	%r1, %r1, %r8

	! BUNDLE
	vpkuumh	%r1, %r1, %r4
	vpkuumw	%r1, %r1, %r6
	vpkuumd	%r1, %r1, %r8

	! BUNDLE
	vpkuush	%r1, %r1, %r4
	vpkuusw	%r1, %r1, %r6
	vpkuusd	%r1, %r1, %r8

	! BUNDLE
	vupkhsb	%r1, %r1
	vupkhsh	%r1, %r1
	vupkhsw	%r1, %r1

	! BUNDLE
	vupklsb	%r1, %r1
	vupklsh	%r1, %r1
	vupklsw	%r1, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end140:
	.size	test_builtin_vector_mixed, .Lfunc_end140-test_builtin_vector_mixed
	.size	.Ltest_builtin_vector_mixed$local, .Lfunc_end140-test_builtin_vector_mixed
                                        ! -- End function
	.globl	test_builtin_popcount           ! -- Begin function test_builtin_popcount
	.p2align	4
	.type	test_builtin_popcount,@function
test_builtin_popcount:                  ! @test_builtin_popcount
.Ltest_builtin_popcount$local:
	.type	.Ltest_builtin_popcount$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	popc	%r1, %r1, 31
	retf	0

.Lfunc_end141:
	.size	test_builtin_popcount, .Lfunc_end141-test_builtin_popcount
	.size	.Ltest_builtin_popcount$local, .Lfunc_end141-test_builtin_popcount
                                        ! -- End function
	.globl	test_builtin_popcountl          ! -- Begin function test_builtin_popcountl
	.p2align	4
	.type	test_builtin_popcountl,@function
test_builtin_popcountl:                 ! @test_builtin_popcountl
.Ltest_builtin_popcountl$local:
	.type	.Ltest_builtin_popcountl$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	popc	%r1, %r1, 63
	retf	0

.Lfunc_end142:
	.size	test_builtin_popcountl, .Lfunc_end142-test_builtin_popcountl
	.size	.Ltest_builtin_popcountl$local, .Lfunc_end142-test_builtin_popcountl
                                        ! -- End function
	.globl	test_builtin_ctz                ! -- Begin function test_builtin_ctz
	.p2align	4
	.type	test_builtin_ctz,@function
test_builtin_ctz:                       ! @test_builtin_ctz
.Ltest_builtin_ctz$local:
	.type	.Ltest_builtin_ctz$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cttz	%r1, %r1, 31
	retf	0

.Lfunc_end143:
	.size	test_builtin_ctz, .Lfunc_end143-test_builtin_ctz
	.size	.Ltest_builtin_ctz$local, .Lfunc_end143-test_builtin_ctz
                                        ! -- End function
	.globl	test_builtin_ctzl               ! -- Begin function test_builtin_ctzl
	.p2align	4
	.type	test_builtin_ctzl,@function
test_builtin_ctzl:                      ! @test_builtin_ctzl
.Ltest_builtin_ctzl$local:
	.type	.Ltest_builtin_ctzl$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cttz	%r1, %r1, 63
	retf	0

.Lfunc_end144:
	.size	test_builtin_ctzl, .Lfunc_end144-test_builtin_ctzl
	.size	.Ltest_builtin_ctzl$local, .Lfunc_end144-test_builtin_ctzl
                                        ! -- End function
	.globl	test_builtin_clz                ! -- Begin function test_builtin_clz
	.p2align	4
	.type	test_builtin_clz,@function
test_builtin_clz:                       ! @test_builtin_clz
.Ltest_builtin_clz$local:
	.type	.Ltest_builtin_clz$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ctlz	%r1, %r1, 31
	retf	0

.Lfunc_end145:
	.size	test_builtin_clz, .Lfunc_end145-test_builtin_clz
	.size	.Ltest_builtin_clz$local, .Lfunc_end145-test_builtin_clz
                                        ! -- End function
	.globl	test_builtin_clzl               ! -- Begin function test_builtin_clzl
	.p2align	4
	.type	test_builtin_clzl,@function
test_builtin_clzl:                      ! @test_builtin_clzl
.Ltest_builtin_clzl$local:
	.type	.Ltest_builtin_clzl$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ctlz	%r1, %r1, 63
	retf	0

.Lfunc_end146:
	.size	test_builtin_clzl, .Lfunc_end146-test_builtin_clzl
	.size	.Ltest_builtin_clzl$local, .Lfunc_end146-test_builtin_clzl
                                        ! -- End function
	.globl	test_builtin_readcyclecounter   ! -- Begin function test_builtin_readcyclecounter
	.p2align	4
	.type	test_builtin_readcyclecounter,@function
test_builtin_readcyclecounter:          ! @test_builtin_readcyclecounter
.Ltest_builtin_readcyclecounter$local:
	.type	.Ltest_builtin_readcyclecounter$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mfspr	%r1, %itc
	retf	0

.Lfunc_end147:
	.size	test_builtin_readcyclecounter, .Lfunc_end147-test_builtin_readcyclecounter
	.size	.Ltest_builtin_readcyclecounter$local, .Lfunc_end147-test_builtin_readcyclecounter
                                        ! -- End function
	.globl	test_builtin_thread_pointer     ! -- Begin function test_builtin_thread_pointer
	.p2align	4
	.type	test_builtin_thread_pointer,@function
test_builtin_thread_pointer:            ! @test_builtin_thread_pointer
.Ltest_builtin_thread_pointer$local:
	.type	.Ltest_builtin_thread_pointer$local,@function
	.register %sp, #scratch
	.register %tp, #ignore
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mov	%r1, %tp
	retf	0

.Lfunc_end148:
	.size	test_builtin_thread_pointer, .Lfunc_end148-test_builtin_thread_pointer
	.size	.Ltest_builtin_thread_pointer$local, .Lfunc_end148-test_builtin_thread_pointer
                                        ! -- End function
	.globl	test_builtin_set_thread_pointer ! -- Begin function test_builtin_set_thread_pointer
	.p2align	4
	.type	test_builtin_set_thread_pointer,@function
test_builtin_set_thread_pointer:        ! @test_builtin_set_thread_pointer
.Ltest_builtin_set_thread_pointer$local:
	.type	.Ltest_builtin_set_thread_pointer$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mov	%tp, %r1
	retf	0

.Lfunc_end149:
	.size	test_builtin_set_thread_pointer, .Lfunc_end149-test_builtin_set_thread_pointer
	.size	.Ltest_builtin_set_thread_pointer$local, .Lfunc_end149-test_builtin_set_thread_pointer
                                        ! -- End function
	.globl	test_frame_buitin               ! -- Begin function test_frame_buitin
	.p2align	4
	.type	test_frame_buitin,@function
test_frame_buitin:                      ! @test_frame_buitin
.Ltest_frame_buitin$local:
	.type	.Ltest_frame_buitin$local,@function
	.register %fp, #scratch
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	3, 0
	mov	%r2, %fp
	addi	%fp, %sp, 0

	! BUNDLE
	add	%r1, %r0, %fp
	mov	%sp, %fp
	mov	%fp, %r2

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end150:
	.size	test_frame_buitin, .Lfunc_end150-test_frame_buitin
	.size	.Ltest_frame_buitin$local, .Lfunc_end150-test_frame_buitin
                                        ! -- End function
	.globl	test_builtin_stack_pointer      ! -- Begin function test_builtin_stack_pointer
	.p2align	4
	.type	test_builtin_stack_pointer,@function
test_builtin_stack_pointer:             ! @test_builtin_stack_pointer
.Ltest_builtin_stack_pointer$local:
	.type	.Ltest_builtin_stack_pointer$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mov	%r1, %sp
	retf	0

.Lfunc_end151:
	.size	test_builtin_stack_pointer, .Lfunc_end151-test_builtin_stack_pointer
	.size	.Ltest_builtin_stack_pointer$local, .Lfunc_end151-test_builtin_stack_pointer
                                        ! -- End function
	.globl	test_builtin_bitreverse         ! -- Begin function test_builtin_bitreverse
	.p2align	4
	.type	test_builtin_bitreverse,@function
test_builtin_bitreverse:                ! @test_builtin_bitreverse
.Ltest_builtin_bitreverse$local:
	.type	.Ltest_builtin_bitreverse$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	6, 16
	addw	%r1, %r2, %r1
	permb	%r3, %r1, 31

	! BUNDLE
	ldafr	%r5, %pcfwd(.L.str.13)
	stw	%r3, %sp, 0
	callr	%r4, %jpc(printf)

	! BUNDLE
	srawi	%r1, %r2, 0
	addi	%r1, %r1, 11
	permb	%r2, %r1, 63

	! BUNDLE
	ldafr	%r5, %pcfwd(.L.str.14)
	std	%r2, %sp, 0
	callr	%r4, %jpc(printf)

	! BUNDLE
	ldax.ws	%r1, %r2, %r3, 0, 0
	retf	16
	nop	0

.Lfunc_end152:
	.size	test_builtin_bitreverse, .Lfunc_end152-test_builtin_bitreverse
	.size	.Ltest_builtin_bitreverse$local, .Lfunc_end152-test_builtin_bitreverse
                                        ! -- End function
	.globl	test_builtin_halt               ! -- Begin function test_builtin_halt
	.p2align	4
	.type	test_builtin_halt,@function
test_builtin_halt:                      ! @test_builtin_halt
.Ltest_builtin_halt$local:
	.type	.Ltest_builtin_halt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	halt
	retf	0

.Lfunc_end153:
	.size	test_builtin_halt, .Lfunc_end153-test_builtin_halt
	.size	.Ltest_builtin_halt$local, .Lfunc_end153-test_builtin_halt
                                        ! -- End function
	.globl	test_builtin_undef              ! -- Begin function test_builtin_undef
	.p2align	4
	.type	test_builtin_undef,@function
test_builtin_undef:                     ! @test_builtin_undef
.Ltest_builtin_undef$local:
	.type	.Ltest_builtin_undef$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	undef
	retf	0

.Lfunc_end154:
	.size	test_builtin_undef, .Lfunc_end154-test_builtin_undef
	.size	.Ltest_builtin_undef$local, .Lfunc_end154-test_builtin_undef
                                        ! -- End function
	.globl	test_builtin_rfi                ! -- Begin function test_builtin_rfi
	.p2align	4
	.type	test_builtin_rfi,@function
test_builtin_rfi:                       ! @test_builtin_rfi
.Ltest_builtin_rfi$local:
	.type	.Ltest_builtin_rfi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	rfi
	retf	0

.Lfunc_end155:
	.size	test_builtin_rfi, .Lfunc_end155-test_builtin_rfi
	.size	.Ltest_builtin_rfi$local, .Lfunc_end155-test_builtin_rfi
                                        ! -- End function
	.globl	test_builtin_nop                ! -- Begin function test_builtin_nop
	.p2align	4
	.type	test_builtin_nop,@function
test_builtin_nop:                       ! @test_builtin_nop
.Ltest_builtin_nop$local:
	.type	.Ltest_builtin_nop$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	nop	1234567
	nop	2

	! BUNDLE
	nop.l	123456789123456789
	nop	123

	! BUNDLE
	nop.l	123
	retf	0

.Lfunc_end156:
	.size	test_builtin_nop, .Lfunc_end156-test_builtin_nop
	.size	.Ltest_builtin_nop$local, .Lfunc_end156-test_builtin_nop
                                        ! -- End function
	.globl	test_builtin_tpa                ! -- Begin function test_builtin_tpa
	.p2align	4
	.type	test_builtin_tpa,@function
test_builtin_tpa:                       ! @test_builtin_tpa
.Ltest_builtin_tpa$local:
	.type	.Ltest_builtin_tpa$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	tpa	%r1, %r1
	retf	0

.Lfunc_end157:
	.size	test_builtin_tpa, .Lfunc_end157-test_builtin_tpa
	.size	.Ltest_builtin_tpa$local, .Lfunc_end157-test_builtin_tpa
                                        ! -- End function
	.globl	test_clmul                      ! -- Begin function test_clmul
	.p2align	4
	.type	test_clmul,@function
test_clmul:                             ! @test_clmul
.Ltest_clmul$local:
	.type	.Ltest_clmul$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	clmulll	%r3, %r1, %r2
	clmulhl	%r1, %r1, %r3

	! BUNDLE
	clmulhh	%r1, %r1, %r2
	retf	0
	nop	0

.Lfunc_end158:
	.size	test_clmul, .Lfunc_end158-test_clmul
	.size	.Ltest_clmul$local, .Lfunc_end158-test_clmul
                                        ! -- End function
	.globl	test_bool_eq                    ! -- Begin function test_bool_eq
	.p2align	4
	.type	test_bool_eq,@function
test_bool_eq:                           ! @test_bool_eq
.Ltest_bool_eq$local:
	.type	.Ltest_bool_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xor	%r1, %r1, %r2
	xori	%r1, %r1, 1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end159:
	.size	test_bool_eq, .Lfunc_end159-test_bool_eq
	.size	.Ltest_bool_eq$local, .Lfunc_end159-test_bool_eq
                                        ! -- End function
	.globl	test_bool_ne                    ! -- Begin function test_bool_ne
	.p2align	4
	.type	test_bool_ne,@function
test_bool_ne:                           ! @test_bool_ne
.Ltest_bool_ne$local:
	.type	.Ltest_bool_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xor	%r1, %r1, %r2
	retf	0

.Lfunc_end160:
	.size	test_bool_ne, .Lfunc_end160-test_bool_ne
	.size	.Ltest_bool_ne$local, .Lfunc_end160-test_bool_ne
                                        ! -- End function
	.globl	test_bool_lt                    ! -- Begin function test_bool_lt
	.p2align	4
	.type	test_bool_lt,@function
test_bool_lt:                           ! @test_bool_lt
.Ltest_bool_lt$local:
	.type	.Ltest_bool_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xori	%r1, %r1, 1
	and	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end161:
	.size	test_bool_lt, .Lfunc_end161-test_bool_lt
	.size	.Ltest_bool_lt$local, .Lfunc_end161-test_bool_lt
                                        ! -- End function
	.globl	test_bool_le                    ! -- Begin function test_bool_le
	.p2align	4
	.type	test_bool_le,@function
test_bool_le:                           ! @test_bool_le
.Ltest_bool_le$local:
	.type	.Ltest_bool_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xori	%r1, %r1, 1
	or	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end162:
	.size	test_bool_le, .Lfunc_end162-test_bool_le
	.size	.Ltest_bool_le$local, .Lfunc_end162-test_bool_le
                                        ! -- End function
	.globl	test_bool_gt                    ! -- Begin function test_bool_gt
	.p2align	4
	.type	test_bool_gt,@function
test_bool_gt:                           ! @test_bool_gt
.Ltest_bool_gt$local:
	.type	.Ltest_bool_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xori	%r2, %r2, 1
	and	%r1, %r1, %r2

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end163:
	.size	test_bool_gt, .Lfunc_end163-test_bool_gt
	.size	.Ltest_bool_gt$local, .Lfunc_end163-test_bool_gt
                                        ! -- End function
	.globl	test_bool_ge                    ! -- Begin function test_bool_ge
	.p2align	4
	.type	test_bool_ge,@function
test_bool_ge:                           ! @test_bool_ge
.Ltest_bool_ge$local:
	.type	.Ltest_bool_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xori	%r2, %r2, 1
	or	%r1, %r1, %r2

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end164:
	.size	test_bool_ge, .Lfunc_end164-test_bool_ge
	.size	.Ltest_bool_ge$local, .Lfunc_end164-test_bool_ge
                                        ! -- End function
	.globl	test_ptr_eq                     ! -- Begin function test_ptr_eq
	.p2align	4
	.type	test_ptr_eq,@function
test_ptr_eq:                            ! @test_ptr_eq
.Ltest_ptr_eq$local:
	.type	.Ltest_ptr_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdeq	%r1, %r1, %r2
	retf	0

.Lfunc_end165:
	.size	test_ptr_eq, .Lfunc_end165-test_ptr_eq
	.size	.Ltest_ptr_eq$local, .Lfunc_end165-test_ptr_eq
                                        ! -- End function
	.globl	test_ptr_ne                     ! -- Begin function test_ptr_ne
	.p2align	4
	.type	test_ptr_ne,@function
test_ptr_ne:                            ! @test_ptr_ne
.Ltest_ptr_ne$local:
	.type	.Ltest_ptr_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdne	%r1, %r1, %r2
	retf	0

.Lfunc_end166:
	.size	test_ptr_ne, .Lfunc_end166-test_ptr_ne
	.size	.Ltest_ptr_ne$local, .Lfunc_end166-test_ptr_ne
                                        ! -- End function
	.globl	test_ptr_lt                     ! -- Begin function test_ptr_lt
	.p2align	4
	.type	test_ptr_lt,@function
test_ptr_lt:                            ! @test_ptr_lt
.Ltest_ptr_lt$local:
	.type	.Ltest_ptr_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdltu	%r1, %r1, %r2
	retf	0

.Lfunc_end167:
	.size	test_ptr_lt, .Lfunc_end167-test_ptr_lt
	.size	.Ltest_ptr_lt$local, .Lfunc_end167-test_ptr_lt
                                        ! -- End function
	.globl	test_ptr_le                     ! -- Begin function test_ptr_le
	.p2align	4
	.type	test_ptr_le,@function
test_ptr_le:                            ! @test_ptr_le
.Ltest_ptr_le$local:
	.type	.Ltest_ptr_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdleu	%r1, %r1, %r2
	retf	0

.Lfunc_end168:
	.size	test_ptr_le, .Lfunc_end168-test_ptr_le
	.size	.Ltest_ptr_le$local, .Lfunc_end168-test_ptr_le
                                        ! -- End function
	.globl	test_ptr_gt                     ! -- Begin function test_ptr_gt
	.p2align	4
	.type	test_ptr_gt,@function
test_ptr_gt:                            ! @test_ptr_gt
.Ltest_ptr_gt$local:
	.type	.Ltest_ptr_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdltu	%r1, %r2, %r1
	retf	0

.Lfunc_end169:
	.size	test_ptr_gt, .Lfunc_end169-test_ptr_gt
	.size	.Ltest_ptr_gt$local, .Lfunc_end169-test_ptr_gt
                                        ! -- End function
	.globl	test_ptr_ge                     ! -- Begin function test_ptr_ge
	.p2align	4
	.type	test_ptr_ge,@function
test_ptr_ge:                            ! @test_ptr_ge
.Ltest_ptr_ge$local:
	.type	.Ltest_ptr_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdleu	%r1, %r2, %r1
	retf	0

.Lfunc_end170:
	.size	test_ptr_ge, .Lfunc_end170-test_ptr_ge
	.size	.Ltest_ptr_ge$local, .Lfunc_end170-test_ptr_ge
                                        ! -- End function
	.globl	test_i64_eq                     ! -- Begin function test_i64_eq
	.p2align	4
	.type	test_i64_eq,@function
test_i64_eq:                            ! @test_i64_eq
.Ltest_i64_eq$local:
	.type	.Ltest_i64_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdeq	%r1, %r1, %r2
	retf	0

.Lfunc_end171:
	.size	test_i64_eq, .Lfunc_end171-test_i64_eq
	.size	.Ltest_i64_eq$local, .Lfunc_end171-test_i64_eq
                                        ! -- End function
	.globl	test_i64_ne                     ! -- Begin function test_i64_ne
	.p2align	4
	.type	test_i64_ne,@function
test_i64_ne:                            ! @test_i64_ne
.Ltest_i64_ne$local:
	.type	.Ltest_i64_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdne	%r1, %r1, %r2
	retf	0

.Lfunc_end172:
	.size	test_i64_ne, .Lfunc_end172-test_i64_ne
	.size	.Ltest_i64_ne$local, .Lfunc_end172-test_i64_ne
                                        ! -- End function
	.globl	test_i64_lt                     ! -- Begin function test_i64_lt
	.p2align	4
	.type	test_i64_lt,@function
test_i64_lt:                            ! @test_i64_lt
.Ltest_i64_lt$local:
	.type	.Ltest_i64_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdlt	%r1, %r1, %r2
	retf	0

.Lfunc_end173:
	.size	test_i64_lt, .Lfunc_end173-test_i64_lt
	.size	.Ltest_i64_lt$local, .Lfunc_end173-test_i64_lt
                                        ! -- End function
	.globl	test_i64_le                     ! -- Begin function test_i64_le
	.p2align	4
	.type	test_i64_le,@function
test_i64_le:                            ! @test_i64_le
.Ltest_i64_le$local:
	.type	.Ltest_i64_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdle	%r1, %r1, %r2
	retf	0

.Lfunc_end174:
	.size	test_i64_le, .Lfunc_end174-test_i64_le
	.size	.Ltest_i64_le$local, .Lfunc_end174-test_i64_le
                                        ! -- End function
	.globl	test_i64_gt                     ! -- Begin function test_i64_gt
	.p2align	4
	.type	test_i64_gt,@function
test_i64_gt:                            ! @test_i64_gt
.Ltest_i64_gt$local:
	.type	.Ltest_i64_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdlt	%r1, %r2, %r1
	retf	0

.Lfunc_end175:
	.size	test_i64_gt, .Lfunc_end175-test_i64_gt
	.size	.Ltest_i64_gt$local, .Lfunc_end175-test_i64_gt
                                        ! -- End function
	.globl	test_i64_ge                     ! -- Begin function test_i64_ge
	.p2align	4
	.type	test_i64_ge,@function
test_i64_ge:                            ! @test_i64_ge
.Ltest_i64_ge$local:
	.type	.Ltest_i64_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdle	%r1, %r2, %r1
	retf	0

.Lfunc_end176:
	.size	test_i64_ge, .Lfunc_end176-test_i64_ge
	.size	.Ltest_i64_ge$local, .Lfunc_end176-test_i64_ge
                                        ! -- End function
	.globl	test_u64_lt                     ! -- Begin function test_u64_lt
	.p2align	4
	.type	test_u64_lt,@function
test_u64_lt:                            ! @test_u64_lt
.Ltest_u64_lt$local:
	.type	.Ltest_u64_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdltu	%r1, %r1, %r2
	retf	0

.Lfunc_end177:
	.size	test_u64_lt, .Lfunc_end177-test_u64_lt
	.size	.Ltest_u64_lt$local, .Lfunc_end177-test_u64_lt
                                        ! -- End function
	.globl	test_u64_le                     ! -- Begin function test_u64_le
	.p2align	4
	.type	test_u64_le,@function
test_u64_le:                            ! @test_u64_le
.Ltest_u64_le$local:
	.type	.Ltest_u64_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdleu	%r1, %r1, %r2
	retf	0

.Lfunc_end178:
	.size	test_u64_le, .Lfunc_end178-test_u64_le
	.size	.Ltest_u64_le$local, .Lfunc_end178-test_u64_le
                                        ! -- End function
	.globl	test_u64_gt                     ! -- Begin function test_u64_gt
	.p2align	4
	.type	test_u64_gt,@function
test_u64_gt:                            ! @test_u64_gt
.Ltest_u64_gt$local:
	.type	.Ltest_u64_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdltu	%r1, %r2, %r1
	retf	0

.Lfunc_end179:
	.size	test_u64_gt, .Lfunc_end179-test_u64_gt
	.size	.Ltest_u64_gt$local, .Lfunc_end179-test_u64_gt
                                        ! -- End function
	.globl	test_u64_ge                     ! -- Begin function test_u64_ge
	.p2align	4
	.type	test_u64_ge,@function
test_u64_ge:                            ! @test_u64_ge
.Ltest_u64_ge$local:
	.type	.Ltest_u64_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpdleu	%r1, %r2, %r1
	retf	0

.Lfunc_end180:
	.size	test_u64_ge, .Lfunc_end180-test_u64_ge
	.size	.Ltest_u64_ge$local, .Lfunc_end180-test_u64_ge
                                        ! -- End function
	.globl	test_i64_eq_imm                 ! -- Begin function test_i64_eq_imm
	.p2align	4
	.type	test_i64_eq_imm,@function
test_i64_eq_imm:                        ! @test_i64_eq_imm
.Ltest_i64_eq_imm$local:
	.type	.Ltest_i64_eq_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdeqi	%r1, %r1, 1234
	retf	0

.Lfunc_end181:
	.size	test_i64_eq_imm, .Lfunc_end181-test_i64_eq_imm
	.size	.Ltest_i64_eq_imm$local, .Lfunc_end181-test_i64_eq_imm
                                        ! -- End function
	.globl	test_i64_ne_imm                 ! -- Begin function test_i64_ne_imm
	.p2align	4
	.type	test_i64_ne_imm,@function
test_i64_ne_imm:                        ! @test_i64_ne_imm
.Ltest_i64_ne_imm$local:
	.type	.Ltest_i64_ne_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdnei	%r1, %r1, 1234
	retf	0

.Lfunc_end182:
	.size	test_i64_ne_imm, .Lfunc_end182-test_i64_ne_imm
	.size	.Ltest_i64_ne_imm$local, .Lfunc_end182-test_i64_ne_imm
                                        ! -- End function
	.globl	test_i64_lt_imm                 ! -- Begin function test_i64_lt_imm
	.p2align	4
	.type	test_i64_lt_imm,@function
test_i64_lt_imm:                        ! @test_i64_lt_imm
.Ltest_i64_lt_imm$local:
	.type	.Ltest_i64_lt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdlti	%r1, %r1, 1234
	retf	0

.Lfunc_end183:
	.size	test_i64_lt_imm, .Lfunc_end183-test_i64_lt_imm
	.size	.Ltest_i64_lt_imm$local, .Lfunc_end183-test_i64_lt_imm
                                        ! -- End function
	.globl	test_i64_gt_imm                 ! -- Begin function test_i64_gt_imm
	.p2align	4
	.type	test_i64_gt_imm,@function
test_i64_gt_imm:                        ! @test_i64_gt_imm
.Ltest_i64_gt_imm$local:
	.type	.Ltest_i64_gt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdgti	%r1, %r1, 1234
	retf	0

.Lfunc_end184:
	.size	test_i64_gt_imm, .Lfunc_end184-test_i64_gt_imm
	.size	.Ltest_i64_gt_imm$local, .Lfunc_end184-test_i64_gt_imm
                                        ! -- End function
	.globl	test_i64_le_imm                 ! -- Begin function test_i64_le_imm
	.p2align	4
	.type	test_i64_le_imm,@function
test_i64_le_imm:                        ! @test_i64_le_imm
.Ltest_i64_le_imm$local:
	.type	.Ltest_i64_le_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdlti	%r1, %r1, 1235
	retf	0

.Lfunc_end185:
	.size	test_i64_le_imm, .Lfunc_end185-test_i64_le_imm
	.size	.Ltest_i64_le_imm$local, .Lfunc_end185-test_i64_le_imm
                                        ! -- End function
	.globl	test_i64_ge_imm                 ! -- Begin function test_i64_ge_imm
	.p2align	4
	.type	test_i64_ge_imm,@function
test_i64_ge_imm:                        ! @test_i64_ge_imm
.Ltest_i64_ge_imm$local:
	.type	.Ltest_i64_ge_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdgti	%r1, %r1, 1233
	retf	0

.Lfunc_end186:
	.size	test_i64_ge_imm, .Lfunc_end186-test_i64_ge_imm
	.size	.Ltest_i64_ge_imm$local, .Lfunc_end186-test_i64_ge_imm
                                        ! -- End function
	.globl	test_u64_lt_imm                 ! -- Begin function test_u64_lt_imm
	.p2align	4
	.type	test_u64_lt_imm,@function
test_u64_lt_imm:                        ! @test_u64_lt_imm
.Ltest_u64_lt_imm$local:
	.type	.Ltest_u64_lt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdltui	%r1, %r1, 1234
	retf	0

.Lfunc_end187:
	.size	test_u64_lt_imm, .Lfunc_end187-test_u64_lt_imm
	.size	.Ltest_u64_lt_imm$local, .Lfunc_end187-test_u64_lt_imm
                                        ! -- End function
	.globl	test_u64_gt_imm                 ! -- Begin function test_u64_gt_imm
	.p2align	4
	.type	test_u64_gt_imm,@function
test_u64_gt_imm:                        ! @test_u64_gt_imm
.Ltest_u64_gt_imm$local:
	.type	.Ltest_u64_gt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdgtui	%r1, %r1, 1234
	retf	0

.Lfunc_end188:
	.size	test_u64_gt_imm, .Lfunc_end188-test_u64_gt_imm
	.size	.Ltest_u64_gt_imm$local, .Lfunc_end188-test_u64_gt_imm
                                        ! -- End function
	.globl	test_u64_le_imm                 ! -- Begin function test_u64_le_imm
	.p2align	4
	.type	test_u64_le_imm,@function
test_u64_le_imm:                        ! @test_u64_le_imm
.Ltest_u64_le_imm$local:
	.type	.Ltest_u64_le_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdltui	%r1, %r1, 1235
	retf	0

.Lfunc_end189:
	.size	test_u64_le_imm, .Lfunc_end189-test_u64_le_imm
	.size	.Ltest_u64_le_imm$local, .Lfunc_end189-test_u64_le_imm
                                        ! -- End function
	.globl	test_u64_ge_imm                 ! -- Begin function test_u64_ge_imm
	.p2align	4
	.type	test_u64_ge_imm,@function
test_u64_ge_imm:                        ! @test_u64_ge_imm
.Ltest_u64_ge_imm$local:
	.type	.Ltest_u64_ge_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpdgtui	%r1, %r1, 1233
	retf	0

.Lfunc_end190:
	.size	test_u64_ge_imm, .Lfunc_end190-test_u64_ge_imm
	.size	.Ltest_u64_ge_imm$local, .Lfunc_end190-test_u64_ge_imm
                                        ! -- End function
	.globl	test_i32_eq                     ! -- Begin function test_i32_eq
	.p2align	4
	.type	test_i32_eq,@function
test_i32_eq:                            ! @test_i32_eq
.Ltest_i32_eq$local:
	.type	.Ltest_i32_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpweq	%r1, %r1, %r2
	retf	0

.Lfunc_end191:
	.size	test_i32_eq, .Lfunc_end191-test_i32_eq
	.size	.Ltest_i32_eq$local, .Lfunc_end191-test_i32_eq
                                        ! -- End function
	.globl	test_i32_ne                     ! -- Begin function test_i32_ne
	.p2align	4
	.type	test_i32_ne,@function
test_i32_ne:                            ! @test_i32_ne
.Ltest_i32_ne$local:
	.type	.Ltest_i32_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwne	%r1, %r1, %r2
	retf	0

.Lfunc_end192:
	.size	test_i32_ne, .Lfunc_end192-test_i32_ne
	.size	.Ltest_i32_ne$local, .Lfunc_end192-test_i32_ne
                                        ! -- End function
	.globl	test_i32_lt                     ! -- Begin function test_i32_lt
	.p2align	4
	.type	test_i32_lt,@function
test_i32_lt:                            ! @test_i32_lt
.Ltest_i32_lt$local:
	.type	.Ltest_i32_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwlt	%r1, %r1, %r2
	retf	0

.Lfunc_end193:
	.size	test_i32_lt, .Lfunc_end193-test_i32_lt
	.size	.Ltest_i32_lt$local, .Lfunc_end193-test_i32_lt
                                        ! -- End function
	.globl	test_i32_le                     ! -- Begin function test_i32_le
	.p2align	4
	.type	test_i32_le,@function
test_i32_le:                            ! @test_i32_le
.Ltest_i32_le$local:
	.type	.Ltest_i32_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwle	%r1, %r1, %r2
	retf	0

.Lfunc_end194:
	.size	test_i32_le, .Lfunc_end194-test_i32_le
	.size	.Ltest_i32_le$local, .Lfunc_end194-test_i32_le
                                        ! -- End function
	.globl	test_i32_gt                     ! -- Begin function test_i32_gt
	.p2align	4
	.type	test_i32_gt,@function
test_i32_gt:                            ! @test_i32_gt
.Ltest_i32_gt$local:
	.type	.Ltest_i32_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwlt	%r1, %r2, %r1
	retf	0

.Lfunc_end195:
	.size	test_i32_gt, .Lfunc_end195-test_i32_gt
	.size	.Ltest_i32_gt$local, .Lfunc_end195-test_i32_gt
                                        ! -- End function
	.globl	test_i32_ge                     ! -- Begin function test_i32_ge
	.p2align	4
	.type	test_i32_ge,@function
test_i32_ge:                            ! @test_i32_ge
.Ltest_i32_ge$local:
	.type	.Ltest_i32_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwle	%r1, %r2, %r1
	retf	0

.Lfunc_end196:
	.size	test_i32_ge, .Lfunc_end196-test_i32_ge
	.size	.Ltest_i32_ge$local, .Lfunc_end196-test_i32_ge
                                        ! -- End function
	.globl	test_u32_lt                     ! -- Begin function test_u32_lt
	.p2align	4
	.type	test_u32_lt,@function
test_u32_lt:                            ! @test_u32_lt
.Ltest_u32_lt$local:
	.type	.Ltest_u32_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwltu	%r1, %r1, %r2
	retf	0

.Lfunc_end197:
	.size	test_u32_lt, .Lfunc_end197-test_u32_lt
	.size	.Ltest_u32_lt$local, .Lfunc_end197-test_u32_lt
                                        ! -- End function
	.globl	test_u32_le                     ! -- Begin function test_u32_le
	.p2align	4
	.type	test_u32_le,@function
test_u32_le:                            ! @test_u32_le
.Ltest_u32_le$local:
	.type	.Ltest_u32_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwleu	%r1, %r1, %r2
	retf	0

.Lfunc_end198:
	.size	test_u32_le, .Lfunc_end198-test_u32_le
	.size	.Ltest_u32_le$local, .Lfunc_end198-test_u32_le
                                        ! -- End function
	.globl	test_u32_gt                     ! -- Begin function test_u32_gt
	.p2align	4
	.type	test_u32_gt,@function
test_u32_gt:                            ! @test_u32_gt
.Ltest_u32_gt$local:
	.type	.Ltest_u32_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwltu	%r1, %r2, %r1
	retf	0

.Lfunc_end199:
	.size	test_u32_gt, .Lfunc_end199-test_u32_gt
	.size	.Ltest_u32_gt$local, .Lfunc_end199-test_u32_gt
                                        ! -- End function
	.globl	test_u32_ge                     ! -- Begin function test_u32_ge
	.p2align	4
	.type	test_u32_ge,@function
test_u32_ge:                            ! @test_u32_ge
.Ltest_u32_ge$local:
	.type	.Ltest_u32_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	cmpwleu	%r1, %r2, %r1
	retf	0

.Lfunc_end200:
	.size	test_u32_ge, .Lfunc_end200-test_u32_ge
	.size	.Ltest_u32_ge$local, .Lfunc_end200-test_u32_ge
                                        ! -- End function
	.globl	test_i32_eq_imm                 ! -- Begin function test_i32_eq_imm
	.p2align	4
	.type	test_i32_eq_imm,@function
test_i32_eq_imm:                        ! @test_i32_eq_imm
.Ltest_i32_eq_imm$local:
	.type	.Ltest_i32_eq_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpweqi	%r1, %r1, 1234
	retf	0

.Lfunc_end201:
	.size	test_i32_eq_imm, .Lfunc_end201-test_i32_eq_imm
	.size	.Ltest_i32_eq_imm$local, .Lfunc_end201-test_i32_eq_imm
                                        ! -- End function
	.globl	test_i32_ne_imm                 ! -- Begin function test_i32_ne_imm
	.p2align	4
	.type	test_i32_ne_imm,@function
test_i32_ne_imm:                        ! @test_i32_ne_imm
.Ltest_i32_ne_imm$local:
	.type	.Ltest_i32_ne_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwnei	%r1, %r1, 1234
	retf	0

.Lfunc_end202:
	.size	test_i32_ne_imm, .Lfunc_end202-test_i32_ne_imm
	.size	.Ltest_i32_ne_imm$local, .Lfunc_end202-test_i32_ne_imm
                                        ! -- End function
	.globl	test_i32_lt_imm                 ! -- Begin function test_i32_lt_imm
	.p2align	4
	.type	test_i32_lt_imm,@function
test_i32_lt_imm:                        ! @test_i32_lt_imm
.Ltest_i32_lt_imm$local:
	.type	.Ltest_i32_lt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwlti	%r1, %r1, 1234
	retf	0

.Lfunc_end203:
	.size	test_i32_lt_imm, .Lfunc_end203-test_i32_lt_imm
	.size	.Ltest_i32_lt_imm$local, .Lfunc_end203-test_i32_lt_imm
                                        ! -- End function
	.globl	test_i32_gt_imm                 ! -- Begin function test_i32_gt_imm
	.p2align	4
	.type	test_i32_gt_imm,@function
test_i32_gt_imm:                        ! @test_i32_gt_imm
.Ltest_i32_gt_imm$local:
	.type	.Ltest_i32_gt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwgti	%r1, %r1, 1234
	retf	0

.Lfunc_end204:
	.size	test_i32_gt_imm, .Lfunc_end204-test_i32_gt_imm
	.size	.Ltest_i32_gt_imm$local, .Lfunc_end204-test_i32_gt_imm
                                        ! -- End function
	.globl	test_i32_le_imm                 ! -- Begin function test_i32_le_imm
	.p2align	4
	.type	test_i32_le_imm,@function
test_i32_le_imm:                        ! @test_i32_le_imm
.Ltest_i32_le_imm$local:
	.type	.Ltest_i32_le_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwlti	%r1, %r1, 1235
	retf	0

.Lfunc_end205:
	.size	test_i32_le_imm, .Lfunc_end205-test_i32_le_imm
	.size	.Ltest_i32_le_imm$local, .Lfunc_end205-test_i32_le_imm
                                        ! -- End function
	.globl	test_i32_ge_imm                 ! -- Begin function test_i32_ge_imm
	.p2align	4
	.type	test_i32_ge_imm,@function
test_i32_ge_imm:                        ! @test_i32_ge_imm
.Ltest_i32_ge_imm$local:
	.type	.Ltest_i32_ge_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwgti	%r1, %r1, 1233
	retf	0

.Lfunc_end206:
	.size	test_i32_ge_imm, .Lfunc_end206-test_i32_ge_imm
	.size	.Ltest_i32_ge_imm$local, .Lfunc_end206-test_i32_ge_imm
                                        ! -- End function
	.globl	test_u32_lt_imm                 ! -- Begin function test_u32_lt_imm
	.p2align	4
	.type	test_u32_lt_imm,@function
test_u32_lt_imm:                        ! @test_u32_lt_imm
.Ltest_u32_lt_imm$local:
	.type	.Ltest_u32_lt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwltui	%r1, %r1, 1234
	retf	0

.Lfunc_end207:
	.size	test_u32_lt_imm, .Lfunc_end207-test_u32_lt_imm
	.size	.Ltest_u32_lt_imm$local, .Lfunc_end207-test_u32_lt_imm
                                        ! -- End function
	.globl	test_u32_gt_imm                 ! -- Begin function test_u32_gt_imm
	.p2align	4
	.type	test_u32_gt_imm,@function
test_u32_gt_imm:                        ! @test_u32_gt_imm
.Ltest_u32_gt_imm$local:
	.type	.Ltest_u32_gt_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwgtui	%r1, %r1, 1234
	retf	0

.Lfunc_end208:
	.size	test_u32_gt_imm, .Lfunc_end208-test_u32_gt_imm
	.size	.Ltest_u32_gt_imm$local, .Lfunc_end208-test_u32_gt_imm
                                        ! -- End function
	.globl	test_u32_le_imm                 ! -- Begin function test_u32_le_imm
	.p2align	4
	.type	test_u32_le_imm,@function
test_u32_le_imm:                        ! @test_u32_le_imm
.Ltest_u32_le_imm$local:
	.type	.Ltest_u32_le_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwltui	%r1, %r1, 1235
	retf	0

.Lfunc_end209:
	.size	test_u32_le_imm, .Lfunc_end209-test_u32_le_imm
	.size	.Ltest_u32_le_imm$local, .Lfunc_end209-test_u32_le_imm
                                        ! -- End function
	.globl	test_u32_ge_imm                 ! -- Begin function test_u32_ge_imm
	.p2align	4
	.type	test_u32_ge_imm,@function
test_u32_ge_imm:                        ! @test_u32_ge_imm
.Ltest_u32_ge_imm$local:
	.type	.Ltest_u32_ge_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	cmpwgtui	%r1, %r1, 1233
	retf	0

.Lfunc_end210:
	.size	test_u32_ge_imm, .Lfunc_end210-test_u32_ge_imm
	.size	.Ltest_u32_ge_imm$local, .Lfunc_end210-test_u32_ge_imm
                                        ! -- End function
	.globl	test_cpuid                      ! -- Begin function test_cpuid
	.p2align	4
	.type	test_cpuid,@function
test_cpuid:                             ! @test_cpuid
.Ltest_cpuid$local:
	.type	.Ltest_cpuid$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	cpuid	%r2, %r1, 33
	cpuid	%r3, %r1, 0

	! BUNDLE
	addi	%r1, %r1, 10000
	cpuid	%r1, %r1, 0
	addadd	%r1, %r3, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end211:
	.size	test_cpuid, .Lfunc_end211-test_cpuid
	.size	.Ltest_cpuid$local, .Lfunc_end211-test_cpuid
                                        ! -- End function
	.globl	test_crc32c                     ! -- Begin function test_crc32c
	.p2align	4
	.type	test_crc32c,@function
test_crc32c:                            ! @test_crc32c
.Ltest_crc32c$local:
	.type	.Ltest_crc32c$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	crc32c	%r1, %r1, %r2, %r3
	retf	0

.Lfunc_end212:
	.size	test_crc32c, .Lfunc_end212-test_crc32c
	.size	.Ltest_crc32c$local, .Lfunc_end212-test_crc32c
                                        ! -- End function
	.globl	TestDebugRegisters              ! -- Begin function TestDebugRegisters
	.p2align	4
	.type	TestDebugRegisters,@function
TestDebugRegisters:                     ! @TestDebugRegisters
.LTestDebugRegisters$local:
	.type	.LTestDebugRegisters$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mfmr	%r2, %r1, 1
	mtmr	%r2, %r1, 1

	! BUNDLE
	mfibr	%r2, %r1, 2
	mtibr	%r2, %r1, 1
	mfdbr	%r2, %r1, 1

	! BUNDLE
	mtdbr	%r2, %r1, 2
	retf	0
	nop	0

.Lfunc_end213:
	.size	TestDebugRegisters, .Lfunc_end213-TestDebugRegisters
	.size	.LTestDebugRegisters$local, .Lfunc_end213-TestDebugRegisters
                                        ! -- End function
	.globl	test_fma_f16                    ! -- Begin function test_fma_f16
	.p2align	4
	.type	test_fma_f16,@function
test_fma_f16:                           ! @test_fma_f16
.Ltest_fma_f16$local:
	.type	.Ltest_fma_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	fmaddsh	%r3, %r1, %r2, %r3
	fnmaddsh	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsubsh	%r2, %r1, %r2, %r3
	fnmsubsh	%r1, %r1, %r2, %r2
	retf	0

.Lfunc_end214:
	.size	test_fma_f16, .Lfunc_end214-test_fma_f16
	.size	.Ltest_fma_f16$local, .Lfunc_end214-test_fma_f16
                                        ! -- End function
	.globl	test_fma_f32                    ! -- Begin function test_fma_f32
	.p2align	4
	.type	test_fma_f32,@function
test_fma_f32:                           ! @test_fma_f32
.Ltest_fma_f32$local:
	.type	.Ltest_fma_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	fmaddss	%r3, %r1, %r2, %r3
	fnmaddss	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsubss	%r2, %r1, %r2, %r3
	fnmsubss	%r1, %r1, %r2, %r2
	retf	0

.Lfunc_end215:
	.size	test_fma_f32, .Lfunc_end215-test_fma_f32
	.size	.Ltest_fma_f32$local, .Lfunc_end215-test_fma_f32
                                        ! -- End function
	.globl	test_fma_f64                    ! -- Begin function test_fma_f64
	.p2align	4
	.type	test_fma_f64,@function
test_fma_f64:                           ! @test_fma_f64
.Ltest_fma_f64$local:
	.type	.Ltest_fma_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	fmaddsd	%r3, %r1, %r2, %r3
	fnmaddsd	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsubsd	%r2, %r1, %r2, %r3
	fnmsubsd	%r1, %r1, %r2, %r2
	retf	0

.Lfunc_end216:
	.size	test_fma_f64, .Lfunc_end216-test_fma_f64
	.size	.Ltest_fma_f64$local, .Lfunc_end216-test_fma_f64
                                        ! -- End function
	.globl	test_fma_f128                   ! -- Begin function test_fma_f128
	.p2align	4
	.type	test_fma_f128,@function
test_fma_f128:                          ! @test_fma_f128
.Ltest_fma_f128$local:
	.type	.Ltest_fma_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	fmaddsq	%r3, %r1, %r2, %r3
	fnmaddsq	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsubsq	%r2, %r1, %r2, %r3
	fnmsubsq	%r1, %r1, %r2, %r2
	retf	0

.Lfunc_end217:
	.size	test_fma_f128, .Lfunc_end217-test_fma_f128
	.size	.Ltest_fma_f128$local, .Lfunc_end217-test_fma_f128
                                        ! -- End function
	.globl	test_fma_v8f16                  ! -- Begin function test_fma_v8f16
	.p2align	4
	.type	test_fma_v8f16,@function
test_fma_v8f16:                         ! @test_fma_v8f16
.Ltest_fma_v8f16$local:
	.type	.Ltest_fma_v8f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	fmaddph	%r3, %r1, %r2, %r3
	fnmaddph	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsubph	%r2, %r1, %r2, %r3
	fnmsubph	%r1, %r1, %r2, %r2
	retf	0

.Lfunc_end218:
	.size	test_fma_v8f16, .Lfunc_end218-test_fma_v8f16
	.size	.Ltest_fma_v8f16$local, .Lfunc_end218-test_fma_v8f16
                                        ! -- End function
	.globl	test_fma_v4f32                  ! -- Begin function test_fma_v4f32
	.p2align	4
	.type	test_fma_v4f32,@function
test_fma_v4f32:                         ! @test_fma_v4f32
.Ltest_fma_v4f32$local:
	.type	.Ltest_fma_v4f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	fmaddps	%r3, %r1, %r2, %r3
	fnmaddps	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsubps	%r2, %r1, %r2, %r3
	fnmsubps	%r1, %r1, %r2, %r2
	retf	0

.Lfunc_end219:
	.size	test_fma_v4f32, .Lfunc_end219-test_fma_v4f32
	.size	.Ltest_fma_v4f32$local, .Lfunc_end219-test_fma_v4f32
                                        ! -- End function
	.globl	test_fma_v2f64                  ! -- Begin function test_fma_v2f64
	.p2align	4
	.type	test_fma_v2f64,@function
test_fma_v2f64:                         ! @test_fma_v2f64
.Ltest_fma_v2f64$local:
	.type	.Ltest_fma_v2f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	fmaddpd	%r3, %r1, %r2, %r3
	fnmaddpd	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsubpd	%r2, %r1, %r2, %r3
	fnmsubpd	%r1, %r1, %r2, %r2
	retf	0

.Lfunc_end220:
	.size	test_fma_v2f64, .Lfunc_end220-test_fma_v2f64
	.size	.Ltest_fma_v2f64$local, .Lfunc_end220-test_fma_v2f64
                                        ! -- End function
	.globl	ffff                            ! -- Begin function ffff
	.p2align	4
	.type	ffff,@function
ffff:                                   ! @ffff
.Lffff$local:
	.type	.Lffff$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	lddz	%r1, %sp, 0
	ldwz	%r2, %r1, 0

	! BUNDLE
	ldwz	%r1, %r1, 4
	faddss	%r1, %r1, %r2
	retf	0

.Lfunc_end221:
	.size	ffff, .Lfunc_end221-ffff
	.size	.Lffff$local, .Lfunc_end221-ffff
                                        ! -- End function
	.globl	i64_test                        ! -- Begin function i64_test
	.p2align	4
	.type	i64_test,@function
i64_test:                               ! @i64_test
.Li64_test$local:
	.type	.Li64_test$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	11, 96
	add	%r3, %r2, %r1
	std	%r3, %sp, 88

	! BUNDLE
	sub	%r3, %r1, %r2
	std	%r3, %sp, 80
	subfi	%r3, %r2, 1000

	! BUNDLE
	std	%r3, %sp, 72
	subfi.l	%r3, %r2, 1000000000

	! BUNDLE
	std	%r3, %sp, 64
	addi	%r3, %r1, -100
	std	%r3, %sp, 56

	! BUNDLE
	std	%r3, %sp, 48
	lddz	%r3, %sp, 88
	lddz	%r4, %sp, 80

	! BUNDLE
	lddz	%r5, %sp, 72
	lddz	%r6, %sp, 64
	lddz	%r7, %sp, 56

	! BUNDLE
	lddz	%r8, %sp, 48
	addadd	%r4, %r4, %r5, %r7
	addsub	%r3, %r3, %r6, %r4

	! BUNDLE
	add	%r3, %r3, %r8
	std	%r3, %sp, 40
	bdle	%r2, %r1, .LBB222_3

	! BUNDLE
	lddz	%r2, %sp, 88
	bdle	%r2, %r1, .LBB222_3
	lddz	%r1, %sp, 40

	! BUNDLE
	addi	%r1, %r1, -1
	ldafr	%r10, %pcfwd(.L.str.15)
	std	%r1, %sp, 0

	! BUNDLE
	callr	%r9, %jpc(printf)
	lddz	%r1, %sp, 88
	lddz	%r2, %sp, 80

	! BUNDLE
	lddz	%r3, %sp, 64
	addsub	%r1, %r2, %r1, %r3
	std	%r1, %sp, 32

	! BUNDLE
	lddz	%r1, %sp, 88
	lddz	%r2, %sp, 80
	lddz	%r3, %sp, 64

	! BUNDLE
	subsub	%r1, %r1, %r2, %r3
	std	%r1, %sp, 24
	lddz	%r1, %sp, 40

	! BUNDLE
	retf	96
	nop	0
	nop	0

.LBB222_3:                              ! %if.else
                                        ! Label of block must be emitted
	! BUNDLE
	lddz	%r1, %sp, 40
	addi	%r1, %r1, 1
	ldafr	%r10, %pcfwd(.L.str.16)

	! BUNDLE
	std	%r1, %sp, 0
	callr	%r9, %jpc(printf)
	mov	%r10, %gz

	! BUNDLE
	callr	%r9, %jpc(exit)
	nop	0
	nop	0

.Lfunc_end222:
	.size	i64_test, .Lfunc_end222-i64_test
	.size	.Li64_test$local, .Lfunc_end222-i64_test
                                        ! -- End function
	.globl	NeverReturn                     ! -- Begin function NeverReturn
	.p2align	4
	.type	NeverReturn,@function
NeverReturn:                            ! @NeverReturn
.LNeverReturn$local:
	.type	.LNeverReturn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	7, 16
	bweq	%r1, %gz, .LBB223_3
	ldafr	%r3, %pcfwd(.L.str.17)

.LBB223_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%r1, %sp, 0
	mov	%r6, %r3
	callr	%r5, %jpc(printf)

	! BUNDLE
	lddzr	%r4, %pcfwd(uglbx)
	addi	%r4, %r4, 1
	addiws	%r1, %r1, -1

	! BUNDLE
	stdr	%r4, %pcfwd(uglbx)
	bwne	%r1, %gz, .LBB223_2
	nop	0

.LBB223_3:                              ! %for.cond1.preheader
                                        ! Label of block must be emitted
	! BUNDLE
	ldafr	%r1, %pcfwd(.L.str.18)
	nop	0
	nop	0

.LBB223_4:                              ! %for.cond1
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%r2, %sp, 0
	mov	%r6, %r1
	callr	%r5, %jpc(printf)

	! BUNDLE
	ldwzr	%r3, %pcfwd(glbx)
	addiws	%r2, %r2, -1
	addiws	%r3, %r3, -1

	! BUNDLE
	stwr	%r3, %pcfwd(glbx)
	jmp	.LBB223_4
	nop	0

.Lfunc_end223:
	.size	NeverReturn, .Lfunc_end223-NeverReturn
	.size	.LNeverReturn$local, .Lfunc_end223-NeverReturn
                                        ! -- End function
	.globl	i64_add                         ! -- Begin function i64_add
	.p2align	4
	.type	i64_add,@function
i64_add:                                ! @i64_add
.Li64_add$local:
	.type	.Li64_add$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldwsr	%r2, %pcfwd(glbx)
	nop	2

	! BUNDLE
	ldax.l	%r1, %r1, %r2, 0, 1234
	stwr	%r1, %pcfwd(glbx)

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end224:
	.size	i64_add, .Lfunc_end224-i64_add
	.size	.Li64_add$local, .Lfunc_end224-i64_add
                                        ! -- End function
	.globl	i32_add                         ! -- Begin function i32_add
	.p2align	4
	.type	i32_add,@function
i32_add:                                ! @i32_add
.Li32_add$local:
	.type	.Li32_add$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	6
	ldwzr	%r3, %pcfwd(glb)
	mov	%r5, %r1

	! BUNDLE
	callr	%r4, %jpc(cccc)
	ldar	%r2, %jpc(cccc)
	addw	%r1, %r2, %r1

	! BUNDLE
	addw	%r1, %r1, %r3
	addw	%r1, %r1, %r5
	addiws	%r1, %r1, 12345

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end225:
	.size	i32_add, .Lfunc_end225-i32_add
	.size	.Li32_add$local, .Lfunc_end225-i32_add
                                        ! -- End function
	.globl	u32_add                         ! -- Begin function u32_add
	.p2align	4
	.type	u32_add,@function
u32_add:                                ! @u32_add
.Lu32_add$local:
	.type	.Lu32_add$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldwzr	%r2, %pcfwd(uglb)
	addw	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end226:
	.size	u32_add, .Lfunc_end226-u32_add
	.size	.Lu32_add$local, .Lfunc_end226-u32_add
                                        ! -- End function
	.globl	float_test                      ! -- Begin function float_test
	.p2align	4
	.type	float_test,@function
float_test:                             ! @float_test
.Lfloat_test$local:
	.type	.Lfloat_test$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	bfsdole	%r2, %r1, .LBB227_1
	ldafr	%r4, %pcfwd(.L.str.19)

	! BUNDLE
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	mov	%r1, %r4

	! BUNDLE
	retf	16
	nop	0
	nop	0

.LBB227_1:                              ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	retf	16
	nop	0

.Lfunc_end227:
	.size	float_test, .Lfunc_end227-float_test
	.size	.Lfloat_test$local, .Lfunc_end227-float_test
                                        ! -- End function
	.globl	main                            ! -- Begin function main
	.p2align	4
	.type	main,@function
main:                                   ! @main
.Lmain$local:
	.type	.Lmain$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mov	%r1, %gz
	retf	0

.Lfunc_end228:
	.size	main, .Lfunc_end228-main
	.size	.Lmain$local, .Lfunc_end228-main
                                        ! -- End function
	.globl	test_fp_arithmetic_f16          ! -- Begin function test_fp_arithmetic_f16
	.p2align	4
	.type	test_fp_arithmetic_f16,@function
test_fp_arithmetic_f16:                 ! @test_fp_arithmetic_f16
.Ltest_fp_arithmetic_f16$local:
	.type	.Ltest_fp_arithmetic_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	faddsh	%r3, %r2, %r1
	fsubsh	%r1, %r1, %r2

	! BUNDLE
	fmulsh	%r1, %r3, %r1
	fdivsh	%r1, %r1, %r2
	retf	0

.Lfunc_end229:
	.size	test_fp_arithmetic_f16, .Lfunc_end229-test_fp_arithmetic_f16
	.size	.Ltest_fp_arithmetic_f16$local, .Lfunc_end229-test_fp_arithmetic_f16
                                        ! -- End function
	.globl	test_fp_arithmetic_f32          ! -- Begin function test_fp_arithmetic_f32
	.p2align	4
	.type	test_fp_arithmetic_f32,@function
test_fp_arithmetic_f32:                 ! @test_fp_arithmetic_f32
.Ltest_fp_arithmetic_f32$local:
	.type	.Ltest_fp_arithmetic_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	faddss	%r3, %r2, %r1
	fsubss	%r1, %r1, %r2

	! BUNDLE
	fmulss	%r1, %r3, %r1
	fdivss	%r1, %r1, %r2
	retf	0

.Lfunc_end230:
	.size	test_fp_arithmetic_f32, .Lfunc_end230-test_fp_arithmetic_f32
	.size	.Ltest_fp_arithmetic_f32$local, .Lfunc_end230-test_fp_arithmetic_f32
                                        ! -- End function
	.globl	test_fp_arithmetic_f64          ! -- Begin function test_fp_arithmetic_f64
	.p2align	4
	.type	test_fp_arithmetic_f64,@function
test_fp_arithmetic_f64:                 ! @test_fp_arithmetic_f64
.Ltest_fp_arithmetic_f64$local:
	.type	.Ltest_fp_arithmetic_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	faddsd	%r3, %r2, %r1
	fsubsd	%r1, %r1, %r2

	! BUNDLE
	fmulsd	%r1, %r3, %r1
	fdivsd	%r1, %r1, %r2
	retf	0

.Lfunc_end231:
	.size	test_fp_arithmetic_f64, .Lfunc_end231-test_fp_arithmetic_f64
	.size	.Ltest_fp_arithmetic_f64$local, .Lfunc_end231-test_fp_arithmetic_f64
                                        ! -- End function
	.globl	test_fp_arithmetic_f128         ! -- Begin function test_fp_arithmetic_f128
	.p2align	4
	.type	test_fp_arithmetic_f128,@function
test_fp_arithmetic_f128:                ! @test_fp_arithmetic_f128
.Ltest_fp_arithmetic_f128$local:
	.type	.Ltest_fp_arithmetic_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	faddsq	%r3, %r2, %r1
	fsubsq	%r1, %r1, %r2

	! BUNDLE
	fmulsq	%r1, %r3, %r1
	fdivsq	%r1, %r1, %r2
	retf	0

.Lfunc_end232:
	.size	test_fp_arithmetic_f128, .Lfunc_end232-test_fp_arithmetic_f128
	.size	.Ltest_fp_arithmetic_f128$local, .Lfunc_end232-test_fp_arithmetic_f128
                                        ! -- End function
	.globl	test_fp_bitcast_i16_to_f16      ! -- Begin function test_fp_bitcast_i16_to_f16
	.p2align	4
	.type	test_fp_bitcast_i16_to_f16,@function
test_fp_bitcast_i16_to_f16:             ! @test_fp_bitcast_i16_to_f16
.Ltest_fp_bitcast_i16_to_f16$local:
	.type	.Ltest_fp_bitcast_i16_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	2, 16
	sth	%r1, %sp, 14
	ldhz	%r1, %sp, 14

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end233:
	.size	test_fp_bitcast_i16_to_f16, .Lfunc_end233-test_fp_bitcast_i16_to_f16
	.size	.Ltest_fp_bitcast_i16_to_f16$local, .Lfunc_end233-test_fp_bitcast_i16_to_f16
                                        ! -- End function
	.globl	test_fp_bitcast_f16_to_i16      ! -- Begin function test_fp_bitcast_f16_to_i16
	.p2align	4
	.type	test_fp_bitcast_f16_to_i16,@function
test_fp_bitcast_f16_to_i16:             ! @test_fp_bitcast_f16_to_i16
.Ltest_fp_bitcast_f16_to_i16$local:
	.type	.Ltest_fp_bitcast_f16_to_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	2, 16
	sth	%r1, %sp, 14
	ldhs	%r1, %sp, 14

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end234:
	.size	test_fp_bitcast_f16_to_i16, .Lfunc_end234-test_fp_bitcast_f16_to_i16
	.size	.Ltest_fp_bitcast_f16_to_i16$local, .Lfunc_end234-test_fp_bitcast_f16_to_i16
                                        ! -- End function
	.globl	test_fp_bitcast_i32_to_f32      ! -- Begin function test_fp_bitcast_i32_to_f32
	.p2align	4
	.type	test_fp_bitcast_i32_to_f32,@function
test_fp_bitcast_i32_to_f32:             ! @test_fp_bitcast_i32_to_f32
.Ltest_fp_bitcast_i32_to_f32$local:
	.type	.Ltest_fp_bitcast_i32_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end235:
	.size	test_fp_bitcast_i32_to_f32, .Lfunc_end235-test_fp_bitcast_i32_to_f32
	.size	.Ltest_fp_bitcast_i32_to_f32$local, .Lfunc_end235-test_fp_bitcast_i32_to_f32
                                        ! -- End function
	.globl	test_fp_bitcast_f32_to_i32      ! -- Begin function test_fp_bitcast_f32_to_i32
	.p2align	4
	.type	test_fp_bitcast_f32_to_i32,@function
test_fp_bitcast_f32_to_i32:             ! @test_fp_bitcast_f32_to_i32
.Ltest_fp_bitcast_f32_to_i32$local:
	.type	.Ltest_fp_bitcast_f32_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end236:
	.size	test_fp_bitcast_f32_to_i32, .Lfunc_end236-test_fp_bitcast_f32_to_i32
	.size	.Ltest_fp_bitcast_f32_to_i32$local, .Lfunc_end236-test_fp_bitcast_f32_to_i32
                                        ! -- End function
	.globl	test_fp_bitcast_i64_to_f64      ! -- Begin function test_fp_bitcast_i64_to_f64
	.p2align	4
	.type	test_fp_bitcast_i64_to_f64,@function
test_fp_bitcast_i64_to_f64:             ! @test_fp_bitcast_i64_to_f64
.Ltest_fp_bitcast_i64_to_f64$local:
	.type	.Ltest_fp_bitcast_i64_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end237:
	.size	test_fp_bitcast_i64_to_f64, .Lfunc_end237-test_fp_bitcast_i64_to_f64
	.size	.Ltest_fp_bitcast_i64_to_f64$local, .Lfunc_end237-test_fp_bitcast_i64_to_f64
                                        ! -- End function
	.globl	test_fp_bitcast_f64_to_i64      ! -- Begin function test_fp_bitcast_f64_to_i64
	.p2align	4
	.type	test_fp_bitcast_f64_to_i64,@function
test_fp_bitcast_f64_to_i64:             ! @test_fp_bitcast_f64_to_i64
.Ltest_fp_bitcast_f64_to_i64$local:
	.type	.Ltest_fp_bitcast_f64_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end238:
	.size	test_fp_bitcast_f64_to_i64, .Lfunc_end238-test_fp_bitcast_f64_to_i64
	.size	.Ltest_fp_bitcast_f64_to_i64$local, .Lfunc_end238-test_fp_bitcast_f64_to_i64
                                        ! -- End function
	.globl	test_fp_bitcast_i128_to_f128    ! -- Begin function test_fp_bitcast_i128_to_f128
	.p2align	4
	.type	test_fp_bitcast_i128_to_f128,@function
test_fp_bitcast_i128_to_f128:           ! @test_fp_bitcast_i128_to_f128
.Ltest_fp_bitcast_i128_to_f128$local:
	.type	.Ltest_fp_bitcast_i128_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end239:
	.size	test_fp_bitcast_i128_to_f128, .Lfunc_end239-test_fp_bitcast_i128_to_f128
	.size	.Ltest_fp_bitcast_i128_to_f128$local, .Lfunc_end239-test_fp_bitcast_i128_to_f128
                                        ! -- End function
	.globl	test_fp_bitcast_f128_to_i128    ! -- Begin function test_fp_bitcast_f128_to_i128
	.p2align	4
	.type	test_fp_bitcast_f128_to_i128,@function
test_fp_bitcast_f128_to_i128:           ! @test_fp_bitcast_f128_to_i128
.Ltest_fp_bitcast_f128_to_i128$local:
	.type	.Ltest_fp_bitcast_f128_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end240:
	.size	test_fp_bitcast_f128_to_i128, .Lfunc_end240-test_fp_bitcast_f128_to_i128
	.size	.Ltest_fp_bitcast_f128_to_i128$local, .Lfunc_end240-test_fp_bitcast_f128_to_i128
                                        ! -- End function
	.globl	test_fp_branch_f32_eq           ! -- Begin function test_fp_branch_f32_eq
	.p2align	4
	.type	test_fp_branch_f32_eq,@function
test_fp_branch_f32_eq:                  ! @test_fp_branch_f32_eq
.Ltest_fp_branch_f32_eq$local:
	.type	.Ltest_fp_branch_f32_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfssoeq	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.99)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end241:
	.size	test_fp_branch_f32_eq, .Lfunc_end241-test_fp_branch_f32_eq
	.size	.Ltest_fp_branch_f32_eq$local, .Lfunc_end241-test_fp_branch_f32_eq
                                        ! -- End function
	.globl	test_fp_branch_f32_ne           ! -- Begin function test_fp_branch_f32_ne
	.p2align	4
	.type	test_fp_branch_f32_ne,@function
test_fp_branch_f32_ne:                  ! @test_fp_branch_f32_ne
.Ltest_fp_branch_f32_ne$local:
	.type	.Ltest_fp_branch_f32_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfssone	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.100)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end242:
	.size	test_fp_branch_f32_ne, .Lfunc_end242-test_fp_branch_f32_ne
	.size	.Ltest_fp_branch_f32_ne$local, .Lfunc_end242-test_fp_branch_f32_ne
                                        ! -- End function
	.globl	test_fp_branch_f32_lt           ! -- Begin function test_fp_branch_f32_lt
	.p2align	4
	.type	test_fp_branch_f32_lt,@function
test_fp_branch_f32_lt:                  ! @test_fp_branch_f32_lt
.Ltest_fp_branch_f32_lt$local:
	.type	.Ltest_fp_branch_f32_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfssolt	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.101)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end243:
	.size	test_fp_branch_f32_lt, .Lfunc_end243-test_fp_branch_f32_lt
	.size	.Ltest_fp_branch_f32_lt$local, .Lfunc_end243-test_fp_branch_f32_lt
                                        ! -- End function
	.globl	test_fp_branch_f32_le           ! -- Begin function test_fp_branch_f32_le
	.p2align	4
	.type	test_fp_branch_f32_le,@function
test_fp_branch_f32_le:                  ! @test_fp_branch_f32_le
.Ltest_fp_branch_f32_le$local:
	.type	.Ltest_fp_branch_f32_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfssole	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.102)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end244:
	.size	test_fp_branch_f32_le, .Lfunc_end244-test_fp_branch_f32_le
	.size	.Ltest_fp_branch_f32_le$local, .Lfunc_end244-test_fp_branch_f32_le
                                        ! -- End function
	.globl	test_fp_branch_f32_gt           ! -- Begin function test_fp_branch_f32_gt
	.p2align	4
	.type	test_fp_branch_f32_gt,@function
test_fp_branch_f32_gt:                  ! @test_fp_branch_f32_gt
.Ltest_fp_branch_f32_gt$local:
	.type	.Ltest_fp_branch_f32_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfssolt	%r2, %r1, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.103)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end245:
	.size	test_fp_branch_f32_gt, .Lfunc_end245-test_fp_branch_f32_gt
	.size	.Ltest_fp_branch_f32_gt$local, .Lfunc_end245-test_fp_branch_f32_gt
                                        ! -- End function
	.globl	test_fp_branch_f32_ge           ! -- Begin function test_fp_branch_f32_ge
	.p2align	4
	.type	test_fp_branch_f32_ge,@function
test_fp_branch_f32_ge:                  ! @test_fp_branch_f32_ge
.Ltest_fp_branch_f32_ge$local:
	.type	.Ltest_fp_branch_f32_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfssole	%r2, %r1, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.104)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end246:
	.size	test_fp_branch_f32_ge, .Lfunc_end246-test_fp_branch_f32_ge
	.size	.Ltest_fp_branch_f32_ge$local, .Lfunc_end246-test_fp_branch_f32_ge
                                        ! -- End function
	.globl	test_fp_branch_f64_eq           ! -- Begin function test_fp_branch_f64_eq
	.p2align	4
	.type	test_fp_branch_f64_eq,@function
test_fp_branch_f64_eq:                  ! @test_fp_branch_f64_eq
.Ltest_fp_branch_f64_eq$local:
	.type	.Ltest_fp_branch_f64_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsdoeq	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.105)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end247:
	.size	test_fp_branch_f64_eq, .Lfunc_end247-test_fp_branch_f64_eq
	.size	.Ltest_fp_branch_f64_eq$local, .Lfunc_end247-test_fp_branch_f64_eq
                                        ! -- End function
	.globl	test_fp_branch_f64_ne           ! -- Begin function test_fp_branch_f64_ne
	.p2align	4
	.type	test_fp_branch_f64_ne,@function
test_fp_branch_f64_ne:                  ! @test_fp_branch_f64_ne
.Ltest_fp_branch_f64_ne$local:
	.type	.Ltest_fp_branch_f64_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsdone	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.106)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end248:
	.size	test_fp_branch_f64_ne, .Lfunc_end248-test_fp_branch_f64_ne
	.size	.Ltest_fp_branch_f64_ne$local, .Lfunc_end248-test_fp_branch_f64_ne
                                        ! -- End function
	.globl	test_fp_branch_f64_lt           ! -- Begin function test_fp_branch_f64_lt
	.p2align	4
	.type	test_fp_branch_f64_lt,@function
test_fp_branch_f64_lt:                  ! @test_fp_branch_f64_lt
.Ltest_fp_branch_f64_lt$local:
	.type	.Ltest_fp_branch_f64_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsdolt	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.107)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end249:
	.size	test_fp_branch_f64_lt, .Lfunc_end249-test_fp_branch_f64_lt
	.size	.Ltest_fp_branch_f64_lt$local, .Lfunc_end249-test_fp_branch_f64_lt
                                        ! -- End function
	.globl	test_fp_branch_f64_le           ! -- Begin function test_fp_branch_f64_le
	.p2align	4
	.type	test_fp_branch_f64_le,@function
test_fp_branch_f64_le:                  ! @test_fp_branch_f64_le
.Ltest_fp_branch_f64_le$local:
	.type	.Ltest_fp_branch_f64_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsdole	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.108)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end250:
	.size	test_fp_branch_f64_le, .Lfunc_end250-test_fp_branch_f64_le
	.size	.Ltest_fp_branch_f64_le$local, .Lfunc_end250-test_fp_branch_f64_le
                                        ! -- End function
	.globl	test_fp_branch_f64_gt           ! -- Begin function test_fp_branch_f64_gt
	.p2align	4
	.type	test_fp_branch_f64_gt,@function
test_fp_branch_f64_gt:                  ! @test_fp_branch_f64_gt
.Ltest_fp_branch_f64_gt$local:
	.type	.Ltest_fp_branch_f64_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsdolt	%r2, %r1, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.109)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end251:
	.size	test_fp_branch_f64_gt, .Lfunc_end251-test_fp_branch_f64_gt
	.size	.Ltest_fp_branch_f64_gt$local, .Lfunc_end251-test_fp_branch_f64_gt
                                        ! -- End function
	.globl	test_fp_branch_f64_ge           ! -- Begin function test_fp_branch_f64_ge
	.p2align	4
	.type	test_fp_branch_f64_ge,@function
test_fp_branch_f64_ge:                  ! @test_fp_branch_f64_ge
.Ltest_fp_branch_f64_ge$local:
	.type	.Ltest_fp_branch_f64_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsdole	%r2, %r1, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.110)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end252:
	.size	test_fp_branch_f64_ge, .Lfunc_end252-test_fp_branch_f64_ge
	.size	.Ltest_fp_branch_f64_ge$local, .Lfunc_end252-test_fp_branch_f64_ge
                                        ! -- End function
	.globl	test_fp_branch_f128_eq          ! -- Begin function test_fp_branch_f128_eq
	.p2align	4
	.type	test_fp_branch_f128_eq,@function
test_fp_branch_f128_eq:                 ! @test_fp_branch_f128_eq
.Ltest_fp_branch_f128_eq$local:
	.type	.Ltest_fp_branch_f128_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsqoeq	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.111)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end253:
	.size	test_fp_branch_f128_eq, .Lfunc_end253-test_fp_branch_f128_eq
	.size	.Ltest_fp_branch_f128_eq$local, .Lfunc_end253-test_fp_branch_f128_eq
                                        ! -- End function
	.globl	test_fp_branch_f128_ne          ! -- Begin function test_fp_branch_f128_ne
	.p2align	4
	.type	test_fp_branch_f128_ne,@function
test_fp_branch_f128_ne:                 ! @test_fp_branch_f128_ne
.Ltest_fp_branch_f128_ne$local:
	.type	.Ltest_fp_branch_f128_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsqone	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.112)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end254:
	.size	test_fp_branch_f128_ne, .Lfunc_end254-test_fp_branch_f128_ne
	.size	.Ltest_fp_branch_f128_ne$local, .Lfunc_end254-test_fp_branch_f128_ne
                                        ! -- End function
	.globl	test_fp_branch_f128_lt          ! -- Begin function test_fp_branch_f128_lt
	.p2align	4
	.type	test_fp_branch_f128_lt,@function
test_fp_branch_f128_lt:                 ! @test_fp_branch_f128_lt
.Ltest_fp_branch_f128_lt$local:
	.type	.Ltest_fp_branch_f128_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsqolt	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.113)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end255:
	.size	test_fp_branch_f128_lt, .Lfunc_end255-test_fp_branch_f128_lt
	.size	.Ltest_fp_branch_f128_lt$local, .Lfunc_end255-test_fp_branch_f128_lt
                                        ! -- End function
	.globl	test_fp_branch_f128_le          ! -- Begin function test_fp_branch_f128_le
	.p2align	4
	.type	test_fp_branch_f128_le,@function
test_fp_branch_f128_le:                 ! @test_fp_branch_f128_le
.Ltest_fp_branch_f128_le$local:
	.type	.Ltest_fp_branch_f128_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsqole	%r1, %r2, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.114)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end256:
	.size	test_fp_branch_f128_le, .Lfunc_end256-test_fp_branch_f128_le
	.size	.Ltest_fp_branch_f128_le$local, .Lfunc_end256-test_fp_branch_f128_le
                                        ! -- End function
	.globl	test_fp_branch_f128_gt          ! -- Begin function test_fp_branch_f128_gt
	.p2align	4
	.type	test_fp_branch_f128_gt,@function
test_fp_branch_f128_gt:                 ! @test_fp_branch_f128_gt
.Ltest_fp_branch_f128_gt$local:
	.type	.Ltest_fp_branch_f128_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsqolt	%r2, %r1, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.115)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end257:
	.size	test_fp_branch_f128_gt, .Lfunc_end257-test_fp_branch_f128_gt
	.size	.Ltest_fp_branch_f128_gt$local, .Lfunc_end257-test_fp_branch_f128_gt
                                        ! -- End function
	.globl	test_fp_branch_f128_ge          ! -- Begin function test_fp_branch_f128_ge
	.p2align	4
	.type	test_fp_branch_f128_ge,@function
test_fp_branch_f128_ge:                 ! @test_fp_branch_f128_ge
.Ltest_fp_branch_f128_ge$local:
	.type	.Ltest_fp_branch_f128_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	nulfsqole	%r2, %r1, 2, 0
	ldafr	%r4, %pcfwd(.Lstr.116)

	! BUNDLE
	callr	%r3, %jpc(puts)
	retf	0
	nop	0

.Lfunc_end258:
	.size	test_fp_branch_f128_ge, .Lfunc_end258-test_fp_branch_f128_ge
	.size	.Ltest_fp_branch_f128_ge$local, .Lfunc_end258-test_fp_branch_f128_ge
                                        ! -- End function
	.globl	test_builtin_f32_isnan          ! -- Begin function test_builtin_f32_isnan
	.p2align	4
	.type	test_builtin_f32_isnan,@function
test_builtin_f32_isnan:                 ! @test_builtin_f32_isnan
.Ltest_builtin_f32_isnan$local:
	.type	.Ltest_builtin_f32_isnan$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	mov	%r1, %gz
	retf	0

.Lfunc_end259:
	.size	test_builtin_f32_isnan, .Lfunc_end259-test_builtin_f32_isnan
	.size	.Ltest_builtin_f32_isnan$local, .Lfunc_end259-test_builtin_f32_isnan
                                        ! -- End function
	.globl	test_builtin_f32_copysign       ! -- Begin function test_builtin_f32_copysign
	.p2align	4
	.type	test_builtin_f32_copysign,@function
test_builtin_f32_copysign:              ! @test_builtin_f32_copysign
.Ltest_builtin_f32_copysign$local:
	.type	.Ltest_builtin_f32_copysign$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fmergess	%r1, %r2, %r1, %r1
	retf	0

.Lfunc_end260:
	.size	test_builtin_f32_copysign, .Lfunc_end260-test_builtin_f32_copysign
	.size	.Ltest_builtin_f32_copysign$local, .Lfunc_end260-test_builtin_f32_copysign
                                        ! -- End function
	.globl	test_builtin_f64_copysign       ! -- Begin function test_builtin_f64_copysign
	.p2align	4
	.type	test_builtin_f64_copysign,@function
test_builtin_f64_copysign:              ! @test_builtin_f64_copysign
.Ltest_builtin_f64_copysign$local:
	.type	.Ltest_builtin_f64_copysign$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fmergesd	%r1, %r2, %r1, %r1
	retf	0

.Lfunc_end261:
	.size	test_builtin_f64_copysign, .Lfunc_end261-test_builtin_f64_copysign
	.size	.Ltest_builtin_f64_copysign$local, .Lfunc_end261-test_builtin_f64_copysign
                                        ! -- End function
	.globl	test_builtin_f128_copysign      ! -- Begin function test_builtin_f128_copysign
	.p2align	4
	.type	test_builtin_f128_copysign,@function
test_builtin_f128_copysign:             ! @test_builtin_f128_copysign
.Ltest_builtin_f128_copysign$local:
	.type	.Ltest_builtin_f128_copysign$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fmergesq	%r1, %r2, %r1, %r1
	retf	0

.Lfunc_end262:
	.size	test_builtin_f128_copysign, .Lfunc_end262-test_builtin_f128_copysign
	.size	.Ltest_builtin_f128_copysign$local, .Lfunc_end262-test_builtin_f128_copysign
                                        ! -- End function
	.globl	test_compare_f16_eq             ! -- Begin function test_compare_f16_eq
	.p2align	4
	.type	test_compare_f16_eq,@function
test_compare_f16_eq:                    ! @test_compare_f16_eq
.Ltest_compare_f16_eq$local:
	.type	.Ltest_compare_f16_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpshoeq	%r1, %r1, %r2
	retf	0

.Lfunc_end263:
	.size	test_compare_f16_eq, .Lfunc_end263-test_compare_f16_eq
	.size	.Ltest_compare_f16_eq$local, .Lfunc_end263-test_compare_f16_eq
                                        ! -- End function
	.globl	test_compare_f16_ne             ! -- Begin function test_compare_f16_ne
	.p2align	4
	.type	test_compare_f16_ne,@function
test_compare_f16_ne:                    ! @test_compare_f16_ne
.Ltest_compare_f16_ne$local:
	.type	.Ltest_compare_f16_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpshone	%r1, %r1, %r2
	retf	0

.Lfunc_end264:
	.size	test_compare_f16_ne, .Lfunc_end264-test_compare_f16_ne
	.size	.Ltest_compare_f16_ne$local, .Lfunc_end264-test_compare_f16_ne
                                        ! -- End function
	.globl	test_compare_f16_lt             ! -- Begin function test_compare_f16_lt
	.p2align	4
	.type	test_compare_f16_lt,@function
test_compare_f16_lt:                    ! @test_compare_f16_lt
.Ltest_compare_f16_lt$local:
	.type	.Ltest_compare_f16_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsholt	%r1, %r1, %r2
	retf	0

.Lfunc_end265:
	.size	test_compare_f16_lt, .Lfunc_end265-test_compare_f16_lt
	.size	.Ltest_compare_f16_lt$local, .Lfunc_end265-test_compare_f16_lt
                                        ! -- End function
	.globl	test_compare_f16_le             ! -- Begin function test_compare_f16_le
	.p2align	4
	.type	test_compare_f16_le,@function
test_compare_f16_le:                    ! @test_compare_f16_le
.Ltest_compare_f16_le$local:
	.type	.Ltest_compare_f16_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpshole	%r1, %r1, %r2
	retf	0

.Lfunc_end266:
	.size	test_compare_f16_le, .Lfunc_end266-test_compare_f16_le
	.size	.Ltest_compare_f16_le$local, .Lfunc_end266-test_compare_f16_le
                                        ! -- End function
	.globl	test_compare_f16_gt             ! -- Begin function test_compare_f16_gt
	.p2align	4
	.type	test_compare_f16_gt,@function
test_compare_f16_gt:                    ! @test_compare_f16_gt
.Ltest_compare_f16_gt$local:
	.type	.Ltest_compare_f16_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsholt	%r1, %r2, %r1
	retf	0

.Lfunc_end267:
	.size	test_compare_f16_gt, .Lfunc_end267-test_compare_f16_gt
	.size	.Ltest_compare_f16_gt$local, .Lfunc_end267-test_compare_f16_gt
                                        ! -- End function
	.globl	test_compare_f16_ge             ! -- Begin function test_compare_f16_ge
	.p2align	4
	.type	test_compare_f16_ge,@function
test_compare_f16_ge:                    ! @test_compare_f16_ge
.Ltest_compare_f16_ge$local:
	.type	.Ltest_compare_f16_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpshole	%r1, %r2, %r1
	retf	0

.Lfunc_end268:
	.size	test_compare_f16_ge, .Lfunc_end268-test_compare_f16_ge
	.size	.Ltest_compare_f16_ge$local, .Lfunc_end268-test_compare_f16_ge
                                        ! -- End function
	.globl	test_compare_f32_eq             ! -- Begin function test_compare_f32_eq
	.p2align	4
	.type	test_compare_f32_eq,@function
test_compare_f32_eq:                    ! @test_compare_f32_eq
.Ltest_compare_f32_eq$local:
	.type	.Ltest_compare_f32_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpssoeq	%r1, %r1, %r2
	retf	0

.Lfunc_end269:
	.size	test_compare_f32_eq, .Lfunc_end269-test_compare_f32_eq
	.size	.Ltest_compare_f32_eq$local, .Lfunc_end269-test_compare_f32_eq
                                        ! -- End function
	.globl	test_compare_f32_ne             ! -- Begin function test_compare_f32_ne
	.p2align	4
	.type	test_compare_f32_ne,@function
test_compare_f32_ne:                    ! @test_compare_f32_ne
.Ltest_compare_f32_ne$local:
	.type	.Ltest_compare_f32_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpssone	%r1, %r1, %r2
	retf	0

.Lfunc_end270:
	.size	test_compare_f32_ne, .Lfunc_end270-test_compare_f32_ne
	.size	.Ltest_compare_f32_ne$local, .Lfunc_end270-test_compare_f32_ne
                                        ! -- End function
	.globl	test_compare_f32_lt             ! -- Begin function test_compare_f32_lt
	.p2align	4
	.type	test_compare_f32_lt,@function
test_compare_f32_lt:                    ! @test_compare_f32_lt
.Ltest_compare_f32_lt$local:
	.type	.Ltest_compare_f32_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpssolt	%r1, %r1, %r2
	retf	0

.Lfunc_end271:
	.size	test_compare_f32_lt, .Lfunc_end271-test_compare_f32_lt
	.size	.Ltest_compare_f32_lt$local, .Lfunc_end271-test_compare_f32_lt
                                        ! -- End function
	.globl	test_compare_f32_le             ! -- Begin function test_compare_f32_le
	.p2align	4
	.type	test_compare_f32_le,@function
test_compare_f32_le:                    ! @test_compare_f32_le
.Ltest_compare_f32_le$local:
	.type	.Ltest_compare_f32_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpssole	%r1, %r1, %r2
	retf	0

.Lfunc_end272:
	.size	test_compare_f32_le, .Lfunc_end272-test_compare_f32_le
	.size	.Ltest_compare_f32_le$local, .Lfunc_end272-test_compare_f32_le
                                        ! -- End function
	.globl	test_compare_f32_gt             ! -- Begin function test_compare_f32_gt
	.p2align	4
	.type	test_compare_f32_gt,@function
test_compare_f32_gt:                    ! @test_compare_f32_gt
.Ltest_compare_f32_gt$local:
	.type	.Ltest_compare_f32_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpssolt	%r1, %r2, %r1
	retf	0

.Lfunc_end273:
	.size	test_compare_f32_gt, .Lfunc_end273-test_compare_f32_gt
	.size	.Ltest_compare_f32_gt$local, .Lfunc_end273-test_compare_f32_gt
                                        ! -- End function
	.globl	test_compare_f32_ge             ! -- Begin function test_compare_f32_ge
	.p2align	4
	.type	test_compare_f32_ge,@function
test_compare_f32_ge:                    ! @test_compare_f32_ge
.Ltest_compare_f32_ge$local:
	.type	.Ltest_compare_f32_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpssole	%r1, %r2, %r1
	retf	0

.Lfunc_end274:
	.size	test_compare_f32_ge, .Lfunc_end274-test_compare_f32_ge
	.size	.Ltest_compare_f32_ge$local, .Lfunc_end274-test_compare_f32_ge
                                        ! -- End function
	.globl	test_compare_f64_eq             ! -- Begin function test_compare_f64_eq
	.p2align	4
	.type	test_compare_f64_eq,@function
test_compare_f64_eq:                    ! @test_compare_f64_eq
.Ltest_compare_f64_eq$local:
	.type	.Ltest_compare_f64_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsdoeq	%r1, %r1, %r2
	retf	0

.Lfunc_end275:
	.size	test_compare_f64_eq, .Lfunc_end275-test_compare_f64_eq
	.size	.Ltest_compare_f64_eq$local, .Lfunc_end275-test_compare_f64_eq
                                        ! -- End function
	.globl	test_compare_f64_ne             ! -- Begin function test_compare_f64_ne
	.p2align	4
	.type	test_compare_f64_ne,@function
test_compare_f64_ne:                    ! @test_compare_f64_ne
.Ltest_compare_f64_ne$local:
	.type	.Ltest_compare_f64_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsdone	%r1, %r1, %r2
	retf	0

.Lfunc_end276:
	.size	test_compare_f64_ne, .Lfunc_end276-test_compare_f64_ne
	.size	.Ltest_compare_f64_ne$local, .Lfunc_end276-test_compare_f64_ne
                                        ! -- End function
	.globl	test_compare_f64_lt             ! -- Begin function test_compare_f64_lt
	.p2align	4
	.type	test_compare_f64_lt,@function
test_compare_f64_lt:                    ! @test_compare_f64_lt
.Ltest_compare_f64_lt$local:
	.type	.Ltest_compare_f64_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsdolt	%r1, %r1, %r2
	retf	0

.Lfunc_end277:
	.size	test_compare_f64_lt, .Lfunc_end277-test_compare_f64_lt
	.size	.Ltest_compare_f64_lt$local, .Lfunc_end277-test_compare_f64_lt
                                        ! -- End function
	.globl	test_compare_f64_le             ! -- Begin function test_compare_f64_le
	.p2align	4
	.type	test_compare_f64_le,@function
test_compare_f64_le:                    ! @test_compare_f64_le
.Ltest_compare_f64_le$local:
	.type	.Ltest_compare_f64_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsdole	%r1, %r1, %r2
	retf	0

.Lfunc_end278:
	.size	test_compare_f64_le, .Lfunc_end278-test_compare_f64_le
	.size	.Ltest_compare_f64_le$local, .Lfunc_end278-test_compare_f64_le
                                        ! -- End function
	.globl	test_compare_f64_gt             ! -- Begin function test_compare_f64_gt
	.p2align	4
	.type	test_compare_f64_gt,@function
test_compare_f64_gt:                    ! @test_compare_f64_gt
.Ltest_compare_f64_gt$local:
	.type	.Ltest_compare_f64_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsdolt	%r1, %r2, %r1
	retf	0

.Lfunc_end279:
	.size	test_compare_f64_gt, .Lfunc_end279-test_compare_f64_gt
	.size	.Ltest_compare_f64_gt$local, .Lfunc_end279-test_compare_f64_gt
                                        ! -- End function
	.globl	test_compare_f64_ge             ! -- Begin function test_compare_f64_ge
	.p2align	4
	.type	test_compare_f64_ge,@function
test_compare_f64_ge:                    ! @test_compare_f64_ge
.Ltest_compare_f64_ge$local:
	.type	.Ltest_compare_f64_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsdole	%r1, %r2, %r1
	retf	0

.Lfunc_end280:
	.size	test_compare_f64_ge, .Lfunc_end280-test_compare_f64_ge
	.size	.Ltest_compare_f64_ge$local, .Lfunc_end280-test_compare_f64_ge
                                        ! -- End function
	.globl	test_compare_f128_eq            ! -- Begin function test_compare_f128_eq
	.p2align	4
	.type	test_compare_f128_eq,@function
test_compare_f128_eq:                   ! @test_compare_f128_eq
.Ltest_compare_f128_eq$local:
	.type	.Ltest_compare_f128_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsqoeq	%r1, %r1, %r2
	retf	0

.Lfunc_end281:
	.size	test_compare_f128_eq, .Lfunc_end281-test_compare_f128_eq
	.size	.Ltest_compare_f128_eq$local, .Lfunc_end281-test_compare_f128_eq
                                        ! -- End function
	.globl	test_compare_f128_ne            ! -- Begin function test_compare_f128_ne
	.p2align	4
	.type	test_compare_f128_ne,@function
test_compare_f128_ne:                   ! @test_compare_f128_ne
.Ltest_compare_f128_ne$local:
	.type	.Ltest_compare_f128_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsqone	%r1, %r1, %r2
	retf	0

.Lfunc_end282:
	.size	test_compare_f128_ne, .Lfunc_end282-test_compare_f128_ne
	.size	.Ltest_compare_f128_ne$local, .Lfunc_end282-test_compare_f128_ne
                                        ! -- End function
	.globl	test_compare_f128_lt            ! -- Begin function test_compare_f128_lt
	.p2align	4
	.type	test_compare_f128_lt,@function
test_compare_f128_lt:                   ! @test_compare_f128_lt
.Ltest_compare_f128_lt$local:
	.type	.Ltest_compare_f128_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsqolt	%r1, %r1, %r2
	retf	0

.Lfunc_end283:
	.size	test_compare_f128_lt, .Lfunc_end283-test_compare_f128_lt
	.size	.Ltest_compare_f128_lt$local, .Lfunc_end283-test_compare_f128_lt
                                        ! -- End function
	.globl	test_compare_f128_le            ! -- Begin function test_compare_f128_le
	.p2align	4
	.type	test_compare_f128_le,@function
test_compare_f128_le:                   ! @test_compare_f128_le
.Ltest_compare_f128_le$local:
	.type	.Ltest_compare_f128_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsqole	%r1, %r1, %r2
	retf	0

.Lfunc_end284:
	.size	test_compare_f128_le, .Lfunc_end284-test_compare_f128_le
	.size	.Ltest_compare_f128_le$local, .Lfunc_end284-test_compare_f128_le
                                        ! -- End function
	.globl	test_compare_f128_gt            ! -- Begin function test_compare_f128_gt
	.p2align	4
	.type	test_compare_f128_gt,@function
test_compare_f128_gt:                   ! @test_compare_f128_gt
.Ltest_compare_f128_gt$local:
	.type	.Ltest_compare_f128_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsqolt	%r1, %r2, %r1
	retf	0

.Lfunc_end285:
	.size	test_compare_f128_gt, .Lfunc_end285-test_compare_f128_gt
	.size	.Ltest_compare_f128_gt$local, .Lfunc_end285-test_compare_f128_gt
                                        ! -- End function
	.globl	test_compare_f128_ge            ! -- Begin function test_compare_f128_ge
	.p2align	4
	.type	test_compare_f128_ge,@function
test_compare_f128_ge:                   ! @test_compare_f128_ge
.Ltest_compare_f128_ge$local:
	.type	.Ltest_compare_f128_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fcmpsqole	%r1, %r2, %r1
	retf	0

.Lfunc_end286:
	.size	test_compare_f128_ge, .Lfunc_end286-test_compare_f128_ge
	.size	.Ltest_compare_f128_ge$local, .Lfunc_end286-test_compare_f128_ge
                                        ! -- End function
	.globl	test_conv_f16_to_f32            ! -- Begin function test_conv_f16_to_f32
	.p2align	4
	.type	test_conv_f16_to_f32,@function
test_conv_f16_to_f32:                   ! @test_conv_f16_to_f32
.Ltest_conv_f16_to_f32$local:
	.type	.Ltest_conv_f16_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fextsh2ss	%r1, %r1
	retf	0

.Lfunc_end287:
	.size	test_conv_f16_to_f32, .Lfunc_end287-test_conv_f16_to_f32
	.size	.Ltest_conv_f16_to_f32$local, .Lfunc_end287-test_conv_f16_to_f32
                                        ! -- End function
	.globl	test_conv_f16_to_f64            ! -- Begin function test_conv_f16_to_f64
	.p2align	4
	.type	test_conv_f16_to_f64,@function
test_conv_f16_to_f64:                   ! @test_conv_f16_to_f64
.Ltest_conv_f16_to_f64$local:
	.type	.Ltest_conv_f16_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fextsh2sd	%r1, %r1
	retf	0

.Lfunc_end288:
	.size	test_conv_f16_to_f64, .Lfunc_end288-test_conv_f16_to_f64
	.size	.Ltest_conv_f16_to_f64$local, .Lfunc_end288-test_conv_f16_to_f64
                                        ! -- End function
	.globl	test_conv_f16_to_f128           ! -- Begin function test_conv_f16_to_f128
	.p2align	4
	.type	test_conv_f16_to_f128,@function
test_conv_f16_to_f128:                  ! @test_conv_f16_to_f128
.Ltest_conv_f16_to_f128$local:
	.type	.Ltest_conv_f16_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fextsh2sq	%r1, %r1
	retf	0

.Lfunc_end289:
	.size	test_conv_f16_to_f128, .Lfunc_end289-test_conv_f16_to_f128
	.size	.Ltest_conv_f16_to_f128$local, .Lfunc_end289-test_conv_f16_to_f128
                                        ! -- End function
	.globl	test_conv_f64_to_f128           ! -- Begin function test_conv_f64_to_f128
	.p2align	4
	.type	test_conv_f64_to_f128,@function
test_conv_f64_to_f128:                  ! @test_conv_f64_to_f128
.Ltest_conv_f64_to_f128$local:
	.type	.Ltest_conv_f64_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fextsd2sq	%r1, %r1
	retf	0

.Lfunc_end290:
	.size	test_conv_f64_to_f128, .Lfunc_end290-test_conv_f64_to_f128
	.size	.Ltest_conv_f64_to_f128$local, .Lfunc_end290-test_conv_f64_to_f128
                                        ! -- End function
	.globl	test_conv_f32_to_f64            ! -- Begin function test_conv_f32_to_f64
	.p2align	4
	.type	test_conv_f32_to_f64,@function
test_conv_f32_to_f64:                   ! @test_conv_f32_to_f64
.Ltest_conv_f32_to_f64$local:
	.type	.Ltest_conv_f32_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fextss2sd	%r1, %r1
	retf	0

.Lfunc_end291:
	.size	test_conv_f32_to_f64, .Lfunc_end291-test_conv_f32_to_f64
	.size	.Ltest_conv_f32_to_f64$local, .Lfunc_end291-test_conv_f32_to_f64
                                        ! -- End function
	.globl	test_conv_f32_to_f128           ! -- Begin function test_conv_f32_to_f128
	.p2align	4
	.type	test_conv_f32_to_f128,@function
test_conv_f32_to_f128:                  ! @test_conv_f32_to_f128
.Ltest_conv_f32_to_f128$local:
	.type	.Ltest_conv_f32_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fextss2sq	%r1, %r1
	retf	0

.Lfunc_end292:
	.size	test_conv_f32_to_f128, .Lfunc_end292-test_conv_f32_to_f128
	.size	.Ltest_conv_f32_to_f128$local, .Lfunc_end292-test_conv_f32_to_f128
                                        ! -- End function
	.globl	test_conv_f32_to_f16            ! -- Begin function test_conv_f32_to_f16
	.p2align	4
	.type	test_conv_f32_to_f16,@function
test_conv_f32_to_f16:                   ! @test_conv_f32_to_f16
.Ltest_conv_f32_to_f16$local:
	.type	.Ltest_conv_f32_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtss2sh	%r1, %r1, 0
	retf	0

.Lfunc_end293:
	.size	test_conv_f32_to_f16, .Lfunc_end293-test_conv_f32_to_f16
	.size	.Ltest_conv_f32_to_f16$local, .Lfunc_end293-test_conv_f32_to_f16
                                        ! -- End function
	.globl	test_conv_f64_to_f16            ! -- Begin function test_conv_f64_to_f16
	.p2align	4
	.type	test_conv_f64_to_f16,@function
test_conv_f64_to_f16:                   ! @test_conv_f64_to_f16
.Ltest_conv_f64_to_f16$local:
	.type	.Ltest_conv_f64_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2sh	%r1, %r1, 0
	retf	0

.Lfunc_end294:
	.size	test_conv_f64_to_f16, .Lfunc_end294-test_conv_f64_to_f16
	.size	.Ltest_conv_f64_to_f16$local, .Lfunc_end294-test_conv_f64_to_f16
                                        ! -- End function
	.globl	test_conv_f64_to_f32            ! -- Begin function test_conv_f64_to_f32
	.p2align	4
	.type	test_conv_f64_to_f32,@function
test_conv_f64_to_f32:                   ! @test_conv_f64_to_f32
.Ltest_conv_f64_to_f32$local:
	.type	.Ltest_conv_f64_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2ss	%r1, %r1, 0
	retf	0

.Lfunc_end295:
	.size	test_conv_f64_to_f32, .Lfunc_end295-test_conv_f64_to_f32
	.size	.Ltest_conv_f64_to_f32$local, .Lfunc_end295-test_conv_f64_to_f32
                                        ! -- End function
	.globl	test_conv_f128_to_f16           ! -- Begin function test_conv_f128_to_f16
	.p2align	4
	.type	test_conv_f128_to_f16,@function
test_conv_f128_to_f16:                  ! @test_conv_f128_to_f16
.Ltest_conv_f128_to_f16$local:
	.type	.Ltest_conv_f128_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2sh	%r1, %r1, 0
	retf	0

.Lfunc_end296:
	.size	test_conv_f128_to_f16, .Lfunc_end296-test_conv_f128_to_f16
	.size	.Ltest_conv_f128_to_f16$local, .Lfunc_end296-test_conv_f128_to_f16
                                        ! -- End function
	.globl	test_conv_f128_to_f32           ! -- Begin function test_conv_f128_to_f32
	.p2align	4
	.type	test_conv_f128_to_f32,@function
test_conv_f128_to_f32:                  ! @test_conv_f128_to_f32
.Ltest_conv_f128_to_f32$local:
	.type	.Ltest_conv_f128_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2ss	%r1, %r1, 0
	retf	0

.Lfunc_end297:
	.size	test_conv_f128_to_f32, .Lfunc_end297-test_conv_f128_to_f32
	.size	.Ltest_conv_f128_to_f32$local, .Lfunc_end297-test_conv_f128_to_f32
                                        ! -- End function
	.globl	test_conv_f128_to_f64           ! -- Begin function test_conv_f128_to_f64
	.p2align	4
	.type	test_conv_f128_to_f64,@function
test_conv_f128_to_f64:                  ! @test_conv_f128_to_f64
.Ltest_conv_f128_to_f64$local:
	.type	.Ltest_conv_f128_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2sd	%r1, %r1, 0
	retf	0

.Lfunc_end298:
	.size	test_conv_f128_to_f64, .Lfunc_end298-test_conv_f128_to_f64
	.size	.Ltest_conv_f128_to_f64$local, .Lfunc_end298-test_conv_f128_to_f64
                                        ! -- End function
	.globl	test_conv_f16_to_i32            ! -- Begin function test_conv_f16_to_i32
	.p2align	4
	.type	test_conv_f16_to_i32,@function
test_conv_f16_to_i32:                   ! @test_conv_f16_to_i32
.Ltest_conv_f16_to_i32$local:
	.type	.Ltest_conv_f16_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsh2iw	%r1, %r1, 0
	retf	0

.Lfunc_end299:
	.size	test_conv_f16_to_i32, .Lfunc_end299-test_conv_f16_to_i32
	.size	.Ltest_conv_f16_to_i32$local, .Lfunc_end299-test_conv_f16_to_i32
                                        ! -- End function
	.globl	test_conv_f16_to_u32            ! -- Begin function test_conv_f16_to_u32
	.p2align	4
	.type	test_conv_f16_to_u32,@function
test_conv_f16_to_u32:                   ! @test_conv_f16_to_u32
.Ltest_conv_f16_to_u32$local:
	.type	.Ltest_conv_f16_to_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsh2uw	%r1, %r1, 0
	retf	0

.Lfunc_end300:
	.size	test_conv_f16_to_u32, .Lfunc_end300-test_conv_f16_to_u32
	.size	.Ltest_conv_f16_to_u32$local, .Lfunc_end300-test_conv_f16_to_u32
                                        ! -- End function
	.globl	test_conv_f16_to_i64            ! -- Begin function test_conv_f16_to_i64
	.p2align	4
	.type	test_conv_f16_to_i64,@function
test_conv_f16_to_i64:                   ! @test_conv_f16_to_i64
.Ltest_conv_f16_to_i64$local:
	.type	.Ltest_conv_f16_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsh2id	%r1, %r1, 0
	retf	0

.Lfunc_end301:
	.size	test_conv_f16_to_i64, .Lfunc_end301-test_conv_f16_to_i64
	.size	.Ltest_conv_f16_to_i64$local, .Lfunc_end301-test_conv_f16_to_i64
                                        ! -- End function
	.globl	test_conv_f16_to_u64            ! -- Begin function test_conv_f16_to_u64
	.p2align	4
	.type	test_conv_f16_to_u64,@function
test_conv_f16_to_u64:                   ! @test_conv_f16_to_u64
.Ltest_conv_f16_to_u64$local:
	.type	.Ltest_conv_f16_to_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsh2ud	%r1, %r1, 0
	retf	0

.Lfunc_end302:
	.size	test_conv_f16_to_u64, .Lfunc_end302-test_conv_f16_to_u64
	.size	.Ltest_conv_f16_to_u64$local, .Lfunc_end302-test_conv_f16_to_u64
                                        ! -- End function
	.globl	test_conv_f16_to_i128           ! -- Begin function test_conv_f16_to_i128
	.p2align	4
	.type	test_conv_f16_to_i128,@function
test_conv_f16_to_i128:                  ! @test_conv_f16_to_i128
.Ltest_conv_f16_to_i128$local:
	.type	.Ltest_conv_f16_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsh2iq	%r1, %r1, 0
	retf	0

.Lfunc_end303:
	.size	test_conv_f16_to_i128, .Lfunc_end303-test_conv_f16_to_i128
	.size	.Ltest_conv_f16_to_i128$local, .Lfunc_end303-test_conv_f16_to_i128
                                        ! -- End function
	.globl	test_conv_f16_to_u128           ! -- Begin function test_conv_f16_to_u128
	.p2align	4
	.type	test_conv_f16_to_u128,@function
test_conv_f16_to_u128:                  ! @test_conv_f16_to_u128
.Ltest_conv_f16_to_u128$local:
	.type	.Ltest_conv_f16_to_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsh2uq	%r1, %r1, 0
	retf	0

.Lfunc_end304:
	.size	test_conv_f16_to_u128, .Lfunc_end304-test_conv_f16_to_u128
	.size	.Ltest_conv_f16_to_u128$local, .Lfunc_end304-test_conv_f16_to_u128
                                        ! -- End function
	.globl	test_conv_i32_to_f16            ! -- Begin function test_conv_i32_to_f16
	.p2align	4
	.type	test_conv_i32_to_f16,@function
test_conv_i32_to_f16:                   ! @test_conv_i32_to_f16
.Ltest_conv_i32_to_f16$local:
	.type	.Ltest_conv_i32_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiw2sh	%r1, %r1, 0
	retf	0

.Lfunc_end305:
	.size	test_conv_i32_to_f16, .Lfunc_end305-test_conv_i32_to_f16
	.size	.Ltest_conv_i32_to_f16$local, .Lfunc_end305-test_conv_i32_to_f16
                                        ! -- End function
	.globl	test_conv_u32_to_f16            ! -- Begin function test_conv_u32_to_f16
	.p2align	4
	.type	test_conv_u32_to_f16,@function
test_conv_u32_to_f16:                   ! @test_conv_u32_to_f16
.Ltest_conv_u32_to_f16$local:
	.type	.Ltest_conv_u32_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuw2sh	%r1, %r1, 0
	retf	0

.Lfunc_end306:
	.size	test_conv_u32_to_f16, .Lfunc_end306-test_conv_u32_to_f16
	.size	.Ltest_conv_u32_to_f16$local, .Lfunc_end306-test_conv_u32_to_f16
                                        ! -- End function
	.globl	test_conv_i64_to_f16            ! -- Begin function test_conv_i64_to_f16
	.p2align	4
	.type	test_conv_i64_to_f16,@function
test_conv_i64_to_f16:                   ! @test_conv_i64_to_f16
.Ltest_conv_i64_to_f16$local:
	.type	.Ltest_conv_i64_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtid2sh	%r1, %r1, 0
	retf	0

.Lfunc_end307:
	.size	test_conv_i64_to_f16, .Lfunc_end307-test_conv_i64_to_f16
	.size	.Ltest_conv_i64_to_f16$local, .Lfunc_end307-test_conv_i64_to_f16
                                        ! -- End function
	.globl	test_conv_u64_to_f16            ! -- Begin function test_conv_u64_to_f16
	.p2align	4
	.type	test_conv_u64_to_f16,@function
test_conv_u64_to_f16:                   ! @test_conv_u64_to_f16
.Ltest_conv_u64_to_f16$local:
	.type	.Ltest_conv_u64_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtud2sh	%r1, %r1, 0
	retf	0

.Lfunc_end308:
	.size	test_conv_u64_to_f16, .Lfunc_end308-test_conv_u64_to_f16
	.size	.Ltest_conv_u64_to_f16$local, .Lfunc_end308-test_conv_u64_to_f16
                                        ! -- End function
	.globl	test_conv_i128_to_f16           ! -- Begin function test_conv_i128_to_f16
	.p2align	4
	.type	test_conv_i128_to_f16,@function
test_conv_i128_to_f16:                  ! @test_conv_i128_to_f16
.Ltest_conv_i128_to_f16$local:
	.type	.Ltest_conv_i128_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiq2sh	%r1, %r1, 0
	retf	0

.Lfunc_end309:
	.size	test_conv_i128_to_f16, .Lfunc_end309-test_conv_i128_to_f16
	.size	.Ltest_conv_i128_to_f16$local, .Lfunc_end309-test_conv_i128_to_f16
                                        ! -- End function
	.globl	test_conv_u128_to_f16           ! -- Begin function test_conv_u128_to_f16
	.p2align	4
	.type	test_conv_u128_to_f16,@function
test_conv_u128_to_f16:                  ! @test_conv_u128_to_f16
.Ltest_conv_u128_to_f16$local:
	.type	.Ltest_conv_u128_to_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuq2sh	%r1, %r1, 0
	retf	0

.Lfunc_end310:
	.size	test_conv_u128_to_f16, .Lfunc_end310-test_conv_u128_to_f16
	.size	.Ltest_conv_u128_to_f16$local, .Lfunc_end310-test_conv_u128_to_f16
                                        ! -- End function
	.globl	test_conv_f32_to_i32            ! -- Begin function test_conv_f32_to_i32
	.p2align	4
	.type	test_conv_f32_to_i32,@function
test_conv_f32_to_i32:                   ! @test_conv_f32_to_i32
.Ltest_conv_f32_to_i32$local:
	.type	.Ltest_conv_f32_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtss2iw	%r1, %r1, 0
	retf	0

.Lfunc_end311:
	.size	test_conv_f32_to_i32, .Lfunc_end311-test_conv_f32_to_i32
	.size	.Ltest_conv_f32_to_i32$local, .Lfunc_end311-test_conv_f32_to_i32
                                        ! -- End function
	.globl	test_conv_f32_to_u32            ! -- Begin function test_conv_f32_to_u32
	.p2align	4
	.type	test_conv_f32_to_u32,@function
test_conv_f32_to_u32:                   ! @test_conv_f32_to_u32
.Ltest_conv_f32_to_u32$local:
	.type	.Ltest_conv_f32_to_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtss2uw	%r1, %r1, 0
	retf	0

.Lfunc_end312:
	.size	test_conv_f32_to_u32, .Lfunc_end312-test_conv_f32_to_u32
	.size	.Ltest_conv_f32_to_u32$local, .Lfunc_end312-test_conv_f32_to_u32
                                        ! -- End function
	.globl	test_conv_f32_to_i64            ! -- Begin function test_conv_f32_to_i64
	.p2align	4
	.type	test_conv_f32_to_i64,@function
test_conv_f32_to_i64:                   ! @test_conv_f32_to_i64
.Ltest_conv_f32_to_i64$local:
	.type	.Ltest_conv_f32_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtss2id	%r1, %r1, 0
	retf	0

.Lfunc_end313:
	.size	test_conv_f32_to_i64, .Lfunc_end313-test_conv_f32_to_i64
	.size	.Ltest_conv_f32_to_i64$local, .Lfunc_end313-test_conv_f32_to_i64
                                        ! -- End function
	.globl	test_conv_f32_to_u64            ! -- Begin function test_conv_f32_to_u64
	.p2align	4
	.type	test_conv_f32_to_u64,@function
test_conv_f32_to_u64:                   ! @test_conv_f32_to_u64
.Ltest_conv_f32_to_u64$local:
	.type	.Ltest_conv_f32_to_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtss2ud	%r1, %r1, 0
	retf	0

.Lfunc_end314:
	.size	test_conv_f32_to_u64, .Lfunc_end314-test_conv_f32_to_u64
	.size	.Ltest_conv_f32_to_u64$local, .Lfunc_end314-test_conv_f32_to_u64
                                        ! -- End function
	.globl	test_conv_f32_to_i128           ! -- Begin function test_conv_f32_to_i128
	.p2align	4
	.type	test_conv_f32_to_i128,@function
test_conv_f32_to_i128:                  ! @test_conv_f32_to_i128
.Ltest_conv_f32_to_i128$local:
	.type	.Ltest_conv_f32_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtss2iq	%r1, %r1, 0
	retf	0

.Lfunc_end315:
	.size	test_conv_f32_to_i128, .Lfunc_end315-test_conv_f32_to_i128
	.size	.Ltest_conv_f32_to_i128$local, .Lfunc_end315-test_conv_f32_to_i128
                                        ! -- End function
	.globl	test_conv_f32_to_u128           ! -- Begin function test_conv_f32_to_u128
	.p2align	4
	.type	test_conv_f32_to_u128,@function
test_conv_f32_to_u128:                  ! @test_conv_f32_to_u128
.Ltest_conv_f32_to_u128$local:
	.type	.Ltest_conv_f32_to_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtss2uq	%r1, %r1, 0
	retf	0

.Lfunc_end316:
	.size	test_conv_f32_to_u128, .Lfunc_end316-test_conv_f32_to_u128
	.size	.Ltest_conv_f32_to_u128$local, .Lfunc_end316-test_conv_f32_to_u128
                                        ! -- End function
	.globl	test_conv_i32_to_f32            ! -- Begin function test_conv_i32_to_f32
	.p2align	4
	.type	test_conv_i32_to_f32,@function
test_conv_i32_to_f32:                   ! @test_conv_i32_to_f32
.Ltest_conv_i32_to_f32$local:
	.type	.Ltest_conv_i32_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiw2ss	%r1, %r1, 0
	retf	0

.Lfunc_end317:
	.size	test_conv_i32_to_f32, .Lfunc_end317-test_conv_i32_to_f32
	.size	.Ltest_conv_i32_to_f32$local, .Lfunc_end317-test_conv_i32_to_f32
                                        ! -- End function
	.globl	test_conv_u32_to_f32            ! -- Begin function test_conv_u32_to_f32
	.p2align	4
	.type	test_conv_u32_to_f32,@function
test_conv_u32_to_f32:                   ! @test_conv_u32_to_f32
.Ltest_conv_u32_to_f32$local:
	.type	.Ltest_conv_u32_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuw2ss	%r1, %r1, 0
	retf	0

.Lfunc_end318:
	.size	test_conv_u32_to_f32, .Lfunc_end318-test_conv_u32_to_f32
	.size	.Ltest_conv_u32_to_f32$local, .Lfunc_end318-test_conv_u32_to_f32
                                        ! -- End function
	.globl	test_conv_i64_to_f32            ! -- Begin function test_conv_i64_to_f32
	.p2align	4
	.type	test_conv_i64_to_f32,@function
test_conv_i64_to_f32:                   ! @test_conv_i64_to_f32
.Ltest_conv_i64_to_f32$local:
	.type	.Ltest_conv_i64_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtid2ss	%r1, %r1, 0
	retf	0

.Lfunc_end319:
	.size	test_conv_i64_to_f32, .Lfunc_end319-test_conv_i64_to_f32
	.size	.Ltest_conv_i64_to_f32$local, .Lfunc_end319-test_conv_i64_to_f32
                                        ! -- End function
	.globl	test_conv_u64_to_f32            ! -- Begin function test_conv_u64_to_f32
	.p2align	4
	.type	test_conv_u64_to_f32,@function
test_conv_u64_to_f32:                   ! @test_conv_u64_to_f32
.Ltest_conv_u64_to_f32$local:
	.type	.Ltest_conv_u64_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtud2ss	%r1, %r1, 0
	retf	0

.Lfunc_end320:
	.size	test_conv_u64_to_f32, .Lfunc_end320-test_conv_u64_to_f32
	.size	.Ltest_conv_u64_to_f32$local, .Lfunc_end320-test_conv_u64_to_f32
                                        ! -- End function
	.globl	test_conv_i128_to_f32           ! -- Begin function test_conv_i128_to_f32
	.p2align	4
	.type	test_conv_i128_to_f32,@function
test_conv_i128_to_f32:                  ! @test_conv_i128_to_f32
.Ltest_conv_i128_to_f32$local:
	.type	.Ltest_conv_i128_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiq2ss	%r1, %r1, 0
	retf	0

.Lfunc_end321:
	.size	test_conv_i128_to_f32, .Lfunc_end321-test_conv_i128_to_f32
	.size	.Ltest_conv_i128_to_f32$local, .Lfunc_end321-test_conv_i128_to_f32
                                        ! -- End function
	.globl	test_conv_u128_to_f32           ! -- Begin function test_conv_u128_to_f32
	.p2align	4
	.type	test_conv_u128_to_f32,@function
test_conv_u128_to_f32:                  ! @test_conv_u128_to_f32
.Ltest_conv_u128_to_f32$local:
	.type	.Ltest_conv_u128_to_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuq2ss	%r1, %r1, 0
	retf	0

.Lfunc_end322:
	.size	test_conv_u128_to_f32, .Lfunc_end322-test_conv_u128_to_f32
	.size	.Ltest_conv_u128_to_f32$local, .Lfunc_end322-test_conv_u128_to_f32
                                        ! -- End function
	.globl	test_conv_f64_to_i32            ! -- Begin function test_conv_f64_to_i32
	.p2align	4
	.type	test_conv_f64_to_i32,@function
test_conv_f64_to_i32:                   ! @test_conv_f64_to_i32
.Ltest_conv_f64_to_i32$local:
	.type	.Ltest_conv_f64_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2iw	%r1, %r1, 0
	retf	0

.Lfunc_end323:
	.size	test_conv_f64_to_i32, .Lfunc_end323-test_conv_f64_to_i32
	.size	.Ltest_conv_f64_to_i32$local, .Lfunc_end323-test_conv_f64_to_i32
                                        ! -- End function
	.globl	test_conv_f64_to_u32            ! -- Begin function test_conv_f64_to_u32
	.p2align	4
	.type	test_conv_f64_to_u32,@function
test_conv_f64_to_u32:                   ! @test_conv_f64_to_u32
.Ltest_conv_f64_to_u32$local:
	.type	.Ltest_conv_f64_to_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2uw	%r1, %r1, 0
	retf	0

.Lfunc_end324:
	.size	test_conv_f64_to_u32, .Lfunc_end324-test_conv_f64_to_u32
	.size	.Ltest_conv_f64_to_u32$local, .Lfunc_end324-test_conv_f64_to_u32
                                        ! -- End function
	.globl	test_conv_f64_to_i64            ! -- Begin function test_conv_f64_to_i64
	.p2align	4
	.type	test_conv_f64_to_i64,@function
test_conv_f64_to_i64:                   ! @test_conv_f64_to_i64
.Ltest_conv_f64_to_i64$local:
	.type	.Ltest_conv_f64_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2id	%r1, %r1, 0
	retf	0

.Lfunc_end325:
	.size	test_conv_f64_to_i64, .Lfunc_end325-test_conv_f64_to_i64
	.size	.Ltest_conv_f64_to_i64$local, .Lfunc_end325-test_conv_f64_to_i64
                                        ! -- End function
	.globl	test_conv_f64_to_u64            ! -- Begin function test_conv_f64_to_u64
	.p2align	4
	.type	test_conv_f64_to_u64,@function
test_conv_f64_to_u64:                   ! @test_conv_f64_to_u64
.Ltest_conv_f64_to_u64$local:
	.type	.Ltest_conv_f64_to_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2ud	%r1, %r1, 0
	retf	0

.Lfunc_end326:
	.size	test_conv_f64_to_u64, .Lfunc_end326-test_conv_f64_to_u64
	.size	.Ltest_conv_f64_to_u64$local, .Lfunc_end326-test_conv_f64_to_u64
                                        ! -- End function
	.globl	test_conv_f64_to_i128           ! -- Begin function test_conv_f64_to_i128
	.p2align	4
	.type	test_conv_f64_to_i128,@function
test_conv_f64_to_i128:                  ! @test_conv_f64_to_i128
.Ltest_conv_f64_to_i128$local:
	.type	.Ltest_conv_f64_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2iq	%r1, %r1, 0
	retf	0

.Lfunc_end327:
	.size	test_conv_f64_to_i128, .Lfunc_end327-test_conv_f64_to_i128
	.size	.Ltest_conv_f64_to_i128$local, .Lfunc_end327-test_conv_f64_to_i128
                                        ! -- End function
	.globl	test_conv_f64_to_u128           ! -- Begin function test_conv_f64_to_u128
	.p2align	4
	.type	test_conv_f64_to_u128,@function
test_conv_f64_to_u128:                  ! @test_conv_f64_to_u128
.Ltest_conv_f64_to_u128$local:
	.type	.Ltest_conv_f64_to_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsd2uq	%r1, %r1, 0
	retf	0

.Lfunc_end328:
	.size	test_conv_f64_to_u128, .Lfunc_end328-test_conv_f64_to_u128
	.size	.Ltest_conv_f64_to_u128$local, .Lfunc_end328-test_conv_f64_to_u128
                                        ! -- End function
	.globl	test_conv_i32_to_f64            ! -- Begin function test_conv_i32_to_f64
	.p2align	4
	.type	test_conv_i32_to_f64,@function
test_conv_i32_to_f64:                   ! @test_conv_i32_to_f64
.Ltest_conv_i32_to_f64$local:
	.type	.Ltest_conv_i32_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiw2sd	%r1, %r1, 0
	retf	0

.Lfunc_end329:
	.size	test_conv_i32_to_f64, .Lfunc_end329-test_conv_i32_to_f64
	.size	.Ltest_conv_i32_to_f64$local, .Lfunc_end329-test_conv_i32_to_f64
                                        ! -- End function
	.globl	test_conv_u32_to_f64            ! -- Begin function test_conv_u32_to_f64
	.p2align	4
	.type	test_conv_u32_to_f64,@function
test_conv_u32_to_f64:                   ! @test_conv_u32_to_f64
.Ltest_conv_u32_to_f64$local:
	.type	.Ltest_conv_u32_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuw2sd	%r1, %r1, 0
	retf	0

.Lfunc_end330:
	.size	test_conv_u32_to_f64, .Lfunc_end330-test_conv_u32_to_f64
	.size	.Ltest_conv_u32_to_f64$local, .Lfunc_end330-test_conv_u32_to_f64
                                        ! -- End function
	.globl	test_conv_i64_to_f64            ! -- Begin function test_conv_i64_to_f64
	.p2align	4
	.type	test_conv_i64_to_f64,@function
test_conv_i64_to_f64:                   ! @test_conv_i64_to_f64
.Ltest_conv_i64_to_f64$local:
	.type	.Ltest_conv_i64_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtid2sd	%r1, %r1, 0
	retf	0

.Lfunc_end331:
	.size	test_conv_i64_to_f64, .Lfunc_end331-test_conv_i64_to_f64
	.size	.Ltest_conv_i64_to_f64$local, .Lfunc_end331-test_conv_i64_to_f64
                                        ! -- End function
	.globl	test_conv_u64_to_f64            ! -- Begin function test_conv_u64_to_f64
	.p2align	4
	.type	test_conv_u64_to_f64,@function
test_conv_u64_to_f64:                   ! @test_conv_u64_to_f64
.Ltest_conv_u64_to_f64$local:
	.type	.Ltest_conv_u64_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtud2sd	%r1, %r1, 0
	retf	0

.Lfunc_end332:
	.size	test_conv_u64_to_f64, .Lfunc_end332-test_conv_u64_to_f64
	.size	.Ltest_conv_u64_to_f64$local, .Lfunc_end332-test_conv_u64_to_f64
                                        ! -- End function
	.globl	test_conv_i128_to_f64           ! -- Begin function test_conv_i128_to_f64
	.p2align	4
	.type	test_conv_i128_to_f64,@function
test_conv_i128_to_f64:                  ! @test_conv_i128_to_f64
.Ltest_conv_i128_to_f64$local:
	.type	.Ltest_conv_i128_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiq2sd	%r1, %r1, 0
	retf	0

.Lfunc_end333:
	.size	test_conv_i128_to_f64, .Lfunc_end333-test_conv_i128_to_f64
	.size	.Ltest_conv_i128_to_f64$local, .Lfunc_end333-test_conv_i128_to_f64
                                        ! -- End function
	.globl	test_conv_u128_to_f64           ! -- Begin function test_conv_u128_to_f64
	.p2align	4
	.type	test_conv_u128_to_f64,@function
test_conv_u128_to_f64:                  ! @test_conv_u128_to_f64
.Ltest_conv_u128_to_f64$local:
	.type	.Ltest_conv_u128_to_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuq2sd	%r1, %r1, 0
	retf	0

.Lfunc_end334:
	.size	test_conv_u128_to_f64, .Lfunc_end334-test_conv_u128_to_f64
	.size	.Ltest_conv_u128_to_f64$local, .Lfunc_end334-test_conv_u128_to_f64
                                        ! -- End function
	.globl	test_conv_f128_to_i32           ! -- Begin function test_conv_f128_to_i32
	.p2align	4
	.type	test_conv_f128_to_i32,@function
test_conv_f128_to_i32:                  ! @test_conv_f128_to_i32
.Ltest_conv_f128_to_i32$local:
	.type	.Ltest_conv_f128_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2iw	%r1, %r1, 0
	retf	0

.Lfunc_end335:
	.size	test_conv_f128_to_i32, .Lfunc_end335-test_conv_f128_to_i32
	.size	.Ltest_conv_f128_to_i32$local, .Lfunc_end335-test_conv_f128_to_i32
                                        ! -- End function
	.globl	test_conv_f128_to_u32           ! -- Begin function test_conv_f128_to_u32
	.p2align	4
	.type	test_conv_f128_to_u32,@function
test_conv_f128_to_u32:                  ! @test_conv_f128_to_u32
.Ltest_conv_f128_to_u32$local:
	.type	.Ltest_conv_f128_to_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2uw	%r1, %r1, 0
	retf	0

.Lfunc_end336:
	.size	test_conv_f128_to_u32, .Lfunc_end336-test_conv_f128_to_u32
	.size	.Ltest_conv_f128_to_u32$local, .Lfunc_end336-test_conv_f128_to_u32
                                        ! -- End function
	.globl	test_conv_f128_to_i64           ! -- Begin function test_conv_f128_to_i64
	.p2align	4
	.type	test_conv_f128_to_i64,@function
test_conv_f128_to_i64:                  ! @test_conv_f128_to_i64
.Ltest_conv_f128_to_i64$local:
	.type	.Ltest_conv_f128_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2id	%r1, %r1, 0
	retf	0

.Lfunc_end337:
	.size	test_conv_f128_to_i64, .Lfunc_end337-test_conv_f128_to_i64
	.size	.Ltest_conv_f128_to_i64$local, .Lfunc_end337-test_conv_f128_to_i64
                                        ! -- End function
	.globl	test_conv_f128_to_u64           ! -- Begin function test_conv_f128_to_u64
	.p2align	4
	.type	test_conv_f128_to_u64,@function
test_conv_f128_to_u64:                  ! @test_conv_f128_to_u64
.Ltest_conv_f128_to_u64$local:
	.type	.Ltest_conv_f128_to_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2ud	%r1, %r1, 0
	retf	0

.Lfunc_end338:
	.size	test_conv_f128_to_u64, .Lfunc_end338-test_conv_f128_to_u64
	.size	.Ltest_conv_f128_to_u64$local, .Lfunc_end338-test_conv_f128_to_u64
                                        ! -- End function
	.globl	test_conv_f128_to_i128          ! -- Begin function test_conv_f128_to_i128
	.p2align	4
	.type	test_conv_f128_to_i128,@function
test_conv_f128_to_i128:                 ! @test_conv_f128_to_i128
.Ltest_conv_f128_to_i128$local:
	.type	.Ltest_conv_f128_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2iq	%r1, %r1, 0
	retf	0

.Lfunc_end339:
	.size	test_conv_f128_to_i128, .Lfunc_end339-test_conv_f128_to_i128
	.size	.Ltest_conv_f128_to_i128$local, .Lfunc_end339-test_conv_f128_to_i128
                                        ! -- End function
	.globl	test_conv_f128_to_u128          ! -- Begin function test_conv_f128_to_u128
	.p2align	4
	.type	test_conv_f128_to_u128,@function
test_conv_f128_to_u128:                 ! @test_conv_f128_to_u128
.Ltest_conv_f128_to_u128$local:
	.type	.Ltest_conv_f128_to_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtsq2uq	%r1, %r1, 0
	retf	0

.Lfunc_end340:
	.size	test_conv_f128_to_u128, .Lfunc_end340-test_conv_f128_to_u128
	.size	.Ltest_conv_f128_to_u128$local, .Lfunc_end340-test_conv_f128_to_u128
                                        ! -- End function
	.globl	test_conv_i32_to_f128           ! -- Begin function test_conv_i32_to_f128
	.p2align	4
	.type	test_conv_i32_to_f128,@function
test_conv_i32_to_f128:                  ! @test_conv_i32_to_f128
.Ltest_conv_i32_to_f128$local:
	.type	.Ltest_conv_i32_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiw2sq	%r1, %r1, 0
	retf	0

.Lfunc_end341:
	.size	test_conv_i32_to_f128, .Lfunc_end341-test_conv_i32_to_f128
	.size	.Ltest_conv_i32_to_f128$local, .Lfunc_end341-test_conv_i32_to_f128
                                        ! -- End function
	.globl	test_conv_u32_to_f128           ! -- Begin function test_conv_u32_to_f128
	.p2align	4
	.type	test_conv_u32_to_f128,@function
test_conv_u32_to_f128:                  ! @test_conv_u32_to_f128
.Ltest_conv_u32_to_f128$local:
	.type	.Ltest_conv_u32_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuw2sq	%r1, %r1, 0
	retf	0

.Lfunc_end342:
	.size	test_conv_u32_to_f128, .Lfunc_end342-test_conv_u32_to_f128
	.size	.Ltest_conv_u32_to_f128$local, .Lfunc_end342-test_conv_u32_to_f128
                                        ! -- End function
	.globl	test_conv_i64_to_f128           ! -- Begin function test_conv_i64_to_f128
	.p2align	4
	.type	test_conv_i64_to_f128,@function
test_conv_i64_to_f128:                  ! @test_conv_i64_to_f128
.Ltest_conv_i64_to_f128$local:
	.type	.Ltest_conv_i64_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtid2sq	%r1, %r1, 0
	retf	0

.Lfunc_end343:
	.size	test_conv_i64_to_f128, .Lfunc_end343-test_conv_i64_to_f128
	.size	.Ltest_conv_i64_to_f128$local, .Lfunc_end343-test_conv_i64_to_f128
                                        ! -- End function
	.globl	test_conv_u64_to_f128           ! -- Begin function test_conv_u64_to_f128
	.p2align	4
	.type	test_conv_u64_to_f128,@function
test_conv_u64_to_f128:                  ! @test_conv_u64_to_f128
.Ltest_conv_u64_to_f128$local:
	.type	.Ltest_conv_u64_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtud2sq	%r1, %r1, 0
	retf	0

.Lfunc_end344:
	.size	test_conv_u64_to_f128, .Lfunc_end344-test_conv_u64_to_f128
	.size	.Ltest_conv_u64_to_f128$local, .Lfunc_end344-test_conv_u64_to_f128
                                        ! -- End function
	.globl	test_conv_i128_to_f128          ! -- Begin function test_conv_i128_to_f128
	.p2align	4
	.type	test_conv_i128_to_f128,@function
test_conv_i128_to_f128:                 ! @test_conv_i128_to_f128
.Ltest_conv_i128_to_f128$local:
	.type	.Ltest_conv_i128_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtiq2sq	%r1, %r1, 0
	retf	0

.Lfunc_end345:
	.size	test_conv_i128_to_f128, .Lfunc_end345-test_conv_i128_to_f128
	.size	.Ltest_conv_i128_to_f128$local, .Lfunc_end345-test_conv_i128_to_f128
                                        ! -- End function
	.globl	test_conv_u128_to_f128          ! -- Begin function test_conv_u128_to_f128
	.p2align	4
	.type	test_conv_u128_to_f128,@function
test_conv_u128_to_f128:                 ! @test_conv_u128_to_f128
.Ltest_conv_u128_to_f128$local:
	.type	.Ltest_conv_u128_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fcvtuq2sq	%r1, %r1, 0
	retf	0

.Lfunc_end346:
	.size	test_conv_u128_to_f128, .Lfunc_end346-test_conv_u128_to_f128
	.size	.Ltest_conv_u128_to_f128$local, .Lfunc_end346-test_conv_u128_to_f128
                                        ! -- End function
	.globl	test_fp_immediate_f16           ! -- Begin function test_fp_immediate_f16
	.p2align	4
	.type	test_fp_immediate_f16,@function
test_fp_immediate_f16:                  ! @test_fp_immediate_f16
.Ltest_fp_immediate_f16$local:
	.type	.Ltest_fp_immediate_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 12263
	retf	0

.Lfunc_end347:
	.size	test_fp_immediate_f16, .Lfunc_end347-test_fp_immediate_f16
	.size	.Ltest_fp_immediate_f16$local, .Lfunc_end347-test_fp_immediate_f16
                                        ! -- End function
	.globl	test_fp_immediate_f32           ! -- Begin function test_fp_immediate_f32
	.p2align	4
	.type	test_fp_immediate_f32,@function
test_fp_immediate_f32:                  ! @test_fp_immediate_f32
.Ltest_fp_immediate_f32$local:
	.type	.Ltest_fp_immediate_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, 1039980160

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end348:
	.size	test_fp_immediate_f32, .Lfunc_end348-test_fp_immediate_f32
	.size	.Ltest_fp_immediate_f32$local, .Lfunc_end348-test_fp_immediate_f32
                                        ! -- End function
	.globl	test_fp_immediate_f64           ! -- Begin function test_fp_immediate_f64
	.p2align	4
	.type	test_fp_immediate_f64,@function
test_fp_immediate_f64:                  ! @test_fp_immediate_f64
.Ltest_fp_immediate_f64$local:
	.type	.Ltest_fp_immediate_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, -4629811673862064449

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end349:
	.size	test_fp_immediate_f64, .Lfunc_end349-test_fp_immediate_f64
	.size	.Ltest_fp_immediate_f64$local, .Lfunc_end349-test_fp_immediate_f64
                                        ! -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          ! -- Begin function test_fp_immediate_f128
.LCPI350_0:
	.xword	0xf444e0daa0cae643              ! fp128 0.123456000000000000000000000000000003
	.xword	0x3ffbf9acffa7eb6b
	.text
	.globl	test_fp_immediate_f128
	.p2align	4
	.type	test_fp_immediate_f128,@function
test_fp_immediate_f128:                 ! @test_fp_immediate_f128
.Ltest_fp_immediate_f128$local:
	.type	.Ltest_fp_immediate_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldqr	%r1, %pcfwd(.LCPI350_0)
	retf	0

.Lfunc_end350:
	.size	test_fp_immediate_f128, .Lfunc_end350-test_fp_immediate_f128
	.size	.Ltest_fp_immediate_f128$local, .Lfunc_end350-test_fp_immediate_f128
                                        ! -- End function
	.globl	test_round_f32                  ! -- Begin function test_round_f32
	.p2align	4
	.type	test_round_f32,@function
test_round_f32:                         ! @test_round_f32
.Ltest_round_f32$local:
	.type	.Ltest_round_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndss.even	%r1, %r1
	retf	0

.Lfunc_end351:
	.size	test_round_f32, .Lfunc_end351-test_round_f32
	.size	.Ltest_round_f32$local, .Lfunc_end351-test_round_f32
                                        ! -- End function
	.globl	test_floor_f32                  ! -- Begin function test_floor_f32
	.p2align	4
	.type	test_floor_f32,@function
test_floor_f32:                         ! @test_floor_f32
.Ltest_floor_f32$local:
	.type	.Ltest_floor_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndss.floor	%r1, %r1
	retf	0

.Lfunc_end352:
	.size	test_floor_f32, .Lfunc_end352-test_floor_f32
	.size	.Ltest_floor_f32$local, .Lfunc_end352-test_floor_f32
                                        ! -- End function
	.globl	test_ceil_f32                   ! -- Begin function test_ceil_f32
	.p2align	4
	.type	test_ceil_f32,@function
test_ceil_f32:                          ! @test_ceil_f32
.Ltest_ceil_f32$local:
	.type	.Ltest_ceil_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndss.ceil	%r1, %r1
	retf	0

.Lfunc_end353:
	.size	test_ceil_f32, .Lfunc_end353-test_ceil_f32
	.size	.Ltest_ceil_f32$local, .Lfunc_end353-test_ceil_f32
                                        ! -- End function
	.globl	test_trunc_f32                  ! -- Begin function test_trunc_f32
	.p2align	4
	.type	test_trunc_f32,@function
test_trunc_f32:                         ! @test_trunc_f32
.Ltest_trunc_f32$local:
	.type	.Ltest_trunc_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndss.trunc	%r1, %r1
	retf	0

.Lfunc_end354:
	.size	test_trunc_f32, .Lfunc_end354-test_trunc_f32
	.size	.Ltest_trunc_f32$local, .Lfunc_end354-test_trunc_f32
                                        ! -- End function
	.globl	test_nearbyint_f32              ! -- Begin function test_nearbyint_f32
	.p2align	4
	.type	test_nearbyint_f32,@function
test_nearbyint_f32:                     ! @test_nearbyint_f32
.Ltest_nearbyint_f32$local:
	.type	.Ltest_nearbyint_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndss.dyn	%r1, %r1
	retf	0

.Lfunc_end355:
	.size	test_nearbyint_f32, .Lfunc_end355-test_nearbyint_f32
	.size	.Ltest_nearbyint_f32$local, .Lfunc_end355-test_nearbyint_f32
                                        ! -- End function
	.globl	test_rint_f32                   ! -- Begin function test_rint_f32
	.p2align	4
	.type	test_rint_f32,@function
test_rint_f32:                          ! @test_rint_f32
.Ltest_rint_f32$local:
	.type	.Ltest_rint_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndss.dyn	%r1, %r1
	retf	0

.Lfunc_end356:
	.size	test_rint_f32, .Lfunc_end356-test_rint_f32
	.size	.Ltest_rint_f32$local, .Lfunc_end356-test_rint_f32
                                        ! -- End function
	.globl	test_round_f64                  ! -- Begin function test_round_f64
	.p2align	4
	.type	test_round_f64,@function
test_round_f64:                         ! @test_round_f64
.Ltest_round_f64$local:
	.type	.Ltest_round_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsd.even	%r1, %r1
	retf	0

.Lfunc_end357:
	.size	test_round_f64, .Lfunc_end357-test_round_f64
	.size	.Ltest_round_f64$local, .Lfunc_end357-test_round_f64
                                        ! -- End function
	.globl	test_floor_f64                  ! -- Begin function test_floor_f64
	.p2align	4
	.type	test_floor_f64,@function
test_floor_f64:                         ! @test_floor_f64
.Ltest_floor_f64$local:
	.type	.Ltest_floor_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsd.floor	%r1, %r1
	retf	0

.Lfunc_end358:
	.size	test_floor_f64, .Lfunc_end358-test_floor_f64
	.size	.Ltest_floor_f64$local, .Lfunc_end358-test_floor_f64
                                        ! -- End function
	.globl	test_ceil_f64                   ! -- Begin function test_ceil_f64
	.p2align	4
	.type	test_ceil_f64,@function
test_ceil_f64:                          ! @test_ceil_f64
.Ltest_ceil_f64$local:
	.type	.Ltest_ceil_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsd.ceil	%r1, %r1
	retf	0

.Lfunc_end359:
	.size	test_ceil_f64, .Lfunc_end359-test_ceil_f64
	.size	.Ltest_ceil_f64$local, .Lfunc_end359-test_ceil_f64
                                        ! -- End function
	.globl	test_trunc_f64                  ! -- Begin function test_trunc_f64
	.p2align	4
	.type	test_trunc_f64,@function
test_trunc_f64:                         ! @test_trunc_f64
.Ltest_trunc_f64$local:
	.type	.Ltest_trunc_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsd.trunc	%r1, %r1
	retf	0

.Lfunc_end360:
	.size	test_trunc_f64, .Lfunc_end360-test_trunc_f64
	.size	.Ltest_trunc_f64$local, .Lfunc_end360-test_trunc_f64
                                        ! -- End function
	.globl	test_nearbyint_f64              ! -- Begin function test_nearbyint_f64
	.p2align	4
	.type	test_nearbyint_f64,@function
test_nearbyint_f64:                     ! @test_nearbyint_f64
.Ltest_nearbyint_f64$local:
	.type	.Ltest_nearbyint_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsd.dyn	%r1, %r1
	retf	0

.Lfunc_end361:
	.size	test_nearbyint_f64, .Lfunc_end361-test_nearbyint_f64
	.size	.Ltest_nearbyint_f64$local, .Lfunc_end361-test_nearbyint_f64
                                        ! -- End function
	.globl	test_rint_f64                   ! -- Begin function test_rint_f64
	.p2align	4
	.type	test_rint_f64,@function
test_rint_f64:                          ! @test_rint_f64
.Ltest_rint_f64$local:
	.type	.Ltest_rint_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsd.dyn	%r1, %r1
	retf	0

.Lfunc_end362:
	.size	test_rint_f64, .Lfunc_end362-test_rint_f64
	.size	.Ltest_rint_f64$local, .Lfunc_end362-test_rint_f64
                                        ! -- End function
	.globl	test_round_f128                 ! -- Begin function test_round_f128
	.p2align	4
	.type	test_round_f128,@function
test_round_f128:                        ! @test_round_f128
.Ltest_round_f128$local:
	.type	.Ltest_round_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsq.even	%r1, %r1
	retf	0

.Lfunc_end363:
	.size	test_round_f128, .Lfunc_end363-test_round_f128
	.size	.Ltest_round_f128$local, .Lfunc_end363-test_round_f128
                                        ! -- End function
	.globl	test_floor_f128                 ! -- Begin function test_floor_f128
	.p2align	4
	.type	test_floor_f128,@function
test_floor_f128:                        ! @test_floor_f128
.Ltest_floor_f128$local:
	.type	.Ltest_floor_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsq.floor	%r1, %r1
	retf	0

.Lfunc_end364:
	.size	test_floor_f128, .Lfunc_end364-test_floor_f128
	.size	.Ltest_floor_f128$local, .Lfunc_end364-test_floor_f128
                                        ! -- End function
	.globl	test_ceil_f128                  ! -- Begin function test_ceil_f128
	.p2align	4
	.type	test_ceil_f128,@function
test_ceil_f128:                         ! @test_ceil_f128
.Ltest_ceil_f128$local:
	.type	.Ltest_ceil_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsq.ceil	%r1, %r1
	retf	0

.Lfunc_end365:
	.size	test_ceil_f128, .Lfunc_end365-test_ceil_f128
	.size	.Ltest_ceil_f128$local, .Lfunc_end365-test_ceil_f128
                                        ! -- End function
	.globl	test_trunc_f128                 ! -- Begin function test_trunc_f128
	.p2align	4
	.type	test_trunc_f128,@function
test_trunc_f128:                        ! @test_trunc_f128
.Ltest_trunc_f128$local:
	.type	.Ltest_trunc_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsq.trunc	%r1, %r1
	retf	0

.Lfunc_end366:
	.size	test_trunc_f128, .Lfunc_end366-test_trunc_f128
	.size	.Ltest_trunc_f128$local, .Lfunc_end366-test_trunc_f128
                                        ! -- End function
	.globl	test_nearbyint_f128             ! -- Begin function test_nearbyint_f128
	.p2align	4
	.type	test_nearbyint_f128,@function
test_nearbyint_f128:                    ! @test_nearbyint_f128
.Ltest_nearbyint_f128$local:
	.type	.Ltest_nearbyint_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsq.dyn	%r1, %r1
	retf	0

.Lfunc_end367:
	.size	test_nearbyint_f128, .Lfunc_end367-test_nearbyint_f128
	.size	.Ltest_nearbyint_f128$local, .Lfunc_end367-test_nearbyint_f128
                                        ! -- End function
	.globl	test_rint_f128                  ! -- Begin function test_rint_f128
	.p2align	4
	.type	test_rint_f128,@function
test_rint_f128:                         ! @test_rint_f128
.Ltest_rint_f128$local:
	.type	.Ltest_rint_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	frndsq.dyn	%r1, %r1
	retf	0

.Lfunc_end368:
	.size	test_rint_f128, .Lfunc_end368-test_rint_f128
	.size	.Ltest_rint_f128$local, .Lfunc_end368-test_rint_f128
                                        ! -- End function
	.globl	test_fneg_f32                   ! -- Begin function test_fneg_f32
	.p2align	4
	.type	test_fneg_f32,@function
test_fneg_f32:                          ! @test_fneg_f32
.Ltest_fneg_f32$local:
	.type	.Ltest_fneg_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fnegss	%r1, %r1
	retf	0

.Lfunc_end369:
	.size	test_fneg_f32, .Lfunc_end369-test_fneg_f32
	.size	.Ltest_fneg_f32$local, .Lfunc_end369-test_fneg_f32
                                        ! -- End function
	.globl	test_fabs_f32                   ! -- Begin function test_fabs_f32
	.p2align	4
	.type	test_fabs_f32,@function
test_fabs_f32:                          ! @test_fabs_f32
.Ltest_fabs_f32$local:
	.type	.Ltest_fabs_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fabsss	%r1, %r1
	retf	0

.Lfunc_end370:
	.size	test_fabs_f32, .Lfunc_end370-test_fabs_f32
	.size	.Ltest_fabs_f32$local, .Lfunc_end370-test_fabs_f32
                                        ! -- End function
	.globl	test_fnabs_f32                  ! -- Begin function test_fnabs_f32
	.p2align	4
	.type	test_fnabs_f32,@function
test_fnabs_f32:                         ! @test_fnabs_f32
.Ltest_fnabs_f32$local:
	.type	.Ltest_fnabs_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fnabsss	%r1, %r1
	retf	0

.Lfunc_end371:
	.size	test_fnabs_f32, .Lfunc_end371-test_fnabs_f32
	.size	.Ltest_fnabs_f32$local, .Lfunc_end371-test_fnabs_f32
                                        ! -- End function
	.globl	test_fabsd_f32                  ! -- Begin function test_fabsd_f32
	.p2align	4
	.type	test_fabsd_f32,@function
test_fabsd_f32:                         ! @test_fabsd_f32
.Ltest_fabsd_f32$local:
	.type	.Ltest_fabsd_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fabsdss	%r1, %r1, %r2
	retf	0

.Lfunc_end372:
	.size	test_fabsd_f32, .Lfunc_end372-test_fabsd_f32
	.size	.Ltest_fabsd_f32$local, .Lfunc_end372-test_fabsd_f32
                                        ! -- End function
	.globl	test_fnabsd_f32                 ! -- Begin function test_fnabsd_f32
	.p2align	4
	.type	test_fnabsd_f32,@function
test_fnabsd_f32:                        ! @test_fnabsd_f32
.Ltest_fnabsd_f32$local:
	.type	.Ltest_fnabsd_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fnabsdss	%r1, %r1, %r2
	retf	0

.Lfunc_end373:
	.size	test_fnabsd_f32, .Lfunc_end373-test_fnabsd_f32
	.size	.Ltest_fnabsd_f32$local, .Lfunc_end373-test_fnabsd_f32
                                        ! -- End function
	.globl	test_fneg_f64                   ! -- Begin function test_fneg_f64
	.p2align	4
	.type	test_fneg_f64,@function
test_fneg_f64:                          ! @test_fneg_f64
.Ltest_fneg_f64$local:
	.type	.Ltest_fneg_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fnegsd	%r1, %r1
	retf	0

.Lfunc_end374:
	.size	test_fneg_f64, .Lfunc_end374-test_fneg_f64
	.size	.Ltest_fneg_f64$local, .Lfunc_end374-test_fneg_f64
                                        ! -- End function
	.globl	test_fabs_f64                   ! -- Begin function test_fabs_f64
	.p2align	4
	.type	test_fabs_f64,@function
test_fabs_f64:                          ! @test_fabs_f64
.Ltest_fabs_f64$local:
	.type	.Ltest_fabs_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fabssd	%r1, %r1
	retf	0

.Lfunc_end375:
	.size	test_fabs_f64, .Lfunc_end375-test_fabs_f64
	.size	.Ltest_fabs_f64$local, .Lfunc_end375-test_fabs_f64
                                        ! -- End function
	.globl	test_fnabs_f64                  ! -- Begin function test_fnabs_f64
	.p2align	4
	.type	test_fnabs_f64,@function
test_fnabs_f64:                         ! @test_fnabs_f64
.Ltest_fnabs_f64$local:
	.type	.Ltest_fnabs_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fnabssd	%r1, %r1
	retf	0

.Lfunc_end376:
	.size	test_fnabs_f64, .Lfunc_end376-test_fnabs_f64
	.size	.Ltest_fnabs_f64$local, .Lfunc_end376-test_fnabs_f64
                                        ! -- End function
	.globl	test_fabsd_f64                  ! -- Begin function test_fabsd_f64
	.p2align	4
	.type	test_fabsd_f64,@function
test_fabsd_f64:                         ! @test_fabsd_f64
.Ltest_fabsd_f64$local:
	.type	.Ltest_fabsd_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fabsdsd	%r1, %r1, %r2
	retf	0

.Lfunc_end377:
	.size	test_fabsd_f64, .Lfunc_end377-test_fabsd_f64
	.size	.Ltest_fabsd_f64$local, .Lfunc_end377-test_fabsd_f64
                                        ! -- End function
	.globl	test_fnabsd_f64                 ! -- Begin function test_fnabsd_f64
	.p2align	4
	.type	test_fnabsd_f64,@function
test_fnabsd_f64:                        ! @test_fnabsd_f64
.Ltest_fnabsd_f64$local:
	.type	.Ltest_fnabsd_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fnabsdsd	%r1, %r1, %r2
	retf	0

.Lfunc_end378:
	.size	test_fnabsd_f64, .Lfunc_end378-test_fnabsd_f64
	.size	.Ltest_fnabsd_f64$local, .Lfunc_end378-test_fnabsd_f64
                                        ! -- End function
	.globl	test_fneg_f128                  ! -- Begin function test_fneg_f128
	.p2align	4
	.type	test_fneg_f128,@function
test_fneg_f128:                         ! @test_fneg_f128
.Ltest_fneg_f128$local:
	.type	.Ltest_fneg_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fnegsq	%r1, %r1
	retf	0

.Lfunc_end379:
	.size	test_fneg_f128, .Lfunc_end379-test_fneg_f128
	.size	.Ltest_fneg_f128$local, .Lfunc_end379-test_fneg_f128
                                        ! -- End function
	.globl	test_fabs_f128                  ! -- Begin function test_fabs_f128
	.p2align	4
	.type	test_fabs_f128,@function
test_fabs_f128:                         ! @test_fabs_f128
.Ltest_fabs_f128$local:
	.type	.Ltest_fabs_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fabssq	%r1, %r1
	retf	0

.Lfunc_end380:
	.size	test_fabs_f128, .Lfunc_end380-test_fabs_f128
	.size	.Ltest_fabs_f128$local, .Lfunc_end380-test_fabs_f128
                                        ! -- End function
	.globl	test_fnabs_f128                 ! -- Begin function test_fnabs_f128
	.p2align	4
	.type	test_fnabs_f128,@function
test_fnabs_f128:                        ! @test_fnabs_f128
.Ltest_fnabs_f128$local:
	.type	.Ltest_fnabs_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	fnabssq	%r1, %r1
	retf	0

.Lfunc_end381:
	.size	test_fnabs_f128, .Lfunc_end381-test_fnabs_f128
	.size	.Ltest_fnabs_f128$local, .Lfunc_end381-test_fnabs_f128
                                        ! -- End function
	.globl	test_fabsd_f128                 ! -- Begin function test_fabsd_f128
	.p2align	4
	.type	test_fabsd_f128,@function
test_fabsd_f128:                        ! @test_fabsd_f128
.Ltest_fabsd_f128$local:
	.type	.Ltest_fabsd_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fabsdsq	%r1, %r1, %r2
	retf	0

.Lfunc_end382:
	.size	test_fabsd_f128, .Lfunc_end382-test_fabsd_f128
	.size	.Ltest_fabsd_f128$local, .Lfunc_end382-test_fabsd_f128
                                        ! -- End function
	.globl	test_fnabsd_f128                ! -- Begin function test_fnabsd_f128
	.p2align	4
	.type	test_fnabsd_f128,@function
test_fnabsd_f128:                       ! @test_fnabsd_f128
.Ltest_fnabsd_f128$local:
	.type	.Ltest_fnabsd_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	fnabsdsq	%r1, %r1, %r2
	retf	0

.Lfunc_end383:
	.size	test_fnabsd_f128, .Lfunc_end383-test_fnabsd_f128
	.size	.Ltest_fnabsd_f128$local, .Lfunc_end383-test_fnabsd_f128
                                        ! -- End function
	.section	.rodata.cst16,"aM",@progbits,16
	.p2align	4, 0x0                          ! -- Begin function fp_store_zero_dummy_return
.LCPI384_0:
	.xword	0x0000000000000000              ! fp128 1
	.xword	0x3fff000000000000
	.text
	.globl	fp_store_zero_dummy_return
	.p2align	4
	.type	fp_store_zero_dummy_return,@function
fp_store_zero_dummy_return:             ! @fp_store_zero_dummy_return
.Lfp_store_zero_dummy_return$local:
	.type	.Lfp_store_zero_dummy_return$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	7
	ldhzr	%r1, %pcfwd(f16_zero)
	ldi	%r2, 15360

	! BUNDLE
	faddsh	%r1, %r1, %r2
	sthr	%r1, %pcfwd(f16_zero)
	ldwzr	%r2, %pcfwd(f32_zero)

	! BUNDLE
	ldi.l	%r3, 1065353216
	lddzr	%r4, %pcfwd(f64_zero)

	! BUNDLE
	ldqr	%r5, %pcfwd(f128_zero)
	ldqr	%r6, %pcfwd(.LCPI384_0)
	faddss	%r2, %r2, %r3

	! BUNDLE
	stwr	%r2, %pcfwd(f32_zero)
	ldi.l	%r3, 4607182418800017408

	! BUNDLE
	faddsd	%r3, %r4, %r3
	faddsq	%r4, %r5, %r6
	fextsh2ss	%r1, %r1

	! BUNDLE
	faddss	%r1, %r2, %r1
	fextss2sd	%r1, %r1
	faddsd	%r1, %r3, %r1

	! BUNDLE
	fextsd2sq	%r1, %r1
	faddsq	%r1, %r4, %r1
	stdr	%r3, %pcfwd(f64_zero)

	! BUNDLE
	fcvtsq2sd	%r1, %r1, 0
	stqr	%r4, %pcfwd(f128_zero)
	retf	0

.Lfunc_end384:
	.size	fp_store_zero_dummy_return, .Lfunc_end384-fp_store_zero_dummy_return
	.size	.Lfp_store_zero_dummy_return$local, .Lfunc_end384-fp_store_zero_dummy_return
                                        ! -- End function
	.globl	test_store_based_f16_zero       ! -- Begin function test_store_based_f16_zero
	.p2align	4
	.type	test_store_based_f16_zero,@function
test_store_based_f16_zero:              ! @test_store_based_f16_zero
.Ltest_store_based_f16_zero$local:
	.type	.Ltest_store_based_f16_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	sth	%gz, %r1, 8000
	sthr	%gz, %pcfwd(f16_zero)

	! BUNDLE
	sthx	%gz, %r1, %r2, 1, 20
	sthx.ws	%gz, %r1, %r3, 1, 20
	retf	0

.Lfunc_end385:
	.size	test_store_based_f16_zero, .Lfunc_end385-test_store_based_f16_zero
	.size	.Ltest_store_based_f16_zero$local, .Lfunc_end385-test_store_based_f16_zero
                                        ! -- End function
	.globl	test_store_based_f32_zero       ! -- Begin function test_store_based_f32_zero
	.p2align	4
	.type	test_store_based_f32_zero,@function
test_store_based_f32_zero:              ! @test_store_based_f32_zero
.Ltest_store_based_f32_zero$local:
	.type	.Ltest_store_based_f32_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stw	%gz, %r1, 16000
	stwr	%gz, %pcfwd(f32_zero)

	! BUNDLE
	stwx	%gz, %r1, %r2, 2, 40
	stwx.ws	%gz, %r1, %r3, 2, 40
	retf	0

.Lfunc_end386:
	.size	test_store_based_f32_zero, .Lfunc_end386-test_store_based_f32_zero
	.size	.Ltest_store_based_f32_zero$local, .Lfunc_end386-test_store_based_f32_zero
                                        ! -- End function
	.globl	test_store_based_f64_zero       ! -- Begin function test_store_based_f64_zero
	.p2align	4
	.type	test_store_based_f64_zero,@function
test_store_based_f64_zero:              ! @test_store_based_f64_zero
.Ltest_store_based_f64_zero$local:
	.type	.Ltest_store_based_f64_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	std	%gz, %r1, 32000
	stdr	%gz, %pcfwd(f64_zero)

	! BUNDLE
	stdx	%gz, %r1, %r2, 3, 80
	stdx.ws	%gz, %r1, %r3, 3, 80
	retf	0

.Lfunc_end387:
	.size	test_store_based_f64_zero, .Lfunc_end387-test_store_based_f64_zero
	.size	.Ltest_store_based_f64_zero$local, .Lfunc_end387-test_store_based_f64_zero
                                        ! -- End function
	.globl	test_store_based_f128_zero      ! -- Begin function test_store_based_f128_zero
	.p2align	4
	.type	test_store_based_f128_zero,@function
test_store_based_f128_zero:             ! @test_store_based_f128_zero
.Ltest_store_based_f128_zero$local:
	.type	.Ltest_store_based_f128_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stq	%gz, %r1, 64000
	stqr	%gz, %pcfwd(f128_zero)

	! BUNDLE
	stqx	%gz, %r1, %r2, 4, 160
	stqx.ws	%gz, %r1, %r3, 4, 160
	retf	0

.Lfunc_end388:
	.size	test_store_based_f128_zero, .Lfunc_end388-test_store_based_f128_zero
	.size	.Ltest_store_based_f128_zero$local, .Lfunc_end388-test_store_based_f128_zero
                                        ! -- End function
	.globl	test_function_pointer           ! -- Begin function test_function_pointer
	.p2align	4
	.type	test_function_pointer,@function
test_function_pointer:                  ! @test_function_pointer
.Ltest_function_pointer$local:
	.type	.Ltest_function_pointer$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	mov	%r4, %r2
	callri	%r3, %r1, %gz

	! BUNDLE
	addw	%r1, %r4, %r2
	retf	0
	nop	0

.Lfunc_end389:
	.size	test_function_pointer, .Lfunc_end389-test_function_pointer
	.size	.Ltest_function_pointer$local, .Lfunc_end389-test_function_pointer
                                        ! -- End function
	.globl	test_function_call              ! -- Begin function test_function_call
	.p2align	4
	.type	test_function_call,@function
test_function_call:                     ! @test_function_call
.Ltest_function_call$local:
	.type	.Ltest_function_call$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	mov	%r4, %r2
	callr	%r3, %jpc(ccc)

	! BUNDLE
	addw	%r1, %r4, %r2
	retf	0
	nop	0

.Lfunc_end390:
	.size	test_function_call, .Lfunc_end390-test_function_call
	.size	.Ltest_function_call$local, .Lfunc_end390-test_function_call
                                        ! -- End function
	.globl	test_function                   ! -- Begin function test_function
	.p2align	4
	.type	test_function,@function
test_function:                          ! @test_function
.Ltest_function$local:
	.type	.Ltest_function$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	mov	%r4, %r1
	callr	%r3, %jpc(xprintf)

	! BUNDLE
	addw	%r1, %r4, %r2
	retf	0
	nop	0

.Lfunc_end391:
	.size	test_function, .Lfunc_end391-test_function
	.size	.Ltest_function$local, .Lfunc_end391-test_function
                                        ! -- End function
	.globl	test_callmi                     ! -- Begin function test_callmi
	.p2align	4
	.type	test_callmi,@function
test_callmi:                            ! @test_callmi
.Ltest_callmi$local:
	.type	.Ltest_callmi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	ldax	%r2, %r2, %r1, 3, 0
	callmi	%r3, %r2, 0

	! BUNDLE
	addw	%r1, %r4, %r1
	retf	0
	nop	0

.Lfunc_end392:
	.size	test_callmi, .Lfunc_end392-test_callmi
	.size	.Ltest_callmi$local, .Lfunc_end392-test_callmi
                                        ! -- End function
	.globl	test_callmi_folding             ! -- Begin function test_callmi_folding
	.p2align	4
	.type	test_callmi_folding,@function
test_callmi_folding:                    ! @test_callmi_folding
.Ltest_callmi_folding$local:
	.type	.Ltest_callmi_folding$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	mov	%r4, %r2
	callmi	%r3, %r1, 48

	! BUNDLE
	addiws	%r1, %r4, 2
	retf	0
	nop	0

.Lfunc_end393:
	.size	test_callmi_folding, .Lfunc_end393-test_callmi_folding
	.size	.Ltest_callmi_folding$local, .Lfunc_end393-test_callmi_folding
                                        ! -- End function
	.globl	test_callplt_folding            ! -- Begin function test_callplt_folding
	.p2align	4
	.type	test_callplt_folding,@function
test_callplt_folding:                   ! @test_callplt_folding
.Ltest_callplt_folding$local:
	.type	.Ltest_callplt_folding$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	mov	%r3, %r1
	callplt	%r2, %pcfwd(gftable+40)

	! BUNDLE
	addiws	%r1, %r3, 2
	retf	0
	nop	0

.Lfunc_end394:
	.size	test_callplt_folding, .Lfunc_end394-test_callplt_folding
	.size	.Ltest_callplt_folding$local, .Lfunc_end394-test_callplt_folding
                                        ! -- End function
	.globl	test_global_function_pointer_target ! -- Begin function test_global_function_pointer_target
	.p2align	4
	.type	test_global_function_pointer_target,@function
test_global_function_pointer_target:    ! @test_global_function_pointer_target
.Ltest_global_function_pointer_target$local:
	.type	.Ltest_global_function_pointer_target$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	addiws	%r1, %r1, 1
	retf	0

.Lfunc_end395:
	.size	test_global_function_pointer_target, .Lfunc_end395-test_global_function_pointer_target
	.size	.Ltest_global_function_pointer_target$local, .Lfunc_end395-test_global_function_pointer_target
                                        ! -- End function
	.globl	test_static_function_pointer_materialize ! -- Begin function test_static_function_pointer_materialize
	.p2align	4
	.type	test_static_function_pointer_materialize,@function
test_static_function_pointer_materialize: ! @test_static_function_pointer_materialize
.Ltest_static_function_pointer_materialize$local:
	.type	.Ltest_static_function_pointer_materialize$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldar	%r1, %jpc(test_static_function_pointer_target)
	retf	0

.Lfunc_end396:
	.size	test_static_function_pointer_materialize, .Lfunc_end396-test_static_function_pointer_materialize
	.size	.Ltest_static_function_pointer_materialize$local, .Lfunc_end396-test_static_function_pointer_materialize
                                        ! -- End function
	.p2align	4                               ! -- Begin function test_static_function_pointer_target
	.type	test_static_function_pointer_target,@function
test_static_function_pointer_target:    ! @test_static_function_pointer_target
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	addiws	%r1, %r1, 1
	retf	0

.Lfunc_end397:
	.size	test_static_function_pointer_target, .Lfunc_end397-test_static_function_pointer_target
                                        ! -- End function
	.globl	test_global_function_pointer_materialize ! -- Begin function test_global_function_pointer_materialize
	.p2align	4
	.type	test_global_function_pointer_materialize,@function
test_global_function_pointer_materialize: ! @test_global_function_pointer_materialize
.Ltest_global_function_pointer_materialize$local:
	.type	.Ltest_global_function_pointer_materialize$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldar	%r1, %jpc(test_global_function_pointer_target)
	retf	0

.Lfunc_end398:
	.size	test_global_function_pointer_materialize, .Lfunc_end398-test_global_function_pointer_materialize
	.size	.Ltest_global_function_pointer_materialize$local, .Lfunc_end398-test_global_function_pointer_materialize
                                        ! -- End function
	.globl	test_extern_function_pointer_materialize ! -- Begin function test_extern_function_pointer_materialize
	.p2align	4
	.type	test_extern_function_pointer_materialize,@function
test_extern_function_pointer_materialize: ! @test_extern_function_pointer_materialize
.Ltest_extern_function_pointer_materialize$local:
	.type	.Ltest_extern_function_pointer_materialize$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldar	%r1, %jpc(test_extern_function_pointer_target)
	retf	0

.Lfunc_end399:
	.size	test_extern_function_pointer_materialize, .Lfunc_end399-test_extern_function_pointer_materialize
	.size	.Ltest_extern_function_pointer_materialize$local, .Lfunc_end399-test_extern_function_pointer_materialize
                                        ! -- End function
	.globl	test_global_data                ! -- Begin function test_global_data
	.p2align	4
	.type	test_global_data,@function
test_global_data:                       ! @test_global_data
.Ltest_global_data$local:
	.type	.Ltest_global_data$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	lddzr	%r2, %pcfwd(x_bss)
	addi	%r2, %r2, 1

	! BUNDLE
	stdr	%r2, %pcfwd(x_bss)
	lddzr	%r2, %pcfwd(x_bss_extern)
	addi	%r2, %r2, 1

	! BUNDLE
	stdr	%r2, %pcfwd(x_bss_extern)
	retf	0
	nop	0

.Lfunc_end400:
	.size	test_global_data, .Lfunc_end400-test_global_data
	.size	.Ltest_global_data$local, .Lfunc_end400-test_global_data
                                        ! -- End function
	.globl	test_global_i8_ptr              ! -- Begin function test_global_i8_ptr
	.p2align	4
	.type	test_global_i8_ptr,@function
test_global_i8_ptr:                     ! @test_global_i8_ptr
.Ltest_global_i8_ptr$local:
	.type	.Ltest_global_i8_ptr$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldafr	%r1, %pcfwd(global_i8)
	retf	0

.Lfunc_end401:
	.size	test_global_i8_ptr, .Lfunc_end401-test_global_i8_ptr
	.size	.Ltest_global_i8_ptr$local, .Lfunc_end401-test_global_i8_ptr
                                        ! -- End function
	.globl	test_global_i8                  ! -- Begin function test_global_i8
	.p2align	4
	.type	test_global_i8,@function
test_global_i8:                         ! @test_global_i8
.Ltest_global_i8$local:
	.type	.Ltest_global_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbsr	%r1, %pcfwd(global_i8)
	retf	0

.Lfunc_end402:
	.size	test_global_i8, .Lfunc_end402-test_global_i8
	.size	.Ltest_global_i8$local, .Lfunc_end402-test_global_i8
                                        ! -- End function
	.globl	test_global_u8                  ! -- Begin function test_global_u8
	.p2align	4
	.type	test_global_u8,@function
test_global_u8:                         ! @test_global_u8
.Ltest_global_u8$local:
	.type	.Ltest_global_u8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbzr	%r1, %pcfwd(global_u8)
	retf	0

.Lfunc_end403:
	.size	test_global_u8, .Lfunc_end403-test_global_u8
	.size	.Ltest_global_u8$local, .Lfunc_end403-test_global_u8
                                        ! -- End function
	.globl	test_global_i16                 ! -- Begin function test_global_i16
	.p2align	4
	.type	test_global_i16,@function
test_global_i16:                        ! @test_global_i16
.Ltest_global_i16$local:
	.type	.Ltest_global_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhsr	%r1, %pcfwd(global_i16)
	retf	0

.Lfunc_end404:
	.size	test_global_i16, .Lfunc_end404-test_global_i16
	.size	.Ltest_global_i16$local, .Lfunc_end404-test_global_i16
                                        ! -- End function
	.globl	test_global_u16                 ! -- Begin function test_global_u16
	.p2align	4
	.type	test_global_u16,@function
test_global_u16:                        ! @test_global_u16
.Ltest_global_u16$local:
	.type	.Ltest_global_u16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhzr	%r1, %pcfwd(global_u16)
	retf	0

.Lfunc_end405:
	.size	test_global_u16, .Lfunc_end405-test_global_u16
	.size	.Ltest_global_u16$local, .Lfunc_end405-test_global_u16
                                        ! -- End function
	.globl	test_global_i8_to_i16           ! -- Begin function test_global_i8_to_i16
	.p2align	4
	.type	test_global_i8_to_i16,@function
test_global_i8_to_i16:                  ! @test_global_i8_to_i16
.Ltest_global_i8_to_i16$local:
	.type	.Ltest_global_i8_to_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbsr	%r1, %pcfwd(global_i8)
	retf	0

.Lfunc_end406:
	.size	test_global_i8_to_i16, .Lfunc_end406-test_global_i8_to_i16
	.size	.Ltest_global_i8_to_i16$local, .Lfunc_end406-test_global_i8_to_i16
                                        ! -- End function
	.globl	test_global_u8_to_u16           ! -- Begin function test_global_u8_to_u16
	.p2align	4
	.type	test_global_u8_to_u16,@function
test_global_u8_to_u16:                  ! @test_global_u8_to_u16
.Ltest_global_u8_to_u16$local:
	.type	.Ltest_global_u8_to_u16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbzr	%r1, %pcfwd(global_u8)
	retf	0

.Lfunc_end407:
	.size	test_global_u8_to_u16, .Lfunc_end407-test_global_u8_to_u16
	.size	.Ltest_global_u8_to_u16$local, .Lfunc_end407-test_global_u8_to_u16
                                        ! -- End function
	.globl	test_global_i32                 ! -- Begin function test_global_i32
	.p2align	4
	.type	test_global_i32,@function
test_global_i32:                        ! @test_global_i32
.Ltest_global_i32$local:
	.type	.Ltest_global_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldwzr	%r1, %pcfwd(global_i32)
	retf	0

.Lfunc_end408:
	.size	test_global_i32, .Lfunc_end408-test_global_i32
	.size	.Ltest_global_i32$local, .Lfunc_end408-test_global_i32
                                        ! -- End function
	.globl	test_global_u32                 ! -- Begin function test_global_u32
	.p2align	4
	.type	test_global_u32,@function
test_global_u32:                        ! @test_global_u32
.Ltest_global_u32$local:
	.type	.Ltest_global_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldwzr	%r1, %pcfwd(global_u32)
	retf	0

.Lfunc_end409:
	.size	test_global_u32, .Lfunc_end409-test_global_u32
	.size	.Ltest_global_u32$local, .Lfunc_end409-test_global_u32
                                        ! -- End function
	.globl	test_global_i8_to_i32           ! -- Begin function test_global_i8_to_i32
	.p2align	4
	.type	test_global_i8_to_i32,@function
test_global_i8_to_i32:                  ! @test_global_i8_to_i32
.Ltest_global_i8_to_i32$local:
	.type	.Ltest_global_i8_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbsr	%r1, %pcfwd(global_i8)
	retf	0

.Lfunc_end410:
	.size	test_global_i8_to_i32, .Lfunc_end410-test_global_i8_to_i32
	.size	.Ltest_global_i8_to_i32$local, .Lfunc_end410-test_global_i8_to_i32
                                        ! -- End function
	.globl	test_global_u8_to_u32           ! -- Begin function test_global_u8_to_u32
	.p2align	4
	.type	test_global_u8_to_u32,@function
test_global_u8_to_u32:                  ! @test_global_u8_to_u32
.Ltest_global_u8_to_u32$local:
	.type	.Ltest_global_u8_to_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbzr	%r1, %pcfwd(global_u8)
	retf	0

.Lfunc_end411:
	.size	test_global_u8_to_u32, .Lfunc_end411-test_global_u8_to_u32
	.size	.Ltest_global_u8_to_u32$local, .Lfunc_end411-test_global_u8_to_u32
                                        ! -- End function
	.globl	test_global_i16_to_i32          ! -- Begin function test_global_i16_to_i32
	.p2align	4
	.type	test_global_i16_to_i32,@function
test_global_i16_to_i32:                 ! @test_global_i16_to_i32
.Ltest_global_i16_to_i32$local:
	.type	.Ltest_global_i16_to_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhsr	%r1, %pcfwd(global_i16)
	retf	0

.Lfunc_end412:
	.size	test_global_i16_to_i32, .Lfunc_end412-test_global_i16_to_i32
	.size	.Ltest_global_i16_to_i32$local, .Lfunc_end412-test_global_i16_to_i32
                                        ! -- End function
	.globl	test_global_u16_to_u32          ! -- Begin function test_global_u16_to_u32
	.p2align	4
	.type	test_global_u16_to_u32,@function
test_global_u16_to_u32:                 ! @test_global_u16_to_u32
.Ltest_global_u16_to_u32$local:
	.type	.Ltest_global_u16_to_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhzr	%r1, %pcfwd(global_u16)
	retf	0

.Lfunc_end413:
	.size	test_global_u16_to_u32, .Lfunc_end413-test_global_u16_to_u32
	.size	.Ltest_global_u16_to_u32$local, .Lfunc_end413-test_global_u16_to_u32
                                        ! -- End function
	.globl	test_global_i8_to_i64           ! -- Begin function test_global_i8_to_i64
	.p2align	4
	.type	test_global_i8_to_i64,@function
test_global_i8_to_i64:                  ! @test_global_i8_to_i64
.Ltest_global_i8_to_i64$local:
	.type	.Ltest_global_i8_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbsr	%r1, %pcfwd(global_i8)
	retf	0

.Lfunc_end414:
	.size	test_global_i8_to_i64, .Lfunc_end414-test_global_i8_to_i64
	.size	.Ltest_global_i8_to_i64$local, .Lfunc_end414-test_global_i8_to_i64
                                        ! -- End function
	.globl	test_global_u8_to_u64           ! -- Begin function test_global_u8_to_u64
	.p2align	4
	.type	test_global_u8_to_u64,@function
test_global_u8_to_u64:                  ! @test_global_u8_to_u64
.Ltest_global_u8_to_u64$local:
	.type	.Ltest_global_u8_to_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbzr	%r1, %pcfwd(global_u8)
	retf	0

.Lfunc_end415:
	.size	test_global_u8_to_u64, .Lfunc_end415-test_global_u8_to_u64
	.size	.Ltest_global_u8_to_u64$local, .Lfunc_end415-test_global_u8_to_u64
                                        ! -- End function
	.globl	test_global_i16_to_i64          ! -- Begin function test_global_i16_to_i64
	.p2align	4
	.type	test_global_i16_to_i64,@function
test_global_i16_to_i64:                 ! @test_global_i16_to_i64
.Ltest_global_i16_to_i64$local:
	.type	.Ltest_global_i16_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhsr	%r1, %pcfwd(global_i16)
	retf	0

.Lfunc_end416:
	.size	test_global_i16_to_i64, .Lfunc_end416-test_global_i16_to_i64
	.size	.Ltest_global_i16_to_i64$local, .Lfunc_end416-test_global_i16_to_i64
                                        ! -- End function
	.globl	test_global_u16_to_u64          ! -- Begin function test_global_u16_to_u64
	.p2align	4
	.type	test_global_u16_to_u64,@function
test_global_u16_to_u64:                 ! @test_global_u16_to_u64
.Ltest_global_u16_to_u64$local:
	.type	.Ltest_global_u16_to_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhzr	%r1, %pcfwd(global_u16)
	retf	0

.Lfunc_end417:
	.size	test_global_u16_to_u64, .Lfunc_end417-test_global_u16_to_u64
	.size	.Ltest_global_u16_to_u64$local, .Lfunc_end417-test_global_u16_to_u64
                                        ! -- End function
	.globl	test_global_i32_to_i64          ! -- Begin function test_global_i32_to_i64
	.p2align	4
	.type	test_global_i32_to_i64,@function
test_global_i32_to_i64:                 ! @test_global_i32_to_i64
.Ltest_global_i32_to_i64$local:
	.type	.Ltest_global_i32_to_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldwsr	%r1, %pcfwd(global_i32)
	retf	0

.Lfunc_end418:
	.size	test_global_i32_to_i64, .Lfunc_end418-test_global_i32_to_i64
	.size	.Ltest_global_i32_to_i64$local, .Lfunc_end418-test_global_i32_to_i64
                                        ! -- End function
	.globl	test_global_u32_to_u64          ! -- Begin function test_global_u32_to_u64
	.p2align	4
	.type	test_global_u32_to_u64,@function
test_global_u32_to_u64:                 ! @test_global_u32_to_u64
.Ltest_global_u32_to_u64$local:
	.type	.Ltest_global_u32_to_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldwzr	%r1, %pcfwd(global_u32)
	retf	0

.Lfunc_end419:
	.size	test_global_u32_to_u64, .Lfunc_end419-test_global_u32_to_u64
	.size	.Ltest_global_u32_to_u64$local, .Lfunc_end419-test_global_u32_to_u64
                                        ! -- End function
	.globl	test_global_i64                 ! -- Begin function test_global_i64
	.p2align	4
	.type	test_global_i64,@function
test_global_i64:                        ! @test_global_i64
.Ltest_global_i64$local:
	.type	.Ltest_global_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	lddzr	%r1, %pcfwd(global_i64)
	retf	0

.Lfunc_end420:
	.size	test_global_i64, .Lfunc_end420-test_global_i64
	.size	.Ltest_global_i64$local, .Lfunc_end420-test_global_i64
                                        ! -- End function
	.globl	test_global_u64                 ! -- Begin function test_global_u64
	.p2align	4
	.type	test_global_u64,@function
test_global_u64:                        ! @test_global_u64
.Ltest_global_u64$local:
	.type	.Ltest_global_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	lddzr	%r1, %pcfwd(global_u64)
	retf	0

.Lfunc_end421:
	.size	test_global_u64, .Lfunc_end421-test_global_u64
	.size	.Ltest_global_u64$local, .Lfunc_end421-test_global_u64
                                        ! -- End function
	.globl	test_global_array_folding_i8    ! -- Begin function test_global_array_folding_i8
	.p2align	4
	.type	test_global_array_folding_i8,@function
test_global_array_folding_i8:           ! @test_global_array_folding_i8
.Ltest_global_array_folding_i8$local:
	.type	.Ltest_global_array_folding_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbsr	%r1, %pcfwd(global_array_i8+5)
	retf	0

.Lfunc_end422:
	.size	test_global_array_folding_i8, .Lfunc_end422-test_global_array_folding_i8
	.size	.Ltest_global_array_folding_i8$local, .Lfunc_end422-test_global_array_folding_i8
                                        ! -- End function
	.globl	test_global_array_folding_i16   ! -- Begin function test_global_array_folding_i16
	.p2align	4
	.type	test_global_array_folding_i16,@function
test_global_array_folding_i16:          ! @test_global_array_folding_i16
.Ltest_global_array_folding_i16$local:
	.type	.Ltest_global_array_folding_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhsr	%r1, %pcfwd(global_array_i16+10)
	retf	0

.Lfunc_end423:
	.size	test_global_array_folding_i16, .Lfunc_end423-test_global_array_folding_i16
	.size	.Ltest_global_array_folding_i16$local, .Lfunc_end423-test_global_array_folding_i16
                                        ! -- End function
	.globl	test_global_array_folding_i32   ! -- Begin function test_global_array_folding_i32
	.p2align	4
	.type	test_global_array_folding_i32,@function
test_global_array_folding_i32:          ! @test_global_array_folding_i32
.Ltest_global_array_folding_i32$local:
	.type	.Ltest_global_array_folding_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldwzr	%r1, %pcfwd(global_array_i32+20)
	retf	0

.Lfunc_end424:
	.size	test_global_array_folding_i32, .Lfunc_end424-test_global_array_folding_i32
	.size	.Ltest_global_array_folding_i32$local, .Lfunc_end424-test_global_array_folding_i32
                                        ! -- End function
	.globl	test_global_array_folding_i64   ! -- Begin function test_global_array_folding_i64
	.p2align	4
	.type	test_global_array_folding_i64,@function
test_global_array_folding_i64:          ! @test_global_array_folding_i64
.Ltest_global_array_folding_i64$local:
	.type	.Ltest_global_array_folding_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	lddzr	%r1, %pcfwd(global_array_i64+40)
	retf	0

.Lfunc_end425:
	.size	test_global_array_folding_i64, .Lfunc_end425-test_global_array_folding_i64
	.size	.Ltest_global_array_folding_i64$local, .Lfunc_end425-test_global_array_folding_i64
                                        ! -- End function
	.globl	test_ldi128                     ! -- Begin function test_ldi128
	.p2align	4
	.type	test_ldi128,@function
test_ldi128:                            ! @test_ldi128
.Ltest_ldi128$local:
	.type	.Ltest_ldi128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 12345778
	retf	0

.Lfunc_end426:
	.size	test_ldi128, .Lfunc_end426-test_ldi128
	.size	.Ltest_ldi128$local, .Lfunc_end426-test_ldi128
                                        ! -- End function
	.globl	test_ldi128_long                ! -- Begin function test_ldi128_long
	.p2align	4
	.type	test_ldi128_long,@function
test_ldi128_long:                       ! @test_ldi128_long
.Ltest_ldi128_long$local:
	.type	.Ltest_ldi128_long$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, 987654321123456789

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end427:
	.size	test_ldi128_long, .Lfunc_end427-test_ldi128_long
	.size	.Ltest_ldi128_long$local, .Lfunc_end427-test_ldi128_long
                                        ! -- End function
	.globl	test_subfi_i128                 ! -- Begin function test_subfi_i128
	.p2align	4
	.type	test_subfi_i128,@function
test_subfi_i128:                        ! @test_subfi_i128
.Ltest_subfi_i128$local:
	.type	.Ltest_subfi_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldi	%r2, 1234
	subq	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end428:
	.size	test_subfi_i128, .Lfunc_end428-test_subfi_i128
	.size	.Ltest_subfi_i128$local, .Lfunc_end428-test_subfi_i128
                                        ! -- End function
	.globl	test_addi_i128                  ! -- Begin function test_addi_i128
	.p2align	4
	.type	test_addi_i128,@function
test_addi_i128:                         ! @test_addi_i128
.Ltest_addi_i128$local:
	.type	.Ltest_addi_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldi.l	%r2, 123456789123456789

	! BUNDLE
	addq	%r1, %r1, %r2
	ldi.l	%r2, 9200848539817279407

	! BUNDLE
	or	%r1, %r1, %r2
	retf	0
	nop	0

.Lfunc_end429:
	.size	test_addi_i128, .Lfunc_end429-test_addi_i128
	.size	.Ltest_addi_i128$local, .Lfunc_end429-test_addi_i128
                                        ! -- End function
	.globl	test_andi_i128                  ! -- Begin function test_andi_i128
	.p2align	4
	.type	test_andi_i128,@function
test_andi_i128:                         ! @test_andi_i128
.Ltest_andi_i128$local:
	.type	.Ltest_andi_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldi.l	%r2, 123456789123456789

	! BUNDLE
	and	%r1, %r1, %r2
	retf	0
	nop	0

.Lfunc_end430:
	.size	test_andi_i128, .Lfunc_end430-test_andi_i128
	.size	.Ltest_andi_i128$local, .Lfunc_end430-test_andi_i128
                                        ! -- End function
	.globl	test_ori_i128                   ! -- Begin function test_ori_i128
	.p2align	4
	.type	test_ori_i128,@function
test_ori_i128:                          ! @test_ori_i128
.Ltest_ori_i128$local:
	.type	.Ltest_ori_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ldi.l	%r2, 987654321123456789

	! BUNDLE
	or	%r1, %r1, %r2
	retf	0
	nop	0

.Lfunc_end431:
	.size	test_ori_i128, .Lfunc_end431-test_ori_i128
	.size	.Ltest_ori_i128$local, .Lfunc_end431-test_ori_i128
                                        ! -- End function
	.globl	test_add_i128                   ! -- Begin function test_add_i128
	.p2align	4
	.type	test_add_i128,@function
test_add_i128:                          ! @test_add_i128
.Ltest_add_i128$local:
	.type	.Ltest_add_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	addq	%r1, %r2, %r1
	retf	0

.Lfunc_end432:
	.size	test_add_i128, .Lfunc_end432-test_add_i128
	.size	.Ltest_add_i128$local, .Lfunc_end432-test_add_i128
                                        ! -- End function
	.globl	test_sub_i128                   ! -- Begin function test_sub_i128
	.p2align	4
	.type	test_sub_i128,@function
test_sub_i128:                          ! @test_sub_i128
.Ltest_sub_i128$local:
	.type	.Ltest_sub_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	subq	%r1, %r1, %r2
	retf	0

.Lfunc_end433:
	.size	test_sub_i128, .Lfunc_end433-test_sub_i128
	.size	.Ltest_sub_i128$local, .Lfunc_end433-test_sub_i128
                                        ! -- End function
	.globl	test_immediate_i32              ! -- Begin function test_immediate_i32
	.p2align	4
	.type	test_immediate_i32,@function
test_immediate_i32:                     ! @test_immediate_i32
.Ltest_immediate_i32$local:
	.type	.Ltest_immediate_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, -123456789
	retf	0

.Lfunc_end434:
	.size	test_immediate_i32, .Lfunc_end434-test_immediate_i32
	.size	.Ltest_immediate_i32$local, .Lfunc_end434-test_immediate_i32
                                        ! -- End function
	.globl	test_immediate_u32              ! -- Begin function test_immediate_u32
	.p2align	4
	.type	test_immediate_u32,@function
test_immediate_u32:                     ! @test_immediate_u32
.Ltest_immediate_u32$local:
	.type	.Ltest_immediate_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 123456789
	retf	0

.Lfunc_end435:
	.size	test_immediate_u32, .Lfunc_end435-test_immediate_u32
	.size	.Ltest_immediate_u32$local, .Lfunc_end435-test_immediate_u32
                                        ! -- End function
	.globl	test_immediate_i32_long         ! -- Begin function test_immediate_i32_long
	.p2align	4
	.type	test_immediate_i32_long,@function
test_immediate_i32_long:                ! @test_immediate_i32_long
.Ltest_immediate_i32_long$local:
	.type	.Ltest_immediate_i32_long$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, -1234567890

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end436:
	.size	test_immediate_i32_long, .Lfunc_end436-test_immediate_i32_long
	.size	.Ltest_immediate_i32_long$local, .Lfunc_end436-test_immediate_i32_long
                                        ! -- End function
	.globl	test_immediate_u32_long         ! -- Begin function test_immediate_u32_long
	.p2align	4
	.type	test_immediate_u32_long,@function
test_immediate_u32_long:                ! @test_immediate_u32_long
.Ltest_immediate_u32_long$local:
	.type	.Ltest_immediate_u32_long$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, 1234567890

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end437:
	.size	test_immediate_u32_long, .Lfunc_end437-test_immediate_u32_long
	.size	.Ltest_immediate_u32_long$local, .Lfunc_end437-test_immediate_u32_long
                                        ! -- End function
	.globl	test_immediate_i64              ! -- Begin function test_immediate_i64
	.p2align	4
	.type	test_immediate_i64,@function
test_immediate_i64:                     ! @test_immediate_i64
.Ltest_immediate_i64$local:
	.type	.Ltest_immediate_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, -12345678
	retf	0

.Lfunc_end438:
	.size	test_immediate_i64, .Lfunc_end438-test_immediate_i64
	.size	.Ltest_immediate_i64$local, .Lfunc_end438-test_immediate_i64
                                        ! -- End function
	.globl	test_immediate_u64              ! -- Begin function test_immediate_u64
	.p2align	4
	.type	test_immediate_u64,@function
test_immediate_u64:                     ! @test_immediate_u64
.Ltest_immediate_u64$local:
	.type	.Ltest_immediate_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 12345678
	retf	0

.Lfunc_end439:
	.size	test_immediate_u64, .Lfunc_end439-test_immediate_u64
	.size	.Ltest_immediate_u64$local, .Lfunc_end439-test_immediate_u64
                                        ! -- End function
	.globl	test_immediate_i64_long         ! -- Begin function test_immediate_i64_long
	.p2align	4
	.type	test_immediate_i64_long,@function
test_immediate_i64_long:                ! @test_immediate_i64_long
.Ltest_immediate_i64_long$local:
	.type	.Ltest_immediate_i64_long$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, -1234567890123

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end440:
	.size	test_immediate_i64_long, .Lfunc_end440-test_immediate_i64_long
	.size	.Ltest_immediate_i64_long$local, .Lfunc_end440-test_immediate_i64_long
                                        ! -- End function
	.globl	test_immediate_u64_long         ! -- Begin function test_immediate_u64_long
	.p2align	4
	.type	test_immediate_u64_long,@function
test_immediate_u64_long:                ! @test_immediate_u64_long
.Ltest_immediate_u64_long$local:
	.type	.Ltest_immediate_u64_long$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, 1234567890123

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end441:
	.size	test_immediate_u64_long, .Lfunc_end441-test_immediate_u64_long
	.size	.Ltest_immediate_u64_long$local, .Lfunc_end441-test_immediate_u64_long
                                        ! -- End function
	.globl	test_immediate_size_max         ! -- Begin function test_immediate_size_max
	.p2align	4
	.type	test_immediate_size_max,@function
test_immediate_size_max:                ! @test_immediate_size_max
.Ltest_immediate_size_max$local:
	.type	.Ltest_immediate_size_max$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, 9223372036854775807

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end442:
	.size	test_immediate_size_max, .Lfunc_end442-test_immediate_size_max
	.size	.Ltest_immediate_size_max$local, .Lfunc_end442-test_immediate_size_max
                                        ! -- End function
	.globl	test_load_based_u32             ! -- Begin function test_load_based_u32
	.p2align	4
	.type	test_load_based_u32,@function
test_load_based_u32:                    ! @test_load_based_u32
.Ltest_load_based_u32$local:
	.type	.Ltest_load_based_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldwz	%r1, %r1, 16000
	retf	0

.Lfunc_end443:
	.size	test_load_based_u32, .Lfunc_end443-test_load_based_u32
	.size	.Ltest_load_based_u32$local, .Lfunc_end443-test_load_based_u32
                                        ! -- End function
	.globl	test_load_based_u16             ! -- Begin function test_load_based_u16
	.p2align	4
	.type	test_load_based_u16,@function
test_load_based_u16:                    ! @test_load_based_u16
.Ltest_load_based_u16$local:
	.type	.Ltest_load_based_u16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhz	%r1, %r1, 8000
	retf	0

.Lfunc_end444:
	.size	test_load_based_u16, .Lfunc_end444-test_load_based_u16
	.size	.Ltest_load_based_u16$local, .Lfunc_end444-test_load_based_u16
                                        ! -- End function
	.globl	test_load_based_u8              ! -- Begin function test_load_based_u8
	.p2align	4
	.type	test_load_based_u8,@function
test_load_based_u8:                     ! @test_load_based_u8
.Ltest_load_based_u8$local:
	.type	.Ltest_load_based_u8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbz	%r1, %r1, 4000
	retf	0

.Lfunc_end445:
	.size	test_load_based_u8, .Lfunc_end445-test_load_based_u8
	.size	.Ltest_load_based_u8$local, .Lfunc_end445-test_load_based_u8
                                        ! -- End function
	.globl	test_load_based_i32             ! -- Begin function test_load_based_i32
	.p2align	4
	.type	test_load_based_i32,@function
test_load_based_i32:                    ! @test_load_based_i32
.Ltest_load_based_i32$local:
	.type	.Ltest_load_based_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldwz	%r1, %r1, 16000
	retf	0

.Lfunc_end446:
	.size	test_load_based_i32, .Lfunc_end446-test_load_based_i32
	.size	.Ltest_load_based_i32$local, .Lfunc_end446-test_load_based_i32
                                        ! -- End function
	.globl	test_load_based_i16             ! -- Begin function test_load_based_i16
	.p2align	4
	.type	test_load_based_i16,@function
test_load_based_i16:                    ! @test_load_based_i16
.Ltest_load_based_i16$local:
	.type	.Ltest_load_based_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldhs	%r1, %r1, 8000
	retf	0

.Lfunc_end447:
	.size	test_load_based_i16, .Lfunc_end447-test_load_based_i16
	.size	.Ltest_load_based_i16$local, .Lfunc_end447-test_load_based_i16
                                        ! -- End function
	.globl	test_load_based_i8              ! -- Begin function test_load_based_i8
	.p2align	4
	.type	test_load_based_i8,@function
test_load_based_i8:                     ! @test_load_based_i8
.Ltest_load_based_i8$local:
	.type	.Ltest_load_based_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldbs	%r1, %r1, 4000
	retf	0

.Lfunc_end448:
	.size	test_load_based_i8, .Lfunc_end448-test_load_based_i8
	.size	.Ltest_load_based_i8$local, .Lfunc_end448-test_load_based_i8
                                        ! -- End function
	.globl	test_base_index_u32_u32         ! -- Begin function test_base_index_u32_u32
	.p2align	4
	.type	test_base_index_u32_u32,@function
test_base_index_u32_u32:                ! @test_base_index_u32_u32
.Ltest_base_index_u32_u32$local:
	.type	.Ltest_base_index_u32_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	divuw	%r2, %r2, %r3
	lddzx.wz	%r1, %r1, %r2, 3, 0

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end449:
	.size	test_base_index_u32_u32, .Lfunc_end449-test_base_index_u32_u32
	.size	.Ltest_base_index_u32_u32$local, .Lfunc_end449-test_base_index_u32_u32
                                        ! -- End function
	.globl	test_base_address               ! -- Begin function test_base_address
	.p2align	4
	.type	test_base_address,@function
test_base_address:                      ! @test_base_address
.Ltest_base_address$local:
	.type	.Ltest_base_address$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	8
	lddz.l	%r5, %r1, 800000000000

	! BUNDLE
	lddz	%r6, %r1, 8000
	lddzx.l	%r7, %r1, %r2, 3, 800000000

	! BUNDLE
	lddzx	%r1, %r1, %r4, 3, 80
	lddzx	%r2, %r3, %r2, 4, 160
	nop	2

	! BUNDLE
	lddzx.l	%r3, %r3, %r4, 4, 1608
	add	%r4, %r6, %r5

	! BUNDLE
	addadd	%r1, %r4, %r7, %r1
	addadd	%r1, %r1, %r2, %r3
	retf	0

.Lfunc_end450:
	.size	test_base_address, .Lfunc_end450-test_base_address
	.size	.Ltest_base_address$local, .Lfunc_end450-test_base_address
                                        ! -- End function
	.globl	MemoryIndexed                   ! -- Begin function MemoryIndexed
	.p2align	4
	.type	MemoryIndexed,@function
MemoryIndexed:                          ! @MemoryIndexed
.LMemoryIndexed$local:
	.type	.LMemoryIndexed$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	bdlti	%r3, 1, .LBB451_3
	addi	%r2, %r2, 40

.LBB451_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	lddz	%r4, %r1, 0
	lddzx	%r4, %r2, %r4, 4, 0
	std	%r4, %r1, 0

	! BUNDLE
	addi	%r3, %r3, -1
	addi	%r1, %r1, 8
	bdne	%r3, %gz, .LBB451_2

.LBB451_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end451:
	.size	MemoryIndexed, .Lfunc_end451-MemoryIndexed
	.size	.LMemoryIndexed$local, .Lfunc_end451-MemoryIndexed
                                        ! -- End function
	.globl	test_base_index_i32             ! -- Begin function test_base_index_i32
	.p2align	4
	.type	test_base_index_i32,@function
test_base_index_i32:                    ! @test_base_index_i32
.Ltest_base_index_i32$local:
	.type	.Ltest_base_index_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	lddzx.ws	%r1, %r1, %r2, 3, 0
	retf	0

.Lfunc_end452:
	.size	test_base_index_i32, .Lfunc_end452-test_base_index_i32
	.size	.Ltest_base_index_i32$local, .Lfunc_end452-test_base_index_i32
                                        ! -- End function
	.globl	test_base_index_i32_offset      ! -- Begin function test_base_index_i32_offset
	.p2align	4
	.type	test_base_index_i32_offset,@function
test_base_index_i32_offset:             ! @test_base_index_i32_offset
.Ltest_base_index_i32_offset$local:
	.type	.Ltest_base_index_i32_offset$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	lddzx.ws	%r1, %r1, %r2, 3, 160
	retf	0

.Lfunc_end453:
	.size	test_base_index_i32_offset, .Lfunc_end453-test_base_index_i32_offset
	.size	.Ltest_base_index_i32_offset$local, .Lfunc_end453-test_base_index_i32_offset
                                        ! -- End function
	.globl	test_base_index_u32             ! -- Begin function test_base_index_u32
	.p2align	4
	.type	test_base_index_u32,@function
test_base_index_u32:                    ! @test_base_index_u32
.Ltest_base_index_u32$local:
	.type	.Ltest_base_index_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	lddzx.wz	%r1, %r1, %r2, 3, 0
	retf	0

.Lfunc_end454:
	.size	test_base_index_u32, .Lfunc_end454-test_base_index_u32
	.size	.Ltest_base_index_u32$local, .Lfunc_end454-test_base_index_u32
                                        ! -- End function
	.globl	test_base_index_u32_offset      ! -- Begin function test_base_index_u32_offset
	.p2align	4
	.type	test_base_index_u32_offset,@function
test_base_index_u32_offset:             ! @test_base_index_u32_offset
.Ltest_base_index_u32_offset$local:
	.type	.Ltest_base_index_u32_offset$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	addiws	%r2, %r2, 20
	lddzx.wz	%r1, %r1, %r2, 3, 0

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end455:
	.size	test_base_index_u32_offset, .Lfunc_end455-test_base_index_u32_offset
	.size	.Ltest_base_index_u32_offset$local, .Lfunc_end455-test_base_index_u32_offset
                                        ! -- End function
	.globl	test_base_index_i64             ! -- Begin function test_base_index_i64
	.p2align	4
	.type	test_base_index_i64,@function
test_base_index_i64:                    ! @test_base_index_i64
.Ltest_base_index_i64$local:
	.type	.Ltest_base_index_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	lddzx	%r1, %r1, %r2, 3, 160
	retf	0

.Lfunc_end456:
	.size	test_base_index_i64, .Lfunc_end456-test_base_index_i64
	.size	.Ltest_base_index_i64$local, .Lfunc_end456-test_base_index_i64
                                        ! -- End function
	.globl	test_base_index_i32_struct      ! -- Begin function test_base_index_i32_struct
	.p2align	4
	.type	test_base_index_i32_struct,@function
test_base_index_i32_struct:             ! @test_base_index_i32_struct
.Ltest_base_index_i32_struct$local:
	.type	.Ltest_base_index_i32_struct$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	lddzx.ws.l	%r1, %r1, %r2, 4, 328

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end457:
	.size	test_base_index_i32_struct, .Lfunc_end457-test_base_index_i32_struct
	.size	.Ltest_base_index_i32_struct$local, .Lfunc_end457-test_base_index_i32_struct
                                        ! -- End function
	.globl	for_loop                        ! -- Begin function for_loop
	.p2align	4
	.type	for_loop,@function
for_loop:                               ! @for_loop
.Lfor_loop$local:
	.type	.Lfor_loop$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	7
	mov	%r4, %gz
	bdlti	%r1, 1, .LBB458_3

	! BUNDLE
	addi	%r5, %r3, 80
	nop	0
	nop	0

.LBB458_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	lddz	%r6, %r5, 0
	add	%r4, %r6, %r4
	addi	%r1, %r1, -1

	! BUNDLE
	addi	%r5, %r5, 8
	bdne	%r1, %gz, .LBB458_2
	nop	0

.LBB458_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	lddzx	%r1, %r3, %r2, 3, 168
	add	%r1, %r1, %r4
	retf	0

.Lfunc_end458:
	.size	for_loop, .Lfunc_end458-for_loop
	.size	.Lfor_loop$local, .Lfunc_end458-for_loop
                                        ! -- End function
	.globl	for_loop_32                     ! -- Begin function for_loop_32
	.p2align	4
	.type	for_loop_32,@function
for_loop_32:                            ! @for_loop_32
.Lfor_loop_32$local:
	.type	.Lfor_loop_32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	7
	mov	%r4, %gz
	bwlti	%r1, 1, .LBB459_3

	! BUNDLE
	addi	%r5, %r3, 40
	srlwi	%r1, %r1, 0
	nop	0

.LBB459_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ldwz	%r6, %r5, 0
	addw	%r4, %r6, %r4
	addi	%r1, %r1, -1

	! BUNDLE
	addi	%r5, %r5, 4
	bdne	%r1, %gz, .LBB459_2
	nop	0

.LBB459_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	ldwzx.ws	%r1, %r3, %r2, 2, 84
	addw	%r1, %r1, %r4
	retf	0

.Lfunc_end459:
	.size	for_loop_32, .Lfunc_end459-for_loop_32
	.size	.Lfor_loop_32$local, .Lfunc_end459-for_loop_32
                                        ! -- End function
	.globl	test_i64_min_lt                 ! -- Begin function test_i64_min_lt
	.p2align	4
	.type	test_i64_min_lt,@function
test_i64_min_lt:                        ! @test_i64_min_lt
.Ltest_i64_min_lt$local:
	.type	.Ltest_i64_min_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mins	%r1, %r1, %r2
	retf	0

.Lfunc_end460:
	.size	test_i64_min_lt, .Lfunc_end460-test_i64_min_lt
	.size	.Ltest_i64_min_lt$local, .Lfunc_end460-test_i64_min_lt
                                        ! -- End function
	.globl	test_u64_min_lt                 ! -- Begin function test_u64_min_lt
	.p2align	4
	.type	test_u64_min_lt,@function
test_u64_min_lt:                        ! @test_u64_min_lt
.Ltest_u64_min_lt$local:
	.type	.Ltest_u64_min_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minu	%r1, %r1, %r2
	retf	0

.Lfunc_end461:
	.size	test_u64_min_lt, .Lfunc_end461-test_u64_min_lt
	.size	.Ltest_u64_min_lt$local, .Lfunc_end461-test_u64_min_lt
                                        ! -- End function
	.globl	test_i64_min_le                 ! -- Begin function test_i64_min_le
	.p2align	4
	.type	test_i64_min_le,@function
test_i64_min_le:                        ! @test_i64_min_le
.Ltest_i64_min_le$local:
	.type	.Ltest_i64_min_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mins	%r1, %r1, %r2
	retf	0

.Lfunc_end462:
	.size	test_i64_min_le, .Lfunc_end462-test_i64_min_le
	.size	.Ltest_i64_min_le$local, .Lfunc_end462-test_i64_min_le
                                        ! -- End function
	.globl	test_u64_min_le                 ! -- Begin function test_u64_min_le
	.p2align	4
	.type	test_u64_min_le,@function
test_u64_min_le:                        ! @test_u64_min_le
.Ltest_u64_min_le$local:
	.type	.Ltest_u64_min_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minu	%r1, %r1, %r2
	retf	0

.Lfunc_end463:
	.size	test_u64_min_le, .Lfunc_end463-test_u64_min_le
	.size	.Ltest_u64_min_le$local, .Lfunc_end463-test_u64_min_le
                                        ! -- End function
	.globl	test_i64_min_gt                 ! -- Begin function test_i64_min_gt
	.p2align	4
	.type	test_i64_min_gt,@function
test_i64_min_gt:                        ! @test_i64_min_gt
.Ltest_i64_min_gt$local:
	.type	.Ltest_i64_min_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mins	%r1, %r2, %r1
	retf	0

.Lfunc_end464:
	.size	test_i64_min_gt, .Lfunc_end464-test_i64_min_gt
	.size	.Ltest_i64_min_gt$local, .Lfunc_end464-test_i64_min_gt
                                        ! -- End function
	.globl	test_u64_min_gt                 ! -- Begin function test_u64_min_gt
	.p2align	4
	.type	test_u64_min_gt,@function
test_u64_min_gt:                        ! @test_u64_min_gt
.Ltest_u64_min_gt$local:
	.type	.Ltest_u64_min_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minu	%r1, %r2, %r1
	retf	0

.Lfunc_end465:
	.size	test_u64_min_gt, .Lfunc_end465-test_u64_min_gt
	.size	.Ltest_u64_min_gt$local, .Lfunc_end465-test_u64_min_gt
                                        ! -- End function
	.globl	test_i64_min_ge                 ! -- Begin function test_i64_min_ge
	.p2align	4
	.type	test_i64_min_ge,@function
test_i64_min_ge:                        ! @test_i64_min_ge
.Ltest_i64_min_ge$local:
	.type	.Ltest_i64_min_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mins	%r1, %r2, %r1
	retf	0

.Lfunc_end466:
	.size	test_i64_min_ge, .Lfunc_end466-test_i64_min_ge
	.size	.Ltest_i64_min_ge$local, .Lfunc_end466-test_i64_min_ge
                                        ! -- End function
	.globl	test_u64_min_ge                 ! -- Begin function test_u64_min_ge
	.p2align	4
	.type	test_u64_min_ge,@function
test_u64_min_ge:                        ! @test_u64_min_ge
.Ltest_u64_min_ge$local:
	.type	.Ltest_u64_min_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minu	%r1, %r2, %r1
	retf	0

.Lfunc_end467:
	.size	test_u64_min_ge, .Lfunc_end467-test_u64_min_ge
	.size	.Ltest_u64_min_ge$local, .Lfunc_end467-test_u64_min_ge
                                        ! -- End function
	.globl	test_i64_max_lt                 ! -- Begin function test_i64_max_lt
	.p2align	4
	.type	test_i64_max_lt,@function
test_i64_max_lt:                        ! @test_i64_max_lt
.Ltest_i64_max_lt$local:
	.type	.Ltest_i64_max_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxs	%r1, %r1, %r2
	retf	0

.Lfunc_end468:
	.size	test_i64_max_lt, .Lfunc_end468-test_i64_max_lt
	.size	.Ltest_i64_max_lt$local, .Lfunc_end468-test_i64_max_lt
                                        ! -- End function
	.globl	test_u64_max_lt                 ! -- Begin function test_u64_max_lt
	.p2align	4
	.type	test_u64_max_lt,@function
test_u64_max_lt:                        ! @test_u64_max_lt
.Ltest_u64_max_lt$local:
	.type	.Ltest_u64_max_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxu	%r1, %r1, %r2
	retf	0

.Lfunc_end469:
	.size	test_u64_max_lt, .Lfunc_end469-test_u64_max_lt
	.size	.Ltest_u64_max_lt$local, .Lfunc_end469-test_u64_max_lt
                                        ! -- End function
	.globl	test_i64_max_le                 ! -- Begin function test_i64_max_le
	.p2align	4
	.type	test_i64_max_le,@function
test_i64_max_le:                        ! @test_i64_max_le
.Ltest_i64_max_le$local:
	.type	.Ltest_i64_max_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxs	%r1, %r1, %r2
	retf	0

.Lfunc_end470:
	.size	test_i64_max_le, .Lfunc_end470-test_i64_max_le
	.size	.Ltest_i64_max_le$local, .Lfunc_end470-test_i64_max_le
                                        ! -- End function
	.globl	test_u64_max_le                 ! -- Begin function test_u64_max_le
	.p2align	4
	.type	test_u64_max_le,@function
test_u64_max_le:                        ! @test_u64_max_le
.Ltest_u64_max_le$local:
	.type	.Ltest_u64_max_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxu	%r1, %r1, %r2
	retf	0

.Lfunc_end471:
	.size	test_u64_max_le, .Lfunc_end471-test_u64_max_le
	.size	.Ltest_u64_max_le$local, .Lfunc_end471-test_u64_max_le
                                        ! -- End function
	.globl	test_i64_max_gt                 ! -- Begin function test_i64_max_gt
	.p2align	4
	.type	test_i64_max_gt,@function
test_i64_max_gt:                        ! @test_i64_max_gt
.Ltest_i64_max_gt$local:
	.type	.Ltest_i64_max_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxs	%r1, %r2, %r1
	retf	0

.Lfunc_end472:
	.size	test_i64_max_gt, .Lfunc_end472-test_i64_max_gt
	.size	.Ltest_i64_max_gt$local, .Lfunc_end472-test_i64_max_gt
                                        ! -- End function
	.globl	test_u64_max_gt                 ! -- Begin function test_u64_max_gt
	.p2align	4
	.type	test_u64_max_gt,@function
test_u64_max_gt:                        ! @test_u64_max_gt
.Ltest_u64_max_gt$local:
	.type	.Ltest_u64_max_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxu	%r1, %r2, %r1
	retf	0

.Lfunc_end473:
	.size	test_u64_max_gt, .Lfunc_end473-test_u64_max_gt
	.size	.Ltest_u64_max_gt$local, .Lfunc_end473-test_u64_max_gt
                                        ! -- End function
	.globl	test_i64_max_ge                 ! -- Begin function test_i64_max_ge
	.p2align	4
	.type	test_i64_max_ge,@function
test_i64_max_ge:                        ! @test_i64_max_ge
.Ltest_i64_max_ge$local:
	.type	.Ltest_i64_max_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxs	%r1, %r2, %r1
	retf	0

.Lfunc_end474:
	.size	test_i64_max_ge, .Lfunc_end474-test_i64_max_ge
	.size	.Ltest_i64_max_ge$local, .Lfunc_end474-test_i64_max_ge
                                        ! -- End function
	.globl	test_u64_max_ge                 ! -- Begin function test_u64_max_ge
	.p2align	4
	.type	test_u64_max_ge,@function
test_u64_max_ge:                        ! @test_u64_max_ge
.Ltest_u64_max_ge$local:
	.type	.Ltest_u64_max_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxu	%r1, %r2, %r1
	retf	0

.Lfunc_end475:
	.size	test_u64_max_ge, .Lfunc_end475-test_u64_max_ge
	.size	.Ltest_u64_max_ge$local, .Lfunc_end475-test_u64_max_ge
                                        ! -- End function
	.globl	test_i32_min_lt                 ! -- Begin function test_i32_min_lt
	.p2align	4
	.type	test_i32_min_lt,@function
test_i32_min_lt:                        ! @test_i32_min_lt
.Ltest_i32_min_lt$local:
	.type	.Ltest_i32_min_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minsw	%r1, %r1, %r2
	retf	0

.Lfunc_end476:
	.size	test_i32_min_lt, .Lfunc_end476-test_i32_min_lt
	.size	.Ltest_i32_min_lt$local, .Lfunc_end476-test_i32_min_lt
                                        ! -- End function
	.globl	test_u32_min_lt                 ! -- Begin function test_u32_min_lt
	.p2align	4
	.type	test_u32_min_lt,@function
test_u32_min_lt:                        ! @test_u32_min_lt
.Ltest_u32_min_lt$local:
	.type	.Ltest_u32_min_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minuw	%r1, %r1, %r2
	retf	0

.Lfunc_end477:
	.size	test_u32_min_lt, .Lfunc_end477-test_u32_min_lt
	.size	.Ltest_u32_min_lt$local, .Lfunc_end477-test_u32_min_lt
                                        ! -- End function
	.globl	test_i32_min_le                 ! -- Begin function test_i32_min_le
	.p2align	4
	.type	test_i32_min_le,@function
test_i32_min_le:                        ! @test_i32_min_le
.Ltest_i32_min_le$local:
	.type	.Ltest_i32_min_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minsw	%r1, %r1, %r2
	retf	0

.Lfunc_end478:
	.size	test_i32_min_le, .Lfunc_end478-test_i32_min_le
	.size	.Ltest_i32_min_le$local, .Lfunc_end478-test_i32_min_le
                                        ! -- End function
	.globl	test_u32_min_le                 ! -- Begin function test_u32_min_le
	.p2align	4
	.type	test_u32_min_le,@function
test_u32_min_le:                        ! @test_u32_min_le
.Ltest_u32_min_le$local:
	.type	.Ltest_u32_min_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minuw	%r1, %r1, %r2
	retf	0

.Lfunc_end479:
	.size	test_u32_min_le, .Lfunc_end479-test_u32_min_le
	.size	.Ltest_u32_min_le$local, .Lfunc_end479-test_u32_min_le
                                        ! -- End function
	.globl	test_i32_min_gt                 ! -- Begin function test_i32_min_gt
	.p2align	4
	.type	test_i32_min_gt,@function
test_i32_min_gt:                        ! @test_i32_min_gt
.Ltest_i32_min_gt$local:
	.type	.Ltest_i32_min_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minsw	%r1, %r2, %r1
	retf	0

.Lfunc_end480:
	.size	test_i32_min_gt, .Lfunc_end480-test_i32_min_gt
	.size	.Ltest_i32_min_gt$local, .Lfunc_end480-test_i32_min_gt
                                        ! -- End function
	.globl	test_u32_min_gt                 ! -- Begin function test_u32_min_gt
	.p2align	4
	.type	test_u32_min_gt,@function
test_u32_min_gt:                        ! @test_u32_min_gt
.Ltest_u32_min_gt$local:
	.type	.Ltest_u32_min_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minuw	%r1, %r2, %r1
	retf	0

.Lfunc_end481:
	.size	test_u32_min_gt, .Lfunc_end481-test_u32_min_gt
	.size	.Ltest_u32_min_gt$local, .Lfunc_end481-test_u32_min_gt
                                        ! -- End function
	.globl	test_i32_min_ge                 ! -- Begin function test_i32_min_ge
	.p2align	4
	.type	test_i32_min_ge,@function
test_i32_min_ge:                        ! @test_i32_min_ge
.Ltest_i32_min_ge$local:
	.type	.Ltest_i32_min_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minsw	%r1, %r2, %r1
	retf	0

.Lfunc_end482:
	.size	test_i32_min_ge, .Lfunc_end482-test_i32_min_ge
	.size	.Ltest_i32_min_ge$local, .Lfunc_end482-test_i32_min_ge
                                        ! -- End function
	.globl	test_u32_min_ge                 ! -- Begin function test_u32_min_ge
	.p2align	4
	.type	test_u32_min_ge,@function
test_u32_min_ge:                        ! @test_u32_min_ge
.Ltest_u32_min_ge$local:
	.type	.Ltest_u32_min_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	minuw	%r1, %r2, %r1
	retf	0

.Lfunc_end483:
	.size	test_u32_min_ge, .Lfunc_end483-test_u32_min_ge
	.size	.Ltest_u32_min_ge$local, .Lfunc_end483-test_u32_min_ge
                                        ! -- End function
	.globl	test_i32_max_lt                 ! -- Begin function test_i32_max_lt
	.p2align	4
	.type	test_i32_max_lt,@function
test_i32_max_lt:                        ! @test_i32_max_lt
.Ltest_i32_max_lt$local:
	.type	.Ltest_i32_max_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxsw	%r1, %r1, %r2
	retf	0

.Lfunc_end484:
	.size	test_i32_max_lt, .Lfunc_end484-test_i32_max_lt
	.size	.Ltest_i32_max_lt$local, .Lfunc_end484-test_i32_max_lt
                                        ! -- End function
	.globl	test_u32_max_lt                 ! -- Begin function test_u32_max_lt
	.p2align	4
	.type	test_u32_max_lt,@function
test_u32_max_lt:                        ! @test_u32_max_lt
.Ltest_u32_max_lt$local:
	.type	.Ltest_u32_max_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxuw	%r1, %r1, %r2
	retf	0

.Lfunc_end485:
	.size	test_u32_max_lt, .Lfunc_end485-test_u32_max_lt
	.size	.Ltest_u32_max_lt$local, .Lfunc_end485-test_u32_max_lt
                                        ! -- End function
	.globl	test_i32_max_le                 ! -- Begin function test_i32_max_le
	.p2align	4
	.type	test_i32_max_le,@function
test_i32_max_le:                        ! @test_i32_max_le
.Ltest_i32_max_le$local:
	.type	.Ltest_i32_max_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxsw	%r1, %r1, %r2
	retf	0

.Lfunc_end486:
	.size	test_i32_max_le, .Lfunc_end486-test_i32_max_le
	.size	.Ltest_i32_max_le$local, .Lfunc_end486-test_i32_max_le
                                        ! -- End function
	.globl	test_u32_max_le                 ! -- Begin function test_u32_max_le
	.p2align	4
	.type	test_u32_max_le,@function
test_u32_max_le:                        ! @test_u32_max_le
.Ltest_u32_max_le$local:
	.type	.Ltest_u32_max_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxuw	%r1, %r1, %r2
	retf	0

.Lfunc_end487:
	.size	test_u32_max_le, .Lfunc_end487-test_u32_max_le
	.size	.Ltest_u32_max_le$local, .Lfunc_end487-test_u32_max_le
                                        ! -- End function
	.globl	test_i32_max_gt                 ! -- Begin function test_i32_max_gt
	.p2align	4
	.type	test_i32_max_gt,@function
test_i32_max_gt:                        ! @test_i32_max_gt
.Ltest_i32_max_gt$local:
	.type	.Ltest_i32_max_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxsw	%r1, %r2, %r1
	retf	0

.Lfunc_end488:
	.size	test_i32_max_gt, .Lfunc_end488-test_i32_max_gt
	.size	.Ltest_i32_max_gt$local, .Lfunc_end488-test_i32_max_gt
                                        ! -- End function
	.globl	test_u32_max_gt                 ! -- Begin function test_u32_max_gt
	.p2align	4
	.type	test_u32_max_gt,@function
test_u32_max_gt:                        ! @test_u32_max_gt
.Ltest_u32_max_gt$local:
	.type	.Ltest_u32_max_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxuw	%r1, %r2, %r1
	retf	0

.Lfunc_end489:
	.size	test_u32_max_gt, .Lfunc_end489-test_u32_max_gt
	.size	.Ltest_u32_max_gt$local, .Lfunc_end489-test_u32_max_gt
                                        ! -- End function
	.globl	test_i32_max_ge                 ! -- Begin function test_i32_max_ge
	.p2align	4
	.type	test_i32_max_ge,@function
test_i32_max_ge:                        ! @test_i32_max_ge
.Ltest_i32_max_ge$local:
	.type	.Ltest_i32_max_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxsw	%r1, %r2, %r1
	retf	0

.Lfunc_end490:
	.size	test_i32_max_ge, .Lfunc_end490-test_i32_max_ge
	.size	.Ltest_i32_max_ge$local, .Lfunc_end490-test_i32_max_ge
                                        ! -- End function
	.globl	test_u32_max_ge                 ! -- Begin function test_u32_max_ge
	.p2align	4
	.type	test_u32_max_ge,@function
test_u32_max_ge:                        ! @test_u32_max_ge
.Ltest_u32_max_ge$local:
	.type	.Ltest_u32_max_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	maxuw	%r1, %r2, %r1
	retf	0

.Lfunc_end491:
	.size	test_u32_max_ge, .Lfunc_end491-test_u32_max_ge
	.size	.Ltest_u32_max_ge$local, .Lfunc_end491-test_u32_max_ge
                                        ! -- End function
	.globl	test_mov2                       ! -- Begin function test_mov2
	.p2align	4
	.type	test_mov2,@function
test_mov2:                              ! @test_mov2
.Ltest_mov2$local:
	.type	.Ltest_mov2$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	11
	mov2	%r7, %r8, %r3, %r4
	mov2	%r9, %r10, %r2, %r1

	! BUNDLE
	callr	%r6, %jpc(test_mov2_callback)
	addw	%r5, %r7, %r1
	mov2	%r7, %r8, %r3, %r4

	! BUNDLE
	mov2	%r9, %r10, %r2, %gz
	callr	%r6, %jpc(test_mov2_callback)
	addw	%r2, %r5, %r7

	! BUNDLE
	mov2	%r7, %r8, %r3, %r4
	mov2	%r9, %r10, %gz, %gz
	callr	%r6, %jpc(test_mov2_callback)

	! BUNDLE
	addw	%r2, %r2, %r7
	mov2	%r7, %r8, %r3, %gz
	mov2	%r9, %r10, %gz, %gz

	! BUNDLE
	callr	%r6, %jpc(test_mov2_callback)
	addw	%r2, %r2, %r7
	mov2	%r7, %r8, %gz, %gz

	! BUNDLE
	mov2	%r9, %r10, %gz, %gz
	callr	%r6, %jpc(test_mov2_callback)
	addw	%r1, %r2, %r7

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end492:
	.size	test_mov2, .Lfunc_end492-test_mov2
	.size	.Ltest_mov2$local, .Lfunc_end492-test_mov2
                                        ! -- End function
	.globl	test_naked                      ! -- Begin function test_naked
	.p2align	4
	.type	test_naked,@function
test_naked:                             ! @test_naked
.Ltest_naked$local:
	.type	.Ltest_naked$local,@function
! %bb.0:                                ! %entry
.Lfunc_end493:
	.size	test_naked, .Lfunc_end493-test_naked
	.size	.Ltest_naked$local, .Lfunc_end493-test_naked
                                        ! -- End function
	.globl	test_after_naked                ! -- Begin function test_after_naked
	.p2align	4
	.type	test_after_naked,@function
test_after_naked:                       ! @test_after_naked
.Ltest_after_naked$local:
	.type	.Ltest_after_naked$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	retf	0
	nop	0

.Lfunc_end494:
	.size	test_after_naked, .Lfunc_end494-test_after_naked
	.size	.Ltest_after_naked$local, .Lfunc_end494-test_after_naked
                                        ! -- End function
	.globl	test_nullification_i64_le       ! -- Begin function test_nullification_i64_le
	.p2align	4
	.type	test_nullification_i64_le,@function
test_nullification_i64_le:              ! @test_nullification_i64_le
.Ltest_nullification_i64_le$local:
	.type	.Ltest_nullification_i64_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	lddz	%r4, %r2, 16
	nuldle	%r4, %r1, 3, 4

	! BUNDLE
	lddzx	%r2, %r2, %r1, 3, 8
	muli	%r4, %r2, 5
	ldi	%r2, 2000

	! BUNDLE
	lddzx	%r4, %r3, %r1, 3, 16
	muli	%r4, %r4, 3
	stdx	%gz, %r2, %r1, 3, 0

	! BUNDLE
	ldi	%r2, 3000
	lddzx	%r3, %r3, %r1, 3, 40
	add	%r1, %r1, %r2

	! BUNDLE
	addadd	%r1, %r1, %r4, %r3
	retf	0
	nop	0

.Lfunc_end495:
	.size	test_nullification_i64_le, .Lfunc_end495-test_nullification_i64_le
	.size	.Ltest_nullification_i64_le$local, .Lfunc_end495-test_nullification_i64_le
                                        ! -- End function
	.globl	test_nullification_i64_lt       ! -- Begin function test_nullification_i64_lt
	.p2align	4
	.type	test_nullification_i64_lt,@function
test_nullification_i64_lt:              ! @test_nullification_i64_lt
.Ltest_nullification_i64_lt$local:
	.type	.Ltest_nullification_i64_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	lddz	%r4, %r2, 16
	nuldlt	%r4, %r1, 3, 4

	! BUNDLE
	lddzx	%r2, %r2, %r1, 3, 8
	muli	%r4, %r2, 5
	ldi	%r2, 2000

	! BUNDLE
	lddzx	%r4, %r3, %r1, 3, 16
	muli	%r4, %r4, 3
	stdx	%gz, %r2, %r1, 3, 0

	! BUNDLE
	ldi	%r2, 3000
	lddzx	%r3, %r3, %r1, 3, 40
	add	%r1, %r1, %r2

	! BUNDLE
	addadd	%r1, %r1, %r4, %r3
	retf	0
	nop	0

.Lfunc_end496:
	.size	test_nullification_i64_lt, .Lfunc_end496-test_nullification_i64_lt
	.size	.Ltest_nullification_i64_lt$local, .Lfunc_end496-test_nullification_i64_lt
                                        ! -- End function
	.globl	test_nuldleu                    ! -- Begin function test_nuldleu
	.p2align	4
	.type	test_nuldleu,@function
test_nuldleu:                           ! @test_nuldleu
.Ltest_nuldleu$local:
	.type	.Ltest_nuldleu$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nuldleu	%r2, %r1, 2, 3
	lddzr	%r2, %pcfwd(left)

	! BUNDLE
	add	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right)
	add	%r1, %r2, %r1

	! BUNDLE
	stdx	%gz, %r3, %r1, 3, 16
	lddz	%r2, %r3, 80
	add	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end497:
	.size	test_nuldleu, .Lfunc_end497-test_nuldleu
	.size	.Ltest_nuldleu$local, .Lfunc_end497-test_nuldleu
                                        ! -- End function
	.globl	test_nullify_bool               ! -- Begin function test_nullify_bool
	.p2align	4
	.type	test_nullify_bool,@function
test_nullify_bool:                      ! @test_nullify_bool
.Ltest_nullify_bool$local:
	.type	.Ltest_nullify_bool$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulbci	%r1, 0, 2, 3
	lddzr	%r2, %pcfwd(left_bool)

	! BUNDLE
	add	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1

	! BUNDLE
	stdx	%gz, %r3, %r1, 3, 16
	lddz	%r2, %r3, 80
	add	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end498:
	.size	test_nullify_bool, .Lfunc_end498-test_nullify_bool
	.size	.Ltest_nullify_bool$local, .Lfunc_end498-test_nullify_bool
                                        ! -- End function
	.globl	test_nullify_bool_not           ! -- Begin function test_nullify_bool_not
	.p2align	4
	.type	test_nullify_bool_not,@function
test_nullify_bool_not:                  ! @test_nullify_bool_not
.Ltest_nullify_bool_not$local:
	.type	.Ltest_nullify_bool_not$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulbsi	%r1, 0, 2, 3
	lddzr	%r2, %pcfwd(left_bool)

	! BUNDLE
	add	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1

	! BUNDLE
	stdx	%gz, %r3, %r1, 3, 16
	lddz	%r2, %r3, 80
	add	%r1, %r2, %r1

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end499:
	.size	test_nullify_bool_not, .Lfunc_end499-test_nullify_bool_not
	.size	.Ltest_nullify_bool_not$local, .Lfunc_end499-test_nullify_bool_not
                                        ! -- End function
	.globl	test_nullify_bool_and           ! -- Begin function test_nullify_bool_and
	.p2align	4
	.type	test_nullify_bool_and,@function
test_nullify_bool_and:                  ! @test_nullify_bool_and
.Ltest_nullify_bool_and$local:
	.type	.Ltest_nullify_bool_and$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	bbci	%r1, 0, .LBB500_3
	bbci	%r2, 0, .LBB500_3

	! BUNDLE
	lddzr	%r2, %pcfwd(left_bool)
	add	%r1, %r2, %r1
	jmp	.LBB500_4

.LBB500_3:                              ! %if.else
                                        ! Label of block must be emitted
	! BUNDLE
	lddzr	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1
	stdx	%gz, %r3, %r1, 3, 16

.LBB500_4:                              ! %if.end
                                        ! Label of block must be emitted
	! BUNDLE
	lddz	%r2, %r3, 80
	add	%r1, %r2, %r1
	retf	0

.Lfunc_end500:
	.size	test_nullify_bool_and, .Lfunc_end500-test_nullify_bool_and
	.size	.Ltest_nullify_bool_and$local, .Lfunc_end500-test_nullify_bool_and
                                        ! -- End function
	.globl	test_nullify_bool_or            ! -- Begin function test_nullify_bool_or
	.p2align	4
	.type	test_nullify_bool_or,@function
test_nullify_bool_or:                   ! @test_nullify_bool_or
.Ltest_nullify_bool_or$local:
	.type	.Ltest_nullify_bool_or$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	bbsi	%r1, 0, .LBB501_2
	bbsi	%r2, 0, .LBB501_2

	! BUNDLE
	lddzr	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1
	stdx	%gz, %r3, %r1, 3, 16

	! BUNDLE
	jmp	.LBB501_4
	nop	0
	nop	0

.LBB501_2:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	lddzr	%r2, %pcfwd(left_bool)
	add	%r1, %r2, %r1
	nop	0

.LBB501_4:                              ! %if.end
                                        ! Label of block must be emitted
	! BUNDLE
	lddz	%r2, %r3, 80
	add	%r1, %r2, %r1
	retf	0

.Lfunc_end501:
	.size	test_nullify_bool_or, .Lfunc_end501-test_nullify_bool_or
	.size	.Ltest_nullify_bool_or$local, .Lfunc_end501-test_nullify_bool_or
                                        ! -- End function
	.globl	test_nullify_f128_eq            ! -- Begin function test_nullify_f128_eq
	.p2align	4
	.type	test_nullify_f128_eq,@function
test_nullify_f128_eq:                   ! @test_nullify_f128_eq
.Ltest_nullify_f128_eq$local:
	.type	.Ltest_nullify_f128_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsqone	%r1, %r2, 2, 4
	ldqr	%r2, %pcfwd(left_f128)

	! BUNDLE
	faddsq	%r1, %r2, %r1
	ldqr	%r2, %pcfwd(right_f128)
	faddsq	%r1, %r2, %r1

	! BUNDLE
	fcvtsq2id	%r2, %r1, 0
	stqx	%gz, %r3, %r2, 4, 32
	ldq	%r2, %r3, 160

	! BUNDLE
	faddsq	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end502:
	.size	test_nullify_f128_eq, .Lfunc_end502-test_nullify_f128_eq
	.size	.Ltest_nullify_f128_eq$local, .Lfunc_end502-test_nullify_f128_eq
                                        ! -- End function
	.globl	test_nullify_f128_ne            ! -- Begin function test_nullify_f128_ne
	.p2align	4
	.type	test_nullify_f128_ne,@function
test_nullify_f128_ne:                   ! @test_nullify_f128_ne
.Ltest_nullify_f128_ne$local:
	.type	.Ltest_nullify_f128_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsqoeq	%r1, %r2, 2, 4
	ldqr	%r2, %pcfwd(left_f128)

	! BUNDLE
	faddsq	%r1, %r2, %r1
	ldqr	%r2, %pcfwd(right_f128)
	faddsq	%r1, %r2, %r1

	! BUNDLE
	fcvtsq2id	%r2, %r1, 0
	stqx	%gz, %r3, %r2, 4, 32
	ldq	%r2, %r3, 160

	! BUNDLE
	faddsq	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end503:
	.size	test_nullify_f128_ne, .Lfunc_end503-test_nullify_f128_ne
	.size	.Ltest_nullify_f128_ne$local, .Lfunc_end503-test_nullify_f128_ne
                                        ! -- End function
	.globl	test_nullify_f128_lt            ! -- Begin function test_nullify_f128_lt
	.p2align	4
	.type	test_nullify_f128_lt,@function
test_nullify_f128_lt:                   ! @test_nullify_f128_lt
.Ltest_nullify_f128_lt$local:
	.type	.Ltest_nullify_f128_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsqole	%r2, %r1, 2, 4
	ldqr	%r2, %pcfwd(left_f128)

	! BUNDLE
	faddsq	%r1, %r2, %r1
	ldqr	%r2, %pcfwd(right_f128)
	faddsq	%r1, %r2, %r1

	! BUNDLE
	fcvtsq2id	%r2, %r1, 0
	stqx	%gz, %r3, %r2, 4, 32
	ldq	%r2, %r3, 160

	! BUNDLE
	faddsq	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end504:
	.size	test_nullify_f128_lt, .Lfunc_end504-test_nullify_f128_lt
	.size	.Ltest_nullify_f128_lt$local, .Lfunc_end504-test_nullify_f128_lt
                                        ! -- End function
	.globl	test_nullify_f128_le            ! -- Begin function test_nullify_f128_le
	.p2align	4
	.type	test_nullify_f128_le,@function
test_nullify_f128_le:                   ! @test_nullify_f128_le
.Ltest_nullify_f128_le$local:
	.type	.Ltest_nullify_f128_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsqolt	%r2, %r1, 2, 4
	ldqr	%r2, %pcfwd(left_f128)

	! BUNDLE
	faddsq	%r1, %r2, %r1
	ldqr	%r2, %pcfwd(right_f128)
	faddsq	%r1, %r2, %r1

	! BUNDLE
	fcvtsq2id	%r2, %r1, 0
	stqx	%gz, %r3, %r2, 4, 32
	ldq	%r2, %r3, 160

	! BUNDLE
	faddsq	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end505:
	.size	test_nullify_f128_le, .Lfunc_end505-test_nullify_f128_le
	.size	.Ltest_nullify_f128_le$local, .Lfunc_end505-test_nullify_f128_le
                                        ! -- End function
	.globl	test_nullify_f128_gt            ! -- Begin function test_nullify_f128_gt
	.p2align	4
	.type	test_nullify_f128_gt,@function
test_nullify_f128_gt:                   ! @test_nullify_f128_gt
.Ltest_nullify_f128_gt$local:
	.type	.Ltest_nullify_f128_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsqole	%r1, %r2, 2, 4
	ldqr	%r2, %pcfwd(left_f128)

	! BUNDLE
	faddsq	%r1, %r2, %r1
	ldqr	%r2, %pcfwd(right_f128)
	faddsq	%r1, %r2, %r1

	! BUNDLE
	fcvtsq2id	%r2, %r1, 0
	stqx	%gz, %r3, %r2, 4, 32
	ldq	%r2, %r3, 160

	! BUNDLE
	faddsq	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end506:
	.size	test_nullify_f128_gt, .Lfunc_end506-test_nullify_f128_gt
	.size	.Ltest_nullify_f128_gt$local, .Lfunc_end506-test_nullify_f128_gt
                                        ! -- End function
	.globl	test_nullify_f128_ge            ! -- Begin function test_nullify_f128_ge
	.p2align	4
	.type	test_nullify_f128_ge,@function
test_nullify_f128_ge:                   ! @test_nullify_f128_ge
.Ltest_nullify_f128_ge$local:
	.type	.Ltest_nullify_f128_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsqolt	%r1, %r2, 2, 4
	ldqr	%r2, %pcfwd(left_f128)

	! BUNDLE
	faddsq	%r1, %r2, %r1
	ldqr	%r2, %pcfwd(right_f128)
	faddsq	%r1, %r2, %r1

	! BUNDLE
	fcvtsq2id	%r2, %r1, 0
	stqx	%gz, %r3, %r2, 4, 32
	ldq	%r2, %r3, 160

	! BUNDLE
	faddsq	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end507:
	.size	test_nullify_f128_ge, .Lfunc_end507-test_nullify_f128_ge
	.size	.Ltest_nullify_f128_ge$local, .Lfunc_end507-test_nullify_f128_ge
                                        ! -- End function
	.globl	test_nullify_f32_eq             ! -- Begin function test_nullify_f32_eq
	.p2align	4
	.type	test_nullify_f32_eq,@function
test_nullify_f32_eq:                    ! @test_nullify_f32_eq
.Ltest_nullify_f32_eq$local:
	.type	.Ltest_nullify_f32_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfssone	%r1, %r2, 2, 4
	ldwzr	%r2, %pcfwd(left_f32)

	! BUNDLE
	faddss	%r1, %r2, %r1
	ldwzr	%r2, %pcfwd(right_f32)
	faddss	%r1, %r2, %r1

	! BUNDLE
	fcvtss2id	%r2, %r1, 0
	stwx	%gz, %r3, %r2, 2, 8
	ldwz	%r2, %r3, 40

	! BUNDLE
	faddss	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end508:
	.size	test_nullify_f32_eq, .Lfunc_end508-test_nullify_f32_eq
	.size	.Ltest_nullify_f32_eq$local, .Lfunc_end508-test_nullify_f32_eq
                                        ! -- End function
	.globl	test_nullify_f32_ne             ! -- Begin function test_nullify_f32_ne
	.p2align	4
	.type	test_nullify_f32_ne,@function
test_nullify_f32_ne:                    ! @test_nullify_f32_ne
.Ltest_nullify_f32_ne$local:
	.type	.Ltest_nullify_f32_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfssoeq	%r1, %r2, 2, 4
	ldwzr	%r2, %pcfwd(left_f32)

	! BUNDLE
	faddss	%r1, %r2, %r1
	ldwzr	%r2, %pcfwd(right_f32)
	faddss	%r1, %r2, %r1

	! BUNDLE
	fcvtss2id	%r2, %r1, 0
	stwx	%gz, %r3, %r2, 2, 8
	ldwz	%r2, %r3, 40

	! BUNDLE
	faddss	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end509:
	.size	test_nullify_f32_ne, .Lfunc_end509-test_nullify_f32_ne
	.size	.Ltest_nullify_f32_ne$local, .Lfunc_end509-test_nullify_f32_ne
                                        ! -- End function
	.globl	test_nullify_f32_lt             ! -- Begin function test_nullify_f32_lt
	.p2align	4
	.type	test_nullify_f32_lt,@function
test_nullify_f32_lt:                    ! @test_nullify_f32_lt
.Ltest_nullify_f32_lt$local:
	.type	.Ltest_nullify_f32_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfssole	%r2, %r1, 2, 4
	ldwzr	%r2, %pcfwd(left_f32)

	! BUNDLE
	faddss	%r1, %r2, %r1
	ldwzr	%r2, %pcfwd(right_f32)
	faddss	%r1, %r2, %r1

	! BUNDLE
	fcvtss2id	%r2, %r1, 0
	stwx	%gz, %r3, %r2, 2, 8
	ldwz	%r2, %r3, 40

	! BUNDLE
	faddss	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end510:
	.size	test_nullify_f32_lt, .Lfunc_end510-test_nullify_f32_lt
	.size	.Ltest_nullify_f32_lt$local, .Lfunc_end510-test_nullify_f32_lt
                                        ! -- End function
	.globl	test_nullify_f32_le             ! -- Begin function test_nullify_f32_le
	.p2align	4
	.type	test_nullify_f32_le,@function
test_nullify_f32_le:                    ! @test_nullify_f32_le
.Ltest_nullify_f32_le$local:
	.type	.Ltest_nullify_f32_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfssolt	%r2, %r1, 2, 4
	ldwzr	%r2, %pcfwd(left_f32)

	! BUNDLE
	faddss	%r1, %r2, %r1
	ldwzr	%r2, %pcfwd(right_f32)
	faddss	%r1, %r2, %r1

	! BUNDLE
	fcvtss2id	%r2, %r1, 0
	stwx	%gz, %r3, %r2, 2, 8
	ldwz	%r2, %r3, 40

	! BUNDLE
	faddss	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end511:
	.size	test_nullify_f32_le, .Lfunc_end511-test_nullify_f32_le
	.size	.Ltest_nullify_f32_le$local, .Lfunc_end511-test_nullify_f32_le
                                        ! -- End function
	.globl	test_nullify_f32_gt             ! -- Begin function test_nullify_f32_gt
	.p2align	4
	.type	test_nullify_f32_gt,@function
test_nullify_f32_gt:                    ! @test_nullify_f32_gt
.Ltest_nullify_f32_gt$local:
	.type	.Ltest_nullify_f32_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfssole	%r1, %r2, 2, 4
	ldwzr	%r2, %pcfwd(left_f32)

	! BUNDLE
	faddss	%r1, %r2, %r1
	ldwzr	%r2, %pcfwd(right_f32)
	faddss	%r1, %r2, %r1

	! BUNDLE
	fcvtss2id	%r2, %r1, 0
	stwx	%gz, %r3, %r2, 2, 8
	ldwz	%r2, %r3, 40

	! BUNDLE
	faddss	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end512:
	.size	test_nullify_f32_gt, .Lfunc_end512-test_nullify_f32_gt
	.size	.Ltest_nullify_f32_gt$local, .Lfunc_end512-test_nullify_f32_gt
                                        ! -- End function
	.globl	test_nullify_f32_ge             ! -- Begin function test_nullify_f32_ge
	.p2align	4
	.type	test_nullify_f32_ge,@function
test_nullify_f32_ge:                    ! @test_nullify_f32_ge
.Ltest_nullify_f32_ge$local:
	.type	.Ltest_nullify_f32_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfssolt	%r1, %r2, 2, 4
	ldwzr	%r2, %pcfwd(left_f32)

	! BUNDLE
	faddss	%r1, %r2, %r1
	ldwzr	%r2, %pcfwd(right_f32)
	faddss	%r1, %r2, %r1

	! BUNDLE
	fcvtss2id	%r2, %r1, 0
	stwx	%gz, %r3, %r2, 2, 8
	ldwz	%r2, %r3, 40

	! BUNDLE
	faddss	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end513:
	.size	test_nullify_f32_ge, .Lfunc_end513-test_nullify_f32_ge
	.size	.Ltest_nullify_f32_ge$local, .Lfunc_end513-test_nullify_f32_ge
                                        ! -- End function
	.globl	test_nullify_f64_eq             ! -- Begin function test_nullify_f64_eq
	.p2align	4
	.type	test_nullify_f64_eq,@function
test_nullify_f64_eq:                    ! @test_nullify_f64_eq
.Ltest_nullify_f64_eq$local:
	.type	.Ltest_nullify_f64_eq$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsdone	%r1, %r2, 2, 4
	lddzr	%r2, %pcfwd(left_f64)

	! BUNDLE
	faddsd	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_f64)
	faddsd	%r1, %r2, %r1

	! BUNDLE
	fcvtsd2id	%r2, %r1, 0
	stdx	%gz, %r3, %r2, 3, 16
	lddz	%r2, %r3, 80

	! BUNDLE
	faddsd	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end514:
	.size	test_nullify_f64_eq, .Lfunc_end514-test_nullify_f64_eq
	.size	.Ltest_nullify_f64_eq$local, .Lfunc_end514-test_nullify_f64_eq
                                        ! -- End function
	.globl	test_nullify_f64_ne             ! -- Begin function test_nullify_f64_ne
	.p2align	4
	.type	test_nullify_f64_ne,@function
test_nullify_f64_ne:                    ! @test_nullify_f64_ne
.Ltest_nullify_f64_ne$local:
	.type	.Ltest_nullify_f64_ne$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsdoeq	%r1, %r2, 2, 4
	lddzr	%r2, %pcfwd(left_f64)

	! BUNDLE
	faddsd	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_f64)
	faddsd	%r1, %r2, %r1

	! BUNDLE
	fcvtsd2id	%r2, %r1, 0
	stdx	%gz, %r3, %r2, 3, 16
	lddz	%r2, %r3, 80

	! BUNDLE
	faddsd	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end515:
	.size	test_nullify_f64_ne, .Lfunc_end515-test_nullify_f64_ne
	.size	.Ltest_nullify_f64_ne$local, .Lfunc_end515-test_nullify_f64_ne
                                        ! -- End function
	.globl	test_nullify_f64_lt             ! -- Begin function test_nullify_f64_lt
	.p2align	4
	.type	test_nullify_f64_lt,@function
test_nullify_f64_lt:                    ! @test_nullify_f64_lt
.Ltest_nullify_f64_lt$local:
	.type	.Ltest_nullify_f64_lt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsdole	%r2, %r1, 2, 4
	lddzr	%r2, %pcfwd(left_f64)

	! BUNDLE
	faddsd	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_f64)
	faddsd	%r1, %r2, %r1

	! BUNDLE
	fcvtsd2id	%r2, %r1, 0
	stdx	%gz, %r3, %r2, 3, 16
	lddz	%r2, %r3, 80

	! BUNDLE
	faddsd	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end516:
	.size	test_nullify_f64_lt, .Lfunc_end516-test_nullify_f64_lt
	.size	.Ltest_nullify_f64_lt$local, .Lfunc_end516-test_nullify_f64_lt
                                        ! -- End function
	.globl	test_nullify_f64_le             ! -- Begin function test_nullify_f64_le
	.p2align	4
	.type	test_nullify_f64_le,@function
test_nullify_f64_le:                    ! @test_nullify_f64_le
.Ltest_nullify_f64_le$local:
	.type	.Ltest_nullify_f64_le$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsdolt	%r2, %r1, 2, 4
	lddzr	%r2, %pcfwd(left_f64)

	! BUNDLE
	faddsd	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_f64)
	faddsd	%r1, %r2, %r1

	! BUNDLE
	fcvtsd2id	%r2, %r1, 0
	stdx	%gz, %r3, %r2, 3, 16
	lddz	%r2, %r3, 80

	! BUNDLE
	faddsd	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end517:
	.size	test_nullify_f64_le, .Lfunc_end517-test_nullify_f64_le
	.size	.Ltest_nullify_f64_le$local, .Lfunc_end517-test_nullify_f64_le
                                        ! -- End function
	.globl	test_nullify_f64_gt             ! -- Begin function test_nullify_f64_gt
	.p2align	4
	.type	test_nullify_f64_gt,@function
test_nullify_f64_gt:                    ! @test_nullify_f64_gt
.Ltest_nullify_f64_gt$local:
	.type	.Ltest_nullify_f64_gt$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsdole	%r1, %r2, 2, 4
	lddzr	%r2, %pcfwd(left_f64)

	! BUNDLE
	faddsd	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_f64)
	faddsd	%r1, %r2, %r1

	! BUNDLE
	fcvtsd2id	%r2, %r1, 0
	stdx	%gz, %r3, %r2, 3, 16
	lddz	%r2, %r3, 80

	! BUNDLE
	faddsd	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end518:
	.size	test_nullify_f64_gt, .Lfunc_end518-test_nullify_f64_gt
	.size	.Ltest_nullify_f64_gt$local, .Lfunc_end518-test_nullify_f64_gt
                                        ! -- End function
	.globl	test_nullify_f64_ge             ! -- Begin function test_nullify_f64_ge
	.p2align	4
	.type	test_nullify_f64_ge,@function
test_nullify_f64_ge:                    ! @test_nullify_f64_ge
.Ltest_nullify_f64_ge$local:
	.type	.Ltest_nullify_f64_ge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulfsdolt	%r1, %r2, 2, 4
	lddzr	%r2, %pcfwd(left_f64)

	! BUNDLE
	faddsd	%r1, %r2, %r1
	lddzr	%r2, %pcfwd(right_f64)
	faddsd	%r1, %r2, %r1

	! BUNDLE
	fcvtsd2id	%r2, %r1, 0
	stdx	%gz, %r3, %r2, 3, 16
	lddz	%r2, %r3, 80

	! BUNDLE
	faddsd	%r1, %r2, %r1
	retf	0
	nop	0

.Lfunc_end519:
	.size	test_nullify_f64_ge, .Lfunc_end519-test_nullify_f64_ge
	.size	.Ltest_nullify_f64_ge$local, .Lfunc_end519-test_nullify_f64_ge
                                        ! -- End function
	.globl	test_sign_extend_32             ! -- Begin function test_sign_extend_32
	.p2align	4
	.type	test_sign_extend_32,@function
test_sign_extend_32:                    ! @test_sign_extend_32
.Ltest_sign_extend_32$local:
	.type	.Ltest_sign_extend_32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	srawi	%r1, %r1, 0
	retf	0

.Lfunc_end520:
	.size	test_sign_extend_32, .Lfunc_end520-test_sign_extend_32
	.size	.Ltest_sign_extend_32$local, .Lfunc_end520-test_sign_extend_32
                                        ! -- End function
	.globl	test_sign_extend_16             ! -- Begin function test_sign_extend_16
	.p2align	4
	.type	test_sign_extend_16,@function
test_sign_extend_16:                    ! @test_sign_extend_16
.Ltest_sign_extend_16$local:
	.type	.Ltest_sign_extend_16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	slsrai	%r1, %r1, 48, 48
	retf	0

.Lfunc_end521:
	.size	test_sign_extend_16, .Lfunc_end521-test_sign_extend_16
	.size	.Ltest_sign_extend_16$local, .Lfunc_end521-test_sign_extend_16
                                        ! -- End function
	.globl	test_sign_extend_8              ! -- Begin function test_sign_extend_8
	.p2align	4
	.type	test_sign_extend_8,@function
test_sign_extend_8:                     ! @test_sign_extend_8
.Ltest_sign_extend_8$local:
	.type	.Ltest_sign_extend_8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	slsrai	%r1, %r1, 56, 56
	retf	0

.Lfunc_end522:
	.size	test_sign_extend_8, .Lfunc_end522-test_sign_extend_8
	.size	.Ltest_sign_extend_8$local, .Lfunc_end522-test_sign_extend_8
                                        ! -- End function
	.globl	test_zero_extend_32             ! -- Begin function test_zero_extend_32
	.p2align	4
	.type	test_zero_extend_32,@function
test_zero_extend_32:                    ! @test_zero_extend_32
.Ltest_zero_extend_32$local:
	.type	.Ltest_zero_extend_32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	srlwi	%r1, %r1, 0
	retf	0

.Lfunc_end523:
	.size	test_zero_extend_32, .Lfunc_end523-test_zero_extend_32
	.size	.Ltest_zero_extend_32$local, .Lfunc_end523-test_zero_extend_32
                                        ! -- End function
	.globl	test_zero_extend_16             ! -- Begin function test_zero_extend_16
	.p2align	4
	.type	test_zero_extend_16,@function
test_zero_extend_16:                    ! @test_zero_extend_16
.Ltest_zero_extend_16$local:
	.type	.Ltest_zero_extend_16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi	%r1, %r1, 65535
	retf	0

.Lfunc_end524:
	.size	test_zero_extend_16, .Lfunc_end524-test_zero_extend_16
	.size	.Ltest_zero_extend_16$local, .Lfunc_end524-test_zero_extend_16
                                        ! -- End function
	.globl	test_zero_extend_8              ! -- Begin function test_zero_extend_8
	.p2align	4
	.type	test_zero_extend_8,@function
test_zero_extend_8:                     ! @test_zero_extend_8
.Ltest_zero_extend_8$local:
	.type	.Ltest_zero_extend_8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi	%r1, %r1, 255
	retf	0

.Lfunc_end525:
	.size	test_zero_extend_8, .Lfunc_end525-test_zero_extend_8
	.size	.Ltest_zero_extend_8$local, .Lfunc_end525-test_zero_extend_8
                                        ! -- End function
	.globl	test_store_based_u32            ! -- Begin function test_store_based_u32
	.p2align	4
	.type	test_store_based_u32,@function
test_store_based_u32:                   ! @test_store_based_u32
.Ltest_store_based_u32$local:
	.type	.Ltest_store_based_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	stw	%r2, %r1, 16000
	retf	0

.Lfunc_end526:
	.size	test_store_based_u32, .Lfunc_end526-test_store_based_u32
	.size	.Ltest_store_based_u32$local, .Lfunc_end526-test_store_based_u32
                                        ! -- End function
	.globl	test_store_based_u16            ! -- Begin function test_store_based_u16
	.p2align	4
	.type	test_store_based_u16,@function
test_store_based_u16:                   ! @test_store_based_u16
.Ltest_store_based_u16$local:
	.type	.Ltest_store_based_u16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	sth	%r2, %r1, 8000
	retf	0

.Lfunc_end527:
	.size	test_store_based_u16, .Lfunc_end527-test_store_based_u16
	.size	.Ltest_store_based_u16$local, .Lfunc_end527-test_store_based_u16
                                        ! -- End function
	.globl	test_store_based_u8             ! -- Begin function test_store_based_u8
	.p2align	4
	.type	test_store_based_u8,@function
test_store_based_u8:                    ! @test_store_based_u8
.Ltest_store_based_u8$local:
	.type	.Ltest_store_based_u8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	stb	%r2, %r1, 4000
	retf	0

.Lfunc_end528:
	.size	test_store_based_u8, .Lfunc_end528-test_store_based_u8
	.size	.Ltest_store_based_u8$local, .Lfunc_end528-test_store_based_u8
                                        ! -- End function
	.globl	test_store_based_i32            ! -- Begin function test_store_based_i32
	.p2align	4
	.type	test_store_based_i32,@function
test_store_based_i32:                   ! @test_store_based_i32
.Ltest_store_based_i32$local:
	.type	.Ltest_store_based_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	stw	%r2, %r1, 16000
	retf	0

.Lfunc_end529:
	.size	test_store_based_i32, .Lfunc_end529-test_store_based_i32
	.size	.Ltest_store_based_i32$local, .Lfunc_end529-test_store_based_i32
                                        ! -- End function
	.globl	test_store_based_i16            ! -- Begin function test_store_based_i16
	.p2align	4
	.type	test_store_based_i16,@function
test_store_based_i16:                   ! @test_store_based_i16
.Ltest_store_based_i16$local:
	.type	.Ltest_store_based_i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	sth	%r2, %r1, 8000
	retf	0

.Lfunc_end530:
	.size	test_store_based_i16, .Lfunc_end530-test_store_based_i16
	.size	.Ltest_store_based_i16$local, .Lfunc_end530-test_store_based_i16
                                        ! -- End function
	.globl	test_store_based_i8             ! -- Begin function test_store_based_i8
	.p2align	4
	.type	test_store_based_i8,@function
test_store_based_i8:                    ! @test_store_based_i8
.Ltest_store_based_i8$local:
	.type	.Ltest_store_based_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	stb	%r2, %r1, 4000
	retf	0

.Lfunc_end531:
	.size	test_store_based_i8, .Lfunc_end531-test_store_based_i8
	.size	.Ltest_store_based_i8$local, .Lfunc_end531-test_store_based_i8
                                        ! -- End function
	.globl	test_store_indexed_u32_u32      ! -- Begin function test_store_indexed_u32_u32
	.p2align	4
	.type	test_store_indexed_u32_u32,@function
test_store_indexed_u32_u32:             ! @test_store_indexed_u32_u32
.Ltest_store_indexed_u32_u32$local:
	.type	.Ltest_store_indexed_u32_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	divuw	%r2, %r2, %r3
	stdx.wz	%r4, %r1, %r2, 3, 0

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end532:
	.size	test_store_indexed_u32_u32, .Lfunc_end532-test_store_indexed_u32_u32
	.size	.Ltest_store_indexed_u32_u32$local, .Lfunc_end532-test_store_indexed_u32_u32
                                        ! -- End function
	.globl	test_store_indexed_i32          ! -- Begin function test_store_indexed_i32
	.p2align	4
	.type	test_store_indexed_i32,@function
test_store_indexed_i32:                 ! @test_store_indexed_i32
.Ltest_store_indexed_i32$local:
	.type	.Ltest_store_indexed_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stdx.ws	%r3, %r1, %r2, 3, 0
	retf	0

.Lfunc_end533:
	.size	test_store_indexed_i32, .Lfunc_end533-test_store_indexed_i32
	.size	.Ltest_store_indexed_i32$local, .Lfunc_end533-test_store_indexed_i32
                                        ! -- End function
	.globl	test_store_indexed_i32_offset   ! -- Begin function test_store_indexed_i32_offset
	.p2align	4
	.type	test_store_indexed_i32_offset,@function
test_store_indexed_i32_offset:          ! @test_store_indexed_i32_offset
.Ltest_store_indexed_i32_offset$local:
	.type	.Ltest_store_indexed_i32_offset$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stdx.ws	%r3, %r1, %r2, 3, 160
	retf	0

.Lfunc_end534:
	.size	test_store_indexed_i32_offset, .Lfunc_end534-test_store_indexed_i32_offset
	.size	.Ltest_store_indexed_i32_offset$local, .Lfunc_end534-test_store_indexed_i32_offset
                                        ! -- End function
	.globl	test_store_indexed_u32          ! -- Begin function test_store_indexed_u32
	.p2align	4
	.type	test_store_indexed_u32,@function
test_store_indexed_u32:                 ! @test_store_indexed_u32
.Ltest_store_indexed_u32$local:
	.type	.Ltest_store_indexed_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stdx.wz	%r3, %r1, %r2, 3, 0
	retf	0

.Lfunc_end535:
	.size	test_store_indexed_u32, .Lfunc_end535-test_store_indexed_u32
	.size	.Ltest_store_indexed_u32$local, .Lfunc_end535-test_store_indexed_u32
                                        ! -- End function
	.globl	test_store_indexed_u32_offset   ! -- Begin function test_store_indexed_u32_offset
	.p2align	4
	.type	test_store_indexed_u32_offset,@function
test_store_indexed_u32_offset:          ! @test_store_indexed_u32_offset
.Ltest_store_indexed_u32_offset$local:
	.type	.Ltest_store_indexed_u32_offset$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	addiws	%r2, %r2, 20
	stdx.wz	%r3, %r1, %r2, 3, 0

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end536:
	.size	test_store_indexed_u32_offset, .Lfunc_end536-test_store_indexed_u32_offset
	.size	.Ltest_store_indexed_u32_offset$local, .Lfunc_end536-test_store_indexed_u32_offset
                                        ! -- End function
	.globl	test_store_indexed_i64          ! -- Begin function test_store_indexed_i64
	.p2align	4
	.type	test_store_indexed_i64,@function
test_store_indexed_i64:                 ! @test_store_indexed_i64
.Ltest_store_indexed_i64$local:
	.type	.Ltest_store_indexed_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stdx	%r3, %r1, %r2, 3, 160
	retf	0

.Lfunc_end537:
	.size	test_store_indexed_i64, .Lfunc_end537-test_store_indexed_i64
	.size	.Ltest_store_indexed_i64$local, .Lfunc_end537-test_store_indexed_i64
                                        ! -- End function
	.globl	test_store_indexed_i32_struct   ! -- Begin function test_store_indexed_i32_struct
	.p2align	4
	.type	test_store_indexed_i32_struct,@function
test_store_indexed_i32_struct:          ! @test_store_indexed_i32_struct
.Ltest_store_indexed_i32_struct$local:
	.type	.Ltest_store_indexed_i32_struct$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stdx.ws.l	%r3, %r1, %r2, 4, 328

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end538:
	.size	test_store_indexed_i32_struct, .Lfunc_end538-test_store_indexed_i32_struct
	.size	.Ltest_store_indexed_i32_struct$local, .Lfunc_end538-test_store_indexed_i32_struct
                                        ! -- End function
	.globl	dummy_return                    ! -- Begin function dummy_return
	.p2align	4
	.type	dummy_return,@function
dummy_return:                           ! @dummy_return
.Ldummy_return$local:
	.type	.Ldummy_return$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	12
	ldbzr	%r1, %pcfwd(i8_zero)
	ldbzr	%r2, %pcfwd(u8_zero)

	! BUNDLE
	addiws	%r1, %r1, 1
	stbr	%r1, %pcfwd(i8_zero)
	ldhzr	%r3, %pcfwd(i16_zero)

	! BUNDLE
	addiws	%r2, %r2, 1
	stbr	%r2, %pcfwd(u8_zero)
	ldhzr	%r4, %pcfwd(u16_zero)

	! BUNDLE
	addiws	%r3, %r3, 1
	sthr	%r3, %pcfwd(i16_zero)
	ldwzr	%r5, %pcfwd(i32_zero)

	! BUNDLE
	addiws	%r4, %r4, 1
	sthr	%r4, %pcfwd(u16_zero)
	ldwzr	%r6, %pcfwd(u32_zero)

	! BUNDLE
	addiws	%r5, %r5, 1
	stwr	%r5, %pcfwd(i32_zero)
	lddzr	%r7, %pcfwd(i64_zero)

	! BUNDLE
	addiws	%r6, %r6, 1
	stwr	%r6, %pcfwd(u32_zero)
	lddzr	%r8, %pcfwd(u64_zero)

	! BUNDLE
	addi	%r9, %r7, 1
	stdr	%r9, %pcfwd(i64_zero)
	ldqr	%r9, %pcfwd(i128_zero)

	! BUNDLE
	addi	%r10, %r8, 1
	ldqr	%r11, %pcfwd(u128_zero)
	stdr	%r10, %pcfwd(u64_zero)

	! BUNDLE
	ldi	%r10, 1
	xor	%r9, %r9, %r10
	stqr	%r9, %pcfwd(i128_zero)

	! BUNDLE
	xor	%r10, %r11, %r10
	stqr	%r10, %pcfwd(u128_zero)
	slsrai	%r1, %r1, 56, 56

	! BUNDLE
	andi	%r2, %r2, 255
	addw	%r1, %r2, %r1
	slsrai	%r2, %r3, 48, 48

	! BUNDLE
	addw	%r1, %r1, %r2
	andi	%r2, %r4, 65535
	addw	%r1, %r1, %r2

	! BUNDLE
	addw	%r1, %r1, %r5
	addw	%r1, %r1, %r6
	ldax.wz	%r1, %r7, %r1, 0, 1

	! BUNDLE
	ldax	%r1, %r8, %r1, 0, 1
	srli	%r1, %r1, 0
	xor	%r1, %r9, %r1

	! BUNDLE
	xor	%r1, %r1, %r10
	retf	0
	nop	0

.Lfunc_end539:
	.size	dummy_return, .Lfunc_end539-dummy_return
	.size	.Ldummy_return$local, .Lfunc_end539-dummy_return
                                        ! -- End function
	.globl	test_store_based_i8_zero        ! -- Begin function test_store_based_i8_zero
	.p2align	4
	.type	test_store_based_i8_zero,@function
test_store_based_i8_zero:               ! @test_store_based_i8_zero
.Ltest_store_based_i8_zero$local:
	.type	.Ltest_store_based_i8_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stb	%gz, %r1, 4000
	stbr	%gz, %pcfwd(i8_zero)

	! BUNDLE
	stbx	%gz, %r1, %r2, 0, 10
	stbx.ws	%gz, %r1, %r3, 0, 10
	retf	0

.Lfunc_end540:
	.size	test_store_based_i8_zero, .Lfunc_end540-test_store_based_i8_zero
	.size	.Ltest_store_based_i8_zero$local, .Lfunc_end540-test_store_based_i8_zero
                                        ! -- End function
	.globl	test_store_based_u8_zero        ! -- Begin function test_store_based_u8_zero
	.p2align	4
	.type	test_store_based_u8_zero,@function
test_store_based_u8_zero:               ! @test_store_based_u8_zero
.Ltest_store_based_u8_zero$local:
	.type	.Ltest_store_based_u8_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stb	%gz, %r1, 4000
	stbr	%gz, %pcfwd(u8_zero)

	! BUNDLE
	stbx	%gz, %r1, %r2, 0, 10
	stbx.ws	%gz, %r1, %r3, 0, 10
	retf	0

.Lfunc_end541:
	.size	test_store_based_u8_zero, .Lfunc_end541-test_store_based_u8_zero
	.size	.Ltest_store_based_u8_zero$local, .Lfunc_end541-test_store_based_u8_zero
                                        ! -- End function
	.globl	test_store_based_i16_zero       ! -- Begin function test_store_based_i16_zero
	.p2align	4
	.type	test_store_based_i16_zero,@function
test_store_based_i16_zero:              ! @test_store_based_i16_zero
.Ltest_store_based_i16_zero$local:
	.type	.Ltest_store_based_i16_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	sth	%gz, %r1, 8000
	sthr	%gz, %pcfwd(i16_zero)

	! BUNDLE
	sthx	%gz, %r1, %r2, 1, 20
	sthx.ws	%gz, %r1, %r3, 1, 20
	retf	0

.Lfunc_end542:
	.size	test_store_based_i16_zero, .Lfunc_end542-test_store_based_i16_zero
	.size	.Ltest_store_based_i16_zero$local, .Lfunc_end542-test_store_based_i16_zero
                                        ! -- End function
	.globl	test_store_based_u16_zero       ! -- Begin function test_store_based_u16_zero
	.p2align	4
	.type	test_store_based_u16_zero,@function
test_store_based_u16_zero:              ! @test_store_based_u16_zero
.Ltest_store_based_u16_zero$local:
	.type	.Ltest_store_based_u16_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	sth	%gz, %r1, 8000
	sthr	%gz, %pcfwd(u16_zero)

	! BUNDLE
	sthx	%gz, %r1, %r2, 1, 20
	sthx.ws	%gz, %r1, %r3, 1, 20
	retf	0

.Lfunc_end543:
	.size	test_store_based_u16_zero, .Lfunc_end543-test_store_based_u16_zero
	.size	.Ltest_store_based_u16_zero$local, .Lfunc_end543-test_store_based_u16_zero
                                        ! -- End function
	.globl	test_store_based_i32_zero       ! -- Begin function test_store_based_i32_zero
	.p2align	4
	.type	test_store_based_i32_zero,@function
test_store_based_i32_zero:              ! @test_store_based_i32_zero
.Ltest_store_based_i32_zero$local:
	.type	.Ltest_store_based_i32_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stw	%gz, %r1, 16000
	stwr	%gz, %pcfwd(i32_zero)

	! BUNDLE
	stwx	%gz, %r1, %r2, 2, 40
	stwx.ws	%gz, %r1, %r3, 2, 40
	retf	0

.Lfunc_end544:
	.size	test_store_based_i32_zero, .Lfunc_end544-test_store_based_i32_zero
	.size	.Ltest_store_based_i32_zero$local, .Lfunc_end544-test_store_based_i32_zero
                                        ! -- End function
	.globl	test_store_based_u32_zero       ! -- Begin function test_store_based_u32_zero
	.p2align	4
	.type	test_store_based_u32_zero,@function
test_store_based_u32_zero:              ! @test_store_based_u32_zero
.Ltest_store_based_u32_zero$local:
	.type	.Ltest_store_based_u32_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stw	%gz, %r1, 16000
	stwr	%gz, %pcfwd(u32_zero)

	! BUNDLE
	stwx	%gz, %r1, %r2, 2, 40
	stwx.ws	%gz, %r1, %r3, 2, 40
	retf	0

.Lfunc_end545:
	.size	test_store_based_u32_zero, .Lfunc_end545-test_store_based_u32_zero
	.size	.Ltest_store_based_u32_zero$local, .Lfunc_end545-test_store_based_u32_zero
                                        ! -- End function
	.globl	test_store_based_i64_zero       ! -- Begin function test_store_based_i64_zero
	.p2align	4
	.type	test_store_based_i64_zero,@function
test_store_based_i64_zero:              ! @test_store_based_i64_zero
.Ltest_store_based_i64_zero$local:
	.type	.Ltest_store_based_i64_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	std	%gz, %r1, 32000
	stdr	%gz, %pcfwd(i64_zero)

	! BUNDLE
	stdx	%gz, %r1, %r2, 3, 80
	stdx.ws	%gz, %r1, %r3, 3, 80
	retf	0

.Lfunc_end546:
	.size	test_store_based_i64_zero, .Lfunc_end546-test_store_based_i64_zero
	.size	.Ltest_store_based_i64_zero$local, .Lfunc_end546-test_store_based_i64_zero
                                        ! -- End function
	.globl	test_store_based_u64_zero       ! -- Begin function test_store_based_u64_zero
	.p2align	4
	.type	test_store_based_u64_zero,@function
test_store_based_u64_zero:              ! @test_store_based_u64_zero
.Ltest_store_based_u64_zero$local:
	.type	.Ltest_store_based_u64_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	std	%gz, %r1, 32000
	stdr	%gz, %pcfwd(u64_zero)

	! BUNDLE
	stdx	%gz, %r1, %r2, 3, 80
	stdx.ws	%gz, %r1, %r3, 3, 80
	retf	0

.Lfunc_end547:
	.size	test_store_based_u64_zero, .Lfunc_end547-test_store_based_u64_zero
	.size	.Ltest_store_based_u64_zero$local, .Lfunc_end547-test_store_based_u64_zero
                                        ! -- End function
	.globl	test_store_based_i128_zero      ! -- Begin function test_store_based_i128_zero
	.p2align	4
	.type	test_store_based_i128_zero,@function
test_store_based_i128_zero:             ! @test_store_based_i128_zero
.Ltest_store_based_i128_zero$local:
	.type	.Ltest_store_based_i128_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stq	%gz, %r1, 64000
	stqr	%gz, %pcfwd(i128_zero)

	! BUNDLE
	stqx	%gz, %r1, %r2, 4, 160
	stqx.ws	%gz, %r1, %r3, 4, 160
	retf	0

.Lfunc_end548:
	.size	test_store_based_i128_zero, .Lfunc_end548-test_store_based_i128_zero
	.size	.Ltest_store_based_i128_zero$local, .Lfunc_end548-test_store_based_i128_zero
                                        ! -- End function
	.globl	test_store_based_u128_zero      ! -- Begin function test_store_based_u128_zero
	.p2align	4
	.type	test_store_based_u128_zero,@function
test_store_based_u128_zero:             ! @test_store_based_u128_zero
.Ltest_store_based_u128_zero$local:
	.type	.Ltest_store_based_u128_zero$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	stq	%gz, %r1, 64000
	stqr	%gz, %pcfwd(u128_zero)

	! BUNDLE
	stqx	%gz, %r1, %r2, 4, 160
	stqx.ws	%gz, %r1, %r3, 4, 160
	retf	0

.Lfunc_end549:
	.size	test_store_based_u128_zero, .Lfunc_end549-test_store_based_u128_zero
	.size	.Ltest_store_based_u128_zero$local, .Lfunc_end549-test_store_based_u128_zero
                                        ! -- End function
	.globl	test_struct_memset              ! -- Begin function test_struct_memset
	.p2align	4
	.type	test_struct_memset,@function
test_struct_memset:                     ! @test_struct_memset
.Ltest_struct_memset$local:
	.type	.Ltest_struct_memset$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	4, 240
	stq	%gz, %sp, 64
	stq	%gz, %sp, 80

	! BUNDLE
	stq	%gz, %sp, 128
	stq	%gz, %sp, 144
	stq	%gz, %sp, 208

	! BUNDLE
	std	%gz, %sp, 224
	stq	%gz, %sp, 192
	stq	%gz, %sp, 176

	! BUNDLE
	stq	%gz, %sp, 160
	stq	%gz, %sp, 112
	stq	%gz, %sp, 96

	! BUNDLE
	stq	%gz, %sp, 48
	stq	%gz, %sp, 32
	stq	%gz, %sp, 16

	! BUNDLE
	stq	%gz, %sp, 0
	ldar	%r1, %jpc(sn_write)
	std	%r1, %sp, 72

	! BUNDLE
	addi	%r1, %sp, 239
	std	%r1, %sp, 88
	ldi	%r1, -1

	! BUNDLE
	stw	%r1, %sp, 140
	stw	%r1, %sp, 144
	addi	%r3, %sp, 0

	! BUNDLE
	std	%r3, %sp, 208
	callr	%r2, %jpc(test_struct_memset_consumer)
	mov	%r1, %r3

	! BUNDLE
	retf	240
	nop	0
	nop	0

.Lfunc_end550:
	.size	test_struct_memset, .Lfunc_end550-test_struct_memset
	.size	.Ltest_struct_memset$local, .Lfunc_end550-test_struct_memset
                                        ! -- End function
	.p2align	4                               ! -- Begin function sn_write
	.type	sn_write,@function
sn_write:                               ! @sn_write
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	7
	lddz	%r2, %r1, 152
	lddz	%r4, %r1, 40

	! BUNDLE
	lddz	%r6, %r1, 56
	lddz	%r5, %r2, 8
	sub	%r4, %r4, %r6

	! BUNDLE
	minu	%r6, %r5, %r4
	lddz	%r4, %r2, 0
	nuldeq	%r6, %gz, 4, 0

	! BUNDLE
	add	%r4, %r4, %r6
	std	%r4, %r2, 0
	sub	%r5, %r5, %r6

	! BUNDLE
	std	%r5, %r2, 8
	minu	%r6, %r5, %r3
	nuldeq	%r6, %gz, 4, 0

	! BUNDLE
	add	%r4, %r4, %r6
	std	%r4, %r2, 0
	sub	%r5, %r5, %r6

	! BUNDLE
	std	%r5, %r2, 8
	stb	%gz, %r4, 0
	lddz	%r2, %r1, 88

	! BUNDLE
	std	%r2, %r1, 56
	std	%r2, %r1, 40
	mov	%r1, %r3

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end551:
	.size	sn_write, .Lfunc_end551-sn_write
                                        ! -- End function
	.globl	test_switch_table_i64           ! -- Begin function test_switch_table_i64
	.p2align	4
	.type	test_switch_table_i64,@function
test_switch_table_i64:                  ! @test_switch_table_i64
.Ltest_switch_table_i64$local:
	.type	.Ltest_switch_table_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	bdgeui	%r1, 6, .LBB552_9
	ldafr	%r2, %pcfwd(.LJTI552_0)

	! BUNDLE
	jmpt	%r2, %r1
	nop	0
	nop	0

.LBB552_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	ldafr	%r4, %pcfwd(.L.str.38)
	jmp	.LBB552_8

.LBB552_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 8
	std	%r1, %sp, 8
	ldi	%r1, 5

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.42)
	jmp	.LBB552_8
	nop	0

.LBB552_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	std	%r1, %sp, 8
	ldi	%r1, 3

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.40)
	jmp	.LBB552_8
	nop	0

.LBB552_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	std	%r1, %sp, 8
	ldi	%r1, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.41)
	jmp	.LBB552_8
	nop	0

.LBB552_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	std	%gz, %sp, 8
	ldi	%r1, 2
	ldafr	%r4, %pcfwd(.L.str.39)

	! BUNDLE
	jmp	.LBB552_8
	nop	0
	nop	0

.LBB552_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 27
	std	%r1, %sp, 8
	ldi	%r1, 6

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.43)
	nop	0
	nop	0

.LBB552_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	nop	0

.LBB552_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	retf	16
	nop	0

.Lfunc_end552:
	.size	test_switch_table_i64, .Lfunc_end552-test_switch_table_i64
	.size	.Ltest_switch_table_i64$local, .Lfunc_end552-test_switch_table_i64
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI552_0:
	.word	.LBB552_2-.LJTI552_0
	.word	.LBB552_3-.LJTI552_0
	.word	.LBB552_4-.LJTI552_0
	.word	.LBB552_5-.LJTI552_0
	.word	.LBB552_6-.LJTI552_0
	.word	.LBB552_7-.LJTI552_0
                                        ! -- End function
	.text
	.globl	test_switch_table_i32           ! -- Begin function test_switch_table_i32
	.p2align	4
	.type	test_switch_table_i32,@function
test_switch_table_i32:                  ! @test_switch_table_i32
.Ltest_switch_table_i32$local:
	.type	.Ltest_switch_table_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	bwgeui	%r1, 6, .LBB553_9
	srlwi	%r1, %r1, 0

	! BUNDLE
	ldafr	%r2, %pcfwd(.LJTI553_0)
	jmpt	%r2, %r1
	nop	0

.LBB553_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	ldafr	%r4, %pcfwd(.L.str.44)
	jmp	.LBB553_8

.LBB553_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 8
	stw	%r1, %sp, 4
	ldi	%r1, 5

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.48)
	jmp	.LBB553_8
	nop	0

.LBB553_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	stw	%r1, %sp, 4
	ldi	%r1, 3

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.46)
	jmp	.LBB553_8
	nop	0

.LBB553_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	stw	%r1, %sp, 4
	ldi	%r1, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.47)
	jmp	.LBB553_8
	nop	0

.LBB553_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%gz, %sp, 4
	ldi	%r1, 2
	ldafr	%r4, %pcfwd(.L.str.45)

	! BUNDLE
	jmp	.LBB553_8
	nop	0
	nop	0

.LBB553_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 27
	stw	%r1, %sp, 4
	ldi	%r1, 6

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.49)
	nop	0
	nop	0

.LBB553_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	nop	0

.LBB553_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	retf	16
	nop	0

.Lfunc_end553:
	.size	test_switch_table_i32, .Lfunc_end553-test_switch_table_i32
	.size	.Ltest_switch_table_i32$local, .Lfunc_end553-test_switch_table_i32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI553_0:
	.word	.LBB553_2-.LJTI553_0
	.word	.LBB553_3-.LJTI553_0
	.word	.LBB553_4-.LJTI553_0
	.word	.LBB553_5-.LJTI553_0
	.word	.LBB553_6-.LJTI553_0
	.word	.LBB553_7-.LJTI553_0
                                        ! -- End function
	.text
	.globl	test_switch_table_u32           ! -- Begin function test_switch_table_u32
	.p2align	4
	.type	test_switch_table_u32,@function
test_switch_table_u32:                  ! @test_switch_table_u32
.Ltest_switch_table_u32$local:
	.type	.Ltest_switch_table_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	bwgeui	%r1, 6, .LBB554_9
	srlwi	%r1, %r1, 0

	! BUNDLE
	ldafr	%r2, %pcfwd(.LJTI554_0)
	jmpt	%r2, %r1
	nop	0

.LBB554_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	ldafr	%r4, %pcfwd(.L.str.50)
	jmp	.LBB554_8

.LBB554_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 8
	stw	%r1, %sp, 4
	ldi	%r1, 5

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.54)
	jmp	.LBB554_8
	nop	0

.LBB554_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	stw	%r1, %sp, 4
	ldi	%r1, 3

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.52)
	jmp	.LBB554_8
	nop	0

.LBB554_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	stw	%r1, %sp, 4
	ldi	%r1, 4

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.53)
	jmp	.LBB554_8
	nop	0

.LBB554_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%gz, %sp, 4
	ldi	%r1, 2
	ldafr	%r4, %pcfwd(.L.str.51)

	! BUNDLE
	jmp	.LBB554_8
	nop	0
	nop	0

.LBB554_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 27
	stw	%r1, %sp, 4
	ldi	%r1, 6

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.55)
	nop	0
	nop	0

.LBB554_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	nop	0

.LBB554_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	retf	16
	nop	0

.Lfunc_end554:
	.size	test_switch_table_u32, .Lfunc_end554-test_switch_table_u32
	.size	.Ltest_switch_table_u32$local, .Lfunc_end554-test_switch_table_u32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI554_0:
	.word	.LBB554_2-.LJTI554_0
	.word	.LBB554_3-.LJTI554_0
	.word	.LBB554_4-.LJTI554_0
	.word	.LBB554_5-.LJTI554_0
	.word	.LBB554_6-.LJTI554_0
	.word	.LBB554_7-.LJTI554_0
                                        ! -- End function
	.text
	.globl	test_switch_table_nonzero_i64   ! -- Begin function test_switch_table_nonzero_i64
	.p2align	4
	.type	test_switch_table_nonzero_i64,@function
test_switch_table_nonzero_i64:          ! @test_switch_table_nonzero_i64
.Ltest_switch_table_nonzero_i64$local:
	.type	.Ltest_switch_table_nonzero_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	addi	%r1, %r1, -10
	bdgeui	%r1, 6, .LBB555_9

	! BUNDLE
	ldafr	%r2, %pcfwd(.LJTI555_0)
	jmpt	%r2, %r1
	nop	0

.LBB555_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 11
	ldafr	%r4, %pcfwd(.L.str.56)
	jmp	.LBB555_8

.LBB555_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 28
	std	%r1, %sp, 8
	ldi	%r1, 15

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.60)
	jmp	.LBB555_8
	nop	0

.LBB555_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 14
	std	%r1, %sp, 8
	ldi	%r1, 13

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.58)
	jmp	.LBB555_8
	nop	0

.LBB555_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 15
	std	%r1, %sp, 8
	ldi	%r1, 14

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.59)
	jmp	.LBB555_8
	nop	0

.LBB555_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 10
	std	%r1, %sp, 8
	ldi	%r1, 12

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.57)
	jmp	.LBB555_8
	nop	0

.LBB555_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 37
	std	%r1, %sp, 8
	ldi	%r1, 16

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.61)
	nop	0
	nop	0

.LBB555_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	std	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	nop	0

.LBB555_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	retf	16
	nop	0

.Lfunc_end555:
	.size	test_switch_table_nonzero_i64, .Lfunc_end555-test_switch_table_nonzero_i64
	.size	.Ltest_switch_table_nonzero_i64$local, .Lfunc_end555-test_switch_table_nonzero_i64
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI555_0:
	.word	.LBB555_2-.LJTI555_0
	.word	.LBB555_3-.LJTI555_0
	.word	.LBB555_4-.LJTI555_0
	.word	.LBB555_5-.LJTI555_0
	.word	.LBB555_6-.LJTI555_0
	.word	.LBB555_7-.LJTI555_0
                                        ! -- End function
	.text
	.globl	test_switch_table_nonzero_i32   ! -- Begin function test_switch_table_nonzero_i32
	.p2align	4
	.type	test_switch_table_nonzero_i32,@function
test_switch_table_nonzero_i32:          ! @test_switch_table_nonzero_i32
.Ltest_switch_table_nonzero_i32$local:
	.type	.Ltest_switch_table_nonzero_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	addiws	%r2, %r1, -10
	bwgeui	%r2, 6, .LBB556_9

	! BUNDLE
	addiwz	%r1, %r1, -10
	ldafr	%r2, %pcfwd(.LJTI556_0)
	jmpt	%r2, %r1

.LBB556_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 11
	ldafr	%r4, %pcfwd(.L.str.62)
	jmp	.LBB556_8

.LBB556_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 28
	stw	%r1, %sp, 4
	ldi	%r1, 15

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.66)
	jmp	.LBB556_8
	nop	0

.LBB556_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 14
	stw	%r1, %sp, 4
	ldi	%r1, 13

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.64)
	jmp	.LBB556_8
	nop	0

.LBB556_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 15
	stw	%r1, %sp, 4
	ldi	%r1, 14

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.65)
	jmp	.LBB556_8
	nop	0

.LBB556_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 10
	stw	%r1, %sp, 4
	ldi	%r1, 12

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.63)
	jmp	.LBB556_8
	nop	0

.LBB556_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 37
	stw	%r1, %sp, 4
	ldi	%r1, 16

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.67)
	nop	0
	nop	0

.LBB556_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	nop	0

.LBB556_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	retf	16
	nop	0

.Lfunc_end556:
	.size	test_switch_table_nonzero_i32, .Lfunc_end556-test_switch_table_nonzero_i32
	.size	.Ltest_switch_table_nonzero_i32$local, .Lfunc_end556-test_switch_table_nonzero_i32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI556_0:
	.word	.LBB556_2-.LJTI556_0
	.word	.LBB556_3-.LJTI556_0
	.word	.LBB556_4-.LJTI556_0
	.word	.LBB556_5-.LJTI556_0
	.word	.LBB556_6-.LJTI556_0
	.word	.LBB556_7-.LJTI556_0
                                        ! -- End function
	.text
	.globl	test_switch_table_nonzero_u32   ! -- Begin function test_switch_table_nonzero_u32
	.p2align	4
	.type	test_switch_table_nonzero_u32,@function
test_switch_table_nonzero_u32:          ! @test_switch_table_nonzero_u32
.Ltest_switch_table_nonzero_u32$local:
	.type	.Ltest_switch_table_nonzero_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	addiws	%r2, %r1, -10
	bwgeui	%r2, 6, .LBB557_9

	! BUNDLE
	addiwz	%r1, %r1, -10
	ldafr	%r2, %pcfwd(.LJTI557_0)
	jmpt	%r2, %r1

.LBB557_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 11
	ldafr	%r4, %pcfwd(.L.str.68)
	jmp	.LBB557_8

.LBB557_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 28
	stw	%r1, %sp, 4
	ldi	%r1, 15

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.72)
	jmp	.LBB557_8
	nop	0

.LBB557_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 14
	stw	%r1, %sp, 4
	ldi	%r1, 13

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.70)
	jmp	.LBB557_8
	nop	0

.LBB557_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 15
	stw	%r1, %sp, 4
	ldi	%r1, 14

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.71)
	jmp	.LBB557_8
	nop	0

.LBB557_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 10
	stw	%r1, %sp, 4
	ldi	%r1, 12

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.69)
	jmp	.LBB557_8
	nop	0

.LBB557_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 37
	stw	%r1, %sp, 4
	ldi	%r1, 16

	! BUNDLE
	ldafr	%r4, %pcfwd(.L.str.73)
	nop	0
	nop	0

.LBB557_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	stw	%r1, %sp, 0
	callr	%r3, %jpc(printf)
	nop	0

.LBB557_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	retf	16
	nop	0

.Lfunc_end557:
	.size	test_switch_table_nonzero_u32, .Lfunc_end557-test_switch_table_nonzero_u32
	.size	.Ltest_switch_table_nonzero_u32$local, .Lfunc_end557-test_switch_table_nonzero_u32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI557_0:
	.word	.LBB557_2-.LJTI557_0
	.word	.LBB557_3-.LJTI557_0
	.word	.LBB557_4-.LJTI557_0
	.word	.LBB557_5-.LJTI557_0
	.word	.LBB557_6-.LJTI557_0
	.word	.LBB557_7-.LJTI557_0
                                        ! -- End function
	.text
	.globl	test_syscall_2                  ! -- Begin function test_syscall_2
	.p2align	4
	.type	test_syscall_2,@function
test_syscall_2:                         ! @test_syscall_2
.Ltest_syscall_2$local:
	.type	.Ltest_syscall_2$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	syscall
	retf	0

.Lfunc_end558:
	.size	test_syscall_2, .Lfunc_end558-test_syscall_2
	.size	.Ltest_syscall_2$local, .Lfunc_end558-test_syscall_2
                                        ! -- End function
	.globl	test_syscall                    ! -- Begin function test_syscall
	.p2align	4
	.type	test_syscall,@function
test_syscall:                           ! @test_syscall
.Ltest_syscall$local:
	.type	.Ltest_syscall$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	1
	syscall
	sysret

	! BUNDLE
	retf	0
	nop	0
	nop	0

.Lfunc_end559:
	.size	test_syscall, .Lfunc_end559-test_syscall
	.size	.Ltest_syscall$local, .Lfunc_end559-test_syscall
                                        ! -- End function
	.section	.text.unlikely.,"ax",@progbits
	.globl	test_int                        ! -- Begin function test_int
	.p2align	4
	.type	test_int,@function
test_int:                               ! @test_int
.Ltest_int$local:
	.type	.Ltest_int$local,@function
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	int	%r1, 0
	int	%gz, 100

	! BUNDLE
	int	%gz, 200
	int	%gz, 255
	int	%gz, 99

	! BUNDLE
	int	%gz, 1
	int	%gz, 5
	nop	0

.Lfunc_end560:
	.size	test_int, .Lfunc_end560-test_int
	.size	.Ltest_int$local, .Lfunc_end560-test_int
                                        ! -- End function
	.text
	.globl	test_call_global_tailcall       ! -- Begin function test_call_global_tailcall
	.p2align	4
	.type	test_call_global_tailcall,@function
test_call_global_tailcall:              ! @test_call_global_tailcall
.Ltest_call_global_tailcall$local:
	.type	.Ltest_call_global_tailcall$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	addiws	%r3, %r1, 1
	addiws	%r4, %r1, 10

	! BUNDLE
	callr	%r2, %jpc(tail_call)
	mov	%r1, %r3
	retf	0

.Lfunc_end561:
	.size	test_call_global_tailcall, .Lfunc_end561-test_call_global_tailcall
	.size	.Ltest_call_global_tailcall$local, .Lfunc_end561-test_call_global_tailcall
                                        ! -- End function
	.globl	test_ptr_tailcall               ! -- Begin function test_ptr_tailcall
	.p2align	4
	.type	test_ptr_tailcall,@function
test_ptr_tailcall:                      ! @test_ptr_tailcall
.Ltest_ptr_tailcall$local:
	.type	.Ltest_ptr_tailcall$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	5
	mov	%r4, %r2
	callmi	%r3, %r1, 48

	! BUNDLE
	mov	%r1, %r4
	retf	0
	nop	0

.Lfunc_end562:
	.size	test_ptr_tailcall, .Lfunc_end562-test_ptr_tailcall
	.size	.Ltest_ptr_tailcall$local, .Lfunc_end562-test_ptr_tailcall
                                        ! -- End function
	.globl	test_call_global_ptr_table_tailcall ! -- Begin function test_call_global_ptr_table_tailcall
	.p2align	4
	.type	test_call_global_ptr_table_tailcall,@function
test_call_global_ptr_table_tailcall:    ! @test_call_global_ptr_table_tailcall
.Ltest_call_global_ptr_table_tailcall$local:
	.type	.Ltest_call_global_ptr_table_tailcall$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	mov	%r3, %r1
	callplt	%r2, %pcfwd(global_ftable+40)

	! BUNDLE
	mov	%r1, %r3
	retf	0
	nop	0

.Lfunc_end563:
	.size	test_call_global_ptr_table_tailcall, .Lfunc_end563-test_call_global_ptr_table_tailcall
	.size	.Ltest_call_global_ptr_table_tailcall$local, .Lfunc_end563-test_call_global_ptr_table_tailcall
                                        ! -- End function
	.globl	test_builtin_tlb_purge          ! -- Begin function test_builtin_tlb_purge
	.p2align	4
	.type	test_builtin_tlb_purge,@function
test_builtin_tlb_purge:                 ! @test_builtin_tlb_purge
.Ltest_builtin_tlb_purge$local:
	.type	.Ltest_builtin_tlb_purge$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	ptc	%r1, %r2, %r3
	retf	0

.Lfunc_end564:
	.size	test_builtin_tlb_purge, .Lfunc_end564-test_builtin_tlb_purge
	.size	.Ltest_builtin_tlb_purge$local, .Lfunc_end564-test_builtin_tlb_purge
                                        ! -- End function
	.globl	test_local_tls                  ! -- Begin function test_local_tls
	.p2align	4
	.type	test_local_tls,@function
test_local_tls:                         ! @test_local_tls
.Ltest_local_tls$local:
	.type	.Ltest_local_tls$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	ldafr	%r4, %tld(x_tls)
	callr	%r3, %jpc(__tls_get_addr)

	! BUNDLE
	lddz	%r2, %r4, %dtpoff(x_tls)
	add	%r1, %r2, %r1
	retf	16

.Lfunc_end565:
	.size	test_local_tls, .Lfunc_end565-test_local_tls
	.size	.Ltest_local_tls$local, .Lfunc_end565-test_local_tls
                                        ! -- End function
	.globl	test_extern_tls                 ! -- Begin function test_extern_tls
	.p2align	4
	.type	test_extern_tls,@function
test_extern_tls:                        ! @test_extern_tls
.Ltest_extern_tls$local:
	.type	.Ltest_extern_tls$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	ldafr	%r4, %tld(x_tls_extern)
	callr	%r3, %jpc(__tls_get_addr)

	! BUNDLE
	lddz	%r2, %r4, %dtpoff(x_tls_extern)
	add	%r1, %r2, %r1
	retf	16

.Lfunc_end566:
	.size	test_extern_tls, .Lfunc_end566-test_extern_tls
	.size	.Ltest_extern_tls$local, .Lfunc_end566-test_extern_tls
                                        ! -- End function
	.globl	test_va_start                   ! -- Begin function test_va_start
	.p2align	4
	.type	test_va_start,@function
test_va_start:                          ! @test_va_start
.Ltest_va_start$local:
	.type	.Ltest_va_start$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	7, 144
	addi	%r6, %sp, 144
	std	%r6, %sp, 8

	! BUNDLE
	addiws	%r3, %r1, 1
	addiws	%r4, %r1, -1
	addi	%r5, %sp, 16

	! BUNDLE
	callr	%r2, %jpc(test_va_list_callback)
	mov	%r1, %r3
	retf	144

.Lfunc_end567:
	.size	test_va_start, .Lfunc_end567-test_va_start
	.size	.Ltest_va_start$local, .Lfunc_end567-test_va_start
                                        ! -- End function
	.globl	test_va_start_end               ! -- Begin function test_va_start_end
	.p2align	4
	.type	test_va_start_end,@function
test_va_start_end:                      ! @test_va_start_end
.Ltest_va_start_end$local:
	.type	.Ltest_va_start_end$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	8, 144
	addi	%r1, %sp, 144
	std	%r1, %sp, 8

	! BUNDLE
	nop	1000
	lddz	%r1, %sp, 8
	algnup	%r1, %r1, 0, 2

	! BUNDLE
	addi	%r2, %r1, 4
	std	%r2, %sp, 8
	ldwz	%r4, %r1, 0

	! BUNDLE
	nop	1001
	lddz	%r1, %sp, 8
	algnup	%r1, %r1, 0, 3

	! BUNDLE
	addi	%r2, %r1, 8
	std	%r2, %sp, 8
	lddz	%r5, %r1, 0

	! BUNDLE
	nop	1002
	nop	1003
	addi	%r6, %sp, 16

	! BUNDLE
	addi	%r7, %sp, 8
	callr	%r3, %jpc(test_va_callback)
	mov	%r1, %r4

	! BUNDLE
	retf	144
	nop	0
	nop	0

.Lfunc_end568:
	.size	test_va_start_end, .Lfunc_end568-test_va_start_end
	.size	.Ltest_va_start_end$local, .Lfunc_end568-test_va_start_end
                                        ! -- End function
	.globl	test_printf_valist              ! -- Begin function test_printf_valist
	.p2align	4
	.type	test_printf_valist,@function
test_printf_valist:                     ! @test_printf_valist
.Ltest_printf_valist$local:
	.type	.Ltest_printf_valist$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	6, 16
	std	%r2, %sp, 8
	std	%r2, %sp, 0

	! BUNDLE
	ldi	%r4, 100
	addi	%r1, %sp, 0
	mov	%r5, %r1

	! BUNDLE
	callr	%r3, %jpc(test_printf_callback)
	ldi	%r4, 200
	mov	%r5, %r1

	! BUNDLE
	callr	%r3, %jpc(test_printf_callback)
	mov	%r1, %r4
	retf	16

.Lfunc_end569:
	.size	test_printf_valist, .Lfunc_end569-test_printf_valist
	.size	.Ltest_printf_valist$local, .Lfunc_end569-test_printf_valist
                                        ! -- End function
	.globl	test_vprintf_valist             ! -- Begin function test_vprintf_valist
	.p2align	4
	.type	test_vprintf_valist,@function
test_vprintf_valist:                    ! @test_vprintf_valist
.Ltest_vprintf_valist$local:
	.type	.Ltest_vprintf_valist$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	6, 32
	std	%r2, %sp, 24
	std	%r2, %sp, 16

	! BUNDLE
	std	%r2, %sp, 8
	ldi	%r4, 100
	addi	%r5, %sp, 16

	! BUNDLE
	callr	%r3, %jpc(test_vprintf_callback)
	ldi	%r4, 200
	addi	%r5, %sp, 8

	! BUNDLE
	callr	%r3, %jpc(test_vprintf_callback)
	mov	%r1, %r4
	retf	32

.Lfunc_end570:
	.size	test_vprintf_valist, .Lfunc_end570-test_vprintf_valist
	.size	.Ltest_vprintf_valist$local, .Lfunc_end570-test_vprintf_valist
                                        ! -- End function
	.globl	test_vacopy                     ! -- Begin function test_vacopy
	.p2align	4
	.type	test_vacopy,@function
test_vacopy:                            ! @test_vacopy
.Ltest_vacopy$local:
	.type	.Ltest_vacopy$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	5, 16
	std	%r2, %sp, 8
	std	%r2, %sp, 0

	! BUNDLE
	addi	%r1, %sp, 0
	mov	%r4, %r1
	callr	%r3, %jpc(test_valist_callback)

	! BUNDLE
	mov	%r4, %r1
	callr	%r3, %jpc(test_valist_callback)
	mov	%r1, %r4

	! BUNDLE
	retf	16
	nop	0
	nop	0

.Lfunc_end571:
	.size	test_vacopy, .Lfunc_end571-test_vacopy
	.size	.Ltest_vacopy$local, .Lfunc_end571-test_vacopy
                                        ! -- End function
	.globl	test_vector_bitcast_v2i64_to_v4i32 ! -- Begin function test_vector_bitcast_v2i64_to_v4i32
	.p2align	4
	.type	test_vector_bitcast_v2i64_to_v4i32,@function
test_vector_bitcast_v2i64_to_v4i32:     ! @test_vector_bitcast_v2i64_to_v4i32
.Ltest_vector_bitcast_v2i64_to_v4i32$local:
	.type	.Ltest_vector_bitcast_v2i64_to_v4i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end572:
	.size	test_vector_bitcast_v2i64_to_v4i32, .Lfunc_end572-test_vector_bitcast_v2i64_to_v4i32
	.size	.Ltest_vector_bitcast_v2i64_to_v4i32$local, .Lfunc_end572-test_vector_bitcast_v2i64_to_v4i32
                                        ! -- End function
	.globl	test_vector_bitcast_v2i64_to_v8i16 ! -- Begin function test_vector_bitcast_v2i64_to_v8i16
	.p2align	4
	.type	test_vector_bitcast_v2i64_to_v8i16,@function
test_vector_bitcast_v2i64_to_v8i16:     ! @test_vector_bitcast_v2i64_to_v8i16
.Ltest_vector_bitcast_v2i64_to_v8i16$local:
	.type	.Ltest_vector_bitcast_v2i64_to_v8i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end573:
	.size	test_vector_bitcast_v2i64_to_v8i16, .Lfunc_end573-test_vector_bitcast_v2i64_to_v8i16
	.size	.Ltest_vector_bitcast_v2i64_to_v8i16$local, .Lfunc_end573-test_vector_bitcast_v2i64_to_v8i16
                                        ! -- End function
	.globl	test_vector_bitcast_v2i64_to_v16i8 ! -- Begin function test_vector_bitcast_v2i64_to_v16i8
	.p2align	4
	.type	test_vector_bitcast_v2i64_to_v16i8,@function
test_vector_bitcast_v2i64_to_v16i8:     ! @test_vector_bitcast_v2i64_to_v16i8
.Ltest_vector_bitcast_v2i64_to_v16i8$local:
	.type	.Ltest_vector_bitcast_v2i64_to_v16i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end574:
	.size	test_vector_bitcast_v2i64_to_v16i8, .Lfunc_end574-test_vector_bitcast_v2i64_to_v16i8
	.size	.Ltest_vector_bitcast_v2i64_to_v16i8$local, .Lfunc_end574-test_vector_bitcast_v2i64_to_v16i8
                                        ! -- End function
	.globl	test_vector_bitcast_v4i32_to_v2i64 ! -- Begin function test_vector_bitcast_v4i32_to_v2i64
	.p2align	4
	.type	test_vector_bitcast_v4i32_to_v2i64,@function
test_vector_bitcast_v4i32_to_v2i64:     ! @test_vector_bitcast_v4i32_to_v2i64
.Ltest_vector_bitcast_v4i32_to_v2i64$local:
	.type	.Ltest_vector_bitcast_v4i32_to_v2i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end575:
	.size	test_vector_bitcast_v4i32_to_v2i64, .Lfunc_end575-test_vector_bitcast_v4i32_to_v2i64
	.size	.Ltest_vector_bitcast_v4i32_to_v2i64$local, .Lfunc_end575-test_vector_bitcast_v4i32_to_v2i64
                                        ! -- End function
	.globl	test_vector_bitcast_v4i32_to_v8i16 ! -- Begin function test_vector_bitcast_v4i32_to_v8i16
	.p2align	4
	.type	test_vector_bitcast_v4i32_to_v8i16,@function
test_vector_bitcast_v4i32_to_v8i16:     ! @test_vector_bitcast_v4i32_to_v8i16
.Ltest_vector_bitcast_v4i32_to_v8i16$local:
	.type	.Ltest_vector_bitcast_v4i32_to_v8i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end576:
	.size	test_vector_bitcast_v4i32_to_v8i16, .Lfunc_end576-test_vector_bitcast_v4i32_to_v8i16
	.size	.Ltest_vector_bitcast_v4i32_to_v8i16$local, .Lfunc_end576-test_vector_bitcast_v4i32_to_v8i16
                                        ! -- End function
	.globl	test_vector_bitcast_v4i32_to_v16i8 ! -- Begin function test_vector_bitcast_v4i32_to_v16i8
	.p2align	4
	.type	test_vector_bitcast_v4i32_to_v16i8,@function
test_vector_bitcast_v4i32_to_v16i8:     ! @test_vector_bitcast_v4i32_to_v16i8
.Ltest_vector_bitcast_v4i32_to_v16i8$local:
	.type	.Ltest_vector_bitcast_v4i32_to_v16i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end577:
	.size	test_vector_bitcast_v4i32_to_v16i8, .Lfunc_end577-test_vector_bitcast_v4i32_to_v16i8
	.size	.Ltest_vector_bitcast_v4i32_to_v16i8$local, .Lfunc_end577-test_vector_bitcast_v4i32_to_v16i8
                                        ! -- End function
	.globl	test_vector_bitcast_v8i16_to_v2i64 ! -- Begin function test_vector_bitcast_v8i16_to_v2i64
	.p2align	4
	.type	test_vector_bitcast_v8i16_to_v2i64,@function
test_vector_bitcast_v8i16_to_v2i64:     ! @test_vector_bitcast_v8i16_to_v2i64
.Ltest_vector_bitcast_v8i16_to_v2i64$local:
	.type	.Ltest_vector_bitcast_v8i16_to_v2i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end578:
	.size	test_vector_bitcast_v8i16_to_v2i64, .Lfunc_end578-test_vector_bitcast_v8i16_to_v2i64
	.size	.Ltest_vector_bitcast_v8i16_to_v2i64$local, .Lfunc_end578-test_vector_bitcast_v8i16_to_v2i64
                                        ! -- End function
	.globl	test_vector_bitcast_v8i16_to_v4i32 ! -- Begin function test_vector_bitcast_v8i16_to_v4i32
	.p2align	4
	.type	test_vector_bitcast_v8i16_to_v4i32,@function
test_vector_bitcast_v8i16_to_v4i32:     ! @test_vector_bitcast_v8i16_to_v4i32
.Ltest_vector_bitcast_v8i16_to_v4i32$local:
	.type	.Ltest_vector_bitcast_v8i16_to_v4i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end579:
	.size	test_vector_bitcast_v8i16_to_v4i32, .Lfunc_end579-test_vector_bitcast_v8i16_to_v4i32
	.size	.Ltest_vector_bitcast_v8i16_to_v4i32$local, .Lfunc_end579-test_vector_bitcast_v8i16_to_v4i32
                                        ! -- End function
	.globl	test_vector_bitcast_v8i16_to_v16i8 ! -- Begin function test_vector_bitcast_v8i16_to_v16i8
	.p2align	4
	.type	test_vector_bitcast_v8i16_to_v16i8,@function
test_vector_bitcast_v8i16_to_v16i8:     ! @test_vector_bitcast_v8i16_to_v16i8
.Ltest_vector_bitcast_v8i16_to_v16i8$local:
	.type	.Ltest_vector_bitcast_v8i16_to_v16i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end580:
	.size	test_vector_bitcast_v8i16_to_v16i8, .Lfunc_end580-test_vector_bitcast_v8i16_to_v16i8
	.size	.Ltest_vector_bitcast_v8i16_to_v16i8$local, .Lfunc_end580-test_vector_bitcast_v8i16_to_v16i8
                                        ! -- End function
	.globl	test_vector_bitcast_v16i8_to_v2i64 ! -- Begin function test_vector_bitcast_v16i8_to_v2i64
	.p2align	4
	.type	test_vector_bitcast_v16i8_to_v2i64,@function
test_vector_bitcast_v16i8_to_v2i64:     ! @test_vector_bitcast_v16i8_to_v2i64
.Ltest_vector_bitcast_v16i8_to_v2i64$local:
	.type	.Ltest_vector_bitcast_v16i8_to_v2i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end581:
	.size	test_vector_bitcast_v16i8_to_v2i64, .Lfunc_end581-test_vector_bitcast_v16i8_to_v2i64
	.size	.Ltest_vector_bitcast_v16i8_to_v2i64$local, .Lfunc_end581-test_vector_bitcast_v16i8_to_v2i64
                                        ! -- End function
	.globl	test_vector_bitcast_v16i8_to_v4i32 ! -- Begin function test_vector_bitcast_v16i8_to_v4i32
	.p2align	4
	.type	test_vector_bitcast_v16i8_to_v4i32,@function
test_vector_bitcast_v16i8_to_v4i32:     ! @test_vector_bitcast_v16i8_to_v4i32
.Ltest_vector_bitcast_v16i8_to_v4i32$local:
	.type	.Ltest_vector_bitcast_v16i8_to_v4i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end582:
	.size	test_vector_bitcast_v16i8_to_v4i32, .Lfunc_end582-test_vector_bitcast_v16i8_to_v4i32
	.size	.Ltest_vector_bitcast_v16i8_to_v4i32$local, .Lfunc_end582-test_vector_bitcast_v16i8_to_v4i32
                                        ! -- End function
	.globl	test_vector_bitcast_v16i8_to_v8i16 ! -- Begin function test_vector_bitcast_v16i8_to_v8i16
	.p2align	4
	.type	test_vector_bitcast_v16i8_to_v8i16,@function
test_vector_bitcast_v16i8_to_v8i16:     ! @test_vector_bitcast_v16i8_to_v8i16
.Ltest_vector_bitcast_v16i8_to_v8i16$local:
	.type	.Ltest_vector_bitcast_v16i8_to_v8i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end583:
	.size	test_vector_bitcast_v16i8_to_v8i16, .Lfunc_end583-test_vector_bitcast_v16i8_to_v8i16
	.size	.Ltest_vector_bitcast_v16i8_to_v8i16$local, .Lfunc_end583-test_vector_bitcast_v16i8_to_v8i16
                                        ! -- End function
	.globl	test_vector_bitcast_f128_to_v16i8 ! -- Begin function test_vector_bitcast_f128_to_v16i8
	.p2align	4
	.type	test_vector_bitcast_f128_to_v16i8,@function
test_vector_bitcast_f128_to_v16i8:      ! @test_vector_bitcast_f128_to_v16i8
.Ltest_vector_bitcast_f128_to_v16i8$local:
	.type	.Ltest_vector_bitcast_f128_to_v16i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end584:
	.size	test_vector_bitcast_f128_to_v16i8, .Lfunc_end584-test_vector_bitcast_f128_to_v16i8
	.size	.Ltest_vector_bitcast_f128_to_v16i8$local, .Lfunc_end584-test_vector_bitcast_f128_to_v16i8
                                        ! -- End function
	.globl	test_vector_bitcast_f128_to_v8i16 ! -- Begin function test_vector_bitcast_f128_to_v8i16
	.p2align	4
	.type	test_vector_bitcast_f128_to_v8i16,@function
test_vector_bitcast_f128_to_v8i16:      ! @test_vector_bitcast_f128_to_v8i16
.Ltest_vector_bitcast_f128_to_v8i16$local:
	.type	.Ltest_vector_bitcast_f128_to_v8i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end585:
	.size	test_vector_bitcast_f128_to_v8i16, .Lfunc_end585-test_vector_bitcast_f128_to_v8i16
	.size	.Ltest_vector_bitcast_f128_to_v8i16$local, .Lfunc_end585-test_vector_bitcast_f128_to_v8i16
                                        ! -- End function
	.globl	test_vector_bitcast_f128_to_v4i32 ! -- Begin function test_vector_bitcast_f128_to_v4i32
	.p2align	4
	.type	test_vector_bitcast_f128_to_v4i32,@function
test_vector_bitcast_f128_to_v4i32:      ! @test_vector_bitcast_f128_to_v4i32
.Ltest_vector_bitcast_f128_to_v4i32$local:
	.type	.Ltest_vector_bitcast_f128_to_v4i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end586:
	.size	test_vector_bitcast_f128_to_v4i32, .Lfunc_end586-test_vector_bitcast_f128_to_v4i32
	.size	.Ltest_vector_bitcast_f128_to_v4i32$local, .Lfunc_end586-test_vector_bitcast_f128_to_v4i32
                                        ! -- End function
	.globl	test_vector_bitcast_f128_to_v2i64 ! -- Begin function test_vector_bitcast_f128_to_v2i64
	.p2align	4
	.type	test_vector_bitcast_f128_to_v2i64,@function
test_vector_bitcast_f128_to_v2i64:      ! @test_vector_bitcast_f128_to_v2i64
.Ltest_vector_bitcast_f128_to_v2i64$local:
	.type	.Ltest_vector_bitcast_f128_to_v2i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end587:
	.size	test_vector_bitcast_f128_to_v2i64, .Lfunc_end587-test_vector_bitcast_f128_to_v2i64
	.size	.Ltest_vector_bitcast_f128_to_v2i64$local, .Lfunc_end587-test_vector_bitcast_f128_to_v2i64
                                        ! -- End function
	.globl	test_vector_bitcast_v16i8_to_f128 ! -- Begin function test_vector_bitcast_v16i8_to_f128
	.p2align	4
	.type	test_vector_bitcast_v16i8_to_f128,@function
test_vector_bitcast_v16i8_to_f128:      ! @test_vector_bitcast_v16i8_to_f128
.Ltest_vector_bitcast_v16i8_to_f128$local:
	.type	.Ltest_vector_bitcast_v16i8_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end588:
	.size	test_vector_bitcast_v16i8_to_f128, .Lfunc_end588-test_vector_bitcast_v16i8_to_f128
	.size	.Ltest_vector_bitcast_v16i8_to_f128$local, .Lfunc_end588-test_vector_bitcast_v16i8_to_f128
                                        ! -- End function
	.globl	test_vector_bitcast_v8i16_to_f128 ! -- Begin function test_vector_bitcast_v8i16_to_f128
	.p2align	4
	.type	test_vector_bitcast_v8i16_to_f128,@function
test_vector_bitcast_v8i16_to_f128:      ! @test_vector_bitcast_v8i16_to_f128
.Ltest_vector_bitcast_v8i16_to_f128$local:
	.type	.Ltest_vector_bitcast_v8i16_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end589:
	.size	test_vector_bitcast_v8i16_to_f128, .Lfunc_end589-test_vector_bitcast_v8i16_to_f128
	.size	.Ltest_vector_bitcast_v8i16_to_f128$local, .Lfunc_end589-test_vector_bitcast_v8i16_to_f128
                                        ! -- End function
	.globl	test_vector_bitcast_v4i32_to_f128 ! -- Begin function test_vector_bitcast_v4i32_to_f128
	.p2align	4
	.type	test_vector_bitcast_v4i32_to_f128,@function
test_vector_bitcast_v4i32_to_f128:      ! @test_vector_bitcast_v4i32_to_f128
.Ltest_vector_bitcast_v4i32_to_f128$local:
	.type	.Ltest_vector_bitcast_v4i32_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end590:
	.size	test_vector_bitcast_v4i32_to_f128, .Lfunc_end590-test_vector_bitcast_v4i32_to_f128
	.size	.Ltest_vector_bitcast_v4i32_to_f128$local, .Lfunc_end590-test_vector_bitcast_v4i32_to_f128
                                        ! -- End function
	.globl	test_vector_bitcast_v2i64_to_f128 ! -- Begin function test_vector_bitcast_v2i64_to_f128
	.p2align	4
	.type	test_vector_bitcast_v2i64_to_f128,@function
test_vector_bitcast_v2i64_to_f128:      ! @test_vector_bitcast_v2i64_to_f128
.Ltest_vector_bitcast_v2i64_to_f128$local:
	.type	.Ltest_vector_bitcast_v2i64_to_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end591:
	.size	test_vector_bitcast_v2i64_to_f128, .Lfunc_end591-test_vector_bitcast_v2i64_to_f128
	.size	.Ltest_vector_bitcast_v2i64_to_f128$local, .Lfunc_end591-test_vector_bitcast_v2i64_to_f128
                                        ! -- End function
	.globl	test_vector_bitcast_i128_to_v16i8 ! -- Begin function test_vector_bitcast_i128_to_v16i8
	.p2align	4
	.type	test_vector_bitcast_i128_to_v16i8,@function
test_vector_bitcast_i128_to_v16i8:      ! @test_vector_bitcast_i128_to_v16i8
.Ltest_vector_bitcast_i128_to_v16i8$local:
	.type	.Ltest_vector_bitcast_i128_to_v16i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end592:
	.size	test_vector_bitcast_i128_to_v16i8, .Lfunc_end592-test_vector_bitcast_i128_to_v16i8
	.size	.Ltest_vector_bitcast_i128_to_v16i8$local, .Lfunc_end592-test_vector_bitcast_i128_to_v16i8
                                        ! -- End function
	.globl	test_vector_bitcast_i128_to_v8i16 ! -- Begin function test_vector_bitcast_i128_to_v8i16
	.p2align	4
	.type	test_vector_bitcast_i128_to_v8i16,@function
test_vector_bitcast_i128_to_v8i16:      ! @test_vector_bitcast_i128_to_v8i16
.Ltest_vector_bitcast_i128_to_v8i16$local:
	.type	.Ltest_vector_bitcast_i128_to_v8i16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end593:
	.size	test_vector_bitcast_i128_to_v8i16, .Lfunc_end593-test_vector_bitcast_i128_to_v8i16
	.size	.Ltest_vector_bitcast_i128_to_v8i16$local, .Lfunc_end593-test_vector_bitcast_i128_to_v8i16
                                        ! -- End function
	.globl	test_vector_bitcast_i128_to_v4i32 ! -- Begin function test_vector_bitcast_i128_to_v4i32
	.p2align	4
	.type	test_vector_bitcast_i128_to_v4i32,@function
test_vector_bitcast_i128_to_v4i32:      ! @test_vector_bitcast_i128_to_v4i32
.Ltest_vector_bitcast_i128_to_v4i32$local:
	.type	.Ltest_vector_bitcast_i128_to_v4i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end594:
	.size	test_vector_bitcast_i128_to_v4i32, .Lfunc_end594-test_vector_bitcast_i128_to_v4i32
	.size	.Ltest_vector_bitcast_i128_to_v4i32$local, .Lfunc_end594-test_vector_bitcast_i128_to_v4i32
                                        ! -- End function
	.globl	test_vector_bitcast_i128_to_v2i64 ! -- Begin function test_vector_bitcast_i128_to_v2i64
	.p2align	4
	.type	test_vector_bitcast_i128_to_v2i64,@function
test_vector_bitcast_i128_to_v2i64:      ! @test_vector_bitcast_i128_to_v2i64
.Ltest_vector_bitcast_i128_to_v2i64$local:
	.type	.Ltest_vector_bitcast_i128_to_v2i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end595:
	.size	test_vector_bitcast_i128_to_v2i64, .Lfunc_end595-test_vector_bitcast_i128_to_v2i64
	.size	.Ltest_vector_bitcast_i128_to_v2i64$local, .Lfunc_end595-test_vector_bitcast_i128_to_v2i64
                                        ! -- End function
	.globl	test_vector_bitcast_v16i8_to_i128 ! -- Begin function test_vector_bitcast_v16i8_to_i128
	.p2align	4
	.type	test_vector_bitcast_v16i8_to_i128,@function
test_vector_bitcast_v16i8_to_i128:      ! @test_vector_bitcast_v16i8_to_i128
.Ltest_vector_bitcast_v16i8_to_i128$local:
	.type	.Ltest_vector_bitcast_v16i8_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end596:
	.size	test_vector_bitcast_v16i8_to_i128, .Lfunc_end596-test_vector_bitcast_v16i8_to_i128
	.size	.Ltest_vector_bitcast_v16i8_to_i128$local, .Lfunc_end596-test_vector_bitcast_v16i8_to_i128
                                        ! -- End function
	.globl	test_vector_bitcast_v8i16_to_i128 ! -- Begin function test_vector_bitcast_v8i16_to_i128
	.p2align	4
	.type	test_vector_bitcast_v8i16_to_i128,@function
test_vector_bitcast_v8i16_to_i128:      ! @test_vector_bitcast_v8i16_to_i128
.Ltest_vector_bitcast_v8i16_to_i128$local:
	.type	.Ltest_vector_bitcast_v8i16_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end597:
	.size	test_vector_bitcast_v8i16_to_i128, .Lfunc_end597-test_vector_bitcast_v8i16_to_i128
	.size	.Ltest_vector_bitcast_v8i16_to_i128$local, .Lfunc_end597-test_vector_bitcast_v8i16_to_i128
                                        ! -- End function
	.globl	test_vector_bitcast_v4i32_to_i128 ! -- Begin function test_vector_bitcast_v4i32_to_i128
	.p2align	4
	.type	test_vector_bitcast_v4i32_to_i128,@function
test_vector_bitcast_v4i32_to_i128:      ! @test_vector_bitcast_v4i32_to_i128
.Ltest_vector_bitcast_v4i32_to_i128$local:
	.type	.Ltest_vector_bitcast_v4i32_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end598:
	.size	test_vector_bitcast_v4i32_to_i128, .Lfunc_end598-test_vector_bitcast_v4i32_to_i128
	.size	.Ltest_vector_bitcast_v4i32_to_i128$local, .Lfunc_end598-test_vector_bitcast_v4i32_to_i128
                                        ! -- End function
	.globl	test_vector_bitcast_v2i64_to_i128 ! -- Begin function test_vector_bitcast_v2i64_to_i128
	.p2align	4
	.type	test_vector_bitcast_v2i64_to_i128,@function
test_vector_bitcast_v2i64_to_i128:      ! @test_vector_bitcast_v2i64_to_i128
.Ltest_vector_bitcast_v2i64_to_i128$local:
	.type	.Ltest_vector_bitcast_v2i64_to_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	retf	0
	nop	0

.Lfunc_end599:
	.size	test_vector_bitcast_v2i64_to_i128, .Lfunc_end599-test_vector_bitcast_v2i64_to_i128
	.size	.Ltest_vector_bitcast_v2i64_to_i128$local, .Lfunc_end599-test_vector_bitcast_v2i64_to_i128
                                        ! -- End function
	.globl	test_stack_array                ! -- Begin function test_stack_array
	.p2align	4
	.type	test_stack_array,@function
test_stack_array:                       ! @test_stack_array
.Ltest_stack_array$local:
	.type	.Ltest_stack_array$local,@function
	.register %fp, #scratch
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	allocsp	13, 16
	mov	%r5, %fp
	addi	%fp, %sp, 16

	! BUNDLE
	addi	%sp, %sp, -16
	ldafr	%r12, %pcfwd(.L.str.74)
	std	%r1, %sp, 0

	! BUNDLE
	callr	%r11, %jpc(printf)
	addi	%sp, %sp, 16
	algnup	%r4, %r1, 3, 4

	! BUNDLE
	sub	%r4, %sp, %r4
	mov	%sp, %r4
	add	%r9, %r2, %r1

	! BUNDLE
	ldax.l	%r8, %r9, %r3, 0, 333
	sub	%r3, %r1, %r2

	! BUNDLE
	addi	%r10, %r3, 44
	ldax	%r11, %r2, %r1, 1, 0
	ldax	%r12, %r1, %r2, 1, 0

	! BUNDLE
	mov	%r7, %r4
	callr	%r6, %jpc(stack_array_consumer)
	std	%r7, %fp, -8

	! BUNDLE
	lddz	%r3, %r4, 0
	lddz	%r4, %fp, -8
	addadd	%r2, %r3, %r2, %r4

	! BUNDLE
	std	%r2, %fp, -8
	lddz	%r2, %fp, -8
	addi	%sp, %sp, -16

	! BUNDLE
	ldafr	%r12, %pcfwd(.L.str.75)
	std	%r2, %sp, 0
	callr	%r11, %jpc(printf)

	! BUNDLE
	addi	%sp, %sp, 16
	lddz	%r2, %fp, -8
	add	%r1, %r2, %r1

	! BUNDLE
	mov	%sp, %fp
	mov	%fp, %r5
	retf	0

.Lfunc_end600:
	.size	test_stack_array, .Lfunc_end600-test_stack_array
	.size	.Ltest_stack_array$local, .Lfunc_end600-test_stack_array
                                        ! -- End function
	.type	.L.str.1,@object                ! @.str.1
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.1:
	.asciz	"XXX b-bit set %lld %lld\n"
	.size	.L.str.1, 25

	.type	.L.str.2,@object                ! @.str.2
.L.str.2:
	.asciz	"yy b-bit set %lld %lld\n"
	.size	.L.str.2, 24

	.type	.L.str.3,@object                ! @.str.3
.L.str.3:
	.asciz	"zzz b-bit set %lld %lld\n"
	.size	.L.str.3, 25

	.type	.L.str.4,@object                ! @.str.4
.L.str.4:
	.asciz	"eee b-bit set %lld %lld\n"
	.size	.L.str.4, 25

	.type	.L.str.5,@object                ! @.str.5
.L.str.5:
	.asciz	"qqq b-bit set %lld %lld\n"
	.size	.L.str.5, 25

	.type	.L.str.6,@object                ! @.str.6
.L.str.6:
	.asciz	"XXX b-bit set %d %d\n"
	.size	.L.str.6, 21

	.type	.L.str.7,@object                ! @.str.7
.L.str.7:
	.asciz	"yy b-bit set %d %d\n"
	.size	.L.str.7, 20

	.type	.L.str.8,@object                ! @.str.8
.L.str.8:
	.asciz	"zzz b-bit set %d %d\n"
	.size	.L.str.8, 21

	.type	.L.str.9,@object                ! @.str.9
.L.str.9:
	.asciz	"eee b-bit set %d %d\n"
	.size	.L.str.9, 21

	.type	.L.str.10,@object               ! @.str.10
.L.str.10:
	.asciz	"qqq b-bit set %d %d\n"
	.size	.L.str.10, 21

	.type	.L.str.11,@object               ! @.str.11
.L.str.11:
	.asciz	"%lld"
	.size	.L.str.11, 5

	.type	.L.str.12,@object               ! @.str.12
.L.str.12:
	.asciz	"test"
	.size	.L.str.12, 5

	.type	.L.str.13,@object               ! @.str.13
.L.str.13:
	.asciz	"x32=%d"
	.size	.L.str.13, 7

	.type	.L.str.14,@object               ! @.str.14
.L.str.14:
	.asciz	"x32=%lld"
	.size	.L.str.14, 9

	.type	.L.str.15,@object               ! @.str.15
.L.str.15:
	.asciz	"less %lld\n"
	.size	.L.str.15, 11

	.type	.L.str.16,@object               ! @.str.16
.L.str.16:
	.asciz	"greater %lld\n"
	.size	.L.str.16, 14

	.type	.L.str.17,@object               ! @.str.17
.L.str.17:
	.asciz	"zz=%d\n"
	.size	.L.str.17, 7

	.type	uglbx,@object                   ! @uglbx
	.data
	.p2align	3, 0x0
uglbx:
	.xword	1                               ! 0x1
	.size	uglbx, 8

	.type	.L.str.18,@object               ! @.str.18
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.18:
	.asciz	"xx=%d\n"
	.size	.L.str.18, 7

	.type	.L.str.19,@object               ! @.str.19
.L.str.19:
	.asciz	"x=%le\n"
	.size	.L.str.19, 7

	.type	f16_zero,@object                ! @f16_zero
	.local	f16_zero
	.comm	f16_zero,2,2
	.type	f32_zero,@object                ! @f32_zero
	.local	f32_zero
	.comm	f32_zero,4,4
	.type	f64_zero,@object                ! @f64_zero
	.local	f64_zero
	.comm	f64_zero,8,8
	.type	f128_zero,@object               ! @f128_zero
	.local	f128_zero
	.comm	f128_zero,16,16
	.type	x_bss,@object                   ! @x_bss
	.section	.bss,"aw",@nobits
	.globl	x_bss
	.p2align	3, 0x0
x_bss:
	.xword	0                               ! 0x0
	.size	x_bss, 8

	.type	i8_zero,@object                 ! @i8_zero
	.local	i8_zero
	.comm	i8_zero,1,1
	.type	u8_zero,@object                 ! @u8_zero
	.local	u8_zero
	.comm	u8_zero,1,1
	.type	i16_zero,@object                ! @i16_zero
	.local	i16_zero
	.comm	i16_zero,2,2
	.type	u16_zero,@object                ! @u16_zero
	.local	u16_zero
	.comm	u16_zero,2,2
	.type	i32_zero,@object                ! @i32_zero
	.local	i32_zero
	.comm	i32_zero,4,4
	.type	u32_zero,@object                ! @u32_zero
	.local	u32_zero
	.comm	u32_zero,4,4
	.type	i64_zero,@object                ! @i64_zero
	.local	i64_zero
	.comm	i64_zero,8,8
	.type	u64_zero,@object                ! @u64_zero
	.local	u64_zero
	.comm	u64_zero,8,8
	.type	i128_zero,@object               ! @i128_zero
	.local	i128_zero
	.comm	i128_zero,16,16
	.type	u128_zero,@object               ! @u128_zero
	.local	u128_zero
	.comm	u128_zero,16,16
	.type	.L.str.38,@object               ! @.str.38
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.38:
	.asciz	"case 0 %lld"
	.size	.L.str.38, 12

	.type	.L.str.39,@object               ! @.str.39
.L.str.39:
	.asciz	"case 1 %lld %lld"
	.size	.L.str.39, 17

	.type	.L.str.40,@object               ! @.str.40
.L.str.40:
	.asciz	"case 2 %lld %lld"
	.size	.L.str.40, 17

	.type	.L.str.41,@object               ! @.str.41
.L.str.41:
	.asciz	"case 3 %lld %lld"
	.size	.L.str.41, 17

	.type	.L.str.42,@object               ! @.str.42
.L.str.42:
	.asciz	"case 4 %lld %lld"
	.size	.L.str.42, 17

	.type	.L.str.43,@object               ! @.str.43
.L.str.43:
	.asciz	"case 5 %lld %lld"
	.size	.L.str.43, 17

	.type	.L.str.44,@object               ! @.str.44
.L.str.44:
	.asciz	"case 0 %d"
	.size	.L.str.44, 10

	.type	.L.str.45,@object               ! @.str.45
.L.str.45:
	.asciz	"case 1 %d %d"
	.size	.L.str.45, 13

	.type	.L.str.46,@object               ! @.str.46
.L.str.46:
	.asciz	"case 2 %d %d"
	.size	.L.str.46, 13

	.type	.L.str.47,@object               ! @.str.47
.L.str.47:
	.asciz	"case 3 %d %d"
	.size	.L.str.47, 13

	.type	.L.str.48,@object               ! @.str.48
.L.str.48:
	.asciz	"case 4 %d %d"
	.size	.L.str.48, 13

	.type	.L.str.49,@object               ! @.str.49
.L.str.49:
	.asciz	"case 5 %d %d"
	.size	.L.str.49, 13

	.type	.L.str.50,@object               ! @.str.50
.L.str.50:
	.asciz	"case 0 %u"
	.size	.L.str.50, 10

	.type	.L.str.51,@object               ! @.str.51
.L.str.51:
	.asciz	"case 1 %u %u"
	.size	.L.str.51, 13

	.type	.L.str.52,@object               ! @.str.52
.L.str.52:
	.asciz	"case 2 %u %u"
	.size	.L.str.52, 13

	.type	.L.str.53,@object               ! @.str.53
.L.str.53:
	.asciz	"case 3 %u %du"
	.size	.L.str.53, 14

	.type	.L.str.54,@object               ! @.str.54
.L.str.54:
	.asciz	"case 4 %u %u"
	.size	.L.str.54, 13

	.type	.L.str.55,@object               ! @.str.55
.L.str.55:
	.asciz	"case 5 %u %u"
	.size	.L.str.55, 13

	.type	.L.str.56,@object               ! @.str.56
.L.str.56:
	.asciz	"case 10 %lld"
	.size	.L.str.56, 13

	.type	.L.str.57,@object               ! @.str.57
.L.str.57:
	.asciz	"case 11 %lld %lld"
	.size	.L.str.57, 18

	.type	.L.str.58,@object               ! @.str.58
.L.str.58:
	.asciz	"case 12 %lld %lld"
	.size	.L.str.58, 18

	.type	.L.str.59,@object               ! @.str.59
.L.str.59:
	.asciz	"case 13 %lld %lld"
	.size	.L.str.59, 18

	.type	.L.str.60,@object               ! @.str.60
.L.str.60:
	.asciz	"case 14 %lld %lld"
	.size	.L.str.60, 18

	.type	.L.str.61,@object               ! @.str.61
.L.str.61:
	.asciz	"case 15 %lld %lld"
	.size	.L.str.61, 18

	.type	.L.str.62,@object               ! @.str.62
.L.str.62:
	.asciz	"case 10 %d"
	.size	.L.str.62, 11

	.type	.L.str.63,@object               ! @.str.63
.L.str.63:
	.asciz	"case 11 %d %d"
	.size	.L.str.63, 14

	.type	.L.str.64,@object               ! @.str.64
.L.str.64:
	.asciz	"case 12 %d %d"
	.size	.L.str.64, 14

	.type	.L.str.65,@object               ! @.str.65
.L.str.65:
	.asciz	"case 13 %d %d"
	.size	.L.str.65, 14

	.type	.L.str.66,@object               ! @.str.66
.L.str.66:
	.asciz	"case 14 %d %d"
	.size	.L.str.66, 14

	.type	.L.str.67,@object               ! @.str.67
.L.str.67:
	.asciz	"case 15 %d %d"
	.size	.L.str.67, 14

	.type	.L.str.68,@object               ! @.str.68
.L.str.68:
	.asciz	"case 10 %u"
	.size	.L.str.68, 11

	.type	.L.str.69,@object               ! @.str.69
.L.str.69:
	.asciz	"case 11 %u %u"
	.size	.L.str.69, 14

	.type	.L.str.70,@object               ! @.str.70
.L.str.70:
	.asciz	"case 12 %u %u"
	.size	.L.str.70, 14

	.type	.L.str.71,@object               ! @.str.71
.L.str.71:
	.asciz	"case 13 %u %du"
	.size	.L.str.71, 15

	.type	.L.str.72,@object               ! @.str.72
.L.str.72:
	.asciz	"case 14 %u %u"
	.size	.L.str.72, 14

	.type	.L.str.73,@object               ! @.str.73
.L.str.73:
	.asciz	"case 15 %u %u"
	.size	.L.str.73, 14

	.type	x_tls,@object                   ! @x_tls
	.section	.tbss,"awT",@nobits
	.globl	x_tls
	.p2align	3, 0x0
x_tls:
	.xword	0                               ! 0x0
	.size	x_tls, 8

	.type	.L.str.74,@object               ! @.str.74
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.74:
	.asciz	"ccc=%lld\n"
	.size	.L.str.74, 10

	.type	.L.str.75,@object               ! @.str.75
.L.str.75:
	.asciz	"temp=%lld\n"
	.size	.L.str.75, 11

	.type	.Lstr.98,@object                ! @str.98
.Lstr.98:
	.asciz	"branch!"
	.size	.Lstr.98, 8

	.type	.Lstr.99,@object                ! @str.99
.Lstr.99:
	.asciz	"test f32 eq"
	.size	.Lstr.99, 12

	.type	.Lstr.100,@object               ! @str.100
.Lstr.100:
	.asciz	"test f32 ne"
	.size	.Lstr.100, 12

	.type	.Lstr.101,@object               ! @str.101
.Lstr.101:
	.asciz	"test f32 lt"
	.size	.Lstr.101, 12

	.type	.Lstr.102,@object               ! @str.102
.Lstr.102:
	.asciz	"test f32 le"
	.size	.Lstr.102, 12

	.type	.Lstr.103,@object               ! @str.103
.Lstr.103:
	.asciz	"test f32 gt"
	.size	.Lstr.103, 12

	.type	.Lstr.104,@object               ! @str.104
.Lstr.104:
	.asciz	"test f32 ge"
	.size	.Lstr.104, 12

	.type	.Lstr.105,@object               ! @str.105
.Lstr.105:
	.asciz	"test f64 eq"
	.size	.Lstr.105, 12

	.type	.Lstr.106,@object               ! @str.106
.Lstr.106:
	.asciz	"test f64 ne"
	.size	.Lstr.106, 12

	.type	.Lstr.107,@object               ! @str.107
.Lstr.107:
	.asciz	"test f64 lt"
	.size	.Lstr.107, 12

	.type	.Lstr.108,@object               ! @str.108
.Lstr.108:
	.asciz	"test f64 le"
	.size	.Lstr.108, 12

	.type	.Lstr.109,@object               ! @str.109
.Lstr.109:
	.asciz	"test f64 gt"
	.size	.Lstr.109, 12

	.type	.Lstr.110,@object               ! @str.110
.Lstr.110:
	.asciz	"test f64 ge"
	.size	.Lstr.110, 12

	.type	.Lstr.111,@object               ! @str.111
.Lstr.111:
	.asciz	"test f128 eq"
	.size	.Lstr.111, 13

	.type	.Lstr.112,@object               ! @str.112
.Lstr.112:
	.asciz	"test f128 ne"
	.size	.Lstr.112, 13

	.type	.Lstr.113,@object               ! @str.113
.Lstr.113:
	.asciz	"test f128 lt"
	.size	.Lstr.113, 13

	.type	.Lstr.114,@object               ! @str.114
.Lstr.114:
	.asciz	"test f128 le"
	.size	.Lstr.114, 13

	.type	.Lstr.115,@object               ! @str.115
.Lstr.115:
	.asciz	"test f128 gt"
	.size	.Lstr.115, 13

	.type	.Lstr.116,@object               ! @str.116
.Lstr.116:
	.asciz	"test f128 ge"
	.size	.Lstr.116, 13

	.ident	"clang version 20.0.0git"
	.section	".note.GNU-stack","",@progbits
