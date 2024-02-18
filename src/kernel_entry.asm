[bits 32]
[extern main]

; fake _start symbol to get warnings to shut up
; program actually starts in the bootloader and jumps to here (0x1000) in memory
global _start
_start:

call main
; main should never return

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


global returnToReal
[BITS 32]
returnToReal:
mov [0xb8000], byte 'D'

cli

mov eax, cr0
and eax, 0x7FFFFFFF    ; clear PG bit
mov cr0, eax

xor     eax,eax         ; A convenient zero
mov     cr3,eax         ; Flush the TLB

mov eax, cr0
and eax, 0xFFFFFFFE    ; clear PE bit
mov cr0, eax

jmp 0:continue

[BITS 16]

continue:

mov [0xb8001], byte 'g'
sti

mov al, 'C'
mov ah, 0x0E
mov bh, 0x00
mov bl, 0x07
int 0x10

jmp $
