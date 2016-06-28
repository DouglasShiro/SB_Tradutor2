
FLAG: 
global _start 
section .text 
_start: 
	mov	esi, 0 
	add	eax, [B + ESI*4] 
	mov	esi, 0 
	mov	eax, [B + ESI*4] 
	mov	esi, 0 
	mul	dword	[H + ESI*4] 
	mov	esi, 0 
	div	dword	[DOIS + ESI*4] 
	mov	esi, 0 
	mov	dword	[R + ESI*4], eax 
	mov eax, 1 
	mov ebx, 0 
	int 80h 

section .data 
B 	dw	0 
H 	dw	0 
R 	dw	0 
DOIS 	EQU	2 