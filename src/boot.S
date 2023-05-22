; https://grandidierite.github.io/bios-interrupts/
; https://www.asciitable.com/
; https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og
; https://stackoverflow.com/questions/48608423/what-do-square-brackets-mean-in-x86-assembly


; ====================================================================================
; 16 bit Code Section
; ====================================================================================

%macro print 1
	mov bh, 0x00 ; offset
	mov bl, 10
	; mov bh, 10
	loop1:
	mov al, [variableName+bx]
	cmp al, 0x00
	je end
	mov ah, 0x0e ; Write Character
	int 0x10
	inc bh
	jmp loop1
	end:

	variableName:
	db %1, 0
%endmacro



[org 0x7c00]                      

; Section _main:
; .text

mov [BOOT_DISK], dl               


; setTtyMode:
; 	mov ah, 0x0	; set Video mode 
; 	mov al, 0x3 ; 80 x 25	Color text	CGA, EGA, VGA
; 	int 0x10	; Video Services
; ; end setTtyMode


; ; setGuiMode:
; 	mov ax, 0x4f02 ; VESA set display mode
; 	mov bx, 0x0107 ; 1280×1024 256bit color pallet
; 	int 0x10
; ; end setGuiMode

setOldGuiMode:
	mov ah, 0x0	; set Video mode 
	mov al, 0x13 ; VGA 320*200 256 color
	int 0x10	; Video Services
;end setOldGuiMode

; setMediumGuiMode:
; 	mov ah, 0x0	; set Video mode 
; 	mov al, 0x10 ; VGA 640 x 350
; 	int 0x10	; Video Services
; ;end setMediumGuiMode



; print "Loading memory from disk"

loadKernelFromDisk:
	mov ah, 2 ; Read Sectors Into Memory
	mov al, 54; Number of Sectors to read
	mov ch, 0 ; Starting Cylinder Number
	mov cl, 2 ; Starting Sector number
	mov dh, 0 ; Head Number
	; dl = Drive number (Already set)
	mov bx, KERNEL_LOCATION
	int 0x13 ; add more disk interrupt

;end load more disk

; pciCheck:
; 	mov ax, 0xb101
; 	int 0x1A
; 	push ax
; 	mov ah, 0x03
; 	mov al, "A"
; 	int 0x10
; ;end pcicheck


; int 0x11
; mov bx, ax

; print "Hello"


; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1000]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1001]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1002]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1003]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1004]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1005]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1006]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1007]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1008]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x1009]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x100a]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x100b]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x100b]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x100c]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x100d]
; int 0x10
; mov ah, 0x0e ; Write Character
; mov bl, 15
; mov al, [0x100f]
; int 0x10

; jmp skip



; skip:








; Load the GDT table and switch to 32 bit protected mode:
cli
lgdt [GDT_Descriptor] 
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
; video memory start: 0xb8000
; first byte: character
; second byte: color

;mov al, 'A'
;mov ah, 0x0f
;mov [0xb8000], ax 
;jmp $

jmp KERNEL_LOCATION
jmp $

; ====================================================================================
; Data Section
; ====================================================================================
BOOT_DISK: db 0

KERNEL_LOCATION equ 0x1000      
CODE_SEG equ code_descriptor - GDT_Start
DATA_SEG equ data_descriptor - GDT_Start ; equ = constants

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

GDT_Descriptor:
	dw GDT_End - GDT_Start - 1 ; size
	dd GDT_Start


times 510-($-$$) db 0x00
db 0x55, 0xaa

