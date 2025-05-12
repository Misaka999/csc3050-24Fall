	.file	"branch.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	__floatsidf
	.globl	__fixdfsi
	.align	2
	.globl	generatePrimes
	.type	generatePrimes, @function
generatePrimes:
	#start your code
	li	t0, 0
	li	t1, 2
	lui	t5,%hi(primes)
	addi	t5,t5,%lo(primes)
	addi	s0, a0, 0
i_loop:
	bgt	t1, s0, end_i_loop
	addi	a0, t1, 0
	li	t2, 2
	addi	sp, sp, -20
	sw	ra, 16(sp)
	sw	t0, 12(sp)
	sw	t1, 8(sp)
	sw	t2, 4(sp)
	sw	t5, 0(sp)
	call	__floatsidf
	call	sqrt
	call	__fixdfsi
	lw	t5, 0(sp)
	lw	t2, 4(sp)
	lw	t1, 8(sp)
	lw	t0, 12(sp)
	lw	ra, 16(sp)
	addi	sp, sp, 20
	addi	t3, a0, 0

j_loop:
	bgt	t2, t3, prime_add
	addi	a1, t1, 0
	addi	a2, t2, 0
	rem a0, a1, a2
	beqz	a0, next_i
	addi	t2, t2, 1
	j	j_loop
	
prime_add:
	sw	t1, 0(t5)
	addi	t5, t5, 4
	addi	t0, t0, 1
	
next_i:
	addi	t1, t1, 1
	j	i_loop
	
end_i_loop:
	ret
	.size	generatePrimes, .-generatePrimes
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
.LC0:
	.string	"Prime numbers up to %d\n"
	.align	2
.LC1:
	.string	"Success"
	.align	2
.LC2:
	.string	"Failure"
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	lui	a0,%hi(.LC0)
	addi	sp,sp,-16
	li	a1,100
	addi	a0,a0,%lo(.LC0)
	sw	ra,12(sp)
	call	printf
	li	a0,100
	call	generatePrimes
	lui	a4,%hi(.LANCHOR1)
	addi	a4,a4,%lo(.LANCHOR1)
	lui	a3,%hi(.LANCHOR0)
	addi	a3,a3,%lo(.LANCHOR0)
	addi	a0,a4,120
	li	a2,1
.L22:
	lw	a5,0(a4)
	lw	a1,0(a3)
	addi	a4,a4,4
	addi	a3,a3,4
	sub	a5,a5,a1
	seqz	a5,a5
	neg	a5,a5
	and	a2,a2,a5
	bne	a4,a0,.L22
	beq	a2,zero,.L23
	lui	a0,%hi(.LC1)
	addi	a0,a0,%lo(.LC1)
	call	puts
.L24:
	lw	ra,12(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
.L23:
	lui	a0,%hi(.LC2)
	addi	a0,a0,%lo(.LC2)
	call	puts
	j	.L24
	.size	main, .-main
	.globl	primes
	.globl	target
	.data
	.align	2
	.set	.LANCHOR1,. + 0
	.type	target, @object
	.size	target, 120
target:
	.word	2
	.word	3
	.word	5
	.word	7
	.word	11
	.word	13
	.word	17
	.word	19
	.word	23
	.word	29
	.word	31
	.word	37
	.word	41
	.word	43
	.word	47
	.word	53
	.word	59
	.word	61
	.word	67
	.word	71
	.word	73
	.word	79
	.word	83
	.word	89
	.word	97
	.zero	20
	.bss
	.align	2
	.set	.LANCHOR0,. + 0
	.type	primes, @object
	.size	primes, 120
primes:
	.zero	120
	.ident	"GCC: (SiFive GCC 10.1.0-2020.08.2) 10.1.0"
