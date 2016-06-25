section .data

section .bss
letra resb 1

global _start
section .text
_start:

    push letra

    call LerChar

    call EscreverChar

FIM:
    mov eax, 1
    mov ebx, 0
    int 80h


;Le um caractere em ASCII
LerChar:
    ;frame de pilha
    push    ebp
    mov ebp, esp

    ;salva os registradores usados
    push    eax
    push    ebx
    push    ecx
    push    edx

    mov eax, 3 ;Le do teclado
    mov ebx, 1
    mov ecx, [EBP + 8] ; argumento
    mov edx, 1  ; 1 byte de leitura
    int 80h

    ;retorna ao valor anterior
    pop    eax
    pop    ebx
    pop    ecx
    pop    edx

    mov esp, ebp
    pop ebp

    ret

;Escreve um caractere em ASCII
EscreverChar:
    ;frame de pilha
    push    ebp
    mov ebp, esp

    ;salva os registradores usados
    push    eax
    push    ebx
    push    ecx
    push    edx

    mov eax, 4 ;Le do teclado
    mov ebx, 0
    mov ecx, [EBP + 8] ; argumento
    mov edx, 1  ; 1 byte de leitura
    int 80h

    ;retorna ao valor anterior
    pop    eax
    pop    ebx
    pop    ecx
    pop    edx

    mov esp, ebp
    pop ebp

    ret
