section .data
    vel_enemigo: dd 1.0

section .text
global movimientoEnemigo

; Función única que maneja ambos movimientos de enemigos
; Parámetros:
;   edi = tipo de movimiento (0: horizontal, 1: abajo)
;   esi = parámetro int (direction para horizontal, distance para abajo)
;   rdx = puntero a Vector2
;   xmm0 = limiteInferior (float, solo para tipo 1)
;   xmm1 = velocidad multiplicador (float, nuevo parámetro)
; Retorna en EAX: 0 = normal, 1 = alcanzó límite inferior (GAME OVER)

movimientoEnemigo:
    test edi, edi                   ; verificar tipo
    jz .horizontal
    
    ; === MOVIMIENTO ABAJO ===
.abajo:
    push rbx                        ; preservar rbx
    mov rbx, rdx                    ; rbx = puntero a Vector2
    movss xmm2, xmm0                ; xmm2 = limiteInferior (backup)
    cvtsi2ss xmm0, esi              ; xmm0 = distance como float
    movss xmm3, xmm1                ; xmm3 = velocidad (backup)
    mulss xmm0, xmm3                ; distance *= velocidad
    movss xmm1, dword [rbx + 4]     ; xmm1 = position.y actual
    addss xmm1, xmm0                ; position.y += distance * velocidad
    
    ; Verificar si alcanzó/superó el límite inferior
    ; Si position.y >= limiteInferior, entonces GAME OVER
    comiss xmm1, xmm2               ; comparar y con límite
    jb .guardar_y_normal            ; si y < límite, continuar normal
    
    ; ALCANZÓ EL LÍMITE - GAME OVER
    movss xmm1, xmm2                ; ajustar y al límite exacto
    movss dword [rbx + 4], xmm1     ; guardar y
    mov eax, 1                      ; retornar 1 (game over)
    pop rbx
    ret
    
.guardar_y_normal:
    movss dword [rbx + 4], xmm1     ; guardar y
    xor eax, eax                    ; retornar 0 (todo bien)
    pop rbx
    ret
    
    ; === MOVIMIENTO HORIZONTAL ===
.horizontal:
    mov rax, rdx                    ; rax = puntero a Vector2
    cvtsi2ss xmm0, esi              ; xmm0 = direction como float
    mulss xmm0, xmm1                ; direction *= velocidad
    movss xmm2, dword [rax]         ; xmm2 = position.x actual
    addss xmm2, xmm0                ; position.x += direction * velocidad
    movss dword [rax], xmm2
    xor eax, eax                    ; retornar 0 (todo bien)
    ret

section .note.GNU-stack noalloc noexec nowrite progbits