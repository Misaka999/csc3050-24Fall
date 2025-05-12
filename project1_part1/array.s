	.file	"array.c"
	.option nopic
	.attribute arch, "rv32i2p0_m2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 16
	.text
	.align	2
	.globl	printBitmap
	.type	printBitmap, @function
printBitmap:
	li	t1, 0    # i = 0
	addi	s0, a0, 0
	lui	t5, %hi(bitmap)
	addi	t5, t5, %lo(bitmap)

i_loop:
	bge	t1, s0, i_end
	li	t2, 0    # j = 0
	li	s1, 8

j_loop:
	bge	t2, s1, j_end
	lbu	s3, 0(t5)
	li	t3, 7   # k = 7
	li	s2, 0

k_loop:
	blt	t3, s2, k_end

	li	t6, 1
	sll	a4, t6, t3    
	and	a1, s3, a4      

	bnez	a1, print_1 

	li	a0, 32
	addi	sp, sp, -20
	sw	ra, 16(sp)
	sw	t1, 12(sp)
	sw	t2, 8(sp)
	sw	t3, 4(sp)
	sw	t5, 0(sp)
	call	putchar
	lw	t5, 0(sp)
	lw	t3, 4(sp)
	lw	t2, 8(sp)
	lw	t1, 12(sp)
	lw	ra, 16(sp)
	addi	sp, sp, 20

	j	next_k

print_1:
	li	a0, 49

	addi	sp, sp, -20
	sw	ra, 16(sp)
	sw	t1, 12(sp)
	sw	t2, 8(sp)
	sw	t3, 4(sp)
	sw	t5, 0(sp)
	call	putchar
	lw	t5, 0(sp)
	lw	t3, 4(sp)
	lw	t2, 8(sp)
	lw	t1, 12(sp)
	lw	ra, 16(sp)
	addi	sp, sp, 20

next_k:
	addi	t3, t3, -1
	j	k_loop

k_end:
	li	a0, 32

	addi	sp, sp, -20
	sw	ra, 16(sp)
	sw	t1, 12(sp)
	sw	t2, 8(sp)
	sw	t3, 4(sp)
	sw	t5, 0(sp)
	call    putchar
	lw	t5, 0(sp)
	lw	t3, 4(sp)
	lw	t2, 8(sp)
	lw	t1, 12(sp)
	lw	ra, 16(sp)
	addi	sp, sp, 20

	addi	t2, t2, 1
	addi	t5, t5, 1

	j	j_loop

j_end:
	li	a0, 10

	addi	sp, sp, -20
	sw	ra, 16(sp)
	sw	t1, 12(sp)
	sw	t2, 8(sp)
	sw	t3, 4(sp)
	sw	t5, 0(sp)
	call	putchar	
	lw	t5, 0(sp)
	lw	t3, 4(sp)
	lw	t2, 8(sp)
	lw	t1, 12(sp)
	lw	ra, 16(sp)
	addi	sp, sp, 20
	
	addi	t1, t1, 1
	j	i_loop

i_end:
	ret
	.size	printBitmap, .-printBitmap
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	li	a0,16
	sw	ra,12(sp)
	call	printBitmap
	lw	ra,12(sp)
	li	a0,0
	addi	sp,sp,16
	jr	ra
	.size	main, .-main
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
	.type	bitmap, @object
	.size	bitmap, 128
bitmap:
	.string	"\020"
	.string	"\020"
	.string	"\020@"
	.ascii	" "
	.string	"\b\004\020"
	.string	"\020@"
	.ascii	"\360"
	.string	"\177x\037\374\020@\037"
	.string	""
	.string	"@ \200\023\370\020"
	.string	"\"@ \200\030H\021"
	.string	"\024@@\200TH!"
	.string	"\377~\037\370PH!"
	.ascii	"\bH\020\200PH?\374"
	.string	"\bH\020\200\227\376\001"
	.ascii	"\177H\020\200\020@\t "
	.ascii	"\bH\377\376\020\240\t\020"
	.string	"*H"
	.ascii	"\200\020\240\021\b"
	.string	"IH"
	.ascii	"\200\021\020!\004"
	.string	"\210\210"
	.ascii	"\200\021\020A\004"
	.string	"(\210"
	.string	"\200\022\b\005"
	.string	"\021\b"
	.string	"\200\024\006\002"
	.ident	"GCC: (SiFive GCC 10.1.0-2020.08.2) 10.1.0"
