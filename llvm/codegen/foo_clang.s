	.file	"foo.temp.c"
	.text
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
	ret.f	0

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
	absd.w	%r1, %r1, %gz
	ret.f	0

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
	ret.f	0

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
	absd.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end3:
	.size	test_i32_abs_difference, .Lfunc_end3-test_i32_abs_difference
	.size	.Ltest_i32_abs_difference$local, .Lfunc_end3-test_i32_abs_difference
                                        ! -- End function
	.globl	test_aesenc                     ! -- Begin function test_aesenc
	.p2align	4
	.type	test_aesenc,@function
test_aesenc:                            ! @test_aesenc
.Ltest_aesenc$local:
	.type	.Ltest_aesenc$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	aes.enc	%r1, %r1, %r2
	ret.f	0

.Lfunc_end4:
	.size	test_aesenc, .Lfunc_end4-test_aesenc
	.size	.Ltest_aesenc$local, .Lfunc_end4-test_aesenc
                                        ! -- End function
	.globl	test_aesenclast                 ! -- Begin function test_aesenclast
	.p2align	4
	.type	test_aesenclast,@function
test_aesenclast:                        ! @test_aesenclast
.Ltest_aesenclast$local:
	.type	.Ltest_aesenclast$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	aes.enc.last	%r1, %r1, %r2
	ret.f	0

.Lfunc_end5:
	.size	test_aesenclast, .Lfunc_end5-test_aesenclast
	.size	.Ltest_aesenclast$local, .Lfunc_end5-test_aesenclast
                                        ! -- End function
	.globl	test_aesdec                     ! -- Begin function test_aesdec
	.p2align	4
	.type	test_aesdec,@function
test_aesdec:                            ! @test_aesdec
.Ltest_aesdec$local:
	.type	.Ltest_aesdec$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	aes.dec	%r1, %r1, %r2
	ret.f	0

.Lfunc_end6:
	.size	test_aesdec, .Lfunc_end6-test_aesdec
	.size	.Ltest_aesdec$local, .Lfunc_end6-test_aesdec
                                        ! -- End function
	.globl	test_aesdeclast                 ! -- Begin function test_aesdeclast
	.p2align	4
	.type	test_aesdeclast,@function
test_aesdeclast:                        ! @test_aesdeclast
.Ltest_aesdeclast$local:
	.type	.Ltest_aesdeclast$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	aes.dec.last	%r1, %r1, %r2
	ret.f	0

.Lfunc_end7:
	.size	test_aesdeclast, .Lfunc_end7-test_aesdeclast
	.size	.Ltest_aesdeclast$local, .Lfunc_end7-test_aesdeclast
                                        ! -- End function
	.globl	test_aesimc                     ! -- Begin function test_aesimc
	.p2align	4
	.type	test_aesimc,@function
test_aesimc:                            ! @test_aesimc
.Ltest_aesimc$local:
	.type	.Ltest_aesimc$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	aes.imc	%r1, %r1
	ret.f	0

.Lfunc_end8:
	.size	test_aesimc, .Lfunc_end8-test_aesimc
	.size	.Ltest_aesimc$local, .Lfunc_end8-test_aesimc
                                        ! -- End function
	.globl	test_aeskeygenassist            ! -- Begin function test_aeskeygenassist
	.p2align	4
	.type	test_aeskeygenassist,@function
test_aeskeygenassist:                   ! @test_aeskeygenassist
.Ltest_aeskeygenassist$local:
	.type	.Ltest_aeskeygenassist$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	aeskeygenassist	%r1, %r1, 100
	aeskeygenassist	%r1, %r1, -1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end9:
	.size	test_aeskeygenassist, .Lfunc_end9-test_aeskeygenassist
	.size	.Ltest_aeskeygenassist$local, .Lfunc_end9-test_aeskeygenassist
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
	ret.f	0

.Lfunc_end10:
	.size	test_ldi32, .Lfunc_end10-test_ldi32
	.size	.Ltest_ldi32$local, .Lfunc_end10-test_ldi32
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end11:
	.size	test_ldi32_long, .Lfunc_end11-test_ldi32_long
	.size	.Ltest_ldi32_long$local, .Lfunc_end11-test_ldi32_long
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
	ret.f	0

.Lfunc_end12:
	.size	test_ldi64, .Lfunc_end12-test_ldi64
	.size	.Ltest_ldi64$local, .Lfunc_end12-test_ldi64
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end13:
	.size	test_ldi64_long, .Lfunc_end13-test_ldi64_long
	.size	.Ltest_ldi64_long$local, .Lfunc_end13-test_ldi64_long
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
	subfi	%r1, %r1, 1234
	ret.f	0

.Lfunc_end14:
	.size	test_subfi_i64, .Lfunc_end14-test_subfi_i64
	.size	.Ltest_subfi_i64$local, .Lfunc_end14-test_subfi_i64
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
	subfi.ws	%r1, %r1, 1234
	ret.f	0

.Lfunc_end15:
	.size	test_subfi_i32, .Lfunc_end15-test_subfi_i32
	.size	.Ltest_subfi_i32$local, .Lfunc_end15-test_subfi_i32
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end16:
	.size	test_addil, .Lfunc_end16-test_addil
	.size	.Ltest_addil$local, .Lfunc_end16-test_addil
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end17:
	.size	test_andil, .Lfunc_end17-test_andil
	.size	.Ltest_andil$local, .Lfunc_end17-test_andil
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end18:
	.size	test_oril, .Lfunc_end18-test_oril
	.size	.Ltest_oril$local, .Lfunc_end18-test_oril
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
	add.add	%r1, %r2, %r1, %r3
	ret.f	0

.Lfunc_end19:
	.size	test_addadd, .Lfunc_end19-test_addadd
	.size	.Ltest_addadd$local, .Lfunc_end19-test_addadd
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
	add.sub	%r1, %r2, %r1, %r3
	ret.f	0

.Lfunc_end20:
	.size	test_addsub, .Lfunc_end20-test_addsub
	.size	.Ltest_addsub$local, .Lfunc_end20-test_addsub
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
	sub.sub	%r1, %r1, %r2, %r3
	ret.f	0

.Lfunc_end21:
	.size	test_subsub, .Lfunc_end21-test_subsub
	.size	.Ltest_subsub$local, .Lfunc_end21-test_subsub
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
	mul.ws	%r1, %r2, %r1
	ret.f	0

.Lfunc_end22:
	.size	test_mul_i32_i32, .Lfunc_end22-test_mul_i32_i32
	.size	.Ltest_mul_i32_i32$local, .Lfunc_end22-test_mul_i32_i32
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
	muli.ws	%r1, %r1, 98765
	ret.f	0

.Lfunc_end23:
	.size	test_mul_i32_imm, .Lfunc_end23-test_mul_i32_imm
	.size	.Ltest_mul_i32_imm$local, .Lfunc_end23-test_mul_i32_imm
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
	muli.ws.l	%r1, %r1, 98765432

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end24:
	.size	test_mul_i32_imm_ext, .Lfunc_end24-test_mul_i32_imm_ext
	.size	.Ltest_mul_i32_imm_ext$local, .Lfunc_end24-test_mul_i32_imm_ext
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
	muli.ws	%r1, %r1, 98765
	ret.f	0

.Lfunc_end25:
	.size	test_mul_u32_imm, .Lfunc_end25-test_mul_u32_imm
	.size	.Ltest_mul_u32_imm$local, .Lfunc_end25-test_mul_u32_imm
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
	muli.ws.l	%r1, %r1, 98765432

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end26:
	.size	test_mul_u32_imm_ext, .Lfunc_end26-test_mul_u32_imm_ext
	.size	.Ltest_mul_u32_imm_ext$local, .Lfunc_end26-test_mul_u32_imm_ext
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
	mul.ws	%r1, %r2, %r1
	ret.f	0

.Lfunc_end27:
	.size	test_mul_i32_i64, .Lfunc_end27-test_mul_i32_i64
	.size	.Ltest_mul_i32_i64$local, .Lfunc_end27-test_mul_i32_i64
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
	mul.wz	%r1, %r2, %r1
	ret.f	0

.Lfunc_end28:
	.size	test_mul_u32_u64, .Lfunc_end28-test_mul_u32_u64
	.size	.Ltest_mul_u32_u64$local, .Lfunc_end28-test_mul_u32_u64
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
	mul.ws	%r1, %r2, %r1
	ret.f	0

.Lfunc_end29:
	.size	test_mul_i32_u64, .Lfunc_end29-test_mul_i32_u64
	.size	.Ltest_mul_i32_u64$local, .Lfunc_end29-test_mul_i32_u64
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
	muli.ws	%r1, %r1, 98765
	ret.f	0

.Lfunc_end30:
	.size	test_mul_i32_i64_imm, .Lfunc_end30-test_mul_i32_i64_imm
	.size	.Ltest_mul_i32_i64_imm$local, .Lfunc_end30-test_mul_i32_i64_imm
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
	muli.ws.l	%r1, %r1, 98765432

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end31:
	.size	test_mul_i32_i64_imm_ext, .Lfunc_end31-test_mul_i32_i64_imm_ext
	.size	.Ltest_mul_i32_i64_imm_ext$local, .Lfunc_end31-test_mul_i32_i64_imm_ext
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
	muli.wz	%r1, %r1, 98765
	ret.f	0

.Lfunc_end32:
	.size	test_mul_u32_u64_imm, .Lfunc_end32-test_mul_u32_u64_imm
	.size	.Ltest_mul_u32_u64_imm$local, .Lfunc_end32-test_mul_u32_u64_imm
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
	muli.wz.l	%r1, %r1, 98765432

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end33:
	.size	test_mul_u32_u64_imm_ext, .Lfunc_end33-test_mul_u32_u64_imm_ext
	.size	.Ltest_mul_u32_u64_imm_ext$local, .Lfunc_end33-test_mul_u32_u64_imm_ext
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
	ret.f	0

.Lfunc_end34:
	.size	test_mul_i64_i64, .Lfunc_end34-test_mul_i64_i64
	.size	.Ltest_mul_i64_i64$local, .Lfunc_end34-test_mul_i64_i64
                                        ! -- End function
	.globl	test_div_i32                    ! -- Begin function test_div_i32
	.p2align	4
	.type	test_div_i32,@function
test_div_i32:                           ! @test_div_i32
.Ltest_div_i32$local:
	.type	.Ltest_div_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	div.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end35:
	.size	test_div_i32, .Lfunc_end35-test_div_i32
	.size	.Ltest_div_i32$local, .Lfunc_end35-test_div_i32
                                        ! -- End function
	.globl	test_div_i64                    ! -- Begin function test_div_i64
	.p2align	4
	.type	test_div_i64,@function
test_div_i64:                           ! @test_div_i64
.Ltest_div_i64$local:
	.type	.Ltest_div_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	div	%r1, %r1, %r2
	ret.f	0

.Lfunc_end36:
	.size	test_div_i64, .Lfunc_end36-test_div_i64
	.size	.Ltest_div_i64$local, .Lfunc_end36-test_div_i64
                                        ! -- End function
	.globl	test_div_u32                    ! -- Begin function test_div_u32
	.p2align	4
	.type	test_div_u32,@function
test_div_u32:                           ! @test_div_u32
.Ltest_div_u32$local:
	.type	.Ltest_div_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	divu.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end37:
	.size	test_div_u32, .Lfunc_end37-test_div_u32
	.size	.Ltest_div_u32$local, .Lfunc_end37-test_div_u32
                                        ! -- End function
	.globl	test_div_u64                    ! -- Begin function test_div_u64
	.p2align	4
	.type	test_div_u64,@function
test_div_u64:                           ! @test_div_u64
.Ltest_div_u64$local:
	.type	.Ltest_div_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	divu	%r1, %r1, %r2
	ret.f	0

.Lfunc_end38:
	.size	test_div_u64, .Lfunc_end38-test_div_u64
	.size	.Ltest_div_u64$local, .Lfunc_end38-test_div_u64
                                        ! -- End function
	.globl	test_mod_i32                    ! -- Begin function test_mod_i32
	.p2align	4
	.type	test_mod_i32,@function
test_mod_i32:                           ! @test_mod_i32
.Ltest_mod_i32$local:
	.type	.Ltest_mod_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mod.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end39:
	.size	test_mod_i32, .Lfunc_end39-test_mod_i32
	.size	.Ltest_mod_i32$local, .Lfunc_end39-test_mod_i32
                                        ! -- End function
	.globl	test_mod_i64                    ! -- Begin function test_mod_i64
	.p2align	4
	.type	test_mod_i64,@function
test_mod_i64:                           ! @test_mod_i64
.Ltest_mod_i64$local:
	.type	.Ltest_mod_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	mod	%r1, %r1, %r2
	ret.f	0

.Lfunc_end40:
	.size	test_mod_i64, .Lfunc_end40-test_mod_i64
	.size	.Ltest_mod_i64$local, .Lfunc_end40-test_mod_i64
                                        ! -- End function
	.globl	test_mod_u32                    ! -- Begin function test_mod_u32
	.p2align	4
	.type	test_mod_u32,@function
test_mod_u32:                           ! @test_mod_u32
.Ltest_mod_u32$local:
	.type	.Ltest_mod_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	modu.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end41:
	.size	test_mod_u32, .Lfunc_end41-test_mod_u32
	.size	.Ltest_mod_u32$local, .Lfunc_end41-test_mod_u32
                                        ! -- End function
	.globl	test_mod_u64                    ! -- Begin function test_mod_u64
	.p2align	4
	.type	test_mod_u64,@function
test_mod_u64:                           ! @test_mod_u64
.Ltest_mod_u64$local:
	.type	.Ltest_mod_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	modu	%r1, %r1, %r2
	ret.f	0

.Lfunc_end42:
	.size	test_mod_u64, .Lfunc_end42-test_mod_u64
	.size	.Ltest_mod_u64$local, .Lfunc_end42-test_mod_u64
                                        ! -- End function
	.globl	test_mod_imm_i64                ! -- Begin function test_mod_imm_i64
	.p2align	4
	.type	test_mod_imm_i64,@function
test_mod_imm_i64:                       ! @test_mod_imm_i64
.Ltest_mod_imm_i64$local:
	.type	.Ltest_mod_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	modi	%r1, %r1, 100
	ret.f	0

.Lfunc_end43:
	.size	test_mod_imm_i64, .Lfunc_end43-test_mod_imm_i64
	.size	.Ltest_mod_imm_i64$local, .Lfunc_end43-test_mod_imm_i64
                                        ! -- End function
	.globl	test_mod_imm_u64                ! -- Begin function test_mod_imm_u64
	.p2align	4
	.type	test_mod_imm_u64,@function
test_mod_imm_u64:                       ! @test_mod_imm_u64
.Ltest_mod_imm_u64$local:
	.type	.Ltest_mod_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	modui	%r1, %r1, 100
	ret.f	0

.Lfunc_end44:
	.size	test_mod_imm_u64, .Lfunc_end44-test_mod_imm_u64
	.size	.Ltest_mod_imm_u64$local, .Lfunc_end44-test_mod_imm_u64
                                        ! -- End function
	.globl	test_mod_imm_ext_i64            ! -- Begin function test_mod_imm_ext_i64
	.p2align	4
	.type	test_mod_imm_ext_i64,@function
test_mod_imm_ext_i64:                   ! @test_mod_imm_ext_i64
.Ltest_mod_imm_ext_i64$local:
	.type	.Ltest_mod_imm_ext_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	modi.l	%r1, %r1, 1000000000

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end45:
	.size	test_mod_imm_ext_i64, .Lfunc_end45-test_mod_imm_ext_i64
	.size	.Ltest_mod_imm_ext_i64$local, .Lfunc_end45-test_mod_imm_ext_i64
                                        ! -- End function
	.globl	test_mod_imm_ext_u64            ! -- Begin function test_mod_imm_ext_u64
	.p2align	4
	.type	test_mod_imm_ext_u64,@function
test_mod_imm_ext_u64:                   ! @test_mod_imm_ext_u64
.Ltest_mod_imm_ext_u64$local:
	.type	.Ltest_mod_imm_ext_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	modui.l	%r1, %r1, 1000000000

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end46:
	.size	test_mod_imm_ext_u64, .Lfunc_end46-test_mod_imm_ext_u64
	.size	.Ltest_mod_imm_ext_u64$local, .Lfunc_end46-test_mod_imm_ext_u64
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
	alloc.sp	7, 16
	st.w	%gz, %sp, 12
	ldz.b	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cas.b	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0

	! BUNDLE
	cas.b.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0
	cas.b.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.b	%r6, %r2, 0
	cas.b.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0

	! BUNDLE
	cas.b.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0
	cas.b.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.b	%r6, %r2, 0
	cas.b.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0

	! BUNDLE
	cas.b.r	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0
	cas.b.ar	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.b	%r6, %r2, 0
	cas.b.ar	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0

	! BUNDLE
	cas.b.ar	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0
	fence.sc

	! BUNDLE
	mov	%r5, %r3
	cas.b	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.b	%r6, %r2, 0

	! BUNDLE
	fence.sc
	mov	%r5, %r3
	cas.b	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.b	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.b	%r6, %r2, 0
	fence.sc
	mov	%r5, %r3

	! BUNDLE
	cas.b	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.b	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.b	%r4, %r2, 0
	fence.sc

	! BUNDLE
	cas.b	%r3, %r1, %r4
	cmp.eq.w	%r1, %r3, %r4
	nul.eq.w	%r3, %r4, 1, 0

	! BUNDLE
	st.b	%r3, %r2, 0
	ldz.w	%r2, %sp, 12
	add.ws	%r1, %r2, %r1

	! BUNDLE
	st.w	%r1, %sp, 12
	ldz.w	%r1, %sp, 12
	ret.f	16

.Lfunc_end47:
	.size	test_atomic_compare_exchange_i8, .Lfunc_end47-test_atomic_compare_exchange_i8
	.size	.Ltest_atomic_compare_exchange_i8$local, .Lfunc_end47-test_atomic_compare_exchange_i8
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
	alloc.sp	7, 16
	st.w	%gz, %sp, 12
	ldz.h	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cas.h	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0

	! BUNDLE
	cas.h.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0
	cas.h.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.h	%r6, %r2, 0
	cas.h.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0

	! BUNDLE
	cas.h.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0
	cas.h.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.h	%r6, %r2, 0
	cas.h.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0

	! BUNDLE
	cas.h.r	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0
	cas.h.ar	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.h	%r6, %r2, 0
	cas.h.ar	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0

	! BUNDLE
	cas.h.ar	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0
	fence.sc

	! BUNDLE
	mov	%r5, %r3
	cas.h	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.h	%r6, %r2, 0

	! BUNDLE
	fence.sc
	mov	%r5, %r3
	cas.h	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.h	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.h	%r6, %r2, 0
	fence.sc
	mov	%r5, %r3

	! BUNDLE
	cas.h	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.h	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.h	%r4, %r2, 0
	fence.sc

	! BUNDLE
	cas.h	%r3, %r1, %r4
	cmp.eq.w	%r1, %r3, %r4
	nul.eq.w	%r3, %r4, 1, 0

	! BUNDLE
	st.h	%r3, %r2, 0
	ldz.w	%r2, %sp, 12
	add.ws	%r1, %r2, %r1

	! BUNDLE
	st.w	%r1, %sp, 12
	ldz.w	%r1, %sp, 12
	ret.f	16

.Lfunc_end48:
	.size	test_atomic_compare_exchange_i16, .Lfunc_end48-test_atomic_compare_exchange_i16
	.size	.Ltest_atomic_compare_exchange_i16$local, .Lfunc_end48-test_atomic_compare_exchange_i16
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
	alloc.sp	7, 16
	st.w	%gz, %sp, 12
	ldz.w	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cas.w	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0

	! BUNDLE
	cas.w.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0
	cas.w.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.w	%r6, %r2, 0
	cas.w.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0

	! BUNDLE
	cas.w.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0
	cas.w.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.w	%r6, %r2, 0
	cas.w.a	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0

	! BUNDLE
	cas.w.r	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0
	cas.w.ar	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.w	%r6, %r2, 0
	cas.w.ar	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0

	! BUNDLE
	cas.w.ar	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0
	fence.sc

	! BUNDLE
	mov	%r5, %r3
	cas.w	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6

	! BUNDLE
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.w	%r6, %r2, 0

	! BUNDLE
	fence.sc
	mov	%r5, %r3
	cas.w	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0
	st.w	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.w	%r6, %r2, 0
	fence.sc
	mov	%r5, %r3

	! BUNDLE
	cas.w	%r5, %r1, %r6
	cmp.eq.w	%r4, %r5, %r6
	nul.eq.w	%r5, %r6, 1, 0

	! BUNDLE
	st.w	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.w	%r4, %r2, 0
	fence.sc

	! BUNDLE
	cas.w	%r3, %r1, %r4
	cmp.eq.w	%r1, %r3, %r4
	nul.eq.w	%r3, %r4, 1, 0

	! BUNDLE
	st.w	%r3, %r2, 0
	ldz.w	%r2, %sp, 12
	add.ws	%r1, %r2, %r1

	! BUNDLE
	st.w	%r1, %sp, 12
	ldz.w	%r1, %sp, 12
	ret.f	16

.Lfunc_end49:
	.size	test_atomic_compare_exchange_i32, .Lfunc_end49-test_atomic_compare_exchange_i32
	.size	.Ltest_atomic_compare_exchange_i32$local, .Lfunc_end49-test_atomic_compare_exchange_i32
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
	alloc.sp	7, 16
	st.w	%gz, %sp, 12
	ldz.d	%r6, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cas.d	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6

	! BUNDLE
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0

	! BUNDLE
	cas.d.a	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0

	! BUNDLE
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0
	cas.d.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.d	%r6, %r2, 0
	cas.d.a	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6

	! BUNDLE
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0

	! BUNDLE
	cas.d.a	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0

	! BUNDLE
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0
	cas.d.a	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.d	%r6, %r2, 0
	cas.d.a	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6

	! BUNDLE
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0

	! BUNDLE
	cas.d.r	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0

	! BUNDLE
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0
	cas.d.ar	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.d	%r6, %r2, 0
	cas.d.ar	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6

	! BUNDLE
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0

	! BUNDLE
	cas.d.ar	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0

	! BUNDLE
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0
	fence.sc

	! BUNDLE
	mov	%r5, %r3
	cas.d	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6

	! BUNDLE
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ldz.d	%r6, %r2, 0

	! BUNDLE
	fence.sc
	mov	%r5, %r3
	cas.d	%r5, %r1, %r6

	! BUNDLE
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0
	st.d	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ldz.d	%r6, %r2, 0
	fence.sc
	mov	%r5, %r3

	! BUNDLE
	cas.d	%r5, %r1, %r6
	cmp.eq.d	%r4, %r5, %r6
	nul.eq.d	%r5, %r6, 1, 0

	! BUNDLE
	st.d	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ldz.d	%r4, %r2, 0
	fence.sc

	! BUNDLE
	cas.d	%r3, %r1, %r4
	cmp.eq.d	%r1, %r3, %r4
	nul.eq.d	%r3, %r4, 1, 0

	! BUNDLE
	st.d	%r3, %r2, 0
	ldz.w	%r2, %sp, 12
	add.ws	%r1, %r2, %r1

	! BUNDLE
	st.w	%r1, %sp, 12
	ldz.w	%r1, %sp, 12
	ret.f	16

.Lfunc_end50:
	.size	test_atomic_compare_exchange_i64, .Lfunc_end50-test_atomic_compare_exchange_i64
	.size	.Ltest_atomic_compare_exchange_i64$local, .Lfunc_end50-test_atomic_compare_exchange_i64
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
	alloc.sp	6, 16
	st.w	%gz, %sp, 12
	ld.q	%r4, %r2, 0

	! BUNDLE
	mov	%r5, %r3
	cas.q	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4

	! BUNDLE
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0

	! BUNDLE
	cas.q.a	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0

	! BUNDLE
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0
	cas.q.a	%r5, %r1, %r4

	! BUNDLE
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ld.q	%r4, %r2, 0
	cas.q.a	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4

	! BUNDLE
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0

	! BUNDLE
	cas.q.a	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0

	! BUNDLE
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0
	cas.q.a	%r5, %r1, %r4

	! BUNDLE
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ld.q	%r4, %r2, 0
	cas.q.a	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4

	! BUNDLE
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0

	! BUNDLE
	cas.q.r	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0

	! BUNDLE
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0
	cas.q.ar	%r5, %r1, %r4

	! BUNDLE
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ld.q	%r4, %r2, 0
	cas.q.ar	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4

	! BUNDLE
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0

	! BUNDLE
	cas.q.ar	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0

	! BUNDLE
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0
	fence.sc

	! BUNDLE
	mov	%r5, %r3
	cas.q	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4

	! BUNDLE
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12

	! BUNDLE
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0

	! BUNDLE
	fence.sc
	mov	%r5, %r3
	cas.q	%r5, %r1, %r4

	! BUNDLE
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0
	st.q	%r5, %r2, 0

	! BUNDLE
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4
	st.w	%r4, %sp, 12

	! BUNDLE
	ld.q	%r4, %r2, 0
	fence.sc
	mov	%r5, %r3

	! BUNDLE
	cas.q	%r5, %r1, %r4
	cmp.eq.q	%r4, %r5, %r4
	nul.bsi	%r4, 0, 1, 0

	! BUNDLE
	st.q	%r5, %r2, 0
	ldz.w	%r5, %sp, 12
	add.ws	%r4, %r5, %r4

	! BUNDLE
	st.w	%r4, %sp, 12
	ld.q	%r4, %r2, 0
	fence.sc

	! BUNDLE
	cas.q	%r3, %r1, %r4
	cmp.eq.q	%r1, %r3, %r4
	nul.bsi	%r1, 0, 1, 0

	! BUNDLE
	st.q	%r3, %r2, 0
	ldz.w	%r2, %sp, 12
	add.ws	%r1, %r2, %r1

	! BUNDLE
	st.w	%r1, %sp, 12
	ldz.w	%r1, %sp, 12
	ret.f	16

.Lfunc_end51:
	.size	test_atomic_compare_exchange_i128, .Lfunc_end51-test_atomic_compare_exchange_i128
	.size	.Ltest_atomic_compare_exchange_i128$local, .Lfunc_end51-test_atomic_compare_exchange_i128
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
	swap.b	%r1, %r2, %r1
	swap.b.a	%r1, %r2, %r1

	! BUNDLE
	swap.b.a	%r1, %r2, %r1
	swap.b.r	%r1, %r2, %r1
	swap.b.ar	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	swap.b	%r1, %r2, %r1
	slsrai	%r1, %r1, 56, 56

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end52:
	.size	test_atomic_exchange_i8, .Lfunc_end52-test_atomic_exchange_i8
	.size	.Ltest_atomic_exchange_i8$local, .Lfunc_end52-test_atomic_exchange_i8
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
	swap.h	%r1, %r2, %r1
	swap.h.a	%r1, %r2, %r1

	! BUNDLE
	swap.h.a	%r1, %r2, %r1
	swap.h.r	%r1, %r2, %r1
	swap.h.ar	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	swap.h	%r1, %r2, %r1
	slsrai	%r1, %r1, 48, 48

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end53:
	.size	test_atomic_exchange_i16, .Lfunc_end53-test_atomic_exchange_i16
	.size	.Ltest_atomic_exchange_i16$local, .Lfunc_end53-test_atomic_exchange_i16
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
	swap.w	%r1, %r2, %r1
	swap.w.a	%r1, %r2, %r1

	! BUNDLE
	swap.w.a	%r1, %r2, %r1
	swap.w.r	%r1, %r2, %r1
	swap.w.ar	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	swap.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end54:
	.size	test_atomic_exchange_i32, .Lfunc_end54-test_atomic_exchange_i32
	.size	.Ltest_atomic_exchange_i32$local, .Lfunc_end54-test_atomic_exchange_i32
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
	swap.d	%r1, %r2, %r1
	swap.d.a	%r1, %r2, %r1

	! BUNDLE
	swap.d.a	%r1, %r2, %r1
	swap.d.r	%r1, %r2, %r1
	swap.d.ar	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	swap.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end55:
	.size	test_atomic_exchange_i64, .Lfunc_end55-test_atomic_exchange_i64
	.size	.Ltest_atomic_exchange_i64$local, .Lfunc_end55-test_atomic_exchange_i64
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
	swap.q	%r1, %r2, %r1
	swap.q.a	%r1, %r2, %r1

	! BUNDLE
	swap.q.a	%r1, %r2, %r1
	swap.q.r	%r1, %r2, %r1
	swap.q.ar	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	swap.q	%r1, %r2, %r1
	ret.f	0

.Lfunc_end56:
	.size	test_atomic_exchange_i128, .Lfunc_end56-test_atomic_exchange_i128
	.size	.Ltest_atomic_exchange_i128$local, .Lfunc_end56-test_atomic_exchange_i128
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
	fence.a
	ret.f	0

.Lfunc_end57:
	.size	test_fence_acquire, .Lfunc_end57-test_fence_acquire
	.size	.Ltest_fence_acquire$local, .Lfunc_end57-test_fence_acquire
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
	fence.r
	ret.f	0

.Lfunc_end58:
	.size	test_fence_release, .Lfunc_end58-test_fence_release
	.size	.Ltest_fence_release$local, .Lfunc_end58-test_fence_release
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
	fence.ar
	ret.f	0

.Lfunc_end59:
	.size	test_fence_acq_rel, .Lfunc_end59-test_fence_acq_rel
	.size	.Ltest_fence_acq_rel$local, .Lfunc_end59-test_fence_acq_rel
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
	fence.sc
	ret.f	0

.Lfunc_end60:
	.size	test_fence_seq_cst, .Lfunc_end60-test_fence_seq_cst
	.size	.Ltest_fence_seq_cst$local, .Lfunc_end60-test_fence_seq_cst
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
	ret.f	0
	nop	0

.Lfunc_end61:
	.size	test_compiler_fence, .Lfunc_end61-test_compiler_fence
	.size	.Ltest_compiler_fence$local, .Lfunc_end61-test_compiler_fence
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
	lda.b	%r1, %r1
	lda.b.a	%r2, %r2

	! BUNDLE
	lda.b.a	%r3, %r3
	fence.sc
	lda.b	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end62:
	.size	test_atomic_load_u8_u8, .Lfunc_end62-test_atomic_load_u8_u8
	.size	.Ltest_atomic_load_u8_u8$local, .Lfunc_end62-test_atomic_load_u8_u8
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
	lda.b	%r1, %r1
	lda.b.a	%r2, %r2

	! BUNDLE
	lda.b.a	%r3, %r3
	fence.sc
	lda.b	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end63:
	.size	test_atomic_load_u8_u16, .Lfunc_end63-test_atomic_load_u8_u16
	.size	.Ltest_atomic_load_u8_u16$local, .Lfunc_end63-test_atomic_load_u8_u16
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
	lda.b	%r1, %r1
	lda.b.a	%r2, %r2

	! BUNDLE
	lda.b.a	%r3, %r3
	fence.sc
	lda.b	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end64:
	.size	test_atomic_load_u8_u32, .Lfunc_end64-test_atomic_load_u8_u32
	.size	.Ltest_atomic_load_u8_u32$local, .Lfunc_end64-test_atomic_load_u8_u32
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
	lda.b	%r1, %r1
	lda.b.a	%r2, %r2

	! BUNDLE
	lda.b.a	%r3, %r3
	fence.sc
	lda.b	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	srli.w	%r1, %r1, 0
	fence.a
	ret.f	0

.Lfunc_end65:
	.size	test_atomic_load_u8_u64, .Lfunc_end65-test_atomic_load_u8_u64
	.size	.Ltest_atomic_load_u8_u64$local, .Lfunc_end65-test_atomic_load_u8_u64
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
	lda.h	%r1, %r1
	lda.h.a	%r2, %r2

	! BUNDLE
	lda.h.a	%r3, %r3
	fence.sc
	lda.h	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end66:
	.size	test_atomic_load_u16_u16, .Lfunc_end66-test_atomic_load_u16_u16
	.size	.Ltest_atomic_load_u16_u16$local, .Lfunc_end66-test_atomic_load_u16_u16
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
	lda.h	%r1, %r1
	lda.h.a	%r2, %r2

	! BUNDLE
	lda.h.a	%r3, %r3
	fence.sc
	lda.h	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end67:
	.size	test_atomic_load_u16_u32, .Lfunc_end67-test_atomic_load_u16_u32
	.size	.Ltest_atomic_load_u16_u32$local, .Lfunc_end67-test_atomic_load_u16_u32
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
	lda.h	%r1, %r1
	lda.h.a	%r2, %r2

	! BUNDLE
	lda.h.a	%r3, %r3
	fence.sc
	lda.h	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	srli.w	%r1, %r1, 0
	fence.a
	ret.f	0

.Lfunc_end68:
	.size	test_atomic_load_u16_u64, .Lfunc_end68-test_atomic_load_u16_u64
	.size	.Ltest_atomic_load_u16_u64$local, .Lfunc_end68-test_atomic_load_u16_u64
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
	lda.w	%r1, %r1
	lda.w.a	%r2, %r2

	! BUNDLE
	lda.w.a	%r3, %r3
	fence.sc
	lda.w	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end69:
	.size	test_atomic_load_u32_u32, .Lfunc_end69-test_atomic_load_u32_u32
	.size	.Ltest_atomic_load_u32_u32$local, .Lfunc_end69-test_atomic_load_u32_u32
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
	lda.w	%r1, %r1
	lda.w.a	%r2, %r2

	! BUNDLE
	lda.w.a	%r3, %r3
	fence.sc
	lda.w	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	srli.w	%r1, %r1, 0
	fence.a
	ret.f	0

.Lfunc_end70:
	.size	test_atomic_load_u32_u64, .Lfunc_end70-test_atomic_load_u32_u64
	.size	.Ltest_atomic_load_u32_u64$local, .Lfunc_end70-test_atomic_load_u32_u64
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
	lda.d	%r1, %r1
	lda.d.a	%r2, %r2

	! BUNDLE
	lda.d.a	%r3, %r3
	fence.sc
	lda.d	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end71:
	.size	test_atomic_load_u64_u64, .Lfunc_end71-test_atomic_load_u64_u64
	.size	.Ltest_atomic_load_u64_u64$local, .Lfunc_end71-test_atomic_load_u64_u64
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
	lda.q	%r1, %r1
	lda.q.a	%r2, %r2

	! BUNDLE
	lda.q.a	%r3, %r3
	fence.sc
	lda.q	%r4, %r4

	! BUNDLE
	xor	%r1, %r2, %r1
	and	%r1, %r1, %r3
	or	%r1, %r1, %r4

	! BUNDLE
	fence.a
	ret.f	0
	nop	0

.Lfunc_end72:
	.size	test_atomic_load_u128_u128, .Lfunc_end72-test_atomic_load_u128_u128
	.size	.Ltest_atomic_load_u128_u128$local, .Lfunc_end72-test_atomic_load_u128_u128
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
	ld.add.b	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.b	%r1, %r2, %r1
	ld.and.b	%r1, %r2, %r1
	ld.or.b	%r1, %r2, %r1

	! BUNDLE
	ld.xor.b	%r1, %r2, %r1
	ld.smin.b	%r1, %r2, %r1
	ld.smax.b	%r1, %r2, %r1

	! BUNDLE
	ld.add.b.a	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.b.a	%r1, %r2, %r1
	ld.or.b.a	%r1, %r2, %r1
	ld.xor.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.smin.b.a	%r1, %r2, %r1
	ld.smax.b.a	%r1, %r2, %r1
	ld.add.b.a	%r1, %r2, %r1

	! BUNDLE
	sub.ws	%r1, %gz, %r1
	ld.add.b.a	%r1, %r2, %r1
	ld.and.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.b.a	%r1, %r2, %r1
	ld.xor.b.a	%r1, %r2, %r1
	ld.smin.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.smax.b.a	%r1, %r2, %r1
	ld.add.b.r	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.b.r	%r1, %r2, %r1
	ld.and.b.r	%r1, %r2, %r1
	ld.or.b.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.b.r	%r1, %r2, %r1
	ld.smin.b.r	%r1, %r2, %r1
	ld.smax.b.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.b.ar	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.b.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.b.ar	%r1, %r2, %r1
	ld.or.b.ar	%r1, %r2, %r1
	ld.xor.b.ar	%r1, %r2, %r1

	! BUNDLE
	ld.smin.b.ar	%r1, %r2, %r1
	ld.smax.b.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.b	%r1, %r2, %r1
	fence.sc
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.b	%r1, %r2, %r1
	fence.sc
	ld.and.b	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.b	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.b	%r1, %r2, %r1
	fence.sc
	ld.smin.b	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.smax.b	%r1, %r2, %r1
	slsrai	%r1, %r1, 56, 56

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end73:
	.size	test_atomic_load_op_i8, .Lfunc_end73-test_atomic_load_op_i8
	.size	.Ltest_atomic_load_op_i8$local, .Lfunc_end73-test_atomic_load_op_i8
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
	ld.add.b	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.b	%r1, %r2, %r1
	ld.and.b	%r1, %r2, %r1
	ld.or.b	%r1, %r2, %r1

	! BUNDLE
	ld.xor.b	%r1, %r2, %r1
	ld.umin.b	%r1, %r2, %r1
	ld.umax.b	%r1, %r2, %r1

	! BUNDLE
	ld.add.b.a	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.b.a	%r1, %r2, %r1
	ld.or.b.a	%r1, %r2, %r1
	ld.xor.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.umin.b.a	%r1, %r2, %r1
	ld.umax.b.a	%r1, %r2, %r1
	ld.add.b.a	%r1, %r2, %r1

	! BUNDLE
	sub.ws	%r1, %gz, %r1
	ld.add.b.a	%r1, %r2, %r1
	ld.and.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.b.a	%r1, %r2, %r1
	ld.xor.b.a	%r1, %r2, %r1
	ld.umin.b.a	%r1, %r2, %r1

	! BUNDLE
	ld.umax.b.a	%r1, %r2, %r1
	ld.add.b.r	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.b.r	%r1, %r2, %r1
	ld.and.b.r	%r1, %r2, %r1
	ld.or.b.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.b.r	%r1, %r2, %r1
	ld.umin.b.r	%r1, %r2, %r1
	ld.umax.b.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.b.ar	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.b.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.b.ar	%r1, %r2, %r1
	ld.or.b.ar	%r1, %r2, %r1
	ld.xor.b.ar	%r1, %r2, %r1

	! BUNDLE
	ld.umin.b.ar	%r1, %r2, %r1
	ld.umax.b.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.b	%r1, %r2, %r1
	fence.sc
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.b	%r1, %r2, %r1
	fence.sc
	ld.and.b	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.b	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.b	%r1, %r2, %r1
	fence.sc
	ld.umin.b	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.umax.b	%r1, %r2, %r1
	ret.f	0

.Lfunc_end74:
	.size	test_atomic_load_op_u8, .Lfunc_end74-test_atomic_load_op_u8
	.size	.Ltest_atomic_load_op_u8$local, .Lfunc_end74-test_atomic_load_op_u8
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
	ld.add.h	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.h	%r1, %r2, %r1
	ld.and.h	%r1, %r2, %r1
	ld.or.h	%r1, %r2, %r1

	! BUNDLE
	ld.xor.h	%r1, %r2, %r1
	ld.smin.h	%r1, %r2, %r1
	ld.smax.h	%r1, %r2, %r1

	! BUNDLE
	ld.add.h.a	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.h.a	%r1, %r2, %r1
	ld.or.h.a	%r1, %r2, %r1
	ld.xor.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.smin.h.a	%r1, %r2, %r1
	ld.smax.h.a	%r1, %r2, %r1
	ld.add.h.a	%r1, %r2, %r1

	! BUNDLE
	sub.ws	%r1, %gz, %r1
	ld.add.h.a	%r1, %r2, %r1
	ld.and.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.h.a	%r1, %r2, %r1
	ld.xor.h.a	%r1, %r2, %r1
	ld.smin.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.smax.h.a	%r1, %r2, %r1
	ld.add.h.r	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.h.r	%r1, %r2, %r1
	ld.and.h.r	%r1, %r2, %r1
	ld.or.h.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.h.r	%r1, %r2, %r1
	ld.smin.h.r	%r1, %r2, %r1
	ld.smax.h.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.h.ar	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.h.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.h.ar	%r1, %r2, %r1
	ld.or.h.ar	%r1, %r2, %r1
	ld.xor.h.ar	%r1, %r2, %r1

	! BUNDLE
	ld.smin.h.ar	%r1, %r2, %r1
	ld.smax.h.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.h	%r1, %r2, %r1
	fence.sc
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.h	%r1, %r2, %r1
	fence.sc
	ld.and.h	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.h	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.h	%r1, %r2, %r1
	fence.sc
	ld.smin.h	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.smax.h	%r1, %r2, %r1
	slsrai	%r1, %r1, 48, 48

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end75:
	.size	test_atomic_load_op_i16, .Lfunc_end75-test_atomic_load_op_i16
	.size	.Ltest_atomic_load_op_i16$local, .Lfunc_end75-test_atomic_load_op_i16
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
	ld.add.h	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.h	%r1, %r2, %r1
	ld.and.h	%r1, %r2, %r1
	ld.or.h	%r1, %r2, %r1

	! BUNDLE
	ld.xor.h	%r1, %r2, %r1
	ld.umin.h	%r1, %r2, %r1
	ld.umax.h	%r1, %r2, %r1

	! BUNDLE
	ld.add.h.a	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.h.a	%r1, %r2, %r1
	ld.or.h.a	%r1, %r2, %r1
	ld.xor.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.umin.h.a	%r1, %r2, %r1
	ld.umax.h.a	%r1, %r2, %r1
	ld.add.h.a	%r1, %r2, %r1

	! BUNDLE
	sub.ws	%r1, %gz, %r1
	ld.add.h.a	%r1, %r2, %r1
	ld.and.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.h.a	%r1, %r2, %r1
	ld.xor.h.a	%r1, %r2, %r1
	ld.umin.h.a	%r1, %r2, %r1

	! BUNDLE
	ld.umax.h.a	%r1, %r2, %r1
	ld.add.h.r	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.h.r	%r1, %r2, %r1
	ld.and.h.r	%r1, %r2, %r1
	ld.or.h.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.h.r	%r1, %r2, %r1
	ld.umin.h.r	%r1, %r2, %r1
	ld.umax.h.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.h.ar	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.h.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.h.ar	%r1, %r2, %r1
	ld.or.h.ar	%r1, %r2, %r1
	ld.xor.h.ar	%r1, %r2, %r1

	! BUNDLE
	ld.umin.h.ar	%r1, %r2, %r1
	ld.umax.h.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.h	%r1, %r2, %r1
	fence.sc
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.h	%r1, %r2, %r1
	fence.sc
	ld.and.h	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.h	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.h	%r1, %r2, %r1
	fence.sc
	ld.umin.h	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.umax.h	%r1, %r2, %r1
	ret.f	0

.Lfunc_end76:
	.size	test_atomic_load_op_u16, .Lfunc_end76-test_atomic_load_op_u16
	.size	.Ltest_atomic_load_op_u16$local, .Lfunc_end76-test_atomic_load_op_u16
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
	ld.add.w	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.w	%r1, %r2, %r1
	ld.and.w	%r1, %r2, %r1
	ld.or.w	%r1, %r2, %r1

	! BUNDLE
	ld.xor.w	%r1, %r2, %r1
	ld.smin.w	%r1, %r2, %r1
	ld.smax.w	%r1, %r2, %r1

	! BUNDLE
	ld.add.w.a	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.w.a	%r1, %r2, %r1
	ld.or.w.a	%r1, %r2, %r1
	ld.xor.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.smin.w.a	%r1, %r2, %r1
	ld.smax.w.a	%r1, %r2, %r1
	ld.add.w.a	%r1, %r2, %r1

	! BUNDLE
	sub.ws	%r1, %gz, %r1
	ld.add.w.a	%r1, %r2, %r1
	ld.and.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.w.a	%r1, %r2, %r1
	ld.xor.w.a	%r1, %r2, %r1
	ld.smin.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.smax.w.a	%r1, %r2, %r1
	ld.add.w.r	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.w.r	%r1, %r2, %r1
	ld.and.w.r	%r1, %r2, %r1
	ld.or.w.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.w.r	%r1, %r2, %r1
	ld.smin.w.r	%r1, %r2, %r1
	ld.smax.w.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.w.ar	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.w.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.w.ar	%r1, %r2, %r1
	ld.or.w.ar	%r1, %r2, %r1
	ld.xor.w.ar	%r1, %r2, %r1

	! BUNDLE
	ld.smin.w.ar	%r1, %r2, %r1
	ld.smax.w.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.w	%r1, %r2, %r1
	fence.sc
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.w	%r1, %r2, %r1
	fence.sc
	ld.and.w	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.w	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.w	%r1, %r2, %r1
	fence.sc
	ld.smin.w	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.smax.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end77:
	.size	test_atomic_load_op_i32, .Lfunc_end77-test_atomic_load_op_i32
	.size	.Ltest_atomic_load_op_i32$local, .Lfunc_end77-test_atomic_load_op_i32
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
	ld.add.w	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.w	%r1, %r2, %r1
	ld.and.w	%r1, %r2, %r1
	ld.or.w	%r1, %r2, %r1

	! BUNDLE
	ld.xor.w	%r1, %r2, %r1
	ld.umin.w	%r1, %r2, %r1
	ld.umax.w	%r1, %r2, %r1

	! BUNDLE
	ld.add.w.a	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.w.a	%r1, %r2, %r1
	ld.or.w.a	%r1, %r2, %r1
	ld.xor.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.umin.w.a	%r1, %r2, %r1
	ld.umax.w.a	%r1, %r2, %r1
	ld.add.w.a	%r1, %r2, %r1

	! BUNDLE
	sub.ws	%r1, %gz, %r1
	ld.add.w.a	%r1, %r2, %r1
	ld.and.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.w.a	%r1, %r2, %r1
	ld.xor.w.a	%r1, %r2, %r1
	ld.umin.w.a	%r1, %r2, %r1

	! BUNDLE
	ld.umax.w.a	%r1, %r2, %r1
	ld.add.w.r	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.w.r	%r1, %r2, %r1
	ld.and.w.r	%r1, %r2, %r1
	ld.or.w.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.w.r	%r1, %r2, %r1
	ld.umin.w.r	%r1, %r2, %r1
	ld.umax.w.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.w.ar	%r1, %r2, %r1
	sub.ws	%r1, %gz, %r1
	ld.add.w.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.w.ar	%r1, %r2, %r1
	ld.or.w.ar	%r1, %r2, %r1
	ld.xor.w.ar	%r1, %r2, %r1

	! BUNDLE
	ld.umin.w.ar	%r1, %r2, %r1
	ld.umax.w.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.w	%r1, %r2, %r1
	fence.sc
	sub.ws	%r1, %gz, %r1

	! BUNDLE
	ld.add.w	%r1, %r2, %r1
	fence.sc
	ld.and.w	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.w	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.w	%r1, %r2, %r1
	fence.sc
	ld.umin.w	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.umax.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end78:
	.size	test_atomic_load_op_u32, .Lfunc_end78-test_atomic_load_op_u32
	.size	.Ltest_atomic_load_op_u32$local, .Lfunc_end78-test_atomic_load_op_u32
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
	ld.add.d	%r1, %r2, %r1
	sub	%r1, %gz, %r1

	! BUNDLE
	ld.add.d	%r1, %r2, %r1
	ld.and.d	%r1, %r2, %r1
	ld.or.d	%r1, %r2, %r1

	! BUNDLE
	ld.xor.d	%r1, %r2, %r1
	ld.smin.d	%r1, %r2, %r1
	ld.smax.d	%r1, %r2, %r1

	! BUNDLE
	ld.add.d.a	%r1, %r2, %r1
	sub	%r1, %gz, %r1
	ld.add.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.d.a	%r1, %r2, %r1
	ld.or.d.a	%r1, %r2, %r1
	ld.xor.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.smin.d.a	%r1, %r2, %r1
	ld.smax.d.a	%r1, %r2, %r1
	ld.add.d.a	%r1, %r2, %r1

	! BUNDLE
	sub	%r1, %gz, %r1
	ld.add.d.a	%r1, %r2, %r1
	ld.and.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.d.a	%r1, %r2, %r1
	ld.xor.d.a	%r1, %r2, %r1
	ld.smin.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.smax.d.a	%r1, %r2, %r1
	ld.add.d.r	%r1, %r2, %r1
	sub	%r1, %gz, %r1

	! BUNDLE
	ld.add.d.r	%r1, %r2, %r1
	ld.and.d.r	%r1, %r2, %r1
	ld.or.d.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.d.r	%r1, %r2, %r1
	ld.smin.d.r	%r1, %r2, %r1
	ld.smax.d.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.d.ar	%r1, %r2, %r1
	sub	%r1, %gz, %r1
	ld.add.d.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.d.ar	%r1, %r2, %r1
	ld.or.d.ar	%r1, %r2, %r1
	ld.xor.d.ar	%r1, %r2, %r1

	! BUNDLE
	ld.smin.d.ar	%r1, %r2, %r1
	ld.smax.d.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.d	%r1, %r2, %r1
	fence.sc
	sub	%r1, %gz, %r1

	! BUNDLE
	ld.add.d	%r1, %r2, %r1
	fence.sc
	ld.and.d	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.d	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.d	%r1, %r2, %r1
	fence.sc
	ld.smin.d	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.smax.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end79:
	.size	test_atomic_load_op_i64, .Lfunc_end79-test_atomic_load_op_i64
	.size	.Ltest_atomic_load_op_i64$local, .Lfunc_end79-test_atomic_load_op_i64
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
	ld.add.d	%r1, %r2, %r1
	sub	%r1, %gz, %r1

	! BUNDLE
	ld.add.d	%r1, %r2, %r1
	ld.and.d	%r1, %r2, %r1
	ld.or.d	%r1, %r2, %r1

	! BUNDLE
	ld.xor.d	%r1, %r2, %r1
	ld.umin.d	%r1, %r2, %r1
	ld.umax.d	%r1, %r2, %r1

	! BUNDLE
	ld.add.d.a	%r1, %r2, %r1
	sub	%r1, %gz, %r1
	ld.add.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.d.a	%r1, %r2, %r1
	ld.or.d.a	%r1, %r2, %r1
	ld.xor.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.umin.d.a	%r1, %r2, %r1
	ld.umax.d.a	%r1, %r2, %r1
	ld.add.d.a	%r1, %r2, %r1

	! BUNDLE
	sub	%r1, %gz, %r1
	ld.add.d.a	%r1, %r2, %r1
	ld.and.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.d.a	%r1, %r2, %r1
	ld.xor.d.a	%r1, %r2, %r1
	ld.umin.d.a	%r1, %r2, %r1

	! BUNDLE
	ld.umax.d.a	%r1, %r2, %r1
	ld.add.d.r	%r1, %r2, %r1
	sub	%r1, %gz, %r1

	! BUNDLE
	ld.add.d.r	%r1, %r2, %r1
	ld.and.d.r	%r1, %r2, %r1
	ld.or.d.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.d.r	%r1, %r2, %r1
	ld.umin.d.r	%r1, %r2, %r1
	ld.umax.d.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.d.ar	%r1, %r2, %r1
	sub	%r1, %gz, %r1
	ld.add.d.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.d.ar	%r1, %r2, %r1
	ld.or.d.ar	%r1, %r2, %r1
	ld.xor.d.ar	%r1, %r2, %r1

	! BUNDLE
	ld.umin.d.ar	%r1, %r2, %r1
	ld.umax.d.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.d	%r1, %r2, %r1
	fence.sc
	sub	%r1, %gz, %r1

	! BUNDLE
	ld.add.d	%r1, %r2, %r1
	fence.sc
	ld.and.d	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.d	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.d	%r1, %r2, %r1
	fence.sc
	ld.umin.d	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.umax.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end80:
	.size	test_atomic_load_op_u64, .Lfunc_end80-test_atomic_load_op_u64
	.size	.Ltest_atomic_load_op_u64$local, .Lfunc_end80-test_atomic_load_op_u64
                                        ! -- End function
	.globl	test_atomic_load_op_i128        ! -- Begin function test_atomic_load_op_i128
	.p2align	4
	.type	test_atomic_load_op_i128,@function
test_atomic_load_op_i128:               ! @test_atomic_load_op_i128
.Ltest_atomic_load_op_i128$local:
	.type	.Ltest_atomic_load_op_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ld.add.q	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1

	! BUNDLE
	ld.add.q	%r1, %r2, %r1
	ld.and.q	%r1, %r2, %r1
	ld.or.q	%r1, %r2, %r1

	! BUNDLE
	ld.xor.q	%r1, %r2, %r1
	ld.smin.q	%r1, %r2, %r1
	ld.smax.q	%r1, %r2, %r1

	! BUNDLE
	ld.add.q.a	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1
	ld.add.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.q.a	%r1, %r2, %r1
	ld.or.q.a	%r1, %r2, %r1
	ld.xor.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.smin.q.a	%r1, %r2, %r1
	ld.smax.q.a	%r1, %r2, %r1
	ld.add.q.a	%r1, %r2, %r1

	! BUNDLE
	sub.q	%r1, %gz, %r1
	ld.add.q.a	%r1, %r2, %r1
	ld.and.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.q.a	%r1, %r2, %r1
	ld.xor.q.a	%r1, %r2, %r1
	ld.smin.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.smax.q.a	%r1, %r2, %r1
	ld.add.q.r	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1

	! BUNDLE
	ld.add.q.r	%r1, %r2, %r1
	ld.and.q.r	%r1, %r2, %r1
	ld.or.q.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.q.r	%r1, %r2, %r1
	ld.smin.q.r	%r1, %r2, %r1
	ld.smax.q.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.q.ar	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1
	ld.add.q.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.q.ar	%r1, %r2, %r1
	ld.or.q.ar	%r1, %r2, %r1
	ld.xor.q.ar	%r1, %r2, %r1

	! BUNDLE
	ld.smin.q.ar	%r1, %r2, %r1
	ld.smax.q.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.q	%r1, %r2, %r1
	fence.sc
	sub.q	%r1, %gz, %r1

	! BUNDLE
	ld.add.q	%r1, %r2, %r1
	fence.sc
	ld.and.q	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.q	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.q	%r1, %r2, %r1
	fence.sc
	ld.smin.q	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.smax.q	%r1, %r2, %r1
	ret.f	0

.Lfunc_end81:
	.size	test_atomic_load_op_i128, .Lfunc_end81-test_atomic_load_op_i128
	.size	.Ltest_atomic_load_op_i128$local, .Lfunc_end81-test_atomic_load_op_i128
                                        ! -- End function
	.globl	test_atomic_load_op_u128        ! -- Begin function test_atomic_load_op_u128
	.p2align	4
	.type	test_atomic_load_op_u128,@function
test_atomic_load_op_u128:               ! @test_atomic_load_op_u128
.Ltest_atomic_load_op_u128$local:
	.type	.Ltest_atomic_load_op_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	ld.add.q	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1

	! BUNDLE
	ld.add.q	%r1, %r2, %r1
	ld.and.q	%r1, %r2, %r1
	ld.or.q	%r1, %r2, %r1

	! BUNDLE
	ld.xor.q	%r1, %r2, %r1
	ld.umin.q	%r1, %r2, %r1
	ld.umax.q	%r1, %r2, %r1

	! BUNDLE
	ld.add.q.a	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1
	ld.add.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.and.q.a	%r1, %r2, %r1
	ld.or.q.a	%r1, %r2, %r1
	ld.xor.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.umin.q.a	%r1, %r2, %r1
	ld.umax.q.a	%r1, %r2, %r1
	ld.add.q.a	%r1, %r2, %r1

	! BUNDLE
	sub.q	%r1, %gz, %r1
	ld.add.q.a	%r1, %r2, %r1
	ld.and.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.or.q.a	%r1, %r2, %r1
	ld.xor.q.a	%r1, %r2, %r1
	ld.umin.q.a	%r1, %r2, %r1

	! BUNDLE
	ld.umax.q.a	%r1, %r2, %r1
	ld.add.q.r	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1

	! BUNDLE
	ld.add.q.r	%r1, %r2, %r1
	ld.and.q.r	%r1, %r2, %r1
	ld.or.q.r	%r1, %r2, %r1

	! BUNDLE
	ld.xor.q.r	%r1, %r2, %r1
	ld.umin.q.r	%r1, %r2, %r1
	ld.umax.q.r	%r1, %r2, %r1

	! BUNDLE
	ld.add.q.ar	%r1, %r2, %r1
	sub.q	%r1, %gz, %r1
	ld.add.q.ar	%r1, %r2, %r1

	! BUNDLE
	ld.and.q.ar	%r1, %r2, %r1
	ld.or.q.ar	%r1, %r2, %r1
	ld.xor.q.ar	%r1, %r2, %r1

	! BUNDLE
	ld.umin.q.ar	%r1, %r2, %r1
	ld.umax.q.ar	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.add.q	%r1, %r2, %r1
	fence.sc
	sub.q	%r1, %gz, %r1

	! BUNDLE
	ld.add.q	%r1, %r2, %r1
	fence.sc
	ld.and.q	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.or.q	%r1, %r2, %r1
	fence.sc

	! BUNDLE
	ld.xor.q	%r1, %r2, %r1
	fence.sc
	ld.umin.q	%r1, %r2, %r1

	! BUNDLE
	fence.sc
	ld.umax.q	%r1, %r2, %r1
	ret.f	0

.Lfunc_end82:
	.size	test_atomic_load_op_u128, .Lfunc_end82-test_atomic_load_op_u128
	.size	.Ltest_atomic_load_op_u128$local, .Lfunc_end82-test_atomic_load_op_u128
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
	sta.b	%r4, %r1
	sta.b.r	%r4, %r2

	! BUNDLE
	fence.sc
	sta.b	%r4, %r3
	ret.f	0

.Lfunc_end83:
	.size	test_atomic_store_i8, .Lfunc_end83-test_atomic_store_i8
	.size	.Ltest_atomic_store_i8$local, .Lfunc_end83-test_atomic_store_i8
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
	sta.h	%r4, %r1
	sta.h.r	%r4, %r2

	! BUNDLE
	fence.sc
	sta.h	%r4, %r3
	ret.f	0

.Lfunc_end84:
	.size	test_atomic_store_i16, .Lfunc_end84-test_atomic_store_i16
	.size	.Ltest_atomic_store_i16$local, .Lfunc_end84-test_atomic_store_i16
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
	sta.w	%r4, %r1
	sta.w.r	%r4, %r2

	! BUNDLE
	fence.sc
	sta.w	%r4, %r3
	ret.f	0

.Lfunc_end85:
	.size	test_atomic_store_i32, .Lfunc_end85-test_atomic_store_i32
	.size	.Ltest_atomic_store_i32$local, .Lfunc_end85-test_atomic_store_i32
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
	sta.d	%r4, %r1
	sta.d.r	%r4, %r2

	! BUNDLE
	fence.sc
	sta.d	%r4, %r3
	ret.f	0

.Lfunc_end86:
	.size	test_atomic_store_i64, .Lfunc_end86-test_atomic_store_i64
	.size	.Ltest_atomic_store_i64$local, .Lfunc_end86-test_atomic_store_i64
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
	sta.q	%r4, %r1
	sta.q.r	%r4, %r2

	! BUNDLE
	fence.sc
	sta.q	%r4, %r3
	ret.f	0

.Lfunc_end87:
	.size	test_atomic_store_i128, .Lfunc_end87-test_atomic_store_i128
	.size	.Ltest_atomic_store_i128$local, .Lfunc_end87-test_atomic_store_i128
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
	ret.f	0

.Lfunc_end88:
	.size	test_i32_inv, .Lfunc_end88-test_i32_inv
	.size	.Ltest_i32_inv$local, .Lfunc_end88-test_i32_inv
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
	ret.f	0

.Lfunc_end89:
	.size	test_i64_inv, .Lfunc_end89-test_i64_inv
	.size	.Ltest_i64_inv$local, .Lfunc_end89-test_i64_inv
                                        ! -- End function
	.globl	test_i128_inv                   ! -- Begin function test_i128_inv
	.p2align	4
	.type	test_i128_inv,@function
test_i128_inv:                          ! @test_i128_inv
.Ltest_i128_inv$local:
	.type	.Ltest_i128_inv$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori	%r1, %r1, -1
	ret.f	0

.Lfunc_end90:
	.size	test_i128_inv, .Lfunc_end90-test_i128_inv
	.size	.Ltest_i128_inv$local, .Lfunc_end90-test_i128_inv
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
	ret.f	0

.Lfunc_end91:
	.size	test_i32_and, .Lfunc_end91-test_i32_and
	.size	.Ltest_i32_and$local, .Lfunc_end91-test_i32_and
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
	ret.f	0

.Lfunc_end92:
	.size	test_i64_and, .Lfunc_end92-test_i64_and
	.size	.Ltest_i64_and$local, .Lfunc_end92-test_i64_and
                                        ! -- End function
	.globl	test_i128_and                   ! -- Begin function test_i128_and
	.p2align	4
	.type	test_i128_and,@function
test_i128_and:                          ! @test_i128_and
.Ltest_i128_and$local:
	.type	.Ltest_i128_and$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	and	%r1, %r2, %r1
	ret.f	0

.Lfunc_end93:
	.size	test_i128_and, .Lfunc_end93-test_i128_and
	.size	.Ltest_i128_and$local, .Lfunc_end93-test_i128_and
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
	ret.f	0

.Lfunc_end94:
	.size	test_i32_andi, .Lfunc_end94-test_i32_andi
	.size	.Ltest_i32_andi$local, .Lfunc_end94-test_i32_andi
                                        ! -- End function
	.globl	test_i32_andi_ext               ! -- Begin function test_i32_andi_ext
	.p2align	4
	.type	test_i32_andi_ext,@function
test_i32_andi_ext:                      ! @test_i32_andi_ext
.Ltest_i32_andi_ext$local:
	.type	.Ltest_i32_andi_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end95:
	.size	test_i32_andi_ext, .Lfunc_end95-test_i32_andi_ext
	.size	.Ltest_i32_andi_ext$local, .Lfunc_end95-test_i32_andi_ext
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
	ret.f	0

.Lfunc_end96:
	.size	test_i64_andi, .Lfunc_end96-test_i64_andi
	.size	.Ltest_i64_andi$local, .Lfunc_end96-test_i64_andi
                                        ! -- End function
	.globl	test_i64_andi_ext               ! -- Begin function test_i64_andi_ext
	.p2align	4
	.type	test_i64_andi_ext,@function
test_i64_andi_ext:                      ! @test_i64_andi_ext
.Ltest_i64_andi_ext$local:
	.type	.Ltest_i64_andi_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end97:
	.size	test_i64_andi_ext, .Lfunc_end97-test_i64_andi_ext
	.size	.Ltest_i64_andi_ext$local, .Lfunc_end97-test_i64_andi_ext
                                        ! -- End function
	.globl	test_i128_andi                  ! -- Begin function test_i128_andi
	.p2align	4
	.type	test_i128_andi,@function
test_i128_andi:                         ! @test_i128_andi
.Ltest_i128_andi$local:
	.type	.Ltest_i128_andi$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi	%r1, %r1, 12345
	ret.f	0

.Lfunc_end98:
	.size	test_i128_andi, .Lfunc_end98-test_i128_andi
	.size	.Ltest_i128_andi$local, .Lfunc_end98-test_i128_andi
                                        ! -- End function
	.globl	test_i128_andi_ext              ! -- Begin function test_i128_andi_ext
	.p2align	4
	.type	test_i128_andi_ext,@function
test_i128_andi_ext:                     ! @test_i128_andi_ext
.Ltest_i128_andi_ext$local:
	.type	.Ltest_i128_andi_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andi.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end99:
	.size	test_i128_andi_ext, .Lfunc_end99-test_i128_andi_ext
	.size	.Ltest_i128_andi_ext$local, .Lfunc_end99-test_i128_andi_ext
                                        ! -- End function
	.globl	test_i32_andn                   ! -- Begin function test_i32_andn
	.p2align	4
	.type	test_i32_andn,@function
test_i32_andn:                          ! @test_i32_andn
.Ltest_i32_andn$local:
	.type	.Ltest_i32_andn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	andn	%r1, %r1, %r2
	ret.f	0

.Lfunc_end100:
	.size	test_i32_andn, .Lfunc_end100-test_i32_andn
	.size	.Ltest_i32_andn$local, .Lfunc_end100-test_i32_andn
                                        ! -- End function
	.globl	test_i64_andn                   ! -- Begin function test_i64_andn
	.p2align	4
	.type	test_i64_andn,@function
test_i64_andn:                          ! @test_i64_andn
.Ltest_i64_andn$local:
	.type	.Ltest_i64_andn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	andn	%r1, %r1, %r2
	ret.f	0

.Lfunc_end101:
	.size	test_i64_andn, .Lfunc_end101-test_i64_andn
	.size	.Ltest_i64_andn$local, .Lfunc_end101-test_i64_andn
                                        ! -- End function
	.globl	test_i128_andn                  ! -- Begin function test_i128_andn
	.p2align	4
	.type	test_i128_andn,@function
test_i128_andn:                         ! @test_i128_andn
.Ltest_i128_andn$local:
	.type	.Ltest_i128_andn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	andn	%r1, %r1, %r2
	ret.f	0

.Lfunc_end102:
	.size	test_i128_andn, .Lfunc_end102-test_i128_andn
	.size	.Ltest_i128_andn$local, .Lfunc_end102-test_i128_andn
                                        ! -- End function
	.globl	test_i32_andni                  ! -- Begin function test_i32_andni
	.p2align	4
	.type	test_i32_andni,@function
test_i32_andni:                         ! @test_i32_andni
.Ltest_i32_andni$local:
	.type	.Ltest_i32_andni$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andni	%r1, %r1, 12345
	ret.f	0

.Lfunc_end103:
	.size	test_i32_andni, .Lfunc_end103-test_i32_andni
	.size	.Ltest_i32_andni$local, .Lfunc_end103-test_i32_andni
                                        ! -- End function
	.globl	test_i32_andni_ext              ! -- Begin function test_i32_andni_ext
	.p2align	4
	.type	test_i32_andni_ext,@function
test_i32_andni_ext:                     ! @test_i32_andni_ext
.Ltest_i32_andni_ext$local:
	.type	.Ltest_i32_andni_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andni.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end104:
	.size	test_i32_andni_ext, .Lfunc_end104-test_i32_andni_ext
	.size	.Ltest_i32_andni_ext$local, .Lfunc_end104-test_i32_andni_ext
                                        ! -- End function
	.globl	test_i64_andni                  ! -- Begin function test_i64_andni
	.p2align	4
	.type	test_i64_andni,@function
test_i64_andni:                         ! @test_i64_andni
.Ltest_i64_andni$local:
	.type	.Ltest_i64_andni$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andni	%r1, %r1, 12345
	ret.f	0

.Lfunc_end105:
	.size	test_i64_andni, .Lfunc_end105-test_i64_andni
	.size	.Ltest_i64_andni$local, .Lfunc_end105-test_i64_andni
                                        ! -- End function
	.globl	test_i64_andni_ext              ! -- Begin function test_i64_andni_ext
	.p2align	4
	.type	test_i64_andni_ext,@function
test_i64_andni_ext:                     ! @test_i64_andni_ext
.Ltest_i64_andni_ext$local:
	.type	.Ltest_i64_andni_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andni.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end106:
	.size	test_i64_andni_ext, .Lfunc_end106-test_i64_andni_ext
	.size	.Ltest_i64_andni_ext$local, .Lfunc_end106-test_i64_andni_ext
                                        ! -- End function
	.globl	test_i128_andni                 ! -- Begin function test_i128_andni
	.p2align	4
	.type	test_i128_andni,@function
test_i128_andni:                        ! @test_i128_andni
.Ltest_i128_andni$local:
	.type	.Ltest_i128_andni$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andni	%r1, %r1, 12345
	ret.f	0

.Lfunc_end107:
	.size	test_i128_andni, .Lfunc_end107-test_i128_andni
	.size	.Ltest_i128_andni$local, .Lfunc_end107-test_i128_andni
                                        ! -- End function
	.globl	test_i128_andni_ext             ! -- Begin function test_i128_andni_ext
	.p2align	4
	.type	test_i128_andni_ext,@function
test_i128_andni_ext:                    ! @test_i128_andni_ext
.Ltest_i128_andni_ext$local:
	.type	.Ltest_i128_andni_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	andni.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end108:
	.size	test_i128_andni_ext, .Lfunc_end108-test_i128_andni_ext
	.size	.Ltest_i128_andni_ext$local, .Lfunc_end108-test_i128_andni_ext
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
	ret.f	0

.Lfunc_end109:
	.size	test_i32_or, .Lfunc_end109-test_i32_or
	.size	.Ltest_i32_or$local, .Lfunc_end109-test_i32_or
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
	ret.f	0

.Lfunc_end110:
	.size	test_i64_or, .Lfunc_end110-test_i64_or
	.size	.Ltest_i64_or$local, .Lfunc_end110-test_i64_or
                                        ! -- End function
	.globl	test_i128_or                    ! -- Begin function test_i128_or
	.p2align	4
	.type	test_i128_or,@function
test_i128_or:                           ! @test_i128_or
.Ltest_i128_or$local:
	.type	.Ltest_i128_or$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	or	%r1, %r2, %r1
	ret.f	0

.Lfunc_end111:
	.size	test_i128_or, .Lfunc_end111-test_i128_or
	.size	.Ltest_i128_or$local, .Lfunc_end111-test_i128_or
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
	ret.f	0

.Lfunc_end112:
	.size	test_i32_ori, .Lfunc_end112-test_i32_ori
	.size	.Ltest_i32_ori$local, .Lfunc_end112-test_i32_ori
                                        ! -- End function
	.globl	test_i32_ori_ext                ! -- Begin function test_i32_ori_ext
	.p2align	4
	.type	test_i32_ori_ext,@function
test_i32_ori_ext:                       ! @test_i32_ori_ext
.Ltest_i32_ori_ext$local:
	.type	.Ltest_i32_ori_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ori.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end113:
	.size	test_i32_ori_ext, .Lfunc_end113-test_i32_ori_ext
	.size	.Ltest_i32_ori_ext$local, .Lfunc_end113-test_i32_ori_ext
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
	ret.f	0

.Lfunc_end114:
	.size	test_i64_ori, .Lfunc_end114-test_i64_ori
	.size	.Ltest_i64_ori$local, .Lfunc_end114-test_i64_ori
                                        ! -- End function
	.globl	test_i64_ori_ext                ! -- Begin function test_i64_ori_ext
	.p2align	4
	.type	test_i64_ori_ext,@function
test_i64_ori_ext:                       ! @test_i64_ori_ext
.Ltest_i64_ori_ext$local:
	.type	.Ltest_i64_ori_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ori.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end115:
	.size	test_i64_ori_ext, .Lfunc_end115-test_i64_ori_ext
	.size	.Ltest_i64_ori_ext$local, .Lfunc_end115-test_i64_ori_ext
                                        ! -- End function
	.globl	test_i128_ori                   ! -- Begin function test_i128_ori
	.p2align	4
	.type	test_i128_ori,@function
test_i128_ori:                          ! @test_i128_ori
.Ltest_i128_ori$local:
	.type	.Ltest_i128_ori$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ori	%r1, %r1, 12345
	ret.f	0

.Lfunc_end116:
	.size	test_i128_ori, .Lfunc_end116-test_i128_ori
	.size	.Ltest_i128_ori$local, .Lfunc_end116-test_i128_ori
                                        ! -- End function
	.globl	test_i128_ori_ext               ! -- Begin function test_i128_ori_ext
	.p2align	4
	.type	test_i128_ori_ext,@function
test_i128_ori_ext:                      ! @test_i128_ori_ext
.Ltest_i128_ori_ext$local:
	.type	.Ltest_i128_ori_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ori.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end117:
	.size	test_i128_ori_ext, .Lfunc_end117-test_i128_ori_ext
	.size	.Ltest_i128_ori_ext$local, .Lfunc_end117-test_i128_ori_ext
                                        ! -- End function
	.globl	test_i32_orn                    ! -- Begin function test_i32_orn
	.p2align	4
	.type	test_i32_orn,@function
test_i32_orn:                           ! @test_i32_orn
.Ltest_i32_orn$local:
	.type	.Ltest_i32_orn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	orn	%r1, %r1, %r2
	ret.f	0

.Lfunc_end118:
	.size	test_i32_orn, .Lfunc_end118-test_i32_orn
	.size	.Ltest_i32_orn$local, .Lfunc_end118-test_i32_orn
                                        ! -- End function
	.globl	test_i64_orn                    ! -- Begin function test_i64_orn
	.p2align	4
	.type	test_i64_orn,@function
test_i64_orn:                           ! @test_i64_orn
.Ltest_i64_orn$local:
	.type	.Ltest_i64_orn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	orn	%r1, %r1, %r2
	ret.f	0

.Lfunc_end119:
	.size	test_i64_orn, .Lfunc_end119-test_i64_orn
	.size	.Ltest_i64_orn$local, .Lfunc_end119-test_i64_orn
                                        ! -- End function
	.globl	test_i128_orn                   ! -- Begin function test_i128_orn
	.p2align	4
	.type	test_i128_orn,@function
test_i128_orn:                          ! @test_i128_orn
.Ltest_i128_orn$local:
	.type	.Ltest_i128_orn$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	orn	%r1, %r1, %r2
	ret.f	0

.Lfunc_end120:
	.size	test_i128_orn, .Lfunc_end120-test_i128_orn
	.size	.Ltest_i128_orn$local, .Lfunc_end120-test_i128_orn
                                        ! -- End function
	.globl	test_i32_orni                   ! -- Begin function test_i32_orni
	.p2align	4
	.type	test_i32_orni,@function
test_i32_orni:                          ! @test_i32_orni
.Ltest_i32_orni$local:
	.type	.Ltest_i32_orni$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	orni	%r1, %r1, 12345
	ret.f	0

.Lfunc_end121:
	.size	test_i32_orni, .Lfunc_end121-test_i32_orni
	.size	.Ltest_i32_orni$local, .Lfunc_end121-test_i32_orni
                                        ! -- End function
	.globl	test_i32_orni_ext               ! -- Begin function test_i32_orni_ext
	.p2align	4
	.type	test_i32_orni_ext,@function
test_i32_orni_ext:                      ! @test_i32_orni_ext
.Ltest_i32_orni_ext$local:
	.type	.Ltest_i32_orni_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	orni.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end122:
	.size	test_i32_orni_ext, .Lfunc_end122-test_i32_orni_ext
	.size	.Ltest_i32_orni_ext$local, .Lfunc_end122-test_i32_orni_ext
                                        ! -- End function
	.globl	test_i64_orni                   ! -- Begin function test_i64_orni
	.p2align	4
	.type	test_i64_orni,@function
test_i64_orni:                          ! @test_i64_orni
.Ltest_i64_orni$local:
	.type	.Ltest_i64_orni$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	orni	%r1, %r1, 12345
	ret.f	0

.Lfunc_end123:
	.size	test_i64_orni, .Lfunc_end123-test_i64_orni
	.size	.Ltest_i64_orni$local, .Lfunc_end123-test_i64_orni
                                        ! -- End function
	.globl	test_i64_orni_ext               ! -- Begin function test_i64_orni_ext
	.p2align	4
	.type	test_i64_orni_ext,@function
test_i64_orni_ext:                      ! @test_i64_orni_ext
.Ltest_i64_orni_ext$local:
	.type	.Ltest_i64_orni_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	orni.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end124:
	.size	test_i64_orni_ext, .Lfunc_end124-test_i64_orni_ext
	.size	.Ltest_i64_orni_ext$local, .Lfunc_end124-test_i64_orni_ext
                                        ! -- End function
	.globl	test_i128_orni                  ! -- Begin function test_i128_orni
	.p2align	4
	.type	test_i128_orni,@function
test_i128_orni:                         ! @test_i128_orni
.Ltest_i128_orni$local:
	.type	.Ltest_i128_orni$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	orni	%r1, %r1, 12345
	ret.f	0

.Lfunc_end125:
	.size	test_i128_orni, .Lfunc_end125-test_i128_orni
	.size	.Ltest_i128_orni$local, .Lfunc_end125-test_i128_orni
                                        ! -- End function
	.globl	test_i128_orni_ext              ! -- Begin function test_i128_orni_ext
	.p2align	4
	.type	test_i128_orni_ext,@function
test_i128_orni_ext:                     ! @test_i128_orni_ext
.Ltest_i128_orni_ext$local:
	.type	.Ltest_i128_orni_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	orni.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end126:
	.size	test_i128_orni_ext, .Lfunc_end126-test_i128_orni_ext
	.size	.Ltest_i128_orni_ext$local, .Lfunc_end126-test_i128_orni_ext
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
	ret.f	0

.Lfunc_end127:
	.size	test_i32_xor, .Lfunc_end127-test_i32_xor
	.size	.Ltest_i32_xor$local, .Lfunc_end127-test_i32_xor
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
	ret.f	0

.Lfunc_end128:
	.size	test_i64_xor, .Lfunc_end128-test_i64_xor
	.size	.Ltest_i64_xor$local, .Lfunc_end128-test_i64_xor
                                        ! -- End function
	.globl	test_i128_xor                   ! -- Begin function test_i128_xor
	.p2align	4
	.type	test_i128_xor,@function
test_i128_xor:                          ! @test_i128_xor
.Ltest_i128_xor$local:
	.type	.Ltest_i128_xor$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	xor	%r1, %r2, %r1
	ret.f	0

.Lfunc_end129:
	.size	test_i128_xor, .Lfunc_end129-test_i128_xor
	.size	.Ltest_i128_xor$local, .Lfunc_end129-test_i128_xor
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
	ret.f	0

.Lfunc_end130:
	.size	test_i32_xori, .Lfunc_end130-test_i32_xori
	.size	.Ltest_i32_xori$local, .Lfunc_end130-test_i32_xori
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end131:
	.size	test_i32_xori_ext, .Lfunc_end131-test_i32_xori_ext
	.size	.Ltest_i32_xori_ext$local, .Lfunc_end131-test_i32_xori_ext
                                        ! -- End function
	.globl	test_i64_xori                   ! -- Begin function test_i64_xori
	.p2align	4
	.type	test_i64_xori,@function
test_i64_xori:                          ! @test_i64_xori
.Ltest_i64_xori$local:
	.type	.Ltest_i64_xori$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori	%r1, %r1, 12345
	ret.f	0

.Lfunc_end132:
	.size	test_i64_xori, .Lfunc_end132-test_i64_xori
	.size	.Ltest_i64_xori$local, .Lfunc_end132-test_i64_xori
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end133:
	.size	test_i64_xori_ext, .Lfunc_end133-test_i64_xori_ext
	.size	.Ltest_i64_xori_ext$local, .Lfunc_end133-test_i64_xori_ext
                                        ! -- End function
	.globl	test_i128_xori                  ! -- Begin function test_i128_xori
	.p2align	4
	.type	test_i128_xori,@function
test_i128_xori:                         ! @test_i128_xori
.Ltest_i128_xori$local:
	.type	.Ltest_i128_xori$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori	%r1, %r1, 12345
	ret.f	0

.Lfunc_end134:
	.size	test_i128_xori, .Lfunc_end134-test_i128_xori
	.size	.Ltest_i128_xori$local, .Lfunc_end134-test_i128_xori
                                        ! -- End function
	.globl	test_i128_xori_ext              ! -- Begin function test_i128_xori_ext
	.p2align	4
	.type	test_i128_xori_ext,@function
test_i128_xori_ext:                     ! @test_i128_xori_ext
.Ltest_i128_xori_ext$local:
	.type	.Ltest_i128_xori_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	xori.l	%r1, %r1, 123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end135:
	.size	test_i128_xori_ext, .Lfunc_end135-test_i128_xori_ext
	.size	.Ltest_i128_xori_ext$local, .Lfunc_end135-test_i128_xori_ext
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
	ret.f	0

.Lfunc_end136:
	.size	test_i1_and, .Lfunc_end136-test_i1_and
	.size	.Ltest_i1_and$local, .Lfunc_end136-test_i1_and
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
	ret.f	0

.Lfunc_end137:
	.size	test_i1_or, .Lfunc_end137-test_i1_or
	.size	.Ltest_i1_or$local, .Lfunc_end137-test_i1_or
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
	ret.f	0

.Lfunc_end138:
	.size	test_i1_xor, .Lfunc_end138-test_i1_xor
	.size	.Ltest_i1_xor$local, .Lfunc_end138-test_i1_xor
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
	ret.f	0

.Lfunc_end139:
	.size	test_i1_not, .Lfunc_end139-test_i1_not
	.size	.Ltest_i1_not$local, .Lfunc_end139-test_i1_not
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end140:
	.size	test_i1_andn, .Lfunc_end140-test_i1_andn
	.size	.Ltest_i1_andn$local, .Lfunc_end140-test_i1_andn
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end141:
	.size	test_i1_orn, .Lfunc_end141-test_i1_orn
	.size	.Ltest_i1_orn$local, .Lfunc_end141-test_i1_orn
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end142:
	.size	test_i1_nand, .Lfunc_end142-test_i1_nand
	.size	.Ltest_i1_nand$local, .Lfunc_end142-test_i1_nand
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
	ret.f	0
	nop	0

.Lfunc_end143:
	.size	test_i1_to_i32, .Lfunc_end143-test_i1_to_i32
	.size	.Ltest_i1_to_i32$local, .Lfunc_end143-test_i1_to_i32
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
	ret.f	0
	nop	0

.Lfunc_end144:
	.size	test_i1_to_i64, .Lfunc_end144-test_i1_to_i64
	.size	.Ltest_i1_to_i64$local, .Lfunc_end144-test_i1_to_i64
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
	ret.f	0

.Lfunc_end145:
	.size	test_i1_inv, .Lfunc_end145-test_i1_inv
	.size	.Ltest_i1_inv$local, .Lfunc_end145-test_i1_inv
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
	ret.f	0

.Lfunc_end146:
	.size	test_imm_to_i1, .Lfunc_end146-test_imm_to_i1
	.size	.Ltest_imm_to_i1$local, .Lfunc_end146-test_imm_to_i1
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
	st.b.r	%r1, %pcfwd(test_bool_true)

	! BUNDLE
	st.b.r	%gz, %pcfwd(test_bool_false)
	st.b.r	%r1, %pcfwd(test_i8_true)
	st.b.r	%gz, %pcfwd(test_i8_false)

	! BUNDLE
	st.h.r	%r1, %pcfwd(test_i16_true)
	st.h.r	%gz, %pcfwd(test_i16_false)
	st.w.r	%r1, %pcfwd(test_i32_true)

	! BUNDLE
	st.w.r	%gz, %pcfwd(test_i32_false)
	st.d.r	%r1, %pcfwd(test_i64_true)
	st.d.r	%gz, %pcfwd(test_i64_false)

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end147:
	.size	test_write_i1, .Lfunc_end147-test_write_i1
	.size	.Ltest_write_i1$local, .Lfunc_end147-test_write_i1
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
	nuli.ne.d	%r1, 534, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end148:
	.size	test_branch_i64_eqi, .Lfunc_end148-test_branch_i64_eqi
	.size	.Ltest_branch_i64_eqi$local, .Lfunc_end148-test_branch_i64_eqi
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
	nuli.eq.d	%r1, 534, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end149:
	.size	test_branch_i64_nei, .Lfunc_end149-test_branch_i64_nei
	.size	.Ltest_branch_i64_nei$local, .Lfunc_end149-test_branch_i64_nei
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
	nulsi.lt.d	%r1, 534, 0, 2
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end150:
	.size	test_branch_i64_lti, .Lfunc_end150-test_branch_i64_lti
	.size	.Ltest_branch_i64_lti$local, .Lfunc_end150-test_branch_i64_lti
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
	nulsi.lt.d	%r1, 535, 0, 2
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end151:
	.size	test_branch_i64_lei, .Lfunc_end151-test_branch_i64_lei
	.size	.Ltest_branch_i64_lei$local, .Lfunc_end151-test_branch_i64_lei
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
	nulsi.lt.d	%r1, 535, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end152:
	.size	test_branch_i64_gti, .Lfunc_end152-test_branch_i64_gti
	.size	.Ltest_branch_i64_gti$local, .Lfunc_end152-test_branch_i64_gti
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
	nulsi.lt.d	%r1, 534, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end153:
	.size	test_branch_i64_gei, .Lfunc_end153-test_branch_i64_gei
	.size	.Ltest_branch_i64_gei$local, .Lfunc_end153-test_branch_i64_gei
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
	nuli.ne.d.l	%r1, 12345678, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end154:
	.size	test_branch_ext_i64_eqi, .Lfunc_end154-test_branch_ext_i64_eqi
	.size	.Ltest_branch_ext_i64_eqi$local, .Lfunc_end154-test_branch_ext_i64_eqi
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
	nuli.eq.d.l	%r1, 12345678, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end155:
	.size	test_branch_ext_i64_nei, .Lfunc_end155-test_branch_ext_i64_nei
	.size	.Ltest_branch_ext_i64_nei$local, .Lfunc_end155-test_branch_ext_i64_nei
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
	nulsi.lt.d.l	%r1, 12345678, 0, 2

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end156:
	.size	test_branch_ext_i64_lti, .Lfunc_end156-test_branch_ext_i64_lti
	.size	.Ltest_branch_ext_i64_lti$local, .Lfunc_end156-test_branch_ext_i64_lti
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
	nulsi.lt.d.l	%r1, 12345679, 0, 2

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end157:
	.size	test_branch_ext_i64_lei, .Lfunc_end157-test_branch_ext_i64_lei
	.size	.Ltest_branch_ext_i64_lei$local, .Lfunc_end157-test_branch_ext_i64_lei
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
	nulsi.lt.d.l	%r1, 12345679, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end158:
	.size	test_branch_ext_i64_gti, .Lfunc_end158-test_branch_ext_i64_gti
	.size	.Ltest_branch_ext_i64_gti$local, .Lfunc_end158-test_branch_ext_i64_gti
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
	nulsi.lt.d.l	%r1, 12345678, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end159:
	.size	test_branch_ext_i64_gei, .Lfunc_end159-test_branch_ext_i64_gei
	.size	.Ltest_branch_ext_i64_gei$local, .Lfunc_end159-test_branch_ext_i64_gei
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
	nuli.ne.w	%r1, 534, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end160:
	.size	test_branch_i32_eqi, .Lfunc_end160-test_branch_i32_eqi
	.size	.Ltest_branch_i32_eqi$local, .Lfunc_end160-test_branch_i32_eqi
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
	nuli.eq.w	%r1, 534, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end161:
	.size	test_branch_i32_nei, .Lfunc_end161-test_branch_i32_nei
	.size	.Ltest_branch_i32_nei$local, .Lfunc_end161-test_branch_i32_nei
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
	nulsi.lt.w	%r1, 534, 0, 2
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end162:
	.size	test_branch_i32_lti, .Lfunc_end162-test_branch_i32_lti
	.size	.Ltest_branch_i32_lti$local, .Lfunc_end162-test_branch_i32_lti
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
	nulsi.lt.w	%r1, 535, 0, 2
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end163:
	.size	test_branch_i32_lei, .Lfunc_end163-test_branch_i32_lei
	.size	.Ltest_branch_i32_lei$local, .Lfunc_end163-test_branch_i32_lei
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
	nulsi.lt.w	%r1, 535, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end164:
	.size	test_branch_i32_gti, .Lfunc_end164-test_branch_i32_gti
	.size	.Ltest_branch_i32_gti$local, .Lfunc_end164-test_branch_i32_gti
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
	nulsi.lt.w	%r1, 534, 2, 0
	ca.rf	%r3, %pcfwd(.Lstr.98)

	! BUNDLE
	call.r	%r2, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end165:
	.size	test_branch_i32_gei, .Lfunc_end165-test_branch_i32_gei
	.size	.Ltest_branch_i32_gei$local, .Lfunc_end165-test_branch_i32_gei
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
	nuli.ne.w.l	%r1, 12345678, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end166:
	.size	test_branch_ext_i32_eqi, .Lfunc_end166-test_branch_ext_i32_eqi
	.size	.Ltest_branch_ext_i32_eqi$local, .Lfunc_end166-test_branch_ext_i32_eqi
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
	nuli.eq.w.l	%r1, 12345678, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end167:
	.size	test_branch_ext_i32_nei, .Lfunc_end167-test_branch_ext_i32_nei
	.size	.Ltest_branch_ext_i32_nei$local, .Lfunc_end167-test_branch_ext_i32_nei
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
	nulsi.lt.w.l	%r1, 12345678, 0, 2

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end168:
	.size	test_branch_ext_i32_lti, .Lfunc_end168-test_branch_ext_i32_lti
	.size	.Ltest_branch_ext_i32_lti$local, .Lfunc_end168-test_branch_ext_i32_lti
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
	nulsi.lt.w.l	%r1, 12345679, 0, 2

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end169:
	.size	test_branch_ext_i32_lei, .Lfunc_end169-test_branch_ext_i32_lei
	.size	.Ltest_branch_ext_i32_lei$local, .Lfunc_end169-test_branch_ext_i32_lei
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
	nulsi.lt.w.l	%r1, 12345679, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end170:
	.size	test_branch_ext_i32_gti, .Lfunc_end170-test_branch_ext_i32_gti
	.size	.Ltest_branch_ext_i32_gti$local, .Lfunc_end170-test_branch_ext_i32_gti
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
	nulsi.lt.w.l	%r1, 12345678, 2, 0

	! BUNDLE
	ca.rf	%r3, %pcfwd(.Lstr.98)
	call.r	%r2, %jpc(puts)
	ret.f	0

.Lfunc_end171:
	.size	test_branch_ext_i32_gei, .Lfunc_end171-test_branch_ext_i32_gei
	.size	.Ltest_branch_ext_i32_gei$local, .Lfunc_end171-test_branch_ext_i32_gei
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
	alloc.sp	5, 16
	nul.bs	%r1, %r2, 4, 0
	st.d	%r2, %sp, 8

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.1)
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end172:
	.size	test_bit_1, .Lfunc_end172-test_bit_1
	.size	.Ltest_bit_1$local, .Lfunc_end172-test_bit_1
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
	alloc.sp	5, 16
	nul.bc	%r1, %r2, 4, 0
	st.d	%r2, %sp, 8

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.2)
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end173:
	.size	test_bit_2, .Lfunc_end173-test_bit_2
	.size	.Ltest_bit_2$local, .Lfunc_end173-test_bit_2
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
	alloc.sp	5, 16
	nul.bs	%r1, %r2, 4, 0
	st.d	%r2, %sp, 8

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.3)
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end174:
	.size	test_bit_3, .Lfunc_end174-test_bit_3
	.size	.Ltest_bit_3$local, .Lfunc_end174-test_bit_3
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
	alloc.sp	5, 16
	nul.bci	%r1, 15, 4, 0
	st.d	%r2, %sp, 8

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.4)
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end175:
	.size	test_bit_4, .Lfunc_end175-test_bit_4
	.size	.Ltest_bit_4$local, .Lfunc_end175-test_bit_4
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
	alloc.sp	5, 16
	nul.bsi	%r1, 14, 4, 0
	st.d	%r2, %sp, 8

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.5)
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end176:
	.size	test_bit_5, .Lfunc_end176-test_bit_5
	.size	.Ltest_bit_5$local, .Lfunc_end176-test_bit_5
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
	alloc.sp	5, 16
	nul.bs	%r1, %r2, 4, 0
	st.w	%r2, %sp, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.6)
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end177:
	.size	test_bit_i32_1, .Lfunc_end177-test_bit_i32_1
	.size	.Ltest_bit_i32_1$local, .Lfunc_end177-test_bit_i32_1
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
	alloc.sp	5, 16
	nul.bc	%r1, %r2, 4, 0
	st.w	%r2, %sp, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.7)
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end178:
	.size	test_bit_i32_2, .Lfunc_end178-test_bit_i32_2
	.size	.Ltest_bit_i32_2$local, .Lfunc_end178-test_bit_i32_2
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
	alloc.sp	5, 16
	nul.bs	%r1, %r2, 4, 0
	st.w	%r2, %sp, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.8)
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end179:
	.size	test_bit_i32_3, .Lfunc_end179-test_bit_i32_3
	.size	.Ltest_bit_i32_3$local, .Lfunc_end179-test_bit_i32_3
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
	alloc.sp	5, 16
	nul.bci	%r1, 15, 4, 0
	st.w	%r2, %sp, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.9)
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end180:
	.size	test_bit_i32_4, .Lfunc_end180-test_bit_i32_4
	.size	.Ltest_bit_i32_4$local, .Lfunc_end180-test_bit_i32_4
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
	alloc.sp	5, 16
	nul.bsi	%r1, 14, 4, 0
	st.w	%r2, %sp, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.10)
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end181:
	.size	test_bit_i32_5, .Lfunc_end181-test_bit_i32_5
	.size	.Ltest_bit_i32_5$local, .Lfunc_end181-test_bit_i32_5
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
	alloc.sp	5, 16
	mov	%r2, %r1
	nulm.all	%r1, 2000, 4, 0

	! BUNDLE
	addi	%r1, %r2, 2000
	ca.rf	%r4, %pcfwd(.L.str.11)
	st.d	%r1, %sp, 0

	! BUNDLE
	call.r	%r3, %jpc(printf)
	nulm.notall	%r2, 1792, 4, 0
	addi	%r1, %r1, 5000

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.11)
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)

	! BUNDLE
	nulm.none	%r2, 1536, 4, 0
	addi	%r1, %r1, 3000
	ca.rf	%r4, %pcfwd(.L.str.11)

	! BUNDLE
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	nulm.any	%r2, 448, 4, 0

	! BUNDLE
	addi	%r1, %r1, 4000
	ca.rf	%r4, %pcfwd(.L.str.11)
	st.d	%r1, %sp, 0

	! BUNDLE
	call.r	%r3, %jpc(printf)
	ret.f	16
	nop	0

.Lfunc_end182:
	.size	test_ball, .Lfunc_end182-test_ball
	.size	.Ltest_ball$local, .Lfunc_end182-test_ball
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
	cnt.pop	%r1, %r1, 31
	ret.f	0

.Lfunc_end183:
	.size	test_builtin_popcount, .Lfunc_end183-test_builtin_popcount
	.size	.Ltest_builtin_popcount$local, .Lfunc_end183-test_builtin_popcount
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
	cnt.pop	%r1, %r1, 63
	ret.f	0

.Lfunc_end184:
	.size	test_builtin_popcountl, .Lfunc_end184-test_builtin_popcountl
	.size	.Ltest_builtin_popcountl$local, .Lfunc_end184-test_builtin_popcountl
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
	cnt.tz	%r1, %r1, 31
	ret.f	0

.Lfunc_end185:
	.size	test_builtin_ctz, .Lfunc_end185-test_builtin_ctz
	.size	.Ltest_builtin_ctz$local, .Lfunc_end185-test_builtin_ctz
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
	cnt.tz	%r1, %r1, 63
	ret.f	0

.Lfunc_end186:
	.size	test_builtin_ctzl, .Lfunc_end186-test_builtin_ctzl
	.size	.Ltest_builtin_ctzl$local, .Lfunc_end186-test_builtin_ctzl
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
	cnt.lz	%r1, %r1, 31
	ret.f	0

.Lfunc_end187:
	.size	test_builtin_clz, .Lfunc_end187-test_builtin_clz
	.size	.Ltest_builtin_clz$local, .Lfunc_end187-test_builtin_clz
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
	cnt.lz	%r1, %r1, 63
	ret.f	0

.Lfunc_end188:
	.size	test_builtin_clzl, .Lfunc_end188-test_builtin_clzl
	.size	.Ltest_builtin_clzl$local, .Lfunc_end188-test_builtin_clzl
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
	mf.spr	%r1, %itc
	ret.f	0

.Lfunc_end189:
	.size	test_builtin_readcyclecounter, .Lfunc_end189-test_builtin_readcyclecounter
	.size	.Ltest_builtin_readcyclecounter$local, .Lfunc_end189-test_builtin_readcyclecounter
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
	ret.f	0

.Lfunc_end190:
	.size	test_builtin_thread_pointer, .Lfunc_end190-test_builtin_thread_pointer
	.size	.Ltest_builtin_thread_pointer$local, .Lfunc_end190-test_builtin_thread_pointer
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
	ret.f	0

.Lfunc_end191:
	.size	test_builtin_set_thread_pointer, .Lfunc_end191-test_builtin_set_thread_pointer
	.size	.Ltest_builtin_set_thread_pointer$local, .Lfunc_end191-test_builtin_set_thread_pointer
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
	alloc.sp	3, 0
	mov	%r2, %fp
	addi	%fp, %sp, 0

	! BUNDLE
	add	%r1, %r0, %fp
	mov	%sp, %fp
	mov	%fp, %r2

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end192:
	.size	test_frame_buitin, .Lfunc_end192-test_frame_buitin
	.size	.Ltest_frame_buitin$local, .Lfunc_end192-test_frame_buitin
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
	ret.f	0

.Lfunc_end193:
	.size	test_builtin_stack_pointer, .Lfunc_end193-test_builtin_stack_pointer
	.size	.Ltest_builtin_stack_pointer$local, .Lfunc_end193-test_builtin_stack_pointer
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
	alloc.sp	6, 16
	add.ws	%r1, %r2, %r1
	permb	%r3, %r1, 31

	! BUNDLE
	ca.rf	%r5, %pcfwd(.L.str.12)
	st.w	%r3, %sp, 0
	call.r	%r4, %jpc(printf)

	! BUNDLE
	srai.w	%r1, %r2, 0
	addi	%r1, %r1, 11
	permb	%r2, %r1, 63

	! BUNDLE
	ca.rf	%r5, %pcfwd(.L.str.13)
	st.d	%r2, %sp, 0
	call.r	%r4, %jpc(printf)

	! BUNDLE
	ca.xw	%r1, %r2, %r3, 0, 0
	ret.f	16
	nop	0

.Lfunc_end194:
	.size	test_builtin_bitreverse, .Lfunc_end194-test_builtin_bitreverse
	.size	.Ltest_builtin_bitreverse$local, .Lfunc_end194-test_builtin_bitreverse
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
	ret.f	0

.Lfunc_end195:
	.size	test_builtin_halt, .Lfunc_end195-test_builtin_halt
	.size	.Ltest_builtin_halt$local, .Lfunc_end195-test_builtin_halt
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
	ret.f	0

.Lfunc_end196:
	.size	test_builtin_undef, .Lfunc_end196-test_builtin_undef
	.size	.Ltest_builtin_undef$local, .Lfunc_end196-test_builtin_undef
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
	ret.f	0

.Lfunc_end197:
	.size	test_builtin_rfi, .Lfunc_end197-test_builtin_rfi
	.size	.Ltest_builtin_rfi$local, .Lfunc_end197-test_builtin_rfi
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
	ret.f	0

.Lfunc_end198:
	.size	test_builtin_nop, .Lfunc_end198-test_builtin_nop
	.size	.Ltest_builtin_nop$local, .Lfunc_end198-test_builtin_nop
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
	ret.f	0

.Lfunc_end199:
	.size	test_builtin_tpa, .Lfunc_end199-test_builtin_tpa
	.size	.Ltest_builtin_tpa$local, .Lfunc_end199-test_builtin_tpa
                                        ! -- End function
	.globl	test_builtin_random             ! -- Begin function test_builtin_random
	.p2align	4
	.type	test_builtin_random,@function
test_builtin_random:                    ! @test_builtin_random
.Ltest_builtin_random$local:
	.type	.Ltest_builtin_random$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	random	%r1, %r1
	random	%r2, %gz

	! BUNDLE
	add	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end200:
	.size	test_builtin_random, .Lfunc_end200-test_builtin_random
	.size	.Ltest_builtin_random$local, .Lfunc_end200-test_builtin_random
                                        ! -- End function
	.globl	test_builtin_cache              ! -- Begin function test_builtin_cache
	.p2align	4
	.type	test_builtin_cache,@function
test_builtin_cache:                     ! @test_builtin_cache
.Ltest_builtin_cache$local:
	.type	.Ltest_builtin_cache$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	dcb.f	%r1, 0
	dcb.f	%r1, 1000

	! BUNDLE
	dcb.f.l	%r1, 1000000000
	dcb.t	%r1, 0

	! BUNDLE
	dcb.t	%r1, 1000
	dcb.t.l	%r1, 1000000000

	! BUNDLE
	dcb.i	%r1, 0
	dcb.i	%r1, 1000
	nop	2

	! BUNDLE
	dcb.i.l	%r1, 1000000000
	icb.i	%r1, 0

	! BUNDLE
	icb.i	%r1, 1000
	icb.i.l	%r1, 1000000000

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end201:
	.size	test_builtin_cache, .Lfunc_end201-test_builtin_cache
	.size	.Ltest_builtin_cache$local, .Lfunc_end201-test_builtin_cache
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
	mf.spr	%r2, %ip
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %eip
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %fpcr

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %eca
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %rsc
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %rsp

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %bsp
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %reip
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %kip

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %ksp
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %krsp
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %peb

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %teb
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %itc
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %itm

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %pta
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %iva
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %iip

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %iipa
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %cause
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %ifa

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %irr0
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %irr1
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %irr2

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %irr3
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %isr0
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %isr1

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %isr2
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %isr3
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %iv

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %lid
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %tpr
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %itcv

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %tsv
	call.r	%r1, %jpc(consume_sr64)

	! BUNDLE
	mf.spr	%r2, %pmv
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %cmcv

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %iib
	call.r	%r1, %jpc(consume_sr128)

	! BUNDLE
	mf.spr	%r2, %psr
	call.r	%r1, %jpc(consume_sr64)
	mf.spr	%r2, %ipsr

	! BUNDLE
	call.r	%r1, %jpc(consume_sr64)
	ret.f	0
	nop	0

.Lfunc_end202:
	.size	test_builtin_get_special_reg, .Lfunc_end202-test_builtin_get_special_reg
	.size	.Ltest_builtin_get_special_reg$local, .Lfunc_end202-test_builtin_get_special_reg
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
	mt.spr	%r1, %ip
	mt.spr	%r1, %eip

	! BUNDLE
	mt.spr	%r1, %fpcr
	mt.spr	%r1, %eca
	mt.spr	%r1, %rsc

	! BUNDLE
	mt.spr	%r1, %rsp
	mt.spr	%r1, %bsp
	mt.spr	%r1, %reip

	! BUNDLE
	mt.spr	%r1, %kip
	mt.spr	%r1, %ksp
	mt.spr	%r1, %krsp

	! BUNDLE
	mt.spr	%r1, %peb
	mt.spr	%r1, %teb
	mt.spr	%r1, %itc

	! BUNDLE
	mt.spr	%r1, %itm
	mt.spr	%r1, %pta
	mt.spr	%r1, %iva

	! BUNDLE
	mt.spr	%r1, %iip
	mt.spr	%r1, %iipa
	mt.spr	%r1, %cause

	! BUNDLE
	mt.spr	%r1, %ifa
	mt.spr	%r1, %irr0
	mt.spr	%r1, %irr1

	! BUNDLE
	mt.spr	%r1, %irr2
	mt.spr	%r1, %irr3
	mt.spr	%r1, %isr0

	! BUNDLE
	mt.spr	%r1, %isr1
	mt.spr	%r1, %isr2
	mt.spr	%r1, %isr3

	! BUNDLE
	mt.spr	%r1, %iv
	mt.spr	%r1, %lid
	mt.spr	%r1, %tpr

	! BUNDLE
	mt.spr	%r1, %itcv
	mt.spr	%r1, %tsv
	mt.spr	%r1, %pmv

	! BUNDLE
	mt.spr	%r1, %cmcv
	mt.spr	%r1, %psr
	mt.spr	%r2, %iib

	! BUNDLE
	mt.spr	%r1, %ipsr
	ret.f	0
	nop	0

.Lfunc_end203:
	.size	test_builtin_set_special_reg, .Lfunc_end203-test_builtin_set_special_reg
	.size	.Ltest_builtin_set_special_reg$local, .Lfunc_end203-test_builtin_set_special_reg
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
	ld.q	%r2, %r1, 16
	st.q	%r2, %r1, 0

	! BUNDLE
	ld.q	%r1, %r1, 48
	ret.f	0
	nop	0

.Lfunc_end204:
	.size	test_i128_load_store, .Lfunc_end204-test_i128_load_store
	.size	.Ltest_i128_load_store$local, .Lfunc_end204-test_i128_load_store
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
	alloc.sp	7, 16
	addi	%r2, %sp, 16
	ori	%r2, %r2, 4

	! BUNDLE
	st.d	%r2, %sp, 8
	ldz.w	%r2, %sp, 16
	mov2	%r5, %r6, %r1, %r2

	! BUNDLE
	call.r	%r4, %jpc(test_builtin_va_consume_i32)
	ldz.d	%r1, %sp, 8
	algnup	%r1, %r1, 0, 3

	! BUNDLE
	addi	%r3, %r1, 8
	st.d	%r3, %sp, 8
	ldz.d	%r6, %r1, 0

	! BUNDLE
	call.r	%r5, %jpc(test_builtin_va_consume_f64)
	ldz.d	%r1, %sp, 8
	algnup	%r1, %r1, 0, 4

	! BUNDLE
	addi	%r3, %r1, 16
	st.d	%r3, %sp, 8
	ld.q	%r6, %r1, 0

	! BUNDLE
	call.r	%r5, %jpc(test_builtin_va_consume_f128)
	mov	%r1, %r2
	ret.f	16

.Lfunc_end205:
	.size	test_builtin_va, .Lfunc_end205-test_builtin_va
	.size	.Ltest_builtin_va$local, .Lfunc_end205-test_builtin_va
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
	alloc.sp	6, 272
	st.q	%r3, %sp, 256
	st.w	%r1, %sp, 240

	! BUNDLE
	st.d	%r2, %sp, 232
	st.w	%r1, %sp, 224
	st.q	%r3, %sp, 208

	! BUNDLE
	st.d	%r2, %sp, 200
	st.w	%r1, %sp, 192
	st.q	%r3, %sp, 176

	! BUNDLE
	st.d	%r2, %sp, 168
	st.w	%r1, %sp, 160
	st.q	%r3, %sp, 144

	! BUNDLE
	st.d	%r2, %sp, 136
	st.w	%r1, %sp, 128
	st.q	%r3, %sp, 112

	! BUNDLE
	st.d	%r2, %sp, 104
	st.w	%r1, %sp, 96
	st.q	%r3, %sp, 80

	! BUNDLE
	st.d	%r2, %sp, 72
	st.w	%r1, %sp, 64
	st.q	%r3, %sp, 48

	! BUNDLE
	st.d	%r2, %sp, 40
	st.w	%r1, %sp, 32
	st.q	%r3, %sp, 16

	! BUNDLE
	st.d	%r2, %sp, 8
	ca.rf	%r5, %pcfwd(.L.str.14)
	st.w	%r1, %sp, 0

	! BUNDLE
	call.r	%r4, %jpc(test_builtin_va)
	mov	%r1, %r5
	ret.f	272

.Lfunc_end206:
	.size	test_test_builtin_va, .Lfunc_end206-test_test_builtin_va
	.size	.Ltest_test_builtin_va$local, .Lfunc_end206-test_test_builtin_va
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
	vaddc.b	%r2, %r1, %r2
	vaddo.b	%r2, %r2, %r1

	! BUNDLE
	vaddss.b	%r2, %r2, %r1
	vaddus.b	%r2, %r2, %r1
	vaddu.b	%r2, %r2, %r1

	! BUNDLE
	vavgu.b	%r2, %r2, %r1
	vavgs.b	%r2, %r2, %r1
	vcmp.eq.b	%r2, %r2, %r1

	! BUNDLE
	vcmp.lt.b	%r2, %r2, %r1
	vcmp.ltu.b	%r2, %r2, %r1
	vmaxs.b	%r2, %r2, %r1

	! BUNDLE
	vmaxu.b	%r2, %r2, %r1
	vmins.b	%r2, %r2, %r1
	vminu.b	%r2, %r2, %r1

	! BUNDLE
	vrol.b	%r2, %r2, %r1
	vror.b	%r2, %r2, %r1
	vsll.b	%r2, %r2, %r1

	! BUNDLE
	vsra.b	%r2, %r2, %r1
	vsrl.b	%r2, %r2, %r1
	vsubb.b	%r2, %r2, %r1

	! BUNDLE
	vsubo.b	%r2, %r2, %r1
	vsubu.b	%r2, %r2, %r1
	vsubss.b	%r2, %r2, %r1

	! BUNDLE
	vsubus.b	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end207:
	.size	test_builtin_vector_i8, .Lfunc_end207-test_builtin_vector_i8
	.size	.Ltest_builtin_vector_i8$local, .Lfunc_end207-test_builtin_vector_i8
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
	vaddc.h	%r2, %r1, %r2
	vaddo.h	%r2, %r2, %r1

	! BUNDLE
	vaddss.h	%r2, %r2, %r1
	vaddus.h	%r2, %r2, %r1
	vaddu.h	%r2, %r2, %r1

	! BUNDLE
	vavgu.h	%r2, %r2, %r1
	vavgs.h	%r2, %r2, %r1
	vcmp.eq.h	%r2, %r2, %r1

	! BUNDLE
	vcmp.lt.h	%r2, %r2, %r1
	vcmp.ltu.h	%r2, %r2, %r1
	vmaxs.h	%r2, %r2, %r1

	! BUNDLE
	vmaxu.h	%r2, %r2, %r1
	vmins.h	%r2, %r2, %r1
	vminu.h	%r2, %r2, %r1

	! BUNDLE
	vrol.h	%r2, %r2, %r1
	vror.h	%r2, %r2, %r1
	vsll.h	%r2, %r2, %r1

	! BUNDLE
	vsra.h	%r2, %r2, %r1
	vsrl.h	%r2, %r2, %r1
	vsubb.h	%r2, %r2, %r1

	! BUNDLE
	vsubo.h	%r2, %r2, %r1
	vsubu.h	%r2, %r2, %r1
	vsubss.h	%r2, %r2, %r1

	! BUNDLE
	vsubus.h	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end208:
	.size	test_builtin_vector_i16, .Lfunc_end208-test_builtin_vector_i16
	.size	.Ltest_builtin_vector_i16$local, .Lfunc_end208-test_builtin_vector_i16
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
	vaddc.w	%r2, %r1, %r2
	vaddo.w	%r2, %r2, %r1

	! BUNDLE
	vaddss.w	%r2, %r2, %r1
	vaddus.w	%r2, %r2, %r1
	vaddu.w	%r2, %r2, %r1

	! BUNDLE
	vavgu.w	%r2, %r2, %r1
	vavgs.w	%r2, %r2, %r1
	vcmp.eq.w	%r2, %r2, %r1

	! BUNDLE
	vcmp.lt.w	%r2, %r2, %r1
	vcmp.ltu.w	%r2, %r2, %r1
	vmaxs.w	%r2, %r2, %r1

	! BUNDLE
	vmaxu.w	%r2, %r2, %r1
	vmins.w	%r2, %r2, %r1
	vminu.w	%r2, %r2, %r1

	! BUNDLE
	vrol.w	%r2, %r2, %r1
	vror.w	%r2, %r2, %r1
	vsll.w	%r2, %r2, %r1

	! BUNDLE
	vsra.w	%r2, %r2, %r1
	vsrl.w	%r2, %r2, %r1
	vsubb.w	%r2, %r2, %r1

	! BUNDLE
	vsubo.w	%r2, %r2, %r1
	vsubu.w	%r2, %r2, %r1
	vsubss.w	%r2, %r2, %r1

	! BUNDLE
	vsubus.w	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end209:
	.size	test_builtin_vector_i32, .Lfunc_end209-test_builtin_vector_i32
	.size	.Ltest_builtin_vector_i32$local, .Lfunc_end209-test_builtin_vector_i32
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
	vaddc.d	%r2, %r1, %r2
	vaddo.d	%r2, %r2, %r1

	! BUNDLE
	vaddss.d	%r2, %r2, %r1
	vaddus.d	%r2, %r2, %r1
	vaddu.d	%r2, %r2, %r1

	! BUNDLE
	vavgu.d	%r2, %r2, %r1
	vavgs.d	%r2, %r2, %r1
	vcmp.eq.d	%r2, %r2, %r1

	! BUNDLE
	vcmp.lt.d	%r2, %r2, %r1
	vcmp.ltu.d	%r2, %r2, %r1
	vmaxs.d	%r2, %r2, %r1

	! BUNDLE
	vmaxu.d	%r2, %r2, %r1
	vmins.d	%r2, %r2, %r1
	vminu.d	%r2, %r2, %r1

	! BUNDLE
	vrol.d	%r2, %r2, %r1
	vror.d	%r2, %r2, %r1
	vsll.d	%r2, %r2, %r1

	! BUNDLE
	vsra.d	%r2, %r2, %r1
	vsrl.d	%r2, %r2, %r1
	vsubb.d	%r2, %r2, %r1

	! BUNDLE
	vsubo.d	%r2, %r2, %r1
	vsubu.d	%r2, %r2, %r1
	vsubss.d	%r2, %r2, %r1

	! BUNDLE
	vsubus.d	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end210:
	.size	test_builtin_vector_i64, .Lfunc_end210-test_builtin_vector_i64
	.size	.Ltest_builtin_vector_i64$local, .Lfunc_end210-test_builtin_vector_i64
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
	vmrg.h.b	%r1, %r1, %r2
	vmrg.l.b	%r1, %r1, %r2

	! BUNDLE
	vmrg.h.h	%r1, %r1, %r4
	vmrg.l.h	%r1, %r1, %r4
	vmrg.h.w	%r1, %r1, %r6

	! BUNDLE
	vmrg.l.w	%r1, %r1, %r6
	vmrg.h.d	%r1, %r1, %r8
	vmrg.l.d	%r1, %r1, %r8

	! BUNDLE
	vpksss.h	%r1, %r1, %r7
	vpksss.w	%r1, %r1, %r5
	vpksss.d	%r1, %r1, %r7

	! BUNDLE
	vpksus.h	%r1, %r1, %r3
	vpksus.w	%r1, %r1, %r6
	vpksus.d	%r1, %r1, %r8

	! BUNDLE
	vpkuum.h	%r1, %r1, %r4
	vpkuum.w	%r1, %r1, %r6
	vpkuum.d	%r1, %r1, %r8

	! BUNDLE
	vpkuus.h	%r1, %r1, %r4
	vpkuus.w	%r1, %r1, %r6
	vpkuus.d	%r1, %r1, %r8

	! BUNDLE
	vupkhs.b	%r1, %r1
	vupkhs.h	%r1, %r1
	vupkhs.w	%r1, %r1

	! BUNDLE
	vupkls.b	%r1, %r1
	vupkls.h	%r1, %r1
	vupkls.w	%r1, %r1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end211:
	.size	test_builtin_vector_mixed, .Lfunc_end211-test_builtin_vector_mixed
	.size	.Ltest_builtin_vector_mixed$local, .Lfunc_end211-test_builtin_vector_mixed
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
	clmul.ll	%r3, %r1, %r2
	clmul.hl	%r1, %r1, %r3

	! BUNDLE
	clmul.hh	%r1, %r1, %r2
	ret.f	0
	nop	0

.Lfunc_end212:
	.size	test_clmul, .Lfunc_end212-test_clmul
	.size	.Ltest_clmul$local, .Lfunc_end212-test_clmul
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end213:
	.size	test_bool_eq, .Lfunc_end213-test_bool_eq
	.size	.Ltest_bool_eq$local, .Lfunc_end213-test_bool_eq
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
	ret.f	0

.Lfunc_end214:
	.size	test_bool_ne, .Lfunc_end214-test_bool_ne
	.size	.Ltest_bool_ne$local, .Lfunc_end214-test_bool_ne
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end215:
	.size	test_bool_lt, .Lfunc_end215-test_bool_lt
	.size	.Ltest_bool_lt$local, .Lfunc_end215-test_bool_lt
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end216:
	.size	test_bool_le, .Lfunc_end216-test_bool_le
	.size	.Ltest_bool_le$local, .Lfunc_end216-test_bool_le
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end217:
	.size	test_bool_gt, .Lfunc_end217-test_bool_gt
	.size	.Ltest_bool_gt$local, .Lfunc_end217-test_bool_gt
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end218:
	.size	test_bool_ge, .Lfunc_end218-test_bool_ge
	.size	.Ltest_bool_ge$local, .Lfunc_end218-test_bool_ge
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
	cmp.eq.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end219:
	.size	test_ptr_eq, .Lfunc_end219-test_ptr_eq
	.size	.Ltest_ptr_eq$local, .Lfunc_end219-test_ptr_eq
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
	cmp.ne.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end220:
	.size	test_ptr_ne, .Lfunc_end220-test_ptr_ne
	.size	.Ltest_ptr_ne$local, .Lfunc_end220-test_ptr_ne
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
	cmpu.lt.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end221:
	.size	test_ptr_lt, .Lfunc_end221-test_ptr_lt
	.size	.Ltest_ptr_lt$local, .Lfunc_end221-test_ptr_lt
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
	cmpu.le.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end222:
	.size	test_ptr_le, .Lfunc_end222-test_ptr_le
	.size	.Ltest_ptr_le$local, .Lfunc_end222-test_ptr_le
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
	cmpu.lt.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end223:
	.size	test_ptr_gt, .Lfunc_end223-test_ptr_gt
	.size	.Ltest_ptr_gt$local, .Lfunc_end223-test_ptr_gt
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
	cmpu.le.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end224:
	.size	test_ptr_ge, .Lfunc_end224-test_ptr_ge
	.size	.Ltest_ptr_ge$local, .Lfunc_end224-test_ptr_ge
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
	cmp.eq.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end225:
	.size	test_i64_eq, .Lfunc_end225-test_i64_eq
	.size	.Ltest_i64_eq$local, .Lfunc_end225-test_i64_eq
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
	cmp.ne.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end226:
	.size	test_i64_ne, .Lfunc_end226-test_i64_ne
	.size	.Ltest_i64_ne$local, .Lfunc_end226-test_i64_ne
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
	cmps.lt.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end227:
	.size	test_i64_lt, .Lfunc_end227-test_i64_lt
	.size	.Ltest_i64_lt$local, .Lfunc_end227-test_i64_lt
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
	cmps.le.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end228:
	.size	test_i64_le, .Lfunc_end228-test_i64_le
	.size	.Ltest_i64_le$local, .Lfunc_end228-test_i64_le
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
	cmps.lt.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end229:
	.size	test_i64_gt, .Lfunc_end229-test_i64_gt
	.size	.Ltest_i64_gt$local, .Lfunc_end229-test_i64_gt
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
	cmps.le.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end230:
	.size	test_i64_ge, .Lfunc_end230-test_i64_ge
	.size	.Ltest_i64_ge$local, .Lfunc_end230-test_i64_ge
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
	cmpu.lt.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end231:
	.size	test_u64_lt, .Lfunc_end231-test_u64_lt
	.size	.Ltest_u64_lt$local, .Lfunc_end231-test_u64_lt
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
	cmpu.le.d	%r1, %r1, %r2
	ret.f	0

.Lfunc_end232:
	.size	test_u64_le, .Lfunc_end232-test_u64_le
	.size	.Ltest_u64_le$local, .Lfunc_end232-test_u64_le
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
	cmpu.lt.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end233:
	.size	test_u64_gt, .Lfunc_end233-test_u64_gt
	.size	.Ltest_u64_gt$local, .Lfunc_end233-test_u64_gt
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
	cmpu.le.d	%r1, %r2, %r1
	ret.f	0

.Lfunc_end234:
	.size	test_u64_ge, .Lfunc_end234-test_u64_ge
	.size	.Ltest_u64_ge$local, .Lfunc_end234-test_u64_ge
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
	cmpi.eq.d	%r1, %r1, 1234
	ret.f	0

.Lfunc_end235:
	.size	test_i64_eq_imm, .Lfunc_end235-test_i64_eq_imm
	.size	.Ltest_i64_eq_imm$local, .Lfunc_end235-test_i64_eq_imm
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
	cmpi.ne.d	%r1, %r1, 1234
	ret.f	0

.Lfunc_end236:
	.size	test_i64_ne_imm, .Lfunc_end236-test_i64_ne_imm
	.size	.Ltest_i64_ne_imm$local, .Lfunc_end236-test_i64_ne_imm
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
	cmpsi.lt.d	%r1, %r1, 1234
	ret.f	0

.Lfunc_end237:
	.size	test_i64_lt_imm, .Lfunc_end237-test_i64_lt_imm
	.size	.Ltest_i64_lt_imm$local, .Lfunc_end237-test_i64_lt_imm
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
	cmpsi.gt.d	%r1, %r1, 1234
	ret.f	0

.Lfunc_end238:
	.size	test_i64_gt_imm, .Lfunc_end238-test_i64_gt_imm
	.size	.Ltest_i64_gt_imm$local, .Lfunc_end238-test_i64_gt_imm
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
	cmpsi.lt.d	%r1, %r1, 1235
	ret.f	0

.Lfunc_end239:
	.size	test_i64_le_imm, .Lfunc_end239-test_i64_le_imm
	.size	.Ltest_i64_le_imm$local, .Lfunc_end239-test_i64_le_imm
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
	cmpsi.gt.d	%r1, %r1, 1233
	ret.f	0

.Lfunc_end240:
	.size	test_i64_ge_imm, .Lfunc_end240-test_i64_ge_imm
	.size	.Ltest_i64_ge_imm$local, .Lfunc_end240-test_i64_ge_imm
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
	cmpui.lt.d	%r1, %r1, 1234
	ret.f	0

.Lfunc_end241:
	.size	test_u64_lt_imm, .Lfunc_end241-test_u64_lt_imm
	.size	.Ltest_u64_lt_imm$local, .Lfunc_end241-test_u64_lt_imm
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
	cmpui.gt.d	%r1, %r1, 1234
	ret.f	0

.Lfunc_end242:
	.size	test_u64_gt_imm, .Lfunc_end242-test_u64_gt_imm
	.size	.Ltest_u64_gt_imm$local, .Lfunc_end242-test_u64_gt_imm
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
	cmpui.lt.d	%r1, %r1, 1235
	ret.f	0

.Lfunc_end243:
	.size	test_u64_le_imm, .Lfunc_end243-test_u64_le_imm
	.size	.Ltest_u64_le_imm$local, .Lfunc_end243-test_u64_le_imm
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
	cmpui.gt.d	%r1, %r1, 1233
	ret.f	0

.Lfunc_end244:
	.size	test_u64_ge_imm, .Lfunc_end244-test_u64_ge_imm
	.size	.Ltest_u64_ge_imm$local, .Lfunc_end244-test_u64_ge_imm
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
	cmp.eq.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end245:
	.size	test_i32_eq, .Lfunc_end245-test_i32_eq
	.size	.Ltest_i32_eq$local, .Lfunc_end245-test_i32_eq
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
	cmp.ne.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end246:
	.size	test_i32_ne, .Lfunc_end246-test_i32_ne
	.size	.Ltest_i32_ne$local, .Lfunc_end246-test_i32_ne
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
	cmps.lt.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end247:
	.size	test_i32_lt, .Lfunc_end247-test_i32_lt
	.size	.Ltest_i32_lt$local, .Lfunc_end247-test_i32_lt
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
	cmps.le.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end248:
	.size	test_i32_le, .Lfunc_end248-test_i32_le
	.size	.Ltest_i32_le$local, .Lfunc_end248-test_i32_le
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
	cmps.lt.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end249:
	.size	test_i32_gt, .Lfunc_end249-test_i32_gt
	.size	.Ltest_i32_gt$local, .Lfunc_end249-test_i32_gt
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
	cmps.le.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end250:
	.size	test_i32_ge, .Lfunc_end250-test_i32_ge
	.size	.Ltest_i32_ge$local, .Lfunc_end250-test_i32_ge
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
	cmpu.lt.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end251:
	.size	test_u32_lt, .Lfunc_end251-test_u32_lt
	.size	.Ltest_u32_lt$local, .Lfunc_end251-test_u32_lt
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
	cmpu.le.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end252:
	.size	test_u32_le, .Lfunc_end252-test_u32_le
	.size	.Ltest_u32_le$local, .Lfunc_end252-test_u32_le
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
	cmpu.lt.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end253:
	.size	test_u32_gt, .Lfunc_end253-test_u32_gt
	.size	.Ltest_u32_gt$local, .Lfunc_end253-test_u32_gt
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
	cmpu.le.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end254:
	.size	test_u32_ge, .Lfunc_end254-test_u32_ge
	.size	.Ltest_u32_ge$local, .Lfunc_end254-test_u32_ge
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
	cmpi.eq.w	%r1, %r1, 1234
	ret.f	0

.Lfunc_end255:
	.size	test_i32_eq_imm, .Lfunc_end255-test_i32_eq_imm
	.size	.Ltest_i32_eq_imm$local, .Lfunc_end255-test_i32_eq_imm
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
	cmpi.ne.w	%r1, %r1, 1234
	ret.f	0

.Lfunc_end256:
	.size	test_i32_ne_imm, .Lfunc_end256-test_i32_ne_imm
	.size	.Ltest_i32_ne_imm$local, .Lfunc_end256-test_i32_ne_imm
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
	cmpsi.lt.w	%r1, %r1, 1234
	ret.f	0

.Lfunc_end257:
	.size	test_i32_lt_imm, .Lfunc_end257-test_i32_lt_imm
	.size	.Ltest_i32_lt_imm$local, .Lfunc_end257-test_i32_lt_imm
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
	cmpsi.gt.w	%r1, %r1, 1234
	ret.f	0

.Lfunc_end258:
	.size	test_i32_gt_imm, .Lfunc_end258-test_i32_gt_imm
	.size	.Ltest_i32_gt_imm$local, .Lfunc_end258-test_i32_gt_imm
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
	cmpsi.lt.w	%r1, %r1, 1235
	ret.f	0

.Lfunc_end259:
	.size	test_i32_le_imm, .Lfunc_end259-test_i32_le_imm
	.size	.Ltest_i32_le_imm$local, .Lfunc_end259-test_i32_le_imm
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
	cmpsi.gt.w	%r1, %r1, 1233
	ret.f	0

.Lfunc_end260:
	.size	test_i32_ge_imm, .Lfunc_end260-test_i32_ge_imm
	.size	.Ltest_i32_ge_imm$local, .Lfunc_end260-test_i32_ge_imm
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
	cmpui.lt.w	%r1, %r1, 1234
	ret.f	0

.Lfunc_end261:
	.size	test_u32_lt_imm, .Lfunc_end261-test_u32_lt_imm
	.size	.Ltest_u32_lt_imm$local, .Lfunc_end261-test_u32_lt_imm
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
	cmpui.gt.w	%r1, %r1, 1234
	ret.f	0

.Lfunc_end262:
	.size	test_u32_gt_imm, .Lfunc_end262-test_u32_gt_imm
	.size	.Ltest_u32_gt_imm$local, .Lfunc_end262-test_u32_gt_imm
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
	cmpui.lt.w	%r1, %r1, 1235
	ret.f	0

.Lfunc_end263:
	.size	test_u32_le_imm, .Lfunc_end263-test_u32_le_imm
	.size	.Ltest_u32_le_imm$local, .Lfunc_end263-test_u32_le_imm
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
	cmpui.gt.w	%r1, %r1, 1233
	ret.f	0

.Lfunc_end264:
	.size	test_u32_ge_imm, .Lfunc_end264-test_u32_ge_imm
	.size	.Ltest_u32_ge_imm$local, .Lfunc_end264-test_u32_ge_imm
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
	add.add	%r1, %r3, %r2, %r1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end265:
	.size	test_cpuid, .Lfunc_end265-test_cpuid
	.size	.Ltest_cpuid$local, .Lfunc_end265-test_cpuid
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
	ret.f	0

.Lfunc_end266:
	.size	test_crc32c, .Lfunc_end266-test_crc32c
	.size	.Ltest_crc32c$local, .Lfunc_end266-test_crc32c
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
	mf.mr	%r2, %r1, 1
	mt.mr	%r2, %r1, 1

	! BUNDLE
	mf.ibr	%r2, %r1, 2
	mt.ibr	%r2, %r1, 1
	mf.dbr	%r2, %r1, 1

	! BUNDLE
	mt.dbr	%r2, %r1, 2
	ret.f	0
	nop	0

.Lfunc_end267:
	.size	TestDebugRegisters, .Lfunc_end267-TestDebugRegisters
	.size	.LTestDebugRegisters$local, .Lfunc_end267-TestDebugRegisters
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
	fmadd.sh	%r3, %r1, %r2, %r3
	fnmadd.sh	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsub.sh	%r2, %r1, %r2, %r3
	fnmsub.sh	%r1, %r1, %r2, %r2
	ret.f	0

.Lfunc_end268:
	.size	test_fma_f16, .Lfunc_end268-test_fma_f16
	.size	.Ltest_fma_f16$local, .Lfunc_end268-test_fma_f16
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
	fmadd.ss	%r3, %r1, %r2, %r3
	fnmadd.ss	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsub.ss	%r2, %r1, %r2, %r3
	fnmsub.ss	%r1, %r1, %r2, %r2
	ret.f	0

.Lfunc_end269:
	.size	test_fma_f32, .Lfunc_end269-test_fma_f32
	.size	.Ltest_fma_f32$local, .Lfunc_end269-test_fma_f32
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
	fmadd.sd	%r3, %r1, %r2, %r3
	fnmadd.sd	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsub.sd	%r2, %r1, %r2, %r3
	fnmsub.sd	%r1, %r1, %r2, %r2
	ret.f	0

.Lfunc_end270:
	.size	test_fma_f64, .Lfunc_end270-test_fma_f64
	.size	.Ltest_fma_f64$local, .Lfunc_end270-test_fma_f64
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
	fmadd.sq	%r3, %r1, %r2, %r3
	fnmadd.sq	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsub.sq	%r2, %r1, %r2, %r3
	fnmsub.sq	%r1, %r1, %r2, %r2
	ret.f	0

.Lfunc_end271:
	.size	test_fma_f128, .Lfunc_end271-test_fma_f128
	.size	.Ltest_fma_f128$local, .Lfunc_end271-test_fma_f128
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
	fmadd.ph	%r3, %r1, %r2, %r3
	fnmadd.ph	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsub.ph	%r2, %r1, %r2, %r3
	fnmsub.ph	%r1, %r1, %r2, %r2
	ret.f	0

.Lfunc_end272:
	.size	test_fma_v8f16, .Lfunc_end272-test_fma_v8f16
	.size	.Ltest_fma_v8f16$local, .Lfunc_end272-test_fma_v8f16
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
	fmadd.ps	%r3, %r1, %r2, %r3
	fnmadd.ps	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsub.ps	%r2, %r1, %r2, %r3
	fnmsub.ps	%r1, %r1, %r2, %r2
	ret.f	0

.Lfunc_end273:
	.size	test_fma_v4f32, .Lfunc_end273-test_fma_v4f32
	.size	.Ltest_fma_v4f32$local, .Lfunc_end273-test_fma_v4f32
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
	fmadd.pd	%r3, %r1, %r2, %r3
	fnmadd.pd	%r3, %r1, %r2, %r3

	! BUNDLE
	fmsub.pd	%r2, %r1, %r2, %r3
	fnmsub.pd	%r1, %r1, %r2, %r2
	ret.f	0

.Lfunc_end274:
	.size	test_fma_v2f64, .Lfunc_end274-test_fma_v2f64
	.size	.Ltest_fma_v2f64$local, .Lfunc_end274-test_fma_v2f64
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
	ldz.d	%r1, %sp, 0
	ldz.w	%r2, %r1, 0

	! BUNDLE
	ldz.w	%r1, %r1, 4
	fadd.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end275:
	.size	ffff, .Lfunc_end275-ffff
	.size	.Lffff$local, .Lfunc_end275-ffff
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
	alloc.sp	11, 96
	add	%r3, %r2, %r1
	st.d	%r3, %sp, 88

	! BUNDLE
	sub	%r3, %r1, %r2
	st.d	%r3, %sp, 80
	subfi	%r3, %r2, 1000

	! BUNDLE
	st.d	%r3, %sp, 72
	subfi.l	%r3, %r2, 1000000000

	! BUNDLE
	st.d	%r3, %sp, 64
	addi	%r3, %r1, -100
	st.d	%r3, %sp, 56

	! BUNDLE
	st.d	%r3, %sp, 48
	ldz.d	%r3, %sp, 88
	ldz.d	%r4, %sp, 80

	! BUNDLE
	ldz.d	%r5, %sp, 72
	ldz.d	%r6, %sp, 64
	ldz.d	%r7, %sp, 56

	! BUNDLE
	ldz.d	%r8, %sp, 48
	add.add	%r4, %r4, %r5, %r7
	add.sub	%r3, %r3, %r6, %r4

	! BUNDLE
	add	%r3, %r3, %r8
	st.d	%r3, %sp, 40
	bs.le.d	%r2, %r1, .LBB276_3

	! BUNDLE
	ldz.d	%r2, %sp, 88
	bs.le.d	%r2, %r1, .LBB276_3
	ldz.d	%r1, %sp, 40

	! BUNDLE
	addi	%r1, %r1, -1
	ca.rf	%r10, %pcfwd(.L.str.15)
	st.d	%r1, %sp, 0

	! BUNDLE
	call.r	%r9, %jpc(printf)
	ldz.d	%r1, %sp, 88
	ldz.d	%r2, %sp, 80

	! BUNDLE
	ldz.d	%r3, %sp, 64
	add.sub	%r1, %r2, %r1, %r3
	st.d	%r1, %sp, 32

	! BUNDLE
	ldz.d	%r1, %sp, 88
	ldz.d	%r2, %sp, 80
	ldz.d	%r3, %sp, 64

	! BUNDLE
	sub.sub	%r1, %r1, %r2, %r3
	st.d	%r1, %sp, 24
	ldz.d	%r1, %sp, 40

	! BUNDLE
	ret.f	96
	nop	0
	nop	0

.LBB276_3:                              ! %if.else
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d	%r1, %sp, 40
	addi	%r1, %r1, 1
	ca.rf	%r10, %pcfwd(.L.str.16)

	! BUNDLE
	st.d	%r1, %sp, 0
	call.r	%r9, %jpc(printf)
	mov	%r10, %gz

	! BUNDLE
	call.r	%r9, %jpc(exit)
	nop	0
	nop	0

.Lfunc_end276:
	.size	i64_test, .Lfunc_end276-i64_test
	.size	.Li64_test$local, .Lfunc_end276-i64_test
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
	alloc.sp	7, 16
	b.eq.w	%r1, %gz, .LBB277_3
	ca.rf	%r3, %pcfwd(.L.str.17)

.LBB277_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%r1, %sp, 0
	mov	%r6, %r3
	call.r	%r5, %jpc(printf)

	! BUNDLE
	ldz.d.r	%r4, %pcfwd(uglbx)
	addi	%r4, %r4, 1
	addi.ws	%r1, %r1, -1

	! BUNDLE
	st.d.r	%r4, %pcfwd(uglbx)
	b.ne.w	%r1, %gz, .LBB277_2
	nop	0

.LBB277_3:                              ! %for.cond1.preheader
                                        ! Label of block must be emitted
	! BUNDLE
	ca.rf	%r1, %pcfwd(.L.str.18)
	nop	0
	nop	0

.LBB277_4:                              ! %for.cond1
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%r2, %sp, 0
	mov	%r6, %r1
	call.r	%r5, %jpc(printf)

	! BUNDLE
	ldz.w.r	%r3, %pcfwd(glbx)
	addi.ws	%r2, %r2, -1
	addi.ws	%r3, %r3, -1

	! BUNDLE
	st.w.r	%r3, %pcfwd(glbx)
	jmp	.LBB277_4
	nop	0

.Lfunc_end277:
	.size	NeverReturn, .Lfunc_end277-NeverReturn
	.size	.LNeverReturn$local, .Lfunc_end277-NeverReturn
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
	lds.w.r	%r2, %pcfwd(glbx)
	nop	2

	! BUNDLE
	ca.xd.l	%r1, %r1, %r2, 0, 1234
	st.w.r	%r1, %pcfwd(glbx)

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end278:
	.size	i64_add, .Lfunc_end278-i64_add
	.size	.Li64_add$local, .Lfunc_end278-i64_add
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
	ldz.w.r	%r3, %pcfwd(glb)
	mov	%r5, %r1

	! BUNDLE
	call.r	%r4, %jpc(cccc)
	ca.r	%r2, %jpc(cccc)
	add.ws	%r1, %r2, %r1

	! BUNDLE
	add.ws	%r1, %r1, %r3
	add.ws	%r1, %r1, %r5
	addi.ws	%r1, %r1, 12345

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end279:
	.size	i32_add, .Lfunc_end279-i32_add
	.size	.Li32_add$local, .Lfunc_end279-i32_add
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
	ldz.w.r	%r2, %pcfwd(uglb)
	add.ws	%r1, %r2, %r1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end280:
	.size	u32_add, .Lfunc_end280-u32_add
	.size	.Lu32_add$local, .Lfunc_end280-u32_add
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
	alloc.sp	5, 16
	bfo.le.sd	%r2, %r1, .LBB281_1
	ca.rf	%r4, %pcfwd(.L.str.19)

	! BUNDLE
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	mov	%r1, %r4

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.LBB281_1:                              ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	ret.f	16
	nop	0

.Lfunc_end281:
	.size	float_test, .Lfunc_end281-float_test
	.size	.Lfloat_test$local, .Lfunc_end281-float_test
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
	ret.f	0

.Lfunc_end282:
	.size	main, .Lfunc_end282-main
	.size	.Lmain$local, .Lfunc_end282-main
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
	fadd.sh	%r3, %r2, %r1
	fsub.sh	%r1, %r1, %r2

	! BUNDLE
	fmul.sh	%r1, %r3, %r1
	fdiv.sh	%r1, %r1, %r2
	ret.f	0

.Lfunc_end283:
	.size	test_fp_arithmetic_f16, .Lfunc_end283-test_fp_arithmetic_f16
	.size	.Ltest_fp_arithmetic_f16$local, .Lfunc_end283-test_fp_arithmetic_f16
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
	fadd.ss	%r3, %r2, %r1
	fsub.ss	%r1, %r1, %r2

	! BUNDLE
	fmul.ss	%r1, %r3, %r1
	fdiv.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end284:
	.size	test_fp_arithmetic_f32, .Lfunc_end284-test_fp_arithmetic_f32
	.size	.Ltest_fp_arithmetic_f32$local, .Lfunc_end284-test_fp_arithmetic_f32
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
	fadd.sd	%r3, %r2, %r1
	fsub.sd	%r1, %r1, %r2

	! BUNDLE
	fmul.sd	%r1, %r3, %r1
	fdiv.sd	%r1, %r1, %r2
	ret.f	0

.Lfunc_end285:
	.size	test_fp_arithmetic_f64, .Lfunc_end285-test_fp_arithmetic_f64
	.size	.Ltest_fp_arithmetic_f64$local, .Lfunc_end285-test_fp_arithmetic_f64
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
	fadd.sq	%r3, %r2, %r1
	fsub.sq	%r1, %r1, %r2

	! BUNDLE
	fmul.sq	%r1, %r3, %r1
	fdiv.sq	%r1, %r1, %r2
	ret.f	0

.Lfunc_end286:
	.size	test_fp_arithmetic_f128, .Lfunc_end286-test_fp_arithmetic_f128
	.size	.Ltest_fp_arithmetic_f128$local, .Lfunc_end286-test_fp_arithmetic_f128
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
	alloc.sp	2, 16
	st.h	%r1, %sp, 14
	ldz.h	%r1, %sp, 14

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end287:
	.size	test_fp_bitcast_i16_to_f16, .Lfunc_end287-test_fp_bitcast_i16_to_f16
	.size	.Ltest_fp_bitcast_i16_to_f16$local, .Lfunc_end287-test_fp_bitcast_i16_to_f16
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
	alloc.sp	2, 16
	st.h	%r1, %sp, 14
	lds.h	%r1, %sp, 14

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end288:
	.size	test_fp_bitcast_f16_to_i16, .Lfunc_end288-test_fp_bitcast_f16_to_i16
	.size	.Ltest_fp_bitcast_f16_to_i16$local, .Lfunc_end288-test_fp_bitcast_f16_to_i16
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
	ret.f	0
	nop	0

.Lfunc_end289:
	.size	test_fp_bitcast_i32_to_f32, .Lfunc_end289-test_fp_bitcast_i32_to_f32
	.size	.Ltest_fp_bitcast_i32_to_f32$local, .Lfunc_end289-test_fp_bitcast_i32_to_f32
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
	ret.f	0
	nop	0

.Lfunc_end290:
	.size	test_fp_bitcast_f32_to_i32, .Lfunc_end290-test_fp_bitcast_f32_to_i32
	.size	.Ltest_fp_bitcast_f32_to_i32$local, .Lfunc_end290-test_fp_bitcast_f32_to_i32
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
	ret.f	0
	nop	0

.Lfunc_end291:
	.size	test_fp_bitcast_i64_to_f64, .Lfunc_end291-test_fp_bitcast_i64_to_f64
	.size	.Ltest_fp_bitcast_i64_to_f64$local, .Lfunc_end291-test_fp_bitcast_i64_to_f64
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
	ret.f	0
	nop	0

.Lfunc_end292:
	.size	test_fp_bitcast_f64_to_i64, .Lfunc_end292-test_fp_bitcast_f64_to_i64
	.size	.Ltest_fp_bitcast_f64_to_i64$local, .Lfunc_end292-test_fp_bitcast_f64_to_i64
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
	ret.f	0
	nop	0

.Lfunc_end293:
	.size	test_fp_bitcast_i128_to_f128, .Lfunc_end293-test_fp_bitcast_i128_to_f128
	.size	.Ltest_fp_bitcast_i128_to_f128$local, .Lfunc_end293-test_fp_bitcast_i128_to_f128
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
	ret.f	0
	nop	0

.Lfunc_end294:
	.size	test_fp_bitcast_f128_to_i128, .Lfunc_end294-test_fp_bitcast_f128_to_i128
	.size	.Ltest_fp_bitcast_f128_to_i128$local, .Lfunc_end294-test_fp_bitcast_f128_to_i128
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
	nulfo.eq.ss	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.99)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end295:
	.size	test_fp_branch_f32_eq, .Lfunc_end295-test_fp_branch_f32_eq
	.size	.Ltest_fp_branch_f32_eq$local, .Lfunc_end295-test_fp_branch_f32_eq
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
	nulfo.ne.ss	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.100)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end296:
	.size	test_fp_branch_f32_ne, .Lfunc_end296-test_fp_branch_f32_ne
	.size	.Ltest_fp_branch_f32_ne$local, .Lfunc_end296-test_fp_branch_f32_ne
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
	nulfo.lt.ss	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.101)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end297:
	.size	test_fp_branch_f32_lt, .Lfunc_end297-test_fp_branch_f32_lt
	.size	.Ltest_fp_branch_f32_lt$local, .Lfunc_end297-test_fp_branch_f32_lt
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
	nulfo.le.ss	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.102)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end298:
	.size	test_fp_branch_f32_le, .Lfunc_end298-test_fp_branch_f32_le
	.size	.Ltest_fp_branch_f32_le$local, .Lfunc_end298-test_fp_branch_f32_le
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
	nulfo.lt.ss	%r2, %r1, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.103)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end299:
	.size	test_fp_branch_f32_gt, .Lfunc_end299-test_fp_branch_f32_gt
	.size	.Ltest_fp_branch_f32_gt$local, .Lfunc_end299-test_fp_branch_f32_gt
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
	nulfo.le.ss	%r2, %r1, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.104)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end300:
	.size	test_fp_branch_f32_ge, .Lfunc_end300-test_fp_branch_f32_ge
	.size	.Ltest_fp_branch_f32_ge$local, .Lfunc_end300-test_fp_branch_f32_ge
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
	nulfo.eq.sd	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.105)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end301:
	.size	test_fp_branch_f64_eq, .Lfunc_end301-test_fp_branch_f64_eq
	.size	.Ltest_fp_branch_f64_eq$local, .Lfunc_end301-test_fp_branch_f64_eq
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
	nulfo.ne.sd	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.106)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end302:
	.size	test_fp_branch_f64_ne, .Lfunc_end302-test_fp_branch_f64_ne
	.size	.Ltest_fp_branch_f64_ne$local, .Lfunc_end302-test_fp_branch_f64_ne
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
	nulfo.lt.sd	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.107)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end303:
	.size	test_fp_branch_f64_lt, .Lfunc_end303-test_fp_branch_f64_lt
	.size	.Ltest_fp_branch_f64_lt$local, .Lfunc_end303-test_fp_branch_f64_lt
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
	nulfo.le.sd	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.108)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end304:
	.size	test_fp_branch_f64_le, .Lfunc_end304-test_fp_branch_f64_le
	.size	.Ltest_fp_branch_f64_le$local, .Lfunc_end304-test_fp_branch_f64_le
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
	nulfo.lt.sd	%r2, %r1, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.109)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end305:
	.size	test_fp_branch_f64_gt, .Lfunc_end305-test_fp_branch_f64_gt
	.size	.Ltest_fp_branch_f64_gt$local, .Lfunc_end305-test_fp_branch_f64_gt
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
	nulfo.le.sd	%r2, %r1, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.110)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end306:
	.size	test_fp_branch_f64_ge, .Lfunc_end306-test_fp_branch_f64_ge
	.size	.Ltest_fp_branch_f64_ge$local, .Lfunc_end306-test_fp_branch_f64_ge
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
	nulfo.eq.sq	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.111)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end307:
	.size	test_fp_branch_f128_eq, .Lfunc_end307-test_fp_branch_f128_eq
	.size	.Ltest_fp_branch_f128_eq$local, .Lfunc_end307-test_fp_branch_f128_eq
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
	nulfo.ne.sq	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.112)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end308:
	.size	test_fp_branch_f128_ne, .Lfunc_end308-test_fp_branch_f128_ne
	.size	.Ltest_fp_branch_f128_ne$local, .Lfunc_end308-test_fp_branch_f128_ne
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
	nulfo.lt.sq	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.113)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end309:
	.size	test_fp_branch_f128_lt, .Lfunc_end309-test_fp_branch_f128_lt
	.size	.Ltest_fp_branch_f128_lt$local, .Lfunc_end309-test_fp_branch_f128_lt
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
	nulfo.le.sq	%r1, %r2, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.114)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end310:
	.size	test_fp_branch_f128_le, .Lfunc_end310-test_fp_branch_f128_le
	.size	.Ltest_fp_branch_f128_le$local, .Lfunc_end310-test_fp_branch_f128_le
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
	nulfo.lt.sq	%r2, %r1, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.115)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end311:
	.size	test_fp_branch_f128_gt, .Lfunc_end311-test_fp_branch_f128_gt
	.size	.Ltest_fp_branch_f128_gt$local, .Lfunc_end311-test_fp_branch_f128_gt
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
	nulfo.le.sq	%r2, %r1, 2, 0
	ca.rf	%r4, %pcfwd(.Lstr.116)

	! BUNDLE
	call.r	%r3, %jpc(puts)
	ret.f	0
	nop	0

.Lfunc_end312:
	.size	test_fp_branch_f128_ge, .Lfunc_end312-test_fp_branch_f128_ge
	.size	.Ltest_fp_branch_f128_ge$local, .Lfunc_end312-test_fp_branch_f128_ge
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
	ret.f	0

.Lfunc_end313:
	.size	test_builtin_f32_isnan, .Lfunc_end313-test_builtin_f32_isnan
	.size	.Ltest_builtin_f32_isnan$local, .Lfunc_end313-test_builtin_f32_isnan
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
	fmerge.ss	%r1, %r2, %r1, %r1
	ret.f	0

.Lfunc_end314:
	.size	test_builtin_f32_copysign, .Lfunc_end314-test_builtin_f32_copysign
	.size	.Ltest_builtin_f32_copysign$local, .Lfunc_end314-test_builtin_f32_copysign
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
	fmerge.sd	%r1, %r2, %r1, %r1
	ret.f	0

.Lfunc_end315:
	.size	test_builtin_f64_copysign, .Lfunc_end315-test_builtin_f64_copysign
	.size	.Ltest_builtin_f64_copysign$local, .Lfunc_end315-test_builtin_f64_copysign
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
	fmerge.sq	%r1, %r2, %r1, %r1
	ret.f	0

.Lfunc_end316:
	.size	test_builtin_f128_copysign, .Lfunc_end316-test_builtin_f128_copysign
	.size	.Ltest_builtin_f128_copysign$local, .Lfunc_end316-test_builtin_f128_copysign
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
	fcmpo.eq.sh	%r1, %r1, %r2
	ret.f	0

.Lfunc_end317:
	.size	test_compare_f16_eq, .Lfunc_end317-test_compare_f16_eq
	.size	.Ltest_compare_f16_eq$local, .Lfunc_end317-test_compare_f16_eq
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
	fcmpo.ne.sh	%r1, %r1, %r2
	ret.f	0

.Lfunc_end318:
	.size	test_compare_f16_ne, .Lfunc_end318-test_compare_f16_ne
	.size	.Ltest_compare_f16_ne$local, .Lfunc_end318-test_compare_f16_ne
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
	fcmpo.lt.sh	%r1, %r1, %r2
	ret.f	0

.Lfunc_end319:
	.size	test_compare_f16_lt, .Lfunc_end319-test_compare_f16_lt
	.size	.Ltest_compare_f16_lt$local, .Lfunc_end319-test_compare_f16_lt
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
	fcmpo.le.sh	%r1, %r1, %r2
	ret.f	0

.Lfunc_end320:
	.size	test_compare_f16_le, .Lfunc_end320-test_compare_f16_le
	.size	.Ltest_compare_f16_le$local, .Lfunc_end320-test_compare_f16_le
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
	fcmpo.lt.sh	%r1, %r2, %r1
	ret.f	0

.Lfunc_end321:
	.size	test_compare_f16_gt, .Lfunc_end321-test_compare_f16_gt
	.size	.Ltest_compare_f16_gt$local, .Lfunc_end321-test_compare_f16_gt
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
	fcmpo.le.sh	%r1, %r2, %r1
	ret.f	0

.Lfunc_end322:
	.size	test_compare_f16_ge, .Lfunc_end322-test_compare_f16_ge
	.size	.Ltest_compare_f16_ge$local, .Lfunc_end322-test_compare_f16_ge
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
	fcmpo.eq.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end323:
	.size	test_compare_f32_eq, .Lfunc_end323-test_compare_f32_eq
	.size	.Ltest_compare_f32_eq$local, .Lfunc_end323-test_compare_f32_eq
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
	fcmpo.ne.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end324:
	.size	test_compare_f32_ne, .Lfunc_end324-test_compare_f32_ne
	.size	.Ltest_compare_f32_ne$local, .Lfunc_end324-test_compare_f32_ne
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
	fcmpo.lt.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end325:
	.size	test_compare_f32_lt, .Lfunc_end325-test_compare_f32_lt
	.size	.Ltest_compare_f32_lt$local, .Lfunc_end325-test_compare_f32_lt
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
	fcmpo.le.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end326:
	.size	test_compare_f32_le, .Lfunc_end326-test_compare_f32_le
	.size	.Ltest_compare_f32_le$local, .Lfunc_end326-test_compare_f32_le
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
	fcmpo.lt.ss	%r1, %r2, %r1
	ret.f	0

.Lfunc_end327:
	.size	test_compare_f32_gt, .Lfunc_end327-test_compare_f32_gt
	.size	.Ltest_compare_f32_gt$local, .Lfunc_end327-test_compare_f32_gt
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
	fcmpo.le.ss	%r1, %r2, %r1
	ret.f	0

.Lfunc_end328:
	.size	test_compare_f32_ge, .Lfunc_end328-test_compare_f32_ge
	.size	.Ltest_compare_f32_ge$local, .Lfunc_end328-test_compare_f32_ge
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
	fcmpo.eq.sd	%r1, %r1, %r2
	ret.f	0

.Lfunc_end329:
	.size	test_compare_f64_eq, .Lfunc_end329-test_compare_f64_eq
	.size	.Ltest_compare_f64_eq$local, .Lfunc_end329-test_compare_f64_eq
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
	fcmpo.ne.sd	%r1, %r1, %r2
	ret.f	0

.Lfunc_end330:
	.size	test_compare_f64_ne, .Lfunc_end330-test_compare_f64_ne
	.size	.Ltest_compare_f64_ne$local, .Lfunc_end330-test_compare_f64_ne
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
	fcmpo.lt.sd	%r1, %r1, %r2
	ret.f	0

.Lfunc_end331:
	.size	test_compare_f64_lt, .Lfunc_end331-test_compare_f64_lt
	.size	.Ltest_compare_f64_lt$local, .Lfunc_end331-test_compare_f64_lt
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
	fcmpo.le.sd	%r1, %r1, %r2
	ret.f	0

.Lfunc_end332:
	.size	test_compare_f64_le, .Lfunc_end332-test_compare_f64_le
	.size	.Ltest_compare_f64_le$local, .Lfunc_end332-test_compare_f64_le
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
	fcmpo.lt.sd	%r1, %r2, %r1
	ret.f	0

.Lfunc_end333:
	.size	test_compare_f64_gt, .Lfunc_end333-test_compare_f64_gt
	.size	.Ltest_compare_f64_gt$local, .Lfunc_end333-test_compare_f64_gt
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
	fcmpo.le.sd	%r1, %r2, %r1
	ret.f	0

.Lfunc_end334:
	.size	test_compare_f64_ge, .Lfunc_end334-test_compare_f64_ge
	.size	.Ltest_compare_f64_ge$local, .Lfunc_end334-test_compare_f64_ge
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
	fcmpo.eq.sq	%r1, %r1, %r2
	ret.f	0

.Lfunc_end335:
	.size	test_compare_f128_eq, .Lfunc_end335-test_compare_f128_eq
	.size	.Ltest_compare_f128_eq$local, .Lfunc_end335-test_compare_f128_eq
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
	fcmpo.ne.sq	%r1, %r1, %r2
	ret.f	0

.Lfunc_end336:
	.size	test_compare_f128_ne, .Lfunc_end336-test_compare_f128_ne
	.size	.Ltest_compare_f128_ne$local, .Lfunc_end336-test_compare_f128_ne
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
	fcmpo.lt.sq	%r1, %r1, %r2
	ret.f	0

.Lfunc_end337:
	.size	test_compare_f128_lt, .Lfunc_end337-test_compare_f128_lt
	.size	.Ltest_compare_f128_lt$local, .Lfunc_end337-test_compare_f128_lt
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
	fcmpo.le.sq	%r1, %r1, %r2
	ret.f	0

.Lfunc_end338:
	.size	test_compare_f128_le, .Lfunc_end338-test_compare_f128_le
	.size	.Ltest_compare_f128_le$local, .Lfunc_end338-test_compare_f128_le
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
	fcmpo.lt.sq	%r1, %r2, %r1
	ret.f	0

.Lfunc_end339:
	.size	test_compare_f128_gt, .Lfunc_end339-test_compare_f128_gt
	.size	.Ltest_compare_f128_gt$local, .Lfunc_end339-test_compare_f128_gt
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
	fcmpo.le.sq	%r1, %r2, %r1
	ret.f	0

.Lfunc_end340:
	.size	test_compare_f128_ge, .Lfunc_end340-test_compare_f128_ge
	.size	.Ltest_compare_f128_ge$local, .Lfunc_end340-test_compare_f128_ge
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
	fext.sh.ss	%r1, %r1
	ret.f	0

.Lfunc_end341:
	.size	test_conv_f16_to_f32, .Lfunc_end341-test_conv_f16_to_f32
	.size	.Ltest_conv_f16_to_f32$local, .Lfunc_end341-test_conv_f16_to_f32
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
	fext.sh.sd	%r1, %r1
	ret.f	0

.Lfunc_end342:
	.size	test_conv_f16_to_f64, .Lfunc_end342-test_conv_f16_to_f64
	.size	.Ltest_conv_f16_to_f64$local, .Lfunc_end342-test_conv_f16_to_f64
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
	fext.sh.sq	%r1, %r1
	ret.f	0

.Lfunc_end343:
	.size	test_conv_f16_to_f128, .Lfunc_end343-test_conv_f16_to_f128
	.size	.Ltest_conv_f16_to_f128$local, .Lfunc_end343-test_conv_f16_to_f128
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
	fext.sd.sq	%r1, %r1
	ret.f	0

.Lfunc_end344:
	.size	test_conv_f64_to_f128, .Lfunc_end344-test_conv_f64_to_f128
	.size	.Ltest_conv_f64_to_f128$local, .Lfunc_end344-test_conv_f64_to_f128
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
	fext.ss.sd	%r1, %r1
	ret.f	0

.Lfunc_end345:
	.size	test_conv_f32_to_f64, .Lfunc_end345-test_conv_f32_to_f64
	.size	.Ltest_conv_f32_to_f64$local, .Lfunc_end345-test_conv_f32_to_f64
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
	fext.ss.sq	%r1, %r1
	ret.f	0

.Lfunc_end346:
	.size	test_conv_f32_to_f128, .Lfunc_end346-test_conv_f32_to_f128
	.size	.Ltest_conv_f32_to_f128$local, .Lfunc_end346-test_conv_f32_to_f128
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
	fcvt.ss.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end347:
	.size	test_conv_f32_to_f16, .Lfunc_end347-test_conv_f32_to_f16
	.size	.Ltest_conv_f32_to_f16$local, .Lfunc_end347-test_conv_f32_to_f16
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
	fcvt.sd.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end348:
	.size	test_conv_f64_to_f16, .Lfunc_end348-test_conv_f64_to_f16
	.size	.Ltest_conv_f64_to_f16$local, .Lfunc_end348-test_conv_f64_to_f16
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
	fcvt.sd.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end349:
	.size	test_conv_f64_to_f32, .Lfunc_end349-test_conv_f64_to_f32
	.size	.Ltest_conv_f64_to_f32$local, .Lfunc_end349-test_conv_f64_to_f32
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
	fcvt.sq.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end350:
	.size	test_conv_f128_to_f16, .Lfunc_end350-test_conv_f128_to_f16
	.size	.Ltest_conv_f128_to_f16$local, .Lfunc_end350-test_conv_f128_to_f16
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
	fcvt.sq.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end351:
	.size	test_conv_f128_to_f32, .Lfunc_end351-test_conv_f128_to_f32
	.size	.Ltest_conv_f128_to_f32$local, .Lfunc_end351-test_conv_f128_to_f32
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
	fcvt.sq.sd	%r1, %r1, 0
	ret.f	0

.Lfunc_end352:
	.size	test_conv_f128_to_f64, .Lfunc_end352-test_conv_f128_to_f64
	.size	.Ltest_conv_f128_to_f64$local, .Lfunc_end352-test_conv_f128_to_f64
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
	fcvt.sh.iw	%r1, %r1, 0
	ret.f	0

.Lfunc_end353:
	.size	test_conv_f16_to_i32, .Lfunc_end353-test_conv_f16_to_i32
	.size	.Ltest_conv_f16_to_i32$local, .Lfunc_end353-test_conv_f16_to_i32
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
	fcvt.sh.uw	%r1, %r1, 0
	ret.f	0

.Lfunc_end354:
	.size	test_conv_f16_to_u32, .Lfunc_end354-test_conv_f16_to_u32
	.size	.Ltest_conv_f16_to_u32$local, .Lfunc_end354-test_conv_f16_to_u32
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
	fcvt.sh.id	%r1, %r1, 0
	ret.f	0

.Lfunc_end355:
	.size	test_conv_f16_to_i64, .Lfunc_end355-test_conv_f16_to_i64
	.size	.Ltest_conv_f16_to_i64$local, .Lfunc_end355-test_conv_f16_to_i64
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
	fcvt.sh.ud	%r1, %r1, 0
	ret.f	0

.Lfunc_end356:
	.size	test_conv_f16_to_u64, .Lfunc_end356-test_conv_f16_to_u64
	.size	.Ltest_conv_f16_to_u64$local, .Lfunc_end356-test_conv_f16_to_u64
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
	fcvt.sh.iq	%r1, %r1, 0
	ret.f	0

.Lfunc_end357:
	.size	test_conv_f16_to_i128, .Lfunc_end357-test_conv_f16_to_i128
	.size	.Ltest_conv_f16_to_i128$local, .Lfunc_end357-test_conv_f16_to_i128
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
	fcvt.sh.uq	%r1, %r1, 0
	ret.f	0

.Lfunc_end358:
	.size	test_conv_f16_to_u128, .Lfunc_end358-test_conv_f16_to_u128
	.size	.Ltest_conv_f16_to_u128$local, .Lfunc_end358-test_conv_f16_to_u128
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
	fcvt.iw.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end359:
	.size	test_conv_i32_to_f16, .Lfunc_end359-test_conv_i32_to_f16
	.size	.Ltest_conv_i32_to_f16$local, .Lfunc_end359-test_conv_i32_to_f16
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
	fcvt.uw.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end360:
	.size	test_conv_u32_to_f16, .Lfunc_end360-test_conv_u32_to_f16
	.size	.Ltest_conv_u32_to_f16$local, .Lfunc_end360-test_conv_u32_to_f16
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
	fcvt.id.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end361:
	.size	test_conv_i64_to_f16, .Lfunc_end361-test_conv_i64_to_f16
	.size	.Ltest_conv_i64_to_f16$local, .Lfunc_end361-test_conv_i64_to_f16
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
	fcvt.ud.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end362:
	.size	test_conv_u64_to_f16, .Lfunc_end362-test_conv_u64_to_f16
	.size	.Ltest_conv_u64_to_f16$local, .Lfunc_end362-test_conv_u64_to_f16
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
	fcvt.iq.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end363:
	.size	test_conv_i128_to_f16, .Lfunc_end363-test_conv_i128_to_f16
	.size	.Ltest_conv_i128_to_f16$local, .Lfunc_end363-test_conv_i128_to_f16
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
	fcvt.uq.sh	%r1, %r1, 0
	ret.f	0

.Lfunc_end364:
	.size	test_conv_u128_to_f16, .Lfunc_end364-test_conv_u128_to_f16
	.size	.Ltest_conv_u128_to_f16$local, .Lfunc_end364-test_conv_u128_to_f16
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
	fcvt.ss.iw	%r1, %r1, 0
	ret.f	0

.Lfunc_end365:
	.size	test_conv_f32_to_i32, .Lfunc_end365-test_conv_f32_to_i32
	.size	.Ltest_conv_f32_to_i32$local, .Lfunc_end365-test_conv_f32_to_i32
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
	fcvt.ss.uw	%r1, %r1, 0
	ret.f	0

.Lfunc_end366:
	.size	test_conv_f32_to_u32, .Lfunc_end366-test_conv_f32_to_u32
	.size	.Ltest_conv_f32_to_u32$local, .Lfunc_end366-test_conv_f32_to_u32
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
	fcvt.ss.id	%r1, %r1, 0
	ret.f	0

.Lfunc_end367:
	.size	test_conv_f32_to_i64, .Lfunc_end367-test_conv_f32_to_i64
	.size	.Ltest_conv_f32_to_i64$local, .Lfunc_end367-test_conv_f32_to_i64
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
	fcvt.ss.ud	%r1, %r1, 0
	ret.f	0

.Lfunc_end368:
	.size	test_conv_f32_to_u64, .Lfunc_end368-test_conv_f32_to_u64
	.size	.Ltest_conv_f32_to_u64$local, .Lfunc_end368-test_conv_f32_to_u64
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
	fcvt.ss.iq	%r1, %r1, 0
	ret.f	0

.Lfunc_end369:
	.size	test_conv_f32_to_i128, .Lfunc_end369-test_conv_f32_to_i128
	.size	.Ltest_conv_f32_to_i128$local, .Lfunc_end369-test_conv_f32_to_i128
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
	fcvt.ss.uq	%r1, %r1, 0
	ret.f	0

.Lfunc_end370:
	.size	test_conv_f32_to_u128, .Lfunc_end370-test_conv_f32_to_u128
	.size	.Ltest_conv_f32_to_u128$local, .Lfunc_end370-test_conv_f32_to_u128
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
	fcvt.iw.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end371:
	.size	test_conv_i32_to_f32, .Lfunc_end371-test_conv_i32_to_f32
	.size	.Ltest_conv_i32_to_f32$local, .Lfunc_end371-test_conv_i32_to_f32
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
	fcvt.uw.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end372:
	.size	test_conv_u32_to_f32, .Lfunc_end372-test_conv_u32_to_f32
	.size	.Ltest_conv_u32_to_f32$local, .Lfunc_end372-test_conv_u32_to_f32
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
	fcvt.id.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end373:
	.size	test_conv_i64_to_f32, .Lfunc_end373-test_conv_i64_to_f32
	.size	.Ltest_conv_i64_to_f32$local, .Lfunc_end373-test_conv_i64_to_f32
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
	fcvt.ud.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end374:
	.size	test_conv_u64_to_f32, .Lfunc_end374-test_conv_u64_to_f32
	.size	.Ltest_conv_u64_to_f32$local, .Lfunc_end374-test_conv_u64_to_f32
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
	fcvt.iq.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end375:
	.size	test_conv_i128_to_f32, .Lfunc_end375-test_conv_i128_to_f32
	.size	.Ltest_conv_i128_to_f32$local, .Lfunc_end375-test_conv_i128_to_f32
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
	fcvt.uq.ss	%r1, %r1, 0
	ret.f	0

.Lfunc_end376:
	.size	test_conv_u128_to_f32, .Lfunc_end376-test_conv_u128_to_f32
	.size	.Ltest_conv_u128_to_f32$local, .Lfunc_end376-test_conv_u128_to_f32
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
	fcvt.sd.iw	%r1, %r1, 0
	ret.f	0

.Lfunc_end377:
	.size	test_conv_f64_to_i32, .Lfunc_end377-test_conv_f64_to_i32
	.size	.Ltest_conv_f64_to_i32$local, .Lfunc_end377-test_conv_f64_to_i32
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
	fcvt.sd.uw	%r1, %r1, 0
	ret.f	0

.Lfunc_end378:
	.size	test_conv_f64_to_u32, .Lfunc_end378-test_conv_f64_to_u32
	.size	.Ltest_conv_f64_to_u32$local, .Lfunc_end378-test_conv_f64_to_u32
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
	fcvt.sd.id	%r1, %r1, 0
	ret.f	0

.Lfunc_end379:
	.size	test_conv_f64_to_i64, .Lfunc_end379-test_conv_f64_to_i64
	.size	.Ltest_conv_f64_to_i64$local, .Lfunc_end379-test_conv_f64_to_i64
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
	fcvt.sd.ud	%r1, %r1, 0
	ret.f	0

.Lfunc_end380:
	.size	test_conv_f64_to_u64, .Lfunc_end380-test_conv_f64_to_u64
	.size	.Ltest_conv_f64_to_u64$local, .Lfunc_end380-test_conv_f64_to_u64
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
	fcvt.sd.iq	%r1, %r1, 0
	ret.f	0

.Lfunc_end381:
	.size	test_conv_f64_to_i128, .Lfunc_end381-test_conv_f64_to_i128
	.size	.Ltest_conv_f64_to_i128$local, .Lfunc_end381-test_conv_f64_to_i128
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
	fcvt.sd.uq	%r1, %r1, 0
	ret.f	0

.Lfunc_end382:
	.size	test_conv_f64_to_u128, .Lfunc_end382-test_conv_f64_to_u128
	.size	.Ltest_conv_f64_to_u128$local, .Lfunc_end382-test_conv_f64_to_u128
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
	fcvt.iw.sd	%r1, %r1, 0
	ret.f	0

.Lfunc_end383:
	.size	test_conv_i32_to_f64, .Lfunc_end383-test_conv_i32_to_f64
	.size	.Ltest_conv_i32_to_f64$local, .Lfunc_end383-test_conv_i32_to_f64
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
	fcvt.uw.sd	%r1, %r1, 0
	ret.f	0

.Lfunc_end384:
	.size	test_conv_u32_to_f64, .Lfunc_end384-test_conv_u32_to_f64
	.size	.Ltest_conv_u32_to_f64$local, .Lfunc_end384-test_conv_u32_to_f64
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
	fcvt.id.sd	%r1, %r1, 0
	ret.f	0

.Lfunc_end385:
	.size	test_conv_i64_to_f64, .Lfunc_end385-test_conv_i64_to_f64
	.size	.Ltest_conv_i64_to_f64$local, .Lfunc_end385-test_conv_i64_to_f64
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
	fcvt.ud.sd	%r1, %r1, 0
	ret.f	0

.Lfunc_end386:
	.size	test_conv_u64_to_f64, .Lfunc_end386-test_conv_u64_to_f64
	.size	.Ltest_conv_u64_to_f64$local, .Lfunc_end386-test_conv_u64_to_f64
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
	fcvt.iq.sd	%r1, %r1, 0
	ret.f	0

.Lfunc_end387:
	.size	test_conv_i128_to_f64, .Lfunc_end387-test_conv_i128_to_f64
	.size	.Ltest_conv_i128_to_f64$local, .Lfunc_end387-test_conv_i128_to_f64
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
	fcvt.uq.sd	%r1, %r1, 0
	ret.f	0

.Lfunc_end388:
	.size	test_conv_u128_to_f64, .Lfunc_end388-test_conv_u128_to_f64
	.size	.Ltest_conv_u128_to_f64$local, .Lfunc_end388-test_conv_u128_to_f64
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
	fcvt.sq.iw	%r1, %r1, 0
	ret.f	0

.Lfunc_end389:
	.size	test_conv_f128_to_i32, .Lfunc_end389-test_conv_f128_to_i32
	.size	.Ltest_conv_f128_to_i32$local, .Lfunc_end389-test_conv_f128_to_i32
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
	fcvt.sq.uw	%r1, %r1, 0
	ret.f	0

.Lfunc_end390:
	.size	test_conv_f128_to_u32, .Lfunc_end390-test_conv_f128_to_u32
	.size	.Ltest_conv_f128_to_u32$local, .Lfunc_end390-test_conv_f128_to_u32
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
	fcvt.sq.id	%r1, %r1, 0
	ret.f	0

.Lfunc_end391:
	.size	test_conv_f128_to_i64, .Lfunc_end391-test_conv_f128_to_i64
	.size	.Ltest_conv_f128_to_i64$local, .Lfunc_end391-test_conv_f128_to_i64
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
	fcvt.sq.ud	%r1, %r1, 0
	ret.f	0

.Lfunc_end392:
	.size	test_conv_f128_to_u64, .Lfunc_end392-test_conv_f128_to_u64
	.size	.Ltest_conv_f128_to_u64$local, .Lfunc_end392-test_conv_f128_to_u64
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
	fcvt.sq.iq	%r1, %r1, 0
	ret.f	0

.Lfunc_end393:
	.size	test_conv_f128_to_i128, .Lfunc_end393-test_conv_f128_to_i128
	.size	.Ltest_conv_f128_to_i128$local, .Lfunc_end393-test_conv_f128_to_i128
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
	fcvt.sq.uq	%r1, %r1, 0
	ret.f	0

.Lfunc_end394:
	.size	test_conv_f128_to_u128, .Lfunc_end394-test_conv_f128_to_u128
	.size	.Ltest_conv_f128_to_u128$local, .Lfunc_end394-test_conv_f128_to_u128
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
	fcvt.iw.sq	%r1, %r1, 0
	ret.f	0

.Lfunc_end395:
	.size	test_conv_i32_to_f128, .Lfunc_end395-test_conv_i32_to_f128
	.size	.Ltest_conv_i32_to_f128$local, .Lfunc_end395-test_conv_i32_to_f128
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
	fcvt.uw.sq	%r1, %r1, 0
	ret.f	0

.Lfunc_end396:
	.size	test_conv_u32_to_f128, .Lfunc_end396-test_conv_u32_to_f128
	.size	.Ltest_conv_u32_to_f128$local, .Lfunc_end396-test_conv_u32_to_f128
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
	fcvt.id.sq	%r1, %r1, 0
	ret.f	0

.Lfunc_end397:
	.size	test_conv_i64_to_f128, .Lfunc_end397-test_conv_i64_to_f128
	.size	.Ltest_conv_i64_to_f128$local, .Lfunc_end397-test_conv_i64_to_f128
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
	fcvt.ud.sq	%r1, %r1, 0
	ret.f	0

.Lfunc_end398:
	.size	test_conv_u64_to_f128, .Lfunc_end398-test_conv_u64_to_f128
	.size	.Ltest_conv_u64_to_f128$local, .Lfunc_end398-test_conv_u64_to_f128
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
	fcvt.iq.sq	%r1, %r1, 0
	ret.f	0

.Lfunc_end399:
	.size	test_conv_i128_to_f128, .Lfunc_end399-test_conv_i128_to_f128
	.size	.Ltest_conv_i128_to_f128$local, .Lfunc_end399-test_conv_i128_to_f128
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
	fcvt.uq.sq	%r1, %r1, 0
	ret.f	0

.Lfunc_end400:
	.size	test_conv_u128_to_f128, .Lfunc_end400-test_conv_u128_to_f128
	.size	.Ltest_conv_u128_to_f128$local, .Lfunc_end400-test_conv_u128_to_f128
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
	ret.f	0

.Lfunc_end401:
	.size	test_fp_immediate_f16, .Lfunc_end401-test_fp_immediate_f16
	.size	.Ltest_fp_immediate_f16$local, .Lfunc_end401-test_fp_immediate_f16
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end402:
	.size	test_fp_immediate_f32, .Lfunc_end402-test_fp_immediate_f32
	.size	.Ltest_fp_immediate_f32$local, .Lfunc_end402-test_fp_immediate_f32
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end403:
	.size	test_fp_immediate_f64, .Lfunc_end403-test_fp_immediate_f64
	.size	.Ltest_fp_immediate_f64$local, .Lfunc_end403-test_fp_immediate_f64
                                        ! -- End function
	.globl	test_fp_immediate_f128          ! -- Begin function test_fp_immediate_f128
	.p2align	4
	.type	test_fp_immediate_f128,@function
test_fp_immediate_f128:                 ! @test_fp_immediate_f128
.Ltest_fp_immediate_f128$local:
	.type	.Ltest_fp_immediate_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, -845303600433666493

	! BUNDLE
	ldih.l	%r1, 4610553164962720619
	ret.f	0

.Lfunc_end404:
	.size	test_fp_immediate_f128, .Lfunc_end404-test_fp_immediate_f128
	.size	.Ltest_fp_immediate_f128$local, .Lfunc_end404-test_fp_immediate_f128
                                        ! -- End function
	.globl	test_fp_immediate_f128_int      ! -- Begin function test_fp_immediate_f128_int
	.p2align	4
	.type	test_fp_immediate_f128_int,@function
test_fp_immediate_f128_int:             ! @test_fp_immediate_f128_int
.Ltest_fp_immediate_f128_int$local:
	.type	.Ltest_fp_immediate_f128_int$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 0
	nop	2

	! BUNDLE
	ldih.l	%r1, 4612635996473786368
	ret.f	0

.Lfunc_end405:
	.size	test_fp_immediate_f128_int, .Lfunc_end405-test_fp_immediate_f128_int
	.size	.Ltest_fp_immediate_f128_int$local, .Lfunc_end405-test_fp_immediate_f128_int
                                        ! -- End function
	.globl	test_fp_immediate_f128_denorm   ! -- Begin function test_fp_immediate_f128_denorm
	.p2align	4
	.type	test_fp_immediate_f128_denorm,@function
test_fp_immediate_f128_denorm:          ! @test_fp_immediate_f128_denorm
.Ltest_fp_immediate_f128_denorm$local:
	.type	.Ltest_fp_immediate_f128_denorm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi	%r1, 0
	nop	2

	! BUNDLE
	ldih.l	%r1, -8961037358560444416
	ret.f	0

.Lfunc_end406:
	.size	test_fp_immediate_f128_denorm, .Lfunc_end406-test_fp_immediate_f128_denorm
	.size	.Ltest_fp_immediate_f128_denorm$local, .Lfunc_end406-test_fp_immediate_f128_denorm
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
	ret.f	0

.Lfunc_end407:
	.size	test_round_f32, .Lfunc_end407-test_round_f32
	.size	.Ltest_round_f32$local, .Lfunc_end407-test_round_f32
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
	ret.f	0

.Lfunc_end408:
	.size	test_floor_f32, .Lfunc_end408-test_floor_f32
	.size	.Ltest_floor_f32$local, .Lfunc_end408-test_floor_f32
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
	ret.f	0

.Lfunc_end409:
	.size	test_ceil_f32, .Lfunc_end409-test_ceil_f32
	.size	.Ltest_ceil_f32$local, .Lfunc_end409-test_ceil_f32
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
	ret.f	0

.Lfunc_end410:
	.size	test_trunc_f32, .Lfunc_end410-test_trunc_f32
	.size	.Ltest_trunc_f32$local, .Lfunc_end410-test_trunc_f32
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
	ret.f	0

.Lfunc_end411:
	.size	test_nearbyint_f32, .Lfunc_end411-test_nearbyint_f32
	.size	.Ltest_nearbyint_f32$local, .Lfunc_end411-test_nearbyint_f32
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
	ret.f	0

.Lfunc_end412:
	.size	test_rint_f32, .Lfunc_end412-test_rint_f32
	.size	.Ltest_rint_f32$local, .Lfunc_end412-test_rint_f32
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
	ret.f	0

.Lfunc_end413:
	.size	test_round_f64, .Lfunc_end413-test_round_f64
	.size	.Ltest_round_f64$local, .Lfunc_end413-test_round_f64
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
	ret.f	0

.Lfunc_end414:
	.size	test_floor_f64, .Lfunc_end414-test_floor_f64
	.size	.Ltest_floor_f64$local, .Lfunc_end414-test_floor_f64
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
	ret.f	0

.Lfunc_end415:
	.size	test_ceil_f64, .Lfunc_end415-test_ceil_f64
	.size	.Ltest_ceil_f64$local, .Lfunc_end415-test_ceil_f64
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
	ret.f	0

.Lfunc_end416:
	.size	test_trunc_f64, .Lfunc_end416-test_trunc_f64
	.size	.Ltest_trunc_f64$local, .Lfunc_end416-test_trunc_f64
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
	ret.f	0

.Lfunc_end417:
	.size	test_nearbyint_f64, .Lfunc_end417-test_nearbyint_f64
	.size	.Ltest_nearbyint_f64$local, .Lfunc_end417-test_nearbyint_f64
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
	ret.f	0

.Lfunc_end418:
	.size	test_rint_f64, .Lfunc_end418-test_rint_f64
	.size	.Ltest_rint_f64$local, .Lfunc_end418-test_rint_f64
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
	ret.f	0

.Lfunc_end419:
	.size	test_round_f128, .Lfunc_end419-test_round_f128
	.size	.Ltest_round_f128$local, .Lfunc_end419-test_round_f128
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
	ret.f	0

.Lfunc_end420:
	.size	test_floor_f128, .Lfunc_end420-test_floor_f128
	.size	.Ltest_floor_f128$local, .Lfunc_end420-test_floor_f128
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
	ret.f	0

.Lfunc_end421:
	.size	test_ceil_f128, .Lfunc_end421-test_ceil_f128
	.size	.Ltest_ceil_f128$local, .Lfunc_end421-test_ceil_f128
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
	ret.f	0

.Lfunc_end422:
	.size	test_trunc_f128, .Lfunc_end422-test_trunc_f128
	.size	.Ltest_trunc_f128$local, .Lfunc_end422-test_trunc_f128
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
	ret.f	0

.Lfunc_end423:
	.size	test_nearbyint_f128, .Lfunc_end423-test_nearbyint_f128
	.size	.Ltest_nearbyint_f128$local, .Lfunc_end423-test_nearbyint_f128
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
	ret.f	0

.Lfunc_end424:
	.size	test_rint_f128, .Lfunc_end424-test_rint_f128
	.size	.Ltest_rint_f128$local, .Lfunc_end424-test_rint_f128
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
	fneg.ss	%r1, %r1
	ret.f	0

.Lfunc_end425:
	.size	test_fneg_f32, .Lfunc_end425-test_fneg_f32
	.size	.Ltest_fneg_f32$local, .Lfunc_end425-test_fneg_f32
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
	fabs.ss	%r1, %r1
	ret.f	0

.Lfunc_end426:
	.size	test_fabs_f32, .Lfunc_end426-test_fabs_f32
	.size	.Ltest_fabs_f32$local, .Lfunc_end426-test_fabs_f32
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
	fnabs.ss	%r1, %r1
	ret.f	0

.Lfunc_end427:
	.size	test_fnabs_f32, .Lfunc_end427-test_fnabs_f32
	.size	.Ltest_fnabs_f32$local, .Lfunc_end427-test_fnabs_f32
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
	fabsd.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end428:
	.size	test_fabsd_f32, .Lfunc_end428-test_fabsd_f32
	.size	.Ltest_fabsd_f32$local, .Lfunc_end428-test_fabsd_f32
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
	fnabsd.ss	%r1, %r1, %r2
	ret.f	0

.Lfunc_end429:
	.size	test_fnabsd_f32, .Lfunc_end429-test_fnabsd_f32
	.size	.Ltest_fnabsd_f32$local, .Lfunc_end429-test_fnabsd_f32
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
	fneg.sd	%r1, %r1
	ret.f	0

.Lfunc_end430:
	.size	test_fneg_f64, .Lfunc_end430-test_fneg_f64
	.size	.Ltest_fneg_f64$local, .Lfunc_end430-test_fneg_f64
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
	fabs.sd	%r1, %r1
	ret.f	0

.Lfunc_end431:
	.size	test_fabs_f64, .Lfunc_end431-test_fabs_f64
	.size	.Ltest_fabs_f64$local, .Lfunc_end431-test_fabs_f64
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
	fnabs.sd	%r1, %r1
	ret.f	0

.Lfunc_end432:
	.size	test_fnabs_f64, .Lfunc_end432-test_fnabs_f64
	.size	.Ltest_fnabs_f64$local, .Lfunc_end432-test_fnabs_f64
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
	fabsd.sd	%r1, %r1, %r2
	ret.f	0

.Lfunc_end433:
	.size	test_fabsd_f64, .Lfunc_end433-test_fabsd_f64
	.size	.Ltest_fabsd_f64$local, .Lfunc_end433-test_fabsd_f64
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
	fnabsd.sd	%r1, %r1, %r2
	ret.f	0

.Lfunc_end434:
	.size	test_fnabsd_f64, .Lfunc_end434-test_fnabsd_f64
	.size	.Ltest_fnabsd_f64$local, .Lfunc_end434-test_fnabsd_f64
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
	fneg.sq	%r1, %r1
	ret.f	0

.Lfunc_end435:
	.size	test_fneg_f128, .Lfunc_end435-test_fneg_f128
	.size	.Ltest_fneg_f128$local, .Lfunc_end435-test_fneg_f128
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
	fabs.sq	%r1, %r1
	ret.f	0

.Lfunc_end436:
	.size	test_fabs_f128, .Lfunc_end436-test_fabs_f128
	.size	.Ltest_fabs_f128$local, .Lfunc_end436-test_fabs_f128
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
	fnabs.sq	%r1, %r1
	ret.f	0

.Lfunc_end437:
	.size	test_fnabs_f128, .Lfunc_end437-test_fnabs_f128
	.size	.Ltest_fnabs_f128$local, .Lfunc_end437-test_fnabs_f128
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
	fabsd.sq	%r1, %r1, %r2
	ret.f	0

.Lfunc_end438:
	.size	test_fabsd_f128, .Lfunc_end438-test_fabsd_f128
	.size	.Ltest_fabsd_f128$local, .Lfunc_end438-test_fabsd_f128
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
	fnabsd.sq	%r1, %r1, %r2
	ret.f	0

.Lfunc_end439:
	.size	test_fnabsd_f128, .Lfunc_end439-test_fnabsd_f128
	.size	.Ltest_fnabsd_f128$local, .Lfunc_end439-test_fnabsd_f128
                                        ! -- End function
	.globl	fp_store_zero_dummy_return      ! -- Begin function fp_store_zero_dummy_return
	.p2align	4
	.type	fp_store_zero_dummy_return,@function
fp_store_zero_dummy_return:             ! @fp_store_zero_dummy_return
.Lfp_store_zero_dummy_return$local:
	.type	.Lfp_store_zero_dummy_return$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	6
	ldz.h.r	%r1, %pcfwd(f16_zero)
	ldi	%r2, 15360

	! BUNDLE
	ldz.w.r	%r3, %pcfwd(f32_zero)
	fadd.sh	%r1, %r1, %r2
	st.h.r	%r1, %pcfwd(f16_zero)

	! BUNDLE
	ldi.l	%r2, 1065353216
	ldz.d.r	%r4, %pcfwd(f64_zero)

	! BUNDLE
	fadd.ss	%r2, %r3, %r2
	ld.q.r	%r3, %pcfwd(f128_zero)
	st.w.r	%r2, %pcfwd(f32_zero)

	! BUNDLE
	ldi.l	%r5, 4607182418800017408
	fadd.sd	%r4, %r4, %r5

	! BUNDLE
	ldi	%r5, 0
	st.d.r	%r4, %pcfwd(f64_zero)
	nop	2

	! BUNDLE
	ldih.l	%r5, 4611404543450677248
	fadd.sq	%r3, %r3, %r5

	! BUNDLE
	fext.sh.ss	%r1, %r1
	fadd.ss	%r1, %r2, %r1
	fext.ss.sd	%r1, %r1

	! BUNDLE
	fadd.sd	%r1, %r4, %r1
	fext.sd.sq	%r1, %r1
	fadd.sq	%r1, %r3, %r1

	! BUNDLE
	fcvt.sq.sd	%r1, %r1, 0
	st.q.r	%r3, %pcfwd(f128_zero)
	ret.f	0

.Lfunc_end440:
	.size	fp_store_zero_dummy_return, .Lfunc_end440-fp_store_zero_dummy_return
	.size	.Lfp_store_zero_dummy_return$local, .Lfunc_end440-fp_store_zero_dummy_return
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
	st.h	%gz, %r1, 8000
	st.h.r	%gz, %pcfwd(f16_zero)

	! BUNDLE
	st.h.xd	%gz, %r1, %r2, 1, 20
	st.h.xw	%gz, %r1, %r3, 1, 20
	ret.f	0

.Lfunc_end441:
	.size	test_store_based_f16_zero, .Lfunc_end441-test_store_based_f16_zero
	.size	.Ltest_store_based_f16_zero$local, .Lfunc_end441-test_store_based_f16_zero
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
	st.w	%gz, %r1, 16000
	st.w.r	%gz, %pcfwd(f32_zero)

	! BUNDLE
	st.w.xd	%gz, %r1, %r2, 2, 40
	st.w.xw	%gz, %r1, %r3, 2, 40
	ret.f	0

.Lfunc_end442:
	.size	test_store_based_f32_zero, .Lfunc_end442-test_store_based_f32_zero
	.size	.Ltest_store_based_f32_zero$local, .Lfunc_end442-test_store_based_f32_zero
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
	st.d	%gz, %r1, 32000
	st.d.r	%gz, %pcfwd(f64_zero)

	! BUNDLE
	st.d.xd	%gz, %r1, %r2, 3, 40
	st.d.xw	%gz, %r1, %r3, 3, 40
	ret.f	0

.Lfunc_end443:
	.size	test_store_based_f64_zero, .Lfunc_end443-test_store_based_f64_zero
	.size	.Ltest_store_based_f64_zero$local, .Lfunc_end443-test_store_based_f64_zero
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
	st.q	%gz, %r1, 64000
	st.q.r	%gz, %pcfwd(f128_zero)

	! BUNDLE
	st.q.xd	%gz, %r1, %r2, 4, 48
	st.q.xw	%gz, %r1, %r3, 4, 48
	ret.f	0

.Lfunc_end444:
	.size	test_store_based_f128_zero, .Lfunc_end444-test_store_based_f128_zero
	.size	.Ltest_store_based_f128_zero$local, .Lfunc_end444-test_store_based_f128_zero
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
	call.ri	%r3, %r1, %gz

	! BUNDLE
	add.ws	%r1, %r4, %r2
	ret.f	0
	nop	0

.Lfunc_end445:
	.size	test_function_pointer, .Lfunc_end445-test_function_pointer
	.size	.Ltest_function_pointer$local, .Lfunc_end445-test_function_pointer
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
	call.r	%r3, %jpc(ccc)

	! BUNDLE
	add.ws	%r1, %r4, %r2
	ret.f	0
	nop	0

.Lfunc_end446:
	.size	test_function_call, .Lfunc_end446-test_function_call
	.size	.Ltest_function_call$local, .Lfunc_end446-test_function_call
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
	call.r	%r3, %jpc(xprintf)

	! BUNDLE
	add.ws	%r1, %r4, %r2
	ret.f	0
	nop	0

.Lfunc_end447:
	.size	test_function, .Lfunc_end447-test_function
	.size	.Ltest_function$local, .Lfunc_end447-test_function
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
	sl.add	%r2, %r2, %r1, 3
	call.mi	%r3, %r2, 0

	! BUNDLE
	add.ws	%r1, %r4, %r1
	ret.f	0
	nop	0

.Lfunc_end448:
	.size	test_callmi, .Lfunc_end448-test_callmi
	.size	.Ltest_callmi$local, .Lfunc_end448-test_callmi
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
	call.mi	%r3, %r1, 48

	! BUNDLE
	addi.ws	%r1, %r4, 2
	ret.f	0
	nop	0

.Lfunc_end449:
	.size	test_callmi_folding, .Lfunc_end449-test_callmi_folding
	.size	.Ltest_callmi_folding$local, .Lfunc_end449-test_callmi_folding
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
	call.plt	%r2, %pcfwd(gftable+40)

	! BUNDLE
	addi.ws	%r1, %r3, 2
	ret.f	0
	nop	0

.Lfunc_end450:
	.size	test_callplt_folding, .Lfunc_end450-test_callplt_folding
	.size	.Ltest_callplt_folding$local, .Lfunc_end450-test_callplt_folding
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
	addi.ws	%r1, %r1, 1
	ret.f	0

.Lfunc_end451:
	.size	test_global_function_pointer_target, .Lfunc_end451-test_global_function_pointer_target
	.size	.Ltest_global_function_pointer_target$local, .Lfunc_end451-test_global_function_pointer_target
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
	ca.r	%r1, %jpc(test_static_function_pointer_target)
	ret.f	0

.Lfunc_end452:
	.size	test_static_function_pointer_materialize, .Lfunc_end452-test_static_function_pointer_materialize
	.size	.Ltest_static_function_pointer_materialize$local, .Lfunc_end452-test_static_function_pointer_materialize
                                        ! -- End function
	.p2align	4                               ! -- Begin function test_static_function_pointer_target
	.type	test_static_function_pointer_target,@function
test_static_function_pointer_target:    ! @test_static_function_pointer_target
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	addi.ws	%r1, %r1, 1
	ret.f	0

.Lfunc_end453:
	.size	test_static_function_pointer_target, .Lfunc_end453-test_static_function_pointer_target
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
	ca.r	%r1, %jpc(test_global_function_pointer_target)
	ret.f	0

.Lfunc_end454:
	.size	test_global_function_pointer_materialize, .Lfunc_end454-test_global_function_pointer_materialize
	.size	.Ltest_global_function_pointer_materialize$local, .Lfunc_end454-test_global_function_pointer_materialize
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
	ca.r	%r1, %jpc(test_extern_function_pointer_target)
	ret.f	0

.Lfunc_end455:
	.size	test_extern_function_pointer_materialize, .Lfunc_end455-test_extern_function_pointer_materialize
	.size	.Ltest_extern_function_pointer_materialize$local, .Lfunc_end455-test_extern_function_pointer_materialize
                                        ! -- End function
	.globl	test_fshr_u32                   ! -- Begin function test_fshr_u32
	.p2align	4
	.type	test_fshr_u32,@function
test_fshr_u32:                          ! @test_fshr_u32
.Ltest_fshr_u32$local:
	.type	.Ltest_fshr_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	srpi.w	%r1, %r2, %r1, 20
	ret.f	0

.Lfunc_end456:
	.size	test_fshr_u32, .Lfunc_end456-test_fshr_u32
	.size	.Ltest_fshr_u32$local, .Lfunc_end456-test_fshr_u32
                                        ! -- End function
	.globl	test_fshr_u64                   ! -- Begin function test_fshr_u64
	.p2align	4
	.type	test_fshr_u64,@function
test_fshr_u64:                          ! @test_fshr_u64
.Ltest_fshr_u64$local:
	.type	.Ltest_fshr_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	srpi	%r1, %r2, %r1, 20
	ret.f	0

.Lfunc_end457:
	.size	test_fshr_u64, .Lfunc_end457-test_fshr_u64
	.size	.Ltest_fshr_u64$local, .Lfunc_end457-test_fshr_u64
                                        ! -- End function
	.globl	test_fshr_u128                  ! -- Begin function test_fshr_u128
	.p2align	4
	.type	test_fshr_u128,@function
test_fshr_u128:                         ! @test_fshr_u128
.Ltest_fshr_u128$local:
	.type	.Ltest_fshr_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	srpi.q	%r1, %r2, %r1, 20
	ret.f	0

.Lfunc_end458:
	.size	test_fshr_u128, .Lfunc_end458-test_fshr_u128
	.size	.Ltest_fshr_u128$local, .Lfunc_end458-test_fshr_u128
                                        ! -- End function
	.globl	test_rotate_u32                 ! -- Begin function test_rotate_u32
	.p2align	4
	.type	test_rotate_u32,@function
test_rotate_u32:                        ! @test_rotate_u32
.Ltest_rotate_u32$local:
	.type	.Ltest_rotate_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	srpi.w	%r1, %r1, %r1, 20
	ret.f	0

.Lfunc_end459:
	.size	test_rotate_u32, .Lfunc_end459-test_rotate_u32
	.size	.Ltest_rotate_u32$local, .Lfunc_end459-test_rotate_u32
                                        ! -- End function
	.globl	test_rotate_u64                 ! -- Begin function test_rotate_u64
	.p2align	4
	.type	test_rotate_u64,@function
test_rotate_u64:                        ! @test_rotate_u64
.Ltest_rotate_u64$local:
	.type	.Ltest_rotate_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	srpi	%r1, %r1, %r1, 20
	ret.f	0

.Lfunc_end460:
	.size	test_rotate_u64, .Lfunc_end460-test_rotate_u64
	.size	.Ltest_rotate_u64$local, .Lfunc_end460-test_rotate_u64
                                        ! -- End function
	.globl	test_rotate_u128                ! -- Begin function test_rotate_u128
	.p2align	4
	.type	test_rotate_u128,@function
test_rotate_u128:                       ! @test_rotate_u128
.Ltest_rotate_u128$local:
	.type	.Ltest_rotate_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	srpi.q	%r1, %r1, %r1, 20
	ret.f	0

.Lfunc_end461:
	.size	test_rotate_u128, .Lfunc_end461-test_rotate_u128
	.size	.Ltest_rotate_u128$local, .Lfunc_end461-test_rotate_u128
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
	ldz.d.r	%r2, %pcfwd(x_bss)
	addi	%r2, %r2, 1

	! BUNDLE
	st.d.r	%r2, %pcfwd(x_bss)
	ldz.d.r	%r2, %pcfwd(x_bss_extern)
	addi	%r2, %r2, 1

	! BUNDLE
	st.d.r	%r2, %pcfwd(x_bss_extern)
	ret.f	0
	nop	0

.Lfunc_end462:
	.size	test_global_data, .Lfunc_end462-test_global_data
	.size	.Ltest_global_data$local, .Lfunc_end462-test_global_data
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
	ca.rf	%r1, %pcfwd(global_i8)
	ret.f	0

.Lfunc_end463:
	.size	test_global_i8_ptr, .Lfunc_end463-test_global_i8_ptr
	.size	.Ltest_global_i8_ptr$local, .Lfunc_end463-test_global_i8_ptr
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
	lds.b.r	%r1, %pcfwd(global_i8)
	ret.f	0

.Lfunc_end464:
	.size	test_global_i8, .Lfunc_end464-test_global_i8
	.size	.Ltest_global_i8$local, .Lfunc_end464-test_global_i8
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
	ldz.b.r	%r1, %pcfwd(global_u8)
	ret.f	0

.Lfunc_end465:
	.size	test_global_u8, .Lfunc_end465-test_global_u8
	.size	.Ltest_global_u8$local, .Lfunc_end465-test_global_u8
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
	lds.h.r	%r1, %pcfwd(global_i16)
	ret.f	0

.Lfunc_end466:
	.size	test_global_i16, .Lfunc_end466-test_global_i16
	.size	.Ltest_global_i16$local, .Lfunc_end466-test_global_i16
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
	ldz.h.r	%r1, %pcfwd(global_u16)
	ret.f	0

.Lfunc_end467:
	.size	test_global_u16, .Lfunc_end467-test_global_u16
	.size	.Ltest_global_u16$local, .Lfunc_end467-test_global_u16
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
	lds.b.r	%r1, %pcfwd(global_i8)
	ret.f	0

.Lfunc_end468:
	.size	test_global_i8_to_i16, .Lfunc_end468-test_global_i8_to_i16
	.size	.Ltest_global_i8_to_i16$local, .Lfunc_end468-test_global_i8_to_i16
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
	ldz.b.r	%r1, %pcfwd(global_u8)
	ret.f	0

.Lfunc_end469:
	.size	test_global_u8_to_u16, .Lfunc_end469-test_global_u8_to_u16
	.size	.Ltest_global_u8_to_u16$local, .Lfunc_end469-test_global_u8_to_u16
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
	ldz.w.r	%r1, %pcfwd(global_i32)
	ret.f	0

.Lfunc_end470:
	.size	test_global_i32, .Lfunc_end470-test_global_i32
	.size	.Ltest_global_i32$local, .Lfunc_end470-test_global_i32
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
	ldz.w.r	%r1, %pcfwd(global_u32)
	ret.f	0

.Lfunc_end471:
	.size	test_global_u32, .Lfunc_end471-test_global_u32
	.size	.Ltest_global_u32$local, .Lfunc_end471-test_global_u32
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
	lds.b.r	%r1, %pcfwd(global_i8)
	ret.f	0

.Lfunc_end472:
	.size	test_global_i8_to_i32, .Lfunc_end472-test_global_i8_to_i32
	.size	.Ltest_global_i8_to_i32$local, .Lfunc_end472-test_global_i8_to_i32
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
	ldz.b.r	%r1, %pcfwd(global_u8)
	ret.f	0

.Lfunc_end473:
	.size	test_global_u8_to_u32, .Lfunc_end473-test_global_u8_to_u32
	.size	.Ltest_global_u8_to_u32$local, .Lfunc_end473-test_global_u8_to_u32
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
	lds.h.r	%r1, %pcfwd(global_i16)
	ret.f	0

.Lfunc_end474:
	.size	test_global_i16_to_i32, .Lfunc_end474-test_global_i16_to_i32
	.size	.Ltest_global_i16_to_i32$local, .Lfunc_end474-test_global_i16_to_i32
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
	ldz.h.r	%r1, %pcfwd(global_u16)
	ret.f	0

.Lfunc_end475:
	.size	test_global_u16_to_u32, .Lfunc_end475-test_global_u16_to_u32
	.size	.Ltest_global_u16_to_u32$local, .Lfunc_end475-test_global_u16_to_u32
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
	lds.b.r	%r1, %pcfwd(global_i8)
	ret.f	0

.Lfunc_end476:
	.size	test_global_i8_to_i64, .Lfunc_end476-test_global_i8_to_i64
	.size	.Ltest_global_i8_to_i64$local, .Lfunc_end476-test_global_i8_to_i64
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
	ldz.b.r	%r1, %pcfwd(global_u8)
	ret.f	0

.Lfunc_end477:
	.size	test_global_u8_to_u64, .Lfunc_end477-test_global_u8_to_u64
	.size	.Ltest_global_u8_to_u64$local, .Lfunc_end477-test_global_u8_to_u64
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
	lds.h.r	%r1, %pcfwd(global_i16)
	ret.f	0

.Lfunc_end478:
	.size	test_global_i16_to_i64, .Lfunc_end478-test_global_i16_to_i64
	.size	.Ltest_global_i16_to_i64$local, .Lfunc_end478-test_global_i16_to_i64
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
	ldz.h.r	%r1, %pcfwd(global_u16)
	ret.f	0

.Lfunc_end479:
	.size	test_global_u16_to_u64, .Lfunc_end479-test_global_u16_to_u64
	.size	.Ltest_global_u16_to_u64$local, .Lfunc_end479-test_global_u16_to_u64
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
	lds.w.r	%r1, %pcfwd(global_i32)
	ret.f	0

.Lfunc_end480:
	.size	test_global_i32_to_i64, .Lfunc_end480-test_global_i32_to_i64
	.size	.Ltest_global_i32_to_i64$local, .Lfunc_end480-test_global_i32_to_i64
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
	ldz.w.r	%r1, %pcfwd(global_u32)
	ret.f	0

.Lfunc_end481:
	.size	test_global_u32_to_u64, .Lfunc_end481-test_global_u32_to_u64
	.size	.Ltest_global_u32_to_u64$local, .Lfunc_end481-test_global_u32_to_u64
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
	ldz.d.r	%r1, %pcfwd(global_i64)
	ret.f	0

.Lfunc_end482:
	.size	test_global_i64, .Lfunc_end482-test_global_i64
	.size	.Ltest_global_i64$local, .Lfunc_end482-test_global_i64
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
	ldz.d.r	%r1, %pcfwd(global_u64)
	ret.f	0

.Lfunc_end483:
	.size	test_global_u64, .Lfunc_end483-test_global_u64
	.size	.Ltest_global_u64$local, .Lfunc_end483-test_global_u64
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
	lds.b.r	%r1, %pcfwd(global_array_i8+5)
	ret.f	0

.Lfunc_end484:
	.size	test_global_array_folding_i8, .Lfunc_end484-test_global_array_folding_i8
	.size	.Ltest_global_array_folding_i8$local, .Lfunc_end484-test_global_array_folding_i8
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
	lds.h.r	%r1, %pcfwd(global_array_i16+10)
	ret.f	0

.Lfunc_end485:
	.size	test_global_array_folding_i16, .Lfunc_end485-test_global_array_folding_i16
	.size	.Ltest_global_array_folding_i16$local, .Lfunc_end485-test_global_array_folding_i16
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
	ldz.w.r	%r1, %pcfwd(global_array_i32+20)
	ret.f	0

.Lfunc_end486:
	.size	test_global_array_folding_i32, .Lfunc_end486-test_global_array_folding_i32
	.size	.Ltest_global_array_folding_i32$local, .Lfunc_end486-test_global_array_folding_i32
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
	ldz.d.r	%r1, %pcfwd(global_array_i64+40)
	ret.f	0

.Lfunc_end487:
	.size	test_global_array_folding_i64, .Lfunc_end487-test_global_array_folding_i64
	.size	.Ltest_global_array_folding_i64$local, .Lfunc_end487-test_global_array_folding_i64
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
	ret.f	0

.Lfunc_end488:
	.size	test_ldi128, .Lfunc_end488-test_ldi128
	.size	.Ltest_ldi128$local, .Lfunc_end488-test_ldi128
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end489:
	.size	test_ldi128_long, .Lfunc_end489-test_ldi128_long
	.size	.Ltest_ldi128_long$local, .Lfunc_end489-test_ldi128_long
                                        ! -- End function
	.globl	test_ldi128_verylong            ! -- Begin function test_ldi128_verylong
	.p2align	4
	.type	test_ldi128_verylong,@function
test_ldi128_verylong:                   ! @test_ldi128_verylong
.Ltest_ldi128_verylong$local:
	.type	.Ltest_ldi128_verylong$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	2
	ldi.l	%r1, -230686721

	! BUNDLE
	ldih.l	%r1, 1048575
	ret.f	0

.Lfunc_end490:
	.size	test_ldi128_verylong, .Lfunc_end490-test_ldi128_verylong
	.size	.Ltest_ldi128_verylong$local, .Lfunc_end490-test_ldi128_verylong
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
	sub.q	%r1, %r2, %r1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end491:
	.size	test_subfi_i128, .Lfunc_end491-test_subfi_i128
	.size	.Ltest_subfi_i128$local, .Lfunc_end491-test_subfi_i128
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
	add.q	%r1, %r1, %r2
	ldi.l	%r2, 9200848539817279407

	! BUNDLE
	or	%r1, %r1, %r2
	ret.f	0
	nop	0

.Lfunc_end492:
	.size	test_addi_i128, .Lfunc_end492-test_addi_i128
	.size	.Ltest_addi_i128$local, .Lfunc_end492-test_addi_i128
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
	alloc	2
	andi.l	%r1, %r1, 123456789123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end493:
	.size	test_andi_i128, .Lfunc_end493-test_andi_i128
	.size	.Ltest_andi_i128$local, .Lfunc_end493-test_andi_i128
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
	alloc	2
	ori.l	%r1, %r1, 987654321123456789

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end494:
	.size	test_ori_i128, .Lfunc_end494-test_ori_i128
	.size	.Ltest_ori_i128$local, .Lfunc_end494-test_ori_i128
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
	add.q	%r1, %r2, %r1
	ret.f	0

.Lfunc_end495:
	.size	test_add_i128, .Lfunc_end495-test_add_i128
	.size	.Ltest_add_i128$local, .Lfunc_end495-test_add_i128
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
	sub.q	%r1, %r1, %r2
	ret.f	0

.Lfunc_end496:
	.size	test_sub_i128, .Lfunc_end496-test_sub_i128
	.size	.Ltest_sub_i128$local, .Lfunc_end496-test_sub_i128
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
	ret.f	0

.Lfunc_end497:
	.size	test_immediate_i32, .Lfunc_end497-test_immediate_i32
	.size	.Ltest_immediate_i32$local, .Lfunc_end497-test_immediate_i32
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
	ret.f	0

.Lfunc_end498:
	.size	test_immediate_u32, .Lfunc_end498-test_immediate_u32
	.size	.Ltest_immediate_u32$local, .Lfunc_end498-test_immediate_u32
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end499:
	.size	test_immediate_i32_long, .Lfunc_end499-test_immediate_i32_long
	.size	.Ltest_immediate_i32_long$local, .Lfunc_end499-test_immediate_i32_long
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end500:
	.size	test_immediate_u32_long, .Lfunc_end500-test_immediate_u32_long
	.size	.Ltest_immediate_u32_long$local, .Lfunc_end500-test_immediate_u32_long
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
	ret.f	0

.Lfunc_end501:
	.size	test_immediate_i64, .Lfunc_end501-test_immediate_i64
	.size	.Ltest_immediate_i64$local, .Lfunc_end501-test_immediate_i64
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
	ret.f	0

.Lfunc_end502:
	.size	test_immediate_u64, .Lfunc_end502-test_immediate_u64
	.size	.Ltest_immediate_u64$local, .Lfunc_end502-test_immediate_u64
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end503:
	.size	test_immediate_i64_long, .Lfunc_end503-test_immediate_i64_long
	.size	.Ltest_immediate_i64_long$local, .Lfunc_end503-test_immediate_i64_long
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end504:
	.size	test_immediate_u64_long, .Lfunc_end504-test_immediate_u64_long
	.size	.Ltest_immediate_u64_long$local, .Lfunc_end504-test_immediate_u64_long
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end505:
	.size	test_immediate_size_max, .Lfunc_end505-test_immediate_size_max
	.size	.Ltest_immediate_size_max$local, .Lfunc_end505-test_immediate_size_max
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
	ldz.w	%r1, %r1, 16000
	ret.f	0

.Lfunc_end506:
	.size	test_load_based_u32, .Lfunc_end506-test_load_based_u32
	.size	.Ltest_load_based_u32$local, .Lfunc_end506-test_load_based_u32
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
	ldz.h	%r1, %r1, 8000
	ret.f	0

.Lfunc_end507:
	.size	test_load_based_u16, .Lfunc_end507-test_load_based_u16
	.size	.Ltest_load_based_u16$local, .Lfunc_end507-test_load_based_u16
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
	ldz.b	%r1, %r1, 4000
	ret.f	0

.Lfunc_end508:
	.size	test_load_based_u8, .Lfunc_end508-test_load_based_u8
	.size	.Ltest_load_based_u8$local, .Lfunc_end508-test_load_based_u8
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
	ldz.w	%r1, %r1, 16000
	ret.f	0

.Lfunc_end509:
	.size	test_load_based_i32, .Lfunc_end509-test_load_based_i32
	.size	.Ltest_load_based_i32$local, .Lfunc_end509-test_load_based_i32
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
	lds.h	%r1, %r1, 8000
	ret.f	0

.Lfunc_end510:
	.size	test_load_based_i16, .Lfunc_end510-test_load_based_i16
	.size	.Ltest_load_based_i16$local, .Lfunc_end510-test_load_based_i16
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
	lds.b	%r1, %r1, 4000
	ret.f	0

.Lfunc_end511:
	.size	test_load_based_i8, .Lfunc_end511-test_load_based_i8
	.size	.Ltest_load_based_i8$local, .Lfunc_end511-test_load_based_i8
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
	divu.w	%r2, %r2, %r3
	ldz.d.xuw	%r1, %r1, %r2, 3, 0

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end512:
	.size	test_base_index_u32_u32, .Lfunc_end512-test_base_index_u32_u32
	.size	.Ltest_base_index_u32_u32$local, .Lfunc_end512-test_base_index_u32_u32
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
	ldz.d.l	%r5, %r1, 800000000000

	! BUNDLE
	ldz.d	%r6, %r1, 8000
	ldz.d.xd.l	%r7, %r1, %r2, 3, 800000000

	! BUNDLE
	ldz.d.xd.l	%r1, %r1, %r4, 3, 80
	nop	2

	! BUNDLE
	ldz.d.xd.l	%r2, %r3, %r2, 4, 160
	nop	2

	! BUNDLE
	ldz.d.xd.l	%r3, %r3, %r4, 4, 1608
	add	%r4, %r6, %r5

	! BUNDLE
	add.add	%r1, %r4, %r7, %r1
	add.add	%r1, %r1, %r2, %r3
	ret.f	0

.Lfunc_end513:
	.size	test_base_address, .Lfunc_end513-test_base_address
	.size	.Ltest_base_address$local, .Lfunc_end513-test_base_address
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
	bsi.lt.d	%r3, 1, .LBB514_3
	addi	%r2, %r2, 40

.LBB514_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d	%r4, %r1, 0
	ldz.d.xd	%r4, %r2, %r4, 4, 0
	st.d	%r4, %r1, 0

	! BUNDLE
	addi	%r3, %r3, -1
	addi	%r1, %r1, 8
	b.ne.d	%r3, %gz, .LBB514_2

.LBB514_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end514:
	.size	MemoryIndexed, .Lfunc_end514-MemoryIndexed
	.size	.LMemoryIndexed$local, .Lfunc_end514-MemoryIndexed
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
	ldz.d.xw	%r1, %r1, %r2, 3, 0
	ret.f	0

.Lfunc_end515:
	.size	test_base_index_i32, .Lfunc_end515-test_base_index_i32
	.size	.Ltest_base_index_i32$local, .Lfunc_end515-test_base_index_i32
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
	ldz.d.xw	%r1, %r1, %r2, 3, 40
	ret.f	0

.Lfunc_end516:
	.size	test_base_index_i32_offset, .Lfunc_end516-test_base_index_i32_offset
	.size	.Ltest_base_index_i32_offset$local, .Lfunc_end516-test_base_index_i32_offset
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
	ldz.d.xuw	%r1, %r1, %r2, 3, 0
	ret.f	0

.Lfunc_end517:
	.size	test_base_index_u32, .Lfunc_end517-test_base_index_u32
	.size	.Ltest_base_index_u32$local, .Lfunc_end517-test_base_index_u32
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
	addi.ws	%r2, %r2, 20
	ldz.d.xuw	%r1, %r1, %r2, 3, 0

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end518:
	.size	test_base_index_u32_offset, .Lfunc_end518-test_base_index_u32_offset
	.size	.Ltest_base_index_u32_offset$local, .Lfunc_end518-test_base_index_u32_offset
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
	ldz.d.xd	%r1, %r1, %r2, 3, 40
	ret.f	0

.Lfunc_end519:
	.size	test_base_index_i64, .Lfunc_end519-test_base_index_i64
	.size	.Ltest_base_index_i64$local, .Lfunc_end519-test_base_index_i64
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
	ldz.d.xw.l	%r1, %r1, %r2, 4, 328

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end520:
	.size	test_base_index_i32_struct, .Lfunc_end520-test_base_index_i32_struct
	.size	.Ltest_base_index_i32_struct$local, .Lfunc_end520-test_base_index_i32_struct
                                        ! -- End function
	.globl	test_base_index_u32_struct      ! -- Begin function test_base_index_u32_struct
	.p2align	4
	.type	test_base_index_u32_struct,@function
test_base_index_u32_struct:             ! @test_base_index_u32_struct
.Ltest_base_index_u32_struct$local:
	.type	.Ltest_base_index_u32_struct$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	3
	addi.ws	%r2, %r2, 20
	ldz.d.xuw	%r1, %r1, %r2, 4, 8

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end521:
	.size	test_base_index_u32_struct, .Lfunc_end521-test_base_index_u32_struct
	.size	.Ltest_base_index_u32_struct$local, .Lfunc_end521-test_base_index_u32_struct
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
	bsi.lt.d	%r1, 1, .LBB522_3

	! BUNDLE
	addi	%r5, %r3, 80
	nop	0
	nop	0

.LBB522_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d	%r6, %r5, 0
	add	%r4, %r6, %r4
	addi	%r1, %r1, -1

	! BUNDLE
	addi	%r5, %r5, 8
	b.ne.d	%r1, %gz, .LBB522_2
	nop	0

.LBB522_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d.xd.l	%r1, %r3, %r2, 3, 168
	add	%r1, %r1, %r4

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end522:
	.size	for_loop, .Lfunc_end522-for_loop
	.size	.Lfor_loop$local, .Lfunc_end522-for_loop
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
	bsi.lt.w	%r1, 1, .LBB523_3

	! BUNDLE
	addi	%r5, %r3, 40
	srli.w	%r1, %r1, 0
	nop	0

.LBB523_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.w	%r6, %r5, 0
	add.ws	%r4, %r6, %r4
	addi	%r1, %r1, -1

	! BUNDLE
	addi	%r5, %r5, 4
	b.ne.d	%r1, %gz, .LBB523_2
	nop	0

.LBB523_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.w.xw.l	%r1, %r3, %r2, 2, 84
	add.ws	%r1, %r1, %r4

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end523:
	.size	for_loop_32, .Lfunc_end523-for_loop_32
	.size	.Lfor_loop_32$local, .Lfunc_end523-for_loop_32
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
	ret.f	0

.Lfunc_end524:
	.size	test_i64_min_lt, .Lfunc_end524-test_i64_min_lt
	.size	.Ltest_i64_min_lt$local, .Lfunc_end524-test_i64_min_lt
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
	ret.f	0

.Lfunc_end525:
	.size	test_u64_min_lt, .Lfunc_end525-test_u64_min_lt
	.size	.Ltest_u64_min_lt$local, .Lfunc_end525-test_u64_min_lt
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
	ret.f	0

.Lfunc_end526:
	.size	test_i64_min_le, .Lfunc_end526-test_i64_min_le
	.size	.Ltest_i64_min_le$local, .Lfunc_end526-test_i64_min_le
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
	ret.f	0

.Lfunc_end527:
	.size	test_u64_min_le, .Lfunc_end527-test_u64_min_le
	.size	.Ltest_u64_min_le$local, .Lfunc_end527-test_u64_min_le
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
	ret.f	0

.Lfunc_end528:
	.size	test_i64_min_gt, .Lfunc_end528-test_i64_min_gt
	.size	.Ltest_i64_min_gt$local, .Lfunc_end528-test_i64_min_gt
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
	ret.f	0

.Lfunc_end529:
	.size	test_u64_min_gt, .Lfunc_end529-test_u64_min_gt
	.size	.Ltest_u64_min_gt$local, .Lfunc_end529-test_u64_min_gt
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
	ret.f	0

.Lfunc_end530:
	.size	test_i64_min_ge, .Lfunc_end530-test_i64_min_ge
	.size	.Ltest_i64_min_ge$local, .Lfunc_end530-test_i64_min_ge
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
	ret.f	0

.Lfunc_end531:
	.size	test_u64_min_ge, .Lfunc_end531-test_u64_min_ge
	.size	.Ltest_u64_min_ge$local, .Lfunc_end531-test_u64_min_ge
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
	ret.f	0

.Lfunc_end532:
	.size	test_i64_max_lt, .Lfunc_end532-test_i64_max_lt
	.size	.Ltest_i64_max_lt$local, .Lfunc_end532-test_i64_max_lt
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
	ret.f	0

.Lfunc_end533:
	.size	test_u64_max_lt, .Lfunc_end533-test_u64_max_lt
	.size	.Ltest_u64_max_lt$local, .Lfunc_end533-test_u64_max_lt
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
	ret.f	0

.Lfunc_end534:
	.size	test_i64_max_le, .Lfunc_end534-test_i64_max_le
	.size	.Ltest_i64_max_le$local, .Lfunc_end534-test_i64_max_le
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
	ret.f	0

.Lfunc_end535:
	.size	test_u64_max_le, .Lfunc_end535-test_u64_max_le
	.size	.Ltest_u64_max_le$local, .Lfunc_end535-test_u64_max_le
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
	ret.f	0

.Lfunc_end536:
	.size	test_i64_max_gt, .Lfunc_end536-test_i64_max_gt
	.size	.Ltest_i64_max_gt$local, .Lfunc_end536-test_i64_max_gt
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
	ret.f	0

.Lfunc_end537:
	.size	test_u64_max_gt, .Lfunc_end537-test_u64_max_gt
	.size	.Ltest_u64_max_gt$local, .Lfunc_end537-test_u64_max_gt
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
	ret.f	0

.Lfunc_end538:
	.size	test_i64_max_ge, .Lfunc_end538-test_i64_max_ge
	.size	.Ltest_i64_max_ge$local, .Lfunc_end538-test_i64_max_ge
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
	ret.f	0

.Lfunc_end539:
	.size	test_u64_max_ge, .Lfunc_end539-test_u64_max_ge
	.size	.Ltest_u64_max_ge$local, .Lfunc_end539-test_u64_max_ge
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
	mins.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end540:
	.size	test_i32_min_lt, .Lfunc_end540-test_i32_min_lt
	.size	.Ltest_i32_min_lt$local, .Lfunc_end540-test_i32_min_lt
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
	minu.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end541:
	.size	test_u32_min_lt, .Lfunc_end541-test_u32_min_lt
	.size	.Ltest_u32_min_lt$local, .Lfunc_end541-test_u32_min_lt
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
	mins.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end542:
	.size	test_i32_min_le, .Lfunc_end542-test_i32_min_le
	.size	.Ltest_i32_min_le$local, .Lfunc_end542-test_i32_min_le
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
	minu.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end543:
	.size	test_u32_min_le, .Lfunc_end543-test_u32_min_le
	.size	.Ltest_u32_min_le$local, .Lfunc_end543-test_u32_min_le
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
	mins.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end544:
	.size	test_i32_min_gt, .Lfunc_end544-test_i32_min_gt
	.size	.Ltest_i32_min_gt$local, .Lfunc_end544-test_i32_min_gt
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
	minu.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end545:
	.size	test_u32_min_gt, .Lfunc_end545-test_u32_min_gt
	.size	.Ltest_u32_min_gt$local, .Lfunc_end545-test_u32_min_gt
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
	mins.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end546:
	.size	test_i32_min_ge, .Lfunc_end546-test_i32_min_ge
	.size	.Ltest_i32_min_ge$local, .Lfunc_end546-test_i32_min_ge
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
	minu.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end547:
	.size	test_u32_min_ge, .Lfunc_end547-test_u32_min_ge
	.size	.Ltest_u32_min_ge$local, .Lfunc_end547-test_u32_min_ge
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
	maxs.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end548:
	.size	test_i32_max_lt, .Lfunc_end548-test_i32_max_lt
	.size	.Ltest_i32_max_lt$local, .Lfunc_end548-test_i32_max_lt
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
	maxu.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end549:
	.size	test_u32_max_lt, .Lfunc_end549-test_u32_max_lt
	.size	.Ltest_u32_max_lt$local, .Lfunc_end549-test_u32_max_lt
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
	maxs.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end550:
	.size	test_i32_max_le, .Lfunc_end550-test_i32_max_le
	.size	.Ltest_i32_max_le$local, .Lfunc_end550-test_i32_max_le
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
	maxu.w	%r1, %r1, %r2
	ret.f	0

.Lfunc_end551:
	.size	test_u32_max_le, .Lfunc_end551-test_u32_max_le
	.size	.Ltest_u32_max_le$local, .Lfunc_end551-test_u32_max_le
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
	maxs.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end552:
	.size	test_i32_max_gt, .Lfunc_end552-test_i32_max_gt
	.size	.Ltest_i32_max_gt$local, .Lfunc_end552-test_i32_max_gt
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
	maxu.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end553:
	.size	test_u32_max_gt, .Lfunc_end553-test_u32_max_gt
	.size	.Ltest_u32_max_gt$local, .Lfunc_end553-test_u32_max_gt
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
	maxs.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end554:
	.size	test_i32_max_ge, .Lfunc_end554-test_i32_max_ge
	.size	.Ltest_i32_max_ge$local, .Lfunc_end554-test_i32_max_ge
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
	maxu.w	%r1, %r2, %r1
	ret.f	0

.Lfunc_end555:
	.size	test_u32_max_ge, .Lfunc_end555-test_u32_max_ge
	.size	.Ltest_u32_max_ge$local, .Lfunc_end555-test_u32_max_ge
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
	call.r	%r6, %jpc(test_mov2_callback)
	add.ws	%r5, %r7, %r1
	mov2	%r7, %r8, %r3, %r4

	! BUNDLE
	mov2	%r9, %r10, %r2, %gz
	call.r	%r6, %jpc(test_mov2_callback)
	add.ws	%r2, %r5, %r7

	! BUNDLE
	mov2	%r7, %r8, %r3, %r4
	mov2	%r9, %r10, %gz, %gz
	call.r	%r6, %jpc(test_mov2_callback)

	! BUNDLE
	add.ws	%r2, %r2, %r7
	mov2	%r7, %r8, %r3, %gz
	mov2	%r9, %r10, %gz, %gz

	! BUNDLE
	call.r	%r6, %jpc(test_mov2_callback)
	add.ws	%r2, %r2, %r7
	mov2	%r7, %r8, %gz, %gz

	! BUNDLE
	mov2	%r9, %r10, %gz, %gz
	call.r	%r6, %jpc(test_mov2_callback)
	add.ws	%r1, %r2, %r7

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end556:
	.size	test_mov2, .Lfunc_end556-test_mov2
	.size	.Ltest_mov2$local, .Lfunc_end556-test_mov2
                                        ! -- End function
	.globl	test_naked                      ! -- Begin function test_naked
	.p2align	4
	.type	test_naked,@function
test_naked:                             ! @test_naked
.Ltest_naked$local:
	.type	.Ltest_naked$local,@function
! %bb.0:                                ! %entry
.Lfunc_end557:
	.size	test_naked, .Lfunc_end557-test_naked
	.size	.Ltest_naked$local, .Lfunc_end557-test_naked
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
	ret.f	0
	nop	0

.Lfunc_end558:
	.size	test_after_naked, .Lfunc_end558-test_after_naked
	.size	.Ltest_after_naked$local, .Lfunc_end558-test_after_naked
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
	nul.bci	%r1, 0, 2, 3
	ldz.d.r	%r2, %pcfwd(left_bool)

	! BUNDLE
	add	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1

	! BUNDLE
	st.d.xd	%gz, %r3, %r1, 3, 16
	ldz.d	%r2, %r3, 80
	add	%r1, %r2, %r1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end559:
	.size	test_nullify_bool, .Lfunc_end559-test_nullify_bool
	.size	.Ltest_nullify_bool$local, .Lfunc_end559-test_nullify_bool
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
	nul.bsi	%r1, 0, 2, 3
	ldz.d.r	%r2, %pcfwd(left_bool)

	! BUNDLE
	add	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1

	! BUNDLE
	st.d.xd	%gz, %r3, %r1, 3, 16
	ldz.d	%r2, %r3, 80
	add	%r1, %r2, %r1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end560:
	.size	test_nullify_bool_not, .Lfunc_end560-test_nullify_bool_not
	.size	.Ltest_nullify_bool_not$local, .Lfunc_end560-test_nullify_bool_not
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
	b.bci	%r1, 0, .LBB561_3
	b.bci	%r2, 0, .LBB561_3

	! BUNDLE
	ldz.d.r	%r2, %pcfwd(left_bool)
	add	%r1, %r2, %r1
	jmp	.LBB561_4

.LBB561_3:                              ! %if.else
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d.r	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1
	st.d.xd	%gz, %r3, %r1, 3, 16

.LBB561_4:                              ! %if.end
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d	%r2, %r3, 80
	add	%r1, %r2, %r1
	ret.f	0

.Lfunc_end561:
	.size	test_nullify_bool_and, .Lfunc_end561-test_nullify_bool_and
	.size	.Ltest_nullify_bool_and$local, .Lfunc_end561-test_nullify_bool_and
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
	b.bsi	%r1, 0, .LBB562_2
	b.bsi	%r2, 0, .LBB562_2

	! BUNDLE
	ldz.d.r	%r2, %pcfwd(right_bool)
	add	%r1, %r2, %r1
	st.d.xd	%gz, %r3, %r1, 3, 16

	! BUNDLE
	jmp	.LBB562_4
	nop	0
	nop	0

.LBB562_2:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d.r	%r2, %pcfwd(left_bool)
	add	%r1, %r2, %r1
	nop	0

.LBB562_4:                              ! %if.end
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d	%r2, %r3, 80
	add	%r1, %r2, %r1
	ret.f	0

.Lfunc_end562:
	.size	test_nullify_bool_or, .Lfunc_end562-test_nullify_bool_or
	.size	.Ltest_nullify_bool_or$local, .Lfunc_end562-test_nullify_bool_or
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
	ldz.d	%r4, %r2, 16
	nuls.le.d	%r4, %r1, 3, 4

	! BUNDLE
	ldz.d.xd	%r2, %r2, %r1, 3, 8
	muli	%r4, %r2, 5
	ldi	%r2, 2000

	! BUNDLE
	ldz.d.xd	%r4, %r3, %r1, 3, 16
	muli	%r4, %r4, 3
	st.d.xd	%gz, %r2, %r1, 3, 0

	! BUNDLE
	ldi	%r2, 3000
	ldz.d.xd	%r3, %r3, %r1, 3, 40
	add	%r1, %r1, %r2

	! BUNDLE
	add.add	%r1, %r1, %r4, %r3
	ret.f	0
	nop	0

.Lfunc_end563:
	.size	test_nullification_i64_le, .Lfunc_end563-test_nullification_i64_le
	.size	.Ltest_nullification_i64_le$local, .Lfunc_end563-test_nullification_i64_le
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
	ldz.d	%r4, %r2, 16
	nuls.lt.d	%r4, %r1, 3, 4

	! BUNDLE
	ldz.d.xd	%r2, %r2, %r1, 3, 8
	muli	%r4, %r2, 5
	ldi	%r2, 2000

	! BUNDLE
	ldz.d.xd	%r4, %r3, %r1, 3, 16
	muli	%r4, %r4, 3
	st.d.xd	%gz, %r2, %r1, 3, 0

	! BUNDLE
	ldi	%r2, 3000
	ldz.d.xd	%r3, %r3, %r1, 3, 40
	add	%r1, %r1, %r2

	! BUNDLE
	add.add	%r1, %r1, %r4, %r3
	ret.f	0
	nop	0

.Lfunc_end564:
	.size	test_nullification_i64_lt, .Lfunc_end564-test_nullification_i64_lt
	.size	.Ltest_nullification_i64_lt$local, .Lfunc_end564-test_nullification_i64_lt
                                        ! -- End function
	.globl	test_nulu_le_d                  ! -- Begin function test_nulu_le_d
	.p2align	4
	.type	test_nulu_le_d,@function
test_nulu_le_d:                         ! @test_nulu_le_d
.Ltest_nulu_le_d$local:
	.type	.Ltest_nulu_le_d$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	4
	nulu.le.d	%r2, %r1, 2, 3
	ldz.d.r	%r2, %pcfwd(left)

	! BUNDLE
	add	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right)
	add	%r1, %r2, %r1

	! BUNDLE
	st.d.xd	%gz, %r3, %r1, 3, 16
	ldz.d	%r2, %r3, 80
	add	%r1, %r2, %r1

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end565:
	.size	test_nulu_le_d, .Lfunc_end565-test_nulu_le_d
	.size	.Ltest_nulu_le_d$local, .Lfunc_end565-test_nulu_le_d
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
	nulfo.ne.sq	%r1, %r2, 2, 4
	ld.q.r	%r2, %pcfwd(left_f128)

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ld.q.r	%r2, %pcfwd(right_f128)
	fadd.sq	%r1, %r2, %r1

	! BUNDLE
	fcvt.sq.id	%r2, %r1, 0
	st.q.xd	%gz, %r3, %r2, 4, 32
	ld.q	%r2, %r3, 160

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end566:
	.size	test_nullify_f128_eq, .Lfunc_end566-test_nullify_f128_eq
	.size	.Ltest_nullify_f128_eq$local, .Lfunc_end566-test_nullify_f128_eq
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
	nulfo.eq.sq	%r1, %r2, 2, 4
	ld.q.r	%r2, %pcfwd(left_f128)

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ld.q.r	%r2, %pcfwd(right_f128)
	fadd.sq	%r1, %r2, %r1

	! BUNDLE
	fcvt.sq.id	%r2, %r1, 0
	st.q.xd	%gz, %r3, %r2, 4, 32
	ld.q	%r2, %r3, 160

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end567:
	.size	test_nullify_f128_ne, .Lfunc_end567-test_nullify_f128_ne
	.size	.Ltest_nullify_f128_ne$local, .Lfunc_end567-test_nullify_f128_ne
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
	nulfo.le.sq	%r2, %r1, 2, 4
	ld.q.r	%r2, %pcfwd(left_f128)

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ld.q.r	%r2, %pcfwd(right_f128)
	fadd.sq	%r1, %r2, %r1

	! BUNDLE
	fcvt.sq.id	%r2, %r1, 0
	st.q.xd	%gz, %r3, %r2, 4, 32
	ld.q	%r2, %r3, 160

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end568:
	.size	test_nullify_f128_lt, .Lfunc_end568-test_nullify_f128_lt
	.size	.Ltest_nullify_f128_lt$local, .Lfunc_end568-test_nullify_f128_lt
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
	nulfo.lt.sq	%r2, %r1, 2, 4
	ld.q.r	%r2, %pcfwd(left_f128)

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ld.q.r	%r2, %pcfwd(right_f128)
	fadd.sq	%r1, %r2, %r1

	! BUNDLE
	fcvt.sq.id	%r2, %r1, 0
	st.q.xd	%gz, %r3, %r2, 4, 32
	ld.q	%r2, %r3, 160

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end569:
	.size	test_nullify_f128_le, .Lfunc_end569-test_nullify_f128_le
	.size	.Ltest_nullify_f128_le$local, .Lfunc_end569-test_nullify_f128_le
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
	nulfo.le.sq	%r1, %r2, 2, 4
	ld.q.r	%r2, %pcfwd(left_f128)

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ld.q.r	%r2, %pcfwd(right_f128)
	fadd.sq	%r1, %r2, %r1

	! BUNDLE
	fcvt.sq.id	%r2, %r1, 0
	st.q.xd	%gz, %r3, %r2, 4, 32
	ld.q	%r2, %r3, 160

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end570:
	.size	test_nullify_f128_gt, .Lfunc_end570-test_nullify_f128_gt
	.size	.Ltest_nullify_f128_gt$local, .Lfunc_end570-test_nullify_f128_gt
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
	nulfo.lt.sq	%r1, %r2, 2, 4
	ld.q.r	%r2, %pcfwd(left_f128)

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ld.q.r	%r2, %pcfwd(right_f128)
	fadd.sq	%r1, %r2, %r1

	! BUNDLE
	fcvt.sq.id	%r2, %r1, 0
	st.q.xd	%gz, %r3, %r2, 4, 32
	ld.q	%r2, %r3, 160

	! BUNDLE
	fadd.sq	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end571:
	.size	test_nullify_f128_ge, .Lfunc_end571-test_nullify_f128_ge
	.size	.Ltest_nullify_f128_ge$local, .Lfunc_end571-test_nullify_f128_ge
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
	nulfo.ne.ss	%r1, %r2, 2, 4
	ldz.w.r	%r2, %pcfwd(left_f32)

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ldz.w.r	%r2, %pcfwd(right_f32)
	fadd.ss	%r1, %r2, %r1

	! BUNDLE
	fcvt.ss.id	%r2, %r1, 0
	st.w.xd	%gz, %r3, %r2, 2, 8
	ldz.w	%r2, %r3, 40

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end572:
	.size	test_nullify_f32_eq, .Lfunc_end572-test_nullify_f32_eq
	.size	.Ltest_nullify_f32_eq$local, .Lfunc_end572-test_nullify_f32_eq
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
	nulfo.eq.ss	%r1, %r2, 2, 4
	ldz.w.r	%r2, %pcfwd(left_f32)

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ldz.w.r	%r2, %pcfwd(right_f32)
	fadd.ss	%r1, %r2, %r1

	! BUNDLE
	fcvt.ss.id	%r2, %r1, 0
	st.w.xd	%gz, %r3, %r2, 2, 8
	ldz.w	%r2, %r3, 40

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end573:
	.size	test_nullify_f32_ne, .Lfunc_end573-test_nullify_f32_ne
	.size	.Ltest_nullify_f32_ne$local, .Lfunc_end573-test_nullify_f32_ne
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
	nulfo.le.ss	%r2, %r1, 2, 4
	ldz.w.r	%r2, %pcfwd(left_f32)

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ldz.w.r	%r2, %pcfwd(right_f32)
	fadd.ss	%r1, %r2, %r1

	! BUNDLE
	fcvt.ss.id	%r2, %r1, 0
	st.w.xd	%gz, %r3, %r2, 2, 8
	ldz.w	%r2, %r3, 40

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end574:
	.size	test_nullify_f32_lt, .Lfunc_end574-test_nullify_f32_lt
	.size	.Ltest_nullify_f32_lt$local, .Lfunc_end574-test_nullify_f32_lt
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
	nulfo.lt.ss	%r2, %r1, 2, 4
	ldz.w.r	%r2, %pcfwd(left_f32)

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ldz.w.r	%r2, %pcfwd(right_f32)
	fadd.ss	%r1, %r2, %r1

	! BUNDLE
	fcvt.ss.id	%r2, %r1, 0
	st.w.xd	%gz, %r3, %r2, 2, 8
	ldz.w	%r2, %r3, 40

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end575:
	.size	test_nullify_f32_le, .Lfunc_end575-test_nullify_f32_le
	.size	.Ltest_nullify_f32_le$local, .Lfunc_end575-test_nullify_f32_le
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
	nulfo.le.ss	%r1, %r2, 2, 4
	ldz.w.r	%r2, %pcfwd(left_f32)

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ldz.w.r	%r2, %pcfwd(right_f32)
	fadd.ss	%r1, %r2, %r1

	! BUNDLE
	fcvt.ss.id	%r2, %r1, 0
	st.w.xd	%gz, %r3, %r2, 2, 8
	ldz.w	%r2, %r3, 40

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end576:
	.size	test_nullify_f32_gt, .Lfunc_end576-test_nullify_f32_gt
	.size	.Ltest_nullify_f32_gt$local, .Lfunc_end576-test_nullify_f32_gt
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
	nulfo.lt.ss	%r1, %r2, 2, 4
	ldz.w.r	%r2, %pcfwd(left_f32)

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ldz.w.r	%r2, %pcfwd(right_f32)
	fadd.ss	%r1, %r2, %r1

	! BUNDLE
	fcvt.ss.id	%r2, %r1, 0
	st.w.xd	%gz, %r3, %r2, 2, 8
	ldz.w	%r2, %r3, 40

	! BUNDLE
	fadd.ss	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end577:
	.size	test_nullify_f32_ge, .Lfunc_end577-test_nullify_f32_ge
	.size	.Ltest_nullify_f32_ge$local, .Lfunc_end577-test_nullify_f32_ge
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
	nulfo.ne.sd	%r1, %r2, 2, 4
	ldz.d.r	%r2, %pcfwd(left_f64)

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_f64)
	fadd.sd	%r1, %r2, %r1

	! BUNDLE
	fcvt.sd.id	%r2, %r1, 0
	st.d.xd	%gz, %r3, %r2, 3, 16
	ldz.d	%r2, %r3, 80

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end578:
	.size	test_nullify_f64_eq, .Lfunc_end578-test_nullify_f64_eq
	.size	.Ltest_nullify_f64_eq$local, .Lfunc_end578-test_nullify_f64_eq
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
	nulfo.eq.sd	%r1, %r2, 2, 4
	ldz.d.r	%r2, %pcfwd(left_f64)

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_f64)
	fadd.sd	%r1, %r2, %r1

	! BUNDLE
	fcvt.sd.id	%r2, %r1, 0
	st.d.xd	%gz, %r3, %r2, 3, 16
	ldz.d	%r2, %r3, 80

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end579:
	.size	test_nullify_f64_ne, .Lfunc_end579-test_nullify_f64_ne
	.size	.Ltest_nullify_f64_ne$local, .Lfunc_end579-test_nullify_f64_ne
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
	nulfo.le.sd	%r2, %r1, 2, 4
	ldz.d.r	%r2, %pcfwd(left_f64)

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_f64)
	fadd.sd	%r1, %r2, %r1

	! BUNDLE
	fcvt.sd.id	%r2, %r1, 0
	st.d.xd	%gz, %r3, %r2, 3, 16
	ldz.d	%r2, %r3, 80

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end580:
	.size	test_nullify_f64_lt, .Lfunc_end580-test_nullify_f64_lt
	.size	.Ltest_nullify_f64_lt$local, .Lfunc_end580-test_nullify_f64_lt
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
	nulfo.lt.sd	%r2, %r1, 2, 4
	ldz.d.r	%r2, %pcfwd(left_f64)

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_f64)
	fadd.sd	%r1, %r2, %r1

	! BUNDLE
	fcvt.sd.id	%r2, %r1, 0
	st.d.xd	%gz, %r3, %r2, 3, 16
	ldz.d	%r2, %r3, 80

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end581:
	.size	test_nullify_f64_le, .Lfunc_end581-test_nullify_f64_le
	.size	.Ltest_nullify_f64_le$local, .Lfunc_end581-test_nullify_f64_le
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
	nulfo.le.sd	%r1, %r2, 2, 4
	ldz.d.r	%r2, %pcfwd(left_f64)

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_f64)
	fadd.sd	%r1, %r2, %r1

	! BUNDLE
	fcvt.sd.id	%r2, %r1, 0
	st.d.xd	%gz, %r3, %r2, 3, 16
	ldz.d	%r2, %r3, 80

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end582:
	.size	test_nullify_f64_gt, .Lfunc_end582-test_nullify_f64_gt
	.size	.Ltest_nullify_f64_gt$local, .Lfunc_end582-test_nullify_f64_gt
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
	nulfo.lt.sd	%r1, %r2, 2, 4
	ldz.d.r	%r2, %pcfwd(left_f64)

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ldz.d.r	%r2, %pcfwd(right_f64)
	fadd.sd	%r1, %r2, %r1

	! BUNDLE
	fcvt.sd.id	%r2, %r1, 0
	st.d.xd	%gz, %r3, %r2, 3, 16
	ldz.d	%r2, %r3, 80

	! BUNDLE
	fadd.sd	%r1, %r2, %r1
	ret.f	0
	nop	0

.Lfunc_end583:
	.size	test_nullify_f64_ge, .Lfunc_end583-test_nullify_f64_ge
	.size	.Ltest_nullify_f64_ge$local, .Lfunc_end583-test_nullify_f64_ge
                                        ! -- End function
	.globl	test_post_update                ! -- Begin function test_post_update
	.p2align	4
	.type	test_post_update,@function
test_post_update:                       ! @test_post_update
.Ltest_post_update$local:
	.type	.Ltest_post_update$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	7
	mov	%r4, %gz
	bsi.lt.d	%r1, 1, .LBB584_2

.LBB584_1:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ldz.d	%r5, %r2, 0
	ldz.d	%r6, %r3, 0
	mul.add	%r4, %r6, %r5, %r4

	! BUNDLE
	addi	%r1, %r1, -1
	addi	%r3, %r3, 8
	addi	%r2, %r2, 8

	! BUNDLE
	b.ne.d	%r1, %gz, .LBB584_1
	nop	0
	nop	0

.LBB584_2:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %r4
	ret.f	0
	nop	0

.Lfunc_end584:
	.size	test_post_update, .Lfunc_end584-test_post_update
	.size	.Ltest_post_update$local, .Lfunc_end584-test_post_update
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
	srai.w	%r1, %r1, 0
	ret.f	0

.Lfunc_end585:
	.size	test_sign_extend_32, .Lfunc_end585-test_sign_extend_32
	.size	.Ltest_sign_extend_32$local, .Lfunc_end585-test_sign_extend_32
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
	ret.f	0

.Lfunc_end586:
	.size	test_sign_extend_16, .Lfunc_end586-test_sign_extend_16
	.size	.Ltest_sign_extend_16$local, .Lfunc_end586-test_sign_extend_16
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
	ret.f	0

.Lfunc_end587:
	.size	test_sign_extend_8, .Lfunc_end587-test_sign_extend_8
	.size	.Ltest_sign_extend_8$local, .Lfunc_end587-test_sign_extend_8
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
	srli.w	%r1, %r1, 0
	ret.f	0

.Lfunc_end588:
	.size	test_zero_extend_32, .Lfunc_end588-test_zero_extend_32
	.size	.Ltest_zero_extend_32$local, .Lfunc_end588-test_zero_extend_32
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
	ret.f	0

.Lfunc_end589:
	.size	test_zero_extend_16, .Lfunc_end589-test_zero_extend_16
	.size	.Ltest_zero_extend_16$local, .Lfunc_end589-test_zero_extend_16
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
	ret.f	0

.Lfunc_end590:
	.size	test_zero_extend_8, .Lfunc_end590-test_zero_extend_8
	.size	.Ltest_zero_extend_8$local, .Lfunc_end590-test_zero_extend_8
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
	st.w	%r2, %r1, 16000
	ret.f	0

.Lfunc_end591:
	.size	test_store_based_u32, .Lfunc_end591-test_store_based_u32
	.size	.Ltest_store_based_u32$local, .Lfunc_end591-test_store_based_u32
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
	st.h	%r2, %r1, 8000
	ret.f	0

.Lfunc_end592:
	.size	test_store_based_u16, .Lfunc_end592-test_store_based_u16
	.size	.Ltest_store_based_u16$local, .Lfunc_end592-test_store_based_u16
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
	st.b	%r2, %r1, 4000
	ret.f	0

.Lfunc_end593:
	.size	test_store_based_u8, .Lfunc_end593-test_store_based_u8
	.size	.Ltest_store_based_u8$local, .Lfunc_end593-test_store_based_u8
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
	st.w	%r2, %r1, 16000
	ret.f	0

.Lfunc_end594:
	.size	test_store_based_i32, .Lfunc_end594-test_store_based_i32
	.size	.Ltest_store_based_i32$local, .Lfunc_end594-test_store_based_i32
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
	st.h	%r2, %r1, 8000
	ret.f	0

.Lfunc_end595:
	.size	test_store_based_i16, .Lfunc_end595-test_store_based_i16
	.size	.Ltest_store_based_i16$local, .Lfunc_end595-test_store_based_i16
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
	st.b	%r2, %r1, 4000
	ret.f	0

.Lfunc_end596:
	.size	test_store_based_i8, .Lfunc_end596-test_store_based_i8
	.size	.Ltest_store_based_i8$local, .Lfunc_end596-test_store_based_i8
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
	divu.w	%r2, %r2, %r3
	st.d.xuw	%r4, %r1, %r2, 3, 0

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end597:
	.size	test_store_indexed_u32_u32, .Lfunc_end597-test_store_indexed_u32_u32
	.size	.Ltest_store_indexed_u32_u32$local, .Lfunc_end597-test_store_indexed_u32_u32
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
	st.d.xw	%r3, %r1, %r2, 3, 0
	ret.f	0

.Lfunc_end598:
	.size	test_store_indexed_i32, .Lfunc_end598-test_store_indexed_i32
	.size	.Ltest_store_indexed_i32$local, .Lfunc_end598-test_store_indexed_i32
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
	st.d.xw	%r3, %r1, %r2, 3, 40
	ret.f	0

.Lfunc_end599:
	.size	test_store_indexed_i32_offset, .Lfunc_end599-test_store_indexed_i32_offset
	.size	.Ltest_store_indexed_i32_offset$local, .Lfunc_end599-test_store_indexed_i32_offset
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
	st.d.xuw	%r3, %r1, %r2, 3, 0
	ret.f	0

.Lfunc_end600:
	.size	test_store_indexed_u32, .Lfunc_end600-test_store_indexed_u32
	.size	.Ltest_store_indexed_u32$local, .Lfunc_end600-test_store_indexed_u32
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
	addi.ws	%r2, %r2, 20
	st.d.xuw	%r3, %r1, %r2, 3, 0

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end601:
	.size	test_store_indexed_u32_offset, .Lfunc_end601-test_store_indexed_u32_offset
	.size	.Ltest_store_indexed_u32_offset$local, .Lfunc_end601-test_store_indexed_u32_offset
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
	st.d.xd	%r3, %r1, %r2, 3, 40
	ret.f	0

.Lfunc_end602:
	.size	test_store_indexed_i64, .Lfunc_end602-test_store_indexed_i64
	.size	.Ltest_store_indexed_i64$local, .Lfunc_end602-test_store_indexed_i64
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
	st.d.xw.l	%r3, %r1, %r2, 4, 328

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end603:
	.size	test_store_indexed_i32_struct, .Lfunc_end603-test_store_indexed_i32_struct
	.size	.Ltest_store_indexed_i32_struct$local, .Lfunc_end603-test_store_indexed_i32_struct
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
	ldz.b.r	%r1, %pcfwd(i8_zero)
	ldz.b.r	%r2, %pcfwd(u8_zero)

	! BUNDLE
	addi.ws	%r1, %r1, 1
	st.b.r	%r1, %pcfwd(i8_zero)
	ldz.h.r	%r3, %pcfwd(i16_zero)

	! BUNDLE
	addi.ws	%r2, %r2, 1
	st.b.r	%r2, %pcfwd(u8_zero)
	ldz.h.r	%r4, %pcfwd(u16_zero)

	! BUNDLE
	addi.ws	%r3, %r3, 1
	st.h.r	%r3, %pcfwd(i16_zero)
	ldz.w.r	%r5, %pcfwd(i32_zero)

	! BUNDLE
	addi.ws	%r4, %r4, 1
	st.h.r	%r4, %pcfwd(u16_zero)
	ldz.w.r	%r6, %pcfwd(u32_zero)

	! BUNDLE
	addi.ws	%r5, %r5, 1
	st.w.r	%r5, %pcfwd(i32_zero)
	ldz.d.r	%r7, %pcfwd(i64_zero)

	! BUNDLE
	addi.ws	%r6, %r6, 1
	st.w.r	%r6, %pcfwd(u32_zero)
	ldz.d.r	%r8, %pcfwd(u64_zero)

	! BUNDLE
	addi	%r9, %r7, 1
	st.d.r	%r9, %pcfwd(i64_zero)
	ld.q.r	%r9, %pcfwd(i128_zero)

	! BUNDLE
	addi	%r10, %r8, 1
	ld.q.r	%r11, %pcfwd(u128_zero)
	st.d.r	%r10, %pcfwd(u64_zero)

	! BUNDLE
	xori	%r9, %r9, 1
	st.q.r	%r9, %pcfwd(i128_zero)
	xori	%r10, %r11, 1

	! BUNDLE
	st.q.r	%r10, %pcfwd(u128_zero)
	slsrai	%r1, %r1, 56, 56
	andi	%r2, %r2, 255

	! BUNDLE
	add.ws	%r1, %r2, %r1
	slsrai	%r2, %r3, 48, 48
	add.ws	%r1, %r1, %r2

	! BUNDLE
	andi	%r2, %r4, 65535
	add.ws	%r1, %r1, %r2
	add.ws	%r1, %r1, %r5

	! BUNDLE
	add.ws	%r1, %r1, %r6
	ca.xuw	%r1, %r7, %r1, 0, 1
	ca.xd	%r1, %r8, %r1, 0, 1

	! BUNDLE
	xor	%r1, %r9, %r1
	xor	%r1, %r1, %r10
	ret.f	0

.Lfunc_end604:
	.size	dummy_return, .Lfunc_end604-dummy_return
	.size	.Ldummy_return$local, .Lfunc_end604-dummy_return
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
	st.b	%gz, %r1, 4000
	st.b.r	%gz, %pcfwd(i8_zero)

	! BUNDLE
	st.b.xd	%gz, %r1, %r2, 0, 10
	st.b.xw	%gz, %r1, %r3, 0, 10
	ret.f	0

.Lfunc_end605:
	.size	test_store_based_i8_zero, .Lfunc_end605-test_store_based_i8_zero
	.size	.Ltest_store_based_i8_zero$local, .Lfunc_end605-test_store_based_i8_zero
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
	st.b	%gz, %r1, 4000
	st.b.r	%gz, %pcfwd(u8_zero)

	! BUNDLE
	st.b.xd	%gz, %r1, %r2, 0, 10
	st.b.xw	%gz, %r1, %r3, 0, 10
	ret.f	0

.Lfunc_end606:
	.size	test_store_based_u8_zero, .Lfunc_end606-test_store_based_u8_zero
	.size	.Ltest_store_based_u8_zero$local, .Lfunc_end606-test_store_based_u8_zero
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
	st.h	%gz, %r1, 8000
	st.h.r	%gz, %pcfwd(i16_zero)

	! BUNDLE
	st.h.xd	%gz, %r1, %r2, 1, 20
	st.h.xw	%gz, %r1, %r3, 1, 20
	ret.f	0

.Lfunc_end607:
	.size	test_store_based_i16_zero, .Lfunc_end607-test_store_based_i16_zero
	.size	.Ltest_store_based_i16_zero$local, .Lfunc_end607-test_store_based_i16_zero
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
	st.h	%gz, %r1, 8000
	st.h.r	%gz, %pcfwd(u16_zero)

	! BUNDLE
	st.h.xd	%gz, %r1, %r2, 1, 20
	st.h.xw	%gz, %r1, %r3, 1, 20
	ret.f	0

.Lfunc_end608:
	.size	test_store_based_u16_zero, .Lfunc_end608-test_store_based_u16_zero
	.size	.Ltest_store_based_u16_zero$local, .Lfunc_end608-test_store_based_u16_zero
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
	st.w	%gz, %r1, 16000
	st.w.r	%gz, %pcfwd(i32_zero)

	! BUNDLE
	st.w.xd	%gz, %r1, %r2, 2, 40
	st.w.xw	%gz, %r1, %r3, 2, 40
	ret.f	0

.Lfunc_end609:
	.size	test_store_based_i32_zero, .Lfunc_end609-test_store_based_i32_zero
	.size	.Ltest_store_based_i32_zero$local, .Lfunc_end609-test_store_based_i32_zero
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
	st.w	%gz, %r1, 16000
	st.w.r	%gz, %pcfwd(u32_zero)

	! BUNDLE
	st.w.xd	%gz, %r1, %r2, 2, 40
	st.w.xw	%gz, %r1, %r3, 2, 40
	ret.f	0

.Lfunc_end610:
	.size	test_store_based_u32_zero, .Lfunc_end610-test_store_based_u32_zero
	.size	.Ltest_store_based_u32_zero$local, .Lfunc_end610-test_store_based_u32_zero
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
	st.d	%gz, %r1, 32000
	st.d.r	%gz, %pcfwd(i64_zero)

	! BUNDLE
	st.d.xd	%gz, %r1, %r2, 3, 40
	st.d.xw	%gz, %r1, %r3, 3, 40
	ret.f	0

.Lfunc_end611:
	.size	test_store_based_i64_zero, .Lfunc_end611-test_store_based_i64_zero
	.size	.Ltest_store_based_i64_zero$local, .Lfunc_end611-test_store_based_i64_zero
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
	st.d	%gz, %r1, 32000
	st.d.r	%gz, %pcfwd(u64_zero)

	! BUNDLE
	st.d.xd	%gz, %r1, %r2, 3, 40
	st.d.xw	%gz, %r1, %r3, 3, 40
	ret.f	0

.Lfunc_end612:
	.size	test_store_based_u64_zero, .Lfunc_end612-test_store_based_u64_zero
	.size	.Ltest_store_based_u64_zero$local, .Lfunc_end612-test_store_based_u64_zero
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
	st.q	%gz, %r1, 64000
	st.q.r	%gz, %pcfwd(i128_zero)

	! BUNDLE
	st.q.xd	%gz, %r1, %r2, 4, 48
	st.q.xw	%gz, %r1, %r3, 4, 48
	ret.f	0

.Lfunc_end613:
	.size	test_store_based_i128_zero, .Lfunc_end613-test_store_based_i128_zero
	.size	.Ltest_store_based_i128_zero$local, .Lfunc_end613-test_store_based_i128_zero
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
	st.q	%gz, %r1, 64000
	st.q.r	%gz, %pcfwd(u128_zero)

	! BUNDLE
	st.q.xd	%gz, %r1, %r2, 4, 48
	st.q.xw	%gz, %r1, %r3, 4, 48
	ret.f	0

.Lfunc_end614:
	.size	test_store_based_u128_zero, .Lfunc_end614-test_store_based_u128_zero
	.size	.Ltest_store_based_u128_zero$local, .Lfunc_end614-test_store_based_u128_zero
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
	alloc.sp	4, 240
	st.q	%gz, %sp, 64
	st.q	%gz, %sp, 80

	! BUNDLE
	st.q	%gz, %sp, 128
	st.q	%gz, %sp, 144
	st.q	%gz, %sp, 208

	! BUNDLE
	st.d	%gz, %sp, 224
	st.q	%gz, %sp, 192
	st.q	%gz, %sp, 176

	! BUNDLE
	st.q	%gz, %sp, 160
	st.q	%gz, %sp, 112
	st.q	%gz, %sp, 96

	! BUNDLE
	st.q	%gz, %sp, 48
	st.q	%gz, %sp, 32
	st.q	%gz, %sp, 16

	! BUNDLE
	st.q	%gz, %sp, 0
	ca.r	%r1, %jpc(sn_write)
	st.d	%r1, %sp, 72

	! BUNDLE
	addi	%r1, %sp, 239
	st.d	%r1, %sp, 88
	ldi	%r1, -1

	! BUNDLE
	st.w	%r1, %sp, 140
	st.w	%r1, %sp, 144
	addi	%r3, %sp, 0

	! BUNDLE
	st.d	%r3, %sp, 208
	call.r	%r2, %jpc(test_struct_memset_consumer)
	mov	%r1, %r3

	! BUNDLE
	ret.f	240
	nop	0
	nop	0

.Lfunc_end615:
	.size	test_struct_memset, .Lfunc_end615-test_struct_memset
	.size	.Ltest_struct_memset$local, .Lfunc_end615-test_struct_memset
                                        ! -- End function
	.p2align	4                               ! -- Begin function sn_write
	.type	sn_write,@function
sn_write:                               ! @sn_write
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc	7
	ldz.d	%r2, %r1, 152
	ldz.d	%r4, %r1, 40

	! BUNDLE
	ldz.d	%r6, %r1, 56
	ldz.d	%r5, %r2, 8
	sub	%r4, %r4, %r6

	! BUNDLE
	minu	%r6, %r5, %r4
	ldz.d	%r4, %r2, 0
	nul.eq.d	%r6, %gz, 4, 0

	! BUNDLE
	add	%r4, %r4, %r6
	st.d	%r4, %r2, 0
	sub	%r5, %r5, %r6

	! BUNDLE
	st.d	%r5, %r2, 8
	minu	%r6, %r5, %r3
	nul.eq.d	%r6, %gz, 4, 0

	! BUNDLE
	add	%r4, %r4, %r6
	st.d	%r4, %r2, 0
	sub	%r5, %r5, %r6

	! BUNDLE
	st.d	%r5, %r2, 8
	st.b	%gz, %r4, 0
	ldz.d	%r2, %r1, 88

	! BUNDLE
	st.d	%r2, %r1, 56
	st.d	%r2, %r1, 40
	mov	%r1, %r3

	! BUNDLE
	ret.f	0
	nop	0
	nop	0

.Lfunc_end616:
	.size	sn_write, .Lfunc_end616-sn_write
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
	alloc.sp	5, 16
	bui.ge.d	%r1, 6, .LBB617_9
	ca.rf	%r2, %pcfwd(.LJTI617_0)

	! BUNDLE
	jmp.t	%r2, %r1
	nop	0
	nop	0

.LBB617_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	ca.rf	%r4, %pcfwd(.L.str.38)
	jmp	.LBB617_8

.LBB617_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 8
	st.d	%r1, %sp, 8
	ldi	%r1, 5

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.42)
	jmp	.LBB617_8
	nop	0

.LBB617_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	st.d	%r1, %sp, 8
	ldi	%r1, 3

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.40)
	jmp	.LBB617_8
	nop	0

.LBB617_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	st.d	%r1, %sp, 8
	ldi	%r1, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.41)
	jmp	.LBB617_8
	nop	0

.LBB617_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	st.d	%gz, %sp, 8
	ldi	%r1, 2
	ca.rf	%r4, %pcfwd(.L.str.39)

	! BUNDLE
	jmp	.LBB617_8
	nop	0
	nop	0

.LBB617_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 27
	st.d	%r1, %sp, 8
	ldi	%r1, 6

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.43)
	nop	0
	nop	0

.LBB617_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	nop	0

.LBB617_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	ret.f	16
	nop	0

.Lfunc_end617:
	.size	test_switch_table_i64, .Lfunc_end617-test_switch_table_i64
	.size	.Ltest_switch_table_i64$local, .Lfunc_end617-test_switch_table_i64
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI617_0:
	.word	.LBB617_2-.LJTI617_0
	.word	.LBB617_3-.LJTI617_0
	.word	.LBB617_4-.LJTI617_0
	.word	.LBB617_5-.LJTI617_0
	.word	.LBB617_6-.LJTI617_0
	.word	.LBB617_7-.LJTI617_0
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
	alloc.sp	5, 16
	bui.ge.w	%r1, 6, .LBB618_9
	srli.w	%r1, %r1, 0

	! BUNDLE
	ca.rf	%r2, %pcfwd(.LJTI618_0)
	jmp.t	%r2, %r1
	nop	0

.LBB618_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	ca.rf	%r4, %pcfwd(.L.str.44)
	jmp	.LBB618_8

.LBB618_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 8
	st.w	%r1, %sp, 4
	ldi	%r1, 5

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.48)
	jmp	.LBB618_8
	nop	0

.LBB618_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	st.w	%r1, %sp, 4
	ldi	%r1, 3

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.46)
	jmp	.LBB618_8
	nop	0

.LBB618_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	st.w	%r1, %sp, 4
	ldi	%r1, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.47)
	jmp	.LBB618_8
	nop	0

.LBB618_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%gz, %sp, 4
	ldi	%r1, 2
	ca.rf	%r4, %pcfwd(.L.str.45)

	! BUNDLE
	jmp	.LBB618_8
	nop	0
	nop	0

.LBB618_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 27
	st.w	%r1, %sp, 4
	ldi	%r1, 6

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.49)
	nop	0
	nop	0

.LBB618_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	nop	0

.LBB618_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	ret.f	16
	nop	0

.Lfunc_end618:
	.size	test_switch_table_i32, .Lfunc_end618-test_switch_table_i32
	.size	.Ltest_switch_table_i32$local, .Lfunc_end618-test_switch_table_i32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI618_0:
	.word	.LBB618_2-.LJTI618_0
	.word	.LBB618_3-.LJTI618_0
	.word	.LBB618_4-.LJTI618_0
	.word	.LBB618_5-.LJTI618_0
	.word	.LBB618_6-.LJTI618_0
	.word	.LBB618_7-.LJTI618_0
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
	alloc.sp	5, 16
	bui.ge.w	%r1, 6, .LBB619_9
	srli.w	%r1, %r1, 0

	! BUNDLE
	ca.rf	%r2, %pcfwd(.LJTI619_0)
	jmp.t	%r2, %r1
	nop	0

.LBB619_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	ca.rf	%r4, %pcfwd(.L.str.50)
	jmp	.LBB619_8

.LBB619_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 8
	st.w	%r1, %sp, 4
	ldi	%r1, 5

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.54)
	jmp	.LBB619_8
	nop	0

.LBB619_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 2
	st.w	%r1, %sp, 4
	ldi	%r1, 3

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.52)
	jmp	.LBB619_8
	nop	0

.LBB619_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 1
	st.w	%r1, %sp, 4
	ldi	%r1, 4

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.53)
	jmp	.LBB619_8
	nop	0

.LBB619_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%gz, %sp, 4
	ldi	%r1, 2
	ca.rf	%r4, %pcfwd(.L.str.51)

	! BUNDLE
	jmp	.LBB619_8
	nop	0
	nop	0

.LBB619_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 27
	st.w	%r1, %sp, 4
	ldi	%r1, 6

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.55)
	nop	0
	nop	0

.LBB619_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	nop	0

.LBB619_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	ret.f	16
	nop	0

.Lfunc_end619:
	.size	test_switch_table_u32, .Lfunc_end619-test_switch_table_u32
	.size	.Ltest_switch_table_u32$local, .Lfunc_end619-test_switch_table_u32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI619_0:
	.word	.LBB619_2-.LJTI619_0
	.word	.LBB619_3-.LJTI619_0
	.word	.LBB619_4-.LJTI619_0
	.word	.LBB619_5-.LJTI619_0
	.word	.LBB619_6-.LJTI619_0
	.word	.LBB619_7-.LJTI619_0
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
	alloc.sp	5, 16
	addi	%r1, %r1, -10
	bui.ge.d	%r1, 6, .LBB620_9

	! BUNDLE
	ca.rf	%r2, %pcfwd(.LJTI620_0)
	jmp.t	%r2, %r1
	nop	0

.LBB620_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 11
	ca.rf	%r4, %pcfwd(.L.str.56)
	jmp	.LBB620_8

.LBB620_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 28
	st.d	%r1, %sp, 8
	ldi	%r1, 15

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.60)
	jmp	.LBB620_8
	nop	0

.LBB620_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 14
	st.d	%r1, %sp, 8
	ldi	%r1, 13

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.58)
	jmp	.LBB620_8
	nop	0

.LBB620_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 15
	st.d	%r1, %sp, 8
	ldi	%r1, 14

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.59)
	jmp	.LBB620_8
	nop	0

.LBB620_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 10
	st.d	%r1, %sp, 8
	ldi	%r1, 12

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.57)
	jmp	.LBB620_8
	nop	0

.LBB620_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 37
	st.d	%r1, %sp, 8
	ldi	%r1, 16

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.61)
	nop	0
	nop	0

.LBB620_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st.d	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	nop	0

.LBB620_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	ret.f	16
	nop	0

.Lfunc_end620:
	.size	test_switch_table_nonzero_i64, .Lfunc_end620-test_switch_table_nonzero_i64
	.size	.Ltest_switch_table_nonzero_i64$local, .Lfunc_end620-test_switch_table_nonzero_i64
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI620_0:
	.word	.LBB620_2-.LJTI620_0
	.word	.LBB620_3-.LJTI620_0
	.word	.LBB620_4-.LJTI620_0
	.word	.LBB620_5-.LJTI620_0
	.word	.LBB620_6-.LJTI620_0
	.word	.LBB620_7-.LJTI620_0
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
	alloc.sp	5, 16
	addi.ws	%r2, %r1, -10
	bui.ge.w	%r2, 6, .LBB621_9

	! BUNDLE
	addi.wz	%r1, %r1, -10
	ca.rf	%r2, %pcfwd(.LJTI621_0)
	jmp.t	%r2, %r1

.LBB621_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 11
	ca.rf	%r4, %pcfwd(.L.str.62)
	jmp	.LBB621_8

.LBB621_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 28
	st.w	%r1, %sp, 4
	ldi	%r1, 15

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.66)
	jmp	.LBB621_8
	nop	0

.LBB621_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 14
	st.w	%r1, %sp, 4
	ldi	%r1, 13

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.64)
	jmp	.LBB621_8
	nop	0

.LBB621_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 15
	st.w	%r1, %sp, 4
	ldi	%r1, 14

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.65)
	jmp	.LBB621_8
	nop	0

.LBB621_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 10
	st.w	%r1, %sp, 4
	ldi	%r1, 12

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.63)
	jmp	.LBB621_8
	nop	0

.LBB621_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 37
	st.w	%r1, %sp, 4
	ldi	%r1, 16

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.67)
	nop	0
	nop	0

.LBB621_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	nop	0

.LBB621_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	ret.f	16
	nop	0

.Lfunc_end621:
	.size	test_switch_table_nonzero_i32, .Lfunc_end621-test_switch_table_nonzero_i32
	.size	.Ltest_switch_table_nonzero_i32$local, .Lfunc_end621-test_switch_table_nonzero_i32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI621_0:
	.word	.LBB621_2-.LJTI621_0
	.word	.LBB621_3-.LJTI621_0
	.word	.LBB621_4-.LJTI621_0
	.word	.LBB621_5-.LJTI621_0
	.word	.LBB621_6-.LJTI621_0
	.word	.LBB621_7-.LJTI621_0
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
	alloc.sp	5, 16
	addi.ws	%r2, %r1, -10
	bui.ge.w	%r2, 6, .LBB622_9

	! BUNDLE
	addi.wz	%r1, %r1, -10
	ca.rf	%r2, %pcfwd(.LJTI622_0)
	jmp.t	%r2, %r1

.LBB622_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 11
	ca.rf	%r4, %pcfwd(.L.str.68)
	jmp	.LBB622_8

.LBB622_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 28
	st.w	%r1, %sp, 4
	ldi	%r1, 15

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.72)
	jmp	.LBB622_8
	nop	0

.LBB622_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 14
	st.w	%r1, %sp, 4
	ldi	%r1, 13

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.70)
	jmp	.LBB622_8
	nop	0

.LBB622_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 15
	st.w	%r1, %sp, 4
	ldi	%r1, 14

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.71)
	jmp	.LBB622_8
	nop	0

.LBB622_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 10
	st.w	%r1, %sp, 4
	ldi	%r1, 12

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.69)
	jmp	.LBB622_8
	nop	0

.LBB622_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ldi	%r1, 37
	st.w	%r1, %sp, 4
	ldi	%r1, 16

	! BUNDLE
	ca.rf	%r4, %pcfwd(.L.str.73)
	nop	0
	nop	0

.LBB622_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st.w	%r1, %sp, 0
	call.r	%r3, %jpc(printf)
	nop	0

.LBB622_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov	%r1, %gz
	ret.f	16
	nop	0

.Lfunc_end622:
	.size	test_switch_table_nonzero_u32, .Lfunc_end622-test_switch_table_nonzero_u32
	.size	.Ltest_switch_table_nonzero_u32$local, .Lfunc_end622-test_switch_table_nonzero_u32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI622_0:
	.word	.LBB622_2-.LJTI622_0
	.word	.LBB622_3-.LJTI622_0
	.word	.LBB622_4-.LJTI622_0
	.word	.LBB622_5-.LJTI622_0
	.word	.LBB622_6-.LJTI622_0
	.word	.LBB622_7-.LJTI622_0
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
	ret.f	0

.Lfunc_end623:
	.size	test_syscall_2, .Lfunc_end623-test_syscall_2
	.size	.Ltest_syscall_2$local, .Lfunc_end623-test_syscall_2
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
	ret.f	0
	nop	0
	nop	0

.Lfunc_end624:
	.size	test_syscall, .Lfunc_end624-test_syscall
	.size	.Ltest_syscall$local, .Lfunc_end624-test_syscall
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

.Lfunc_end625:
	.size	test_int, .Lfunc_end625-test_int
	.size	.Ltest_int$local, .Lfunc_end625-test_int
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
	addi.ws	%r3, %r1, 1
	addi.ws	%r4, %r1, 10

	! BUNDLE
	call.r	%r2, %jpc(tail_call)
	mov	%r1, %r3
	ret.f	0

.Lfunc_end626:
	.size	test_call_global_tailcall, .Lfunc_end626-test_call_global_tailcall
	.size	.Ltest_call_global_tailcall$local, .Lfunc_end626-test_call_global_tailcall
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
	call.mi	%r3, %r1, 48

	! BUNDLE
	mov	%r1, %r4
	ret.f	0
	nop	0

.Lfunc_end627:
	.size	test_ptr_tailcall, .Lfunc_end627-test_ptr_tailcall
	.size	.Ltest_ptr_tailcall$local, .Lfunc_end627-test_ptr_tailcall
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
	call.plt	%r2, %pcfwd(global_ftable+40)

	! BUNDLE
	mov	%r1, %r3
	ret.f	0
	nop	0

.Lfunc_end628:
	.size	test_call_global_ptr_table_tailcall, .Lfunc_end628-test_call_global_ptr_table_tailcall
	.size	.Ltest_call_global_ptr_table_tailcall$local, .Lfunc_end628-test_call_global_ptr_table_tailcall
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
	ret.f	0

.Lfunc_end629:
	.size	test_builtin_tlb_purge, .Lfunc_end629-test_builtin_tlb_purge
	.size	.Ltest_builtin_tlb_purge$local, .Lfunc_end629-test_builtin_tlb_purge
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
	alloc.sp	5, 16
	ca.rf	%r4, %tld(x_tls)
	call.r	%r3, %jpc(__tls_get_addr)

	! BUNDLE
	ldz.d	%r2, %r4, %dtpoff(x_tls)
	add	%r1, %r2, %r1
	ret.f	16

.Lfunc_end630:
	.size	test_local_tls, .Lfunc_end630-test_local_tls
	.size	.Ltest_local_tls$local, .Lfunc_end630-test_local_tls
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
	alloc.sp	5, 16
	ca.rf	%r4, %tld(x_tls_extern)
	call.r	%r3, %jpc(__tls_get_addr)

	! BUNDLE
	ldz.d	%r2, %r4, %dtpoff(x_tls_extern)
	add	%r1, %r2, %r1
	ret.f	16

.Lfunc_end631:
	.size	test_extern_tls, .Lfunc_end631-test_extern_tls
	.size	.Ltest_extern_tls$local, .Lfunc_end631-test_extern_tls
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
	alloc.sp	7, 144
	addi	%r6, %sp, 144
	st.d	%r6, %sp, 8

	! BUNDLE
	addi.ws	%r3, %r1, 1
	addi.ws	%r4, %r1, -1
	addi	%r5, %sp, 16

	! BUNDLE
	call.r	%r2, %jpc(test_va_list_callback)
	mov	%r1, %r3
	ret.f	144

.Lfunc_end632:
	.size	test_va_start, .Lfunc_end632-test_va_start
	.size	.Ltest_va_start$local, .Lfunc_end632-test_va_start
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
	alloc.sp	8, 144
	addi	%r1, %sp, 144
	st.d	%r1, %sp, 8

	! BUNDLE
	nop	1000
	ldz.d	%r1, %sp, 8
	algnup	%r1, %r1, 0, 2

	! BUNDLE
	addi	%r2, %r1, 4
	st.d	%r2, %sp, 8
	ldz.w	%r4, %r1, 0

	! BUNDLE
	nop	1001
	ldz.d	%r1, %sp, 8
	algnup	%r1, %r1, 0, 3

	! BUNDLE
	addi	%r2, %r1, 8
	st.d	%r2, %sp, 8
	ldz.d	%r5, %r1, 0

	! BUNDLE
	nop	1002
	nop	1003
	addi	%r6, %sp, 16

	! BUNDLE
	addi	%r7, %sp, 8
	call.r	%r3, %jpc(test_va_callback)
	mov	%r1, %r4

	! BUNDLE
	ret.f	144
	nop	0
	nop	0

.Lfunc_end633:
	.size	test_va_start_end, .Lfunc_end633-test_va_start_end
	.size	.Ltest_va_start_end$local, .Lfunc_end633-test_va_start_end
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
	alloc.sp	6, 16
	st.d	%r2, %sp, 8
	st.d	%r2, %sp, 0

	! BUNDLE
	ldi	%r4, 100
	addi	%r1, %sp, 0
	mov	%r5, %r1

	! BUNDLE
	call.r	%r3, %jpc(test_printf_callback)
	ldi	%r4, 200
	mov	%r5, %r1

	! BUNDLE
	call.r	%r3, %jpc(test_printf_callback)
	mov	%r1, %r4
	ret.f	16

.Lfunc_end634:
	.size	test_printf_valist, .Lfunc_end634-test_printf_valist
	.size	.Ltest_printf_valist$local, .Lfunc_end634-test_printf_valist
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
	alloc.sp	6, 32
	st.d	%r2, %sp, 24
	st.d	%r2, %sp, 16

	! BUNDLE
	st.d	%r2, %sp, 8
	ldi	%r4, 100
	addi	%r5, %sp, 16

	! BUNDLE
	call.r	%r3, %jpc(test_vprintf_callback)
	ldi	%r4, 200
	addi	%r5, %sp, 8

	! BUNDLE
	call.r	%r3, %jpc(test_vprintf_callback)
	mov	%r1, %r4
	ret.f	32

.Lfunc_end635:
	.size	test_vprintf_valist, .Lfunc_end635-test_vprintf_valist
	.size	.Ltest_vprintf_valist$local, .Lfunc_end635-test_vprintf_valist
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
	alloc.sp	5, 16
	st.d	%r2, %sp, 8
	st.d	%r2, %sp, 0

	! BUNDLE
	addi	%r1, %sp, 0
	mov	%r4, %r1
	call.r	%r3, %jpc(test_valist_callback)

	! BUNDLE
	mov	%r4, %r1
	call.r	%r3, %jpc(test_valist_callback)
	mov	%r1, %r4

	! BUNDLE
	ret.f	16
	nop	0
	nop	0

.Lfunc_end636:
	.size	test_vacopy, .Lfunc_end636-test_vacopy
	.size	.Ltest_vacopy$local, .Lfunc_end636-test_vacopy
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
	ret.f	0
	nop	0

.Lfunc_end637:
	.size	test_vector_bitcast_v2i64_to_v4i32, .Lfunc_end637-test_vector_bitcast_v2i64_to_v4i32
	.size	.Ltest_vector_bitcast_v2i64_to_v4i32$local, .Lfunc_end637-test_vector_bitcast_v2i64_to_v4i32
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
	ret.f	0
	nop	0

.Lfunc_end638:
	.size	test_vector_bitcast_v2i64_to_v8i16, .Lfunc_end638-test_vector_bitcast_v2i64_to_v8i16
	.size	.Ltest_vector_bitcast_v2i64_to_v8i16$local, .Lfunc_end638-test_vector_bitcast_v2i64_to_v8i16
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
	ret.f	0
	nop	0

.Lfunc_end639:
	.size	test_vector_bitcast_v2i64_to_v16i8, .Lfunc_end639-test_vector_bitcast_v2i64_to_v16i8
	.size	.Ltest_vector_bitcast_v2i64_to_v16i8$local, .Lfunc_end639-test_vector_bitcast_v2i64_to_v16i8
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
	ret.f	0
	nop	0

.Lfunc_end640:
	.size	test_vector_bitcast_v4i32_to_v2i64, .Lfunc_end640-test_vector_bitcast_v4i32_to_v2i64
	.size	.Ltest_vector_bitcast_v4i32_to_v2i64$local, .Lfunc_end640-test_vector_bitcast_v4i32_to_v2i64
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
	ret.f	0
	nop	0

.Lfunc_end641:
	.size	test_vector_bitcast_v4i32_to_v8i16, .Lfunc_end641-test_vector_bitcast_v4i32_to_v8i16
	.size	.Ltest_vector_bitcast_v4i32_to_v8i16$local, .Lfunc_end641-test_vector_bitcast_v4i32_to_v8i16
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
	ret.f	0
	nop	0

.Lfunc_end642:
	.size	test_vector_bitcast_v4i32_to_v16i8, .Lfunc_end642-test_vector_bitcast_v4i32_to_v16i8
	.size	.Ltest_vector_bitcast_v4i32_to_v16i8$local, .Lfunc_end642-test_vector_bitcast_v4i32_to_v16i8
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
	ret.f	0
	nop	0

.Lfunc_end643:
	.size	test_vector_bitcast_v8i16_to_v2i64, .Lfunc_end643-test_vector_bitcast_v8i16_to_v2i64
	.size	.Ltest_vector_bitcast_v8i16_to_v2i64$local, .Lfunc_end643-test_vector_bitcast_v8i16_to_v2i64
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
	ret.f	0
	nop	0

.Lfunc_end644:
	.size	test_vector_bitcast_v8i16_to_v4i32, .Lfunc_end644-test_vector_bitcast_v8i16_to_v4i32
	.size	.Ltest_vector_bitcast_v8i16_to_v4i32$local, .Lfunc_end644-test_vector_bitcast_v8i16_to_v4i32
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
	ret.f	0
	nop	0

.Lfunc_end645:
	.size	test_vector_bitcast_v8i16_to_v16i8, .Lfunc_end645-test_vector_bitcast_v8i16_to_v16i8
	.size	.Ltest_vector_bitcast_v8i16_to_v16i8$local, .Lfunc_end645-test_vector_bitcast_v8i16_to_v16i8
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
	ret.f	0
	nop	0

.Lfunc_end646:
	.size	test_vector_bitcast_v16i8_to_v2i64, .Lfunc_end646-test_vector_bitcast_v16i8_to_v2i64
	.size	.Ltest_vector_bitcast_v16i8_to_v2i64$local, .Lfunc_end646-test_vector_bitcast_v16i8_to_v2i64
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
	ret.f	0
	nop	0

.Lfunc_end647:
	.size	test_vector_bitcast_v16i8_to_v4i32, .Lfunc_end647-test_vector_bitcast_v16i8_to_v4i32
	.size	.Ltest_vector_bitcast_v16i8_to_v4i32$local, .Lfunc_end647-test_vector_bitcast_v16i8_to_v4i32
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
	ret.f	0
	nop	0

.Lfunc_end648:
	.size	test_vector_bitcast_v16i8_to_v8i16, .Lfunc_end648-test_vector_bitcast_v16i8_to_v8i16
	.size	.Ltest_vector_bitcast_v16i8_to_v8i16$local, .Lfunc_end648-test_vector_bitcast_v16i8_to_v8i16
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
	ret.f	0
	nop	0

.Lfunc_end649:
	.size	test_vector_bitcast_f128_to_v16i8, .Lfunc_end649-test_vector_bitcast_f128_to_v16i8
	.size	.Ltest_vector_bitcast_f128_to_v16i8$local, .Lfunc_end649-test_vector_bitcast_f128_to_v16i8
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
	ret.f	0
	nop	0

.Lfunc_end650:
	.size	test_vector_bitcast_f128_to_v8i16, .Lfunc_end650-test_vector_bitcast_f128_to_v8i16
	.size	.Ltest_vector_bitcast_f128_to_v8i16$local, .Lfunc_end650-test_vector_bitcast_f128_to_v8i16
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
	ret.f	0
	nop	0

.Lfunc_end651:
	.size	test_vector_bitcast_f128_to_v4i32, .Lfunc_end651-test_vector_bitcast_f128_to_v4i32
	.size	.Ltest_vector_bitcast_f128_to_v4i32$local, .Lfunc_end651-test_vector_bitcast_f128_to_v4i32
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
	ret.f	0
	nop	0

.Lfunc_end652:
	.size	test_vector_bitcast_f128_to_v2i64, .Lfunc_end652-test_vector_bitcast_f128_to_v2i64
	.size	.Ltest_vector_bitcast_f128_to_v2i64$local, .Lfunc_end652-test_vector_bitcast_f128_to_v2i64
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
	ret.f	0
	nop	0

.Lfunc_end653:
	.size	test_vector_bitcast_v16i8_to_f128, .Lfunc_end653-test_vector_bitcast_v16i8_to_f128
	.size	.Ltest_vector_bitcast_v16i8_to_f128$local, .Lfunc_end653-test_vector_bitcast_v16i8_to_f128
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
	ret.f	0
	nop	0

.Lfunc_end654:
	.size	test_vector_bitcast_v8i16_to_f128, .Lfunc_end654-test_vector_bitcast_v8i16_to_f128
	.size	.Ltest_vector_bitcast_v8i16_to_f128$local, .Lfunc_end654-test_vector_bitcast_v8i16_to_f128
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
	ret.f	0
	nop	0

.Lfunc_end655:
	.size	test_vector_bitcast_v4i32_to_f128, .Lfunc_end655-test_vector_bitcast_v4i32_to_f128
	.size	.Ltest_vector_bitcast_v4i32_to_f128$local, .Lfunc_end655-test_vector_bitcast_v4i32_to_f128
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
	ret.f	0
	nop	0

.Lfunc_end656:
	.size	test_vector_bitcast_v2i64_to_f128, .Lfunc_end656-test_vector_bitcast_v2i64_to_f128
	.size	.Ltest_vector_bitcast_v2i64_to_f128$local, .Lfunc_end656-test_vector_bitcast_v2i64_to_f128
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
	ret.f	0
	nop	0

.Lfunc_end657:
	.size	test_vector_bitcast_i128_to_v16i8, .Lfunc_end657-test_vector_bitcast_i128_to_v16i8
	.size	.Ltest_vector_bitcast_i128_to_v16i8$local, .Lfunc_end657-test_vector_bitcast_i128_to_v16i8
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
	ret.f	0
	nop	0

.Lfunc_end658:
	.size	test_vector_bitcast_i128_to_v8i16, .Lfunc_end658-test_vector_bitcast_i128_to_v8i16
	.size	.Ltest_vector_bitcast_i128_to_v8i16$local, .Lfunc_end658-test_vector_bitcast_i128_to_v8i16
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
	ret.f	0
	nop	0

.Lfunc_end659:
	.size	test_vector_bitcast_i128_to_v4i32, .Lfunc_end659-test_vector_bitcast_i128_to_v4i32
	.size	.Ltest_vector_bitcast_i128_to_v4i32$local, .Lfunc_end659-test_vector_bitcast_i128_to_v4i32
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
	ret.f	0
	nop	0

.Lfunc_end660:
	.size	test_vector_bitcast_i128_to_v2i64, .Lfunc_end660-test_vector_bitcast_i128_to_v2i64
	.size	.Ltest_vector_bitcast_i128_to_v2i64$local, .Lfunc_end660-test_vector_bitcast_i128_to_v2i64
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
	ret.f	0
	nop	0

.Lfunc_end661:
	.size	test_vector_bitcast_v16i8_to_i128, .Lfunc_end661-test_vector_bitcast_v16i8_to_i128
	.size	.Ltest_vector_bitcast_v16i8_to_i128$local, .Lfunc_end661-test_vector_bitcast_v16i8_to_i128
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
	ret.f	0
	nop	0

.Lfunc_end662:
	.size	test_vector_bitcast_v8i16_to_i128, .Lfunc_end662-test_vector_bitcast_v8i16_to_i128
	.size	.Ltest_vector_bitcast_v8i16_to_i128$local, .Lfunc_end662-test_vector_bitcast_v8i16_to_i128
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
	ret.f	0
	nop	0

.Lfunc_end663:
	.size	test_vector_bitcast_v4i32_to_i128, .Lfunc_end663-test_vector_bitcast_v4i32_to_i128
	.size	.Ltest_vector_bitcast_v4i32_to_i128$local, .Lfunc_end663-test_vector_bitcast_v4i32_to_i128
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
	ret.f	0
	nop	0

.Lfunc_end664:
	.size	test_vector_bitcast_v2i64_to_i128, .Lfunc_end664-test_vector_bitcast_v2i64_to_i128
	.size	.Ltest_vector_bitcast_v2i64_to_i128$local, .Lfunc_end664-test_vector_bitcast_v2i64_to_i128
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
	alloc.sp	13, 16
	mov	%r5, %fp
	addi	%fp, %sp, 16

	! BUNDLE
	addi	%sp, %sp, -16
	ca.rf	%r12, %pcfwd(.L.str.74)
	st.d	%r1, %sp, 0

	! BUNDLE
	call.r	%r11, %jpc(printf)
	addi	%sp, %sp, 16
	algnup	%r4, %r1, 3, 4

	! BUNDLE
	sub	%r4, %sp, %r4
	mov	%sp, %r4
	add	%r9, %r2, %r1

	! BUNDLE
	ca.xd.l	%r8, %r9, %r3, 0, 333
	sub	%r3, %r1, %r2

	! BUNDLE
	addi	%r10, %r3, 44
	sl.add	%r11, %r2, %r1, 1
	sl.add	%r12, %r1, %r2, 1

	! BUNDLE
	mov	%r7, %r4
	call.r	%r6, %jpc(stack_array_consumer)
	st.d	%r7, %fp, -8

	! BUNDLE
	ldz.d	%r3, %r4, 0
	ldz.d	%r4, %fp, -8
	add.add	%r2, %r3, %r2, %r4

	! BUNDLE
	st.d	%r2, %fp, -8
	ldz.d	%r2, %fp, -8
	addi	%sp, %sp, -16

	! BUNDLE
	ca.rf	%r12, %pcfwd(.L.str.75)
	st.d	%r2, %sp, 0
	call.r	%r11, %jpc(printf)

	! BUNDLE
	addi	%sp, %sp, 16
	ldz.d	%r2, %fp, -8
	add	%r1, %r2, %r1

	! BUNDLE
	mov	%sp, %fp
	mov	%fp, %r5
	ret.f	0

.Lfunc_end665:
	.size	test_stack_array, .Lfunc_end665-test_stack_array
	.size	.Ltest_stack_array$local, .Lfunc_end665-test_stack_array
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
	.asciz	"x32=%d"
	.size	.L.str.12, 7

	.type	.L.str.13,@object               ! @.str.13
.L.str.13:
	.asciz	"x32=%lld"
	.size	.L.str.13, 9

	.type	.L.str.14,@object               ! @.str.14
.L.str.14:
	.asciz	"test"
	.size	.L.str.14, 5

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

	.ident	"clang version 21.0.0-postrisc"
	.section	".note.GNU-stack","",@progbits
