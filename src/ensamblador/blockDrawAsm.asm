; void asm_block_draw(float x, float y);
; x -> xmm0, y -> xmm1      (SysV AMD64)
; Llama a DrawRectangle(int x, int y, int w, int h, Color c)
; int/punteros -> rdi, rsi, rdx, rcx, r8, r9
; Color {r,g,b,a} = 243,216,63,255 => 0xFF3FD8F3 (little-endian)

global asm_block_draw
extern DrawRectangle

section .text
asm_block_draw:
    ; convertir float->int
    cvttss2si  edi, xmm0       ; x -> EDI
    cvttss2si  esi, xmm1       ; y -> ESI
    mov        edx, 3          ; w
    mov        ecx, 3          ; h
    mov        r8d, 0xFFFFFFFF ; Color(r,g,b,a)

    ; Alinear stack a 16 bytes antes del call
    sub        rsp, 8
    call       DrawRectangle
    add        rsp, 8
    ret
