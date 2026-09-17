SECTION .text
[bits 32]

global asmTimerHandler
[extern schedulerTick]
asmTimerHandler:
    ; Save the interrupted registers before calling any C++ function.
    push esp
    add dword [esp], 12   ; recover ESP from before the CPU interrupt frame
    push ebp
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi

    
    ; ESP is still the snapshot base after the C++ call; recompute these
    ; caller-saved pointers instead of trusting ECX/EDX across the call.
    mov edx, esp
    lea ecx, [esp + 32]
    push edx ; pointer to the saved registers on the stack
    push ecx ; pointer to the interrupt frame (IP, cs, eflags) on the stack
    call schedulerTick
    add esp, 8
    
    mov al, 0x20
    out 0x20, al

    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    add esp, 4
    iret
