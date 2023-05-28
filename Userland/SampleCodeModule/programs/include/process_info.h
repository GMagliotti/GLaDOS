#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include "../include/user_syscalls.h"
#include "../include/stdio.h"

int create_process();

int getpid();
int ps();
int loop_process();
int pkill_process();
int nice_process();
int block_process();

#endif