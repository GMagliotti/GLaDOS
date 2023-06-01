GLOBAL enter_region
GLOBAL leave_region
EXTERN printf

section .text
enter_region:
    sti
    MOV  rax, QWORD 0x0
    XCHG rax, QWORD [rdi]
    CMP  rax, QWORD 0x0
    JZ   enter_region
    RET

; JZ enter_region -> Performs busy waiting
; JZ .whiff       -> Blocks process after failing to capture the semaphore
;                    Requires process list to be implemented in semaphore.c.

.whiff:
    MOV r15, 0x2    ; Ver como "avisar" al timer tick que bloquee
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