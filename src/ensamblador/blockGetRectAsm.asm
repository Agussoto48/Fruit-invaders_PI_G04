; void asm_block_get_rect(float x, float y, Rectangle* out);
; x -> xmm0, y -> xmm1, out -> rdi
; Rectangle: 4 floats: x,y,width,height

global asm_block_get_rect

section .text
asm_block_get_rect:
    movss  [rdi+0],  xmm0          ; x
    movss  [rdi+4],  xmm1          ; y
    mov    dword [rdi+8],  0x40400000  ; 3.0f
    mov    dword [rdi+12], 0x40400000  ; 3.0f
    ret
