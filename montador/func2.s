section .data
msg	db "Más deeeeeeu certoooo m e s m o //?? ",0
msg1 db 0

section .bss
letra resb	1

global _start
section .text
_start:

	push msg
	call EscreverString

	push msg1
	call LerString
	call EscreverString

	push letra
 	call LerChar
    call EscreverChar

FIM:
    mov eax, 1
    mov ebx, 0
    int 80h

;-----------------------------------------------------------
;Le um caractere em ASCII
LerChar:
    ;frame de pilha
    push    ebp
    mov ebp, esp

    ;salva o acumulador
    push    eax

    mov eax, 3 ;Le do teclado
    mov ebx, 1
    mov ecx, [EBP + 8] ; argumento
    mov edx, 1  ; 1 byte de leitura
    int 80h

    ;retorna ao valor anterior
    pop    eax

    mov esp, ebp
    pop ebp

    ret
;-----------------------------------------------------------
;Escreve um caractere em ASCII
EscreverChar:
    ;frame de pilha
    push    ebp
    mov ebp, esp

    ;salva o acumulador
    push    eax

    mov eax, 4 ;Escreve na tela
    mov ebx, 0
    mov ecx, [EBP + 8] ; argumento
    mov edx, 1  ; 1 byte de leitura
    int 80h

    ;retorna ao valor anterior
    pop    eax

    mov esp, ebp
    pop ebp

    ret

;-----------------------------------------------------------
;Escrever uma string do teclado em ASCII
EscreverString:
	;frame de pilha
    push    ebp
    mov ebp, esp

	;salva o acumulador
	push    eax

	sub esi, esi
	mov	edi, [EBP + 8]
	dec edi

EscreverS:
	inc edi
	mov eax, 4
	mov ebx, 1
	mov ecx, edi
	mov edx, 1
	int 80h

	cmp byte [edi], 0
    jne EscreverS

FimEscreverS:
    ;retorna ao valor anterior
	pop eax
    mov esp, ebp
    pop ebp

    ret

;-----------------------------------------------------------
;Ler uma string do teclado em ASCII
LerString:
	;frame de pilha
    push    ebp
    mov ebp, esp

	sub esi, esi
	mov	edi, [EBP + 8]
	dec edi

LerS:
	inc edi
	inc esi
	mov eax, 3
	mov ebx, 0
	mov ecx, edi
	mov edx, 1
	int 80h

	cmp byte [edi], 0x0A
    jne LerS

FimLerS:
    ;retorna ao valor anterior
	mov eax, esi
    mov esp, ebp
    pop ebp

    ret
