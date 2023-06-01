[bits 32]
[extern main]

; fake _start symbol to get warnings to shut up
; program actually starts in the bootloader and jumps to here (0x1000) in memory
global _start
_start:

call main


[extern keyboard_handler]
global keyboard_handler_int
keyboard_handler_int:
    pushad
    cld
    call keyboard_handler
    popad
    iretd


global load_idt
load_idt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret
