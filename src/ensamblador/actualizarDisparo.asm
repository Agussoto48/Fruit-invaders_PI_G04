; void asm_actualizar_disparo(Vector2* position, int speed, int screenHeight, int* active);
; Parámetros:
;   rdi = position (puntero a Vector2)
;   esi = speed
;   edx = screenHeight
;   rcx = active (puntero a int/bool)

global asm_actualizar_disparo

section .text
asm_actualizar_disparo:
    ; position->y += speed
    cvtsi2ss xmm0, esi             
    movss xmm1, dword [rdi + 4]    
    addss xmm1, xmm0               
    movss dword [rdi + 4], xmm1     
    
    ; Verificar si está fuera de la pantalla
    xorps xmm2, xmm2               
    cvtsi2ss xmm3, edx             
    
    ; Si position.y > screenHeight  
    comiss xmm1, xmm3              
    ja .desactivar                
    
    ; Si position.y < 0
    comiss xmm1, xmm2              
    jb .desactivar                  
    
    ret                             
    
.desactivar:
    mov dword [rcx], 0             
    ret