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
