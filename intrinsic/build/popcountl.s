	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## BB#0:
	pushq	%rbp
Lcfi0:
	.cfi_def_cfa_offset 16
Lcfi1:
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
Lcfi2:
	.cfi_def_cfa_register %rbp
	subq	$32, %rsp
	movabsq	$1099511627521, %rax    ## imm = 0xFFFFFFFF01
	movl	$0, -4(%rbp)
	movl	$0, -12(%rbp)
	movq	%rax, -24(%rbp)
	movl	$0, -8(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$2000000000, -8(%rbp)   ## imm = 0x77359400
	jge	LBB0_6
## BB#2:                                ##   in Loop: Header=BB0_1 Depth=1
	movq	-24(%rbp), %rax
	movq	%rax, %rcx
	shrq	%rcx
	movabsq	$6148914691236517205, %rdx ## imm = 0x5555555555555555
	andq	%rdx, %rcx
	subq	%rcx, %rax
	movabsq	$3689348814741910323, %rcx ## imm = 0x3333333333333333
	movq	%rax, %rdx
	andq	%rcx, %rdx
	shrq	$2, %rax
	andq	%rcx, %rax
	addq	%rax, %rdx
	movq	%rdx, %rax
	shrq	$4, %rax
	addq	%rax, %rdx
	movabsq	$1085102592571150095, %rax ## imm = 0xF0F0F0F0F0F0F0F
	andq	%rax, %rdx
	movabsq	$72340172838076673, %rax ## imm = 0x101010101010101
	imulq	%rax, %rdx
	shrq	$56, %rdx
	movl	%edx, %esi
	movl	%esi, -12(%rbp)
	cmpl	$33, -12(%rbp)
	je	LBB0_4
## BB#3:
	leaq	L_.str(%rip), %rdi
	movb	$0, %al
	callq	_printf
	movl	$0, -4(%rbp)
	movl	%eax, -28(%rbp)         ## 4-byte Spill
	jmp	LBB0_7
LBB0_4:                                 ##   in Loop: Header=BB0_1 Depth=1
	jmp	LBB0_5
LBB0_5:                                 ##   in Loop: Header=BB0_1 Depth=1
	movl	-8(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -8(%rbp)
	jmp	LBB0_1
LBB0_6:
	leaq	L_.str.1(%rip), %rdi
	movl	-12(%rbp), %esi
	movb	$0, %al
	callq	_printf
	movl	$0, -4(%rbp)
	movl	%eax, -32(%rbp)         ## 4-byte Spill
LBB0_7:
	movl	-4(%rbp), %eax
	addq	$32, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"ERROR!"

L_.str.1:                               ## @.str.1
	.asciz	"%d"


.subsections_via_symbols
