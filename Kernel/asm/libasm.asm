GLOBAL cpuVendor
GLOBAL accessRTC
GLOBAL getKey
GLOBAL generateRandSeed
GLOBAL inb
GLOBAL outb
GLOBAL memoryAt

section .text


;----------------------------------------------------------------------
; uint8_t accessRTC(uint32 register)
; accessRTC - Accesses the Real-Time Clock and returns the value of the
; register passed as a parameter
; 
; PARAMETERS: edi / register - The register from the RTC to access
; Do note there is a full list of registers available in lib.h
;
; RETURNS: The value in the register passed as parameter
;----------------------------------------------------------------------
accessRTC:
	push rbp
	mov rbp, rsp

	mov eax, edi
	out 70h, al
	in 	al, 71h
	
	mov rsp, rbp
	pop rbp
	ret

;----------------------------------------------------------------------
; uint8_t getKey()
; getKey - Acceses the keyboard and waits for it to send a scan code
;
; RETURNS: Scan code of the action performed
;----------------------------------------------------------------------

getKey:
        push rbp
        mov rbp, rsp
      
        mov rax,0
.loop:  in al,0x64       
    	mov cl,al
        and al,0x01       
    	cmp al,0
        je .loop
    	in al,0x60
       
    	mov rsp, rbp
    	pop rbp
    	ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

inb:				; Funciones para el correcto funcionamiento del soundDriver
	push rbp
	mov rbp, rsp

    mov rdx,rdi
    in al,dx		; pasaje en 8 bits

	mov rsp, rbp
	pop rbp
	ret

outb:
	push rbp
	mov rbp, rsp

    mov rax, rsi    
    mov rdx, rdi
	out dx, al		; pasaje en 8 bits

	mov rsp, rbp
	pop rbp
	ret

memoryAt:			; Devuelve el contenido de la dir de memoria pasada como arg
    push rbp
    mov rbp, rsp
    mov rax, [rdi]
    mov rsp, rbp
	pop rbp
    ret