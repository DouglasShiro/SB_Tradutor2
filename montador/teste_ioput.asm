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

    sub esi, esi
output:
    mov ecx, 0
    mov cl, [integer+esi]
    ;add cl, 0x30
    mov [integer], cl
    inc esi

    mov eax, 4
    mov ebx, 1
    mov ecx, integer
    mov edx, 1
    int 80h

    cmp byte [integer], 0x00
    jne output

done:
    mov eax, 1
    mov ebx, 0
    int 80h

section .data
    integer db "1234"
