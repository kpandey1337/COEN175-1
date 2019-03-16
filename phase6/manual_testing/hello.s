_main:
#Begin: Prologue
	pushl	%ebp
	movl	%esp, %ebp
	subl	$main.size, %esp
#Begin: Function Body
	subl	$12, %esp
	movl	.L0,	%eax
	leal	.L0,	%eax
	pushl	%eax
	call	_printf
	addl	$16, %esp
#Begin: Epilogue
	movl	%ebp, %esp
	popl	%ebp
	ret

	.set	main.size, 8
	.globl	_main

.L0:	.asciz	"hello world\n"
