section .text

GLOBAL testInvalidExc

testInvalidExc:
    push rbp
    mov rbp, rsp
    ud2				; lanza un error de iop
    mov rsp, rbp
	pop rbp
    ret