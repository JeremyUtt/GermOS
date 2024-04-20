; Miscellaneous functions that need to be written in asm

SECTION .text

[bits 32]
global loadIdt
loadIdt:
    mov edx, [esp + 4]
    lidt [edx]
    sti
    ret


[bits 32]
global vgaSwitch
vgaSwitch:
SetVideoMode:
; Switch to real mode
cli                    ; Disable interrupts
mov ax, 0x0000         ; Value for data segment registers in real mode
; mov ds, ax
; mov es, ax
; mov fs, ax
; mov gs, ax
; mov ss, ax
mov eax, cr0           ; Get the value of the control register CR0
and ax, 0xFFFE         ; Clear the PE bit to switch to real mode

mov cr0, eax           ; Set the new value of the control register CR0
jmp 0x0000:RealMode    ; Far jump to flush the CPU's instruction prefetch queue

RealMode:
[bits 16]
; Your real mode code goes here

sti
mov al, 'A'
mov ah, 0x0e
int 0x10
jmp $
jmp $

; Switch back to protected mode
cli                    ; Disable interrupts
extern GDT_Descriptor
lgdt [GDT_Descriptor]            ; Load the Global Descriptor Table register
mov eax, cr0           ; Get the value of the control register CR0
or ax, 0x0001          ; Set the PE bit to switch to protected mode
mov cr0, eax           ; Set the new value of the control register CR0
jmp 0x0000:ProtectedMode ; Far jump to flush the CPU's instruction prefetch queue

ProtectedMode:
[bits 32]
ret