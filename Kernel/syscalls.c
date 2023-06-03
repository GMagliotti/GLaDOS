#include "Scheduler/include/process.h"
#include "Scheduler/include/scheduler.h"
#include "include/videoDriver.h"
#include <syscalls.h>

#define STDIN 0
#define STDOUT 0
#define STDERR 2

extern uint8_t memoryAt(int dir); 

extern MemoryManagerADT the_memory_manager;

int r_w_sem_id = 10;

void initialize_sys_blocking_sem() {
    r_w_sem_id = create_sem(0, R_W_SEMNAME);
}


void sys_exit() {
    hvdClear();
    return;
}

/* Writes the string passed for a length amount of bytes, in the specified file descriptor 
   If specified length is longer than string length, it'll only print up to the string end
   filedescriptors supported: 1 - STDOUT, 2 - STDERR
*/
void sys_write(char * string, int length, char fd) {

    process_ptr proc = current_process();

    if (proc->fd_w == 0) {
        if (current_is_foreground()) {
            for (int i = 0; i < length; i++) {
                printChar(string[i]);
            }
        }
    } else {
        write_pipe(proc->fd_w, string);
    }
}

/* Reads a specified amount of characters from the file descriptor passed, and stores
   said characters in the specified buffer toRet
   FileDescriptors supported: 0 - STDIN
*/
int sys_read(uint8_t fd, char * toRet, int cantChars) {

    int read_count = 0;
    bool null_found = false;

    process_ptr proc = current_process();

    if (proc->fd_r == 0) {
        if (current_is_foreground()){
            for (; read_count < cantChars && !null_found; read_count++) {
                sem_wait(r_w_sem_id);
                toRet[read_count] = getChar();
                if (toRet[read_count] == '\0') null_found = true;
            }
        }
    } else {
        for (; read_count < cantChars; read_count++) {
            toRet[read_count] = read_pipe(proc->fd_r);
        }
    }

    return read_count;
}


/* 
	Paints a pixel in the position specified in the color specified. 
   The format of the color argument should be 0x00RRGGBB, where each pair of letters
   indicate the opacity of that color (R red, G green, B blue)
*/
void sys_putpixel(uint32_t x, uint32_t y, uint32_t color){
    // When drawing graphics the text buffer becomes disabled
    //setTextBufferActive(false);
    if (current_is_foreground()){
        putpixel(x, y, color);
    }
    return;
}

/*
	Paints an area of pixels in the color and position specified.
	The position (or x and y values) indicate the starting vertex of the rectangle
	and it grows RIGHTWARDS in width and DOWNWARDS in height
	The format of the color argument should be 0x00RRGGBB, where each pair of letters
    indicate the opacity of that color (R red, G green, B blue).
*/
void sys_fillrectangle(uint16_t x, uint16_t y, uint32_t color, uint16_t width, uint16_t height) {
    if (current_is_foreground()){
        fillrect(x, y, color, width, height);
    }
    return;
}

void sys_setbash(){
    if(current_is_foreground()) {
        setBash();
    }
    return;
}

/* Halts the execution of instructions for a specified amount of milliseconds */
void sys_sleep(uint32_t milliseconds){
    sleepms(milliseconds);
    return;
}

void sys_setptrx(int num){
    if (current_is_foreground()) {
        setptrx(num);
    }
    return;
}

void sys_setptry(int num){
    if (current_is_foreground()) {
        setptry(num);
    }
    return;
}

uint16_t sys_getvbeheight(){
    return getVBEHeight();
}
uint16_t sys_getvbewidth(){
    return getVBEWidth();
}

void sys_clearbuffer(){

    // printString("Starting PIPE test:\n", 50);

    // int pipe_id = pipe_open("Pipe Test");

    // write_pipe(pipe_id, "Hello World!\n");
    
    // for(int i = 0; i < 13; i++) {
    //     char c = read_pipe(pipe_id);
    //     //     if (c == '\0') break;
    //     printChar(c);
    //     // }
    // }


    // printString("Ending PIPE test:\n", 50);


    // if (current_is_foreground()) {
        clearBuffer();
    // }
    // return;
}

int sys_getbufferpos(){
    return getBufferPos();
}

void sys_beep(int freq, int timems) {
    beep(freq, timems);
}

void sys_hlt(){
    _hlt();
    return;
}

int sys_getticks(){
    return ticks_elapsed(); }

void sys_accessRTC(timeStructT * timeStruct) {
    timeStruct->seconds = accessRTC(RTCSECONDS);
    timeStruct->minutes = accessRTC(RTCMINUTES);
    timeStruct->hours = accessRTC(RTCHOURS);
}

uint8_t sys_memoryAt(int dir) {
    return memoryAt(dir);
}

void sys_setSize(int newSize) {
    if (current_is_foreground()){
        setFontSize(newSize);
    }
}

void sys_printRegisters() {
    if (current_is_foreground()) {
        printCurrentRegisters(&registerDump);
    }
}

void * sys_malloc(size_t requestedMemory) {
    return allocMemory(the_memory_manager, requestedMemory);
}

void sys_free(void * memptr) {
    mman_free(the_memory_manager, memptr);
}

int sys_getpid() {
    return get_current_pid();
}

void sys_ps(void) {
    if (current_is_foreground()) {
        ps();
    }
}

void sys_set_print_mode() {
    set_print_mode();
}

int sys_kill(int pid) {
    return kill_process(pid);
}

int sys_nice(int pid, int priority) {
    return nice_process(pid, priority);
}

int sys_block(int pid) {
    return block_process(pid);
}

int sys_unblock(int pid) {
    return unblock_process(pid);
}


int sys_create_process(int argc, char** argv, void (*fn)(int, char **), int fd[2]) {
    return scheduler_create_process(argc, argv, fn, fd);
}

int sys_waitpid(int pid) {
    return scheduler_waitpid(pid);
}

int sys_create_sem(int initial_value, char* sem_name) {
    return create_sem(initial_value, sem_name);
}

void sys_destroy_sem(int sem_index) {
    destroy_sem(sem_index);
}

int sys_sem_open(char* sem_name) {
    return sem_open(sem_name);
}

int sys_sem_close(char* sem_name) {
    return sem_close(sem_name);
}

int sys_sem_wait(int sem_index) {
    return sem_wait(sem_index);
}

int sys_sem_post(int sem_index) {
    return sem_post(sem_index);
}

int sys_pipe_open(char *name) {
    return pipe_open(name);
}

int sys_pipe_close(int pipe_index) {
    return pipe_close(pipe_index);
}

void sys_print_mem() {
    print_mem();
}