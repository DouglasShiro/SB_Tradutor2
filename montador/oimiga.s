
global _start 
section .data 
 B 	resw 1 
 DOIS 	resw 1 
 H 	resw 1 
 R 	resw 1 
section .text 
_start: 
	add	eax,  [B] 
	syscall	input [B] 
	syscall	input [H] 
	mov	load [B] 
	mul	 [H] 
	div	 [DOIS] 
	mov	store [R] 
	syscall	output [R] 
	mov eax, 1 
	mov ebx, 0 
	int 80h 2 