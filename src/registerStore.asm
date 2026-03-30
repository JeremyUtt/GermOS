SECTION .text

[bits 32]
global storeState
storeState:
    push ebp             ; Save the caller's EBP
    mov ebp, esp         ; Set EBP to the current ESP to create a new stack frame
    push eax

    ; Access the first parameter (int a) at [ebp + 8]
    ; Access the second parameter (int b) at [ebp + 12]
    ; (4 bytes for EBP, 4 bytes for return address)

    push ebx
    mov ebx, eax
    mov eax, [ebp + 8]   ; Load 'a' into EAX
    mov [eax + 0], ebx ; load original eax
    pop ebx
    mov [eax + 4], ecx
    mov [eax + 8], edx
    mov [eax + 12], ebx
    mov [eax + 16], esp
    mov [eax + 20], ebp
    mov [eax + 24], esi
    mov [eax + 28], edi
    
    push ebx ; save original ebx just in case
    
    mov ebx, [ebp + 8] ; interrupts return address
    mov [eax + 32], ebx ; interrupts return address store to memory
    
    pop ebx

    mov [eax + 36], dword 0xffffffff
    mov [eax + 40], cs
    mov [eax + 44], ss
    mov [eax + 48], ds
    mov [eax + 52], es
    mov [eax + 46], fs
    mov [eax + 50], gs

    pop eax
    pop ebp              ; Restore the caller's EBP
    ret                  ; Return to the caller; caller cleans the stack

