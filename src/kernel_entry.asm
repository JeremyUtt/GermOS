[bits 32]


[extern keyboardHandler]
global keyboardHandlerInt
keyboardHandlerInt:
    pushad
    cld
    call keyboardHandler
    popad
    iretd


global loadIdt
loadIdt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret


; global returnToReal
; [BITS 32]
; returnToReal:
; mov [0xb8000], byte 'D'

; cli

; mov eax, cr0
; and eax, 0x7FFFFFFF    ; clear PG bit
; mov cr0, eax

; xor     eax,eax         ; A convenient zero
; mov     cr3,eax         ; Flush the TLB

; mov eax, cr0
; and eax, 0xFFFFFFFE    ; clear PE bit
; mov cr0, eax

; jmp 0:continue

; [BITS 16]

; continue:

; mov [0xb8001], byte 'g'
; sti

; mov al, 'C'
; mov ah, 0x0E
; mov bh, 0x00
; mov bl, 0x07
; int 0x10

; jmp $
