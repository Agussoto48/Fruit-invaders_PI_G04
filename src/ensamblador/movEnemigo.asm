section .data
    vel_enemigo: dd 1.0

section .text
global moverEnemigo
global moverEnemigoAbajo

; void moverEnemigo(int direction, Vector2* pos)
; edi = direction (int)
; rsi = pos (Vector2*)
moverEnemigo:
    mov rax, rsi                    ; rax = puntero a position
    cvtsi2ss xmm0, edi              ; convertir direction (int) a float
    movss xmm1, dword [rax]         ; xmm1 = position.x
    addss xmm1, xmm0                ; position.x += direction
    movss dword [rax], xmm1         ; guardar position.x
    ret

; void moverEnemigoAbajo(int distance, Vector2* pos)
; edi = distance (int)
; rsi = pos (Vector2*)
moverEnemigoAbajo:
    mov rax, rsi                    ; rax = puntero a position
    cvtsi2ss xmm0, edi              ; convertir distance (int) a float
    movss xmm1, dword [rax + 4]     ; xmm1 = position.y (offset +4 bytes)
    addss xmm1, xmm0                ; position.y += distance
    movss dword [rax + 4], xmm1     ; guardar position.y
    ret

; Sección para eliminar warning del linker sobre stack ejecutable
section .note.GNU-stack noalloc noexec nowrite progbits