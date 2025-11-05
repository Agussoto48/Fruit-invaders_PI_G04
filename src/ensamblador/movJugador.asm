section .data
    vel: dd 7.0

section .text
global moverJugadorIzquierda
global moverJugadorDerecha


; void moverJugadorIzquierda(float left, Vector2* pos)
; xmm0 es el primer parametro float, entonces left
; y rdi es el primer parametro puntero, osea pos 
moverJugadorIzquierda:
    mov rax, rdi                   
    movss xmm1, dword [vel]        
    movss xmm2, dword [rax]        

    subss xmm2, xmm1                ; x -= vel
    ucomiss xmm2, xmm0             
    jae .moverIZQ

    movss xmm2, xmm0                ; si x < leftLimit, le pone el mismo valor
.moverIZQ:
    movss dword [rax], xmm2         ; guarda x nuevo
    ret


; void moverJugadorDerecha(float right, Vector2* pos)
; Misma logica que el de izquierda, pero aumentando
moverJugadorDerecha:
    mov rax, rdi                    
    movss xmm1, dword [vel]         
    movss xmm2, dword [rax]        

    addss xmm2, xmm1                ; x += vel
    ucomiss xmm2, xmm0              
    jbe .moverDER

    movss xmm2, xmm0                ; si x > rightLimit, le pone el mismo valor
.moverDER:
    movss dword [rax], xmm2
    ret