global _start
section .text
_start:

input:
    ; mov eax, 3
    ; mov ebx, 0
    ; mov ecx, integer
    ; mov edx, 1
    ; int 80h
    ;
    ; cmp byte [integer], 0x0A
    ; je output
    ; jmp input
    ;
    ; mov ecx, 0
    ; mov cl, [integer]
    ; sub cl, 0x30
    ; mov [integer], cl

output:
    sub esi, esi
do_while:
    mov eax, integer
    mov ebx, 10
    div ebx
    mov [integer_to_char+esi], eax
    mov eax, edx
    cmp eax, 0
    je proximo
    mov [integer_to_char+esi+1],[integer_to_char+esi]
proximo:
    inc esi
    cmp eax, 0
    jne do_while
    mov byte [integer_to_char+esi], 0x00
    ;
    ; i=0
    ; do{
    ; str[i] = (char)((Valor % 10) + 0x30);
    ; Valor = (int) (Valor / 10);
    ; If Valor != 0 str[i+1]= str[i]
    ; i = i+1
    ; } while (Valor != 0)
    ; str[i] = '/0'

    ;
    ; mov eax, 4
    ; mov ebx, 1
    ; mov ecx, char
    ; mov edx, 1
    ; int 80h

    ; dps de transformar o inteiro em ascii, imprimir ascii
    s_output_ini:
        sub esi, esi
    s_output:
        mov ecx, 0
        mov cl, [integer_to_char+esi]
        mov [char], cl
        inc esi

        mov eax, 4
        mov ebx, 1
        mov ecx, char
        mov edx, 1
        int 80h

        cmp byte [integer_to_char+esi], 0x00
        jne s_output
done:
    mov eax, 1
    mov ebx, 0
    int 80h

section .data
    integer dd 1234
    integer_to_char db 0
    char db 0
