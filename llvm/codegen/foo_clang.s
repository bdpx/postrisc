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
	alloc 2
	abs_i64 %r1, %r1
	retf 0

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
	alloc 2
	abs_i32 %r1, %r1
	retf 0

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
	alloc 3
	abs_diff_i64 %r1, %r2, %r1
	retf 0

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
	alloc 3
	abs_diff_i32 %r1, %r2, %r1
	retf 0

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
	alloc 3
	aes_enc %r1, %r1, %r2
	retf 0

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
	alloc 3
	aes_enc_last %r1, %r1, %r2
	retf 0

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
	alloc 3
	aes_dec %r1, %r1, %r2
	retf 0

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
	alloc 3
	aes_dec_last %r1, %r1, %r2
	retf 0

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
	alloc 2
	aes_imc %r1, %r1
	retf 0

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
	alloc 2
	aes_keygen_assist %r1, %r1, 100
	aes_keygen_assist %r1, %r1, -1

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 2
	ld_imm %r1, 12345778
	retf 0

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
	alloc 2
	ld_imm.l %r1, 987654321

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 2
	ld_imm %r1, 12345778
	retf 0

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
	alloc 2
	ld_imm.l %r1, 987654321123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end13:
	.size	test_ldi64_long, .Lfunc_end13-test_ldi64_long
	.size	.Ltest_ldi64_long$local, .Lfunc_end13-test_ldi64_long
                                        ! -- End function
	.globl	test_subr_i64                   ! -- Begin function test_subr_i64
	.p2align	4
	.type	test_subr_i64,@function
test_subr_i64:                          ! @test_subr_i64
.Ltest_subr_i64$local:
	.type	.Ltest_subr_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	subr_imm_i64 %r1, %r1, 1234
	retf 0

.Lfunc_end14:
	.size	test_subr_i64, .Lfunc_end14-test_subr_i64
	.size	.Ltest_subr_i64$local, .Lfunc_end14-test_subr_i64
                                        ! -- End function
	.globl	test_subr_i32                   ! -- Begin function test_subr_i32
	.p2align	4
	.type	test_subr_i32,@function
test_subr_i32:                          ! @test_subr_i32
.Ltest_subr_i32$local:
	.type	.Ltest_subr_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	subr_imm_i32 %r1, %r1, 1234
	retf 0

.Lfunc_end15:
	.size	test_subr_i32, .Lfunc_end15-test_subr_i32
	.size	.Ltest_subr_i32$local, .Lfunc_end15-test_subr_i32
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
	alloc 3
	add_imm_i64.l %r1, %r1, 123456789123456789

	! BUNDLE
	ld_imm.l %r2, 9200848539817279407
	or %r1, %r1, %r2

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 2
	and_imm.l %r1, %r1, 123456789123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 2
	or_imm.l %r1, %r1, 987654321123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 4
	add_add_i64 %r1, %r2, %r1, %r3
	retf 0

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
	alloc 4
	add_sub_i64 %r1, %r2, %r1, %r3
	retf 0

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
	alloc 4
	sub_sub_i64 %r1, %r1, %r2, %r3
	retf 0

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
	alloc 3
	mul_i32 %r1, %r2, %r1
	retf 0

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
	alloc 2
	mul_imm_i32 %r1, %r1, 98765
	retf 0

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
	alloc 2
	mul_imm_i32.l %r1, %r1, 98765432

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 2
	mul_imm_i32 %r1, %r1, 98765
	retf 0

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
	alloc 2
	mul_imm_i32.l %r1, %r1, 98765432

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 3
	mul_i32 %r1, %r2, %r1
	retf 0

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
	alloc 3
	mul_u32 %r1, %r2, %r1
	retf 0

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
	alloc 3
	mul_i32 %r1, %r2, %r1
	retf 0

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
	alloc 2
	mul_imm_i32 %r1, %r1, 98765
	retf 0

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
	alloc 2
	mul_imm_i32.l %r1, %r1, 98765432

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 2
	mul_imm_u32 %r1, %r1, 98765
	retf 0

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
	alloc 2
	mul_imm_u32.l %r1, %r1, 98765432

	! BUNDLE
	retf 0
	nop 0
	nop 0

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
	alloc 3
	mul_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end34:
	.size	test_mul_i64_i64, .Lfunc_end34-test_mul_i64_i64
	.size	.Ltest_mul_i64_i64$local, .Lfunc_end34-test_mul_i64_i64
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
	alloc_sp 7, 16
	st_i32 %gz, %sp, 12
	ld_u8 %r6, %r2, 0

	! BUNDLE
	amx_cas_i8 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i8 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u8 %r6, %r2, 0
	amq_cas_i8 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u8 %r6, %r2, 0
	amq_cas_i8 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u8 %r6, %r2, 0

	! BUNDLE
	amq_cas_i8 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i8 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u8 %r6, %r2, 0
	amq_cas_i8 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u8 %r6, %r2, 0
	amq_cas_i8 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u8 %r6, %r2, 0

	! BUNDLE
	amq_cas_i8 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i8 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u8 %r6, %r2, 0
	amr_cas_i8 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u8 %r6, %r2, 0
	amz_cas_i8 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u8 %r6, %r2, 0

	! BUNDLE
	amz_cas_i8 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i8 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u8 %r6, %r2, 0
	amz_cas_i8 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u8 %r6, %r2, 0
	fence_sc
	amx_cas_i8 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u8 %r6, %r2, 0
	fence_sc
	amx_cas_i8 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u8 %r6, %r2, 0
	fence_sc
	amx_cas_i8 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i8 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u8 %r4, %r2, 0
	fence_sc
	amx_cas_i8 %r3, %r1, %r4, %r3

	! BUNDLE
	cmp_eq_i32 %r1, %r3, %r4
	nul_eq_i32 %r3, %r4, 1, 0
	st_i8 %r3, %r2, 0

	! BUNDLE
	ld_u32 %r2, %sp, 12
	add_i32 %r1, %r2, %r1
	st_i32 %r1, %sp, 12

	! BUNDLE
	ld_u32 %r1, %sp, 12
	retf 16
	nop 0

.Lfunc_end35:
	.size	test_atomic_compare_exchange_i8, .Lfunc_end35-test_atomic_compare_exchange_i8
	.size	.Ltest_atomic_compare_exchange_i8$local, .Lfunc_end35-test_atomic_compare_exchange_i8
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
	alloc_sp 7, 16
	st_i32 %gz, %sp, 12
	ld_u16 %r6, %r2, 0

	! BUNDLE
	amx_cas_i16 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i16 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u16 %r6, %r2, 0
	amq_cas_i16 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u16 %r6, %r2, 0
	amq_cas_i16 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u16 %r6, %r2, 0

	! BUNDLE
	amq_cas_i16 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i16 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u16 %r6, %r2, 0
	amq_cas_i16 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u16 %r6, %r2, 0
	amq_cas_i16 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u16 %r6, %r2, 0

	! BUNDLE
	amq_cas_i16 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i16 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u16 %r6, %r2, 0
	amr_cas_i16 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u16 %r6, %r2, 0
	amz_cas_i16 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u16 %r6, %r2, 0

	! BUNDLE
	amz_cas_i16 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i16 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u16 %r6, %r2, 0
	amz_cas_i16 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u16 %r6, %r2, 0
	fence_sc
	amx_cas_i16 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u16 %r6, %r2, 0
	fence_sc
	amx_cas_i16 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u16 %r6, %r2, 0
	fence_sc
	amx_cas_i16 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i16 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u16 %r4, %r2, 0
	fence_sc
	amx_cas_i16 %r3, %r1, %r4, %r3

	! BUNDLE
	cmp_eq_i32 %r1, %r3, %r4
	nul_eq_i32 %r3, %r4, 1, 0
	st_i16 %r3, %r2, 0

	! BUNDLE
	ld_u32 %r2, %sp, 12
	add_i32 %r1, %r2, %r1
	st_i32 %r1, %sp, 12

	! BUNDLE
	ld_u32 %r1, %sp, 12
	retf 16
	nop 0

.Lfunc_end36:
	.size	test_atomic_compare_exchange_i16, .Lfunc_end36-test_atomic_compare_exchange_i16
	.size	.Ltest_atomic_compare_exchange_i16$local, .Lfunc_end36-test_atomic_compare_exchange_i16
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
	alloc_sp 7, 16
	st_i32 %gz, %sp, 12
	ld_u32 %r6, %r2, 0

	! BUNDLE
	amx_cas_i32 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i32 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u32 %r6, %r2, 0
	amq_cas_i32 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u32 %r6, %r2, 0
	amq_cas_i32 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u32 %r6, %r2, 0

	! BUNDLE
	amq_cas_i32 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i32 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u32 %r6, %r2, 0
	amq_cas_i32 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u32 %r6, %r2, 0
	amq_cas_i32 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u32 %r6, %r2, 0

	! BUNDLE
	amq_cas_i32 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i32 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u32 %r6, %r2, 0
	amr_cas_i32 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u32 %r6, %r2, 0
	amz_cas_i32 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u32 %r6, %r2, 0

	! BUNDLE
	amz_cas_i32 %r5, %r1, %r6, %r3
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0

	! BUNDLE
	st_i32 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u32 %r6, %r2, 0
	amz_cas_i32 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u32 %r6, %r2, 0
	fence_sc
	amx_cas_i32 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u32 %r6, %r2, 0
	fence_sc
	amx_cas_i32 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u32 %r6, %r2, 0
	fence_sc
	amx_cas_i32 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i32 %r4, %r5, %r6
	nul_eq_i32 %r5, %r6, 1, 0
	st_i32 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u32 %r4, %r2, 0
	fence_sc
	amx_cas_i32 %r3, %r1, %r4, %r3

	! BUNDLE
	cmp_eq_i32 %r1, %r3, %r4
	nul_eq_i32 %r3, %r4, 1, 0
	st_i32 %r3, %r2, 0

	! BUNDLE
	ld_u32 %r2, %sp, 12
	add_i32 %r1, %r2, %r1
	st_i32 %r1, %sp, 12

	! BUNDLE
	ld_u32 %r1, %sp, 12
	retf 16
	nop 0

.Lfunc_end37:
	.size	test_atomic_compare_exchange_i32, .Lfunc_end37-test_atomic_compare_exchange_i32
	.size	.Ltest_atomic_compare_exchange_i32$local, .Lfunc_end37-test_atomic_compare_exchange_i32
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
	alloc_sp 7, 16
	st_i32 %gz, %sp, 12
	ld_u64 %r6, %r2, 0

	! BUNDLE
	amx_cas_i64 %r5, %r1, %r6, %r3
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0

	! BUNDLE
	st_i64 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u64 %r6, %r2, 0
	amq_cas_i64 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u64 %r6, %r2, 0
	amq_cas_i64 %r5, %r1, %r6, %r3
	cmp_eq_i64 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u64 %r6, %r2, 0

	! BUNDLE
	amq_cas_i64 %r5, %r1, %r6, %r3
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0

	! BUNDLE
	st_i64 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u64 %r6, %r2, 0
	amq_cas_i64 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u64 %r6, %r2, 0
	amq_cas_i64 %r5, %r1, %r6, %r3
	cmp_eq_i64 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u64 %r6, %r2, 0

	! BUNDLE
	amq_cas_i64 %r5, %r1, %r6, %r3
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0

	! BUNDLE
	st_i64 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u64 %r6, %r2, 0
	amr_cas_i64 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u64 %r6, %r2, 0
	amz_cas_i64 %r5, %r1, %r6, %r3
	cmp_eq_i64 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_u64 %r6, %r2, 0

	! BUNDLE
	amz_cas_i64 %r5, %r1, %r6, %r3
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0

	! BUNDLE
	st_i64 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_u64 %r6, %r2, 0
	amz_cas_i64 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u64 %r6, %r2, 0
	fence_sc
	amx_cas_i64 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u64 %r6, %r2, 0
	fence_sc
	amx_cas_i64 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u64 %r6, %r2, 0
	fence_sc
	amx_cas_i64 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i64 %r4, %r5, %r6
	nul_eq_i64 %r5, %r6, 1, 0
	st_i64 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_u64 %r4, %r2, 0
	fence_sc
	amx_cas_i64 %r3, %r1, %r4, %r3

	! BUNDLE
	cmp_eq_i64 %r1, %r3, %r4
	nul_eq_i64 %r3, %r4, 1, 0
	st_i64 %r3, %r2, 0

	! BUNDLE
	ld_u32 %r2, %sp, 12
	add_i32 %r1, %r2, %r1
	st_i32 %r1, %sp, 12

	! BUNDLE
	ld_u32 %r1, %sp, 12
	retf 16
	nop 0

.Lfunc_end38:
	.size	test_atomic_compare_exchange_i64, .Lfunc_end38-test_atomic_compare_exchange_i64
	.size	.Ltest_atomic_compare_exchange_i64$local, .Lfunc_end38-test_atomic_compare_exchange_i64
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
	alloc_sp 7, 16
	st_i32 %gz, %sp, 12
	ld_i128 %r6, %r2, 0

	! BUNDLE
	amx_cas_i128 %r5, %r1, %r6, %r3
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0

	! BUNDLE
	st_i128 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_i128 %r6, %r2, 0
	amq_cas_i128 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_i128 %r6, %r2, 0
	amq_cas_i128 %r5, %r1, %r6, %r3
	cmp_eq_i128 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_i128 %r6, %r2, 0

	! BUNDLE
	amq_cas_i128 %r5, %r1, %r6, %r3
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0

	! BUNDLE
	st_i128 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_i128 %r6, %r2, 0
	amq_cas_i128 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_i128 %r6, %r2, 0
	amq_cas_i128 %r5, %r1, %r6, %r3
	cmp_eq_i128 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_i128 %r6, %r2, 0

	! BUNDLE
	amq_cas_i128 %r5, %r1, %r6, %r3
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0

	! BUNDLE
	st_i128 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_i128 %r6, %r2, 0
	amr_cas_i128 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_i128 %r6, %r2, 0
	amz_cas_i128 %r5, %r1, %r6, %r3
	cmp_eq_i128 %r4, %r5, %r6

	! BUNDLE
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0
	ld_u32 %r5, %sp, 12

	! BUNDLE
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12
	ld_i128 %r6, %r2, 0

	! BUNDLE
	amz_cas_i128 %r5, %r1, %r6, %r3
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0

	! BUNDLE
	st_i128 %r5, %r2, 0
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4

	! BUNDLE
	st_i32 %r4, %sp, 12
	ld_i128 %r6, %r2, 0
	amz_cas_i128 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_i128 %r6, %r2, 0
	fence_sc
	amx_cas_i128 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_i128 %r6, %r2, 0
	fence_sc
	amx_cas_i128 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_i128 %r6, %r2, 0
	fence_sc
	amx_cas_i128 %r5, %r1, %r6, %r3

	! BUNDLE
	cmp_eq_i128 %r4, %r5, %r6
	nul_eq_i128 %r5, %r6, 1, 0
	st_i128 %r5, %r2, 0

	! BUNDLE
	ld_u32 %r5, %sp, 12
	add_i32 %r4, %r5, %r4
	st_i32 %r4, %sp, 12

	! BUNDLE
	ld_i128 %r4, %r2, 0
	fence_sc
	amx_cas_i128 %r3, %r1, %r4, %r3

	! BUNDLE
	cmp_eq_i128 %r1, %r3, %r4
	nul_eq_i128 %r3, %r4, 1, 0
	st_i128 %r3, %r2, 0

	! BUNDLE
	ld_u32 %r2, %sp, 12
	add_i32 %r1, %r2, %r1
	st_i32 %r1, %sp, 12

	! BUNDLE
	ld_u32 %r1, %sp, 12
	retf 16
	nop 0

.Lfunc_end39:
	.size	test_atomic_compare_exchange_i128, .Lfunc_end39-test_atomic_compare_exchange_i128
	.size	.Ltest_atomic_compare_exchange_i128$local, .Lfunc_end39-test_atomic_compare_exchange_i128
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
	alloc 3
	amx_swap_u8 %r1, %r2, %r1
	amq_swap_u8 %r1, %r2, %r1

	! BUNDLE
	amq_swap_u8 %r1, %r2, %r1
	amr_swap_u8 %r1, %r2, %r1
	amz_swap_u8 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_swap_u8 %r1, %r2, %r1
	slsra_imm_i64 %r1, %r1, 56, 56

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end40:
	.size	test_atomic_exchange_i8, .Lfunc_end40-test_atomic_exchange_i8
	.size	.Ltest_atomic_exchange_i8$local, .Lfunc_end40-test_atomic_exchange_i8
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
	alloc 3
	amx_swap_u16 %r1, %r2, %r1
	amq_swap_u16 %r1, %r2, %r1

	! BUNDLE
	amq_swap_u16 %r1, %r2, %r1
	amr_swap_u16 %r1, %r2, %r1
	amz_swap_u16 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_swap_u16 %r1, %r2, %r1
	slsra_imm_i64 %r1, %r1, 48, 48

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end41:
	.size	test_atomic_exchange_i16, .Lfunc_end41-test_atomic_exchange_i16
	.size	.Ltest_atomic_exchange_i16$local, .Lfunc_end41-test_atomic_exchange_i16
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
	alloc 3
	amx_swap_u32 %r1, %r2, %r1
	amq_swap_u32 %r1, %r2, %r1

	! BUNDLE
	amq_swap_u32 %r1, %r2, %r1
	amr_swap_u32 %r1, %r2, %r1
	amz_swap_u32 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_swap_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end42:
	.size	test_atomic_exchange_i32, .Lfunc_end42-test_atomic_exchange_i32
	.size	.Ltest_atomic_exchange_i32$local, .Lfunc_end42-test_atomic_exchange_i32
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
	alloc 3
	amx_swap_u64 %r1, %r2, %r1
	amq_swap_u64 %r1, %r2, %r1

	! BUNDLE
	amq_swap_u64 %r1, %r2, %r1
	amr_swap_u64 %r1, %r2, %r1
	amz_swap_u64 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_swap_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end43:
	.size	test_atomic_exchange_i64, .Lfunc_end43-test_atomic_exchange_i64
	.size	.Ltest_atomic_exchange_i64$local, .Lfunc_end43-test_atomic_exchange_i64
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
	alloc 3
	amx_swap_u128 %r1, %r2, %r1
	amq_swap_u128 %r1, %r2, %r1

	! BUNDLE
	amq_swap_u128 %r1, %r2, %r1
	amr_swap_u128 %r1, %r2, %r1
	amz_swap_u128 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_swap_u128 %r1, %r2, %r1
	retf 0

.Lfunc_end44:
	.size	test_atomic_exchange_i128, .Lfunc_end44-test_atomic_exchange_i128
	.size	.Ltest_atomic_exchange_i128$local, .Lfunc_end44-test_atomic_exchange_i128
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
	alloc 1
	fence_a
	retf 0

.Lfunc_end45:
	.size	test_fence_acquire, .Lfunc_end45-test_fence_acquire
	.size	.Ltest_fence_acquire$local, .Lfunc_end45-test_fence_acquire
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
	alloc 1
	fence_r
	retf 0

.Lfunc_end46:
	.size	test_fence_release, .Lfunc_end46-test_fence_release
	.size	.Ltest_fence_release$local, .Lfunc_end46-test_fence_release
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
	alloc 1
	fence_ar
	retf 0

.Lfunc_end47:
	.size	test_fence_acq_rel, .Lfunc_end47-test_fence_acq_rel
	.size	.Ltest_fence_acq_rel$local, .Lfunc_end47-test_fence_acq_rel
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
	alloc 1
	fence_sc
	retf 0

.Lfunc_end48:
	.size	test_fence_seq_cst, .Lfunc_end48-test_fence_seq_cst
	.size	.Ltest_fence_seq_cst$local, .Lfunc_end48-test_fence_seq_cst
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
	alloc 1
	retf 0
	nop 0

.Lfunc_end49:
	.size	test_compiler_fence, .Lfunc_end49-test_compiler_fence
	.size	.Ltest_compiler_fence$local, .Lfunc_end49-test_compiler_fence
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
	alloc 5
	amx_ld_i8 %r1, %r1
	amq_ld_i8 %r2, %r2

	! BUNDLE
	amq_ld_i8 %r3, %r3
	fence_sc
	amx_ld_i8 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end50:
	.size	test_atomic_load_u8_u8, .Lfunc_end50-test_atomic_load_u8_u8
	.size	.Ltest_atomic_load_u8_u8$local, .Lfunc_end50-test_atomic_load_u8_u8
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
	alloc 5
	amx_ld_i8 %r1, %r1
	amq_ld_i8 %r2, %r2

	! BUNDLE
	amq_ld_i8 %r3, %r3
	fence_sc
	amx_ld_i8 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end51:
	.size	test_atomic_load_u8_u16, .Lfunc_end51-test_atomic_load_u8_u16
	.size	.Ltest_atomic_load_u8_u16$local, .Lfunc_end51-test_atomic_load_u8_u16
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
	alloc 5
	amx_ld_i8 %r1, %r1
	amq_ld_i8 %r2, %r2

	! BUNDLE
	amq_ld_i8 %r3, %r3
	fence_sc
	amx_ld_i8 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end52:
	.size	test_atomic_load_u8_u32, .Lfunc_end52-test_atomic_load_u8_u32
	.size	.Ltest_atomic_load_u8_u32$local, .Lfunc_end52-test_atomic_load_u8_u32
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
	alloc 5
	amx_ld_i8 %r1, %r1
	amq_ld_i8 %r2, %r2

	! BUNDLE
	amq_ld_i8 %r3, %r3
	fence_sc
	amx_ld_i8 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	srl_imm_u32 %r1, %r1, 0
	fence_a
	retf 0

.Lfunc_end53:
	.size	test_atomic_load_u8_u64, .Lfunc_end53-test_atomic_load_u8_u64
	.size	.Ltest_atomic_load_u8_u64$local, .Lfunc_end53-test_atomic_load_u8_u64
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
	alloc 5
	amx_ld_i16 %r1, %r1
	amq_ld_i16 %r2, %r2

	! BUNDLE
	amq_ld_i16 %r3, %r3
	fence_sc
	amx_ld_i16 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end54:
	.size	test_atomic_load_u16_u16, .Lfunc_end54-test_atomic_load_u16_u16
	.size	.Ltest_atomic_load_u16_u16$local, .Lfunc_end54-test_atomic_load_u16_u16
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
	alloc 5
	amx_ld_i16 %r1, %r1
	amq_ld_i16 %r2, %r2

	! BUNDLE
	amq_ld_i16 %r3, %r3
	fence_sc
	amx_ld_i16 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end55:
	.size	test_atomic_load_u16_u32, .Lfunc_end55-test_atomic_load_u16_u32
	.size	.Ltest_atomic_load_u16_u32$local, .Lfunc_end55-test_atomic_load_u16_u32
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
	alloc 5
	amx_ld_i16 %r1, %r1
	amq_ld_i16 %r2, %r2

	! BUNDLE
	amq_ld_i16 %r3, %r3
	fence_sc
	amx_ld_i16 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	srl_imm_u32 %r1, %r1, 0
	fence_a
	retf 0

.Lfunc_end56:
	.size	test_atomic_load_u16_u64, .Lfunc_end56-test_atomic_load_u16_u64
	.size	.Ltest_atomic_load_u16_u64$local, .Lfunc_end56-test_atomic_load_u16_u64
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
	alloc 5
	amx_ld_i32 %r1, %r1
	amq_ld_i32 %r2, %r2

	! BUNDLE
	amq_ld_i32 %r3, %r3
	fence_sc
	amx_ld_i32 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end57:
	.size	test_atomic_load_u32_u32, .Lfunc_end57-test_atomic_load_u32_u32
	.size	.Ltest_atomic_load_u32_u32$local, .Lfunc_end57-test_atomic_load_u32_u32
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
	alloc 5
	amx_ld_i32 %r1, %r1
	amq_ld_i32 %r2, %r2

	! BUNDLE
	amq_ld_i32 %r3, %r3
	fence_sc
	amx_ld_i32 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	srl_imm_u32 %r1, %r1, 0
	fence_a
	retf 0

.Lfunc_end58:
	.size	test_atomic_load_u32_u64, .Lfunc_end58-test_atomic_load_u32_u64
	.size	.Ltest_atomic_load_u32_u64$local, .Lfunc_end58-test_atomic_load_u32_u64
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
	alloc 5
	amx_ld_i64 %r1, %r1
	amq_ld_i64 %r2, %r2

	! BUNDLE
	amq_ld_i64 %r3, %r3
	fence_sc
	amx_ld_i64 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end59:
	.size	test_atomic_load_u64_u64, .Lfunc_end59-test_atomic_load_u64_u64
	.size	.Ltest_atomic_load_u64_u64$local, .Lfunc_end59-test_atomic_load_u64_u64
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
	alloc 5
	amx_ld_i128 %r1, %r1
	amq_ld_i128 %r2, %r2

	! BUNDLE
	amq_ld_i128 %r3, %r3
	fence_sc
	amx_ld_i128 %r4, %r4

	! BUNDLE
	xor %r1, %r2, %r1
	and %r1, %r1, %r3
	or %r1, %r1, %r4

	! BUNDLE
	fence_a
	retf 0
	nop 0

.Lfunc_end60:
	.size	test_atomic_load_u128_u128, .Lfunc_end60-test_atomic_load_u128_u128
	.size	.Ltest_atomic_load_u128_u128$local, .Lfunc_end60-test_atomic_load_u128_u128
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
	alloc 3
	amx_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u128 %r1, %r2, %r1
	amx_ld_and_u128 %r1, %r2, %r1
	amx_ld_or_u128 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u128 %r1, %r2, %r1
	amx_ld_min_i128 %r1, %r2, %r1
	amx_ld_max_i128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1
	amq_ld_add_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u128 %r1, %r2, %r1
	amq_ld_or_u128 %r1, %r2, %r1
	amq_ld_xor_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_i128 %r1, %r2, %r1
	amq_ld_max_i128 %r1, %r2, %r1
	amq_ld_add_u128 %r1, %r2, %r1

	! BUNDLE
	sub_i128 %r1, %gz, %r1
	amq_ld_add_u128 %r1, %r2, %r1
	amq_ld_and_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u128 %r1, %r2, %r1
	amq_ld_xor_u128 %r1, %r2, %r1
	amq_ld_min_i128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_i128 %r1, %r2, %r1
	amr_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u128 %r1, %r2, %r1
	amr_ld_and_u128 %r1, %r2, %r1
	amr_ld_or_u128 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u128 %r1, %r2, %r1
	amr_ld_min_i128 %r1, %r2, %r1
	amr_ld_max_i128 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1
	amz_ld_add_u128 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u128 %r1, %r2, %r1
	amz_ld_or_u128 %r1, %r2, %r1
	amz_ld_xor_u128 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_i128 %r1, %r2, %r1
	amz_ld_max_i128 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u128 %r1, %r2, %r1
	fence_sc
	sub_i128 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u128 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u128 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u128 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u128 %r1, %r2, %r1
	fence_sc
	amx_ld_min_i128 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_i128 %r1, %r2, %r1
	retf 0

.Lfunc_end61:
	.size	test_atomic_load_op_i128, .Lfunc_end61-test_atomic_load_op_i128
	.size	.Ltest_atomic_load_op_i128$local, .Lfunc_end61-test_atomic_load_op_i128
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
	alloc 3
	amx_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u128 %r1, %r2, %r1
	amx_ld_and_u128 %r1, %r2, %r1
	amx_ld_or_u128 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u128 %r1, %r2, %r1
	amx_ld_min_u128 %r1, %r2, %r1
	amx_ld_max_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1
	amq_ld_add_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u128 %r1, %r2, %r1
	amq_ld_or_u128 %r1, %r2, %r1
	amq_ld_xor_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_u128 %r1, %r2, %r1
	amq_ld_max_u128 %r1, %r2, %r1
	amq_ld_add_u128 %r1, %r2, %r1

	! BUNDLE
	sub_i128 %r1, %gz, %r1
	amq_ld_add_u128 %r1, %r2, %r1
	amq_ld_and_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u128 %r1, %r2, %r1
	amq_ld_xor_u128 %r1, %r2, %r1
	amq_ld_min_u128 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_u128 %r1, %r2, %r1
	amr_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u128 %r1, %r2, %r1
	amr_ld_and_u128 %r1, %r2, %r1
	amr_ld_or_u128 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u128 %r1, %r2, %r1
	amr_ld_min_u128 %r1, %r2, %r1
	amr_ld_max_u128 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u128 %r1, %r2, %r1
	sub_i128 %r1, %gz, %r1
	amz_ld_add_u128 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u128 %r1, %r2, %r1
	amz_ld_or_u128 %r1, %r2, %r1
	amz_ld_xor_u128 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_u128 %r1, %r2, %r1
	amz_ld_max_u128 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u128 %r1, %r2, %r1
	fence_sc
	sub_i128 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u128 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u128 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u128 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u128 %r1, %r2, %r1
	fence_sc
	amx_ld_min_u128 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_u128 %r1, %r2, %r1
	retf 0

.Lfunc_end62:
	.size	test_atomic_load_op_u128, .Lfunc_end62-test_atomic_load_op_u128
	.size	.Ltest_atomic_load_op_u128$local, .Lfunc_end62-test_atomic_load_op_u128
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
	alloc 3
	amx_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u16 %r1, %r2, %r1
	amx_ld_and_u16 %r1, %r2, %r1
	amx_ld_or_u16 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u16 %r1, %r2, %r1
	amx_ld_min_i16 %r1, %r2, %r1
	amx_ld_max_i16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u16 %r1, %r2, %r1
	amq_ld_or_u16 %r1, %r2, %r1
	amq_ld_xor_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_i16 %r1, %r2, %r1
	amq_ld_max_i16 %r1, %r2, %r1
	amq_ld_add_u16 %r1, %r2, %r1

	! BUNDLE
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u16 %r1, %r2, %r1
	amq_ld_and_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u16 %r1, %r2, %r1
	amq_ld_xor_u16 %r1, %r2, %r1
	amq_ld_min_i16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_i16 %r1, %r2, %r1
	amr_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u16 %r1, %r2, %r1
	amr_ld_and_u16 %r1, %r2, %r1
	amr_ld_or_u16 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u16 %r1, %r2, %r1
	amr_ld_min_i16 %r1, %r2, %r1
	amr_ld_max_i16 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amz_ld_add_u16 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u16 %r1, %r2, %r1
	amz_ld_or_u16 %r1, %r2, %r1
	amz_ld_xor_u16 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_i16 %r1, %r2, %r1
	amz_ld_max_i16 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u16 %r1, %r2, %r1
	fence_sc
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u16 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u16 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u16 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u16 %r1, %r2, %r1
	fence_sc
	amx_ld_min_i16 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_i16 %r1, %r2, %r1
	slsra_imm_i64 %r1, %r1, 48, 48

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end63:
	.size	test_atomic_load_op_i16, .Lfunc_end63-test_atomic_load_op_i16
	.size	.Ltest_atomic_load_op_i16$local, .Lfunc_end63-test_atomic_load_op_i16
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
	alloc 3
	amx_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u16 %r1, %r2, %r1
	amx_ld_and_u16 %r1, %r2, %r1
	amx_ld_or_u16 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u16 %r1, %r2, %r1
	amx_ld_min_u16 %r1, %r2, %r1
	amx_ld_max_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u16 %r1, %r2, %r1
	amq_ld_or_u16 %r1, %r2, %r1
	amq_ld_xor_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_u16 %r1, %r2, %r1
	amq_ld_max_u16 %r1, %r2, %r1
	amq_ld_add_u16 %r1, %r2, %r1

	! BUNDLE
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u16 %r1, %r2, %r1
	amq_ld_and_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u16 %r1, %r2, %r1
	amq_ld_xor_u16 %r1, %r2, %r1
	amq_ld_min_u16 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_u16 %r1, %r2, %r1
	amr_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u16 %r1, %r2, %r1
	amr_ld_and_u16 %r1, %r2, %r1
	amr_ld_or_u16 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u16 %r1, %r2, %r1
	amr_ld_min_u16 %r1, %r2, %r1
	amr_ld_max_u16 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u16 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amz_ld_add_u16 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u16 %r1, %r2, %r1
	amz_ld_or_u16 %r1, %r2, %r1
	amz_ld_xor_u16 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_u16 %r1, %r2, %r1
	amz_ld_max_u16 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u16 %r1, %r2, %r1
	fence_sc
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u16 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u16 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u16 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u16 %r1, %r2, %r1
	fence_sc
	amx_ld_min_u16 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_u16 %r1, %r2, %r1
	retf 0

.Lfunc_end64:
	.size	test_atomic_load_op_u16, .Lfunc_end64-test_atomic_load_op_u16
	.size	.Ltest_atomic_load_op_u16$local, .Lfunc_end64-test_atomic_load_op_u16
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
	alloc 3
	amx_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u32 %r1, %r2, %r1
	amx_ld_and_u32 %r1, %r2, %r1
	amx_ld_or_u32 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u32 %r1, %r2, %r1
	amx_ld_min_i32 %r1, %r2, %r1
	amx_ld_max_i32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u32 %r1, %r2, %r1
	amq_ld_or_u32 %r1, %r2, %r1
	amq_ld_xor_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_i32 %r1, %r2, %r1
	amq_ld_max_i32 %r1, %r2, %r1
	amq_ld_add_u32 %r1, %r2, %r1

	! BUNDLE
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u32 %r1, %r2, %r1
	amq_ld_and_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u32 %r1, %r2, %r1
	amq_ld_xor_u32 %r1, %r2, %r1
	amq_ld_min_i32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_i32 %r1, %r2, %r1
	amr_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u32 %r1, %r2, %r1
	amr_ld_and_u32 %r1, %r2, %r1
	amr_ld_or_u32 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u32 %r1, %r2, %r1
	amr_ld_min_i32 %r1, %r2, %r1
	amr_ld_max_i32 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amz_ld_add_u32 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u32 %r1, %r2, %r1
	amz_ld_or_u32 %r1, %r2, %r1
	amz_ld_xor_u32 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_i32 %r1, %r2, %r1
	amz_ld_max_i32 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u32 %r1, %r2, %r1
	fence_sc
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u32 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u32 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u32 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u32 %r1, %r2, %r1
	fence_sc
	amx_ld_min_i32 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_i32 %r1, %r2, %r1
	retf 0

.Lfunc_end65:
	.size	test_atomic_load_op_i32, .Lfunc_end65-test_atomic_load_op_i32
	.size	.Ltest_atomic_load_op_i32$local, .Lfunc_end65-test_atomic_load_op_i32
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
	alloc 3
	amx_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u32 %r1, %r2, %r1
	amx_ld_and_u32 %r1, %r2, %r1
	amx_ld_or_u32 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u32 %r1, %r2, %r1
	amx_ld_min_u32 %r1, %r2, %r1
	amx_ld_max_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u32 %r1, %r2, %r1
	amq_ld_or_u32 %r1, %r2, %r1
	amq_ld_xor_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_u32 %r1, %r2, %r1
	amq_ld_max_u32 %r1, %r2, %r1
	amq_ld_add_u32 %r1, %r2, %r1

	! BUNDLE
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u32 %r1, %r2, %r1
	amq_ld_and_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u32 %r1, %r2, %r1
	amq_ld_xor_u32 %r1, %r2, %r1
	amq_ld_min_u32 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_u32 %r1, %r2, %r1
	amr_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u32 %r1, %r2, %r1
	amr_ld_and_u32 %r1, %r2, %r1
	amr_ld_or_u32 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u32 %r1, %r2, %r1
	amr_ld_min_u32 %r1, %r2, %r1
	amr_ld_max_u32 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u32 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amz_ld_add_u32 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u32 %r1, %r2, %r1
	amz_ld_or_u32 %r1, %r2, %r1
	amz_ld_xor_u32 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_u32 %r1, %r2, %r1
	amz_ld_max_u32 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u32 %r1, %r2, %r1
	fence_sc
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u32 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u32 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u32 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u32 %r1, %r2, %r1
	fence_sc
	amx_ld_min_u32 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end66:
	.size	test_atomic_load_op_u32, .Lfunc_end66-test_atomic_load_op_u32
	.size	.Ltest_atomic_load_op_u32$local, .Lfunc_end66-test_atomic_load_op_u32
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
	alloc 3
	amx_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u64 %r1, %r2, %r1
	amx_ld_and_u64 %r1, %r2, %r1
	amx_ld_or_u64 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u64 %r1, %r2, %r1
	amx_ld_min_i64 %r1, %r2, %r1
	amx_ld_max_i64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1
	amq_ld_add_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u64 %r1, %r2, %r1
	amq_ld_or_u64 %r1, %r2, %r1
	amq_ld_xor_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_i64 %r1, %r2, %r1
	amq_ld_max_i64 %r1, %r2, %r1
	amq_ld_add_u64 %r1, %r2, %r1

	! BUNDLE
	sub_i64 %r1, %gz, %r1
	amq_ld_add_u64 %r1, %r2, %r1
	amq_ld_and_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u64 %r1, %r2, %r1
	amq_ld_xor_u64 %r1, %r2, %r1
	amq_ld_min_i64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_i64 %r1, %r2, %r1
	amr_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u64 %r1, %r2, %r1
	amr_ld_and_u64 %r1, %r2, %r1
	amr_ld_or_u64 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u64 %r1, %r2, %r1
	amr_ld_min_i64 %r1, %r2, %r1
	amr_ld_max_i64 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1
	amz_ld_add_u64 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u64 %r1, %r2, %r1
	amz_ld_or_u64 %r1, %r2, %r1
	amz_ld_xor_u64 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_i64 %r1, %r2, %r1
	amz_ld_max_i64 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u64 %r1, %r2, %r1
	fence_sc
	sub_i64 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u64 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u64 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u64 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u64 %r1, %r2, %r1
	fence_sc
	amx_ld_min_i64 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end67:
	.size	test_atomic_load_op_i64, .Lfunc_end67-test_atomic_load_op_i64
	.size	.Ltest_atomic_load_op_i64$local, .Lfunc_end67-test_atomic_load_op_i64
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
	alloc 3
	amx_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u64 %r1, %r2, %r1
	amx_ld_and_u64 %r1, %r2, %r1
	amx_ld_or_u64 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u64 %r1, %r2, %r1
	amx_ld_min_u64 %r1, %r2, %r1
	amx_ld_max_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1
	amq_ld_add_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u64 %r1, %r2, %r1
	amq_ld_or_u64 %r1, %r2, %r1
	amq_ld_xor_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_u64 %r1, %r2, %r1
	amq_ld_max_u64 %r1, %r2, %r1
	amq_ld_add_u64 %r1, %r2, %r1

	! BUNDLE
	sub_i64 %r1, %gz, %r1
	amq_ld_add_u64 %r1, %r2, %r1
	amq_ld_and_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u64 %r1, %r2, %r1
	amq_ld_xor_u64 %r1, %r2, %r1
	amq_ld_min_u64 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_u64 %r1, %r2, %r1
	amr_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u64 %r1, %r2, %r1
	amr_ld_and_u64 %r1, %r2, %r1
	amr_ld_or_u64 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u64 %r1, %r2, %r1
	amr_ld_min_u64 %r1, %r2, %r1
	amr_ld_max_u64 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u64 %r1, %r2, %r1
	sub_i64 %r1, %gz, %r1
	amz_ld_add_u64 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u64 %r1, %r2, %r1
	amz_ld_or_u64 %r1, %r2, %r1
	amz_ld_xor_u64 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_u64 %r1, %r2, %r1
	amz_ld_max_u64 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u64 %r1, %r2, %r1
	fence_sc
	sub_i64 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u64 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u64 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u64 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u64 %r1, %r2, %r1
	fence_sc
	amx_ld_min_u64 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end68:
	.size	test_atomic_load_op_u64, .Lfunc_end68-test_atomic_load_op_u64
	.size	.Ltest_atomic_load_op_u64$local, .Lfunc_end68-test_atomic_load_op_u64
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
	alloc 3
	amx_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u8 %r1, %r2, %r1
	amx_ld_and_u8 %r1, %r2, %r1
	amx_ld_or_u8 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u8 %r1, %r2, %r1
	amx_ld_min_i8 %r1, %r2, %r1
	amx_ld_max_i8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u8 %r1, %r2, %r1
	amq_ld_or_u8 %r1, %r2, %r1
	amq_ld_xor_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_i8 %r1, %r2, %r1
	amq_ld_max_i8 %r1, %r2, %r1
	amq_ld_add_u8 %r1, %r2, %r1

	! BUNDLE
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u8 %r1, %r2, %r1
	amq_ld_and_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u8 %r1, %r2, %r1
	amq_ld_xor_u8 %r1, %r2, %r1
	amq_ld_min_i8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_i8 %r1, %r2, %r1
	amr_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u8 %r1, %r2, %r1
	amr_ld_and_u8 %r1, %r2, %r1
	amr_ld_or_u8 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u8 %r1, %r2, %r1
	amr_ld_min_i8 %r1, %r2, %r1
	amr_ld_max_i8 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amz_ld_add_u8 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u8 %r1, %r2, %r1
	amz_ld_or_u8 %r1, %r2, %r1
	amz_ld_xor_u8 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_i8 %r1, %r2, %r1
	amz_ld_max_i8 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u8 %r1, %r2, %r1
	fence_sc
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u8 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u8 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u8 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u8 %r1, %r2, %r1
	fence_sc
	amx_ld_min_i8 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_i8 %r1, %r2, %r1
	slsra_imm_i64 %r1, %r1, 56, 56

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end69:
	.size	test_atomic_load_op_i8, .Lfunc_end69-test_atomic_load_op_i8
	.size	.Ltest_atomic_load_op_i8$local, .Lfunc_end69-test_atomic_load_op_i8
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
	alloc 3
	amx_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u8 %r1, %r2, %r1
	amx_ld_and_u8 %r1, %r2, %r1
	amx_ld_or_u8 %r1, %r2, %r1

	! BUNDLE
	amx_ld_xor_u8 %r1, %r2, %r1
	amx_ld_min_u8 %r1, %r2, %r1
	amx_ld_max_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_and_u8 %r1, %r2, %r1
	amq_ld_or_u8 %r1, %r2, %r1
	amq_ld_xor_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_min_u8 %r1, %r2, %r1
	amq_ld_max_u8 %r1, %r2, %r1
	amq_ld_add_u8 %r1, %r2, %r1

	! BUNDLE
	sub_i32 %r1, %gz, %r1
	amq_ld_add_u8 %r1, %r2, %r1
	amq_ld_and_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_or_u8 %r1, %r2, %r1
	amq_ld_xor_u8 %r1, %r2, %r1
	amq_ld_min_u8 %r1, %r2, %r1

	! BUNDLE
	amq_ld_max_u8 %r1, %r2, %r1
	amr_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amr_ld_add_u8 %r1, %r2, %r1
	amr_ld_and_u8 %r1, %r2, %r1
	amr_ld_or_u8 %r1, %r2, %r1

	! BUNDLE
	amr_ld_xor_u8 %r1, %r2, %r1
	amr_ld_min_u8 %r1, %r2, %r1
	amr_ld_max_u8 %r1, %r2, %r1

	! BUNDLE
	amz_ld_add_u8 %r1, %r2, %r1
	sub_i32 %r1, %gz, %r1
	amz_ld_add_u8 %r1, %r2, %r1

	! BUNDLE
	amz_ld_and_u8 %r1, %r2, %r1
	amz_ld_or_u8 %r1, %r2, %r1
	amz_ld_xor_u8 %r1, %r2, %r1

	! BUNDLE
	amz_ld_min_u8 %r1, %r2, %r1
	amz_ld_max_u8 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_add_u8 %r1, %r2, %r1
	fence_sc
	sub_i32 %r1, %gz, %r1

	! BUNDLE
	amx_ld_add_u8 %r1, %r2, %r1
	fence_sc
	amx_ld_and_u8 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_or_u8 %r1, %r2, %r1
	fence_sc

	! BUNDLE
	amx_ld_xor_u8 %r1, %r2, %r1
	fence_sc
	amx_ld_min_u8 %r1, %r2, %r1

	! BUNDLE
	fence_sc
	amx_ld_max_u8 %r1, %r2, %r1
	retf 0

.Lfunc_end70:
	.size	test_atomic_load_op_u8, .Lfunc_end70-test_atomic_load_op_u8
	.size	.Ltest_atomic_load_op_u8$local, .Lfunc_end70-test_atomic_load_op_u8
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
	alloc 5
	amx_st_i8 %r4, %r1
	amr_st_i8 %r4, %r2

	! BUNDLE
	fence_sc
	amx_st_i8 %r4, %r3
	retf 0

.Lfunc_end71:
	.size	test_atomic_store_i8, .Lfunc_end71-test_atomic_store_i8
	.size	.Ltest_atomic_store_i8$local, .Lfunc_end71-test_atomic_store_i8
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
	alloc 5
	amx_st_i16 %r4, %r1
	amr_st_i16 %r4, %r2

	! BUNDLE
	fence_sc
	amx_st_i16 %r4, %r3
	retf 0

.Lfunc_end72:
	.size	test_atomic_store_i16, .Lfunc_end72-test_atomic_store_i16
	.size	.Ltest_atomic_store_i16$local, .Lfunc_end72-test_atomic_store_i16
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
	alloc 5
	amx_st_i32 %r4, %r1
	amr_st_i32 %r4, %r2

	! BUNDLE
	fence_sc
	amx_st_i32 %r4, %r3
	retf 0

.Lfunc_end73:
	.size	test_atomic_store_i32, .Lfunc_end73-test_atomic_store_i32
	.size	.Ltest_atomic_store_i32$local, .Lfunc_end73-test_atomic_store_i32
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
	alloc 5
	amx_st_i64 %r4, %r1
	amr_st_i64 %r4, %r2

	! BUNDLE
	fence_sc
	amx_st_i64 %r4, %r3
	retf 0

.Lfunc_end74:
	.size	test_atomic_store_i64, .Lfunc_end74-test_atomic_store_i64
	.size	.Ltest_atomic_store_i64$local, .Lfunc_end74-test_atomic_store_i64
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
	alloc 5
	amx_st_i128 %r4, %r1
	amr_st_i128 %r4, %r2

	! BUNDLE
	fence_sc
	amx_st_i128 %r4, %r3
	retf 0

.Lfunc_end75:
	.size	test_atomic_store_i128, .Lfunc_end75-test_atomic_store_i128
	.size	.Ltest_atomic_store_i128$local, .Lfunc_end75-test_atomic_store_i128
                                        ! -- End function
	.globl	atomic_store_op_i8              ! -- Begin function atomic_store_op_i8
	.p2align	4
	.type	atomic_store_op_i8,@function
atomic_store_op_i8:                     ! @atomic_store_op_i8
.Latomic_store_op_i8$local:
	.type	.Latomic_store_op_i8$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 5
	amx_ld_add_u8 %r3, %r2, %r1
	add_imm_i64 %r3, %r2, 1

	! BUNDLE
	sub_i32 %r4, %gz, %r1
	amx_ld_add_u8 %r3, %r3, %r4
	add_imm_i64 %r3, %r2, 2

	! BUNDLE
	amx_ld_and_u8 %r3, %r3, %r1
	add_imm_i64 %r3, %r2, 3
	amx_ld_or_u8 %r3, %r3, %r1

	! BUNDLE
	add_imm_i64 %r3, %r2, 4
	amx_ld_xor_u8 %r3, %r3, %r1
	add_imm_i64 %r3, %r2, 5

	! BUNDLE
	amx_ld_min_i8 %r3, %r3, %r1
	add_imm_i64 %r3, %r2, 6
	amx_ld_max_i8 %r3, %r3, %r1

	! BUNDLE
	add_imm_i64 %r3, %r2, 8
	amr_ld_add_u8 %r3, %r3, %r1
	add_imm_i64 %r3, %r2, 9

	! BUNDLE
	bit_flip_imm %r4, %r1, 1
	sub_i32 %r4, %gz, %r4
	amr_ld_add_u8 %r3, %r3, %r4

	! BUNDLE
	add_imm_i64 %r3, %r2, 10
	amr_ld_and_u8 %r3, %r3, %r1
	add_imm_i64 %r3, %r2, 11

	! BUNDLE
	amr_ld_or_u8 %r3, %r3, %r1
	add_imm_i64 %r3, %r2, 12
	amr_ld_xor_u8 %r3, %r3, %r1

	! BUNDLE
	add_imm_i64 %r3, %r2, 13
	amr_ld_min_i8 %r3, %r3, %r1
	add_imm_i64 %r2, %r2, 14

	! BUNDLE
	amr_ld_max_i8 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end76:
	.size	atomic_store_op_i8, .Lfunc_end76-atomic_store_op_i8
	.size	.Latomic_store_op_i8$local, .Lfunc_end76-atomic_store_op_i8
                                        ! -- End function
	.globl	test_bit_clear_i32              ! -- Begin function test_bit_clear_i32
	.p2align	4
	.type	test_bit_clear_i32,@function
test_bit_clear_i32:                     ! @test_bit_clear_i32
.Ltest_bit_clear_i32$local:
	.type	.Ltest_bit_clear_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_clear %r1, %r1, %r2
	retf 0

.Lfunc_end77:
	.size	test_bit_clear_i32, .Lfunc_end77-test_bit_clear_i32
	.size	.Ltest_bit_clear_i32$local, .Lfunc_end77-test_bit_clear_i32
                                        ! -- End function
	.globl	test_bit_clear_imm_i32          ! -- Begin function test_bit_clear_imm_i32
	.p2align	4
	.type	test_bit_clear_imm_i32,@function
test_bit_clear_imm_i32:                 ! @test_bit_clear_imm_i32
.Ltest_bit_clear_imm_i32$local:
	.type	.Ltest_bit_clear_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_clear_imm %r1, %r1, 20
	retf 0

.Lfunc_end78:
	.size	test_bit_clear_imm_i32, .Lfunc_end78-test_bit_clear_imm_i32
	.size	.Ltest_bit_clear_imm_i32$local, .Lfunc_end78-test_bit_clear_imm_i32
                                        ! -- End function
	.globl	test_bit_clear_i64_i32          ! -- Begin function test_bit_clear_i64_i32
	.p2align	4
	.type	test_bit_clear_i64_i32,@function
test_bit_clear_i64_i32:                 ! @test_bit_clear_i64_i32
.Ltest_bit_clear_i64_i32$local:
	.type	.Ltest_bit_clear_i64_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_clear %r1, %r1, %r2
	retf 0

.Lfunc_end79:
	.size	test_bit_clear_i64_i32, .Lfunc_end79-test_bit_clear_i64_i32
	.size	.Ltest_bit_clear_i64_i32$local, .Lfunc_end79-test_bit_clear_i64_i32
                                        ! -- End function
	.globl	test_bit_clear_i64_i64          ! -- Begin function test_bit_clear_i64_i64
	.p2align	4
	.type	test_bit_clear_i64_i64,@function
test_bit_clear_i64_i64:                 ! @test_bit_clear_i64_i64
.Ltest_bit_clear_i64_i64$local:
	.type	.Ltest_bit_clear_i64_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_clear %r1, %r1, %r2
	retf 0

.Lfunc_end80:
	.size	test_bit_clear_i64_i64, .Lfunc_end80-test_bit_clear_i64_i64
	.size	.Ltest_bit_clear_i64_i64$local, .Lfunc_end80-test_bit_clear_i64_i64
                                        ! -- End function
	.globl	test_bit_clear_imm_i64          ! -- Begin function test_bit_clear_imm_i64
	.p2align	4
	.type	test_bit_clear_imm_i64,@function
test_bit_clear_imm_i64:                 ! @test_bit_clear_imm_i64
.Ltest_bit_clear_imm_i64$local:
	.type	.Ltest_bit_clear_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_clear_imm %r1, %r1, 30
	retf 0

.Lfunc_end81:
	.size	test_bit_clear_imm_i64, .Lfunc_end81-test_bit_clear_imm_i64
	.size	.Ltest_bit_clear_imm_i64$local, .Lfunc_end81-test_bit_clear_imm_i64
                                        ! -- End function
	.globl	test_bit_flip_i32               ! -- Begin function test_bit_flip_i32
	.p2align	4
	.type	test_bit_flip_i32,@function
test_bit_flip_i32:                      ! @test_bit_flip_i32
.Ltest_bit_flip_i32$local:
	.type	.Ltest_bit_flip_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_flip %r1, %r1, %r2
	retf 0

.Lfunc_end82:
	.size	test_bit_flip_i32, .Lfunc_end82-test_bit_flip_i32
	.size	.Ltest_bit_flip_i32$local, .Lfunc_end82-test_bit_flip_i32
                                        ! -- End function
	.globl	test_bit_flip_imm_i32           ! -- Begin function test_bit_flip_imm_i32
	.p2align	4
	.type	test_bit_flip_imm_i32,@function
test_bit_flip_imm_i32:                  ! @test_bit_flip_imm_i32
.Ltest_bit_flip_imm_i32$local:
	.type	.Ltest_bit_flip_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_flip_imm %r1, %r1, 20
	retf 0

.Lfunc_end83:
	.size	test_bit_flip_imm_i32, .Lfunc_end83-test_bit_flip_imm_i32
	.size	.Ltest_bit_flip_imm_i32$local, .Lfunc_end83-test_bit_flip_imm_i32
                                        ! -- End function
	.globl	test_bit_flip_i64_i32           ! -- Begin function test_bit_flip_i64_i32
	.p2align	4
	.type	test_bit_flip_i64_i32,@function
test_bit_flip_i64_i32:                  ! @test_bit_flip_i64_i32
.Ltest_bit_flip_i64_i32$local:
	.type	.Ltest_bit_flip_i64_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_flip %r1, %r1, %r2
	retf 0

.Lfunc_end84:
	.size	test_bit_flip_i64_i32, .Lfunc_end84-test_bit_flip_i64_i32
	.size	.Ltest_bit_flip_i64_i32$local, .Lfunc_end84-test_bit_flip_i64_i32
                                        ! -- End function
	.globl	test_bit_flip_i64_i64           ! -- Begin function test_bit_flip_i64_i64
	.p2align	4
	.type	test_bit_flip_i64_i64,@function
test_bit_flip_i64_i64:                  ! @test_bit_flip_i64_i64
.Ltest_bit_flip_i64_i64$local:
	.type	.Ltest_bit_flip_i64_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_flip %r1, %r1, %r2
	retf 0

.Lfunc_end85:
	.size	test_bit_flip_i64_i64, .Lfunc_end85-test_bit_flip_i64_i64
	.size	.Ltest_bit_flip_i64_i64$local, .Lfunc_end85-test_bit_flip_i64_i64
                                        ! -- End function
	.globl	test_bit_flip_imm_i64           ! -- Begin function test_bit_flip_imm_i64
	.p2align	4
	.type	test_bit_flip_imm_i64,@function
test_bit_flip_imm_i64:                  ! @test_bit_flip_imm_i64
.Ltest_bit_flip_imm_i64$local:
	.type	.Ltest_bit_flip_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_flip_imm %r1, %r1, 30
	retf 0

.Lfunc_end86:
	.size	test_bit_flip_imm_i64, .Lfunc_end86-test_bit_flip_imm_i64
	.size	.Ltest_bit_flip_imm_i64$local, .Lfunc_end86-test_bit_flip_imm_i64
                                        ! -- End function
	.globl	test_bit_flip_i128_i32          ! -- Begin function test_bit_flip_i128_i32
	.p2align	4
	.type	test_bit_flip_i128_i32,@function
test_bit_flip_i128_i32:                 ! @test_bit_flip_i128_i32
.Ltest_bit_flip_i128_i32$local:
	.type	.Ltest_bit_flip_i128_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_flip %r1, %r1, %r2
	retf 0

.Lfunc_end87:
	.size	test_bit_flip_i128_i32, .Lfunc_end87-test_bit_flip_i128_i32
	.size	.Ltest_bit_flip_i128_i32$local, .Lfunc_end87-test_bit_flip_i128_i32
                                        ! -- End function
	.globl	test_bit_flip_i128_i64          ! -- Begin function test_bit_flip_i128_i64
	.p2align	4
	.type	test_bit_flip_i128_i64,@function
test_bit_flip_i128_i64:                 ! @test_bit_flip_i128_i64
.Ltest_bit_flip_i128_i64$local:
	.type	.Ltest_bit_flip_i128_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_flip %r1, %r1, %r2
	retf 0

.Lfunc_end88:
	.size	test_bit_flip_i128_i64, .Lfunc_end88-test_bit_flip_i128_i64
	.size	.Ltest_bit_flip_i128_i64$local, .Lfunc_end88-test_bit_flip_i128_i64
                                        ! -- End function
	.globl	test_bit_flip_imm_i128          ! -- Begin function test_bit_flip_imm_i128
	.p2align	4
	.type	test_bit_flip_imm_i128,@function
test_bit_flip_imm_i128:                 ! @test_bit_flip_imm_i128
.Ltest_bit_flip_imm_i128$local:
	.type	.Ltest_bit_flip_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_flip_imm %r1, %r1, 40
	retf 0

.Lfunc_end89:
	.size	test_bit_flip_imm_i128, .Lfunc_end89-test_bit_flip_imm_i128
	.size	.Ltest_bit_flip_imm_i128$local, .Lfunc_end89-test_bit_flip_imm_i128
                                        ! -- End function
	.globl	test_bit_set_i32                ! -- Begin function test_bit_set_i32
	.p2align	4
	.type	test_bit_set_i32,@function
test_bit_set_i32:                       ! @test_bit_set_i32
.Ltest_bit_set_i32$local:
	.type	.Ltest_bit_set_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_set %r1, %r1, %r2
	retf 0

.Lfunc_end90:
	.size	test_bit_set_i32, .Lfunc_end90-test_bit_set_i32
	.size	.Ltest_bit_set_i32$local, .Lfunc_end90-test_bit_set_i32
                                        ! -- End function
	.globl	test_bit_set_imm_i32            ! -- Begin function test_bit_set_imm_i32
	.p2align	4
	.type	test_bit_set_imm_i32,@function
test_bit_set_imm_i32:                   ! @test_bit_set_imm_i32
.Ltest_bit_set_imm_i32$local:
	.type	.Ltest_bit_set_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_set_imm %r1, %r1, 20
	retf 0

.Lfunc_end91:
	.size	test_bit_set_imm_i32, .Lfunc_end91-test_bit_set_imm_i32
	.size	.Ltest_bit_set_imm_i32$local, .Lfunc_end91-test_bit_set_imm_i32
                                        ! -- End function
	.globl	test_bit_set_i64_i32            ! -- Begin function test_bit_set_i64_i32
	.p2align	4
	.type	test_bit_set_i64_i32,@function
test_bit_set_i64_i32:                   ! @test_bit_set_i64_i32
.Ltest_bit_set_i64_i32$local:
	.type	.Ltest_bit_set_i64_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_set %r1, %r1, %r2
	retf 0

.Lfunc_end92:
	.size	test_bit_set_i64_i32, .Lfunc_end92-test_bit_set_i64_i32
	.size	.Ltest_bit_set_i64_i32$local, .Lfunc_end92-test_bit_set_i64_i32
                                        ! -- End function
	.globl	test_bit_set_i64_i64            ! -- Begin function test_bit_set_i64_i64
	.p2align	4
	.type	test_bit_set_i64_i64,@function
test_bit_set_i64_i64:                   ! @test_bit_set_i64_i64
.Ltest_bit_set_i64_i64$local:
	.type	.Ltest_bit_set_i64_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_set %r1, %r1, %r2
	retf 0

.Lfunc_end93:
	.size	test_bit_set_i64_i64, .Lfunc_end93-test_bit_set_i64_i64
	.size	.Ltest_bit_set_i64_i64$local, .Lfunc_end93-test_bit_set_i64_i64
                                        ! -- End function
	.globl	test_bit_set_imm_i64            ! -- Begin function test_bit_set_imm_i64
	.p2align	4
	.type	test_bit_set_imm_i64,@function
test_bit_set_imm_i64:                   ! @test_bit_set_imm_i64
.Ltest_bit_set_imm_i64$local:
	.type	.Ltest_bit_set_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_set_imm %r1, %r1, 30
	retf 0

.Lfunc_end94:
	.size	test_bit_set_imm_i64, .Lfunc_end94-test_bit_set_imm_i64
	.size	.Ltest_bit_set_imm_i64$local, .Lfunc_end94-test_bit_set_imm_i64
                                        ! -- End function
	.globl	test_bit_set_i128_i32           ! -- Begin function test_bit_set_i128_i32
	.p2align	4
	.type	test_bit_set_i128_i32,@function
test_bit_set_i128_i32:                  ! @test_bit_set_i128_i32
.Ltest_bit_set_i128_i32$local:
	.type	.Ltest_bit_set_i128_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_set %r1, %r1, %r2
	retf 0

.Lfunc_end95:
	.size	test_bit_set_i128_i32, .Lfunc_end95-test_bit_set_i128_i32
	.size	.Ltest_bit_set_i128_i32$local, .Lfunc_end95-test_bit_set_i128_i32
                                        ! -- End function
	.globl	test_bit_set_i128_i64           ! -- Begin function test_bit_set_i128_i64
	.p2align	4
	.type	test_bit_set_i128_i64,@function
test_bit_set_i128_i64:                  ! @test_bit_set_i128_i64
.Ltest_bit_set_i128_i64$local:
	.type	.Ltest_bit_set_i128_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	bit_set %r1, %r1, %r2
	retf 0

.Lfunc_end96:
	.size	test_bit_set_i128_i64, .Lfunc_end96-test_bit_set_i128_i64
	.size	.Ltest_bit_set_i128_i64$local, .Lfunc_end96-test_bit_set_i128_i64
                                        ! -- End function
	.globl	test_bit_set_imm_i128           ! -- Begin function test_bit_set_imm_i128
	.p2align	4
	.type	test_bit_set_imm_i128,@function
test_bit_set_imm_i128:                  ! @test_bit_set_imm_i128
.Ltest_bit_set_imm_i128$local:
	.type	.Ltest_bit_set_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	bit_set_imm %r1, %r1, 40
	retf 0

.Lfunc_end97:
	.size	test_bit_set_imm_i128, .Lfunc_end97-test_bit_set_imm_i128
	.size	.Ltest_bit_set_imm_i128$local, .Lfunc_end97-test_bit_set_imm_i128
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
	alloc 2
	not %r1, %r1
	retf 0

.Lfunc_end98:
	.size	test_i128_inv, .Lfunc_end98-test_i128_inv
	.size	.Ltest_i128_inv$local, .Lfunc_end98-test_i128_inv
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
	alloc 3
	and %r1, %r2, %r1
	retf 0

.Lfunc_end99:
	.size	test_i128_and, .Lfunc_end99-test_i128_and
	.size	.Ltest_i128_and$local, .Lfunc_end99-test_i128_and
                                        ! -- End function
	.globl	test_i128_and_imm               ! -- Begin function test_i128_and_imm
	.p2align	4
	.type	test_i128_and_imm,@function
test_i128_and_imm:                      ! @test_i128_and_imm
.Ltest_i128_and_imm$local:
	.type	.Ltest_i128_and_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	and_imm %r1, %r1, 12345
	retf 0

.Lfunc_end100:
	.size	test_i128_and_imm, .Lfunc_end100-test_i128_and_imm
	.size	.Ltest_i128_and_imm$local, .Lfunc_end100-test_i128_and_imm
                                        ! -- End function
	.globl	test_i128_and_imm_ext           ! -- Begin function test_i128_and_imm_ext
	.p2align	4
	.type	test_i128_and_imm_ext,@function
test_i128_and_imm_ext:                  ! @test_i128_and_imm_ext
.Ltest_i128_and_imm_ext$local:
	.type	.Ltest_i128_and_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	and_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end101:
	.size	test_i128_and_imm_ext, .Lfunc_end101-test_i128_and_imm_ext
	.size	.Ltest_i128_and_imm_ext$local, .Lfunc_end101-test_i128_and_imm_ext
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
	alloc 3
	andn %r1, %r1, %r2
	retf 0

.Lfunc_end102:
	.size	test_i128_andn, .Lfunc_end102-test_i128_andn
	.size	.Ltest_i128_andn$local, .Lfunc_end102-test_i128_andn
                                        ! -- End function
	.globl	test_i128_andn_imm              ! -- Begin function test_i128_andn_imm
	.p2align	4
	.type	test_i128_andn_imm,@function
test_i128_andn_imm:                     ! @test_i128_andn_imm
.Ltest_i128_andn_imm$local:
	.type	.Ltest_i128_andn_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	andn_imm %r1, %r1, 12345
	retf 0

.Lfunc_end103:
	.size	test_i128_andn_imm, .Lfunc_end103-test_i128_andn_imm
	.size	.Ltest_i128_andn_imm$local, .Lfunc_end103-test_i128_andn_imm
                                        ! -- End function
	.globl	test_i128_andn_imm_ext          ! -- Begin function test_i128_andn_imm_ext
	.p2align	4
	.type	test_i128_andn_imm_ext,@function
test_i128_andn_imm_ext:                 ! @test_i128_andn_imm_ext
.Ltest_i128_andn_imm_ext$local:
	.type	.Ltest_i128_andn_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	andn_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end104:
	.size	test_i128_andn_imm_ext, .Lfunc_end104-test_i128_andn_imm_ext
	.size	.Ltest_i128_andn_imm_ext$local, .Lfunc_end104-test_i128_andn_imm_ext
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
	alloc 3
	or %r1, %r2, %r1
	retf 0

.Lfunc_end105:
	.size	test_i128_or, .Lfunc_end105-test_i128_or
	.size	.Ltest_i128_or$local, .Lfunc_end105-test_i128_or
                                        ! -- End function
	.globl	test_i128_or_imm                ! -- Begin function test_i128_or_imm
	.p2align	4
	.type	test_i128_or_imm,@function
test_i128_or_imm:                       ! @test_i128_or_imm
.Ltest_i128_or_imm$local:
	.type	.Ltest_i128_or_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	or_imm %r1, %r1, 12345
	retf 0

.Lfunc_end106:
	.size	test_i128_or_imm, .Lfunc_end106-test_i128_or_imm
	.size	.Ltest_i128_or_imm$local, .Lfunc_end106-test_i128_or_imm
                                        ! -- End function
	.globl	test_i128_or_imm_ext            ! -- Begin function test_i128_or_imm_ext
	.p2align	4
	.type	test_i128_or_imm_ext,@function
test_i128_or_imm_ext:                   ! @test_i128_or_imm_ext
.Ltest_i128_or_imm_ext$local:
	.type	.Ltest_i128_or_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	or_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end107:
	.size	test_i128_or_imm_ext, .Lfunc_end107-test_i128_or_imm_ext
	.size	.Ltest_i128_or_imm_ext$local, .Lfunc_end107-test_i128_or_imm_ext
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
	alloc 3
	orn %r1, %r1, %r2
	retf 0

.Lfunc_end108:
	.size	test_i128_orn, .Lfunc_end108-test_i128_orn
	.size	.Ltest_i128_orn$local, .Lfunc_end108-test_i128_orn
                                        ! -- End function
	.globl	test_i128_orn_imm               ! -- Begin function test_i128_orn_imm
	.p2align	4
	.type	test_i128_orn_imm,@function
test_i128_orn_imm:                      ! @test_i128_orn_imm
.Ltest_i128_orn_imm$local:
	.type	.Ltest_i128_orn_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	orn_imm %r1, %r1, 12345
	retf 0

.Lfunc_end109:
	.size	test_i128_orn_imm, .Lfunc_end109-test_i128_orn_imm
	.size	.Ltest_i128_orn_imm$local, .Lfunc_end109-test_i128_orn_imm
                                        ! -- End function
	.globl	test_i128_orn_imm_ext           ! -- Begin function test_i128_orn_imm_ext
	.p2align	4
	.type	test_i128_orn_imm_ext,@function
test_i128_orn_imm_ext:                  ! @test_i128_orn_imm_ext
.Ltest_i128_orn_imm_ext$local:
	.type	.Ltest_i128_orn_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	orn_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end110:
	.size	test_i128_orn_imm_ext, .Lfunc_end110-test_i128_orn_imm_ext
	.size	.Ltest_i128_orn_imm_ext$local, .Lfunc_end110-test_i128_orn_imm_ext
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
	alloc 3
	xor %r1, %r2, %r1
	retf 0

.Lfunc_end111:
	.size	test_i128_xor, .Lfunc_end111-test_i128_xor
	.size	.Ltest_i128_xor$local, .Lfunc_end111-test_i128_xor
                                        ! -- End function
	.globl	test_i128_xor_imm               ! -- Begin function test_i128_xor_imm
	.p2align	4
	.type	test_i128_xor_imm,@function
test_i128_xor_imm:                      ! @test_i128_xor_imm
.Ltest_i128_xor_imm$local:
	.type	.Ltest_i128_xor_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	xor_imm %r1, %r1, 12345
	retf 0

.Lfunc_end112:
	.size	test_i128_xor_imm, .Lfunc_end112-test_i128_xor_imm
	.size	.Ltest_i128_xor_imm$local, .Lfunc_end112-test_i128_xor_imm
                                        ! -- End function
	.globl	test_i128_xor_imm_ext           ! -- Begin function test_i128_xor_imm_ext
	.p2align	4
	.type	test_i128_xor_imm_ext,@function
test_i128_xor_imm_ext:                  ! @test_i128_xor_imm_ext
.Ltest_i128_xor_imm_ext$local:
	.type	.Ltest_i128_xor_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	xor_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end113:
	.size	test_i128_xor_imm_ext, .Lfunc_end113-test_i128_xor_imm_ext
	.size	.Ltest_i128_xor_imm_ext$local, .Lfunc_end113-test_i128_xor_imm_ext
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
	alloc 2
	not %r1, %r1
	retf 0

.Lfunc_end114:
	.size	test_i32_inv, .Lfunc_end114-test_i32_inv
	.size	.Ltest_i32_inv$local, .Lfunc_end114-test_i32_inv
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
	alloc 3
	and %r1, %r2, %r1
	retf 0

.Lfunc_end115:
	.size	test_i32_and, .Lfunc_end115-test_i32_and
	.size	.Ltest_i32_and$local, .Lfunc_end115-test_i32_and
                                        ! -- End function
	.globl	test_i32_and_imm                ! -- Begin function test_i32_and_imm
	.p2align	4
	.type	test_i32_and_imm,@function
test_i32_and_imm:                       ! @test_i32_and_imm
.Ltest_i32_and_imm$local:
	.type	.Ltest_i32_and_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	and_imm %r1, %r1, 12345
	retf 0

.Lfunc_end116:
	.size	test_i32_and_imm, .Lfunc_end116-test_i32_and_imm
	.size	.Ltest_i32_and_imm$local, .Lfunc_end116-test_i32_and_imm
                                        ! -- End function
	.globl	test_i32_and_imm_ext            ! -- Begin function test_i32_and_imm_ext
	.p2align	4
	.type	test_i32_and_imm_ext,@function
test_i32_and_imm_ext:                   ! @test_i32_and_imm_ext
.Ltest_i32_and_imm_ext$local:
	.type	.Ltest_i32_and_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	and_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end117:
	.size	test_i32_and_imm_ext, .Lfunc_end117-test_i32_and_imm_ext
	.size	.Ltest_i32_and_imm_ext$local, .Lfunc_end117-test_i32_and_imm_ext
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
	alloc 3
	andn %r1, %r1, %r2
	retf 0

.Lfunc_end118:
	.size	test_i32_andn, .Lfunc_end118-test_i32_andn
	.size	.Ltest_i32_andn$local, .Lfunc_end118-test_i32_andn
                                        ! -- End function
	.globl	test_i32_andn_imm               ! -- Begin function test_i32_andn_imm
	.p2align	4
	.type	test_i32_andn_imm,@function
test_i32_andn_imm:                      ! @test_i32_andn_imm
.Ltest_i32_andn_imm$local:
	.type	.Ltest_i32_andn_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	andn_imm %r1, %r1, 12345
	retf 0

.Lfunc_end119:
	.size	test_i32_andn_imm, .Lfunc_end119-test_i32_andn_imm
	.size	.Ltest_i32_andn_imm$local, .Lfunc_end119-test_i32_andn_imm
                                        ! -- End function
	.globl	test_i32_andn_imm_ext           ! -- Begin function test_i32_andn_imm_ext
	.p2align	4
	.type	test_i32_andn_imm_ext,@function
test_i32_andn_imm_ext:                  ! @test_i32_andn_imm_ext
.Ltest_i32_andn_imm_ext$local:
	.type	.Ltest_i32_andn_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	andn_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end120:
	.size	test_i32_andn_imm_ext, .Lfunc_end120-test_i32_andn_imm_ext
	.size	.Ltest_i32_andn_imm_ext$local, .Lfunc_end120-test_i32_andn_imm_ext
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
	alloc 3
	or %r1, %r2, %r1
	retf 0

.Lfunc_end121:
	.size	test_i32_or, .Lfunc_end121-test_i32_or
	.size	.Ltest_i32_or$local, .Lfunc_end121-test_i32_or
                                        ! -- End function
	.globl	test_i32_or_imm                 ! -- Begin function test_i32_or_imm
	.p2align	4
	.type	test_i32_or_imm,@function
test_i32_or_imm:                        ! @test_i32_or_imm
.Ltest_i32_or_imm$local:
	.type	.Ltest_i32_or_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	or_imm %r1, %r1, 12345
	retf 0

.Lfunc_end122:
	.size	test_i32_or_imm, .Lfunc_end122-test_i32_or_imm
	.size	.Ltest_i32_or_imm$local, .Lfunc_end122-test_i32_or_imm
                                        ! -- End function
	.globl	test_i32_or_imm_ext             ! -- Begin function test_i32_or_imm_ext
	.p2align	4
	.type	test_i32_or_imm_ext,@function
test_i32_or_imm_ext:                    ! @test_i32_or_imm_ext
.Ltest_i32_or_imm_ext$local:
	.type	.Ltest_i32_or_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	or_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end123:
	.size	test_i32_or_imm_ext, .Lfunc_end123-test_i32_or_imm_ext
	.size	.Ltest_i32_or_imm_ext$local, .Lfunc_end123-test_i32_or_imm_ext
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
	alloc 3
	orn %r1, %r1, %r2
	retf 0

.Lfunc_end124:
	.size	test_i32_orn, .Lfunc_end124-test_i32_orn
	.size	.Ltest_i32_orn$local, .Lfunc_end124-test_i32_orn
                                        ! -- End function
	.globl	test_i32_orn_imm                ! -- Begin function test_i32_orn_imm
	.p2align	4
	.type	test_i32_orn_imm,@function
test_i32_orn_imm:                       ! @test_i32_orn_imm
.Ltest_i32_orn_imm$local:
	.type	.Ltest_i32_orn_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	orn_imm %r1, %r1, 12345
	retf 0

.Lfunc_end125:
	.size	test_i32_orn_imm, .Lfunc_end125-test_i32_orn_imm
	.size	.Ltest_i32_orn_imm$local, .Lfunc_end125-test_i32_orn_imm
                                        ! -- End function
	.globl	test_i32_orn_imm_ext            ! -- Begin function test_i32_orn_imm_ext
	.p2align	4
	.type	test_i32_orn_imm_ext,@function
test_i32_orn_imm_ext:                   ! @test_i32_orn_imm_ext
.Ltest_i32_orn_imm_ext$local:
	.type	.Ltest_i32_orn_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	orn_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end126:
	.size	test_i32_orn_imm_ext, .Lfunc_end126-test_i32_orn_imm_ext
	.size	.Ltest_i32_orn_imm_ext$local, .Lfunc_end126-test_i32_orn_imm_ext
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
	alloc 3
	xor %r1, %r2, %r1
	retf 0

.Lfunc_end127:
	.size	test_i32_xor, .Lfunc_end127-test_i32_xor
	.size	.Ltest_i32_xor$local, .Lfunc_end127-test_i32_xor
                                        ! -- End function
	.globl	test_i32_xor_imm                ! -- Begin function test_i32_xor_imm
	.p2align	4
	.type	test_i32_xor_imm,@function
test_i32_xor_imm:                       ! @test_i32_xor_imm
.Ltest_i32_xor_imm$local:
	.type	.Ltest_i32_xor_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	xor_imm %r1, %r1, 12345
	retf 0

.Lfunc_end128:
	.size	test_i32_xor_imm, .Lfunc_end128-test_i32_xor_imm
	.size	.Ltest_i32_xor_imm$local, .Lfunc_end128-test_i32_xor_imm
                                        ! -- End function
	.globl	test_i32_xor_imm_ext            ! -- Begin function test_i32_xor_imm_ext
	.p2align	4
	.type	test_i32_xor_imm_ext,@function
test_i32_xor_imm_ext:                   ! @test_i32_xor_imm_ext
.Ltest_i32_xor_imm_ext$local:
	.type	.Ltest_i32_xor_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	xor_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end129:
	.size	test_i32_xor_imm_ext, .Lfunc_end129-test_i32_xor_imm_ext
	.size	.Ltest_i32_xor_imm_ext$local, .Lfunc_end129-test_i32_xor_imm_ext
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
	alloc 2
	not %r1, %r1
	retf 0

.Lfunc_end130:
	.size	test_i64_inv, .Lfunc_end130-test_i64_inv
	.size	.Ltest_i64_inv$local, .Lfunc_end130-test_i64_inv
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
	alloc 3
	and %r1, %r2, %r1
	retf 0

.Lfunc_end131:
	.size	test_i64_and, .Lfunc_end131-test_i64_and
	.size	.Ltest_i64_and$local, .Lfunc_end131-test_i64_and
                                        ! -- End function
	.globl	test_i64_and_imm                ! -- Begin function test_i64_and_imm
	.p2align	4
	.type	test_i64_and_imm,@function
test_i64_and_imm:                       ! @test_i64_and_imm
.Ltest_i64_and_imm$local:
	.type	.Ltest_i64_and_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	and_imm %r1, %r1, 12345
	retf 0

.Lfunc_end132:
	.size	test_i64_and_imm, .Lfunc_end132-test_i64_and_imm
	.size	.Ltest_i64_and_imm$local, .Lfunc_end132-test_i64_and_imm
                                        ! -- End function
	.globl	test_i64_and_imm_ext            ! -- Begin function test_i64_and_imm_ext
	.p2align	4
	.type	test_i64_and_imm_ext,@function
test_i64_and_imm_ext:                   ! @test_i64_and_imm_ext
.Ltest_i64_and_imm_ext$local:
	.type	.Ltest_i64_and_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	and_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end133:
	.size	test_i64_and_imm_ext, .Lfunc_end133-test_i64_and_imm_ext
	.size	.Ltest_i64_and_imm_ext$local, .Lfunc_end133-test_i64_and_imm_ext
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
	alloc 3
	andn %r1, %r1, %r2
	retf 0

.Lfunc_end134:
	.size	test_i64_andn, .Lfunc_end134-test_i64_andn
	.size	.Ltest_i64_andn$local, .Lfunc_end134-test_i64_andn
                                        ! -- End function
	.globl	test_i64_andn_imm               ! -- Begin function test_i64_andn_imm
	.p2align	4
	.type	test_i64_andn_imm,@function
test_i64_andn_imm:                      ! @test_i64_andn_imm
.Ltest_i64_andn_imm$local:
	.type	.Ltest_i64_andn_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	andn_imm %r1, %r1, 12345
	retf 0

.Lfunc_end135:
	.size	test_i64_andn_imm, .Lfunc_end135-test_i64_andn_imm
	.size	.Ltest_i64_andn_imm$local, .Lfunc_end135-test_i64_andn_imm
                                        ! -- End function
	.globl	test_i64_andn_imm_ext           ! -- Begin function test_i64_andn_imm_ext
	.p2align	4
	.type	test_i64_andn_imm_ext,@function
test_i64_andn_imm_ext:                  ! @test_i64_andn_imm_ext
.Ltest_i64_andn_imm_ext$local:
	.type	.Ltest_i64_andn_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	andn_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end136:
	.size	test_i64_andn_imm_ext, .Lfunc_end136-test_i64_andn_imm_ext
	.size	.Ltest_i64_andn_imm_ext$local, .Lfunc_end136-test_i64_andn_imm_ext
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
	alloc 3
	or %r1, %r2, %r1
	retf 0

.Lfunc_end137:
	.size	test_i64_or, .Lfunc_end137-test_i64_or
	.size	.Ltest_i64_or$local, .Lfunc_end137-test_i64_or
                                        ! -- End function
	.globl	test_i64_or_imm                 ! -- Begin function test_i64_or_imm
	.p2align	4
	.type	test_i64_or_imm,@function
test_i64_or_imm:                        ! @test_i64_or_imm
.Ltest_i64_or_imm$local:
	.type	.Ltest_i64_or_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	or_imm %r1, %r1, 12345
	retf 0

.Lfunc_end138:
	.size	test_i64_or_imm, .Lfunc_end138-test_i64_or_imm
	.size	.Ltest_i64_or_imm$local, .Lfunc_end138-test_i64_or_imm
                                        ! -- End function
	.globl	test_i64_or_imm_ext             ! -- Begin function test_i64_or_imm_ext
	.p2align	4
	.type	test_i64_or_imm_ext,@function
test_i64_or_imm_ext:                    ! @test_i64_or_imm_ext
.Ltest_i64_or_imm_ext$local:
	.type	.Ltest_i64_or_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	or_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end139:
	.size	test_i64_or_imm_ext, .Lfunc_end139-test_i64_or_imm_ext
	.size	.Ltest_i64_or_imm_ext$local, .Lfunc_end139-test_i64_or_imm_ext
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
	alloc 3
	orn %r1, %r1, %r2
	retf 0

.Lfunc_end140:
	.size	test_i64_orn, .Lfunc_end140-test_i64_orn
	.size	.Ltest_i64_orn$local, .Lfunc_end140-test_i64_orn
                                        ! -- End function
	.globl	test_i64_orn_imm                ! -- Begin function test_i64_orn_imm
	.p2align	4
	.type	test_i64_orn_imm,@function
test_i64_orn_imm:                       ! @test_i64_orn_imm
.Ltest_i64_orn_imm$local:
	.type	.Ltest_i64_orn_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	orn_imm %r1, %r1, 12345
	retf 0

.Lfunc_end141:
	.size	test_i64_orn_imm, .Lfunc_end141-test_i64_orn_imm
	.size	.Ltest_i64_orn_imm$local, .Lfunc_end141-test_i64_orn_imm
                                        ! -- End function
	.globl	test_i64_orn_imm_ext            ! -- Begin function test_i64_orn_imm_ext
	.p2align	4
	.type	test_i64_orn_imm_ext,@function
test_i64_orn_imm_ext:                   ! @test_i64_orn_imm_ext
.Ltest_i64_orn_imm_ext$local:
	.type	.Ltest_i64_orn_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	orn_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end142:
	.size	test_i64_orn_imm_ext, .Lfunc_end142-test_i64_orn_imm_ext
	.size	.Ltest_i64_orn_imm_ext$local, .Lfunc_end142-test_i64_orn_imm_ext
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
	alloc 3
	xor %r1, %r2, %r1
	retf 0

.Lfunc_end143:
	.size	test_i64_xor, .Lfunc_end143-test_i64_xor
	.size	.Ltest_i64_xor$local, .Lfunc_end143-test_i64_xor
                                        ! -- End function
	.globl	test_i64_xor_imm                ! -- Begin function test_i64_xor_imm
	.p2align	4
	.type	test_i64_xor_imm,@function
test_i64_xor_imm:                       ! @test_i64_xor_imm
.Ltest_i64_xor_imm$local:
	.type	.Ltest_i64_xor_imm$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	xor_imm %r1, %r1, 12345
	retf 0

.Lfunc_end144:
	.size	test_i64_xor_imm, .Lfunc_end144-test_i64_xor_imm
	.size	.Ltest_i64_xor_imm$local, .Lfunc_end144-test_i64_xor_imm
                                        ! -- End function
	.globl	test_i64_xor_imm_ext            ! -- Begin function test_i64_xor_imm_ext
	.p2align	4
	.type	test_i64_xor_imm_ext,@function
test_i64_xor_imm_ext:                   ! @test_i64_xor_imm_ext
.Ltest_i64_xor_imm_ext$local:
	.type	.Ltest_i64_xor_imm_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	xor_imm.l %r1, %r1, 123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end145:
	.size	test_i64_xor_imm_ext, .Lfunc_end145-test_i64_xor_imm_ext
	.size	.Ltest_i64_xor_imm_ext$local, .Lfunc_end145-test_i64_xor_imm_ext
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
	alloc 3
	and %r1, %r1, %r2
	retf 0

.Lfunc_end146:
	.size	test_i1_and, .Lfunc_end146-test_i1_and
	.size	.Ltest_i1_and$local, .Lfunc_end146-test_i1_and
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
	alloc 3
	or %r1, %r1, %r2
	retf 0

.Lfunc_end147:
	.size	test_i1_or, .Lfunc_end147-test_i1_or
	.size	.Ltest_i1_or$local, .Lfunc_end147-test_i1_or
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
	alloc 3
	xor %r1, %r1, %r2
	retf 0

.Lfunc_end148:
	.size	test_i1_xor, .Lfunc_end148-test_i1_xor
	.size	.Ltest_i1_xor$local, .Lfunc_end148-test_i1_xor
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
	alloc 2
	xor_imm %r1, %r1, 1
	retf 0

.Lfunc_end149:
	.size	test_i1_not, .Lfunc_end149-test_i1_not
	.size	.Ltest_i1_not$local, .Lfunc_end149-test_i1_not
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
	alloc 3
	xor_imm %r1, %r1, 1
	and %r1, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end150:
	.size	test_i1_andn, .Lfunc_end150-test_i1_andn
	.size	.Ltest_i1_andn$local, .Lfunc_end150-test_i1_andn
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
	alloc 3
	xor_imm %r1, %r1, 1
	or %r1, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end151:
	.size	test_i1_orn, .Lfunc_end151-test_i1_orn
	.size	.Ltest_i1_orn$local, .Lfunc_end151-test_i1_orn
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
	alloc 3
	and %r1, %r1, %r2
	xor_imm %r1, %r1, 1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end152:
	.size	test_i1_nand, .Lfunc_end152-test_i1_nand
	.size	.Ltest_i1_nand$local, .Lfunc_end152-test_i1_nand
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end153:
	.size	test_i1_to_i32, .Lfunc_end153-test_i1_to_i32
	.size	.Ltest_i1_to_i32$local, .Lfunc_end153-test_i1_to_i32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end154:
	.size	test_i1_to_i64, .Lfunc_end154-test_i1_to_i64
	.size	.Ltest_i1_to_i64$local, .Lfunc_end154-test_i1_to_i64
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
	alloc 2
	ld_imm %r1, 1
	retf 0

.Lfunc_end155:
	.size	test_i1_inv, .Lfunc_end155-test_i1_inv
	.size	.Ltest_i1_inv$local, .Lfunc_end155-test_i1_inv
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
	alloc 2
	ld_imm %r1, 1
	retf 0

.Lfunc_end156:
	.size	test_imm_to_i1, .Lfunc_end156-test_imm_to_i1
	.size	.Ltest_imm_to_i1$local, .Lfunc_end156-test_imm_to_i1
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
	alloc 2
	ld_imm %r1, 1
	st_iprel_i8 %r1, %pcfwd(test_bool_true)

	! BUNDLE
	st_iprel_i8 %gz, %pcfwd(test_bool_false)
	st_iprel_i8 %r1, %pcfwd(test_i8_true)
	st_iprel_i8 %gz, %pcfwd(test_i8_false)

	! BUNDLE
	st_iprel_i16 %r1, %pcfwd(test_i16_true)
	st_iprel_i16 %gz, %pcfwd(test_i16_false)
	st_iprel_i32 %r1, %pcfwd(test_i32_true)

	! BUNDLE
	st_iprel_i32 %gz, %pcfwd(test_i32_false)
	st_iprel_i64 %r1, %pcfwd(test_i64_true)
	st_iprel_i64 %gz, %pcfwd(test_i64_false)

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end157:
	.size	test_write_i1, .Lfunc_end157-test_write_i1
	.size	.Ltest_write_i1$local, .Lfunc_end157-test_write_i1
                                        ! -- End function
	.globl	br_eq_imm_i32                   ! -- Begin function br_eq_imm_i32
	.p2align	4
	.type	br_eq_imm_i32,@function
br_eq_imm_i32:                          ! @br_eq_imm_i32
.Lbr_eq_imm_i32$local:
	.type	.Lbr_eq_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i32 %r1, 543, .LBB158_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB158_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end158:
	.size	br_eq_imm_i32, .Lfunc_end158-br_eq_imm_i32
	.size	.Lbr_eq_imm_i32$local, .Lfunc_end158-br_eq_imm_i32
                                        ! -- End function
	.globl	br_ne_imm_i32                   ! -- Begin function br_ne_imm_i32
	.p2align	4
	.type	br_ne_imm_i32,@function
br_ne_imm_i32:                          ! @br_ne_imm_i32
.Lbr_ne_imm_i32$local:
	.type	.Lbr_ne_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i32 %r1, 543, .LBB159_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB159_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end159:
	.size	br_ne_imm_i32, .Lfunc_end159-br_ne_imm_i32
	.size	.Lbr_ne_imm_i32$local, .Lfunc_end159-br_ne_imm_i32
                                        ! -- End function
	.globl	br_lt_imm_i32                   ! -- Begin function br_lt_imm_i32
	.p2align	4
	.type	br_lt_imm_i32,@function
br_lt_imm_i32:                          ! @br_lt_imm_i32
.Lbr_lt_imm_i32$local:
	.type	.Lbr_lt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i32 %r1, 543, .LBB160_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB160_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end160:
	.size	br_lt_imm_i32, .Lfunc_end160-br_lt_imm_i32
	.size	.Lbr_lt_imm_i32$local, .Lfunc_end160-br_lt_imm_i32
                                        ! -- End function
	.globl	br_ge_imm_i32                   ! -- Begin function br_ge_imm_i32
	.p2align	4
	.type	br_ge_imm_i32,@function
br_ge_imm_i32:                          ! @br_ge_imm_i32
.Lbr_ge_imm_i32$local:
	.type	.Lbr_ge_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i32 %r1, 543, .LBB161_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB161_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end161:
	.size	br_ge_imm_i32, .Lfunc_end161-br_ge_imm_i32
	.size	.Lbr_ge_imm_i32$local, .Lfunc_end161-br_ge_imm_i32
                                        ! -- End function
	.globl	br_le_imm_i32                   ! -- Begin function br_le_imm_i32
	.p2align	4
	.type	br_le_imm_i32,@function
br_le_imm_i32:                          ! @br_le_imm_i32
.Lbr_le_imm_i32$local:
	.type	.Lbr_le_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i32 %r1, 543, .LBB162_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB162_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end162:
	.size	br_le_imm_i32, .Lfunc_end162-br_le_imm_i32
	.size	.Lbr_le_imm_i32$local, .Lfunc_end162-br_le_imm_i32
                                        ! -- End function
	.globl	br_gt_imm_i32                   ! -- Begin function br_gt_imm_i32
	.p2align	4
	.type	br_gt_imm_i32,@function
br_gt_imm_i32:                          ! @br_gt_imm_i32
.Lbr_gt_imm_i32$local:
	.type	.Lbr_gt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i32 %r1, 543, .LBB163_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB163_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end163:
	.size	br_gt_imm_i32, .Lfunc_end163-br_gt_imm_i32
	.size	.Lbr_gt_imm_i32$local, .Lfunc_end163-br_gt_imm_i32
                                        ! -- End function
	.globl	br_lt_imm_u32                   ! -- Begin function br_lt_imm_u32
	.p2align	4
	.type	br_lt_imm_u32,@function
br_lt_imm_u32:                          ! @br_lt_imm_u32
.Lbr_lt_imm_u32$local:
	.type	.Lbr_lt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u32 %r1, 543, .LBB164_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB164_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end164:
	.size	br_lt_imm_u32, .Lfunc_end164-br_lt_imm_u32
	.size	.Lbr_lt_imm_u32$local, .Lfunc_end164-br_lt_imm_u32
                                        ! -- End function
	.globl	br_ge_imm_u32                   ! -- Begin function br_ge_imm_u32
	.p2align	4
	.type	br_ge_imm_u32,@function
br_ge_imm_u32:                          ! @br_ge_imm_u32
.Lbr_ge_imm_u32$local:
	.type	.Lbr_ge_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u32 %r1, 543, .LBB165_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB165_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end165:
	.size	br_ge_imm_u32, .Lfunc_end165-br_ge_imm_u32
	.size	.Lbr_ge_imm_u32$local, .Lfunc_end165-br_ge_imm_u32
                                        ! -- End function
	.globl	br_le_imm_u32                   ! -- Begin function br_le_imm_u32
	.p2align	4
	.type	br_le_imm_u32,@function
br_le_imm_u32:                          ! @br_le_imm_u32
.Lbr_le_imm_u32$local:
	.type	.Lbr_le_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u32 %r1, 543, .LBB166_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB166_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end166:
	.size	br_le_imm_u32, .Lfunc_end166-br_le_imm_u32
	.size	.Lbr_le_imm_u32$local, .Lfunc_end166-br_le_imm_u32
                                        ! -- End function
	.globl	br_gt_imm_u32                   ! -- Begin function br_gt_imm_u32
	.p2align	4
	.type	br_gt_imm_u32,@function
br_gt_imm_u32:                          ! @br_gt_imm_u32
.Lbr_gt_imm_u32$local:
	.type	.Lbr_gt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u32 %r1, 543, .LBB167_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB167_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end167:
	.size	br_gt_imm_u32, .Lfunc_end167-br_gt_imm_u32
	.size	.Lbr_gt_imm_u32$local, .Lfunc_end167-br_gt_imm_u32
                                        ! -- End function
	.globl	br_eq_imm_i32_ext               ! -- Begin function br_eq_imm_i32_ext
	.p2align	4
	.type	br_eq_imm_i32_ext,@function
br_eq_imm_i32_ext:                      ! @br_eq_imm_i32_ext
.Lbr_eq_imm_i32_ext$local:
	.type	.Lbr_eq_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i32.l %r1, 1234567890, .LBB168_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB168_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end168:
	.size	br_eq_imm_i32_ext, .Lfunc_end168-br_eq_imm_i32_ext
	.size	.Lbr_eq_imm_i32_ext$local, .Lfunc_end168-br_eq_imm_i32_ext
                                        ! -- End function
	.globl	br_ne_imm_i32_ext               ! -- Begin function br_ne_imm_i32_ext
	.p2align	4
	.type	br_ne_imm_i32_ext,@function
br_ne_imm_i32_ext:                      ! @br_ne_imm_i32_ext
.Lbr_ne_imm_i32_ext$local:
	.type	.Lbr_ne_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i32.l %r1, 1234567890, .LBB169_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB169_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end169:
	.size	br_ne_imm_i32_ext, .Lfunc_end169-br_ne_imm_i32_ext
	.size	.Lbr_ne_imm_i32_ext$local, .Lfunc_end169-br_ne_imm_i32_ext
                                        ! -- End function
	.globl	br_lt_imm_i32_ext               ! -- Begin function br_lt_imm_i32_ext
	.p2align	4
	.type	br_lt_imm_i32_ext,@function
br_lt_imm_i32_ext:                      ! @br_lt_imm_i32_ext
.Lbr_lt_imm_i32_ext$local:
	.type	.Lbr_lt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i32.l %r1, 1234567890, .LBB170_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB170_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end170:
	.size	br_lt_imm_i32_ext, .Lfunc_end170-br_lt_imm_i32_ext
	.size	.Lbr_lt_imm_i32_ext$local, .Lfunc_end170-br_lt_imm_i32_ext
                                        ! -- End function
	.globl	br_ge_imm_i32_ext               ! -- Begin function br_ge_imm_i32_ext
	.p2align	4
	.type	br_ge_imm_i32_ext,@function
br_ge_imm_i32_ext:                      ! @br_ge_imm_i32_ext
.Lbr_ge_imm_i32_ext$local:
	.type	.Lbr_ge_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i32.l %r1, 1234567890, .LBB171_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB171_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end171:
	.size	br_ge_imm_i32_ext, .Lfunc_end171-br_ge_imm_i32_ext
	.size	.Lbr_ge_imm_i32_ext$local, .Lfunc_end171-br_ge_imm_i32_ext
                                        ! -- End function
	.globl	br_le_imm_i32_ext               ! -- Begin function br_le_imm_i32_ext
	.p2align	4
	.type	br_le_imm_i32_ext,@function
br_le_imm_i32_ext:                      ! @br_le_imm_i32_ext
.Lbr_le_imm_i32_ext$local:
	.type	.Lbr_le_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i32.l %r1, 1234567890, .LBB172_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB172_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end172:
	.size	br_le_imm_i32_ext, .Lfunc_end172-br_le_imm_i32_ext
	.size	.Lbr_le_imm_i32_ext$local, .Lfunc_end172-br_le_imm_i32_ext
                                        ! -- End function
	.globl	br_gt_imm_i32_ext               ! -- Begin function br_gt_imm_i32_ext
	.p2align	4
	.type	br_gt_imm_i32_ext,@function
br_gt_imm_i32_ext:                      ! @br_gt_imm_i32_ext
.Lbr_gt_imm_i32_ext$local:
	.type	.Lbr_gt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i32.l %r1, 1234567890, .LBB173_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB173_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end173:
	.size	br_gt_imm_i32_ext, .Lfunc_end173-br_gt_imm_i32_ext
	.size	.Lbr_gt_imm_i32_ext$local, .Lfunc_end173-br_gt_imm_i32_ext
                                        ! -- End function
	.globl	br_lt_imm_u32_ext               ! -- Begin function br_lt_imm_u32_ext
	.p2align	4
	.type	br_lt_imm_u32_ext,@function
br_lt_imm_u32_ext:                      ! @br_lt_imm_u32_ext
.Lbr_lt_imm_u32_ext$local:
	.type	.Lbr_lt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u32.l %r1, 1234567890, .LBB174_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB174_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end174:
	.size	br_lt_imm_u32_ext, .Lfunc_end174-br_lt_imm_u32_ext
	.size	.Lbr_lt_imm_u32_ext$local, .Lfunc_end174-br_lt_imm_u32_ext
                                        ! -- End function
	.globl	br_ge_imm_u32_ext               ! -- Begin function br_ge_imm_u32_ext
	.p2align	4
	.type	br_ge_imm_u32_ext,@function
br_ge_imm_u32_ext:                      ! @br_ge_imm_u32_ext
.Lbr_ge_imm_u32_ext$local:
	.type	.Lbr_ge_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u32.l %r1, 1234567890, .LBB175_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB175_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end175:
	.size	br_ge_imm_u32_ext, .Lfunc_end175-br_ge_imm_u32_ext
	.size	.Lbr_ge_imm_u32_ext$local, .Lfunc_end175-br_ge_imm_u32_ext
                                        ! -- End function
	.globl	br_le_imm_u32_ext               ! -- Begin function br_le_imm_u32_ext
	.p2align	4
	.type	br_le_imm_u32_ext,@function
br_le_imm_u32_ext:                      ! @br_le_imm_u32_ext
.Lbr_le_imm_u32_ext$local:
	.type	.Lbr_le_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u32.l %r1, 1234567890, .LBB176_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB176_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end176:
	.size	br_le_imm_u32_ext, .Lfunc_end176-br_le_imm_u32_ext
	.size	.Lbr_le_imm_u32_ext$local, .Lfunc_end176-br_le_imm_u32_ext
                                        ! -- End function
	.globl	br_gt_imm_u32_ext               ! -- Begin function br_gt_imm_u32_ext
	.p2align	4
	.type	br_gt_imm_u32_ext,@function
br_gt_imm_u32_ext:                      ! @br_gt_imm_u32_ext
.Lbr_gt_imm_u32_ext$local:
	.type	.Lbr_gt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u32.l %r1, 1234567890, .LBB177_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB177_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end177:
	.size	br_gt_imm_u32_ext, .Lfunc_end177-br_gt_imm_u32_ext
	.size	.Lbr_gt_imm_u32_ext$local, .Lfunc_end177-br_gt_imm_u32_ext
                                        ! -- End function
	.globl	br_eq_imm_i64                   ! -- Begin function br_eq_imm_i64
	.p2align	4
	.type	br_eq_imm_i64,@function
br_eq_imm_i64:                          ! @br_eq_imm_i64
.Lbr_eq_imm_i64$local:
	.type	.Lbr_eq_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i64 %r1, 543, .LBB178_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB178_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end178:
	.size	br_eq_imm_i64, .Lfunc_end178-br_eq_imm_i64
	.size	.Lbr_eq_imm_i64$local, .Lfunc_end178-br_eq_imm_i64
                                        ! -- End function
	.globl	br_ne_imm_i64                   ! -- Begin function br_ne_imm_i64
	.p2align	4
	.type	br_ne_imm_i64,@function
br_ne_imm_i64:                          ! @br_ne_imm_i64
.Lbr_ne_imm_i64$local:
	.type	.Lbr_ne_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i64 %r1, 543, .LBB179_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB179_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end179:
	.size	br_ne_imm_i64, .Lfunc_end179-br_ne_imm_i64
	.size	.Lbr_ne_imm_i64$local, .Lfunc_end179-br_ne_imm_i64
                                        ! -- End function
	.globl	br_lt_imm_i64                   ! -- Begin function br_lt_imm_i64
	.p2align	4
	.type	br_lt_imm_i64,@function
br_lt_imm_i64:                          ! @br_lt_imm_i64
.Lbr_lt_imm_i64$local:
	.type	.Lbr_lt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i64 %r1, 543, .LBB180_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB180_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end180:
	.size	br_lt_imm_i64, .Lfunc_end180-br_lt_imm_i64
	.size	.Lbr_lt_imm_i64$local, .Lfunc_end180-br_lt_imm_i64
                                        ! -- End function
	.globl	br_ge_imm_i64                   ! -- Begin function br_ge_imm_i64
	.p2align	4
	.type	br_ge_imm_i64,@function
br_ge_imm_i64:                          ! @br_ge_imm_i64
.Lbr_ge_imm_i64$local:
	.type	.Lbr_ge_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i64 %r1, 543, .LBB181_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB181_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end181:
	.size	br_ge_imm_i64, .Lfunc_end181-br_ge_imm_i64
	.size	.Lbr_ge_imm_i64$local, .Lfunc_end181-br_ge_imm_i64
                                        ! -- End function
	.globl	br_le_imm_i64                   ! -- Begin function br_le_imm_i64
	.p2align	4
	.type	br_le_imm_i64,@function
br_le_imm_i64:                          ! @br_le_imm_i64
.Lbr_le_imm_i64$local:
	.type	.Lbr_le_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i64 %r1, 543, .LBB182_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB182_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end182:
	.size	br_le_imm_i64, .Lfunc_end182-br_le_imm_i64
	.size	.Lbr_le_imm_i64$local, .Lfunc_end182-br_le_imm_i64
                                        ! -- End function
	.globl	br_gt_imm_i64                   ! -- Begin function br_gt_imm_i64
	.p2align	4
	.type	br_gt_imm_i64,@function
br_gt_imm_i64:                          ! @br_gt_imm_i64
.Lbr_gt_imm_i64$local:
	.type	.Lbr_gt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i64 %r1, 543, .LBB183_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB183_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end183:
	.size	br_gt_imm_i64, .Lfunc_end183-br_gt_imm_i64
	.size	.Lbr_gt_imm_i64$local, .Lfunc_end183-br_gt_imm_i64
                                        ! -- End function
	.globl	br_lt_imm_u64                   ! -- Begin function br_lt_imm_u64
	.p2align	4
	.type	br_lt_imm_u64,@function
br_lt_imm_u64:                          ! @br_lt_imm_u64
.Lbr_lt_imm_u64$local:
	.type	.Lbr_lt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u64 %r1, 543, .LBB184_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB184_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end184:
	.size	br_lt_imm_u64, .Lfunc_end184-br_lt_imm_u64
	.size	.Lbr_lt_imm_u64$local, .Lfunc_end184-br_lt_imm_u64
                                        ! -- End function
	.globl	br_ge_imm_u64                   ! -- Begin function br_ge_imm_u64
	.p2align	4
	.type	br_ge_imm_u64,@function
br_ge_imm_u64:                          ! @br_ge_imm_u64
.Lbr_ge_imm_u64$local:
	.type	.Lbr_ge_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u64 %r1, 543, .LBB185_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB185_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end185:
	.size	br_ge_imm_u64, .Lfunc_end185-br_ge_imm_u64
	.size	.Lbr_ge_imm_u64$local, .Lfunc_end185-br_ge_imm_u64
                                        ! -- End function
	.globl	br_le_imm_u64                   ! -- Begin function br_le_imm_u64
	.p2align	4
	.type	br_le_imm_u64,@function
br_le_imm_u64:                          ! @br_le_imm_u64
.Lbr_le_imm_u64$local:
	.type	.Lbr_le_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u64 %r1, 543, .LBB186_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB186_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end186:
	.size	br_le_imm_u64, .Lfunc_end186-br_le_imm_u64
	.size	.Lbr_le_imm_u64$local, .Lfunc_end186-br_le_imm_u64
                                        ! -- End function
	.globl	br_gt_imm_u64                   ! -- Begin function br_gt_imm_u64
	.p2align	4
	.type	br_gt_imm_u64,@function
br_gt_imm_u64:                          ! @br_gt_imm_u64
.Lbr_gt_imm_u64$local:
	.type	.Lbr_gt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u64 %r1, 543, .LBB187_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB187_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end187:
	.size	br_gt_imm_u64, .Lfunc_end187-br_gt_imm_u64
	.size	.Lbr_gt_imm_u64$local, .Lfunc_end187-br_gt_imm_u64
                                        ! -- End function
	.globl	br_eq_imm_i64_ext               ! -- Begin function br_eq_imm_i64_ext
	.p2align	4
	.type	br_eq_imm_i64_ext,@function
br_eq_imm_i64_ext:                      ! @br_eq_imm_i64_ext
.Lbr_eq_imm_i64_ext$local:
	.type	.Lbr_eq_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i64.l %r1, 1234567890, .LBB188_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB188_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end188:
	.size	br_eq_imm_i64_ext, .Lfunc_end188-br_eq_imm_i64_ext
	.size	.Lbr_eq_imm_i64_ext$local, .Lfunc_end188-br_eq_imm_i64_ext
                                        ! -- End function
	.globl	br_ne_imm_i64_ext               ! -- Begin function br_ne_imm_i64_ext
	.p2align	4
	.type	br_ne_imm_i64_ext,@function
br_ne_imm_i64_ext:                      ! @br_ne_imm_i64_ext
.Lbr_ne_imm_i64_ext$local:
	.type	.Lbr_ne_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i64.l %r1, 1234567890, .LBB189_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB189_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end189:
	.size	br_ne_imm_i64_ext, .Lfunc_end189-br_ne_imm_i64_ext
	.size	.Lbr_ne_imm_i64_ext$local, .Lfunc_end189-br_ne_imm_i64_ext
                                        ! -- End function
	.globl	br_lt_imm_i64_ext               ! -- Begin function br_lt_imm_i64_ext
	.p2align	4
	.type	br_lt_imm_i64_ext,@function
br_lt_imm_i64_ext:                      ! @br_lt_imm_i64_ext
.Lbr_lt_imm_i64_ext$local:
	.type	.Lbr_lt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i64.l %r1, 1234567890, .LBB190_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB190_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end190:
	.size	br_lt_imm_i64_ext, .Lfunc_end190-br_lt_imm_i64_ext
	.size	.Lbr_lt_imm_i64_ext$local, .Lfunc_end190-br_lt_imm_i64_ext
                                        ! -- End function
	.globl	br_ge_imm_i64_ext               ! -- Begin function br_ge_imm_i64_ext
	.p2align	4
	.type	br_ge_imm_i64_ext,@function
br_ge_imm_i64_ext:                      ! @br_ge_imm_i64_ext
.Lbr_ge_imm_i64_ext$local:
	.type	.Lbr_ge_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i64.l %r1, 1234567890, .LBB191_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB191_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end191:
	.size	br_ge_imm_i64_ext, .Lfunc_end191-br_ge_imm_i64_ext
	.size	.Lbr_ge_imm_i64_ext$local, .Lfunc_end191-br_ge_imm_i64_ext
                                        ! -- End function
	.globl	br_le_imm_i64_ext               ! -- Begin function br_le_imm_i64_ext
	.p2align	4
	.type	br_le_imm_i64_ext,@function
br_le_imm_i64_ext:                      ! @br_le_imm_i64_ext
.Lbr_le_imm_i64_ext$local:
	.type	.Lbr_le_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i64.l %r1, 1234567890, .LBB192_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB192_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end192:
	.size	br_le_imm_i64_ext, .Lfunc_end192-br_le_imm_i64_ext
	.size	.Lbr_le_imm_i64_ext$local, .Lfunc_end192-br_le_imm_i64_ext
                                        ! -- End function
	.globl	br_gt_imm_i64_ext               ! -- Begin function br_gt_imm_i64_ext
	.p2align	4
	.type	br_gt_imm_i64_ext,@function
br_gt_imm_i64_ext:                      ! @br_gt_imm_i64_ext
.Lbr_gt_imm_i64_ext$local:
	.type	.Lbr_gt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i64.l %r1, 1234567890, .LBB193_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB193_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end193:
	.size	br_gt_imm_i64_ext, .Lfunc_end193-br_gt_imm_i64_ext
	.size	.Lbr_gt_imm_i64_ext$local, .Lfunc_end193-br_gt_imm_i64_ext
                                        ! -- End function
	.globl	br_lt_imm_u64_ext               ! -- Begin function br_lt_imm_u64_ext
	.p2align	4
	.type	br_lt_imm_u64_ext,@function
br_lt_imm_u64_ext:                      ! @br_lt_imm_u64_ext
.Lbr_lt_imm_u64_ext$local:
	.type	.Lbr_lt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u64.l %r1, 1234567890, .LBB194_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB194_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end194:
	.size	br_lt_imm_u64_ext, .Lfunc_end194-br_lt_imm_u64_ext
	.size	.Lbr_lt_imm_u64_ext$local, .Lfunc_end194-br_lt_imm_u64_ext
                                        ! -- End function
	.globl	br_ge_imm_u64_ext               ! -- Begin function br_ge_imm_u64_ext
	.p2align	4
	.type	br_ge_imm_u64_ext,@function
br_ge_imm_u64_ext:                      ! @br_ge_imm_u64_ext
.Lbr_ge_imm_u64_ext$local:
	.type	.Lbr_ge_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u64.l %r1, 1234567890, .LBB195_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB195_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end195:
	.size	br_ge_imm_u64_ext, .Lfunc_end195-br_ge_imm_u64_ext
	.size	.Lbr_ge_imm_u64_ext$local, .Lfunc_end195-br_ge_imm_u64_ext
                                        ! -- End function
	.globl	br_le_imm_u64_ext               ! -- Begin function br_le_imm_u64_ext
	.p2align	4
	.type	br_le_imm_u64_ext,@function
br_le_imm_u64_ext:                      ! @br_le_imm_u64_ext
.Lbr_le_imm_u64_ext$local:
	.type	.Lbr_le_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u64.l %r1, 1234567890, .LBB196_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB196_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end196:
	.size	br_le_imm_u64_ext, .Lfunc_end196-br_le_imm_u64_ext
	.size	.Lbr_le_imm_u64_ext$local, .Lfunc_end196-br_le_imm_u64_ext
                                        ! -- End function
	.globl	br_gt_imm_u64_ext               ! -- Begin function br_gt_imm_u64_ext
	.p2align	4
	.type	br_gt_imm_u64_ext,@function
br_gt_imm_u64_ext:                      ! @br_gt_imm_u64_ext
.Lbr_gt_imm_u64_ext$local:
	.type	.Lbr_gt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u64.l %r1, 1234567890, .LBB197_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB197_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end197:
	.size	br_gt_imm_u64_ext, .Lfunc_end197-br_gt_imm_u64_ext
	.size	.Lbr_gt_imm_u64_ext$local, .Lfunc_end197-br_gt_imm_u64_ext
                                        ! -- End function
	.globl	br_eq_imm_i128                  ! -- Begin function br_eq_imm_i128
	.p2align	4
	.type	br_eq_imm_i128,@function
br_eq_imm_i128:                         ! @br_eq_imm_i128
.Lbr_eq_imm_i128$local:
	.type	.Lbr_eq_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i128 %r1, 543, .LBB198_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB198_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end198:
	.size	br_eq_imm_i128, .Lfunc_end198-br_eq_imm_i128
	.size	.Lbr_eq_imm_i128$local, .Lfunc_end198-br_eq_imm_i128
                                        ! -- End function
	.globl	br_ne_imm_i128                  ! -- Begin function br_ne_imm_i128
	.p2align	4
	.type	br_ne_imm_i128,@function
br_ne_imm_i128:                         ! @br_ne_imm_i128
.Lbr_ne_imm_i128$local:
	.type	.Lbr_ne_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i128 %r1, 543, .LBB199_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB199_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end199:
	.size	br_ne_imm_i128, .Lfunc_end199-br_ne_imm_i128
	.size	.Lbr_ne_imm_i128$local, .Lfunc_end199-br_ne_imm_i128
                                        ! -- End function
	.globl	br_lt_imm_i128                  ! -- Begin function br_lt_imm_i128
	.p2align	4
	.type	br_lt_imm_i128,@function
br_lt_imm_i128:                         ! @br_lt_imm_i128
.Lbr_lt_imm_i128$local:
	.type	.Lbr_lt_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i128 %r1, 543, .LBB200_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB200_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end200:
	.size	br_lt_imm_i128, .Lfunc_end200-br_lt_imm_i128
	.size	.Lbr_lt_imm_i128$local, .Lfunc_end200-br_lt_imm_i128
                                        ! -- End function
	.globl	br_ge_imm_i128                  ! -- Begin function br_ge_imm_i128
	.p2align	4
	.type	br_ge_imm_i128,@function
br_ge_imm_i128:                         ! @br_ge_imm_i128
.Lbr_ge_imm_i128$local:
	.type	.Lbr_ge_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i128 %r1, 543, .LBB201_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB201_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end201:
	.size	br_ge_imm_i128, .Lfunc_end201-br_ge_imm_i128
	.size	.Lbr_ge_imm_i128$local, .Lfunc_end201-br_ge_imm_i128
                                        ! -- End function
	.globl	br_le_imm_i128                  ! -- Begin function br_le_imm_i128
	.p2align	4
	.type	br_le_imm_i128,@function
br_le_imm_i128:                         ! @br_le_imm_i128
.Lbr_le_imm_i128$local:
	.type	.Lbr_le_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i128 %r1, 543, .LBB202_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB202_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end202:
	.size	br_le_imm_i128, .Lfunc_end202-br_le_imm_i128
	.size	.Lbr_le_imm_i128$local, .Lfunc_end202-br_le_imm_i128
                                        ! -- End function
	.globl	br_gt_imm_i128                  ! -- Begin function br_gt_imm_i128
	.p2align	4
	.type	br_gt_imm_i128,@function
br_gt_imm_i128:                         ! @br_gt_imm_i128
.Lbr_gt_imm_i128$local:
	.type	.Lbr_gt_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i128 %r1, 543, .LBB203_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB203_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end203:
	.size	br_gt_imm_i128, .Lfunc_end203-br_gt_imm_i128
	.size	.Lbr_gt_imm_i128$local, .Lfunc_end203-br_gt_imm_i128
                                        ! -- End function
	.globl	br_lt_imm_u128                  ! -- Begin function br_lt_imm_u128
	.p2align	4
	.type	br_lt_imm_u128,@function
br_lt_imm_u128:                         ! @br_lt_imm_u128
.Lbr_lt_imm_u128$local:
	.type	.Lbr_lt_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u128 %r1, 543, .LBB204_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB204_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end204:
	.size	br_lt_imm_u128, .Lfunc_end204-br_lt_imm_u128
	.size	.Lbr_lt_imm_u128$local, .Lfunc_end204-br_lt_imm_u128
                                        ! -- End function
	.globl	br_ge_imm_u128                  ! -- Begin function br_ge_imm_u128
	.p2align	4
	.type	br_ge_imm_u128,@function
br_ge_imm_u128:                         ! @br_ge_imm_u128
.Lbr_ge_imm_u128$local:
	.type	.Lbr_ge_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u128 %r1, 543, .LBB205_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB205_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end205:
	.size	br_ge_imm_u128, .Lfunc_end205-br_ge_imm_u128
	.size	.Lbr_ge_imm_u128$local, .Lfunc_end205-br_ge_imm_u128
                                        ! -- End function
	.globl	br_le_imm_u128                  ! -- Begin function br_le_imm_u128
	.p2align	4
	.type	br_le_imm_u128,@function
br_le_imm_u128:                         ! @br_le_imm_u128
.Lbr_le_imm_u128$local:
	.type	.Lbr_le_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u128 %r1, 543, .LBB206_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB206_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end206:
	.size	br_le_imm_u128, .Lfunc_end206-br_le_imm_u128
	.size	.Lbr_le_imm_u128$local, .Lfunc_end206-br_le_imm_u128
                                        ! -- End function
	.globl	br_gt_imm_u128                  ! -- Begin function br_gt_imm_u128
	.p2align	4
	.type	br_gt_imm_u128,@function
br_gt_imm_u128:                         ! @br_gt_imm_u128
.Lbr_gt_imm_u128$local:
	.type	.Lbr_gt_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u128 %r1, 543, .LBB207_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB207_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end207:
	.size	br_gt_imm_u128, .Lfunc_end207-br_gt_imm_u128
	.size	.Lbr_gt_imm_u128$local, .Lfunc_end207-br_gt_imm_u128
                                        ! -- End function
	.globl	br_eq_imm_i128_ext              ! -- Begin function br_eq_imm_i128_ext
	.p2align	4
	.type	br_eq_imm_i128_ext,@function
br_eq_imm_i128_ext:                     ! @br_eq_imm_i128_ext
.Lbr_eq_imm_i128_ext$local:
	.type	.Lbr_eq_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i128.l %r1, 1234567890, .LBB208_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB208_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end208:
	.size	br_eq_imm_i128_ext, .Lfunc_end208-br_eq_imm_i128_ext
	.size	.Lbr_eq_imm_i128_ext$local, .Lfunc_end208-br_eq_imm_i128_ext
                                        ! -- End function
	.globl	br_ne_imm_i128_ext              ! -- Begin function br_ne_imm_i128_ext
	.p2align	4
	.type	br_ne_imm_i128_ext,@function
br_ne_imm_i128_ext:                     ! @br_ne_imm_i128_ext
.Lbr_ne_imm_i128_ext$local:
	.type	.Lbr_ne_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i128.l %r1, 1234567890, .LBB209_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB209_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end209:
	.size	br_ne_imm_i128_ext, .Lfunc_end209-br_ne_imm_i128_ext
	.size	.Lbr_ne_imm_i128_ext$local, .Lfunc_end209-br_ne_imm_i128_ext
                                        ! -- End function
	.globl	br_lt_imm_i128_ext              ! -- Begin function br_lt_imm_i128_ext
	.p2align	4
	.type	br_lt_imm_i128_ext,@function
br_lt_imm_i128_ext:                     ! @br_lt_imm_i128_ext
.Lbr_lt_imm_i128_ext$local:
	.type	.Lbr_lt_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i128.l %r1, 1234567890, .LBB210_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB210_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end210:
	.size	br_lt_imm_i128_ext, .Lfunc_end210-br_lt_imm_i128_ext
	.size	.Lbr_lt_imm_i128_ext$local, .Lfunc_end210-br_lt_imm_i128_ext
                                        ! -- End function
	.globl	br_ge_imm_i128_ext              ! -- Begin function br_ge_imm_i128_ext
	.p2align	4
	.type	br_ge_imm_i128_ext,@function
br_ge_imm_i128_ext:                     ! @br_ge_imm_i128_ext
.Lbr_ge_imm_i128_ext$local:
	.type	.Lbr_ge_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i128.l %r1, 1234567890, .LBB211_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB211_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end211:
	.size	br_ge_imm_i128_ext, .Lfunc_end211-br_ge_imm_i128_ext
	.size	.Lbr_ge_imm_i128_ext$local, .Lfunc_end211-br_ge_imm_i128_ext
                                        ! -- End function
	.globl	br_le_imm_i128_ext              ! -- Begin function br_le_imm_i128_ext
	.p2align	4
	.type	br_le_imm_i128_ext,@function
br_le_imm_i128_ext:                     ! @br_le_imm_i128_ext
.Lbr_le_imm_i128_ext$local:
	.type	.Lbr_le_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i128.l %r1, 1234567890, .LBB212_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB212_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end212:
	.size	br_le_imm_i128_ext, .Lfunc_end212-br_le_imm_i128_ext
	.size	.Lbr_le_imm_i128_ext$local, .Lfunc_end212-br_le_imm_i128_ext
                                        ! -- End function
	.globl	br_gt_imm_i128_ext              ! -- Begin function br_gt_imm_i128_ext
	.p2align	4
	.type	br_gt_imm_i128_ext,@function
br_gt_imm_i128_ext:                     ! @br_gt_imm_i128_ext
.Lbr_gt_imm_i128_ext$local:
	.type	.Lbr_gt_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i128.l %r1, 1234567890, .LBB213_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB213_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end213:
	.size	br_gt_imm_i128_ext, .Lfunc_end213-br_gt_imm_i128_ext
	.size	.Lbr_gt_imm_i128_ext$local, .Lfunc_end213-br_gt_imm_i128_ext
                                        ! -- End function
	.globl	br_lt_imm_u128_ext              ! -- Begin function br_lt_imm_u128_ext
	.p2align	4
	.type	br_lt_imm_u128_ext,@function
br_lt_imm_u128_ext:                     ! @br_lt_imm_u128_ext
.Lbr_lt_imm_u128_ext$local:
	.type	.Lbr_lt_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u128.l %r1, 1234567890, .LBB214_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB214_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end214:
	.size	br_lt_imm_u128_ext, .Lfunc_end214-br_lt_imm_u128_ext
	.size	.Lbr_lt_imm_u128_ext$local, .Lfunc_end214-br_lt_imm_u128_ext
                                        ! -- End function
	.globl	br_ge_imm_u128_ext              ! -- Begin function br_ge_imm_u128_ext
	.p2align	4
	.type	br_ge_imm_u128_ext,@function
br_ge_imm_u128_ext:                     ! @br_ge_imm_u128_ext
.Lbr_ge_imm_u128_ext$local:
	.type	.Lbr_ge_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u128.l %r1, 1234567890, .LBB215_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB215_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end215:
	.size	br_ge_imm_u128_ext, .Lfunc_end215-br_ge_imm_u128_ext
	.size	.Lbr_ge_imm_u128_ext$local, .Lfunc_end215-br_ge_imm_u128_ext
                                        ! -- End function
	.globl	br_le_imm_u128_ext              ! -- Begin function br_le_imm_u128_ext
	.p2align	4
	.type	br_le_imm_u128_ext,@function
br_le_imm_u128_ext:                     ! @br_le_imm_u128_ext
.Lbr_le_imm_u128_ext$local:
	.type	.Lbr_le_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_u128.l %r1, 1234567890, .LBB216_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB216_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end216:
	.size	br_le_imm_u128_ext, .Lfunc_end216-br_le_imm_u128_ext
	.size	.Lbr_le_imm_u128_ext$local, .Lfunc_end216-br_le_imm_u128_ext
                                        ! -- End function
	.globl	br_gt_imm_u128_ext              ! -- Begin function br_gt_imm_u128_ext
	.p2align	4
	.type	br_gt_imm_u128_ext,@function
br_gt_imm_u128_ext:                     ! @br_gt_imm_u128_ext
.Lbr_gt_imm_u128_ext$local:
	.type	.Lbr_gt_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_u128.l %r1, 1234567890, .LBB217_1

	! BUNDLE
	nop 222
	retf 0
	nop 0

.LBB217_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end217:
	.size	br_gt_imm_u128_ext, .Lfunc_end217-br_gt_imm_u128_ext
	.size	.Lbr_gt_imm_u128_ext$local, .Lfunc_end217-br_gt_imm_u128_ext
                                        ! -- End function
	.globl	br_eq_zero_i32                  ! -- Begin function br_eq_zero_i32
	.p2align	4
	.type	br_eq_zero_i32,@function
br_eq_zero_i32:                         ! @br_eq_zero_i32
.Lbr_eq_zero_i32$local:
	.type	.Lbr_eq_zero_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i32 %r1, 0, .LBB218_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB218_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end218:
	.size	br_eq_zero_i32, .Lfunc_end218-br_eq_zero_i32
	.size	.Lbr_eq_zero_i32$local, .Lfunc_end218-br_eq_zero_i32
                                        ! -- End function
	.globl	br_ne_zero_i32                  ! -- Begin function br_ne_zero_i32
	.p2align	4
	.type	br_ne_zero_i32,@function
br_ne_zero_i32:                         ! @br_ne_zero_i32
.Lbr_ne_zero_i32$local:
	.type	.Lbr_ne_zero_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i32 %r1, 0, .LBB219_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB219_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end219:
	.size	br_ne_zero_i32, .Lfunc_end219-br_ne_zero_i32
	.size	.Lbr_ne_zero_i32$local, .Lfunc_end219-br_ne_zero_i32
                                        ! -- End function
	.globl	br_lt_zero_i32                  ! -- Begin function br_lt_zero_i32
	.p2align	4
	.type	br_lt_zero_i32,@function
br_lt_zero_i32:                         ! @br_lt_zero_i32
.Lbr_lt_zero_i32$local:
	.type	.Lbr_lt_zero_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i32 %r1, 0, .LBB220_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB220_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end220:
	.size	br_lt_zero_i32, .Lfunc_end220-br_lt_zero_i32
	.size	.Lbr_lt_zero_i32$local, .Lfunc_end220-br_lt_zero_i32
                                        ! -- End function
	.globl	br_ge_zero_i32                  ! -- Begin function br_ge_zero_i32
	.p2align	4
	.type	br_ge_zero_i32,@function
br_ge_zero_i32:                         ! @br_ge_zero_i32
.Lbr_ge_zero_i32$local:
	.type	.Lbr_ge_zero_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i32 %r1, 0, .LBB221_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB221_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end221:
	.size	br_ge_zero_i32, .Lfunc_end221-br_ge_zero_i32
	.size	.Lbr_ge_zero_i32$local, .Lfunc_end221-br_ge_zero_i32
                                        ! -- End function
	.globl	br_le_zero_i32                  ! -- Begin function br_le_zero_i32
	.p2align	4
	.type	br_le_zero_i32,@function
br_le_zero_i32:                         ! @br_le_zero_i32
.Lbr_le_zero_i32$local:
	.type	.Lbr_le_zero_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i32 %r1, 1, .LBB222_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB222_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end222:
	.size	br_le_zero_i32, .Lfunc_end222-br_le_zero_i32
	.size	.Lbr_le_zero_i32$local, .Lfunc_end222-br_le_zero_i32
                                        ! -- End function
	.globl	br_gt_zero_i32                  ! -- Begin function br_gt_zero_i32
	.p2align	4
	.type	br_gt_zero_i32,@function
br_gt_zero_i32:                         ! @br_gt_zero_i32
.Lbr_gt_zero_i32$local:
	.type	.Lbr_gt_zero_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i32 %r1, 1, .LBB223_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB223_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end223:
	.size	br_gt_zero_i32, .Lfunc_end223-br_gt_zero_i32
	.size	.Lbr_gt_zero_i32$local, .Lfunc_end223-br_gt_zero_i32
                                        ! -- End function
	.globl	br_gt_zero_u32                  ! -- Begin function br_gt_zero_u32
	.p2align	4
	.type	br_gt_zero_u32,@function
br_gt_zero_u32:                         ! @br_gt_zero_u32
.Lbr_gt_zero_u32$local:
	.type	.Lbr_gt_zero_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i32 %r1, 0, .LBB224_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB224_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end224:
	.size	br_gt_zero_u32, .Lfunc_end224-br_gt_zero_u32
	.size	.Lbr_gt_zero_u32$local, .Lfunc_end224-br_gt_zero_u32
                                        ! -- End function
	.globl	br_eq_zero_i64                  ! -- Begin function br_eq_zero_i64
	.p2align	4
	.type	br_eq_zero_i64,@function
br_eq_zero_i64:                         ! @br_eq_zero_i64
.Lbr_eq_zero_i64$local:
	.type	.Lbr_eq_zero_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i64 %r1, 0, .LBB225_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB225_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end225:
	.size	br_eq_zero_i64, .Lfunc_end225-br_eq_zero_i64
	.size	.Lbr_eq_zero_i64$local, .Lfunc_end225-br_eq_zero_i64
                                        ! -- End function
	.globl	br_ne_zero_i64                  ! -- Begin function br_ne_zero_i64
	.p2align	4
	.type	br_ne_zero_i64,@function
br_ne_zero_i64:                         ! @br_ne_zero_i64
.Lbr_ne_zero_i64$local:
	.type	.Lbr_ne_zero_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i64 %r1, 0, .LBB226_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB226_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end226:
	.size	br_ne_zero_i64, .Lfunc_end226-br_ne_zero_i64
	.size	.Lbr_ne_zero_i64$local, .Lfunc_end226-br_ne_zero_i64
                                        ! -- End function
	.globl	br_lt_zero_i64                  ! -- Begin function br_lt_zero_i64
	.p2align	4
	.type	br_lt_zero_i64,@function
br_lt_zero_i64:                         ! @br_lt_zero_i64
.Lbr_lt_zero_i64$local:
	.type	.Lbr_lt_zero_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i64 %r1, 0, .LBB227_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB227_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end227:
	.size	br_lt_zero_i64, .Lfunc_end227-br_lt_zero_i64
	.size	.Lbr_lt_zero_i64$local, .Lfunc_end227-br_lt_zero_i64
                                        ! -- End function
	.globl	br_ge_zero_i64                  ! -- Begin function br_ge_zero_i64
	.p2align	4
	.type	br_ge_zero_i64,@function
br_ge_zero_i64:                         ! @br_ge_zero_i64
.Lbr_ge_zero_i64$local:
	.type	.Lbr_ge_zero_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i64 %r1, 0, .LBB228_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB228_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end228:
	.size	br_ge_zero_i64, .Lfunc_end228-br_ge_zero_i64
	.size	.Lbr_ge_zero_i64$local, .Lfunc_end228-br_ge_zero_i64
                                        ! -- End function
	.globl	br_le_zero_i64                  ! -- Begin function br_le_zero_i64
	.p2align	4
	.type	br_le_zero_i64,@function
br_le_zero_i64:                         ! @br_le_zero_i64
.Lbr_le_zero_i64$local:
	.type	.Lbr_le_zero_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i64 %r1, 1, .LBB229_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB229_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end229:
	.size	br_le_zero_i64, .Lfunc_end229-br_le_zero_i64
	.size	.Lbr_le_zero_i64$local, .Lfunc_end229-br_le_zero_i64
                                        ! -- End function
	.globl	br_gt_zero_i64                  ! -- Begin function br_gt_zero_i64
	.p2align	4
	.type	br_gt_zero_i64,@function
br_gt_zero_i64:                         ! @br_gt_zero_i64
.Lbr_gt_zero_i64$local:
	.type	.Lbr_gt_zero_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i64 %r1, 1, .LBB230_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB230_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end230:
	.size	br_gt_zero_i64, .Lfunc_end230-br_gt_zero_i64
	.size	.Lbr_gt_zero_i64$local, .Lfunc_end230-br_gt_zero_i64
                                        ! -- End function
	.globl	br_gt_zero_u64                  ! -- Begin function br_gt_zero_u64
	.p2align	4
	.type	br_gt_zero_u64,@function
br_gt_zero_u64:                         ! @br_gt_zero_u64
.Lbr_gt_zero_u64$local:
	.type	.Lbr_gt_zero_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i64 %r1, 0, .LBB231_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB231_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end231:
	.size	br_gt_zero_u64, .Lfunc_end231-br_gt_zero_u64
	.size	.Lbr_gt_zero_u64$local, .Lfunc_end231-br_gt_zero_u64
                                        ! -- End function
	.globl	br_eq_zero_i128                 ! -- Begin function br_eq_zero_i128
	.p2align	4
	.type	br_eq_zero_i128,@function
br_eq_zero_i128:                        ! @br_eq_zero_i128
.Lbr_eq_zero_i128$local:
	.type	.Lbr_eq_zero_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_eq_imm_i128 %r1, 0, .LBB232_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB232_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end232:
	.size	br_eq_zero_i128, .Lfunc_end232-br_eq_zero_i128
	.size	.Lbr_eq_zero_i128$local, .Lfunc_end232-br_eq_zero_i128
                                        ! -- End function
	.globl	br_ne_zero_i128                 ! -- Begin function br_ne_zero_i128
	.p2align	4
	.type	br_ne_zero_i128,@function
br_ne_zero_i128:                        ! @br_ne_zero_i128
.Lbr_ne_zero_i128$local:
	.type	.Lbr_ne_zero_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i128 %r1, 0, .LBB233_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB233_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end233:
	.size	br_ne_zero_i128, .Lfunc_end233-br_ne_zero_i128
	.size	.Lbr_ne_zero_i128$local, .Lfunc_end233-br_ne_zero_i128
                                        ! -- End function
	.globl	br_lt_zero_i128                 ! -- Begin function br_lt_zero_i128
	.p2align	4
	.type	br_lt_zero_i128,@function
br_lt_zero_i128:                        ! @br_lt_zero_i128
.Lbr_lt_zero_i128$local:
	.type	.Lbr_lt_zero_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i128 %r1, 0, .LBB234_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB234_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end234:
	.size	br_lt_zero_i128, .Lfunc_end234-br_lt_zero_i128
	.size	.Lbr_lt_zero_i128$local, .Lfunc_end234-br_lt_zero_i128
                                        ! -- End function
	.globl	br_ge_zero_i128                 ! -- Begin function br_ge_zero_i128
	.p2align	4
	.type	br_ge_zero_i128,@function
br_ge_zero_i128:                        ! @br_ge_zero_i128
.Lbr_ge_zero_i128$local:
	.type	.Lbr_ge_zero_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i128 %r1, 0, .LBB235_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB235_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end235:
	.size	br_ge_zero_i128, .Lfunc_end235-br_ge_zero_i128
	.size	.Lbr_ge_zero_i128$local, .Lfunc_end235-br_ge_zero_i128
                                        ! -- End function
	.globl	br_le_zero_i128                 ! -- Begin function br_le_zero_i128
	.p2align	4
	.type	br_le_zero_i128,@function
br_le_zero_i128:                        ! @br_le_zero_i128
.Lbr_le_zero_i128$local:
	.type	.Lbr_le_zero_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_lt_imm_i128 %r1, 1, .LBB236_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB236_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end236:
	.size	br_le_zero_i128, .Lfunc_end236-br_le_zero_i128
	.size	.Lbr_le_zero_i128$local, .Lfunc_end236-br_le_zero_i128
                                        ! -- End function
	.globl	br_gt_zero_i128                 ! -- Begin function br_gt_zero_i128
	.p2align	4
	.type	br_gt_zero_i128,@function
br_gt_zero_i128:                        ! @br_gt_zero_i128
.Lbr_gt_zero_i128$local:
	.type	.Lbr_gt_zero_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ge_imm_i128 %r1, 1, .LBB237_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB237_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end237:
	.size	br_gt_zero_i128, .Lfunc_end237-br_gt_zero_i128
	.size	.Lbr_gt_zero_i128$local, .Lfunc_end237-br_gt_zero_i128
                                        ! -- End function
	.globl	br_gt_zero_u128                 ! -- Begin function br_gt_zero_u128
	.p2align	4
	.type	br_gt_zero_u128,@function
br_gt_zero_u128:                        ! @br_gt_zero_u128
.Lbr_gt_zero_u128$local:
	.type	.Lbr_gt_zero_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_ne_imm_i128 %r1, 0, .LBB238_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB238_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end238:
	.size	br_gt_zero_u128, .Lfunc_end238-br_gt_zero_u128
	.size	.Lbr_gt_zero_u128$local, .Lfunc_end238-br_gt_zero_u128
                                        ! -- End function
	.globl	test_bit_1_i32                  ! -- Begin function test_bit_1_i32
	.p2align	4
	.type	test_bit_1_i32,@function
test_bit_1_i32:                         ! @test_bit_1_i32
.Ltest_bit_1_i32$local:
	.type	.Ltest_bit_1_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bs %r1, %r2, .LBB239_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB239_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end239:
	.size	test_bit_1_i32, .Lfunc_end239-test_bit_1_i32
	.size	.Ltest_bit_1_i32$local, .Lfunc_end239-test_bit_1_i32
                                        ! -- End function
	.globl	test_bit_2_i32                  ! -- Begin function test_bit_2_i32
	.p2align	4
	.type	test_bit_2_i32,@function
test_bit_2_i32:                         ! @test_bit_2_i32
.Ltest_bit_2_i32$local:
	.type	.Ltest_bit_2_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bc %r1, %r2, .LBB240_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB240_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end240:
	.size	test_bit_2_i32, .Lfunc_end240-test_bit_2_i32
	.size	.Ltest_bit_2_i32$local, .Lfunc_end240-test_bit_2_i32
                                        ! -- End function
	.globl	test_bit_3_i32                  ! -- Begin function test_bit_3_i32
	.p2align	4
	.type	test_bit_3_i32,@function
test_bit_3_i32:                         ! @test_bit_3_i32
.Ltest_bit_3_i32$local:
	.type	.Ltest_bit_3_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bs %r1, %r2, .LBB241_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB241_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end241:
	.size	test_bit_3_i32, .Lfunc_end241-test_bit_3_i32
	.size	.Ltest_bit_3_i32$local, .Lfunc_end241-test_bit_3_i32
                                        ! -- End function
	.globl	test_bit_4_i32_4                ! -- Begin function test_bit_4_i32_4
	.p2align	4
	.type	test_bit_4_i32_4,@function
test_bit_4_i32_4:                       ! @test_bit_4_i32_4
.Ltest_bit_4_i32_4$local:
	.type	.Ltest_bit_4_i32_4$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_bc_imm %r1, 15, .LBB242_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB242_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end242:
	.size	test_bit_4_i32_4, .Lfunc_end242-test_bit_4_i32_4
	.size	.Ltest_bit_4_i32_4$local, .Lfunc_end242-test_bit_4_i32_4
                                        ! -- End function
	.globl	test_bit_5_i32                  ! -- Begin function test_bit_5_i32
	.p2align	4
	.type	test_bit_5_i32,@function
test_bit_5_i32:                         ! @test_bit_5_i32
.Ltest_bit_5_i32$local:
	.type	.Ltest_bit_5_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_bs_imm %r1, 14, .LBB243_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB243_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end243:
	.size	test_bit_5_i32, .Lfunc_end243-test_bit_5_i32
	.size	.Ltest_bit_5_i32$local, .Lfunc_end243-test_bit_5_i32
                                        ! -- End function
	.globl	test_bit_1_i64                  ! -- Begin function test_bit_1_i64
	.p2align	4
	.type	test_bit_1_i64,@function
test_bit_1_i64:                         ! @test_bit_1_i64
.Ltest_bit_1_i64$local:
	.type	.Ltest_bit_1_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bs %r1, %r2, .LBB244_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB244_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end244:
	.size	test_bit_1_i64, .Lfunc_end244-test_bit_1_i64
	.size	.Ltest_bit_1_i64$local, .Lfunc_end244-test_bit_1_i64
                                        ! -- End function
	.globl	test_bit_2_i64                  ! -- Begin function test_bit_2_i64
	.p2align	4
	.type	test_bit_2_i64,@function
test_bit_2_i64:                         ! @test_bit_2_i64
.Ltest_bit_2_i64$local:
	.type	.Ltest_bit_2_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bc %r1, %r2, .LBB245_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB245_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end245:
	.size	test_bit_2_i64, .Lfunc_end245-test_bit_2_i64
	.size	.Ltest_bit_2_i64$local, .Lfunc_end245-test_bit_2_i64
                                        ! -- End function
	.globl	test_bit_3_i64                  ! -- Begin function test_bit_3_i64
	.p2align	4
	.type	test_bit_3_i64,@function
test_bit_3_i64:                         ! @test_bit_3_i64
.Ltest_bit_3_i64$local:
	.type	.Ltest_bit_3_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bs %r1, %r2, .LBB246_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB246_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end246:
	.size	test_bit_3_i64, .Lfunc_end246-test_bit_3_i64
	.size	.Ltest_bit_3_i64$local, .Lfunc_end246-test_bit_3_i64
                                        ! -- End function
	.globl	test_bit_4_i64_4                ! -- Begin function test_bit_4_i64_4
	.p2align	4
	.type	test_bit_4_i64_4,@function
test_bit_4_i64_4:                       ! @test_bit_4_i64_4
.Ltest_bit_4_i64_4$local:
	.type	.Ltest_bit_4_i64_4$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_bc_imm %r1, 15, .LBB247_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB247_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end247:
	.size	test_bit_4_i64_4, .Lfunc_end247-test_bit_4_i64_4
	.size	.Ltest_bit_4_i64_4$local, .Lfunc_end247-test_bit_4_i64_4
                                        ! -- End function
	.globl	test_bit_5_i64                  ! -- Begin function test_bit_5_i64
	.p2align	4
	.type	test_bit_5_i64,@function
test_bit_5_i64:                         ! @test_bit_5_i64
.Ltest_bit_5_i64$local:
	.type	.Ltest_bit_5_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_bs_imm %r1, 14, .LBB248_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB248_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end248:
	.size	test_bit_5_i64, .Lfunc_end248-test_bit_5_i64
	.size	.Ltest_bit_5_i64$local, .Lfunc_end248-test_bit_5_i64
                                        ! -- End function
	.globl	test_bit_1_i128                 ! -- Begin function test_bit_1_i128
	.p2align	4
	.type	test_bit_1_i128,@function
test_bit_1_i128:                        ! @test_bit_1_i128
.Ltest_bit_1_i128$local:
	.type	.Ltest_bit_1_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bs %r1, %r2, .LBB249_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB249_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end249:
	.size	test_bit_1_i128, .Lfunc_end249-test_bit_1_i128
	.size	.Ltest_bit_1_i128$local, .Lfunc_end249-test_bit_1_i128
                                        ! -- End function
	.globl	test_bit_2_i128                 ! -- Begin function test_bit_2_i128
	.p2align	4
	.type	test_bit_2_i128,@function
test_bit_2_i128:                        ! @test_bit_2_i128
.Ltest_bit_2_i128$local:
	.type	.Ltest_bit_2_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bc %r1, %r2, .LBB250_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB250_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end250:
	.size	test_bit_2_i128, .Lfunc_end250-test_bit_2_i128
	.size	.Ltest_bit_2_i128$local, .Lfunc_end250-test_bit_2_i128
                                        ! -- End function
	.globl	test_bit_3_i128                 ! -- Begin function test_bit_3_i128
	.p2align	4
	.type	test_bit_3_i128,@function
test_bit_3_i128:                        ! @test_bit_3_i128
.Ltest_bit_3_i128$local:
	.type	.Ltest_bit_3_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	br_bs %r1, %r2, .LBB251_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB251_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end251:
	.size	test_bit_3_i128, .Lfunc_end251-test_bit_3_i128
	.size	.Ltest_bit_3_i128$local, .Lfunc_end251-test_bit_3_i128
                                        ! -- End function
	.globl	test_bit_4_i128_4               ! -- Begin function test_bit_4_i128_4
	.p2align	4
	.type	test_bit_4_i128_4,@function
test_bit_4_i128_4:                      ! @test_bit_4_i128_4
.Ltest_bit_4_i128_4$local:
	.type	.Ltest_bit_4_i128_4$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_bc_imm %r1, 15, .LBB252_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB252_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end252:
	.size	test_bit_4_i128_4, .Lfunc_end252-test_bit_4_i128_4
	.size	.Ltest_bit_4_i128_4$local, .Lfunc_end252-test_bit_4_i128_4
                                        ! -- End function
	.globl	test_bit_5_i128                 ! -- Begin function test_bit_5_i128
	.p2align	4
	.type	test_bit_5_i128,@function
test_bit_5_i128:                        ! @test_bit_5_i128
.Ltest_bit_5_i128$local:
	.type	.Ltest_bit_5_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_bs_imm %r1, 14, .LBB253_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB253_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end253:
	.size	test_bit_5_i128, .Lfunc_end253-test_bit_5_i128
	.size	.Ltest_bit_5_i128$local, .Lfunc_end253-test_bit_5_i128
                                        ! -- End function
	.globl	br_mask_notall_i32              ! -- Begin function br_mask_notall_i32
	.p2align	4
	.type	br_mask_notall_i32,@function
br_mask_notall_i32:                     ! @br_mask_notall_i32
.Lbr_mask_notall_i32$local:
	.type	.Lbr_mask_notall_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_notall %r1, 100, .LBB254_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB254_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end254:
	.size	br_mask_notall_i32, .Lfunc_end254-br_mask_notall_i32
	.size	.Lbr_mask_notall_i32$local, .Lfunc_end254-br_mask_notall_i32
                                        ! -- End function
	.globl	br_mask_all_i32                 ! -- Begin function br_mask_all_i32
	.p2align	4
	.type	br_mask_all_i32,@function
br_mask_all_i32:                        ! @br_mask_all_i32
.Lbr_mask_all_i32$local:
	.type	.Lbr_mask_all_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_all %r1, 100, .LBB255_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB255_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end255:
	.size	br_mask_all_i32, .Lfunc_end255-br_mask_all_i32
	.size	.Lbr_mask_all_i32$local, .Lfunc_end255-br_mask_all_i32
                                        ! -- End function
	.globl	br_mask_any_i32                 ! -- Begin function br_mask_any_i32
	.p2align	4
	.type	br_mask_any_i32,@function
br_mask_any_i32:                        ! @br_mask_any_i32
.Lbr_mask_any_i32$local:
	.type	.Lbr_mask_any_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_any %r1, 100, .LBB256_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB256_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end256:
	.size	br_mask_any_i32, .Lfunc_end256-br_mask_any_i32
	.size	.Lbr_mask_any_i32$local, .Lfunc_end256-br_mask_any_i32
                                        ! -- End function
	.globl	br_mask_none_i32                ! -- Begin function br_mask_none_i32
	.p2align	4
	.type	br_mask_none_i32,@function
br_mask_none_i32:                       ! @br_mask_none_i32
.Lbr_mask_none_i32$local:
	.type	.Lbr_mask_none_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_none %r1, 100, .LBB257_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB257_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end257:
	.size	br_mask_none_i32, .Lfunc_end257-br_mask_none_i32
	.size	.Lbr_mask_none_i32$local, .Lfunc_end257-br_mask_none_i32
                                        ! -- End function
	.globl	br_mask_notall_i64              ! -- Begin function br_mask_notall_i64
	.p2align	4
	.type	br_mask_notall_i64,@function
br_mask_notall_i64:                     ! @br_mask_notall_i64
.Lbr_mask_notall_i64$local:
	.type	.Lbr_mask_notall_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_notall %r1, 100, .LBB258_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB258_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end258:
	.size	br_mask_notall_i64, .Lfunc_end258-br_mask_notall_i64
	.size	.Lbr_mask_notall_i64$local, .Lfunc_end258-br_mask_notall_i64
                                        ! -- End function
	.globl	br_mask_all_i64                 ! -- Begin function br_mask_all_i64
	.p2align	4
	.type	br_mask_all_i64,@function
br_mask_all_i64:                        ! @br_mask_all_i64
.Lbr_mask_all_i64$local:
	.type	.Lbr_mask_all_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_all %r1, 100, .LBB259_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB259_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end259:
	.size	br_mask_all_i64, .Lfunc_end259-br_mask_all_i64
	.size	.Lbr_mask_all_i64$local, .Lfunc_end259-br_mask_all_i64
                                        ! -- End function
	.globl	br_mask_any_i64                 ! -- Begin function br_mask_any_i64
	.p2align	4
	.type	br_mask_any_i64,@function
br_mask_any_i64:                        ! @br_mask_any_i64
.Lbr_mask_any_i64$local:
	.type	.Lbr_mask_any_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_any %r1, 100, .LBB260_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB260_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end260:
	.size	br_mask_any_i64, .Lfunc_end260-br_mask_any_i64
	.size	.Lbr_mask_any_i64$local, .Lfunc_end260-br_mask_any_i64
                                        ! -- End function
	.globl	br_mask_none_i64                ! -- Begin function br_mask_none_i64
	.p2align	4
	.type	br_mask_none_i64,@function
br_mask_none_i64:                       ! @br_mask_none_i64
.Lbr_mask_none_i64$local:
	.type	.Lbr_mask_none_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_none %r1, 100, .LBB261_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB261_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end261:
	.size	br_mask_none_i64, .Lfunc_end261-br_mask_none_i64
	.size	.Lbr_mask_none_i64$local, .Lfunc_end261-br_mask_none_i64
                                        ! -- End function
	.globl	br_mask_notall_i128             ! -- Begin function br_mask_notall_i128
	.p2align	4
	.type	br_mask_notall_i128,@function
br_mask_notall_i128:                    ! @br_mask_notall_i128
.Lbr_mask_notall_i128$local:
	.type	.Lbr_mask_notall_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_notall %r1, 100, .LBB262_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB262_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end262:
	.size	br_mask_notall_i128, .Lfunc_end262-br_mask_notall_i128
	.size	.Lbr_mask_notall_i128$local, .Lfunc_end262-br_mask_notall_i128
                                        ! -- End function
	.globl	br_mask_all_i128                ! -- Begin function br_mask_all_i128
	.p2align	4
	.type	br_mask_all_i128,@function
br_mask_all_i128:                       ! @br_mask_all_i128
.Lbr_mask_all_i128$local:
	.type	.Lbr_mask_all_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_all %r1, 100, .LBB263_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB263_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end263:
	.size	br_mask_all_i128, .Lfunc_end263-br_mask_all_i128
	.size	.Lbr_mask_all_i128$local, .Lfunc_end263-br_mask_all_i128
                                        ! -- End function
	.globl	br_mask_any_i128                ! -- Begin function br_mask_any_i128
	.p2align	4
	.type	br_mask_any_i128,@function
br_mask_any_i128:                       ! @br_mask_any_i128
.Lbr_mask_any_i128$local:
	.type	.Lbr_mask_any_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_any %r1, 100, .LBB264_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB264_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end264:
	.size	br_mask_any_i128, .Lfunc_end264-br_mask_any_i128
	.size	.Lbr_mask_any_i128$local, .Lfunc_end264-br_mask_any_i128
                                        ! -- End function
	.globl	br_mask_none_i128               ! -- Begin function br_mask_none_i128
	.p2align	4
	.type	br_mask_none_i128,@function
br_mask_none_i128:                      ! @br_mask_none_i128
.Lbr_mask_none_i128$local:
	.type	.Lbr_mask_none_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	br_mask_none %r1, 100, .LBB265_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB265_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end265:
	.size	br_mask_none_i128, .Lfunc_end265-br_mask_none_i128
	.size	.Lbr_mask_none_i128$local, .Lfunc_end265-br_mask_none_i128
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
	alloc 3
	get_spr %r2, %ip
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %eip
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %fpcr

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %eca
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %rsc
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %rsp

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %bsp
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %reip
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %kip

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %ksp
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %krsp
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %peb

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %teb
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %itc
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %itm

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %pta
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %iva
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %iip

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %iipa
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %cause
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %ifa

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %irr0
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %irr1
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %irr2

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %irr3
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %isr0
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %isr1

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %isr2
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %isr3
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %iv

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %lid
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %tpr
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %itcv

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %tsv
	call %r1, %jpc(consume_sr64)

	! BUNDLE
	get_spr %r2, %pmv
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %cmcv

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %iib
	call %r1, %jpc(consume_sr128)

	! BUNDLE
	get_spr %r2, %psr
	call %r1, %jpc(consume_sr64)
	get_spr %r2, %ipsr

	! BUNDLE
	call %r1, %jpc(consume_sr64)
	retf 0
	nop 0

.Lfunc_end266:
	.size	test_builtin_get_special_reg, .Lfunc_end266-test_builtin_get_special_reg
	.size	.Ltest_builtin_get_special_reg$local, .Lfunc_end266-test_builtin_get_special_reg
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
	alloc 3
	set_spr %r1, %ip
	set_spr %r1, %eip

	! BUNDLE
	set_spr %r1, %fpcr
	set_spr %r1, %eca
	set_spr %r1, %rsc

	! BUNDLE
	set_spr %r1, %rsp
	set_spr %r1, %bsp
	set_spr %r1, %reip

	! BUNDLE
	set_spr %r1, %kip
	set_spr %r1, %ksp
	set_spr %r1, %krsp

	! BUNDLE
	set_spr %r1, %peb
	set_spr %r1, %teb
	set_spr %r1, %itc

	! BUNDLE
	set_spr %r1, %itm
	set_spr %r1, %pta
	set_spr %r1, %iva

	! BUNDLE
	set_spr %r1, %iip
	set_spr %r1, %iipa
	set_spr %r1, %cause

	! BUNDLE
	set_spr %r1, %ifa
	set_spr %r1, %irr0
	set_spr %r1, %irr1

	! BUNDLE
	set_spr %r1, %irr2
	set_spr %r1, %irr3
	set_spr %r1, %isr0

	! BUNDLE
	set_spr %r1, %isr1
	set_spr %r1, %isr2
	set_spr %r1, %isr3

	! BUNDLE
	set_spr %r1, %iv
	set_spr %r1, %lid
	set_spr %r1, %tpr

	! BUNDLE
	set_spr %r1, %itcv
	set_spr %r1, %tsv
	set_spr %r1, %pmv

	! BUNDLE
	set_spr %r1, %cmcv
	set_spr %r1, %psr
	set_spr %r2, %iib

	! BUNDLE
	set_spr %r1, %ipsr
	retf 0
	nop 0

.Lfunc_end267:
	.size	test_builtin_set_special_reg, .Lfunc_end267-test_builtin_set_special_reg
	.size	.Ltest_builtin_set_special_reg$local, .Lfunc_end267-test_builtin_set_special_reg
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
	alloc 3
	ld_i128 %r2, %r1, 16
	st_i128 %r2, %r1, 0

	! BUNDLE
	ld_i128 %r1, %r1, 48
	retf 0
	nop 0

.Lfunc_end268:
	.size	test_i128_load_store, .Lfunc_end268-test_i128_load_store
	.size	.Ltest_i128_load_store$local, .Lfunc_end268-test_i128_load_store
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
	alloc_sp 7, 16
	add_imm_i64 %r2, %sp, 16
	bit_set_imm %r2, %r2, 2

	! BUNDLE
	st_i64 %r2, %sp, 8
	ld_u32 %r2, %sp, 16
	mov2 %r5, %r6, %r1, %r2

	! BUNDLE
	call %r4, %jpc(test_builtin_va_consume_i32)
	ld_u64 %r1, %sp, 8
	alignup_u64 %r1, %r1, 0, 3

	! BUNDLE
	add_imm_i64 %r3, %r1, 8
	st_i64 %r3, %sp, 8
	ld_u64 %r6, %r1, 0

	! BUNDLE
	call %r5, %jpc(test_builtin_va_consume_f64)
	ld_u64 %r1, %sp, 8
	alignup_u64 %r1, %r1, 0, 4

	! BUNDLE
	add_imm_i64 %r3, %r1, 16
	st_i64 %r3, %sp, 8
	ld_i128 %r6, %r1, 0

	! BUNDLE
	call %r5, %jpc(test_builtin_va_consume_f128)
	mov %r1, %r2
	retf 16

.Lfunc_end269:
	.size	test_builtin_va, .Lfunc_end269-test_builtin_va
	.size	.Ltest_builtin_va$local, .Lfunc_end269-test_builtin_va
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
	alloc_sp 6, 272
	st_i128 %r3, %sp, 256
	st_i32 %r1, %sp, 240

	! BUNDLE
	st_i64 %r2, %sp, 232
	st_i32 %r1, %sp, 224
	st_i128 %r3, %sp, 208

	! BUNDLE
	st_i64 %r2, %sp, 200
	st_i32 %r1, %sp, 192
	st_i128 %r3, %sp, 176

	! BUNDLE
	st_i64 %r2, %sp, 168
	st_i32 %r1, %sp, 160
	st_i128 %r3, %sp, 144

	! BUNDLE
	st_i64 %r2, %sp, 136
	st_i32 %r1, %sp, 128
	st_i128 %r3, %sp, 112

	! BUNDLE
	st_i64 %r2, %sp, 104
	st_i32 %r1, %sp, 96
	st_i128 %r3, %sp, 80

	! BUNDLE
	st_i64 %r2, %sp, 72
	st_i32 %r1, %sp, 64
	st_i128 %r3, %sp, 48

	! BUNDLE
	st_i64 %r2, %sp, 40
	st_i32 %r1, %sp, 32
	st_i128 %r3, %sp, 16

	! BUNDLE
	st_i64 %r2, %sp, 8
	lda_iprel %r5, %pcfwd(.L.str)
	st_i32 %r1, %sp, 0

	! BUNDLE
	call %r4, %jpc(test_builtin_va)
	mov %r1, %r5
	retf 272

.Lfunc_end270:
	.size	test_test_builtin_va, .Lfunc_end270-test_test_builtin_va
	.size	.Ltest_test_builtin_va$local, .Lfunc_end270-test_test_builtin_va
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
	alloc 3
	addc_vu8 %r2, %r1, %r2
	addo_vi8 %r2, %r2, %r1

	! BUNDLE
	add_sat_vi8 %r2, %r2, %r1
	add_sat_vu8 %r2, %r2, %r1
	add_vu8 %r2, %r2, %r1

	! BUNDLE
	avg_vu8 %r2, %r2, %r1
	avg_vi8 %r2, %r2, %r1
	cmp_eq_vi8 %r2, %r2, %r1

	! BUNDLE
	cmp_lt_vi8 %r2, %r2, %r1
	cmp_lt_vu8 %r2, %r2, %r1
	max_vi8 %r2, %r2, %r1

	! BUNDLE
	max_vu8 %r2, %r2, %r1
	min_vi8 %r2, %r2, %r1
	min_vu8 %r2, %r2, %r1

	! BUNDLE
	rol_vu8 %r2, %r2, %r1
	ror_vu8 %r2, %r2, %r1
	sll_vu8 %r2, %r2, %r1

	! BUNDLE
	sra_vi8 %r2, %r2, %r1
	srl_vu8 %r2, %r2, %r1
	subb_vu8 %r2, %r2, %r1

	! BUNDLE
	subo_vi8 %r2, %r2, %r1
	sub_vu8 %r2, %r2, %r1
	sub_sat_vi8 %r2, %r2, %r1

	! BUNDLE
	sub_sat_vu8 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end271:
	.size	test_builtin_vector_i8, .Lfunc_end271-test_builtin_vector_i8
	.size	.Ltest_builtin_vector_i8$local, .Lfunc_end271-test_builtin_vector_i8
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
	alloc 3
	addc_vu16 %r2, %r1, %r2
	addo_vi16 %r2, %r2, %r1

	! BUNDLE
	add_sat_vi16 %r2, %r2, %r1
	add_sat_vu16 %r2, %r2, %r1
	add_vu16 %r2, %r2, %r1

	! BUNDLE
	avg_vu16 %r2, %r2, %r1
	avg_vi16 %r2, %r2, %r1
	cmp_eq_vi16 %r2, %r2, %r1

	! BUNDLE
	cmp_lt_vi16 %r2, %r2, %r1
	cmp_lt_vu16 %r2, %r2, %r1
	max_vi16 %r2, %r2, %r1

	! BUNDLE
	max_vu16 %r2, %r2, %r1
	min_vi16 %r2, %r2, %r1
	min_vu16 %r2, %r2, %r1

	! BUNDLE
	rol_vu16 %r2, %r2, %r1
	ror_vu16 %r2, %r2, %r1
	sll_vu16 %r2, %r2, %r1

	! BUNDLE
	sra_vi16 %r2, %r2, %r1
	srl_vu16 %r2, %r2, %r1
	subb_vu16 %r2, %r2, %r1

	! BUNDLE
	subo_vi16 %r2, %r2, %r1
	sub_vu16 %r2, %r2, %r1
	sub_sat_vi16 %r2, %r2, %r1

	! BUNDLE
	sub_sat_vu16 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end272:
	.size	test_builtin_vector_i16, .Lfunc_end272-test_builtin_vector_i16
	.size	.Ltest_builtin_vector_i16$local, .Lfunc_end272-test_builtin_vector_i16
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
	alloc 3
	addc_vu32 %r2, %r1, %r2
	addo_vi32 %r2, %r2, %r1

	! BUNDLE
	add_sat_vi32 %r2, %r2, %r1
	add_sat_vu32 %r2, %r2, %r1
	add_vu32 %r2, %r2, %r1

	! BUNDLE
	avg_vu32 %r2, %r2, %r1
	avg_vi32 %r2, %r2, %r1
	cmp_eq_vi32 %r2, %r2, %r1

	! BUNDLE
	cmp_lt_vi32 %r2, %r2, %r1
	cmp_lt_vu32 %r2, %r2, %r1
	max_vi32 %r2, %r2, %r1

	! BUNDLE
	max_vu32 %r2, %r2, %r1
	min_vi32 %r2, %r2, %r1
	min_vu32 %r2, %r2, %r1

	! BUNDLE
	rol_vu32 %r2, %r2, %r1
	ror_vu32 %r2, %r2, %r1
	sll_vu32 %r2, %r2, %r1

	! BUNDLE
	sra_vi32 %r2, %r2, %r1
	srl_vu32 %r2, %r2, %r1
	subb_vu32 %r2, %r2, %r1

	! BUNDLE
	subo_vi32 %r2, %r2, %r1
	sub_vu32 %r2, %r2, %r1
	sub_sat_vi32 %r2, %r2, %r1

	! BUNDLE
	sub_sat_vu32 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end273:
	.size	test_builtin_vector_i32, .Lfunc_end273-test_builtin_vector_i32
	.size	.Ltest_builtin_vector_i32$local, .Lfunc_end273-test_builtin_vector_i32
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
	alloc 3
	addc_vu64 %r2, %r1, %r2
	addo_vi64 %r2, %r2, %r1

	! BUNDLE
	add_sat_vi64 %r2, %r2, %r1
	add_sat_vu64 %r2, %r2, %r1
	add_vu64 %r2, %r2, %r1

	! BUNDLE
	avg_vu64 %r2, %r2, %r1
	avg_vi64 %r2, %r2, %r1
	cmp_eq_vi64 %r2, %r2, %r1

	! BUNDLE
	cmp_lt_vi64 %r2, %r2, %r1
	cmp_lt_vu64 %r2, %r2, %r1
	max_vi64 %r2, %r2, %r1

	! BUNDLE
	max_vu64 %r2, %r2, %r1
	min_vi64 %r2, %r2, %r1
	min_vu64 %r2, %r2, %r1

	! BUNDLE
	rol_vu64 %r2, %r2, %r1
	ror_vu64 %r2, %r2, %r1
	sll_vu64 %r2, %r2, %r1

	! BUNDLE
	sra_vi64 %r2, %r2, %r1
	srl_vu64 %r2, %r2, %r1
	subb_vu64 %r2, %r2, %r1

	! BUNDLE
	subo_vi64 %r2, %r2, %r1
	sub_vu64 %r2, %r2, %r1
	sub_sat_vi64 %r2, %r2, %r1

	! BUNDLE
	sub_sat_vu64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end274:
	.size	test_builtin_vector_i64, .Lfunc_end274-test_builtin_vector_i64
	.size	.Ltest_builtin_vector_i64$local, .Lfunc_end274-test_builtin_vector_i64
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
	alloc 9
	merge_high_vu8 %r1, %r1, %r2
	merge_low_vu8 %r1, %r1, %r2

	! BUNDLE
	merge_high_vu16 %r1, %r1, %r4
	merge_low_vu16 %r1, %r1, %r4
	merge_high_vu32 %r1, %r1, %r6

	! BUNDLE
	merge_low_vu32 %r1, %r1, %r6
	merge_high_vu64 %r1, %r1, %r8
	merge_low_vu64 %r1, %r1, %r8

	! BUNDLE
	pack_sat_vi16 %r1, %r1, %r7
	pack_sat_vi32 %r1, %r1, %r5
	pack_sat_vi64 %r1, %r1, %r7

	! BUNDLE
	pack_usat_vi16 %r1, %r1, %r3
	pack_usat_vi32 %r1, %r1, %r6
	pack_usat_vi64 %r1, %r1, %r8

	! BUNDLE
	pack_mod_vu16 %r1, %r1, %r4
	pack_mod_vu32 %r1, %r1, %r6
	pack_mod_vu64 %r1, %r1, %r8

	! BUNDLE
	pack_sat_vu16 %r1, %r1, %r4
	pack_sat_vu32 %r1, %r1, %r6
	pack_sat_vu64 %r1, %r1, %r8

	! BUNDLE
	unpack_high_vi8 %r1, %r1
	unpack_high_vi16 %r1, %r1
	unpack_high_vi32 %r1, %r1

	! BUNDLE
	unpack_low_vi8 %r1, %r1
	unpack_low_vi16 %r1, %r1
	unpack_low_vi32 %r1, %r1

	! BUNDLE
	unpack_high_vu8 %r1, %r1
	unpack_high_vu16 %r1, %r1
	unpack_high_vu32 %r1, %r1

	! BUNDLE
	unpack_low_vu8 %r1, %r1
	unpack_low_vu16 %r1, %r1
	unpack_low_vu32 %r1, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end275:
	.size	test_builtin_vector_mixed, .Lfunc_end275-test_builtin_vector_mixed
	.size	.Ltest_builtin_vector_mixed$local, .Lfunc_end275-test_builtin_vector_mixed
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
	alloc 2
	cnt_pop %r1, %r1, 31
	retf 0

.Lfunc_end276:
	.size	test_builtin_popcount, .Lfunc_end276-test_builtin_popcount
	.size	.Ltest_builtin_popcount$local, .Lfunc_end276-test_builtin_popcount
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
	alloc 2
	cnt_pop %r1, %r1, 63
	retf 0

.Lfunc_end277:
	.size	test_builtin_popcountl, .Lfunc_end277-test_builtin_popcountl
	.size	.Ltest_builtin_popcountl$local, .Lfunc_end277-test_builtin_popcountl
                                        ! -- End function
	.globl	test_builtin_popcountll         ! -- Begin function test_builtin_popcountll
	.p2align	4
	.type	test_builtin_popcountll,@function
test_builtin_popcountll:                ! @test_builtin_popcountll
.Ltest_builtin_popcountll$local:
	.type	.Ltest_builtin_popcountll$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cnt_pop %r1, %r1, 63
	retf 0

.Lfunc_end278:
	.size	test_builtin_popcountll, .Lfunc_end278-test_builtin_popcountll
	.size	.Ltest_builtin_popcountll$local, .Lfunc_end278-test_builtin_popcountll
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
	alloc 2
	cnt_tz %r1, %r1, 31
	retf 0

.Lfunc_end279:
	.size	test_builtin_ctz, .Lfunc_end279-test_builtin_ctz
	.size	.Ltest_builtin_ctz$local, .Lfunc_end279-test_builtin_ctz
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
	alloc 2
	cnt_tz %r1, %r1, 63
	retf 0

.Lfunc_end280:
	.size	test_builtin_ctzl, .Lfunc_end280-test_builtin_ctzl
	.size	.Ltest_builtin_ctzl$local, .Lfunc_end280-test_builtin_ctzl
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
	alloc 2
	cnt_lz %r1, %r1, 31
	retf 0

.Lfunc_end281:
	.size	test_builtin_clz, .Lfunc_end281-test_builtin_clz
	.size	.Ltest_builtin_clz$local, .Lfunc_end281-test_builtin_clz
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
	alloc 2
	cnt_lz %r1, %r1, 63
	retf 0

.Lfunc_end282:
	.size	test_builtin_clzl, .Lfunc_end282-test_builtin_clzl
	.size	.Ltest_builtin_clzl$local, .Lfunc_end282-test_builtin_clzl
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
	alloc 2
	get_spr %r1, %itc
	retf 0

.Lfunc_end283:
	.size	test_builtin_readcyclecounter, .Lfunc_end283-test_builtin_readcyclecounter
	.size	.Ltest_builtin_readcyclecounter$local, .Lfunc_end283-test_builtin_readcyclecounter
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
	alloc 2
	mov %r1, %tp
	retf 0

.Lfunc_end284:
	.size	test_builtin_thread_pointer, .Lfunc_end284-test_builtin_thread_pointer
	.size	.Ltest_builtin_thread_pointer$local, .Lfunc_end284-test_builtin_thread_pointer
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
	alloc 2
	mov %tp, %r1
	retf 0

.Lfunc_end285:
	.size	test_builtin_set_thread_pointer, .Lfunc_end285-test_builtin_set_thread_pointer
	.size	.Ltest_builtin_set_thread_pointer$local, .Lfunc_end285-test_builtin_set_thread_pointer
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
	alloc_sp 3, 0
	mov %r2, %fp
	add_imm_i64 %fp, %sp, 0

	! BUNDLE
	add_i64 %r1, %r0, %fp
	mov %sp, %fp
	mov %fp, %r2

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end286:
	.size	test_frame_buitin, .Lfunc_end286-test_frame_buitin
	.size	.Ltest_frame_buitin$local, .Lfunc_end286-test_frame_buitin
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
	alloc 2
	mov %r1, %sp
	retf 0

.Lfunc_end287:
	.size	test_builtin_stack_pointer, .Lfunc_end287-test_builtin_stack_pointer
	.size	.Ltest_builtin_stack_pointer$local, .Lfunc_end287-test_builtin_stack_pointer
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
	alloc_sp 6, 16
	add_i32 %r1, %r2, %r1
	permb %r3, %r1, 31

	! BUNDLE
	lda_iprel %r5, %pcfwd(.L.str.1)
	st_i32 %r3, %sp, 0
	call %r4, %jpc(printf)

	! BUNDLE
	sra_imm_i32 %r1, %r2, 0
	add_imm_i64 %r1, %r1, 11
	permb %r2, %r1, 63

	! BUNDLE
	lda_iprel %r5, %pcfwd(.L.str.2)
	st_i64 %r2, %sp, 0
	call %r4, %jpc(printf)

	! BUNDLE
	lda_xi32 %r1, %r2, %r3, 0, 0
	retf 16
	nop 0

.Lfunc_end288:
	.size	test_builtin_bitreverse, .Lfunc_end288-test_builtin_bitreverse
	.size	.Ltest_builtin_bitreverse$local, .Lfunc_end288-test_builtin_bitreverse
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
	alloc 1
	halt
	retf 0

.Lfunc_end289:
	.size	test_builtin_halt, .Lfunc_end289-test_builtin_halt
	.size	.Ltest_builtin_halt$local, .Lfunc_end289-test_builtin_halt
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
	alloc 1
	undef
	retf 0

.Lfunc_end290:
	.size	test_builtin_undef, .Lfunc_end290-test_builtin_undef
	.size	.Ltest_builtin_undef$local, .Lfunc_end290-test_builtin_undef
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
	alloc 1
	rfi
	retf 0

.Lfunc_end291:
	.size	test_builtin_rfi, .Lfunc_end291-test_builtin_rfi
	.size	.Ltest_builtin_rfi$local, .Lfunc_end291-test_builtin_rfi
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
	alloc 1
	nop 1234567
	nop 2

	! BUNDLE
	nop.l 123456789123456789
	nop 123

	! BUNDLE
	nop.l 123
	retf 0

.Lfunc_end292:
	.size	test_builtin_nop, .Lfunc_end292-test_builtin_nop
	.size	.Ltest_builtin_nop$local, .Lfunc_end292-test_builtin_nop
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
	alloc 3
	tpa %r1, %r1, %r2
	retf 0

.Lfunc_end293:
	.size	test_builtin_tpa, .Lfunc_end293-test_builtin_tpa
	.size	.Ltest_builtin_tpa$local, .Lfunc_end293-test_builtin_tpa
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
	alloc 3
	random %r1, %r1
	random %r2, %gz

	! BUNDLE
	add_i64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end294:
	.size	test_builtin_random, .Lfunc_end294-test_builtin_random
	.size	.Ltest_builtin_random$local, .Lfunc_end294-test_builtin_random
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
	alloc 2
	dcbf %r1, 0
	dcbf %r1, 1000

	! BUNDLE
	dcbf.l %r1, 1000000000
	dcbt %r1, 0

	! BUNDLE
	dcbt %r1, 1000
	dcbt.l %r1, 1000000000

	! BUNDLE
	dcbi %r1, 0
	dcbi %r1, 1000
	nop 2

	! BUNDLE
	dcbi.l %r1, 1000000000
	icbi %r1, 0

	! BUNDLE
	icbi %r1, 1000
	icbi.l %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end295:
	.size	test_builtin_cache, .Lfunc_end295-test_builtin_cache
	.size	.Ltest_builtin_cache$local, .Lfunc_end295-test_builtin_cache
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
	alloc 4
	clmul_ll %r3, %r1, %r2
	clmul_hl %r1, %r1, %r3

	! BUNDLE
	clmul_hh %r1, %r1, %r2
	retf 0
	nop 0

.Lfunc_end296:
	.size	test_clmul, .Lfunc_end296-test_clmul
	.size	.Ltest_clmul$local, .Lfunc_end296-test_clmul
                                        ! -- End function
	.globl	cmp_eq_i32                      ! -- Begin function cmp_eq_i32
	.p2align	4
	.type	cmp_eq_i32,@function
cmp_eq_i32:                             ! @cmp_eq_i32
.Lcmp_eq_i32$local:
	.type	.Lcmp_eq_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_eq_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end297:
	.size	cmp_eq_i32, .Lfunc_end297-cmp_eq_i32
	.size	.Lcmp_eq_i32$local, .Lfunc_end297-cmp_eq_i32
                                        ! -- End function
	.globl	cmp_ne_i32                      ! -- Begin function cmp_ne_i32
	.p2align	4
	.type	cmp_ne_i32,@function
cmp_ne_i32:                             ! @cmp_ne_i32
.Lcmp_ne_i32$local:
	.type	.Lcmp_ne_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ne_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end298:
	.size	cmp_ne_i32, .Lfunc_end298-cmp_ne_i32
	.size	.Lcmp_ne_i32$local, .Lfunc_end298-cmp_ne_i32
                                        ! -- End function
	.globl	cmp_lt_i32                      ! -- Begin function cmp_lt_i32
	.p2align	4
	.type	cmp_lt_i32,@function
cmp_lt_i32:                             ! @cmp_lt_i32
.Lcmp_lt_i32$local:
	.type	.Lcmp_lt_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end299:
	.size	cmp_lt_i32, .Lfunc_end299-cmp_lt_i32
	.size	.Lcmp_lt_i32$local, .Lfunc_end299-cmp_lt_i32
                                        ! -- End function
	.globl	cmp_le_i32                      ! -- Begin function cmp_le_i32
	.p2align	4
	.type	cmp_le_i32,@function
cmp_le_i32:                             ! @cmp_le_i32
.Lcmp_le_i32$local:
	.type	.Lcmp_le_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_i32 %r1, %r2, %r1
	retf 0

.Lfunc_end300:
	.size	cmp_le_i32, .Lfunc_end300-cmp_le_i32
	.size	.Lcmp_le_i32$local, .Lfunc_end300-cmp_le_i32
                                        ! -- End function
	.globl	cmp_gt_i32                      ! -- Begin function cmp_gt_i32
	.p2align	4
	.type	cmp_gt_i32,@function
cmp_gt_i32:                             ! @cmp_gt_i32
.Lcmp_gt_i32$local:
	.type	.Lcmp_gt_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_i32 %r1, %r2, %r1
	retf 0

.Lfunc_end301:
	.size	cmp_gt_i32, .Lfunc_end301-cmp_gt_i32
	.size	.Lcmp_gt_i32$local, .Lfunc_end301-cmp_gt_i32
                                        ! -- End function
	.globl	cmp_ge_i32                      ! -- Begin function cmp_ge_i32
	.p2align	4
	.type	cmp_ge_i32,@function
cmp_ge_i32:                             ! @cmp_ge_i32
.Lcmp_ge_i32$local:
	.type	.Lcmp_ge_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end302:
	.size	cmp_ge_i32, .Lfunc_end302-cmp_ge_i32
	.size	.Lcmp_ge_i32$local, .Lfunc_end302-cmp_ge_i32
                                        ! -- End function
	.globl	cmp_lt_u32                      ! -- Begin function cmp_lt_u32
	.p2align	4
	.type	cmp_lt_u32,@function
cmp_lt_u32:                             ! @cmp_lt_u32
.Lcmp_lt_u32$local:
	.type	.Lcmp_lt_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end303:
	.size	cmp_lt_u32, .Lfunc_end303-cmp_lt_u32
	.size	.Lcmp_lt_u32$local, .Lfunc_end303-cmp_lt_u32
                                        ! -- End function
	.globl	cmp_le_u32                      ! -- Begin function cmp_le_u32
	.p2align	4
	.type	cmp_le_u32,@function
cmp_le_u32:                             ! @cmp_le_u32
.Lcmp_le_u32$local:
	.type	.Lcmp_le_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end304:
	.size	cmp_le_u32, .Lfunc_end304-cmp_le_u32
	.size	.Lcmp_le_u32$local, .Lfunc_end304-cmp_le_u32
                                        ! -- End function
	.globl	cmp_gt_u32                      ! -- Begin function cmp_gt_u32
	.p2align	4
	.type	cmp_gt_u32,@function
cmp_gt_u32:                             ! @cmp_gt_u32
.Lcmp_gt_u32$local:
	.type	.Lcmp_gt_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end305:
	.size	cmp_gt_u32, .Lfunc_end305-cmp_gt_u32
	.size	.Lcmp_gt_u32$local, .Lfunc_end305-cmp_gt_u32
                                        ! -- End function
	.globl	cmp_ge_u32                      ! -- Begin function cmp_ge_u32
	.p2align	4
	.type	cmp_ge_u32,@function
cmp_ge_u32:                             ! @cmp_ge_u32
.Lcmp_ge_u32$local:
	.type	.Lcmp_ge_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end306:
	.size	cmp_ge_u32, .Lfunc_end306-cmp_ge_u32
	.size	.Lcmp_ge_u32$local, .Lfunc_end306-cmp_ge_u32
                                        ! -- End function
	.globl	cmp_eq_imm_i32                  ! -- Begin function cmp_eq_imm_i32
	.p2align	4
	.type	cmp_eq_imm_i32,@function
cmp_eq_imm_i32:                         ! @cmp_eq_imm_i32
.Lcmp_eq_imm_i32$local:
	.type	.Lcmp_eq_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_eq_imm_i32 %r1, %r1, 123456
	retf 0

.Lfunc_end307:
	.size	cmp_eq_imm_i32, .Lfunc_end307-cmp_eq_imm_i32
	.size	.Lcmp_eq_imm_i32$local, .Lfunc_end307-cmp_eq_imm_i32
                                        ! -- End function
	.globl	cmp_ne_imm_i32                  ! -- Begin function cmp_ne_imm_i32
	.p2align	4
	.type	cmp_ne_imm_i32,@function
cmp_ne_imm_i32:                         ! @cmp_ne_imm_i32
.Lcmp_ne_imm_i32$local:
	.type	.Lcmp_ne_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ne_imm_i32 %r1, %r1, 123456
	retf 0

.Lfunc_end308:
	.size	cmp_ne_imm_i32, .Lfunc_end308-cmp_ne_imm_i32
	.size	.Lcmp_ne_imm_i32$local, .Lfunc_end308-cmp_ne_imm_i32
                                        ! -- End function
	.globl	cmp_lt_imm_i32                  ! -- Begin function cmp_lt_imm_i32
	.p2align	4
	.type	cmp_lt_imm_i32,@function
cmp_lt_imm_i32:                         ! @cmp_lt_imm_i32
.Lcmp_lt_imm_i32$local:
	.type	.Lcmp_lt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i32 %r1, %r1, 123456
	retf 0

.Lfunc_end309:
	.size	cmp_lt_imm_i32, .Lfunc_end309-cmp_lt_imm_i32
	.size	.Lcmp_lt_imm_i32$local, .Lfunc_end309-cmp_lt_imm_i32
                                        ! -- End function
	.globl	cmp_gt_imm_i32                  ! -- Begin function cmp_gt_imm_i32
	.p2align	4
	.type	cmp_gt_imm_i32,@function
cmp_gt_imm_i32:                         ! @cmp_gt_imm_i32
.Lcmp_gt_imm_i32$local:
	.type	.Lcmp_gt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i32 %r1, %r1, 123456
	retf 0

.Lfunc_end310:
	.size	cmp_gt_imm_i32, .Lfunc_end310-cmp_gt_imm_i32
	.size	.Lcmp_gt_imm_i32$local, .Lfunc_end310-cmp_gt_imm_i32
                                        ! -- End function
	.globl	cmp_le_imm_i32                  ! -- Begin function cmp_le_imm_i32
	.p2align	4
	.type	cmp_le_imm_i32,@function
cmp_le_imm_i32:                         ! @cmp_le_imm_i32
.Lcmp_le_imm_i32$local:
	.type	.Lcmp_le_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i32 %r1, %r1, 123456
	retf 0

.Lfunc_end311:
	.size	cmp_le_imm_i32, .Lfunc_end311-cmp_le_imm_i32
	.size	.Lcmp_le_imm_i32$local, .Lfunc_end311-cmp_le_imm_i32
                                        ! -- End function
	.globl	cmp_ge_imm_i32                  ! -- Begin function cmp_ge_imm_i32
	.p2align	4
	.type	cmp_ge_imm_i32,@function
cmp_ge_imm_i32:                         ! @cmp_ge_imm_i32
.Lcmp_ge_imm_i32$local:
	.type	.Lcmp_ge_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i32 %r1, %r1, 123456
	retf 0

.Lfunc_end312:
	.size	cmp_ge_imm_i32, .Lfunc_end312-cmp_ge_imm_i32
	.size	.Lcmp_ge_imm_i32$local, .Lfunc_end312-cmp_ge_imm_i32
                                        ! -- End function
	.globl	cmp_lt_imm_u32                  ! -- Begin function cmp_lt_imm_u32
	.p2align	4
	.type	cmp_lt_imm_u32,@function
cmp_lt_imm_u32:                         ! @cmp_lt_imm_u32
.Lcmp_lt_imm_u32$local:
	.type	.Lcmp_lt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u32 %r1, %r1, 123456
	retf 0

.Lfunc_end313:
	.size	cmp_lt_imm_u32, .Lfunc_end313-cmp_lt_imm_u32
	.size	.Lcmp_lt_imm_u32$local, .Lfunc_end313-cmp_lt_imm_u32
                                        ! -- End function
	.globl	cmp_gt_imm_u32                  ! -- Begin function cmp_gt_imm_u32
	.p2align	4
	.type	cmp_gt_imm_u32,@function
cmp_gt_imm_u32:                         ! @cmp_gt_imm_u32
.Lcmp_gt_imm_u32$local:
	.type	.Lcmp_gt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u32 %r1, %r1, 123456
	retf 0

.Lfunc_end314:
	.size	cmp_gt_imm_u32, .Lfunc_end314-cmp_gt_imm_u32
	.size	.Lcmp_gt_imm_u32$local, .Lfunc_end314-cmp_gt_imm_u32
                                        ! -- End function
	.globl	cmp_le_imm_u32                  ! -- Begin function cmp_le_imm_u32
	.p2align	4
	.type	cmp_le_imm_u32,@function
cmp_le_imm_u32:                         ! @cmp_le_imm_u32
.Lcmp_le_imm_u32$local:
	.type	.Lcmp_le_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u32 %r1, %r1, 123456
	retf 0

.Lfunc_end315:
	.size	cmp_le_imm_u32, .Lfunc_end315-cmp_le_imm_u32
	.size	.Lcmp_le_imm_u32$local, .Lfunc_end315-cmp_le_imm_u32
                                        ! -- End function
	.globl	cmp_ge_imm_u32                  ! -- Begin function cmp_ge_imm_u32
	.p2align	4
	.type	cmp_ge_imm_u32,@function
cmp_ge_imm_u32:                         ! @cmp_ge_imm_u32
.Lcmp_ge_imm_u32$local:
	.type	.Lcmp_ge_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u32 %r1, %r1, 123456
	retf 0

.Lfunc_end316:
	.size	cmp_ge_imm_u32, .Lfunc_end316-cmp_ge_imm_u32
	.size	.Lcmp_ge_imm_u32$local, .Lfunc_end316-cmp_ge_imm_u32
                                        ! -- End function
	.globl	cmp_eq_imm_i32_ext              ! -- Begin function cmp_eq_imm_i32_ext
	.p2align	4
	.type	cmp_eq_imm_i32_ext,@function
cmp_eq_imm_i32_ext:                     ! @cmp_eq_imm_i32_ext
.Lcmp_eq_imm_i32_ext$local:
	.type	.Lcmp_eq_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_eq_imm_i32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end317:
	.size	cmp_eq_imm_i32_ext, .Lfunc_end317-cmp_eq_imm_i32_ext
	.size	.Lcmp_eq_imm_i32_ext$local, .Lfunc_end317-cmp_eq_imm_i32_ext
                                        ! -- End function
	.globl	cmp_ne_imm_i32_ext              ! -- Begin function cmp_ne_imm_i32_ext
	.p2align	4
	.type	cmp_ne_imm_i32_ext,@function
cmp_ne_imm_i32_ext:                     ! @cmp_ne_imm_i32_ext
.Lcmp_ne_imm_i32_ext$local:
	.type	.Lcmp_ne_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ne_imm_i32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end318:
	.size	cmp_ne_imm_i32_ext, .Lfunc_end318-cmp_ne_imm_i32_ext
	.size	.Lcmp_ne_imm_i32_ext$local, .Lfunc_end318-cmp_ne_imm_i32_ext
                                        ! -- End function
	.globl	cmp_lt_imm_i32_ext              ! -- Begin function cmp_lt_imm_i32_ext
	.p2align	4
	.type	cmp_lt_imm_i32_ext,@function
cmp_lt_imm_i32_ext:                     ! @cmp_lt_imm_i32_ext
.Lcmp_lt_imm_i32_ext$local:
	.type	.Lcmp_lt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end319:
	.size	cmp_lt_imm_i32_ext, .Lfunc_end319-cmp_lt_imm_i32_ext
	.size	.Lcmp_lt_imm_i32_ext$local, .Lfunc_end319-cmp_lt_imm_i32_ext
                                        ! -- End function
	.globl	cmp_gt_imm_i32_ext              ! -- Begin function cmp_gt_imm_i32_ext
	.p2align	4
	.type	cmp_gt_imm_i32_ext,@function
cmp_gt_imm_i32_ext:                     ! @cmp_gt_imm_i32_ext
.Lcmp_gt_imm_i32_ext$local:
	.type	.Lcmp_gt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end320:
	.size	cmp_gt_imm_i32_ext, .Lfunc_end320-cmp_gt_imm_i32_ext
	.size	.Lcmp_gt_imm_i32_ext$local, .Lfunc_end320-cmp_gt_imm_i32_ext
                                        ! -- End function
	.globl	cmp_le_imm_i32_ext              ! -- Begin function cmp_le_imm_i32_ext
	.p2align	4
	.type	cmp_le_imm_i32_ext,@function
cmp_le_imm_i32_ext:                     ! @cmp_le_imm_i32_ext
.Lcmp_le_imm_i32_ext$local:
	.type	.Lcmp_le_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end321:
	.size	cmp_le_imm_i32_ext, .Lfunc_end321-cmp_le_imm_i32_ext
	.size	.Lcmp_le_imm_i32_ext$local, .Lfunc_end321-cmp_le_imm_i32_ext
                                        ! -- End function
	.globl	cmp_ge_imm_i32_ext              ! -- Begin function cmp_ge_imm_i32_ext
	.p2align	4
	.type	cmp_ge_imm_i32_ext,@function
cmp_ge_imm_i32_ext:                     ! @cmp_ge_imm_i32_ext
.Lcmp_ge_imm_i32_ext$local:
	.type	.Lcmp_ge_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end322:
	.size	cmp_ge_imm_i32_ext, .Lfunc_end322-cmp_ge_imm_i32_ext
	.size	.Lcmp_ge_imm_i32_ext$local, .Lfunc_end322-cmp_ge_imm_i32_ext
                                        ! -- End function
	.globl	cmp_lt_imm_u32_ext              ! -- Begin function cmp_lt_imm_u32_ext
	.p2align	4
	.type	cmp_lt_imm_u32_ext,@function
cmp_lt_imm_u32_ext:                     ! @cmp_lt_imm_u32_ext
.Lcmp_lt_imm_u32_ext$local:
	.type	.Lcmp_lt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end323:
	.size	cmp_lt_imm_u32_ext, .Lfunc_end323-cmp_lt_imm_u32_ext
	.size	.Lcmp_lt_imm_u32_ext$local, .Lfunc_end323-cmp_lt_imm_u32_ext
                                        ! -- End function
	.globl	cmp_gt_imm_u32_ext              ! -- Begin function cmp_gt_imm_u32_ext
	.p2align	4
	.type	cmp_gt_imm_u32_ext,@function
cmp_gt_imm_u32_ext:                     ! @cmp_gt_imm_u32_ext
.Lcmp_gt_imm_u32_ext$local:
	.type	.Lcmp_gt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end324:
	.size	cmp_gt_imm_u32_ext, .Lfunc_end324-cmp_gt_imm_u32_ext
	.size	.Lcmp_gt_imm_u32_ext$local, .Lfunc_end324-cmp_gt_imm_u32_ext
                                        ! -- End function
	.globl	cmp_le_imm_u32_ext              ! -- Begin function cmp_le_imm_u32_ext
	.p2align	4
	.type	cmp_le_imm_u32_ext,@function
cmp_le_imm_u32_ext:                     ! @cmp_le_imm_u32_ext
.Lcmp_le_imm_u32_ext$local:
	.type	.Lcmp_le_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end325:
	.size	cmp_le_imm_u32_ext, .Lfunc_end325-cmp_le_imm_u32_ext
	.size	.Lcmp_le_imm_u32_ext$local, .Lfunc_end325-cmp_le_imm_u32_ext
                                        ! -- End function
	.globl	cmp_ge_imm_u32_ext              ! -- Begin function cmp_ge_imm_u32_ext
	.p2align	4
	.type	cmp_ge_imm_u32_ext,@function
cmp_ge_imm_u32_ext:                     ! @cmp_ge_imm_u32_ext
.Lcmp_ge_imm_u32_ext$local:
	.type	.Lcmp_ge_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u32.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end326:
	.size	cmp_ge_imm_u32_ext, .Lfunc_end326-cmp_ge_imm_u32_ext
	.size	.Lcmp_ge_imm_u32_ext$local, .Lfunc_end326-cmp_ge_imm_u32_ext
                                        ! -- End function
	.globl	cmp_eq_i64                      ! -- Begin function cmp_eq_i64
	.p2align	4
	.type	cmp_eq_i64,@function
cmp_eq_i64:                             ! @cmp_eq_i64
.Lcmp_eq_i64$local:
	.type	.Lcmp_eq_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_eq_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end327:
	.size	cmp_eq_i64, .Lfunc_end327-cmp_eq_i64
	.size	.Lcmp_eq_i64$local, .Lfunc_end327-cmp_eq_i64
                                        ! -- End function
	.globl	cmp_ne_i64                      ! -- Begin function cmp_ne_i64
	.p2align	4
	.type	cmp_ne_i64,@function
cmp_ne_i64:                             ! @cmp_ne_i64
.Lcmp_ne_i64$local:
	.type	.Lcmp_ne_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ne_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end328:
	.size	cmp_ne_i64, .Lfunc_end328-cmp_ne_i64
	.size	.Lcmp_ne_i64$local, .Lfunc_end328-cmp_ne_i64
                                        ! -- End function
	.globl	cmp_lt_i64                      ! -- Begin function cmp_lt_i64
	.p2align	4
	.type	cmp_lt_i64,@function
cmp_lt_i64:                             ! @cmp_lt_i64
.Lcmp_lt_i64$local:
	.type	.Lcmp_lt_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end329:
	.size	cmp_lt_i64, .Lfunc_end329-cmp_lt_i64
	.size	.Lcmp_lt_i64$local, .Lfunc_end329-cmp_lt_i64
                                        ! -- End function
	.globl	cmp_le_i64                      ! -- Begin function cmp_le_i64
	.p2align	4
	.type	cmp_le_i64,@function
cmp_le_i64:                             ! @cmp_le_i64
.Lcmp_le_i64$local:
	.type	.Lcmp_le_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end330:
	.size	cmp_le_i64, .Lfunc_end330-cmp_le_i64
	.size	.Lcmp_le_i64$local, .Lfunc_end330-cmp_le_i64
                                        ! -- End function
	.globl	cmp_gt_i64                      ! -- Begin function cmp_gt_i64
	.p2align	4
	.type	cmp_gt_i64,@function
cmp_gt_i64:                             ! @cmp_gt_i64
.Lcmp_gt_i64$local:
	.type	.Lcmp_gt_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end331:
	.size	cmp_gt_i64, .Lfunc_end331-cmp_gt_i64
	.size	.Lcmp_gt_i64$local, .Lfunc_end331-cmp_gt_i64
                                        ! -- End function
	.globl	cmp_ge_i64                      ! -- Begin function cmp_ge_i64
	.p2align	4
	.type	cmp_ge_i64,@function
cmp_ge_i64:                             ! @cmp_ge_i64
.Lcmp_ge_i64$local:
	.type	.Lcmp_ge_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end332:
	.size	cmp_ge_i64, .Lfunc_end332-cmp_ge_i64
	.size	.Lcmp_ge_i64$local, .Lfunc_end332-cmp_ge_i64
                                        ! -- End function
	.globl	cmp_lt_u64                      ! -- Begin function cmp_lt_u64
	.p2align	4
	.type	cmp_lt_u64,@function
cmp_lt_u64:                             ! @cmp_lt_u64
.Lcmp_lt_u64$local:
	.type	.Lcmp_lt_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end333:
	.size	cmp_lt_u64, .Lfunc_end333-cmp_lt_u64
	.size	.Lcmp_lt_u64$local, .Lfunc_end333-cmp_lt_u64
                                        ! -- End function
	.globl	cmp_le_u64                      ! -- Begin function cmp_le_u64
	.p2align	4
	.type	cmp_le_u64,@function
cmp_le_u64:                             ! @cmp_le_u64
.Lcmp_le_u64$local:
	.type	.Lcmp_le_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end334:
	.size	cmp_le_u64, .Lfunc_end334-cmp_le_u64
	.size	.Lcmp_le_u64$local, .Lfunc_end334-cmp_le_u64
                                        ! -- End function
	.globl	cmp_gt_u64                      ! -- Begin function cmp_gt_u64
	.p2align	4
	.type	cmp_gt_u64,@function
cmp_gt_u64:                             ! @cmp_gt_u64
.Lcmp_gt_u64$local:
	.type	.Lcmp_gt_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end335:
	.size	cmp_gt_u64, .Lfunc_end335-cmp_gt_u64
	.size	.Lcmp_gt_u64$local, .Lfunc_end335-cmp_gt_u64
                                        ! -- End function
	.globl	cmp_ge_u64                      ! -- Begin function cmp_ge_u64
	.p2align	4
	.type	cmp_ge_u64,@function
cmp_ge_u64:                             ! @cmp_ge_u64
.Lcmp_ge_u64$local:
	.type	.Lcmp_ge_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end336:
	.size	cmp_ge_u64, .Lfunc_end336-cmp_ge_u64
	.size	.Lcmp_ge_u64$local, .Lfunc_end336-cmp_ge_u64
                                        ! -- End function
	.globl	cmp_eq_imm_i64                  ! -- Begin function cmp_eq_imm_i64
	.p2align	4
	.type	cmp_eq_imm_i64,@function
cmp_eq_imm_i64:                         ! @cmp_eq_imm_i64
.Lcmp_eq_imm_i64$local:
	.type	.Lcmp_eq_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_eq_imm_i64 %r1, %r1, 123456
	retf 0

.Lfunc_end337:
	.size	cmp_eq_imm_i64, .Lfunc_end337-cmp_eq_imm_i64
	.size	.Lcmp_eq_imm_i64$local, .Lfunc_end337-cmp_eq_imm_i64
                                        ! -- End function
	.globl	cmp_ne_imm_i64                  ! -- Begin function cmp_ne_imm_i64
	.p2align	4
	.type	cmp_ne_imm_i64,@function
cmp_ne_imm_i64:                         ! @cmp_ne_imm_i64
.Lcmp_ne_imm_i64$local:
	.type	.Lcmp_ne_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ne_imm_i64 %r1, %r1, 123456
	retf 0

.Lfunc_end338:
	.size	cmp_ne_imm_i64, .Lfunc_end338-cmp_ne_imm_i64
	.size	.Lcmp_ne_imm_i64$local, .Lfunc_end338-cmp_ne_imm_i64
                                        ! -- End function
	.globl	cmp_lt_imm_i64                  ! -- Begin function cmp_lt_imm_i64
	.p2align	4
	.type	cmp_lt_imm_i64,@function
cmp_lt_imm_i64:                         ! @cmp_lt_imm_i64
.Lcmp_lt_imm_i64$local:
	.type	.Lcmp_lt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i64 %r1, %r1, 123456
	retf 0

.Lfunc_end339:
	.size	cmp_lt_imm_i64, .Lfunc_end339-cmp_lt_imm_i64
	.size	.Lcmp_lt_imm_i64$local, .Lfunc_end339-cmp_lt_imm_i64
                                        ! -- End function
	.globl	cmp_gt_imm_i64                  ! -- Begin function cmp_gt_imm_i64
	.p2align	4
	.type	cmp_gt_imm_i64,@function
cmp_gt_imm_i64:                         ! @cmp_gt_imm_i64
.Lcmp_gt_imm_i64$local:
	.type	.Lcmp_gt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i64 %r1, %r1, 123456
	retf 0

.Lfunc_end340:
	.size	cmp_gt_imm_i64, .Lfunc_end340-cmp_gt_imm_i64
	.size	.Lcmp_gt_imm_i64$local, .Lfunc_end340-cmp_gt_imm_i64
                                        ! -- End function
	.globl	cmp_le_imm_i64                  ! -- Begin function cmp_le_imm_i64
	.p2align	4
	.type	cmp_le_imm_i64,@function
cmp_le_imm_i64:                         ! @cmp_le_imm_i64
.Lcmp_le_imm_i64$local:
	.type	.Lcmp_le_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i64 %r1, %r1, 123456
	retf 0

.Lfunc_end341:
	.size	cmp_le_imm_i64, .Lfunc_end341-cmp_le_imm_i64
	.size	.Lcmp_le_imm_i64$local, .Lfunc_end341-cmp_le_imm_i64
                                        ! -- End function
	.globl	cmp_ge_imm_i64                  ! -- Begin function cmp_ge_imm_i64
	.p2align	4
	.type	cmp_ge_imm_i64,@function
cmp_ge_imm_i64:                         ! @cmp_ge_imm_i64
.Lcmp_ge_imm_i64$local:
	.type	.Lcmp_ge_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i64 %r1, %r1, 123456
	retf 0

.Lfunc_end342:
	.size	cmp_ge_imm_i64, .Lfunc_end342-cmp_ge_imm_i64
	.size	.Lcmp_ge_imm_i64$local, .Lfunc_end342-cmp_ge_imm_i64
                                        ! -- End function
	.globl	cmp_lt_imm_u64                  ! -- Begin function cmp_lt_imm_u64
	.p2align	4
	.type	cmp_lt_imm_u64,@function
cmp_lt_imm_u64:                         ! @cmp_lt_imm_u64
.Lcmp_lt_imm_u64$local:
	.type	.Lcmp_lt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u64 %r1, %r1, 123456
	retf 0

.Lfunc_end343:
	.size	cmp_lt_imm_u64, .Lfunc_end343-cmp_lt_imm_u64
	.size	.Lcmp_lt_imm_u64$local, .Lfunc_end343-cmp_lt_imm_u64
                                        ! -- End function
	.globl	cmp_gt_imm_u64                  ! -- Begin function cmp_gt_imm_u64
	.p2align	4
	.type	cmp_gt_imm_u64,@function
cmp_gt_imm_u64:                         ! @cmp_gt_imm_u64
.Lcmp_gt_imm_u64$local:
	.type	.Lcmp_gt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u64 %r1, %r1, 123456
	retf 0

.Lfunc_end344:
	.size	cmp_gt_imm_u64, .Lfunc_end344-cmp_gt_imm_u64
	.size	.Lcmp_gt_imm_u64$local, .Lfunc_end344-cmp_gt_imm_u64
                                        ! -- End function
	.globl	cmp_le_imm_u64                  ! -- Begin function cmp_le_imm_u64
	.p2align	4
	.type	cmp_le_imm_u64,@function
cmp_le_imm_u64:                         ! @cmp_le_imm_u64
.Lcmp_le_imm_u64$local:
	.type	.Lcmp_le_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u64 %r1, %r1, 123456
	retf 0

.Lfunc_end345:
	.size	cmp_le_imm_u64, .Lfunc_end345-cmp_le_imm_u64
	.size	.Lcmp_le_imm_u64$local, .Lfunc_end345-cmp_le_imm_u64
                                        ! -- End function
	.globl	cmp_ge_imm_u64                  ! -- Begin function cmp_ge_imm_u64
	.p2align	4
	.type	cmp_ge_imm_u64,@function
cmp_ge_imm_u64:                         ! @cmp_ge_imm_u64
.Lcmp_ge_imm_u64$local:
	.type	.Lcmp_ge_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u64 %r1, %r1, 123456
	retf 0

.Lfunc_end346:
	.size	cmp_ge_imm_u64, .Lfunc_end346-cmp_ge_imm_u64
	.size	.Lcmp_ge_imm_u64$local, .Lfunc_end346-cmp_ge_imm_u64
                                        ! -- End function
	.globl	cmp_eq_imm_i64_ext              ! -- Begin function cmp_eq_imm_i64_ext
	.p2align	4
	.type	cmp_eq_imm_i64_ext,@function
cmp_eq_imm_i64_ext:                     ! @cmp_eq_imm_i64_ext
.Lcmp_eq_imm_i64_ext$local:
	.type	.Lcmp_eq_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_eq_imm_i64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end347:
	.size	cmp_eq_imm_i64_ext, .Lfunc_end347-cmp_eq_imm_i64_ext
	.size	.Lcmp_eq_imm_i64_ext$local, .Lfunc_end347-cmp_eq_imm_i64_ext
                                        ! -- End function
	.globl	cmp_ne_imm_i64_ext              ! -- Begin function cmp_ne_imm_i64_ext
	.p2align	4
	.type	cmp_ne_imm_i64_ext,@function
cmp_ne_imm_i64_ext:                     ! @cmp_ne_imm_i64_ext
.Lcmp_ne_imm_i64_ext$local:
	.type	.Lcmp_ne_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ne_imm_i64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end348:
	.size	cmp_ne_imm_i64_ext, .Lfunc_end348-cmp_ne_imm_i64_ext
	.size	.Lcmp_ne_imm_i64_ext$local, .Lfunc_end348-cmp_ne_imm_i64_ext
                                        ! -- End function
	.globl	cmp_lt_imm_i64_ext              ! -- Begin function cmp_lt_imm_i64_ext
	.p2align	4
	.type	cmp_lt_imm_i64_ext,@function
cmp_lt_imm_i64_ext:                     ! @cmp_lt_imm_i64_ext
.Lcmp_lt_imm_i64_ext$local:
	.type	.Lcmp_lt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end349:
	.size	cmp_lt_imm_i64_ext, .Lfunc_end349-cmp_lt_imm_i64_ext
	.size	.Lcmp_lt_imm_i64_ext$local, .Lfunc_end349-cmp_lt_imm_i64_ext
                                        ! -- End function
	.globl	cmp_gt_imm_i64_ext              ! -- Begin function cmp_gt_imm_i64_ext
	.p2align	4
	.type	cmp_gt_imm_i64_ext,@function
cmp_gt_imm_i64_ext:                     ! @cmp_gt_imm_i64_ext
.Lcmp_gt_imm_i64_ext$local:
	.type	.Lcmp_gt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end350:
	.size	cmp_gt_imm_i64_ext, .Lfunc_end350-cmp_gt_imm_i64_ext
	.size	.Lcmp_gt_imm_i64_ext$local, .Lfunc_end350-cmp_gt_imm_i64_ext
                                        ! -- End function
	.globl	cmp_le_imm_i64_ext              ! -- Begin function cmp_le_imm_i64_ext
	.p2align	4
	.type	cmp_le_imm_i64_ext,@function
cmp_le_imm_i64_ext:                     ! @cmp_le_imm_i64_ext
.Lcmp_le_imm_i64_ext$local:
	.type	.Lcmp_le_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end351:
	.size	cmp_le_imm_i64_ext, .Lfunc_end351-cmp_le_imm_i64_ext
	.size	.Lcmp_le_imm_i64_ext$local, .Lfunc_end351-cmp_le_imm_i64_ext
                                        ! -- End function
	.globl	cmp_ge_imm_i64_ext              ! -- Begin function cmp_ge_imm_i64_ext
	.p2align	4
	.type	cmp_ge_imm_i64_ext,@function
cmp_ge_imm_i64_ext:                     ! @cmp_ge_imm_i64_ext
.Lcmp_ge_imm_i64_ext$local:
	.type	.Lcmp_ge_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end352:
	.size	cmp_ge_imm_i64_ext, .Lfunc_end352-cmp_ge_imm_i64_ext
	.size	.Lcmp_ge_imm_i64_ext$local, .Lfunc_end352-cmp_ge_imm_i64_ext
                                        ! -- End function
	.globl	cmp_lt_imm_u64_ext              ! -- Begin function cmp_lt_imm_u64_ext
	.p2align	4
	.type	cmp_lt_imm_u64_ext,@function
cmp_lt_imm_u64_ext:                     ! @cmp_lt_imm_u64_ext
.Lcmp_lt_imm_u64_ext$local:
	.type	.Lcmp_lt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end353:
	.size	cmp_lt_imm_u64_ext, .Lfunc_end353-cmp_lt_imm_u64_ext
	.size	.Lcmp_lt_imm_u64_ext$local, .Lfunc_end353-cmp_lt_imm_u64_ext
                                        ! -- End function
	.globl	cmp_gt_imm_u64_ext              ! -- Begin function cmp_gt_imm_u64_ext
	.p2align	4
	.type	cmp_gt_imm_u64_ext,@function
cmp_gt_imm_u64_ext:                     ! @cmp_gt_imm_u64_ext
.Lcmp_gt_imm_u64_ext$local:
	.type	.Lcmp_gt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end354:
	.size	cmp_gt_imm_u64_ext, .Lfunc_end354-cmp_gt_imm_u64_ext
	.size	.Lcmp_gt_imm_u64_ext$local, .Lfunc_end354-cmp_gt_imm_u64_ext
                                        ! -- End function
	.globl	cmp_le_imm_u64_ext              ! -- Begin function cmp_le_imm_u64_ext
	.p2align	4
	.type	cmp_le_imm_u64_ext,@function
cmp_le_imm_u64_ext:                     ! @cmp_le_imm_u64_ext
.Lcmp_le_imm_u64_ext$local:
	.type	.Lcmp_le_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end355:
	.size	cmp_le_imm_u64_ext, .Lfunc_end355-cmp_le_imm_u64_ext
	.size	.Lcmp_le_imm_u64_ext$local, .Lfunc_end355-cmp_le_imm_u64_ext
                                        ! -- End function
	.globl	cmp_ge_imm_u64_ext              ! -- Begin function cmp_ge_imm_u64_ext
	.p2align	4
	.type	cmp_ge_imm_u64_ext,@function
cmp_ge_imm_u64_ext:                     ! @cmp_ge_imm_u64_ext
.Lcmp_ge_imm_u64_ext$local:
	.type	.Lcmp_ge_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u64.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end356:
	.size	cmp_ge_imm_u64_ext, .Lfunc_end356-cmp_ge_imm_u64_ext
	.size	.Lcmp_ge_imm_u64_ext$local, .Lfunc_end356-cmp_ge_imm_u64_ext
                                        ! -- End function
	.globl	cmp_eq_i128                     ! -- Begin function cmp_eq_i128
	.p2align	4
	.type	cmp_eq_i128,@function
cmp_eq_i128:                            ! @cmp_eq_i128
.Lcmp_eq_i128$local:
	.type	.Lcmp_eq_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_eq_i128 %r1, %r1, %r2
	retf 0

.Lfunc_end357:
	.size	cmp_eq_i128, .Lfunc_end357-cmp_eq_i128
	.size	.Lcmp_eq_i128$local, .Lfunc_end357-cmp_eq_i128
                                        ! -- End function
	.globl	cmp_ne_i128                     ! -- Begin function cmp_ne_i128
	.p2align	4
	.type	cmp_ne_i128,@function
cmp_ne_i128:                            ! @cmp_ne_i128
.Lcmp_ne_i128$local:
	.type	.Lcmp_ne_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ne_i128 %r1, %r1, %r2
	retf 0

.Lfunc_end358:
	.size	cmp_ne_i128, .Lfunc_end358-cmp_ne_i128
	.size	.Lcmp_ne_i128$local, .Lfunc_end358-cmp_ne_i128
                                        ! -- End function
	.globl	cmp_lt_i128                     ! -- Begin function cmp_lt_i128
	.p2align	4
	.type	cmp_lt_i128,@function
cmp_lt_i128:                            ! @cmp_lt_i128
.Lcmp_lt_i128$local:
	.type	.Lcmp_lt_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_i128 %r1, %r1, %r2
	retf 0

.Lfunc_end359:
	.size	cmp_lt_i128, .Lfunc_end359-cmp_lt_i128
	.size	.Lcmp_lt_i128$local, .Lfunc_end359-cmp_lt_i128
                                        ! -- End function
	.globl	cmp_le_i128                     ! -- Begin function cmp_le_i128
	.p2align	4
	.type	cmp_le_i128,@function
cmp_le_i128:                            ! @cmp_le_i128
.Lcmp_le_i128$local:
	.type	.Lcmp_le_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_i128 %r1, %r2, %r1
	retf 0

.Lfunc_end360:
	.size	cmp_le_i128, .Lfunc_end360-cmp_le_i128
	.size	.Lcmp_le_i128$local, .Lfunc_end360-cmp_le_i128
                                        ! -- End function
	.globl	cmp_gt_i128                     ! -- Begin function cmp_gt_i128
	.p2align	4
	.type	cmp_gt_i128,@function
cmp_gt_i128:                            ! @cmp_gt_i128
.Lcmp_gt_i128$local:
	.type	.Lcmp_gt_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_i128 %r1, %r2, %r1
	retf 0

.Lfunc_end361:
	.size	cmp_gt_i128, .Lfunc_end361-cmp_gt_i128
	.size	.Lcmp_gt_i128$local, .Lfunc_end361-cmp_gt_i128
                                        ! -- End function
	.globl	cmp_ge_i128                     ! -- Begin function cmp_ge_i128
	.p2align	4
	.type	cmp_ge_i128,@function
cmp_ge_i128:                            ! @cmp_ge_i128
.Lcmp_ge_i128$local:
	.type	.Lcmp_ge_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_i128 %r1, %r1, %r2
	retf 0

.Lfunc_end362:
	.size	cmp_ge_i128, .Lfunc_end362-cmp_ge_i128
	.size	.Lcmp_ge_i128$local, .Lfunc_end362-cmp_ge_i128
                                        ! -- End function
	.globl	cmp_lt_u128                     ! -- Begin function cmp_lt_u128
	.p2align	4
	.type	cmp_lt_u128,@function
cmp_lt_u128:                            ! @cmp_lt_u128
.Lcmp_lt_u128$local:
	.type	.Lcmp_lt_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_u128 %r1, %r1, %r2
	retf 0

.Lfunc_end363:
	.size	cmp_lt_u128, .Lfunc_end363-cmp_lt_u128
	.size	.Lcmp_lt_u128$local, .Lfunc_end363-cmp_lt_u128
                                        ! -- End function
	.globl	cmp_le_u128                     ! -- Begin function cmp_le_u128
	.p2align	4
	.type	cmp_le_u128,@function
cmp_le_u128:                            ! @cmp_le_u128
.Lcmp_le_u128$local:
	.type	.Lcmp_le_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_u128 %r1, %r2, %r1
	retf 0

.Lfunc_end364:
	.size	cmp_le_u128, .Lfunc_end364-cmp_le_u128
	.size	.Lcmp_le_u128$local, .Lfunc_end364-cmp_le_u128
                                        ! -- End function
	.globl	cmp_gt_u128                     ! -- Begin function cmp_gt_u128
	.p2align	4
	.type	cmp_gt_u128,@function
cmp_gt_u128:                            ! @cmp_gt_u128
.Lcmp_gt_u128$local:
	.type	.Lcmp_gt_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_lt_u128 %r1, %r2, %r1
	retf 0

.Lfunc_end365:
	.size	cmp_gt_u128, .Lfunc_end365-cmp_gt_u128
	.size	.Lcmp_gt_u128$local, .Lfunc_end365-cmp_gt_u128
                                        ! -- End function
	.globl	cmp_ge_u128                     ! -- Begin function cmp_ge_u128
	.p2align	4
	.type	cmp_ge_u128,@function
cmp_ge_u128:                            ! @cmp_ge_u128
.Lcmp_ge_u128$local:
	.type	.Lcmp_ge_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	cmp_ge_u128 %r1, %r1, %r2
	retf 0

.Lfunc_end366:
	.size	cmp_ge_u128, .Lfunc_end366-cmp_ge_u128
	.size	.Lcmp_ge_u128$local, .Lfunc_end366-cmp_ge_u128
                                        ! -- End function
	.globl	cmp_eq_imm_i128                 ! -- Begin function cmp_eq_imm_i128
	.p2align	4
	.type	cmp_eq_imm_i128,@function
cmp_eq_imm_i128:                        ! @cmp_eq_imm_i128
.Lcmp_eq_imm_i128$local:
	.type	.Lcmp_eq_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_eq_imm_i128 %r1, %r1, 123456
	retf 0

.Lfunc_end367:
	.size	cmp_eq_imm_i128, .Lfunc_end367-cmp_eq_imm_i128
	.size	.Lcmp_eq_imm_i128$local, .Lfunc_end367-cmp_eq_imm_i128
                                        ! -- End function
	.globl	cmp_ne_imm_i128                 ! -- Begin function cmp_ne_imm_i128
	.p2align	4
	.type	cmp_ne_imm_i128,@function
cmp_ne_imm_i128:                        ! @cmp_ne_imm_i128
.Lcmp_ne_imm_i128$local:
	.type	.Lcmp_ne_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ne_imm_i128 %r1, %r1, 123456
	retf 0

.Lfunc_end368:
	.size	cmp_ne_imm_i128, .Lfunc_end368-cmp_ne_imm_i128
	.size	.Lcmp_ne_imm_i128$local, .Lfunc_end368-cmp_ne_imm_i128
                                        ! -- End function
	.globl	cmp_lt_imm_i128                 ! -- Begin function cmp_lt_imm_i128
	.p2align	4
	.type	cmp_lt_imm_i128,@function
cmp_lt_imm_i128:                        ! @cmp_lt_imm_i128
.Lcmp_lt_imm_i128$local:
	.type	.Lcmp_lt_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i128 %r1, %r1, 123456
	retf 0

.Lfunc_end369:
	.size	cmp_lt_imm_i128, .Lfunc_end369-cmp_lt_imm_i128
	.size	.Lcmp_lt_imm_i128$local, .Lfunc_end369-cmp_lt_imm_i128
                                        ! -- End function
	.globl	cmp_gt_imm_i128                 ! -- Begin function cmp_gt_imm_i128
	.p2align	4
	.type	cmp_gt_imm_i128,@function
cmp_gt_imm_i128:                        ! @cmp_gt_imm_i128
.Lcmp_gt_imm_i128$local:
	.type	.Lcmp_gt_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i128 %r1, %r1, 123456
	retf 0

.Lfunc_end370:
	.size	cmp_gt_imm_i128, .Lfunc_end370-cmp_gt_imm_i128
	.size	.Lcmp_gt_imm_i128$local, .Lfunc_end370-cmp_gt_imm_i128
                                        ! -- End function
	.globl	cmp_le_imm_i128                 ! -- Begin function cmp_le_imm_i128
	.p2align	4
	.type	cmp_le_imm_i128,@function
cmp_le_imm_i128:                        ! @cmp_le_imm_i128
.Lcmp_le_imm_i128$local:
	.type	.Lcmp_le_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i128 %r1, %r1, 123456
	retf 0

.Lfunc_end371:
	.size	cmp_le_imm_i128, .Lfunc_end371-cmp_le_imm_i128
	.size	.Lcmp_le_imm_i128$local, .Lfunc_end371-cmp_le_imm_i128
                                        ! -- End function
	.globl	cmp_ge_imm_i128                 ! -- Begin function cmp_ge_imm_i128
	.p2align	4
	.type	cmp_ge_imm_i128,@function
cmp_ge_imm_i128:                        ! @cmp_ge_imm_i128
.Lcmp_ge_imm_i128$local:
	.type	.Lcmp_ge_imm_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i128 %r1, %r1, 123456
	retf 0

.Lfunc_end372:
	.size	cmp_ge_imm_i128, .Lfunc_end372-cmp_ge_imm_i128
	.size	.Lcmp_ge_imm_i128$local, .Lfunc_end372-cmp_ge_imm_i128
                                        ! -- End function
	.globl	cmp_lt_imm_u128                 ! -- Begin function cmp_lt_imm_u128
	.p2align	4
	.type	cmp_lt_imm_u128,@function
cmp_lt_imm_u128:                        ! @cmp_lt_imm_u128
.Lcmp_lt_imm_u128$local:
	.type	.Lcmp_lt_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u128 %r1, %r1, 123456
	retf 0

.Lfunc_end373:
	.size	cmp_lt_imm_u128, .Lfunc_end373-cmp_lt_imm_u128
	.size	.Lcmp_lt_imm_u128$local, .Lfunc_end373-cmp_lt_imm_u128
                                        ! -- End function
	.globl	cmp_gt_imm_u128                 ! -- Begin function cmp_gt_imm_u128
	.p2align	4
	.type	cmp_gt_imm_u128,@function
cmp_gt_imm_u128:                        ! @cmp_gt_imm_u128
.Lcmp_gt_imm_u128$local:
	.type	.Lcmp_gt_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u128 %r1, %r1, 123456
	retf 0

.Lfunc_end374:
	.size	cmp_gt_imm_u128, .Lfunc_end374-cmp_gt_imm_u128
	.size	.Lcmp_gt_imm_u128$local, .Lfunc_end374-cmp_gt_imm_u128
                                        ! -- End function
	.globl	cmp_le_imm_u128                 ! -- Begin function cmp_le_imm_u128
	.p2align	4
	.type	cmp_le_imm_u128,@function
cmp_le_imm_u128:                        ! @cmp_le_imm_u128
.Lcmp_le_imm_u128$local:
	.type	.Lcmp_le_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u128 %r1, %r1, 123456
	retf 0

.Lfunc_end375:
	.size	cmp_le_imm_u128, .Lfunc_end375-cmp_le_imm_u128
	.size	.Lcmp_le_imm_u128$local, .Lfunc_end375-cmp_le_imm_u128
                                        ! -- End function
	.globl	cmp_ge_imm_u128                 ! -- Begin function cmp_ge_imm_u128
	.p2align	4
	.type	cmp_ge_imm_u128,@function
cmp_ge_imm_u128:                        ! @cmp_ge_imm_u128
.Lcmp_ge_imm_u128$local:
	.type	.Lcmp_ge_imm_u128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u128 %r1, %r1, 123456
	retf 0

.Lfunc_end376:
	.size	cmp_ge_imm_u128, .Lfunc_end376-cmp_ge_imm_u128
	.size	.Lcmp_ge_imm_u128$local, .Lfunc_end376-cmp_ge_imm_u128
                                        ! -- End function
	.globl	cmp_eq_imm_i128_ext             ! -- Begin function cmp_eq_imm_i128_ext
	.p2align	4
	.type	cmp_eq_imm_i128_ext,@function
cmp_eq_imm_i128_ext:                    ! @cmp_eq_imm_i128_ext
.Lcmp_eq_imm_i128_ext$local:
	.type	.Lcmp_eq_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_eq_imm_i128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end377:
	.size	cmp_eq_imm_i128_ext, .Lfunc_end377-cmp_eq_imm_i128_ext
	.size	.Lcmp_eq_imm_i128_ext$local, .Lfunc_end377-cmp_eq_imm_i128_ext
                                        ! -- End function
	.globl	cmp_ne_imm_i128_ext             ! -- Begin function cmp_ne_imm_i128_ext
	.p2align	4
	.type	cmp_ne_imm_i128_ext,@function
cmp_ne_imm_i128_ext:                    ! @cmp_ne_imm_i128_ext
.Lcmp_ne_imm_i128_ext$local:
	.type	.Lcmp_ne_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ne_imm_i128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end378:
	.size	cmp_ne_imm_i128_ext, .Lfunc_end378-cmp_ne_imm_i128_ext
	.size	.Lcmp_ne_imm_i128_ext$local, .Lfunc_end378-cmp_ne_imm_i128_ext
                                        ! -- End function
	.globl	cmp_lt_imm_i128_ext             ! -- Begin function cmp_lt_imm_i128_ext
	.p2align	4
	.type	cmp_lt_imm_i128_ext,@function
cmp_lt_imm_i128_ext:                    ! @cmp_lt_imm_i128_ext
.Lcmp_lt_imm_i128_ext$local:
	.type	.Lcmp_lt_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end379:
	.size	cmp_lt_imm_i128_ext, .Lfunc_end379-cmp_lt_imm_i128_ext
	.size	.Lcmp_lt_imm_i128_ext$local, .Lfunc_end379-cmp_lt_imm_i128_ext
                                        ! -- End function
	.globl	cmp_gt_imm_i128_ext             ! -- Begin function cmp_gt_imm_i128_ext
	.p2align	4
	.type	cmp_gt_imm_i128_ext,@function
cmp_gt_imm_i128_ext:                    ! @cmp_gt_imm_i128_ext
.Lcmp_gt_imm_i128_ext$local:
	.type	.Lcmp_gt_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end380:
	.size	cmp_gt_imm_i128_ext, .Lfunc_end380-cmp_gt_imm_i128_ext
	.size	.Lcmp_gt_imm_i128_ext$local, .Lfunc_end380-cmp_gt_imm_i128_ext
                                        ! -- End function
	.globl	cmp_le_imm_i128_ext             ! -- Begin function cmp_le_imm_i128_ext
	.p2align	4
	.type	cmp_le_imm_i128_ext,@function
cmp_le_imm_i128_ext:                    ! @cmp_le_imm_i128_ext
.Lcmp_le_imm_i128_ext$local:
	.type	.Lcmp_le_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_i128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end381:
	.size	cmp_le_imm_i128_ext, .Lfunc_end381-cmp_le_imm_i128_ext
	.size	.Lcmp_le_imm_i128_ext$local, .Lfunc_end381-cmp_le_imm_i128_ext
                                        ! -- End function
	.globl	cmp_ge_imm_i128_ext             ! -- Begin function cmp_ge_imm_i128_ext
	.p2align	4
	.type	cmp_ge_imm_i128_ext,@function
cmp_ge_imm_i128_ext:                    ! @cmp_ge_imm_i128_ext
.Lcmp_ge_imm_i128_ext$local:
	.type	.Lcmp_ge_imm_i128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_i128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end382:
	.size	cmp_ge_imm_i128_ext, .Lfunc_end382-cmp_ge_imm_i128_ext
	.size	.Lcmp_ge_imm_i128_ext$local, .Lfunc_end382-cmp_ge_imm_i128_ext
                                        ! -- End function
	.globl	cmp_lt_imm_u128_ext             ! -- Begin function cmp_lt_imm_u128_ext
	.p2align	4
	.type	cmp_lt_imm_u128_ext,@function
cmp_lt_imm_u128_ext:                    ! @cmp_lt_imm_u128_ext
.Lcmp_lt_imm_u128_ext$local:
	.type	.Lcmp_lt_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end383:
	.size	cmp_lt_imm_u128_ext, .Lfunc_end383-cmp_lt_imm_u128_ext
	.size	.Lcmp_lt_imm_u128_ext$local, .Lfunc_end383-cmp_lt_imm_u128_ext
                                        ! -- End function
	.globl	cmp_gt_imm_u128_ext             ! -- Begin function cmp_gt_imm_u128_ext
	.p2align	4
	.type	cmp_gt_imm_u128_ext,@function
cmp_gt_imm_u128_ext:                    ! @cmp_gt_imm_u128_ext
.Lcmp_gt_imm_u128_ext$local:
	.type	.Lcmp_gt_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end384:
	.size	cmp_gt_imm_u128_ext, .Lfunc_end384-cmp_gt_imm_u128_ext
	.size	.Lcmp_gt_imm_u128_ext$local, .Lfunc_end384-cmp_gt_imm_u128_ext
                                        ! -- End function
	.globl	cmp_le_imm_u128_ext             ! -- Begin function cmp_le_imm_u128_ext
	.p2align	4
	.type	cmp_le_imm_u128_ext,@function
cmp_le_imm_u128_ext:                    ! @cmp_le_imm_u128_ext
.Lcmp_le_imm_u128_ext$local:
	.type	.Lcmp_le_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_lt_imm_u128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end385:
	.size	cmp_le_imm_u128_ext, .Lfunc_end385-cmp_le_imm_u128_ext
	.size	.Lcmp_le_imm_u128_ext$local, .Lfunc_end385-cmp_le_imm_u128_ext
                                        ! -- End function
	.globl	cmp_ge_imm_u128_ext             ! -- Begin function cmp_ge_imm_u128_ext
	.p2align	4
	.type	cmp_ge_imm_u128_ext,@function
cmp_ge_imm_u128_ext:                    ! @cmp_ge_imm_u128_ext
.Lcmp_ge_imm_u128_ext$local:
	.type	.Lcmp_ge_imm_u128_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	cmp_ge_imm_u128.l %r1, %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end386:
	.size	cmp_ge_imm_u128_ext, .Lfunc_end386-cmp_ge_imm_u128_ext
	.size	.Lcmp_ge_imm_u128_ext$local, .Lfunc_end386-cmp_ge_imm_u128_ext
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
	alloc 4
	cpuid %r2, %r1, 33
	cpuid %r3, %r1, 0

	! BUNDLE
	add_imm_i64 %r1, %r1, 10000
	cpuid %r1, %r1, 0
	add_add_i64 %r1, %r3, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end387:
	.size	test_cpuid, .Lfunc_end387-test_cpuid
	.size	.Ltest_cpuid$local, .Lfunc_end387-test_cpuid
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
	alloc 4
	crc32c %r1, %r1, %r2, %r3
	retf 0

.Lfunc_end388:
	.size	test_crc32c, .Lfunc_end388-test_crc32c
	.size	.Ltest_crc32c$local, .Lfunc_end388-test_crc32c
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
	alloc 3
	get_mr %r2, %r1, 1
	set_mr %r2, %r1, 1

	! BUNDLE
	get_ibr %r2, %r1, 2
	set_ibr %r2, %r1, 1
	get_dbr %r2, %r1, 1

	! BUNDLE
	set_dbr %r2, %r1, 2
	retf 0
	nop 0

.Lfunc_end389:
	.size	TestDebugRegisters, .Lfunc_end389-TestDebugRegisters
	.size	.LTestDebugRegisters$local, .Lfunc_end389-TestDebugRegisters
                                        ! -- End function
	.globl	test_divide_pow2_32             ! -- Begin function test_divide_pow2_32
	.p2align	4
	.type	test_divide_pow2_32,@function
test_divide_pow2_32:                    ! @test_divide_pow2_32
.Ltest_divide_pow2_32$local:
	.type	.Ltest_divide_pow2_32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	srd_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end390:
	.size	test_divide_pow2_32, .Lfunc_end390-test_divide_pow2_32
	.size	.Ltest_divide_pow2_32$local, .Lfunc_end390-test_divide_pow2_32
                                        ! -- End function
	.globl	test_divide_pow2_imm_32         ! -- Begin function test_divide_pow2_imm_32
	.p2align	4
	.type	test_divide_pow2_imm_32,@function
test_divide_pow2_imm_32:                ! @test_divide_pow2_imm_32
.Ltest_divide_pow2_imm_32$local:
	.type	.Ltest_divide_pow2_imm_32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	srd_imm_i32 %r1, %r1, 10
	retf 0

.Lfunc_end391:
	.size	test_divide_pow2_imm_32, .Lfunc_end391-test_divide_pow2_imm_32
	.size	.Ltest_divide_pow2_imm_32$local, .Lfunc_end391-test_divide_pow2_imm_32
                                        ! -- End function
	.globl	test_divide_pow2_64             ! -- Begin function test_divide_pow2_64
	.p2align	4
	.type	test_divide_pow2_64,@function
test_divide_pow2_64:                    ! @test_divide_pow2_64
.Ltest_divide_pow2_64$local:
	.type	.Ltest_divide_pow2_64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	srd_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end392:
	.size	test_divide_pow2_64, .Lfunc_end392-test_divide_pow2_64
	.size	.Ltest_divide_pow2_64$local, .Lfunc_end392-test_divide_pow2_64
                                        ! -- End function
	.globl	test_divide_pow2_imm_64         ! -- Begin function test_divide_pow2_imm_64
	.p2align	4
	.type	test_divide_pow2_imm_64,@function
test_divide_pow2_imm_64:                ! @test_divide_pow2_imm_64
.Ltest_divide_pow2_imm_64$local:
	.type	.Ltest_divide_pow2_imm_64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	srd_imm_i64 %r1, %r1, 10
	retf 0

.Lfunc_end393:
	.size	test_divide_pow2_imm_64, .Lfunc_end393-test_divide_pow2_imm_64
	.size	.Ltest_divide_pow2_imm_64$local, .Lfunc_end393-test_divide_pow2_imm_64
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
	alloc 4
	madd_f16 %r3, %r1, %r2, %r3
	nmadd_f16 %r3, %r1, %r2, %r3

	! BUNDLE
	msub_f16 %r2, %r1, %r2, %r3
	nmsub_f16 %r1, %r1, %r2, %r2
	retf 0

.Lfunc_end394:
	.size	test_fma_f16, .Lfunc_end394-test_fma_f16
	.size	.Ltest_fma_f16$local, .Lfunc_end394-test_fma_f16
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
	alloc 4
	madd_f32 %r3, %r1, %r2, %r3
	nmadd_f32 %r3, %r1, %r2, %r3

	! BUNDLE
	msub_f32 %r2, %r1, %r2, %r3
	nmsub_f32 %r1, %r1, %r2, %r2
	retf 0

.Lfunc_end395:
	.size	test_fma_f32, .Lfunc_end395-test_fma_f32
	.size	.Ltest_fma_f32$local, .Lfunc_end395-test_fma_f32
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
	alloc 4
	madd_f64 %r3, %r1, %r2, %r3
	nmadd_f64 %r3, %r1, %r2, %r3

	! BUNDLE
	msub_f64 %r2, %r1, %r2, %r3
	nmsub_f64 %r1, %r1, %r2, %r2
	retf 0

.Lfunc_end396:
	.size	test_fma_f64, .Lfunc_end396-test_fma_f64
	.size	.Ltest_fma_f64$local, .Lfunc_end396-test_fma_f64
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
	alloc 4
	madd_f128 %r3, %r1, %r2, %r3
	nmadd_f128 %r3, %r1, %r2, %r3

	! BUNDLE
	msub_f128 %r2, %r1, %r2, %r3
	nmsub_f128 %r1, %r1, %r2, %r2
	retf 0

.Lfunc_end397:
	.size	test_fma_f128, .Lfunc_end397-test_fma_f128
	.size	.Ltest_fma_f128$local, .Lfunc_end397-test_fma_f128
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
	alloc 4
	madd_vf16 %r3, %r1, %r2, %r3
	nmadd_vf16 %r3, %r1, %r2, %r3

	! BUNDLE
	msub_vf16 %r2, %r1, %r2, %r3
	nmsub_vf16 %r2, %r1, %r2, %r2
	madd_alt_vf16 %r1, %r1, %r2, %r2

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end398:
	.size	test_fma_v8f16, .Lfunc_end398-test_fma_v8f16
	.size	.Ltest_fma_v8f16$local, .Lfunc_end398-test_fma_v8f16
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
	alloc 4
	madd_vf32 %r3, %r1, %r2, %r3
	nmadd_vf32 %r3, %r1, %r2, %r3

	! BUNDLE
	msub_vf32 %r2, %r1, %r2, %r3
	nmsub_vf32 %r2, %r1, %r2, %r2
	madd_alt_vf32 %r1, %r1, %r2, %r2

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end399:
	.size	test_fma_v4f32, .Lfunc_end399-test_fma_v4f32
	.size	.Ltest_fma_v4f32$local, .Lfunc_end399-test_fma_v4f32
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
	alloc 4
	madd_vf64 %r3, %r1, %r2, %r3
	nmadd_vf64 %r3, %r1, %r2, %r3

	! BUNDLE
	msub_vf64 %r2, %r1, %r2, %r3
	nmsub_vf64 %r2, %r1, %r2, %r2
	madd_alt_vf64 %r1, %r1, %r2, %r2

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end400:
	.size	test_fma_v2f64, .Lfunc_end400-test_fma_v2f64
	.size	.Ltest_fma_v2f64$local, .Lfunc_end400-test_fma_v2f64
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
	alloc 3
	ld_u64 %r1, %sp, 0
	ld_u32 %r2, %r1, 0

	! BUNDLE
	ld_u32 %r1, %r1, 4
	add_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end401:
	.size	ffff, .Lfunc_end401-ffff
	.size	.Lffff$local, .Lfunc_end401-ffff
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
	alloc_sp 11, 96
	add_i64 %r3, %r2, %r1
	st_i64 %r3, %sp, 88

	! BUNDLE
	sub_i64 %r3, %r1, %r2
	st_i64 %r3, %sp, 80
	subr_imm_i64 %r3, %r2, 1000

	! BUNDLE
	st_i64 %r3, %sp, 72
	subr_imm_i64.l %r3, %r2, 1000000000

	! BUNDLE
	st_i64 %r3, %sp, 64
	add_imm_i64 %r3, %r1, -100
	st_i64 %r3, %sp, 56

	! BUNDLE
	st_i64 %r3, %sp, 48
	ld_u64 %r3, %sp, 88
	ld_u64 %r4, %sp, 80

	! BUNDLE
	ld_u64 %r5, %sp, 72
	ld_u64 %r6, %sp, 64
	ld_u64 %r7, %sp, 56

	! BUNDLE
	ld_u64 %r8, %sp, 48
	add_add_i64 %r4, %r4, %r5, %r7
	add_sub_i64 %r3, %r3, %r6, %r4

	! BUNDLE
	add_i64 %r3, %r3, %r8
	st_i64 %r3, %sp, 40
	br_ge_i64 %r1, %r2, .LBB402_3

	! BUNDLE
	ld_u64 %r2, %sp, 88
	br_ge_i64 %r1, %r2, .LBB402_3
	ld_u64 %r1, %sp, 40

	! BUNDLE
	add_imm_i64 %r1, %r1, -1
	lda_iprel %r10, %pcfwd(.L.str.3)
	st_i64 %r1, %sp, 0

	! BUNDLE
	call %r9, %jpc(printf)
	ld_u64 %r1, %sp, 88
	ld_u64 %r2, %sp, 80

	! BUNDLE
	ld_u64 %r3, %sp, 64
	add_sub_i64 %r1, %r2, %r1, %r3
	st_i64 %r1, %sp, 32

	! BUNDLE
	ld_u64 %r1, %sp, 88
	ld_u64 %r2, %sp, 80
	ld_u64 %r3, %sp, 64

	! BUNDLE
	sub_sub_i64 %r1, %r1, %r2, %r3
	st_i64 %r1, %sp, 24
	ld_u64 %r1, %sp, 40

	! BUNDLE
	retf 96
	nop 0
	nop 0

.LBB402_3:                              ! %if.else
                                        ! Label of block must be emitted
	! BUNDLE
	ld_u64 %r1, %sp, 40
	add_imm_i64 %r1, %r1, 1
	lda_iprel %r10, %pcfwd(.L.str.4)

	! BUNDLE
	st_i64 %r1, %sp, 0
	call %r9, %jpc(printf)
	mov %r10, %gz

	! BUNDLE
	call %r9, %jpc(exit)
	nop 0
	nop 0

.Lfunc_end402:
	.size	i64_test, .Lfunc_end402-i64_test
	.size	.Li64_test$local, .Lfunc_end402-i64_test
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
	alloc_sp 7, 16
	br_eq_imm_i32 %r1, 0, .LBB403_3
	lda_iprel %r3, %pcfwd(.L.str.5)

.LBB403_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %r1, %sp, 0
	mov %r6, %r3
	call %r5, %jpc(printf)

	! BUNDLE
	ld_iprel_u64 %r4, %pcfwd(uglbx)
	add_imm_i64 %r4, %r4, 1
	add_imm_i32 %r1, %r1, -1

	! BUNDLE
	st_iprel_i64 %r4, %pcfwd(uglbx)
	br_ne_imm_i32 %r1, 0, .LBB403_2
	nop 0

.LBB403_3:                              ! %for.cond1.preheader
                                        ! Label of block must be emitted
	! BUNDLE
	lda_iprel %r1, %pcfwd(.L.str.6)
	nop 0
	nop 0

.LBB403_4:                              ! %for.cond1
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %r2, %sp, 0
	mov %r6, %r1
	call %r5, %jpc(printf)

	! BUNDLE
	ld_iprel_u32 %r3, %pcfwd(glbx)
	add_imm_i32 %r2, %r2, -1
	add_imm_i32 %r3, %r3, -1

	! BUNDLE
	st_iprel_i32 %r3, %pcfwd(glbx)
	jmp .LBB403_4
	nop 0

.Lfunc_end403:
	.size	NeverReturn, .Lfunc_end403-NeverReturn
	.size	.LNeverReturn$local, .Lfunc_end403-NeverReturn
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
	alloc 3
	ld_iprel_i32 %r2, %pcfwd(glbx)
	nop 2

	! BUNDLE
	lda_xi64.l %r1, %r1, %r2, 0, 1234
	st_iprel_i32 %r1, %pcfwd(glbx)

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end404:
	.size	i64_add, .Lfunc_end404-i64_add
	.size	.Li64_add$local, .Lfunc_end404-i64_add
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
	alloc 6
	ld_iprel_u32 %r3, %pcfwd(glb)
	mov %r5, %r1

	! BUNDLE
	call %r4, %jpc(cccc)
	lda_r %r2, %jpc(cccc)
	add_i32 %r1, %r2, %r1

	! BUNDLE
	add_i32 %r1, %r1, %r3
	add_i32 %r1, %r1, %r5
	add_imm_i32 %r1, %r1, 12345

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end405:
	.size	i32_add, .Lfunc_end405-i32_add
	.size	.Li32_add$local, .Lfunc_end405-i32_add
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
	alloc 3
	ld_iprel_u32 %r2, %pcfwd(uglb)
	add_i32 %r1, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end406:
	.size	u32_add, .Lfunc_end406-u32_add
	.size	.Lu32_add$local, .Lfunc_end406-u32_add
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
	alloc_sp 5, 16
	br_oge_f64 %r1, %r2, .LBB407_1
	lda_iprel %r4, %pcfwd(.L.str.7)

	! BUNDLE
	st_i64 %r1, %sp, 0
	call %r3, %jpc(printf)
	mov %r1, %r4

	! BUNDLE
	retf 16
	nop 0
	nop 0

.LBB407_1:                              ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 2
	retf 16
	nop 0

.Lfunc_end407:
	.size	float_test, .Lfunc_end407-float_test
	.size	.Lfloat_test$local, .Lfunc_end407-float_test
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
	alloc 2
	mov %r1, %gz
	retf 0

.Lfunc_end408:
	.size	main, .Lfunc_end408-main
	.size	.Lmain$local, .Lfunc_end408-main
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
	alloc 4
	add_f16 %r3, %r2, %r1
	sub_f16 %r1, %r1, %r2

	! BUNDLE
	mul_f16 %r1, %r3, %r1
	div_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end409:
	.size	test_fp_arithmetic_f16, .Lfunc_end409-test_fp_arithmetic_f16
	.size	.Ltest_fp_arithmetic_f16$local, .Lfunc_end409-test_fp_arithmetic_f16
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
	alloc 4
	add_f32 %r3, %r2, %r1
	sub_f32 %r1, %r1, %r2

	! BUNDLE
	mul_f32 %r1, %r3, %r1
	div_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end410:
	.size	test_fp_arithmetic_f32, .Lfunc_end410-test_fp_arithmetic_f32
	.size	.Ltest_fp_arithmetic_f32$local, .Lfunc_end410-test_fp_arithmetic_f32
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
	alloc 4
	add_f64 %r3, %r2, %r1
	sub_f64 %r1, %r1, %r2

	! BUNDLE
	mul_f64 %r1, %r3, %r1
	div_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end411:
	.size	test_fp_arithmetic_f64, .Lfunc_end411-test_fp_arithmetic_f64
	.size	.Ltest_fp_arithmetic_f64$local, .Lfunc_end411-test_fp_arithmetic_f64
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
	alloc 4
	add_f128 %r3, %r2, %r1
	sub_f128 %r1, %r1, %r2

	! BUNDLE
	mul_f128 %r1, %r3, %r1
	div_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end412:
	.size	test_fp_arithmetic_f128, .Lfunc_end412-test_fp_arithmetic_f128
	.size	.Ltest_fp_arithmetic_f128$local, .Lfunc_end412-test_fp_arithmetic_f128
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
	alloc_sp 2, 16
	st_i16 %r1, %sp, 14
	ld_u16 %r1, %sp, 14

	! BUNDLE
	retf 16
	nop 0
	nop 0

.Lfunc_end413:
	.size	test_fp_bitcast_i16_to_f16, .Lfunc_end413-test_fp_bitcast_i16_to_f16
	.size	.Ltest_fp_bitcast_i16_to_f16$local, .Lfunc_end413-test_fp_bitcast_i16_to_f16
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
	alloc_sp 2, 16
	st_i16 %r1, %sp, 14
	ld_i16 %r1, %sp, 14

	! BUNDLE
	retf 16
	nop 0
	nop 0

.Lfunc_end414:
	.size	test_fp_bitcast_f16_to_i16, .Lfunc_end414-test_fp_bitcast_f16_to_i16
	.size	.Ltest_fp_bitcast_f16_to_i16$local, .Lfunc_end414-test_fp_bitcast_f16_to_i16
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end415:
	.size	test_fp_bitcast_i32_to_f32, .Lfunc_end415-test_fp_bitcast_i32_to_f32
	.size	.Ltest_fp_bitcast_i32_to_f32$local, .Lfunc_end415-test_fp_bitcast_i32_to_f32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end416:
	.size	test_fp_bitcast_f32_to_i32, .Lfunc_end416-test_fp_bitcast_f32_to_i32
	.size	.Ltest_fp_bitcast_f32_to_i32$local, .Lfunc_end416-test_fp_bitcast_f32_to_i32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end417:
	.size	test_fp_bitcast_i64_to_f64, .Lfunc_end417-test_fp_bitcast_i64_to_f64
	.size	.Ltest_fp_bitcast_i64_to_f64$local, .Lfunc_end417-test_fp_bitcast_i64_to_f64
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end418:
	.size	test_fp_bitcast_f64_to_i64, .Lfunc_end418-test_fp_bitcast_f64_to_i64
	.size	.Ltest_fp_bitcast_f64_to_i64$local, .Lfunc_end418-test_fp_bitcast_f64_to_i64
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end419:
	.size	test_fp_bitcast_i128_to_f128, .Lfunc_end419-test_fp_bitcast_i128_to_f128
	.size	.Ltest_fp_bitcast_i128_to_f128$local, .Lfunc_end419-test_fp_bitcast_i128_to_f128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end420:
	.size	test_fp_bitcast_f128_to_i128, .Lfunc_end420-test_fp_bitcast_f128_to_i128
	.size	.Ltest_fp_bitcast_f128_to_i128$local, .Lfunc_end420-test_fp_bitcast_f128_to_i128
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
	alloc 3
	br_ueq_f32 %r1, %r2, .LBB421_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB421_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end421:
	.size	test_fp_branch_f32_eq, .Lfunc_end421-test_fp_branch_f32_eq
	.size	.Ltest_fp_branch_f32_eq$local, .Lfunc_end421-test_fp_branch_f32_eq
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
	alloc 5
	nul_one_f32 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end422:
	.size	test_fp_branch_f32_ne, .Lfunc_end422-test_fp_branch_f32_ne
	.size	.Ltest_fp_branch_f32_ne$local, .Lfunc_end422-test_fp_branch_f32_ne
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
	alloc 3
	br_ult_f32 %r1, %r2, .LBB423_1
	nop 222

	! BUNDLE
	retf 0
	nop 0
	nop 0

.LBB423_1:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	nop 111
	retf 0
	nop 0

.Lfunc_end423:
	.size	test_fp_branch_f32_lt, .Lfunc_end423-test_fp_branch_f32_lt
	.size	.Ltest_fp_branch_f32_lt$local, .Lfunc_end423-test_fp_branch_f32_lt
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
	alloc 5
	nul_oge_f32 %r2, %r1, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.63)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end424:
	.size	test_fp_branch_f32_le, .Lfunc_end424-test_fp_branch_f32_le
	.size	.Ltest_fp_branch_f32_le$local, .Lfunc_end424-test_fp_branch_f32_le
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
	alloc 5
	nul_olt_f32 %r2, %r1, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.64)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end425:
	.size	test_fp_branch_f32_gt, .Lfunc_end425-test_fp_branch_f32_gt
	.size	.Ltest_fp_branch_f32_gt$local, .Lfunc_end425-test_fp_branch_f32_gt
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
	alloc 5
	nul_oge_f32 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.65)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end426:
	.size	test_fp_branch_f32_ge, .Lfunc_end426-test_fp_branch_f32_ge
	.size	.Ltest_fp_branch_f32_ge$local, .Lfunc_end426-test_fp_branch_f32_ge
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
	alloc 5
	nul_oeq_f64 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.66)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end427:
	.size	test_fp_branch_f64_eq, .Lfunc_end427-test_fp_branch_f64_eq
	.size	.Ltest_fp_branch_f64_eq$local, .Lfunc_end427-test_fp_branch_f64_eq
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
	alloc 5
	nul_one_f64 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.67)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end428:
	.size	test_fp_branch_f64_ne, .Lfunc_end428-test_fp_branch_f64_ne
	.size	.Ltest_fp_branch_f64_ne$local, .Lfunc_end428-test_fp_branch_f64_ne
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
	alloc 5
	nul_olt_f64 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.68)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end429:
	.size	test_fp_branch_f64_lt, .Lfunc_end429-test_fp_branch_f64_lt
	.size	.Ltest_fp_branch_f64_lt$local, .Lfunc_end429-test_fp_branch_f64_lt
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
	alloc 5
	nul_oge_f64 %r2, %r1, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.69)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end430:
	.size	test_fp_branch_f64_le, .Lfunc_end430-test_fp_branch_f64_le
	.size	.Ltest_fp_branch_f64_le$local, .Lfunc_end430-test_fp_branch_f64_le
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
	alloc 5
	nul_olt_f64 %r2, %r1, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.70)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end431:
	.size	test_fp_branch_f64_gt, .Lfunc_end431-test_fp_branch_f64_gt
	.size	.Ltest_fp_branch_f64_gt$local, .Lfunc_end431-test_fp_branch_f64_gt
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
	alloc 5
	nul_oge_f64 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.71)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end432:
	.size	test_fp_branch_f64_ge, .Lfunc_end432-test_fp_branch_f64_ge
	.size	.Ltest_fp_branch_f64_ge$local, .Lfunc_end432-test_fp_branch_f64_ge
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
	alloc 5
	nul_oeq_f128 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.72)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end433:
	.size	test_fp_branch_f128_eq, .Lfunc_end433-test_fp_branch_f128_eq
	.size	.Ltest_fp_branch_f128_eq$local, .Lfunc_end433-test_fp_branch_f128_eq
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
	alloc 5
	nul_one_f128 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.73)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end434:
	.size	test_fp_branch_f128_ne, .Lfunc_end434-test_fp_branch_f128_ne
	.size	.Ltest_fp_branch_f128_ne$local, .Lfunc_end434-test_fp_branch_f128_ne
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
	alloc 5
	nul_olt_f128 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.74)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end435:
	.size	test_fp_branch_f128_lt, .Lfunc_end435-test_fp_branch_f128_lt
	.size	.Ltest_fp_branch_f128_lt$local, .Lfunc_end435-test_fp_branch_f128_lt
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
	alloc 5
	nul_oge_f128 %r2, %r1, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.75)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end436:
	.size	test_fp_branch_f128_le, .Lfunc_end436-test_fp_branch_f128_le
	.size	.Ltest_fp_branch_f128_le$local, .Lfunc_end436-test_fp_branch_f128_le
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
	alloc 5
	nul_olt_f128 %r2, %r1, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.76)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end437:
	.size	test_fp_branch_f128_gt, .Lfunc_end437-test_fp_branch_f128_gt
	.size	.Ltest_fp_branch_f128_gt$local, .Lfunc_end437-test_fp_branch_f128_gt
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
	alloc 5
	nul_oge_f128 %r1, %r2, 2, 0
	lda_iprel %r4, %pcfwd(.Lstr.77)

	! BUNDLE
	call %r3, %jpc(puts)
	retf 0
	nop 0

.Lfunc_end438:
	.size	test_fp_branch_f128_ge, .Lfunc_end438-test_fp_branch_f128_ge
	.size	.Ltest_fp_branch_f128_ge$local, .Lfunc_end438-test_fp_branch_f128_ge
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
	alloc 2
	mov %r1, %gz
	retf 0

.Lfunc_end439:
	.size	test_builtin_f32_isnan, .Lfunc_end439-test_builtin_f32_isnan
	.size	.Ltest_builtin_f32_isnan$local, .Lfunc_end439-test_builtin_f32_isnan
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
	alloc 3
	merge_f32 %r1, %r2, %r1, %r1
	retf 0

.Lfunc_end440:
	.size	test_builtin_f32_copysign, .Lfunc_end440-test_builtin_f32_copysign
	.size	.Ltest_builtin_f32_copysign$local, .Lfunc_end440-test_builtin_f32_copysign
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
	alloc 3
	merge_f64 %r1, %r2, %r1, %r1
	retf 0

.Lfunc_end441:
	.size	test_builtin_f64_copysign, .Lfunc_end441-test_builtin_f64_copysign
	.size	.Ltest_builtin_f64_copysign$local, .Lfunc_end441-test_builtin_f64_copysign
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
	alloc 3
	merge_f128 %r1, %r2, %r1, %r1
	retf 0

.Lfunc_end442:
	.size	test_builtin_f128_copysign, .Lfunc_end442-test_builtin_f128_copysign
	.size	.Ltest_builtin_f128_copysign$local, .Lfunc_end442-test_builtin_f128_copysign
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
	alloc 3
	cmp_oeq_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end443:
	.size	test_compare_f16_eq, .Lfunc_end443-test_compare_f16_eq
	.size	.Ltest_compare_f16_eq$local, .Lfunc_end443-test_compare_f16_eq
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
	alloc 3
	cmp_one_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end444:
	.size	test_compare_f16_ne, .Lfunc_end444-test_compare_f16_ne
	.size	.Ltest_compare_f16_ne$local, .Lfunc_end444-test_compare_f16_ne
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
	alloc 3
	cmp_olt_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end445:
	.size	test_compare_f16_lt, .Lfunc_end445-test_compare_f16_lt
	.size	.Ltest_compare_f16_lt$local, .Lfunc_end445-test_compare_f16_lt
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
	alloc 3
	cmp_oge_f16 %r1, %r2, %r1
	retf 0

.Lfunc_end446:
	.size	test_compare_f16_le, .Lfunc_end446-test_compare_f16_le
	.size	.Ltest_compare_f16_le$local, .Lfunc_end446-test_compare_f16_le
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
	alloc 3
	cmp_olt_f16 %r1, %r2, %r1
	retf 0

.Lfunc_end447:
	.size	test_compare_f16_gt, .Lfunc_end447-test_compare_f16_gt
	.size	.Ltest_compare_f16_gt$local, .Lfunc_end447-test_compare_f16_gt
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
	alloc 3
	cmp_oge_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end448:
	.size	test_compare_f16_ge, .Lfunc_end448-test_compare_f16_ge
	.size	.Ltest_compare_f16_ge$local, .Lfunc_end448-test_compare_f16_ge
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
	alloc 3
	cmp_oeq_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end449:
	.size	test_compare_f32_eq, .Lfunc_end449-test_compare_f32_eq
	.size	.Ltest_compare_f32_eq$local, .Lfunc_end449-test_compare_f32_eq
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
	alloc 3
	cmp_one_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end450:
	.size	test_compare_f32_ne, .Lfunc_end450-test_compare_f32_ne
	.size	.Ltest_compare_f32_ne$local, .Lfunc_end450-test_compare_f32_ne
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
	alloc 3
	cmp_olt_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end451:
	.size	test_compare_f32_lt, .Lfunc_end451-test_compare_f32_lt
	.size	.Ltest_compare_f32_lt$local, .Lfunc_end451-test_compare_f32_lt
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
	alloc 3
	cmp_oge_f32 %r1, %r2, %r1
	retf 0

.Lfunc_end452:
	.size	test_compare_f32_le, .Lfunc_end452-test_compare_f32_le
	.size	.Ltest_compare_f32_le$local, .Lfunc_end452-test_compare_f32_le
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
	alloc 3
	cmp_olt_f32 %r1, %r2, %r1
	retf 0

.Lfunc_end453:
	.size	test_compare_f32_gt, .Lfunc_end453-test_compare_f32_gt
	.size	.Ltest_compare_f32_gt$local, .Lfunc_end453-test_compare_f32_gt
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
	alloc 3
	cmp_oge_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end454:
	.size	test_compare_f32_ge, .Lfunc_end454-test_compare_f32_ge
	.size	.Ltest_compare_f32_ge$local, .Lfunc_end454-test_compare_f32_ge
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
	alloc 3
	cmp_oeq_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end455:
	.size	test_compare_f64_eq, .Lfunc_end455-test_compare_f64_eq
	.size	.Ltest_compare_f64_eq$local, .Lfunc_end455-test_compare_f64_eq
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
	alloc 3
	cmp_one_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end456:
	.size	test_compare_f64_ne, .Lfunc_end456-test_compare_f64_ne
	.size	.Ltest_compare_f64_ne$local, .Lfunc_end456-test_compare_f64_ne
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
	alloc 3
	cmp_olt_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end457:
	.size	test_compare_f64_lt, .Lfunc_end457-test_compare_f64_lt
	.size	.Ltest_compare_f64_lt$local, .Lfunc_end457-test_compare_f64_lt
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
	alloc 3
	cmp_oge_f64 %r1, %r2, %r1
	retf 0

.Lfunc_end458:
	.size	test_compare_f64_le, .Lfunc_end458-test_compare_f64_le
	.size	.Ltest_compare_f64_le$local, .Lfunc_end458-test_compare_f64_le
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
	alloc 3
	cmp_olt_f64 %r1, %r2, %r1
	retf 0

.Lfunc_end459:
	.size	test_compare_f64_gt, .Lfunc_end459-test_compare_f64_gt
	.size	.Ltest_compare_f64_gt$local, .Lfunc_end459-test_compare_f64_gt
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
	alloc 3
	cmp_oge_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end460:
	.size	test_compare_f64_ge, .Lfunc_end460-test_compare_f64_ge
	.size	.Ltest_compare_f64_ge$local, .Lfunc_end460-test_compare_f64_ge
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
	alloc 3
	cmp_oeq_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end461:
	.size	test_compare_f128_eq, .Lfunc_end461-test_compare_f128_eq
	.size	.Ltest_compare_f128_eq$local, .Lfunc_end461-test_compare_f128_eq
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
	alloc 3
	cmp_one_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end462:
	.size	test_compare_f128_ne, .Lfunc_end462-test_compare_f128_ne
	.size	.Ltest_compare_f128_ne$local, .Lfunc_end462-test_compare_f128_ne
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
	alloc 3
	cmp_olt_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end463:
	.size	test_compare_f128_lt, .Lfunc_end463-test_compare_f128_lt
	.size	.Ltest_compare_f128_lt$local, .Lfunc_end463-test_compare_f128_lt
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
	alloc 3
	cmp_oge_f128 %r1, %r2, %r1
	retf 0

.Lfunc_end464:
	.size	test_compare_f128_le, .Lfunc_end464-test_compare_f128_le
	.size	.Ltest_compare_f128_le$local, .Lfunc_end464-test_compare_f128_le
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
	alloc 3
	cmp_olt_f128 %r1, %r2, %r1
	retf 0

.Lfunc_end465:
	.size	test_compare_f128_gt, .Lfunc_end465-test_compare_f128_gt
	.size	.Ltest_compare_f128_gt$local, .Lfunc_end465-test_compare_f128_gt
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
	alloc 3
	cmp_oge_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end466:
	.size	test_compare_f128_ge, .Lfunc_end466-test_compare_f128_ge
	.size	.Ltest_compare_f128_ge$local, .Lfunc_end466-test_compare_f128_ge
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
	alloc 2
	extend_f16_f32 %r1, %r1
	retf 0

.Lfunc_end467:
	.size	test_conv_f16_to_f32, .Lfunc_end467-test_conv_f16_to_f32
	.size	.Ltest_conv_f16_to_f32$local, .Lfunc_end467-test_conv_f16_to_f32
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
	alloc 2
	extend_f16_f64 %r1, %r1
	retf 0

.Lfunc_end468:
	.size	test_conv_f16_to_f64, .Lfunc_end468-test_conv_f16_to_f64
	.size	.Ltest_conv_f16_to_f64$local, .Lfunc_end468-test_conv_f16_to_f64
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
	alloc 2
	extend_f16_f128 %r1, %r1
	retf 0

.Lfunc_end469:
	.size	test_conv_f16_to_f128, .Lfunc_end469-test_conv_f16_to_f128
	.size	.Ltest_conv_f16_to_f128$local, .Lfunc_end469-test_conv_f16_to_f128
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
	alloc 2
	extend_f64_f128 %r1, %r1
	retf 0

.Lfunc_end470:
	.size	test_conv_f64_to_f128, .Lfunc_end470-test_conv_f64_to_f128
	.size	.Ltest_conv_f64_to_f128$local, .Lfunc_end470-test_conv_f64_to_f128
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
	alloc 2
	extend_f32_f64 %r1, %r1
	retf 0

.Lfunc_end471:
	.size	test_conv_f32_to_f64, .Lfunc_end471-test_conv_f32_to_f64
	.size	.Ltest_conv_f32_to_f64$local, .Lfunc_end471-test_conv_f32_to_f64
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
	alloc 2
	extend_f32_f128 %r1, %r1
	retf 0

.Lfunc_end472:
	.size	test_conv_f32_to_f128, .Lfunc_end472-test_conv_f32_to_f128
	.size	.Ltest_conv_f32_to_f128$local, .Lfunc_end472-test_conv_f32_to_f128
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
	alloc 2
	cvt_f32_f16 %r1, %r1
	retf 0

.Lfunc_end473:
	.size	test_conv_f32_to_f16, .Lfunc_end473-test_conv_f32_to_f16
	.size	.Ltest_conv_f32_to_f16$local, .Lfunc_end473-test_conv_f32_to_f16
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
	alloc 2
	cvt_f64_f16 %r1, %r1
	retf 0

.Lfunc_end474:
	.size	test_conv_f64_to_f16, .Lfunc_end474-test_conv_f64_to_f16
	.size	.Ltest_conv_f64_to_f16$local, .Lfunc_end474-test_conv_f64_to_f16
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
	alloc 2
	cvt_f64_f32 %r1, %r1
	retf 0

.Lfunc_end475:
	.size	test_conv_f64_to_f32, .Lfunc_end475-test_conv_f64_to_f32
	.size	.Ltest_conv_f64_to_f32$local, .Lfunc_end475-test_conv_f64_to_f32
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
	alloc 2
	cvt_f128_f16 %r1, %r1
	retf 0

.Lfunc_end476:
	.size	test_conv_f128_to_f16, .Lfunc_end476-test_conv_f128_to_f16
	.size	.Ltest_conv_f128_to_f16$local, .Lfunc_end476-test_conv_f128_to_f16
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
	alloc 2
	cvt_f128_f32 %r1, %r1
	retf 0

.Lfunc_end477:
	.size	test_conv_f128_to_f32, .Lfunc_end477-test_conv_f128_to_f32
	.size	.Ltest_conv_f128_to_f32$local, .Lfunc_end477-test_conv_f128_to_f32
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
	alloc 2
	cvt_f128_f64 %r1, %r1
	retf 0

.Lfunc_end478:
	.size	test_conv_f128_to_f64, .Lfunc_end478-test_conv_f128_to_f64
	.size	.Ltest_conv_f128_to_f64$local, .Lfunc_end478-test_conv_f128_to_f64
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
	alloc 2
	trunk_f16_i32 %r1, %r1
	retf 0

.Lfunc_end479:
	.size	test_conv_f16_to_i32, .Lfunc_end479-test_conv_f16_to_i32
	.size	.Ltest_conv_f16_to_i32$local, .Lfunc_end479-test_conv_f16_to_i32
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
	alloc 2
	trunk_f16_u32 %r1, %r1
	retf 0

.Lfunc_end480:
	.size	test_conv_f16_to_u32, .Lfunc_end480-test_conv_f16_to_u32
	.size	.Ltest_conv_f16_to_u32$local, .Lfunc_end480-test_conv_f16_to_u32
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
	alloc 2
	trunk_f16_i64 %r1, %r1
	retf 0

.Lfunc_end481:
	.size	test_conv_f16_to_i64, .Lfunc_end481-test_conv_f16_to_i64
	.size	.Ltest_conv_f16_to_i64$local, .Lfunc_end481-test_conv_f16_to_i64
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
	alloc 2
	trunk_f16_u64 %r1, %r1
	retf 0

.Lfunc_end482:
	.size	test_conv_f16_to_u64, .Lfunc_end482-test_conv_f16_to_u64
	.size	.Ltest_conv_f16_to_u64$local, .Lfunc_end482-test_conv_f16_to_u64
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
	alloc 2
	trunk_f16_i128 %r1, %r1
	retf 0

.Lfunc_end483:
	.size	test_conv_f16_to_i128, .Lfunc_end483-test_conv_f16_to_i128
	.size	.Ltest_conv_f16_to_i128$local, .Lfunc_end483-test_conv_f16_to_i128
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
	alloc 2
	trunk_f16_u128 %r1, %r1
	retf 0

.Lfunc_end484:
	.size	test_conv_f16_to_u128, .Lfunc_end484-test_conv_f16_to_u128
	.size	.Ltest_conv_f16_to_u128$local, .Lfunc_end484-test_conv_f16_to_u128
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
	alloc 2
	cvt_i32_f16 %r1, %r1
	retf 0

.Lfunc_end485:
	.size	test_conv_i32_to_f16, .Lfunc_end485-test_conv_i32_to_f16
	.size	.Ltest_conv_i32_to_f16$local, .Lfunc_end485-test_conv_i32_to_f16
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
	alloc 2
	cvt_u32_f16 %r1, %r1
	retf 0

.Lfunc_end486:
	.size	test_conv_u32_to_f16, .Lfunc_end486-test_conv_u32_to_f16
	.size	.Ltest_conv_u32_to_f16$local, .Lfunc_end486-test_conv_u32_to_f16
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
	alloc 2
	cvt_i64_f16 %r1, %r1
	retf 0

.Lfunc_end487:
	.size	test_conv_i64_to_f16, .Lfunc_end487-test_conv_i64_to_f16
	.size	.Ltest_conv_i64_to_f16$local, .Lfunc_end487-test_conv_i64_to_f16
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
	alloc 2
	cvt_u64_f16 %r1, %r1
	retf 0

.Lfunc_end488:
	.size	test_conv_u64_to_f16, .Lfunc_end488-test_conv_u64_to_f16
	.size	.Ltest_conv_u64_to_f16$local, .Lfunc_end488-test_conv_u64_to_f16
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
	alloc 2
	cvt_i128_f16 %r1, %r1
	retf 0

.Lfunc_end489:
	.size	test_conv_i128_to_f16, .Lfunc_end489-test_conv_i128_to_f16
	.size	.Ltest_conv_i128_to_f16$local, .Lfunc_end489-test_conv_i128_to_f16
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
	alloc 2
	cvt_u128_f16 %r1, %r1
	retf 0

.Lfunc_end490:
	.size	test_conv_u128_to_f16, .Lfunc_end490-test_conv_u128_to_f16
	.size	.Ltest_conv_u128_to_f16$local, .Lfunc_end490-test_conv_u128_to_f16
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
	alloc 2
	trunk_f32_i32 %r1, %r1
	retf 0

.Lfunc_end491:
	.size	test_conv_f32_to_i32, .Lfunc_end491-test_conv_f32_to_i32
	.size	.Ltest_conv_f32_to_i32$local, .Lfunc_end491-test_conv_f32_to_i32
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
	alloc 2
	trunk_f32_u32 %r1, %r1
	retf 0

.Lfunc_end492:
	.size	test_conv_f32_to_u32, .Lfunc_end492-test_conv_f32_to_u32
	.size	.Ltest_conv_f32_to_u32$local, .Lfunc_end492-test_conv_f32_to_u32
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
	alloc 2
	trunk_f32_i64 %r1, %r1
	retf 0

.Lfunc_end493:
	.size	test_conv_f32_to_i64, .Lfunc_end493-test_conv_f32_to_i64
	.size	.Ltest_conv_f32_to_i64$local, .Lfunc_end493-test_conv_f32_to_i64
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
	alloc 2
	trunk_f32_u64 %r1, %r1
	retf 0

.Lfunc_end494:
	.size	test_conv_f32_to_u64, .Lfunc_end494-test_conv_f32_to_u64
	.size	.Ltest_conv_f32_to_u64$local, .Lfunc_end494-test_conv_f32_to_u64
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
	alloc 2
	trunk_f32_i128 %r1, %r1
	retf 0

.Lfunc_end495:
	.size	test_conv_f32_to_i128, .Lfunc_end495-test_conv_f32_to_i128
	.size	.Ltest_conv_f32_to_i128$local, .Lfunc_end495-test_conv_f32_to_i128
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
	alloc 2
	trunk_f32_u128 %r1, %r1
	retf 0

.Lfunc_end496:
	.size	test_conv_f32_to_u128, .Lfunc_end496-test_conv_f32_to_u128
	.size	.Ltest_conv_f32_to_u128$local, .Lfunc_end496-test_conv_f32_to_u128
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
	alloc 2
	cvt_i32_f32 %r1, %r1
	retf 0

.Lfunc_end497:
	.size	test_conv_i32_to_f32, .Lfunc_end497-test_conv_i32_to_f32
	.size	.Ltest_conv_i32_to_f32$local, .Lfunc_end497-test_conv_i32_to_f32
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
	alloc 2
	cvt_u32_f32 %r1, %r1
	retf 0

.Lfunc_end498:
	.size	test_conv_u32_to_f32, .Lfunc_end498-test_conv_u32_to_f32
	.size	.Ltest_conv_u32_to_f32$local, .Lfunc_end498-test_conv_u32_to_f32
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
	alloc 2
	cvt_i64_f32 %r1, %r1
	retf 0

.Lfunc_end499:
	.size	test_conv_i64_to_f32, .Lfunc_end499-test_conv_i64_to_f32
	.size	.Ltest_conv_i64_to_f32$local, .Lfunc_end499-test_conv_i64_to_f32
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
	alloc 2
	cvt_u64_f32 %r1, %r1
	retf 0

.Lfunc_end500:
	.size	test_conv_u64_to_f32, .Lfunc_end500-test_conv_u64_to_f32
	.size	.Ltest_conv_u64_to_f32$local, .Lfunc_end500-test_conv_u64_to_f32
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
	alloc 2
	cvt_i128_f32 %r1, %r1
	retf 0

.Lfunc_end501:
	.size	test_conv_i128_to_f32, .Lfunc_end501-test_conv_i128_to_f32
	.size	.Ltest_conv_i128_to_f32$local, .Lfunc_end501-test_conv_i128_to_f32
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
	alloc 2
	cvt_u128_f32 %r1, %r1
	retf 0

.Lfunc_end502:
	.size	test_conv_u128_to_f32, .Lfunc_end502-test_conv_u128_to_f32
	.size	.Ltest_conv_u128_to_f32$local, .Lfunc_end502-test_conv_u128_to_f32
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
	alloc 2
	trunk_f64_i32 %r1, %r1
	retf 0

.Lfunc_end503:
	.size	test_conv_f64_to_i32, .Lfunc_end503-test_conv_f64_to_i32
	.size	.Ltest_conv_f64_to_i32$local, .Lfunc_end503-test_conv_f64_to_i32
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
	alloc 2
	trunk_f64_u32 %r1, %r1
	retf 0

.Lfunc_end504:
	.size	test_conv_f64_to_u32, .Lfunc_end504-test_conv_f64_to_u32
	.size	.Ltest_conv_f64_to_u32$local, .Lfunc_end504-test_conv_f64_to_u32
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
	alloc 2
	trunk_f64_i64 %r1, %r1
	retf 0

.Lfunc_end505:
	.size	test_conv_f64_to_i64, .Lfunc_end505-test_conv_f64_to_i64
	.size	.Ltest_conv_f64_to_i64$local, .Lfunc_end505-test_conv_f64_to_i64
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
	alloc 2
	trunk_f64_u64 %r1, %r1
	retf 0

.Lfunc_end506:
	.size	test_conv_f64_to_u64, .Lfunc_end506-test_conv_f64_to_u64
	.size	.Ltest_conv_f64_to_u64$local, .Lfunc_end506-test_conv_f64_to_u64
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
	alloc 2
	trunk_f64_i128 %r1, %r1
	retf 0

.Lfunc_end507:
	.size	test_conv_f64_to_i128, .Lfunc_end507-test_conv_f64_to_i128
	.size	.Ltest_conv_f64_to_i128$local, .Lfunc_end507-test_conv_f64_to_i128
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
	alloc 2
	trunk_f64_u128 %r1, %r1
	retf 0

.Lfunc_end508:
	.size	test_conv_f64_to_u128, .Lfunc_end508-test_conv_f64_to_u128
	.size	.Ltest_conv_f64_to_u128$local, .Lfunc_end508-test_conv_f64_to_u128
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
	alloc 2
	cvt_i32_f64 %r1, %r1
	retf 0

.Lfunc_end509:
	.size	test_conv_i32_to_f64, .Lfunc_end509-test_conv_i32_to_f64
	.size	.Ltest_conv_i32_to_f64$local, .Lfunc_end509-test_conv_i32_to_f64
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
	alloc 2
	cvt_u32_f64 %r1, %r1
	retf 0

.Lfunc_end510:
	.size	test_conv_u32_to_f64, .Lfunc_end510-test_conv_u32_to_f64
	.size	.Ltest_conv_u32_to_f64$local, .Lfunc_end510-test_conv_u32_to_f64
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
	alloc 2
	cvt_i64_f64 %r1, %r1
	retf 0

.Lfunc_end511:
	.size	test_conv_i64_to_f64, .Lfunc_end511-test_conv_i64_to_f64
	.size	.Ltest_conv_i64_to_f64$local, .Lfunc_end511-test_conv_i64_to_f64
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
	alloc 2
	cvt_u64_f64 %r1, %r1
	retf 0

.Lfunc_end512:
	.size	test_conv_u64_to_f64, .Lfunc_end512-test_conv_u64_to_f64
	.size	.Ltest_conv_u64_to_f64$local, .Lfunc_end512-test_conv_u64_to_f64
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
	alloc 2
	cvt_i128_f64 %r1, %r1
	retf 0

.Lfunc_end513:
	.size	test_conv_i128_to_f64, .Lfunc_end513-test_conv_i128_to_f64
	.size	.Ltest_conv_i128_to_f64$local, .Lfunc_end513-test_conv_i128_to_f64
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
	alloc 2
	cvt_u128_f64 %r1, %r1
	retf 0

.Lfunc_end514:
	.size	test_conv_u128_to_f64, .Lfunc_end514-test_conv_u128_to_f64
	.size	.Ltest_conv_u128_to_f64$local, .Lfunc_end514-test_conv_u128_to_f64
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
	alloc 2
	trunk_f128_i32 %r1, %r1
	retf 0

.Lfunc_end515:
	.size	test_conv_f128_to_i32, .Lfunc_end515-test_conv_f128_to_i32
	.size	.Ltest_conv_f128_to_i32$local, .Lfunc_end515-test_conv_f128_to_i32
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
	alloc 2
	trunk_f128_u32 %r1, %r1
	retf 0

.Lfunc_end516:
	.size	test_conv_f128_to_u32, .Lfunc_end516-test_conv_f128_to_u32
	.size	.Ltest_conv_f128_to_u32$local, .Lfunc_end516-test_conv_f128_to_u32
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
	alloc 2
	trunk_f128_i64 %r1, %r1
	retf 0

.Lfunc_end517:
	.size	test_conv_f128_to_i64, .Lfunc_end517-test_conv_f128_to_i64
	.size	.Ltest_conv_f128_to_i64$local, .Lfunc_end517-test_conv_f128_to_i64
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
	alloc 2
	trunk_f128_u64 %r1, %r1
	retf 0

.Lfunc_end518:
	.size	test_conv_f128_to_u64, .Lfunc_end518-test_conv_f128_to_u64
	.size	.Ltest_conv_f128_to_u64$local, .Lfunc_end518-test_conv_f128_to_u64
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
	alloc 2
	trunk_f128_i128 %r1, %r1
	retf 0

.Lfunc_end519:
	.size	test_conv_f128_to_i128, .Lfunc_end519-test_conv_f128_to_i128
	.size	.Ltest_conv_f128_to_i128$local, .Lfunc_end519-test_conv_f128_to_i128
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
	alloc 2
	trunk_f128_u128 %r1, %r1
	retf 0

.Lfunc_end520:
	.size	test_conv_f128_to_u128, .Lfunc_end520-test_conv_f128_to_u128
	.size	.Ltest_conv_f128_to_u128$local, .Lfunc_end520-test_conv_f128_to_u128
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
	alloc 2
	cvt_i32_f128 %r1, %r1
	retf 0

.Lfunc_end521:
	.size	test_conv_i32_to_f128, .Lfunc_end521-test_conv_i32_to_f128
	.size	.Ltest_conv_i32_to_f128$local, .Lfunc_end521-test_conv_i32_to_f128
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
	alloc 2
	cvt_u32_f128 %r1, %r1
	retf 0

.Lfunc_end522:
	.size	test_conv_u32_to_f128, .Lfunc_end522-test_conv_u32_to_f128
	.size	.Ltest_conv_u32_to_f128$local, .Lfunc_end522-test_conv_u32_to_f128
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
	alloc 2
	cvt_i64_f128 %r1, %r1
	retf 0

.Lfunc_end523:
	.size	test_conv_i64_to_f128, .Lfunc_end523-test_conv_i64_to_f128
	.size	.Ltest_conv_i64_to_f128$local, .Lfunc_end523-test_conv_i64_to_f128
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
	alloc 2
	cvt_u64_f128 %r1, %r1
	retf 0

.Lfunc_end524:
	.size	test_conv_u64_to_f128, .Lfunc_end524-test_conv_u64_to_f128
	.size	.Ltest_conv_u64_to_f128$local, .Lfunc_end524-test_conv_u64_to_f128
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
	alloc 2
	cvt_i128_f128 %r1, %r1
	retf 0

.Lfunc_end525:
	.size	test_conv_i128_to_f128, .Lfunc_end525-test_conv_i128_to_f128
	.size	.Ltest_conv_i128_to_f128$local, .Lfunc_end525-test_conv_i128_to_f128
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
	alloc 2
	cvt_u128_f128 %r1, %r1
	retf 0

.Lfunc_end526:
	.size	test_conv_u128_to_f128, .Lfunc_end526-test_conv_u128_to_f128
	.size	.Ltest_conv_u128_to_f128$local, .Lfunc_end526-test_conv_u128_to_f128
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
	alloc 2
	ld_imm %r1, 12263
	retf 0

.Lfunc_end527:
	.size	test_fp_immediate_f16, .Lfunc_end527-test_fp_immediate_f16
	.size	.Ltest_fp_immediate_f16$local, .Lfunc_end527-test_fp_immediate_f16
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
	alloc 2
	ld_imm.l %r1, 1039980160

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end528:
	.size	test_fp_immediate_f32, .Lfunc_end528-test_fp_immediate_f32
	.size	.Ltest_fp_immediate_f32$local, .Lfunc_end528-test_fp_immediate_f32
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
	alloc 2
	ld_imm.l %r1, -4629811673862064449

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end529:
	.size	test_fp_immediate_f64, .Lfunc_end529-test_fp_immediate_f64
	.size	.Ltest_fp_immediate_f64$local, .Lfunc_end529-test_fp_immediate_f64
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
	alloc 2
	ld_imm.l %r1, -845303600433666493

	! BUNDLE
	ld_imm_high.l %r1, 4610553164962720619
	retf 0

.Lfunc_end530:
	.size	test_fp_immediate_f128, .Lfunc_end530-test_fp_immediate_f128
	.size	.Ltest_fp_immediate_f128$local, .Lfunc_end530-test_fp_immediate_f128
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
	alloc 2
	ld_imm %r1, 0
	nop 2

	! BUNDLE
	ld_imm_high.l %r1, 4612635996473786368
	retf 0

.Lfunc_end531:
	.size	test_fp_immediate_f128_int, .Lfunc_end531-test_fp_immediate_f128_int
	.size	.Ltest_fp_immediate_f128_int$local, .Lfunc_end531-test_fp_immediate_f128_int
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
	alloc 2
	ld_imm %r1, 0
	nop 2

	! BUNDLE
	ld_imm_high.l %r1, -8961037358560444416
	retf 0

.Lfunc_end532:
	.size	test_fp_immediate_f128_denorm, .Lfunc_end532-test_fp_immediate_f128_denorm
	.size	.Ltest_fp_immediate_f128_denorm$local, .Lfunc_end532-test_fp_immediate_f128_denorm
                                        ! -- End function
	.globl	builtin_fmin_f16                ! -- Begin function builtin_fmin_f16
	.p2align	4
	.type	builtin_fmin_f16,@function
builtin_fmin_f16:                       ! @builtin_fmin_f16
.Lbuiltin_fmin_f16$local:
	.type	.Lbuiltin_fmin_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end533:
	.size	builtin_fmin_f16, .Lfunc_end533-builtin_fmin_f16
	.size	.Lbuiltin_fmin_f16$local, .Lfunc_end533-builtin_fmin_f16
                                        ! -- End function
	.globl	min_le_f16                      ! -- Begin function min_le_f16
	.p2align	4
	.type	min_le_f16,@function
min_le_f16:                             ! @min_le_f16
.Lmin_le_f16$local:
	.type	.Lmin_le_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end534:
	.size	min_le_f16, .Lfunc_end534-min_le_f16
	.size	.Lmin_le_f16$local, .Lfunc_end534-min_le_f16
                                        ! -- End function
	.globl	min_gt_f16                      ! -- Begin function min_gt_f16
	.p2align	4
	.type	min_gt_f16,@function
min_gt_f16:                             ! @min_gt_f16
.Lmin_gt_f16$local:
	.type	.Lmin_gt_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f16 %r1, %r2, %r1
	retf 0

.Lfunc_end535:
	.size	min_gt_f16, .Lfunc_end535-min_gt_f16
	.size	.Lmin_gt_f16$local, .Lfunc_end535-min_gt_f16
                                        ! -- End function
	.globl	min_ge_f16                      ! -- Begin function min_ge_f16
	.p2align	4
	.type	min_ge_f16,@function
min_ge_f16:                             ! @min_ge_f16
.Lmin_ge_f16$local:
	.type	.Lmin_ge_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f16 %r1, %r2, %r1
	retf 0

.Lfunc_end536:
	.size	min_ge_f16, .Lfunc_end536-min_ge_f16
	.size	.Lmin_ge_f16$local, .Lfunc_end536-min_ge_f16
                                        ! -- End function
	.globl	builtin_fmax_f16                ! -- Begin function builtin_fmax_f16
	.p2align	4
	.type	builtin_fmax_f16,@function
builtin_fmax_f16:                       ! @builtin_fmax_f16
.Lbuiltin_fmax_f16$local:
	.type	.Lbuiltin_fmax_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end537:
	.size	builtin_fmax_f16, .Lfunc_end537-builtin_fmax_f16
	.size	.Lbuiltin_fmax_f16$local, .Lfunc_end537-builtin_fmax_f16
                                        ! -- End function
	.globl	max_lt_f16                      ! -- Begin function max_lt_f16
	.p2align	4
	.type	max_lt_f16,@function
max_lt_f16:                             ! @max_lt_f16
.Lmax_lt_f16$local:
	.type	.Lmax_lt_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end538:
	.size	max_lt_f16, .Lfunc_end538-max_lt_f16
	.size	.Lmax_lt_f16$local, .Lfunc_end538-max_lt_f16
                                        ! -- End function
	.globl	max_le_f16                      ! -- Begin function max_le_f16
	.p2align	4
	.type	max_le_f16,@function
max_le_f16:                             ! @max_le_f16
.Lmax_le_f16$local:
	.type	.Lmax_le_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f16 %r1, %r1, %r2
	retf 0

.Lfunc_end539:
	.size	max_le_f16, .Lfunc_end539-max_le_f16
	.size	.Lmax_le_f16$local, .Lfunc_end539-max_le_f16
                                        ! -- End function
	.globl	max_gt_f16                      ! -- Begin function max_gt_f16
	.p2align	4
	.type	max_gt_f16,@function
max_gt_f16:                             ! @max_gt_f16
.Lmax_gt_f16$local:
	.type	.Lmax_gt_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f16 %r1, %r2, %r1
	retf 0

.Lfunc_end540:
	.size	max_gt_f16, .Lfunc_end540-max_gt_f16
	.size	.Lmax_gt_f16$local, .Lfunc_end540-max_gt_f16
                                        ! -- End function
	.globl	max_ge_f16                      ! -- Begin function max_ge_f16
	.p2align	4
	.type	max_ge_f16,@function
max_ge_f16:                             ! @max_ge_f16
.Lmax_ge_f16$local:
	.type	.Lmax_ge_f16$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f16 %r1, %r2, %r1
	retf 0

.Lfunc_end541:
	.size	max_ge_f16, .Lfunc_end541-max_ge_f16
	.size	.Lmax_ge_f16$local, .Lfunc_end541-max_ge_f16
                                        ! -- End function
	.globl	builtin_fmin_f32                ! -- Begin function builtin_fmin_f32
	.p2align	4
	.type	builtin_fmin_f32,@function
builtin_fmin_f32:                       ! @builtin_fmin_f32
.Lbuiltin_fmin_f32$local:
	.type	.Lbuiltin_fmin_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end542:
	.size	builtin_fmin_f32, .Lfunc_end542-builtin_fmin_f32
	.size	.Lbuiltin_fmin_f32$local, .Lfunc_end542-builtin_fmin_f32
                                        ! -- End function
	.globl	min_le_f32                      ! -- Begin function min_le_f32
	.p2align	4
	.type	min_le_f32,@function
min_le_f32:                             ! @min_le_f32
.Lmin_le_f32$local:
	.type	.Lmin_le_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end543:
	.size	min_le_f32, .Lfunc_end543-min_le_f32
	.size	.Lmin_le_f32$local, .Lfunc_end543-min_le_f32
                                        ! -- End function
	.globl	min_gt_f32                      ! -- Begin function min_gt_f32
	.p2align	4
	.type	min_gt_f32,@function
min_gt_f32:                             ! @min_gt_f32
.Lmin_gt_f32$local:
	.type	.Lmin_gt_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f32 %r1, %r2, %r1
	retf 0

.Lfunc_end544:
	.size	min_gt_f32, .Lfunc_end544-min_gt_f32
	.size	.Lmin_gt_f32$local, .Lfunc_end544-min_gt_f32
                                        ! -- End function
	.globl	min_ge_f32                      ! -- Begin function min_ge_f32
	.p2align	4
	.type	min_ge_f32,@function
min_ge_f32:                             ! @min_ge_f32
.Lmin_ge_f32$local:
	.type	.Lmin_ge_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f32 %r1, %r2, %r1
	retf 0

.Lfunc_end545:
	.size	min_ge_f32, .Lfunc_end545-min_ge_f32
	.size	.Lmin_ge_f32$local, .Lfunc_end545-min_ge_f32
                                        ! -- End function
	.globl	builtin_fmax_f32                ! -- Begin function builtin_fmax_f32
	.p2align	4
	.type	builtin_fmax_f32,@function
builtin_fmax_f32:                       ! @builtin_fmax_f32
.Lbuiltin_fmax_f32$local:
	.type	.Lbuiltin_fmax_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end546:
	.size	builtin_fmax_f32, .Lfunc_end546-builtin_fmax_f32
	.size	.Lbuiltin_fmax_f32$local, .Lfunc_end546-builtin_fmax_f32
                                        ! -- End function
	.globl	max_lt_f32                      ! -- Begin function max_lt_f32
	.p2align	4
	.type	max_lt_f32,@function
max_lt_f32:                             ! @max_lt_f32
.Lmax_lt_f32$local:
	.type	.Lmax_lt_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end547:
	.size	max_lt_f32, .Lfunc_end547-max_lt_f32
	.size	.Lmax_lt_f32$local, .Lfunc_end547-max_lt_f32
                                        ! -- End function
	.globl	max_le_f32                      ! -- Begin function max_le_f32
	.p2align	4
	.type	max_le_f32,@function
max_le_f32:                             ! @max_le_f32
.Lmax_le_f32$local:
	.type	.Lmax_le_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end548:
	.size	max_le_f32, .Lfunc_end548-max_le_f32
	.size	.Lmax_le_f32$local, .Lfunc_end548-max_le_f32
                                        ! -- End function
	.globl	max_gt_f32                      ! -- Begin function max_gt_f32
	.p2align	4
	.type	max_gt_f32,@function
max_gt_f32:                             ! @max_gt_f32
.Lmax_gt_f32$local:
	.type	.Lmax_gt_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f32 %r1, %r2, %r1
	retf 0

.Lfunc_end549:
	.size	max_gt_f32, .Lfunc_end549-max_gt_f32
	.size	.Lmax_gt_f32$local, .Lfunc_end549-max_gt_f32
                                        ! -- End function
	.globl	max_ge_f32                      ! -- Begin function max_ge_f32
	.p2align	4
	.type	max_ge_f32,@function
max_ge_f32:                             ! @max_ge_f32
.Lmax_ge_f32$local:
	.type	.Lmax_ge_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f32 %r1, %r2, %r1
	retf 0

.Lfunc_end550:
	.size	max_ge_f32, .Lfunc_end550-max_ge_f32
	.size	.Lmax_ge_f32$local, .Lfunc_end550-max_ge_f32
                                        ! -- End function
	.globl	builtin_fmin_f64                ! -- Begin function builtin_fmin_f64
	.p2align	4
	.type	builtin_fmin_f64,@function
builtin_fmin_f64:                       ! @builtin_fmin_f64
.Lbuiltin_fmin_f64$local:
	.type	.Lbuiltin_fmin_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end551:
	.size	builtin_fmin_f64, .Lfunc_end551-builtin_fmin_f64
	.size	.Lbuiltin_fmin_f64$local, .Lfunc_end551-builtin_fmin_f64
                                        ! -- End function
	.globl	min_le_f64                      ! -- Begin function min_le_f64
	.p2align	4
	.type	min_le_f64,@function
min_le_f64:                             ! @min_le_f64
.Lmin_le_f64$local:
	.type	.Lmin_le_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end552:
	.size	min_le_f64, .Lfunc_end552-min_le_f64
	.size	.Lmin_le_f64$local, .Lfunc_end552-min_le_f64
                                        ! -- End function
	.globl	min_gt_f64                      ! -- Begin function min_gt_f64
	.p2align	4
	.type	min_gt_f64,@function
min_gt_f64:                             ! @min_gt_f64
.Lmin_gt_f64$local:
	.type	.Lmin_gt_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f64 %r1, %r2, %r1
	retf 0

.Lfunc_end553:
	.size	min_gt_f64, .Lfunc_end553-min_gt_f64
	.size	.Lmin_gt_f64$local, .Lfunc_end553-min_gt_f64
                                        ! -- End function
	.globl	min_ge_f64                      ! -- Begin function min_ge_f64
	.p2align	4
	.type	min_ge_f64,@function
min_ge_f64:                             ! @min_ge_f64
.Lmin_ge_f64$local:
	.type	.Lmin_ge_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f64 %r1, %r2, %r1
	retf 0

.Lfunc_end554:
	.size	min_ge_f64, .Lfunc_end554-min_ge_f64
	.size	.Lmin_ge_f64$local, .Lfunc_end554-min_ge_f64
                                        ! -- End function
	.globl	builtin_fmax_f64                ! -- Begin function builtin_fmax_f64
	.p2align	4
	.type	builtin_fmax_f64,@function
builtin_fmax_f64:                       ! @builtin_fmax_f64
.Lbuiltin_fmax_f64$local:
	.type	.Lbuiltin_fmax_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end555:
	.size	builtin_fmax_f64, .Lfunc_end555-builtin_fmax_f64
	.size	.Lbuiltin_fmax_f64$local, .Lfunc_end555-builtin_fmax_f64
                                        ! -- End function
	.globl	max_lt_f64                      ! -- Begin function max_lt_f64
	.p2align	4
	.type	max_lt_f64,@function
max_lt_f64:                             ! @max_lt_f64
.Lmax_lt_f64$local:
	.type	.Lmax_lt_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end556:
	.size	max_lt_f64, .Lfunc_end556-max_lt_f64
	.size	.Lmax_lt_f64$local, .Lfunc_end556-max_lt_f64
                                        ! -- End function
	.globl	max_le_f64                      ! -- Begin function max_le_f64
	.p2align	4
	.type	max_le_f64,@function
max_le_f64:                             ! @max_le_f64
.Lmax_le_f64$local:
	.type	.Lmax_le_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end557:
	.size	max_le_f64, .Lfunc_end557-max_le_f64
	.size	.Lmax_le_f64$local, .Lfunc_end557-max_le_f64
                                        ! -- End function
	.globl	max_gt_f64                      ! -- Begin function max_gt_f64
	.p2align	4
	.type	max_gt_f64,@function
max_gt_f64:                             ! @max_gt_f64
.Lmax_gt_f64$local:
	.type	.Lmax_gt_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f64 %r1, %r2, %r1
	retf 0

.Lfunc_end558:
	.size	max_gt_f64, .Lfunc_end558-max_gt_f64
	.size	.Lmax_gt_f64$local, .Lfunc_end558-max_gt_f64
                                        ! -- End function
	.globl	max_ge_f64                      ! -- Begin function max_ge_f64
	.p2align	4
	.type	max_ge_f64,@function
max_ge_f64:                             ! @max_ge_f64
.Lmax_ge_f64$local:
	.type	.Lmax_ge_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f64 %r1, %r2, %r1
	retf 0

.Lfunc_end559:
	.size	max_ge_f64, .Lfunc_end559-max_ge_f64
	.size	.Lmax_ge_f64$local, .Lfunc_end559-max_ge_f64
                                        ! -- End function
	.globl	builtin_fmin_f128               ! -- Begin function builtin_fmin_f128
	.p2align	4
	.type	builtin_fmin_f128,@function
builtin_fmin_f128:                      ! @builtin_fmin_f128
.Lbuiltin_fmin_f128$local:
	.type	.Lbuiltin_fmin_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end560:
	.size	builtin_fmin_f128, .Lfunc_end560-builtin_fmin_f128
	.size	.Lbuiltin_fmin_f128$local, .Lfunc_end560-builtin_fmin_f128
                                        ! -- End function
	.globl	min_le_f128                     ! -- Begin function min_le_f128
	.p2align	4
	.type	min_le_f128,@function
min_le_f128:                            ! @min_le_f128
.Lmin_le_f128$local:
	.type	.Lmin_le_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end561:
	.size	min_le_f128, .Lfunc_end561-min_le_f128
	.size	.Lmin_le_f128$local, .Lfunc_end561-min_le_f128
                                        ! -- End function
	.globl	min_gt_f128                     ! -- Begin function min_gt_f128
	.p2align	4
	.type	min_gt_f128,@function
min_gt_f128:                            ! @min_gt_f128
.Lmin_gt_f128$local:
	.type	.Lmin_gt_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f128 %r1, %r2, %r1
	retf 0

.Lfunc_end562:
	.size	min_gt_f128, .Lfunc_end562-min_gt_f128
	.size	.Lmin_gt_f128$local, .Lfunc_end562-min_gt_f128
                                        ! -- End function
	.globl	min_ge_f128                     ! -- Begin function min_ge_f128
	.p2align	4
	.type	min_ge_f128,@function
min_ge_f128:                            ! @min_ge_f128
.Lmin_ge_f128$local:
	.type	.Lmin_ge_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	minnum_f128 %r1, %r2, %r1
	retf 0

.Lfunc_end563:
	.size	min_ge_f128, .Lfunc_end563-min_ge_f128
	.size	.Lmin_ge_f128$local, .Lfunc_end563-min_ge_f128
                                        ! -- End function
	.globl	builtin_fmax_f128               ! -- Begin function builtin_fmax_f128
	.p2align	4
	.type	builtin_fmax_f128,@function
builtin_fmax_f128:                      ! @builtin_fmax_f128
.Lbuiltin_fmax_f128$local:
	.type	.Lbuiltin_fmax_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end564:
	.size	builtin_fmax_f128, .Lfunc_end564-builtin_fmax_f128
	.size	.Lbuiltin_fmax_f128$local, .Lfunc_end564-builtin_fmax_f128
                                        ! -- End function
	.globl	max_lt_f128                     ! -- Begin function max_lt_f128
	.p2align	4
	.type	max_lt_f128,@function
max_lt_f128:                            ! @max_lt_f128
.Lmax_lt_f128$local:
	.type	.Lmax_lt_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end565:
	.size	max_lt_f128, .Lfunc_end565-max_lt_f128
	.size	.Lmax_lt_f128$local, .Lfunc_end565-max_lt_f128
                                        ! -- End function
	.globl	max_le_f128                     ! -- Begin function max_le_f128
	.p2align	4
	.type	max_le_f128,@function
max_le_f128:                            ! @max_le_f128
.Lmax_le_f128$local:
	.type	.Lmax_le_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end566:
	.size	max_le_f128, .Lfunc_end566-max_le_f128
	.size	.Lmax_le_f128$local, .Lfunc_end566-max_le_f128
                                        ! -- End function
	.globl	max_gt_f128                     ! -- Begin function max_gt_f128
	.p2align	4
	.type	max_gt_f128,@function
max_gt_f128:                            ! @max_gt_f128
.Lmax_gt_f128$local:
	.type	.Lmax_gt_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f128 %r1, %r2, %r1
	retf 0

.Lfunc_end567:
	.size	max_gt_f128, .Lfunc_end567-max_gt_f128
	.size	.Lmax_gt_f128$local, .Lfunc_end567-max_gt_f128
                                        ! -- End function
	.globl	max_ge_f128                     ! -- Begin function max_ge_f128
	.p2align	4
	.type	max_ge_f128,@function
max_ge_f128:                            ! @max_ge_f128
.Lmax_ge_f128$local:
	.type	.Lmax_ge_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	maxnum_f128 %r1, %r2, %r1
	retf 0

.Lfunc_end568:
	.size	max_ge_f128, .Lfunc_end568-max_ge_f128
	.size	.Lmax_ge_f128$local, .Lfunc_end568-max_ge_f128
                                        ! -- End function
	.globl	test_roundeven_f32              ! -- Begin function test_roundeven_f32
	.p2align	4
	.type	test_roundeven_f32,@function
test_roundeven_f32:                     ! @test_roundeven_f32
.Ltest_roundeven_f32$local:
	.type	.Ltest_roundeven_f32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	roundeven_f32 %r1, %r1
	retf 0

.Lfunc_end569:
	.size	test_roundeven_f32, .Lfunc_end569-test_roundeven_f32
	.size	.Ltest_roundeven_f32$local, .Lfunc_end569-test_roundeven_f32
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
	alloc 2
	round_f32 %r1, %r1
	retf 0

.Lfunc_end570:
	.size	test_round_f32, .Lfunc_end570-test_round_f32
	.size	.Ltest_round_f32$local, .Lfunc_end570-test_round_f32
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
	alloc 2
	floor_f32 %r1, %r1
	retf 0

.Lfunc_end571:
	.size	test_floor_f32, .Lfunc_end571-test_floor_f32
	.size	.Ltest_floor_f32$local, .Lfunc_end571-test_floor_f32
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
	alloc 2
	ceil_f32 %r1, %r1
	retf 0

.Lfunc_end572:
	.size	test_ceil_f32, .Lfunc_end572-test_ceil_f32
	.size	.Ltest_ceil_f32$local, .Lfunc_end572-test_ceil_f32
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
	alloc 2
	trunk_f32 %r1, %r1
	retf 0

.Lfunc_end573:
	.size	test_trunc_f32, .Lfunc_end573-test_trunc_f32
	.size	.Ltest_trunc_f32$local, .Lfunc_end573-test_trunc_f32
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
	alloc 2
	nearbyint_f32 %r1, %r1
	retf 0

.Lfunc_end574:
	.size	test_nearbyint_f32, .Lfunc_end574-test_nearbyint_f32
	.size	.Ltest_nearbyint_f32$local, .Lfunc_end574-test_nearbyint_f32
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
	alloc 2
	rint_f32 %r1, %r1
	retf 0

.Lfunc_end575:
	.size	test_rint_f32, .Lfunc_end575-test_rint_f32
	.size	.Ltest_rint_f32$local, .Lfunc_end575-test_rint_f32
                                        ! -- End function
	.globl	test_roundeven_f64              ! -- Begin function test_roundeven_f64
	.p2align	4
	.type	test_roundeven_f64,@function
test_roundeven_f64:                     ! @test_roundeven_f64
.Ltest_roundeven_f64$local:
	.type	.Ltest_roundeven_f64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	roundeven_f64 %r1, %r1
	retf 0

.Lfunc_end576:
	.size	test_roundeven_f64, .Lfunc_end576-test_roundeven_f64
	.size	.Ltest_roundeven_f64$local, .Lfunc_end576-test_roundeven_f64
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
	alloc 2
	round_f64 %r1, %r1
	retf 0

.Lfunc_end577:
	.size	test_round_f64, .Lfunc_end577-test_round_f64
	.size	.Ltest_round_f64$local, .Lfunc_end577-test_round_f64
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
	alloc 2
	floor_f64 %r1, %r1
	retf 0

.Lfunc_end578:
	.size	test_floor_f64, .Lfunc_end578-test_floor_f64
	.size	.Ltest_floor_f64$local, .Lfunc_end578-test_floor_f64
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
	alloc 2
	ceil_f64 %r1, %r1
	retf 0

.Lfunc_end579:
	.size	test_ceil_f64, .Lfunc_end579-test_ceil_f64
	.size	.Ltest_ceil_f64$local, .Lfunc_end579-test_ceil_f64
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
	alloc 2
	trunk_f64 %r1, %r1
	retf 0

.Lfunc_end580:
	.size	test_trunc_f64, .Lfunc_end580-test_trunc_f64
	.size	.Ltest_trunc_f64$local, .Lfunc_end580-test_trunc_f64
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
	alloc 2
	nearbyint_f64 %r1, %r1
	retf 0

.Lfunc_end581:
	.size	test_nearbyint_f64, .Lfunc_end581-test_nearbyint_f64
	.size	.Ltest_nearbyint_f64$local, .Lfunc_end581-test_nearbyint_f64
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
	alloc 2
	rint_f64 %r1, %r1
	retf 0

.Lfunc_end582:
	.size	test_rint_f64, .Lfunc_end582-test_rint_f64
	.size	.Ltest_rint_f64$local, .Lfunc_end582-test_rint_f64
                                        ! -- End function
	.globl	test_roundeven_f128             ! -- Begin function test_roundeven_f128
	.p2align	4
	.type	test_roundeven_f128,@function
test_roundeven_f128:                    ! @test_roundeven_f128
.Ltest_roundeven_f128$local:
	.type	.Ltest_roundeven_f128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	roundeven_f128 %r1, %r1
	retf 0

.Lfunc_end583:
	.size	test_roundeven_f128, .Lfunc_end583-test_roundeven_f128
	.size	.Ltest_roundeven_f128$local, .Lfunc_end583-test_roundeven_f128
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
	alloc 2
	round_f128 %r1, %r1
	retf 0

.Lfunc_end584:
	.size	test_round_f128, .Lfunc_end584-test_round_f128
	.size	.Ltest_round_f128$local, .Lfunc_end584-test_round_f128
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
	alloc 2
	floor_f128 %r1, %r1
	retf 0

.Lfunc_end585:
	.size	test_floor_f128, .Lfunc_end585-test_floor_f128
	.size	.Ltest_floor_f128$local, .Lfunc_end585-test_floor_f128
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
	alloc 2
	ceil_f128 %r1, %r1
	retf 0

.Lfunc_end586:
	.size	test_ceil_f128, .Lfunc_end586-test_ceil_f128
	.size	.Ltest_ceil_f128$local, .Lfunc_end586-test_ceil_f128
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
	alloc 2
	trunk_f128 %r1, %r1
	retf 0

.Lfunc_end587:
	.size	test_trunc_f128, .Lfunc_end587-test_trunc_f128
	.size	.Ltest_trunc_f128$local, .Lfunc_end587-test_trunc_f128
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
	alloc 2
	nearbyint_f128 %r1, %r1
	retf 0

.Lfunc_end588:
	.size	test_nearbyint_f128, .Lfunc_end588-test_nearbyint_f128
	.size	.Ltest_nearbyint_f128$local, .Lfunc_end588-test_nearbyint_f128
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
	alloc 2
	rint_f128 %r1, %r1
	retf 0

.Lfunc_end589:
	.size	test_rint_f128, .Lfunc_end589-test_rint_f128
	.size	.Ltest_rint_f128$local, .Lfunc_end589-test_rint_f128
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
	alloc 2
	neg_f32 %r1, %r1
	retf 0

.Lfunc_end590:
	.size	test_fneg_f32, .Lfunc_end590-test_fneg_f32
	.size	.Ltest_fneg_f32$local, .Lfunc_end590-test_fneg_f32
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
	alloc 2
	abs_f32 %r1, %r1
	retf 0

.Lfunc_end591:
	.size	test_fabs_f32, .Lfunc_end591-test_fabs_f32
	.size	.Ltest_fabs_f32$local, .Lfunc_end591-test_fabs_f32
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
	alloc 2
	nabs_f32 %r1, %r1
	retf 0

.Lfunc_end592:
	.size	test_fnabs_f32, .Lfunc_end592-test_fnabs_f32
	.size	.Ltest_fnabs_f32$local, .Lfunc_end592-test_fnabs_f32
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
	alloc 3
	abs_diff_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end593:
	.size	test_fabsd_f32, .Lfunc_end593-test_fabsd_f32
	.size	.Ltest_fabsd_f32$local, .Lfunc_end593-test_fabsd_f32
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
	alloc 3
	nabs_diff_f32 %r1, %r1, %r2
	retf 0

.Lfunc_end594:
	.size	test_fnabsd_f32, .Lfunc_end594-test_fnabsd_f32
	.size	.Ltest_fnabsd_f32$local, .Lfunc_end594-test_fnabsd_f32
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
	alloc 2
	neg_f64 %r1, %r1
	retf 0

.Lfunc_end595:
	.size	test_fneg_f64, .Lfunc_end595-test_fneg_f64
	.size	.Ltest_fneg_f64$local, .Lfunc_end595-test_fneg_f64
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
	alloc 2
	abs_f64 %r1, %r1
	retf 0

.Lfunc_end596:
	.size	test_fabs_f64, .Lfunc_end596-test_fabs_f64
	.size	.Ltest_fabs_f64$local, .Lfunc_end596-test_fabs_f64
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
	alloc 2
	nabs_f64 %r1, %r1
	retf 0

.Lfunc_end597:
	.size	test_fnabs_f64, .Lfunc_end597-test_fnabs_f64
	.size	.Ltest_fnabs_f64$local, .Lfunc_end597-test_fnabs_f64
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
	alloc 3
	abs_diff_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end598:
	.size	test_fabsd_f64, .Lfunc_end598-test_fabsd_f64
	.size	.Ltest_fabsd_f64$local, .Lfunc_end598-test_fabsd_f64
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
	alloc 3
	nabs_diff_f64 %r1, %r1, %r2
	retf 0

.Lfunc_end599:
	.size	test_fnabsd_f64, .Lfunc_end599-test_fnabsd_f64
	.size	.Ltest_fnabsd_f64$local, .Lfunc_end599-test_fnabsd_f64
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
	alloc 2
	neg_f128 %r1, %r1
	retf 0

.Lfunc_end600:
	.size	test_fneg_f128, .Lfunc_end600-test_fneg_f128
	.size	.Ltest_fneg_f128$local, .Lfunc_end600-test_fneg_f128
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
	alloc 2
	abs_f128 %r1, %r1
	retf 0

.Lfunc_end601:
	.size	test_fabs_f128, .Lfunc_end601-test_fabs_f128
	.size	.Ltest_fabs_f128$local, .Lfunc_end601-test_fabs_f128
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
	alloc 2
	nabs_f128 %r1, %r1
	retf 0

.Lfunc_end602:
	.size	test_fnabs_f128, .Lfunc_end602-test_fnabs_f128
	.size	.Ltest_fnabs_f128$local, .Lfunc_end602-test_fnabs_f128
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
	alloc 3
	abs_diff_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end603:
	.size	test_fabsd_f128, .Lfunc_end603-test_fabsd_f128
	.size	.Ltest_fabsd_f128$local, .Lfunc_end603-test_fabsd_f128
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
	alloc 3
	nabs_diff_f128 %r1, %r1, %r2
	retf 0

.Lfunc_end604:
	.size	test_fnabsd_f128, .Lfunc_end604-test_fnabsd_f128
	.size	.Ltest_fnabsd_f128$local, .Lfunc_end604-test_fnabsd_f128
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
	alloc 6
	ld_iprel_u16 %r1, %pcfwd(f16_zero)
	ld_imm %r2, 15360

	! BUNDLE
	ld_iprel_u32 %r3, %pcfwd(f32_zero)
	add_f16 %r1, %r1, %r2
	st_iprel_i16 %r1, %pcfwd(f16_zero)

	! BUNDLE
	ld_imm.l %r2, 1065353216
	ld_iprel_u64 %r4, %pcfwd(f64_zero)

	! BUNDLE
	add_f32 %r2, %r3, %r2
	ld_iprel_i128 %r3, %pcfwd(f128_zero)
	st_iprel_i32 %r2, %pcfwd(f32_zero)

	! BUNDLE
	ld_imm.l %r5, 4607182418800017408
	add_f64 %r4, %r4, %r5

	! BUNDLE
	ld_imm %r5, 0
	st_iprel_i64 %r4, %pcfwd(f64_zero)
	nop 2

	! BUNDLE
	ld_imm_high.l %r5, 4611404543450677248
	add_f128 %r3, %r3, %r5

	! BUNDLE
	extend_f16_f32 %r1, %r1
	add_f32 %r1, %r2, %r1
	extend_f32_f64 %r1, %r1

	! BUNDLE
	add_f64 %r1, %r4, %r1
	extend_f64_f128 %r1, %r1
	add_f128 %r1, %r3, %r1

	! BUNDLE
	cvt_f128_f64 %r1, %r1
	st_iprel_i128 %r3, %pcfwd(f128_zero)
	retf 0

.Lfunc_end605:
	.size	fp_store_zero_dummy_return, .Lfunc_end605-fp_store_zero_dummy_return
	.size	.Lfp_store_zero_dummy_return$local, .Lfunc_end605-fp_store_zero_dummy_return
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
	alloc 4
	st_i16 %gz, %r1, 8000
	st_iprel_i16 %gz, %pcfwd(f16_zero)

	! BUNDLE
	st_xi64_i16 %gz, %r1, %r2, 1, 20
	st_xi32_i16 %gz, %r1, %r3, 1, 20
	retf 0

.Lfunc_end606:
	.size	test_store_based_f16_zero, .Lfunc_end606-test_store_based_f16_zero
	.size	.Ltest_store_based_f16_zero$local, .Lfunc_end606-test_store_based_f16_zero
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
	alloc 4
	st_i32 %gz, %r1, 16000
	st_iprel_i32 %gz, %pcfwd(f32_zero)

	! BUNDLE
	st_xi64_i32 %gz, %r1, %r2, 2, 40
	st_xi32_i32 %gz, %r1, %r3, 2, 40
	retf 0

.Lfunc_end607:
	.size	test_store_based_f32_zero, .Lfunc_end607-test_store_based_f32_zero
	.size	.Ltest_store_based_f32_zero$local, .Lfunc_end607-test_store_based_f32_zero
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
	alloc 4
	st_i64 %gz, %r1, 32000
	st_iprel_i64 %gz, %pcfwd(f64_zero)

	! BUNDLE
	st_xi64_i64 %gz, %r1, %r2, 3, 40
	st_xi32_i64 %gz, %r1, %r3, 3, 40
	retf 0

.Lfunc_end608:
	.size	test_store_based_f64_zero, .Lfunc_end608-test_store_based_f64_zero
	.size	.Ltest_store_based_f64_zero$local, .Lfunc_end608-test_store_based_f64_zero
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
	alloc 4
	st_i128 %gz, %r1, 64000
	st_iprel_i128 %gz, %pcfwd(f128_zero)

	! BUNDLE
	st_xi64_i128 %gz, %r1, %r2, 4, 48
	st_xi32_i128 %gz, %r1, %r3, 4, 48
	retf 0

.Lfunc_end609:
	.size	test_store_based_f128_zero, .Lfunc_end609-test_store_based_f128_zero
	.size	.Ltest_store_based_f128_zero$local, .Lfunc_end609-test_store_based_f128_zero
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
	alloc 5
	mov %r4, %r2
	call_ri %r3, %r1, %gz

	! BUNDLE
	add_i32 %r1, %r4, %r2
	retf 0
	nop 0

.Lfunc_end610:
	.size	test_function_pointer, .Lfunc_end610-test_function_pointer
	.size	.Ltest_function_pointer$local, .Lfunc_end610-test_function_pointer
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
	alloc 5
	mov %r4, %r2
	call %r3, %jpc(ccc)

	! BUNDLE
	add_i32 %r1, %r4, %r2
	retf 0
	nop 0

.Lfunc_end611:
	.size	test_function_call, .Lfunc_end611-test_function_call
	.size	.Ltest_function_call$local, .Lfunc_end611-test_function_call
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
	alloc 5
	mov %r4, %r1
	call %r3, %jpc(xprintf)

	! BUNDLE
	add_i32 %r1, %r4, %r2
	retf 0
	nop 0

.Lfunc_end612:
	.size	test_function, .Lfunc_end612-test_function
	.size	.Ltest_function$local, .Lfunc_end612-test_function
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
	alloc 5
	sl_add_i64 %r2, %r2, %r1, 3
	call_mi %r3, %r2, 0

	! BUNDLE
	add_i32 %r1, %r4, %r1
	retf 0
	nop 0

.Lfunc_end613:
	.size	test_callmi, .Lfunc_end613-test_callmi
	.size	.Ltest_callmi$local, .Lfunc_end613-test_callmi
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
	alloc 5
	mov %r4, %r2
	call_mi %r3, %r1, 48

	! BUNDLE
	add_imm_i32 %r1, %r4, 2
	retf 0
	nop 0

.Lfunc_end614:
	.size	test_callmi_folding, .Lfunc_end614-test_callmi_folding
	.size	.Ltest_callmi_folding$local, .Lfunc_end614-test_callmi_folding
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
	alloc 4
	mov %r3, %r1
	call_plt %r2, %pcfwd(gftable+40)

	! BUNDLE
	add_imm_i32 %r1, %r3, 2
	retf 0
	nop 0

.Lfunc_end615:
	.size	test_callplt_folding, .Lfunc_end615-test_callplt_folding
	.size	.Ltest_callplt_folding$local, .Lfunc_end615-test_callplt_folding
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
	alloc 2
	add_imm_i32 %r1, %r1, 1
	retf 0

.Lfunc_end616:
	.size	test_global_function_pointer_target, .Lfunc_end616-test_global_function_pointer_target
	.size	.Ltest_global_function_pointer_target$local, .Lfunc_end616-test_global_function_pointer_target
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
	alloc 2
	lda_r %r1, %jpc(test_static_function_pointer_target)
	retf 0

.Lfunc_end617:
	.size	test_static_function_pointer_materialize, .Lfunc_end617-test_static_function_pointer_materialize
	.size	.Ltest_static_function_pointer_materialize$local, .Lfunc_end617-test_static_function_pointer_materialize
                                        ! -- End function
	.p2align	4                               ! -- Begin function test_static_function_pointer_target
	.type	test_static_function_pointer_target,@function
test_static_function_pointer_target:    ! @test_static_function_pointer_target
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	add_imm_i32 %r1, %r1, 1
	retf 0

.Lfunc_end618:
	.size	test_static_function_pointer_target, .Lfunc_end618-test_static_function_pointer_target
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
	alloc 2
	lda_r %r1, %jpc(test_global_function_pointer_target)
	retf 0

.Lfunc_end619:
	.size	test_global_function_pointer_materialize, .Lfunc_end619-test_global_function_pointer_materialize
	.size	.Ltest_global_function_pointer_materialize$local, .Lfunc_end619-test_global_function_pointer_materialize
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
	alloc 2
	lda_r %r1, %jpc(test_extern_function_pointer_target)
	retf 0

.Lfunc_end620:
	.size	test_extern_function_pointer_materialize, .Lfunc_end620-test_extern_function_pointer_materialize
	.size	.Ltest_extern_function_pointer_materialize$local, .Lfunc_end620-test_extern_function_pointer_materialize
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
	alloc 3
	srp_imm_i32 %r1, %r2, %r1, 20
	retf 0

.Lfunc_end621:
	.size	test_fshr_u32, .Lfunc_end621-test_fshr_u32
	.size	.Ltest_fshr_u32$local, .Lfunc_end621-test_fshr_u32
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
	alloc 3
	srp_imm_i64 %r1, %r2, %r1, 20
	retf 0

.Lfunc_end622:
	.size	test_fshr_u64, .Lfunc_end622-test_fshr_u64
	.size	.Ltest_fshr_u64$local, .Lfunc_end622-test_fshr_u64
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
	alloc 3
	srp_imm_i128 %r1, %r2, %r1, 20
	retf 0

.Lfunc_end623:
	.size	test_fshr_u128, .Lfunc_end623-test_fshr_u128
	.size	.Ltest_fshr_u128$local, .Lfunc_end623-test_fshr_u128
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
	alloc 2
	srp_imm_i32 %r1, %r1, %r1, 20
	retf 0

.Lfunc_end624:
	.size	test_rotate_u32, .Lfunc_end624-test_rotate_u32
	.size	.Ltest_rotate_u32$local, .Lfunc_end624-test_rotate_u32
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
	alloc 2
	srp_imm_i64 %r1, %r1, %r1, 20
	retf 0

.Lfunc_end625:
	.size	test_rotate_u64, .Lfunc_end625-test_rotate_u64
	.size	.Ltest_rotate_u64$local, .Lfunc_end625-test_rotate_u64
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
	alloc 2
	srp_imm_i128 %r1, %r1, %r1, 20
	retf 0

.Lfunc_end626:
	.size	test_rotate_u128, .Lfunc_end626-test_rotate_u128
	.size	.Ltest_rotate_u128$local, .Lfunc_end626-test_rotate_u128
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
	alloc 3
	ld_iprel_u64 %r2, %pcfwd(x_bss)
	add_imm_i64 %r2, %r2, 1

	! BUNDLE
	st_iprel_i64 %r2, %pcfwd(x_bss)
	ld_iprel_u64 %r2, %pcfwd(x_bss_extern)
	add_imm_i64 %r2, %r2, 1

	! BUNDLE
	st_iprel_i64 %r2, %pcfwd(x_bss_extern)
	retf 0
	nop 0

.Lfunc_end627:
	.size	test_global_data, .Lfunc_end627-test_global_data
	.size	.Ltest_global_data$local, .Lfunc_end627-test_global_data
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
	alloc 2
	lda_iprel %r1, %pcfwd(global_i8)
	retf 0

.Lfunc_end628:
	.size	test_global_i8_ptr, .Lfunc_end628-test_global_i8_ptr
	.size	.Ltest_global_i8_ptr$local, .Lfunc_end628-test_global_i8_ptr
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
	alloc 2
	ld_iprel_i8 %r1, %pcfwd(global_i8)
	retf 0

.Lfunc_end629:
	.size	test_global_i8, .Lfunc_end629-test_global_i8
	.size	.Ltest_global_i8$local, .Lfunc_end629-test_global_i8
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
	alloc 2
	ld_iprel_u8 %r1, %pcfwd(global_u8)
	retf 0

.Lfunc_end630:
	.size	test_global_u8, .Lfunc_end630-test_global_u8
	.size	.Ltest_global_u8$local, .Lfunc_end630-test_global_u8
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
	alloc 2
	ld_iprel_i16 %r1, %pcfwd(global_i16)
	retf 0

.Lfunc_end631:
	.size	test_global_i16, .Lfunc_end631-test_global_i16
	.size	.Ltest_global_i16$local, .Lfunc_end631-test_global_i16
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
	alloc 2
	ld_iprel_u16 %r1, %pcfwd(global_u16)
	retf 0

.Lfunc_end632:
	.size	test_global_u16, .Lfunc_end632-test_global_u16
	.size	.Ltest_global_u16$local, .Lfunc_end632-test_global_u16
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
	alloc 2
	ld_iprel_i8 %r1, %pcfwd(global_i8)
	retf 0

.Lfunc_end633:
	.size	test_global_i8_to_i16, .Lfunc_end633-test_global_i8_to_i16
	.size	.Ltest_global_i8_to_i16$local, .Lfunc_end633-test_global_i8_to_i16
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
	alloc 2
	ld_iprel_u8 %r1, %pcfwd(global_u8)
	retf 0

.Lfunc_end634:
	.size	test_global_u8_to_u16, .Lfunc_end634-test_global_u8_to_u16
	.size	.Ltest_global_u8_to_u16$local, .Lfunc_end634-test_global_u8_to_u16
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
	alloc 2
	ld_iprel_u32 %r1, %pcfwd(global_i32)
	retf 0

.Lfunc_end635:
	.size	test_global_i32, .Lfunc_end635-test_global_i32
	.size	.Ltest_global_i32$local, .Lfunc_end635-test_global_i32
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
	alloc 2
	ld_iprel_u32 %r1, %pcfwd(global_u32)
	retf 0

.Lfunc_end636:
	.size	test_global_u32, .Lfunc_end636-test_global_u32
	.size	.Ltest_global_u32$local, .Lfunc_end636-test_global_u32
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
	alloc 2
	ld_iprel_i8 %r1, %pcfwd(global_i8)
	retf 0

.Lfunc_end637:
	.size	test_global_i8_to_i32, .Lfunc_end637-test_global_i8_to_i32
	.size	.Ltest_global_i8_to_i32$local, .Lfunc_end637-test_global_i8_to_i32
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
	alloc 2
	ld_iprel_u8 %r1, %pcfwd(global_u8)
	retf 0

.Lfunc_end638:
	.size	test_global_u8_to_u32, .Lfunc_end638-test_global_u8_to_u32
	.size	.Ltest_global_u8_to_u32$local, .Lfunc_end638-test_global_u8_to_u32
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
	alloc 2
	ld_iprel_i16 %r1, %pcfwd(global_i16)
	retf 0

.Lfunc_end639:
	.size	test_global_i16_to_i32, .Lfunc_end639-test_global_i16_to_i32
	.size	.Ltest_global_i16_to_i32$local, .Lfunc_end639-test_global_i16_to_i32
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
	alloc 2
	ld_iprel_u16 %r1, %pcfwd(global_u16)
	retf 0

.Lfunc_end640:
	.size	test_global_u16_to_u32, .Lfunc_end640-test_global_u16_to_u32
	.size	.Ltest_global_u16_to_u32$local, .Lfunc_end640-test_global_u16_to_u32
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
	alloc 2
	ld_iprel_i8 %r1, %pcfwd(global_i8)
	retf 0

.Lfunc_end641:
	.size	test_global_i8_to_i64, .Lfunc_end641-test_global_i8_to_i64
	.size	.Ltest_global_i8_to_i64$local, .Lfunc_end641-test_global_i8_to_i64
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
	alloc 2
	ld_iprel_u8 %r1, %pcfwd(global_u8)
	retf 0

.Lfunc_end642:
	.size	test_global_u8_to_u64, .Lfunc_end642-test_global_u8_to_u64
	.size	.Ltest_global_u8_to_u64$local, .Lfunc_end642-test_global_u8_to_u64
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
	alloc 2
	ld_iprel_i16 %r1, %pcfwd(global_i16)
	retf 0

.Lfunc_end643:
	.size	test_global_i16_to_i64, .Lfunc_end643-test_global_i16_to_i64
	.size	.Ltest_global_i16_to_i64$local, .Lfunc_end643-test_global_i16_to_i64
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
	alloc 2
	ld_iprel_u16 %r1, %pcfwd(global_u16)
	retf 0

.Lfunc_end644:
	.size	test_global_u16_to_u64, .Lfunc_end644-test_global_u16_to_u64
	.size	.Ltest_global_u16_to_u64$local, .Lfunc_end644-test_global_u16_to_u64
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
	alloc 2
	ld_iprel_i32 %r1, %pcfwd(global_i32)
	retf 0

.Lfunc_end645:
	.size	test_global_i32_to_i64, .Lfunc_end645-test_global_i32_to_i64
	.size	.Ltest_global_i32_to_i64$local, .Lfunc_end645-test_global_i32_to_i64
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
	alloc 2
	ld_iprel_u32 %r1, %pcfwd(global_u32)
	retf 0

.Lfunc_end646:
	.size	test_global_u32_to_u64, .Lfunc_end646-test_global_u32_to_u64
	.size	.Ltest_global_u32_to_u64$local, .Lfunc_end646-test_global_u32_to_u64
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
	alloc 2
	ld_iprel_u64 %r1, %pcfwd(global_i64)
	retf 0

.Lfunc_end647:
	.size	test_global_i64, .Lfunc_end647-test_global_i64
	.size	.Ltest_global_i64$local, .Lfunc_end647-test_global_i64
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
	alloc 2
	ld_iprel_u64 %r1, %pcfwd(global_u64)
	retf 0

.Lfunc_end648:
	.size	test_global_u64, .Lfunc_end648-test_global_u64
	.size	.Ltest_global_u64$local, .Lfunc_end648-test_global_u64
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
	alloc 2
	ld_iprel_i8 %r1, %pcfwd(global_array_i8+5)
	retf 0

.Lfunc_end649:
	.size	test_global_array_folding_i8, .Lfunc_end649-test_global_array_folding_i8
	.size	.Ltest_global_array_folding_i8$local, .Lfunc_end649-test_global_array_folding_i8
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
	alloc 2
	ld_iprel_i16 %r1, %pcfwd(global_array_i16+10)
	retf 0

.Lfunc_end650:
	.size	test_global_array_folding_i16, .Lfunc_end650-test_global_array_folding_i16
	.size	.Ltest_global_array_folding_i16$local, .Lfunc_end650-test_global_array_folding_i16
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
	alloc 2
	ld_iprel_u32 %r1, %pcfwd(global_array_i32+20)
	retf 0

.Lfunc_end651:
	.size	test_global_array_folding_i32, .Lfunc_end651-test_global_array_folding_i32
	.size	.Ltest_global_array_folding_i32$local, .Lfunc_end651-test_global_array_folding_i32
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
	alloc 2
	ld_iprel_u64 %r1, %pcfwd(global_array_i64+40)
	retf 0

.Lfunc_end652:
	.size	test_global_array_folding_i64, .Lfunc_end652-test_global_array_folding_i64
	.size	.Ltest_global_array_folding_i64$local, .Lfunc_end652-test_global_array_folding_i64
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
	alloc 2
	ld_imm %r1, 12345778
	retf 0

.Lfunc_end653:
	.size	test_ldi128, .Lfunc_end653-test_ldi128
	.size	.Ltest_ldi128$local, .Lfunc_end653-test_ldi128
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
	alloc 2
	ld_imm.l %r1, 987654321123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end654:
	.size	test_ldi128_long, .Lfunc_end654-test_ldi128_long
	.size	.Ltest_ldi128_long$local, .Lfunc_end654-test_ldi128_long
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
	alloc 2
	ld_imm.l %r1, -230686721

	! BUNDLE
	ld_imm_high.l %r1, 1048575
	retf 0

.Lfunc_end655:
	.size	test_ldi128_verylong, .Lfunc_end655-test_ldi128_verylong
	.size	.Ltest_ldi128_verylong$local, .Lfunc_end655-test_ldi128_verylong
                                        ! -- End function
	.globl	test_subr_i128                  ! -- Begin function test_subr_i128
	.p2align	4
	.type	test_subr_i128,@function
test_subr_i128:                         ! @test_subr_i128
.Ltest_subr_i128$local:
	.type	.Ltest_subr_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	ld_imm %r2, 1234
	sub_i128 %r1, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end656:
	.size	test_subr_i128, .Lfunc_end656-test_subr_i128
	.size	.Ltest_subr_i128$local, .Lfunc_end656-test_subr_i128
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
	alloc 3
	ld_imm.l %r2, 123456789123456789

	! BUNDLE
	add_i128 %r1, %r1, %r2
	ld_imm.l %r2, 9200848539817279407

	! BUNDLE
	or %r1, %r1, %r2
	retf 0
	nop 0

.Lfunc_end657:
	.size	test_addi_i128, .Lfunc_end657-test_addi_i128
	.size	.Ltest_addi_i128$local, .Lfunc_end657-test_addi_i128
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
	alloc 2
	and_imm.l %r1, %r1, 123456789123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end658:
	.size	test_andi_i128, .Lfunc_end658-test_andi_i128
	.size	.Ltest_andi_i128$local, .Lfunc_end658-test_andi_i128
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
	alloc 2
	or_imm.l %r1, %r1, 987654321123456789

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end659:
	.size	test_ori_i128, .Lfunc_end659-test_ori_i128
	.size	.Ltest_ori_i128$local, .Lfunc_end659-test_ori_i128
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
	alloc 3
	add_i128 %r1, %r2, %r1
	retf 0

.Lfunc_end660:
	.size	test_add_i128, .Lfunc_end660-test_add_i128
	.size	.Ltest_add_i128$local, .Lfunc_end660-test_add_i128
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
	alloc 3
	sub_i128 %r1, %r1, %r2
	retf 0

.Lfunc_end661:
	.size	test_sub_i128, .Lfunc_end661-test_sub_i128
	.size	.Ltest_sub_i128$local, .Lfunc_end661-test_sub_i128
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
	alloc 2
	ld_imm %r1, -123456789
	retf 0

.Lfunc_end662:
	.size	test_immediate_i32, .Lfunc_end662-test_immediate_i32
	.size	.Ltest_immediate_i32$local, .Lfunc_end662-test_immediate_i32
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
	alloc 2
	ld_imm %r1, 123456789
	retf 0

.Lfunc_end663:
	.size	test_immediate_u32, .Lfunc_end663-test_immediate_u32
	.size	.Ltest_immediate_u32$local, .Lfunc_end663-test_immediate_u32
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
	alloc 2
	ld_imm.l %r1, -1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end664:
	.size	test_immediate_i32_long, .Lfunc_end664-test_immediate_i32_long
	.size	.Ltest_immediate_i32_long$local, .Lfunc_end664-test_immediate_i32_long
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
	alloc 2
	ld_imm.l %r1, 1234567890

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end665:
	.size	test_immediate_u32_long, .Lfunc_end665-test_immediate_u32_long
	.size	.Ltest_immediate_u32_long$local, .Lfunc_end665-test_immediate_u32_long
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
	alloc 2
	ld_imm %r1, -12345678
	retf 0

.Lfunc_end666:
	.size	test_immediate_i64, .Lfunc_end666-test_immediate_i64
	.size	.Ltest_immediate_i64$local, .Lfunc_end666-test_immediate_i64
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
	alloc 2
	ld_imm %r1, 12345678
	retf 0

.Lfunc_end667:
	.size	test_immediate_u64, .Lfunc_end667-test_immediate_u64
	.size	.Ltest_immediate_u64$local, .Lfunc_end667-test_immediate_u64
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
	alloc 2
	ld_imm.l %r1, -1234567890123

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end668:
	.size	test_immediate_i64_long, .Lfunc_end668-test_immediate_i64_long
	.size	.Ltest_immediate_i64_long$local, .Lfunc_end668-test_immediate_i64_long
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
	alloc 2
	ld_imm.l %r1, 1234567890123

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end669:
	.size	test_immediate_u64_long, .Lfunc_end669-test_immediate_u64_long
	.size	.Ltest_immediate_u64_long$local, .Lfunc_end669-test_immediate_u64_long
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
	alloc 2
	ld_imm.l %r1, 9223372036854775807

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end670:
	.size	test_immediate_size_max, .Lfunc_end670-test_immediate_size_max
	.size	.Ltest_immediate_size_max$local, .Lfunc_end670-test_immediate_size_max
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
	alloc 3
	div_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end671:
	.size	test_div_i32, .Lfunc_end671-test_div_i32
	.size	.Ltest_div_i32$local, .Lfunc_end671-test_div_i32
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
	alloc 3
	div_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end672:
	.size	test_div_i64, .Lfunc_end672-test_div_i64
	.size	.Ltest_div_i64$local, .Lfunc_end672-test_div_i64
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
	alloc 3
	div_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end673:
	.size	test_div_u32, .Lfunc_end673-test_div_u32
	.size	.Ltest_div_u32$local, .Lfunc_end673-test_div_u32
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
	alloc 3
	div_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end674:
	.size	test_div_u64, .Lfunc_end674-test_div_u64
	.size	.Ltest_div_u64$local, .Lfunc_end674-test_div_u64
                                        ! -- End function
	.globl	test_divi_i32                   ! -- Begin function test_divi_i32
	.p2align	4
	.type	test_divi_i32,@function
test_divi_i32:                          ! @test_divi_i32
.Ltest_divi_i32$local:
	.type	.Ltest_divi_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end675:
	.size	test_divi_i32, .Lfunc_end675-test_divi_i32
	.size	.Ltest_divi_i32$local, .Lfunc_end675-test_divi_i32
                                        ! -- End function
	.globl	test_divi_i64                   ! -- Begin function test_divi_i64
	.p2align	4
	.type	test_divi_i64,@function
test_divi_i64:                          ! @test_divi_i64
.Ltest_divi_i64$local:
	.type	.Ltest_divi_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end676:
	.size	test_divi_i64, .Lfunc_end676-test_divi_i64
	.size	.Ltest_divi_i64$local, .Lfunc_end676-test_divi_i64
                                        ! -- End function
	.globl	test_divi_u32                   ! -- Begin function test_divi_u32
	.p2align	4
	.type	test_divi_u32,@function
test_divi_u32:                          ! @test_divi_u32
.Ltest_divi_u32$local:
	.type	.Ltest_divi_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end677:
	.size	test_divi_u32, .Lfunc_end677-test_divi_u32
	.size	.Ltest_divi_u32$local, .Lfunc_end677-test_divi_u32
                                        ! -- End function
	.globl	test_divi_u64                   ! -- Begin function test_divi_u64
	.p2align	4
	.type	test_divi_u64,@function
test_divi_u64:                          ! @test_divi_u64
.Ltest_divi_u64$local:
	.type	.Ltest_divi_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end678:
	.size	test_divi_u64, .Lfunc_end678-test_divi_u64
	.size	.Ltest_divi_u64$local, .Lfunc_end678-test_divi_u64
                                        ! -- End function
	.globl	test_divi_ext_i32               ! -- Begin function test_divi_ext_i32
	.p2align	4
	.type	test_divi_ext_i32,@function
test_divi_ext_i32:                      ! @test_divi_ext_i32
.Ltest_divi_ext_i32$local:
	.type	.Ltest_divi_ext_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end679:
	.size	test_divi_ext_i32, .Lfunc_end679-test_divi_ext_i32
	.size	.Ltest_divi_ext_i32$local, .Lfunc_end679-test_divi_ext_i32
                                        ! -- End function
	.globl	test_divi_ext_i64               ! -- Begin function test_divi_ext_i64
	.p2align	4
	.type	test_divi_ext_i64,@function
test_divi_ext_i64:                      ! @test_divi_ext_i64
.Ltest_divi_ext_i64$local:
	.type	.Ltest_divi_ext_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end680:
	.size	test_divi_ext_i64, .Lfunc_end680-test_divi_ext_i64
	.size	.Ltest_divi_ext_i64$local, .Lfunc_end680-test_divi_ext_i64
                                        ! -- End function
	.globl	test_divi_ext_u32               ! -- Begin function test_divi_ext_u32
	.p2align	4
	.type	test_divi_ext_u32,@function
test_divi_ext_u32:                      ! @test_divi_ext_u32
.Ltest_divi_ext_u32$local:
	.type	.Ltest_divi_ext_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end681:
	.size	test_divi_ext_u32, .Lfunc_end681-test_divi_ext_u32
	.size	.Ltest_divi_ext_u32$local, .Lfunc_end681-test_divi_ext_u32
                                        ! -- End function
	.globl	test_divi_ext_u64               ! -- Begin function test_divi_ext_u64
	.p2align	4
	.type	test_divi_ext_u64,@function
test_divi_ext_u64:                      ! @test_divi_ext_u64
.Ltest_divi_ext_u64$local:
	.type	.Ltest_divi_ext_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	div_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end682:
	.size	test_divi_ext_u64, .Lfunc_end682-test_divi_ext_u64
	.size	.Ltest_divi_ext_u64$local, .Lfunc_end682-test_divi_ext_u64
                                        ! -- End function
	.globl	test_neg_i32                    ! -- Begin function test_neg_i32
	.p2align	4
	.type	test_neg_i32,@function
test_neg_i32:                           ! @test_neg_i32
.Ltest_neg_i32$local:
	.type	.Ltest_neg_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	neg_i32 %r1, %r1
	retf 0

.Lfunc_end683:
	.size	test_neg_i32, .Lfunc_end683-test_neg_i32
	.size	.Ltest_neg_i32$local, .Lfunc_end683-test_neg_i32
                                        ! -- End function
	.globl	test_neg_i64                    ! -- Begin function test_neg_i64
	.p2align	4
	.type	test_neg_i64,@function
test_neg_i64:                           ! @test_neg_i64
.Ltest_neg_i64$local:
	.type	.Ltest_neg_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	neg_i64 %r1, %r1
	retf 0

.Lfunc_end684:
	.size	test_neg_i64, .Lfunc_end684-test_neg_i64
	.size	.Ltest_neg_i64$local, .Lfunc_end684-test_neg_i64
                                        ! -- End function
	.globl	test_neg_i128                   ! -- Begin function test_neg_i128
	.p2align	4
	.type	test_neg_i128,@function
test_neg_i128:                          ! @test_neg_i128
.Ltest_neg_i128$local:
	.type	.Ltest_neg_i128$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	neg_i128 %r1, %r1
	retf 0

.Lfunc_end685:
	.size	test_neg_i128, .Lfunc_end685-test_neg_i128
	.size	.Ltest_neg_i128$local, .Lfunc_end685-test_neg_i128
                                        ! -- End function
	.globl	test_rem_i32                    ! -- Begin function test_rem_i32
	.p2align	4
	.type	test_rem_i32,@function
test_rem_i32:                           ! @test_rem_i32
.Ltest_rem_i32$local:
	.type	.Ltest_rem_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	rem_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end686:
	.size	test_rem_i32, .Lfunc_end686-test_rem_i32
	.size	.Ltest_rem_i32$local, .Lfunc_end686-test_rem_i32
                                        ! -- End function
	.globl	test_rem_i64                    ! -- Begin function test_rem_i64
	.p2align	4
	.type	test_rem_i64,@function
test_rem_i64:                           ! @test_rem_i64
.Ltest_rem_i64$local:
	.type	.Ltest_rem_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	rem_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end687:
	.size	test_rem_i64, .Lfunc_end687-test_rem_i64
	.size	.Ltest_rem_i64$local, .Lfunc_end687-test_rem_i64
                                        ! -- End function
	.globl	test_rem_u32                    ! -- Begin function test_rem_u32
	.p2align	4
	.type	test_rem_u32,@function
test_rem_u32:                           ! @test_rem_u32
.Ltest_rem_u32$local:
	.type	.Ltest_rem_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	rem_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end688:
	.size	test_rem_u32, .Lfunc_end688-test_rem_u32
	.size	.Ltest_rem_u32$local, .Lfunc_end688-test_rem_u32
                                        ! -- End function
	.globl	test_rem_u64                    ! -- Begin function test_rem_u64
	.p2align	4
	.type	test_rem_u64,@function
test_rem_u64:                           ! @test_rem_u64
.Ltest_rem_u64$local:
	.type	.Ltest_rem_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	rem_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end689:
	.size	test_rem_u64, .Lfunc_end689-test_rem_u64
	.size	.Ltest_rem_u64$local, .Lfunc_end689-test_rem_u64
                                        ! -- End function
	.globl	test_rem_imm_i32                ! -- Begin function test_rem_imm_i32
	.p2align	4
	.type	test_rem_imm_i32,@function
test_rem_imm_i32:                       ! @test_rem_imm_i32
.Ltest_rem_imm_i32$local:
	.type	.Ltest_rem_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end690:
	.size	test_rem_imm_i32, .Lfunc_end690-test_rem_imm_i32
	.size	.Ltest_rem_imm_i32$local, .Lfunc_end690-test_rem_imm_i32
                                        ! -- End function
	.globl	test_rem_imm_u32                ! -- Begin function test_rem_imm_u32
	.p2align	4
	.type	test_rem_imm_u32,@function
test_rem_imm_u32:                       ! @test_rem_imm_u32
.Ltest_rem_imm_u32$local:
	.type	.Ltest_rem_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end691:
	.size	test_rem_imm_u32, .Lfunc_end691-test_rem_imm_u32
	.size	.Ltest_rem_imm_u32$local, .Lfunc_end691-test_rem_imm_u32
                                        ! -- End function
	.globl	test_rem_imm_i64                ! -- Begin function test_rem_imm_i64
	.p2align	4
	.type	test_rem_imm_i64,@function
test_rem_imm_i64:                       ! @test_rem_imm_i64
.Ltest_rem_imm_i64$local:
	.type	.Ltest_rem_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end692:
	.size	test_rem_imm_i64, .Lfunc_end692-test_rem_imm_i64
	.size	.Ltest_rem_imm_i64$local, .Lfunc_end692-test_rem_imm_i64
                                        ! -- End function
	.globl	test_rem_imm_u64                ! -- Begin function test_rem_imm_u64
	.p2align	4
	.type	test_rem_imm_u64,@function
test_rem_imm_u64:                       ! @test_rem_imm_u64
.Ltest_rem_imm_u64$local:
	.type	.Ltest_rem_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end693:
	.size	test_rem_imm_u64, .Lfunc_end693-test_rem_imm_u64
	.size	.Ltest_rem_imm_u64$local, .Lfunc_end693-test_rem_imm_u64
                                        ! -- End function
	.globl	test_rem_imm_ext_i32            ! -- Begin function test_rem_imm_ext_i32
	.p2align	4
	.type	test_rem_imm_ext_i32,@function
test_rem_imm_ext_i32:                   ! @test_rem_imm_ext_i32
.Ltest_rem_imm_ext_i32$local:
	.type	.Ltest_rem_imm_ext_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end694:
	.size	test_rem_imm_ext_i32, .Lfunc_end694-test_rem_imm_ext_i32
	.size	.Ltest_rem_imm_ext_i32$local, .Lfunc_end694-test_rem_imm_ext_i32
                                        ! -- End function
	.globl	test_rem_imm_ext_u32            ! -- Begin function test_rem_imm_ext_u32
	.p2align	4
	.type	test_rem_imm_ext_u32,@function
test_rem_imm_ext_u32:                   ! @test_rem_imm_ext_u32
.Ltest_rem_imm_ext_u32$local:
	.type	.Ltest_rem_imm_ext_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end695:
	.size	test_rem_imm_ext_u32, .Lfunc_end695-test_rem_imm_ext_u32
	.size	.Ltest_rem_imm_ext_u32$local, .Lfunc_end695-test_rem_imm_ext_u32
                                        ! -- End function
	.globl	test_rem_imm_ext_i64            ! -- Begin function test_rem_imm_ext_i64
	.p2align	4
	.type	test_rem_imm_ext_i64,@function
test_rem_imm_ext_i64:                   ! @test_rem_imm_ext_i64
.Ltest_rem_imm_ext_i64$local:
	.type	.Ltest_rem_imm_ext_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end696:
	.size	test_rem_imm_ext_i64, .Lfunc_end696-test_rem_imm_ext_i64
	.size	.Ltest_rem_imm_ext_i64$local, .Lfunc_end696-test_rem_imm_ext_i64
                                        ! -- End function
	.globl	test_rem_imm_ext_u64            ! -- Begin function test_rem_imm_ext_u64
	.p2align	4
	.type	test_rem_imm_ext_u64,@function
test_rem_imm_ext_u64:                   ! @test_rem_imm_ext_u64
.Ltest_rem_imm_ext_u64$local:
	.type	.Ltest_rem_imm_ext_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	rem_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end697:
	.size	test_rem_imm_ext_u64, .Lfunc_end697-test_rem_imm_ext_u64
	.size	.Ltest_rem_imm_ext_u64$local, .Lfunc_end697-test_rem_imm_ext_u64
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
	alloc 2
	ld_u32 %r1, %r1, 16000
	retf 0

.Lfunc_end698:
	.size	test_load_based_u32, .Lfunc_end698-test_load_based_u32
	.size	.Ltest_load_based_u32$local, .Lfunc_end698-test_load_based_u32
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
	alloc 2
	ld_u16 %r1, %r1, 8000
	retf 0

.Lfunc_end699:
	.size	test_load_based_u16, .Lfunc_end699-test_load_based_u16
	.size	.Ltest_load_based_u16$local, .Lfunc_end699-test_load_based_u16
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
	alloc 2
	ld_u8 %r1, %r1, 4000
	retf 0

.Lfunc_end700:
	.size	test_load_based_u8, .Lfunc_end700-test_load_based_u8
	.size	.Ltest_load_based_u8$local, .Lfunc_end700-test_load_based_u8
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
	alloc 2
	ld_u32 %r1, %r1, 16000
	retf 0

.Lfunc_end701:
	.size	test_load_based_i32, .Lfunc_end701-test_load_based_i32
	.size	.Ltest_load_based_i32$local, .Lfunc_end701-test_load_based_i32
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
	alloc 2
	ld_i16 %r1, %r1, 8000
	retf 0

.Lfunc_end702:
	.size	test_load_based_i16, .Lfunc_end702-test_load_based_i16
	.size	.Ltest_load_based_i16$local, .Lfunc_end702-test_load_based_i16
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
	alloc 2
	ld_i8 %r1, %r1, 4000
	retf 0

.Lfunc_end703:
	.size	test_load_based_i8, .Lfunc_end703-test_load_based_i8
	.size	.Ltest_load_based_i8$local, .Lfunc_end703-test_load_based_i8
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
	alloc 4
	div_u32 %r2, %r2, %r3
	ld_xu32_u64 %r1, %r1, %r2, 3, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end704:
	.size	test_base_index_u32_u32, .Lfunc_end704-test_base_index_u32_u32
	.size	.Ltest_base_index_u32_u32$local, .Lfunc_end704-test_base_index_u32_u32
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
	alloc 8
	ld_u64.l %r5, %r1, 800000000000

	! BUNDLE
	ld_u64 %r6, %r1, 8000
	ld_xi64_u64.l %r7, %r1, %r2, 3, 800000000

	! BUNDLE
	ld_xi64_u64.l %r1, %r1, %r4, 3, 80
	nop 2

	! BUNDLE
	ld_xi64_u64.l %r2, %r3, %r2, 4, 160
	nop 2

	! BUNDLE
	ld_xi64_u64.l %r3, %r3, %r4, 4, 1608
	add_i64 %r4, %r6, %r5

	! BUNDLE
	add_add_i64 %r1, %r4, %r7, %r1
	add_add_i64 %r1, %r1, %r2, %r3
	retf 0

.Lfunc_end705:
	.size	test_base_address, .Lfunc_end705-test_base_address
	.size	.Ltest_base_address$local, .Lfunc_end705-test_base_address
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
	alloc 5
	br_lt_imm_i64 %r3, 1, .LBB706_2
	nop 0

.LBB706_1:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ld_u64 %r4, %r1, 0
	ld_xi64_u64 %r4, %r2, %r4, 4, 40
	st_i64 %r4, %r1, 0

	! BUNDLE
	add_imm_i64 %r3, %r3, -1
	add_imm_i64 %r1, %r1, 8
	br_ne_imm_i64 %r3, 0, .LBB706_1

.LBB706_2:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end706:
	.size	MemoryIndexed, .Lfunc_end706-MemoryIndexed
	.size	.LMemoryIndexed$local, .Lfunc_end706-MemoryIndexed
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
	alloc 3
	ld_xi32_u64 %r1, %r1, %r2, 3, 0
	retf 0

.Lfunc_end707:
	.size	test_base_index_i32, .Lfunc_end707-test_base_index_i32
	.size	.Ltest_base_index_i32$local, .Lfunc_end707-test_base_index_i32
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
	alloc 3
	ld_xi32_u64 %r1, %r1, %r2, 3, 40
	retf 0

.Lfunc_end708:
	.size	test_base_index_i32_offset, .Lfunc_end708-test_base_index_i32_offset
	.size	.Ltest_base_index_i32_offset$local, .Lfunc_end708-test_base_index_i32_offset
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
	alloc 3
	ld_xu32_u64 %r1, %r1, %r2, 3, 0
	retf 0

.Lfunc_end709:
	.size	test_base_index_u32, .Lfunc_end709-test_base_index_u32
	.size	.Ltest_base_index_u32$local, .Lfunc_end709-test_base_index_u32
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
	alloc 3
	add_imm_i32 %r2, %r2, 20
	ld_xu32_u64 %r1, %r1, %r2, 3, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end710:
	.size	test_base_index_u32_offset, .Lfunc_end710-test_base_index_u32_offset
	.size	.Ltest_base_index_u32_offset$local, .Lfunc_end710-test_base_index_u32_offset
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
	alloc 3
	ld_xi64_u64 %r1, %r1, %r2, 3, 40
	retf 0

.Lfunc_end711:
	.size	test_base_index_i64, .Lfunc_end711-test_base_index_i64
	.size	.Ltest_base_index_i64$local, .Lfunc_end711-test_base_index_i64
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
	alloc 3
	ld_xi32_u64.l %r1, %r1, %r2, 4, 328

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end712:
	.size	test_base_index_i32_struct, .Lfunc_end712-test_base_index_i32_struct
	.size	.Ltest_base_index_i32_struct$local, .Lfunc_end712-test_base_index_i32_struct
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
	alloc 3
	add_imm_i32 %r2, %r2, 20
	ld_xu32_u64 %r1, %r1, %r2, 4, 8

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end713:
	.size	test_base_index_u32_struct, .Lfunc_end713-test_base_index_u32_struct
	.size	.Ltest_base_index_u32_struct$local, .Lfunc_end713-test_base_index_u32_struct
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
	alloc 7
	mov %r4, %gz
	br_lt_imm_i64 %r1, 1, .LBB714_3

	! BUNDLE
	add_imm_i64 %r5, %r3, 80
	nop 0
	nop 0

.LBB714_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ld_u64 %r6, %r5, 0
	add_i64 %r4, %r6, %r4
	add_imm_i64 %r1, %r1, -1

	! BUNDLE
	add_imm_i64 %r5, %r5, 8
	br_ne_imm_i64 %r1, 0, .LBB714_2
	nop 0

.LBB714_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	ld_xi64_u64.l %r1, %r3, %r2, 3, 168
	add_i64 %r1, %r1, %r4

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end714:
	.size	for_loop, .Lfunc_end714-for_loop
	.size	.Lfor_loop$local, .Lfunc_end714-for_loop
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
	alloc 7
	mov %r4, %gz
	br_lt_imm_i32 %r1, 1, .LBB715_3

	! BUNDLE
	srl_imm_u32 %r1, %r1, 0
	add_imm_i64 %r5, %r3, 40
	nop 0

.LBB715_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ld_u32 %r6, %r5, 0
	add_i32 %r4, %r6, %r4
	add_imm_i64 %r1, %r1, -1

	! BUNDLE
	add_imm_i64 %r5, %r5, 4
	br_ne_imm_i64 %r1, 0, .LBB715_2
	nop 0

.LBB715_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	ld_xi32_u32.l %r1, %r3, %r2, 2, 84
	add_i32 %r1, %r1, %r4

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end715:
	.size	for_loop_32, .Lfunc_end715-for_loop_32
	.size	.Lfor_loop_32$local, .Lfunc_end715-for_loop_32
                                        ! -- End function
	.globl	test_rep_lt_i64                 ! -- Begin function test_rep_lt_i64
	.p2align	4
	.type	test_rep_lt_i64,@function
test_rep_lt_i64:                        ! @test_rep_lt_i64
.Ltest_rep_lt_i64$local:
	.type	.Ltest_rep_lt_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc_sp 7, 16
	br_lt_imm_i32 %r1, 1, .LBB716_3
	mov %r4, %gz

	! BUNDLE
	lda_iprel %r2, %pcfwd(.L.str.24)
	nop 0
	nop 0

.LBB716_2:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %r4, %sp, 0
	mov %r6, %r2
	call %r5, %jpc(printf)

	! BUNDLE
	add_imm_i32 %r4, %r4, 1
	br_lt_i32 %r4, %r1, .LBB716_2
	nop 0

.LBB716_3:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	retf 16
	nop 0
	nop 0

.Lfunc_end716:
	.size	test_rep_lt_i64, .Lfunc_end716-test_rep_lt_i64
	.size	.Ltest_rep_lt_i64$local, .Lfunc_end716-test_rep_lt_i64
                                        ! -- End function
	.globl	min_lt_i32                      ! -- Begin function min_lt_i32
	.p2align	4
	.type	min_lt_i32,@function
min_lt_i32:                             ! @min_lt_i32
.Lmin_lt_i32$local:
	.type	.Lmin_lt_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end717:
	.size	min_lt_i32, .Lfunc_end717-min_lt_i32
	.size	.Lmin_lt_i32$local, .Lfunc_end717-min_lt_i32
                                        ! -- End function
	.globl	min_lt_u32                      ! -- Begin function min_lt_u32
	.p2align	4
	.type	min_lt_u32,@function
min_lt_u32:                             ! @min_lt_u32
.Lmin_lt_u32$local:
	.type	.Lmin_lt_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end718:
	.size	min_lt_u32, .Lfunc_end718-min_lt_u32
	.size	.Lmin_lt_u32$local, .Lfunc_end718-min_lt_u32
                                        ! -- End function
	.globl	min_le_i32                      ! -- Begin function min_le_i32
	.p2align	4
	.type	min_le_i32,@function
min_le_i32:                             ! @min_le_i32
.Lmin_le_i32$local:
	.type	.Lmin_le_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end719:
	.size	min_le_i32, .Lfunc_end719-min_le_i32
	.size	.Lmin_le_i32$local, .Lfunc_end719-min_le_i32
                                        ! -- End function
	.globl	min_le_u32                      ! -- Begin function min_le_u32
	.p2align	4
	.type	min_le_u32,@function
min_le_u32:                             ! @min_le_u32
.Lmin_le_u32$local:
	.type	.Lmin_le_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end720:
	.size	min_le_u32, .Lfunc_end720-min_le_u32
	.size	.Lmin_le_u32$local, .Lfunc_end720-min_le_u32
                                        ! -- End function
	.globl	min_gt_i32                      ! -- Begin function min_gt_i32
	.p2align	4
	.type	min_gt_i32,@function
min_gt_i32:                             ! @min_gt_i32
.Lmin_gt_i32$local:
	.type	.Lmin_gt_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i32 %r1, %r2, %r1
	retf 0

.Lfunc_end721:
	.size	min_gt_i32, .Lfunc_end721-min_gt_i32
	.size	.Lmin_gt_i32$local, .Lfunc_end721-min_gt_i32
                                        ! -- End function
	.globl	min_gt_u32                      ! -- Begin function min_gt_u32
	.p2align	4
	.type	min_gt_u32,@function
min_gt_u32:                             ! @min_gt_u32
.Lmin_gt_u32$local:
	.type	.Lmin_gt_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end722:
	.size	min_gt_u32, .Lfunc_end722-min_gt_u32
	.size	.Lmin_gt_u32$local, .Lfunc_end722-min_gt_u32
                                        ! -- End function
	.globl	min_ge_i32                      ! -- Begin function min_ge_i32
	.p2align	4
	.type	min_ge_i32,@function
min_ge_i32:                             ! @min_ge_i32
.Lmin_ge_i32$local:
	.type	.Lmin_ge_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i32 %r1, %r2, %r1
	retf 0

.Lfunc_end723:
	.size	min_ge_i32, .Lfunc_end723-min_ge_i32
	.size	.Lmin_ge_i32$local, .Lfunc_end723-min_ge_i32
                                        ! -- End function
	.globl	min_ge_u32                      ! -- Begin function min_ge_u32
	.p2align	4
	.type	min_ge_u32,@function
min_ge_u32:                             ! @min_ge_u32
.Lmin_ge_u32$local:
	.type	.Lmin_ge_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end724:
	.size	min_ge_u32, .Lfunc_end724-min_ge_u32
	.size	.Lmin_ge_u32$local, .Lfunc_end724-min_ge_u32
                                        ! -- End function
	.globl	max_lt_i32                      ! -- Begin function max_lt_i32
	.p2align	4
	.type	max_lt_i32,@function
max_lt_i32:                             ! @max_lt_i32
.Lmax_lt_i32$local:
	.type	.Lmax_lt_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end725:
	.size	max_lt_i32, .Lfunc_end725-max_lt_i32
	.size	.Lmax_lt_i32$local, .Lfunc_end725-max_lt_i32
                                        ! -- End function
	.globl	max_lt_u32                      ! -- Begin function max_lt_u32
	.p2align	4
	.type	max_lt_u32,@function
max_lt_u32:                             ! @max_lt_u32
.Lmax_lt_u32$local:
	.type	.Lmax_lt_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end726:
	.size	max_lt_u32, .Lfunc_end726-max_lt_u32
	.size	.Lmax_lt_u32$local, .Lfunc_end726-max_lt_u32
                                        ! -- End function
	.globl	max_le_i32                      ! -- Begin function max_le_i32
	.p2align	4
	.type	max_le_i32,@function
max_le_i32:                             ! @max_le_i32
.Lmax_le_i32$local:
	.type	.Lmax_le_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i32 %r1, %r1, %r2
	retf 0

.Lfunc_end727:
	.size	max_le_i32, .Lfunc_end727-max_le_i32
	.size	.Lmax_le_i32$local, .Lfunc_end727-max_le_i32
                                        ! -- End function
	.globl	max_le_u32                      ! -- Begin function max_le_u32
	.p2align	4
	.type	max_le_u32,@function
max_le_u32:                             ! @max_le_u32
.Lmax_le_u32$local:
	.type	.Lmax_le_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u32 %r1, %r1, %r2
	retf 0

.Lfunc_end728:
	.size	max_le_u32, .Lfunc_end728-max_le_u32
	.size	.Lmax_le_u32$local, .Lfunc_end728-max_le_u32
                                        ! -- End function
	.globl	max_gt_i32                      ! -- Begin function max_gt_i32
	.p2align	4
	.type	max_gt_i32,@function
max_gt_i32:                             ! @max_gt_i32
.Lmax_gt_i32$local:
	.type	.Lmax_gt_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i32 %r1, %r2, %r1
	retf 0

.Lfunc_end729:
	.size	max_gt_i32, .Lfunc_end729-max_gt_i32
	.size	.Lmax_gt_i32$local, .Lfunc_end729-max_gt_i32
                                        ! -- End function
	.globl	max_gt_u32                      ! -- Begin function max_gt_u32
	.p2align	4
	.type	max_gt_u32,@function
max_gt_u32:                             ! @max_gt_u32
.Lmax_gt_u32$local:
	.type	.Lmax_gt_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end730:
	.size	max_gt_u32, .Lfunc_end730-max_gt_u32
	.size	.Lmax_gt_u32$local, .Lfunc_end730-max_gt_u32
                                        ! -- End function
	.globl	max_ge_i32                      ! -- Begin function max_ge_i32
	.p2align	4
	.type	max_ge_i32,@function
max_ge_i32:                             ! @max_ge_i32
.Lmax_ge_i32$local:
	.type	.Lmax_ge_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i32 %r1, %r2, %r1
	retf 0

.Lfunc_end731:
	.size	max_ge_i32, .Lfunc_end731-max_ge_i32
	.size	.Lmax_ge_i32$local, .Lfunc_end731-max_ge_i32
                                        ! -- End function
	.globl	max_ge_u32                      ! -- Begin function max_ge_u32
	.p2align	4
	.type	max_ge_u32,@function
max_ge_u32:                             ! @max_ge_u32
.Lmax_ge_u32$local:
	.type	.Lmax_ge_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u32 %r1, %r2, %r1
	retf 0

.Lfunc_end732:
	.size	max_ge_u32, .Lfunc_end732-max_ge_u32
	.size	.Lmax_ge_u32$local, .Lfunc_end732-max_ge_u32
                                        ! -- End function
	.globl	min_lt_i64                      ! -- Begin function min_lt_i64
	.p2align	4
	.type	min_lt_i64,@function
min_lt_i64:                             ! @min_lt_i64
.Lmin_lt_i64$local:
	.type	.Lmin_lt_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end733:
	.size	min_lt_i64, .Lfunc_end733-min_lt_i64
	.size	.Lmin_lt_i64$local, .Lfunc_end733-min_lt_i64
                                        ! -- End function
	.globl	min_lt_u64                      ! -- Begin function min_lt_u64
	.p2align	4
	.type	min_lt_u64,@function
min_lt_u64:                             ! @min_lt_u64
.Lmin_lt_u64$local:
	.type	.Lmin_lt_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end734:
	.size	min_lt_u64, .Lfunc_end734-min_lt_u64
	.size	.Lmin_lt_u64$local, .Lfunc_end734-min_lt_u64
                                        ! -- End function
	.globl	min_le_i64                      ! -- Begin function min_le_i64
	.p2align	4
	.type	min_le_i64,@function
min_le_i64:                             ! @min_le_i64
.Lmin_le_i64$local:
	.type	.Lmin_le_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end735:
	.size	min_le_i64, .Lfunc_end735-min_le_i64
	.size	.Lmin_le_i64$local, .Lfunc_end735-min_le_i64
                                        ! -- End function
	.globl	min_le_u64                      ! -- Begin function min_le_u64
	.p2align	4
	.type	min_le_u64,@function
min_le_u64:                             ! @min_le_u64
.Lmin_le_u64$local:
	.type	.Lmin_le_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end736:
	.size	min_le_u64, .Lfunc_end736-min_le_u64
	.size	.Lmin_le_u64$local, .Lfunc_end736-min_le_u64
                                        ! -- End function
	.globl	min_gt_i64                      ! -- Begin function min_gt_i64
	.p2align	4
	.type	min_gt_i64,@function
min_gt_i64:                             ! @min_gt_i64
.Lmin_gt_i64$local:
	.type	.Lmin_gt_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end737:
	.size	min_gt_i64, .Lfunc_end737-min_gt_i64
	.size	.Lmin_gt_i64$local, .Lfunc_end737-min_gt_i64
                                        ! -- End function
	.globl	min_gt_u64                      ! -- Begin function min_gt_u64
	.p2align	4
	.type	min_gt_u64,@function
min_gt_u64:                             ! @min_gt_u64
.Lmin_gt_u64$local:
	.type	.Lmin_gt_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end738:
	.size	min_gt_u64, .Lfunc_end738-min_gt_u64
	.size	.Lmin_gt_u64$local, .Lfunc_end738-min_gt_u64
                                        ! -- End function
	.globl	min_ge_i64                      ! -- Begin function min_ge_i64
	.p2align	4
	.type	min_ge_i64,@function
min_ge_i64:                             ! @min_ge_i64
.Lmin_ge_i64$local:
	.type	.Lmin_ge_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end739:
	.size	min_ge_i64, .Lfunc_end739-min_ge_i64
	.size	.Lmin_ge_i64$local, .Lfunc_end739-min_ge_i64
                                        ! -- End function
	.globl	min_ge_u64                      ! -- Begin function min_ge_u64
	.p2align	4
	.type	min_ge_u64,@function
min_ge_u64:                             ! @min_ge_u64
.Lmin_ge_u64$local:
	.type	.Lmin_ge_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	min_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end740:
	.size	min_ge_u64, .Lfunc_end740-min_ge_u64
	.size	.Lmin_ge_u64$local, .Lfunc_end740-min_ge_u64
                                        ! -- End function
	.globl	max_lt_i64                      ! -- Begin function max_lt_i64
	.p2align	4
	.type	max_lt_i64,@function
max_lt_i64:                             ! @max_lt_i64
.Lmax_lt_i64$local:
	.type	.Lmax_lt_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end741:
	.size	max_lt_i64, .Lfunc_end741-max_lt_i64
	.size	.Lmax_lt_i64$local, .Lfunc_end741-max_lt_i64
                                        ! -- End function
	.globl	max_lt_u64                      ! -- Begin function max_lt_u64
	.p2align	4
	.type	max_lt_u64,@function
max_lt_u64:                             ! @max_lt_u64
.Lmax_lt_u64$local:
	.type	.Lmax_lt_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end742:
	.size	max_lt_u64, .Lfunc_end742-max_lt_u64
	.size	.Lmax_lt_u64$local, .Lfunc_end742-max_lt_u64
                                        ! -- End function
	.globl	max_le_i64                      ! -- Begin function max_le_i64
	.p2align	4
	.type	max_le_i64,@function
max_le_i64:                             ! @max_le_i64
.Lmax_le_i64$local:
	.type	.Lmax_le_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i64 %r1, %r1, %r2
	retf 0

.Lfunc_end743:
	.size	max_le_i64, .Lfunc_end743-max_le_i64
	.size	.Lmax_le_i64$local, .Lfunc_end743-max_le_i64
                                        ! -- End function
	.globl	max_le_u64                      ! -- Begin function max_le_u64
	.p2align	4
	.type	max_le_u64,@function
max_le_u64:                             ! @max_le_u64
.Lmax_le_u64$local:
	.type	.Lmax_le_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u64 %r1, %r1, %r2
	retf 0

.Lfunc_end744:
	.size	max_le_u64, .Lfunc_end744-max_le_u64
	.size	.Lmax_le_u64$local, .Lfunc_end744-max_le_u64
                                        ! -- End function
	.globl	max_gt_i64                      ! -- Begin function max_gt_i64
	.p2align	4
	.type	max_gt_i64,@function
max_gt_i64:                             ! @max_gt_i64
.Lmax_gt_i64$local:
	.type	.Lmax_gt_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end745:
	.size	max_gt_i64, .Lfunc_end745-max_gt_i64
	.size	.Lmax_gt_i64$local, .Lfunc_end745-max_gt_i64
                                        ! -- End function
	.globl	max_gt_u64                      ! -- Begin function max_gt_u64
	.p2align	4
	.type	max_gt_u64,@function
max_gt_u64:                             ! @max_gt_u64
.Lmax_gt_u64$local:
	.type	.Lmax_gt_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end746:
	.size	max_gt_u64, .Lfunc_end746-max_gt_u64
	.size	.Lmax_gt_u64$local, .Lfunc_end746-max_gt_u64
                                        ! -- End function
	.globl	max_ge_i64                      ! -- Begin function max_ge_i64
	.p2align	4
	.type	max_ge_i64,@function
max_ge_i64:                             ! @max_ge_i64
.Lmax_ge_i64$local:
	.type	.Lmax_ge_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end747:
	.size	max_ge_i64, .Lfunc_end747-max_ge_i64
	.size	.Lmax_ge_i64$local, .Lfunc_end747-max_ge_i64
                                        ! -- End function
	.globl	max_ge_u64                      ! -- Begin function max_ge_u64
	.p2align	4
	.type	max_ge_u64,@function
max_ge_u64:                             ! @max_ge_u64
.Lmax_ge_u64$local:
	.type	.Lmax_ge_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	max_u64 %r1, %r2, %r1
	retf 0

.Lfunc_end748:
	.size	max_ge_u64, .Lfunc_end748-max_ge_u64
	.size	.Lmax_ge_u64$local, .Lfunc_end748-max_ge_u64
                                        ! -- End function
	.globl	min_lt_imm_i32                  ! -- Begin function min_lt_imm_i32
	.p2align	4
	.type	min_lt_imm_i32,@function
min_lt_imm_i32:                         ! @min_lt_imm_i32
.Lmin_lt_imm_i32$local:
	.type	.Lmin_lt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end749:
	.size	min_lt_imm_i32, .Lfunc_end749-min_lt_imm_i32
	.size	.Lmin_lt_imm_i32$local, .Lfunc_end749-min_lt_imm_i32
                                        ! -- End function
	.globl	min_lt_imm_u32                  ! -- Begin function min_lt_imm_u32
	.p2align	4
	.type	min_lt_imm_u32,@function
min_lt_imm_u32:                         ! @min_lt_imm_u32
.Lmin_lt_imm_u32$local:
	.type	.Lmin_lt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end750:
	.size	min_lt_imm_u32, .Lfunc_end750-min_lt_imm_u32
	.size	.Lmin_lt_imm_u32$local, .Lfunc_end750-min_lt_imm_u32
                                        ! -- End function
	.globl	min_le_imm_i32                  ! -- Begin function min_le_imm_i32
	.p2align	4
	.type	min_le_imm_i32,@function
min_le_imm_i32:                         ! @min_le_imm_i32
.Lmin_le_imm_i32$local:
	.type	.Lmin_le_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end751:
	.size	min_le_imm_i32, .Lfunc_end751-min_le_imm_i32
	.size	.Lmin_le_imm_i32$local, .Lfunc_end751-min_le_imm_i32
                                        ! -- End function
	.globl	min_le_imm_u32                  ! -- Begin function min_le_imm_u32
	.p2align	4
	.type	min_le_imm_u32,@function
min_le_imm_u32:                         ! @min_le_imm_u32
.Lmin_le_imm_u32$local:
	.type	.Lmin_le_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end752:
	.size	min_le_imm_u32, .Lfunc_end752-min_le_imm_u32
	.size	.Lmin_le_imm_u32$local, .Lfunc_end752-min_le_imm_u32
                                        ! -- End function
	.globl	min_gt_imm_i32                  ! -- Begin function min_gt_imm_i32
	.p2align	4
	.type	min_gt_imm_i32,@function
min_gt_imm_i32:                         ! @min_gt_imm_i32
.Lmin_gt_imm_i32$local:
	.type	.Lmin_gt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end753:
	.size	min_gt_imm_i32, .Lfunc_end753-min_gt_imm_i32
	.size	.Lmin_gt_imm_i32$local, .Lfunc_end753-min_gt_imm_i32
                                        ! -- End function
	.globl	min_gt_imm_u32                  ! -- Begin function min_gt_imm_u32
	.p2align	4
	.type	min_gt_imm_u32,@function
min_gt_imm_u32:                         ! @min_gt_imm_u32
.Lmin_gt_imm_u32$local:
	.type	.Lmin_gt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end754:
	.size	min_gt_imm_u32, .Lfunc_end754-min_gt_imm_u32
	.size	.Lmin_gt_imm_u32$local, .Lfunc_end754-min_gt_imm_u32
                                        ! -- End function
	.globl	min_ge_imm_i32                  ! -- Begin function min_ge_imm_i32
	.p2align	4
	.type	min_ge_imm_i32,@function
min_ge_imm_i32:                         ! @min_ge_imm_i32
.Lmin_ge_imm_i32$local:
	.type	.Lmin_ge_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end755:
	.size	min_ge_imm_i32, .Lfunc_end755-min_ge_imm_i32
	.size	.Lmin_ge_imm_i32$local, .Lfunc_end755-min_ge_imm_i32
                                        ! -- End function
	.globl	min_ge_imm_u32                  ! -- Begin function min_ge_imm_u32
	.p2align	4
	.type	min_ge_imm_u32,@function
min_ge_imm_u32:                         ! @min_ge_imm_u32
.Lmin_ge_imm_u32$local:
	.type	.Lmin_ge_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end756:
	.size	min_ge_imm_u32, .Lfunc_end756-min_ge_imm_u32
	.size	.Lmin_ge_imm_u32$local, .Lfunc_end756-min_ge_imm_u32
                                        ! -- End function
	.globl	min_lt_imm_i32_ext              ! -- Begin function min_lt_imm_i32_ext
	.p2align	4
	.type	min_lt_imm_i32_ext,@function
min_lt_imm_i32_ext:                     ! @min_lt_imm_i32_ext
.Lmin_lt_imm_i32_ext$local:
	.type	.Lmin_lt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end757:
	.size	min_lt_imm_i32_ext, .Lfunc_end757-min_lt_imm_i32_ext
	.size	.Lmin_lt_imm_i32_ext$local, .Lfunc_end757-min_lt_imm_i32_ext
                                        ! -- End function
	.globl	min_lt_imm_u32_ext              ! -- Begin function min_lt_imm_u32_ext
	.p2align	4
	.type	min_lt_imm_u32_ext,@function
min_lt_imm_u32_ext:                     ! @min_lt_imm_u32_ext
.Lmin_lt_imm_u32_ext$local:
	.type	.Lmin_lt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end758:
	.size	min_lt_imm_u32_ext, .Lfunc_end758-min_lt_imm_u32_ext
	.size	.Lmin_lt_imm_u32_ext$local, .Lfunc_end758-min_lt_imm_u32_ext
                                        ! -- End function
	.globl	min_le_imm_i32_ext              ! -- Begin function min_le_imm_i32_ext
	.p2align	4
	.type	min_le_imm_i32_ext,@function
min_le_imm_i32_ext:                     ! @min_le_imm_i32_ext
.Lmin_le_imm_i32_ext$local:
	.type	.Lmin_le_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end759:
	.size	min_le_imm_i32_ext, .Lfunc_end759-min_le_imm_i32_ext
	.size	.Lmin_le_imm_i32_ext$local, .Lfunc_end759-min_le_imm_i32_ext
                                        ! -- End function
	.globl	min_le_imm_u32_ext              ! -- Begin function min_le_imm_u32_ext
	.p2align	4
	.type	min_le_imm_u32_ext,@function
min_le_imm_u32_ext:                     ! @min_le_imm_u32_ext
.Lmin_le_imm_u32_ext$local:
	.type	.Lmin_le_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end760:
	.size	min_le_imm_u32_ext, .Lfunc_end760-min_le_imm_u32_ext
	.size	.Lmin_le_imm_u32_ext$local, .Lfunc_end760-min_le_imm_u32_ext
                                        ! -- End function
	.globl	min_gt_imm_i32_ext              ! -- Begin function min_gt_imm_i32_ext
	.p2align	4
	.type	min_gt_imm_i32_ext,@function
min_gt_imm_i32_ext:                     ! @min_gt_imm_i32_ext
.Lmin_gt_imm_i32_ext$local:
	.type	.Lmin_gt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end761:
	.size	min_gt_imm_i32_ext, .Lfunc_end761-min_gt_imm_i32_ext
	.size	.Lmin_gt_imm_i32_ext$local, .Lfunc_end761-min_gt_imm_i32_ext
                                        ! -- End function
	.globl	min_gt_imm_u32_ext              ! -- Begin function min_gt_imm_u32_ext
	.p2align	4
	.type	min_gt_imm_u32_ext,@function
min_gt_imm_u32_ext:                     ! @min_gt_imm_u32_ext
.Lmin_gt_imm_u32_ext$local:
	.type	.Lmin_gt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end762:
	.size	min_gt_imm_u32_ext, .Lfunc_end762-min_gt_imm_u32_ext
	.size	.Lmin_gt_imm_u32_ext$local, .Lfunc_end762-min_gt_imm_u32_ext
                                        ! -- End function
	.globl	min_ge_imm_i32_ext              ! -- Begin function min_ge_imm_i32_ext
	.p2align	4
	.type	min_ge_imm_i32_ext,@function
min_ge_imm_i32_ext:                     ! @min_ge_imm_i32_ext
.Lmin_ge_imm_i32_ext$local:
	.type	.Lmin_ge_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end763:
	.size	min_ge_imm_i32_ext, .Lfunc_end763-min_ge_imm_i32_ext
	.size	.Lmin_ge_imm_i32_ext$local, .Lfunc_end763-min_ge_imm_i32_ext
                                        ! -- End function
	.globl	min_ge_imm_u32_ext              ! -- Begin function min_ge_imm_u32_ext
	.p2align	4
	.type	min_ge_imm_u32_ext,@function
min_ge_imm_u32_ext:                     ! @min_ge_imm_u32_ext
.Lmin_ge_imm_u32_ext$local:
	.type	.Lmin_ge_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end764:
	.size	min_ge_imm_u32_ext, .Lfunc_end764-min_ge_imm_u32_ext
	.size	.Lmin_ge_imm_u32_ext$local, .Lfunc_end764-min_ge_imm_u32_ext
                                        ! -- End function
	.globl	max_lt_imm_i32                  ! -- Begin function max_lt_imm_i32
	.p2align	4
	.type	max_lt_imm_i32,@function
max_lt_imm_i32:                         ! @max_lt_imm_i32
.Lmax_lt_imm_i32$local:
	.type	.Lmax_lt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end765:
	.size	max_lt_imm_i32, .Lfunc_end765-max_lt_imm_i32
	.size	.Lmax_lt_imm_i32$local, .Lfunc_end765-max_lt_imm_i32
                                        ! -- End function
	.globl	max_lt_imm_u32                  ! -- Begin function max_lt_imm_u32
	.p2align	4
	.type	max_lt_imm_u32,@function
max_lt_imm_u32:                         ! @max_lt_imm_u32
.Lmax_lt_imm_u32$local:
	.type	.Lmax_lt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end766:
	.size	max_lt_imm_u32, .Lfunc_end766-max_lt_imm_u32
	.size	.Lmax_lt_imm_u32$local, .Lfunc_end766-max_lt_imm_u32
                                        ! -- End function
	.globl	max_le_imm_i32                  ! -- Begin function max_le_imm_i32
	.p2align	4
	.type	max_le_imm_i32,@function
max_le_imm_i32:                         ! @max_le_imm_i32
.Lmax_le_imm_i32$local:
	.type	.Lmax_le_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end767:
	.size	max_le_imm_i32, .Lfunc_end767-max_le_imm_i32
	.size	.Lmax_le_imm_i32$local, .Lfunc_end767-max_le_imm_i32
                                        ! -- End function
	.globl	max_le_imm_u32                  ! -- Begin function max_le_imm_u32
	.p2align	4
	.type	max_le_imm_u32,@function
max_le_imm_u32:                         ! @max_le_imm_u32
.Lmax_le_imm_u32$local:
	.type	.Lmax_le_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end768:
	.size	max_le_imm_u32, .Lfunc_end768-max_le_imm_u32
	.size	.Lmax_le_imm_u32$local, .Lfunc_end768-max_le_imm_u32
                                        ! -- End function
	.globl	max_gt_imm_i32                  ! -- Begin function max_gt_imm_i32
	.p2align	4
	.type	max_gt_imm_i32,@function
max_gt_imm_i32:                         ! @max_gt_imm_i32
.Lmax_gt_imm_i32$local:
	.type	.Lmax_gt_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end769:
	.size	max_gt_imm_i32, .Lfunc_end769-max_gt_imm_i32
	.size	.Lmax_gt_imm_i32$local, .Lfunc_end769-max_gt_imm_i32
                                        ! -- End function
	.globl	max_gt_imm_u32                  ! -- Begin function max_gt_imm_u32
	.p2align	4
	.type	max_gt_imm_u32,@function
max_gt_imm_u32:                         ! @max_gt_imm_u32
.Lmax_gt_imm_u32$local:
	.type	.Lmax_gt_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end770:
	.size	max_gt_imm_u32, .Lfunc_end770-max_gt_imm_u32
	.size	.Lmax_gt_imm_u32$local, .Lfunc_end770-max_gt_imm_u32
                                        ! -- End function
	.globl	max_ge_imm_i32                  ! -- Begin function max_ge_imm_i32
	.p2align	4
	.type	max_ge_imm_i32,@function
max_ge_imm_i32:                         ! @max_ge_imm_i32
.Lmax_ge_imm_i32$local:
	.type	.Lmax_ge_imm_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32 %r1, %r1, 1000
	retf 0

.Lfunc_end771:
	.size	max_ge_imm_i32, .Lfunc_end771-max_ge_imm_i32
	.size	.Lmax_ge_imm_i32$local, .Lfunc_end771-max_ge_imm_i32
                                        ! -- End function
	.globl	max_ge_imm_u32                  ! -- Begin function max_ge_imm_u32
	.p2align	4
	.type	max_ge_imm_u32,@function
max_ge_imm_u32:                         ! @max_ge_imm_u32
.Lmax_ge_imm_u32$local:
	.type	.Lmax_ge_imm_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32 %r1, %r1, 1000
	retf 0

.Lfunc_end772:
	.size	max_ge_imm_u32, .Lfunc_end772-max_ge_imm_u32
	.size	.Lmax_ge_imm_u32$local, .Lfunc_end772-max_ge_imm_u32
                                        ! -- End function
	.globl	max_lt_imm_i32_ext              ! -- Begin function max_lt_imm_i32_ext
	.p2align	4
	.type	max_lt_imm_i32_ext,@function
max_lt_imm_i32_ext:                     ! @max_lt_imm_i32_ext
.Lmax_lt_imm_i32_ext$local:
	.type	.Lmax_lt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end773:
	.size	max_lt_imm_i32_ext, .Lfunc_end773-max_lt_imm_i32_ext
	.size	.Lmax_lt_imm_i32_ext$local, .Lfunc_end773-max_lt_imm_i32_ext
                                        ! -- End function
	.globl	max_lt_imm_u32_ext              ! -- Begin function max_lt_imm_u32_ext
	.p2align	4
	.type	max_lt_imm_u32_ext,@function
max_lt_imm_u32_ext:                     ! @max_lt_imm_u32_ext
.Lmax_lt_imm_u32_ext$local:
	.type	.Lmax_lt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end774:
	.size	max_lt_imm_u32_ext, .Lfunc_end774-max_lt_imm_u32_ext
	.size	.Lmax_lt_imm_u32_ext$local, .Lfunc_end774-max_lt_imm_u32_ext
                                        ! -- End function
	.globl	max_le_imm_i32_ext              ! -- Begin function max_le_imm_i32_ext
	.p2align	4
	.type	max_le_imm_i32_ext,@function
max_le_imm_i32_ext:                     ! @max_le_imm_i32_ext
.Lmax_le_imm_i32_ext$local:
	.type	.Lmax_le_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end775:
	.size	max_le_imm_i32_ext, .Lfunc_end775-max_le_imm_i32_ext
	.size	.Lmax_le_imm_i32_ext$local, .Lfunc_end775-max_le_imm_i32_ext
                                        ! -- End function
	.globl	max_le_imm_u32_ext              ! -- Begin function max_le_imm_u32_ext
	.p2align	4
	.type	max_le_imm_u32_ext,@function
max_le_imm_u32_ext:                     ! @max_le_imm_u32_ext
.Lmax_le_imm_u32_ext$local:
	.type	.Lmax_le_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end776:
	.size	max_le_imm_u32_ext, .Lfunc_end776-max_le_imm_u32_ext
	.size	.Lmax_le_imm_u32_ext$local, .Lfunc_end776-max_le_imm_u32_ext
                                        ! -- End function
	.globl	max_gt_imm_i32_ext              ! -- Begin function max_gt_imm_i32_ext
	.p2align	4
	.type	max_gt_imm_i32_ext,@function
max_gt_imm_i32_ext:                     ! @max_gt_imm_i32_ext
.Lmax_gt_imm_i32_ext$local:
	.type	.Lmax_gt_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end777:
	.size	max_gt_imm_i32_ext, .Lfunc_end777-max_gt_imm_i32_ext
	.size	.Lmax_gt_imm_i32_ext$local, .Lfunc_end777-max_gt_imm_i32_ext
                                        ! -- End function
	.globl	max_gt_imm_u32_ext              ! -- Begin function max_gt_imm_u32_ext
	.p2align	4
	.type	max_gt_imm_u32_ext,@function
max_gt_imm_u32_ext:                     ! @max_gt_imm_u32_ext
.Lmax_gt_imm_u32_ext$local:
	.type	.Lmax_gt_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end778:
	.size	max_gt_imm_u32_ext, .Lfunc_end778-max_gt_imm_u32_ext
	.size	.Lmax_gt_imm_u32_ext$local, .Lfunc_end778-max_gt_imm_u32_ext
                                        ! -- End function
	.globl	max_ge_imm_i32_ext              ! -- Begin function max_ge_imm_i32_ext
	.p2align	4
	.type	max_ge_imm_i32_ext,@function
max_ge_imm_i32_ext:                     ! @max_ge_imm_i32_ext
.Lmax_ge_imm_i32_ext$local:
	.type	.Lmax_ge_imm_i32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end779:
	.size	max_ge_imm_i32_ext, .Lfunc_end779-max_ge_imm_i32_ext
	.size	.Lmax_ge_imm_i32_ext$local, .Lfunc_end779-max_ge_imm_i32_ext
                                        ! -- End function
	.globl	max_ge_imm_u32_ext              ! -- Begin function max_ge_imm_u32_ext
	.p2align	4
	.type	max_ge_imm_u32_ext,@function
max_ge_imm_u32_ext:                     ! @max_ge_imm_u32_ext
.Lmax_ge_imm_u32_ext$local:
	.type	.Lmax_ge_imm_u32_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u32.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end780:
	.size	max_ge_imm_u32_ext, .Lfunc_end780-max_ge_imm_u32_ext
	.size	.Lmax_ge_imm_u32_ext$local, .Lfunc_end780-max_ge_imm_u32_ext
                                        ! -- End function
	.globl	min_lt_imm_i64                  ! -- Begin function min_lt_imm_i64
	.p2align	4
	.type	min_lt_imm_i64,@function
min_lt_imm_i64:                         ! @min_lt_imm_i64
.Lmin_lt_imm_i64$local:
	.type	.Lmin_lt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end781:
	.size	min_lt_imm_i64, .Lfunc_end781-min_lt_imm_i64
	.size	.Lmin_lt_imm_i64$local, .Lfunc_end781-min_lt_imm_i64
                                        ! -- End function
	.globl	min_lt_imm_u64                  ! -- Begin function min_lt_imm_u64
	.p2align	4
	.type	min_lt_imm_u64,@function
min_lt_imm_u64:                         ! @min_lt_imm_u64
.Lmin_lt_imm_u64$local:
	.type	.Lmin_lt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end782:
	.size	min_lt_imm_u64, .Lfunc_end782-min_lt_imm_u64
	.size	.Lmin_lt_imm_u64$local, .Lfunc_end782-min_lt_imm_u64
                                        ! -- End function
	.globl	min_le_imm_i64                  ! -- Begin function min_le_imm_i64
	.p2align	4
	.type	min_le_imm_i64,@function
min_le_imm_i64:                         ! @min_le_imm_i64
.Lmin_le_imm_i64$local:
	.type	.Lmin_le_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end783:
	.size	min_le_imm_i64, .Lfunc_end783-min_le_imm_i64
	.size	.Lmin_le_imm_i64$local, .Lfunc_end783-min_le_imm_i64
                                        ! -- End function
	.globl	min_le_imm_u64                  ! -- Begin function min_le_imm_u64
	.p2align	4
	.type	min_le_imm_u64,@function
min_le_imm_u64:                         ! @min_le_imm_u64
.Lmin_le_imm_u64$local:
	.type	.Lmin_le_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end784:
	.size	min_le_imm_u64, .Lfunc_end784-min_le_imm_u64
	.size	.Lmin_le_imm_u64$local, .Lfunc_end784-min_le_imm_u64
                                        ! -- End function
	.globl	min_gt_imm_i64                  ! -- Begin function min_gt_imm_i64
	.p2align	4
	.type	min_gt_imm_i64,@function
min_gt_imm_i64:                         ! @min_gt_imm_i64
.Lmin_gt_imm_i64$local:
	.type	.Lmin_gt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end785:
	.size	min_gt_imm_i64, .Lfunc_end785-min_gt_imm_i64
	.size	.Lmin_gt_imm_i64$local, .Lfunc_end785-min_gt_imm_i64
                                        ! -- End function
	.globl	min_gt_imm_u64                  ! -- Begin function min_gt_imm_u64
	.p2align	4
	.type	min_gt_imm_u64,@function
min_gt_imm_u64:                         ! @min_gt_imm_u64
.Lmin_gt_imm_u64$local:
	.type	.Lmin_gt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end786:
	.size	min_gt_imm_u64, .Lfunc_end786-min_gt_imm_u64
	.size	.Lmin_gt_imm_u64$local, .Lfunc_end786-min_gt_imm_u64
                                        ! -- End function
	.globl	min_ge_imm_i64                  ! -- Begin function min_ge_imm_i64
	.p2align	4
	.type	min_ge_imm_i64,@function
min_ge_imm_i64:                         ! @min_ge_imm_i64
.Lmin_ge_imm_i64$local:
	.type	.Lmin_ge_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end787:
	.size	min_ge_imm_i64, .Lfunc_end787-min_ge_imm_i64
	.size	.Lmin_ge_imm_i64$local, .Lfunc_end787-min_ge_imm_i64
                                        ! -- End function
	.globl	min_ge_imm_u64                  ! -- Begin function min_ge_imm_u64
	.p2align	4
	.type	min_ge_imm_u64,@function
min_ge_imm_u64:                         ! @min_ge_imm_u64
.Lmin_ge_imm_u64$local:
	.type	.Lmin_ge_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end788:
	.size	min_ge_imm_u64, .Lfunc_end788-min_ge_imm_u64
	.size	.Lmin_ge_imm_u64$local, .Lfunc_end788-min_ge_imm_u64
                                        ! -- End function
	.globl	min_lt_imm_i64_ext              ! -- Begin function min_lt_imm_i64_ext
	.p2align	4
	.type	min_lt_imm_i64_ext,@function
min_lt_imm_i64_ext:                     ! @min_lt_imm_i64_ext
.Lmin_lt_imm_i64_ext$local:
	.type	.Lmin_lt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end789:
	.size	min_lt_imm_i64_ext, .Lfunc_end789-min_lt_imm_i64_ext
	.size	.Lmin_lt_imm_i64_ext$local, .Lfunc_end789-min_lt_imm_i64_ext
                                        ! -- End function
	.globl	min_lt_imm_u64_ext              ! -- Begin function min_lt_imm_u64_ext
	.p2align	4
	.type	min_lt_imm_u64_ext,@function
min_lt_imm_u64_ext:                     ! @min_lt_imm_u64_ext
.Lmin_lt_imm_u64_ext$local:
	.type	.Lmin_lt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end790:
	.size	min_lt_imm_u64_ext, .Lfunc_end790-min_lt_imm_u64_ext
	.size	.Lmin_lt_imm_u64_ext$local, .Lfunc_end790-min_lt_imm_u64_ext
                                        ! -- End function
	.globl	min_le_imm_i64_ext              ! -- Begin function min_le_imm_i64_ext
	.p2align	4
	.type	min_le_imm_i64_ext,@function
min_le_imm_i64_ext:                     ! @min_le_imm_i64_ext
.Lmin_le_imm_i64_ext$local:
	.type	.Lmin_le_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end791:
	.size	min_le_imm_i64_ext, .Lfunc_end791-min_le_imm_i64_ext
	.size	.Lmin_le_imm_i64_ext$local, .Lfunc_end791-min_le_imm_i64_ext
                                        ! -- End function
	.globl	min_le_imm_u64_ext              ! -- Begin function min_le_imm_u64_ext
	.p2align	4
	.type	min_le_imm_u64_ext,@function
min_le_imm_u64_ext:                     ! @min_le_imm_u64_ext
.Lmin_le_imm_u64_ext$local:
	.type	.Lmin_le_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end792:
	.size	min_le_imm_u64_ext, .Lfunc_end792-min_le_imm_u64_ext
	.size	.Lmin_le_imm_u64_ext$local, .Lfunc_end792-min_le_imm_u64_ext
                                        ! -- End function
	.globl	min_gt_imm_i64_ext              ! -- Begin function min_gt_imm_i64_ext
	.p2align	4
	.type	min_gt_imm_i64_ext,@function
min_gt_imm_i64_ext:                     ! @min_gt_imm_i64_ext
.Lmin_gt_imm_i64_ext$local:
	.type	.Lmin_gt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end793:
	.size	min_gt_imm_i64_ext, .Lfunc_end793-min_gt_imm_i64_ext
	.size	.Lmin_gt_imm_i64_ext$local, .Lfunc_end793-min_gt_imm_i64_ext
                                        ! -- End function
	.globl	min_gt_imm_u64_ext              ! -- Begin function min_gt_imm_u64_ext
	.p2align	4
	.type	min_gt_imm_u64_ext,@function
min_gt_imm_u64_ext:                     ! @min_gt_imm_u64_ext
.Lmin_gt_imm_u64_ext$local:
	.type	.Lmin_gt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end794:
	.size	min_gt_imm_u64_ext, .Lfunc_end794-min_gt_imm_u64_ext
	.size	.Lmin_gt_imm_u64_ext$local, .Lfunc_end794-min_gt_imm_u64_ext
                                        ! -- End function
	.globl	min_ge_imm_i64_ext              ! -- Begin function min_ge_imm_i64_ext
	.p2align	4
	.type	min_ge_imm_i64_ext,@function
min_ge_imm_i64_ext:                     ! @min_ge_imm_i64_ext
.Lmin_ge_imm_i64_ext$local:
	.type	.Lmin_ge_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end795:
	.size	min_ge_imm_i64_ext, .Lfunc_end795-min_ge_imm_i64_ext
	.size	.Lmin_ge_imm_i64_ext$local, .Lfunc_end795-min_ge_imm_i64_ext
                                        ! -- End function
	.globl	min_ge_imm_u64_ext              ! -- Begin function min_ge_imm_u64_ext
	.p2align	4
	.type	min_ge_imm_u64_ext,@function
min_ge_imm_u64_ext:                     ! @min_ge_imm_u64_ext
.Lmin_ge_imm_u64_ext$local:
	.type	.Lmin_ge_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	min_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end796:
	.size	min_ge_imm_u64_ext, .Lfunc_end796-min_ge_imm_u64_ext
	.size	.Lmin_ge_imm_u64_ext$local, .Lfunc_end796-min_ge_imm_u64_ext
                                        ! -- End function
	.globl	max_lt_imm_i64                  ! -- Begin function max_lt_imm_i64
	.p2align	4
	.type	max_lt_imm_i64,@function
max_lt_imm_i64:                         ! @max_lt_imm_i64
.Lmax_lt_imm_i64$local:
	.type	.Lmax_lt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end797:
	.size	max_lt_imm_i64, .Lfunc_end797-max_lt_imm_i64
	.size	.Lmax_lt_imm_i64$local, .Lfunc_end797-max_lt_imm_i64
                                        ! -- End function
	.globl	max_lt_imm_u64                  ! -- Begin function max_lt_imm_u64
	.p2align	4
	.type	max_lt_imm_u64,@function
max_lt_imm_u64:                         ! @max_lt_imm_u64
.Lmax_lt_imm_u64$local:
	.type	.Lmax_lt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end798:
	.size	max_lt_imm_u64, .Lfunc_end798-max_lt_imm_u64
	.size	.Lmax_lt_imm_u64$local, .Lfunc_end798-max_lt_imm_u64
                                        ! -- End function
	.globl	max_le_imm_i64                  ! -- Begin function max_le_imm_i64
	.p2align	4
	.type	max_le_imm_i64,@function
max_le_imm_i64:                         ! @max_le_imm_i64
.Lmax_le_imm_i64$local:
	.type	.Lmax_le_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end799:
	.size	max_le_imm_i64, .Lfunc_end799-max_le_imm_i64
	.size	.Lmax_le_imm_i64$local, .Lfunc_end799-max_le_imm_i64
                                        ! -- End function
	.globl	max_le_imm_u64                  ! -- Begin function max_le_imm_u64
	.p2align	4
	.type	max_le_imm_u64,@function
max_le_imm_u64:                         ! @max_le_imm_u64
.Lmax_le_imm_u64$local:
	.type	.Lmax_le_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end800:
	.size	max_le_imm_u64, .Lfunc_end800-max_le_imm_u64
	.size	.Lmax_le_imm_u64$local, .Lfunc_end800-max_le_imm_u64
                                        ! -- End function
	.globl	max_gt_imm_i64                  ! -- Begin function max_gt_imm_i64
	.p2align	4
	.type	max_gt_imm_i64,@function
max_gt_imm_i64:                         ! @max_gt_imm_i64
.Lmax_gt_imm_i64$local:
	.type	.Lmax_gt_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end801:
	.size	max_gt_imm_i64, .Lfunc_end801-max_gt_imm_i64
	.size	.Lmax_gt_imm_i64$local, .Lfunc_end801-max_gt_imm_i64
                                        ! -- End function
	.globl	max_gt_imm_u64                  ! -- Begin function max_gt_imm_u64
	.p2align	4
	.type	max_gt_imm_u64,@function
max_gt_imm_u64:                         ! @max_gt_imm_u64
.Lmax_gt_imm_u64$local:
	.type	.Lmax_gt_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end802:
	.size	max_gt_imm_u64, .Lfunc_end802-max_gt_imm_u64
	.size	.Lmax_gt_imm_u64$local, .Lfunc_end802-max_gt_imm_u64
                                        ! -- End function
	.globl	max_ge_imm_i64                  ! -- Begin function max_ge_imm_i64
	.p2align	4
	.type	max_ge_imm_i64,@function
max_ge_imm_i64:                         ! @max_ge_imm_i64
.Lmax_ge_imm_i64$local:
	.type	.Lmax_ge_imm_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64 %r1, %r1, 1000
	retf 0

.Lfunc_end803:
	.size	max_ge_imm_i64, .Lfunc_end803-max_ge_imm_i64
	.size	.Lmax_ge_imm_i64$local, .Lfunc_end803-max_ge_imm_i64
                                        ! -- End function
	.globl	max_ge_imm_u64                  ! -- Begin function max_ge_imm_u64
	.p2align	4
	.type	max_ge_imm_u64,@function
max_ge_imm_u64:                         ! @max_ge_imm_u64
.Lmax_ge_imm_u64$local:
	.type	.Lmax_ge_imm_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64 %r1, %r1, 1000
	retf 0

.Lfunc_end804:
	.size	max_ge_imm_u64, .Lfunc_end804-max_ge_imm_u64
	.size	.Lmax_ge_imm_u64$local, .Lfunc_end804-max_ge_imm_u64
                                        ! -- End function
	.globl	max_lt_imm_i64_ext              ! -- Begin function max_lt_imm_i64_ext
	.p2align	4
	.type	max_lt_imm_i64_ext,@function
max_lt_imm_i64_ext:                     ! @max_lt_imm_i64_ext
.Lmax_lt_imm_i64_ext$local:
	.type	.Lmax_lt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end805:
	.size	max_lt_imm_i64_ext, .Lfunc_end805-max_lt_imm_i64_ext
	.size	.Lmax_lt_imm_i64_ext$local, .Lfunc_end805-max_lt_imm_i64_ext
                                        ! -- End function
	.globl	max_lt_imm_u64_ext              ! -- Begin function max_lt_imm_u64_ext
	.p2align	4
	.type	max_lt_imm_u64_ext,@function
max_lt_imm_u64_ext:                     ! @max_lt_imm_u64_ext
.Lmax_lt_imm_u64_ext$local:
	.type	.Lmax_lt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end806:
	.size	max_lt_imm_u64_ext, .Lfunc_end806-max_lt_imm_u64_ext
	.size	.Lmax_lt_imm_u64_ext$local, .Lfunc_end806-max_lt_imm_u64_ext
                                        ! -- End function
	.globl	max_le_imm_i64_ext              ! -- Begin function max_le_imm_i64_ext
	.p2align	4
	.type	max_le_imm_i64_ext,@function
max_le_imm_i64_ext:                     ! @max_le_imm_i64_ext
.Lmax_le_imm_i64_ext$local:
	.type	.Lmax_le_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end807:
	.size	max_le_imm_i64_ext, .Lfunc_end807-max_le_imm_i64_ext
	.size	.Lmax_le_imm_i64_ext$local, .Lfunc_end807-max_le_imm_i64_ext
                                        ! -- End function
	.globl	max_le_imm_u64_ext              ! -- Begin function max_le_imm_u64_ext
	.p2align	4
	.type	max_le_imm_u64_ext,@function
max_le_imm_u64_ext:                     ! @max_le_imm_u64_ext
.Lmax_le_imm_u64_ext$local:
	.type	.Lmax_le_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end808:
	.size	max_le_imm_u64_ext, .Lfunc_end808-max_le_imm_u64_ext
	.size	.Lmax_le_imm_u64_ext$local, .Lfunc_end808-max_le_imm_u64_ext
                                        ! -- End function
	.globl	max_gt_imm_i64_ext              ! -- Begin function max_gt_imm_i64_ext
	.p2align	4
	.type	max_gt_imm_i64_ext,@function
max_gt_imm_i64_ext:                     ! @max_gt_imm_i64_ext
.Lmax_gt_imm_i64_ext$local:
	.type	.Lmax_gt_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end809:
	.size	max_gt_imm_i64_ext, .Lfunc_end809-max_gt_imm_i64_ext
	.size	.Lmax_gt_imm_i64_ext$local, .Lfunc_end809-max_gt_imm_i64_ext
                                        ! -- End function
	.globl	max_gt_imm_u64_ext              ! -- Begin function max_gt_imm_u64_ext
	.p2align	4
	.type	max_gt_imm_u64_ext,@function
max_gt_imm_u64_ext:                     ! @max_gt_imm_u64_ext
.Lmax_gt_imm_u64_ext$local:
	.type	.Lmax_gt_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end810:
	.size	max_gt_imm_u64_ext, .Lfunc_end810-max_gt_imm_u64_ext
	.size	.Lmax_gt_imm_u64_ext$local, .Lfunc_end810-max_gt_imm_u64_ext
                                        ! -- End function
	.globl	max_ge_imm_i64_ext              ! -- Begin function max_ge_imm_i64_ext
	.p2align	4
	.type	max_ge_imm_i64_ext,@function
max_ge_imm_i64_ext:                     ! @max_ge_imm_i64_ext
.Lmax_ge_imm_i64_ext$local:
	.type	.Lmax_ge_imm_i64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_i64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end811:
	.size	max_ge_imm_i64_ext, .Lfunc_end811-max_ge_imm_i64_ext
	.size	.Lmax_ge_imm_i64_ext$local, .Lfunc_end811-max_ge_imm_i64_ext
                                        ! -- End function
	.globl	max_ge_imm_u64_ext              ! -- Begin function max_ge_imm_u64_ext
	.p2align	4
	.type	max_ge_imm_u64_ext,@function
max_ge_imm_u64_ext:                     ! @max_ge_imm_u64_ext
.Lmax_ge_imm_u64_ext$local:
	.type	.Lmax_ge_imm_u64_ext$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 2
	max_imm_u64.l %r1, %r1, 1000000000

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end812:
	.size	max_ge_imm_u64_ext, .Lfunc_end812-max_ge_imm_u64_ext
	.size	.Lmax_ge_imm_u64_ext$local, .Lfunc_end812-max_ge_imm_u64_ext
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
	alloc 11
	mov2 %r7, %r8, %r3, %r4
	mov2 %r9, %r10, %r2, %r1

	! BUNDLE
	call %r6, %jpc(test_mov2_callback)
	add_i32 %r5, %r7, %r1
	mov2 %r7, %r8, %r3, %r4

	! BUNDLE
	mov2 %r9, %r10, %r2, %gz
	call %r6, %jpc(test_mov2_callback)
	add_i32 %r2, %r5, %r7

	! BUNDLE
	mov2 %r7, %r8, %r3, %r4
	mov2 %r9, %r10, %gz, %gz
	call %r6, %jpc(test_mov2_callback)

	! BUNDLE
	add_i32 %r2, %r2, %r7
	mov2 %r7, %r8, %r3, %gz
	mov2 %r9, %r10, %gz, %gz

	! BUNDLE
	call %r6, %jpc(test_mov2_callback)
	add_i32 %r2, %r2, %r7
	mov2 %r7, %r8, %gz, %gz

	! BUNDLE
	mov2 %r9, %r10, %gz, %gz
	call %r6, %jpc(test_mov2_callback)
	add_i32 %r1, %r2, %r7

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end813:
	.size	test_mov2, .Lfunc_end813-test_mov2
	.size	.Ltest_mov2$local, .Lfunc_end813-test_mov2
                                        ! -- End function
	.globl	test_naked                      ! -- Begin function test_naked
	.p2align	4
	.type	test_naked,@function
test_naked:                             ! @test_naked
.Ltest_naked$local:
	.type	.Ltest_naked$local,@function
! %bb.0:                                ! %entry
.Lfunc_end814:
	.size	test_naked, .Lfunc_end814-test_naked
	.size	.Ltest_naked$local, .Lfunc_end814-test_naked
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
	alloc 1
	retf 0
	nop 0

.Lfunc_end815:
	.size	test_after_naked, .Lfunc_end815-test_after_naked
	.size	.Ltest_after_naked$local, .Lfunc_end815-test_after_naked
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
	alloc 5
	ld_u64 %r4, %r2, 16
	nul_ge_i64 %r1, %r4, 3, 4

	! BUNDLE
	ld_xi64_u64 %r2, %r2, %r1, 3, 8
	mul_imm_i64 %r4, %r2, 5
	ld_imm %r2, 2000

	! BUNDLE
	ld_xi64_u64 %r4, %r3, %r1, 3, 16
	mul_imm_i64 %r4, %r4, 3
	st_xi64_i64 %gz, %r2, %r1, 3, 0

	! BUNDLE
	ld_imm %r2, 3000
	ld_xi64_u64 %r3, %r3, %r1, 3, 40
	add_i64 %r1, %r1, %r2

	! BUNDLE
	add_add_i64 %r1, %r1, %r4, %r3
	retf 0
	nop 0

.Lfunc_end816:
	.size	test_nullification_i64_le, .Lfunc_end816-test_nullification_i64_le
	.size	.Ltest_nullification_i64_le$local, .Lfunc_end816-test_nullification_i64_le
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
	alloc 5
	ld_u64 %r4, %r2, 16
	nul_lt_i64 %r4, %r1, 3, 4

	! BUNDLE
	ld_xi64_u64 %r2, %r2, %r1, 3, 8
	mul_imm_i64 %r4, %r2, 5
	ld_imm %r2, 2000

	! BUNDLE
	ld_xi64_u64 %r4, %r3, %r1, 3, 16
	mul_imm_i64 %r4, %r4, 3
	st_xi64_i64 %gz, %r2, %r1, 3, 0

	! BUNDLE
	ld_imm %r2, 3000
	ld_xi64_u64 %r3, %r3, %r1, 3, 40
	add_i64 %r1, %r1, %r2

	! BUNDLE
	add_add_i64 %r1, %r1, %r4, %r3
	retf 0
	nop 0

.Lfunc_end817:
	.size	test_nullification_i64_lt, .Lfunc_end817-test_nullification_i64_lt
	.size	.Ltest_nullification_i64_lt$local, .Lfunc_end817-test_nullification_i64_lt
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
	alloc 4
	nul_ge_u64 %r1, %r2, 2, 3
	ld_iprel_u64 %r2, %pcfwd(left)

	! BUNDLE
	add_i64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right)
	add_i64 %r1, %r2, %r1

	! BUNDLE
	st_xi64_i64 %gz, %r3, %r1, 3, 16
	ld_u64 %r2, %r3, 80
	add_i64 %r1, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end818:
	.size	test_nulu_le_d, .Lfunc_end818-test_nulu_le_d
	.size	.Ltest_nulu_le_d$local, .Lfunc_end818-test_nulu_le_d
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
	alloc 4
	nul_bc_imm %r1, 0, 2, 3
	ld_iprel_u64 %r2, %pcfwd(left_bool)

	! BUNDLE
	add_i64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_bool)
	add_i64 %r1, %r2, %r1

	! BUNDLE
	st_xi64_i64 %gz, %r3, %r1, 3, 16
	ld_u64 %r2, %r3, 80
	add_i64 %r1, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end819:
	.size	test_nullify_bool, .Lfunc_end819-test_nullify_bool
	.size	.Ltest_nullify_bool$local, .Lfunc_end819-test_nullify_bool
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
	alloc 4
	nul_bs_imm %r1, 0, 2, 3
	ld_iprel_u64 %r2, %pcfwd(left_bool)

	! BUNDLE
	add_i64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_bool)
	add_i64 %r1, %r2, %r1

	! BUNDLE
	st_xi64_i64 %gz, %r3, %r1, 3, 16
	ld_u64 %r2, %r3, 80
	add_i64 %r1, %r2, %r1

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end820:
	.size	test_nullify_bool_not, .Lfunc_end820-test_nullify_bool_not
	.size	.Ltest_nullify_bool_not$local, .Lfunc_end820-test_nullify_bool_not
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
	alloc 4
	br_bc_imm %r1, 0, .LBB821_3
	br_bc_imm %r2, 0, .LBB821_3

	! BUNDLE
	ld_iprel_u64 %r2, %pcfwd(left_bool)
	add_i64 %r1, %r2, %r1
	jmp .LBB821_4

.LBB821_3:                              ! %if.else
                                        ! Label of block must be emitted
	! BUNDLE
	ld_iprel_u64 %r2, %pcfwd(right_bool)
	add_i64 %r1, %r2, %r1
	st_xi64_i64 %gz, %r3, %r1, 3, 16

.LBB821_4:                              ! %if.end
                                        ! Label of block must be emitted
	! BUNDLE
	ld_u64 %r2, %r3, 80
	add_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end821:
	.size	test_nullify_bool_and, .Lfunc_end821-test_nullify_bool_and
	.size	.Ltest_nullify_bool_and$local, .Lfunc_end821-test_nullify_bool_and
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
	alloc 4
	br_bs_imm %r1, 0, .LBB822_2
	br_bs_imm %r2, 0, .LBB822_2

	! BUNDLE
	ld_iprel_u64 %r2, %pcfwd(right_bool)
	add_i64 %r1, %r2, %r1
	st_xi64_i64 %gz, %r3, %r1, 3, 16

	! BUNDLE
	jmp .LBB822_4
	nop 0
	nop 0

.LBB822_2:                              ! %if.then
                                        ! Label of block must be emitted
	! BUNDLE
	ld_iprel_u64 %r2, %pcfwd(left_bool)
	add_i64 %r1, %r2, %r1
	nop 0

.LBB822_4:                              ! %if.end
                                        ! Label of block must be emitted
	! BUNDLE
	ld_u64 %r2, %r3, 80
	add_i64 %r1, %r2, %r1
	retf 0

.Lfunc_end822:
	.size	test_nullify_bool_or, .Lfunc_end822-test_nullify_bool_or
	.size	.Ltest_nullify_bool_or$local, .Lfunc_end822-test_nullify_bool_or
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
	alloc 4
	nul_one_f128 %r1, %r2, 2, 4
	ld_iprel_i128 %r2, %pcfwd(left_f128)

	! BUNDLE
	add_f128 %r1, %r2, %r1
	ld_iprel_i128 %r2, %pcfwd(right_f128)
	add_f128 %r1, %r2, %r1

	! BUNDLE
	trunk_f128_i64 %r2, %r1
	st_xi64_i128 %gz, %r3, %r2, 4, 32
	ld_i128 %r2, %r3, 160

	! BUNDLE
	add_f128 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end823:
	.size	test_nullify_f128_eq, .Lfunc_end823-test_nullify_f128_eq
	.size	.Ltest_nullify_f128_eq$local, .Lfunc_end823-test_nullify_f128_eq
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
	alloc 4
	nul_oeq_f128 %r1, %r2, 2, 4
	ld_iprel_i128 %r2, %pcfwd(left_f128)

	! BUNDLE
	add_f128 %r1, %r2, %r1
	ld_iprel_i128 %r2, %pcfwd(right_f128)
	add_f128 %r1, %r2, %r1

	! BUNDLE
	trunk_f128_i64 %r2, %r1
	st_xi64_i128 %gz, %r3, %r2, 4, 32
	ld_i128 %r2, %r3, 160

	! BUNDLE
	add_f128 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end824:
	.size	test_nullify_f128_ne, .Lfunc_end824-test_nullify_f128_ne
	.size	.Ltest_nullify_f128_ne$local, .Lfunc_end824-test_nullify_f128_ne
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
	alloc 4
	nul_oge_f128 %r1, %r2, 2, 4
	ld_iprel_i128 %r2, %pcfwd(left_f128)

	! BUNDLE
	add_f128 %r1, %r2, %r1
	ld_iprel_i128 %r2, %pcfwd(right_f128)
	add_f128 %r1, %r2, %r1

	! BUNDLE
	trunk_f128_i64 %r2, %r1
	st_xi64_i128 %gz, %r3, %r2, 4, 32
	ld_i128 %r2, %r3, 160

	! BUNDLE
	add_f128 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end825:
	.size	test_nullify_f128_lt, .Lfunc_end825-test_nullify_f128_lt
	.size	.Ltest_nullify_f128_lt$local, .Lfunc_end825-test_nullify_f128_lt
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
	alloc 4
	nul_olt_f128 %r2, %r1, 2, 4
	ld_iprel_i128 %r2, %pcfwd(left_f128)

	! BUNDLE
	add_f128 %r1, %r2, %r1
	ld_iprel_i128 %r2, %pcfwd(right_f128)
	add_f128 %r1, %r2, %r1

	! BUNDLE
	trunk_f128_i64 %r2, %r1
	st_xi64_i128 %gz, %r3, %r2, 4, 32
	ld_i128 %r2, %r3, 160

	! BUNDLE
	add_f128 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end826:
	.size	test_nullify_f128_le, .Lfunc_end826-test_nullify_f128_le
	.size	.Ltest_nullify_f128_le$local, .Lfunc_end826-test_nullify_f128_le
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
	alloc 4
	nul_oge_f128 %r2, %r1, 2, 4
	ld_iprel_i128 %r2, %pcfwd(left_f128)

	! BUNDLE
	add_f128 %r1, %r2, %r1
	ld_iprel_i128 %r2, %pcfwd(right_f128)
	add_f128 %r1, %r2, %r1

	! BUNDLE
	trunk_f128_i64 %r2, %r1
	st_xi64_i128 %gz, %r3, %r2, 4, 32
	ld_i128 %r2, %r3, 160

	! BUNDLE
	add_f128 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end827:
	.size	test_nullify_f128_gt, .Lfunc_end827-test_nullify_f128_gt
	.size	.Ltest_nullify_f128_gt$local, .Lfunc_end827-test_nullify_f128_gt
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
	alloc 4
	nul_olt_f128 %r1, %r2, 2, 4
	ld_iprel_i128 %r2, %pcfwd(left_f128)

	! BUNDLE
	add_f128 %r1, %r2, %r1
	ld_iprel_i128 %r2, %pcfwd(right_f128)
	add_f128 %r1, %r2, %r1

	! BUNDLE
	trunk_f128_i64 %r2, %r1
	st_xi64_i128 %gz, %r3, %r2, 4, 32
	ld_i128 %r2, %r3, 160

	! BUNDLE
	add_f128 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end828:
	.size	test_nullify_f128_ge, .Lfunc_end828-test_nullify_f128_ge
	.size	.Ltest_nullify_f128_ge$local, .Lfunc_end828-test_nullify_f128_ge
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
	alloc 4
	nul_one_f32 %r1, %r2, 2, 4
	ld_iprel_u32 %r2, %pcfwd(left_f32)

	! BUNDLE
	add_f32 %r1, %r2, %r1
	ld_iprel_u32 %r2, %pcfwd(right_f32)
	add_f32 %r1, %r2, %r1

	! BUNDLE
	trunk_f32_i64 %r2, %r1
	st_xi64_i32 %gz, %r3, %r2, 2, 8
	ld_u32 %r2, %r3, 40

	! BUNDLE
	add_f32 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end829:
	.size	test_nullify_f32_eq, .Lfunc_end829-test_nullify_f32_eq
	.size	.Ltest_nullify_f32_eq$local, .Lfunc_end829-test_nullify_f32_eq
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
	alloc 4
	nul_oeq_f32 %r1, %r2, 2, 4
	ld_iprel_u32 %r2, %pcfwd(left_f32)

	! BUNDLE
	add_f32 %r1, %r2, %r1
	ld_iprel_u32 %r2, %pcfwd(right_f32)
	add_f32 %r1, %r2, %r1

	! BUNDLE
	trunk_f32_i64 %r2, %r1
	st_xi64_i32 %gz, %r3, %r2, 2, 8
	ld_u32 %r2, %r3, 40

	! BUNDLE
	add_f32 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end830:
	.size	test_nullify_f32_ne, .Lfunc_end830-test_nullify_f32_ne
	.size	.Ltest_nullify_f32_ne$local, .Lfunc_end830-test_nullify_f32_ne
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
	alloc 4
	nul_oge_f32 %r1, %r2, 2, 4
	ld_iprel_u32 %r2, %pcfwd(left_f32)

	! BUNDLE
	add_f32 %r1, %r2, %r1
	ld_iprel_u32 %r2, %pcfwd(right_f32)
	add_f32 %r1, %r2, %r1

	! BUNDLE
	trunk_f32_i64 %r2, %r1
	st_xi64_i32 %gz, %r3, %r2, 2, 8
	ld_u32 %r2, %r3, 40

	! BUNDLE
	add_f32 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end831:
	.size	test_nullify_f32_lt, .Lfunc_end831-test_nullify_f32_lt
	.size	.Ltest_nullify_f32_lt$local, .Lfunc_end831-test_nullify_f32_lt
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
	alloc 4
	nul_olt_f32 %r2, %r1, 2, 4
	ld_iprel_u32 %r2, %pcfwd(left_f32)

	! BUNDLE
	add_f32 %r1, %r2, %r1
	ld_iprel_u32 %r2, %pcfwd(right_f32)
	add_f32 %r1, %r2, %r1

	! BUNDLE
	trunk_f32_i64 %r2, %r1
	st_xi64_i32 %gz, %r3, %r2, 2, 8
	ld_u32 %r2, %r3, 40

	! BUNDLE
	add_f32 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end832:
	.size	test_nullify_f32_le, .Lfunc_end832-test_nullify_f32_le
	.size	.Ltest_nullify_f32_le$local, .Lfunc_end832-test_nullify_f32_le
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
	alloc 4
	nul_oge_f32 %r2, %r1, 2, 4
	ld_iprel_u32 %r2, %pcfwd(left_f32)

	! BUNDLE
	add_f32 %r1, %r2, %r1
	ld_iprel_u32 %r2, %pcfwd(right_f32)
	add_f32 %r1, %r2, %r1

	! BUNDLE
	trunk_f32_i64 %r2, %r1
	st_xi64_i32 %gz, %r3, %r2, 2, 8
	ld_u32 %r2, %r3, 40

	! BUNDLE
	add_f32 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end833:
	.size	test_nullify_f32_gt, .Lfunc_end833-test_nullify_f32_gt
	.size	.Ltest_nullify_f32_gt$local, .Lfunc_end833-test_nullify_f32_gt
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
	alloc 4
	nul_olt_f32 %r1, %r2, 2, 4
	ld_iprel_u32 %r2, %pcfwd(left_f32)

	! BUNDLE
	add_f32 %r1, %r2, %r1
	ld_iprel_u32 %r2, %pcfwd(right_f32)
	add_f32 %r1, %r2, %r1

	! BUNDLE
	trunk_f32_i64 %r2, %r1
	st_xi64_i32 %gz, %r3, %r2, 2, 8
	ld_u32 %r2, %r3, 40

	! BUNDLE
	add_f32 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end834:
	.size	test_nullify_f32_ge, .Lfunc_end834-test_nullify_f32_ge
	.size	.Ltest_nullify_f32_ge$local, .Lfunc_end834-test_nullify_f32_ge
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
	alloc 4
	nul_one_f64 %r1, %r2, 2, 4
	ld_iprel_u64 %r2, %pcfwd(left_f64)

	! BUNDLE
	add_f64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_f64)
	add_f64 %r1, %r2, %r1

	! BUNDLE
	trunk_f64_i64 %r2, %r1
	st_xi64_i64 %gz, %r3, %r2, 3, 16
	ld_u64 %r2, %r3, 80

	! BUNDLE
	add_f64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end835:
	.size	test_nullify_f64_eq, .Lfunc_end835-test_nullify_f64_eq
	.size	.Ltest_nullify_f64_eq$local, .Lfunc_end835-test_nullify_f64_eq
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
	alloc 4
	nul_oeq_f64 %r1, %r2, 2, 4
	ld_iprel_u64 %r2, %pcfwd(left_f64)

	! BUNDLE
	add_f64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_f64)
	add_f64 %r1, %r2, %r1

	! BUNDLE
	trunk_f64_i64 %r2, %r1
	st_xi64_i64 %gz, %r3, %r2, 3, 16
	ld_u64 %r2, %r3, 80

	! BUNDLE
	add_f64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end836:
	.size	test_nullify_f64_ne, .Lfunc_end836-test_nullify_f64_ne
	.size	.Ltest_nullify_f64_ne$local, .Lfunc_end836-test_nullify_f64_ne
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
	alloc 4
	nul_oge_f64 %r1, %r2, 2, 4
	ld_iprel_u64 %r2, %pcfwd(left_f64)

	! BUNDLE
	add_f64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_f64)
	add_f64 %r1, %r2, %r1

	! BUNDLE
	trunk_f64_i64 %r2, %r1
	st_xi64_i64 %gz, %r3, %r2, 3, 16
	ld_u64 %r2, %r3, 80

	! BUNDLE
	add_f64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end837:
	.size	test_nullify_f64_lt, .Lfunc_end837-test_nullify_f64_lt
	.size	.Ltest_nullify_f64_lt$local, .Lfunc_end837-test_nullify_f64_lt
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
	alloc 4
	nul_olt_f64 %r2, %r1, 2, 4
	ld_iprel_u64 %r2, %pcfwd(left_f64)

	! BUNDLE
	add_f64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_f64)
	add_f64 %r1, %r2, %r1

	! BUNDLE
	trunk_f64_i64 %r2, %r1
	st_xi64_i64 %gz, %r3, %r2, 3, 16
	ld_u64 %r2, %r3, 80

	! BUNDLE
	add_f64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end838:
	.size	test_nullify_f64_le, .Lfunc_end838-test_nullify_f64_le
	.size	.Ltest_nullify_f64_le$local, .Lfunc_end838-test_nullify_f64_le
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
	alloc 4
	nul_oge_f64 %r2, %r1, 2, 4
	ld_iprel_u64 %r2, %pcfwd(left_f64)

	! BUNDLE
	add_f64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_f64)
	add_f64 %r1, %r2, %r1

	! BUNDLE
	trunk_f64_i64 %r2, %r1
	st_xi64_i64 %gz, %r3, %r2, 3, 16
	ld_u64 %r2, %r3, 80

	! BUNDLE
	add_f64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end839:
	.size	test_nullify_f64_gt, .Lfunc_end839-test_nullify_f64_gt
	.size	.Ltest_nullify_f64_gt$local, .Lfunc_end839-test_nullify_f64_gt
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
	alloc 4
	nul_olt_f64 %r1, %r2, 2, 4
	ld_iprel_u64 %r2, %pcfwd(left_f64)

	! BUNDLE
	add_f64 %r1, %r2, %r1
	ld_iprel_u64 %r2, %pcfwd(right_f64)
	add_f64 %r1, %r2, %r1

	! BUNDLE
	trunk_f64_i64 %r2, %r1
	st_xi64_i64 %gz, %r3, %r2, 3, 16
	ld_u64 %r2, %r3, 80

	! BUNDLE
	add_f64 %r1, %r2, %r1
	retf 0
	nop 0

.Lfunc_end840:
	.size	test_nullify_f64_ge, .Lfunc_end840-test_nullify_f64_ge
	.size	.Ltest_nullify_f64_ge$local, .Lfunc_end840-test_nullify_f64_ge
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
	alloc 7
	mov %r4, %gz
	br_lt_imm_i64 %r1, 1, .LBB841_2

.LBB841_1:                              ! %for.body
                                        ! =>This Inner Loop Header: Depth=1
                                        ! Label of block must be emitted
	! BUNDLE
	ld_u64 %r5, %r2, 0
	ld_u64 %r6, %r3, 0
	mul_add %r4, %r6, %r5, %r4

	! BUNDLE
	add_imm_i64 %r1, %r1, -1
	add_imm_i64 %r3, %r3, 8
	add_imm_i64 %r2, %r2, 8

	! BUNDLE
	br_ne_imm_i64 %r1, 0, .LBB841_1
	nop 0
	nop 0

.LBB841_2:                              ! %for.cond.cleanup
                                        ! Label of block must be emitted
	! BUNDLE
	mov %r1, %r4
	retf 0
	nop 0

.Lfunc_end841:
	.size	test_post_update, .Lfunc_end841-test_post_update
	.size	.Ltest_post_update$local, .Lfunc_end841-test_post_update
                                        ! -- End function
	.globl	shift_right_and_load_i32        ! -- Begin function shift_right_and_load_i32
	.p2align	4
	.type	shift_right_and_load_i32,@function
shift_right_and_load_i32:               ! @shift_right_and_load_i32
.Lshift_right_and_load_i32$local:
	.type	.Lshift_right_and_load_i32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	slsrl_imm_u64 %r2, %r2, 41, 57
	ld_xi64_u8 %r1, %r1, %r2, 0, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end842:
	.size	shift_right_and_load_i32, .Lfunc_end842-shift_right_and_load_i32
	.size	.Lshift_right_and_load_i32$local, .Lfunc_end842-shift_right_and_load_i32
                                        ! -- End function
	.globl	shift_right_and_load_u32        ! -- Begin function shift_right_and_load_u32
	.p2align	4
	.type	shift_right_and_load_u32,@function
shift_right_and_load_u32:               ! @shift_right_and_load_u32
.Lshift_right_and_load_u32$local:
	.type	.Lshift_right_and_load_u32$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	slsrl_imm_u64 %r2, %r2, 41, 57
	ld_xi64_u8 %r1, %r1, %r2, 0, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end843:
	.size	shift_right_and_load_u32, .Lfunc_end843-shift_right_and_load_u32
	.size	.Lshift_right_and_load_u32$local, .Lfunc_end843-shift_right_and_load_u32
                                        ! -- End function
	.globl	shift_right_and_load_i64        ! -- Begin function shift_right_and_load_i64
	.p2align	4
	.type	shift_right_and_load_i64,@function
shift_right_and_load_i64:               ! @shift_right_and_load_i64
.Lshift_right_and_load_i64$local:
	.type	.Lshift_right_and_load_i64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	slsrl_imm_u64 %r2, %r2, 41, 57
	ld_xi64_u8 %r1, %r1, %r2, 0, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end844:
	.size	shift_right_and_load_i64, .Lfunc_end844-shift_right_and_load_i64
	.size	.Lshift_right_and_load_i64$local, .Lfunc_end844-shift_right_and_load_i64
                                        ! -- End function
	.globl	shift_right_and_load_u64        ! -- Begin function shift_right_and_load_u64
	.p2align	4
	.type	shift_right_and_load_u64,@function
shift_right_and_load_u64:               ! @shift_right_and_load_u64
.Lshift_right_and_load_u64$local:
	.type	.Lshift_right_and_load_u64$local,@function
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 3
	slsrl_imm_u64 %r2, %r2, 41, 57
	ld_xi64_u8 %r1, %r1, %r2, 0, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end845:
	.size	shift_right_and_load_u64, .Lfunc_end845-shift_right_and_load_u64
	.size	.Lshift_right_and_load_u64$local, .Lfunc_end845-shift_right_and_load_u64
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
	alloc 2
	sra_imm_i32 %r1, %r1, 0
	retf 0

.Lfunc_end846:
	.size	test_sign_extend_32, .Lfunc_end846-test_sign_extend_32
	.size	.Ltest_sign_extend_32$local, .Lfunc_end846-test_sign_extend_32
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
	alloc 2
	slsra_imm_i64 %r1, %r1, 48, 48
	retf 0

.Lfunc_end847:
	.size	test_sign_extend_16, .Lfunc_end847-test_sign_extend_16
	.size	.Ltest_sign_extend_16$local, .Lfunc_end847-test_sign_extend_16
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
	alloc 2
	slsra_imm_i64 %r1, %r1, 56, 56
	retf 0

.Lfunc_end848:
	.size	test_sign_extend_8, .Lfunc_end848-test_sign_extend_8
	.size	.Ltest_sign_extend_8$local, .Lfunc_end848-test_sign_extend_8
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
	alloc 2
	srl_imm_u32 %r1, %r1, 0
	retf 0

.Lfunc_end849:
	.size	test_zero_extend_32, .Lfunc_end849-test_zero_extend_32
	.size	.Ltest_zero_extend_32$local, .Lfunc_end849-test_zero_extend_32
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
	alloc 2
	and_imm %r1, %r1, 65535
	retf 0

.Lfunc_end850:
	.size	test_zero_extend_16, .Lfunc_end850-test_zero_extend_16
	.size	.Ltest_zero_extend_16$local, .Lfunc_end850-test_zero_extend_16
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
	alloc 2
	and_imm %r1, %r1, 255
	retf 0

.Lfunc_end851:
	.size	test_zero_extend_8, .Lfunc_end851-test_zero_extend_8
	.size	.Ltest_zero_extend_8$local, .Lfunc_end851-test_zero_extend_8
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
	alloc 3
	st_i32 %r2, %r1, 16000
	retf 0

.Lfunc_end852:
	.size	test_store_based_u32, .Lfunc_end852-test_store_based_u32
	.size	.Ltest_store_based_u32$local, .Lfunc_end852-test_store_based_u32
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
	alloc 3
	st_i16 %r2, %r1, 8000
	retf 0

.Lfunc_end853:
	.size	test_store_based_u16, .Lfunc_end853-test_store_based_u16
	.size	.Ltest_store_based_u16$local, .Lfunc_end853-test_store_based_u16
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
	alloc 3
	st_i8 %r2, %r1, 4000
	retf 0

.Lfunc_end854:
	.size	test_store_based_u8, .Lfunc_end854-test_store_based_u8
	.size	.Ltest_store_based_u8$local, .Lfunc_end854-test_store_based_u8
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
	alloc 3
	st_i32 %r2, %r1, 16000
	retf 0

.Lfunc_end855:
	.size	test_store_based_i32, .Lfunc_end855-test_store_based_i32
	.size	.Ltest_store_based_i32$local, .Lfunc_end855-test_store_based_i32
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
	alloc 3
	st_i16 %r2, %r1, 8000
	retf 0

.Lfunc_end856:
	.size	test_store_based_i16, .Lfunc_end856-test_store_based_i16
	.size	.Ltest_store_based_i16$local, .Lfunc_end856-test_store_based_i16
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
	alloc 3
	st_i8 %r2, %r1, 4000
	retf 0

.Lfunc_end857:
	.size	test_store_based_i8, .Lfunc_end857-test_store_based_i8
	.size	.Ltest_store_based_i8$local, .Lfunc_end857-test_store_based_i8
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
	alloc 5
	div_u32 %r2, %r2, %r3
	st_xu32_i64 %r4, %r1, %r2, 3, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end858:
	.size	test_store_indexed_u32_u32, .Lfunc_end858-test_store_indexed_u32_u32
	.size	.Ltest_store_indexed_u32_u32$local, .Lfunc_end858-test_store_indexed_u32_u32
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
	alloc 4
	st_xi32_i64 %r3, %r1, %r2, 3, 0
	retf 0

.Lfunc_end859:
	.size	test_store_indexed_i32, .Lfunc_end859-test_store_indexed_i32
	.size	.Ltest_store_indexed_i32$local, .Lfunc_end859-test_store_indexed_i32
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
	alloc 4
	st_xi32_i64 %r3, %r1, %r2, 3, 40
	retf 0

.Lfunc_end860:
	.size	test_store_indexed_i32_offset, .Lfunc_end860-test_store_indexed_i32_offset
	.size	.Ltest_store_indexed_i32_offset$local, .Lfunc_end860-test_store_indexed_i32_offset
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
	alloc 4
	st_xu32_i64 %r3, %r1, %r2, 3, 0
	retf 0

.Lfunc_end861:
	.size	test_store_indexed_u32, .Lfunc_end861-test_store_indexed_u32
	.size	.Ltest_store_indexed_u32$local, .Lfunc_end861-test_store_indexed_u32
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
	alloc 4
	add_imm_i32 %r2, %r2, 20
	st_xu32_i64 %r3, %r1, %r2, 3, 0

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end862:
	.size	test_store_indexed_u32_offset, .Lfunc_end862-test_store_indexed_u32_offset
	.size	.Ltest_store_indexed_u32_offset$local, .Lfunc_end862-test_store_indexed_u32_offset
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
	alloc 4
	st_xi64_i64 %r3, %r1, %r2, 3, 40
	retf 0

.Lfunc_end863:
	.size	test_store_indexed_i64, .Lfunc_end863-test_store_indexed_i64
	.size	.Ltest_store_indexed_i64$local, .Lfunc_end863-test_store_indexed_i64
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
	alloc 4
	st_xi32_i64.l %r3, %r1, %r2, 4, 328

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end864:
	.size	test_store_indexed_i32_struct, .Lfunc_end864-test_store_indexed_i32_struct
	.size	.Ltest_store_indexed_i32_struct$local, .Lfunc_end864-test_store_indexed_i32_struct
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
	alloc 12
	ld_iprel_u8 %r1, %pcfwd(i8_zero)
	ld_iprel_u8 %r2, %pcfwd(u8_zero)

	! BUNDLE
	add_imm_i32 %r1, %r1, 1
	st_iprel_i8 %r1, %pcfwd(i8_zero)
	ld_iprel_u16 %r3, %pcfwd(i16_zero)

	! BUNDLE
	add_imm_i32 %r2, %r2, 1
	st_iprel_i8 %r2, %pcfwd(u8_zero)
	ld_iprel_u16 %r4, %pcfwd(u16_zero)

	! BUNDLE
	add_imm_i32 %r3, %r3, 1
	st_iprel_i16 %r3, %pcfwd(i16_zero)
	ld_iprel_u32 %r5, %pcfwd(i32_zero)

	! BUNDLE
	add_imm_i32 %r4, %r4, 1
	st_iprel_i16 %r4, %pcfwd(u16_zero)
	ld_iprel_u32 %r6, %pcfwd(u32_zero)

	! BUNDLE
	add_imm_i32 %r5, %r5, 1
	st_iprel_i32 %r5, %pcfwd(i32_zero)
	ld_iprel_u64 %r7, %pcfwd(i64_zero)

	! BUNDLE
	add_imm_i32 %r6, %r6, 1
	st_iprel_i32 %r6, %pcfwd(u32_zero)
	ld_iprel_u64 %r8, %pcfwd(u64_zero)

	! BUNDLE
	add_imm_i64 %r9, %r7, 1
	st_iprel_i64 %r9, %pcfwd(i64_zero)
	ld_iprel_i128 %r9, %pcfwd(i128_zero)

	! BUNDLE
	add_imm_i64 %r10, %r8, 1
	ld_iprel_i128 %r11, %pcfwd(u128_zero)
	st_iprel_i64 %r10, %pcfwd(u64_zero)

	! BUNDLE
	bit_flip_imm %r9, %r9, 0
	st_iprel_i128 %r9, %pcfwd(i128_zero)
	bit_flip_imm %r10, %r11, 0

	! BUNDLE
	st_iprel_i128 %r10, %pcfwd(u128_zero)
	slsra_imm_i64 %r1, %r1, 56, 56
	and_imm %r2, %r2, 255

	! BUNDLE
	add_i32 %r1, %r2, %r1
	slsra_imm_i64 %r2, %r3, 48, 48
	add_i32 %r1, %r1, %r2

	! BUNDLE
	and_imm %r2, %r4, 65535
	add_i32 %r1, %r1, %r2
	add_i32 %r1, %r1, %r5

	! BUNDLE
	add_i32 %r1, %r1, %r6
	lda_xu32 %r1, %r7, %r1, 0, 1
	lda_xi64 %r1, %r8, %r1, 0, 1

	! BUNDLE
	xor %r1, %r9, %r1
	xor %r1, %r1, %r10
	retf 0

.Lfunc_end865:
	.size	dummy_return, .Lfunc_end865-dummy_return
	.size	.Ldummy_return$local, .Lfunc_end865-dummy_return
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
	alloc 4
	st_i8 %gz, %r1, 4000
	st_iprel_i8 %gz, %pcfwd(i8_zero)

	! BUNDLE
	st_xi64_i8 %gz, %r1, %r2, 0, 10
	st_xi32_i8 %gz, %r1, %r3, 0, 10
	retf 0

.Lfunc_end866:
	.size	test_store_based_i8_zero, .Lfunc_end866-test_store_based_i8_zero
	.size	.Ltest_store_based_i8_zero$local, .Lfunc_end866-test_store_based_i8_zero
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
	alloc 4
	st_i8 %gz, %r1, 4000
	st_iprel_i8 %gz, %pcfwd(u8_zero)

	! BUNDLE
	st_xi64_i8 %gz, %r1, %r2, 0, 10
	st_xi32_i8 %gz, %r1, %r3, 0, 10
	retf 0

.Lfunc_end867:
	.size	test_store_based_u8_zero, .Lfunc_end867-test_store_based_u8_zero
	.size	.Ltest_store_based_u8_zero$local, .Lfunc_end867-test_store_based_u8_zero
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
	alloc 4
	st_i16 %gz, %r1, 8000
	st_iprel_i16 %gz, %pcfwd(i16_zero)

	! BUNDLE
	st_xi64_i16 %gz, %r1, %r2, 1, 20
	st_xi32_i16 %gz, %r1, %r3, 1, 20
	retf 0

.Lfunc_end868:
	.size	test_store_based_i16_zero, .Lfunc_end868-test_store_based_i16_zero
	.size	.Ltest_store_based_i16_zero$local, .Lfunc_end868-test_store_based_i16_zero
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
	alloc 4
	st_i16 %gz, %r1, 8000
	st_iprel_i16 %gz, %pcfwd(u16_zero)

	! BUNDLE
	st_xi64_i16 %gz, %r1, %r2, 1, 20
	st_xi32_i16 %gz, %r1, %r3, 1, 20
	retf 0

.Lfunc_end869:
	.size	test_store_based_u16_zero, .Lfunc_end869-test_store_based_u16_zero
	.size	.Ltest_store_based_u16_zero$local, .Lfunc_end869-test_store_based_u16_zero
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
	alloc 4
	st_i32 %gz, %r1, 16000
	st_iprel_i32 %gz, %pcfwd(i32_zero)

	! BUNDLE
	st_xi64_i32 %gz, %r1, %r2, 2, 40
	st_xi32_i32 %gz, %r1, %r3, 2, 40
	retf 0

.Lfunc_end870:
	.size	test_store_based_i32_zero, .Lfunc_end870-test_store_based_i32_zero
	.size	.Ltest_store_based_i32_zero$local, .Lfunc_end870-test_store_based_i32_zero
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
	alloc 4
	st_i32 %gz, %r1, 16000
	st_iprel_i32 %gz, %pcfwd(u32_zero)

	! BUNDLE
	st_xi64_i32 %gz, %r1, %r2, 2, 40
	st_xi32_i32 %gz, %r1, %r3, 2, 40
	retf 0

.Lfunc_end871:
	.size	test_store_based_u32_zero, .Lfunc_end871-test_store_based_u32_zero
	.size	.Ltest_store_based_u32_zero$local, .Lfunc_end871-test_store_based_u32_zero
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
	alloc 4
	st_i64 %gz, %r1, 32000
	st_iprel_i64 %gz, %pcfwd(i64_zero)

	! BUNDLE
	st_xi64_i64 %gz, %r1, %r2, 3, 40
	st_xi32_i64 %gz, %r1, %r3, 3, 40
	retf 0

.Lfunc_end872:
	.size	test_store_based_i64_zero, .Lfunc_end872-test_store_based_i64_zero
	.size	.Ltest_store_based_i64_zero$local, .Lfunc_end872-test_store_based_i64_zero
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
	alloc 4
	st_i64 %gz, %r1, 32000
	st_iprel_i64 %gz, %pcfwd(u64_zero)

	! BUNDLE
	st_xi64_i64 %gz, %r1, %r2, 3, 40
	st_xi32_i64 %gz, %r1, %r3, 3, 40
	retf 0

.Lfunc_end873:
	.size	test_store_based_u64_zero, .Lfunc_end873-test_store_based_u64_zero
	.size	.Ltest_store_based_u64_zero$local, .Lfunc_end873-test_store_based_u64_zero
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
	alloc 4
	st_i128 %gz, %r1, 64000
	st_iprel_i128 %gz, %pcfwd(i128_zero)

	! BUNDLE
	st_xi64_i128 %gz, %r1, %r2, 4, 48
	st_xi32_i128 %gz, %r1, %r3, 4, 48
	retf 0

.Lfunc_end874:
	.size	test_store_based_i128_zero, .Lfunc_end874-test_store_based_i128_zero
	.size	.Ltest_store_based_i128_zero$local, .Lfunc_end874-test_store_based_i128_zero
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
	alloc 4
	st_i128 %gz, %r1, 64000
	st_iprel_i128 %gz, %pcfwd(u128_zero)

	! BUNDLE
	st_xi64_i128 %gz, %r1, %r2, 4, 48
	st_xi32_i128 %gz, %r1, %r3, 4, 48
	retf 0

.Lfunc_end875:
	.size	test_store_based_u128_zero, .Lfunc_end875-test_store_based_u128_zero
	.size	.Ltest_store_based_u128_zero$local, .Lfunc_end875-test_store_based_u128_zero
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
	alloc_sp 4, 240
	st_i128 %gz, %sp, 64
	st_i128 %gz, %sp, 80

	! BUNDLE
	st_i128 %gz, %sp, 128
	st_i128 %gz, %sp, 144
	st_i128 %gz, %sp, 208

	! BUNDLE
	st_i64 %gz, %sp, 224
	st_i128 %gz, %sp, 192
	st_i128 %gz, %sp, 176

	! BUNDLE
	st_i128 %gz, %sp, 160
	st_i128 %gz, %sp, 112
	st_i128 %gz, %sp, 96

	! BUNDLE
	st_i128 %gz, %sp, 48
	st_i128 %gz, %sp, 32
	st_i128 %gz, %sp, 16

	! BUNDLE
	st_i128 %gz, %sp, 0
	lda_r %r1, %jpc(sn_write)
	st_i64 %r1, %sp, 72

	! BUNDLE
	add_imm_i64 %r1, %sp, 239
	st_i64 %r1, %sp, 88
	ld_imm %r1, -1

	! BUNDLE
	st_i32 %r1, %sp, 140
	st_i32 %r1, %sp, 144
	add_imm_i64 %r3, %sp, 0

	! BUNDLE
	st_i64 %r3, %sp, 208
	call %r2, %jpc(test_struct_memset_consumer)
	mov %r1, %r3

	! BUNDLE
	retf 240
	nop 0
	nop 0

.Lfunc_end876:
	.size	test_struct_memset, .Lfunc_end876-test_struct_memset
	.size	.Ltest_struct_memset$local, .Lfunc_end876-test_struct_memset
                                        ! -- End function
	.p2align	4                               ! -- Begin function sn_write
	.type	sn_write,@function
sn_write:                               ! @sn_write
	.register %sp, #scratch
! %bb.0:                                ! %entry
	! BUNDLE
	alloc 7
	ld_u64 %r2, %r1, 152
	ld_u64 %r4, %r1, 40

	! BUNDLE
	ld_u64 %r6, %r1, 56
	ld_u64 %r5, %r2, 8
	sub_i64 %r4, %r4, %r6

	! BUNDLE
	min_u64 %r6, %r5, %r4
	ld_u64 %r4, %r2, 0
	nul_eq_imm_i64 %r6, 0, 4, 0

	! BUNDLE
	add_i64 %r4, %r4, %r6
	st_i64 %r4, %r2, 0
	sub_i64 %r5, %r5, %r6

	! BUNDLE
	st_i64 %r5, %r2, 8
	min_u64 %r6, %r5, %r3
	nul_eq_imm_i64 %r6, 0, 4, 0

	! BUNDLE
	add_i64 %r4, %r4, %r6
	st_i64 %r4, %r2, 0
	sub_i64 %r5, %r5, %r6

	! BUNDLE
	st_i64 %r5, %r2, 8
	st_i8 %gz, %r4, 0
	ld_u64 %r2, %r1, 88

	! BUNDLE
	st_i64 %r2, %r1, 56
	st_i64 %r2, %r1, 40
	mov %r1, %r3

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end877:
	.size	sn_write, .Lfunc_end877-sn_write
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
	alloc_sp 5, 16
	br_ge_imm_u64 %r1, 6, .LBB878_9
	lda_iprel %r2, %pcfwd(.LJTI878_0)

	! BUNDLE
	jmp_t %r2, %r1
	nop 0
	nop 0

.LBB878_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 1
	lda_iprel %r4, %pcfwd(.L.str.25)
	jmp .LBB878_8

.LBB878_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 8
	st_i64 %r1, %sp, 8
	ld_imm %r1, 5

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.29)
	jmp .LBB878_8
	nop 0

.LBB878_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 2
	st_i64 %r1, %sp, 8
	ld_imm %r1, 3

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.27)
	jmp .LBB878_8
	nop 0

.LBB878_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 1
	st_i64 %r1, %sp, 8
	ld_imm %r1, 4

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.28)
	jmp .LBB878_8
	nop 0

.LBB878_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	st_i64 %gz, %sp, 8
	ld_imm %r1, 2
	lda_iprel %r4, %pcfwd(.L.str.26)

	! BUNDLE
	jmp .LBB878_8
	nop 0
	nop 0

.LBB878_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 27
	st_i64 %r1, %sp, 8
	ld_imm %r1, 6

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.30)
	nop 0
	nop 0

.LBB878_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st_i64 %r1, %sp, 0
	call %r3, %jpc(printf)
	nop 0

.LBB878_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov %r1, %gz
	retf 16
	nop 0

.Lfunc_end878:
	.size	test_switch_table_i64, .Lfunc_end878-test_switch_table_i64
	.size	.Ltest_switch_table_i64$local, .Lfunc_end878-test_switch_table_i64
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI878_0:
	.word	.LBB878_2-.LJTI878_0
	.word	.LBB878_3-.LJTI878_0
	.word	.LBB878_4-.LJTI878_0
	.word	.LBB878_5-.LJTI878_0
	.word	.LBB878_6-.LJTI878_0
	.word	.LBB878_7-.LJTI878_0
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
	alloc_sp 5, 16
	br_ge_imm_u32 %r1, 6, .LBB879_9
	srl_imm_u32 %r1, %r1, 0

	! BUNDLE
	lda_iprel %r2, %pcfwd(.LJTI879_0)
	jmp_t %r2, %r1
	nop 0

.LBB879_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 1
	lda_iprel %r4, %pcfwd(.L.str.31)
	jmp .LBB879_8

.LBB879_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 8
	st_i32 %r1, %sp, 4
	ld_imm %r1, 5

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.35)
	jmp .LBB879_8
	nop 0

.LBB879_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 2
	st_i32 %r1, %sp, 4
	ld_imm %r1, 3

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.33)
	jmp .LBB879_8
	nop 0

.LBB879_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 1
	st_i32 %r1, %sp, 4
	ld_imm %r1, 4

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.34)
	jmp .LBB879_8
	nop 0

.LBB879_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %gz, %sp, 4
	ld_imm %r1, 2
	lda_iprel %r4, %pcfwd(.L.str.32)

	! BUNDLE
	jmp .LBB879_8
	nop 0
	nop 0

.LBB879_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 27
	st_i32 %r1, %sp, 4
	ld_imm %r1, 6

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.36)
	nop 0
	nop 0

.LBB879_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %r1, %sp, 0
	call %r3, %jpc(printf)
	nop 0

.LBB879_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov %r1, %gz
	retf 16
	nop 0

.Lfunc_end879:
	.size	test_switch_table_i32, .Lfunc_end879-test_switch_table_i32
	.size	.Ltest_switch_table_i32$local, .Lfunc_end879-test_switch_table_i32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI879_0:
	.word	.LBB879_2-.LJTI879_0
	.word	.LBB879_3-.LJTI879_0
	.word	.LBB879_4-.LJTI879_0
	.word	.LBB879_5-.LJTI879_0
	.word	.LBB879_6-.LJTI879_0
	.word	.LBB879_7-.LJTI879_0
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
	alloc_sp 5, 16
	br_ge_imm_u32 %r1, 6, .LBB880_9
	srl_imm_u32 %r1, %r1, 0

	! BUNDLE
	lda_iprel %r2, %pcfwd(.LJTI880_0)
	jmp_t %r2, %r1
	nop 0

.LBB880_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 1
	lda_iprel %r4, %pcfwd(.L.str.37)
	jmp .LBB880_8

.LBB880_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 8
	st_i32 %r1, %sp, 4
	ld_imm %r1, 5

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.41)
	jmp .LBB880_8
	nop 0

.LBB880_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 2
	st_i32 %r1, %sp, 4
	ld_imm %r1, 3

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.39)
	jmp .LBB880_8
	nop 0

.LBB880_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 1
	st_i32 %r1, %sp, 4
	ld_imm %r1, 4

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.40)
	jmp .LBB880_8
	nop 0

.LBB880_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %gz, %sp, 4
	ld_imm %r1, 2
	lda_iprel %r4, %pcfwd(.L.str.38)

	! BUNDLE
	jmp .LBB880_8
	nop 0
	nop 0

.LBB880_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 27
	st_i32 %r1, %sp, 4
	ld_imm %r1, 6

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.42)
	nop 0
	nop 0

.LBB880_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %r1, %sp, 0
	call %r3, %jpc(printf)
	nop 0

.LBB880_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov %r1, %gz
	retf 16
	nop 0

.Lfunc_end880:
	.size	test_switch_table_u32, .Lfunc_end880-test_switch_table_u32
	.size	.Ltest_switch_table_u32$local, .Lfunc_end880-test_switch_table_u32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI880_0:
	.word	.LBB880_2-.LJTI880_0
	.word	.LBB880_3-.LJTI880_0
	.word	.LBB880_4-.LJTI880_0
	.word	.LBB880_5-.LJTI880_0
	.word	.LBB880_6-.LJTI880_0
	.word	.LBB880_7-.LJTI880_0
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
	alloc_sp 5, 16
	add_imm_i64 %r1, %r1, -10
	br_ge_imm_u64 %r1, 6, .LBB881_9

	! BUNDLE
	lda_iprel %r2, %pcfwd(.LJTI881_0)
	jmp_t %r2, %r1
	nop 0

.LBB881_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 11
	lda_iprel %r4, %pcfwd(.L.str.43)
	jmp .LBB881_8

.LBB881_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 28
	st_i64 %r1, %sp, 8
	ld_imm %r1, 15

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.47)
	jmp .LBB881_8
	nop 0

.LBB881_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 14
	st_i64 %r1, %sp, 8
	ld_imm %r1, 13

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.45)
	jmp .LBB881_8
	nop 0

.LBB881_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 15
	st_i64 %r1, %sp, 8
	ld_imm %r1, 14

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.46)
	jmp .LBB881_8
	nop 0

.LBB881_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 10
	st_i64 %r1, %sp, 8
	ld_imm %r1, 12

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.44)
	jmp .LBB881_8
	nop 0

.LBB881_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 37
	st_i64 %r1, %sp, 8
	ld_imm %r1, 16

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.48)
	nop 0
	nop 0

.LBB881_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st_i64 %r1, %sp, 0
	call %r3, %jpc(printf)
	nop 0

.LBB881_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov %r1, %gz
	retf 16
	nop 0

.Lfunc_end881:
	.size	test_switch_table_nonzero_i64, .Lfunc_end881-test_switch_table_nonzero_i64
	.size	.Ltest_switch_table_nonzero_i64$local, .Lfunc_end881-test_switch_table_nonzero_i64
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI881_0:
	.word	.LBB881_2-.LJTI881_0
	.word	.LBB881_3-.LJTI881_0
	.word	.LBB881_4-.LJTI881_0
	.word	.LBB881_5-.LJTI881_0
	.word	.LBB881_6-.LJTI881_0
	.word	.LBB881_7-.LJTI881_0
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
	alloc_sp 5, 16
	add_imm_i32 %r2, %r1, -10
	br_ge_imm_u32 %r2, 6, .LBB882_9

	! BUNDLE
	add_imm_u32 %r1, %r1, -10
	lda_iprel %r2, %pcfwd(.LJTI882_0)
	jmp_t %r2, %r1

.LBB882_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 11
	lda_iprel %r4, %pcfwd(.L.str.49)
	jmp .LBB882_8

.LBB882_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 28
	st_i32 %r1, %sp, 4
	ld_imm %r1, 15

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.53)
	jmp .LBB882_8
	nop 0

.LBB882_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 14
	st_i32 %r1, %sp, 4
	ld_imm %r1, 13

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.51)
	jmp .LBB882_8
	nop 0

.LBB882_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 15
	st_i32 %r1, %sp, 4
	ld_imm %r1, 14

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.52)
	jmp .LBB882_8
	nop 0

.LBB882_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 10
	st_i32 %r1, %sp, 4
	ld_imm %r1, 12

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.50)
	jmp .LBB882_8
	nop 0

.LBB882_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 37
	st_i32 %r1, %sp, 4
	ld_imm %r1, 16

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.54)
	nop 0
	nop 0

.LBB882_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %r1, %sp, 0
	call %r3, %jpc(printf)
	nop 0

.LBB882_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov %r1, %gz
	retf 16
	nop 0

.Lfunc_end882:
	.size	test_switch_table_nonzero_i32, .Lfunc_end882-test_switch_table_nonzero_i32
	.size	.Ltest_switch_table_nonzero_i32$local, .Lfunc_end882-test_switch_table_nonzero_i32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI882_0:
	.word	.LBB882_2-.LJTI882_0
	.word	.LBB882_3-.LJTI882_0
	.word	.LBB882_4-.LJTI882_0
	.word	.LBB882_5-.LJTI882_0
	.word	.LBB882_6-.LJTI882_0
	.word	.LBB882_7-.LJTI882_0
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
	alloc_sp 5, 16
	add_imm_i32 %r2, %r1, -10
	br_ge_imm_u32 %r2, 6, .LBB883_9

	! BUNDLE
	add_imm_u32 %r1, %r1, -10
	lda_iprel %r2, %pcfwd(.LJTI883_0)
	jmp_t %r2, %r1

.LBB883_2:                              ! %sw.bb
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 11
	lda_iprel %r4, %pcfwd(.L.str.55)
	jmp .LBB883_8

.LBB883_6:                              ! %sw.bb10
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 28
	st_i32 %r1, %sp, 4
	ld_imm %r1, 15

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.59)
	jmp .LBB883_8
	nop 0

.LBB883_4:                              ! %sw.bb4
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 14
	st_i32 %r1, %sp, 4
	ld_imm %r1, 13

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.57)
	jmp .LBB883_8
	nop 0

.LBB883_5:                              ! %sw.bb7
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 15
	st_i32 %r1, %sp, 4
	ld_imm %r1, 14

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.58)
	jmp .LBB883_8
	nop 0

.LBB883_3:                              ! %sw.bb1
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 10
	st_i32 %r1, %sp, 4
	ld_imm %r1, 12

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.56)
	jmp .LBB883_8
	nop 0

.LBB883_7:                              ! %sw.bb13
                                        ! Label of block must be emitted
	! BUNDLE
	ld_imm %r1, 37
	st_i32 %r1, %sp, 4
	ld_imm %r1, 16

	! BUNDLE
	lda_iprel %r4, %pcfwd(.L.str.60)
	nop 0
	nop 0

.LBB883_8:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	st_i32 %r1, %sp, 0
	call %r3, %jpc(printf)
	nop 0

.LBB883_9:                              ! %return
                                        ! Label of block must be emitted
	! BUNDLE
	mov %r1, %gz
	retf 16
	nop 0

.Lfunc_end883:
	.size	test_switch_table_nonzero_u32, .Lfunc_end883-test_switch_table_nonzero_u32
	.size	.Ltest_switch_table_nonzero_u32$local, .Lfunc_end883-test_switch_table_nonzero_u32
	.section	.rodata,"a",@progbits
	.p2align	2, 0x0
.LJTI883_0:
	.word	.LBB883_2-.LJTI883_0
	.word	.LBB883_3-.LJTI883_0
	.word	.LBB883_4-.LJTI883_0
	.word	.LBB883_5-.LJTI883_0
	.word	.LBB883_6-.LJTI883_0
	.word	.LBB883_7-.LJTI883_0
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
	alloc 2
	syscall
	retf 0

.Lfunc_end884:
	.size	test_syscall_2, .Lfunc_end884-test_syscall_2
	.size	.Ltest_syscall_2$local, .Lfunc_end884-test_syscall_2
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
	alloc 1
	syscall
	sysret

	! BUNDLE
	retf 0
	nop 0
	nop 0

.Lfunc_end885:
	.size	test_syscall, .Lfunc_end885-test_syscall
	.size	.Ltest_syscall$local, .Lfunc_end885-test_syscall
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
	alloc 2
	int %r1, 0
	int %gz, 100

	! BUNDLE
	int %gz, 200
	int %gz, 255
	int %gz, 99

	! BUNDLE
	int %gz, 1
	int %gz, 5
	nop 0

.Lfunc_end886:
	.size	test_int, .Lfunc_end886-test_int
	.size	.Ltest_int$local, .Lfunc_end886-test_int
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
	alloc 5
	add_imm_i32 %r3, %r1, 1
	add_imm_i32 %r4, %r1, 10

	! BUNDLE
	call %r2, %jpc(tail_call)
	mov %r1, %r3
	retf 0

.Lfunc_end887:
	.size	test_call_global_tailcall, .Lfunc_end887-test_call_global_tailcall
	.size	.Ltest_call_global_tailcall$local, .Lfunc_end887-test_call_global_tailcall
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
	alloc 5
	mov %r4, %r2
	call_mi %r3, %r1, 48

	! BUNDLE
	mov %r1, %r4
	retf 0
	nop 0

.Lfunc_end888:
	.size	test_ptr_tailcall, .Lfunc_end888-test_ptr_tailcall
	.size	.Ltest_ptr_tailcall$local, .Lfunc_end888-test_ptr_tailcall
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
	alloc 4
	mov %r3, %r1
	call_plt %r2, %pcfwd(global_ftable+40)

	! BUNDLE
	mov %r1, %r3
	retf 0
	nop 0

.Lfunc_end889:
	.size	test_call_global_ptr_table_tailcall, .Lfunc_end889-test_call_global_ptr_table_tailcall
	.size	.Ltest_call_global_ptr_table_tailcall$local, .Lfunc_end889-test_call_global_ptr_table_tailcall
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
	alloc 4
	ptc %r1, %r2, %r3
	retf 0

.Lfunc_end890:
	.size	test_builtin_tlb_purge, .Lfunc_end890-test_builtin_tlb_purge
	.size	.Ltest_builtin_tlb_purge$local, .Lfunc_end890-test_builtin_tlb_purge
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
	alloc_sp 5, 16
	lda_iprel %r4, %tld(x_tls)
	call %r3, %jpc(__tls_get_addr)

	! BUNDLE
	ld_u64 %r2, %r4, %dtpoff(x_tls)
	add_i64 %r1, %r2, %r1
	retf 16

.Lfunc_end891:
	.size	test_local_tls, .Lfunc_end891-test_local_tls
	.size	.Ltest_local_tls$local, .Lfunc_end891-test_local_tls
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
	alloc_sp 5, 16
	lda_iprel %r4, %tld(x_tls_extern)
	call %r3, %jpc(__tls_get_addr)

	! BUNDLE
	ld_u64 %r2, %r4, %dtpoff(x_tls_extern)
	add_i64 %r1, %r2, %r1
	retf 16

.Lfunc_end892:
	.size	test_extern_tls, .Lfunc_end892-test_extern_tls
	.size	.Ltest_extern_tls$local, .Lfunc_end892-test_extern_tls
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
	alloc_sp 7, 144
	add_imm_i64 %r6, %sp, 144
	st_i64 %r6, %sp, 8

	! BUNDLE
	add_imm_i32 %r3, %r1, 1
	add_imm_i32 %r4, %r1, -1
	add_imm_i64 %r5, %sp, 16

	! BUNDLE
	call %r2, %jpc(test_va_list_callback)
	mov %r1, %r3
	retf 144

.Lfunc_end893:
	.size	test_va_start, .Lfunc_end893-test_va_start
	.size	.Ltest_va_start$local, .Lfunc_end893-test_va_start
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
	alloc_sp 8, 144
	add_imm_i64 %r1, %sp, 144
	st_i64 %r1, %sp, 8

	! BUNDLE
	nop 1000
	ld_u64 %r1, %sp, 8
	alignup_u64 %r1, %r1, 0, 2

	! BUNDLE
	add_imm_i64 %r2, %r1, 4
	st_i64 %r2, %sp, 8
	ld_u32 %r4, %r1, 0

	! BUNDLE
	nop 1001
	ld_u64 %r1, %sp, 8
	alignup_u64 %r1, %r1, 0, 3

	! BUNDLE
	add_imm_i64 %r2, %r1, 8
	st_i64 %r2, %sp, 8
	ld_u64 %r5, %r1, 0

	! BUNDLE
	nop 1002
	nop 1003
	add_imm_i64 %r6, %sp, 16

	! BUNDLE
	add_imm_i64 %r7, %sp, 8
	call %r3, %jpc(test_va_callback)
	mov %r1, %r4

	! BUNDLE
	retf 144
	nop 0
	nop 0

.Lfunc_end894:
	.size	test_va_start_end, .Lfunc_end894-test_va_start_end
	.size	.Ltest_va_start_end$local, .Lfunc_end894-test_va_start_end
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
	alloc_sp 6, 16
	st_i64 %r2, %sp, 8
	st_i64 %r2, %sp, 0

	! BUNDLE
	ld_imm %r4, 100
	add_imm_i64 %r1, %sp, 0
	mov %r5, %r1

	! BUNDLE
	call %r3, %jpc(test_printf_callback)
	ld_imm %r4, 200
	mov %r5, %r1

	! BUNDLE
	call %r3, %jpc(test_printf_callback)
	mov %r1, %r4
	retf 16

.Lfunc_end895:
	.size	test_printf_valist, .Lfunc_end895-test_printf_valist
	.size	.Ltest_printf_valist$local, .Lfunc_end895-test_printf_valist
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
	alloc_sp 6, 32
	st_i64 %r2, %sp, 24
	st_i64 %r2, %sp, 16

	! BUNDLE
	st_i64 %r2, %sp, 8
	ld_imm %r4, 100
	add_imm_i64 %r5, %sp, 16

	! BUNDLE
	call %r3, %jpc(test_vprintf_callback)
	ld_imm %r4, 200
	add_imm_i64 %r5, %sp, 8

	! BUNDLE
	call %r3, %jpc(test_vprintf_callback)
	mov %r1, %r4
	retf 32

.Lfunc_end896:
	.size	test_vprintf_valist, .Lfunc_end896-test_vprintf_valist
	.size	.Ltest_vprintf_valist$local, .Lfunc_end896-test_vprintf_valist
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
	alloc_sp 5, 16
	st_i64 %r2, %sp, 8
	st_i64 %r2, %sp, 0

	! BUNDLE
	add_imm_i64 %r1, %sp, 0
	mov %r4, %r1
	call %r3, %jpc(test_valist_callback)

	! BUNDLE
	mov %r4, %r1
	call %r3, %jpc(test_valist_callback)
	mov %r1, %r4

	! BUNDLE
	retf 16
	nop 0
	nop 0

.Lfunc_end897:
	.size	test_vacopy, .Lfunc_end897-test_vacopy
	.size	.Ltest_vacopy$local, .Lfunc_end897-test_vacopy
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end898:
	.size	test_vector_bitcast_v2i64_to_v4i32, .Lfunc_end898-test_vector_bitcast_v2i64_to_v4i32
	.size	.Ltest_vector_bitcast_v2i64_to_v4i32$local, .Lfunc_end898-test_vector_bitcast_v2i64_to_v4i32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end899:
	.size	test_vector_bitcast_v2i64_to_v8i16, .Lfunc_end899-test_vector_bitcast_v2i64_to_v8i16
	.size	.Ltest_vector_bitcast_v2i64_to_v8i16$local, .Lfunc_end899-test_vector_bitcast_v2i64_to_v8i16
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end900:
	.size	test_vector_bitcast_v2i64_to_v16i8, .Lfunc_end900-test_vector_bitcast_v2i64_to_v16i8
	.size	.Ltest_vector_bitcast_v2i64_to_v16i8$local, .Lfunc_end900-test_vector_bitcast_v2i64_to_v16i8
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end901:
	.size	test_vector_bitcast_v4i32_to_v2i64, .Lfunc_end901-test_vector_bitcast_v4i32_to_v2i64
	.size	.Ltest_vector_bitcast_v4i32_to_v2i64$local, .Lfunc_end901-test_vector_bitcast_v4i32_to_v2i64
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end902:
	.size	test_vector_bitcast_v4i32_to_v8i16, .Lfunc_end902-test_vector_bitcast_v4i32_to_v8i16
	.size	.Ltest_vector_bitcast_v4i32_to_v8i16$local, .Lfunc_end902-test_vector_bitcast_v4i32_to_v8i16
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end903:
	.size	test_vector_bitcast_v4i32_to_v16i8, .Lfunc_end903-test_vector_bitcast_v4i32_to_v16i8
	.size	.Ltest_vector_bitcast_v4i32_to_v16i8$local, .Lfunc_end903-test_vector_bitcast_v4i32_to_v16i8
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end904:
	.size	test_vector_bitcast_v8i16_to_v2i64, .Lfunc_end904-test_vector_bitcast_v8i16_to_v2i64
	.size	.Ltest_vector_bitcast_v8i16_to_v2i64$local, .Lfunc_end904-test_vector_bitcast_v8i16_to_v2i64
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end905:
	.size	test_vector_bitcast_v8i16_to_v4i32, .Lfunc_end905-test_vector_bitcast_v8i16_to_v4i32
	.size	.Ltest_vector_bitcast_v8i16_to_v4i32$local, .Lfunc_end905-test_vector_bitcast_v8i16_to_v4i32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end906:
	.size	test_vector_bitcast_v8i16_to_v16i8, .Lfunc_end906-test_vector_bitcast_v8i16_to_v16i8
	.size	.Ltest_vector_bitcast_v8i16_to_v16i8$local, .Lfunc_end906-test_vector_bitcast_v8i16_to_v16i8
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end907:
	.size	test_vector_bitcast_v16i8_to_v2i64, .Lfunc_end907-test_vector_bitcast_v16i8_to_v2i64
	.size	.Ltest_vector_bitcast_v16i8_to_v2i64$local, .Lfunc_end907-test_vector_bitcast_v16i8_to_v2i64
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end908:
	.size	test_vector_bitcast_v16i8_to_v4i32, .Lfunc_end908-test_vector_bitcast_v16i8_to_v4i32
	.size	.Ltest_vector_bitcast_v16i8_to_v4i32$local, .Lfunc_end908-test_vector_bitcast_v16i8_to_v4i32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end909:
	.size	test_vector_bitcast_v16i8_to_v8i16, .Lfunc_end909-test_vector_bitcast_v16i8_to_v8i16
	.size	.Ltest_vector_bitcast_v16i8_to_v8i16$local, .Lfunc_end909-test_vector_bitcast_v16i8_to_v8i16
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end910:
	.size	test_vector_bitcast_f128_to_v16i8, .Lfunc_end910-test_vector_bitcast_f128_to_v16i8
	.size	.Ltest_vector_bitcast_f128_to_v16i8$local, .Lfunc_end910-test_vector_bitcast_f128_to_v16i8
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end911:
	.size	test_vector_bitcast_f128_to_v8i16, .Lfunc_end911-test_vector_bitcast_f128_to_v8i16
	.size	.Ltest_vector_bitcast_f128_to_v8i16$local, .Lfunc_end911-test_vector_bitcast_f128_to_v8i16
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end912:
	.size	test_vector_bitcast_f128_to_v4i32, .Lfunc_end912-test_vector_bitcast_f128_to_v4i32
	.size	.Ltest_vector_bitcast_f128_to_v4i32$local, .Lfunc_end912-test_vector_bitcast_f128_to_v4i32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end913:
	.size	test_vector_bitcast_f128_to_v2i64, .Lfunc_end913-test_vector_bitcast_f128_to_v2i64
	.size	.Ltest_vector_bitcast_f128_to_v2i64$local, .Lfunc_end913-test_vector_bitcast_f128_to_v2i64
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end914:
	.size	test_vector_bitcast_v16i8_to_f128, .Lfunc_end914-test_vector_bitcast_v16i8_to_f128
	.size	.Ltest_vector_bitcast_v16i8_to_f128$local, .Lfunc_end914-test_vector_bitcast_v16i8_to_f128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end915:
	.size	test_vector_bitcast_v8i16_to_f128, .Lfunc_end915-test_vector_bitcast_v8i16_to_f128
	.size	.Ltest_vector_bitcast_v8i16_to_f128$local, .Lfunc_end915-test_vector_bitcast_v8i16_to_f128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end916:
	.size	test_vector_bitcast_v4i32_to_f128, .Lfunc_end916-test_vector_bitcast_v4i32_to_f128
	.size	.Ltest_vector_bitcast_v4i32_to_f128$local, .Lfunc_end916-test_vector_bitcast_v4i32_to_f128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end917:
	.size	test_vector_bitcast_v2i64_to_f128, .Lfunc_end917-test_vector_bitcast_v2i64_to_f128
	.size	.Ltest_vector_bitcast_v2i64_to_f128$local, .Lfunc_end917-test_vector_bitcast_v2i64_to_f128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end918:
	.size	test_vector_bitcast_i128_to_v16i8, .Lfunc_end918-test_vector_bitcast_i128_to_v16i8
	.size	.Ltest_vector_bitcast_i128_to_v16i8$local, .Lfunc_end918-test_vector_bitcast_i128_to_v16i8
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end919:
	.size	test_vector_bitcast_i128_to_v8i16, .Lfunc_end919-test_vector_bitcast_i128_to_v8i16
	.size	.Ltest_vector_bitcast_i128_to_v8i16$local, .Lfunc_end919-test_vector_bitcast_i128_to_v8i16
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end920:
	.size	test_vector_bitcast_i128_to_v4i32, .Lfunc_end920-test_vector_bitcast_i128_to_v4i32
	.size	.Ltest_vector_bitcast_i128_to_v4i32$local, .Lfunc_end920-test_vector_bitcast_i128_to_v4i32
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end921:
	.size	test_vector_bitcast_i128_to_v2i64, .Lfunc_end921-test_vector_bitcast_i128_to_v2i64
	.size	.Ltest_vector_bitcast_i128_to_v2i64$local, .Lfunc_end921-test_vector_bitcast_i128_to_v2i64
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end922:
	.size	test_vector_bitcast_v16i8_to_i128, .Lfunc_end922-test_vector_bitcast_v16i8_to_i128
	.size	.Ltest_vector_bitcast_v16i8_to_i128$local, .Lfunc_end922-test_vector_bitcast_v16i8_to_i128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end923:
	.size	test_vector_bitcast_v8i16_to_i128, .Lfunc_end923-test_vector_bitcast_v8i16_to_i128
	.size	.Ltest_vector_bitcast_v8i16_to_i128$local, .Lfunc_end923-test_vector_bitcast_v8i16_to_i128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end924:
	.size	test_vector_bitcast_v4i32_to_i128, .Lfunc_end924-test_vector_bitcast_v4i32_to_i128
	.size	.Ltest_vector_bitcast_v4i32_to_i128$local, .Lfunc_end924-test_vector_bitcast_v4i32_to_i128
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
	alloc 2
	retf 0
	nop 0

.Lfunc_end925:
	.size	test_vector_bitcast_v2i64_to_i128, .Lfunc_end925-test_vector_bitcast_v2i64_to_i128
	.size	.Ltest_vector_bitcast_v2i64_to_i128$local, .Lfunc_end925-test_vector_bitcast_v2i64_to_i128
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
	alloc_sp 13, 16
	mov %r5, %fp
	add_imm_i64 %fp, %sp, 16

	! BUNDLE
	add_imm_i64 %sp, %sp, -16
	lda_iprel %r12, %pcfwd(.L.str.61)
	st_i64 %r1, %sp, 0

	! BUNDLE
	call %r11, %jpc(printf)
	add_imm_i64 %sp, %sp, 16
	alignup_u64 %r4, %r1, 3, 4

	! BUNDLE
	sub_i64 %r4, %sp, %r4
	mov %sp, %r4
	add_i64 %r9, %r2, %r1

	! BUNDLE
	lda_xi64.l %r8, %r9, %r3, 0, 333
	sub_i64 %r3, %r1, %r2

	! BUNDLE
	add_imm_i64 %r10, %r3, 44
	sl_add_i64 %r11, %r2, %r1, 1
	sl_add_i64 %r12, %r1, %r2, 1

	! BUNDLE
	mov %r7, %r4
	call %r6, %jpc(stack_array_consumer)
	st_i64 %r7, %fp, -8

	! BUNDLE
	ld_u64 %r3, %r4, 0
	ld_u64 %r4, %fp, -8
	add_add_i64 %r2, %r3, %r2, %r4

	! BUNDLE
	st_i64 %r2, %fp, -8
	ld_u64 %r2, %fp, -8
	add_imm_i64 %sp, %sp, -16

	! BUNDLE
	lda_iprel %r12, %pcfwd(.L.str.62)
	st_i64 %r2, %sp, 0
	call %r11, %jpc(printf)

	! BUNDLE
	add_imm_i64 %sp, %sp, 16
	ld_u64 %r2, %fp, -8
	add_i64 %r1, %r2, %r1

	! BUNDLE
	mov %sp, %fp
	mov %fp, %r5
	retf 0

.Lfunc_end926:
	.size	test_stack_array, .Lfunc_end926-test_stack_array
	.size	.Ltest_stack_array$local, .Lfunc_end926-test_stack_array
                                        ! -- End function
	.type	.L.str,@object                  ! @.str
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str:
	.asciz	"test"
	.size	.L.str, 5

	.type	.L.str.1,@object                ! @.str.1
.L.str.1:
	.asciz	"x32=%d"
	.size	.L.str.1, 7

	.type	.L.str.2,@object                ! @.str.2
.L.str.2:
	.asciz	"x32=%lld"
	.size	.L.str.2, 9

	.type	.L.str.3,@object                ! @.str.3
.L.str.3:
	.asciz	"less %lld\n"
	.size	.L.str.3, 11

	.type	.L.str.4,@object                ! @.str.4
.L.str.4:
	.asciz	"greater %lld\n"
	.size	.L.str.4, 14

	.type	.L.str.5,@object                ! @.str.5
.L.str.5:
	.asciz	"zz=%d\n"
	.size	.L.str.5, 7

	.type	uglbx,@object                   ! @uglbx
	.data
	.p2align	3, 0x0
uglbx:
	.xword	1                               ! 0x1
	.size	uglbx, 8

	.type	.L.str.6,@object                ! @.str.6
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.6:
	.asciz	"xx=%d\n"
	.size	.L.str.6, 7

	.type	.L.str.7,@object                ! @.str.7
.L.str.7:
	.asciz	"x=%le\n"
	.size	.L.str.7, 7

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

	.type	.L.str.24,@object               ! @.str.24
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.24:
	.asciz	"%d\n"
	.size	.L.str.24, 4

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
	.type	.L.str.25,@object               ! @.str.25
.L.str.25:
	.asciz	"case 0 %lld"
	.size	.L.str.25, 12

	.type	.L.str.26,@object               ! @.str.26
.L.str.26:
	.asciz	"case 1 %lld %lld"
	.size	.L.str.26, 17

	.type	.L.str.27,@object               ! @.str.27
.L.str.27:
	.asciz	"case 2 %lld %lld"
	.size	.L.str.27, 17

	.type	.L.str.28,@object               ! @.str.28
.L.str.28:
	.asciz	"case 3 %lld %lld"
	.size	.L.str.28, 17

	.type	.L.str.29,@object               ! @.str.29
.L.str.29:
	.asciz	"case 4 %lld %lld"
	.size	.L.str.29, 17

	.type	.L.str.30,@object               ! @.str.30
.L.str.30:
	.asciz	"case 5 %lld %lld"
	.size	.L.str.30, 17

	.type	.L.str.31,@object               ! @.str.31
.L.str.31:
	.asciz	"case 0 %d"
	.size	.L.str.31, 10

	.type	.L.str.32,@object               ! @.str.32
.L.str.32:
	.asciz	"case 1 %d %d"
	.size	.L.str.32, 13

	.type	.L.str.33,@object               ! @.str.33
.L.str.33:
	.asciz	"case 2 %d %d"
	.size	.L.str.33, 13

	.type	.L.str.34,@object               ! @.str.34
.L.str.34:
	.asciz	"case 3 %d %d"
	.size	.L.str.34, 13

	.type	.L.str.35,@object               ! @.str.35
.L.str.35:
	.asciz	"case 4 %d %d"
	.size	.L.str.35, 13

	.type	.L.str.36,@object               ! @.str.36
.L.str.36:
	.asciz	"case 5 %d %d"
	.size	.L.str.36, 13

	.type	.L.str.37,@object               ! @.str.37
.L.str.37:
	.asciz	"case 0 %u"
	.size	.L.str.37, 10

	.type	.L.str.38,@object               ! @.str.38
.L.str.38:
	.asciz	"case 1 %u %u"
	.size	.L.str.38, 13

	.type	.L.str.39,@object               ! @.str.39
.L.str.39:
	.asciz	"case 2 %u %u"
	.size	.L.str.39, 13

	.type	.L.str.40,@object               ! @.str.40
.L.str.40:
	.asciz	"case 3 %u %du"
	.size	.L.str.40, 14

	.type	.L.str.41,@object               ! @.str.41
.L.str.41:
	.asciz	"case 4 %u %u"
	.size	.L.str.41, 13

	.type	.L.str.42,@object               ! @.str.42
.L.str.42:
	.asciz	"case 5 %u %u"
	.size	.L.str.42, 13

	.type	.L.str.43,@object               ! @.str.43
.L.str.43:
	.asciz	"case 10 %lld"
	.size	.L.str.43, 13

	.type	.L.str.44,@object               ! @.str.44
.L.str.44:
	.asciz	"case 11 %lld %lld"
	.size	.L.str.44, 18

	.type	.L.str.45,@object               ! @.str.45
.L.str.45:
	.asciz	"case 12 %lld %lld"
	.size	.L.str.45, 18

	.type	.L.str.46,@object               ! @.str.46
.L.str.46:
	.asciz	"case 13 %lld %lld"
	.size	.L.str.46, 18

	.type	.L.str.47,@object               ! @.str.47
.L.str.47:
	.asciz	"case 14 %lld %lld"
	.size	.L.str.47, 18

	.type	.L.str.48,@object               ! @.str.48
.L.str.48:
	.asciz	"case 15 %lld %lld"
	.size	.L.str.48, 18

	.type	.L.str.49,@object               ! @.str.49
.L.str.49:
	.asciz	"case 10 %d"
	.size	.L.str.49, 11

	.type	.L.str.50,@object               ! @.str.50
.L.str.50:
	.asciz	"case 11 %d %d"
	.size	.L.str.50, 14

	.type	.L.str.51,@object               ! @.str.51
.L.str.51:
	.asciz	"case 12 %d %d"
	.size	.L.str.51, 14

	.type	.L.str.52,@object               ! @.str.52
.L.str.52:
	.asciz	"case 13 %d %d"
	.size	.L.str.52, 14

	.type	.L.str.53,@object               ! @.str.53
.L.str.53:
	.asciz	"case 14 %d %d"
	.size	.L.str.53, 14

	.type	.L.str.54,@object               ! @.str.54
.L.str.54:
	.asciz	"case 15 %d %d"
	.size	.L.str.54, 14

	.type	.L.str.55,@object               ! @.str.55
.L.str.55:
	.asciz	"case 10 %u"
	.size	.L.str.55, 11

	.type	.L.str.56,@object               ! @.str.56
.L.str.56:
	.asciz	"case 11 %u %u"
	.size	.L.str.56, 14

	.type	.L.str.57,@object               ! @.str.57
.L.str.57:
	.asciz	"case 12 %u %u"
	.size	.L.str.57, 14

	.type	.L.str.58,@object               ! @.str.58
.L.str.58:
	.asciz	"case 13 %u %du"
	.size	.L.str.58, 15

	.type	.L.str.59,@object               ! @.str.59
.L.str.59:
	.asciz	"case 14 %u %u"
	.size	.L.str.59, 14

	.type	.L.str.60,@object               ! @.str.60
.L.str.60:
	.asciz	"case 15 %u %u"
	.size	.L.str.60, 14

	.type	x_tls,@object                   ! @x_tls
	.section	.tbss,"awT",@nobits
	.globl	x_tls
	.p2align	3, 0x0
x_tls:
	.xword	0                               ! 0x0
	.size	x_tls, 8

	.type	.L.str.61,@object               ! @.str.61
	.section	.rodata.str1.1,"aMS",@progbits,1
.L.str.61:
	.asciz	"ccc=%lld\n"
	.size	.L.str.61, 10

	.type	.L.str.62,@object               ! @.str.62
.L.str.62:
	.asciz	"temp=%lld\n"
	.size	.L.str.62, 11

	.type	.Lstr,@object                   ! @str
.Lstr:
	.asciz	"test f32 ne"
	.size	.Lstr, 12

	.type	.Lstr.63,@object                ! @str.63
.Lstr.63:
	.asciz	"test f32 le"
	.size	.Lstr.63, 12

	.type	.Lstr.64,@object                ! @str.64
.Lstr.64:
	.asciz	"test f32 gt"
	.size	.Lstr.64, 12

	.type	.Lstr.65,@object                ! @str.65
.Lstr.65:
	.asciz	"test f32 ge"
	.size	.Lstr.65, 12

	.type	.Lstr.66,@object                ! @str.66
.Lstr.66:
	.asciz	"test f64 eq"
	.size	.Lstr.66, 12

	.type	.Lstr.67,@object                ! @str.67
.Lstr.67:
	.asciz	"test f64 ne"
	.size	.Lstr.67, 12

	.type	.Lstr.68,@object                ! @str.68
.Lstr.68:
	.asciz	"test f64 lt"
	.size	.Lstr.68, 12

	.type	.Lstr.69,@object                ! @str.69
.Lstr.69:
	.asciz	"test f64 le"
	.size	.Lstr.69, 12

	.type	.Lstr.70,@object                ! @str.70
.Lstr.70:
	.asciz	"test f64 gt"
	.size	.Lstr.70, 12

	.type	.Lstr.71,@object                ! @str.71
.Lstr.71:
	.asciz	"test f64 ge"
	.size	.Lstr.71, 12

	.type	.Lstr.72,@object                ! @str.72
.Lstr.72:
	.asciz	"test f128 eq"
	.size	.Lstr.72, 13

	.type	.Lstr.73,@object                ! @str.73
.Lstr.73:
	.asciz	"test f128 ne"
	.size	.Lstr.73, 13

	.type	.Lstr.74,@object                ! @str.74
.Lstr.74:
	.asciz	"test f128 lt"
	.size	.Lstr.74, 13

	.type	.Lstr.75,@object                ! @str.75
.Lstr.75:
	.asciz	"test f128 le"
	.size	.Lstr.75, 13

	.type	.Lstr.76,@object                ! @str.76
.Lstr.76:
	.asciz	"test f128 gt"
	.size	.Lstr.76, 13

	.type	.Lstr.77,@object                ! @str.77
.Lstr.77:
	.asciz	"test f128 ge"
	.size	.Lstr.77, 13

	.ident	"clang version 23.0.0git"
	.section	".note.GNU-stack","",@progbits
