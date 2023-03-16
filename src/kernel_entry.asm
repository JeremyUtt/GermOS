[bits 32]
[extern main]
call main
jmp $
db 0x69,0x69,0x69,0x69,0x69,0x69 

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



jmp $