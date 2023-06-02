#ifndef USER_SYSCALLS_H
#define USER_SYSCALLS_H

#include <stdint.h>
#include <stdlib.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

typedef struct time{
	uint8_t seconds, minutes, hours;
} timeStructT;

void call_to_sys_write(char * str, uint8_t len, uint8_t fd);
void call_to_sys_read(uint8_t fd, uint8_t * toRet, uint8_t cantChars);
void call_to_putpixel(uint32_t x, uint32_t y, uint32_t color);
void call_to_sleep(uint32_t seconds);
void call_to_setptrx(int num);
void call_to_setptry(int num);
uint64_t call_to_getvbeheight();
uint64_t call_to_getvbewidth();
void call_to_clearbuffer();
void call_to_beep();
void call_to_fillrectangle(uint16_t x, uint16_t y , uint32_t color, uint16_t w, uint16_t h);
void call_to_hlt();
int call_to_getticks();
void call_to_accessRTC(timeStructT * timeStruct);
uint8_t call_to_memoryAt(int dir);
void call_to_setSize(int newSize);
void call_to_printRegisters();

void * call_to_malloc(size_t requestedMemory);
void call_to_free(void * mem_ptr);

int call_to_getpid();
int call_to_ps();
int call_to_pkill_process(int pid);
int call_to_nice_process(int pid, int priority);
int call_to_block_process(int pid);
int call_to_create_process(int argc, char** argv, void (*fn)(int, char **), int fd[2]);
void call_to_set_print_mode();

int call_to_waitpid(int pid);

#endif