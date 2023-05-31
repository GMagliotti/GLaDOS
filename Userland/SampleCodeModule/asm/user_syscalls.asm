section .text
GLOBAL call_to_setbash
GLOBAL call_to_putpixel
GLOBAL call_to_sys_write
GLOBAL call_to_sys_read
GLOBAL call_to_sleep
GLOBAL call_to_setptrx
GLOBAL call_to_setptry
GLOBAL call_to_getvbeheight
GLOBAL call_to_getvbewidth
GLOBAL call_to_clearbuffer
GLOBAL call_to_getchar
GLOBAL call_to_beep
GLOBAL call_to_fillrectangle
GLOBAL call_to_hlt
GLOBAL call_to_getticks
GLOBAL call_to_accessRTC
GLOBAL call_to_memoryAt
GLOBAL call_to_setSize
GLOBAL call_to_printRegisters
GLOBAL call_to_malloc
GLOBAL call_to_free
GLOBAL call_to_getpid
GLOBAL call_to_ps
GLOBAL call_to_loop_process
GLOBAL call_to_pkill_process
GLOBAL call_to_nice_process
GLOBAL call_to_block_process
GLOBAL call_to_create_process
GLOBAL call_to_waitpid

%macro call_to_handler 1
    push rbp
    mov rbp, rsp
    mov r9, r8      ;arg 5
    mov r8, rcx     ;arg 4
    mov rcx, rdx    ;arg 3
    mov rdx, rsi    ;arg 2
    mov rsi, rdi    ;arg 1
    mov rdi, %1     ;syscall deseada
    int 80h         ;interrupcion 80
    mov rsp, rbp
	pop rbp
    ret
%endmacro

call_to_sys_write:
    call_to_handler 1

call_to_putpixel:
    call_to_handler 3

call_to_sys_read:
    call_to_handler 4

call_to_sleep:
    call_to_handler 5

call_to_setptrx:
    call_to_handler 6

call_to_setptry:
    call_to_handler 7

call_to_getvbeheight:
    call_to_handler 8

call_to_getvbewidth:
    call_to_handler 9

call_to_getchar:
    call_to_handler 10

call_to_clearbuffer:
    call_to_handler 11

call_to_beep:
    call_to_handler 12

call_to_fillrectangle:
    call_to_handler 13

call_to_accessRTC:
    call_to_handler 14

call_to_getticks:
    call_to_handler 15

call_to_hlt:
    call_to_handler 16

call_to_memoryAt:
    call_to_handler 17

call_to_setSize:
    call_to_handler 18

call_to_printRegisters:
    call_to_handler 19

call_to_malloc:
    call_to_handler 20

call_to_free:
    call_to_handler 21

call_to_getpid:
    call_to_handler 22

call_to_ps:
    call_to_handler 23

call_to_loop_process:
    call_to_handler 24

call_to_pkill_process:
    call_to_handler 25

call_to_nice_process:
    call_to_handler 26

call_to_block_process:
    call_to_handler 27

call_to_create_process:
    call_to_handler 28

call_to_waitpid:
    call_to_handler 29