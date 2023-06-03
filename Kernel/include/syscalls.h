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
#include "../Scheduler/include/process.h"
#include <MemoryManager.h>
#include "../Semaphore/include/semaphore.h"
#include "pipe.h"


#define R_W_SEMNAME "r_w_sem"

typedef struct time{
	uint8_t seconds, minutes, hours;
} timeStructT;

void initialize_sys_blocking_sem();

void sys_exit();
void sys_write(char * string, int length, char fd);
int sys_read(uint8_t fd, char * toRet, int cantChars);
void sys_putpixel(uint32_t x, uint32_t y, uint32_t color);
void sys_fillrectangle(uint16_t x, uint16_t y, uint32_t color, uint16_t width, uint16_t height);
void sys_sleep(uint32_t seconds);
void sys_bashflag(int flag);
void sys_microsleep(uint64_t ms);
void sys_setptrx(int num);
void sys_setptry(int num);
uint16_t sys_getvbeheight();
uint16_t sys_getvbewidth();
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
void sys_set_print_mode();
int sys_kill(int pid);
int sys_nice(int pid, int priority);
int sys_block(int pid);
int sys_unblock(int pid);
int sys_create_process(int argc, char** argv, void (*fn)(int, char **), int fd[2]);

int sys_waitpid(int pid);

int sys_create_sem(int initial_value, char* sem_name);
void sys_destroy_sem(int sem_index);
int sys_sem_open(char* sem_name);
int sys_sem_close(char* sem_name);
int sys_sem_wait(int sem_index);
int sys_sem_post(int sem_index);
int sys_pipe_open(char *name);
int sys_pipe_close(int pipe_index);
void sys_yield();
#endif