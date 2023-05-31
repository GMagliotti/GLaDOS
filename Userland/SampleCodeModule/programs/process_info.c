#include "include/process_info.h"

void getpid() {
    printf("My PID: %d\n", call_to_getpid()); 
}

void ps() {
    call_to_ps();
}

void loop_process(int argc, char** argv) {

    if (argc < 2) {
        printf("Usage: LOOP <ms>\n");
        return;
    }

    int ms = string_to_int(argv[1]);

    if (ms < 0) {
        printf("Invalid sleep time\n");
        return;
    }

    int max_prints = 40;
    int i = 0;

    while( i < max_prints) {
        printf("Hello there! My PID: %d\n", call_to_getpid());
        sleepms(ms);
        i++;
    }
}

void kill_process(int argc, char** argv) {

    if (argc < 2) {
        printf("Usage: PKILL <pid>\n");
        return;
    }

    int pid = string_to_int(argv[1]);

    call_to_pkill_process(pid);
}

void nice_process(int argc, char** argv) {

    if ( argc < 3) {
        printf("Usage: NICE <pid> <new_priority>\n");
        return;
    }

    int pid = string_to_int(argv[1]);


    if (pid < 0) {
        printf("PID must be positive\n");
        return;
    }

    int priority = string_to_int(argv[2]);

    if (priority < 0 || priority > 5) {
        printf("Priority must be between 0 and 5\n");
        return;
    }

    call_to_nice_process(pid, priority);
}

void block_process(int argc, char** argv) {

    if (argc < 2) {
        printf("Usage: BLOCK <pid>\n");
        return;
    }

    int pid = string_to_int(argv[1]);

    if ( pid < 0) {
        printf("PID must be positive\n");
        return;
    }

    call_to_block_process(pid);
}