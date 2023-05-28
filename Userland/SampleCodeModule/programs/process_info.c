#include "include/process_info.h"


void bofa(int argc, char** argv) {
    printf("Bofa deez nuts\n");
}

void create_process(void) {
    char* name = "Bofa";
    int argc = 0;
    char** argv = 0;
    // void (*fn)(int, char **) = 0;
    int visibility = 1;
    call_to_create_process(name, argc, argv, bofa, visibility);
}

void getpid() {
    printf("My PID: %d\n", call_to_getpid()); 
}

void ps() {
    call_to_ps();
}

void loop_process(void) {
    call_to_loop_process(call_to_getpid(), 1000);
}

void kill_process(int pid) {
    call_to_pkill_process(pid);
}

void nice_process(int pid) {
    int priority = 2;
    call_to_nice_process(pid, priority);
}

void block_process(int pid) {
    call_to_block_process(pid);
}