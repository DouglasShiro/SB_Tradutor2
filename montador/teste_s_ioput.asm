global _start
section .text
_start:

s_input_ini:
    sub esi, esi
s_input:
    mov eax, 3
    mov ebx, 0
    mov ecx, char
    mov edx, 1
    int 80h

    cmp byte [char], 0x0A ; '\n' fim de numeros digitados
    je s_input_fim

    mov ecx, 0
    mov cl, [char]
    mov byte [string+esi], cl ; armazena char digitado na string
    inc esi
    jmp s_input

s_input_fim:
    mov cl, 0x00
    mov [string+esi], cl ; insere '\0' fim arquivo

s_output_ini:
    sub esi, esi
s_output:
    mov ecx, 0
    mov cl, [string+esi]
    mov [char], cl
    inc esi

    mov eax, 4
    mov ebx, 1
    mov ecx, char
    mov edx, 1
    int 80h

    cmp byte [string+esi], 0x00
    jne s_output

done:
    mov eax, 1
    mov ebx, 0
    int 80h

section .data
    string db 0
    char db 0
