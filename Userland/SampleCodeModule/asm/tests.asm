section .text

GLOBAL test_invalid_exc

test_invalid_exc:
    push rbp
    mov rbp, rsp
    ud2				; throws IOC error
    mov rsp, rbp
	pop rbp
    ret