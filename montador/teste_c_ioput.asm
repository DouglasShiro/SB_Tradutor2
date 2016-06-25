global _start
section .text
_start:

c_input:
    mov eax, 3
    mov ebx, 0
    mov ecx, char
    mov edx, 1
    int 80h

c_output:
    mov eax, 4
    mov ebx, 1
    mov ecx, char
    mov edx, 1
    int 80h

done:
    mov eax, 1
    mov ebx, 0
    int 80h

section .data
    char db 0
