%macro printR 1
%ifdef TEXT_MODE
%%printer:
    mov si, %%text_string ;set source index to begining of text_string

%%repeat:
    ;I'm trying to emulate the behavior of lodsb to learn how it works:
    mov al, [si] ;put character at si in al register
    add si, 1 ;increment source index

    cmp al, 0 ;if the character is a zero (end of the string)
    je %%done ;end execution

    mov bx, 0x00ff
    mov ah, 0x0e ;for bios interrupt
    int 10h ;bios interrupt to put character on screen
    jmp %%repeat
    ; section .bdata
    %%text_string db %1, 0
    ; section .btext
%%done:
%endif
%endmacro
