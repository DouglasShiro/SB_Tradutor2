
global _start
section .text
_start:
	push MSG1
	call LerChar
	push MSG2
	call LerChar
	push MSG3
	call LerChar
	push MSG1
	call EscreverChar
	push MSG2
	call EscreverChar
	push MSG3
	call EscreverChar
	mov eax, 1
	mov ebx, 0
	int 80h
LerChar:
	push    ebp
	mov ebp, esp
	push    eax
	mov eax, 3
	mov ebx, 0
	mov ecx, [EBP + 8]
	mov edx, 1
	int 80h
	pop    eax
	mov esp, ebp
	pop ebp
	ret 4 
EscreverChar:
	push	ebp
	mov	ebp, esp
	push	eax
	mov	eax, 4
	mov	ebx, 1
	mov	ecx, [EBP + 8]
	mov	edx, 1
	int	80h
	pop	eax
	mov	esp, ebp
	pop	ebp
	ret	4

section .data
MSG1 	dd	0
MSG2 	dd	0
MSG3 	dd	0
