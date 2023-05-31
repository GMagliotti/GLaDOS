#include "include/process_info.h"

int count = 0;

void bofa(int argc, char** argv) {
    printf("Arg count: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("Arg %d: %s\n", i, argv[i]);
    }
    // ps();
}

void create_process(void) {
    int argc = 2;
    char* argv[2] = {"Bofa!!", "Segundo arg"};
    // void (*fn)(int, char **) = 0;
    call_to_create_process(argc, argv, bofa);
}

void getpid() {
    printf("My PID: %d\n", call_to_getpid()); 
}

void ps() {
    call_to_ps();
}

void loop_process(void) {
    call_to_loop_process(call_to_getpid(), 500);
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