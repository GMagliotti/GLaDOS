
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL initialize_stack

EXTERN hvdPrintChar
EXTERN hvdClear
EXTERN terminate
EXTERN guruMeditation
EXTERN scheduler


GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler
GLOBAL _irq60Handler

GLOBAL _exception0Handler
GLOBAL _exception6Handler
GLOBAL _exception13Handler

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN printCurrentRegisters

SECTION .text

%macro dumpState 0
	; We use the rbp register as a temporary "mover"
	mov [registerDump.dumprbp], rbp
	mov rbp, [rsp]
	mov [registerDump.dumpr15], rbp
	mov rbp, [rsp+8]
	mov [registerDump.dumpr14], rbp
	mov rbp, [rsp+16]
	mov [registerDump.dumpr13], rbp
	mov rbp, [rsp+24]
	mov [registerDump.dumpr12], rbp
	mov rbp, [rsp+32]
	mov [registerDump.dumpr11], rbp
	mov rbp, [rsp+40]
	mov [registerDump.dumpr10], rbp
	mov rbp, [rsp+48]
	mov [registerDump.dumpr9], rbp
	mov rbp, [rsp+56]
	mov [registerDump.dumpr8], rbp
	mov rbp, [rsp+64]
	mov [registerDump.dumprsi], rbp
	mov rbp, [rsp+72]
	mov [registerDump.dumprdi], rbp
	mov rbp, [rsp+88]
	mov [registerDump.dumprdx], rbp
	mov rbp, [rsp+96]
	mov [registerDump.dumprcx], rbp
	mov rbp, [rsp+104]
	mov [registerDump.dumprbx], rbp
	mov rbp, [rsp+112]
	mov [registerDump.dumprax], rbp
	mov rbp, [rsp+120]
	mov [registerDump.dumprip], rbp
	mov rbp, [rsp+128]
	mov [registerDump.dumpcs], rbp
	mov rbp, [rsp+136]
	mov [registerDump.dumprfl], rbp
	mov rbp, [rsp+144]
	mov [registerDump.dumprsp], rbp
	mov rbp, [rsp+152]
	mov [registerDump.dumpss], rbp
	mov rbp, [registerDump.dumprbp]
%endmacro

%macro pushState 0
	push rax
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
	pop rax 
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro de irq code
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro exceptionHandler 1
	pushState
	dumpState
	
	mov rax, [rsp+120]
	cmp rax, userlandStartAddress ; Check if error happened within Kernel bounds
	jge .nopanic
	call guruMeditation


	.nopanic: mov rdi, %1 ; pasaje de parametro
	mov rsi, registerDump ; Struct de registros dumpeados
	call exceptionDispatcher

	popState
	; TODO change with shell initializer function later
	mov rax, userlandStartAddress
	mov [rsp], rax
	iretq
%endmacro

GLOBAL test
test:
	mov rax, rcx
	ret

; rdi = void * rsp
; rsi = char** argv
; rdx = int argc 
; rcx = void (*fn)(int, char **)
; r8 = void (*fn)(int, char **, void (*fn)(int, char **))
initialize_stack:
	;pop rbx		; Direccion de retorno a funcion create_process()
	;push rdx		; ARGC
	;push rsi		; ARGV	
	mov rax, 0h 
	mov [rdi+152], rax	 
	;push QWORD 0h 	; Stack Segment (Hardcoded, cambiar)
	mov [rdi+144], rdi
	;push rdi		; RSP
	mov rax, 202h
	mov [rdi+136], rax
	;push QWORD 202h ; RFLAGS (Hardcoded, cambiar)
	mov rax, 8h
	mov [rdi+128], rax
	;push QWORD 8h	; Code Segment (Hardcoded, cambiar)
	mov [rdi+120], r8
	;push rcx		; Puntero al init
	;pushState		; El resto, porque popState se llama en int
	mov [rdi+112], rax
	;push QWORD 1h
	mov [rdi+104], rbx
	;push QWORD 2h
	mov [rdi+96], r8	; ip
	;push QWORD 3h
	mov [rdi+88], rcx	; rdx - guardo el 3 arg
	;push QWORD 4h
	mov [rdi+80], rsi 	; rbp - guardo el 2 arg
	;push QWORD 5h
	mov [rdi+72], rdx	; rdi - guardo el 1 arg
	;push QWORD 6h
	mov [rdi+64], rsi
	;push QWORD 7h
	mov r8, 8h
	mov [rdi+56], r8
	;push QWORD 8h
	mov r9, 9h
	mov [rdi+48], r9
	;push QWORD 9h
	mov r10, 10h
	mov [rdi+40], r10
	;push QWORD 10h
	mov r11, 11h
	mov [rdi+32], r11
	;push QWORD 11h
	mov r12, 12h
	mov [rdi+24], r12
	;push QWORD 12h
	mov r13, 13h
	mov [rdi+16], r13
	;push QWORD 13h
	mov r14, 14h
	mov [rdi+8], r14
	;push QWORD 14h
	mov r15, 15h
	mov [rdi], r15
	;push rax		; Direccion de retorno
	ret
	
_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:

	pushState

	mov rdi, 0h ; pasaje de parametro de irq code
	call irqDispatcher

	mov rdi, rsp
	call scheduler
	mov rsp, rax

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

;Keyboard
_irq01Handler:
	pushState

	mov rdi, 1h ; pasaje de parametro de irq code
	call irqDispatcher
	cmp eax, 0
	jz .eoi
	dumpState

	; signal pic EOI (End of Interrupt)
	.eoi: mov al, 20h
	out 20h, al

	popState
	iretq

;Cascade pic never called
_irq02Handler:
	irqHandlerMaster 2h

;Serial Port 2 and 4
_irq03Handler:
	irqHandlerMaster 3h

;Serial Port 1 and 3
_irq04Handler:
	irqHandlerMaster 4h

;USB
_irq05Handler:
	irqHandlerMaster 5h

;User-called syscalls
_irq60Handler:
	;pushState -> no usare push y pop state porque quiero retornar un valor en rax
	;			ademas al ser una interrupcion de syscall (a pedido del user) no es necesario preservar todos los registros
	push rbx
	push r12
	push r13
	push r14
	push r15
	push rbp

	mov rbp, rsp
	mov r9, r8
	mov r8, rcx
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, 60h ; pasaje de parametro de irq code
	call irqDispatcher
	
	mov rsp, rbp
	;popState
	pop rbp
	pop r15
	pop r14
	pop r13
	pop r12
	pop rbx
	iretq

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid OpCode Exception
_exception6Handler:
	exceptionHandler 6

;General Protection Fault
_exception13Handler:
	add rsp, 8 			
	; ^ Necessary, as exception 13 (and a few others) push an error code into the stack
	; Not doing so will most likely pop the wrong values when iretq executes
	exceptionHandler 13

haltcpu:
	cli
	hlt
	ret

GLOBAL printRegisters
printRegisters:
	pushState
	dumpState
	mov rdi, registerDump
;	call printCurrentRegisters
;	popState
;	iretq


SECTION .bss
	aux resq 1

	GLOBAL registerDump
	registerDump:
	.dumprax resq 1
	.dumprbx resq 1
	.dumprcx resq 1
	.dumprdx resq 1
	.dumprsi resq 1
	.dumprdi resq 1
	.dumprsp resq 1
	.dumprbp resq 1
	.dumpr8  resq 1
	.dumpr9  resq 1
	.dumpr10 resq 1
	.dumpr11 resq 1
	.dumpr12 resq 1
	.dumpr13 resq 1
	.dumpr14 resq 1
	.dumpr15 resq 1
	.dumpss  resq 1
	.dumpcs  resq 1
	.dumprfl resq 1
	.dumprip resq 1

SECTION .data
	userlandStartAddress equ 0x400000