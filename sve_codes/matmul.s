	.arch armv8-a+sve
	.file	"matmul.c"
	.text
	.align	2
	.p2align 4,,11
	.global	matmul_basic
	.type	matmul_basic, %function
matmul_basic:
.LFB11:
	.cfi_startproc
	stp	x29, x30, [sp, -64]!
	.cfi_def_cfa_offset 64
	.cfi_offset 29, -64
	.cfi_offset 30, -56
	mov	x29, sp
	stp	x21, x22, [sp, 32]
	.cfi_offset 21, -32
	.cfi_offset 22, -24
	mov	x21, x0
	mul	w0, w2, w3
	mov	x22, x1
	stp	x19, x20, [sp, 16]
	.cfi_offset 19, -48
	.cfi_offset 20, -40
	mov	w20, w2
	mov	w19, w4
	sbfiz	x0, x0, 2, 32
	str	x23, [sp, 48]
	.cfi_offset 23, -16
	mov	w23, w3
	bl	malloc
	cmp	w20, 0
	ble	.L1
	cmp	w23, 0
	ble	.L1
	sxtw	x4, w19
	cntb	x1
	index	z3.s, #0, w4
	mov	w8, w23
	mov	w9, 0
	mov	w10, 0
	mul	x4, x4, x1
	whilelo	p2.s, wzr, w19
	.p2align 3,,7
.L4:
	mov	x6, x22
	add	x3, x21, x9, sxtw 2
	mov	w5, w9
	.p2align 3,,7
.L9:
	cmp	w19, 0
	ble	.L8
	sxtw	x7, w5
	mov	x2, x6
	mov	x1, 0
	mov	p0.b, p2.b
	ldr	s1, [x0, x7, lsl 2]
	.p2align 3,,7
.L5:
	ld1w	z2.s, p0/z, [x3, x1, lsl 2]
	ld1w	z0.s, p0/z, [x2, z3.s, sxtw 2]
	incw	x1
	fmul	z0.s, z0.s, z2.s
	add	x2, x2, x4
	fadda	s1, p0, s1, z0.s
	whilelo	p0.s, w1, w19
	b.any	.L5
	str	s1, [x0, x7, lsl 2]
.L8:
	add	w5, w5, 1
	add	x6, x6, 4
	cmp	w5, w8
	bne	.L9
	add	w10, w10, 1
	add	w8, w8, w20
	add	w9, w9, w20
	cmp	w20, w10
	bne	.L4
.L1:
	ldp	x19, x20, [sp, 16]
	ldp	x21, x22, [sp, 32]
	ldr	x23, [sp, 48]
	ldp	x29, x30, [sp], 64
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 23
	.cfi_restore 21
	.cfi_restore 22
	.cfi_restore 19
	.cfi_restore 20
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE11:
	.size	matmul_basic, .-matmul_basic
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 4,,11
	.global	main
	.type	main, %function
main:
.LFB12:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x0, 65536
	mov	x29, sp
	str	x19, [sp, 16]
	.cfi_offset 19, -16
	bl	malloc
	mov	x19, x0
	mov	x0, 65536
	bl	malloc
	mov	x1, x0
	mov	w4, 128
	mov	x0, x19
	mov	w3, w4
	mov	w2, w4
	bl	matmul_basic
	mov	w0, 0
	ldr	x19, [sp, 16]
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_restore 19
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (Debian 10.2.1-6) 10.2.1 20210110"
	.section	.note.GNU-stack,"",@progbits
