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
; xmm0 = limiteInferior (float)
moverEnemigoAbajo:
    mov rax, rsi                    ; rax = puntero a position
    movss xmm2, xmm0                ; xmm2 = limiteInferior (guardar)
    cvtsi2ss xmm0, edi              ; convertir distance (int) a float
    movss xmm1, dword [rax + 4]     ; xmm1 = position.y
    addss xmm1, xmm0                ; position.y += distance
    
    ; Verificar si superó el límite inferior
    ucomiss xmm1, xmm2              ; comparar position.y con limiteInferior
    jbe .moverAbajo                 ; si position.y <= limite, mover
    movss xmm1, xmm2                ; si superó, poner en el límite
.moverAbajo:
    movss dword [rax + 4], xmm1     ; guardar position.y
    ret

; Sección para eliminar warning del linker sobre stack ejecutable
section .note.GNU-stack noalloc noexec nowrite progbits