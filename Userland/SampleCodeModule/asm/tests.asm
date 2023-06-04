section .text

GLOBAL test_invalid_exc

test_invalid_exc:
    push rbp
    mov rbp, rsp
    ud2				; lanza un error de iop
    mov rsp, rbp
	pop rbp
    ret