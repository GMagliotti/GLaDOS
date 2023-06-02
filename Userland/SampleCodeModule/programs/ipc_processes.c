#include "include/ipc_processes.h"


// SE NECESITAN SYSCALLS BLOQUEANTES PARA QUE ESTOS PROCESOS FUNCIONEN!!!

void cat(int argc, char *argv[]) {
    char c;
    while ((c = call_to_getchar()) != 0 ) putc(c);
}

void wc(int argc, char *argv[]) {
    int lines = 0;
    char c;
    while ((c = call_to_getchar()) != 0) {
        if (c == '\n') {
            lines++;
            printf("Amount of lines: %i\n", lines);
        }
    }
}


int is_vowel(char c) {
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
            c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U');
}


void filter(int argc, char *argv[]) {
    char c;
    while ((c = call_to_getchar()) != 0) {
        printf("entre\n");
        if (!is_vowel(c)) {
            putc(c);
            printf("%c", c);
        }
    }
    return;
}