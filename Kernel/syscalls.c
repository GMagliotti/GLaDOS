#include <syscalls.h>
#include <MemoryManager.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

extern uint8_t memoryAt(int dir); 

extern MemoryManagerADT the_memory_manager;

void sys_exit() {
    hvdClear();
    return;
}

/* Writes the string passed for a length amount of bytes, in the specified file descriptor 
   If specified length is longer than string length, it'll only print up to the string end
   filedescriptors supported: 1 - STDOUT, 2 - STDERR
*/
void sys_write(char * string, int length, char fd) {
    switch (fd)
    {
    case STDERR:
        printColorString(string, length, RED);
        break;
    case STDOUT:
        printString(string, length);
        break;
    default:    ;
        char errorMessage[] = "Error: Invalid file descriptor";
        sys_write(errorMessage, strLength(errorMessage), STDERR);
        break;
    }
}

/* Reads a specified amount of characters from the file descriptor passed, and stores
   said characters in the specified buffer toRet
   FileDescriptors supported: 0 - STDIN
*/
void sys_read(uint8_t fd, char * toRet, int cantChars) {
    switch (fd)
    {
    case STDIN:     ;
        int j=0;
        int bufferPos=getBufferPos();
        for(int i=bufferPos; i < cantChars+bufferPos;){
            if(bufferAt(i)!=0){
                toRet[j] = bufferAt(i);
                printString(toRet+j, 1);
                i++;
                j++;
            }
            _hlt();
        }
        break;
    default:    ;
        printNumber(fd, 10);
        char errorMessage[] = "Error: Invalid file descriptor";
        sys_write(errorMessage, strLength(errorMessage), STDERR);
        break;
    }
   
}


/* 
	Paints a pixel in the position specified in the color specified. 
   The format of the color argument should be 0x00RRGGBB, where each pair of letters
   indicate the opacity of that color (R red, G green, B blue)
*/
void sys_putpixel(uint32_t x, uint32_t y, uint32_t color){
    // When drawing graphics the text buffer becomes disabled
    //setTextBufferActive(false);
    putpixel(x, y, color);
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
    fillrect(x, y, color, width, height);
    return;
}

void sys_setbash(){
    setBash();
    return;
}

/* Halts the execution of instructions for a specified amount of milliseconds */
void sys_sleep(uint32_t milliseconds){
    sleepms(milliseconds);
    return;
}


void sys_putchar(int c){
    printChar(c);
    return;
}

void sys_setptrx(int num){
    setptrx(num);
    return;
}

void sys_setptry(int num){
    setptry(num);
    return;
}

uint16_t sys_getvbeheight(){
    return getVBEHeight();
}
uint16_t sys_getvbewidth(){
    return getVBEWidth();
}

char sys_getchar(){
    return getChar();
}

void sys_clearbuffer(){
    clearBuffer();
    return;
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
    setFontSize(newSize);
}

void sys_printRegisters() {
    printCurrentRegisters(&registerDump);
}

void * sys_malloc(size_t requestedMemory) {
    return allocMemory(the_memory_manager, requestedMemory);
}

void sys_free(void * memptr) {
    mman_free(the_memory_manager, memptr);
}

int sys_getpid() {
    //return current_pid();
    return 0;
}

void sys_ps() {
    // ps();
    return;
}

void sys_loop(int pid, int ms) {
    //loop_process(pid, ms);
    return;
}

int sys_kill(int pid) {
    //return kill_process(pid);
    return 0;
}

int sys_nice(int pid, int priority) {
    //return nice_process(pid, priority);
    return 0;
}

void sys_block(int pid) {
    //block_process(pid);
    return;
}

int create_process(char* name) {
    return 0;
}

void sys_foreground(int pid) {
    //foreground_process(pid);
    return;
}

