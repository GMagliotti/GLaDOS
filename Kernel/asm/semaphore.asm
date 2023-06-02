GLOBAL enter_region
GLOBAL leave_region
EXTERN printf
EXTERN sem_whiff

section .text
enter_region:
    CLI
    MOV  rax, QWORD 0x0
    XCHG rax, QWORD [rdi]
    CMP  rax, QWORD 0x0
    JNE  .success
    PUSH rdi 
    MOV  rdi, rsi
    CALL sem_whiff
    POP  rdi
    STI
    JMP  .whiff

.success:
    STI
    RET                  ;Requires process list to be implemented in semaphore.c.

.whiff:
    INT 20h
    JMP enter_region


leave_region:
    PUSH rbp
    MOV  rbp, rsp
    MOV  rax, 0x1
    MOV  QWORD [rdi], rax
    MOV  rsp, rbp
    POP  rbp
    RET

section .data
    ; I love NASM :)
    fmt db "ADDR: %x / VALUE: %x\n"