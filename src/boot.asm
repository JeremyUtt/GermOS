; https://grandidierite.github.io/bios-interrupts/
; https://www.asciitable.com/
; https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og
; https://stackoverflow.com/questions/48608423/what-do-square-brackets-mean-in-x86-assembly
[bits 16]
SECTION .btext
%include "src/macros.S"

global _start
_start:

mov [BOOT_DISK], dl               

%ifdef TEXT_MODE
setTtyMode:
	mov ah, 0x0	; set Video mode 
	mov al, 0x3 ; 80 x 25	Color text	CGA, EGA, VGA
	int 0x10	; Video Services
 	printR "Set Video Mode to text"
;end setTtyMode
%endif

%ifndef TEXT_MODE
setMediumGuiMode:
	mov ah, 0x0	; set Video mode 
	mov al, 0x13 ; VGA 320x200
	int 0x10	; Video Services
	printR "Set Video Mode to GUI"
;end setMediumGuiMode
%endif

printR 0ah
printR 0dh

printR "Loading memory from disk"
printR 0ah
printR 0dh

loadKernelFromDisk:
	mov ah, 2 ; Read Sectors Into Memory
	mov al, 65; Number of Sectors to read
	mov ch, 0 ; Starting Cylinder Number
	mov cl, 2 ; Starting Sector number
	mov dh, 0 ; Head Number
	; dl = Drive number (Already set)
	mov bx, KERNEL_LOCATION
	int 0x13 ; add more disk interrupt
	jc disk_error

	jmp noerror
	disk_error:
		printR "Error"
		jmp $
	noerror:


;end load more disk


; mov si, DAPACK		; address of "disk address packet"
	; mov ah, 0x42		; AL is unused
	; mov dl, 0x80		; drive number 0 (OR the drive # with 0x80)
	; int 0x13

; pciCheck:
; 	mov ax, 0xb101
; 	int 0x1A
; 	push ax
; 	mov ah, 0x03
; 	mov al, "A"
; 	int 0x10
; ;end pcicheck

printR "Loading GDT"
printR 0ah
printR 0dh
printR "Switching to Protected Mode"
printR 0ah
printR 0dh

; Load the GDT table and switch to 32 bit protected mode:

loadGDT:
	cli
	lgdt [GDT_Descriptor] 

switchToProtected:
	; change last bit of cr0 to 1
	mov eax, cr0
	or eax, 1
	mov cr0, eax
	;far jump
	jmp CODE_SEG:start_protected_mode


; ====================================================================================
; 32 bit Code Section
; ====================================================================================


[bits 32]
start_protected_mode:
	[extern main]
	jmp main

; ====================================================================================
; Data Section
; ====================================================================================

KERNEL_LOCATION equ 0x7E00      
CODE_SEG equ code_descriptor - GDT_Start
DATA_SEG equ data_descriptor - GDT_Start ; equ = constants

BOOT_DISK: db 0

GDT_Start:
	null_descriptor:
		dd 0x0
		dd 0x0			; 32 bit
	code_descriptor:
		dw 0xffff		; first 16 bit of Limit (size)
		dw 0x0000		; first 16 bits of	Base (start pos)
		db 0x0			; middle 8 bits of Base (24 bit total)
		db 0b10011010 	; ppt AND Type flags
		db 0b11001111   ; other flags + Last 4 of limit
		db 0x0			; last 8 bits of base (32 total)
	data_descriptor:
		dw 0xffff
		dw 0x0
		db 0x0
		db 0b10010010
		db 0b11001111
		db 0x0

GDT_End:

global GDT_Descriptor
GDT_Descriptor:
	dw GDT_End - GDT_Start - 1 ; size
	dd GDT_Start

times 510-($-$$) db 0x00
db 0x55, 0xaa

