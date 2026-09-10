SECTION .text
[bits 32]


global asmfunction
asmfunction:
; infinate loop that sets general registers to a value, then calls storeState to save the state to memory
    push ebp             ; Save the caller's EBP
    mov ebp, esp         ; Set EBP to the current ESP to create a new stack frame


    loop:
    call asmfunction2
    jmp loop
    

    pop ebp              ; Restore the caller's EBP
    ret                  ; Return to the caller; caller cleans the stack


asmfunction2:
    push ebp             ; Save the caller's EBP
    mov ebp, esp         ; Set EBP to the current ESP to create a new stack frame

    push 0x69
    push 0x420
    loop2:
    jmp loop2
    pop eax
    pop eax

    pop ebp              ; Restore the caller's EBP
    ret                  ; Return to the caller; caller cleans the stack