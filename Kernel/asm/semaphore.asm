GLOBAL enter_region
GLOBAL leave_region
EXTERN printf
EXTERN sem_whiff

section .text
enter_region:
    MOV  rax, locked
    XCHG rax, [rdi]
    CMP  rax, locked
    JNE  .success
    PUSH rdi 
    MOV  rdi, rsi
    CALL sem_whiff
    POP  rdi
    JMP  .whiff

.success:
    ; STI
    RET                  ;Requires process list to be implemented in semaphore.c.

.whiff:
    INT 20h
    JMP enter_region


leave_region:
    PUSH rbp
    MOV  rbp, rsp
    MOV  rax, 0x00
    MOV  QWORD [rdi], rax
    MOV  rsp, rbp
    POP  rbp
    RET

section .data
    ; I love NASM :)
    fmt db "ADDR: %x / VALUE: %x\n"
    locked      equ 0x01
    unlocked    equ 0x00