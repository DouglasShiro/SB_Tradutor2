global _start
section .text
_start:
	mov	esi, 0
	add	eax, [B + ESI*4]
	int 80h
	push dword B
	call LerInteiro
	call LerInteiro

	push dword H
	call LerInteiro
	call LerInteiro

	mov	eax, B
	mov	esi, 0
	mov	ecx, [H + ESI*4]
	mul	ecx
	mov	esi, 0
	mov	ecx, DOIS
	div	ecx
	mov	esi, 0
	mov	dword	[R + ESI*4], eax
	push dword R
	call EscreverInteiro
	mov eax, 1
	mov ebx, 0
	int 80h
LerInteiro:
	push    ebp
	mov ebp, esp
	pusha
	mov eax, 3
	mov ebx, 0
	mov ecx, [EBP + 8]
	mov edx, 1
	int 80h
	popa
	mov esp, ebp
	pop ebp
	ret 4
EscreverInteiro:
	push    ebp
	mov ebp, esp
	pusha
	mov eax, 4
	mov ebx, 1
	mov ecx, [EBP + 8]
	mov edx, 1
	int 80h
	popa
	mov esp, ebp
	pop ebp
	ret 4

section .data
B 	dd	0
H 	dd	0
R 	dd	0
DOIS 	EQU	2
