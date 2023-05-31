#ifndef _SYSCALLS_H
#define _SYSCALLS_H
#define STDOUT 1
#define STDERR 2

#include <stdint.h>
#include "textModeDriver.h"
#include "textDriver.h"
#include "videoDriver.h"
#include "lib.h"
#include "interrupts.h"
#include <time.h>
#include "soundDriver.h"
#include "registers.h"
#include "../Scheduler/include/scheduler.h"
#include <MemoryManager.h>

typedef struct time{
	uint8_t seconds, minutes, hours;
} timeStructT;

void sys_exit();
void sys_write(char * string, int length, char fd);
void sys_read(uint8_t fd, char * toRet, int cantChars);
void sys_putpixel(uint32_t x, uint32_t y, uint32_t color);
void sys_fillrectangle(uint16_t x, uint16_t y, uint32_t color, uint16_t width, uint16_t height);
void sys_sleep(uint32_t seconds);
void sys_bashflag(int flag);
void sys_microsleep(uint64_t ms);
void sys_putchar(int c);
void sys_setptrx(int num);
void sys_setptry(int num);
uint16_t sys_getvbeheight();
uint16_t sys_getvbewidth();
char sys_getchar();
void sys_clearbuffer();
int sys_getbufferpos();
void sys_hlt();
int sys_getticks();
void sys_accessRTC(timeStructT * timeStruct);
uint8_t sys_memoryAt(int dir);
void sys_setbash();
void sys_beep(int freq, int timems);
void sys_setSize(int newSize);
void sys_printRegisters();

void * sys_malloc(size_t requestedSize);
void sys_free(void * memptr);

int sys_getpid(void);
void sys_ps(void);
void sys_loop(int pid, int ms);
int sys_kill(int pid);
int sys_nice(int pid, int priority);
int sys_block(int pid);
int sys_create_process(int argc, char** argv, void (*fn)(int, char **));

#endif