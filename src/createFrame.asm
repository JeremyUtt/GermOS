SECTION .text
[bits 32]



global createFrame:
[extern trampolineFunction]
createFrame:
    ; create stack frame with base address at param a
 
    
    push ebp             ; Save the caller's EBP
    mov ebp, esp         ; Set EBP to the current ESP to create a new stack frame

    ; mov eax, [ebp + 8]   ; Load 'a' (first parameter, in this case a pointer to struct) into EAX

    ; "jump" to new stack
    mov esp, 0xbfffc 
    mov ebp, 0xC0000

    ; push return address
    push trampolineFunction
    jmp newCode

    pop ebp              ; Restore the caller's EBP
    ret                  ; Return to the caller; caller cleans the stack



    ;represents a second "task"
newCode:
    push ebp             ; Save the caller's EBP
    mov ebp, esp         ; Set EBP to the current ESP to create a new stack frame

    mov ax, ax
    mov ax, ax
    mov ax, ax
    mov ax, ax

    pop ebp              ; Restore the caller's EBP
    ret                  ; Return to the caller; caller cleans the stack
    ; confirmed: jumps to trampolineFunction
