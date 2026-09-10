SECTION .text
[bits 32]


; Normally:
    ; Access the first parameter (int a) at [ebp + 8]
    ; Access the second parameter (int b) at [ebp + 12]
    ; (4 bytes for EBP, 4 bytes for return address)



global storeState
storeState:
    push eax


    push ebx
    mov ebx, eax
    ; Special case:
    ; Access the first parameter (int a) at [esp - 12] (since we pushed EAX and EBX onto the stack)
    mov eax, [esp + 12]   ; Load 'a' (first parameter, in this case a pointer to struct) into EAX
    mov [eax + 0], ebx ; load original eax
    pop ebx
    mov [eax + 4], ebx
    mov [eax + 8], ecx
    mov [eax + 12], edx
    mov [eax + 16], esp
    mov [eax + 20], ebp
    mov [eax + 24], esi
    mov [eax + 28], edi
    
    push ebx ; save original ebx just in case
    
    mov ebx, [ebp + 4] ; interrupts return address
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
    ret                  ; Return to the caller


global asmTimerHandler
[extern incTimer]
[extern determineIfSwitchNeeded]
[extern currentState]
asmTimerHandler:
    push ebp             ; Save the caller's EBP
    mov ebp, esp         ; Set EBP to the current ESP to create a new stack frame
    
    ; --- Main function:
    push eax

    
    call determineIfSwitchNeeded ; Call the C++ function to determine if a context switch is needed
    test eax, eax                ; Test the return value (eax) to see if it's zero
    jz noSwitch                  ; If zero (no switch needed), jump to noSwitch label

    push currentState ; Push the address of currentState onto the stack
    call storeState   ; Call the storeState function to save the CPU state to currentState 
    pop eax           ; pop currentState address to clean stack (eax unused)

    noSwitch:
    call incTimer          ; Call the C++ function to increment the timer counter

    mov al, 0x20          ; Prepare to send End of Interrupt (EOI) signal to the PIC
    out 0x20, al          ; Send End of Interrupt (EOI) signal to the PIC
    
    pop eax
    ; --- END of main function

    pop ebp              ; Restore the caller's EBP
    iret                  ; Return to the caller;