; https://grandidierite.github.io/bios-interrupts/
; https://www.asciitable.com/
; https://www.youtube.com/channel/UCuWLGQB4WRBKvW1C26zA2og
; https://stackoverflow.com/questions/48608423/what-do-square-brackets-mean-in-x86-assembly


; ====================================================================================
; 16 bit Code Section
; ====================================================================================


section .btext 
    global _main:
; _main:


; %macro print 1
	
; 	mov cx, 0x00 ; offset
; 	; mov bl, 10
; 	; mov bh, 10
; 	loop1:
; 	mov al, [variableName+cx]
; 	cmp al, 0x00
; 	je end
; 	mov ah, 0x0e ; Write Character
; 	int 0x10
; 	inc cx
; 	jmp loop1


; ;     mov     si, HELLO_MSG
; ;     xor     bx, bx
; ; Next:
; ;     mov     al, byte [si + bx]
; ;     mov     ah, 0x0e
; ;     int     0x10

; ;     mov     al, 10
; ;     mov     ah, 0x0e
; ;     int     0x10

; ;     inc     bx
; ;     cmp     bx, HELLO_LEN
; ;     jne     Next

	
; 	variableName db %1, 0
;  	HELLO_LEN equ  $ - variableName
	
; 	end:
; %endmacro

; setOldGuiMode:
; 	mov ah, 0x0	; set Video mode 
; 	mov al, 0x13 ; VGA 320*200 256 color
; 	int 0x10	; Video Services
; ;end setOldGuiMode

setTtyMode:
	mov ah, 0x0	; set Video mode 
	mov al, 0x3 ; 80 x 25	Color text	CGA, EGA, VGA
	int 0x10	; Video Services
; end setTtyMode

; setGuiMode:
; 	mov ax, 0x4f02 ; VESA set display mode
; 	mov bx, 0x0107 ; 1280×1024 256bit color pallet
; 	int 0x10
; ; end setGuiMode


mov ah, 0x0e ; Write Character
mov al, 'h'
int 0x10

; print "Hello"



; print "Loading memory from disk"

loadKernelFromDisk:
	mov [BOOT_DISK], dl               
	mov ah, 2 ; Read Sectors Into Memory
	mov al, 54; Number of Sectors to read
	mov ch, 0 ; Starting Cylinder Number
	mov cl, 2 ; Starting Sector number
	mov dh, 0 ; Head Number
	; dl = Drive number (Already set)
	mov bx, KERNEL_LOCATION
	int 0x13 ; add more disk interrupt

;end load more disk


jmp $

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
