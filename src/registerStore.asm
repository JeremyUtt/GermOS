SECTION .text
[bits 32]

; storeState(CpuState* destination, uint32_t* interruptFrame,
;            uint32_t* registerSnapshot)
; interruptFrame: EIP, CS, EFLAGS
; registerSnapshot is the stack area after pushes, in memory order:
; EDI, ESI, EDX, ECX, EBX, EAX, EBP, original ESP
global storeState
storeState:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 8]
    mov edx, [ebp + 12]
    mov ecx, [ebp + 16]

    mov ebx, [ecx + 20]
    mov [eax + 0], ebx       ; EAX
    mov ebx, [ecx + 16]
    mov [eax + 4], ebx       ; EBX
    mov ebx, [ecx + 12]
    mov [eax + 8], ebx       ; ECX
    mov ebx, [ecx + 8]
    mov [eax + 12], ebx      ; EDX
    mov ebx, [ecx + 28]
    mov [eax + 16], ebx      ; ESP before interrupt
    mov ebx, [ecx + 24]
    mov [eax + 20], ebx      ; EBP
    mov ebx, [ecx + 4]
    mov [eax + 24], ebx      ; ESI
    mov ebx, [ecx + 0]
    mov [eax + 28], ebx      ; EDI

    mov ebx, [edx + 0]
    mov [eax + 32], ebx      ; EIP
    mov ebx, [edx + 8]
    mov [eax + 36], ebx      ; EFLAGS
    mov ebx, [edx + 4]
    mov [eax + 40], ebx      ; CS
    xor ebx, ebx
    mov bx, ss
    mov [eax + 44], ebx      ; SS
    xor ebx, ebx
    mov bx, ds
    mov [eax + 48], ebx      ; DS
    xor ebx, ebx
    mov bx, es
    mov [eax + 52], ebx      ; ES
    xor ebx, ebx
    mov bx, fs
    mov [eax + 56], ebx      ; FS
    xor ebx, ebx
    mov bx, gs
    mov [eax + 60], ebx      ; GS

    pop ebp
    ret

global asmTimerHandler
[extern incTimer]
[extern determineIfSwitchNeeded]
[extern currentState]
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

    call determineIfSwitchNeeded
    test eax, eax
    jz .noSwitch

    ; ESP is still the snapshot base after the C++ call; recompute these
    ; caller-saved pointers instead of trusting ECX/EDX across the call.
    mov edx, esp
    lea ecx, [esp + 32]
    push edx
    push ecx
    push dword currentState
    call storeState
    add esp, 12

.noSwitch:
    call incTimer
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
