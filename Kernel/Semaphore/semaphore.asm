GLOBAL enter_region
GLOBAL leave_region
EXTERN printf

section .text
enter_region:
    MOV  rax, QWORD 0x0
    XCHG rax, QWORD [rdi]
    CMP  rax, QWORD 0x0
    JZ   .whiff
    RET

; Si no llegamos en tiempo, comentar lo de abajo y 
; cambiar el JZ a enter_region
; Bussy waitussy

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