#include "include/process_info.h"



void bofa(int argc, char** argv) {
    printf("Bofa deez nuts\n");
}

int create_process(void) {
    char* name = "Bofa";
    int argc = 0;
    char** argv = 0;
    // void (*fn)(int, char **) = 0;
    int visibility = 1;
    return call_to_create_process(name, argc, argv, bofa, visibility);
}

int getpid() {
    printf("My PID: %d\n", call_to_getpid()); 
    return 1;
}

int ps() {
    return call_to_ps();
}

int loop_process(void) {
    return call_to_loop_process(call_to_getpid(), 1000);
}

int pkill_process(void) {
    return call_to_pkill_process(call_to_getpid());
}

int nice_process(void) {
    int priority = 2;
    return call_to_nice_process(call_to_getpid(), priority);
}

int block_process() {
    return call_to_block_process(call_to_getpid());
}

int to_background_process() {
    call_to_background_process(call_to_getpid());
    return 0;
}

int to_foreground_process() {
    call_to_foreground_process(call_to_getpid());
    return 0;
}