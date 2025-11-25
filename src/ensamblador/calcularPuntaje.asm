; int asm_calcular_puntaje(int tipo_enemigo);
; Parámetros:
;   edi = tipo_enemigo (0=MANZANA, 1=PINA, 2=SANDIA)
; Retorna en eax: puntaje (10, 20, o 30)

global asm_calcular_puntaje

section .rodata
    align 4
    tabla_puntajes: dd 10, 20, 30  ; MANZANA, PINA, SANDIA

section .text
asm_calcular_puntaje:
    ; Validar rango [0-2]
    cmp edi, 2
    jg .invalido
    test edi, edi
    js .invalido
    
    ; Acceder a la tabla usando el tipo como índice
    lea rax, [rel tabla_puntajes]
    mov eax, dword [rax + rdi*4]  ; tabla[tipo] * 4 bytes
    ret
    
.invalido:
    xor eax, eax    ; retornar 0 si tipo inválido
    ret

section .note.GNU-stack noalloc noexec nowrite progbits