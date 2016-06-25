
global _start 
section .text 
_start: 
	mov	esi, 0 
	push	eax 
	mov	eax, 4 
	mov	ebx, 1 
	mov	ecx, [A + ESI*2] 
	mov	edx, 4 
	pop	eax 
	mov	esi, 0 
	push	eax 
	mov	eax, 4 
	mov	ebx, 1 
	mov	ecx, [B + ESI*2] 
	mov	edx, 4 
	pop	eax 
	mov	esi, 0 
	mov	eax, [A + ESI*2] 
	mov	esi, 0 
	add	eax, [B + ESI*2] 
	add	eax, [B] 
	mov	esi, 0 
	mov	[R_ADD + ESI*2], eax 
	mov	esi, 0 
	mov	eax, [A + ESI*2] 
	mov	esi, 0 
	sub	eax, [B + ESI*2] 
	mov	esi, 0 
	mov	[R_SUB + ESI*2], eax 
	mov	esi, 0 
	mov	eax, [A + ESI*2] 
	mov	esi, 0 
	mul	[B + ESI*2] 
	mov	esi, 0 
	mov	[R_MUL + ESI*2], eax 
	mov	esi, 0 
	mov	eax, [A + ESI*2] 
	mov	esi, 0 
	div	[B + ESI*2] 
	mov	esi, 0 
	mov	[R_DIV + ESI*2], eax 
	mov	esi, 0 
	push	eax 
	mov	eax, 3 
	mov	ebx, 0 
	mov	ecx, [R_ADD + ESI*2] 
	mov	edx, 4 
	pop	eax 
	mov	esi, 0 
	push	eax 
	mov	eax, 3 
	mov	ebx, 0 
	mov	ecx, [R_SUB + ESI*2] 
	mov	edx, 4 
	pop	eax 
	mov	esi, 0 
	push	eax 
	mov	eax, 3 
	mov	ebx, 0 
	mov	ecx, [R_MUL + ESI*2] 
	mov	edx, 4 
	pop	eax 
	mov	esi, 0 
	push	eax 
	mov	eax, 3 
	mov	ebx, 0 
	mov	ecx, [R_DIV + ESI*2] 
	mov	edx, 4 
	pop	eax 

section .data 
A 	dw	0 
B 	dw	0 
R_ADD 	dw	0 
R_SUB 	dw	0 
R_MUL 	dw	0 
R_DIV 	dw	0 