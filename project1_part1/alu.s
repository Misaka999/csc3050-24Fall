	.file	"alu.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.globl	registers
	.bss
	.align	2
	.type	registers, @object
	.size	registers, 128
registers:
	.zero	128
	.text
	.align	2
	.globl	add
	.type	add, @function
add:
	#template 
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	add t4, t1, t2
	sw	t4,0(t3)
	ret #return
	.size	add, .-add
	.align	2
	.globl	And
	.type	And, @function
And:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	and t4, t1, t2
	sw	t4,0(t3)

	ret
	.size	And, .-And
	.align	2
	.globl	sub
	.type	sub, @function
sub:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	sub t4, t1, t2
	sw	t4,0(t3)

	ret
	.size	sub, .-sub
	.align	2
	.globl	nor
	.type	nor, @function
nor:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	or	t4, t1, t2
	xor	t4, t4, -1
	sw	t4,0(t3)

	ret
	.size	nor, .-nor
	.align	2
	.globl	Or
	.type	Or, @function
Or:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1, 0(t1)
	lw	t2, 0(t2)
	or t4, t1, t2
	sw	t4, 0(t3)

	ret
	.size	Or, .-Or
	.align	2
	.globl	Xor
	.type	Xor, @function
Xor:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	xor t4, t1, t2
	sw	t4, 0(t3)

	ret
	.size	Xor, .-Xor
	.align	2
	.globl	beq
	.type	beq, @function
beq:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	sub     t4, t1, t2
	sw      t4, 0(t3)

	ret
	.size	beq, .-beq
	.align	2
	.globl	bne
	.type	bne, @function
bne:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	sub     t4, t1, t2
	sw      t4, 0(t3)
	ret
	.size	bne, .-bne
	.align	2
	.globl	slt
	.type	slt, @function
slt:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	slt t4, t1, t2
	sw	t4,0(t3)
	
	ret
	.size	slt, .-slt
	.align	2
	.globl	sll
	.type	sll, @function
sll:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	sll t4, t1, t2
	sw	t4,0(t3)

	ret
	.size	sll, .-sll
	.align	2
	.globl	srl
	.type	srl, @function
srl:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	srl t4, t1, t2
	sw	t4,0(t3)

	ret
	.size	srl, .-srl
	.align	2
	.globl	sra
	.type	sra, @function
sra:
	#start your code
	lui	a5,%hi(registers)
	addi	t0,a5,%lo(registers)
	slli	a0,a0,2
	slli	a1,a1,2
	slli	a2,a2,2
	add t1, t0, a0
	add t2, t0, a1
	add t3, t0, a2
	lw	t1,0(t1)
	lw	t2,0(t2)
	sra t4, t1, t2
	sw	t4,0(t3)
	
	ret
	.size	sra, .-sra
	.section	.rodata
	.align	2
.LC0:
	.string	"test1 about add "
	.align	2
.LC1:
	.string	"success"
	.align	2
.LC2:
	.string	"fail"
	.align	2
.LC3:
	.string	"test2 about add"
	.align	2
.LC4:
	.string	"test3 about add"
	.align	2
.LC5:
	.string	"test4 about sub"
	.align	2
.LC6:
	.string	"test5 about sub"
	.align	2
.LC7:
	.string	"test6 about and"
	.align	2
.LC8:
	.string	"test7 about nor"
	.align	2
.LC9:
	.string	"test8 about or"
	.align	2
.LC10:
	.string	"test9 about xor"
	.align	2
.LC11:
	.string	"test10 about beq"
	.align	2
.LC12:
	.string	"test11 about beq"
	.align	2
.LC13:
	.string	"test12 about bne"
	.align	2
.LC14:
	.string	"test13 about bne"
	.align	2
.LC15:
	.string	"test14 about slt"
	.align	2
.LC16:
	.string	"test15 about slt"
	.align	2
.LC17:
	.string	"test16 about slt"
	.align	2
.LC18:
	.string	"test17 about sll"
	.align	2
.LC19:
	.string	"test18 about sll"
	.align	2
.LC20:
	.string	"test19 about srl"
	.align	2
.LC21:
	.string	"test20 about srl"
	.align	2
.LC22:
	.string	"test21 about sra"
	.align	2
.LC23:
	.string	"test22 about sra"
	.align	2
.LC24:
	.string	"congratulation! Pass all tests."
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-128
	sw	ra,124(sp)
	sw	s0,120(sp)
	addi	s0,sp,128
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	addi	a5,s0,-116
	li	a4,88
	mv	a2,a4
	li	a1,0
	mv	a0,a5
	call	memset
	sw	zero,-28(s0)
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	add
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,2
	bne	a4,a5,.L27
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L28
.L27:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L28:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-2147483648
	xori	a4,a4,-1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,2
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	add
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-2147483648
	addi	a5,a5,1
	bne	a4,a5,.L29
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L30
.L29:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L30:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-2147483648
	addi	a4,a4,1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-2
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	li	a2,2
	li	a1,1
	li	a0,0
	call	add
	lui	a5,%hi(.LC4)
	addi	a0,a5,%lo(.LC4)
	call	puts
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-2147483648
	xori	a5,a5,-1
	bne	a4,a5,.L31
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L32
.L31:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L32:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(.LC5)
	addi	a0,a5,%lo(.LC5)
	call	puts
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-2
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,2
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	li	a2,2
	li	a1,1
	li	a0,0
	call	sub
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-4
	bne	a4,a5,.L33
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L34
.L33:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L34:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(.LC6)
	addi	a0,a5,%lo(.LC6)
	call	puts
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-2147483648
	xori	a4,a4,-1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-1
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	li	a2,2
	li	a1,1
	li	a0,0
	call	sub
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-2147483648
	bne	a4,a5,.L35
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L36
.L35:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L36:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(.LC7)
	addi	a0,a5,%lo(.LC7)
	call	puts
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,65536
	addi	a4,a4,1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,286330880
	addi	a4,a4,273
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	li	a2,2
	li	a1,1
	li	a0,0
	call	And
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,65536
	addi	a5,a5,1
	bne	a4,a5,.L37
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L38
.L37:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L38:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,16
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC8)
	addi	a0,a5,%lo(.LC8)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	nor
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-18
	bne	a4,a5,.L39
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L40
.L39:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L40:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,16
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC9)
	addi	a0,a5,%lo(.LC9)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	Or
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,17
	bne	a4,a5,.L41
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L42
.L41:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L42:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,17
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC10)
	addi	a0,a5,%lo(.LC10)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	Xor
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,16
	bne	a4,a5,.L43
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L44
.L43:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L44:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,6
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC11)
	addi	a0,a5,%lo(.LC11)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	beq
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-1
	bne	a4,a5,.L45
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L46
.L45:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L46:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-1
	sw	a4,8(a5)
	lui	a5,%hi(.LC12)
	addi	a0,a5,%lo(.LC12)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	beq
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a5,8(a5)
	bne	a5,zero,.L47
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L48
.L47:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L48:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,6
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC13)
	addi	a0,a5,%lo(.LC13)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	bne
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,1
	bne	a4,a5,.L49
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L50
.L49:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L50:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,8(a5)
	lui	a5,%hi(.LC14)
	addi	a0,a5,%lo(.LC14)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	bne
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a5,8(a5)
	bne	a5,zero,.L51
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L52
.L51:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L52:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-5
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-1
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,2
	sw	a4,8(a5)
	lui	a5,%hi(.LC15)
	addi	a0,a5,%lo(.LC15)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	slt
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,1
	bne	a4,a5,.L53
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L54
.L53:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L54:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,2
	sw	a4,8(a5)
	lui	a5,%hi(.LC16)
	addi	a0,a5,%lo(.LC16)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	slt
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a5,8(a5)
	bne	a5,zero,.L55
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L56
.L55:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L56:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,8
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-9
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,2
	sw	a4,8(a5)
	lui	a5,%hi(.LC17)
	addi	a0,a5,%lo(.LC17)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	slt
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a5,8(a5)
	bne	a5,zero,.L57
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L58
.L57:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L58:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,273
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,3
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC18)
	addi	a0,a5,%lo(.LC18)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	sll
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,4096
	addi	a5,a5,-1912
	bne	a4,a5,.L59
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L60
.L59:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L60:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,3
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC19)
	addi	a0,a5,%lo(.LC19)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	sll
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-8
	bne	a4,a5,.L61
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L62
.L61:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L62:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,3
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,8(a5)
	lui	a5,%hi(.LC20)
	addi	a0,a5,%lo(.LC20)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	srl
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a5,8(a5)
	bne	a5,zero,.L63
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L64
.L63:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L64:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,3
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC21)
	addi	a0,a5,%lo(.LC21)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	srl
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,536870912
	addi	a5,a5,-1
	bne	a4,a5,.L65
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L66
.L65:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L66:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,3
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,5
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,1
	sw	a4,8(a5)
	lui	a5,%hi(.LC22)
	addi	a0,a5,%lo(.LC22)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	sra
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a5,8(a5)
	bne	a5,zero,.L67
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L68
.L67:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L68:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,-1
	sw	a4,0(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	li	a4,3
	sw	a4,4(a5)
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	sw	zero,8(a5)
	lui	a5,%hi(.LC23)
	addi	a0,a5,%lo(.LC23)
	call	puts
	li	a2,2
	li	a1,1
	li	a0,0
	call	sra
	lui	a5,%hi(registers)
	addi	a5,a5,%lo(registers)
	lw	a4,8(a5)
	li	a5,-1
	bne	a4,a5,.L69
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	puts
	lw	a5,-28(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	li	a4,1
	sw	a4,-100(a5)
	j	.L70
.L69:
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	puts
.L70:
	lw	a5,-28(s0)
	addi	a5,a5,1
	sw	a5,-28(s0)
	li	a5,1
	sw	a5,-20(s0)
	sw	zero,-24(s0)
	j	.L71
.L74:
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	lw	a5,-100(a5)
	bne	a5,zero,.L72
	sw	zero,-20(s0)
	j	.L73
.L72:
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L71:
	lw	a4,-24(s0)
	li	a5,21
	ble	a4,a5,.L74
.L73:
	lw	a5,-20(s0)
	beq	a5,zero,.L75
	lui	a5,%hi(.LC24)
	addi	a0,a5,%lo(.LC24)
	call	puts
.L75:
	li	a5,0
	mv	a0,a5
	lw	ra,124(sp)
	lw	s0,120(sp)
	addi	sp,sp,128
	jr	ra
	.size	main, .-main
	.ident	"GCC: (SiFive GCC 10.1.0-2020.08.2) 10.1.0"
	
