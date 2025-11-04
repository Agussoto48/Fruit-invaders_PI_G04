; ==============================================================
; int asm_obstacle_emit_positions(float base_x, float base_y,
;                                 int rows, int cols,
;                                 const int* const* row_ptrs,
;                                 float* out_xy, int out_cap);
;
; Convención SysV AMD64:
;   xmm0 = base_x (float)
;   xmm1 = base_y (float)
;   rdi  = rows
;   rsi  = cols
;   rdx  = row_ptrs (const int**)
;   rcx  = out_xy   (float*)
;   r8d  = out_cap
; Retorna en eax: count (pares escritos)
; ==============================================================

global asm_obstacle_emit_positions

section .rodata
align 4
kThree: dd 0x40400000     ; 3.0f

section .text
asm_obstacle_emit_positions:
    push    rbp
    mov     rbp, rsp
    push    rbx
    push    r12
    push    r13
    push    r14
    push    r15

    ; Mapear argumentos
    mov     ebx, edi       ; rows -> ebx
    mov     r15d, esi      ; cols -> r15d
    mov     r14, rdx       ; row_ptrs -> r14
    mov     r13, rcx       ; out_xy -> r13
    mov     r9d, r8d       ; out_cap -> r9d

    xor     r12d, r12d     ; count = 0
    xor     r10d, r10d     ; i = 0 (fila)

.row_loop:
    cmp     r10d, ebx
    jge     .done

    ; row_ptr = row_ptrs[i]
    mov     r11, [r14 + r10*8]   ; (puntero de 8 bytes por entrada)

    xor     r8d, r8d             ; j = 0 (columna)
.col_loop:
    cmp     r8d, r15d
    jge     .next_row

    mov     eax, [r11 + r8*4]    ; valor grid[i][j]
    test    eax, eax
    jz      .skip_cell

    ; Capacidad
    cmp     r12d, r9d
    jge     .next_row

    ; x = base_x + j*3.0f
    cvtsi2ss xmm2, r8d
    mulss   xmm2, [rel kThree]
    addss   xmm2, xmm0

    ; y = base_y + i*3.0f
    cvtsi2ss xmm3, r10d
    mulss   xmm3, [rel kThree]
    addss   xmm3, xmm1

    ; out_xy[2*count] = x ; out_xy[2*count+1] = y
    mov     eax, r12d
    shl     eax, 3                 ; *8 bytes (2 floats)
    mov     rdx, r13
    add     rdx, rax

    movss   [rdx],    xmm2         ; x
    movss   [rdx+4],  xmm3         ; y

    inc     r12d

.skip_cell:
    inc     r8d
    jmp     .col_loop

.next_row:
    inc     r10d
    jmp     .row_loop

.done:
    mov     eax, r12d              ; return count

    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     rbx
    pop     rbp
    ret
