bits 64
default rel
global _start

section .text
_start:

	mov	rax, 1
	mov	rdi, 1
	mov	rsi, msg
	mov	rdx, 14
	syscall
	jmp	0xbabeface

msg		db "....WOODY....", 10
