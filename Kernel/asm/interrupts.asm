
GLOBAL _cli
GLOBAL _sti
GLOBAL pic_master_mask
GLOBAL pic_slave_mask
GLOBAL haltcpu
GLOBAL _hlt
GLOBAL initialize_stack

EXTERN hvd_printChar
EXTERN hvd_clear
EXTERN terminate
EXTERN guruMeditation
EXTERN scheduler
EXTERN kill_current_process

GLOBAL _irq00_handler
GLOBAL _irq01_handler
GLOBAL _irq02_handler
GLOBAL _irq03_handler
GLOBAL _irq04_handler
GLOBAL _irq05_handler
GLOBAL _irq60_handler

GLOBAL _exception0_handler
GLOBAL _exception6_handler
GLOBAL _exception13_handler

EXTERN irq_dispatcher
EXTERN exceptionDispatcher
EXTERN print_current_registers

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

%macro irq_handlerMaster 1
	pushState

	mov rdi, %1 ; passing of parameter irq code
	call irq_dispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro


%macro exception_handler 1
	pushState
	dumpState
	
	mov rax, [rsp+120]
	cmp rax, userlandStartAddress ; Check if error happened within Kernel bounds
	jge .nopanic
	call guruMeditation


	.nopanic: mov rdi, %1 ; passing of parameter
	mov rsi, registerDump ; Struct de registros dumpeados
	call exceptionDispatcher

	popState

	call kill_current_process
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
	;pop rbx		; Return address of function create_process()
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
	;push rax		; Return address
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

pic_master_mask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

pic_slave_mask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = 16 bit mask
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00_handler:
	pushState

	mov rdi, 0h ; passing of parameter irq code
	call irq_dispatcher

	mov rdi, rsp
	call scheduler
	mov rsp, rax

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq

;Keyboard
_irq01_handler:
	pushState

	mov rdi, 1h ; passing of parameter irq code
	call irq_dispatcher
	cmp eax, 0
	jz .eoi
	dumpState

	; signal pic EOI (End of Interrupt)
	.eoi: mov al, 20h
	out 20h, al

	popState
	iretq

;Cascade pic never called
_irq02_handler:
	irq_handlerMaster 2h

;Serial Port 2 and 4
_irq03_handler:
	irq_handlerMaster 3h

;Serial Port 1 and 3
_irq04_handler:
	irq_handlerMaster 4h

;USB
_irq05_handler:
	irq_handlerMaster 5h

;User-called syscalls
_irq60_handler:
	;pushState -> wont use push/pop state becuase we want to return a value in rax. Also, since its a syscall interruption
	;				preserving all registers isnt necessary
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
	mov rdi, 60h ; passing of parameter irq code
	call irq_dispatcher
	
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
_exception0_handler:
	exception_handler 0

;Invalid OpCode Exception
_exception6_handler:
	exception_handler 6

;General Protection Fault
_exception13_handler:
	add rsp, 8 			
	; ^ Necessary, as exception 13 (and a few others) push an error code into the stack
	; Not doing so will most likely pop the wrong values when iretq executes
	exception_handler 13

haltcpu:
	cli
	hlt
	ret

GLOBAL print_registers
print_registers:
	pushState
	dumpState
	mov rdi, registerDump
;	call print_current_registers
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