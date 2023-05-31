#ifndef IPC_PROCESSES_H
#define IPC_PROCESSES_H

#include <stdio.h>
#include "../include/user_syscalls.h"

void cat(int argc, char *argv[]);

void wc(int argc, char *argv[]);

void filter(int argc, char *argv[]);


#endif