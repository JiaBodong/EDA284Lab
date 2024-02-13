	.arch armv8-a
	.file	"stencil.c"
	.text
	.align	2
	.p2align 3,,7
	.global	stencil_1d
	.type	stencil_1d, %function
stencil_1d:
.LFB11:
	.cfi_startproc
	sub	w9, w1, #2
	cmp	w9, 0
	ble	.L1
	lsr	w3, w9, 2
	mov	w13, 2
	fmov	v3.4s, 3.0e+0
	sub	w5, w1, #3
	and	w10, w9, -4
	add	x3, x0, x3, uxtw 4
	sub	w11, w1, #1
	mov	w7, 100
	movk	w13, 0x8000, lsl 16
	fmov	s4, 3.0e+0
	.p2align 3
.L3:
	cmp	w5, 2
	bls	.L9
	mov	x2, x0
	.p2align 3
.L4:
	ldr	q0, [x2]
	ldr	q2, [x2, 4]
	ldr	q1, [x2, 8]
	fadd	v0.4s, v0.4s, v2.4s
	fadd	v0.4s, v0.4s, v1.4s
	fdiv	v0.4s, v0.4s, v3.4s
	str	q0, [x2], 16
	cmp	x3, x2
	bne	.L4
	mov	w4, w10
	cmp	w9, w10
	beq	.L5
	cmp	w11, w5
	add	w2, w4, 1
	csel	w6, w11, w5, le
	sxtw	x8, w4
	cmp	w2, w6
	ccmp	w1, w13, 4, lt
	add	x6, x0, x8, lsl 2
	beq	.L8
.L15:
	ldp	s0, s2, [x6, 4]
	add	w2, w4, 3
	ldr	s1, [x0, x8, lsl 2]
	cmp	w5, w2
	add	w2, w4, 2
	fadd	s1, s0, s1
	fadd	s0, s0, s2
	fadd	s1, s1, s2
	fdiv	s1, s1, s4
	str	s1, [x0, x8, lsl 2]
	ldr	s1, [x6, 12]
	fadd	s0, s0, s1
	fdiv	s0, s0, s4
	str	s0, [x6, 4]
	ble	.L6
	sxtw	x12, w2
	fadd	s0, s2, s1
	add	w8, w4, 5
	add	w2, w4, 4
	add	x6, x0, x12, lsl 2
	cmp	w5, w8
	ldr	s2, [x6, 8]
	fadd	s0, s0, s2
	fadd	s1, s1, s2
	fdiv	s0, s0, s4
	str	s0, [x0, x12, lsl 2]
	ldr	s5, [x6, 12]
	fadd	s0, s1, s5
	fdiv	s0, s0, s4
	str	s0, [x6, 4]
	ble	.L6
	sxtw	x6, w2
	add	w2, w4, 6
	fadd	s0, s2, s5
	add	x4, x0, x6, lsl 2
	ldr	s1, [x4, 8]
	fadd	s0, s0, s1
	fadd	s1, s1, s5
	fdiv	s0, s0, s4
	str	s0, [x0, x6, lsl 2]
	ldr	s0, [x4, 12]
	fadd	s0, s1, s0
	fdiv	s0, s0, s4
	str	s0, [x4, 4]
.L6:
	sxtw	x6, w2
	add	w4, w2, 1
	cmp	w9, w4
	add	x8, x0, x6, lsl 2
	ldr	s2, [x0, x6, lsl 2]
	ldp	s0, s1, [x8, 4]
	fadd	s0, s0, s2
	fadd	s0, s0, s1
	fdiv	s0, s0, s4
	str	s0, [x0, x6, lsl 2]
	ble	.L5
	sxtw	x4, w4
	add	w2, w2, 2
	cmp	w9, w2
	add	x6, x0, x4, lsl 2
	ldr	s2, [x0, x4, lsl 2]
	ldp	s0, s1, [x6, 4]
	fadd	s0, s0, s2
	fadd	s0, s0, s1
	fdiv	s0, s0, s4
	str	s0, [x0, x4, lsl 2]
	ble	.L5
	sxtw	x2, w2
	add	x4, x0, x2, lsl 2
	ldr	s2, [x0, x2, lsl 2]
	ldp	s0, s1, [x4, 4]
	fadd	s0, s0, s2
	fadd	s0, s0, s1
	fdiv	s0, s0, s4
	str	s0, [x0, x2, lsl 2]
.L5:
	subs	w7, w7, #1
	bne	.L3
.L1:
	ret
.L9:
	mov	w4, 0
	cmp	w11, w5
	csel	w6, w11, w5, le
	add	w2, w4, 1
	sxtw	x8, w4
	cmp	w2, w6
	ccmp	w1, w13, 4, lt
	add	x6, x0, x8, lsl 2
	bne	.L15
.L8:
	mov	w2, w4
	b	.L6
	.cfi_endproc
.LFE11:
	.size	stencil_1d, .-stencil_1d
	.section	.text.startup,"ax",@progbits
	.align	2
	.p2align 3,,7
	.global	main
	.type	main, %function
main:
.LFB12:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	mov	x0, 4096
	mov	x29, sp
	bl	malloc
	mov	w1, 1024
	bl	stencil_1d
	bl	free
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE12:
	.size	main, .-main
	.ident	"GCC: (Debian 8.3.0-2) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
