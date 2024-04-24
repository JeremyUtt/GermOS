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
 	; printR "Set Video Mode to text"
;end setTtyMode
%endif

%ifndef TEXT_MODE
setMediumGuiMode:
	mov ah, 0x0	; set Video mode 
	mov al, 0x13 ; VGA 320x200
	int 0x10	; Video Services
	printR "Set Video Mode to GUI"
; ;end setMediumGuiMode


; setVesaGuiMode:
; 	mov ax, 0x4f01
; 	mov cx, 0x107
; 	mov di, 0x500
; 	int 0x10
	
	; mov ax, 0x4f02
	; mov bx, 0x107
	; int 0x10
	; cmp ax, 0x4f
	; je vesaError
	; jmp done
	; vesaError:
	; printR "vesaEr"
	
	; done:
	; printR "Set Video Mode to GUI"
%endif


; printR 0ah
; printR 0dh

; printR "Loading memory from disk"
; printR 0ah
; printR 0dh



loadKernelFromDisk:
	; set ds and es to 0
	xor ax, ax
	mov ds, ax
	mov es, ax

	mov ax, 35
	mov bx, KERNEL_LOCATION ; Destination Offset
	mov cl, 2 				; Starting Sector number
	mov ch, 0 				; Starting Cylinder Number
	
	disk_loop:
		push ax
		; printR "A"
		
		mov ah, 2 				; Read Sectors Into Memory
		mov al, 1				; Number of Sectors to read
		mov dh, 0 				; Head Number
		; mov dl, 0 			; Drive number
		int 0x13 				; add more disk interrupt
	
		jc disk_error1
		
		jmp noerror
		disk_error1:
		call disk_error
		noerror:
		
		inc cl
		cmp cl, 37
		jnz no_inc
		mov cl, 1
		inc ch
		
		; cmp ch, 80
		; jnz no_inc
		; mov ch, 0
		; inc dh
		no_inc:

		; add 0x20 to es
		; equix to adding 0x200 to bx
		; (add one segment of memory to counter)
		mov ax, es
		add ax, 0x20
		mov es, ax
		
		pop ax
		dec ax
		jnz disk_loop

		

;end load more disk

; mov ax, 50
; testloop:
; printR "he"
; dec ax
; jnz testloop

; jmp $

printR "Loading GDT"
; printR 0ah
; printR 0dh

; Load the GDT table and switch to 32 bit protected mode:
loadGDT:
	cli
	lgdt [GDT_Descriptor] 

switchToProtected:
	; change last bit of cr0 to 1
	
	; printR "Switching to Protected Mode"
	; printR 0ah
	; printR 0dh

	mov eax, cr0
	or eax, 1
	mov cr0, eax
	;far jump
	; jmp $
	jmp CODE_SEG:start_protected_mode



; ========== Functions ========

disk_error:
	push ax
	printR "Error"
	
	; mov al, ah
	; mov ah, 0x0e ;for bios interrupt
	; int 10h 
	pop ax
	ret




; ====================================================================================
; 32 bit Code Section
; ====================================================================================


[bits 32]
start_protected_mode:
	[extern main]
	mov esp, 0x7bFF
	; mov esp, 0x9000
	; jmp $
	jmp main

; ====================================================================================
; Data Section
; ====================================================================================

KERNEL_LOCATION equ 0x7E00      
KERNEL_TWO equ 10000
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

