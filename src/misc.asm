; Miscellaneous functions that need to be written in asm


[bits 32]
global loadIdt
loadIdt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret